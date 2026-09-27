#!/usr/bin/env python3
"""Rewrite splat/spimdisasm assembly into syntax the original ee-as accepts.

C translation units are assembled by Sony's ee-as (binutils 2.9-ee), because
its macro expansion (`move` -> daddu, R5900 hazard handling, delay-slot
filling) is what produced retail. Functions still in assembly are pulled into
those units with INCLUDE_ASM, so their text must also be ee-as syntax:

  * `%gp_rel(sym)` is not understood: the operand is replaced by the numeric
    offset taken from the instruction word splat prints in the line comment.
  * Instructions ee-as rejects (spimdisasm's VU0 macro-mode spelling) become
    `.word` with the original text kept as a comment.

Both rewrites take the encoding from retail, so they are byte-exact; the cost
is that those instructions carry no relocation, which is irrelevant for a
matching (non-shiftable) build.

    python tools/eeas_compat.py IN.s OUT.s
"""
from __future__ import annotations

import re
import sys
from pathlib import Path

# /* ROM VRAM BYTES */  mnemonic operands   -- BYTES as printed by spimdisasm (file order)
INSN = re.compile(r"^(?P<pre>\s*/\*\s*[0-9A-F]+\s+[0-9A-F]+\s+(?P<word>[0-9A-F]{8})\s*\*/\s*)(?P<mn>\S+)(?P<ops>.*)$")
GP_REL = re.compile(r"%gp_rel\([^()]*(?:\([^()]*\)[^()]*)*\)")

# Mnemonics spimdisasm spells differently from ee-as (VU0 macro mode, COP2).
EE_AS_UNSUPPORTED = re.compile(r"^(v[a-z0-9]+(\.[xyzw]+)?|vcallms|vcallmsr|qmfc2|qmtc2|cfc2|ctc2|lqc2|sqc2)$")


def word_value(word: str) -> int:
    return int.from_bytes(bytes.fromhex(word), "little")


def convert_line(line: str) -> str:
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
        return f"{m.group('pre')}{mn}{GP_REL.sub(str(imm), ops)}"
    return line


def convert(text: str) -> str:
    return "\n".join(convert_line(line) for line in text.split("\n"))


def main() -> None:
    if len(sys.argv) != 3:
        raise SystemExit(__doc__)
    src, dst = Path(sys.argv[1]), Path(sys.argv[2])
    dst.parent.mkdir(parents=True, exist_ok=True)
    dst.write_text(convert(src.read_text()))


if __name__ == "__main__":
    main()
