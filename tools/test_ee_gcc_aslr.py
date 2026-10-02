#!/usr/bin/env python3
"""Regression tests for the EE GCC 2.96 ASLR patch layout."""

from __future__ import annotations

import struct
import unittest

import ee_gcc_aslr as patch


class EeGccAslrTests(unittest.TestCase):
    def fixture(self) -> bytes:
        image = bytearray(b"\xcc" * (patch.WRAPPER_FILE_OFFSET + len(patch.PAYLOAD)))
        image[
            patch.PROGRAM_HEADER_FILE_OFFSET :
            patch.PROGRAM_HEADER_FILE_OFFSET + len(patch.ORIGINAL_PROGRAM_HEADER)
        ] = patch.ORIGINAL_PROGRAM_HEADER
        image[
            patch.WRAPPER_FILE_OFFSET : patch.WRAPPER_FILE_OFFSET + len(patch.PAYLOAD)
        ] = patch.ORIGINAL_PAYLOAD_BYTES
        for site, instruction in patch.CALL_SITES.items():
            offset = site - patch.IMAGE_BASE
            image[offset : offset + len(instruction)] = instruction
        return bytes(image)

    def test_patch_layout_and_only_expected_bytes_change(self) -> None:
        source = self.fixture()
        result = patch.patch_image(source, verify_hash=False)

        header = struct.unpack_from("<8I", result, patch.PROGRAM_HEADER_FILE_OFFSET)
        self.assertEqual(
            header,
            (
                1,
                patch.WRAPPER_FILE_OFFSET,
                patch.WRAPPER_VA,
                patch.WRAPPER_VA,
                len(patch.PAYLOAD),
                len(patch.PAYLOAD),
                5,
                0x1000,
            ),
        )
        self.assertEqual(
            result[patch.WRAPPER_FILE_OFFSET : patch.WRAPPER_FILE_OFFSET + len(patch.PAYLOAD)],
            patch.PAYLOAD,
        )
        for site in patch.CALL_SITES:
            offset = site - patch.IMAGE_BASE
            expected = b"\xe8" + struct.pack("<i", patch.WRAPPER_VA - (site + 5))
            self.assertEqual(result[offset : offset + 5], expected)

        changed = {index for index, pair in enumerate(zip(source, result)) if pair[0] != pair[1]}
        allowed = set(range(
            patch.PROGRAM_HEADER_FILE_OFFSET,
            patch.PROGRAM_HEADER_FILE_OFFSET + len(patch.ORIGINAL_PROGRAM_HEADER),
        ))
        allowed.update(range(
            patch.WRAPPER_FILE_OFFSET,
            patch.WRAPPER_FILE_OFFSET + len(patch.PAYLOAD),
        ))
        for site in patch.CALL_SITES:
            allowed.update(range(site - patch.IMAGE_BASE, site - patch.IMAGE_BASE + 5))
        self.assertLessEqual(changed, allowed)

    def test_refuses_changed_call_site(self) -> None:
        image = bytearray(self.fixture())
        site = next(iter(patch.CALL_SITES)) - patch.IMAGE_BASE
        image[site] ^= 1
        with self.assertRaisesRegex(patch.PatchError, "refusing patch"):
            patch.patch_image(bytes(image), verify_hash=False)

    def test_refuses_unknown_complete_image(self) -> None:
        with self.assertRaisesRegex(patch.PatchError, "unknown compiler"):
            patch.patch_image(self.fixture())


if __name__ == "__main__":
    unittest.main()
