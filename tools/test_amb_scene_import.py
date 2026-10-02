from __future__ import annotations

import struct
import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

import amb  # noqa: E402
import amb_scene  # noqa: E402
import amb_scene_import  # noqa: E402
import fld  # noqa: E402
import fld_model  # noqa: E402
import sdf_model_import  # noqa: E402
from test_amb_scene import SOURCE  # noqa: E402


def accessor_offset(document: dict, accessor_index: int) -> int:
    accessor = document["accessors"][accessor_index]
    view = document["bufferViews"][accessor["bufferView"]]
    return view.get("byteOffset", 0) + accessor.get("byteOffset", 0)


def first_mesh(data: bytes) -> fld.ModelMesh:
    model = amb.decode(data)
    graph = model.models[0]
    item = next(item for item in graph.items if item.commands)
    draw_list = graph.draw_lists[graph.draw_roots[item.commands][0]]
    draw = graph.draws[draw_list.draws[0]]
    meshes, _ = fld._read_model_mesh_packet(
        data, draw.packet, draw.quadwords * 0x10, "test packet"
    )
    return meshes[0]


class AmbSceneImportTests(unittest.TestCase):
    def setUp(self) -> None:
        self.data = amb.encode(amb.parse_source(SOURCE))
        self.document, binary = amb_scene.build_gltf(
            self.data, meters_per_unit=0.01
        )
        self.binary = bytearray(binary)

    def test_unchanged_glb_preserves_every_amb_byte(self) -> None:
        glb = fld_model.encode_glb(self.document, bytes(self.binary))
        document, binary = sdf_model_import.decode_glb(glb)
        rebuilt, summary = amb_scene_import.import_geometry(
            self.data, document, binary
        )
        self.assertEqual(rebuilt, self.data)
        self.assertEqual(
            summary,
            sdf_model_import.ImportSummary(1, 1, 0, 0, 0, 0, 0, 0),
        )

    def test_imports_position_without_changing_packet_shape(self) -> None:
        primitive = self.document["meshes"][0]["primitives"][0]
        position = primitive["attributes"]["POSITION"]
        struct.pack_into(
            "<f",
            self.binary,
            accessor_offset(self.document, position),
            1.25,
        )
        rebuilt, summary = amb_scene_import.import_geometry(
            self.data, self.document, bytes(self.binary)
        )
        self.assertEqual(
            summary,
            sdf_model_import.ImportSummary(1, 1, 1, 1, 0, 0, 0, 0),
        )
        self.assertEqual(first_mesh(rebuilt).positions[0][0], 125.0)
        self.assertEqual(first_mesh(rebuilt).triangles, first_mesh(self.data).triangles)
        self.assertEqual(len(rebuilt), len(self.data))
        self.assertEqual(
            amb.encode(amb.parse_source(amb.render_source(rebuilt))), rebuilt
        )

    def test_rejects_an_area_identity_mismatch(self) -> None:
        wrapper = next(
            node
            for node in self.document["nodes"]
            if "ddsAreaIndex" in node.get("extras", {})
        )
        wrapper["name"] = "area_002"
        with self.assertRaisesRegex(
            sdf_model_import.ModelImportError, "expected 'area_001'"
        ):
            amb_scene_import.import_geometry(
                self.data, self.document, bytes(self.binary)
            )

    def test_imports_from_a_composed_scene(self) -> None:
        builder = fld_model.GltfBuilder.create()
        marker = fld_model.add_marker_mesh(builder, "existing", 0, 1.0)
        builder.document["nodes"].append({"name": "existing", "mesh": marker})
        builder.document["scenes"][0]["nodes"].append(0)
        document, binary = amb_scene.append_automap_scene(
            builder.document,
            bytes(builder.binary),
            self.data,
            meters_per_unit=0.01,
            icon_marker_size=0.0,
        )
        rebuilt, summary = amb_scene_import.import_geometry(
            self.data, document, binary
        )
        self.assertEqual(rebuilt, self.data)
        self.assertEqual(summary.models, 1)

    def test_complete_tracked_corpus_round_trips(self) -> None:
        for version in ("dds1", "dds2"):
            paths = sorted(
                (ROOT / "src" / version / "data" / "field").glob("*.ambasm")
            )
            for path in paths:
                data = amb.encode(amb.parse_source(path.read_text(encoding="utf-8")))
                document, binary = amb_scene.build_gltf(
                    data, icon_marker_size=0.0
                )
                rebuilt, _ = amb_scene_import.import_geometry(
                    data, document, binary
                )
                self.assertEqual(rebuilt, data, path.name)


if __name__ == "__main__":
    unittest.main()
