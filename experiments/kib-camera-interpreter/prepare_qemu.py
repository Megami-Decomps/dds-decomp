#!/usr/bin/env python3
"""Build an official QEMU user-mode release for private Unix-socket observation."""
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import tarfile
import tempfile
import urllib.request

VERSION = "11.1.2"
SIGNER = "CEACC9E15534EBABB82D3FA03353C9CEF108B584"
PREFIX = Path("/tmp/dds-camera-qemu")

def run(args, **kwargs):
    result = subprocess.run(args, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                            text=True, check=False, timeout=1200, **kwargs)
    if result.returncode:
        raise RuntimeError("command_failed")
    return result.stdout

def main():
    # This official release and signer are listed on https://www.qemu.org/download/.
    with tempfile.TemporaryDirectory(prefix="dds-qemu-build-") as temp:
        work = Path(temp)
        archive = work / ("qemu-" + VERSION + ".tar.xz")
        signature = work / (archive.name + ".sig")
        key = work / "release-key.asc"
        for url, output in (
            ("https://download.qemu.org/" + archive.name, archive),
            ("https://download.qemu.org/" + signature.name, signature),
            ("https://keys.openpgp.org/vks/v1/by-fingerprint/" + SIGNER, key),
        ):
            with urllib.request.urlopen(url, timeout=120) as response:
                output.write_bytes(response.read())
        home = work / "gnupg"
        home.mkdir(mode=0o700)
        run(["gpg", "--batch", "--homedir", str(home), "--import", str(key)])
        status = run(["gpg", "--batch", "--homedir", str(home), "--status-fd", "1",
                      "--verify", str(signature), str(archive)])
        valid = [line.split() for line in status.splitlines()
                 if line.startswith("[GNUPG:] VALIDSIG ")]
        if not any(row[2] == SIGNER or row[-1] == SIGNER for row in valid):
            raise RuntimeError("release_signer_mismatch")
        with tarfile.open(archive) as source:
            source.extractall(work, filter="data")
        source = work / ("qemu-" + VERSION)
        build = source / "build"
        build.mkdir()
        run([str(source / "configure"), "--target-list=i386-linux-user",
             "--disable-system", "--disable-tools", "--disable-docs",
             "--disable-guest-agent"], cwd=build)
        run(["ninja", "-j2", "qemu-i386"], cwd=build)
        binary = build / "qemu-i386"
        help_text = run([str(binary), "--help"])
        # Keep the upstream observer's conservative transport gate.
        if not re.search(r"-g\s+endpoint\b", help_text):
            raise RuntimeError("unix_debugger_capability_missing")
        PREFIX.mkdir(exist_ok=False)
        shutil.copy2(binary, PREFIX / "qemu-i386")
        for name in ("COPYING", "COPYING.LIB"):
            if (source / name).exists():
                shutil.copy2(source / name, PREFIX / name)
        (PREFIX / "SOURCE.txt").write_text(
            "Official QEMU " + VERSION + "\nhttps://download.qemu.org/" + archive.name + "\n")
        print(json.dumps(dict(status="qemu_prepared", version=VERSION,
                              official_signature_verified=True,
                              unix_endpoint_advertised=True)))
if __name__ == "__main__":
    try:
        main()
    except Exception:
        print(json.dumps(dict(status="qemu_preparation_failed")))
        raise SystemExit(2)
