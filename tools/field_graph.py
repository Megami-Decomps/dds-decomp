#!/usr/bin/env python3
"""Export the DDS field-area transition graph from exact WAP source."""

from __future__ import annotations

import argparse
import json
import re
from collections import Counter
from pathlib import Path

import field_world
import wap


class FieldGraphError(ValueError):
    """Raised when a field source directory cannot form a transition graph."""


FIELD_AREA_PATTERN = re.compile(r"f(\d{3})_(\d{3})", re.IGNORECASE)
WAP_PATTERN = re.compile(r"f(\d{3})", re.IGNORECASE)


def _area_id(field_number: int, area_number: int) -> str:
    return f"f{field_number:03}_{area_number:03}"


def build_graph(
    field_sources: set[str],
    tables: dict[int, wap.WapFile],
) -> dict:
    """Return one deterministic graph for field transitions with known targets."""

    known_areas: set[str] = set()
    for stem in field_sources:
        match = FIELD_AREA_PATTERN.fullmatch(stem)
        if match:
            known_areas.add(stem.lower())

    edges = []
    actor_counts: Counter[tuple[str, str]] = Counter()
    nodes = set(known_areas)
    for field_number, table in sorted(tables.items()):
        default = wap.default_entry(table.profile)
        for entry_index, entry in enumerate(table.entries):
            target_area = entry.warp_args[1]
            if (
                entry.kind == 0
                or entry.area <= 0
                or entry.warp_type != 0
                or target_area <= 0
            ):
                continue
            target_field = entry.warp_args[0] or field_number
            source = _area_id(field_number, entry.area)
            target = _area_id(target_field, target_area)
            nodes.update((source, target))
            actor = entry.name.value
            if actor:
                actor_counts[source, actor] += 1
            edges.append(
                {
                    "source": source,
                    "target": target,
                    "sourcePresent": source in known_areas,
                    "targetPresent": target in known_areas,
                    "actor": actor or None,
                    **field_world.transition_metadata(
                        entry, entry_index, field_number, default
                    ),
                }
            )

    node_rows = []
    for node_id in sorted(nodes):
        match = FIELD_AREA_PATTERN.fullmatch(node_id)
        assert match is not None
        node_rows.append(
            {
                "id": node_id,
                "field": int(match.group(1)),
                "area": int(match.group(2)),
                "hasFieldSource": node_id in known_areas,
            }
        )
    return {
        "schema": "dds-field-world-1",
        "summary": {
            "fieldSources": len(known_areas),
            "transitionTables": len(tables),
            "areaNodes": len(node_rows),
            "fieldTransitions": len(edges),
            "sourcePresent": sum(edge["sourcePresent"] for edge in edges),
            "targetPresent": sum(edge["targetPresent"] for edge in edges),
            "conditionalTransitions": sum("gate" in edge for edge in edges),
            "multiTransitionActors": sum(count > 1 for count in actor_counts.values()),
        },
        "areas": node_rows,
        "transitions": edges,
    }


def render_dot(graph: dict) -> str:
    """Render a Graphviz graph while keeping missing field sources visible."""

    lines = [
        "digraph dds_field_world {",
        "  graph [rankdir=LR];",
        '  node [shape=box fontname="sans-serif"];',
        '  edge [fontname="sans-serif" fontsize=9];',
    ]
    for area in graph["areas"]:
        attributes = [f"label={json.dumps(area['id'])}"]
        if not area["hasFieldSource"]:
            attributes.extend(('style="dashed"', 'color="gray"'))
        lines.append(f"  {json.dumps(area['id'])} [{', '.join(attributes)}];")
    for edge in graph["transitions"]:
        label = edge["actor"] or f"entry {edge['entry']}"
        label = f"{label} [{edge['entry']}]"
        if "gate" in edge:
            gate = edge["gate"]
            label += f" flag {gate['flag']} mode {gate['mode']}"
        attributes = [f"label={json.dumps(label)}"]
        if not edge["targetPresent"]:
            attributes.extend(('style="dashed"', 'color="gray"'))
        lines.append(
            f"  {json.dumps(edge['source'])} -> {json.dumps(edge['target'])} "
            f"[{', '.join(attributes)}];"
        )
    lines.append("}")
    return "\n".join(lines) + "\n"


def _load_graph(field_dir: Path, script_dir: Path) -> dict:
    if not field_dir.is_dir():
        raise FieldGraphError(f"field source directory does not exist: {field_dir}")
    if not script_dir.is_dir():
        raise FieldGraphError(f"field script directory does not exist: {script_dir}")

    field_sources = {path.stem for path in field_dir.glob("*.fldasm")}
    tables: dict[int, wap.WapFile] = {}
    for source in sorted(field_dir.glob("*.wapasm")):
        match = WAP_PATTERN.fullmatch(source.stem)
        if match is None:
            raise FieldGraphError(f"WAP source name is not fNNN: {source.name}")
        field_number = int(match.group(1))
        if field_number in tables:
            raise FieldGraphError(f"duplicate WAP table for field {field_number}")
        references = wap.load_references(
            script_dir / f"f{field_number:03}.bfasm",
            source.with_suffix(".infasm"),
        )
        tables[field_number] = wap.parse_source(
            source.read_text(encoding="utf-8"), references
        )
    if not tables:
        raise FieldGraphError(f"no .wapasm sources found in {field_dir}")
    return build_graph(field_sources, tables)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("field_dir", type=Path)
    parser.add_argument("output", type=Path)
    parser.add_argument(
        "--scripts-dir",
        type=Path,
        help="paired field BF source directory (inferred for the repository layout)",
    )
    parser.add_argument("--format", choices=("json", "dot"), default="json")
    args = parser.parse_args()
    try:
        script_dir = args.scripts_dir or args.field_dir.parent.parent / "scripts/field"
        graph = _load_graph(args.field_dir, script_dir)
        text = (
            json.dumps(graph, indent=2, ensure_ascii=True) + "\n"
            if args.format == "json"
            else render_dot(graph)
        )
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(text, encoding="utf-8")
    except (FieldGraphError, OSError, ValueError, wap.WapError) as exc:
        parser.error(str(exc))
    summary = graph["summary"]
    print(
        f"wrote {summary['areaNodes']} areas and "
        f"{summary['fieldTransitions']} field transitions to {args.output}"
    )


if __name__ == "__main__":
    main()
