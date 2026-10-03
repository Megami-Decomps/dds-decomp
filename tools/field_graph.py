#!/usr/bin/env python3
"""Export the DDS field-world and interaction graphs from exact source."""

from __future__ import annotations

import argparse
import json
import re
from collections import Counter
from pathlib import Path

import fld
import field_world
import inf
import wap


class FieldGraphError(ValueError):
    """Raised when a field source directory cannot form a transition graph."""


FIELD_AREA_PATTERN = re.compile(r"f(\d{3})_(\d{3})", re.IGNORECASE)
WAP_PATTERN = re.compile(r"f(\d{3})", re.IGNORECASE)


def _area_id(field_number: int, area_number: int) -> str:
    return f"f{field_number:03}_{area_number:03}"


def _target_node_id(set_id: str, target: dict) -> str:
    """Return the stable graph-node identity for one encoded INF target."""

    target_type = target["type"]
    if target_type == "row":
        return f"{set_id}:row:{target['row']}"
    if target_type == "control":
        return f"{set_id}:control:{target['value']}"
    return f"{set_id}:{target_type}"


def _record_target_node(nodes: dict[str, dict], set_id: str, target: dict) -> str:
    """Record a terminal target node and return any target's stable identity."""

    node_id = _target_node_id(set_id, target)
    if target["type"] != "row":
        nodes.setdefault(
            node_id,
            {
                "id": node_id,
                "set": set_id,
                "type": target["type"],
                "value": target["value"],
            },
        )
    return node_id


def _interaction_sections(
    known_areas: set[str],
    placements: dict[str, Counter[str]],
    interaction_tables: dict[int, inf.InfFile],
    transition_tables: dict[int, wap.WapFile],
    message_symbols: dict[int, tuple[str | None, ...]],
) -> dict:
    """Build exact INF state edges and their same-actor WAP handoffs."""

    sets = []
    nodes: dict[str, dict] = {}
    edges = []
    handoffs = []
    linked_warp_sets = 0

    for field_number, table in sorted(interaction_tables.items()):
        transitions = transition_tables.get(field_number)
        transitions_by_area: dict[int, dict[str, tuple[dict, ...]]] = {}
        for set_index, interaction in enumerate(table.sets):
            if interaction == inf.DEFAULT_SET:
                continue
            metadata = field_world.interaction_metadata(
                table,
                interaction,
                set_index,
                message_symbols.get(field_number, ()),
            )
            area = _area_id(field_number, interaction.start.action)
            set_id = f"{area}:set:{set_index}"
            actor = interaction.start.event
            placement_matches = placements.get(area, Counter())[actor]
            if placement_matches > 1:
                raise FieldGraphError(
                    f"{area} interaction actor {actor!r} matches "
                    f"{placement_matches} placements"
                )

            flag_nodes = []
            row_nodes = []
            for selector in metadata["flagSelectors"]:
                node_id = f"{set_id}:flag:{selector['row']}"
                flag_nodes.append(node_id)
                nodes[node_id] = {
                    "id": node_id,
                    "set": set_id,
                    "type": "flag",
                    "row": selector["row"],
                    "flag": selector["flag"],
                }
                for state, target in (("off", selector["off"]), ("on", selector["on"])):
                    target_id = _record_target_node(nodes, set_id, target)
                    edges.append(
                        {
                            "source": node_id,
                            "target": target_id,
                            "type": "flag",
                            "state": state,
                        }
                    )

            for row in metadata["rows"]:
                node_id = f"{set_id}:row:{row['row']}"
                row_nodes.append(node_id)
                nodes[node_id] = {
                    "id": node_id,
                    "set": set_id,
                    "type": "row",
                    **{key: value for key, value in row.items() if key != "choices"},
                }
                for choice, target in enumerate(row["choices"]):
                    target_id = _record_target_node(nodes, set_id, target)
                    edges.append(
                        {
                            "source": node_id,
                            "target": target_id,
                            "type": "choice",
                            "choice": choice,
                        }
                    )

            warp_id = f"{set_id}:warp"
            warp_rows: tuple[dict, ...] = ()
            if warp_id in nodes and transitions is not None:
                area_number = interaction.start.action
                if area_number not in transitions_by_area:
                    transitions_by_area[area_number] = field_world.area_transitions(
                        transitions, field_number, area_number
                    )
                warp_rows = transitions_by_area[area_number].get(actor, ())
                if warp_rows:
                    linked_warp_sets += 1
                for transition in warp_rows:
                    destination = transition["destination"]
                    target = None
                    if destination["type"] == "field" and destination.get("area", 0) > 0:
                        target = _area_id(destination["field"], destination["area"])
                    handoffs.append(
                        {
                            "source": warp_id,
                            "target": target,
                            "targetPresent": (
                                target in known_areas if target is not None else None
                            ),
                            "area": area,
                            "actor": actor,
                            "set": set_id,
                            **transition,
                        }
                    )

            sets.append(
                {
                    "id": set_id,
                    "field": field_number,
                    "area": area,
                    "set": set_index,
                    "kindId": metadata["kindId"],
                    "kind": metadata["kind"],
                    "startArea": metadata["area"],
                    "action": metadata["action"],
                    "eventHit": metadata["eventHit"],
                    "actor": actor,
                    "placementPresent": placement_matches == 1,
                    "flagNodes": flag_nodes,
                    "rowNodes": row_nodes,
                    "warpTransitions": len(warp_rows),
                }
            )

    return {
        "interactionSets": sets,
        "interactionNodes": list(nodes.values()),
        "interactionEdges": edges,
        "warpHandoffs": handoffs,
        "interactionSummary": {
            "interactionTables": len(interaction_tables),
            "interactionSets": len(sets),
            "linkedInteractionSets": sum(row["placementPresent"] for row in sets),
            "flagSelectors": sum(
                node["type"] == "flag" for node in nodes.values()
            ),
            "stateRows": sum(node["type"] == "row" for node in nodes.values()),
            "stateEdges": len(edges),
            "warpSets": sum(f"{row['id']}:warp" in nodes for row in sets),
            "linkedWarpSets": linked_warp_sets,
            "warpHandoffs": len(handoffs),
            "fieldWarpHandoffs": sum(row["target"] is not None for row in handoffs),
        },
    }


def build_graph(
    field_sources: set[str],
    tables: dict[int, wap.WapFile],
    placements: dict[str, Counter[str]] | None = None,
    interaction_tables: dict[int, inf.InfFile] | None = None,
    message_symbols: dict[int, tuple[str | None, ...]] | None = None,
) -> dict:
    """Return a deterministic graph for field transitions and INF state flow."""

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
    graph = {
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
    if interaction_tables is not None:
        sections = _interaction_sections(
            known_areas,
            placements or {},
            interaction_tables,
            tables,
            message_symbols or {},
        )
        graph["schema"] = "dds-field-world-2"
        graph["summary"].update(sections.pop("interactionSummary"))
        graph.update(sections)
    return graph


def _render_interaction_dot(graph: dict, area_id: str) -> str:
    """Render the interaction state machines for one ordinary field area."""

    interaction_sets = [
        row for row in graph.get("interactionSets", ()) if row["area"] == area_id
    ]
    if not interaction_sets:
        raise FieldGraphError(f"no interaction sets found for {area_id}")
    set_ids = {row["id"] for row in interaction_sets}
    nodes = {
        row["id"]: row
        for row in graph["interactionNodes"]
        if row["set"] in set_ids
    }
    edges = [
        row for row in graph["interactionEdges"] if row["source"] in nodes
    ]
    handoffs = [
        row for row in graph["warpHandoffs"] if row["set"] in set_ids
    ]
    linked_warps = {row["source"] for row in handoffs}

    lines = [
        "digraph dds_field_interactions {",
        "  graph [rankdir=LR compound=true];",
        '  node [fontname="sans-serif"];',
        '  edge [fontname="sans-serif" fontsize=9];',
    ]
    for interaction in interaction_sets:
        label = f"{interaction['actor']} | set {interaction['set']} | {interaction['kind']}"
        if not interaction["placementPresent"]:
            label += " | unlinked placement"
        lines.append(f"  subgraph {json.dumps('cluster_' + interaction['id'])} {{")
        lines.append(f"    label={json.dumps(label)};")
        if not interaction["placementPresent"]:
            lines.extend(('    color="gray";', '    style="dashed";'))
        for node_id in (*interaction["flagNodes"], *interaction["rowNodes"]):
            node = nodes[node_id]
            if node["type"] == "flag":
                node_label = f"flag {node['flag']}"
                attributes = [
                    f"label={json.dumps(node_label)}",
                    'shape="diamond"',
                ]
            else:
                message = node.get("messageName", f"message {node['messageId']}")
                node_label = f"row {node['row']} | {node['kind']} | {message}"
                attributes = [
                    f"label={json.dumps(node_label)}",
                    'shape="box"',
                ]
            lines.append(f"    {json.dumps(node_id)} [{', '.join(attributes)}];")
        terminal_ids = sorted(
            node_id
            for node_id, node in nodes.items()
            if node["set"] == interaction["id"] and node["type"] not in {"flag", "row"}
        )
        for node_id in terminal_ids:
            node = nodes[node_id]
            label = node["type"]
            attributes = [f"label={json.dumps(label)}", 'shape="ellipse"']
            if node["type"] == "control":
                label = f"control {node['value']}"
                attributes[0] = f"label={json.dumps(label)}"
                attributes.extend(('style="dashed"', 'color="gray"'))
            elif node["type"] == "warp" and node_id not in linked_warps:
                attributes.extend(('style="dashed"', 'color="gray"'))
            lines.append(f"    {json.dumps(node_id)} [{', '.join(attributes)}];")
        lines.append("  }")

    for edge in edges:
        label = edge["state"] if edge["type"] == "flag" else f"choice {edge['choice']}"
        lines.append(
            f"  {json.dumps(edge['source'])} -> {json.dumps(edge['target'])} "
            f"[label={json.dumps(label)}];"
        )

    rendered_targets: set[str] = set()
    for handoff in handoffs:
        target = handoff["target"]
        if target is None:
            target = f"{handoff['source']}:wap:{handoff['entry']}"
            destination = handoff["destination"]
            destination_label = (
                f"{destination['type']} {','.join(str(value) for value in destination['arguments'])}"
            )
            lines.append(
                f"  {json.dumps(target)} "
                f"[label={json.dumps(destination_label)}, shape=ellipse];"
            )
        elif target not in rendered_targets:
            attributes = [f"label={json.dumps(target)}", 'shape="box"']
            if not handoff["targetPresent"]:
                attributes.extend(('style="dashed"', 'color="gray"'))
            lines.append(f"  {json.dumps(target)} [{', '.join(attributes)}];")
            rendered_targets.add(target)
        label = f"{handoff['kind']} [{handoff['entry']}]"
        if "gate" in handoff:
            gate = handoff["gate"]
            label += f" | flag {gate['flag']} mode {gate['mode']}"
        lines.append(
            f"  {json.dumps(handoff['source'])} -> {json.dumps(target)} "
            f"[label={json.dumps(label)}];"
        )
    lines.append("}")
    return "\n".join(lines) + "\n"


def render_dot(graph: dict, interaction_area: str | None = None) -> str:
    """Render the world graph or one area's INF interaction state machines."""

    if interaction_area is not None:
        return _render_interaction_dot(graph, interaction_area)

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


def _placement_names(source: Path) -> Counter[str]:
    """Read exact type-10 names from canonical FLD2 source."""

    operations = fld.parse_source(source.read_text(encoding="utf-8"))
    string_labels: dict[str, str] = {}
    placement_labels = []
    for index, operation in enumerate(operations):
        if (
            operation.name == "string16"
            and index > 0
            and operations[index - 1].name == "label"
        ):
            string_labels[operations[index - 1].args[0]] = operation.args[0]
        elif operation.name == "resource":
            fields = dict(argument.split("=", 1) for argument in operation.args)
            if fields["type"] == "10" and fields["name"] != "null":
                name = fields["name"]
                if not name.startswith("@"):
                    raise FieldGraphError(
                        f"{source.name}:{operation.line}: placement name is not a label"
                    )
                placement_labels.append((operation.line, name[1:]))

    result: Counter[str] = Counter()
    for line, label in placement_labels:
        if label not in string_labels:
            raise FieldGraphError(
                f"{source.name}:{line}: placement name label {label!r} has no string16"
            )
        result[string_labels[label]] += 1
    return result


def _load_graph(
    field_dir: Path,
    script_dir: Path,
    include_interactions: bool = False,
) -> dict:
    if not field_dir.is_dir():
        raise FieldGraphError(f"field source directory does not exist: {field_dir}")
    if not script_dir.is_dir():
        raise FieldGraphError(f"field script directory does not exist: {script_dir}")

    field_paths = sorted(field_dir.glob("*.fldasm"))
    field_sources = {path.stem for path in field_paths}
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

    if not include_interactions:
        return build_graph(field_sources, tables)

    placements = {
        source.stem.lower(): _placement_names(source)
        for source in field_paths
        if FIELD_AREA_PATTERN.fullmatch(source.stem)
    }
    interaction_tables: dict[int, inf.InfFile] = {}
    message_symbols: dict[int, tuple[str | None, ...]] = {}
    for source in sorted(field_dir.glob("*.infasm")):
        match = WAP_PATTERN.fullmatch(source.stem)
        if match is None:
            raise FieldGraphError(f"INF source name is not fNNN: {source.name}")
        field_number = int(match.group(1))
        if field_number in interaction_tables:
            raise FieldGraphError(f"duplicate INF table for field {field_number}")
        script = script_dir / f"f{field_number:03}.bfasm"
        by_index, by_name = inf.load_message_symbols(script)
        interaction_tables[field_number] = inf.parse_source(
            source.read_text(encoding="utf-8"), by_name
        )
        message_symbols[field_number] = by_index
    if not interaction_tables:
        raise FieldGraphError(f"no .infasm sources found in {field_dir}")
    return build_graph(
        field_sources,
        tables,
        placements,
        interaction_tables,
        message_symbols,
    )


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
    parser.add_argument(
        "--include-interactions",
        action="store_true",
        help="include exact INF state flow and same-actor WAP handoffs",
    )
    parser.add_argument(
        "--interaction-area",
        help="render one fNNN_AAA interaction graph (DOT output only)",
    )
    args = parser.parse_args()
    try:
        interaction_area = None
        if args.interaction_area is not None:
            interaction_area = args.interaction_area.lower()
            if args.format != "dot":
                raise FieldGraphError("--interaction-area requires --format dot")
            if FIELD_AREA_PATTERN.fullmatch(interaction_area) is None:
                raise FieldGraphError("interaction area must have the form fNNN_AAA")
        script_dir = args.scripts_dir or args.field_dir.parent.parent / "scripts/field"
        graph = _load_graph(
            args.field_dir,
            script_dir,
            args.include_interactions or interaction_area is not None,
        )
        text = (
            json.dumps(graph, indent=2, ensure_ascii=True) + "\n"
            if args.format == "json"
            else render_dot(graph, interaction_area)
        )
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(text, encoding="utf-8")
    except (FieldGraphError, OSError, ValueError, wap.WapError) as exc:
        parser.error(str(exc))
    summary = graph["summary"]
    description = (
        f"{summary['areaNodes']} areas and "
        f"{summary['fieldTransitions']} field transitions"
    )
    if "interactionSets" in summary:
        description += (
            f", {summary['interactionSets']} interaction sets, and "
            f"{summary['warpHandoffs']} warp handoffs"
        )
    print(f"wrote {description} to {args.output}")


if __name__ == "__main__":
    main()
