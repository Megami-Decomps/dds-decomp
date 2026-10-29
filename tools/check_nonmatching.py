#!/usr/bin/env python3
"""Compile every unit containing plain NON_MATCHING C, without asm fallbacks.

    python3 tools/check_nonmatching.py [src/dds1/<unit>.c ...]

Uses the unit's normal compiler and flags, with NON_MATCHING and SKIP_ASM.
Implicit function declarations are errors in this compile. No retail
executable or split assembly is needed.
"""
import os
from pathlib import Path
import re
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parent.parent
GUARD = re.compile(r"^\s*#\s*ifdef\s+NON_MATCHING\s*$", re.M)


def compile_unit(path: Path, output: Path, version: str):
    # GCC 2.96 needs this spelling. The modern -Werror=... form is silently
    # ignored, so compiler-backed negative controls cover this option.
    return subprocess.run(
        [str(ROOT / "tools/cc.sh"), "-DNON_MATCHING", "-DSKIP_ASM",
         "-Werror-implicit-function-declaration", str(path), "-o", str(output)],
        env=dict(os.environ, DDS_VERSION=version), cwd=ROOT,
        capture_output=True, text=True)


def main():
    paths = [Path(a).resolve() for a in sys.argv[1:]] if sys.argv[1:] else [
        p for p in sorted((ROOT / "src").rglob("*.c")) if GUARD.search(p.read_text())]
    failed = 0
    with tempfile.TemporaryDirectory() as directory:
        for path in paths:
            version = path.relative_to(ROOT / "src").parts[0]
            result = compile_unit(path, Path(directory) / "unit.o", version)
            if result.returncode:
                failed += 1
                print(f"{path.relative_to(ROOT)}: DOES NOT COMPILE")
                print(result.stdout + result.stderr)
            else:
                print(f"{path.relative_to(ROOT)}: compiles")
    print(f"{len(paths)} non-matching units, {failed} failed")
    return bool(failed)


if __name__ == "__main__":
    sys.exit(main())
