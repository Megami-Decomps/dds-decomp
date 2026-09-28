#!/usr/bin/env python3
"""Rewrite splat/spimdisasm assembly into syntax the original ee-as accepts.

C translation units are assembled by Sony's ee-as (binutils 2.9-ee), because
its macro expansion (`move` -> daddu, R5900 hazard handling, delay-slot
filling) is what produced retail. Functions still in assembly are pulled into
those units with INCLUDE_ASM, so their text must also be ee-as syntax:

  * `%gp_rel(sym)` is not understood: the operand is replaced by the numeric
    offset taken from the instruction word splat prints in the line comment.
  * A float load from the retail `.lit4` pool (`lwc1 $fN, %gp_rel(D_...)($28)`)
    becomes `li.s $fN, <value>`, which is what ee-gcc emitted: ee-as puts the
    constant into the unit's own `.lit4` (merging repeats within the object) and
    the linker resolves the offset. C functions and asm functions of a unit then
    build its `.lit4` together, in source order, exactly as the original object
    did. Needs the retail ELF and gp (config/versions.json) for the version.
  * Instructions ee-as rejects (spimdisasm's VU0 macro-mode spelling) become
    `.word` with the original text kept as a comment.

The other rewrites take the encoding from retail, so they are byte-exact; the
cost is that those instructions carry no relocation, which is irrelevant for a
matching (non-shiftable) build.

    python tools/eeas_compat.py IN.s OUT.s
"""
from __future__ import annotations

import json
import re
import struct
import sys
from pathlib import Path

# /* ROM VRAM BYTES */  mnemonic operands   -- BYTES as printed by spimdisasm (file order)
INSN = re.compile(r"^(?P<pre>\s*/\*\s*[0-9A-F]+\s+[0-9A-F]+\s+(?P<word>[0-9A-F]{8})\s*\*/\s*)(?P<mn>\S+)(?P<ops>.*)$")
GP_REL = re.compile(r"%gp_rel\([^()]*(?:\([^()]*\)[^()]*)*\)")

# Mnemonics spimdisasm spells differently from ee-as (VU0 macro mode, COP2).
EE_AS_UNSUPPORTED = re.compile(r"^(v[a-z0-9]+(\.[xyzw]+)?|vcallms|vcallmsr|qmfc2|qmtc2|cfc2|ctc2|lqc2|sqc2)$")


def word_value(word: str) -> int:
    return int.from_bytes(bytes.fromhex(word), "little")


LITERAL_LOAD = re.compile(r"^\s+(\$f\d+),\s*%gp_rel\(\w+\)\(\$28\)\s*$")


class Pool:
    """Retail .lit4 words by address, for one version."""

    def __init__(self, version: str):
        root = Path(__file__).resolve().parent.parent
        info = json.loads((root / "config/versions.json").read_text())[version]
        self.gp = int(info["gp"], 16)
        sys.path.insert(0, str(Path(__file__).resolve().parent))
        from pairing import RETAIL, load_segments, va_to_off
        self.elf = (root / RETAIL[version]).read_bytes()
        self.segs, self.va_to_off = load_segments(self.elf), va_to_off
        self.lo, self.hi = lit4_range(self.elf)

    def word(self, addr: int) -> int | None:
        if not self.lo <= addr < self.hi:
            return None
        return int.from_bytes(self.elf[self.va_to_off(self.segs, addr):][:4], "little")


def lit4_range(elf: bytes) -> tuple[int, int]:
    """[start, end) of the .lit4 section from the ELF section headers."""
    shoff = int.from_bytes(elf[0x20:0x24], "little")
    shnum = int.from_bytes(elf[0x30:0x32], "little")
    shstrndx = int.from_bytes(elf[0x32:0x34], "little")
    hdr = lambda i: [int.from_bytes(elf[shoff + i * 40 + k:shoff + i * 40 + k + 4], "little") for k in range(0, 40, 4)]
    names = hdr(shstrndx)[4]
    for i in range(shnum):
        h = hdr(i)
        name = elf[names + h[0]:elf.index(b"\0", names + h[0])]
        if name == b".lit4":
            return h[3], h[3] + h[5]
    return 0, 0


def as_float(bits: int) -> str:
    value = struct.unpack("<f", bits.to_bytes(4, "little"))[0]
    text = f"{value:.9e}"
    assert struct.unpack("<f", struct.pack("<f", float(text)))[0] == value
    return text


def convert_line(line: str, pool: Pool | None = None) -> str:
    m = INSN.match(line)
    if not m:
        return line
    mn = m.group("mn")
    ops = m.group("ops")
    value = word_value(m.group("word"))
    if EE_AS_UNSUPPORTED.match(mn):
        return f"{m.group('pre')}.word 0x{value:08X} /* {mn}{ops.rstrip()} */"
    if "%gp_rel(" in ops:
        imm = value & 0xFFFF
        if imm >= 0x8000:
            imm -= 0x10000
        if pool and mn == "lwc1" and (lit := LITERAL_LOAD.match(ops)):
            bits = pool.word(pool.gp + imm)
            # ee-as materialises 0.0 and values with a zero low half inline, so
            # a pooled constant is always one of the others.
            if bits:
                return f"{m.group('pre')}li.s {lit.group(1)}, {as_float(bits)}"
        return f"{m.group('pre')}{mn}{GP_REL.sub(str(imm), ops)}"
    return line


def convert(text: str, pool: Pool | None = None) -> str:
    return "\n".join(convert_line(line, pool) for line in text.split("\n"))


def main() -> None:
    if len(sys.argv) != 3:
        raise SystemExit(__doc__)
    src, dst = Path(sys.argv[1]), Path(sys.argv[2])
    dst.parent.mkdir(parents=True, exist_ok=True)
    version = next((p for p in src.resolve().parts if p in ("dds1", "dds2")), None)
    dst.write_text(convert(src.read_text(), Pool(version) if version else None))


if __name__ == "__main__":
    main()
