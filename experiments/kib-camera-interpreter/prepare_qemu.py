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
STAGE = "start"
LAST_CODE = None

def run(args, **kwargs):
    global STAGE, LAST_CODE
    STAGE = ("verify_signature" if "--verify" in args else
             "import_release_key" if "--import" in args else Path(args[0]).name)
    result = subprocess.run(args, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                            text=True, check=False, timeout=1200, **kwargs)
    LAST_CODE = result.returncode
    if result.returncode:
        raise RuntimeError("command_failed")
    return result.stdout

def main():
    global STAGE
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
            STAGE = "download_" + output.name
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
        STAGE = "extract_verified_archive"
        with tarfile.open(archive) as source:
            try:
                def user_mode_source(member, destination):
                    # EDK2's macOS X11 header shortcut is unrelated to the
                    # i386 Linux user-mode build. Omit this exact signed member;
                    # every other member still passes Python's data filter.
                    excluded = "qemu-" + VERSION + "/roms/edk2/EmulatorPkg/Unix/Host/X11IncludeHack"
                    if member.name == excluded:
                        if not member.issym() or member.linkname != "/opt/X11/include":
                            raise RuntimeError("excluded_member_changed")
                        return None
                    return tarfile.data_filter(member, destination)
                source.extractall(work, filter=user_mode_source)
            except tarfile.FilterError as exc:
                # Only official, signature-verified archive member metadata.
                member = getattr(exc, "tarinfo", None)
                print(json.dumps(dict(status="official_archive_filter_rejected",
                                      member=member.name if member else None,
                                      link=member.linkname if member else None,
                                      category=type(exc).__name__)))
                raise
        source = work / ("qemu-" + VERSION)
        build = source / "build"
        build.mkdir()
        run([str(source / "configure"), "--target-list=i386-linux-user",
             "--disable-system", "--disable-tools", "--disable-docs",
             "--disable-guest-agent"], cwd=build)
        run(["ninja", "-j2", "qemu-i386"], cwd=build)
        binary = build / "qemu-i386"
        help_text = run([str(binary), "--help"])
        # Help text is informational: the case builder separately qualifies
        # an owned Unix-only endpoint before any live compiler observation.
        endpoint_advertised = bool(re.search(r"-g\s+endpoint\b", help_text))
        PREFIX.mkdir(exist_ok=False)
        shutil.copy2(binary, PREFIX / "qemu-i386")
        for name in ("COPYING", "COPYING.LIB"):
            if (source / name).exists():
                shutil.copy2(source / name, PREFIX / name)
        (PREFIX / "SOURCE.txt").write_text(
            "Official QEMU " + VERSION + "\nhttps://download.qemu.org/" + archive.name + "\n")
        print(json.dumps(dict(status="qemu_built", version=VERSION,
                              official_signature_verified=True,
                              unix_endpoint_advertised=endpoint_advertised,
                              unix_capability_qualified=False)))
if __name__ == "__main__":
    try:
        main()
    except Exception as exc:
        code = getattr(exc, "code", LAST_CODE)
        print(json.dumps(dict(status="qemu_preparation_failed", stage=STAGE,
                              category=type(exc).__name__, code=code if isinstance(code, int) else None)))
        raise SystemExit(2)
