#!/usr/bin/env python3
"""Make a loopback-only debugger copy of one precisely pinned QEMU i386 build."""
import argparse
import hashlib
import json
from pathlib import Path

SOURCE_SHA256 = '3378bb95493e33dc6cb215a8d89c4aeb58d12201b817413a4c0449ffa5abb443'
PATCHED_SHA256 = 'ba9a200e82870b336c942bc38cf898265bb5ca0bf268ea55da1982aad738c7c1'
OFFSET = 0x318798
BEFORE = bytes.fromhex('c7 44 24 34 00 00 00 00')
AFTER = bytes.fromhex('c7 44 24 34 7f 00 00 01')


def patch_bytes(data):
    """Change only sockaddr.sin_addr from INADDR_ANY to 127.0.0.1."""
    if hashlib.sha256(data).hexdigest() != SOURCE_SHA256:
        raise ValueError('QEMU SHA-256 is not the supported build')
    if data[OFFSET:OFFSET + len(BEFORE)] != BEFORE:
        raise ValueError('QEMU instruction differs at the pinned offset')
    result = data[:OFFSET] + AFTER + data[OFFSET + len(BEFORE):]
    if len(result) != len(data) or sum(a != b for a, b in zip(data, result)) != 2:
        raise ValueError('Patch must change exactly two bytes')
    if hashlib.sha256(result).hexdigest() != PATCHED_SHA256:
        raise ValueError('Unexpected patched QEMU SHA-256')
    return result


def make_copy(source, output):
    source, output = Path(source), Path(output)
    data = patch_bytes(source.read_bytes())
    # Exclusive creation: never modify the installed emulator or overwrite a copy.
    with output.open('xb') as handle:
        handle.write(data)
    output.chmod(0o700)
    return {
        'source': str(source.resolve()), 'source_sha256': SOURCE_SHA256,
        'diagnostic_copy': str(output.resolve()), 'diagnostic_sha256': PATCHED_SHA256,
        'instruction_file_offset': hex(OFFSET),
        'instruction_before': BEFORE.hex(), 'instruction_after': AFTER.hex(),
        'changed_bytes': 2,
        'purpose': 'Restrict host debugger bind address to IPv4 loopback; guest compiler unchanged',
        'source_reference': 'https://gitlab.com/qemu-project/qemu/-/blob/v10.0.0/gdbstub/user.c',
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--input', required=True, type=Path)
    parser.add_argument('--output', required=True, type=Path)
    args = parser.parse_args()
    print(json.dumps(make_copy(args.input, args.output), indent=2))


if __name__ == '__main__':
    main()
