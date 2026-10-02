#!/usr/bin/env python3
"""Regression tests for tools/split_unit.py: what each part of a split unit keeps."""

from __future__ import annotations

import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

import split_unit  # noqa: E402

UNIT = "game/code_00100000"
SOURCE = """\
#include "common.h"

extern s32 callee(s32);
extern s32 asmOnlyCallee(s32);
extern s32 unrelated(s32);

/* Only reached through a header's `struct HiddenInfo *` member. */
typedef struct HiddenInfo {
    u32 flags;
} HiddenInfo;

void func_00100000(void) {
    callee(1);
}

INCLUDE_ASM(const s32, "game/code_00100000", func_00100010);

/* Old-style definition: the comment must not hide the header from the parser. */
s32 func_00100100(a, b)
    s32 a;
    s32 b;
{
    return a + b;
}

INCLUDE_RODATA(const s32, "game/code_00100000", D_00200000);

void func_00100200(void) {
    callee(2);
}
"""
# Retail code per function: func_00100010 calls asmOnlyCallee, func_00100100 loads D_00200000.
REFS = {
    "func_00100000": {"callee"},
    "func_00100010": {"asmOnlyCallee"},
    "func_00100100": {"D_00200000"},
    "func_00100200": {"callee"},
}


class SplitUnitTests(unittest.TestCase):
    def split(self, refs=None):
        parts, counts, new = split_unit.split_source(SOURCE, {}, 0x100100, 0x100200, UNIT, refs)
        return parts, counts, new

    def test_functions_land_in_their_address_range(self) -> None:
        parts, counts, new = self.split()
        self.assertEqual(counts, {"head": 2, "mid": 1, "tail": 1})
        self.assertEqual(new["mid"], "game/code_00100100")
        self.assertEqual(new["tail"], "game/code_00100200")
        self.assertIn("void func_00100000(void)", parts["head"])
        self.assertIn("func_00100100(a, b)", parts["mid"])
        self.assertIn("void func_00100200(void)", parts["tail"])
        self.assertNotIn("func_00100100", parts["head"])
        self.assertNotIn("func_00100100", parts["tail"])

    def test_commented_knr_definition_stays_in_one_piece(self) -> None:
        mid = self.split()[0]["mid"]
        header = "func_00100100(a, b)\n    s32 a;\n    s32 b;\n{\n    return a + b;\n}"
        self.assertEqual(mid.count(header), 1)
        # Neither a stray parameter line nor a copy of the body leaks into the next part.
        tail = self.split()[0]["tail"]
        self.assertNotIn("s32 b;", tail)
        self.assertNotIn("return a + b;", tail)

    def test_type_named_only_by_its_own_definition_reaches_every_part(self) -> None:
        for key, text in self.split()[0].items():
            self.assertEqual(text.count("} HiddenInfo;"), 1, key)

    def test_declaration_used_only_by_include_asm_follows_its_function(self) -> None:
        without = self.split()[0]
        self.assertNotIn("asmOnlyCallee", without["head"])
        head = self.split(REFS)[0]["head"]
        self.assertIn("extern s32 asmOnlyCallee(s32);", head)
        self.assertNotIn("asmOnlyCallee", self.split(REFS)[0]["tail"])
        self.assertNotIn("unrelated", head)

    def test_include_rodata_goes_to_the_part_whose_function_uses_it(self) -> None:
        line = 'INCLUDE_RODATA(const s32, "{}", D_00200000);'
        parts = self.split()[0]
        self.assertIn(line.format("game/code_00100200"), parts["tail"])  # no references: travels on
        parts = self.split(REFS)[0]
        self.assertIn(line.format("game/code_00100100"), parts["mid"])
        self.assertNotIn("D_00200000", parts["tail"])


if __name__ == "__main__":
    unittest.main()
