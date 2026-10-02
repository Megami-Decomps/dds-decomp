#!/usr/bin/env python3
"""Export a composed DDS field scene as a self-contained glTF 2.0 GLB."""

from __future__ import annotations

import argparse
import math
import re
import struct
from pathlib import Path

import fld
import fld_model
import field_world
import lb
import tmx
import wap


def _resource_name(data: bytes, resource: fld.Resource) -> str:
    if resource.name:
        return fld._fixed_string(data, resource.name, "field resource name")
    return f"type_{resource.type_id}_resource_{resource.serial:04d}"


def _apply_resource_transform(
    node: dict,
    data: bytes,
    resource: fld.Resource,
    meters_per_unit: float,
) -> None:
    if not resource.transform:
        return
    fld._range(data, resource.transform, fld.TRANSFORM_SIZE, "field resource transform")
    values = struct.unpack_from("<12f", data, resource.transform)
    translation = values[:3]
    fld_model._set_transform_component(
        node,
        "translation",
        translation,
        [value * meters_per_unit for value in translation],
    )
    rotation = values[4:8]
    fld_model._set_transform_component(
        node,
        "rotation",
        rotation,
        fld_model._normalized_quaternion(rotation)
        if all(math.isfinite(value) for value in rotation)
        else [],
    )
    scale = values[8:11]
    fld_model._set_transform_component(node, "scale", scale, list(scale))


def _unlit_material(document: dict, name: str, color: list[float]) -> int:
    extensions = document.setdefault("extensionsUsed", [])
    if "KHR_materials_unlit" not in extensions:
        extensions.append("KHR_materials_unlit")
    material = {
        "name": name,
        "doubleSided": True,
        "pbrMetallicRoughness": {
            "baseColorFactor": color,
            "metallicFactor": 0.0,
            "roughnessFactor": 1.0,
        },
        "extensions": {"KHR_materials_unlit": {}},
    }
    if color[3] < 1.0:
        material["alphaMode"] = "BLEND"
    index = len(document["materials"])
    document["materials"].append(material)
    return index


def _add_collision_mesh(
    builder: fld_model.GltfBuilder,
    data: bytes,
    resource: fld.Resource,
    name: str,
    material: int,
    meters_per_unit: float,
) -> int | None:
    values = struct.unpack_from("<12I", data, resource.data)
    vertex_count, face_count, extra_count = values[4:7]
    vertices_offset, faces_offset, stop = values[7:10]
    vertices = tuple(
        struct.unpack_from("<4f", data, vertices_offset + index * fld.VERTEX_SIZE)
        for index in range(vertex_count)
    )
    if any(not math.isfinite(value) for vertex in vertices for value in vertex[:3]):
        raise fld.FldError(f"collision {name} has non-finite vertices")

    indices: list[int] = []
    triangles = 0
    for face_index in range(face_count):
        face = struct.unpack_from(
            "<IBBH HBB 4I hhhh", data, faces_offset + face_index * fld.FACE_SIZE
        )
        corners = face[7:11]
        if 0xFFFFFFFF in corners[:3]:
            raise fld.FldError(
                f"collision {name} face {face_index} has a sentinel before its fourth vertex"
            )
        indices.extend(corners[:3])
        triangles += 1
        if corners[3] != 0xFFFFFFFF:
            indices.extend((corners[0], corners[2], corners[3]))
            triangles += 1
    if not vertices or not indices:
        return None

    positions = tuple(
        tuple(value * meters_per_unit for value in vertex[:3]) for vertex in vertices
    )
    position_accessor = builder.accessor(
        fld_model._pack_floats(positions),
        fld_model.FLOAT,
        "VEC3",
        len(positions),
        target=fld_model.ARRAY_BUFFER,
        minimum=[min(vertex[axis] for vertex in positions) for axis in range(3)],
        maximum=[max(vertex[axis] for vertex in positions) for axis in range(3)],
    )
    if max(indices) <= 0xFFFF:
        component_type = fld_model.UNSIGNED_SHORT
        payload = struct.pack("<" + "H" * len(indices), *indices)
    else:
        component_type = fld_model.UNSIGNED_INT
        payload = struct.pack("<" + "I" * len(indices), *indices)
    index_accessor = builder.accessor(
        payload,
        component_type,
        "SCALAR",
        len(indices),
        target=fld_model.ELEMENT_ARRAY_BUFFER,
        minimum=[min(indices)],
        maximum=[max(indices)],
    )
    mesh_index = len(builder.document["meshes"])
    builder.document["meshes"].append(
        {
            "name": f"{name}/collision",
            "primitives": [
                {
                    "attributes": {"POSITION": position_accessor},
                    "indices": index_accessor,
                    "material": material,
                    "mode": 4,
                }
            ],
            "extras": {
                "ddsFaceCount": face_count,
                "ddsTriangleCount": triangles,
                "ddsExtraCount": extra_count,
                "ddsHasStopData": bool(stop),
            },
        }
    )
    return mesh_index


def append_field_scene(
    document: dict,
    binary: bytes,
    field_data: bytes,
    *,
    meters_per_unit: float,
    placement_marker_size: float = 50.0,
    transitions: dict[str, tuple[dict, ...]] | None = None,
) -> tuple[dict, bytes]:
    """Append FLD2 collision, cameras, and placements to a glTF document."""

    if not math.isfinite(meters_per_unit) or meters_per_unit <= 0.0:
        raise fld.FldError("meters per unit must be a positive finite number")
    if not math.isfinite(placement_marker_size) or placement_marker_size < 0.0:
        raise fld.FldError("placement marker size must be finite and nonnegative")
    fld.validate(field_data)
    words, data_end, _ = fld._read_header(field_data)
    if field_data[4:8] != b"FLD2":
        raise fld.FldError("field scene export requires an FLD2 file")
    resources = fld._read_resources(
        field_data, fld._read_types(field_data, words, data_end)
    )
    builder = fld_model.GltfBuilder(document, bytearray(binary))
    scene_children = []
    collision_material = None
    marker_material = None
    marker_mesh = None
    counts = {"collision": 0, "camera": 0, "placement": 0}
    transition_actors: set[str] = set()

    for resource in resources:
        if resource.type_id not in {3, 4, 10}:
            continue
        name = _resource_name(field_data, resource)
        node = {
            "name": name,
            "extras": {
                "ddsResourceType": resource.type_id,
                "ddsResourceSerial": resource.serial,
                "ddsResourceFlags": resource.flags,
            },
        }
        _apply_resource_transform(node, field_data, resource, meters_per_unit)
        if resource.type_id == 3 and resource.data:
            if collision_material is None:
                collision_material = _unlit_material(
                    document, "FLD2 collision", [0.0, 0.65, 1.0, 0.28]
                )
            mesh = _add_collision_mesh(
                builder,
                field_data,
                resource,
                name,
                collision_material,
                meters_per_unit,
            )
            if mesh is not None:
                node["mesh"] = mesh
            counts["collision"] += 1
        elif resource.type_id == 4 and resource.data:
            node["extras"]["ddsCameraYFov"] = struct.unpack_from(
                "<f", field_data, resource.data
            )[0]
            counts["camera"] += 1
        elif resource.type_id == 10 and resource.data:
            kind, event_index, visible, payload = struct.unpack_from(
                "<IiII", field_data, resource.data
            )
            node["extras"].update(
                {
                    "ddsPlacementKind": kind,
                    "ddsEventIndex": event_index,
                    "ddsVisible": visible,
                }
            )
            if kind == 8 and payload:
                special_kind, point_id = fld._read_special_point(
                    field_data, payload, f"placement {name}"
                )
                node["extras"]["ddsSpecialPoint"] = {
                    "kind": fld.SPECIAL_POINT_KINDS[special_kind],
                    "id": point_id,
                }
            actor_transitions = (transitions or {}).get(name)
            if actor_transitions:
                node["extras"]["ddsTransitions"] = list(actor_transitions)
                transition_actors.add(name)
            if placement_marker_size > 0.0:
                if marker_material is None:
                    marker_material = _unlit_material(
                        document, "FLD2 placement", [1.0, 0.15, 0.65, 1.0]
                    )
                    marker_mesh = fld_model.add_marker_mesh(
                        builder,
                        "FLD2 placement marker",
                        marker_material,
                        placement_marker_size * meters_per_unit,
                    )
                node["mesh"] = marker_mesh
            counts["placement"] += 1
        node_index = len(document["nodes"])
        document["nodes"].append(node)
        scene_children.append(node_index)

    wrapper_index = len(document["nodes"])
    wrapper = {
        "name": "FLD2 field data",
        "extras": {
            "ddsCollisionResources": counts["collision"],
            "ddsCameraResources": counts["camera"],
            "ddsPlacementResources": counts["placement"],
        },
    }
    if transitions is not None:
        transition_rows = sum(len(rows) for rows in transitions.values())
        linked_rows = sum(
            len(rows) for name, rows in transitions.items() if name in transition_actors
        )
        wrapper["extras"].update(
            {
                "ddsTransitionRows": transition_rows,
                "ddsLinkedTransitionRows": linked_rows,
            }
        )
        unlinked = [
            {"actor": name, "entries": [row["entry"] for row in rows]}
            for name, rows in sorted(transitions.items())
            if name not in transition_actors
        ]
        if unlinked:
            wrapper["extras"]["ddsUnlinkedTransitionActors"] = unlinked
    if scene_children:
        wrapper["children"] = scene_children
    document["nodes"].append(wrapper)
    document["scenes"][document.get("scene", 0)]["nodes"].append(wrapper_index)
    builder.binary.extend(bytes((-len(builder.binary)) & 3))
    document["buffers"] = [{"byteLength": len(builder.binary)}]
    return document, bytes(builder.binary)


def build_scene(
    model_data: bytes,
    field_data: bytes,
    *,
    textures: tuple[tmx.Texture, ...] | None = None,
    resources: set[str] | None = None,
    meters_per_unit: float = 1.0,
    frames_per_second: float = 1.0,
    placement_marker_size: float = 50.0,
    warp_data: bytes | None = None,
    field_number: int | None = None,
    area_number: int | None = None,
) -> tuple[dict, bytes]:
    transitions = None
    if warp_data is not None:
        if field_number is None or area_number is None:
            raise fld.FldError("WAP scene metadata requires a field and area number")
        transitions = field_world.area_transitions(
            wap.decode(warp_data), field_number, area_number
        )
    document, binary = fld_model.build_gltf(
        model_data,
        textures=textures,
        resources=resources,
        meters_per_unit=meters_per_unit,
        frames_per_second=frames_per_second,
    )
    document, binary = append_field_scene(
        document,
        binary,
        field_data,
        meters_per_unit=meters_per_unit,
        placement_marker_size=placement_marker_size,
        transitions=transitions,
    )
    if warp_data is not None:
        document["asset"]["generator"] = "dds-decomp field-world exporter"
        document["asset"]["extras"].update(
            {"ddsFieldNumber": field_number, "ddsAreaNumber": area_number}
        )
    return document, binary


def _source_or_binary(path: Path, source_suffix: str) -> bytes:
    if path.suffix.lower() == source_suffix:
        return fld.encode(fld.parse_source(path.read_text(encoding="utf-8")))
    return path.read_bytes()


def _warp_source_or_binary(path: Path) -> bytes:
    if path.suffix.lower() != ".wapasm":
        return path.read_bytes()
    resolved = path.resolve()
    interaction_path = resolved.with_suffix(".infasm")
    game_root = resolved.parent.parent.parent
    script_path = game_root / "scripts" / "field" / f"{resolved.stem}.bfasm"
    if not interaction_path.is_file() or not script_path.is_file():
        raise wap.WapError(
            "WAP source export requires its paired INF and field script sources"
        )
    references = wap.load_references(script_path, interaction_path)
    return wap.encode(wap.parse_source(resolved.read_text(encoding="utf-8"), references))


def _field_identity(*paths: Path | None) -> tuple[int, int] | None:
    for path in paths:
        if path is None:
            continue
        match = re.fullmatch(r"[fk](\d{3})_(\d{3})", path.stem, re.IGNORECASE)
        if match:
            return int(match.group(1)), int(match.group(2))
    return None


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("input", type=Path)
    parser.add_argument("output", type=Path)
    parser.add_argument("--field", type=Path, help="FLD2 binary or source for a loose FLD1")
    parser.add_argument("--texture-bundle", type=Path, help="TBN/TXP0 bundle for a loose FLD1")
    parser.add_argument("--resource", action="append", dest="resources")
    parser.add_argument("--meters-per-unit", type=float, default=1.0)
    parser.add_argument("--frames-per-second", type=float, default=1.0)
    parser.add_argument("--placement-marker-size", type=float, default=50.0)
    parser.add_argument(
        "--warps",
        type=Path,
        help="WAP binary or tracked source whose transitions annotate placements",
    )
    args = parser.parse_args()
    try:
        textures = None
        if args.input.suffix.lower() == ".lb":
            if args.field is not None or args.texture_bundle is not None:
                raise fld.FldError("an LB input already supplies its FLD2 and texture bundle")
            archive = lb.parse_archive(args.input.read_bytes())
            models = [entry for entry in archive.entries if entry.extension.upper() == "F1"]
            fields = [entry for entry in archive.entries if entry.extension.upper() == "F2"]
            bundles = [entry for entry in archive.entries if entry.extension.upper() == "TBN"]
            if len(models) != 1 or len(fields) != 1 or len(bundles) != 1:
                raise fld.FldError(
                    "LB scene export requires exactly one F1, one F2, and one TBN entry"
                )
            model_data = lb.entry_data(models[0])
            field_data = lb.entry_data(fields[0])
            textures = tmx.parse_bundle(lb.entry_data(bundles[0]))
        else:
            if args.field is None:
                raise fld.FldError("a loose FLD1 input requires --field")
            model_data = _source_or_binary(args.input, ".f1asm")
            field_data = _source_or_binary(args.field, ".fldasm")
            if args.texture_bundle is not None:
                textures = tmx.parse_bundle(args.texture_bundle.read_bytes())
        identity = _field_identity(args.input, args.field)
        if args.warps is not None and identity is None:
            raise fld.FldError(
                "WAP scene metadata requires an fNNN_AAA input or --field filename"
            )
        if args.warps is not None:
            warp_match = re.fullmatch(r"f(\d{3})", args.warps.stem, re.IGNORECASE)
            if warp_match and int(warp_match.group(1)) != identity[0]:
                raise fld.FldError("WAP filename does not match the field scene")
        document, binary = build_scene(
            model_data,
            field_data,
            textures=textures,
            resources=set(args.resources) if args.resources else None,
            meters_per_unit=args.meters_per_unit,
            frames_per_second=args.frames_per_second,
            placement_marker_size=args.placement_marker_size,
            warp_data=(
                _warp_source_or_binary(args.warps)
                if args.warps is not None
                else None
            ),
            field_number=identity[0] if identity is not None else None,
            area_number=identity[1] if identity is not None else None,
        )
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_bytes(fld_model.encode_glb(document, binary))
    except (OSError, fld.FldError, lb.LbError, tmx.TmxError, ValueError) as exc:
        parser.error(str(exc))


if __name__ == "__main__":
    main()
