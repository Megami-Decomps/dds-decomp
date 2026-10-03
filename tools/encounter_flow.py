#!/usr/bin/env python3
"""Join script encounter requests to exact DDS battle-table source."""

from __future__ import annotations

from collections import deque
from pathlib import Path

import battle_tbl


class EncounterFlowError(ValueError):
    """Raised when a script request cannot form an exact encounter graph."""


def load_sources(
    directory: Path,
) -> tuple[battle_tbl.EncountTable, battle_tbl.BattleSymbols]:
    """Load one game's maintained encounter table and battle-ID symbols."""

    message_path = directory / "msg.tblasm"
    skill_path = directory / "skill.tblasm"
    encounter_path = directory / "encount.tblasm"
    try:
        message = battle_tbl.parse_message_source(
            message_path.read_text(encoding="utf-8"), directory
        )
        skill = battle_tbl.parse_skill_source(
            skill_path.read_text(encoding="utf-8")
        )
        symbols = battle_tbl.battle_symbols_from_message(message, skill)
        encounters = battle_tbl.parse_encount_source(
            encounter_path.read_text(encoding="utf-8"), symbols
        )
    except (OSError, battle_tbl.BattleTableError) as exc:
        raise EncounterFlowError(
            f"cannot load battle source from {directory}: {exc}"
        ) from exc
    return encounters, symbols


def _checked_index(table: battle_tbl.EncountTable, value: int, context: str) -> int:
    if not isinstance(value, int) or isinstance(value, bool):
        raise EncounterFlowError(f"{context} is not an integer: {value!r}")
    if value < 0 or value >= len(table.encounters):
        raise EncounterFlowError(
            f"{context} {value} is outside the encounter table's "
            f"0..{len(table.encounters) - 1} range"
        )
    return value


def encounter_node(
    index: int,
    encounter: battle_tbl.Encounter,
    enemy_names: dict[int, str],
    requested: set[int],
    reachable: set[int],
) -> dict:
    """Describe one exact encounter formation with shared graph identity."""

    slots = [
        None if enemy == 0 else {"id": enemy, "name": enemy_names.get(enemy)}
        for enemy in encounter.enemies
    ]
    return {
        "id": f"encounter:{index}",
        "index": index,
        "requested": index in requested,
        "reachableFromPlacement": index in reachable,
        "voiceGroup": encounter.voice_group,
        "startItem": encounter.start_item,
        "startItemCount": encounter.start_item_count,
        "unknown03": encounter.unknown_03,
        "nextEncounter": encounter.next_encounter,
        "enemySlots": slots,
        "backgrounds": [encounter.background_a, encounter.background_b],
        "flags": encounter.flags,
        "bgm": encounter.bgm,
        "event": encounter.event,
    }


def mark_reachable(sections: dict, reachable: set[str]) -> None:
    """Apply a combined script/encounter closure to one section bundle."""

    for node in sections["encounterNodes"]:
        node["reachableFromPlacement"] = node["id"] in reachable
    for edge in sections["encounterRequestEdges"]:
        edge["sourceReachable"] = edge["source"] in reachable
    for key in ("encounterChainEdges", "encounterEventEdges"):
        for edge in sections[key]:
            edge["sourceReachable"] = edge["source"] in reachable
    summary = sections["encounterSummary"]
    summary["reachableEncounters"] = sum(
        node["reachableFromPlacement"] for node in sections["encounterNodes"]
    )
    summary["reachableEncounterEventEdges"] = sum(
        edge["sourceReachable"] for edge in sections["encounterEventEdges"]
    )


def build_sections(
    requests: list[dict] | tuple[dict, ...],
    table: battle_tbl.EncountTable,
    event_scripts: set[str] | frozenset[str],
    symbols: battle_tbl.BattleSymbols | None = None,
) -> dict:
    """Resolve literal script requests, encounter chains, and battle events."""

    if symbols is not None and symbols.profile_name != table.profile.name:
        raise EncounterFlowError(
            f"cannot use {symbols.profile_name} battle symbols with "
            f"{table.profile.name} encounters"
        )
    enemy_names = symbols.enemies.by_value if symbols is not None else {}

    request_edges = []
    requested: set[int] = set()
    reachable_starts: set[int] = set()
    for edge in requests:
        if "requestId" not in edge:
            raise EncounterFlowError("encounter request has no requestId")
        index = _checked_index(table, edge["requestId"], "encounter request")
        requested.add(index)
        if edge.get("sourceReachable", False):
            reachable_starts.add(index)
        request_edges.append(
            {
                **edge,
                "target": f"encounter:{index}",
                "targetPresent": True,
            }
        )

    included = set(requested)
    pending = deque(sorted(requested))
    while pending:
        index = pending.popleft()
        next_index = table.encounters[index].next_encounter
        if next_index == 0:
            continue
        _checked_index(table, next_index, f"encounter {index} next target")
        if next_index not in included:
            included.add(next_index)
            pending.append(next_index)

    reachable = set(reachable_starts)
    pending = deque(sorted(reachable_starts))
    while pending:
        index = pending.popleft()
        next_index = table.encounters[index].next_encounter
        if next_index != 0 and next_index not in reachable:
            reachable.add(next_index)
            pending.append(next_index)

    nodes = [
        encounter_node(
            index, table.encounters[index], enemy_names, requested, reachable
        )
        for index in sorted(included)
    ]
    chain_edges = []
    event_edges = []
    for index in sorted(included):
        encounter = table.encounters[index]
        if encounter.next_encounter != 0:
            chain_edges.append(
                {
                    "source": f"encounter:{index}",
                    "target": f"encounter:{encounter.next_encounter}",
                    "type": "next-encounter",
                    "sourceReachable": index in reachable,
                }
            )
        if encounter.event != 0:
            event_script = f"e{encounter.event:03d}"
            target_present = event_script in event_scripts
            event_edges.append(
                {
                    "source": f"encounter:{index}",
                    "target": (
                        f"{event_script}:procedure:0"
                        if target_present
                        else f"event:{event_script}"
                    ),
                    "type": "encounter-event",
                    "eventId": encounter.event,
                    "targetScript": event_script,
                    "targetProcedure": 0 if target_present else None,
                    "targetPresent": target_present,
                    "sourceReachable": index in reachable,
                }
            )

    sections = {
        "encounterNodes": nodes,
        "encounterRequestEdges": request_edges,
        "encounterChainEdges": chain_edges,
        "encounterEventEdges": event_edges,
        "encounterSummary": {
            "encounterRequestEdges": len(request_edges),
            "encounterRequestSites": sum(edge.get("count", 1) for edge in requests),
            "requestedEncounters": len(requested),
            "encounterNodes": len(nodes),
            "chainOnlyEncounters": len(included - requested),
            "reachableEncounters": len(reachable),
            "encounterChainEdges": len(chain_edges),
            "encounterEventEdges": len(event_edges),
            "resolvedEncounterEventEdges": sum(
                edge["targetPresent"] for edge in event_edges
            ),
            "reachableEncounterEventEdges": sum(
                edge["sourceReachable"] for edge in event_edges
            ),
        },
    }
    local_reachable = {
        edge["source"] for edge in request_edges if edge.get("sourceReachable", False)
    }
    local_reachable.update(f"encounter:{index}" for index in reachable)
    mark_reachable(sections, local_reachable)
    return sections
