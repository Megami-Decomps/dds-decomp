#!/usr/bin/env python3
"""Compare a built IRX against the retail one and report where it differs.

    python tools/irx_verify.py build/irx/SDFIOP.IRX orig/irx/SDFIOP.IRX
"""
from __future__ import annotations

import argparse
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
import irx  # noqa: E402


def locate(module: irx.Module, offset: int) -> str:
    """Name the part of the file an offset falls in."""
    base = irx.load_offset(len(irx.iopmod_bytes(module)))
    for s in module.sections:
        # .iopmod sits before the load segment and shares address 0 with
        # .text, so it would otherwise claim .text's offsets.
        if s.type == irx.SHT_NOBITS or not s.size or s.name == ".iopmod":
            continue
        start = s.addr + base
        if start <= offset < start + s.size:
            return f"{s.name}+{offset - start:#x} (vram {s.addr + offset - start:#x})"
    if offset < irx.IOPMOD_OFFSET:
        return "ELF header or program headers"
    return "container metadata"


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("built")
    ap.add_argument("orig")
    args = ap.parse_args()
    built = Path(args.built).read_bytes()
    want = Path(args.orig).read_bytes()
    if built == want:
        print(f"{args.built}: OK")
        return
    module = irx.read(Path(args.orig))
    print(f"{args.built}: DIFFERS from {args.orig}", file=sys.stderr)
    print(f"  size {len(built)}, expected {len(want)}", file=sys.stderr)
    bad = [i for i in range(min(len(built), len(want))) if built[i] != want[i]]
    print(f"  {len(bad)} differing bytes", file=sys.stderr)
    seen = []
    for off in bad:
        where = locate(module, off)
        if where not in seen:
            seen.append(where)
            print(f"  first at {off:#x}: {where}", file=sys.stderr)
        if len(seen) >= 12:
            break
    raise SystemExit(1)


if __name__ == "__main__":
    main()
