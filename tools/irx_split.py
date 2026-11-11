#!/usr/bin/env python3
"""Disassemble an IRX into reassemblable asm under asm/irx/<module>/.

Relocated operands become %hi/%lo and symbol references, so the assembler and
linker regenerate the module's relocation tables. See docs/irx-modules.md.
"""
from __future__ import annotations

import argparse
import re
import struct
import sys
from pathlib import Path

import rabbitizer

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
import irx  # noqa: E402
import irx_config  # noqa: E402

R_32, R_26, R_HI16, R_LO16 = 2, 4, 5, 6


SYMBOL_LINE = re.compile(
    r"^\s*(?P<name>[A-Za-z_][\w]*)\s*=\s*(?P<addr>0x[0-9A-Fa-f]+)\s*;(?P<attrs>.*)$")


def load_syms(cfg: dict) -> list[tuple[int, str, int, str]]:
    """The module's symbol_addrs file, in splat's format."""
    path = irx_config.path(cfg, "symbols")
    rows = []
    for line in path.read_text().splitlines():
        m = SYMBOL_LINE.match(line.split("//", 1)[0])
        if not m:
            continue
        attrs = dict(a.split(":", 1) for a in line.partition("//")[2].split() if ":" in a)
        size = int(attrs.get("size", "0"), 0)
        kind = "F" if attrs.get("type") == "func" else "D"
        rows.append((int(m["addr"], 16), m["name"], size, kind))
    rows.sort()
    return rows


class SymbolMap:
    def __init__(self, rows):
        self.by_addr = {a: n for a, n, _s, _k in rows}
        self.sorted = sorted(self.by_addr)

    def add(self, addr: int, name: str) -> None:
        if addr not in self.by_addr:
            self.by_addr[addr] = name
            self.sorted = sorted(self.by_addr)

    def ref(self, addr: int) -> str:
        """A symbolic reference to addr, as `sym` or `sym + 0xN`."""
        if addr in self.by_addr:
            return self.by_addr[addr]
        import bisect
        i = bisect.bisect_right(self.sorted, addr) - 1
        if i >= 0:
            base = self.sorted[i]
            return f"{self.by_addr[base]} + {addr - base:#x}"
        return f"{addr:#x}"


def reloc_map(section) -> dict[int, int]:
    return {off: ty for off, ty in section.relocs}


def pair_hi_lo(section, base: int) -> dict[int, int]:
    """Resolve each HI16/LO16 pair to the address it builds.

    The relocation table lists them one for one and in order, which is how the
    linker emitted them; the stored immediates are the addend for a base-0 link.
    """
    out = {}
    pending = None
    for off, ty in section.relocs:
        if ty not in (R_HI16, R_LO16):
            continue
        word, = struct.unpack_from("<I", section.data, off)
        if ty == R_HI16:
            pending = (off, word & 0xFFFF)
        else:
            lo = word & 0xFFFF
            lo = lo - 0x10000 if lo & 0x8000 else lo
            if pending is None:
                raise SystemExit(f"LO16 at {base + off:#x} with no HI16 before it")
            hi_off, hi = pending
            addr = (hi << 16) + lo
            out[hi_off] = addr
            out[off] = addr
            pending = None
    if pending is not None:
        raise SystemExit(f"HI16 at {base + pending[0]:#x} with no LO16 after it")
    return out


# gas 2.8, the assembler these modules were built with, predates the o32
# register aliases, so everything is emitted numerically.
REG = [f"${i}" for i in range(32)]
rabbitizer.config.regNames_namedRegisters = False
rabbitizer.config.regNames_gprAbiNames = rabbitizer.Abi.NUMERIC


def text_labels(text, syms: SymbolMap, relocs, hilo) -> set[int]:
    """Every branch target, so each one gets a label."""
    labels = set()
    for off in range(0, len(text.data), 4):
        word, = struct.unpack_from("<I", text.data, off)
        instr = rabbitizer.Instruction(word, text.addr + off)
        if instr.isBranch() and not instr.isJumpWithAddress():
            labels.add(text.addr + off + instr.getBranchOffsetGeneric())
    return labels


def jump_targets(module: irx.Module) -> set[int]:
    """Addresses in .text that a word elsewhere points at: the jump tables."""
    text = module.section(".text")
    lo, hi = text.addr, text.addr + text.size
    out = set()
    for sec in module.sections:
        if sec.name == ".text" or sec.type == irx.SHT_NOBITS:
            continue
        for off, ty in sec.relocs:
            if ty != R_32:
                continue
            word, = struct.unpack_from("<I", sec.data, off)
            if lo <= word < hi:
                out.add(word)
    return out


def disasm_text(module: irx.Module, syms: SymbolMap, funcs,
                exported: set[int]) -> list[tuple[str, str]]:
    """Walk .text end to end: functions where they are, tables in the gaps.

    The IRX import and export tables live in .text between the stub functions,
    so anything not covered by a function is emitted as data to keep the
    section byte-complete.
    """
    text = module.section(".text")
    relocs = reloc_map(text)
    hilo = pair_hi_lo(text, text.addr)
    labels = text_labels(text, syms, relocs, hilo) | exported
    for addr in labels:
        if addr not in syms.by_addr:
            # A jump table entry has to resolve across objects, so its label
            # cannot be one of gas's local .L names.
            syms.add(addr, f"L_{addr:08X}" if addr in exported else f".L_{addr:08X}")

    spans = sorted((a, a + sz, n) for a, n, sz, k in funcs)
    chunks: list[tuple[str, str]] = []
    cursor = text.addr
    end = text.addr + text.size
    for start, stop, name in spans:
        if start > cursor:
            chunks.append(emit_gap(text, syms, relocs, cursor, start))
        lines = [f"    .globl {name}", f"{name}:"]
        for off in range(start, stop, 4):
            fo = off - text.addr
            word, = struct.unpack_from("<I", text.data, fo)
            if off in labels and off != start:
                if off in exported:
                    lines.append(f"    .globl {syms.by_addr[off]}")
                lines.append(f"{syms.by_addr[off]}:")
            lines.append("    " + format_insn(word, off, fo, relocs, hilo, syms))
        chunks.append((name, "\n".join(lines) + "\n"))
        cursor = stop
    if cursor < end:
        chunks.append(emit_gap(text, syms, relocs, cursor, end))
    return chunks


def emit_gap(text, syms: SymbolMap, relocs, start: int, stop: int) -> tuple[str, str]:
    """A run of .text that no function covers: an import or export table."""
    name = syms.by_addr.get(start) or f"D_{start:08X}"
    syms.add(start, name)
    lines = [f"    .globl {name}", f"{name}:"]
    off = start
    pending: list[int] = []

    def flush():
        for i in range(0, len(pending), 16):
            lines.append("    .byte " + ", ".join(f"0x{b:02X}" for b in pending[i:i + 16]))
        pending.clear()

    while off < stop:
        fo = off - text.addr
        if relocs.get(fo) == R_32:
            flush()
            word, = struct.unpack_from("<I", text.data, fo)
            lines.append(f"    .word {syms.ref(word)}")
            off += 4
            continue
        if off != start and off in syms.by_addr:
            flush()
            lines.append(f"    .globl {syms.by_addr[off]}")
            lines.append(f"{syms.by_addr[off]}:")
        pending.append(text.data[fo])
        off += 1
    flush()
    return (name, "\n".join(lines) + "\n")


def format_insn(word: int, vram: int, fo: int, relocs, hilo, syms: SymbolMap) -> str:
    ty = relocs.get(fo)
    op = word >> 26
    if ty == R_26:
        target = (word & 0x03FFFFFF) << 2
        mnem = "jal" if op == 3 else "j"
        return f"{mnem} {syms.ref(target)}"
    if ty in (R_HI16, R_LO16):
        addr = hilo[fo]
        part = "%hi" if ty == R_HI16 else "%lo"
        rs, rt = (word >> 21) & 31, (word >> 16) & 31
        if op == 0x0F:  # lui
            return f"lui {REG[rt]}, {part}({syms.ref(addr)})"
        mnem = rabbitizer.Instruction(word, vram).getOpcodeName()
        if op in (0x08, 0x09, 0x0C, 0x0D, 0x0E):  # addi/addiu/andi/ori/xori
            return f"{mnem} {REG[rt]}, {REG[rs]}, {part}({syms.ref(addr)})"
        # load or store: the displacement carries the relocation
        return f"{mnem} {REG[rt]}, {part}({syms.ref(addr)})({REG[rs]})"
    if ty is not None:
        raise SystemExit(f"unexpected relocation {ty} at {vram:#x}")

    instr = rabbitizer.Instruction(word, vram)
    if instr.isBranch() and not instr.isJumpWithAddress():
        target = vram + instr.getBranchOffsetGeneric()
        return f"{branch_text(word)}{syms.by_addr[target]}"
    return instr.disassemble(extraLJust=-1).replace("\t", " ")


# Always the canonical operand form: rabbitizer prints beq/bne against $zero as
# beqz/bnez, which takes a different number of operands.
BRANCH_RS_RT = {4: "beq", 5: "bne", 20: "beql", 21: "bnel"}
BRANCH_RS = {6: "blez", 7: "bgtz", 22: "blezl", 23: "bgtzl"}
BRANCH_REGIMM = {0: "bltz", 1: "bgez", 16: "bltzal", 17: "bgezal",
                 2: "bltzl", 3: "bgezl"}


def branch_text(word: int) -> str:
    op, rs, rt = word >> 26, (word >> 21) & 31, (word >> 16) & 31
    if op in BRANCH_RS_RT:
        return f"{BRANCH_RS_RT[op]} {REG[rs]}, {REG[rt]}, "
    if op in BRANCH_RS:
        return f"{BRANCH_RS[op]} {REG[rs]}, "
    if op == 1 and rt in BRANCH_REGIMM:
        return f"{BRANCH_REGIMM[rt]} {REG[rs]}, "
    raise SystemExit(f"unhandled branch {word:#010x}")


def dump_data(section, syms: SymbolMap, name: str) -> str:
    """Relocated words become symbol references; everything else stays bytes."""
    relocs = reloc_map(section)
    labels = {a - section.addr for a in syms.sorted
              if section.addr <= a < section.addr + section.size}
    lines = [f"    .section {name}", f"    .align {section.align.bit_length() - 1}"]
    off = 0
    pending: list[int] = []

    def flush():
        for i in range(0, len(pending), 16):
            lines.append("    .byte " + ", ".join(f"0x{b:02X}" for b in pending[i:i + 16]))
        pending.clear()

    while off < section.size:
        if off in labels and off:
            flush()
        if off in labels:
            lines.append(f"    .globl {syms.by_addr[section.addr + off]}")
            lines.append(f"{syms.by_addr[section.addr + off]}:")
        if relocs.get(off) == R_32:
            flush()
            word, = struct.unpack_from("<I", section.data, off)
            lines.append(f"    .word {syms.ref(word)}")
            off += 4
            continue
        pending.append(section.data[off])
        off += 1
    flush()
    return "\n".join(lines) + "\n"


def dump_bss(module: irx.Module, syms: SymbolMap) -> str:
    """Reserve the bss symbols.

    gas rounds a NOBITS section's size up to the section's own alignment, and
    .bss rarely ends on a 16-byte boundary, so the reservation goes into a
    `.reserve` input section the linker script folds into .bss. The empty .bss
    beside it is what gives the output section its alignment. The reservation
    itself is byte aligned so that nothing rounds its size either: MODHSYN's
    .bss is 0x3842 bytes, which four-byte alignment would round to 0x3844.
    Symbol placement inside it comes from the .space runs, not the alignment.
    """
    lines = []
    for sec in module.sections:
        if sec.type != irx.SHT_NOBITS:
            continue
        align = sec.align.bit_length() - 1
        lines.append(f'    .section {sec.name}, "aw", @nobits')
        lines.append(f"    .align {align}")
        lines.append(f'    .section {sec.name}.reserve, "aw", @nobits')
        lines.append("    .align 0")
        cursor = sec.addr
        for addr in [a for a in syms.sorted if sec.addr <= a < sec.addr + sec.size]:
            if addr > cursor:
                lines.append(f"    .space {addr - cursor:#x}")
                cursor = addr
            lines.append(f"    .globl {syms.by_addr[addr]}")
            lines.append(f"{syms.by_addr[addr]}:")
        if cursor < sec.addr + sec.size:
            lines.append(f"    .space {sec.addr + sec.size - cursor:#x}")
    return "\n".join(lines) + "\n"


def split(module: str) -> None:
    cfg = irx_config.load(module)
    mod = irx.read(irx_config.path(cfg, "target"))
    rows = load_syms(cfg)
    syms = SymbolMap(rows)
    funcs = [r for r in rows if r[3] == "F"]
    out = irx_config.path(cfg, "asm")
    # Clear the chunk directory: a rename in the IDB changes a chunk's file
    # name, and the old file would otherwise sit there looking current.
    if (out / "text").is_dir():
        for stale in (out / "text").glob("*.s"):
            stale.unlink()
    (out / "text").mkdir(parents=True, exist_ok=True)

    chunks = disasm_text(mod, syms, funcs, jump_targets(mod))
    for chunk_name, body in chunks:
        (out / "text" / f"{chunk_name}.s").write_text(body)
    # The chunk order only bootstraps the C file; after that the INCLUDE_ASM
    # lines in the C file are the order, so this is a build artefact.
    order = irx_config.path(cfg, "build") / "text_order.txt"
    order.parent.mkdir(parents=True, exist_ok=True)
    order.write_text("# Generated by tools/irx_split.py: .text chunks in link order.\n"
                     + "".join(f"{c}\n" for c, _ in chunks))

    for sec_name, fname in ((".rodata", "rodata.s"), (".data", "data.s")):
        sec = mod.section(sec_name)
        if sec:
            (out / fname).write_text(dump_data(sec, syms, sec_name))
    (out / "bss.s").write_text(dump_bss(mod, syms))
    nfunc = sum(1 for c, _ in chunks if c in {f[1] for f in funcs})
    print(f"{module}: {len(chunks)} chunks ({nfunc} functions)")


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("modules", nargs="*")
    args = ap.parse_args()
    for name in args.modules or irx_config.modules():
        split(name)


if __name__ == "__main__":
    main()
