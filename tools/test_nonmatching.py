"""Source scans must see the same fallback branch as the matching build."""
import unittest
from pathlib import Path
import subprocess
import tempfile

from nonmatching import matching_source
from check_nonmatching import compile_unit, ROOT


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


@unittest.skipUnless((ROOT / "tools/compilers/ee-gcc2.96").is_dir(), "ee-gcc not installed")
class ImplicitDeclarationTests(unittest.TestCase):
    def test_retained_c_requires_a_declaration_before_use(self):
        with tempfile.TemporaryDirectory() as directory:
            source = Path(directory) / "unit.c"
            output = Path(directory) / "unit.o"
            source.write_text(
                '#ifdef NON_MATCHING\n'
                'int retained(void) { return undeclared(1); }\n'
                '#else\nint matching(void) { return 0; }\n#endif\n')
            rejected = compile_unit(source, output, "dds1")
            self.assertNotEqual(rejected.returncode, 0, rejected.stderr)
            self.assertIn("implicit declaration", rejected.stderr)
            # The matching compiler invocation does not select the retained C.
            normal = subprocess.run([str(ROOT / "tools/cc.sh"), "-DSKIP_ASM",
                                     str(source), "-o", str(output)],
                                    cwd=ROOT, capture_output=True, text=True)
            self.assertEqual(normal.returncode, 0, normal.stderr)

    def test_early_prototype_allows_the_retained_call(self):
        with tempfile.TemporaryDirectory() as directory:
            source = Path(directory) / "unit.c"
            source.write_text('extern int declared(int);\n'
                              'int retained(void) { return declared(1); }\n')
            result = compile_unit(source, Path(directory) / "unit.o", "dds1")
            self.assertEqual(result.returncode, 0, result.stderr)


if __name__ == '__main__':
    unittest.main()
