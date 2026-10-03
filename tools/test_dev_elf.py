#!/usr/bin/env python3
"""Regression tests for development ELF construction."""

from __future__ import annotations

import hashlib
import struct
import sys
import unittest
from pathlib import Path


TOOLS = Path(__file__).resolve().parent
if str(TOOLS) not in sys.path:
    sys.path.insert(0, str(TOOLS))

import dev_elf  # noqa: E402


def _address_words(address: int, register: int = 4) -> tuple[int, int]:
    high = ((address + 0x8000) >> 16) & 0xFFFF
    return (
        (0x0F << 26) | (register << 16) | high,
        (0x09 << 26) | (register << 21) | (register << 16) | (address & 0xFFFF),
    )


def _elf(payload_size: int = 0x100) -> bytes:
    image = bytearray(0x1000 + payload_size)
    ident = b"\x7fELF" + bytes((1, 1, 1)) + bytes(9)
    dev_elf.ELF_HEADER.pack_into(
        image,
        0,
        ident,
        2,
        dev_elf.EM_MIPS,
        1,
        0x100008,
        dev_elf.ELF_HEADER.size,
        0,
        0,
        dev_elf.ELF_HEADER.size,
        dev_elf.PROGRAM_HEADER.size,
        1,
        40,
        0,
        0,
    )
    dev_elf.PROGRAM_HEADER.pack_into(
        image,
        dev_elf.ELF_HEADER.size,
        dev_elf.PT_LOAD,
        0x1000,
        0x100000,
        0x100000,
        payload_size,
        0x200,
        dev_elf.PF_R | dev_elf.PF_X,
        0x1000,
    )
    return bytes(image)


def _relocation_elf(
    entries: list[tuple[int, int, int, str]],
) -> bytes:
    symbol_offset = 0x100
    symbols = list(
        dict.fromkeys((symbol_vaddr, name) for _, symbol_vaddr, _, name in entries)
    )
    symbol_indexes = {symbol: index for index, symbol in enumerate(symbols, start=1)}
    symbol_size = (len(symbols) + 1) * dev_elf.SYMBOL_ENTRY.size
    relocation_offset = (symbol_offset + symbol_size + 0xF) & ~0xF
    relocation_size = len(entries) * dev_elf.REL_ENTRY.size
    string_offset = (relocation_offset + relocation_size + 0xF) & ~0xF
    strings = bytearray(b"\0")
    name_offsets = []
    for _, name in symbols:
        name_offsets.append(len(strings))
        strings.extend(name.encode("ascii") + b"\0")
    section_offset = (string_offset + len(strings) + 0xF) & ~0xF
    image = bytearray(section_offset + 4 * dev_elf.SECTION_HEADER.size)
    ident = b"\x7fELF" + bytes((1, 1, 1)) + bytes(9)
    dev_elf.ELF_HEADER.pack_into(
        image,
        0,
        ident,
        2,
        dev_elf.EM_MIPS,
        1,
        0,
        dev_elf.ELF_HEADER.size,
        section_offset,
        0,
        dev_elf.ELF_HEADER.size,
        dev_elf.PROGRAM_HEADER.size,
        0,
        dev_elf.SECTION_HEADER.size,
        4,
        0,
    )
    for index, ((symbol_vaddr, _), name_offset) in enumerate(
        zip(symbols, name_offsets), start=1
    ):
        dev_elf.SYMBOL_ENTRY.pack_into(
            image,
            symbol_offset + index * dev_elf.SYMBOL_ENTRY.size,
            name_offset,
            symbol_vaddr,
            0,
            0,
            0,
            0,
        )
    for relocation_index, (reloc_vaddr, symbol_vaddr, kind, name) in enumerate(
        entries
    ):
        symbol_index = symbol_indexes[(symbol_vaddr, name)]
        dev_elf.REL_ENTRY.pack_into(
            image,
            relocation_offset + relocation_index * dev_elf.REL_ENTRY.size,
            reloc_vaddr,
            (symbol_index << 8) | kind,
        )
    image[string_offset : string_offset + len(strings)] = strings
    dev_elf.SECTION_HEADER.pack_into(
        image,
        section_offset + dev_elf.SECTION_HEADER.size,
        0,
        dev_elf.SHT_SYMTAB,
        0,
        0,
        symbol_offset,
        symbol_size,
        3,
        0,
        4,
        dev_elf.SYMBOL_ENTRY.size,
    )
    dev_elf.SECTION_HEADER.pack_into(
        image,
        section_offset + 2 * dev_elf.SECTION_HEADER.size,
        0,
        dev_elf.SHT_REL,
        0,
        0,
        relocation_offset,
        relocation_size,
        1,
        0,
        4,
        dev_elf.REL_ENTRY.size,
    )
    dev_elf.SECTION_HEADER.pack_into(
        image,
        section_offset + 3 * dev_elf.SECTION_HEADER.size,
        0,
        dev_elf.SHT_STRTAB,
        0,
        0,
        string_offset,
        len(strings),
        0,
        0,
        1,
        0,
    )
    return bytes(image)


def _metadata_elf(
    symbols: list[tuple[str, int, int, int, int]],
    relocations: list[tuple[int, int, str]],
) -> bytes:
    symbol_offset = 0x100
    symbol_size = (len(symbols) + 1) * dev_elf.SYMBOL_ENTRY.size
    relocation_offset = (symbol_offset + symbol_size + 0xF) & ~0xF
    relocation_size = len(relocations) * dev_elf.REL_ENTRY.size
    string_offset = (relocation_offset + relocation_size + 0xF) & ~0xF
    strings = bytearray(b"\0")
    name_offsets = []
    for name, _, _, _, _ in symbols:
        name_offsets.append(len(strings))
        strings.extend(name.encode("ascii") + b"\0")
    section_offset = (string_offset + len(strings) + 0xF) & ~0xF
    image = bytearray(section_offset + 4 * dev_elf.SECTION_HEADER.size)
    ident = b"\x7fELF" + bytes((1, 1, 1)) + bytes(9)
    dev_elf.ELF_HEADER.pack_into(
        image,
        0,
        ident,
        2,
        dev_elf.EM_MIPS,
        1,
        0,
        dev_elf.ELF_HEADER.size,
        section_offset,
        0,
        dev_elf.ELF_HEADER.size,
        dev_elf.PROGRAM_HEADER.size,
        0,
        dev_elf.SECTION_HEADER.size,
        4,
        0,
    )
    indexes: dict[str, int] = {}
    for index, ((name, value, size, info, section), name_offset) in enumerate(
        zip(symbols, name_offsets), start=1
    ):
        indexes.setdefault(name, index)
        dev_elf.SYMBOL_ENTRY.pack_into(
            image,
            symbol_offset + index * dev_elf.SYMBOL_ENTRY.size,
            name_offset,
            value,
            size,
            info,
            0,
            section,
        )
    for index, (offset, kind, name) in enumerate(relocations):
        dev_elf.REL_ENTRY.pack_into(
            image,
            relocation_offset + index * dev_elf.REL_ENTRY.size,
            offset,
            (indexes[name] << 8) | kind,
        )
    image[string_offset : string_offset + len(strings)] = strings
    dev_elf.SECTION_HEADER.pack_into(
        image,
        section_offset + dev_elf.SECTION_HEADER.size,
        0,
        dev_elf.SHT_SYMTAB,
        0,
        0,
        symbol_offset,
        symbol_size,
        3,
        0,
        4,
        dev_elf.SYMBOL_ENTRY.size,
    )
    dev_elf.SECTION_HEADER.pack_into(
        image,
        section_offset + 2 * dev_elf.SECTION_HEADER.size,
        0,
        dev_elf.SHT_REL,
        0,
        0,
        relocation_offset,
        relocation_size,
        1,
        0,
        4,
        dev_elf.REL_ENTRY.size,
    )
    dev_elf.SECTION_HEADER.pack_into(
        image,
        section_offset + 3 * dev_elf.SECTION_HEADER.size,
        0,
        dev_elf.SHT_STRTAB,
        0,
        0,
        string_offset,
        len(strings),
        0,
        0,
        1,
        0,
    )
    return bytes(image)


def _allocated_object(sections: list[tuple[str, int, int]]) -> bytes:
    """Create a minimal relocatable ELF with named allocated sections."""

    names = bytearray(b"\0.shstrtab\0")
    name_offsets = {".shstrtab": 1}
    for name, _, _ in sections:
        name_offsets[name] = len(names)
        names.extend(name.encode("ascii") + b"\0")
    data_offset = 0x100
    section_offsets = []
    cursor = data_offset
    for _, size, section_type in sections:
        section_offsets.append(cursor)
        if section_type != dev_elf.SHT_NOBITS:
            cursor += size
    string_offset = cursor
    cursor += len(names)
    section_offset = (cursor + 0xF) & ~0xF
    image = bytearray(
        section_offset + (len(sections) + 2) * dev_elf.SECTION_HEADER.size
    )
    ident = b"\x7fELF" + bytes((1, 1, 1)) + bytes(9)
    dev_elf.ELF_HEADER.pack_into(
        image,
        0,
        ident,
        1,
        dev_elf.EM_MIPS,
        1,
        0,
        dev_elf.ELF_HEADER.size,
        section_offset,
        0,
        dev_elf.ELF_HEADER.size,
        dev_elf.PROGRAM_HEADER.size,
        0,
        dev_elf.SECTION_HEADER.size,
        len(sections) + 2,
        1,
    )
    image[string_offset : string_offset + len(names)] = names
    dev_elf.SECTION_HEADER.pack_into(
        image,
        section_offset + dev_elf.SECTION_HEADER.size,
        name_offsets[".shstrtab"],
        dev_elf.SHT_STRTAB,
        0,
        0,
        string_offset,
        len(names),
        0,
        0,
        1,
        0,
    )
    for index, ((name, size, section_type), offset) in enumerate(
        zip(sections, section_offsets), start=2
    ):
        dev_elf.SECTION_HEADER.pack_into(
            image,
            section_offset + index * dev_elf.SECTION_HEADER.size,
            name_offsets[name],
            section_type,
            dev_elf.SHF_ALLOC,
            0,
            offset,
            size,
            0,
            0,
            4,
            0,
        )
    return bytes(image)


def _fallback_object(
    body: bytes,
    *,
    symbol_value: int = 0,
    relocation_name: str = "target",
    target_value: int | None = None,
    target_size: int = 4,
    common_symbol: bool = False,
) -> bytes:
    """Create a relocatable object with one fallback and one text relocation."""

    section_names = b"\0.shstrtab\0.text\0.rel.text\0.symtab\0.strtab\0"
    section_name_offsets = {
        name: section_names.index(name.encode("ascii"))
        for name in (".shstrtab", ".text", ".rel.text", ".symtab", ".strtab")
    }
    strings = b"\0fallback\0" + relocation_name.encode("ascii") + b"\0common\0"
    fallback_name = strings.index(b"fallback")
    target_name = strings.index(relocation_name.encode("ascii"))
    common_name = strings.index(b"common")
    text = bytes(symbol_value) + body
    text_offset = 0x100
    relocation_offset = (text_offset + len(text) + 3) & ~3
    symbol_offset = relocation_offset + dev_elf.REL_ENTRY.size
    symbol_count = 4 if common_symbol else 3
    symbol_size = symbol_count * dev_elf.SYMBOL_ENTRY.size
    string_offset = symbol_offset + symbol_size
    shstr_offset = string_offset + len(strings)
    section_offset = (shstr_offset + len(section_names) + 0xF) & ~0xF
    image = bytearray(section_offset + 6 * dev_elf.SECTION_HEADER.size)
    ident = b"\x7fELF" + bytes((1, 1, 1)) + bytes(9)
    dev_elf.ELF_HEADER.pack_into(
        image,
        0,
        ident,
        1,
        dev_elf.EM_MIPS,
        1,
        0,
        0,
        section_offset,
        0,
        dev_elf.ELF_HEADER.size,
        0,
        0,
        dev_elf.SECTION_HEADER.size,
        6,
        1,
    )
    image[text_offset : text_offset + len(text)] = text
    dev_elf.REL_ENTRY.pack_into(
        image,
        relocation_offset,
        symbol_value + 4,
        (2 << 8) | 4,
    )
    global_func = (dev_elf.STB_GLOBAL << 4) | dev_elf.STT_FUNC
    global_object = (dev_elf.STB_GLOBAL << 4) | dev_elf.STT_OBJECT
    dev_elf.SYMBOL_ENTRY.pack_into(
        image,
        symbol_offset + dev_elf.SYMBOL_ENTRY.size,
        fallback_name,
        symbol_value,
        len(body),
        global_func,
        0,
        2,
    )
    dev_elf.SYMBOL_ENTRY.pack_into(
        image,
        symbol_offset + 2 * dev_elf.SYMBOL_ENTRY.size,
        target_name,
        0 if target_value is None else target_value,
        0 if target_value is None else target_size,
        global_func,
        0,
        dev_elf.SHN_UNDEF if target_value is None else 2,
    )
    if common_symbol:
        dev_elf.SYMBOL_ENTRY.pack_into(
            image,
            symbol_offset + 3 * dev_elf.SYMBOL_ENTRY.size,
            common_name,
            4,
            4,
            global_object,
            0,
            dev_elf.SHN_COMMON,
        )
    image[string_offset : string_offset + len(strings)] = strings
    image[shstr_offset : shstr_offset + len(section_names)] = section_names
    headers = [
        (
            section_name_offsets[".shstrtab"],
            dev_elf.SHT_STRTAB,
            0,
            shstr_offset,
            len(section_names),
            0,
            0,
            1,
            0,
        ),
        (
            section_name_offsets[".text"],
            1,
            dev_elf.SHF_ALLOC,
            text_offset,
            len(text),
            0,
            0,
            4,
            0,
        ),
        (
            section_name_offsets[".rel.text"],
            dev_elf.SHT_REL,
            0,
            relocation_offset,
            dev_elf.REL_ENTRY.size,
            4,
            2,
            4,
            dev_elf.REL_ENTRY.size,
        ),
        (
            section_name_offsets[".symtab"],
            dev_elf.SHT_SYMTAB,
            0,
            symbol_offset,
            symbol_size,
            5,
            1,
            4,
            dev_elf.SYMBOL_ENTRY.size,
        ),
        (
            section_name_offsets[".strtab"],
            dev_elf.SHT_STRTAB,
            0,
            string_offset,
            len(strings),
            0,
            0,
            1,
            0,
        ),
    ]
    for index, (name, kind, flags, offset, size, link, info, align, entsize) in enumerate(
        headers, start=1
    ):
        dev_elf.SECTION_HEADER.pack_into(
            image,
            section_offset + index * dev_elf.SECTION_HEADER.size,
            name,
            kind,
            flags,
            0,
            offset,
            size,
            link,
            info,
            align,
            entsize,
        )
    return bytes(image)


def _retained_object(payload: bytes) -> bytes:
    """Create an object with one sized global object in a retained .data section."""

    image = bytearray(_fallback_object(payload))
    section_name = image.find(b".text\0")
    if section_name < 0:
        raise AssertionError("test object has no .text section name")
    image[section_name : section_name + 6] = b".data\0"
    sections = dev_elf._object_sections(image)
    relocation_section = next(
        section for section in sections if section.kind == dev_elf.SHT_REL
    )
    header = dev_elf.ElfHeader(*dev_elf.ELF_HEADER.unpack_from(image))
    struct.pack_into(
        "<I",
        image,
        header.shoff + relocation_section.index * header.shentsize + 8,
        dev_elf.SHF_ALLOC,
    )
    sections = dev_elf._object_sections(image)
    table = next(section for section in sections if section.kind == dev_elf.SHT_SYMTAB)
    fields = list(
        dev_elf.SYMBOL_ENTRY.unpack_from(
            image, table.offset + dev_elf.SYMBOL_ENTRY.size
        )
    )
    fields[3] = (dev_elf.STB_GLOBAL << 4) | dev_elf.STT_OBJECT
    dev_elf.SYMBOL_ENTRY.pack_into(
        image, table.offset + dev_elf.SYMBOL_ENTRY.size, *fields
    )
    return bytes(image)


class DevElfTests(unittest.TestCase):
    def test_relocation_closure_accepts_only_explained_prefix_changes(self) -> None:
        old_vaddr = 0x00100020
        new_vaddr = 0x00412000
        call_vaddr = 0x00100010
        original_entry = 0x00100080
        wrapper_vaddr = 0x00412004
        base = bytearray(_elf())
        struct.pack_into("<I", base, 0x1010, (3 << 26) | (old_vaddr >> 2))
        struct.pack_into("<I", base, 0x1014, (3 << 26) | (original_entry >> 2))
        struct.pack_into("<I", base, 0x1020, 0x03E00008)

        linked = bytearray(base)
        struct.pack_into("<I", linked, 0x1010, (3 << 26) | (new_vaddr >> 2))
        struct.pack_into("<I", linked, 0x1014, (3 << 26) | (wrapper_vaddr >> 2))
        struct.pack_into("<I", linked, 0x1020, 0)
        linked.extend(bytes((-len(linked)) & 0xFFF))
        linked.extend(struct.pack("<II", 0x03E00008, 0x03E00008))
        spec = {
            "format": 1,
            "base_sha1": hashlib.sha1(base).hexdigest(),
            "extension_vaddr": new_vaddr,
            "extension_size": 8,
            "retail_static_end": 0x0040C5F0,
            "moves": [
                {
                    "old_vaddr": old_vaddr,
                    "new_vaddr": new_vaddr,
                    "size": 4,
                    "expected_relocations": 0,
                }
            ],
            "additions": [
                {
                    "new_vaddr": wrapper_vaddr,
                    "size": 4,
                    "expected_relocations": 0,
                }
            ],
            "redirects": [
                {
                    "symbol": "entry",
                    "wrapper": "__wrap_entry",
                    "old_vaddr": original_entry,
                    "expected_relocations": 1,
                }
            ],
        }
        output, _ = dev_elf.finalize_image(bytes(base), bytes(linked), spec)
        relocation_elf = _relocation_elf(
            [
                (call_vaddr, new_vaddr, 4, "moved"),
                (call_vaddr + 4, wrapper_vaddr, 4, "__wrap_entry"),
            ]
        )

        with self.assertRaisesRegex(dev_elf.DevElfError, "expected 0x10"):
            dev_elf.finalize_image(
                bytes(base), bytes(linked), {**spec, "extension_size": 0x10}
            )

        summary = dev_elf.audit_relocation_closure(
            bytes(base), output, relocation_elf, spec
        )
        self.assertEqual(summary["changed_relocation_words"], 2)
        self.assertEqual(summary["changed_payload_words"], 0)
        self.assertEqual(summary["moved_relocations"], 1)
        self.assertEqual(summary["redirect_relocations"], 1)
        self.assertEqual(summary["addition_relocations"], 0)

        wrong_redirect = {
            **spec,
            "redirects": [
                {**spec["redirects"][0], "old_vaddr": original_entry + 4}
            ],
        }
        with self.assertRaisesRegex(dev_elf.DevElfError, "declared move or redirect"):
            dev_elf.audit_relocation_closure(
                bytes(base), output, relocation_elf, wrong_redirect
            )

        wrong_addition = {
            **spec,
            "additions": [
                {**spec["additions"][0], "expected_relocations": 1}
            ],
        }
        with self.assertRaisesRegex(dev_elf.DevElfError, "retained 0 relocation"):
            dev_elf.audit_relocation_closure(
                bytes(base), output, relocation_elf, wrong_addition
            )

        corrupted = bytearray(output)
        corrupted[0x1030] = 1
        with self.assertRaisesRegex(dev_elf.DevElfError, "has no relocation"):
            dev_elf.audit_relocation_closure(
                bytes(base), bytes(corrupted), relocation_elf, spec
            )

        corrupted = bytearray(output)
        struct.pack_into("<I", corrupted, 0x2000, 0)
        with self.assertRaisesRegex(dev_elf.DevElfError, "differs from retail"):
            dev_elf.audit_relocation_closure(
                bytes(base), bytes(corrupted), relocation_elf, spec
            )

    def test_linked_relocation_parser_resolves_symbol_values(self) -> None:
        relocations = dev_elf.parse_linked_relocations(
            _relocation_elf([(0x00100010, 0x00412000, 4, "target")])
        )
        self.assertEqual(
            relocations,
            [
                dev_elf.LinkedRelocation(
                    0x00100010, 4, 1, 1, 0x00412000, "target"
                )
            ],
        )

    def test_linked_layout_resolves_dynamic_addition_without_replacement(self) -> None:
        symbols = [
            ("dev_move_0_START", 0x00412000, 0, 0, 1),
            ("dev_move_0_END", 0x00412004, 0, 0, 1),
            ("dev_addition_0_START", 0x00412004, 0, 0, 1),
            ("dev_addition_0_END", 0x0041200C, 0, 0, 1),
            ("dev_addition_0_section_0_START", 0x00412004, 0, 0, 1),
            ("dev_addition_0_section_0_END", 0x0041200C, 0, 0, 1),
        ]
        spec = {
            "moves": [{"old_vaddr": 0x00100020, "old_size": 4}],
            "additions": [{"sections": [".text"]}],
        }
        resolved, _ = dev_elf._resolve_linked_layout(
            spec, _metadata_elf(symbols, [])
        )
        self.assertEqual(resolved["moves"][0]["new_vaddr"], 0x00412000)
        self.assertEqual(resolved["moves"][0]["new_size"], 4)
        self.assertEqual(resolved["additions"][0]["new_vaddr"], 0x00412004)
        self.assertEqual(resolved["additions"][0]["size"], 8)
        self.assertEqual(
            resolved["additions"][0]["_linked_sections"],
            [{"section": ".text", "start": 0x00412004, "end": 0x0041200C}],
        )

    def test_nonempty_retained_section_resolves_at_old_vaddr(self) -> None:
        old_vaddr = 0x00100080
        symbols = [
            ("dev_move_0_START", 0x00412000, 0, 0, 1),
            ("dev_move_0_END", 0x00412004, 0, 0, 1),
            ("dev_replacement_0_retained_0_START", old_vaddr, 0, 0, 1),
            ("dev_replacement_0_retained_0_END", old_vaddr + 8, 0, 0, 1),
        ]
        spec = {
            "replacements": [
                {
                    "retained_sections": [
                        {
                            "section": ".data",
                            "size": 8,
                            "old_vaddr": old_vaddr,
                            "alignment": 8,
                            "storage": "file",
                            "expected_symbols": 1,
                            "expected_gp_references": 0,
                        }
                    ]
                }
            ],
            "moves": [{"old_vaddr": 0x00100020, "old_size": 4}],
        }
        dev_elf._resolve_linked_layout(spec, _metadata_elf(symbols, []))

        moved = [*symbols]
        moved[2] = (moved[2][0], old_vaddr + 8, 0, 0, 1)
        moved[3] = (moved[3][0], old_vaddr + 16, 0, 0, 1)
        with self.assertRaisesRegex(dev_elf.DevElfError, "linked start"):
            dev_elf._resolve_linked_layout(spec, _metadata_elf(moved, []))

        resized = [*symbols]
        resized[3] = (resized[3][0], old_vaddr + 4, 0, 0, 1)
        with self.assertRaisesRegex(dev_elf.DevElfError, "linked size"):
            dev_elf._resolve_linked_layout(spec, _metadata_elf(resized, []))

    def test_linked_extension_sizes_require_ordered_boundary_symbols(self) -> None:
        spec = {"extension_vaddr": 0x00412000}
        symbols = [
            ("dev_extension_VRAM_START", 0x00412000, 0, 0, 1),
            ("dev_extension_FILE_END", 0x00412008, 0, 0, 1),
            ("dev_extension_VRAM_END", 0x00412048, 0, 0, 1),
        ]
        self.assertEqual(
            dev_elf._linked_extension_sizes(spec, _metadata_elf(symbols, [])),
            (8, 0x48),
        )
        reversed_symbols = [*symbols[:2], (symbols[2][0], 0x00412004, 0, 0, 1)]
        with self.assertRaisesRegex(dev_elf.DevElfError, "precedes its file end"):
            dev_elf._linked_extension_sizes(
                spec, _metadata_elf(reversed_symbols, [])
            )

    def test_range_storage_distinguishes_file_nobits_and_mixed(self) -> None:
        _, programs = dev_elf.parse_elf(_elf(payload_size=0x40))
        self.assertEqual(
            dev_elf._range_storage(programs, 0x00100000, 0x00100040), "file"
        )
        self.assertEqual(
            dev_elf._range_storage(programs, 0x00100040, 0x00100080), "nobits"
        )
        self.assertEqual(
            dev_elf._range_storage(programs, 0x00100020, 0x00100060), "mixed"
        )
        self.assertIsNone(
            dev_elf._range_storage(programs, 0x00100200, 0x00100240)
        )

    def test_stale_reference_scan_rejects_words_and_jumps(self) -> None:
        ranges = [(0x00100020, 0x00100040, 0x00412000, 0x00412024)]
        image = bytearray(_elf())
        struct.pack_into("<I", image, 0x1000, 0x00100024)
        with self.assertRaisesRegex(dev_elf.DevElfError, "stale absolute word"):
            dev_elf.scan_stale_move_references(bytes(image), ranges)

        struct.pack_into("<I", image, 0x1000, (3 << 26) | (0x00100028 >> 2))
        with self.assertRaisesRegex(dev_elf.DevElfError, "stale JAL"):
            dev_elf.scan_stale_move_references(bytes(image), ranges)

        hi, low = _address_words(0x0010002C)
        struct.pack_into("<I", image, 0x1000, hi)
        struct.pack_into(
            "<I", image, 0x1004, (0x23 << 26) | (4 << 21) | (low & 0xFFFF)
        )
        with self.assertRaisesRegex(dev_elf.DevElfError, "stale HI16/LO16"):
            dev_elf.scan_stale_move_references(bytes(image), ranges)

    def test_relocation_closure_rejects_stale_relocation_target(self) -> None:
        old_vaddr = 0x00100040
        new_vaddr = 0x00412000
        base = bytearray(_elf())
        struct.pack_into("<I", base, 0x1000, _address_words(old_vaddr)[0])
        struct.pack_into(
            "<I", base, 0x1004, (0x23 << 26) | (4 << 21) | (old_vaddr & 0xFFFF)
        )
        struct.pack_into("<I", base, 0x1040, 0x03E00008)
        linked = bytearray(base)
        struct.pack_into("<I", linked, 0x1040, 0)
        linked.extend(bytes((-len(linked)) & 0xFFF))
        linked.extend(struct.pack("<I", 0x03E00008))
        spec = {
            "format": 1,
            "base_sha1": hashlib.sha1(base).hexdigest(),
            "extension_vaddr": new_vaddr,
            "extension_size": 4,
            "retail_static_end": 0x0040C5F0,
            "moves": [
                {
                    "old_vaddr": old_vaddr,
                    "new_vaddr": new_vaddr,
                    "size": 4,
                    "expected_relocations": 0,
                }
            ],
        }
        output, _ = dev_elf.finalize_image(bytes(base), bytes(linked), spec)
        relocation_elf = _relocation_elf(
            [
                (0x00100000, old_vaddr, 5, "stale"),
                (0x00100004, old_vaddr, 6, "stale"),
            ]
        )
        with self.assertRaisesRegex(dev_elf.DevElfError, "still targets abandoned"):
            dev_elf.audit_relocation_closure(
                bytes(base), output, relocation_elf, spec
            )

    def test_relocation_closure_accepts_nobits_move_and_target_count(self) -> None:
        old_vaddr = 0x00100080
        extension_vaddr = 0x00412000
        new_vaddr = extension_vaddr + 8
        base = bytearray(_elf(payload_size=0x40))
        old_hi, old_lo = _address_words(old_vaddr)
        struct.pack_into("<I", base, 0x1000, old_hi)
        struct.pack_into("<I", base, 0x1004, old_lo)

        linked = bytearray(base)
        new_hi, new_lo = _address_words(new_vaddr)
        struct.pack_into("<I", linked, 0x1000, new_hi)
        struct.pack_into("<I", linked, 0x1004, new_lo)
        linked.extend(bytes((-len(linked)) & 0xFFF))
        linked.extend(bytes(8))
        spec = {
            "format": 1,
            "base_sha1": hashlib.sha1(base).hexdigest(),
            "extension_vaddr": extension_vaddr,
            "retail_static_end": 0x00100200,
            "moves": [
                {
                    "old_vaddr": old_vaddr,
                    "new_vaddr": new_vaddr,
                    "size": 0x40,
                    "storage": "nobits",
                    "expected_relocations": 0,
                    "expected_target_relocations": 2,
                }
            ],
        }
        output, _ = dev_elf.finalize_image(
            bytes(base),
            bytes(linked),
            spec,
            linked_file_size=8,
            linked_memory_size=0x48,
        )
        relocation_elf = _relocation_elf(
            [
                (0x00100000, new_vaddr, dev_elf.R_MIPS_HI16, "fileManagerWork"),
                (0x00100004, new_vaddr, dev_elf.R_MIPS_LO16, "fileManagerWork"),
            ]
        )
        summary = dev_elf.audit_relocation_closure(
            bytes(base), output, relocation_elf, spec
        )
        self.assertEqual(summary["target_relocations"], 2)
        self.assertEqual(summary["changed_payload_words"], 0)

        with self.assertRaisesRegex(dev_elf.DevElfError, "expected 3"):
            wrong_count = {
                **spec,
                "moves": [
                    {**spec["moves"][0], "expected_target_relocations": 3}
                ],
            }
            dev_elf.audit_relocation_closure(
                bytes(base), output, relocation_elf, wrong_count
            )

        with self.assertRaisesRegex(dev_elf.DevElfError, "old range has nobits"):
            wrong_storage = {
                **spec,
                "moves": [{**spec["moves"][0], "storage": "file"}],
            }
            dev_elf.audit_relocation_closure(
                bytes(base), output, relocation_elf, wrong_storage
            )

        bad_new_vaddr = extension_vaddr + 4
        bad_linked = bytearray(linked)
        bad_hi, bad_lo = _address_words(bad_new_vaddr)
        struct.pack_into("<I", bad_linked, 0x1000, bad_hi)
        struct.pack_into("<I", bad_linked, 0x1004, bad_lo)
        bad_spec = {
            **spec,
            "moves": [{**spec["moves"][0], "new_vaddr": bad_new_vaddr}],
        }
        bad_output, _ = dev_elf.finalize_image(
            bytes(base),
            bytes(bad_linked),
            bad_spec,
            linked_file_size=8,
            linked_memory_size=0x48,
        )
        bad_relocations = _relocation_elf(
            [
                (0x00100000, bad_new_vaddr, dev_elf.R_MIPS_HI16, "fileManagerWork"),
                (0x00100004, bad_new_vaddr, dev_elf.R_MIPS_LO16, "fileManagerWork"),
            ]
        )
        with self.assertRaisesRegex(
            dev_elf.DevElfError, "outside the development nobits payload"
        ):
            dev_elf.audit_relocation_closure(
                bytes(base), bad_output, bad_relocations, bad_spec
            )

    def test_moved_payload_allows_validated_relocation(self) -> None:
        old_vaddr = 0x00100040
        new_vaddr = 0x00412000
        base = bytearray(_elf())
        struct.pack_into("<I", base, 0x1040, old_vaddr)
        linked = bytearray(base)
        struct.pack_into("<I", linked, 0x1040, 0)
        linked.extend(bytes((-len(linked)) & 0xFFF))
        linked.extend(struct.pack("<I", new_vaddr))
        spec = {
            "format": 1,
            "base_sha1": hashlib.sha1(base).hexdigest(),
            "extension_vaddr": new_vaddr,
            "extension_size": 4,
            "retail_static_end": 0x0040C5F0,
            "moves": [
                {
                    "old_vaddr": old_vaddr,
                    "new_vaddr": new_vaddr,
                    "size": 4,
                    "expected_relocations": 1,
                }
            ],
        }
        output, _ = dev_elf.finalize_image(bytes(base), bytes(linked), spec)
        relocation_elf = _relocation_elf(
            [(new_vaddr, new_vaddr, 2, "moved_section")]
        )
        summary = dev_elf.audit_relocation_closure(
            bytes(base), output, relocation_elf, spec
        )
        self.assertEqual(summary["changed_payload_words"], 1)
        self.assertEqual(summary["moved_relocations"], 1)

    def test_effective_targets_keep_hi_active_for_multiple_lows(self) -> None:
        target = 0x00412020
        image = bytearray(_elf())
        hi, _ = _address_words(target)
        struct.pack_into("<I", image, 0x1000, hi)
        struct.pack_into(
            "<I", image, 0x1004, (0x23 << 26) | (4 << 21) | (target & 0xFFFF)
        )
        struct.pack_into(
            "<I", image, 0x1008, (0x2B << 26) | (4 << 21) | ((target + 4) & 0xFFFF)
        )
        relocations = [
            dev_elf.LinkedRelocation(0x00100000, 5, 1, 1, target, "shared"),
            dev_elf.LinkedRelocation(0x00100004, 6, 1, 1, target, "shared"),
            dev_elf.LinkedRelocation(0x00100008, 6, 1, 1, target, "shared"),
        ]
        _, programs = dev_elf.parse_elf(image)
        targets = dev_elf._relocation_effective_targets(
            bytes(image), programs, relocations
        )
        self.assertEqual(targets[relocations[1]], {target})
        self.assertEqual(targets[relocations[2]], {target + 4})

    def test_effective_targets_new_hi_supersedes_same_register(self) -> None:
        first_target = 0x00412020
        second_target = 0x0039F3D0
        image = bytearray(_elf())
        first_hi, first_lo = _address_words(first_target)
        second_hi, second_lo = _address_words(second_target)
        for offset, word in enumerate(
            (first_hi, first_lo, second_hi, second_lo), start=0
        ):
            struct.pack_into("<I", image, 0x1000 + offset * 4, word)
        relocations = [
            dev_elf.LinkedRelocation(0x00100000, 5, 1, 1, 0, ".main"),
            dev_elf.LinkedRelocation(0x00100004, 6, 1, 1, 0, ".main"),
            dev_elf.LinkedRelocation(0x00100008, 5, 1, 1, 0, ".main"),
            dev_elf.LinkedRelocation(0x0010000C, 6, 1, 1, 0, ".main"),
        ]
        _, programs = dev_elf.parse_elf(image)
        targets = dev_elf._relocation_effective_targets(
            bytes(image), programs, relocations
        )
        self.assertEqual(targets[relocations[0]], {first_target})
        self.assertEqual(targets[relocations[1]], {first_target})
        self.assertEqual(targets[relocations[2]], {second_target})
        self.assertEqual(targets[relocations[3]], {second_target})

    def test_effective_targets_resolve_signed_gprel16_addends(self) -> None:
        gp = 0x00108000
        image = bytearray(_elf())
        struct.pack_into("<I", image, 0x1000, (0x23 << 26) | (28 << 21) | 0x7FFF)
        struct.pack_into("<I", image, 0x1004, (0x23 << 26) | (28 << 21) | 0x8000)
        relocations = [
            dev_elf.LinkedRelocation(
                0x00100000, dev_elf.R_MIPS_GPREL16, 1, 1, 0, "positive"
            ),
            dev_elf.LinkedRelocation(
                0x00100004, dev_elf.R_MIPS_GPREL16, 1, 2, 0, "negative"
            ),
        ]
        _, programs = dev_elf.parse_elf(image)
        targets = dev_elf._relocation_effective_targets(
            bytes(image), programs, relocations, gp
        )
        self.assertEqual(targets[relocations[0]], {gp + 0x7FFF})
        self.assertEqual(targets[relocations[1]], {gp - 0x8000})
        with self.assertRaisesRegex(dev_elf.DevElfError, "requires a defined _gp"):
            dev_elf._relocation_effective_targets(
                bytes(image), programs, relocations
            )

    def test_relocation_closure_rejects_stale_gprel16_target(self) -> None:
        old_vaddr = 0x00100040
        new_vaddr = 0x00412000
        gp = 0x00108000
        base = bytearray(_elf())
        immediate = (old_vaddr - gp) & 0xFFFF
        struct.pack_into(
            "<I", base, 0x1000, (0x23 << 26) | (28 << 21) | immediate
        )
        struct.pack_into("<I", base, 0x1040, 0x03E00008)
        linked = bytearray(base)
        struct.pack_into("<I", linked, 0x1040, 0)
        linked.extend(bytes((-len(linked)) & 0xFFF))
        linked.extend(struct.pack("<I", 0x03E00008))
        spec = {
            "format": 1,
            "base_sha1": hashlib.sha1(base).hexdigest(),
            "extension_vaddr": new_vaddr,
            "extension_size": 4,
            "retail_static_end": 0x0040C5F0,
            "moves": [
                {
                    "old_vaddr": old_vaddr,
                    "new_vaddr": new_vaddr,
                    "size": 4,
                    "expected_relocations": 0,
                }
            ],
        }
        output, _ = dev_elf.finalize_image(bytes(base), bytes(linked), spec)
        relocation_elf = _metadata_elf(
            [
                ("stale", old_vaddr, 4, 0, 1),
                ("_gp", gp, 0, 0, 1),
            ],
            [(0x00100000, dev_elf.R_MIPS_GPREL16, "stale")],
        )
        retail_symbols = _metadata_elf([("_gp", gp, 0, 0, 1)], [])
        with self.assertRaisesRegex(
            dev_elf.DevElfError, "requires the retail symbol ELF"
        ):
            dev_elf.audit_relocation_closure(
                bytes(base), output, relocation_elf, spec
            )
        with self.assertRaisesRegex(dev_elf.DevElfError, "still targets abandoned"):
            dev_elf.audit_relocation_closure(
                bytes(base), output, relocation_elf, spec, retail_symbols
            )

        drifted_retail = _metadata_elf([("_gp", gp + 4, 0, 0, 1)], [])
        with self.assertRaisesRegex(dev_elf.DevElfError, "differs from retail"):
            dev_elf.audit_relocation_closure(
                bytes(base), output, relocation_elf, spec, drifted_retail
            )

        duplicate_development_gp = _metadata_elf(
            [
                ("stale", old_vaddr, 4, 0, 1),
                ("_gp", gp, 0, 0, 1),
                ("_gp", gp, 0, 0, 1),
            ],
            [(0x00100000, dev_elf.R_MIPS_GPREL16, "stale")],
        )
        with self.assertRaisesRegex(dev_elf.DevElfError, "2 definitions"):
            dev_elf.audit_relocation_closure(
                bytes(base), output, duplicate_development_gp, spec, retail_symbols
            )

    def test_relocation_closure_accepts_unchanged_gprel16_target(self) -> None:
        old_vaddr = 0x00100040
        new_vaddr = 0x00412000
        target = 0x00100100
        gp = 0x00108000
        base = bytearray(_elf())
        immediate = (target - gp) & 0xFFFF
        struct.pack_into(
            "<I", base, 0x1000, (0x23 << 26) | (28 << 21) | immediate
        )
        struct.pack_into("<I", base, 0x1040, 0x03E00008)
        linked = bytearray(base)
        struct.pack_into("<I", linked, 0x1040, 0)
        linked.extend(bytes((-len(linked)) & 0xFFF))
        linked.extend(struct.pack("<I", 0x03E00008))
        spec = {
            "format": 1,
            "base_sha1": hashlib.sha1(base).hexdigest(),
            "extension_vaddr": new_vaddr,
            "extension_size": 4,
            "retail_static_end": 0x0040C5F0,
            "moves": [
                {
                    "old_vaddr": old_vaddr,
                    "new_vaddr": new_vaddr,
                    "size": 4,
                    "expected_relocations": 0,
                }
            ],
        }
        output, _ = dev_elf.finalize_image(bytes(base), bytes(linked), spec)
        relocation_elf = _metadata_elf(
            [
                ("retained", target, 4, 0, 1),
                ("_gp", gp, 0, 0, 1),
            ],
            [(0x00100000, dev_elf.R_MIPS_GPREL16, "retained")],
        )
        retail_symbols = _metadata_elf([("_gp", gp, 0, 0, 1)], [])
        summary = dev_elf.audit_relocation_closure(
            bytes(base), output, relocation_elf, spec, retail_symbols
        )
        self.assertEqual(summary["moved_relocations"], 0)

    def test_named_replacement_target_cannot_use_containment_mapping(self) -> None:
        old_head = 0x00100040
        old_tail = 0x00100048
        new_head = 0x00412000
        new_tail = 0x00412010
        global_func = (dev_elf.STB_GLOBAL << 4) | dev_elf.STT_FUNC
        symbols = {
            "head": (
                dev_elf.LinkedSymbol(1, 1, "head", old_head, 8, global_func, 0, 1),
                dev_elf.LinkedSymbol(1, 1, "head", new_head, 8, global_func, 0, 1),
            ),
            "tail": (
                dev_elf.LinkedSymbol(1, 2, "tail", old_tail, 8, global_func, 0, 1),
                dev_elf.LinkedSymbol(1, 2, "tail", new_tail, 8, global_func, 0, 1),
            ),
        }
        ranges = [(old_head, old_tail + 8, new_head, new_tail + 8)]
        named = dev_elf.LinkedRelocation(
            0x00100000, 4, 1, 1, old_head, "head", global_func, 1
        )
        section = dev_elf.LinkedRelocation(
            0x00100000, 4, 1, 3, 0, ".main", dev_elf.STT_SECTION, 1
        )
        self.assertFalse(
            dev_elf._replacement_target_follows(
                named,
                old_tail + 4,
                new_tail + 4,
                symbols,
                ranges,
                {0},
            )
        )
        self.assertTrue(
            dev_elf._replacement_target_follows(
                named,
                old_head + 4,
                new_head + 4,
                symbols,
                ranges,
                {0},
            )
        )
        self.assertTrue(
            dev_elf._replacement_target_follows(
                section,
                old_tail + 4,
                new_tail + 4,
                symbols,
                ranges,
                {0},
            )
        )

    def test_relocation_closure_keeps_hi_active_for_multiple_lows(self) -> None:
        old_start = 0x00100040
        new_start = 0x00412000
        base = bytearray(_elf())
        old_hi, _ = _address_words(old_start)
        new_hi, _ = _address_words(new_start)
        struct.pack_into("<I", base, 0x1000, old_hi)
        struct.pack_into(
            "<I", base, 0x1004, (0x23 << 26) | (4 << 21) | (2 << 16) | 0x40
        )
        struct.pack_into(
            "<I", base, 0x1008, (0x2B << 26) | (4 << 21) | (3 << 16) | 0x44
        )
        base[0x1040:0x1048] = bytes.fromhex("0800E00300000000")

        linked = bytearray(base)
        struct.pack_into("<I", linked, 0x1000, new_hi)
        struct.pack_into(
            "<I", linked, 0x1004, (0x23 << 26) | (4 << 21) | (2 << 16) | 0x2000
        )
        struct.pack_into(
            "<I", linked, 0x1008, (0x2B << 26) | (4 << 21) | (3 << 16) | 0x2004
        )
        linked[0x1040:0x1048] = bytes(8)
        linked.extend(bytes((-len(linked)) & 0xFFF))
        linked.extend(bytes.fromhex("0800E00300000000"))
        spec = {
            "format": 1,
            "base_sha1": hashlib.sha1(base).hexdigest(),
            "extension_vaddr": new_start,
            "retail_static_end": 0x0040C5F0,
            "moves": [
                {
                    "old_vaddr": old_start,
                    "new_vaddr": new_start,
                    "size": 8,
                    "expected_relocations": 0,
                }
            ],
        }
        output, _ = dev_elf.finalize_image(bytes(base), bytes(linked), spec)
        relocation_elf = _relocation_elf(
            [
                (0x00100000, new_start, 5, "target"),
                (0x00100004, new_start, 6, "target"),
                (0x00100008, new_start, 6, "target"),
            ]
        )
        summary = dev_elf.audit_relocation_closure(
            bytes(base), output, relocation_elf, spec
        )
        self.assertEqual(summary["changed_relocation_words"], 3)
        self.assertEqual(summary["checked_hi_lo_pairs"], 2)

    def test_changed_size_replacement_maps_shifted_symbol_and_addend(self) -> None:
        old_start = 0x00100040
        old_tail = old_start + 8
        new_start = 0x00412000
        new_tail = new_start + 0xC
        base = bytearray(_elf())
        struct.pack_into("<I", base, 0x1000, (3 << 26) | (old_tail >> 2))
        struct.pack_into("<I", base, 0x1004, old_tail + 4)
        base[0x1040:0x1050] = bytes(range(1, 17))

        linked = bytearray(base)
        struct.pack_into("<I", linked, 0x1000, (3 << 26) | (new_tail >> 2))
        struct.pack_into("<I", linked, 0x1004, new_tail + 4)
        linked[0x1040:0x1050] = bytes(16)
        linked.extend(bytes((-len(linked)) & 0xFFF))
        linked.extend(bytes(range(0x14)))
        spec = {
            "format": 1,
            "base_sha1": hashlib.sha1(base).hexdigest(),
            "extension_vaddr": new_start,
            "retail_static_end": 0x0040C5F0,
            "expected_replacement_symbols": 2,
            "expected_shifted_replacement_symbols": 1,
            "expected_shifted_symbol_relocations": 2,
            "replacements": [
                {
                    "retail_object": "old.o",
                    "object": "new.o",
                    "retained_sections": [],
                }
            ],
            "moves": [
                {
                    "object": "old.o",
                    "section": ".text",
                    "old_vaddr": old_start,
                    "old_size": 0x10,
                    "expected_relocations": 0,
                }
            ],
        }
        output, _ = dev_elf.finalize_image(bytes(base), bytes(linked), spec)
        global_func = (dev_elf.STB_GLOBAL << 4) | 2
        global_notype = dev_elf.STB_GLOBAL << 4
        development_symbols = [
            ("dev_move_0_START", new_start, 0, global_notype, 1),
            ("dev_move_0_END", new_start + 0x14, 0, global_notype, 1),
            ("head", new_start, 0xC, global_func, 1),
            ("tail", new_tail, 8, global_func, 1),
        ]
        relocations = [
            (0x00100000, 4, "tail"),
            (0x00100004, 2, "tail"),
        ]
        relocation_elf = _metadata_elf(development_symbols, relocations)
        retail_symbol_elf = _metadata_elf(
            [
                ("head", old_start, 8, global_func, 1),
                ("tail", old_tail, 8, global_func, 1),
            ],
            [],
        )

        summary = dev_elf.audit_relocation_closure(
            bytes(base), output, relocation_elf, spec, retail_symbol_elf
        )
        self.assertEqual(summary["changed_relocation_words"], 2)
        self.assertEqual(summary["replacement_symbols"], 2)
        self.assertEqual(summary["shifted_replacement_symbols"], 1)
        self.assertEqual(summary["shifted_symbol_relocations"], 2)
        self.assertEqual(output[0x1040:0x1050], bytes(16))
        self.assertEqual(struct.unpack_from("<I", output, 0x1004)[0], new_tail + 4)

        with self.assertRaisesRegex(
            dev_elf.DevElfError, "expected_shifted_replacement_symbols is 1"
        ):
            dev_elf.audit_relocation_closure(
                bytes(base),
                output,
                relocation_elf,
                {**spec, "expected_shifted_replacement_symbols": 3},
                retail_symbol_elf,
            )

        missing_count = dict(spec)
        del missing_count["expected_replacement_symbols"]
        with self.assertRaisesRegex(
            dev_elf.DevElfError,
            "replacement descriptor requires expected_replacement_symbols",
        ):
            dev_elf.audit_relocation_closure(
                bytes(base),
                output,
                relocation_elf,
                missing_count,
                retail_symbol_elf,
            )

        wrong = bytearray(output)
        struct.pack_into("<I", wrong, 0x1004, new_tail)
        with self.assertRaisesRegex(dev_elf.DevElfError, "does not match"):
            dev_elf.audit_relocation_closure(
                bytes(base), bytes(wrong), relocation_elf, spec, retail_symbol_elf
            )

        missing_symbols = development_symbols[:-1] + [
            ("tail", new_tail, 0, global_func, dev_elf.SHN_UNDEF)
        ]
        with self.assertRaisesRegex(dev_elf.DevElfError, "tail has 0 development"):
            dev_elf.audit_relocation_closure(
                bytes(base),
                output,
                _metadata_elf(missing_symbols, relocations),
                spec,
                retail_symbol_elf,
            )

        ambiguous_symbols = development_symbols + [
            ("tail", new_tail, 8, global_func, 1)
        ]
        with self.assertRaisesRegex(dev_elf.DevElfError, "tail has 2 development"):
            dev_elf.audit_relocation_closure(
                bytes(base),
                output,
                _metadata_elf(ambiguous_symbols, relocations),
                spec,
                retail_symbol_elf,
            )

    def test_two_replacements_require_and_count_cross_relocations(self) -> None:
        old_a = 0x00100040
        old_b = 0x00100050
        new_a = 0x00412000
        new_b = 0x00412010
        base = bytearray(_elf())
        base[0x1040:0x1048] = bytes.fromhex("0800e00300000000")
        base[0x1050:0x1058] = bytes.fromhex("0800e00300000000")
        linked = bytearray(base)
        linked[0x1040:0x1048] = bytes(8)
        linked[0x1050:0x1058] = bytes(8)
        linked.extend(bytes((-len(linked)) & 0xFFF))
        linked.extend(struct.pack("<I", (3 << 26) | (new_b >> 2)))
        linked.extend(bytes(0x18))
        spec = {
            "format": 1,
            "base_sha1": hashlib.sha1(base).hexdigest(),
            "extension_vaddr": new_a,
            "extension_size": 0x1C,
            "retail_static_end": 0x0040C5F0,
            "expected_replacement_symbols": 2,
            "expected_shifted_replacement_symbols": 0,
            "expected_shifted_symbol_relocations": 0,
            "expected_cross_replacement_relocations": 1,
            "replacements": [
                {
                    "retail_object": "a.o",
                    "object": "new-a.o",
                    "expected_symbols": 1,
                    "expected_shifted_symbols": 0,
                    "retained_sections": [],
                },
                {
                    "retail_object": "b.o",
                    "object": "new-b.o",
                    "expected_symbols": 1,
                    "expected_shifted_symbols": 0,
                    "retained_sections": [],
                },
            ],
            "moves": [
                {
                    "object": "a.o",
                    "section": ".text",
                    "old_vaddr": old_a,
                    "old_size": 8,
                    "expected_relocations": 1,
                },
                {
                    "object": "b.o",
                    "section": ".text",
                    "old_vaddr": old_b,
                    "old_size": 8,
                    "expected_relocations": 0,
                },
            ],
        }
        output, _ = dev_elf.finalize_image(bytes(base), bytes(linked), spec)
        global_func = (dev_elf.STB_GLOBAL << 4) | dev_elf.STT_FUNC
        global_notype = dev_elf.STB_GLOBAL << 4
        relocation_elf = _metadata_elf(
            [
                ("dev_move_0_START", new_a, 0, global_notype, 1),
                ("dev_move_0_END", new_a + 0xC, 0, global_notype, 1),
                ("dev_move_1_START", new_b, 0, global_notype, 1),
                ("dev_move_1_END", new_b + 0xC, 0, global_notype, 1),
                ("head_a", new_a, 0xC, global_func, 1),
                ("head_b", new_b, 0xC, global_func, 1),
            ],
            [(new_a, 4, "head_b")],
        )
        retail_symbols = _metadata_elf(
            [
                ("head_a", old_a, 8, global_func, 1),
                ("head_b", old_b, 8, global_func, 1),
            ],
            [],
        )
        summary = dev_elf.audit_relocation_closure(
            bytes(base), output, relocation_elf, spec, retail_symbols
        )
        self.assertEqual(summary["replacement_symbols"], 2)
        self.assertEqual(summary["cross_replacement_relocations"], 1)

        missing_cross = dict(spec)
        del missing_cross["expected_cross_replacement_relocations"]
        with self.assertRaisesRegex(
            dev_elf.DevElfError,
            "multiple replacements require expected_cross_replacement_relocations",
        ):
            dev_elf.audit_relocation_closure(
                bytes(base), output, relocation_elf, missing_cross, retail_symbols
            )

    def test_replacement_object_rejects_unaccounted_allocated_section(self) -> None:
        spec = {
            "replacements": [
                {
                    "retail_object": "old.o",
                    "object": "new.o",
                    "retained_sections": [
                        {
                            "section": ".data",
                            "size": 4,
                            "old_vaddr": 0x100080,
                            "alignment": 4,
                            "storage": "file",
                            "expected_symbols": 1,
                            "expected_gp_references": 0,
                        },
                        {"section": ".bss", "size": 0},
                    ],
                }
            ],
            "moves": [{"object": "old.o", "section": ".rel.text"}],
        }
        replacement = _allocated_object(
            [
                (".text", 0x14, 1),
                (".data", 0, 1),
                (".bss", 0, dev_elf.SHT_NOBITS),
                (".sdata", 4, 1),
            ]
        )
        with self.assertRaisesRegex(
            dev_elf.DevElfError, "unaccounted allocated section .sdata"
        ):
            dev_elf.audit_replacement_objects(spec, [replacement])

    def test_moved_section_storage_must_match_input_section_kind(self) -> None:
        spec = {
            "moves": [
                {
                    "object": "old.o",
                    "section": ".bss",
                    "storage": "nobits",
                }
            ]
        }
        nobits = _allocated_object([(".bss", 0x40, dev_elf.SHT_NOBITS)])
        self.assertEqual(
            dev_elf.audit_moved_sections(spec, {"old.o": nobits}),
            {"moved_input_sections": 1},
        )

        progbits = _allocated_object([(".bss", 0x40, 1)])
        with self.assertRaisesRegex(dev_elf.DevElfError, "declares nobits"):
            dev_elf.audit_moved_sections(spec, {"old.o": progbits})

        file_spec = {"moves": [{"object": "old.o", "section": ".bss"}]}
        with self.assertRaisesRegex(dev_elf.DevElfError, "declares file"):
            dev_elf.audit_moved_sections(file_spec, {"old.o": nobits})

    def test_replacement_object_verifies_nonempty_retained_section(self) -> None:
        spec = {
            "replacements": [
                {
                    "retail_object": "old.o",
                    "object": "new.o",
                    "retained_sections": [
                        {
                            "section": ".data",
                            "size": 8,
                            "old_vaddr": 0x100080,
                            "alignment": 4,
                            "storage": "file",
                            "expected_symbols": 1,
                            "expected_gp_references": 0,
                        }
                    ],
                }
            ],
            "moves": [{"object": "old.o", "section": ".rel.text"}],
        }
        retained = _retained_object(bytes.fromhex("1122334455667788"))
        self.assertEqual(
            dev_elf.audit_replacement_objects(spec, [retained], [retained]),
            {
                "fallback_symbols": 0,
                "retained_sections": 1,
                "retained_symbols": 1,
            },
        )

        changed = bytearray(retained)
        data = dev_elf.parse_allocated_sections(changed)[".data"]
        changed[data.offset] ^= 1
        with self.assertRaisesRegex(dev_elf.DevElfError, "changed bytes"):
            dev_elf.audit_replacement_objects(spec, [bytes(changed)], [retained])

        changed_relocation = bytearray(retained)
        relocation = next(
            section
            for section in dev_elf._object_sections(changed_relocation)
            if section.kind == dev_elf.SHT_REL
        )
        offset, info = dev_elf.REL_ENTRY.unpack_from(
            changed_relocation, relocation.offset
        )
        dev_elf.REL_ENTRY.pack_into(
            changed_relocation, relocation.offset, offset, (info & ~0xFF) | 2
        )
        with self.assertRaisesRegex(dev_elf.DevElfError, "relocation contract"):
            dev_elf.audit_replacement_objects(
                spec, [bytes(changed_relocation)], [retained]
            )

        incomplete = {
            "replacements": [
                {
                    "retail_object": "old.o",
                    "object": "new.o",
                    "retained_sections": [{"section": ".data", "size": 8}],
                }
            ],
            "moves": [{"object": "old.o", "section": ".text"}],
        }
        with self.assertRaisesRegex(dev_elf.DevElfError, "nonempty contract requires"):
            dev_elf.audit_replacement_objects(incomplete, [retained], [retained])

        empty_spec = {
            "replacements": [
                {
                    "retail_object": "old.o",
                    "object": "new.o",
                    "retained_sections": [{"section": ".data", "size": 0}],
                }
            ],
            "moves": [{"object": "old.o", "section": ".text"}],
        }
        replacement_without_data = _fallback_object(bytes(8))
        self.assertEqual(
            dev_elf.audit_replacement_objects(empty_spec, [replacement_without_data]),
            {
                "fallback_symbols": 0,
                "retained_sections": 0,
                "retained_symbols": 0,
            },
        )

    def test_nonempty_retained_section_verifies_linked_gp_contract(self) -> None:
        retained_vaddr = 0x100080
        gp = 0x108000
        displacement = (retained_vaddr - gp) & 0xFFFF
        raw_lw = (0x23 << 26) | (28 << 21) | (2 << 16) | displacement
        raw_sw = (0x2B << 26) | (28 << 21) | (2 << 16) | displacement
        base = bytearray(_elf())
        output = bytearray(base)
        struct.pack_into("<I", base, 0x1010, raw_lw)
        struct.pack_into("<I", output, 0x1010, raw_lw)
        struct.pack_into("<I", base, 0x1014, raw_sw)
        struct.pack_into("<I", output, 0x1014, raw_sw)
        _, base_programs = dev_elf.parse_elf(base)
        _, output_programs = dev_elf.parse_elf(output)
        global_object = (dev_elf.STB_GLOBAL << 4) | dev_elf.STT_OBJECT
        global_func = (dev_elf.STB_GLOBAL << 4) | dev_elf.STT_FUNC
        global_notype = dev_elf.STB_GLOBAL << 4
        development_elf = _metadata_elf(
            [
                ("_gp", gp, 0, global_notype, 1),
                ("state", retained_vaddr, 8, global_object, 1),
                ("worker", 0x100000, 0x20, global_func, 1),
            ],
            [],
        )
        retail_elf = _metadata_elf(
            [
                ("_gp", gp, 0, global_notype, 1),
                ("state", retained_vaddr, 8, global_object, 1),
                ("worker", 0x100000, 0x20, global_func, 1),
            ],
            [],
        )
        development_symbols = dev_elf.parse_linked_symbols(development_elf)
        development_gp, _ = dev_elf._matched_gp_values(
            [], development_symbols, retail_elf, required=True
        )
        drifted_retail_elf = _metadata_elf(
            [
                ("_gp", gp + 4, 0, global_notype, 1),
                ("state", retained_vaddr, 8, global_object, 1),
                ("worker", 0x100000, 0x20, global_func, 1),
            ],
            [],
        )
        with self.assertRaisesRegex(dev_elf.DevElfError, "differs from retail"):
            dev_elf._matched_gp_values(
                [], development_symbols, drifted_retail_elf, required=True
            )
        spec = {
            "replacements": [
                {
                    "retained_sections": [
                        {
                            "section": ".data",
                            "size": 8,
                            "old_vaddr": retained_vaddr,
                            "alignment": 4,
                            "storage": "file",
                            "expected_symbols": 1,
                            "expected_gp_references": 2,
                        }
                    ]
                }
            ],
            "moves": [
                {
                    "object": "old.o",
                    "section": ".text",
                    "old_vaddr": 0x100000,
                    "new_vaddr": 0x100000,
                    "old_size": 0x20,
                    "new_size": 0x20,
                }
            ],
        }
        self.assertEqual(
            dev_elf._audit_retained_sections(
                bytes(base),
                bytes(output),
                base_programs,
                output_programs,
                development_symbols,
                retail_elf,
                spec,
                development_gp,
            ),
            {
                "retained_linked_sections": 1,
                "retained_linked_symbols": 1,
                "retained_gp_references": 2,
            },
        )

        contracted_output = bytearray(output)
        struct.pack_into("<I", contracted_output, 0x1014, 0)
        retained_contract = spec["replacements"][0]["retained_sections"][0]
        retained_contract["expected_retail_gp_references"] = 2
        retained_contract["expected_gp_references"] = 1
        self.assertEqual(
            dev_elf._audit_retained_sections(
                bytes(base),
                bytes(contracted_output),
                base_programs,
                output_programs,
                development_symbols,
                retail_elf,
                spec,
                development_gp,
            )["retained_gp_references"],
            1,
        )
        del retained_contract["expected_retail_gp_references"]
        retained_contract["expected_gp_references"] = 2

        drifted_output = bytearray(output)
        struct.pack_into("<I", drifted_output, 0x1014, 0)
        struct.pack_into("<I", drifted_output, 0x1018, raw_sw)
        with self.assertRaisesRegex(
            dev_elf.DevElfError, "changed its GP-reference sites"
        ):
            dev_elf._audit_retained_sections(
                bytes(base),
                bytes(drifted_output),
                base_programs,
                output_programs,
                development_symbols,
                retail_elf,
                spec,
                development_gp,
            )

        spec["replacements"][0]["retained_sections"][0][
            "expected_gp_references"
        ] = 0
        with self.assertRaisesRegex(dev_elf.DevElfError, "2 retail GP references"):
            dev_elf._audit_retained_sections(
                bytes(base),
                bytes(output),
                base_programs,
                output_programs,
                development_symbols,
                retail_elf,
                spec,
                development_gp,
            )

    def test_moved_state_verifies_complete_gp_reference_contract(self) -> None:
        gp = 0x108000
        old_vaddr = gp - 0x10
        new_vaddr = gp + 0x10

        def raw_lw(target: int) -> int:
            displacement = (target - gp) & 0xFFFF
            return (0x23 << 26) | (28 << 21) | (2 << 16) | displacement

        base = bytearray(_elf())
        output = bytearray(base)
        struct.pack_into("<I", base, 0x1010, raw_lw(old_vaddr))
        struct.pack_into("<I", base, 0x1014, raw_lw(old_vaddr + 4))
        struct.pack_into("<I", output, 0x1010, raw_lw(new_vaddr))
        struct.pack_into("<I", output, 0x1014, 0)
        _, base_programs = dev_elf.parse_elf(base)
        _, output_programs = dev_elf.parse_elf(output)
        global_func = (dev_elf.STB_GLOBAL << 4) | dev_elf.STT_FUNC
        global_notype = dev_elf.STB_GLOBAL << 4
        symbols = [
            ("_gp", gp, 0, global_notype, 1),
            ("main_TEXT_START", 0x100000, 0, global_notype, 1),
            ("main_TEXT_END", 0x100020, 0, global_notype, 1),
            ("worker", 0x100000, 0x20, global_func, 1),
        ]
        development_elf = _metadata_elf(symbols, [])
        retail_elf = _metadata_elf(symbols, [])
        development_symbols = dev_elf.parse_linked_symbols(development_elf)
        spec = {
            "moves": [
                {
                    "object": "state.o",
                    "section": ".sbss",
                    "storage": "nobits",
                    "old_vaddr": old_vaddr,
                    "new_vaddr": new_vaddr,
                    "old_size": 8,
                    "new_size": 8,
                    "expected_retail_gp_references": 2,
                    "expected_gp_references": 1,
                }
            ]
        }
        self.assertEqual(
            dev_elf._audit_moved_gp_references(
                bytes(base),
                bytes(output),
                base_programs,
                output_programs,
                development_symbols,
                retail_elf,
                spec,
                gp,
            ),
            {"moved_gp_contracts": 1, "moved_gp_references": 1},
        )

        wrong_count = {
            "moves": [{**spec["moves"][0], "expected_gp_references": 0}]
        }
        with self.assertRaisesRegex(dev_elf.DevElfError, "1 development GP references"):
            dev_elf._audit_moved_gp_references(
                bytes(base),
                bytes(output),
                base_programs,
                output_programs,
                development_symbols,
                retail_elf,
                wrong_count,
                gp,
            )

        stale_output = bytearray(output)
        struct.pack_into("<I", stale_output, 0x1010, raw_lw(old_vaddr))
        with self.assertRaisesRegex(dev_elf.DevElfError, "abandoned retail range"):
            dev_elf._audit_moved_gp_references(
                bytes(base),
                bytes(stale_output),
                base_programs,
                output_programs,
                development_symbols,
                retail_elf,
                spec,
                gp,
            )

        addition_output = bytearray(output)
        struct.pack_into("<I", addition_output, 0x1040, raw_lw(old_vaddr))
        addition_symbols = dev_elf.parse_linked_symbols(
            _metadata_elf(
                [
                    *symbols,
                    ("addition_worker", 0x100040, 0x20, global_func, 1),
                ],
                [],
            )
        )
        addition_spec = {
            "moves": spec["moves"],
            "additions": [
                {
                    "sections": [".text"],
                    "_linked_sections": [
                        {
                            "section": ".text",
                            "start": 0x100040,
                            "end": 0x100060,
                        }
                    ],
                }
            ],
        }
        with self.assertRaisesRegex(dev_elf.DevElfError, "abandoned retail range"):
            dev_elf._audit_moved_gp_references(
                bytes(base),
                bytes(addition_output),
                base_programs,
                output_programs,
                addition_symbols,
                retail_elf,
                addition_spec,
                gp,
            )

        moved_site_output = bytearray(output)
        struct.pack_into("<I", moved_site_output, 0x1010, 0)
        struct.pack_into("<I", moved_site_output, 0x1018, raw_lw(new_vaddr))
        with self.assertRaisesRegex(dev_elf.DevElfError, "changed its GP-reference sites"):
            dev_elf._audit_moved_gp_references(
                bytes(base),
                bytes(moved_site_output),
                base_programs,
                output_programs,
                development_symbols,
                retail_elf,
                spec,
                gp,
            )

        missing_contract = {
            "moves": [
                {
                    key: value
                    for key, value in spec["moves"][0].items()
                    if not key.startswith("expected_")
                }
            ]
        }
        with self.assertRaisesRegex(dev_elf.DevElfError, "overlaps the retail GP window"):
            dev_elf._audit_moved_gp_references(
                bytes(base),
                bytes(output),
                base_programs,
                output_programs,
                development_symbols,
                retail_elf,
                missing_contract,
                gp,
            )

    def test_replacement_fallback_preserves_bytes_and_relocations(self) -> None:
        spec = {
            "replacements": [
                {
                    "retail_object": "old.o",
                    "object": "new.o",
                    "fallback_symbols": ["fallback"],
                    "retained_sections": [],
                }
            ],
            "moves": [{"object": "old.o", "section": ".text"}],
        }
        body = bytes.fromhex(
            "00000000 0000000c 0800e003 00000000".replace(" ", "")
        )
        retail = _fallback_object(body)
        replacement = _fallback_object(body, symbol_value=8)
        self.assertEqual(
            dev_elf.audit_replacement_objects(
                spec, [replacement], [retail]
            )["fallback_symbols"],
            1,
        )

        relocated = bytearray(replacement)
        text = next(
            section
            for section in dev_elf._object_sections(relocated)
            if section.name == ".text"
        )
        relocation_word = text.offset + 8 + 4
        word = struct.unpack_from("<I", relocated, relocation_word)[0]
        struct.pack_into("<I", relocated, relocation_word, word | 1)
        with self.assertRaisesRegex(dev_elf.DevElfError, "changed bytes"):
            dev_elf.audit_replacement_objects(
                spec, [bytes(relocated)], [retail]
            )

        shifted_named_body = bytearray(body)
        struct.pack_into("<I", shifted_named_body, 4, 0x0C000001)
        shifted_retail = _fallback_object(body, symbol_value=8, target_value=0)
        shifted_replacement = _fallback_object(
            bytes(shifted_named_body), symbol_value=12, target_value=4
        )
        with self.assertRaisesRegex(dev_elf.DevElfError, "changed bytes"):
            dev_elf.audit_replacement_objects(
                spec, [shifted_replacement], [shifted_retail]
            )

        text_section = dev_elf.ObjectSection(
            2, ".text", 1, 0, 0, 0, 0x40, 0, 0, 4, 0
        )
        section_relocation = (
            4,
            dev_elf.R_MIPS_26,
            "",
            0,
            dev_elf.STT_SECTION,
            ".text",
        )
        global_func = (dev_elf.STB_GLOBAL << 4) | dev_elf.STT_FUNC
        old_target = dev_elf.LinkedSymbol(
            1, 1, "callee", 0, 4, global_func, 0, 2
        )
        new_target = dev_elf.LinkedSymbol(
            1, 1, "callee", 4, 4, global_func, 0, 2
        )
        self.assertEqual(
            dev_elf._object_relocation_target(
                0x0C000000, section_relocation, [text_section], [old_target]
            ),
            ("callee", 0),
        )
        self.assertEqual(
            dev_elf._object_relocation_target(
                0x0C000001, section_relocation, [text_section], [new_target]
            ),
            ("callee", 0),
        )

        address_relocations = [
            (0, dev_elf.R_MIPS_HI16, "", 0, dev_elf.STT_SECTION, ".text"),
            (4, dev_elf.R_MIPS_LO16, "", 0, dev_elf.STT_SECTION, ".text"),
        ]
        old_address_body = struct.pack("<II", 0x3C050000, 0x24A50020)
        new_address_body = struct.pack("<II", 0x3C050000, 0x24A50028)
        old_address_target = dev_elf.LinkedSymbol(
            1, 1, "callee", 0x20, 0x10, global_func, 0, 2
        )
        new_address_target = dev_elf.LinkedSymbol(
            1, 1, "callee", 0x28, 0x10, global_func, 0, 2
        )
        old_targets = dev_elf._fallback_hi16_lo16_targets(
            old_address_body,
            address_relocations,
            [text_section],
            [old_address_target],
        )
        self.assertEqual(old_targets, {0: {("callee", 0)}, 4: {("callee", 0)}})
        self.assertEqual(
            dev_elf._fallback_hi16_lo16_targets(
                new_address_body,
                address_relocations,
                [text_section],
                [new_address_target],
            ),
            old_targets,
        )
        wrong_address_target = dev_elf.LinkedSymbol(
            1, 1, "other", 0x28, 0x10, global_func, 0, 2
        )
        self.assertNotEqual(
            dev_elf._fallback_hi16_lo16_targets(
                new_address_body,
                address_relocations,
                [text_section],
                [wrong_address_target],
            ),
            old_targets,
        )
        old_fallback = dev_elf.LinkedSymbol(
            1, 2, "fallback", 0, 8, global_func, 0, 2
        )
        new_fallback = dev_elf.LinkedSymbol(
            1, 2, "fallback", 0, 8, global_func, 0, 2
        )
        self.assertTrue(
            dev_elf._fallback_bytes_match(
                old_address_body,
                new_address_body,
                text_section,
                text_section,
                old_fallback,
                new_fallback,
                address_relocations,
                [text_section],
                [text_section],
                [old_address_target, old_fallback],
                [new_address_target, new_fallback],
            )
        )

        changed_opcode = bytearray(relocated)
        word = struct.unpack_from("<I", changed_opcode, relocation_word)[0]
        struct.pack_into("<I", changed_opcode, relocation_word, word ^ 0x04000000)
        with self.assertRaisesRegex(dev_elf.DevElfError, "changed bytes"):
            dev_elf.audit_replacement_objects(
                spec, [bytes(changed_opcode)], [retail]
            )

        corrupted = bytearray(replacement)
        corrupted[text.offset + 8] ^= 1
        with self.assertRaisesRegex(dev_elf.DevElfError, "changed bytes"):
            dev_elf.audit_replacement_objects(
                spec, [bytes(corrupted)], [retail]
            )

        changed_target = _fallback_object(
            body, symbol_value=8, relocation_name="other"
        )
        with self.assertRaisesRegex(dev_elf.DevElfError, "relocation contract"):
            dev_elf.audit_replacement_objects(
                spec, [changed_target], [retail]
            )

    def test_replacement_object_rejects_common_storage(self) -> None:
        spec = {
            "replacements": [
                {
                    "retail_object": "old.o",
                    "object": "new.o",
                    "retained_sections": [],
                }
            ],
            "moves": [{"object": "old.o", "section": ".text"}],
        }
        replacement = _fallback_object(bytes(8), common_symbol=True)
        with self.assertRaisesRegex(dev_elf.DevElfError, "uses COMMON storage"):
            dev_elf.audit_replacement_objects(spec, [replacement])

    def test_stale_scan_stops_at_overwriting_lui(self) -> None:
        image = bytearray(_elf())
        old_start = 0x00412000
        hi, _ = _address_words(0x0040B7DB, 2)
        replacement_hi, _ = _address_words(0x003B25A0, 2)
        struct.pack_into("<I", image, 0x1000, hi)
        struct.pack_into("<I", image, 0x1004, replacement_hi)
        struct.pack_into(
            "<I",
            image,
            0x1008,
            (0x09 << 26) | (2 << 21) | (2 << 16) | 0x25A0,
        )
        scanned = dev_elf.scan_stale_move_references(
            bytes(image),
            [(old_start, old_start + 0x100, 0x00500000, 0x00500100)],
        )
        self.assertGreater(scanned, 0)

    def test_section_relative_hi_lo_repair_uses_containing_symbol(self) -> None:
        old_start = 0x00100040
        new_start = 0x00412000
        old_target = old_start + 8
        new_target = new_start + 8
        image = bytearray(_elf())
        hi, lo = _address_words(old_target)
        struct.pack_into("<I", image, 0x1000, hi)
        struct.pack_into("<I", image, 0x1004, lo)
        global_func = (dev_elf.STB_GLOBAL << 4) | dev_elf.STT_FUNC
        global_notype = dev_elf.STB_GLOBAL << 4
        relocation_elf = _metadata_elf(
            [
                ("dev_move_0_START", new_start, 0, global_notype, 1),
                ("dev_move_0_END", new_start + 0x14, 0, global_notype, 1),
                ("head", new_start, 0x14, global_func, 1),
                (".main", 0x00100000, 0, dev_elf.STT_SECTION, 1),
            ],
            [(0x00100000, 5, ".main"), (0x00100004, 6, ".main")],
        )
        retail_symbols = _metadata_elf(
            [("head", old_start, 0x10, global_func, 1)], []
        )
        spec = {
            "replacements": [
                {
                    "retail_object": "old.o",
                    "object": "new.o",
                    "retained_sections": [],
                }
            ],
            "moves": [
                {
                    "object": "old.o",
                    "section": ".text",
                    "old_vaddr": old_start,
                    "old_size": 0x10,
                    "new_vaddr": new_start,
                    "new_size": 0x14,
                }
            ],
        }
        repaired, count = dev_elf.repair_stale_replacement_relocations(
            bytes(image), relocation_elf, spec, retail_symbols
        )
        self.assertEqual(count, 2)
        self.assertEqual(
            dev_elf._decode_mips_address(repaired, 0x1000, 0x1004), new_target
        )

    def test_finalize_adds_segment_and_separates_heap_from_bss(self) -> None:
        retail_end = 0x0040C5F0
        base = bytearray(_elf())
        heap_hi, heap_lo = _address_words(retail_end)
        bss_hi, bss_lo = _address_words(retail_end, 3)
        struct.pack_into("<I", base, 0x200, heap_hi)
        struct.pack_into("<I", base, 0x208, heap_lo)
        struct.pack_into("<I", base, 0x20C, retail_end)
        struct.pack_into("<I", base, 0x210, bss_hi)
        struct.pack_into("<I", base, 0x218, bss_lo)

        linked = bytearray(base)
        linked.extend(bytes((-len(linked)) & 0xFFF))
        linked.extend(bytes.fromhex("1122334455667788"))
        spec = {
            "format": 1,
            "base_sha1": hashlib.sha1(base).hexdigest(),
            "extension_vaddr": "0x00412000",
            "alignment": "0x1000",
            "heap_alignment": "0x80",
            "retail_static_end": hex(retail_end),
            "bss_end_assertions": [{"hi_offset": "0x210", "lo_offset": "0x218"}],
            "heap_address_patches": [{"hi_offset": "0x200", "lo_offset": "0x208"}],
            "heap_word_offsets": ["0x20c"],
        }

        output, summary = dev_elf.finalize_image(bytes(base), bytes(linked), spec)
        header, programs = dev_elf.parse_elf(output)
        self.assertEqual(header.phnum, 2)
        self.assertEqual(programs[-1].offset, 0x2000)
        self.assertEqual(programs[-1].vaddr, 0x00412000)
        self.assertEqual(programs[-1].file_size, 8)
        self.assertEqual(output[0x2000:], bytes.fromhex("1122334455667788"))
        self.assertEqual(summary["heap_start"], 0x00412080)
        dev_elf.assert_mips_address(output, 0x200, 0x208, 0x00412080, "heap")
        dev_elf.assert_mips_address(output, 0x210, 0x218, retail_end, "bss")
        self.assertEqual(struct.unpack_from("<I", output, 0x20C)[0], 0x00412080)

    def test_finalize_keeps_nobits_tail_out_of_file_payload(self) -> None:
        base = _elf()
        linked = bytearray(base)
        linked.extend(bytes((-len(linked)) & 0xFFF))
        linked.extend(bytes.fromhex("1122334455667788"))
        spec = {
            "format": 1,
            "base_sha1": hashlib.sha1(base).hexdigest(),
            "extension_vaddr": 0x00412000,
            "extension_memory_size": 0x48,
            "retail_static_end": 0x0040C5F0,
        }

        output, summary = dev_elf.finalize_image(
            base,
            bytes(linked),
            spec,
            linked_file_size=8,
            linked_memory_size=0x48,
        )
        _, programs = dev_elf.parse_elf(output)
        self.assertEqual(programs[-1].file_size, 8)
        self.assertEqual(programs[-1].memory_size, 0x48)
        self.assertEqual(len(output), len(linked))
        self.assertEqual(output[-8:], bytes.fromhex("1122334455667788"))
        self.assertEqual(summary["heap_start"], 0x00412080)

        with self.assertRaisesRegex(dev_elf.DevElfError, "expected 0x49"):
            dev_elf.finalize_image(
                base,
                bytes(linked),
                {**spec, "extension_memory_size": 0x49},
                linked_file_size=8,
                linked_memory_size=0x48,
            )

    def test_finalize_rejects_drifted_patch_site(self) -> None:
        base = bytearray(_elf())
        hi, lo = _address_words(0x0040C5F0)
        struct.pack_into("<I", base, 0x200, hi)
        struct.pack_into("<I", base, 0x208, lo)
        linked = bytes(base) + bytes((-len(base)) & 0xFFF) + bytes(4)
        spec = {
            "format": 1,
            "base_sha1": hashlib.sha1(base).hexdigest(),
            "extension_vaddr": 0x00412000,
            "retail_static_end": 0x0040C600,
            "heap_address_patches": [{"hi_offset": 0x200, "lo_offset": 0x208}],
        }
        with self.assertRaisesRegex(dev_elf.DevElfError, "expected 0x0040C600"):
            dev_elf.finalize_image(bytes(base), linked, spec)

    def test_segment_rejects_overlap_and_occupied_header_slot(self) -> None:
        image = bytearray(_elf())
        image.extend(bytes((-len(image)) & 0xFFF) + bytes(4))
        with self.assertRaisesRegex(dev_elf.DevElfError, "overlaps"):
            dev_elf.install_appended_load_segment(
                image, base_file_size=0x1100, vaddr=0x00100000, alignment=0x1000
            )

        image = bytearray(_elf())
        image.extend(bytes((-len(image)) & 0xFFF) + bytes(4))
        image[dev_elf.ELF_HEADER.size + dev_elf.PROGRAM_HEADER.size] = 1
        with self.assertRaisesRegex(dev_elf.DevElfError, "nonzero"):
            dev_elf.install_appended_load_segment(
                image, base_file_size=0x1100, vaddr=0x00412000, alignment=0x1000
            )

    def test_linker_script_moves_exact_declared_section(self) -> None:
        source = """\
SECTIONS
{
    .main 0x100000 :
    {
        FILL(0x00000000);
        build/dds1/a.o(.text);
        build/dds1/b.o(.text);
    }
    elf_trailer_VRAM_END = .;

    /DISCARD/ :
    {
        *(*);
    }
}
"""
        spec = {
            "moves": [
                {"object": "build/dds1/a.o", "section": ".text", "size": "0x38"}
            ],
            "additions": [
                {
                    "object": "build/dds1/dev.o",
                    "sections": [".text", ".data"],
                    "alignment": 8,
                }
            ],
            "extension_vaddr": "0x412000",
            "alignment": "0x1000",
        }
        result = dev_elf.render_linker_script(source, spec)
        self.assertIn(". = ALIGN(0x8);", result)
        self.assertIn(". += 0x38; /* development slot", result)
        self.assertEqual(result.count("build/dds1/a.o(.text);"), 1)
        self.assertIn("build/dds1/dev.o(.text);", result)
        self.assertIn("build/dds1/dev.o(.data);", result)
        self.assertIn("dev_addition_0_START", result)
        self.assertIn("dev_addition_0_END", result)
        self.assertIn(".dev_extension 0x412000", result)
        self.assertLess(result.index(".dev_extension"), result.index("/DISCARD/"))

    def test_linker_script_places_nobits_move_in_no_load_tail(self) -> None:
        source = """\
SECTIONS
{
    .main :
    {
        old.o(.text);
    }
    .main_bss (NOLOAD) :
    {
        old.o(.bss);
    }
    elf_trailer_VRAM_END = .;
    /DISCARD/ : { *(*); }
}
"""
        spec = {
            "moves": [
                {
                    "object": "old.o",
                    "section": ".text",
                    "old_size": 4,
                    "new_size": 4,
                },
                {
                    "object": "old.o",
                    "section": ".bss",
                    "storage": "nobits",
                    "old_size": 0x40,
                    "new_size": 0x40,
                    "alignment": 8,
                },
            ],
            "extension_vaddr": 0x412000,
        }
        result = dev_elf.render_linker_script(source, spec)
        file_start = result.index(".dev_extension 0x412000")
        file_end = result.index("dev_extension_FILE_END")
        nobits_start = result.index(".dev_extension_nobits (NOLOAD)")
        memory_end = result.index("dev_extension_VRAM_END")
        size_assertion = result.index("ASSERT(dev_move_1_END")
        self.assertLess(file_start, file_end)
        self.assertLess(file_end, nobits_start)
        self.assertLess(nobits_start, memory_end)
        self.assertLess(memory_end, size_assertion)
        self.assertLess(result.index("old.o(.text);", file_start), file_end)
        self.assertLess(nobits_start, result.rindex("old.o(.bss);"))
        self.assertLess(result.rindex("old.o(.bss);"), memory_end)

    def test_linker_script_rejects_missing_or_duplicate_placement(self) -> None:
        spec = {
            "moves": [{"object": "a.o", "section": ".text", "size": 4}],
            "extension_vaddr": 0x412000,
        }
        with self.assertRaisesRegex(dev_elf.DevElfError, "found 0"):
            dev_elf.render_linker_script("SECTIONS {\n /DISCARD/ : {}\n}", spec)
        duplicate = "SECTIONS {\n a.o(.text);\n a.o(.text);\n /DISCARD/ : {}\n}"
        with self.assertRaisesRegex(dev_elf.DevElfError, "found 2"):
            dev_elf.render_linker_script(duplicate, spec)

    def test_linker_script_substitutes_dynamic_replacement(self) -> None:
        source = """\
SECTIONS
{
    .text :
    {
        old.o(.text);
    }
    .data :
    {
        old.o(.data);
    }
    .bss :
    {
        old.o(.bss);
    }
    elf_trailer_VRAM_END = .;
    /DISCARD/ : { *(*); }
}
"""
        spec = {
            "extension_vaddr": 0x412000,
            "replacements": [
                {
                    "retail_object": "old.o",
                    "object": "new.o",
                    "retained_sections": [
                        {
                            "section": ".data",
                            "size": 4,
                            "old_vaddr": 0x100080,
                            "alignment": 4,
                            "storage": "file",
                            "expected_symbols": 1,
                            "expected_gp_references": 0,
                        },
                        {"section": ".bss", "size": 0},
                    ],
                }
            ],
            "moves": [
                {
                    "object": "old.o",
                    "section": ".text",
                    "old_size": 0x10,
                    "alignment": 8,
                }
            ],
        }
        result = dev_elf.render_linker_script(source, spec)
        self.assertIn(". = ALIGN(0x8);", result)
        self.assertIn(". += 0x10; /* development slot", result)
        self.assertIn("dev_move_0_START", result)
        self.assertIn("new.o(.text);", result)
        self.assertIn("new.o(.data);", result)
        self.assertIn("new.o(.bss);", result)
        self.assertIn("dev_replacement_0_retained_0_START", result)
        self.assertIn("dev_replacement_0_retained_0_START == 0x100080", result)
        self.assertIn("dev_replacement_0_retained_1_END", result)
        self.assertNotIn("old.o(.text);", result)
        self.assertNotIn("old.o(.data);", result)
        self.assertNotIn("old.o(.bss);", result)


if __name__ == "__main__":
    unittest.main()
