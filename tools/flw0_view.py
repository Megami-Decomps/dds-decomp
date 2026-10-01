"""Conservative, PC-anchored reading view for DDS BF/FLW0 scripts.

The view is derived from assembled FLW0 data. It is deliberately not source:
unknown native commands and ambiguous control-flow joins discard inferred
stack state instead of manufacturing expressions.
"""

from __future__ import annotations

import json
import math
import re
import struct
from collections import deque
from dataclasses import dataclass, field

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


@dataclass(frozen=True)
class _Value:
    """A display expression with an identity suitable for CFG joins."""

    text: str
    identity: tuple[object, ...]
    origins: frozenset[int] = field(default_factory=frozenset, compare=False)


@dataclass(frozen=True)
class _State:
    """Known stack suffix and VM result after joining incoming paths."""

    stack: tuple[_Value, ...] = ()
    result: _Value | None = None


@dataclass(frozen=True)
class _Region:
    kind: str
    start: int
    end: int
    condition_pc: int
    body_start: int
    false_start: int | None
    true_terminal: int
    false_terminal: int | None = None


@dataclass
class _Stack:
    """Mutable transfer-function view of a state's known stack suffix."""

    known: list[_Value]
    pc: int
    unknown_count: int = 0
    popped: list[_Value] = field(default_factory=list)

    def push(self, value: _Value) -> None:
        self.known.append(value)

    def pop(self) -> _Value:
        if self.known:
            value = self.known.pop()
        else:
            value = _Value("<?>", ("unknown_stack", self.pc, self.unknown_count))
            self.unknown_count += 1
        self.popped.append(value)
        return value

    def arguments(self, count: int) -> list[_Value]:
        return [self.pop() for _ in range(count)]


def _value(text: str, *identity: object, origin: int | None = None) -> _Value:
    origins = frozenset() if origin is None else frozenset((origin,))
    return _Value(text, tuple(identity), origins)


def _symbolize_argument(
    command: flw0_profiles.NativeCommand, index: int, value: _Value
) -> _Value:
    if len(value.identity) != 2 or value.identity[0] != "constant16":
        return value
    symbols = command.symbols_for_argument(index)
    integer = _signed(value.identity[1], 16)
    name = symbols.by_value.get(integer) if symbols is not None else None
    return _Value(name, value.identity, value.origins) if name is not None else value


def _merge_value_origins(left: _Value, right: _Value) -> _Value:
    return _Value(left.text, left.identity, left.origins | right.origins)


def _join_states(left: _State, right: _State) -> _State:
    """Keep only facts that agree at the top of both incoming stacks."""

    common = 0
    limit = min(len(left.stack), len(right.stack))
    while common < limit and left.stack[-1 - common] == right.stack[-1 - common]:
        common += 1
    if common:
        left_suffix = left.stack[len(left.stack) - common :]
        right_suffix = right.stack[len(right.stack) - common :]
        stack = tuple(
            _merge_value_origins(left_value, right_value)
            for left_value, right_value in zip(left_suffix, right_suffix)
        )
    else:
        stack = ()
    result = (
        _merge_value_origins(left.result, right.result)
        if left.result is not None and left.result == right.result
        else None
    )
    return _State(stack, result)


def _same_state_with_origins(left: _State, right: _State) -> bool:
    if left != right or len(left.stack) != len(right.stack):
        return False
    if any(
        left_value.origins != right_value.origins
        for left_value, right_value in zip(left.stack, right.stack)
    ):
        return False
    if left.result is None or right.result is None:
        return left.result is right.result
    return left.result.origins == right.result.origins


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
    semantic: bool = False,
    structured: bool = False,
) -> str:
    """Render a conservative instruction-level or semantic reading view."""

    semantic = semantic or structured
    try:
        profile = flw0_profiles.get(profile_name) if profile_name is not None else None
    except KeyError as exc:
        raise flw0.Flw0Error(f"unknown command profile {profile_name!r}") from exc
    commands = profile.by_id if profile is not None else {}
    procedures, labels, entries = _symbol_maps(script)
    procedure_symbols = tuple(
        procedures.get(index, "") for index in range(len(procedures))
    )
    procedure_pcs = {
        row.row_index: row.start_pc for row in script.named_rows(0)
    }
    label_pcs = {row.row_index: row.start_pc for row in script.named_rows(1)}
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

    def step(
        pc: int, state: _State
    ) -> tuple[int, _State, str, tuple[_Value, ...]]:
        """Apply one instruction's abstract stack/result transfer."""

        raw = words[pc].raw
        opcode = raw & 0xFFFF
        operand = raw >> 16
        stack = _Stack(list(state.stack), pc)
        result = state.result

        if opcode >= len(flw0.OPCODE_NAMES):
            statement = f"WORD 0x{raw:08x}  # unknown opcode and stack effect"
            return pc + 1, _State(), statement, ()

        name = flw0.OPCODE_NAMES[opcode]
        if opcode in flw0._EXTENDED_OPCODES:
            if operand or pc + 1 >= len(words):
                statement = f"WORD 0x{raw:08x}  # malformed {name}"
                return pc + 1, _State(), statement, ()
            bits = words[pc + 1].raw
            expression = (
                str(_signed(bits, 32))
                if opcode == flw0.OPCODE_IDS["PUSHI"]
                else _float_literal(bits)
            )
            stack.push(_value(expression, "constant32", opcode, bits, origin=pc))
            return (
                pc + 2,
                _State(tuple(stack.known), result),
                f"push {expression}",
                (),
            )

        if opcode == flw0.OPCODE_IDS["PUSHIX"]:
            expression = f"global_int[{_signed(operand, 16)}]"
            stack.push(_value(expression, "load", pc, origin=pc))
            statement = f"push {expression}"
        elif opcode == flw0.OPCODE_IDS["PUSHIF"]:
            expression = f"global_float[{_signed(operand, 16)}]"
            stack.push(_value(expression, "load", pc, origin=pc))
            statement = f"push {expression}"
        elif opcode == flw0.OPCODE_IDS["PUSHREG"]:
            expression = result or _value("result<?>", "unknown_result", pc)
            expression = _Value(
                expression.text, expression.identity, expression.origins | {pc}
            )
            stack.push(expression)
            statement = f"push {expression.text}"
        elif opcode == flw0.OPCODE_IDS["POPIX"]:
            statement = f"global_int[{_signed(operand, 16)}] = {stack.pop().text}"
        elif opcode == flw0.OPCODE_IDS["POPFX"]:
            statement = f"global_float[{_signed(operand, 16)}] = {stack.pop().text}"
        elif opcode == flw0.OPCODE_IDS["PROC"]:
            statement = f"procedure_marker {_target(procedures, operand, 'procedure')}"
        elif opcode == flw0.OPCODE_IDS["COMM"]:
            command = commands.get(operand)
            if command is None:
                statement = f"COMM 0x{operand:04x}  # unknown stack effect"
                return pc + 1, _State(), statement, ()
            arguments = [
                _symbolize_argument(command, index, value)
                for index, value in enumerate(stack.arguments(command.stack_pop))
            ]
            call = f"{command.name}({', '.join(value.text for value in arguments)})"
            if command.writes_result:
                statement = f"result = {call}"
                result = _value("result", "native_result", pc, origin=pc)
            else:
                statement = call
                if command.writes_result is None:
                    result = None
        elif opcode == flw0.OPCODE_IDS["END"]:
            statement = "return_or_end"
        elif opcode == flw0.OPCODE_IDS["JUMP"]:
            statement = f"jump_procedure {_target(procedures, operand, 'procedure')}"
        elif opcode == flw0.OPCODE_IDS["CALL"]:
            statement = f"call_procedure {_target(procedures, operand, 'procedure')}"
            return pc + 1, _State(), statement, ()
        elif opcode == flw0.OPCODE_IDS["RUN"]:
            statement = f"advance_pc  # RUN operand=0x{operand:04x}"
        elif opcode == flw0.OPCODE_IDS["GOTO"]:
            statement = f"goto {_target(labels, operand, 'jump_label')}"
        elif opcode in _BINARY_OPERATORS:
            left = stack.pop()
            right = stack.pop()
            expression = f"({left.text} {_BINARY_OPERATORS[opcode]} {right.text})"
            stack.push(
                _Value(
                    expression,
                    ("expression", pc, left, right),
                    left.origins | right.origins | {pc},
                )
            )
            statement = f"push {expression}"
        elif opcode == flw0.OPCODE_IDS["MINUS"]:
            argument = stack.pop()
            expression = f"-({argument.text})"
            stack.push(
                _Value(
                    expression,
                    ("expression", pc, argument),
                    argument.origins | {pc},
                )
            )
            statement = f"push {expression}"
        elif opcode == flw0.OPCODE_IDS["NOT"]:
            argument = stack.pop()
            expression = f"!({argument.text})"
            stack.push(
                _Value(
                    expression,
                    ("expression", pc, argument),
                    argument.origins | {pc},
                )
            )
            statement = f"push {expression}"
        elif opcode == flw0.OPCODE_IDS["IF"]:
            condition = stack.pop()
            statement = (
                f"if !({condition.text}) goto "
                f"{_target(labels, operand, 'jump_label')}"
            )
        elif opcode == flw0.OPCODE_IDS["PUSHIS"]:
            next_raw = words[pc + 1].raw if pc + 1 < len(words) else None
            message_symbol = flw0._message_push_symbol(
                raw, next_raw, profile, message_symbols
            )
            selection_symbol = flw0._selection_push_symbol(
                raw, next_raw, profile, selection_symbols
            )
            event_symbol = flw0._event_push_symbol(raw, next_raw, profile)
            procedure_symbol = flw0._procedure_push_symbol(
                raw, next_raw, profile, procedure_symbols
            )
            if message_symbol is not None:
                expression = f"message({message_symbol})"
                identity = ("message", operand)
            elif selection_symbol is not None:
                expression = f"selection({selection_symbol})"
                identity = ("selection", operand)
            elif event_symbol is not None:
                expression = f"event({event_symbol})"
                identity = ("event", operand)
            elif procedure_symbol:
                expression = f"procedure({procedure_symbol})"
                identity = ("procedure", operand)
            else:
                expression = str(_signed(operand, 16))
                identity = ("constant16", operand)
            stack.push(_Value(expression, identity, frozenset((pc,))))
            statement = f"push {expression}"
        elif opcode == flw0.OPCODE_IDS["PUSHLIX"]:
            expression = f"local_int[{_signed(operand, 16)}]"
            stack.push(_value(expression, "load", pc, origin=pc))
            statement = f"push {expression}"
        elif opcode == flw0.OPCODE_IDS["PUSHLFX"]:
            expression = f"local_float[{_signed(operand, 16)}]"
            stack.push(_value(expression, "load", pc, origin=pc))
            statement = f"push {expression}"
        elif opcode == flw0.OPCODE_IDS["POPLIX"]:
            statement = f"local_int[{_signed(operand, 16)}] = {stack.pop().text}"
        elif opcode == flw0.OPCODE_IDS["POPLFX"]:
            statement = f"local_float[{_signed(operand, 16)}] = {stack.pop().text}"
        elif opcode == flw0.OPCODE_IDS["PUSHTYPE5"]:
            if operand in strings:
                expression = (
                    f"type5_ref(0x{operand:04x}, {json.dumps(strings[operand])})"
                )
            else:
                expression = f"type5[0x{operand:04x}]"
            stack.push(_value(expression, "type5", operand, origin=pc))
            statement = f"push {expression}"
        else:
            statement = f"{name} 0x{operand:04x}  # semantics not lifted"
            return pc + 1, _State(), statement, ()

        return (
            pc + 1,
            _State(tuple(stack.known), result),
            statement,
            tuple(stack.popped),
        )

    instruction_pcs: list[int] = []
    next_pcs: dict[int, int] = {}
    pc = 0
    while pc < len(words):
        instruction_pcs.append(pc)
        next_pc, _, _, _ = step(pc, _State())
        next_pcs[pc] = next_pc
        pc = next_pc
    instruction_set = set(instruction_pcs)

    def successors(pc: int) -> tuple[int, ...]:
        raw = words[pc].raw
        opcode = raw & 0xFFFF
        operand = raw >> 16
        next_pc = next_pcs[pc]
        if opcode >= len(flw0.OPCODE_NAMES):
            return (next_pc,) if next_pc in instruction_set else ()
        if opcode in (flw0.OPCODE_IDS["END"], flw0.OPCODE_IDS["JUMP"]):
            return ()
        if opcode == flw0.OPCODE_IDS["GOTO"]:
            target = label_pcs.get(operand)
            return (target,) if target in instruction_set else ()
        if opcode == flw0.OPCODE_IDS["IF"]:
            targets = []
            if next_pc in instruction_set:
                targets.append(next_pc)
            target = label_pcs.get(operand)
            if target in instruction_set and target not in targets:
                targets.append(target)
            return tuple(targets)
        return (next_pc,) if next_pc in instruction_set else ()

    predecessor_sources: dict[int, list[int]] = {pc: [] for pc in instruction_pcs}
    for pc in instruction_pcs:
        for target in successors(pc):
            predecessor_sources[target].append(pc)

    roots = {
        target for target in procedure_pcs.values() if target in instruction_set
    }
    roots.update(pc for pc, sources in predecessor_sources.items() if not sources)
    if instruction_pcs and not roots:
        roots.add(instruction_pcs[0])

    input_states: dict[int, _State] = {pc: _State() for pc in roots}
    worklist = deque(sorted(roots))
    while worklist:
        pc = worklist.popleft()
        _, output, _, _ = step(pc, input_states[pc])
        for target in successors(pc):
            old = input_states.get(target)
            merged = output if old is None else _join_states(old, output)
            if old is None or not _same_state_with_origins(old, merged):
                input_states[target] = merged
                worklist.append(target)

    rendered: dict[int, str] = {}
    conditions: dict[int, str] = {}
    consumed_origins: set[int] = set()
    for pc in instruction_pcs:
        _, _, statement, consumed = step(pc, input_states.get(pc, _State()))
        rendered[pc] = statement
        if words[pc].opcode == flw0.OPCODE_IDS["IF"] and consumed:
            conditions[pc] = consumed[0].text
        for value in consumed:
            consumed_origins.update(value.origins)

    pure_stack_opcodes = {
        flw0.OPCODE_IDS[name]
        for name in (
            "PUSHI",
            "PUSHF",
            "PUSHIX",
            "PUSHIF",
            "PUSHREG",
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
            "PUSHIS",
            "PUSHLIX",
            "PUSHLFX",
            "PUSHTYPE5",
        )
    }

    def is_suppressed(pc: int) -> bool:
        opcode = words[pc].opcode
        return semantic and (
            opcode == flw0.OPCODE_IDS["PROC"]
            or (opcode in pure_stack_opcodes and pc in consumed_origins)
        )

    positions = {pc: index for index, pc in enumerate(instruction_pcs)}
    procedure_starts = set(procedure_pcs.values())
    candidates: list[_Region] = []
    if structured:
        for condition_pc in conditions:
            raw = words[condition_pc]
            false_start = label_pcs.get(raw.operand_u16)
            if false_start not in positions or false_start <= condition_pc:
                continue
            false_index = positions[false_start]
            if false_index == 0:
                continue
            true_terminal = instruction_pcs[false_index - 1]
            terminal_word = words[true_terminal]
            if terminal_word.opcode != flw0.OPCODE_IDS["GOTO"]:
                continue
            terminal_target = label_pcs.get(terminal_word.operand_u16)
            body_start = next_pcs[condition_pc]
            if terminal_target == false_start:
                region = _Region(
                    "if",
                    condition_pc,
                    false_start,
                    condition_pc,
                    body_start,
                    None,
                    true_terminal,
                )
            elif terminal_target is not None and terminal_target > false_start:
                false_terminal = None
                end_index = positions.get(terminal_target)
                if end_index is not None and end_index > 0:
                    possible = instruction_pcs[end_index - 1]
                    possible_word = words[possible]
                    if (
                        possible >= false_start
                        and possible_word.opcode == flw0.OPCODE_IDS["GOTO"]
                        and label_pcs.get(possible_word.operand_u16)
                        == terminal_target
                    ):
                        false_terminal = possible
                region = _Region(
                    "ifelse",
                    condition_pc,
                    terminal_target,
                    condition_pc,
                    body_start,
                    false_start,
                    true_terminal,
                    false_terminal,
                )
            elif terminal_target is not None and terminal_target < condition_pc:
                condition_prefix = instruction_pcs[
                    positions[terminal_target] : positions[condition_pc]
                ]
                if any(not is_suppressed(pc) for pc in condition_prefix):
                    continue
                region = _Region(
                    "while",
                    terminal_target,
                    false_start,
                    condition_pc,
                    body_start,
                    None,
                    true_terminal,
                )
            else:
                continue

            if any(region.start < pc < region.end for pc in procedure_starts):
                continue
            has_external_entry = any(
                source < region.start or source >= region.end
                for pc in instruction_pcs
                if region.start < pc < region.end
                for source in predecessor_sources[pc]
            )
            crosses_ifelse_arms = False
            if region.kind == "ifelse" and region.false_start is not None:
                for pc in instruction_pcs:
                    if not region.body_start <= pc <= region.true_terminal:
                        continue
                    for target in successors(pc):
                        if region.false_start <= target < region.end:
                            crosses_ifelse_arms = True
                            break
                    if crosses_ifelse_arms:
                        break
            if not has_external_entry and not crosses_ifelse_arms:
                candidates.append(region)

    crossing: set[_Region] = set()
    for index, left in enumerate(candidates):
        for right in candidates[index + 1 :]:
            if (
                left.start < right.start < left.end < right.end
                or right.start < left.start < right.end < left.end
            ):
                crossing.update((left, right))
    regions = {
        region.start: region for region in candidates if region not in crossing
    }

    lines = [
        (
            "# FLW0 structured reading view (advisory; assemble the .bfasm source)"
            if structured
            else "# FLW0 semantic reading view (advisory; assemble the .bfasm source)"
            if semantic
            else "# FLW0 reading view (advisory; assemble the .bfasm source)"
        ),
        f"profile {profile.name}" if profile is not None else "profile none",
        "",
    ]

    def emit_entries(pc: int, indent: int, suppress: bool = False) -> None:
        if suppress or pc not in entries:
            return
        if pc:
            lines.append("")
        prefix = "  " * indent
        for kind, name in entries[pc]:
            lines.append(f"{prefix}{kind} {name} @ 0x{pc:04x}")

    def emit_range(
        start: int,
        end: int,
        indent: int = 0,
        suppress_first_entry: bool = False,
    ) -> None:
        index = positions.get(start, len(instruction_pcs))
        first = True
        while index < len(instruction_pcs):
            pc = instruction_pcs[index]
            if pc >= end:
                return
            region = regions.get(pc)
            if region is not None and region.end <= end:
                emit_entries(pc, indent, suppress_first_entry and first)
                prefix = "  " * (indent + 1)
                condition = conditions[region.condition_pc]
                keyword = "while" if region.kind == "while" else "if"
                lines.append(
                    f"{prefix}{region.condition_pc:04x}: {keyword} ({condition}) {{"
                )
                emit_range(
                    region.body_start,
                    region.true_terminal,
                    indent + 1,
                )
                if region.kind == "ifelse":
                    lines.append(
                        f"{prefix}{region.true_terminal:04x}: }} else {{"
                    )
                    false_end = region.false_terminal or region.end
                    emit_range(
                        region.false_start or false_end,
                        false_end,
                        indent + 1,
                        suppress_first_entry=True,
                    )
                    closing_pc = region.false_terminal or region.end
                else:
                    closing_pc = region.true_terminal
                lines.append(f"{prefix}{closing_pc:04x}: }}")
                index = positions[region.end]
                first = False
                continue

            emit_entries(pc, indent, suppress_first_entry and first)
            if not is_suppressed(pc):
                lines.append(f"{'  ' * (indent + 1)}{pc:04x}: {rendered[pc]}")
            index += 1
            first = False

    if instruction_pcs:
        emit_range(instruction_pcs[0], len(words))

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
