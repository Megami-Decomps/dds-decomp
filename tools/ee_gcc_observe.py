#!/usr/bin/env python3
"""Observe one frozen EE GCC case; prove same-path original-QEMU byte equality.

Use only a locally reviewed JSON configuration. Commands run without a shell.
The manifest is reproducibility evidence, not a sandbox for untrusted commands.
"""
import argparse
import contextlib
import hashlib
import json
import os
from pathlib import Path, PurePosixPath
import re
import shutil
import socket
import stat
import struct
import subprocess
import tempfile
import time

from _ee_gcc_observer import GatedGlobalTracer, OperandTracer
from ee_gcc_qemu_loopback import SOURCE_SHA256, PATCHED_SHA256

COMPILER_SHA256 = 'd11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1'
ALLOWED_COMMAND = re.compile(r'(?:qSupported|\?|g|c|s|m[0-9a-f]+,[0-9a-f]+|[Zz]0,[0-9a-f]+,1)')
MAX_PACKET = 16 * 1024 * 1024
# Deliberately narrow: recorded compiler/runtime settings, never ambient secrets.
ENVIRONMENT_KEYS = frozenset({
    'PATH', 'HOME', 'LANG', 'LC_ALL', 'TZ', 'TMPDIR',
    'DDS_I386_LIBDIR', 'DDS_VERSION', 'DDS_AS_UNIT',
})


def sha(path):
    digest = hashlib.sha256()
    with Path(path).open('rb') as handle:
        for block in iter(lambda: handle.read(1024 * 1024), b''):
            digest.update(block)
    return digest.hexdigest()


def decode_payload(payload):
    """Decode RSP escaping and run-length encoding after wire checksum validation."""
    result = bytearray()
    index = 0
    while index < len(payload):
        char = payload[index]
        if char in (125, 42):
            index += 1
            if index == len(payload):
                raise ValueError('Truncated RSP escape or repeat')
            if char == 125:
                result.append(payload[index] ^ 32)
            else:
                count = payload[index] - 29
                if not result or count < 3 or payload[index] in b'$#+-':
                    raise ValueError('Invalid RSP repeat')
                result.extend([result[-1]] * count)
        else:
            result.append(char)
        if len(result) > MAX_PACKET:
            raise ValueError('Oversized decoded RSP packet')
        index += 1
    return result.decode('ascii')


class RSP:
    """Only guest reads, QEMU virtual breakpoints and execution control are allowed."""
    def __init__(self, connection, deadline=None):
        self.s = connection
        self.buf = b''
        self.deadline = deadline
        if connection.family == socket.AF_INET:
            connection.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
        connection.settimeout(300)

    def byte(self):
        if self.deadline is not None and time.monotonic() >= self.deadline:
            raise TimeoutError('Observer time limit exceeded')
        if not self.buf:
            if self.deadline is not None:
                self.s.settimeout(min(300, max(0.001, self.deadline - time.monotonic())))
            self.buf = self.s.recv(65536)
            if not self.buf:
                raise EOFError('QEMU debugger disconnected')
        result, self.buf = self.buf[0], self.buf[1:]
        return result

    def packet(self):
        while self.byte() != ord('$'):
            pass
        payload = bytearray()
        while True:
            char = self.byte()
            if char == ord('#'):
                break
            payload.append(char)
            if len(payload) > MAX_PACKET:
                raise ValueError('Oversized RSP packet')
        checksum = bytes([self.byte(), self.byte()])
        if not re.fullmatch(b'[0-9a-fA-F]{2}', checksum):
            raise ValueError('Malformed RSP checksum')
        if sum(payload) % 256 != int(checksum, 16):
            raise ValueError('RSP checksum mismatch')
        result = decode_payload(payload)
        self.s.sendall(b'+')
        return result

    def cmd(self, command):
        # Never replace this with assert: python -O must not disable the boundary.
        if not ALLOWED_COMMAND.fullmatch(command):
            raise ValueError(f'Not an observational RSP command: {command}')
        payload = command.encode('ascii')
        self.s.sendall(b'$' + payload + b'#' + f'{sum(payload) % 256:02x}'.encode())
        return self.packet()

    def mem(self, address, size):
        if not (0 <= size <= MAX_PACKET and 0 <= address <= 0xffffffff):
            raise ValueError('Invalid i386 memory range')
        if (not address and size) or address + size > 0x100000000:
            raise ValueError('Null or overflowing i386 memory range')
        result = bytearray()
        for offset in range(0, size, 1024):
            count = min(1024, size - offset)
            response = self.cmd(f'm{address + offset:x},{count:x}')
            if not re.fullmatch(f'[0-9a-fA-F]{{{count * 2}}}', response):
                raise ValueError(f'Invalid memory reply at {address + offset:x}')
            result.extend(bytes.fromhex(response))
        return bytes(result)

    def u32(self, address):
        return struct.unpack('<I', self.mem(address, 4))[0]

    def ints(self, address, count):
        return list(struct.unpack('<' + 'i' * count, self.mem(address, count * 4)))

    def shorts(self, address, count):
        return list(struct.unpack('<' + 'h' * count, self.mem(address, count * 2)))

    def cstr(self, address):
        if not address:
            return ''
        result = b''
        for offset in range(0, 512, 64):
            block = self.mem(address + offset, 64)
            result += block
            if b'\0' in block:
                return result.split(b'\0')[0].decode(errors='replace')
        return result.decode(errors='replace')

    def regs(self):
        raw = bytes.fromhex(self.cmd('g'))
        if len(raw) < 64:
            raise ValueError('Short i386 register packet')
        names = ('eax', 'ecx', 'edx', 'ebx', 'esp', 'ebp', 'esi', 'edi',
                 'eip', 'eflags', 'cs', 'ss', 'ds', 'es', 'fs', 'gs')
        return dict(zip(names, struct.unpack('<16I', raw[:64])))

    def bp(self, address, enabled):
        response = self.cmd(f'{"Z" if enabled else "z"}0,{address:x},1')
        if response != 'OK':
            raise ValueError(f'Virtual breakpoint failed at {address:x}: {response}')


def exact_keys(record, required, optional=()):
    if not isinstance(record, dict) or set(record) - set(required) - set(optional) or set(required) - set(record):
        raise ValueError(f'Expected keys {sorted(required)} with optional {sorted(optional)}')


def string(value):
    if not isinstance(value, str) or not value or '\0' in value:
        raise ValueError('Expected a nonempty string without NUL')
    return value


def archive_name(value):
    value = string(value)
    path = PurePosixPath(value)
    if path.is_absolute() or any(part in ('', '.', '..') for part in value.split('/')) or '\\' in value:
        raise ValueError('Archive paths must be relative and cannot traverse directories')
    return value


def hash_string(value):
    if not isinstance(value, str) or not re.fullmatch('[0-9a-f]{64}', value):
        raise ValueError('Expected lowercase SHA-256')
    return value


class Case:
    """Fixed invocation, environment, hashed inputs, and narrowly named outputs."""
    def __init__(self, data, base):
        exact_keys(data, {'schema', 'cwd', 'environment', 'compiler', 'qemu', 'assembler',
                         'command', 'assembler_command', 'function', 'inputs',
                         'artifacts', 'logs'}, {'transport', 'timeout_seconds'})
        if type(data['schema']) is not int or data['schema'] != 1:
            raise ValueError('Unsupported case schema')
        self.data = data
        self.base = Path(base).resolve()
        self.cwd = self.path(data['cwd'])
        self.compiler = self.path(data['compiler'])
        self.qemu = self.path(data['qemu'])
        self.assembler_path = self.path(data['assembler'])
        self.function = string(data['function'])
        if not re.fullmatch(r'[A-Za-z_][A-Za-z0-9_.$]*', self.function):
            raise ValueError('Invalid target function')
        env = data['environment']
        if not isinstance(env, dict):
            raise ValueError('environment must be the complete environment object')
        for key, value in env.items():
            string(key)
            if key not in ENVIRONMENT_KEYS:
                raise ValueError('Environment key is outside the compiler/runtime allowlist')
            if not isinstance(value, str) or '\0' in value:
                raise ValueError('Invalid compiler/runtime environment value')
        self.env = dict(env)
        self.command = self.command_list(data['command'])
        self.assembler = self.command_list(data['assembler_command'])
        if self.command[0] != str(self.qemu) or self.assembler[0] != str(self.qemu):
            raise ValueError('Both commands must use the original absolute QEMU path')
        if self.command.count(str(self.compiler)) != 1:
            raise ValueError('Compiler command must contain the exact absolute compiler path once')
        if self.assembler.count(str(self.assembler_path)) != 1:
            raise ValueError('Assembler command must contain its exact absolute path once')
        for command, guest in ((self.command, self.compiler), (self.assembler, self.assembler_path)):
            if any(arg.startswith('-g') for arg in command[1:command.index(str(guest))]):
                raise ValueError('Baseline command cannot start a debugger')
        self.inputs = {}
        if not isinstance(data['inputs'], dict) or not data['inputs']:
            raise ValueError('A nonempty explicit input hash manifest is required')
        for path, expected in data['inputs'].items():
            resolved = self.path(path)
            if resolved in self.inputs:
                raise ValueError('Duplicate input path')
            self.inputs[resolved] = hash_string(expected)
        if self.inputs.get(self.compiler) != COMPILER_SHA256:
            raise ValueError('Pinned compiler hash is required in inputs')
        if self.qemu not in self.inputs or self.assembler_path not in self.inputs:
            raise ValueError('Original QEMU and assembler hashes are required in inputs')
        self.timeout = data.get('timeout_seconds', 1800)
        if type(self.timeout) is not int or not 1 <= self.timeout <= 86400:
            raise ValueError('timeout_seconds must be between 1 and 86400')
        self.transport = data.get('transport', {'kind': 'unix'})
        if self.transport == {'kind': 'unix'}:
            self.observed_qemu = self.qemu
        else:
            exact_keys(self.transport, {'kind', 'qemu'})
            if self.transport['kind'] != 'loopback-copy':
                raise ValueError('Only unix or pinned loopback-copy transport is allowed')
            self.observed_qemu = self.path(self.transport['qemu'])
            if self.inputs.get(self.qemu) != SOURCE_SHA256:
                raise ValueError('Loopback transport requires the pinned original QEMU')
            if self.inputs.get(self.observed_qemu) != PATCHED_SHA256:
                raise ValueError('Loopback copy hash is required in inputs')
        exact_keys(data['logs'], {'cc1_stdout', 'cc1_stderr', 'as_stdout', 'as_stderr'})
        self.logs = {name: self.path(value) for name, value in data['logs'].items()}
        if len(set(self.logs.values())) != 4 or any(path in self.inputs for path in self.logs.values()):
            raise ValueError('Log paths must be distinct outputs, never inputs')
        self.artifacts = data['artifacts']
        if not isinstance(self.artifacts, list) or not self.artifacts:
            raise ValueError('At least one artifact specification is required')
        for spec in self.artifacts:
            if 'path' in spec:
                exact_keys(spec, {'path', 'archive'})
                self.path(spec['path'])
                archive_name(spec['archive'])
            else:
                exact_keys(spec, {'root', 'pattern', 'archive_dir'})
                self.path(spec['root'])
                pattern = string(spec['pattern'])
                if '/' in pattern or '\\' in pattern or pattern in ('.', '..', '*') or '**' in pattern:
                    raise ValueError('Use a narrow single-directory artifact glob')
                archive_name(spec['archive_dir'])
        named = {self.path(spec['path']) for spec in self.artifacts if 'path' in spec}
        if not set(self.logs.values()) <= named:
            raise ValueError('All four logs must be explicitly archived')
        if named & self.inputs.keys():
            raise ValueError('An artifact cannot be a frozen input')

    def path(self, value):
        path = Path(string(value))
        return (self.base / path).resolve() if not path.is_absolute() else path.resolve()

    @staticmethod
    def command_list(value):
        if not isinstance(value, list) or not value:
            raise ValueError('Commands must be nonempty argv arrays')
        return [string(item) for item in value]

    def verify(self):
        if not self.cwd.is_dir():
            raise ValueError('Case cwd does not exist')
        for path, expected in self.inputs.items():
            if sha(path) != expected:
                raise ValueError(f'Input hash changed: {path}')
        # Hash pins the image; the ELF check documents the decoder ABI as well.
        with self.compiler.open('rb') as handle:
            header = handle.read(20)
        if header[:6] != b'\x7fELF\x01\x01' or header[18:20] != b'\x03\x00':
            raise ValueError('The observer requires little-endian i386 ELF cc1')

    def inventory(self, required=True):
        result = {}
        sources = set()
        for spec in self.artifacts:
            if 'path' in spec:
                raw_path = Path(spec['path'])
                raw_path = raw_path if raw_path.is_absolute() else self.base / raw_path
                if raw_path.is_symlink():
                    raise ValueError(f'Artifacts must be regular files: {raw_path}')
                entries = [(self.path(spec['path']), spec['archive'])]
            else:
                entries = [(path, spec['archive_dir'] + '/' + path.name)
                           for path in sorted(self.path(spec['root']).glob(spec['pattern']))]
                if required and not entries:
                    raise ValueError(f'No artifacts match {spec["pattern"]}')
            for source, name in entries:
                if source.is_symlink() or (source.exists() and not source.is_file()):
                    raise ValueError(f'Artifacts must be regular files: {source}')
                if not source.exists():
                    if required:
                        raise ValueError(f'Missing artifact: {source}')
                    continue
                source = source.resolve()
                if source in self.inputs or source in sources or name in result:
                    raise ValueError('Overlapping artifact or frozen input')
                if any(name.startswith(old + '/') or old.startswith(name + '/') for old in result):
                    raise ValueError('Overlapping archive destinations')
                sources.add(source)
                result[name] = source
        return result


def load_case(path):
    path = Path(path).resolve()
    # Duplicate JSON keys are often accidental edits to a frozen manifest.
    def unique(pairs):
        result = {}
        for key, value in pairs:
            if key in result:
                raise ValueError(f'Duplicate JSON key: {key}')
            result[key] = value
        return result
    return Case(json.loads(path.read_text(), object_pairs_hook=unique), path.parent)


def copy_inventory(inventory, destination):
    destination.mkdir()
    for name, source in inventory.items():
        target = destination / name
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(source, target)
    return {name: sha(destination / name) for name in sorted(inventory)}


def compare_artifacts(baseline, observed):
    if set(baseline) != set(observed):
        raise ValueError(f'Artifact inventory differs: missing={sorted(set(baseline) - set(observed))}, '
                         f'extra={sorted(set(observed) - set(baseline))}')
    return {name: baseline[name] == observed[name] for name in sorted(baseline)}


def run_command(case, command, stem):
    with case.logs[stem + '_stdout'].open('wb') as stdout, case.logs[stem + '_stderr'].open('wb') as stderr:
        result = subprocess.run(command, cwd=case.cwd, env=case.env,
                                stdout=stdout, stderr=stderr, timeout=case.timeout)
    if result.returncode:
        raise RuntimeError(f'{stem} failed with status {result.returncode}; see configured stderr')


def unix_supported(help_text):
    # Conservative compatibility gate: require explicit endpoint advertisement.
    # Capable builds with legacy '-g port' help are intentionally rejected here.
    return re.search(r'^-g\s+endpoint\s', help_text, re.MULTILINE) is not None


def process_socket_inodes(pid):
    result = set()
    for descriptor in Path(f'/proc/{pid}/fd').iterdir():
        try:
            target = descriptor.readlink()
        except FileNotFoundError:
            continue
        match = re.fullmatch(r'socket:\[(\d+)\]', str(target))
        if match:
            result.add(match[1])
    return result


def loopback_listener(port, tables, owner_inodes=None):
    """Require at least one IPv4 loopback listener and no other bind for this port."""
    found = []
    for family, text in tables:
        for row in text.splitlines()[1:]:
            fields = row.split()
            if len(fields) < 4:
                continue
            address, actual_port = fields[1].split(':')
            if fields[3] == '0A' and int(actual_port, 16) == port:
                if family != socket.AF_INET or address != '0100007F':
                    raise ValueError('Debugger listener is not exactly IPv4 loopback')
                if owner_inodes is not None and (len(fields) < 10 or fields[9] not in owner_inodes):
                    raise ValueError('Debugger listener is not owned by the spawned QEMU')
                found.append(row)
    return found


@contextlib.contextmanager
def debugger_endpoint(case):
    if case.transport['kind'] == 'unix':
        help_result = subprocess.run([str(case.qemu), '--help'], env=case.env,
                                     capture_output=True, text=True, timeout=10)
        if help_result.returncode or not unix_supported(help_result.stdout):
            raise ValueError('QEMU does not advertise -g endpoint; this conservative gate will not launch Unix transport')
        with tempfile.TemporaryDirectory(prefix='ee-gcc-observe-') as directory:
            path = Path(directory) / 'gdb.sock'
            if len(os.fsencode(path)) >= 104 or ',' in str(path) or '%' in str(path):
                raise ValueError('Unsafe or overlong Unix socket path')
            # Verify host capability before launching. Denied sockets are terminal.
            with socket.socket(socket.AF_UNIX, socket.SOCK_STREAM) as probe:
                probe.bind(str(path))
            path.unlink()
            yield str(path), socket.AF_UNIX, str(path)
    else:
        # This branch requires an explicit config selection and the two pinned hashes.
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as reserve:
            reserve.bind(('127.0.0.1', 0))
            port = reserve.getsockname()[1]
        yield str(port), socket.AF_INET, ('127.0.0.1', port)


def observe(case, output, mode):
    tracer_class = GatedGlobalTracer if mode == 'allocation' else OperandTracer
    with debugger_endpoint(case) as (endpoint, family, address):
        command = [str(case.observed_qemu), '-g', endpoint, *case.command[1:]]
        (output / 'observed-command.json').write_text(json.dumps(command, indent=2) + '\n')
        with case.logs['cc1_stdout'].open('wb') as stdout, case.logs['cc1_stderr'].open('wb') as stderr:
            process = subprocess.Popen(command, cwd=case.cwd, env=case.env, stdout=stdout, stderr=stderr)
            tracer = None
            try:
                deadline = time.monotonic() + min(10, case.timeout)
                while True:
                    if process.poll() is not None:
                        raise RuntimeError('QEMU exited before debugger connection; see configured stderr')
                    if family == socket.AF_UNIX:
                        ready = Path(address).exists()
                        if ready and not stat.S_ISSOCK(Path(address).stat().st_mode):
                            raise ValueError('Debugger endpoint is not a Unix socket')
                    else:
                        tables = [(socket.AF_INET, Path('/proc/net/tcp').read_text())]
                        if Path('/proc/net/tcp6').exists():
                            tables.append((socket.AF_INET6, Path('/proc/net/tcp6').read_text()))
                        rows = loopback_listener(address[1], tables, process_socket_inodes(process.pid))
                        ready = bool(rows)
                        if ready:
                            (output / 'listener.txt').write_text('\n'.join(rows) + '\n')
                            (output / 'listener-owner.json').write_text(json.dumps({
                                'spawned_qemu_pid': process.pid,
                                'listener_inodes': [row.split()[9] for row in rows],
                                'address': '127.0.0.1', 'port': address[1],
                                'ownership_verified_before_connection': True}, indent=2) + '\n')
                    if ready:
                        break
                    if time.monotonic() >= deadline:
                        raise TimeoutError('Local debugger listener did not appear')
                    time.sleep(0.05)
                with socket.socket(family, socket.SOCK_STREAM) as connection:
                    connection.settimeout(10)
                    connection.connect(address)
                    tracer = tracer_class(RSP(connection, deadline=time.monotonic() + case.timeout),
                                          output / 'events.jsonl', case.function)
                    tracer.run()
                if process.wait(timeout=30):
                    raise RuntimeError('Observed compiler exited unsuccessfully')
                (output / 'hook-checks.json').write_text(json.dumps({
                    'verified_static_hooks': [hex(address) for address in sorted(tracer.checked_hooks)],
                    'check_time': 'first breakpoint trap before decoding state'}, indent=2) + '\n')
                return tracer.seq
            finally:
                if tracer is not None and not tracer.f.closed:
                    tracer.f.close()
                if process.poll() is None:
                    process.kill()
                    process.wait()


def run(case, output, mode):
    """Run sequentially in one cwd; no baseline reuse and no artifact overwrites."""
    case.verify()
    output = Path(output).resolve()
    # Archive directories must not become compiler artifacts or overwrite inputs.
    if output == case.cwd or output in case.inputs or output.exists():
        raise ValueError('Output must be a new directory distinct from case cwd and inputs')
    initial = case.inventory(required=False)
    for path in [*initial.values(), *case.inputs, *case.logs.values()]:
        if output == path or output in path.parents:
            raise ValueError('Archive output cannot contain case inputs or artifacts')
    for spec in case.artifacts:
        if 'root' in spec and (output == case.path(spec['root']) or output in case.path(spec['root']).parents):
            raise ValueError('Archive output cannot be an artifact glob root')
    output.mkdir(parents=True)
    (output / 'case.json').write_text(json.dumps(case.data, indent=2) + '\n')
    (output / 'tool-hashes.json').write_text(json.dumps({p.name: sha(p) for p in
        (Path(__file__), Path(__file__).with_name('_ee_gcc_observer.py'),
         Path(__file__).with_name('ee_gcc_qemu_loopback.py'))}, indent=2) + '\n')
    copy_inventory(initial, output / 'preexisting')
    # Preserve preexisting outputs before clearing them; stale files cannot fake equality.
    try:
        for path in initial.values():
            path.unlink()
        for path in case.logs.values():
            path.parent.mkdir(parents=True, exist_ok=True)
        case.verify()
        run_command(case, case.command, 'cc1')
        case.verify()
        run_command(case, case.assembler, 'as')
        case.verify()
        baseline_inventory = case.inventory()
        baseline = copy_inventory(baseline_inventory, output / 'baseline')
        for path in baseline_inventory.values():
            path.unlink()
        case.verify()
        events = observe(case, output, mode)
        case.verify()
        run_command(case, case.assembler, 'as')
        case.verify()
        observed = copy_inventory(case.inventory(), output / 'observed')
        equality = compare_artifacts(baseline, observed)
        receipt = {'schema': 1, 'mode': mode, 'function': case.function,
                   'compiler_sha256': COMPILER_SHA256, 'events': events,
                   'baseline_sha256': baseline, 'observed_sha256': observed,
                   'artifacts_equal': equality, 'all_equal': all(equality.values()),
                   'same_cwd_environment_and_compiler_argv': True,
                   'inputs_verified_before_and_after': True}
        (output / 'equivalence.json').write_text(json.dumps(receipt, indent=2) + '\n')
        if not receipt['all_equal']:
            raise ValueError('Observed artifacts differ; trace is not qualified')
        return receipt
    except BaseException:
        # Retain failure products and restore the pre-run outputs for safe recovery.
        failed = case.inventory(required=False)
        copy_inventory(failed, output / 'failed')
        for path in failed.values():
            path.unlink()
        for name, path in initial.items():
            path.parent.mkdir(parents=True, exist_ok=True)
            shutil.copy2(output / 'preexisting' / name, path)
        raise


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--config', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    parser.add_argument('--dry-run', action='store_true',
                        help='Validate configuration and all input hashes without starting tools or writing files')
    parser.add_argument('--mode', choices=('allocation', 'operands'), default='allocation')
    args = parser.parse_args()
    try:
        case = load_case(args.config)
        if args.dry_run:
            case.verify()
            case.inventory(required=False)
            if args.output.exists():
                raise ValueError('Output directory already exists')
            print(json.dumps({'validated': True, 'mode': args.mode,
                              'function': case.function, 'inputs': len(case.inputs),
                              'transport': case.transport['kind'], 'executed': False}))
            return
        receipt = run(case, args.output, args.mode)
    except (ValueError, OSError, RuntimeError, subprocess.SubprocessError) as error:
        parser.exit(1, f'{error}\n')
    print(json.dumps({'output': str(args.output.resolve()), 'events': receipt['events'],
                      'artifacts': len(receipt['artifacts_equal']), 'all_equal': True}))


if __name__ == '__main__':
    main()
