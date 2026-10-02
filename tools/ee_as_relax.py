#!/usr/bin/env python3
"""Fix EE GAS 2.10's overlapping MIPS relaxation copy.

The assembler stores a provisional short instruction immediately before its
long alternative. When relaxation selects the long form, md_convert_frag
copies the alternative down with memcpy even though the ranges overlap. The
result depends on the host libc's memcpy implementation and buffer address.

This patch redirects that one call to a tiny forward-copy helper placed in
existing executable alignment padding. Forward copying is correct because the
destination is always below the source. Every changed byte and the full
input/output SHA-256 are checked.
"""

from __future__ import annotations

import argparse
import hashlib
import os
import struct
import tempfile
from pathlib import Path


ORIGINAL_SHA256 = "b8cfdb6ecb6931642914020f92a62e653378f57230eed4895f69afa050d47b5e"
PATCHED_SHA256 = "660a9679adcd8e15569002402cb1b0dafff062075a80b7038cb1a28ee7d859ba"
ASSEMBLER_RELATIVE_PATH = Path("ee/bin/as")

IMAGE_BASE = 0x08048000
COPY_HELPER_VA = 0x08067782
COPY_HELPER_OFFSET = COPY_HELPER_VA - IMAGE_BASE
RELAX_COPY_CALL_VA = 0x08079DD3
RELAX_COPY_CALL_OFFSET = RELAX_COPY_CALL_VA - IMAGE_BASE

# The call site has already placed destination, source and length in eax, edx
# and ebx respectively. Preserve the callee-saved string registers around a
# forward rep movsb; the direction flag is clear on this path.
ORIGINAL_HELPER_BYTES = b"\x90" * 13
COPY_HELPER = bytes.fromhex("56 57 89 d9 89 d6 89 c7 f3 a4 5f 5e c3")
ORIGINAL_CALL = bytes.fromhex("e8 d8 f7 fc ff")
PATCHED_CALL = b"\xe8" + struct.pack(
    "<i", COPY_HELPER_VA - (RELAX_COPY_CALL_VA + 5)
)


class PatchError(RuntimeError):
    """The assembler is not the exact binary this patch understands."""


def sha256(data: bytes | bytearray) -> str:
    return hashlib.sha256(data).hexdigest()


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
    """Return a patched ee-as image, failing closed on unexpected input."""
    digest = sha256(source)
    if verify_hash and digest != ORIGINAL_SHA256:
        raise PatchError(
            f"refusing unknown assembler: expected SHA-256 {ORIGINAL_SHA256}, "
            f"found {digest}"
        )

    image = bytearray(source)
    _replace_checked(
        image, COPY_HELPER_OFFSET, ORIGINAL_HELPER_BYTES, COPY_HELPER
    )
    _replace_checked(image, RELAX_COPY_CALL_OFFSET, ORIGINAL_CALL, PATCHED_CALL)

    result = bytes(image)
    if verify_hash and sha256(result) != PATCHED_SHA256:
        raise PatchError("patched assembler checksum does not match the validated image")
    return result


def _atomic_write(path: Path, data: bytes, mode: int) -> None:
    temporary: Path | None = None
    try:
        with tempfile.NamedTemporaryFile(
            dir=path.parent, prefix=f".{path.name}.", delete=False
        ) as out:
            temporary = Path(out.name)
            out.write(data)
        os.chmod(temporary, mode)
        os.replace(temporary, path)
        temporary = None
    finally:
        if temporary is not None:
            temporary.unlink(missing_ok=True)


def patch_assembler(assembler: Path) -> bool:
    """Patch *assembler* in place. Return True if its image changed."""
    source = assembler.read_bytes()
    digest = sha256(source)
    if digest == PATCHED_SHA256:
        return False
    if digest != ORIGINAL_SHA256:
        raise PatchError(
            f"refusing unknown assembler {assembler}: expected SHA-256 "
            f"{ORIGINAL_SHA256}, found {digest}"
        )

    result = patch_image(source)
    _atomic_write(assembler, result, assembler.stat().st_mode)
    return True


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("assembler", type=Path, help="path to ee-gcc2.96 ee-as")
    args = parser.parse_args()
    try:
        changed = patch_assembler(args.assembler)
    except (OSError, PatchError) as exc:
        parser.error(str(exc))
    state = "patched" if changed else "already patched"
    print(f"{state}: {args.assembler} ({PATCHED_SHA256})")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
