#!/usr/bin/env python3
"""Find original files that were built without sibling-call optimisation.

    python3 tools/find_nosibcall.py dds1 [--min 10] [--apply]

ee-gcc 2.96 at -O2 turns a call in tail position into `j callee` after the
epilogue. Some DDS files were compiled with -fno-optimize-sibling-calls: there
every such call stays `jal callee` followed by the epilogue. Each function of a
unit is classified from retail asm:

  J  contains a `j` to a function (a sibcall; the file had the optimisation on)
  L  ends with `jal f; <delay>; epilogue; jr $ra` without using f's result
     (a tail call that was NOT turned into a sibcall)
  .  neither (tells nothing)

A candidate file is a run between two J functions holding at least --min L
functions (and at least --ratio of the run) and no J. One L on its own proves little (a taken address, varargs
or a struct return also block sibcalls), but a long run of them with none of
the other kind means the whole file was built that way. Units already listed
in config/<v>/cflags.txt are skipped.

--apply splits each candidate out with tools/split_unit.py (starting right
after the preceding J function, ending at the next J function) and records it
in cflags.txt. Rebuild and compare afterwards; a wrong split shows as a
mismatch.
"""
import argparse
import re
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from find_fragments import functions  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
EPILOGUE = ("ld", "lq", "lw", "jr", "addiu", "daddiu", "nop")
RESULT = re.compile(r"\s*\$2\b|.*,\s*\$2\b")


def kind(body):
    ops = [(op, args) for _, op, args in body]
    while ops and ops[-1][0] == "nop":
        ops.pop()
    if any(op == "j" and ".L" not in args for op, args in ops):
        return "J"
    calls = [i for i, (op, _) in enumerate(ops) if op == "jal"]
    if calls:
        rest = ops[calls[-1] + 2:]
        if rest and any(op == "jr" for op, _ in rest) and all(op in EPILOGUE for op, _ in rest) \
                and not any(RESULT.match(args) for op, args in rest if op != "jr"):
            return "L"
    return "."


def candidates(version, unit, minimum):
    path = ROOT / "asm" / version / f"{unit}.s"
    funcs = [(body[0][0], kind(body)) for _, body in functions(path) if body]
    out, last = [], -1
    for i, (_, k) in enumerate(funcs + [(None, "J")]):
        if k != "J":
            continue
        run = funcs[last + 1:i]
        tails = sum(1 for _, x in run if x == "L")
        if tails >= minimum and last >= 0:
            out.append((run[0][0], funcs[i][0] if i < len(funcs) else None, tails, len(run)))
        last = i
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("version")
    ap.add_argument("--min", type=int, default=10, help="tail calls a run needs (default 10)")
    ap.add_argument("--ratio", type=float, default=0.25,
                    help="minimum share of the run's functions that are such tail calls (default 0.25)")
    ap.add_argument("--apply", action="store_true")
    args = ap.parse_args()
    cflags = ROOT / "config" / args.version / "cflags.txt"
    flagged = {line.split()[0] for line in cflags.read_text().splitlines()
               if line.strip() and not line.startswith("#")}
    found = []
    for c in sorted((ROOT / "src" / args.version).rglob("*.c")):
        unit = c.relative_to(ROOT / "src" / args.version).with_suffix("").as_posix()
        if unit.startswith("sdk/") or unit in flagged or not (ROOT / "asm" / args.version / f"{unit}.s").exists():
            continue
        for start, end, tails, n in candidates(args.version, unit, args.min):
            if tails < args.ratio * n or end is None:
                continue  # too diluted to say, or no sibcalling code after it
            found.append((unit, start, end, tails, n))
            print(f"{unit:28} 0x{start:08X}..{f'0x{end:08X}' if end else 'end':10} {tails:3} jal tail calls, 0 sibcalls, {n} functions")
    if not args.apply:
        return
    # Later runs first, so earlier split points still name the unit they lie in.
    for unit, start, end, tails, n in sorted(found, key=lambda f: -f[1]):
        cmd = [sys.executable, str(ROOT / "tools/split_unit.py"), args.version, unit, f"0x{start:08X}"]
        if end:
            cmd.append(f"0x{end:08X}")
        subprocess.run(cmd, check=True)
        with cflags.open("a") as f:
            f.write(f"game/code_{start:08X}  -fno-optimize-sibling-calls  # {tails} tail calls in {n} functions, "
                    "all jal+epilogue, no j to a function; neighbours sibcall (find_nosibcall.py)\n")


if __name__ == "__main__":
    main()
