#!/usr/bin/env python3
"""Resolve jump-table entries that point into another split file.

spimdisasm names jump-table targets `.L<ADDR>` (or `func_<ADDR>` when the
address is also a known function start, yet it is emitted as a local `jlabel`);
GNU as never emits `.L` symbols, so a table in asm/<v>/data/rodata.rodata.s
cannot reference those labels in a text file. Making the labels global would turn every case label into a
symbol that objdiff and the linker treat as a function boundary, so instead the
table entry becomes the absolute address it already encodes (the build is
non-shiftable: every address is fixed by the retail layout).

Jump tables in a C unit's .rodata live in the function's own asm file
(splat rodata migration, tools/split_rodata.py) next to their labels, so they
need no resolving; when the function is decompiled the compiler emits the table.

ee-gcc 2.96 starts every jump table with `.align 4` (all retail tables sit on
16-byte boundaries, and the zero words after a table are that padding); splat
writes `.align 3`. Tables in C units' function files (nonmatchings/) are given
`.align 4` so that a compiled table before them, which carries no trailing
padding, still leaves them in place. Those objects were compiled, so their
rodata really is 16-aligned; the asm rodata chunks (data/) keep `.align 3`,
because a chunk can start mid-alignment where one original object ended.

Run by configure.py after each split; idempotent.

    python tools/resolve_jtbl_targets.py asm/dds1
"""
from __future__ import annotations

import re
import sys
from pathlib import Path

ENTRY = re.compile(r"\.word\s+(\.L|func_)([0-9A-F]{8})\b")


JTBL_ALIGN = re.compile(r"^\.align 3\n(?=nonmatching jtbl_)", re.M)
JTBL = re.compile(r"^dlabel jtbl_[0-9A-F]{8}\n.*?^enddlabel jtbl_[0-9A-F]{8}$", re.M | re.S)


def resolve(table: re.Match) -> str:
    return ENTRY.sub(lambda m: f".word 0x{m.group(2)} /* {m.group(1)}{m.group(2)} */", table.group(0))


def main() -> None:
    if len(sys.argv) != 2:
        raise SystemExit(__doc__)
    root = Path(sys.argv[1])
    count = aligned = 0
    for path in sorted((root / "data").glob("*.s")):
        text = path.read_text()
        new = JTBL.sub(resolve, text)
        if new != text:
            count += len(ENTRY.findall(text)) - len(ENTRY.findall(new))
            path.write_text(new)
    for path in sorted((root / "nonmatchings").rglob("*.s")):
        text = path.read_text()
        new, n = JTBL_ALIGN.subn(".align 4\n", text)
        if n:
            aligned += n
            path.write_text(new)
    print(f"{root}: {count} jump-table entries resolved, {aligned} tables aligned to 16")


if __name__ == "__main__":
    main()
