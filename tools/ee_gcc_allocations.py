#!/usr/bin/env python3
"""Explain register-allocation records in an EE GCC 2.96 pass-20 dump."""

from __future__ import annotations

import argparse
import json
import re
import sys
from pathlib import Path
from typing import Any


FIRST_PSEUDO_REGISTER = 79
DEBUG_REG_NAMES = (
    ["$0", "at", "v0", "v1", "a0", "a1", "a2", "a3"]
    + [f"t{i}" for i in range(8)]
    + [f"s{i}" for i in range(8)]
    + ["t8", "t9", "k0", "k1", "gp", "sp", "$fp", "ra"]
    + [f"$f{i}" for i in range(32)]
    + ["hi", "lo", "accum"]
    + [f"$fcc{i}" for i in range(8)]
    + ["$rap", "hi1", "lo1", "accum1"]
)

FUNCTION = re.compile(r"(?m)^;; Function (.+?)\s*$")
ALLOCATE = re.compile(r"(?m)^;; (\d+) regs to allocate:(.*)$")
RELATION = re.compile(r"(?m)^;; (\d+) (conflicts|preferences):(.*)$")
ALLOC_ENTRY = re.compile(r"(?<![+\d])(\d+)(?:\s+\((\d+)\))?")
RELATION_ENTRY = re.compile(r"(?<![+\d])(\d+)(?:\+\d+)?(?:\s+\(\d+\))?")
DISPOSITION_BLOCK = re.compile(
    r"(?ms)^;; Register dispositions:\s*\n(.*?)(?=^;; Hard regs used:)"
)
DISPOSITION = re.compile(r"(\d+) in (-?\d+)")
HARD_USED = re.compile(r"(?m)^;; Hard regs used:\s*(.*)$")
SPILL = re.compile(r"(?m)^Spilling for insn (\d+)\.$")
RELOAD = re.compile(r"(?m)^Reloads for insn # (\d+)\s*$")


def register_name(regno: int | None) -> str | None:
    if regno is None or regno < 0:
        return None
    if regno < len(DEBUG_REG_NAMES):
        return DEBUG_REG_NAMES[regno]
    return f"r{regno}"


def split_functions(text: str) -> list[tuple[str, str]]:
    matches = list(FUNCTION.finditer(text))
    return [
        (match.group(1), text[match.start(): matches[index + 1].start()
                              if index + 1 < len(matches) else len(text)])
        for index, match in enumerate(matches)
    ]


def allocation_entries(text: str) -> list[tuple[int, int]]:
    return [
        (int(match.group(1)), int(match.group(2) or 1))
        for match in ALLOC_ENTRY.finditer(text)
    ]


def relation_entries(text: str) -> list[int]:
    return [int(match.group(1)) for match in RELATION_ENTRY.finditer(text)]


def parse_allocation_attempts(text: str) -> list[dict[str, Any]]:
    headings = list(ALLOCATE.finditer(text))
    attempts = []
    disposition = DISPOSITION_BLOCK.search(text)
    for index, heading in enumerate(headings):
        end = headings[index + 1].start() if index + 1 < len(headings) else (
            disposition.start() if disposition and disposition.start() > heading.start() else len(text)
        )
        section = text[heading.start():end]
        declared_count = int(heading.group(1))
        entries = allocation_entries(heading.group(2))
        relations: dict[int, dict[str, list[int]]] = {}
        for match in RELATION.finditer(section):
            pseudo = int(match.group(1))
            relations.setdefault(pseudo, {})[match.group(2)] = relation_entries(match.group(3))
        attempts.append({
            "index": index,
            "declared_count": declared_count,
            "allocation_order": [pseudo for pseudo, _ in entries],
            "hard_register_widths": {
                str(pseudo): width for pseudo, width in entries if width != 1
            },
            "count_matches": len(entries) == declared_count,
            "relations": relations,
        })
    return attempts


def parse_function(name: str, text: str) -> dict[str, Any]:
    attempts = parse_allocation_attempts(text)
    final_attempt = attempts[-1] if attempts else None
    allocation_order = final_attempt["allocation_order"] if final_attempt else []
    relations = final_attempt["relations"] if final_attempt else {}

    dispositions: dict[int, int] = {}
    block = DISPOSITION_BLOCK.search(text)
    if block:
        dispositions = {
            int(pseudo): int(hard_reg)
            for pseudo, hard_reg in DISPOSITION.findall(block.group(1))
        }

    hard_used_match = HARD_USED.search(text)
    hard_used = relation_entries(hard_used_match.group(1)) if hard_used_match else []
    pseudos = sorted(set(allocation_order) | set(relations) | set(dispositions))
    rows = []
    for pseudo in pseudos:
        conflicts = relations.get(pseudo, {}).get("conflicts", [])
        preferences = relations.get(pseudo, {}).get("preferences", [])
        selected = dispositions.get(pseudo)
        rows.append({
            "pseudo": pseudo,
            "allocation_kind": (
                "global_representative" if pseudo in allocation_order else "other_disposition"
            ),
            "global_order": allocation_order.index(pseudo) if pseudo in allocation_order else None,
            "selected": selected,
            "selected_name": register_name(selected),
            "hard_conflicts": [value for value in conflicts if value < FIRST_PSEUDO_REGISTER],
            "pseudo_conflicts": [value for value in conflicts if value >= FIRST_PSEUDO_REGISTER],
            "hard_preferences": [value for value in preferences if value < FIRST_PSEUDO_REGISTER],
            "pseudo_preferences": [value for value in preferences if value >= FIRST_PSEUDO_REGISTER],
        })

    return {
        "function": name,
        "allocation_attempts": attempts,
        "retry_count": max(len(attempts) - 1, 0),
        "global_allocation_order": allocation_order,
        "spills": [int(value) for value in SPILL.findall(text)],
        "reloads": [int(value) for value in RELOAD.findall(text)],
        "hard_regs_used": [
            {"regno": regno, "name": register_name(regno)} for regno in hard_used
        ],
        "pseudos": rows,
    }


def parse_dump(text: str, function: str | None = None) -> list[dict[str, Any]]:
    sections = split_functions(text)
    if function is not None:
        sections = [section for section in sections if section[0] == function]
        if not sections:
            raise ValueError(f"function not found in dump: {function}")
    return [parse_function(name, section) for name, section in sections]


def resolve_dump(probe: Path, function: str | None) -> Path:
    if probe.is_file():
        return probe
    if not probe.is_dir():
        raise FileNotFoundError(f"not a file or directory: {probe}")
    if function:
        extracted = probe / "functions" / function / "20.greg"
        if extracted.is_file():
            return extracted
    whole = probe / "rtl.20.greg"
    if whole.is_file():
        return whole
    raise FileNotFoundError(f"no pass-20 dump found under: {probe}")


def format_registers(regnos: list[int]) -> str:
    return ",".join(register_name(regno) or str(regno) for regno in regnos) or "-"


def render(report: dict[str, Any]) -> str:
    lines = [str(report["function"])]
    attempts = report["allocation_attempts"]
    if len(attempts) > 1:
        lines.append(f"allocator attempts  {len(attempts)} ({report['retry_count']} retries)")
    order = report["global_allocation_order"]
    lines.append("global order  " + (" ".join(f"r{pseudo}" for pseudo in order) or "(none)"))
    lines.append("pseudo\tkind/order\tselected\thard conflicts\thard preferences")
    for row in report["pseudos"]:
        selected = row["selected_name"] or row["selected"] or "-"
        kind = "other" if row["global_order"] is None else f"global/{row['global_order']}"
        lines.append(
            f"r{row['pseudo']}\t{kind}\t{selected}\t"
            f"{format_registers(row['hard_conflicts'])}\t"
            f"{format_registers(row['hard_preferences'])}"
        )
    if report["spills"]:
        lines.append("spill insns   " + " ".join(str(uid) for uid in report["spills"]))
    if report["reloads"]:
        lines.append("reload insns  " + " ".join(str(uid) for uid in report["reloads"]))
    return "\n".join(lines)


def parse_args(argv: list[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("probe", type=Path, help="probe directory or pass-20 dump")
    parser.add_argument("--function", help="use or select the named function")
    parser.add_argument("--json", type=Path, help="also write a machine-readable report")
    return parser.parse_args(argv)


def main(argv: list[str] | None = None) -> int:
    args = parse_args(argv)
    try:
        dump = resolve_dump(args.probe, args.function)
        functions = parse_dump(dump.read_text(errors="replace"), args.function)
    except (FileNotFoundError, ValueError) as error:
        print(error, file=sys.stderr)
        return 2
    report = {
        "schema": 1,
        "format": "ee-gcc-2.96-rtl",
        "stage": "20.greg",
        "dump": str(dump),
        "functions": functions,
    }
    if args.json:
        args.json.parent.mkdir(parents=True, exist_ok=True)
        args.json.write_text(json.dumps(report, indent=2, sort_keys=True) + "\n")
    print("\n\n".join(render(function) for function in functions))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
