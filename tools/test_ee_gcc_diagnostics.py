#!/usr/bin/env python3
import hashlib
import importlib.util
import json
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
schedules = load("ee_gcc_schedules", ROOT / "tools/ee_gcc_schedules.py")
why = load("ee_gcc_why", ROOT / "tools/ee_gcc_why.py")


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

    def test_object_record_hashes_present_output(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            obj = root / "candidate.o"
            obj.write_bytes(b"object bytes")
            self.assertEqual(
                {
                    "path": "candidate.o",
                    "size": 12,
                    "sha256": hashlib.sha256(b"object bytes").hexdigest(),
                    "path_normalized_sha256": hashlib.sha256(b"object bytes").hexdigest(),
                },
                probe.object_record(obj, root),
            )
            self.assertIsNone(probe.object_record(root / "missing.o", root))

    def test_object_record_normalizes_wrapper_scratch_directory(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            first = root / "first.o"
            second = root / "second.o"
            first.write_bytes(b"prefix .8b/dds1/game/unit.c suffix")
            second.write_bytes(b"prefix .a9/dds1/game/unit.c suffix")
            a = probe.object_record(first, root)
            b = probe.object_record(second, root)
            self.assertNotEqual(a["sha256"], b["sha256"])
            self.assertEqual(a["path_normalized_sha256"], b["path_normalized_sha256"])


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

    def test_scheduler_verbose_comments_are_observation_noise(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            left, right = root / "left", root / "right"
            left.mkdir()
            right.mkdir()
            body = "\n(insn 6 0 0 (set (reg:SI 2) (const_int 1)))\n"
            (left / "rtl.17.sched").write_text(
                ";; Function wanted\n;; Ready list (t = 0): 6\n" + body
            )
            (right / "rtl.17.sched").write_text(
                ";; Function wanted\n;; verbose dependency table\n"
                ";; call [`renamedNameThatGCCTruncates\n" + body
            )
            report = compare.compare_runs(left, right)
            self.assertIsNone(report["first_semantic_divergence"])
            self.assertEqual("observation-noise", report["passes"][0]["status"])

    def test_scheduler_rtl_difference_remains_semantic(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            left, right = root / "left", root / "right"
            left.mkdir()
            right.mkdir()
            (left / "rtl.25.sched2").write_text(
                ";; verbose text\n(insn 6 0 0 (set (reg:SI 2) (const_int 1)))\n"
            )
            (right / "rtl.25.sched2").write_text(
                ";; other text\n(insn 6 0 0 (set (reg:SI 2) (const_int 2)))\n"
            )
            report = compare.compare_runs(left, right)
            self.assertEqual("rtl.25.sched2", report["first_semantic_divergence"])

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

    ANNULLED_SAMPLE = '''
(insn 122 40 50 (sequence[
            (jump_insn/u:TI 41 40 106 (set (pc)
                    (if_then_else (ne:SI (reg:SI 3 v1)
                            (const_int 0))
                        (label_ref 50)
                        (pc))) 336 (nil)
                (nil))
            (insn/s 106 41 50 (set (reg:DI 16 s0)
                    (mem:DI (reg:SI 4 a0) 0)) 178 (nil)
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
        self.assertFalse(sequences[0]["branch"].get("annulled", False))
        self.assertEqual("always", sequences[0]["slots"][0]["executes_when"])

    def test_analyze_annulled_target_donor(self):
        sequences = delay_slots.analyze_dump(self.ANNULLED_SAMPLE)
        self.assertEqual(1, len(sequences))
        self.assertEqual(122, sequences[0]["wrapper_uid"])
        self.assertTrue(sequences[0]["branch"]["annulled"])
        self.assertTrue(sequences[0]["slots"][0]["from_target"])
        self.assertEqual("taken", sequences[0]["slots"][0]["executes_when"])
        self.assertIn("annulled", delay_slots.render(sequences))
        self.assertIn("executes=taken origin=target", delay_slots.render(sequences))

    def test_analyze_annulled_fallthrough_donor(self):
        sample = self.ANNULLED_SAMPLE.replace("(insn/s 106", "(insn 106")
        sequences = delay_slots.analyze_dump(sample)
        self.assertFalse(sequences[0]["slots"][0]["from_target"])
        self.assertEqual("not_taken", sequences[0]["slots"][0]["executes_when"])


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

    def test_lreg_inputs_explain_priority_and_local_assignments(self):
        lreg = """;; Function sample
Register 84 used 3 times across 12 insns; set 2 times; user var; crosses 2 calls; 4 bytes; GR_REGS or none.
Register 85 used 4 times across 10 insns; set 1 time; 8 bytes; pref GR_REGS, else AP_AND_GR_REGS; pointer.
Register 91 used 2 times across 2 insns in block 1; set 1 time; 4 bytes; GR_REGS or none.
;; Register 91 in 2.
"""
        report = allocations.parse_dump(self.SAMPLE, "sample")
        allocations.enrich_with_lreg(report, lreg, "sample")
        rows = {row["pseudo"]: row for row in report[0]["pseudos"]}
        self.assertEqual(2500, rows[84]["priority_inputs"]["allocation_priority"])
        self.assertEqual(2, rows[84]["priority_inputs"]["calls_crossed"])
        self.assertEqual(8000, rows[85]["priority_inputs"]["allocation_priority"])
        self.assertEqual(8, rows[85]["priority_inputs"]["size_bytes"])
        self.assertEqual(1, rows[85]["priority_inputs"]["hard_register_width"])
        self.assertTrue(rows[85]["priority_inputs"]["pointer"])
        self.assertEqual("AP_AND_GR_REGS", rows[85]["priority_inputs"]["alternate_class"])
        self.assertEqual("local_assignment", rows[91]["allocation_kind"])
        self.assertIsNone(rows[91]["priority_inputs"]["allocation_priority"])

    def test_priority_uses_integer_floor_log2_and_truncates(self):
        self.assertEqual(6000, allocations.allocation_priority(3, 10, 2))
        self.assertIsNone(allocations.allocation_priority(3, 0, 1))

    def test_probe_directory_pairs_pass19_and_pass20(self):
        lreg = """;; Function renamed_sample
Register 84 used 3 times across 12 insns; set 2 times; 4 bytes; GR_REGS or none.
Register 91 used 2 times across 2 insns in block 1; set 1 time; 4 bytes; GR_REGS or none.
;; Register 91 in 2.
"""
        greg = self.SAMPLE.replace(
            ";; Function sample\n;; 2 regs to allocate: 84 85",
            ";; Function renamed_sample\n;; 2 regs to allocate: 84 (2) 85",
        )
        with tempfile.TemporaryDirectory() as directory:
            probe_dir = Path(directory)
            extracted = probe_dir / "functions" / "sample"
            extracted.mkdir(parents=True)
            (extracted / "19.lreg").write_text(lreg)
            (extracted / "20.greg").write_text(greg)
            report = allocations.parse_probe(probe_dir, "sample")[0]

        rows = {row["pseudo"]: row for row in report["pseudos"]}
        self.assertEqual("renamed_sample", report["function"])
        self.assertEqual(2, rows[84]["priority_inputs"]["hard_register_width"])
        self.assertEqual(5000, rows[84]["priority_inputs"]["allocation_priority"])
        self.assertEqual("local_assignment", rows[91]["allocation_kind"])


class ScheduleTests(unittest.TestCase):
    SAMPLE = """;; Function wanted
;;   -- basic block 0 from 1 to 9 -- after reload
;;      insn  code    bb   dep  prio  cost   blockage units
;;      ----  ----    --   ---  ----  ----   -------- -----
;;       35   148     0     3    10     2    0 -  0   memory : 91 85 60 40
;;       37   148     0     3    10     2    0 -  0   memory : 91 88 85 61 40
;;       39   148     0     3    10     2    0 -  0   memory : 91 88 85 61 40
;; Ready list (t =  4):    35  39  37
;; --> scheduling insn <<<37>>> on unit memory
;; Ready list (t =  5):    35  39
;; --> scheduling insn <<<39>>> on unit memory
(insn 37)
"""

    def test_contested_ready_set_and_visible_narrowing(self):
        report = schedules.parse_dump(self.SAMPLE, "wanted")[0]
        self.assertEqual(2, report["contested_count"])
        first = report["blocks"][0]["contested"][0]
        self.assertEqual(37, first["selected_uid"])
        self.assertEqual([35, 39, 37], first["ready_uids"])
        self.assertEqual([39, 37], first["printed_field_heuristic"][1]["uids"])
        self.assertEqual([37], first["printed_field_heuristic"][2]["uids"])

    def test_missing_verbose_records_is_rejected(self):
        with self.assertRaisesRegex(ValueError, "fsched-verbose"):
            schedules.parse_dump(
                ";; Function wanted\n"
                ";; -- basic block 0 from 1 to 2 -- after reload\n"
                "(insn 1)\n",
                "wanted",
            )

    def test_incomplete_ready_set_is_rejected(self):
        sample = self.SAMPLE.replace(
            ";; Ready list (t =  4):    35  39  37",
            ";; Ready list (t =  4):    35  39  99  37",
        )
        with self.assertRaisesRegex(ValueError, "absent from"):
            schedules.parse_dump(sample, "wanted")

    def test_unmatched_selection_is_rejected(self):
        sample = self.SAMPLE.replace(
            ";; --> scheduling insn <<<37>>> on unit memory",
            ";; --> scheduling insn <<<99>>> on unit memory",
        )
        with self.assertRaisesRegex(ValueError, "without a matching ready"):
            schedules.parse_dump(sample, "wanted")

    def test_partial_multiblock_records_are_rejected(self):
        sample = (
            self.SAMPLE
            + "\n;; -- basic block 1 from 10 to 11 -- after reload\n(insn 10)\n"
        )
        with self.assertRaisesRegex(ValueError, "fsched-verbose"):
            schedules.parse_dump(sample, "wanted")

    def test_resolved_instruction_joins_current_ready_set(self):
        sample = """;; Function wanted
;; -- basic block 0 from 1 to 4 -- after reload
;; 1 188 0 0 2 1 0 - 0 alu : 4
;; 2 188 0 0 2 1 0 - 0 alu : 4
;; 4 188 0 1 1 1 0 - 0 alu :
;; Ready list (t = 0): 1 2
;; --> scheduling insn <<<2>>> on unit alu
;; dependences resolved: insn 4 /b1 into ready
;; --> scheduling insn <<<4>>> on unit alu
(insn 4)
"""
        report = schedules.parse_dump(sample, "wanted")[0]
        self.assertEqual(2, report["contested_count"])
        second = report["blocks"][0]["contested"][1]
        self.assertEqual(4, second["selected_uid"])
        self.assertEqual([1, 4], second["ready_uids"])

    def test_sched1_interblock_candidate_marker_is_parsed(self):
        sample = self.SAMPLE.replace(
            ";;       35   148",
            ";;   +   35   148",
        )
        first = schedules.parse_dump(sample, "wanted")[0]["blocks"][0]["contested"][0]
        self.assertEqual([35, 39, 37], first["ready_uids"])

    def test_sched1_ready_block_suffix_is_not_a_uid(self):
        sample = self.SAMPLE.replace(
            ";; Ready list (t =  4):    35  39  37",
            ";; Ready list (t =  4):    35/b1  39/b1  37/b0",
        )
        first = schedules.parse_dump(sample, "wanted")[0]["blocks"][0]["contested"][0]
        self.assertEqual([35, 39, 37], first["ready_uids"])

    def test_whole_dump_skips_function_without_scheduler_records(self):
        sample = ";; Function empty\n(note 1)\n" + self.SAMPLE
        reports = schedules.parse_dump(sample)
        self.assertEqual(["wanted"], [report["function"] for report in reports])

    def test_hidden_comparator_can_override_printed_order(self):
        sample = """;; Function wanted
;; -- basic block 0 from 1 to 2 -- after reload
;; 48 188 0 1 4 2 0 - 0 memory : 90 91
;; 50 188 0 1 4 2 0 - 0 memory : 90 91
;; Ready list (t = 1): 48 50
;; --> scheduling insn <<<50>>> on unit memory
(insn 50)
"""
        choice = schedules.parse_dump(sample, "wanted")[0]["blocks"][0]["contested"][0]
        final = choice["printed_field_heuristic"][-1]
        self.assertEqual("table_order", final["field"])
        self.assertFalse(final["selected_survives"])


class WhyTests(unittest.TestCase):
    ALLOC = """;; Function wanted
;; 1 regs to allocate: 84
;; 84 conflicts: 84 2 28 29
;; Register dispositions:
84 in 4
;; Hard regs used: 4
"""

    def make_probe(self, root, name, passes, assembly="same\n", manifest=None):
        run = root / name
        (run / "functions/wanted").mkdir(parents=True)
        for artifact, text in passes.items():
            (run / "functions/wanted" / artifact).write_text(text)
        (run / "functions/wanted/final.s").write_text(assembly)
        (run / "manifest.json").write_text(json.dumps(manifest or {
            "version": "dds1",
            "as_unit": "src/dds1/sample.c",
            "compiler": {"sha256": "compiler"},
            "extra_cflags": [],
            "wrapper_returncode": 0,
            "cc1_succeeded": True,
            "object": {
                "path": "candidate.o", "size": 1,
                "sha256": name, "path_normalized_sha256": name,
            },
        }))
        return run

    def test_no_difference_stops_source_search(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            left = self.make_probe(root, "left", {"03.cse": "same\n"})
            right = self.make_probe(root, "right", {"03.cse": "same\n"})
            report = why.analyze(left, right, "wanted")
            self.assertEqual("no-codegen-difference", report["diagnosis"]["class"])
            self.assertIn("stop", report["diagnosis"]["next_step"])

    def test_missing_requested_function_is_insufficient_evidence(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            left = self.make_probe(root, "left", {})
            right = self.make_probe(root, "right", {})
            report = why.analyze(left, right, "misspelled")
            self.assertEqual("insufficient-evidence", report["diagnosis"]["class"])
            self.assertIn("neither probe", report["diagnosis"]["basis"][0])

    def test_pass20_includes_both_allocation_summaries(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            left = self.make_probe(root, "left", {
                "19.lreg": "same\n", "20.greg": self.ALLOC + "left\n"
            })
            right = self.make_probe(root, "right", {
                "19.lreg": "same\n", "20.greg": self.ALLOC + "right\n"
            })
            report = why.analyze(left, right, "wanted")
            self.assertEqual("register-allocation", report["diagnosis"]["class"])
            self.assertEqual([84], report["allocation"]["left"][0]["global_allocation_order"])
            self.assertEqual([84], report["allocation"]["right"][0]["global_allocation_order"])

    def test_pass29_includes_both_delay_slot_reports(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            sample = DelaySlotTests.SAMPLE
            left = self.make_probe(root, "left", {"28.mach": "same\n", "29.dbr": sample})
            right = self.make_probe(
                root, "right", {"28.mach": "same\n", "29.dbr": sample.replace("convert", "other")}
            )
            report = why.analyze(left, right, "wanted")
            self.assertEqual("delay-slot-selection", report["diagnosis"]["class"])
            self.assertEqual("convert", report["delay_slots"]["left"][0]["branch"]["target"])
            self.assertEqual("other", report["delay_slots"]["right"][0]["branch"]["target"])

    def test_pass25_includes_both_scheduler_reports(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            left = self.make_probe(root, "left", {
                "22.ce2": "same\n", "25.sched2": ScheduleTests.SAMPLE + "left\n"
            })
            right = self.make_probe(root, "right", {
                "22.ce2": "same\n", "25.sched2": ScheduleTests.SAMPLE + "right\n"
            })
            report = why.analyze(left, right, "wanted")
            self.assertEqual("post-reload-scheduling", report["diagnosis"]["class"])
            self.assertEqual(2, report["schedules"]["left"][0]["contested_count"])

    def test_pass17_includes_both_scheduler_reports(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            sample = ScheduleTests.SAMPLE.replace("after reload", "before reload")
            left = self.make_probe(root, "left", {
                "14.combine": "same\n", "17.sched": sample + "left\n"
            })
            right = self.make_probe(root, "right", {
                "14.combine": "same\n", "17.sched": sample + "right\n"
            })
            report = why.analyze(left, right, "wanted")
            self.assertEqual("pre-reload-scheduling", report["diagnosis"]["class"])
            self.assertEqual(2, report["schedules"]["left"][0]["contested_count"])

    def test_pass25_without_verbose_records_reports_evidence_gap(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            block = ";; Function wanted\n;; -- basic block 0 from 1 to 2 -- after reload\n"
            left = self.make_probe(root, "left", {"25.sched2": block + "(insn 1)\n"})
            right = self.make_probe(root, "right", {"25.sched2": block + "(insn 2)\n"})
            report = why.analyze(left, right, "wanted")
            self.assertIn("evidence_gaps", report)
            self.assertEqual({}, report["schedules"])

    def test_whole_tu_object_difference_after_same_assembly(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            left = self.make_probe(root, "left", {})
            right = self.make_probe(root, "right", {})
            (left / "candidate.s").write_text("same\n")
            (right / "candidate.s").write_text("same\n")
            report = why.analyze(left, right)
            self.assertEqual("object-emission", report["diagnosis"]["class"])
            self.assertEqual("different", report["object"]["status"])

    def test_failed_wrapper_makes_recorded_object_unavailable(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            failed = {
                "wrapper_returncode": 1,
                "cc1_succeeded": True,
                "object": {
                    "path": "candidate.o", "size": 1,
                    "sha256": "partial", "path_normalized_sha256": "partial",
                },
            }
            left = self.make_probe(root, "left", {}, manifest=failed)
            right = self.make_probe(root, "right", {}, manifest=failed)
            self.assertEqual("unavailable", why.analyze(left, right, "wanted")["object"]["status"])

    def test_failed_compiler_is_insufficient_evidence(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            failed = {
                "wrapper_returncode": 1,
                "cc1_succeeded": False,
                "object": None,
            }
            left = self.make_probe(root, "left", {"03.cse": "same\n"}, manifest=failed)
            right = self.make_probe(root, "right", {"03.cse": "same\n"}, manifest=failed)
            report = why.analyze(left, right, "wanted")
            self.assertEqual("insufficient-evidence", report["diagnosis"]["class"])

    def test_extra_cflag_mismatch_is_warned(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            base = {
                "version": "dds1", "as_unit": "src/dds1/sample.c",
                "compiler": {"sha256": "compiler"},
                "wrapper_returncode": 0, "cc1_succeeded": True,
                "object": None,
            }
            left_manifest = {**base, "extra_cflags": []}
            right_manifest = {**base, "extra_cflags": ["-fno-schedule-insns2"]}
            left = self.make_probe(root, "left", {"03.cse": "same\n"}, manifest=left_manifest)
            right = self.make_probe(root, "right", {"03.cse": "same\n"}, manifest=right_manifest)
            report = why.analyze(left, right, "wanted")
            self.assertTrue(any("extra_cflags" in warning for warning in report["warnings"]))
            self.assertEqual("insufficient-evidence", report["diagnosis"]["class"])

    def test_scheduler_verbose_mismatch_does_not_block_stop(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            base = {
                "version": "dds1", "as_unit": "src/dds1/sample.c",
                "compiler": {"sha256": "compiler"},
                "wrapper_returncode": 0, "cc1_succeeded": True,
                "object": None,
            }
            left = self.make_probe(
                root, "left", {"17.sched": ";; quiet\n(insn 1)\n"},
                manifest={**base, "extra_cflags": []},
            )
            right = self.make_probe(
                root, "right", {"17.sched": ";; verbose detail\n(insn 1)\n"},
                manifest={**base, "extra_cflags": ["-fsched-verbose=5"]},
            )
            report = why.analyze(left, right, "wanted")
            self.assertEqual("no-codegen-difference", report["diagnosis"]["class"])

    def test_old_manifest_flags_are_recovered_from_command(self):
        manifest = {
            "compiled_source": "/tmp/input.c",
            "command": ["cc.sh", "-da", "-dumpbase", "/tmp/rtl",
                        "-fsched-verbose=5", "/tmp/input.c", "-o", "/tmp/out.o"],
        }
        self.assertEqual(["-fsched-verbose=5"], why.effective_cflags(manifest))

    def test_function_scope_is_inferred_from_both_manifests(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            manifest = {
                "function": "wanted", "wrapper_returncode": 0,
                "cc1_succeeded": True, "object": None,
            }
            left = self.make_probe(root, "left", {"03.cse": "same\n"}, manifest=manifest)
            right = self.make_probe(root, "right", {"03.cse": "same\n"}, manifest=manifest)
            report = why.analyze(left, right)
            self.assertEqual({"function": "wanted"}, report["comparison"]["scope"])
            self.assertTrue(any("inferred function" in item for item in report["warnings"]))

    def test_function_scope_mismatch_is_rejected(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            left = self.make_probe(root, "left", {}, manifest={"function": "wanted"})
            right = self.make_probe(root, "right", {}, manifest={"function": "other"})
            with self.assertRaisesRegex(ValueError, "scope mismatch"):
                why.analyze(left, right)


if __name__ == "__main__":
    unittest.main()
