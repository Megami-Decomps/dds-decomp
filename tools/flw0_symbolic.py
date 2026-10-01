"""Symbolic, relayout-capable source for DDS BF/FLW0 scripts."""

from __future__ import annotations

import json
import re
import struct
from dataclasses import dataclass

import flw0
import flw0_profiles
import msg1


_SYMBOL = re.compile(r"[A-Za-z_][A-Za-z0-9_]*\Z")
_PROCEDURE_OPCODES = {7, 10, 11}
_JUMP_LABEL_OPCODES = {13, 28}
_COMMAND_OPCODE = flw0.OPCODE_IDS["COMM"]
_STANDARD_SECTIONS = ((0, 0x20), (1, 0x20), (2, 4), (3, 1), (4, 1))


@dataclass(frozen=True)
class Declaration:
    symbol: str
    name: str
    reserved: int
    line_number: int


@dataclass(frozen=True)
class SymbolReference:
    opcode: int
    symbol: str
    line_number: int


def _symbol(text: str, line_number: int) -> str:
    if not _SYMBOL.fullmatch(text):
        raise flw0.Flw0Error(f"line {line_number}: invalid symbol {text!r}")
    return text


def _unique_symbol(name: str, prefix: str, index: int, used: set[str]) -> str:
    candidate = re.sub(r"[^A-Za-z0-9_]", "_", name)
    if not candidate or candidate[0].isdigit():
        candidate = f"{prefix}_{index:04x}"
    if candidate in used:
        candidate = f"{prefix}_{index:04x}"
    suffix = 2
    base = candidate
    while candidate in used:
        candidate = f"{base}_{suffix}"
        suffix += 1
    used.add(candidate)
    return candidate


def _canonical_row(row: flw0.NamedRow) -> bool:
    if len(row.raw) != 0x20 or row.reserved is None:
        return False
    try:
        name = row.name.encode("ascii")
    except UnicodeEncodeError:
        return False
    if len(name) > 0x18:
        return False
    expected = name + bytes(0x18 - len(name)) + struct.pack(
        "<II", row.start_pc, row.reserved
    )
    return expected == row.raw


def _raw_block(payload: bytes) -> list[str]:
    if not payload:
        return []
    if not any(payload):
        return [f"  zero {len(payload)}"]
    return [
        f"  bytes {payload[start:start + 32].hex()}"
        for start in range(0, len(payload), 32)
    ]


def _get_profile(
    name: str | None, line_number: int | None = None
) -> flw0_profiles.CommandProfile | None:
    if name is None:
        return None
    try:
        return flw0_profiles.get(name)
    except KeyError as exc:
        prefix = f"line {line_number}: " if line_number is not None else ""
        raise flw0.Flw0Error(f"{prefix}unknown command profile {name!r}") from exc


def _require_standard_layout(script: flw0.Flw0File) -> None:
    shape = tuple((section.type_id, section.element_size) for section in script.sections)
    if shape != _STANDARD_SECTIONS:
        raise flw0.Flw0Error(
            "symbolic source requires the standard five DDS sections; use flw0 1"
        )
    cursor = script.table_end
    for section in script.sections:
        if section.offset != cursor:
            raise flw0.Flw0Error(
                f"section {section.index} has noncanonical offset 0x{section.offset:x}; "
                "use flw0 1"
            )
        cursor += section.logical_size
    if cursor != script.physical_size:
        raise flw0.Flw0Error("file has trailing or uncovered bytes; use flw0 1")
    if script.header.declared_size != script.sections[4].offset:
        raise flw0.Flw0Error(
            "declared size is not the string-section offset; use flw0 1"
        )


def _render_instruction(
    words: tuple[flw0.InstructionWord, ...],
    pc: int,
    procedure_symbols: list[str],
    jump_symbols: list[str],
    command_profile: flw0_profiles.CommandProfile | None,
    string_symbols: dict[int, str],
    message_symbols: tuple[str | None, ...],
) -> tuple[str, int]:
    raw = words[pc].raw
    opcode = raw & 0xFFFF
    operand = raw >> 16
    if opcode >= len(flw0.OPCODE_NAMES):
        return f"  WORD 0x{raw:08x}", pc + 1
    name = flw0.OPCODE_NAMES[opcode]
    message_symbol = flw0._message_push_symbol(
        raw,
        words[pc + 1].raw if pc + 1 < len(words) else None,
        command_profile,
        message_symbols,
    )
    if message_symbol is not None:
        return f"  PUSHMSG {message_symbol}", pc + 1
    if opcode in flw0._EXTENDED_OPCODES:
        if pc + 1 >= len(words) or operand:
            return f"  WORD 0x{raw:08x}", pc + 1
        return f"  {name} 0x{words[pc + 1].raw:08x}", pc + 2
    if opcode in flw0._NO_OPERAND_OPCODES:
        if operand:
            return f"  WORD 0x{raw:08x}", pc + 1
        return f"  {name}", pc + 1
    if opcode in _PROCEDURE_OPCODES and operand < len(procedure_symbols):
        return f"  {name} {procedure_symbols[operand]}", pc + 1
    if opcode in _JUMP_LABEL_OPCODES and operand < len(jump_symbols):
        return f"  {name} {jump_symbols[operand]}", pc + 1
    if opcode == flw0.OPCODE_IDS["PUSHTYPE5"] and operand in string_symbols:
        return f"  {name} {string_symbols[operand]}", pc + 1
    if opcode == _COMMAND_OPCODE and command_profile is not None:
        command = command_profile.by_id.get(operand)
        if command is not None:
            return f"  {name} {command.name}", pc + 1
    if opcode == 29:
        signed_operand = operand - 0x10000 if operand & 0x8000 else operand
        return f"  {name} {signed_operand}", pc + 1
    return f"  {name} 0x{operand:04x}", pc + 1


def render(script: flw0.Flw0File, profile_name: str | None = None) -> str:
    """Render standard-layout FLW0 as symbolic version-2 source."""

    _require_standard_layout(script)
    command_profile = _get_profile(profile_name)
    procedures = script.named_rows(0)
    jump_labels = script.named_rows(1)
    for row in (*procedures, *jump_labels):
        if not _canonical_row(row):
            raise flw0.Flw0Error(
                f"named row {row.section_index}:{row.row_index} is noncanonical; "
                "use flw0 1"
            )

    used: set[str] = set()
    procedure_symbols = [
        _unique_symbol(row.name, "proc", row.row_index, used) for row in procedures
    ]
    jump_symbols = [
        _unique_symbol(row.name, "label", row.row_index, used) for row in jump_labels
    ]
    string_lines, string_symbols = flw0._render_string_payload(
        script.section_bytes(script.sections[4]), flw0._type5_offsets(script), used
    )
    message_data = script.section_bytes(script.sections[3])
    message_symbols = flw0._message_symbols(message_data)[0]
    if message_data:
        try:
            message_lines = msg1.render(message_data)
        except msg1.Msg1Error:
            message_lines = ["messages", *_raw_block(message_data)]
    else:
        message_lines = ["messages"]
    symbols_at_pc: dict[int, list[str]] = {}
    for row, symbol in zip(procedures, procedure_symbols):
        symbols_at_pc.setdefault(row.start_pc, []).append(symbol)
    for row, symbol in zip(jump_labels, jump_symbols):
        symbols_at_pc.setdefault(row.start_pc, []).append(symbol)

    header = script.header
    lines = ["flw0 2"]
    if command_profile is not None:
        lines.append(f"profile {command_profile.name}")
    lines.extend(
        [
            "",
            (
                "header "
                f"word00=0x{header.word_00:08x} "
                f"word0c=0x{header.word_0c:08x} "
                f"word18=0x{header.word_18:08x} "
                f"word1c=0x{header.word_1c:08x}"
            ),
            f"locals int={header.int_local_count} float={header.float_local_count}",
            "",
        ]
    )
    for row, symbol in zip(procedures, procedure_symbols):
        name = f" name={json.dumps(row.name)}" if row.name != symbol else ""
        lines.append(
            f"procedure {symbol}{name} reserved=0x{row.reserved:08x}"
        )
    for row, symbol in zip(jump_labels, jump_symbols):
        name = f" name={json.dumps(row.name)}" if row.name != symbol else ""
        lines.append(
            f"jump_label {symbol}{name} reserved=0x{row.reserved:08x}"
        )

    lines.extend(("", "code"))
    words = script.code_words()
    pc = 0
    instruction_boundaries: set[int] = set()
    while pc < len(words):
        instruction_boundaries.add(pc)
        for symbol in symbols_at_pc.get(pc, ()):
            lines.append(f"{symbol}:")
        instruction, next_pc = _render_instruction(
            words,
            pc,
            procedure_symbols,
            jump_symbols,
            command_profile,
            string_symbols,
            message_symbols,
        )
        lines.append(instruction)
        pc = next_pc
    instruction_boundaries.add(pc)
    invalid_targets = sorted(set(symbols_at_pc) - instruction_boundaries)
    if invalid_targets:
        rendered = ", ".join(f"0x{target:x}" for target in invalid_targets)
        raise flw0.Flw0Error(
            f"table target is not an instruction boundary: {rendered}; use flw0 1"
        )
    for symbol in symbols_at_pc.get(pc, ()):
        lines.append(f"{symbol}:")
    lines.append("end")
    lines.append("")
    lines.extend(message_lines)
    lines.extend(("end", "", "strings"))
    lines.extend(string_lines)
    lines.append("end")
    return "\n".join(lines).rstrip() + "\n"


def _parse_declaration(tokens: list[str], line_number: int) -> Declaration:
    if len(tokens) < 2:
        raise flw0.Flw0Error(f"line {line_number}: missing declaration symbol")
    symbol = _symbol(tokens[1], line_number)
    values = flw0._key_values(tokens[2:], line_number)
    if values.keys() - {"name", "reserved"}:
        raise flw0.Flw0Error(f"line {line_number}: unknown declaration field")
    name = values.get("name", symbol)
    try:
        encoded = name.encode("ascii")
    except UnicodeEncodeError as exc:
        raise flw0.Flw0Error(f"line {line_number}: name is not ASCII") from exc
    if len(encoded) > 0x18:
        raise flw0.Flw0Error(f"line {line_number}: name is longer than 24 bytes")
    reserved = flw0._unsigned(values.get("reserved", "0"), line_number)
    return Declaration(symbol, name, reserved, line_number)


def _parse_raw_block(content: list[tuple[int, str]]) -> bytes:
    chunks: list[bytes] = []
    for line_number, line in content:
        tokens = flw0._tokens(line, line_number)
        if len(tokens) != 2 or tokens[0] not in ("bytes", "zero"):
            raise flw0.Flw0Error(
                f"line {line_number}: expected 'bytes HEX' or 'zero SIZE'"
            )
        if tokens[0] == "zero":
            chunks.append(bytes(flw0._unsigned(tokens[1], line_number)))
        else:
            try:
                chunks.append(bytes.fromhex(tokens[1]))
            except ValueError as exc:
                raise flw0.Flw0Error(
                    f"line {line_number}: invalid byte string"
                ) from exc
    return b"".join(chunks)


def _parse_code(
    content: list[tuple[int, str]],
    procedures: list[Declaration],
    jump_labels: list[Declaration],
    command_profile: flw0_profiles.CommandProfile | None,
    string_symbols: dict[str, int],
    message_symbols: dict[str, int],
) -> tuple[bytes, dict[str, int]]:
    words: list[int | SymbolReference] = []
    labels: dict[str, int] = {}
    for line_number, line in content:
        if line.endswith(":"):
            symbol = _symbol(line[:-1].strip(), line_number)
            if symbol in labels:
                raise flw0.Flw0Error(
                    f"line {line_number}: duplicate code label {symbol!r}"
                )
            labels[symbol] = len(words)
            continue

        tokens = flw0._tokens(line, line_number)
        if not tokens:
            continue
        mnemonic = tokens[0].upper()
        if mnemonic == "PUSHMSG":
            if len(tokens) != 2:
                raise flw0.Flw0Error(
                    f"line {line_number}: PUSHMSG takes one symbol"
                )
            symbol = tokens[1]
            if symbol not in message_symbols:
                raise flw0.Flw0Error(
                    f"line {line_number}: unknown message {symbol!r}"
                )
            words.append(
                (message_symbols[symbol] << 16) | flw0.OPCODE_IDS["PUSHIS"]
            )
            continue
        if mnemonic == "WORD":
            if len(tokens) != 2:
                raise flw0.Flw0Error(f"line {line_number}: WORD takes one value")
            words.append(flw0._unsigned(tokens[1], line_number))
            continue
        if mnemonic not in flw0.OPCODE_IDS:
            raise flw0.Flw0Error(
                f"line {line_number}: unknown mnemonic {mnemonic!r}"
            )
        opcode = flw0.OPCODE_IDS[mnemonic]
        if opcode in flw0._EXTENDED_OPCODES:
            if len(tokens) != 2:
                raise flw0.Flw0Error(
                    f"line {line_number}: {mnemonic} takes one value"
                )
            words.extend((opcode, flw0._unsigned(tokens[1], line_number)))
            continue
        if opcode in flw0._NO_OPERAND_OPCODES:
            if len(tokens) != 1:
                raise flw0.Flw0Error(
                    f"line {line_number}: {mnemonic} takes no value"
                )
            words.append(opcode)
            continue
        if len(tokens) != 2:
            raise flw0.Flw0Error(
                f"line {line_number}: {mnemonic} takes one value"
            )
        operand_text = tokens[1]
        if opcode == flw0.OPCODE_IDS["PUSHTYPE5"] and _SYMBOL.fullmatch(operand_text):
            if operand_text not in string_symbols:
                raise flw0.Flw0Error(
                    f"line {line_number}: unknown string {operand_text!r}"
                )
            offset = string_symbols[operand_text]
            if offset >= 1 << 16:
                raise flw0.Flw0Error(
                    f"line {line_number}: string offset does not fit in 16 bits"
                )
            words.append((offset << 16) | opcode)
            continue
        if opcode == _COMMAND_OPCODE and _SYMBOL.fullmatch(operand_text):
            if command_profile is None:
                raise flw0.Flw0Error(
                    f"line {line_number}: named COMM operand requires a profile"
                )
            command = command_profile.by_name.get(operand_text.upper())
            if command is None:
                raise flw0.Flw0Error(
                    f"line {line_number}: unknown {command_profile.name} "
                    f"command {operand_text!r}"
                )
            words.append((command.command_id << 16) | opcode)
            continue
        if opcode in _PROCEDURE_OPCODES | _JUMP_LABEL_OPCODES and _SYMBOL.fullmatch(
            operand_text
        ):
            words.append(SymbolReference(opcode, operand_text, line_number))
            continue
        if opcode == 29 and operand_text.startswith("-"):
            operand = flw0._signed_halfword(operand_text, line_number)
        else:
            operand = flw0._unsigned(operand_text, line_number, bits=16)
        words.append((operand << 16) | opcode)

    procedure_indices = {decl.symbol: index for index, decl in enumerate(procedures)}
    jump_indices = {decl.symbol: index for index, decl in enumerate(jump_labels)}
    resolved: list[int] = []
    for word in words:
        if isinstance(word, int):
            resolved.append(word)
            continue
        indices = (
            procedure_indices if word.opcode in _PROCEDURE_OPCODES else jump_indices
        )
        if word.symbol not in indices:
            kind = "procedure" if word.opcode in _PROCEDURE_OPCODES else "jump label"
            raise flw0.Flw0Error(
                f"line {word.line_number}: unknown {kind} {word.symbol!r}"
            )
        resolved.append((indices[word.symbol] << 16) | word.opcode)
    return b"".join(struct.pack("<I", word) for word in resolved), labels


def _named_payload(
    declarations: list[Declaration], labels: dict[str, int]
) -> bytes:
    rows: list[bytes] = []
    for declaration in declarations:
        if declaration.symbol not in labels:
            raise flw0.Flw0Error(
                f"line {declaration.line_number}: symbol "
                f"{declaration.symbol!r} has no code label"
            )
        name = declaration.name.encode("ascii")
        rows.append(
            name
            + bytes(0x18 - len(name))
            + struct.pack(
                "<II", labels[declaration.symbol], declaration.reserved
            )
        )
    return b"".join(rows)


def parse(text: str) -> flw0.Flw0File:
    """Assemble version-2 symbolic source and derive a canonical layout."""

    meaningful = [
        (number, line.strip())
        for number, line in enumerate(text.splitlines(), 1)
        if line.strip() and not line.lstrip().startswith("#")
    ]
    if not meaningful or meaningful[0][1] != "flw0 2":
        raise flw0.Flw0Error("symbolic source must begin with 'flw0 2'")

    header_values: dict[str, str] | None = None
    locals_values: dict[str, str] | None = None
    profile_record: tuple[str, int] | None = None
    procedures: list[Declaration] = []
    jump_labels: list[Declaration] = []
    blocks: dict[str, list[tuple[int, str]]] = {}
    messages_are_msg1 = False
    index = 1
    while index < len(meaningful):
        line_number, line = meaningful[index]
        tokens = flw0._tokens(line, line_number)
        directive = tokens[0]
        if directive == "profile":
            if len(tokens) != 2 or profile_record is not None:
                raise flw0.Flw0Error(f"line {line_number}: invalid profile")
            profile_record = (tokens[1], line_number)
            index += 1
            continue
        if directive == "header":
            if header_values is not None:
                raise flw0.Flw0Error(f"line {line_number}: duplicate header")
            header_values = flw0._key_values(tokens[1:], line_number)
            index += 1
            continue
        if directive == "locals":
            if locals_values is not None:
                raise flw0.Flw0Error(f"line {line_number}: duplicate locals")
            locals_values = flw0._key_values(tokens[1:], line_number)
            index += 1
            continue
        if directive in ("procedure", "jump_label"):
            declaration = _parse_declaration(tokens, line_number)
            target = procedures if directive == "procedure" else jump_labels
            target.append(declaration)
            index += 1
            continue
        if directive in ("code", "messages", "strings"):
            valid_header = len(tokens) == 1 or (
                directive == "messages" and tokens == ["messages", "msg1"]
            )
            if not valid_header or directive in blocks:
                raise flw0.Flw0Error(f"line {line_number}: invalid {directive} block")
            content: list[tuple[int, str]] = []
            index += 1
            while index < len(meaningful) and meaningful[index][1] != "end":
                content.append(meaningful[index])
                index += 1
            if index == len(meaningful):
                raise flw0.Flw0Error(f"line {line_number}: block has no 'end'")
            blocks[directive] = content
            if directive == "messages" and len(tokens) == 2:
                messages_are_msg1 = True
            index += 1
            continue
        raise flw0.Flw0Error(
            f"line {line_number}: unexpected directive {directive!r}"
        )

    if header_values is None or header_values.keys() != {
        "word00",
        "word0c",
        "word18",
        "word1c",
    }:
        raise flw0.Flw0Error("symbolic source header is incomplete or unknown")
    if locals_values is None or locals_values.keys() != {"int", "float"}:
        raise flw0.Flw0Error("symbolic source locals are incomplete or unknown")
    if blocks.keys() != {"code", "messages", "strings"}:
        raise flw0.Flw0Error("symbolic source needs code, messages, and strings blocks")

    declarations = procedures + jump_labels
    symbols = [declaration.symbol for declaration in declarations]
    if len(symbols) != len(set(symbols)):
        raise flw0.Flw0Error("procedure and jump-label symbols must be unique")

    command_profile = _get_profile(*profile_record) if profile_record else None
    string_data, string_symbols = flw0._parse_string_payload(blocks["strings"])
    if messages_are_msg1:
        try:
            message_data = msg1.parse_source(blocks["messages"])
        except msg1.Msg1Error as exc:
            raise flw0.Flw0Error(str(exc)) from exc
    else:
        message_data = _parse_raw_block(blocks["messages"])
    message_symbols = flw0._message_symbols(message_data)[1]
    code, labels = _parse_code(
        blocks["code"],
        procedures,
        jump_labels,
        command_profile,
        string_symbols,
        message_symbols,
    )
    procedure_data = _named_payload(procedures, labels)
    jump_label_data = _named_payload(jump_labels, labels)
    payloads = (
        procedure_data,
        jump_label_data,
        code,
        message_data,
        string_data,
    )

    sections: list[flw0.Section] = []
    offset = flw0.HEADER_SIZE + len(_STANDARD_SECTIONS) * flw0.SECTION_SIZE
    for index, ((type_id, stride), payload) in enumerate(
        zip(_STANDARD_SECTIONS, payloads)
    ):
        if len(payload) % stride:
            raise flw0.Flw0Error(
                f"section {index} payload size is not divisible by stride {stride}"
            )
        sections.append(
            flw0.Section(index, type_id, stride, len(payload) // stride, offset)
        )
        offset += len(payload)

    int_locals = flw0._signed_halfword(locals_values["int"], 0)
    float_locals = flw0._signed_halfword(locals_values["float"], 0)
    header = flw0.Header(
        flw0._unsigned(header_values["word00"], 0),
        sections[4].offset,
        b"FLW0",
        flw0._unsigned(header_values["word0c"], 0),
        len(sections),
        int_locals | (float_locals << 16),
        flw0._unsigned(header_values["word18"], 0),
        flw0._unsigned(header_values["word1c"], 0),
    )
    return flw0.parse(
        flw0.Flw0File(header, tuple(sections), b"".join(payloads)).to_bytes()
    )
