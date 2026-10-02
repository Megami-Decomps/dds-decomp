#!/usr/bin/env python3
import importlib.util
import json
import sys
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parent.parent


def load():
    path = ROOT / "tools/ee_gcc_source_locations.py"
    spec = importlib.util.spec_from_file_location("ee_gcc_source_locations", path)
    assert spec and spec.loader
    module = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = module
    spec.loader.exec_module(module)
    return module


locations = load()


class SourceLocationTests(unittest.TestCase):
    SAMPLE = '''
.stabs "type:t1=r1;0;127;",128,0,0,0
.stabs "first:F20",36,0,0,first
.stabs "arg:P1",64,0,0,18
.stabs "weight:r2",64,0,0,39
.stabs "scratch:3",128,0,0,-32
.stabs "arg:p1",160,0,0,16
.stabs "empty:F20",36,0,0,empty
'''

    def test_register_stack_and_duplicate_records(self):
        report = locations.parse_assembly(self.SAMPLE, "first")
        self.assertEqual(1, len(report))
        rows = report[0]["locations"]
        self.assertEqual("$s2", rows[0]["register"])
        self.assertEqual("$f1", rows[1]["register"])
        self.assertEqual(-32, rows[2]["stack_offset"])
        self.assertEqual("parameter", rows[3]["kind"])
        self.assertEqual(["arg", "weight", "scratch", "arg"], [row["name"] for row in rows])

    def test_typedefs_are_not_stack_locations(self):
        report = locations.parse_assembly(self.SAMPLE)
        self.assertEqual([], report[-1]["locations"])
        self.assertFalse(any(row["name"] == "type" for item in report for row in item["locations"]))

    def test_missing_function_fails(self):
        with self.assertRaises(locations.SourceLocationError):
            locations.parse_assembly(self.SAMPLE, "missing")

    def test_probe_requires_stabs_flag(self):
        with tempfile.TemporaryDirectory() as directory:
            probe = Path(directory)
            (probe / "manifest.json").write_text(json.dumps({
                "extra_cflags": [], "cc1_succeeded": True,
            }))
            (probe / "candidate.s").write_text(self.SAMPLE)
            with self.assertRaises(locations.SourceLocationError):
                locations.parse_probe(probe)

    def test_probe_rejects_non_object_manifest(self):
        with tempfile.TemporaryDirectory() as directory:
            probe = Path(directory)
            (probe / "manifest.json").write_text("[]\n")
            with self.assertRaises(locations.SourceLocationError):
                locations.parse_probe(probe)


if __name__ == "__main__":
    unittest.main()
