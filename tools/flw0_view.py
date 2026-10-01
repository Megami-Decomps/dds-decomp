"""Conservative, PC-anchored reading view for DDS BF/FLW0 scripts.

The view is derived from assembled FLW0 data.  It is deliberately not source:
unknown native commands and ambiguous control-flow entries discard inferred
stack state instead of manufacturing expressions.
"""

from __future__ import annotations

import json
import math
import re
import struct
from dataclasses import dataclass

import flw0
import flw0_profiles


_BINARY_OPERATORS = {
    flw0.OPCODE_IDS["ADD"]: "+",
    flw0.OPCODE_IDS["SUB"]: "-",
    flw0.OPCODE_IDS["MUL"]: "*",
    flw0.OPCODE_IDS["DIV"]: "/",
    flw0.OPCODE_IDS["OR"]: "||",
    flw0.OPCODE_IDS["AND"]: "&&",
    flw0.OPCODE_IDS["EQ"]: "==",
    flw0.OPCODE_IDS["NEQ"]: "!=",
    flw0.OPCODE_IDS["LT"]: "<",
    flw0.OPCODE_IDS["GT"]: ">",
    flw0.OPCODE_IDS["LE"]: "<=",
    flw0.OPCODE_IDS["GE"]: ">=",
}
_PROFILE_LINE = re.compile(r"^\s*profile\s+([A-Za-z0-9_-]+)\s*(?:#.*)?$")


@dataclass
class _Stack:
    """Known top-of-stack suffix after the most recent analysis barrier."""

    known: list[str]

    def reset_unknown(self) -> None:
        self.known.clear()

    def push(self, value: str) -> None:
        self.known.append(value)

    def pop(self) -> str:
        if self.known:
            return self.known.pop()
        return "<?>"

    def arguments(self, count: int) -> list[str]:
        return [self.pop() for _ in range(count)]


def _signed(value: int, bits: int) -> int:
    sign = 1 << (bits - 1)
    return value - (1 << bits) if value & sign else value


def _float_literal(bits: int) -> str:
    value = struct.unpack("<f", struct.pack("<I", bits))[0]
    if math.isnan(value):
        readable = "nan"
    elif math.isinf(value):
        readable = "-inf" if value < 0 else "inf"
    else:
        readable = repr(value)
    return f"float32({readable}, bits=0x{bits:08x})"


def _symbol_maps(
    script: flw0.Flw0File,
) -> tuple[dict[int, str], dict[int, str], dict[int, list[tuple[str, str]]]]:
    procedures = {row.row_index: row.name for row in script.named_rows(0)}
    labels = {row.row_index: row.name for row in script.named_rows(1)}
    entries: dict[int, list[tuple[str, str]]] = {}
    for row in script.named_rows(0):
        entries.setdefault(row.start_pc, []).append(("procedure", row.name))
    for row in script.named_rows(1):
        entries.setdefault(row.start_pc, []).append(("jump_label", row.name))
    return procedures, labels, entries


def _target(table: dict[int, str], operand: int, fallback: str) -> str:
    name = table.get(operand)
    return name if name is not None else f"{fallback}[0x{operand:04x}]"


def type5_strings(script: flw0.Flw0File) -> dict[int, str]:
    """Return observed NUL-delimited ASCII strings keyed by section-4 offset."""

    sections = script.sections_of_type(4)
    if len(sections) != 1:
        return {}
    section = sections[0]
    # The seven irregular DDS1 files continue their final type-4 payload past
    # the descriptor's nominal count. The physical source preserves those
    # bytes, and PUSHTYPE5 offsets address them normally.
    payload = script.to_bytes()[section.offset :]
    strings: dict[int, str] = {}
    cursor = 0
    while cursor < len(payload):
        while cursor < len(payload) and payload[cursor] == 0:
            cursor += 1
        if cursor == len(payload):
            break
        start = cursor
        end = payload.find(b"\0", start)
        if end < 0:
            end = len(payload)
        chunk = payload[start:end]
        try:
            strings[start] = chunk.decode("ascii")
        except UnicodeDecodeError:
            pass
        cursor = end + 1
    return strings


def source_profile_name(source: str) -> str | None:
    """Return the profile declared by symbolic source, if present."""

    for line in source.splitlines():
        match = _PROFILE_LINE.match(line)
        if match is not None:
            return match.group(1)
    return None


def render(
    script: flw0.Flw0File,
    profile_name: str | None = None,
) -> str:
    """Render a conservative reading view with one statement per instruction."""

    try:
        profile = flw0_profiles.get(profile_name) if profile_name is not None else None
    except KeyError as exc:
        raise flw0.Flw0Error(f"unknown command profile {profile_name!r}") from exc
    commands = profile.by_id if profile is not None else {}
    procedures, labels, entries = _symbol_maps(script)
    strings = type5_strings(script)
    words = script.code_words()
    message_sections = [
        section
        for section in script.sections_of_type(3)
        if section.element_size == 1
    ]
    message_symbols = (
        flw0._message_symbols(script.section_bytes(message_sections[0]))[0]
        if len(message_sections) == 1
        else ()
    )
    selection_symbols = (
        flw0._selection_symbols(script.section_bytes(message_sections[0]))[0]
        if len(message_sections) == 1
        else ()
    )
    stack = _Stack([])
    result_known = False

    lines = [
        "# FLW0 reading view (advisory; assemble the .bfasm source)",
        f"profile {profile.name}" if profile is not None else "profile none",
        "",
    ]
    pc = 0
    while pc < len(words):
        if pc in entries:
            if pc:
                lines.append("")
            for kind, name in entries[pc]:
                lines.append(f"{kind} {name} @ 0x{pc:04x}")
            stack.reset_unknown()
            result_known = False

        raw = words[pc].raw
        opcode = raw & 0xFFFF
        operand = raw >> 16
        prefix = f"  {pc:04x}: "

        if opcode >= len(flw0.OPCODE_NAMES):
            lines.append(f"{prefix}WORD 0x{raw:08x}  # unknown opcode and stack effect")
            stack.reset_unknown()
            result_known = False
            pc += 1
            continue

        name = flw0.OPCODE_NAMES[opcode]
        if opcode in flw0._EXTENDED_OPCODES:
            if operand or pc + 1 >= len(words):
                lines.append(f"{prefix}WORD 0x{raw:08x}  # malformed {name}")
                stack.reset_unknown()
                result_known = False
                pc += 1
                continue
            bits = words[pc + 1].raw
            expression = (
                str(_signed(bits, 32))
                if opcode == flw0.OPCODE_IDS["PUSHI"]
                else _float_literal(bits)
            )
            stack.push(expression)
            lines.append(f"{prefix}push {expression}")
            pc += 2
            continue

        if opcode == flw0.OPCODE_IDS["PUSHIX"]:
            expression = f"global_int[{_signed(operand, 16)}]"
            stack.push(expression)
            statement = f"push {expression}"
        elif opcode == flw0.OPCODE_IDS["PUSHIF"]:
            expression = f"global_float[{_signed(operand, 16)}]"
            stack.push(expression)
            statement = f"push {expression}"
        elif opcode == flw0.OPCODE_IDS["PUSHREG"]:
            expression = "result" if result_known else "result<?>"
            stack.push(expression)
            statement = f"push {expression}"
        elif opcode == flw0.OPCODE_IDS["POPIX"]:
            statement = f"global_int[{_signed(operand, 16)}] = {stack.pop()}"
        elif opcode == flw0.OPCODE_IDS["POPFX"]:
            statement = f"global_float[{_signed(operand, 16)}] = {stack.pop()}"
        elif opcode == flw0.OPCODE_IDS["PROC"]:
            statement = f"procedure_marker {_target(procedures, operand, 'procedure')}"
        elif opcode == flw0.OPCODE_IDS["COMM"]:
            command = commands.get(operand)
            if command is None:
                statement = f"COMM 0x{operand:04x}  # unknown stack effect"
                stack.reset_unknown()
                result_known = False
            else:
                arguments = stack.arguments(command.stack_pop)
                call = f"{command.name}({', '.join(arguments)})"
                if command.writes_result:
                    statement = f"result = {call}"
                    result_known = True
                else:
                    statement = call
                    if command.writes_result is None:
                        result_known = False
        elif opcode == flw0.OPCODE_IDS["END"]:
            statement = "return_or_end"
            stack.reset_unknown()
            result_known = False
        elif opcode == flw0.OPCODE_IDS["JUMP"]:
            statement = f"jump_procedure {_target(procedures, operand, 'procedure')}"
            stack.reset_unknown()
            result_known = False
        elif opcode == flw0.OPCODE_IDS["CALL"]:
            statement = f"call_procedure {_target(procedures, operand, 'procedure')}"
            stack.reset_unknown()
            result_known = False
        elif opcode == flw0.OPCODE_IDS["RUN"]:
            statement = f"advance_pc  # RUN operand=0x{operand:04x}"
        elif opcode == flw0.OPCODE_IDS["GOTO"]:
            statement = f"goto {_target(labels, operand, 'jump_label')}"
            stack.reset_unknown()
            result_known = False
        elif opcode in _BINARY_OPERATORS:
            left = stack.pop()
            right = stack.pop()
            expression = f"({left} {_BINARY_OPERATORS[opcode]} {right})"
            stack.push(expression)
            statement = f"push {expression}"
        elif opcode == flw0.OPCODE_IDS["MINUS"]:
            expression = f"-({stack.pop()})"
            stack.push(expression)
            statement = f"push {expression}"
        elif opcode == flw0.OPCODE_IDS["NOT"]:
            expression = f"!({stack.pop()})"
            stack.push(expression)
            statement = f"push {expression}"
        elif opcode == flw0.OPCODE_IDS["IF"]:
            condition = stack.pop()
            statement = f"if !({condition}) goto {_target(labels, operand, 'jump_label')}"
        elif opcode == flw0.OPCODE_IDS["PUSHIS"]:
            message_symbol = flw0._message_push_symbol(
                raw,
                words[pc + 1].raw if pc + 1 < len(words) else None,
                profile,
                message_symbols,
            )
            selection_symbol = flw0._selection_push_symbol(
                raw,
                words[pc + 1].raw if pc + 1 < len(words) else None,
                profile,
                selection_symbols,
            )
            event_symbol = flw0._event_push_symbol(
                raw,
                words[pc + 1].raw if pc + 1 < len(words) else None,
                profile,
            )
            procedure_symbol = flw0._procedure_push_symbol(
                raw,
                words[pc + 1].raw if pc + 1 < len(words) else None,
                profile,
                tuple(
                    procedures.get(index, "") for index in range(len(procedures))
                ),
            )
            if message_symbol is not None:
                expression = f"message({message_symbol})"
            elif selection_symbol is not None:
                expression = f"selection({selection_symbol})"
            elif event_symbol is not None:
                expression = f"event({event_symbol})"
            elif procedure_symbol:
                expression = f"procedure({procedure_symbol})"
            else:
                expression = str(_signed(operand, 16))
            stack.push(expression)
            statement = f"push {expression}"
        elif opcode == flw0.OPCODE_IDS["PUSHLIX"]:
            expression = f"local_int[{_signed(operand, 16)}]"
            stack.push(expression)
            statement = f"push {expression}"
        elif opcode == flw0.OPCODE_IDS["PUSHLFX"]:
            expression = f"local_float[{_signed(operand, 16)}]"
            stack.push(expression)
            statement = f"push {expression}"
        elif opcode == flw0.OPCODE_IDS["POPLIX"]:
            statement = f"local_int[{_signed(operand, 16)}] = {stack.pop()}"
        elif opcode == flw0.OPCODE_IDS["POPLFX"]:
            statement = f"local_float[{_signed(operand, 16)}] = {stack.pop()}"
        elif opcode == flw0.OPCODE_IDS["PUSHTYPE5"]:
            if operand in strings:
                expression = f"type5_ref(0x{operand:04x}, {json.dumps(strings[operand])})"
            else:
                expression = f"type5[0x{operand:04x}]"
            stack.push(expression)
            statement = f"push {expression}"
        else:
            statement = f"{name} 0x{operand:04x}  # semantics not lifted"
            stack.reset_unknown()
            result_known = False

        lines.append(prefix + statement)
        pc += 1

    message_size = sum(section.logical_size for section in script.sections_of_type(3))
    string_sections = script.sections_of_type(4)
    string_size = sum(section.logical_size for section in string_sections)
    string_summary = f"strings {string_size} bytes"
    if len(string_sections) == 1:
        physical_extent = script.physical_size - string_sections[0].offset
        if physical_extent != string_size:
            string_summary = (
                f"strings {string_size} descriptor bytes, "
                f"{physical_extent} physical bytes"
            )
    lines.extend(("", f"messages {message_size} bytes", string_summary))
    return "\n".join(lines) + "\n"
