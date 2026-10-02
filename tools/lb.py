#!/usr/bin/env python3
"""Read, rebuild, and edit DDS/Nocturne ``.LB`` resource archives.

LB archives are ordered, 64-byte-aligned resource blocks.  Retail blocks use
the SDF byte-stream codec, but the format also accepts uncompressed blocks.
The source format describes the archive and names rebuilt resources while a
user-supplied retail archive provides entries that have not been decompiled.

Assembly preserves a retail block byte-for-byte when its rebuilt resource is
unchanged.  A changed resource is deterministically compressed.  The normal
source build is therefore exact without checking retail payloads into the
repository, while edits still produce a complete format-valid LB archive.
"""

from __future__ import annotations

import argparse
import hashlib
import re
import shlex
import struct
import sys
from collections import defaultdict
from dataclasses import dataclass
from pathlib import Path


ALIGNMENT = 0x40
HEADER_SIZE = 0x10
MAX_COUNT = 0xFFFF
END_HEADER = bytes.fromhex("ff00000010000000454e443000000000")


class LbError(ValueError):
    """Raised when an LB archive, compressed stream, or source is invalid."""


@dataclass(frozen=True)
class Entry:
    index: int
    offset: int
    type_id: int
    compressed: bool
    user_id: int
    extension: str
    raw_size: int
    stored: bytes
    block: bytes


@dataclass(frozen=True)
class Archive:
    entries: tuple[Entry, ...]
    ending: bytes


@dataclass(frozen=True)
class SourceEntry:
    type_id: int
    compressed: bool
    user_id: int
    extension: str
    raw_size: int
    retail_sha1: str
    source: str


@dataclass(frozen=True)
class Source:
    base_sha1: str
    base_size: int
    entries: tuple[SourceEntry, ...]


def _align(value: int) -> int:
    return (value + ALIGNMENT - 1) & -ALIGNMENT


def _sha1(data: bytes) -> str:
    return hashlib.sha1(data).hexdigest()


def _read(data: bytes, offset: int, size: int, context: str) -> bytes:
    if offset < 0 or size < 0 or offset + size > len(data):
        raise LbError(f"truncated {context} at 0x{offset:x}")
    return data[offset : offset + size]


def decompress(data: bytes, raw_size: int) -> bytes:
    """Decode one stored SDF stream and require its conventional terminator."""

    if raw_size < 0:
        raise LbError("negative decompressed size")
    output = bytearray()
    cursor = 0

    def take(size: int, context: str) -> bytes:
        nonlocal cursor
        chunk = _read(data, cursor, size, context)
        cursor += size
        return chunk

    while len(output) < raw_size:
        header = take(1, "compression opcode")[0]
        if header == 0xFF:
            raise LbError("compressed stream ends before its declared size")
        opcode = header >> 5
        count = header & 0x1F
        if count == 0:
            count = struct.unpack("<H", take(2, "extended opcode length"))[0]
            if count == 0:
                raise LbError("compressed stream contains a zero-length opcode")

        if opcode == 0:
            output.extend(take(count, "literal payload"))
        elif opcode == 1:
            output.extend(bytes(count))
        elif opcode == 2:
            output.extend(take(1, "repeat-byte payload") * count)
        elif opcode in (3, 4):
            width = 1 if opcode == 3 else 2
            distance = int.from_bytes(take(width, "copy distance"), "little")
            if distance == 0 or distance > len(output):
                raise LbError(f"invalid copy distance {distance} at output 0x{len(output):x}")
            for _ in range(count):
                output.append(output[-distance])
        elif opcode == 5:
            for value in take(count, "zero-interleaved payload"):
                output.extend((value, 0))
        else:
            raise LbError(f"unknown compression opcode {opcode}")

        if len(output) > raw_size:
            raise LbError("compressed stream exceeds its declared size")

    if cursor >= len(data) or data[cursor] != 0xFF:
        raise LbError("compressed stream has no terminator")
    if cursor + 1 != len(data):
        raise LbError("compressed stream has bytes after its terminator")
    return bytes(output)


def _opcode(opcode: int, count: int, payload: bytes = b"") -> bytes:
    if not 1 <= count <= MAX_COUNT:
        raise LbError(f"compression count {count} is outside u16 range")
    if count <= 0x1F:
        return bytes((opcode << 5 | count,)) + payload
    return bytes((opcode << 5,)) + struct.pack("<H", count) + payload


def _command_cost(count: int, payload_size: int) -> int:
    return (1 if count <= 0x1F else 3) + payload_size


def _literal_cost(count: int) -> int:
    return _command_cost(count, count)


def _emit_literals(output: bytearray, data: bytes) -> None:
    cursor = 0
    while cursor < len(data):
        count = min(MAX_COUNT, len(data) - cursor)
        output.extend(_opcode(0, count, data[cursor : cursor + count]))
        cursor += count


def compress(data: bytes) -> bytes:
    """Encode a deterministic, bounded-search SDF stream.

    The retail encoder is not size-optimal and its private match-finder is not
    needed for reconstruction.  This encoder chooses profitable native
    commands greedily, examines up to 256 recent matches per three-byte key,
    and emits the same stream language consumed by the game.
    """

    output = bytearray()
    positions: dict[bytes, list[int]] = defaultdict(list)
    literal_start = 0
    cursor = 0

    def remember(start: int, end: int) -> None:
        for position in range(start, end):
            if position + 3 <= len(data):
                positions[data[position : position + 3]].append(position)

    while cursor < len(data):
        remaining = min(MAX_COUNT, len(data) - cursor)

        run = 1
        while run < remaining and data[cursor + run] == data[cursor]:
            run += 1

        word_count = 0
        while (
            word_count < MAX_COUNT
            and cursor + word_count * 2 + 1 < len(data)
            and data[cursor + word_count * 2] != 0
            and data[cursor + word_count * 2 + 1] == 0
        ):
            word_count += 1

        short_length = short_distance = 0
        long_length = long_distance = 0
        if run < 3 and cursor + 3 <= len(data):
            candidates = positions.get(data[cursor : cursor + 3], ())
            examined = 0
            for previous in reversed(candidates):
                distance = cursor - previous
                if distance > MAX_COUNT:
                    break
                examined += 1
                if examined > 256:
                    break
                length = 3
                while (
                    length < remaining
                    and data[cursor + length] == data[cursor + length - distance]
                ):
                    length += 1
                if distance <= 0xFF:
                    if length > short_length:
                        short_length, short_distance = length, distance
                elif length > long_length:
                    long_length, long_distance = length, distance

        choices: list[tuple[int, int, int, int, int]] = []
        # (saving, bytes consumed, inverse priority, opcode, argument)
        if data[cursor] == 0:
            cost = _command_cost(run, 0)
            choices.append((_literal_cost(run) - cost, run, 5, 1, 0))
        elif run >= 2:
            cost = _command_cost(run, 1)
            choices.append((_literal_cost(run) - cost, run, 4, 2, data[cursor]))
        if word_count:
            consumed = word_count * 2
            cost = _command_cost(word_count, word_count)
            choices.append((_literal_cost(consumed) - cost, consumed, 3, 5, word_count))
        if short_length >= 3:
            cost = _command_cost(short_length, 1)
            choices.append((_literal_cost(short_length) - cost, short_length, 2, 3, short_distance))
        if long_length >= 4:
            cost = _command_cost(long_length, 2)
            choices.append((_literal_cost(long_length) - cost, long_length, 1, 4, long_distance))

        choice = max(choices, default=None)
        if choice is None or choice[0] <= 0:
            remember(cursor, cursor + 1)
            cursor += 1
            continue

        if literal_start < cursor:
            _emit_literals(output, data[literal_start:cursor])

        _, consumed, _, opcode, argument = choice
        if opcode == 1:
            output.extend(_opcode(1, consumed))
        elif opcode == 2:
            output.extend(_opcode(2, consumed, bytes((argument,))))
        elif opcode == 3:
            output.extend(_opcode(3, consumed, bytes((argument,))))
        elif opcode == 4:
            output.extend(_opcode(4, consumed, struct.pack("<H", argument)))
        elif opcode == 5:
            count = consumed // 2
            output.extend(_opcode(5, count, data[cursor : cursor + consumed : 2]))
        else:
            raise AssertionError(opcode)

        remember(cursor, cursor + consumed)
        cursor += consumed
        literal_start = cursor

    if literal_start < len(data):
        _emit_literals(output, data[literal_start:])
    output.append(0xFF)
    return bytes(output)


def parse_archive(data: bytes) -> Archive:
    entries: list[Entry] = []
    offset = 0
    while True:
        header = _read(data, offset, HEADER_SIZE, "LB header")
        if header[0] == 0xFF:
            if header != END_HEADER:
                raise LbError(f"invalid END0 header at 0x{offset:x}")
            ending = data[offset:]
            if len(ending) != ALIGNMENT or any(ending[HEADER_SIZE:]):
                raise LbError("LB END0 block is not one zero-padded alignment unit")
            return Archive(tuple(entries), ending)

        type_id, compressed, user_id, total_size = struct.unpack_from("<BBhI", header)
        if compressed not in (0, 1):
            raise LbError(f"entry {len(entries)} has invalid compression flag {compressed}")
        if total_size < HEADER_SIZE:
            raise LbError(f"entry {len(entries)} has invalid size {total_size}")
        end = offset + total_size
        aligned_end = _align(end)
        _read(data, offset, aligned_end - offset, f"entry {len(entries)}")
        extension_bytes = header[8:12]
        try:
            extension = extension_bytes.rstrip(b"\0").decode("ascii")
        except UnicodeDecodeError as exc:
            raise LbError(f"entry {len(entries)} extension is not ASCII") from exc
        if not extension or b"\0" in extension_bytes.rstrip(b"\0"):
            raise LbError(f"entry {len(entries)} has an invalid extension")
        raw_size = struct.unpack_from("<I", header, 12)[0]
        stored = data[offset + HEADER_SIZE : end]
        if not compressed and raw_size not in (0, len(stored)):
            raise LbError(f"uncompressed entry {len(entries)} size does not match its header")
        if compressed:
            decompress(stored, raw_size)
        entries.append(
            Entry(
                len(entries), offset, type_id, bool(compressed), user_id,
                extension, raw_size, stored, data[offset:aligned_end],
            )
        )
        offset = aligned_end


def entry_data(entry: Entry) -> bytes:
    return decompress(entry.stored, entry.raw_size) if entry.compressed else entry.stored


def _fields(tokens: list[str], required: tuple[str, ...], line: int) -> dict[str, str]:
    values: dict[str, str] = {}
    for token in tokens:
        if "=" not in token:
            raise LbError(f"line {line}: expected key=value, got {token!r}")
        key, value = token.split("=", 1)
        if key in values:
            raise LbError(f"line {line}: duplicate field {key}")
        values[key] = value
    missing = set(required) - values.keys()
    extra = values.keys() - set(required)
    if missing or extra:
        detail = []
        if missing:
            detail.append(f"missing {', '.join(sorted(missing))}")
        if extra:
            detail.append(f"unknown {', '.join(sorted(extra))}")
        raise LbError(f"line {line}: {'; '.join(detail)}")
    return values


def _integer(value: str, line: int) -> int:
    try:
        return int(value, 0)
    except ValueError as exc:
        raise LbError(f"line {line}: invalid integer {value!r}") from exc


def _digest(value: str, line: int) -> str:
    value = value.lower()
    if not re.fullmatch(r"[0-9a-f]{40}", value):
        raise LbError(f"line {line}: invalid SHA-1 {value!r}")
    return value


def parse_source(text: str) -> Source:
    statements: list[tuple[int, list[str]]] = []
    for line_number, raw_line in enumerate(text.splitlines(), 1):
        try:
            tokens = shlex.split(raw_line, comments=True)
        except ValueError as exc:
            raise LbError(f"line {line_number}: {exc}") from exc
        if tokens:
            statements.append((line_number, tokens))
    if not statements or statements[0][1] != ["lb", "1"]:
        raise LbError("source must begin with 'lb 1'")
    if len(statements) < 2 or statements[1][1][0] != "base":
        raise LbError("source must declare its retail base")
    line, tokens = statements[1]
    base = _fields(tokens[1:], ("sha1", "size"), line)
    entries: list[SourceEntry] = []
    required = ("type", "compressed", "user", "extension", "raw_size", "retail_sha1", "source")
    for line, tokens in statements[2:]:
        if tokens[0] != "entry":
            raise LbError(f"line {line}: unknown directive {tokens[0]!r}")
        fields = _fields(tokens[1:], required, line)
        extension = fields["extension"]
        try:
            extension_bytes = extension.encode("ascii")
        except UnicodeEncodeError as exc:
            raise LbError(f"line {line}: extension is not ASCII") from exc
        if not extension or len(extension_bytes) > 4:
            raise LbError(f"line {line}: extension must contain one to four ASCII bytes")
        type_id = _integer(fields["type"], line)
        if not 0 <= type_id < 0xFF:
            raise LbError(f"line {line}: entry type is outside u8 resource range")
        compressed = _integer(fields["compressed"], line)
        if compressed not in (0, 1):
            raise LbError(f"line {line}: compressed must be 0 or 1")
        user_id = _integer(fields["user"], line)
        if not -0x8000 <= user_id <= 0x7FFF:
            raise LbError(f"line {line}: user id is outside s16 range")
        raw_size = _integer(fields["raw_size"], line)
        if not 0 <= raw_size <= 0xFFFFFFFF:
            raise LbError(f"line {line}: raw size is outside u32 range")
        entries.append(
            SourceEntry(
                type_id, bool(compressed), user_id,
                extension, raw_size, _digest(fields["retail_sha1"], line),
                fields["source"],
            )
        )
    if not entries:
        raise LbError("source contains no entries")
    base_size = _integer(base["size"], statements[1][0])
    if base_size < 0:
        raise LbError(f"line {statements[1][0]}: base size is negative")
    return Source(_digest(base["sha1"], statements[1][0]), base_size, tuple(entries))


def render_source(data: bytes) -> str:
    archive = parse_archive(data)
    lines = ["lb 1", f"base sha1={_sha1(data)} size=0x{len(data):x}"]
    for entry in archive.entries:
        raw = entry_data(entry)
        lines.append(
            "entry "
            f"type={entry.type_id} compressed={int(entry.compressed)} user={entry.user_id} "
            f"extension={entry.extension} raw_size=0x{entry.raw_size:x} "
            f"retail_sha1={_sha1(raw)} source=base"
        )
    return "\n".join(lines) + "\n"


def _replacement_path(resource_dir: Path, source: str) -> Path:
    relative = Path(source)
    if relative.is_absolute() or ".." in relative.parts:
        raise LbError(f"resource source {source!r} must be a relative path without '..'")
    return resource_dir / relative


def _build_block(source: SourceEntry, raw: bytes) -> bytes:
    if len(raw) > 0xFFFFFFFF:
        raise LbError("replacement resource exceeds the LB u32 size limit")
    encoded = compress(raw)
    if len(encoded) < len(raw):
        compressed = 1
        stored = encoded
    else:
        compressed = 0
        stored = raw
    if HEADER_SIZE + len(stored) > 0xFFFFFFFF:
        raise LbError("stored resource exceeds the LB u32 size limit")
    extension = source.extension.encode("ascii").ljust(4, b"\0")
    header = struct.pack(
        "<BBhI4sI", source.type_id, compressed, source.user_id,
        HEADER_SIZE + len(stored), extension, len(raw),
    )
    block = header + stored
    return block + bytes(_align(len(block)) - len(block))


def assemble(source: Source, base_data: bytes, resource_dir: Path) -> bytes:
    if len(base_data) != source.base_size or _sha1(base_data) != source.base_sha1:
        raise LbError("retail base archive does not match the source declaration")
    archive = parse_archive(base_data)
    if len(archive.entries) != len(source.entries):
        raise LbError("source entry count does not match the retail base")

    blocks: list[bytes] = []
    for index, (expected, entry) in enumerate(zip(source.entries, archive.entries)):
        actual = (entry.type_id, entry.compressed, entry.user_id, entry.extension, entry.raw_size)
        wanted = (expected.type_id, expected.compressed, expected.user_id, expected.extension, expected.raw_size)
        if actual != wanted:
            raise LbError(f"entry {index} metadata does not match the retail base")
        retail_raw = entry_data(entry)
        if _sha1(retail_raw) != expected.retail_sha1:
            raise LbError(f"entry {index} retail resource SHA-1 does not match")

        if expected.source == "base":
            blocks.append(entry.block)
            continue
        path = _replacement_path(resource_dir, expected.source)
        try:
            replacement = path.read_bytes()
        except OSError as exc:
            raise LbError(f"cannot read resource {path}: {exc}") from exc
        blocks.append(entry.block if replacement == retail_raw else _build_block(expected, replacement))

    result = b"".join(blocks) + archive.ending
    parse_archive(result)
    return result


def _command_disassemble(args: argparse.Namespace) -> None:
    source = render_source(args.input.read_bytes())
    if args.output:
        args.output.parent.mkdir(parents=True, exist_ok=True)
        args.output.write_text(source, encoding="utf-8")
    else:
        sys.stdout.write(source)


def _command_extract(args: argparse.Namespace) -> None:
    archive = parse_archive(args.input.read_bytes())
    args.output.mkdir(parents=True, exist_ok=True)
    for entry in archive.entries:
        name = f"{entry.index:02}_{entry.user_id}.{entry.extension.lower()}"
        (args.output / name).write_bytes(entry_data(entry))


def _assembled(args: argparse.Namespace) -> bytes:
    source = parse_source(args.input.read_text(encoding="utf-8"))
    return assemble(source, args.base.read_bytes(), args.resources)


def _command_assemble(args: argparse.Namespace) -> None:
    data = _assembled(args)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_bytes(data)


def _command_verify(args: argparse.Namespace) -> None:
    data = _assembled(args)
    base = args.base.read_bytes()
    if data != base:
        raise LbError("LB source does not reproduce the retail archive")
    print(f"{args.input}: exact ({len(data)} bytes, sha1 {_sha1(data)})")


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    commands = parser.add_subparsers(dest="command", required=True)

    disassemble = commands.add_parser("disassemble", help="describe a retail LB archive")
    disassemble.add_argument("input", type=Path)
    disassemble.add_argument("output", nargs="?", type=Path)

    extract = commands.add_parser("extract", help="extract decompressed LB resources")
    extract.add_argument("input", type=Path)
    extract.add_argument("output", type=Path)

    assemble_parser = commands.add_parser("assemble", help="assemble LB source over a retail base")
    assemble_parser.add_argument("input", type=Path)
    assemble_parser.add_argument("base", type=Path)
    assemble_parser.add_argument("output", type=Path)
    assemble_parser.add_argument("--resources", required=True, type=Path)

    verify = commands.add_parser("verify", help="verify an exact source-to-retail rebuild")
    verify.add_argument("input", type=Path)
    verify.add_argument("base", type=Path)
    verify.add_argument("--resources", required=True, type=Path)

    args = parser.parse_args()
    try:
        {
            "disassemble": _command_disassemble,
            "extract": _command_extract,
            "assemble": _command_assemble,
            "verify": _command_verify,
        }[args.command](args)
    except (LbError, OSError, UnicodeError) as exc:
        parser.error(str(exc))


if __name__ == "__main__":
    main()
