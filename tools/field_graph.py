#!/usr/bin/env python3
"""Export the DDS field-world and interaction graphs from exact source."""

from __future__ import annotations

import argparse
import json
import re
import struct
from collections import Counter, defaultdict, deque
from pathlib import Path

import fld
import field_world
import flw0
import flw0_flow
import flw0_profiles
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


def _render_event_dot(graph: dict, field_id: str) -> str:
    """Render placement-rooted procedure flow for one ordinary field script."""

    field_number = int(field_id[1:])
    entries = [
        row
        for row in graph.get("eventEntries", ())
        if row["field"] == field_number
    ]
    procedures = {
        row["id"]: row
        for row in graph.get("scriptProcedures", ())
        if row["field"] == field_number and row["reachableFromPlacement"]
    }
    if not entries:
        raise FieldGraphError(f"no event placements found for {field_id}")
    procedure_edges = [
        row
        for row in graph["scriptProcedureEdges"]
        if row["field"] == field_number and row["source"] in procedures
    ]
    event_edges = [
        row
        for row in graph["eventScriptEdges"]
        if row["field"] == field_number and row["source"] in procedures
    ]
    battle_exit_edges = [
        row
        for row in graph.get("deferredBattleExitEdges", ())
        if row["field"] == field_number and row["source"] in procedures
    ]

    lines = [
        "digraph dds_field_event_flow {",
        "  graph [rankdir=LR];",
        '  node [fontname="sans-serif"];',
        '  edge [fontname="sans-serif" fontsize=9];',
    ]
    for entry in entries:
        label = f"{entry['area']} | {entry['actor'] or 'unnamed'} | {entry['label']}"
        attributes = [f"label={json.dumps(label)}", 'shape="diamond"']
        if entry["procedure"] is None:
            attributes.extend(('style="dashed"', 'color="gray"'))
        lines.append(f"  {json.dumps(entry['id'])} [{', '.join(attributes)}];")
    for procedure in procedures.values():
        calls = sorted(
            procedure["nativeCalls"], key=lambda row: (-row["count"], row["id"])
        )
        command_names = [
            row.get("name", f"COMM_{row['id']:03X}") for row in calls[:3]
        ]
        call_count = sum(row["count"] for row in calls)
        label = (
            f"{procedure['name']} [{procedure['index']}] | "
            f"{call_count} native calls"
        )
        if command_names:
            label += " | " + ", ".join(command_names)
        lines.append(
            f"  {json.dumps(procedure['id'])} "
            f"[label={json.dumps(label)}, shape=box];"
        )
    event_targets = {edge["target"]: edge for edge in event_edges}
    for target, edge in sorted(event_targets.items()):
        label = edge.get("event", f"event {edge['eventId']}")
        attributes = [f"label={json.dumps(label)}", 'shape="ellipse"']
        if not edge["targetPresent"]:
            attributes.extend(('style="dashed"', 'color="gray"'))
        lines.append(f"  {json.dumps(target)} [{', '.join(attributes)}];")
    battle_exit_targets = {}
    for edge in battle_exit_edges:
        target = edge["target"] or (
            f"battle-exit:{edge['lookupField']}:{edge['lookupEvent']}"
        )
        battle_exit_targets[target] = edge
    for target, edge in sorted(battle_exit_targets.items()):
        label = edge["target"] or (
            f"unresolved f{edge['lookupField']:03} battle exit "
            f"{edge['lookupEvent']}"
        )
        attributes = [f"label={json.dumps(label)}", 'shape="ellipse"']
        if not edge["targetPresent"]:
            attributes.extend(('style="dashed"', 'color="gray"'))
        lines.append(f"  {json.dumps(target)} [{', '.join(attributes)}];")
    for entry in entries:
        if entry["procedure"] is not None:
            lines.append(
                f"  {json.dumps(entry['id'])} -> {json.dumps(entry['procedure'])} "
                f"[label={json.dumps('event ' + str(entry['eventIndex']))}];"
            )
    for edge in procedure_edges:
        label = edge["kind"]
        if edge["count"] > 1:
            label += f" x{edge['count']}"
        lines.append(
            f"  {json.dumps(edge['source'])} -> {json.dumps(edge['target'])} "
            f"[label={json.dumps(label)}];"
        )
    for edge in event_edges:
        label = edge["command"]
        if edge["count"] > 1:
            label += f" x{edge['count']}"
        lines.append(
            f"  {json.dumps(edge['source'])} -> {json.dumps(edge['target'])} "
            f"[label={json.dumps(label)}];"
        )
    battle_exit_routes = Counter(
        (
            edge["source"],
            edge["target"]
            or f"battle-exit:{edge['lookupField']}:{edge['lookupEvent']}",
            edge["lookupField"],
            edge["lookupEvent"],
        )
        for edge in battle_exit_edges
    )
    for (source, target, lookup_field, lookup_event), count in sorted(
        battle_exit_routes.items()
    ):
        label = f"DEFER_BATTLE_EXIT f{lookup_field:03}/{lookup_event}"
        if count > 1:
            label += f" x{count}"
        lines.append(
            f"  {json.dumps(source)} -> {json.dumps(target)} "
            f"[label={json.dumps(label)}];"
        )
    lines.append("}")
    return "\n".join(lines) + "\n"


def render_dot(
    graph: dict,
    interaction_area: str | None = None,
    event_field: str | None = None,
) -> str:
    """Render the world graph or one area's INF interaction state machines."""

    if interaction_area is not None:
        return _render_interaction_dot(graph, interaction_area)
    if event_field is not None:
        return _render_event_dot(graph, event_field)

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


def _event_entries(
    source: Path,
    field_number: int,
    area_number: int,
    procedures: dict[str, int],
) -> list[dict]:
    """Read exact kind-1 placement-to-event links from canonical FLD2 source."""

    data = fld.encode(fld.parse_source(source.read_text(encoding="utf-8")))
    words, data_end, _ = fld._read_header(data)
    resources = fld._read_resources(
        data, fld._read_types(data, words, data_end)
    )
    events = [resource for resource in resources if resource.type_id == 6]
    event_labels: list[str | None] = []
    for event_index, resource in enumerate(events):
        label = None
        if resource.data:
            label_pointer = struct.unpack_from("<I", data, resource.data + 4)[0]
            if label_pointer:
                label, _ = fld._cstring(
                    data,
                    label_pointer,
                    data_end,
                    f"event resource {event_index} label",
                )
        event_labels.append(label)

    area = _area_id(field_number, area_number)
    rows = []
    placement_index = 0
    for resource in resources:
        if resource.type_id != 10:
            continue
        current_index = placement_index
        placement_index += 1
        if not resource.data:
            continue
        kind, event_index = struct.unpack_from("<Ii", data, resource.data)
        if kind != 1 or event_index < 0:
            continue
        label = event_labels[event_index]
        name = (
            fld._fixed_string(data, resource.name, "field placement name")
            if resource.name
            else None
        )
        procedure_index = procedures.get(label) if label is not None else None
        rows.append(
            {
                "id": f"{area}:event-placement:{current_index}",
                "field": field_number,
                "area": area,
                "placement": current_index,
                "placementSerial": resource.serial,
                "actor": name,
                "eventIndex": event_index,
                "eventResourceSerial": events[event_index].serial,
                "label": label,
                "procedure": (
                    f"f{field_number:03}:procedure:{procedure_index}"
                    if procedure_index is not None
                    else None
                ),
            }
        )
    return rows


def _battle_exit_candidates(
    table: wap.WapFile | None, field_number: int, event_id: int
) -> list[dict]:
    """Resolve the WAP rows inspected by the deferred battle-exit dispatcher."""

    if table is None:
        return []
    candidates = []
    for entry_index, entry in enumerate(table.entries):
        if entry.kind != 8 or entry.scene_args[0] != event_id:
            continue
        target = None
        if entry.warp_type == 0 and entry.warp_args[1] > 0:
            target = _area_id(
                entry.warp_args[0] or field_number, entry.warp_args[1]
            )
        candidates.append({"entry": entry_index, "target": target})
    return candidates


def _event_sections(
    field_paths: list[Path],
    script_dir: Path,
    profile: flw0_profiles.CommandProfile,
    tables: dict[int, wap.WapFile] | None = None,
) -> dict:
    """Build exact placement roots and procedure-level field-script flow."""

    tables = tables or {}
    known_areas = {path.stem.lower() for path in field_paths}
    scripts: dict[int, dict] = {}
    procedure_names: dict[int, dict[str, int]] = {}
    for source in sorted(script_dir.glob("f???.bfasm")):
        match = re.fullmatch(r"f(\d{3})", source.stem, re.IGNORECASE)
        if match is None:
            continue
        field_number = int(match.group(1))
        if field_number in scripts:
            raise FieldGraphError(f"duplicate field script for f{field_number:03}")
        script = flw0.parse_source(source.read_text(encoding="utf-8"))
        flow = flw0_flow.analyze(script, profile)
        scripts[field_number] = flow
        procedure_names[field_number] = {
            row["name"]: row["index"] for row in flow["procedures"]
        }
    if not scripts:
        raise FieldGraphError(f"no field BF sources found in {script_dir}")

    entries = []
    for source in field_paths:
        match = FIELD_AREA_PATTERN.fullmatch(source.stem)
        if match is None:
            continue
        field_number = int(match.group(1))
        if field_number not in scripts:
            continue
        entries.extend(
            _event_entries(
                source,
                field_number,
                int(match.group(2)),
                procedure_names[field_number],
            )
        )

    entry_counts = Counter(
        row["procedure"] for row in entries if row["procedure"] is not None
    )
    procedures = []
    procedure_edges = []
    event_edges = []
    battle_exit_edges = []
    unresolved = []
    for field_number, flow in sorted(scripts.items()):
        prefix = f"f{field_number:03}:procedure:"
        for row in flow["procedures"]:
            node_id = f"{prefix}{row['index']}"
            procedures.append(
                {
                    "id": node_id,
                    "field": field_number,
                    **row,
                    "entryPlacements": entry_counts[node_id],
                }
            )
        for edge in flow["procedureEdges"]:
            procedure_edges.append(
                {
                    **edge,
                    "field": field_number,
                    "source": f"{prefix}{edge['source']}",
                    "target": f"{prefix}{edge['target']}",
                }
            )
        for edge in flow["eventEdges"]:
            event_name = edge.get("event")
            event_edges.append(
                {
                    **edge,
                    "field": field_number,
                    "source": f"{prefix}{edge['source']}",
                    "target": f"event:{event_name or edge['eventId']}",
                    "targetPresent": (
                        event_name is not None
                        and (
                            script_dir.parent / "event" / f"{event_name}.bfasm"
                        ).is_file()
                    ),
                }
            )
        for site in flow["deferredBattleExits"]:
            candidates = _battle_exit_candidates(
                tables.get(site["field"]), site["field"], site["event"]
            )
            targets = {
                row["target"]
                for row in candidates
                if row["target"] is not None
            }
            target = next(iter(targets)) if len(targets) == 1 else None
            battle_exit_edges.append(
                {
                    "source": f"{prefix}{site['source']}",
                    "field": field_number,
                    "pc": site["pc"],
                    "diagnostic": site["diagnostic"],
                    "lookupField": site["field"],
                    "lookupEvent": site["event"],
                    "candidates": candidates,
                    "target": target,
                    "targetPresent": (
                        target in known_areas if target is not None else False
                    ),
                }
            )
        unresolved.extend(
            {
                **row,
                "field": field_number,
                "source": f"{prefix}{row['source']}",
            }
            for row in flow["unresolvedTargets"]
        )

    adjacency: dict[str, set[str]] = defaultdict(set)
    for edge in procedure_edges:
        adjacency[edge["source"]].add(edge["target"])
    reachable = set(entry_counts)
    pending = deque(sorted(reachable))
    while pending:
        source = pending.popleft()
        for target in sorted(adjacency[source]):
            if target not in reachable:
                reachable.add(target)
                pending.append(target)
    for procedure in procedures:
        procedure["reachableFromPlacement"] = procedure["id"] in reachable
    for edge in procedure_edges:
        edge["sourceReachable"] = edge["source"] in reachable
    for edge in event_edges:
        edge["sourceReachable"] = edge["source"] in reachable
    for edge in battle_exit_edges:
        edge["sourceReachable"] = edge["source"] in reachable

    native_calls = sum(
        call["count"] for procedure in procedures for call in procedure["nativeCalls"]
    )
    named_native_calls = sum(
        call["count"]
        for procedure in procedures
        for call in procedure["nativeCalls"]
        if "name" in call
    )
    reachable_native_calls = sum(
        call["count"]
        for procedure in procedures
        if procedure["reachableFromPlacement"]
        for call in procedure["nativeCalls"]
    )
    return {
        "eventEntries": entries,
        "scriptProcedures": procedures,
        "scriptProcedureEdges": procedure_edges,
        "eventScriptEdges": event_edges,
        "deferredBattleExitEdges": battle_exit_edges,
        "scriptUnresolvedTargets": unresolved,
        "eventSummary": {
            "fieldScripts": len(scripts),
            "eventPlacements": len(entries),
            "linkedEventPlacements": sum(
                row["procedure"] is not None for row in entries
            ),
            "entryProcedures": len(entry_counts),
            "procedureNodes": len(procedures),
            "reachableProcedures": len(reachable),
            "procedureEdges": len(procedure_edges),
            "taskEdges": sum(edge["kind"] == "task" for edge in procedure_edges),
            "taskCalls": sum(
                edge["count"]
                for edge in procedure_edges
                if edge["kind"] == "task"
            ),
            "eventScriptEdges": len(event_edges),
            "reachableEventScriptEdges": sum(
                edge["sourceReachable"] for edge in event_edges
            ),
            "deferredBattleExitSites": len(battle_exit_edges),
            "resolvedDeferredBattleExitSites": sum(
                edge["target"] is not None for edge in battle_exit_edges
            ),
            "reachableDeferredBattleExitSites": sum(
                edge["sourceReachable"] for edge in battle_exit_edges
            ),
            "nativeCalls": native_calls,
            "namedNativeCalls": named_native_calls,
            "reachableNativeCalls": reachable_native_calls,
            "unresolvedScriptTargets": len(unresolved),
        },
    }


def _load_graph(
    field_dir: Path,
    script_dir: Path,
    include_interactions: bool = False,
    include_events: bool = False,
    profile_name: str | None = None,
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
        graph = build_graph(field_sources, tables)
    else:
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
        graph = build_graph(
            field_sources,
            tables,
            placements,
            interaction_tables,
            message_symbols,
        )

    if include_events:
        if profile_name is None:
            raise FieldGraphError("event flow requires a DDS command profile")
        try:
            profile = flw0_profiles.get(profile_name)
        except KeyError as exc:
            raise FieldGraphError(f"unknown command profile {profile_name!r}") from exc
        sections = _event_sections(field_paths, script_dir, profile, tables)
        graph["schema"] = "dds-field-world-4"
        graph["summary"].update(sections.pop("eventSummary"))
        graph.update(sections)
    return graph


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
        "--include-events",
        action="store_true",
        help="include placement-rooted field-script procedure flow",
    )
    parser.add_argument(
        "--profile",
        choices=("dds1", "dds2"),
        help="command profile for event flow (inferred from the repository layout)",
    )
    parser.add_argument(
        "--interaction-area",
        help="render one fNNN_AAA interaction graph (DOT output only)",
    )
    parser.add_argument(
        "--event-field",
        help="render one fNNN placement and procedure graph (DOT output only)",
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
        event_field = None
        if args.event_field is not None:
            event_field = args.event_field.lower()
            if args.format != "dot":
                raise FieldGraphError("--event-field requires --format dot")
            if WAP_PATTERN.fullmatch(event_field) is None:
                raise FieldGraphError("event field must have the form fNNN")
            if interaction_area is not None:
                raise FieldGraphError(
                    "--event-field and --interaction-area are mutually exclusive"
                )
        profile_name = args.profile
        if profile_name is None:
            inferred_profile = script_dir.parent.parent.name.lower()
            if inferred_profile in ("dds1", "dds2"):
                profile_name = inferred_profile
        graph = _load_graph(
            args.field_dir,
            script_dir,
            args.include_interactions or interaction_area is not None,
            args.include_events or event_field is not None,
            profile_name,
        )
        text = (
            json.dumps(graph, indent=2, ensure_ascii=True) + "\n"
            if args.format == "json"
            else render_dot(graph, interaction_area, event_field)
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
    if "eventPlacements" in summary:
        description += (
            f", {summary['linkedEventPlacements']} linked event placements, and "
            f"{summary['reachableProcedures']} reachable procedures"
        )
    print(f"wrote {description} to {args.output}")


if __name__ == "__main__":
    main()
