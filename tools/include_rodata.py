#!/usr/bin/env python3
"""Place INCLUDE_RODATA for the rodata splat could not give to one asm function.

    python3 tools/include_rodata.py [dds1 dds2]

With per-unit .rodata (tools/split_rodata.py) a unit's rodata is emitted in
source order: each INCLUDE_ASM brings the rodata splat migrated into that
function's asm file, and each C function brings what the compiler emits for it
(jump tables). Two kinds of rodata have no such home:

- rodata of functions that are already C but whose C does not yet produce it
  (strings and constants still referenced as extern D_ symbols); splat leaves it
  in asm/<v>/matchings/<unit>/<func>.s. It goes directly before that function.
  Once the C writes the literal itself (the D_ symbol no longer appears in the
  function), the compiler emits it and no line is needed.
- rodata several functions share, or that only .data tables point at; splat
  writes it to asm/<v>/nonmatchings/<unit>/<sym>.s. It goes where its address
  falls: before the first function whose rodata comes after it, or at the end.

Both get per-symbol files in asm/<v>/nonmatchings/<unit>/ and an INCLUDE_RODATA
line. Lines already present are replaced: the placement is recomputed on every run.
"""
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
INCLUDE = re.compile(r'^INCLUDE_(?:ASM|RODATA)\([^,]+,\s*"[^"]+",\s*(\w+)\);$', re.M)
INCLUDE_ASM = re.compile(r'^INCLUDE_ASM\([^,]+,\s*"[^"]+",\s*(\w+)\);$', re.M)
DEF = re.compile(r"^[A-Za-z_][\w \t\*]*?\b(\w+)\s*\([^;{]*\)\s*\{?[ \t]*$", re.M)
LABEL = re.compile(r"^dlabel (\w+)\n\s*/\* [0-9A-F]+ ([0-9A-F]{8})", re.M)
SYMBOL_BLOCK = re.compile(r"(?:\.align \d+\n)?nonmatching (\w+).*?enddlabel \1\n", re.S)


def rodata_part(path):
    text = path.read_text() if path.exists() else ""
    return text.split(".section .text", 1)[0] if text.startswith(".section .rodata") else ""


def place(version):
    added = 0
    for c in sorted((ROOT / "src" / version).rglob("*.c")):
        unit = c.relative_to(ROOT / "src" / version).with_suffix("").as_posix()
        nonmatchings = ROOT / "asm" / version / "nonmatchings" / unit
        matchings = ROOT / "asm" / version / "matchings" / unit
        if not nonmatchings.is_dir():
            continue
        # The unit's rodata as splat last split it; INCLUDE_RODATA lines and per-symbol
        # files for anything else are left over from an earlier split.
        full = ROOT / "asm" / version / "data" / f"{unit}.rodata.s"
        owned = {n for n, _ in LABEL.findall(full.read_text())} if full.exists() else set()
        # Symbols splat migrated into an asm function's own file come with its
        # INCLUDE_ASM and must not be included again.
        for f in nonmatchings.glob("*.s"):
            if "glabel " in f.read_text():
                owned -= {n for n, _ in LABEL.findall(rodata_part(f))}
        text = c.read_text()
        # Placed from scratch every time: once a function compiles its own
        # literal, the lines around it must move with it.
        text = re.sub(r'^INCLUDE_RODATA\([^,]+,\s*"[^"]+",\s*(\w+)\);\n\n?', "", text, flags=re.M)
        # Rodata a C function no longer names is compiled by that function itself
        # (a string literal): nothing to include.
        # The retail references come from the unit's full disassembly, which
        # also covers rodata several functions share (splat leaves that
        # standalone instead of in one function's file).
        full = ROOT / "asm" / version / f"{unit}.s"
        retail_refs = {}
        if full.exists():
            for f in re.finditer(r"^glabel (\w+)\n(.*?)^endlabel \1", full.read_text(), re.M | re.S):
                retail_refs[f.group(1)] = set(re.findall(r"%lo\((\w+)\)", f.group(2)))
        compiled = set()
        for m in DEF.finditer(text):
            code = function_text(text, m.start())
            for sym in retail_refs.get(m.group(1), ()):
                if sym.startswith(("D_", "jtbl_")) and not re.search(rf"\b{sym}\b", code):
                    compiled.add(sym)
        # ... unless an asm function still needs the retail copy (check_unit SHARED).
        for f in INCLUDE_ASM.findall(text):
            compiled -= retail_refs.get(f, set())
        # The compiler emits each such literal with the first function using it.
        compiled_at = {}
        for m in DEF.finditer(text):
            for sym in retail_refs.get(m.group(1), ()):
                if sym in compiled:
                    compiled_at.setdefault(sym, m.start())
        owned -= compiled
        have = set(INCLUDE.findall(text))
        anchors = []   # (address, position in text or None, symbol, needs a line)
        for m in INCLUDE_ASM.finditer(text):
            for _, addr in LABEL.findall(rodata_part(nonmatchings / f"{m.group(1)}.s")):
                anchors.append((int(addr, 16), m.start(), None, False))
        for m in DEF.finditer(text):
            rodata = rodata_part(matchings / f"{m.group(1)}.s")
            for block in SYMBOL_BLOCK.finditer(rodata):
                sym = block.group(1)
                addr = int(LABEL.search(block.group(0)).group(2), 16)
                if sym.startswith("jtbl_") or sym in compiled:
                    anchors.append((addr, m.start(), None, False))  # the C emits it
                    continue
                out = nonmatchings / f"{sym}.s"
                if not out.exists():
                    out.write_text(".section .rodata\n\n" + block.group(0))
                anchors.append((addr, m.start(), sym, sym not in have))
        for sym, pos in compiled_at.items():
            if not any(a[2] == sym for a in anchors):
                anchors.append((int(sym[-8:], 16), pos, None, False))
        for s in [s for s in nonmatchings.glob("*.s") if "glabel " not in s.read_text()]:
            m = LABEL.search(s.read_text())
            if m and m.group(1) in owned and not any(sym == m.group(1) for _, _, sym, _ in anchors):
                anchors.append((int(m.group(2), 16), None, m.group(1), m.group(1) not in have))
        anchors.sort(key=lambda a: a[0])
        # Floating symbols take the position of the next anchored one.
        following = len(text)
        placed = []
        for addr, pos, sym, needed in reversed(anchors):
            pos = following if pos is None else pos
            following = pos
            placed.append((pos, addr, sym, needed))
        positions = [pos for pos, *_ in sorted(placed, key=lambda p: p[1])]
        if positions != sorted(positions):
            print(f"warning: {c.relative_to(ROOT)}: rodata order does not follow function order")
        inserts = {}
        for pos, addr, sym, needed in sorted(placed, key=lambda p: p[1]):
            if needed:
                inserts.setdefault(pos, []).append(sym)
        for pos in sorted(inserts, reverse=True):
            lines = "".join(f'INCLUDE_RODATA(const s32, "{unit}", {s});\n\n' for s in inserts[pos])
            text = text[:pos] + ("\n" if pos == len(text) else "") + lines + text[pos:]
            added += len(inserts[pos])
        c.write_text(text)
    print(f"{version}: {added} INCLUDE_RODATA lines")


def function_text(text, start):
    """Text of the C function definition starting at `start`."""
    depth, i = 0, text.index("{", start)
    for j in range(i, len(text)):
        depth += {"{": 1, "}": -1}.get(text[j], 0)
        if depth == 0:
            return text[start:j + 1]
    return text[start:]


if __name__ == "__main__":
    for v in sys.argv[1:] or ["dds1", "dds2"]:
        place(v)
