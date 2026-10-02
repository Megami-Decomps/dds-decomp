#!/usr/bin/env python3
"""Report source names and final homes from an EE GCC ``-gstabs`` probe.

The old compiler emits its variable records after each function.  They are a
useful name-to-location hint, but are not RTL pseudo identities or location
lifetimes.
"""

from __future__ import annotations

import argparse
import json
import re
import sys
from pathlib import Path
from typing import Any


STABS = re.compile(
    r'^\s*\.stabs\s+"((?:\\.|[^"\\])*)",\s*'
    r'(-?\d+),\s*(-?\d+),\s*(-?\d+),\s*(\S+)\s*$'
)
GPR_NAMES = (
    "$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3",
    "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
    "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
    "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra",
)


class SourceLocationError(ValueError):
    pass


def unescape_stabs_string(value: str) -> str:
    """Decode the quoting needed by the emitted assembler directive."""
    return re.sub(r"\\([\\\"])", r"\1", value)


def split_symbol(value: str) -> tuple[str, str] | None:
    if ":" not in value:
        return None
    name, description = value.split(":", 1)
    if not name or not description:
        return None
    return name, description


def debug_register_name(number: int) -> str | None:
    if 0 <= number < len(GPR_NAMES):
        return GPR_NAMES[number]
    # The MIPS backend's DBX map reserves 32--37 and maps f0--f31 here.
    if 38 <= number <= 69:
        return f"$f{number - 38}"
    return None


def parse_value(value: str) -> int | None:
    try:
        return int(value, 0)
    except ValueError:
        return None


def location_record(stab_type: int, name: str, description: str,
                    value: int, order: int) -> dict[str, Any] | None:
    descriptor = description[0]
    if stab_type == 64 and descriptor in {"P", "r"}:  # N_RSYM
        kind = "parameter" if descriptor == "P" else "local"
        return {
            "name": name,
            "kind": kind,
            "storage": "register",
            "symbol_descriptor": descriptor,
            "type": description[1:],
            "stab_type": stab_type,
            "value": value,
            "record_order": order,
            "debug_register": value,
            "register": debug_register_name(value),
            "stack_offset": None,
            "stack_base": None,
        }
    if stab_type == 160 and descriptor == "p":  # N_PSYM
        return {
            "name": name,
            "kind": "parameter",
            "storage": "stack",
            "symbol_descriptor": descriptor,
            "type": description[1:],
            "stab_type": stab_type,
            "value": value,
            "record_order": order,
            "debug_register": None,
            "register": None,
            "stack_offset": value,
            "stack_base": "virtual_frame_pointer",
        }
    # N_LSYM is also used for typedefs.  A real stack local has no symbolic
    # descriptor, so its type starts with a type number or parenthesized type.
    if stab_type == 128 and descriptor in "-0123456789(":
        return {
            "name": name,
            "kind": "local",
            "storage": "stack",
            "symbol_descriptor": None,
            "type": description,
            "stab_type": stab_type,
            "value": value,
            "record_order": order,
            "debug_register": None,
            "register": None,
            "stack_offset": value,
            "stack_base": "virtual_frame_pointer",
        }
    return None


def parse_assembly(text: str, function: str | None = None) -> list[dict[str, Any]]:
    functions: list[dict[str, Any]] = []
    current: dict[str, Any] | None = None
    order = 0
    for line in text.splitlines():
        match = STABS.match(line)
        if not match:
            continue
        symbol = split_symbol(unescape_stabs_string(match.group(1)))
        if symbol is None:
            continue
        name, description = symbol
        stab_type = int(match.group(2))
        value = parse_value(match.group(5))
        if stab_type == 36 and description[0] in {"F", "f"}:  # N_FUN
            current = {"function": name, "locations": []}
            functions.append(current)
            order = 0
            continue
        if current is None or value is None:
            continue
        record = location_record(stab_type, name, description, value, order)
        order += 1
        if record is not None:
            current["locations"].append(record)

    if function is not None:
        functions = [item for item in functions if item["function"] == function]
        if not functions:
            raise SourceLocationError(f"function not found in STABS records: {function}")
    return functions


def parse_probe(probe: Path, function: str | None = None) -> dict[str, Any]:
    if not probe.is_dir():
        raise SourceLocationError(f"probe directory not found: {probe}")
    manifest_path = probe / "manifest.json"
    assembly_path = probe / "candidate.s"
    if not manifest_path.is_file():
        raise SourceLocationError(f"probe manifest not found: {manifest_path}")
    try:
        manifest = json.loads(manifest_path.read_text())
    except (OSError, json.JSONDecodeError) as error:
        raise SourceLocationError(f"invalid probe manifest: {error}") from error
    if not isinstance(manifest, dict):
        raise SourceLocationError("invalid probe manifest: expected a JSON object")
    flags = manifest.get("extra_cflags")
    if not isinstance(flags, list) or not any(
        isinstance(flag, str) and flag.startswith("-gstabs") for flag in flags
    ):
        raise SourceLocationError("probe manifest does not record a -gstabs compiler flag")
    if manifest.get("cc1_succeeded") is not True:
        raise SourceLocationError("probe manifest does not record a successful compiler run")
    if not assembly_path.is_file():
        raise SourceLocationError(f"probe assembly not found: {assembly_path}")
    functions = parse_assembly(assembly_path.read_text(errors="replace"), function)
    if not functions:
        raise SourceLocationError("no function STABS records found in probe assembly")
    return {"schema": 1, "functions": functions}


def format_location(row: dict[str, Any]) -> str:
    if row["storage"] == "register":
        name = row["register"] or f"debug-register-{row['debug_register']}"
        return f"{name} (STABS {row['debug_register']})"
    offset = row["stack_offset"]
    return f"virtual-fp{offset:+d}"


def render(report: dict[str, Any]) -> str:
    sections = []
    for function in report["functions"]:
        lines = [function["function"], "name\tkind\tlocation\ttype"]
        if not function["locations"]:
            lines.append("(no parameter/local locations emitted)")
        for row in function["locations"]:
            lines.append(
                f"{row['name']}\t{row['kind']}\t{format_location(row)}\t{row['type']}"
            )
        sections.append("\n".join(lines))
    return "\n\n".join(sections)


def parse_args(argv: list[str] | None = None) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("probe", type=Path, help="probe directory captured with -gstabs")
    parser.add_argument("--function", help="report only this function")
    parser.add_argument("--json", type=Path, help="write the machine-readable report")
    return parser.parse_args(argv)


def main(argv: list[str] | None = None) -> int:
    args = parse_args(argv)
    try:
        report = parse_probe(args.probe, args.function)
    except SourceLocationError as error:
        print(f"error: {error}", file=sys.stderr)
        return 2
    print(render(report))
    if args.json:
        args.json.write_text(json.dumps(report, indent=2, sort_keys=True) + "\n")
    return 0


if __name__ == "__main__":
    sys.exit(main())
