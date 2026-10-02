#!/usr/bin/env python3
"""Verify or import the complete version-23 FLD2 corpus from an owned disc.

    python3 tools/fld_corpus.py dds1 game.iso
    python3 tools/fld_corpus.py dds1 game.iso \
        --output-dir src/dds1/data/field \
        --manifest config/dds1/field_fld2.sha1

The importer reads FLD2 payloads directly from loose files and LB archives in
DDS3.IMG. Every emitted source is assembled again and compared byte-for-byte
before any output is written. Identical loose/archive copies share one source.
Older versions and non-FLD2 `.f2` payloads are reported and left for their own
format profiles.
"""
from __future__ import annotations

import argparse
import hashlib
import io
import struct
import sys
from dataclasses import dataclass
from pathlib import Path, PurePosixPath

import pycdlib

import extract
import fld
import lb


SECTOR_SIZE = 0x800
VERSION = 23


@dataclass(frozen=True)
class Source:
    data: bytes
    text: str
    locations: tuple[str, ...]


def _disc_index(version: str, iso: Path) -> tuple[bytes, int]:
    info = extract.VERSIONS[version]
    cd = pycdlib.PyCdlib()
    cd.open(str(iso))
    try:
        executable = io.BytesIO()
        cd.get_file_from_iso_fp(executable, iso_path=f"/{info['serial']};1")
        digest = hashlib.sha1(executable.getvalue()).hexdigest()
        if digest != info["elf_sha1"]:
            raise fld.FldError(
                f"{iso} has executable SHA-1 {digest}, expected {info['elf_sha1']} for {version}"
            )
        buffer = io.BytesIO()
        cd.get_file_from_iso_fp(buffer, iso_path="/DDS3.DDT;1")
        img_lsn = cd.get_record(iso_path="/DDS3.IMG;1").extent_location()
        return buffer.getvalue(), img_lsn
    finally:
        cd.close()


def _field_files(ddt: bytes) -> tuple[tuple[str, int, int], ...]:
    return tuple(
        entry
        for entry in extract.walk_ddt(ddt)
        if entry[0].lower().startswith("/fld/")
        and PurePosixPath(entry[0]).suffix.lower() in {".lb", ".f2"}
    )


def _payloads(path: str, data: bytes) -> tuple[tuple[str, bytes], ...]:
    if path.lower().endswith(".f2"):
        return ((path, data),)
    entries = tuple(
        entry
        for entry in lb.parse_archive(data).entries
        if entry.extension.upper() == "F2"
    )
    if len(entries) > 1:
        raise fld.FldError(f"{path} contains multiple FLD2 entries")
    return tuple((f"{path}#{entry.index}", lb.entry_data(entry)) for entry in entries)


def read_sources(
    version: str, iso: Path
) -> tuple[dict[str, Source], tuple[str, ...], int]:
    ddt, img_lsn = _disc_index(version, iso)
    sources: dict[str, Source] = {}
    non_v23: list[str] = []
    payload_count = 0
    files = _field_files(ddt)

    with iso.open("rb") as image:
        for file_index, (path, relative_lsn, size) in enumerate(files, 1):
            image.seek((img_lsn + relative_lsn) * SECTOR_SIZE)
            packed = image.read(size)
            if len(packed) != size:
                raise fld.FldError(f"{path} is truncated in DDS3.IMG")

            for location, data in _payloads(path, packed):
                payload_count += 1
                if (
                    len(data) < 8
                    or struct.unpack_from("<I", data)[0] != VERSION
                    or data[4:8] != b"FLD2"
                ):
                    non_v23.append(location)
                    continue

                text = fld.render_source(data)
                rebuilt = fld.encode(fld.parse_source(text))
                if rebuilt != data:
                    raise fld.FldError(f"{location} did not round-trip exactly")

                stem = PurePosixPath(path).stem.lower()
                old = sources.get(stem)
                if old is None:
                    sources[stem] = Source(data, text, (location,))
                elif old.data != data:
                    raise fld.FldError(
                        f"{location} conflicts with {old.locations[0]} at source name {stem}.fldasm"
                    )
                else:
                    sources[stem] = Source(old.data, old.text, old.locations + (location,))

            if file_index % 100 == 0:
                print(f"verified {file_index}/{len(files)} field files", file=sys.stderr)

    return sources, tuple(non_v23), payload_count


def write_sources(
    version: str, sources: dict[str, Source], output_dir: Path, manifest: Path
) -> None:
    output_dir.mkdir(parents=True, exist_ok=True)
    for stem, source in sorted(sources.items()):
        (output_dir / f"{stem}.fldasm").write_text(source.text, encoding="utf-8")

    lines = [
        f"{hashlib.sha1(source.data).hexdigest()}  build/{version}/data/field/{stem}.f2"
        for stem, source in sorted(sources.items())
    ]
    manifest.parent.mkdir(parents=True, exist_ok=True)
    manifest.write_text("\n".join(lines) + "\n", encoding="utf-8")


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("version", choices=("dds1", "dds2"))
    parser.add_argument("iso", type=Path)
    parser.add_argument("--output-dir", type=Path)
    parser.add_argument("--manifest", type=Path)
    args = parser.parse_args()
    if bool(args.output_dir) != bool(args.manifest):
        parser.error("--output-dir and --manifest must be supplied together")

    try:
        sources, non_v23, payload_count = read_sources(args.version, args.iso)
        if args.output_dir:
            write_sources(args.version, sources, args.output_dir, args.manifest)
    except (
        OSError,
        ValueError,
        fld.FldError,
        lb.LbError,
        pycdlib.pycdlibexception.PyCdlibException,
    ) as exc:
        parser.error(str(exc))

    duplicates = sum(len(source.locations) - 1 for source in sources.values())
    action = "wrote" if args.output_dir else "verified"
    print(
        f"{args.version}: {action} {len(sources)} exact version-{VERSION} sources "
        f"from {payload_count} FLD2 payloads ({duplicates} identical duplicates, "
        f"{len(non_v23)} non-v23 payloads)"
    )
    for location in non_v23:
        print(f"{args.version}: non-v23 profile: {location}", file=sys.stderr)


if __name__ == "__main__":
    main()
