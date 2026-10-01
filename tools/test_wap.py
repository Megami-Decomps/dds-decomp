#!/usr/bin/env python3
"""Regression tests for the DDS field actor/warp table codec."""

from __future__ import annotations

import hashlib
import sys
import unittest
from dataclasses import replace
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
ROOT = TOOLS.parent
if str(TOOLS) not in sys.path:
    sys.path.insert(0, str(TOOLS))

import wap  # noqa: E402


class WapCodecTests(unittest.TestCase):
    def test_profiles_and_templates(self) -> None:
        expected = {
            "legacy": 0x64A0,
            "dds1": 0x6CA0,
            "dds2": 0x6D00,
        }
        for name, size in expected.items():
            with self.subTest(profile=name):
                profile = wap.PROFILES[name]
                model = wap.default_file(profile)
                data = wap.encode(model)
                self.assertEqual(len(data), size)
                self.assertEqual(wap.decode(data), model)

    def test_every_typed_field_and_padding_round_trip(self) -> None:
        profile = wap.PROFILES["dds2"]
        model = wap.default_file(profile)
        elevators = list(model.elevators)
        elevators[2] = wap.Elevator(
            42,
            66,
            3,
            (-1, 4, 9, 0, 0, 0),
            (2, 3, 4, 0, 0, 0),
        )
        entries = list(model.entries)
        entries[3] = wap.Entry(
            9,
            2,
            1234,
            7,
            wap.FixedString("interaction"),
            (-1, 2, -3),
            wap.FixedString("scene_a"),
            wap.FixedString("scene_b"),
            3,
            4,
            (1001, 8, 9),
            wap.FixedString("position"),
            2,
            5,
            wap.FixedString("camera"),
            3,
            4,
            2,
            wap.FixedString("procedure"),
            (3, 1, 2, 3, 4, 5, 6, 7),
        )
        entries[4] = replace(
            entries[4],
            after_script=wap.FixedString("", b"residue" + bytes(7)),
        )
        model = wap.WapFile(profile, tuple(elevators), tuple(entries))

        data = wap.encode(model)
        self.assertEqual(wap.decode(data), model)
        references = wap.References(
            frozenset({"interaction"}), frozenset({"procedure"})
        )
        source = wap.render_source(model, references)
        self.assertIn("elevator 2 area=42 sound=66 floor_count=3", source)
        self.assertIn("entry 3 kind=9 flag_mode=2 flag=1234 area=7 name=@interaction", source)
        self.assertIn("warp type=event attributes=4 args=1001,8,9", source)
        self.assertIn("after bgm=3 footstep=4 flag=2 script=@procedure", source)
        self.assertIn("script_padding=7265736964756500000000000000", source)
        self.assertEqual(wap.encode(wap.parse_source(source, references)), data)

    def test_invalid_layout_and_source_are_rejected(self) -> None:
        with self.assertRaisesRegex(wap.WapError, "expected 0x64a0"):
            wap.decode(bytes(17))

        duplicate = """\
wap 1 profile=dds1
entry 0 kind=1
end
entry 0 kind=2
end
"""
        with self.assertRaisesRegex(wap.WapError, "duplicate entry 0"):
            wap.parse_source(duplicate)

        unknown_symbol = """\
wap 1 profile=dds1
entry 0 name=@UNKNOWN
end
"""
        with self.assertRaisesRegex(wap.WapError, "unknown interaction symbol"):
            wap.parse_source(unknown_symbol)

        legacy_tail = """\
wap 1 profile=legacy
entry 0
  tail control=1 args=0,0,0,0,0,0,0
end
"""
        with self.assertRaisesRegex(wap.WapError, "has no DDS tail"):
            wap.parse_source(legacy_tail)

    def test_paired_references(self) -> None:
        references = wap.load_references(
            ROOT / "src/dds1/scripts/field/f004.bfasm",
            ROOT / "src/dds1/data/field/f004.infasm",
        )
        assert references.procedures is not None
        assert references.interactions is not None
        self.assertIn("meri_enc_off", references.procedures)
        self.assertIn("01d_03", references.interactions)

    def test_tracked_corpus_hashes(self) -> None:
        total_files = 0
        for game in ("dds1", "dds2"):
            manifest = ROOT / f"config/{game}/field_wap.sha1"
            expected = {}
            for line in manifest.read_text(encoding="utf-8").splitlines():
                digest, path = line.split(None, 1)
                expected[Path(path).stem] = digest

            source_dir = ROOT / f"src/{game}/data/field"
            sources = sorted(source_dir.glob("*.wapasm"))
            self.assertEqual({path.stem for path in sources}, expected.keys())
            total_files += len(sources)
            for source in sources:
                with self.subTest(game=game, field=source.stem):
                    references = wap.load_references(
                        ROOT / f"src/{game}/scripts/field/{source.stem}.bfasm",
                        source.with_suffix(".infasm"),
                    )
                    source_text = source.read_text(encoding="utf-8")
                    model = wap.parse_source(source_text, references)
                    data = wap.encode(model)
                    self.assertEqual(
                        hashlib.sha1(data).hexdigest(), expected[source.stem]
                    )
                    self.assertEqual(
                        wap.render_source(wap.decode(data), references), source_text
                    )
        self.assertEqual(total_files, 46)


if __name__ == "__main__":
    unittest.main()
