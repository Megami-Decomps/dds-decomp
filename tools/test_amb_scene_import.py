from __future__ import annotations

import copy
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

    def test_imports_from_a_scene_with_unrelated_geometry(self) -> None:
        builder = fld_model.GltfBuilder(self.document, self.binary)
        marker = fld_model.add_marker_mesh(builder, "unrelated", 0, 1.0)
        builder.document["nodes"].append({"name": "unrelated", "mesh": marker})
        builder.document["scenes"][0]["nodes"].append(
            len(builder.document["nodes"]) - 1
        )
        builder.document["buffers"] = [{"byteLength": len(builder.binary)}]
        rebuilt, summary = amb_scene_import.import_geometry(
            self.data, builder.document, bytes(builder.binary)
        )
        self.assertEqual(rebuilt, self.data)
        self.assertEqual(summary.models, 1)

    def test_rejects_unsupported_scene_edits(self) -> None:
        def alternate_material(document: dict) -> None:
            document["materials"].append({"name": "unsupported replacement"})
            document["meshes"][0]["primitives"][0]["material"] = (
                len(document["materials"]) - 1
            )

        def morph_target(document: dict) -> None:
            primitive = document["meshes"][0]["primitives"][0]
            primitive["targets"] = [
                {"POSITION": primitive["attributes"]["POSITION"]}
            ]

        def mesh_morph_weights(document: dict) -> None:
            document["meshes"][0]["weights"] = [0.5]

        def node_morph_weights(document: dict) -> None:
            node = next(
                item
                for item in document["nodes"]
                if "ddsNodeId" in item.get("extras", {})
            )
            node["weights"] = [0.5]

        def model_transform(document: dict) -> None:
            node = next(
                item
                for item in document["nodes"]
                if "ddsNodeId" in item.get("extras", {})
            )
            node["scale"] = [2.0, 1.0, 1.0]

        def wrapper_transform(document: dict) -> None:
            node = next(
                item
                for item in document["nodes"]
                if "ddsAreaIndex" in item.get("extras", {})
            )
            node["translation"] = [1.0, 0.0, 0.0]

        def detach_mesh(document: dict) -> None:
            node = next(
                item
                for item in document["nodes"]
                if "ddsNodeId" in item.get("extras", {}) and "mesh" in item
            )
            del node["mesh"]

        def change_model_hierarchy(document: dict) -> None:
            wrapper_index = next(
                index
                for index, item in enumerate(document["nodes"])
                if "ddsAreaIndex" in item.get("extras", {})
            )
            node = next(
                item
                for item in document["nodes"]
                if "ddsNodeId" in item.get("extras", {})
            )
            node["children"] = [wrapper_index]

        def change_wrapper_hierarchy(document: dict) -> None:
            wrapper = next(
                item
                for item in document["nodes"]
                if "ddsAreaIndex" in item.get("extras", {})
            )
            wrapper["children"] = [
                index
                for index in wrapper["children"]
                if "ddsNodeId" not in document["nodes"][index].get("extras", {})
            ]

        cases = (
            ("material", alternate_material, "material assignment"),
            ("morph target", morph_target, "morph targets"),
            ("mesh morph weights", mesh_morph_weights, "morph weights"),
            ("node morph weights", node_morph_weights, "morph weights"),
            ("model transform", model_transform, "node transform"),
            ("wrapper transform", wrapper_transform, "node transform"),
            ("mesh attachment", detach_mesh, "mesh attachment"),
            ("model hierarchy", change_model_hierarchy, "model hierarchy"),
            ("wrapper hierarchy", change_wrapper_hierarchy, "model hierarchy"),
        )
        for label, mutate, message in cases:
            with self.subTest(label=label):
                document = copy.deepcopy(self.document)
                mutate(document)
                with self.assertRaisesRegex(
                    sdf_model_import.ModelImportError, message
                ):
                    amb_scene_import.import_geometry(
                        self.data, document, bytes(self.binary)
                    )

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
