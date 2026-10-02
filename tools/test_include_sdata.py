#!/usr/bin/env python3
"""Regression tests for small-data source ownership detection."""

import unittest

import include_sdata


class IncludeSdataTests(unittest.TestCase):
    def test_finds_plain_and_attribute_decorated_definitions(self) -> None:
        text = """\
s32 plainValue = 1;
static u8 arrayValue[4];
CallbackTable callbackTable __attribute__((section(".sdata"))) = {
    callbackA,
    callbackB,
};
"""
        self.assertEqual(
            include_sdata.DATA_DEF.findall(text),
            ["plainValue", "arrayValue", "callbackTable"],
        )

    def test_ignores_declarations_and_type_definitions(self) -> None:
        text = """\
extern s32 externalValue;
typedef struct CallbackTable CallbackTable;
"""
        self.assertEqual(include_sdata.DATA_DEF.findall(text), [])


if __name__ == "__main__":
    unittest.main()
