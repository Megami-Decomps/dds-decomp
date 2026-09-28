#!/usr/bin/env python3
"""Turn string literals that check_unit.py reports as SHARED back into externs.

    python3 tools/unshare_strings.py src/dds1/<dir>/<unit>.c [...]

A literal is SHARED while an INCLUDE_ASM function of the unit still uses the
same retail string: the object may hold only one copy, so the C must name the
retail symbol until that function is C too. Each literal becomes
`D_XXXXXXXX` with an `extern char D_XXXXXXXX[]; /* "text" */` declaration.
"""
import re
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from pairing import RETAIL, ROOT, load_segments, va_to_off  # noqa: E402
from shared_funcs import DEF, blocks  # noqa: E402

SHARED = re.compile(r"^SHARED rodata of (\w+) \(retail 0x([0-9A-F]{8})\)", re.M)


def c_literal(raw: bytes) -> str:
    out = []
    for c in raw:
        ch = chr(c)
        out.append({"\n": "\\n", "\t": "\\t", "\r": "\\r", '"': '\\"', "\\": "\\\\", "\x1b": "\\x1b"}.get(ch, ch))
    return '"' + "".join(out) + '"'


def fix(path: Path) -> int:
    version = path.resolve().relative_to(ROOT / "src").parts[0]
    elf = (ROOT / RETAIL[version]).read_bytes()
    segs = load_segments(elf)
    out = subprocess.run([sys.executable, str(ROOT / "tools/check_unit.py"), str(path)],
                         capture_output=True, text=True).stdout
    todo = {}
    for func, addr in SHARED.findall(out):
        todo.setdefault(func, set()).add(int(addr, 16))
    if not todo:
        return 0
    text = path.read_text()
    decls, n = {}, 0
    for block in blocks(text):
        m = DEF.search(block) if "{" in block else None
        if not m or m.group(1) not in todo:
            continue
        new = block
        for addr in todo[m.group(1)]:
            off = va_to_off(segs, addr)
            raw = elf[off:elf.index(b"\0", off)]
            lit, sym = c_literal(raw), f"D_{addr:08X}"
            if lit not in new:
                print(f"{path}: {m.group(1)}: literal for {sym} not found, fix by hand")
                continue
            new = new.replace(lit, sym)
            decls[sym] = f"extern char {sym}[]; /* {lit} */"
            n += 1
        text = text.replace(block, new, 1)
    add = [d for s, d in sorted(decls.items()) if not re.search(rf"^extern[^\n]*\b{s}\b", text, re.M)]
    if add:
        at = text.index("\n", text.index("#include")) + 1
        text = text[:at] + "\n" + "\n".join(add) + "\n" + text[at:]
    path.write_text(text)
    return n


if __name__ == "__main__":
    for arg in sys.argv[1:]:
        print(arg, fix(Path(arg)), "literals made extern")
