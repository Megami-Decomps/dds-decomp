#!/usr/bin/env python3
"""Check a C unit's decompiled functions against retail without running ninja.

    python3 tools/check_unit.py src/dds1/sdf/sdfMemory.c [-v] [--func func_002D0390]

Compiles the unit with -DSKIP_ASM (only real C) through tools/cc.sh into a temp dir,
so any number of these can run at once. For each function the object defines, the
bytes are compared with the retail ELF at the symbol's address. Words with a
relocation are compared by resolved target when the symbol has a known address
(func_/D_XXXXXXXX, config/<v>/symbol_addrs.txt); relocations against local
sections (.rodata/.data of this unit) are masked. `ninja` remains the final proof.
Exit status 0 only if every compiled function matches.
"""
import argparse
import os
import re
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from pairing import RETAIL, ROOT, load_segments, va_to_off  # noqa: E402

BIN = ROOT / "tools/bin"
VERSIONS = __import__("json").loads((ROOT / "config/versions.json").read_text())
ROW = re.compile(r"^\s*(\S+)\s*=\s*0x([0-9A-Fa-f]+)\s*;")
AUTO = re.compile(r"^(?:func|D|jtbl)_([0-9A-F]{8})$")


def symbols(version):
    out = {}
    for line in (ROOT / "config" / version / "symbol_addrs.txt").read_text().splitlines():
        if m := ROW.match(line):
            out.setdefault(m.group(1), int(m.group(2), 16))
    return out


def address(name, syms):
    if name in syms:
        return syms[name]
    m = AUTO.match(name)
    return int(m.group(1), 16) if m else None


def run(*cmd, **kw):
    return subprocess.run(cmd, check=True, capture_output=True, text=True, **kw).stdout


def sections(obj):
    """{name: (offset, size)} for every section of an ELF32 object."""
    data = Path(obj).read_bytes()
    shoff = struct.unpack_from("<I", data, 0x20)[0]
    shnum, shstrndx = struct.unpack_from("<HH", data, 0x30)
    sh = [struct.unpack_from("<10I", data, shoff + i * 40) for i in range(shnum)]
    names = sh[shstrndx][4]
    return data, {data[names + s[0]:data.index(b"\0", names + s[0])].decode(): (s[4], s[5]) for s in sh}


def text_section(obj):
    data, secs = sections(obj)
    off, size = secs.get(".text", (0, 0))
    return data[off:off + size]


# Data still lives in the split data/rodata files, so C may not emit its own yet
# (string literals, switch jump tables, float constants, statics).
DATA_SECTIONS = (".rodata", ".data", ".sdata", ".sbss", ".bss", ".lit4", ".lit8")


def relocations(obj):
    """{text_offset: (type, symbol)} for .text relocations."""
    out, in_text = {}, False
    for line in run(str(BIN / "mips-ps2-decompals-objdump"), "-r", str(obj)).splitlines():
        if line.startswith("RELOCATION RECORDS FOR"):
            in_text = "[.text]" in line
            continue
        parts = line.split()
        if in_text and len(parts) >= 3 and re.fullmatch(r"[0-9a-f]{8}", parts[0]):
            out[int(parts[0], 16)] = (parts[1], parts[2])
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("unit", type=Path)
    ap.add_argument("-v", "--verbose", action="store_true", help="print every differing instruction")
    ap.add_argument("--func", help="only report this function")
    args = ap.parse_args()
    unit = args.unit.resolve()
    version = unit.relative_to(ROOT / "src").parts[0]
    syms = symbols(version)
    gp = int(VERSIONS[version]["gp"], 16)
    retail = (ROOT / RETAIL[version]).read_bytes()
    segs = load_segments(retail)

    with tempfile.TemporaryDirectory() as tmp:
        obj = Path(tmp) / "unit.o"
        env = dict(os.environ, DDS_VERSION=version)
        r = subprocess.run([str(ROOT / "tools/cc.sh"), "-DSKIP_ASM", str(unit), "-o", str(obj)],
                           capture_output=True, text=True, env=env)
        if r.returncode:
            sys.stderr.write(r.stderr)
            sys.exit(f"compile failed: {args.unit}")
        text = text_section(obj)
        _, secs = sections(obj)
        emitted = {n: secs[n][1] for n in DATA_SECTIONS if secs.get(n, (0, 0))[1]}
        relocs = relocations(obj)
        funcs = []
        for line in run(str(BIN / "mips-ps2-decompals-nm"), "-S", "--defined-only", str(obj)).splitlines():
            parts = line.split()
            if len(parts) == 4 and parts[2] in "Tt":
                funcs.append((int(parts[0], 16), int(parts[1], 16), parts[3]))

    ok = bad = 0
    for off, size, name in sorted(funcs):
        if args.func and name != args.func:
            continue
        addr = address(name, syms)
        if addr is None:
            print(f"?    {name}: no address (add it to symbol_addrs.txt or use func_XXXXXXXX)")
            bad += 1
            continue
        roff = va_to_off(segs, addr)
        diffs = []
        pending_hi = {}
        for i in range(0, size, 4):
            mine = struct.unpack_from("<I", text, off + i)[0]
            want = struct.unpack_from("<I", retail, roff + i)[0]
            rel = relocations_at = relocs.get(off + i)
            if rel is None:
                if mine != want:
                    diffs.append((i, mine, want, ""))
                continue
            rtype, sym = relocations_at
            base = sym.split("+")[0]
            target = address(base, syms)
            if (mine & 0xFC000000 if rtype == "R_MIPS_26" else mine & 0xFFFF0000) != \
                    (want & 0xFC000000 if rtype == "R_MIPS_26" else want & 0xFFFF0000):
                diffs.append((i, mine, want, f"{rtype} {sym}"))
                continue
            if target is None:
                continue  # local section: masked
            addend = mine & 0x3FFFFFF if rtype == "R_MIPS_26" else ((mine & 0xFFFF) ^ 0x8000) - 0x8000
            if rtype == "R_MIPS_26":
                good = (want & 0x3FFFFFF) == (((target >> 2) + addend) & 0x3FFFFFF)
            elif rtype == "R_MIPS_HI16":
                pending_hi[base] = (i, mine, want, rtype, sym, want & 0xFFFF)
                good = True
            elif rtype == "R_MIPS_LO16":
                full = target + addend
                good = (want & 0xFFFF) == full & 0xFFFF
                if base in pending_hi:
                    hi = pending_hi.pop(base)
                    if hi[5] != ((full + 0x8000) >> 16) & 0xFFFF:
                        diffs.append(hi[:3] + (f"{hi[3]} {hi[4]}",))
            elif rtype == "R_MIPS_GPREL16":
                good = (want & 0xFFFF) == (target + addend - gp) & 0xFFFF
            else:
                good = True
            if not good:
                diffs.append((i, mine, want, f"{rtype} {sym} (retail uses a different address)"))
        if diffs:
            bad += 1
            print(f"DIFF {name} @ 0x{addr:08X}: {len(diffs)} of {size // 4} words differ"
                  f" (first at +0x{diffs[0][0]:X})")
            for i, mine, want, note in (diffs if args.verbose else diffs[:3]):
                print(f"       +0x{i:03X} mine {mine:08X} retail {want:08X} {note}")
        else:
            ok += 1
            print(f"OK   {name} @ 0x{addr:08X} ({size} bytes)")
    for name, size in emitted.items():
        bad += 1
        print(f"DATA {name}: 0x{size:X} bytes emitted by the unit; reference the existing D_ symbol "
              "instead or keep the function as INCLUDE_ASM (data is not split per unit yet)")
    print(f"{ok} match, {bad} differ")
    sys.exit(1 if bad else 0)


if __name__ == "__main__":
    main()
