#!/usr/bin/env python3
"""Extract the retail executable from a disc image you own into orig/<version>/.

    python tools/extract.py                      # every version whose ISO is found
    python tools/extract.py --iso path/to.iso dds1

The ISO is located by config/versions.json's `iso_glob` in the repository root
and orig/ unless --iso is given. The extracted ELF must match the recorded SHA-1.
"""
from __future__ import annotations

import argparse
import hashlib
import io
import json
import sys
from pathlib import Path

import pycdlib

ROOT = Path(__file__).resolve().parent.parent
VERSIONS = json.loads((ROOT / "config" / "versions.json").read_text())


def find_iso(glob: str) -> Path | None:
    for base in (ROOT, ROOT / "orig"):
        hits = sorted(base.glob(glob))
        if hits:
            return hits[0]
    return None


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
