#!/usr/bin/env python3
"""Decode DDS TBN/TXP0 texture bundles and their TMX0 images."""

from __future__ import annotations

import argparse
import struct
import zlib
from dataclasses import dataclass
from pathlib import Path


PSM_NAMES = {
    0x00: "PSMCT32",
    0x01: "PSMCT24",
    0x02: "PSMCT16",
    0x13: "PSMT8",
    0x14: "PSMT4",
}


class TmxError(ValueError):
    """Raised when a DDS texture bundle is malformed or unsupported."""


@dataclass(frozen=True)
class Texture:
    index: int
    width: int
    height: int
    psm: int
    rgba: bytes
    mipmap_count: int = 0
    clut_psm: int = 0
    texture_flags: int = 0


def _u32(data: bytes, offset: int, context: str) -> int:
    if offset < 0 or offset + 4 > len(data):
        raise TmxError(f"{context} lies outside the texture bundle")
    return struct.unpack_from("<I", data, offset)[0]


def _expand_alpha(value: int) -> int:
    return min(value * 2, 0xFF)


def _unswizzle_psmt8_palette(palette: bytes) -> bytes:
    """Convert the GS CSM1 order of a 256-color palette to index order."""

    result = bytearray(palette)
    for block in range(0, 256, 32):
        for index in range(8):
            left = (block + 8 + index) * 4
            right = (block + 16 + index) * 4
            result[left : left + 4], result[right : right + 4] = (
                result[right : right + 4],
                result[left : left + 4],
            )
    return bytes(result)


def _palette(data: bytes, count: int) -> tuple[tuple[int, int, int, int], ...]:
    return tuple(
        (
            data[index * 4],
            data[index * 4 + 1],
            data[index * 4 + 2],
            _expand_alpha(data[index * 4 + 3]),
        )
        for index in range(count)
    )


def _palette_16(data: bytes, count: int) -> tuple[tuple[int, int, int, int], ...]:
    result = []
    for index in range(count):
        value = struct.unpack_from("<H", data, index * 2)[0]
        result.append(
            (
                (value & 0x1F) * 0xFF // 0x1F,
                ((value >> 5) & 0x1F) * 0xFF // 0x1F,
                ((value >> 10) & 0x1F) * 0xFF // 0x1F,
                0xFF if value & 0x8000 else 0,
            )
        )
    return tuple(result)


def _decode_pixels(
    data: bytes,
    width: int,
    height: int,
    psm: int,
    clut_psm: int,
) -> bytes:
    count = width * height
    rgba = bytearray(count * 4)
    if psm == 0x13:
        if clut_psm != 0:
            raise TmxError(f"unsupported PSMT8 CLUT mode 0x{clut_psm:x}")
        palette = _palette(_unswizzle_psmt8_palette(data[:0x400]), 256)
        indices = data[0x400:]
        for index, palette_index in enumerate(indices):
            rgba[index * 4 : index * 4 + 4] = bytes(palette[palette_index])
    elif psm == 0x14:
        if clut_psm == 0:
            palette_size = 0x40
            palette = _palette(data[:palette_size], 16)
        elif clut_psm == 2:
            palette_size = 0x20
            palette = _palette_16(data[:palette_size], 16)
        else:
            raise TmxError(f"unsupported PSMT4 CLUT mode 0x{clut_psm:x}")
        indices = data[palette_size:]
        for index in range(count):
            palette_index = (indices[index // 2] >> ((index & 1) * 4)) & 0xF
            rgba[index * 4 : index * 4 + 4] = bytes(palette[palette_index])
    elif psm == 0x00:
        for index in range(count):
            source = index * 4
            rgba[source : source + 3] = data[source : source + 3]
            rgba[source + 3] = _expand_alpha(data[source + 3])
    elif psm == 0x01:
        for index in range(count):
            source = index * 3
            rgba[index * 4 : index * 4 + 4] = data[source : source + 3] + b"\xff"
    elif psm == 0x02:
        for index in range(count):
            value = struct.unpack_from("<H", data, index * 2)[0]
            rgba[index * 4 : index * 4 + 4] = bytes(
                (
                    (value & 0x1F) * 0xFF // 0x1F,
                    ((value >> 5) & 0x1F) * 0xFF // 0x1F,
                    ((value >> 10) & 0x1F) * 0xFF // 0x1F,
                    0xFF if value & 0x8000 else 0,
                )
            )
    else:
        raise TmxError(f"unsupported GS pixel storage mode 0x{psm:x}")
    return bytes(rgba)


def _pixel_data_size(
    width: int,
    height: int,
    psm: int,
    mipmap_count: int,
    clut_psm: int,
) -> int:
    dimensions = (
        (max(1, width >> level), max(1, height >> level))
        for level in range(mipmap_count + 1)
    )
    pixels = sum(level_width * level_height for level_width, level_height in dimensions)
    try:
        return {
            0x00: pixels * 4,
            0x01: pixels * 3,
            0x02: pixels * 2,
            0x13: (0x400 if clut_psm == 0 else 0) + pixels,
            0x14: ({0: 0x40, 2: 0x20}.get(clut_psm, 0)) + (pixels + 1) // 2,
        }[psm]
    except KeyError as exc:
        raise TmxError(f"unsupported GS pixel storage mode 0x{psm:x}") from exc


def parse_bundle(data: bytes) -> tuple[Texture, ...]:
    """Parse a DDS TBN payload or its embedded TXP0 packet."""

    if data[:4] == b"TXP0":
        base = 0
    elif data[8:12] == b"TXP0":
        base = 8
        if _u32(data, 0, "TBN type") != 9 or _u32(data, 4, "TBN size") != len(data):
            raise TmxError("invalid TBN packet header")
    else:
        raise TmxError("texture bundle has no TXP0 header")

    if _u32(data, base + 4, "TXP0 reserved word") != 0:
        raise TmxError("TXP0 reserved word is nonzero")
    count = _u32(data, base + 8, "TXP0 texture count")
    table_end = base + 0xC + count * 4
    if table_end > len(data):
        raise TmxError("TXP0 offset table exceeds the bundle")
    offsets = struct.unpack_from(f"<{count}I", data, base + 0xC)
    if tuple(sorted(offsets)) != offsets or len(set(offsets)) != count:
        raise TmxError("TXP0 texture offsets are not strictly increasing")
    if offsets and offsets[0] < 0xC + count * 4:
        raise TmxError("first TMX0 overlaps the TXP0 offset table")

    textures = []
    for index, relative in enumerate(offsets):
        magic = base + relative
        packet = magic - 8
        if packet < base or data[magic : magic + 4] != b"TMX0":
            raise TmxError(f"texture {index} has no TMX0 header")
        packet_type = _u32(data, packet, f"texture {index} packet type")
        packet_size = _u32(data, packet + 4, f"texture {index} packet size")
        end = packet + packet_size
        expected_end = (
            base + offsets[index + 1] - 8 if index + 1 < count else len(data)
        )
        if packet_type != 2 or packet_size < 0x40 or end != expected_end:
            raise TmxError(f"texture {index} has an invalid packet boundary")
        if _u32(data, magic + 4, f"texture {index} reserved word") != 0:
            raise TmxError(f"texture {index} has a nonzero reserved word")
        kind, clut_psm, width, height, psm, mipmap_count, texture_flags, word_12 = (
            struct.unpack_from("<BBHHBBHH", data, magic + 8)
        )
        if not width or not height:
            raise TmxError(f"texture {index} has zero dimensions")
        expected_kind = 1 if psm in {0x13, 0x14} else 0
        valid_clut = clut_psm == 0 or (psm == 0x14 and clut_psm == 2)
        if (
            kind != expected_kind
            or not valid_clut
            or word_12 != 0xFF00
        ):
            raise TmxError(f"texture {index} has an unsupported TMX0 profile")
        if any(data[magic + 0x14 : magic + 0x38]):
            raise TmxError(f"texture {index} has nonzero TMX0 padding")
        pixel_size = _pixel_data_size(width, height, psm, mipmap_count, clut_psm)
        pixel_data = data[magic + 0x38 : end]
        if len(pixel_data) != pixel_size:
            raise TmxError(
                f"texture {index} has {len(pixel_data)} pixel bytes, expected {pixel_size}"
            )
        textures.append(
            Texture(
                index,
                width,
                height,
                psm,
                _decode_pixels(pixel_data, width, height, psm, clut_psm),
                mipmap_count,
                clut_psm,
                texture_flags,
            )
        )
    return tuple(textures)


def encode_png(texture: Texture) -> bytes:
    """Encode a decoded texture as an RGBA8 PNG using only the standard library."""

    raw = b"".join(
        b"\0"
        + texture.rgba[row * texture.width * 4 : (row + 1) * texture.width * 4]
        for row in range(texture.height)
    )

    def chunk(kind: bytes, payload: bytes) -> bytes:
        return b"".join(
            (
                struct.pack(">I", len(payload)),
                kind,
                payload,
                struct.pack(">I", zlib.crc32(kind + payload) & 0xFFFFFFFF),
            )
        )

    header = struct.pack(
        ">IIBBBBB", texture.width, texture.height, 8, 6, 0, 0, 0
    )
    return b"".join(
        (
            b"\x89PNG\r\n\x1a\n",
            chunk(b"IHDR", header),
            chunk(b"IDAT", zlib.compress(raw, 9)),
            chunk(b"IEND", b""),
        )
    )


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("input", type=Path)
    parser.add_argument("output", type=Path)
    args = parser.parse_args()
    try:
        textures = parse_bundle(args.input.read_bytes())
        args.output.mkdir(parents=True, exist_ok=True)
        for texture in textures:
            name = (
                f"{texture.index:03d}_{PSM_NAMES[texture.psm]}_"
                f"{texture.width}x{texture.height}.png"
            )
            (args.output / name).write_bytes(encode_png(texture))
    except (OSError, TmxError) as exc:
        parser.error(str(exc))


if __name__ == "__main__":
    main()
