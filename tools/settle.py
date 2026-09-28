#!/usr/bin/env python3
"""Make every unit check clean by reverting what no longer matches.

    python3 tools/settle.py [dds1 dds2] [-j 12]

Renames, ports and reverts change the preprocessed text of a unit, and
ee-gcc 2.96's code for other functions can change with it (CONTEXT in
docs/idioms.md). This checks every unit, reverts the functions check_unit
flags (tools/revert_differing.py), re-splits so the reverted functions have
their asm files again, and repeats until nothing changes.
"""
import argparse
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent


def unclean(unit):
    r = subprocess.run([sys.executable, str(ROOT / "tools/check_unit.py"), str(unit)], capture_output=True, text=True)
    return unit if r.returncode else None


def resplit(versions):
    """Split again (reverted functions need their asm files) and regenerate the
    ee-as copies of the asm the unit compiles include (build/eeasm)."""
    subprocess.run([sys.executable, str(ROOT / "configure.py"), "--force-split", *versions],
                   cwd=ROOT, check=True, capture_output=True)
    targets = subprocess.run(["ninja", "-t", "targets", "all"], cwd=ROOT, capture_output=True,
                             text=True).stdout.splitlines()
    eeasm = [t.split(":")[0] for t in targets if t.startswith("build/eeasm/")]
    subprocess.run(["ninja", *eeasm], cwd=ROOT, capture_output=True)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("versions", nargs="*", default=["dds1", "dds2"])
    ap.add_argument("-j", type=int, default=12)
    args = ap.parse_args()
    resplit(args.versions)
    for round_ in range(1, 9):
        units = [u for v in args.versions for u in sorted((ROOT / "src" / v).rglob("*.c"))]
        with ThreadPoolExecutor(args.j) as pool:
            bad = [u for u in pool.map(unclean, units) if u]
        print(f"round {round_}: {len(bad)} unclean unit(s)", flush=True)
        if not bad:
            return
        for u in bad:
            out = subprocess.run([sys.executable, str(ROOT / "tools/revert_differing.py"), str(u)],
                                 capture_output=True, text=True).stdout.strip()
            print(" ", out, flush=True)
        resplit(args.versions)
    sys.exit("still unclean after 8 rounds; inspect the units above")


if __name__ == "__main__":
    main()
