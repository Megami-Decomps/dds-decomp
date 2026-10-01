#!/usr/bin/env python3
"""Editable semantic statements for symbolic DDS FLW0 source.

The semantic syntax is deliberately a layer over the VM instruction set.  Each
expression and statement has one deterministic lowering, while ordinary
assembly mnemonics remain valid beside it.  This makes partial decompilation
useful: understood stretches become readable without guessing across an
unknown command or control-flow join.
"""

from __future__ import annotations

import math
import re
import struct
from dataclasses import dataclass
from typing import Callable, Iterable

import flw0
import flw0_profiles


_NUMBER = r"0[xX][0-9a-fA-F]+|\d+(?:\.\d*)?(?:[eE][+-]?\d+)?"
_TOKEN = re.compile(
    r"\s*(?:"
    rf"(?P<number>{_NUMBER})|"
    r"(?P<name>[A-Za-z_][A-Za-z0-9_]*)|"
    r"(?P<operator>==|!=|<=|>=|\|\||&&|[+\-*/!<>()\[\],=])"
    r")"
)
_LABEL = re.compile(r"([A-Za-z_][A-Za-z0-9_]*):\Z")
_ASSEMBLY = frozenset(
    (*flw0.OPCODE_NAMES, "WORD", "PUSHMSG", "PUSHSELECT", "PUSHEVENT", "PUSHPROC")
)
_VARIABLES = {
    "global_int": ("PUSHIX", "POPIX"),
    "global_float": ("PUSHIF", "POPFX"),
    "local_int": ("PUSHLIX", "POPLIX"),
    "local_float": ("PUSHLFX", "POPLFX"),
}
_BINARY = {
    "||": "OR",
    "&&": "AND",
    "==": "EQ",
    "!=": "NEQ",
    "<": "LT",
    ">": "GT",
    "<=": "LE",
    ">=": "GE",
    "+": "ADD",
    "-": "SUB",
    "*": "MUL",
    "/": "DIV",
}
_OPCODE_TO_BINARY = {
    flw0.OPCODE_IDS[mnemonic]: operator for operator, mnemonic in _BINARY.items()
}
_OPCODE_TO_STORE = {
    flw0.OPCODE_IDS["POPIX"]: "global_int",
    flw0.OPCODE_IDS["POPFX"]: "global_float",
    flw0.OPCODE_IDS["POPLIX"]: "local_int",
    flw0.OPCODE_IDS["POPLFX"]: "local_float",
}
_PRECEDENCE = {
    "||": 1,
    "&&": 2,
    "==": 3,
    "!=": 3,
    "<": 3,
    ">": 3,
    "<=": 3,
    ">=": 3,
    "+": 4,
    "-": 4,
    "*": 5,
    "/": 5,
}


class Expr:
    """A semantic value with a deterministic stack-machine lowering."""

    def lower(self) -> list[str]:
        raise NotImplementedError

    def render(self, parent_precedence: int = 0) -> str:
        raise NotImplementedError


@dataclass(frozen=True)
class Integer(Expr):
    value: int
    wide: bool = False

    def lower(self) -> list[str]:
        mnemonic = "PUSHI" if self.wide else "PUSHIS"
        value = self.value & 0xFFFFFFFF if self.wide else self.value
        return [f"{mnemonic} {value}"]

    def render(self, parent_precedence: int = 0) -> str:
        return f"int32({self.value})" if self.wide else str(self.value)


@dataclass(frozen=True)
class Float32(Expr):
    bits: int

    def lower(self) -> list[str]:
        return [f"PUSHF 0x{self.bits:08x}"]

    def render(self, parent_precedence: int = 0) -> str:
        value = struct.unpack("<f", struct.pack("<I", self.bits))[0]
        if math.isnan(value):
            readable = "nan"
        elif math.isinf(value):
            readable = "-inf" if value < 0 else "inf"
        else:
            readable = repr(value)
        return f"float32({readable}, bits=0x{self.bits:08x})"


@dataclass(frozen=True)
class Variable(Expr):
    kind: str
    index: int

    def lower(self) -> list[str]:
        return [f"{_VARIABLES[self.kind][0]} {self.index}"]

    def render(self, parent_precedence: int = 0) -> str:
        return f"{self.kind}[{self.index}]"


@dataclass(frozen=True)
class Result(Expr):
    def lower(self) -> list[str]:
        return ["PUSHREG"]

    def render(self, parent_precedence: int = 0) -> str:
        return "result"


@dataclass(frozen=True)
class Reference(Expr):
    kind: str
    symbol: str

    def lower(self) -> list[str]:
        mnemonic = {
            "message": "PUSHMSG",
            "selection": "PUSHSELECT",
            "event": "PUSHEVENT",
            "procedure": "PUSHPROC",
            "string": "PUSHTYPE5",
        }[self.kind]
        return [f"{mnemonic} {self.symbol}"]

    def render(self, parent_precedence: int = 0) -> str:
        return f"{self.kind}({self.symbol})"


@dataclass(frozen=True)
class Type5Offset(Expr):
    offset: int

    def lower(self) -> list[str]:
        return [f"PUSHTYPE5 0x{self.offset:04x}"]

    def render(self, parent_precedence: int = 0) -> str:
        return f"type5[0x{self.offset:04x}]"


@dataclass(frozen=True)
class Unary(Expr):
    operator: str
    operand: Expr

    def lower(self) -> list[str]:
        return [*self.operand.lower(), "MINUS" if self.operator == "-" else "NOT"]

    def render(self, parent_precedence: int = 0) -> str:
        if self.operator == "-" and isinstance(self.operand, Integer):
            # Keep a VM MINUS distinct from a signed PUSHIS literal.
            text = f"-({self.operand.render()})"
        else:
            text = f"{self.operator}{self.operand.render(6)}"
        return f"({text})" if parent_precedence > 6 else text


@dataclass(frozen=True)
class Binary(Expr):
    operator: str
    left: Expr
    right: Expr

    def lower(self) -> list[str]:
        # DDS binary opcodes pop their left operand first.
        return [*self.right.lower(), *self.left.lower(), _BINARY[self.operator]]

    def render(self, parent_precedence: int = 0) -> str:
        precedence = _PRECEDENCE[self.operator]
        text = (
            f"{self.left.render(precedence)} {self.operator} "
            f"{self.right.render(precedence + 1)}"
        )
        return f"({text})" if precedence < parent_precedence else text


@dataclass(frozen=True)
class NativeCall(Expr):
    name: str
    arguments: tuple[Expr, ...]
    writes_result: bool | None

    def lower_call(self) -> list[str]:
        lines: list[str] = []
        # Source arguments follow handler order; argument zero is VM stack top.
        for argument in reversed(self.arguments):
            lines.extend(argument.lower())
        lines.append(f"COMM {self.name}")
        return lines

    def lower(self) -> list[str]:
        if not self.writes_result:
            raise flw0.Flw0Error(f"{self.name} does not produce a value")
        return [*self.lower_call(), "PUSHREG"]

    def render(self, parent_precedence: int = 0) -> str:
        return f"{self.name}({', '.join(arg.render() for arg in self.arguments)})"


class _ExpressionParser:
    def __init__(
        self,
        text: str,
        line_number: int,
        profile: flw0_profiles.CommandProfile | None,
    ) -> None:
        self.line_number = line_number
        self.profile = profile
        self.tokens = self._tokenize(text)
        self.index = 0

    def _tokenize(self, text: str) -> list[str]:
        tokens: list[str] = []
        cursor = 0
        while cursor < len(text):
            match = _TOKEN.match(text, cursor)
            if match is None:
                raise flw0.Flw0Error(
                    f"line {self.line_number}: invalid expression near {text[cursor:]!r}"
                )
            tokens.append(match.group(match.lastgroup))
            cursor = match.end()
        return tokens

    def peek(self) -> str | None:
        return self.tokens[self.index] if self.index < len(self.tokens) else None

    def take(self, expected: str | None = None) -> str:
        if self.index == len(self.tokens):
            wanted = f" {expected!r}" if expected else ""
            raise flw0.Flw0Error(
                f"line {self.line_number}: expected{wanted} before end of expression"
            )
        token = self.tokens[self.index]
        if expected is not None and token != expected:
            raise flw0.Flw0Error(
                f"line {self.line_number}: expected {expected!r}, found {token!r}"
            )
        self.index += 1
        return token

    def parse(self) -> Expr:
        expression = self.parse_binary(1)
        if self.peek() is not None:
            raise flw0.Flw0Error(
                f"line {self.line_number}: unexpected token {self.peek()!r}"
            )
        return expression

    def parse_binary(self, minimum: int) -> Expr:
        left = self.parse_unary()
        while (
            (operator := self.peek()) in _PRECEDENCE
            and _PRECEDENCE[operator] >= minimum
        ):
            precedence = _PRECEDENCE[self.take()]
            right = self.parse_binary(precedence + 1)
            left = Binary(operator, left, right)
        return left

    def parse_unary(self) -> Expr:
        if self.peek() == "-":
            self.take("-")
            literal = self.peek()
            if literal is not None and re.fullmatch(_NUMBER, literal):
                if any(marker in literal for marker in (".", "e", "E")):
                    self.take()
                    bits = struct.unpack("<I", struct.pack("<f", -float(literal)))[0]
                    return Float32(bits)
                if re.fullmatch(r"0[xX][0-9a-fA-F]+|\d+", literal):
                    return Integer(-int(self.take(), 0))
            return Unary("-", self.parse_unary())
        if self.peek() == "!":
            self.take("!")
            return Unary("!", self.parse_unary())
        return self.parse_primary()

    def parse_primary(self) -> Expr:
        token = self.take()
        if token == "(":
            expression = self.parse_binary(1)
            self.take(")")
            return expression
        if token[0].isdigit():
            if any(marker in token for marker in (".", "e", "E")):
                bits = struct.unpack("<I", struct.pack("<f", float(token)))[0]
                return Float32(bits)
            return Integer(int(token, 0))
        name = token
        if name == "result":
            return Result()
        if name in _VARIABLES or name == "type5":
            self.take("[")
            sign = 1
            if self.peek() == "-":
                self.take("-")
                sign = -1
            literal = self.take()
            if re.fullmatch(r"0[xX][0-9a-fA-F]+|\d+", literal) is None:
                raise flw0.Flw0Error(
                    f"line {self.line_number}: {name} index must be an integer"
                )
            value = int(literal, 0) * sign
            self.take("]")
            if name == "type5":
                if not 0 <= value <= 0xFFFF:
                    raise flw0.Flw0Error(
                        f"line {self.line_number}: type5 offset does not fit in 16 bits"
                    )
                return Type5Offset(value)
            if not -0x8000 <= value <= 0x7FFF:
                raise flw0.Flw0Error(
                    f"line {self.line_number}: variable index does not fit in 16 bits"
                )
            return Variable(name, value)
        if self.peek() != "(":
            if name == "nan":
                return Float32(0x7FC00000)
            if name == "inf":
                return Float32(0x7F800000)
            raise flw0.Flw0Error(
                f"line {self.line_number}: unknown value {name!r}"
            )
        self.take("(")
        if name in ("message", "selection", "event", "procedure", "string"):
            symbol = self.take()
            if not re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", symbol):
                raise flw0.Flw0Error(
                    f"line {self.line_number}: {name} takes one symbol"
                )
            self.take(")")
            return Reference(name, symbol)
        arguments: list[Expr] = []
        keyword_bits: int | None = None
        while self.peek() != ")":
            if name == "float32" and self.peek() == "bits":
                self.take("bits")
                self.take("=")
                keyword_bits = int(self.take(), 0)
            else:
                arguments.append(self.parse_binary(1))
            if self.peek() != ",":
                break
            self.take(",")
        self.take(")")
        if name == "int32":
            value = arguments[0] if len(arguments) == 1 else None
            if (
                isinstance(value, Unary)
                and value.operator == "-"
                and isinstance(value.operand, Integer)
            ):
                value = Integer(-value.operand.value)
            if not isinstance(value, Integer):
                raise flw0.Flw0Error(
                    f"line {self.line_number}: int32 takes one integer literal"
                )
            if not -(1 << 31) <= value.value < 1 << 32:
                raise flw0.Flw0Error(
                    f"line {self.line_number}: int32 value does not fit in 32 bits"
                )
            return Integer(value.value, wide=True)
        if name == "float32":
            if keyword_bits is not None:
                if not 0 <= keyword_bits <= 0xFFFFFFFF:
                    raise flw0.Flw0Error(
                        f"line {self.line_number}: float32 bits do not fit in 32 bits"
                    )
                return Float32(keyword_bits)
            if len(arguments) != 1 or not isinstance(arguments[0], (Integer, Float32)):
                raise flw0.Flw0Error(
                    f"line {self.line_number}: float32 needs a numeric value or bits="
                )
            if isinstance(arguments[0], Float32):
                return arguments[0]
            return Float32(
                struct.unpack("<I", struct.pack("<f", float(arguments[0].value)))[0]
            )
        command = self.profile.by_name.get(name) if self.profile is not None else None
        if command is None:
            raise flw0.Flw0Error(
                f"line {self.line_number}: unknown native command {name!r}"
            )
        if len(arguments) != command.stack_pop:
            raise flw0.Flw0Error(
                f"line {self.line_number}: {name} takes {command.stack_pop} arguments, "
                f"found {len(arguments)}"
            )
        return NativeCall(name, tuple(arguments), command.writes_result)


def parse_expression(
    text: str,
    line_number: int,
    profile: flw0_profiles.CommandProfile | None,
) -> Expr:
    return _ExpressionParser(text.strip(), line_number, profile).parse()


def _strip_statement(line: str) -> str:
    line = line.split("#", 1)[0].strip()
    return line[:-1].rstrip() if line.endswith(";") else line


def _assignment_target(text: str, line_number: int) -> tuple[str, int] | None:
    match = re.fullmatch(
        r"(global_int|global_float|local_int|local_float)\[(-?(?:0[xX][0-9a-fA-F]+|\d+))\]",
        text.strip(),
    )
    if match is None:
        return None
    index = int(match.group(2), 0)
    if not -0x8000 <= index <= 0x7FFF:
        raise flw0.Flw0Error(
            f"line {line_number}: variable index does not fit in a signed 16-bit field"
        )
    return match.group(1), index


def _lower_semantic_line(
    line: str,
    line_number: int,
    profile: flw0_profiles.CommandProfile | None,
) -> list[str] | None:
    text = _strip_statement(line)
    if not text:
        return []
    first = text.split(None, 1)[0].upper()
    if first in _ASSEMBLY or _LABEL.fullmatch(text):
        return None
    if text == "return":
        return ["END"]
    control_transfers = (("call ", "CALL"), ("jump ", "JUMP"), ("goto ", "GOTO"))
    for keyword, mnemonic in control_transfers:
        if text.startswith(keyword):
            symbol = text[len(keyword) :].strip()
            if not re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", symbol):
                raise flw0.Flw0Error(
                    f"line {line_number}: invalid target symbol {symbol!r}"
                )
            return [f"{mnemonic} {symbol}"]
    if text.startswith("push "):
        return parse_expression(text[5:], line_number, profile).lower()
    if text.startswith("if_not "):
        match = re.fullmatch(
            r"if_not\s*\((.*)\)\s+goto\s+([A-Za-z_][A-Za-z0-9_]*)", text
        )
        if match is None:
            raise flw0.Flw0Error(f"line {line_number}: invalid if_not statement")
        return [
            *parse_expression(match.group(1), line_number, profile).lower(),
            f"IF {match.group(2)}",
        ]
    assignment = re.fullmatch(
        r"(result|(?:global_int|global_float|local_int|local_float)"
        r"\[-?(?:0[xX][0-9a-fA-F]+|\d+)\])\s*=(?!=)\s*(.+)",
        text,
    )
    if assignment is not None:
        left, right = assignment.groups()
        expression = parse_expression(right, line_number, profile)
        if left == "result":
            if not isinstance(expression, NativeCall):
                raise flw0.Flw0Error(
                    f"line {line_number}: result can only name a native call"
                )
            command = profile.by_name[expression.name] if profile is not None else None
            if command is None or not command.writes_result:
                raise flw0.Flw0Error(
                    f"line {line_number}: {expression.name} does not write result"
                )
            return expression.lower_call()
        target = _assignment_target(left, line_number)
        if target is None:
            raise flw0.Flw0Error(
                f"line {line_number}: invalid assignment target {left!r}"
            )
        value_lines = expression.lower()
        return [*value_lines, f"{_VARIABLES[target[0]][1]} {target[1]}"]
    expression = parse_expression(text, line_number, profile)
    if not isinstance(expression, NativeCall):
        raise flw0.Flw0Error(
            f"line {line_number}: expression statement must be a native call"
        )
    return expression.lower_call()


@dataclass(frozen=True)
class _SourceLine:
    line_number: int
    text: str


@dataclass(frozen=True)
class _Structured:
    line_number: int
    kind: str
    condition: str
    body: tuple[object, ...]
    alternative: tuple[object, ...] | None = None


def _parse_blocks(content: list[tuple[int, str]]) -> tuple[object, ...]:
    def block(index: int, nested: bool) -> tuple[list[object], int, str | None]:
        nodes: list[object] = []
        while index < len(content):
            line_number, original = content[index]
            text = _strip_statement(original)
            if text == "}":
                if not nested:
                    raise flw0.Flw0Error(f"line {line_number}: unmatched '}}'")
                return nodes, index + 1, "end"
            if text == "} else {":
                if not nested:
                    raise flw0.Flw0Error(f"line {line_number}: unmatched else")
                return nodes, index + 1, "else"
            match = re.fullmatch(r"(if|while)\s*\((.*)\)\s*\{", text)
            if match is None:
                nodes.append(_SourceLine(line_number, original))
                index += 1
                continue
            body, index, terminator = block(index + 1, True)
            alternative = None
            if terminator == "else":
                if match.group(1) != "if":
                    raise flw0.Flw0Error(
                        f"line {line_number}: while cannot have an else block"
                    )
                alternative, index, terminator = block(index, True)
            if terminator != "end":
                raise flw0.Flw0Error(
                    f"line {line_number}: structured block has no closing '}}'"
                )
            nodes.append(
                _Structured(
                    line_number,
                    match.group(1),
                    match.group(2),
                    tuple(body),
                    tuple(alternative) if alternative is not None else None,
                )
            )
        if nested:
            return nodes, index, None
        return nodes, index, "end"

    nodes, _, _ = block(0, False)
    return tuple(nodes)


def _line_label(node: object) -> str | None:
    if not isinstance(node, _SourceLine):
        return None
    match = _LABEL.fullmatch(_strip_statement(node.text))
    return match.group(1) if match is not None else None


def _lower_nodes(
    nodes: tuple[object, ...],
    profile: flw0_profiles.CommandProfile | None,
) -> list[tuple[int, str]]:
    lowered: list[tuple[int, str]] = []
    for index, node in enumerate(nodes):
        if isinstance(node, _SourceLine):
            lines = _lower_semantic_line(node.text, node.line_number, profile)
            if lines is None:
                lowered.append((node.line_number, node.text))
            else:
                lowered.extend((node.line_number, line) for line in lines)
            continue

        assert isinstance(node, _Structured)
        next_label = _line_label(nodes[index + 1]) if index + 1 < len(nodes) else None
        condition = parse_expression(node.condition, node.line_number, profile)
        if node.kind == "while":
            start_label = _line_label(nodes[index - 1]) if index else None
            if start_label is None or next_label is None:
                raise flw0.Flw0Error(
                    f"line {node.line_number}: while needs labels immediately before "
                    "and after its block"
                )
            lowered.extend((node.line_number, line) for line in condition.lower())
            lowered.append((node.line_number, f"IF {next_label}"))
            lowered.extend(_lower_nodes(node.body, profile))
            lowered.append((node.line_number, f"GOTO {start_label}"))
            continue

        if next_label is None:
            raise flw0.Flw0Error(
                f"line {node.line_number}: if needs an end label immediately after its block"
            )
        false_label = next_label
        if node.alternative is not None:
            false_label = _line_label(node.alternative[0]) if node.alternative else None
            if false_label is None:
                raise flw0.Flw0Error(
                    f"line {node.line_number}: else needs its target label as its first line"
                )
        lowered.extend((node.line_number, line) for line in condition.lower())
        lowered.append((node.line_number, f"IF {false_label}"))
        lowered.extend(_lower_nodes(node.body, profile))
        if node.alternative is not None:
            lowered.append((node.line_number, f"GOTO {next_label}"))
            lowered.extend(_lower_nodes(node.alternative, profile))
    return lowered


def lower_code(
    content: Iterable[tuple[int, str]],
    profile: flw0_profiles.CommandProfile | None,
) -> list[tuple[int, str]]:
    """Lower semantic lines while leaving ordinary assembly lines untouched."""

    return _lower_nodes(_parse_blocks(list(content)), profile)


def _signed(value: int, bits: int) -> int:
    sign = 1 << (bits - 1)
    return value - (1 << bits) if value & sign else value


def literal_expression(
    opcode: int, operand: int, extended: int | None = None
) -> Expr | None:
    """Return the exact semantic expression for one value-producing opcode."""

    if opcode == flw0.OPCODE_IDS["PUSHI"] and extended is not None:
        return Integer(_signed(extended, 32), wide=True)
    if opcode == flw0.OPCODE_IDS["PUSHF"] and extended is not None:
        return Float32(extended)
    if opcode == flw0.OPCODE_IDS["PUSHIX"]:
        return Variable("global_int", _signed(operand, 16))
    if opcode == flw0.OPCODE_IDS["PUSHIF"]:
        return Variable("global_float", _signed(operand, 16))
    if opcode == flw0.OPCODE_IDS["PUSHLIX"]:
        return Variable("local_int", _signed(operand, 16))
    if opcode == flw0.OPCODE_IDS["PUSHLFX"]:
        return Variable("local_float", _signed(operand, 16))
    if opcode == flw0.OPCODE_IDS["PUSHIS"]:
        return Integer(_signed(operand, 16))
    if opcode == flw0.OPCODE_IDS["PUSHREG"]:
        return Result()
    if opcode == flw0.OPCODE_IDS["PUSHTYPE5"]:
        return Type5Offset(operand)
    return None


def render_code(
    words: tuple[flw0.InstructionWord, ...],
    symbols_at_pc: dict[int, list[str]],
    procedure_symbols: list[str],
    jump_symbols: list[str],
    profile: flw0_profiles.CommandProfile | None,
    string_symbols: dict[int, str],
    message_symbols: tuple[str | None, ...],
    selection_symbols: tuple[str | None, ...],
    raw_instruction: Callable[[int], tuple[str, int]],
) -> list[str]:
    """Render exact hybrid code, lifting only procedure-local linear idioms."""

    lines: list[str] = []
    stack: list[Expr] = []
    result_known = False

    def emit(text: str) -> None:
        lines.append(f"  {text}")

    def flush_stack() -> None:
        nonlocal stack
        for expression in stack:
            emit(f"push {expression.render()}")
        stack = []

    def raw(pc: int) -> int:
        nonlocal result_known
        flush_stack()
        instruction, next_pc = raw_instruction(pc)
        lines.append(instruction)
        result_known = False
        return next_pc

    def target(symbols: list[str], operand: int) -> str | None:
        return symbols[operand] if operand < len(symbols) else None

    pc = 0
    while pc < len(words):
        if pc in symbols_at_pc:
            flush_stack()
            stack = []
            result_known = False
            for symbol in symbols_at_pc[pc]:
                lines.append(f"{symbol}:")

        word = words[pc]
        opcode = word.opcode
        operand = word.operand_u16
        next_raw = words[pc + 1].raw if pc + 1 < len(words) else None

        if opcode in flw0._EXTENDED_OPCODES:
            if operand or pc + 1 >= len(words):
                pc = raw(pc)
                continue
            expression = literal_expression(opcode, operand, next_raw)
            if expression is None:
                pc = raw(pc)
                continue
            stack.append(expression)
            pc += 2
            continue

        if opcode == flw0.OPCODE_IDS["PUSHIS"]:
            expression: Expr | None = None
            message = flw0._message_push_symbol(
                word.raw, next_raw, profile, message_symbols
            )
            selection = flw0._selection_push_symbol(
                word.raw, next_raw, profile, selection_symbols
            )
            event = flw0._event_push_symbol(word.raw, next_raw, profile)
            procedure = flw0._procedure_push_symbol(
                word.raw, next_raw, profile, tuple(procedure_symbols)
            )
            if message is not None:
                expression = Reference("message", message)
            elif selection is not None:
                expression = Reference("selection", selection)
            elif event is not None:
                expression = Reference("event", event)
            elif procedure:
                expression = Reference("procedure", procedure)
            else:
                expression = literal_expression(opcode, operand)
            stack.append(expression)
            pc += 1
            continue

        if opcode == flw0.OPCODE_IDS["PUSHTYPE5"]:
            symbol = string_symbols.get(operand)
            stack.append(
                Reference("string", symbol) if symbol is not None else Type5Offset(operand)
            )
            pc += 1
            continue

        expression = literal_expression(opcode, operand)
        if expression is not None:
            if isinstance(expression, Result) and not result_known:
                pc = raw(pc)
                continue
            stack.append(expression)
            pc += 1
            continue

        if opcode in _OPCODE_TO_BINARY:
            if len(stack) < 2:
                pc = raw(pc)
                continue
            left = stack.pop()
            right = stack.pop()
            stack.append(Binary(_OPCODE_TO_BINARY[opcode], left, right))
            pc += 1
            continue

        if opcode in (flw0.OPCODE_IDS["MINUS"], flw0.OPCODE_IDS["NOT"]):
            if not stack:
                pc = raw(pc)
                continue
            operator = "-" if opcode == flw0.OPCODE_IDS["MINUS"] else "!"
            stack.append(Unary(operator, stack.pop()))
            pc += 1
            continue

        if opcode in _OPCODE_TO_STORE:
            if len(stack) != 1:
                pc = raw(pc)
                continue
            value = stack.pop()
            target_name = _OPCODE_TO_STORE[opcode]
            emit(f"{target_name}[{_signed(operand, 16)}] = {value.render()}")
            pc += 1
            continue

        if opcode == flw0.OPCODE_IDS["COMM"]:
            command = profile.by_id.get(operand) if profile is not None else None
            if command is None or len(stack) != command.stack_pop:
                pc = raw(pc)
                continue
            arguments = tuple(reversed(stack))
            stack = []
            call = NativeCall(command.name, arguments, command.writes_result)

            # The common call/PUSHREG/store sequence is one source assignment.
            store_pc = pc + 2
            if (
                command.writes_result
                and pc + 1 < len(words)
                and words[pc + 1].opcode == flw0.OPCODE_IDS["PUSHREG"]
                and store_pc < len(words)
                and words[store_pc].opcode in _OPCODE_TO_STORE
                and pc + 1 not in symbols_at_pc
                and store_pc not in symbols_at_pc
            ):
                store = words[store_pc]
                emit(
                    f"{_OPCODE_TO_STORE[store.opcode]}"
                    f"[{_signed(store.operand_u16, 16)}] = "
                    f"{call.render()}"
                )
                result_known = True
                pc += 3
                continue

            emit(("result = " if command.writes_result else "") + call.render())
            result_known = bool(command.writes_result) or (
                result_known and command.writes_result is False
            )
            pc += 1
            continue

        if opcode == flw0.OPCODE_IDS["IF"]:
            symbol = target(jump_symbols, operand)
            if len(stack) != 1 or symbol is None:
                pc = raw(pc)
                continue
            condition = stack.pop()
            emit(f"if_not ({condition.render()}) goto {symbol}")
            pc += 1
            continue

        if stack:
            flush_stack()
            result_known = False

        if opcode == flw0.OPCODE_IDS["CALL"]:
            symbol = target(procedure_symbols, operand)
            if symbol is None:
                pc = raw(pc)
            else:
                emit(f"call {symbol}")
                result_known = False
                pc += 1
            continue
        if opcode == flw0.OPCODE_IDS["JUMP"]:
            symbol = target(procedure_symbols, operand)
            if symbol is None:
                pc = raw(pc)
            else:
                emit(f"jump {symbol}")
                result_known = False
                pc += 1
            continue
        if opcode == flw0.OPCODE_IDS["GOTO"]:
            symbol = target(jump_symbols, operand)
            if symbol is None:
                pc = raw(pc)
            else:
                emit(f"goto {symbol}")
                result_known = False
                pc += 1
            continue
        if opcode == flw0.OPCODE_IDS["END"] and operand == 0:
            emit("return")
            result_known = False
            pc += 1
            continue
        pc = raw(pc)

    flush_stack()
    for symbol in symbols_at_pc.get(len(words), ()):
        lines.append(f"{symbol}:")
    return lines
