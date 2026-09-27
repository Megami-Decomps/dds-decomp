#!/usr/bin/env python3
"""Find false function starts: code romwright split off the function it belongs to.

    python3 tools/find_fragments.py dds1 [--write]

ee-gcc emits one frame per function. A "function" that uses a caller's frame
without building its own is a piece of that caller, usually a switch case or a
shared tail that romwright took for an entry because it is a jump/branch target.
Such a piece can never be written as C, and it splits its owner as well.

A start is reported when, before the code sets up any frame, it
  - reads a callee-saved register ($16-$23, $30) that it never wrote or saved, or
  - restores $31 from the stack, or pops a frame it did not push.

With --write, the starts go to config/<v>/not_functions.txt. romwright_sync.py
drops them from the generated symbols; `configure.py --force-split` then
disassembles each fragment as part of the function before it.
"""
import argparse
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
INSN = re.compile(r"/\* [0-9A-F]+ ([0-9A-F]{8}) [0-9A-F]{8} \*/\s+(\S+)\s*(.*)$")
GLABEL = re.compile(r"^glabel (\w+)")
REG = re.compile(r"\$(\d+)\b")
SAVED = {16, 17, 18, 19, 20, 21, 22, 23, 30}
STORES = ("sb", "sh", "sw", "sd", "sq", "swl", "swr", "sdl", "sdr", "swc1", "sqc2")
READS_ALL = ("b", "j", "jr", "jalr", "mthi", "mtlo", "mult", "multu", "div", "divu",
             "mult1", "multu1", "div1", "divu1", "madd", "maddu", "madd1", "maddu1")


def functions(path):
    """(name, [(vram, op, operands)]) per glabel of a splat asm file."""
    name, body = None, []
    for line in path.read_text().splitlines():
        if m := GLABEL.match(line):
            if name:
                yield name, body
            name, body = m[1], []
        elif name and (m := INSN.search(line)):
            body.append((int(m[1], 16), m[2], m[3]))
    if name:
        yield name, body


def fragment_reason(body):
    written, framed = set(), False
    for _, op, args in body:
        regs = [int(r) for r in REG.findall(args)]
        base = op.split(".")[0]
        if base in ("addiu", "daddiu") and regs[:2] == [29, 29]:
            if args.rstrip().split(",")[-1].strip().startswith("-"):
                framed = True
            elif not framed:
                return "pops a frame it did not push"
            continue
        if framed:
            return None
        if base in STORES:
            src, addr = regs[0], regs[1:]
            if 29 in addr and src in SAVED:
                written.add(src)  # a save of the caller's value, not a use
                continue
            reads, dests = regs, []
        elif base in ("ld", "lw", "lq") and regs and regs[0] == 31 and 29 in regs[1:]:
            return "restores $31 from a frame it did not push"
        elif base in READS_ALL or base.startswith(("b", "t")):
            reads, dests = regs, []
        else:
            dests, reads = regs[:1], regs[1:]
        for r in reads:
            if r in SAVED and r not in written:
                return f"reads ${r} before setting it"
        written.update(dests)
        if base in ("jr",) and 31 in regs:
            return None
    return None


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("version")
    ap.add_argument("--write", action="store_true")
    args = ap.parse_args()
    found = []
    for path in sorted((ROOT / "asm" / args.version).rglob("*.s")):
        # Unit files repeat what nonmatchings/ holds; data has no code; SDK code
        # includes hand-written asm (context switches, setjmp) that legitimately
        # reads callee-saved registers.
        if path.relative_to(ROOT / "asm" / args.version).parts[0] in ("data", "nonmatchings", "sdk", "matchings"):
            continue
        funcs = list(functions(path))
        for name, body in funcs:
            if body and (why := fragment_reason(body)):
                found.append((body[0][0], name, why, path.relative_to(ROOT)))
        found += branched_into(funcs, path)
        found += fallen_into(funcs, path)
    found = sorted({f[0]: f for f in found}.values())
    for addr, name, why, path in found:
        print(f"0x{addr:08X}  {name:32} {why:44} {path}")
    print(f"{len(found)} fragments")
    if args.write:
        out = ROOT / "config" / args.version / "not_functions.txt"
        # Starts already folded away are no longer in the asm; keep them.
        old = {}
        if out.exists():
            for line in out.read_text().splitlines():
                if line.strip() and not line.startswith("#"):
                    old[int(line.split()[0], 16)] = line.split("#", 1)[1].strip() if "#" in line else ""
        rows = {**old, **{a: why for a, _, why, _ in found}}
        out.write_text("# False function starts (tools/find_fragments.py); romwright_sync.py drops them.\n" +
                       "".join(f"0x{a:08X}  # {rows[a]}\n" for a in sorted(rows)))
        print(f"-> {out.relative_to(ROOT)} ({len(rows)} starts)")


LOCAL_BRANCH = re.compile(r"^(?:b\w*|j)$")
LOCAL_LABEL = re.compile(r"\.L([0-9A-F]{8})\b")


def branched_into(funcs, path):
    """Starts that a neighbouring function jumps into with a local branch (or a
    jump-table entry): the code between is one function, split at a case label."""
    starts = sorted(body[0][0] for _, body in funcs if body)
    names = {body[0][0]: name for name, body in funcs if body}
    ends = {body[0][0]: body[-1][0] + 4 for _, body in funcs if body}
    text = path.read_text()
    tables = {}
    for m in re.finditer(r"^dlabel (jtbl_[0-9A-F]{8})\n(.*?)^enddlabel", text, re.M | re.S):
        tables[m.group(1)] = [int(x, 16) for x in LOCAL_LABEL.findall(m.group(2))]
    out = []
    for name, body in funcs:
        if not body:
            continue
        lo, hi = body[0][0], ends[body[0][0]]
        targets = [int(m.group(1), 16) for _, op, args in body if LOCAL_BRANCH.match(op)
                   for m in LOCAL_LABEL.finditer(args)]
        for _, op, args in body:
            for jt in re.findall(r"%lo\((jtbl_[0-9A-F]{8})\)", args):
                targets += tables.get(jt, [])
        for t in targets:
            if lo <= t < hi:
                continue
            a, b = (lo, t) if t > lo else (t, lo)
            for s in starts:
                if a < s <= b:
                    out.append((s, names[s], f"branched into from {name}", path.relative_to(ROOT)))
    return out


TERMINATORS = ("jr", "j", "b", "eret")


def fallen_into(funcs, path):
    """Starts the previous function runs into: its last instruction (ignoring
    alignment nops) is not the delay slot of a return, jump or unconditional
    branch, so execution continues past the "start"."""
    out = []
    for (prev_name, prev), (name, body) in zip(funcs, funcs[1:]):
        if not prev or not body:
            continue
        ops = [op for _, op, _ in prev]
        while ops and ops[-1] == "nop":
            ops.pop()  # alignment padding, or a nop delay slot
        if ops and ops[-1] in TERMINATORS + ("syscall", "break"):
            continue
        if len(ops) >= 2 and ops[-2] in TERMINATORS:
            continue
        if name.startswith("func_"):  # named entry points (_start) are real
            out.append((body[0][0], name, f"fallen into from {prev_name}", path.relative_to(ROOT)))
    return out


if __name__ == "__main__":
    main()
