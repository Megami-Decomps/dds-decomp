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


class FieldWorldError(ValueError):
    """Raised when independently sourced field-world identities disagree."""


@dataclass(frozen=True)
class EncounterLinkSummary:
    fields: int
    default_areas: int
    default_zone_references: int
    override_faces: int
    zones: int


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
