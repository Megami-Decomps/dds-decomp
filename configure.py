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
  FLW0 (.bfasm)      tools/flw0.py assemble -> build/<v>/scripts/, then SHA-1 check
  INF (.infasm)      tools/inf.py assemble -> build/<v>/data/field/, then SHA-1 check
  WAP (.wapasm)      tools/wap.py assemble -> build/<v>/data/field/, then SHA-1 check
  NPL (.nplasm)      tools/npl.py assemble -> build/<v>/data/field/, then SHA-1 check
  SKY (.skyasm)      tools/sky.py assemble -> build/<v>/data/field/, then SHA-1 check
  FLD2 (.fldasm)     tools/fld.py assemble -> build/<v>/data/field/, then SHA-1 check
  LB (.lbasm)        tools/lb.py assemble over an extracted base -> build/<v>/data/field/
  battle (.tblasm)   tools/battle_tbl.py assemble -> build/<v>/data/battle/, then SHA-1 check

Every version is linked as a byte-identical copy of the retail executable, so any
edit that changes code or data shows up as a checksum failure.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
import platform
import re
import shlex
import shutil
import subprocess
import sys
from pathlib import Path

import ninja_syntax
from tools.ee_gcc_aslr import marker_is_valid

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
# Retail was assembled with -g: GNU as then leaves `.set reorder` code where cc1
# put it, so every delay slot cc1 did not fill keeps its `nop` (soft-double and
# libm call argument moves, mtc1/lwc1/cvt dependents, indexed la/memory macros).
EE_AS_FLAGS = "-EL -G8 -g -Iinclude"


def i386_prefix() -> str:
    """cc1/ee-as are 32-bit i386 binaries. Without a system /lib/ld-linux.so.2,
    point DDS_I386_LIBDIR at a directory holding ld-linux.so.2 + libc.so.6."""
    return no_aslr_prefix() + i386_loader()


def no_aslr_prefix() -> str:
    """Fallback for an unpatched, manually installed ee-gcc 2.96.

    tools/download_tools.py patches the compiler's GGC arena, allowing normal
    ASLR.  Keep setarch support for an otherwise-compatible manual toolchain.
    """
    if marker_is_valid(ROOT / CC1):
        return ""
    if os.environ.get("DDS_ALLOW_ASLR") or not shutil.which("setarch"):
        return ""
    return f"setarch {platform.machine()} -R "


def i386_loader() -> str:
    # The pinned glibc from tools/download_tools.py comes first: ee-gcc 2.96's
    # output depends on heap layout, so the system libc is only a fallback.
    libdir = os.environ.get("DDS_I386_LIBDIR")
    if not libdir and (ROOT / "tools/glibc32/libc.so.6").exists():
        return "tools/glibc32/ld-linux.so.2 --library-path tools/glibc32 "  # ninja runs in ROOT
    if libdir:
        libdir = shlex.quote(str(Path(libdir).expanduser().resolve()))
        return f"{libdir}/ld-linux.so.2 --library-path {libdir} "
    if Path("/lib/ld-linux.so.2").exists():
        return ""
    sys.exit("32-bit loader missing: install 32-bit glibc (e.g. glibc.i686 / libc6:i386) "
             "or set DDS_I386_LIBDIR=<dir with ld-linux.so.2 and libc.so.6>")


def unit_cflags(version: str) -> dict[str, str]:
    """Per-unit cc1 flags from config/<v>/cflags.txt (`<dir>/<unit>  <flags>  # evidence`).

    Some original files were built with different options; each entry records the
    evidence that proves it."""
    path = ROOT / "config" / version / "cflags.txt"
    out = {}
    if path.exists():
        for line in path.read_text().splitlines():
            line = line.split("#", 1)[0].strip()
            if line:
                unit, flags = line.split(None, 1)
                out[unit] = flags
    return out


def run_splat(version: str, yaml: Path, force: bool) -> None:
    """Split when the config inputs changed since the last successful split."""
    inputs = [yaml, ROOT / "config" / version / "symbol_addrs.txt", ROOT / "config" / version / "reloc_addrs.txt"]
    digest = hashlib.sha1(b"".join(p.read_bytes() for p in inputs if p.exists())).hexdigest()
    stamp = ROOT / "build" / version / ".splat_stamp"
    if not force and stamp.exists() and stamp.read_text() == digest and (ROOT / "asm" / version).exists():
        return
    print(f"splat: {version}")
    # splat never deletes files: a unit or rodata split that moved leaves stale
    # asm behind (and include_rodata.py would include it twice).
    shutil.rmtree(ROOT / "asm" / version, ignore_errors=True)
    subprocess.run([sys.executable, "-m", "splat", "split", str(yaml)], cwd=ROOT, check=True)
    subprocess.run([sys.executable, "tools/resolve_jtbl_targets.py", f"asm/{version}"], cwd=ROOT, check=True)
    subprocess.run([sys.executable, "tools/include_rodata.py", version], cwd=ROOT, check=True)
    subprocess.run([sys.executable, "tools/include_sdata.py", version], cwd=ROOT, check=True)
    align_bss(ROOT / "build" / version / f"{VERSIONS[version]['serial']}.ld")
    provide_data_symbols(version)
    stamp.parent.mkdir(parents=True, exist_ok=True)
    stamp.write_text(digest)

def provide_data_symbols(version: str) -> None:
    """Give every D_XXXXXXXX the asm refers to its retail address as a fallback.

    Per-unit .lit4/.rodata is compiled by C objects as local labels, so a .data
    word pointing at such a constant has no global definition. PROVIDE only
    defines a symbol no object defines."""
    names = set()
    for path in (ROOT / "asm" / version).rglob("*.s"):
        names.update(re.findall(r"\bD_[0-9A-F]{8}\b", path.read_text()))
    path = ROOT / "config" / version / "undefined_syms_auto.txt"
    text = path.read_text() if path.exists() else ""
    path.write_text(text + "".join(f"PROVIDE({n} = 0x{n[2:]});\n" for n in sorted(names)))


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


INCLUDE_DIRECTIVE = re.compile(
    r'\bINCLUDE_(ASM|RODATA|SDATA)\b'
    r'(?:\s|/\*.*?\*/|//[^\r\n]*(?:\r?\n|$))*'
    r'\(\s*[^,]+,\s*"([^"]+)"\s*,\s*(\w+)\s*\)',
    re.S,
)
INCLUDE_TOKEN = re.compile(r'\bINCLUDE_(ASM|RODATA|SDATA)\b')
C_NON_CODE = re.compile(
    r'//[^\r\n]*|/\*.*?\*/|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'',
    re.S,
)


def splice_c_lines(text: str) -> tuple[str, list[int]]:
    """Apply C phase-2 line splicing and map output offsets to the source."""

    result = []
    source_offsets = []
    index = 0
    while index < len(text):
        if text[index] == "\\":
            if text.startswith("\r\n", index + 1):
                index += 3
                continue
            if index + 1 < len(text) and text[index + 1] in "\r\n":
                index += 2
                continue
        result.append(text[index])
        source_offsets.append(index)
        index += 1
    source_offsets.append(len(text))
    return "".join(result), source_offsets


def mask_c_non_code(text: str) -> str:
    """Blank comments and literals while preserving token offsets and lines."""

    return C_NON_CODE.sub(
        lambda match: "".join(
            character if character in "\r\n" else " "
            for character in match.group()
        ),
        text,
    )


def include_directives(
    text: str, *, context: str | None = None
) -> list[tuple[str, str, str]]:
    spliced, source_offsets = splice_c_lines(text)
    code = mask_c_non_code(spliced)
    matches = [
        match
        for match in INCLUDE_DIRECTIVE.finditer(spliced)
        if code.startswith("INCLUDE_", match.start())
    ]
    if context is not None:
        matched_starts = {match.start() for match in matches}
        unmatched = [
            match for match in INCLUDE_TOKEN.finditer(code)
            if match.start() not in matched_starts
        ]
        if unmatched:
            source_offset = source_offsets[unmatched[0].start()]
            line = text.count("\n", 0, source_offset) + 1
            raise SystemExit(
                f"{context}: unsupported INCLUDE_{unmatched[0].group(1)} "
                f"syntax at line {line}"
            )
    return [match.groups() for match in matches]


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


def objdiff_progress_category(version: str, name: str) -> str:
    # splat exposes the binary VU1 program as a text unit, not EE code.
    if name == f"asm/{version}/data/vutext":
        return "vu1"
    return "sdk" if "/sdk/" in name else "game"


def write_objdiff_reports(n, units: dict[str, list[dict]]) -> None:
    objdiff_objects = sorted({p for rows in units.values() for row in rows
                              for p in (row["target"], row["base"]) if p})
    n.rule("report", f"{OBJDIFF} report generate -p $project -o $out", description="objdiff report $out")
    n.build("objdiff", "phony", objdiff_objects)
    n.build("report.json", "report", implicit=objdiff_objects + ["objdiff.json"], variables={"project": "."})
    reports = []
    for version, rows in units.items():
        game_rows = [row for row in rows if objdiff_progress_category(version, row["name"]) == "game"]
        # Keep the full-binary audit separate from the primary game-code report.
        # objdiff computes every measure from each project's actual unit list.
        for selected, project, out in (
            (rows, f"build/{version}", f"build/{version}/report.all.json"),
            (game_rows, f"build/{version}/progress", f"build/{version}/report.json"),
        ):
            objs = sorted({p for row in selected for p in (row["target"], row["base"]) if p})
            n.build(out, "report", implicit=objs + [f"{project}/objdiff.json"], variables={"project": project})
            reports.append(out)
    n.build("report", "phony", ["report.json"] + reports)


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
        f"{prefix}{CC1} {CC1_DEFINES} {INCLUDES} $cdefs {CC1_FLAGS} $cflags $in -o $out.s && "
        f"{prefix}{EE_AS} {EE_AS_FLAGS} -o $out $out.s",
        description="cc $in",
        depfile="$out.d",
        deps="gcc",
    )
    n.rule(
        "dev_cc",
        f"mkdir -p $outdir && "
        f"cpp -MM -MG -MF $out.d -MT $out -nostdinc {INCLUDES} $cdefs $in && "
        f"{prefix}{CC1} {CC1_DEFINES} {INCLUDES} $cdefs {CC1_FLAGS} $cflags -G0 $in -o $out.s && "
        f"{prefix}{EE_AS} {EE_AS_FLAGS} -G0 -o $out $out.s",
        description="dev cc $in",
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
    n.rule(
        "dev_link_script",
        f"{sys.executable} tools/dev_elf.py link-script $descriptor $in $out",
        description="dev linker script $out",
    )
    n.rule(
        "dev_ld",
        f"{LD} -EL --emit-relocs -T $ldscript -T $undef_syms -T $undef_funcs "
        "$wraps -Map $map --no-check-sections -o $out",
        description="dev link $out",
    )
    n.rule(
        "dev_finalize",
        f"{sys.executable} tools/dev_elf.py finalize "
        "$descriptor $retail $linked $out --reloc-elf $relocelf "
        "--retail-symbol-elf $retailsym",
        description="dev ELF $out",
    )
    n.rule(
        "flw0",
        f"mkdir -p $outdir && {sys.executable} tools/flw0.py assemble $in $out",
        description="flw0 $in",
    )
    n.rule(
        "inf",
        f"mkdir -p $outdir && {sys.executable} tools/inf.py assemble --messages $messages $in $out",
        description="inf $in",
    )
    n.rule(
        "wap",
        f"mkdir -p $outdir && {sys.executable} tools/wap.py assemble "
        "--scripts $scripts --interactions $interactions $in $out",
        description="wap $in",
    )
    n.rule(
        "npl",
        f"mkdir -p $outdir && {sys.executable} tools/npl.py assemble $in $out",
        description="npl $in",
    )
    n.rule(
        "sky",
        f"mkdir -p $outdir && {sys.executable} tools/sky.py assemble $in $out",
        description="sky $in",
    )
    n.rule(
        "fld2",
        f"mkdir -p $outdir && {sys.executable} tools/fld.py assemble "
        "$links $in $out",
        description="fld2 $in",
    )
    n.rule(
        "fld1",
        f"mkdir -p $outdir && {sys.executable} tools/fld.py assemble $in $out",
        description="fld1 $in",
    )
    n.rule(
        "field_world",
        f"mkdir -p $outdir && {sys.executable} tools/field_world.py "
        "--manifest $manifest --encounters $encounters && touch $out",
        description="link field world $out",
    )
    n.rule(
        "lb",
        f"mkdir -p $outdir && {sys.executable} tools/lb.py assemble "
        "--resources $resources $in $base $out",
        description="lb $in",
    )
    n.rule(
        "battle_tbl",
        f"mkdir -p $outdir && {sys.executable} tools/battle_tbl.py assemble $context $in $out",
        description="battle table $in",
    )
    n.rule("configure", f"{sys.executable} configure.py $args", description="configure", generator=True)
    n.newline()

    units: dict[str, list[dict]] = {}
    defaults = []
    dev_configure_inputs: set[str] = set()
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
                for _, folder, name in include_directives(text):
                    asm = nonmatchings / folder / f"{name}.s"
                    out = Path("build") / "eeasm" / asm
                    n.build(str(out), "eeasm", str(asm), implicit=["tools/eeas_compat.py"])
                    eeasm.append(str(out))
                cdefs = f"'-DASM_ROOT=\"build/eeasm/{nonmatchings}/\"' -DVERSION_{version.upper()}"
                flags = unit_cflags(version).get(src.relative_to(Path("src") / version).with_suffix("").as_posix(), "")
                n.build(str(obj), "cc", str(src), implicit=eeasm + ["include/macro.inc", f"config/{version}/cflags.txt"],
                        variables={"cdefs": cdefs, "cflags": flags})
                # objdiff base: the same unit without its INCLUDE_ASM fallbacks, so only C counts.
                base = Path("build") / version / "base" / src.with_suffix(".o")
                n.build(str(base), "cc", str(src), variables={"cdefs": f"{cdefs} -DSKIP_ASM", "cflags": flags})
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
        version_outputs = [str(stamp)]

        dev_descriptor = Path("config") / version / "devbuild.json"
        if (ROOT / dev_descriptor).exists():
            dev_configure_inputs.add(f"config/{version}/cflags.txt")
            dev_spec = json.loads((ROOT / dev_descriptor).read_text())
            replacement_objects = []
            replacement_by_retail = {}
            for index, replacement in enumerate(dev_spec.get("replacements", [])):
                obj = replacement.get("object")
                retail_obj = replacement.get("retail_object")
                if not all(isinstance(value, str) and value for value in (obj, retail_obj)):
                    raise SystemExit(
                        f"{dev_descriptor}: replacements[{index}] needs object and "
                        "retail_object"
                    )
                if retail_obj in replacement_by_retail:
                    raise SystemExit(
                        f"{dev_descriptor}: replacement for {retail_obj} is declared twice"
                    )
                if obj in replacement_objects:
                    raise SystemExit(
                        f"{dev_descriptor}: replacement output {obj} is declared twice"
                    )
                if Path(retail_obj) not in objects:
                    raise SystemExit(
                        f"{dev_descriptor}: replacement retail object {retail_obj} "
                        "is not in the retail link"
                    )
                kind, source = source_for(Path(retail_obj), version)
                if kind != "c":
                    raise SystemExit(
                        f"{dev_descriptor}: replacement retail object {retail_obj} "
                        "must come from C"
                    )
                source_text = (ROOT / source).read_text(errors="replace")
                includes = include_directives(
                    source_text,
                    context=f"{dev_descriptor}: replacement source {source}",
                )
                fallback_symbols = replacement.get("fallback_symbols", [])
                if not isinstance(fallback_symbols, list) or not all(
                    isinstance(symbol, str) and symbol for symbol in fallback_symbols
                ):
                    raise SystemExit(
                        f"{dev_descriptor}: replacements[{index}].fallback_symbols "
                        "must be a list of symbol names"
                    )
                included_symbols = [name for kind, _, name in includes if kind == "ASM"]
                unsupported = [
                    f"INCLUDE_{kind}({name})"
                    for kind, _, name in includes
                    if kind != "ASM"
                ]
                if unsupported:
                    raise SystemExit(
                        f"{dev_descriptor}: replacement source {source} has unsupported "
                        f"fallback {unsupported[0]}"
                    )
                if included_symbols != fallback_symbols:
                    raise SystemExit(
                        f"{dev_descriptor}: replacements[{index}].fallback_symbols "
                        f"must exactly match {included_symbols!r} from {source}"
                    )
                eeasm = [
                    str(
                        Path("build")
                        / "eeasm"
                        / "asm"
                        / version
                        / "nonmatchings"
                        / folder
                        / f"{name}.s"
                    )
                    for _, folder, name in includes
                ]
                dev_configure_inputs.add(str(source))
                flags = unit_cflags(version).get(
                    source.relative_to(Path("src") / version)
                    .with_suffix("")
                    .as_posix(),
                    "",
                )
                replacement_by_retail[retail_obj] = obj
                replacement_objects.append(obj)
                n.build(
                    obj,
                    "dev_cc",
                    str(source),
                    implicit=[
                        "include/macro.inc",
                        f"config/{version}/cflags.txt",
                        *eeasm,
                    ],
                    variables={
                        "cdefs": (
                            f"'-DASM_ROOT=\"build/eeasm/asm/{version}/nonmatchings/\"' "
                            f"-DVERSION_{version.upper()} -DDDS_DEV_BUILD"
                        ),
                        "cflags": flags,
                        "outdir": str(Path(obj).parent),
                    },
                )
            dev_objects = []
            for index, addition in enumerate(dev_spec.get("additions", [])):
                source = addition.get("source")
                obj = addition.get("object")
                if not isinstance(source, str) or not isinstance(obj, str):
                    raise SystemExit(
                        f"{dev_descriptor}: additions[{index}] needs source and object"
                    )
                if obj in replacement_objects or obj in dev_objects:
                    raise SystemExit(
                        f"{dev_descriptor}: development object {obj} is declared twice"
                    )
                dev_objects.append(obj)
                n.build(
                    obj,
                    "dev_cc",
                    source,
                    implicit=["include/macro.inc"],
                    variables={
                        "cdefs": (
                            f"'-DASM_ROOT=\"build/eeasm/asm/{version}/nonmatchings/\"' "
                            f"-DVERSION_{version.upper()} -DDDS_DEV_BUILD"
                        ),
                        "outdir": str(Path(obj).parent),
                    },
                )
            wrap_symbols = []
            for index, redirect in enumerate(dev_spec.get("redirects", [])):
                symbol = redirect.get("symbol")
                if not isinstance(symbol, str) or not symbol:
                    raise SystemExit(
                        f"{dev_descriptor}: redirects[{index}] needs a symbol"
                    )
                wrap_symbols.append(f"--wrap={symbol}")
            dev_script = Path("build") / version / f"{serial}.dev.ld"
            dev_wrapper = Path("build") / version / f"{serial}.dev-link.elf"
            dev_raw = Path("build") / version / f"{serial}.dev.raw"
            dev_image = Path("build") / version / f"{serial}.dev"
            n.build(
                str(dev_script),
                "dev_link_script",
                str(ld_script),
                implicit=[str(dev_descriptor), "tools/dev_elf.py"],
                variables={"descriptor": str(dev_descriptor)},
            )
            n.build(
                str(dev_wrapper),
                "dev_ld",
                [
                    replacement_by_retail.get(str(obj), str(obj))
                    for obj in objects
                ]
                + dev_objects,
                implicit=[
                    str(dev_script),
                    f"config/{version}/undefined_syms_auto.txt",
                    f"config/{version}/undefined_funcs_auto.txt",
                ],
                variables={
                    "ldscript": str(dev_script),
                    "undef_syms": f"config/{version}/undefined_syms_auto.txt",
                    "undef_funcs": f"config/{version}/undefined_funcs_auto.txt",
                    "map": str(dev_wrapper.with_suffix(".map")),
                    "wraps": " ".join(wrap_symbols),
                },
            )
            n.build(str(dev_raw), "objcopy", str(dev_wrapper))
            n.build(
                str(dev_image),
                "dev_finalize",
                str(dev_raw),
                implicit=[
                    str(stamp),
                    str(elf),
                    str(dev_wrapper),
                    str(dev_descriptor),
                    *replacement_objects,
                    "tools/dev_elf.py",
                ],
                variables={
                    "descriptor": str(dev_descriptor),
                    "retail": str(image),
                    "linked": str(dev_raw),
                    "relocelf": str(dev_wrapper),
                    "retailsym": str(elf),
                },
            )
            n.build(f"{version}-dev", "phony", str(dev_image))

        all_scripts_manifest = Path("config") / version / "scripts.sha1"
        event_scripts_manifest = Path("config") / version / "event_scripts.sha1"
        if (ROOT / all_scripts_manifest).exists():
            script_manifest = all_scripts_manifest
            script_source_dir = Path("src") / version / "scripts"
            script_output_dir = Path("build") / version / "scripts"
            script_sources = sorted((ROOT / script_source_dir).rglob("*.bfasm"))
            script_stamp = script_output_dir / "scripts.ok"
        else:
            script_manifest = event_scripts_manifest
            script_source_dir = Path("src") / version / "scripts" / "event"
            script_output_dir = Path("build") / version / "scripts" / "event"
            script_sources = sorted((ROOT / script_source_dir).glob("*.bfasm"))
            script_stamp = script_output_dir / "event.ok"
        if (ROOT / script_manifest).exists():
            script_outputs = []
            for source in script_sources:
                source = source.relative_to(ROOT)
                relative = source.relative_to(script_source_dir).with_suffix(".bf")
                output = script_output_dir / relative
                n.build(
                    str(output),
                    "flw0",
                    str(source),
                    implicit=[
                        "tools/flw0.py",
                        "tools/flw0_profiles.py",
                        "tools/flw0_symbolic.py",
                    ],
                    variables={"outdir": str(output.parent)},
                )
                script_outputs.append(str(output))
            n.build(str(script_stamp), "check", str(script_manifest), implicit=script_outputs)
            n.build(f"{version}-scripts", "phony", str(script_stamp))
            version_outputs.append(str(script_stamp))

        field_data_stamps = []
        inf_manifest = Path("config") / version / "field_inf.sha1"
        if (ROOT / inf_manifest).exists():
            inf_source_dir = Path("src") / version / "data" / "field"
            inf_output_dir = Path("build") / version / "data" / "field"
            inf_sources = sorted((ROOT / inf_source_dir).glob("*.infasm"))
            inf_outputs = []
            for source in inf_sources:
                source = source.relative_to(ROOT)
                output = inf_output_dir / source.with_suffix(".inf").name
                messages = (
                    Path("src")
                    / version
                    / "scripts"
                    / "field"
                    / source.with_suffix(".bfasm").name
                )
                n.build(
                    str(output),
                    "inf",
                    str(source),
                    implicit=[
                        "tools/inf.py",
                        "tools/flw0.py",
                        "tools/flw0_symbolic.py",
                        "tools/flw0_profiles.py",
                        "tools/msg1.py",
                        "tools/dds1_msg1_chars.tsv",
                        str(messages),
                    ],
                    variables={"outdir": str(output.parent), "messages": str(messages)},
                )
                inf_outputs.append(str(output))
            inf_stamp = inf_output_dir / "field_inf.ok"
            n.build(str(inf_stamp), "check", str(inf_manifest), implicit=inf_outputs)
            version_outputs.append(str(inf_stamp))
            field_data_stamps.append(str(inf_stamp))

        wap_manifest = Path("config") / version / "field_wap.sha1"
        if (ROOT / wap_manifest).exists():
            wap_source_dir = Path("src") / version / "data" / "field"
            wap_output_dir = Path("build") / version / "data" / "field"
            wap_sources = sorted((ROOT / wap_source_dir).glob("*.wapasm"))
            wap_outputs = []
            for source in wap_sources:
                source = source.relative_to(ROOT)
                output = wap_output_dir / source.with_suffix(".wap").name
                stem = source.stem
                scripts = Path("src") / version / "scripts" / "field" / f"{stem}.bfasm"
                interactions = wap_source_dir / f"{stem}.infasm"
                n.build(
                    str(output),
                    "wap",
                    str(source),
                    implicit=[
                        "tools/wap.py",
                        "tools/flw0.py",
                        "tools/flw0_symbolic.py",
                        "tools/flw0_profiles.py",
                        "tools/msg1.py",
                        "tools/dds1_msg1_chars.tsv",
                        str(scripts),
                        str(interactions),
                    ],
                    variables={
                        "outdir": str(output.parent),
                        "scripts": str(scripts),
                        "interactions": str(interactions),
                    },
                )
                wap_outputs.append(str(output))
            wap_stamp = wap_output_dir / "field_wap.ok"
            n.build(str(wap_stamp), "check", str(wap_manifest), implicit=wap_outputs)
            version_outputs.append(str(wap_stamp))
            field_data_stamps.append(str(wap_stamp))

        for kind in ("npl", "sky"):
            environment_manifest = Path("config") / version / f"field_{kind}.sha1"
            if not (ROOT / environment_manifest).exists():
                continue
            environment_source_dir = Path("src") / version / "data" / "field"
            environment_output_dir = Path("build") / version / "data" / "field"
            environment_sources = sorted(
                (ROOT / environment_source_dir).glob(f"*.{kind}asm")
            )
            environment_outputs = []
            for source in environment_sources:
                source = source.relative_to(ROOT)
                output = environment_output_dir / source.with_suffix(f".{kind}").name
                n.build(
                    str(output),
                    kind,
                    str(source),
                    implicit=[f"tools/{kind}.py"],
                    variables={"outdir": str(output.parent)},
                )
                environment_outputs.append(str(output))
            environment_stamp = environment_output_dir / f"field_{kind}.ok"
            n.build(
                str(environment_stamp),
                "check",
                str(environment_manifest),
                implicit=environment_outputs,
            )
            version_outputs.append(str(environment_stamp))
            field_data_stamps.append(str(environment_stamp))

        fld_manifest = Path("config") / version / "field_fld2.sha1"
        if (ROOT / fld_manifest).exists():
            fld_source_dir = Path("src") / version / "data" / "field"
            fld_output_dir = Path("build") / version / "data" / "field"
            fld_sources = sorted((ROOT / fld_source_dir).glob("*.fldasm"))
            linked_stems = set(VERSIONS[version].get("field_fld2_links", ()))
            fld_outputs = []
            for source in fld_sources:
                source = source.relative_to(ROOT)
                output = fld_output_dir / source.with_suffix(".f2").name
                field_stem = source.stem.split("_", 1)[0]
                scripts = Path("src") / version / "scripts" / "field" / f"{field_stem}.bfasm"
                warps = fld_source_dir / f"{field_stem}.wapasm"
                implicit = ["tools/fld.py"]
                links = ""
                if source.stem in linked_stems:
                    if not (ROOT / scripts).exists() or not (ROOT / warps).exists():
                        raise ValueError(f"missing FLD2 link peers: {scripts}, {warps}")
                    implicit.extend((str(scripts), str(warps)))
                    links = f"--scripts {scripts} --warps {warps}"
                n.build(
                    str(output),
                    "fld2",
                    str(source),
                    implicit=implicit,
                    variables={
                        "outdir": str(output.parent),
                        "links": links,
                    },
                )
                fld_outputs.append(str(output))
            missing_links = linked_stems - {source.stem for source in fld_sources}
            if missing_links:
                raise ValueError(f"missing linked FLD2 sources: {', '.join(sorted(missing_links))}")
            fld_stamp = fld_output_dir / "field_fld2.ok"
            n.build(str(fld_stamp), "check", str(fld_manifest), implicit=fld_outputs)
            version_outputs.append(str(fld_stamp))
            field_data_stamps.append(str(fld_stamp))

            encounters = Path("build") / version / "data" / "battle" / "ENCOUNT.TBL"
            encounter_stamp = fld_output_dir / "field_encounters.ok"
            n.build(
                str(encounter_stamp),
                "field_world",
                implicit=[
                    "tools/field_world.py",
                    "tools/fld.py",
                    "tools/battle_tbl.py",
                    str(fld_manifest),
                    str(encounters),
                    *fld_outputs,
                ],
                variables={
                    "outdir": str(encounter_stamp.parent),
                    "manifest": str(fld_manifest),
                    "encounters": str(encounters),
                },
            )
            version_outputs.append(str(encounter_stamp))
            field_data_stamps.append(str(encounter_stamp))

        fld1_manifest = Path("config") / version / "field_fld1.sha1"
        if (ROOT / fld1_manifest).exists():
            fld1_source_dir = Path("src") / version / "data" / "field"
            fld1_output_dir = Path("build") / version / "data" / "field"
            fld1_sources = sorted((ROOT / fld1_source_dir).glob("*.f1asm"))
            fld1_outputs = []
            for source in fld1_sources:
                source = source.relative_to(ROOT)
                output = fld1_output_dir / source.with_suffix(".f1").name
                n.build(
                    str(output),
                    "fld1",
                    str(source),
                    implicit=["tools/fld.py"],
                    variables={"outdir": str(output.parent)},
                )
                fld1_outputs.append(str(output))
            fld1_stamp = fld1_output_dir / "field_fld1.ok"
            n.build(str(fld1_stamp), "check", str(fld1_manifest), implicit=fld1_outputs)
            version_outputs.append(str(fld1_stamp))
            field_data_stamps.append(str(fld1_stamp))

        if field_data_stamps:
            n.build(f"{version}-field-data", "phony", field_data_stamps)

        lb_manifest = Path("config") / version / "field_lb.sha1"
        if (ROOT / lb_manifest).exists():
            lb_source_dir = Path("src") / version / "data" / "field"
            lb_output_dir = Path("build") / version / "data" / "field"
            lb_sources = sorted((ROOT / lb_source_dir).glob("*.lbasm"))
            lb_outputs = []
            for source in lb_sources:
                source = source.relative_to(ROOT)
                output = lb_output_dir / source.with_suffix(".LB").name
                base = Path("orig") / version / "field" / output.name
                resource = lb_output_dir / source.with_suffix(".f2").name
                resource_f1 = lb_output_dir / source.with_suffix(".f1").name
                resources = ["tools/lb.py", str(base), str(resource)]
                if (ROOT / fld1_manifest).exists():
                    resources.append(str(resource_f1))
                n.build(
                    str(output),
                    "lb",
                    str(source),
                    implicit=resources,
                    variables={
                        "outdir": str(output.parent),
                        "base": str(base),
                        "resources": str(lb_output_dir),
                    },
                )
                lb_outputs.append(str(output))
            lb_stamp = lb_output_dir / "field_lb.ok"
            n.build(str(lb_stamp), "check", str(lb_manifest), implicit=lb_outputs)
            n.build(f"{version}-field-archives", "phony", str(lb_stamp))

        battle_manifest = Path("config") / version / "battle_tables.sha1"
        if (ROOT / battle_manifest).exists():
            battle_source_dir = Path("src") / version / "data" / "battle"
            battle_output_dir = Path("build") / version / "data" / "battle"
            battle_sources = sorted((ROOT / battle_source_dir).glob("*.tblasm"))
            battle_outputs = []
            for source in battle_sources:
                source = source.relative_to(ROOT)
                output = battle_output_dir / f"{source.stem.upper()}.TBL"
                dependencies = ["tools/battle_tbl.py"]
                context = ""
                message_source = source.with_name("msg.tblasm")
                skill_source = source.with_name("skill.tblasm")
                unit_source = source.with_name("unit.tblasm")
                message_dependencies = (
                    "tools/msg1.py",
                    "tools/dds1_msg1_chars.tsv",
                    str(source.with_name("msg-items.msgasm")),
                    str(source.with_name("msg-skills.msgasm")),
                    str(source.with_name("msg-status-help.msgasm")),
                    str(source.with_name("msg-command-help.msgasm")),
                )
                if source.stem in {"aicalc", "encount", "unit"}:
                    dependencies.extend((str(message_source), *message_dependencies))
                    context = f"--messages {message_source} --skills {skill_source}"
                if source.stem == "aicalc":
                    dependencies.extend(
                        (
                            "tools/flw0.py",
                            "tools/flw0_symbolic.py",
                            "tools/flw0_semantic.py",
                            "tools/flw0_profiles.py",
                            str(source.with_name("aicalc-ai.bfasm")),
                            str(source.with_name("aicalc-formulas.bfasm")),
                            str(skill_source),
                            str(unit_source),
                        )
                    )
                    context += f" --units {unit_source}"
                elif source.stem == "encount":
                    dependencies.extend((str(skill_source), str(unit_source)))
                    context += f" --units {unit_source}"
                elif source.stem == "unit":
                    dependencies.append(str(skill_source))
                elif source.stem == "msg":
                    dependencies.extend(message_dependencies)
                n.build(
                    str(output),
                    "battle_tbl",
                    str(source),
                    implicit=dependencies,
                    variables={"outdir": str(output.parent), "context": context},
                )
                battle_outputs.append(str(output))
            battle_stamp = battle_output_dir / "battle_tables.ok"
            n.build(
                str(battle_stamp),
                "check",
                str(battle_manifest),
                implicit=battle_outputs,
            )
            n.build(f"{version}-battle-data", "phony", str(battle_stamp))
            version_outputs.append(str(battle_stamp))

        n.build(version, "phony", version_outputs)
        defaults.append(version)
        (ROOT / "config" / version / "checksum.sha1").write_text(f"{VERSIONS[version]['elf_sha1']}  {image}\n")
        n.newline()

    write_objdiff_reports(n, units)

    configure_inputs = ["configure.py", "config/versions.json"] + [
        f"config/{v}/{VERSIONS[v]['serial']}.yaml" for v in versions
    ] + [f"config/{v}/symbol_addrs.txt" for v in versions]
    configure_inputs.extend(sorted(dev_configure_inputs))
    for version in versions:
        dev_descriptor = ROOT / "config" / version / "devbuild.json"
        if dev_descriptor.exists():
            configure_inputs.append(str(dev_descriptor.relative_to(ROOT)))
        for name in ("scripts.sha1", "event_scripts.sha1"):
            manifest = ROOT / "config" / version / name
            if manifest.exists():
                configure_inputs.append(str(manifest.relative_to(ROOT)))
                break
        inf_manifest = ROOT / "config" / version / "field_inf.sha1"
        if inf_manifest.exists():
            configure_inputs.append(str(inf_manifest.relative_to(ROOT)))
        wap_manifest = ROOT / "config" / version / "field_wap.sha1"
        if wap_manifest.exists():
            configure_inputs.append(str(wap_manifest.relative_to(ROOT)))
        for kind in ("npl", "sky"):
            environment_manifest = ROOT / "config" / version / f"field_{kind}.sha1"
            if environment_manifest.exists():
                configure_inputs.append(str(environment_manifest.relative_to(ROOT)))
        fld_manifest = ROOT / "config" / version / "field_fld2.sha1"
        if fld_manifest.exists():
            configure_inputs.append(str(fld_manifest.relative_to(ROOT)))
        fld1_manifest = ROOT / "config" / version / "field_fld1.sha1"
        if fld1_manifest.exists():
            configure_inputs.append(str(fld1_manifest.relative_to(ROOT)))
        battle_manifest = ROOT / "config" / version / "battle_tables.sha1"
        if battle_manifest.exists():
            configure_inputs.append(str(battle_manifest.relative_to(ROOT)))
    n.build("build.ninja", "configure", implicit=configure_inputs,
            variables={"args": " ".join(sys.argv[1:])})
    n.default(defaults)
    n.close()
    return units


def write_objdiff(units: dict[str, list[dict]]) -> None:
    categories = [
        {"id": "game", "name": "Atlus game/engine"},
        {"id": "sdk", "name": "Sony SDK / C runtime"},
        {"id": "vu1", "name": "VU1 microcode (binary)"},
    ]
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
            category = objdiff_progress_category(version, row["name"])
            unit = {
                "name": "/".join(Path(row["name"]).parts[1:]),  # drop asm/ or src/: "<version>/<unit>"
                "target_path": row["target"],
                "metadata": {"progress_categories": [version, category]},
            }
            if row["base"]:
                unit["base_path"] = row["base"]
            config["units"].append(unit)
    config["progress_categories"] = [{"id": v, "name": VERSIONS[v]["title"]} for v in units] + categories
    (ROOT / "objdiff.json").write_text(json.dumps(config, indent=2) + "\n")
    # Per-version configs for the per-version reports; objdiff resolves paths
    # relative to the config's directory.
    for version in units:
        sub = dict(config, units=[], progress_categories=categories)
        for unit in config["units"]:
            if unit["name"].split("/", 1)[0] != version:
                continue
            unit = dict(unit, name=unit["name"].split("/", 1)[1],
                        metadata={"progress_categories": [c for c in unit["metadata"]["progress_categories"]
                                                          if c != version]})
            for key in ("target_path", "base_path"):
                if key in unit:
                    unit[key] = str(Path("..", "..", unit[key]))
            sub["units"].append(unit)
        path = ROOT / "build" / version / "objdiff.json"
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(json.dumps(sub, indent=2) + "\n")
        # decomp.dev's headline uses the report's overall matching-code measure.
        # Give it a coherent game-only project, while keeping the full config
        # above available for SDK/VU diagnostics and report.all.json.
        primary = dict(sub, units=[], progress_categories=[c for c in categories if c["id"] == "game"])
        for unit in sub["units"]:
            if "game" not in unit["metadata"]["progress_categories"]:
                continue
            unit = dict(unit)
            for key in ("target_path", "base_path"):
                if key in unit:
                    unit[key] = str(Path("..", unit[key]))
            primary["units"].append(unit)
        path = ROOT / "build" / version / "progress" / "objdiff.json"
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(json.dumps(primary, indent=2) + "\n")


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
