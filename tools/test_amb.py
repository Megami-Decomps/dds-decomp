#!/usr/bin/env python3
"""Regression tests for the DDS AMB source codec."""

from __future__ import annotations

import struct
import sys
import unittest
from pathlib import Path


TOOLS = Path(__file__).resolve().parent
ROOT = TOOLS.parent
if str(TOOLS) not in sys.path:
    sys.path.insert(0, str(TOOLS))

import amb  # noqa: E402


SOURCE = """\
amb 1
header version=3 magic=ATMP areas=@areas count=1
label areas
area name=@area_name sblocks=@sblocks count=1 model=@model position=@area_position
label area_name
string16 "001"
label sblocks
sblock name=@sblock_name node=0 icons=@icons count=1 floor=-2 bounds=null,@bound_max
label sblock_name
string16 "s01"
label icons
icon type=5 position=@icon_position
label icon_position
vec3 1.0 -0.0 3.5
label bound_max
vec3 10.0 20.0 30.0
label model
model geometry=@geometry material=@material
label geometry
model_items count=1
model_item node_id=0 parent=-1 rotation=0,0,0 position=0,0,0,1 scale=1,1,1,0 bounds=null commands=null
label material
model_assets count=0
label opaque
u32 0x12345678
pointer @payload
label payload
bytes deadbeef
label area_position
vec3 -1.0 -2.0 -3.0
label data_end
end_data
"""


class AmbCodecTests(unittest.TestCase):
    def test_semantic_source_round_trips(self) -> None:
        data = amb.encode(amb.parse_source(SOURCE))
        rendered = amb.render_source(data)
        self.assertIn("area name=@area_001_name", rendered)
        self.assertIn("floor=-2 bounds=null,@area_001_s01_bound_max", rendered)
        self.assertIn("model geometry=@area_001_geometry material=@area_001_material", rendered)
        self.assertIn("model_items count=1", rendered)
        self.assertEqual(amb.encode(amb.parse_source(rendered)), data)

    def test_labels_relocate_the_complete_object_graph(self) -> None:
        original = amb.encode(amb.parse_source(SOURCE))
        shifted = amb.encode(amb.parse_source(SOURCE.replace("label areas", "zeros 16\nlabel areas")))
        self.assertEqual(struct.unpack_from("<I", original, 0x18)[0] + 16, struct.unpack_from("<I", shifted, 0x18)[0])
        self.assertEqual(len(original) + 16, len(shifted))
        amb.validate(shifted)

    def test_pointer_relocation_invariant_is_enforced(self) -> None:
        data = bytearray(amb.encode(amb.parse_source(SOURCE)))
        data_end = struct.unpack_from("<I", data, 0x08)[0]
        # Delete the first area-name relocation while retaining its pointer.
        locations = list(amb.reloc.decode(data[data_end:]))
        locations.remove(struct.unpack_from("<I", data, 0x18)[0])
        packed = amb.reloc.encode(locations)
        struct.pack_into("<I", data, 0x10, len(packed))
        data[data_end:] = packed
        with self.assertRaisesRegex(amb.AmbError, "relocation does not agree"):
            amb.validate(bytes(data))

    def test_subblock_model_node_is_validated(self) -> None:
        data = bytearray(amb.encode(amb.parse_source(SOURCE)))
        decoded = amb.decode(data)
        struct.pack_into("<I", data, decoded.sblocks[0][0].offset + 4, 1)
        with self.assertRaisesRegex(amb.AmbError, "references model node 1"):
            amb.validate(bytes(data))

    def test_tracked_corpus_is_canonical(self) -> None:
        for version in ("dds1", "dds2"):
            paths = sorted((ROOT / "src" / version / "data" / "field").glob("*.ambasm"))
            if not paths:
                continue
            for path in paths:
                source = path.read_text(encoding="utf-8")
                self.assertNotRegex(source, r"(?m)^(?:bytes|pointer) ", path.name)
                data = amb.encode(amb.parse_source(source))
                self.assertEqual(amb.render_source(data), source, path.name)


if __name__ == "__main__":
    unittest.main()
