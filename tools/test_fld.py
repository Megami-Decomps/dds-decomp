#!/usr/bin/env python3
"""Regression tests for the DDS FLD2 source codec."""

from __future__ import annotations

import hashlib
import json
import struct
import sys
import unittest
from dataclasses import replace
from pathlib import Path


TOOLS = Path(__file__).resolve().parent
ROOT = TOOLS.parent
if str(TOOLS) not in sys.path:
    sys.path.insert(0, str(TOOLS))

import battle_tbl  # noqa: E402
import fld  # noqa: E402
import field_world  # noqa: E402


class FldCodecTests(unittest.TestCase):
    @staticmethod
    def _encounter_field(attributes: int = 0x2000, zone: int = 7) -> bytes:
        source = f"""\
fld2 1
header version=23 magic=FLD2 type_count=1 type_table=@resource_types word_1c=0 word_20=0 word_24=0 word_28=0 word_2c=0 word_30=0 word_34=0 word_38=0 word_3c=0
label resource_types
type id=3 count=1 resources=@resources
label resources
resource serial=0 flags=0 type=3 name=null reserved=0 transform=null area=null link=null sblock=null data=@collision_data
label collision_data
collision vertex_count=4 face_count=1 extra_count=0 vertices=@vertices faces=@faces stop=null reserved=0,0
label vertices
vertex 0 0 0 1
vertex 1 0 0 1
vertex 1 0 1 1
vertex 0 0 1 1
label faces
face attributes={attributes:#010x} move_floor=0 sound=0 stop=0 place=0 automap=0,0 vertices=0,1,2,3 encounter_zone={zone} special=0,0
label data_end
end_data
"""
        return fld.encode(fld.parse_source(source))

    def test_packed_relocations_cover_all_forms_and_runs(self) -> None:
        locations = [
            0x08,
            0x18,
            0x1C,
            0x20,
            0x24,
            0x400,
            0x40000,
            0x40004,
            0x40008,
        ]
        packed = fld._encode_relocations(locations)
        self.assertEqual(fld._decode_relocations(packed), tuple(locations))
        self.assertIn(0x0F, packed)  # a three-word consecutive run

    def test_labels_relocate_when_layout_changes(self) -> None:
        source = """\
fld2 1
header version=23 magic=FLD2 type_count=1 type_table=@resource_types word_1c=0 word_20=0 word_24=0 word_28=0 word_2c=0 word_30=0 word_34=0 word_38=0 word_3c=0
label resource_types
type id=99 count=0 resources=@resources
label resources
u32 0x12345678
pointer @resources
label data_end
end_data
"""
        original = fld.encode(fld.parse_source(source))
        shifted = fld.encode(
            fld.parse_source(source.replace("label resource_types", "zeros 4\nlabel resource_types"))
        )

        self.assertEqual(struct.unpack_from("<I", original, 0x18)[0], 0x40)
        self.assertEqual(struct.unpack_from("<I", shifted, 0x18)[0], 0x44)
        self.assertEqual(struct.unpack_from("<I", original, 0x48)[0], 0x4C)
        self.assertEqual(struct.unpack_from("<I", shifted, 0x4C)[0], 0x50)
        self.assertEqual(struct.unpack_from("<I", original, 0x50)[0], 0x4C)
        self.assertEqual(struct.unpack_from("<I", shifted, 0x54)[0], 0x50)

    def test_encounter_zone_has_semantic_source_and_tag_invariant(self) -> None:
        data = self._encounter_field()
        self.assertEqual(fld.encounter_zone_overrides(data), (7,))
        self.assertIn("encounter_zone=7", fld.render_source(data))
        with self.assertRaisesRegex(fld.FldError, "without attribute"):
            self._encounter_field(attributes=0)

    def test_field_world_links_default_and_override_zones(self) -> None:
        profile = battle_tbl.ENCOUNT_PROFILES["dds1"]
        table = battle_tbl.default_encount(profile)
        entries = list(table.default_maps[0].entries)
        entries[45] = battle_tbl.Selector(value=3, flag_a=-1, flag_b=-1)
        maps = list(table.default_maps)
        maps[0] = battle_tbl.SelectorMap(22, tuple(entries))
        zones = list(table.zones)
        zones[3] = replace(zones[3], bgm=5)
        zones[7] = replace(zones[7], bgm=5)
        table = replace(table, default_maps=tuple(maps), zones=tuple(zones))

        summary = field_world.validate_encounter_links(
            (("f022_045", self._encounter_field()),), table
        )
        self.assertEqual(
            summary,
            field_world.EncounterLinkSummary(1, 1, 1, 1, 2),
        )

    def test_invalid_event_placement_is_rejected(self) -> None:
        source = """\
fld2 1
header version=23 magic=FLD2 type_count=1 type_table=@resource_types word_1c=0 word_20=0 word_24=0 word_28=0 word_2c=0 word_30=0 word_34=0 word_38=0 word_3c=0
label resource_types
type id=10 count=1 resources=@resources
label resources
resource serial=0 flags=0 type=10 name=@name reserved=0 transform=null area=null link=null sblock=null data=@put
label name
string16 point
label put
placement kind=1 event=3 visible=0 payload=@payload
label payload
u32 0 0 0 0
label data_end
end_data
"""
        with self.assertRaisesRegex(fld.FldError, "references event 3"):
            fld.encode(fld.parse_source(source))

    def test_motion_keys_must_increase(self) -> None:
        source = ROOT / "src/dds1/data/field/f011_001.fldasm"
        data = bytearray(fld.encode(fld.parse_source(source.read_text(encoding="utf-8"))))
        words, data_end, _ = fld._read_header(data)
        resources = fld._read_resources(data, fld._read_types(data, words, data_end))
        motion = next(resource for resource in resources if resource.type_id == 9)
        track = fld._read_motion_tracks(data, motion.data, data_end, "test motion")[0]
        first_key = struct.unpack_from("<I", data, track.keys)[0]
        struct.pack_into("<I", data, track.keys + 4, first_key)
        with self.assertRaisesRegex(fld.FldError, "not strictly increasing"):
            fld.validate(bytes(data))

    def test_single_key_motion_is_valid(self) -> None:
        source = """\
fld2 1
header version=23 magic=FLD2 type_count=1 type_table=@resource_types word_1c=0 word_20=0 word_24=0 word_28=0 word_2c=0 word_30=0 word_34=0 word_38=0 word_3c=0
label resource_types
type id=9 count=1 resources=@resources
label resources
resource serial=0 flags=0 type=9 name=null reserved=0 transform=null area=null link=null sblock=null data=@motion_data
label motion_data
motion tracks=scalar:@curve
label curve
motion_curve count=1 values=@values keys=@frames word_0c=1
label values
scalar 2.5
label frames
keys 120
label data_end
end_data
"""
        data = fld.encode(fld.parse_source(source))
        fld.validate(data)
        rendered = fld.render_source(data)
        self.assertEqual(fld.encode(fld.parse_source(rendered)), data)

    def test_special_point_round_trip(self) -> None:
        source = """\
fld2 1
header version=23 magic=FLD2 type_count=1 type_table=@resource_types word_1c=0 word_20=0 word_24=0 word_28=0 word_2c=0 word_30=0 word_34=0 word_38=0 word_3c=0
label resource_types
type id=10 count=1 resources=@resources
label resources
resource serial=0 flags=0 type=10 name=null reserved=0 transform=null area=null link=null sblock=null data=@placement_data
label placement_data
placement kind=8 event=-1 visible=0 payload=@special
label special
special_point kind=heal id=3
label data_end
end_data
"""
        data = fld.encode(fld.parse_source(source))
        fld.validate(data)
        rendered = fld.render_source(data)
        self.assertIn("special_point kind=heal id=3", rendered)
        self.assertEqual(fld.encode(fld.parse_source(rendered)), data)

    def test_empty_resource_type_round_trip(self) -> None:
        source = """\
fld2 1
header version=23 magic=FLD2 type_count=1 type_table=@resource_types word_1c=0 word_20=0 word_24=0 word_28=0 word_2c=0 word_30=0 word_34=0 word_38=0 word_3c=0
label resource_types
type id=99 count=0 resources=@data_end
label data_end
end_data
"""
        data = fld.encode(fld.parse_source(source))
        rendered = fld.render_source(data)
        self.assertEqual(fld.encode(fld.parse_source(rendered)), data)

    def test_compact_camera_round_trip(self) -> None:
        source = """\
fld2 1
header version=23 magic=FLD2 type_count=1 type_table=@resource_types word_1c=0 word_20=0 word_24=0 word_28=0 word_2c=0 word_30=0 word_34=0 word_38=0 word_3c=0
label resource_types
type id=4 count=1 resources=@resources
label resources
resource serial=0 flags=0 type=4 name=null reserved=0 transform=null area=null link=null sblock=null data=@camera_data
label camera_data
camera fovy=0.6024157404899597
label data_end
end_data
"""
        data = fld.encode(fld.parse_source(source))
        rendered = fld.render_source(data)
        self.assertEqual(fld.encode(fld.parse_source(rendered)), data)

    def test_fld1_texture_effect_and_light_round_trip(self) -> None:
        source = """\
fld1 1
header version=23 magic=FLD1 type_count=3 type_table=@resource_types word_1c=0 word_20=0 word_24=0 word_28=0 word_2c=0 word_30=0 word_34=0 word_38=0 word_3c=0
label resource_types
type id=5 count=1 resources=@texture_resources
type id=11 count=1 resources=@effect_resources
type id=12 count=1 resources=@light_resources
label texture_resources
resource serial=0 flags=0 type=5 name=null reserved=0 transform=null area=null link=null sblock=null data=@texture_data
label effect_resources
resource serial=1 flags=0 type=11 name=null reserved=0 transform=null area=null link=null sblock=null data=@effect_data
label light_resources
resource serial=2 flags=0 type=12 name=null reserved=0 transform=null area=null link=null sblock=null data=@light_data
label texture_data
texture_list "f011_001.TB"
label effect_data
effect flags=0 type=2 selector=1 size=400,400,1 parameters=0,1,0,0,0,0
label light_data
light reserved=0 flags=1 animation=2 inner_radius=100 outer_radius=200 softness=3 bias=0.25 diffuse=1,0.5,0.25 ambient=0.1,0.2,0.3
label data_end
end_data
"""
        data = fld.encode(fld.parse_source(source))
        rendered = fld.render_source(data)
        self.assertIn('texture_list "f011_001.TB"', rendered)
        self.assertIn("effect flags=0 type=2 selector=1", rendered)
        self.assertIn("light reserved=0 flags=1 animation=2", rendered)
        self.assertEqual(fld.encode(fld.parse_source(rendered)), data)

        with self.assertRaisesRegex(fld.FldError, "does not match fld1 preamble"):
            fld.parse_source(source.replace("magic=FLD1", "magic=FLD2", 1))

    def test_fld1_model_graph_round_trip(self) -> None:
        source = """\
fld1 1
header version=23 magic=FLD1 type_count=1 type_table=@resource_types word_1c=0 word_20=0 word_24=0 word_28=0 word_2c=0 word_30=0 word_34=0 word_38=0 word_3c=0
label resource_types
type id=2 count=1 resources=@resources
label resources
resource serial=0 flags=0 type=2 name=null reserved=0 transform=null area=null link=null sblock=null data=@model_resource
label model_resource
model_resource items=@items assets=@assets motion=@motion
label items
model_items count=2
model_item node_id=0 parent=-1 rotation=0,0,0 position=0,0,0,1 scale=1,1,1,0 bounds=null commands=null
model_item node_id=1 parent=0 rotation=0,1.5,0 position=10,20,30,1 scale=1,2,1,0 bounds=@bounds commands=@commands
label bounds
model_bounds minimum=-1,-2,-3 maximum=1,2,3
label commands
model_draw_set lists=@draw_list
label draw_list
model_draw_list selector=2 draws=@draw
label draw
model_draw asset=0 qwords=2 packet=@packet
label packet
packet_data 000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f
label assets
model_assets count=1
model_asset index=0 word_01=0x80969696 resource_04=3 values_08=0,0,1,0.5,0 scalar_200=0.9
label motion
u32 0
label data_end
end_data
"""
        data = fld.encode(fld.parse_source(source))
        rendered = fld.render_source(data)
        self.assertIn("model_items count=2", rendered)
        self.assertIn("node_id=1 parent=0", rendered)
        self.assertIn("model_bounds minimum=-1.0,-2.0,-3.0", rendered)
        self.assertIn("model_asset index=0 word_01=0x80969696", rendered)
        self.assertIn("model_draw_list selector=2", rendered)
        self.assertIn("model_draw asset=0 qwords=2", rendered)
        self.assertEqual(fld.encode(fld.parse_source(rendered)), data)

        with self.assertRaisesRegex(fld.FldError, "has parent 2"):
            fld.encode(fld.parse_source(source.replace("node_id=1 parent=0", "node_id=1 parent=2")))
        with self.assertRaisesRegex(fld.FldError, "followed by 2 model_item"):
            fld.encode(fld.parse_source(source.replace("model_items count=2", "model_items count=1")))
        with self.assertRaisesRegex(fld.FldError, "contains 32"):
            fld.encode(fld.parse_source(source.replace("qwords=2", "qwords=1")))
        with self.assertRaisesRegex(fld.FldError, "only 1 assets exist"):
            fld.encode(fld.parse_source(source.replace("model_draw asset=0", "model_draw asset=1")))

    def test_tracked_sources_are_canonical_and_exact(self) -> None:
        expected_links = {
            ("dds1", "f011_001"): fld.LinkSummary(2, 3, 1),
            ("dds2", "f011_001"): fld.LinkSummary(1, 2, 1),
        }
        versions = json.loads((ROOT / "config/versions.json").read_text(encoding="utf-8"))
        for game in ("dds1", "dds2"):
            manifest = ROOT / "config" / game / "field_fld2.sha1"
            linked = set(versions[game].get("field_fld2_links", ()))
            self.assertEqual(
                linked,
                {stem for (version, stem) in expected_links if version == game},
            )
            for line in manifest.read_text(encoding="utf-8").splitlines():
                expected, output = line.split()
                source = ROOT / "src" / game / "data" / "field" / (Path(output).stem + ".fldasm")
                with self.subTest(game=game, source=source.name):
                    text = source.read_text(encoding="utf-8")
                    data = fld.encode(fld.parse_source(text))
                    self.assertEqual(hashlib.sha1(data).hexdigest(), expected)
                    self.assertEqual(fld.render_source(data), text)
                    key = game, source.stem
                    if key not in expected_links:
                        continue
                    field_text, area_text = source.stem.split("_")
                    field, area = int(field_text[1:]), int(area_text)
                    scripts = ROOT / "src" / game / "scripts" / "field" / f"f{field:03}.bfasm"
                    warps = source.with_name(f"f{field:03}.wapasm")
                    links = fld.validate_links(
                        data,
                        scripts.read_text(encoding="utf-8"),
                        warps.read_text(encoding="utf-8"),
                        field,
                        area,
                    )
                    self.assertEqual(links, expected_links[key])

        for game in ("dds1", "dds2"):
            manifest = ROOT / "config" / game / "field_fld1.sha1"
            for line in manifest.read_text(encoding="utf-8").splitlines():
                expected, output = line.split()
                source = ROOT / "src" / game / "data" / "field" / (Path(output).stem + ".f1asm")
                with self.subTest(game=game, source=source.name):
                    text = source.read_text(encoding="utf-8")
                    data = fld.encode(fld.parse_source(text))
                    self.assertEqual(hashlib.sha1(data).hexdigest(), expected)
                    self.assertEqual(fld.render_source(data), text)

    def test_missing_script_event_is_rejected(self) -> None:
        source = ROOT / "src/dds1/data/field/f011_001.fldasm"
        data = fld.encode(fld.parse_source(source.read_text(encoding="utf-8")))
        scripts = ROOT / "src/dds1/scripts/field/f011.bfasm"
        warps = source.with_name("f011.wapasm")
        script_text = scripts.read_text(encoding="utf-8").replace('name="001_01eve_01"', 'name="missing"')
        with self.assertRaisesRegex(fld.FldError, "001_01eve_01"):
            fld.validate_links(
                data,
                script_text,
                warps.read_text(encoding="utf-8"),
                11,
                1,
            )


if __name__ == "__main__":
    unittest.main()
