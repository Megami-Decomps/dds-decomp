#!/usr/bin/env python3
"""Verify or import every DDS field NPL and SKY resource from an owned disc.

    python3 tools/field_environment_corpus.py dds1 game.iso
    python3 tools/field_environment_corpus.py dds1 game.iso \
        --output-dir src/dds1/data/field \
        --npl-manifest config/dds1/field_npl.sha1 \
        --sky-manifest config/dds1/field_sky.sha1

The importer reads loose field resources directly from DDS3.IMG, renders
readable sparse source, and assembles every result byte-for-byte before writing
anything.  These are the same payloads embedded in shared field LB archives.
"""

from __future__ import annotations

import argparse
import hashlib
import io
from dataclasses import dataclass
from pathlib import Path, PurePosixPath

import pycdlib

import extract
import npl
import sky


SECTOR_SIZE = 0x800
KINDS = {
    ".npl": (npl, ".nplasm"),
    ".sky": (sky, ".skyasm"),
}


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
            raise ValueError(
                f"{iso} has executable SHA-1 {digest}, expected {info['elf_sha1']} for {version}"
            )
        buffer = io.BytesIO()
        cd.get_file_from_iso_fp(buffer, iso_path="/DDS3.DDT;1")
        img_lsn = cd.get_record(iso_path="/DDS3.IMG;1").extent_location()
        return buffer.getvalue(), img_lsn
    finally:
        cd.close()


def read_sources(version: str, iso: Path) -> dict[str, dict[str, Source]]:
    """Read and exactly round-trip all loose NPL/SKY field resources."""

    ddt, img_lsn = _disc_index(version, iso)
    files = tuple(
        entry
        for entry in extract.walk_ddt(ddt)
        if entry[0].lower().startswith("/fld/")
        and PurePosixPath(entry[0]).suffix.lower() in KINDS
    )
    sources: dict[str, dict[str, Source]] = {suffix: {} for suffix in KINDS}
    with iso.open("rb") as image:
        for path, relative_lsn, size in files:
            suffix = PurePosixPath(path).suffix.lower()
            codec, source_suffix = KINDS[suffix]
            image.seek((img_lsn + relative_lsn) * SECTOR_SIZE)
            data = image.read(size)
            if len(data) != size:
                raise ValueError(f"{path} is truncated in DDS3.IMG")
            text = codec.render_source(data)
            if codec.encode(codec.parse_source(text)) != data:
                raise ValueError(f"{path} did not round-trip exactly")
            name = PurePosixPath(path).stem.lower() + source_suffix
            previous = sources[suffix].get(name)
            if previous is not None:
                if previous.data != data:
                    raise ValueError(
                        f"{path} conflicts with {previous.location} at source name {name}"
                    )
                continue
            sources[suffix][name] = Source(data, text, path)
    return sources


def write_sources(
    version: str,
    sources: dict[str, dict[str, Source]],
    output_dir: Path,
    manifests: dict[str, Path],
) -> None:
    output_dir.mkdir(parents=True, exist_ok=True)
    for suffix, kind_sources in sources.items():
        for name, source in sorted(kind_sources.items()):
            (output_dir / name).write_text(source.text, encoding="utf-8")
        binary_suffix = suffix
        lines = [
            f"{hashlib.sha1(source.data).hexdigest()}  "
            f"build/{version}/data/field/{Path(name).stem}{binary_suffix}"
            for name, source in sorted(kind_sources.items())
        ]
        manifest = manifests[suffix]
        manifest.parent.mkdir(parents=True, exist_ok=True)
        manifest.write_text("\n".join(lines) + "\n", encoding="utf-8")


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("version", choices=("dds1", "dds2"))
    parser.add_argument("iso", type=Path)
    parser.add_argument("--output-dir", type=Path)
    parser.add_argument("--npl-manifest", type=Path)
    parser.add_argument("--sky-manifest", type=Path)
    args = parser.parse_args()
    outputs = (args.output_dir, args.npl_manifest, args.sky_manifest)
    if any(outputs) and not all(outputs):
        parser.error("--output-dir, --npl-manifest, and --sky-manifest must be supplied together")

    try:
        sources = read_sources(args.version, args.iso)
        if args.output_dir:
            write_sources(
                args.version,
                sources,
                args.output_dir,
                {".npl": args.npl_manifest, ".sky": args.sky_manifest},
            )
    except (
        OSError,
        ValueError,
        npl.NplError,
        sky.SkyError,
        pycdlib.pycdlibexception.PyCdlibException,
    ) as exc:
        parser.error(str(exc))

    action = "wrote" if args.output_dir else "verified"
    for suffix, kind_sources in sources.items():
        total = sum(len(source.data) for source in kind_sources.values())
        print(
            f"{args.version}: {action} {len(kind_sources)} exact {suffix[1:].upper()} "
            f"sources ({total} bytes)"
        )


if __name__ == "__main__":
    main()
