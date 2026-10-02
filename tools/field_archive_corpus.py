#!/usr/bin/env python3
"""Verify or import the shared DDS field archives from an owned disc.

    python3 tools/field_archive_corpus.py dds1 game.iso --resources build/dds1
    python3 tools/field_archive_corpus.py dds1 game.iso --resources build/dds1 --write

Each shared ``fNNN_000.LB`` archive contains INF, NPL, SKY, WAP, BF, and AMB
resources.  Variant archives use the same layout.  This tool identifies each
entry by its exact rebuilt payload, writes fully authored ``.lbasm`` sources,
and verifies that those sources reproduce every retail archive byte-for-byte.
"""

from __future__ import annotations

import argparse
import hashlib
import io
import json
import re
import sys
from dataclasses import dataclass
from pathlib import Path, PurePosixPath

import pycdlib

import extract
import lb


SECTOR_SIZE = 0x800
SHARED_EXTENSIONS = ("INF", "NPL", "SKY", "WAP", "BF", "AMB")
ARCHIVE_PATH = re.compile(
    r"/fld/f/(?P<field>f\d{3})/(?P<name>(?P=field)_00(?:0|[a-d]))\.LB",
    re.IGNORECASE,
)
RESOURCE_GLOBS = {
    "INF": "data/field/*.inf",
    "NPL": "data/field/*.npl",
    "SKY": "data/field/*.sky",
    "WAP": "data/field/*.wap",
    "BF": "scripts/field/*.bf",
    "AMB": "data/field/*.amb",
}


@dataclass(frozen=True)
class DiscArchive:
    path: str
    name: str
    field: str
    data: bytes
    source: str


def _sha1(data: bytes) -> str:
    return hashlib.sha1(data).hexdigest()


def _disc_index(version: str, iso: Path) -> tuple[bytes, int]:
    info = extract.VERSIONS[version]
    cd = pycdlib.PyCdlib()
    cd.open(str(iso))
    try:
        executable = io.BytesIO()
        cd.get_file_from_iso_fp(executable, iso_path=f"/{info['serial']};1")
        digest = _sha1(executable.getvalue())
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


def _resource_index(resources: Path) -> dict[tuple[str, str], list[PurePosixPath]]:
    result: dict[tuple[str, str], list[PurePosixPath]] = {}
    for extension, pattern in RESOURCE_GLOBS.items():
        matches = sorted(resources.glob(pattern))
        if not matches:
            raise ValueError(f"no rebuilt {extension} resources match {resources / pattern}")
        for path in matches:
            relative = PurePosixPath(path.relative_to(resources).as_posix())
            result.setdefault((extension, _sha1(path.read_bytes())), []).append(relative)
    return result


def _preferred_stems(name: str, field: str) -> tuple[str, ...]:
    suffix = name.removeprefix(field + "_00")
    variant = field + suffix if suffix != "0" else field
    return (variant, field) if variant != field else (field,)


def _choose_resource(
    entry: lb.Entry,
    raw: bytes,
    archive_name: str,
    field: str,
    resources: dict[tuple[str, str], list[PurePosixPath]],
) -> PurePosixPath:
    candidates = resources.get((entry.extension.upper(), _sha1(raw)), ())
    if not candidates:
        raise ValueError(
            f"{archive_name}: {entry.extension} entry {entry.index} has no exact rebuilt resource"
        )
    for stem in _preferred_stems(archive_name, field):
        preferred = [path for path in candidates if path.stem.lower() == stem.lower()]
        if len(preferred) == 1:
            return preferred[0]
    if len(candidates) == 1:
        return candidates[0]
    choices = ", ".join(path.as_posix() for path in candidates)
    raise ValueError(
        f"{archive_name}: {entry.extension} entry {entry.index} ambiguously matches {choices}"
    )


def _render_source(
    data: bytes,
    name: str,
    field: str,
    resources: Path,
    resource_index: dict[tuple[str, str], list[PurePosixPath]],
) -> str:
    archive = lb.parse_archive(data)
    if tuple(entry.extension.upper() for entry in archive.entries) != SHARED_EXTENSIONS:
        raise ValueError(f"{name}: expected the shared six-entry field layout")
    lines = ["lb 1", f"base sha1={_sha1(data)} size=0x{len(data):x}"]
    for entry in archive.entries:
        raw = lb.entry_data(entry)
        source = _choose_resource(entry, raw, name, field, resource_index)
        if (resources / source).read_bytes() != raw:
            raise AssertionError(source)
        lines.append(
            "entry "
            f"type={entry.type_id} compressed={int(entry.compressed)} user={entry.user_id} "
            f"extension={entry.extension} raw_size=0x{entry.raw_size:x} "
            f"retail_sha1={_sha1(raw)} source={source.as_posix()}"
        )
    text = "\n".join(lines) + "\n"
    rebuilt = lb.assemble(lb.parse_source(text), data, resources)
    if rebuilt != data:
        raise ValueError(f"{name}: generated source did not reproduce the retail archive")
    return text


def read_archives(version: str, iso: Path, resources: Path) -> list[DiscArchive]:
    ddt, img_lsn = _disc_index(version, iso)
    resource_index = _resource_index(resources)
    result: list[DiscArchive] = []
    with iso.open("rb") as image:
        for path, relative_lsn, size in extract.walk_ddt(ddt):
            match = ARCHIVE_PATH.fullmatch(path)
            if match is None:
                continue
            image.seek((img_lsn + relative_lsn) * SECTOR_SIZE)
            data = image.read(size)
            if len(data) != size:
                raise ValueError(f"{path} is truncated in DDS3.IMG")
            try:
                archive = lb.parse_archive(data)
            except lb.LbError:
                continue
            if tuple(entry.extension.upper() for entry in archive.entries) != SHARED_EXTENSIONS:
                continue
            name = match.group("name").lower()
            field = match.group("field").lower()
            source = _render_source(data, name, field, resources, resource_index)
            result.append(DiscArchive(path, name, field, data, source))
    if not result:
        raise ValueError("disc contains no shared six-entry field archives")
    return sorted(result, key=lambda archive: archive.path.lower())


def _archive_records(archives: list[DiscArchive]) -> list[dict[str, object]]:
    return [
        {
            "path": archive.path,
            "output": f"field/{archive.name}.LB",
            "size": len(archive.data),
            "sha1": _sha1(archive.data),
        }
        for archive in archives
    ]


def _checksum_entries(version: str, archives: list[DiscArchive]) -> dict[str, str]:
    return {
        f"build/{version}/data/field/{archive.name}.LB": _sha1(archive.data)
        for archive in archives
    }


def _read_checksums(path: Path) -> dict[str, str]:
    result: dict[str, str] = {}
    for number, line in enumerate(path.read_text(encoding="utf-8").splitlines(), 1):
        parts = line.split()
        if len(parts) != 2 or not re.fullmatch(r"[0-9a-f]{40}", parts[0]):
            raise ValueError(f"{path}:{number}: invalid SHA-1 manifest row")
        result[parts[1]] = parts[0]
    return result


def write_corpus(
    version: str,
    archives: list[DiscArchive],
    source_dir: Path,
    archive_manifest: Path,
    checksum_manifest: Path,
) -> None:
    source_dir.mkdir(parents=True, exist_ok=True)
    for archive in archives:
        (source_dir / f"{archive.name}.lbasm").write_text(archive.source, encoding="utf-8")
    archive_manifest.parent.mkdir(parents=True, exist_ok=True)
    archive_manifest.write_text(
        json.dumps(_archive_records(archives), indent=2) + "\n", encoding="utf-8"
    )
    checksums = _read_checksums(checksum_manifest) if checksum_manifest.exists() else {}
    checksums.update(_checksum_entries(version, archives))
    checksum_manifest.write_text(
        "".join(f"{digest}  {path}\n" for path, digest in sorted(checksums.items())),
        encoding="utf-8",
    )


def verify_corpus(
    version: str,
    archives: list[DiscArchive],
    source_dir: Path,
    archive_manifest: Path,
    checksum_manifest: Path,
) -> None:
    for archive in archives:
        path = source_dir / f"{archive.name}.lbasm"
        if path.read_text(encoding="utf-8") != archive.source:
            raise ValueError(f"{path}: source differs from the owned-disc corpus")
    records = json.loads(archive_manifest.read_text(encoding="utf-8"))
    if records != _archive_records(archives):
        raise ValueError(f"{archive_manifest}: archive records differ from the owned-disc corpus")
    checksums = _read_checksums(checksum_manifest)
    for path, digest in _checksum_entries(version, archives).items():
        if checksums.get(path) != digest:
            raise ValueError(f"{checksum_manifest}: missing or incorrect checksum for {path}")


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("version", choices=("dds1", "dds2"))
    parser.add_argument("iso", type=Path)
    parser.add_argument("--resources", required=True, type=Path)
    parser.add_argument("--source-dir", type=Path)
    parser.add_argument("--archive-manifest", type=Path)
    parser.add_argument("--checksum-manifest", type=Path)
    parser.add_argument("--write", action="store_true")
    args = parser.parse_args()

    source_dir = args.source_dir or Path("src") / args.version / "data" / "field"
    archive_manifest = args.archive_manifest or Path("config") / args.version / "field_archives.json"
    checksum_manifest = args.checksum_manifest or Path("config") / args.version / "field_lb.sha1"
    try:
        archives = read_archives(args.version, args.iso, args.resources)
        if args.write:
            write_corpus(args.version, archives, source_dir, archive_manifest, checksum_manifest)
        else:
            verify_corpus(args.version, archives, source_dir, archive_manifest, checksum_manifest)
    except (
        OSError,
        ValueError,
        json.JSONDecodeError,
        lb.LbError,
        pycdlib.pycdlibexception.PyCdlibException,
    ) as exc:
        parser.error(str(exc))
    total = sum(len(archive.data) for archive in archives)
    action = "wrote" if args.write else "verified"
    print(
        f"{args.version}: {action} {len(archives)} exact shared field archives "
        f"({len(archives) * len(SHARED_EXTENSIONS)} authored entries, {total} bytes)"
    )


if __name__ == "__main__":
    main()
