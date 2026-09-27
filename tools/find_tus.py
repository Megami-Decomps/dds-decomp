#!/usr/bin/env python3
"""Collect translation-unit (TU) evidence from the split assembly.

    python tools/find_tus.py dds1 [--sdk-lib DIR] [--p4-diff build/p4_diff_dds1.json --p4-root DIR]

Writes build/<v>/tu_evidence.txt. Every row says which rule produced it.

Hard evidence (ee-gcc 2.96 + ee-as, verified by compiling test units):
  * same TU     A string literal, jump table or .lit4 constant is emitted into
                the object that uses it (gcc merges identical strings within a
                unit; ee-as never shares .lit4 entries across objects). Functions
                that reference the same one share a TU. Each section's per-object
                blocks are linked in text order, so two references whose data
                order inverts their text order also share a TU (and so does
                everything between them: a TU is contiguous).
  * boundary    The same string at two addresses means two TUs, since a TU
                never holds two copies. The TUs that use the copies are separated
                by at least one boundary.
  * SDK member  romwright signature names mapped to archive members (`nm` of
                the SDK archives): every member object is one TU.

Hint (not proof):
  * P4 name     Runs of functions that romwright's structural diff pairs with
                functions from one Persona 4 source file.
"""
from __future__ import annotations

import argparse
import collections
import json
import re
import subprocess
from pathlib import Path

from elftools.elf.elffile import ELFFile

ROOT = Path(__file__).resolve().parent.parent
VERSIONS = json.loads((ROOT / "config" / "versions.json").read_text())
NM = ROOT / "tools" / "bin" / "mips-ps2-decompals-nm"
SYMREF = re.compile(r"%(?:lo|gp_rel)\(((?:D_|jtbl_)([0-9A-F]{8}))(?:\s*\+\s*(0x[0-9A-F]+|\d+))?\)")
INSN = re.compile(r"\s*/\* [0-9A-F]+ ([0-9A-F]{8}) ")
SDK_ARCHIVES = ["ee/lib/libgraph.a", "ee/lib/libdma.a", "ee/lib/libpad.a", "ee/lib/libcdvd.a",
                "ee/lib/libmrpc.a", "ee/lib/libipu.a", "ee/lib/libsdr.a", "ee/lib/libmc.a",
                "ee/lib/libkernl.a", "ee/gcc/ee/lib/libc.a", "ee/gcc/ee/lib/libm.a"]


class Image:
    def __init__(self, version: str):
        path = ROOT / "orig" / version / VERSIONS[version]["serial"]
        self.data = path.read_bytes()
        elf = ELFFile(path.open("rb"))
        self.sections = {s.name: (s["sh_addr"], s["sh_addr"] + s["sh_size"])
                         for s in elf.iter_sections() if s["sh_addr"]}
        text = elf.get_section_by_name(".text")
        self.delta = text["sh_addr"] - text["sh_offset"]

    def section(self, addr: int) -> str | None:
        for name in (".rodata", ".sdata", ".lit4"):
            lo, hi = self.sections[name]
            if lo <= addr < hi:
                return name
        return None

    def cstring(self, addr: int) -> bytes | None:
        off = addr - self.delta
        end = self.data.find(b"\0", off)
        s = self.data[off:end]
        if (len(s) >= 2 and all(32 <= c < 127 or c in (9, 10, 13) or c >= 0xA0 for c in s)
                and any(chr(c).isalnum() for c in s if c < 127)):
            return s
        return None


def functions_and_refs(version: str) -> tuple[list[int], dict[int, set[tuple[bool, int]]]]:
    """Function starts (glabel) and their %lo/%gp_rel data references."""
    skip = {"nonmatchings", "matchings", "data"}
    starts, refs = [], collections.defaultdict(set)
    for path in sorted((ROOT / "asm" / version).rglob("*.s")):
        if skip & set(path.relative_to(ROOT / "asm" / version).parts):
            continue
        cur, pending = None, False
        for line in path.read_text().splitlines():
            if line.startswith("glabel "):
                pending = True
                continue
            m = INSN.match(line)
            if m and pending:
                cur, pending = int(m.group(1), 16), False
                starts.append(cur)
            if cur is None:
                continue
            for full, addr, off in SYMREF.findall(line):
                refs[cur].add((full.startswith("jtbl_"), int(addr, 16) + (int(off, 0) if off else 0)))
    return sorted(set(starts)), refs


def literal_evidence(img: Image, starts: list[int], refs) -> tuple[list[list[int]], list[tuple[int, int, bytes]]]:
    index = {f: i for i, f in enumerate(starts)}
    items = []  # (function index, section, address, string-or-None)
    for f, rs in refs.items():
        for is_jtbl, addr in rs:
            sec = img.section(addr)
            if sec == ".lit4" or (is_jtbl and sec == ".rodata"):
                items.append((index[f], sec, addr, None))
            elif sec in (".rodata", ".sdata") and addr % 4 == 0 and (s := img.cstring(addr)):
                items.append((index[f], sec, addr, s))

    intervals = []
    users = collections.defaultdict(set)
    for fi, _, addr, _ in items:
        users[addr].add(fi)
    intervals += [(min(u), max(u)) for u in users.values()]
    per_section = collections.defaultdict(list)
    for fi, sec, addr, _ in items:
        per_section[sec].append((addr, fi))
    for lst in per_section.values():
        highest = -1
        for _, fi in sorted(lst):
            if highest > fi:
                intervals.append((fi, highest))
            highest = max(highest, fi)
    cores: list[list[int]] = []
    for lo, hi in sorted(intervals):
        if cores and lo <= cores[-1][1]:
            cores[-1][1] = max(cores[-1][1], hi)
        else:
            cores.append([lo, hi])

    core_of = {}
    for ci, (lo, hi) in enumerate(cores):
        for i in range(lo, hi + 1):
            core_of[i] = ci
    copies = collections.defaultdict(set)
    for fi, _, addr, s in items:
        if s is not None:
            copies[s].add((addr, core_of[fi]))
    # Keep only the tightest pair per gap: cores a < b with no other core between
    # the copies' owners give "a boundary lies in (end of a, start of b]".
    boundaries = {}
    for s, where in copies.items():
        owners = sorted({c for _, c in where})
        for a, b in zip(owners, owners[1:]):
            key = (cores[a][1], cores[b][0])
            boundaries.setdefault(key, s)
    return cores, sorted((a, b, s) for (a, b), s in boundaries.items())


def sdk_members(version: str, sdk_lib: Path) -> list[tuple[str, int, int, int]]:
    members = collections.defaultdict(set)
    for rel in SDK_ARCHIVES:
        archive = sdk_lib / rel
        if not archive.exists():
            continue
        out = subprocess.run([str(NM), "-A", "--defined-only", str(archive)], capture_output=True, text=True).stdout
        for line in out.splitlines():
            parts = line.split()
            if len(parts) == 3 and parts[1] in ("T", "t"):
                member = parts[0].split(":")[-2]
                members[parts[2]].add(f"{archive.stem}/{member}")
    names = []
    for line in (ROOT / "config" / version / "symbol_addrs.txt").read_text().splitlines():
        m = re.match(r"(\S+) = 0x([0-9A-F]+); // type:func", line)
        if m and len(members.get(m.group(1), ())) == 1:
            names.append((int(m.group(2), 16), next(iter(members[m.group(1)]))))
    runs: list[list] = []
    for addr, member in sorted(names):
        if runs and runs[-1][0] == member:
            runs[-1][2] = addr
            runs[-1][3] += 1
        else:
            runs.append([member, addr, addr, 1])
    return [tuple(r) for r in runs]


def p4_runs(starts: list[int], diff_path: Path, p4_root: Path) -> list[tuple[str, int, int, int]]:
    p4_file = {}
    for src in (p4_root / "src").rglob("*.c"):
        if "generated" in src.parts:
            continue
        for m in re.finditer(r"//\s*FUN_([0-9A-Fa-f]{8})", src.read_text(errors="ignore")):
            p4_file[int(m.group(1), 16)] = str(src.relative_to(p4_root / "src"))
    index = {f: i for i, f in enumerate(starts)}
    seq = []
    for m in json.loads(diff_path.read_text())["matches"]:
        target, ref = m["target"]["offset"], m["reference"]["offset"]
        f = p4_file.get(ref)
        if m["confidence"] in ("exact", "likely") and f and target in index:
            seq.append((index[target], f))
    runs: list[list] = []
    for i, f in sorted(seq):
        if runs and runs[-1][0] == f and i - runs[-1][2] <= 30:
            runs[-1][2] = i
            runs[-1][3] += 1
        else:
            runs.append([f, i, i, 1])
    return [(f, starts[lo], starts[hi], n) for f, lo, hi, n in runs if n >= 3]


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("version", choices=list(VERSIONS))
    ap.add_argument("--sdk-lib", type=Path, help="extracted SDK root (contains ee/lib)")
    ap.add_argument("--p4-diff", type=Path, help="romwright-cli diff --json output (target = this version)")
    ap.add_argument("--p4-root", type=Path, help="Persona 4 decomp checkout, for its source file names")
    args = ap.parse_args()

    img = Image(args.version)
    starts, refs = functions_and_refs(args.version)
    cores, boundaries = literal_evidence(img, starts, refs)
    multi = [c for c in cores if c[1] > c[0]]
    lines = [f"# TU evidence for {args.version} ({VERSIONS[args.version]['serial']}); tools/find_tus.py", ""]
    tight = sum(1 for a, b, _ in boundaries if b == a + 1)
    lines.append(f"functions {len(starts)}; same-TU cores with 2+ functions: {len(multi)} "
                 f"covering {sum(h - l + 1 for l, h in multi)} functions; proven boundaries: {len(boundaries)} "
                 f"({tight} exact)")
    lines += ["", "## same TU (hard): first..last function of each core", ""]
    lines += [f"{starts[lo]:08X}..{starts[hi]:08X}  {hi - lo + 1:4d} functions" for lo, hi in multi]
    lines += ["", "## boundary (hard): a TU boundary lies after the first address and at or before the second", ""]
    lines += [f"{starts[a]:08X} | {starts[b]:08X}  {b - a - 1:5d} functions between  duplicate string {s[:40]!r}"
              for a, b, s in sorted(boundaries, key=lambda r: r[1] - r[0])]
    if args.sdk_lib:
        lines += ["", "## SDK archive members (hard): first..last named function", ""]
        lines += [f"{lo:08X}..{hi:08X}  {n:3d} named  {m}" for m, lo, hi, n in sdk_members(args.version, args.sdk_lib)]
    if args.p4_diff and args.p4_root:
        lines += ["", "## Persona 4 source-file runs (hint): first..last paired function", ""]
        lines += [f"{lo:08X}..{hi:08X}  {n:3d} pairs  {f}" for f, lo, hi, n in p4_runs(starts, args.p4_diff, args.p4_root)]
    out = ROOT / "build" / args.version / "tu_evidence.txt"
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text("\n".join(lines) + "\n")
    print(lines[2])
    print(f"wrote {out.relative_to(ROOT)}")


if __name__ == "__main__":
    main()
