#!/usr/bin/env python3
"""Build and validate the separate DDS development ELF layout.

The retail targets remain untouched.  This tool transforms a generated Splat
linker script for a declared set of section moves, then finalizes the linked
binary by installing one appended PT_LOAD and separating the development heap
start from the retail BSS-clear end.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import struct
import sys
from dataclasses import dataclass
from pathlib import Path
from typing import Any


ELF_HEADER = struct.Struct("<16sHHIIIIIHHHHHH")
PROGRAM_HEADER = struct.Struct("<IIIIIIII")
SECTION_HEADER = struct.Struct("<IIIIIIIIII")
REL_ENTRY = struct.Struct("<II")
SYMBOL_ENTRY = struct.Struct("<IIIBBH")
ELF_MAGIC = b"\x7fELF"
ELFCLASS32 = 1
ELFDATA2LSB = 1
EM_MIPS = 8
PT_LOAD = 1
SHT_SYMTAB = 2
SHT_REL = 9
PF_X = 1
PF_W = 2
PF_R = 4


class DevElfError(ValueError):
    """The requested development image is structurally unsafe."""


@dataclass(frozen=True)
class ElfHeader:
    ident: bytes
    elf_type: int
    machine: int
    version: int
    entry: int
    phoff: int
    shoff: int
    flags: int
    ehsize: int
    phentsize: int
    phnum: int
    shentsize: int
    shnum: int
    shstrndx: int


@dataclass(frozen=True)
class ProgramHeader:
    kind: int
    offset: int
    vaddr: int
    paddr: int
    file_size: int
    memory_size: int
    flags: int
    alignment: int


@dataclass(frozen=True)
class LinkedRelocation:
    offset: int
    kind: int
    symbol_table_index: int
    symbol_index: int
    symbol_value: int


def _number(value: Any, context: str) -> int:
    if isinstance(value, int):
        result = value
    elif isinstance(value, str):
        try:
            result = int(value, 0)
        except ValueError as exc:
            raise DevElfError(f"{context} is not an integer: {value!r}") from exc
    else:
        raise DevElfError(f"{context} is not an integer: {value!r}")
    if not 0 <= result <= 0xFFFFFFFF:
        raise DevElfError(f"{context} is outside the 32-bit address space")
    return result


def _align_up(value: int, alignment: int) -> int:
    if alignment <= 0 or alignment & (alignment - 1):
        raise DevElfError(f"alignment 0x{alignment:X} is not a positive power of two")
    return (value + alignment - 1) & -alignment


def _range_end(start: int, size: int, context: str) -> int:
    end = start + size
    if end > 0x100000000:
        raise DevElfError(f"{context} exceeds the 32-bit address space")
    return end


def parse_elf(image: bytes | bytearray) -> tuple[ElfHeader, list[ProgramHeader]]:
    if len(image) < ELF_HEADER.size:
        raise DevElfError("file is smaller than an ELF32 header")
    fields = ELF_HEADER.unpack_from(image)
    header = ElfHeader(*fields)
    if header.ident[:4] != ELF_MAGIC:
        raise DevElfError("file does not begin with ELF magic")
    if header.ident[4] != ELFCLASS32 or header.ident[5] != ELFDATA2LSB:
        raise DevElfError("development input must be a little-endian ELF32 file")
    if header.machine != EM_MIPS:
        raise DevElfError(f"development input has machine {header.machine}, expected MIPS")
    if header.ehsize != ELF_HEADER.size:
        raise DevElfError(f"ELF header size is {header.ehsize}, expected {ELF_HEADER.size}")
    if header.phentsize != PROGRAM_HEADER.size:
        raise DevElfError(
            f"program header size is {header.phentsize}, expected {PROGRAM_HEADER.size}"
        )
    table_end = header.phoff + header.phnum * header.phentsize
    if header.phoff < header.ehsize or table_end > len(image):
        raise DevElfError("program header table is outside the file")

    programs = []
    for index in range(header.phnum):
        offset = header.phoff + index * header.phentsize
        program = ProgramHeader(*PROGRAM_HEADER.unpack_from(image, offset))
        if program.file_size > program.memory_size:
            raise DevElfError(f"program header {index} has p_filesz larger than p_memsz")
        if (
            _range_end(program.offset, program.file_size, f"program header {index}")
            > len(image)
        ):
            raise DevElfError(f"program header {index} file range is outside the file")
        if program.alignment not in (0, 1):
            _align_up(0, program.alignment)
            if program.offset % program.alignment != program.vaddr % program.alignment:
                raise DevElfError(f"program header {index} violates p_offset/p_vaddr alignment")
        programs.append(program)
    return header, programs


def parse_linked_relocations(image: bytes | bytearray) -> list[LinkedRelocation]:
    header, _ = parse_elf(image)
    if header.shentsize != SECTION_HEADER.size:
        raise DevElfError(
            f"section header size is {header.shentsize}, expected {SECTION_HEADER.size}"
        )
    table_end = header.shoff + header.shnum * header.shentsize
    if header.shoff == 0 or table_end > len(image):
        raise DevElfError("section header table is outside the relocation ELF")
    sections = [
        SECTION_HEADER.unpack_from(image, header.shoff + index * header.shentsize)
        for index in range(header.shnum)
    ]

    relocations: list[LinkedRelocation] = []
    for section_index, section in enumerate(sections):
        kind, offset, size, link, entry_size = (
            section[1],
            section[4],
            section[5],
            section[6],
            section[9],
        )
        if kind != SHT_REL:
            continue
        if entry_size != REL_ENTRY.size or size % entry_size:
            raise DevElfError(f"relocation section {section_index} has an invalid entry size")
        if offset + size > len(image):
            raise DevElfError(f"relocation section {section_index} is outside the file")
        if link >= len(sections) or sections[link][1] != SHT_SYMTAB:
            raise DevElfError(f"relocation section {section_index} has no linked symbol table")
        symbols = sections[link]
        sym_offset, sym_size, sym_entry_size = symbols[4], symbols[5], symbols[9]
        if sym_entry_size != SYMBOL_ENTRY.size or sym_size % sym_entry_size:
            raise DevElfError("linked symbol table has an invalid entry size")
        if sym_offset + sym_size > len(image):
            raise DevElfError("linked symbol table is outside the file")
        symbol_count = sym_size // sym_entry_size
        for entry_offset in range(offset, offset + size, entry_size):
            reloc_offset, info = REL_ENTRY.unpack_from(image, entry_offset)
            symbol_index = info >> 8
            if symbol_index >= symbol_count:
                raise DevElfError("relocation references a symbol outside its symbol table")
            symbol = SYMBOL_ENTRY.unpack_from(
                image, sym_offset + symbol_index * sym_entry_size
            )
            relocations.append(
                LinkedRelocation(
                    reloc_offset,
                    info & 0xFF,
                    link,
                    symbol_index,
                    symbol[1],
                )
            )
    if not relocations:
        raise DevElfError("relocation ELF contains no SHT_REL entries")
    return relocations


def _unpack_word(image: bytes | bytearray, offset: int, context: str) -> int:
    if offset < 0 or offset + 4 > len(image):
        raise DevElfError(f"{context} word at 0x{offset:X} is outside the file")
    return struct.unpack_from("<I", image, offset)[0]


def _decode_mips_address(image: bytes | bytearray, hi_offset: int, lo_offset: int) -> int:
    hi_word = _unpack_word(image, hi_offset, "HI16")
    lo_word = _unpack_word(image, lo_offset, "LO16")
    if hi_word >> 26 != 0x0F:
        raise DevElfError(f"word at 0x{hi_offset:X} is not LUI")
    register = (hi_word >> 16) & 0x1F
    opcode = lo_word >> 26
    if opcode not in (0x09, 0x0D):
        raise DevElfError(f"word at 0x{lo_offset:X} is not ADDIU or ORI")
    if (lo_word >> 21) & 0x1F != register:
        raise DevElfError("LO16 instruction does not read the LUI destination register")
    high = (hi_word & 0xFFFF) << 16
    low = lo_word & 0xFFFF
    if opcode == 0x09 and low & 0x8000:
        low -= 0x10000
    return (high + low) & 0xFFFFFFFF


def assert_mips_address(
    image: bytes | bytearray, hi_offset: int, lo_offset: int, expected: int, context: str
) -> None:
    actual = _decode_mips_address(image, hi_offset, lo_offset)
    if actual != expected:
        raise DevElfError(f"{context} is 0x{actual:08X}, expected 0x{expected:08X}")


def patch_mips_address(
    image: bytearray, hi_offset: int, lo_offset: int, expected: int, replacement: int, context: str
) -> None:
    assert_mips_address(image, hi_offset, lo_offset, expected, context)
    hi_word = _unpack_word(image, hi_offset, context)
    lo_word = _unpack_word(image, lo_offset, context)
    opcode = lo_word >> 26
    if opcode == 0x09:
        high = ((replacement + 0x8000) >> 16) & 0xFFFF
    else:
        high = replacement >> 16
    struct.pack_into("<I", image, hi_offset, (hi_word & 0xFFFF0000) | high)
    struct.pack_into("<I", image, lo_offset, (lo_word & 0xFFFF0000) | (replacement & 0xFFFF))
    assert_mips_address(image, hi_offset, lo_offset, replacement, context)


def patch_u32(image: bytearray, offset: int, expected: int, replacement: int, context: str) -> None:
    actual = _unpack_word(image, offset, context)
    if actual != expected:
        raise DevElfError(f"{context} is 0x{actual:08X}, expected 0x{expected:08X}")
    struct.pack_into("<I", image, offset, replacement)


def _load_ranges(programs: list[ProgramHeader]) -> list[tuple[int, int]]:
    return [
        (program.vaddr, _range_end(program.vaddr, program.memory_size, "PT_LOAD"))
        for program in programs
        if program.kind == PT_LOAD and program.memory_size
    ]


def install_appended_load_segment(
    image: bytearray,
    *,
    base_file_size: int,
    vaddr: int,
    alignment: int,
    memory_size: int | None = None,
) -> ProgramHeader:
    header, programs = parse_elf(image)
    segment_offset = _align_up(base_file_size, alignment)
    if segment_offset >= len(image):
        raise DevElfError("linked image has no appended development payload")
    if any(image[base_file_size:segment_offset]):
        raise DevElfError("bytes between the retail image and development payload are not zero")
    file_size = len(image) - segment_offset
    if memory_size is None:
        memory_size = file_size
    if memory_size < file_size:
        raise DevElfError("development segment memory size is smaller than its file payload")
    if segment_offset % alignment != vaddr % alignment:
        raise DevElfError("development p_offset and p_vaddr are not congruent")

    new_end = _range_end(vaddr, memory_size, "development segment")
    for start, end in _load_ranges(programs):
        if vaddr < end and start < new_end:
            raise DevElfError(
                f"development segment 0x{vaddr:X}..0x{new_end:X} overlaps "
                f"PT_LOAD 0x{start:X}..0x{end:X}"
            )

    slot = header.phoff + header.phnum * header.phentsize
    slot_end = slot + PROGRAM_HEADER.size
    loaded_offsets = [program.offset for program in programs if program.file_size]
    first_payload = min(loaded_offsets) if loaded_offsets else len(image)
    if slot_end > first_payload:
        raise DevElfError("there is no room for another program header before loaded data")
    if any(image[slot:slot_end]):
        raise DevElfError("next program-header slot contains nonzero data")

    program = ProgramHeader(
        PT_LOAD,
        segment_offset,
        vaddr,
        vaddr,
        file_size,
        memory_size,
        PF_R | PF_W | PF_X,
        alignment,
    )
    PROGRAM_HEADER.pack_into(
        image,
        slot,
        program.kind,
        program.offset,
        program.vaddr,
        program.paddr,
        program.file_size,
        program.memory_size,
        program.flags,
        program.alignment,
    )
    struct.pack_into("<H", image, 44, header.phnum + 1)
    parse_elf(image)
    return program


def _file_offset_for_vaddr(programs: list[ProgramHeader], vaddr: int) -> int | None:
    for program in programs:
        if program.kind != PT_LOAD:
            continue
        end = program.vaddr + program.file_size
        if program.vaddr <= vaddr < end:
            return program.offset + vaddr - program.vaddr
    return None


def _vaddr_for_file_offset(programs: list[ProgramHeader], offset: int) -> int | None:
    for program in programs:
        if program.kind != PT_LOAD:
            continue
        end = program.offset + program.file_size
        if program.offset <= offset < end:
            return program.vaddr + offset - program.offset
    return None


def _move_ranges(spec: dict[str, Any]) -> list[tuple[int, int, int, int]]:
    ranges: list[tuple[int, int, int, int]] = []
    for index, move in enumerate(spec.get("moves", [])):
        old = _number(move.get("old_vaddr"), f"moves[{index}].old_vaddr")
        new = _number(move.get("new_vaddr"), f"moves[{index}].new_vaddr")
        size = _number(move.get("size"), f"moves[{index}].size")
        if size == 0:
            raise DevElfError(f"moves[{index}].size is zero")
        old_end = _range_end(old, size, f"moves[{index}] old range")
        new_end = _range_end(new, size, f"moves[{index}] new range")
        for other_old, other_old_end, other_new, other_new_end in ranges:
            if old < other_old_end and other_old < old_end:
                raise DevElfError(f"moves[{index}] old range overlaps another move")
            if new < other_new_end and other_new < new_end:
                raise DevElfError(f"moves[{index}] new range overlaps another move")
        ranges.append((old, old_end, new, new_end))
    return ranges


def _mapped_move_address(
    address: int, ranges: list[tuple[int, int, int, int]], *, reverse: bool = False
) -> int | None:
    for old_start, old_end, new_start, new_end in ranges:
        source_start, source_end = (new_start, new_end) if reverse else (old_start, old_end)
        target_start = old_start if reverse else new_start
        if source_start <= address < source_end:
            return target_start + address - source_start
    return None


def _jump_target(word: int, pc: int) -> int:
    return ((pc + 4) & 0xF0000000) | ((word & 0x03FFFFFF) << 2)


def scan_stale_move_references(
    image: bytes,
    ranges: list[tuple[int, int, int, int]],
) -> int:
    """Find ordinary address encodings that still target an abandoned slot."""

    _, programs = parse_elf(image)
    scanned_words = 0
    for program in programs:
        if program.kind != PT_LOAD or not program.file_size:
            continue
        start = _align_up(program.offset, 4)
        end = program.offset + program.file_size
        for file_offset in range(start, end - 3, 4):
            vaddr = program.vaddr + file_offset - program.offset
            word = _unpack_word(image, file_offset, "stale-reference scan")
            scanned_words += 1
            if _mapped_move_address(word, ranges) is not None:
                raise DevElfError(
                    f"stale absolute word at 0x{vaddr:X} points into an abandoned move slot"
                )
            opcode = word >> 26
            if opcode in (2, 3):
                target = _jump_target(word, vaddr)
                if _mapped_move_address(target, ranges) is not None:
                    raise DevElfError(
                        f"stale {'JAL' if opcode == 3 else 'J'} at 0x{vaddr:X} "
                        "targets an abandoned move slot"
                    )

            if not (program.flags & PF_X) or opcode != 0x0F:
                continue
            register = (word >> 16) & 0x1F
            for distance in range(1, 17):
                low_offset = file_offset + distance * 4
                if low_offset + 4 > end:
                    break
                low_word = _unpack_word(image, low_offset, "stale HI16/LO16 scan")
                low_opcode = low_word >> 26
                if low_opcode not in (0x09, 0x0D) or (low_word >> 21) & 0x1F != register:
                    continue
                address = _decode_mips_address(image, file_offset, low_offset)
                if _mapped_move_address(address, ranges) is not None:
                    raise DevElfError(
                        f"stale HI16/LO16 construction at 0x{vaddr:X}/"
                        f"0x{vaddr + distance * 4:X} targets an abandoned move slot"
                    )
    return scanned_words


def audit_relocation_closure(
    base: bytes,
    output: bytes,
    relocation_elf: bytes,
    spec: dict[str, Any],
) -> dict[str, int]:
    """Reject every prefix change not explained by a declared move or patch."""

    _, base_programs = parse_elf(base)
    output_header, output_programs = parse_elf(output)
    relocations = parse_linked_relocations(relocation_elf)
    ranges = _move_ranges(spec)
    if not ranges:
        raise DevElfError("relocation closure requires at least one moved range")

    allowed_bytes: set[int] = set(range(44, 46))
    old_header, _ = parse_elf(base)
    new_phdr_slot = old_header.phoff + old_header.phnum * old_header.phentsize
    allowed_bytes.update(range(new_phdr_slot, new_phdr_slot + PROGRAM_HEADER.size))

    for index, (old_start, old_end, _, _) in enumerate(ranges):
        slot_offset = _file_offset_for_vaddr(base_programs, old_start)
        if slot_offset is None or _file_offset_for_vaddr(base_programs, old_end - 1) is None:
            raise DevElfError(f"moves[{index}] old range is not wholly file-backed")
        old_slot = output[slot_offset : slot_offset + old_end - old_start]
        if any(old_slot):
            raise DevElfError(f"moves[{index}] abandoned retail slot is not zero-filled")
        allowed_bytes.update(range(slot_offset, slot_offset + old_end - old_start))

    for index, patch in enumerate(spec.get("heap_address_patches", [])):
        hi = _number(patch.get("hi_offset"), f"heap_address_patches[{index}].hi_offset")
        lo = _number(patch.get("lo_offset"), f"heap_address_patches[{index}].lo_offset")
        allowed_bytes.update(range(hi, hi + 4))
        allowed_bytes.update(range(lo, lo + 4))
    for index, offset in enumerate(spec.get("heap_word_offsets", [])):
        word = _number(offset, f"heap_word_offsets[{index}]")
        allowed_bytes.update(range(word, word + 4))

    unexplained_words = {
        offset & ~3
        for offset, (old, new) in enumerate(zip(base, output))
        if old != new and offset not in allowed_bytes
    }
    relocations_by_offset: dict[int, list[LinkedRelocation]] = {}
    moved_relocations = []
    for relocation in relocations:
        relocations_by_offset.setdefault(relocation.offset, []).append(relocation)
        if _mapped_move_address(relocation.symbol_value, ranges, reverse=True) is not None:
            moved_relocations.append(relocation)

    explained_words: set[int] = set()
    hi_lo_words: set[int] = set()
    for word_offset in sorted(unexplained_words):
        vaddr = _vaddr_for_file_offset(base_programs, word_offset)
        if vaddr is None:
            raise DevElfError(f"unexplained change at non-loaded file offset 0x{word_offset:X}")
        candidates = [
            relocation
            for relocation in relocations_by_offset.get(vaddr, [])
            if _mapped_move_address(relocation.symbol_value, ranges, reverse=True) is not None
        ]
        if not candidates:
            raise DevElfError(
                f"change at file offset 0x{word_offset:X} / vaddr 0x{vaddr:X} "
                "has no relocation to a moved symbol"
            )
        old_word = _unpack_word(base, word_offset, "closure old word")
        new_word = _unpack_word(output, word_offset, "closure new word")
        matched: LinkedRelocation | None = None
        for relocation in candidates:
            if (
                relocation.kind == 4
                and old_word >> 26 in (2, 3)
                and new_word >> 26 == old_word >> 26
            ):
                old_target = _jump_target(old_word, vaddr)
                new_target = _jump_target(new_word, vaddr)
                valid = _mapped_move_address(old_target, ranges) == new_target
            elif relocation.kind == 2:
                valid = _mapped_move_address(old_word, ranges) == new_word
            elif relocation.kind in (5, 6):
                # Paired HI16/LO16 encodings are checked together below.
                valid = True
            else:
                valid = False
            if valid:
                matched = relocation
                break
        if matched is None:
            raise DevElfError(
                f"relocated word at file offset 0x{word_offset:X} does not map "
                "the declared old interval to the new interval"
            )
        explained_words.add(word_offset)
        if matched.kind in (5, 6):
            hi_lo_words.add(word_offset)

    pending_hi: dict[tuple[int, int], list[LinkedRelocation]] = {}
    checked_hi_lo_words: set[int] = set()
    checked_pairs = 0
    for relocation in moved_relocations:
        symbol_key = (relocation.symbol_table_index, relocation.symbol_index)
        if relocation.kind == 5:
            pending_hi.setdefault(symbol_key, []).append(relocation)
        elif relocation.kind == 6:
            for hi in pending_hi.pop(symbol_key, []):
                hi_file = _file_offset_for_vaddr(base_programs, hi.offset)
                lo_file = _file_offset_for_vaddr(base_programs, relocation.offset)
                if hi_file is None or lo_file is None:
                    continue
                if hi_file not in unexplained_words and lo_file not in unexplained_words:
                    continue
                old_address = _decode_mips_address(base, hi_file, lo_file)
                new_address = _decode_mips_address(output, hi_file, lo_file)
                if _mapped_move_address(old_address, ranges) != new_address:
                    raise DevElfError(
                        f"HI16/LO16 pair at 0x{hi.offset:X}/"
                        f"0x{relocation.offset:X} does not follow its declared move"
                    )
                checked_hi_lo_words.update((hi_file, lo_file))
                checked_pairs += 1

    unchecked_hi_lo = hi_lo_words - checked_hi_lo_words
    if unchecked_hi_lo:
        first = min(unchecked_hi_lo)
        raise DevElfError(
            f"changed HI16/LO16 relocation at file offset 0x{first:X} "
            "has no validated pair"
        )

    moved_sites_in_prefix = {
        relocation.offset
        for relocation in moved_relocations
        if relocation.kind in (2, 4)
        and _file_offset_for_vaddr(base_programs, relocation.offset) is not None
    }
    changed_vaddrs = {
        _vaddr_for_file_offset(base_programs, offset) for offset in unexplained_words
    }
    missing = moved_sites_in_prefix - changed_vaddrs
    if missing:
        first = min(missing)
        raise DevElfError(
            f"relocation to moved code at 0x{first:X} did not change the linked image"
        )

    extension_relocations = 0
    for index, move in enumerate(spec.get("moves", [])):
        start = _number(move.get("new_vaddr"), f"moves[{index}].new_vaddr")
        size = _number(move.get("size"), f"moves[{index}].size")
        actual = sum(start <= relocation.offset < start + size for relocation in relocations)
        expected = _number(
            move.get("expected_relocations"), f"moves[{index}].expected_relocations"
        )
        if actual != expected:
            raise DevElfError(
                f"moves[{index}] retained {actual} relocation entries, expected {expected}"
            )
        extension_relocations += actual

    extension = output_programs[-1]
    if extension.kind != PT_LOAD or extension.vaddr != _number(
        spec.get("extension_vaddr"), "extension_vaddr"
    ):
        raise DevElfError("final PT_LOAD is not the declared development extension")
    if output_header.phnum != old_header.phnum + 1:
        raise DevElfError("development ELF did not gain exactly one program header")
    extension_end = extension.vaddr + extension.file_size
    for index, (_, _, new_start, new_end) in enumerate(ranges):
        if new_start < extension.vaddr or new_end > extension_end:
            raise DevElfError(
                f"moves[{index}] new range is outside the development payload"
            )
    scanned_words = scan_stale_move_references(output, ranges)
    return {
        "changed_relocation_words": len(explained_words),
        "moved_relocations": len(moved_relocations),
        "extension_relocations": extension_relocations,
        "checked_hi_lo_pairs": checked_pairs,
        "stale_reference_words_scanned": scanned_words,
    }


def render_linker_script(base: str, spec: dict[str, Any]) -> str:
    moves = spec.get("moves")
    if not isinstance(moves, list) or not moves:
        raise DevElfError("development descriptor must contain at least one section move")

    text = base
    extension_lines: list[str] = []
    for index, move in enumerate(moves):
        if not isinstance(move, dict):
            raise DevElfError(f"moves[{index}] is not an object")
        obj = move.get("object")
        section = move.get("section")
        if not isinstance(obj, str) or not isinstance(section, str):
            raise DevElfError(f"moves[{index}] needs string object and section fields")
        size = _number(move.get("size"), f"moves[{index}].size")
        needle = f"{obj}({section});"
        matching = [line for line in text.splitlines() if line.strip() == needle]
        if len(matching) != 1:
            raise DevElfError(
                f"expected one linker-script placement for {needle}, found {len(matching)}"
            )
        original = matching[0]
        indent = original[: len(original) - len(original.lstrip())]
        replacement = f"{indent}. += 0x{size:X}; /* development slot for {obj}({section}) */"
        text = text.replace(original, replacement, 1)
        extension_lines.append(f"        {obj}({section});")

    marker = "    /DISCARD/ :"
    if text.count(marker) != 1:
        raise DevElfError("could not find the unique /DISCARD/ section in the linker script")
    vaddr = _number(spec.get("extension_vaddr"), "extension_vaddr")
    alignment = _number(spec.get("alignment", 0x1000), "alignment")
    _align_up(vaddr, alignment)
    if vaddr % alignment:
        raise DevElfError("extension_vaddr is not aligned")
    block = "\n".join(
        [
            f"    __romPos = ALIGN(__romPos, 0x{alignment:X});",
            "    dev_extension_ROM_START = __romPos;",
            f"    .dev_extension 0x{vaddr:X} : AT(dev_extension_ROM_START)",
            "    {",
            "        FILL(0x00000000);",
            "        dev_extension_TEXT_START = .;",
            *extension_lines,
            "        dev_extension_TEXT_END = .;",
            "    }",
            "    __romPos += SIZEOF(.dev_extension);",
            "    dev_extension_ROM_END = __romPos;",
            "    dev_extension_VRAM_END = .;",
            "    ASSERT(ADDR(.dev_extension) >= elf_trailer_VRAM_END,",
            '           "development extension overlaps the linker wrapper trailer")',
            "",
        ]
    )
    return text.replace(marker, block + marker, 1)


def _read_descriptor(path: Path) -> dict[str, Any]:
    try:
        spec = json.loads(path.read_text())
    except (OSError, json.JSONDecodeError) as exc:
        raise DevElfError(f"could not read development descriptor {path}: {exc}") from exc
    if not isinstance(spec, dict) or spec.get("format") != 1:
        raise DevElfError("development descriptor must be an object with format 1")
    return spec


def finalize_image(
    base: bytes, linked: bytes, spec: dict[str, Any]
) -> tuple[bytes, dict[str, int]]:
    expected_sha1 = spec.get("base_sha1")
    actual_sha1 = hashlib.sha1(base).hexdigest()
    if expected_sha1 != actual_sha1:
        raise DevElfError(f"retail input SHA-1 is {actual_sha1}, expected {expected_sha1}")
    if linked[:4] != ELF_MAGIC:
        raise DevElfError("linked development image is not an ELF file")
    image = bytearray(linked)
    extension_vaddr = _number(spec.get("extension_vaddr"), "extension_vaddr")
    alignment = _number(spec.get("alignment", 0x1000), "alignment")
    program = install_appended_load_segment(
        image,
        base_file_size=len(base),
        vaddr=extension_vaddr,
        alignment=alignment,
    )
    if "extension_size" in spec:
        expected_size = _number(spec["extension_size"], "extension_size")
        if program.file_size != expected_size:
            raise DevElfError(
                f"development payload is 0x{program.file_size:X} bytes, "
                f"expected 0x{expected_size:X}"
            )
    heap_alignment = _number(spec.get("heap_alignment", 0x80), "heap_alignment")
    heap_start = _align_up(program.vaddr + program.memory_size, heap_alignment)
    retail_end = _number(spec.get("retail_static_end"), "retail_static_end")

    for index, assertion in enumerate(spec.get("bss_end_assertions", [])):
        assert_mips_address(
            image,
            _number(assertion.get("hi_offset"), f"bss_end_assertions[{index}].hi_offset"),
            _number(assertion.get("lo_offset"), f"bss_end_assertions[{index}].lo_offset"),
            retail_end,
            f"BSS end assertion {index}",
        )
    for index, patch in enumerate(spec.get("heap_address_patches", [])):
        patch_mips_address(
            image,
            _number(patch.get("hi_offset"), f"heap_address_patches[{index}].hi_offset"),
            _number(patch.get("lo_offset"), f"heap_address_patches[{index}].lo_offset"),
            retail_end,
            heap_start,
            f"heap address patch {index}",
        )
    for index, offset in enumerate(spec.get("heap_word_offsets", [])):
        patch_u32(
            image,
            _number(offset, f"heap_word_offsets[{index}]"),
            retail_end,
            heap_start,
            f"heap word patch {index}",
        )
    parse_elf(image)
    return bytes(image), {
        "segment_offset": program.offset,
        "segment_vaddr": program.vaddr,
        "segment_file_size": program.file_size,
        "segment_memory_size": program.memory_size,
        "heap_start": heap_start,
    }


def _write(path: Path, data: str | bytes) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    if isinstance(data, str):
        path.write_text(data)
    else:
        path.write_bytes(data)


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    subparsers = parser.add_subparsers(dest="command", required=True)

    link = subparsers.add_parser("link-script", help="create the development linker script")
    link.add_argument("descriptor", type=Path)
    link.add_argument("input", type=Path)
    link.add_argument("output", type=Path)

    finalize = subparsers.add_parser("finalize", help="install and validate the appended PT_LOAD")
    finalize.add_argument("descriptor", type=Path)
    finalize.add_argument("retail", type=Path)
    finalize.add_argument("linked", type=Path)
    finalize.add_argument("output", type=Path)
    finalize.add_argument(
        "--reloc-elf",
        type=Path,
        required=True,
        help="linked wrapper ELF produced with --emit-relocs",
    )

    args = parser.parse_args(argv)
    try:
        spec = _read_descriptor(args.descriptor)
        if args.command == "link-script":
            _write(args.output, render_linker_script(args.input.read_text(), spec))
        else:
            output, summary = finalize_image(
                args.retail.read_bytes(), args.linked.read_bytes(), spec
            )
            summary.update(
                audit_relocation_closure(
                    args.retail.read_bytes(),
                    output,
                    args.reloc_elf.read_bytes(),
                    spec,
                )
            )
            _write(args.output, output)
            print(json.dumps(summary, sort_keys=True))
    except (DevElfError, OSError) as exc:
        parser.exit(1, f"dev_elf: {exc}\n")
    return 0


if __name__ == "__main__":
    sys.exit(main())
