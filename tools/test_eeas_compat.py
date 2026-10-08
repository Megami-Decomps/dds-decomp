"""Native padding at and within a translation unit's rodata boundary."""
import unittest
from types import SimpleNamespace

from tools.eeas_compat import realign_rodata


class RodataAlignmentTests(unittest.TestCase):
    def convert(self, address, unit_start, preceding=bytes(8)):
        elf = bytearray(0x100)
        offset = address - 0x1000
        elf[offset - 8:offset] = preceding
        pool = SimpleNamespace(
            elf=bytes(elf), segs=None,
            va_to_off=lambda segments, addr: addr - 0x1000,
        )
        text = (
            ".section .rodata\n.align 3\nnonmatching table\n\n"
            f"dlabel table\n    /* 000020 {address:08X} 01000000 */ .word 1\n"
        )
        return realign_rodata(text, pool, unit_start)

    def test_first_block_after_native_padding(self):
        # A preceding C object may omit padding carried by its old asm blob.
        self.assertIn(".align 4", self.convert(0x1020, 0x1020))

    def test_interior_block_after_native_padding(self):
        self.assertIn(".align 4", self.convert(0x1040, 0x1020))

    def test_nonzero_predecessor_keeps_alignment(self):
        self.assertIn(".align 3", self.convert(0x1020, 0x1020, b"\x01" + bytes(7)))

    def test_eight_aligned_unit_keeps_alignment(self):
        self.assertIn(".align 3", self.convert(0x1040, 0x1028))

    def test_block_before_unit_keeps_alignment(self):
        self.assertIn(".align 3", self.convert(0x1020, 0x1040))


if __name__ == "__main__":
    unittest.main()
