#!/usr/bin/env python3
"""Import edited FLD1 mesh attributes from a DDS model GLB."""

from __future__ import annotations

import argparse
import json
import math
import struct
from dataclasses import dataclass
from pathlib import Path

import fld
import fld_model


class ModelImportError(ValueError):
    """Raised when a GLB cannot be mapped safely to one FLD1 source."""


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
    resources: int
    meshes: int
    changed_meshes: int
    positions: int
    normals: int
    texcoords: int
    attributes: int
    colors: int


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


def import_geometry(
    field_data: bytes,
    document: dict,
    binary: bytes,
) -> tuple[bytes, ImportSummary]:
    """Apply changed model vertex attributes while preserving packet structure."""

    fld.validate(field_data)
    if field_data[4:8] != b"FLD1":
        raise ModelImportError("model geometry import requires an FLD1 file")
    meters_per_unit = _asset_metadata(document)
    nodes = document.get("nodes")
    meshes = document.get("meshes")
    if not isinstance(nodes, list) or not isinstance(meshes, list):
        raise ModelImportError("GLB has no node or mesh array")

    selected: dict[int, str] = {}
    for node in nodes:
        if not isinstance(node, dict):
            continue
        extras = node.get("extras")
        if not isinstance(extras, dict) or "ddsMotionBindings" not in extras:
            continue
        serial = extras.get("ddsResourceSerial")
        name = node.get("name")
        if not isinstance(serial, int) or not isinstance(name, str):
            raise ModelImportError("model wrapper has an invalid identity")
        if serial in selected:
            raise ModelImportError(f"GLB repeats model resource serial {serial}")
        selected[serial] = name
    if not selected:
        raise ModelImportError("GLB has no DDS model-resource wrappers")

    meshes_by_name: dict[str, dict] = {}
    for mesh in meshes:
        name = mesh.get("name") if isinstance(mesh, dict) else None
        if isinstance(name, str):
            if name in meshes_by_name:
                raise ModelImportError(f"GLB repeats mesh name {name!r}")
            meshes_by_name[name] = mesh

    words, data_end, relocation_tuple = fld._read_header(field_data)
    relocations = set(relocation_tuple)
    resources = fld._read_resources(
        field_data, fld._read_types(field_data, words, data_end)
    )
    source_by_serial = {
        resource.serial: resource
        for resource in resources
        if resource.type_id == 2 and resource.data
    }
    if len(source_by_serial) != sum(
        resource.type_id == 2 and bool(resource.data) for resource in resources
    ):
        raise ModelImportError("FLD1 repeats a model resource serial")

    output = bytearray(field_data)
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

    imported_resources = 0
    consumed_mesh_names: set[str] = set()
    for serial, gltf_name in selected.items():
        try:
            resource = source_by_serial[serial]
        except KeyError as exc:
            raise ModelImportError(
                f"GLB model serial {serial} is absent from the FLD1"
            ) from exc
        source_name = (
            fld._fixed_string(field_data, resource.name, "model resource name")
            if resource.name
            else f"model_{resource.serial:04d}"
        )
        if gltf_name != source_name:
            raise ModelImportError(
                f"GLB model serial {serial} is named {gltf_name!r}, "
                f"expected {source_name!r}"
            )
        model, items = fld._read_model_resource(
            field_data, resource.data, data_end, f"model {source_name}"
        )
        assets = fld._read_model_assets(
            field_data, model.assets, data_end, f"model {source_name} assets"
        )
        draw_roots, draw_lists, draws = fld._read_model_draw_graph(
            field_data,
            items,
            len(assets),
            data_end,
            relocations,
            f"model {source_name}",
        )
        command_meshes: dict[int, str] = {}
        for item in items:
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
            for list_index, list_offset in enumerate(draw_roots[item.commands]):
                draw_list = draw_lists[list_offset]
                for draw_index, draw_offset in enumerate(draw_list.draws):
                    draw = draws[draw_offset]
                    packet_size = draw.quadwords * 0x10
                    packet_meshes, _ = fld._read_model_mesh_packet(
                        field_data,
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
        imported_resources += 1

    extra_meshes = set(meshes_by_name) - consumed_mesh_names
    selected_prefixes = tuple(f"{name}/node_" for name in selected.values())
    if any(name.startswith(selected_prefixes) for name in extra_meshes):
        raise ModelImportError("GLB contains an unrecognized selected-model mesh")
    rebuilt = bytes(output)
    fld.validate(rebuilt)
    return rebuilt, ImportSummary(
        imported_resources,
        len(mesh_keys),
        len(changed_meshes),
        changes["positions"],
        changes["normals"],
        changes["texcoords"],
        changes["attributes"],
        changes["colors"],
    )


def _source_or_binary(path: Path) -> bytes:
    if path.suffix.lower() == ".f1asm":
        return fld.encode(fld.parse_source(path.read_text(encoding="utf-8")))
    return path.read_bytes()


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("scene", type=Path)
    parser.add_argument("input", type=Path, help="FLD1 binary or .f1asm source")
    parser.add_argument("output", type=Path, help="output FLD1 binary or .f1asm source")
    args = parser.parse_args()
    try:
        document, binary = decode_glb(args.scene.read_bytes())
        data, summary = import_geometry(
            _source_or_binary(args.input), document, binary
        )
        args.output.parent.mkdir(parents=True, exist_ok=True)
        if args.output.suffix.lower() == ".f1asm":
            args.output.write_text(fld.render_source(data), encoding="utf-8")
        else:
            args.output.write_bytes(data)
    except (ModelImportError, OSError, ValueError) as exc:
        parser.error(str(exc))
    print(
        f"imported {summary.resources} resources and {summary.meshes} meshes; "
        f"changed {summary.changed_meshes} meshes "
        f"({summary.positions} position, {summary.normals} normal, "
        f"{summary.texcoords} texcoord, {summary.attributes} attribute, "
        f"{summary.colors} color streams)"
    )


if __name__ == "__main__":
    main()
