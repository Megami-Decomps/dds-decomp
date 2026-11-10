#!/usr/bin/env python3
"""Compare the built IRX against retail one function at a time.

    python tools/irx_diff.py SDFIOP [function]

Lists every .text chunk that differs, then disassembles the first one
side by side. Run after ninja -f build-irx.ninja.
"""
from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path

import rabbitizer

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
import irx  # noqa: E402
import irx_config  # noqa: E402

rabbitizer.config.regNames_gprAbiNames = rabbitizer.Abi.NUMERIC
SYM = re.compile(r"^(\w+)\s*=\s*(0x[0-9A-Fa-f]+);\s*//\s*type:(\w+)(?:\s+size:(0x[0-9A-Fa-f]+))?")


def symbols(cfg: dict) -> list[tuple[str, int, int]]:
    """(name, addr, size) for every .text chunk, in address order."""
    out = []
    for line in irx_config.path(cfg, "symbols").read_text().splitlines():
        m = SYM.match(line.strip())
        if m and m[4]:
            out.append((m[1], int(m[2], 16), int(m[4], 16)))
    return sorted(out, key=lambda s: s[1])


def text(path: Path) -> tuple[bytes, int]:
    module = irx.read(path)
    for s in module.sections:
        if s.name == ".text":
            return s.data, s.addr
    raise SystemExit(f"{path}: no .text")


def disasm(data: bytes, addr: int) -> list[str]:
    out = []
    for i in range(0, len(data), 4):
        word = int.from_bytes(data[i:i + 4], "little")
        ins = rabbitizer.Instruction(word, vram=addr + i)
        out.append(f"{addr + i:08X}  {ins.disassemble(immOverride=None, extraLJust=0)}")
    return out


def built_symbols(elf: Path) -> dict:
    """name -> (addr, span to the next symbol) from the linked ELF."""
    import subprocess
    nm = ROOT / "tools" / "bin" / "mips-ps2-decompals-nm"
    out = subprocess.run([nm, "-n", str(elf)], capture_output=True, text=True).stdout
    rows = []
    for line in out.splitlines():
        f = line.split()
        if len(f) == 3 and f[1] in "tT" and not f[2].startswith(("L_", "$", "_", "gcc2")):
            rows.append((int(f[0], 16), f[2]))
    syms = {}
    for i, (addr, name) in enumerate(rows):
        end = rows[i + 1][0] if i + 1 < len(rows) else addr
        syms[name] = (addr, end - addr)
    return syms


def stale_warning(cfg: dict, built: Path) -> None:
    """Say so when the image predates the source.

    The diff reads whatever was packed last, so a compile that failed leaves the
    previous image in place and every function still diffs clean.
    """
    if not built.exists():
        raise SystemExit(f"{built}: not built yet; run ninja -f build-irx.ninja")
    newest = max((p.stat().st_mtime for p in (irx_config.path(cfg, "src"),)
                  if p.exists()), default=0)
    if newest > built.stat().st_mtime:
        print(f"warning: {built.name} is older than {cfg['paths']['src']}; "
              f"the last build failed or did not run, so this diff is stale",
              file=sys.stderr)


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("module")
    ap.add_argument("function", nargs="?")
    ap.add_argument("--sizes", action="store_true", help="only report chunks whose size changed")
    args = ap.parse_args()
    cfg = irx_config.load(args.module)
    want, base = text(irx_config.path(cfg, "target"))
    built_path = irx_config.path(cfg, "build").parent / f"{args.module}.IRX"
    stale_warning(cfg, built_path)
    got, got_base = text(built_path)

    if args.sizes:
        bs = built_symbols(irx_config.path(cfg, "build") / f"{args.module}.elf")
        total = 0
        for name, addr, size in symbols(cfg):
            baddr, bsize = bs.get(name, (addr, size))
            if bsize != size:
                print(f"{name:<28} retail {size:#6x}  built {bsize:#6x}  {bsize - size:+d}")
                total += bsize - size
        print(f"net {total:+d} bytes")
        return
    bsym = built_symbols(irx_config.path(cfg, "build") / f"{args.module}.elf")
    bad = []
    for name, addr, size in symbols(cfg):
        baddr, bsize = bsym.get(name, (addr, size))
        a, b = addr - base, baddr - got_base
        if want[a:a + size] != got[b:b + size]:
            bad.append((name, addr, size, baddr, bsize))
    if not bad:
        print(f"{args.module}: .text matches ({len(want)} bytes)")
        return
    print(f"{args.module}: {len(bad)} chunk(s) differ: " + " ".join(n for n, *_ in bad))
    pick = bad[0]
    for b in bad:
        if b[0] == args.function:
            pick = b
    name, addr, size, baddr, bsize = pick
    print(f"\n--- {name} retail {addr:#x} ({size} bytes) | built {baddr:#x} ({bsize} bytes)")
    w = disasm(want[addr - base:addr - base + size], addr)
    g = disasm(got[baddr - got_base:baddr - got_base + size], baddr)
    for i in range(max(len(w), len(g))):
        lw = w[i] if i < len(w) else ""
        lg = g[i] if i < len(g) else ""
        mark = "  " if lw == lg else "^ "
        print(f"{mark}{lw:<46}{lg[10:] if lg else ''}")


if __name__ == "__main__":
    main()
