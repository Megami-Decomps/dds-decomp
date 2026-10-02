from __future__ import annotations

import struct
import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

import tmx  # noqa: E402


def texture_bundle(
    width: int,
    height: int,
    psm: int,
    pixels: bytes,
    *,
    mipmap_count: int = 0,
    clut_psm: int = 0,
    texture_flags: int = 0,
) -> bytes:
    kind = 1 if psm in {0x13, 0x14} else 0
    tmx_header = b"".join(
        (
            b"TMX0",
            bytes(4),
            struct.pack(
                "<BBHHBBHH",
                kind,
                clut_psm,
                width,
                height,
                psm,
                mipmap_count,
                texture_flags,
                0xFF00,
            ),
            bytes(0x24),
        )
    )
    packet_size = 8 + len(tmx_header) + len(pixels)
    child = struct.pack("<II", 2, packet_size) + tmx_header + pixels
    txp = b"TXP0" + bytes(4) + struct.pack("<II", 1, 0x40)
    body = txp + bytes(0x38 - len(txp)) + child
    return struct.pack("<II", 9, 8 + len(body)) + body


class TmxTests(unittest.TestCase):
    def test_accepts_empty_texture_bundle(self) -> None:
        body = b"TXP0" + bytes(4) + struct.pack("<I", 0)
        data = struct.pack("<II", 9, 8 + len(body)) + body
        self.assertEqual(tmx.parse_bundle(data), ())

    def test_decodes_low_nibble_first_psmt4_and_expands_alpha(self) -> None:
        palette = bytearray(0x40)
        palette[4:8] = bytes((10, 20, 30, 0x40))
        palette[8:12] = bytes((40, 50, 60, 0x80))
        indices = bytes((0x21,)) * (32 * 32 // 2)
        texture = tmx.parse_bundle(texture_bundle(32, 32, 0x14, palette + indices))[0]
        self.assertEqual(texture.rgba[:8], bytes((10, 20, 30, 0x80, 40, 50, 60, 0xFF)))

    def test_unswizzles_psmt8_palette(self) -> None:
        palette = bytearray(0x400)
        palette[8 * 4 : 8 * 4 + 4] = bytes((1, 2, 3, 0x80))
        palette[16 * 4 : 16 * 4 + 4] = bytes((4, 5, 6, 0x80))
        stored = tmx._unswizzle_psmt8_palette(palette)
        indices = bytes((8, 16)) * (16 * 16 // 2)
        texture = tmx.parse_bundle(texture_bundle(16, 16, 0x13, stored + indices))[0]
        self.assertEqual(texture.rgba[:8], bytes((1, 2, 3, 0xFF, 4, 5, 6, 0xFF)))

    def test_encodes_self_contained_png(self) -> None:
        pixels = bytes((11, 22, 33, 0x80)) * (8 * 8)
        texture = tmx.parse_bundle(texture_bundle(8, 8, 0x00, pixels))[0]
        png = tmx.encode_png(texture)
        self.assertEqual(png[:8], b"\x89PNG\r\n\x1a\n")
        self.assertEqual(struct.unpack_from(">II", png, 16), (8, 8))
        self.assertEqual(texture.rgba[:4], bytes((11, 22, 33, 0xFF)))

    def test_decodes_psmct24(self) -> None:
        pixels = bytes((11, 22, 33)) * (8 * 8)
        texture = tmx.parse_bundle(texture_bundle(8, 8, 0x01, pixels))[0]
        self.assertEqual(texture.rgba[:4], bytes((11, 22, 33, 0xFF)))

    def test_ignores_mip_pixels_after_the_base_image(self) -> None:
        palette = bytearray(0x20)
        struct.pack_into("<H", palette, 2, 0xFFFF)
        base = bytes((0x11,)) * (32 * 32 // 2)
        mip = bytes((0,)) * (16 * 16 // 2)
        texture = tmx.parse_bundle(
            texture_bundle(
                32,
                32,
                0x14,
                palette + base + mip,
                mipmap_count=1,
                clut_psm=2,
                texture_flags=0x0F65,
            )
        )[0]
        self.assertEqual(texture.rgba[:4], bytes((0xFF, 0xFF, 0xFF, 0xFF)))
        self.assertEqual(texture.mipmap_count, 1)
        self.assertEqual(texture.clut_psm, 2)

    def test_rejects_bad_texture_boundaries(self) -> None:
        pixels = bytes((0, 0, 0, 0x80)) * (8 * 8)
        data = bytearray(texture_bundle(8, 8, 0x00, pixels))
        struct.pack_into("<I", data, 0x44, 0x40)
        with self.assertRaisesRegex(tmx.TmxError, "packet boundary"):
            tmx.parse_bundle(bytes(data))


if __name__ == "__main__":
    unittest.main()
