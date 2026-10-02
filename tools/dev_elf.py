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
SHT_STRTAB = 3
SHT_RELA = 4
SHT_NOBITS = 8
SHT_REL = 9
SHF_ALLOC = 2
SHN_UNDEF = 0
SHN_MIPS_ACOMMON = 0xFF00
SHN_MIPS_SCOMMON = 0xFF03
SHN_COMMON = 0xFFF2
STB_GLOBAL = 1
STB_WEAK = 2
STT_OBJECT = 1
STT_FUNC = 2
STT_SECTION = 3
R_MIPS_32 = 2
R_MIPS_26 = 4
R_MIPS_HI16 = 5
R_MIPS_LO16 = 6
R_MIPS_GPREL16 = 7
PF_X = 1
PF_W = 2
PF_R = 4
ADDRESS_LOW_OPCODES = {
    0x08,  # ADDI
    0x09,  # ADDIU
    0x0D,  # ORI
    0x18,  # DADDI
    0x19,  # DADDIU
    0x1A,  # LDL
    0x1B,  # LDR
    0x1E,  # LQ
    0x1F,  # SQ
    *range(0x20, 0x40),  # load/store, CACHE, and PREF forms
}


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
    symbol_name: str
    symbol_info: int = 0
    symbol_section_index: int = SHN_UNDEF

    @property
    def symbol_kind(self) -> int:
        return self.symbol_info & 0xF


@dataclass(frozen=True)
class LinkedSymbol:
    table_index: int
    symbol_index: int
    name: str
    value: int
    size: int
    info: int
    other: int
    section_index: int

    @property
    def binding(self) -> int:
        return self.info >> 4

    @property
    def kind(self) -> int:
        return self.info & 0xF


@dataclass(frozen=True)
class ObjectSection:
    index: int
    name: str
    kind: int
    flags: int
    address: int
    offset: int
    size: int
    link: int
    info: int
    alignment: int
    entry_size: int


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


def _move_old_size(move: dict[str, Any], index: int) -> int:
    legacy_size = move.get("size")
    old_size = _number(
        move.get("old_size", legacy_size), f"moves[{index}].old_size"
    )
    if old_size == 0:
        raise DevElfError(f"moves[{index}].old_size is zero")
    return old_size


def _move_new_size(move: dict[str, Any], index: int) -> int:
    legacy_size = move.get("size")
    new_size = _number(
        move.get("new_size", legacy_size), f"moves[{index}].new_size"
    )
    if new_size == 0:
        raise DevElfError(f"moves[{index}].new_size is zero")
    return new_size


def _move_storage(move: dict[str, Any], index: int) -> str:
    storage = move.get("storage", "file")
    if storage not in ("file", "nobits"):
        raise DevElfError(
            f"moves[{index}].storage must be 'file' or 'nobits', got {storage!r}"
        )
    return storage


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
        string_index = symbols[6]
        if string_index >= len(sections) or sections[string_index][1] != SHT_STRTAB:
            raise DevElfError("linked symbol table has no string table")
        strings = sections[string_index]
        string_offset, string_size = strings[4], strings[5]
        if string_offset + string_size > len(image):
            raise DevElfError("linked string table is outside the file")
        symbol_count = sym_size // sym_entry_size
        for entry_offset in range(offset, offset + size, entry_size):
            reloc_offset, info = REL_ENTRY.unpack_from(image, entry_offset)
            symbol_index = info >> 8
            if symbol_index >= symbol_count:
                raise DevElfError("relocation references a symbol outside its symbol table")
            symbol = SYMBOL_ENTRY.unpack_from(
                image, sym_offset + symbol_index * sym_entry_size
            )
            name_offset = symbol[0]
            if name_offset >= string_size:
                raise DevElfError("relocation symbol name is outside its string table")
            name_start = string_offset + name_offset
            name_end = image.find(b"\0", name_start, string_offset + string_size)
            if name_end < 0:
                raise DevElfError("relocation symbol name is not terminated")
            relocations.append(
                LinkedRelocation(
                    reloc_offset,
                    info & 0xFF,
                    link,
                    symbol_index,
                    symbol[1],
                    bytes(image[name_start:name_end]).decode("ascii", errors="replace"),
                    symbol[3],
                    symbol[5],
                )
            )
    if not relocations:
        raise DevElfError("relocation ELF contains no SHT_REL entries")
    return relocations


def parse_linked_symbols(image: bytes | bytearray) -> list[LinkedSymbol]:
    """Return all symbols from every complete ELF32 symbol table."""

    header, _ = parse_elf(image)
    if header.shentsize != SECTION_HEADER.size:
        raise DevElfError(
            f"section header size is {header.shentsize}, expected {SECTION_HEADER.size}"
        )
    table_end = header.shoff + header.shnum * header.shentsize
    if header.shoff == 0 or table_end > len(image):
        raise DevElfError("section header table is outside the symbol ELF")
    sections = [
        SECTION_HEADER.unpack_from(image, header.shoff + index * header.shentsize)
        for index in range(header.shnum)
    ]

    result: list[LinkedSymbol] = []
    for table_index, section in enumerate(sections):
        if section[1] != SHT_SYMTAB:
            continue
        offset, size, string_index, entry_size = (
            section[4],
            section[5],
            section[6],
            section[9],
        )
        if entry_size != SYMBOL_ENTRY.size or size % entry_size:
            raise DevElfError(f"symbol table {table_index} has an invalid entry size")
        if offset + size > len(image):
            raise DevElfError(f"symbol table {table_index} is outside the file")
        if string_index >= len(sections) or sections[string_index][1] != SHT_STRTAB:
            raise DevElfError(f"symbol table {table_index} has no string table")
        strings = sections[string_index]
        string_offset, string_size = strings[4], strings[5]
        if string_offset + string_size > len(image):
            raise DevElfError(f"symbol table {table_index} string table is outside the file")
        for symbol_index, entry_offset in enumerate(
            range(offset, offset + size, entry_size)
        ):
            name_offset, value, symbol_size, info, other, section_index = (
                SYMBOL_ENTRY.unpack_from(image, entry_offset)
            )
            if name_offset >= string_size:
                raise DevElfError("symbol name is outside its string table")
            name_start = string_offset + name_offset
            name_end = image.find(b"\0", name_start, string_offset + string_size)
            if name_end < 0:
                raise DevElfError("symbol name is not terminated")
            result.append(
                LinkedSymbol(
                    table_index,
                    symbol_index,
                    bytes(image[name_start:name_end]).decode(
                        "ascii", errors="replace"
                    ),
                    value,
                    symbol_size,
                    info,
                    other,
                    section_index,
                )
            )
    if not result:
        raise DevElfError("symbol ELF contains no SHT_SYMTAB entries")
    return result


def _object_sections(image: bytes | bytearray) -> list[ObjectSection]:
    """Parse named section headers from a little-endian MIPS ELF32 object."""

    if len(image) < ELF_HEADER.size:
        raise DevElfError("replacement object is smaller than an ELF32 header")
    header = ElfHeader(*ELF_HEADER.unpack_from(image))
    if header.ident[:4] != ELF_MAGIC:
        raise DevElfError("replacement object does not begin with ELF magic")
    if header.ident[4] != ELFCLASS32 or header.ident[5] != ELFDATA2LSB:
        raise DevElfError("replacement object must be little-endian ELF32")
    if header.machine != EM_MIPS:
        raise DevElfError("replacement object is not MIPS")
    if header.ehsize != ELF_HEADER.size:
        raise DevElfError("replacement object has an invalid ELF-header size")
    if header.shentsize != SECTION_HEADER.size:
        raise DevElfError("replacement object has an invalid section-header size")
    table_end = header.shoff + header.shnum * header.shentsize
    if header.shoff == 0 or table_end > len(image):
        raise DevElfError("replacement object section headers are outside the file")
    sections = [
        SECTION_HEADER.unpack_from(image, header.shoff + index * header.shentsize)
        for index in range(header.shnum)
    ]
    if header.shstrndx >= len(sections):
        raise DevElfError("replacement object has no section-name string table")
    strings = sections[header.shstrndx]
    if strings[1] != SHT_STRTAB or strings[4] + strings[5] > len(image):
        raise DevElfError("replacement object section-name table is invalid")
    result = []
    for section_index, section in enumerate(sections):
        name_offset, section_type, flags, address, offset, size, link, info, alignment, entry_size = (
            section[0],
            section[1],
            section[2],
            section[3],
            section[4],
            section[5],
            section[6],
            section[7],
            section[8],
            section[9],
        )
        if section_type != SHT_NOBITS and offset + size > len(image):
            raise DevElfError(
                f"replacement object section {section_index} is outside the file"
            )
        if name_offset >= strings[5]:
            raise DevElfError("replacement object section name is outside its table")
        name_start = strings[4] + name_offset
        name_end = image.find(b"\0", name_start, strings[4] + strings[5])
        if name_end < 0:
            raise DevElfError("replacement object section name is not terminated")
        name = bytes(image[name_start:name_end]).decode("ascii", errors="replace")
        result.append(
            ObjectSection(
                section_index,
                name,
                section_type,
                flags,
                address,
                offset,
                size,
                link,
                info,
                alignment,
                entry_size,
            )
        )
    return result


def parse_allocated_sections(image: bytes | bytearray) -> dict[str, ObjectSection]:
    """Return allocated input sections from an ELF32 object."""

    result: dict[str, ObjectSection] = {}
    for section in _object_sections(image):
        if not section.flags & SHF_ALLOC:
            continue
        if section.name in result:
            raise DevElfError(
                f"replacement object has duplicate section {section.name}"
            )
        result[section.name] = section
    return result


def _object_symbols(
    image: bytes | bytearray, sections: list[ObjectSection]
) -> list[LinkedSymbol]:
    """Return symbols from a relocatable object without requiring program headers."""

    result = []
    for table in sections:
        if table.kind != SHT_SYMTAB:
            continue
        if table.entry_size != SYMBOL_ENTRY.size or table.size % table.entry_size:
            raise DevElfError(
                f"replacement object symbol table {table.index} has an invalid entry size"
            )
        if table.link >= len(sections) or sections[table.link].kind != SHT_STRTAB:
            raise DevElfError(
                f"replacement object symbol table {table.index} has no string table"
            )
        strings = sections[table.link]
        for symbol_index, entry_offset in enumerate(
            range(table.offset, table.offset + table.size, table.entry_size)
        ):
            name_offset, value, size, info, other, section_index = (
                SYMBOL_ENTRY.unpack_from(image, entry_offset)
            )
            if name_offset >= strings.size:
                raise DevElfError(
                    "replacement object symbol name is outside its string table"
                )
            name_start = strings.offset + name_offset
            name_end = image.find(b"\0", name_start, strings.offset + strings.size)
            if name_end < 0:
                raise DevElfError("replacement object symbol name is not terminated")
            result.append(
                LinkedSymbol(
                    table.index,
                    symbol_index,
                    bytes(image[name_start:name_end]).decode(
                        "ascii", errors="replace"
                    ),
                    value,
                    size,
                    info,
                    other,
                    section_index,
                )
            )
    if not result:
        raise DevElfError("replacement object contains no symbol table")
    return result


def _fallback_symbol(
    symbols: list[LinkedSymbol], name: str, context: str
) -> LinkedSymbol:
    matching = [
        symbol
        for symbol in symbols
        if symbol.name == name and symbol.section_index != SHN_UNDEF
    ]
    if len(matching) != 1:
        raise DevElfError(
            f"{context} fallback symbol {name} has {len(matching)} definitions, expected one"
        )
    symbol = matching[0]
    if symbol.kind != STT_FUNC or symbol.size == 0:
        raise DevElfError(f"{context} fallback symbol {name} is not a sized function")
    return symbol


def _fallback_relocations(
    image: bytes | bytearray,
    sections: list[ObjectSection],
    symbols: list[LinkedSymbol],
    function: LinkedSymbol,
) -> list[tuple[int, int, str, int, int, str]]:
    """Describe a fallback's REL entries relative to the function start."""

    symbol_tables = {
        symbol.table_index: {} for symbol in symbols
    }
    for symbol in symbols:
        symbol_tables[symbol.table_index][symbol.symbol_index] = symbol
    result = []
    for relocation_section in sections:
        if relocation_section.info != function.section_index:
            continue
        if relocation_section.kind == SHT_RELA and relocation_section.size:
            raise DevElfError("replacement fallback uses unsupported RELA relocations")
        if relocation_section.kind != SHT_REL:
            continue
        if (
            relocation_section.entry_size != REL_ENTRY.size
            or relocation_section.size % relocation_section.entry_size
        ):
            raise DevElfError("replacement fallback relocation table is malformed")
        table = symbol_tables.get(relocation_section.link)
        if table is None:
            raise DevElfError("replacement fallback relocation has no symbol table")
        for entry_offset in range(
            relocation_section.offset,
            relocation_section.offset + relocation_section.size,
            relocation_section.entry_size,
        ):
            offset, info = REL_ENTRY.unpack_from(image, entry_offset)
            if not function.value <= offset < function.value + function.size:
                continue
            target = table.get(info >> 8)
            if target is None:
                raise DevElfError(
                    "replacement fallback relocation references an invalid symbol"
                )
            if target.section_index < len(sections):
                target_section = sections[target.section_index].name
            else:
                target_section = f"0x{target.section_index:X}"
            result.append(
                (
                    offset - function.value,
                    info & 0xFF,
                    target.name,
                    target.binding,
                    target.kind,
                    target_section,
                )
            )
    return sorted(result)


def _audit_fallback_symbols(
    replacement_index: int,
    replacement: dict[str, Any],
    retail_image: bytes,
    development_image: bytes,
) -> int:
    names = replacement.get("fallback_symbols", [])
    if not isinstance(names, list) or not all(
        isinstance(name, str) and name for name in names
    ):
        raise DevElfError(
            f"replacements[{replacement_index}].fallback_symbols must be a list of names"
        )
    if len(names) != len(set(names)):
        raise DevElfError(
            f"replacements[{replacement_index}].fallback_symbols contains a duplicate"
        )
    if not names:
        return 0

    retail_sections = _object_sections(retail_image)
    development_sections = _object_sections(development_image)
    retail_symbols = _object_symbols(retail_image, retail_sections)
    development_symbols = _object_symbols(development_image, development_sections)
    for name in names:
        context = f"replacements[{replacement_index}]"
        old_symbol = _fallback_symbol(retail_symbols, name, f"{context} retail")
        new_symbol = _fallback_symbol(
            development_symbols, name, f"{context} development"
        )
        if (
            old_symbol.size != new_symbol.size
            or old_symbol.binding != new_symbol.binding
            or old_symbol.kind != new_symbol.kind
            or (old_symbol.other & 3) != (new_symbol.other & 3)
        ):
            raise DevElfError(f"{context} fallback symbol {name} changed its contract")
        if (
            old_symbol.section_index >= len(retail_sections)
            or new_symbol.section_index >= len(development_sections)
        ):
            raise DevElfError(f"{context} fallback symbol {name} has no input section")
        old_section = retail_sections[old_symbol.section_index]
        new_section = development_sections[new_symbol.section_index]
        if old_section.name != ".text" or new_section.name != ".text":
            raise DevElfError(f"{context} fallback symbol {name} is not in .text")
        old_start = old_section.offset + old_symbol.value
        new_start = new_section.offset + new_symbol.value
        old_end = old_start + old_symbol.size
        new_end = new_start + new_symbol.size
        if old_end > old_section.offset + old_section.size:
            raise DevElfError(f"{context} retail fallback symbol {name} exceeds .text")
        if new_end > new_section.offset + new_section.size:
            raise DevElfError(
                f"{context} development fallback symbol {name} exceeds .text"
            )
        if retail_image[old_start:old_end] != development_image[new_start:new_end]:
            raise DevElfError(f"{context} fallback symbol {name} changed bytes")
        old_relocations = _fallback_relocations(
            retail_image, retail_sections, retail_symbols, old_symbol
        )
        new_relocations = _fallback_relocations(
            development_image,
            development_sections,
            development_symbols,
            new_symbol,
        )
        if old_relocations != new_relocations:
            raise DevElfError(
                f"{context} fallback symbol {name} changed its relocation contract"
            )
    return len(names)


def audit_replacement_objects(
    spec: dict[str, Any], objects: list[bytes], retail_objects: list[bytes] | None = None
) -> dict[str, int]:
    replacements = spec.get("replacements", [])
    if len(objects) != len(replacements):
        raise DevElfError(
            f"received {len(objects)} replacement objects for "
            f"{len(replacements)} declarations"
        )
    if retail_objects is not None and len(retail_objects) != len(replacements):
        raise DevElfError(
            f"received {len(retail_objects)} retail objects for "
            f"{len(replacements)} replacement declarations"
        )
    fallback_count = 0
    for index, (replacement, image) in enumerate(zip(replacements, objects)):
        retail_obj = replacement.get("retail_object")
        expected: dict[str, int | None] = {}
        for move_index, move in enumerate(spec.get("moves", [])):
            if move.get("object") != retail_obj:
                continue
            section = move.get("section")
            if not isinstance(section, str):
                raise DevElfError(f"moves[{move_index}].section is not a string")
            expected[section] = None
        for section_index, entry in enumerate(
            replacement.get("retained_sections", [])
        ):
            section = entry.get("section")
            if not isinstance(section, str):
                raise DevElfError(
                    f"replacements[{index}].retained_sections"
                    f"[{section_index}].section is not a string"
                )
            expected[section] = _number(
                entry.get("size"),
                f"replacements[{index}].retained_sections"
                f"[{section_index}].size",
            )
            if expected[section] != 0:
                raise DevElfError(
                    f"replacements[{index}].retained_sections"
                    f"[{section_index}] must remain empty"
                )
        actual = parse_allocated_sections(image)
        unaccounted = sorted(set(actual) - set(expected))
        if unaccounted:
            section = unaccounted[0]
            raise DevElfError(
                f"replacements[{index}] has unaccounted allocated section "
                f"{section} of size 0x{actual[section].size:X}"
            )
        missing = sorted(
            section
            for section, expected_size in expected.items()
            if section not in actual and expected_size != 0
        )
        if missing:
            raise DevElfError(
                f"replacements[{index}] object is missing section {missing[0]}"
            )
        for section, expected_size in expected.items():
            if (
                section in actual
                and expected_size is not None
                and actual[section].size != expected_size
            ):
                raise DevElfError(
                    f"replacements[{index}] section {section} has size "
                    f"0x{actual[section].size:X}, expected 0x{expected_size:X}"
                )
        symbols = _object_symbols(image, _object_sections(image))
        common = [
            symbol
            for symbol in symbols
            if symbol.size
            and symbol.section_index
            in (SHN_MIPS_ACOMMON, SHN_MIPS_SCOMMON, SHN_COMMON)
        ]
        if common:
            raise DevElfError(
                f"replacements[{index}] symbol {common[0].name or '<anonymous>'} "
                "uses COMMON storage"
            )
        fallback_symbols = replacement.get("fallback_symbols", [])
        if fallback_symbols and retail_objects is None:
            raise DevElfError(
                f"replacements[{index}] fallback audit requires the retail object"
            )
        if retail_objects is not None:
            fallback_count += _audit_fallback_symbols(
                index, replacement, retail_objects[index], image
            )
    return {"fallback_symbols": fallback_count}


def _effective_move_object_paths(spec: dict[str, Any]) -> list[str]:
    replacements = {
        replacement.get("retail_object"): replacement.get("object")
        for replacement in spec.get("replacements", [])
        if isinstance(replacement, dict)
    }
    result = []
    for index, move in enumerate(spec.get("moves", [])):
        if not isinstance(move, dict):
            raise DevElfError(f"moves[{index}] is not an object")
        obj = move.get("object")
        if not isinstance(obj, str):
            raise DevElfError(f"moves[{index}].object is not a string")
        linked_obj = replacements.get(obj, obj)
        if not isinstance(linked_obj, str):
            raise DevElfError(f"moves[{index}] replacement object is not a string")
        result.append(linked_obj)
    return result


def audit_moved_sections(
    spec: dict[str, Any], objects: dict[str, bytes]
) -> dict[str, int]:
    """Require each moved input section to match its declared storage kind."""

    paths = _effective_move_object_paths(spec)
    parsed: dict[str, dict[str, ObjectSection]] = {}
    for path in set(paths):
        if path not in objects:
            raise DevElfError(f"no object image was supplied for moved object {path}")
        parsed[path] = parse_allocated_sections(objects[path])

    for index, (move, path) in enumerate(zip(spec.get("moves", []), paths)):
        section_name = move.get("section")
        if not isinstance(section_name, str):
            raise DevElfError(f"moves[{index}].section is not a string")
        section = parsed[path].get(section_name)
        if section is None:
            raise DevElfError(
                f"moves[{index}] object {path} has no allocated section {section_name}"
            )
        storage = _move_storage(move, index)
        is_nobits = section.kind == SHT_NOBITS
        if (storage == "nobits") != is_nobits:
            actual = "SHT_NOBITS" if is_nobits else "file-backed"
            raise DevElfError(
                f"moves[{index}] declares {storage} storage but {path}"
                f"({section_name}) is {actual}"
            )
    return {"moved_input_sections": len(paths)}


def _unique_defined_symbol(
    symbols: list[LinkedSymbol], name: str, context: str
) -> LinkedSymbol:
    matching = [
        symbol
        for symbol in symbols
        if symbol.name == name and symbol.section_index != SHN_UNDEF
    ]
    if len(matching) != 1:
        raise DevElfError(
            f"{context} symbol {name} has {len(matching)} definitions, expected one"
        )
    return matching[0]


def _linked_extension_sizes(
    spec: dict[str, Any], relocation_elf: bytes
) -> tuple[int, int]:
    """Resolve the file-backed and in-memory extension spans from linker symbols."""

    symbols = parse_linked_symbols(relocation_elf)
    start = _unique_defined_symbol(
        symbols, "dev_extension_VRAM_START", "development extension"
    ).value
    file_end = _unique_defined_symbol(
        symbols, "dev_extension_FILE_END", "development extension"
    ).value
    memory_end = _unique_defined_symbol(
        symbols, "dev_extension_VRAM_END", "development extension"
    ).value
    expected_start = _number(spec.get("extension_vaddr"), "extension_vaddr")
    if start != expected_start:
        raise DevElfError(
            f"development extension starts at 0x{start:X}, expected 0x{expected_start:X}"
        )
    if file_end <= start:
        raise DevElfError("development extension has an empty file-backed prefix")
    if memory_end < file_end:
        raise DevElfError("development extension memory end precedes its file end")
    return file_end - start, memory_end - start


def _matched_gp_values(
    relocations: list[LinkedRelocation],
    development_symbols: list[LinkedSymbol],
    retail_symbol_elf: bytes | None,
) -> tuple[int | None, int | None]:
    """Resolve the fixed retail/development GP pair when GPREL16 is present."""

    if not any(relocation.kind == R_MIPS_GPREL16 for relocation in relocations):
        return None, None
    development_gp = _unique_defined_symbol(
        development_symbols, "_gp", "development ELF"
    ).value
    if retail_symbol_elf is None:
        raise DevElfError(
            "R_MIPS_GPREL16 relocation audit requires the retail symbol ELF"
        )
    retail_gp = _unique_defined_symbol(
        parse_linked_symbols(retail_symbol_elf), "_gp", "retail ELF"
    ).value
    if development_gp != retail_gp:
        raise DevElfError(
            f"development _gp 0x{development_gp:X} differs from retail "
            f"0x{retail_gp:X}"
        )
    return development_gp, retail_gp


def _resolve_linked_layout(
    spec: dict[str, Any], relocation_elf: bytes
) -> tuple[dict[str, Any], list[LinkedSymbol]]:
    """Fill replacement-owned spans from linker-script boundary symbols."""

    symbols = parse_linked_symbols(relocation_elf)
    needs_linked_layout = bool(spec.get("replacements")) or any(
        "new_vaddr" not in move
        or ("new_size" not in move and "size" not in move)
        for move in spec.get("moves", [])
    ) or any(
        "new_vaddr" not in addition or "size" not in addition
        for addition in spec.get("additions", [])
    )
    if not needs_linked_layout:
        return spec, symbols
    resolved = json.loads(json.dumps(spec))
    for index, move in enumerate(resolved.get("moves", [])):
        start = _unique_defined_symbol(
            symbols, f"dev_move_{index}_START", f"moves[{index}] start"
        ).value
        end = _unique_defined_symbol(
            symbols, f"dev_move_{index}_END", f"moves[{index}] end"
        ).value
        if end <= start:
            raise DevElfError(f"moves[{index}] linked span is empty or reversed")
        if "new_vaddr" in move and _number(
            move["new_vaddr"], f"moves[{index}].new_vaddr"
        ) != start:
            raise DevElfError(f"moves[{index}] linked start does not match descriptor")
        expected_size = move.get("new_size", move.get("size"))
        if expected_size is not None and _number(
            expected_size, f"moves[{index}].new_size"
        ) != end - start:
            raise DevElfError(f"moves[{index}] linked size does not match descriptor")
        move["new_vaddr"] = start
        move["new_size"] = end - start

    for index, addition in enumerate(resolved.get("additions", [])):
        start = _unique_defined_symbol(
            symbols, f"dev_addition_{index}_START", f"additions[{index}] start"
        ).value
        end = _unique_defined_symbol(
            symbols, f"dev_addition_{index}_END", f"additions[{index}] end"
        ).value
        if end <= start:
            raise DevElfError(f"additions[{index}] linked span is empty or reversed")
        if "new_vaddr" in addition and _number(
            addition["new_vaddr"], f"additions[{index}].new_vaddr"
        ) != start:
            raise DevElfError(
                f"additions[{index}] linked start does not match descriptor"
            )
        if "size" in addition and _number(
            addition["size"], f"additions[{index}].size"
        ) != end - start:
            raise DevElfError(f"additions[{index}] linked size does not match descriptor")
        addition["new_vaddr"] = start
        addition["size"] = end - start
    for replacement_index, replacement in enumerate(
        resolved.get("replacements", [])
    ):
        for section_index, entry in enumerate(
            replacement.get("retained_sections", [])
        ):
            marker = (
                f"dev_replacement_{replacement_index}_retained_{section_index}"
            )
            start = _unique_defined_symbol(
                symbols, f"{marker}_START", f"{marker} start"
            ).value
            end = _unique_defined_symbol(
                symbols, f"{marker}_END", f"{marker} end"
            ).value
            expected = _number(
                entry.get("size"),
                f"replacements[{replacement_index}].retained_sections"
                f"[{section_index}].size",
            )
            if end - start != expected:
                raise DevElfError(
                    f"replacements[{replacement_index}].retained_sections"
                    f"[{section_index}] linked size is 0x{end - start:X}, "
                    f"expected 0x{expected:X}"
                )
    return resolved, symbols


def _replacement_symbol_map(
    retail_symbol_elf: bytes,
    development_symbols: list[LinkedSymbol],
    ranges: list[tuple[int, int, int, int]],
    replacement_moves: set[int],
) -> dict[str, tuple[LinkedSymbol, LinkedSymbol]]:
    """Pair replacement-owned exported definitions by stable symbol identity."""

    retail_symbols = parse_linked_symbols(retail_symbol_elf)
    old_ranges = [ranges[index][:2] for index in sorted(replacement_moves)]
    candidates = [
        symbol
        for symbol in retail_symbols
        if symbol.name
        and symbol.size
        and symbol.section_index != SHN_UNDEF
        and symbol.binding in (STB_GLOBAL, STB_WEAK)
        and symbol.kind in (STT_OBJECT, STT_FUNC)
        and any(start <= symbol.value < end for start, end in old_ranges)
    ]
    if not candidates:
        raise DevElfError("replacement ranges contain no exported retail symbols")
    result: dict[str, tuple[LinkedSymbol, LinkedSymbol]] = {}
    for old_symbol in candidates:
        move_index = _range_index_for_address(old_symbol.value, ranges)
        if (
            move_index is None
            or move_index not in replacement_moves
            or old_symbol.value + old_symbol.size > ranges[move_index][1]
        ):
            raise DevElfError(
                f"replacement retail symbol {old_symbol.name} is not wholly "
                "contained in its logical move"
            )
        if old_symbol.name in result:
            raise DevElfError(
                f"replacement retail symbol {old_symbol.name} is ambiguous"
            )
        matching = [
            symbol
            for symbol in development_symbols
            if symbol.name == old_symbol.name
            and symbol.size
            and symbol.section_index != SHN_UNDEF
            and symbol.binding == old_symbol.binding
            and symbol.kind == old_symbol.kind
            and (symbol.other & 3) == (old_symbol.other & 3)
        ]
        if len(matching) != 1:
            raise DevElfError(
                f"replacement symbol {old_symbol.name} has {len(matching)} "
                "development definitions, expected one"
            )
        new_symbol = matching[0]
        if not (
            ranges[move_index][2] <= new_symbol.value
            and new_symbol.value + new_symbol.size <= ranges[move_index][3]
        ):
            raise DevElfError(
                f"replacement symbol {old_symbol.name} is not wholly contained "
                "in its logical move"
            )
        result[old_symbol.name] = (old_symbol, new_symbol)
    return result


def _replacement_target_matches(
    symbol_name: str,
    old_target: int,
    new_target: int,
    symbols: dict[str, tuple[LinkedSymbol, LinkedSymbol]],
) -> bool:
    pair = symbols.get(symbol_name)
    if pair is None:
        return False
    old_symbol, new_symbol = pair
    addend = old_target - old_symbol.value
    return (
        0 <= addend < old_symbol.size
        and addend < new_symbol.size
        and new_target == new_symbol.value + addend
    )


def _replacement_symbol_shifted_within_move(
    old_symbol: LinkedSymbol,
    new_symbol: LinkedSymbol,
    ranges: list[tuple[int, int, int, int]],
    replacement_moves: set[int],
) -> bool:
    old_index = _range_index_for_address(old_symbol.value, ranges)
    new_index = _range_index_for_address(new_symbol.value, ranges, new=True)
    if (
        old_index is None
        or new_index is None
        or old_index != new_index
        or old_index not in replacement_moves
    ):
        raise DevElfError(
            f"replacement symbol {old_symbol.name} does not stay in its logical move"
        )
    old_start, _, new_start, _ = ranges[old_index]
    return old_symbol.value - old_start != new_symbol.value - new_start


def _map_replacement_target(
    old_target: int,
    symbols: dict[str, tuple[LinkedSymbol, LinkedSymbol]],
    ranges: list[tuple[int, int, int, int]],
) -> int | None:
    """Map an old target by containing symbol, then by a same-size move."""

    matching = []
    for old_symbol, new_symbol in symbols.values():
        addend = old_target - old_symbol.value
        if 0 <= addend < old_symbol.size and addend < new_symbol.size:
            matching.append(new_symbol.value + addend)
    if len(matching) > 1:
        raise DevElfError(
            f"old replacement target 0x{old_target:X} is contained by multiple symbols"
        )
    if matching:
        return matching[0]
    return _mapped_move_address(old_target, ranges)


def _replacement_target_follows(
    relocation: LinkedRelocation,
    old_target: int,
    new_target: int,
    symbols: dict[str, tuple[LinkedSymbol, LinkedSymbol]],
    ranges: list[tuple[int, int, int, int]],
    replacement_moves: set[int],
) -> bool:
    if relocation.symbol_name in symbols:
        return _replacement_target_matches(
            relocation.symbol_name, old_target, new_target, symbols
        )
    move_index = _range_index_for_address(old_target, ranges)
    if move_index in replacement_moves:
        return (
            relocation.symbol_kind == STT_SECTION
            and _map_replacement_target(old_target, symbols, ranges) == new_target
        )
    return _mapped_move_address(old_target, ranges) == new_target


def repair_stale_replacement_relocations(
    output: bytes,
    relocation_elf: bytes,
    spec: dict[str, Any],
    retail_symbol_elf: bytes,
) -> tuple[bytes, int]:
    """Repair section-symbol REL addends that the linker cannot retarget."""

    resolved, development_symbols = _resolve_linked_layout(spec, relocation_elf)
    ranges = _move_ranges(resolved)
    replacement_moves = _replacement_move_indexes(resolved)
    if not replacement_moves:
        return output, 0
    symbols = _replacement_symbol_map(
        retail_symbol_elf,
        development_symbols,
        ranges,
        replacement_moves,
    )
    _, programs = parse_elf(output)
    relocations = parse_linked_relocations(relocation_elf)
    development_gp, _ = _matched_gp_values(
        relocations, development_symbols, retail_symbol_elf
    )
    original = bytes(output)
    effective_targets = _relocation_effective_targets(
        original, programs, relocations, development_gp
    )
    patches: dict[int, int] = {}

    def stage_patch(file_offset: int, word: int, context: str) -> None:
        prior = patches.get(file_offset)
        if prior is not None and prior != word:
            raise DevElfError(f"{context} requires conflicting relocation repairs")
        patches[file_offset] = word

    for relocation in relocations:
        if relocation.kind not in (2, 4):
            continue
        stale_targets = [
            target
            for target in effective_targets.get(relocation, ())
            if _address_in_move(target, ranges)
        ]
        if not stale_targets:
            continue
        if relocation.symbol_kind != STT_SECTION:
            continue
        if len(stale_targets) != 1:
            raise DevElfError(
                f"relocation at 0x{relocation.offset:X} has ambiguous stale targets"
            )
        old_target = stale_targets[0]
        new_target = _map_replacement_target(old_target, symbols, ranges)
        if new_target is None:
            raise DevElfError(
                f"relocation at 0x{relocation.offset:X} targets unmapped "
                f"replacement address 0x{old_target:X}"
            )
        file_offset = _file_offset_for_vaddr(programs, relocation.offset)
        if file_offset is None:
            raise DevElfError(
                f"relocation repair site 0x{relocation.offset:X} is not file-backed"
            )
        word = _unpack_word(original, file_offset, "replacement relocation repair")
        if relocation.kind == 2:
            if word != old_target:
                raise DevElfError(
                    f"R_MIPS_32 repair at 0x{relocation.offset:X} has "
                    "an inconsistent addend"
                )
            new_word = new_target
        else:
            if word >> 26 not in (2, 3) or _jump_target(
                word, relocation.offset
            ) != old_target:
                raise DevElfError(
                    f"R_MIPS_26 repair at 0x{relocation.offset:X} has "
                    "an inconsistent encoding"
                )
            if new_target & 3 or (
                (relocation.offset + 4) & 0xF0000000
            ) != (new_target & 0xF0000000):
                raise DevElfError(
                    f"R_MIPS_26 repair at 0x{relocation.offset:X} cannot encode "
                    f"0x{new_target:X}"
                )
            new_word = (word & 0xFC000000) | ((new_target >> 2) & 0x03FFFFFF)
        stage_patch(file_offset, new_word, f"relocation at 0x{relocation.offset:X}")

    for hi, lo, old_target in _hi16_lo16_pairs(
        original, programs, relocations
    ):
        if not _address_in_move(old_target, ranges):
            continue
        if hi.symbol_kind != STT_SECTION or lo.symbol_kind != STT_SECTION:
            continue
        new_target = _map_replacement_target(old_target, symbols, ranges)
        if new_target is None:
            raise DevElfError(
                f"HI16/LO16 repair at 0x{hi.offset:X}/0x{lo.offset:X} "
                f"targets unmapped replacement address 0x{old_target:X}"
            )
        hi_file = _file_offset_for_vaddr(programs, hi.offset)
        lo_file = _file_offset_for_vaddr(programs, lo.offset)
        if hi_file is None or lo_file is None:
            raise DevElfError("HI16/LO16 repair site is not file-backed")
        hi_word = _unpack_word(original, hi_file, "HI16 repair")
        lo_word = _unpack_word(original, lo_file, "LO16 repair")
        if lo_word >> 26 == 0x0D:
            high = (new_target >> 16) & 0xFFFF
        else:
            high = ((new_target + 0x8000) >> 16) & 0xFFFF
        stage_patch(
            hi_file,
            (hi_word & 0xFFFF0000) | high,
            f"HI16 relocation at 0x{hi.offset:X}",
        )
        stage_patch(
            lo_file,
            (lo_word & 0xFFFF0000) | (new_target & 0xFFFF),
            f"LO16 relocation at 0x{lo.offset:X}",
        )

    repaired = bytearray(output)
    for file_offset, word in patches.items():
        struct.pack_into("<I", repaired, file_offset, word)
    repaired_targets = _relocation_effective_targets(
        bytes(repaired), programs, relocations, development_gp
    )
    stale = [
        (relocation.offset, target)
        for relocation, targets in repaired_targets.items()
        for target in targets
        if _address_in_move(target, ranges)
    ]
    if stale:
        offset, target = min(stale)
        raise DevElfError(
            f"relocation repair left 0x{offset:X} targeting abandoned "
            f"address 0x{target:X}"
        )
    return bytes(repaired), len(patches)


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


def _decode_mips_relocation_address(
    image: bytes | bytearray, hi_offset: int, lo_offset: int
) -> int:
    """Decode a linked HI16/LO16 pair, including load/store LO16 sites."""

    hi_word = _unpack_word(image, hi_offset, "HI16 relocation")
    lo_word = _unpack_word(image, lo_offset, "LO16 relocation")
    if hi_word >> 26 != 0x0F:
        raise DevElfError(f"word at 0x{hi_offset:X} is not LUI")
    register = (hi_word >> 16) & 0x1F
    if (lo_word >> 21) & 0x1F != register:
        raise DevElfError("LO16 relocation does not read the LUI destination register")
    high = (hi_word & 0xFFFF) << 16
    low = lo_word & 0xFFFF
    if lo_word >> 26 != 0x0D and low & 0x8000:
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


def _range_storage(
    programs: list[ProgramHeader], start: int, end: int
) -> str | None:
    """Classify a nonempty range within one PT_LOAD as file, nobits, or mixed."""

    if end <= start:
        raise DevElfError("cannot classify an empty or reversed range")
    matches = []
    for program in programs:
        if program.kind != PT_LOAD:
            continue
        memory_end = program.vaddr + program.memory_size
        if program.vaddr <= start and end <= memory_end:
            file_end = program.vaddr + program.file_size
            if end <= file_end:
                matches.append("file")
            elif start >= file_end:
                matches.append("nobits")
            else:
                matches.append("mixed")
    if len(matches) > 1:
        raise DevElfError(
            f"range 0x{start:X}..0x{end:X} is covered by multiple PT_LOAD segments"
        )
    return matches[0] if matches else None


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


def _hi16_lo16_pairs(
    image: bytes,
    programs: list[ProgramHeader],
    relocations: list[LinkedRelocation],
) -> list[tuple[LinkedRelocation, LinkedRelocation, int]]:
    """Pair HI16/LO16 relocations using the live LUI destination register."""

    active_hi: dict[
        int, tuple[tuple[int, int], LinkedRelocation]
    ] = {}
    pairs = []
    for relocation in sorted(relocations, key=lambda entry: entry.offset):
        file_offset = _file_offset_for_vaddr(programs, relocation.offset)
        if relocation.kind == 5:
            if file_offset is None:
                continue
            word = _unpack_word(image, file_offset, "HI16 relocation")
            if word >> 26 != 0x0F:
                raise DevElfError(
                    f"HI16 relocation at 0x{relocation.offset:X} is not LUI"
                )
            register = (word >> 16) & 0x1F
            active_hi[register] = (
                (relocation.symbol_table_index, relocation.symbol_index),
                relocation,
            )
            continue
        if relocation.kind == 6:
            if file_offset is None:
                continue
            low_word = _unpack_word(image, file_offset, "LO16 relocation")
            register = (low_word >> 21) & 0x1F
            active = active_hi.get(register)
            symbol_key = (relocation.symbol_table_index, relocation.symbol_index)
            if active is None or active[0] != symbol_key:
                continue
            hi = active[1]
            hi_file = _file_offset_for_vaddr(programs, hi.offset)
            if hi_file is None:
                continue
            try:
                target = _decode_mips_relocation_address(
                    image, hi_file, file_offset
                )
            except DevElfError:
                continue
            # Keep the HI active so one LUI may feed multiple low-half users.
            pairs.append((hi, relocation, target))
    return pairs


def _relocation_effective_targets(
    image: bytes,
    programs: list[ProgramHeader],
    relocations: list[LinkedRelocation],
    gp: int | None = None,
) -> dict[LinkedRelocation, set[int]]:
    """Resolve addends encoded at REL sites into their final linked targets."""

    targets: dict[LinkedRelocation, set[int]] = {}
    for hi, lo, target in _hi16_lo16_pairs(image, programs, relocations):
        targets.setdefault(hi, set()).add(target)
        targets.setdefault(lo, set()).add(target)
    for relocation in relocations:
        file_offset = _file_offset_for_vaddr(programs, relocation.offset)
        if relocation.kind in (R_MIPS_HI16, R_MIPS_LO16):
            continue
        if file_offset is None:
            continue
        word = _unpack_word(image, file_offset, "relocation target")
        if relocation.kind == R_MIPS_26 and word >> 26 in (2, 3):
            targets.setdefault(relocation, set()).add(
                _jump_target(word, relocation.offset)
            )
        elif relocation.kind == R_MIPS_32:
            targets.setdefault(relocation, set()).add(word)
        elif relocation.kind == R_MIPS_GPREL16:
            if gp is None:
                raise DevElfError("R_MIPS_GPREL16 relocation requires a defined _gp")
            addend = word & 0xFFFF
            if addend & 0x8000:
                addend -= 0x10000
            targets.setdefault(relocation, set()).add((gp + addend) & 0xFFFFFFFF)
    return targets


def _move_ranges(spec: dict[str, Any]) -> list[tuple[int, int, int, int]]:
    ranges: list[tuple[int, int, int, int]] = []
    for index, move in enumerate(spec.get("moves", [])):
        old = _number(move.get("old_vaddr"), f"moves[{index}].old_vaddr")
        new = _number(move.get("new_vaddr"), f"moves[{index}].new_vaddr")
        old_size = _move_old_size(move, index)
        new_size = _move_new_size(move, index)
        old_end = _range_end(old, old_size, f"moves[{index}] old range")
        new_end = _range_end(new, new_size, f"moves[{index}] new range")
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
        if old_end - old_start != new_end - new_start:
            continue
        source_start, source_end = (new_start, new_end) if reverse else (old_start, old_end)
        target_start = old_start if reverse else new_start
        if source_start <= address < source_end:
            return target_start + address - source_start
    return None


def _address_in_move(
    address: int,
    ranges: list[tuple[int, int, int, int]],
    *,
    new: bool = False,
) -> bool:
    for old_start, old_end, new_start, new_end in ranges:
        start, end = (new_start, new_end) if new else (old_start, old_end)
        if start <= address < end:
            return True
    return False


def _replacement_move_indexes(spec: dict[str, Any]) -> set[int]:
    objects = {
        replacement.get("retail_object")
        for replacement in spec.get("replacements", [])
        if isinstance(replacement, dict)
    }
    return {
        index
        for index, move in enumerate(spec.get("moves", []))
        if isinstance(move, dict) and move.get("object") in objects
    }


def _range_index_for_address(
    address: int,
    ranges: list[tuple[int, int, int, int]],
    *,
    new: bool = False,
) -> int | None:
    for index, (old_start, old_end, new_start, new_end) in enumerate(ranges):
        start, end = (new_start, new_end) if new else (old_start, old_end)
        if start <= address < end:
            return index
    return None


def _audit_moved_payloads(
    base: bytes,
    output: bytes,
    base_programs: list[ProgramHeader],
    output_programs: list[ProgramHeader],
    relocations: list[LinkedRelocation],
    effective_targets: dict[LinkedRelocation, set[int]],
    ranges: list[tuple[int, int, int, int]],
    replacement_moves: set[int],
    retail_gp: int | None,
    moves: list[dict[str, Any]],
) -> int:
    """Require each moved byte to equal retail, except validated relocations."""

    old_relocations: list[LinkedRelocation] = []
    old_for_new: dict[LinkedRelocation, LinkedRelocation] = {}
    relocations_by_offset: dict[int, list[LinkedRelocation]] = {}
    for relocation in relocations:
        relocations_by_offset.setdefault(relocation.offset, []).append(relocation)
        move_index = _range_index_for_address(
            relocation.offset, ranges, new=True
        )
        if move_index in replacement_moves:
            continue
        old_offset = _mapped_move_address(relocation.offset, ranges, reverse=True)
        if old_offset is None:
            continue
        old_symbol_value = _mapped_move_address(
            relocation.symbol_value, ranges, reverse=True
        )
        old_relocation = LinkedRelocation(
            old_offset,
            relocation.kind,
            relocation.symbol_table_index,
            relocation.symbol_index,
            relocation.symbol_value if old_symbol_value is None else old_symbol_value,
            relocation.symbol_name,
            relocation.symbol_info,
            relocation.symbol_section_index,
        )
        old_relocations.append(old_relocation)
        old_for_new[relocation] = old_relocation
    old_targets = _relocation_effective_targets(
        base, base_programs, old_relocations, retail_gp
    )

    changed_words = 0
    for index, (old_start, old_end, new_start, new_end) in enumerate(ranges):
        if index in replacement_moves or _move_storage(moves[index], index) == "nobits":
            continue
        size = old_end - old_start
        if new_end - new_start != size:
            raise DevElfError(f"moves[{index}] is not a same-size relocation")
        old_file = _file_offset_for_vaddr(base_programs, old_start)
        new_file = _file_offset_for_vaddr(output_programs, new_start)
        if (
            old_file is None
            or new_file is None
            or _file_offset_for_vaddr(base_programs, old_end - 1) is None
            or _file_offset_for_vaddr(output_programs, new_end - 1) is None
        ):
            raise DevElfError(f"moves[{index}] payload is not wholly file-backed")

        differing_words = {
            relative & ~3
            for relative, (old_byte, new_byte) in enumerate(
                zip(
                    base[old_file : old_file + size],
                    output[new_file : new_file + size],
                )
            )
            if old_byte != new_byte
        }
        for relative in sorted(differing_words):
            if relative + 4 > size:
                raise DevElfError(
                    f"moves[{index}] has a changed partial word at +0x{relative:X}"
                )
            new_vaddr = new_start + relative
            old_word = _unpack_word(base, old_file + relative, "moved retail word")
            new_word = _unpack_word(output, new_file + relative, "moved output word")
            valid = False
            for relocation in relocations_by_offset.get(new_vaddr, []):
                old_relocation = old_for_new.get(relocation)
                if old_relocation is None or relocation.kind not in (2, 4, 5, 6):
                    continue
                if relocation.kind == 4:
                    encoding_valid = (
                        old_word >> 26 in (2, 3)
                        and new_word >> 26 == old_word >> 26
                    )
                elif relocation.kind in (5, 6):
                    encoding_valid = (
                        old_word & 0xFFFF0000
                    ) == (
                        new_word & 0xFFFF0000
                    )
                else:
                    encoding_valid = True
                if not encoding_valid:
                    continue
                if any(
                    _mapped_move_address(old_target, ranges) == new_target
                    for old_target in old_targets.get(old_relocation, ())
                    for new_target in effective_targets.get(relocation, ())
                ):
                    valid = True
                    break
            if not valid:
                raise DevElfError(
                    f"moves[{index}] word at +0x{relative:X} differs from retail "
                    "without a validated relocation"
                )
            changed_words += 1
    return changed_words


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
            if _address_in_move(word, ranges):
                raise DevElfError(
                    f"stale absolute word at 0x{vaddr:X} points into an abandoned move slot"
                )
            opcode = word >> 26
            if opcode in (2, 3):
                target = _jump_target(word, vaddr)
                if _address_in_move(target, ranges):
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
                if low_opcode == 0x0F and (low_word >> 16) & 0x1F == register:
                    break
                if (
                    low_opcode not in ADDRESS_LOW_OPCODES
                    or (low_word >> 21) & 0x1F != register
                ):
                    continue
                address = _decode_mips_relocation_address(
                    image, file_offset, low_offset
                )
                if _address_in_move(address, ranges):
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
    retail_symbol_elf: bytes | None = None,
) -> dict[str, int]:
    """Reject every prefix change not explained by a declared move or patch."""

    spec, development_symbols = _resolve_linked_layout(spec, relocation_elf)
    _, base_programs = parse_elf(base)
    output_header, output_programs = parse_elf(output)
    relocations = parse_linked_relocations(relocation_elf)
    ranges = _move_ranges(spec)
    if not ranges:
        raise DevElfError("relocation closure requires at least one moved range")
    replacement_moves = _replacement_move_indexes(spec)
    replacement_symbols: dict[str, tuple[LinkedSymbol, LinkedSymbol]] = {}
    if replacement_moves:
        if retail_symbol_elf is None:
            raise DevElfError("replacement audit requires the retail symbol ELF")
        replacement_symbols = _replacement_symbol_map(
            retail_symbol_elf,
            development_symbols,
            ranges,
            replacement_moves,
        )
    internally_shifted_symbols = {
        name
        for name, (old_symbol, new_symbol) in replacement_symbols.items()
        if _replacement_symbol_shifted_within_move(
            old_symbol,
            new_symbol,
            ranges,
            replacement_moves,
        )
    }
    moves = spec.get("moves", [])
    for replacement_index, replacement in enumerate(
        spec.get("replacements", [])
    ):
        retail_object = replacement.get("retail_object")
        object_move_indexes = {
            index
            for index, move in enumerate(moves)
            if move.get("object") == retail_object
        }
        object_symbols = {
            name
            for name, (old_symbol, _) in replacement_symbols.items()
            if _range_index_for_address(old_symbol.value, ranges)
            in object_move_indexes
        }
        object_shifted = object_symbols & internally_shifted_symbols
        for field, actual in (
            ("expected_symbols", len(object_symbols)),
            ("expected_shifted_symbols", len(object_shifted)),
        ):
            if len(spec.get("replacements", [])) > 1 and field not in replacement:
                raise DevElfError(
                    f"replacements[{replacement_index}] requires {field}"
                )
            if field in replacement:
                expected = _number(
                    replacement[field], f"replacements[{replacement_index}].{field}"
                )
                if expected != actual:
                    raise DevElfError(
                        f"replacements[{replacement_index}].{field} is {actual}, "
                        f"expected {expected}"
                    )

    allowed_bytes: set[int] = set(range(44, 46))
    old_header, _ = parse_elf(base)
    new_phdr_slot = old_header.phoff + old_header.phnum * old_header.phentsize
    allowed_bytes.update(range(new_phdr_slot, new_phdr_slot + PROGRAM_HEADER.size))

    for index, (old_start, old_end, _, _) in enumerate(ranges):
        storage = _move_storage(moves[index], index)
        actual_storage = _range_storage(base_programs, old_start, old_end)
        if actual_storage != storage:
            raise DevElfError(
                f"moves[{index}] old range has {actual_storage or 'unmapped'} storage, "
                f"expected {storage}"
            )
        if storage == "nobits":
            continue
        slot_offset = _file_offset_for_vaddr(base_programs, old_start)
        if slot_offset is None:
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
    redirects: dict[str, tuple[int, int]] = {}
    for index, redirect in enumerate(spec.get("redirects", [])):
        symbol = redirect.get("symbol")
        wrapper = redirect.get("wrapper")
        if not isinstance(symbol, str) or not symbol:
            raise DevElfError(f"redirects[{index}].symbol is not a symbol name")
        if not isinstance(wrapper, str) or not wrapper:
            raise DevElfError(f"redirects[{index}].wrapper is not a symbol name")
        if wrapper != f"__wrap_{symbol}":
            raise DevElfError(
                f"redirects[{index}].wrapper must be __wrap_{symbol}"
            )
        if wrapper in redirects:
            raise DevElfError(f"redirect wrapper {wrapper} is declared more than once")
        redirects[wrapper] = (
            _number(redirect.get("old_vaddr"), f"redirects[{index}].old_vaddr"),
            _number(
                redirect.get("expected_relocations"),
                f"redirects[{index}].expected_relocations",
            ),
        )

    relocations_by_offset: dict[int, list[LinkedRelocation]] = {}
    redirect_relocations: list[LinkedRelocation] = []
    for relocation in relocations:
        relocations_by_offset.setdefault(relocation.offset, []).append(relocation)
        if relocation.symbol_name in redirects:
            redirect_relocations.append(relocation)
    development_gp, retail_gp = _matched_gp_values(
        relocations, development_symbols, retail_symbol_elf
    )
    effective_targets = _relocation_effective_targets(
        output, output_programs, relocations, development_gp
    )
    retail_effective_targets = _relocation_effective_targets(
        base, base_programs, relocations, retail_gp
    )
    changed_payload_words = _audit_moved_payloads(
        base,
        output,
        base_programs,
        output_programs,
        relocations,
        effective_targets,
        ranges,
        replacement_moves,
        retail_gp,
        moves,
    )
    stale_relocations = [
        (relocation, target)
        for relocation, targets in effective_targets.items()
        for target in targets
        if _address_in_move(target, ranges)
    ]
    if stale_relocations:
        relocation, target = min(
            stale_relocations, key=lambda item: (item[0].offset, item[1])
        )
        raise DevElfError(
            f"relocation at 0x{relocation.offset:X} still targets abandoned "
            f"move address 0x{target:X}"
        )
    moved_relocations = [
        relocation
        for relocation in relocations
        if any(
            _address_in_move(target, ranges, new=True)
            for target in effective_targets.get(relocation, ())
        )
    ]
    moved_relocation_set = set(moved_relocations)
    shifted_symbol_relocations = sum(
        relocation.symbol_name in internally_shifted_symbols
        and _range_index_for_address(relocation.offset, ranges, new=True) is None
        and any(
            _address_in_move(target, ranges, new=True)
            for target in effective_targets.get(relocation, ())
        )
        for relocation in relocations
    )
    cross_replacement_relocations = 0
    for relocation in relocations:
        source_index = _range_index_for_address(
            relocation.offset, ranges, new=True
        )
        if source_index not in replacement_moves:
            continue
        source_object = moves[source_index].get("object")
        crosses = False
        for target in effective_targets.get(relocation, ()):
            target_index = _range_index_for_address(target, ranges, new=True)
            if (
                target_index in replacement_moves
                and moves[target_index].get("object") != source_object
            ):
                crosses = True
                break
        cross_replacement_relocations += crosses
    if len(spec.get("replacements", [])) > 1 and (
        "expected_cross_replacement_relocations" not in spec
    ):
        raise DevElfError(
            "multiple replacements require expected_cross_replacement_relocations"
        )
    if "expected_cross_replacement_relocations" in spec:
        expected_cross = _number(
            spec["expected_cross_replacement_relocations"],
            "expected_cross_replacement_relocations",
        )
        if expected_cross != cross_replacement_relocations:
            raise DevElfError(
                "expected_cross_replacement_relocations is "
                f"{cross_replacement_relocations}, expected {expected_cross}"
            )
    replacement_counts = {
        "expected_replacement_symbols": len(replacement_symbols),
        "expected_shifted_replacement_symbols": len(internally_shifted_symbols),
        "expected_shifted_symbol_relocations": shifted_symbol_relocations,
    }
    for field, actual in replacement_counts.items():
        if replacement_moves and field not in spec:
            raise DevElfError(f"replacement descriptor requires {field}")
        if field in spec and _number(spec[field], field) != actual:
            raise DevElfError(
                f"{field} is {actual}, expected {_number(spec[field], field)}"
            )

    explained_words: set[int] = set()
    hi_lo_words: set[int] = set()
    for word_offset in sorted(unexplained_words):
        vaddr = _vaddr_for_file_offset(base_programs, word_offset)
        if vaddr is None:
            raise DevElfError(f"unexplained change at non-loaded file offset 0x{word_offset:X}")
        candidates = [
            relocation
            for relocation in relocations_by_offset.get(vaddr, [])
            if relocation in moved_relocation_set or relocation.symbol_name in redirects
        ]
        if not candidates:
            raise DevElfError(
                f"change at file offset 0x{word_offset:X} / vaddr 0x{vaddr:X} "
                "has no relocation for a declared move or redirect"
            )
        old_word = _unpack_word(base, word_offset, "closure old word")
        new_word = _unpack_word(output, word_offset, "closure new word")
        matched: LinkedRelocation | None = None
        for relocation in candidates:
            redirect = redirects.get(relocation.symbol_name)
            if (
                relocation.kind == 4
                and old_word >> 26 in (2, 3)
                and new_word >> 26 == old_word >> 26
            ):
                old_target = _jump_target(old_word, vaddr)
                new_target = _jump_target(new_word, vaddr)
                if redirect is not None:
                    valid = (
                        old_target == redirect[0]
                        and new_target == relocation.symbol_value
                    )
                else:
                    valid = _replacement_target_follows(
                        relocation,
                        old_target,
                        new_target,
                        replacement_symbols,
                        ranges,
                        replacement_moves,
                    )
            elif relocation.kind == 2:
                valid = _replacement_target_follows(
                    relocation,
                    old_word,
                    new_word,
                    replacement_symbols,
                    ranges,
                    replacement_moves,
                )
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
                f"relocated word at file offset 0x{word_offset:X} does not match "
                "its declared move or redirect"
            )
        explained_words.add(word_offset)
        if matched.kind in (5, 6):
            hi_lo_words.add(word_offset)

    checked_hi_lo_words: set[int] = set()
    checked_pairs = 0
    for relocation in moved_relocations:
        if relocation.kind not in (5, 6):
            continue
        file_offset = _file_offset_for_vaddr(base_programs, relocation.offset)
        if file_offset is None or file_offset not in hi_lo_words:
            continue
        valid = any(
            _replacement_target_follows(
                relocation,
                old_target,
                new_target,
                replacement_symbols,
                ranges,
                replacement_moves,
            )
            for old_target in retail_effective_targets.get(relocation, ())
            for new_target in effective_targets.get(relocation, ())
        )
        if not valid:
            raise DevElfError(
                f"HI16/LO16 relocation at 0x{relocation.offset:X} does not "
                "follow its declared move"
            )
        checked_hi_lo_words.add(file_offset)
        if relocation.kind == 6:
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

    for wrapper, (_, expected) in redirects.items():
        matching = [
            relocation
            for relocation in redirect_relocations
            if relocation.symbol_name == wrapper
        ]
        if len(matching) != expected:
            raise DevElfError(
                f"redirect wrapper {wrapper} has {len(matching)} relocations, "
                f"expected {expected}"
            )
        if any(relocation.kind != 4 for relocation in matching):
            raise DevElfError(f"redirect wrapper {wrapper} has a non-R_MIPS_26 relocation")

    extension_relocations = 0
    target_relocations = 0
    for index, move in enumerate(spec.get("moves", [])):
        start = _number(move.get("new_vaddr"), f"moves[{index}].new_vaddr")
        size = _move_new_size(move, index)
        actual = sum(start <= relocation.offset < start + size for relocation in relocations)
        expected = _number(
            move.get("expected_relocations"), f"moves[{index}].expected_relocations"
        )
        if actual != expected:
            raise DevElfError(
                f"moves[{index}] retained {actual} relocation entries, expected {expected}"
            )
        extension_relocations += actual
        targeted = sum(
            any(start <= target < start + size for target in targets)
            for targets in effective_targets.values()
        )
        if _move_storage(move, index) == "nobits" and (
            "expected_target_relocations" not in move
        ):
            raise DevElfError(
                f"moves[{index}] nobits move requires expected_target_relocations"
            )
        if "expected_target_relocations" in move:
            expected_targets = _number(
                move["expected_target_relocations"],
                f"moves[{index}].expected_target_relocations",
            )
            if targeted != expected_targets:
                raise DevElfError(
                    f"moves[{index}] has {targeted} target relocation entries, "
                    f"expected {expected_targets}"
                )
        target_relocations += targeted

    addition_relocations = 0
    addition_ranges: list[tuple[int, int]] = []
    for index, addition in enumerate(spec.get("additions", [])):
        start = _number(addition.get("new_vaddr"), f"additions[{index}].new_vaddr")
        size = _number(addition.get("size"), f"additions[{index}].size")
        if size == 0:
            raise DevElfError(f"additions[{index}].size is zero")
        end = _range_end(start, size, f"additions[{index}] range")
        if any(
            start < other_end and other_start < end
            for other_start, other_end in addition_ranges
        ):
            raise DevElfError(f"additions[{index}] overlaps another addition")
        if any(start < new_end and new_start < end for _, _, new_start, new_end in ranges):
            raise DevElfError(f"additions[{index}] overlaps a moved range")
        addition_ranges.append((start, end))
        actual = sum(start <= relocation.offset < end for relocation in relocations)
        expected = _number(
            addition.get("expected_relocations"),
            f"additions[{index}].expected_relocations",
        )
        if actual != expected:
            raise DevElfError(
                f"additions[{index}] retained {actual} relocation entries, "
                f"expected {expected}"
            )
        addition_relocations += actual

    extension = output_programs[-1]
    if extension.kind != PT_LOAD or extension.vaddr != _number(
        spec.get("extension_vaddr"), "extension_vaddr"
    ):
        raise DevElfError("final PT_LOAD is not the declared development extension")
    if output_header.phnum != old_header.phnum + 1:
        raise DevElfError("development ELF did not gain exactly one program header")
    extension_file_end = extension.vaddr + extension.file_size
    extension_memory_end = extension.vaddr + extension.memory_size
    for index, (_, _, new_start, new_end) in enumerate(ranges):
        storage = _move_storage(moves[index], index)
        if storage == "file":
            in_payload = (
                extension.vaddr <= new_start
                and new_end <= extension_file_end
            )
        else:
            in_payload = (
                extension_file_end <= new_start
                and new_end <= extension_memory_end
            )
        if not in_payload:
            raise DevElfError(
                f"moves[{index}] new range is outside the development {storage} payload"
            )
    for index, (start, end) in enumerate(addition_ranges):
        if start < extension.vaddr or end > extension_file_end:
            raise DevElfError(
                f"additions[{index}] is outside the development payload"
            )
    for relocation in redirect_relocations:
        if not extension.vaddr <= relocation.symbol_value < extension_file_end:
            raise DevElfError(
                f"redirect wrapper {relocation.symbol_name} is outside "
                "the development payload"
            )
    scanned_words = scan_stale_move_references(output, ranges)
    return {
        "changed_relocation_words": len(explained_words),
        "changed_payload_words": changed_payload_words,
        "moved_relocations": len(moved_relocations),
        "redirect_relocations": len(redirect_relocations),
        "extension_relocations": extension_relocations,
        "target_relocations": target_relocations,
        "addition_relocations": addition_relocations,
        "checked_hi_lo_pairs": checked_pairs,
        "replacement_symbols": len(replacement_symbols),
        "shifted_replacement_symbols": len(internally_shifted_symbols),
        "shifted_symbol_relocations": shifted_symbol_relocations,
        "cross_replacement_relocations": cross_replacement_relocations,
        "stale_reference_words_scanned": scanned_words,
    }


def render_linker_script(base: str, spec: dict[str, Any]) -> str:
    moves = spec.get("moves")
    if not isinstance(moves, list) or not moves:
        raise DevElfError("development descriptor must contain at least one section move")
    for index, move in enumerate(moves):
        if not isinstance(move, dict):
            raise DevElfError(f"moves[{index}] is not an object")
        if not isinstance(move.get("object"), str) or not isinstance(
            move.get("section"), str
        ):
            raise DevElfError(f"moves[{index}] needs string object and section fields")

    replacements = spec.get("replacements", [])
    if not isinstance(replacements, list):
        raise DevElfError("replacements must be a list")
    replacement_by_retail: dict[str, tuple[int, dict[str, Any]]] = {}
    for index, replacement in enumerate(replacements):
        if not isinstance(replacement, dict):
            raise DevElfError(f"replacements[{index}] is not an object")
        retail_obj = replacement.get("retail_object")
        obj = replacement.get("object")
        if not isinstance(retail_obj, str) or not isinstance(obj, str):
            raise DevElfError(
                f"replacements[{index}] needs string retail_object and object fields"
            )
        if retail_obj in replacement_by_retail:
            raise DevElfError(f"replacement for {retail_obj} is declared more than once")
        replacement_by_retail[retail_obj] = (index, replacement)

    for retail_obj, (replacement_index, replacement) in replacement_by_retail.items():
        moved_sections = [
            move.get("section") for move in moves if move.get("object") == retail_obj
        ]
        retained = replacement.get("retained_sections")
        if not moved_sections:
            raise DevElfError(
                f"replacements[{replacement_index}] has no declared section move"
            )
        if not isinstance(retained, list):
            raise DevElfError(
                f"replacements[{replacement_index}].retained_sections must be a list"
            )
        retained_sections = []
        for section_index, entry in enumerate(retained):
            if not isinstance(entry, dict) or not isinstance(entry.get("section"), str):
                raise DevElfError(
                    f"replacements[{replacement_index}].retained_sections"
                    f"[{section_index}] needs a section"
                )
            if _number(
                entry.get("size"),
                f"replacements[{replacement_index}].retained_sections"
                f"[{section_index}].size",
            ) != 0:
                raise DevElfError(
                    f"replacements[{replacement_index}].retained_sections"
                    f"[{section_index}] must remain empty"
                )
            retained_sections.append(entry["section"])
        expected_sections = moved_sections + retained_sections
        actual_sections = []
        prefix = f"{retail_obj}("
        for line in base.splitlines():
            stripped = line.strip()
            if stripped.startswith(prefix) and stripped.endswith(");"):
                actual_sections.append(stripped[len(prefix) : -2])
        if sorted(actual_sections) != sorted(expected_sections):
            raise DevElfError(
                f"replacements[{replacement_index}] accounts for sections "
                f"{sorted(expected_sections)!r}, linker script contains "
                f"{sorted(actual_sections)!r}"
            )

    text = base
    file_extension_lines: list[str] = []
    nobits_extension_lines: list[str] = []
    # The pinned linker rejects ASSERT commands inside an output section.
    move_assertion_lines: list[str] = []
    for index, move in enumerate(moves):
        obj = move.get("object")
        section = move.get("section")
        old_size = _move_old_size(move, index)
        expected_new_size = move.get("new_size", move.get("size"))
        alignment = _number(move.get("alignment", 1), f"moves[{index}].alignment")
        _align_up(0, alignment)
        needle = f"{obj}({section});"
        matching = [line for line in text.splitlines() if line.strip() == needle]
        if len(matching) != 1:
            raise DevElfError(
                f"expected one linker-script placement for {needle}, found {len(matching)}"
            )
        original = matching[0]
        indent = original[: len(original) - len(original.lstrip())]
        slot = "\n".join(
            [
                f"{indent}. = ALIGN(0x{alignment:X});",
                f"{indent}. += 0x{old_size:X}; "
                f"/* development slot for {obj}({section}) */",
            ]
        )
        text = text.replace(original, slot, 1)
        linked_obj = replacement_by_retail.get(obj, (0, {}))[1].get("object", obj)
        storage = _move_storage(move, index)
        extension_lines = (
            nobits_extension_lines if storage == "nobits" else file_extension_lines
        )
        extension_lines.append(f"        . = ALIGN(0x{alignment:X});")
        extension_lines.append(f"        dev_move_{index}_START = .;")
        extension_lines.append(f"        {linked_obj}({section});")
        extension_lines.append(f"        dev_move_{index}_END = .;")
        if expected_new_size is not None:
            new_size = _number(expected_new_size, f"moves[{index}].new_size")
            move_assertion_lines.extend(
                [
                    f"    ASSERT(dev_move_{index}_END - dev_move_{index}_START "
                    f"== 0x{new_size:X},",
                    f'           "development move {index} has unexpected size")',
                ]
            )

    for retail_obj, (replacement_index, replacement) in replacement_by_retail.items():
        linked_obj = replacement["object"]
        for section_index, entry in enumerate(replacement["retained_sections"]):
            section = entry["section"]
            needle = f"{retail_obj}({section});"
            matching = [line for line in text.splitlines() if line.strip() == needle]
            if len(matching) != 1:
                raise DevElfError(
                    f"expected one linker-script placement for {needle}, "
                    f"found {len(matching)}"
                )
            original = matching[0]
            indent = original[: len(original) - len(original.lstrip())]
            marker = f"dev_replacement_{replacement_index}_retained_{section_index}"
            retained_lines = "\n".join(
                [
                    f"{indent}{marker}_START = .;",
                    f"{indent}{linked_obj}({section});",
                    f"{indent}{marker}_END = .;",
                ]
            )
            text = text.replace(original, retained_lines, 1)

    for index, addition in enumerate(spec.get("additions", [])):
        if not isinstance(addition, dict):
            raise DevElfError(f"additions[{index}] is not an object")
        obj = addition.get("object")
        sections = addition.get("sections")
        if not isinstance(obj, str) or not isinstance(sections, list) or not sections:
            raise DevElfError(
                f"additions[{index}] needs an object and a nonempty sections list"
            )
        alignment = _number(
            addition.get("alignment", 8), f"additions[{index}].alignment"
        )
        _align_up(0, alignment)
        file_extension_lines.append(f"        . = ALIGN(0x{alignment:X});")
        file_extension_lines.append(f"        dev_addition_{index}_START = .;")
        for section_index, section in enumerate(sections):
            if not isinstance(section, str) or not section.startswith("."):
                raise DevElfError(
                    f"additions[{index}].sections[{section_index}] is invalid"
                )
            file_extension_lines.append(f"        {obj}({section});")
        file_extension_lines.append(f"        dev_addition_{index}_END = .;")

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
            "        dev_extension_VRAM_START = .;",
            "        dev_extension_TEXT_START = .;",
            *file_extension_lines,
            "        dev_extension_TEXT_END = .;",
            "    }",
            "    dev_extension_FILE_END = ADDR(.dev_extension) + SIZEOF(.dev_extension);",
            "    __romPos += SIZEOF(.dev_extension);",
            "    dev_extension_ROM_END = __romPos;",
            "    .dev_extension_nobits (NOLOAD) :",
            "    {",
            *nobits_extension_lines,
            "    }",
            "    dev_extension_VRAM_END = .;",
            *move_assertion_lines,
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
    base: bytes,
    linked: bytes,
    spec: dict[str, Any],
    *,
    linked_file_size: int | None = None,
    linked_memory_size: int | None = None,
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
        memory_size=linked_memory_size,
    )
    if linked_file_size is not None and program.file_size != linked_file_size:
        raise DevElfError(
            f"development payload is 0x{program.file_size:X} bytes, "
            f"but linker symbols describe 0x{linked_file_size:X}"
        )
    if "extension_size" in spec:
        expected_size = _number(spec["extension_size"], "extension_size")
        if program.file_size != expected_size:
            raise DevElfError(
                f"development payload is 0x{program.file_size:X} bytes, "
                f"expected 0x{expected_size:X}"
            )
    if "extension_memory_size" in spec:
        expected_memory_size = _number(
            spec["extension_memory_size"], "extension_memory_size"
        )
        if program.memory_size != expected_memory_size:
            raise DevElfError(
                f"development memory span is 0x{program.memory_size:X} bytes, "
                f"expected 0x{expected_memory_size:X}"
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
    finalize.add_argument(
        "--retail-symbol-elf",
        type=Path,
        help="ordinary retail wrapper ELF containing replacement symbol addresses",
    )

    args = parser.parse_args(argv)
    try:
        spec = _read_descriptor(args.descriptor)
        if args.command == "link-script":
            _write(args.output, render_linker_script(args.input.read_text(), spec))
        else:
            retail_image = args.retail.read_bytes()
            linked_image = args.linked.read_bytes()
            relocation_elf = args.reloc_elf.read_bytes()
            retail_symbol_elf = (
                args.retail_symbol_elf.read_bytes()
                if args.retail_symbol_elf is not None
                else None
            )
            object_summary = audit_replacement_objects(
                spec,
                [
                    Path(replacement["object"]).read_bytes()
                    for replacement in spec.get("replacements", [])
                ],
                [
                    Path(replacement["retail_object"]).read_bytes()
                    for replacement in spec.get("replacements", [])
                ],
            )
            move_paths = _effective_move_object_paths(spec)
            object_summary.update(
                audit_moved_sections(
                    spec,
                    {path: Path(path).read_bytes() for path in set(move_paths)},
                )
            )
            linked_file_size, linked_memory_size = _linked_extension_sizes(
                spec, relocation_elf
            )
            output, summary = finalize_image(
                retail_image,
                linked_image,
                spec,
                linked_file_size=linked_file_size,
                linked_memory_size=linked_memory_size,
            )
            if args.retail_symbol_elf is not None:
                output, repaired_words = repair_stale_replacement_relocations(
                    output,
                    relocation_elf,
                    spec,
                    retail_symbol_elf,
                )
            else:
                repaired_words = 0
            if repaired_words and "expected_repaired_relocation_words" not in spec:
                raise DevElfError(
                    "replacement relocation repairs require "
                    "expected_repaired_relocation_words"
                )
            if "expected_repaired_relocation_words" in spec:
                expected_repairs = _number(
                    spec["expected_repaired_relocation_words"],
                    "expected_repaired_relocation_words",
                )
                if expected_repairs != repaired_words:
                    raise DevElfError(
                        f"repaired relocation words are {repaired_words}, "
                        f"expected {expected_repairs}"
                    )
            summary.update(object_summary)
            summary["repaired_relocation_words"] = repaired_words
            summary.update(
                audit_relocation_closure(
                    retail_image,
                    output,
                    relocation_elf,
                    spec,
                    retail_symbol_elf,
                )
            )
            _write(args.output, output)
            print(json.dumps(summary, sort_keys=True))
    except (DevElfError, OSError) as exc:
        parser.exit(1, f"dev_elf: {exc}\n")
    return 0


if __name__ == "__main__":
    sys.exit(main())
