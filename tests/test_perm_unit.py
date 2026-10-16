"""Offline importer command regression; no vendor compiler or game data required."""
import argparse
import json
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
from unittest import mock

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
import perm_unit


class AssemblerFlagsTests(unittest.TestCase):
    def test_target_and_candidate_reassembly_preserve_production_debug_flag(self):
        for mode in ('TARGET', 'candidate', 'fast_candidate'):
            with self.subTest(mode=mode), tempfile.TemporaryDirectory() as tmp:
                directory = Path(tmp)
                (directory / 'meta.json').write_text(json.dumps({
                    'func': 'example', 'version': 'dds2',
                    'unit': 'src/dds2/game/example.c',
                    'include_asm': 'INCLUDE_ASM(const int, "game/example", example);',
                }))
                (directory / 'template.c').write_text('@@PERM@@\n')
                if mode == 'fast_candidate':
                    (directory / 'template_fast.c').write_text('@@PERM@@\n')
                source = directory / 'candidate.c'
                source.write_text('int example(void) { return 1; }\n')
                output = directory / 'output.o'
                calls = []

                def run(command, **kwargs):
                    calls.append(command)
                    if len(calls) == 1:
                        Path(kwargs['env']['DDS_KEEP_S']).write_text(
                            '.text\n.ent example\nexample:\n jr $31\n nop\n.end example\n')
                    return subprocess.CompletedProcess(command, 0, '', '')

                with mock.patch.object(perm_unit.subprocess, 'run', side_effect=run):
                    perm_unit.cmd_cc(argparse.Namespace(
                        dir=str(directory), input='TARGET' if mode == 'TARGET' else str(source),
                        output=str(output)))
                self.assertEqual(len(calls), 2)
                self.assertEqual(calls[1][:-1], [
                    perm_unit.AS, '-EL', '-G8', '-g', '-Iinclude', '-o', output.resolve()])
                self.assertEqual(Path(calls[1][-1]).name, 'f.s')


if __name__ == '__main__':
    unittest.main()
