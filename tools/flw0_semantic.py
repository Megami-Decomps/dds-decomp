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
class SymbolicInteger(Expr):
    name: str
    value: int

    def lower(self) -> list[str]:
        return [f"PUSHIS {self.value}"]

    def render(self, parent_precedence: int = 0) -> str:
        return self.name


@dataclass(frozen=True)
class _UnresolvedSymbol(Expr):
    name: str

    def lower(self) -> list[str]:
        raise AssertionError("unresolved command argument symbol")

    def render(self, parent_precedence: int = 0) -> str:
        return self.name


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


def _resolve_argument_symbols(
    expression: Expr,
    command: flw0_profiles.NativeCommand,
    argument_index: int,
    line_number: int,
) -> Expr:
    symbols = command.symbols_for_argument(argument_index)
    if isinstance(expression, _UnresolvedSymbol):
        value = symbols.by_name.get(expression.name) if symbols is not None else None
        if value is None:
            raise flw0.Flw0Error(
                f"line {line_number}: {expression.name!r} is not a symbolic value "
                f"for argument {argument_index} of {command.name}"
            )
        return SymbolicInteger(expression.name, value)
    if isinstance(expression, Unary):
        return Unary(
            expression.operator,
            _resolve_argument_symbols(
                expression.operand, command, argument_index, line_number
            ),
        )
    if isinstance(expression, Binary):
        return Binary(
            expression.operator,
            _resolve_argument_symbols(
                expression.left, command, argument_index, line_number
            ),
            _resolve_argument_symbols(
                expression.right, command, argument_index, line_number
            ),
        )
    return expression


def _find_unresolved_symbol(expression: Expr) -> str | None:
    if isinstance(expression, _UnresolvedSymbol):
        return expression.name
    if isinstance(expression, Unary):
        return _find_unresolved_symbol(expression.operand)
    if isinstance(expression, Binary):
        return _find_unresolved_symbol(expression.left) or _find_unresolved_symbol(
            expression.right
        )
    if isinstance(expression, NativeCall):
        for argument in expression.arguments:
            if symbol := _find_unresolved_symbol(argument):
                return symbol
    return None


def _symbolize_argument(
    expression: Expr,
    command: flw0_profiles.NativeCommand,
    argument_index: int,
) -> Expr:
    if not isinstance(expression, Integer) or expression.wide:
        return expression
    symbols = command.symbols_for_argument(argument_index)
    name = symbols.by_value.get(expression.value) if symbols is not None else None
    return SymbolicInteger(name, expression.value) if name is not None else expression


@dataclass(frozen=True)
class _Pending:
    expression: Expr
    start: int
    end: int


@dataclass(frozen=True)
class _Statement:
    start: int
    end: int
    text: str


@dataclass(frozen=True)
class _Region:
    kind: str
    start: int
    end: int
    condition_pc: int
    body_start: int
    false_start: int | None
    true_terminal: int


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
        if symbol := _find_unresolved_symbol(expression):
            raise flw0.Flw0Error(
                f"line {self.line_number}: unknown value {symbol!r}"
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
            return _UnresolvedSymbol(name)
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
        resolved = tuple(
            _resolve_argument_symbols(argument, command, index, self.line_number)
            for index, argument in enumerate(arguments)
        )
        return NativeCall(name, resolved, command.writes_result)


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
    following_label: str | None = None,
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
        next_label = (
            _line_label(nodes[index + 1])
            if index + 1 < len(nodes)
            else following_label
        )
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
            # A nested conditional at the end of an else arm may share the
            # enclosing join label, which appears after the outer block.
            lowered.extend(_lower_nodes(node.alternative, profile, next_label))
        else:
            # The retail compiler's canonical one-arm shape retains this
            # otherwise redundant jump to the join label.
            lowered.append((node.line_number, f"GOTO {next_label}"))
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


def _find_regions(
    words: tuple[flw0.InstructionWord, ...],
    instruction_pcs: list[int],
    next_pcs: dict[int, int],
    conditions: dict[int, _Pending],
    symbols_at_pc: dict[int, list[str]],
    procedure_symbols: list[str],
    jump_symbols: list[str],
) -> dict[int, _Region]:
    """Find canonical regions whose implicit lowering preserves every word."""

    positions = {pc: index for index, pc in enumerate(instruction_pcs)}
    instruction_set = set(instruction_pcs)
    symbol_pcs = {
        symbol: pc for pc, symbols in symbols_at_pc.items() for symbol in symbols
    }
    procedure_starts = {
        symbol_pcs[symbol]
        for symbol in procedure_symbols
        if symbol in symbol_pcs
    }
    label_pcs = {
        index: symbol_pcs[symbol]
        for index, symbol in enumerate(jump_symbols)
        if symbol in symbol_pcs
    }

    def successors(pc: int) -> tuple[int, ...]:
        word = words[pc]
        next_pc = next_pcs[pc]
        if word.opcode >= len(flw0.OPCODE_NAMES):
            return (next_pc,) if next_pc in instruction_set else ()
        if word.opcode in (flw0.OPCODE_IDS["END"], flw0.OPCODE_IDS["JUMP"]):
            return ()
        if word.opcode == flw0.OPCODE_IDS["GOTO"]:
            target = label_pcs.get(word.operand_u16)
            return (target,) if target in instruction_set else ()
        if word.opcode == flw0.OPCODE_IDS["IF"]:
            targets = []
            if next_pc in instruction_set:
                targets.append(next_pc)
            target = label_pcs.get(word.operand_u16)
            if target in instruction_set and target not in targets:
                targets.append(target)
            return tuple(targets)
        return (next_pc,) if next_pc in instruction_set else ()

    predecessors: dict[int, list[int]] = {pc: [] for pc in instruction_pcs}
    for pc in instruction_pcs:
        for target in successors(pc):
            predecessors[target].append(pc)

    def first_symbol(pc: int) -> str | None:
        symbols = symbols_at_pc.get(pc, ())
        return symbols[0] if symbols else None

    def last_symbol(pc: int) -> str | None:
        symbols = symbols_at_pc.get(pc, ())
        return symbols[-1] if symbols else None

    candidates: list[_Region] = []
    for condition_pc, condition in conditions.items():
        false_index_value = words[condition_pc].operand_u16
        false_start = label_pcs.get(false_index_value)
        if (
            false_start not in positions
            or false_start <= condition_pc
            or condition.start not in positions
        ):
            continue
        false_index = positions[false_start]
        if false_index == 0:
            continue
        true_terminal = instruction_pcs[false_index - 1]
        terminal = words[true_terminal]
        if terminal.opcode != flw0.OPCODE_IDS["GOTO"]:
            continue
        terminal_target = label_pcs.get(terminal.operand_u16)
        false_symbol = (
            jump_symbols[false_index_value]
            if false_index_value < len(jump_symbols)
            else None
        )
        terminal_symbol = (
            jump_symbols[terminal.operand_u16]
            if terminal.operand_u16 < len(jump_symbols)
            else None
        )

        body_start = next_pcs[condition_pc]
        if (
            terminal_target == false_start
            and false_symbol == terminal_symbol == first_symbol(false_start)
        ):
            region = _Region(
                "if",
                condition.start,
                false_start,
                condition_pc,
                body_start,
                None,
                true_terminal,
            )
        elif (
            terminal_target is not None
            and terminal_target > false_start
            and false_symbol == first_symbol(false_start)
            and terminal_symbol == first_symbol(terminal_target)
        ):
            region = _Region(
                "ifelse",
                condition.start,
                terminal_target,
                condition_pc,
                body_start,
                false_start,
                true_terminal,
            )
        elif (
            terminal_target is not None
            and terminal_target < condition_pc
            and condition.start == terminal_target
            and false_symbol == first_symbol(false_start)
            and terminal_symbol == last_symbol(terminal_target)
        ):
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

        if true_terminal in symbols_at_pc:
            continue
        if any(region.start < pc < region.end for pc in procedure_starts):
            continue
        has_external_entry = any(
            source < region.start or source >= region.end
            for pc in instruction_pcs
            if region.start < pc < region.end
            for source in predecessors[pc]
        )
        crosses_ifelse_arms = False
        if region.kind == "ifelse" and region.false_start is not None:
            for pc in instruction_pcs:
                if not region.body_start <= pc <= region.true_terminal:
                    continue
                if any(
                    region.false_start <= target < region.end
                    for target in successors(pc)
                ):
                    crosses_ifelse_arms = True
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
    return {
        region.start: region for region in candidates if region not in crossing
    }


def _format_statements(
    statements: list[_Statement],
    symbols_at_pc: dict[int, list[str]],
    code_end: int,
    regions: dict[int, _Region],
    conditions: dict[int, _Pending],
) -> list[str]:
    by_start = {statement.start: statement for statement in statements}
    lines: list[str] = []

    def emit_labels(pc: int, indent: int, suppress: bool = False) -> None:
        if suppress:
            return
        for symbol in symbols_at_pc.get(pc, ()):
            lines.append(f"{'  ' * indent}{symbol}:")

    def emit_range(
        start: int,
        end: int,
        indent: int = 0,
        suppress_first_labels: bool = False,
    ) -> None:
        pc = start
        first = True
        while pc < end:
            region = regions.get(pc)
            if region is not None and region.end <= end:
                emit_labels(pc, indent, suppress_first_labels and first)
                prefix = "  " * (indent + 1)
                keyword = "while" if region.kind == "while" else "if"
                condition = conditions[region.condition_pc].expression.render()
                lines.append(f"{prefix}{keyword} ({condition}) {{")
                emit_range(region.body_start, region.true_terminal, indent + 1)
                if region.kind == "ifelse":
                    lines.append(f"{prefix}}} else {{")
                    assert region.false_start is not None
                    emit_labels(region.false_start, indent + 1)
                    emit_range(
                        region.false_start,
                        region.end,
                        indent + 1,
                        suppress_first_labels=True,
                    )
                lines.append(f"{prefix}}}")
                pc = region.end
                first = False
                continue

            emit_labels(pc, indent, suppress_first_labels and first)
            statement = by_start.get(pc)
            if statement is None or statement.end > end:
                raise flw0.Flw0Error(
                    f"semantic renderer lost the instruction at code word 0x{pc:x}"
                )
            lines.append(f"{'  ' * (indent + 1)}{statement.text}")
            pc = statement.end
            first = False

    if statements:
        emit_range(statements[0].start, code_end)
    emit_labels(code_end, 0)
    return lines


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
    structured: bool = False,
) -> list[str]:
    """Render exact hybrid code, lifting only procedure-local proven idioms."""

    instruction_pcs: list[int] = []
    next_pcs: dict[int, int] = {}
    cursor = 0
    while cursor < len(words):
        instruction_pcs.append(cursor)
        _, next_pc = raw_instruction(cursor)
        next_pcs[cursor] = next_pc
        cursor = next_pc

    statements: list[_Statement] = []
    stack: list[_Pending] = []
    conditions: dict[int, _Pending] = {}
    result_known = False

    def emit(text: str, start: int, end: int) -> None:
        statements.append(_Statement(start, end, text))

    def flush_stack() -> None:
        nonlocal stack
        for pending in stack:
            emit(f"push {pending.expression.render()}", pending.start, pending.end)
        stack = []

    def raw(pc: int) -> int:
        nonlocal result_known
        flush_stack()
        instruction, next_pc = raw_instruction(pc)
        emit(instruction.strip(), pc, next_pc)
        result_known = False
        return next_pc

    def target(symbols: list[str], operand: int) -> str | None:
        return symbols[operand] if operand < len(symbols) else None

    pc = 0
    while pc < len(words):
        if pc in symbols_at_pc:
            flush_stack()
            result_known = False

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
            stack.append(_Pending(expression, pc, pc + 2))
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
            assert expression is not None
            stack.append(_Pending(expression, pc, pc + 1))
            pc += 1
            continue

        if opcode == flw0.OPCODE_IDS["PUSHTYPE5"]:
            symbol = string_symbols.get(operand)
            expression = (
                Reference("string", symbol)
                if symbol is not None
                else Type5Offset(operand)
            )
            stack.append(_Pending(expression, pc, pc + 1))
            pc += 1
            continue

        expression = literal_expression(opcode, operand)
        if expression is not None:
            if isinstance(expression, Result) and not result_known:
                pc = raw(pc)
                continue
            stack.append(_Pending(expression, pc, pc + 1))
            pc += 1
            continue

        if opcode in _OPCODE_TO_BINARY:
            if len(stack) < 2:
                pc = raw(pc)
                continue
            left = stack.pop()
            right = stack.pop()
            expression = Binary(
                _OPCODE_TO_BINARY[opcode], left.expression, right.expression
            )
            stack.append(_Pending(expression, min(left.start, right.start), pc + 1))
            pc += 1
            continue

        if opcode in (flw0.OPCODE_IDS["MINUS"], flw0.OPCODE_IDS["NOT"]):
            if not stack:
                pc = raw(pc)
                continue
            argument = stack.pop()
            operator = "-" if opcode == flw0.OPCODE_IDS["MINUS"] else "!"
            stack.append(
                _Pending(Unary(operator, argument.expression), argument.start, pc + 1)
            )
            pc += 1
            continue

        if opcode in _OPCODE_TO_STORE:
            if len(stack) != 1:
                pc = raw(pc)
                continue
            value = stack.pop()
            target_name = _OPCODE_TO_STORE[opcode]
            text = (
                f"{target_name}[{_signed(operand, 16)}] = "
                f"{value.expression.render()}"
            )
            emit(text, value.start, pc + 1)
            pc += 1
            continue

        if opcode == flw0.OPCODE_IDS["COMM"]:
            command = profile.by_id.get(operand) if profile is not None else None
            if command is None:
                pc = raw(pc)
                continue
            captures_result = (
                command.writes_result is True
                and pc + 1 < len(words)
                and words[pc + 1].opcode == flw0.OPCODE_IDS["PUSHREG"]
                and pc + 1 not in symbols_at_pc
            )
            if (
                len(stack) < command.stack_pop
                or (len(stack) != command.stack_pop and not captures_result)
            ):
                pc = raw(pc)
                continue
            argument_start = len(stack) - command.stack_pop
            argument_values = stack[argument_start:]
            arguments = tuple(
                _symbolize_argument(pending.expression, command, index)
                for index, pending in enumerate(reversed(argument_values))
            )
            call_start = min(
                (pending.start for pending in argument_values), default=pc
            )
            stack = stack[:argument_start]
            call = NativeCall(command.name, arguments, command.writes_result)

            if captures_result:
                stack.append(_Pending(call, call_start, pc + 2))
                result_known = True
                pc += 2
                continue

            assert not stack
            text = ("result = " if command.writes_result else "") + call.render()
            emit(text, call_start, pc + 1)
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
            condition = _Pending(condition.expression, condition.start, pc + 1)
            conditions[pc] = condition
            emit(
                f"if_not ({condition.expression.render()}) goto {symbol}",
                condition.start,
                pc + 1,
            )
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
                emit(f"call {symbol}", pc, pc + 1)
                result_known = False
                pc += 1
            continue
        if opcode == flw0.OPCODE_IDS["JUMP"]:
            symbol = target(procedure_symbols, operand)
            if symbol is None:
                pc = raw(pc)
            else:
                emit(f"jump {symbol}", pc, pc + 1)
                result_known = False
                pc += 1
            continue
        if opcode == flw0.OPCODE_IDS["GOTO"]:
            symbol = target(jump_symbols, operand)
            if symbol is None:
                pc = raw(pc)
            else:
                emit(f"goto {symbol}", pc, pc + 1)
                result_known = False
                pc += 1
            continue
        if opcode == flw0.OPCODE_IDS["END"] and operand == 0:
            emit("return", pc, pc + 1)
            result_known = False
            pc += 1
            continue
        pc = raw(pc)

    flush_stack()
    regions = (
        _find_regions(
            words,
            instruction_pcs,
            next_pcs,
            conditions,
            symbols_at_pc,
            procedure_symbols,
            jump_symbols,
        )
        if structured
        else {}
    )
    return _format_statements(
        statements, symbols_at_pc, len(words), regions, conditions
    )
