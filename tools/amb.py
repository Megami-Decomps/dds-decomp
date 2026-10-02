#!/usr/bin/env python3
"""Disassemble and assemble DDS automap (ATMP/AMB) resources.

AMB is a relocatable object graph.  The source language names the area,
sub-block, icon, bounds, and model-root records understood by the field
runtime.  Pointers inside the still-opaque model payload remain labelled
pointer directives, so the entire file can be edited and relocated without
pretending that unknown bytes have known semantics.
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

import reloc


HEADER_SIZE = 0x20
AREA_SIZE = 0x14
SBLOCK_SIZE = 0x1C
ICON_SIZE = 0x08
MODEL_SIZE = 0x08
VEC3_SIZE = 0x0C
STRING_SIZE = 0x10


class AmbError(ValueError):
    """Raised when AMB data or source is invalid."""


@dataclass(frozen=True)
class Area:
    offset: int
    name: int
    sblocks: int
    sblock_count: int
    model: int
    position: int


@dataclass(frozen=True)
class Sblock:
    offset: int
    name: int
    node: int
    icons: int
    icon_count: int
    floor: int
    bound_min: int
    bound_max: int


@dataclass(frozen=True)
class Icon:
    offset: int
    type_id: int
    position: int


@dataclass(frozen=True)
class AmbFile:
    data_end: int
    relocations: tuple[int, ...]
    areas_offset: int
    areas: tuple[Area, ...]
    sblocks: tuple[tuple[Sblock, ...], ...]
    icons: tuple[tuple[tuple[Icon, ...], ...], ...]


@dataclass(frozen=True)
class Operation:
    line: int
    name: str
    args: tuple[str, ...]


def _range(data_end: int, offset: int, size: int, context: str) -> None:
    if offset < 0 or size < 0 or offset + size > data_end:
        raise AmbError(f"{context} lies outside the AMB data region")


def _u32(data: bytes, offset: int) -> int:
    return struct.unpack_from("<I", data, offset)[0]


def _pointer(
    data: bytes, relocations: set[int], location: int, context: str, *, required: bool
) -> int:
    value = _u32(data, location)
    if bool(value) != (location in relocations):
        state = "non-null" if value else "null"
        raise AmbError(f"{context} is {state} but its relocation does not agree")
    if required and not value:
        raise AmbError(f"{context} is null")
    return value


def _fixed_string(data: bytes, data_end: int, offset: int, context: str) -> str:
    _range(data_end, offset, STRING_SIZE, context)
    raw = data[offset : offset + STRING_SIZE]
    value, separator, padding = raw.partition(b"\0")
    if not separator:
        raise AmbError(f"{context} is not null-terminated")
    if any(padding):
        raise AmbError(f"{context} has nonzero string padding")
    try:
        return value.decode("ascii")
    except UnicodeDecodeError as exc:
        raise AmbError(f"{context} is not ASCII") from exc


def decode(data: bytes) -> AmbFile:
    """Validate and decode the known AMB object graph."""

    if len(data) < HEADER_SIZE:
        raise AmbError("AMB header is truncated")
    version, magic, data_end, data_end_copy, reloc_size, data_start, areas_at, area_count = (
        struct.unpack_from("<I4s6I", data)
    )
    if version != 3 or magic != b"ATMP":
        raise AmbError("expected version-3 ATMP data")
    if data_end != data_end_copy:
        raise AmbError("AMB data-size fields disagree")
    if data_start != HEADER_SIZE:
        raise AmbError(f"AMB data-start pointer is 0x{data_start:x}, expected 0x20")
    if data_end < HEADER_SIZE or data_end + reloc_size != len(data):
        raise AmbError("AMB data and relocation sizes do not match the file")

    try:
        relocation_tuple = reloc.decode(data[data_end:])
        if reloc.encode(list(relocation_tuple)) != data[data_end:]:
            raise AmbError("AMB uses a noncanonical relocation stream")
    except reloc.RelocationError as exc:
        raise AmbError(str(exc)) from exc
    relocations = set(relocation_tuple)
    for location in relocation_tuple:
        _range(data_end, location, 4, "relocation word")
        target = _u32(data, location)
        if not target or target > data_end:
            raise AmbError(f"relocation at 0x{location:x} targets 0x{target:x}")

    if {location for location in relocations if location < HEADER_SIZE} != {
        0x08,
        0x14,
        0x18,
    }:
        raise AmbError("AMB header relocations do not match its pointer fields")
    if _u32(data, 0x08) != data_end:
        raise AmbError("AMB data-end pointer does not name the relocation boundary")
    _pointer(data, relocations, 0x18, "area table", required=area_count != 0)
    _range(data_end, areas_at, area_count * AREA_SIZE, "area table")

    areas: list[Area] = []
    all_sblocks: list[tuple[Sblock, ...]] = []
    all_icons: list[tuple[tuple[Icon, ...], ...]] = []
    for area_index in range(area_count):
        offset = areas_at + area_index * AREA_SIZE
        name, sblocks_at, sblock_count, model, position = struct.unpack_from(
            "<5I", data, offset
        )
        for field_offset, context in (
            (0, "name"),
            (4, "sub-block table"),
            (12, "model root"),
            (16, "position"),
        ):
            _pointer(
                data,
                relocations,
                offset + field_offset,
                f"area {area_index} {context}",
                required=True,
            )
        if offset + 8 in relocations:
            raise AmbError(f"area {area_index} count word is relocated")
        _fixed_string(data, data_end, name, f"area {area_index} name")
        _range(data_end, sblocks_at, sblock_count * SBLOCK_SIZE, f"area {area_index} sub-blocks")
        _range(data_end, model, MODEL_SIZE, f"area {area_index} model root")
        _range(data_end, position, VEC3_SIZE, f"area {area_index} position")
        for field_offset, context in ((0, "geometry"), (4, "material")):
            _pointer(
                data,
                relocations,
                model + field_offset,
                f"area {area_index} model {context}",
                required=False,
            )

        area = Area(offset, name, sblocks_at, sblock_count, model, position)
        areas.append(area)
        sblock_rows: list[Sblock] = []
        icon_groups: list[tuple[Icon, ...]] = []
        for sblock_index in range(sblock_count):
            sblock_offset = sblocks_at + sblock_index * SBLOCK_SIZE
            values = struct.unpack_from("<7I", data, sblock_offset)
            name_at, node, icons_at, icon_count, _, bound_min, bound_max = values
            for field_index, context, required in (
                (0, "name", True),
                (2, "icon table", icon_count != 0),
                (5, "minimum bound", False),
                (6, "maximum bound", False),
            ):
                _pointer(
                    data,
                    relocations,
                    sblock_offset + field_index * 4,
                    f"area {area_index} sub-block {sblock_index} {context}",
                    required=required,
                )
            if any(
                sblock_offset + index * 4 in relocations for index in (1, 3, 4)
            ):
                raise AmbError(
                    f"area {area_index} sub-block {sblock_index} has a relocated scalar"
                )
            _fixed_string(
                data,
                data_end,
                name_at,
                f"area {area_index} sub-block {sblock_index} name",
            )
            if icons_at:
                _range(
                    data_end,
                    icons_at,
                    icon_count * ICON_SIZE,
                    f"area {area_index} sub-block {sblock_index} icons",
                )
            for bound, context in ((bound_min, "minimum bound"), (bound_max, "maximum bound")):
                if bound:
                    _range(
                        data_end,
                        bound,
                        VEC3_SIZE,
                        f"area {area_index} sub-block {sblock_index} {context}",
                    )

            sblock = Sblock(
                sblock_offset,
                name_at,
                node,
                icons_at,
                icon_count,
                struct.unpack_from("<i", data, sblock_offset + 16)[0],
                bound_min,
                bound_max,
            )
            sblock_rows.append(sblock)
            icons: list[Icon] = []
            for icon_index in range(icon_count):
                icon_offset = icons_at + icon_index * ICON_SIZE
                type_id, icon_position = struct.unpack_from("<2I", data, icon_offset)
                if icon_offset in relocations:
                    raise AmbError(
                        f"area {area_index} sub-block {sblock_index} icon {icon_index} type is relocated"
                    )
                _pointer(
                    data,
                    relocations,
                    icon_offset + 4,
                    f"area {area_index} sub-block {sblock_index} icon {icon_index} position",
                    required=True,
                )
                _range(
                    data_end,
                    icon_position,
                    VEC3_SIZE,
                    f"area {area_index} sub-block {sblock_index} icon {icon_index} position",
                )
                icons.append(Icon(icon_offset, type_id, icon_position))
            icon_groups.append(tuple(icons))
        all_sblocks.append(tuple(sblock_rows))
        all_icons.append(tuple(icon_groups))

    return AmbFile(
        data_end,
        relocation_tuple,
        areas_at,
        tuple(areas),
        tuple(all_sblocks),
        tuple(all_icons),
    )


def validate(data: bytes) -> None:
    decode(data)


def _float_text(data: bytes, offset: int) -> str:
    bits = _u32(data, offset)
    value = struct.unpack_from("<f", data, offset)[0]
    if math.isfinite(value):
        text = repr(value)
        if struct.pack("<f", float(text)) == struct.pack("<I", bits):
            return text
    return f"bits:0x{bits:08x}"


def _parse_float(text: str) -> float:
    if text.startswith("bits:0x"):
        try:
            return struct.unpack("<f", struct.pack("<I", int(text[7:], 16)))[0]
        except (ValueError, struct.error) as exc:
            raise AmbError(f"invalid float bits {text!r}") from exc
    try:
        return float(text)
    except ValueError as exc:
        raise AmbError(f"invalid float {text!r}") from exc


def _name_part(value: str, fallback: str) -> str:
    value = re.sub(r"[^A-Za-z0-9_]", "_", value).strip("_")
    if not value:
        return fallback
    return value


def render_source(data: bytes) -> str:
    model = decode(data)
    relocation_set = set(model.relocations)
    targets = {_u32(data, location) for location in model.relocations}
    labels = {offset: f"loc_{offset:05x}" for offset in targets}
    used = {name: offset for offset, name in labels.items()}

    def assign(offset: int, name: str) -> None:
        if not offset:
            return
        old = labels.get(offset)
        if old is not None and not old.startswith("loc_"):
            return
        candidate = name
        if candidate in used and used[candidate] != offset:
            candidate = f"{name}_{offset:05x}"
        if old is not None:
            used.pop(old, None)
        labels[offset] = candidate
        used[candidate] = offset

    assign(model.data_end, "data_end")
    assign(model.areas_offset, "areas")
    area_stems: list[str] = []
    seen_stems: set[str] = set()
    for area_index, area in enumerate(model.areas):
        area_name = _fixed_string(data, model.data_end, area.name, "area name")
        stem = _name_part(area_name, f"area_{area_index + 1:03d}")
        stem = f"area_{stem}"
        if stem in seen_stems:
            stem = f"{stem}_{area_index + 1:03d}"
        seen_stems.add(stem)
        area_stems.append(stem)
        assign(area.name, f"{stem}_name")
        assign(area.sblocks, f"{stem}_sblocks")
        assign(area.model, f"{stem}_model")
        assign(area.position, f"{stem}_position")
        geometry, material = struct.unpack_from("<2I", data, area.model)
        assign(geometry, f"{stem}_geometry")
        assign(material, f"{stem}_material")
        for sblock_index, sblock in enumerate(model.sblocks[area_index]):
            sblock_name = _fixed_string(data, model.data_end, sblock.name, "sub-block name")
            part = _name_part(sblock_name, f"sblock_{sblock_index + 1:02d}")
            child = f"{stem}_{part}"
            assign(sblock.name, f"{child}_name")
            assign(sblock.icons, f"{child}_icons")
            assign(sblock.bound_min, f"{child}_bound_min")
            assign(sblock.bound_max, f"{child}_bound_max")
            for icon_index, icon in enumerate(model.icons[area_index][sblock_index]):
                assign(icon.position, f"{child}_icon_{icon_index + 1}_position")

    def reference(value: int) -> str:
        return "null" if value == 0 else f"@{labels[value]}"

    spans: dict[int, tuple[int, list[str]]] = {}
    starts: list[int] = []

    def add_span(start: int, size: int, lines: list[str], context: str) -> None:
        if size == 0:
            return
        _range(model.data_end, start, size, context)
        old = spans.get(start)
        if old is not None:
            if old[0] == start + size:
                return
            raise AmbError(f"{context} overlaps typed data at 0x{start:x}")
        index = bisect.bisect_left(starts, start)
        if index and spans[starts[index - 1]][0] > start:
            raise AmbError(f"{context} overlaps typed data at 0x{starts[index - 1]:x}")
        if index < len(starts) and starts[index] < start + size:
            raise AmbError(f"{context} overlaps typed data at 0x{starts[index]:x}")
        spans[start] = (start + size, lines)
        starts.insert(index, start)

    area_lines = []
    for area_index, area in enumerate(model.areas):
        area_lines.append(f"# {_fixed_string(data, model.data_end, area.name, 'area name')}")
        area_lines.append(
            "area "
            f"name={reference(area.name)} sblocks={reference(area.sblocks)} "
            f"count={area.sblock_count} model={reference(area.model)} "
            f"position={reference(area.position)}"
        )
    add_span(model.areas_offset, len(model.areas) * AREA_SIZE, area_lines, "area table")

    for area_index, area in enumerate(model.areas):
        stem = area_stems[area_index]
        area_name = _fixed_string(data, model.data_end, area.name, "area name")
        add_span(area.name, STRING_SIZE, [f"string16 {json.dumps(area_name)}"], f"{stem} name")
        sblock_lines = []
        for sblock_index, sblock in enumerate(model.sblocks[area_index]):
            bound_text = f"{reference(sblock.bound_min)},{reference(sblock.bound_max)}"
            sblock_lines.append(
                "sblock "
                f"name={reference(sblock.name)} node={sblock.node} "
                f"icons={reference(sblock.icons)} count={sblock.icon_count} "
                f"floor={sblock.floor} bounds={bound_text}"
            )
        add_span(
            area.sblocks,
            area.sblock_count * SBLOCK_SIZE,
            sblock_lines,
            f"{stem} sub-block table",
        )
        geometry, material = struct.unpack_from("<2I", data, area.model)
        add_span(
            area.model,
            MODEL_SIZE,
            [f"model geometry={reference(geometry)} material={reference(material)}"],
            f"{stem} model root",
        )
        add_span(
            area.position,
            VEC3_SIZE,
            ["vec3 " + " ".join(_float_text(data, area.position + index * 4) for index in range(3))],
            f"{stem} position",
        )
        for sblock_index, sblock in enumerate(model.sblocks[area_index]):
            child = f"{stem} sub-block {sblock_index}"
            name = _fixed_string(data, model.data_end, sblock.name, "sub-block name")
            add_span(sblock.name, STRING_SIZE, [f"string16 {json.dumps(name)}"], f"{child} name")
            icon_lines = [
                f"icon type={icon.type_id} position={reference(icon.position)}"
                for icon in model.icons[area_index][sblock_index]
            ]
            add_span(sblock.icons, sblock.icon_count * ICON_SIZE, icon_lines, f"{child} icons")
            for bound, bound_name in ((sblock.bound_min, "minimum bound"), (sblock.bound_max, "maximum bound")):
                if bound:
                    add_span(
                        bound,
                        VEC3_SIZE,
                        ["vec3 " + " ".join(_float_text(data, bound + index * 4) for index in range(3))],
                        f"{child} {bound_name}",
                    )
            for icon_index, icon in enumerate(model.icons[area_index][sblock_index]):
                add_span(
                    icon.position,
                    VEC3_SIZE,
                    ["vec3 " + " ".join(_float_text(data, icon.position + index * 4) for index in range(3))],
                    f"{child} icon {icon_index} position",
                )

    # A label inside a typed directive cannot be represented.  The DDS corpus
    # targets only object boundaries; diagnose any future profile that differs.
    for target in targets:
        for start in starts:
            end = spans[start][0]
            if start < target < end:
                raise AmbError(f"pointer target 0x{target:x} lies inside typed data at 0x{start:x}")
            if start > target:
                break

    lines = [
        "amb 1",
        (
            "header version=3 magic=ATMP "
            f"areas=@{labels[model.areas_offset]} count={len(model.areas)}"
        ),
    ]
    cursor = HEADER_SIZE
    emitted_labels: set[int] = set()

    def emit_label(offset: int) -> None:
        if offset in labels and offset not in emitted_labels:
            if lines and lines[-1] != "":
                lines.append("")
            lines.append(f"label {labels[offset]}")
            emitted_labels.add(offset)

    def emit_raw(start: int, end: int) -> None:
        raw = data[start:end]
        if raw and not any(raw):
            lines.append(f"zeros {len(raw)}")
            return
        for offset in range(start, end, 32):
            lines.append(f"bytes {data[offset:min(offset + 32, end)].hex()}")

    while cursor < model.data_end:
        emit_label(cursor)
        span = spans.get(cursor)
        if span is not None:
            lines.extend(span[1])
            cursor = span[0]
            continue
        if cursor in relocation_set:
            lines.append(f"pointer @{labels[_u32(data, cursor)]}")
            cursor += 4
            continue
        next_events = [model.data_end]
        next_events.extend(start for start in starts if start > cursor)
        next_events.extend(location for location in model.relocations if location > cursor)
        next_events.extend(offset for offset in labels if offset > cursor)
        end = min(next_events)
        emit_raw(cursor, end)
        cursor = end

    emit_label(model.data_end)
    lines.append("end_data")
    return "\n".join(lines) + "\n"


def _int(text: str) -> int:
    try:
        return int(text, 0)
    except ValueError as exc:
        raise AmbError(f"invalid integer {text!r}") from exc


def _fields(args: tuple[str, ...]) -> dict[str, str]:
    output: dict[str, str] = {}
    for arg in args:
        if "=" not in arg:
            raise AmbError(f"expected name=value, got {arg!r}")
        name, value = arg.split("=", 1)
        if not name or name in output:
            raise AmbError(f"duplicate or empty field {name!r}")
        output[name] = value
    return output


def _symbol(text: str) -> str:
    if not re.fullmatch(r"@[A-Za-z_][A-Za-z0-9_]*", text):
        raise AmbError(f"expected @SYMBOL, got {text!r}")
    return text[1:]


def _reference(text: str, labels: dict[str, int]) -> int:
    if text == "null":
        return 0
    name = _symbol(text)
    try:
        return labels[name]
    except KeyError as exc:
        raise AmbError(f"unknown label @{name}") from exc


def parse_source(source: str) -> tuple[Operation, ...]:
    operations: list[Operation] = []
    saw_preamble = False
    for line_number, raw_line in enumerate(source.splitlines(), 1):
        raw_bytes = re.fullmatch(r"\s*bytes\s+([0-9A-Fa-f]+)\s*", raw_line)
        if raw_bytes:
            tokens = ["bytes", raw_bytes.group(1)]
        else:
            try:
                tokens = shlex.split(raw_line, comments=True, posix=True)
            except ValueError as exc:
                raise AmbError(f"line {line_number}: {exc}") from exc
        if not tokens:
            continue
        if not saw_preamble:
            if tokens != ["amb", "1"]:
                raise AmbError(f"line {line_number}: expected 'amb 1'")
            saw_preamble = True
            continue
        operations.append(Operation(line_number, tokens[0], tuple(tokens[1:])))
    if not saw_preamble:
        raise AmbError("missing 'amb 1' preamble")
    return tuple(operations)


def _operation_size(operation: Operation, offset: int) -> int:
    fixed = {
        "header": HEADER_SIZE,
        "area": AREA_SIZE,
        "sblock": SBLOCK_SIZE,
        "icon": ICON_SIZE,
        "model": MODEL_SIZE,
        "vec3": VEC3_SIZE,
        "string16": STRING_SIZE,
        "pointer": 4,
    }
    if operation.name in fixed:
        return fixed[operation.name]
    if operation.name in {"u32", "s32", "f32"}:
        return 4 * len(operation.args)
    if operation.name == "bytes":
        try:
            return len(bytes.fromhex(operation.args[0]))
        except (IndexError, ValueError) as exc:
            raise AmbError(f"line {operation.line}: invalid bytes directive") from exc
    if operation.name == "zeros":
        return _int(operation.args[0])
    if operation.name == "align":
        alignment = _int(operation.args[0])
        if alignment <= 0 or alignment & (alignment - 1):
            raise AmbError(f"line {operation.line}: alignment must be a positive power of two")
        return (-offset) & (alignment - 1)
    if operation.name in {"label", "end_data"}:
        return 0
    raise AmbError(f"line {operation.line}: unknown directive {operation.name!r}")


def encode(operations: tuple[Operation, ...]) -> bytes:
    labels: dict[str, int] = {}
    offset = 0
    header_count = 0
    end_count = 0
    for operation in operations:
        if operation.name == "label":
            if len(operation.args) != 1 or not re.fullmatch(
                r"[A-Za-z_][A-Za-z0-9_]*", operation.args[0]
            ):
                raise AmbError(f"line {operation.line}: invalid label")
            if operation.args[0] in labels:
                raise AmbError(f"line {operation.line}: duplicate label {operation.args[0]}")
            labels[operation.args[0]] = offset
        elif operation.name == "header":
            header_count += 1
        elif operation.name == "end_data":
            end_count += 1
        offset += _operation_size(operation, offset)
    if header_count != 1:
        raise AmbError("source must contain exactly one header")
    if end_count != 1:
        raise AmbError("source must contain exactly one end_data")
    if "data_end" not in labels:
        raise AmbError("source must define label data_end")
    if labels["data_end"] != offset:
        raise AmbError("label data_end must mark the end of the data directives")

    output = bytearray()
    relocations: list[int] = []

    def pointer(text: str, location: int | None = None) -> int:
        value = _reference(text, labels)
        if value:
            relocations.append(len(output) if location is None else location)
        return value

    for operation in operations:
        try:
            if operation.name in {"label", "end_data"}:
                continue
            if operation.name == "header":
                fields = _fields(operation.args)
                if set(fields) != {"version", "magic", "areas", "count"}:
                    raise AmbError("header expects version, magic, areas, and count")
                if _int(fields["version"]) != 3 or fields["magic"] != "ATMP":
                    raise AmbError("header must use version=3 magic=ATMP")
                if output:
                    raise AmbError("header must be the first data directive")
                areas = _reference(fields["areas"], labels)
                output.extend(
                    struct.pack(
                        "<I4s6I",
                        3,
                        b"ATMP",
                        labels["data_end"],
                        labels["data_end"],
                        0,
                        HEADER_SIZE,
                        areas,
                        _int(fields["count"]),
                    )
                )
                relocations.extend((0x08, 0x14, 0x18))
            elif operation.name == "area":
                fields = _fields(operation.args)
                if set(fields) != {"name", "sblocks", "count", "model", "position"}:
                    raise AmbError("area expects name, sblocks, count, model, and position")
                base = len(output)
                output.extend(
                    struct.pack(
                        "<5I",
                        pointer(fields["name"], base),
                        pointer(fields["sblocks"], base + 4),
                        _int(fields["count"]),
                        pointer(fields["model"], base + 12),
                        pointer(fields["position"], base + 16),
                    )
                )
            elif operation.name == "sblock":
                fields = _fields(operation.args)
                if set(fields) != {"name", "node", "icons", "count", "floor", "bounds"}:
                    raise AmbError("sblock expects name, node, icons, count, floor, and bounds")
                bounds = fields["bounds"].split(",")
                if len(bounds) != 2:
                    raise AmbError("sblock bounds expects two references")
                base = len(output)
                output.extend(
                    struct.pack(
                        "<4Ii2I",
                        pointer(fields["name"], base),
                        _int(fields["node"]),
                        pointer(fields["icons"], base + 8),
                        _int(fields["count"]),
                        _int(fields["floor"]),
                        pointer(bounds[0], base + 20),
                        pointer(bounds[1], base + 24),
                    )
                )
            elif operation.name == "icon":
                fields = _fields(operation.args)
                if set(fields) != {"type", "position"}:
                    raise AmbError("icon expects type and position")
                type_id = _int(fields["type"])
                output.extend(struct.pack("<I", type_id))
                output.extend(struct.pack("<I", pointer(fields["position"])))
            elif operation.name == "model":
                fields = _fields(operation.args)
                if set(fields) != {"geometry", "material"}:
                    raise AmbError("model expects geometry and material")
                base = len(output)
                output.extend(
                    struct.pack(
                        "<2I",
                        pointer(fields["geometry"], base),
                        pointer(fields["material"], base + 4),
                    )
                )
            elif operation.name == "vec3":
                if len(operation.args) != 3:
                    raise AmbError("vec3 expects three values")
                output.extend(struct.pack("<3f", *(_parse_float(value) for value in operation.args)))
            elif operation.name == "string16":
                if len(operation.args) != 1:
                    raise AmbError("string16 expects one string")
                encoded = operation.args[0].encode("ascii")
                if len(encoded) > 15 or b"\0" in encoded:
                    raise AmbError("string16 must be at most 15 non-null ASCII bytes")
                output.extend(encoded + bytes(STRING_SIZE - len(encoded)))
            elif operation.name == "pointer":
                if len(operation.args) != 1:
                    raise AmbError("pointer expects one reference")
                output.extend(struct.pack("<I", pointer(operation.args[0])))
            elif operation.name in {"u32", "s32"}:
                code = "I" if operation.name == "u32" else "i"
                output.extend(struct.pack("<" + code * len(operation.args), *(_int(value) for value in operation.args)))
            elif operation.name == "f32":
                output.extend(struct.pack("<" + "f" * len(operation.args), *(_parse_float(value) for value in operation.args)))
            elif operation.name == "bytes":
                output.extend(bytes.fromhex(operation.args[0]))
            elif operation.name == "zeros":
                output.extend(bytes(_int(operation.args[0])))
            elif operation.name == "align":
                alignment = _int(operation.args[0])
                output.extend(bytes((-len(output)) & (alignment - 1)))
            else:
                raise AssertionError(operation.name)
        except (KeyError, struct.error, UnicodeError, ValueError) as exc:
            if isinstance(exc, AmbError):
                raise AmbError(f"line {operation.line}: {exc}") from exc
            raise AmbError(f"line {operation.line}: {exc}") from exc

    if len(output) != labels["data_end"]:
        raise AmbError("assembled data does not end at label data_end")
    try:
        packed = reloc.encode(relocations)
    except reloc.RelocationError as exc:
        raise AmbError(str(exc)) from exc
    struct.pack_into("<I", output, 0x10, len(packed))
    result = bytes(output) + packed
    validate(result)
    return result


def _command_disassemble(args: argparse.Namespace) -> None:
    source = render_source(args.input.read_bytes())
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(source, encoding="utf-8")
    else:
        sys.stdout.write(source)


def _command_assemble(args: argparse.Namespace) -> None:
    data = encode(parse_source(args.input.read_text(encoding="utf-8")))
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_bytes(data)


def _command_verify(args: argparse.Namespace) -> None:
    data = args.input.read_bytes()
    rebuilt = encode(parse_source(render_source(data)))
    if rebuilt != data:
        raise AmbError("AMB did not round-trip")
    model = decode(data)
    print(
        f"{args.input}: exact ({len(data)} bytes, {len(model.areas)} areas, "
        f"sha1 {hashlib.sha1(data).hexdigest()})"
    )


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    commands = parser.add_subparsers(dest="command", required=True)
    disassemble = commands.add_parser("disassemble", help="write readable AMB source")
    disassemble.add_argument("input", type=Path)
    disassemble.add_argument("output", nargs="?", type=Path)
    assemble = commands.add_parser("assemble", help="assemble AMB source")
    assemble.add_argument("input", type=Path)
    assemble.add_argument("output", type=Path)
    verify = commands.add_parser("verify", help="verify an exact source round trip")
    verify.add_argument("input", type=Path)
    args = parser.parse_args()
    try:
        {"disassemble": _command_disassemble, "assemble": _command_assemble, "verify": _command_verify}[
            args.command
        ](args)
    except (OSError, AmbError) as exc:
        parser.error(str(exc))


if __name__ == "__main__":
    main()
