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


# Only units with their own .rodata subsegment (tools/split_rodata.py) may emit
# rodata, and only jump tables, which are verified entry by entry. Strings and
# constants still come from INCLUDE_RODATA; other data is not split per unit.
DATA_SECTIONS = (".rodata", ".data", ".sdata", ".sbss", ".bss", ".lit4", ".lit8")


def relocations(obj):
    """{section: {offset: (type, symbol)}} for .text and .rodata relocations."""
    out, section = {".text": {}, ".rodata": {}}, None
    for line in run(str(BIN / "mips-ps2-decompals-objdump"), "-r", str(obj)).splitlines():
        if line.startswith("RELOCATION RECORDS FOR"):
            section = line.split("[", 1)[1].rstrip("]:")
            continue
        parts = line.split()
        if section in out and len(parts) >= 3 and re.fullmatch(r"[0-9a-f]{8}", parts[0]):
            out[section][int(parts[0], 16)] = (parts[1], parts[2])
    return out


def owns_rodata(version, unit, section="rodata"):
    yaml = (ROOT / "config" / version / f"{VERSIONS[version]['serial']}.yaml").read_text()
    return re.search(rf"\.{section}, {re.escape(unit)}\]", yaml) is not None


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("unit", type=Path)
    ap.add_argument("-v", "--verbose", action="store_true", help="print every differing instruction")
    ap.add_argument("--func", help="only report this function")
    args = ap.parse_args()
    unit = args.unit.resolve()
    version = unit.relative_to(ROOT / "src").parts[0]
    unit_name = unit.relative_to(ROOT / "src" / version).with_suffix("").as_posix()
    syms = symbols(version)
    func_starts = sorted({int(m.group(2), 16) for m in re.finditer(
        r"^\s*(\S+)\s*=\s*0x([0-9A-Fa-f]+)\s*;[^\n]*type:func", (ROOT / "config" / version / "symbol_addrs.txt").read_text(), re.M)})
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
        data, secs = sections(obj)
        emitted = {n: secs[n][1] for n in DATA_SECTIONS if secs.get(n, (0, 0))[1]}
        l4_off, l4_size = secs.get(".lit4", (0, 0))
        lit4 = data[l4_off:l4_off + l4_size]
        ri_off, ri_size = secs.get(".reginfo", (0, 0))
        gp0 = struct.unpack_from("<i", data, ri_off + 20)[0] if ri_size >= 24 else 0
        ro_off, ro_size = secs.get(".rodata", (0, 0))
        rodata = data[ro_off:ro_off + ro_size]
        all_relocs = relocations(obj)
        relocs, rodata_relocs = all_relocs[".text"], all_relocs[".rodata"]
        funcs = []
        for line in run(str(BIN / "mips-ps2-decompals-nm"), "-S", "--defined-only", str(obj)).splitlines():
            parts = line.split()
            if len(parts) == 4 and parts[2] in "Tt":
                funcs.append((int(parts[0], 16), int(parts[1], 16), parts[3]))

    ok = bad = 0
    tables = []  # (offset in our .rodata, retail address, function)
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
        # A function may not run into the next retail function: that means the
        # retail "start" there is really part of this one (tools/find_fragments.py).
        nxt = next((a for a in func_starts if a > addr), None)
        if nxt is not None and addr + size > nxt and any(
                struct.unpack_from("<I", text, off + (nxt - addr) + j)[0] for j in range(0, off + size - (off + nxt - addr), 4)):
            print(f"OVER {name} @ 0x{addr:08X}: runs past the next function at 0x{nxt:08X}")
            bad += 1
            continue
        pending_hi = {}
        rodata_hi = None
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
            if base == ".lit4":
                # A float constant: our pool offset differs from retail's (asm
                # functions are not compiled here), so compare the value itself.
                at = (((mine & 0xFFFF) ^ 0x8000) - 0x8000) + gp0  # object gp is .reginfo's
                ours = struct.unpack_from("<I", lit4, at)[0] if 0 <= at <= len(lit4) - 4 else None
                theirs = struct.unpack_from("<I", retail, va_to_off(segs, gp + (((want & 0xFFFF) ^ 0x8000) - 0x8000)))[0]
                if ours != theirs:
                    diffs.append((i, mine, want, f"float constant {ours if ours is None else hex(ours)} vs retail {theirs:#x}"))
                continue
            if base == ".rodata":
                # A switch's jump table: remember where each side keeps it.
                if rtype == "R_MIPS_HI16":
                    rodata_hi = (mine & 0xFFFF, want & 0xFFFF)
                elif rtype == "R_MIPS_LO16" and rodata_hi:
                    sext = lambda v: (v ^ 0x8000) - 0x8000
                    tables.append(((rodata_hi[0] << 16) + sext(mine & 0xFFFF),
                                   (rodata_hi[1] << 16) + sext(want & 0xFFFF), name))
                continue
            if target is None:
                continue  # other local section: masked
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
    asm_dir = ROOT / "asm" / version / "nonmatchings" / unit_name
    asm_names = set(re.findall(r'^INCLUDE_ASM\([^,]+,\s*"[^"]+",\s*(\w+)\);', unit.read_text(), re.M))

    def asm_users(addr):
        sym = next((n for n, a in syms.items() if a == addr), f"D_{addr:08X}")
        pat = re.compile(rf"%(?:hi|lo)\({re.escape(sym)}\)")
        return sorted(n for n in asm_names if (asm_dir / f"{n}.s").exists()
                      and pat.search((asm_dir / f"{n}.s").read_text()))

    # Rodata the C emits: jump tables (every entry must land on the retail case
    # label) and data items such as string literals (bytes must equal retail's).
    funcs_by_off = sorted(funcs)
    covered = set()
    starts = sorted({t[0] for t in tables} | {len(rodata)})
    for table_off, retail_addr, name in dict.fromkeys(tables):
        if not 0 <= table_off < len(rodata):
            continue
        if rodata_relocs.get(table_off, ("", ""))[1] != ".text":
            end = next(s for s in starts if s > table_off)
            item = rodata[table_off:end]
            nul = item.find(b"\0")
            if nul >= 0 and all(32 <= c < 127 or c in b"\t\n\r\x1b" for c in item[:nul]) \
                    and not any(item[nul:]):
                item = item[:nul + 1]  # a string; the rest is alignment
            else:
                item = item.rstrip(b"\0") or item[:4]
            theirs = retail[va_to_off(segs, retail_addr):][:len(item)]
            if item != theirs:
                bad += 1
                print(f"DIFF rodata of {name} (retail 0x{retail_addr:08X}): {item[:40]!r} vs {theirs[:40]!r}")
            elif users := asm_users(retail_addr):
                # The object keeps one copy; an asm function still pulls in its own.
                bad += 1
                print(f"SHARED rodata of {name} (retail 0x{retail_addr:08X}) is also used by asm "
                      f"{', '.join(users)}: keep the extern D_ symbol until they are C")
            covered.update(range(table_off, end))
            continue
        k, wrong = 0, 0
        while rodata_relocs.get(table_off + 4 * k, ("", ""))[1] == ".text":
            label = struct.unpack_from("<I", rodata, table_off + 4 * k)[0]
            owner = next(((o, n) for o, s, n in funcs_by_off if o <= label < o + s), None)
            want = struct.unpack_from("<I", retail, va_to_off(segs, retail_addr + 4 * k))[0]
            if owner is None or address(owner[1], syms) is None \
                    or address(owner[1], syms) + label - owner[0] != want:
                wrong += 1
            covered.update(range(table_off + 4 * k, table_off + 4 * k + 4))
            k += 1
        if wrong:
            bad += 1
            print(f"DIFF jump table of {name} (retail 0x{retail_addr:08X}): {wrong} of {k} entries differ")
    stray = [o for o in range(ro_size) if o not in covered and rodata[o]]
    if emitted.get(".rodata") and owns_rodata(version, unit_name) and not stray:
        del emitted[".rodata"]
    if emitted.get(".lit4") and owns_rodata(version, unit_name, "lit4"):
        del emitted[".lit4"]  # every constant was compared with retail above
    for name, size in emitted.items():
        bad += 1
        why = ("rodata no instruction refers to (unused static data?)"
               if name == ".rodata" and owns_rodata(version, unit_name)
               else "reference the existing D_ symbol instead or keep the function as INCLUDE_ASM "
               "(this data is not split per unit yet)")
        print(f"DATA {name}: 0x{size:X} bytes emitted by the unit; {why}")
    # Every retail function of the unit must still be there, as C or INCLUDE_ASM.
    full = ROOT / "asm" / version / f"{unit_name}.s"
    if full.exists() and not args.func:
        source = unit.read_text()
        for name in re.findall(r"^glabel (\w+)", full.read_text(), re.M):
            if not re.search(rf"^INCLUDE_ASM\([^\n]*\b{name}\);|^[A-Za-z_][^;\n]*\b{name}\s*\([^;]*$", source, re.M):
                bad += 1
                print(f"MISSING {name}: neither C nor INCLUDE_ASM in the unit")
    print(f"{ok} match, {bad} differ")
    sys.exit(1 if bad else 0)


if __name__ == "__main__":
    main()
