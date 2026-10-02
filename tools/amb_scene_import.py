#!/usr/bin/env python3
"""Import edited DDS automap model geometry from an AMB scene GLB."""

from __future__ import annotations

import argparse
from pathlib import Path

import amb
from sdf_model_import import (
    ImportSummary,
    ModelGraph,
    ModelImportError,
    decode_glb,
    import_model_graphs,
)


def import_geometry(
    automap_data: bytes,
    document: dict,
    binary: bytes,
) -> tuple[bytes, ImportSummary]:
    """Apply changed model streams while preserving the AMB object graph."""

    model = amb.decode(automap_data)
    nodes = document.get("nodes")
    if not isinstance(nodes, list):
        raise ModelImportError("GLB has no node array")

    selected: dict[int, str] = {}
    for node in nodes:
        if not isinstance(node, dict):
            continue
        extras = node.get("extras")
        if not isinstance(extras, dict) or "ddsSubBlocks" not in extras:
            continue
        area_index = extras.get("ddsAreaIndex")
        name = node.get("name")
        if not isinstance(area_index, int) or not isinstance(name, str):
            raise ModelImportError("automap area wrapper has an invalid identity")
        if area_index in selected:
            raise ModelImportError(f"GLB repeats automap area index {area_index}")
        selected[area_index] = name
    if not selected:
        raise ModelImportError("GLB has no DDS automap-area wrappers")

    graphs = []
    for area_index, gltf_name in selected.items():
        if not 0 <= area_index < len(model.areas):
            raise ModelImportError(
                f"GLB automap area index {area_index} is absent from the AMB"
            )
        area = model.areas[area_index]
        area_name = amb._fixed_string(
            automap_data,
            model.data_end,
            area.name,
            f"area {area_index} name",
        )
        source_name = f"area_{area_name}"
        if gltf_name != source_name:
            raise ModelImportError(
                f"GLB automap area {area_index} is named {gltf_name!r}, "
                f"expected {source_name!r}"
            )
        graph = model.models[area_index]
        graphs.append(
            ModelGraph(
                source_name,
                graph.items,
                graph.draw_roots,
                graph.draw_lists,
                graph.draws,
            )
        )

    rebuilt, summary = import_model_graphs(
        automap_data, document, binary, tuple(graphs)
    )
    amb.validate(rebuilt)
    return rebuilt, summary


def _source_or_binary(path: Path) -> bytes:
    if path.suffix.lower() == ".ambasm":
        return amb.encode(amb.parse_source(path.read_text(encoding="utf-8")))
    return path.read_bytes()


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("scene", type=Path)
    parser.add_argument("input", type=Path, help="AMB binary or .ambasm source")
    parser.add_argument("output", type=Path, help="output AMB binary or .ambasm source")
    args = parser.parse_args()
    try:
        document, binary = decode_glb(args.scene.read_bytes())
        data, summary = import_geometry(
            _source_or_binary(args.input), document, binary
        )
        args.output.parent.mkdir(parents=True, exist_ok=True)
        if args.output.suffix.lower() == ".ambasm":
            args.output.write_text(amb.render_source(data), encoding="utf-8")
        else:
            args.output.write_bytes(data)
    except (ModelImportError, OSError, ValueError) as exc:
        parser.error(str(exc))
    print(
        f"imported {summary.models} areas and {summary.meshes} meshes; "
        f"changed {summary.changed_meshes} meshes "
        f"({summary.positions} position, {summary.normals} normal, "
        f"{summary.texcoords} texcoord, {summary.attributes} attribute, "
        f"{summary.colors} color streams)"
    )


if __name__ == "__main__":
    main()
