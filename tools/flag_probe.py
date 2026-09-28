#!/usr/bin/env python3
"""Which single ee-gcc 2.96 option makes a stuck function match?

    python3 tools/flag_probe.py src/dds1/<dir>/<unit>.c <func> <candidate.c>
    python3 tools/flag_probe.py --batch build/parked/dds1

The candidate file holds the function's best natural C (its definition, plus
any declarations it needs). It replaces the function's INCLUDE_ASM line in a
copy of the unit, which is compiled once per option below; the report lists
the options under which the function is byte-identical to retail.

--batch reads every build/parked/<v>/<dir>/<unit>/<func>.c and prints, per
option, which functions it alone fixes, grouped by address. An option earns a
place in config/<v>/cflags.txt only when it explains a contiguous run of
functions of one file (like -fno-optimize-sibling-calls did); a lone function
that matches under some option is not evidence and stays INCLUDE_ASM.
"""
import argparse
import re
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
# Options ee-gcc 2.96 accepts that a makefile might have set per file (checked).
OPTIONS = [
    "-O1", "-O3", "-Os",
    "-fno-optimize-sibling-calls", "-fno-schedule-insns", "-fschedule-insns",
    "-fno-schedule-insns2", "-fno-sched-interblock", "-fno-sched-spec", "-fno-delayed-branch",
    "-fno-strength-reduce", "-fno-gcse", "-fno-cse-follow-jumps", "-fno-cse-skip-blocks",
    "-fno-rerun-cse-after-loop", "-fno-rerun-loop-opt", "-fno-expensive-optimizations",
    "-fno-thread-jumps", "-fno-peephole", "-fno-regmove", "-fno-reorder-blocks",
    "-fno-caller-saves", "-fno-force-mem", "-fno-inline", "-fno-defer-pop",
    "-fno-omit-frame-pointer", "-funroll-loops", "-g", "-mno-gpopt",
]


def probe(unit: Path, func: str, candidate: Path, options=OPTIONS):
    text = unit.read_text()
    line = re.compile(rf'^INCLUDE_ASM\([^\n]*\b{func}\);$', re.M)
    if not line.search(text):
        raise SystemExit(f"{func}: no INCLUDE_ASM line in {unit}")
    source = line.sub(lambda _: candidate.read_text().strip(), text, count=1)
    hits = []
    with tempfile.TemporaryDirectory() as tmp:
        src = Path(tmp) / unit.name
        src.write_text(source)
        for opt in [""] + options:
            r = subprocess.run([sys.executable, str(ROOT / "tools/check_unit.py"), str(unit), "--func", func,
                                "--source", str(src), "--cflags", opt], capture_output=True, text=True)
            if re.search(rf"^OK\s+{func}\b", r.stdout, re.M):
                if opt == "":
                    return ["(default: matches without any option)"]
                hits.append(opt)
    return hits


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("unit", nargs="?", type=Path)
    ap.add_argument("func", nargs="?")
    ap.add_argument("candidate", nargs="?", type=Path)
    ap.add_argument("--batch", type=Path, help="directory of parked candidates (build/parked/<v>)")
    args = ap.parse_args()
    if not args.batch:
        print(" ".join(probe(args.unit.resolve(), args.func, args.candidate)) or "no single option matches")
        return
    version = args.batch.name
    by_option = {}
    for cand in sorted(args.batch.rglob("*.c")):
        unit = ROOT / "src" / version / f"{cand.parent.relative_to(args.batch).as_posix()}.c"
        if not unit.exists():
            continue
        try:
            hits = probe(unit, cand.stem, cand)
        except SystemExit:
            continue  # already matched or removed
        print(f"{cand.parent.relative_to(args.batch)}/{cand.stem}: {' '.join(hits) or '-'}", flush=True)
        for h in hits:
            by_option.setdefault(h, []).append(f"{cand.parent.relative_to(args.batch)}/{cand.stem}")
    for opt, fs in sorted(by_option.items(), key=lambda kv: -len(kv[1])):
        print(f"\n{opt}: {len(fs)}\n  " + "\n  ".join(fs))


if __name__ == "__main__":
    main()
