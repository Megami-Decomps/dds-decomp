#!/usr/bin/env python3
"""Regression tests for small-data source ownership detection."""

import unittest
import tempfile
from pathlib import Path
from unittest.mock import patch

import include_sdata


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


if __name__ == "__main__":
    unittest.main()
