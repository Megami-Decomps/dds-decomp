#!/usr/bin/env python3
"""Decompilation progress per version: functions and code bytes in C.

    python3 tools/progress.py [dds1 dds2] [--markdown]

Every retail function of a game unit is either C in src/<v>/ or an
INCLUDE_ASM line. Sizes come from the split assembly (asm/<v>/, so run
configure.py first). Library code outside src/ is not counted.
"""
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
INCLUDE = re.compile(r'^INCLUDE_ASM\([^\n]*\b(\w+)\);', re.M)
GLABEL = re.compile(r"^glabel (\w+)\n(.*?)^endlabel \1", re.M | re.S)
INSN = re.compile(r"^\s*/\* [0-9A-F]+ [0-9A-F]{8} [0-9A-F]{8} \*/", re.M)


def version_stats(version):
    funcs = c_funcs = size = c_size = 0
    for unit in (ROOT / "src" / version).rglob("*.c"):
        name = unit.relative_to(ROOT / "src" / version).with_suffix("").as_posix()
        full = ROOT / "asm" / version / f"{name}.s"
        if not full.exists():
            continue
        asm_funcs = set(INCLUDE.findall(unit.read_text()))
        for fname, body in GLABEL.findall(full.read_text()):
            n = 4 * len(INSN.findall(body))
            funcs += 1
            size += n
            if fname not in asm_funcs:
                c_funcs += 1
                c_size += n
    return funcs, c_funcs, size, c_size


def main():
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    md = "--markdown" in sys.argv
    if md:
        print("| Version | Functions in C | Code bytes in C |\n|---|---|---|")
    for v in args or ["dds1", "dds2"]:
        f, cf, s, cs = version_stats(v)
        if not f:
            print(f"{v}: no split assembly (run configure.py)", file=sys.stderr)
            continue
        if md:
            print(f"| `{v}` | {cf:,} / {f:,} ({100 * cf / f:.1f}%) | {cs:,} / {s:,} ({100 * cs / s:.1f}%) |")
        else:
            print(f"{v}: {cf}/{f} functions ({100 * cf / f:.1f}%), {cs}/{s} bytes ({100 * cs / s:.1f}%)")


if __name__ == "__main__":
    main()
