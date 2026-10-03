from __future__ import annotations

import copy
import struct
import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

import fld  # noqa: E402
import fld_model  # noqa: E402
import fld_model_import  # noqa: E402
from test_fld_model import SOURCE  # noqa: E402


EDIT_SOURCE = (
    SOURCE.replace("model_draw asset=0 qwords=4", "model_draw asset=0 qwords=10")
    .replace(
        "mesh_program address=12",
        "mesh_normals\n"
        "normal 0,0,1 0,0,1 0,0,1\n"
        "mesh_texcoords\n"
        "texcoord 0,0 1,0 0,1\n"
        "mesh_colors\n"
        "color 128,64,0,64 128,64,0,128 128,64,0,128\n"
        "mesh_program address=12\n"
        "vif_nops count=3",
    )
)

ATTRIBUTE_SOURCE = (
    SOURCE.replace("model_draw asset=0 qwords=4", "model_draw asset=0 qwords=8")
    .replace(
        "mesh_program address=12",
        "mesh_attributes\n"
        "attribute 1,2,3,4 5,6,7,8 9,10,11,12\n"
        "mesh_program address=12\n"
        "vif_nops count=3",
    )
)


def accessor_offset(document: dict, accessor_index: int) -> int:
    accessor = document["accessors"][accessor_index]
    view = document["bufferViews"][accessor["bufferView"]]
    return view.get("byteOffset", 0) + accessor.get("byteOffset", 0)


def first_mesh(data: bytes) -> fld.ModelMesh:
    words, data_end, relocations = fld._read_header(data)
    resources = fld._read_resources(data, fld._read_types(data, words, data_end))
    resource = next(resource for resource in resources if resource.type_id == 2)
    model, items = fld._read_model_resource(data, resource.data, data_end, "test")
    assets = fld._read_model_assets(data, model.assets, data_end, "test assets")
    roots, lists, draws = fld._read_model_draw_graph(
        data, items, len(assets), data_end, set(relocations), "test"
    )
    draw_list = lists[roots[items[0].commands][0]]
    draw = draws[draw_list.draws[0]]
    meshes, _ = fld._read_model_mesh_packet(
        data, draw.packet, draw.quadwords * 0x10, "test packet"
    )
    return meshes[0]


class FldModelImportTests(unittest.TestCase):
    def setUp(self) -> None:
        self.data = fld.encode(fld.parse_source(EDIT_SOURCE))
        self.document, binary = fld_model.build_gltf(
            self.data, meters_per_unit=0.01
        )
        self.binary = bytearray(binary)
        self.primitive = self.document["meshes"][0]["primitives"][0]

    def test_unchanged_glb_preserves_every_fld1_byte(self) -> None:
        glb = fld_model.encode_glb(self.document, bytes(self.binary))
        document, binary = fld_model_import.decode_glb(glb)
        rebuilt, summary = fld_model_import.import_geometry(
            self.data, document, binary
        )
        self.assertEqual(rebuilt, self.data)
        self.assertEqual(
            summary,
            fld_model_import.ImportSummary(1, 1, 0, 0, 0, 0, 0, 0),
        )

    def test_imports_vertex_attributes_without_changing_packet_shape(self) -> None:
        attributes = self.primitive["attributes"]
        struct.pack_into(
            "<f",
            self.binary,
            accessor_offset(self.document, attributes["POSITION"]),
            1.25,
        )
        struct.pack_into(
            "<f",
            self.binary,
            accessor_offset(self.document, attributes["NORMAL"]),
            0.5,
        )
        struct.pack_into(
            "<f",
            self.binary,
            accessor_offset(self.document, attributes["TEXCOORD_0"]),
            0.25,
        )
        self.binary[
            accessor_offset(self.document, attributes["COLOR_0"])
        ] = 0x80

        rebuilt, summary = fld_model_import.import_geometry(
            self.data, self.document, bytes(self.binary)
        )
        self.assertEqual(
            summary,
            fld_model_import.ImportSummary(1, 1, 1, 1, 1, 1, 0, 1),
        )
        mesh = first_mesh(rebuilt)
        self.assertEqual(mesh.positions[0][0], 125.0)
        self.assertEqual(mesh.normals[0][0], 0.5)
        self.assertEqual(mesh.texcoords[0][0], 0.25)
        self.assertEqual(mesh.colors[0][0], 64)
        self.assertEqual(mesh.triangles, first_mesh(self.data).triangles)
        self.assertEqual(mesh.controls, first_mesh(self.data).controls)
        self.assertEqual(len(rebuilt), len(self.data))
        fld.parse_source(fld.render_source(rebuilt))

    def test_imports_unclassified_four_float_attribute(self) -> None:
        data = fld.encode(fld.parse_source(ATTRIBUTE_SOURCE))
        document, binary = fld_model.build_gltf(data)
        binary = bytearray(binary)
        accessor = document["meshes"][0]["primitives"][0]["attributes"][
            "_DDS_ATTRIBUTE"
        ]
        struct.pack_into("<f", binary, accessor_offset(document, accessor), 2.5)

        rebuilt, summary = fld_model_import.import_geometry(
            data, document, bytes(binary)
        )
        self.assertEqual(
            summary,
            fld_model_import.ImportSummary(1, 1, 1, 0, 0, 0, 1, 0),
        )
        self.assertEqual(first_mesh(rebuilt).attributes[0][0], 2.5)

    def test_rejects_topology_and_identity_changes(self) -> None:
        indices = self.primitive["indices"]
        self.binary[accessor_offset(self.document, indices)] = 2
        with self.assertRaisesRegex(
            fld_model_import.ModelImportError, "triangle topology"
        ):
            fld_model_import.import_geometry(
                self.data, self.document, bytes(self.binary)
            )

        self.binary[accessor_offset(self.document, indices)] = 0
        self.primitive["extras"]["ddsPacketMeshIndex"] = 9
        with self.assertRaisesRegex(
            fld_model_import.ModelImportError, "identity metadata"
        ):
            fld_model_import.import_geometry(
                self.data, self.document, bytes(self.binary)
            )

    def test_requires_native_axes_and_matching_channels(self) -> None:
        self.document["asset"]["extras"]["ddsNativeAxesPreserved"] = False
        with self.assertRaisesRegex(
            fld_model_import.ModelImportError, "native DDS axes"
        ):
            fld_model_import.import_geometry(
                self.data, self.document, bytes(self.binary)
            )
        self.document["asset"]["extras"]["ddsNativeAxesPreserved"] = True
        del self.primitive["attributes"]["NORMAL"]
        with self.assertRaisesRegex(
            fld_model_import.ModelImportError, "vertex-channel layout"
        ):
            fld_model_import.import_geometry(
                self.data, self.document, bytes(self.binary)
            )

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
            node["translation"] = [123.0, 0.0, 0.0]

        def wrapper_transform(document: dict) -> None:
            node = next(
                item
                for item in document["nodes"]
                if "ddsResourceSerial" in item.get("extras", {})
            )
            node["matrix"] = [1.0, 0.0, 0.0, 0.0] * 4

        def detach_mesh(document: dict) -> None:
            node = next(item for item in document["nodes"] if "mesh" in item)
            del node["mesh"]

        def change_model_hierarchy(document: dict) -> None:
            wrapper_index = next(
                index
                for index, item in enumerate(document["nodes"])
                if "ddsResourceSerial" in item.get("extras", {})
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
                if "ddsResourceSerial" in item.get("extras", {})
            )
            wrapper["children"] = []

        cases = (
            ("material", alternate_material, "material assignment"),
            ("morph target", morph_target, "morph targets"),
            ("mesh morph weights", mesh_morph_weights, "morph weights"),
            ("node morph weights", node_morph_weights, "morph weights"),
            ("model transform", model_transform, "node transform"),
            ("wrapper transform", wrapper_transform, "node matrix"),
            ("mesh attachment", detach_mesh, "mesh attachment"),
            ("model hierarchy", change_model_hierarchy, "model hierarchy"),
            ("wrapper hierarchy", change_wrapper_hierarchy, "model hierarchy"),
        )
        for label, mutate, message in cases:
            with self.subTest(label=label):
                document = copy.deepcopy(self.document)
                mutate(document)
                with self.assertRaisesRegex(
                    fld_model_import.ModelImportError, message
                ):
                    fld_model_import.import_geometry(
                        self.data, document, bytes(self.binary)
                    )

    def test_tracked_fld1_sources_round_trip_unchanged(self) -> None:
        for version in ("dds1", "dds2"):
            paths = sorted(
                (ROOT / "src" / version / "data" / "field").glob("*.f1asm")
            )
            for path in paths:
                data = fld.encode(fld.parse_source(path.read_text(encoding="utf-8")))
                document, binary = fld_model.build_gltf(data)
                rebuilt, _ = fld_model_import.import_geometry(
                    data, document, binary
                )
                self.assertEqual(rebuilt, data, path.name)


if __name__ == "__main__":
    unittest.main()
