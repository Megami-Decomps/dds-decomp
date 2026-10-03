#!/usr/bin/env python3
"""Regression tests for script-to-encounter graph joins."""

from __future__ import annotations

import sys
import unittest
from dataclasses import replace
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
ROOT = TOOLS.parent
if str(TOOLS) not in sys.path:
    sys.path.insert(0, str(TOOLS))

import battle_tbl  # noqa: E402
import encounter_flow  # noqa: E402
import flw0_profiles  # noqa: E402


class EncounterFlowTests(unittest.TestCase):
    def _fixture(
        self,
    ) -> tuple[battle_tbl.EncountTable, battle_tbl.BattleSymbols]:
        table = battle_tbl.default_encount(battle_tbl.ENCOUNT_PROFILES["dds1"])
        encounters = list(table.encounters)
        encounters[10] = battle_tbl.Encounter(
            voice_group=1,
            start_item=7,
            start_item_count=2,
            unknown_03=3,
            next_encounter=11,
            enemies=(1, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0),
            background_a=22,
            background_b=4,
            flags=0x50D,
            bgm=6,
            event=901,
        )
        encounters[11] = battle_tbl.Encounter(
            enemies=(2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
            event=902,
        )
        table = replace(table, encounters=tuple(encounters))
        symbols = battle_tbl.BattleSymbols(
            "dds1",
            flw0_profiles.IntegerSymbols(()),
            flw0_profiles.IntegerSymbols(((1, "ONE"), (2, "TWO"))),
        )
        return table, symbols

    def test_resolves_request_chain_and_battle_events(self) -> None:
        table, symbols = self._fixture()
        sections = encounter_flow.build_sections(
            [
                {
                    "source": "f001:procedure:2",
                    "requestId": 10,
                    "command": "SUBMIT_EVENT_WITH_SELECTION",
                    "count": 2,
                    "selection": "e700",
                    "sourceReachable": True,
                }
            ],
            table,
            {"e901"},
            symbols,
        )

        self.assertEqual(
            sections["encounterRequestEdges"][0]["target"], "encounter:10"
        )
        self.assertEqual(
            sections["encounterChainEdges"],
            [
                {
                    "source": "encounter:10",
                    "target": "encounter:11",
                    "type": "next-encounter",
                    "sourceReachable": True,
                }
            ],
        )
        self.assertEqual(
            [edge["target"] for edge in sections["encounterEventEdges"]],
            ["e901:procedure:0", "event:e902"],
        )
        self.assertEqual(
            sections["encounterNodes"][0]["enemySlots"][:3],
            [{"id": 1, "name": "ONE"}, None, {"id": 2, "name": "TWO"}],
        )
        self.assertEqual(
            sections["encounterSummary"],
            {
                "encounterRequestEdges": 1,
                "encounterRequestSites": 2,
                "requestedEncounters": 1,
                "encounterNodes": 2,
                "chainOnlyEncounters": 1,
                "reachableEncounters": 2,
                "encounterChainEdges": 1,
                "encounterEventEdges": 2,
                "resolvedEncounterEventEdges": 1,
                "reachableEncounterEventEdges": 2,
            },
        )

    def test_rejects_out_of_range_request_and_chain(self) -> None:
        table, _ = self._fixture()
        with self.assertRaisesRegex(
            encounter_flow.EncounterFlowError, "encounter request 1024"
        ):
            encounter_flow.build_sections(
                [{"source": "f001:procedure:0", "requestId": 1024}], table, set()
            )

        encounters = list(table.encounters)
        encounters[10] = replace(encounters[10], next_encounter=1024)
        with self.assertRaisesRegex(
            encounter_flow.EncounterFlowError, "encounter 10 next target 1024"
        ):
            encounter_flow.build_sections(
                [{"source": "f001:procedure:0", "requestId": 10}],
                replace(table, encounters=tuple(encounters)),
                set(),
            )

    def test_rejects_symbol_profile_mismatch(self) -> None:
        table, symbols = self._fixture()
        with self.assertRaisesRegex(
            encounter_flow.EncounterFlowError,
            "cannot use dds2 battle symbols with dds1 encounters",
        ):
            encounter_flow.build_sections(
                [], table, set(), replace(symbols, profile_name="dds2")
            )

    def test_loads_maintained_battle_sources(self) -> None:
        expected = {"dds1": (1024, 257), "dds2": (1024, 768)}
        for game, (count, populated) in expected.items():
            with self.subTest(game=game):
                table, symbols = encounter_flow.load_sources(
                    ROOT / f"src/{game}/data/battle"
                )
                self.assertEqual(table.profile.name, game)
                self.assertEqual(len(table.encounters), count)
                self.assertNotEqual(
                    table.encounters[populated], battle_tbl.Encounter()
                )
                self.assertEqual(symbols.profile_name, game)


if __name__ == "__main__":
    unittest.main()
