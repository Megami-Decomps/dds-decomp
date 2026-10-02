#!/usr/bin/env python3
"""Explain delay-slot sequences in an EE GCC 2.96 reorg dump."""

from __future__ import annotations

import argparse
import json
import re
import sys
from pathlib import Path


NODE_HEADER = re.compile(r"\((call_insn|jump_insn|insn)(?::[^\s]+)?\s+(\d+)")
SYMBOL_REF = re.compile(r'\(symbol_ref(?::[^\s]+)?\s+\("([^"]+)"\)')
SET_REGISTER = re.compile(
    r"\(set\s+\(reg(?:/[^:\s]+)?(?::[^\s]+)?\s+\d+\s+([^\)]+)\)"
)
SOURCE_LOCATION = re.compile(r'\("([^"]+)"\)\s+(\d+)\)')


def balanced_form(text: str, start: int) -> tuple[str, int]:
    """Return the parenthesized form at START and its exclusive end."""
    if start >= len(text) or text[start] != "(":
        raise ValueError("form does not start with '('")
    depth = 0
    quoted = False
    escaped = False
    for index in range(start, len(text)):
        char = text[index]
        if quoted:
            if escaped:
                escaped = False
            elif char == "\\":
                escaped = True
            elif char == '"':
                quoted = False
            continue
        if char == '"':
            quoted = True
        elif char == "(":
            depth += 1
        elif char == ")":
            depth -= 1
            if depth == 0:
                return text[start:index + 1], index + 1
    raise ValueError("unterminated RTL form")


def top_level_forms(text: str) -> list[str]:
    forms: list[str] = []
    cursor = 0
    while cursor < len(text):
        start = text.find("(", cursor)
        if start < 0:
            break
        try:
            form, cursor = balanced_form(text, start)
        except ValueError:
            break
        forms.append(form)
    return forms


def sequence_nodes(form: str) -> list[tuple[str, int, str]]:
    marker = form.find("(sequence[")
    if marker < 0:
        return []
    nodes: list[tuple[str, int, str]] = []
    cursor = marker + len("(sequence[")
    while match := NODE_HEADER.search(form, cursor):
        node, end = balanced_form(form, match.start())
        nodes.append((match.group(1), int(match.group(2)), node))
        cursor = end
    return nodes


def describe_node(kind: str, uid: int, form: str) -> dict[str, object]:
    result: dict[str, object] = {"kind": kind, "uid": uid}
    symbol = SYMBOL_REF.search(form)
    if symbol:
        result["target"] = symbol.group(1)
    location = SOURCE_LOCATION.search(form)
    if location:
        result["source"] = {"file": location.group(1), "line": int(location.group(2))}

    destination = SET_REGISTER.search(form)
    if destination:
        register = destination.group(1).strip()
        result["destination"] = register
        result["operation"] = "load" if "(mem" in form else "set"
    elif "asm_operands" in form or "asm_input" in form:
        result["operation"] = "asm"
    elif kind == "call_insn":
        result["operation"] = "call"
    elif kind == "jump_insn":
        result["operation"] = "jump"
    else:
        result["operation"] = "other"
    return result


def analyze_dump(text: str) -> list[dict[str, object]]:
    sequences: list[dict[str, object]] = []
    for form in top_level_forms(text):
        if "(sequence[" not in form:
            continue
        wrapper = NODE_HEADER.match(form)
        nodes = sequence_nodes(form)
        if wrapper is None or len(nodes) < 2:
            continue
        branch_kind, branch_uid, branch_form = nodes[0]
        sequences.append({
            "wrapper_uid": int(wrapper.group(2)),
            "branch": describe_node(branch_kind, branch_uid, branch_form),
            "slots": [describe_node(kind, uid, node) for kind, uid, node in nodes[1:]],
        })
    return sequences


def resolve_dump(probe: Path, function: str | None) -> Path:
    if function:
        path = probe / "functions" / function / "29.dbr"
    else:
        path = probe / "rtl.29.dbr"
    if not path.is_file():
        scope = f" for function {function}" if function else ""
        raise FileNotFoundError(f"missing delayed-branch dump{scope}: {path}")
    return path


def render(sequences: list[dict[str, object]]) -> str:
    lines = [f"delay sequences  {len(sequences)}"]
    for sequence in sequences:
        branch = sequence["branch"]
        assert isinstance(branch, dict)
        target = f" {branch['target']}" if "target" in branch else ""
        lines.append(
            f"  uid {branch['uid']} {branch['kind']}{target} "
            f"-> wrapper uid {sequence['wrapper_uid']}"
        )
        for index, slot in enumerate(sequence["slots"]):
            assert isinstance(slot, dict)
            operation = str(slot["operation"])
            destination = f" {slot['destination']}" if "destination" in slot else ""
            source = ""
            if "source" in slot:
                location = slot["source"]
                assert isinstance(location, dict)
                source = f" ({location['file']}:{location['line']})"
            lines.append(
                f"    slot {index}: uid {slot['uid']} {operation}{destination}{source}"
            )
    return "\n".join(lines)


def parse_args(argv: list[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("probe", type=Path, help="directory produced by ee_gcc_probe.py")
    parser.add_argument("--function", help="use the extracted target-function dump")
    parser.add_argument("--json", type=Path, help="also write a machine-readable report")
    return parser.parse_args(argv)


def main(argv: list[str] | None = None) -> int:
    args = parse_args(argv)
    try:
        dump = resolve_dump(args.probe, args.function)
    except FileNotFoundError as error:
        print(error, file=sys.stderr)
        return 2
    sequences = analyze_dump(dump.read_text(errors="replace"))
    report = {
        "dump": str(dump),
        "function": args.function,
        "sequences": sequences,
    }
    if args.json:
        args.json.parent.mkdir(parents=True, exist_ok=True)
        args.json.write_text(json.dumps(report, indent=2) + "\n")
    print(render(sequences))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
