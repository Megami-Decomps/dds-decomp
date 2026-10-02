#!/usr/bin/env python3
"""Explain contested choices in EE GCC 2.96 verbose scheduler dumps."""

from __future__ import annotations

import argparse
import json
import re
import sys
from pathlib import Path
from typing import Any


FUNCTION = re.compile(r"(?m)^;; Function (.+?)\s*$")
BLOCK = re.compile(
    r"(?m)^;;\s+-- basic block (\d+) from .* -- (before|after) reload\s*$"
)
DEPENDENCY_ROW = re.compile(
    r"^;;\s+(?:\+\s+)?(\d+)\s+(-?\d+)\s+(\d+)\s+(\d+)\s+(-?\d+)\s+"
    r"(-?\d+)\s+\S+\s+-\s+\S+\s+(\S+)\s*:\s*(.*)$"
)
READY = re.compile(r"^;;\s*Ready list \(t =\s*(\d+)\):\s*(.*)$")
RESOLVED_READY = re.compile(
    r"^;;\s*dependences resolved: insn (\d+)(?:\s*/b\d+)? into ready\s*$"
)
SELECT = re.compile(r"^;;\s*--> scheduling insn <<<(\d+)>>>")
# Sched1 may print a ready instruction as ``UID/bBLOCK``.  The block suffix
# describes the region placement; it is not a second instruction UID.
UIDS = re.compile(r"(?<![A-Za-z0-9_/])\d+")


def split_sections(text: str, pattern: re.Pattern[str]) -> list[tuple[re.Match[str], str]]:
    matches = list(pattern.finditer(text))
    return [
        (match, text[match.start(): matches[index + 1].start()
                      if index + 1 < len(matches) else len(text)])
        for index, match in enumerate(matches)
    ]


def printed_field_heuristic(candidates: list[dict[str, Any]],
                            selected: int) -> list[dict[str, Any]]:
    """Show how far fields printed by GCC would narrow the ready set.

    The complete comparator also considers dependency classes which the
    verbose table does not print, so this heuristic is not a policy replay.
    """
    remaining = list(candidates)
    steps: list[dict[str, Any]] = []
    for field, prefer in (("priority", max), ("forward_dependents", max),
                          ("table_order", min)):
        if not remaining:
            break
        value = prefer(int(item[field]) for item in remaining)
        narrowed = [item for item in remaining if int(item[field]) == value]
        steps.append({
            "field": field,
            "preferred": value,
            "uids": [int(item["uid"]) for item in narrowed],
            "selected_survives": any(int(item["uid"]) == selected for item in narrowed),
        })
        if not any(int(item["uid"]) == selected for item in narrowed):
            break
        remaining = narrowed
        if len(remaining) == 1:
            break
    return steps


def parse_block(number: int, reload_state: str, text: str) -> dict[str, Any]:
    instructions: dict[int, dict[str, Any]] = {}
    table_order = 0
    current_clock: int | None = None
    current_ready: list[int] = []
    selections: list[dict[str, Any]] = []
    ready_count = 0
    selection_count = 0

    for line in text.splitlines():
        if match := DEPENDENCY_ROW.match(line):
            uid = int(match.group(1))
            forward = [int(value) for value in UIDS.findall(match.group(8))]
            instructions[uid] = {
                "uid": uid,
                "code": int(match.group(2)),
                "basic_block": int(match.group(3)),
                "dependencies": int(match.group(4)),
                "priority": int(match.group(5)),
                "cost": int(match.group(6)),
                "units": match.group(7),
                "forward_dependents": len(forward),
                "forward_uids": forward,
                "table_order": table_order,
            }
            table_order += 1
            continue
        if match := READY.match(line):
            current_clock = int(match.group(1))
            current_ready = [int(value) for value in UIDS.findall(match.group(2))]
            missing = [uid for uid in current_ready if uid not in instructions]
            if missing:
                raise ValueError(
                    "verbose scheduler ready set has UIDs absent from its "
                    f"dependency table: {missing}"
                )
            ready_count += 1
            continue
        if match := RESOLVED_READY.match(line):
            uid = int(match.group(1))
            if uid not in instructions:
                raise ValueError(
                    "verbose scheduler resolved UID absent from its dependency "
                    f"table: {uid}"
                )
            if uid not in current_ready:
                current_ready.append(uid)
            continue
        if match := SELECT.match(line):
            selected = int(match.group(1))
            selection_count += 1
            if selected not in current_ready:
                raise ValueError(
                    f"scheduler selected UID {selected} without a matching ready record"
                )
            ready = list(current_ready)
            current_ready.remove(selected)
            if len(ready) < 2:
                continue
            candidates = [instructions[uid] for uid in ready if uid in instructions]
            selections.append({
                "clock": current_clock,
                "selected_uid": selected,
                "ready_uids": ready,
                "candidates": candidates,
                "printed_field_heuristic": printed_field_heuristic(candidates, selected),
            })

    return {
        "block": number,
        "stage": "sched1" if reload_state == "before" else "sched2",
        "ready_snapshots": ready_count,
        "selections": selection_count,
        "instructions": [instructions[uid] for uid in instructions],
        "contested": selections,
    }


def parse_function(name: str, text: str) -> dict[str, Any]:
    blocks = [
        parse_block(int(match.group(1)), match.group(2), section)
        for match, section in split_sections(text, BLOCK)
    ]
    if (not blocks or any(
        not block["instructions"] or not block["ready_snapshots"] or not block["selections"]
        for block in blocks
    )):
        raise ValueError(
            f"no verbose scheduler records for {name}; recapture with "
            "--cflag=-fsched-verbose=5"
        )
    return {
        "function": name,
        "blocks": blocks,
        "contested_count": sum(len(block["contested"]) for block in blocks),
    }


def parse_dump(text: str, function: str | None = None) -> list[dict[str, Any]]:
    sections = [(match.group(1), section) for match, section in split_sections(text, FUNCTION)]
    if function is not None:
        sections = [section for section in sections if section[0] == function]
        if not sections:
            raise ValueError(f"function not found in dump: {function}")
    else:
        # GCC can emit a Function heading without running the scheduler for
        # that function.  Such sections are not malformed scheduler records;
        # they simply have nothing for this report to explain.
        sections = [
            section for section in sections
            if BLOCK.search(section[1]) and any(
                DEPENDENCY_ROW.match(line) for line in section[1].splitlines()
            )
        ]
    if not sections:
        raise ValueError("no verbose scheduler functions found in dump")
    return [parse_function(name, section) for name, section in sections]


def resolve_dump(probe: Path, function: str | None, stage: str) -> Path:
    filename = "17.sched" if stage == "sched1" else "25.sched2"
    if probe.is_file():
        return probe
    if not probe.is_dir():
        raise FileNotFoundError(f"not a file or directory: {probe}")
    if function:
        extracted = probe / "functions" / function / filename
        if extracted.is_file():
            return extracted
    whole = probe / f"rtl.{filename}"
    if whole.is_file():
        return whole
    raise FileNotFoundError(f"no pass-{filename} dump found under: {probe}")


def render(function: dict[str, Any], limit: int = 40) -> str:
    lines = [
        str(function["function"]),
        f"contested selections  {function['contested_count']}",
    ]
    emitted = 0
    for block in function["blocks"]:
        for choice in block["contested"]:
            if emitted >= limit:
                lines.append(
                    f"  ... {function['contested_count'] - emitted} more; use --json for all"
                )
                return "\n".join(lines)
            ready = " ".join(str(uid) for uid in choice["ready_uids"])
            lines.append(
                f"  {block['stage']} bb {block['block']} t={choice['clock']}: "
                f"uid {choice['selected_uid']} selected from {ready}"
            )
            for step in choice["printed_field_heuristic"]:
                uids = ",".join(str(uid) for uid in step["uids"])
                suffix = (
                    "" if step["selected_survives"] else
                    " (printed-field heuristic does not explain selection)"
                )
                lines.append(
                    f"    printed {step['field']} {step['preferred']}: {uids}{suffix}"
                )
            emitted += 1
    return "\n".join(lines)


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("probe", type=Path, help="probe directory or scheduler dump")
    parser.add_argument("--function", help="use or select the named function")
    parser.add_argument("--stage", choices=("sched1", "sched2"), default="sched2")
    parser.add_argument(
        "--limit", type=int, default=40,
        help="maximum choices printed per function",
    )
    parser.add_argument("--json", type=Path, help="also write a machine-readable report")
    args = parser.parse_args(argv)
    try:
        dump = resolve_dump(args.probe, args.function, args.stage)
        functions = parse_dump(dump.read_text(errors="replace"), args.function)
    except (OSError, ValueError) as error:
        print(error, file=sys.stderr)
        return 2
    report = {
        "schema": 1,
        "format": "ee-gcc-2.96-scheduler-verbose",
        "stage": args.stage,
        "dump": dump.name,
        "functions": functions,
    }
    if args.json:
        args.json.parent.mkdir(parents=True, exist_ok=True)
        args.json.write_text(json.dumps(report, indent=2, sort_keys=True) + "\n")
    print("\n\n".join(render(function, max(args.limit, 0)) for function in functions))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
