"""Lossless source support for the DDS ``MSG1``/BMD message format."""

from __future__ import annotations

import json
import re
import shlex
import struct
from dataclasses import dataclass


HEADER_SIZE = 0x20
NAME_SIZE = 0x18
MESSAGE_KIND = 0
SELECTION_KIND = 1
_SYMBOL = re.compile(r"[A-Za-z_][A-Za-z0-9_]*\Z")


class Msg1Error(ValueError):
    """Raised when an MSG1 bank or its source is invalid."""


@dataclass(frozen=True)
class Message:
    name: str
    speaker: int
    pages: tuple[bytes, ...]


@dataclass(frozen=True)
class Selection:
    name: str
    ext: int
    pattern: int
    reserved: int
    options: tuple[bytes, ...]
    trailing: bytes = b""


@dataclass(frozen=True)
class Bank:
    dialogs: tuple[Message | Selection, ...]
    speakers: tuple[bytes, ...]


def _range(data: bytes, offset: int, size: int, context: str) -> None:
    if offset < 0 or size < 0 or offset + size > len(data):
        raise Msg1Error(f"{context} lies outside the MSG1 bank")


def _u32(data: bytes, offset: int, context: str) -> int:
    _range(data, offset, 4, context)
    return struct.unpack_from("<I", data, offset)[0]


def _name(data: bytes, offset: int, context: str) -> str:
    _range(data, offset, NAME_SIZE, context)
    raw = data[offset : offset + NAME_SIZE]
    value, separator, padding = raw.partition(b"\0")
    if separator and any(padding):
        raise Msg1Error(f"{context} has nonzero name padding")
    if any(byte < 0x20 or byte > 0x7E for byte in value):
        raise Msg1Error(f"{context} name is not printable ASCII")
    try:
        return value.decode("ascii")
    except UnicodeDecodeError as exc:
        raise Msg1Error(f"{context} name is not ASCII") from exc


def _relative_pointer(data: bytes, offset: int, context: str) -> int:
    return HEADER_SIZE + _u32(data, offset, context)


def _split_message_pages(
    data: bytes, starts: list[int], buffer_start: int, buffer_size: int, context: str
) -> tuple[bytes, ...]:
    _range(data, buffer_start, buffer_size, f"{context} text buffer")
    if not starts:
        if buffer_size:
            raise Msg1Error(f"{context} has text without pages")
        return ()
    if starts[0] != buffer_start:
        raise Msg1Error(f"{context} first page does not start at its text buffer")
    buffer_end = buffer_start + buffer_size
    if starts != sorted(starts):
        raise Msg1Error(f"{context} page pointers are not ordered")
    if starts[-1] >= buffer_end:
        raise Msg1Error(f"{context} page pointer lies outside its text buffer")
    if not data[buffer_end - 1 : buffer_end] == b"\0":
        raise Msg1Error(f"{context} text buffer has no terminator")
    pages = [data[start:end] for start, end in zip(starts, starts[1:] + [buffer_end])]
    pages[-1] = pages[-1][:-1]
    if any(b"\0" in page for page in pages):
        raise Msg1Error(f"{context} contains an embedded NUL")
    return tuple(pages)


def _split_selection_options(
    data: bytes, starts: list[int], buffer_start: int, buffer_size: int, context: str
) -> tuple[tuple[bytes, ...], bytes]:
    _range(data, buffer_start, buffer_size, f"{context} text buffer")
    if not starts:
        if buffer_size:
            raise Msg1Error(f"{context} has text without options")
        return (), b""
    if starts[0] != buffer_start:
        raise Msg1Error(f"{context} first option does not start at its text buffer")
    buffer_end = buffer_start + buffer_size
    if starts != sorted(starts) or len(set(starts)) != len(starts):
        raise Msg1Error(f"{context} option pointers are not strictly ordered")
    if starts[-1] >= buffer_end:
        raise Msg1Error(f"{context} option pointer lies outside its text buffer")
    chunks = [data[start:end] for start, end in zip(starts, starts[1:] + [buffer_end])]
    options: list[bytes] = []
    for option_index, chunk in enumerate(chunks):
        try:
            terminator = chunk.index(0)
        except ValueError as exc:
            raise Msg1Error(f"{context} option {option_index} has no terminator") from exc
        if option_index + 1 != len(chunks) and terminator != len(chunk) - 1:
            raise Msg1Error(f"{context} option {option_index} has bytes after its terminator")
        options.append(chunk[:terminator])
    trailing = chunks[-1][chunks[-1].index(0) + 1 :]
    return tuple(options), trailing


def decode(data: bytes) -> Bank:
    """Decode a canonical DDS MSG1 bank and verify its complete physical layout."""

    _range(data, 0, HEADER_SIZE, "header")
    (
        file_type,
        format_id,
        user_id,
        file_size,
        magic,
        ext_size,
        relocation_offset,
        relocation_size,
        dialog_count,
        relocated,
        version,
    ) = struct.unpack_from("<BBhI4sIIIIhh", data)
    if (file_type, format_id, user_id, magic, ext_size, relocated, version) != (
        7,
        0,
        0,
        b"MSG1",
        0,
        0,
        2,
    ):
        raise Msg1Error("unsupported MSG1 header")
    if file_size != len(data):
        raise Msg1Error("MSG1 file size does not match its payload")
    _range(data, relocation_offset, relocation_size, "relocation table")
    if relocation_offset + relocation_size != len(data):
        raise Msg1Error("MSG1 relocation table is not at the end of the bank")

    table_size = dialog_count * 8 + 0x10
    _range(data, HEADER_SIZE, table_size, "dialog and speaker headers")
    dialogs: list[Message | Selection] = []
    for index in range(dialog_count):
        header = HEADER_SIZE + index * 8
        kind = _u32(data, header, f"dialog {index} kind")
        record = _relative_pointer(data, header + 4, f"dialog {index} pointer")
        name = _name(data, record, f"dialog {index}")
        if kind == MESSAGE_KIND:
            _range(data, record + NAME_SIZE, 4, f"message {index} header")
            page_count, speaker = struct.unpack_from("<hH", data, record + NAME_SIZE)
            if page_count < 0:
                raise Msg1Error(f"message {index} has a negative page count")
            pointer_table = record + 0x1C
            _range(data, pointer_table, page_count * 4, f"message {index} pages")
            starts = [
                _relative_pointer(data, pointer_table + item * 4, f"message {index} page {item}")
                for item in range(page_count)
            ]
            if page_count:
                size_offset = pointer_table + page_count * 4
                buffer_size = _u32(data, size_offset, f"message {index} text size")
                pages = _split_message_pages(
                    data, starts, size_offset + 4, buffer_size, f"message {index}"
                )
            else:
                pages = ()
            dialogs.append(Message(name, speaker, pages))
        elif kind == SELECTION_KIND:
            _range(data, record + NAME_SIZE, 8, f"selection {index} header")
            ext, option_count, pattern, reserved = struct.unpack_from(
                "<hhhh", data, record + NAME_SIZE
            )
            if option_count < 0:
                raise Msg1Error(f"selection {index} has a negative option count")
            pointer_table = record + 0x20
            _range(data, pointer_table, option_count * 4, f"selection {index} options")
            starts = [
                _relative_pointer(
                    data,
                    pointer_table + item * 4,
                    f"selection {index} option {item}",
                )
                for item in range(option_count)
            ]
            size_offset = pointer_table + option_count * 4
            buffer_size = _u32(data, size_offset, f"selection {index} text size")
            options, trailing = _split_selection_options(
                data, starts, size_offset + 4, buffer_size, f"selection {index}"
            )
            dialogs.append(Selection(name, ext, pattern, reserved, options, trailing))
        else:
            raise Msg1Error(f"dialog {index} has unknown kind {kind}")

    speaker_header = HEADER_SIZE + dialog_count * 8
    speaker_array = _relative_pointer(data, speaker_header, "speaker array pointer")
    speaker_count = _u32(data, speaker_header + 4, "speaker count")
    ext_data = _u32(data, speaker_header + 8, "speaker extension")
    reserved = _u32(data, speaker_header + 12, "speaker reserved word")
    if ext_data or reserved:
        raise Msg1Error("unsupported speaker-table extension")
    _range(data, speaker_array, speaker_count * 4, "speaker pointer array")
    speakers: list[bytes] = []
    for index in range(speaker_count):
        start = _relative_pointer(data, speaker_array + index * 4, f"speaker {index}")
        try:
            end = data.index(0, start, relocation_offset)
        except ValueError as exc:
            raise Msg1Error(f"speaker {index} has no terminator") from exc
        speakers.append(data[start:end])

    bank = Bank(tuple(dialogs), tuple(speakers))
    if encode(bank) != data:
        raise Msg1Error("MSG1 bank has a noncanonical or unsupported physical layout")
    return bank


def _encode_relocations(locations: list[int]) -> bytes:
    """Encode sorted pointer locations with the game's packed delta format."""

    output = bytearray()
    previous = HEADER_SIZE
    index = 0

    def emit_delta(delta: int) -> None:
        if delta < 0:
            raise Msg1Error("relocation locations are not ordered")
        if delta % 2 == 0 and delta // 2 <= 0xFF:
            output.append(delta // 2)
        elif delta < 0x4000:
            value = (delta << 2) | 1
            output.extend((value & 0xFF, value >> 8))
        elif delta < 0x200000:
            value = (delta << 3) | 3
            output.extend((value & 0xFF, (value >> 8) & 0xFF, value >> 16))
        else:
            raise Msg1Error("relocation delta is too large")

    def emit_run(count: int) -> None:
        nonlocal index, previous
        while count:
            chunk = min(count, 33)
            output.append(((chunk - 2) << 3) | 7)
            previous += chunk * 4
            index += chunk
            count -= chunk

    while index < len(locations):
        if locations[index] - previous == 4:
            end = index + 1
            while end < len(locations) and locations[end] == locations[end - 1] + 4:
                end += 1
            if end - index >= 2:
                emit_run(end - index)
                continue

        emit_delta(locations[index] - previous)
        previous = locations[index]
        index += 1

        end = index
        while end < len(locations) and locations[end] == previous + 4 * (end - index + 1):
            end += 1
        if end - index >= 2:
            emit_run(end - index)

    return bytes(output)


def _encoded_name(name: str) -> bytes:
    try:
        raw = name.encode("ascii")
    except UnicodeEncodeError as exc:
        raise Msg1Error(f"dialog name {name!r} is not ASCII") from exc
    if any(byte < 0x20 or byte > 0x7E for byte in raw):
        raise Msg1Error(f"dialog name {name!r} is not printable ASCII")
    if len(raw) > NAME_SIZE:
        raise Msg1Error(f"dialog name {name!r} exceeds {NAME_SIZE} bytes")
    return raw + bytes(NAME_SIZE - len(raw))


def encode(bank: Bank) -> bytes:
    """Build a canonical, relocatable DDS MSG1 bank."""

    dialog_count = len(bank.dialogs)
    if dialog_count > 0x7FFF:
        raise Msg1Error("MSG1 bank has too many dialogs")
    if len(bank.speakers) > 0xFFFFFFFF:
        raise Msg1Error("MSG1 bank has too many speakers")
    body = bytearray(dialog_count * 8 + 0x10)
    relocations = [HEADER_SIZE + index * 8 + 4 for index in range(dialog_count)]
    relocations.append(HEADER_SIZE + dialog_count * 8)

    def align() -> None:
        body.extend(bytes((-len(body)) & 3))

    def put_u32(offset: int, value: int) -> None:
        struct.pack_into("<I", body, offset, value)

    for index, dialog in enumerate(bank.dialogs):
        kind = MESSAGE_KIND if isinstance(dialog, Message) else SELECTION_KIND
        put_u32(index * 8, kind)
        align()
        record_offset = len(body)
        put_u32(index * 8 + 4, record_offset)
        body.extend(_encoded_name(dialog.name))

        if isinstance(dialog, Message):
            if not 0 <= dialog.speaker <= 0xFFFF:
                raise Msg1Error(f"message {dialog.name!r} speaker is outside u16")
            if len(dialog.pages) > 0x7FFF:
                raise Msg1Error(f"message {dialog.name!r} has too many pages")
            body.extend(struct.pack("<hH", len(dialog.pages), dialog.speaker))
            if dialog.pages:
                text_offset = record_offset + 0x1C + len(dialog.pages) * 4 + 4
                cursor = text_offset
                for page in dialog.pages:
                    if b"\0" in page:
                        raise Msg1Error(f"message {dialog.name!r} page contains NUL")
                    relocations.append(HEADER_SIZE + len(body))
                    body.extend(struct.pack("<I", cursor))
                    cursor += len(page)
                text = b"".join(dialog.pages) + b"\0"
                body.extend(struct.pack("<I", len(text)))
                body.extend(text)
        else:
            if dialog.trailing and not dialog.options:
                raise Msg1Error(
                    f"selection {dialog.name!r} has trailing bytes without options"
                )
            for field, value in (
                ("ext", dialog.ext),
                ("pattern", dialog.pattern),
                ("reserved", dialog.reserved),
            ):
                if not -0x8000 <= value <= 0x7FFF:
                    raise Msg1Error(f"selection {dialog.name!r} {field} is outside s16")
            if len(dialog.options) > 0x7FFF:
                raise Msg1Error(f"selection {dialog.name!r} has too many options")
            body.extend(
                struct.pack(
                    "<hhhh", dialog.ext, len(dialog.options), dialog.pattern, dialog.reserved
                )
            )
            text_offset = record_offset + 0x20 + len(dialog.options) * 4 + 4
            cursor = text_offset
            for option in dialog.options:
                if b"\0" in option:
                    raise Msg1Error(f"selection {dialog.name!r} option contains NUL")
                relocations.append(HEADER_SIZE + len(body))
                body.extend(struct.pack("<I", cursor))
                cursor += len(option) + 1
            text = b"".join(option + b"\0" for option in dialog.options) + dialog.trailing
            body.extend(struct.pack("<I", len(text)))
            body.extend(text)

    align()
    speaker_array = len(body)
    put_u32(dialog_count * 8, speaker_array)
    put_u32(dialog_count * 8 + 4, len(bank.speakers))
    pointer_offsets: list[int] = []
    for speaker in bank.speakers:
        if b"\0" in speaker:
            raise Msg1Error("speaker string contains NUL")
        pointer_offsets.append(len(body))
        relocations.append(HEADER_SIZE + len(body))
        body.extend(b"\0\0\0\0")
    for pointer_offset, speaker in zip(pointer_offsets, bank.speakers):
        put_u32(pointer_offset, len(body))
        body.extend(speaker)
        body.append(0)

    align()
    relocation_offset = HEADER_SIZE + len(body)
    relocation_table = _encode_relocations(relocations)
    file_size = relocation_offset + len(relocation_table)
    header = struct.pack(
        "<BBhI4sIIIIhh",
        7,
        0,
        0,
        file_size,
        b"MSG1",
        0,
        relocation_offset,
        len(relocation_table),
        dialog_count,
        0,
        2,
    )
    return header + body + relocation_table


def _render_name(name: str) -> str:
    return name if _SYMBOL.fullmatch(name) else json.dumps(name)


def _render_stream(data: bytes, indent: str) -> list[str]:
    lines: list[str] = []
    index = 0
    while index < len(data):
        byte = data[index]
        if byte == 0x0A:
            lines.append(f"{indent}newline")
            index += 1
            continue
        if 0x20 <= byte <= 0x7E:
            end = index + 1
            while end < len(data) and 0x20 <= data[end] <= 0x7E:
                end += 1
            lines.append(f"{indent}text {json.dumps(data[index:end].decode('ascii'))}")
            index = end
            continue
        if byte & 0xF0 == 0xF0:
            size = 2 * (byte & 0x0F)
            if size >= 2 and index + size <= len(data):
                chunk = data[index : index + size]
                lines.append(f"{indent}control {' '.join(f'{item:02x}' for item in chunk)}")
                index += size
                continue
        if byte >= 0x80 and byte < 0xF0 and index + 1 < len(data):
            glyphs: list[str] = []
            while (
                index + 1 < len(data)
                and 0x80 <= data[index] < 0xF0
                and len(glyphs) < 12
            ):
                glyphs.append(data[index : index + 2].hex())
                index += 2
            lines.append(f"{indent}glyphs {' '.join(glyphs)}")
            continue
        end = index + 1
        while end < len(data) and end - index < 24:
            next_byte = data[end]
            if next_byte == 0x0A or 0x20 <= next_byte <= 0x7E:
                break
            if next_byte & 0xF0 == 0xF0:
                break
            if next_byte >= 0x80 and next_byte < 0xF0 and end + 1 < len(data):
                break
            end += 1
        lines.append(f"{indent}bytes {data[index:end].hex()}")
        index = end
    return lines


def render(data: bytes) -> list[str]:
    """Render a canonical MSG1 bank as lines inside an FLW0 messages block."""

    bank = decode(data)
    lines = ["messages msg1"]
    for dialog in bank.dialogs:
        if isinstance(dialog, Message):
            speaker = "none" if dialog.speaker == 0xFFFF else str(dialog.speaker)
            lines.append(f"  message {_render_name(dialog.name)} speaker={speaker}")
            for page in dialog.pages:
                lines.append("    page")
                lines.extend(_render_stream(page, "      "))
                lines.append("    endpage")
            lines.append("  endmessage")
        else:
            fields = ""
            if dialog.ext or dialog.pattern or dialog.reserved or dialog.trailing:
                fields = (
                    f" ext={dialog.ext} pattern={dialog.pattern}"
                    f" reserved={dialog.reserved}"
                )
                if dialog.trailing:
                    fields += f" trailing={dialog.trailing.hex()}"
            lines.append(f"  select {_render_name(dialog.name)}{fields}")
            for option in dialog.options:
                lines.append("    option")
                lines.extend(_render_stream(option, "      "))
                lines.append("    endoption")
            lines.append("  endselect")
    for index, speaker in enumerate(bank.speakers):
        lines.append(f"  speaker {index}")
        lines.extend(_render_stream(speaker, "    "))
        lines.append("  endspeaker")
    return lines


def _tokens(line: str, line_number: int) -> list[str]:
    try:
        return shlex.split(line, comments=True)
    except ValueError as exc:
        raise Msg1Error(f"line {line_number}: {exc}") from exc


def _integer(text: str, line_number: int, context: str) -> int:
    try:
        return int(text, 0)
    except ValueError as exc:
        raise Msg1Error(f"line {line_number}: invalid {context} {text!r}") from exc


def _values(tokens: list[str], line_number: int) -> dict[str, str]:
    values: dict[str, str] = {}
    for token in tokens:
        if "=" not in token:
            raise Msg1Error(f"line {line_number}: expected key=value, found {token!r}")
        key, value = token.split("=", 1)
        if not key or key in values:
            raise Msg1Error(f"line {line_number}: invalid or duplicate field {key!r}")
        values[key] = value
    return values


def _parse_stream(content: list[tuple[int, str]]) -> bytes:
    output = bytearray()
    for line_number, line in content:
        tokens = _tokens(line, line_number)
        if not tokens:
            continue
        if tokens[0] == "text":
            if len(tokens) != 2:
                raise Msg1Error(f"line {line_number}: text takes one quoted string")
            try:
                output.extend(tokens[1].encode("ascii"))
            except UnicodeEncodeError as exc:
                raise Msg1Error(f"line {line_number}: text is not ASCII") from exc
        elif tokens[0] == "newline":
            if len(tokens) != 1:
                raise Msg1Error(f"line {line_number}: newline takes no operands")
            output.append(0x0A)
        elif tokens[0] == "glyphs":
            if len(tokens) < 2:
                raise Msg1Error(f"line {line_number}: glyphs needs at least one code")
            for token in tokens[1:]:
                if not re.fullmatch(r"[0-9A-Fa-f]{4}", token):
                    raise Msg1Error(f"line {line_number}: invalid glyph code {token!r}")
                value = int(token, 16)
                if not 0x8000 <= value < 0xF000:
                    raise Msg1Error(
                        f"line {line_number}: glyph code {token!r} is outside "
                        "the glyph range"
                    )
                output.extend(value.to_bytes(2, "big"))
        elif tokens[0] == "control":
            if len(tokens) < 3 or any(
                not re.fullmatch(r"[0-9A-Fa-f]{2}", token)
                for token in tokens[1:]
            ):
                raise Msg1Error(f"line {line_number}: invalid control byte sequence")
            chunk = bytes(int(token, 16) for token in tokens[1:])
            if chunk[0] & 0xF0 != 0xF0 or len(chunk) != 2 * (chunk[0] & 0x0F):
                raise Msg1Error(f"line {line_number}: control length does not match its lead byte")
            output.extend(chunk)
        elif tokens[0] == "bytes":
            if len(tokens) != 2:
                raise Msg1Error(f"line {line_number}: bytes takes one hexadecimal string")
            try:
                output.extend(bytes.fromhex(tokens[1]))
            except ValueError as exc:
                raise Msg1Error(f"line {line_number}: invalid byte string") from exc
        else:
            raise Msg1Error(f"line {line_number}: unknown message-stream directive {tokens[0]!r}")
    if b"\0" in output:
        raise Msg1Error("message-stream source contains NUL; terminators are implicit")
    return bytes(output)


def parse_source(content: list[tuple[int, str]]) -> bytes:
    """Parse the contents of a symbolic ``messages msg1`` block."""

    dialogs: list[Message | Selection] = []
    speakers: list[bytes] = []
    index = 0
    while index < len(content):
        line_number, line = content[index]
        tokens = _tokens(line, line_number)
        if not tokens:
            index += 1
            continue
        directive = tokens[0]
        if directive in ("message", "select"):
            if len(tokens) < 2:
                raise Msg1Error(f"line {line_number}: {directive} needs a name")
            name = tokens[1]
            values = _values(tokens[2:], line_number)
            expected_end = "endmessage" if directive == "message" else "endselect"
            child_name = "page" if directive == "message" else "option"
            children: list[bytes] = []
            index += 1
            while index < len(content):
                child_line, child_text = content[index]
                child_tokens = _tokens(child_text, child_line)
                if child_tokens == [expected_end]:
                    break
                if child_tokens != [child_name]:
                    raise Msg1Error(
                        f"line {child_line}: expected {child_name!r} or {expected_end!r}"
                    )
                end_child = "endpage" if directive == "message" else "endoption"
                stream: list[tuple[int, str]] = []
                index += 1
                while index < len(content) and _tokens(
                    content[index][1], content[index][0]
                ) != [end_child]:
                    stream.append(content[index])
                    index += 1
                if index == len(content):
                    raise Msg1Error(f"line {child_line}: {child_name} has no {end_child}")
                children.append(_parse_stream(stream))
                index += 1
            if index == len(content):
                raise Msg1Error(f"line {line_number}: {directive} has no {expected_end}")
            if directive == "message":
                if values.keys() != {"speaker"}:
                    raise Msg1Error(f"line {line_number}: message needs only speaker=VALUE")
                speaker_text = values["speaker"]
                speaker = (
                    0xFFFF
                    if speaker_text == "none"
                    else _integer(speaker_text, line_number, "speaker")
                )
                dialogs.append(Message(name, speaker, tuple(children)))
            else:
                if values.keys() - {"ext", "pattern", "reserved", "trailing"}:
                    raise Msg1Error(f"line {line_number}: unknown selection field")
                try:
                    trailing = bytes.fromhex(values.get("trailing", ""))
                except ValueError as exc:
                    raise Msg1Error(
                        f"line {line_number}: invalid selection trailing bytes"
                    ) from exc
                dialogs.append(
                    Selection(
                        name,
                        _integer(values.get("ext", "0"), line_number, "ext"),
                        _integer(values.get("pattern", "0"), line_number, "pattern"),
                        _integer(values.get("reserved", "0"), line_number, "reserved"),
                        tuple(children),
                        trailing,
                    )
                )
            index += 1
            continue
        if directive == "speaker":
            if len(tokens) != 2:
                raise Msg1Error(f"line {line_number}: speaker needs an index")
            speaker_index = _integer(tokens[1], line_number, "speaker index")
            if speaker_index != len(speakers):
                raise Msg1Error(f"line {line_number}: expected speaker index {len(speakers)}")
            stream: list[tuple[int, str]] = []
            index += 1
            while index < len(content) and _tokens(
                content[index][1], content[index][0]
            ) != ["endspeaker"]:
                stream.append(content[index])
                index += 1
            if index == len(content):
                raise Msg1Error(f"line {line_number}: speaker has no endspeaker")
            speakers.append(_parse_stream(stream))
            index += 1
            continue
        raise Msg1Error(f"line {line_number}: unexpected MSG1 directive {directive!r}")

    return encode(Bank(tuple(dialogs), tuple(speakers)))
