#!/usr/bin/env python3
"""Capture exact-wrapper EE GCC assembly and RTL dumps outside the checkout.

The public `tools/cc.sh` already preserves the canonical input/output filename
lengths that affect this compiler.  This private harness adds `-da -dumpbase`
and `DDS_KEEP_S`, inventories every artifact, and optionally extracts one
function across all passes.

Example:

    python3 tools/ee_gcc_probe.py \
      src/dds1/kernel/dds3KernelCore.c \
      --function kwlnTaskActivate
"""

from __future__ import annotations

import argparse
import hashlib
import json
import os
import re
import subprocess
import sys
import tempfile
from datetime import datetime, timezone
from pathlib import Path


ROOT = Path(__file__).resolve().parent.parent
DEFAULT_REPO = ROOT
FUNCTION_HEADER = re.compile(r"^;; Function ([^\s]+).*$", re.M)


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as fh:
        for chunk in iter(lambda: fh.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def inside(path: Path, root: Path) -> bool:
    try:
        path.resolve().relative_to(root.resolve())
        return True
    except ValueError:
        return False


def extract_dump_function(text: str, name: str) -> str | None:
    matches = list(FUNCTION_HEADER.finditer(text))
    for index, match in enumerate(matches):
        if match.group(1) != name:
            continue
        end = matches[index + 1].start() if index + 1 < len(matches) else len(text)
        return text[match.start():end].rstrip() + "\n"
    return None


def extract_assembly_function(text: str, name: str) -> str | None:
    start = re.search(rf"(?m)^\s*\.ent\s+{re.escape(name)}\s*$", text)
    if not start:
        return None
    end = re.search(rf"(?m)^\s*\.end\s+{re.escape(name)}\s*$", text[start.end():])
    if not end:
        return None
    stop = start.end() + end.end()
    return text[start.start():stop].rstrip() + "\n"


def artifact_record(path: Path) -> dict[str, object]:
    return {"name": path.name, "size": path.stat().st_size, "sha256": sha256(path)}


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("source", type=Path)
    parser.add_argument("--repo", type=Path, default=DEFAULT_REPO)
    parser.add_argument("--version", choices=("dds1", "dds2"), default="dds1")
    parser.add_argument("--as-unit", help="canonical src/<version>/... path for a scratch source")
    parser.add_argument("--function", help="extract this function from every RTL dump and assembly")
    parser.add_argument("--out-dir", type=Path)
    parser.add_argument("--allow-aslr", action="store_true",
                        help="set DDS_ALLOW_ASLR=1 when setarch is unavailable")
    parser.add_argument("--strict-assemble", action="store_true",
                        help="fail when ee-as cannot build the final object")
    parser.add_argument("--allow-inside-repo", action="store_true",
                        help="permit an output directory inside the target checkout")
    parser.add_argument("--cflag", action="append", default=[], help="extra cc1 flag; repeatable")
    parser.add_argument("--replace", action="append", default=[], metavar="OLD=NEW",
                        help="controlled token rename in a temporary input; repeatable")
    args = parser.parse_args()

    repo = args.repo.resolve()
    source = args.source.resolve()
    wrapper = repo / "tools/cc.sh"
    compiler = repo / "tools/compilers/ee-gcc2.96/lib/gcc-lib/ee/2.96-ee-001003-1/cc1"
    if not source.is_file():
        parser.error(f"source not found: {source}")
    if not wrapper.is_file() or not compiler.is_file():
        parser.error(f"configured DDS toolchain not found under {repo}")

    if args.out_dir:
        out_dir = args.out_dir.resolve()
        if out_dir.is_dir() and any(out_dir.iterdir()):
            parser.error(f"output directory is not empty: {out_dir}")
        out_dir.mkdir(parents=True, exist_ok=True)
    else:
        out_dir = Path(tempfile.mkdtemp(prefix="dds-ee-gcc-probe."))
    if inside(out_dir, repo) and not args.allow_inside_repo:
        parser.error("refusing to put compiler dumps inside the checkout; use /tmp or --allow-inside-repo")

    replacements: list[tuple[str, str, int]] = []
    compiled_source = source
    as_unit = args.as_unit
    if args.replace:
        text = source.read_text()
        for item in args.replace:
            if "=" not in item:
                parser.error(f"replacement must be OLD=NEW: {item}")
            old, new = item.split("=", 1)
            if not old or not new:
                parser.error(f"replacement must have non-empty tokens: {item}")
            pattern = re.compile(rf"\b{re.escape(old)}\b")
            text, count = pattern.subn(new, text)
            if not count:
                parser.error(f"replacement token not found: {old}")
            replacements.append((old, new, count))
        compiled_source = out_dir / "input.c"
        compiled_source.write_text(text)
        if not as_unit:
            try:
                as_unit = source.relative_to(repo).as_posix()
            except ValueError:
                parser.error("--replace requires --as-unit when source is outside the checkout")

    dump_base = out_dir / "rtl"
    assembly = out_dir / "candidate.s"
    obj = out_dir / "candidate.o"
    command = [str(wrapper), "-da", "-dumpbase", str(dump_base), *args.cflag,
               str(compiled_source), "-o", str(obj)]
    env = os.environ.copy()
    env["DDS_VERSION"] = args.version
    env["DDS_KEEP_S"] = str(assembly)
    if as_unit:
        env["DDS_AS_UNIT"] = as_unit
    if args.allow_aslr:
        env["DDS_ALLOW_ASLR"] = "1"

    result = subprocess.run(command, cwd=repo, env=env, text=True, capture_output=True)
    dumps = sorted(out_dir.glob("rtl.[0-9][0-9].*"))
    cc1_succeeded = assembly.is_file() and bool(dumps)

    extracted: list[Path] = []
    if args.function:
        function_dir = out_dir / "functions" / args.function
        function_dir.mkdir(parents=True, exist_ok=True)
        for dump in dumps:
            body = extract_dump_function(dump.read_text(errors="replace"), args.function)
            if body is not None:
                target = function_dir / dump.name.removeprefix("rtl.")
                target.write_text(body)
                extracted.append(target)
        if assembly.is_file():
            body = extract_assembly_function(assembly.read_text(errors="replace"), args.function)
            if body is not None:
                target = function_dir / "final.s"
                target.write_text(body)
                extracted.append(target)

    manifest = {
        "schema": 1,
        "created_utc": datetime.now(timezone.utc).isoformat(),
        "repo": str(repo),
        "source": str(source),
        "source_sha256": sha256(source),
        "version": args.version,
        "as_unit": as_unit,
        "compiled_source": str(compiled_source),
        "replacements": [
            {"old": old, "new": new, "count": count}
            for old, new, count in replacements
        ],
        "function": args.function,
        "compiler": {"path": str(compiler), "sha256": sha256(compiler)},
        "command": command,
        "environment": {
            key: env[key]
            for key in ("DDS_VERSION", "DDS_AS_UNIT", "DDS_ALLOW_ASLR", "DDS_KEEP_S")
            if key in env
        },
        "wrapper_returncode": result.returncode,
        "cc1_succeeded": cc1_succeeded,
        "assembled": obj.is_file(),
        "artifacts": [artifact_record(path) for path in ([assembly] if assembly.exists() else []) + dumps],
        "extracted": [str(path.relative_to(out_dir)) for path in extracted],
        "stdout": result.stdout,
        "stderr": result.stderr,
    }
    manifest_path = out_dir / "manifest.json"
    manifest_path.write_text(json.dumps(manifest, indent=2, sort_keys=True) + "\n")

    print(f"probe       {out_dir}")
    print(f"cc1         {'ok' if cc1_succeeded else 'failed'}")
    print(f"assembler   {'ok' if obj.is_file() else 'failed/skipped'}")
    print(f"rtl dumps   {len(dumps)}")
    if args.function:
        print(f"extracted   {len(extracted)} artifacts for {args.function}")
    print(f"manifest    {manifest_path}")

    if not cc1_succeeded:
        sys.stderr.write(result.stderr[-4000:])
        return result.returncode or 1
    if args.strict_assemble and not obj.is_file():
        sys.stderr.write(result.stderr[-4000:])
        return result.returncode or 1
    if not obj.is_file() and result.stderr:
        print("note        cc1 artifacts retained although final assembly failed; see manifest")
    return 0


if __name__ == "__main__":
    sys.exit(main())
