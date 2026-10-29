#!/usr/bin/env python3
"""Compile every unit containing plain NON_MATCHING C, without asm fallbacks.

    python3 tools/check_nonmatching.py [src/dds1/<unit>.c ...]

Uses the unit's normal compiler and flags, with NON_MATCHING and SKIP_ASM.
No retail executable or split assembly is needed.
"""
import os
from pathlib import Path
import re
import subprocess
import sys
import tempfile

ROOT = Path(__file__).resolve().parent.parent
GUARD = re.compile(r"^\s*#\s*ifdef\s+NON_MATCHING\s*$", re.M)


def main():
    paths = [Path(a).resolve() for a in sys.argv[1:]] if sys.argv[1:] else [
        p for p in sorted((ROOT / "src").rglob("*.c")) if GUARD.search(p.read_text())]
    failed = 0
    with tempfile.TemporaryDirectory() as directory:
        for path in paths:
            version = path.relative_to(ROOT / "src").parts[0]
            result = subprocess.run(
                [str(ROOT / "tools/cc.sh"), "-DNON_MATCHING", "-DSKIP_ASM",
                 str(path), "-o", str(Path(directory) / "unit.o")],
                env=dict(os.environ, DDS_VERSION=version), cwd=ROOT,
                capture_output=True, text=True)
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
