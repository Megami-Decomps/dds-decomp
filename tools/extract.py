#!/usr/bin/env python3
"""Extract required retail inputs from a disc image you own into orig/<version>/.

    python tools/extract.py                      # every version whose ISO is found
    python tools/extract.py --iso path/to.iso dds1

The ISO is located by config/versions.json's `iso_glob` in the repository root
and orig/ unless --iso is given. Every extracted file must match its recorded
SHA-1. Selected DDS3 archive files are read directly through DDS3.DDT without
copying the roughly 1.5 GiB DDS3.IMG out of the disc image.
"""
from __future__ import annotations

import argparse
import hashlib
import io
import json
import struct
import sys
from collections.abc import Iterator
from pathlib import Path

import pycdlib

ROOT = Path(__file__).resolve().parent.parent
VERSIONS = json.loads((ROOT / "config" / "versions.json").read_text())
SECTOR_SIZE = 0x800


def find_iso(glob: str) -> Path | None:
    for base in (ROOT, ROOT / "orig"):
        hits = sorted(base.glob(glob))
        if hits:
            return hits[0]
    return None


def _ddt_entry(data: bytes, offset: int) -> tuple[int, int, int]:
    if offset < 0 or offset + 12 > len(data):
        raise ValueError(f"DDS3.DDT entry offset 0x{offset:x} is out of range")
    return struct.unpack_from("<IIi", data, offset)


def _ddt_name(data: bytes, offset: int) -> str:
    if offset < 0 or offset >= len(data):
        raise ValueError(f"DDS3.DDT name offset 0x{offset:x} is out of range")
    end = data.find(b"\0", offset)
    if end < 0:
        raise ValueError(f"DDS3.DDT name at 0x{offset:x} is unterminated")
    return data[offset:end].decode("ascii")


def _resolve_ddt(data: bytes, path: str) -> tuple[int, int]:
    offset = 0
    for component in (part for part in path.split("/") if part):
        _, children, neg_count = _ddt_entry(data, offset)
        if neg_count >= 0:
            raise ValueError(f"{path}: {component!r} follows a non-directory entry")
        low, high = 0, -neg_count
        while low < high:
            middle = (low + high) // 2
            candidate = children + middle * 12
            name_offset, _, _ = _ddt_entry(data, candidate)
            if _ddt_name(data, name_offset) < component:
                low = middle + 1
            else:
                high = middle
        if low >= -neg_count:
            raise FileNotFoundError(f"{path}: component {component!r} was not found")
        offset = children + low * 12
        name_offset, _, _ = _ddt_entry(data, offset)
        if _ddt_name(data, name_offset) != component:
            raise FileNotFoundError(f"{path}: component {component!r} was not found")
    _, relative_lsn, size = _ddt_entry(data, offset)
    if size < 0:
        raise IsADirectoryError(path)
    return relative_lsn, size


def walk_ddt(data: bytes) -> Iterator[tuple[str, int, int]]:
    """Yield every DDS3.DDT file as (absolute path, relative LSN, size)."""

    active: set[int] = set()

    def visit(offset: int, parent: str) -> Iterator[tuple[str, int, int]]:
        if offset in active:
            raise ValueError(f"DDS3.DDT directory cycle at entry 0x{offset:x}")
        name_offset, location, count = _ddt_entry(data, offset)
        name = _ddt_name(data, name_offset)
        path = parent if not name else f"{parent}/{name}"
        if count >= 0:
            yield path or "/", location, count
            return

        active.add(offset)
        try:
            for index in range(-count):
                yield from visit(location + index * 12, path)
        finally:
            active.remove(offset)

    yield from visit(0, "")


def _extract_archive_files(version: str, iso: Path, cd: pycdlib.PyCdlib, out_dir: Path) -> None:
    files = VERSIONS[version].get("archive_files", ())
    if not files:
        return
    ddt_buffer = io.BytesIO()
    cd.get_file_from_iso_fp(ddt_buffer, iso_path="/DDS3.DDT;1")
    ddt = ddt_buffer.getvalue()
    img_lsn = cd.get_record(iso_path="/DDS3.IMG;1").extent_location()

    with iso.open("rb") as image:
        for spec in files:
            relative_lsn, size = _resolve_ddt(ddt, spec["path"])
            if size != spec["size"]:
                raise SystemExit(
                    f"{version}: {spec['path']} size {size} != expected {spec['size']}; wrong or modified disc"
                )
            image.seek((img_lsn + relative_lsn) * SECTOR_SIZE)
            data = image.read(size)
            digest = hashlib.sha1(data).hexdigest()
            if len(data) != size or digest != spec["sha1"]:
                raise SystemExit(
                    f"{version}: {spec['path']} SHA-1 {digest} != expected {spec['sha1']}; wrong or modified disc"
                )
            output = out_dir / spec["output"]
            output.parent.mkdir(parents=True, exist_ok=True)
            output.write_bytes(data)
            print(f"{version}: {output.relative_to(ROOT)} OK ({digest})")


def extract(version: str, iso: Path) -> Path:
    info = VERSIONS[version]
    serial = info["serial"]
    out_dir = ROOT / "orig" / version
    out_dir.mkdir(parents=True, exist_ok=True)

    cd = pycdlib.PyCdlib()
    cd.open(str(iso))
    try:
        for name in (serial, "SYSTEM.CNF"):
            buf = io.BytesIO()
            cd.get_file_from_iso_fp(buf, iso_path=f"/{name};1")
            (out_dir / name).write_bytes(buf.getvalue())
        _extract_archive_files(version, iso, cd, out_dir)
    finally:
        cd.close()

    elf = out_dir / serial
    digest = hashlib.sha1(elf.read_bytes()).hexdigest()
    if digest != info["elf_sha1"]:
        elf.unlink()
        raise SystemExit(f"{version}: {serial} SHA-1 {digest} != expected {info['elf_sha1']}; wrong or modified disc")
    print(f"{version}: {elf.relative_to(ROOT)} OK ({digest})")
    return elf


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("versions", nargs="*", help=f"any of: {', '.join(VERSIONS)} (default: all)")
    ap.add_argument("--iso", type=Path, help="disc image (requires exactly one version)")
    args = ap.parse_args()
    unknown = set(args.versions) - VERSIONS.keys()
    if unknown:
        ap.error(f"unknown version(s): {', '.join(sorted(unknown))}")

    if args.iso:
        if len(args.versions) != 1:
            ap.error("--iso needs exactly one version")
        extract(args.versions[0], args.iso)
        return

    found = False
    for version in args.versions or VERSIONS:
        iso = find_iso(VERSIONS[version]["iso_glob"])
        if iso is None:
            print(f"{version}: no ISO matching {VERSIONS[version]['iso_glob']!r}; skipped", file=sys.stderr)
            continue
        extract(version, iso)
        found = True
    if not found:
        raise SystemExit("no disc images found; pass --iso")


if __name__ == "__main__":
    main()
