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
import flw0
import inf
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
                "ddsVertexCount": vertex_count,
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
    interactions: dict[str, tuple[dict, ...]] | None = None,
    event_procedures: dict[str, int] | None = None,
) -> tuple[dict, bytes]:
    """Append FLD2 collision, cameras, events, and placements to a glTF document."""

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
    events = [resource for resource in resources if resource.type_id == 6]
    event_labels: list[str | None] = []
    for event_index, resource in enumerate(events):
        label = None
        if resource.data:
            label_pointer = struct.unpack_from("<I", field_data, resource.data + 4)[0]
            if label_pointer:
                label, _ = fld._cstring(
                    field_data,
                    label_pointer,
                    data_end,
                    f"event resource {event_index} label",
                )
        event_labels.append(label)
    builder = fld_model.GltfBuilder(document, bytearray(binary))
    scene_children = []
    collision_material = None
    marker_material = None
    marker_mesh = None
    counts = {"collision": 0, "camera": 0, "event": 0, "placement": 0}
    linked_event_placements = 0
    script_linked_event_resources = 0
    script_linked_event_placements = 0
    transition_actors: set[str] = set()
    interaction_actors: set[str] = set()
    event_ordinal = 0

    for resource in resources:
        if resource.type_id not in {3, 4, 6, 10}:
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
        elif resource.type_id == 6:
            node["extras"]["ddsEventIndex"] = event_ordinal
            if resource.data:
                flags, _, reserved_0, reserved_1 = struct.unpack_from(
                    "<IIII", field_data, resource.data
                )
                node["extras"].update(
                    {
                        "ddsEventFlags": flags,
                        "ddsEventReserved": [reserved_0, reserved_1],
                    }
                )
                label = event_labels[event_ordinal]
                if label is not None:
                    node["extras"]["ddsEventLabel"] = label
                    if event_procedures is not None and label in event_procedures:
                        node["extras"]["ddsEventProcedure"] = {
                            "name": label,
                            "index": event_procedures[label],
                        }
                        script_linked_event_resources += 1
                counts["event"] += 1
            event_ordinal += 1
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
            if kind == 1 and event_index >= 0:
                label = event_labels[event_index]
                if label is not None:
                    node["extras"]["ddsEventLabel"] = label
                    if event_procedures is not None and label in event_procedures:
                        node["extras"]["ddsEventProcedure"] = {
                            "name": label,
                            "index": event_procedures[label],
                        }
                        script_linked_event_placements += 1
                node["extras"]["ddsEventResourceSerial"] = events[event_index].serial
                linked_event_placements += 1
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
            actor_interactions = (interactions or {}).get(name)
            if actor_interactions:
                node["extras"]["ddsInteractions"] = list(actor_interactions)
                interaction_actors.add(name)
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
            "ddsEventResources": counts["event"],
            "ddsPlacementResources": counts["placement"],
            "ddsLinkedEventPlacements": linked_event_placements,
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
    if event_procedures is not None:
        wrapper["extras"].update(
            {
                "ddsFieldScriptProcedures": len(event_procedures),
                "ddsScriptLinkedEventResources": script_linked_event_resources,
                "ddsScriptLinkedEventPlacements": script_linked_event_placements,
            }
        )
    if interactions is not None:
        interaction_sets = sum(len(rows) for rows in interactions.values())
        linked_sets = sum(
            len(rows) for name, rows in interactions.items() if name in interaction_actors
        )
        wrapper["extras"].update(
            {
                "ddsInteractionSets": interaction_sets,
                "ddsLinkedInteractionSets": linked_sets,
            }
        )
        unlinked = [
            {"actor": name, "sets": [row["set"] for row in rows]}
            for name, rows in sorted(interactions.items())
            if name not in interaction_actors
        ]
        if unlinked:
            wrapper["extras"]["ddsUnlinkedInteractionActors"] = unlinked
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
    interaction_data: bytes | None = None,
    message_symbols: tuple[str | None, ...] = (),
    event_procedures: dict[str, int] | None = None,
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
    interactions = None
    if interaction_data is not None:
        if area_number is None:
            raise fld.FldError("INF scene metadata requires an area number")
        interactions = field_world.area_interactions(
            inf.decode(interaction_data), area_number, message_symbols
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
        interactions=interactions,
        event_procedures=event_procedures,
    )
    if (
        warp_data is not None
        or interaction_data is not None
        or event_procedures is not None
    ):
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


def _interaction_source_or_binary(
    path: Path,
) -> tuple[bytes, tuple[str | None, ...]]:
    if path.suffix.lower() != ".infasm":
        return path.read_bytes(), ()
    resolved = path.resolve()
    game_root = resolved.parent.parent.parent
    script_path = game_root / "scripts" / "field" / f"{resolved.stem}.bfasm"
    if not script_path.is_file():
        raise inf.InfError("INF source export requires its paired field script source")
    by_index, by_name = inf.load_message_symbols(script_path)
    table = inf.parse_source(resolved.read_text(encoding="utf-8"), by_name)
    return inf.encode(table), by_index


def _script_procedures(path: Path) -> dict[str, int]:
    """Load the runtime procedure-name lookup table from one BF/FLW0 script."""

    if path.suffix.lower() == ".bfasm":
        script = flw0.parse_source(path.read_text(encoding="utf-8"))
    else:
        script = flw0.parse(path.read_bytes())
    procedures: dict[str, int] = {}
    for row in script.named_rows(0):
        if row.name in procedures:
            raise flw0.Flw0Error(f"duplicate procedure name {row.name!r} in {path}")
        procedures[row.name] = row.row_index
    return procedures


def _paired_script_path(field_number: int, *paths: Path | None) -> Path | None:
    """Find the tracked field script paired with a tracked field-data source."""

    for path in paths:
        if path is None:
            continue
        resolved = path.resolve()
        if (
            resolved.parent.name.lower() != "field"
            or resolved.parent.parent.name.lower() != "data"
        ):
            continue
        candidate = (
            resolved.parent.parent.parent
            / "scripts"
            / "field"
            / f"f{field_number:03}.bfasm"
        )
        if candidate.is_file():
            return candidate
    return None


def _field_identity(*paths: Path | None) -> tuple[int, int] | None:
    for path in paths:
        if path is None:
            continue
        match = re.fullmatch(r"[fk](\d{3})_(\d{3})", path.stem, re.IGNORECASE)
        if match:
            return int(match.group(1)), int(match.group(2))
    return None


def _field_prefix(*paths: Path | None) -> str | None:
    for path in paths:
        if path is None:
            continue
        match = re.fullmatch(r"([fk])\d{3}_\d{3}", path.stem, re.IGNORECASE)
        if match:
            return match.group(1).lower()
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
    parser.add_argument(
        "--interactions",
        type=Path,
        help="INF binary or tracked source whose state machines annotate placements",
    )
    parser.add_argument(
        "--scripts",
        type=Path,
        help="paired BF/FLW0 binary or source whose procedures annotate events",
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
        interaction_path = args.interactions
        if interaction_path is None and args.warps is not None:
            inferred = args.warps.with_suffix(".infasm")
            if (
                _field_prefix(args.input, args.field) == "f"
                and args.warps.suffix.lower() == ".wapasm"
                and inferred.is_file()
            ):
                interaction_path = inferred
        if (
            args.warps is not None
            or interaction_path is not None
            or args.scripts is not None
        ) and identity is None:
            raise fld.FldError(
                "field metadata requires an fNNN_AAA input or --field filename"
            )
        if args.warps is not None:
            warp_match = re.fullmatch(r"f(\d{3})", args.warps.stem, re.IGNORECASE)
            if warp_match and int(warp_match.group(1)) != identity[0]:
                raise fld.FldError("WAP filename does not match the field scene")
        if interaction_path is not None:
            interaction_match = re.fullmatch(
                r"f(\d{3})", interaction_path.stem, re.IGNORECASE
            )
            if interaction_match and int(interaction_match.group(1)) != identity[0]:
                raise fld.FldError("INF filename does not match the field scene")
            interaction_data, message_symbols = _interaction_source_or_binary(
                interaction_path
            )
        else:
            interaction_data, message_symbols = None, ()
        script_path = args.scripts
        if (
            script_path is None
            and identity is not None
            and _field_prefix(args.input, args.field) == "f"
        ):
            script_path = _paired_script_path(identity[0], args.input, args.field)
        if script_path is not None:
            script_match = re.fullmatch(r"f(\d{3})", script_path.stem, re.IGNORECASE)
            if script_match and int(script_match.group(1)) != identity[0]:
                raise fld.FldError("BF filename does not match the field scene")
            event_procedures = _script_procedures(script_path)
        else:
            event_procedures = None
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
            interaction_data=interaction_data,
            message_symbols=message_symbols,
            event_procedures=event_procedures,
            field_number=identity[0] if identity is not None else None,
            area_number=identity[1] if identity is not None else None,
        )
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_bytes(fld_model.encode_glb(document, binary))
    except (OSError, fld.FldError, lb.LbError, tmx.TmxError, ValueError) as exc:
        parser.error(str(exc))


if __name__ == "__main__":
    main()
