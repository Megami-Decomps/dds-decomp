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


def build_gltf(
    data: bytes,
    *,
    areas: set[str] | None = None,
    meters_per_unit: float = 1.0,
    icon_marker_size: float = 50.0,
) -> tuple[dict, bytes]:
    """Return a glTF document and binary buffer for selected automap areas."""

    if not math.isfinite(meters_per_unit) or meters_per_unit <= 0.0:
        raise amb.AmbError("meters per unit must be a positive finite number")
    if not math.isfinite(icon_marker_size) or icon_marker_size < 0.0:
        raise amb.AmbError("icon marker size must be finite and nonnegative")

    model = amb.decode(data)
    builder = fld_model.GltfBuilder.create()
    builder.document["asset"]["generator"] = "dds-decomp AMB automap exporter"
    builder.document["extensionsUsed"] = ["KHR_materials_unlit"]
    builder.document["materials"][0].update(
        {
            "name": "AMB geometry",
            "extensions": {"KHR_materials_unlit": {}},
        }
    )
    builder.document["materials"].append(
        {
            "name": "AMB icon",
            "doubleSided": True,
            "pbrMetallicRoughness": {
                "baseColorFactor": [1.0, 0.15, 0.65, 1.0],
                "metallicFactor": 0.0,
                "roughnessFactor": 1.0,
            },
            "extensions": {"KHR_materials_unlit": {}},
        }
    )
    marker_mesh = None

    found: set[str] = set()
    exported = 0
    for area_index, area in enumerate(model.areas):
        area_name = amb._fixed_string(
            data, model.data_end, area.name, f"area {area_index} name"
        )
        if areas is not None and area_name not in areas:
            continue
        found.add(area_name)
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
                        1,
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

    if areas is not None:
        missing = areas - found
        if missing:
            raise amb.AmbError(
                f"automap areas not found: {', '.join(sorted(missing))}"
            )
    if not exported:
        raise amb.AmbError("AMB contains no selected automap areas")

    builder.binary.extend(bytes((-len(builder.binary)) & 3))
    builder.document["buffers"] = [{"byteLength": len(builder.binary)}]
    builder.document["asset"]["extras"] = {
        "ddsMetersPerUnit": meters_per_unit,
        "ddsNativeAxesPreserved": True,
        "ddsAreaCount": exported,
    }
    return builder.document, bytes(builder.binary)


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
            meters_per_unit=args.meters_per_unit,
            icon_marker_size=args.icon_marker_size,
        )
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_bytes(fld_model.encode_glb(document, binary))
    except (OSError, ValueError, amb.AmbError, fld.FldError) as exc:
        parser.error(str(exc))


if __name__ == "__main__":
    main()
