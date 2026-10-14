#!/usr/bin/env python3
"""Focused tests for relocation-aware unit verification."""

import contextlib
import io
import struct
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import patch

import check_unit
from symbol_aliases import SymbolAlias

from check_unit import (
    address,
    asm_literal_users,
    bss_ownership_problems,
    bss_symbols,
    common_symbols,
    initialized_data_problems,
    nobits_ownership_problems,
    nobits_symbols,
    owned_bss_range,
    owned_bss_size,
    owned_nobits_range,
    owned_nobits_size,
    owns_exact_section_item,
    relocate_sdata_item,
    rodata_code_pointers,
    section_symbols,
    source_owned_item_size,
    symbols,
    trim_sdata_item,
    native_function_size,
    text_comparison_extent,
    text_residual_summary,
)


class TextResidualTests(unittest.TestCase):
    def test_agreeing_overrun_prefix_still_fails_the_complete_checker(self):
        compiled = struct.pack("<III", 0x03E00008, 0, 0x24020001)
        retail = struct.pack("<III", 0x03E00008, 0, 0xDEADBEEF)
        with tempfile.TemporaryDirectory() as tmp, contextlib.ExitStack() as stack:
            root = Path(tmp)
            unit = root / "src/dds1/game/unit.c"
            unit.parent.mkdir(parents=True)
            unit.write_text("void target(void) {}\n")
            config = root / "config/dds1"
            config.mkdir(parents=True)
            (config / "symbol_addrs.txt").write_text(
                "target = 0x1000; // type:func\nnext = 0x1008; // type:func\n")
            (config / "SLUS_209.74.yaml").write_text("")
            original = root / check_unit.RETAIL["dds1"]
            original.parent.mkdir(parents=True)
            original.write_bytes(retail)
            assembly = root / "asm/dds1/nonmatchings/game/unit/target.s"
            assembly.parent.mkdir(parents=True)
            assembly.write_text("nonmatching target, 0x8\n")
            stack.enter_context(patch.object(check_unit, "ROOT", root))
            stack.enter_context(patch.object(check_unit.sys, "argv", ["check_unit.py", str(unit)]))
            stack.enter_context(patch("eeas_compat.lit4_range", return_value=(0, 0)))
            stack.enter_context(patch.object(check_unit, "load_segments", return_value=[]))
            stack.enter_context(patch.object(check_unit, "va_to_off", side_effect=lambda segments, addr: addr - 0x1000))
            stack.enter_context(patch.object(check_unit, "text_section", return_value=compiled))
            stack.enter_context(patch.object(check_unit, "sections", return_value=(b"", {})))
            stack.enter_context(patch.object(check_unit, "relocations", return_value={".text": {}, ".rodata": {}, ".sdata": {}}))
            stack.enter_context(patch.object(check_unit, "run", side_effect=lambda *args: "0 c T target\n" if "-S" in args else ""))
            stack.enter_context(patch.object(check_unit.subprocess, "run", return_value=SimpleNamespace(returncode=0, stderr="")))
            output = stack.enter_context(contextlib.redirect_stdout(io.StringIO()))
            with self.assertRaises(SystemExit) as stopped:
                check_unit.main()
            self.assertEqual(stopped.exception.code, 1)
            report = output.getvalue()
            self.assertIn("OVER target", report)
            self.assertIn("text prefix: 2 matching, 0 differing words", report)
            self.assertIn("0 match, 1 differ", report)
            self.assertNotIn("DIFF target", report)
            self.assertNotIn("OK   target", report)

    def test_nonzero_overrun_compares_only_the_native_prefix(self):
        text = struct.pack("<IIII", 0xFFFFFFFF, 0x03E00008, 0, 0x24020001)
        self.assertEqual(text_comparison_extent(text, 4, 12, 0x1000, 0x1008),
                         (8, True))
        # An agreeing prefix still reports excess code, not an exact function.
        self.assertEqual(text_residual_summary(12, 8, [], 8),
                         "text prefix: 2 matching, 0 differing words; 12 emitted bytes, "
                         "8 native bytes; 0 uncovered, 4 excess bytes")

    def test_zero_padding_preserves_the_original_comparison_path(self):
        text = struct.pack("<III", 0x03E00008, 0, 0)
        self.assertEqual(text_comparison_extent(text, 0, 12, 0x1000, 0x1008),
                         (12, False))
        self.assertEqual(text_comparison_extent(text, 0, 12, 0x1000, None),
                         (12, False))

    def test_repeated_relocation_diagnostics_do_not_double_count_a_word(self):
        diffs = [(0, 1, 2, "HI16 pair one"), (0, 1, 2, "HI16 pair two"),
                 (8, 3, 4, "outside the native function")]
        self.assertEqual(text_residual_summary(12, 12, diffs, 8),
                         "text prefix: 1 matching, 1 differing words; 12 emitted bytes, "
                         "8 native bytes; 0 uncovered, 4 excess bytes")

    def test_short_candidate_reports_uncovered_native_code(self):
        diffs = [(4, 1, 2, "")]
        self.assertEqual(text_residual_summary(8, 8, diffs, 12),
                         "text prefix: 1 matching, 1 differing words; 8 emitted bytes, "
                         "12 native bytes; 4 uncovered, 0 excess bytes")

    def test_missing_or_unaligned_extent_is_not_invented(self):
        self.assertIsNone(native_function_size("nonmatching other, 0x10\n", "target"))
        self.assertIsNone(native_function_size("nonmatching target, 0x11\n", "target"))
        self.assertEqual(native_function_size("nonmatching target, 0x25C\n", "target"), 604)
        self.assertEqual(text_residual_summary(8, 8, [(0, 1, 2, "")]),
                         "text prefix: 1 matching, 1 differing words; 8 emitted bytes")


class LinkerSymbolTests(unittest.TestCase):
    def test_linker_gp_resolves_to_each_titles_retail_address(self):
        for version, expected in (("dds1", 0x003C0CF0), ("dds2", 0x0043AE70)):
            with self.subTest(version=version):
                self.assertEqual(address("_gp", symbols(version)), expected)

    def test_unprovided_linker_symbol_remains_unresolved(self):
        self.assertIsNone(address("_missing_linker_symbol", symbols("dds2")))


class AsmLiteralSharingTests(unittest.TestCase):
    def test_candidate_replaces_self_but_preserves_remaining_asm_sharing(self):
        production = '\n'.join([
            'INCLUDE_ASM(s32, "game/unit", target);',
            'INCLUDE_ASM(s32, "game/unit", other);',
        ])
        candidate = production.replace(
            'INCLUDE_ASM(s32, "game/unit", target);',
            'const char ownLiteral[] = "own";\nvoid target(void) {}',
        )
        with tempfile.TemporaryDirectory() as tmp:
            asm_dir = Path(tmp)
            (asm_dir / "target.s").write_text(
                'lui $a0, %hi(ownLiteral)\naddiu $a1, $gp, %gp_rel(D_00300010)\n'
            )
            (asm_dir / "other.s").write_text(
                'addiu $a0, $gp, %gp_rel(D_00300010)\n'
            )
            syms = {"ownLiteral": 0x00300000}
            self.assertEqual(
                asm_literal_users(production, asm_dir, syms, 0x00300000),
                ["target"],
            )
            self.assertEqual(
                asm_literal_users(candidate, asm_dir, syms, 0x00300000), [],
            )
            self.assertEqual(
                asm_literal_users(candidate, asm_dir, syms, 0x00300010),
                ["other"],
            )


class RodataCodePointerTests(unittest.TestCase):
    def test_mixed_callback_relocations_and_next_table_boundary(self):
        data = struct.pack("<IIII", 0x24, 4, 0x28, 0x2C)
        relocs = {0: ("R_MIPS_32", ".text"),
                  4: ("R_MIPS_32", "asmCallback"),
                  8: ("R_MIPS_32", ".text"),
                  12: ("R_MIPS_32", ".text")}
        funcs = [(0x20, 0x10, "localCallback")]
        syms = {"localCallback": 0x101000, "asmCallback": 0x202000}
        self.assertEqual(rodata_code_pointers(
            data, 0, 12, relocs, funcs, syms, {0x101000, 0x202000}),
            [(0, 0x101004), (4, 0x202004), (8, 0x101008)])

    def test_data_symbol_does_not_extend_code_table(self):
        data = struct.pack("<II", 0x24, 0)
        relocs = {0: ("R_MIPS_32", ".text"),
                  4: ("R_MIPS_32", "dataTable")}
        self.assertEqual(rodata_code_pointers(
            data, 0, 8, relocs, [(0x20, 0x10, "callback")],
            {"callback": 0x101000, "dataTable": 0x303000}, {0x101000}),
            [(0, 0x101004)])

    def test_unresolved_or_unsupported_code_pointer_remains_invalid(self):
        data = struct.pack("<II", 0x40, 0x24)
        relocs = {0: ("R_MIPS_32", ".text"),
                  4: ("R_MIPS_HI16", ".text")}
        self.assertEqual(rodata_code_pointers(
            data, 0, 8, relocs, [(0x20, 0x10, "callback")],
            {"callback": 0x101000}, {0x101000}), [(0, None), (4, None)])


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

    def test_explicit_native_extent_excludes_alignment_gap(self):
        emitted = [(0, 0xC, "fileManagerHead", True),
                   (0x10, 0x30, "fileManagerSlots", True)]
        syms = {"fileManagerHead": 0x003DC658,
                "fileManagerSlots": 0x003DC668}
        self.assertEqual(nobits_ownership_problems(
            self.YAML, "file/fileManager", ".bss", 0x40, emitted, syms,
            explicit_sizes={"fileManagerHead": 0xC}), [])
        self.assertTrue(nobits_ownership_problems(
            self.YAML, "file/fileManager", ".bss", 0x40, emitted, syms))

    def test_explicit_native_extent_must_fit_its_address_interval(self):
        emitted = [(0, 0x10, "fileManagerHead", True),
                   (0x10, 0x30, "fileManagerSlots", True)]
        syms = {"fileManagerHead": 0x003DC658,
                "fileManagerSlots": 0x003DC668}
        for size in (0, -1, 0x11):
            with self.subTest(size=size):
                self.assertTrue(nobits_ownership_problems(
                    self.YAML, "file/fileManager", ".bss", 0x40,
                    emitted, syms, explicit_sizes={"fileManagerHead": size}))

    def test_same_address_names_share_one_explicit_extent(self):
        emitted = [(0, 0xC, "fileManagerHead", True),
                   (0, 0xC, "headAlias", True),
                   (0x10, 0x30, "fileManagerSlots", True)]
        syms = {"fileManagerHead": 0x003DC658, "headAlias": 0x003DC658,
                "fileManagerSlots": 0x003DC668}
        self.assertEqual(nobits_ownership_problems(
            self.YAML, "file/fileManager", ".bss", 0x40, emitted, syms,
            explicit_sizes={"fileManagerHead": 0xC}), [])
        self.assertTrue(nobits_ownership_problems(
            self.YAML, "file/fileManager", ".bss", 0x40, emitted, syms,
            explicit_sizes={"fileManagerHead": 0xC, "headAlias": 0x10}))

    def test_interior_alias_cannot_enter_alignment_gap(self):
        emitted = [(0, 0xC, "fileManagerHead", True),
                   (0x10, 0x30, "fileManagerSlots", True)]
        syms = {"fileManagerHead": 0x003DC658,
                "fileManagerSlots": 0x003DC668}
        alias = SymbolAlias("gapAlias", "fileManagerHead", "file/fileManager",
                            ".bss", 0xC, 0x003DC664)
        self.assertTrue(nobits_ownership_problems(
            self.YAML, "file/fileManager", ".bss", 0x40, emitted, syms,
            (alias,), {"fileManagerHead": 0xC}))

    def test_absolute_linker_gp_does_not_split_owned_objects(self):
        emitted = [
            (0x00, 0x10, "fileManagerHead", True),
            (0x10, 0x30, "fileManagerSlots", True),
        ]
        syms = {
            "fileManagerHead": 0x003DC658,
            "fileManagerSlots": 0x003DC668,
            "_gp": 0x003DC678,
        }
        self.assertEqual(self.ownership_problems(emitted, syms), [])

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


class InitializedDataTests(unittest.TestCase):
    ORIGIN = 0x00400000
    RANGE = (0x1000, 0x1008)

    def problems(self, *, section_range=RANGE, section_origin=ORIGIN,
                 retail=b"\x01\x02\x03\x04\x05\x06\x07\x08",
                 data=b"\x01\x02\x03\x04\x05\x06\x07\x08",
                 definitions=((0, 8, "consoleUv"),), relocs=None,
                 full_data=None, full_definitions=None, full_relocs=None,
                 syms=None, section_bases=None):
        if full_data is None:
            full_data = data
        if full_definitions is None:
            full_definitions = definitions
        if relocs is None:
            relocs = {}
        if full_relocs is None:
            full_relocs = relocs
        if syms is None:
            syms = {"consoleUv": self.ORIGIN}
        return initialized_data_problems(
            section_range, section_origin, retail,
            data, list(definitions), relocs, [],
            full_data, list(full_definitions), full_relocs, [], syms,
            section_bases or {})

    def test_accepts_exact_object_and_native_payload(self):
        self.assertEqual(self.problems(), [])

    def test_resolves_native_supported_data_relocation(self):
        native_pointer = struct.pack("<I", self.ORIGIN)
        self.assertEqual(self.problems(
            retail=native_pointer + b"\0" * 4, data=b"\0" * 4,
            definitions=((0, 4, "consoleUv"),),
            relocs={0: ("R_MIPS_32", ".data")},
            full_data=b"\0" * 4, full_relocs={0: ("R_MIPS_32", ".data")},
            syms={"consoleUv": self.ORIGIN},
            section_bases={".data": self.ORIGIN}), [])

    def test_rejects_unowned_or_ambiguous_yaml_extent(self):
        self.assertTrue(any("no unambiguous YAML-owned" in p
                            for p in self.problems(section_range=None)))

    def test_rejects_unknown_or_misplaced_object(self):
        unknown = self.problems(syms={})
        self.assertTrue(any("no unambiguous native symbol" in p for p in unknown))
        misplaced = self.problems(
            definitions=((4, 4, "consoleUv"),),
            full_definitions=((4, 4, "consoleUv"),),
            data=b"\0" * 4 + b"\x05\x06\x07\x08",
            full_data=b"\0" * 4 + b"\x05\x06\x07\x08")
        self.assertTrue(any("does not match native address" in p for p in misplaced))

    def test_rejects_ambiguous_or_overlapping_object_inventory(self):
        duplicate = self.problems(
            definitions=((0, 4, "consoleUv"), (4, 4, "consoleUv")),
            full_definitions=((0, 4, "consoleUv"), (4, 4, "consoleUv")))
        self.assertTrue(any("duplicate STT_OBJECT names" in p for p in duplicate))
        overlap = self.problems(
            definitions=((0, 8, "consoleUv"), (0, 4, "otherUv")),
            full_definitions=((0, 8, "consoleUv"), (0, 4, "otherUv")),
            syms={"consoleUv": self.ORIGIN, "otherUv": self.ORIGIN})
        self.assertTrue(any("overlapping STT_OBJECT" in p for p in overlap))

    def test_rejects_overlong_or_different_payload(self):
        overlong = self.problems(
            data=b"\x01" * 12, full_data=b"\x01" * 12,
            definitions=((0, 12, "consoleUv"),),
            full_definitions=((0, 12, "consoleUv"),))
        self.assertTrue(any("exceeds its 0x8-byte" in p for p in overlong))
        changed = self.problems(data=b"\x09\x02\x03\x04\x05\x06\x07\x08")
        self.assertTrue(any("bytes/relocations differ" in p for p in changed))

    def test_rejects_unowned_bytes_and_unresolvable_relocations(self):
        unowned = self.problems(
            definitions=((0, 4, "consoleUv"),),
            full_definitions=((0, 4, "consoleUv"),),
            data=b"\x01\x02\x03\x04\x00\x00\x00\x09",
            full_data=b"\x01\x02\x03\x04\x00\x00\x00\x09")
        self.assertTrue(any("unowned nonzero byte" in p for p in unowned))
        unresolved = self.problems(
            data=b"\0" * 8,
            relocs={0: ("R_MIPS_32", "missingTable")},
            full_data=b"\0" * 8,
            full_relocs={0: ("R_MIPS_32", "missingTable")})
        self.assertTrue(any("unresolved R_MIPS_32" in p for p in unresolved))
        unsupported = self.problems(
            relocs={0: ("R_MIPS_26", "consoleUv")},
            full_relocs={0: ("R_MIPS_26", "consoleUv")})
        self.assertTrue(any("unsupported R_MIPS_26" in p for p in unsupported))

    def test_requires_same_full_unit_object_extent(self):
        problems = self.problems(
            full_definitions=((0, 4, "consoleUv"),),
            full_data=b"\x01\x02\x03\x04\0\0\0\0")
        self.assertTrue(any("STT_OBJECT inventories differ" in p for p in problems))


if __name__ == "__main__":
    unittest.main()
