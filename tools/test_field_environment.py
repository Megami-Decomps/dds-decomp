#!/usr/bin/env python3
"""Regression tests for DDS NPL and SKY source codecs."""

from __future__ import annotations

import struct
import sys
import unittest
from pathlib import Path


TOOLS = Path(__file__).resolve().parent
ROOT = TOOLS.parent
if str(TOOLS) not in sys.path:
    sys.path.insert(0, str(TOOLS))

import npl  # noqa: E402
import sky  # noqa: E402


NPL_SOURCE = """\
npl 1 count=64

default primary=#737373 secondary=#333333 texture=15,0 direction=0
palette 7 primary=#102030 secondary=#a0b0c0 texture=2,3 direction=70
"""

SKY_SET = """\
  state type=2 display=-120 fade=1 sway=0
  draw_vector 10,20,30,40
  draw_color 64,80,96
  light 0 color=0.7,0.8,0.9 direction=-0.0,0.25,1.0
  light 1 color=0.1,0.2,0.3 direction=0.4,0.5,0.6
  light 2 color=1.0,0.0,0.5 direction=-1.0,0.0,1.0
  background 0.2,0.3,0.4
  unit_color_a 0.6,0.7,0.8
  unit_direction 0.5,0.25,-0.5
  reserved 000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f202122232425262728292a2b2c2d2e2f
  unit_color_b 0.1,0.15,0.2
end
"""

SKY_SOURCE = "sky 1\n\ndefault\n" + SKY_SET + "\nset 17\n" + SKY_SET.replace(
    "state type=2", "state type=3"
)


class FieldEnvironmentCodecTests(unittest.TestCase):
    def test_npl_sparse_source_round_trips(self) -> None:
        model = npl.parse_source(NPL_SOURCE)
        data = npl.encode(model)
        self.assertEqual(len(data), 64 * npl.ENTRY_SIZE)
        self.assertEqual(data[7 * 8 : 8 * 8], bytes.fromhex("102030a0b0c02346"))
        rendered = npl.render_source(data)
        self.assertIn("palette 7 primary=#102030", rendered)
        self.assertEqual(npl.encode(npl.parse_source(rendered)), data)

    def test_sky_source_preserves_layout_and_float_bits(self) -> None:
        model = sky.parse_source(SKY_SOURCE)
        data = sky.encode(model)
        self.assertEqual(len(data), sky.FILE_SIZE)
        self.assertEqual(struct.unpack_from("<4i", data, 0x1C), (10, 30, 20, 40))
        self.assertEqual(struct.unpack_from("<I", data, 0x44)[0], 0x80000000)
        self.assertEqual(data[0xA4 : 0xD4], bytes(range(0x30)))
        rendered = sky.render_source(data)
        self.assertIn("draw_vector 10,20,30,40", rendered)
        self.assertIn("direction=-0.0,0.25,1.0", rendered)
        self.assertEqual(sky.encode(sky.parse_source(rendered)), data)

    def test_tracked_corpus_is_canonical(self) -> None:
        for version in ("dds1", "dds2"):
            directory = ROOT / "src" / version / "data" / "field"
            for codec, suffix in ((npl, ".nplasm"), (sky, ".skyasm")):
                for path in sorted(directory.glob(f"*{suffix}")):
                    source = path.read_text(encoding="utf-8")
                    data = codec.encode(codec.parse_source(source))
                    self.assertEqual(codec.render_source(data), source, path.name)


if __name__ == "__main__":
    unittest.main()
