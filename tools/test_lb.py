#!/usr/bin/env python3
"""Regression tests for the DDS LB archive and compression codec."""

from __future__ import annotations

import hashlib
import struct
import sys
import tempfile
import unittest
from pathlib import Path


TOOLS = Path(__file__).resolve().parent
ROOT = TOOLS.parent
if str(TOOLS) not in sys.path:
    sys.path.insert(0, str(TOOLS))

import lb  # noqa: E402


def _block(type_id: int, user_id: int, extension: str, raw: bytes, fill: int) -> bytes:
    stored = lb.compress(raw)
    header = struct.pack(
        "<BBhI4sI",
        type_id,
        1,
        user_id,
        lb.HEADER_SIZE + len(stored),
        extension.encode("ascii").ljust(4, b"\0"),
        len(raw),
    )
    data = header + stored
    return data + bytes((fill,)) * (lb._align(len(data)) - len(data))


def _archive(resources: list[tuple[int, int, str, bytes]]) -> bytes:
    blocks = [
        _block(type_id, user_id, extension, raw, 0xA0 + index)
        for index, (type_id, user_id, extension, raw) in enumerate(resources)
    ]
    return b"".join(blocks) + lb.END_HEADER + bytes(lb.ALIGNMENT - lb.HEADER_SIZE)


def _source(base: bytes, resources: list[tuple[int, int, str, bytes]], replacement: str) -> lb.Source:
    entries = []
    for index, (type_id, user_id, extension, raw) in enumerate(resources):
        entries.append(
            lb.SourceEntry(
                type_id,
                True,
                user_id,
                extension,
                len(raw),
                hashlib.sha1(raw).hexdigest(),
                replacement if index == 1 else "base",
            )
        )
    return lb.Source(hashlib.sha1(base).hexdigest(), len(base), tuple(entries))


class LbCodecTests(unittest.TestCase):
    def test_decoder_covers_every_opcode(self) -> None:
        stream = bytearray()
        expected = bytearray()

        stream += lb._opcode(0, 3, b"abc")
        expected += b"abc"
        stream += lb._opcode(1, 2)
        expected += b"\0\0"
        stream += lb._opcode(2, 3, b"Z")
        expected += b"ZZZ"
        stream += lb._opcode(3, 3, b"\x03")
        for _ in range(3):
            expected.append(expected[-3])

        literal = bytes(range(250))
        stream += lb._opcode(0, len(literal), literal)
        expected += literal
        stream += lb._opcode(4, 4, struct.pack("<H", 256))
        for _ in range(4):
            expected.append(expected[-256])

        stream += lb._opcode(5, 3, b"ABC")
        expected += b"A\0B\0C\0"
        stream.append(0xFF)
        self.assertEqual(lb.decompress(bytes(stream), len(expected)), expected)

    def test_compressor_is_deterministic_and_round_trips(self) -> None:
        raw = (
            bytes(180)
            + b"field-path-001" * 40
            + b"\x01\0\x02\0\x03\0" * 80
            + bytes(range(256)) * 3
        )
        encoded = lb.compress(raw)
        self.assertEqual(encoded, lb.compress(raw))
        self.assertEqual(lb.decompress(encoded, len(raw)), raw)
        self.assertLess(len(encoded), len(raw))

    def test_uncompressed_entry_may_use_zero_for_decompressed_size(self) -> None:
        raw = b"raw texture bundle"
        header = struct.pack("<BBhI4sI", 1, 0, 0, lb.HEADER_SIZE + len(raw), b"TBN\0", 0)
        block = header + raw
        block += bytes(lb._align(len(block)) - len(block))
        archive = lb.parse_archive(block + lb.END_HEADER + bytes(lb.ALIGNMENT - lb.HEADER_SIZE))
        self.assertEqual(archive.entries[0].raw_size, 0)
        self.assertEqual(lb.entry_data(archive.entries[0]), raw)
        self.assertIn("raw_size=0x0", lb.render_source(block + lb.END_HEADER + bytes(lb.ALIGNMENT - lb.HEADER_SIZE)))

    def test_unchanged_replacement_preserves_the_archive_exactly(self) -> None:
        resources = [
            (1, 0, "TBN", b"texture" * 100),
            (1, 7, "F2", b"field" * 160),
        ]
        base = _archive(resources)
        with tempfile.TemporaryDirectory() as temp:
            directory = Path(temp)
            (directory / "field.f2").write_bytes(resources[1][3])
            rebuilt = lb.assemble(_source(base, resources, "field.f2"), base, directory)
        self.assertEqual(rebuilt, base)

    def test_changed_replacement_rebuilds_only_its_resource(self) -> None:
        resources = [
            (1, 0, "TBN", b"texture" * 100),
            (1, 7, "F2", b"field" * 160),
        ]
        replacement = b"changed-field" * 100
        base = _archive(resources)
        with tempfile.TemporaryDirectory() as temp:
            directory = Path(temp)
            (directory / "field.f2").write_bytes(replacement)
            rebuilt = lb.assemble(_source(base, resources, "field.f2"), base, directory)

        self.assertNotEqual(rebuilt, base)
        original = lb.parse_archive(base)
        changed = lb.parse_archive(rebuilt)
        self.assertEqual(changed.entries[0].block, original.entries[0].block)
        self.assertEqual(lb.entry_data(changed.entries[0]), resources[0][3])
        self.assertEqual(lb.entry_data(changed.entries[1]), replacement)

    def test_wrong_retail_base_is_rejected(self) -> None:
        resources = [(1, 0, "F2", b"field" * 40)]
        base = _archive(resources)
        source = _source(base, resources + [(1, 0, "F1", b"unused")], "field.f2")
        with self.assertRaisesRegex(lb.LbError, "entry count"):
            lb.assemble(source, base, Path("."))

    def test_tracked_sources_are_well_formed(self) -> None:
        expected = {
            "dds1": "3937f9b1d070d83657220806c3abd6a90b21e4b4",
            "dds2": "99d8c73ae4dc27d28a38dd24234f64f80506e4dc",
        }
        for game, digest in expected.items():
            path = ROOT / "src" / game / "data" / "field" / "f011_001.lbasm"
            with self.subTest(game=game):
                source = lb.parse_source(path.read_text(encoding="utf-8"))
                self.assertEqual(source.base_sha1, digest)
                self.assertEqual([entry.extension for entry in source.entries], ["TBN", "F2", "F1"])
                self.assertEqual(source.entries[1].source, "f011_001.f2")


if __name__ == "__main__":
    unittest.main()
