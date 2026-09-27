#!/usr/bin/env python3
"""Generate build.ninja (and objdiff.json) for the Digital Devil Saga decomp.

    python configure.py              # every version with an extracted ELF
    python configure.py dds1         # one version
    python configure.py --no-split   # reuse the existing split (fast)
    ninja                            # build + SHA-1 check every configured version

Pipeline per version (see README.md):
  splat              config/<v>/<serial>.yaml -> asm/<v>/, assets/<v>/, build/<v>/<serial>.ld
  asm (.s)           mips-ps2-decompals-as  (splat output is explicit, noreorder code)
  C (.c)             ee-gcc 2.96 cc1 -O2 -> original ee-as; INCLUDE_ASM bodies are
                     rewritten for ee-as by tools/eeas_compat.py
  link               mips-ps2-decompals-ld with splat's script -> objcopy -O binary
  check              sha1sum -c config/<v>/checksum.sha1 (the output IS the retail ELF file)

Every version is linked as a byte-identical copy of the retail executable, so any
edit that changes code or data shows up as a checksum failure.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
import re
import shutil
import subprocess
import sys
from pathlib import Path

import ninja_syntax

ROOT = Path(__file__).resolve().parent
VERSIONS = json.loads((ROOT / "config" / "versions.json").read_text())

TOOLS = Path("tools")
BIN = TOOLS / "bin"
EEGCC = TOOLS / "compilers" / "ee-gcc2.96"
CC1 = EEGCC / "lib" / "gcc-lib" / "ee" / "2.96-ee-001003-1" / "cc1"
EE_AS = EEGCC / "ee" / "bin" / "as"
AS = BIN / "mips-ps2-decompals-as"
LD = BIN / "mips-ps2-decompals-ld"
OBJCOPY = BIN / "mips-ps2-decompals-objcopy"
OBJDIFF = BIN / "objdiff-cli"

# What the ee-gcc 2.96 driver passes to cc1 (`ee-gcc -v`); cc1 preprocesses itself.
CC1_DEFINES = (
    "-D__GNUC__=2 -D__GNUC_MINOR__=96 -D__GNUC_PATCHLEVEL__=0 "
    "-Dmips -DMIPSEL -DR5900 -D_mips -D_MIPSEL -D_R5900 -D__ee__ "
    "-D__mips__ -D__MIPSEL__ -D__R5900__ -D__mips -D__MIPSEL -D__R5900 -D__OPTIMIZE__ "
    "-D__LANGUAGE_C -D_LANGUAGE_C -DLANGUAGE_C "
    "'-D__SIZE_TYPE__=unsigned int' '-D__PTRDIFF_TYPE__=int' -D__LONG_MAX__=9223372036854775807L "
    "-U__mips -D__mips=3 -D__mips64 -D__mips_eabi -D__mips_single_float"
)
# Retail game code: -O2, default small-data limit (-G8: .sdata/.sbss/.lit4 are $gp-relative).
CC1_FLAGS = "-quiet -O2"
INCLUDES = "-Iinclude -Isrc"
AS_FLAGS = "-EL -march=r5900 -mabi=eabi -G8 -Iinclude"
EE_AS_FLAGS = "-EL -G8 -Iinclude"


def i386_prefix() -> str:
    """cc1/ee-as are 32-bit i386 binaries. Without a system /lib/ld-linux.so.2,
    point DDS_I386_LIBDIR at a directory holding ld-linux.so.2 + libc.so.6."""
    libdir = os.environ.get("DDS_I386_LIBDIR")
    if libdir:
        libdir = str(Path(libdir).expanduser().resolve())
        return f"{libdir}/ld-linux.so.2 --library-path {libdir} "
    if Path("/lib/ld-linux.so.2").exists():
        return ""
    sys.exit("32-bit loader missing: install 32-bit glibc (e.g. glibc.i686 / libc6:i386) "
             "or set DDS_I386_LIBDIR=<dir with ld-linux.so.2 and libc.so.6>")


def run_splat(version: str, yaml: Path, force: bool) -> None:
    """Split when the config inputs changed since the last successful split."""
    inputs = [yaml, ROOT / "config" / version / "symbol_addrs.txt", ROOT / "config" / version / "reloc_addrs.txt"]
    digest = hashlib.sha1(b"".join(p.read_bytes() for p in inputs if p.exists())).hexdigest()
    stamp = ROOT / "build" / version / ".splat_stamp"
    if not force and stamp.exists() and stamp.read_text() == digest and (ROOT / "asm" / version).exists():
        return
    print(f"splat: {version}")
    subprocess.run([sys.executable, "-m", "splat", "split", str(yaml)], cwd=ROOT, check=True)
    subprocess.run([sys.executable, "tools/resolve_jtbl_targets.py", f"asm/{version}"], cwd=ROOT, check=True)
    subprocess.run([sys.executable, "tools/include_rodata.py", version], cwd=ROOT, check=True)
    align_bss(ROOT / "build" / version / f"{VERSIONS[version]['serial']}.ld")
    stamp.parent.mkdir(parents=True, exist_ok=True)
    stamp.write_text(digest)


def align_bss(ld_script: Path) -> None:
    """Sony's app.cmd starts .sbss and .bss on 128-byte boundaries; splat's
    script does not, so insert the alignment the retail layout depends on."""
    text = ld_script.read_text()
    for sym in ("main_SBSS_START", "main_BSS_START"):
        text = re.sub(rf"^(\s*)({sym} = \.;)", r"\1. = ALIGN(128);\n\1\2", text, count=1, flags=re.M)
    ld_script.write_text(text)

LD_OBJECT = re.compile(r"^\s*(build/\S+\.o)\(")


def linker_objects(ld_script: Path) -> list[Path]:
    seen: dict[Path, None] = {}
    for line in ld_script.read_text().splitlines():
        m = LD_OBJECT.match(line)
        if m:
            seen.setdefault(Path(m.group(1)), None)
    return list(seen)


INCLUDE_ASM = re.compile(r'^\s*INCLUDE_(?:ASM|RODATA)\(\s*[^,]+,\s*"([^"]+)"\s*,\s*(\w+)\s*\)', re.M)


def source_for(obj: Path, version: str) -> tuple[str, Path]:
    """Map a linker-script object back to the input splat or the tree provides."""
    rel = obj.relative_to(Path("build") / version)
    stem = Path(str(rel)[: -len(".o")])
    for kind, suffix in (("c", ".c"), ("s", ".s")):
        candidate = Path(f"{stem}{suffix}")
        if (ROOT / candidate).exists():
            return kind, candidate
    if rel.parts[0] == "assets":
        return "bin", Path(f"{stem}.bin")
    raise SystemExit(f"{obj}: no source found (expected {stem}.c or {stem}.s); re-run splat")


def write_ninja(versions: list[str], args: argparse.Namespace) -> dict[str, list[dict]]:
    prefix = i386_prefix()
    n = ninja_syntax.Writer(open(ROOT / "build.ninja", "w"), width=120)
    n.comment("Generated by configure.py; do not edit.")
    n.variable("ninja_required_version", "1.10")
    n.newline()

    n.rule("as", f"{AS} {AS_FLAGS} -o $out $in", description="as $in")
    n.rule("bin", f"{OBJCOPY} -I binary -O elf32-littlemips -B mips:5900 $in $out", description="bin $in")
    n.rule("eeasm", f"{sys.executable} tools/eeas_compat.py $in $out", description="ee-as compat $in")
    n.rule(
        "cc",
        f"cpp -MM -MG -MF $out.d -MT $out -nostdinc {INCLUDES} $cdefs $in && "
        f"{prefix}{CC1} {CC1_DEFINES} {INCLUDES} $cdefs {CC1_FLAGS} $in -o $out.s && "
        f"{prefix}{EE_AS} {EE_AS_FLAGS} -o $out $out.s",
        description="cc $in",
        depfile="$out.d",
        deps="gcc",
    )
    n.rule(
        "ld",
        f"{LD} -EL -T $ldscript -T $undef_syms -T $undef_funcs -Map $map --no-check-sections -o $out",
        description="ld $out",
    )
    n.rule("objcopy", f"{OBJCOPY} -O binary $in $out", description="objcopy $out")
    n.rule("check", "sha1sum --quiet -c $in && touch $out", description="check $in")
    n.rule("configure", f"{sys.executable} configure.py $args", description="configure", generator=True)
    n.newline()

    units: dict[str, list[dict]] = {}
    defaults = []
    for version in versions:
        serial = VERSIONS[version]["serial"]
        yaml = Path("config") / version / f"{serial}.yaml"
        if not args.no_split:
            run_splat(version, ROOT / yaml, args.force_split)
        ld_script = Path("build") / version / f"{serial}.ld"
        objects = linker_objects(ROOT / ld_script)
        units[version] = []
        n.comment(f"--- {version} ({serial}) ---")
        for obj in objects:
            kind, src = source_for(obj, version)
            if kind == "s":
                n.build(str(obj), "as", str(src), implicit=["include/macro.inc"])
                units[version].append({"name": str(src.with_suffix("")), "target": str(obj), "base": None})
            elif kind == "bin":
                n.build(str(obj), "bin", str(src))
            else:
                text = (ROOT / src).read_text(errors="replace")
                eeasm = []
                nonmatchings = Path("asm") / version / "nonmatchings"
                for folder, name in INCLUDE_ASM.findall(text):
                    asm = nonmatchings / folder / f"{name}.s"
                    out = Path("build") / "eeasm" / asm
                    n.build(str(out), "eeasm", str(asm), implicit=["tools/eeas_compat.py"])
                    eeasm.append(str(out))
                cdefs = f"'-DASM_ROOT=\"build/eeasm/{nonmatchings}/\"' -DVERSION_{version.upper()}"
                n.build(str(obj), "cc", str(src), implicit=eeasm + ["include/macro.inc"], variables={"cdefs": cdefs})
                # objdiff base: the same unit without its INCLUDE_ASM fallbacks, so only C counts.
                base = Path("build") / version / "base" / src.with_suffix(".o")
                n.build(str(base), "cc", str(src), variables={"cdefs": f"{cdefs} -DSKIP_ASM"})
                # objdiff target: splat's full disassembly of this C unit, assembled as-is.
                full = Path("asm") / version / src.relative_to(Path("src") / version).with_suffix(".s")
                target = Path("build") / version / "target" / full.with_suffix(".o")
                if (ROOT / full).exists():
                    n.build(str(target), "as", str(full), implicit=["include/macro.inc"])
                units[version].append({"name": str(src.with_suffix("")), "target": str(target), "base": str(base)})
        elf = Path("build") / version / f"{serial}.elf"
        image = Path("build") / version / serial
        n.build(
            str(elf), "ld", [str(o) for o in objects],
            implicit=[str(ld_script), f"config/{version}/undefined_syms_auto.txt", f"config/{version}/undefined_funcs_auto.txt"],
            variables={
                "ldscript": str(ld_script),
                "undef_syms": f"config/{version}/undefined_syms_auto.txt",
                "undef_funcs": f"config/{version}/undefined_funcs_auto.txt",
                "map": str(elf.with_suffix(".map")),
            },
        )
        n.build(str(image), "objcopy", str(elf))
        stamp = Path("build") / version / f"{serial}.ok"
        n.build(str(stamp), "check", f"config/{version}/checksum.sha1", implicit=[str(image)])
        n.build(version, "phony", str(stamp))
        defaults.append(version)
        (ROOT / "config" / version / "checksum.sha1").write_text(f"{VERSIONS[version]['elf_sha1']}  {image}\n")
        n.newline()

    objdiff_objects = sorted({p for rows in units.values() for row in rows
                              for p in (row["target"], row["base"]) if p})
    n.rule("report", f"{OBJDIFF} report generate -o $out", description="objdiff report")
    n.build("objdiff", "phony", objdiff_objects)
    n.build("report.json", "report", implicit=objdiff_objects + ["objdiff.json"])
    n.build("report", "phony", "report.json")

    configure_inputs = ["configure.py", "config/versions.json"] + [
        f"config/{v}/{VERSIONS[v]['serial']}.yaml" for v in versions] + [f"config/{v}/symbol_addrs.txt" for v in versions]
    n.build("build.ninja", "configure", implicit=configure_inputs,
            variables={"args": " ".join(sys.argv[1:])})
    n.default(defaults)
    n.close()
    return units


def write_objdiff(units: dict[str, list[dict]]) -> None:
    config = {
        "$schema": "https://raw.githubusercontent.com/encounter/objdiff/main/config.schema.json",
        "custom_make": "ninja",
        "build_target": False,
        "build_base": True,
        "watch_patterns": ["*.c", "*.h", "*.s", "*.inc"],
        "units": [],
    }
    for version, rows in units.items():
        for row in rows:
            unit = {
                "name": "/".join(Path(row["name"]).parts[1:]),  # drop asm/ or src/: "<version>/<unit>"
                "target_path": row["target"],
                "metadata": {"progress_categories": [version, "sdk" if "/sdk/" in row["name"] else "game"]},
            }
            if row["base"]:
                unit["base_path"] = row["base"]
            config["units"].append(unit)
    config["progress_categories"] = [{"id": v, "name": VERSIONS[v]["title"]} for v in units] + [
        {"id": "game", "name": "Atlus game/engine"}, {"id": "sdk", "name": "Sony SDK / C runtime"}]
    (ROOT / "objdiff.json").write_text(json.dumps(config, indent=2) + "\n")


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("versions", nargs="*", help=f"any of {', '.join(VERSIONS)} (default: all extracted)")
    ap.add_argument("--no-split", action="store_true", help="do not run splat")
    ap.add_argument("--force-split", action="store_true", help="run splat even if its inputs are unchanged")
    ap.add_argument("--clean", action="store_true", help="remove generated asm/, assets/, build/ first")
    args = ap.parse_args()

    versions = args.versions or [v for v in VERSIONS if (ROOT / "orig" / v / VERSIONS[v]["serial"]).exists()]
    for v in versions:
        if v not in VERSIONS:
            ap.error(f"unknown version {v}")
        if not (ROOT / "orig" / v / VERSIONS[v]["serial"]).exists():
            ap.error(f"{v}: orig/{v}/{VERSIONS[v]['serial']} missing; run tools/extract.py")
    if not versions:
        ap.error("no extracted versions; run tools/extract.py")
    for tool in (CC1, EE_AS, AS, LD, OBJCOPY):
        if not (ROOT / tool).exists():
            ap.error(f"{tool} missing; run tools/download_tools.py")
    if args.clean:
        for d in ("asm", "assets", "build"):
            shutil.rmtree(ROOT / d, ignore_errors=True)

    os.chdir(ROOT)
    units = write_ninja(versions, args)
    write_objdiff(units)
    print(f"configured {', '.join(versions)}; run ninja")


if __name__ == "__main__":
    main()
