#!/usr/bin/env python3
"""Disassemble and assemble relocatable DDS ``FLD1``/``FLD2`` resources.

The format is a compact object file.  Its data region contains file-relative
pointers and its tail encodes the locations of those pointer words.  The source
language keeps the physical order explicit, gives every pointer a label, and
uses typed directives for the field structures whose contracts are known.
Unknown regions remain ordinary bytes and pointers, so they are lossless
without preventing the rest of the file from relocating.
"""

from __future__ import annotations

import argparse
import bisect
import hashlib
import json
import math
import re
import shlex
import struct
import sys
from dataclasses import dataclass
from pathlib import Path


HEADER_SIZE = 0x40
TYPE_SIZE = 0x0C
RESOURCE_SIZE = 0x24
TRANSFORM_SIZE = 0x30
AREA_SIZE = 0x20
COLLISION_SIZE = 0x30
VERTEX_SIZE = 0x10
FACE_SIZE = 0x24
ENCOUNTER_ZONE_ATTRIBUTE = 0x2000
STRING_SIZE = 0x10
EFFECT_SIZE = 0x30
LIGHT_SIZE = 0x34
MODEL_RESOURCE_SIZE = 0x24
MODEL_ITEM_LIST_SIZE = 0x10
MODEL_ITEM_SIZE = 0x50
MODEL_BOUNDS_SIZE = 0x18
MODEL_DRAW_SIZE = 0x10
MESH_RECORDS_PER_LINE = 8
MESH_DATA_DIRECTIVES = {
    "triangle": (4, 4),
    "position": (12, 3),
    "normal": (12, 3),
    "texcoord": (8, 2),
    "attribute": (16, 4),
    "color": (4, 4),
}
MESH_SECTION_DIRECTIVES = {
    "mesh_triangles",
    "mesh_positions",
    "mesh_normals",
    "mesh_texcoords",
    "mesh_attributes",
    "mesh_colors",
}
MESH_PACKET_DIRECTIVES = (
    {"mesh_header", "mesh_program", "vif_nops"}
    | MESH_SECTION_DIRECTIVES
    | set(MESH_DATA_DIRECTIVES)
)
MODEL_ASSET_FIELDS = (
    (0x001, "word_01", "u32", 1),
    (0x002, "word_02", "u32", 1),
    (0x004, "resource_04", "resource", 1),
    (0x008, "values_08", "float", 5),
    (0x010, "word_10", "u32", 1),
    (0x020, "resource_20", "resource_pair", 2),
    (0x040, "values_40", "float", 5),
    (0x080, "word_80", "u32", 1),
    (0x100, "scalar_100", "float", 1),
    (0x200, "scalar_200", "float", 1),
    (0x400, "pair_400", "float", 2),
)
MODEL_ASSET_FIELD_NAMES = {name for _, name, _, _ in MODEL_ASSET_FIELDS}
MODEL_ASSET_FLAG_MASK = sum(bit for bit, _, _, _ in MODEL_ASSET_FIELDS)
MODEL_MOTION_FAMILIES = {0: "node", 1: "asset"}
MODEL_MOTION_FAMILY_IDS = {name: value for value, name in MODEL_MOTION_FAMILIES.items()}
MODEL_MOTION_NODE_SELECTORS = {
    0: "translation",
    1: "euler_rotation",
    2: "scale",
    3: "quaternion",
    4: "flag",
}
MODEL_MOTION_NODE_SELECTOR_IDS = {
    name: value for value, name in MODEL_MOTION_NODE_SELECTORS.items()
}
MODEL_MOTION_FORMATS = {
    (0, 0): ("vector3", 12),
    (0, 1): ("vector3", 12),
    (0, 2): ("vector3", 12),
    (0, 3): ("quaternion_s16", 8),
    (0, 4): ("flag_u8", 1),
    (1, 0): ("rgba8", 4),
    (1, 1): ("rgba8", 4),
    (1, 2): ("float5", 20),
    (1, 3): ("float5", 20),
    (1, 4): ("rgba8", 4),
    (1, 5): ("float", 4),
    (1, 6): ("float5", 20),
    (1, 7): ("float5", 20),
    (1, 8): ("rgba8", 4),
    (1, 9): ("rgba8", 4),
}
MODEL_MOTION_VALUE_DIRECTIVES = {
    "vector3": ("motion_vector3", 12, 3, "float"),
    "quaternion_s16": ("motion_quaternion_s16", 8, 4, "s16"),
    "flag_u8": ("motion_flag_u8", 1, 1, "u8"),
    "rgba8": ("motion_rgba8", 4, 4, "u8"),
    "float5": ("motion_float5", 20, 5, "float"),
    "float": ("motion_float", 4, 1, "float"),
}
MODEL_MOTION_DIRECTIVE_FORMATS = {
    directive: format_name
    for format_name, (directive, _, _, _) in MODEL_MOTION_VALUE_DIRECTIVES.items()
}
MOTION_CURVE_SIZE = 0x10
MOTION_KINDS = {
    0: ("vector3", 3),
    2: ("quaternion", 4),
    4: ("scalar", 1),
    5: ("light", 10),
}
MOTION_KIND_IDS = {name: kind for kind, (name, _) in MOTION_KINDS.items()}
SPECIAL_POINT_KINDS = {1: "save", 2: "heal", 3: "hunt"}
SPECIAL_POINT_KIND_IDS = {name: kind for kind, name in SPECIAL_POINT_KINDS.items()}


class FldError(ValueError):
    """Raised when FLD data or source is invalid."""


@dataclass(frozen=True)
class TypeRow:
    type_id: int
    count: int
    resources: int


@dataclass(frozen=True)
class Resource:
    offset: int
    serial: int
    flags: int
    type_id: int
    name: int
    reserved: int
    transform: int
    area: int
    link: int
    sblock: int
    data: int


@dataclass(frozen=True)
class Operation:
    line: int
    name: str
    args: tuple[str, ...]


@dataclass(frozen=True)
class LinkSummary:
    event_procedures: int
    actor_resources: int
    destination_resources: int


@dataclass(frozen=True)
class MotionTrack:
    kind: int
    curve: int
    count: int
    values: int
    keys: int
    word_0c: int


@dataclass(frozen=True)
class ModelResource:
    flags: int
    lod_count: int
    reserved_08: int
    reserved_0c: int
    items: int
    assets: int
    slot_count: int
    parameter: int
    motion: int


@dataclass(frozen=True)
class ModelItem:
    command_mode: int
    reserved_02: int
    word_04: int
    node_id: int
    parent: int
    rotation: tuple[float, float, float]
    reserved_1c: int
    position: tuple[float, float, float, float]
    scale: tuple[float, float, float, float]
    bounds: int
    commands: int


@dataclass(frozen=True)
class ModelAsset:
    index: int
    flags: int
    fields: tuple[tuple[str, tuple[int | float, ...]], ...]
    size: int


@dataclass(frozen=True)
class ModelDraw:
    offset: int
    asset: int
    quadwords: int
    packet: int


@dataclass(frozen=True)
class ModelDrawList:
    offset: int
    selector: int
    draws: tuple[int, ...]


@dataclass(frozen=True)
class ModelMesh:
    triangles: tuple[tuple[int, int, int, int], ...]
    positions: tuple[tuple[float, float, float], ...]
    normals: tuple[tuple[float, float, float], ...] | None
    texcoords: tuple[tuple[float, float], ...] | None
    attributes: tuple[tuple[float, float, float, float], ...] | None
    colors: tuple[tuple[int, int, int, int], ...] | None
    controls: tuple[int, int]
    program: int


@dataclass(frozen=True)
class ModelMotionBinding:
    family: int
    selector: int
    target: int


@dataclass(frozen=True)
class ModelMotionTrack:
    offset: int
    size: int
    count: int
    stride: int
    frames: tuple[int, ...]
    values: int
    format_name: str


@dataclass(frozen=True)
class ModelMotionClip:
    offset: int
    duration: int
    reserved: int
    tracks: tuple[ModelMotionTrack, ...]


@dataclass(frozen=True)
class ModelMotionPlaybook:
    offset: int
    clip_table: int
    bindings: tuple[ModelMotionBinding, ...]
    clips: tuple[ModelMotionClip | None, ...]


def _face_encounter_zone(face: tuple[int, ...], face_index: int) -> int | None:
    tagged = bool(face[0] & ENCOUNTER_ZONE_ATTRIBUTE)
    encounter_type, zone = face[11], face[12]
    if tagged:
        if encounter_type != 1 or zone < 0:
            raise FldError(
                f"collision face {face_index} encounter-zone tag is "
                f"{{{encounter_type}, {zone}}}, expected {{1, nonnegative zone}}"
            )
        return zone
    if encounter_type != 0 or zone != 0:
        raise FldError(
            f"collision face {face_index} has encounter values "
            f"{{{encounter_type}, {zone}}} without attribute "
            f"0x{ENCOUNTER_ZONE_ATTRIBUTE:x}"
        )
    return None


def encounter_zone_overrides(data: bytes) -> tuple[int, ...]:
    """Return collision-face encounter zones after validating their tag pair."""

    words, data_end, _ = _read_header(data)
    resources = _read_resources(data, _read_types(data, words, data_end))
    zones: list[int] = []
    for resource in resources:
        if resource.type_id != 3 or not resource.data:
            continue
        _range(data, resource.data, COLLISION_SIZE, "collision header")
        values = struct.unpack_from("<12I", data, resource.data)
        face_count, faces = values[5], values[8]
        _range(data, faces, face_count * FACE_SIZE, "collision faces")
        for face_index in range(face_count):
            face = struct.unpack_from(
                "<IBBH HBB 4I hhhh", data, faces + face_index * FACE_SIZE
            )
            zone = _face_encounter_zone(face, face_index)
            if zone is not None:
                zones.append(zone)
    return tuple(zones)


def _range(data: bytes, offset: int, size: int, context: str) -> None:
    if offset < 0 or size < 0 or offset + size > len(data):
        raise FldError(f"{context} lies outside the FLD file")


def _data_range(
    data: bytes, offset: int, size: int, data_end: int, context: str
) -> None:
    _range(data, offset, size, context)
    if offset + size > data_end:
        raise FldError(f"{context} lies outside the data region")


def _u32(data: bytes, offset: int, context: str) -> int:
    _range(data, offset, 4, context)
    return struct.unpack_from("<I", data, offset)[0]


def _s32(data: bytes, offset: int, context: str) -> int:
    _range(data, offset, 4, context)
    return struct.unpack_from("<i", data, offset)[0]


def _decode_relocations(data: bytes) -> tuple[int, ...]:
    """Decode the field runtime's packed u32-word delta stream."""

    cursor = 0
    index = 0
    locations: list[int] = []
    while index < len(data):
        code = data[index]
        index += 1
        if not code & 1:
            delta = code >> 1
        elif not code & 2:
            if index >= len(data):
                raise FldError("truncated two-byte relocation")
            delta = (code | data[index] << 8) >> 2
            index += 1
        elif not code & 4:
            if index + 1 >= len(data):
                raise FldError("truncated three-byte relocation")
            delta = (code | data[index] << 8 | data[index + 1] << 16) >> 3
            index += 2
        else:
            count = (code >> 3) + 2
            for _ in range(count):
                cursor += 1
                locations.append(cursor * 4)
            continue
        cursor += delta
        locations.append(cursor * 4)
    if locations != sorted(set(locations)):
        raise FldError("relocation locations are not strictly ordered")
    return tuple(locations)


def _encode_relocations(locations: list[int]) -> bytes:
    if locations != sorted(set(locations)):
        raise FldError("relocation locations are not strictly ordered")
    if any(location & 3 for location in locations):
        raise FldError("relocation location is not u32-aligned")

    words = [location // 4 for location in locations]
    output = bytearray()
    previous = 0
    index = 0

    def emit_delta(delta: int) -> None:
        if delta < 0:
            raise FldError("negative relocation delta")
        if delta <= 0x7F:
            output.append(delta << 1)
        elif delta < 0x4000:
            value = (delta << 2) | 1
            output.extend((value & 0xFF, value >> 8))
        elif delta < 0x200000:
            value = (delta << 3) | 3
            output.extend((value & 0xFF, (value >> 8) & 0xFF, value >> 16))
        else:
            raise FldError("relocation delta is too large")

    def emit_run(count: int) -> None:
        nonlocal index, previous
        while count:
            chunk = min(count, 33)
            output.append(((chunk - 2) << 3) | 7)
            previous += chunk
            index += chunk
            count -= chunk

    while index < len(words):
        if words[index] - previous == 1:
            end = index + 1
            while end < len(words) and words[end] == words[end - 1] + 1:
                end += 1
            if end - index >= 2:
                emit_run(end - index)
                continue

        emit_delta(words[index] - previous)
        previous = words[index]
        index += 1

        end = index
        while end < len(words) and words[end] == previous + end - index + 1:
            end += 1
        if end - index >= 2:
            emit_run(end - index)

    return bytes(output)


def _read_header(data: bytes) -> tuple[list[int], int, tuple[int, ...]]:
    _range(data, 0, HEADER_SIZE, "header")
    words = list(struct.unpack_from("<16I", data))
    magic = data[4:8]
    if words[0] != 23 or magic not in (b"FLD1", b"FLD2"):
        raise FldError("expected version-23 FLD1 or FLD2 data")
    data_end = words[2]
    if words[3] != data_end:
        raise FldError("FLD data-size fields disagree")
    if data_end < HEADER_SIZE or data_end + words[4] != len(data):
        raise FldError("FLD data and relocation sizes do not match the file")
    relocations = _decode_relocations(data[data_end:])
    if _encode_relocations(list(relocations)) != data[data_end:]:
        raise FldError("FLD uses a noncanonical relocation stream")
    for location in relocations:
        if location + 4 > data_end:
            raise FldError(f"relocation word at 0x{location:x} lies outside the data region")
        target = _u32(data, location, "relocation target")
        if target > data_end:
            raise FldError(f"relocation at 0x{location:x} targets 0x{target:x}")
    return words, data_end, relocations


def _read_types(data: bytes, words: list[int], data_end: int) -> tuple[TypeRow, ...]:
    count = words[5]
    offset = words[6]
    if offset + count * TYPE_SIZE > data_end:
        raise FldError("resource type table exceeds the data region")
    rows = []
    for index in range(count):
        type_id, resource_count, resources = struct.unpack_from(
            "<III", data, offset + index * TYPE_SIZE
        )
        if resources + resource_count * RESOURCE_SIZE > data_end:
            raise FldError(f"type {type_id} resource table exceeds the data region")
        rows.append(TypeRow(type_id, resource_count, resources))
    return tuple(rows)


def _read_resources(data: bytes, rows: tuple[TypeRow, ...]) -> tuple[Resource, ...]:
    resources = []
    for row in rows:
        for index in range(row.count):
            offset = row.resources + index * RESOURCE_SIZE
            values = struct.unpack_from("<9I", data, offset)
            if values[1] != row.type_id:
                raise FldError(
                    f"resource at 0x{offset:x} has type {values[1]}, expected {row.type_id}"
                )
            resources.append(
                Resource(
                    offset=offset,
                    serial=values[0] & 0xFFFF,
                    flags=values[0] >> 16,
                    type_id=values[1],
                    name=values[2],
                    reserved=values[3],
                    transform=values[4],
                    area=values[5],
                    link=values[6],
                    sblock=values[7],
                    data=values[8],
                )
            )
    return tuple(resources)


def _read_model_resource(
    data: bytes, offset: int, data_end: int, context: str
) -> tuple[ModelResource, tuple[ModelItem, ...]]:
    _range(data, offset, MODEL_RESOURCE_SIZE, context)
    values = struct.unpack_from("<9I", data, offset)
    resource = ModelResource(*values)
    if (
        resource.flags != 0
        or resource.lod_count != 1
        or resource.reserved_08 != 0
        or resource.reserved_0c != 0
        or resource.slot_count != 0
        or resource.parameter != 0
    ):
        raise FldError(f"{context} has an unsupported model-resource profile")
    if not resource.items:
        raise FldError(f"{context} has no model-item list")
    if not resource.assets or not resource.motion:
        raise FldError(f"{context} has a null asset or motion definition")

    if resource.items + MODEL_ITEM_LIST_SIZE > data_end:
        raise FldError(f"{context} model-item list lies outside the data region")
    _range(data, resource.items, MODEL_ITEM_LIST_SIZE, context + " model-item list")
    count, word_04, word_08, word_0c = struct.unpack_from("<4I", data, resource.items)
    if word_04 != 0 or word_08 != 0 or word_0c != 0:
        raise FldError(f"{context} model-item list has nonzero reserved words")
    if resource.items + MODEL_ITEM_LIST_SIZE + count * MODEL_ITEM_SIZE > data_end:
        raise FldError(f"{context} model items lie outside the data region")
    _range(
        data,
        resource.items + MODEL_ITEM_LIST_SIZE,
        count * MODEL_ITEM_SIZE,
        context + " model items",
    )
    items = []
    for index in range(count):
        item_offset = resource.items + MODEL_ITEM_LIST_SIZE + index * MODEL_ITEM_SIZE
        command_mode, reserved_02, word_04, node_id, parent = struct.unpack_from(
            "<HHIIi", data, item_offset
        )
        rotation = struct.unpack_from("<3f", data, item_offset + 0x10)
        reserved_1c = _u32(data, item_offset + 0x1C, context + " item reserved word")
        position = struct.unpack_from("<4f", data, item_offset + 0x20)
        scale = struct.unpack_from("<4f", data, item_offset + 0x30)
        bounds, command_0, command_1, command_2 = struct.unpack_from(
            "<4I", data, item_offset + 0x40
        )
        if (
            command_mode != 1
            or reserved_02 != 0
            or word_04 != 0
            or node_id != index
            or reserved_1c != 0
            or command_1 != 0
            or command_2 != 0
        ):
            raise FldError(f"{context} model item {index} has an unsupported profile")
        if parent < -1 or parent >= count:
            raise FldError(
                f"{context} model item {index} has parent {parent}, expected -1..{count - 1}"
            )
        if bounds:
            if bounds + MODEL_BOUNDS_SIZE > data_end:
                raise FldError(
                    f"{context} model item {index} bounds lie outside the data region"
                )
            _range(data, bounds, MODEL_BOUNDS_SIZE, context + f" model item {index} bounds")
        if bool(bounds) != bool(command_0):
            raise FldError(
                f"{context} model item {index} has only one of bounds and commands"
            )
        if command_0:
            if command_0 + 4 > data_end:
                raise FldError(
                    f"{context} model item {index} commands lie outside the data region"
                )
            _range(data, command_0, 4, context + f" model item {index} commands")
        items.append(
            ModelItem(
                command_mode,
                reserved_02,
                word_04,
                node_id,
                parent,
                rotation,
                reserved_1c,
                position,
                scale,
                bounds,
                command_0,
            )
        )
    return resource, tuple(items)


def _read_model_assets(
    data: bytes, offset: int, data_end: int, context: str
) -> tuple[ModelAsset, ...]:
    _data_range(data, offset, 4, data_end, context)
    count = _u32(data, offset, context + " count")
    cursor = offset + 4
    assets = []
    for index in range(count):
        _data_range(data, cursor, 8, data_end, context + f" asset {index}")
        entry_start = cursor
        identifier, reserved, flags = struct.unpack_from("<IHH", data, cursor)
        cursor += 8
        if identifier != index:
            raise FldError(
                f"{context} asset {index} has identifier {identifier}"
            )
        if reserved != 0:
            raise FldError(f"{context} asset {index} has a nonzero reserved word")
        if flags & ~MODEL_ASSET_FLAG_MASK:
            raise FldError(
                f"{context} asset {index} has unsupported flags 0x{flags:x}"
            )

        fields = []
        for bit, name, kind, width in MODEL_ASSET_FIELDS:
            if not flags & bit:
                continue
            size = 4 if kind in {"resource", "resource_pair"} else width * 4
            _data_range(
                data, cursor, size, data_end, context + f" asset {index} {name}"
            )
            if kind == "u32":
                values = struct.unpack_from("<" + "I" * width, data, cursor)
            elif kind == "float":
                values = struct.unpack_from("<" + "f" * width, data, cursor)
            elif kind == "resource":
                resource_index, padding = struct.unpack_from("<HH", data, cursor)
                if padding != 0:
                    raise FldError(
                        f"{context} asset {index} {name} has nonzero padding"
                    )
                values = (resource_index,)
            elif kind == "resource_pair":
                values = struct.unpack_from("<HH", data, cursor)
            else:
                raise AssertionError(kind)
            fields.append((name, values))
            cursor += size
        assets.append(ModelAsset(index, flags, tuple(fields), cursor - entry_start))
    return tuple(assets)


def _model_motion_format(family: int, selector: int, context: str) -> tuple[str, int]:
    try:
        return MODEL_MOTION_FORMATS[family, selector]
    except KeyError as exc:
        raise FldError(
            f"{context} uses unsupported motion family {family}, selector {selector}"
        ) from exc


def _read_model_motion(
    data: bytes,
    offset: int,
    data_end: int,
    relocations: set[int],
    node_count: int,
    asset_count: int,
    context: str,
) -> ModelMotionPlaybook:
    """Read the model motion playbook and its size-prefixed key tracks."""

    _data_range(data, offset, 8, data_end, context)
    clip_count, binding_count, clip_table = struct.unpack_from("<HHI", data, offset)
    if not clip_count:
        raise FldError(f"{context} has an empty clip table")
    if offset + 4 not in relocations or not clip_table:
        raise FldError(f"{context} clip-table pointer is not relocated")
    _data_range(
        data,
        offset + 8,
        binding_count * 8,
        data_end,
        context + " bindings",
    )

    bindings = []
    for index in range(binding_count):
        command, target = struct.unpack_from("<II", data, offset + 8 + index * 8)
        family, selector = command >> 16, command & 0xFFFF
        _model_motion_format(family, selector, context + f" binding {index}")
        target_count = node_count if family == 0 else asset_count
        if target >= target_count:
            raise FldError(
                f"{context} binding {index} targets {target}, but family "
                f"{MODEL_MOTION_FAMILIES[family]} has only {target_count} entries"
            )
        bindings.append(ModelMotionBinding(family, selector, target))

    _data_range(
        data,
        clip_table,
        clip_count * 4,
        data_end,
        context + " clip table",
    )
    clips: list[ModelMotionClip | None] = []
    for clip_index in range(clip_count):
        pointer_offset = clip_table + clip_index * 4
        clip_offset = _u32(data, pointer_offset, context + " clip pointer")
        if not clip_offset:
            if pointer_offset in relocations:
                raise FldError(f"{context} null clip pointer is relocated")
            clips.append(None)
            continue
        if pointer_offset not in relocations:
            raise FldError(f"{context} clip {clip_index} pointer is not relocated")
        _data_range(data, clip_offset, 4, data_end, context + f" clip {clip_index}")
        duration, reserved = struct.unpack_from("<HH", data, clip_offset)
        cursor = clip_offset + 4
        tracks = []
        for binding_index, binding in enumerate(bindings):
            track_context = (
                context + f" clip {clip_index} binding {binding_index} track"
            )
            _data_range(data, cursor, 8, data_end, track_context)
            size, count, stride = struct.unpack_from("<IHH", data, cursor)
            format_name, expected_stride = _model_motion_format(
                binding.family, binding.selector, track_context
            )
            if not count:
                raise FldError(f"{track_context} has no keys")
            if stride != expected_stride:
                raise FldError(
                    f"{track_context} stride is {stride}, expected {expected_stride}"
                )
            frame_size = (count * 2 + 3) & ~3
            expected_size = 8 + frame_size + count * stride
            if size != expected_size:
                raise FldError(
                    f"{track_context} size is {size}, expected {expected_size}"
                )
            _data_range(data, cursor, size, data_end, track_context)
            frames = struct.unpack_from(f"<{count}H", data, cursor + 8)
            padding = data[cursor + 8 + count * 2 : cursor + 8 + frame_size]
            if any(padding):
                raise FldError(f"{track_context} has nonzero frame padding")
            values = cursor + 8 + frame_size
            tracks.append(
                ModelMotionTrack(
                    cursor,
                    size,
                    count,
                    stride,
                    frames,
                    values,
                    format_name,
                )
            )
            cursor += size
        clips.append(ModelMotionClip(clip_offset, duration, reserved, tuple(tracks)))
    return ModelMotionPlaybook(offset, clip_table, tuple(bindings), tuple(clips))


def _read_model_draw_graph(
    data: bytes,
    items: tuple[ModelItem, ...],
    asset_count: int,
    data_end: int,
    relocations: set[int],
    context: str,
) -> tuple[dict[int, tuple[int, ...]], dict[int, ModelDrawList], dict[int, ModelDraw]]:
    roots: dict[int, tuple[int, ...]] = {}
    lists: dict[int, ModelDrawList] = {}
    draws: dict[int, ModelDraw] = {}

    for item_index, item in enumerate(items):
        root = item.commands
        if not root or root in roots:
            continue
        list_offsets = []
        cursor = root
        while True:
            _data_range(
                data,
                cursor,
                4,
                data_end,
                context + f" item {item_index} draw-list set",
            )
            list_offset = _u32(data, cursor, context + " draw-list pointer")
            if list_offset == 0:
                if cursor in relocations:
                    raise FldError(f"{context} draw-list terminator is relocated")
                break
            if cursor not in relocations:
                raise FldError(f"{context} draw-list pointer is not relocated")
            list_offsets.append(list_offset)
            cursor += 4
        roots[root] = tuple(list_offsets)

        for list_offset in list_offsets:
            if list_offset in lists:
                continue
            _data_range(data, list_offset, 4, data_end, context + " draw list")
            header = _u32(data, list_offset, context + " draw-list header")
            count, selector = header & 0xFFFF, header >> 16
            draw_offsets = []
            for draw_index in range(count):
                pointer_offset = list_offset + 4 + draw_index * 4
                _data_range(
                    data,
                    pointer_offset,
                    4,
                    data_end,
                    context + " draw pointer",
                )
                if pointer_offset not in relocations:
                    raise FldError(f"{context} draw pointer is not relocated")
                draw_offset = _u32(data, pointer_offset, context + " draw pointer")
                if not draw_offset:
                    raise FldError(f"{context} draw pointer is null")
                draw_offsets.append(draw_offset)

                if draw_offset in draws:
                    continue
                _data_range(
                    data,
                    draw_offset,
                    MODEL_DRAW_SIZE,
                    data_end,
                    context + " draw",
                )
                opcode, packed, packet, reserved = struct.unpack_from(
                    "<4I", data, draw_offset
                )
                if opcode != 1:
                    raise FldError(f"{context} uses unsupported draw opcode {opcode}")
                if draw_offset + 8 not in relocations:
                    raise FldError(f"{context} draw packet pointer is not relocated")
                if reserved != 0:
                    raise FldError(f"{context} draw has a nonzero reserved word")
                quadwords, asset = packed & 0xFFFF, packed >> 16
                if asset >= asset_count:
                    raise FldError(
                        f"{context} draw selects asset {asset}, but only "
                        f"{asset_count} assets exist"
                    )
                _data_range(
                    data,
                    packet,
                    quadwords * 0x10,
                    data_end,
                    context + " draw packet",
                )
                draws[draw_offset] = ModelDraw(
                    draw_offset, asset, quadwords, packet
                )
            lists[list_offset] = ModelDrawList(
                list_offset, selector, tuple(draw_offsets)
            )
    return roots, lists, draws


def _vif_unpack_code(format_id: int, count: int, address: int) -> int:
    """Build the canonical masked, unsigned, double-buffered VIF UNPACK word."""

    if not 1 <= count <= 0x100:
        raise FldError(f"VIF UNPACK count {count} is outside 1..256")
    if not 0 <= address <= 0x3FF:
        raise FldError(f"VIF UNPACK address {address} exceeds 10 bits")
    return (
        (0x60 | format_id) << 24
        | (count & 0xFF) << 16
        | 0xC000
        | address
    )


def _read_model_mesh_packet(
    data: bytes, offset: int, size: int, context: str
) -> tuple[tuple[ModelMesh, ...], int]:
    """Decode the VIF mesh stream used by retail FLD1 model draws."""

    if size <= 0 or size % 0x10:
        raise FldError(f"{context} size must be a positive multiple of 16")
    _range(data, offset, size, context)
    cursor, end = offset, offset + size
    meshes: list[ModelMesh] = []

    def word(expected_context: str) -> int:
        nonlocal cursor
        if cursor + 4 > end:
            raise FldError(f"{context} ends before {expected_context}")
        value = struct.unpack_from("<I", data, cursor)[0]
        cursor += 4
        return value

    def expect_unpack(format_id: int, count: int, address: int, label: str) -> None:
        actual = word(label)
        expected = _vif_unpack_code(format_id, count, address)
        if actual != expected:
            raise FldError(
                f"{context} {label} is 0x{actual:08x}, expected 0x{expected:08x}"
            )

    def records(count: int, format_text: str, label: str) -> tuple[tuple, ...]:
        nonlocal cursor
        record_size = struct.calcsize(format_text)
        byte_count = count * record_size
        if cursor + byte_count > end:
            raise FldError(f"{context} ends inside {label}")
        values = tuple(
            struct.unpack_from(format_text, data, cursor + index * record_size)
            for index in range(count)
        )
        cursor += byte_count
        return values

    while cursor < end and struct.unpack_from("<I", data, cursor)[0] != 0:
        mesh_index = len(meshes)
        prefix = f"mesh {mesh_index}"
        expect_unpack(0xD, 1, 0, prefix + " header command")
        triangle_count, vertex_count, control_0, control_1 = records(
            1, "<4H", prefix + " header"
        )[0]
        if not triangle_count or not vertex_count:
            raise FldError(f"{context} {prefix} has an empty triangle or vertex array")

        address = 1
        expect_unpack(0xE, triangle_count, address, prefix + " triangle command")
        triangles = records(triangle_count, "<4B", prefix + " triangles")
        for triangle_index, triangle in enumerate(triangles):
            if any(index >= vertex_count for index in triangle[:3]):
                raise FldError(
                    f"{context} {prefix} triangle {triangle_index} indexes "
                    f"outside {vertex_count} vertices"
                )
        address += triangle_count

        expect_unpack(0x8, vertex_count, address, prefix + " position command")
        positions = records(vertex_count, "<3f", prefix + " positions")
        address += vertex_count

        normals = None
        if cursor + 4 <= end and struct.unpack_from("<I", data, cursor)[0] == _vif_unpack_code(
            0x8, vertex_count, address
        ):
            cursor += 4
            normals = records(vertex_count, "<3f", prefix + " normals")
            address += vertex_count

        texcoords = None
        attributes = None
        if cursor + 4 <= end:
            next_word = struct.unpack_from("<I", data, cursor)[0]
            if next_word == _vif_unpack_code(0x4, vertex_count, address):
                cursor += 4
                texcoords = records(vertex_count, "<2f", prefix + " texture coordinates")
                address += vertex_count
            elif next_word == _vif_unpack_code(0xC, vertex_count, address):
                cursor += 4
                attributes = records(vertex_count, "<4f", prefix + " attributes")
                address += vertex_count

        colors = None
        if cursor + 4 <= end and struct.unpack_from("<I", data, cursor)[0] == _vif_unpack_code(
            0xE, vertex_count, address
        ):
            cursor += 4
            colors = records(vertex_count, "<4B", prefix + " colors")

        program_word = word(prefix + " program command")
        if program_word & 0xFFFF0000 != 0x14000000:
            raise FldError(
                f"{context} {prefix} program command is 0x{program_word:08x}"
            )
        meshes.append(
            ModelMesh(
                triangles,
                positions,
                normals,
                texcoords,
                attributes,
                colors,
                (control_0, control_1),
                program_word & 0xFFFF,
            )
        )

    tail_size = end - cursor
    if tail_size % 4 or any(data[cursor:end]):
        raise FldError(f"{context} has nonzero or partial trailing VIF words")
    nop_count = tail_size // 4
    if nop_count > 3:
        raise FldError(f"{context} has {nop_count} trailing NOPs, expected at most 3")
    return tuple(meshes), nop_count


def _read_motion_tracks(
    data: bytes, offset: int, data_end: int, context: str
) -> tuple[MotionTrack, ...]:
    _range(data, offset, 4, context)
    count = _u32(data, offset, context + " track count")
    root_size = (4 + count * 8 + 0xF) & ~0xF
    _range(data, offset, root_size, context)
    if any(data[offset + 4 + count * 8 : offset + root_size]):
        raise FldError(f"{context} has nonzero root padding")

    tracks = []
    for index in range(count):
        kind, curve = struct.unpack_from("<II", data, offset + 4 + index * 8)
        if kind not in MOTION_KINDS:
            raise FldError(f"{context} track {index} has unknown kind {kind}")
        if not curve:
            raise FldError(f"{context} track {index} has a null curve")
        _range(data, curve, MOTION_CURVE_SIZE, f"{context} track {index} curve")
        value_count, values, keys, word_0c = struct.unpack_from("<IIII", data, curve)
        if value_count == 0:
            raise FldError(f"{context} track {index} has no keys")
        width = MOTION_KINDS[kind][1]
        _range(data, values, value_count * width * 4, f"{context} track {index} values")
        _range(data, keys, value_count * 4, f"{context} track {index} keys")
        frames = struct.unpack_from(f"<{value_count}I", data, keys)
        if any(left >= right for left, right in zip(frames, frames[1:])):
            raise FldError(f"{context} track {index} keys are not strictly increasing")
        tracks.append(MotionTrack(kind, curve, value_count, values, keys, word_0c))
    return tuple(tracks)


def _read_special_point(data: bytes, offset: int, context: str) -> tuple[int, int]:
    _range(data, offset, 8, context)
    kind, point_id = struct.unpack_from("<II", data, offset)
    if kind not in SPECIAL_POINT_KINDS:
        raise FldError(f"{context} has unknown kind {kind}")
    return kind, point_id


def validate(data: bytes) -> None:
    """Validate the known FLD object graph and semantic index domains."""

    words, data_end, relocation_tuple = _read_header(data)
    relocations = set(relocation_tuple)
    rows = _read_types(data, words, data_end)
    resources = _read_resources(data, rows)
    event_count = sum(row.count for row in rows if row.type_id == 6)

    for resource in resources:
        if resource.name:
            _fixed_string(data, resource.name, f"resource at 0x{resource.offset:x} name")
        if resource.type_id == 2 and resource.data and data[4:8] == b"FLD1":
            model_resource, items = _read_model_resource(
                data, resource.data, data_end, "field model"
            )
            assets = _read_model_assets(
                data,
                _u32(data, resource.data + 0x14, "field model asset pointer"),
                data_end,
                "field model assets",
            )
            _read_model_motion(
                data,
                model_resource.motion,
                data_end,
                relocations,
                len(items),
                len(assets),
                "field model motion",
            )
            _, _, draws = _read_model_draw_graph(
                data,
                items,
                len(assets),
                data_end,
                relocations,
                "field model",
            )
            packets: dict[int, int] = {}
            for draw in draws.values():
                size = draw.quadwords * 0x10
                old_size = packets.setdefault(draw.packet, size)
                if old_size != size:
                    raise FldError(
                        f"field model packet at 0x{draw.packet:x} has conflicting sizes"
                    )
            for packet, size in packets.items():
                _read_model_mesh_packet(
                    data, packet, size, f"field model packet at 0x{packet:x}"
                )
        elif resource.type_id == 3 and resource.data:
            _range(data, resource.data, COLLISION_SIZE, "collision header")
            values = struct.unpack_from("<12I", data, resource.data)
            if values[2] != resource.data + 0x10:
                raise FldError(f"collision at 0x{resource.data:x} has a detached header")
            vertex_count, face_count = values[4], values[5]
            vertices, faces = values[7], values[8]
            _range(data, vertices, vertex_count * VERTEX_SIZE, "collision vertices")
            _range(data, faces, face_count * FACE_SIZE, "collision faces")
            for face_index in range(face_count):
                face = struct.unpack_from(
                    "<IBBH HBB 4I hhhh", data, faces + face_index * FACE_SIZE
                )
                for vertex in face[7:11]:
                    if vertex != 0xFFFFFFFF and vertex >= vertex_count:
                        raise FldError(
                            f"collision face {face_index} vertex {vertex} exceeds {vertex_count}"
                        )
                _face_encounter_zone(face, face_index)
        elif resource.type_id == 4 and resource.data:
            _range(data, resource.data, 4, "camera resource")
        elif resource.type_id == 6 and resource.data:
            _range(data, resource.data, 0x10, "event resource")
            label = _u32(data, resource.data + 4, "event label")
            if label:
                _cstring(data, label, data_end, "event label")
        elif resource.type_id == 5 and resource.data and data[4:8] == b"FLD1":
            _fixed_string(data, resource.data, "texture-list filename")
        elif resource.type_id == 11 and resource.data and data[4:8] == b"FLD1":
            _range(data, resource.data, EFFECT_SIZE, "effect resource")
        elif resource.type_id == 12 and resource.data and data[4:8] == b"FLD1":
            _range(data, resource.data, LIGHT_SIZE, "light resource")
        elif resource.type_id == 9 and resource.data:
            _read_motion_tracks(data, resource.data, data_end, "motion resource")
        elif resource.type_id == 10 and resource.data:
            _range(data, resource.data, 0x10, "placement resource")
            kind, event_index = struct.unpack_from("<Ii", data, resource.data)
            if kind > 8:
                raise FldError(f"placement at 0x{resource.data:x} has kind {kind}")
            if kind == 1 and event_index >= 0 and event_index >= event_count:
                raise FldError(
                    f"event placement at 0x{resource.data:x} references event {event_index}, "
                    f"but the file has {event_count} event resources"
                )
            if kind == 8:
                payload = _u32(data, resource.data + 0xC, "special point pointer")
                if not payload:
                    raise FldError(f"special placement at 0x{resource.data:x} has no payload")
                _read_special_point(data, payload, "special point")


def validate_links(
    data: bytes,
    script_source: str,
    wap_source: str,
    field_number: int,
    area_number: int,
) -> LinkSummary:
    """Join one FLD2 area to its field script and WAP source identities."""

    words, data_end, _ = _read_header(data)
    resources = _read_resources(data, _read_types(data, words, data_end))
    names_by_type: dict[int, set[str]] = {}
    event_labels: set[str] = set()
    for resource in resources:
        if resource.name:
            name = _fixed_string(data, resource.name, "resource name")
            if name:
                names_by_type.setdefault(resource.type_id, set()).add(name)
        if resource.type_id == 6 and resource.data:
            label_pointer = _u32(data, resource.data + 4, "event label")
            if label_pointer:
                label, _ = _cstring(data, label_pointer, data_end, "event label")
                event_labels.add(label)

    procedures: set[str] = set()
    for line_number, line in enumerate(script_source.splitlines(), 1):
        try:
            tokens = shlex.split(line, comments=True, posix=True)
        except ValueError as exc:
            raise FldError(f"script line {line_number}: {exc}") from exc
        if tokens and tokens[0] == "procedure":
            fields = _fields(tuple(token for token in tokens[2:] if "=" in token))
            if "name" in fields:
                procedures.add(fields["name"])
    missing_events = sorted(event_labels - procedures)
    if missing_events:
        raise FldError(f"FLD2 event labels are absent from the field script: {missing_events}")

    actors = names_by_type.get(10, set())
    cameras = names_by_type.get(4, set())
    actor_links = 0
    destination_links = 0
    current_entry: dict[str, str] | None = None
    target_matches = False

    for line_number, line in enumerate(wap_source.splitlines(), 1):
        try:
            tokens = shlex.split(line, comments=True, posix=True)
        except ValueError as exc:
            raise FldError(f"WAP line {line_number}: {exc}") from exc
        if not tokens:
            continue
        if tokens[0] == "entry":
            current_entry = _fields(tuple(token for token in tokens[2:] if "=" in token))
            target_matches = False
            source_area = _int(current_entry.get("area", "0"))
            actor = current_entry.get("name", "").removeprefix("@")
            if source_area == area_number and actor:
                if actor not in actors:
                    raise FldError(
                        f"WAP actor {actor!r} in area {area_number} has no FLD2 placement"
                    )
                actor_links += 1
        elif tokens[0] == "warp" and current_entry is not None:
            fields = _fields(tuple(token for token in tokens[1:] if "=" in token))
            target_area = _int(fields["area"]) if "area" in fields else None
            target_field = _int(fields["field"]) if "field" in fields else field_number
            target_matches = target_field == field_number and target_area == area_number
            position = fields.get("position", "").removeprefix("@")
            if target_matches and position:
                if position not in actors:
                    raise FldError(
                        f"WAP destination {position!r} has no FLD2 placement in area {area_number}"
                    )
                destination_links += 1
        elif tokens[0] == "camera" and current_entry is not None and target_matches:
            fields = _fields(tuple(token for token in tokens[1:] if "=" in token))
            camera = fields.get("name", "").removeprefix("@")
            if camera:
                if camera not in cameras:
                    raise FldError(
                        f"WAP destination camera {camera!r} is absent from FLD2 area {area_number}"
                    )
                destination_links += 1
        elif tokens[0] == "end":
            current_entry = None
            target_matches = False

    return LinkSummary(len(event_labels), actor_links, destination_links)


def _fixed_string(data: bytes, offset: int, context: str) -> str:
    _range(data, offset, STRING_SIZE, context)
    raw = data[offset : offset + STRING_SIZE]
    value, separator, padding = raw.partition(b"\0")
    if separator and any(padding):
        raise FldError(f"{context} has nonzero string padding")
    try:
        return value.decode("ascii")
    except UnicodeDecodeError as exc:
        raise FldError(f"{context} is not ASCII") from exc


def _cstring(data: bytes, offset: int, limit: int, context: str) -> tuple[str, int]:
    if offset <= 0 or offset >= limit:
        raise FldError(f"{context} lies outside the FLD2 data region")
    try:
        end = data.index(0, offset, limit)
    except ValueError as exc:
        raise FldError(f"{context} has no terminator") from exc
    try:
        return data[offset:end].decode("ascii"), end + 1 - offset
    except UnicodeDecodeError as exc:
        raise FldError(f"{context} is not ASCII") from exc


def _float_text(value: float) -> str:
    bits = struct.unpack("<I", struct.pack("<f", value))[0]
    if math.isfinite(value):
        text = repr(value)
        if struct.pack("<f", float(text)) == struct.pack("<I", bits):
            return text
    return f"bits:0x{bits:08x}"


def _parse_float(text: str) -> float:
    if text.startswith("bits:0x"):
        try:
            bits = int(text[7:], 16)
            return struct.unpack("<f", struct.pack("<I", bits))[0]
        except (ValueError, struct.error) as exc:
            raise FldError(f"invalid float bits {text!r}") from exc
    try:
        return float(text)
    except ValueError as exc:
        raise FldError(f"invalid float {text!r}") from exc


def _symbol(text: str) -> str:
    if not text.startswith("@") or not re.fullmatch(r"@[A-Za-z_][A-Za-z0-9_]*", text):
        raise FldError(f"expected @SYMBOL, got {text!r}")
    return text[1:]


def _int(text: str) -> int:
    try:
        return int(text, 0)
    except ValueError as exc:
        raise FldError(f"invalid integer {text!r}") from exc


def _fields(args: tuple[str, ...]) -> dict[str, str]:
    out: dict[str, str] = {}
    for arg in args:
        if "=" not in arg:
            raise FldError(f"expected name=value, got {arg!r}")
        name, value = arg.split("=", 1)
        if not name or name in out:
            raise FldError(f"duplicate or empty field {name!r}")
        out[name] = value
    return out


def _references(text: str) -> tuple[str, ...]:
    references = tuple(text.split(",")) if text else ()
    for reference in references:
        _symbol(reference)
    return references


def _model_asset_source_fields(operation: Operation) -> dict[str, str]:
    fields = _fields(operation.args)
    if "index" not in fields:
        raise FldError(f"line {operation.line}: model_asset requires index")
    extra = set(fields) - MODEL_ASSET_FIELD_NAMES - {"index"}
    if extra:
        raise FldError(
            f"line {operation.line}: unknown model_asset fields {sorted(extra)}"
        )
    return fields


def _csv(text: str, count: int, parse=_int) -> tuple:
    values = tuple(parse(value) for value in text.split(","))
    if len(values) != count:
        raise FldError(f"expected {count} comma-separated values, got {len(values)}")
    return values


def _label_for(labels: dict[int, str], offset: int) -> str:
    try:
        return labels[offset]
    except KeyError as exc:
        raise FldError(f"no label for pointer target 0x{offset:x}") from exc


def _assign_label(labels: dict[int, str], offset: int, name: str) -> None:
    if offset == 0:
        return
    old = labels.get(offset)
    if old is None or old.startswith("loc_"):
        labels[offset] = name


def _name_part(value: str, fallback: str) -> str:
    value = re.sub(r"[^A-Za-z0-9_]", "_", value)
    value = value.strip("_")
    if not value or value[0].isdigit():
        value = f"r_{value}" if value else fallback
    return value


def _model_mesh_source(meshes: tuple[ModelMesh, ...], nop_count: int) -> list[str]:
    lines: list[str] = []

    def emit_records(name: str, values: tuple[tuple, ...], floats: bool) -> None:
        rendered = [
            ",".join(_float_text(value) for value in record)
            if floats
            else ",".join(str(value) for value in record)
            for record in values
        ]
        for start in range(0, len(rendered), MESH_RECORDS_PER_LINE):
            lines.append(f"{name} " + " ".join(rendered[start : start + MESH_RECORDS_PER_LINE]))

    for mesh_index, mesh in enumerate(meshes):
        if lines:
            lines.append("")
        lines.append(f"# mesh {mesh_index}")
        lines.append(
            "mesh_header "
            f"triangles={len(mesh.triangles)} vertices={len(mesh.positions)} "
            f"controls=0x{mesh.controls[0]:04x},0x{mesh.controls[1]:04x}"
        )
        lines.append("mesh_triangles")
        emit_records("triangle", mesh.triangles, False)
        lines.append("mesh_positions")
        emit_records("position", mesh.positions, True)
        if mesh.normals is not None:
            lines.append("mesh_normals")
            emit_records("normal", mesh.normals, True)
        if mesh.texcoords is not None:
            lines.append("mesh_texcoords")
            emit_records("texcoord", mesh.texcoords, True)
        elif mesh.attributes is not None:
            lines.append("mesh_attributes")
            emit_records("attribute", mesh.attributes, True)
        if mesh.colors is not None:
            lines.append("mesh_colors")
            emit_records("color", mesh.colors, False)
        lines.append(f"mesh_program address={mesh.program}")
    if nop_count:
        lines.append(f"vif_nops count={nop_count}")
    return lines


def render_source(data: bytes) -> str:
    validate(data)
    words, data_end, relocation_tuple = _read_header(data)
    relocations = set(relocation_tuple)
    rows = _read_types(data, words, data_end)
    resources = _read_resources(data, rows)

    pointer_targets = {_u32(data, location, "pointer") for location in relocations}
    labels = {offset: f"loc_{offset:05x}" for offset in pointer_targets if offset}
    labels[data_end] = "data_end"
    _assign_label(labels, words[6], "resource_types")
    for offset, name in ((words[7], "area_data"), (words[9], "constraint_area"), (words[13], "area_table")):
        _assign_label(labels, offset, name)
    for row in rows:
        _assign_label(labels, row.resources, f"type_{row.type_id}_resources")

    resource_names: dict[int, str] = {}
    for resource in resources:
        value = _fixed_string(data, resource.name, f"resource at 0x{resource.offset:x} name") if resource.name else ""
        stem = _name_part(value, f"type_{resource.type_id}_{resource.serial}")
        if stem in resource_names.values():
            stem = f"{stem}_{resource.serial}"
        resource_names[resource.offset] = stem
        _assign_label(labels, resource.name, f"{stem}_name")
        _assign_label(labels, resource.transform, f"{stem}_transform")
        _assign_label(labels, resource.area, f"{stem}_area")
        _assign_label(labels, resource.link, f"{stem}_link")
        _assign_label(labels, resource.sblock, f"{stem}_sblock")
        _assign_label(labels, resource.data, f"{stem}_data")

    spans: dict[int, tuple[int, list[str]]] = {}
    span_starts: list[int] = []
    internal_targets: set[int] = set()

    def add_span(start: int, size: int, lines: list[str], context: str) -> None:
        if size == 0:
            return
        if start <= 0 or start + size > data_end:
            raise FldError(f"{context} lies outside the data region")
        old = spans.get(start)
        if old is not None:
            if start + size == old[0]:
                return
            raise FldError(f"{context} overlaps typed data at 0x{start:x}")
        index = bisect.bisect_left(span_starts, start)
        if index:
            previous = span_starts[index - 1]
            if spans[previous][0] > start:
                raise FldError(
                    f"{context} overlaps typed data at 0x{previous:x}"
                )
        if index < len(span_starts) and span_starts[index] < start + size:
            raise FldError(
                f"{context} overlaps typed data at 0x{span_starts[index]:x}"
            )
        spans[start] = (start + size, lines)
        span_starts.insert(index, start)

    # Type rows and resource heads are the backbone of the object graph.
    type_lines = [
        f"type id={row.type_id} count={row.count} resources=@{_label_for(labels, row.resources)}"
        for row in rows
    ]
    add_span(words[6], len(rows) * TYPE_SIZE, type_lines, "resource type table")

    for row in rows:
        lines = []
        for resource in (item for item in resources if item.offset >= row.resources and item.offset < row.resources + row.count * RESOURCE_SIZE):
            def ref(value: int) -> str:
                return "null" if value == 0 else f"@{_label_for(labels, value)}"
            lines.append(f"# {resource_names[resource.offset]}")
            lines.append(
                "resource "
                f"serial={resource.serial} flags={resource.flags} type={resource.type_id} "
                f"name={ref(resource.name)} reserved={resource.reserved} "
                f"transform={ref(resource.transform)} area={ref(resource.area)} "
                f"link={ref(resource.link)} sblock={ref(resource.sblock)} data={ref(resource.data)}"
            )
        add_span(row.resources, row.count * RESOURCE_SIZE, lines, f"type {row.type_id} resources")

    # Common descriptor children.
    for resource in resources:
        stem = resource_names[resource.offset]
        if resource.name:
            add_span(resource.name, STRING_SIZE, [f"string16 {json.dumps(_fixed_string(data, resource.name, stem + ' name'))}"], stem + " name")
        if resource.transform:
            _range(data, resource.transform, TRANSFORM_SIZE, stem + " transform")
            values = struct.unpack_from("<12f", data, resource.transform)
            add_span(
                resource.transform,
                TRANSFORM_SIZE,
                [
                    "transform "
                    f"position={','.join(_float_text(v) for v in values[0:4])} "
                    f"rotation={','.join(_float_text(v) for v in values[4:8])} "
                    f"scale={','.join(_float_text(v) for v in values[8:12])}"
                ],
                stem + " transform",
            )
        if resource.area:
            _range(data, resource.area, AREA_SIZE, stem + " area")
            number = _u32(data, resource.area, stem + " area number")
            name_pointer = _u32(data, resource.area + 4, stem + " area name")
            if name_pointer != resource.area + 0x10:
                raise FldError(f"{stem} area name is not inline")
            if any(data[resource.area + 8 : resource.area + 0x10]):
                raise FldError(f"{stem} area has nonzero reserved words")
            name = _fixed_string(data, name_pointer, stem + " area name")
            add_span(resource.area, AREA_SIZE, [f"area number={number} name={json.dumps(name)}"], stem + " area")
            internal_targets.add(name_pointer)

    # Type-specific payloads.
    for resource in resources:
        stem = resource_names[resource.offset]
        if resource.type_id == 2 and resource.data and data[4:8] == b"FLD1":
            model_resource, items = _read_model_resource(
                data, resource.data, data_end, stem + " field model"
            )
            assets = _read_model_assets(
                data,
                model_resource.assets,
                data_end,
                stem + " model assets",
            )
            motion = _read_model_motion(
                data,
                model_resource.motion,
                data_end,
                relocations,
                len(items),
                len(assets),
                stem + " model motion",
            )
            draw_roots, draw_lists, draws = _read_model_draw_graph(
                data,
                items,
                len(assets),
                data_end,
                relocations,
                stem + " field model",
            )
            _assign_label(labels, model_resource.items, f"{stem}_items")
            _assign_label(labels, model_resource.assets, f"{stem}_assets")
            _assign_label(labels, model_resource.motion, f"{stem}_motion")
            _assign_label(labels, motion.clip_table, f"{stem}_motion_clips")
            for clip_index, clip in enumerate(motion.clips):
                if clip is not None:
                    _assign_label(
                        labels,
                        clip.offset,
                        f"{stem}_motion_clip_{clip_index}",
                    )

            for index, item in enumerate(items):
                if not item.commands:
                    continue
                root_name = f"{stem}_node_{index}_draws"
                _assign_label(labels, item.commands, root_name)
                for list_index, list_offset in enumerate(draw_roots[item.commands]):
                    list_name = f"{root_name}_list_{list_index}"
                    _assign_label(labels, list_offset, list_name)
                    for draw_index, draw_offset in enumerate(
                        draw_lists[list_offset].draws
                    ):
                        draw_name = f"{list_name}_draw_{draw_index}"
                        _assign_label(labels, draw_offset, draw_name)
                        _assign_label(
                            labels,
                            draws[draw_offset].packet,
                            f"{draw_name}_packet",
                        )

            def model_ref(value: int) -> str:
                return "null" if value == 0 else f"@{_label_for(labels, value)}"

            add_span(
                resource.data,
                MODEL_RESOURCE_SIZE,
                [
                    "model_resource "
                    f"items={model_ref(model_resource.items)} "
                    f"assets={model_ref(model_resource.assets)} "
                    f"motion={model_ref(model_resource.motion)}"
                ],
                stem + " field model",
            )

            count, _, _, _ = struct.unpack_from(
                "<4I", data, model_resource.items
            )
            node_lines = [
                f"model_items count={count}"
            ]
            for index, node in enumerate(items):
                _assign_label(labels, node.bounds, f"{stem}_node_{index}_bounds")
                node_lines.append(
                    "model_item "
                    f"node_id={node.node_id} parent={node.parent} "
                    f"rotation={','.join(_float_text(value) for value in node.rotation)} "
                    f"position={','.join(_float_text(value) for value in node.position)} "
                    f"scale={','.join(_float_text(value) for value in node.scale)} "
                    f"bounds={model_ref(node.bounds)} "
                    f"commands={model_ref(node.commands)}"
                )
            add_span(
                model_resource.items,
                MODEL_ITEM_LIST_SIZE + len(items) * MODEL_ITEM_SIZE,
                node_lines,
                stem + " model items",
            )
            for index, node in enumerate(items):
                if not node.bounds:
                    continue
                bounds = struct.unpack_from("<6f", data, node.bounds)
                add_span(
                    node.bounds,
                    MODEL_BOUNDS_SIZE,
                    [
                        "model_bounds "
                        f"minimum={','.join(_float_text(value) for value in bounds[:3])} "
                        f"maximum={','.join(_float_text(value) for value in bounds[3:])}"
                    ],
                    f"{stem} model item {index} bounds",
                )

            asset_lines = [f"model_assets count={len(assets)}"]
            for asset in assets:
                fields = [f"index={asset.index}"]
                for field_name, values in asset.fields:
                    if field_name in {"values_08", "values_40", "scalar_100", "scalar_200", "pair_400"}:
                        text = ",".join(_float_text(value) for value in values)
                    elif field_name in {"word_01", "word_02", "word_10", "word_80"}:
                        text = ",".join(f"0x{value:08x}" for value in values)
                    else:
                        text = ",".join(str(value) for value in values)
                    fields.append(f"{field_name}={text}")
                asset_lines.append("model_asset " + " ".join(fields))
            add_span(
                model_resource.assets,
                4 + sum(asset.size for asset in assets),
                asset_lines,
                stem + " model assets",
            )

            motion_lines = [
                "model_motion_playbook "
                f"clip_count={len(motion.clips)} bindings={len(motion.bindings)} "
                f"clips=@{_label_for(labels, motion.clip_table)}"
            ]
            for binding in motion.bindings:
                family = MODEL_MOTION_FAMILIES[binding.family]
                if binding.family == 0:
                    selector = MODEL_MOTION_NODE_SELECTORS[binding.selector]
                else:
                    selector = str(binding.selector)
                motion_lines.append(
                    "model_motion_binding "
                    f"family={family} selector={selector} target={binding.target}"
                )
            add_span(
                motion.offset,
                8 + len(motion.bindings) * 8,
                motion_lines,
                stem + " model motion playbook",
            )

            clip_refs = ",".join(
                "null" if clip is None else f"@{_label_for(labels, clip.offset)}"
                for clip in motion.clips
            )
            add_span(
                motion.clip_table,
                len(motion.clips) * 4,
                [f"model_motion_clip_table clips={clip_refs}"],
                stem + " model motion clip table",
            )

            rendered_clips: set[int] = set()
            for clip_index, clip in enumerate(motion.clips):
                if clip is None or clip.offset in rendered_clips:
                    continue
                rendered_clips.add(clip.offset)
                clip_lines = [
                    "model_motion_clip "
                    f"duration={clip.duration} reserved={clip.reserved}"
                ]
                for track in clip.tracks:
                    clip_lines.append(
                        "model_motion_track "
                        f"format={track.format_name} "
                        f"frames={','.join(str(frame) for frame in track.frames)}"
                    )
                    directive, _, width, value_kind = MODEL_MOTION_VALUE_DIRECTIVES[
                        track.format_name
                    ]
                    records = []
                    for value_index in range(track.count):
                        value_offset = track.values + value_index * track.stride
                        if value_kind == "float":
                            values = struct.unpack_from(
                                "<" + "f" * width, data, value_offset
                            )
                            records.append(
                                ",".join(_float_text(value) for value in values)
                            )
                        elif value_kind == "s16":
                            values = struct.unpack_from(
                                "<" + "h" * width, data, value_offset
                            )
                            records.append(",".join(str(value) for value in values))
                        elif value_kind == "u8":
                            values = struct.unpack_from(
                                "<" + "B" * width, data, value_offset
                            )
                            records.append(",".join(str(value) for value in values))
                        else:
                            raise AssertionError(value_kind)
                    for start in range(0, len(records), MESH_RECORDS_PER_LINE):
                        clip_lines.append(
                            directive
                            + " "
                            + " ".join(records[start : start + MESH_RECORDS_PER_LINE])
                        )
                add_span(
                    clip.offset,
                    4 + sum(track.size for track in clip.tracks),
                    clip_lines,
                    f"{stem} model motion clip {clip_index}",
                )

            for root_offset, list_offsets in draw_roots.items():
                list_refs = ",".join(
                    f"@{_label_for(labels, value)}" for value in list_offsets
                )
                add_span(
                    root_offset,
                    (len(list_offsets) + 1) * 4,
                    [f"model_draw_set lists={list_refs}"],
                    stem + " model draw-list set",
                )
            for list_offset, draw_list in draw_lists.items():
                draw_refs = ",".join(
                    f"@{_label_for(labels, value)}" for value in draw_list.draws
                )
                add_span(
                    list_offset,
                    4 + len(draw_list.draws) * 4,
                    [
                        "model_draw_list "
                        f"selector={draw_list.selector} draws={draw_refs}"
                    ],
                    stem + " model draw list",
                )
            packet_sizes: dict[int, int] = {}
            for draw_offset, draw in draws.items():
                add_span(
                    draw_offset,
                    MODEL_DRAW_SIZE,
                    [
                        "model_draw "
                        f"asset={draw.asset} qwords={draw.quadwords} "
                        f"packet=@{_label_for(labels, draw.packet)}"
                    ],
                    stem + " model draw",
                )
                packet_size = draw.quadwords * 0x10
                old_size = packet_sizes.setdefault(draw.packet, packet_size)
                if old_size != packet_size:
                    raise FldError(
                        f"{stem} packet at 0x{draw.packet:x} has conflicting sizes"
                    )
            for packet, packet_size in packet_sizes.items():
                meshes, nop_count = _read_model_mesh_packet(
                    data,
                    packet,
                    packet_size,
                    stem + " model draw packet",
                )
                packet_lines = _model_mesh_source(meshes, nop_count)
                add_span(
                    packet,
                    packet_size,
                    packet_lines,
                    stem + " model draw packet",
                )
        elif resource.type_id == 3 and resource.data:
            offset = resource.data
            _range(data, offset, COLLISION_SIZE, stem + " collision")
            values = struct.unpack_from("<12I", data, offset)
            header_pointer = values[2]
            if header_pointer != offset + 0x10:
                raise FldError(f"{stem} collision header is not inline")
            vertex_count, face_count, extra_count = values[4:7]
            vertices, faces, stop = values[7:10]
            _assign_label(labels, vertices, f"{stem}_vertices")
            _assign_label(labels, faces, f"{stem}_faces")
            _assign_label(labels, stop, f"{stem}_stop")
            stop_ref = "null" if stop == 0 else f"@{_label_for(labels, stop)}"
            add_span(
                offset,
                COLLISION_SIZE,
                [
                    "collision "
                    f"vertex_count={vertex_count} face_count={face_count} extra_count={extra_count} "
                    f"vertices=@{_label_for(labels, vertices)} faces=@{_label_for(labels, faces)} "
                    f"stop={stop_ref} reserved={values[10]},{values[11]}"
                ],
                stem + " collision",
            )
            internal_targets.add(header_pointer)
            vertex_lines = []
            _range(data, vertices, vertex_count * VERTEX_SIZE, stem + " vertices")
            for index in range(vertex_count):
                vals = struct.unpack_from("<4f", data, vertices + index * VERTEX_SIZE)
                vertex_lines.append("vertex " + " ".join(_float_text(v) for v in vals))
            add_span(vertices, vertex_count * VERTEX_SIZE, vertex_lines, stem + " vertices")
            face_lines = []
            _range(data, faces, face_count * FACE_SIZE, stem + " faces")
            for index in range(face_count):
                values = struct.unpack_from("<IBBH HBB 4I hhhh", data, faces + index * FACE_SIZE)
                if values[0] & ENCOUNTER_ZONE_ATTRIBUTE:
                    encounter = f"encounter_zone={values[12]}"
                else:
                    encounter = f"encounter_type={values[11]} encounter={values[12]}"
                face_lines.append(
                    "face "
                    f"attributes=0x{values[0]:08x} move_floor={values[1]} sound={values[2]} "
                    f"stop={values[3]} place={values[4]} automap={values[5]},{values[6]} "
                    f"vertices={values[7]},{values[8]},{values[9]},{values[10]} "
                    f"{encounter} "
                    f"special={values[13]},{values[14]}"
                )
            add_span(faces, face_count * FACE_SIZE, face_lines, stem + " faces")
        elif resource.type_id == 4 and resource.data:
            _range(data, resource.data, 4, stem + " camera")
            fovy = struct.unpack_from("<f", data, resource.data)[0]
            add_span(resource.data, 4, [f"camera fovy={_float_text(fovy)}"], stem + " camera")
        elif resource.type_id == 6 and resource.data:
            _range(data, resource.data, 0x10, stem + " event")
            flag, label, reserved0, reserved1 = struct.unpack_from("<IIII", data, resource.data)
            _assign_label(labels, label, f"{stem}_event_label")
            add_span(
                resource.data,
                0x10,
                [
                    f"event flags={flag} label=@{_label_for(labels, label)} "
                    f"reserved={reserved0},{reserved1}"
                ],
                stem + " event",
            )
            if label:
                label_text, label_size = _cstring(data, label, data_end, stem + " event label")
                add_span(label, label_size, [f"cstring {json.dumps(label_text)}"], stem + " event label")
        elif resource.type_id == 5 and resource.data and data[4:8] == b"FLD1":
            add_span(
                resource.data,
                STRING_SIZE,
                [f"texture_list {json.dumps(_fixed_string(data, resource.data, stem + ' texture list'))}"],
                stem + " texture list",
            )
        elif resource.type_id == 11 and resource.data and data[4:8] == b"FLD1":
            values = struct.unpack_from("<III3f6I", data, resource.data)
            add_span(
                resource.data,
                EFFECT_SIZE,
                [
                    "effect "
                    f"flags={values[0]} type={values[1]} selector={values[2]} "
                    f"size={','.join(_float_text(value) for value in values[3:6])} "
                    f"parameters={','.join(str(value) for value in values[6:12])}"
                ],
                stem + " effect",
            )
        elif resource.type_id == 12 and resource.data and data[4:8] == b"FLD1":
            values = struct.unpack_from("<III10f", data, resource.data)
            add_span(
                resource.data,
                LIGHT_SIZE,
                [
                    "light "
                    f"reserved={values[0]} flags={values[1]} animation={values[2]} "
                    f"inner_radius={_float_text(values[3])} outer_radius={_float_text(values[4])} "
                    f"softness={_float_text(values[5])} bias={_float_text(values[6])} "
                    f"diffuse={','.join(_float_text(value) for value in values[7:10])} "
                    f"ambient={','.join(_float_text(value) for value in values[10:13])}"
                ],
                stem + " light",
            )
        elif resource.type_id == 9 and resource.data:
            tracks = _read_motion_tracks(data, resource.data, data_end, stem + " motion")
            root_size = (4 + len(tracks) * 8 + 0xF) & ~0xF
            track_refs = []
            for index, track in enumerate(tracks):
                kind_name, width = MOTION_KINDS[track.kind]
                suffix = kind_name
                if sum(other.kind == track.kind for other in tracks) > 1:
                    suffix += f"_{index}"
                _assign_label(labels, track.curve, f"{stem}_{suffix}_curve")
                _assign_label(labels, track.values, f"{stem}_{suffix}_values")
                _assign_label(labels, track.keys, f"{stem}_{suffix}_keys")
                track_refs.append(
                    f"{kind_name}:@{_label_for(labels, track.curve)}"
                )

                add_span(
                    track.curve,
                    MOTION_CURVE_SIZE,
                    [
                        "motion_curve "
                        f"count={track.count} values=@{_label_for(labels, track.values)} "
                        f"keys=@{_label_for(labels, track.keys)} word_0c={track.word_0c}"
                    ],
                    f"{stem} {kind_name} curve",
                )
                value_lines = []
                for value_index in range(track.count):
                    values = struct.unpack_from(
                        "<" + "f" * width,
                        data,
                        track.values + value_index * width * 4,
                    )
                    value_lines.append(
                        f"{kind_name} " + " ".join(_float_text(value) for value in values)
                    )
                add_span(
                    track.values,
                    track.count * width * 4,
                    value_lines,
                    f"{stem} {kind_name} values",
                )
                frames = struct.unpack_from(f"<{track.count}I", data, track.keys)
                key_lines = [
                    "keys " + " ".join(str(frame) for frame in frames[start : start + 16])
                    for start in range(0, len(frames), 16)
                ]
                add_span(
                    track.keys,
                    track.count * 4,
                    key_lines,
                    f"{stem} {kind_name} keys",
                )
            add_span(
                resource.data,
                root_size,
                [f"motion tracks={','.join(track_refs)}"],
                stem + " motion",
            )
        elif resource.type_id == 10 and resource.data:
            _range(data, resource.data, 0x10, stem + " placement")
            kind, event_index, visible, payload = struct.unpack_from("<IiII", data, resource.data)
            _assign_label(labels, payload, f"{stem}_payload")
            payload_ref = "null" if payload == 0 else f"@{_label_for(labels, payload)}"
            add_span(
                resource.data,
                0x10,
                [f"placement kind={kind} event={event_index} visible={visible} payload={payload_ref}"],
                stem + " placement",
            )
            if kind == 8:
                special_kind, point_id = _read_special_point(data, payload, stem + " special point")
                add_span(
                    payload,
                    8,
                    [f"special_point kind={SPECIAL_POINT_KINDS[special_kind]} id={point_id}"],
                    stem + " special point",
                )

    # Internal pointer targets are emitted by their owning directive.
    for target in internal_targets:
        labels.pop(target, None)

    # The header is rendered separately because its size/fixup fields are computed.
    def header_value(index: int) -> str:
        offset = index * 4
        value = words[index]
        if offset in relocations:
            if value < HEADER_SIZE:
                return f"offset:0x{value:x}"
            return f"@{_label_for(labels, value)}"
        return str(value)

    magic = data[4:8].decode("ascii")
    header = (
        "header "
        f"version={words[0]} magic={magic} type_count={words[5]} type_table={header_value(6)} "
        f"word_1c={header_value(7)} word_20={header_value(8)} word_24={header_value(9)} "
        f"word_28={header_value(10)} word_2c={header_value(11)} word_30={header_value(12)} "
        f"word_34={header_value(13)} word_38={header_value(14)} word_3c={header_value(15)}"
    )

    lines = [f"{magic.lower()} 1", header, ""]
    position = HEADER_SIZE
    relocation_set = set(relocations)

    while position < data_end:
        if position in labels:
            lines.append(f"label {labels[position]}")
        if position in spans:
            end, body = spans[position]
            lines.extend(body)
            lines.append("")
            position = end
            continue
        if position in relocation_set:
            target = _u32(data, position, "pointer")
            lines.append(f"pointer @{_label_for(labels, target)}")
            position += 4
            continue

        boundaries = [data_end]
        boundaries.extend(start for start in spans if start > position)
        boundaries.extend(start for start in labels if start > position)
        boundaries.extend(location for location in relocation_set if location > position)
        end = min(boundaries)
        chunk = data[position:end]
        if not chunk:
            raise FldError(f"could not render byte at 0x{position:x}")
        while chunk:
            part, chunk = chunk[:32], chunk[32:]
            if not any(part):
                lines.append(f"zeros {len(part)}")
            else:
                lines.append(f"bytes {part.hex()}")
            position += len(part)

    if position != data_end:
        raise FldError("renderer crossed the data-region boundary")
    lines.append("label data_end")
    lines.append("end_data")
    lines.append("")
    source = "\n".join(lines)
    model = parse_source(source)
    rebuilt = encode(model)
    if rebuilt != data:
        for index, (actual, expected) in enumerate(zip(rebuilt, data)):
            if actual != expected:
                raise FldError(
                    f"rendered source differs at 0x{index:x}: {actual:02x} != {expected:02x}"
                )
        raise FldError("rendered source has a different length")
    return source


def parse_source(source: str) -> tuple[Operation, ...]:
    operations: list[Operation] = []
    saw_preamble = False
    source_magic = ""
    for line_number, raw_line in enumerate(source.splitlines(), 1):
        raw_bytes = re.fullmatch(
            r"\s*(bytes|packet_data)\s+([0-9A-Fa-f]+)\s*", raw_line
        )
        if raw_bytes:
            tokens = [raw_bytes.group(1), raw_bytes.group(2)]
        else:
            try:
                tokens = shlex.split(raw_line, comments=True, posix=True)
            except ValueError as exc:
                raise FldError(f"line {line_number}: {exc}") from exc
        if not tokens:
            continue
        if not saw_preamble:
            if tokens not in (["fld1", "1"], ["fld2", "1"]):
                raise FldError(f"line {line_number}: expected 'fld1 1' or 'fld2 1'")
            saw_preamble = True
            source_magic = tokens[0].upper()
            continue
        operations.append(Operation(line_number, tokens[0], tuple(tokens[1:])))
    if not saw_preamble:
        raise FldError("missing 'fld1 1' or 'fld2 1' preamble")
    headers = [operation for operation in operations if operation.name == "header"]
    if headers:
        magic = _fields(headers[0].args).get("magic")
        if magic is not None and magic != source_magic:
            raise FldError(
                f"line {headers[0].line}: {magic} header does not match "
                f"{source_magic.lower()} preamble"
            )
    return tuple(operations)


def _model_motion_binding_source(
    operation: Operation,
) -> tuple[int, int, int, str]:
    fields = _fields(operation.args)
    if set(fields) != {"family", "selector", "target"}:
        raise FldError(
            f"line {operation.line}: model_motion_binding expects "
            "family=... selector=... target=..."
        )
    family = MODEL_MOTION_FAMILY_IDS.get(fields["family"])
    if family is None:
        raise FldError(
            f"line {operation.line}: unknown motion family {fields['family']!r}"
        )
    if family == 0:
        selector = MODEL_MOTION_NODE_SELECTOR_IDS.get(fields["selector"])
        if selector is None:
            raise FldError(
                f"line {operation.line}: unknown node-motion selector "
                f"{fields['selector']!r}"
            )
    else:
        selector = _int(fields["selector"])
    format_name, _ = _model_motion_format(
        family, selector, f"line {operation.line}"
    )
    target = _int(fields["target"])
    if target < 0:
        raise FldError(f"line {operation.line}: motion target must be nonnegative")
    return family, selector, target, format_name


def _validate_model_motion_source(operations: tuple[Operation, ...]) -> None:
    """Validate linked playbook, clip-table, and typed track source blocks."""

    label_operations: dict[str, int] = {}
    for index, operation in enumerate(operations):
        if operation.name == "label":
            label_operations[operation.args[0]] = index

    def operation_after_label(reference: str, expected: str) -> tuple[int, Operation]:
        name = _symbol(reference)
        if name not in label_operations:
            raise FldError(f"unknown label @{name}")
        index = label_operations[name] + 1
        while index < len(operations) and operations[index].name == "label":
            index += 1
        if index >= len(operations) or operations[index].name != expected:
            line = operations[label_operations[name]].line
            raise FldError(f"line {line}: @{name} must contain {expected}")
        return index, operations[index]

    checked_clips: dict[str, tuple[tuple[int, int, int, str], ...]] = {}
    for index, operation in enumerate(operations):
        if operation.name != "model_motion_playbook":
            continue
        fields = _fields(operation.args)
        if set(fields) != {"clip_count", "bindings", "clips"}:
            raise FldError(
                f"line {operation.line}: model_motion_playbook expects "
                "clip_count=... bindings=... clips=..."
            )
        clip_count = _int(fields["clip_count"])
        binding_count = _int(fields["bindings"])
        if not 1 <= clip_count <= 0xFFFF or not 0 <= binding_count <= 0xFFFF:
            raise FldError(
                f"line {operation.line}: motion clip and binding counts must fit u16"
            )
        bindings = []
        for binding_index in range(binding_count):
            source_index = index + 1 + binding_index
            if (
                source_index >= len(operations)
                or operations[source_index].name != "model_motion_binding"
            ):
                raise FldError(
                    f"line {operation.line}: expected {binding_count} "
                    "model_motion_binding directives"
                )
            bindings.append(_model_motion_binding_source(operations[source_index]))

        _, table = operation_after_label(fields["clips"], "model_motion_clip_table")
        table_fields = _fields(table.args)
        if set(table_fields) != {"clips"}:
            raise FldError(
                f"line {table.line}: model_motion_clip_table expects clips=..."
            )
        clip_refs = tuple(table_fields["clips"].split(",")) if table_fields["clips"] else ()
        if len(clip_refs) != clip_count:
            raise FldError(
                f"line {table.line}: clip table contains {len(clip_refs)} entries, "
                f"expected {clip_count}"
            )
        binding_tuple = tuple(bindings)
        for reference in clip_refs:
            if reference == "null":
                continue
            clip_name = _symbol(reference)
            old_bindings = checked_clips.get(clip_name)
            if old_bindings is not None:
                if old_bindings != binding_tuple:
                    raise FldError(
                        f"line {table.line}: shared clip @{clip_name} has incompatible bindings"
                    )
                continue
            clip_index, clip = operation_after_label(reference, "model_motion_clip")
            clip_fields = _fields(clip.args)
            if set(clip_fields) != {"duration", "reserved"}:
                raise FldError(
                    f"line {clip.line}: model_motion_clip expects duration=... reserved=..."
                )
            if any(
                not 0 <= _int(clip_fields[name]) <= 0xFFFF
                for name in ("duration", "reserved")
            ):
                raise FldError(f"line {clip.line}: clip fields must fit u16")
            cursor = clip_index + 1
            for _, _, _, expected_format in bindings:
                if (
                    cursor >= len(operations)
                    or operations[cursor].name != "model_motion_track"
                ):
                    raise FldError(
                        f"line {clip.line}: expected {binding_count} model_motion_track directives"
                    )
                track = operations[cursor]
                track_fields = _fields(track.args)
                _operation_size(track, 0)
                format_name = track_fields["format"]
                if format_name != expected_format:
                    raise FldError(
                        f"line {track.line}: motion binding requires {expected_format}, "
                        f"got {format_name}"
                    )
                key_count = len(track_fields["frames"].split(","))
                directive = MODEL_MOTION_VALUE_DIRECTIVES[format_name][0]
                cursor += 1
                actual = 0
                while cursor < len(operations) and operations[cursor].name == directive:
                    _operation_size(operations[cursor], 0)
                    actual += len(operations[cursor].args)
                    cursor += 1
                if actual != key_count:
                    raise FldError(
                        f"line {track.line}: track has {key_count} frames followed by "
                        f"{actual} {directive} records"
                    )
            checked_clips[clip_name] = binding_tuple


def _model_mesh_source_codes(operations: tuple[Operation, ...]) -> dict[int, int]:
    """Validate semantic mesh blocks and return their generated VIF words."""

    codes: dict[int, int] = {}

    def fields(operation: Operation, required: set[str]) -> dict[str, str]:
        values = _fields(operation.args)
        if set(values) != required:
            missing = required - set(values)
            extra = set(values) - required
            raise FldError(
                f"line {operation.line}: fields differ; "
                f"missing={sorted(missing)} extra={sorted(extra)}"
            )
        return values

    def section(
        index: int,
        marker: str,
        record: str,
        count: int,
        format_id: int,
        address: int,
    ) -> tuple[int, int]:
        if index >= len(operations) or operations[index].name != marker:
            line = operations[index - 1].line if index else 1
            raise FldError(f"line {line}: expected {marker}")
        if operations[index].args:
            raise FldError(f"line {operations[index].line}: {marker} takes no arguments")
        codes[index] = _vif_unpack_code(format_id, count, address)
        index += 1
        actual = 0
        while index < len(operations) and operations[index].name == record:
            actual += len(operations[index].args)
            index += 1
        if actual != count:
            raise FldError(
                f"line {operations[index - 1].line}: {marker} declares {count} "
                f"records, followed by {actual} {record} records"
            )
        return index, address + count

    index = 0
    while index < len(operations):
        operation = operations[index]
        if operation.name == "vif_nops":
            values = fields(operation, {"count"})
            count = _int(values["count"])
            if not 1 <= count <= 3:
                raise FldError(
                    f"line {operation.line}: vif_nops count must be in 1..3"
                )
            index += 1
            continue
        if operation.name != "mesh_header":
            if operation.name in MESH_PACKET_DIRECTIVES:
                raise FldError(
                    f"line {operation.line}: {operation.name} appears outside a mesh block"
                )
            index += 1
            continue

        values = fields(operation, {"triangles", "vertices", "controls"})
        triangle_count = _int(values["triangles"])
        vertex_count = _int(values["vertices"])
        controls = _csv(values["controls"], 2)
        if not all(0 <= value <= 0xFFFF for value in controls):
            raise FldError(f"line {operation.line}: mesh controls exceed u16")
        codes[index] = _vif_unpack_code(0xD, 1, 0)
        index += 1
        address = 1
        index, address = section(
            index,
            "mesh_triangles",
            "triangle",
            triangle_count,
            0xE,
            address,
        )
        index, address = section(
            index,
            "mesh_positions",
            "position",
            vertex_count,
            0x8,
            address,
        )
        if index < len(operations) and operations[index].name == "mesh_normals":
            index, address = section(
                index,
                "mesh_normals",
                "normal",
                vertex_count,
                0x8,
                address,
            )
        if index < len(operations) and operations[index].name == "mesh_texcoords":
            index, address = section(
                index,
                "mesh_texcoords",
                "texcoord",
                vertex_count,
                0x4,
                address,
            )
        elif index < len(operations) and operations[index].name == "mesh_attributes":
            index, address = section(
                index,
                "mesh_attributes",
                "attribute",
                vertex_count,
                0xC,
                address,
            )
        if index < len(operations) and operations[index].name == "mesh_colors":
            index, address = section(
                index,
                "mesh_colors",
                "color",
                vertex_count,
                0xE,
                address,
            )
        if index >= len(operations) or operations[index].name != "mesh_program":
            raise FldError(f"line {operation.line}: mesh block has no mesh_program")
        program_values = fields(operations[index], {"address"})
        program = _int(program_values["address"])
        if not 0 <= program <= 0xFFFF:
            raise FldError(
                f"line {operations[index].line}: mesh program address exceeds u16"
            )
        codes[index] = 0x14000000 | program
        index += 1
    return codes


def _operation_size(operation: Operation, offset: int) -> int:
    name, args = operation.name, operation.args
    fixed = {
        "header": HEADER_SIZE,
        "type": TYPE_SIZE,
        "resource": RESOURCE_SIZE,
        "transform": TRANSFORM_SIZE,
        "area": AREA_SIZE,
        "collision": COLLISION_SIZE,
        "vertex": VERTEX_SIZE,
        "face": FACE_SIZE,
        "camera": 4,
        "event": 0x10,
        "placement": 0x10,
        "special_point": 8,
        "motion_curve": MOTION_CURVE_SIZE,
        "string16": STRING_SIZE,
        "texture_list": STRING_SIZE,
        "effect": EFFECT_SIZE,
        "light": LIGHT_SIZE,
        "model_resource": MODEL_RESOURCE_SIZE,
        "model_items": MODEL_ITEM_LIST_SIZE,
        "model_item": MODEL_ITEM_SIZE,
        "model_bounds": MODEL_BOUNDS_SIZE,
        "model_assets": 4,
        "model_draw": MODEL_DRAW_SIZE,
        "model_motion_playbook": 8,
        "model_motion_binding": 8,
        "model_motion_clip": 4,
        "mesh_header": 12,
        "mesh_triangles": 4,
        "mesh_positions": 4,
        "mesh_normals": 4,
        "mesh_texcoords": 4,
        "mesh_attributes": 4,
        "mesh_colors": 4,
        "mesh_program": 4,
        "pointer": 4,
        "u32": 4 * len(args),
        "s32": 4 * len(args),
        "f32": 4 * len(args),
        "label": 0,
        "end_data": 0,
    }
    if name in fixed:
        return fixed[name]
    if name in MESH_DATA_DIRECTIVES:
        record_size, width = MESH_DATA_DIRECTIVES[name]
        if not args:
            raise FldError(f"line {operation.line}: {name} expects records")
        integer_record = name in {"triangle", "color"}
        for arg in args:
            values = _csv(arg, width, _int if integer_record else _parse_float)
            if integer_record and any(not 0 <= value <= 0xFF for value in values):
                raise FldError(f"line {operation.line}: {name} component exceeds u8")
        return record_size * len(args)
    if name == "vif_nops":
        fields = _fields(args)
        if set(fields) != {"count"}:
            raise FldError(f"line {operation.line}: vif_nops expects count=...")
        count = _int(fields["count"])
        if not 1 <= count <= 3:
            raise FldError(f"line {operation.line}: vif_nops count must be in 1..3")
        return count * 4
    if name == "model_asset":
        fields = _model_asset_source_fields(operation)
        size = 8
        for _, field_name, kind, width in MODEL_ASSET_FIELDS:
            if field_name in fields:
                size += 4 if kind in {"resource", "resource_pair"} else width * 4
        return size
    if name == "model_draw_set":
        fields = _fields(args)
        if set(fields) != {"lists"}:
            raise FldError(f"line {operation.line}: model_draw_set expects lists=...")
        return (len(_references(fields["lists"])) + 1) * 4
    if name == "model_draw_list":
        fields = _fields(args)
        if set(fields) != {"selector", "draws"}:
            raise FldError(
                f"line {operation.line}: model_draw_list expects selector=... draws=..."
            )
        return 4 + len(_references(fields["draws"])) * 4
    if name == "model_motion_clip_table":
        fields = _fields(args)
        if set(fields) != {"clips"}:
            raise FldError(
                f"line {operation.line}: model_motion_clip_table expects clips=..."
            )
        clips = fields["clips"].split(",") if fields["clips"] else []
        if not clips or any(value != "null" and not value.startswith("@") for value in clips):
            raise FldError(
                f"line {operation.line}: motion clips must be labels or null"
            )
        return len(clips) * 4
    if name == "model_motion_track":
        fields = _fields(args)
        if set(fields) != {"format", "frames"}:
            raise FldError(
                f"line {operation.line}: model_motion_track expects format=... frames=..."
            )
        if fields["format"] not in MODEL_MOTION_VALUE_DIRECTIVES:
            raise FldError(
                f"line {operation.line}: unknown motion format {fields['format']!r}"
            )
        frames = tuple(_int(value) for value in fields["frames"].split(","))
        if not frames or any(not 0 <= frame <= 0xFFFF for frame in frames):
            raise FldError(f"line {operation.line}: motion frames must be u16 values")
        return 8 + ((len(frames) * 2 + 3) & ~3)
    if name in MODEL_MOTION_DIRECTIVE_FORMATS:
        format_name = MODEL_MOTION_DIRECTIVE_FORMATS[name]
        _, record_size, width, value_kind = MODEL_MOTION_VALUE_DIRECTIVES[format_name]
        if not args:
            raise FldError(f"line {operation.line}: {name} expects key records")
        for arg in args:
            if value_kind == "float":
                _csv(arg, width, _parse_float)
            else:
                values = _csv(arg, width)
                low, high = (-0x8000, 0x7FFF) if value_kind == "s16" else (0, 0xFF)
                if any(not low <= value <= high for value in values):
                    raise FldError(
                        f"line {operation.line}: {name} component is outside {value_kind}"
                    )
        return record_size * len(args)
    if name == "packet_data":
        if len(args) != 1:
            raise FldError(f"line {operation.line}: packet_data expects one hex string")
        try:
            size = len(bytes.fromhex(args[0]))
        except ValueError as exc:
            raise FldError(f"line {operation.line}: invalid packet byte string") from exc
        if size == 0 or size % 0x10:
            raise FldError(
                f"line {operation.line}: packet_data size must be a positive multiple of 16"
            )
        return size
    if name == "motion":
        fields = _fields(args)
        if set(fields) != {"tracks"}:
            raise FldError(f"line {operation.line}: motion expects tracks=...")
        tracks = fields["tracks"].split(",") if fields["tracks"] else []
        if not tracks:
            raise FldError(f"line {operation.line}: motion requires at least one track")
        return (4 + len(tracks) * 8 + 0xF) & ~0xF
    if name in MOTION_KIND_IDS:
        width = MOTION_KINDS[MOTION_KIND_IDS[name]][1]
        if len(args) != width:
            raise FldError(f"line {operation.line}: {name} expects {width} floats")
        return width * 4
    if name == "keys":
        if not args:
            raise FldError(f"line {operation.line}: keys expects at least one frame")
        return len(args) * 4
    if name == "cstring":
        if len(args) != 1:
            raise FldError(f"line {operation.line}: cstring expects one string")
        try:
            return len(args[0].encode("ascii")) + 1
        except UnicodeEncodeError as exc:
            raise FldError(f"line {operation.line}: cstring is not ASCII") from exc
    if name == "bytes":
        if len(args) != 1:
            raise FldError(f"line {operation.line}: bytes expects one hex string")
        try:
            return len(bytes.fromhex(args[0]))
        except ValueError as exc:
            raise FldError(f"line {operation.line}: invalid byte string") from exc
    if name == "zeros":
        if len(args) != 1 or _int(args[0]) < 0:
            raise FldError(f"line {operation.line}: zeros expects a nonnegative size")
        return _int(args[0])
    if name == "align":
        if len(args) != 1:
            raise FldError(f"line {operation.line}: align expects one value")
        alignment = _int(args[0])
        if alignment <= 0 or alignment & (alignment - 1):
            raise FldError(f"line {operation.line}: alignment must be a power of two")
        return (-offset) & (alignment - 1)
    raise FldError(f"line {operation.line}: unknown directive {name!r}")


def _layout(operations: tuple[Operation, ...]) -> tuple[dict[str, int], int]:
    _model_mesh_source_codes(operations)
    _validate_model_motion_source(operations)
    labels: dict[str, int] = {}
    offset = 0
    end_data: int | None = None
    for operation in operations:
        if operation.name == "label":
            if len(operation.args) != 1 or not re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", operation.args[0]):
                raise FldError(f"line {operation.line}: invalid label")
            if operation.args[0] in labels:
                raise FldError(f"line {operation.line}: duplicate label {operation.args[0]}")
            labels[operation.args[0]] = offset
        elif operation.name == "end_data":
            if operation.args:
                raise FldError(f"line {operation.line}: end_data takes no arguments")
            if end_data is not None:
                raise FldError(f"line {operation.line}: duplicate end_data")
            end_data = offset
        elif end_data is not None:
            raise FldError(f"line {operation.line}: data appears after end_data")
        offset += _operation_size(operation, offset)
    if end_data is None:
        raise FldError("missing end_data")
    if labels.get("data_end") != end_data:
        raise FldError("label data_end must immediately precede end_data")
    label_operations = {
        operation.args[0]: index
        for index, operation in enumerate(operations)
        if operation.name == "label"
    }
    counted_blocks = {
        "model_items": "model_item",
        "model_assets": "model_asset",
    }
    for index, operation in enumerate(operations):
        item_name = counted_blocks.get(operation.name)
        if item_name is not None:
            expected = _int(_fields(operation.args).get("count", "-1"))
            actual = 0
            for item in operations[index + 1 :]:
                if item.name != item_name:
                    break
                actual += 1
            if actual != expected:
                raise FldError(
                    f"line {operation.line}: {operation.name} count is {expected}, "
                    f"followed by {actual} {item_name} directives"
                )
        if operation.name != "model_draw":
            continue
        fields = _fields(operation.args)
        if set(fields) != {"asset", "qwords", "packet"}:
            raise FldError(
                f"line {operation.line}: model_draw expects asset=... qwords=... packet=..."
            )
        packet_name = _symbol(fields["packet"])
        if packet_name not in label_operations:
            raise FldError(f"unknown label @{packet_name}")
        expected_size = _int(fields["qwords"]) * 0x10
        packet_index = label_operations[packet_name] + 1
        actual_size = 0
        while (
            packet_index < len(operations)
            and operations[packet_index].name
            in MESH_PACKET_DIRECTIVES | {"packet_data"}
        ):
            actual_size += _operation_size(operations[packet_index], 0)
            packet_index += 1
        if actual_size != expected_size:
            raise FldError(
                f"line {operation.line}: model_draw declares {expected_size} packet bytes, "
                f"but @{packet_name} contains {actual_size}"
            )
    return labels, end_data


def encode(operations: tuple[Operation, ...]) -> bytes:
    labels, data_end = _layout(operations)
    mesh_codes = _model_mesh_source_codes(operations)
    output = bytearray()
    relocations: list[int] = []
    header_offset: int | None = None

    def value(text: str) -> int:
        if text == "null":
            return 0
        if text.startswith("offset:"):
            return _int(text[7:])
        if text.startswith("@"):
            symbol = _symbol(text)
            if symbol not in labels:
                raise FldError(f"unknown label @{symbol}")
            return labels[symbol]
        return _int(text)

    def pointer(text: str) -> bytes:
        location = len(output)
        target = value(text)
        if target:
            relocations.append(location)
        return struct.pack("<I", target)

    def checked_fields(operation: Operation, required: tuple[str, ...]) -> dict[str, str]:
        fields = _fields(operation.args)
        if set(fields) != set(required):
            missing = set(required) - set(fields)
            extra = set(fields) - set(required)
            raise FldError(
                f"line {operation.line}: fields differ; missing={sorted(missing)} extra={sorted(extra)}"
            )
        return fields

    for operation_index, operation in enumerate(operations):
        name, args = operation.name, operation.args
        if name in {"label", "end_data"}:
            continue
        if name == "header":
            if output or header_offset is not None:
                raise FldError(f"line {operation.line}: header must be first")
            f = checked_fields(
                operation,
                ("version", "magic", "type_count", "type_table", "word_1c", "word_20", "word_24", "word_28", "word_2c", "word_30", "word_34", "word_38", "word_3c"),
            )
            if f["magic"] not in ("FLD1", "FLD2"):
                raise FldError(f"line {operation.line}: unsupported magic")
            header_offset = len(output)
            output.extend(struct.pack("<I4s", _int(f["version"]), f["magic"].encode("ascii")))
            output.extend(pointer("@data_end"))
            output.extend(struct.pack("<II", data_end, 0))
            output.extend(struct.pack("<I", _int(f["type_count"])))
            output.extend(pointer(f["type_table"]))
            for key in ("word_1c", "word_20", "word_24", "word_28", "word_2c", "word_30", "word_34", "word_38", "word_3c"):
                if f[key].startswith("@") or f[key].startswith("offset:"):
                    output.extend(pointer(f[key]))
                else:
                    output.extend(struct.pack("<I", value(f[key])))
        elif name == "type":
            f = checked_fields(operation, ("id", "count", "resources"))
            output.extend(struct.pack("<II", _int(f["id"]), _int(f["count"])))
            output.extend(pointer(f["resources"]))
        elif name == "resource":
            f = checked_fields(operation, ("serial", "flags", "type", "name", "reserved", "transform", "area", "link", "sblock", "data"))
            serial, flags = _int(f["serial"]), _int(f["flags"])
            if not 0 <= serial <= 0xFFFF or not 0 <= flags <= 0xFFFF:
                raise FldError(f"line {operation.line}: resource serial/flags exceed u16")
            output.extend(struct.pack("<II", serial | flags << 16, _int(f["type"])))
            output.extend(pointer(f["name"]))
            output.extend(struct.pack("<I", _int(f["reserved"])))
            for key in ("transform", "area", "link", "sblock", "data"):
                output.extend(pointer(f[key]))
        elif name == "transform":
            f = checked_fields(operation, ("position", "rotation", "scale"))
            values = _csv(f["position"], 4, _parse_float) + _csv(f["rotation"], 4, _parse_float) + _csv(f["scale"], 4, _parse_float)
            output.extend(struct.pack("<12f", *values))
        elif name == "area":
            f = checked_fields(operation, ("number", "name"))
            try:
                raw = f["name"].encode("ascii")
            except UnicodeEncodeError as exc:
                raise FldError(f"line {operation.line}: area name is not ASCII") from exc
            if len(raw) > STRING_SIZE:
                raise FldError(f"line {operation.line}: area name is too long")
            start = len(output)
            output.extend(struct.pack("<I", _int(f["number"])))
            relocations.append(len(output))
            output.extend(struct.pack("<I", start + 0x10))
            output.extend(bytes(8))
            output.extend(raw + bytes(STRING_SIZE - len(raw)))
        elif name == "collision":
            f = checked_fields(operation, ("vertex_count", "face_count", "extra_count", "vertices", "faces", "stop", "reserved"))
            start = len(output)
            output.extend(struct.pack("<II", 0, 0))
            relocations.append(len(output))
            output.extend(struct.pack("<I", start + 0x10))
            output.extend(struct.pack("<I", 0))
            output.extend(struct.pack("<III", _int(f["vertex_count"]), _int(f["face_count"]), _int(f["extra_count"])))
            output.extend(pointer(f["vertices"]))
            output.extend(pointer(f["faces"]))
            output.extend(pointer(f["stop"]))
            output.extend(struct.pack("<II", *_csv(f["reserved"], 2)))
        elif name == "vertex":
            if len(args) != 4:
                raise FldError(f"line {operation.line}: vertex expects four floats")
            output.extend(struct.pack("<4f", *(_parse_float(arg) for arg in args)))
        elif name == "face":
            f = _fields(operation.args)
            common = {
                "attributes", "move_floor", "sound", "stop", "place",
                "automap", "vertices", "special",
            }
            if "encounter_zone" in f:
                required = common | {"encounter_zone"}
                encounter_type, encounter_zone = 1, _int(f["encounter_zone"])
            else:
                required = common | {"encounter_type", "encounter"}
                encounter_type = _int(f["encounter_type"])
                encounter_zone = _int(f["encounter"])
            if set(f) != required:
                missing = required - set(f)
                extra = set(f) - required
                raise FldError(
                    f"line {operation.line}: fields differ; "
                    f"missing={sorted(missing)} extra={sorted(extra)}"
                )
            output.extend(
                struct.pack(
                    "<IBBH HBB 4I hhhh",
                    _int(f["attributes"]), _int(f["move_floor"]), _int(f["sound"]), _int(f["stop"]), _int(f["place"]),
                    *_csv(f["automap"], 2), *_csv(f["vertices"], 4), encounter_type, encounter_zone, *_csv(f["special"], 2),
                )
            )
        elif name == "camera":
            f = checked_fields(operation, ("fovy",))
            output.extend(struct.pack("<f", _parse_float(f["fovy"])))
        elif name == "texture_list":
            if len(args) != 1:
                raise FldError(f"line {operation.line}: texture_list expects one filename")
            try:
                raw = args[0].encode("ascii")
            except UnicodeEncodeError as exc:
                raise FldError(f"line {operation.line}: texture-list filename is not ASCII") from exc
            if len(raw) >= STRING_SIZE:
                raise FldError(f"line {operation.line}: texture-list filename is too long")
            output.extend(raw + bytes(STRING_SIZE - len(raw)))
        elif name == "effect":
            f = checked_fields(operation, ("flags", "type", "selector", "size", "parameters"))
            output.extend(
                struct.pack(
                    "<III3f6I",
                    _int(f["flags"]),
                    _int(f["type"]),
                    _int(f["selector"]),
                    *_csv(f["size"], 3, _parse_float),
                    *_csv(f["parameters"], 6),
                )
            )
        elif name == "light":
            f = checked_fields(
                operation,
                (
                    "reserved", "flags", "animation", "inner_radius", "outer_radius",
                    "softness", "bias", "diffuse", "ambient",
                ),
            )
            output.extend(
                struct.pack(
                    "<III10f",
                    _int(f["reserved"]),
                    _int(f["flags"]),
                    _int(f["animation"]),
                    _parse_float(f["inner_radius"]),
                    _parse_float(f["outer_radius"]),
                    _parse_float(f["softness"]),
                    _parse_float(f["bias"]),
                    *_csv(f["diffuse"], 3, _parse_float),
                    *_csv(f["ambient"], 3, _parse_float),
                )
            )
        elif name == "model_resource":
            f = checked_fields(
                operation,
                ("items", "assets", "motion"),
            )
            output.extend(struct.pack("<4I", 0, 1, 0, 0))
            output.extend(pointer(f["items"]))
            output.extend(pointer(f["assets"]))
            output.extend(struct.pack("<II", 0, 0))
            output.extend(pointer(f["motion"]))
        elif name == "model_items":
            f = checked_fields(operation, ("count",))
            output.extend(struct.pack("<4I", _int(f["count"]), 0, 0, 0))
        elif name == "model_item":
            f = checked_fields(
                operation,
                (
                    "node_id", "parent", "rotation", "position", "scale", "bounds",
                    "commands",
                ),
            )
            output.extend(
                struct.pack(
                    "<HHIIi3fI4f4f",
                    1,
                    0,
                    0,
                    _int(f["node_id"]),
                    _int(f["parent"]),
                    *_csv(f["rotation"], 3, _parse_float),
                    0,
                    *_csv(f["position"], 4, _parse_float),
                    *_csv(f["scale"], 4, _parse_float),
                )
            )
            output.extend(pointer(f["bounds"]))
            output.extend(pointer(f["commands"]))
            output.extend(bytes(8))
        elif name == "model_bounds":
            f = checked_fields(operation, ("minimum", "maximum"))
            output.extend(
                struct.pack(
                    "<6f",
                    *_csv(f["minimum"], 3, _parse_float),
                    *_csv(f["maximum"], 3, _parse_float),
                )
            )
        elif name == "model_assets":
            f = checked_fields(operation, ("count",))
            output.extend(struct.pack("<I", _int(f["count"])))
        elif name == "model_asset":
            f = _model_asset_source_fields(operation)
            flags = sum(
                bit
                for bit, field_name, _, _ in MODEL_ASSET_FIELDS
                if field_name in f
            )
            output.extend(struct.pack("<IHH", _int(f["index"]), 0, flags))
            for _, field_name, kind, width in MODEL_ASSET_FIELDS:
                if field_name not in f:
                    continue
                if kind == "u32":
                    output.extend(
                        struct.pack("<" + "I" * width, *_csv(f[field_name], width))
                    )
                elif kind == "float":
                    output.extend(
                        struct.pack(
                            "<" + "f" * width,
                            *_csv(f[field_name], width, _parse_float),
                        )
                    )
                elif kind == "resource":
                    output.extend(struct.pack("<HH", _int(f[field_name]), 0))
                elif kind == "resource_pair":
                    output.extend(
                        struct.pack("<HH", *_csv(f[field_name], width))
                    )
                else:
                    raise AssertionError(kind)
        elif name == "model_draw_set":
            f = checked_fields(operation, ("lists",))
            for reference in _references(f["lists"]):
                output.extend(pointer(reference))
            output.extend(bytes(4))
        elif name == "model_draw_list":
            f = checked_fields(operation, ("selector", "draws"))
            references = _references(f["draws"])
            selector = _int(f["selector"])
            if len(references) > 0xFFFF or not 0 <= selector <= 0xFFFF:
                raise FldError(
                    f"line {operation.line}: draw count or selector exceeds u16"
                )
            output.extend(struct.pack("<I", len(references) | selector << 16))
            for reference in references:
                output.extend(pointer(reference))
        elif name == "model_draw":
            f = checked_fields(operation, ("asset", "qwords", "packet"))
            asset, quadwords = _int(f["asset"]), _int(f["qwords"])
            if not 0 <= asset <= 0xFFFF or not 0 <= quadwords <= 0xFFFF:
                raise FldError(
                    f"line {operation.line}: asset index or quadword count exceeds u16"
                )
            output.extend(struct.pack("<II", 1, quadwords | asset << 16))
            output.extend(pointer(f["packet"]))
            output.extend(bytes(4))
        elif name == "model_motion_playbook":
            f = checked_fields(operation, ("clip_count", "bindings", "clips"))
            clip_count, binding_count = _int(f["clip_count"]), _int(f["bindings"])
            output.extend(struct.pack("<HH", clip_count, binding_count))
            output.extend(pointer(f["clips"]))
        elif name == "model_motion_binding":
            family, selector, target, _ = _model_motion_binding_source(operation)
            output.extend(struct.pack("<II", family << 16 | selector, target))
        elif name == "model_motion_clip_table":
            f = checked_fields(operation, ("clips",))
            clips = f["clips"].split(",") if f["clips"] else []
            for clip in clips:
                output.extend(pointer(clip))
        elif name == "model_motion_clip":
            f = checked_fields(operation, ("duration", "reserved"))
            output.extend(struct.pack("<HH", _int(f["duration"]), _int(f["reserved"])))
        elif name == "model_motion_track":
            f = checked_fields(operation, ("format", "frames"))
            format_name = f["format"]
            _, stride, _, _ = MODEL_MOTION_VALUE_DIRECTIVES[format_name]
            frames = tuple(_int(value) for value in f["frames"].split(","))
            frame_size = (len(frames) * 2 + 3) & ~3
            size = 8 + frame_size + len(frames) * stride
            output.extend(struct.pack("<IHH", size, len(frames), stride))
            output.extend(struct.pack("<" + "H" * len(frames), *frames))
            output.extend(bytes(frame_size - len(frames) * 2))
        elif name in MODEL_MOTION_DIRECTIVE_FORMATS:
            format_name = MODEL_MOTION_DIRECTIVE_FORMATS[name]
            _, _, width, value_kind = MODEL_MOTION_VALUE_DIRECTIVES[format_name]
            for arg in args:
                if value_kind == "float":
                    output.extend(
                        struct.pack(
                            "<" + "f" * width,
                            *_csv(arg, width, _parse_float),
                        )
                    )
                elif value_kind == "s16":
                    output.extend(struct.pack("<" + "h" * width, *_csv(arg, width)))
                elif value_kind == "u8":
                    output.extend(struct.pack("<" + "B" * width, *_csv(arg, width)))
                else:
                    raise AssertionError(value_kind)
        elif name == "mesh_header":
            f = checked_fields(operation, ("triangles", "vertices", "controls"))
            triangles, vertices = _int(f["triangles"]), _int(f["vertices"])
            controls = _csv(f["controls"], 2)
            output.extend(
                struct.pack(
                    "<I4H",
                    mesh_codes[operation_index],
                    triangles,
                    vertices,
                    *controls,
                )
            )
        elif name in MESH_SECTION_DIRECTIVES or name == "mesh_program":
            output.extend(struct.pack("<I", mesh_codes[operation_index]))
        elif name in MESH_DATA_DIRECTIVES:
            _, width = MESH_DATA_DIRECTIVES[name]
            if name in {"triangle", "color"}:
                for arg in args:
                    output.extend(struct.pack(f"<{width}B", *_csv(arg, width)))
            else:
                for arg in args:
                    output.extend(
                        struct.pack(
                            f"<{width}f", *_csv(arg, width, _parse_float)
                        )
                    )
        elif name == "vif_nops":
            f = checked_fields(operation, ("count",))
            output.extend(bytes(_int(f["count"]) * 4))
        elif name == "packet_data":
            output.extend(bytes.fromhex(args[0]))
        elif name == "event":
            f = checked_fields(operation, ("flags", "label", "reserved"))
            output.extend(struct.pack("<I", _int(f["flags"])))
            output.extend(pointer(f["label"]))
            output.extend(struct.pack("<II", *_csv(f["reserved"], 2)))
        elif name == "motion":
            f = checked_fields(operation, ("tracks",))
            tracks = f["tracks"].split(",") if f["tracks"] else []
            start = len(output)
            output.extend(struct.pack("<I", len(tracks)))
            for track in tracks:
                try:
                    kind_text, curve = track.split(":", 1)
                except ValueError as exc:
                    raise FldError(
                        f"line {operation.line}: motion track must be KIND:@CURVE"
                    ) from exc
                kind = MOTION_KIND_IDS.get(kind_text)
                if kind is None:
                    raise FldError(
                        f"line {operation.line}: unknown motion kind {kind_text!r}"
                    )
                output.extend(struct.pack("<I", kind))
                output.extend(pointer(curve))
            size = (4 + len(tracks) * 8 + 0xF) & ~0xF
            output.extend(bytes(start + size - len(output)))
        elif name == "motion_curve":
            f = checked_fields(operation, ("count", "values", "keys", "word_0c"))
            output.extend(struct.pack("<I", _int(f["count"])))
            output.extend(pointer(f["values"]))
            output.extend(pointer(f["keys"]))
            output.extend(struct.pack("<I", _int(f["word_0c"])))
        elif name == "placement":
            f = checked_fields(operation, ("kind", "event", "visible", "payload"))
            output.extend(struct.pack("<IiI", _int(f["kind"]), _int(f["event"]), _int(f["visible"])))
            output.extend(pointer(f["payload"]))
        elif name == "special_point":
            f = checked_fields(operation, ("kind", "id"))
            kind = SPECIAL_POINT_KIND_IDS.get(f["kind"])
            if kind is None:
                raise FldError(
                    f"line {operation.line}: unknown special point kind {f['kind']!r}"
                )
            output.extend(struct.pack("<II", kind, _int(f["id"])))
        elif name == "string16":
            if len(args) != 1:
                raise FldError(f"line {operation.line}: string16 expects one string")
            try:
                raw = args[0].encode("ascii")
            except UnicodeEncodeError as exc:
                raise FldError(f"line {operation.line}: string16 is not ASCII") from exc
            if len(raw) > STRING_SIZE:
                raise FldError(f"line {operation.line}: string16 is too long")
            output.extend(raw + bytes(STRING_SIZE - len(raw)))
        elif name == "cstring":
            if len(args) != 1:
                raise FldError(f"line {operation.line}: cstring expects one string")
            try:
                raw = args[0].encode("ascii")
            except UnicodeEncodeError as exc:
                raise FldError(f"line {operation.line}: cstring is not ASCII") from exc
            if b"\0" in raw:
                raise FldError(f"line {operation.line}: cstring contains NUL")
            output.extend(raw + b"\0")
        elif name == "pointer":
            if len(args) != 1:
                raise FldError(f"line {operation.line}: pointer expects one label")
            output.extend(pointer(args[0]))
        elif name in MOTION_KIND_IDS:
            width = MOTION_KINDS[MOTION_KIND_IDS[name]][1]
            if len(args) != width:
                raise FldError(
                    f"line {operation.line}: {name} expects {width} floats"
                )
            output.extend(struct.pack("<" + "f" * width, *(_parse_float(arg) for arg in args)))
        elif name == "keys":
            if not args:
                raise FldError(f"line {operation.line}: keys expects at least one frame")
            output.extend(struct.pack("<" + "I" * len(args), *(_int(arg) for arg in args)))
        elif name in {"u32", "s32"}:
            fmt = "<" + ("I" if name == "u32" else "i") * len(args)
            output.extend(struct.pack(fmt, *(_int(arg) for arg in args)))
        elif name == "f32":
            output.extend(struct.pack("<" + "f" * len(args), *(_parse_float(arg) for arg in args)))
        elif name == "bytes":
            output.extend(bytes.fromhex(args[0]))
        elif name == "zeros":
            output.extend(bytes(_int(args[0])))
        elif name == "align":
            alignment = _int(args[0])
            output.extend(bytes((-len(output)) & (alignment - 1)))
        else:
            raise AssertionError(name)

    if header_offset is None:
        raise FldError("missing header")
    if len(output) != data_end:
        raise FldError(f"assembled data is 0x{len(output):x}, expected 0x{data_end:x}")
    packed = _encode_relocations(relocations)
    struct.pack_into("<I", output, 0x10, len(packed))
    result = bytes(output) + packed
    validate(result)
    return result


def decode(data: bytes) -> tuple[Operation, ...]:
    """Decode through the canonical source representation."""
    return parse_source(render_source(data))


def _command_disassemble(args: argparse.Namespace) -> None:
    source = render_source(args.input.read_bytes())
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(source, encoding="utf-8")
    else:
        sys.stdout.write(source)


def _command_assemble(args: argparse.Namespace) -> None:
    data = encode(parse_source(args.input.read_text(encoding="utf-8")))
    if bool(args.scripts) != bool(args.warps):
        raise FldError("--scripts and --warps must be supplied together")
    if args.scripts:
        match = re.fullmatch(r"f(\d{3})_(\d{3})", args.input.stem, re.IGNORECASE)
        if not match:
            raise FldError("linked source name must be fNNN_AAA.fldasm")
        summary = validate_links(
            data,
            args.scripts.read_text(encoding="utf-8"),
            args.warps.read_text(encoding="utf-8"),
            int(match.group(1)),
            int(match.group(2)),
        )
        print(
            f"linked {summary.event_procedures} events, {summary.actor_resources} actors, "
            f"and {summary.destination_resources} destinations"
        )
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_bytes(data)


def _command_verify(args: argparse.Namespace) -> None:
    data = args.input.read_bytes()
    rebuilt = encode(decode(data))
    if rebuilt != data:
        raise FldError("FLD did not round-trip")
    print(f"{args.input}: exact ({len(data)} bytes, sha1 {hashlib.sha1(data).hexdigest()})")


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    commands = parser.add_subparsers(dest="command", required=True)
    disassemble = commands.add_parser("disassemble", help="write readable FLD1/FLD2 source")
    disassemble.add_argument("input", type=Path)
    disassemble.add_argument("output", nargs="?", type=Path)
    assemble = commands.add_parser("assemble", help="assemble FLD1/FLD2 source")
    assemble.add_argument("input", type=Path)
    assemble.add_argument("output", type=Path)
    assemble.add_argument("--scripts", type=Path, help="paired field BF source")
    assemble.add_argument("--warps", type=Path, help="paired field WAP source")
    verify = commands.add_parser("verify", help="verify canonical exact round-trip")
    verify.add_argument("input", type=Path)
    args = parser.parse_args()
    try:
        {"disassemble": _command_disassemble, "assemble": _command_assemble, "verify": _command_verify}[args.command](args)
    except (FldError, OSError) as exc:
        parser.error(str(exc))


if __name__ == "__main__":
    main()
