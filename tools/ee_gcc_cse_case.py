#!/usr/bin/env python3
"""Prepare a current-runtime, read-only EE GCC CSE observation case.

No compilation is performed. The QEMU capability check and ldd dependency
inspection use only the explicitly selected, trusted local executables.
The case, commands, source, and any later compiler output remain private.
"""
from __future__ import annotations

import argparse
import json
import os
from pathlib import Path
import re
import shlex
import subprocess
import sys

import ee_gcc_observe as observer
import ee_gcc_unix_capability as unix_capability
from ee_gcc_cse_role_tracer import validate_watch
from ee_gcc_role_lineage import COMPILER, digest

UNIT = "src/dds2/game/code_001DD390.c"
SOURCE_REL = ".ci/dds2/game/code_001DD390.c"
ASSEMBLY_REL = ".cse0/dds2/src/dds2/game/code_001DD390.o.s"


def invocation_templates(wrapper, unit_flags, dumpbase, object_path):
    """Resolve only the finite shell substitutions in the current cc.sh."""
    compile_match = re.search(r'(?ms)^run "\$cc1" \\\n(.*?)\n# DDS_KEEP_S', wrapper)
    assemble_match = re.search(r'(?m)^run "\$ee/ee/bin/as" ([^\n]+)$', wrapper)
    if compile_match is None or assemble_match is None:
        raise ValueError("unsupported cc.sh invocation structure")
    compiler_tokens = shlex.split(compile_match.group(1).replace("\\\n", " "))
    assembler_tokens = shlex.split(assemble_match.group(1))
    flags = ["-da", "-dumpbase", str(dumpbase), *unit_flags]
    replacements = {"$cc_in": SOURCE_REL, "$cc_out": ASSEMBLY_REL,
                    "$out": str(object_path)}
    def expand(tokens, compiler):
        result = []
        for token in tokens:
            if token == "$flags":
                if not compiler:
                    raise ValueError("unexpected assembler flag expansion")
                result.extend(flags)
                continue
            token = replacements.get(token, token)
            token = token.replace("$(echo $version | tr a-z A-Z)", "DDS2")
            token = token.replace("$version", "dds2")
            if "$" in token or token in (";", "&&", "||", "|", ">", "<"):
                raise ValueError("unsupported cc.sh token expansion")
            result.append(token)
        return result
    cc, assembler = expand(compiler_tokens, True), expand(assembler_tokens, False)
    if (cc.count(SOURCE_REL) != 1 or cc[-2:] != ["-o", ASSEMBLY_REL]
            or "-O2" not in cc or cc.count("-dumpbase") != 1):
        raise ValueError("compiler invocation did not preserve the expected contract")
    if assembler[-1] != ASSEMBLY_REL or str(object_path) not in assembler:
        raise ValueError("assembler invocation did not preserve the expected contract")
    return cc, assembler


def flags_for_unit(text):
    selected = []
    for line in text.splitlines():
        line = line.split("#", 1)[0].strip()
        if not line:
            continue
        parts = line.split(None, 1)
        if parts[0] == "game/code_001DD390":
            if selected:
                raise ValueError("duplicate target per-unit flags")
            selected = shlex.split(parts[1]) if len(parts) == 2 else []
    return selected


def host_libraries(executable, env):
    result = subprocess.run(["/usr/bin/ldd", str(executable)], env=env,
                            capture_output=True, text=True, timeout=30)
    text = result.stdout + result.stderr
    if "not found" in text:
        raise ValueError("unresolved host dependency for " + executable.name)
    if result.returncode:
        if "not a dynamic executable" in text or "statically linked" in text:
            return []
        raise ValueError("could not inspect host dependencies for " + executable.name)
    libraries = []
    for line in text.splitlines():
        # ldd prints the entire transitive ELF dependency closure and interpreter.
        match = re.search(r"(?:=>\s*)?(/[^\s]+)\s+\(", line)
        if match:
            path = Path(match.group(1)).resolve()
            if not path.is_file():
                raise ValueError("missing resolved host dependency")
            libraries.append(path)
    return sorted(set(libraries))


def make_case(repo, source, work, qemu, watch_path):
    repo, source, work, qemu, watch_path = (
        p.resolve() for p in (repo, source, work, qemu, watch_path))
    if work.exists():
        raise ValueError("work directory must be new")
    if not source.is_file() or not qemu.is_file() or not os.access(qemu, os.X_OK):
        raise ValueError("source or executable QEMU is unavailable")
    compiler = repo / "tools/compilers/ee-gcc2.96/lib/gcc-lib/ee/2.96-ee-001003-1/cc1"
    assembler = repo / "tools/compilers/ee-gcc2.96/ee/bin/as"
    loader, libc = repo / "tools/glibc32/ld-linux.so.2", repo / "tools/glibc32/libc.so.6"
    wrapper = repo / "tools/cc.sh"
    cflags = repo / "config/dds2/cflags.txt"
    fixed_source, fixed_assembly = repo / SOURCE_REL, repo / ASSEMBLY_REL
    if fixed_source.exists() or fixed_assembly.exists():
        raise ValueError("fixed-length scratch source/output must be unused")
    if len(SOURCE_REL) != len(UNIT) or len(ASSEMBLY_REL) != len("build/dds2/" + UNIT[:-2] + ".o.s"):
        raise ValueError("canonical filename lengths disagree")
    source_bytes = source.read_bytes()
    watch = json.loads(watch_path.read_text())
    validate_watch(watch, watch.get("function"))
    if watch.get("source_sha256") != digest(source_bytes):
        raise ValueError("watch does not describe this source")
    if digest(compiler.read_bytes()) != COMPILER:
        raise ValueError("compiler is not the pinned observer executable")
    for path in (assembler, loader, libc, wrapper, cflags):
        if not path.is_file():
            raise ValueError("required repository tool/input missing: " + str(path))
    unit_flags = flags_for_unit(cflags.read_text())
    cc_args, as_args = invocation_templates(wrapper.read_text(), unit_flags,
                                            work / "rtl", work / "candidate.o")
    # No inherited environment values, credentials, preload, or QEMU overrides.
    env = {"PATH": "/usr/bin:/bin", "LANG": "C", "LC_ALL": "C", "TZ": "UTC",
           "HOME": str(work / "home"), "TMPDIR": str(work / "tmp"),
           "DDS_VERSION": "dds2", "DDS_AS_UNIT": UNIT}
    work.mkdir(parents=True)
    (work / "home").mkdir()
    (work / "tmp").mkdir()
    capability_receipt = unix_capability.qualify(
        qemu, loader, compiler, repo, env, work / "unix-capability", observer.RSP)
    inputs = {}
    def add(path):
        path = path.resolve()
        if not path.is_file():
            raise ValueError("input closure contains a missing/non-file path")
        inputs[str(path)] = digest(path.read_bytes())
    for path in (source, compiler, assembler, qemu, loader, libc, wrapper, cflags,
                 watch_path, capability_receipt, Path(sys.executable), Path(__file__)):
        add(path)
    # Supersets avoid guessing which include branches/macros the old compiler
    # opens. Keep generated assembly private; its hashes are local receipts only.
    for name in ("include", "src", "build/eeasm"):
        directory = repo / name
        if not directory.is_dir():
            raise ValueError("required input-closure directory missing: " + name)
        for path in sorted(directory.rglob("*")):
            if path.is_file():
                add(path)
    # These exact modules execute in the observer process.
    for name in ("ee_gcc_cse_role_tracer.py", "ee_gcc_role_lineage.py",
                 "ee_gcc_delay_slots.py", "ee_gcc_probe.py", "ee_gcc_observe.py",
                 "_ee_gcc_observer.py", "ee_gcc_qemu_loopback.py",
                 "ee_gcc_unix_capability.py"):
        add(Path(__file__).with_name(name))
    native_modules = set()
    for module in list(sys.modules.values()):
        for attribute in ("__file__", "__cached__"):
            value = getattr(module, attribute, None)
            if isinstance(value, str) and Path(value).is_file():
                add(Path(value))
                if ".so" in Path(value).name:
                    native_modules.add(Path(value).resolve())
    for binary in (qemu, Path(sys.executable).resolve(), *sorted(native_modules)):
        for library in host_libraries(binary, env):
            add(library)
    # The copied source has precisely the same bytes as the qualified snapshot.
    inputs[str(fixed_source.resolve())] = digest(source_bytes)
    command = [str(qemu), str(loader), "--library-path", str(loader.parent),
               str(compiler), *cc_args]
    assembler_command = [str(qemu), str(loader), "--library-path", str(loader.parent),
                         str(assembler), *as_args]
    logs = {name: str(work / (name + ".log"))
            for name in ("cc1_stdout", "cc1_stderr", "as_stdout", "as_stderr")}
    case = {"schema": 1, "cwd": str(repo), "environment": env,
            "compiler": str(compiler), "assembler": str(assembler), "qemu": str(qemu),
            "transport": {"kind": "unix-verified", "receipt": str(capability_receipt)},
            "timeout_seconds": 1800,
            "function": watch["function"], "command": command,
            "assembler_command": assembler_command, "inputs": inputs, "logs": logs,
            "artifacts": [
                {"path": str(fixed_assembly), "archive": "candidate.s"},
                {"path": str(work / "candidate.o"), "archive": "candidate.o"},
                {"root": str(work), "pattern": "rtl.[0-9][0-9].*", "archive_dir": "rtl"},
            ] + [{"path": path, "archive": name + ".log"} for name, path in logs.items()]}
    # Validate before creating compiler inputs/outputs; the private capability
    # receipt already exists and is frozen with the selected executable closure.
    observer.Case(case, work)
    fixed_source.parent.mkdir(parents=True, exist_ok=True)
    fixed_assembly.parent.mkdir(parents=True, exist_ok=True)
    fixed_source.write_bytes(source_bytes)
    (work / "case.json").write_text(json.dumps(case, indent=2, sort_keys=True) + "\n")
    control = {"schema": 1, "cwd": str(repo), "environment": env,
               "compiler_command": command[1:], "assembler_command": assembler_command[1:],
               "source_sha256": digest(source_bytes),
               "note": "Run these native control commands serially, archive their outputs, and independently compare payload/relocations before using the QEMU observer."}
    (work / "native-control.json").write_text(json.dumps(control, indent=2, sort_keys=True) + "\n")
    (work / "preparation.json").write_text(json.dumps({
        "schema": 1, "compiler_sha256": COMPILER, "source_sha256": digest(source_bytes),
        "qemu_sha256": inputs[str(qemu)], "unix_capability_qualified": True,
        "transport": "unix-verified", "input_count": len(inputs), "compiled": False,
        "canonical_source_length": len(SOURCE_REL),
        "canonical_assembly_length": len(ASSEMBLY_REL),
        "closure": "all include/src/build-eeasm files, declared tools, loaded Python modules and resolved host ELF dependencies",
        "limitations": ["A directory supersets receipt is not a syscall-level proof that no undeclared file was opened.",
                        "This builder does not establish ordinary/native/QEMU compiler-output parity."]
    }, indent=2, sort_keys=True) + "\n")
    observer.load_case(work / "case.json").verify()
    return len(inputs)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ("repo", "source", "work", "qemu", "watch"):
        parser.add_argument("--" + name, required=True, type=Path)
    args = parser.parse_args()
    try:
        count = make_case(args.repo, args.source, args.work, args.qemu, args.watch)
    except (OSError, ValueError, KeyError, RuntimeError, subprocess.SubprocessError) as error:
        parser.exit(2, str(error) + "\n")
    print(json.dumps({"prepared": True, "compiled": False, "transport": "unix-verified", "inputs": count}))


if __name__ == "__main__":
    main()
