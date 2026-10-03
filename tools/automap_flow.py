#!/usr/bin/env python3
"""Link DDS field rooms and collision discovery faces to base automap data."""

from __future__ import annotations

import re
import struct

import amb
import fld


AUTOMAP_DISCOVERY_ATTRIBUTE = 0x0800
FIELD_RESOURCE_PATTERN = re.compile(r"([fk])(\d{3})_(\d{3})", re.IGNORECASE)
AUTOMAP_PATTERN = re.compile(r"f(\d{3})", re.IGNORECASE)


class AutomapFlowError(ValueError):
    """Raised when field and automap sources cannot form an exact runtime graph."""


def _vec3(data: bytes, offset: int) -> list[float] | None:
    return list(struct.unpack_from("<3f", data, offset)) if offset else None


def _automap_area_id(field_number: int, area_index: int) -> str:
    return f"automap:f{field_number:03}:area:{area_index}"


def _subblock_id(field_number: int, area_index: int, subblock_index: int) -> str:
    return f"automap:f{field_number:03}:area:{area_index}:subblock:{subblock_index}"


def _collision_faces(data: bytes):
    """Yield each collision resource and decoded face in physical order."""

    words, data_end, _ = fld._read_header(data)
    resources = fld._read_resources(data, fld._read_types(data, words, data_end))
    collision_index = 0
    for resource in resources:
        if resource.type_id != 3 or not resource.data:
            continue
        fld._range(data, resource.data, fld.COLLISION_SIZE, "collision header")
        values = struct.unpack_from("<12I", data, resource.data)
        face_count, faces = values[5], values[8]
        fld._range(data, faces, face_count * fld.FACE_SIZE, "collision faces")
        name = (
            fld._fixed_string(data, resource.name, "collision resource name")
            if resource.name
            else ""
        )
        for face_index in range(face_count):
            yield collision_index, resource, name, face_index, struct.unpack_from(
                "<IBBH HBB 4I hhhh",
                data,
                faces + face_index * fld.FACE_SIZE,
            )
        collision_index += 1


def decode_automaps(
    sources: dict[int, tuple[str, bytes]],
) -> tuple[list[dict], list[dict], dict[int, amb.AmbFile]]:
    """Decode base automap sources into stable area and sub-block nodes."""

    areas: list[dict] = []
    subblocks: list[dict] = []
    models: dict[int, amb.AmbFile] = {}
    for field_number, (source, data) in sorted(sources.items()):
        match = AUTOMAP_PATTERN.fullmatch(source)
        if match is None or int(match.group(1)) != field_number:
            raise AutomapFlowError(
                f"automap source {source!r} does not identify field {field_number}"
            )
        try:
            model = amb.decode(data)
        except amb.AmbError as exc:
            raise AutomapFlowError(f"{source}: {exc}") from exc
        models[field_number] = model
        for area_index, area in enumerate(model.areas):
            area_name = amb._fixed_string(
                data, model.data_end, area.name, f"{source} area {area_index} name"
            )
            area_id = _automap_area_id(field_number, area_index)
            rows = model.sblocks[area_index]
            areas.append(
                {
                    "id": area_id,
                    "source": source,
                    "field": field_number,
                    "index": area_index,
                    "name": area_name,
                    "position": _vec3(data, area.position),
                    "subblockCount": len(rows),
                }
            )
            for subblock_index, row in enumerate(rows):
                subblocks.append(
                    {
                        "id": _subblock_id(
                            field_number, area_index, subblock_index
                        ),
                        "area": area_id,
                        "source": source,
                        "field": field_number,
                        "areaIndex": area_index,
                        "index": subblock_index,
                        "name": amb._fixed_string(
                            data,
                            model.data_end,
                            row.name,
                            f"{source} area {area_index} sub-block {subblock_index} name",
                        ),
                        "modelNode": row.node,
                        "floor": row.floor,
                        "runtimeFloor": row.floor + 1,
                        "iconCount": row.icon_count,
                        "bounds": [_vec3(data, row.bound_min), _vec3(data, row.bound_max)],
                    }
                )
    return areas, subblocks, models


def build_sections(
    fields: tuple[tuple[str, bytes], ...],
    automaps: dict[int, tuple[str, bytes]],
) -> dict:
    """Build the exact field-room, active-map, and discovery-face relations."""

    automap_areas, automap_subblocks, models = decode_automaps(automaps)
    selection_edges: list[dict] = []
    discovery_edges: list[dict] = []
    seen_fields: set[str] = set()

    for source, data in sorted(fields):
        match = FIELD_RESOURCE_PATTERN.fullmatch(source)
        if match is None:
            continue
        resource_kind, field_text, area_text = match.groups()
        field_number, area_number = int(field_text), int(area_text)
        resource_kind = resource_kind.lower()
        area_id = f"{resource_kind}{field_number:03}_{area_number:03}"
        if area_id in seen_fields:
            raise AutomapFlowError(f"duplicate field area {area_id}")
        seen_fields.add(area_id)
        area_index = area_number - 1
        model = models.get(field_number)
        if resource_kind != "f":
            status = "non-field"
            target = None
        elif model is None:
            status = "missing-map"
            target = None
        elif area_index < 0 or area_index >= len(model.areas):
            status = "out-of-range"
            target = None
        else:
            status = "linked"
            target = _automap_area_id(field_number, area_index)
        if resource_kind == "f":
            selection_edges.append(
                {
                    "source": area_id,
                    "target": target,
                    "field": field_number,
                    "room": area_number,
                    "areaIndex": area_index,
                    "status": status,
                }
            )

        try:
            faces = _collision_faces(data)
            for collision_index, resource, resource_name, face_index, face in faces:
                if not face[0] & AUTOMAP_DISCOVERY_ATTRIBUTE:
                    continue
                selector = face[5]
                subblock_index = selector - 1
                edge = {
                    "id": (
                        f"{area_id}:collision:{collision_index}:face:{face_index}:automap"
                    ),
                    "source": area_id,
                    "target": None,
                    "resourceKind": resource_kind,
                    "areaIndex": area_index,
                    "areaStatus": status,
                    "collisionSerial": resource.serial,
                    "collisionIndex": collision_index,
                    "collisionName": resource_name,
                    "face": face_index,
                    "selector": selector,
                    "subblockIndex": subblock_index if selector else None,
                    "upperName": face[6],
                    "runtimeFloor": None,
                }
                if status != "linked":
                    edge["resolution"] = "unresolved-area"
                else:
                    assert model is not None
                    rows = model.sblocks[area_index]
                    if selector and subblock_index < len(rows):
                        row = rows[subblock_index]
                        edge.update(
                            {
                                "target": _subblock_id(
                                    field_number, area_index, subblock_index
                                ),
                                "runtimeFloor": row.floor + 1,
                                "resolution": "subblock",
                            }
                        )
                    else:
                        edge.update(
                            {"runtimeFloor": 1, "resolution": "default-floor"}
                        )
                discovery_edges.append(edge)
        except fld.FldError as exc:
            raise AutomapFlowError(f"{source}: {exc}") from exc

    linked = sum(row["status"] == "linked" for row in selection_edges)
    default_faces = sum(
        row["resolution"] == "default-floor" for row in discovery_edges
    )
    unresolved_faces = sum(
        row["resolution"] == "unresolved-area" for row in discovery_edges
    )
    return {
        "automapAreas": automap_areas,
        "automapSubblocks": automap_subblocks,
        "automapAreaEdges": selection_edges,
        "automapDiscoveryEdges": discovery_edges,
        "automapSummary": {
            "automapSourceMaps": len(models),
            "automapAreas": len(automap_areas),
            "automapSubblocks": len(automap_subblocks),
            "automapFieldAreas": len(selection_edges),
            "linkedAutomapFieldAreas": linked,
            "missingAutomapFieldAreas": sum(
                row["status"] == "missing-map" for row in selection_edges
            ),
            "outOfRangeAutomapFieldAreas": sum(
                row["status"] == "out-of-range" for row in selection_edges
            ),
            "automapDiscoveryFaces": len(discovery_edges),
            "resolvedAutomapDiscoveryFaces": sum(
                row["resolution"] == "subblock" for row in discovery_edges
            ),
            "defaultFloorAutomapDiscoveryFaces": default_faces,
            "unresolvedAutomapDiscoveryFaces": unresolved_faces,
            "nonFieldAutomapDiscoveryFaces": sum(
                row["areaStatus"] == "non-field" for row in discovery_edges
            ),
            "missingMapAutomapDiscoveryFaces": sum(
                row["areaStatus"] == "missing-map" for row in discovery_edges
            ),
            "outOfRangeAutomapDiscoveryFaces": sum(
                row["areaStatus"] == "out-of-range" for row in discovery_edges
            ),
        },
    }
