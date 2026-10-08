"""Offline tests; no vendor compiler, game data, emulator or open sockets required."""
import copy
import json
from pathlib import Path
import socket
import struct
import subprocess
import sys
import tempfile
import unittest
from unittest import mock

sys.path.insert(0, str(Path(__file__).resolve().parents[1] / 'tools'))
import ee_gcc_observe as observer
import ee_gcc_qemu_loopback as loopback
from _ee_gcc_observer import signed, GatedGlobalTracer, OperandTracer, Tracer, HOOK_BYTES


def frame(payload):
    return b'$' + payload + b'#' + f'{sum(payload) % 256:02x}'.encode()


class FakeSocket:
    family = socket.AF_UNIX

    def __init__(self, data=b''):
        self.data = data
        self.sent = []

    def settimeout(self, value):
        pass

    def setsockopt(self, *args):
        raise AssertionError('TCP option used on a Unix socket')

    def recv(self, size):
        result, self.data = self.data[:3], self.data[3:]
        return result

    def sendall(self, value):
        self.sent.append(value)


class ProtocolTests(unittest.TestCase):
    def test_rejects_all_guest_writes_and_unknown_commands_before_send(self):
        connection = FakeSocket()
        client = observer.RSP(connection)
        for command in ('P7=03000000', 'M8048000,1:cc', 'X8048000,1:a',
                        'G0000', 'vRun', 'qRcmd,71756974', 'monitor quit',
                        'c8048000', 's8048000', 'Z1,8048000,1',
                        'Z0,8048000,2', 'g\nM1,1:cc', 'qSupported:xmlRegisters=i386'):
            with self.subTest(command=command), self.assertRaises(ValueError):
                client.cmd(command)
        self.assertEqual(connection.sent, [])

    def test_boundary_remains_enabled_under_python_optimization(self):
        code = "import ee_gcc_observe as o; r=o.RSP.__new__(o.RSP); r.cmd('P7=00')"
        result = subprocess.run([sys.executable, '-O', '-c', code],
                                cwd=Path(observer.__file__).parent, capture_output=True, text=True)
        self.assertNotEqual(result.returncode, 0)
        self.assertIn('Not an observational RSP command', result.stderr)

    def test_each_allowed_packet_is_framed_and_acknowledged(self):
        for command in ('qSupported', '?', 'g', 'c', 's', 'm8048000,10',
                        'Z0,8048000,1', 'z0,8048000,1'):
            with self.subTest(command=command):
                connection = FakeSocket(b'+' + frame(b'OK'))
                self.assertEqual(observer.RSP(connection).cmd(command), 'OK')
                self.assertEqual(connection.sent, [frame(command.encode()), b'+'])

    def test_escaped_payload_and_repeat(self):
        self.assertEqual(observer.decode_payload(b'A* '), 'AAAA')
        self.assertEqual(observer.decode_payload(b'}\x03}\x04}\x5d}\x0a'), '#$}*')
        connection = FakeSocket(frame(b'0* '))
        self.assertEqual(observer.RSP(connection).packet(), '0000')

    def test_malformed_payload_fails_closed(self):
        for data in (b'}', b'A*', b'* ', b'A*\x1c', b'A*#'):
            with self.subTest(data=data), self.assertRaises(ValueError):
                observer.decode_payload(data)

    def test_bad_checksum_and_eof(self):
        for wire, error in ((b'$OK#00', ValueError), (b'$OK#zz', ValueError),
                            (b'$OK#', EOFError)):
            with self.subTest(wire=wire):
                connection = FakeSocket(wire)
                with self.assertRaises(error):
                    observer.RSP(connection).packet()
                self.assertEqual(connection.sent, [])

    def test_short_memory_response_rejected(self):
        client = observer.RSP(FakeSocket(frame(b'00')))
        with self.assertRaisesRegex(ValueError, 'Invalid memory'):
            client.mem(0x8048000, 4)

    def test_null_overflow_and_negative_reads_rejected(self):
        client = observer.RSP(FakeSocket())
        for address, size in ((0, 1), (0xffffffff, 2), (5, -1), (-1, 1)):
            with self.subTest(address=address, size=size), self.assertRaises(ValueError):
                client.mem(address, size)

    def test_i386_register_packet(self):
        client = observer.RSP(FakeSocket(frame(struct.pack('<16I', *range(16)).hex().encode())))
        regs = client.regs()
        self.assertEqual(regs['eip'], 8)
        self.assertEqual(regs['esp'], 4)
        self.assertEqual(signed(0xffffffff), -1)
        self.assertEqual(signed(0x80000000), -2147483648)

    def test_deadline_ends_observation(self):
        client = observer.RSP(FakeSocket(), deadline=0)
        with self.assertRaises(TimeoutError):
            client.packet()


class HookTests(unittest.TestCase):
    def test_static_hook_opcode_mismatch_refuses_state_decoding(self):
        address = next(iter(HOOK_BYTES))
        reader = mock.Mock()
        reader.mem.return_value = b'bad-code'
        with tempfile.TemporaryDirectory() as directory:
            tracer = Tracer(reader, Path(directory) / 'events.jsonl', 'target')
            self.addCleanup(tracer.f.close)
            with self.assertRaisesRegex(RuntimeError, 'opcode mismatch'):
                tracer.verify_hook(address)
        reader.bp.assert_not_called()

    def test_missing_target_is_a_failure_in_both_modes(self):
        for cls in (GatedGlobalTracer, OperandTracer):
            with self.subTest(mode=cls.__name__), tempfile.TemporaryDirectory() as directory:
                reader = mock.Mock()
                reader.cmd.side_effect = ['', 'S05', 'W00']
                reader.mem.side_effect = lambda address, size: HOOK_BYTES[address]
                tracer = cls(reader, Path(directory) / 'events.jsonl', 'missing_target')
                self.addCleanup(tracer.f.close)
                with self.assertRaises(RuntimeError):
                    tracer.run()


class CaseTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        for name, data in [('cc1', b'compiler'), ('qemu', b'emulator'),
                           ('assembler', b'assembler'), ('source.c', b'int f(void) { return 1; }')]:
            (self.root / name).write_bytes(data)
        self.data = {
            'schema': 1, 'cwd': str(self.root), 'environment': {'LANG': 'C'},
            'compiler': str(self.root / 'cc1'), 'qemu': str(self.root / 'qemu'),
            'assembler': str(self.root / 'assembler'), 'function': 'f',
            'command': [str(self.root / 'qemu'), str(self.root / 'cc1'), 'source.c', '-o', 'out.s'],
            'assembler_command': [str(self.root / 'qemu'), str(self.root / 'assembler'), '-g', 'out.s'],
            'inputs': {str(self.root / name): observer.sha(self.root / name)
                       for name in ['qemu', 'assembler', 'source.c']},
            'artifacts': [{'path': 'out.s', 'archive': 'candidate.s'},
                          {'root': '.', 'pattern': 'rtl.[0-9][0-9].*', 'archive_dir': 'rtl'}],
            'logs': {},
        }
        self.data['inputs'][str(self.root / 'cc1')] = observer.COMPILER_SHA256
        for stem in ['cc1', 'as']:
            for stream in ['stdout', 'stderr']:
                name = stem + '.' + stream
                self.data['logs'][stem + '_' + stream] = name
                self.data['artifacts'].append({'path': name, 'archive': name})

    def case(self, data=None):
        return observer.Case(self.data if data is None else data, self.root)

    def make_artifacts(self):
        (self.root / 'out.s').write_bytes(b'assembly')
        (self.root / 'rtl.00.rtl').write_bytes(b'rtl')
        for path in self.data['logs'].values():
            (self.root / path).write_bytes(b'')

    def test_complete_config_uses_exact_environment_and_original_command(self):
        case = self.case()
        self.assertEqual(case.env, {'LANG': 'C'})
        self.assertEqual(case.command, self.data['command'])
        self.assertEqual(case.transport, {'kind': 'unix'})

    def test_bad_schema_unknown_fields_and_hashes(self):
        mutations = [lambda d: d.update(schema=True), lambda d: d.update(code='import os'),
                     lambda d: d['inputs'].update({d['compiler']: '0' * 64}),
                     lambda d: d['inputs'].pop(d['qemu']),
                     lambda d: d['inputs'].pop(d['assembler']),
                     lambda d: d['inputs'].update({d['qemu']: 'broken'}),
                     lambda d: d.update(command='qemu cc1 source.c'),
                     lambda d: d.update(timeout_seconds=0)]
        for mutate in mutations:
            data = copy.deepcopy(self.data)
            mutate(data)
            with self.subTest(data=data), self.assertRaises(ValueError):
                self.case(data)

    def test_debugger_launch_in_baseline_rejected(self):
        for key in ['command', 'assembler_command']:
            data = copy.deepcopy(self.data)
            data[key][1:1] = ['-g', '1234']
            with self.subTest(key=key), self.assertRaises(ValueError):
                self.case(data)
        data = copy.deepcopy(self.data)
        data['environment']['QEMU_GDB'] = '1234'
        with self.assertRaises(ValueError):
            self.case(data)

    def test_only_documented_environment_keys_are_accepted(self):
        allowed = {'PATH', 'HOME', 'LANG', 'LC_ALL', 'TZ', 'TMPDIR',
                   'DDS_I386_LIBDIR', 'DDS_VERSION', 'DDS_AS_UNIT'}
        self.assertEqual(observer.ENVIRONMENT_KEYS, allowed)
        self.data['environment'] = {key: 'synthetic-fixture-value' for key in allowed}
        self.assertEqual(self.case().env, self.data['environment'])

    def test_disallowed_environment_rejected_before_archive_or_process_creation(self):
        denied = ('AWS_SECRET_ACCESS_KEY', 'OPENAI_API_KEY', 'GITHUB_TOKEN',
                  'PASSWORD', 'SSH_AUTH_SOCK', 'HTTP_PROXY', 'UNRELATED_SETTING',
                  'LD_PRELOAD', 'LD_LIBRARY_PATH', 'QEMU_GDB', 'QEMU_SET_ENV')
        config = self.root / 'rejected-case.json'
        output = self.root / 'never-created'
        for key in denied:
            with self.subTest(key=key):
                data = copy.deepcopy(self.data)
                data['environment'][key] = 'synthetic-secret-do-not-record'
                config.write_text(json.dumps(data))
                argv = ['ee_gcc_observe.py', '--config', str(config), '--output', str(output)]
                with mock.patch.object(sys, 'argv', argv), \
                     mock.patch.object(observer.subprocess, 'run') as execute, \
                     mock.patch.object(observer.subprocess, 'Popen') as launch, \
                     mock.patch.object(observer.Path, 'mkdir') as mkdir, \
                     mock.patch('sys.stderr') as stderr:
                    with self.assertRaises(SystemExit):
                        observer.main()
                execute.assert_not_called()
                launch.assert_not_called()
                mkdir.assert_not_called()
                self.assertFalse(output.exists())
                self.assertNotIn('synthetic-secret-do-not-record', str(stderr.mock_calls))

    def test_public_case_fixture_has_only_explicit_synthetic_environment(self):
        example = Path(__file__).resolve().parents[1] / 'docs/ee-gcc-observer-case.example.json'
        data = json.loads(example.read_text())
        self.assertEqual(data['environment'], {
            'PATH': '/usr/bin:/bin', 'HOME': '/your/frozen-case',
            'LANG': 'C', 'LC_ALL': 'C', 'TZ': 'UTC',
            'TMPDIR': '/your/frozen-case/tmp',
        })
        self.assertLessEqual(set(data['environment']), observer.ENVIRONMENT_KEYS)
        self.assertNotIn('/workspace/', example.read_text())
        self.assertNotIn('/home/agent', example.read_text())

    def test_hash_drift_fails_verification(self):
        case = self.case()
        (self.root / 'source.c').write_bytes(b'changed')
        # Check source first to isolate the expected failure from the fixture compiler.
        case.inputs = {self.root / 'source.c': self.data['inputs'][str(self.root / 'source.c')]}
        with self.assertRaisesRegex(ValueError, 'Input hash changed'):
            case.verify()

    def test_empty_target_is_rejected(self):
        self.data['function'] = ''
        with self.assertRaises(ValueError):
            self.case()

    def test_wrong_i386_elf_fails_after_hash_check(self):
        case = self.case()
        with mock.patch.object(observer, 'sha', side_effect=lambda path: case.inputs[Path(path)]):
            with self.assertRaisesRegex(ValueError, 'i386 ELF'):
                case.verify()

    def test_duplicate_json_keys_rejected(self):
        path = self.root / 'case.json'
        path.write_text('{"schema":1,"schema":1}')
        with self.assertRaisesRegex(ValueError, 'Duplicate JSON'):
            observer.load_case(path)

    def test_path_traversal_and_broad_globs_rejected(self):
        for name in ('../escape', '/tmp/escape', 'foo/../escape', './file', 'a\\b'):
            data = copy.deepcopy(self.data)
            data['artifacts'][0]['archive'] = name
            with self.subTest(name=name), self.assertRaises(ValueError):
                self.case(data)
        for pattern in ('*', '**', '../*', 'rtl/*'):
            data = copy.deepcopy(self.data)
            data['artifacts'][1]['pattern'] = pattern
            with self.subTest(pattern=pattern), self.assertRaises(ValueError):
                self.case(data)

    def test_logs_and_artifacts_cannot_overlap_inputs(self):
        data = copy.deepcopy(self.data)
        data['artifacts'][0]['path'] = 'source.c'
        with self.assertRaises(ValueError):
            self.case(data)
        data = copy.deepcopy(self.data)
        data['logs']['cc1_stdout'] = 'source.c'
        with self.assertRaises(ValueError):
            self.case(data)

    def test_missing_and_duplicate_artifacts_rejected(self):
        with self.assertRaisesRegex(ValueError, 'Missing artifact'):
            self.case().inventory()
        self.make_artifacts()
        data = copy.deepcopy(self.data)
        data['artifacts'].append({'path': 'out.s', 'archive': 'other.s'})
        with self.assertRaisesRegex(ValueError, 'Overlapping artifact'):
            self.case(data).inventory()

    def test_missing_glob_rejected(self):
        self.make_artifacts()
        (self.root / 'rtl.00.rtl').unlink()
        with self.assertRaisesRegex(ValueError, 'No artifacts match'):
            self.case().inventory()

    def test_exact_symlinks_rejected(self):
        self.make_artifacts()
        (self.root / 'out.s').unlink()
        (self.root / 'real-output').write_bytes(b'assembly')
        (self.root / 'out.s').symlink_to(self.root / 'real-output')
        with self.assertRaisesRegex(ValueError, 'regular files'):
            self.case().inventory()

    def test_glob_symlinks_rejected(self):
        self.make_artifacts()
        (self.root / 'rtl.01.fake').symlink_to(self.root / 'source.c')
        with self.assertRaisesRegex(ValueError, 'regular files'):
            self.case().inventory()

    def test_loopback_transport_requires_both_pinned_hashes(self):
        data = copy.deepcopy(self.data)
        data['transport'] = {'kind': 'loopback-copy', 'qemu': 'qemu-copy'}
        with self.assertRaises(ValueError):
            self.case(data)
        data['inputs'][data['qemu']] = loopback.SOURCE_SHA256
        with self.assertRaises(ValueError):
            self.case(data)
        data['inputs']['qemu-copy'] = loopback.PATCHED_SHA256
        self.assertEqual(self.case(data).observed_qemu, self.root / 'qemu-copy')

    def test_baseline_and_observed_inventory_and_byte_equality(self):
        self.assertEqual(observer.compare_artifacts({'a': '1'}, {'a': '1'}), {'a': True})
        self.assertEqual(observer.compare_artifacts({'a': '1'}, {'a': '2'}), {'a': False})
        for observed in ({}, {'a': '1', 'b': '2'}):
            with self.assertRaisesRegex(ValueError, 'inventory differs'):
                observer.compare_artifacts({'a': '1'}, observed)

    def test_runner_clears_stale_outputs_and_restores_on_failure(self):
        self.make_artifacts()
        (self.root / 'out.s').write_bytes(b'original')
        case = self.case()
        calls = []

        def run_command(actual_case, command, stem):
            calls.append((actual_case.cwd, actual_case.env, command, stem))
            self.make_artifacts()

        def fail_observe(actual_case, output, mode):
            self.assertFalse((self.root / 'out.s').exists())
            self.assertFalse((self.root / 'rtl.00.rtl').exists())
            (self.root / 'out.s').write_bytes(b'failed-output')
            raise RuntimeError('synthetic stop')

        with mock.patch.object(case, 'verify'), mock.patch.object(observer, 'run_command', run_command), \
             mock.patch.object(observer, 'observe', fail_observe):
            with self.assertRaisesRegex(RuntimeError, 'synthetic stop'):
                observer.run(case, self.root / 'archive', 'allocation')
        self.assertEqual((self.root / 'out.s').read_bytes(), b'original')
        self.assertEqual((self.root / 'archive/failed/candidate.s').read_bytes(), b'failed-output')
        self.assertEqual(calls[0][:2], (self.root, {'LANG': 'C'}))
        self.assertEqual(calls[0][2], self.data['command'])
        self.assertEqual(calls[1][2], self.data['assembler_command'])

    def test_dry_run_executes_no_commands_and_creates_no_archive(self):
        config = self.root / 'case.json'
        config.write_text(json.dumps(self.data))
        output = self.root / 'dry-run-output'
        argv = ['ee_gcc_observe.py', '--config', str(config), '--output', str(output), '--dry-run']
        with mock.patch.object(sys, 'argv', argv), mock.patch.object(observer.Case, 'verify'), \
             mock.patch.object(observer.subprocess, 'run') as execute, \
             mock.patch.object(observer.subprocess, 'Popen') as launch, \
             mock.patch('builtins.print'):
            observer.main()
        execute.assert_not_called()
        launch.assert_not_called()
        self.assertFalse(output.exists())

    def test_runner_refuses_existing_archive(self):
        with mock.patch.object(observer.Case, 'verify'):
            with self.assertRaisesRegex(ValueError, 'new directory'):
                observer.run(self.case(), self.root, 'allocation')


class TransportTests(unittest.TestCase):
    def test_legacy_port_help_is_rejected_by_conservative_gate(self):
        self.assertFalse(observer.unix_supported('-g port QEMU_GDB wait gdb connection'))
        self.assertTrue(observer.unix_supported('-g endpoint QEMU_GDB wait gdb connection'))

    def test_only_exact_loopback_listener_is_accepted(self):
        def table(address):
            return 'header\n 0: ' + address + ':04D2 00000000:0000 0A rest\n'
        self.assertEqual(len(observer.loopback_listener(1234, [(socket.AF_INET, table('0100007F'))])), 1)
        for family, address in ((socket.AF_INET, '00000000'), (socket.AF_INET, '0200007F'),
                                (socket.AF_INET6, '0' * 32)):
            with self.subTest(address=address), self.assertRaises(ValueError):
                observer.loopback_listener(1234, [(family, table(address))])
        self.assertEqual(observer.loopback_listener(1235, [(socket.AF_INET, table('00000000'))]), [])

    def test_listener_must_belong_to_spawned_process(self):
        table = 'header\n 0: 0100007F:04D2 00000000:0000 0A 0 0 0 0 0 12345\n'
        self.assertEqual(len(observer.loopback_listener(1234, [(socket.AF_INET, table)], {'12345'})), 1)
        with self.assertRaisesRegex(ValueError, 'not owned'):
            observer.loopback_listener(1234, [(socket.AF_INET, table)], {'67890'})

    def test_unix_denial_does_not_try_another_family(self):
        case = mock.Mock(transport={'kind': 'unix'}, qemu=Path('/qemu'), env={})
        result = mock.Mock(returncode=0, stdout='-g endpoint QEMU_GDB wait gdb connection')
        with mock.patch.object(observer.subprocess, 'run', return_value=result), \
             mock.patch.object(observer.socket, 'socket', side_effect=PermissionError('denied')) as create:
            with self.assertRaises(PermissionError):
                with observer.debugger_endpoint(case):
                    self.fail('A denied socket cannot yield an endpoint')
        create.assert_called_once_with(socket.AF_UNIX, socket.SOCK_STREAM)


class QemuPatchTests(unittest.TestCase):
    def test_unknown_qemu_fails_before_any_output(self):
        with tempfile.TemporaryDirectory() as directory:
            source, output = Path(directory) / 'qemu', Path(directory) / 'copy'
            source.write_bytes(b'not the pinned emulator')
            with self.assertRaisesRegex(ValueError, 'SHA-256'):
                loopback.make_copy(source, output)
            self.assertFalse(output.exists())
            self.assertEqual(source.read_bytes(), b'not the pinned emulator')

    def test_exact_two_byte_recipe_and_fail_closed_checks(self):
        data = b'\0' * loopback.OFFSET + loopback.BEFORE + b'\0'
        digest = mock.Mock()
        digest.hexdigest.side_effect = [loopback.SOURCE_SHA256, loopback.PATCHED_SHA256]
        with mock.patch.object(loopback.hashlib, 'sha256', return_value=digest):
            patched = loopback.patch_bytes(data)
        self.assertEqual(patched[loopback.OFFSET:loopback.OFFSET + 8], loopback.AFTER)
        self.assertEqual(len(patched), len(data))
        self.assertEqual(sum(a != b for a, b in zip(data, patched)), 2)
        digest.hexdigest.side_effect = [loopback.SOURCE_SHA256]
        with mock.patch.object(loopback.hashlib, 'sha256', return_value=digest):
            with self.assertRaisesRegex(ValueError, 'instruction differs'):
                loopback.patch_bytes(b'\0' * len(data))
        digest.hexdigest.side_effect = [loopback.SOURCE_SHA256, '0' * 64]
        with mock.patch.object(loopback.hashlib, 'sha256', return_value=digest):
            with self.assertRaisesRegex(ValueError, 'Unexpected patched'):
                loopback.patch_bytes(data)


if __name__ == '__main__':
    unittest.main()
