#!/usr/bin/env python3
"""Regression tests for named C-owned rodata and split-stable placement."""

import contextlib
import io
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch

import include_rodata


class IncludeRodataTests(unittest.TestCase):
    def setUp(self) -> None:
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        self.root = Path(temporary.name)
        root_patch = patch.object(include_rodata, "ROOT", self.root)
        root_patch.start()
        self.addCleanup(root_patch.stop)
        self.source = self.root / "src/dds1/game/testUnit.c"
        self.nonmatchings = self.root / "asm/dds1/nonmatchings/game/testUnit"
        self.matchings = self.root / "asm/dds1/matchings/game/testUnit"
        self.full_rodata = self.root / "asm/dds1/data/game/testUnit.rodata.s"
        self.disassembly = self.root / "asm/dds1/game/testUnit.s"
        for path in (self.source.parent, self.nonmatchings, self.matchings,
                     self.full_rodata.parent, self.disassembly.parent):
            path.mkdir(parents=True, exist_ok=True)

    @staticmethod
    def block(symbol: str, address: int) -> str:
        return (
            f".align 3\nnonmatching {symbol}\n\ndlabel {symbol}\n"
            f"    /* 100000 {address:08X} 00000000 */ .word 0\n"
            f"enddlabel {symbol}\n"
        )

    def prepare(self, source: str, symbols: list[tuple[str, int]],
                disassembly: str = "") -> None:
        self.source.write_text(source)
        blocks = []
        for symbol, address in symbols:
            block = self.block(symbol, address)
            blocks.append(block)
            (self.nonmatchings / f"{symbol}.s").write_text(".section .rodata\n\n" + block)
        self.full_rodata.write_text(".section .rodata\n\n" + "\n".join(blocks))
        self.disassembly.write_text(disassembly)

    def place(self) -> str:
        with contextlib.redirect_stdout(io.StringIO()):
            include_rodata.place("dds1")
        return self.source.read_text()

    def assert_stable(self, first: str) -> None:
        self.assertEqual(self.place(), first)

    def test_aligned_array_definition_keeps_its_matching_rodata_owner(self) -> None:
        definition = "const f32 D_00300000[4] __attribute__((aligned(16))) = {0, 1, 0, 0};"
        self.prepare(
            "extern const f32 D_00300000[4] __attribute__((aligned(16)));\n"
            "void useVector(void) {\n    load(D_00300000);\n}\n\n"
            'INCLUDE_RODATA(const s32, "game/testUnit", D_00300000);\n\n'
            + definition + "\n",
            [("D_00300000", 0x00300000)],
            "glabel useVector\n    addiu $2, $2, %lo(D_00300000)\nendlabel useVector\n",
        )
        (self.matchings / "useVector.s").write_text(
            ".section .rodata\n\n" + self.block("D_00300000", 0x00300000)
            + ".section .text\n\nglabel useVector\nendlabel useVector\n"
        )
        first = self.place()
        self.assertIn(definition, first)
        self.assertNotIn("INCLUDE_RODATA", first)
        self.assert_stable(first)

    def test_named_struct_definition_can_be_referenced_by_asm(self) -> None:
        definition = "const BattlePanelColors panelColors = {{0x80808080, 0x80808080, 0x80808080, 0x80808080}};"
        self.prepare(
            'INCLUDE_ASM(const s32, "game/testUnit", drawPanel);\n\n'
            + definition + "\n",
            [("panelColors", 0x00300000)],
            "glabel drawPanel\n    addiu $2, $2, %lo(panelColors)\nendlabel drawPanel\n",
        )
        (self.nonmatchings / "drawPanel.s").write_text(
            ".section .text\n\nglabel drawPanel\nendlabel drawPanel\n"
        )
        first = self.place()
        self.assertIn(definition, first)
        self.assertIn("INCLUDE_ASM", first)
        self.assertNotIn("INCLUDE_RODATA", first)
        self.assert_stable(first)

    def test_definition_anchors_neighboring_includes_by_address(self) -> None:
        definition = "const u32 D_00300010[4] = {1, 2, 3, 4};"
        self.prepare(definition + "\n", [
            ("D_00300000", 0x00300000),
            ("D_00300010", 0x00300010),
            ("D_00300020", 0x00300020),
        ])
        first = self.place()
        preceding = 'INCLUDE_RODATA(const s32, "game/testUnit", D_00300000);'
        following = 'INCLUDE_RODATA(const s32, "game/testUnit", D_00300020);'
        self.assertLess(first.index(preceding), first.index(definition))
        self.assertLess(first.index(definition), first.index(following))
        self.assertNotIn('"game/testUnit", D_00300010);', first)
        self.assert_stable(first)

    def test_extern_declaration_still_needs_an_include(self) -> None:
        self.prepare(
            "extern const f32 D_00300000[4] __attribute__((aligned(16)));\n"
            "void useVector(void) {\n    load(D_00300000);\n}\n",
            [("D_00300000", 0x00300000)],
            "glabel useVector\n    addiu $2, $2, %lo(D_00300000)\nendlabel useVector\n",
        )
        first = self.place()
        self.assertIn('INCLUDE_RODATA(const s32, "game/testUnit", D_00300000);', first)
        self.assert_stable(first)

    def test_function_literal_ownership_is_unchanged(self) -> None:
        self.prepare(
            'void useLiteral(void) {\n    print("message");\n}\n',
            [("D_00300000", 0x00300000)],
            "glabel useLiteral\n    addiu $2, $2, %lo(D_00300000)\nendlabel useLiteral\n",
        )
        first = self.place()
        self.assertNotIn("INCLUDE_RODATA", first)
        self.assert_stable(first)

    def test_definition_outside_unit_cannot_hide_unit_rodata(self) -> None:
        self.prepare(
            "const u32 D_00400000 = 1;\n"
            "void useVector(void) {\n    load(D_00300000);\n}\n",
            [("D_00300000", 0x00300000)],
            "glabel useVector\n    addiu $2, $2, %lo(D_00300000)\nendlabel useVector\n",
        )
        first = self.place()
        self.assertIn('INCLUDE_RODATA(const s32, "game/testUnit", D_00300000);', first)
        self.assert_stable(first)


if __name__ == "__main__":
    unittest.main()
