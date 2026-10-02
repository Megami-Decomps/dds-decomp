#!/usr/bin/env python3
"""Regression tests for the DDS FLD2 source codec."""

from __future__ import annotations

import hashlib
import json
import struct
import sys
import unittest
from pathlib import Path


TOOLS = Path(__file__).resolve().parent
ROOT = TOOLS.parent
if str(TOOLS) not in sys.path:
    sys.path.insert(0, str(TOOLS))

import fld  # noqa: E402


class FldCodecTests(unittest.TestCase):
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
