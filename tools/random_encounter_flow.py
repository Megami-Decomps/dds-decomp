#!/usr/bin/env python3
"""Join DDS field areas to exact random-encounter table source."""

from __future__ import annotations

import re
from collections import Counter

import battle_tbl
import encounter_flow
import fld


class RandomEncounterFlowError(ValueError):
    """Raised when field and encounter source cannot form an exact graph."""


FIELD_AREA_PATTERN = re.compile(r"f(\d{3})_(\d{3})", re.IGNORECASE)
ROUTE_NAMES = ("abc", "ab", "ac", "bc", "a", "b", "c", "none")


def _zone_id(index: int) -> str:
    return f"encounter-zone:{index}"


def _pool_id(zone: int, pool: int) -> str:
    return f"encounter-zone:{zone}:pool:{pool}"


def _route_candidates(results: tuple[bool, bool, bool]) -> tuple[int, ...]:
    """Return the native most-specific-first route order for three results."""

    a, b, c = results
    candidates = []
    if a and b and c:
        candidates.append(0)
    if a and b:
        candidates.append(1)
    if a and c:
        candidates.append(2)
    if b and c:
        candidates.append(3)
    if a:
        candidates.append(4)
    if b:
        candidates.append(5)
    if c:
        candidates.append(6)
    candidates.append(7)
    return tuple(candidates)


def resolve_pool(
    zone: battle_tbl.Zone,
    pool_count: int,
    results: tuple[bool, bool, bool],
    context: str,
) -> tuple[int, str]:
    """Resolve one condition-result state through the runtime fallback chain."""

    if len(zone.routes) != len(ROUTE_NAMES):
        raise RandomEncounterFlowError(
            f"{context} has {len(zone.routes)} routes, expected eight"
        )
    for route_index in _route_candidates(results):
        value = zone.routes[route_index]
        if value == 8:
            continue
        if not 0 <= value < pool_count:
            raise RandomEncounterFlowError(
                f"{context} route {ROUTE_NAMES[route_index]} selects pool {value}, "
                f"outside the 0..{pool_count - 1} range"
            )
        return value, ROUTE_NAMES[route_index]
    # The native selector initializes its result to pool zero. Route value 8
    # continues down the fallback chain, so an all-sentinel chain ends here.
    return 0, "default"


def _checked_zone(
    table: battle_tbl.EncountTable, value: int, context: str
) -> battle_tbl.Zone:
    if value < 0 or value >= table.profile.zone_count:
        raise RandomEncounterFlowError(
            f"{context} references encounter zone {value}, outside the "
            f"0..{table.profile.zone_count - 1} range"
        )
    zone = table.zones[value]
    if zone == battle_tbl.default_zone(table.profile):
        raise RandomEncounterFlowError(
            f"{context} references empty encounter zone {value}"
        )
    return zone


def _selector_maps(
    table: battle_tbl.EncountTable,
) -> dict[int, battle_tbl.SelectorMap]:
    maps: dict[int, battle_tbl.SelectorMap] = {}
    for selector_map in table.default_maps:
        if selector_map.key == 0:
            continue
        if selector_map.key in maps:
            raise RandomEncounterFlowError(
                f"duplicate encounter default map {selector_map.key}"
            )
        maps[selector_map.key] = selector_map
    return maps


def build_sections(
    fields: tuple[tuple[str, bytes], ...] | list[tuple[str, bytes]],
    table: battle_tbl.EncountTable,
    symbols: battle_tbl.BattleSymbols | None = None,
) -> dict:
    """Build exact area, zone, pool, and weighted-formation graph sections."""

    if symbols is not None and symbols.profile_name != table.profile.name:
        raise RandomEncounterFlowError(
            f"cannot use {symbols.profile_name} battle symbols with "
            f"{table.profile.name} encounters"
        )
    maps = _selector_maps(table)
    selector_rows = []
    zone_edges = []
    collision_faces = 0
    included_zones: set[int] = set()

    for stem, data in sorted(fields):
        match = FIELD_AREA_PATTERN.fullmatch(stem)
        if match is None:
            continue
        field_number, area_number = (int(value) for value in match.groups())
        area_id = stem.lower()
        selector_map = maps.get(field_number)
        if selector_map is not None:
            if area_number >= len(selector_map.entries):
                raise RandomEncounterFlowError(
                    f"{area_id} area {area_number} exceeds the encounter map's "
                    f"{len(selector_map.entries)} entries"
                )
            selector = selector_map.entries[area_number]
            alternatives = []
            selections = [("default", selector.value, None, 0)]
            if selector.flag_a > 0:
                alternatives.append(
                    {
                        "name": "a",
                        "flag": selector.flag_a,
                        "zone": selector.alternate_a,
                        "priority": 1,
                    }
                )
                selections.append(
                    ("flag-a", selector.alternate_a, selector.flag_a, 1)
                )
            if selector.flag_b > 0:
                alternatives.append(
                    {
                        "name": "b",
                        "flag": selector.flag_b,
                        "zone": selector.alternate_b,
                        "priority": 2,
                    }
                )
                selections.append(
                    ("flag-b", selector.alternate_b, selector.flag_b, 2)
                )
            selector_rows.append(
                {
                    "id": f"{area_id}:encounter-selector",
                    "area": area_id,
                    "field": field_number,
                    "areaIndex": area_number,
                    "defaultZone": selector.value,
                    "alternatives": alternatives,
                }
            )
            for kind, zone_index, flag, priority in selections:
                _checked_zone(table, zone_index, f"{area_id} {kind}")
                included_zones.add(zone_index)
                edge = {
                    "source": area_id,
                    "target": _zone_id(zone_index),
                    "type": kind,
                    "priority": priority,
                }
                if flag is not None:
                    edge["flag"] = flag
                zone_edges.append(edge)

        overrides = Counter(fld.encounter_zone_overrides(data))
        for zone_index, face_count in sorted(overrides.items()):
            _checked_zone(table, zone_index, f"{area_id} collision face")
            included_zones.add(zone_index)
            collision_faces += face_count
            zone_edges.append(
                {
                    "source": area_id,
                    "target": _zone_id(zone_index),
                    "type": "collision-override",
                    "priority": 3,
                    "faceCount": face_count,
                }
            )

    zone_nodes = []
    route_edges = []
    pool_nodes = []
    slot_edges = []
    encounter_indices: set[int] = set()
    selectable_encounters: set[int] = set()
    condition_reachable_pools: set[str] = set()
    for zone_index in sorted(included_zones):
        zone = _checked_zone(table, zone_index, f"encounter zone {zone_index}")
        zone_nodes.append(
            {
                "id": _zone_id(zone_index),
                "index": zone_index,
                "backgrounds": [zone.background_a, zone.background_b],
                "bgm": zone.bgm,
                "unknown06": zone.unknown_06,
                "conditions": [
                    {"index": index, "kind": kind, "value": value}
                    for index, (kind, value) in enumerate(zone.conditions)
                ],
                "routes": [
                    {
                        "name": name,
                        "value": value,
                        "fallsThrough": value == 8,
                    }
                    for name, value in zip(ROUTE_NAMES, zone.routes)
                ],
            }
        )
        for bits in range(8):
            results = (bool(bits & 4), bool(bits & 2), bool(bits & 1))
            pool_index, selected_by = resolve_pool(
                zone,
                table.profile.pool_count,
                results,
                f"encounter zone {zone_index}",
            )
            target = _pool_id(zone_index, pool_index)
            condition_reachable_pools.add(target)
            route_edges.append(
                {
                    "source": _zone_id(zone_index),
                    "target": target,
                    "type": "condition-route",
                    "results": list(results),
                    "truth": "".join(
                        name for name, result in zip("abc", results) if result
                    ) or "none",
                    "selectedBy": selected_by,
                }
            )

        for pool_index, pool in enumerate(zone.pools):
            pool_id = _pool_id(zone_index, pool_index)
            populated = [slot for slot in pool.slots if slot.encounter != 0]
            pool_nodes.append(
                {
                    "id": pool_id,
                    "zone": zone_index,
                    "index": pool_index,
                    "conditionReachable": pool_id in condition_reachable_pools,
                    "threshold": pool.threshold,
                    "unknown02": pool.unknown_02,
                    "populatedSlots": len(populated),
                    "totalWeight": sum(slot.weight for slot in populated),
                }
            )
            for slot_index, slot in enumerate(pool.slots):
                if slot.encounter == 0:
                    continue
                if not 0 <= slot.encounter < len(table.encounters):
                    raise RandomEncounterFlowError(
                        f"zone {zone_index} pool {pool_index} slot {slot_index} "
                        f"references encounter {slot.encounter}, outside the "
                        f"0..{len(table.encounters) - 1} range"
                    )
                encounter_indices.add(slot.encounter)
                selectable = (
                    pool_id in condition_reachable_pools
                    and pool.threshold > 0
                    and slot.weight > 0
                )
                if selectable:
                    selectable_encounters.add(slot.encounter)
                slot_edges.append(
                    {
                        "source": pool_id,
                        "target": f"encounter:{slot.encounter}",
                        "type": "weighted-encounter",
                        "slot": slot_index,
                        "weight": slot.weight,
                        "modifier": slot.modifier,
                        "nextRoll": slot.next_roll,
                        "conditionReachable": pool_id in condition_reachable_pools,
                        "selectable": selectable,
                    }
                )

    enemy_names = symbols.enemies.by_value if symbols is not None else {}
    encounter_nodes = []
    for index in sorted(encounter_indices):
        node = encounter_flow.encounter_node(
            index, table.encounters[index], enemy_names, set(), set()
        )
        node["availableAsRandomEncounter"] = index in selectable_encounters
        encounter_nodes.append(node)

    return {
        "areaEncounterSelectors": selector_rows,
        "areaEncounterZoneEdges": zone_edges,
        "encounterZoneNodes": zone_nodes,
        "encounterZoneRouteEdges": route_edges,
        "encounterPoolNodes": pool_nodes,
        "encounterPoolSlotEdges": slot_edges,
        "encounterNodes": encounter_nodes,
        "randomEncounterSummary": {
            "randomEncounterAreas": len(selector_rows),
            "defaultZoneEdges": sum(edge["type"] == "default" for edge in zone_edges),
            "flagZoneEdges": sum(edge["type"].startswith("flag-") for edge in zone_edges),
            "collisionOverrideEdges": sum(
                edge["type"] == "collision-override" for edge in zone_edges
            ),
            "collisionOverrideFaces": collision_faces,
            "encounterZoneNodes": len(zone_nodes),
            "conditionRouteEdges": len(route_edges),
            "encounterPoolNodes": len(pool_nodes),
            "conditionReachablePools": len(condition_reachable_pools),
            "weightedEncounterEdges": len(slot_edges),
            "selectableWeightedEncounterEdges": sum(
                edge["selectable"] for edge in slot_edges
            ),
            "randomEncounterNodes": len(encounter_nodes),
            "selectableRandomEncounterNodes": len(selectable_encounters),
        },
    }
