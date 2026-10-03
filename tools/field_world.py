#!/usr/bin/env python3
"""Validate identities that join DDS field resources to global data."""

from __future__ import annotations

import argparse
import re
import sys
from dataclasses import dataclass
from pathlib import Path

import battle_tbl
import fld
import inf
import wap


class FieldWorldError(ValueError):
    """Raised when independently sourced field-world identities disagree."""


@dataclass(frozen=True)
class EncounterLinkSummary:
    fields: int
    default_areas: int
    default_zone_references: int
    override_faces: int
    zones: int


def _transition_destination(entry: wap.Entry, current_field: int) -> dict:
    """Describe one WAP destination without discarding its physical arguments."""

    first, second, third = entry.warp_args
    destination = {
        "typeId": entry.warp_type,
        "type": wap.WARP_TYPE_NAMES.get(entry.warp_type, "unknown"),
        "arguments": [first, second, third],
        "position": entry.position.value,
        "camera": {
            "name": entry.camera.value,
            "mode": entry.camera_mode,
            "table": entry.camera_table,
        },
    }
    if entry.warp_type == 0:
        destination.update(
            {
                "field": first or current_field,
                "area": second,
            }
        )
    elif entry.warp_type == 1:
        destination.update({"table": first, "floor": second})
    elif entry.warp_type == 2:
        destination.update(
            {
                "actionId": first,
                "action": wap.FACILITY_ACTION_NAMES.get(first, "unknown"),
            }
        )
        names = wap.FACILITY_ARGUMENT_NAMES.get(first)
        if names is not None:
            destination[names[0]] = second
            destination["floorFlag"] = third
    elif entry.warp_type == 3:
        destination.update({"event": first, "alternateField": third})
    return destination


def transition_metadata(
    entry: wap.Entry,
    entry_index: int,
    current_field: int,
    default: wap.Entry,
) -> dict:
    """Return the semantic and exact transition fields useful to consumers."""

    transition = {
        "entry": entry_index,
        "kindId": entry.kind,
        "kind": wap.ENTRY_KIND_NAMES.get(entry.kind, "unknown"),
        "destination": _transition_destination(entry, current_field),
    }
    if entry.flag_mode or entry.flag:
        transition["gate"] = {"mode": entry.flag_mode, "flag": entry.flag}
    if entry.attributes:
        transition["attributes"] = entry.attributes
    after = {}
    if entry.bgm != default.bgm:
        after["bgm"] = entry.bgm
    if entry.footstep != default.footstep:
        after["footstep"] = entry.footstep
    if entry.after_flag:
        after["flags"] = entry.after_flag
    if entry.after_script.value:
        after["script"] = entry.after_script.value
    if after:
        transition["after"] = after
    if entry.tail != default.tail:
        transition["tail"] = {
            "control": entry.tail[0],
            "arguments": list(entry.tail[1:]),
        }
    return transition


def area_transitions(
    table: wap.WapFile,
    current_field: int,
    current_area: int,
) -> dict[str, tuple[dict, ...]]:
    """Group the named WAP transition rows owned by one field area."""

    result: dict[str, list[dict]] = {}
    default = wap.default_entry(table.profile)
    for entry_index, entry in enumerate(table.entries):
        if entry.kind == 0 or entry.area != current_area or not entry.name.value:
            continue
        transition = transition_metadata(entry, entry_index, current_field, default)
        result.setdefault(entry.name.value, []).append(transition)
    return {name: tuple(rows) for name, rows in result.items()}


def _interaction_target(value: int) -> dict:
    """Describe one INF control value without hiding its encoded value."""

    target = {"value": value}
    if value == 0:
        target["type"] = "complete"
    elif 10 <= value < 10 + inf.MESSAGE_COUNT:
        target.update({"type": "row", "row": value - 10})
    elif value == 100:
        target["type"] = "warp"
    else:
        target["type"] = "control"
    return target


def interaction_metadata(
    table: inf.InfFile,
    interaction: inf.InteractionSet,
    set_index: int,
    message_symbols: tuple[str | None, ...] = (),
) -> dict:
    """Return one complete nondefault INF interaction state machine."""

    start = interaction.start
    result = {
        "set": set_index,
        "kindId": start.type_id,
        "kind": {0: "npc", 1: "event"}.get(start.type_id, "unknown"),
        "area": start.area,
        "action": start.action,
        "eventHit": start.event_hit,
        "event": start.event,
        "flagSelectors": [],
        "rows": [],
    }
    for row_index, selector in enumerate(interaction.flags):
        if selector == inf.DEFAULT_FLAG:
            continue
        result["flagSelectors"].append(
            {
                "row": row_index,
                "flag": selector.flag_id,
                "off": _interaction_target(selector.go_off),
                "on": _interaction_target(selector.go_on),
            }
        )
    referenced_rows = {
        value - 10
        for selector in interaction.flags
        for value in (selector.go_off, selector.go_on)
        if 10 <= value < 10 + inf.MESSAGE_COUNT
    }
    referenced_rows.update(
        value - 10
        for row in interaction.messages
        if row != inf.DEFAULT_MESSAGE
        for value in row.go
        if 10 <= value < 10 + inf.MESSAGE_COUNT
    )
    for row_index, row in enumerate(interaction.messages):
        if row == inf.DEFAULT_MESSAGE and row_index not in referenced_rows:
            continue
        message = {
            "row": row_index,
            "kindId": row.type_id,
            "kind": {-1: "action", 0: "message", 1: "selection"}.get(
                row.type_id, "unknown"
            ),
            "messageId": row.message_id,
            "choices": [_interaction_target(value) for value in row.go],
            "flags": {"off": row.flag_off, "on": row.flag_on},
        }
        if 0 <= row.message_id < len(message_symbols):
            symbol = message_symbols[row.message_id]
            if symbol is not None:
                message["messageName"] = symbol
        if row.view_id:
            view_metadata = {"index": row.view_id}
            if row.view_id < len(table.views):
                view = table.views[row.view_id]
                view_metadata.update(
                    {
                        "player": view.player_on,
                        "motion": view.player_motion,
                        "position": view.player_position,
                        "camera": view.camera,
                    }
                )
            message["view"] = view_metadata
        if row.ex_id:
            action_metadata = {"index": row.ex_id}
            if row.ex_id < len(table.extra_actions):
                action = table.extra_actions[row.ex_id]
                action_metadata.update(
                    {"id": action.action_id, "parameters": list(action.parameters)}
                )
            message["extraAction"] = action_metadata
        result["rows"].append(message)
    return result


def area_interactions(
    table: inf.InfFile,
    area_number: int,
    message_symbols: tuple[str | None, ...] = (),
) -> dict[str, tuple[dict, ...]]:
    """Group nondefault INF sets by their exact actor in one field area."""

    result: dict[str, list[dict]] = {}
    for set_index, interaction in enumerate(table.sets):
        if interaction == inf.DEFAULT_SET or interaction.start.action != area_number:
            continue
        result.setdefault(interaction.start.event, []).append(
            interaction_metadata(table, interaction, set_index, message_symbols)
        )
    return {name: tuple(rows) for name, rows in result.items()}


def _populated_zone(table: battle_tbl.EncountTable, zone: int, context: str) -> None:
    if not 0 <= zone < table.profile.zone_count:
        raise FieldWorldError(
            f"{context} references encounter zone {zone}, outside the "
            f"{table.profile.zone_count}-zone {table.profile.name} table"
        )
    if table.zones[zone] == battle_tbl.default_zone(table.profile):
        raise FieldWorldError(f"{context} references empty encounter zone {zone}")


def validate_encounter_links(
    fields: tuple[tuple[str, bytes], ...], table: battle_tbl.EncountTable
) -> EncounterLinkSummary:
    """Join field map/area names and face overrides to ``ENCOUNT.TBL``."""

    maps: dict[int, battle_tbl.SelectorMap] = {}
    for selector_map in table.default_maps:
        if selector_map.key == 0:
            continue
        if selector_map.key in maps:
            raise FieldWorldError(
                f"duplicate encounter default map {selector_map.key}"
            )
        maps[selector_map.key] = selector_map

    default_areas = 0
    default_references = 0
    override_faces = 0
    linked_zones: set[int] = set()
    for stem, data in fields:
        match = re.fullmatch(r"([fk])(\d{3})_(\d{3})", stem, re.IGNORECASE)
        if not match:
            raise FieldWorldError(f"field source name {stem!r} is not [fk]NNN_AAA")
        prefix, field_text, area_text = match.groups()
        field_number, area_number = int(field_text), int(area_text)

        # The runtime uses fNNN as the ENCOUNT map key and AAA as the selector
        # index.  kNNN source names do not identify ENCOUNT map rows.
        selector_map = maps.get(field_number) if prefix.lower() == "f" else None
        if selector_map is not None:
            if area_number >= len(selector_map.entries):
                raise FieldWorldError(
                    f"{stem} area {area_number} exceeds the encounter map's "
                    f"{len(selector_map.entries)} entries"
                )
            selector = selector_map.entries[area_number]
            candidates = [selector.value]
            if selector.flag_a > 0:
                candidates.append(selector.alternate_a)
            if selector.flag_b > 0:
                candidates.append(selector.alternate_b)
            for zone in candidates:
                _populated_zone(table, zone, f"{stem} default map")
                linked_zones.add(zone)
            default_areas += 1
            default_references += len(candidates)

        for zone in fld.encounter_zone_overrides(data):
            _populated_zone(table, zone, f"{stem} collision face")
            linked_zones.add(zone)
            override_faces += 1

    return EncounterLinkSummary(
        len(fields),
        default_areas,
        default_references,
        override_faces,
        len(linked_zones),
    )


def _manifest_fields(manifest: Path) -> tuple[tuple[str, bytes], ...]:
    fields: list[tuple[str, bytes]] = []
    lines = manifest.read_text(encoding="utf-8").splitlines()
    for line_number, line in enumerate(lines, 1):
        parts = line.split()
        if len(parts) != 2:
            raise FieldWorldError(f"{manifest}:{line_number}: expected SHA-1 and path")
        path = Path(parts[1])
        fields.append((path.stem, path.read_bytes()))
    return tuple(fields)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--manifest", type=Path, required=True)
    parser.add_argument("--encounters", type=Path, required=True)
    args = parser.parse_args()
    try:
        fields = _manifest_fields(args.manifest)
        table = battle_tbl.decode_encount(args.encounters.read_bytes())
        summary = validate_encounter_links(fields, table)
    except (FieldWorldError, fld.FldError, battle_tbl.BattleTableError, OSError) as exc:
        parser.error(str(exc))
    print(
        f"linked {summary.fields} fields: {summary.default_areas} default areas, "
        f"{summary.default_zone_references} default zone references, "
        f"{summary.override_faces} override faces, {summary.zones} zones"
    )


if __name__ == "__main__":
    main()
