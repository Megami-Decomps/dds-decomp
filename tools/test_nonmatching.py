"""Source scans must see the same fallback branch as the matching build."""
import unittest

from nonmatching import matching_source


class MatchingSourceTests(unittest.TestCase):
    def test_nested_conditionals_and_offsets(self):
        source = ('before\n#ifdef NON_MATCHING\nvoid pending(void) {\n'
                  '#if DETAIL\nfirst();\n#else\nsecond();\n#endif\n}\n'
                  '#else\nINCLUDE_ASM(s32, "unit", pending);\n#endif\nafter\n')
        selected = matching_source(source)
        self.assertNotIn('void pending', selected)
        self.assertNotIn('second()', selected)
        self.assertEqual(len(source), len(selected))
        self.assertEqual(source.index('INCLUDE_ASM'), selected.index('INCLUDE_ASM'))
        self.assertEqual(source.count('\n'), selected.count('\n'))
        self.assertTrue(selected.endswith('after\n'))

    def test_unrelated_conditionals_and_multiple_blocks(self):
        source = ('#if PLATFORM\n#ifdef NON_MATCHING\na();\n#else\n'
                  '#if FEATURE\nb();\n#endif\n#endif\n#endif\n'
                  '#ifdef NON_MATCHING\nc();\n#endif\nd();\n')
        selected = matching_source(source)
        self.assertIn('#if PLATFORM', selected)
        self.assertIn('#if FEATURE\nb();\n#endif', selected)
        self.assertNotIn('a();', selected)
        self.assertNotIn('c();', selected)
        self.assertIn('d();', selected)


if __name__ == '__main__':
    unittest.main()
