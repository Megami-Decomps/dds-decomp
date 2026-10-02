#!/usr/bin/env python3
"""Disassemble and assemble DDS field actor/warp (``.WAP``) tables."""

from __future__ import annotations

import argparse
import json
import shlex
import struct
import sys
from dataclasses import dataclass, replace
from pathlib import Path


ELEVATOR_SIZE = 0x20
ENTRY_COUNT = 0x100
LEGACY_ENTRY_SIZE = 0x64
EXTENDED_ENTRY_SIZE = 0x6C


class WapError(ValueError):
    """The binary or source is not a supported canonical DDS WAP table."""


@dataclass(frozen=True)
class Profile:
    name: str
    elevator_count: int
    entry_size: int

    @property
    def file_size(self) -> int:
        return self.elevator_count * ELEVATOR_SIZE + ENTRY_COUNT * self.entry_size


PROFILES = {
    profile.name: profile
    for profile in (
        Profile("legacy", 5, LEGACY_ENTRY_SIZE),
        Profile("dds1", 5, EXTENDED_ENTRY_SIZE),
        Profile("dds2", 8, EXTENDED_ENTRY_SIZE),
    )
}
PROFILES_BY_SIZE = {profile.file_size: profile for profile in PROFILES.values()}


@dataclass(frozen=True)
class FixedString:
    value: str
    padding: bytes = b""


@dataclass(frozen=True)
class Elevator:
    area: int
    sound: int
    floor_count: int
    floors: tuple[int, int, int, int, int, int]
    blocks: tuple[int, int, int, int, int, int]


@dataclass(frozen=True)
class Entry:
    kind: int
    flag_mode: int
    flag: int
    area: int
    name: FixedString
    scene_args: tuple[int, int, int]
    scene_primary: FixedString
    scene_secondary: FixedString
    warp_type: int
    attributes: int
    warp_args: tuple[int, int, int]
    position: FixedString
    camera_mode: int
    camera_table: int
    camera: FixedString
    bgm: int
    footstep: int
    after_flag: int
    after_script: FixedString
    tail: tuple[int, ...]


@dataclass(frozen=True)
class WapFile:
    profile: Profile
    elevators: tuple[Elevator, ...]
    entries: tuple[Entry, ...]


@dataclass(frozen=True)
class References:
    interactions: frozenset[str] | None = None
    procedures: frozenset[str] | None = None


DEFAULT_ELEVATOR = Elevator(1, 0, 1, (0,) * 6, (0,) * 6)


def default_entry(profile: Profile) -> Entry:
    return Entry(
        0,
        0,
        0,
        0,
        FixedString(""),
        (0, 0, 0),
        FixedString(""),
        FixedString(""),
        0,
        0,
        (0, 0, 0),
        FixedString("01pos_01"),
        0,
        0,
        FixedString("01cam_01"),
        1,
        1,
        0,
        FixedString(""),
        (0,) * (profile.entry_size - LEGACY_ENTRY_SIZE),
    )


def default_file(profile: Profile) -> WapFile:
    return WapFile(
        profile,
        (DEFAULT_ELEVATOR,) * profile.elevator_count,
        (default_entry(profile),) * ENTRY_COUNT,
    )


def _range(value: int, minimum: int, maximum: int, context: str) -> int:
    if not minimum <= value <= maximum:
        raise WapError(f"{context} {value} is outside {minimum}..{maximum}")
    return value


def _s16(value: int, context: str) -> int:
    return _range(value, -0x8000, 0x7FFF, context)


def _s32(value: int, context: str) -> int:
    return _range(value, -0x80000000, 0x7FFFFFFF, context)


def _u8(value: int, context: str) -> int:
    return _range(value, 0, 0xFF, context)


def _decode_ascii(data: bytes, offset: int, size: int, context: str) -> FixedString:
    raw = data[offset : offset + size]
    value, separator, padding = raw.partition(b"\0")
    if any(byte < 0x20 or byte > 0x7E for byte in value):
        raise WapError(f"{context} is not printable ASCII")
    try:
        text = value.decode("ascii")
    except UnicodeDecodeError as exc:  # guarded above, retained for clarity
        raise WapError(f"{context} is not ASCII") from exc
    residue = padding if separator and any(padding) else b""
    return FixedString(text, residue)


def _encode_ascii(value: FixedString, size: int, context: str) -> bytes:
    try:
        raw = value.value.encode("ascii")
    except UnicodeEncodeError as exc:
        raise WapError(f"{context} is not ASCII") from exc
    if any(byte < 0x20 or byte > 0x7E for byte in raw):
        raise WapError(f"{context} is not printable ASCII")
    if value.padding:
        if len(raw) + 1 + len(value.padding) != size:
            raise WapError(
                f"{context} padding gives {len(raw) + 1 + len(value.padding)} bytes, "
                f"expected {size}"
            )
        return raw + b"\0" + value.padding
    if len(raw) > size:
        raise WapError(f"{context} exceeds {size} bytes")
    return raw + bytes(size - len(raw))


def decode(data: bytes) -> WapFile:
    """Decode and validate one of the three retail DDS WAP layouts."""

    try:
        profile = PROFILES_BY_SIZE[len(data)]
    except KeyError as exc:
        sizes = ", ".join(f"{size:#x}" for size in sorted(PROFILES_BY_SIZE))
        raise WapError(f"WAP file is {len(data):#x} bytes, expected {sizes}") from exc

    elevators = tuple(
        Elevator(
            values[0],
            values[1],
            values[2],
            tuple(values[3:9]),  # type: ignore[arg-type]
            tuple(values[9:15]),  # type: ignore[arg-type]
        )
        for index in range(profile.elevator_count)
        for values in (struct.unpack_from("<ihh6h6h", data, index * ELEVATOR_SIZE),)
    )

    entries: list[Entry] = []
    table_offset = profile.elevator_count * ELEVATOR_SIZE
    for index in range(ENTRY_COUNT):
        base = table_offset + index * profile.entry_size
        entries.append(
            Entry(
                data[base],
                data[base + 1],
                struct.unpack_from("<h", data, base + 2)[0],
                struct.unpack_from("<h", data, base + 4)[0],
                _decode_ascii(data, base + 0x06, 12, f"entry {index} name"),
                struct.unpack_from("<3h", data, base + 0x12),
                _decode_ascii(data, base + 0x18, 12, f"entry {index} primary scene"),
                _decode_ascii(data, base + 0x24, 12, f"entry {index} secondary scene"),
                data[base + 0x30],
                data[base + 0x31],
                struct.unpack_from("<3h", data, base + 0x32),
                _decode_ascii(data, base + 0x38, 12, f"entry {index} position"),
                data[base + 0x44],
                data[base + 0x45],
                _decode_ascii(data, base + 0x46, 12, f"entry {index} camera"),
                data[base + 0x52],
                data[base + 0x53],
                data[base + 0x54],
                _decode_ascii(data, base + 0x55, 15, f"entry {index} after script"),
                tuple(data[base + LEGACY_ENTRY_SIZE : base + profile.entry_size]),
            )
        )
    return WapFile(profile, elevators, tuple(entries))


def encode(wap: WapFile) -> bytes:
    """Encode one WAP model to its exact physical profile."""

    profile = PROFILES.get(wap.profile.name)
    if profile != wap.profile:
        raise WapError(f"unknown or modified WAP profile {wap.profile.name!r}")
    if len(wap.elevators) != profile.elevator_count:
        raise WapError(
            f"profile {profile.name} needs {profile.elevator_count} elevator rows, "
            f"found {len(wap.elevators)}"
        )
    if len(wap.entries) != ENTRY_COUNT:
        raise WapError(f"expected {ENTRY_COUNT} actor/warp entries, found {len(wap.entries)}")

    output = bytearray(profile.file_size)
    for index, elevator in enumerate(wap.elevators):
        if len(elevator.floors) != 6 or len(elevator.blocks) != 6:
            raise WapError(f"elevator {index} needs six floors and six blocks")
        try:
            struct.pack_into(
                "<ihh6h6h",
                output,
                index * ELEVATOR_SIZE,
                _s32(elevator.area, f"elevator {index} area"),
                _s16(elevator.sound, f"elevator {index} sound"),
                _s16(elevator.floor_count, f"elevator {index} floor count"),
                *(_s16(value, f"elevator {index} floor") for value in elevator.floors),
                *(_s16(value, f"elevator {index} block") for value in elevator.blocks),
            )
        except struct.error as exc:
            raise WapError(f"cannot encode elevator {index}: {exc}") from exc

    table_offset = profile.elevator_count * ELEVATOR_SIZE
    tail_size = profile.entry_size - LEGACY_ENTRY_SIZE
    for index, entry in enumerate(wap.entries):
        if len(entry.scene_args) != 3 or len(entry.warp_args) != 3:
            raise WapError(f"entry {index} needs three scene and three warp arguments")
        if len(entry.tail) != tail_size:
            raise WapError(f"entry {index} needs {tail_size} profile-tail bytes")
        base = table_offset + index * profile.entry_size
        output[base] = _u8(entry.kind, f"entry {index} kind")
        output[base + 1] = _u8(entry.flag_mode, f"entry {index} flag mode")
        struct.pack_into(
            "<hh",
            output,
            base + 2,
            _s16(entry.flag, f"entry {index} flag"),
            _s16(entry.area, f"entry {index} area"),
        )
        output[base + 0x06 : base + 0x12] = _encode_ascii(
            entry.name, 12, f"entry {index} name"
        )
        struct.pack_into(
            "<3h",
            output,
            base + 0x12,
            *(_s16(value, f"entry {index} scene argument") for value in entry.scene_args),
        )
        output[base + 0x18 : base + 0x24] = _encode_ascii(
            entry.scene_primary, 12, f"entry {index} primary scene"
        )
        output[base + 0x24 : base + 0x30] = _encode_ascii(
            entry.scene_secondary, 12, f"entry {index} secondary scene"
        )
        output[base + 0x30] = _u8(entry.warp_type, f"entry {index} warp type")
        output[base + 0x31] = _u8(entry.attributes, f"entry {index} attributes")
        struct.pack_into(
            "<3h",
            output,
            base + 0x32,
            *(_s16(value, f"entry {index} warp argument") for value in entry.warp_args),
        )
        output[base + 0x38 : base + 0x44] = _encode_ascii(
            entry.position, 12, f"entry {index} position"
        )
        output[base + 0x44] = _u8(entry.camera_mode, f"entry {index} camera mode")
        output[base + 0x45] = _u8(entry.camera_table, f"entry {index} camera table")
        output[base + 0x46 : base + 0x52] = _encode_ascii(
            entry.camera, 12, f"entry {index} camera"
        )
        output[base + 0x52] = _u8(entry.bgm, f"entry {index} BGM")
        output[base + 0x53] = _u8(entry.footstep, f"entry {index} footstep")
        output[base + 0x54] = _u8(entry.after_flag, f"entry {index} after flag")
        output[base + 0x55 : base + LEGACY_ENTRY_SIZE] = _encode_ascii(
            entry.after_script, 15, f"entry {index} after script"
        )
        output[base + LEGACY_ENTRY_SIZE : base + profile.entry_size] = bytes(
            _u8(value, f"entry {index} tail byte") for value in entry.tail
        )
    return bytes(output)


def _tokens(line: str, line_number: int) -> list[str]:
    try:
        return shlex.split(line, comments=True, posix=True)
    except ValueError as exc:
        raise WapError(f"line {line_number}: {exc}") from exc


def _values(tokens: list[str], line_number: int) -> dict[str, str]:
    values: dict[str, str] = {}
    for token in tokens:
        if "=" not in token:
            raise WapError(f"line {line_number}: expected key=value, found {token!r}")
        key, value = token.split("=", 1)
        if not key or key in values:
            raise WapError(f"line {line_number}: invalid or duplicate field {key!r}")
        values[key] = value
    return values


def _fields(
    tokens: list[str], line_number: int, allowed: set[str], directive: str
) -> dict[str, str]:
    values = _values(tokens, line_number)
    unknown = values.keys() - allowed
    if unknown:
        names = ", ".join(sorted(unknown))
        raise WapError(f"line {line_number}: unknown {directive} field(s): {names}")
    return values


def _integer(text: str, line_number: int, context: str) -> int:
    try:
        return int(text, 0)
    except ValueError as exc:
        raise WapError(f"line {line_number}: invalid {context} {text!r}") from exc


def _index(text: str, line_number: int, count: int, context: str) -> int:
    return _range(_integer(text, line_number, context), 0, count - 1, context)


def _int_list(text: str, line_number: int, count: int, context: str) -> tuple[int, ...]:
    parts = text.split(",")
    if len(parts) != count:
        raise WapError(
            f"line {line_number}: {context} needs {count} comma-separated integers"
        )
    return tuple(_integer(part, line_number, context) for part in parts)


def _resolve_string(
    text: str,
    line_number: int,
    context: str,
    symbols: frozenset[str] | None,
) -> str:
    if not text.startswith("@"):
        return text
    symbol = text[1:]
    if not symbol or symbols is None or symbol not in symbols:
        raise WapError(f"line {line_number}: unknown {context} symbol {symbol!r}")
    return symbol


def _fixed_from_fields(
    fields: dict[str, str],
    key: str,
    base: FixedString,
    line_number: int,
    context: str,
    symbols: frozenset[str] | None = None,
) -> FixedString:
    padding_key = f"{key}_padding"
    value = (
        _resolve_string(fields[key], line_number, context, symbols)
        if key in fields
        else base.value
    )
    padding = base.padding
    if padding_key in fields:
        try:
            padding = bytes.fromhex(fields[padding_key])
        except ValueError as exc:
            raise WapError(f"line {line_number}: invalid {padding_key} hex") from exc
    elif key in fields:
        padding = b""
    return FixedString(value, padding)


_ENTRY_KINDS = {
    "door": 1,
    "run_warp": 2,
    "hole_warp": 3,
    "jump_warp": 4,
    "ladder": 5,
    "elevator_exit": 6,
    "side_exit": 7,
    "battle_exit": 8,
    "special_warp": 9,
    "warp": 10,
    "suction_warp": 11,
    "barrier": 12,
}
ENTRY_KIND_NAMES = {number: name for name, number in _ENTRY_KINDS.items()}

_SCENE_ARG_NAMES: dict[int, tuple[str | None, str | None, str | None]] = {
    1: ("motion", "secondary_motion", "sound"),
    5: ("direction", "direct_prompt", None),
    6: ("elevator", "floor", None),
    7: ("exit_mode", "selector", None),
    8: ("event", None, None),
    9: ("id", None, None),
    11: ("state_selector", "map_entry", "motion"),
    12: ("barrier", None, None),
}
_SCENE_NAMED_FIELDS = frozenset(
    name for names in _SCENE_ARG_NAMES.values() for name in names if name is not None
)

_WARP_TYPES = {"field": 0, "elevator": 1, "facility": 2, "event": 3}
WARP_TYPE_NAMES = {number: name for name, number in _WARP_TYPES.items()}

_WARP_ARG_NAMES: dict[int, tuple[str | None, str | None, str | None]] = {
    0: ("field", "area", None),
    1: ("table", "floor", None),
    3: ("event", None, "alternate_field"),
}
_FACILITY_ACTIONS = {
    "shop": 0,
    "terminal": 3,
    "save": 4,
    "heal": 5,
}
FACILITY_ACTION_NAMES = {
    number: name for name, number in _FACILITY_ACTIONS.items()
}
FACILITY_ARGUMENT_NAMES: dict[int, tuple[str, str]] = {
    0: ("selection", "floor_flag"),
    3: ("slot", "floor_flag"),
    4: ("slot", "floor_flag"),
    5: ("slot", "floor_flag"),
}
_WARP_NAMED_FIELDS = frozenset(
    name for names in _WARP_ARG_NAMES.values() for name in names if name is not None
) | frozenset(
    {"action"}
    | {name for names in FACILITY_ARGUMENT_NAMES.values() for name in names}
)


def _kind(text: str, line_number: int) -> int:
    if text in _ENTRY_KINDS:
        return _ENTRY_KINDS[text]
    return _integer(text, line_number, "kind")


def _warp_type(text: str, line_number: int) -> int:
    if text in _WARP_TYPES:
        return _WARP_TYPES[text]
    return _integer(text, line_number, "warp type")


def _facility_action(text: str, line_number: int) -> int:
    if text in _FACILITY_ACTIONS:
        return _FACILITY_ACTIONS[text]
    return _integer(text, line_number, "facility action")


def _facility_args(
    fields: dict[str, str],
    base: tuple[int, int, int],
    line_number: int,
) -> tuple[int, int, int]:
    """Read a facility action, its action-specific value, and its floor flag."""

    named_fields = _WARP_NAMED_FIELDS.intersection(fields)
    if "args" in fields and named_fields:
        raise WapError(
            f"line {line_number}: warp args cannot be combined with named arguments"
        )
    if "args" in fields:
        values = _int_list(fields["args"], line_number, 3, "warp args")
        return values[0], values[1], values[2]

    action = (
        _facility_action(fields["action"], line_number)
        if "action" in fields
        else base[0]
    )
    names = FACILITY_ARGUMENT_NAMES.get(action)
    valid_fields = {"action", *(names or ())}
    invalid_fields = named_fields - valid_fields
    if invalid_fields:
        rendered = ", ".join(sorted(invalid_fields))
        verb = "does" if len(invalid_fields) == 1 else "do"
        raise WapError(
            f"line {line_number}: {rendered} {verb} not apply to facility action "
            f"{_format_facility_action(action)}"
        )

    values = [action, base[1], base[2]]
    if names is not None:
        for index, name in enumerate(names, 1):
            if name in fields:
                values[index] = _integer(
                    fields[name], line_number, f"facility {name}"
                )
    return values[0], values[1], values[2]


def _named_args(
    fields: dict[str, str],
    names: tuple[str | None, str | None, str | None] | None,
    base: tuple[int, int, int],
    line_number: int,
    context: str,
) -> tuple[int, int, int]:
    """Read either a generic argument triple or its dispatch-specific names."""

    named = {name for name in names or () if name is not None}
    present = named.intersection(fields)
    if "args" in fields and present:
        raise WapError(
            f"line {line_number}: {context} args cannot be combined with named arguments"
        )
    if "args" in fields:
        values = _int_list(fields["args"], line_number, 3, f"{context} args")
        return values[0], values[1], values[2]
    values = list(base)
    if names is not None:
        for index, name in enumerate(names):
            if name is not None and name in fields:
                values[index] = _integer(fields[name], line_number, f"{context} {name}")
    return values[0], values[1], values[2]


def parse_source(text: str, references: References | None = None) -> WapFile:
    """Assemble version-1 WAP source on top of its retail profile template."""

    refs = references or References()
    meaningful = [
        (number, line.strip())
        for number, line in enumerate(text.splitlines(), 1)
        if line.strip() and not line.lstrip().startswith("#")
    ]
    if not meaningful:
        raise WapError("empty WAP source")
    first_number, first_line = meaningful[0]
    first = _tokens(first_line, first_number)
    if len(first) != 3 or first[:2] != ["wap", "1"] or not first[2].startswith("profile="):
        raise WapError("source must begin with 'wap 1 profile=PROFILE'")
    profile_name = first[2].split("=", 1)[1]
    try:
        profile = PROFILES[profile_name]
    except KeyError as exc:
        raise WapError(f"unknown WAP profile {profile_name!r}") from exc

    model = default_file(profile)
    elevators = list(model.elevators)
    entries = list(model.entries)
    seen: set[tuple[str, int]] = set()
    source_index = 1
    while source_index < len(meaningful):
        line_number, line = meaningful[source_index]
        tokens = _tokens(line, line_number)
        directive = tokens[0]
        if directive == "elevator":
            if len(tokens) < 2:
                raise WapError(f"line {line_number}: elevator needs an index")
            row_index = _index(
                tokens[1], line_number, profile.elevator_count, "elevator index"
            )
            key = (directive, row_index)
            if key in seen:
                raise WapError(f"line {line_number}: duplicate elevator {row_index}")
            seen.add(key)
            fields = _fields(
                tokens[2:],
                line_number,
                {"area", "sound", "floor_count", "floors", "blocks"},
                directive,
            )
            base = DEFAULT_ELEVATOR
            elevators[row_index] = Elevator(
                _integer(fields["area"], line_number, "area") if "area" in fields else base.area,
                _integer(fields["sound"], line_number, "sound")
                if "sound" in fields
                else base.sound,
                _integer(fields["floor_count"], line_number, "floor count")
                if "floor_count" in fields
                else base.floor_count,
                _int_list(fields["floors"], line_number, 6, "floors")
                if "floors" in fields
                else base.floors,
                _int_list(fields["blocks"], line_number, 6, "blocks")
                if "blocks" in fields
                else base.blocks,
            )
        elif directive == "entry":
            if len(tokens) < 2:
                raise WapError(f"line {line_number}: entry needs an index")
            row_index = _index(tokens[1], line_number, ENTRY_COUNT, "entry index")
            key = (directive, row_index)
            if key in seen:
                raise WapError(f"line {line_number}: duplicate entry {row_index}")
            seen.add(key)
            fields = _fields(
                tokens[2:],
                line_number,
                {"kind", "flag_mode", "flag", "area", "name", "name_padding"},
                directive,
            )
            entry = default_entry(profile)
            entry = replace(
                entry,
                kind=_kind(fields["kind"], line_number)
                if "kind" in fields
                else entry.kind,
                flag_mode=_integer(fields["flag_mode"], line_number, "flag mode")
                if "flag_mode" in fields
                else entry.flag_mode,
                flag=_integer(fields["flag"], line_number, "flag")
                if "flag" in fields
                else entry.flag,
                area=_integer(fields["area"], line_number, "area")
                if "area" in fields
                else entry.area,
                name=_fixed_from_fields(
                    fields,
                    "name",
                    entry.name,
                    line_number,
                    "interaction",
                    refs.interactions,
                ),
            )
            child_seen: set[str] = set()
            source_index += 1
            while source_index < len(meaningful):
                child_number, child_line = meaningful[source_index]
                child = _tokens(child_line, child_number)
                if child == ["end"]:
                    break
                child_kind = child[0]
                if child_kind in child_seen:
                    raise WapError(f"line {child_number}: duplicate {child_kind} row")
                child_seen.add(child_kind)
                if child_kind == "scene":
                    scene_names = _SCENE_ARG_NAMES.get(entry.kind)
                    child_fields = _fields(
                        child[1:],
                        child_number,
                        {
                            "args",
                            "primary",
                            "primary_padding",
                            "secondary",
                            "secondary_padding",
                            *_SCENE_NAMED_FIELDS,
                        },
                        child_kind,
                    )
                    present_scene_fields = _SCENE_NAMED_FIELDS.intersection(child_fields)
                    valid_scene_fields = {
                        name for name in scene_names or () if name is not None
                    }
                    invalid_scene_fields = present_scene_fields - valid_scene_fields
                    if invalid_scene_fields:
                        names = ", ".join(sorted(invalid_scene_fields))
                        verb = "does" if len(invalid_scene_fields) == 1 else "do"
                        raise WapError(
                            f"line {child_number}: {names} {verb} not apply to kind "
                            f"{_format_kind(entry.kind)}"
                        )
                    entry = replace(
                        entry,
                        scene_args=_named_args(
                            child_fields,
                            scene_names,
                            entry.scene_args,
                            child_number,
                            "scene",
                        ),
                        scene_primary=_fixed_from_fields(
                            child_fields,
                            "primary",
                            entry.scene_primary,
                            child_number,
                            "primary scene",
                        ),
                        scene_secondary=_fixed_from_fields(
                            child_fields,
                            "secondary",
                            entry.scene_secondary,
                            child_number,
                            "secondary scene",
                        ),
                    )
                elif child_kind == "warp":
                    child_fields = _fields(
                        child[1:],
                        child_number,
                        {
                            "type",
                            "attributes",
                            "args",
                            "position",
                            "position_padding",
                            *_WARP_NAMED_FIELDS,
                        },
                        child_kind,
                    )
                    warp_type = (
                        _warp_type(child_fields["type"], child_number)
                        if "type" in child_fields
                        else entry.warp_type
                    )
                    warp_names = _WARP_ARG_NAMES.get(warp_type)
                    present_warp_fields = _WARP_NAMED_FIELDS.intersection(child_fields)
                    if warp_type != 2:
                        valid_warp_fields = {
                            name for name in warp_names or () if name is not None
                        }
                        invalid_warp_fields = present_warp_fields - valid_warp_fields
                        if invalid_warp_fields:
                            names = ", ".join(sorted(invalid_warp_fields))
                            verb = "does" if len(invalid_warp_fields) == 1 else "do"
                            raise WapError(
                                f"line {child_number}: {names} {verb} not apply to warp type "
                                f"{_format_warp_type(warp_type)}"
                            )
                    entry = replace(
                        entry,
                        warp_type=warp_type,
                        attributes=_integer(child_fields["attributes"], child_number, "attributes")
                        if "attributes" in child_fields
                        else entry.attributes,
                        warp_args=(
                            _facility_args(child_fields, entry.warp_args, child_number)
                            if warp_type == 2
                            else _named_args(
                                child_fields,
                                warp_names,
                                entry.warp_args,
                                child_number,
                                "warp",
                            )
                        ),
                        position=_fixed_from_fields(
                            child_fields,
                            "position",
                            entry.position,
                            child_number,
                            "position",
                        ),
                    )
                elif child_kind == "camera":
                    child_fields = _fields(
                        child[1:],
                        child_number,
                        {"mode", "table", "name", "name_padding"},
                        child_kind,
                    )
                    entry = replace(
                        entry,
                        camera_mode=_integer(child_fields["mode"], child_number, "camera mode")
                        if "mode" in child_fields
                        else entry.camera_mode,
                        camera_table=_integer(child_fields["table"], child_number, "camera table")
                        if "table" in child_fields
                        else entry.camera_table,
                        camera=_fixed_from_fields(
                            child_fields, "name", entry.camera, child_number, "camera"
                        ),
                    )
                elif child_kind == "after":
                    child_fields = _fields(
                        child[1:],
                        child_number,
                        {"bgm", "footstep", "flag", "script", "script_padding"},
                        child_kind,
                    )
                    entry = replace(
                        entry,
                        bgm=_integer(child_fields["bgm"], child_number, "BGM")
                        if "bgm" in child_fields
                        else entry.bgm,
                        footstep=_integer(child_fields["footstep"], child_number, "footstep")
                        if "footstep" in child_fields
                        else entry.footstep,
                        after_flag=_integer(child_fields["flag"], child_number, "after flag")
                        if "flag" in child_fields
                        else entry.after_flag,
                        after_script=_fixed_from_fields(
                            child_fields,
                            "script",
                            entry.after_script,
                            child_number,
                            "procedure",
                            refs.procedures,
                        ),
                    )
                elif child_kind == "tail":
                    if profile.entry_size == LEGACY_ENTRY_SIZE:
                        raise WapError(f"line {child_number}: legacy profile has no DDS tail")
                    child_fields = _fields(
                        child[1:], child_number, {"control", "args"}, child_kind
                    )
                    if child_fields.keys() != {"control", "args"}:
                        raise WapError(
                            f"line {child_number}: tail needs control and seven args"
                        )
                    entry = replace(
                        entry,
                        tail=(
                            _integer(child_fields["control"], child_number, "tail control"),
                            *_int_list(child_fields["args"], child_number, 7, "tail args"),
                        ),
                    )
                else:
                    raise WapError(
                        f"line {child_number}: expected scene, warp, camera, after, tail, or end"
                    )
                source_index += 1
            if source_index == len(meaningful):
                raise WapError(f"line {line_number}: entry has no end")
            entries[row_index] = entry
        else:
            raise WapError(f"line {line_number}: unknown directive {directive!r}")
        source_index += 1

    wap = WapFile(profile, tuple(elevators), tuple(entries))
    encode(wap)
    return wap


def _append_scalar(fields: list[str], key: str, value: int, default: int) -> None:
    if value != default:
        fields.append(f"{key}={value}")


def _format_fixed(
    fields: list[str],
    key: str,
    value: FixedString,
    default: FixedString,
    symbols: frozenset[str] | None = None,
) -> None:
    if value.value != default.value or value.padding != default.padding:
        rendered = (
            f"@{value.value}"
            if value.value and not value.padding and symbols is not None and value.value in symbols
            else json.dumps(value.value)
        )
        fields.append(f"{key}={rendered}")
    if value.padding:
        fields.append(f"{key}_padding={value.padding.hex()}")


def _format_list(values: tuple[int, ...]) -> str:
    return ",".join(str(value) for value in values)


def _format_kind(value: int) -> str:
    return ENTRY_KIND_NAMES.get(value, str(value))


def _format_warp_type(value: int) -> str:
    return WARP_TYPE_NAMES.get(value, str(value))


def _format_facility_action(value: int) -> str:
    return FACILITY_ACTION_NAMES.get(value, str(value))


def _format_named_args(
    fields: list[str],
    values: tuple[int, ...],
    names: tuple[str | None, str | None, str | None] | None,
) -> None:
    """Prefer dispatch-specific scalar names when they describe the whole triple."""

    if names is not None and all(
        name is not None or values[index] == 0 for index, name in enumerate(names)
    ):
        for name, value in zip(names, values):
            if name is not None and value != 0:
                fields.append(f"{name}={value}")
    elif any(values):
        fields.append(f"args={_format_list(values)}")


def _format_warp_args(
    fields: list[str], warp_type: int, values: tuple[int, int, int]
) -> None:
    if warp_type != 2:
        _format_named_args(fields, values, _WARP_ARG_NAMES.get(warp_type))
        return

    names = FACILITY_ARGUMENT_NAMES.get(values[0])
    if names is None:
        if any(values):
            fields.append(f"args={_format_list(values)}")
        return
    fields.append(f"action={_format_facility_action(values[0])}")
    for name, value in zip(names, values[1:]):
        if value != 0:
            fields.append(f"{name}={value}")


def render_source(wap: WapFile, references: References | None = None) -> str:
    """Render compact canonical source relative to the profile template."""

    encode(wap)
    refs = references or References()
    lines = [f"wap 1 profile={wap.profile.name}", ""]
    for index, elevator in enumerate(wap.elevators):
        if elevator == DEFAULT_ELEVATOR:
            continue
        fields: list[str] = []
        _append_scalar(fields, "area", elevator.area, DEFAULT_ELEVATOR.area)
        _append_scalar(fields, "sound", elevator.sound, DEFAULT_ELEVATOR.sound)
        _append_scalar(
            fields, "floor_count", elevator.floor_count, DEFAULT_ELEVATOR.floor_count
        )
        if elevator.floors != DEFAULT_ELEVATOR.floors:
            fields.append(f"floors={_format_list(elevator.floors)}")
        if elevator.blocks != DEFAULT_ELEVATOR.blocks:
            fields.append(f"blocks={_format_list(elevator.blocks)}")
        lines.append(f"elevator {index} {' '.join(fields)}")
    if any(elevator != DEFAULT_ELEVATOR for elevator in wap.elevators):
        lines.append("")

    base = default_entry(wap.profile)
    for index, entry in enumerate(wap.entries):
        if entry == base:
            continue
        fields = []
        if entry.kind != base.kind:
            fields.append(f"kind={_format_kind(entry.kind)}")
        _append_scalar(fields, "flag_mode", entry.flag_mode, base.flag_mode)
        _append_scalar(fields, "flag", entry.flag, base.flag)
        _append_scalar(fields, "area", entry.area, base.area)
        _format_fixed(fields, "name", entry.name, base.name, refs.interactions)
        suffix = f" {' '.join(fields)}" if fields else ""
        lines.append(f"entry {index}{suffix}")

        child = []
        _format_named_args(child, entry.scene_args, _SCENE_ARG_NAMES.get(entry.kind))
        _format_fixed(child, "primary", entry.scene_primary, base.scene_primary)
        _format_fixed(child, "secondary", entry.scene_secondary, base.scene_secondary)
        if child:
            lines.append(f"  scene {' '.join(child)}")

        child = []
        if entry.warp_type != base.warp_type:
            child.append(f"type={_format_warp_type(entry.warp_type)}")
        _append_scalar(child, "attributes", entry.attributes, base.attributes)
        _format_warp_args(child, entry.warp_type, entry.warp_args)
        _format_fixed(child, "position", entry.position, base.position)
        if child:
            lines.append(f"  warp {' '.join(child)}")

        child = []
        _append_scalar(child, "mode", entry.camera_mode, base.camera_mode)
        _append_scalar(child, "table", entry.camera_table, base.camera_table)
        _format_fixed(child, "name", entry.camera, base.camera)
        if child:
            lines.append(f"  camera {' '.join(child)}")

        child = []
        _append_scalar(child, "bgm", entry.bgm, base.bgm)
        _append_scalar(child, "footstep", entry.footstep, base.footstep)
        _append_scalar(child, "flag", entry.after_flag, base.after_flag)
        _format_fixed(
            child,
            "script",
            entry.after_script,
            base.after_script,
            refs.procedures,
        )
        if child:
            lines.append(f"  after {' '.join(child)}")

        if entry.tail != base.tail:
            lines.append(
                f"  tail control={entry.tail[0]} args={_format_list(entry.tail[1:])}"
            )
        lines.extend(("end", ""))
    return "\n".join(lines).rstrip() + "\n"


def load_references(
    script_path: Path | None = None, interaction_path: Path | None = None
) -> References:
    """Load exact procedure and interaction names from paired editable sources."""

    procedures: frozenset[str] | None = None
    if script_path is not None:
        try:
            import flw0

            script = flw0.parse_source(script_path.read_text(encoding="utf-8"))
            procedures = frozenset(row.name for row in script.named_rows(0))
        except (OSError, ValueError) as exc:
            raise WapError(f"cannot read procedure symbols from {script_path}: {exc}") from exc

    interactions: frozenset[str] | None = None
    if interaction_path is not None:
        names: set[str] = set()
        try:
            for line_number, line in enumerate(
                interaction_path.read_text(encoding="utf-8").splitlines(), 1
            ):
                tokens = _tokens(line, line_number)
                if not tokens or tokens[0] not in ("hit", "set"):
                    continue
                for token in tokens[2:]:
                    if token.startswith("event="):
                        names.add(token.split("=", 1)[1])
        except OSError as exc:
            raise WapError(
                f"cannot read interaction symbols from {interaction_path}: {exc}"
            ) from exc
        interactions = frozenset(names)
    return References(interactions, procedures)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    commands = parser.add_subparsers(dest="command", required=True)

    disassemble = commands.add_parser("disassemble", help="write readable WAP source")
    disassemble.add_argument("input", type=Path)
    disassemble.add_argument("output", nargs="?", type=Path)
    disassemble.add_argument("--scripts", type=Path, help="paired BF source")
    disassemble.add_argument("--interactions", type=Path, help="paired INF source")

    assemble = commands.add_parser("assemble", help="assemble readable WAP source")
    assemble.add_argument("input", type=Path)
    assemble.add_argument("output", type=Path)
    assemble.add_argument("--scripts", type=Path, help="paired BF source")
    assemble.add_argument("--interactions", type=Path, help="paired INF source")

    verify = commands.add_parser("verify", help="validate a WAP file and its exact rewrite")
    verify.add_argument("input", type=Path)
    args = parser.parse_args()

    try:
        if args.command == "verify":
            data = args.input.read_bytes()
            if encode(decode(data)) != data:
                raise WapError("decoded WAP does not rewrite exactly")
            print(f"{args.input}: valid ({len(data):#x} bytes)")
            return 0

        references = load_references(args.scripts, args.interactions)
        if args.command == "disassemble":
            source = render_source(decode(args.input.read_bytes()), references)
            if args.output is None:
                sys.stdout.write(source)
            else:
                args.output.parent.mkdir(parents=True, exist_ok=True)
                args.output.write_text(source, encoding="utf-8")
            return 0

        data = encode(
            parse_source(args.input.read_text(encoding="utf-8"), references)
        )
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_bytes(data)
        return 0
    except (OSError, WapError) as exc:
        parser.error(str(exc))
    return 2


if __name__ == "__main__":
    raise SystemExit(main())
