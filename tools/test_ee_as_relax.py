#!/usr/bin/env python3
"""Regression tests for the EE GAS relaxation patch layout."""

from __future__ import annotations

import struct
import unittest

import ee_as_relax as patch


class EeAsRelaxTests(unittest.TestCase):
    def fixture(self) -> bytes:
        size = max(
            patch.COPY_HELPER_OFFSET + len(patch.COPY_HELPER),
            patch.RELAX_COPY_CALL_OFFSET + len(patch.ORIGINAL_CALL),
        )
        image = bytearray(b"\xcc" * size)
        image[
            patch.COPY_HELPER_OFFSET :
            patch.COPY_HELPER_OFFSET + len(patch.ORIGINAL_HELPER_BYTES)
        ] = patch.ORIGINAL_HELPER_BYTES
        image[
            patch.RELAX_COPY_CALL_OFFSET :
            patch.RELAX_COPY_CALL_OFFSET + len(patch.ORIGINAL_CALL)
        ] = patch.ORIGINAL_CALL
        return bytes(image)

    def test_patch_layout_and_only_expected_bytes_change(self) -> None:
        source = self.fixture()
        result = patch.patch_image(source, verify_hash=False)

        self.assertEqual(
            result[
                patch.COPY_HELPER_OFFSET :
                patch.COPY_HELPER_OFFSET + len(patch.COPY_HELPER)
            ],
            patch.COPY_HELPER,
        )
        expected_call = b"\xe8" + struct.pack(
            "<i", patch.COPY_HELPER_VA - (patch.RELAX_COPY_CALL_VA + 5)
        )
        self.assertEqual(
            result[
                patch.RELAX_COPY_CALL_OFFSET :
                patch.RELAX_COPY_CALL_OFFSET + len(expected_call)
            ],
            expected_call,
        )

        changed = {
            index for index, pair in enumerate(zip(source, result))
            if pair[0] != pair[1]
        }
        allowed = set(range(
            patch.COPY_HELPER_OFFSET,
            patch.COPY_HELPER_OFFSET + len(patch.COPY_HELPER),
        ))
        allowed.update(range(
            patch.RELAX_COPY_CALL_OFFSET,
            patch.RELAX_COPY_CALL_OFFSET + len(patch.PATCHED_CALL),
        ))
        self.assertLessEqual(changed, allowed)

    def test_refuses_changed_copy_site(self) -> None:
        image = bytearray(self.fixture())
        image[patch.COPY_HELPER_OFFSET] ^= 1
        with self.assertRaisesRegex(patch.PatchError, "refusing patch"):
            patch.patch_image(bytes(image), verify_hash=False)

    def test_refuses_changed_call_site(self) -> None:
        image = bytearray(self.fixture())
        image[patch.RELAX_COPY_CALL_OFFSET] ^= 1
        with self.assertRaisesRegex(patch.PatchError, "refusing patch"):
            patch.patch_image(bytes(image), verify_hash=False)

    def test_refuses_unknown_complete_image(self) -> None:
        with self.assertRaisesRegex(patch.PatchError, "unknown assembler"):
            patch.patch_image(self.fixture())


if __name__ == "__main__":
    unittest.main()
