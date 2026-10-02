#!/usr/bin/env python3
"""Regression tests for configure-time source discovery."""

from __future__ import annotations

import sys
import types
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

# Source-discovery helpers do not use Ninja, and CI runs these tests before
# installing build dependencies.
sys.modules.setdefault("ninja_syntax", types.ModuleType("ninja_syntax"))

import configure  # noqa: E402


class ConfigureTests(unittest.TestCase):
    def test_include_directive_accepts_leading_and_internal_comments(self) -> None:
        source = """\
/* retained table */ INCLUDE_RODATA /* generated */ (
    const s32, "script/example", table
);
"""
        self.assertEqual(
            configure.include_directives(source, context="test"),
            [("RODATA", "script/example", "table")],
        )

    def test_include_directive_accepts_whitespace_before_parenthesis(self) -> None:
        source = 'INCLUDE_ASM \n (const s32, "script/example", fallback);'
        self.assertEqual(
            configure.include_directives(source, context="test"),
            [("ASM", "script/example", "fallback")],
        )

    def test_include_identifier_honors_c_line_splicing(self) -> None:
        source = """\
INCLUDE_\\
RODATA(const s32, "script/example", table);
INCLUDE\\
_ASM(const s32, "script/example", fallback);
"""
        self.assertEqual(
            configure.include_directives(source, context="test"),
            [
                ("RODATA", "script/example", "table"),
                ("ASM", "script/example", "fallback"),
            ],
        )

    def test_include_tokens_in_comments_and_literals_are_ignored(self) -> None:
        source = """\
// INCLUDE_RODATA(const s32, "hidden", table);
const char *text = "INCLUDE_SDATA(hidden)";
/* INCLUDE_ASM(hidden) */
"""
        self.assertEqual(configure.include_directives(source, context="test"), [])

    def test_malformed_include_token_is_rejected(self) -> None:
        with self.assertRaisesRegex(SystemExit, "unsupported INCLUDE_ASM syntax"):
            configure.include_directives(
                'INCLUDE_ASM + (const s32, "script/example", fallback);',
                context="test",
            )


if __name__ == "__main__":
    unittest.main()
