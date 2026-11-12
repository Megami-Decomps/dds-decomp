#!/usr/bin/env python3
"""Exercise INCLUDE_RODATA through real ee-gcc 2.96 and ee-as, not text mocks.

Requires the normal pinned tool installation; no retail inputs. Set
DDS_SECTION_TEST_ARTIFACTS to retain C, expanded assembly, objects and logs.
"""
import io
import importlib.util
import json
import os
from pathlib import Path
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parent.parent
EE = ROOT / 'tools/compilers/ee-gcc2.96'


@unittest.skipUnless((EE / 'ee/bin/as').is_file(), 'pinned EE tools not installed')
@unittest.skipUnless(importlib.util.find_spec('elftools'), 'pyelftools is not installed')
class IncludeRodataSectionTests(unittest.TestCase):
    def setUp(self):
        keep = os.environ.get('DDS_SECTION_TEST_ARTIFACTS')
        if keep:
            self.directory = Path(keep) / self._testMethodName
            self.directory.mkdir(parents=True, exist_ok=True)
        else:
            temporary = tempfile.TemporaryDirectory()
            self.addCleanup(temporary.cleanup)
            self.directory = Path(temporary.name)
        self.count = 0

    def compile(self, source, payload='.section .rodata\nblob: .byte 0x33\n',
                legacy=False, flags=()):
        self.count += 1
        directory = self.directory / str(self.count)
        directory.mkdir(exist_ok=True)
        (directory / 'blob.s').write_text(payload)
        prefix = ('#undef ASM_ROOT\n#define ASM_ROOT "' + str(directory) + '/"\n'
                  '#include "include_asm.h"\n')
        if legacy:
            prefix += ('#undef INCLUDE_RODATA\n#include "' +
                       str(ROOT / 'tests/fixtures/include_rodata_legacy.h') + '"\n')
        src, obj, asm = (directory / name for name in ('fixture.c', 'fixture.o', 'fixture.s'))
        src.write_text(prefix + source)
        env = dict(os.environ, DDS_KEEP_S=str(asm))
        for key in ('DDS_AS_UNIT', 'DDS_KEEP_LISTING'):
            env.pop(key, None)
        command = [str(ROOT / 'tools/cc.sh'), *flags, str(src), '-o', str(obj)]
        result = subprocess.run(command, cwd=ROOT, env=env, capture_output=True, text=True)
        (directory / 'command.json').write_text(json.dumps(command) + '\n')
        (directory / 'stdout.log').write_text(result.stdout)
        (directory / 'stderr.log').write_text(result.stderr)
        self.assertEqual(result.returncode, 0, result.stderr)
        from elftools.elf.elffile import ELFFile
        elf = ELFFile(io.BytesIO(obj.read_bytes()))
        sections = {s.name: s.data() for s in elf.iter_sections()}
        symbols = {}
        for symbol in elf.get_section_by_name('.symtab').iter_symbols():
            index = symbol['st_shndx']
            if isinstance(index, int):
                section = elf.get_section(index).name
                offset, size = symbol['st_value'], symbol['st_size']
                symbols[symbol.name] = (section, offset, size)
        return sections, symbols, asm.read_text()

    @staticmethod
    def asm(text):
        return '__asm__(' + json.dumps(text) + ');\n'

    def test_compiler_owned_constants_after_include(self):
        source = ('const unsigned char character_table[257] = {1, 2, 3};\n'
                  'INCLUDE_RODATA(const int, "", blob);\n'
                  'extern void consume(const char *, const char *, const char *);\n'
                  'void format(void) {\n'
                  '  static const char blanks[16] = "                ";\n'
                  '  static const char zeroes[16] = "0000000000000000";\n'
                  '  consume(blanks, zeroes, "0123456789abcdef");\n}\n')
        for legacy in (True, False):
            with self.subTest(legacy=legacy):
                sections, symbols, assembly = self.compile(source, legacy=legacy)
                self.assertEqual(symbols['character_table'][0], '.rodata')
                self.assertEqual(symbols['blob'][0], '.rodata')
                expected = '.text' if legacy else '.rodata'
                for name, contents in [('blanks.0', b' ' * 16),
                                       ('zeroes.1', b'0' * 16),
                                       ('$LC0', b'0123456789abcdef\0')]:
                    section, offset, size = symbols[name]
                    self.assertEqual(section, expected)
                    self.assertEqual(sections[section][offset:offset + len(contents)], contents)
                # Verify the triggering compiler state, not merely a mocked asm stream.
                start = assembly.index('#NO_APP', assembly.index('blob.s'))
                end = assembly.index('blanks.0:', start)
                self.assertNotIn('.rdata', assembly[start:end])
                self.assertNotIn('.section', assembly[start:end])

    def check_caller(self, directive, section, subsection=0, payload=None, legacy=False):
        # .word sentinels avoid instruction alignment behavior in .text.
        source = self.asm(directive + '\n.subsection %d\nbefore: .word 0x11111111\n' % subsection)
        source += 'INCLUDE_RODATA(const int, "", blob);\n'
        source += self.asm('after: .word 0x22222222\n' + directive +
                           '\n.subsection 0\nzero: .word 0x66666666\n')
        options = {'legacy': legacy}
        if payload is not None:
            options['payload'] = payload
        sections, symbols, _ = self.compile(source, **options)
        self.assertEqual(symbols['before'][0], section)
        self.assertEqual(symbols['after'][0], section)
        self.assertEqual(symbols['after'][1], symbols['before'][1] + 4)
        self.assertEqual(sections[section][symbols['after'][1]:symbols['after'][1]+4], b'\x22'*4)
        if subsection:
            self.assertLess(symbols['zero'][1], symbols['before'][1])
        self.assertEqual(symbols['blob'][0], '.rodata')
        return sections, symbols

    def test_text_rodata_rdata_data_and_custom_callers(self):
        for directive, section in [('.text', '.text'), ('.rdata', '.rodata'),
                                   ('.section .rodata', '.rodata'), ('.data', '.data'),
                                   ('.section .custom,"aw",@progbits', '.custom')]:
            with self.subTest(directive=directive):
                # If caller is rodata, included bytes intervene legitimately.
                if section == '.rodata':
                    source = self.asm(directive + '\nbefore: .byte 0x11\n')
                    source += 'INCLUDE_RODATA(const int, "", blob);\n'
                    source += self.asm('after: .byte 0x22\n')
                    sections, symbols, _ = self.compile(source)
                    self.assertEqual(symbols['after'][0], section)
                    self.assertEqual(sections[section], b'\x11\x33\x22')
                else:
                    self.check_caller(directive, section)

    def test_restores_nonzero_subsections(self):
        for directive, section in [('.text', '.text'), ('.data', '.data'),
                                   ('.section .rodata', '.rodata'),
                                   ('.section .custom,"aw",@progbits', '.custom')]:
            with self.subTest(section=section):
                self.check_caller(directive, section, subsection=2)

    def test_redundant_sections_and_balanced_nested_pushes(self):
        payload = ('.section .rodata\n.section .rodata\nblob: .byte 0x33\n'
                   '.pushsection .nested,"aw",@progbits\n.subsection 3\n'
                   'nested: .byte 0x44\n.pushsection .rodata\n.byte 0x55\n'
                   '.popsection\nnested_after: .byte 0x66\n.popsection\n.byte 0x77\n')
        sections, symbols = self.check_caller('.data', '.data', 2, payload=payload)
        self.assertEqual(sections['.nested'], b'\x44\x66')
        self.assertEqual(sections['.rodata'], b'\x33\x55\x77')
        self.assertEqual(symbols['nested_after'][1], symbols['nested'][1] + 1)

    def test_old_macro_fails_data_subsection_contract(self):
        source = self.asm('.data\n.subsection 2\nbefore: .word 0x11111111\n')
        source += 'INCLUDE_RODATA(const int, "", blob);\n'
        source += self.asm('after: .word 0x22222222\n')
        sections, symbols, _ = self.compile(source, legacy=True)
        self.assertEqual(symbols['before'][0], '.data')
        self.assertEqual(symbols['after'][0], '.text')
        self.assertEqual(sections['.text'][symbols['after'][1]:symbols['after'][1]+4], b'\x22'*4)

    def test_repeated_includes_preserve_the_same_caller(self):
        source = self.asm('.data\n.subsection 2\nbefore: .word 0x11111111\n')
        source += 'INCLUDE_RODATA(const int, "", blob);\n' * 2
        source += self.asm('after: .word 0x22222222\n.data\n.subsection 0\nzero: .word 0\n')
        sections, symbols, _ = self.compile(source, payload='.section .rodata\n.byte 0x33\n')
        self.assertEqual(symbols['after'][0], '.data')
        self.assertEqual(symbols['after'][1], symbols['before'][1] + 4)
        self.assertLess(symbols['zero'][1], symbols['before'][1])
        self.assertEqual(sections['.rodata'], b'\x33\x33')

    def test_skip_and_context_modes_omit_include(self):
        source = ('const char before[9] = "12345678";\n'
                  'INCLUDE_RODATA(const int, "missing", nonexistent);\n'
                  'const char after[9] = "abcdefgh";\n')
        for mode in ('SKIP_ASM', 'M2CTX', 'PERMUTER'):
            with self.subTest(mode=mode):
                sections, symbols, assembly = self.compile(source, flags=['-D'+mode])
                self.assertNotIn('nonexistent.s', assembly)
                self.assertEqual(symbols['before'][0], '.rodata')
                self.assertEqual(symbols['after'][0], '.rodata')


if __name__ == '__main__':
    unittest.main()
