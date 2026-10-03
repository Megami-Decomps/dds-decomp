from __future__ import annotations

import sys
import tempfile
import unittest
from collections import Counter
from dataclasses import replace
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

import field_graph  # noqa: E402
import flw0_profiles  # noqa: E402
import inf  # noqa: E402
import wap  # noqa: E402


class FieldGraphTests(unittest.TestCase):
    def test_links_complete_maintained_event_corpora(self) -> None:
        expected = {
            "dds1": (
                24, 104, 1528, 165, 607, 12, 54, 6, 48, 69, 53, 16, 24, 19
            ),
            "dds2": (
                22, 103, 1711, 135, 723, 10, 52, 19, 33, 79, 64, 15, 22, 10
            ),
        }
        for game, counts in expected.items():
            with self.subTest(game=game):
                root = ROOT / f"src/{game}"
                sections = field_graph._event_sections(
                    sorted((root / "data/field").glob("*.fldasm")),
                    root / "scripts/field",
                    flw0_profiles.get(game),
                )
                summary = sections["eventSummary"]
                selected_sites = sum(
                    edge["count"]
                    for edge in sections["eventScriptEdges"]
                    if edge["command"] == "SUBMIT_EVENT_WITH_SELECTION"
                )
                self.assertEqual(
                    (
                        summary["fieldScripts"],
                        summary["eventScripts"],
                        summary["fieldProcedureNodes"],
                        summary["eventProcedureNodes"],
                        summary["reachableProcedures"],
                        summary["reachableEventScripts"],
                        summary["eventScriptEdges"],
                        summary["fieldEventScriptEdges"],
                        summary["eventEventScriptEdges"],
                        summary["eventRequestEdges"],
                        summary["fieldEventRequestEdges"],
                        summary["eventEventRequestEdges"],
                        summary["unresolvedScriptTargets"],
                        selected_sites,
                    ),
                    counts,
                )
                self.assertTrue(
                    all(
                        edge["targetPresent"]
                        for edge in sections["eventScriptEdges"]
                        if edge["command"] == "SUBMIT_EVENT_WITH_SELECTION"
                    )
                )
                self.assertFalse(
                    any(
                        edge["command"] == "SUBMIT_EVENT"
                        for edge in sections["eventScriptEdges"]
                    )
                )
                self.assertTrue(
                    any(
                        edge["command"] == "SUBMIT_EVENT"
                        for edge in sections["eventRequestEdges"]
                    )
                )
                self.assertEqual(
                    {
                        row.get("command")
                        for row in sections["scriptUnresolvedTargets"]
                    },
                    {"SUBMIT_EVENT_IMMEDIATE"},
                )

    def test_resolves_deferred_battle_exits_through_wap_rows(self) -> None:
        field_dir = ROOT / "src/dds1/data/field"
        script_source = ROOT / "src/dds1/scripts/field/f022.bfasm"
        wap_source = field_dir / "f022.wapasm"
        references = wap.load_references(
            script_source, field_dir / "f022.infasm"
        )
        table = wap.parse_source(
            wap_source.read_text(encoding="utf-8"), references
        )
        with tempfile.TemporaryDirectory() as temporary:
            script_dir = Path(temporary) / "scripts/field"
            script_dir.mkdir(parents=True)
            (script_dir.parent / "event").mkdir()
            (script_dir / script_source.name).write_text(
                script_source.read_text(encoding="utf-8"), encoding="utf-8"
            )
            sections = field_graph._event_sections(
                sorted(field_dir.glob("f022_*.fldasm")),
                script_dir,
                flw0_profiles.get("dds1"),
                {22: table},
            )

        self.assertEqual(
            [edge["target"] for edge in sections["deferredBattleExitEdges"]],
            ["f022_010", "f022_011", "f022_028", "f022_031"],
        )
        self.assertEqual(
            [
                edge["candidates"][0]["entry"]
                for edge in sections["deferredBattleExitEdges"]
            ],
            [1, 2, 6, 7],
        )
        self.assertEqual(
            sections["eventSummary"]["resolvedDeferredBattleExitSites"], 4
        )

    def test_links_event_placements_to_reachable_script_procedures(self) -> None:
        source_script = ROOT / "src/dds1/scripts/field/f011.bfasm"
        field_source = ROOT / "src/dds1/data/field/f011_001.fldasm"
        with tempfile.TemporaryDirectory() as temporary:
            script_dir = Path(temporary) / "scripts/field"
            script_dir.mkdir(parents=True)
            (script_dir.parent / "event").mkdir()
            (script_dir / "f011.bfasm").write_text(
                source_script.read_text(encoding="utf-8"), encoding="utf-8"
            )
            sections = field_graph._event_sections(
                [field_source], script_dir, flw0_profiles.get("dds1")
            )

        self.assertEqual(
            {
                key: sections["eventSummary"][key]
                for key in (
                    "fieldScripts",
                    "eventPlacements",
                    "linkedEventPlacements",
                    "entryProcedures",
                    "procedureNodes",
                    "reachableProcedures",
                    "unresolvedScriptTargets",
                )
            },
            {
                "fieldScripts": 1,
                "eventPlacements": 2,
                "linkedEventPlacements": 2,
                "entryProcedures": 2,
                "procedureNodes": 43,
                "reachableProcedures": 2,
                "unresolvedScriptTargets": 1,
            },
        )
        self.assertEqual(
            [row["label"] for row in sections["eventEntries"]],
            ["001_01eve_01", "001_01eve_02"],
        )
        graph = {key: value for key, value in sections.items() if key != "eventSummary"}
        dot = field_graph._render_event_dot(graph, "f011")
        self.assertIn('"f011_001:event-placement:4" -> "f011:procedure:38"', dot)
        self.assertIn("001_01eve_02 [39]", dot)

    def test_follows_event_scripts_from_field_placements(self) -> None:
        field_dir = ROOT / "src/dds1/data/field"
        source_script = ROOT / "src/dds1/scripts/field/f025.bfasm"
        event_source_dir = ROOT / "src/dds1/scripts/event"
        with tempfile.TemporaryDirectory() as temporary:
            script_dir = Path(temporary) / "scripts/field"
            event_dir = script_dir.parent / "event"
            script_dir.mkdir(parents=True)
            event_dir.mkdir()
            (script_dir / source_script.name).write_text(
                source_script.read_text(encoding="utf-8"), encoding="utf-8"
            )
            for event_name in ("e632", "e633", "e634", "e635"):
                source = event_source_dir / f"{event_name}.bfasm"
                (event_dir / source.name).write_text(
                    source.read_text(encoding="utf-8"), encoding="utf-8"
                )
            sections = field_graph._event_sections(
                sorted(field_dir.glob("f025_*.fldasm")),
                script_dir,
                flw0_profiles.get("dds1"),
            )

        reachable_events = {
            row["script"]
            for row in sections["scriptProcedures"]
            if row["scriptType"] == "event" and row["reachableFromPlacement"]
        }
        self.assertEqual(reachable_events, {"e632", "e633", "e634", "e635"})
        routes = {
            (edge["source"], edge["target"], edge["command"])
            for edge in sections["eventScriptEdges"]
            if edge["sourceReachable"] and edge["targetPresent"]
        }
        self.assertIn(
            ("f025:procedure:246", "e632:procedure:0", "CALL_EVENT"), routes
        )
        self.assertIn(
            (
                "e632:procedure:0",
                "e633:procedure:0",
                "SUBMIT_EVENT_WITH_SELECTION",
            ),
            routes,
        )
        self.assertIn(
            ("e633:procedure:0", "e634:procedure:0", "CALL_EVENT"), routes
        )
        self.assertIn(
            (
                "e634:procedure:0",
                "e635:procedure:0",
                "SUBMIT_EVENT_WITH_SELECTION",
            ),
            routes,
        )

    def test_builds_present_missing_and_conditional_edges(self) -> None:
        table = wap.default_file(wap.PROFILES["dds1"])
        entries = list(table.entries)
        entries[2] = replace(
            entries[2],
            kind=1,
            area=1,
            name=wap.FixedString("01d_01"),
            warp_args=(0, 2, 0),
        )
        entries[3] = replace(
            entries[3],
            kind=1,
            area=1,
            name=wap.FixedString("01d_01"),
            flag_mode=2,
            flag=17,
            warp_args=(24, 3, 0),
        )
        entries[4] = replace(
            entries[4],
            kind=6,
            area=1,
            name=wap.FixedString("elevator"),
            warp_type=1,
            warp_args=(2, 4, 0),
        )
        table = replace(table, entries=tuple(entries))

        graph = field_graph.build_graph(
            {"f011_001", "f011_002"},
            {11: table},
        )

        self.assertEqual(
            graph["summary"],
            {
                "fieldSources": 2,
                "transitionTables": 1,
                "areaNodes": 3,
                "fieldTransitions": 2,
                "sourcePresent": 2,
                "targetPresent": 1,
                "conditionalTransitions": 1,
                "multiTransitionActors": 1,
            },
        )
        self.assertEqual(
            [edge["target"] for edge in graph["transitions"]],
            ["f011_002", "f024_003"],
        )
        self.assertEqual(graph["transitions"][1]["gate"], {"mode": 2, "flag": 17})
        missing = next(area for area in graph["areas"] if area["id"] == "f024_003")
        self.assertFalse(missing["hasFieldSource"])

        dot = field_graph.render_dot(graph)
        self.assertIn('"f011_001" -> "f011_002"', dot)
        self.assertIn('"f024_003" [label="f024_003", style="dashed"', dot)
        self.assertIn("01d_01 [3] flag 17 mode 2", dot)

    def test_links_interaction_state_flow_to_wap_destinations(self) -> None:
        transition_table = wap.default_file(wap.PROFILES["dds1"])
        entries = list(transition_table.entries)
        entries[2] = replace(
            entries[2],
            kind=1,
            area=1,
            name=wap.FixedString("01d_01"),
            warp_args=(0, 2, 0),
        )
        entries[3] = replace(
            entries[3],
            kind=1,
            area=1,
            name=wap.FixedString("01d_01"),
            flag_mode=2,
            flag=17,
            warp_args=(24, 3, 0),
        )
        transition_table = replace(transition_table, entries=tuple(entries))

        flags = list(inf.DEFAULT_SET.flags)
        flags[0] = inf.FlagSelector(23, 12, 100)
        rows = list(inf.DEFAULT_SET.messages)
        rows[2] = inf.MessageRow(1, 7, (100, 0, 2, 12), 4, 5, 0, 0)
        sets = list(inf.DEFAULT_FILE.sets)
        sets[1] = inf.InteractionSet(
            inf.Start(1, 0, 1, 2, "01d_01"), tuple(flags), tuple(rows)
        )
        interaction_table = replace(inf.DEFAULT_FILE, sets=tuple(sets))

        graph = field_graph.build_graph(
            {"f011_001", "f011_002"},
            {11: transition_table},
            {"f011_001": Counter({"01d_01": 1})},
            {11: interaction_table},
            {11: (None,) * 7 + ("OPEN_DOOR",)},
        )

        self.assertEqual(graph["schema"], "dds-field-world-2")
        self.assertEqual(
            {
                key: graph["summary"][key]
                for key in (
                    "interactionSets",
                    "linkedInteractionSets",
                    "flagSelectors",
                    "stateRows",
                    "stateEdges",
                    "warpSets",
                    "linkedWarpSets",
                    "warpHandoffs",
                    "fieldWarpHandoffs",
                )
            },
            {
                "interactionSets": 1,
                "linkedInteractionSets": 1,
                "flagSelectors": 1,
                "stateRows": 1,
                "stateEdges": 6,
                "warpSets": 1,
                "linkedWarpSets": 1,
                "warpHandoffs": 2,
                "fieldWarpHandoffs": 2,
            },
        )
        row = next(
            node for node in graph["interactionNodes"] if node["type"] == "row"
        )
        self.assertEqual(row["messageName"], "OPEN_DOOR")
        self.assertEqual(
            [handoff["target"] for handoff in graph["warpHandoffs"]],
            ["f011_002", "f024_003"],
        )
        self.assertFalse(graph["warpHandoffs"][1]["targetPresent"])

        dot = field_graph.render_dot(graph, "f011_001")
        self.assertIn("flag 23", dot)
        self.assertIn("row 2", dot)
        self.assertIn("OPEN_DOOR", dot)
        self.assertIn('"f011_001:set:1:warp" -> "f011_002"', dot)


if __name__ == "__main__":
    unittest.main()
