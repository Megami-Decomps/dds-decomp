#!/usr/bin/env python3
"""Import the script and data corpus from a disc image you own.

    python3 tools/import_corpus.py dds1 game.iso

Script and data sources are retail content, so they are not tracked. This
writes src/<version>/scripts and src/<version>/data as the SHA-1 manifests in
config/<version>/ describe them, with the format tools that verify them, and
never changes a manifest. Two parts of the corpus are not derived from the
disc: the local-variable names in config/<version>/script_locals.json, and LB
entries rebuilt from corpus sources instead of copied from the retail archive.

Build the script, field, archive and battle data targets afterwards to check
every source against retail bytes.
"""
from __future__ import annotations

import argparse
import dataclasses
import hashlib
import json
import re
import shutil
import tempfile
from pathlib import Path

import pycdlib

import amb
import amb_corpus
import battle_tbl
import extract
import fld
import fld_corpus
import field_archive_corpus
import field_environment_corpus
import flw0
import flw0_symbolic
import inf
import lb
import npl
import sky
import wap

ROOT = Path(__file__).resolve().parent.parent
SECTOR_SIZE = 0x800
# In dependency order: later tables resolve symbols from earlier sources.
BATTLE_CONTEXT = {
    "MSG": (),
    "SKILL": (),
    "UNIT": ("--messages", "msg", "--skills", "skill"),
    "ENCOUNT": ("--messages", "msg", "--skills", "skill", "--units", "unit"),
    "AICALC": ("--messages", "msg", "--skills", "skill", "--units", "unit"),
}
LOCAL_SLOT = re.compile(r"(local_int|local_float)\[(\d+)\]")
BASE_ENTRY = re.compile(r"extension=(\S+) .* retail_sha1=([0-9a-f]{40}) source=base$")


def _sha1(data: bytes) -> str:
    return hashlib.sha1(data).hexdigest()


def _manifest(path: Path) -> dict[str, str]:
    """Build paths and their SHA-1s, in manifest order."""
    rows = (line.split() for line in path.read_text(encoding="utf-8").splitlines())
    return {name: digest for digest, name in rows}


def _same(generated: Path, tracked: Path) -> None:
    if generated.read_bytes() != tracked.read_bytes():
        raise ValueError(f"the disc does not reproduce {tracked.relative_to(ROOT)}")


class Disc:
    def __init__(self, version: str, iso: Path) -> None:
        ddt, self.img_lsn = fld_corpus._disc_index(version, iso)
        self.iso = iso
        self.files = {path.lower(): (lsn, size) for path, lsn, size in extract.walk_ddt(ddt)}

    def find(self, name: str) -> str:
        """The one DDS3.IMG path with this file name."""
        paths = [path for path in self.files if path.rsplit("/", 1)[-1] == name.lower()]
        if len(paths) != 1:
            raise ValueError(f"{name} matches {len(paths)} files on the disc, expected one")
        return paths[0]

    def read(self, path: str, digest: str) -> bytes:
        """Read one DDS3.IMG file, which must match its manifest SHA-1."""
        try:
            lsn, size = self.files[path.lower()]
        except KeyError:
            raise ValueError(f"{path} is not on the disc") from None
        with self.iso.open("rb") as image:
            image.seek((self.img_lsn + lsn) * SECTOR_SIZE)
            data = image.read(size)
        if _sha1(data) != digest:
            raise ValueError(f"{path} has SHA-1 {_sha1(data)}, expected {digest}")
        return data


def load_locals(path: Path) -> dict[str, tuple[flw0.LocalAlias, ...]]:
    """Local-variable names per script, keyed by group/name."""
    if not path.exists():
        return {}
    result = {}
    for script, names in json.loads(path.read_text(encoding="utf-8")).items():
        aliases = []
        for name, slot in names.items():
            match = LOCAL_SLOT.fullmatch(slot)
            if match is None:
                raise ValueError(f"{path.name}: {script}: {name} has invalid slot {slot!r}")
            aliases.append(flw0.LocalAlias(name, match.group(1), int(match.group(2))))
        result[script] = tuple(aliases)
    return result


def render_script(data: bytes, profile: str, aliases: tuple[flw0.LocalAlias, ...]) -> str:
    """Structured symbolic source, or physical source for a nonstandard layout."""
    script = dataclasses.replace(flw0.parse(data), local_aliases=aliases)
    try:
        return flw0_symbolic.render(script, profile, semantic=True, structured=True)
    except flw0.Flw0Error:
        if aliases:
            raise
        return flw0.render_source(script, profile)


def rewire(source: str, stem: str, resources: dict[tuple[str, str], list[str]]) -> str:
    """Point retail LB entries at the rebuilt resource with the same bytes.

    A resource named after the archive wins when several share those bytes.
    """
    lines = []
    for line in source.splitlines():
        match = BASE_ENTRY.search(line)
        candidates = resources.get((match.group(1).upper(), match.group(2)), []) if match else []
        named = [path for path in candidates if Path(path).stem == stem]
        chosen = named or candidates
        if len(chosen) > 1:
            raise ValueError(f"{stem}: {match.group(1)} entry matches {', '.join(chosen)}")
        if chosen:
            line = line.removesuffix("source=base") + f"source={chosen[0]}"
        lines.append(line)
    return "\n".join(lines) + "\n"


def import_corpus(version: str, iso: Path, source_dir: Path) -> dict[str, int]:
    config = ROOT / "config" / version
    disc = Disc(version, iso)
    script_dir = source_dir / "scripts"
    field_dir = source_dir / "data" / "field"
    battle_dir = source_dir / "data" / "battle"
    archives_lb = _manifest(config / "field_lb.sha1")
    counts: dict[str, int] = {}
    with tempfile.TemporaryDirectory() as scratch_name:
        scratch = Path(scratch_name)
        resources = scratch / "resources"

        def keep(relative: str, data: bytes) -> None:
            """Keep a retail resource for the LB archive sources."""
            (resources / relative).parent.mkdir(parents=True, exist_ok=True)
            (resources / relative).write_bytes(data)

        def write(path: Path, text: str) -> None:
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text(text, encoding="utf-8")

        script_locals = load_locals(config / "script_locals.json")
        scripts = _manifest(config / "scripts.sha1")
        for build_path, digest in scripts.items():
            group, name = Path(build_path).parent.name, Path(build_path).stem
            data = disc.read(disc.find(f"{name}.bf"), digest)
            aliases = script_locals.get(f"{group}/{name}", ())
            write(script_dir / group / f"{name}.bfasm", render_script(data, version, aliases))
            keep(f"scripts/{group}/{name}.bf", data)
        counts["scripts"] = len(scripts)

        interactions = _manifest(config / "field_inf.sha1")
        for build_path, digest in interactions.items():
            stem = Path(build_path).stem
            data = disc.read(f"/fld/f/{stem}/{stem}.inf", digest)
            by_index, _ = inf.load_message_symbols(script_dir / "field" / f"{stem}.bfasm")
            write(field_dir / f"{stem}.infasm", inf.render_source(inf.decode(data), by_index))
            keep(f"data/field/{stem}.inf", data)
        actors = _manifest(config / "field_wap.sha1")
        for build_path, digest in actors.items():
            stem = Path(build_path).stem
            data = disc.read(f"/fld/f/{stem}/{stem}.wap", digest)
            references = wap.load_references(
                script_dir / "field" / f"{stem}.bfasm", field_dir / f"{stem}.infasm"
            )
            write(field_dir / f"{stem}.wapasm", wap.render_source(wap.decode(data), references))
            keep(f"data/field/{stem}.wap", data)
        counts["INF and WAP tables"] = len(interactions) + len(actors)

        battle_dir.mkdir(parents=True, exist_ok=True)
        tables = _manifest(config / "battle_tables.sha1")
        for table, context in BATTLE_CONTEXT.items():
            retail = scratch / f"{table}.TBL"
            retail.write_bytes(
                disc.read(f"/battle/{table}.TBL", tables[f"build/{version}/data/battle/{table}.TBL"])
            )
            arguments = [
                str(battle_dir / f"{word}.tblasm") if index % 2 else word
                for index, word in enumerate(context)
            ]
            output = battle_dir / f"{table.lower()}.tblasm"
            battle_tbl.main(["disassemble", *arguments, str(retail), str(output)])
        counts["battle tables"] = len(tables)

        fld2, _, _ = fld_corpus.read_sources(version, iso, "fld2")
        fld_corpus.write_sources(version, fld2, field_dir, scratch / "field_fld2.sha1", "fld2")
        _same(scratch / "field_fld2.sha1", config / "field_fld2.sha1")
        for stem, source in fld2.items():
            keep(f"data/field/{stem}.f2", source.data)
        # Only the FLD1 payloads the manifest names, from their LB archives.
        fld1 = _manifest(config / "field_fld1.sha1")
        for build_path, digest in fld1.items():
            stem = Path(build_path).stem
            archive = f"/fld/f/{stem[:4]}/{stem}.LB"
            packed = disc.read(archive, archives_lb[f"build/{version}/data/field/{stem}.LB"])
            payloads = fld_corpus._payloads(archive, packed, "fld1")
            data = next((data for _, data in payloads if _sha1(data) == digest), None)
            if data is None:
                raise ValueError(f"{archive} has no FLD1 payload with SHA-1 {digest}")
            write(field_dir / f"{stem}.f1asm", fld.render_source(data))
            keep(f"data/field/{stem}.f1", data)
        counts["FLD1 and FLD2 sources"] = len(fld2) + len(fld1)

        automaps = amb_corpus.read_sources(version, iso)
        amb_corpus.write_sources(version, automaps, field_dir, scratch / "field_amb.sha1")
        _same(scratch / "field_amb.sha1", config / "field_amb.sha1")
        for stem, source in automaps.items():
            keep(f"data/field/{stem}.amb", source.data)
        environment = field_environment_corpus.read_sources(version, iso)
        field_environment_corpus.write_sources(
            version,
            environment,
            field_dir,
            {".npl": scratch / "field_npl.sha1", ".sky": scratch / "field_sky.sha1"},
        )
        for kind in ("npl", "sky"):
            _same(scratch / f"field_{kind}.sha1", config / f"field_{kind}.sha1")
            for name, source in environment[f".{kind}"].items():
                keep(f"data/field/{Path(name).stem}.{kind}", source.data)
        counts["AMB, NPL and SKY sources"] = len(automaps) + sum(map(len, environment.values()))

        archives = field_archive_corpus.read_archives(version, iso, resources)
        shutil.copyfile(config / "field_lb.sha1", scratch / "field_lb.sha1")
        field_archive_corpus.write_corpus(
            version, archives, field_dir, scratch / "field_archives.json", scratch / "field_lb.sha1"
        )
        _same(scratch / "field_archives.json", config / "field_archives.json")
        _same(scratch / "field_lb.sha1", config / "field_lb.sha1")
        rebuilt: dict[tuple[str, str], list[str]] = {}
        for path in sorted(resources.rglob("*.*")):
            key = (path.suffix[1:].upper(), _sha1(path.read_bytes()))
            rebuilt.setdefault(key, []).append(path.relative_to(resources).as_posix())
        shared = {f"build/{version}/data/field/{archive.name}.LB" for archive in archives}
        others = {path: digest for path, digest in archives_lb.items() if path not in shared}
        for build_path, digest in others.items():
            stem = Path(build_path).stem
            data = disc.read(f"/fld/f/{stem[:4]}/{stem}.LB", digest)
            source = rewire(lb.render_source(data), stem, rebuilt)
            if lb.assemble(lb.parse_source(source), data, resources) != data:
                raise ValueError(f"{stem}.lbasm does not reproduce the retail archive")
            write(field_dir / f"{stem}.lbasm", source)
        counts["LB archive sources"] = len(archives) + len(others)
    return counts


def main() -> None:
    parser = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parser.add_argument("version", choices=("dds1", "dds2"))
    parser.add_argument("iso", type=Path)
    parser.add_argument("--source-dir", type=Path, help="output root, default src/<version>")
    args = parser.parse_args()
    source_dir = args.source_dir or ROOT / "src" / args.version
    try:
        counts = import_corpus(args.version, args.iso, source_dir)
    except (
        OSError,
        ValueError,
        amb.AmbError,
        battle_tbl.BattleTableError,
        fld.FldError,
        flw0.Flw0Error,
        inf.InfError,
        lb.LbError,
        npl.NplError,
        sky.SkyError,
        wap.WapError,
        pycdlib.pycdlibexception.PyCdlibException,
    ) as exc:
        parser.error(str(exc))
    for kind, count in counts.items():
        print(f"{args.version}: wrote {count} {kind}")


if __name__ == "__main__":
    main()
