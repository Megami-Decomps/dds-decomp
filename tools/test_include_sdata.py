#!/usr/bin/env python3
"""Regression tests for small-data source ownership detection."""

import os
import sys
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from tools import include_sdata  # noqa: E402


class IncludeSdataTests(unittest.TestCase):
    def test_preserves_native_store_labels_replaced_by_array_indexing(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            for folder in ["config/dds1", "src/dds1/game", "asm/dds1/data/game", "asm/dds1/game"]:
                (root / folder).mkdir(parents=True)
            (root / "config/dds1/symbol_addrs.txt").write_text("")
            source = root / "src/dds1/game/test.c"
            source.write_text(
                'u32 D_00001008 = 3;\n'
                'void reset(void) {\n'
                '    byteArray[3] = 0;\n'
                '    puts("abc");\n'
                '    D_00001008 = 0;\n'
                '}\n'
            )
            (root / "asm/dds1/game/test.s").write_text(
                'glabel reset\n'
                '    /* 0000 00002000 */ sb $0, %lo(D_00001000)($3)\n'
                '    /* 0004 00002004 */ addiu $4, $4, %lo(D_00001004)\n'
                '    /* 0008 00002008 */ sw $0, %gp_rel(D_00001008)($28)\n'
                'endlabel reset\n'
            )
            (root / "asm/dds1/data/game/test.sdata.s").write_text(
                'dlabel D_00001000\n'
                '    /* 0000 00001000 */ .byte 0xFF\n'
                'enddlabel D_00001000\n'
                'dlabel D_00001004\n'
                '    /* 0004 00001004 */ .asciz "abc"\n'
                'enddlabel D_00001004\n'
                'dlabel D_00001008\n'
                '    /* 0008 00001008 */ .word 3\n'
                'enddlabel D_00001008\n'
            )
            with patch.object(include_sdata, "ROOT", root):
                include_sdata.place("dds1")
                once = source.read_text()
                self.assertEqual(include_sdata.LINE.findall(once), ["D_00001000"])
                include_sdata.place("dds1")
                self.assertEqual(source.read_text(), once)

    def test_finds_plain_and_attribute_decorated_definitions(self) -> None:
        text = """\
s32 plainValue = 1;
static u8 arrayValue[4];
CallbackTable callbackTable __attribute__((section(".sdata"))) = {
    callbackA,
    callbackB,
};
void (*tickCallback)(void) __attribute__((section(".sdata"))) = NULL;
"""
        self.assertEqual(
            include_sdata.DATA_DEF.findall(text),
            ["plainValue", "arrayValue", "callbackTable", "tickCallback"],
        )

    def test_ignores_declarations_and_type_definitions(self) -> None:
        text = """\
extern s32 externalValue;
typedef struct CallbackTable CallbackTable;
extern void (*externalCallback)(void);
typedef void (*Callback)(void);
void callback(void);
"""
        self.assertEqual(include_sdata.DATA_DEF.findall(text), [])


class IncludeSdataPlacementTests(unittest.TestCase):
    def _make_tree(
        self,
        root: Path,
        *,
        named_array: bool = False,
        alias_byte: str = "0xFF",
        define_alias: bool = False,
    ) -> None:
        (root / "config/dds1").mkdir(parents=True)
        source_dir = root / "src/dds1/game"
        source_dir.mkdir(parents=True)
        data_dir = root / "asm/dds1/data/game"
        data_dir.mkdir(parents=True)
        asm_dir = root / "asm/dds1"
        asm_dir.mkdir(parents=True, exist_ok=True)
        (asm_dir / "game").mkdir()

        array_name = "namedBytes" if named_array else "D_003BA860"
        (root / "config/dds1/symbol_addrs.txt").write_text(
            f"{array_name} = 0x003BA860;\n"
        )
        (source_dir / "motor.c").write_text(
            ("u8 D_003BA85F = 0xFF;\n" if define_alias else "")
            + f"extern u8 {array_name}[2][2][2];\n"
            "extern int printf(const char *, ...);\n"
            "void motor(void) {\n"
            f"    {array_name}[0][0][0] = 1;\n"
            '    printf("on");\n'
            "}\n"
        )
        (asm_dir / "game/motor.s").write_text(
            "glabel motor\n"
            "    lui $2, %hi(D_003BA85F)\n"
            "    addiu $2, $2, %lo(D_003BA85F)\n"
            "    sb $0, %lo(D_003BA85F)($3)\n"
            "    lui $4, %hi(D_00400000)\n"
            "    addiu $4, $4, %lo(D_00400000)\n"
            "endlabel motor\n"
        )
        (data_dir / "motor.sdata.s").write_text(
            ".section .sdata\n"
            ".align 0\n"
            "dlabel D_003BA85F\n"
            f"/* 00000000 003BA85F */ .byte {alias_byte}\n"
            "enddlabel D_003BA85F\n"
            ".align 2\n"
            "dlabel D_003BA860\n"
            "/* 00000004 003BA860 */ .byte 1, 2, 3, 4, 5, 6, 7, 8\n"
            "enddlabel D_003BA860\n"
            ".align 0\n"
            "dlabel D_00400000\n"
            '/* 0000000C 00400000 */ .asciz "on"\n'
            "enddlabel D_00400000\n"
        )

    def _place(self, root: Path) -> str:
        with patch.object(include_sdata, "ROOT", root):
            include_sdata.place("dds1")
        return (root / "src/dds1/game/motor.c").read_text()

    def test_one_byte_before_c_array_is_included_and_literal_is_c_owned(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            self._make_tree(root)

            source = self._place(root)

            alias_marker = 'INCLUDE_SDATA(const s32, "game/motor", D_003BA85F);'
            self.assertIn(alias_marker, source)
            self.assertNotIn("D_00400000);", source)
            self.assertEqual(
                (root / "asm/dds1/nonmatchings/game/motor/D_003BA85F.s").read_text(),
                ".section .sdata\n\n.align 0\nnonmatching D_003BA85F\n\n"
                "dlabel D_003BA85F\n/* 00000000 003BA85F */ .byte 0xFF\n"
                "enddlabel D_003BA85F\n",
            )

            source_path = root / "src/dds1/game/motor.c"
            leaf_path = root / "asm/dds1/nonmatchings/game/motor/D_003BA85F.s"
            os.utime(source_path, ns=(1, 1))
            os.utime(leaf_path, ns=(1, 1))
            first_placement = source
            second_placement = self._place(root)
            self.assertEqual(second_placement, first_placement)
            self.assertEqual(source_path.stat().st_mtime_ns, 1)
            self.assertEqual(leaf_path.stat().st_mtime_ns, 1)

    def test_zero_byte_alias_and_config_named_array_keep_the_anchor(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            self._make_tree(root, named_array=True, alias_byte="0x00")

            source = self._place(root)

            self.assertIn(
                'INCLUDE_SDATA(const s32, "game/motor", D_003BA85F);', source
            )
            self.assertEqual(
                (root / "asm/dds1/nonmatchings/game/motor/D_003BA85F.s").read_text()
                .split(".byte ", 1)[1]
                .splitlines()[0],
                "0x00",
            )
            self.assertNotIn("D_00400000);", source)

    def test_actual_c_defined_predecessor_is_not_reincluded(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            self._make_tree(root, define_alias=True)

            source = self._place(root)

            self.assertIn("u8 D_003BA85F = 0xFF;", source)
            self.assertNotIn("D_003BA85F);", source)


if __name__ == "__main__":
    unittest.main()
