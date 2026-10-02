#!/usr/bin/env python3
"""Disassemble and assemble DDS field NPC palette (``.NPL``) tables.

NPL contains 32 or 64 fixed-size palette rows.  Source names both RGB triplets,
splits the packed texture-coordinate byte into its two nibbles, and keeps the
final direction byte explicit.  A complete default row plus sparse indexed
overrides makes the mostly-repeated retail tables concise without hiding data.
"""

from __future__ import annotations

import argparse
import re
import shlex
import sys
from dataclasses import dataclass
from pathlib import Path


ENTRY_SIZE = 8
SUPPORTED_COUNTS = (32, 64)


class NplError(ValueError):
    """Raised when an NPL binary or source file is invalid."""


@dataclass(frozen=True)
class Palette:
    primary: tuple[int, int, int]
    secondary: tuple[int, int, int]
    texture_u: int
    texture_v: int
    direction: int


@dataclass(frozen=True)
class NplFile:
    palettes: tuple[Palette, ...]


def decode(data: bytes) -> NplFile:
    """Decode one fixed-layout NPL table."""

    if len(data) % ENTRY_SIZE or len(data) // ENTRY_SIZE not in SUPPORTED_COUNTS:
        expected = " or ".join(f"{count * ENTRY_SIZE:#x}" for count in SUPPORTED_COUNTS)
        raise NplError(f"NPL file is {len(data):#x} bytes, expected {expected}")
    rows = []
    for offset in range(0, len(data), ENTRY_SIZE):
        packed = data[offset + 6]
        rows.append(
            Palette(
                tuple(data[offset : offset + 3]),  # type: ignore[arg-type]
                tuple(data[offset + 3 : offset + 6]),  # type: ignore[arg-type]
                packed >> 4,
                packed & 0xF,
                data[offset + 7],
            )
        )
    return NplFile(tuple(rows))


def encode(model: NplFile) -> bytes:
    """Encode one validated NPL table."""

    if len(model.palettes) not in SUPPORTED_COUNTS:
        expected = " or ".join(str(count) for count in SUPPORTED_COUNTS)
        raise NplError(f"NPL needs {expected} palettes, got {len(model.palettes)}")
    output = bytearray()
    for index, row in enumerate(model.palettes):
        if len(row.primary) != 3 or len(row.secondary) != 3:
            raise NplError(f"palette {index} colors need three channels")
        channels = row.primary + row.secondary
        if any(not 0 <= value <= 0xFF for value in channels):
            raise NplError(f"palette {index} color channel is outside 0..255")
        if not 0 <= row.texture_u <= 0xF or not 0 <= row.texture_v <= 0xF:
            raise NplError(f"palette {index} texture coordinate is outside 0..15")
        if not 0 <= row.direction <= 0xFF:
            raise NplError(f"palette {index} direction is outside 0..255")
        output.extend(channels)
        output.extend(((row.texture_u << 4) | row.texture_v, row.direction))
    return bytes(output)


def _tokens(line: str, line_number: int) -> list[str]:
    try:
        return shlex.split(line, comments=False)
    except ValueError as exc:
        raise NplError(f"line {line_number}: {exc}") from exc


def _integer(text: str, line_number: int, context: str) -> int:
    try:
        return int(text, 0)
    except ValueError as exc:
        raise NplError(f"line {line_number}: invalid {context} {text!r}") from exc


def _color(text: str, line_number: int, context: str) -> tuple[int, int, int]:
    if not re.fullmatch(r"#[0-9a-fA-F]{6}", text):
        raise NplError(f"line {line_number}: {context} must be #RRGGBB")
    value = int(text[1:], 16)
    return (value >> 16, (value >> 8) & 0xFF, value & 0xFF)


def _texture(text: str, line_number: int) -> tuple[int, int]:
    parts = text.split(",")
    if len(parts) != 2:
        raise NplError(f"line {line_number}: texture needs U,V")
    values = tuple(_integer(part, line_number, "texture coordinate") for part in parts)
    if any(not 0 <= value <= 0xF for value in values):
        raise NplError(f"line {line_number}: texture coordinate is outside 0..15")
    return values  # type: ignore[return-value]


def _palette(tokens: list[str], line_number: int) -> Palette:
    fields: dict[str, str] = {}
    for token in tokens:
        key, separator, value = token.partition("=")
        if not separator or not key or not value or key in fields:
            raise NplError(f"line {line_number}: invalid or duplicate field {token!r}")
        fields[key] = value
    expected = {"primary", "secondary", "texture", "direction"}
    if fields.keys() != expected:
        raise NplError(
            f"line {line_number}: palette fields must be primary, secondary, texture, direction"
        )
    texture_u, texture_v = _texture(fields["texture"], line_number)
    return Palette(
        _color(fields["primary"], line_number, "primary color"),
        _color(fields["secondary"], line_number, "secondary color"),
        texture_u,
        texture_v,
        _integer(fields["direction"], line_number, "direction"),
    )


def parse_source(text: str) -> NplFile:
    """Parse version-1 NPL source."""

    lines = [
        (number, line.strip())
        for number, line in enumerate(text.splitlines(), 1)
        if line.strip() and not line.lstrip().startswith("# ")
    ]
    if not lines:
        raise NplError("source must begin with 'npl 1 count=COUNT'")
    header = _tokens(lines[0][1], lines[0][0])
    if len(header) != 3 or header[:2] != ["npl", "1"] or not header[2].startswith("count="):
        raise NplError("source must begin with 'npl 1 count=COUNT'")
    count = _integer(header[2][6:], lines[0][0], "palette count")
    if count not in SUPPORTED_COUNTS:
        raise NplError(f"line {lines[0][0]}: palette count must be 32 or 64")

    default: Palette | None = None
    overrides: dict[int, Palette] = {}
    for line_number, line in lines[1:]:
        tokens = _tokens(line, line_number)
        if not tokens:
            continue
        if tokens[0] == "default":
            if default is not None:
                raise NplError(f"line {line_number}: duplicate default palette")
            default = _palette(tokens[1:], line_number)
        elif tokens[0] == "palette":
            if len(tokens) < 3:
                raise NplError(f"line {line_number}: palette needs an index and fields")
            index = _integer(tokens[1], line_number, "palette index")
            if not 0 <= index < count:
                raise NplError(
                    f"line {line_number}: palette index {index} is outside 0..{count - 1}"
                )
            if index in overrides:
                raise NplError(f"line {line_number}: duplicate palette {index}")
            overrides[index] = _palette(tokens[2:], line_number)
        else:
            raise NplError(f"line {line_number}: unknown directive {tokens[0]!r}")
    if default is None:
        raise NplError("source needs one default palette")
    rows = [default] * count
    for index, row in overrides.items():
        rows[index] = row
    model = NplFile(tuple(rows))
    encode(model)
    return model


def _color_text(color: tuple[int, int, int]) -> str:
    return f"#{color[0]:02x}{color[1]:02x}{color[2]:02x}"


def _row_text(directive: str, row: Palette) -> str:
    return (
        f"{directive} primary={_color_text(row.primary)} "
        f"secondary={_color_text(row.secondary)} "
        f"texture={row.texture_u},{row.texture_v} direction={row.direction}"
    )


def render_source(data: bytes) -> str:
    """Render canonical sparse source for an NPL binary."""

    model = decode(data)
    counts: dict[Palette, int] = {}
    first: dict[Palette, int] = {}
    for index, row in enumerate(model.palettes):
        counts[row] = counts.get(row, 0) + 1
        first.setdefault(row, index)
    default = max(counts, key=lambda row: (counts[row], -first[row]))
    lines = [f"npl 1 count={len(model.palettes)}", "", _row_text("default", default)]
    overrides = [
        _row_text(f"palette {index}", row)
        for index, row in enumerate(model.palettes)
        if row != default
    ]
    if overrides:
        lines.extend(("", *overrides))
    return "\n".join(lines) + "\n"


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    commands = parser.add_subparsers(dest="command", required=True)
    disassemble = commands.add_parser("disassemble", help="write readable NPL source")
    disassemble.add_argument("input", type=Path)
    disassemble.add_argument("output", nargs="?", type=Path)
    assemble = commands.add_parser("assemble", help="assemble readable NPL source")
    assemble.add_argument("input", type=Path)
    assemble.add_argument("output", type=Path)
    verify = commands.add_parser("verify", help="validate an NPL file and its exact rewrite")
    verify.add_argument("input", type=Path)
    args = parser.parse_args()

    try:
        if args.command == "disassemble":
            output = render_source(args.input.read_bytes())
            if args.output:
                args.output.parent.mkdir(parents=True, exist_ok=True)
                args.output.write_text(output, encoding="utf-8")
            else:
                sys.stdout.write(output)
        elif args.command == "assemble":
            output = encode(parse_source(args.input.read_text(encoding="utf-8")))
            args.output.parent.mkdir(parents=True, exist_ok=True)
            args.output.write_bytes(output)
        else:
            data = args.input.read_bytes()
            if encode(decode(data)) != data:
                raise NplError("NPL rewrite is not byte-identical")
            print(f"{args.input}: NPL OK ({len(data)} bytes)")
    except (OSError, NplError) as exc:
        parser.error(str(exc))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
