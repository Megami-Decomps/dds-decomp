#!/usr/bin/env python3
"""Report exact, source-owned production code; retain raw objdiff as diagnostics.

A retail checksum alone does not identify C.  Every credited function must also
have compiler provenance, an unambiguous production-object span, a link-map and
linked-symbol owner, and exact linked bytes.  A hash receipt binds the evidence
to the actual link inputs.  No missing evidence falls back to source inventory.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import math
from pathlib import Path
import re
import shlex
import struct

from check_unit import address, inline_asm_share
from pairing import ROOT, load_segments

VERSIONS = json.loads((ROOT / "config/versions.json").read_text())


def percent(numerator: int, denominator: int) -> float:
    return round(100.0 * numerator / denominator, 6) if denominator else 0.0


def set_code_measures(measures: dict, code: int, functions: int,
                      fuzzy_points: float | None = None) -> None:
    """Replace code measures with totals derived from the verified providers."""
    total_code = int(measures.get("total_code", 0))
    total_functions = int(measures.get("total_functions", 0))
    fuzzy_percent = percent(code, total_code)
    if fuzzy_points is not None:
        fuzzy_percent = round(fuzzy_points / total_code, 6) if total_code else 0.0
    measures.update(
        matched_code=str(code), matched_code_percent=percent(code, total_code),
        matched_functions=functions,
        matched_functions_percent=percent(functions, total_functions),
        fuzzy_match_percent=fuzzy_percent,
    )


def digest(path: Path) -> str:
    return hashlib.sha256(path.read_bytes()).hexdigest()


def compiler_functions(assembly: str) -> set[str]:
    """Read cc1 function declarations, never declarations supplied by inline asm."""
    names: set[str] = set()
    app = False
    current = None
    for line in assembly.splitlines():
        token = line.strip()
        if token in ("#APP", "#NO_APP"):
            app = token == "#APP"
        elif not app and (match := re.fullmatch(r"\.ent\s+(\S+)", token)):
            name = match[1]
            if name in names or current is not None:
                raise ValueError(f"duplicate/nested compiler function: {name}")
            names.add(name)
            current = name
        elif not app and (match := re.fullmatch(r"\.end\s+(\S+)", token)):
            if match[1] != current:
                raise ValueError("unpaired compiler function end")
            current = None
    # cc1 legitimately leaves its final top-level #APP region open at EOF.
    # A compiler-owned function itself must still have a matching .end.
    if current is not None:
        raise ValueError("unterminated compiler assembly provenance")
    return names


def ineligible_asm_bodies(assembly: str, source: str) -> set[str]:
    """Apply the same ASMBODY and documented VU exemptions as check_unit."""
    result = set()
    for name, (asm, total) in inline_asm_share(assembly).items():
        source_name = re.sub(r"\.\d+$", "", name)
        if asm < 8 or asm * 2 <= total:
            continue
        if re.search(rf"(?:libvu0|vu0 routine):[^\n]*\n(?:[^\n]*\n){{0,2}}"
                     rf"[^\n]*\b{re.escape(source_name)}\s*\(", source):
            continue
        result.add(name)
    return result


def parse_link_map(text: str) -> dict[str, tuple[int, int]]:
    if "Discarded input sections" in text:
        _, marker, text = text.partition("Linker script and memory map")
        if not marker:
            raise ValueError("missing live link map")
    spans = {}
    for match in re.finditer(r"^ \.text\s+(0x[0-9a-fA-F]+)\s+(0x[0-9a-fA-F]+)\s+(\S+\.o)\s*$", text, re.M):
        start, size, path = int(match[1], 16), int(match[2], 16), match[3]
        if size == 0:  # Empty discarded sections do not supply any code.
            continue
        if path in spans:
            raise ValueError(f"duplicate text contribution: {path}")
        spans[path] = (start, size)
    if not spans:
        raise ValueError("missing link-map text contributions")
    ranges = sorted((start, start + size, name) for name, (start, size) in spans.items())
    for left, right in zip(ranges, ranges[1:]):
        if left[1] > right[0]:
            raise ValueError("overlapping link-map text contributions")
    return spans


def validate_partition(report_functions: dict, production_functions: dict,
                       c_names: set[str], fallback: set[str]) -> None:
    if c_names & fallback or c_names | fallback != set(report_functions):
        raise ValueError("C/fallback functions do not partition the report")
    if set(production_functions) != set(report_functions):
        raise ValueError("production/report function sets differ")
    spans = []
    for name, (offset, size) in production_functions.items():
        if size <= 0 or size % 4 or offset < 0 or offset % 4:
            raise ValueError(f"{name}: invalid function span")
        if int(report_functions[name]["size"]) != size:
            raise ValueError(f"{name}: production/report size differs")
        spans.append((offset, offset + size, name))
    for left, right in zip(sorted(spans), sorted(spans)[1:]):
        if left[1] > right[0]:
            raise ValueError(f"overlapping function spans: {left[2]}, {right[2]}")


def mapped_bytes(data: bytes, segments, address: int, size: int) -> bytes:
    if size <= 0 or address < 0:
        raise ValueError("invalid byte-proof span")
    matches = [(offset + address - vma, size) for vma, offset, length in segments
               if vma <= address and address + size <= vma + length]
    if len(matches) != 1:
        raise ValueError("unmapped or ambiguous byte-proof span")
    offset, size = matches[0]
    if offset < 0 or offset + size > len(data):
        raise ValueError("truncated byte-proof input")
    return data[offset:offset + size]


def verify_bytes(linked: bytes, segments, retail: bytes, retail_segments,
                 address: int, size: int) -> None:
    if mapped_bytes(linked, segments, address, size) != mapped_bytes(retail, retail_segments, address, size):
        raise ValueError(f"linked bytes differ at 0x{address:X}")


def verify_linked_image(elf: bytes, image: bytes) -> None:
    """Prove objcopy's complete file-backed load image, including data/literals.

    The matching linker script uses contiguous physical addresses for the
    original ELF header, main image, and trailer. NOBITS has no file bytes.
    """
    phoff = struct.unpack_from("<I", elf, 28)[0]
    phentsize, count = struct.unpack_from("<HH", elf, 42)
    if phentsize != 32 or not count or phoff + count * 32 > len(elf):
        raise ValueError("invalid production load segments")
    spans = []
    for index in range(count):
        kind, offset, _, physical, size, *_ = struct.unpack_from("<8I", elf, phoff + index * 32)
        if kind != 1 or size == 0:
            continue
        if offset + size > len(elf) or physical + size > len(image):
            raise ValueError("truncated production load image")
        if elf[offset:offset + size] != image[physical:physical + size]:
            raise ValueError("linked ELF and production image differ")
        spans.append((physical, physical + size))
    at = 0
    for start, end in sorted(spans):
        if start != at:
            raise ValueError("noncontiguous/overlapping production load image")
        at = end
    if at != len(image):
        raise ValueError("incomplete production load image")


class Elf:
    """Small ELF32-LE reader, so reporting tests need no optional packages."""
    def __init__(self, path: Path):
        self.data = path.read_bytes()
        if self.data[:6] != b"\x7fELF\x01\x01" or len(self.data) < 52:
            raise ValueError(f"not ELF32 little-endian: {path}")
        shoff = struct.unpack_from("<I", self.data, 32)[0]
        shentsize, shnum, shstr = struct.unpack_from("<HHH", self.data, 46)
        if shentsize != 40 or not shnum or shstr >= shnum or shoff + shnum * 40 > len(self.data):
            raise ValueError(f"invalid ELF section table: {path}")
        raw = [struct.unpack_from("<10I", self.data, shoff + i * 40) for i in range(shnum)]
        names = raw[shstr]
        strings = self.data[names[4]:names[4] + names[5]]
        self.sections = [(self.string(strings, s[0]), s) for s in raw]
        self.functions = []
        for _, section in self.sections:
            if section[1] != 2:
                continue
            if section[9] != 16 or section[6] >= shnum:
                raise ValueError("invalid ELF symbol table")
            st = raw[section[6]]
            strs = self.data[st[4]:st[4] + st[5]]
            for off in range(section[4], section[4] + section[5], 16):
                name, value, size, info, _, index = struct.unpack_from("<IIIBBH", self.data, off)
                if info & 15 == 2 and index not in (0, 0xFFF1) and size:
                    self.functions.append((self.string(strs, name), value, size, index))

    @staticmethod
    def string(data: bytes, at: int) -> str:
        return data[at:data.index(b"\0", at)].decode()

    def text_functions(self) -> dict[str, tuple[int, int]]:
        result = {}
        for name, value, size, index in self.functions:
            if index >= len(self.sections) or self.sections[index][0] != ".text":
                continue
            if name in result:
                raise ValueError(f"duplicate object function: {name}")
            result[name] = (value, size)
        return result


def record_object(root: Path, source: Path, obj: Path) -> None:
    """Bind compiler ownership to the object in the successful compile recipe."""
    match = re.fullmatch(r"build/(dds[12])/(?:base/)?(src/\1/.+)\.o", str(obj))
    if match is None or str(source) != match[2] + ".c":
        raise ValueError("compile object/source identity differs")
    dependency_text = (root / (str(obj) + ".d")).read_text().replace("\\\n", " ")
    target, separator, dependencies = dependency_text.partition(":")
    if not separator or target.strip() != str(obj):
        raise ValueError("invalid compiler dependency file")
    paths = {str(obj), str(obj) + ".s", str(source), *shlex.split(dependencies)}
    assembly = (root / (str(obj) + ".s")).read_text()
    for included in re.findall(r'^\s*\.include "([^"\n]+)"\s*$', assembly, re.M):
        paths.add("include/macro.inc" if included == "macro.inc" else included)
    version = source.parts[1]
    paths.update((f"config/{version}/cflags.txt", "include/macro.inc"))
    receipt = {"schema": 1, "source": str(source), "object": str(obj),
               "files": {p: digest(root / p) for p in sorted(paths)}}
    write_json(root / (str(obj) + ".proof.json"), receipt)


def write_json(path: Path, value: dict) -> None:
    temporary = path.with_suffix(".tmp")
    temporary.write_text(json.dumps(value, indent=2) + "\n")
    temporary.replace(path)


def check_hashes(root: Path, files: dict) -> None:
    for name, expected in files.items():
        if Path(name).is_absolute() or ".." in Path(name).parts:
            raise ValueError("invalid provenance path")
        if digest(root / name) != expected:
            raise ValueError(f"stale provenance input: {name}")


def read_object_receipt(root: Path, obj: str) -> dict:
    receipt = json.loads((root / (obj + ".proof.json")).read_text())
    source = receipt.get("source")
    if receipt.get("schema") != 1 or receipt.get("object") != obj or not source:
        raise ValueError("wrong compile receipt identity")
    match = re.fullmatch(r"build/(dds[12])/(?:base/)?(src/\1/.+)\.o", obj)
    if match is None or source != match[2] + ".c":
        raise ValueError("compile object/source identity differs")
    files = receipt.get("files", {})
    if not {obj, obj + ".s", source} <= files.keys():
        raise ValueError("missing compile receipt inputs")
    check_hashes(root, files)
    return receipt


def record_link(root: Path, elf_path: Path, map_path: Path, objects: list[str]) -> None:
    """Run only in the successful link recipe, after ld has consumed its inputs."""
    paths = {str(elf_path), str(map_path), *objects}
    for obj in objects:
        if re.fullmatch(r"build/(dds[12])/(src/\1/.+)\.o", obj):
            compiled = read_object_receipt(root, obj)
            paths.update(compiled["files"])
            paths.add(obj + ".proof.json")
    paths.add("config/versions.json")
    for version in VERSIONS:
        paths.add(f"config/{version}/symbol_addrs.txt")
    receipt = {"schema": 1, "elf": str(elf_path), "map": str(map_path),
               "objects": objects, "files": {p: digest(root / p) for p in sorted(paths)}}
    write_json(root / (str(elf_path) + ".proof.json"), receipt)


def read_receipt(root: Path, elf_path: Path, map_path: Path, receipt_path: Path) -> dict:
    receipt = json.loads((root / receipt_path).read_text())
    if receipt.get("schema") != 1 or receipt.get("elf") != str(elf_path) or receipt.get("map") != str(map_path):
        raise ValueError("wrong link receipt identity")
    files = receipt.get("files", {})
    required = {str(elf_path), str(map_path), *receipt.get("objects", [])}
    if not receipt.get("objects") or len(set(receipt["objects"])) != len(receipt["objects"]) or not required <= files.keys():
        raise ValueError("missing link receipt inputs")
    check_hashes(root, files)
    for obj in receipt["objects"]:
        if re.fullmatch(r"build/(dds[12])/(src/\1/.+)\.o", obj):
            compiled = read_object_receipt(root, obj)
            if not {obj + ".proof.json", *compiled["files"]} <= files.keys():
                raise ValueError("missing compile proof in link receipt")
    return receipt


class Production:
    def __init__(self, root: Path, version: str):
        self.root, self.version = root, version
        spec = VERSIONS[version]
        serial = spec["serial"]
        self.elf_path = Path(f"build/{version}/{serial}.elf")
        map_path = self.elf_path.with_suffix(".map")
        self.receipt = read_receipt(root, self.elf_path, map_path, Path(str(self.elf_path) + ".proof.json"))
        self.elf = Elf(root / self.elf_path)
        self.segments = load_segments(self.elf.data)
        self.map = parse_link_map((root / map_path).read_text())
        self.linked_symbols = {(n, a, s) for n, a, s, _ in self.elf.functions}
        self.retail = (root / f"orig/{version}/{serial}").read_bytes()
        self.retail_segments = load_segments(self.retail)
        image = (root / f"build/{version}/{serial}").read_bytes()
        if hashlib.sha1(self.retail).hexdigest() != spec["elf_sha1"] or image != self.retail:
            raise ValueError(f"{version}: production image does not match pinned retail")
        verify_linked_image(self.elf.data, image)
        self.syms = {}
        for line in (root / f"config/{version}/symbol_addrs.txt").read_text().splitlines():
            if match := re.match(r"^\s*(\S+)\s*=\s*0x([0-9A-Fa-f]+)\s*;", line):
                self.syms.setdefault(match[1], int(match[2], 16))

    def unit(self, unit: dict, name: str) -> dict:
        obj = f"build/{self.version}/src/{self.version}/{name}.o"
        source = f"src/{self.version}/{name}.c"
        assembly_path = obj + ".s"
        if obj not in self.receipt["objects"] or not {source, assembly_path} <= self.receipt["files"].keys():
            raise ValueError(f"{name}: missing compiled source provenance")
        assembly = (self.root / assembly_path).read_text()
        c_emitted = compiler_functions(assembly)
        blocked = ineligible_asm_bodies(assembly, (self.root / source).read_text())
        production = Elf(self.root / obj).text_functions()
        start, text_size = self.map[obj]
        entries = unit.get("functions", [])
        report = {entry["name"]: entry for entry in entries}
        if len(report) != len(entries):
            raise ValueError(f"{name}: duplicate report functions")
        if sum(int(e["size"]) for e in entries) != int(unit["measures"]["total_code"]) or len(entries) != int(unit["measures"]["total_functions"]):
            raise ValueError(f"{name}: function inventory does not equal report denominator")
        raw_scores = [float(entry.get("fuzzy_match_percent", 0)) for entry in entries]
        if any(not math.isfinite(score) or not 0 <= score <= 100 for score in raw_scores):
            raise ValueError(f"{name}: invalid raw function score")
        raw_code = sum(int(entry["size"]) for entry, score in zip(entries, raw_scores) if score == 100)
        raw_functions = raw_scores.count(100)
        if raw_code != int(unit["measures"].get("matched_code", 0)) or raw_functions != int(unit["measures"].get("matched_functions", 0)):
            raise ValueError(f"{name}: inconsistent raw exact totals")
        raw_fuzzy = float(unit["measures"].get("fuzzy_match_percent", 0))
        total_code = int(unit["measures"].get("total_code", 0))
        weighted = sum(int(entry["size"]) * score for entry, score in zip(entries, raw_scores))
        if not math.isfinite(raw_fuzzy) or not total_code or abs(raw_fuzzy - weighted / total_code) > 0.0001:
            raise ValueError(f"{name}: inconsistent raw fuzzy total")
        # The disassembler can group several real C functions under one retail
        # label. Prove the complete interval, not just its first named symbol.
        report_spans = []
        for target, entry in report.items():
            retail_address = address(target, self.syms)
            size = int(entry["size"])
            if retail_address is None or size <= 0 or size % 4:
                raise ValueError(f"{name}:{target}: invalid retail span")
            report_spans.append((retail_address - start, size, target))
        included = set(re.findall(r'^\s*\.include "build/eeasm/asm/' + self.version +
                                 r'/nonmatchings/[^"\n]+/([^/"\n]+)\.s"\s*$', assembly, re.M))
        if c_emitted - production.keys():
            raise ValueError(f"{name}: compiler functions absent from production object")
        if (set(production) - c_emitted) - included or included & c_emitted:
            raise ValueError(f"{name}: fallback lacks assembly-include provenance")
        providers = {}
        used = set()
        canonical = {}
        c_names = set()
        for offset, size, target in sorted(report_spans):
            owners = sorted((at, span, emitted) for emitted, (at, span) in production.items()
                            if offset <= at < offset + size)
            at = offset
            for owner_at, owner_size, emitted in owners:
                if owner_at != at or owner_size <= 0 or owner_size % 4 or emitted in used:
                    raise ValueError(f"{name}:{target}: overlapping/incomplete production providers")
                owner_address = start + owner_at
                plain_name = re.sub(r"\.\d+$", "", emitted)
                if address(plain_name, self.syms) != owner_address:
                    raise ValueError(f"{name}:{emitted}: production/retail address differs")
                if (emitted, owner_address, owner_size) not in self.linked_symbols:
                    raise ValueError(f"{name}:{emitted}: missing linked symbol ownership")
                if owner_at == offset and plain_name != target:
                    raise ValueError(f"{name}:{target}: ambiguous production symbol")
                used.add(emitted)
                at += owner_size
            if not owners or at != offset + size or offset < 0 or at > text_size:
                raise ValueError(f"{name}:{target}: production providers do not tile retail span")
            ownership = {emitted in c_emitted for _, _, emitted in owners}
            if len(ownership) != 1:
                raise ValueError(f"{name}:{target}: mixed C/ASM report span; fix retail boundaries")
            if True in ownership:
                c_names.add(target)
            canonical[target] = (offset, size)
            providers[target] = [emitted for _, _, emitted in owners]
        if used != set(production):
            raise ValueError(f"{name}: unreported production functions")
        fallback = set(report) - c_names
        validate_partition(report, canonical, c_names, fallback)
        evidence = []
        matched_code = matched_functions = 0
        for target, entry in report.items():
            emitted = providers[target]
            offset, size = canonical[target]
            verify_bytes(self.elf.data, self.segments, self.retail, self.retail_segments, start + offset, size)
            eligible = target in c_names and not blocked.intersection(emitted)
            previous = float(entry.get("fuzzy_match_percent", 0.0))
            score = 100.0 if eligible else 0.0
            if eligible:
                matched_code += size
                matched_functions += 1
            entry["fuzzy_match_percent"] = score
            evidence.append({"name": target, "production_symbols": emitted,
                             "address": start + offset, "size": size,
                             "owner": "c" if eligible else "asm_body" if target in c_names else "include_asm",
                             "raw_fuzzy_match_percent": previous,
                             "production_exact": eligible})
        set_code_measures(unit["measures"], matched_code, matched_functions)
        for section in unit.get("sections", []):
            if section["name"] == ".text":
                section["fuzzy_match_percent"] = unit["measures"]["fuzzy_match_percent"]
        return {"unit": name, "version": self.version, "functions": evidence}


def reconcile(report: dict, scope: str, root: Path = ROOT, *,
              expected_versions: list[str] | None = None) -> dict:
    if expected_versions is None:
        if scope == "all":
            raise ValueError("combined report requires configured versions")
        expected_versions = [scope]
    expected = set(expected_versions)
    if not expected or len(expected) != len(expected_versions) or not expected <= VERSIONS.keys():
        raise ValueError("invalid configured report versions")
    if scope != "all" and expected != {scope}:
        raise ValueError("configured versions do not match report scope")
    if scope == "all":
        observed = {unit["name"].split("/", 1)[0] for unit in report["units"]
                    if "game" in unit.get("metadata", {}).get("progress_categories", [])
                    and int(unit["measures"].get("total_code", 0))}
        if observed != expected:
            raise ValueError("report versions do not match configured versions")
    names = [unit["name"] for unit in report["units"]]
    if len(set(names)) != len(names):
        raise ValueError("duplicate report units")
    proofs = {}
    proven = set()
    evidence = {"schema": 1, "metric": "production_exact_source", "units": []}
    for unit in report["units"]:
        if "game" not in unit.get("metadata", {}).get("progress_categories", []):
            continue
        measures = unit["measures"]
        if not int(measures.get("total_code", 0)):
            if unit.get("functions") or int(measures.get("total_functions", 0)) or int(measures.get("matched_code", 0)) or int(measures.get("matched_functions", 0)):
                raise ValueError("inconsistent zero-code report unit")
            # Data-only/linker metadata units are in the game category but
            # contribute no functions or bytes to the game-code denominator.
            continue
        if scope == "all":
            version, name = unit["name"].split("/", 1)
        else:
            version, name = scope, unit["name"]
        if version not in VERSIONS:
            raise ValueError(f"unknown report version: {version}")
        if not (root / f"src/{version}/{name}.c").is_file():
            # Primary scope is deliberately limited to the C reconstruction
            # target.  An unexpected unit is a configuration error, not credit.
            raise ValueError(f"missing source for game report unit: {version}/{name}")
        if version not in proofs:
            proofs[version] = Production(root, version)
        item = proofs[version].unit(unit, name)
        proven.add(unit["name"])
        evidence["units"].append(item)
    if not proven:
        raise ValueError("report contains no game-code production evidence")
    for version, production in proofs.items():
        prefix = f"build/{version}/src/{version}/"
        expected = {path[len(prefix):-2] for path in production.map if path.startswith(prefix)}
        actual = {unit["unit"] for unit in evidence["units"] if unit["version"] == version}
        if actual != expected:
            missing = ", ".join(sorted(expected - actual))
            raise ValueError(f"{version}: report/production unit inventory differs; missing: {missing}")
    def aggregate(measures: dict, units: list[dict]) -> None:
        code = sum(int(u["measures"].get("total_code", 0)) for u in units)
        functions = sum(int(u["measures"].get("total_functions", 0)) for u in units)
        if code != int(measures.get("total_code", 0)) or functions != int(measures.get("total_functions", 0)):
            raise ValueError("aggregate report denominator does not equal its units")
        matched = sum(int(u["measures"].get("matched_code", 0)) for u in units)
        matched_functions = sum(int(u["measures"].get("matched_functions", 0)) for u in units)
        # Do not accumulate rounding error across hundreds of corrected units.
        points = sum(100 * int(u["measures"].get("matched_code", 0)) if u["name"] in proven
                     else int(u["measures"].get("total_code", 0)) * float(u["measures"].get("fuzzy_match_percent", 0))
                     for u in units if int(u["measures"].get("total_code", 0)))
        set_code_measures(measures, matched, matched_functions, points)
    for category in report["categories"]:
        units = [u for u in report["units"] if category["id"] in u.get("metadata", {}).get("progress_categories", [])]
        aggregate(category["measures"], units)
    aggregate(report["measures"], report["units"])
    evidence["retail_sha1"] = {v: VERSIONS[v]["elf_sha1"] for v in proofs}
    return evidence


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    sub = parser.add_subparsers(dest="command", required=True)
    obj = sub.add_parser("record-object")
    obj.add_argument("source", type=Path)
    obj.add_argument("object", type=Path)
    link = sub.add_parser("record-link")
    link.add_argument("elf", type=Path)
    link.add_argument("map", type=Path)
    link.add_argument("objects", nargs="+")
    report = sub.add_parser("report")
    report.add_argument("input", type=Path)
    report.add_argument("output", type=Path)
    report.add_argument("--scope", choices=("all", *VERSIONS), required=True)
    report.add_argument("--version", action="append", choices=tuple(VERSIONS), required=True,
                        help="Configured game; repeat for a combined report")
    args = parser.parse_args()
    try:
        if args.command == "record-object":
            record_object(ROOT, args.source, args.object)
        elif args.command == "record-link":
            record_link(ROOT, args.elf, args.map, args.objects)
        else:
            value = json.loads(args.input.read_text())
            proof = reconcile(value, args.scope, expected_versions=args.version)
            args.output.write_text(json.dumps(value, indent=2) + "\n")
            Path(str(args.output) + ".proof.json").write_text(json.dumps(proof, indent=2) + "\n")
    except (ValueError, KeyError, OSError, struct.error) as error:
        raise SystemExit(f"production report proof failed: {error}") from error


if __name__ == "__main__":
    main()
