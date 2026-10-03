#!/usr/bin/env python3
"""Export a DDS AMB automap as a self-contained glTF 2.0 GLB file."""

from __future__ import annotations

import argparse
import math
import struct
from pathlib import Path

import amb
import fld
import fld_model


def _vec3(data: bytes, offset: int) -> tuple[float, float, float]:
    return struct.unpack_from("<3f", data, offset)


def _scaled_vec3(
    data: bytes,
    offset: int,
    meters_per_unit: float,
    context: str,
) -> list[float]:
    values = _vec3(data, offset)
    if not all(math.isfinite(value) for value in values):
        raise amb.AmbError(f"{context} has non-finite coordinates")
    return [value * meters_per_unit for value in values]


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
    index = len(document["materials"])
    document["materials"].append(material)
    return index


def append_automap_scene(
    document: dict,
    binary: bytes,
    data: bytes,
    *,
    areas: set[str] | None = None,
    area_indices: set[int] | None = None,
    meters_per_unit: float = 1.0,
    icon_marker_size: float = 50.0,
) -> tuple[dict, bytes]:
    """Append selected automap areas to an existing glTF scene."""

    if not math.isfinite(meters_per_unit) or meters_per_unit <= 0.0:
        raise amb.AmbError("meters per unit must be a positive finite number")
    if not math.isfinite(icon_marker_size) or icon_marker_size < 0.0:
        raise amb.AmbError("icon marker size must be finite and nonnegative")
    if areas is not None and area_indices is not None:
        raise amb.AmbError("select automap areas by name or index, not both")

    extras = document["asset"].get("extras", {})
    old_scale = extras.get("ddsMetersPerUnit")
    if old_scale is not None and old_scale != meters_per_unit:
        raise amb.AmbError("automap and existing scene use different unit scales")
    model = amb.decode(data)
    area_names = tuple(
        amb._fixed_string(data, model.data_end, area.name, f"area {index} name")
        for index, area in enumerate(model.areas)
    )
    if areas is not None:
        missing = areas - set(area_names)
        if missing:
            raise amb.AmbError(
                f"automap areas not found: {', '.join(sorted(missing))}"
            )
    if area_indices is not None:
        invalid = sorted(
            index
            for index in area_indices
            if index < 0 or index >= len(model.areas)
        )
        if invalid:
            raise amb.AmbError(
                "automap area indices out of range: "
                + ", ".join(str(index) for index in invalid)
            )

    builder = fld_model.GltfBuilder(document, bytearray(binary))
    extras = document["asset"].setdefault("extras", {})
    if not document["meshes"] and len(document["materials"]) == 1:
        geometry_material = 0
        document["materials"][0].update(
            {
                "name": "AMB geometry",
                "extensions": {"KHR_materials_unlit": {}},
            }
        )
        extensions = document.setdefault("extensionsUsed", [])
        if "KHR_materials_unlit" not in extensions:
            extensions.append("KHR_materials_unlit")
    else:
        geometry_material = _unlit_material(
            document, "AMB geometry", [1.0, 1.0, 1.0, 1.0]
        )
    icon_material = _unlit_material(
        document, "AMB icon", [1.0, 0.15, 0.65, 1.0]
    )
    marker_mesh = None

    exported = 0
    for area_index, area in enumerate(model.areas):
        area_name = area_names[area_index]
        if areas is not None and area_name not in areas:
            continue
        if area_indices is not None and area_index not in area_indices:
            continue
        graph = model.models[area_index]
        try:
            node_indices, roots = fld_model.add_model_graph(
                builder,
                data,
                f"area_{area_name}",
                graph.items,
                graph.assets,
                graph.draw_roots,
                graph.draw_lists,
                graph.draws,
                meters_per_unit=meters_per_unit,
                default_material=geometry_material,
            )
        except fld.FldError as exc:
            raise amb.AmbError(str(exc)) from exc

        children = list(roots)
        subblocks = []
        for sblock_index, sblock in enumerate(model.sblocks[area_index]):
            sblock_name = amb._fixed_string(
                data,
                model.data_end,
                sblock.name,
                f"area {area_name} sub-block {sblock_index} name",
            )
            bounds = []
            for offset in (sblock.bound_min, sblock.bound_max):
                bounds.append(
                    _scaled_vec3(
                        data,
                        offset,
                        meters_per_unit,
                        f"area {area_name} sub-block {sblock_name} bound",
                    )
                    if offset
                    else None
                )
            icons = model.icons[area_index][sblock_index]
            subblocks.append(
                {
                    "name": sblock_name,
                    "node": sblock.node,
                    "floor": sblock.floor,
                    "bounds": bounds,
                    "iconCount": len(icons),
                }
            )
            builder.document["nodes"][node_indices[sblock.node]]["extras"][
                "ddsAutomapSubBlock"
            ] = subblocks[-1]
            for icon_index, icon in enumerate(icons):
                if marker_mesh is None and icon_marker_size > 0.0:
                    marker_mesh = fld_model.add_marker_mesh(
                        builder,
                        "AMB icon marker",
                        icon_material,
                        icon_marker_size * meters_per_unit,
                    )
                node = {
                    "name": f"area_{area_name}/{sblock_name}/icon_{icon_index}",
                    "translation": _scaled_vec3(
                        data,
                        icon.position,
                        meters_per_unit,
                        f"area {area_name} sub-block {sblock_name} icon {icon_index}",
                    ),
                    "extras": {
                        "ddsIconType": icon.type_id,
                        "ddsSubBlock": sblock_name,
                        "ddsFloor": sblock.floor,
                        "ddsModelNode": sblock.node,
                    },
                }
                if marker_mesh is not None:
                    node["mesh"] = marker_mesh
                children.append(len(builder.document["nodes"]))
                builder.document["nodes"].append(node)

        native_area_position = _vec3(data, area.position)
        if not all(math.isfinite(value) for value in native_area_position):
            raise amb.AmbError(f"area {area_name} position has non-finite coordinates")
        area_position = [
            value * meters_per_unit for value in native_area_position
        ]
        matching_roots = [
            item.node_id
            for item in graph.items
            if item.parent < 0 and item.position[:3] == native_area_position
        ]
        wrapper = {
            "name": f"area_{area_name}",
            "children": children,
            "extras": {
                "ddsAreaIndex": area_index,
                "ddsAreaPosition": area_position,
                "ddsAreaRootNode": (
                    matching_roots[0] if len(matching_roots) == 1 else None
                ),
                "ddsSubBlocks": subblocks,
                "ddsAssets": [
                    {
                        "index": asset.index,
                        "flags": asset.flags,
                        **{
                            field_name: list(values)
                            for field_name, values in asset.fields
                        },
                    }
                    for asset in graph.assets
                ],
            },
        }
        wrapper_index = len(builder.document["nodes"])
        builder.document["nodes"].append(wrapper)
        builder.document["scenes"][0]["nodes"].append(wrapper_index)
        exported += 1

    if not exported:
        raise amb.AmbError("AMB contains no selected automap areas")

    builder.binary.extend(bytes((-len(builder.binary)) & 3))
    builder.document["buffers"] = [{"byteLength": len(builder.binary)}]
    extras.setdefault("ddsMetersPerUnit", meters_per_unit)
    extras["ddsNativeAxesPreserved"] = True
    extras["ddsAutomapAreaCount"] = exported
    return builder.document, bytes(builder.binary)


def build_gltf(
    data: bytes,
    *,
    areas: set[str] | None = None,
    area_indices: set[int] | None = None,
    meters_per_unit: float = 1.0,
    icon_marker_size: float = 50.0,
) -> tuple[dict, bytes]:
    """Return a glTF document and binary buffer for selected automap areas."""

    builder = fld_model.GltfBuilder.create()
    builder.document["asset"]["generator"] = "dds-decomp AMB automap exporter"
    document, binary = append_automap_scene(
        builder.document,
        bytes(builder.binary),
        data,
        areas=areas,
        area_indices=area_indices,
        meters_per_unit=meters_per_unit,
        icon_marker_size=icon_marker_size,
    )
    document["asset"]["extras"]["ddsAreaCount"] = document["asset"]["extras"][
        "ddsAutomapAreaCount"
    ]
    return document, binary


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("input", type=Path)
    parser.add_argument("output", type=Path)
    parser.add_argument(
        "--area",
        action="append",
        dest="areas",
        help="export only this AMB area name (repeatable)",
    )
    parser.add_argument(
        "--area-index",
        action="append",
        dest="area_indices",
        type=int,
        help="export only this zero-based AMB area index (repeatable)",
    )
    parser.add_argument("--meters-per-unit", type=float, default=1.0)
    parser.add_argument("--icon-marker-size", type=float, default=50.0)
    args = parser.parse_args()
    try:
        if args.input.suffix.lower() == ".ambasm":
            data = amb.encode(amb.parse_source(args.input.read_text(encoding="utf-8")))
        else:
            data = args.input.read_bytes()
        document, binary = build_gltf(
            data,
            areas=set(args.areas) if args.areas else None,
            area_indices=set(args.area_indices) if args.area_indices else None,
            meters_per_unit=args.meters_per_unit,
            icon_marker_size=args.icon_marker_size,
        )
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_bytes(fld_model.encode_glb(document, binary))
    except (OSError, ValueError, amb.AmbError, fld.FldError) as exc:
        parser.error(str(exc))


if __name__ == "__main__":
    main()
