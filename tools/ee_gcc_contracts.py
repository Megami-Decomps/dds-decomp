#!/usr/bin/env python3
"""Conservatively compare C function declarations with definitions.

This is intentionally a small source auditor, not a C parser. It only reports
contracts it can recognize in ordinary project-style declarations. Return
conflicts are annotated when a recognized caller directly discards the result.
"""

from __future__ import annotations

import argparse
import bisect
import json
import re
import sys
from dataclasses import dataclass
from pathlib import Path
from typing import AbstractSet, Iterable, Optional


TYPE_WORDS = {
    "void", "char", "short", "int", "long", "float", "double", "signed",
    "unsigned", "const", "volatile", "restrict", "_Bool", "struct", "union",
    "enum", "s8", "u8", "s16", "u16", "s32", "u32", "s64", "u64",
    "f32", "f64", "size_t", "uintptr_t", "intptr_t",
}
QUALIFIERS = {"const", "volatile", "restrict"}
STORAGE_WORDS = {"extern", "static", "inline", "__inline", "register", "auto"}
TYPE_ALIASES = {
    "s8": "signed char", "u8": "unsigned char",
    "s16": "signed short", "u16": "unsigned short",
    "s32": "int", "u32": "unsigned int",
    "s64": "long long int", "u64": "unsigned long long int",
    "f32": "float", "f64": "double",
}
TOKEN_RE = re.compile(r"[A-Za-z_$][A-Za-z0-9_$]*|\.\.\.|[^\s]")
KNOWN_BASES = TYPE_WORDS | {
    "float", "double", "signed char", "unsigned char", "short int", "unsigned short int",
    "long int", "unsigned long int", "long long int", "unsigned long long int",
    "int", "unsigned int", "function-pointer",
}


@dataclass(frozen=True)
class TypeShape:
    spelling: str
    pointer_depth: int
    base: str
    qualifiers: tuple[str, ...]


@dataclass(frozen=True)
class Signature:
    name: str
    return_type: TypeShape
    params: Optional[tuple[TypeShape, ...]]  # None means old-style unspecified args.
    varargs: bool
    line: int
    path: str
    kind: str
    is_static: bool


@dataclass(frozen=True)
class IgnoredResultCall:
    name: str
    caller: str
    line: int
    path: str
    explicit_void_cast: bool


def _mask_source(source: str) -> str:
    """Blank comments, strings, chars, and preprocessing lines, preserving lines."""
    out = list(source)
    i, n = 0, len(source)
    state = "normal"
    line_start = True
    while i < n:
        ch = source[i]
        nxt = source[i + 1] if i + 1 < n else ""
        if state == "normal":
            if line_start and ch in " \t\r":
                i += 1
                continue
            if line_start and ch == "#":
                continued = True
                while i < n and continued:
                    line_end = source.find("\n", i)
                    if line_end < 0:
                        line_end = n
                    continued = line_end > i and source[line_end - 1] == "\\"
                    while i < line_end:
                        out[i] = " "
                        i += 1
                    if i < n:
                        i += 1
                        line_start = True
                continue
            if ch == "/" and nxt == "/":
                out[i] = out[i + 1] = " "
                i += 2
                state = "line"
                continue
            if ch == "/" and nxt == "*":
                out[i] = out[i + 1] = " "
                i += 2
                state = "comment"
                continue
            if ch in "\"'":
                state = "string" if ch == '"' else "char"
                out[i] = " "
                i += 1
                continue
            if ch == "\n":
                line_start = True
            else:
                line_start = False
            i += 1
            continue
        if state == "line":
            if ch == "\n":
                state, line_start = "normal", True
            else:
                out[i] = " "
            i += 1
            continue
        if state == "comment":
            if ch == "*" and nxt == "/":
                out[i] = out[i + 1] = " "
                i += 2
                state = "normal"
            else:
                if ch != "\n":
                    out[i] = " "
                else:
                    line_start = True
                i += 1
            continue
        # Strings and character constants are blanked so punctuation in them
        # cannot affect delimiter matching.
        if ch == "\\" and i + 1 < n:
            out[i] = " "
            if source[i + 1] != "\n":
                out[i + 1] = " "
            i += 2
        elif (state == "string" and ch == '"') or (state == "char" and ch == "'"):
            out[i] = " "
            i += 1
            state = "normal"
        else:
            if ch != "\n":
                out[i] = " "
            else:
                line_start = True
            i += 1
    return "".join(out)


FUNCTION_MACRO_RE = re.compile(
    r"(?m)^\s*#\s*define\s+([A-Za-z_$][A-Za-z0-9_$]*)\(")
CONDITIONAL_DIRECTIVE_RE = re.compile(
    r"(?m)^\s*#\s*(?:if|ifdef|ifndef|elif|else|endif)\b")


def _function_macro_names(source: str) -> set[str]:
    return set(FUNCTION_MACRO_RE.findall(source))


def _split_top_level(tokens: list[str], delimiter: str) -> list[list[str]]:
    pieces: list[list[str]] = []
    start = 0
    paren = bracket = 0
    for i, token in enumerate(tokens):
        if token == "(":
            paren += 1
        elif token == ")":
            paren -= 1
        elif token == "[":
            bracket += 1
        elif token == "]":
            bracket -= 1
        elif token == delimiter and paren == 0 and bracket == 0:
            pieces.append(tokens[start:i])
            start = i + 1
    pieces.append(tokens[start:])
    return pieces


def _strip_parameter_name(tokens: list[str]) -> Optional[list[str]]:
    if not tokens:
        return None
    # Function pointer declarator: return-type (*name)(args). Keep the nested
    # function type; remove only the identifier between * and ).
    for i in range(len(tokens) - 2):
        if tokens[i] == "(" and tokens[i + 1] == "*":
            j = i + 2
            while j < len(tokens) and tokens[j] in QUALIFIERS:
                j += 1
            if j < len(tokens) and re.fullmatch(r"[A-Za-z_$][\w$]*", tokens[j]):
                del tokens[j]
            return tokens
    # For simple declarators, a final identifier is the parameter name unless
    # it is the only typedef/base token.
    if (len(tokens) >= 2 and re.fullmatch(r"[A-Za-z_$][\w$]*", tokens[-1])
            and tokens[-1] not in TYPE_WORDS
            and tokens[-2] not in {"struct", "union", "enum"}):
        tokens = tokens[:-1]
    elif len(tokens) == 1 and tokens[0] not in TYPE_WORDS:
        # A single unknown identifier is a typedef type, not a parameter name.
        pass
    return tokens


def _parse_type(tokens: list[str], allow_abstract_declarator: bool = True,
                has_parameter_name: bool = True, is_parameter: bool = False) -> Optional[TypeShape]:
    if not tokens:
        return None
    if has_parameter_name:
        tokens = _strip_parameter_name(tokens)
    else:
        tokens = [token for token in tokens if token not in STORAGE_WORDS]
    if not tokens:
        return None
    if "[" in tokens or "{" in tokens or "}" in tokens or "=" in tokens:
        return None
    # Normalize function-pointer parameters recursively so parameter names do
    # not create false differences.
    if "(" in tokens:
        declarator = next((i for i, token in enumerate(tokens) if token == "("), -1)
        if declarator < 0 or declarator + 1 >= len(tokens) or tokens[declarator + 1] != "*":
            return None
        close_decl = declarator + 2
        while close_decl < len(tokens) and tokens[close_decl] != ")":
            close_decl += 1
        if close_decl + 1 >= len(tokens) or tokens[close_decl + 1] != "(":
            return None
        close_params = close_decl + 2
        nesting = 1
        while close_params < len(tokens) and nesting:
            if tokens[close_params] == "(":
                nesting += 1
            elif tokens[close_params] == ")":
                nesting -= 1
            close_params += 1
        if nesting:
            return None
        ret = _parse_type(tokens[:declarator], allow_abstract_declarator=False,
                          has_parameter_name=False)
        if ret is None:
            return None
        nested_params, nested_varargs, error = _parse_parameters(tokens[close_decl + 2:close_params - 1])
        if error:
            return None
        params_repr = "unspecified" if nested_params is None else ",".join(p.spelling for p in nested_params)
        if nested_varargs:
            params_repr += ",..."
        pointer_quals = [t for t in tokens[declarator + 2:close_decl] if t in QUALIFIERS]
        if is_parameter:
            pointer_quals = []  # the callback pointer itself is the top-level parameter type
        spelling = f"function-pointer({ret.spelling})({params_repr})"
        if pointer_quals:
            spelling += "[" + ",".join(sorted(pointer_quals)) + "]"
        return TypeShape(spelling, 1, "function-pointer", ())
    pointer_depth = tokens.count("*")
    first_star = tokens.index("*") if pointer_depth else len(tokens)
    base_tokens = tokens[:first_star]
    if not base_tokens:
        return None
    if any(not re.fullmatch(r"[A-Za-z_$][\w$]*", t) for t in base_tokens):
        return None
    # `struct Foo` / `enum Bar` are simple base types. Unknown identifiers are
    # accepted as project typedefs, while unsupported declarator punctuation is
    # rejected above.
    base_quals = tuple(sorted(t for t in base_tokens if t in QUALIFIERS))
    base_words = [t for t in base_tokens if t not in QUALIFIERS]
    if not base_words:
        return None
    base = " ".join(base_words)
    base = TYPE_ALIASES.get(base, base)
    base = {
        "signed": "int", "signed int": "int", "short": "short int",
        "signed short": "short int", "long": "long int", "signed long": "long int",
        "long long": "long long int", "signed long long": "long long int",
        "unsigned": "unsigned int", "unsigned short": "unsigned short int",
        "unsigned long": "unsigned long int", "unsigned long long": "unsigned long long int",
    }.get(base, base)
    pointer_quals: list[tuple[str, ...]] = []
    cursor = first_star
    while cursor < len(tokens):
        if tokens[cursor] != "*":
            return None
        cursor += 1
        level: list[str] = []
        while cursor < len(tokens) and tokens[cursor] != "*":
            if tokens[cursor] not in QUALIFIERS:
                return None
            level.append(tokens[cursor])
            cursor += 1
        pointer_quals.append(tuple(sorted(level)))

    # Top-level parameter qualifiers do not affect a function type. For a
    # pointer parameter this is the outermost (last) star; qualifiers on the
    # pointee or an inner pointer remain significant.
    if is_parameter:
        if pointer_quals:
            pointer_quals[-1] = ()
        else:
            base_quals = ()
    spelling = " ".join((*base_quals, base)).strip()
    for level in pointer_quals:
        spelling += " *" + ((" " + " ".join(level)) if level else "")
    qualifier_shape = base_quals + tuple(
        f"pointer{index}:{qualifier}" for index, level in enumerate(pointer_quals, 1)
        for qualifier in level
    )
    return TypeShape(spelling, pointer_depth, base, qualifier_shape)


def _parse_parameters(tokens: list[str]) -> tuple[Optional[tuple[TypeShape, ...]], bool, Optional[str]]:
    if not tokens:
        return None, False, None  # C89 f() is unspecified, not f(void).
    parts = _split_top_level(tokens, ",")
    if len(parts) == 1 and parts[0] == ["void"]:
        return (), False, None
    varargs = bool(parts and parts[-1] == ["..."])
    if varargs:
        parts = parts[:-1]
    params: list[TypeShape] = []
    for part in parts:
        parsed = _parse_type(part, is_parameter=True)
        if parsed is None:
            return None, varargs, "unsupported parameter declarator"
        params.append(parsed)
    if varargs and not params:
        return None, True, "varargs without a fixed parameter"
    return tuple(params), varargs, None


def _looks_like_knr_definition(tokens: list[str]) -> bool:
    for opening, token in enumerate(tokens):
        if token != "(" or opening == 0 or not re.fullmatch(r"[A-Za-z_$][\w$]*", tokens[opening - 1]):
            continue
        depth = 1
        close = opening + 1
        while close < len(tokens) and depth:
            if tokens[close] == "(":
                depth += 1
            elif tokens[close] == ")":
                depth -= 1
            close += 1
        if depth:
            continue
        params = tokens[opening + 1:close - 1]
        parts = _split_top_level(params, ",")
        if parts and all(len(part) == 1 and part[0] not in TYPE_WORDS for part in parts):
            if close < len(tokens):
                return True
    return False


def _candidate(tokens: list[str], delimiter: str, path: str, line: int) -> tuple[Optional[Signature], Optional[str]]:
    """Parse a top-level declaration/definition segment before ; or {."""
    if not tokens:
        return None, None
    # A supported direct function declarator ends at the segment's final `)`.
    close = len(tokens) - 1
    if tokens[close] != ")":
        if delimiter in {"{", ";"} and _looks_like_knr_definition(tokens):
            return None, "K&R-style definition skipped"
        return None, None
    depth = 0
    opening = None
    for i in range(close, -1, -1):
        if tokens[i] == ")":
            depth += 1
        elif tokens[i] == "(":
            depth -= 1
            if depth == 0:
                opening = i
                break
    if opening is None or opening == 0:
        if delimiter == "{" and _looks_like_knr_definition(tokens):
            return None, "K&R-style or implicit-int definition skipped"
        return None, None
    name = tokens[opening - 1]
    if not re.fullmatch(r"[A-Za-z_$][\w$]*", name) or name in {"if", "while", "switch", "for", "sizeof"}:
        return None, None
    # Parenthesized declarators before the function name are unsupported.
    return_tokens = tokens[:opening - 1]
    if not return_tokens:
        if delimiter == "{" and _looks_like_knr_definition(tokens):
            return None, "K&R-style or implicit-int definition skipped"
        return None, None
    if any(t in {"=", ")", "("} for t in return_tokens):
        return None, None
    ret = _parse_type(return_tokens, allow_abstract_declarator=False, has_parameter_name=False)
    if ret is None:
        return None, "unsupported return declarator"
    param_tokens = tokens[opening + 1:close]
    if delimiter == "{" and param_tokens:
        parts = _split_top_level(param_tokens, ",")
        if all(len(part) == 1 and re.fullmatch(r"[A-Za-z_$][\w$]*", part[0])
               and part[0] not in TYPE_WORDS for part in parts):
            return None, "K&R-style definition skipped"
    params, varargs, error = _parse_parameters(param_tokens)
    if error:
        return None, error
    # An opening brace after a parameter list with old-style parameter
    # declarations is K&R, not a prototype. It is deliberately skipped.
    kind = "definition" if delimiter == "{" else "declaration"
    return Signature(name, ret, params, varargs, line, path, kind,
                     "static" in tokens[:opening - 1]), None


def scan_source(
    source: str,
    path: str,
    ignored_result_calls: Optional[list[IgnoredResultCall]] = None,
    known_function_macros: AbstractSet[str] = frozenset(),
) -> tuple[list[Signature], list[dict[str, object]]]:
    masked = _mask_source(source)
    tokens = [(m.group(), m.start()) for m in TOKEN_RE.finditer(masked)]
    signatures: list[Signature] = []
    skipped: list[dict[str, object]] = []
    segment: list[str] = []
    segment_pos = 0
    brace_depth = paren_depth = bracket_depth = 0
    i = 0
    while i < len(tokens):
        token, pos = tokens[i]
        if brace_depth == paren_depth == bracket_depth == 0:
            if not segment:
                segment_pos = pos
            if token == ";":
                parsed, reason = _candidate(segment, ";", path, source.count("\n", 0, segment_pos) + 1)
                if parsed:
                    signatures.append(parsed)
                elif reason:
                    skipped.append({"path": path, "line": source.count("\n", 0, segment_pos) + 1, "reason": reason})
                segment = []
                i += 1
                continue
            if token == "{":
                parsed, reason = _candidate(segment, "{", path, source.count("\n", 0, segment_pos) + 1)
                if parsed:
                    signatures.append(parsed)
                    # Skip the complete function body so local declarations
                    # cannot be mistaken for file-scope contracts.
                    depth = 1
                    body_start = i + 1
                    i += 1
                    while i < len(tokens) and depth:
                        if tokens[i][0] == "{":
                            depth += 1
                        elif tokens[i][0] == "}":
                            depth -= 1
                        i += 1
                    if (ignored_result_calls is not None and depth == 0
                            and not CONDITIONAL_DIRECTIVE_RE.search(source)):
                        ignored_result_calls.extend(_body_ignored_result_calls(
                            tokens[body_start:i - 1], source, path, parsed.name,
                            _definition_parameter_names(segment),
                            known_function_macros | _function_macro_names(source)))
                    segment = []
                    continue
                if reason:
                    skipped.append({"path": path, "line": source.count("\n", 0, segment_pos) + 1, "reason": reason})
                # Struct/initializer block: skip nested braces but resume after
                # its matching close (the trailing semicolon is harmless).
                depth = 1
                i += 1
                while i < len(tokens) and depth:
                    if tokens[i][0] == "{":
                        depth += 1
                    elif tokens[i][0] == "}":
                        depth -= 1
                    i += 1
                segment = []
                continue
            segment.append(token)
        i += 1
    # A common project form spells a typedef of its own struct tag as both
    # `struct Name` and `Name`. Normalize that only when the file contains an
    # actual named typedef definition, so unrelated tags remain distinct.
    struct_aliases = set(re.findall(
        r"\btypedef\s+struct\s+([A-Za-z_$][\w$]*)\s*\{[\s\S]*?\}\s*([A-Za-z_$][\w$]*)\s*;",
        _mask_source(source)))
    aliases = {tag for tag, alias in struct_aliases if tag == alias}
    if aliases:
        def normalize(shape: TypeShape) -> TypeShape:
            match = re.fullmatch(r"struct ([A-Za-z_$][\w$]*)", shape.base)
            if match and match.group(1) in aliases:
                return TypeShape(shape.spelling.replace(shape.base, match.group(1)), shape.pointer_depth,
                                 match.group(1), shape.qualifiers)
            return shape
        signatures = [Signature(sig.name, normalize(sig.return_type),
                                tuple(normalize(param) for param in sig.params) if sig.params is not None else None,
                                sig.varargs, sig.line, sig.path, sig.kind, sig.is_static) for sig in signatures]
    return signatures, skipped


def _direct_call_statement(
    segment: list[tuple[str, int]],
) -> Optional[tuple[str, int, bool]]:
    """Return a direct callee when a statement consists only of its call.

    This deliberately excludes assignments, returns, comma expressions,
    conditionals, and calls nested in arguments. Those forms use or may use
    the result and require a real C parser to classify safely.
    """
    explicit_void_cast = False
    if [token for token, _ in segment[:3]] == ["(", "void", ")"]:
        segment = segment[3:]
        explicit_void_cast = True
    if len(segment) < 3:
        return None
    name, position = segment[0]
    if (not re.fullmatch(r"[A-Za-z_$][\w$]*", name)
            or name in {"if", "while", "switch", "for", "sizeof", "return"}
            or segment[1][0] != "("):
        return None
    depth = 0
    close = None
    for index, (token, _) in enumerate(segment[1:], 1):
        if token == "(":
            depth += 1
        elif token == ")":
            depth -= 1
            if depth == 0:
                close = index
                break
        if depth < 0:
            return None
    if close != len(segment) - 1:
        return None
    return name, position, explicit_void_cast


def _declared_name(tokens: list[str]) -> Optional[str]:
    """Return one conservatively recognized declarator name."""
    for index in range(len(tokens) - 3):
        if tokens[index:index + 2] != ["(", "*"]:
            continue
        cursor = index + 2
        while cursor < len(tokens) and tokens[cursor] in QUALIFIERS:
            cursor += 1
        if (cursor < len(tokens)
                and re.fullmatch(r"[A-Za-z_$][\w$]*", tokens[cursor])):
            return tokens[cursor]
    if "=" in tokens:
        tokens = tokens[:tokens.index("=")]
    if len(tokens) < 2:
        return None
    candidate = next((
        tokens[index - 1]
        for index, token in enumerate(tokens)
        if token == "[" and index > 0
        and re.fullmatch(r"[A-Za-z_$][\w$]*", tokens[index - 1])
    ), None)
    if candidate is None:
        identifiers = [
            token for token in tokens
            if re.fullmatch(r"[A-Za-z_$][\w$]*", token)
            and token not in TYPE_WORDS | QUALIFIERS | STORAGE_WORDS
        ]
        candidate = identifiers[-1] if identifiers else None
    if candidate is None or _parse_type(list(tokens)) is None:
        return None
    return candidate


def _declared_names(tokens: list[str]) -> set[str]:
    """Return names from one conservatively recognized declaration."""
    parts = _split_top_level(tokens, ",")
    first = _declared_name(parts[0]) if parts else None
    if first is None:
        return set()
    names = {first}
    for part in parts[1:]:
        if "=" in part:
            part = part[:part.index("=")]
        function_pointer = None
        for index in range(len(part) - 2):
            if part[index:index + 2] == ["(", "*"]:
                cursor = index + 2
                while cursor < len(part) and part[cursor] in QUALIFIERS:
                    cursor += 1
                if (cursor < len(part)
                        and re.fullmatch(r"[A-Za-z_$][\w$]*", part[cursor])):
                    function_pointer = part[cursor]
                    break
        if function_pointer is not None:
            names.add(function_pointer)
            continue
        identifiers = [
            token for token in part
            if re.fullmatch(r"[A-Za-z_$][\w$]*", token)
            and token not in TYPE_WORDS | QUALIFIERS | STORAGE_WORDS
        ]
        if identifiers:
            names.add(identifiers[-1])
    return names


def _definition_parameter_names(header: list[str]) -> set[str]:
    if not header or header[-1] != ")":
        return set()
    depth = 0
    opening = None
    for index in range(len(header) - 1, -1, -1):
        if header[index] == ")":
            depth += 1
        elif header[index] == "(":
            depth -= 1
            if depth == 0:
                opening = index
                break
    if opening is None:
        return set()
    names: set[str] = set()
    for part in _split_top_level(header[opening + 1:-1], ","):
        names.update(_declared_names(part))
    return names


def _function_local_names(body: list[tuple[str, int]]) -> set[str]:
    """Find local declarators; any same-named call then fails closed."""
    names: set[str] = set()
    segment: list[str] = []
    paren = bracket = 0
    index = 0
    while index < len(body):
        token = body[index][0]
        if token == "for" and index + 1 < len(body) and body[index + 1][0] == "(":
            depth = 1
            cursor = index + 2
            initializer: list[str] = []
            while cursor < len(body) and depth:
                value = body[cursor][0]
                if value == "(":
                    depth += 1
                elif value == ")":
                    depth -= 1
                if value == ";" and depth == 1:
                    break
                initializer.append(value)
                cursor += 1
            names.update(_declared_names(initializer))
        if token == "(":
            paren += 1
        elif token == ")":
            paren -= 1
        elif token == "[":
            bracket += 1
        elif token == "]":
            bracket -= 1
        if token == ";" and paren == bracket == 0:
            names.update(_declared_names(segment))
            segment = []
        elif token in {"{", "}"} and paren == bracket == 0:
            segment = []
        else:
            segment.append(token)
        index += 1
    return names


def _body_ignored_result_calls(
    body: list[tuple[str, int]], source: str, path: str, caller: str,
    parameter_names: set[str], excluded_names: AbstractSet[str],
) -> list[IgnoredResultCall]:
    newlines = [index for index, char in enumerate(source) if char == "\n"]
    calls: list[IgnoredResultCall] = []
    segment: list[tuple[str, int]] = []
    shadowed_names = set(parameter_names) | _function_local_names(body) | excluded_names
    paren = bracket = 0
    for token, position in body:
        if token == "(":
            paren += 1
        elif token == ")":
            paren -= 1
        elif token == "[":
            bracket += 1
        elif token == "]":
            bracket -= 1
        if token == ";" and paren == bracket == 0:
            found = _direct_call_statement(segment)
            if found is not None and found[0] not in shadowed_names:
                name, call_position, explicit_void_cast = found
                calls.append(IgnoredResultCall(
                    name=name,
                    caller=caller,
                    line=bisect.bisect_right(newlines, call_position) + 1,
                    path=path,
                    explicit_void_cast=explicit_void_cast,
                ))
            segment = []
        elif token in {"{", "}"} and paren == bracket == 0:
            # A block boundary cannot be part of a direct expression
            # statement. Clearing also rejects initializer/compound forms.
            segment = []
        else:
            segment.append((token, position))
    return calls


def scan_ignored_result_calls(
    source: str, path: str,
    known_function_macros: AbstractSet[str] = frozenset(),
) -> list[IgnoredResultCall]:
    """Find conservative direct call expression statements in C functions."""
    calls: list[IgnoredResultCall] = []
    scan_source(source, path, calls, known_function_macros)
    return calls


def _mismatch_class(left: TypeShape, right: TypeShape) -> str:
    if left.pointer_depth != right.pointer_depth and {left.pointer_depth, right.pointer_depth} == {0, 1}:
        return "representation-sensitive pointer/integer mismatch; review before treating as ABI conflict"
    return "type mismatch"


def _uncertain_typedef_pair(left: TypeShape, right: TypeShape) -> bool:
    if left.base == right.base:
        return False
    def unknown(base: str) -> bool:
        return base not in KNOWN_BASES and not base.startswith(("struct ", "union ", "enum "))
    return unknown(left.base) or unknown(right.base)


def compare(signatures: Iterable[Signature], skipped: list[dict[str, object]]) -> dict[str, object]:
    grouped: dict[tuple[str, str], list[Signature]] = {}
    for sig in signatures:
        parts = Path(sig.path).parts
        game = next((part for part in parts if part in {"dds1", "dds2"}), "")
        # External symbols are compared across the title's source tree. A
        # static declaration/definition is visible only in its own file.
        scope = f"{game}:{sig.path}" if sig.is_static else game
        grouped.setdefault((scope, sig.name), []).append(sig)
    conflicts: list[dict[str, object]] = []
    for (_scope, name), entries in sorted(grouped.items()):
        definitions = [x for x in entries if x.kind == "definition"]
        declarations = [x for x in entries if x.kind == "declaration" and x.params is not None]
        for definition in definitions:
            for declaration in declarations:
                if declaration.path == definition.path and declaration.line == definition.line:
                    continue
                fields: list[dict[str, str]] = []
                if definition.return_type.spelling != declaration.return_type.spelling:
                    if _uncertain_typedef_pair(declaration.return_type, definition.return_type):
                        skipped.append({"path": declaration.path, "line": declaration.line,
                                        "reason": f"{name} return type: typedef identity cannot be resolved against {definition.path}:{definition.line}"})
                    else:
                        fields.append({"field": "return", "declaration": declaration.return_type.spelling,
                                       "definition": definition.return_type.spelling,
                                       "classification": _mismatch_class(declaration.return_type, definition.return_type)})
                if declaration.params is not None and definition.params is not None:
                    if len(declaration.params) != len(definition.params):
                        fields.append({"field": "parameters", "declaration": str(len(declaration.params)),
                                       "definition": str(len(definition.params)), "classification": "parameter-count mismatch"})
                    else:
                        for index, (decl_type, def_type) in enumerate(zip(declaration.params, definition.params), 1):
                            if decl_type.spelling != def_type.spelling:
                                if _uncertain_typedef_pair(decl_type, def_type):
                                    skipped.append({"path": declaration.path, "line": declaration.line,
                                                    "reason": f"{name} parameter {index}: typedef identity cannot be resolved against {definition.path}:{definition.line}"})
                                else:
                                    fields.append({"field": f"parameter {index}", "declaration": decl_type.spelling,
                                                   "definition": def_type.spelling,
                                                   "classification": _mismatch_class(decl_type, def_type)})
                if declaration.varargs != definition.varargs:
                    fields.append({"field": "varargs", "declaration": str(declaration.varargs),
                                   "definition": str(definition.varargs), "classification": "varargs mismatch"})
                if fields:
                    conflicts.append({"name": name,
                                      "declaration": {"path": declaration.path, "line": declaration.line},
                                      "definition": {"path": definition.path, "line": definition.line},
                                      "mismatches": fields})
    return {"conflicts": conflicts, "skipped": skipped,
            "summary": {"conflicts": len(conflicts), "skipped_unsupported": len(skipped)}}


def annotate_ignored_result_calls(
    report: dict[str, object],
    signatures: Iterable[Signature],
    calls: Iterable[IgnoredResultCall],
) -> None:
    """Attach direct ignored-result evidence to return-contract conflicts.

    A definition is only a comparison anchor, not proof of the historical
    interface. Consequently this adds review evidence and never recommends a
    declaration change.
    """
    signature_rows = list(signatures)
    calls_by_symbol: dict[tuple[str, str], list[IgnoredResultCall]] = {}
    signatures_by_symbol: dict[tuple[str, str], list[Signature]] = {}
    external_definitions: dict[tuple[str, str], list[Signature]] = {}
    static_definitions: dict[tuple[str, str], list[Signature]] = {}
    for call in calls:
        calls_by_symbol.setdefault((call.name, call.path), []).append(call)
    for sig in signature_rows:
        signatures_by_symbol.setdefault((sig.name, sig.path), []).append(sig)
        if sig.kind != "definition":
            continue
        if sig.is_static:
            static_definitions.setdefault((sig.name, sig.path), []).append(sig)
        else:
            game = next((part for part in Path(sig.path).parts
                         if part in {"dds1", "dds2"}), "")
            external_definitions.setdefault((game, sig.name), []).append(sig)
    attached = 0
    for conflict in report["conflicts"]:
        return_mismatch = next(
            (item for item in conflict["mismatches"] if item["field"] == "return"),
            None,
        )
        if return_mismatch is None:
            continue
        declaration = conflict["declaration"]
        definition = conflict["definition"]
        evidence: list[dict[str, object]] = []
        for call in calls_by_symbol.get(
                (conflict["name"], declaration["path"]), []):
            visible = [
                sig for sig in signatures_by_symbol.get((call.name, call.path), [])
                if sig.line < call.line
            ]
            if not visible:
                continue
            latest_line = max(sig.line for sig in visible)
            latest = [sig for sig in visible if sig.line == latest_line]
            # Fail closed when the most recent visible source contract is not
            # uniquely the declaration represented by this conflict row.
            if (len(latest) != 1
                    or latest[0].path != declaration["path"]
                    or latest[0].line != declaration["line"]):
                continue
            visible_contract = latest[0]
            call_game = next(
                (part for part in Path(call.path).parts if part in {"dds1", "dds2"}), "")
            if visible_contract.is_static:
                eligible_definitions = static_definitions.get(
                    (call.name, call.path), [])
            else:
                eligible_definitions = external_definitions.get(
                    (call_game, call.name), [])
            if (len(eligible_definitions) != 1
                    or eligible_definitions[0].path != definition["path"]
                    or eligible_definitions[0].line != definition["line"]):
                continue
            declared = return_mismatch["declaration"]
            defined = return_mismatch["definition"]
            if declared != "void" and defined == "void":
                mechanism = "value-return declaration for void definition"
            elif declared == "void" and defined != "void":
                mechanism = "void declaration for value-return definition"
            else:
                mechanism = "ignored-result return representation mismatch"
            evidence.append({
                "path": call.path,
                "line": call.line,
                "caller": call.caller,
                "callee": call.name,
                "source_form": "explicit (void) discard" if call.explicit_void_cast else "direct expression statement",
                "mechanism": mechanism,
                "disposition": "review caller and emitted data flow; do not change the declaration mechanically",
                "definition": {"path": definition["path"], "line": definition["line"]},
            })
        if evidence:
            conflict["ignored_result_calls"] = sorted(
                evidence, key=lambda row: (row["path"], row["line"], row["caller"]))
            attached += len(evidence)
    report["summary"]["ignored_result_call_sites"] = attached


def _source_paths(args: argparse.Namespace) -> list[Path]:
    if args.paths:
        roots = [Path(path) for path in args.paths]
    else:
        versions = args.version or ("dds1", "dds2")
        roots = [Path("src") / version for version in versions]
    paths: set[Path] = set()
    for root in roots:
        if root.is_dir():
            paths.update(p for p in root.rglob("*.c") if p.is_file())
        elif root.is_file() and root.suffix == ".c":
            paths.add(root)
    return sorted(paths)


def _project_function_macro_names() -> set[str]:
    """Collect header macro names that could replace call-like source text."""
    names: set[str] = set()
    for root in (Path("include"), Path("src")):
        if root.is_dir():
            for path in root.rglob("*.h"):
                names.update(_function_macro_names(path.read_text(errors="replace")))
    return names


def _game(path: Path | str) -> str | None:
    return next((part for part in Path(path).parts if part in {"dds1", "dds2"}), None)


def _definition_index(focus: list[Path]) -> list[Signature]:
    """Load authoritative definitions needed by a focused path audit.

    A declaration in one unit normally refers to a definition in another, so
    narrowing the declaration scan must not also hide the definition evidence.
    """
    games = sorted({game for path in focus if (game := _game(path))})
    definitions: list[Signature] = []
    for game in games:
        for path in sorted((Path("src") / game).rglob("*.c")):
            found, _ = scan_source(path.read_text(errors="replace"), str(path))
            definitions.extend(sig for sig in found if sig.kind == "definition")
    return definitions


def main(argv: Optional[list[str]] = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("paths", nargs="*", help="C files or directories (default: src/dds1 and src/dds2)")
    parser.add_argument("--version", action="append", choices=("dds1", "dds2"),
                        help="limit the default whole-tree scan (repeatable)")
    parser.add_argument("--symbol", action="append", default=[],
                        help="report only this symbol (repeatable)")
    parser.add_argument("--json", metavar="PATH", help="write stable JSON report to PATH (or - for stdout)")
    opts = parser.parse_args(argv)
    missing = [path for path in opts.paths if not Path(path).exists()]
    if missing:
        parser.error(f"path does not exist: {missing[0]}")
    focus = _source_paths(opts)
    if not focus:
        parser.error("no C source files found in the requested scope")
    signatures: list[Signature] = []
    ignored_result_calls: list[IgnoredResultCall] = []
    known_function_macros = _project_function_macro_names()
    skipped: list[dict[str, object]] = []
    for path in focus:
        source = path.read_text(errors="replace")
        found, unsupported = scan_source(
            source, str(path), ignored_result_calls, known_function_macros)
        signatures.extend(found)
        skipped.extend(unsupported)
    focus_signatures = list(signatures)
    focus_signature_count = len(signatures)
    scope_declaration_count = sum(sig.kind == "declaration" for sig in focus_signatures)
    if opts.paths:
        # Keep the requested files as the declaration scope, but join them to
        # real definitions across the same title. Dataclass equality removes
        # definitions that were already present in a focused file.
        signatures = list(dict.fromkeys(signatures + _definition_index(focus)))
    if opts.symbol:
        wanted = set(opts.symbol)
        signatures = [sig for sig in signatures if sig.name in wanted]
        selected_focus = [sig for sig in focus_signatures if sig.name in wanted]
    else:
        selected_focus = focus_signatures
    focus_declaration_count = sum(sig.kind == "declaration" for sig in selected_focus)
    report = compare(signatures, skipped)
    annotate_ignored_result_calls(report, signatures, ignored_result_calls)
    report["summary"].update({
        "focus_files": len(focus),
        "focus_signatures": focus_signature_count,
        "scope_declarations": scope_declaration_count,
        "focus_declarations": focus_declaration_count,
        "indexed_definitions": sum(sig.kind == "definition" for sig in signatures),
        "symbols_with_conflicts": len({row["name"] for row in report["conflicts"]}),
    })
    report["conflicts"] = sorted(report["conflicts"], key=lambda row: (row["name"], row["declaration"]["path"], row["declaration"]["line"]))
    report["skipped"] = sorted(report["skipped"], key=lambda row: (row["path"], row["line"], row["reason"]))
    if opts.json:
        rendered = json.dumps(report, indent=2, sort_keys=True) + "\n"
        if opts.json == "-":
            sys.stdout.write(rendered)
        else:
            Path(opts.json).write_text(rendered)
    if report["conflicts"]:
        for row in report["conflicts"]:
            loc = lambda item: f"{item['path']}:{item['line']}"
            print(f"{row['name']}: declaration {loc(row['declaration'])} vs definition {loc(row['definition'])}")
            for mismatch in row["mismatches"]:
                print(f"  {mismatch['field']}: {mismatch['declaration']} != {mismatch['definition']} ({mismatch['classification']})")
            for call in row.get("ignored_result_calls", []):
                print(f"  ignored result: {call['path']}:{call['line']} in {call['caller']} "
                      f"({call['mechanism']}; review emitted data flow)")
    else:
        print("No fixed declaration/definition contract mismatches found.")
    print(f"Audited {focus_declaration_count} focused declarations against "
          f"{report['summary']['indexed_definitions']} indexed definitions; "
          f"skipped {len(skipped)} unsupported or uncertain comparisons.")
    return 1 if report["conflicts"] else 0


if __name__ == "__main__":
    raise SystemExit(main())
