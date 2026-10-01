#!/usr/bin/env python3
"""Disassemble and assemble DDS battle table (``.TBL``) data."""

from __future__ import annotations

import argparse
import json
import math
import shlex
import struct
import sys
from dataclasses import dataclass, replace
from pathlib import Path

import flw0
import flw0_symbolic
import msg1


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


@dataclass(frozen=True)
class SkillProfile:
    name: str
    action_attr_count: int
    action_count: int
    requirement_count: int
    coefficient_count: int
    item_count: int
    bonus_count: int
    group_count: int
    group_width: int

    @property
    def requirement_start(self) -> int:
        return 0x1AB


SKILL_PROFILES = {
    profile.name: profile
    for profile in (
        SkillProfile("dds1", 608, 512, 85, 192, 192, 0, 16, 16),
        SkillProfile("dds2", 672, 544, 117, 256, 256, 64, 48, 24),
    )
}


@dataclass(frozen=True)
class SkillActionAttribute:
    action_attribute: int = 0
    auxiliary: int = 0


@dataclass(frozen=True)
class SkillAction:
    flags: int = 0
    use: int = 0
    effect_type: int = 0
    cost_type: int = 0
    cost: int = 0
    cost_base: int = 0
    target_type: int = 0
    target_area: int = 0
    target_rule: int = 0
    target_random: int = 0
    untargetable_status: int = 0
    target_program: int = 0
    hit_type: int = 0
    hit_level: int = 0
    hit_program: int = 0
    hits_min: int = 0
    hits_max: int = 0
    hp_type: int = 0
    hp_power: int = 0
    mp_type: int = 0
    mp_power: int = 0
    hp_base: int = 0
    mp_base: int = 0
    effect_percent: int = 0
    ailment_type: int = 0
    ailment_level: int = 0
    base_status: int = 0
    support_type: int = 0
    support_points: int = 0
    death_type: int = 0
    lookup_id: int = 0
    program: int = 0
    magic_base: int = 0
    magic_limit: int = 0


@dataclass(frozen=True)
class SkillRequirement:
    conditions: tuple[int, ...] = (0, 0, 0)
    count: int = 0
    flags: int = 0


@dataclass(frozen=True)
class PartySkillDefaults:
    base_value: int = 0
    value_02: int = 0
    multiplier_bits: int = 0
    flags_08: int = 0
    flags_0a: int = 0
    value_0c: int = 0
    repeat_min: int = 0
    repeat_max: int = 0
    value_10: int = 0
    value_12: int = 0


@dataclass(frozen=True)
class SkillItemEntry:
    flags: int = 0
    item_id: int = 0
    value: int = 0
    auxiliary: int = 0


@dataclass(frozen=True)
class ProfileBonus:
    stats: tuple[int, ...] = (0,) * 5
    tier: int = 0


@dataclass(frozen=True)
class SkillGroup:
    skills: tuple[int, ...] = ()


@dataclass(frozen=True)
class SkillTable:
    profile: SkillProfile
    action_attributes: tuple[SkillActionAttribute, ...]
    actions: tuple[SkillAction, ...]
    requirements: tuple[SkillRequirement, ...]
    coefficient_bits: tuple[int, ...]
    party_defaults: tuple[PartySkillDefaults, ...]
    items: tuple[SkillItemEntry, ...]
    profile_bonuses: tuple[ProfileBonus, ...]
    groups: tuple[SkillGroup, ...]


SKILL_ACTION_FORMAT = "<BBBBHHBBBBHHBBHBBHhHhhhHBBHIbBHIhh"


@dataclass(frozen=True)
class AiCalcProfile:
    name: str
    calculation_word_count: int
    weighted_table_count: int


AICALC_PROFILES = {
    profile.name: profile
    for profile in (
        AiCalcProfile("dds1", 0xA6C // 4, 0),
        AiCalcProfile("dds2", 0xC14 // 4, 32),
    )
}


@dataclass(frozen=True)
class AiOperation:
    selector: int = 0
    argument: int = 0


@dataclass(frozen=True)
class AiDecision:
    predicates: tuple[AiOperation, ...] = (AiOperation(),) * 3
    routes: tuple[int, ...] = (8,) * 8


@dataclass(frozen=True)
class AiChoice:
    weight: int = 0
    action: int = 0
    effect: AiOperation = AiOperation()


@dataclass(frozen=True)
class EnemyAi:
    flags: int = 0
    script_procedure: int = 0
    decisions: tuple[AiDecision, ...] = (AiDecision(),) * 3
    groups: tuple[tuple[AiChoice, ...], ...] = ((AiChoice(),) * 5,) * 7
    reserved: int = 0


@dataclass(frozen=True)
class WeightedValue:
    value: int = 0
    weight: int = 0


@dataclass(frozen=True)
class AiCalcTable:
    profile: AiCalcProfile
    enemies: tuple[EnemyAi, ...]
    calculation_words: tuple[int, ...]
    weighted_tables: tuple[tuple[WeightedValue, ...], ...]
    ai_script: bytes
    formula_script: bytes


@dataclass(frozen=True)
class MessageTextProfile:
    directive: str
    width: int
    count: int


@dataclass(frozen=True)
class MessageProfile:
    name: str
    text_profiles: tuple[MessageTextProfile, ...]
    bank_counts: tuple[int, int, int, int]


MESSAGE_PROFILES = {
    profile.name: profile
    for profile in (
        MessageProfile(
            "dds1",
            (
                MessageTextProfile("affinity-description", 45, 256),
                MessageTextProfile("tribe-name", 19, 98),
                MessageTextProfile("enemy-description", 189, 384),
                MessageTextProfile("enemy-name", 17, 384),
                MessageTextProfile("item-name", 25, 192),
                MessageTextProfile("actor-name", 17, 32),
                MessageTextProfile("race-name", 7, 16),
                MessageTextProfile("skill-name", 17, 624),
                MessageTextProfile("token-label", 17, 64),
                MessageTextProfile("reserved-label", 17, 256),
            ),
            (192, 607, 210, 9),
        ),
        MessageProfile(
            "dds2",
            (
                MessageTextProfile("affinity-description", 45, 256),
                MessageTextProfile("tribe-name", 19, 176),
                MessageTextProfile("enemy-description", 189, 384),
                MessageTextProfile("enemy-name", 17, 384),
                MessageTextProfile("item-name", 25, 256),
                MessageTextProfile("actor-name", 17, 32),
                MessageTextProfile("race-name", 7, 32),
                MessageTextProfile("skill-name", 17, 672),
                MessageTextProfile("skill-family-name", 33, 48),
            ),
            (256, 672, 254, 9),
        ),
    )
}

MESSAGE_BANK_KINDS = ("items", "skills", "status-help", "command-help")


@dataclass(frozen=True)
class MessageTable:
    profile: MessageProfile
    text_tables: tuple[tuple[str, ...], ...]
    message_banks: tuple[bytes, ...]


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


def _skill_profile_from_segments(segments: tuple[bytes, ...]) -> SkillProfile:
    for profile in SKILL_PROFILES.values():
        sizes = (
            profile.action_attr_count * 2,
            profile.action_count * 0x38,
            profile.requirement_count * 0x10,
            profile.coefficient_count * 4,
            16 * 0x14,
            profile.item_count * 8,
        )
        if profile.bonus_count:
            sizes += (profile.bonus_count * 6,)
        sizes += (profile.group_count * profile.group_width * 2,)
        if tuple(map(len, segments)) == sizes:
            return profile
    sizes = ", ".join(f"{len(segment):#x}" for segment in segments)
    raise BattleTableError(f"unsupported SKILL segment sizes: {sizes}")


def decode_skill(data: bytes) -> SkillTable:
    """Decode and validate a retail DDS1 or DDS2 ``SKILL.TBL``."""

    segments = _split_segments(data)
    profile = _skill_profile_from_segments(segments)
    action_attributes = tuple(
        SkillActionAttribute(*struct.unpack_from("<bb", segments[0], offset))
        for offset in range(0, len(segments[0]), 2)
    )
    actions = tuple(
        SkillAction(*struct.unpack_from(SKILL_ACTION_FORMAT, segments[1], offset))
        for offset in range(0, len(segments[1]), 0x38)
    )
    requirements = tuple(
        SkillRequirement(
            struct.unpack_from("<3I", segments[2], offset),
            struct.unpack_from("<H", segments[2], offset + 0xC)[0],
            struct.unpack_from("<H", segments[2], offset + 0xE)[0],
        )
        for offset in range(0, len(segments[2]), 0x10)
    )
    coefficient_bits = struct.unpack(f"<{profile.coefficient_count}I", segments[3])
    party_defaults = tuple(
        PartySkillDefaults(*struct.unpack_from("<HHIHHHBBHH", segments[4], offset))
        for offset in range(0, len(segments[4]), 0x14)
    )
    items = tuple(
        SkillItemEntry(*struct.unpack_from("<4H", segments[5], offset))
        for offset in range(0, len(segments[5]), 8)
    )
    segment_index = 6
    if profile.bonus_count:
        profile_bonuses = tuple(
            ProfileBonus(tuple(struct.unpack_from("<5b", segments[segment_index], offset)),
                         struct.unpack_from("<b", segments[segment_index], offset + 5)[0])
            for offset in range(0, len(segments[segment_index]), 6)
        )
        segment_index += 1
    else:
        profile_bonuses = ()

    groups = []
    group_segment = segments[segment_index]
    for index in range(profile.group_count):
        values = struct.unpack_from(
            f"<{profile.group_width}h",
            group_segment,
            index * profile.group_width * 2,
        )
        try:
            end = values.index(-1)
        except ValueError:
            end = len(values)
        if any(value != -1 for value in values[end:]):
            raise BattleTableError(f"skill group {index} has data after its terminator")
        groups.append(SkillGroup(tuple(values[:end])))

    return SkillTable(
        profile,
        action_attributes,
        actions,
        requirements,
        coefficient_bits,
        party_defaults,
        items,
        profile_bonuses,
        tuple(groups),
    )


def default_skill(profile: SkillProfile) -> SkillTable:
    return SkillTable(
        profile,
        (SkillActionAttribute(),) * profile.action_attr_count,
        (SkillAction(),) * profile.action_count,
        (SkillRequirement(),) * profile.requirement_count,
        (0,) * profile.coefficient_count,
        (PartySkillDefaults(),) * 16,
        (SkillItemEntry(),) * profile.item_count,
        (ProfileBonus(),) * profile.bonus_count,
        (SkillGroup(),) * profile.group_count,
    )


def encode_skill(table: SkillTable) -> bytes:
    """Encode one SKILL model to its exact physical profile."""

    profile = SKILL_PROFILES.get(table.profile.name)
    if profile != table.profile:
        raise BattleTableError(f"unknown or modified SKILL profile {table.profile.name!r}")
    expected = (
        ("action attributes", len(table.action_attributes), profile.action_attr_count),
        ("actions", len(table.actions), profile.action_count),
        ("requirements", len(table.requirements), profile.requirement_count),
        ("coefficients", len(table.coefficient_bits), profile.coefficient_count),
        ("party defaults", len(table.party_defaults), 16),
        ("items", len(table.items), profile.item_count),
        ("profile bonuses", len(table.profile_bonuses), profile.bonus_count),
        ("groups", len(table.groups), profile.group_count),
    )
    for name, actual, wanted in expected:
        if actual != wanted:
            raise BattleTableError(f"SKILL needs {wanted} {name}, found {actual}")
    validate_skill_references(table)

    action_attributes = bytearray(profile.action_attr_count * 2)
    for index, row in enumerate(table.action_attributes):
        struct.pack_into(
            "<bb", action_attributes, index * 2,
            _s8(row.action_attribute, f"action attribute {index}"),
            _s8(row.auxiliary, f"action attribute {index} auxiliary"),
        )

    action_data = bytearray(profile.action_count * 0x38)
    for index, row in enumerate(table.actions):
        context = f"action {index}"
        struct.pack_into(
            SKILL_ACTION_FORMAT, action_data, index * 0x38,
            _u8(row.flags, f"{context} flags"),
            _u8(row.use, f"{context} use"),
            _u8(row.effect_type, f"{context} effect_type"),
            _u8(row.cost_type, f"{context} cost_type"),
            _u16(row.cost, f"{context} cost"),
            _u16(row.cost_base, f"{context} cost_base"),
            _u8(row.target_type, f"{context} target_type"),
            _u8(row.target_area, f"{context} target_area"),
            _u8(row.target_rule, f"{context} target_rule"),
            _u8(row.target_random, f"{context} target_random"),
            _u16(row.untargetable_status, f"{context} untargetable_status"),
            _u16(row.target_program, f"{context} target_program"),
            _u8(row.hit_type, f"{context} hit_type"),
            _u8(row.hit_level, f"{context} hit_level"),
            _u16(row.hit_program, f"{context} hit_program"),
            _u8(row.hits_min, f"{context} hits_min"),
            _u8(row.hits_max, f"{context} hits_max"),
            _u16(row.hp_type, f"{context} hp_type"),
            _s16(row.hp_power, f"{context} hp_power"),
            _u16(row.mp_type, f"{context} mp_type"),
            _s16(row.mp_power, f"{context} mp_power"),
            _s16(row.hp_base, f"{context} hp_base"),
            _s16(row.mp_base, f"{context} mp_base"),
            _u16(row.effect_percent, f"{context} effect_percent"),
            _u8(row.ailment_type, f"{context} ailment_type"),
            _u8(row.ailment_level, f"{context} ailment_level"),
            _u16(row.base_status, f"{context} base_status"),
            _u32(row.support_type, f"{context} support_type"),
            _s8(row.support_points, f"{context} support_points"),
            _u8(row.death_type, f"{context} death_type"),
            _u16(row.lookup_id, f"{context} lookup_id"),
            _u32(row.program, f"{context} program"),
            _s16(row.magic_base, f"{context} magic_base"),
            _s16(row.magic_limit, f"{context} magic_limit"),
        )

    requirement_data = bytearray(profile.requirement_count * 0x10)
    for index, row in enumerate(table.requirements):
        context = f"requirement {profile.requirement_start + index:#x}"
        if len(row.conditions) != 3:
            raise BattleTableError(f"{context} needs three conditions")
        struct.pack_into(
            "<3IHH", requirement_data, index * 0x10,
            *(_u32(value, f"{context} condition") for value in row.conditions),
            _u16(row.count, f"{context} count"),
            _u16(row.flags, f"{context} flags"),
        )

    coefficient_data = struct.pack(
        f"<{profile.coefficient_count}I",
        *(_u32(value, "coefficient bits") for value in table.coefficient_bits),
    )
    party_data = bytearray(16 * 0x14)
    for index, row in enumerate(table.party_defaults):
        context = f"party defaults {index}"
        struct.pack_into(
            "<HHIHHHBBHH", party_data, index * 0x14,
            _u16(row.base_value, f"{context} base_value"),
            _u16(row.value_02, f"{context} value_02"),
            _u32(row.multiplier_bits, f"{context} multiplier"),
            _u16(row.flags_08, f"{context} flags_08"),
            _u16(row.flags_0a, f"{context} flags_0a"),
            _u16(row.value_0c, f"{context} value_0c"),
            _u8(row.repeat_min, f"{context} repeat_min"),
            _u8(row.repeat_max, f"{context} repeat_max"),
            _u16(row.value_10, f"{context} value_10"),
            _u16(row.value_12, f"{context} value_12"),
        )

    item_data = bytearray(profile.item_count * 8)
    for index, row in enumerate(table.items):
        context = f"item entry {index}"
        struct.pack_into(
            "<4H", item_data, index * 8,
            _u16(row.flags, f"{context} flags"),
            _u16(row.item_id, f"{context} item_id"),
            _u16(row.value, f"{context} value"),
            _u16(row.auxiliary, f"{context} auxiliary"),
        )

    segments: list[bytes] = [
        bytes(action_attributes), bytes(action_data), bytes(requirement_data),
        coefficient_data, bytes(party_data), bytes(item_data),
    ]
    if profile.bonus_count:
        bonus_data = bytearray(profile.bonus_count * 6)
        for index, row in enumerate(table.profile_bonuses):
            if len(row.stats) != 5:
                raise BattleTableError(f"profile bonus {index} needs five stats")
            struct.pack_into(
                "<6b", bonus_data, index * 6,
                *(_s8(value, f"profile bonus {index} stat") for value in row.stats),
                _s8(row.tier, f"profile bonus {index} tier"),
            )
        segments.append(bytes(bonus_data))

    group_data = bytearray(profile.group_count * profile.group_width * 2)
    for index, row in enumerate(table.groups):
        if len(row.skills) > profile.group_width:
            raise BattleTableError(
                f"skill group {index} has more than {profile.group_width} skills"
            )
        values = tuple(
            _range(value, 0, 0x7FFF, f"skill group {index} skill")
            for value in row.skills
        ) + (-1,) * (profile.group_width - len(row.skills))
        struct.pack_into(
            f"<{profile.group_width}h", group_data,
            index * profile.group_width * 2, *values,
        )
    segments.append(bytes(group_data))
    return _join_segments(tuple(segments))


def _aicalc_profile_from_segments(segments: tuple[bytes, ...]) -> AiCalcProfile:
    for profile in AICALC_PROFILES.values():
        prefix = [0x20A00, profile.calculation_word_count * 4]
        if profile.weighted_table_count:
            prefix.append(profile.weighted_table_count * 0x20)
        if (
            len(segments) == len(prefix) + 2
            and list(map(len, segments[: len(prefix)])) == prefix
            and all(segment[8:12] == b"FLW0" for segment in segments[-2:])
        ):
            return profile
    sizes = ", ".join(f"{len(segment):#x}" for segment in segments)
    raise BattleTableError(f"unsupported AICALC segment sizes: {sizes}")


def _decode_ai_operation(value: int) -> AiOperation:
    return AiOperation(value >> 22, value & 0x3FFFFF)


def _encode_ai_operation(operation: AiOperation, context: str) -> int:
    selector = _range(operation.selector, 0, 0x3FF, f"{context} selector")
    argument = _range(operation.argument, 0, 0x3FFFFF, f"{context} argument")
    return selector << 22 | argument


def decode_aicalc(data: bytes) -> AiCalcTable:
    """Decode and validate a retail DDS1 or DDS2 ``AICALC.TBL``."""

    segments = _split_segments(data)
    profile = _aicalc_profile_from_segments(segments)
    enemies = []
    for enemy_id in range(384):
        row = segments[0][enemy_id * 0x15C : (enemy_id + 1) * 0x15C]
        decisions = []
        for decision_index in range(3):
            offset = 4 + decision_index * 0x14
            decisions.append(
                AiDecision(
                    tuple(
                        _decode_ai_operation(value)
                        for value in struct.unpack_from("<3I", row, offset)
                    ),
                    tuple(row[offset + 0xC : offset + 0x14]),
                )
            )
        groups = []
        for group_index in range(7):
            choices = []
            for choice_index in range(5):
                offset = 0x40 + (group_index * 5 + choice_index) * 8
                weight, action, effect = struct.unpack_from("<HHI", row, offset)
                choices.append(AiChoice(weight, action, _decode_ai_operation(effect)))
            groups.append(tuple(choices))
        enemies.append(
            EnemyAi(
                *struct.unpack_from("<HH", row),
                tuple(decisions),
                tuple(groups),
                struct.unpack_from("<I", row, 0x158)[0],
            )
        )

    calculation_words = struct.unpack(
        f"<{profile.calculation_word_count}I", segments[1]
    )
    segment_index = 2
    weighted_tables = []
    if profile.weighted_table_count:
        for table_index in range(profile.weighted_table_count):
            offset = table_index * 0x20
            weighted_tables.append(
                tuple(
                    WeightedValue(*struct.unpack_from("<HH", segments[2], offset + i * 4))
                    for i in range(8)
                )
            )
        segment_index += 1

    ai_script = segments[segment_index]
    formula_script = segments[segment_index + 1]
    try:
        flw0.parse(ai_script)
        flw0.parse(formula_script)
    except flw0.Flw0Error as exc:
        raise BattleTableError(f"invalid embedded AICALC script: {exc}") from exc
    return AiCalcTable(
        profile,
        tuple(enemies),
        calculation_words,
        tuple(weighted_tables),
        ai_script,
        formula_script,
    )


def encode_aicalc(table: AiCalcTable) -> bytes:
    """Encode one AICALC model to its exact physical profile."""

    profile = AICALC_PROFILES.get(table.profile.name)
    if profile != table.profile:
        raise BattleTableError(f"unknown or modified AICALC profile {table.profile.name!r}")
    if len(table.enemies) != 384:
        raise BattleTableError(f"AICALC needs 384 enemy rows, found {len(table.enemies)}")
    if len(table.calculation_words) != profile.calculation_word_count:
        raise BattleTableError(
            f"AICALC needs {profile.calculation_word_count} calculation words, "
            f"found {len(table.calculation_words)}"
        )
    if len(table.weighted_tables) != profile.weighted_table_count:
        raise BattleTableError(
            f"AICALC needs {profile.weighted_table_count} weighted tables, "
            f"found {len(table.weighted_tables)}"
        )
    validate_aicalc_references(table)

    enemy_data = bytearray(384 * 0x15C)
    for enemy_id, enemy in enumerate(table.enemies):
        context = f"enemy AI {enemy_id}"
        if len(enemy.decisions) != 3:
            raise BattleTableError(f"{context} needs three decisions")
        if len(enemy.groups) != 7 or any(len(group) != 5 for group in enemy.groups):
            raise BattleTableError(f"{context} needs seven groups of five choices")
        row_offset = enemy_id * 0x15C
        struct.pack_into(
            "<HH",
            enemy_data,
            row_offset,
            _u16(enemy.flags, f"{context} flags"),
            _u16(enemy.script_procedure, f"{context} script procedure"),
        )
        for decision_index, decision in enumerate(enemy.decisions):
            if len(decision.predicates) != 3 or len(decision.routes) != 8:
                raise BattleTableError(f"{context} decision {decision_index} has invalid shape")
            offset = row_offset + 4 + decision_index * 0x14
            struct.pack_into(
                "<3I",
                enemy_data,
                offset,
                *(
                    _encode_ai_operation(
                        operation, f"{context} decision {decision_index} predicate"
                    )
                    for operation in decision.predicates
                ),
            )
            enemy_data[offset + 0xC : offset + 0x14] = bytes(
                _u8(route, f"{context} decision {decision_index} route")
                for route in decision.routes
            )
        for group_index, group in enumerate(enemy.groups):
            for choice_index, choice in enumerate(group):
                offset = row_offset + 0x40 + (group_index * 5 + choice_index) * 8
                choice_context = f"{context} group {group_index} choice {choice_index}"
                struct.pack_into(
                    "<HHI",
                    enemy_data,
                    offset,
                    _u16(choice.weight, f"{choice_context} weight"),
                    _u16(choice.action, f"{choice_context} action"),
                    _encode_ai_operation(choice.effect, f"{choice_context} effect"),
                )
        struct.pack_into(
            "<I",
            enemy_data,
            row_offset + 0x158,
            _u32(enemy.reserved, f"{context} reserved"),
        )

    calculation_data = struct.pack(
        f"<{profile.calculation_word_count}I",
        *(_u32(value, "AICALC calculation word") for value in table.calculation_words),
    )
    segments: list[bytes] = [bytes(enemy_data), calculation_data]
    if profile.weighted_table_count:
        weighted_data = bytearray(profile.weighted_table_count * 0x20)
        for table_index, weighted_table in enumerate(table.weighted_tables):
            if len(weighted_table) != 8:
                raise BattleTableError(f"weighted table {table_index} needs eight entries")
            for entry_index, entry in enumerate(weighted_table):
                struct.pack_into(
                    "<HH",
                    weighted_data,
                    table_index * 0x20 + entry_index * 4,
                    _u16(entry.value, f"weighted table {table_index} value"),
                    _u16(entry.weight, f"weighted table {table_index} weight"),
                )
        segments.append(bytes(weighted_data))

    scripts = []
    for name, script in (("AI", table.ai_script), ("formula", table.formula_script)):
        try:
            scripts.append(flw0.parse(script).to_bytes())
        except flw0.Flw0Error as exc:
            raise BattleTableError(f"invalid {name} script: {exc}") from exc
    segments.extend(scripts)
    return _join_segments(tuple(segments))


def validate_aicalc_references(table: AiCalcTable, skill: SkillTable | None = None) -> None:
    """Validate AI procedure, skill, and weighted-table references."""

    if skill is not None and table.profile.name != skill.profile.name:
        raise BattleTableError(
            f"cannot join {table.profile.name} AICALC with {skill.profile.name} SKILL"
        )
    try:
        procedure_count = len(flw0.parse(table.ai_script).named_rows(0))
    except flw0.Flw0Error as exc:
        raise BattleTableError(f"invalid AI script: {exc}") from exc
    for enemy_id, enemy in enumerate(table.enemies):
        if enemy.script_procedure >= procedure_count:
            raise BattleTableError(
                f"enemy AI {enemy_id} references procedure {enemy.script_procedure} "
                f"outside the AI script"
            )
        for group_index, group in enumerate(enemy.groups):
            for choice_index, choice in enumerate(group):
                if choice.weight == 0:
                    continue
                if choice.action < 0x1000 and skill is not None:
                    if choice.action >= len(skill.action_attributes):
                        raise BattleTableError(
                            f"enemy AI {enemy_id} group {group_index} choice {choice_index} "
                            f"references skill {choice.action} outside SKILL"
                        )
                elif choice.action & 0xF000 == 0x7000:
                    weighted = choice.action & 0xFFF
                    if weighted >= len(table.weighted_tables):
                        raise BattleTableError(
                            f"enemy AI {enemy_id} group {group_index} choice {choice_index} "
                            f"references weighted table {weighted} outside AICALC"
                        )


def _message_profile_from_segments(segments: tuple[bytes, ...]) -> MessageProfile:
    for profile in MESSAGE_PROFILES.values():
        fixed_sizes = tuple(row.width * row.count for row in profile.text_profiles)
        if len(segments) != len(fixed_sizes) + len(MESSAGE_BANK_KINDS):
            continue
        if tuple(map(len, segments[: len(fixed_sizes)])) == fixed_sizes:
            return profile
    sizes = ", ".join(f"{len(segment):#x}" for segment in segments)
    raise BattleTableError(f"unsupported MSG segment sizes: {sizes}")


def _decode_text_table(data: bytes, row: MessageTextProfile) -> tuple[str, ...]:
    values: list[str] = []
    for index in range(row.count):
        value = data[index * row.width : (index + 1) * row.width]
        text, separator, padding = value.partition(b"\0")
        if not separator:
            raise BattleTableError(
                f"{row.directive} {index} has no terminator in its {row.width}-byte row"
            )
        if any(padding):
            raise BattleTableError(f"{row.directive} {index} has nonzero padding")
        try:
            values.append(text.decode("ascii"))
        except UnicodeDecodeError as exc:
            raise BattleTableError(f"{row.directive} {index} is not ASCII") from exc
    return tuple(values)


def _encode_text_table(values: tuple[str, ...], row: MessageTextProfile) -> bytes:
    if len(values) != row.count:
        raise BattleTableError(
            f"{row.directive} needs {row.count} rows, found {len(values)}"
        )
    output = bytearray()
    for index, text in enumerate(values):
        try:
            value = text.encode("ascii")
        except UnicodeEncodeError as exc:
            raise BattleTableError(f"{row.directive} {index} is not ASCII") from exc
        if len(value) >= row.width:
            raise BattleTableError(
                f"{row.directive} {index} needs {len(value) + 1} bytes, "
                f"but its row is {row.width} bytes"
            )
        output.extend(value)
        output.extend(bytes(row.width - len(value)))
    return bytes(output)


def decode_message(data: bytes) -> MessageTable:
    """Decode and validate a retail DDS1 or DDS2 ``MSG.TBL``."""

    segments = _split_segments(data)
    profile = _message_profile_from_segments(segments)
    fixed_count = len(profile.text_profiles)
    text_tables = tuple(
        _decode_text_table(segment, row)
        for segment, row in zip(segments[:fixed_count], profile.text_profiles)
    )
    banks = tuple(segments[fixed_count:])
    for kind, expected, bank in zip(MESSAGE_BANK_KINDS, profile.bank_counts, banks):
        try:
            decoded = msg1.decode(bank)
        except msg1.Msg1Error as exc:
            raise BattleTableError(f"invalid {kind} message bank: {exc}") from exc
        if len(decoded.dialogs) != expected:
            raise BattleTableError(
                f"{kind} message bank needs {expected} dialogs, "
                f"found {len(decoded.dialogs)}"
            )
    return MessageTable(profile, text_tables, banks)


def encode_message(table: MessageTable) -> bytes:
    """Encode one message table to its exact physical profile."""

    profile = MESSAGE_PROFILES.get(table.profile.name)
    if profile != table.profile:
        raise BattleTableError(f"unknown or modified MSG profile {table.profile.name!r}")
    if len(table.text_tables) != len(profile.text_profiles):
        raise BattleTableError(
            f"{profile.name} MSG needs {len(profile.text_profiles)} text tables"
        )
    if len(table.message_banks) != len(MESSAGE_BANK_KINDS):
        raise BattleTableError("MSG needs four message banks")
    segments = [
        _encode_text_table(values, row)
        for values, row in zip(table.text_tables, profile.text_profiles)
    ]
    for kind, expected, bank in zip(
        MESSAGE_BANK_KINDS, profile.bank_counts, table.message_banks
    ):
        try:
            decoded = msg1.decode(bank)
            canonical = msg1.encode(decoded)
        except msg1.Msg1Error as exc:
            raise BattleTableError(f"invalid {kind} message bank: {exc}") from exc
        if canonical != bank:
            raise BattleTableError(f"{kind} message bank is not canonical")
        if len(decoded.dialogs) != expected:
            raise BattleTableError(f"{kind} message bank needs {expected} dialogs")
        segments.append(bank)
    return _join_segments(tuple(segments))


def validate_message_references(
    table: MessageTable,
    unit: UnitTable | None = None,
    skill: SkillTable | None = None,
) -> None:
    """Validate message ID domains against the paired UNIT and SKILL tables."""

    rows = {
        profile.directive: values
        for profile, values in zip(table.profile.text_profiles, table.text_tables)
    }
    if unit is not None:
        if table.profile.name != unit.profile.name:
            raise BattleTableError(
                f"cannot join {table.profile.name} MSG with {unit.profile.name} UNIT"
            )
        if len(rows["enemy-name"]) != len(unit.enemies):
            raise BattleTableError("enemy-name rows do not cover the UNIT enemy domain")
        if len(rows["enemy-description"]) != len(unit.enemies):
            raise BattleTableError(
                "enemy-description rows do not cover the UNIT enemy domain"
            )
    if skill is not None:
        if table.profile.name != skill.profile.name:
            raise BattleTableError(
                f"cannot join {table.profile.name} MSG with {skill.profile.name} SKILL"
            )
        if len(rows["skill-name"]) < len(skill.action_attributes):
            raise BattleTableError("skill-name rows do not cover the SKILL ID domain")
        if len(rows["item-name"]) != len(skill.items):
            raise BattleTableError("item-name rows do not cover the SKILL item domain")


def validate_encount_unit(encount: EncountTable, unit: UnitTable) -> None:
    """Validate encounter enemy IDs against the paired UNIT profile."""

    if encount.profile.name != unit.profile.name:
        raise BattleTableError(
            f"cannot join {encount.profile.name} ENCOUNT with {unit.profile.name} UNIT"
        )
    for encounter_index, encounter in enumerate(encount.encounters):
        for slot_index, enemy_id in enumerate(encounter.enemies):
            if enemy_id == 0:
                continue
            context = f"encounter {encounter_index} enemy slot {slot_index}"
            if enemy_id >= len(unit.enemies):
                raise BattleTableError(f"{context} references enemy {enemy_id} outside UNIT")
            if unit.enemies[enemy_id] == EnemyTemplate():
                raise BattleTableError(f"{context} references empty enemy {enemy_id}")


def validate_skill_references(skill: SkillTable) -> None:
    """Validate skill IDs, requirement counts, and condition-group references."""

    for index, row in enumerate(skill.requirements):
        skill_id = skill.profile.requirement_start + index
        if not 0 <= row.count <= 3:
            raise BattleTableError(f"requirement {skill_id:#x} count is outside 0..3")
        for condition_index, condition in enumerate(row.conditions):
            if condition == 0xFFFFFFFF:
                continue
            kind = condition & 0xF0000000
            value = condition & 0x0FFFFFFF
            if kind == 0 and condition and value >= len(skill.action_attributes):
                raise BattleTableError(
                    f"requirement {skill_id:#x} condition {condition_index} "
                    f"references skill {value} outside SKILL"
                )
            if kind == 0x40000000:
                group = value
                if group >= len(skill.groups):
                    raise BattleTableError(
                        f"requirement {skill_id:#x} condition {condition_index} "
                        f"references group {group} outside SKILL"
                    )
    for group_index, group in enumerate(skill.groups):
        for member_index, skill_id in enumerate(group.skills):
            if skill_id >= len(skill.action_attributes):
                raise BattleTableError(
                    f"skill group {group_index} member {member_index} references "
                    f"skill {skill_id} outside SKILL"
                )


def validate_unit_skill(unit: UnitTable, skill: SkillTable) -> None:
    """Validate UNIT skill IDs against the paired SKILL ID table."""

    if unit.profile.name != skill.profile.name:
        raise BattleTableError(
            f"cannot join {unit.profile.name} UNIT with {skill.profile.name} SKILL"
        )
    for family, rows in (("party", unit.party), ("enemy", unit.enemies)):
        for row_index, row in enumerate(rows):
            for slot_index, skill_id in enumerate(row.skills):
                if skill_id >= len(skill.action_attributes):
                    raise BattleTableError(
                        f"{family} {row_index} skill slot {slot_index} references "
                        f"skill {skill_id} outside SKILL"
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


def _condition(text: str, line_number: int) -> int:
    if text == "any":
        return 0
    if text == "none":
        return 0xFFFFFFFF
    tags = {
        "skill": 0,
        "attribute-mask": 0x10000000,
        "unit-mask": 0x20000000,
        "group": 0x40000000,
    }
    if ":" in text:
        tag, payload = text.split(":", 1)
        if tag not in tags:
            raise BattleTableError(f"line {line_number}: unknown condition kind {tag!r}")
        value = _integer(payload, line_number, "condition value")
        if not 0 <= value <= 0x0FFFFFFF:
            raise BattleTableError(f"line {line_number}: condition value is too large")
        return tags[tag] | value
    return _integer(text, line_number, "condition")


def _condition_values(text: str, line_number: int) -> tuple[int, ...]:
    if not text:
        return ()
    return tuple(_condition(value, line_number) for value in text.split(","))


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


def _float_bits(text: str, line_number: int, context: str) -> int:
    try:
        value = float(text)
    except ValueError as exc:
        raise BattleTableError(f"line {line_number}: invalid {context} {text!r}") from exc
    if not math.isfinite(value):
        raise BattleTableError(f"line {line_number}: {context} must be finite")
    try:
        packed = struct.pack("<f", value)
    except OverflowError as exc:
        raise BattleTableError(f"line {line_number}: {context} is outside f32 range") from exc
    return struct.unpack("<I", packed)[0]


def _parse_bits_or_float(
    fields: dict[str, str], line_number: int, context: str
) -> int:
    present = {key for key in ("value", "bits") if key in fields}
    if len(present) != 1:
        raise BattleTableError(
            f"line {line_number}: {context} needs exactly one of value= or bits="
        )
    if "value" in fields:
        return _float_bits(fields["value"], line_number, context)
    return _integer(fields["bits"], line_number, context)


def _ai_operation(text: str, line_number: int, context: str) -> AiOperation:
    if text == "none":
        return AiOperation()
    if ":" not in text:
        raise BattleTableError(
            f"line {line_number}: {context} needs SELECTOR:ARGUMENT or none"
        )
    selector_text, argument_text = text.split(":", 1)
    return AiOperation(
        _integer(selector_text, line_number, f"{context} selector"),
        _integer(argument_text, line_number, f"{context} argument"),
    )


def _ai_operations(text: str, line_number: int) -> tuple[AiOperation, ...]:
    values = tuple(
        _ai_operation(value, line_number, "predicate") for value in text.split(",")
    )
    if len(values) != 3:
        raise BattleTableError(f"line {line_number}: predicates needs three values")
    return values


def _ai_action(text: str, line_number: int) -> int:
    if ":" not in text:
        return _integer(text, line_number, "AI action")
    parts = text.split(":")
    kind = parts[0]
    if kind == "skill" and len(parts) == 2:
        return _integer(parts[1], line_number, "skill action")
    if kind == "preset" and len(parts) == 3:
        preset = _integer(parts[1], line_number, "action preset")
        argument = _integer(parts[2], line_number, "action preset argument")
        if not 1 <= preset <= 6 or not 0 <= argument <= 0xFFF:
            raise BattleTableError(f"line {line_number}: invalid action preset")
        return preset << 12 | argument
    if kind == "weighted" and len(parts) == 2:
        argument = _integer(parts[1], line_number, "weighted table")
        if not 0 <= argument <= 0xFFF:
            raise BattleTableError(f"line {line_number}: invalid weighted table")
        return 0x7000 | argument
    if kind == "special" and len(parts) == 2:
        argument = _integer(parts[1], line_number, "special action")
        if not 0 <= argument <= 0xFFF:
            raise BattleTableError(f"line {line_number}: invalid special action")
        return 0x8000 | argument
    raise BattleTableError(f"line {line_number}: invalid AI action {text!r}")


def _read_flw0_source(path: Path, kind: str) -> bytes:
    try:
        source = path.read_text(encoding="utf-8")
    except OSError as exc:
        raise BattleTableError(f"cannot read {kind} script {path}: {exc}") from exc
    try:
        return flw0.parse_source(source).to_bytes()
    except flw0.Flw0Error as exc:
        raise BattleTableError(f"invalid {kind} script {path}: {exc}") from exc


def _read_message_source(path: Path, kind: str) -> bytes:
    try:
        text = path.read_text(encoding="utf-8")
    except OSError as exc:
        raise BattleTableError(f"cannot read {kind} message source {path}: {exc}") from exc
    meaningful = [
        (number, line.strip())
        for number, line in enumerate(text.splitlines(), 1)
        if line.strip() and not line.lstrip().startswith("#")
    ]
    if not meaningful or _tokens(meaningful[0][1], meaningful[0][0]) != [
        "messages",
        "msg1",
    ]:
        raise BattleTableError(f"{kind} message source must begin with 'messages msg1'")
    try:
        return msg1.parse_source(meaningful[1:])
    except msg1.Msg1Error as exc:
        raise BattleTableError(f"invalid {kind} message source {path}: {exc}") from exc


def parse_message_source(text: str, source_dir: Path = Path(".")) -> MessageTable:
    """Assemble MSG source and its four sibling MSG1 source files."""

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
            "source must begin with 'battle-table 1 kind=message profile=PROFILE'"
        )
    header = _fields(first[2:], first_number, {"kind", "profile"}, "header")
    if header.get("kind") != "message":
        raise BattleTableError("MSG source needs kind=message")
    try:
        profile = MESSAGE_PROFILES[header["profile"]]
    except KeyError as exc:
        raise BattleTableError(f"unknown MSG profile {header.get('profile')!r}") from exc

    table_by_directive = {
        row.directive: (index, row)
        for index, row in enumerate(profile.text_profiles)
    }
    text_tables: list[list[str | None]] = [
        [None] * row.count for row in profile.text_profiles
    ]
    bank_files: dict[str, tuple[int, str]] = {}
    for line_number, line in meaningful[1:]:
        tokens = _tokens(line, line_number)
        directive = tokens[0]
        if directive == "message-bank":
            if len(tokens) < 3 or tokens[1] not in MESSAGE_BANK_KINDS:
                raise BattleTableError(
                    f"line {line_number}: expected message-bank KIND file=PATH"
                )
            fields = _fields(tokens[2:], line_number, {"file"}, "message bank")
            if set(fields) != {"file"}:
                raise BattleTableError(f"line {line_number}: message bank needs file=PATH")
            kind = tokens[1]
            if kind in bank_files:
                raise BattleTableError(f"line {line_number}: duplicate {kind} message bank")
            bank_files[kind] = (line_number, fields["file"])
            continue
        try:
            table_index, row = table_by_directive[directive]
        except KeyError as exc:
            raise BattleTableError(
                f"line {line_number}: unknown MSG directive {directive!r}"
            ) from exc
        if len(tokens) != 3:
            raise BattleTableError(
                f"line {line_number}: {directive} needs an index and quoted text"
            )
        row_index = _index(tokens[1], line_number, row.count, f"{directive} index")
        if text_tables[table_index][row_index] is not None:
            raise BattleTableError(f"line {line_number}: duplicate {directive} {row_index}")
        text_tables[table_index][row_index] = tokens[2]

    missing_banks = set(MESSAGE_BANK_KINDS) - set(bank_files)
    if missing_banks:
        raise BattleTableError(
            f"MSG source is missing message banks: {', '.join(sorted(missing_banks))}"
        )
    complete_tables: list[tuple[str, ...]] = []
    for row, values in zip(profile.text_profiles, text_tables):
        missing = [index for index, value in enumerate(values) if value is None]
        if missing:
            raise BattleTableError(f"MSG source is missing {row.directive} {missing[0]}")
        complete_tables.append(tuple(value for value in values if value is not None))
    banks = tuple(
        _read_message_source(source_dir / bank_files[kind][1], kind)
        for kind in MESSAGE_BANK_KINDS
    )
    table = MessageTable(
        profile,
        tuple(complete_tables),
        banks,
    )
    encode_message(table)
    return table


def parse_aicalc_source(text: str, source_dir: Path = Path(".")) -> AiCalcTable:
    """Assemble AICALC source and its sibling FLW0 source files."""

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
            "source must begin with 'battle-table 1 kind=aicalc profile=PROFILE'"
        )
    header = _fields(first[2:], first_number, {"kind", "profile"}, "header")
    if header.get("kind") != "aicalc":
        raise BattleTableError("AICALC source needs kind=aicalc")
    try:
        profile = AICALC_PROFILES[header["profile"]]
    except KeyError as exc:
        raise BattleTableError(f"unknown AICALC profile {header.get('profile')!r}") from exc

    script_files: dict[str, tuple[int, str]] = {}
    for line_number, line in meaningful[1:]:
        tokens = _tokens(line, line_number)
        if tokens[0] != "script":
            continue
        if len(tokens) < 3 or tokens[1] not in {"ai", "formula"}:
            raise BattleTableError(f"line {line_number}: expected script ai|formula file=PATH")
        fields = _fields(tokens[2:], line_number, {"file"}, "script")
        if "file" not in fields:
            raise BattleTableError(f"line {line_number}: script needs file=PATH")
        if tokens[1] in script_files:
            raise BattleTableError(f"line {line_number}: duplicate {tokens[1]} script")
        script_files[tokens[1]] = (line_number, fields["file"])
    if set(script_files) != {"ai", "formula"}:
        raise BattleTableError("AICALC source needs one AI and one formula script")

    ai_script = _read_flw0_source(source_dir / script_files["ai"][1], "AI")
    formula_script = _read_flw0_source(
        source_dir / script_files["formula"][1], "formula"
    )
    procedures = flw0.parse(ai_script).named_rows(0)
    procedures_by_name = {row.name: row.row_index for row in procedures}
    if len(procedures_by_name) != len(procedures):
        raise BattleTableError("AI script has duplicate procedure names")

    enemies = [EnemyAi() for _ in range(384)]
    calculation_words = [0] * profile.calculation_word_count
    weighted_tables = [
        [WeightedValue() for _ in range(8)]
        for _ in range(profile.weighted_table_count)
    ]
    seen_calculation_words: set[int] = set()
    seen_enemies: set[int] = set()
    seen_weighted_tables: set[int] = set()
    source_index = 1
    while source_index < len(meaningful):
        line_number, line = meaningful[source_index]
        tokens = _tokens(line, line_number)
        directive = tokens[0]
        if directive == "script":
            source_index += 1
            continue
        if directive == "calculation-word":
            if len(tokens) < 3:
                raise BattleTableError(f"line {line_number}: calculation-word needs an offset")
            offset = _integer(tokens[1], line_number, "calculation-word offset")
            if offset & 3 or not 0 <= offset < profile.calculation_word_count * 4:
                raise BattleTableError(f"line {line_number}: invalid calculation-word offset")
            index = offset // 4
            if index in seen_calculation_words:
                raise BattleTableError(f"line {line_number}: duplicate calculation-word {offset:#x}")
            seen_calculation_words.add(index)
            fields = _fields(tokens[2:], line_number, {"value", "bits"}, directive)
            calculation_words[index] = _parse_bits_or_float(fields, line_number, directive)
            source_index += 1
            continue
        if directive == "weighted-table":
            if len(tokens) != 2:
                raise BattleTableError(f"line {line_number}: weighted-table needs an index")
            table_index = _index(
                tokens[1], line_number, profile.weighted_table_count, "weighted table index"
            )
            if table_index in seen_weighted_tables:
                raise BattleTableError(f"line {line_number}: duplicate weighted-table {table_index}")
            seen_weighted_tables.add(table_index)
            seen_entries: set[int] = set()
            source_index += 1
            while source_index < len(meaningful):
                child_number, child_line = meaningful[source_index]
                child = _tokens(child_line, child_number)
                if child == ["end"]:
                    break
                if len(child) < 2 or child[0] != "entry":
                    raise BattleTableError(f"line {child_number}: expected weighted entry or end")
                entry_index = _index(child[1], child_number, 8, "weighted entry index")
                if entry_index in seen_entries:
                    raise BattleTableError(f"line {child_number}: duplicate weighted entry")
                seen_entries.add(entry_index)
                fields = _fields(child[2:], child_number, {"value", "weight"}, "entry")
                weighted_tables[table_index][entry_index] = WeightedValue(
                    _value(fields, "value", 0, child_number),
                    _value(fields, "weight", 0, child_number),
                )
                source_index += 1
            else:
                raise BattleTableError(f"line {line_number}: unterminated weighted-table")
            source_index += 1
            continue
        if directive != "enemy-ai":
            raise BattleTableError(f"line {line_number}: unknown directive {directive!r}")
        if len(tokens) < 2:
            raise BattleTableError(f"line {line_number}: enemy-ai needs an index")
        enemy_id = _index(tokens[1], line_number, 384, "enemy AI index")
        if enemy_id in seen_enemies:
            raise BattleTableError(f"line {line_number}: duplicate enemy-ai {enemy_id}")
        seen_enemies.add(enemy_id)
        fields = _fields(tokens[2:], line_number, {"flags", "script", "reserved"}, directive)
        procedure = 0
        if "script" in fields:
            try:
                procedure = procedures_by_name[fields["script"]]
            except KeyError as exc:
                raise BattleTableError(
                    f"line {line_number}: unknown AI procedure {fields['script']!r}"
                ) from exc
        decisions = [AiDecision() for _ in range(3)]
        groups = [[AiChoice() for _ in range(5)] for _ in range(7)]
        seen_decisions: set[int] = set()
        seen_choices: set[tuple[int, int]] = set()
        source_index += 1
        while source_index < len(meaningful):
            child_number, child_line = meaningful[source_index]
            child = _tokens(child_line, child_number)
            if child == ["end"]:
                break
            if child[0] == "decision":
                if len(child) < 2:
                    raise BattleTableError(f"line {child_number}: decision needs an index")
                decision_index = _index(child[1], child_number, 3, "decision index")
                if decision_index in seen_decisions:
                    raise BattleTableError(f"line {child_number}: duplicate decision")
                seen_decisions.add(decision_index)
                child_fields = _fields(
                    child[2:], child_number, {"predicates", "routes"}, "decision"
                )
                predicates = _ai_operations(
                    child_fields.get("predicates", "none,none,none"), child_number
                )
                routes = _int_list(child_fields.get("routes", ""), child_number, "routes")
                if len(routes) != 8:
                    raise BattleTableError(f"line {child_number}: routes needs eight values")
                decisions[decision_index] = AiDecision(predicates, routes)
            elif child[0] == "choice":
                if len(child) < 3:
                    raise BattleTableError(
                        f"line {child_number}: choice needs group and slot indices"
                    )
                group_index = _index(child[1], child_number, 7, "choice group")
                choice_index = _index(child[2], child_number, 5, "choice slot")
                key = (group_index, choice_index)
                if key in seen_choices:
                    raise BattleTableError(f"line {child_number}: duplicate choice")
                seen_choices.add(key)
                child_fields = _fields(
                    child[3:], child_number, {"weight", "action", "effect"}, "choice"
                )
                if "action" not in child_fields:
                    raise BattleTableError(f"line {child_number}: choice needs action=")
                groups[group_index][choice_index] = AiChoice(
                    _value(child_fields, "weight", 0, child_number),
                    _ai_action(child_fields["action"], child_number),
                    _ai_operation(child_fields.get("effect", "none"), child_number, "effect"),
                )
            else:
                raise BattleTableError(f"line {child_number}: expected decision, choice, or end")
            source_index += 1
        else:
            raise BattleTableError(f"line {line_number}: unterminated enemy-ai")
        enemies[enemy_id] = EnemyAi(
            _value(fields, "flags", 0, line_number),
            procedure,
            tuple(decisions),
            tuple(tuple(group) for group in groups),
            _value(fields, "reserved", 0, line_number),
        )
        source_index += 1

    result = AiCalcTable(
        profile,
        tuple(enemies),
        tuple(calculation_words),
        tuple(tuple(table) for table in weighted_tables),
        ai_script,
        formula_script,
    )
    validate_aicalc_references(result)
    encode_aicalc(result)
    return result


def parse_skill_source(text: str) -> SkillTable:
    """Assemble SKILL source on top of the selected profile's empty tables."""

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
            "source must begin with 'battle-table 1 kind=skill profile=PROFILE'"
        )
    header = _fields(first[2:], first_number, {"kind", "profile"}, "header")
    if header.get("kind") != "skill":
        raise BattleTableError("SKILL source needs kind=skill")
    try:
        profile = SKILL_PROFILES[header["profile"]]
    except KeyError as exc:
        raise BattleTableError(f"unknown SKILL profile {header.get('profile')!r}") from exc

    model = default_skill(profile)
    action_attributes = list(model.action_attributes)
    actions = list(model.actions)
    requirements = list(model.requirements)
    coefficient_bits = list(model.coefficient_bits)
    party_defaults = list(model.party_defaults)
    items = list(model.items)
    profile_bonuses = list(model.profile_bonuses)
    groups = list(model.groups)
    seen: set[tuple[str, int]] = set()

    action_fields = set(SkillAction.__dataclass_fields__)
    for line_number, line in meaningful[1:]:
        tokens = _tokens(line, line_number)
        directive = tokens[0]
        if len(tokens) < 2:
            raise BattleTableError(f"line {line_number}: {directive} needs an index")

        if directive == "action-attribute":
            index = _index(tokens[1], line_number, profile.action_attr_count, "skill id")
            fields = _fields(
                tokens[2:], line_number, {"attribute", "auxiliary"}, directive
            )
            row = SkillActionAttribute(
                _value(fields, "attribute", 0, line_number),
                _value(fields, "auxiliary", 0, line_number),
            )
            target = action_attributes
        elif directive == "action":
            index = _index(tokens[1], line_number, profile.action_count, "action id")
            fields = _fields(tokens[2:], line_number, action_fields, directive)
            row = SkillAction(
                **{
                    name: _value(fields, name, 0, line_number)
                    for name in SkillAction.__dataclass_fields__
                }
            )
            target = actions
        elif directive == "requirement":
            skill_id = _integer(tokens[1], line_number, "skill id")
            index = skill_id - profile.requirement_start
            if not 0 <= index < profile.requirement_count:
                raise BattleTableError(
                    f"line {line_number}: requirement skill id {skill_id:#x} is outside "
                    f"{profile.requirement_start:#x}.."
                    f"{profile.requirement_start + profile.requirement_count - 1:#x}"
                )
            fields = _fields(tokens[2:], line_number, {"conditions", "count", "flags"}, directive)
            conditions = _condition_values(fields.get("conditions", ""), line_number)
            if len(conditions) != 3:
                raise BattleTableError(f"line {line_number}: requirement needs three conditions")
            row = SkillRequirement(
                conditions,
                _value(fields, "count", 0, line_number),
                _value(fields, "flags", 0, line_number),
            )
            target = requirements
        elif directive == "coefficient":
            offset = _integer(tokens[1], line_number, "coefficient offset")
            if offset & 3 or not 0 <= offset < profile.coefficient_count * 4:
                raise BattleTableError(f"line {line_number}: invalid coefficient offset {offset:#x}")
            index = offset // 4
            fields = _fields(tokens[2:], line_number, {"value", "bits"}, directive)
            row = _parse_bits_or_float(fields, line_number, directive)
            target = coefficient_bits
        elif directive == "party-default":
            index = _index(tokens[1], line_number, 16, "party index")
            allowed = {
                "base_value", "value_02", "multiplier", "multiplier_bits",
                "flags_08", "flags_0a", "value_0c", "repeat_min", "repeat_max",
                "value_10", "value_12",
            }
            fields = _fields(tokens[2:], line_number, allowed, directive)
            if "multiplier" in fields and "multiplier_bits" in fields:
                raise BattleTableError(f"line {line_number}: duplicate multiplier representation")
            multiplier_bits = (
                _float_bits(fields["multiplier"], line_number, "multiplier")
                if "multiplier" in fields
                else _value(fields, "multiplier_bits", 0, line_number)
            )
            row = PartySkillDefaults(
                _value(fields, "base_value", 0, line_number),
                _value(fields, "value_02", 0, line_number),
                multiplier_bits,
                _value(fields, "flags_08", 0, line_number),
                _value(fields, "flags_0a", 0, line_number),
                _value(fields, "value_0c", 0, line_number),
                _value(fields, "repeat_min", 0, line_number),
                _value(fields, "repeat_max", 0, line_number),
                _value(fields, "value_10", 0, line_number),
                _value(fields, "value_12", 0, line_number),
            )
            target = party_defaults
        elif directive == "item-entry":
            index = _index(tokens[1], line_number, profile.item_count, "item entry index")
            fields = _fields(tokens[2:], line_number, {"flags", "item", "value", "auxiliary"}, directive)
            row = SkillItemEntry(
                _value(fields, "flags", 0, line_number),
                _value(fields, "item", 0, line_number),
                _value(fields, "value", 0, line_number),
                _value(fields, "auxiliary", 0, line_number),
            )
            target = items
        elif directive == "profile-bonus":
            bonus_id = _integer(tokens[1], line_number, "profile bonus id")
            index = bonus_id - 0xC0
            if not 0 <= index < profile.bonus_count:
                raise BattleTableError(f"line {line_number}: invalid profile bonus id {bonus_id:#x}")
            fields = _fields(tokens[2:], line_number, {"stats", "tier"}, directive)
            stats = _int_list(fields.get("stats", ""), line_number, "stats")
            if len(stats) != 5:
                raise BattleTableError(f"line {line_number}: profile bonus needs five stats")
            row = ProfileBonus(stats, _value(fields, "tier", 0, line_number))
            target = profile_bonuses
        elif directive == "group":
            index = _index(tokens[1], line_number, profile.group_count, "group index")
            fields = _fields(tokens[2:], line_number, {"skills"}, directive)
            row = SkillGroup(_int_list(fields.get("skills", ""), line_number, "skills"))
            target = groups
        else:
            raise BattleTableError(f"line {line_number}: unknown directive {directive!r}")

        key = (directive, index)
        if key in seen:
            raise BattleTableError(f"line {line_number}: duplicate {directive} {tokens[1]}")
        seen.add(key)
        target[index] = row

    result = SkillTable(
        profile,
        tuple(action_attributes),
        tuple(actions),
        tuple(requirements),
        tuple(coefficient_bits),
        tuple(party_defaults),
        tuple(items),
        tuple(profile_bonuses),
        tuple(groups),
    )
    encode_skill(result)
    return result


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


def _f32_text(bits: int) -> str | None:
    value = struct.unpack("<f", struct.pack("<I", bits))[0]
    if not math.isfinite(value):
        return None
    for precision in range(1, 10):
        text = format(value, f".{precision}g")
        if struct.unpack("<I", struct.pack("<f", float(text)))[0] == bits:
            return text
    return repr(value)


def _append_float_bits(fields: list[str], key: str, bits: int) -> None:
    if bits == 0:
        return
    text = _f32_text(bits)
    if text is not None and 0.0001 <= abs(float(text)) <= 1000:
        fields.append(f"{key}={text}")
    else:
        fields.append(f"{key}_bits={bits:#x}")


def _condition_text(value: int) -> str:
    if value == 0:
        return "any"
    if value == 0xFFFFFFFF:
        return "none"
    tag = value & 0xF0000000
    payload = value & 0x0FFFFFFF
    if tag == 0:
        return f"skill:{payload}"
    if tag == 0x10000000:
        return f"attribute-mask:{payload:#x}"
    if tag == 0x20000000:
        return f"unit-mask:{payload:#x}"
    if tag == 0x40000000:
        return f"group:{payload}"
    return f"{value:#x}"


def _condition_list(values: tuple[int, ...]) -> str:
    return ",".join(_condition_text(value) for value in values)


def render_skill_source(table: SkillTable) -> str:
    """Render complete canonical SKILL source with consumer-backed records."""

    encode_skill(table)
    lines = [f"battle-table 1 kind=skill profile={table.profile.name}", ""]
    for index, row in enumerate(table.action_attributes):
        if row == SkillActionAttribute():
            continue
        fields: list[str] = []
        _append(fields, "attribute", row.action_attribute)
        _append(fields, "auxiliary", row.auxiliary)
        lines.append(f"action-attribute {index} {' '.join(fields)}")
    lines.append("")

    action_renderers = (
        ("flags", "hex"), ("use", "int"), ("effect_type", "int"),
        ("cost_type", "int"), ("cost", "int"), ("cost_base", "int"),
        ("target_type", "int"), ("target_area", "int"),
        ("target_rule", "int"), ("target_random", "int"),
        ("untargetable_status", "hex"), ("target_program", "int"),
        ("hit_type", "int"), ("hit_level", "int"),
        ("hit_program", "int"), ("hits_min", "int"),
        ("hits_max", "int"), ("hp_type", "int"), ("hp_power", "int"),
        ("mp_type", "int"), ("mp_power", "int"), ("hp_base", "int"),
        ("mp_base", "int"), ("effect_percent", "int"),
        ("ailment_type", "int"), ("ailment_level", "int"),
        ("base_status", "hex"), ("support_type", "hex"),
        ("support_points", "int"), ("death_type", "int"),
        ("lookup_id", "int"), ("program", "int"),
        ("magic_base", "int"), ("magic_limit", "int"),
    )
    for index, row in enumerate(table.actions):
        if row == SkillAction():
            continue
        fields = []
        for attribute, style in action_renderers:
            value = getattr(row, attribute)
            if style == "hex":
                _append_hex(fields, attribute, value)
            else:
                _append(fields, attribute, value)
        lines.append(f"action {index} {' '.join(fields)}")
    lines.append("")

    for index, row in enumerate(table.requirements):
        if row == SkillRequirement():
            continue
        skill_id = table.profile.requirement_start + index
        fields = [f"conditions={_condition_list(row.conditions)}"]
        _append(fields, "count", row.count)
        _append_hex(fields, "flags", row.flags)
        lines.append(f"requirement {skill_id:#x} {' '.join(fields)}")
    lines.append("")

    for index, bits in enumerate(table.coefficient_bits):
        if bits == 0:
            continue
        text = _f32_text(bits)
        if text is not None and 0.0001 <= abs(float(text)) <= 1000:
            field = f"value={text}"
        else:
            field = f"bits={bits:#x}"
        lines.append(f"coefficient {index * 4:#x} {field}")
    lines.append("")

    for index, row in enumerate(table.party_defaults):
        if row == PartySkillDefaults():
            continue
        fields = []
        _append(fields, "base_value", row.base_value)
        _append(fields, "value_02", row.value_02)
        _append_float_bits(fields, "multiplier", row.multiplier_bits)
        _append_hex(fields, "flags_08", row.flags_08)
        _append_hex(fields, "flags_0a", row.flags_0a)
        _append(fields, "value_0c", row.value_0c)
        _append(fields, "repeat_min", row.repeat_min)
        _append(fields, "repeat_max", row.repeat_max)
        _append(fields, "value_10", row.value_10)
        _append(fields, "value_12", row.value_12)
        lines.append(f"party-default {index} {' '.join(fields)}")
    lines.append("")

    for index, row in enumerate(table.items):
        if row == SkillItemEntry():
            continue
        fields = []
        _append_hex(fields, "flags", row.flags)
        _append(fields, "item", row.item_id)
        _append(fields, "value", row.value)
        _append(fields, "auxiliary", row.auxiliary)
        lines.append(f"item-entry {index} {' '.join(fields)}")
    if table.profile_bonuses:
        lines.append("")
        for index, row in enumerate(table.profile_bonuses):
            if row == ProfileBonus():
                continue
            lines.append(
                f"profile-bonus {0xC0 + index:#x} "
                f"stats={_list(row.stats)} tier={row.tier}"
            )
    lines.append("")

    for index, row in enumerate(table.groups):
        if not row.skills:
            continue
        lines.append(f"group {index} skills={_list(row.skills)}")
    return "\n".join(lines).rstrip() + "\n"


def _ai_operation_text(operation: AiOperation) -> str:
    if operation == AiOperation():
        return "none"
    argument = (
        f"{operation.argument:#x}" if operation.argument >= 0x1000 else str(operation.argument)
    )
    return f"{operation.selector}:{argument}"


def _ai_action_text(action: int) -> str:
    family = action & 0xF000
    argument = action & 0xFFF
    if family == 0:
        return f"skill:{action}"
    if 0x1000 <= family <= 0x6000:
        return f"preset:{family >> 12}:{argument}"
    if family == 0x7000:
        return f"weighted:{argument}"
    if family == 0x8000:
        return f"special:{argument}"
    return f"{action:#x}"


def render_aicalc_source(
    table: AiCalcTable,
    ai_script_file: str = "aicalc-ai.bfasm",
    formula_script_file: str = "aicalc-formulas.bfasm",
) -> str:
    """Render canonical AICALC table source with sibling script references."""

    encode_aicalc(table)
    validate_aicalc_references(table)
    procedures = flw0.parse(table.ai_script).named_rows(0)
    procedure_names = {row.row_index: row.name for row in procedures}
    lines = [
        f"battle-table 1 kind=aicalc profile={table.profile.name}",
        "",
        f"script ai file={ai_script_file}",
        f"script formula file={formula_script_file}",
        "",
    ]

    for index, bits in enumerate(table.calculation_words):
        if bits == 0:
            continue
        text = _f32_text(bits)
        if text is not None and 0.0001 <= abs(float(text)) <= 1000:
            field = f"value={text}"
        else:
            field = f"bits={bits:#x}"
        lines.append(f"calculation-word {index * 4:#x} {field}")

    for table_index, weighted_table in enumerate(table.weighted_tables):
        if not any(entry != WeightedValue() for entry in weighted_table):
            continue
        lines.extend(("", f"weighted-table {table_index}"))
        for entry_index, entry in enumerate(weighted_table):
            if entry == WeightedValue():
                continue
            fields = []
            _append(fields, "value", entry.value)
            _append(fields, "weight", entry.weight)
            lines.append(f"  entry {entry_index} {' '.join(fields)}")
        lines.append("end")

    for enemy_id, enemy in enumerate(table.enemies):
        if enemy == EnemyAi():
            continue
        fields = []
        _append_hex(fields, "flags", enemy.flags)
        if enemy.script_procedure:
            try:
                fields.append(f"script={procedure_names[enemy.script_procedure]}")
            except KeyError as exc:
                raise BattleTableError(
                    f"enemy AI {enemy_id} references missing procedure "
                    f"{enemy.script_procedure}"
                ) from exc
        _append_hex(fields, "reserved", enemy.reserved)
        suffix = f" {' '.join(fields)}" if fields else ""
        lines.extend(("", f"enemy-ai {enemy_id}{suffix}"))
        for decision_index, decision in enumerate(enemy.decisions):
            if decision == AiDecision():
                continue
            predicates = ",".join(
                _ai_operation_text(operation) for operation in decision.predicates
            )
            routes = _list(decision.routes)
            lines.append(
                f"  decision {decision_index} predicates={predicates} routes={routes}"
            )
        for group_index, group in enumerate(enemy.groups):
            for choice_index, choice in enumerate(group):
                if choice == AiChoice():
                    continue
                choice_fields = []
                _append(choice_fields, "weight", choice.weight)
                choice_fields.append(f"action={_ai_action_text(choice.action)}")
                if choice.effect != AiOperation():
                    choice_fields.append(f"effect={_ai_operation_text(choice.effect)}")
                lines.append(
                    f"  choice {group_index} {choice_index} {' '.join(choice_fields)}"
                )
        lines.append("end")
    return "\n".join(lines).rstrip() + "\n"


def render_message_source(
    table: MessageTable,
    bank_files: tuple[str, ...] = (
        "msg-items.msgasm",
        "msg-skills.msgasm",
        "msg-status-help.msgasm",
        "msg-command-help.msgasm",
    ),
) -> str:
    """Render canonical MSG table source with sibling message-bank references."""

    encode_message(table)
    lines = [f"battle-table 1 kind=message profile={table.profile.name}", ""]
    for kind, path in zip(MESSAGE_BANK_KINDS, bank_files):
        lines.append(f"message-bank {kind} file={path}")
    for row, values in zip(table.profile.text_profiles, table.text_tables):
        lines.extend(("", f"# {row.count} rows, {row.width} bytes each"))
        lines.extend(
            f"{row.directive} {index} {json.dumps(value)}"
            for index, value in enumerate(values)
        )
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
    segments = _split_segments(data)
    segment_count = len(segments)
    if (
        segment_count in {13, 14}
        and all(segment[8:12] == b"MSG1" for segment in segments[-4:])
    ):
        table = decode_message(data)
        bank_names = tuple(f"{args.output.stem}-{kind}.msgasm" for kind in MESSAGE_BANK_KINDS)
        for name, bank in zip(bank_names, table.message_banks):
            _write_text(args.output.parent / name, "\n".join(msg1.render(bank)) + "\n")
        source = render_message_source(table, bank_names)
    elif (
        segment_count in {4, 5}
        and len(segments[0]) == 0x20A00
        and all(segment[8:12] == b"FLW0" for segment in segments[-2:])
    ):
        table = decode_aicalc(data)
        ai_name = f"{args.output.stem}-ai.bfasm"
        formula_name = f"{args.output.stem}-formulas.bfasm"
        try:
            ai_source = flw0_symbolic.render(
                flw0.parse(table.ai_script),
                f"{table.profile.name}-aicalc",
                semantic=True,
                structured=True,
            )
            formula_source = flw0_symbolic.render(
                flw0.parse(table.formula_script),
                f"{table.profile.name}-aicalc",
                semantic=True,
                structured=True,
            )
        except flw0.Flw0Error as exc:
            raise BattleTableError(f"cannot render AICALC script source: {exc}") from exc
        _write_text(args.output.parent / ai_name, ai_source)
        _write_text(args.output.parent / formula_name, formula_source)
        source = render_aicalc_source(table, ai_name, formula_name)
    elif segment_count == 6:
        source = render_encount_source(decode_encount(data))
    elif segment_count == 5:
        source = render_unit_source(decode_unit(data))
    elif segment_count in {7, 8}:
        source = render_skill_source(decode_skill(data))
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
    elif header.get("kind") == "skill":
        data = encode_skill(parse_skill_source(source))
    elif header.get("kind") == "aicalc":
        data = encode_aicalc(parse_aicalc_source(source, args.input.parent))
    elif header.get("kind") == "message":
        data = encode_message(parse_message_source(source, args.input.parent))
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
