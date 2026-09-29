#!/usr/bin/env python3
"""Emulate retail ee-as delay-slot filling after mtc1.

Retail's assembler never moves an instruction that reads the FPR written by
the immediately preceding `mtc1` into the delay slot of the following branch
(DDS1 alone has hundreds of `mtc1 $r,$fN; cvt.s.w $fN,$fN; b; nop` sequences
and no swapped form). The ee-gcc 2.96 assembler treats the move as
interlocked and swaps it. No `as` option reproduces the retail rule (`-O0`,
`-mcpu=*` and the 991111 assemblers all differ elsewhere), so this pass runs
on cc1 output before `as` and puts such an instruction in its own
`.set noreorder` region, which leaves it where cc1 placed it and gives the
branch a `nop` delay slot.

    as_coproc_delay.py in.s out.s   (in place when both are the same path)
"""
import re
import sys

BRANCH = re.compile(
    r"\s+(b|bal|j|jal|jr|jalr|beq|bne|beqz|bnez|blez|bgtz|bltz|bgez|bltzal|bgezal|bc1t|bc1f)\s")
MTC1 = re.compile(r"\s+mtc1\s+\$\w+,(\$f\d+)\s*$")


def reads_fpr(line, fpr):
    parts = line.split(None, 1)
    if len(parts) < 2 or parts[0].startswith((".", "$")):
        return False
    regs = re.findall(r"\$f\d+", parts[1])
    if parts[0] in ("swc1", "s.s"):
        return fpr in regs
    return fpr in regs[1:]


def main(src, dst):
    lines = open(src).read().split("\n")
    out = []
    reorder = True
    for i, line in enumerate(lines):
        directive = line.strip()
        if directive == ".set\tnoreorder":
            reorder = False
        elif directive == ".set\treorder":
            reorder = True
        move = MTC1.match(lines[i - 1]) if reorder and 0 < i < len(lines) - 1 else None
        if move and BRANCH.match(lines[i + 1]) and reads_fpr(line, move.group(1)):
            out += ["\t.set\tnoreorder", line, "\t.set\treorder"]
        else:
            out.append(line)
    open(dst, "w").write("\n".join(out))


if __name__ == "__main__":
    main(sys.argv[1], sys.argv[2])
