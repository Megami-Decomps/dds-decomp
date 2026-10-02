#!/usr/bin/env python3
"""Disassemble and assemble DDS field sky-light (``.SKY``) tables.

SKY contains 256 fixed-size renderer light sets.  Source exposes the fields
consumed by the field renderer and retains the one unclassified 48-byte region
verbatim.  Each file declares a complete default set followed by sparse indexed
overrides, matching the way retail tables repeat a small number of settings.
"""

from __future__ import annotations

import argparse
import math
import shlex
import struct
import sys
from dataclasses import dataclass
from pathlib import Path


SET_COUNT = 256
SET_SIZE = 0xE0
FILE_SIZE = SET_COUNT * SET_SIZE
RESERVED_SIZE = 0x30


class SkyError(ValueError):
    """Raised when a SKY binary or source file is invalid."""


@dataclass(frozen=True)
class Light:
    color: tuple[int, int, int]
    direction: tuple[int, int, int]


@dataclass(frozen=True)
class LightSet:
    type_id: int
    display: int
    fade: int
    sway: int
    draw_vector: tuple[int, int, int, int]
    draw_color: tuple[int, int, int]
    lights: tuple[Light, Light, Light]
    background: tuple[int, int, int]
    unit_color_a: tuple[int, int, int]
    unit_direction: tuple[int, int, int]
    reserved: bytes
    unit_color_b: tuple[int, int, int]


@dataclass(frozen=True)
class SkyFile:
    sets: tuple[LightSet, ...]


def _u32s(data: bytes, offset: int, count: int) -> tuple[int, ...]:
    return struct.unpack_from(f"<{count}I", data, offset)


def decode(data: bytes) -> SkyFile:
    """Decode and validate one fixed-layout SKY table."""

    if len(data) != FILE_SIZE:
        raise SkyError(f"SKY file is {len(data):#x} bytes, expected {FILE_SIZE:#x}")
    sets = []
    for index in range(SET_COUNT):
        base = index * SET_SIZE
        if any(data[base + 1 : base + 4]):
            raise SkyError(f"light set {index} has nonzero padding at 0x01")
        if any(data[base + 0x10 : base + 0x1C]):
            raise SkyError(f"light set {index} has nonzero padding at 0x10")
        display, fade, sway = struct.unpack_from("<iii", data, base + 4)
        packed_vector = struct.unpack_from("<iiii", data, base + 0x1C)
        # Retail stores X, Z, Y, W even though the renderer consumes X, Y, Z, W.
        draw_vector = (packed_vector[0], packed_vector[2], packed_vector[1], packed_vector[3])
        draw_color = struct.unpack_from("<iii", data, base + 0x2C)
        lights = tuple(
            Light(
                _u32s(data, base + 0x38 + light_index * 0x18, 3),
                _u32s(data, base + 0x44 + light_index * 0x18, 3),
            )
            for light_index in range(3)
        )
        sets.append(
            LightSet(
                data[base],
                display,
                fade,
                sway,
                draw_vector,
                draw_color,
                lights,  # type: ignore[arg-type]
                _u32s(data, base + 0x80, 3),
                _u32s(data, base + 0x8C, 3),
                _u32s(data, base + 0x98, 3),
                data[base + 0xA4 : base + 0xD4],
                _u32s(data, base + 0xD4, 3),
            )
        )
    return SkyFile(tuple(sets))


def _range(value: int, minimum: int, maximum: int, context: str) -> int:
    if not minimum <= value <= maximum:
        raise SkyError(f"{context} {value} is outside {minimum}..{maximum}")
    return value


def _pack_s32(values: tuple[int, ...], context: str) -> bytes:
    for value in values:
        _range(value, -0x80000000, 0x7FFFFFFF, context)
    return struct.pack(f"<{len(values)}i", *values)


def _pack_floats(values: tuple[int, ...]) -> bytes:
    if len(values) != 3:
        raise SkyError("float vector needs three components")
    for value in values:
        _range(value, 0, 0xFFFFFFFF, "float bits")
    return struct.pack("<3I", *values)


def encode(model: SkyFile) -> bytes:
    """Encode one validated SKY table."""

    if len(model.sets) != SET_COUNT:
        raise SkyError(f"SKY needs {SET_COUNT} light sets, got {len(model.sets)}")
    output = bytearray(FILE_SIZE)
    for index, light_set in enumerate(model.sets):
        base = index * SET_SIZE
        output[base] = _range(light_set.type_id, 0, 0xFF, f"light set {index} type")
        output[base + 4 : base + 0x10] = _pack_s32(
            (light_set.display, light_set.fade, light_set.sway),
            f"light set {index} state",
        )
        x, y, z, w = light_set.draw_vector
        output[base + 0x1C : base + 0x2C] = _pack_s32(
            (x, z, y, w), f"light set {index} draw vector"
        )
        output[base + 0x2C : base + 0x38] = _pack_s32(
            light_set.draw_color, f"light set {index} draw color"
        )
        if len(light_set.lights) != 3:
            raise SkyError(f"light set {index} needs three directional lights")
        for light_index, light in enumerate(light_set.lights):
            output[
                base + 0x38 + light_index * 0x18 : base + 0x44 + light_index * 0x18
            ] = _pack_floats(light.color)
            output[
                base + 0x44 + light_index * 0x18 : base + 0x50 + light_index * 0x18
            ] = _pack_floats(light.direction)
        output[base + 0x80 : base + 0x8C] = _pack_floats(light_set.background)
        output[base + 0x8C : base + 0x98] = _pack_floats(light_set.unit_color_a)
        output[base + 0x98 : base + 0xA4] = _pack_floats(light_set.unit_direction)
        if len(light_set.reserved) != RESERVED_SIZE:
            raise SkyError(f"light set {index} reserved region needs {RESERVED_SIZE} bytes")
        output[base + 0xA4 : base + 0xD4] = light_set.reserved
        output[base + 0xD4 : base + 0xE0] = _pack_floats(light_set.unit_color_b)
    return bytes(output)


def _tokens(line: str, line_number: int) -> list[str]:
    try:
        return shlex.split(line, comments=True)
    except ValueError as exc:
        raise SkyError(f"line {line_number}: {exc}") from exc


def _integer(text: str, line_number: int, context: str) -> int:
    try:
        return int(text, 0)
    except ValueError as exc:
        raise SkyError(f"line {line_number}: invalid {context} {text!r}") from exc


def _fields(tokens: list[str], line_number: int) -> dict[str, str]:
    result: dict[str, str] = {}
    for token in tokens:
        key, separator, value = token.partition("=")
        if not separator or not key or not value or key in result:
            raise SkyError(f"line {line_number}: invalid or duplicate field {token!r}")
        result[key] = value
    return result


def _f32_bits(text: str, line_number: int) -> int:
    if text.startswith("bits:0x"):
        try:
            value = int(text[7:], 16)
        except ValueError as exc:
            raise SkyError(f"line {line_number}: invalid float bits {text!r}") from exc
        return _range(value, 0, 0xFFFFFFFF, f"line {line_number}: float bits")
    try:
        return struct.unpack("<I", struct.pack("<f", float(text)))[0]
    except (ValueError, OverflowError, struct.error) as exc:
        raise SkyError(f"line {line_number}: invalid float {text!r}") from exc


def _vector(
    text: str, line_number: int, context: str, count: int, *, floats: bool
) -> tuple[int, ...]:
    parts = text.split(",")
    if len(parts) != count:
        raise SkyError(f"line {line_number}: {context} needs {count} comma-separated values")
    if floats:
        return tuple(_f32_bits(part, line_number) for part in parts)
    values = tuple(_integer(part, line_number, context) for part in parts)
    for value in values:
        _range(value, -0x80000000, 0x7FFFFFFF, f"line {line_number}: {context}")
    return values


def _parse_set(
    lines: list[tuple[int, str]], start: int
) -> tuple[LightSet, int]:
    state: tuple[int, int, int, int] | None = None
    draw_vector: tuple[int, ...] | None = None
    draw_color: tuple[int, ...] | None = None
    lights: dict[int, Light] = {}
    background: tuple[int, ...] | None = None
    unit_color_a: tuple[int, ...] | None = None
    unit_direction: tuple[int, ...] | None = None
    reserved: bytes | None = None
    unit_color_b: tuple[int, ...] | None = None
    seen: set[str] = set()
    cursor = start
    while cursor < len(lines):
        line_number, line = lines[cursor]
        tokens = _tokens(line, line_number)
        if tokens == ["end"]:
            break
        if not tokens:
            cursor += 1
            continue
        directive = tokens[0]
        key = directive
        if directive == "light":
            if len(tokens) < 2:
                raise SkyError(f"line {line_number}: light needs an index")
            light_index = _integer(tokens[1], line_number, "light index")
            if not 0 <= light_index < 3:
                raise SkyError(f"line {line_number}: light index is outside 0..2")
            key = f"light{light_index}"
        if key in seen:
            raise SkyError(f"line {line_number}: duplicate {directive} directive")
        seen.add(key)

        if directive == "state":
            fields = _fields(tokens[1:], line_number)
            expected = {"type", "display", "fade", "sway"}
            if fields.keys() != expected:
                raise SkyError(f"line {line_number}: state needs type, display, fade, sway")
            state = (
                _range(_integer(fields["type"], line_number, "type"), 0, 0xFF, "type"),
                _integer(fields["display"], line_number, "display"),
                _integer(fields["fade"], line_number, "fade"),
                _integer(fields["sway"], line_number, "sway"),
            )
        elif directive == "draw_vector" and len(tokens) == 2:
            draw_vector = _vector(tokens[1], line_number, "draw vector", 4, floats=False)
        elif directive == "draw_color" and len(tokens) == 2:
            draw_color = _vector(tokens[1], line_number, "draw color", 3, floats=False)
        elif directive == "light":
            fields = _fields(tokens[2:], line_number)
            if fields.keys() != {"color", "direction"}:
                raise SkyError(f"line {line_number}: light needs color and direction")
            lights[light_index] = Light(
                _vector(fields["color"], line_number, "light color", 3, floats=True),  # type: ignore[arg-type]
                _vector(fields["direction"], line_number, "light direction", 3, floats=True),  # type: ignore[arg-type]
            )
        elif directive == "background" and len(tokens) == 2:
            background = _vector(tokens[1], line_number, "background", 3, floats=True)
        elif directive == "unit_color_a" and len(tokens) == 2:
            unit_color_a = _vector(tokens[1], line_number, "unit color A", 3, floats=True)
        elif directive == "unit_direction" and len(tokens) == 2:
            unit_direction = _vector(tokens[1], line_number, "unit direction", 3, floats=True)
        elif directive == "reserved" and len(tokens) == 2:
            try:
                reserved = bytes.fromhex(tokens[1])
            except ValueError as exc:
                raise SkyError(f"line {line_number}: reserved data is not hexadecimal") from exc
            if len(reserved) != RESERVED_SIZE:
                raise SkyError(
                    f"line {line_number}: reserved data is {len(reserved)} bytes, expected {RESERVED_SIZE}"
                )
        elif directive == "unit_color_b" and len(tokens) == 2:
            unit_color_b = _vector(tokens[1], line_number, "unit color B", 3, floats=True)
        else:
            raise SkyError(f"line {line_number}: invalid {directive} directive")
        cursor += 1

    if cursor == len(lines):
        raise SkyError(f"line {lines[start - 1][0]}: light set has no end")
    required = {
        "state",
        "draw_vector",
        "draw_color",
        "light0",
        "light1",
        "light2",
        "background",
        "unit_color_a",
        "unit_direction",
        "reserved",
        "unit_color_b",
    }
    if seen != required:
        missing = ", ".join(sorted(required - seen))
        extra = ", ".join(sorted(seen - required))
        detail = f"missing {missing}" if missing else f"unexpected {extra}"
        raise SkyError(f"line {lines[start - 1][0]}: incomplete light set ({detail})")
    assert state is not None
    assert draw_vector is not None and draw_color is not None
    assert background is not None and unit_color_a is not None
    assert unit_direction is not None and reserved is not None and unit_color_b is not None
    result = LightSet(
        *state,
        draw_vector,  # type: ignore[arg-type]
        draw_color,  # type: ignore[arg-type]
        (lights[0], lights[1], lights[2]),
        background,  # type: ignore[arg-type]
        unit_color_a,  # type: ignore[arg-type]
        unit_direction,  # type: ignore[arg-type]
        reserved,
        unit_color_b,  # type: ignore[arg-type]
    )
    return result, cursor


def parse_source(text: str) -> SkyFile:
    """Parse version-1 SKY source."""

    lines = [
        (number, line.strip())
        for number, line in enumerate(text.splitlines(), 1)
        if line.strip() and not line.lstrip().startswith("#")
    ]
    if not lines or _tokens(lines[0][1], lines[0][0]) != ["sky", "1"]:
        raise SkyError("source must begin with 'sky 1'")
    default: LightSet | None = None
    overrides: dict[int, LightSet] = {}
    cursor = 1
    while cursor < len(lines):
        line_number, line = lines[cursor]
        tokens = _tokens(line, line_number)
        if tokens == ["default"]:
            if default is not None:
                raise SkyError(f"line {line_number}: duplicate default light set")
            default, cursor = _parse_set(lines, cursor + 1)
        elif len(tokens) == 2 and tokens[0] == "set":
            index = _integer(tokens[1], line_number, "light-set index")
            if not 0 <= index < SET_COUNT:
                raise SkyError(f"line {line_number}: light-set index is outside 0..255")
            if index in overrides:
                raise SkyError(f"line {line_number}: duplicate light set {index}")
            overrides[index], cursor = _parse_set(lines, cursor + 1)
        else:
            raise SkyError(f"line {line_number}: expected default or set INDEX")
        cursor += 1
    if default is None:
        raise SkyError("source needs one default light set")
    sets = [default] * SET_COUNT
    for index, light_set in overrides.items():
        sets[index] = light_set
    model = SkyFile(tuple(sets))
    encode(model)
    return model


def _float_text(bits: int) -> str:
    value = struct.unpack("<f", struct.pack("<I", bits))[0]
    if math.isfinite(value):
        for precision in range(1, 10):
            text = format(value, f".{precision}g")
            if "e" not in text.lower() and "." not in text:
                text += ".0"
            if struct.unpack("<I", struct.pack("<f", float(text)))[0] == bits:
                return text
    return f"bits:0x{bits:08x}"


def _vector_text(values: tuple[int, ...], *, floats: bool) -> str:
    formatter = _float_text if floats else str
    return ",".join(formatter(value) for value in values)


def _set_lines(directive: str, light_set: LightSet) -> list[str]:
    lines = [
        directive,
        f"  state type={light_set.type_id} display={light_set.display} "
        f"fade={light_set.fade} sway={light_set.sway}",
        f"  draw_vector {_vector_text(light_set.draw_vector, floats=False)}",
        f"  draw_color {_vector_text(light_set.draw_color, floats=False)}",
    ]
    for index, light in enumerate(light_set.lights):
        lines.append(
            f"  light {index} color={_vector_text(light.color, floats=True)} "
            f"direction={_vector_text(light.direction, floats=True)}"
        )
    lines.extend(
        (
            f"  background {_vector_text(light_set.background, floats=True)}",
            f"  unit_color_a {_vector_text(light_set.unit_color_a, floats=True)}",
            f"  unit_direction {_vector_text(light_set.unit_direction, floats=True)}",
            f"  reserved {light_set.reserved.hex()}",
            f"  unit_color_b {_vector_text(light_set.unit_color_b, floats=True)}",
            "end",
        )
    )
    return lines


def render_source(data: bytes) -> str:
    """Render canonical sparse source for a SKY binary."""

    model = decode(data)
    counts: dict[LightSet, int] = {}
    first: dict[LightSet, int] = {}
    for index, light_set in enumerate(model.sets):
        counts[light_set] = counts.get(light_set, 0) + 1
        first.setdefault(light_set, index)
    default = max(counts, key=lambda row: (counts[row], -first[row]))
    lines = ["sky 1", "", *_set_lines("default", default)]
    for index, light_set in enumerate(model.sets):
        if light_set == default:
            continue
        lines.extend(("", *_set_lines(f"set {index}", light_set)))
    return "\n".join(lines) + "\n"


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    commands = parser.add_subparsers(dest="command", required=True)
    disassemble = commands.add_parser("disassemble", help="write readable SKY source")
    disassemble.add_argument("input", type=Path)
    disassemble.add_argument("output", nargs="?", type=Path)
    assemble = commands.add_parser("assemble", help="assemble readable SKY source")
    assemble.add_argument("input", type=Path)
    assemble.add_argument("output", type=Path)
    verify = commands.add_parser("verify", help="validate a SKY file and its exact rewrite")
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
                raise SkyError("SKY rewrite is not byte-identical")
            print(f"{args.input}: SKY OK ({len(data)} bytes)")
    except (OSError, SkyError) as exc:
        parser.error(str(exc))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
