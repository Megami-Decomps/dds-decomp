#!/usr/bin/env python3
"""Import edited FLD2 transforms and collision vertices from a composed GLB."""

from __future__ import annotations

import argparse
import math
import struct
from dataclasses import dataclass
from pathlib import Path

import fld
import fld_model
from gltf_import import (
    GltfImportError as FieldSceneImportError,
    decode_glb,
    f32,
    records,
)


SUPPORTED_TYPES = frozenset({3, 4, 10})


@dataclass(frozen=True)
class ImportSummary:
    resources: int
    changed_resources: int
    translations: int
    rotations: int
    scales: int
    collision_meshes: int
    changed_collision_meshes: int
    collision_vertices: int


def _resource_name(data: bytes, resource: fld.Resource) -> str:
    if resource.name:
        return fld._fixed_string(data, resource.name, "field resource name")
    return f"type_{resource.type_id}_resource_{resource.serial:04d}"


def _vec(node: dict, key: str, default: tuple[float, ...]) -> list[float]:
    value = node.get(key, default)
    if (
        not isinstance(value, list)
        or len(value) != len(default)
        or any(
            not isinstance(item, (int, float)) or isinstance(item, bool)
            for item in value
        )
    ):
        raise FieldSceneImportError(
            f"node {node.get('name', '<unnamed>')!r} has an invalid {key}"
        )
    converted = [float(item) for item in value]
    if not all(math.isfinite(item) for item in converted):
        raise FieldSceneImportError(
            f"node {node.get('name', '<unnamed>')!r} has a non-finite {key}"
        )
    return converted


def _same(actual: list[float], expected: list[float]) -> bool:
    return len(actual) == len(expected) and all(
        left == right for left, right in zip(actual, expected, strict=True)
    )


def _pack_component(
    output: bytearray,
    offset: int,
    values: list[float],
    context: str,
) -> None:
    try:
        struct.pack_into(f"<{len(values)}f", output, offset, *values)
    except (OverflowError, struct.error) as exc:
        raise FieldSceneImportError(
            f"{context} cannot be represented as FLD2 float32 values"
        ) from exc


def _collision_source(
    field_data: bytes,
    resource: fld.Resource,
) -> tuple[
    int,
    tuple[tuple[float, float, float, float], ...],
    tuple[tuple[int], ...],
    dict[str, int | bool],
]:
    values = struct.unpack_from("<12I", field_data, resource.data)
    vertex_count, face_count, extra_count = values[4:7]
    vertices_offset, faces_offset, stop = values[7:10]
    vertices = tuple(
        struct.unpack_from("<4f", field_data, vertices_offset + index * fld.VERTEX_SIZE)
        for index in range(vertex_count)
    )
    indices: list[tuple[int]] = []
    for face_index in range(face_count):
        face = struct.unpack_from(
            "<IBBH HBB 4I hhhh",
            field_data,
            faces_offset + face_index * fld.FACE_SIZE,
        )
        corners = face[7:11]
        indices.extend((corner,) for corner in corners[:3])
        if corners[3] != 0xFFFFFFFF:
            indices.extend(((corners[0],), (corners[2],), (corners[3],)))
    metadata: dict[str, int | bool] = {
        "ddsVertexCount": vertex_count,
        "ddsFaceCount": face_count,
        "ddsTriangleCount": len(indices) // 3,
        "ddsExtraCount": extra_count,
        "ddsHasStopData": bool(stop),
    }
    return vertices_offset, vertices, tuple(indices), metadata


def _import_collision(
    field_data: bytes,
    output: bytearray,
    resource: fld.Resource,
    node: dict,
    document: dict,
    binary: bytes,
    meters_per_unit: float,
    used_meshes: set[int],
) -> tuple[int, int, int]:
    """Import one collision mesh; return represented, changed, vertex counts."""

    name = _resource_name(field_data, resource)
    vertices_offset, source_vertices, expected_indices, metadata = _collision_source(
        field_data, resource
    )
    has_geometry = bool(source_vertices and expected_indices)
    if not has_geometry:
        if "mesh" in node:
            raise FieldSceneImportError(
                f"collision node {name!r} exposes geometry absent from the FLD2"
            )
        return 0, 0, 0

    meshes = document.get("meshes")
    mesh_index = node.get("mesh")
    if (
        not isinstance(mesh_index, int)
        or isinstance(mesh_index, bool)
        or not isinstance(meshes, list)
        or not 0 <= mesh_index < len(meshes)
        or not isinstance(meshes[mesh_index], dict)
    ):
        raise FieldSceneImportError(f"collision node {name!r} has no valid mesh")
    if mesh_index in used_meshes:
        raise FieldSceneImportError(
            f"collision mesh {mesh_index} is used more than once"
        )
    used_meshes.add(mesh_index)
    mesh = meshes[mesh_index]
    if mesh.get("name") != f"{name}/collision":
        raise FieldSceneImportError(
            f"collision node {name!r} has the wrong mesh identity"
        )
    extras = mesh.get("extras")
    if not isinstance(extras, dict) or any(
        extras.get(key) != value for key, value in metadata.items()
    ):
        raise FieldSceneImportError(f"collision mesh {name!r} metadata differs")
    primitives = mesh.get("primitives")
    if (
        not isinstance(primitives, list)
        or len(primitives) != 1
        or not isinstance(primitives[0], dict)
    ):
        raise FieldSceneImportError(
            f"collision mesh {name!r} must contain exactly one primitive"
        )
    primitive = primitives[0]
    if primitive.get("mode", 4) != 4:
        raise FieldSceneImportError(f"collision mesh {name!r} is not triangles")
    attributes = primitive.get("attributes")
    if not isinstance(attributes, dict) or set(attributes) != {"POSITION"}:
        raise FieldSceneImportError(
            f"collision mesh {name!r} changes the vertex-channel layout"
        )

    actual_indices = records(
        document,
        binary,
        primitive.get("indices"),
        "SCALAR",
        {
            fld_model.UNSIGNED_BYTE,
            fld_model.UNSIGNED_SHORT,
            fld_model.UNSIGNED_INT,
        },
        f"collision mesh {name!r} indices",
        normalized=False,
    )
    if actual_indices != expected_indices:
        raise FieldSceneImportError(f"collision mesh {name!r} changes face topology")
    positions = records(
        document,
        binary,
        attributes["POSITION"],
        "VEC3",
        {fld_model.FLOAT},
        f"collision mesh {name!r} positions",
        normalized=False,
    )
    if len(positions) != len(source_vertices):
        raise FieldSceneImportError(
            f"collision mesh {name!r} has {len(positions)} vertices, "
            f"expected {len(source_vertices)}"
        )

    changed_vertices = 0
    for vertex_index, (actual, source) in enumerate(
        zip(positions, source_vertices, strict=True)
    ):
        if not all(
            isinstance(value, float) and math.isfinite(value) for value in actual
        ):
            raise FieldSceneImportError(
                f"collision mesh {name!r} has a non-finite vertex"
            )
        changed = False
        for axis, (value, old_value) in enumerate(
            zip(actual, source[:3], strict=True)
        ):
            if value == f32(old_value * meters_per_unit):
                continue
            native = f32(value / meters_per_unit)
            if not math.isfinite(native):
                raise FieldSceneImportError(
                    f"collision mesh {name!r} vertex cannot be represented as float32"
                )
            struct.pack_into(
                "<f",
                output,
                vertices_offset + vertex_index * fld.VERTEX_SIZE + axis * 4,
                native,
            )
            changed = True
        changed_vertices += changed
    return 1, int(bool(changed_vertices)), changed_vertices


def import_scene(
    field_data: bytes,
    document: dict,
    binary: bytes,
) -> tuple[bytes, ImportSummary]:
    """Apply changed FLD2 collision geometry and static TRS components."""

    fld.validate(field_data)
    if field_data[4:8] != b"FLD2":
        raise FieldSceneImportError("field scene import requires an FLD2 file")
    asset = document.get("asset")
    extras = asset.get("extras") if isinstance(asset, dict) else None
    if not isinstance(extras, dict):
        raise FieldSceneImportError("GLB has no DDS asset metadata")
    meters_per_unit = extras.get("ddsMetersPerUnit")
    if (
        not isinstance(meters_per_unit, (int, float))
        or isinstance(meters_per_unit, bool)
        or not math.isfinite(meters_per_unit)
        or meters_per_unit <= 0.0
    ):
        raise FieldSceneImportError("GLB has no positive finite DDS unit scale")
    meters_per_unit = float(meters_per_unit)
    if extras.get("ddsNativeAxesPreserved") is not True:
        raise FieldSceneImportError("GLB does not preserve native DDS axes")

    words, data_end, _ = fld._read_header(field_data)
    resources = fld._read_resources(
        field_data, fld._read_types(field_data, words, data_end)
    )
    by_key: dict[tuple[int, int], fld.Resource] = {}
    for resource in resources:
        if resource.type_id not in SUPPORTED_TYPES:
            continue
        key = resource.type_id, resource.serial
        if key in by_key:
            raise FieldSceneImportError(
                f"FLD2 has duplicate type {key[0]} serial {key[1]} resources"
            )
        if not resource.transform:
            raise FieldSceneImportError(
                f"FLD2 type {key[0]} serial {key[1]} has no transform"
            )
        by_key[key] = resource

    nodes = document.get("nodes")
    if not isinstance(nodes, list):
        raise FieldSceneImportError("GLB has no node array")
    output = bytearray(field_data)
    seen: set[tuple[int, int]] = set()
    used_collision_meshes: set[int] = set()
    changed_resources: set[tuple[int, int]] = set()
    translations = rotations = scales = 0
    collision_meshes = changed_collision_meshes = collision_vertices = 0
    for node in nodes:
        if not isinstance(node, dict):
            continue
        node_extras = node.get("extras")
        if not isinstance(node_extras, dict):
            continue
        type_id = node_extras.get("ddsResourceType")
        serial = node_extras.get("ddsResourceSerial")
        if (
            type_id not in SUPPORTED_TYPES
            or not isinstance(serial, int)
            or isinstance(serial, bool)
        ):
            continue
        key = int(type_id), serial
        if key in seen:
            raise FieldSceneImportError(f"GLB repeats type {key[0]} serial {key[1]}")
        seen.add(key)
        try:
            resource = by_key[key]
        except KeyError as exc:
            raise FieldSceneImportError(
                f"GLB type {key[0]} serial {key[1]} is absent from the FLD2"
            ) from exc
        expected_name = _resource_name(field_data, resource)
        if node.get("name") != expected_name:
            raise FieldSceneImportError(
                f"GLB type {key[0]} serial {key[1]} is named "
                f"{node.get('name')!r}, expected {expected_name!r}"
            )
        if node_extras.get("ddsResourceFlags") != resource.flags:
            raise FieldSceneImportError(
                f"GLB type {key[0]} serial {key[1]} changes its resource flags"
            )
        if "matrix" in node:
            raise FieldSceneImportError(
                f"node {expected_name!r} uses a matrix; preserve editable TRS fields"
            )

        if resource.type_id == 3 and resource.data:
            represented, collision_changed, vertex_changes = _import_collision(
                field_data,
                output,
                resource,
                node,
                document,
                binary,
                meters_per_unit,
                used_collision_meshes,
            )
            collision_meshes += represented
            changed_collision_meshes += collision_changed
            collision_vertices += vertex_changes
            if collision_changed:
                changed_resources.add(key)
        elif resource.type_id == 3 and "mesh" in node:
            raise FieldSceneImportError(
                f"collision node {expected_name!r} exposes geometry absent from "
                "the FLD2"
            )

        native = list(struct.unpack_from("<12f", field_data, resource.transform))
        translation_values = native[:3]
        translation = (
            None
            if "translation" not in node
            and not all(math.isfinite(value) for value in translation_values)
            else _vec(node, "translation", (0.0, 0.0, 0.0))
        )
        old_translation = [value * meters_per_unit for value in translation_values]
        if translation is not None and (
            not all(math.isfinite(value) for value in old_translation)
            or not _same(translation, old_translation)
        ):
            _pack_component(
                output,
                resource.transform,
                [value / meters_per_unit for value in translation],
                f"node {expected_name!r} translation",
            )
            translations += 1
            changed_resources.add(key)

        rotation_values = native[4:8]
        rotation = (
            None
            if "rotation" not in node
            and not all(math.isfinite(value) for value in rotation_values)
            else _vec(node, "rotation", (0.0, 0.0, 0.0, 1.0))
        )
        old_rotation = (
            fld_model._normalized_quaternion(tuple(rotation_values))
            if all(math.isfinite(value) for value in rotation_values)
            else []
        )
        if rotation is not None and not _same(rotation, old_rotation):
            length = math.sqrt(sum(value * value for value in rotation))
            if not math.isfinite(length) or length == 0.0:
                raise FieldSceneImportError(
                    f"node {expected_name!r} rotation is not a finite quaternion"
                )
            normalized = [value / length for value in rotation]
            _pack_component(
                output,
                resource.transform + 0x10,
                normalized,
                f"node {expected_name!r} rotation",
            )
            rotations += 1
            changed_resources.add(key)

        scale_values = native[8:11]
        scale = (
            None
            if "scale" not in node
            and not all(math.isfinite(value) for value in scale_values)
            else _vec(node, "scale", (1.0, 1.0, 1.0))
        )
        if scale is not None and (
            not all(math.isfinite(value) for value in scale_values)
            or not _same(scale, scale_values)
        ):
            _pack_component(
                output,
                resource.transform + 0x20,
                scale,
                f"node {expected_name!r} scale",
            )
            scales += 1
            changed_resources.add(key)

    missing = set(by_key) - seen
    if missing:
        type_id, serial = min(missing)
        raise FieldSceneImportError(
            f"GLB is missing type {type_id} serial {serial}"
        )
    if not by_key:
        has_field_wrapper = any(
            isinstance(node, dict)
            and node.get("name") == "FLD2 field data"
            and isinstance(node.get("extras"), dict)
            for node in nodes
        )
        if not has_field_wrapper:
            raise FieldSceneImportError("GLB has no FLD2 field-data wrapper")

    rebuilt = bytes(output)
    fld.validate(rebuilt)
    return rebuilt, ImportSummary(
        len(seen),
        len(changed_resources),
        translations,
        rotations,
        scales,
        collision_meshes,
        changed_collision_meshes,
        collision_vertices,
    )


def _source_or_binary(path: Path) -> bytes:
    if path.suffix.lower() == ".fldasm":
        return fld.encode(fld.parse_source(path.read_text(encoding="utf-8")))
    return path.read_bytes()


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("scene", type=Path)
    parser.add_argument("input", type=Path, help="FLD2 binary or .fldasm source")
    parser.add_argument(
        "output", type=Path, help="output FLD2 binary or .fldasm source"
    )
    args = parser.parse_args()
    try:
        document, binary = decode_glb(args.scene.read_bytes())
        data, summary = import_scene(_source_or_binary(args.input), document, binary)
        args.output.parent.mkdir(parents=True, exist_ok=True)
        if args.output.suffix.lower() == ".fldasm":
            args.output.write_text(fld.render_source(data), encoding="utf-8")
        else:
            args.output.write_bytes(data)
    except (FieldSceneImportError, OSError, ValueError) as exc:
        parser.error(str(exc))
    print(
        f"imported {summary.resources} resources and {summary.collision_meshes} "
        f"collision meshes; changed {summary.changed_resources} resources "
        f"({summary.translations} translations, {summary.rotations} rotations, "
        f"{summary.scales} scales, {summary.changed_collision_meshes} collision "
        f"meshes, {summary.collision_vertices} collision vertices)"
    )


if __name__ == "__main__":
    main()
