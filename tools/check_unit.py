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
    """{section: {offset: (type, symbol)}} for verified content sections."""
    out, section = {name: {} for name in (".text", ".rodata", ".sdata")}, None
    for line in run(str(BIN / "mips-ps2-decompals-objdump"), "-r", str(obj)).splitlines():
        if line.startswith("RELOCATION RECORDS FOR"):
            section = line.split("[", 1)[1].rstrip("]:")
            continue
        parts = line.split()
        if section in out and len(parts) >= 3 and re.fullmatch(r"[0-9a-f]{8}", parts[0]):
            out[section][int(parts[0], 16)] = (parts[1], parts[2])
    return out


def relocate_sdata_item(item, item_offset, relocs, funcs, syms):
    """Apply resolvable R_MIPS_32 relocations to one emitted .sdata item.

    Object files store section-relative addends for pointers into their own
    text. Resolve those through the owning function's retail address so the
    source-owned pointer value can be compared with the linked executable.
    """
    linked = bytearray(item)
    problems = []
    item_end = item_offset + len(item)
    for off, (rtype, sym) in sorted(relocs.items()):
        if off < item_offset or off + 4 > item_end:
            continue
        at = off - item_offset
        addend = struct.unpack_from("<I", linked, at)[0]
        base = sym.split("+", 1)[0]
        target = None
        if rtype != "R_MIPS_32":
            problems.append(f"unsupported {rtype} at +0x{at:X}")
            continue
        if base == ".text":
            owner = next(((obj_off, size, name) for obj_off, size, name in funcs
                          if obj_off <= addend < obj_off + size), None)
            owner_addr = address(owner[2], syms) if owner else None
            if owner_addr is not None:
                target = owner_addr + addend - owner[0]
        else:
            base_addr = address(base, syms)
            if base_addr is not None:
                target = base_addr + addend
        if target is None:
            problems.append(f"unresolved {rtype} {sym} at +0x{at:X}")
            continue
        struct.pack_into("<I", linked, at, target & 0xFFFFFFFF)
    return bytes(linked), problems


def trim_sdata_item(item, item_offset, relocs):
    """Drop alignment padding without truncating relocation-backed words."""
    reloc_words = {off - item_offset for off in relocs
                   if item_offset <= off < item_offset + len(item)}
    if reloc_words:
        return item[:max(max(reloc_words) + 4, len(item.rstrip(b"\0")))]
    nul = item.find(b"\0")
    return item[:nul + 1] if nul >= 0 and not any(item[nul:]) \
        else item.rstrip(b"\0") or item[:4]


def inline_asm_share(text):
    """{function: (inline asm instructions, all instructions)} from cc1 output
    (asm statements sit between `#APP` and `#NO_APP`)."""
    share, cur, app, asm, total = {}, None, False, 0, 0
    for line in text.splitlines():
        s = line.strip()
        if s.startswith(".ent\t") or s.startswith(".ent "):
            cur, asm, total = s.split()[1], 0, 0
        elif s.startswith(".end\t") or s.startswith(".end "):
            if cur:
                share[cur] = (asm, total)
            cur = None
        elif s in ("#APP", "#NO_APP"):
            app = s == "#APP"
        elif cur and re.match(r"[a-z]", s) and not s.endswith(":"):
            total += 1
            asm += app
    return share


def owns_rodata(version, unit, section="rodata"):
    yaml = (ROOT / "config" / version / f"{VERSIONS[version]['serial']}.yaml").read_text()
    return re.search(rf"\.{section}, {re.escape(unit)}\]", yaml) is not None


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("unit", type=Path)
    ap.add_argument("-v", "--verbose", action="store_true", help="print every differing instruction")
    ap.add_argument("--func", help="only report this function")
    ap.add_argument("--source", type=Path, help="compile this file instead of the unit (same unit layout)")
    ap.add_argument("--cflags", default="", help="extra cc1 flags (experiments; see flag_probe.py)")
    args = ap.parse_args()
    unit = args.unit.resolve()
    version = unit.relative_to(ROOT / "src").parts[0]
    unit_name = unit.relative_to(ROOT / "src" / version).with_suffix("").as_posix()
    syms = symbols(version)
    sys.path.insert(0, str(ROOT / "tools"))
    from eeas_compat import lit4_range
    lit4_lo, lit4_hi = lit4_range((ROOT / RETAIL[version]).read_bytes())
    func_starts = sorted({int(m.group(2), 16) for m in re.finditer(
        r"^\s*(\S+)\s*=\s*0x([0-9A-Fa-f]+)\s*;[^\n]*type:func", (ROOT / "config" / version / "symbol_addrs.txt").read_text(), re.M)})
    gp = int(VERSIONS[version]["gp"], 16)
    retail = (ROOT / RETAIL[version]).read_bytes()
    segs = load_segments(retail)

    with tempfile.TemporaryDirectory() as tmp:
        obj = Path(tmp) / "unit.o"
        # cc.sh compiles under the unit's own path (and flags) even for --source.
        env = dict(os.environ, DDS_VERSION=version, DDS_AS_UNIT=str(unit.relative_to(ROOT)))
        asm_text = Path(tmp) / "unit.s"
        env_s = dict(env, DDS_KEEP_S=str(asm_text))
        extra = args.cflags.split()
        r = subprocess.run([str(ROOT / "tools/cc.sh"), "-DSKIP_ASM", *extra,
                            str(args.source.resolve() if args.source else unit), "-o", str(obj)],
                           capture_output=True, text=True, env=env_s)
        asm_share = inline_asm_share(asm_text.read_text()) if asm_text.exists() else {}
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
        sd_off, sd_size = secs.get(".sdata", (0, 0))
        sdata = data[sd_off:sd_off + sd_size]
        ro_off, ro_size = secs.get(".rodata", (0, 0))
        rodata = data[ro_off:ro_off + ro_size]
        all_relocs = relocations(obj)
        relocs, rodata_relocs = all_relocs[".text"], all_relocs[".rodata"]
        sdata_relocs = all_relocs[".sdata"]
        funcs = []
        for line in run(str(BIN / "mips-ps2-decompals-nm"), "-S", "--defined-only", str(obj)).splitlines():
            parts = line.split()
            if len(parts) == 4 and parts[2] in "Tt":
                funcs.append((int(parts[0], 16), int(parts[1], 16), parts[3]))
        undefined = {l.split()[-1] for l in run(str(BIN / "mips-ps2-decompals-nm"), "-u", str(obj)).splitlines() if l.strip()}
        # The build compiles the unit with its asm included, not with SKIP_ASM.
        # ee-gcc 2.96's CSE hashes symbol-name addresses, so the preprocessed
        # text around a function can change its code: compare each C function
        # as the build compiles it, relocated fields masked.
        context = {}
        as_built = set()  # differs under SKIP_ASM, but the build's code matches retail
        ro_diff = None    # first retail address where the full unit's .rodata differs
        full = Path(tmp) / "full.o"
        rf = subprocess.run([str(ROOT / "tools/cc.sh"), *extra,
                             str(args.source.resolve() if args.source else unit), "-o", str(full)],
                            capture_output=True, text=True, env=env)
        if rf.returncode == 0:
            ftext, frel = text_section(full), relocations(full)[".text"]
            fsyms = {p[3]: (int(p[0], 16), int(p[1], 16)) for p in (
                l.split() for l in run(str(BIN / "mips-ps2-decompals-nm"), "-S", "--defined-only",
                                       str(full)).splitlines()) if len(p) == 4 and p[2] in "Tt"}

            def masked(code, base, rels):
                words = list(struct.unpack_from(f"<{len(code) // 4}I", code))
                for i in range(len(words)):
                    r = rels.get(base + 4 * i)
                    if r:
                        words[i] &= 0xFC000000 if r[0] == "R_MIPS_26" else 0xFFFF0000
                return words

            for off, size, name in funcs:
                if name in fsyms:
                    foff, fsize = fsyms[name]
                    if fsize != size or masked(ftext[foff:foff + fsize], foff, frel) != \
                            masked(text[off:off + size], off, relocs):
                        # The build links the full-unit code: if that matches retail
                        # (relocated fields masked), only the SKIP_ASM compile differs.
                        addr = address(name, syms)
                        roff = va_to_off(segs, addr) if addr is not None else None
                        if roff is not None and masked(ftext[foff:foff + fsize], foff, frel) == \
                                masked(retail[roff:roff + fsize], foff, frel):
                            as_built.add(name)
                        else:
                            context[name] = (fsize, size)
            # The build links the full unit's .rodata at the retail start of the
            # unit's .rodata subsegment: its bytes (relocated words masked) must be
            # retail's, in retail order. Catches string literals and
            # INCLUDE_RODATA items that come out in a different order.
            fdata, fsecs = sections(full)
            fro_off, fro_size = fsecs.get(".rodata", (0, 0))
            ro_start = re.search(rf"\[0x([0-9A-F]+), \.rodata, {re.escape(unit_name)}\]",
                                 (ROOT / "config" / version / f"{VERSIONS[version]['serial']}.yaml").read_text())
            if fro_size and ro_start:
                mine = bytearray(fdata[fro_off:fro_off + fro_size])
                roff = int(ro_start.group(1), 16)
                want = bytearray(retail[roff:roff + fro_size])
                for off in relocations(full)[".rodata"]:
                    mine[off:off + 4] = want[off:off + 4] = b"\0\0\0\0"
                if mine != want:
                    first = next(i for i in range(fro_size) if mine[i] != want[i])
                    ro_diff = f"0x{roff + 0xFF000 + first:08X}"
        else:
            # Usually an INCLUDE_ASM whose asm file splat has not written yet
            # (run configure.py --force-split). Unverifiable is not clean.
            context["(unit)"] = (0, 0)
            sys.stderr.write(rf.stderr[-2000:])

    ok = bad = 0
    if "(unit)" in context:
        bad += 1
        print("NOASM the unit does not compile with its asm included (missing asm file? "
              "run configure.py --force-split); build-context check not possible")
        del context["(unit)"]
    for name, (fsize, size) in context.items():
        if not args.func or name == args.func:
            bad += 1
            print(f"CONTEXT {name}: compiles differently inside the full unit ({fsize} vs {size} bytes "
                  "alone); the build uses the full-unit code, so this function does not match there")
    if ro_diff and not args.func:
        bad += 1
        print(f"RODATA the full unit's .rodata differs from retail at {ro_diff} (string literal "
              "or INCLUDE_RODATA order/content); the build links it there")
    tables = []  # (offset in our .rodata, retail address, function)
    bounds = []  # .rodata items of as-built functions: they only end a neighbour
    small = []   # the same for .sdata
    for off, size, name in sorted(funcs):
        if args.func and name != args.func:
            continue
        addr = address(name, syms)
        if addr is None:
            print(f"?    {name}: no address (add it to symbol_addrs.txt or use func_XXXXXXXX)")
            bad += 1
            continue
        if name in as_built:
            ok += 1
            print(f"OK   {name} @ 0x{addr:08X} (as built in the full unit; the SKIP_ASM compile differs)")
            # Its jump tables still sit in our .rodata and end the item before
            # them; the full-unit .rodata check above compares their bytes.
            hi = None
            for i in range(0, size, 4):
                r = relocs.get(off + i)
                if r and r[1].split("+")[0] == ".rodata":
                    w = struct.unpack_from("<I", text, off + i)[0]
                    if r[0] == "R_MIPS_HI16":
                        hi = w & 0xFFFF
                    elif r[0] == "R_MIPS_LO16" and hi is not None:
                        bounds.append((hi << 16) + ((w & 0xFFFF) ^ 0x8000) - 0x8000)
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
        rodata_hi = sdata_hi = None
        text_hi = {}  # lui destination register -> (index, mine, want)
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
            if target is not None and lit4_lo <= target < lit4_hi:
                diffs.append((i, mine, want, f"{sym} is a .lit4 pool constant: write the float literal"))
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
            if base == ".sdata":
                # Small data the C emits (a short literal or a variable it defines):
                # remember where each side keeps it, compare the bytes below.
                if rtype == "R_MIPS_HI16":
                    sdata_hi = (mine & 0xFFFF, want & 0xFFFF)
                elif rtype == "R_MIPS_LO16" and sdata_hi:
                    sext = lambda v: (v ^ 0x8000) - 0x8000
                    small.append(((sdata_hi[0] << 16) + sext(mine & 0xFFFF),
                                  (sdata_hi[1] << 16) + sext(want & 0xFFFF), name))
                elif rtype == "R_MIPS_GPREL16":
                    sext = lambda v: (v ^ 0x8000) - 0x8000
                    small.append((sext(mine & 0xFFFF) + gp0, gp + sext(want & 0xFFFF), name))
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
            if base == ".text" and rtype == "R_MIPS_26":
                # A call into this unit's own C: the field holds our offset.
                # Map it to the function there and compare with retail's target.
                tgt = (mine & 0x3FFFFFF) << 2
                owner = next(((o, n) for o, s, n in funcs if o <= tgt < o + s), None)
                where = address(owner[1], syms) if owner else None
                if where is None or (want & 0x3FFFFFF) != ((where + tgt - owner[0]) >> 2) & 0x3FFFFFF:
                    diffs.append((i, mine, want, f"{rtype} into {owner[1] if owner else '?'} "
                                                 "(retail calls a different function)"))
                continue
            if base == ".text" and rtype in ("R_MIPS_HI16", "R_MIPS_LO16"):
                # A function pointer into this unit's own C (lui/addiu pair). Pair
                # each LO16 with the lui that loaded its base register: gcc can
                # hoist one pair's lui above another's.
                if rtype == "R_MIPS_HI16":
                    text_hi[(mine >> 16) & 0x1F] = (i, mine, want)
                    continue
                hi = text_hi.get((mine >> 21) & 0x1F)
                if hi is None:
                    continue
                sext = lambda v: (v ^ 0x8000) - 0x8000
                tgt = ((hi[1] & 0xFFFF) << 16) + sext(mine & 0xFFFF)
                theirs = ((hi[2] & 0xFFFF) << 16) + sext(want & 0xFFFF)
                owner = next(((o, n) for o, s, n in funcs if o <= tgt < o + s), None)
                where = address(owner[1], syms) if owner else None
                if where is None or where + tgt - owner[0] != theirs:
                    diffs.append((i, mine, want, f"{rtype} address of {owner[1] if owner else '?'} "
                                                 "(retail uses a different function)"))
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
        pat = re.compile(rf"%(?:hi|lo|gp_rel)\({re.escape(sym)}\)")
        return sorted(n for n in asm_names if (asm_dir / f"{n}.s").exists()
                      and pat.search((asm_dir / f"{n}.s").read_text()))
    # The unit's whole .rodata as splat split it (symbols later moved into
    # function files are listed there too).
    ro_file = ROOT / "asm" / version / "data" / f"{unit_name}.rodata.s"
    ro_text = ro_file.read_text() if ro_file.exists() else ""
    retail_labels = sorted((int(a, 16), k) for k, a in re.findall(r"^dlabel (D|jtbl)_([0-9A-F]{8})\b", ro_text, re.M))
    # Labels renamed in symbol_addrs (e.g. a named table) mark item starts too.
    retail_labels += sorted((syms[n], "D") for n in re.findall(r"^dlabel (\w+)\b", ro_text, re.M)
                            if not AUTO.match(n) and n in syms)
    retail_labels.sort()
    # The unit's retail .rodata ends where the next subsegment starts: bytes
    # between the last item and that end are the unit's own padding too.
    yaml_text = (ROOT / "config" / version / f"{VERSIONS[version]['serial']}.yaml").read_text()
    ro_rows = sorted((int(o, 16), u) for o, u in re.findall(r"\[0x([0-9A-F]+), \.?rodata(?:, ([^\]\s]+))?\]", yaml_text))
    here = next((i for i, (_, u) in enumerate(ro_rows) if u == unit_name), None)
    if here is not None and here + 1 < len(ro_rows):
        retail_labels.append((ro_rows[here + 1][0] + 0xFF000, "end"))
        retail_labels.sort()

    def next_unit_aligned16():
        """The unit after this one starts 16-aligned and holds a jump table, so
        its .rodata section is 16-aligned and the linker pads up to it."""
        if here is None or here + 1 >= len(ro_rows) or not ro_rows[here + 1][1]:
            return False
        nxt_file = ROOT / "asm" / version / "data" / f"{ro_rows[here + 1][1]}.rodata.s"
        return (ro_rows[here + 1][0] + 0xFF000) % 16 == 0 and nxt_file.exists() \
            and re.search(r"^dlabel jtbl_", nxt_file.read_text(), re.M) is not None

    def retail_padding(addr, size):
        """Bytes between an item's 8-aligned end and the next retail symbol
        (jump tables are 16-aligned; so is the start of a following unit whose
        .rodata holds one, the linker pads the gap)."""
        nxt, kind = next(((a, k) for a, k in retail_labels if a > addr), (None, None))
        end = (addr + size + 7) & ~7
        if nxt is None or nxt <= end or kind == "jtbl" and nxt == (end + 15) & ~15:
            return 0
        if kind == "end" and nxt == (end + 15) & ~15 and next_unit_aligned16():
            return 0
        return nxt - end

    included = set(re.findall(r'^INCLUDE_(?:ASM|RODATA)\([^,]+,\s*"[^"]+",\s*(\w+)\);', unit.read_text(), re.M))
    eeasm_dir = ROOT / "build" / "eeasm" / "asm" / version / "nonmatchings" / unit_name

    def realigned(addr):
        """The retail item at addr is an included asm blob that eeas_compat
        16-aligns (`.align 4`), so the object supplies the padding before it."""
        pat = re.compile(rf"^\.align 4\n(?:.*\n){{0,3}}dlabel (?:D|jtbl)_{addr:08X}\b", re.M)
        return any(pat.search(p.read_text()) for p in (eeasm_dir / f"{n}.s" for n in included) if p.exists())


    # Rodata the C emits: jump tables (every entry must land on the retail case
    # label) and data items such as string literals (bytes must equal retail's).
    funcs_by_off = sorted(funcs)
    covered = set()
    starts = sorted({t[0] for t in tables} | set(bounds) | {len(rodata)})
    # Two retail copies that the C compiles to one object item: gcc merged
    # identical constants (e.g. two equal string initializers) that the
    # original kept apart, so the unit's rodata comes out short.
    by_off = {}
    for table_off, retail_addr, name in tables:
        by_off.setdefault(table_off, {}).setdefault(retail_addr, name)
    for off, uses in by_off.items():
        if len(uses) > 1:
            bad += 1
            print(f"MERGED rodata: {', '.join(f'{n} (retail 0x{a:08X})' for a, n in sorted(uses.items()))} "
                  "share one compiled item; retail has separate copies (brace-initialize, or keep the extern)")
    for table_off, retail_addr, name in dict.fromkeys(tables):
        if not 0 <= table_off < len(rodata):
            continue
        if rodata_relocs.get(table_off, ("", ""))[1] != ".text":
            end = next(s for s in starts if s > table_off)
            item = rodata[table_off:end]
            # Words the linker fills in (a table of pointers to strings or
            # data) hold only an addend here: compare the rest, and leave the
            # pointer values to the full build's checksum.
            reloc_words = {o - table_off for o in rodata_relocs if table_off <= o < end}
            nul = item.find(b"\0")
            if reloc_words:
                item = item[:max(max(reloc_words) + 4, len(item.rstrip(b"\0")))]
            elif nul >= 0 and all(32 <= c < 127 or c in b"\t\n\r\x1b" for c in item[:nul]) \
                    and not any(item[nul:]):
                item = item[:nul + 1]  # a string; the rest is alignment
            else:
                item = item.rstrip(b"\0") or item[:4]
            theirs = retail[va_to_off(segs, retail_addr):][:len(item)]
            mine_cmp, theirs_cmp = bytearray(item), bytearray(theirs)
            for w in reloc_words:
                mine_cmp[w:w + 4] = theirs_cmp[w:w + 4] = b"\0\0\0\0"
            if mine_cmp != theirs_cmp:
                bad += 1
                print(f"DIFF rodata of {name} (retail 0x{retail_addr:08X}): {item[:40]!r} vs {theirs[:40]!r}")
            elif (pad := retail_padding(retail_addr, len(item))) and \
                    rodata[table_off + len(item):end][:pad + ((len(item) + 7) & ~7) - len(item)] \
                    != retail[va_to_off(segs, retail_addr) + len(item):][:pad + ((len(item) + 7) & ~7) - len(item)]:
                # (An array whose last elements are zero loses them to rstrip above;
                # when the object emits exactly retail's following bytes, it is complete.)
                # The retail symbol runs on past the literal (unreferenced data
                # after it); a compiled literal would drop those bytes.
                bad += 1
                print(f"PAD rodata of {name} (retail 0x{retail_addr:08X}): retail has {pad} more bytes "
                      "after it that no C emits: keep the extern D_ symbol")
            elif users := asm_users(retail_addr):
                # The object keeps one copy; an asm function still pulls in its own.
                bad += 1
                print(f"SHARED rodata of {name} (retail 0x{retail_addr:08X}) is also used by asm "
                      f"{', '.join(users)}: keep the extern D_ symbol until they are C")
            covered.update(range(table_off, end))
            continue
        k, wrong = 0, 0
        # Stop at the next table: with the asm skipped, tables of C functions
        # that retail keeps apart can sit back to back here.
        while rodata_relocs.get(table_off + 4 * k, ("", ""))[1] == ".text" \
                and (k == 0 or table_off + 4 * k not in starts):
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
        elif (pad := retail_padding(retail_addr, 4 * k)) and not next(
                (k2 == "end" and a == (retail_addr + 4 * k + 15) & ~15
                 or a == (retail_addr + 4 * k + 15) & ~15 and realigned(a)
                 for a, k2 in retail_labels if a > retail_addr), False):
            # Retail has bytes after the table before the next item of this
            # unit that nothing supplies once the table is C. Padding up to the
            # unit's own end comes from the linker's section alignment, and
            # padding before a 16-aligned asm blob from its `.align 4`.
            bad += 1
            print(f"PAD jump table of {name} (retail 0x{retail_addr:08X}): retail has {pad} more bytes "
                  "after it that no C or asm supplies; keep the function as asm")
    for b in bounds:  # as-built items: compared by the full-unit .rodata check
        if 0 <= b < len(rodata):
            covered.update(range(b, next(s for s in starts if s > b)))
    stray = [o for o in range(ro_size) if o not in covered and rodata[o]]
    if emitted.get(".rodata") and owns_rodata(version, unit_name) and not stray:
        del emitted[".rodata"]
    sd_starts = sorted({o for o, _, _ in small} | {len(sdata)})
    sd_covered = set()
    for off, retail_addr, name in dict.fromkeys(small):
        if not 0 <= off < len(sdata):
            continue
        end = next(s for s in sd_starts if s > off)
        item = sdata[off:end]
        item = trim_sdata_item(item, off, sdata_relocs)
        theirs = retail[va_to_off(segs, retail_addr):][:len(item)]
        linked_item, relocation_problems = relocate_sdata_item(
            item, off, sdata_relocs, funcs, syms)
        if relocation_problems:
            bad += 1
            print(f"RELOC sdata of {name} (retail 0x{retail_addr:08X}): "
                  f"{'; '.join(relocation_problems)}")
        elif linked_item != theirs:
            bad += 1
            print(f"DIFF sdata of {name} (retail 0x{retail_addr:08X}): "
                  f"{linked_item[:24]!r} vs {theirs[:24]!r}")
        elif users := asm_users(retail_addr):
            bad += 1
            print(f"SHARED sdata of {name} (retail 0x{retail_addr:08X}) is also used by asm {', '.join(users)}: "
                  "keep the extern D_ symbol until they are C")
        sd_covered.update(range(off, end))
    if emitted.get(".sdata") and owns_rodata(version, unit_name, "sdata") \
            and not [o for o in range(len(sdata)) if o not in sd_covered and sdata[o]]:
        del emitted[".sdata"]
    if emitted.get(".lit4") and owns_rodata(version, unit_name, "lit4"):
        del emitted[".lit4"]  # every constant was compared with retail above
    # --func compares one function: the rest of the unit's data is not its business.
    for name, size in ({} if args.func else emitted).items():
        bad += 1
        why = ("rodata no instruction refers to (unused static data?)"
               if name == ".rodata" and owns_rodata(version, unit_name)
               else "reference the existing D_ symbol instead or keep the function as INCLUDE_ASM "
               "(this data is not split per unit yet)")
        print(f"DATA {name}: 0x{size:X} bytes emitted by the unit; {why}")
    # Constructs that force codegen rather than express the original source.
    source_text = re.sub(r"/\*.*?\*/|//[^\n]*", "", unit.read_text(), flags=re.S)
    for pattern, why in ((r"goto\s*\*", "computed goto (write the switch)"),
                         (r"&&\s*[A-Za-z_]\w*\s*[,}]", "label address table (write the switch)"),
                         (r"\bregister\b[^;]*\basm\s*\(", "register pinned with asm()")):
        for m in re.finditer(pattern, source_text):
            bad += 1
            print(f"TRICK line {source_text.count(chr(10), 0, m.start()) + 1}: {why}")
    # A C function whose code is mostly inline asm may be handwritten code in
    # disguise. Two kinds are real C: copies of SDK routines that Sony wrote as C
    # with an asm body (`/* libvu0: sceVu0Name */` above the definition), and VU0
    # routines in the same style, whose asm takes its values from C operands and
    # has no branches (`/* vu0 routine: ... */`). Anything else is reported;
    # it is not a difference, but progress must not count it as C.
    for name, (asm, total) in sorted(asm_share.items()):
        if (args.func and name != args.func) or asm < 8 or asm * 2 <= total:
            continue
        if re.search(rf"(?:libvu0|vu0 routine):[^\n]*\n(?:[^\n]*\n){{0,2}}[^\n]*\b{re.escape(name)}\s*\(",
                     unit.read_text()):
            continue
        print(f"ASMBODY {name}: {asm} of {total} instructions are inline asm; mark it as an SDK copy "
              "or VU0 routine, or keep it as INCLUDE_ASM (docs/idioms.md, Inline asm)")
    # C functions must keep retail order (the object's text is laid out in source order).
    # (an implicit-int K&R definition starts with the function name itself)
    order = re.findall(r'^INCLUDE_ASM\([^\n]*\b(\w+)\);|^(?:[A-Za-z_][^;\n=]*?\b)?([A-Za-z_]\w*)\s*\([^;\n]*\)\s*\{?\s*$',
                       unit.read_text(), re.M)
    placed = [(0, address(a or b, syms), a or b) for a, b in order if address(a or b, syms) is not None]
    for (_, a, n), (_, b, m) in zip(placed, placed[1:]):
        if b < a:
            bad += 1
            print(f"ORDER {m} (0x{b:08X}) comes after {n} (0x{a:08X}) in the source; move it back")
    seen = {}
    for _, _, n in placed:
        seen[n] = seen.get(n, 0) + 1
    for n in [n for n, k in seen.items() if k > 1]:
        bad += 1
        print(f"TWICE {n}: both C and INCLUDE_ASM (or two definitions) in the unit")
    # Every retail function of the unit must still be there, as C or INCLUDE_ASM.
    full = ROOT / "asm" / version / f"{unit_name}.s"
    if full.exists() and not args.func:
        source = unit.read_text()
        for name in re.findall(r"^glabel (\w+)", full.read_text(), re.M):
            if not re.search(rf"^INCLUDE_ASM\([^\n]*\b{name}\);|^(?:[A-Za-z_][^;\n]*\b)?{name}\s*\([^;]*$", source, re.M):
                bad += 1
                print(f"MISSING {name}: neither C nor INCLUDE_ASM in the unit")
    # A func_XXXXXXXX whose address symbol_addrs now names differently fails a fresh link
    # (a stale local asm tree still defines the old label, so the local build hides it).
    renamed = {f"func_{int(a, 16):08X}": n for n, a in re.findall(
        r"^\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+)\s*;[^\n]*type:func", (ROOT / "config" / version / "symbol_addrs.txt").read_text(), re.M)
        if not n.startswith("func_")}
    for old in sorted(set(re.findall(r"\bfunc_[0-9A-F]{8}\b", unit.read_text())) & renamed.keys()):
        bad += 1
        print(f"STALE {old}: symbol_addrs names it {renamed[old]}; use that name")
    # A symbol the C references but nothing defines (symbol_addrs, the
    # undefined_*_auto lists, a splat label, or C in this game's tree) links only
    # against a stale build tree. Real names count too: a mistyped or other-game
    # name compiles and matches (relocations are compared by address) yet fails the link.
    known = set(syms) | set(re.findall(r"^\s*(?:PROVIDE\s*\(\s*)?(\w+)\s*=", "".join(
        p.read_text() for p in (ROOT / "config" / version).glob("undefined_*_auto.txt")), re.M))
    wanted = undefined - known
    if wanted:
        alt = "|".join(sorted(wanted))
        wanted -= set(subprocess.run(["grep", "-rhoE", rf"^\s*(glabel|dlabel|jlabel|nonmatching) ({alt})\b",
                                      str(ROOT / "asm" / version)], capture_output=True, text=True).stdout.split()[1::2])
    if wanted:
        for path in subprocess.run(["grep", "-rlwE", "|".join(sorted(wanted)), str(ROOT / "src" / version)],
                                   capture_output=True, text=True).stdout.splitlines():
            source = Path(path).read_text()
            wanted = {n for n in wanted if not re.search(
                rf"^(?!extern\b)[A-Za-z_][^;\n]*\b{n}\s*(\([^;]*$|(\[[^\]]*\])*\s*[=;])", source, re.M)}
        for name in sorted(wanted):
            bad += 1
            print(f"UNDEF {name}: nothing defines it (symbol_addrs, splat label or C); a fresh link fails")
    print(f"{ok} match, {bad} differ")
    sys.exit(1 if bad else 0)


if __name__ == "__main__":
    main()
