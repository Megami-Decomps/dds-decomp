from __future__ import annotations

import struct
import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

import fld  # noqa: E402
import fld_model  # noqa: E402
import fld_scene  # noqa: E402


SOURCE = """\
fld2 1
header version=23 magic=FLD2 type_count=3 type_table=@types word_1c=0 word_20=0 word_24=0 word_28=0 word_2c=0 word_30=0 word_34=0 word_38=0 word_3c=0
label types
type id=3 count=1 resources=@collision_resources
type id=4 count=1 resources=@camera_resources
type id=10 count=1 resources=@placement_resources
label collision_resources
resource serial=2 flags=0 type=3 name=@collision_name reserved=0 transform=@collision_transform area=null link=null sblock=null data=@collision
label camera_resources
resource serial=3 flags=1 type=4 name=@camera_name reserved=0 transform=@camera_transform area=null link=null sblock=null data=@camera
label placement_resources
resource serial=4 flags=1 type=10 name=@placement_name reserved=0 transform=@placement_transform area=null link=null sblock=null data=@placement
label collision_name
string16 01all
label camera_name
string16 01cam_01
label placement_name
string16 01heal_01
label collision_transform
transform position=0,0,0,1 rotation=0,0,0,1 scale=1,1,1,1
label camera_transform
transform position=400,500,600,1 rotation=0,0,0,1 scale=1,1,1,1
label placement_transform
transform position=100,200,300,1 rotation=0,0,0,1 scale=1,1,1,1
label collision
collision vertex_count=4 face_count=1 extra_count=0 vertices=@vertices faces=@faces stop=null reserved=0,0
label vertices
vertex 0 0 0 1
vertex 100 0 0 1
vertex 100 0 100 1
vertex 0 0 100 1
label faces
face attributes=0x00002000 move_floor=0 sound=0 stop=0 place=0 automap=1,1 vertices=0,1,2,3 encounter_zone=7 special=0,0
label camera
camera fovy=0.7853981852531433
label placement
placement kind=8 event=-1 visible=1 payload=@special
label special
special_point kind=heal id=3
label data_end
end_data
"""


class FldSceneTests(unittest.TestCase):
    def test_appends_collision_camera_and_placement(self) -> None:
        builder = fld_model.GltfBuilder.create()
        document, binary = fld_scene.append_field_scene(
            builder.document,
            bytes(builder.binary),
            fld.encode(fld.parse_source(SOURCE)),
            meters_per_unit=0.01,
            placement_marker_size=50.0,
        )
        wrapper = document["nodes"][-1]
        self.assertEqual(wrapper["name"], "FLD2 field data")
        self.assertEqual(
            wrapper["extras"],
            {
                "ddsCollisionResources": 1,
                "ddsCameraResources": 1,
                "ddsPlacementResources": 1,
            },
        )
        children = [document["nodes"][index] for index in wrapper["children"]]
        self.assertEqual([node["name"] for node in children], ["01all", "01cam_01", "01heal_01"])
        self.assertEqual(children[1]["extras"]["ddsCameraYFov"], 0.7853981852531433)
        self.assertEqual(children[2]["translation"], [1.0, 2.0, 3.0])
        self.assertEqual(
            children[2]["extras"]["ddsSpecialPoint"], {"kind": "heal", "id": 3}
        )

        collision = document["meshes"][children[0]["mesh"]]
        primitive = collision["primitives"][0]
        accessor = document["accessors"][primitive["indices"]]
        view = document["bufferViews"][accessor["bufferView"]]
        indices = struct.unpack_from("<6H", binary, view["byteOffset"])
        self.assertEqual(indices, (0, 1, 2, 0, 2, 3))
        self.assertEqual(collision["extras"]["ddsTriangleCount"], 2)
        self.assertIn("KHR_materials_unlit", document["extensionsUsed"])
        fld_model.encode_glb(document, binary)

    def test_rejects_negative_marker_size(self) -> None:
        builder = fld_model.GltfBuilder.create()
        with self.assertRaisesRegex(fld.FldError, "marker size"):
            fld_scene.append_field_scene(
                builder.document,
                bytes(builder.binary),
                fld.encode(fld.parse_source(SOURCE)),
                meters_per_unit=0.01,
                placement_marker_size=-1.0,
            )

    def test_empty_field_omits_empty_children_array(self) -> None:
        source = """\
fld2 1
header version=23 magic=FLD2 type_count=1 type_table=@types word_1c=0 word_20=0 word_24=0 word_28=0 word_2c=0 word_30=0 word_34=0 word_38=0 word_3c=0
label types
type id=99 count=0 resources=@data_end
label data_end
end_data
"""
        builder = fld_model.GltfBuilder.create()
        document, binary = fld_scene.append_field_scene(
            builder.document,
            bytes(builder.binary),
            fld.encode(fld.parse_source(source)),
            meters_per_unit=1.0,
        )
        self.assertNotIn("children", document["nodes"][-1])
        fld_model.encode_glb(document, binary)


if __name__ == "__main__":
    unittest.main()
