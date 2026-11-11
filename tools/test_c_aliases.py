#!/usr/bin/env python3
"""Tests for function_aliases."""
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from c_aliases import function_aliases  # noqa: E402


class FunctionAliasesTest(unittest.TestCase):
    def test_asm_label(self):
        self.assertEqual(function_aliases('s32 foo(void) __asm__("bar");\n'), {"foo": "bar"})

    def test_fakematch_asm_name(self):
        self.assertEqual(function_aliases('s32 foo(void) FAKEMATCH_ASM_NAME("main");\n'), {"foo": "main"})

    def test_object_like_macro(self):
        source = "#define sdf_2C0510 effMiscQuatLengthSqVU\n#define SIZE 0x10\n#define F(x) g(x)\n"
        self.assertEqual(function_aliases(source), {"sdf_2C0510": "effMiscQuatLengthSqVU"})


if __name__ == "__main__":
    unittest.main()
