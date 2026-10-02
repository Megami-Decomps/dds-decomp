#!/usr/bin/env python3
"""Encode and decode the packed relocation stream used by DDS field data."""

from __future__ import annotations


class RelocationError(ValueError):
    """Raised when a packed relocation stream is invalid."""


def decode(data: bytes) -> tuple[int, ...]:
    """Return byte offsets of relocated u32 words."""

    cursor = 0
    index = 0
    locations: list[int] = []
    while index < len(data):
        code = data[index]
        index += 1
        if not code & 1:
            delta = code >> 1
        elif not code & 2:
            if index >= len(data):
                raise RelocationError("truncated two-byte relocation")
            delta = (code | data[index] << 8) >> 2
            index += 1
        elif not code & 4:
            if index + 1 >= len(data):
                raise RelocationError("truncated three-byte relocation")
            delta = (code | data[index] << 8 | data[index + 1] << 16) >> 3
            index += 2
        else:
            count = (code >> 3) + 2
            for _ in range(count):
                cursor += 1
                locations.append(cursor * 4)
            continue
        cursor += delta
        locations.append(cursor * 4)

    if locations != sorted(set(locations)):
        raise RelocationError("relocation locations are not strictly ordered")
    return tuple(locations)


def encode(locations: list[int]) -> bytes:
    """Pack strictly ordered, u32-aligned relocation locations."""

    if locations != sorted(set(locations)):
        raise RelocationError("relocation locations are not strictly ordered")
    if any(location & 3 for location in locations):
        raise RelocationError("relocation location is not u32-aligned")

    words = [location // 4 for location in locations]
    output = bytearray()
    previous = 0
    index = 0

    def emit_delta(delta: int) -> None:
        if delta < 0:
            raise RelocationError("negative relocation delta")
        if delta <= 0x7F:
            output.append(delta << 1)
        elif delta < 0x4000:
            value = (delta << 2) | 1
            output.extend((value & 0xFF, value >> 8))
        elif delta < 0x200000:
            value = (delta << 3) | 3
            output.extend((value & 0xFF, (value >> 8) & 0xFF, value >> 16))
        else:
            raise RelocationError("relocation delta is too large")

    def emit_run(count: int) -> None:
        nonlocal index, previous
        while count:
            chunk = min(count, 33)
            output.append(((chunk - 2) << 3) | 7)
            previous += chunk
            index += chunk
            count -= chunk

    while index < len(words):
        if words[index] - previous == 1:
            end = index + 1
            while end < len(words) and words[end] == words[end - 1] + 1:
                end += 1
            if end - index >= 2:
                emit_run(end - index)
                continue

        emit_delta(words[index] - previous)
        previous = words[index]
        index += 1

        end = index
        while end < len(words) and words[end] == previous + end - index + 1:
            end += 1
        if end - index >= 2:
            emit_run(end - index)

    return bytes(output)
