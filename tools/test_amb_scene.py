from __future__ import annotations

import json
import struct
import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

import amb  # noqa: E402
import amb_scene  # noqa: E402
import fld_model  # noqa: E402


SOURCE = """\
amb 1
header version=3 magic=ATMP areas=@areas count=1
label areas
area name=@area_name sblocks=@sblocks count=1 model=@model position=@area_position
label area_name
string16 "001"
label sblocks
sblock name=@sblock_name node=0 icons=@icons count=1 floor=-2 bounds=@bound_min,@bound_max
label sblock_name
string16 "s01"
label icons
icon type=5 position=@icon_position
label icon_position
vec3 10 20 30
label bound_min
vec3 -100 -200 -300
label bound_max
vec3 100 200 300
label model
model geometry=@geometry material=@material
label geometry
model_items count=1
model_item node_id=0 parent=-1 rotation=0,0,0 position=1,2,3,1 scale=1,1,1,0 bounds=@model_bounds commands=@draw_set
label model_bounds
model_bounds minimum=0,0,0 maximum=100,100,0
label draw_set
model_draw_set lists=@draw_list
label draw_list
model_draw_list selector=2 draws=@draw
label draw
model_draw asset=0 qwords=4 packet=@packet
label packet
mesh_header triangles=1 vertices=3 controls=0x1878,0x0360
mesh_triangles
triangle 0,1,2,9
mesh_positions
position 0,0,0 100,0,0 0,100,0
mesh_program address=12
label material
model_assets count=1
model_asset index=0
label area_position
vec3 1 2 3
label data_end
end_data
"""


class AmbSceneTests(unittest.TestCase):
    def setUp(self) -> None:
        self.data = amb.encode(amb.parse_source(SOURCE))

    def test_builds_model_hierarchy_and_automap_metadata(self) -> None:
        document, binary = amb_scene.build_gltf(
            self.data,
            meters_per_unit=0.01,
            icon_marker_size=5.0,
        )
        self.assertEqual(document["asset"]["version"], "2.0")
        self.assertEqual(document["asset"]["extras"]["ddsAreaCount"], 1)
        self.assertEqual(document["nodes"][0]["translation"], [0.01, 0.02, 0.03])
        self.assertEqual(
            document["nodes"][0]["extras"]["ddsAutomapSubBlock"]["name"],
            "s01",
        )
        icon = document["nodes"][1]
        self.assertEqual(icon["translation"], [0.1, 0.2, 0.3])
        self.assertEqual(icon["extras"]["ddsIconType"], 5)
        wrapper = document["nodes"][2]
        for actual, expected in zip(
            wrapper["extras"]["ddsAreaPosition"], [0.01, 0.02, 0.03], strict=True
        ):
            self.assertAlmostEqual(actual, expected)
        self.assertEqual(wrapper["extras"]["ddsSubBlocks"][0]["floor"], -2)
        self.assertEqual(wrapper["extras"]["ddsAreaRootNode"], 0)
        self.assertEqual(
            wrapper["extras"]["ddsAssets"],
            [{"index": 0, "flags": 0}],
        )
        self.assertEqual(document["scenes"][0]["nodes"], [2])
        self.assertEqual(len(document["meshes"]), 2)

        glb = fld_model.encode_glb(document, binary)
        magic, version, size = struct.unpack_from("<4sII", glb)
        self.assertEqual((magic, version, size), (b"glTF", 2, len(glb)))
        json_size, json_kind = struct.unpack_from("<II", glb, 12)
        self.assertEqual(json_kind, fld_model.GLB_JSON_CHUNK)
        decoded = json.loads(glb[20 : 20 + json_size].decode("utf-8"))
        self.assertEqual(decoded["nodes"][2]["name"], "area_001")

    def test_area_selection_and_option_validation(self) -> None:
        document, _ = amb_scene.build_gltf(self.data, areas={"001"})
        self.assertEqual(document["asset"]["extras"]["ddsAreaCount"], 1)
        indexed, _ = amb_scene.build_gltf(self.data, area_indices={0})
        self.assertEqual(indexed["asset"]["extras"]["ddsAreaCount"], 1)
        with self.assertRaisesRegex(amb.AmbError, "not found"):
            amb_scene.build_gltf(self.data, areas={"missing"})
        with self.assertRaisesRegex(amb.AmbError, "name or index"):
            amb_scene.build_gltf(self.data, areas={"001"}, area_indices={0})
        with self.assertRaisesRegex(amb.AmbError, "out of range"):
            amb_scene.build_gltf(self.data, area_indices={1})
        with self.assertRaisesRegex(amb.AmbError, "positive finite"):
            amb_scene.build_gltf(self.data, meters_per_unit=0.0)
        with self.assertRaisesRegex(amb.AmbError, "finite and nonnegative"):
            amb_scene.build_gltf(self.data, icon_marker_size=-1.0)

    def test_appends_to_an_existing_scene_without_reusing_its_material(self) -> None:
        builder = fld_model.GltfBuilder.create()
        existing_mesh = fld_model.add_marker_mesh(builder, "existing", 0, 1.0)
        builder.document["nodes"].append({"name": "existing", "mesh": existing_mesh})
        builder.document["scenes"][0]["nodes"].append(0)
        builder.document["asset"]["extras"] = {
            "ddsMetersPerUnit": 0.01,
            "ddsNativeAxesPreserved": True,
        }
        original_material = dict(builder.document["materials"][0])

        document, binary = amb_scene.append_automap_scene(
            builder.document,
            bytes(builder.binary),
            self.data,
            area_indices={0},
            meters_per_unit=0.01,
            icon_marker_size=0.0,
        )

        self.assertEqual(document["nodes"][0]["name"], "existing")
        self.assertEqual(document["materials"][0], original_material)
        self.assertEqual(document["materials"][1]["name"], "AMB geometry")
        self.assertEqual(document["materials"][2]["name"], "AMB icon")
        automap_primitive = document["meshes"][1]["primitives"][0]
        self.assertEqual(automap_primitive["material"], 1)
        self.assertEqual(document["asset"]["extras"]["ddsAutomapAreaCount"], 1)
        self.assertEqual(document["scenes"][0]["nodes"], [0, 3])
        fld_model.encode_glb(document, binary)

    def test_rejects_invalid_selection_before_mutating_the_scene(self) -> None:
        builder = fld_model.GltfBuilder.create()
        builder.document["asset"]["extras"] = {"ddsMetersPerUnit": 0.01}
        before = json.dumps(builder.document, sort_keys=True)

        for arguments, message in (
            ({"meters_per_unit": 1.0}, "different unit scales"),
            ({"area_indices": {1}, "meters_per_unit": 0.01}, "out of range"),
        ):
            with self.subTest(message=message), self.assertRaisesRegex(
                amb.AmbError, message
            ):
                amb_scene.append_automap_scene(
                    builder.document,
                    bytes(builder.binary),
                    self.data,
                    **arguments,
                )
            self.assertEqual(json.dumps(builder.document, sort_keys=True), before)


if __name__ == "__main__":
    unittest.main()
