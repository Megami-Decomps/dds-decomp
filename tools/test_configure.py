#!/usr/bin/env python3
"""Regression tests for source discovery and progress configuration/labels."""

from __future__ import annotations

import io
import json
import sys
import tempfile
import types
import unittest
from contextlib import redirect_stdout
from pathlib import Path
from unittest.mock import patch


ROOT = Path(__file__).resolve().parents[1]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

# Source-discovery helpers do not use Ninja, and CI runs these tests before
# installing build dependencies.
sys.modules.setdefault("ninja_syntax", types.ModuleType("ninja_syntax"))

import configure  # noqa: E402
from tools import progress  # noqa: E402


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


class ObjdiffProgressTests(unittest.TestCase):
    def setUp(self) -> None:
        self.units = {}
        for version in ("dds1", "dds2"):
            self.units[version] = [
                {
                    "name": f"src/{version}/game/example",
                    "target": f"build/{version}/target/game/example.o",
                    "base": f"build/{version}/base/game/example.o",
                },
                {
                    "name": f"asm/{version}/sdk/libc",
                    "target": f"build/{version}/sdk/libc.o",
                    "base": None,
                },
                {
                    "name": f"asm/{version}/data/vutext",
                    "target": f"build/{version}/data/vutext.o",
                    "base": None,
                },
                {
                    "name": f"src/{version}/effect/effVU0",
                    "target": f"build/{version}/target/effect/effVU0.o",
                    "base": f"build/{version}/base/effect/effVU0.o",
                },
                {
                    "name": f"asm/{version}/game/unmatched",
                    "target": f"build/{version}/game/unmatched.o",
                    "base": None,
                },
            ]
        with tempfile.TemporaryDirectory() as tmp, patch.object(configure, "ROOT", Path(tmp)):
            configure.write_objdiff(self.units)
            self.combined = json.loads((Path(tmp) / "objdiff.json").read_text())
            self.versions = {
                version: json.loads((Path(tmp) / "build" / version / "objdiff.json").read_text())
                for version in self.units
            }

    def test_combined_config_retains_every_unit_and_object_path(self) -> None:
        rows = [row for rows in self.units.values() for row in rows]
        self.assertEqual(len(self.combined["units"]), len(rows))
        for actual, row in zip(self.combined["units"], rows):
            self.assertEqual(actual["name"], row["name"].split("/", 1)[1])
            self.assertEqual(actual["target_path"], row["target"])
            self.assertEqual(actual.get("base_path"), row["base"])
            self.assertNotIn("complete", actual)
            self.assertNotIn("complete", actual["metadata"])

    def test_categories_separate_vu1_and_preserve_sdk_and_ee_game_code(self) -> None:
        self.assertEqual(
            [category["id"] for category in self.combined["progress_categories"]],
            ["dds1", "dds2", "game", "sdk", "vu1"],
        )
        expected = ["game", "sdk", "vu1", "game", "game"]
        for index, version in enumerate(self.units):
            for unit, category in zip(self.combined["units"][index * 5:(index + 1) * 5], expected):
                self.assertEqual(unit["metadata"]["progress_categories"], [version, category])

    def test_per_version_configs_rebase_paths_and_keep_all_categories(self) -> None:
        for version, config in self.versions.items():
            self.assertEqual(
                [category["id"] for category in config["progress_categories"]],
                ["game", "sdk", "vu1"],
            )
            self.assertEqual(len(config["units"]), len(self.units[version]))
            for actual, row, category in zip(
                config["units"], self.units[version], ["game", "sdk", "vu1", "game", "game"]
            ):
                self.assertEqual(actual["name"], row["name"].split("/", 2)[2])
                self.assertEqual(actual["target_path"], "../../" + row["target"])
                self.assertEqual(
                    actual.get("base_path"), "../../" + row["base"] if row["base"] else None
                )
                self.assertEqual(actual["metadata"], {"progress_categories": [category]})
                self.assertNotIn("complete", actual)


class ProgressLabelTests(unittest.TestCase):
    def test_text_labels_source_coverage(self) -> None:
        out = io.StringIO()
        with patch.object(sys, "argv", ["progress.py", "dds1"]), \
                patch.object(progress, "version_stats", return_value=(4, 2, 100, 25)), \
                redirect_stdout(out):
            progress.main()
        self.assertEqual(
            out.getvalue(),
            "dds1: 2/4 functions in C (50.0%), 25/100 code bytes in C (25.0%)\n",
        )

    def test_markdown_keeps_source_coverage_labels_and_values(self) -> None:
        out = io.StringIO()
        with patch.object(sys, "argv", ["progress.py", "dds1", "--markdown"]), \
                patch.object(progress, "version_stats", return_value=(4, 2, 100, 25)), \
                redirect_stdout(out):
            progress.main()
        self.assertEqual(
            out.getvalue(),
            "| Version | Functions in C | Code bytes in C |\n|---|---|---|\n"
            "| `dds1` | 2 / 4 (50.0%) | 25 / 100 (25.0%) |\n",
        )


if __name__ == "__main__":
    unittest.main()
