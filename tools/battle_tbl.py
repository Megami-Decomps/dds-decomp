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


@dataclass(frozen=True)
class UnitProfile:
    name: str
    party_size: int


UNIT_PROFILES = {
    profile.name: profile
    for profile in (
        UnitProfile("dds1", 0x1A4),
        UnitProfile("dds2", 0x1C4),
    )
}


@dataclass(frozen=True)
class PartyTemplate:
    flags: int = 0
    affinity_source: int = 0
    unit_id: int = 0
    hp: int = 0
    max_hp: int = 0
    mp: int = 0
    max_mp: int = 0
    status: int = 0
    experience: int = 0
    level: int = 0
    stats: tuple[int, ...] = (0,) * 5
    unknown_1b_21: bytes = bytes(7)
    skills: tuple[int, ...] = (0,) * 24
    equipped_bullet: int = 0
    unknown_54: int = 0
    current_profile: int = 0
    tail: bytes = b""


@dataclass(frozen=True)
class AffinityRow:
    values: tuple[int, ...] = (0,) * 19


@dataclass(frozen=True)
class EnemyTemplate:
    flags: int = 0
    race: int = 0
    level: int = 0
    hp: int = 0
    max_hp: int = 0
    mp: int = 0
    max_mp: int = 0
    growth_profile: int = 0
    unknown_0f: int = 0
    stats: tuple[int, ...] = (0,) * 5
    summon_category: int = 0
    unknown_16_17: bytes = bytes(2)
    skills: tuple[int, ...] = (0,) * 8
    macca: int = 0
    experience: int = 0
    atma_points: int = 0
    atma_bonus: int = 0
    unknown_34_3d: bytes = bytes(10)
    drop_items: tuple[int, ...] = (0, 0)
    drop_rates: tuple[int, ...] = (0, 0)
    conditional_drop_flag: int = 0
    conditional_drop_item: int = 0
    conditional_drop_rate: int = 0
    attack_attribute: int = 0
    attack_repeats: int = 0
    result_parameter: int = 0
    tail: bytes = bytes(3)


@dataclass(frozen=True)
class UnitTable:
    profile: UnitProfile
    party: tuple[PartyTemplate, ...]
    party_affinities: tuple[AffinityRow, ...]
    alternate_affinities: tuple[AffinityRow, ...]
    enemies: tuple[EnemyTemplate, ...]
    enemy_affinities: tuple[AffinityRow, ...]


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


def _s32(value: int, context: str) -> int:
    return _range(value, -0x80000000, 0x7FFFFFFF, context)


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


def _unit_profile_from_segments(segments: tuple[bytes, ...]) -> UnitProfile:
    if len(segments) != 5:
        raise BattleTableError(f"UNIT needs five segments, found {len(segments)}")
    for profile in UNIT_PROFILES.values():
        sizes = (16 * profile.party_size, 0x4C0, 0x4C0, 0x7200, 0x7200)
        if tuple(map(len, segments)) == sizes:
            return profile
    sizes = ", ".join(f"{len(segment):#x}" for segment in segments)
    raise BattleTableError(f"unsupported UNIT segment sizes: {sizes}")


def _decode_affinities(data: bytes) -> tuple[AffinityRow, ...]:
    if len(data) % 0x4C:
        raise BattleTableError(f"affinity segment has invalid size {len(data):#x}")
    return tuple(
        AffinityRow(struct.unpack_from("<19I", data, offset))
        for offset in range(0, len(data), 0x4C)
    )


def decode_unit(data: bytes) -> UnitTable:
    """Decode and validate a retail DDS1 or DDS2 ``UNIT.TBL``."""

    segments = _split_segments(data)
    profile = _unit_profile_from_segments(segments)
    party = []
    for index in range(16):
        row = segments[0][
            index * profile.party_size : (index + 1) * profile.party_size
        ]
        party.append(
            PartyTemplate(
                flags=struct.unpack_from("<H", row, 0)[0],
                affinity_source=struct.unpack_from("<H", row, 2)[0],
                unit_id=struct.unpack_from("<H", row, 4)[0],
                hp=struct.unpack_from("<H", row, 6)[0],
                max_hp=struct.unpack_from("<H", row, 8)[0],
                mp=struct.unpack_from("<H", row, 0xA)[0],
                max_mp=struct.unpack_from("<H", row, 0xC)[0],
                status=struct.unpack_from("<H", row, 0xE)[0],
                experience=struct.unpack_from("<I", row, 0x10)[0],
                level=struct.unpack_from("<H", row, 0x14)[0],
                stats=tuple(row[0x16:0x1B]),
                unknown_1b_21=row[0x1B:0x22],
                skills=struct.unpack_from("<24H", row, 0x22),
                equipped_bullet=struct.unpack_from("<H", row, 0x52)[0],
                unknown_54=row[0x54],
                current_profile=row[0x55],
                tail=row[0x56:],
            )
        )

    enemies = []
    for offset in range(0, len(segments[3]), 0x4C):
        row = segments[3][offset : offset + 0x4C]
        enemies.append(
            EnemyTemplate(
                flags=struct.unpack_from("<I", row, 0)[0],
                race=row[4],
                level=row[5],
                hp=struct.unpack_from("<H", row, 6)[0],
                max_hp=struct.unpack_from("<H", row, 8)[0],
                mp=struct.unpack_from("<H", row, 0xA)[0],
                max_mp=struct.unpack_from("<H", row, 0xC)[0],
                growth_profile=struct.unpack_from("<b", row, 0xE)[0],
                unknown_0f=row[0xF],
                stats=tuple(row[0x10:0x15]),
                summon_category=row[0x15],
                unknown_16_17=row[0x16:0x18],
                skills=struct.unpack_from("<8H", row, 0x18),
                macca=struct.unpack_from("<i", row, 0x28)[0],
                experience=struct.unpack_from("<H", row, 0x2C)[0],
                atma_points=struct.unpack_from("<H", row, 0x2E)[0],
                atma_bonus=struct.unpack_from("<I", row, 0x30)[0],
                unknown_34_3d=row[0x34:0x3E],
                drop_items=tuple(row[0x3E:0x40]),
                drop_rates=tuple(row[0x40:0x42]),
                conditional_drop_flag=struct.unpack_from("<H", row, 0x42)[0],
                conditional_drop_item=row[0x44],
                conditional_drop_rate=row[0x45],
                attack_attribute=struct.unpack_from("<b", row, 0x46)[0],
                attack_repeats=row[0x47],
                result_parameter=row[0x48],
                tail=row[0x49:0x4C],
            )
        )
    return UnitTable(
        profile,
        tuple(party),
        _decode_affinities(segments[1]),
        _decode_affinities(segments[2]),
        tuple(enemies),
        _decode_affinities(segments[4]),
    )


def default_unit(profile: UnitProfile) -> UnitTable:
    return UnitTable(
        profile,
        tuple(PartyTemplate(tail=bytes(profile.party_size - 0x56)) for _ in range(16)),
        (AffinityRow(),) * 16,
        (AffinityRow(),) * 16,
        (EnemyTemplate(),) * 384,
        (AffinityRow(),) * 384,
    )


def _encode_affinities(rows: tuple[AffinityRow, ...], count: int, context: str) -> bytes:
    if len(rows) != count:
        raise BattleTableError(f"{context} needs {count} rows, found {len(rows)}")
    output = bytearray(count * 0x4C)
    for index, row in enumerate(rows):
        if len(row.values) != 19:
            raise BattleTableError(f"{context} {index} needs 19 values")
        struct.pack_into(
            "<19I",
            output,
            index * 0x4C,
            *(_u32(value, f"{context} {index} value") for value in row.values),
        )
    return bytes(output)


def encode_unit(table: UnitTable) -> bytes:
    """Encode one UNIT model to its exact physical profile."""

    profile = UNIT_PROFILES.get(table.profile.name)
    if profile != table.profile:
        raise BattleTableError(f"unknown or modified UNIT profile {table.profile.name!r}")
    if len(table.party) != 16:
        raise BattleTableError(f"UNIT needs 16 party templates, found {len(table.party)}")
    if len(table.enemies) != 384:
        raise BattleTableError(f"UNIT needs 384 enemy templates, found {len(table.enemies)}")

    party_data = bytearray(16 * profile.party_size)
    for index, row in enumerate(table.party):
        context = f"party {index}"
        if len(row.stats) != 5 or len(row.skills) != 24:
            raise BattleTableError(f"{context} has invalid stats or skills")
        if len(row.unknown_1b_21) != 7:
            raise BattleTableError(f"{context} unknown_1b_21 needs 7 bytes")
        if len(row.tail) != profile.party_size - 0x56:
            raise BattleTableError(
                f"{context} tail needs {profile.party_size - 0x56} bytes"
            )
        offset = index * profile.party_size
        struct.pack_into(
            "<8HIH5B",
            party_data,
            offset,
            _u16(row.flags, f"{context} flags"),
            _u16(row.affinity_source, f"{context} affinity source"),
            _u16(row.unit_id, f"{context} unit"),
            _u16(row.hp, f"{context} hp"),
            _u16(row.max_hp, f"{context} max_hp"),
            _u16(row.mp, f"{context} mp"),
            _u16(row.max_mp, f"{context} max_mp"),
            _u16(row.status, f"{context} status"),
            _u32(row.experience, f"{context} experience"),
            _u16(row.level, f"{context} level"),
            *(_u8(value, f"{context} stat") for value in row.stats),
        )
        party_data[offset + 0x1B : offset + 0x22] = row.unknown_1b_21
        struct.pack_into(
            "<24H",
            party_data,
            offset + 0x22,
            *(_u16(value, f"{context} skill") for value in row.skills),
        )
        struct.pack_into(
            "<HBB",
            party_data,
            offset + 0x52,
            _u16(row.equipped_bullet, f"{context} bullet"),
            _u8(row.unknown_54, f"{context} unknown_54"),
            _u8(row.current_profile, f"{context} current_profile"),
        )
        party_data[offset + 0x56 : offset + profile.party_size] = row.tail

    enemy_data = bytearray(384 * 0x4C)
    for index, row in enumerate(table.enemies):
        context = f"enemy {index}"
        if len(row.stats) != 5 or len(row.skills) != 8:
            raise BattleTableError(f"{context} has invalid stats or skills")
        if len(row.unknown_16_17) != 2 or len(row.unknown_34_3d) != 10:
            raise BattleTableError(f"{context} has invalid unknown byte fields")
        if len(row.drop_items) != 2 or len(row.drop_rates) != 2 or len(row.tail) != 3:
            raise BattleTableError(f"{context} has invalid drop or tail fields")
        offset = index * 0x4C
        struct.pack_into(
            "<IBB4HbB5BB",
            enemy_data,
            offset,
            _u32(row.flags, f"{context} flags"),
            _u8(row.race, f"{context} race"),
            _u8(row.level, f"{context} level"),
            _u16(row.hp, f"{context} hp"),
            _u16(row.max_hp, f"{context} max_hp"),
            _u16(row.mp, f"{context} mp"),
            _u16(row.max_mp, f"{context} max_mp"),
            _s8(row.growth_profile, f"{context} growth_profile"),
            _u8(row.unknown_0f, f"{context} unknown_0f"),
            *(_u8(value, f"{context} stat") for value in row.stats),
            _u8(row.summon_category, f"{context} summon_category"),
        )
        enemy_data[offset + 0x16 : offset + 0x18] = row.unknown_16_17
        struct.pack_into(
            "<8HiHHI",
            enemy_data,
            offset + 0x18,
            *(_u16(value, f"{context} skill") for value in row.skills),
            _s32(row.macca, f"{context} macca"),
            _u16(row.experience, f"{context} experience"),
            _u16(row.atma_points, f"{context} atma_points"),
            _u32(row.atma_bonus, f"{context} atma_bonus"),
        )
        enemy_data[offset + 0x34 : offset + 0x3E] = row.unknown_34_3d
        enemy_data[offset + 0x3E : offset + 0x40] = bytes(
            _u8(value, f"{context} drop item") for value in row.drop_items
        )
        enemy_data[offset + 0x40 : offset + 0x42] = bytes(
            _u8(value, f"{context} drop rate") for value in row.drop_rates
        )
        struct.pack_into(
            "<HBBbBB",
            enemy_data,
            offset + 0x42,
            _u16(row.conditional_drop_flag, f"{context} conditional drop flag"),
            _u8(row.conditional_drop_item, f"{context} conditional drop item"),
            _u8(row.conditional_drop_rate, f"{context} conditional drop rate"),
            _s8(row.attack_attribute, f"{context} attack attribute"),
            _u8(row.attack_repeats, f"{context} attack repeats"),
            _u8(row.result_parameter, f"{context} result parameter"),
        )
        enemy_data[offset + 0x49 : offset + 0x4C] = row.tail

    return _join_segments(
        (
            bytes(party_data),
            _encode_affinities(table.party_affinities, 16, "party affinity"),
            _encode_affinities(table.alternate_affinities, 16, "alternate affinity"),
            bytes(enemy_data),
            _encode_affinities(table.enemy_affinities, 384, "enemy affinity"),
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


def _bytes_field(
    fields: dict[str, str], key: str, size: int, line_number: int
) -> bytes:
    if key not in fields:
        return bytes(size)
    try:
        value = bytes.fromhex(fields[key])
    except ValueError as exc:
        raise BattleTableError(f"line {line_number}: invalid hexadecimal {key}") from exc
    if len(value) != size:
        raise BattleTableError(f"line {line_number}: {key} needs {size} bytes")
    return value


def _sized_list(
    fields: dict[str, str], key: str, size: int, line_number: int
) -> tuple[int, ...]:
    values = _int_list(fields.get(key, ""), line_number, key)
    if len(values) > size:
        raise BattleTableError(f"line {line_number}: {key} has more than {size} values")
    return values + (0,) * (size - len(values))


def parse_unit_source(text: str) -> UnitTable:
    """Assemble UNIT source on top of the selected profile's zero template."""

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
            "source must begin with 'battle-table 1 kind=unit profile=PROFILE'"
        )
    header = _fields(first[2:], first_number, {"kind", "profile"}, "header")
    if header.get("kind") != "unit":
        raise BattleTableError("UNIT source needs kind=unit")
    try:
        profile = UNIT_PROFILES[header["profile"]]
    except KeyError as exc:
        raise BattleTableError(f"unknown UNIT profile {header.get('profile')!r}") from exc

    model = default_unit(profile)
    party = list(model.party)
    party_affinities = list(model.party_affinities)
    alternate_affinities = list(model.alternate_affinities)
    enemies = list(model.enemies)
    enemy_affinities = list(model.enemy_affinities)
    affinity_targets = {
        "party-affinity": party_affinities,
        "alternate-affinity": alternate_affinities,
        "enemy-affinity": enemy_affinities,
    }
    seen: set[tuple[str, int]] = set()

    for line_number, line in meaningful[1:]:
        tokens = _tokens(line, line_number)
        directive = tokens[0]
        if len(tokens) < 2:
            raise BattleTableError(f"line {line_number}: {directive} needs an index")
        count = 384 if directive in {"enemy", "enemy-affinity"} else 16
        index = _index(tokens[1], line_number, count, f"{directive} index")
        key = (directive, index)
        if key in seen:
            raise BattleTableError(f"line {line_number}: duplicate {directive} {index}")
        seen.add(key)

        if directive == "party":
            allowed = {
                "flags", "unit", "hp", "max_hp", "mp", "max_mp", "status",
                "affinity_source",
                "experience", "level", "stats", "unknown_1b_21", "skills",
                "bullet", "unknown_54", "current_profile", "tail",
            }
            fields = _fields(tokens[2:], line_number, allowed, directive)
            party[index] = PartyTemplate(
                flags=_value(fields, "flags", 0, line_number),
                affinity_source=_value(fields, "affinity_source", 0, line_number),
                unit_id=_value(fields, "unit", 0, line_number),
                hp=_value(fields, "hp", 0, line_number),
                max_hp=_value(fields, "max_hp", 0, line_number),
                mp=_value(fields, "mp", 0, line_number),
                max_mp=_value(fields, "max_mp", 0, line_number),
                status=_value(fields, "status", 0, line_number),
                experience=_value(fields, "experience", 0, line_number),
                level=_value(fields, "level", 0, line_number),
                stats=_sized_list(fields, "stats", 5, line_number),
                unknown_1b_21=_bytes_field(fields, "unknown_1b_21", 7, line_number),
                skills=_sized_list(fields, "skills", 24, line_number),
                equipped_bullet=_value(fields, "bullet", 0, line_number),
                unknown_54=_value(fields, "unknown_54", 0, line_number),
                current_profile=_value(fields, "current_profile", 0, line_number),
                tail=_bytes_field(fields, "tail", profile.party_size - 0x56, line_number),
            )
        elif directive in affinity_targets:
            fields = _fields(tokens[2:], line_number, {"values"}, directive)
            affinity_targets[directive][index] = AffinityRow(
                _sized_list(fields, "values", 19, line_number)
            )
        elif directive == "enemy":
            allowed = {
                "flags", "race", "level", "hp", "max_hp", "mp", "max_mp",
                "growth", "unknown_0f", "stats", "summon_category",
                "unknown_16_17", "skills", "macca", "experience", "atma_points",
                "atma_bonus", "unknown_34_3d", "drop_items", "drop_rates",
                "conditional_drop", "attack_attribute", "attack_repeats",
                "result_parameter", "tail",
            }
            fields = _fields(tokens[2:], line_number, allowed, directive)
            conditional = _sized_list(fields, "conditional_drop", 3, line_number)
            enemies[index] = EnemyTemplate(
                flags=_value(fields, "flags", 0, line_number),
                race=_value(fields, "race", 0, line_number),
                level=_value(fields, "level", 0, line_number),
                hp=_value(fields, "hp", 0, line_number),
                max_hp=_value(fields, "max_hp", 0, line_number),
                mp=_value(fields, "mp", 0, line_number),
                max_mp=_value(fields, "max_mp", 0, line_number),
                growth_profile=_value(fields, "growth", 0, line_number),
                unknown_0f=_value(fields, "unknown_0f", 0, line_number),
                stats=_sized_list(fields, "stats", 5, line_number),
                summon_category=_value(fields, "summon_category", 0, line_number),
                unknown_16_17=_bytes_field(fields, "unknown_16_17", 2, line_number),
                skills=_sized_list(fields, "skills", 8, line_number),
                macca=_value(fields, "macca", 0, line_number),
                experience=_value(fields, "experience", 0, line_number),
                atma_points=_value(fields, "atma_points", 0, line_number),
                atma_bonus=_value(fields, "atma_bonus", 0, line_number),
                unknown_34_3d=_bytes_field(fields, "unknown_34_3d", 10, line_number),
                drop_items=_sized_list(fields, "drop_items", 2, line_number),
                drop_rates=_sized_list(fields, "drop_rates", 2, line_number),
                conditional_drop_flag=conditional[0],
                conditional_drop_item=conditional[1],
                conditional_drop_rate=conditional[2],
                attack_attribute=_value(fields, "attack_attribute", 0, line_number),
                attack_repeats=_value(fields, "attack_repeats", 0, line_number),
                result_parameter=_value(fields, "result_parameter", 0, line_number),
                tail=_bytes_field(fields, "tail", 3, line_number),
            )
        else:
            raise BattleTableError(f"line {line_number}: unknown directive {directive!r}")

    result = UnitTable(
        profile,
        tuple(party),
        tuple(party_affinities),
        tuple(alternate_affinities),
        tuple(enemies),
        tuple(enemy_affinities),
    )
    encode_unit(result)
    return result


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


def _append_hex(fields: list[str], key: str, value: int, default: int = 0) -> None:
    if value != default:
        fields.append(f"{key}={value:#x}")


def _append_bytes(fields: list[str], key: str, value: bytes) -> None:
    if any(value):
        fields.append(f"{key}={value.hex()}")


def _packed_list(values: tuple[int, ...]) -> str:
    return ",".join(str(value) if value <= 0xFFFF else f"{value:#x}" for value in values)


def render_unit_source(table: UnitTable) -> str:
    """Render compact canonical UNIT source relative to the selected profile."""

    encode_unit(table)
    lines = [f"battle-table 1 kind=unit profile={table.profile.name}", ""]
    empty_party = PartyTemplate(tail=bytes(table.profile.party_size - 0x56))
    for index, row in enumerate(table.party):
        if row == empty_party:
            continue
        fields: list[str] = []
        _append_hex(fields, "flags", row.flags)
        _append(fields, "affinity_source", row.affinity_source)
        _append(fields, "unit", row.unit_id)
        _append(fields, "hp", row.hp)
        _append(fields, "max_hp", row.max_hp)
        _append(fields, "mp", row.mp)
        _append(fields, "max_mp", row.max_mp)
        _append_hex(fields, "status", row.status)
        _append(fields, "experience", row.experience)
        _append(fields, "level", row.level)
        if any(row.stats):
            fields.append(f"stats={_list(row.stats)}")
        _append_bytes(fields, "unknown_1b_21", row.unknown_1b_21)
        skills = _trimmed(row.skills)
        if skills:
            fields.append(f"skills={_list(skills)}")
        _append(fields, "bullet", row.equipped_bullet)
        _append(fields, "unknown_54", row.unknown_54)
        _append(fields, "current_profile", row.current_profile)
        _append_bytes(fields, "tail", row.tail)
        lines.append(f"party {index} {' '.join(fields)}")
    lines.append("")

    affinity_families = (
        ("party-affinity", table.party_affinities),
        ("alternate-affinity", table.alternate_affinities),
    )
    for directive, rows in affinity_families:
        for index, row in enumerate(rows):
            if row == AffinityRow():
                continue
            values = _trimmed(row.values)
            lines.append(f"{directive} {index} values={_packed_list(values)}")
        lines.append("")

    for index, row in enumerate(table.enemies):
        if row != EnemyTemplate():
            fields = []
            _append_hex(fields, "flags", row.flags)
            _append(fields, "race", row.race)
            _append(fields, "level", row.level)
            _append(fields, "hp", row.hp)
            _append(fields, "max_hp", row.max_hp)
            _append(fields, "mp", row.mp)
            _append(fields, "max_mp", row.max_mp)
            _append(fields, "growth", row.growth_profile)
            _append(fields, "unknown_0f", row.unknown_0f)
            if any(row.stats):
                fields.append(f"stats={_list(row.stats)}")
            _append(fields, "summon_category", row.summon_category)
            _append_bytes(fields, "unknown_16_17", row.unknown_16_17)
            skills = _trimmed(row.skills)
            if skills:
                fields.append(f"skills={_list(skills)}")
            _append(fields, "macca", row.macca)
            _append(fields, "experience", row.experience)
            _append(fields, "atma_points", row.atma_points)
            _append(fields, "atma_bonus", row.atma_bonus)
            _append_bytes(fields, "unknown_34_3d", row.unknown_34_3d)
            if any(row.drop_items):
                fields.append(f"drop_items={_list(row.drop_items)}")
            if any(row.drop_rates):
                fields.append(f"drop_rates={_list(row.drop_rates)}")
            conditional = (
                row.conditional_drop_flag,
                row.conditional_drop_item,
                row.conditional_drop_rate,
            )
            if any(conditional):
                fields.append(f"conditional_drop={_list(conditional)}")
            _append(fields, "attack_attribute", row.attack_attribute)
            _append(fields, "attack_repeats", row.attack_repeats)
            _append(fields, "result_parameter", row.result_parameter)
            _append_bytes(fields, "tail", row.tail)
            lines.append(f"enemy {index} {' '.join(fields)}")
        affinity = table.enemy_affinities[index]
        if affinity != AffinityRow():
            values = _trimmed(affinity.values)
            lines.append(f"enemy-affinity {index} values={_packed_list(values)}")
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
    data = _read(args.input)
    segment_count = len(_split_segments(data))
    if segment_count == 6:
        source = render_encount_source(decode_encount(data))
    elif segment_count == 5:
        source = render_unit_source(decode_unit(data))
    else:
        raise BattleTableError(
            f"unsupported battle table with {segment_count} segments"
        )
    _write_text(args.output, source)


def _command_assemble(args: argparse.Namespace) -> None:
    try:
        source = args.input.read_text(encoding="utf-8")
    except OSError as exc:
        raise BattleTableError(f"cannot read {args.input}: {exc}") from exc
    meaningful = [
        (number, line.strip())
        for number, line in enumerate(source.splitlines(), 1)
        if line.strip() and not line.lstrip().startswith("#")
    ]
    if not meaningful:
        raise BattleTableError("empty battle table source")
    line_number, line = meaningful[0]
    tokens = _tokens(line, line_number)
    if len(tokens) != 4 or tokens[:2] != ["battle-table", "1"]:
        raise BattleTableError("invalid battle table source header")
    header = _fields(tokens[2:], line_number, {"kind", "profile"}, "header")
    if header.get("kind") == "encounter":
        data = encode_encount(parse_encount_source(source))
    elif header.get("kind") == "unit":
        data = encode_unit(parse_unit_source(source))
    else:
        raise BattleTableError(f"unsupported battle table kind {header.get('kind')!r}")
    _write_bytes(args.output, data)


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
