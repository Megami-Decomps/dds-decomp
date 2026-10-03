#!/usr/bin/env python3
"""Inspect, disassemble, and assemble DDS BF/FLW0 script containers.

Version-1 source preserves descriptor order, offsets, gaps, padding, and bytes
outside typed fields exactly. Version-2 source resolves symbols and derives a
canonical physical layout, so code and data can change size.
"""

from __future__ import annotations

import argparse
import json
import re
import shlex
import struct
import sys
from dataclasses import dataclass, replace
from pathlib import Path
from typing import Iterable

import flw0_profiles
import msg1


HEADER_SIZE = 0x20
SECTION_SIZE = 0x10
_HEADER = struct.Struct("<II4sIIIII")
_SECTION = struct.Struct("<IIII")

OPCODE_NAMES = (
    "PUSHI",
    "PUSHF",
    "PUSHIX",
    "PUSHIF",
    "PUSHREG",
    "POPIX",
    "POPFX",
    "PROC",
    "COMM",
    "END",
    "JUMP",
    "CALL",
    "RUN",
    "GOTO",
    "ADD",
    "SUB",
    "MUL",
    "DIV",
    "MINUS",
    "NOT",
    "OR",
    "AND",
    "EQ",
    "NEQ",
    "LT",
    "GT",
    "LE",
    "GE",
    "IF",
    "PUSHIS",
    "PUSHLIX",
    "PUSHLFX",
    "POPLIX",
    "POPLFX",
    "PUSHTYPE5",
)
OPCODE_IDS = {name: opcode for opcode, name in enumerate(OPCODE_NAMES)}
_EXTENDED_OPCODES = {0, 1}
_NO_OPERAND_OPCODES = {4, 9, *range(14, 28)}


class Flw0Error(ValueError):
    """The input is not a structurally valid FLW0 container."""


@dataclass(frozen=True)
class Header:
    word_00: int
    declared_size: int
    magic: bytes
    word_0c: int
    section_count: int
    locals_word: int
    word_18: int
    word_1c: int

    @property
    def int_local_count(self) -> int:
        value = self.locals_word & 0xFFFF
        return value - 0x10000 if value & 0x8000 else value

    @property
    def float_local_count(self) -> int:
        value = self.locals_word >> 16
        return value - 0x10000 if value & 0x8000 else value

    def to_bytes(self) -> bytes:
        try:
            return _HEADER.pack(
                self.word_00,
                self.declared_size,
                self.magic,
                self.word_0c,
                self.section_count,
                self.locals_word,
                self.word_18,
                self.word_1c,
            )
        except struct.error as exc:
            raise Flw0Error(f"header field is outside its encoded range: {exc}") from exc


@dataclass(frozen=True)
class Section:
    index: int
    type_id: int
    element_size: int
    element_count: int
    offset: int

    @property
    def logical_size(self) -> int:
        return self.element_size * self.element_count

    @property
    def logical_end(self) -> int:
        return self.offset + self.logical_size

    def to_bytes(self) -> bytes:
        try:
            return _SECTION.pack(
                self.type_id,
                self.element_size,
                self.element_count,
                self.offset,
            )
        except struct.error as exc:
            raise Flw0Error(
                f"section {self.index} field is outside its encoded range: {exc}"
            ) from exc


@dataclass(frozen=True)
class NamedRow:
    section_index: int
    row_index: int
    raw: bytes
    name: str
    start_pc: int
    reserved: int | None


@dataclass(frozen=True)
class InstructionWord:
    pc: int
    raw: int

    @property
    def opcode(self) -> int:
        return self.raw & 0xFFFF

    @property
    def operand_u16(self) -> int:
        return self.raw >> 16

    @property
    def operand_s16(self) -> int:
        value = self.operand_u16
        return value - 0x10000 if value & 0x8000 else value


@dataclass(frozen=True)
class LocalAlias:
    """A source-only name for one encoded local-variable slot."""

    name: str
    kind: str
    index: int


@dataclass(frozen=True)
class Flw0File:
    """A parsed container with enough information for an exact rewrite."""

    header: Header
    sections: tuple[Section, ...]
    body: bytes
    local_aliases: tuple[LocalAlias, ...] = ()

    @property
    def table_end(self) -> int:
        return HEADER_SIZE + len(self.sections) * SECTION_SIZE

    @property
    def physical_size(self) -> int:
        return self.table_end + len(self.body)

    def to_bytes(self) -> bytes:
        if self.header.section_count != len(self.sections):
            raise Flw0Error(
                "header section count does not match the descriptor list"
            )
        return b"".join(
            (
                self.header.to_bytes(),
                *(section.to_bytes() for section in self.sections),
                self.body,
            )
        )

    def sections_of_type(self, type_id: int) -> tuple[Section, ...]:
        return tuple(section for section in self.sections if section.type_id == type_id)

    def section_bytes(self, section: Section) -> bytes:
        if not section.logical_size:
            return b""
        start = section.offset - self.table_end
        end = start + section.logical_size
        if start < 0 or end > len(self.body):
            raise Flw0Error(f"section {section.index} is outside the physical file")
        return self.body[start:end]

    def named_rows(self, type_id: int) -> tuple[NamedRow, ...]:
        rows: list[NamedRow] = []
        for section in self.sections_of_type(type_id):
            if not section.element_count:
                continue
            if section.element_size < 0x1C:
                raise Flw0Error(
                    f"named section {section.index} has element size "
                    f"0x{section.element_size:x}, expected at least 0x1c"
                )
            payload = self.section_bytes(section)
            for row_index in range(section.element_count):
                start = row_index * section.element_size
                raw = payload[start : start + section.element_size]
                name = raw.split(b"\0", 1)[0].decode("ascii", errors="replace")
                start_pc = struct.unpack_from("<I", raw, 0x18)[0]
                reserved = (
                    struct.unpack_from("<I", raw, 0x1C)[0]
                    if len(raw) >= 0x20
                    else None
                )
                rows.append(
                    NamedRow(
                        section_index=section.index,
                        row_index=row_index,
                        raw=raw,
                        name=name,
                        start_pc=start_pc,
                        reserved=reserved,
                    )
                )
        return tuple(rows)

    def code_words(self) -> tuple[InstructionWord, ...]:
        sections = self.sections_of_type(2)
        if len(sections) != 1:
            raise Flw0Error(f"expected one code section, found {len(sections)}")
        section = sections[0]
        if section.element_size != 4:
            raise Flw0Error(
                f"code section element size is {section.element_size}, expected 4"
            )
        payload = self.section_bytes(section)
        return tuple(
            InstructionWord(pc, struct.unpack_from("<I", payload, pc * 4)[0])
            for pc in range(section.element_count)
        )

    def with_code_word(self, pc: int, value: int) -> "Flw0File":
        """Return a same-layout copy with one code word replaced."""

        if not 0 <= value <= 0xFFFFFFFF:
            raise Flw0Error("code word must fit in an unsigned 32-bit value")
        sections = self.sections_of_type(2)
        if len(sections) != 1:
            raise Flw0Error(f"expected one code section, found {len(sections)}")
        section = sections[0]
        if section.element_size != 4:
            raise Flw0Error(
                f"code section element size is {section.element_size}, expected 4"
            )
        if not 0 <= pc < section.element_count:
            raise Flw0Error(
                f"code word index {pc} is outside 0..{section.element_count - 1}"
            )
        body_offset = section.offset - self.table_end + pc * 4
        body = bytearray(self.body)
        struct.pack_into("<I", body, body_offset, value)
        return replace(self, body=bytes(body))


def parse(data: bytes) -> Flw0File:
    """Parse a structurally valid FLW0 container without normalizing it."""

    if len(data) < HEADER_SIZE:
        raise Flw0Error(
            f"file is 0x{len(data):x} bytes, smaller than the 0x20-byte header"
        )

    header = Header(*_HEADER.unpack_from(data))
    if header.magic != b"FLW0":
        raise Flw0Error(f"invalid magic {header.magic!r}, expected b'FLW0'")

    table_end = HEADER_SIZE + header.section_count * SECTION_SIZE
    if table_end > len(data):
        raise Flw0Error(
            f"section table ends at 0x{table_end:x}, past file size 0x{len(data):x}"
        )

    sections: list[Section] = []
    for index in range(header.section_count):
        row_offset = HEADER_SIZE + index * SECTION_SIZE
        type_id, element_size, element_count, offset = _SECTION.unpack_from(
            data, row_offset
        )
        section = Section(index, type_id, element_size, element_count, offset)
        if section.logical_size and offset < table_end:
            raise Flw0Error(
                f"section {index} starts at 0x{offset:x}, inside the header/table"
            )
        if section.logical_end > len(data):
            raise Flw0Error(
                f"section {index} ends at 0x{section.logical_end:x}, "
                f"past file size 0x{len(data):x}"
            )
        sections.append(section)

    return Flw0File(header, tuple(sections), data[table_end:])


def _canonical_named_row(raw: bytes) -> tuple[str, int, int, bytes] | None:
    if len(raw) < 0x20:
        return None
    name_field = raw[:0x18]
    name_bytes, separator, padding = name_field.partition(b"\0")
    if separator and any(padding):
        return None
    if not separator and len(name_bytes) != 0x18:
        return None
    if any(byte < 0x20 or byte > 0x7E for byte in name_bytes):
        return None
    name = name_bytes.decode("ascii")
    canonical = name_bytes + bytes(0x18 - len(name_bytes))
    if canonical != name_field:
        return None
    start_pc, reserved = struct.unpack_from("<II", raw, 0x18)
    return name, start_pc, reserved, raw[0x20:]


def _render_raw_payload(payload: bytes, indent: str = "  ") -> list[str]:
    if not payload:
        return []
    if not any(payload):
        return [f"{indent}zero {len(payload)}"]
    return [
        f"{indent}bytes {payload[start:start + 32].hex()}"
        for start in range(0, len(payload), 32)
    ]


def _type5_offsets(flw0: Flw0File) -> set[int]:
    return {
        word.operand_u16
        for word in flw0.code_words()
        if word.opcode == OPCODE_IDS["PUSHTYPE5"]
    }


def _type5_extent(flw0: Flw0File, section: Section) -> bytes:
    """Return section 4 plus any trailing bytes reached by PUSHTYPE5."""

    raw = flw0.to_bytes()[section.offset :]
    extent = section.logical_size
    for offset in _type5_offsets(flw0):
        if offset >= len(raw):
            continue
        terminator = raw.find(b"\0", offset)
        if terminator >= 0:
            extent = max(extent, terminator + 1)
    return raw[:extent]


def _string_symbol(value: str, offset: int, used: set[str]) -> str:
    base = re.sub(r"[^A-Za-z0-9_]", "_", value).strip("_")
    if not base:
        base = f"string_{offset:04x}"
    elif base[0].isdigit():
        base = f"str_{base}"
    candidate = base
    suffix = 2
    while candidate in used:
        candidate = f"{base}_{suffix}"
        suffix += 1
    used.add(candidate)
    return candidate


def _render_string_payload(
    payload: bytes,
    references: set[int],
    used_symbols: set[str] | None = None,
) -> tuple[list[str], dict[int, str]]:
    """Render exact bytes, naming referenced NUL-terminated ASCII strings."""

    used = used_symbols if used_symbols is not None else set()
    candidates: list[tuple[int, int, str, str]] = []
    previous_end = 0
    for offset in sorted(references):
        if offset >= len(payload) or (offset and payload[offset - 1] != 0):
            continue
        terminator = payload.find(b"\0", offset)
        if terminator < 0 or offset < previous_end:
            continue
        raw = payload[offset:terminator]
        if any(byte < 0x20 or byte > 0x7E for byte in raw):
            continue
        value = raw.decode("ascii")
        symbol = _string_symbol(value, offset, used)
        candidates.append((offset, terminator + 1, symbol, value))
        previous_end = terminator + 1

    lines: list[str] = []
    symbols: dict[int, str] = {}
    cursor = 0
    for offset, end, symbol, value in candidates:
        lines.extend(_render_raw_payload(payload[cursor:offset]))
        lines.append(f"  string {symbol} {json.dumps(value)}")
        symbols[offset] = symbol
        cursor = end
    lines.extend(_render_raw_payload(payload[cursor:]))
    return lines, symbols


def _parse_string_payload(
    content: list[tuple[int, str]], maximum_size: int | None = None
) -> tuple[bytes, dict[str, int]]:
    chunks: list[bytes] = []
    symbols: dict[str, int] = {}
    size = 0
    for line_number, line in content:
        tokens = _tokens(line, line_number)
        if not tokens:
            continue
        if tokens[0] == "string":
            match = re.fullmatch(r"string\s+(\S+)\s+(.+)", line)
            if match is None or not _symbolic_operand(match.group(1)):
                raise Flw0Error(f"line {line_number}: invalid string declaration")
            symbol = match.group(1)
            if symbol in symbols:
                raise Flw0Error(
                    f"line {line_number}: duplicate string symbol {symbol!r}"
                )
            try:
                value, literal_end = json.JSONDecoder().raw_decode(match.group(2))
            except json.JSONDecodeError as exc:
                raise Flw0Error(
                    f"line {line_number}: invalid JSON string literal"
                ) from exc
            remainder = match.group(2)[literal_end:].lstrip()
            if remainder and not remainder.startswith("#"):
                raise Flw0Error(
                    f"line {line_number}: unexpected text after string literal"
                )
            if not isinstance(value, str):
                raise Flw0Error(f"line {line_number}: string value must be text")
            try:
                encoded = value.encode("ascii")
            except UnicodeEncodeError as exc:
                raise Flw0Error(f"line {line_number}: string value is not ASCII") from exc
            if b"\0" in encoded:
                raise Flw0Error(f"line {line_number}: string value contains NUL")
            symbols[symbol] = size
            chunk = encoded + b"\0"
        elif len(tokens) == 2 and tokens[0] == "zero":
            chunk = bytes(_unsigned(tokens[1], line_number))
        elif len(tokens) == 2 and tokens[0] == "bytes":
            try:
                chunk = bytes.fromhex(tokens[1])
            except ValueError as exc:
                raise Flw0Error(f"line {line_number}: invalid byte string") from exc
        else:
            raise Flw0Error(
                f"line {line_number}: expected 'string NAME TEXT', "
                "'bytes HEX', or 'zero SIZE'"
            )
        if maximum_size is not None and size + len(chunk) > maximum_size:
            raise Flw0Error(f"line {line_number}: string data exceeds declared extent")
        chunks.append(chunk)
        size += len(chunk)
    return b"".join(chunks), symbols


def _symbolic_operand(text: str) -> bool:
    return bool(text) and (text[0].isalpha() or text[0] == "_") and all(
        character.isalnum() or character == "_" for character in text[1:]
    )


def _message_symbols(payload: bytes) -> tuple[tuple[str | None, ...], dict[str, int]]:
    """Return unambiguous source-safe MSG1 names by index and by name."""

    if not payload:
        return (), {}
    try:
        bank = msg1.decode(payload)
    except msg1.Msg1Error:
        return (), {}
    counts: dict[str, int] = {}
    for dialog in bank.dialogs:
        counts[dialog.name] = counts.get(dialog.name, 0) + 1
    by_index = tuple(
        dialog.name
        if counts[dialog.name] == 1 and _symbolic_operand(dialog.name)
        else None
        for dialog in bank.dialogs
    )
    return by_index, {
        symbol: index
        for index, symbol in enumerate(by_index)
        if symbol is not None
    }


def _selection_symbols(payload: bytes) -> tuple[tuple[str | None, ...], dict[str, int]]:
    """Return unambiguous source-safe MSG1 selection names."""

    if not payload:
        return (), {}
    try:
        bank = msg1.decode(payload)
    except msg1.Msg1Error:
        return (), {}
    counts: dict[str, int] = {}
    for dialog in bank.dialogs:
        counts[dialog.name] = counts.get(dialog.name, 0) + 1
    by_index = tuple(
        dialog.name
        if (
            isinstance(dialog, msg1.Selection)
            and counts[dialog.name] == 1
            and _symbolic_operand(dialog.name)
        )
        else None
        for dialog in bank.dialogs
    )
    return by_index, {
        symbol: index
        for index, symbol in enumerate(by_index)
        if symbol is not None
    }


def _message_push_symbol(
    raw: int,
    next_raw: int | None,
    command_profile: flw0_profiles.CommandProfile | None,
    message_symbols: tuple[str | None, ...] | None,
) -> str | None:
    if command_profile is None or message_symbols is None or next_raw is None:
        return None
    command = command_profile.by_name.get("MESSAGE_REQUEST_AND_POLL")
    if command is None:
        return None
    message_index = raw >> 16
    if (
        raw & 0xFFFF != OPCODE_IDS["PUSHIS"]
        or message_index >= len(message_symbols)
        or next_raw & 0xFFFF != OPCODE_IDS["COMM"]
        or next_raw >> 16 != command.command_id
    ):
        return None
    return message_symbols[message_index]


def _selection_push_symbol(
    raw: int,
    next_raw: int | None,
    command_profile: flw0_profiles.CommandProfile | None,
    selection_symbols: tuple[str | None, ...] | None,
) -> str | None:
    if command_profile is None or selection_symbols is None or next_raw is None:
        return None
    command = command_profile.by_name.get("MESSAGE_SELECTION_REQUEST_AND_POLL")
    if command is None:
        return None
    selection_index = raw >> 16
    if (
        raw & 0xFFFF != OPCODE_IDS["PUSHIS"]
        or selection_index >= len(selection_symbols)
        or next_raw & 0xFFFF != OPCODE_IDS["COMM"]
        or next_raw >> 16 != command.command_id
    ):
        return None
    return selection_symbols[selection_index]


def _event_push_symbol(
    raw: int,
    next_raw: int | None,
    command_profile: flw0_profiles.CommandProfile | None,
) -> str | None:
    if command_profile is None or next_raw is None:
        return None
    event_commands = {
        command.command_id
        for name in ("CALL_EVENT", "SUBMIT_EVENT")
        if (command := command_profile.by_name.get(name)) is not None
    }
    if not event_commands:
        return None
    event_id = raw >> 16
    if (
        raw & 0xFFFF != OPCODE_IDS["PUSHIS"]
        or next_raw & 0xFFFF != OPCODE_IDS["COMM"]
        or next_raw >> 16 not in event_commands
    ):
        return None
    return command_profile.events_by_id.get(event_id)


def _procedure_push_symbol(
    raw: int,
    next_raw: int | None,
    command_profile: flw0_profiles.CommandProfile | None,
    procedure_symbols: tuple[str, ...] | list[str] | None,
) -> str | None:
    """Resolve a literal procedure argument to CREATE_SCRIPT_TASK."""

    if command_profile is None or procedure_symbols is None or next_raw is None:
        return None
    command = command_profile.by_name.get("CREATE_SCRIPT_TASK")
    if command is None:
        return None
    procedure_index = raw >> 16
    if (
        raw & 0xFFFF != OPCODE_IDS["PUSHIS"]
        or procedure_index >= len(procedure_symbols)
        or next_raw & 0xFFFF != OPCODE_IDS["COMM"]
        or next_raw >> 16 != command.command_id
    ):
        return None
    return procedure_symbols[procedure_index]


def _render_code(
    flw0: Flw0File,
    section: Section,
    command_profile: flw0_profiles.CommandProfile | None = None,
    string_symbols: dict[int, str] | None = None,
    message_symbols: tuple[str | None, ...] | None = None,
    selection_symbols: tuple[str | None, ...] | None = None,
) -> list[str]:
    payload = flw0.section_bytes(section)
    words = [
        struct.unpack_from("<I", payload, pc * 4)[0]
        for pc in range(section.element_count)
    ]
    procedure_names = {row.row_index: row.name for row in flw0.named_rows(0)}
    label_names = {row.row_index: row.name for row in flw0.named_rows(1)}
    lines: list[str] = []
    pc = 0
    while pc < len(words):
        raw = words[pc]
        opcode = raw & 0xFFFF
        operand = raw >> 16
        if opcode >= len(OPCODE_NAMES):
            lines.append(f"  {pc:04x}: WORD 0x{raw:08x}")
            pc += 1
            continue
        name = OPCODE_NAMES[opcode]
        message_symbol = _message_push_symbol(
            raw,
            words[pc + 1] if pc + 1 < len(words) else None,
            command_profile,
            message_symbols,
        )
        if message_symbol is not None:
            lines.append(f"  {pc:04x}: PUSHMSG {message_symbol}")
            pc += 1
            continue
        selection_symbol = _selection_push_symbol(
            raw,
            words[pc + 1] if pc + 1 < len(words) else None,
            command_profile,
            selection_symbols,
        )
        if selection_symbol is not None:
            lines.append(f"  {pc:04x}: PUSHSELECT {selection_symbol}")
            pc += 1
            continue
        event_symbol = _event_push_symbol(
            raw,
            words[pc + 1] if pc + 1 < len(words) else None,
            command_profile,
        )
        if event_symbol is not None:
            lines.append(f"  {pc:04x}: PUSHEVENT {event_symbol}")
            pc += 1
            continue
        if opcode in _EXTENDED_OPCODES:
            if pc + 1 >= len(words) or operand:
                lines.append(f"  {pc:04x}: WORD 0x{raw:08x}")
                pc += 1
                continue
            lines.append(f"  {pc:04x}: {name} 0x{words[pc + 1]:08x}")
            pc += 2
            continue
        if opcode in _NO_OPERAND_OPCODES:
            if operand:
                lines.append(f"  {pc:04x}: WORD 0x{raw:08x}")
            else:
                lines.append(f"  {pc:04x}: {name}")
        else:
            if opcode == OPCODE_IDS["PUSHTYPE5"] and string_symbols is not None:
                symbol = string_symbols.get(operand)
                if symbol is not None:
                    lines.append(f"  {pc:04x}: {name} {symbol}")
                    pc += 1
                    continue
            if opcode == OPCODE_IDS["COMM"] and command_profile is not None:
                command = command_profile.by_id.get(operand)
                if command is not None:
                    lines.append(f"  {pc:04x}: {name} {command.name}")
                    pc += 1
                    continue
            target_name = None
            if opcode in (7, 10, 11):
                target_name = procedure_names.get(operand)
            elif opcode in (13, 28):
                target_name = label_names.get(operand)
            comment = f"  # {json.dumps(target_name)}" if target_name else ""
            lines.append(f"  {pc:04x}: {name} 0x{operand:04x}{comment}")
        pc += 1
    return lines


def render_source(flw0: Flw0File, profile_name: str | None = None) -> str:
    """Render a self-contained, byte-exact low-level source file."""

    command_profile = None
    if profile_name is not None:
        try:
            command_profile = flw0_profiles.get(profile_name)
        except KeyError as exc:
            raise Flw0Error(f"unknown command profile {profile_name!r}") from exc
    header = flw0.header
    lines = ["flw0 1"]
    if command_profile is not None:
        lines.append(f"profile {command_profile.name}")
    lines.extend(
        [
            "",
            (
                "header "
                f"word00=0x{header.word_00:08x} "
                f"declared_size=0x{header.declared_size:08x} "
                f"word0c=0x{header.word_0c:08x} "
                f"int_locals={header.int_local_count} "
                f"float_locals={header.float_local_count} "
                f"word18=0x{header.word_18:08x} "
                f"word1c=0x{header.word_1c:08x} "
                f"physical_size=0x{flw0.physical_size:x}"
            ),
            "",
        ]
    )

    string_rendering: tuple[int, int, list[str], dict[int, str]] | None = None
    string_sections = [
        section
        for section in flw0.sections
        if section.type_id == 4 and section.element_size == 1
    ]
    if len(string_sections) == 1:
        string_section = string_sections[0]
        string_payload = _type5_extent(flw0, string_section)
        string_lines, string_symbols = _render_string_payload(
            string_payload, _type5_offsets(flw0)
        )
        string_rendering = (
            string_section.index,
            len(string_payload),
            string_lines,
            string_symbols,
        )

    message_symbols: tuple[str | None, ...] | None = None
    selection_symbols: tuple[str | None, ...] | None = None
    message_sections = [
        section
        for section in flw0.sections
        if section.type_id == 3 and section.element_size == 1
    ]
    if len(message_sections) == 1:
        message_payload = flw0.section_bytes(message_sections[0])
        message_symbols = _message_symbols(message_payload)[0]
        selection_symbols = _selection_symbols(message_payload)[0]

    covered = bytearray(flw0.physical_size)
    covered[: flw0.table_end] = b"\x01" * flw0.table_end
    for section in flw0.sections:
        extent = ""
        if (
            string_rendering is not None
            and section.index == string_rendering[0]
            and string_rendering[1] != section.logical_size
        ):
            extent = f" extent=0x{string_rendering[1]:x}"
        lines.append(
            f"section {section.index} type={section.type_id} "
            f"stride=0x{section.element_size:x} count={section.element_count} "
            f"offset=0x{section.offset:x}{extent}"
        )
        payload = flw0.section_bytes(section)
        covered_size = section.logical_size
        if section.type_id in (0, 1):
            noun = "proc" if section.type_id == 0 else "label"
            for row_index in range(section.element_count):
                start = row_index * section.element_size
                row = payload[start : start + section.element_size]
                decoded = _canonical_named_row(row)
                if decoded is None:
                    lines.append(f"  row {row.hex()}")
                    continue
                name, start_pc, reserved, tail = decoded
                suffix = f" tail={tail.hex()}" if tail else ""
                lines.append(
                    f"  {noun} {json.dumps(name)} pc={start_pc} "
                    f"reserved=0x{reserved:08x}{suffix}"
                )
        elif section.type_id == 2 and section.element_size == 4:
            lines.extend(
                _render_code(
                    flw0,
                    section,
                    command_profile,
                    string_rendering[3] if string_rendering is not None else None,
                    message_symbols,
                    selection_symbols,
                )
            )
        elif section.type_id == 3 and section.element_size == 1 and payload:
            try:
                message_lines = msg1.render(payload)
            except msg1.Msg1Error:
                lines.extend(_render_raw_payload(payload))
            else:
                lines.append("  msg1")
                lines.extend(message_lines[1:])
        elif string_rendering is not None and section.index == string_rendering[0]:
            lines.extend(string_rendering[2])
            covered_size = string_rendering[1]
        else:
            lines.extend(_render_raw_payload(payload))
        lines.append("end")
        lines.append("")
        covered[section.offset : section.offset + covered_size] = b"\x01" * covered_size

    raw = flw0.to_bytes()
    cursor = flw0.table_end
    while cursor < flw0.physical_size:
        if covered[cursor]:
            cursor += 1
            continue
        end = cursor + 1
        while end < flw0.physical_size and not covered[end]:
            end += 1
        payload = raw[cursor:end]
        if not any(payload):
            lines.append(f"preserve offset=0x{cursor:x} zero={len(payload)}")
        else:
            for start in range(0, len(payload), 32):
                chunk = payload[start : start + 32]
                lines.append(
                    f"preserve offset=0x{cursor + start:x} bytes={chunk.hex()}"
                )
        cursor = end

    return "\n".join(lines).rstrip() + "\n"


def _key_values(tokens: list[str], line_number: int) -> dict[str, str]:
    values: dict[str, str] = {}
    for token in tokens:
        if "=" not in token:
            raise Flw0Error(f"line {line_number}: expected key=value, found {token!r}")
        key, value = token.split("=", 1)
        if not key or key in values:
            raise Flw0Error(f"line {line_number}: invalid or duplicate key {key!r}")
        values[key] = value
    return values


def _tokens(line: str, line_number: int) -> list[str]:
    try:
        return shlex.split(line, comments=True)
    except ValueError as exc:
        raise Flw0Error(f"line {line_number}: {exc}") from exc


def _integer(text: str, line_number: int) -> int:
    try:
        return int(text, 0)
    except ValueError as exc:
        raise Flw0Error(f"line {line_number}: invalid integer {text!r}") from exc


def _unsigned(text: str, line_number: int, bits: int = 32) -> int:
    value = _integer(text, line_number)
    if not 0 <= value < 1 << bits:
        raise Flw0Error(
            f"line {line_number}: value {text!r} does not fit in {bits} bits"
        )
    return value


def _signed_halfword(text: str, line_number: int) -> int:
    value = _integer(text, line_number)
    if not -0x8000 <= value <= 0x7FFF:
        raise Flw0Error(
            f"line {line_number}: value {text!r} does not fit in a signed 16-bit field"
        )
    return value & 0xFFFF


def _parse_named_payload(
    content: list[tuple[int, str]], section: Section
) -> bytes:
    expected_noun = "proc" if section.type_id == 0 else "label"
    rows: list[bytes] = []
    for line_number, line in content:
        tokens = _tokens(line, line_number)
        if not tokens:
            continue
        if tokens[0] == "row":
            if len(tokens) != 2:
                raise Flw0Error(f"line {line_number}: row takes one hex value")
            try:
                row = bytes.fromhex(tokens[1])
            except ValueError as exc:
                raise Flw0Error(f"line {line_number}: invalid row hex") from exc
        elif tokens[0] == expected_noun:
            if len(tokens) < 4:
                raise Flw0Error(
                    f"line {line_number}: {expected_noun} requires name, pc, and reserved"
                )
            try:
                name = tokens[1].encode("ascii")
            except UnicodeEncodeError as exc:
                raise Flw0Error(f"line {line_number}: name is not ASCII") from exc
            if len(name) > 0x18:
                raise Flw0Error(f"line {line_number}: name is longer than 24 bytes")
            values = _key_values(tokens[2:], line_number)
            required = {"pc", "reserved"}
            if not required <= values.keys() or values.keys() - {
                "pc",
                "reserved",
                "tail",
            }:
                raise Flw0Error(f"line {line_number}: invalid named-row fields")
            try:
                tail = bytes.fromhex(values.get("tail", ""))
            except ValueError as exc:
                raise Flw0Error(f"line {line_number}: invalid tail hex") from exc
            row = (
                name
                + bytes(0x18 - len(name))
                + struct.pack(
                    "<II",
                    _unsigned(values["pc"], line_number),
                    _unsigned(values["reserved"], line_number),
                )
                + tail
            )
        else:
            raise Flw0Error(
                f"line {line_number}: expected {expected_noun!r} or 'row'"
            )
        if len(row) != section.element_size:
            raise Flw0Error(
                f"line {line_number}: row has {len(row)} bytes, "
                f"expected {section.element_size}"
            )
        rows.append(row)
    if len(rows) != section.element_count:
        raise Flw0Error(
            f"section {section.index}: found {len(rows)} rows, "
            f"expected {section.element_count}"
        )
    return b"".join(rows)


def _parse_raw_payload(content: list[tuple[int, str]], section: Section) -> bytes:
    if content:
        first_number, first_line = content[0]
        if _tokens(first_line, first_number) == ["msg1"]:
            if section.type_id != 3 or section.element_size != 1:
                raise Flw0Error(
                    f"line {first_number}: msg1 is only valid in a byte-wide type-3 section"
                )
            try:
                payload = msg1.parse_source(content[1:])
            except msg1.Msg1Error as exc:
                raise Flw0Error(str(exc)) from exc
            if len(payload) != section.logical_size:
                raise Flw0Error(
                    f"section {section.index}: MSG1 payload has {len(payload)} bytes, "
                    f"expected {section.logical_size}"
                )
            return payload

    chunks: list[bytes] = []
    size = 0
    for line_number, line in content:
        tokens = _tokens(line, line_number)
        if not tokens:
            continue
        if len(tokens) != 2 or tokens[0] not in ("bytes", "zero"):
            raise Flw0Error(f"line {line_number}: expected 'bytes HEX' or 'zero SIZE'")
        if tokens[0] == "zero":
            count = _unsigned(tokens[1], line_number)
            if size + count > section.logical_size:
                raise Flw0Error(f"line {line_number}: zero fill exceeds section size")
            chunk = bytes(count)
        else:
            try:
                chunk = bytes.fromhex(tokens[1])
            except ValueError as exc:
                raise Flw0Error(f"line {line_number}: invalid byte string") from exc
            if size + len(chunk) > section.logical_size:
                raise Flw0Error(f"line {line_number}: byte string exceeds section size")
        chunks.append(chunk)
        size += len(chunk)
    payload = b"".join(chunks)
    if len(payload) != section.logical_size:
        raise Flw0Error(
            f"section {section.index}: payload has {len(payload)} bytes, "
            f"expected {section.logical_size}"
        )
    return payload


def _parse_code_payload(
    content: list[tuple[int, str]],
    section: Section,
    command_profile: flw0_profiles.CommandProfile | None = None,
    string_symbols: dict[str, int] | None = None,
    message_symbols: dict[str, int] | None = None,
    selection_symbols: dict[str, int] | None = None,
) -> bytes:
    words: list[int] = []
    for line_number, line in content:
        line = line.split("#", 1)[0].strip()
        if not line:
            continue
        prefix, separator, instruction = line.partition(":")
        if not separator:
            raise Flw0Error(f"line {line_number}: expected 'PC: INSTRUCTION'")
        try:
            pc = int(prefix.strip(), 16)
        except ValueError as exc:
            raise Flw0Error(f"line {line_number}: invalid code address") from exc
        if pc != len(words):
            raise Flw0Error(
                f"line {line_number}: address is {pc:04x}, expected {len(words):04x}"
            )
        tokens = instruction.split()
        if not tokens:
            raise Flw0Error(f"line {line_number}: missing instruction")
        mnemonic = tokens[0].upper()
        if mnemonic == "PUSHMSG":
            if len(tokens) != 2:
                raise Flw0Error(f"line {line_number}: PUSHMSG takes one symbol")
            symbol = tokens[1]
            if message_symbols is None or symbol not in message_symbols:
                raise Flw0Error(f"line {line_number}: unknown message {symbol!r}")
            words.append(
                (message_symbols[symbol] << 16) | OPCODE_IDS["PUSHIS"]
            )
            continue
        if mnemonic == "PUSHSELECT":
            if len(tokens) != 2:
                raise Flw0Error(f"line {line_number}: PUSHSELECT takes one symbol")
            symbol = tokens[1]
            if selection_symbols is None or symbol not in selection_symbols:
                raise Flw0Error(f"line {line_number}: unknown selection {symbol!r}")
            words.append(
                (selection_symbols[symbol] << 16) | OPCODE_IDS["PUSHIS"]
            )
            continue
        if mnemonic == "PUSHEVENT":
            if len(tokens) != 2:
                raise Flw0Error(f"line {line_number}: PUSHEVENT takes one symbol")
            if command_profile is None:
                raise Flw0Error(
                    f"line {line_number}: named event target requires a profile"
                )
            symbol = tokens[1]
            event_id = command_profile.events_by_name.get(symbol)
            if event_id is None:
                raise Flw0Error(
                    f"line {line_number}: unknown {command_profile.name} "
                    f"event target {symbol!r}"
                )
            words.append((event_id << 16) | OPCODE_IDS["PUSHIS"])
            continue
        if mnemonic == "WORD":
            if len(tokens) != 2:
                raise Flw0Error(f"line {line_number}: WORD takes one value")
            words.append(_unsigned(tokens[1], line_number))
            continue
        if mnemonic not in OPCODE_IDS:
            raise Flw0Error(f"line {line_number}: unknown mnemonic {mnemonic!r}")
        opcode = OPCODE_IDS[mnemonic]
        if opcode in _EXTENDED_OPCODES:
            if len(tokens) != 2:
                raise Flw0Error(f"line {line_number}: {mnemonic} takes one value")
            words.extend((opcode, _unsigned(tokens[1], line_number)))
        elif opcode in _NO_OPERAND_OPCODES:
            if len(tokens) != 1:
                raise Flw0Error(f"line {line_number}: {mnemonic} takes no value")
            words.append(opcode)
        else:
            if len(tokens) != 2:
                raise Flw0Error(f"line {line_number}: {mnemonic} takes one value")
            operand_text = tokens[1]
            if opcode == OPCODE_IDS["PUSHTYPE5"] and _symbolic_operand(operand_text):
                if string_symbols is None or operand_text not in string_symbols:
                    raise Flw0Error(
                        f"line {line_number}: unknown string {operand_text!r}"
                    )
                operand = string_symbols[operand_text]
                if operand >= 1 << 16:
                    raise Flw0Error(
                        f"line {line_number}: string offset does not fit in 16 bits"
                    )
            elif opcode == OPCODE_IDS["COMM"] and _symbolic_operand(operand_text):
                if command_profile is None:
                    raise Flw0Error(
                        f"line {line_number}: named COMM operand requires a profile"
                    )
                command = command_profile.by_name.get(operand_text.upper())
                if command is None:
                    raise Flw0Error(
                        f"line {line_number}: unknown {command_profile.name} "
                        f"command {operand_text!r}"
                    )
                operand = command.command_id
            elif opcode == 29 and operand_text.startswith("-"):
                operand = _signed_halfword(operand_text, line_number)
            else:
                operand = _unsigned(operand_text, line_number, bits=16)
            words.append((operand << 16) | opcode)
    if len(words) != section.element_count:
        raise Flw0Error(
            f"section {section.index}: code has {len(words)} words, "
            f"expected {section.element_count}"
        )
    try:
        return b"".join(struct.pack("<I", word) for word in words)
    except struct.error as exc:
        raise Flw0Error(f"section {section.index}: code word is outside u32") from exc


def parse_source(
    text: str, profile: flw0_profiles.CommandProfile | None = None
) -> Flw0File:
    """Assemble physical version-1 or symbolic version-2 source."""

    numbered = enumerate(text.splitlines(), 1)
    meaningful = [
        (number, line.strip())
        for number, line in numbered
        if line.strip() and not line.lstrip().startswith("#")
    ]
    if meaningful and meaningful[0][1] == "flw0 2":
        import flw0_symbolic

        return flw0_symbolic.parse(text, profile=profile)
    if not meaningful or meaningful[0][1] != "flw0 1":
        raise Flw0Error("source must begin with 'flw0 1' or 'flw0 2'")

    header_values: dict[str, str] | None = None
    profile_name: str | None = None
    profile_line = 0
    section_records: list[tuple[Section, list[tuple[int, str]]]] = []
    section_extents: dict[int, int] = {}
    preserves: list[tuple[int, bytes, int]] = []
    index = 1
    while index < len(meaningful):
        line_number, line = meaningful[index]
        tokens = _tokens(line, line_number)
        if not tokens:
            index += 1
            continue
        if tokens[0] == "header":
            if header_values is not None:
                raise Flw0Error(f"line {line_number}: duplicate header")
            header_values = _key_values(tokens[1:], line_number)
            index += 1
            continue
        if tokens[0] == "profile":
            if profile_name is not None or len(tokens) != 2:
                raise Flw0Error(f"line {line_number}: invalid or duplicate profile")
            profile_name = tokens[1]
            profile_line = line_number
            index += 1
            continue
        if tokens[0] == "section":
            if len(tokens) < 6:
                raise Flw0Error(f"line {line_number}: incomplete section")
            section_index = _unsigned(tokens[1], line_number)
            values = _key_values(tokens[2:], line_number)
            required_section_fields = {"type", "stride", "count", "offset"}
            if not required_section_fields <= values.keys() or values.keys() - (
                required_section_fields | {"extent"}
            ):
                raise Flw0Error(f"line {line_number}: invalid section fields")
            section = Section(
                section_index,
                _unsigned(values["type"], line_number),
                _unsigned(values["stride"], line_number),
                _unsigned(values["count"], line_number),
                _unsigned(values["offset"], line_number),
            )
            if "extent" in values:
                if section.type_id != 4 or section.element_size != 1:
                    raise Flw0Error(
                        f"line {line_number}: extent is only valid for a byte-wide "
                        "type-4 section"
                    )
                extent = _unsigned(values["extent"], line_number)
                if extent < section.logical_size:
                    raise Flw0Error(
                        f"line {line_number}: extent is smaller than the logical section"
                    )
                section_extents[section.index] = extent
            content: list[tuple[int, str]] = []
            index += 1
            while index < len(meaningful) and meaningful[index][1] != "end":
                content.append(meaningful[index])
                index += 1
            if index == len(meaningful):
                raise Flw0Error(f"line {line_number}: section has no 'end'")
            section_records.append((section, content))
            index += 1
            continue
        if tokens[0] == "preserve":
            values = _key_values(tokens[1:], line_number)
            if "offset" not in values or len(values) != 2:
                raise Flw0Error(f"line {line_number}: invalid preserve fields")
            offset = _unsigned(values["offset"], line_number)
            if "zero" in values:
                count = _unsigned(values["zero"], line_number)
                if header_values is not None and "physical_size" in header_values:
                    physical_size = _unsigned(header_values["physical_size"], line_number)
                    if count > physical_size:
                        raise Flw0Error(
                            f"line {line_number}: zero fill exceeds physical size"
                        )
                payload = bytes(count)
            elif "bytes" in values:
                try:
                    payload = bytes.fromhex(values["bytes"])
                except ValueError as exc:
                    raise Flw0Error(f"line {line_number}: invalid preserve bytes") from exc
            else:
                raise Flw0Error(f"line {line_number}: preserve needs zero or bytes")
            preserves.append((offset, payload, line_number))
            index += 1
            continue
        raise Flw0Error(f"line {line_number}: unexpected directive {tokens[0]!r}")

    if header_values is None:
        raise Flw0Error("source has no header")
    command_profile = profile
    if command_profile is not None and profile_name not in (None, command_profile.name):
        raise Flw0Error(
            f"line {profile_line}: source profile {profile_name!r} does not match "
            f"provided profile {command_profile.name!r}"
        )
    if command_profile is None and profile_name is not None:
        try:
            command_profile = flw0_profiles.get(profile_name)
        except KeyError as exc:
            raise Flw0Error(
                f"line {profile_line}: unknown command profile {profile_name!r}"
            ) from exc
    expected_header = {
        "word00",
        "declared_size",
        "word0c",
        "int_locals",
        "float_locals",
        "word18",
        "word1c",
        "physical_size",
    }
    if header_values.keys() != expected_header:
        raise Flw0Error("header fields are incomplete or unknown")
    if [section.index for section, _ in section_records] != list(
        range(len(section_records))
    ):
        raise Flw0Error("section indices must be contiguous and ordered from zero")

    physical_size = _unsigned(header_values["physical_size"], 0)
    table_end = HEADER_SIZE + len(section_records) * SECTION_SIZE
    if physical_size < table_end:
        raise Flw0Error("physical size is smaller than the header and section table")
    body = bytearray(physical_size - table_end)
    written = bytearray(physical_size - table_end)

    def place(offset: int, payload: bytes, context: str) -> None:
        if not payload:
            return
        start = offset - table_end
        end = start + len(payload)
        if start < 0 or end > len(body):
            raise Flw0Error(f"{context}: data lies outside the physical file")
        for relative, byte in enumerate(payload, start):
            if written[relative] and body[relative] != byte:
                absolute = relative + table_end
                raise Flw0Error(
                    f"{context}: overlapping data disagrees at 0x{absolute:x}"
                )
            body[relative] = byte
            written[relative] = 1

    sections = tuple(section for section, _ in section_records)
    string_payloads: dict[int, bytes] = {}
    string_symbols: dict[str, int] = {}
    type4_sections = [
        section
        for section, _ in section_records
        if section.type_id == 4 and section.element_size == 1
    ]
    for section, content in section_records:
        if section.type_id != 4 or section.element_size != 1:
            continue
        expected_extent = section_extents.get(section.index, section.logical_size)
        payload, symbols = _parse_string_payload(content, maximum_size=expected_extent)
        if len(payload) != expected_extent:
            raise Flw0Error(
                f"section {section.index}: string payload has {len(payload)} bytes, "
                f"expected {expected_extent}"
            )
        string_payloads[section.index] = payload
        string_symbols.update(symbols)
    if string_symbols and len(type4_sections) != 1:
        raise Flw0Error("named strings require exactly one type-4 section")

    message_payloads: dict[int, bytes] = {}
    message_symbols: dict[str, int] = {}
    selection_symbols: dict[str, int] = {}
    message_sections = [
        section
        for section, _ in section_records
        if section.type_id == 3 and section.element_size == 1
    ]
    for section, content in section_records:
        if section.type_id != 3 or section.element_size != 1 or not content:
            continue
        first_number, first_line = content[0]
        if _tokens(first_line, first_number) != ["msg1"]:
            continue
        payload = _parse_raw_payload(content, section)
        message_payloads[section.index] = payload
        message_symbols.update(_message_symbols(payload)[1])
        selection_symbols.update(_selection_symbols(payload)[1])
    if (message_symbols or selection_symbols) and len(message_sections) != 1:
        raise Flw0Error("named dialogs require exactly one type-3 section")

    for section, content in section_records:
        if section.type_id in (0, 1):
            payload = _parse_named_payload(content, section)
        elif section.type_id == 2 and section.element_size == 4:
            payload = _parse_code_payload(
                content,
                section,
                command_profile,
                string_symbols,
                message_symbols,
                selection_symbols,
            )
        elif section.index in message_payloads:
            payload = message_payloads[section.index]
        elif section.index in string_payloads:
            payload = string_payloads[section.index]
        else:
            payload = _parse_raw_payload(content, section)
        place(section.offset, payload, f"section {section.index}")
    for offset, payload, line_number in preserves:
        place(offset, payload, f"line {line_number}")
    if not all(written):
        first = written.index(0) + table_end
        raise Flw0Error(f"source does not define byte at 0x{first:x}")

    int_locals = _signed_halfword(header_values["int_locals"], 0)
    float_locals = _signed_halfword(header_values["float_locals"], 0)
    header = Header(
        _unsigned(header_values["word00"], 0),
        _unsigned(header_values["declared_size"], 0),
        b"FLW0",
        _unsigned(header_values["word0c"], 0),
        len(sections),
        int_locals | (float_locals << 16),
        _unsigned(header_values["word18"], 0),
        _unsigned(header_values["word1c"], 0),
    )
    return parse(Flw0File(header, sections, bytes(body)).to_bytes())


def inspect_record(flw0: Flw0File) -> dict[str, object]:
    """Return a JSON-friendly structural summary."""

    return {
        "physical_size": flw0.physical_size,
        "declared_size": flw0.header.declared_size,
        "int_local_count": flw0.header.int_local_count,
        "float_local_count": flw0.header.float_local_count,
        "sections": [
            {
                "index": section.index,
                "type": section.type_id,
                "element_size": section.element_size,
                "element_count": section.element_count,
                "offset": section.offset,
                "logical_size": section.logical_size,
            }
            for section in flw0.sections
        ],
        "procedures": [
            {
                "index": row.row_index,
                "name": row.name,
                "start_pc": row.start_pc,
                "reserved": row.reserved,
            }
            for row in flw0.named_rows(0)
        ],
        "labels": [
            {
                "index": row.row_index,
                "name": row.name,
                "start_pc": row.start_pc,
                "reserved": row.reserved,
            }
            for row in flw0.named_rows(1)
        ],
        "code_word_count": sum(
            section.element_count
            for section in flw0.sections_of_type(2)
            if section.element_size == 4
        ),
    }


def _parse_paths(paths: Iterable[Path]) -> int:
    failed = False
    for path in paths:
        try:
            original = path.read_bytes()
            rebuilt = parse(original).to_bytes()
            if rebuilt != original:
                raise Flw0Error("internal error: preserve-layout rewrite differed")
            print(f"{path}: exact ({len(original)} bytes)")
        except (OSError, Flw0Error) as exc:
            failed = True
            print(f"{path}: {exc}")
    return 1 if failed else 0


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    commands = parser.add_subparsers(dest="command", required=True)

    inspect_parser = commands.add_parser("inspect", help="print container metadata")
    inspect_parser.add_argument("path", type=Path)

    verify_parser = commands.add_parser(
        "verify", help="verify an exact preserve-layout rewrite"
    )
    verify_parser.add_argument("paths", nargs="+", type=Path)

    rewrite_parser = commands.add_parser(
        "rewrite", help="write an exact preserve-layout copy"
    )
    rewrite_parser.add_argument("input", type=Path)
    rewrite_parser.add_argument("output", type=Path)

    disassemble_parser = commands.add_parser(
        "disassemble", help="write physical or symbolic FLW0 source"
    )
    disassemble_parser.add_argument("input", type=Path)
    disassemble_parser.add_argument("output", nargs="?", type=Path)
    disassemble_parser.add_argument(
        "--symbolic",
        action="store_true",
        help="derive symbolic, relayout-capable version-2 source",
    )
    disassemble_parser.add_argument(
        "--profile",
        metavar="NAME",
        help="name native commands using a game-specific command profile",
    )
    disassemble_parser.add_argument(
        "--semantic",
        action="store_true",
        help="lift exact linear stack idioms into editable semantic statements",
    )
    disassemble_parser.add_argument(
        "--structured",
        action="store_true",
        help="also recover canonical branches and loops as editable blocks",
    )

    assemble_parser = commands.add_parser(
        "assemble", help="assemble physical or symbolic FLW0 source"
    )
    assemble_parser.add_argument("input", type=Path)
    assemble_parser.add_argument("output", type=Path)

    view_parser = commands.add_parser(
        "view", help="render a conservative, stack-aware reading view"
    )
    view_parser.add_argument("input", type=Path, help="physical or symbolic source")
    view_parser.add_argument("output", nargs="?", type=Path)
    view_parser.add_argument(
        "--profile",
        metavar="NAME",
        help="native-command profile used to lift known calls",
    )
    view_parser.add_argument(
        "--semantic",
        action="store_true",
        help="fold consumed stack operations into semantic statements",
    )
    view_parser.add_argument(
        "--structured",
        action="store_true",
        help="recover canonical branches and loops in the semantic view",
    )

    args = parser.parse_args()
    try:
        if args.command == "inspect":
            flw0 = parse(args.path.read_bytes())
            print(json.dumps(inspect_record(flw0), indent=2))
            return 0
        if args.command == "verify":
            return _parse_paths(args.paths)
        if args.command == "rewrite":
            args.output.write_bytes(parse(args.input.read_bytes()).to_bytes())
            return 0
        if args.command == "disassemble":
            script = parse(args.input.read_bytes())
            if args.symbolic:
                import flw0_symbolic

                source = flw0_symbolic.render(
                    script,
                    args.profile,
                    semantic=args.semantic or args.structured,
                    structured=args.structured,
                )
            else:
                if args.semantic or args.structured:
                    parser.error("--semantic and --structured require --symbolic")
                source = render_source(script, args.profile)
            if args.output is None:
                print(source, end="")
            else:
                args.output.write_text(source, encoding="utf-8")
            return 0
        if args.command == "assemble":
            source = args.input.read_text(encoding="utf-8")
            args.output.write_bytes(parse_source(source).to_bytes())
            return 0
        if args.command == "view":
            import flw0_view

            source = args.input.read_text(encoding="utf-8")
            profile_name = args.profile or flw0_view.source_profile_name(source)
            rendered = flw0_view.render(
                parse_source(source),
                profile_name,
                semantic=args.semantic,
                structured=args.structured,
            )
            if args.output is None:
                print(rendered, end="")
            else:
                args.output.write_text(rendered, encoding="utf-8")
            return 0
    except (OSError, Flw0Error) as exc:
        parser.error(str(exc))
    raise AssertionError("unreachable")


if __name__ == "__main__":
    # Keep the error and data types shared when the companion symbolic module
    # imports this file by its module name.
    sys.modules["flw0"] = sys.modules[__name__]
    raise SystemExit(main())
