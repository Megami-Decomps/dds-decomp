#!/usr/bin/env python3
"""Regression tests for source discovery and progress configuration/labels."""

from __future__ import annotations

import io
import json
import os
import sys
import tempfile
import types
import unittest
from contextlib import redirect_stdout
from pathlib import Path
from unittest.mock import Mock, patch


ROOT = Path(__file__).resolve().parents[1]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

# Source-discovery helpers do not use Ninja, and CI runs these tests before
# installing build dependencies.
sys.modules.setdefault("ninja_syntax", types.ModuleType("ninja_syntax"))

import configure  # noqa: E402
from tools import progress  # noqa: E402


class ConfigureTests(unittest.TestCase):
    def test_one_shot_force_split_is_not_persisted_and_cached_split_is_reused(self) -> None:
        writer = Mock()
        configure.write_configure_build(
            writer, ["config/dds1/main.yaml"], ["dds1", "--force-split", "--no-split"]
        )
        self.assertEqual(
            writer.build.call_args,
            (("build.ninja", "configure"), {
                "implicit": ["config/dds1/main.yaml"],
                "variables": {"args": "dds1 --no-split"},
            }),
        )

        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            config = root / "config/dds1"
            build = root / "build/dds1"
            config.mkdir(parents=True)
            build.mkdir(parents=True)
            yaml = config / "main.yaml"
            yaml.write_text("version: dds1\n")
            (config / "symbol_addrs.txt").write_text("symbol = 0x1;\n")
            (config / "reloc_addrs.txt").write_text("reloc = 0x2;\n")
            (build / "main.ld").write_text("main_SBSS_START = .;\nmain_BSS_START = .;\n")

            def run_forced_step(command, **kwargs):
                if "splat" in command:
                    (root / "asm/dds1").mkdir(parents=True, exist_ok=True)

            with patch.object(configure, "ROOT", root), \
                    patch.dict(configure.VERSIONS, {"dds1": {"serial": "main"}}), \
                    patch.object(configure.subprocess, "run", side_effect=run_forced_step) as run:
                configure.run_splat("dds1", yaml, force=True)
                forced_run_count = run.call_count
                self.assertGreater(forced_run_count, 0)

                # Ninja's regeneration command omits the one-shot flag. With
                # the stamp written by the forced run, its ordinary configure
                # pass follows the real cache gate and does not split again.
                configure.run_splat("dds1", yaml, force=False)
                self.assertEqual(run.call_count, forced_run_count)

    def test_timestamp_restore_only_reverts_unchanged_existing_files(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            unchanged = root / "unchanged.s"
            changed = root / "changed.s"
            deleted = root / "deleted.s"
            new = root / "new.s"
            unchanged.write_bytes(b"same")
            changed.write_bytes(b"before")
            deleted.write_bytes(b"gone")
            old_times = (1_600_000_000_123_456_789, 1_600_000_001_987_654_321)
            for path in (unchanged, changed, deleted):
                os.utime(path, ns=old_times)

            snapshot = configure.snapshot_generated_files([root])
            unchanged.unlink()
            unchanged.write_bytes(b"same")
            changed.write_bytes(b"after")
            deleted.unlink()
            new.write_bytes(b"new")
            fresh_times = (1_700_000_000_111_222_333, 1_700_000_001_444_555_666)
            for path in (unchanged, changed, new):
                os.utime(path, ns=fresh_times)

            configure.restore_unchanged_timestamps(snapshot)

            self.assertEqual(
                (unchanged.stat().st_atime_ns, unchanged.stat().st_mtime_ns), old_times
            )
            self.assertEqual(changed.stat().st_mtime_ns, fresh_times[1])
            self.assertEqual(new.stat().st_mtime_ns, fresh_times[1])
            self.assertFalse(deleted.exists())

    def test_timestamp_snapshot_skips_symlinks(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            target = root / "target.s"
            link = root / "link.s"
            target.write_bytes(b"target")
            link.symlink_to(target)

            snapshot = configure.snapshot_generated_files([link])

            self.assertEqual(snapshot, {})
            self.assertTrue(link.is_symlink())

    def test_timestamp_restore_does_not_follow_replacement_symlink(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            path = root / "generated.s"
            target = root / "target.s"
            path.write_bytes(b"same")
            target.write_bytes(b"same")
            snapshot = configure.snapshot_generated_files([path])
            path.unlink()
            path.symlink_to(target)
            link_times = (1_700_000_000_111_222_333, 1_700_000_001_444_555_666)
            os.utime(path, ns=link_times, follow_symlinks=False)
            target_mtime = target.stat().st_mtime_ns

            configure.restore_unchanged_timestamps(snapshot)

            self.assertTrue(path.is_symlink())
            self.assertEqual(path.lstat().st_mtime_ns, link_times[1])
            self.assertEqual(target.stat().st_mtime_ns, target_mtime)

    def test_field_archive_bases_are_available_when_all_expected_files_exist(self) -> None:
        with tempfile.TemporaryDirectory() as tmp, patch.object(configure, "ROOT", Path(tmp)):
            sources = [Path("f001_000.lbasm"), Path("f002_000.lbasm")]
            base_dir = Path(tmp) / "orig" / "dds1" / "field"
            base_dir.mkdir(parents=True)
            for source in sources:
                (base_dir / f"{source.stem}.LB").touch()

            self.assertEqual(configure.missing_field_archive_bases("dds1", sources), [])

    def test_field_archive_bases_report_a_partial_input_set(self) -> None:
        with tempfile.TemporaryDirectory() as tmp, patch.object(configure, "ROOT", Path(tmp)):
            sources = [Path("f001_000.lbasm"), Path("f002_000.lbasm")]
            base_dir = Path(tmp) / "orig" / "dds1" / "field"
            base_dir.mkdir(parents=True)
            (base_dir / "f001_000.LB").touch()

            self.assertEqual(
                configure.missing_field_archive_bases("dds1", sources),
                [Path("orig/dds1/field/f002_000.LB")],
            )

    def test_unrelated_field_archive_does_not_satisfy_expected_base(self) -> None:
        with tempfile.TemporaryDirectory() as tmp, patch.object(configure, "ROOT", Path(tmp)):
            base_dir = Path(tmp) / "orig" / "dds2" / "field"
            base_dir.mkdir(parents=True)
            (base_dir / "unrelated.LB").touch()

            self.assertEqual(
                configure.missing_field_archive_bases(
                    "dds2", [Path("f001_000.lbasm")]
                ),
                [Path("orig/dds2/field/f001_000.LB")],
            )

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

    def test_replacement_includes_allow_explicitly_retained_small_data(self) -> None:
        includes = [
            ("ASM", "game/example", "fallback"),
            ("SDATA", "game/example", "small_data"),
        ]
        configure.validate_replacement_includes(
            includes,
            ["fallback"],
            [{"section": ".sdata", "size": 4}],
            context="test replacement",
        )

    def test_replacement_includes_reject_unretained_small_data(self) -> None:
        includes = [("SDATA", "game/example", "small_data")]
        with self.assertRaisesRegex(SystemExit, "without retaining .sdata"):
            configure.validate_replacement_includes(
                includes, [], [], context="test replacement"
            )

    def test_replacement_includes_reject_empty_retained_small_data(self) -> None:
        includes = [("SDATA", "game/example", "small_data")]
        with self.assertRaisesRegex(SystemExit, "without retaining .sdata"):
            configure.validate_replacement_includes(
                includes,
                [],
                [{"section": ".sdata", "size": 0}],
                context="test replacement",
            )

    def test_replacement_includes_still_reject_rodata_fallbacks(self) -> None:
        includes = [("RODATA", "game/example", "table")]
        with self.assertRaisesRegex(SystemExit, "unsupported fallback"):
            configure.validate_replacement_includes(
                includes, [], [], context="test replacement"
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
            self.primary_versions = {
                version: json.loads((Path(tmp) / "build" / version / "progress" / "objdiff.json").read_text())
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

    def test_primary_configs_keep_all_game_units_and_exclude_sdk_and_vu1(self) -> None:
        for version, config in self.primary_versions.items():
            self.assertEqual(config["progress_categories"], [{"id": "game", "name": "Atlus game/engine"}])
            # Keep both C and unfinished EE game units, including VU0 helpers.
            expected = [self.units[version][index] for index in (0, 3, 4)]
            self.assertEqual(len(config["units"]), len(expected))
            for actual, row in zip(config["units"], expected):
                self.assertEqual(actual["name"], row["name"].split("/", 2)[2])
                self.assertEqual(actual["target_path"], "../../../" + row["target"])
                self.assertEqual(
                    actual.get("base_path"), "../../../" + row["base"] if row["base"] else None
                )
                self.assertEqual(actual["metadata"], {"progress_categories": ["game"]})
                self.assertNotIn("complete", actual)
            self.assertEqual(len(self.versions[version]["units"]), 5)

    def test_report_targets_use_matching_projects_and_object_dependencies(self) -> None:
        writer = Mock()
        configure.write_objdiff_reports(writer, self.units)
        builds = {call.args[0]: call for call in writer.build.call_args_list}
        all_objects = sorted({p for rows in self.units.values() for row in rows
                              for p in (row["target"], row["base"]) if p})
        self.assertEqual(builds["objdiff"].args, ("objdiff", "phony", all_objects))
        self.assertEqual(builds["report.json.raw"].kwargs, {
            "implicit": all_objects + ["objdiff.json"], "variables": {"project": "."},
        })
        self.assertEqual(builds["report.json"].args, ("report.json", "reconcile_report", "report.json.raw"))
        self.assertEqual(builds["report.json"].kwargs["variables"], {"scope": "all"})
        expected_reports = {"report.json"}
        for version, rows in self.units.items():
            for selected, project, out in (
                (rows, f"build/{version}", f"build/{version}/report.all.json"),
                ([rows[index] for index in (0, 3, 4)], f"build/{version}/progress", f"build/{version}/report.json"),
            ):
                expected_reports.add(out)
                objects = sorted({p for row in selected for p in (row["target"], row["base"]) if p})
                self.assertEqual(builds[f"{out}.raw"].args, (f"{out}.raw", "objdiff_report"))
                self.assertEqual(builds[f"{out}.raw"].kwargs, {
                    "implicit": objects + [f"{project}/objdiff.json"], "variables": {"project": project},
                })
                self.assertEqual(builds[out].args, (out, "reconcile_report", f"{out}.raw"))
                self.assertEqual(builds[out].kwargs["variables"], {"scope": version})
        self.assertEqual(set(builds["report"].args[2]), expected_reports)
        writer.rule.assert_any_call(
            "objdiff_report", f"{configure.OBJDIFF} report generate -p $project -o $out",
            description="objdiff report $out"
        )

    def test_reconciliation_dependencies_follow_report_versions(self) -> None:
        for versions in (("dds1",), ("dds2",), ("dds1", "dds2")):
            with self.subTest(versions=versions):
                writer = Mock()
                configure.write_objdiff_reports(
                    writer, {version: self.units[version] for version in versions}
                )
                for call in writer.build.call_args_list:
                    if call.args[1] != "reconcile_report":
                        continue
                    scope = call.kwargs["variables"]["scope"]
                    expected_versions = versions if scope == "all" else (scope,)
                    dependencies = call.kwargs["implicit"]
                    for version in ("dds1", "dds2"):
                        for dependency in (
                            f"build/{version}/base/src/{version}/effect/effPCPMisc.o",
                            f"config/{version}/symbol_addrs.txt",
                            f"orig/{version}/{configure.VERSIONS[version]['serial']}",
                        ):
                            self.assertEqual(dependency in dependencies,
                                             version in expected_versions)


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
