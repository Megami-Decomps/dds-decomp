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
  under its symbol_addrs.txt name) sits at its definition;
- a pad label spimdisasm made for zero bytes that only align the next object
  becomes SDATA_PAD(n) rather than a D_ symbol.

Everything else is included. Placement is recomputed on every run
(configure.py runs this after each split).
"""
import re
import sys
from pathlib import Path

from nonmatching import matching_source
from c_aliases import function_aliases

ROOT = Path(__file__).resolve().parent.parent
LINE = re.compile(r'^(?:INCLUDE_SDATA\([^,]+,\s*"[^"]+",\s*(\w+)\)|SDATA_PAD\(\d+\));\n\n?', re.M)
PAD_NOTE = "/* Automatically generated and unreferenced pad */\n"
VALUE = re.compile(r"\.(?:byte|short|word) (0x[0-9A-Fa-f]+)")
DEF = re.compile(r"^[A-Za-z_][\w \t\*]*?\b(\w+)\s*\([^;{]*\)\s*\{?[ \t]*$", re.M)
DATA_DEF = re.compile(
    r"^(?!extern\b|typedef\b)(?:static\s+)?(?:const\s+)?"
    r"[A-Za-z_][\w \t\*]*?(?:\(\s*\*\s*)?\b(\w+)\s*"
    r"(?:\)\s*\([^;{}\n]*\))?\s*(?:\[[^\]]*\])*\s*"
    r'(?:__attribute__\s*\(\([^;\n]*\)\)\s*)*(?:=|;)',
    re.M,
)
BLOCK = re.compile(r"((?:^\.align \d+\n)*)^(?:nonmatching (\w+)[^\n]*\n\n?)?^dlabel (\w+)\n(.*?)^enddlabel \3\n", re.M | re.S)
ADDR = re.compile(r"/\* [0-9A-F]+ ([0-9A-F]{8})")
ROW = re.compile(r"^(\w+) = 0x([0-9A-Fa-f]+);", re.M)
STORE_REF = re.compile(
    r"^\s*(?:/\*[^\n]*\*/\s*)?"
    r"(?:sb|sh|sw|sd|sq|swl|swr|sdl|sdr|swc1|sdc1|swc2|sdc2|sqc2)\s+"
    r"[^\n]*?%(?:lo|gp_rel)\((\w+)\)",
    re.M,
)


def function_text(text, start):
    depth, i = 0, text.index("{", start)
    for j in range(i, len(text)):
        depth += {"{": 1, "}": -1}.get(text[j], 0)
        if depth == 0:
            return text[start:j + 1]
    return text[start:]


def write_text_if_changed(path, text):
    if not path.exists() or path.read_text() != text:
        path.write_text(text)


MACRO_ALIAS = re.compile(r"^[ \t]*#[ \t]*define[ \t]+(\w+)[ \t]+(\w+)[ \t]*$", re.M)


def macro_aliases(text):
    """Map each name to the object-like macros a unit defines as it (`#define D_X name`)."""
    aliases = {}
    for alias, name in MACRO_ALIAS.findall(text):
        aliases.setdefault(name, []).append(alias)
    return aliases


def defined_name(name, aliases):
    """The symbol a C definition of `name` emits, through the unit's macro aliases."""
    return next((target for target, names in aliases.items() if name in names), name)


def names_symbol(code, sym, aliases):
    """Whether C code names `sym`, directly or through one of its macro aliases."""
    return any(re.search(rf"\b{n}\b", code) for n in (sym, *aliases.get(sym, ())))


def alignment_pad(addr, end, body):
    """Whether zero bytes [addr, end) only align the object at `end`."""
    body = re.sub(r'^\s*\.(?:align \d+|asciz "")\s*$', "", body, flags=re.M)
    if any(int(v, 16) for v in VALUE.findall(body)) or re.search(r"^\s*\.(?!byte|short|word)\w", body, re.M):
        return False
    a = next(a for a in (8, 4, 2, 1) if end % a == 0)
    return end - addr < a


def place(version):
    names = {n: int(a, 16) for n, a in ROW.findall((ROOT / "config" / version / "symbol_addrs.txt").read_text())}
    added = 0
    for c in sorted((ROOT / "src" / version).rglob("*.c")):
        unit = c.relative_to(ROOT / "src" / version).with_suffix("").as_posix()
        data = ROOT / "asm" / version / "data" / f"{unit}.sdata.s"
        text = LINE.sub("", c.read_text())
        if not data.exists():
            write_text_if_changed(c, text)
            continue
        out_dir = ROOT / "asm" / version / "nonmatchings" / unit
        out_dir.mkdir(parents=True, exist_ok=True)
        syms = []
        blocks = list(BLOCK.finditer(data.read_text()))
        if not blocks:
            write_text_if_changed(c, text)
            continue
        # An .align raises the whole section's alignment, so no symbol may ask for
        # more than the unit's own start address has.
        first = ADDR.search(blocks[0].group(4))
        start = int(first.group(1), 16) if first else int(blocks[0].group(3)[-8:], 16)
        cap = next(k for k in (3, 2, 1, 0) if start % (1 << k) == 0)
        source = data.read_text()
        starts = []
        for m in blocks:
            a = ADDR.search(m.group(4))
            starts.append(int(a.group(1), 16) if a else int(m.group(3)[-8:], 16))
        for i, m in enumerate(blocks):
            sym = m.group(3)
            addr = starts[i]
            if source[: source.index(f"dlabel {sym}\n")].endswith(PAD_NOTE):
                end = starts[i + 1] if i + 1 < len(blocks) else addr + sum(
                    {"byte": 1, "short": 2, "word": 4}[w] for w in re.findall(r"\.(byte|short|word) ", m.group(4)))
                if alignment_pad(addr, end, m.group(4)):
                    syms.append((addr, f"SDATA_PAD({end - addr})"))
                    continue
            k = next(k for k in (3, 2, 1, 0) if addr % (1 << k) == 0)
            if m.group(1):
                k = min(k, int(re.findall(r"\d+", m.group(1))[-1]))
            align = f".align {min(k, cap)}\n"
            write_text_if_changed(out_dir / f"{sym}.s", f".section .sdata\n\n{align}nonmatching {sym}\n\ndlabel {sym}\n{m.group(4)}enddlabel {sym}\n")
            syms.append((addr, sym))
        mine = {s for _, s in syms}
        active = matching_source(text)
        aliases = function_aliases(active)
        macros = macro_aliases(active)
        # What the C emits itself: defined variables, and literals its functions write.
        anchors = []
        for m in DATA_DEF.finditer(active):
            n = defined_name(m.group(1), macros)
            addr = names.get(n, int(n[2:], 16) if re.fullmatch(r"D_[0-9A-F]{8}", n) else None)
            if addr is not None and any(a == addr for a, _ in syms):
                anchors.append((addr, m.start(), n))
        full = ROOT / "asm" / version / f"{unit}.s"
        refs = {}
        written_symbols = set()
        if full.exists():
            native_text = full.read_text()
            written_symbols = set(STORE_REF.findall(native_text))
            for f in re.finditer(r"^glabel (\w+)\n(.*?)^endlabel \1", native_text, re.M | re.S):
                refs[f.group(1)] = set(re.findall(r"%(?:lo|gp_rel)\((\w+)\)", f.group(2)))
        asm_funcs = set(re.findall(r'^INCLUDE_ASM\([^\n]*\b(\w+)\);', active, re.M))
        still_asm = set().union(*(refs.get(f, set()) for f in asm_funcs)) if asm_funcs else set()
        addr_of = {s: a for a, s in syms}
        for m in DEF.finditer(active):
            code = function_text(active, m.start())
            for sym in refs.get(aliases.get(m.group(1), m.group(1)), ()):
                # An indexed C store can use the containing array's name instead
                # of a native label for its last byte. Written storage is not a
                # compiler literal; its initialized bytes must remain included.
                if (
                    sym in mine
                    and sym not in still_asm
                    and sym not in written_symbols
                    and not names_symbol(code, sym, macros)
                ):
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
            line = f"{sym};" if sym.startswith("SDATA_PAD(") else f'INCLUDE_SDATA(const s32, "{unit}", {sym});'
            text = text[:pos] + ("\n" if pos == len(text) else "") + line + "\n\n" + text[pos:]
            added += 1
        # One blank line before each included item, however often this reruns.
        text = re.sub(r"\n{3,}(?=INCLUDE_(?:RODATA|SDATA)\(|SDATA_PAD\()", "\n\n", text)
        write_text_if_changed(c, text)
    print(f"{version}: {added} INCLUDE_SDATA lines")


if __name__ == "__main__":
    for v in sys.argv[1:] or ["dds1", "dds2"]:
        place(v)
