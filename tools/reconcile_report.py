#!/usr/bin/env python3
"""Correct bounded objdiff report entries after linked-word verification.

Objdiff compares relocatable objects.  A source object can therefore differ
from an assembly target solely because equivalent addresses or literal values
use different relocation representations.  This postprocessor grants exact
credit only to explicitly configured source-owned functions whose words agree
after resolving the source object against the retail executable.
"""
import argparse
import json
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from check_unit import (  # noqa: E402
    BIN,
    VERSIONS,
    address,
    relocations,
    run,
    sections,
    symbols,
    text_section,
)
from eeas_compat import lit4_range  # noqa: E402
from pairing import RETAIL, ROOT, load_segments, va_to_off  # noqa: E402
from resolved_code import (  # noqa: E402
    hi_lo_addend,
    instruction_shape_matches,
    linked_gprel_field,
    linked_hi_field,
    linked_jump_field,
    linked_lo_field,
    literal_pool_offset,
    retail_literal_address,
    sext16,
)

MANIFEST = ROOT / "config/report_reconciliations.json"


def object_functions(obj: Path) -> dict[str, tuple[int, int]]:
    output = run(str(BIN / "mips-ps2-decompals-nm"), "-S", "--defined-only", str(obj))
    return {
        parts[3]: (int(parts[0], 16), int(parts[1], 16))
        for parts in (line.split() for line in output.splitlines())
        if len(parts) == 4 and parts[2] in "Tt"
    }


def linked_function_diffs(
    name: str,
    function: tuple[int, int],
    text: bytes,
    relocs: dict[int, tuple[str, str]],
    lit4: bytes,
    object_gp: int,
    funcs: dict[str, tuple[int, int]],
    syms: dict[str, int],
    retail: bytes,
    segments,
    gp: int,
    retail_lit4: tuple[int, int],
) -> list[str]:
    """Return fail-closed linked-word differences for one object function."""
    offset, size = function
    retail_address = address(name, syms)
    if retail_address is None:
        return [f"{name}: no retail address"]
    retail_offset = va_to_off(segments, retail_address)
    if retail_offset is None:
        return [f"{name}: retail address is not mapped"]

    pending_hi = {}
    diffs = []
    for at in range(0, size, 4):
        mine = struct.unpack_from("<I", text, offset + at)[0]
        want = struct.unpack_from("<I", retail, retail_offset + at)[0]
        relocation = relocs.get(offset + at)
        if relocation is None:
            if mine != want:
                diffs.append(f"+0x{at:X}: {mine:08X} != {want:08X}")
            continue

        rtype, symbol = relocation
        base = symbol.split("+", 1)[0]
        if not instruction_shape_matches(mine, want, rtype):
            diffs.append(f"+0x{at:X}: instruction shape differs for {rtype} {symbol}")
            continue

        target = address(base, syms)
        if target is not None and retail_lit4[0] <= target < retail_lit4[1]:
            diffs.append(f"+0x{at:X}: named literal-pool address {symbol}")
            continue

        if base == ".lit4":
            literal_offset = literal_pool_offset(mine, object_gp)
            ours = struct.unpack_from("<I", lit4, literal_offset)[0] \
                if 0 <= literal_offset <= len(lit4) - 4 else None
            retail_literal = retail_literal_address(want, gp)
            mapped = va_to_off(segments, retail_literal)
            theirs = struct.unpack_from("<I", retail, mapped)[0] if mapped is not None else None
            if ours is None or ours != theirs:
                diffs.append(
                    f"+0x{at:X}: literal {ours!r} != retail {theirs!r}"
                )
            continue

        if base == ".text" and rtype == "R_MIPS_26":
            object_target = (mine & 0x03FFFFFF) << 2
            owner = next(
                ((start, span, owner_name) for owner_name, (start, span) in funcs.items()
                 if start <= object_target < start + span),
                None,
            )
            owner_address = address(owner[2], syms) if owner else None
            expected = None if owner_address is None else \
                ((owner_address + object_target - owner[0]) >> 2) & 0x03FFFFFF
            if expected is None or want & 0x03FFFFFF != expected:
                diffs.append(f"+0x{at:X}: local text target differs")
            continue

        if target is None:
            diffs.append(f"+0x{at:X}: unresolved {rtype} {symbol}")
            continue

        addend = mine & 0x03FFFFFF if rtype == "R_MIPS_26" else sext16(mine)
        if rtype == "R_MIPS_26":
            good = want & 0x03FFFFFF == linked_jump_field(target, addend)
        elif rtype == "R_MIPS_HI16":
            pending_hi.setdefault(base, []).append((at, mine, want, symbol))
            good = True
        elif rtype == "R_MIPS_LO16":
            good = want & 0xFFFF == linked_lo_field(target, addend)
            for hi_at, hi_mine, hi_want, hi_symbol in pending_hi.pop(base, []):
                combined_addend = hi_lo_addend(hi_mine, mine)
                if hi_want & 0xFFFF != linked_hi_field(target, combined_addend):
                    diffs.append(
                        f"+0x{hi_at:X}: HI16 address differs for {hi_symbol}"
                    )
        elif rtype == "R_MIPS_GPREL16":
            good = want & 0xFFFF == linked_gprel_field(target, addend, gp)
        else:
            good = False
        if not good:
            diffs.append(f"+0x{at:X}: linked target differs for {rtype} {symbol}")

    for pending in pending_hi.values():
        for at, _, _, symbol in pending:
            diffs.append(f"+0x{at:X}: unpaired HI16 {symbol}")
    return diffs


def percent(numerator: int, denominator: int) -> float:
    return round(100.0 * numerator / denominator, 6) if denominator else 0.0


def validate_function_mask(
    report_functions: dict[str, dict],
    funcs: dict[str, tuple[int, int]],
    fallback: dict[str, int],
) -> None:
    """Require the object-defined C set and configured fallback set to partition a unit."""
    actual_fallback = {
        name: int(report_functions[name]["size"])
        for name in report_functions.keys() - funcs.keys()
    }
    if actual_fallback != fallback:
        raise ValueError("source-owned/fallback mask changed")
    for name in fallback:
        if float(report_functions[name].get("fuzzy_match_percent", 0.0)) != 0.0:
            raise ValueError(f"{name}: fallback function has nonzero C credit")
    if set(funcs) != report_functions.keys() - fallback.keys():
        raise ValueError("source object and report function sets differ")
    for name, (_, size) in funcs.items():
        if int(report_functions[name]["size"]) != size:
            raise ValueError(f"{name}: object/report size differs")


def apply_delta(measures: dict, code: int, functions: int, fuzzy_points: float) -> None:
    total_code = int(measures["total_code"])
    total_functions = int(measures["total_functions"])
    measures["matched_code"] = str(int(measures.get("matched_code", "0")) + code)
    measures["matched_code_percent"] = percent(int(measures["matched_code"]), total_code)
    measures["matched_functions"] = int(measures.get("matched_functions", 0)) + functions
    measures["matched_functions_percent"] = percent(
        measures["matched_functions"], total_functions
    )
    measures["fuzzy_match_percent"] = round(
        float(measures.get("fuzzy_match_percent", 0.0)) + fuzzy_points / total_code,
        6,
    )


def reconcile_unit(report: dict, unit: dict, spec: dict) -> tuple[int, int, float]:
    report_functions = {entry["name"]: entry for entry in unit["functions"]}
    fallback = spec["fallback_functions"]
    obj = ROOT / spec["source_object"]
    funcs = object_functions(obj)

    if int(unit["measures"]["total_code"]) != spec["expected_total_code"] or \
            int(unit["measures"]["total_functions"]) != spec["expected_total_functions"]:
        raise SystemExit(f"{unit['name']}: configured report denominator changed")
    try:
        validate_function_mask(report_functions, funcs, fallback)
    except ValueError as error:
        raise SystemExit(f"{unit['name']}: {error}") from error
    if len(funcs) != spec["expected_source_functions"] or \
            sum(size for _, size in funcs.values()) != spec["expected_source_code"]:
        raise SystemExit(f"{unit['name']}: configured source-owned mask changed")

    data, object_sections = sections(obj)
    lit4_offset, lit4_size = object_sections.get(".lit4", (0, 0))
    reginfo_offset, reginfo_size = object_sections.get(".reginfo", (0, 0))
    object_gp = struct.unpack_from("<i", data, reginfo_offset + 20)[0] \
        if reginfo_size >= 24 else 0
    lit4 = data[lit4_offset:lit4_offset + lit4_size]
    text = text_section(obj)
    relocs = relocations(obj)[".text"]
    version = spec["version"]
    syms = symbols(version)
    retail = (ROOT / RETAIL[version]).read_bytes()
    segments = load_segments(retail)
    gp = int(VERSIONS[version]["gp"], 16)
    retail_lit4 = lit4_range(retail)

    corrected = []
    fuzzy_points = 0.0
    for name, entry in report_functions.items():
        fuzzy = float(entry.get("fuzzy_match_percent", 0.0))
        if name in fallback or fuzzy == 100.0:
            continue
        if fuzzy <= 0.0:
            raise SystemExit(f"{unit['name']}:{name}: source-owned function has no objdiff score")
        diffs = linked_function_diffs(
            name, funcs[name], text, relocs, lit4, object_gp, funcs, syms,
            retail, segments, gp, retail_lit4,
        )
        if diffs:
            raise SystemExit(f"{unit['name']}:{name}: " + "; ".join(diffs[:3]))
        size = int(entry["size"])
        fuzzy_points += size * (100.0 - fuzzy)
        entry["fuzzy_match_percent"] = 100.0
        corrected.append((name, size))

    corrected_code = sum(size for _, size in corrected)
    if {name for name, _ in corrected} != set(spec["corrected_functions"]):
        raise SystemExit(f"{unit['name']}: configured correction set changed")
    if len(corrected) != spec["expected_corrected_functions"] or \
            corrected_code != spec["expected_corrected_code"]:
        raise SystemExit(
            f"{unit['name']}: correction scope is {len(corrected)} functions/"
            f"{corrected_code} bytes"
        )
    apply_delta(unit["measures"], corrected_code, len(corrected), fuzzy_points)
    return corrected_code, len(corrected), fuzzy_points


def reconcile(report: dict, scope: str, manifest: dict) -> None:
    changes = []
    # A combined report may contain only the games passed to configure.py.
    # Still require every configured correction unit within a present game.
    report_versions = {unit["name"].split("/", 1)[0] for unit in report["units"]}
    report_versions.update(category["id"] for category in report["categories"])
    if scope == "all" and not report_versions.intersection(VERSIONS):
        raise SystemExit("combined report contains no configured games")
    for spec in manifest["units"]:
        if scope not in ("all", spec["version"]):
            continue
        if scope == "all" and spec["version"] not in report_versions:
            continue
        report_name = spec["name"] if scope != "all" else f"{spec['version']}/{spec['name']}"
        unit = next((candidate for candidate in report["units"]
                     if candidate["name"] == report_name), None)
        if unit is None:
            raise SystemExit(f"configured report unit is missing: {report_name}")
        code, functions, fuzzy = reconcile_unit(report, unit, spec)
        changes.append((unit, code, functions, fuzzy))

    for unit, code, functions, fuzzy in changes:
        categories = set(unit.get("metadata", {}).get("progress_categories", []))
        for category in report["categories"]:
            if category["id"] in categories:
                apply_delta(category["measures"], code, functions, fuzzy)
        apply_delta(report["measures"], code, functions, fuzzy)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("input", type=Path)
    parser.add_argument("output", type=Path)
    parser.add_argument("--scope", required=True, choices=("all", *VERSIONS.keys()))
    parser.add_argument("--manifest", type=Path, default=MANIFEST)
    args = parser.parse_args()
    report = json.loads(args.input.read_text())
    reconcile(report, args.scope, json.loads(args.manifest.read_text()))
    args.output.write_text(json.dumps(report, indent=2) + "\n")


if __name__ == "__main__":
    main()
