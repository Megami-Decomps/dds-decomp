"""Regression coverage for game-specific m2c context preprocessing."""

import unittest

import m2ctx


class GameContextTests(unittest.TestCase):
    def test_dds1_skill_list_uses_retail_layout(self):
        source = m2ctx.ROOT / "src/dds1/game/code_00242608.c"
        content = m2ctx.context(source)
        self.assertIn("s16 skills[16];", content)
        self.assertNotIn("s16 skills[24];", content)

    def test_dds2_skill_list_uses_retail_layout(self):
        source = m2ctx.ROOT / "src/dds2/game/code_0025DA20.c"
        content = m2ctx.context(source)
        self.assertIn("s16 skills[24];", content)
        self.assertNotIn("s16 skills[16];", content)


if __name__ == "__main__":
    unittest.main()
