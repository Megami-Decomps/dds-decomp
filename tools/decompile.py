#!/usr/bin/env python3
"""Run m2c on a not-yet-decompiled function with its unit's context.

    python tools/decompile.py func_0019D160            # dds1 by default
    python tools/decompile.py func_0019D160 -v dds2
    python tools/decompile.py func_0019D160 -- --stack-structs   # extra m2c flags

Finds asm/<v>/nonmatchings/**/<func>.s, flattens the owning C file's headers
(tools/m2ctx.py) and prints m2c's output for the ee-gcc target.
"""
from __future__ import annotations

import argparse
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import m2ctx  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("function")
    ap.add_argument("-v", "--version", default="dds1")
    ap.add_argument("m2c_args", nargs="*")
    args = ap.parse_args()

    hits = sorted((ROOT / "asm" / args.version / "nonmatchings").rglob(f"{args.function}.s"))
    if not hits:
        raise SystemExit(f"{args.function}: no nonmatching asm under asm/{args.version}/nonmatchings "
                         "(is its range a `c` subsegment in the yaml?)")
    asm = hits[0]
    unit = asm.parent.relative_to(ROOT / "asm" / args.version / "nonmatchings")
    source = ROOT / "src" / args.version / f"{unit}.c"

    cmd = [sys.executable, "-m", "m2c.main", "-t", "mipsee-gcc-c", *args.m2c_args]
    with tempfile.NamedTemporaryFile("w", suffix=".c") as ctx:
        if source.exists():
            ctx.write(m2ctx.context(source))
            ctx.flush()
            cmd += ["--context", ctx.name]
        cmd.append(str(asm))
        sys.exit(subprocess.run(cmd).returncode)


if __name__ == "__main__":
    main()
