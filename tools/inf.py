#!/usr/bin/env python3
"""Disassemble and assemble DDS field interaction (``.INF``) tables."""

from __future__ import annotations

import argparse
import json
import re
import shlex
import struct
import sys
from dataclasses import dataclass
from pathlib import Path


PACK_COUNT = 8
PACK_SIZE = 0x54
HIT_COUNT = 5
HIT_SIZE = 0x10
VIEW_COUNT = 40
VIEW_SIZE = 0x20
EX_COUNT = 40
EX_SIZE = 0x14
SET_COUNT = 40
SET_SIZE = 0x138
FLAG_COUNT = 4
FLAG_SIZE = 4
MESSAGE_COUNT = 20
MESSAGE_SIZE = 0x0E
FILE_SIZE = 0x3B80

PACK_OFFSET = 0
VIEW_OFFSET = PACK_OFFSET + PACK_COUNT * PACK_SIZE
EX_OFFSET = VIEW_OFFSET + VIEW_COUNT * VIEW_SIZE
SET_OFFSET = EX_OFFSET + EX_COUNT * EX_SIZE

_SYMBOL = re.compile(r"[A-Za-z_][A-Za-z0-9_]*\Z")
_ROW_TARGET = re.compile(r"row([0-9]|1[0-9])\Z")


class InfError(ValueError):
    """The binary or source is not a canonical DDS INF table."""


@dataclass(frozen=True)
class Hit:
    area: int
    event: str


@dataclass(frozen=True)
class Pack:
    set_index: int
    hits: tuple[Hit, ...]


@dataclass(frozen=True)
class View:
    player_on: int
    player_motion: int
    player_position: str
    camera: str


@dataclass(frozen=True)
class ExtraAction:
    action_id: int
    parameters: tuple[int, int, int, int]


@dataclass(frozen=True)
class Start:
    type_id: int
    area: int
    action: int
    event_hit: int
    event: str


@dataclass(frozen=True)
class FlagSelector:
    flag_id: int
    go_off: int
    go_on: int


@dataclass(frozen=True)
class MessageRow:
    type_id: int
    message_id: int
    go: tuple[int, int, int, int]
    flag_off: int
    flag_on: int
    view_id: int
    ex_id: int


@dataclass(frozen=True)
class InteractionSet:
    start: Start
    flags: tuple[FlagSelector, ...]
    messages: tuple[MessageRow, ...]


@dataclass(frozen=True)
class InfFile:
    packs: tuple[Pack, ...]
    views: tuple[View, ...]
    extra_actions: tuple[ExtraAction, ...]
    sets: tuple[InteractionSet, ...]


DEFAULT_HIT = Hit(0, "01eve_01")
DEFAULT_PACK = Pack(0, (DEFAULT_HIT,) * HIT_COUNT)
DEFAULT_VIEW = View(1, 0, "01pos_01", "01cam_01")
DEFAULT_EXTRA_ACTION = ExtraAction(0, (0, 0, 0, 0))
DEFAULT_START = Start(0xFF, 0xFF, 1, 1, "01eve_01")
DEFAULT_FLAG = FlagSelector(0, 0, 0)
DEFAULT_MESSAGE = MessageRow(0, -1, (0, 0, 0, 0), 0, 0, 0, 0)
DEFAULT_SET = InteractionSet(
    DEFAULT_START,
    (DEFAULT_FLAG,) * FLAG_COUNT,
    (DEFAULT_MESSAGE,) * MESSAGE_COUNT,
)
DEFAULT_FILE = InfFile(
    (DEFAULT_PACK,) * PACK_COUNT,
    (DEFAULT_VIEW,) * VIEW_COUNT,
    (DEFAULT_EXTRA_ACTION,) * EX_COUNT,
    (DEFAULT_SET,) * SET_COUNT,
)


def _range(value: int, minimum: int, maximum: int, context: str) -> int:
    if not minimum <= value <= maximum:
        raise InfError(f"{context} {value} is outside {minimum}..{maximum}")
    return value


def _s32(value: int, context: str) -> int:
    return _range(value, -0x80000000, 0x7FFFFFFF, context)


def _s16(value: int, context: str) -> int:
    return _range(value, -0x8000, 0x7FFF, context)


def _u8(value: int, context: str) -> int:
    return _range(value, 0, 0xFF, context)


def _fixed_ascii(data: bytes, offset: int, size: int, context: str) -> str:
    raw = data[offset : offset + size]
    value, separator, padding = raw.partition(b"\0")
    if separator and any(padding):
        raise InfError(f"{context} has nonzero string padding")
    if any(byte < 0x20 or byte > 0x7E for byte in value):
        raise InfError(f"{context} is not printable ASCII")
    return value.decode("ascii")


def _encode_ascii(value: str, size: int, context: str) -> bytes:
    try:
        raw = value.encode("ascii")
    except UnicodeEncodeError as exc:
        raise InfError(f"{context} is not ASCII") from exc
    if any(byte < 0x20 or byte > 0x7E for byte in raw):
        raise InfError(f"{context} is not printable ASCII")
    if len(raw) > size:
        raise InfError(f"{context} exceeds {size} bytes")
    return raw + bytes(size - len(raw))


def decode(data: bytes) -> InfFile:
    """Decode and validate one fixed-layout DDS INF file."""

    if len(data) != FILE_SIZE:
        raise InfError(f"INF file is {len(data):#x} bytes, expected {FILE_SIZE:#x}")

    packs: list[Pack] = []
    for pack_index in range(PACK_COUNT):
        base = PACK_OFFSET + pack_index * PACK_SIZE
        hits = tuple(
            Hit(
                data[base + 4 + hit_index * HIT_SIZE],
                _fixed_ascii(
                    data,
                    base + 5 + hit_index * HIT_SIZE,
                    15,
                    f"pack {pack_index} hit {hit_index} event",
                ),
            )
            for hit_index in range(HIT_COUNT)
        )
        packs.append(Pack(struct.unpack_from("<i", data, base)[0], hits))

    views = tuple(
        View(
            *struct.unpack_from("<ii", data, VIEW_OFFSET + index * VIEW_SIZE),
            _fixed_ascii(
                data,
                VIEW_OFFSET + index * VIEW_SIZE + 8,
                12,
                f"view {index} player position",
            ),
            _fixed_ascii(
                data,
                VIEW_OFFSET + index * VIEW_SIZE + 20,
                12,
                f"view {index} camera",
            ),
        )
        for index in range(VIEW_COUNT)
    )

    extra_actions = tuple(
        ExtraAction(
            values[0],
            (values[1], values[2], values[3], values[4]),
        )
        for index in range(EX_COUNT)
        for values in (
            struct.unpack_from("<iiiii", data, EX_OFFSET + index * EX_SIZE),
        )
    )

    sets: list[InteractionSet] = []
    for set_index in range(SET_COUNT):
        base = SET_OFFSET + set_index * SET_SIZE
        start = Start(
            data[base],
            data[base + 1],
            data[base + 2],
            data[base + 3],
            _fixed_ascii(data, base + 4, 12, f"set {set_index} event"),
        )
        flags = tuple(
            FlagSelector(*struct.unpack_from("<hBB", data, base + 0x10 + index * FLAG_SIZE))
            for index in range(FLAG_COUNT)
        )
        messages = tuple(
            MessageRow(
                values[0],
                values[1],
                (values[2], values[3], values[4], values[5]),
                values[6],
                values[7],
                values[8],
                values[9],
            )
            for index in range(MESSAGE_COUNT)
            for values in (
                struct.unpack_from(
                    "<hhBBBBhhBB",
                    data,
                    base + 0x20 + index * MESSAGE_SIZE,
                ),
            )
        )
        sets.append(InteractionSet(start, flags, messages))
    return InfFile(tuple(packs), views, extra_actions, tuple(sets))


def encode(inf: InfFile) -> bytes:
    """Encode one INF model to its exact fixed-layout representation."""

    for actual, expected, context in (
        (len(inf.packs), PACK_COUNT, "pack"),
        (len(inf.views), VIEW_COUNT, "view"),
        (len(inf.extra_actions), EX_COUNT, "extra-action"),
        (len(inf.sets), SET_COUNT, "set"),
    ):
        if actual != expected:
            raise InfError(f"expected {expected} {context} rows, found {actual}")

    output = bytearray(FILE_SIZE)
    for pack_index, pack in enumerate(inf.packs):
        if len(pack.hits) != HIT_COUNT:
            raise InfError(f"pack {pack_index} must contain {HIT_COUNT} hits")
        base = PACK_OFFSET + pack_index * PACK_SIZE
        struct.pack_into(
            "<i",
            output,
            base,
            _s32(pack.set_index, f"pack {pack_index} set index"),
        )
        for hit_index, hit in enumerate(pack.hits):
            offset = base + 4 + hit_index * HIT_SIZE
            output[offset] = _u8(
                hit.area, f"pack {pack_index} hit {hit_index} area"
            )
            output[offset + 1 : offset + HIT_SIZE] = _encode_ascii(
                hit.event, 15, f"pack {pack_index} hit {hit_index} event"
            )

    for index, view in enumerate(inf.views):
        base = VIEW_OFFSET + index * VIEW_SIZE
        struct.pack_into(
            "<ii",
            output,
            base,
                _s32(view.player_on, f"view {index} player"),
                _s32(view.player_motion, f"view {index} motion"),
        )
        output[base + 8 : base + 20] = _encode_ascii(
            view.player_position, 12, f"view {index} player position"
        )
        output[base + 20 : base + 32] = _encode_ascii(
            view.camera, 12, f"view {index} camera"
        )

    for index, action in enumerate(inf.extra_actions):
        values = (action.action_id, *action.parameters)
        if len(values) != 5:
            raise InfError(f"extra action {index} must contain four parameters")
        struct.pack_into(
            "<iiiii",
            output,
            EX_OFFSET + index * EX_SIZE,
            *(
                _s32(value, f"extra action {index} value")
                for value in values
            ),
        )

    for set_index, interaction in enumerate(inf.sets):
        if len(interaction.flags) != FLAG_COUNT:
            raise InfError(f"set {set_index} must contain {FLAG_COUNT} flag selectors")
        if len(interaction.messages) != MESSAGE_COUNT:
            raise InfError(f"set {set_index} must contain {MESSAGE_COUNT} message rows")
        base = SET_OFFSET + set_index * SET_SIZE
        start = interaction.start
        for offset, value, name in (
            (0, start.type_id, "type"),
            (1, start.area, "area"),
            (2, start.action, "action"),
            (3, start.event_hit, "event_hit"),
        ):
            output[base + offset] = _u8(value, f"set {set_index} {name}")
        output[base + 4 : base + 16] = _encode_ascii(
            start.event, 12, f"set {set_index} event"
        )
        for index, selector in enumerate(interaction.flags):
            struct.pack_into(
                "<hBB",
                output,
                base + 0x10 + index * FLAG_SIZE,
                _s16(selector.flag_id, f"set {set_index} flag {index} id"),
                _u8(selector.go_off, f"set {set_index} flag {index} off"),
                _u8(selector.go_on, f"set {set_index} flag {index} on"),
            )
        for index, row in enumerate(interaction.messages):
            if len(row.go) != 4:
                raise InfError(f"set {set_index} row {index} must contain four go values")
            struct.pack_into(
                "<hhBBBBhhBB",
                output,
                base + 0x20 + index * MESSAGE_SIZE,
                _s16(row.type_id, f"set {set_index} row {index} type"),
                _s16(row.message_id, f"set {set_index} row {index} message"),
                *(
                    _u8(value, f"set {set_index} row {index} go")
                    for value in row.go
                ),
                _s16(row.flag_off, f"set {set_index} row {index} flag_off"),
                _s16(row.flag_on, f"set {set_index} row {index} flag_on"),
                _u8(row.view_id, f"set {set_index} row {index} view"),
                _u8(row.ex_id, f"set {set_index} row {index} ex"),
            )
    return bytes(output)


def _tokens(line: str, line_number: int) -> list[str]:
    try:
        return shlex.split(line, comments=True, posix=True)
    except ValueError as exc:
        raise InfError(f"line {line_number}: {exc}") from exc


def _values(tokens: list[str], line_number: int) -> dict[str, str]:
    values: dict[str, str] = {}
    for token in tokens:
        if "=" not in token:
            raise InfError(f"line {line_number}: expected key=value, found {token!r}")
        key, value = token.split("=", 1)
        if not key or key in values:
            raise InfError(f"line {line_number}: invalid or duplicate field {key!r}")
        values[key] = value
    return values


def _integer(text: str, line_number: int, context: str) -> int:
    try:
        return int(text, 0)
    except ValueError as exc:
        raise InfError(f"line {line_number}: invalid {context} {text!r}") from exc


def _index(text: str, line_number: int, count: int, context: str) -> int:
    return _range(_integer(text, line_number, context), 0, count - 1, context)


def _start_type(text: str, line_number: int) -> int:
    names = {"npc": 0, "event": 1}
    if text in names:
        return names[text]
    return _integer(text, line_number, "set type")


def _row_type(text: str, line_number: int) -> int:
    names = {"action": -1, "message": 0, "selection": 1}
    if text in names:
        return names[text]
    return _integer(text, line_number, "row type")


def _go(text: str, line_number: int) -> int:
    if text == "warp":
        return 100
    match = _ROW_TARGET.fullmatch(text)
    if match:
        return 10 + int(match.group(1))
    return _u8(_integer(text, line_number, "go target"), "go target")


def _go_list(text: str, line_number: int) -> tuple[int, int, int, int]:
    parts = text.split(",")
    if len(parts) != 4:
        raise InfError(f"line {line_number}: go needs four comma-separated targets")
    return tuple(_go(part, line_number) for part in parts)  # type: ignore[return-value]


def _s32_list(text: str, line_number: int) -> tuple[int, int, int, int]:
    parts = text.split(",")
    if len(parts) != 4:
        raise InfError(f"line {line_number}: params needs four comma-separated integers")
    return tuple(_integer(part, line_number, "parameter") for part in parts)  # type: ignore[return-value]


def parse_source(text: str, message_symbols: dict[str, int] | None = None) -> InfFile:
    """Assemble version-1 INF source on top of the canonical retail template."""

    meaningful = [
        (number, line.strip())
        for number, line in enumerate(text.splitlines(), 1)
        if line.strip() and not line.lstrip().startswith("#")
    ]
    if not meaningful or _tokens(meaningful[0][1], meaningful[0][0]) != ["inf", "1"]:
        raise InfError("source must begin with 'inf 1'")

    packs = list(DEFAULT_FILE.packs)
    views = list(DEFAULT_FILE.views)
    actions = list(DEFAULT_FILE.extra_actions)
    sets = list(DEFAULT_FILE.sets)
    seen: set[tuple[str, int]] = set()
    index = 1
    while index < len(meaningful):
        line_number, line = meaningful[index]
        tokens = _tokens(line, line_number)
        directive = tokens[0]
        if directive == "pack":
            if len(tokens) < 2:
                raise InfError(f"line {line_number}: pack needs an index")
            row_index = _index(tokens[1], line_number, PACK_COUNT, "pack index")
            key = (directive, row_index)
            if key in seen:
                raise InfError(f"line {line_number}: duplicate pack {row_index}")
            seen.add(key)
            values = _values(tokens[2:], line_number)
            if values.keys() != {"set"}:
                raise InfError(f"line {line_number}: pack needs only set=VALUE")
            hits = list(DEFAULT_PACK.hits)
            index += 1
            hit_seen: set[int] = set()
            while index < len(meaningful):
                child_number, child_line = meaningful[index]
                child = _tokens(child_line, child_number)
                if child == ["end"]:
                    break
                if len(child) < 2 or child[0] != "hit":
                    raise InfError(f"line {child_number}: expected hit or end")
                hit_index = _index(
                    child[1], child_number, HIT_COUNT, "hit index"
                )
                if hit_index in hit_seen:
                    raise InfError(f"line {child_number}: duplicate hit {hit_index}")
                hit_seen.add(hit_index)
                fields = _values(child[2:], child_number)
                if fields.keys() != {"area", "event"}:
                    raise InfError(f"line {child_number}: hit needs area and event")
                hits[hit_index] = Hit(
                    _integer(fields["area"], child_number, "area"),
                    fields["event"],
                )
                index += 1
            if index == len(meaningful):
                raise InfError(f"line {line_number}: pack has no end")
            packs[row_index] = Pack(
                _integer(values["set"], line_number, "set index"),
                tuple(hits),
            )
        elif directive == "view":
            if len(tokens) < 2:
                raise InfError(f"line {line_number}: view needs an index")
            row_index = _index(tokens[1], line_number, VIEW_COUNT, "view index")
            key = (directive, row_index)
            if key in seen:
                raise InfError(f"line {line_number}: duplicate view {row_index}")
            seen.add(key)
            fields = _values(tokens[2:], line_number)
            if fields.keys() != {"player", "motion", "position", "camera"}:
                raise InfError(
                    f"line {line_number}: view needs player, motion, position, and camera"
                )
            views[row_index] = View(
                _integer(fields["player"], line_number, "player"),
                _integer(fields["motion"], line_number, "motion"),
                fields["position"],
                fields["camera"],
            )
        elif directive == "ex":
            if len(tokens) < 2:
                raise InfError(f"line {line_number}: ex needs an index")
            row_index = _index(tokens[1], line_number, EX_COUNT, "ex index")
            key = (directive, row_index)
            if key in seen:
                raise InfError(f"line {line_number}: duplicate ex {row_index}")
            seen.add(key)
            fields = _values(tokens[2:], line_number)
            if fields.keys() != {"id", "params"}:
                raise InfError(f"line {line_number}: ex needs id and params")
            actions[row_index] = ExtraAction(
                _integer(fields["id"], line_number, "action id"),
                _s32_list(fields["params"], line_number),
            )
        elif directive == "set":
            if len(tokens) < 2:
                raise InfError(f"line {line_number}: set needs an index")
            row_index = _index(tokens[1], line_number, SET_COUNT, "set index")
            key = (directive, row_index)
            if key in seen:
                raise InfError(f"line {line_number}: duplicate set {row_index}")
            seen.add(key)
            fields = _values(tokens[2:], line_number)
            if fields.keys() != {"kind", "area", "action", "event_hit", "event"}:
                raise InfError(
                    f"line {line_number}: set needs kind, area, action, "
                    "event_hit, and event"
                )
            flags = list(DEFAULT_SET.flags)
            messages = list(DEFAULT_SET.messages)
            child_seen: set[tuple[str, int]] = set()
            index += 1
            while index < len(meaningful):
                child_number, child_line = meaningful[index]
                child = _tokens(child_line, child_number)
                if child == ["end"]:
                    break
                if len(child) < 2 or child[0] not in ("flag", "row"):
                    raise InfError(f"line {child_number}: expected flag, row, or end")
                child_kind = child[0]
                count = FLAG_COUNT if child_kind == "flag" else MESSAGE_COUNT
                child_index = _index(
                    child[1], child_number, count, f"{child_kind} index"
                )
                child_key = (child_kind, child_index)
                if child_key in child_seen:
                    raise InfError(f"line {child_number}: duplicate {child_kind} {child_index}")
                child_seen.add(child_key)
                child_fields = _values(child[2:], child_number)
                if child_kind == "flag":
                    if child_fields.keys() != {"id", "off", "on"}:
                        raise InfError(f"line {child_number}: flag needs id, off, and on")
                    flags[child_index] = FlagSelector(
                        _integer(child_fields["id"], child_number, "flag id"),
                        _go(child_fields["off"], child_number),
                        _go(child_fields["on"], child_number),
                    )
                else:
                    expected = {
                        "kind",
                        "message",
                        "go",
                        "flag_off",
                        "flag_on",
                        "view",
                        "ex",
                    }
                    if child_fields.keys() != expected:
                        required = ", ".join(sorted(expected))
                        raise InfError(
                            f"line {child_number}: row fields must be {required}"
                        )
                    message_text = child_fields["message"]
                    if message_text.startswith("@"):
                        symbol = message_text[1:]
                        if not _SYMBOL.fullmatch(symbol):
                            raise InfError(
                                f"line {child_number}: invalid message symbol "
                                f"{message_text!r}"
                            )
                        if message_symbols is None or symbol not in message_symbols:
                            raise InfError(
                                f"line {child_number}: unknown message symbol {symbol!r}"
                            )
                        message_id = message_symbols[symbol]
                    else:
                        message_id = _integer(message_text, child_number, "message id")
                    messages[child_index] = MessageRow(
                        _row_type(child_fields["kind"], child_number),
                        message_id,
                        _go_list(child_fields["go"], child_number),
                        _integer(child_fields["flag_off"], child_number, "flag_off"),
                        _integer(child_fields["flag_on"], child_number, "flag_on"),
                        _integer(child_fields["view"], child_number, "view"),
                        _integer(child_fields["ex"], child_number, "ex"),
                    )
                index += 1
            if index == len(meaningful):
                raise InfError(f"line {line_number}: set has no end")
            sets[row_index] = InteractionSet(
                Start(
                    _start_type(fields["kind"], line_number),
                    _integer(fields["area"], line_number, "area"),
                    _integer(fields["action"], line_number, "action"),
                    _integer(fields["event_hit"], line_number, "event_hit"),
                    fields["event"],
                ),
                tuple(flags),
                tuple(messages),
            )
        else:
            raise InfError(f"line {line_number}: unknown directive {directive!r}")
        index += 1

    inf = InfFile(tuple(packs), tuple(views), tuple(actions), tuple(sets))
    encode(inf)
    return inf


def _format_go(value: int) -> str:
    if 10 <= value < 10 + MESSAGE_COUNT:
        return f"row{value - 10}"
    if value == 100:
        return "warp"
    return str(value)


def _format_start_type(value: int) -> str:
    return {0: "npc", 1: "event"}.get(value, str(value))


def _format_row_type(value: int) -> str:
    return {-1: "action", 0: "message", 1: "selection"}.get(value, str(value))


def render_source(inf: InfFile, message_symbols: tuple[str | None, ...] = ()) -> str:
    """Render compact source relative to the canonical retail template."""

    encode(inf)
    lines = ["inf 1", ""]
    for index, pack in enumerate(inf.packs):
        if pack == DEFAULT_PACK:
            continue
        lines.append(f"pack {index} set={pack.set_index}")
        for hit_index, hit in enumerate(pack.hits):
            if hit != DEFAULT_HIT:
                lines.append(
                    f"  hit {hit_index} area={hit.area} event={json.dumps(hit.event)}"
                )
        lines.extend(("end", ""))
    for index, view in enumerate(inf.views):
        if view == DEFAULT_VIEW:
            continue
        lines.append(
            f"view {index} player={view.player_on} motion={view.player_motion} "
            f"position={json.dumps(view.player_position)} camera={json.dumps(view.camera)}"
        )
    if any(view != DEFAULT_VIEW for view in inf.views):
        lines.append("")
    for index, action in enumerate(inf.extra_actions):
        if action == DEFAULT_EXTRA_ACTION:
            continue
        parameters = ",".join(str(value) for value in action.parameters)
        lines.append(f"ex {index} id={action.action_id} params={parameters}")
    if any(action != DEFAULT_EXTRA_ACTION for action in inf.extra_actions):
        lines.append("")
    for set_index, interaction in enumerate(inf.sets):
        if interaction == DEFAULT_SET:
            continue
        start = interaction.start
        lines.append(
            f"set {set_index} kind={_format_start_type(start.type_id)} area={start.area} "
            f"action={start.action} event_hit={start.event_hit} "
            f"event={json.dumps(start.event)}"
        )
        for index, selector in enumerate(interaction.flags):
            if selector == DEFAULT_FLAG:
                continue
            lines.append(
                f"  flag {index} id={selector.flag_id} "
                f"off={_format_go(selector.go_off)} on={_format_go(selector.go_on)}"
            )
        for index, row in enumerate(interaction.messages):
            if row == DEFAULT_MESSAGE:
                continue
            message = str(row.message_id)
            if 0 <= row.message_id < len(message_symbols):
                symbol = message_symbols[row.message_id]
                if symbol is not None:
                    message = f"@{symbol}"
            go = ",".join(_format_go(value) for value in row.go)
            lines.append(
                f"  row {index} kind={_format_row_type(row.type_id)} message={message} go={go} "
                f"flag_off={row.flag_off} flag_on={row.flag_on} "
                f"view={row.view_id} ex={row.ex_id}"
            )
        lines.extend(("end", ""))
    return "\n".join(lines).rstrip() + "\n"


def load_message_symbols(path: Path) -> tuple[tuple[str | None, ...], dict[str, int]]:
    """Read unique source-safe MSG1 names from the paired BF source."""

    try:
        import flw0
        import msg1

        container = flw0.parse_source(path.read_text(encoding="utf-8"))
    except (OSError, ValueError) as exc:
        raise InfError(f"cannot read message symbols from {path}: {exc}") from exc
    banks = []
    for section in container.sections_of_type(3):
        payload = container.section_bytes(section)
        if not payload:
            continue
        try:
            banks.append(msg1.decode(payload))
        except msg1.Msg1Error:
            continue
    if len(banks) != 1:
        return (), {}
    counts: dict[str, int] = {}
    for dialog in banks[0].dialogs:
        counts[dialog.name] = counts.get(dialog.name, 0) + 1
    by_index = tuple(
        dialog.name
        if counts[dialog.name] == 1 and _SYMBOL.fullmatch(dialog.name)
        else None
        for dialog in banks[0].dialogs
    )
    return by_index, {
        symbol: index
        for index, symbol in enumerate(by_index)
        if symbol is not None
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    commands = parser.add_subparsers(dest="command", required=True)

    disassemble = commands.add_parser(
        "disassemble", help="write readable INF source"
    )
    disassemble.add_argument("input", type=Path)
    disassemble.add_argument("output", nargs="?", type=Path)
    disassemble.add_argument(
        "--messages", type=Path, help="paired BF source for symbolic message IDs"
    )

    assemble = commands.add_parser("assemble", help="assemble readable INF source")
    assemble.add_argument("input", type=Path)
    assemble.add_argument("output", type=Path)
    assemble.add_argument(
        "--messages", type=Path, help="paired BF source for symbolic message IDs"
    )

    verify = commands.add_parser("verify", help="validate an INF file and its exact rewrite")
    verify.add_argument("input", type=Path)
    args = parser.parse_args()

    try:
        if args.command == "verify":
            data = args.input.read_bytes()
            if encode(decode(data)) != data:
                raise InfError("decoded INF does not rewrite exactly")
            print(f"{args.input}: valid ({len(data):#x} bytes)")
            return 0

        by_index: tuple[str | None, ...] = ()
        by_name: dict[str, int] | None = None
        if args.messages is not None:
            by_index, by_name = load_message_symbols(args.messages)
        if args.command == "disassemble":
            source = render_source(decode(args.input.read_bytes()), by_index)
            if args.output is None:
                sys.stdout.write(source)
            else:
                args.output.parent.mkdir(parents=True, exist_ok=True)
                args.output.write_text(source, encoding="utf-8")
            return 0

        assert args.output is not None
        data = encode(parse_source(args.input.read_text(encoding="utf-8"), by_name))
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_bytes(data)
        return 0
    except (InfError, OSError) as exc:
        parser.error(str(exc))
    return 2


if __name__ == "__main__":
    raise SystemExit(main())
