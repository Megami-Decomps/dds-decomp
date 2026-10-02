#!/usr/bin/env python3
"""Verify or import every standalone DDS automap resource from an owned disc.

    python3 tools/amb_corpus.py dds1 game.iso
    python3 tools/amb_corpus.py dds1 game.iso \
        --output-dir src/dds1/data/field \
        --manifest config/dds1/field_amb.sha1

The importer reads ``/fld/**/*.AMB`` directly from DDS3.IMG, renders readable
source, and assembles every result byte-for-byte before writing anything.
"""

from __future__ import annotations

import argparse
import hashlib
import io
import sys
from dataclasses import dataclass
from pathlib import Path, PurePosixPath

import pycdlib

import amb
import extract


SECTOR_SIZE = 0x800


@dataclass(frozen=True)
class Source:
    data: bytes
    text: str
    location: str


def _disc_index(version: str, iso: Path) -> tuple[bytes, int]:
    info = extract.VERSIONS[version]
    cd = pycdlib.PyCdlib()
    cd.open(str(iso))
    try:
        executable = io.BytesIO()
        cd.get_file_from_iso_fp(executable, iso_path=f"/{info['serial']};1")
        digest = hashlib.sha1(executable.getvalue()).hexdigest()
        if digest != info["elf_sha1"]:
            raise amb.AmbError(
                f"{iso} has executable SHA-1 {digest}, expected {info['elf_sha1']} for {version}"
            )
        buffer = io.BytesIO()
        cd.get_file_from_iso_fp(buffer, iso_path="/DDS3.DDT;1")
        img_lsn = cd.get_record(iso_path="/DDS3.IMG;1").extent_location()
        return buffer.getvalue(), img_lsn
    finally:
        cd.close()


def read_sources(version: str, iso: Path) -> dict[str, Source]:
    ddt, img_lsn = _disc_index(version, iso)
    files = tuple(
        entry
        for entry in extract.walk_ddt(ddt)
        if entry[0].lower().startswith("/fld/")
        and PurePosixPath(entry[0]).suffix.lower() == ".amb"
    )
    sources: dict[str, Source] = {}
    with iso.open("rb") as image:
        for path, relative_lsn, size in files:
            image.seek((img_lsn + relative_lsn) * SECTOR_SIZE)
            data = image.read(size)
            if len(data) != size:
                raise amb.AmbError(f"{path} is truncated in DDS3.IMG")
            text = amb.render_source(data)
            if amb.encode(amb.parse_source(text)) != data:
                raise amb.AmbError(f"{path} did not round-trip exactly")
            stem = PurePosixPath(path).stem.lower()
            previous = sources.get(stem)
            if previous is not None:
                raise amb.AmbError(
                    f"{path} conflicts with {previous.location} at source name {stem}.ambasm"
                )
            sources[stem] = Source(data, text, path)
    return sources


def write_sources(
    version: str, sources: dict[str, Source], output_dir: Path, manifest: Path
) -> None:
    output_dir.mkdir(parents=True, exist_ok=True)
    for stem, source in sorted(sources.items()):
        (output_dir / f"{stem}.ambasm").write_text(source.text, encoding="utf-8")
    lines = [
        f"{hashlib.sha1(source.data).hexdigest()}  build/{version}/data/field/{stem}.amb"
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
        sources = read_sources(args.version, args.iso)
        if args.output_dir:
            write_sources(args.version, sources, args.output_dir, args.manifest)
    except (OSError, ValueError, amb.AmbError, pycdlib.pycdlibexception.PyCdlibException) as exc:
        parser.error(str(exc))
    action = "wrote" if args.output_dir else "verified"
    total = sum(len(source.data) for source in sources.values())
    print(f"{args.version}: {action} {len(sources)} exact AMB sources ({total} bytes)")


if __name__ == "__main__":
    main()
