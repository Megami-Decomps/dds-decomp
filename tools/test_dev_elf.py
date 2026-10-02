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
        ]
        spec = {
            "moves": [{"old_vaddr": 0x00100020, "old_size": 4}],
            "additions": [{}],
        }
        resolved, _ = dev_elf._resolve_linked_layout(
            spec, _metadata_elf(symbols, [])
        )
        self.assertEqual(resolved["moves"][0]["new_vaddr"], 0x00412000)
        self.assertEqual(resolved["moves"][0]["new_size"], 4)
        self.assertEqual(resolved["additions"][0]["new_vaddr"], 0x00412004)
        self.assertEqual(resolved["additions"][0]["size"], 8)

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
        struct.pack_into("<I", base, 0x1040, 0x03E00008)
        linked = bytearray(base)
        struct.pack_into("<I", linked, 0x1000, _address_words(old_vaddr)[0])
        struct.pack_into(
            "<I", linked, 0x1004, (0x23 << 26) | (4 << 21) | (old_vaddr & 0xFFFF)
        )
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

    def test_replacement_object_rejects_unaccounted_allocated_section(self) -> None:
        spec = {
            "replacements": [
                {
                    "retail_object": "old.o",
                    "object": "new.o",
                    "retained_sections": [
                        {"section": ".data", "size": 0},
                        {"section": ".bss", "size": 0},
                    ],
                }
            ],
            "moves": [{"object": "old.o", "section": ".text"}],
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

    def test_replacement_object_rejects_nonempty_retained_section(self) -> None:
        spec = {
            "replacements": [
                {
                    "retail_object": "old.o",
                    "object": "new.o",
                    "retained_sections": [{"section": ".data", "size": 4}],
                }
            ],
            "moves": [{"object": "old.o", "section": ".text"}],
        }
        replacement = _allocated_object([(".text", 4, 1), (".data", 4, 1)])
        with self.assertRaisesRegex(dev_elf.DevElfError, "must remain empty"):
            dev_elf.audit_replacement_objects(spec, [replacement])

        spec["replacements"][0]["retained_sections"][0]["size"] = 0
        with self.assertRaisesRegex(
            dev_elf.DevElfError, "section .data has size 0x4, expected 0x0"
        ):
            dev_elf.audit_replacement_objects(spec, [replacement])

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
        self.assertIn(". += 0x38; /* development slot", result)
        self.assertEqual(result.count("build/dds1/a.o(.text);"), 1)
        self.assertIn("build/dds1/dev.o(.text);", result)
        self.assertIn("build/dds1/dev.o(.data);", result)
        self.assertIn("dev_addition_0_START", result)
        self.assertIn("dev_addition_0_END", result)
        self.assertIn(".dev_extension 0x412000", result)
        self.assertLess(result.index(".dev_extension"), result.index("/DISCARD/"))

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
                        {"section": ".data", "size": 0},
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
        self.assertIn(". += 0x10; /* development slot", result)
        self.assertIn("dev_move_0_START", result)
        self.assertIn("new.o(.text);", result)
        self.assertIn("new.o(.data);", result)
        self.assertIn("new.o(.bss);", result)
        self.assertIn("dev_replacement_0_retained_0_START", result)
        self.assertIn("dev_replacement_0_retained_1_END", result)
        self.assertNotIn("old.o(.text);", result)
        self.assertNotIn("old.o(.data);", result)
        self.assertNotIn("old.o(.bss);", result)


if __name__ == "__main__":
    unittest.main()
