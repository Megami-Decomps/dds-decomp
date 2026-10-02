from __future__ import annotations

import json
import math
import struct
import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

import fld  # noqa: E402
import fld_model  # noqa: E402


SOURCE = """\
fld1 1
header version=23 magic=FLD1 type_count=1 type_table=@types word_1c=0 word_20=0 word_24=0 word_28=0 word_2c=0 word_30=0 word_34=0 word_38=0 word_3c=0
label types
type id=2 count=1 resources=@resources
label resources
resource serial=7 flags=0 type=2 name=@name reserved=0 transform=@transform area=null link=null sblock=null data=@model
label name
string16 "test_model"
label transform
transform position=100,200,300,1 rotation=0,0,0,1 scale=1,1,1,1
label model
model_resource items=@items assets=@assets motion=@motion
label items
model_items count=1
model_item node_id=0 parent=-1 rotation=0,0,0 position=1,2,3,1 scale=1,1,1,0 bounds=@bounds commands=@draw_set
label bounds
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
label assets
model_assets count=1
model_asset index=0
label motion
model_motion_playbook clip_count=1 bindings=1 clips=@clips
model_motion_binding family=node selector=translation target=0
label clips
model_motion_clip_table clips=@clip
label clip
model_motion_clip duration=30 reserved=0
model_motion_track format=vector3 frames=0,30
motion_vector3 0,0,0 100,0,0
label data_end
end_data
"""


class FldModelTests(unittest.TestCase):
    def setUp(self) -> None:
        self.data = fld.encode(fld.parse_source(SOURCE))

    def test_builds_geometry_hierarchy_and_animation(self) -> None:
        document, binary = fld_model.build_gltf(
            self.data,
            meters_per_unit=0.01,
            frames_per_second=30.0,
        )
        self.assertEqual(document["asset"]["version"], "2.0")
        self.assertEqual(document["asset"]["extras"]["ddsMetersPerUnit"], 0.01)
        self.assertEqual(len(document["meshes"]), 1)
        primitive = document["meshes"][0]["primitives"][0]
        self.assertEqual(set(primitive["attributes"]), {"POSITION"})
        self.assertEqual(primitive["extras"]["ddsTriangleControlAccessor"], 2)
        self.assertEqual(primitive["extras"]["ddsMeshControls"], [0x1878, 0x0360])
        self.assertEqual(document["nodes"][0]["translation"], [0.01, 0.02, 0.03])
        self.assertEqual(document["nodes"][1]["translation"], [1.0, 2.0, 3.0])
        self.assertEqual(
            document["nodes"][1]["extras"]["ddsMotionBindings"][0],
            {
                "family": "node",
                "selector": 0,
                "target": 0,
                "format": "vector3",
            },
        )
        self.assertEqual(document["scenes"][0]["nodes"], [1])

        animation = document["animations"][0]
        self.assertEqual(
            animation["channels"][0]["target"],
            {"node": 0, "path": "translation"},
        )
        time_accessor = document["accessors"][animation["samplers"][0]["input"]]
        time_view = document["bufferViews"][time_accessor["bufferView"]]
        times = struct.unpack_from("<2f", binary, time_view["byteOffset"])
        self.assertEqual(times, (0.0, 1.0))

        glb = fld_model.encode_glb(document, binary)
        magic, version, size = struct.unpack_from("<4sII", glb)
        self.assertEqual((magic, version, size), (b"glTF", 2, len(glb)))
        json_size, json_kind = struct.unpack_from("<II", glb, 12)
        self.assertEqual(json_kind, fld_model.GLB_JSON_CHUNK)
        decoded = json.loads(glb[20 : 20 + json_size].decode("utf-8"))
        self.assertEqual(decoded["meshes"][0]["name"], "test_model/node_0")

    def test_resource_selection_and_scale_validation(self) -> None:
        document, _ = fld_model.build_gltf(self.data, resources={"test_model"})
        self.assertEqual(document["nodes"][-1]["name"], "test_model")
        with self.assertRaisesRegex(fld.FldError, "not found"):
            fld_model.build_gltf(self.data, resources={"missing"})
        with self.assertRaisesRegex(fld.FldError, "positive finite"):
            fld_model.build_gltf(self.data, meters_per_unit=0.0)

    def test_euler_conversion_matches_engine_convention(self) -> None:
        self.assertEqual(fld_model._euler_quaternion(0.0, 0.0, 0.0), [0.0, 0.0, 0.0, 1.0])
        quaternion = fld_model._euler_quaternion(0.0, math.pi, 0.0)
        self.assertAlmostEqual(quaternion[0], 0.0)
        self.assertAlmostEqual(quaternion[1], -1.0)
        self.assertAlmostEqual(quaternion[2], 0.0)
        self.assertAlmostEqual(quaternion[3], 0.0)

    def test_non_finite_static_transform_is_preserved_as_metadata(self) -> None:
        source = SOURCE.replace("position=1,2,3,1", "position=nan,nan,nan,1")
        document, binary = fld_model.build_gltf(
            fld.encode(fld.parse_source(source))
        )
        node = document["nodes"][0]
        self.assertNotIn("translation", node)
        self.assertEqual(
            node["extras"]["ddsOmittedTranslationBits"],
            ["0x7fc00000", "0x7fc00000", "0x7fc00000"],
        )
        fld_model.encode_glb(document, binary)

    def test_tracked_fld1_sources_export_as_complete_glbs(self) -> None:
        expected = {
            "dds1": (5, 189, 175, 0),
            "dds2": (4, 195, 184, 2),
        }
        for version, counts in expected.items():
            path = ROOT / "src" / version / "data" / "field" / "f011_001.f1asm"
            data = fld.encode(fld.parse_source(path.read_text(encoding="utf-8")))
            document, binary = fld_model.build_gltf(
                data,
                meters_per_unit=0.01,
                frames_per_second=30.0,
            )
            wrappers = sum(
                isinstance(node, dict)
                and isinstance(node.get("extras"), dict)
                and "ddsMotionBindings" in node["extras"]
                for node in document["nodes"]
            )
            actual = (
                wrappers,
                len(document["nodes"]),
                len(document["meshes"]),
                len(document.get("animations", [])),
            )
            self.assertEqual(actual, counts, version)
            glb = fld_model.encode_glb(document, binary)
            self.assertEqual(struct.unpack_from("<4sI", glb), (b"glTF", 2))


if __name__ == "__main__":
    unittest.main()
