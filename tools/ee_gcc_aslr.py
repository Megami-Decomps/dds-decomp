#!/usr/bin/env python3
"""Make the project's exact EE GCC 2.96 compiler deterministic under ASLR.

This compiler's CSE pass hashes SYMBOL_REF and LABEL_REF pointer values.  Its
garbage-collected pages normally come from anonymous mmap calls, so ASLR can
change those pointer values and, for sensitive functions, the generated code.

The patch gives only GCC's three GGC mmap calls a fixed, top-down arena.  It
uses MAP_FIXED_NOREPLACE, so an address collision fails instead of replacing
an existing mapping.  The injected wrapper is RX; its one-word cursor lives in
existing writable zero-fill padding.  Every input byte that is changed is
checked, and the complete compiler SHA-256 must match the known archive build.
"""

from __future__ import annotations

import argparse
import hashlib
import os
import struct
import tempfile
from pathlib import Path


ORIGINAL_SHA256 = "59ec92b3f9f3513e0633331af304733e3094de30884e662a8cb584a51c1c42b5"
PATCHED_SHA256 = "d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1"
CC1_RELATIVE_PATH = Path("lib/gcc-lib/ee/2.96-ee-001003-1/cc1")
MARKER_SUFFIX = ".aslr-fixed"

IMAGE_BASE = 0x08048000
WRAPPER_VA = 0x09000790
WRAPPER_FILE_OFFSET = 0x510790
PROGRAM_HEADER_FILE_OFFSET = 0xD4

# i386 wrapper, equivalent to:
#
#   if (!cursor) cursor = 0x30000000;
#   else cursor -= align_up(length, 0x1000);
#   addr = cursor;
#   flags |= MAP_FIXED_NOREPLACE;
#   goto mmap@plt;
#
# init_ggc first maps and unmaps a probe page, so the first retained page is
# also 0x30000000.  The rel32 tail jump targets mmap@plt at 0x08049674.
PAYLOAD = bytes.fromhex(
    "833d70fe230800750cb800000030a370fe2308eb28"
    "8b4c240881c1ff0f000081e100f0ffffa170fe230829c80500100000"
    "8d8800f0ffff890d70fe230889442404814c241000001000"
    "e9968e04ff"
)
ORIGINAL_PAYLOAD_BYTES = bytes.fromhex(
    "08000000000000000100000030312e3031000000080000000000000001000000"
    "30312e303100000008000000000000000100000030312e303100000008000000"
    "000000000100000030312e303100"
)
ORIGINAL_PROGRAM_HEADER = bytes.fromhex(
    "0400000008010000088104080881040820000000200000000400000004000000"
)

# CALL rel32 instructions in ggc_alloc and init_ggc (two call sites).
CALL_SITES = {
    0x081D59AB: bytes.fromhex("e8c43ce7ff"),
    0x081D5CD0: bytes.fromhex("e89f39e7ff"),
    0x081D5D14: bytes.fromhex("e85b39e7ff"),
}


class PatchError(RuntimeError):
    """The compiler is not the exact binary this patch understands."""


def sha256(data: bytes | bytearray) -> str:
    return hashlib.sha256(data).hexdigest()


def marker_path(compiler: Path) -> Path:
    return Path(f"{compiler}{MARKER_SUFFIX}")


def marker_is_valid(compiler: Path) -> bool:
    marker = marker_path(compiler)
    try:
        return marker.read_text().strip() == PATCHED_SHA256
    except OSError:
        return False


def _replace_checked(
    image: bytearray, offset: int, expected: bytes, replacement: bytes
) -> None:
    actual = bytes(image[offset : offset + len(expected)])
    if actual != expected:
        raise PatchError(
            f"refusing patch at file offset 0x{offset:x}: "
            f"expected {expected.hex()}, found {actual.hex()}"
        )
    if len(expected) != len(replacement):
        raise AssertionError("in-place replacement changed size")
    image[offset : offset + len(replacement)] = replacement


def patch_image(source: bytes, *, verify_hash: bool = True) -> bytes:
    """Return a patched cc1 image, failing closed on any unexpected input."""
    digest = sha256(source)
    if verify_hash and digest != ORIGINAL_SHA256:
        raise PatchError(
            f"refusing unknown compiler: expected SHA-256 {ORIGINAL_SHA256}, "
            f"found {digest}"
        )

    image = bytearray(source)
    _replace_checked(
        image,
        PROGRAM_HEADER_FILE_OFFSET,
        ORIGINAL_PROGRAM_HEADER,
        struct.pack(
            "<8I",
            1,                    # PT_LOAD
            WRAPPER_FILE_OFFSET,
            WRAPPER_VA,
            WRAPPER_VA,
            len(PAYLOAD),
            len(PAYLOAD),
            5,                    # PF_R | PF_X
            0x1000,
        ),
    )
    _replace_checked(image, WRAPPER_FILE_OFFSET, ORIGINAL_PAYLOAD_BYTES, PAYLOAD)
    for site, expected in CALL_SITES.items():
        displacement = WRAPPER_VA - (site + 5)
        _replace_checked(
            image,
            site - IMAGE_BASE,
            expected,
            b"\xe8" + struct.pack("<i", displacement),
        )

    result = bytes(image)
    if verify_hash and sha256(result) != PATCHED_SHA256:
        raise PatchError("patched compiler checksum does not match the validated image")
    return result


def _atomic_write(path: Path, data: bytes, mode: int) -> None:
    temporary: Path | None = None
    try:
        with tempfile.NamedTemporaryFile(dir=path.parent, prefix=f".{path.name}.", delete=False) as out:
            temporary = Path(out.name)
            out.write(data)
        os.chmod(temporary, mode)
        os.replace(temporary, path)
        temporary = None
    finally:
        if temporary is not None:
            temporary.unlink(missing_ok=True)


def patch_compiler(compiler: Path) -> bool:
    """Patch *compiler* in place. Return True if its image changed."""
    source = compiler.read_bytes()
    digest = sha256(source)
    if digest == PATCHED_SHA256:
        marker_path(compiler).write_text(f"{PATCHED_SHA256}\n")
        return False
    if digest != ORIGINAL_SHA256:
        raise PatchError(
            f"refusing unknown compiler {compiler}: expected SHA-256 "
            f"{ORIGINAL_SHA256}, found {digest}"
        )

    result = patch_image(source)
    _atomic_write(compiler, result, compiler.stat().st_mode)
    marker_path(compiler).write_text(f"{PATCHED_SHA256}\n")
    return True


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("compiler", type=Path, help="path to ee-gcc2.96 cc1")
    args = parser.parse_args()
    try:
        changed = patch_compiler(args.compiler)
    except (OSError, PatchError) as exc:
        parser.error(str(exc))
    state = "patched" if changed else "already patched"
    print(f"{state}: {args.compiler} ({PATCHED_SHA256})")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
