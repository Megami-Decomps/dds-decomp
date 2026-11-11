#!/usr/bin/env python3
"""Read and write the IRX modules in USERIRX/ (info, unpack, pack).

An IRX is an ELF of type 0xFF80 that keeps its relocations and drops its
symbols: the loader places it anywhere and walks .rel.* adding the load base,
so every relocation carries symbol index 0. See docs/irx-modules.md.
"""
from __future__ import annotations

import argparse
import json
import struct
from dataclasses import dataclass, field
from pathlib import Path

ET_IRX = 0xFF80
PT_IOPMOD = 0x70000080
SHT_IOPMOD = 0x70000080
SHT_PROGBITS, SHT_SYMTAB, SHT_STRTAB, SHT_RELA, SHT_NOBITS, SHT_REL = 1, 2, 3, 4, 8, 9
SHF_WRITE, SHF_ALLOC, SHF_EXEC = 1, 2, 4
SHF_MIPS_GPREL = 0x10000000
EM_MIPS = 8
EF_MIPS_NOREORDER = 1

# The load segment starts right after the ELF header, two program headers and
# the .iopmod section, rounded up to the .text alignment. .iopmod carries the
# module name, so where that lands depends on how long the name is: the three
# SDF modules all come out at 0xA0, Sony's sound modules have longer names and
# come out at 0xB0.
IOPMOD_OFFSET = 0x74
LOAD_ALIGN = 0x10


def load_offset(iopmod_size: int) -> int:
    end = IOPMOD_OFFSET + iopmod_size
    return (end + LOAD_ALIGN - 1) & ~(LOAD_ALIGN - 1)
R_MIPS_32, R_MIPS_26, R_MIPS_HI16, R_MIPS_LO16 = 2, 4, 5, 6
RELOC_NAMES = {R_MIPS_32: "R_MIPS_32", R_MIPS_26: "R_MIPS_26",
               R_MIPS_HI16: "R_MIPS_HI16", R_MIPS_LO16: "R_MIPS_LO16"}


@dataclass
class Section:
    name: str
    type: int
    flags: int
    addr: int
    offset: int
    size: int
    link: int
    info: int
    align: int
    entsize: int
    data: bytes = b""
    relocs: list[tuple[int, int]] = field(default_factory=list)  # (offset, type)


@dataclass
class Module:
    entry: int
    flags: int
    sections: list[Section]
    moduleinfo: int
    gp: int
    text_size: int
    data_size: int
    bss_size: int
    version: int
    name: str

    def section(self, name: str) -> Section | None:
        return next((s for s in self.sections if s.name == name), None)


def _strz(data: bytes, off: int) -> str:
    return data[off:data.index(b"\0", off)].decode("ascii")


def read(path: Path) -> Module:
    d = path.read_bytes()
    if d[:4] != b"\x7fELF":
        raise SystemExit(f"{path}: not an ELF")
    etype, machine = struct.unpack_from("<HH", d, 0x10)
    entry, _phoff, shoff = struct.unpack_from("<III", d, 0x18)
    eflags, = struct.unpack_from("<I", d, 0x24)
    shentsize, shnum, shstrndx = struct.unpack_from("<HHH", d, 0x2E)
    if etype != ET_IRX or machine != EM_MIPS:
        raise SystemExit(f"{path}: not an IRX (type {etype:#x}, machine {machine})")

    raw = [struct.unpack_from("<10I", d, shoff + i * shentsize) for i in range(shnum)]
    stroff = raw[shstrndx][4]
    secs = []
    for nameoff, ty, fl, addr, off, size, link, info, align, entsize in raw:
        s = Section(_strz(d, stroff + nameoff), ty, fl, addr, off, size,
                    link, info, align, entsize)
        if ty not in (SHT_NOBITS,):
            s.data = d[off:off + size]
        secs.append(s)

    # Attach relocations to the section they apply to, dropping the .rel.* shells.
    out = []
    for s in secs:
        if s.type == SHT_REL:
            target = secs[s.info]
            for k in range(s.size // 8):
                roff, rinfo = struct.unpack_from("<II", s.data, k * 8)
                if rinfo >> 8:
                    raise SystemExit(f"{path}: {s.name}: symbol index {rinfo >> 8}, expected 0")
                # r_offset is the module address; hold it section-relative.
                target.relocs.append((roff - target.addr, rinfo & 0xFF))
        else:
            out.append(s)

    im = next(s for s in secs if s.name == ".iopmod")
    moduleinfo, mentry, gp, tsz, dsz, bsz = struct.unpack_from("<6I", im.data, 0)
    version, = struct.unpack_from("<H", im.data, 0x18)
    return Module(entry, eflags, out, moduleinfo, gp, tsz, dsz, bsz,
                  version, _strz(im.data, 0x1A))


def _pair_hi_lo_order(relocs: list[tuple[int, int]]) -> list[tuple[int, int]]:
    """Put each LO16 straight after its HI16, the way the retail tables are.

    The original assembler emitted a %hi/%lo pair's relocations together, at
    the point it resolved the pair, and only then whatever relocation belonged
    to an instruction physically between them. That happens when the %lo lands
    in a jump's delay slot: the compiler writes lui/addiu/jal, the assembler
    moves the addiu down into the slot, and the relocations keep the order
    before the move. Our assembly is already in final order under .set
    noreorder, so gas emits strictly by offset instead.

    The pairing is by order, nth HI16 with nth LO16, which is the same
    assumption tools/irx_split.py's pair_hi_lo reads them back under. Where a
    pair is already adjacent this does nothing, so a function that has become
    C and had its delay slot filled by gas passes through untouched.
    """
    out = list(relocs)
    i = 0
    while i < len(out):
        if out[i][1] != R_MIPS_HI16:
            i += 1
            continue
        lo = next((k for k in range(i + 1, len(out)) if out[k][1] == R_MIPS_LO16), None)
        if lo is None:
            raise SystemExit(f"HI16 at {out[i][0]:#x} with no LO16 after it")
        out.insert(i + 1, out.pop(lo))
        i += 2
    return out


def iopmod_bytes(m: Module) -> bytes:
    body = struct.pack("<6IH", m.moduleinfo, m.entry, m.gp,
                       m.text_size, m.data_size, m.bss_size, m.version)
    return body + m.name.encode("ascii") + b"\0"


def write(m: Module, path: Path) -> bytes:
    """Lay the module out exactly the way the retail files are laid out.

    Allocatable sections follow the load segment with a fixed
    vaddr-to-offset delta, then .shstrtab, the section header table, the .rel
    sections in section order, and finally the stub symtab and strtab.
    """
    iopmod = iopmod_bytes(m)
    base = load_offset(len(iopmod))
    alloc = [s for s in m.sections if s.flags & SHF_ALLOC]
    end = base
    for s in alloc:
        if s.type != SHT_NOBITS:
            s.offset = s.addr + base
            end = max(end, s.offset + s.size)
    # A NOBITS section occupies no file space, so it points at the end of the
    # data rather than at its own address plus the load delta.
    for s in alloc:
        if s.type == SHT_NOBITS:
            s.offset = end

    # Section header order: .iopmod, then each allocatable section with its
    # .rel immediately after, then .shstrtab, .symtab, .strtab. The name table
    # is built in that same order.
    order: list[Section] = []
    im = Section(".iopmod", SHT_IOPMOD, 0, 0, IOPMOD_OFFSET, len(iopmod), 0, 0, 4, 0, iopmod)
    order.append(im)
    rel_for: dict[str, Section] = {}
    for s in alloc:
        order.append(s)
        if s.relocs:
            r = Section(".rel" + s.name, SHT_REL, 0, 0, 0, len(s.relocs) * 8, 0, 0, 4, 8)
            r.data = b"".join(struct.pack("<II", s.addr + off, ty)
                              for off, ty in s.relocs)
            rel_for[s.name] = r
            order.append(r)
    shstrtab = Section(".shstrtab", SHT_STRTAB, 0, 0, end, 0, 0, 0, 1, 0, b"")
    symtab = Section(".symtab", SHT_SYMTAB, 0, 0, 0, 16, 0, 1, 4, 16, b"\0" * 16)
    strtab = Section(".strtab", SHT_STRTAB, 0, 0, 0, 1, 0, 0, 1, 0, b"\0")
    order += [shstrtab, symtab, strtab]

    shstr = b"\0"
    nameoff: dict[str, int] = {}
    for s in order:
        nameoff[s.name] = len(shstr)
        shstr += s.name.encode() + b"\0"
    shstrtab.data = shstr
    shstrtab.size = len(shstr)

    index = {id(s): i for i, s in enumerate([Section("", 0, 0, 0, 0, 0, 0, 0, 0, 0)] + order)}
    symtab_idx = index[id(symtab)]
    for s in alloc:
        if s.name in rel_for:
            r = rel_for[s.name]
            r.link = symtab_idx
            r.info = index[id(s)]
    symtab.link = index[id(strtab)]

    shoff = (shstrtab.offset + shstrtab.size + 3) & ~3
    cursor = shoff + (len(order) + 1) * 40
    for s in order:
        if s.type in (SHT_REL, SHT_SYMTAB, SHT_STRTAB) and s is not shstrtab:
            s.offset = cursor
            cursor += s.size

    out = bytearray(cursor)
    out[0:16] = b"\x7fELF\x01\x01\x01" + b"\0" * 9
    struct.pack_into("<HHIIIIIHHHHHH", out, 0x10, ET_IRX, EM_MIPS, 1, m.entry,
                     52, shoff, m.flags, 52, 32, 2, 40, len(order) + 1,
                     index[id(shstrtab)])
    struct.pack_into("<8I", out, 52, PT_IOPMOD, IOPMOD_OFFSET, 0, 0, len(iopmod), 0, 4, 4)
    memsz = max(s.addr + s.size for s in alloc)
    filesz = end - base
    struct.pack_into("<8I", out, 52 + 32, 1, base, 0, 0, filesz, memsz, 7, 0x10)

    for s in order:
        if s.type != SHT_NOBITS and s.data:
            out[s.offset:s.offset + len(s.data)] = s.data
    for i, s in enumerate([Section("", 0, 0, 0, 0, 0, 0, 0, 0, 0)] + order):
        struct.pack_into("<10I", out, shoff + i * 40,
                         nameoff.get(s.name, 0), s.type, s.flags, s.addr,
                         s.offset, s.size, s.link, s.info, s.align, s.entsize)
    path.write_bytes(bytes(out))
    return bytes(out)


def cmd_info(args) -> None:
    m = read(Path(args.irx))
    print(f"{args.irx}: {m.name!r} version {m.version:#06x}")
    print(f"  entry {m.entry:#x}  gp {m.gp:#x}  moduleinfo {m.moduleinfo:#x}")
    print(f"  text {m.text_size:#x}  data {m.data_size:#x}  bss {m.bss_size:#x}")
    for s in m.sections:
        extra = ""
        if s.relocs:
            kinds = {}
            for _, ty in s.relocs:
                kinds[RELOC_NAMES.get(ty, ty)] = kinds.get(RELOC_NAMES.get(ty, ty), 0) + 1
            extra = "  relocs " + " ".join(f"{k}={v}" for k, v in sorted(kinds.items()))
        print(f"  {s.name:<12} addr {s.addr:#08x} size {s.size:#08x}{extra}")


def cmd_unpack(args) -> None:
    m = read(Path(args.irx))
    out = Path(args.out)
    out.mkdir(parents=True, exist_ok=True)
    manifest = {
        "name": m.name, "version": m.version, "entry": m.entry, "gp": m.gp,
        "moduleinfo": m.moduleinfo, "flags": m.flags,
        "text_size": m.text_size, "data_size": m.data_size, "bss_size": m.bss_size,
        "sections": [],
    }
    for s in m.sections:
        if s.type != SHT_NOBITS and s.size:
            (out / f"{s.name.lstrip('.')}.bin").write_bytes(s.data)
        manifest["sections"].append({
            "name": s.name, "addr": s.addr, "size": s.size, "align": s.align,
            "flags": s.flags, "nobits": s.type == SHT_NOBITS,
            "relocs": [[o, t] for o, t in s.relocs],
        })
    (out / "module.json").write_text(json.dumps(manifest, indent=1) + "\n")
    print(f"unpacked {args.irx} -> {out}")


def _elf_sections(d: bytes) -> tuple[list[dict], bytes]:
    shoff, = struct.unpack_from("<I", d, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from("<HHH", d, 0x2E)
    raw = [struct.unpack_from("<10I", d, shoff + i * shentsize) for i in range(shnum)]
    stroff = raw[shstrndx][4]
    secs = []
    for nameoff, ty, fl, addr, off, size, link, info, align, entsize in raw:
        secs.append({"name": _strz(d, stroff + nameoff), "type": ty, "flags": fl,
                     "addr": addr, "offset": off, "size": size, "link": link,
                     "info": info, "align": align, "entsize": entsize})
    return secs, d


def pack_from_elf(elf: Path, name: str, version: int) -> Module:
    """Build a Module from a linked ELF produced with --emit-relocs."""
    d = elf.read_bytes()
    secs, _ = _elf_sections(d)
    entry, = struct.unpack_from("<I", d, 0x18)

    keep = [".text", ".rodata", ".data", ".sdata", ".sbss", ".bss"]
    alloc = []
    for want in keep:
        # A retail module carries a .bss header even with nothing in it, which
        # MODMSIN is: every other section is dropped when it comes out empty.
        s = next((x for x in secs
                  if x["name"] == want and (x["size"] or want == ".bss")), None)
        if s is None:
            continue
        ty = SHT_NOBITS if s["type"] == SHT_NOBITS else SHT_PROGBITS
        flags = SHF_ALLOC
        if want in (".data", ".sdata", ".sbss", ".bss"):
            flags |= SHF_WRITE
        if want == ".text":
            flags |= SHF_EXEC
        if want == ".sbss":
            flags |= SHF_MIPS_GPREL
        body = b"" if ty == SHT_NOBITS else d[s["offset"]:s["offset"] + s["size"]]
        alloc.append(Section(want, ty, flags, s["addr"], 0, s["size"], 0, 0,
                             s["align"], 0, body))

    by_name = {s.name: s for s in alloc}
    # --emit-relocs leaves .rel.<section>; every entry becomes a base-relative
    # fixup with symbol index 0, which is what the IOP loader applies.
    for s in secs:
        if s["type"] != SHT_REL:
            continue
        target = secs[s["info"]]["name"]
        if target not in by_name:
            continue
        body = d[s["offset"]:s["offset"] + s["size"]]
        for k in range(s["size"] // 8):
            roff, rinfo = struct.unpack_from("<II", body, k * 8)
            ty = rinfo & 0xFF
            if ty not in RELOC_NAMES:
                raise SystemExit(f"{elf}: {s['name']}: unsupported relocation type {ty}")
            by_name[target].relocs.append((roff - by_name[target].addr, ty))
    for s in alloc:
        s.relocs.sort(key=lambda r: r[0])
        s.relocs = _pair_hi_lo_order(s.relocs)

    syms = _symbols(d, secs)
    gp = syms.get("_gp")
    if gp is None:
        raise SystemExit(f"{elf}: the linker script must define _gp")
    moduleinfo = syms.get("_irx_id")
    if moduleinfo is None:
        data = by_name.get(".data")
        if data is None:
            raise SystemExit(f"{elf}: no _irx_id and no .data to fall back to")
        moduleinfo = data.addr

    text = by_name[".text"]
    text_size = text.size
    data_size = sum(s.size for s in alloc
                    if s.name in (".rodata", ".data", ".sdata"))
    bss_start = min((s.addr for s in alloc if s.name in (".sbss", ".bss")),
                    default=None)
    if bss_start is None:
        bss_size = 0
    else:
        bss_end = max(s.addr + s.size for s in alloc if s.name in (".sbss", ".bss"))
        bss_size = bss_end - bss_start
    return Module(entry, EF_MIPS_NOREORDER, alloc, moduleinfo, gp,
                  text_size, data_size, bss_size, version, name)


def _symbols(d: bytes, secs: list[dict]) -> dict[str, int]:
    out = {}
    for s in secs:
        if s["type"] != SHT_SYMTAB:
            continue
        stroff = secs[s["link"]]["offset"]
        for k in range(s["size"] // 16):
            nameoff, value, _size, _info, _other, _shndx = struct.unpack_from(
                "<IIIBBH", d, s["offset"] + k * 16)
            if nameoff:
                out[_strz(d, stroff + nameoff)] = value
    return out


def cmd_pack(args) -> None:
    m = pack_from_elf(Path(args.elf), args.name, int(args.version, 0))
    data = write(m, Path(args.out))
    print(f"packed {args.out} ({len(data)} bytes)")


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("info"); p.add_argument("irx"); p.set_defaults(fn=cmd_info)
    p = sub.add_parser("unpack"); p.add_argument("irx"); p.add_argument("-o", "--out", required=True)
    p.set_defaults(fn=cmd_unpack)
    p = sub.add_parser("pack")
    p.add_argument("elf"); p.add_argument("-o", "--out", required=True)
    p.add_argument("--name", required=True); p.add_argument("--version", default="0")
    p.set_defaults(fn=cmd_pack)
    args = ap.parse_args()
    args.fn(args)


if __name__ == "__main__":
    main()
