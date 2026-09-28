#!/usr/bin/env python3
"""Fetch the external build tools into tools/ (ignored by git).

    python tools/download_tools.py                  # default toolchain
    python tools/download_tools.py --compiler ee-gcc2.95.3-136   # extra compiler

Compilers come from the decomp.me compiler archive; binutils are the decompals
MIPS/PS2 build (R5900-aware objdump/ld); objdiff-cli is used for diffing and
the decomp.dev progress report; a pinned 32-bit glibc (tools/glibc32) runs the
i386 compiler binaries identically on every machine.
"""
from __future__ import annotations

import argparse
import io
import platform
import stat
import tarfile
import urllib.request
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
TOOLS = ROOT / "tools"
COMPILER_URL = "https://github.com/decompme/compilers/releases/download/compilers/{name}{ext}"
COMPILER_EXTS = (".tar.xz", ".tar.gz")
DEFAULT_COMPILERS = ["ee-gcc2.96"]
BINUTILS_URL = ("https://github.com/decompals/binutils-mips-ps2-decompals/releases/download/"
                "v0.10/binutils-mips-ps2-decompals-linux-x86-64.tar.gz")
OBJDIFF_URL = "https://github.com/encounter/objdiff/releases/download/v3.8.1/objdiff-cli-linux-x86_64"
# The 32-bit glibc cc1 and ee-as run under. ee-gcc 2.96's code selection
# depends on heap addresses (CSE hashes symbol-name pointers), so a different
# libc can compile some functions differently: every build uses this one.
GLIBC_URL = "https://kojipkgs.fedoraproject.org/packages/glibc/2.43/8.fc44/i686/glibc-2.43-8.fc44.i686.rpm"
GLIBC_SHA256 = "e77eb7b8eef4851653dc50836ad949e769220a74ebbea0bdc6c647aba90faaf7"
GLIBC_FILES = ("usr/lib/ld-linux.so.2", "usr/lib/libc.so.6")


def fetch(url: str) -> bytes:
    print(f"fetch {url}")
    with urllib.request.urlopen(url) as resp:
        return resp.read()


def make_executable(path: Path) -> None:
    path.chmod(path.stat().st_mode | stat.S_IXUSR | stat.S_IXGRP | stat.S_IXOTH)


def get_compiler(name: str, force: bool) -> None:
    dest = TOOLS / "compilers" / name
    if dest.is_dir() and not force:
        return
    last_error: Exception | None = None
    for ext in COMPILER_EXTS:
        try:
            data = fetch(COMPILER_URL.format(name=name, ext=ext))
        except Exception as exc:  # 404 for the other extension
            last_error = exc
            continue
        dest.mkdir(parents=True, exist_ok=True)
        with tarfile.open(fileobj=io.BytesIO(data)) as tar:
            tar.extractall(dest, filter="tar")
        for path in dest.rglob("*"):
            if path.is_file() and path.read_bytes()[:4] == b"\x7fELF":
                make_executable(path)
        return
    raise SystemExit(f"could not download compiler {name}: {last_error}")


def get_binutils(force: bool) -> None:
    dest = TOOLS / "bin"
    if (dest / "mips-ps2-decompals-ld").exists() and not force:
        return
    data = fetch(BINUTILS_URL)
    dest.mkdir(parents=True, exist_ok=True)
    with tarfile.open(fileobj=io.BytesIO(data)) as tar:
        tar.extractall(dest, filter="tar")
    for path in dest.iterdir():
        if path.is_file():
            make_executable(path)


def get_objdiff(force: bool) -> None:
    dest = TOOLS / "bin" / "objdiff-cli"
    if dest.exists() and not force:
        return
    dest.parent.mkdir(parents=True, exist_ok=True)
    dest.write_bytes(fetch(OBJDIFF_URL))
    make_executable(dest)


def rpm_payload(data: bytes) -> bytes:
    """The decompressed cpio archive of an RPM (lead, two headers, payload)."""
    import struct
    off = 96  # lead
    for index in range(2):  # signature header, then main header
        if data[off:off + 3] != b"\x8e\xad\xe8":
            raise SystemExit("not an RPM header")
        entries, size = struct.unpack(">II", data[off + 8:off + 16])
        off += 16 + 16 * entries + size
        if index == 0:
            off = (off + 7) & ~7
    payload = data[off:]
    if payload[:4] == b"\x28\xb5\x2f\xfd":
        try:
            from compression import zstd  # Python 3.14+
            return zstd.decompress(payload)
        except ImportError:
            import zstandard
            return zstandard.ZstdDecompressor().decompressobj().decompress(payload)
    import lzma
    return lzma.decompress(payload)


def cpio_files(archive: bytes):
    """(name, data) for every member of a newc cpio archive."""
    off = 0
    while off + 110 <= len(archive):
        header = archive[off:off + 110]
        if header[:6] not in (b"070701", b"070702"):
            raise SystemExit("bad cpio header")
        fields = [int(header[6 + 8 * i:14 + 8 * i], 16) for i in range(13)]
        size, namesize = fields[6], fields[11]
        name = archive[off + 110:off + 110 + namesize - 1].decode()
        off = (off + 110 + namesize + 3) & ~3
        yield name.lstrip("./"), archive[off:off + size]
        off = (off + size + 3) & ~3
        if name == "TRAILER!!!":
            return


def get_glibc(force: bool) -> None:
    import hashlib
    dest = TOOLS / "glibc32"
    if (dest / "libc.so.6").exists() and not force:
        return
    data = fetch(GLIBC_URL)
    if hashlib.sha256(data).hexdigest() != GLIBC_SHA256:
        raise SystemExit("glibc rpm checksum mismatch")
    dest.mkdir(parents=True, exist_ok=True)
    found = 0
    for name, body in cpio_files(rpm_payload(data)):
        if name in GLIBC_FILES:
            out = dest / Path(name).name
            out.write_bytes(body)
            make_executable(out)
            found += 1
    if found != len(GLIBC_FILES):
        raise SystemExit("glibc rpm is missing ld-linux.so.2 or libc.so.6")


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--compiler", action="append", default=[], help="extra decomp.me compiler id")
    ap.add_argument("--force", action="store_true")
    args = ap.parse_args()
    if platform.system() != "Linux" or platform.machine() not in ("x86_64", "AMD64"):
        raise SystemExit("prebuilt tools are linux-x86_64 only; run under Linux/WSL")
    for name in DEFAULT_COMPILERS + args.compiler:
        get_compiler(name, args.force)
    get_binutils(args.force)
    get_objdiff(args.force)
    get_glibc(args.force)


if __name__ == "__main__":
    main()
