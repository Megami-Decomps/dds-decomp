#!/usr/bin/env python3
"""Fetch the external build tools into tools/ (ignored by git).

    python tools/download_tools.py                  # default toolchain
    python tools/download_tools.py --compiler ee-gcc2.95.3-136   # extra compiler

Compilers come from the decomp.me compiler archive; binutils are the decompals
MIPS/PS2 build (R5900-aware objdump/ld); objdiff-cli is used for diffing and
the decomp.dev progress report.
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


if __name__ == "__main__":
    main()
