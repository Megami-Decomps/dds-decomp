#!/usr/bin/env python3
"""Regression tests for field random-encounter graph joins."""

from __future__ import annotations

import sys
import unittest
from dataclasses import replace
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
TOOLS = ROOT / "tools"
if str(TOOLS) not in sys.path:
    sys.path.insert(0, str(TOOLS))

import battle_tbl  # noqa: E402
import encounter_flow  # noqa: E402
import fld  # noqa: E402
import random_encounter_flow  # noqa: E402


class RandomEncounterFlowTests(unittest.TestCase):
    def test_resolves_native_route_fallback_order(self) -> None:
        zone = battle_tbl.Zone(routes=(8, 1, 8, 8, 0, 8, 8, 8))
        self.assertEqual(
            random_encounter_flow.resolve_pool(
                zone, 2, (True, True, True), "fixture zone"
            ),
            (1, "ab"),
        )
        self.assertEqual(
            random_encounter_flow.resolve_pool(
                zone, 2, (True, False, False), "fixture zone"
            ),
            (0, "a"),
        )
        self.assertEqual(
            random_encounter_flow.resolve_pool(
                zone, 2, (False, False, False), "fixture zone"
            ),
            (0, "default"),
        )

        invalid = replace(zone, routes=(8, 2, 8, 8, 0, 8, 8, 8))
        with self.assertRaisesRegex(
            random_encounter_flow.RandomEncounterFlowError,
            "route ab selects pool 2",
        ):
            random_encounter_flow.resolve_pool(
                invalid, 2, (True, True, True), "fixture zone"
            )

    def test_links_complete_maintained_field_corpora(self) -> None:
        expected = {
            "dds1": {
                "randomEncounterAreas": 419,
                "defaultZoneEdges": 419,
                "flagZoneEdges": 221,
                "collisionOverrideEdges": 16,
                "collisionOverrideFaces": 40,
                "encounterZoneNodes": 70,
                "conditionRouteEdges": 560,
                "encounterPoolNodes": 280,
                "conditionReachablePools": 76,
                "weightedEncounterEdges": 816,
                "selectableWeightedEncounterEdges": 789,
                "randomEncounterNodes": 398,
                "selectableRandomEncounterNodes": 391,
            },
            "dds2": {
                "randomEncounterAreas": 452,
                "defaultZoneEdges": 452,
                "flagZoneEdges": 235,
                "collisionOverrideEdges": 20,
                "collisionOverrideFaces": 22,
                "encounterZoneNodes": 34,
                "conditionRouteEdges": 272,
                "encounterPoolNodes": 102,
                "conditionReachablePools": 34,
                "weightedEncounterEdges": 561,
                "selectableWeightedEncounterEdges": 537,
                "randomEncounterNodes": 370,
                "selectableRandomEncounterNodes": 369,
            },
        }
        for game, expected_summary in expected.items():
            with self.subTest(game=game):
                root = ROOT / "src" / game
                table, symbols = encounter_flow.load_sources(
                    root / "data" / "battle"
                )
                fields = tuple(
                    (
                        source.stem,
                        fld.encode(
                            fld.parse_source(source.read_text(encoding="utf-8"))
                        ),
                    )
                    for source in sorted((root / "data" / "field").glob("*.fldasm"))
                )
                sections = random_encounter_flow.build_sections(
                    fields, table, symbols
                )
                self.assertEqual(
                    sections["randomEncounterSummary"], expected_summary
                )
                pool_ids = {
                    row["id"] for row in sections["encounterPoolNodes"]
                }
                encounter_ids = {
                    row["id"] for row in sections["encounterNodes"]
                }
                self.assertTrue(
                    all(
                        edge["target"] in pool_ids
                        for edge in sections["encounterZoneRouteEdges"]
                    )
                )
                self.assertTrue(
                    all(
                        edge["target"] in encounter_ids
                        for edge in sections["encounterPoolSlotEdges"]
                    )
                )
                self.assertEqual(
                    sum(
                        edge["selectable"]
                        for edge in sections["encounterPoolSlotEdges"]
                    ),
                    expected_summary["selectableWeightedEncounterEdges"],
                )
                self.assertEqual(
                    sum(
                        node["availableAsRandomEncounter"]
                        for node in sections["encounterNodes"]
                    ),
                    expected_summary["selectableRandomEncounterNodes"],
                )
                self.assertTrue(
                    all(
                        not edge["selectable"]
                        for edge in sections["encounterPoolSlotEdges"]
                        if edge["weight"] == 0
                    )
                )


if __name__ == "__main__":
    unittest.main()
