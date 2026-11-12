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


class SourceReviewDiagnosticsTests(unittest.TestCase):
    def test_all_register_spellings_are_reported_locally_and_rejected_globally(self):
        for spelling in ("asm", "__asm", "__asm__"):
            declaration = f'register float speed {spelling}("$f1") = realSpeed;'
            with self.subTest(spelling=spelling):
                local = check_unit.source_review_diagnostics(f"void f(void) {{ {declaration} }}", "")
                self.assertFalse(any(fatal for fatal, _ in local))
                self.assertIn("local register binding", local[0][1])
                global_ = check_unit.source_review_diagnostics(declaration, "")
                self.assertTrue(global_[0][0])
                self.assertIn("global register binding", global_[0][1])

    def test_comments_strings_and_ordinary_registers_do_not_create_hits(self):
        source = '''
/* register float speed asm("$f1"); goto *destination; */
void f(void) {
    const char *comment = "/*";
    const char *text = "register float speed asm(\\\"$f1\\\"); goto *destination;";
    register int genuineScratch;
    // register float speed __asm__("$f1");
}
'''
        self.assertEqual(check_unit.source_review_diagnostics(source, ""), [])

    def test_quoted_comment_delimiter_does_not_hide_later_computed_goto(self):
        source = 'void f(void) { const char *text = "/*"; goto *destination; }'
        diagnostics = check_unit.source_review_diagnostics(source, "")
        self.assertEqual(len(diagnostics), 1)
        self.assertTrue(diagnostics[0][0])
        self.assertIn("computed goto", diagnostics[0][1])

    def test_label_tables_remain_fatal(self):
        diagnostics = check_unit.source_review_diagnostics(
            'void f(void) { void *labels[] = { &&one, &&two }; }', "")
        self.assertEqual(len(diagnostics), 2)
        self.assertTrue(all(fatal for fatal, _ in diagnostics))

    def test_inactive_nonmatching_draft_does_not_trigger_policy_diagnostics(self):
        source = '#ifdef NON_MATCHING\nvoid f(void) { goto *p; }\n#else\nvoid f(void) {}\n#endif\n'
        self.assertEqual(check_unit.source_review_diagnostics(source, ""), [])

    def test_diagnostics_keep_source_line_numbers_after_inactive_drafts(self):
        source = '#ifdef NON_MATCHING\nvoid f(void) { goto *p; }\n#else\nvoid f(void) { goto *q; }\n#endif\n'
        diagnostics = check_unit.source_review_diagnostics(source, "")
        self.assertEqual(diagnostics, [(True, "TRICK line 4: computed goto (write the switch)")])

    def test_empty_regions_are_diagnostics_not_semantic_approvals(self):
        assembly = '.ent f\n#APP\n# comment only\n\n#NO_APP\njr $ra\n.end f\n'
        self.assertEqual(check_unit.inline_asm_regions(assembly), {"f": (1, 0)})
        self.assertEqual(check_unit.inline_asm_share(assembly), {"f": (0, 1)})
        diagnostics = check_unit.source_review_diagnostics("void f(void) {}", assembly)
        self.assertEqual(len(diagnostics), 1)
        self.assertFalse(diagnostics[0][0])
        self.assertIn("not semantic approval", diagnostics[0][1])

    def test_raw_opcodes_and_directives_cannot_be_called_empty_constraints(self):
        source = 'void f(void) { register int value asm("$2") = input; }'
        for body in ('addiu $2,$2,1', '.word 0x24420001', '.byte 1,0,66,36', '.set noreorder'):
            with self.subTest(body=body):
                assembly = f'.ent f\n#APP\n{body}\n#NO_APP\njr $ra\n.end f\n'
                self.assertEqual(check_unit.inline_asm_regions(assembly), {"f": (0, 1)})
                messages = [message for _, message in check_unit.source_review_diagnostics(source, assembly)]
                self.assertTrue(any("not covered by the empty-constraint allowance" in message for message in messages))
                self.assertFalse(any(message.startswith("CONSTRAINT f:") for message in messages))

    def test_regions_are_kept_separate_by_function(self):
        assembly = '.ent f\n#APP\n#NO_APP\n.end f\n.ent g\n#APP\nnop\n#NO_APP\n.end g\n'
        self.assertEqual(check_unit.inline_asm_regions(assembly), {"f": (1, 0), "g": (0, 1)})


class FocusedFunctionTests(unittest.TestCase):
    def run_checker(self, requested, emitted="target", full_emitted=None, *,
                    code=None, retail_code=None, text_relocs=None, extra_symbols="",
                    source_text="void target(void) {}\n", retail_functions=(), candidate=None, assembly=None):
        """Exercise the CLI with synthetic objects, without invoking a compiler."""
        if code is None:
            code = struct.pack("<II", 0x03E00008, 0)
        with tempfile.TemporaryDirectory() as tmp, contextlib.ExitStack() as stack:
            root = Path(tmp)
            unit = root / "src/dds1/game/unit.c"
            unit.parent.mkdir(parents=True)
            unit.write_text(source_text)
            if retail_functions:
                asm = root / "asm" / "dds1/game/unit.s"
                asm.parent.mkdir(parents=True)
                asm.write_text("".join(f"glabel {name}\n" for name in retail_functions))
            config = root / "config/dds1"
            config.mkdir(parents=True)
            (config / "symbol_addrs.txt").write_text(
                "target = 0x1000; // type:func\nalias = 0x1000; // type:func\n"
                + extra_symbols)
            (config / "SLUS_209.74.yaml").write_text("")
            original = root / check_unit.RETAIL["dds1"]
            original.parent.mkdir(parents=True)
            original.write_bytes(code if retail_code is None else retail_code)
            argv = ["check_unit.py", str(unit)]
            if requested is not None:
                argv.extend(["--func", requested])
            if candidate is not None:
                alternate = root / "candidate.c"
                alternate.write_text(candidate)
                argv.extend(["--source", str(alternate)])
            stack.enter_context(patch.object(check_unit, "ROOT", root))
            stack.enter_context(patch.object(check_unit.sys, "argv", argv))
            stack.enter_context(patch("eeas_compat.lit4_range", return_value=(0, 0)))
            stack.enter_context(patch.object(check_unit, "load_segments", return_value=[]))
            stack.enter_context(patch.object(check_unit, "va_to_off", side_effect=lambda segments, addr: addr - 0x1000))
            stack.enter_context(patch.object(check_unit, "text_section", return_value=code))
            stack.enter_context(patch.object(check_unit, "sections", return_value=(b"", {})))
            stack.enter_context(patch.object(check_unit, "relocations", return_value={".text": text_relocs or {}, ".rodata": {}, ".sdata": {}}))

            def object_output(*args):
                if "-S" not in args:
                    return ""
                names = full_emitted if str(args[-1]).endswith("full.o") and full_emitted is not None else emitted
                return "".join(f"0 {len(code):x} T {name}\n" for name in names.split())

            stack.enter_context(patch.object(check_unit, "run", side_effect=object_output))
            def compile_output(*args, **kwargs):
                keep = kwargs.get("env", {}).get("DDS_KEEP_S")
                if keep and assembly is not None:
                    Path(keep).write_text(assembly)
                return SimpleNamespace(returncode=0, stderr="")

            stack.enter_context(patch.object(check_unit.subprocess, "run", side_effect=compile_output))
            output = stack.enter_context(contextlib.redirect_stdout(io.StringIO()))
            with self.assertRaises(SystemExit) as stopped:
                check_unit.main()
            return stopped.exception.code, output.getvalue()

    def test_local_binding_reports_review_without_rejecting_exact_bytes(self):
        for spelling in ("asm", "__asm", "__asm__"):
            with self.subTest(spelling=spelling):
                source = f'void target(void) {{ register float speed {spelling}("$f1") = realSpeed; }}\n'
                status, report = self.run_checker(None, source_text=source)
                self.assertEqual(status, 0)
                self.assertIn("CONSTRAINT line 1: local register binding", report)
                self.assertIn("not semantic approval", report)
                self.assertNotIn("TRICK", report)

    def test_constraint_does_not_suppress_instruction_mismatch(self):
        source = 'void target(void) { register float speed __asm__("$f1") = realSpeed; }\n'
        status, report = self.run_checker(
            None, source_text=source, retail_code=struct.pack("<II", 0x03E00008, 1))
        self.assertEqual(status, 1)
        self.assertIn("DIFF target", report)
        self.assertIn("CONSTRAINT", report)

    def test_alternate_source_gets_the_same_policy_scan_as_the_compiler(self):
        status, report = self.run_checker(None, candidate="void target(void) { goto *destination; }\n")
        self.assertEqual(status, 1)
        self.assertIn("TRICK line 1: computed goto", report)

    def test_global_binding_is_not_a_local_constraint(self):
        source = 'register float speed __asm__("$f1");\nvoid target(void) {}\n'
        status, report = self.run_checker(None, source_text=source)
        self.assertEqual(status, 1)
        self.assertIn("global register binding", report)

    def test_empty_constraint_and_instruction_body_diagnostics_remain_distinct(self):
        source = 'void target(void) { register float speed asm("$f1") = realSpeed; }\n'
        assembly = ".ent target\n#APP\n#NO_APP\n#APP\n" + "nop\n" * 8 + "#NO_APP\njr $ra\n.end target\n"
        status, report = self.run_checker(None, source_text=source, assembly=assembly)
        self.assertEqual(status, 0)  # ASMBODY remains informational, as before.
        self.assertIn("CONSTRAINT target: 1 empty inline-asm region", report)
        self.assertIn("INLINEASM target: 1 nonempty inline-asm region", report)
        self.assertIn("ASMBODY target: 8 of 9 instructions", report)


    def test_c_assembler_name_satisfies_native_presence(self):
        status, report = self.run_checker(
            None, source_text='void entry(void) asm("target");\nvoid entry(void) {}\n',
            retail_functions=("target",))
        self.assertEqual(status, 0, report)
        self.assertNotIn("MISSING", report)

    def test_assembler_prototype_alone_cannot_satisfy_presence(self):
        status, report = self.run_checker(
            None, emitted="", source_text='void entry(void) asm("target");\n',
            retail_functions=("target",))
        self.assertEqual(status, 1)
        self.assertIn("MISSING target", report)

    def test_wrong_assembler_name_cannot_satisfy_presence(self):
        status, report = self.run_checker(
            None, source_text='void entry(void) asm("alias");\nvoid entry(void) {}\n',
            retail_functions=("target",))
        self.assertEqual(status, 1)
        self.assertIn("MISSING target", report)

    def test_missing_requested_name_fails_even_at_an_emitted_address(self):
        for requested in ("typo", "func_00001000", "alias"):
            with self.subTest(requested=requested):
                status, report = self.run_checker(requested)
                self.assertEqual(status, 1)
                self.assertIn(f"MISSING {requested}: requested function not emitted", report)
                self.assertIn("0 match, 1 differ", report)

    def test_empty_skip_asm_object_cannot_pass_a_focused_check(self):
        status, report = self.run_checker("target", emitted="")
        self.assertEqual(status, 1)
        self.assertIn("MISSING target", report)

    def test_asm_only_function_does_not_count_as_emitted_c(self):
        status, report = self.run_checker("target", emitted="", full_emitted="target")
        self.assertEqual(status, 1)
        self.assertIn("0 match, 1 differ", report)

    def test_present_requested_name_and_emitted_alias_still_pass(self):
        for requested in ("target", "alias"):
            with self.subTest(requested=requested):
                status, report = self.run_checker(requested, emitted="target alias")
                self.assertEqual(status, 0)
                self.assertIn(f"OK   {requested}", report)
                self.assertIn("1 match, 0 differ", report)

    def test_whole_unit_keeps_existing_empty_and_nonempty_behavior(self):
        for emitted, matches in (("", 0), ("target", 1), ("target alias", 2)):
            with self.subTest(emitted=emitted):
                status, report = self.run_checker(None, emitted=emitted)
                self.assertEqual(status, 0)
                self.assertIn(f"{matches} match, 0 differ", report)


    def test_backedge_hi16_uses_elf_relocation_order_and_shared_low(self):
        # ee-as puts both HI16 entries before their shared LO16, although the
        # second LUI lives after the ADDIU in the function's instruction stream.
        compiled = struct.pack("<III", 0x3C020000, 0x244216F0, 0x3C020000)
        retail = struct.pack("<III", 0x3C020030, 0x24428170, 0x3C020030)
        relocs = {0: ("R_MIPS_HI16", "drawBank"),
                  8: ("R_MIPS_HI16", "drawBank"),
                  4: ("R_MIPS_LO16", "drawBank")}
        status, report = self.run_checker(
            "target", code=compiled, retail_code=retail, text_relocs=relocs,
            extra_symbols="drawBank = 0x002F6A80;\n")
        self.assertEqual(status, 0)
        self.assertIn("1 match, 0 differ", report)

    def test_backedge_hi16_still_rejects_an_incorrect_high_field(self):
        compiled = struct.pack("<III", 0x3C020000, 0x244216F0, 0x3C020000)
        retail = struct.pack("<III", 0x3C020030, 0x24428170, 0x3C02002F)
        relocs = {0: ("R_MIPS_HI16", "drawBank"),
                  8: ("R_MIPS_HI16", "drawBank"),
                  4: ("R_MIPS_LO16", "drawBank")}
        status, report = self.run_checker(
            "target", code=compiled, retail_code=retail, text_relocs=relocs,
            extra_symbols="drawBank = 0x002F6A80;\n")
        self.assertEqual(status, 1)
        self.assertIn("DIFF target", report)
        self.assertIn("0 match, 1 differ", report)


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


class FocusedRodataTests(unittest.TestCase):
    def check_tables(self, *, func=None, wrong_entry=False, extra_entry=False,
                     neighbor_as_built=False):
        # Skipping an ASM function packs these C tables together in the object,
        # but retail keeps an unrelated table between them. The selected table
        # must end at the next compiled reference, not the end of .rodata or
        # a guessed retail length.
        entries = [0, 4, 8, 0 if wrong_entry else 12]
        if extra_entry:
            entries.append(8)
        neighbor_offset = len(entries) * 4
        entries.extend([16, 20, 24, 28])
        rodata = struct.pack(f"<{len(entries)}I", *entries)
        compiled = struct.pack("<8I", 0x3C020000, 0x24420000, 0x03E00008, 0,
                               0x3C020000, 0x24420000 + neighbor_offset,
                               0x03E00008, 0)
        full_text = bytearray(compiled)
        if neighbor_as_built:
            struct.pack_into("<I", full_text, 28, 0x24020001)
        retail = bytearray(0x1040)
        struct.pack_into("<4I", retail, 0, 0x3C020000, 0x24422000, 0x03E00008, 0)
        struct.pack_into("<4I", retail, 0x40, 0x3C020000, 0x24422020,
                         0x03E00008, 0x24020001 if neighbor_as_built else 0)
        struct.pack_into("<12I", retail, 0x1000,
                         0x1000, 0x1004, 0x1008, 0x100C,
                         *([0x12345678] * 4),
                         0x1040, 0x1044, 0x1048, 0x104C)
        relocs = {
            ".text": {0: ("R_MIPS_HI16", ".rodata"),
                      4: ("R_MIPS_LO16", ".rodata"),
                      16: ("R_MIPS_HI16", ".rodata"),
                      20: ("R_MIPS_LO16", ".rodata")},
            ".rodata": {off: ("R_MIPS_32", ".text")
                        for off in range(0, len(rodata), 4)},
            ".sdata": {}, ".data": {},
        }
        with tempfile.TemporaryDirectory() as tmp, contextlib.ExitStack() as stack:
            root = Path(tmp)
            unit = root / "src/dds1/game/unit.c"
            unit.parent.mkdir(parents=True)
            unit.write_text("void target(void) {}\nvoid neighbor(void) {}\n")
            config = root / "config/dds1"
            config.mkdir(parents=True)
            (config / "symbol_addrs.txt").write_text(
                "target = 0x1000; // type:func\nneighbor = 0x1040; // type:func\n")
            (config / "SLUS_209.74.yaml").write_text("")
            original = root / check_unit.RETAIL["dds1"]
            original.parent.mkdir(parents=True)
            original.write_bytes(retail)
            assembly = root / "asm/dds1/data/game/unit.rodata.s"
            assembly.parent.mkdir(parents=True)
            assembly.write_text("dlabel jtbl_00002000\ndlabel jtbl_00002010\n"
                                "dlabel jtbl_00002020\n")
            argv = ["check_unit.py", str(unit)]
            if func:
                argv.extend(["--func", func])
            stack.enter_context(patch.object(check_unit, "ROOT", root))
            stack.enter_context(patch.object(check_unit.sys, "argv", argv))
            stack.enter_context(patch("eeas_compat.lit4_range", return_value=(0, 0)))
            stack.enter_context(patch.object(check_unit, "load_segments", return_value=[]))
            stack.enter_context(patch.object(check_unit, "va_to_off", side_effect=lambda segments, addr: addr - 0x1000))
            stack.enter_context(patch.object(check_unit, "text_section", side_effect=lambda obj: full_text if obj.name == "full.o" else compiled))
            stack.enter_context(patch.object(check_unit, "sections", return_value=(rodata, {".rodata": (0, len(rodata))})))
            stack.enter_context(patch.object(check_unit, "relocations", return_value=relocs))
            stack.enter_context(patch.object(check_unit, "owns_rodata", return_value=True))
            stack.enter_context(patch.object(check_unit, "run", side_effect=lambda *args: "0 10 T target\n10 10 T neighbor\n" if "-S" in args else ""))
            stack.enter_context(patch.object(check_unit.subprocess, "run", return_value=SimpleNamespace(returncode=0, stderr="")))
            output = stack.enter_context(contextlib.redirect_stdout(io.StringIO()))
            with self.assertRaises(SystemExit) as stopped:
                check_unit.main()
            return stopped.exception.code, output.getvalue()

    def test_focus_preserves_adjacent_c_table_boundaries(self):
        for func in (None, "target", "neighbor"):
            with self.subTest(func=func):
                code, report = self.check_tables(func=func)
                self.assertEqual(code, 0, report)
                self.assertIn(f"{1 if func else 2} match, 0 differ", report)
                if func == "target":
                    self.assertNotIn("neighbor", report)

    def test_filtered_as_built_neighbor_still_ends_the_table(self):
        for func in (None, "target"):
            with self.subTest(func=func):
                code, report = self.check_tables(func=func, neighbor_as_built=True)
                self.assertEqual(code, 0, report)

    def test_focus_still_rejects_incorrect_selected_table_entry(self):
        code, report = self.check_tables(func="target", wrong_entry=True)
        self.assertEqual(code, 1, report)
        self.assertIn("DIFF jump table of target", report)
        self.assertIn("1 of 4 entries differ", report)

    def test_focus_does_not_truncate_excess_entries_to_retail_length(self):
        code, report = self.check_tables(func="target", extra_entry=True)
        self.assertEqual(code, 1, report)
        self.assertIn("DIFF jump table of target", report)
        self.assertIn("1 of 5 entries differ", report)


class SharedRodataCLITests(unittest.TestCase):
    def check_shared(self, *, anonymous=False, local=False, duplicate=False,
                     bad_skip=False, bad_full=False, size=16, full_failed=False,
                     retained_label=False, func=None, padding=False, merged=False,
                     pointer=False, duplicate_reloc=False, conflicting_metadata="",
                     interior=False, raw_proof=False, wrong_delta=False, zero_tail=False):

        payload = b"0123456789abcdef" if not zero_tail else b"01234567" + b"\0" * 8
        code = struct.pack("<4I", 0x3C020000, 0x24420000, 0x03E00008, 0)
        if merged or interior:
            code *= 2
        if interior:
            code = code[:20] + struct.pack("<I", 0x24420008) + code[24:]
        retail = bytearray(0x2040)
        struct.pack_into("<4I", retail, 0x1000, 0x3C020010, 0x24421000, 0x03E00008, 0)
        native_data = payload + (struct.pack("<II", 0x1111, 0) if pointer else b"padding!" if padding else payload if merged else b"")
        retail[0x2000:0x2000 + len(native_data)] = native_data
        if interior:
            struct.pack_into("<4I", retail, 0x1010, 0x3C020010,
                             0x24421004 if wrong_delta else 0x24421008, 0x03E00008, 0)
        if merged:
            struct.pack_into("<4I", retail, 0x1010, 0x3C020010, 0x24421010, 0x03E00008, 0)
        skip_data = (b"!" + payload[1:]) if bad_skip else payload
        full_data = ((b"!" + payload[1:]) if bad_full else payload) + (payload if duplicate else b"padding!" if padding else payload if merged else b"")
        relocs = {".text": {0: ("R_MIPS_HI16", ".rodata"), 4: ("R_MIPS_LO16", ".rodata")},
                  ".rodata": {}, ".data": {}, ".sdata": {}}
        if pointer:
            skip_data += b"\0" * 8
            full_data += b"\0" * 8
            relocs[".rodata"] = {16: ("R_MIPS_32", "external")}
        if merged or interior:
            relocs[".text"].update({16: ("R_MIPS_HI16", ".rodata"), 20: ("R_MIPS_LO16", ".rodata")})
        with tempfile.TemporaryDirectory() as tmp, contextlib.ExitStack() as stack:
            root = Path(tmp)
            unit = root / "src/dds1/game/unit.c"
            unit.parent.mkdir(parents=True)
            unit.write_text(("extern const char table[16];\n" if anonymous else
                             'const char table[16] = "0123456789abcdef";\n') +
                            'void target(void) {}\nINCLUDE_ASM(const s32, "game/unit", consumer);\n')
            config = root / "config/dds1"
            config.mkdir(parents=True)
            (config / "symbol_addrs.txt").write_text("target = 0x100000; // type:func\ntable = 0x101000;" +
                (" // size:0x10" if padding or pointer else "") + "\n" +
                ("neighbor = 0x100010; // type:func\nother = 0x101010;\n" if merged else "") +
                ("external = 0x1111;\nfirst = 0x2222;\n" if pointer else "") + conflicting_metadata)
            (config / "SLUS_209.74.yaml").write_text(
                f"  - [0x2000, .rodata, game/unit]\n  - [0x{0x2000 + len(native_data):X}, .rodata, game/next]\n")
            original = root / check_unit.RETAIL["dds1"]
            original.parent.mkdir(parents=True)
            original.write_bytes(retail)
            assembly = root / "asm/dds1/data/game/unit.rodata.s"
            assembly.parent.mkdir(parents=True)
            assembly.write_text("dlabel table\n" + ("dlabel other\n" if merged else ""))
            consumer = root / "asm/dds1/nonmatchings/game/unit/consumer.s"
            consumer.parent.mkdir(parents=True)
            consumer.write_text(("dlabel table\n.word 0\n" if retained_label else "") +
                                "glabel consumer\nlui $2, %hi(table)\naddiu $2, $2, %lo(table)\n")
            argv = ["check_unit.py", str(unit)] + (["--func", func] if func else [])
            stack.enter_context(patch.object(check_unit, "ROOT", root))
            stack.enter_context(patch.object(check_unit.sys, "argv", argv))
            stack.enter_context(patch("eeas_compat.lit4_range", return_value=(0, 0)))
            stack.enter_context(patch.object(check_unit, "load_segments", return_value=[]))
            stack.enter_context(patch.object(check_unit, "va_to_off", side_effect=lambda segments, addr: addr - 0xFF000))
            stack.enter_context(patch.object(check_unit, "off_to_va", side_effect=lambda segments, off: off + 0xFF000))
            stack.enter_context(patch.object(check_unit, "text_section", return_value=code))
            stack.enter_context(patch.object(check_unit, "sections", side_effect=lambda obj:
                (full_data, {".rodata": (0, len(full_data))}) if obj.name == "full.o" else
                (skip_data, {".rodata": (0, len(skip_data))})))
            original_relocations = check_unit.relocations
            stack.enter_context(patch.object(check_unit, "relocations", side_effect=lambda obj:
                original_relocations(obj) if pointer else relocs))
            stack.enter_context(patch.object(check_unit, "owns_rodata", return_value=True))
            def output(*args):
                if "-S" in args:
                    return ("0 20 T target\n" if interior else "0 10 T target\n") + ("10 10 T neighbor\n" if merged else "")
                if "-r" in args and pointer:
                    return ("RELOCATION RECORDS FOR [.text]:\n"
                            "00000000 R_MIPS_HI16 .rodata\n00000004 R_MIPS_LO16 .rodata\n"
                            "RELOCATION RECORDS FOR [.rodata]:\n" +
                            ("00000010 R_MIPS_32 first\n" if duplicate_reloc and str(args[-1]).endswith("full.o") else "") +
                            "00000010 R_MIPS_32 external\n")
                if "-t" in args and not anonymous:
                    return f"00000000 {'l' if local else 'g'} O .rodata {size:08x} table\n"
                return ""
            stack.enter_context(patch.object(check_unit, "run", side_effect=output))
            def compile_stub(args, **kwargs):
                if raw_proof:
                    Path(args[-1]).write_bytes(b"synthetic ELF; raw parser tested separately")
                return SimpleNamespace(returncode=int(full_failed and str(args[-1]).endswith("full.o")), stderr="")
            if raw_proof:
                stack.enter_context(patch.object(check_unit, "raw_metadata_clear", return_value=True))
            stack.enter_context(patch.object(check_unit.subprocess, "run", side_effect=compile_stub))
            report = stack.enter_context(contextlib.redirect_stdout(io.StringIO()))
            with self.assertRaises(SystemExit) as stopped:
                check_unit.main()
            return stopped.exception.code, report.getvalue()

    def test_named_shared_owner_passes_full_and_focused_checks(self):
        for func in (None, "target"):
            status, report = self.check_shared(func=func)
            self.assertEqual(status, 0, report)
            self.assertIn("1 match, 0 differ", report)

    def test_literals_local_identity_and_retained_blobs_still_report_shared(self):
        for change in (dict(anonymous=True), dict(local=True), dict(retained_label=True)):
            with self.subTest(change=change):
                status, report = self.check_shared(**change)
                self.assertEqual(status, 1, report)
                self.assertIn("SHARED rodata", report)

    def test_bad_payload_size_and_missing_production_context_remain_failures(self):
        for change in (dict(bad_skip=True), dict(bad_full=True), dict(size=8), dict(full_failed=True)):
            with self.subTest(change=change):
                status, report = self.check_shared(**change)
                self.assertEqual(status, 1, report)

    def test_genuine_duplicates_fail_for_named_and_anonymous_objects(self):
        for anonymous in (False, True):
            for func in (None, "target"):
                with self.subTest(anonymous=anonymous, func=func):
                    status, report = self.check_shared(anonymous=anonymous, duplicate=True, func=func)
                    self.assertEqual(status, 1, report)
                    self.assertIn("SHARED rodata", report)

    def test_raw_duplicate_relocation_rows_do_not_disappear_into_dictionary(self):
        status, report = self.check_shared(pointer=True)
        self.assertEqual(status, 0, report)
        for func in (None, "target"):
            status, report = self.check_shared(pointer=True, duplicate_reloc=True, func=func)
            self.assertEqual(status, 1, report)
            self.assertIn("SHARED rodata", report)

    def test_conflicting_native_metadata_cannot_be_overwritten_by_dictionary(self):
        for row in ("table = 0x101008;\n", "table = 0x101000; // size:8\n"):
            with self.subTest(row=row):
                status, report = self.check_shared(pointer=True, conflicting_metadata=row)
                self.assertEqual(status, 1, report)
                self.assertIn("SHARED rodata", report)
        status, report = self.check_shared(pointer=True, conflicting_metadata="table = 0x101000; // size:0x10\n")
        self.assertEqual(status, 0, report)

    def test_named_owner_does_not_bypass_padding_gate(self):
        status, report = self.check_shared(padding=True)
        self.assertEqual(status, 1, report)
        self.assertIn("PAD rodata", report)

    def test_named_owner_does_not_bypass_merged_native_copies_gate(self):
        status, report = self.check_shared(merged=True)
        self.assertEqual(status, 1, report)
        self.assertIn("MERGED rodata", report)



    def test_proven_interior_reference_keeps_complete_extent_full_and_focused(self):
        for func in (None, "target"):
            for zero_tail in (False, True):
                status, report = self.check_shared(interior=True, raw_proof=True, func=func, zero_tail=zero_tail)
                self.assertEqual(status, 0, report)

    def test_interior_without_raw_proof_retains_old_padding_rejection(self):
        status, report = self.check_shared(interior=True)
        self.assertEqual(status, 1, report)
        self.assertIn("PAD rodata", report)

    def test_interior_wrong_native_delta_fails_explicitly(self):
        status, report = self.check_shared(interior=True, raw_proof=True, wrong_delta=True)
        self.assertEqual(status, 1, report)
        self.assertIn("ADDRESS rodata", report)

    def test_extent_correction_preserves_payload_padding_and_ownership_failures(self):
        for change in (dict(bad_skip=True), dict(bad_full=True), dict(size=8),
                       dict(full_failed=True), dict(anonymous=True), dict(local=True),
                       dict(padding=True), dict(duplicate=True), dict(retained_label=True)):
            with self.subTest(change=change):
                status, report = self.check_shared(interior=True, raw_proof=True, **change)
                self.assertEqual(status, 1, report)


class RodataAttributionTests(unittest.TestCase):
    def classify(self, tables, *, bounds=(), objects=None, sites=None, native=None):
        from rodata_attribution import attribute_rodata_references
        if objects is None:
            objects = [(8, 24, 0x2008, 'array')]
        if sites is None:
            sites = {8: ('array', 8, 16, -4, 'f', 0, 6, 2)}
        if native is None:
            native = struct.pack('<III', 0x3c060000, 0x00c23021, 0x8cc62004)
        return attribute_rodata_references(tables, bounds, objects, sites, lambda name: native)

    def test_interior_references_normalize_only_equal_positive_delta(self):
        tables, bounds, merged, errors = self.classify([(8, 0x2008, 'f', 40), (16, 0x2010, 'f', 48)],
                                                     bounds=[(20, 'other', 60)])
        self.assertEqual(tables, [(8, 0x2008, 'f'), (8, 0x2008, 'f')])
        self.assertEqual(bounds, [8])
        self.assertFalse(errors)
        self.assertEqual(merged, {8: {0x2008: 'f'}, 16: {0x2010: 'f'}})

    def test_wrong_interior_offset_fails_even_when_payload_might_repeat(self):
        tables, _, _, errors = self.classify([(16, 0x200c, 'f', 48)])
        self.assertEqual(tables, [(16, 0x200c, 'f')])
        self.assertIn('wrong native offset', errors[0])

    def test_prebias_has_its_own_site_and_does_not_claim_compacted_storage(self):
        tables, bounds, merged, errors = self.classify([(4, 0x2004, 'f', 8)], bounds=[(4, 'f', 8)])
        self.assertEqual(tables, [(8, 0x2008, 'f')])
        self.assertEqual(bounds, [8])
        self.assertEqual(merged, {})
        self.assertFalse(errors)

    def test_real_table_same_numeric_target_cannot_borrow_site_proof(self):
        tables, _, merged, errors = self.classify([(4, 0x2004, 'f', 8), (4, 0x2004, 'f', 40)])
        self.assertIn((4, 0x2004, 'f'), tables)
        self.assertIn((8, 0x2008, 'f'), tables)
        self.assertEqual(merged, {4: {0x2004: 'f'}})
        self.assertFalse(errors)

    def test_genuine_merged_storage_identities_remain_visible(self):
        _, _, merged, errors = self.classify([(4, 0x2004, 'f', 40), (4, 0x3004, 'g', 80)])
        self.assertEqual(set(merged[4]), {0x2004, 0x3004})
        self.assertFalse(errors)

    def test_unproved_objects_and_missing_site_evidence_do_not_normalize(self):
        for changes in ({'objects': []}, {'sites': {}},
                        {'objects': [(8, 24, 0x2008, 'different')]},
                        {'objects': [(8, 20, 0x2008, 'array')]}):
            with self.subTest(changes=changes):
                tables, _, merged, errors = self.classify([(4, 0x2004, 'f', 8)], **changes)
                self.assertEqual(tables, [(4, 0x2004, 'f')])
                self.assertIn(4, merged)

    def test_wrong_native_prebias_arithmetic_is_explicit_failure(self):
        for native in (b'', struct.pack('<III', 0x3c060000, 0x00c23021, 0x8cc62000),
                       struct.pack('<III', 0x3c060000, 0x00c33021, 0x8cc62004)):
            tables, _, _, errors = self.classify([(4, 0x2004, 'f', 8)], native=native)
            self.assertEqual(tables, [(4, 0x2004, 'f')])
            self.assertIn('wrong native instruction/address', errors[0])

    def test_end_and_arbitrary_nearby_addresses_are_not_interior_objects(self):
        tables, _, _, errors = self.classify([(24, 0x2018, 'f', 40), (4, 0x2004, 'f', 44)], sites={})
        self.assertEqual(tables, [(24, 0x2018, 'f'), (4, 0x2004, 'f')])
        self.assertFalse(errors)

    def test_other_function_cannot_borrow_provenance(self):
        tables, _, _, errors = self.classify([(4, 0x2004, 'other', 8)])
        self.assertEqual(tables, [(4, 0x2004, 'other')])
        self.assertFalse(errors)

    def test_multiple_reference_sites_remain_separate_before_storage_dedup(self):
        fact = ('array', 8, 16, -4, 'f', 0, 6, 2)
        tables, _, _, errors = self.classify([(4, 0x2004, 'f', 8), (4, 0x2004, 'f', 40)],
                                           sites={8: fact, 40: fact})
        self.assertEqual(tables, [(8, 0x2008, 'f'), (8, 0x2008, 'f')])
        self.assertFalse(errors)


class SharedRodataOwnershipTests(unittest.TestCase):
    def evidence(self):
        return dict(
            skip=("00000000 g O .rodata 00000008 sharedTable\n", b"abcdefgh", {}),
            full=("00000008 g O .rodata 00000008 sharedTable\n", b"prefix!!abcdefgh", {}),
            retail=b"prefix!!abcdefgh", origin=0x2000,
            syms={"sharedTable": 0x2008}, sizes={},
            labels={"prefix": 0x2000, "sharedTable": 0x2008, "(end)": 0x2010},
            source_text="const char sharedTable[8] = {1};\n",
            included_assembly=["glabel consumer\nlui $2, %hi(sharedTable)\n"])

    def check(self, **changes):
        evidence = self.evidence()
        evidence.update(changes)
        return check_unit.shared_rodata_owners(**evidence)

    def test_named_shared_c_object_qualifies(self):
        self.assertEqual(self.check(), {(0, 0x2008)})

    def test_source_macro_alias_preserves_native_object_identity(self):
        self.assertEqual(self.check(source_text="#define table sharedTable\nconst char table[8] = {1};\n"),
                         {(0, 0x2008)})

    def test_anonymous_literal_cannot_claim_shared_native_object(self):
        self.assertEqual(self.check(skip=("00000000 l .rodata 00000000 $LC0\n", b"abcdefgh", {})), set())

    def test_static_local_and_compiler_suffix_are_not_global_identity(self):
        for row in ("00000000 l O .rodata 00000008 sharedTable\n",
                    "00000000 g O .rodata 00000008 sharedTable.0\n"):
            with self.subTest(row=row):
                self.assertEqual(self.check(skip=(row, b"abcdefgh", {})), set())

    def test_duplicate_object_alias_and_zero_size_label_fail_closed(self):
        for context in ("skip", "full"):
            dump, data, relocs = self.evidence()[context]
            offset = 0 if context == "skip" else 8
            for extra in (f"{offset:08x} g O .rodata 00000008 alias\n",
                          f"{offset:08x} g .rodata 00000000 alias\n",
                          dump):
                with self.subTest(context=context, extra=extra):
                    self.assertEqual(self.check(**{context: (dump + extra, data, relocs)}), set())

    def test_retained_fallback_native_blob_and_interior_alias_are_rejected(self):
        for name in ("sharedTable", "D_00002008", "D_0000200C"):
            with self.subTest(name=name):
                self.assertEqual(self.check(included_assembly=[f"dlabel {name}\n.word 1\n"]), set())

    def test_unavailable_assembly_evidence_is_not_permission(self):
        self.assertEqual(self.check(included_assembly=[None]), set())

    def test_missing_definition_or_competing_source_identity_is_rejected(self):
        for source in ("extern const char sharedTable[8];\n",
                       "const char sharedTable[8] = {1};\n" * 2):
            self.assertEqual(self.check(source_text=source), set())

    def test_wrong_native_address_and_missing_native_label_are_rejected(self):
        for changes in (dict(syms={"sharedTable": 0x2004}), dict(labels={"(end)": 0x2010}),
                        dict(origin=None), dict(labels={"sharedTable": 0x2008})):
            with self.subTest(changes=changes):
                self.assertEqual(self.check(**changes), set())

    def test_explicit_size_must_agree_and_cannot_cross_native_boundary(self):
        self.assertEqual(self.check(sizes={"sharedTable": 8}), {(0, 0x2008)})
        self.assertEqual(self.check(sizes={"sharedTable": 4}), set())
        labels = dict(self.evidence()["labels"], interior=0x200C)
        self.assertEqual(self.check(sizes={"sharedTable": 8}, labels=labels), set())

    def test_ambiguous_unbounded_extent_requires_truthful_explicit_size(self):
        labels = {"sharedTable": 0x2008, "(end)": 0x2018}
        self.assertEqual(self.check(labels=labels, retail=b"prefix!!abcdefgh" + b"\0" * 8), set())
        dump, data, relocs = self.evidence()["full"]
        self.assertEqual(self.check(labels=labels, retail=data + b"\0" * 8,
                                    full=(dump, data + b"\0" * 8, relocs),
                                    sizes={"sharedTable": 8}), {(0, 0x2008)})

    def test_wrong_object_size_or_production_offset_is_rejected(self):
        for context in ("skip", "full"):
            dump, data, relocs = self.evidence()[context]
            self.assertEqual(self.check(**{context: (dump.replace("00000008 shared", "00000004 shared"), data, relocs)}), set())
        dump, data, relocs = self.evidence()["full"]
        self.assertEqual(self.check(full=(dump.replace("00000008 g", "00000004 g"), data, relocs)), set())

    def test_bad_payload_and_truncation_in_either_context_are_rejected(self):
        for context in ("skip", "full"):
            dump, data, relocs = self.evidence()[context]
            for changed in (data[:-1], data[:-1] + b"!"):
                with self.subTest(context=context, changed=changed):
                    self.assertEqual(self.check(**{context: (dump, changed, relocs)}), set())

    def test_relocated_object_is_not_covered_by_scalar_ownership_rule(self):
        for context, offset in (("skip", 0), ("full", 8)):
            dump, data, _ = self.evidence()[context]
            self.assertEqual(self.check(**{context: (dump, data, {offset: ("R_MIPS_32", "external")})}), set())

    def test_competing_native_base_label_and_overlapping_declared_extent_fail(self):
        evidence = self.evidence()
        self.assertEqual(self.check(labels=dict(evidence["labels"], competing=0x2008)), set())
        self.assertEqual(self.check(syms=dict(evidence["syms"], prefix=0x2000), sizes={"prefix": 16}), set())

    def test_full_span_relocations_must_resolve_to_exact_native_bytes(self):
        dump, data, _ = self.evidence()["full"]
        full = struct.pack("<I", 4) + data[4:]
        retail = struct.pack("<I", 0x3004) + data[4:]
        syms = dict(self.evidence()["syms"], external=0x3000)
        relocs = {0: ("R_MIPS_32", "external")}
        self.assertEqual(self.check(full=(dump, full, relocs), retail=retail, syms=syms), {(0, 0x2008)})
        for target in ("unresolved", ".text", "external+0x4"):
            self.assertEqual(self.check(full=(dump, full, {0: ("R_MIPS_32", target)}), retail=retail, syms=syms), set())
        self.assertEqual(self.check(full=(dump, full, {0: ("R_MIPS_64", "external")}), retail=retail, syms=syms), set())
        self.assertEqual(self.check(full=(dump, full, relocs), syms=syms), set())

    def test_full_span_code_pointers_require_unique_function_ownership(self):
        dump, data, _ = self.evidence()["full"]
        full = struct.pack("<I", 4) + data[4:]
        retail = struct.pack("<I", 0x3004) + data[4:]
        syms = dict(self.evidence()["syms"], function=0x3000)
        relocs = {0: ("R_MIPS_32", ".text")}
        function = "00000000 g F .text 00000008 function\n"
        self.assertEqual(self.check(full=(dump + function, full, relocs), retail=retail, syms=syms), {(0, 0x2008)})
        self.assertEqual(self.check(full=(dump + function * 2, full, relocs), retail=retail, syms=syms), set())
        self.assertEqual(self.check(full=(dump + function, full, {0: ("R_MIPS_32", ".text"), 2: ("R_MIPS_32", "external")}),
                                    retail=retail, syms=syms), set())

    def test_partial_relocation_overlap_is_rejected(self):
        dump, data, _ = self.evidence()["full"]
        self.assertEqual(self.check(full=(dump, data, {6: ("R_MIPS_32", "external")})), set())

    def test_full_span_must_include_zero_tail_and_cannot_append_zero_padding(self):
        dump, data, relocs = self.evidence()["full"]
        self.assertEqual(self.check(retail=data + b"\0" * 8), set())
        self.assertEqual(self.check(full=(dump, data + b"\0" * 8, relocs)), set())

    def test_full_section_duplicate_payload_or_missing_nonzero_tail_is_rejected(self):
        dump, data, relocs = self.evidence()["full"]
        self.assertEqual(self.check(full=(dump, data + b"abcdefgh", relocs)), set())
        self.assertEqual(self.check(retail=self.evidence()["retail"] + b"tail"), set())
        self.assertEqual(self.check(full=(dump, b"wrong!!!" + data[8:], relocs)), set())


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
            "file/fileManager", "sdf/sdfSystem"
        )
        self.assertEqual(
            owned_nobits_range(sbss, "sdf/sdfSystem", ".sbss"),
            (0x003DC658, 0x40),
        )
        self.assertEqual(
            owned_nobits_size(sbss, "sdf/sdfSystem", ".sbss"), 0x40
        )
        emitted = [(0x00, 0x40, "sdfTrackedThreadState", True)]
        self.assertEqual(
            nobits_ownership_problems(
                sbss, "sdf/sdfSystem", ".sbss", 0x40, emitted,
                {"sdfTrackedThreadState": 0x003DC658},
            ),
            [],
        )
        self.assertIsNone(
            owned_nobits_size(sbss, "sdf/sdfSystem", ".bss")
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
            emitted, {"fileQueueHead": 0x003DC658}
        )
        self.assertIn("extra emitted symbol wrongManagerWork", problems)
        self.assertIn(
            "missing retail symbol fileQueueHead at +0x0 size 0x40", problems
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
        emitted = [(0x00, 0x40, "fileQueueHead", False)]
        problems = self.ownership_problems(
            emitted, {"fileQueueHead": 0x003DC65C}
        )
        self.assertIn("fileQueueHead is not STT_OBJECT", problems)
        self.assertIn(
            "retail inventory begins at +0x4, expected +0x0",
            problems,
        )
        self.assertIn(
            "fileQueueHead is at +0x0, retail inventory requires +0x4",
            problems,
        )

    def test_rejects_anonymous_padding_overlap_and_section_size(self):
        gap = [(0x04, 0x3C, "fileQueueHead", True)]
        problems = self.ownership_problems(
            gap, {"fileQueueHead": 0x003DC65C}
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
                [(0x00, 0x40, "fileQueueHead", True)],
                {"fileQueueHead": 0x003DC658},
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
                start = syms["fileQueueHead"]
                end = syms["sdfMcCb"]
                yaml_text = self.YAML.replace("0x003DC658", hex(start)).replace(
                    "0x003DC698", hex(end)
                )
                self.assertEqual(end - start, 0x40)
                self.assertEqual(
                    bss_ownership_problems(
                        yaml_text,
                        "file/fileManager",
                        0x40,
                        [(0, 0x40, "fileQueueHead", True)],
                        syms,
                    ),
                    [],
                )

            with self.subTest(version=version, section=".sbss"):
                syms = symbols(version)
                start = syms["sdfThreadSema"]
                end = syms["sdfVblankSema"]
                yaml_text = self.YAML.replace(".bss", ".sbss", 1).replace(
                    "file/fileManager", "sdf/sdfSystem"
                ).replace("0x003DC658", hex(start)).replace(
                    "0x003DC698", hex(end)
                )
                self.assertEqual(end - start, 8)
                self.assertEqual(
                    nobits_ownership_problems(
                        yaml_text,
                        "sdf/sdfSystem",
                        ".sbss",
                        8,
                        [
                            (0, 4, "sdfThreadSema", True),
                            (4, 4, "sdfVblankThreadId", True),
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
00000000 g     O .bss  00000040 fileQueueHead
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
00000004 00000040 c fileQueueHead
00000000 00000040 B explicitFileManagerWork
00000080 00000008 C ordinaryCommon
"""
        self.assertEqual(
            common_symbols(nm),
            [("fileQueueHead", 0x40), ("ordinaryCommon", 8)],
        )


class SectionOwnershipTests(unittest.TestCase):
    SYMBOLS = """\
00000000 g     O .sdata 00000008 D_003BC7D8
00000008 g     O .sdata 00000008 D_003BC7E0
00000000 g     O .data  00000008 unrelated
00000000         *UND*  00000040 fileQueueHead
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


class CompilerLocalFunctionAliasTests(unittest.TestCase):
    def aliases(self, funcs=None, metadata=None, syms=None, function_names=None):
        return check_unit.compiler_local_function_aliases(
            "00000000 l F .text 00000008 helper.0\n" if metadata is None else metadata,
            [(0, 8, "helper.0")] if funcs is None else funcs,
            {"helper": 0x1000} if syms is None else syms,
            {"helper"} if function_names is None else function_names)

    def test_unique_local_function_gets_exact_canonical_address(self):
        self.assertEqual(self.aliases(), {"helper.0": 0x1000})
        self.assertIsNone(check_unit.address("helper.0", {"helper": 0x1000}))

    def test_explicit_suffixed_symbol_wins(self):
        syms = {"helper": 0x1000, "helper.0": 0x2000}
        self.assertEqual(self.aliases(syms=syms), {})
        self.assertEqual(check_unit.address("helper.0", syms), 0x2000)

    def test_plain_peer_other_suffix_and_duplicate_are_ambiguous(self):
        for peer in ((8, 8, "helper"), (8, 8, "helper.1"), (0, 8, "helper.0")):
            with self.subTest(peer=peer):
                self.assertEqual(self.aliases(funcs=[(0, 8, "helper.0"), peer]), {})

    def test_zero_size_definition_peers_are_ambiguous_even_when_nm_omits_them(self):
        for peer in ("helper", "helper.1"):
            for flags in ("l F", "g F"):
                with self.subTest(peer=peer, flags=flags):
                    self.assertEqual(self.aliases(metadata=(
                        "0 l F .text 8 helper.0\n"
                        f"8 {flags} .text 0 {peer}\n")), {})

    def test_exact_suffixed_address_survives_zero_size_peer(self):
        syms = {"helper": 0x1000, "helper.0": 0x2000}
        self.assertEqual(self.aliases(
            metadata="0 l F .text 8 helper.0\n8 g F .text 0 helper\n",
            syms=syms), {})
        self.assertEqual(check_unit.address("helper.0", syms), 0x2000)

    def test_requires_an_explicit_retail_function_stem(self):
        for syms, names in (({}, {"helper"}), ({"helper": 0x1000}, set())):
            with self.subTest(syms=syms, names=names):
                self.assertEqual(self.aliases(syms=syms, function_names=names), {})

    def test_requires_local_function_type_text_section_and_matching_extent(self):
        for row in (
            "", "0 g F .text 8 helper.0", "0 l O .text 8 helper.0",
            "0 l F .data 8 helper.0", "4 l F .text 8 helper.0",
            "0 l F .text 4 helper.0", "0 l F .text 0 helper.0",
            "0 l F .text 8 helper.0\n0 l F .text 8 helper.0",
        ):
            with self.subTest(row=row):
                self.assertEqual(self.aliases(metadata=row), {})

    def test_nonnumeric_and_compound_clone_suffixes_remain_unresolved(self):
        for name in ("helper.isra.0", "helper.clone", "helper.", "helper.0x1"):
            with self.subTest(name=name):
                self.assertEqual(self.aliases(
                    funcs=[(0, 8, name)], metadata=f"0 l F .text 8 {name}"), {})


class NestedFunctionCheckerTests(unittest.TestCase):
    def run_checker(self, requested=None, *, helper_word=0x03E00008,
                    full_helper_word=None, retail_call=0x0C000400,
                    metadata_flags="l F", full_name="helper.0",
                    full_flags="l F", extra_full=(), extra_skip=(),
                    explicit_address=None):
        """Run the complete checker with fake objects; never invoke cc.sh."""
        compiled = struct.pack("<IIII", helper_word, 0, 0x0C000000, 0)
        full_code = struct.pack("<IIII", helper_word if full_helper_word is None
                                else full_helper_word, 0, 0x0C000000, 0)
        retail = struct.pack("<IIII", 0x03E00008, 0, retail_call, 0)
        skip_funcs = [(0, 8, "helper.0"), (8, 8, "caller"), *extra_skip]
        full_funcs = [(0, 8, full_name), (8, 8, "caller"), *extra_full]
        with tempfile.TemporaryDirectory() as tmp, contextlib.ExitStack() as stack:
            root = Path(tmp)
            unit = root / "src/dds1/game/unit.c"
            unit.parent.mkdir(parents=True)
            unit.write_text("void caller(void) {\n    void helper(void) {}\n}\n")
            config = root / "config/dds1"
            config.mkdir(parents=True)
            declarations = "helper = 0x1000; // type:func\ncaller = 0x1008; // type:func\n"
            if explicit_address is not None:
                declarations += f"helper.0 = 0x{explicit_address:X}; // type:func\n"
            (config / "symbol_addrs.txt").write_text(declarations)
            (config / "SLUS_209.74.yaml").write_text("")
            original = root / check_unit.RETAIL["dds1"]
            original.parent.mkdir(parents=True)
            original.write_bytes(retail)
            argv = ["check_unit.py", str(unit)]
            if requested is not None:
                argv.extend(["--func", requested])
            stack.enter_context(patch.object(check_unit, "ROOT", root))
            stack.enter_context(patch.object(check_unit.sys, "argv", argv))
            stack.enter_context(patch("eeas_compat.lit4_range", return_value=(0, 0)))
            stack.enter_context(patch.object(check_unit, "load_segments", return_value=[]))
            stack.enter_context(patch.object(check_unit, "va_to_off", side_effect=lambda segments, addr: addr - 0x1000))
            stack.enter_context(patch.object(check_unit, "text_section", side_effect=lambda obj: full_code if str(obj).endswith("full.o") else compiled))
            stack.enter_context(patch.object(check_unit, "sections", return_value=(b"", {})))
            stack.enter_context(patch.object(check_unit, "relocations", return_value={
                ".text": {8: ("R_MIPS_26", ".text")}, ".rodata": {}, ".sdata": {}}))

            def object_output(*args):
                full = str(args[-1]).endswith("full.o")
                functions = full_funcs if full else skip_funcs
                if "-S" in args:
                    # Real nm -S omits the size column for zero-size symbols.
                    return "".join(
                        f"{offset:x} " + (f"{size:x} " if size else "")
                        + f"{'T' if name == 'caller' else 't'} {name}\n"
                        for offset, size, name in functions)
                if "-t" in args:
                    flags = full_flags if full else metadata_flags
                    return "".join(f"{offset:x} {'g F' if name == 'caller' else flags} .text {size:x} {name}\n"
                                   for offset, size, name in functions)
                return ""

            stack.enter_context(patch.object(check_unit, "run", side_effect=object_output))
            stack.enter_context(patch.object(check_unit.subprocess, "run", return_value=SimpleNamespace(returncode=0, stderr="")))
            output = stack.enter_context(contextlib.redirect_stdout(io.StringIO()))
            with self.assertRaises(SystemExit) as stopped:
                check_unit.main()
            return stopped.exception.code, output.getvalue()

    def test_whole_unit_resolves_nested_body_and_local_call(self):
        status, report = self.run_checker()
        self.assertEqual(status, 0, report)
        self.assertIn("OK   helper.0 @ 0x00001000", report)
        self.assertIn("OK   caller @ 0x00001008", report)
        self.assertIn("2 match, 0 differ", report)

    def test_focused_selection_still_requires_exact_emitted_name(self):
        status, report = self.run_checker("helper.0")
        self.assertEqual(status, 0, report)
        self.assertIn("1 match, 0 differ", report)
        status, report = self.run_checker("helper")
        self.assertEqual(status, 1, report)
        self.assertIn("MISSING helper: requested function not emitted", report)

    def test_real_helper_difference_is_not_masked(self):
        status, report = self.run_checker(helper_word=0x03E00009)
        self.assertEqual(status, 1, report)
        self.assertIn("DIFF helper.0", report)
        self.assertIn("1 match, 1 differ", report)

    def test_real_call_target_difference_is_not_masked(self):
        status, report = self.run_checker(retail_call=0x0C000402)
        self.assertEqual(status, 1, report)
        self.assertIn("DIFF caller", report)
        self.assertIn("retail calls a different function", report)

    def test_full_unit_helper_difference_keeps_context_failure(self):
        status, report = self.run_checker(full_helper_word=0x03E00009)
        self.assertEqual(status, 1, report)
        self.assertIn("CONTEXT helper.0", report)
        self.assertIn("full unit vs retail: 1 of 2 words differ", report)

    def test_both_objects_must_prove_unique_local_identity(self):
        for arguments in (
            {"metadata_flags": "g F"}, {"metadata_flags": "l O"},
            {"full_flags": "g F"}, {"full_name": "helper.1"},
            {"extra_full": [(0, 8, "helper.1")]},
            {"extra_skip": [(0, 8, "helper.1")]},
            {"extra_full": [(0, 8, "helper")]},
            {"extra_skip": [(0, 8, "helper")]},
        ):
            with self.subTest(arguments=arguments):
                status, report = self.run_checker(**arguments)
                self.assertEqual(status, 1, report)
                self.assertIn("helper.0: no address", report)

    def test_zero_size_peer_in_either_object_blocks_alias_and_call_resolution(self):
        for context in ("extra_skip", "extra_full"):
            for peer in ("helper", "helper.1"):
                with self.subTest(context=context, peer=peer):
                    status, report = self.run_checker(**{context: [(8, 0, peer)]})
                    self.assertEqual(status, 1, report)
                    self.assertIn("helper.0: no address", report)
                    self.assertIn("DIFF caller", report)
                    self.assertIn("0 match, 2 differ", report)

    def test_explicit_symbol_mapping_is_not_overridden_by_stem(self):
        status, report = self.run_checker(explicit_address=0x1008)
        self.assertEqual(status, 1, report)
        self.assertIn("DIFF helper.0 @ 0x00001008", report)


import rodata_attribution as p

def provenance_words(value=0x18004, rt=7, rs=13):
    return [0x3c000000 | (rt << 16) | (((value + 0x8000) >> 16) & 0xffff),
            (rt << 21) | (rs << 16) | (rt << 11) | 0x21,
            0x8c000000 | (rt << 21) | (rt << 16) | (value & 0xffff)]


def provenance_packed(ws):
    return struct.pack('<' + 'I' * len(ws), *ws)


def provenance_elf_blob(code=None, value=0x18000, size=32, name='arbitrary_table', rels=None,
             extra_symbols=(), section_symbols=1, func='worker', function_size=None,
             object_section=2, object_info=0x11, extra_relocation_sections=(), ro_padding=16):
    """Build a real ET_REL ELF from struct, not mocked ELFFile objects.

    Symbol indices: zero, rodata section symbol(s), function, named object,
    extra symbols. Extra-symbol tuples: (name,value,size,info,section).
    """
    code = provenance_packed(provenance_words()) if code is None else code
    strings = bytearray(b'\0')
    def string(s):
        offset = len(strings)
        strings.extend(s.encode() + b'\0')
        return offset
    def sym(n, v, z, info, sec):
        return struct.pack('<IIIBBH', string(n) if n else 0, v, z, info, 0, sec)
    symbols = [b'\0' * 16]
    symbols += [sym('', 0, 0, 3, 2)] * section_symbols
    symbols += [sym(func, 0, len(code) if function_size is None else function_size, 0x12, 1)]
    symbols += [sym(name, value, size, object_info, object_section)]
    symbols += [sym(*args) for args in extra_symbols]
    rels = [(0, 5, 1), (8, 6, 1)] if rels is None else rels
    def relocation_data(items):
        return b''.join(struct.pack('<II', offset, (index << 8) | typ) for offset, typ, index in items)
    # name,type,flags,data,link,info,align,entsize
    sections = [('', 0, 0, b'', 0, 0, 0, 0),
                ('.text', 1, 6, code, 0, 0, 4, 0),
                ('.rodata', 1, 2, b'\0' * (value + max(size, 0) + ro_padding), 0, 0, 4, 0),
                ('.symtab', 2, 0, b''.join(symbols), 4, 1 + section_symbols, 4, 16),
                ('.strtab', 3, 0, bytes(strings), 0, 0, 1, 0),
                ('.rel.text', 9, 0, relocation_data(rels), 3, 1, 4, 8)]
    for secname, target, items in extra_relocation_sections:
        sections.append((secname, 9, 0, relocation_data(items), 3, target, 4, 8))
    shstr = bytearray(b'\0')
    name_offsets = []
    for section in sections + [('.shstrtab',)]:
        name_offsets.append(len(shstr))
        shstr.extend(section[0].encode() + b'\0')
    sections.append(('.shstrtab', 3, 0, bytes(shstr), 0, 0, 1, 0))
    image = bytearray(b'\0' * 52)
    headers = []
    for i, (_, typ, flags, data, link, info, align, entsize) in enumerate(sections):
        while len(image) % max(align, 1):
            image.append(0)
        offset = len(image)
        image.extend(data)
        headers.append(struct.pack('<IIIIIIIIII', name_offsets[i], typ, flags, 0,
                                   offset, len(data), link, info, align, entsize))
    while len(image) % 4:
        image.append(0)
    shoff = len(image)
    image.extend(b''.join(headers))
    ident = b'\x7fELF' + bytes([1, 1, 1, 0, 0]) + b'\0' * 7
    image[:52] = struct.pack('<16sHHIIIIIHHHHHH', ident, 1, 8, 1, 0, 0, shoff,
                             0, 52, 0, 0, 40, len(sections), len(sections) - 1)
    return bytes(image)


def listing(source, claims=None):
    claims = {3: (0, provenance_packed(provenance_words()))} if claims is None else claims
    rows = []
    for n, line in enumerate(source.splitlines(), 1):
        if n not in claims:
            rows.append(f'{n} \t{line}')
            continue
        offset, payload = claims[n]
        chunks = [payload[i:i + 4].hex() for i in range(0, len(payload), 4)]
        rows.append(f'{n} {offset:04x} {chunks[0]}\t{line}')
        rows.extend(f'{n} {chunk}' for chunk in chunks[1:])
    return '\n'.join(rows)


SOURCE = '.text\n.ent worker\nlw $7,arbitrary_table+4($13)\n.end worker'


class ProvenanceAdversarialTests(unittest.TestCase):
    def facts(self, source=SOURCE, blob=None, report=None, listed=None):
        blob = provenance_elf_blob() if blob is None else blob
        return p.named_lw_sites(source, listing(source) if report is None else report,
                                blob, blob if listed is None else listed)

    def test_positive_generic_registers_and_signed_carry(self):
        for rt, rs, name, value, addend in [(7, 13, 'arbitrary_table', 0x18000, 4),
                                           (19, 25, 'Other_32', 0x20000, -32768),
                                           (31, 2, 'zeroAddend', 0x8000, 0)]:
            with self.subTest(rt=rt, addend=addend):
                code = provenance_packed(provenance_words(value + addend, rt, rs))
                expr = name + (f'{addend:+d}' if addend else '')
                source = f'.text\n.ent worker\nlw ${rt},{expr}(${rs})\n.end worker'
                fact = self.facts(source, provenance_elf_blob(code, value=value, name=name), listing(source, {3: (0, code)}))
                self.assertEqual(fact, {8: (name, value, 32, addend, 'worker', 0, rt, rs)})

    def test_changed_whole_elf(self):
        original = provenance_elf_blob()
        changed = bytearray(original)
        changed[9] ^= 1  # ELF identity padding, outside code, is still covered.
        self.assertEqual(self.facts(blob=original, listed=bytes(changed)), {})

    def test_listing_line_identity_and_sequence(self):
        good = listing(SOURCE)
        variants = [good.replace('3 0000', '9 0000'), good.replace('arbitrary_table+4', 'arbitrary_table+8'),
                    '\n'.join(good.splitlines()[1:]), good + '\n4\t.end worker',
                    good.replace('2 \t.ent worker', '1 \t.ent worker')]
        for report in variants:
            with self.subTest(report=report):
                self.assertIsNone(p.listing_lines(SOURCE, report))
                self.assertEqual(self.facts(report=report), {})

    def test_continuation_missing_duplicate_noncontiguous_truncated(self):
        rows = listing(SOURCE).splitlines()
        variants = ['\n'.join(rows[:4] + rows[5:]),
                    '\n'.join(rows[:5] + [rows[4]] + rows[5:]),
                    '\n'.join(rows[:4] + [rows[-1]] + rows[4:-1]),
                    '\n'.join(rows[:4] + [rows[4][:-2]] + rows[5:]),
                    '\n'.join(rows[:4] + [rows[4].replace('3 ', '3 0004 ')] + rows[5:])]
        for report in variants:
            with self.subTest(report=report):
                self.assertEqual(self.facts(report=report), {})

    def test_wrong_text_bytes(self):
        self.assertEqual(self.facts(blob=provenance_elf_blob(provenance_packed(provenance_words(0x18008)))), {})

    def test_wrong_section_function_and_inline_context(self):
        for source in [SOURCE.replace('.text', '.rdata'), SOURCE.replace('worker', 'stranger'),
                       SOURCE.replace('.ent worker', '# no owner'),
                       SOURCE.replace('.ent worker', '.ent worker\n#APP'),
                       SOURCE.replace('.end worker', '.end stranger')]:
            with self.subTest(source=source):
                line = next(i for i, x in enumerate(source.splitlines(), 1) if x.startswith('lw '))
                self.assertEqual(self.facts(source, report=listing(source, {line: (0, provenance_packed(provenance_words()))})), {})

    def test_macro_include_conditional_and_unsupported_context(self):
        for directive in ['.macro x', '.include "x.s"', '.if 1', '.ifdef X', '.else', '.endif',
                          '.rept 2', '.irp x,1', '.incbin "x.bin"', '.pushsection .text', '.subsection 1']:
            source = directive + '\n' + SOURCE
            with self.subTest(directive=directive):
                self.assertEqual(self.facts(source, report=listing(source, {4: (0, provenance_packed(provenance_words()))})), {})

    def test_raw_missing_duplicate_section_symbol(self):
        for count in [0, 2]:
            blob = provenance_elf_blob(section_symbols=count)
            self.assertFalse(p.raw_metadata_clear(blob))
            self.assertEqual(self.facts(blob=blob), {})

    def test_raw_duplicate_overlapping_and_out_of_bounds_relocations(self):
        for rels in [[(0, 5, 1), (8, 6, 1), (8, 6, 1)], [(0, 5, 1), (2, 6, 1)],
                     [(0, 5, 1), (10, 6, 1)], [(0, 5, 1), (100, 6, 1)]]:
            blob = provenance_elf_blob(rels=rels)
            with self.subTest(rels=rels):
                self.assertFalse(p.raw_metadata_clear(blob))
                self.assertEqual(self.facts(blob=blob), {})

    def test_extra_and_wrong_relocation_identities(self):
        for rels in [[(0, 5, 1), (4, 2, 1), (8, 6, 1)], [(0, 5, 1)],
                     [(0, 5, 1), (8, 6, 3)], [(0, 5, 3), (8, 6, 3)],
                     [(0, 6, 1), (8, 5, 1)], [(8, 6, 1), (0, 5, 1)]]:
            with self.subTest(rels=rels):
                self.assertEqual(self.facts(blob=provenance_elf_blob(rels=rels)), {})

    def test_duplicate_relocation_sections_and_cross_section_overlap(self):
        for name in ['.rel.text', '.rel.text.extra']:
            blob = provenance_elf_blob(extra_relocation_sections=[(name, 1, [(8, 6, 1)])])
            self.assertFalse(p.raw_metadata_clear(blob))
            self.assertEqual(self.facts(blob=blob), {})

    def test_repeated_symbol_identities_and_overlapping_functions(self):
        for extra in [('arbitrary_table', 0x18000, 32, 0x11, 2),
                      ('arbitrary_table', 0x18020, 4, 0x11, 2),
                      ('worker', 0, 12, 0x12, 1), ('other_owner', 4, 4, 0x12, 1)]:
            with self.subTest(extra=extra):
                self.assertEqual(self.facts(blob=provenance_elf_blob(extra_symbols=[extra])), {})
        self.assertEqual(self.facts(blob=provenance_elf_blob(function_size=8)), {})

    def test_wrong_object_attributes(self):
        for kwargs in [dict(object_section=1), dict(object_info=1), dict(object_info=0x10), dict(size=0)]:
            with self.subTest(kwargs=kwargs):
                self.assertEqual(self.facts(blob=provenance_elf_blob(**kwargs)), {})

    def test_altered_flow_and_unsupported_expansions(self):
        good = provenance_words()
        variants = [provenance_words(rt=6), provenance_words(rs=12), [good[0], good[1] ^ (1 << 11), good[2]],
                    [good[0], 0, good[2]], good[:2], good + [0], provenance_words(rt=0), provenance_words(rt=13)]
        for ws in variants:
            code = provenance_packed(ws)
            with self.subTest(provenance_words=ws):
                self.assertEqual(self.facts(blob=provenance_elf_blob(code), report=listing(SOURCE, {3: (0, code)})), {})

    def test_same_numeric_address_in_different_sections(self):
        source = SOURCE + '\n.rdata\n.word 0'
        report = listing(source, {3: (0, provenance_packed(provenance_words())), 6: (0, b'\0' * 4)})
        self.assertIn(8, self.facts(source, report=report))
        # The same claim in .text is ambiguous and must not acquire provenance.
        source = SOURCE + '\n.text\n.word 0'
        report = listing(source, {3: (0, provenance_packed(provenance_words())), 6: (0, b'\0' * 4)})
        self.assertEqual(self.facts(source, report=report), {})

    def test_two_source_sites_cannot_claim_one_macro(self):
        source = SOURCE.replace('.end worker', 'lw $7,arbitrary_table+4($13)\n.end worker')
        report = listing(source, {3: (0, provenance_packed(provenance_words())), 4: (0, provenance_packed(provenance_words()))})
        self.assertEqual(self.facts(source, report=report), {})

    def test_native_exact_addend_flow_and_bounds(self):
        fact = self.facts()[8]
        self.assertTrue(p.native_site_agrees(fact, provenance_packed(provenance_words(0x28004)), 0x28000))
        for code, address in [(provenance_packed(provenance_words(0x28008)), 0x28000),
                              (provenance_packed(provenance_words(0x28000)), 0x28000),
                              (provenance_packed(provenance_words(0x28004, rt=6)), 0x28000),
                              (provenance_packed(provenance_words(0x28004))[:8], 0x28000),
                              (provenance_packed(provenance_words(0x28004)), None)]:
            self.assertFalse(p.native_site_agrees(fact, code, address))
        bad_offset = fact[:5] + (-1,) + fact[6:]
        self.assertFalse(p.native_site_agrees(bad_offset, provenance_packed(provenance_words()), 0x18000))

    def test_invalid_or_truncated_elf_fails_closed(self):
        for blob in [b'', b'not an ELF', provenance_elf_blob()[:32], provenance_elf_blob()[:-10]]:
            with self.subTest(length=len(blob)):
                self.assertFalse(p.raw_metadata_clear(blob))
                self.assertEqual(self.facts(blob=blob), {})




class PrebiasedRodataCLITests(unittest.TestCase):
    def check_prebias(self, *, func=None, wrong_native=False, real_table=False,
                      missing_listing=False, unequal_listing=False, missing_listed_object=False,
                      full_failed=False, corrupted_payload=False, padding=False):
        from elftools.elf.elffile import ELFFile
        source = '.text\n.ent worker\nlw $7,array-4($13)\n.end worker'
        code = provenance_packed(provenance_words(4))
        code_relocs = {0: ('R_MIPS_HI16', '.rodata'), 8: ('R_MIPS_LO16', '.rodata')}
        if real_table:
            code += struct.pack('<III', 0x3c020000, 0x24420004, 0)
            code_relocs.update({12: ('R_MIPS_HI16', '.rodata'), 16: ('R_MIPS_LO16', '.rodata')})
        native = provenance_packed(provenance_words(0x101000 if wrong_native else 0x101004))
        if real_table:
            native += struct.pack('<III', 0x3c020010, 0x24421004, 0)
        payload = struct.pack('<4I', 1, 2, 3, 4)
        skip_data = b'\0' * 8 + payload
        full_data = b'\0' * 4 + struct.pack('<I', 4) + payload
        retail_data = b'\0' * 4 + struct.pack('<I', 0x100004) + payload
        if padding:
            full_data += b'padding!'
            retail_data += b'padding!'
        if corrupted_payload:
            skip_data = skip_data[:-1] + b'!'
        relocs = {'.text': code_relocs, '.rodata': {4: ('R_MIPS_32', '.text')}, '.data': {}, '.sdata': {}}
        def object_blob(data):
            blob = bytearray(provenance_elf_blob(
                code=code, value=8, size=16, name='array', ro_padding=len(data)-24,
                extra_symbols=(('', 0, 0, 3, 1),),
                rels=[(o, 5 if kind == 'R_MIPS_HI16' else 6, 1) for o, (kind, _) in code_relocs.items()],
                extra_relocation_sections=(('.rel.rodata', 2, [(4, 2, 4)]),)))
            elf = ELFFile(io.BytesIO(blob))
            start = elf.get_section_by_name('.rodata')['sh_offset']
            blob[start:start+len(data)] = data
            return bytes(blob)
        skip_blob, full_blob = object_blob(skip_data), object_blob(full_data)
        report_listing = listing(source, {3: (0, code[:12])})
        with tempfile.TemporaryDirectory() as tmp, contextlib.ExitStack() as stack:
            root = Path(tmp)
            unit = root / 'src/dds1/game/unit.c'
            unit.parent.mkdir(parents=True)
            unit.write_text('const int array[4] = {1,2,3,4};\nvoid worker(void) {}\n')
            config = root / 'config/dds1'
            config.mkdir(parents=True)
            (config/'symbol_addrs.txt').write_text('worker = 0x100000; // type:func\narray = 0x101008; // size:0x10\nprefix = 0x101000;\n')
            (config/'SLUS_209.74.yaml').write_text(f'  - [0x2000, .rodata, game/unit]\n  - [0x{0x2000+len(full_data):X}, .rodata, game/next]\n')
            image = bytearray(0x3000)
            image[0x1000:0x1000+len(native)] = native
            image[0x2000:0x2000+len(retail_data)] = retail_data
            retail_path = root/check_unit.RETAIL['dds1']
            retail_path.parent.mkdir(parents=True)
            retail_path.write_bytes(image)
            assembly = root/'asm/dds1/data/game/unit.rodata.s'
            assembly.parent.mkdir(parents=True)
            assembly.write_text('dlabel prefix\ndlabel array\n')
            argv = ['check_unit.py',str(unit)] + (['--func',func] if func else [])
            stack.enter_context(patch.object(check_unit, 'ROOT', root))
            stack.enter_context(patch.object(check_unit.sys, 'argv', argv))
            stack.enter_context(patch('eeas_compat.lit4_range',return_value=(0,0)))
            stack.enter_context(patch.object(check_unit,'load_segments',return_value=[]))
            stack.enter_context(patch.object(check_unit,'va_to_off',side_effect=lambda segs,a:a-0xff000))
            stack.enter_context(patch.object(check_unit,'off_to_va',side_effect=lambda segs,a:a+0xff000))
            stack.enter_context(patch.object(check_unit,'text_section',return_value=code))
            stack.enter_context(patch.object(check_unit,'sections',side_effect=lambda obj:
                (full_data,{'.rodata':(0,len(full_data))}) if obj.name=='full.o' else
                (skip_data,{'.rodata':(0,len(skip_data))})))
            stack.enter_context(patch.object(check_unit,'relocations',return_value=relocs))
            stack.enter_context(patch.object(check_unit,'owns_rodata',return_value=True))
            def output(*args):
                if '-S' in args:
                    return f'0 {len(code):x} T worker\n'
                if '-t' in args:
                    return f'00000000 g F .text {len(code):08x} worker\n00000008 g O .rodata 00000010 array\n'
                if '-r' in args:
                    return 'RELOCATION RECORDS FOR [.rodata]:\n00000004 R_MIPS_32 .text\n'
                return ''
            stack.enter_context(patch.object(check_unit,'run',side_effect=output))
            def compile_stub(args, **kwargs):
                out = Path(args[-1]); full = out.name=='full.o'
                out.write_bytes(full_blob if full else skip_blob)
                env = kwargs['env']
                if 'DDS_KEEP_S' in env:
                    Path(env['DDS_KEEP_S']).write_text(source)
                    if not missing_listing:
                        lp = Path(env['DDS_KEEP_LISTING'])
                        lp.write_text(report_listing)
                        if not missing_listed_object:
                            Path(str(lp)+'.o').write_bytes(skip_blob+b'!' if unequal_listing else skip_blob)
                return SimpleNamespace(returncode=int(full and full_failed),stderr='')
            stack.enter_context(patch.object(check_unit.subprocess,'run',side_effect=compile_stub))
            captured = stack.enter_context(contextlib.redirect_stdout(io.StringIO()))
            with self.assertRaises(SystemExit) as stopped:
                check_unit.main()
            return stopped.exception.code,captured.getvalue()

    def test_main_path_named_prebias_full_and_focused(self):
        for func in (None,'worker'):
            status, report = self.check_prebias(func=func)
            self.assertEqual(status,0,report)

    def test_main_path_real_table_same_compact_value_keeps_its_comparison(self):
        for func in (None,'worker'):
            status, report = self.check_prebias(real_table=True,func=func)
            self.assertEqual(status,1,report)
            self.assertIn('DIFF jump table',report)

    def test_main_path_wrong_native_addend_fails_explicitly(self):
        status, report = self.check_prebias(wrong_native=True)
        self.assertEqual(status,1,report)
        self.assertIn('ADDRESS rodata',report)

    def test_main_path_optional_evidence_failure_keeps_old_rejection(self):
        for change in ({'missing_listing':True},{'unequal_listing':True},{'missing_listed_object':True}):
            with self.subTest(change=change):
                status, report = self.check_prebias(**change)
                self.assertEqual(status,1,report)
                self.assertIn('DIFF jump table',report)

    def test_main_path_prebias_preserves_payload_padding_and_production_gate(self):
        for change in ({'full_failed':True},{'corrupted_payload':True},{'padding':True}):
            with self.subTest(change=change):
                status, report = self.check_prebias(**change)
                self.assertEqual(status,1,report)



class ProvenanceContractRevisionTests(unittest.TestCase):
    def test_non_mips_or_non_rel_elf_cannot_supply_mips_site_proof(self):
        for field, value in ((18,3),(16,2)):
            blob=bytearray(provenance_elf_blob())
            struct.pack_into('<H',blob,field,value)
            self.assertFalse(p.raw_metadata_clear(bytes(blob)))
            self.assertEqual(p.named_lw_sites(SOURCE,listing(SOURCE),bytes(blob),bytes(blob)),{})

    def test_relocation_link_type_and_entry_width_are_explicit(self):
        for field,value in ((24,4),(4,4),(36,12)):
            blob=bytearray(provenance_elf_blob())
            shoff=struct.unpack_from('<I',blob,32)[0]
            struct.pack_into('<I',blob,shoff+5*40+field,value)
            self.assertFalse(p.raw_metadata_clear(bytes(blob)))

    def test_raw_record_tails_and_out_of_file_sections_are_rejected(self):
        for section,field,value in ((5,20,17),(3,20,65),(5,16,0xfffffff0),(3,16,0xfffffff0)):
            blob=bytearray(provenance_elf_blob())
            shoff=struct.unpack_from('<I',blob,32)[0]
            struct.pack_into('<I',blob,shoff+section*40+field,value)
            self.assertFalse(p.raw_metadata_clear(bytes(blob)))
            self.assertEqual(p.named_lw_sites(SOURCE,listing(SOURCE),bytes(blob),bytes(blob)),{})

    def test_empty_native_rela_companions_are_allowed_but_never_populated_rela(self):
        blob=bytearray(provenance_elf_blob(extra_relocation_sections=[('.rela.text',1,[])]))
        shoff=struct.unpack_from('<I',blob,32)[0]
        struct.pack_into('<I',blob,shoff+6*40+4,4)
        struct.pack_into('<I',blob,shoff+6*40+36,12)
        self.assertTrue(p.raw_metadata_clear(bytes(blob)))
        self.assertTrue(p.named_lw_sites(SOURCE,listing(SOURCE),bytes(blob),bytes(blob)))
        struct.pack_into('<I',blob,shoff+6*40+20,12)
        self.assertFalse(p.raw_metadata_clear(bytes(blob)))

    def test_zero_size_function_competitors_do_not_disappear(self):
        for name,offset in (('worker',0),('other_owner',0),('interior_owner',4)):
            blob=provenance_elf_blob(extra_symbols=[(name,offset,0,0x12,1)])
            self.assertEqual(p.named_lw_sites(SOURCE,listing(SOURCE),blob,blob),{})
        # Ordinary local assembly labels remain legitimate, not function aliases.
        blob=provenance_elf_blob(extra_symbols=[('local_label',4,0,0,1)])
        self.assertTrue(p.named_lw_sites(SOURCE,listing(SOURCE),blob,blob))

    def test_missing_unreadable_partial_and_invalid_optional_evidence(self):
        with tempfile.TemporaryDirectory() as tmp:
            lp=Path(tmp)/'listing'
            self.assertEqual(p.load_named_lw_sites(SOURCE,provenance_elf_blob(),lp),{})
            lp.write_text(listing(SOURCE))
            self.assertEqual(p.load_named_lw_sites(SOURCE,provenance_elf_blob(),lp),{})
            Path(str(lp)+'.o').mkdir()
            self.assertEqual(p.load_named_lw_sites(SOURCE,provenance_elf_blob(),lp),{})
            lp.write_bytes(b'\xff')
            self.assertEqual(p.load_named_lw_sites(SOURCE,provenance_elf_blob(),lp),{})


class OptionalAssemblerListingWrapperTests(unittest.TestCase):
    def test_listing_failures_do_not_change_original_compile_result(self):
        import os
        import subprocess
        import shutil
        with tempfile.TemporaryDirectory() as tmp:
            root=Path(tmp)
            tools=root/'tools'; tools.mkdir()
            shutil.copy2(Path(check_unit.__file__).with_name('cc.sh'),tools/'cc.sh')
            cc=tools/'compilers/ee-gcc2.96/lib/gcc-lib/ee/2.96-ee-001003-1/cc1'
            assembler=tools/'compilers/ee-gcc2.96/ee/bin/as'
            fake='''#!/usr/bin/env python3
import os,sys,json
from pathlib import Path
args=sys.argv[1:]
with open(os.environ['CALL_LOG'],'a') as f:f.write(json.dumps([Path(sys.argv[0]).name,args])+'\\n')
out=Path(args[args.index('-o')+1])
listing=next((a.split('=',1)[1] for a in args if a.startswith('-aln=')),None)
if Path(sys.argv[0]).name=='cc1':out.write_text('.text\\n')
else:
 if listing and os.environ.get('LISTING_MODE')=='fail':sys.exit(1)
 out.write_bytes(b'different' if listing and os.environ.get('LISTING_MODE')=='unequal' else b'original')
 if listing:Path(listing).write_text('listing')
'''
            for exe in (cc,assembler):
                exe.parent.mkdir(parents=True,exist_ok=True);exe.write_text(fake);exe.chmod(0o755)
            Path(str(cc)+'.aslr-fixed').write_text('d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1\n')
            src=root/'input.c';src.write_text('int f(void) { return 1; }\n')
            out=root/'output.o'; lp=root/'listing'
            env=dict(os.environ,DDS_I386_LIBDIR='',DDS_KEEP_LISTING='',CALL_LOG=str(root/'calls'))
            env.pop('DDS_AS_UNIT',None);env.pop('DDS_KEEP_S',None)
            run=lambda:subprocess.run([str(tools/'cc.sh'),str(src),'-o',str(out)],env=env,capture_output=True,text=True)
            plain=run();self.assertEqual(plain.returncode,0,plain.stderr)
            self.assertEqual(out.read_bytes(),b'original')
            self.assertNotIn('-aln=',(root/'calls').read_text())
            for mode,destination in [('ok',lp),('fail',lp),('unequal',lp),('ok',root/'absent'/'listing'),
                                     ('ok',out),('ok',root/'output')]:
                with self.subTest(mode=mode,destination=destination):
                    env.update(LISTING_MODE=mode,DDS_KEEP_LISTING=str(destination))
                    result=run();self.assertEqual(result.returncode,0,result.stderr)
                    self.assertEqual(out.read_bytes(),b'original')
                    if destination==lp:
                        self.assertEqual(lp.exists(),mode=='ok')
                        self.assertEqual(Path(str(lp)+'.o').exists(),mode=='ok')


if __name__ == "__main__":
    unittest.main()
