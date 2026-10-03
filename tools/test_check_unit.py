#!/usr/bin/env python3
"""Focused tests for relocation-aware unit verification."""

import struct
import unittest

from check_unit import (
    bss_ownership_problems,
    bss_symbols,
    common_symbols,
    nobits_ownership_problems,
    nobits_symbols,
    owned_bss_range,
    owned_bss_size,
    owned_nobits_range,
    owned_nobits_size,
    owns_exact_section_item,
    relocate_sdata_item,
    section_symbols,
    source_owned_item_size,
    symbols,
    trim_sdata_item,
)


class SdataRelocationTests(unittest.TestCase):
    def test_resolves_local_text_and_external_pointers(self):
        item = struct.pack("<II", 0x24, 4)
        relocs = {
            0x10: ("R_MIPS_32", ".text"),
            0x14: ("R_MIPS_32", "externalCallback"),
        }
        funcs = [(0x20, 0x10, "localCallback")]
        syms = {"localCallback": 0x00101000, "externalCallback": 0x00202000}

        linked, problems = relocate_sdata_item(item, 0x10, relocs, funcs, syms)

        self.assertEqual(problems, [])
        self.assertEqual(struct.unpack("<II", linked), (0x00101004, 0x00202004))

    def test_rejects_unresolved_local_text_target(self):
        linked, problems = relocate_sdata_item(
            struct.pack("<I", 0x40), 0, {0: ("R_MIPS_32", ".text")},
            [(0x20, 0x10, "localCallback")], {"localCallback": 0x00101000})

        self.assertEqual(linked, struct.pack("<I", 0x40))
        self.assertEqual(problems, ["unresolved R_MIPS_32 .text at +0x0"])

    def test_rejects_unsupported_relocation(self):
        linked, problems = relocate_sdata_item(
            struct.pack("<I", 0), 0, {0: ("R_MIPS_GPREL16", "global")}, [],
            {"global": 0x00303000})

        self.assertEqual(linked, struct.pack("<I", 0))
        self.assertEqual(problems, ["unsupported R_MIPS_GPREL16 at +0x0"])

    def test_relocation_words_are_not_trimmed_as_zero_padding(self):
        item = b"\0" * 12
        relocs = {0x10: ("R_MIPS_32", ".text"), 0x14: ("R_MIPS_32", ".text")}

        self.assertEqual(trim_sdata_item(item, 0x10, relocs), b"\0" * 8)


class OwnedBssTests(unittest.TestCase):
    YAML = """\
segments:
  - name: main
    subsegments:
      - { start: 0x1000, type: .bss, vram: 0x003DC658, name: file/fileManager }
      - { start: 0x1000, type: bss, vram: 0x003DC698, name: bss }
"""

    def test_dict_form_unit_owns_exact_span(self):
        self.assertEqual(
            owned_bss_range(self.YAML, "file/fileManager"),
            (0x003DC658, 0x40),
        )
        self.assertEqual(owned_bss_size(self.YAML, "file/fileManager"), 0x40)

    def test_sbss_uses_the_same_exact_symbol_contract(self):
        sbss = self.YAML.replace(".bss", ".sbss", 1).replace(
            "file/fileManager", "sdf/sdfThread"
        )
        self.assertEqual(
            owned_nobits_range(sbss, "sdf/sdfThread", ".sbss"),
            (0x003DC658, 0x40),
        )
        self.assertEqual(
            owned_nobits_size(sbss, "sdf/sdfThread", ".sbss"), 0x40
        )
        emitted = [(0x00, 0x40, "sdfTrackedThreadState", True)]
        self.assertEqual(
            nobits_ownership_problems(
                sbss, "sdf/sdfThread", ".sbss", 0x40, emitted,
                {"sdfTrackedThreadState": 0x003DC658},
            ),
            [],
        )
        self.assertIsNone(
            owned_nobits_size(sbss, "sdf/sdfThread", ".bss")
        )

    def test_rejects_non_nobits_section(self):
        with self.assertRaisesRegex(ValueError, "unsupported NOBITS section"):
            owned_nobits_range(self.YAML, "file/fileManager", ".data")
        with self.assertRaisesRegex(ValueError, "unsupported NOBITS section"):
            nobits_symbols("", ".data")

    def ownership_problems(self, emitted, syms=None, section_size=0x40):
        if syms is None:
            syms = {
                "fileManagerHead": 0x003DC658,
                "fileManagerSlots": 0x003DC668,
            }
        return bss_ownership_problems(
            self.YAML, "file/fileManager", section_size, emitted, syms
        )

    def test_accepts_exact_named_object_layout(self):
        emitted = [
            (0x00, 0x10, "fileManagerHead", True),
            (0x10, 0x30, "fileManagerSlots", True),
        ]
        self.assertEqual(self.ownership_problems(emitted), [])

    def test_rejects_same_size_wrong_name(self):
        emitted = [(0x00, 0x40, "wrongManagerWork", True)]
        problems = self.ownership_problems(
            emitted, {"fileManagerWork": 0x003DC658}
        )
        self.assertIn("extra emitted symbol wrongManagerWork", problems)
        self.assertIn(
            "missing retail symbol fileManagerWork at +0x0 size 0x40", problems
        )

    def test_rejects_wrong_symbol_order(self):
        emitted = [
            (0x00, 0x30, "fileManagerSlots", True),
            (0x30, 0x10, "fileManagerHead", True),
        ]
        problems = self.ownership_problems(emitted)
        self.assertIn(
            "fileManagerSlots is at +0x0, retail inventory requires +0x10",
            problems,
        )
        self.assertIn(
            "fileManagerHead is at +0x30, retail inventory requires +0x0",
            problems,
        )

    def test_rejects_wrong_offset_and_individual_size(self):
        wrong_offset = [
            (0x00, 0x14, "fileManagerHead", True),
            (0x14, 0x2C, "fileManagerSlots", True),
        ]
        problems = self.ownership_problems(wrong_offset)
        self.assertIn(
            "fileManagerHead has size 0x14, retail inventory requires 0x10",
            problems,
        )
        self.assertIn(
            "fileManagerSlots is at +0x14, retail inventory requires +0x10",
            problems,
        )

        wrong_size = [
            (0x00, 0x0C, "fileManagerHead", True),
            (0x10, 0x30, "fileManagerSlots", True),
        ]
        problems = self.ownership_problems(wrong_size)
        self.assertIn(
            "fileManagerHead has size 0xC, retail inventory requires 0x10",
            problems,
        )

    def test_rejects_non_object_and_wrong_retail_address(self):
        emitted = [(0x00, 0x40, "fileManagerWork", False)]
        problems = self.ownership_problems(
            emitted, {"fileManagerWork": 0x003DC65C}
        )
        self.assertIn("fileManagerWork is not STT_OBJECT", problems)
        self.assertIn(
            "retail inventory begins at +0x4, expected +0x0",
            problems,
        )
        self.assertIn(
            "fileManagerWork is at +0x0, retail inventory requires +0x4",
            problems,
        )

    def test_rejects_anonymous_padding_overlap_and_section_size(self):
        gap = [(0x04, 0x3C, "fileManagerWork", True)]
        problems = self.ownership_problems(
            gap, {"fileManagerWork": 0x003DC65C}
        )
        self.assertIn(
            "retail inventory begins at +0x4, expected +0x0",
            problems,
        )

        overlap = [
            (0x00, 0x20, "fileManagerHead", True),
            (0x10, 0x30, "fileManagerSlots", True),
        ]
        problems = self.ownership_problems(overlap)
        self.assertIn(
            "fileManagerHead has size 0x20, retail inventory requires 0x10",
            problems,
        )
        self.assertIn(
            "section size 0x44, retail span 0x40",
            self.ownership_problems(
                [(0x00, 0x40, "fileManagerWork", True)],
                {"fileManagerWork": 0x003DC658},
                section_size=0x44,
            ),
        )

    def test_rejects_omitted_interior_retail_symbol(self):
        syms = {
            "fileManagerHead": 0x003DC658,
            "fileManagerInterior": 0x003DC668,
            "fileManagerTail": 0x003DC678,
        }
        emitted = [
            (0x00, 0x20, "fileManagerHead", True),
            (0x20, 0x20, "fileManagerTail", True),
        ]

        problems = self.ownership_problems(emitted, syms)

        self.assertIn(
            "fileManagerHead has size 0x20, retail inventory requires 0x10",
            problems,
        )
        self.assertIn(
            "missing retail symbol fileManagerInterior at +0x10 size 0x10",
            problems,
        )

    def test_requires_aliases_one_for_one(self):
        syms = {
            "fileManagerHead": 0x003DC658,
            "fileManagerHeadAlias": 0x003DC658,
            "fileManagerSlots": 0x003DC668,
        }
        exact = [
            (0x00, 0x10, "fileManagerHeadAlias", True),
            (0x10, 0x30, "fileManagerSlots", True),
            (0x00, 0x10, "fileManagerHead", True),
        ]
        self.assertEqual(self.ownership_problems(exact, syms), [])

        missing_alias = exact[1:]
        self.assertIn(
            "missing retail symbol fileManagerHeadAlias at +0x0 size 0x10",
            self.ownership_problems(missing_alias, syms),
        )

        extra_alias = [*exact, (0x00, 0x10, "inventedAlias", True)]
        self.assertIn(
            "extra emitted symbol inventedAlias",
            self.ownership_problems(extra_alias, syms),
        )

    def test_rejects_duplicate_emitted_name_and_empty_inventory(self):
        duplicate = [
            (0x00, 0x10, "fileManagerHead", True),
            (0x00, 0x10, "fileManagerHead", True),
            (0x10, 0x30, "fileManagerSlots", True),
        ]
        self.assertIn(
            "duplicate symbol fileManagerHead",
            self.ownership_problems(duplicate),
        )
        self.assertIn(
            "retail span has no symbol_addrs inventory",
            self.ownership_problems([], {}),
        )

    def test_real_paired_nobits_inventories_remain_accepted(self):
        for version in ("dds1", "dds2"):
            with self.subTest(version=version, section=".bss"):
                syms = symbols(version)
                start = syms["fileManagerWork"]
                end = syms["fileRequestEntries"]
                yaml_text = self.YAML.replace("0x003DC658", hex(start)).replace(
                    "0x003DC698", hex(end)
                )
                self.assertEqual(end - start, 0x40)
                self.assertEqual(
                    bss_ownership_problems(
                        yaml_text,
                        "file/fileManager",
                        0x40,
                        [(0, 0x40, "fileManagerWork", True)],
                        syms,
                    ),
                    [],
                )

            with self.subTest(version=version, section=".sbss"):
                syms = symbols(version)
                start = syms["sdfTrackedThreadSemaphore"]
                end = syms["sdfThreadWakeWorkerId"]
                yaml_text = self.YAML.replace(".bss", ".sbss", 1).replace(
                    "file/fileManager", "sdf/sdfThread"
                ).replace("0x003DC658", hex(start)).replace(
                    "0x003DC698", hex(end)
                )
                self.assertEqual(end - start, 8)
                self.assertEqual(
                    nobits_ownership_problems(
                        yaml_text,
                        "sdf/sdfThread",
                        ".sbss",
                        8,
                        [
                            (0, 4, "sdfTrackedThreadSemaphore", True),
                            (4, 4, "sdfTrackedThreadHead", True),
                        ],
                        syms,
                    ),
                    [],
                )

    def test_parses_bss_object_type_from_objdump(self):
        table = """\
SYMBOL TABLE:
00000000 l    d  .bss  00000000 .bss
00000000 g     O .bss  00000010 fileManagerHead
00000010 l       .bss  00000030 fileManagerSlots
00000000 g     O .data 00000004 initialized
"""
        self.assertEqual(
            bss_symbols(table),
            [
                (0x00, 0x10, "fileManagerHead", True),
                (0x10, 0x30, "fileManagerSlots", False),
            ],
        )

    def test_parses_sbss_symbols_independently(self):
        table = """\
SYMBOL TABLE:
00000000 l    d  .sbss 00000000 .sbss
00000000 g     O .sbss 00000008 sdfTrackedThreadState
00000000 g     O .bss  00000040 fileManagerWork
"""
        self.assertEqual(
            nobits_symbols(table, ".sbss"),
            [(0x00, 0x08, "sdfTrackedThreadState", True)],
        )

    def test_list_form_or_wrong_unit_does_not_claim_bss(self):
        list_form = """\
segments:
  - [0x1000, .bss, file/fileManager]
  - { start: 0x1000, type: bss, vram: 0x003DC698, name: bss }
"""
        self.assertIsNone(owned_bss_size(list_form, "file/fileManager"))
        self.assertIsNone(owned_bss_size(self.YAML, "file/anotherUnit"))
        self.assertIsNone(owned_bss_size(
            self.YAML.replace("type: .bss", "type: bss", 1),
            "file/fileManager",
        ))

    def test_requires_immediate_explicit_next_vram_boundary(self):
        missing_boundary = """\
segments:
  - name: main
    subsegments:
      - { start: 0x1000, type: .bss, vram: 0x003DC658, name: file/fileManager }
      - [0x1000, bin, trailer]
      - { start: 0x1000, type: bss, vram: 0x003DC698, name: bss }
"""
        self.assertIsNone(
            owned_bss_size(missing_boundary, "file/fileManager")
        )

        crossed_parent = """\
segments:
  - name: main
    subsegments:
      - { start: 0x1000, type: .bss, vram: 0x003DC658, name: file/fileManager }
  - name: another
    subsegments:
      - { start: 0x2000, type: bss, vram: 0x003DC698, name: bss }
"""
        self.assertIsNone(owned_bss_size(crossed_parent, "file/fileManager"))

    def test_rejects_duplicate_or_nonadvancing_claims(self):
        duplicate = self.YAML + """\
      - { start: 0x1000, type: .bss, vram: 0x003DC700, name: file/fileManager }
      - { start: 0x1000, type: bss, vram: 0x003DC740, name: bss }
"""
        backwards = self.YAML.replace("0x003DC698", "0x003DC650")
        self.assertIsNone(owned_bss_size(duplicate, "file/fileManager"))
        self.assertIsNone(owned_bss_size(backwards, "file/fileManager"))

    def test_common_symbols_are_kept_out_of_owned_bss(self):
        nm = """\
00000004 00000040 c fileManagerWork
00000000 00000040 B explicitFileManagerWork
00000080 00000008 C ordinaryCommon
"""
        self.assertEqual(
            common_symbols(nm),
            [("fileManagerWork", 0x40), ("ordinaryCommon", 8)],
        )


class SectionOwnershipTests(unittest.TestCase):
    SYMBOLS = """\
00000000 g     O .sdata 00000008 D_003BC7D8
00000008 g     O .sdata 00000008 D_003BC7E0
00000000 g     O .data  00000008 unrelated
00000000         *UND*  00000040 fileManagerWork
"""

    def test_parses_only_requested_section(self):
        self.assertEqual(
            section_symbols(self.SYMBOLS, ".sdata"),
            [(0, 8, "D_003BC7D8"), (8, 8, "D_003BC7E0")],
        )

    def test_requires_exact_extent_and_retail_address(self):
        definitions = section_symbols(self.SYMBOLS, ".sdata")
        syms = {"D_003BC7D8": 0x003BC7D8, "D_003BC7E0": 0x003BC7E0}

        self.assertTrue(owns_exact_section_item(
            definitions, syms, 0, 8, 0x003BC7D8))
        self.assertFalse(owns_exact_section_item(
            definitions, syms, 0, 4, 0x003BC7D8))
        self.assertFalse(owns_exact_section_item(
            definitions, syms, 0, 8, 0x003BC7E0))
        self.assertEqual(source_owned_item_size(
            definitions, syms, 0, 0x003BC7D8), 8)
        self.assertIsNone(source_owned_item_size(
            definitions, syms, 0, 0x003BC7E0))


if __name__ == "__main__":
    unittest.main()
