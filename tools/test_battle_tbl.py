#!/usr/bin/env python3
"""Regression tests for the DDS battle table codec."""

from __future__ import annotations

import hashlib
import sys
import unittest
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
ROOT = TOOLS.parent
if str(TOOLS) not in sys.path:
    sys.path.insert(0, str(TOOLS))

import battle_tbl  # noqa: E402


class BattleTableTests(unittest.TestCase):
    def test_encount_profiles_and_templates(self) -> None:
        expected_sizes = {"dds1": 0x20FE0, "dds2": 0x1B760}
        for name, size in expected_sizes.items():
            with self.subTest(profile=name):
                profile = battle_tbl.ENCOUNT_PROFILES[name]
                model = battle_tbl.default_encount(profile)
                data = battle_tbl.encode_encount(model)
                self.assertEqual(len(data), size)
                self.assertEqual(battle_tbl.decode_encount(data), model)

    def test_every_encount_record_family_round_trips(self) -> None:
        profile = battle_tbl.ENCOUNT_PROFILES["dds2"]
        model = battle_tbl.default_encount(profile)

        encounters = list(model.encounters)
        encounters[3] = battle_tbl.Encounter(
            -2,
            8,
            2,
            9,
            44,
            (1, 0, 3, 0, 0, 0, 0, 0, 0, 0, 5),
            201,
            4,
            0x10204,
            17,
            606,
        )

        default_maps = list(model.default_maps)
        default_entries = list(default_maps[2].entries)
        default_entries[5] = battle_tbl.Selector(7, 101, 8, 102, 9)
        default_maps[2] = battle_tbl.SelectorMap(24, tuple(default_entries))

        zones = list(model.zones)
        pools = list(zones[4].pools)
        slots = list(pools[1].slots)
        slots[2] = battle_tbl.PoolSlot(33, 60, -3, 70)
        pools[1] = battle_tbl.EncounterPool(30, -4, tuple(slots))
        zones[4] = battle_tbl.Zone(
            203,
            2,
            5,
            6,
            ((1, 0x20), (2, 0x30), (3, 0x40)),
            (1, 2, 3, 4, 5, 6, 7, 8),
            tuple(pools),
        )

        overrides = list(model.overrides)
        overrides[1] = battle_tbl.OverrideRule(-1, 88, 25, 9)

        background_maps = list(model.background_maps)
        background_entries = list(background_maps[1].entries)
        background_entries[6] = battle_tbl.Selector(0x20001, 110, 3, 111, 4)
        background_maps[1] = battle_tbl.SelectorMap(23, tuple(background_entries))

        visuals = list(model.visuals)
        groups = list(visuals[0].groups)
        groups[2] = battle_tbl.VisualGroup(3, (1, 2, 3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF), 4)
        visuals[0] = battle_tbl.VisualRow(tuple(range(16)), tuple(groups))

        model = battle_tbl.EncountTable(
            profile,
            tuple(encounters),
            tuple(default_maps),
            tuple(zones),
            tuple(overrides),
            tuple(background_maps),
            tuple(visuals),
        )
        data = battle_tbl.encode_encount(model)
        self.assertEqual(battle_tbl.decode_encount(data), model)
        source = battle_tbl.render_encount_source(model)
        self.assertIn("encounter 3 voice=-2 item=8 item_count=2", source)
        self.assertIn("flags=0x10204", source)
        self.assertIn("routes abc=1 ab=2 ac=3 bc=4 a=5 b=6 c=7 none=8", source)
        self.assertIn("entry 5 zone=7 flag_a=101 zone_a=8 flag_b=102 zone_b=9", source)
        self.assertEqual(battle_tbl.encode_encount(battle_tbl.parse_encount_source(source)), data)

    def test_invalid_layout_and_source_are_rejected(self) -> None:
        with self.assertRaisesRegex(battle_tbl.BattleTableError, "ENCOUNT needs six segments"):
            battle_tbl.decode_encount(bytes(16))

        mixed_routes = """\
battle-table 1 kind=encounter profile=dds1
zone 0
  routes values=0,0,0,0,0,0,0,0 abc=1
end
"""
        with self.assertRaisesRegex(battle_tbl.BattleTableError, "cannot be combined"):
            battle_tbl.parse_encount_source(mixed_routes)

        bad_padding = bytearray(
            battle_tbl.encode_encount(
                battle_tbl.default_encount(battle_tbl.ENCOUNT_PROFILES["dds1"])
            )
        )
        bad_padding[4 + 0xA000] = 1
        with self.assertRaisesRegex(battle_tbl.BattleTableError, "nonzero alignment"):
            battle_tbl.decode_encount(bytes(bad_padding))

    def test_tracked_encount_corpus_hashes(self) -> None:
        for game in ("dds1", "dds2"):
            manifest = ROOT / f"config/{game}/battle_tables.sha1"
            digest, output = manifest.read_text(encoding="utf-8").split()
            self.assertEqual(Path(output).name, "ENCOUNT.TBL")
            source = ROOT / f"src/{game}/data/battle/encount.tblasm"
            source_text = source.read_text(encoding="utf-8")
            model = battle_tbl.parse_encount_source(source_text)
            data = battle_tbl.encode_encount(model)
            self.assertEqual(hashlib.sha1(data).hexdigest(), digest)
            self.assertEqual(
                battle_tbl.render_encount_source(battle_tbl.decode_encount(data)),
                source_text,
            )


if __name__ == "__main__":
    unittest.main()
