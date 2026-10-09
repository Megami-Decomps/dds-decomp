#!/usr/bin/env python3
"""Synthetic transport tests and a child-only socket-creation smoke test.

No compiler inputs, dumps, credentials, or listening Internet sockets are used.
"""
import contextlib
import errno
import json
import os
from pathlib import Path
import platform
import socket
import struct
import subprocess
import sys
import tempfile
import unittest
from unittest import mock

import ee_gcc_unix_capability as cap
import ee_gcc_observe as observer


def evaluate_filter(nr, family=0, arch=cap.AUDIT_ARCH):
    data = {0: nr, 4: arch, 16: family & 0xffffffff, 20: family >> 32}
    pc, value = 0, 0
    program = cap.filter_instructions()
    for _ in range(100):
        code, jt, jf, k = program[pc]
        if code == 0x20:
            value = data[k]
        elif code == 0x15:
            pc += jt if value == k else jf
        elif code == 0x35:
            pc += jt if value >= k else jf
        elif code == 0x06:
            return k
        else:
            raise AssertionError("unexpected BPF instruction")
        pc += 1
    raise AssertionError("filter did not terminate")


UNIX_HEADER = "Num RefCount Protocol Flags Type St Inode Path\n"
UNIX_ROW = "000: 00000002 00000000 00010000 0001 01 42 /private/gdb.sock\n"
TCP_HEADER = "sl local remote st queue timer retr uid timeout inode\n"
TCP_ROW = "0: 0100007F:1234 00000000:0000 0A 0:0 0:0 0 1000 0 99\n"


class FilterTests(unittest.TestCase):
    def test_socket_domain_and_abi_policy(self):
        for nr in (41, 53):
            self.assertEqual(evaluate_filter(nr, socket.AF_UNIX), cap.ALLOW)
            for family in (0, socket.AF_INET, socket.AF_INET6, 16, 17, (1 << 32) | 1):
                self.assertEqual(evaluate_filter(nr, family), cap.DENY)
        for nr in (0, 1, 3, 9, 59, 60, 202, 231):
            self.assertEqual(evaluate_filter(nr), cap.ALLOW)
        for nr in (425, 438, 0x40000000, 0x40000029, 0xffffffff):
            self.assertEqual(evaluate_filter(nr, socket.AF_UNIX), cap.DENY)
        self.assertEqual(evaluate_filter(41, 1, 0x40000003), cap.KILL)

    def test_unsupported_architecture_is_terminal(self):
        with mock.patch.object(cap.platform, "machine", return_value="aarch64"):
            with self.assertRaises(ValueError):
                cap.confined_child_kwargs()

    def test_failed_prctl_cannot_exec(self):
        prctl = mock.Mock(return_value=-1)
        with mock.patch.object(cap.platform, "machine", return_value="x86_64"), \
             mock.patch.object(cap.platform, "release", return_value="6.8.0"), \
             mock.patch.object(cap.sys, "platform", "linux"), \
             mock.patch.object(cap.ctypes, "CDLL", return_value=mock.Mock(prctl=prctl)), \
             mock.patch.object(cap.Path, "iterdir", return_value=iter([Path("1")])):
            kwargs = cap.confined_child_kwargs()
            self.assertTrue(kwargs["close_fds"])
            self.assertEqual(kwargs["stdin"], subprocess.DEVNULL)
            with self.assertRaises(OSError):
                kwargs["preexec_fn"]()
        self.assertEqual(prctl.call_count, 1)

    def test_multithreaded_launcher_and_old_kernel_rejected(self):
        with mock.patch.object(cap.platform, "machine", return_value="x86_64"), \
             mock.patch.object(cap.sys, "platform", "linux"), \
             mock.patch.object(cap.platform, "release", return_value="6.8"), \
             mock.patch.object(cap.Path, "iterdir", return_value=iter([Path("1"), Path("2")])):
            with self.assertRaises(ValueError):
                cap.confined_child_kwargs()
        with mock.patch.object(cap.platform, "release", return_value="5.3.0"):
            with self.assertRaises(ValueError):
                cap.confined_child_kwargs()


class OwnershipTests(unittest.TestCase):
    def test_owned_unix_and_unrelated_tcp(self):
        self.assertEqual(cap.validate_socket_tables("/private/gdb.sock", {"42"},
                         UNIX_HEADER + UNIX_ROW, [TCP_HEADER + TCP_ROW]), "42")

    def test_owned_internet_and_unbound_socket_rejected(self):
        for tables in ([TCP_HEADER + TCP_ROW], [TCP_HEADER]):
            with self.assertRaises(ValueError):
                cap.validate_socket_tables("/private/gdb.sock", {"42", "99"},
                                           UNIX_HEADER + UNIX_ROW, tables)

    def test_wrong_owner_and_nonlistener(self):
        with self.assertRaises(ValueError):
            cap.validate_socket_tables("/private/gdb.sock", set(), UNIX_HEADER + UNIX_ROW, [])
        self.assertIsNone(cap.validate_socket_tables("/private/gdb.sock", {"42"},
                          UNIX_HEADER + UNIX_ROW.replace("00010000", "00000000"), []))
        with self.assertRaises(ValueError):
            cap.validate_socket_tables("/private/gdb.sock", {"42"}, UNIX_HEADER +
                                       UNIX_ROW.replace("0001 01", "0002 01"), [])

    def test_peer_pid_and_uid_checked_before_rsp(self):
        connection = mock.Mock()
        connection.getsockopt.return_value = struct.pack("3i", 101, os.geteuid(), os.getegid())
        self.assertTrue(cap.verify_peer(connection, 101)["pid_verified"])
        for pid, uid in ((102, os.geteuid()), (101, os.geteuid() + 1)):
            connection.getsockopt.return_value = struct.pack("3i", pid, uid, os.getegid())
            with self.assertRaises(ValueError):
                cap.verify_peer(connection, 101)

    def test_confinement_evidence_required(self):
        cap.confinement_status("NoNewPrivs:\t1\nSeccomp:\t2\n")
        for text in ("", "NoNewPrivs: 0\nSeccomp: 2", "NoNewPrivs: 1\nSeccomp: 0"):
            with self.assertRaises(ValueError):
                cap.confinement_status(text)

    def test_missing_proc_evidence_and_non_socket_rejected(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "gdb.sock"
            path.write_text("regular file")
            with self.assertRaises(ValueError):
                cap.owned_listener(os.getpid(), str(path))
            path.unlink()
            with socket.socket(socket.AF_UNIX, socket.SOCK_STREAM) as listener:
                listener.bind(str(path))
                with self.assertRaises(FileNotFoundError):
                    cap.owned_listener(999999999, str(path))

    def test_legacy_help_gate_unchanged(self):
        self.assertFalse(observer.unix_supported("-g port QEMU_GDB wait for gdb\n"))
        self.assertTrue(observer.unix_supported("-g endpoint QEMU_GDB wait for gdb\n"))

    def test_denied_host_unix_socket_is_terminal(self):
        with mock.patch.object(cap.socket, "socket", side_effect=PermissionError("denied")):
            with self.assertRaises(PermissionError):
                with cap.private_endpoint():
                    self.fail("must not yield")


class QualificationTests(unittest.TestCase):
    def exercise_probe(self, failed=False):
        events = []
        class Process:
            pid = 101
            def poll(self):
                return None
            def kill(self):
                events.append("kill")
            def wait(self, timeout):
                events.append("wait")
                return -9
        class Connection:
            def settimeout(self, timeout):
                pass
            def connect(self, address):
                events.append("connect")
            def close(self):
                events.append("close")
        class RSP:
            def __init__(self, connection, deadline):
                pass
            def cmd(self, command):
                events.append(command)
                if failed:
                    raise TimeoutError("synthetic")
                return "T05" if command == "?" else "PacketSize=1000"
            def regs(self):
                events.append("g")
                return {"eip": 1}
        with tempfile.TemporaryDirectory() as directory, \
             mock.patch.object(cap, "tool_hashes", return_value={"/qemu": "a" * 64}), \
             mock.patch.object(cap, "confined_child_kwargs", return_value={}), \
             mock.patch.object(cap, "private_endpoint", return_value=contextlib.nullcontext("/private/gdb.sock")), \
             mock.patch.object(cap.subprocess, "Popen", return_value=Process()), \
             mock.patch.object(cap.socket, "socket", return_value=Connection()), \
             mock.patch.object(cap, "wait_listener", return_value="42"), \
             mock.patch.object(cap, "owned_listener", return_value="42"), \
             mock.patch.object(cap, "verify_peer", return_value={}):
            out = Path(directory) / "qualification"
            if failed:
                with self.assertRaises(TimeoutError):
                    cap.qualify(Path("/qemu"), Path("/ld"), Path("/cc1"), "/", {}, out, RSP)
                self.assertFalse((out / "receipt.json").exists())
            else:
                receipt = cap.qualify(Path("/qemu"), Path("/ld"), Path("/cc1"), "/", {}, out, RSP)
                data = json.loads(receipt.read_text())
                self.assertTrue(data["qualified"])
                self.assertFalse(data["guest_continued"])
        self.assertEqual(events[-3:], ["kill", "wait", "close"])
        self.assertNotIn("c", events)
        self.assertNotIn("s", events)
        return events

    def test_success_kills_before_disconnect(self):
        self.assertEqual(self.exercise_probe()[:-3], ["connect", "qSupported", "?", "g"])

    def test_timeout_kills_before_disconnect(self):
        self.exercise_probe(failed=True)

    def test_receipt_must_match_frozen_tools_and_flags(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "receipt.json"
            expected = {"/qemu": "a" * 64}
            data = {"schema": 1, "method": cap.METHOD, "inputs": expected,
                    "socket_owned": True, "peer_verified": True, "no_owned_internet_socket": True,
                    "child_confined": True, "read_only_handshake": True,
                    "guest_continued": False, "qualified": True}
            path.write_text(json.dumps(data))
            inputs = {path.resolve(): cap.sha(path), Path("/qemu"): "a" * 64}
            command = ["/qemu", "/runtime/ld", "--library-path", "/runtime", "/cc1"]
            with mock.patch.object(cap, "tool_hashes", return_value=expected), \
                 mock.patch.object(cap, "confined_child_kwargs", return_value={}):
                cap.validate_receipt(path, command, inputs)
                with self.assertRaises(ValueError):
                    cap.validate_receipt(path, command, {})
                inputs[Path("/qemu")] = "b" * 64
                with self.assertRaises(ValueError):
                    cap.validate_receipt(path, command, inputs)
                inputs[Path("/qemu")] = "a" * 64
                data["guest_continued"] = True
                path.write_text(json.dumps(data))
                inputs[path.resolve()] = cap.sha(path)
                with self.assertRaises(ValueError):
                    cap.validate_receipt(path, command, inputs)


@unittest.skipUnless(sys.platform == "linux" and platform.machine() == "x86_64",
                     "host smoke test requires Linux x86_64")
class HostConfinementTests(unittest.TestCase):
    def test_real_child_filter_and_inheritance(self):
        # No bind/listen is attempted, even on unexpected filter failure.
        code = r"""
import errno, os, socket, sys, threading
def check():
    with socket.socket(socket.AF_UNIX, socket.SOCK_STREAM):
        pass
    left, right = socket.socketpair()
    left.close(); right.close()
    for family in (socket.AF_INET, socket.AF_INET6, socket.AF_NETLINK, socket.AF_PACKET):
        try:
            value = socket.socket(family, socket.SOCK_DGRAM)
        except OSError as error:
            assert error.errno == errno.EPERM, error.errno
        else:
            value.close()
            raise AssertionError("non-Unix creation was allowed")
check()
if len(sys.argv) == 2:
    errors = []
    def worker():
        try: check()
        except BaseException as error: errors.append(error)
    thread = threading.Thread(target=worker)
    thread.start(); thread.join()
    assert not errors
    pid = os.fork()
    if pid == 0:
        check()
        os._exit(0)
    assert os.waitpid(pid, 0)[1] == 0
    os.execv(sys.executable, [sys.executable, "-c", sys.argv[1], "after-exec", "done"])
"""
        # Initial argv[1] is the script for an exec inheritance check. The
        # post-exec invocation has three arguments and does not recurse.
        result = subprocess.run([sys.executable, "-c", code, code],
                                env={"PATH": "/usr/bin:/bin", "LANG": "C", "LC_ALL": "C"},
                                capture_output=True, timeout=15, **cap.confined_child_kwargs())
        self.assertEqual(result.returncode, 0, result.stderr.decode(errors="replace"))


if __name__ == "__main__":
    unittest.main()
