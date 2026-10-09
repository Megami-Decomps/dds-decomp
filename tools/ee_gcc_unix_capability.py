#!/usr/bin/env python3
"""Qualify a trusted QEMU Unix debugger without running guest instructions.

Linux x86_64 only. The additional seccomp filter is installed in each child,
never in the observer or host. It denies creation of non-Unix sockets before
QEMU starts, so even a legacy numeric-only -g parser cannot expose TCP.
This is a transport restriction for reviewed tools, not an untrusted-code sandbox.
"""
from __future__ import annotations

import contextlib
import ctypes
import errno
import hashlib
import json
import os
from pathlib import Path
import platform
import re
import socket
import stat
import struct
import subprocess
import sys
import tempfile
import time

COMPILER_SHA256 = "d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1"
METHOD = "linux-x86_64-seccomp-unix-rsp-v1"
AUDIT_ARCH = 0xC000003E
ALLOW, DENY, KILL = 0x7FFF0000, 0x00050000 | errno.EPERM, 0x80000000


def sha(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def filter_instructions():
    # seccomp_data: nr@0, arch@4, args[0] low/high@16/20.
    # JEQ jumps are relative to the next instruction. Socket domains are C int,
    # but require a zero high half as well, rather than permit truncation tricks.
    return [
        (0x20, 0, 0, 4),                    # LD arch
        (0x15, 1, 0, AUDIT_ARCH),           # exact native ABI
        (0x06, 0, 0, KILL),
        (0x20, 0, 0, 0),                    # LD syscall nr
        (0x35, 0, 1, 0x40000000),           # reject x32 ABI
        (0x06, 0, 0, DENY),
        (0x15, 0, 1, 425),                  # io_uring_setup can create sockets
        (0x06, 0, 0, DENY),
        (0x15, 0, 1, 438),                  # pidfd_getfd can acquire foreign fds
        (0x06, 0, 0, DENY),
        (0x15, 2, 0, 41),                   # socket -> check domain
        (0x15, 1, 0, 53),                   # socketpair -> check domain
        (0x06, 0, 0, ALLOW),
        (0x20, 0, 0, 20),
        (0x15, 1, 0, 0),
        (0x06, 0, 0, DENY),
        (0x20, 0, 0, 16),
        (0x15, 1, 0, socket.AF_UNIX),
        (0x06, 0, 0, DENY),
        (0x06, 0, 0, ALLOW),
    ]


def confined_child_kwargs():
    """Prepare ctypes in the parent; install a fail-closed child-local filter."""
    if sys.platform != "linux" or platform.machine() != "x86_64" or ctypes.sizeof(ctypes.c_void_p) != 8:
        raise ValueError("verified Unix transport requires Linux x86_64")
    class SockFilter(ctypes.Structure):
        _fields_ = [("code", ctypes.c_ushort), ("jt", ctypes.c_ubyte),
                    ("jf", ctypes.c_ubyte), ("k", ctypes.c_uint32)]
    class SockFprog(ctypes.Structure):
        _fields_ = [("len", ctypes.c_ushort), ("filter", ctypes.POINTER(SockFilter))]
    instructions = filter_instructions()
    array = (SockFilter * len(instructions))(*(SockFilter(*row) for row in instructions))
    program = SockFprog(len(instructions), array)
    libc = ctypes.CDLL(None, use_errno=True)
    prctl = libc.prctl
    prctl.restype = ctypes.c_int
    prctl.argtypes = [ctypes.c_int, ctypes.c_ulong, ctypes.c_ulong,
                      ctypes.c_ulong, ctypes.c_ulong]
    address = ctypes.addressof(program)
    def install():
        # Keep array/program/libc alive in this closure through exec.
        _ = (array, program, libc)
        if prctl(38, 1, 0, 0, 0) != 0 or prctl(22, 2, address, 0, 0) != 0:
            raise OSError(ctypes.get_errno(), "child Unix-only confinement failed")
    version = re.match(r"(\d+)\.(\d+)", platform.release())
    if version is None or tuple(map(int, version.groups())) < (5, 4):
        raise ValueError("verified Unix transport requires Linux 5.4 or newer")
    if len(list(Path("/proc/self/task").iterdir())) != 1:
        raise ValueError("verified Unix transport requires a single-threaded launcher")
    return {"preexec_fn": install, "close_fds": True, "stdin": subprocess.DEVNULL}


@contextlib.contextmanager
def private_endpoint():
    with tempfile.TemporaryDirectory(prefix="ee-gcc-unix-") as directory:
        parent = Path(directory)
        info = parent.stat()
        if info.st_uid != os.geteuid() or stat.S_IMODE(info.st_mode) != 0o700:
            raise ValueError("Unix endpoint directory is not privately owned")
        path = parent / "gdb.sock"
        if len(os.fsencode(path)) >= 104 or any(c in str(path) for c in (",", "%")):
            raise ValueError("unsafe Unix endpoint path")
        # Host denial is terminal; never substitute TCP.
        with socket.socket(socket.AF_UNIX, socket.SOCK_STREAM) as probe:
            probe.bind(str(path))
        path.unlink()
        yield str(path)


def process_inodes(pid):
    result = set()
    for descriptor in (Path("/proc") / str(pid) / "fd").iterdir():
        try:
            target = str(descriptor.readlink())
        except FileNotFoundError:
            continue
        match = re.fullmatch(r"socket:\[(\d+)\]", target)
        if match:
            result.add(match.group(1))
    return result


def validate_socket_tables(path, inodes, unix_text, internet_tables):
    """Return a listening owned Unix inode; reject any owned Internet socket."""
    for text in internet_tables:
        for row in text.splitlines()[1:]:
            fields = row.split()
            if len(fields) >= 10 and fields[9] in inodes:
                raise ValueError("spawned process owns an Internet socket")
    unix_rows = [row.split(maxsplit=7) for row in unix_text.splitlines()[1:]]
    unix_inodes = {fields[6] for fields in unix_rows if len(fields) >= 7}
    if not inodes <= unix_inodes:
        raise ValueError("spawned process owns a socket outside the Unix inventory")
    found = []
    for fields in unix_rows:
        if len(fields) != 8 or fields[7] != path:
            continue
        if fields[6] not in inodes:
            raise ValueError("Unix listener belongs to another process")
        if fields[4] != "0001":
            raise ValueError("Unix endpoint is not a stream socket")
        if int(fields[3], 16) & 0x10000 and fields[5] == "01":
            found.append(fields[6])
    if len(found) > 1:
        raise ValueError("ambiguous Unix listener ownership")
    return found[0] if found else None


def confinement_status(text):
    values = dict(line.split(":", 1) for line in text.splitlines() if ":" in line)
    if values.get("NoNewPrivs", "").strip() != "1" or values.get("Seccomp", "").strip() != "2":
        raise ValueError("spawned process confinement is absent")


def owned_listener(pid, path):
    candidate = Path(path)
    if not candidate.exists():
        return None
    info = candidate.lstat()
    parent = candidate.parent.stat()
    if (not stat.S_ISSOCK(info.st_mode) or info.st_uid != os.geteuid()
            or parent.st_uid != os.geteuid() or stat.S_IMODE(parent.st_mode) != 0o700):
        raise ValueError("Unix endpoint is not privately owned")
    proc = Path("/proc") / str(pid)
    confinement_status((proc / "status").read_text())
    inodes = process_inodes(pid)
    tables = []
    for name in ("tcp", "tcp6", "udp", "udp6", "raw", "raw6"):
        table = proc / "net" / name
        if table.exists():
            tables.append(table.read_text())
    # IPv4 table absence is an unsupported procfs view, not proof of no sockets.
    if not (proc / "net/tcp").is_file():
        raise ValueError("Internet socket inventory is unavailable")
    return validate_socket_tables(path, inodes, (proc / "net/unix").read_text(), tables)


def verify_peer(connection, pid):
    peer_pid, uid, gid = struct.unpack("3i", connection.getsockopt(
        socket.SOL_SOCKET, socket.SO_PEERCRED, struct.calcsize("3i")))
    if peer_pid != pid or uid != os.geteuid():
        raise ValueError("Unix debugger peer is not the spawned process")
    return {"pid_verified": True, "uid_verified": True}


def wait_listener(process, path, deadline):
    while True:
        if process.poll() is not None:
            raise RuntimeError("confined QEMU exited before its Unix listener was ready")
        inode = owned_listener(process.pid, path)
        if inode is not None:
            return inode
        if time.monotonic() >= deadline:
            raise TimeoutError("confined Unix debugger listener did not appear")
        time.sleep(0.025)


def tool_hashes(qemu, loader, compiler):
    paths = (Path(qemu).resolve(), Path(loader).resolve(), Path(compiler).resolve(),
             Path(loader).resolve().parent / "libc.so.6", Path(__file__).resolve())
    values = {str(path): sha(path) for path in paths}
    if values[str(Path(compiler).resolve())] != COMPILER_SHA256:
        raise ValueError("capability probe requires the pinned compiler")
    # Only a native x86_64 QEMU can run under this host ABI filter.
    with Path(qemu).open("rb") as handle:
        header = handle.read(20)
    if header[:6] != b"\x7fELF\x02\x01" or header[18:20] != b"\x3e\x00":
        raise ValueError("capability probe requires native x86_64 ELF QEMU")
    return values


def qualify(qemu, loader, compiler, cwd, env, directory, rsp_class, timeout=15):
    """Probe only qSupported, stop reason and registers; never send c/s."""
    if type(timeout) is not int or not 1 <= timeout <= 30:
        raise ValueError("capability timeout must be bounded")
    directory = Path(directory)
    if directory.exists():
        raise ValueError("capability output directory must be new")
    inputs = tool_hashes(qemu, loader, compiler)
    child_kwargs = confined_child_kwargs()
    directory.mkdir(parents=True)
    deadline = time.monotonic() + timeout
    with private_endpoint() as endpoint:
        command = [str(qemu), "-g", endpoint, str(loader), "--library-path",
                   str(Path(loader).parent), str(compiler), "--help"]
        with (directory / "stdout.log").open("wb") as stdout, (directory / "stderr.log").open("wb") as stderr:
            process = subprocess.Popen(command, cwd=cwd, env=env, stdout=stdout,
                                       stderr=stderr, **child_kwargs)
            connection = None
            try:
                wait_listener(process, endpoint, deadline)
                connection = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
                connection.settimeout(max(0.001, deadline - time.monotonic()))
                connection.connect(endpoint)
                verify_peer(connection, process.pid)
                rsp = rsp_class(connection, deadline=deadline)
                supported = rsp.cmd("qSupported")
                if supported.startswith("E"):
                    raise ValueError("capability handshake failed")
                stop = rsp.cmd("?")
                if not re.fullmatch(r"[ST][0-9a-fA-F]{2}.*", stop):
                    raise ValueError("capability probe is not at a guest stop")
                rsp.regs()
                if owned_listener(process.pid, endpoint) is None:
                    raise ValueError("capability listener ownership was lost")
            finally:
                # Disconnect can resume QEMU's stopped guest. Kill and reap while
                # the debugger socket is still open, including failure paths.
                try:
                    if process.poll() is None:
                        process.kill()
                    process.wait(timeout=5)
                finally:
                    if connection is not None:
                        connection.close()
    if tool_hashes(qemu, loader, compiler) != inputs:
        raise ValueError("capability input changed during qualification")
    receipt = {"schema": 1, "method": METHOD, "inputs": inputs,
               "socket_owned": True, "peer_verified": True, "no_owned_internet_socket": True,
               "child_confined": True, "read_only_handshake": True,
               "guest_continued": False, "qualified": True}
    (directory / "receipt.json").write_text(json.dumps(receipt, indent=2, sort_keys=True) + "\n")
    return directory / "receipt.json"


def validate_receipt(receipt_path, command, frozen_inputs):
    data = json.loads(Path(receipt_path).read_text())
    expected_keys = {"schema", "method", "inputs", "socket_owned", "peer_verified",
                     "no_owned_internet_socket", "child_confined", "read_only_handshake",
                     "guest_continued", "qualified"}
    if (set(data) != expected_keys or type(data["schema"]) is not int or data["schema"] != 1
            or data["method"] != METHOD or data["guest_continued"] is not False
            or any(data[key] is not True for key in (
                "socket_owned", "peer_verified", "no_owned_internet_socket",
                "child_confined", "read_only_handshake", "qualified"))):
        raise ValueError("invalid verified Unix capability receipt")
    if len(command) < 5 or command[2:4] != ["--library-path", str(Path(command[1]).parent)]:
        raise ValueError("verified Unix case requires the qualified loader invocation")
    expected = tool_hashes(command[0], command[1], command[4])
    if data["inputs"] != expected:
        raise ValueError("verified Unix capability input identity differs")
    if frozen_inputs.get(Path(receipt_path).resolve()) != sha(receipt_path):
        raise ValueError("verified Unix capability receipt is not frozen")
    for path, value in expected.items():
        if frozen_inputs.get(Path(path)) != value:
            raise ValueError("verified Unix capability input is not frozen")
    confined_child_kwargs()  # unsupported hosts fail during dry-run, without exec
