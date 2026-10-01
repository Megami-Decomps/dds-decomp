#!/usr/bin/env python3
"""Regression tests for the DDS field interaction table codec."""

from __future__ import annotations

import hashlib
import sys
import unittest
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
ROOT = TOOLS.parent
if str(TOOLS) not in sys.path:
    sys.path.insert(0, str(TOOLS))

import inf  # noqa: E402


class InfCodecTests(unittest.TestCase):
    def test_layout_and_canonical_template(self) -> None:
        self.assertEqual(inf.VIEW_OFFSET, 0x02A0)
        self.assertEqual(inf.EX_OFFSET, 0x07A0)
        self.assertEqual(inf.SET_OFFSET, 0x0AC0)
        data = inf.encode(inf.DEFAULT_FILE)
        self.assertEqual(len(data), 0x3B80)
        self.assertEqual(inf.decode(data), inf.DEFAULT_FILE)

    def test_every_typed_field_round_trips(self) -> None:
        packs = list(inf.DEFAULT_FILE.packs)
        hits = list(inf.DEFAULT_PACK.hits)
        hits[3] = inf.Hit(7, "event_hit")
        packs[2] = inf.Pack(-4, tuple(hits))

        views = list(inf.DEFAULT_FILE.views)
        views[3] = inf.View(0, -2, "player_pos", "camera")

        actions = list(inf.DEFAULT_FILE.extra_actions)
        actions[4] = inf.ExtraAction(3, (-1, 2, -3, 4))

        sets = list(inf.DEFAULT_FILE.sets)
        flags = list(inf.DEFAULT_SET.flags)
        flags[2] = inf.FlagSelector(1234, 12, 100)
        messages = list(inf.DEFAULT_SET.messages)
        messages[2] = inf.MessageRow(
            1,
            7,
            (13, 0, 100, 29),
            55,
            56,
            128,
            9,
        )
        sets[5] = inf.InteractionSet(
            inf.Start(0, 2, 9, 3, "02npc_01"),
            tuple(flags),
            tuple(messages),
        )
        model = inf.InfFile(tuple(packs), tuple(views), tuple(actions), tuple(sets))

        data = inf.encode(model)
        self.assertEqual(inf.decode(data), model)

        symbols = (None,) * 7 + ("CHOOSE_PATH",)
        source = inf.render_source(model, symbols)
        self.assertIn('hit 3 area=7 event="event_hit"', source)
        self.assertIn("kind=npc", source)
        self.assertIn("kind=selection message=@CHOOSE_PATH", source)
        self.assertIn("go=row3,0,warp,row19", source)
        rebuilt = inf.parse_source(source, {"CHOOSE_PATH": 7})
        self.assertEqual(inf.encode(rebuilt), data)

    def test_invalid_physical_layout_is_rejected(self) -> None:
        with self.assertRaisesRegex(inf.InfError, "expected 0x3b80"):
            inf.decode(bytes(inf.FILE_SIZE - 1))

        data = bytearray(inf.encode(inf.DEFAULT_FILE))
        data[inf.PACK_OFFSET + 14] = 1
        with self.assertRaisesRegex(inf.InfError, "nonzero string padding"):
            inf.decode(bytes(data))

    def test_invalid_source_is_rejected(self) -> None:
        duplicate = """\
inf 1
view 0 player=1 motion=0 position="p" camera="c"
view 0 player=1 motion=0 position="p" camera="c"
"""
        with self.assertRaisesRegex(inf.InfError, "duplicate view 0"):
            inf.parse_source(duplicate)

        missing_symbol = """\
inf 1
set 1 kind=event area=0 action=1 event_hit=1 event="e"
  row 0 kind=message message=@UNKNOWN go=0,0,0,0 flag_off=0 flag_on=0 view=0 ex=0
end
"""
        with self.assertRaisesRegex(inf.InfError, "unknown message symbol"):
            inf.parse_source(missing_symbol)

    def test_paired_bf_message_symbols(self) -> None:
        by_index, by_name = inf.load_message_symbols(
            ROOT / "src/dds1/scripts/field/f004.bfasm"
        )
        self.assertEqual(by_name["DONT_OPEN"], 142)
        self.assertEqual(by_index[142], "DONT_OPEN")

    def test_tracked_corpus_hashes(self) -> None:
        for game in ("dds1", "dds2"):
            manifest = ROOT / f"config/{game}/field_inf.sha1"
            expected = {}
            for line in manifest.read_text(encoding="utf-8").splitlines():
                digest, path = line.split(None, 1)
                expected[Path(path).stem] = digest

            source_dir = ROOT / f"src/{game}/data/field"
            sources = sorted(source_dir.glob("*.infasm"))
            self.assertEqual({path.stem for path in sources}, expected.keys())
            for source in sources:
                with self.subTest(game=game, field=source.stem):
                    by_index, by_name = inf.load_message_symbols(
                        ROOT / f"src/{game}/scripts/field/{source.stem}.bfasm"
                    )
                    source_text = source.read_text(encoding="utf-8")
                    model = inf.parse_source(source_text, by_name)
                    data = inf.encode(model)
                    self.assertEqual(hashlib.sha1(data).hexdigest(), expected[source.stem])
                    self.assertEqual(
                        inf.render_source(inf.decode(data), by_index),
                        source_text,
                    )


if __name__ == "__main__":
    unittest.main()
