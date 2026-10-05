#!/usr/bin/env python3
"""Focused tests for linked-word report reconciliation."""
import copy
import json
import re
import struct
import unittest
from unittest.mock import patch

import reconcile_report
from reconcile_report import (
    MANIFEST, ROOT, address, apply_delta, linked_function_diffs, reconcile,
    symbols, validate_function_mask,
)


def words(*values):
    return struct.pack(f"<{len(values)}I", *values)


class LinkedFunctionTests(unittest.TestCase):
    def compare(self, mine, want, relocs, syms, *, lit4=b"", gp=0, object_gp=0,
                literal_address=0x1080, literal_value=None):
        retail = bytearray(0x100)
        retail[:len(want)] = want
        if literal_value is not None:
            struct.pack_into("<I", retail, literal_address - 0x1000, literal_value)
        return linked_function_diffs(
            "sourceFunction", (0, len(mine)), mine, relocs, lit4, object_gp,
            {"sourceFunction": (0, len(mine))}, syms, bytes(retail),
            [(0x1000, 0, len(retail))], gp, (literal_address, literal_address + 4),
        )

    def test_literal_pool_value_matches_retail_gprel_access(self):
        value = 0x3F800000
        mine = words(0xC7800000)
        want = words(0xC780FF80)
        diffs = self.compare(
            mine, want, {0: ("R_MIPS_LITERAL", ".lit4+0x00004000")},
            {"sourceFunction": 0x1000}, lit4=words(value), gp=0x1100,
            literal_value=value,
        )
        self.assertEqual(diffs, [])

    def test_changed_literal_still_fails(self):
        diffs = self.compare(
            words(0xC7800000), words(0xC780FF80),
            {0: ("R_MIPS_LITERAL", ".lit4+0x00004000")},
            {"sourceFunction": 0x1000}, lit4=words(0x3F800000), gp=0x1100,
            literal_value=0x40000000,
        )
        self.assertRegex(diffs[0], "literal")

    def test_absolute_hi_lo_pair_matches_linked_address(self):
        target = 0x12345670
        mine = words(0x3C040000, 0x24840004)
        want = words(0x3C041234, 0x24845674)
        relocs = {
            0: ("R_MIPS_HI16", "globalTarget"),
            4: ("R_MIPS_LO16", "globalTarget"),
        }
        diffs = self.compare(
            mine, want, relocs,
            {"sourceFunction": 0x1000, "globalTarget": target},
        )
        self.assertEqual(diffs, [])

    def test_changed_address_still_fails(self):
        mine = words(0x3C040000, 0x24840004)
        want = words(0x3C041234, 0x24845674)
        relocs = {
            0: ("R_MIPS_HI16", "globalTarget"),
            4: ("R_MIPS_LO16", "globalTarget"),
        }
        diffs = self.compare(
            mine, want, relocs,
            {"sourceFunction": 0x1000, "globalTarget": 0x12345680},
        )
        self.assertRegex("; ".join(diffs), "address|target")

    def test_changed_hi16_addend_still_fails(self):
        mine = words(0x3C040001, 0x24840004)
        want = words(0x3C041234, 0x24845674)
        relocs = {
            0: ("R_MIPS_HI16", "globalTarget"),
            4: ("R_MIPS_LO16", "globalTarget"),
        }
        diffs = self.compare(
            mine, want, relocs,
            {"sourceFunction": 0x1000, "globalTarget": 0x12345670},
        )
        self.assertRegex("; ".join(diffs), "HI16 address")

    def test_each_pending_hi16_is_checked(self):
        mine = words(0x3C040001, 0x3C050000, 0x24A50004)
        want = words(0x3C041234, 0x3C051234, 0x24A55674)
        relocs = {
            0: ("R_MIPS_HI16", "globalTarget"),
            4: ("R_MIPS_HI16", "globalTarget"),
            8: ("R_MIPS_LO16", "globalTarget"),
        }
        diffs = self.compare(
            mine, want, relocs,
            {"sourceFunction": 0x1000, "globalTarget": 0x12345670},
        )
        self.assertEqual(len(diffs), 1)
        self.assertRegex(diffs[0], r"\+0x0: HI16 address")

    def test_unpaired_hi16_still_fails(self):
        diffs = self.compare(
            words(0x3C040000), words(0x3C041234),
            {0: ("R_MIPS_HI16", "globalTarget")},
            {"sourceFunction": 0x1000, "globalTarget": 0x12345670},
        )
        self.assertRegex(diffs[0], "unpaired HI16")

    def test_changed_register_still_fails_with_equivalent_address(self):
        diffs = self.compare(
            words(0x3C050000, 0x24840004), words(0x3C041234, 0x24845674),
            {0: ("R_MIPS_HI16", "globalTarget"),
             4: ("R_MIPS_LO16", "globalTarget")},
            {"sourceFunction": 0x1000, "globalTarget": 0x12345670},
        )
        self.assertRegex(diffs[0], "instruction shape differs")

    def test_gprel_must_use_the_selected_games_gp(self):
        gp = int(reconcile_report.VERSIONS["dds2"]["gp"], 16)
        syms = {"sourceFunction": 0x1000, "globalTarget": gp + 0x20}
        relocs = {0: ("R_MIPS_GPREL16", "globalTarget")}
        mine, want = words(0x8F820000), words(0x8F820020)
        self.assertEqual(self.compare(mine, want, relocs, syms, gp=gp), [])
        wrong_gp = int(reconcile_report.VERSIONS["dds1"]["gp"], 16)
        self.assertNotEqual(gp, wrong_gp)
        self.assertRegex(self.compare(mine, want, relocs, syms, gp=wrong_gp)[0],
                         "linked target differs")


class FunctionMaskTests(unittest.TestCase):
    def test_source_object_and_fallback_partition_the_denominator(self):
        report = {
            "sourceA": {"name": "sourceA", "size": "8", "fuzzy_match_percent": 99.0},
            "sourceB": {"name": "sourceB", "size": "12", "fuzzy_match_percent": 100.0},
            "fallback": {"name": "fallback", "size": "16"},
        }
        validate_function_mask(
            report, {"sourceA": (0, 8), "sourceB": (8, 12)}, {"fallback": 16}
        )

    def test_fallback_cannot_enter_source_owned_set(self):
        report = {
            "source": {"name": "source", "size": "8"},
            "fallback": {"name": "fallback", "size": "16"},
        }
        with self.assertRaisesRegex(ValueError, "mask changed"):
            validate_function_mask(
                report, {"source": (0, 8), "fallback": (8, 16)}, {"fallback": 16}
            )

    def test_fallback_must_have_zero_credit(self):
        report = {
            "source": {"name": "source", "size": "8", "fuzzy_match_percent": 99.0},
            "fallback": {
                "name": "fallback", "size": "16", "fuzzy_match_percent": 1.0
            },
        }
        with self.assertRaisesRegex(ValueError, "nonzero C credit"):
            validate_function_mask(report, {"source": (0, 8)}, {"fallback": 16})


class ReconciliationScopeTests(unittest.TestCase):
    def setUp(self):
        self.manifest = json.loads(MANIFEST.read_text())

    def report(self, versions, combined):
        measures = {"total_code": "1000000", "matched_code": "100000",
                    "total_functions": 10000, "matched_functions": 100,
                    "fuzzy_match_percent": 10.0}
        categories = ["game", "sdk", "vu1"] + (list(versions) if combined else [])
        return {
            "measures": copy.deepcopy(measures),
            "categories": [{"id": name, "measures": copy.deepcopy(measures)}
                           for name in categories],
            "units": [{"name": f"{version}/effect/effPCPMisc" if combined
                                else "effect/effPCPMisc",
                       "measures": copy.deepcopy(measures),
                       "metadata": {"progress_categories": ["game", version]
                                    if combined else ["game"]}}
                      for version in versions],
        }

    def test_each_game_has_an_explicit_source_and_fallback_mask(self):
        self.assertEqual({spec["version"] for spec in self.manifest["units"]},
                         {"dds1", "dds2"})
        for spec in self.manifest["units"]:
            version = spec["version"]
            with self.subTest(version=version):
                self.assertEqual(spec["source_object"],
                                 f"build/{version}/base/src/{version}/effect/effPCPMisc.o")
                self.assertEqual(spec["expected_total_functions"],
                                 spec["expected_source_functions"] + len(spec["fallback_functions"]))
                self.assertEqual(spec["expected_total_code"],
                                 spec["expected_source_code"] + sum(spec["fallback_functions"].values()))
                self.assertEqual(len(set(spec["corrected_functions"])),
                                 spec["expected_corrected_functions"])
                self.assertFalse(set(spec["corrected_functions"]) & spec["fallback_functions"].keys())
                source = (ROOT / "src" / version / f"{spec['name']}.c").read_text()
                fallback = set(re.findall(r'^INCLUDE_ASM\([^\n]*\b(\w+)\);', source, re.M))
                self.assertEqual(fallback, set(spec["fallback_functions"]))
                syms = symbols(version)
                for name in spec["corrected_functions"]:
                    self.assertIsNotNone(address(name, syms), name)

    def test_corrections_are_scoped_and_keep_denominators(self):
        for versions in (("dds1",), ("dds2",), ("dds1", "dds2")):
            for combined in (False, True):
                if not combined and len(versions) != 1:
                    continue
                with self.subTest(versions=versions, combined=combined):
                    report = self.report(versions, combined)
                    original = copy.deepcopy(report)
                    called = []

                    def correct(_report, unit, spec):
                        called.append(spec["version"])
                        apply_delta(unit["measures"], 12020, 43, 200.0)
                        return 12020, 43, 200.0

                    with patch.object(reconcile_report, "reconcile_unit", side_effect=correct):
                        reconcile(report, "all" if combined else versions[0], self.manifest)
                    self.assertEqual(called, list(versions))
                    self.assertEqual(int(report["measures"]["matched_code"]),
                                     100000 + 12020 * len(versions))
                    before_measures = [original["measures"]] + [c["measures"] for c in original["categories"]]
                    after_measures = [report["measures"]] + [c["measures"] for c in report["categories"]]
                    for before, after in zip(before_measures, after_measures):
                        for denominator in ("total_code", "total_functions"):
                            self.assertEqual(after[denominator], before[denominator])
                    for before, after in zip(original["categories"], report["categories"]):
                        if after["id"] in ("sdk", "vu1"):
                            self.assertEqual(before, after)

    def test_missing_unit_in_a_present_version_still_fails(self):
        report = self.report(("dds2",), True)
        report["units"][0]["name"] = "dds2/game/somethingElse"
        with self.assertRaisesRegex(SystemExit, "configured report unit is missing"):
            reconcile(report, "all", self.manifest)

    def test_per_version_report_cannot_silently_skip_a_missing_unit(self):
        report = self.report(("dds2",), False)
        report["units"] = []
        with self.assertRaisesRegex(SystemExit, "configured report unit is missing"):
            reconcile(report, "dds2", self.manifest)

    def test_empty_combined_report_still_fails(self):
        report = self.report((), True)
        with self.assertRaisesRegex(SystemExit, "contains no configured games"):
            reconcile(report, "all", self.manifest)


if __name__ == "__main__":
    unittest.main()
