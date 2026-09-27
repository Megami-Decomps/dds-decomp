#!/usr/bin/env python3
"""Flatten a C file's headers into ctx.c for m2c, decomp.me and the permuter.

    python tools/m2ctx.py src/dds1/game/sdkTask.c        # writes ./ctx.c
    python tools/m2ctx.py src/dds1/game/sdkTask.c -o -   # stdout

Only declarations survive: the file's own function bodies are dropped by the
preprocessor pass because INCLUDE_ASM expands to nothing under M2CTX, and the
translation unit is reduced to its #include lines first.
"""
from __future__ import annotations

import argparse
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
INCLUDE = re.compile(r'^\s*#\s*include\s+["<][^">]+[">]', re.M)


def context(source: Path) -> str:
    includes = "\n".join(INCLUDE.findall(source.read_text()))
    cmd = ["cpp", "-P", "-undef", "-nostdinc", "-DM2CTX", "-D__GNUC__=2", "-D__mips__", "-D_R5900",
           f"-I{ROOT / 'include'}", f"-I{ROOT / 'src'}", f"-I{source.parent}", "-"]
    out = subprocess.run(cmd, input=includes, capture_output=True, text=True, check=True).stdout
    return re.sub(r"\n{3,}", "\n\n", out).strip() + "\n"


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("source", type=Path)
    ap.add_argument("-o", "--output", default=str(ROOT / "ctx.c"))
    args = ap.parse_args()
    text = context(args.source.resolve())
    if args.output == "-":
        sys.stdout.write(text)
    else:
        Path(args.output).write_text(text)
        print(f"wrote {args.output}")


if __name__ == "__main__":
    main()
