#!/usr/bin/env python3
"""Place INCLUDE_SDATA lines so C units build their own .sdata.

    python3 tools/include_sdata.py [dds1 dds2]

Units with a `.sdata` subsegment (tools/split_rodata.py --section sdata) get
their small data from the object: splat writes the unit's whole .sdata to
asm/<v>/data/<unit>.sdata.s. This splits that file into one file per symbol
(asm/<v>/nonmatchings/<unit>/<sym>.s) and puts an INCLUDE_SDATA line for each
into the C file in address order, so that what the C itself emits lands in
between exactly where ee-gcc put it:

- a short string literal (up to 7 bytes: -G8 puts it in .sdata) a C function
  writes instead of naming the retail D_ symbol goes with that function;
- a variable the C file defines (`s32 D_003BD1F0 = 5;`, `static` or not, or
  under its symbol_addrs.txt name) sits at its definition.

Everything else is included. Placement is recomputed on every run
(configure.py runs this after each split).
"""
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
LINE = re.compile(r'^INCLUDE_SDATA\([^,]+,\s*"[^"]+",\s*(\w+)\);\n\n?', re.M)
DEF = re.compile(r"^[A-Za-z_][\w \t\*]*?\b(\w+)\s*\([^;{]*\)\s*\{?[ \t]*$", re.M)
DATA_DEF = re.compile(r"^(?!extern\b|typedef\b)(?:static\s+)?(?:const\s+)?[A-Za-z_][\w \t\*]*?\b(\w+)\s*(?:\[[^\]]*\])*\s*(?:=|;)", re.M)
BLOCK = re.compile(r"((?:^\.align \d+\n)*)^(?:nonmatching (\w+)[^\n]*\n\n?)?^dlabel (\w+)\n(.*?)^enddlabel \3\n", re.M | re.S)
ADDR = re.compile(r"/\* [0-9A-F]+ ([0-9A-F]{8})")
ROW = re.compile(r"^(\w+) = 0x([0-9A-Fa-f]+);", re.M)


def function_text(text, start):
    depth, i = 0, text.index("{", start)
    for j in range(i, len(text)):
        depth += {"{": 1, "}": -1}.get(text[j], 0)
        if depth == 0:
            return text[start:j + 1]
    return text[start:]


def place(version):
    names = {n: int(a, 16) for n, a in ROW.findall((ROOT / "config" / version / "symbol_addrs.txt").read_text())}
    added = 0
    for c in sorted((ROOT / "src" / version).rglob("*.c")):
        unit = c.relative_to(ROOT / "src" / version).with_suffix("").as_posix()
        data = ROOT / "asm" / version / "data" / f"{unit}.sdata.s"
        text = LINE.sub("", c.read_text())
        if not data.exists():
            c.write_text(text)
            continue
        out_dir = ROOT / "asm" / version / "nonmatchings" / unit
        out_dir.mkdir(parents=True, exist_ok=True)
        syms = []
        blocks = list(BLOCK.finditer(data.read_text()))
        if not blocks:
            c.write_text(text)
            continue
        # An .align raises the whole section's alignment, so no symbol may ask for
        # more than the unit's own start address has.
        first = ADDR.search(blocks[0].group(4))
        start = int(first.group(1), 16) if first else int(blocks[0].group(3)[-8:], 16)
        cap = next(k for k in (3, 2, 1, 0) if start % (1 << k) == 0)
        for m in blocks:
            sym = m.group(3)
            a = ADDR.search(m.group(4))
            addr = int(a.group(1), 16) if a else int(sym[-8:], 16)
            k = next(k for k in (3, 2, 1, 0) if addr % (1 << k) == 0)
            if m.group(1):
                k = min(k, int(re.findall(r"\d+", m.group(1))[-1]))
            align = f".align {min(k, cap)}\n"
            (out_dir / f"{sym}.s").write_text(f".section .sdata\n\n{align}nonmatching {sym}\n\ndlabel {sym}\n{m.group(4)}enddlabel {sym}\n")
            syms.append((addr, sym))
        mine = {s for _, s in syms}
        # What the C emits itself: defined variables, and literals its functions write.
        anchors = []
        for m in DATA_DEF.finditer(text):
            n = m.group(1)
            addr = names.get(n, int(n[2:], 16) if re.fullmatch(r"D_[0-9A-F]{8}", n) else None)
            if addr is not None and any(a == addr for a, _ in syms):
                anchors.append((addr, m.start(), n))
        full = ROOT / "asm" / version / f"{unit}.s"
        refs = {}
        if full.exists():
            for f in re.finditer(r"^glabel (\w+)\n(.*?)^endlabel \1", full.read_text(), re.M | re.S):
                refs[f.group(1)] = set(re.findall(r"%(?:lo|gp_rel)\((\w+)\)", f.group(2)))
        asm_funcs = set(re.findall(r'^INCLUDE_ASM\([^\n]*\b(\w+)\);', text, re.M))
        still_asm = set().union(*(refs.get(f, set()) for f in asm_funcs)) if asm_funcs else set()
        addr_of = {s: a for a, s in syms}
        for m in DEF.finditer(text):
            code = function_text(text, m.start())
            for sym in refs.get(m.group(1), ()):
                if sym in mine and sym not in still_asm and not re.search(rf"\b{sym}\b", code):
                    anchors.append((addr_of[sym], m.start(), sym))
        compiled = {s for _, _, s in anchors}
        order = sorted([(a, None, s) for a, s in syms if s not in compiled] + anchors, key=lambda x: x[0])
        following, placed = len(text), []
        for addr, pos, sym in reversed(order):
            pos = following if pos is None else pos
            following = pos
            if sym not in compiled:
                placed.append((pos, sym))
        for pos, sym in sorted(placed, key=lambda p: -p[0]):
            text = text[:pos] + ("\n" if pos == len(text) else "") + \
                f'INCLUDE_SDATA(const s32, "{unit}", {sym});\n\n' + text[pos:]
            added += 1
        # One blank line before each included item, however often this reruns.
        text = re.sub(r"\n{3,}(?=INCLUDE_(?:RODATA|SDATA)\()", "\n\n", text)
        c.write_text(text)
    print(f"{version}: {added} INCLUDE_SDATA lines")


if __name__ == "__main__":
    for v in sys.argv[1:] or ["dds1", "dds2"]:
        place(v)
