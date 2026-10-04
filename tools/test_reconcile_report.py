#!/usr/bin/env python3
"""Focused tests for linked-word report reconciliation."""
import struct
import unittest

from reconcile_report import linked_function_diffs, validate_function_mask


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


if __name__ == "__main__":
    unittest.main()
