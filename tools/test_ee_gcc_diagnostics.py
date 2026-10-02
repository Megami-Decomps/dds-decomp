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


if __name__ == "__main__":
    unittest.main()
