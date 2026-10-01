#!/usr/bin/env python3
"""Disassemble and assemble DDS battle table (``.TBL``) data."""

from __future__ import annotations

import argparse
import shlex
import struct
import sys
from dataclasses import dataclass, replace
from pathlib import Path


class BattleTableError(ValueError):
    """The binary or source is not a supported canonical battle table."""


@dataclass(frozen=True)
class EncountProfile:
    name: str
    zone_count: int
    pool_count: int
    visual_header_halfwords: int
    visual_group_count: int

    @property
    def zone_size(self) -> int:
        return 0x1C + self.pool_count * 0x7C

    @property
    def visual_size(self) -> int:
        return self.visual_header_halfwords * 2 + self.visual_group_count * 0x10


ENCOUNT_PROFILES = {
    profile.name: profile
    for profile in (
        EncountProfile("dds1", 128, 4, 8, 16),
        EncountProfile("dds2", 112, 3, 16, 12),
    )
}


@dataclass(frozen=True)
class Encounter:
    voice_group: int = 0
    start_item: int = 0
    start_item_count: int = 0
    unknown_03: int = 0
    next_encounter: int = 0
    enemies: tuple[int, ...] = (0,) * 11
    background_a: int = 0
    background_b: int = 0
    flags: int = 0
    bgm: int = 0
    event: int = 0


@dataclass(frozen=True)
class Selector:
    value: int = 0
    flag_a: int = 0
    alternate_a: int = 0
    flag_b: int = 0
    alternate_b: int = 0


@dataclass(frozen=True)
class SelectorMap:
    key: int = 0
    entries: tuple[Selector, ...] = (Selector(),) * 64


@dataclass(frozen=True)
class PoolSlot:
    encounter: int = 0
    weight: int = 0
    modifier: int = 0
    next_roll: int = 0


@dataclass(frozen=True)
class EncounterPool:
    threshold: int = 0
    unknown_02: int = 0
    slots: tuple[PoolSlot, ...] = (PoolSlot(),) * 20


@dataclass(frozen=True)
class Zone:
    background_a: int = 0
    background_b: int = 0
    bgm: int = 0
    unknown_06: int = 0
    conditions: tuple[tuple[int, int], ...] = ((0, 0),) * 3
    routes: tuple[int, ...] = (0,) * 8
    pools: tuple[EncounterPool, ...] = ()


@dataclass(frozen=True)
class OverrideRule:
    map_id: int = 0
    flag: int = 0
    chance: int = 0
    zone: int = 0


@dataclass(frozen=True)
class VisualGroup:
    count: int = 0xFFFFFFFF
    members: tuple[int, ...] = (0xFF,) * 8
    tail: int = 0


@dataclass(frozen=True)
class VisualRow:
    header: tuple[int, ...]
    groups: tuple[VisualGroup, ...]


@dataclass(frozen=True)
class EncountTable:
    profile: EncountProfile
    encounters: tuple[Encounter, ...]
    default_maps: tuple[SelectorMap, ...]
    zones: tuple[Zone, ...]
    overrides: tuple[OverrideRule, ...]
    background_maps: tuple[SelectorMap, ...]
    visuals: tuple[VisualRow, ...]


def default_zone(profile: EncountProfile) -> Zone:
    return Zone(pools=(EncounterPool(),) * profile.pool_count)


def default_visual(profile: EncountProfile) -> VisualRow:
    return VisualRow(
        (0,) * profile.visual_header_halfwords,
        (VisualGroup(),) * profile.visual_group_count,
    )


def default_encount(profile: EncountProfile) -> EncountTable:
    return EncountTable(
        profile,
        (Encounter(),) * 1024,
        (SelectorMap(),) * 16,
        (default_zone(profile),) * profile.zone_count,
        (OverrideRule(),) * 16,
        (SelectorMap(),) * 16,
        (default_visual(profile),) * 8,
    )


def _range(value: int, minimum: int, maximum: int, context: str) -> int:
    if not minimum <= value <= maximum:
        raise BattleTableError(f"{context} {value} is outside {minimum}..{maximum}")
    return value


def _s8(value: int, context: str) -> int:
    return _range(value, -0x80, 0x7F, context)


def _u8(value: int, context: str) -> int:
    return _range(value, 0, 0xFF, context)


def _s16(value: int, context: str) -> int:
    return _range(value, -0x8000, 0x7FFF, context)


def _u16(value: int, context: str) -> int:
    return _range(value, 0, 0xFFFF, context)


def _u32(value: int, context: str) -> int:
    return _range(value, 0, 0xFFFFFFFF, context)


def _split_segments(data: bytes) -> tuple[bytes, ...]:
    segments: list[bytes] = []
    offset = 0
    while offset < len(data):
        if offset + 4 > len(data):
            raise BattleTableError(f"truncated segment header at {offset:#x}")
        size = struct.unpack_from("<I", data, offset)[0]
        end = offset + 4 + size
        if end > len(data):
            raise BattleTableError(
                f"segment {len(segments)} extends past file ({size:#x} bytes)"
            )
        segments.append(data[offset + 4 : end])
        aligned = (end + 0xF) & ~0xF
        if aligned > len(data):
            raise BattleTableError(f"truncated alignment after segment {len(segments) - 1}")
        if any(data[end:aligned]):
            raise BattleTableError(
                f"segment {len(segments) - 1} has nonzero alignment padding"
            )
        offset = aligned
    return tuple(segments)


def _join_segments(segments: tuple[bytes, ...]) -> bytes:
    output = bytearray()
    for segment in segments:
        output.extend(struct.pack("<I", len(segment)))
        output.extend(segment)
        output.extend(bytes((-len(output)) & 0xF))
    return bytes(output)


def _profile_from_segments(segments: tuple[bytes, ...]) -> EncountProfile:
    if len(segments) != 6:
        raise BattleTableError(f"ENCOUNT needs six segments, found {len(segments)}")
    for profile in ENCOUNT_PROFILES.values():
        sizes = (
            0xA000,
            0x3040,
            profile.zone_count * profile.zone_size,
            0x80,
            0x3040,
            8 * profile.visual_size,
        )
        if tuple(map(len, segments)) == sizes:
            return profile
    sizes = ", ".join(f"{len(segment):#x}" for segment in segments)
    raise BattleTableError(f"unsupported ENCOUNT segment sizes: {sizes}")


def _decode_selector_map(data: bytes, offset: int) -> SelectorMap:
    entries = tuple(
        Selector(*struct.unpack_from("<Ihhhh", data, offset + 4 + index * 0xC))
        for index in range(64)
    )
    return SelectorMap(struct.unpack_from("<I", data, offset)[0], entries)


def decode_encount(data: bytes) -> EncountTable:
    """Decode and validate a retail DDS1 or DDS2 ``ENCOUNT.TBL``."""

    segments = _split_segments(data)
    profile = _profile_from_segments(segments)

    encounters = []
    for index in range(1024):
        offset = index * 0x28
        row = segments[0][offset : offset + 0x28]
        encounters.append(
            Encounter(
                struct.unpack_from("<b", row, 0)[0],
                row[1],
                row[2],
                row[3],
                struct.unpack_from("<H", row, 4)[0],
                struct.unpack_from("<11H", row, 6),
                struct.unpack_from("<H", row, 0x1C)[0],
                struct.unpack_from("<H", row, 0x1E)[0],
                struct.unpack_from("<I", row, 0x20)[0],
                struct.unpack_from("<H", row, 0x24)[0],
                struct.unpack_from("<H", row, 0x26)[0],
            )
        )

    default_maps = tuple(
        _decode_selector_map(segments[1], index * 0x304) for index in range(16)
    )

    zones = []
    for zone_index in range(profile.zone_count):
        offset = zone_index * profile.zone_size
        row = segments[2][offset : offset + profile.zone_size]
        conditions = tuple(
            (
                struct.unpack_from("<I", row, 8 + index * 4)[0] >> 16,
                struct.unpack_from("<I", row, 8 + index * 4)[0] & 0xFFFF,
            )
            for index in range(3)
        )
        pools = []
        for pool_index in range(profile.pool_count):
            pool_offset = 0x1C + pool_index * 0x7C
            slots = tuple(
                PoolSlot(*struct.unpack_from("<HHbB", row, pool_offset + 4 + index * 6))
                for index in range(20)
            )
            pools.append(
                EncounterPool(
                    struct.unpack_from("<h", row, pool_offset)[0],
                    struct.unpack_from("<h", row, pool_offset + 2)[0],
                    slots,
                )
            )
        zones.append(
            Zone(
                *struct.unpack_from("<4H", row, 0),
                conditions,
                tuple(row[0x14:0x1C]),
                tuple(pools),
            )
        )

    overrides = tuple(
        OverrideRule(*struct.unpack_from("<hhHH", segments[3], index * 8))
        for index in range(16)
    )
    background_maps = tuple(
        _decode_selector_map(segments[4], index * 0x304) for index in range(16)
    )

    visuals = []
    for visual_index in range(8):
        offset = visual_index * profile.visual_size
        row = segments[5][offset : offset + profile.visual_size]
        header = struct.unpack_from(
            f"<{profile.visual_header_halfwords}H", row, 0
        )
        groups = tuple(
            VisualGroup(
                struct.unpack_from("<I", row, len(header) * 2 + index * 0x10)[0],
                tuple(
                    row[
                        len(header) * 2
                        + index * 0x10
                        + 4 : len(header) * 2
                        + index * 0x10
                        + 12
                    ]
                ),
                struct.unpack_from("<I", row, len(header) * 2 + index * 0x10 + 12)[0],
            )
            for index in range(profile.visual_group_count)
        )
        visuals.append(VisualRow(header, groups))

    return EncountTable(
        profile,
        tuple(encounters),
        default_maps,
        tuple(zones),
        overrides,
        background_maps,
        tuple(visuals),
    )


def _encode_selector_maps(rows: tuple[SelectorMap, ...], context: str) -> bytes:
    if len(rows) != 16:
        raise BattleTableError(f"{context} needs 16 maps, found {len(rows)}")
    output = bytearray(16 * 0x304)
    for row_index, row in enumerate(rows):
        if len(row.entries) != 64:
            raise BattleTableError(
                f"{context} {row_index} needs 64 entries, found {len(row.entries)}"
            )
        offset = row_index * 0x304
        struct.pack_into("<I", output, offset, _u32(row.key, f"{context} {row_index} key"))
        for entry_index, entry in enumerate(row.entries):
            struct.pack_into(
                "<Ihhhh",
                output,
                offset + 4 + entry_index * 0xC,
                _u32(entry.value, f"{context} {row_index} entry {entry_index} value"),
                _s16(entry.flag_a, f"{context} {row_index} entry {entry_index} flag_a"),
                _s16(
                    entry.alternate_a,
                    f"{context} {row_index} entry {entry_index} alternate_a",
                ),
                _s16(entry.flag_b, f"{context} {row_index} entry {entry_index} flag_b"),
                _s16(
                    entry.alternate_b,
                    f"{context} {row_index} entry {entry_index} alternate_b",
                ),
            )
    return bytes(output)


def encode_encount(table: EncountTable) -> bytes:
    """Encode one ENCOUNT model to its exact physical profile."""

    profile = ENCOUNT_PROFILES.get(table.profile.name)
    if profile != table.profile:
        raise BattleTableError(f"unknown or modified ENCOUNT profile {table.profile.name!r}")
    if len(table.encounters) != 1024:
        raise BattleTableError(f"ENCOUNT needs 1024 encounters, found {len(table.encounters)}")
    if len(table.zones) != profile.zone_count:
        raise BattleTableError(
            f"profile {profile.name} needs {profile.zone_count} zones, found {len(table.zones)}"
        )
    if len(table.overrides) != 16:
        raise BattleTableError(f"ENCOUNT needs 16 overrides, found {len(table.overrides)}")
    if len(table.visuals) != 8:
        raise BattleTableError(f"ENCOUNT needs eight visual rows, found {len(table.visuals)}")

    encounter_data = bytearray(0xA000)
    for index, row in enumerate(table.encounters):
        if len(row.enemies) != 11:
            raise BattleTableError(f"encounter {index} needs 11 enemy slots")
        struct.pack_into(
            "<bBBBH11HHHIHH",
            encounter_data,
            index * 0x28,
            _s8(row.voice_group, f"encounter {index} voice"),
            _u8(row.start_item, f"encounter {index} item"),
            _u8(row.start_item_count, f"encounter {index} item count"),
            _u8(row.unknown_03, f"encounter {index} unknown_03"),
            _u16(row.next_encounter, f"encounter {index} next"),
            *(_u16(value, f"encounter {index} enemy") for value in row.enemies),
            _u16(row.background_a, f"encounter {index} background_a"),
            _u16(row.background_b, f"encounter {index} background_b"),
            _u32(row.flags, f"encounter {index} flags"),
            _u16(row.bgm, f"encounter {index} bgm"),
            _u16(row.event, f"encounter {index} event"),
        )

    zone_data = bytearray(profile.zone_count * profile.zone_size)
    for zone_index, zone in enumerate(table.zones):
        if len(zone.conditions) != 3 or len(zone.routes) != 8:
            raise BattleTableError(f"zone {zone_index} has an invalid header")
        if len(zone.pools) != profile.pool_count:
            raise BattleTableError(
                f"zone {zone_index} needs {profile.pool_count} pools, found {len(zone.pools)}"
            )
        offset = zone_index * profile.zone_size
        struct.pack_into(
            "<4H",
            zone_data,
            offset,
            _u16(zone.background_a, f"zone {zone_index} background_a"),
            _u16(zone.background_b, f"zone {zone_index} background_b"),
            _u16(zone.bgm, f"zone {zone_index} bgm"),
            _u16(zone.unknown_06, f"zone {zone_index} unknown_06"),
        )
        for condition_index, (kind, value) in enumerate(zone.conditions):
            word = (_u16(kind, "condition kind") << 16) | _u16(value, "condition value")
            struct.pack_into("<I", zone_data, offset + 8 + condition_index * 4, word)
        zone_data[offset + 0x14 : offset + 0x1C] = bytes(
            _u8(value, f"zone {zone_index} route") for value in zone.routes
        )
        for pool_index, pool in enumerate(zone.pools):
            if len(pool.slots) != 20:
                raise BattleTableError(f"zone {zone_index} pool {pool_index} needs 20 slots")
            pool_offset = offset + 0x1C + pool_index * 0x7C
            struct.pack_into(
                "<hh",
                zone_data,
                pool_offset,
                _s16(pool.threshold, f"zone {zone_index} pool {pool_index} threshold"),
                _s16(pool.unknown_02, f"zone {zone_index} pool {pool_index} unknown_02"),
            )
            for slot_index, slot in enumerate(pool.slots):
                struct.pack_into(
                    "<HHbB",
                    zone_data,
                    pool_offset + 4 + slot_index * 6,
                    _u16(slot.encounter, "pool encounter"),
                    _u16(slot.weight, "pool weight"),
                    _s8(slot.modifier, "pool modifier"),
                    _u8(slot.next_roll, "pool next_roll"),
                )

    override_data = bytearray(0x80)
    for index, row in enumerate(table.overrides):
        struct.pack_into(
            "<hhHH",
            override_data,
            index * 8,
            _s16(row.map_id, f"override {index} map"),
            _s16(row.flag, f"override {index} flag"),
            _u16(row.chance, f"override {index} chance"),
            _u16(row.zone, f"override {index} zone"),
        )

    visual_data = bytearray(8 * profile.visual_size)
    for row_index, row in enumerate(table.visuals):
        if len(row.header) != profile.visual_header_halfwords:
            raise BattleTableError(f"visual {row_index} has an invalid header")
        if len(row.groups) != profile.visual_group_count:
            raise BattleTableError(
                f"visual {row_index} needs {profile.visual_group_count} groups"
            )
        offset = row_index * profile.visual_size
        struct.pack_into(
            f"<{len(row.header)}H",
            visual_data,
            offset,
            *(_u16(value, f"visual {row_index} header") for value in row.header),
        )
        group_base = offset + len(row.header) * 2
        for group_index, group in enumerate(row.groups):
            if len(group.members) != 8:
                raise BattleTableError(f"visual {row_index} group {group_index} needs 8 members")
            struct.pack_into(
                "<I8BI",
                visual_data,
                group_base + group_index * 0x10,
                _u32(group.count, "visual group count"),
                *(_u8(value, "visual group member") for value in group.members),
                _u32(group.tail, "visual group tail"),
            )

    return _join_segments(
        (
            bytes(encounter_data),
            _encode_selector_maps(table.default_maps, "default map"),
            bytes(zone_data),
            bytes(override_data),
            _encode_selector_maps(table.background_maps, "background map"),
            bytes(visual_data),
        )
    )


def _tokens(line: str, line_number: int) -> list[str]:
    try:
        return shlex.split(line, comments=True, posix=True)
    except ValueError as exc:
        raise BattleTableError(f"line {line_number}: {exc}") from exc


def _fields(
    tokens: list[str], line_number: int, allowed: set[str], context: str
) -> dict[str, str]:
    fields: dict[str, str] = {}
    for token in tokens:
        if "=" not in token:
            raise BattleTableError(f"line {line_number}: expected key=value in {context}")
        key, value = token.split("=", 1)
        if key not in allowed:
            raise BattleTableError(f"line {line_number}: unknown {context} field {key!r}")
        if key in fields:
            raise BattleTableError(f"line {line_number}: duplicate {context} field {key!r}")
        fields[key] = value
    return fields


def _integer(text: str, line_number: int, context: str) -> int:
    try:
        return int(text, 0)
    except ValueError as exc:
        raise BattleTableError(f"line {line_number}: invalid {context} {text!r}") from exc


def _int_list(text: str, line_number: int, context: str) -> tuple[int, ...]:
    if not text:
        return ()
    return tuple(_integer(value, line_number, context) for value in text.split(","))


def _index(text: str, line_number: int, count: int, context: str) -> int:
    value = _integer(text, line_number, context)
    if not 0 <= value < count:
        raise BattleTableError(f"line {line_number}: {context} {value} is outside 0..{count - 1}")
    return value


def _value(fields: dict[str, str], key: str, base: int, line: int) -> int:
    return _integer(fields[key], line, key) if key in fields else base


def parse_encount_source(text: str) -> EncountTable:
    """Assemble ENCOUNT source on top of its zero/sentinel profile template."""

    meaningful = [
        (number, line.strip())
        for number, line in enumerate(text.splitlines(), 1)
        if line.strip() and not line.lstrip().startswith("#")
    ]
    if not meaningful:
        raise BattleTableError("empty battle table source")
    first_number, first_line = meaningful[0]
    first = _tokens(first_line, first_number)
    if len(first) != 4 or first[:2] != ["battle-table", "1"]:
        raise BattleTableError(
            "source must begin with 'battle-table 1 kind=encounter profile=PROFILE'"
        )
    header = _fields(first[2:], first_number, {"kind", "profile"}, "header")
    if header.get("kind") != "encounter":
        raise BattleTableError("only kind=encounter is currently supported")
    try:
        profile = ENCOUNT_PROFILES[header["profile"]]
    except KeyError as exc:
        raise BattleTableError(f"unknown ENCOUNT profile {header.get('profile')!r}") from exc

    model = default_encount(profile)
    encounters = list(model.encounters)
    default_maps = list(model.default_maps)
    zones = list(model.zones)
    overrides = list(model.overrides)
    background_maps = list(model.background_maps)
    visuals = list(model.visuals)
    seen: set[tuple[str, int]] = set()
    source_index = 1

    while source_index < len(meaningful):
        line_number, line = meaningful[source_index]
        tokens = _tokens(line, line_number)
        directive = tokens[0]
        if directive == "encounter":
            if len(tokens) < 2:
                raise BattleTableError(f"line {line_number}: encounter needs an index")
            index = _index(tokens[1], line_number, 1024, "encounter index")
            fields = _fields(
                tokens[2:],
                line_number,
                {
                    "voice", "item", "item_count", "unknown_03", "next", "enemies",
                    "backgrounds", "flags", "bgm", "event",
                },
                directive,
            )
            key = (directive, index)
            if key in seen:
                raise BattleTableError(f"line {line_number}: duplicate encounter {index}")
            seen.add(key)
            enemies = _int_list(fields.get("enemies", ""), line_number, "enemy")
            if len(enemies) > 11:
                raise BattleTableError(f"line {line_number}: encounter has more than 11 enemies")
            enemies += (0,) * (11 - len(enemies))
            backgrounds = _int_list(fields.get("backgrounds", ""), line_number, "background")
            if backgrounds and len(backgrounds) != 2:
                raise BattleTableError(f"line {line_number}: backgrounds needs two values")
            encounters[index] = Encounter(
                _value(fields, "voice", 0, line_number),
                _value(fields, "item", 0, line_number),
                _value(fields, "item_count", 0, line_number),
                _value(fields, "unknown_03", 0, line_number),
                _value(fields, "next", 0, line_number),
                enemies,
                backgrounds[0] if backgrounds else 0,
                backgrounds[1] if backgrounds else 0,
                _value(fields, "flags", 0, line_number),
                _value(fields, "bgm", 0, line_number),
                _value(fields, "event", 0, line_number),
            )
        elif directive == "override":
            if len(tokens) < 2:
                raise BattleTableError(f"line {line_number}: override needs an index")
            index = _index(tokens[1], line_number, 16, "override index")
            fields = _fields(
                tokens[2:], line_number, {"map", "flag", "chance", "zone"}, directive
            )
            key = (directive, index)
            if key in seen:
                raise BattleTableError(f"line {line_number}: duplicate override {index}")
            seen.add(key)
            overrides[index] = OverrideRule(
                _value(fields, "map", 0, line_number),
                _value(fields, "flag", 0, line_number),
                _value(fields, "chance", 0, line_number),
                _value(fields, "zone", 0, line_number),
            )
        elif directive in {"default-map", "background-map"}:
            if len(tokens) < 2:
                raise BattleTableError(f"line {line_number}: {directive} needs an index")
            index = _index(tokens[1], line_number, 16, f"{directive} index")
            fields = _fields(tokens[2:], line_number, {"map"}, directive)
            key = (directive, index)
            if key in seen:
                raise BattleTableError(f"line {line_number}: duplicate {directive} {index}")
            seen.add(key)
            entries = [Selector()] * 64
            entry_seen: set[int] = set()
            source_index += 1
            while source_index < len(meaningful):
                child_number, child_line = meaningful[source_index]
                child = _tokens(child_line, child_number)
                if child == ["end"]:
                    break
                if len(child) < 2 or child[0] != "entry":
                    raise BattleTableError(f"line {child_number}: expected entry or end")
                entry_index = _index(child[1], child_number, 64, "entry index")
                if entry_index in entry_seen:
                    raise BattleTableError(f"line {child_number}: duplicate entry {entry_index}")
                entry_seen.add(entry_index)
                if directive == "default-map":
                    entry_names = ("zone", "zone_a", "zone_b")
                else:
                    entry_names = ("background", "background_a", "background_b")
                entry_fields = _fields(
                    child[2:], child_number,
                    {entry_names[0], "flag_a", entry_names[1], "flag_b", entry_names[2]},
                    "entry",
                )
                entries[entry_index] = Selector(
                    _value(entry_fields, entry_names[0], 0, child_number),
                    _value(entry_fields, "flag_a", 0, child_number),
                    _value(entry_fields, entry_names[1], 0, child_number),
                    _value(entry_fields, "flag_b", 0, child_number),
                    _value(entry_fields, entry_names[2], 0, child_number),
                )
                source_index += 1
            if source_index == len(meaningful):
                raise BattleTableError(f"line {line_number}: {directive} has no end")
            row = SelectorMap(_value(fields, "map", 0, line_number), tuple(entries))
            if directive == "default-map":
                default_maps[index] = row
            else:
                background_maps[index] = row
        elif directive == "zone":
            if len(tokens) < 2:
                raise BattleTableError(f"line {line_number}: zone needs an index")
            index = _index(tokens[1], line_number, profile.zone_count, "zone index")
            fields = _fields(
                tokens[2:],
                line_number,
                {"backgrounds", "bgm", "unknown_06"},
                directive,
            )
            key = (directive, index)
            if key in seen:
                raise BattleTableError(f"line {line_number}: duplicate zone {index}")
            seen.add(key)
            backgrounds = _int_list(fields.get("backgrounds", ""), line_number, "background")
            if backgrounds and len(backgrounds) != 2:
                raise BattleTableError(f"line {line_number}: backgrounds needs two values")
            conditions = [(0, 0)] * 3
            routes = (0,) * 8
            pools = [EncounterPool()] * profile.pool_count
            condition_seen: set[int] = set()
            routes_seen = False
            pool_seen: set[int] = set()
            source_index += 1
            while source_index < len(meaningful):
                child_number, child_line = meaningful[source_index]
                child = _tokens(child_line, child_number)
                if child == ["end"]:
                    break
                if child[0] == "condition":
                    condition_index = _index(child[1], child_number, 3, "condition index")
                    if condition_index in condition_seen:
                        raise BattleTableError(
                            f"line {child_number}: duplicate condition {condition_index}"
                        )
                    condition_seen.add(condition_index)
                    condition_fields = _fields(
                        child[2:], child_number, {"kind", "value"}, "condition"
                    )
                    conditions[condition_index] = (
                        _value(condition_fields, "kind", 0, child_number),
                        _value(condition_fields, "value", 0, child_number),
                    )
                elif child[0] == "routes":
                    if routes_seen:
                        raise BattleTableError(f"line {child_number}: duplicate routes")
                    routes_seen = True
                    route_names = ("abc", "ab", "ac", "bc", "a", "b", "c", "none")
                    route_fields = _fields(
                        child[1:], child_number, {"values", *route_names}, "routes"
                    )
                    named = set(route_names).intersection(route_fields)
                    if "values" in route_fields and named:
                        raise BattleTableError(
                            f"line {child_number}: routes values cannot be combined "
                            "with named routes"
                        )
                    if "values" in route_fields:
                        routes = _int_list(route_fields["values"], child_number, "route")
                        if len(routes) != 8:
                            raise BattleTableError(
                                f"line {child_number}: routes needs eight values"
                            )
                    else:
                        routes = tuple(
                            _value(route_fields, name, 0, child_number) for name in route_names
                        )
                elif child[0] == "pool":
                    pool_index = _index(child[1], child_number, profile.pool_count, "pool index")
                    if pool_index in pool_seen:
                        raise BattleTableError(
                            f"line {child_number}: duplicate pool {pool_index}"
                        )
                    pool_seen.add(pool_index)
                    pool_fields = _fields(
                        child[2:], child_number, {"threshold", "unknown_02"}, "pool"
                    )
                    slots = [PoolSlot()] * 20
                    slot_seen: set[int] = set()
                    source_index += 1
                    while source_index < len(meaningful):
                        slot_number, slot_line = meaningful[source_index]
                        slot_tokens = _tokens(slot_line, slot_number)
                        if slot_tokens == ["end"]:
                            break
                        if len(slot_tokens) < 2 or slot_tokens[0] != "slot":
                            raise BattleTableError(f"line {slot_number}: expected slot or end")
                        slot_index = _index(slot_tokens[1], slot_number, 20, "slot index")
                        if slot_index in slot_seen:
                            raise BattleTableError(
                                f"line {slot_number}: duplicate slot {slot_index}"
                            )
                        slot_seen.add(slot_index)
                        slot_fields = _fields(
                            slot_tokens[2:], slot_number,
                            {"encounter", "weight", "modifier", "next_roll"}, "slot",
                        )
                        slots[slot_index] = PoolSlot(
                            _value(slot_fields, "encounter", 0, slot_number),
                            _value(slot_fields, "weight", 0, slot_number),
                            _value(slot_fields, "modifier", 0, slot_number),
                            _value(slot_fields, "next_roll", 0, slot_number),
                        )
                        source_index += 1
                    if source_index == len(meaningful):
                        raise BattleTableError(f"line {child_number}: pool has no end")
                    pools[pool_index] = EncounterPool(
                        _value(pool_fields, "threshold", 0, child_number),
                        _value(pool_fields, "unknown_02", 0, child_number),
                        tuple(slots),
                    )
                else:
                    raise BattleTableError(
                        f"line {child_number}: expected condition, routes, pool, or end"
                    )
                source_index += 1
            if source_index == len(meaningful):
                raise BattleTableError(f"line {line_number}: zone has no end")
            zones[index] = Zone(
                backgrounds[0] if backgrounds else 0,
                backgrounds[1] if backgrounds else 0,
                _value(fields, "bgm", 0, line_number),
                _value(fields, "unknown_06", 0, line_number),
                tuple(conditions), tuple(routes), tuple(pools),
            )
        elif directive == "visual":
            if len(tokens) < 2:
                raise BattleTableError(f"line {line_number}: visual needs an index")
            index = _index(tokens[1], line_number, 8, "visual index")
            fields = _fields(tokens[2:], line_number, {"header"}, directive)
            header_values = _int_list(fields.get("header", ""), line_number, "visual header")
            if len(header_values) != profile.visual_header_halfwords:
                raise BattleTableError(
                    f"line {line_number}: visual header needs "
                    f"{profile.visual_header_halfwords} values"
                )
            key = (directive, index)
            if key in seen:
                raise BattleTableError(f"line {line_number}: duplicate visual {index}")
            seen.add(key)
            groups = [VisualGroup()] * profile.visual_group_count
            group_seen: set[int] = set()
            source_index += 1
            while source_index < len(meaningful):
                child_number, child_line = meaningful[source_index]
                child = _tokens(child_line, child_number)
                if child == ["end"]:
                    break
                if len(child) < 2 or child[0] != "group":
                    raise BattleTableError(f"line {child_number}: expected group or end")
                group_index = _index(
                    child[1], child_number, profile.visual_group_count, "group index"
                )
                if group_index in group_seen:
                    raise BattleTableError(
                        f"line {child_number}: duplicate group {group_index}"
                    )
                group_seen.add(group_index)
                group_fields = _fields(
                    child[2:], child_number, {"count", "members", "tail"}, "group"
                )
                members = _int_list(
                    group_fields.get("members", ""), child_number, "group member"
                )
                if len(members) != 8:
                    raise BattleTableError(f"line {child_number}: group needs eight members")
                groups[group_index] = VisualGroup(
                    _value(group_fields, "count", 0xFFFFFFFF, child_number),
                    members,
                    _value(group_fields, "tail", 0, child_number),
                )
                source_index += 1
            if source_index == len(meaningful):
                raise BattleTableError(f"line {line_number}: visual has no end")
            visuals[index] = VisualRow(header_values, tuple(groups))
        else:
            raise BattleTableError(f"line {line_number}: unknown directive {directive!r}")
        source_index += 1

    result = EncountTable(
        profile,
        tuple(encounters), tuple(default_maps), tuple(zones), tuple(overrides),
        tuple(background_maps), tuple(visuals),
    )
    encode_encount(result)
    return result


def _append(fields: list[str], key: str, value: int, default: int = 0) -> None:
    if value != default:
        fields.append(f"{key}={value}")


def _list(values: tuple[int, ...]) -> str:
    return ",".join(str(value) for value in values)


def _trimmed(values: tuple[int, ...]) -> tuple[int, ...]:
    end = len(values)
    while end and values[end - 1] == 0:
        end -= 1
    return values[:end]


def _render_selector_maps(
    lines: list[str], directive: str, rows: tuple[SelectorMap, ...]
) -> None:
    if directive == "default-map":
        names = ("zone", "zone_a", "zone_b")
    else:
        names = ("background", "background_a", "background_b")
    for row_index, row in enumerate(rows):
        if row == SelectorMap():
            continue
        suffix = f" map={row.key}" if row.key else ""
        lines.append(f"{directive} {row_index}{suffix}")
        for entry_index, entry in enumerate(row.entries):
            if entry == Selector():
                continue
            fields: list[str] = []
            _append(fields, names[0], entry.value)
            _append(fields, "flag_a", entry.flag_a)
            _append(fields, names[1], entry.alternate_a)
            _append(fields, "flag_b", entry.flag_b)
            _append(fields, names[2], entry.alternate_b)
            lines.append(f"  entry {entry_index} {' '.join(fields)}")
        lines.extend(("end", ""))


def render_encount_source(table: EncountTable) -> str:
    """Render compact canonical source relative to the selected profile."""

    encode_encount(table)
    lines = [f"battle-table 1 kind=encounter profile={table.profile.name}", ""]
    for index, row in enumerate(table.encounters):
        if row == Encounter():
            continue
        fields: list[str] = []
        _append(fields, "voice", row.voice_group)
        _append(fields, "item", row.start_item)
        _append(fields, "item_count", row.start_item_count)
        _append(fields, "unknown_03", row.unknown_03)
        _append(fields, "next", row.next_encounter)
        enemies = _trimmed(row.enemies)
        if enemies:
            fields.append(f"enemies={_list(enemies)}")
        if row.background_a or row.background_b:
            fields.append(f"backgrounds={row.background_a},{row.background_b}")
        if row.flags:
            fields.append(f"flags={row.flags:#x}")
        _append(fields, "bgm", row.bgm)
        _append(fields, "event", row.event)
        lines.extend((f"encounter {index} {' '.join(fields)}", ""))

    _render_selector_maps(lines, "default-map", table.default_maps)

    empty_zone = default_zone(table.profile)
    for zone_index, zone in enumerate(table.zones):
        if zone == empty_zone:
            continue
        fields: list[str] = []
        if zone.background_a or zone.background_b:
            fields.append(f"backgrounds={zone.background_a},{zone.background_b}")
        _append(fields, "bgm", zone.bgm)
        _append(fields, "unknown_06", zone.unknown_06)
        suffix = f" {' '.join(fields)}" if fields else ""
        lines.append(f"zone {zone_index}{suffix}")
        for condition_index, (kind, value) in enumerate(zone.conditions):
            if kind or value:
                lines.append(f"  condition {condition_index} kind={kind} value={value}")
        if any(zone.routes):
            route_names = ("abc", "ab", "ac", "bc", "a", "b", "c", "none")
            route_fields = " ".join(
                f"{name}={value}" for name, value in zip(route_names, zone.routes)
            )
            lines.append(f"  routes {route_fields}")
        for pool_index, pool in enumerate(zone.pools):
            if pool == EncounterPool():
                continue
            pool_fields: list[str] = []
            _append(pool_fields, "threshold", pool.threshold)
            _append(pool_fields, "unknown_02", pool.unknown_02)
            suffix = f" {' '.join(pool_fields)}" if pool_fields else ""
            lines.append(f"  pool {pool_index}{suffix}")
            for slot_index, slot in enumerate(pool.slots):
                if slot == PoolSlot():
                    continue
                slot_fields: list[str] = []
                _append(slot_fields, "encounter", slot.encounter)
                _append(slot_fields, "weight", slot.weight)
                _append(slot_fields, "modifier", slot.modifier)
                _append(slot_fields, "next_roll", slot.next_roll)
                lines.append(f"    slot {slot_index} {' '.join(slot_fields)}")
            lines.append("  end")
        lines.extend(("end", ""))

    for index, row in enumerate(table.overrides):
        if row == OverrideRule():
            continue
        fields: list[str] = []
        _append(fields, "map", row.map_id)
        _append(fields, "flag", row.flag)
        _append(fields, "chance", row.chance)
        _append(fields, "zone", row.zone)
        lines.extend((f"override {index} {' '.join(fields)}", ""))

    _render_selector_maps(lines, "background-map", table.background_maps)

    empty_visual = default_visual(table.profile)
    for row_index, row in enumerate(table.visuals):
        if row == empty_visual:
            continue
        lines.append(f"visual {row_index} header={_list(row.header)}")
        for group_index, group in enumerate(row.groups):
            if group == VisualGroup():
                continue
            fields = [f"count={group.count}", f"members={_list(group.members)}"]
            _append(fields, "tail", group.tail)
            lines.append(f"  group {group_index} {' '.join(fields)}")
        lines.extend(("end", ""))
    return "\n".join(lines).rstrip() + "\n"


def _read(path: Path) -> bytes:
    try:
        return path.read_bytes()
    except OSError as exc:
        raise BattleTableError(f"cannot read {path}: {exc}") from exc


def _write_bytes(path: Path, data: bytes) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(data)


def _write_text(path: Path, text: str) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(text, encoding="utf-8")


def _command_disassemble(args: argparse.Namespace) -> None:
    model = decode_encount(_read(args.input))
    _write_text(args.output, render_encount_source(model))


def _command_assemble(args: argparse.Namespace) -> None:
    try:
        source = args.input.read_text(encoding="utf-8")
    except OSError as exc:
        raise BattleTableError(f"cannot read {args.input}: {exc}") from exc
    _write_bytes(args.output, encode_encount(parse_encount_source(source)))


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    subparsers = parser.add_subparsers(dest="command", required=True)
    for name, handler in (
        ("disassemble", _command_disassemble),
        ("assemble", _command_assemble),
    ):
        command = subparsers.add_parser(name)
        command.add_argument("input", type=Path)
        command.add_argument("output", type=Path)
        command.set_defaults(handler=handler)
    args = parser.parse_args(argv)
    try:
        args.handler(args)
    except BattleTableError as exc:
        parser.error(str(exc))
    return 0


if __name__ == "__main__":
    sys.exit(main())
