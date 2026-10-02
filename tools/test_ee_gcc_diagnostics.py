#!/usr/bin/env python3
import importlib.util
import sys
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parent.parent


def load(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    assert spec and spec.loader
    module = importlib.util.module_from_spec(spec)
    sys.modules[name] = module
    spec.loader.exec_module(module)
    return module


probe = load("ee_gcc_probe", ROOT / "tools/ee_gcc_probe.py")
compare = load("ee_gcc_compare", ROOT / "tools/ee_gcc_compare.py")
delay_slots = load("ee_gcc_delay_slots", ROOT / "tools/ee_gcc_delay_slots.py")
allocations = load("ee_gcc_allocations", ROOT / "tools/ee_gcc_allocations.py")


class ProbeTests(unittest.TestCase):
    def test_extract_dump_function(self):
        text = ";; Function first\nA\n\n;; Function wanted\nB\nC\n\n;; Function last\nD\n"
        self.assertEqual(";; Function wanted\nB\nC\n", probe.extract_dump_function(text, "wanted"))
        self.assertIsNone(probe.extract_dump_function(text, "missing"))

    def test_extract_assembly_function(self):
        text = ".ent first\nfirst:\n.end first\n\t.ent wanted\nwanted:\n\tnop\n\t.end wanted\n"
        self.assertEqual(
            "\t.ent wanted\nwanted:\n\tnop\n\t.end wanted\n",
            probe.extract_assembly_function(text, "wanted"),
        )

    def test_inside(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            self.assertTrue(probe.inside(root / "child", root))
            self.assertFalse(probe.inside(root, root / "child"))


class CompareTests(unittest.TestCase):
    def test_normalize_observation_noise(self):
        left = ('(note 1 2 3 0xf7ce4c40 NOTE_INSN_BLOCK_BEG 803438496)\n'
                '(note/s 4 5 6 NOTE_INSN_DELETED -137731424)\n'
                '.file 1 ".18/dds1/a.c"\n')
        right = ('(note 1 2 3 0xf7c4cc40 NOTE_INSN_BLOCK_BEG 0)\n'
                 '(note/s 4 5 6 NOTE_INSN_DELETED -138018144)\n'
                 '.file 1 ".42/dds1/a.c"\n')
        self.assertEqual(compare.normalize(left), compare.normalize(right))

    def test_normalize_global_label_numbers_without_losing_identity(self):
        left = '(code_label 24 23 28 2191 "" "" [0 uses])\nbeq $2,$0,$L2191\n$L2191:\n'
        right = '(code_label 24 23 28 2235 "" "" [0 uses])\nbeq $2,$0,$L2235\n$L2235:\n'
        self.assertEqual(compare.normalize(left), compare.normalize(right))
        different_target = right.replace("beq $2,$0,$L2235", "beq $2,$0,$L2236")
        self.assertNotEqual(compare.normalize(left), compare.normalize(different_target))

    def test_compare_finds_first_semantic_pass(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            left, right = root / "left", root / "right"
            left.mkdir()
            right.mkdir()
            (left / "rtl.00.rtl").write_text("same\n")
            (right / "rtl.00.rtl").write_text("same\n")
            (left / "rtl.03.cse").write_text("choice A\n")
            (right / "rtl.03.cse").write_text("choice B\n")
            report = compare.compare_runs(left, right)
            self.assertEqual("rtl.03.cse", report["first_semantic_divergence"])
            self.assertEqual("CSE", report["classification"]["stage"])

    def test_manifest_rename_is_canonicalized(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            left, right = root / "left", root / "right"
            left.mkdir()
            right.mkdir()
            (left / "rtl.00.rtl").write_text('(symbol_ref "old_name")\n')
            (right / "rtl.00.rtl").write_text('(symbol_ref "long_new_name")\n')
            (right / "manifest.json").write_text(
                '{"replacements":[{"old":"old_name","new":"long_new_name"}]}\n'
            )
            report = compare.compare_runs(left, right)
            self.assertIsNone(report["first_semantic_divergence"])

    def test_function_scope_ignores_other_functions(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            left, right = root / "left", root / "right"
            for run in (left, right):
                (run / "functions/wanted").mkdir(parents=True)
            (left / "rtl.03.cse").write_text("whole TU A\n")
            (right / "rtl.03.cse").write_text("whole TU B\n")
            (left / "functions/wanted/03.cse").write_text("same target\n")
            (right / "functions/wanted/03.cse").write_text("same target\n")
            (left / "functions/wanted/final.s").write_text("same final\n")
            (right / "functions/wanted/final.s").write_text("same final\n")
            self.assertIsNone(compare.compare_runs(left, right, "wanted")["first_semantic_divergence"])

    def test_function_scope_finds_target_pass(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            left, right = root / "left", root / "right"
            for run in (left, right):
                (run / "functions/wanted").mkdir(parents=True)
            (left / "functions/wanted/02.jump").write_text("same\n")
            (right / "functions/wanted/02.jump").write_text("same\n")
            (left / "functions/wanted/03.cse").write_text("choice A\n")
            (right / "functions/wanted/03.cse").write_text("choice B\n")
            self.assertEqual(
                "rtl.03.cse",
                compare.compare_runs(left, right, "wanted")["first_semantic_divergence"],
            )

    def test_assembly_only_divergence_is_reported(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            left, right = root / "left", root / "right"
            left.mkdir()
            right.mkdir()
            (left / "rtl.03.cse").write_text("same\n")
            (right / "rtl.03.cse").write_text("same\n")
            (left / "candidate.s").write_text("assembly A\n")
            (right / "candidate.s").write_text("assembly B\n")
            report = compare.compare_runs(left, right)
            self.assertEqual("assembly", report["first_semantic_divergence"])
            self.assertEqual("final emission", report["classification"]["stage"])


class DelaySlotTests(unittest.TestCase):
    SAMPLE = '''
(insn 138 75 80 (sequence[
            (call_insn:TI 76 75 80 (parallel[
                        (call (mem:SI (symbol_ref:SI ("convert")) 0)
                            (const_int 0))
                        (clobber (reg:SI 31 ra))
                    ] ) 321 (nil)
                (nil)
                (nil))
            (insn:TI 75 73 76 (set (reg:SF 46 $f14)
                    (mem/s:SF (plus:SI (reg:SI 29 sp)
                            (const_int 24)) 22)) 205 (nil)
                (nil))
        ] ) -1 (nil)
    (nil))
'''

    def test_balanced_form_ignores_parentheses_in_strings(self):
        text = '(insn 1 0 0 (asm_input ("text ) ( text")) -1) trailing'
        form, end = delay_slots.balanced_form(text, 0)
        self.assertEqual('(insn 1 0 0 (asm_input ("text ) ( text")) -1)', form)
        self.assertEqual(" trailing", text[end:])

    def test_analyze_delay_sequence(self):
        sequences = delay_slots.analyze_dump(self.SAMPLE)
        self.assertEqual(1, len(sequences))
        self.assertEqual(138, sequences[0]["wrapper_uid"])
        self.assertEqual("convert", sequences[0]["branch"]["target"])
        self.assertEqual(75, sequences[0]["slots"][0]["uid"])
        self.assertEqual("load", sequences[0]["slots"][0]["operation"])
        self.assertEqual("$f14", sequences[0]["slots"][0]["destination"])


class AllocationTests(unittest.TestCase):
    SAMPLE = """;; Function sample
;; 2 regs to allocate: 84 85
;; 84 conflicts: 84 85 2 4 28 29
;; 84 preferences: 5
;; 85 conflicts: 84 85 2 5 28 29

;; Register dispositions:
84 in 5  85 in 4  91 in 2

;; Hard regs used:  0 1 2 4 5 31 75
Spilling for insn 40.
Reloads for insn # 40
"""

    def test_global_candidates_are_distinct_from_other_dispositions(self):
        report = allocations.parse_dump(self.SAMPLE, "sample")[0]
        self.assertEqual([84, 85], report["global_allocation_order"])
        self.assertEqual([40], report["spills"])
        self.assertEqual([40], report["reloads"])
        rows = {row["pseudo"]: row for row in report["pseudos"]}
        self.assertEqual("a1", rows[84]["selected_name"])
        self.assertEqual([2, 4, 28, 29], rows[84]["hard_conflicts"])
        self.assertEqual("other_disposition", rows[91]["allocation_kind"])

    def test_final_attempt_is_used_after_allocator_retry(self):
        text = """;; Function retried
;; 1 regs to allocate: 84
;; 84 conflicts: 84 2
;; 1 regs to allocate: 85
;; 85 conflicts: 85 3
;; Register dispositions:
85 in 4
;; Hard regs used: 4
"""
        report = allocations.parse_dump(text, "retried")[0]
        self.assertEqual(1, report["retry_count"])
        self.assertEqual([85], report["global_allocation_order"])
        self.assertEqual(2, len(report["allocation_attempts"]))

    def test_multireg_width_is_not_mistaken_for_an_allocno(self):
        text = """;; Function wide
;; 2 regs to allocate: 84 (2) 86
;; Register dispositions:
84 in 4  85 in 5  86 in 6
;; Hard regs used: 4 5 6
"""
        attempt = allocations.parse_dump(text, "wide")[0]["allocation_attempts"][0]
        self.assertEqual([84, 86], attempt["allocation_order"])
        self.assertEqual({"84": 2}, attempt["hard_register_widths"])


if __name__ == "__main__":
    unittest.main()
