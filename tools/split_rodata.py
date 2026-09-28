#!/usr/bin/env python3
"""Give each C unit its own .rodata, so functions with switch tables can be matched.

    python3 tools/split_rodata.py dds1 [--write]

Retail .rodata is one run of every object's rodata in link order. A rodata
symbol belongs to the segment whose code references it, and owners must be in
link order along the section: a symbol whose owner comes after a later
symbol's owner is some other file's global that code only refers to (e.g. a
command table defined next to its users), so it stays with the object before it.
Unreferenced symbols (padding, strings reached through data) stay with the
object before them.

Each C unit with rodata becomes a `[rom, .rodata, <unit>]` subsegment and the
rest stays in `[rom, rodata, rodata_<vram>]` asm chunks. splat then moves a
function's jump tables into its asm file and writes INCLUDE_RODATA for the
unit's other rodata (strings, tables); configure.py puts those lines into the
existing C files. Needs the asm from a previous split (asm/<v>/**/*.s).
"""
import argparse
import bisect
import json
import re
from pathlib import Path

import yaml

ROOT = Path(__file__).resolve().parent.parent
VERSIONS = json.loads((ROOT / "config/versions.json").read_text())
LABEL = re.compile(r"^(?:dlabel|glabel|jlabel) (\w+)\n\s*/\* [0-9A-F]+ ([0-9A-F]{8})", re.M)
REF = re.compile(r"/\* [0-9A-F]+ ([0-9A-F]{8}) [0-9A-F]{8} \*/.*?\b\w+?_([0-9A-F]{8})\b")
RODATA_ROW = re.compile(r"^(\s+- \[)0x([0-9A-Fa-f]+), rodata, rodata\](.*)$", re.M)


def layout(version, section="rodata"):
    cfg_path = ROOT / "config" / version / f"{VERSIONS[version]['serial']}.yaml"
    cfg = yaml.safe_load(cfg_path.read_text())
    main = next(s for s in cfg["segments"] if isinstance(s, dict) and s.get("subsegments"))
    rom0, vram0 = main["start"], main["vram"]
    subs = [s for s in main["subsegments"] if isinstance(s, list) and len(s) >= 2]
    text = sorted((s[0] - rom0 + vram0, s[1], s[2]) for s in subs if len(s) >= 3 and s[1] in ("c", "asm"))
    kinds = (section, "." + section)
    rodata_rom = next(s[0] for s in subs if s[1] in kinds)
    end_rom = min(s[0] for s in subs if s[0] > rodata_rom and s[1] not in kinds)
    return cfg_path, rom0, vram0, text, rodata_rom, end_rom


def owners(version, section="rodata"):
    cfg_path, rom0, vram0, text, rodata_rom, end_rom = layout(version, section)
    lo, hi = rodata_rom - rom0 + vram0, end_rom - rom0 + vram0
    asm = ROOT / "asm" / version
    # Every rodata symbol splat wrote: the whole section before a split, the asm
    # chunks and per-unit files after one (re-running is idempotent).
    found = {}
    for path in (asm / "data").rglob(f"*.{section}.s"):
        for n, a in LABEL.findall(path.read_text()):
            found.setdefault(int(a, 16), n)
    syms = sorted(found.items())
    addrs, starts = [a for a, _ in syms], [t[0] for t in text]
    owner = [None] * len(syms)
    for path in asm.rglob("*.s"):
        if path.relative_to(asm).parts[0] in ("data", "nonmatchings", "matchings"):
            continue
        for m in REF.finditer(path.read_text()):
            pc, target = int(m.group(1), 16), int(m.group(2), 16)
            if lo <= target < hi:
                i = bisect.bisect_right(addrs, target) - 1
                seg = bisect.bisect_right(starts, pc) - 1
                owner[i] = seg if owner[i] is None else min(owner[i], seg)
    # Link order: keep the largest set of owned symbols whose owners never decrease
    # along the section; the rest are other files' globals referenced as externs.
    owned = [i for i in range(len(syms)) if owner[i] is not None]
    tails, tail_idx, parent = [], [], {}
    for i in owned:
        k = bisect.bisect_right(tails, owner[i])
        parent[i] = tail_idx[k - 1] if k else None
        if k == len(tails):
            tails.append(owner[i])
            tail_idx.append(i)
        else:
            tails[k], tail_idx[k] = owner[i], i
    keep, i = set(), tail_idx[-1] if tail_idx else None
    while i is not None:
        keep.add(i)
        i = parent[i]
    for i in owned:
        if i not in keep:
            owner[i] = None
    current = None
    for i in range(len(syms)):
        current = owner[i] = owner[i] if owner[i] is not None else current
    return cfg_path, rom0, vram0, text, rodata_rom, end_rom, syms, owner


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("version")
    ap.add_argument("--write", action="store_true")
    ap.add_argument("--section", default="rodata", choices=["rodata", "lit4"],
                    help="lit4: the .lit4 float pool, owned the same way")
    args = ap.parse_args()
    cfg_path, rom0, vram0, text, rodata_rom, end_rom, syms, owner = owners(args.version, args.section)
    to_rom = lambda vram: vram - vram0 + rom0
    rows, prev = [], None
    for (addr, name), seg in zip(syms, owner):
        kind = "c" if seg is not None and text[seg][1] == "c" else "asm"
        key = text[seg][2] if kind == "c" else "asm"
        if key != prev:
            rows.append([addr, kind, key, False])
            prev = key
        if name.startswith("jtbl_"):
            rows[-1][3] = True
    # ee-gcc starts every jump table with .align 4, so an object that has one
    # begins on a 16-byte boundary. A unit whose first symbol is not aligned
    # starts at its first aligned symbol; what comes before belongs to the
    # previous file (referenced here as an extern).
    for i, (addr, kind, key, has_jtbl) in enumerate(rows):
        if args.section == "rodata" and kind == "c" and has_jtbl and addr % 16:
            end = rows[i + 1][0] if i + 1 < len(rows) else None
            rows[i][0] = next(a for a, _ in syms if a > addr and a % 16 == 0 and (end is None or a < end))
    lines, first = [], True
    for addr, kind, key, _ in rows:
        rom = rodata_rom if first else to_rom(addr)
        first = False
        if kind == "c":
            lines.append(f"      - [0x{rom:X}, .{args.section}, {key}]")
        else:
            lines.append(f"      - [0x{rom:X}, {args.section}, {args.section}_{rom - rom0 + vram0:08X}]")
    if args.section == "lit4" and syms:
        # The zero fill between the last constant and .sdata is section alignment
        # in the original link, not any object's literal.
        elf = (ROOT / "orig" / args.version / VERSIONS[args.version]["serial"]).read_bytes()
        pad = end_rom
        while pad > rodata_rom and not any(elf[pad - 4:pad]):
            pad -= 4
        rows = [r for r in rows if to_rom(r[0]) < pad]
        lines = [l for l in lines if int(l.split("[")[1].split(",")[0], 16) < pad]
        if pad < end_rom:
            lines.append(f"      - [0x{pad:X}, lit4, lit4_pad]")
    units = sum(1 for _, k, _, _ in rows if k == "c")
    print(f"{args.version}: {units} C units get their own .{args.section}, {len(rows) - units} asm chunks")
    if args.write:
        text_yaml = cfg_path.read_text()
        start = text_yaml.index(f"      - [0x{rodata_rom:X}, ")
        end = text_yaml.index(f"      - [0x{end_rom:X}, ")
        cfg_path.write_text(text_yaml[:start] + "\n".join(lines) + "\n" + text_yaml[end:])
        print(f"-> {cfg_path.relative_to(ROOT)}")
    else:
        print("\n".join(lines))


if __name__ == "__main__":
    main()
