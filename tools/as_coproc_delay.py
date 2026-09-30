#!/usr/bin/env python3
"""Emulate retail ee-as delay-slot filling after mtc1 (and cvt.w.s / FP loads).

Retail's assembler never moves an instruction that reads the FPR written by
the immediately preceding `mtc1` (or `cvt.w.s`) into the delay slot of the
following branch
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
# Same rule after an FP load: retail has 248 (DDS1) / 284 (DDS2)
# `lwc1 $fN,off(reg); <FP op reading $fN>; <branch>` sequences and never puts
# the dependent op in the delay slot (DDS1 has no filled form at all).
LWCF = re.compile(r"\s+(?:lwc1|l\.s)\s+(\$f\d+),")
LABEL = re.compile(r"^\$?\.?L\w*:\s*$")
# Same rule after a float->int conversion: retail has no `cvt.w.s $fN; jr; swc1 $fN`
# in DDS1 or DDS2 (45 and 59 sequences with a dependent instruction, all unfilled).
CVTWS = re.compile(r"\s+(?:cvt|trunc)\.w\.s\s+(\$f\d+),")
# `la $rd,sym($rs)` expands to lui/addiu/addu; retail (5 sequences in DDS1/DDS2)
# never puts the closing addu in a following `jr`'s delay slot.
LA_INDEXED = re.compile(r"\s+la\s+\$\w+,[^\s(]+\(\$\w+\)\s*$")
# The same holds for a load or store whose base+offset needs lui/addu/op (a
# symbol or an offset outside 16 bits on a base register): retail has 653
# `lui; addu; lw|sw..; branch; nop` sequences and 4 with the access in the slot.
MEM_INDEXED = re.compile(r"\s+(?:l[bhwdq]u?|s[bhwdq]|lwc1|swc1)\s+\$f?\w+,([^\s(]+)\(\$\w+\)\s*$")


def expands_indexed(line):
    m = MEM_INDEXED.match(line)
    if not m:
        return False
    offset = m.group(1)
    if offset.startswith("%"):
        return False  # %lo()/%gp_rel(): one instruction
    try:
        return not -0x8000 <= int(offset, 0) <= 0x7FFF
    except ValueError:
        return True  # symbolic offset on a base register


# An unaligned store (sdl/sdr/swl/swr) that cc1 left for the assembler stays
# out of the following branch's delay slot: retail's struct-copy setters end
# `sdl; sdr; jr; nop` (the filled retail cases are cc1's own .set noreorder fills).
UNALIGNED_STORE = re.compile(r"\s+(?:sdl|sdr|swl|swr)\s")


# Retail never fills a branch delay slot with mfhi/mflo (DDS1 135 and DDS2 101
# `mfhi|mflo; branch` sequences, none with the move in the slot).
HILO = re.compile(r"\s+(mfhi|mflo)\s")


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
        move = None
        if reorder and 0 < i < len(lines) - 1:
            move = MTC1.match(lines[i - 1]) or CVTWS.match(lines[i - 1]) or LWCF.match(lines[i - 1])
        next_is_branch = reorder and i < len(lines) - 1 and BRANCH.match(lines[i + 1])
        # cc1 writes the hi/lo hazard filler as a `#nop` comment line after the move.
        j = i + 1
        while j < len(lines) and lines[j].strip().startswith("#"):
            j += 1
        hilo_before_branch = reorder and j < len(lines) and HILO.match(line) and BRANCH.match(lines[j])
        # A branch target never moves: retail keeps `.L: insn; jr $31; nop` (21
        # sequences, all swappable) where ee-as would pull the labelled insn into the slot.
        labelled = i > 0 and LABEL.match(lines[i - 1]) and line.strip() and not line.strip().startswith((".", "#")) \
            and not BRANCH.match(line)
        # ... nor the store right after one (`sdr; sw; jr; nop` in the 0x24/0x2C copies).
        after_unaligned = i > 0 and UNALIGNED_STORE.match(lines[i - 1]) and MEM_INDEXED.match(line)
        if (move and next_is_branch and reads_fpr(line, move.group(1))) or (
                next_is_branch and (LA_INDEXED.match(line) or expands_indexed(line) or labelled
                                    or UNALIGNED_STORE.match(line) or after_unaligned)) \
                or hilo_before_branch:
            out += ["\t.set\tnoreorder", line, "\t.set\treorder"]
        else:
            out.append(line)
    open(dst, "w").write("\n".join(out))


if __name__ == "__main__":
    main(sys.argv[1], sys.argv[2])
