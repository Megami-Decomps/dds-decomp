#!/usr/bin/env python3
"""Import edited FLD1 mesh attributes from a DDS model GLB."""

from __future__ import annotations

import argparse
from pathlib import Path

import fld
from sdf_model_import import (
    ImportSummary,
    ModelGraph,
    ModelImportError,
    decode_glb,
    import_model_graphs,
)


def import_geometry(
    field_data: bytes,
    document: dict,
    binary: bytes,
) -> tuple[bytes, ImportSummary]:
    """Apply changed model vertex attributes while preserving packet structure."""

    fld.validate(field_data)
    if field_data[4:8] != b"FLD1":
        raise ModelImportError("model geometry import requires an FLD1 file")
    nodes = document.get("nodes")
    if not isinstance(nodes, list):
        raise ModelImportError("GLB has no node array")

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

    graphs = []
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
        graphs.append(
            ModelGraph(source_name, items, draw_roots, draw_lists, draws)
        )

    rebuilt, summary = import_model_graphs(
        field_data, document, binary, tuple(graphs)
    )
    fld.validate(rebuilt)
    return rebuilt, summary


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
        f"imported {summary.models} resources and {summary.meshes} meshes; "
        f"changed {summary.changed_meshes} meshes "
        f"({summary.positions} position, {summary.normals} normal, "
        f"{summary.texcoords} texcoord, {summary.attributes} attribute, "
        f"{summary.colors} color streams)"
    )


if __name__ == "__main__":
    main()
