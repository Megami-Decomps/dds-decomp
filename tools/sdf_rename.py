#!/usr/bin/env python3
"""Carry SMT3 names to DDS1 and DDS2 through names.py apply and rename_refs.py.

    python3 tools/sdf_rename.py rows.txt

A row is `<new> <smt3 addr> <dds1 addr> <dds2 addr> <reason...>`. An address
with no symbol_addrs row gets a bare `D_X = 0xX;` row first. The old-to-new
map is left in build/sdf_rename_refs.tsv for tools/sdf_restage.py.
"""
import re, subprocess, sys
from pathlib import Path
ROOT = Path(__file__).resolve().parent.parent
PY = sys.executable
OUT = ROOT / "build"
ROW = re.compile(r"^\s*(\S+)\s*=\s*0x([0-9A-Fa-f]+)\s*;(.*)$")
rows = [l.split(None, 4) for l in Path(sys.argv[1]).read_text().splitlines() if l.strip() and not l.startswith("#")]
refs = {}
for gi, v in ((2, "dds1"), (3, "dds2")):
    sym = ROOT / f"config/{v}/symbol_addrs.txt"
    lines = sym.read_text().splitlines()
    names = {}
    for l in lines:
        m = ROW.match(l)
        if m:
            names.setdefault(int(m[2], 16), m[1])
    end = next(i for i, l in enumerate(lines) if l.startswith("// BEGIN "))
    tsv = []
    for r in rows:
        new, smt3, addr, reason = r[0], r[1], int(r[gi], 16), r[4]
        old = names.get(addr)
        if old is None:
            old = f"D_{addr:08X}"
            lines.insert(end, f"{old} = 0x{addr:08X};")
            end += 1
        if old != new:
            refs[old] = new
        tsv.append(f"0x{addr:08X}\t{new}\tinferred\tSMT3HD name via SLUS_209.11 {smt3}, {reason}; was {old}")
    sym.write_text("\n".join(lines) + "\n")
    t = OUT / f"sdf_rename_{v}.tsv"
    t.write_text("\n".join(tsv) + "\n")
    subprocess.run([PY, "tools/names.py", "apply", v, str(t)], cwd=ROOT, check=True)
t = OUT / "sdf_rename_refs.tsv"
t.write_text("".join(f"{o}\t{n}\n" for o, n in refs.items()))
subprocess.run([PY, "tools/rename_refs.py", str(t)], cwd=ROOT, check=True)
print(refs)
