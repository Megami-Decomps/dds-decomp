#!/usr/bin/env python3
"""Apply glTF vertex-stream edits to DDS SDF model graphs."""

from __future__ import annotations

import json
import math
import struct
from dataclasses import dataclass

import fld
import fld_model


class ModelImportError(ValueError):
    """Raised when a GLB cannot be mapped safely to one DDS model source."""


GLB_HEADER = struct.Struct("<4sII")
GLB_CHUNK = struct.Struct("<II")
COMPONENT_FORMATS = {
    fld_model.UNSIGNED_BYTE: "B",
    fld_model.UNSIGNED_SHORT: "H",
    fld_model.UNSIGNED_INT: "I",
    fld_model.FLOAT: "f",
}
TYPE_WIDTHS = {"SCALAR": 1, "VEC2": 2, "VEC3": 3, "VEC4": 4}


@dataclass(frozen=True)
class ImportSummary:
    models: int
    meshes: int
    changed_meshes: int
    positions: int
    normals: int
    texcoords: int
    attributes: int
    colors: int


@dataclass(frozen=True)
class ModelGraph:
    name: str
    items: tuple[fld.ModelItem, ...]
    draw_roots: dict[int, tuple[int, ...]]
    draw_lists: dict[int, fld.ModelDrawList]
    draws: dict[int, fld.ModelDraw]


def decode_glb(data: bytes) -> tuple[dict, bytes]:
    """Read one self-contained glTF 2.0 binary document."""

    if len(data) < GLB_HEADER.size + GLB_CHUNK.size:
        raise ModelImportError("GLB is truncated")
    magic, version, total_size = GLB_HEADER.unpack_from(data)
    if magic != b"glTF" or version != 2:
        raise ModelImportError("input is not a glTF 2.0 binary")
    if total_size != len(data):
        raise ModelImportError(
            f"GLB header size is {total_size}, but file size is {len(data)}"
        )
    chunks = []
    offset = GLB_HEADER.size
    while offset < len(data):
        if offset + GLB_CHUNK.size > len(data):
            raise ModelImportError("GLB has a truncated chunk header")
        size, kind = GLB_CHUNK.unpack_from(data, offset)
        offset += GLB_CHUNK.size
        end = offset + size
        if end > len(data):
            raise ModelImportError("GLB has a truncated chunk")
        chunks.append((kind, data[offset:end]))
        offset = end
    if len(chunks) != 2 or chunks[0][0] != fld_model.GLB_JSON_CHUNK:
        raise ModelImportError(
            "GLB must contain one JSON chunk followed by one BIN chunk"
        )
    if chunks[1][0] != fld_model.GLB_BIN_CHUNK:
        raise ModelImportError("GLB second chunk is not binary data")
    try:
        document = json.loads(chunks[0][1].decode("utf-8"))
    except (UnicodeDecodeError, json.JSONDecodeError) as exc:
        raise ModelImportError(f"invalid GLB JSON: {exc}") from exc
    if not isinstance(document, dict):
        raise ModelImportError("GLB JSON root is not an object")
    buffers = document.get("buffers")
    if not isinstance(buffers, list) or len(buffers) != 1:
        raise ModelImportError("GLB must have exactly one embedded buffer")
    byte_length = buffers[0].get("byteLength") if isinstance(buffers[0], dict) else None
    binary = chunks[1][1]
    if not isinstance(byte_length, int) or not 0 <= byte_length <= len(binary):
        raise ModelImportError("GLB buffer has an invalid byte length")
    if len(binary) - byte_length > 3 or any(binary[byte_length:]):
        raise ModelImportError("GLB binary padding is invalid")
    return document, binary[:byte_length]


def _records(
    document: dict,
    binary: bytes,
    accessor_index: object,
    value_type: str,
    component_types: set[int],
    context: str,
    *,
    normalized: bool | None = None,
) -> tuple[tuple[int | float, ...], ...]:
    accessors = document.get("accessors")
    views = document.get("bufferViews")
    if not isinstance(accessor_index, int) or not isinstance(accessors, list):
        raise ModelImportError(f"{context} has no valid accessor")
    if not 0 <= accessor_index < len(accessors) or not isinstance(
        accessors[accessor_index], dict
    ):
        raise ModelImportError(f"{context} accessor is outside the GLB")
    accessor = accessors[accessor_index]
    if "sparse" in accessor:
        raise ModelImportError(f"{context} uses a sparse accessor")
    component_type = accessor.get("componentType")
    count = accessor.get("count")
    if (
        accessor.get("type") != value_type
        or component_type not in component_types
        or not isinstance(count, int)
        or count < 0
    ):
        raise ModelImportError(f"{context} accessor has an incompatible format")
    if normalized is not None and accessor.get("normalized", False) is not normalized:
        raise ModelImportError(f"{context} accessor has incompatible normalization")
    view_index = accessor.get("bufferView")
    if (
        not isinstance(view_index, int)
        or not isinstance(views, list)
        or not 0 <= view_index < len(views)
        or not isinstance(views[view_index], dict)
    ):
        raise ModelImportError(f"{context} has no valid buffer view")
    view = views[view_index]
    if view.get("buffer", 0) != 0:
        raise ModelImportError(f"{context} is not in the embedded buffer")
    code = COMPONENT_FORMATS[component_type]
    width = TYPE_WIDTHS[value_type]
    record_size = struct.calcsize("<" + code * width)
    stride = view.get("byteStride", record_size)
    if not isinstance(stride, int) or stride < record_size:
        raise ModelImportError(f"{context} has an invalid byte stride")
    view_start = view.get("byteOffset", 0)
    accessor_start = accessor.get("byteOffset", 0)
    view_size = view.get("byteLength")
    if (
        not isinstance(view_start, int)
        or not isinstance(accessor_start, int)
        or not isinstance(view_size, int)
        or view_start < 0
        or accessor_start < 0
        or view_size < 0
    ):
        raise ModelImportError(f"{context} has invalid byte offsets")
    start = view_start + accessor_start
    end = start if not count else start + (count - 1) * stride + record_size
    if start < view_start or end > view_start + view_size or end > len(binary):
        raise ModelImportError(f"{context} accessor exceeds its buffer view")
    return tuple(
        struct.unpack_from("<" + code * width, binary, start + index * stride)
        for index in range(count)
    )


def _f32(value: float) -> float:
    try:
        return struct.unpack("<f", struct.pack("<f", value))[0]
    except (OverflowError, struct.error) as exc:
        raise ModelImportError(
            f"value {value!r} cannot be represented as float32"
        ) from exc


def _asset_metadata(document: dict) -> float:
    asset = document.get("asset")
    extras = asset.get("extras") if isinstance(asset, dict) else None
    if not isinstance(extras, dict):
        raise ModelImportError("GLB has no DDS asset metadata")
    scale = extras.get("ddsMetersPerUnit")
    if not isinstance(scale, (int, float)) or not math.isfinite(scale) or scale <= 0:
        raise ModelImportError("GLB has no positive finite DDS unit scale")
    if extras.get("ddsNativeAxesPreserved") is not True:
        raise ModelImportError("GLB does not preserve native DDS axes")
    return float(scale)


def import_model_graphs(
    source_data: bytes,
    document: dict,
    binary: bytes,
    graphs: tuple[ModelGraph, ...],
) -> tuple[bytes, ImportSummary]:
    """Apply changed vertex attributes while preserving SDF packet structure."""

    meters_per_unit = _asset_metadata(document)
    meshes = document.get("meshes")
    if not isinstance(meshes, list):
        raise ModelImportError("GLB has no mesh array")
    if not graphs:
        raise ModelImportError("no SDF model graphs were selected")
    graph_names = tuple(graph.name for graph in graphs)
    if len(set(graph_names)) != len(graph_names):
        raise ModelImportError("selected SDF model graphs repeat a name")
    selected_prefixes = tuple(f"{name}/node_" for name in graph_names)

    meshes_by_name: dict[str, dict] = {}
    for mesh in meshes:
        name = mesh.get("name") if isinstance(mesh, dict) else None
        if isinstance(name, str) and name.startswith(selected_prefixes):
            if name in meshes_by_name:
                raise ModelImportError(f"GLB repeats mesh name {name!r}")
            meshes_by_name[name] = mesh

    output = bytearray(source_data)
    stream_values: dict[tuple[int, str], tuple[tuple[int | float, ...], ...]] = {}
    mesh_keys: set[tuple[int, int, int]] = set()
    changed_meshes: set[tuple[int, int, int]] = set()
    changes = {
        name: 0
        for name in ("positions", "normals", "texcoords", "attributes", "colors")
    }

    def patch_floats(
        key: tuple[int, str],
        offset: int,
        actual: tuple[tuple[int | float, ...], ...],
        expected: tuple[tuple[float, ...], ...],
        scale: float,
        context: str,
    ) -> bool:
        previous = stream_values.get(key)
        if previous is not None:
            if previous != actual:
                raise ModelImportError(f"{context} conflicts with another shared draw")
            return False
        stream_values[key] = actual
        if len(actual) != len(expected):
            raise ModelImportError(
                f"{context} has {len(actual)} records, expected {len(expected)}"
            )
        changed = False
        width = len(expected[0]) if expected else 0
        for row, (new_record, old_record) in enumerate(
            zip(actual, expected, strict=True)
        ):
            if len(new_record) != width or not all(
                isinstance(value, float) and math.isfinite(value)
                for value in new_record
            ):
                raise ModelImportError(f"{context} has non-finite or malformed values")
            for column, (value, old) in enumerate(
                zip(new_record, old_record, strict=True)
            ):
                exported = _f32(old * scale)
                if value == exported:
                    continue
                native = _f32(value / scale)
                struct.pack_into(
                    "<f", output, offset + (row * width + column) * 4, native
                )
                changed = True
        return changed

    def patch_colors(
        key: tuple[int, str],
        offset: int,
        actual: tuple[tuple[int | float, ...], ...],
        expected: tuple[tuple[int, int, int, int], ...],
        context: str,
    ) -> bool:
        previous = stream_values.get(key)
        if previous is not None:
            if previous != actual:
                raise ModelImportError(f"{context} conflicts with another shared draw")
            return False
        stream_values[key] = actual
        if len(actual) != len(expected):
            raise ModelImportError(
                f"{context} has {len(actual)} records, expected {len(expected)}"
            )
        changed = False
        for row, (new_record, old_record) in enumerate(
            zip(actual, expected, strict=True)
        ):
            if len(new_record) != 4 or any(
                not isinstance(value, int) or not 0 <= value <= 0xFF
                for value in new_record
            ):
                raise ModelImportError(f"{context} has malformed color values")
            for column, (value, old) in enumerate(
                zip(new_record, old_record, strict=True)
            ):
                if value == min(old * 2, 0xFF):
                    continue
                output[offset + row * 4 + column] = min((value + 1) // 2, 0x80)
                changed = True
        return changed

    consumed_mesh_names: set[str] = set()
    for graph in graphs:
        source_name = graph.name
        command_meshes: dict[int, str] = {}
        for item in graph.items:
            if not item.commands or item.commands in command_meshes:
                continue
            mesh_name = f"{source_name}/node_{item.node_id}"
            command_meshes[item.commands] = mesh_name
            try:
                gltf_mesh = meshes_by_name[mesh_name]
            except KeyError as exc:
                raise ModelImportError(f"GLB is missing mesh {mesh_name!r}") from exc
            consumed_mesh_names.add(mesh_name)
            primitives = gltf_mesh.get("primitives")
            if not isinstance(primitives, list):
                raise ModelImportError(f"mesh {mesh_name!r} has no primitives")
            expected_primitives = []
            for list_index, list_offset in enumerate(
                graph.draw_roots[item.commands]
            ):
                draw_list = graph.draw_lists[list_offset]
                for draw_index, draw_offset in enumerate(draw_list.draws):
                    draw = graph.draws[draw_offset]
                    packet_size = draw.quadwords * 0x10
                    packet_meshes, _ = fld._read_model_mesh_packet(
                        source_data,
                        draw.packet,
                        packet_size,
                        f"model {source_name} packet",
                    )
                    for packet_mesh_index, source_mesh in enumerate(packet_meshes):
                        expected_primitives.append(
                            (
                                list_index,
                                draw_index,
                                draw,
                                draw_list,
                                packet_size,
                                packet_mesh_index,
                                source_mesh,
                            )
                        )
            if len(primitives) != len(expected_primitives):
                raise ModelImportError(
                    f"mesh {mesh_name!r} has {len(primitives)} primitives, "
                    f"expected {len(expected_primitives)}"
                )
            for primitive, expected in zip(
                primitives, expected_primitives, strict=True
            ):
                if not isinstance(primitive, dict) or primitive.get("mode", 4) != 4:
                    raise ModelImportError(
                        f"mesh {mesh_name!r} has a non-triangle primitive"
                    )
                (
                    list_index,
                    draw_index,
                    draw,
                    draw_list,
                    packet_size,
                    packet_mesh_index,
                    source_mesh,
                ) = expected
                context = f"mesh {mesh_name!r} primitive {len(mesh_keys)}"
                extras = primitive.get("extras")
                metadata = {
                    "ddsAsset": draw.asset,
                    "ddsDrawSelector": draw_list.selector,
                    "ddsDrawListIndex": list_index,
                    "ddsDrawIndex": draw_index,
                    "ddsPacketMeshIndex": packet_mesh_index,
                    "ddsMeshControls": list(source_mesh.controls),
                    "ddsProgramAddress": source_mesh.program,
                }
                if not isinstance(extras, dict) or any(
                    extras.get(name) != value for name, value in metadata.items()
                ):
                    raise ModelImportError(f"{context} DDS identity metadata differs")
                indices = _records(
                    document,
                    binary,
                    primitive.get("indices"),
                    "SCALAR",
                    {
                        fld_model.UNSIGNED_BYTE,
                        fld_model.UNSIGNED_SHORT,
                        fld_model.UNSIGNED_INT,
                    },
                    context + " indices",
                    normalized=False,
                )
                expected_indices = tuple(
                    (value,)
                    for triangle in source_mesh.triangles
                    for value in triangle[:3]
                )
                if indices != expected_indices:
                    raise ModelImportError(f"{context} changes triangle topology")
                controls = _records(
                    document,
                    binary,
                    extras.get("ddsTriangleControlAccessor"),
                    "SCALAR",
                    {fld_model.UNSIGNED_BYTE},
                    context + " triangle controls",
                    normalized=False,
                )
                if controls != tuple(
                    (triangle[3],) for triangle in source_mesh.triangles
                ):
                    raise ModelImportError(f"{context} changes triangle controls")

                source_attributes = {
                    "POSITION": (
                        source_mesh.positions,
                        source_mesh.positions_offset,
                        3,
                    ),
                }
                if source_mesh.normals is not None:
                    source_attributes["NORMAL"] = (
                        source_mesh.normals,
                        source_mesh.normals_offset,
                        3,
                    )
                if source_mesh.texcoords is not None:
                    source_attributes["TEXCOORD_0"] = (
                        source_mesh.texcoords,
                        source_mesh.texcoords_offset,
                        2,
                    )
                if source_mesh.attributes is not None:
                    source_attributes["_DDS_ATTRIBUTE"] = (
                        source_mesh.attributes,
                        source_mesh.attributes_offset,
                        4,
                    )
                if source_mesh.colors is not None:
                    source_attributes["COLOR_0"] = (
                        source_mesh.colors,
                        source_mesh.colors_offset,
                        4,
                    )
                attributes = primitive.get("attributes")
                if not isinstance(attributes, dict) or set(attributes) != set(
                    source_attributes
                ):
                    raise ModelImportError(
                        f"{context} changes the vertex-channel layout"
                    )

                mesh_key = draw.packet, packet_size, packet_mesh_index
                mesh_keys.add(mesh_key)
                mesh_changed = False
                for attribute_name, (
                    old_records,
                    native_offset,
                    width,
                ) in source_attributes.items():
                    if native_offset is None:
                        raise AssertionError(attribute_name)
                    value_type = f"VEC{width}"
                    stream_key = native_offset, attribute_name
                    if attribute_name == "COLOR_0":
                        actual = _records(
                            document,
                            binary,
                            attributes[attribute_name],
                            value_type,
                            {fld_model.UNSIGNED_BYTE},
                            context + " colors",
                            normalized=True,
                        )
                        changed = patch_colors(
                            stream_key,
                            native_offset,
                            actual,
                            old_records,
                            context + " colors",
                        )
                        label = "colors"
                    else:
                        actual = _records(
                            document,
                            binary,
                            attributes[attribute_name],
                            value_type,
                            {fld_model.FLOAT},
                            context + f" {attribute_name}",
                            normalized=False,
                        )
                        label = {
                            "POSITION": "positions",
                            "NORMAL": "normals",
                            "TEXCOORD_0": "texcoords",
                            "_DDS_ATTRIBUTE": "attributes",
                        }[attribute_name]
                        changed = patch_floats(
                            stream_key,
                            native_offset,
                            actual,
                            old_records,
                            meters_per_unit if attribute_name == "POSITION" else 1.0,
                            context + f" {label}",
                        )
                    if changed:
                        changes[label] += 1
                        mesh_changed = True
                if mesh_changed:
                    changed_meshes.add(mesh_key)
    extra_meshes = set(meshes_by_name) - consumed_mesh_names
    if extra_meshes:
        raise ModelImportError("GLB contains an unrecognized selected-model mesh")
    rebuilt = bytes(output)
    return rebuilt, ImportSummary(
        len(graphs),
        len(mesh_keys),
        len(changed_meshes),
        changes["positions"],
        changes["normals"],
        changes["texcoords"],
        changes["attributes"],
        changes["colors"],
    )
