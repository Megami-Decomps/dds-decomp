# Building

Run the commands in the [quickstart](../README.md#quickstart) from the
repository root. Only the game versions extracted under `orig/` are configured
by default.

## Retail executables

```sh
ninja       # build and verify every extracted version
ninja dds1  # build and verify DDS1 only
ninja dds2  # build and verify DDS2 only
```

`tools/extract.py` checks the extracted executables' SHA-1 and places them,
along with archive inputs, under `orig/`. `configure.py` splits the
executables with splat and writes `build.ninja` and `objdiff.json`.

`ninja`'s last step runs `sha1sum --quiet -c` on each built ELF
(`build/<v>/SLUS_*`). It is silent when the ELF matches. A mismatch prints

```
build/dds1/SLUS_209.74: FAILED
```

and the build fails.

## Scripts and data

These targets assemble and verify the tracked source corpora. Archive rebuilds
also use the original inputs extracted from your disc image.

```sh
ninja dds1-scripts dds2-scripts
ninja dds1-field-data dds2-field-data
ninja dds1-field-archives dds2-field-archives
ninja dds1-battle-data dds2-battle-data
```

- **Scripts:** [BF/FLW0](flw0.md) VM scripts.
- **Field data:** [INF](inf.md) interaction tables, [WAP](wap.md) actor,
  elevator, door, and transition tables, [FLD1/FLD2](fld.md) field resources,
  [AMB](amb.md) automaps, and [NPL/SKY](field-environment.md) palettes and lighting.
- **Field archives:** exact compressed [LB](lb.md) archives.
- **Battle data:** [encounter and battle tables](battle-tables.md).

For a readable script view:

```sh
python3 tools/flw0.py view src/dds1/scripts/event/e670.bfasm
```

See [TMX](tmx.md) for field texture bundles and PNG decoding.

## Development executables

```sh
ninja dds1-dev dds2-dev
```

The experimental targets can relocate selected code and data without changing
the exact retail targets. See [Development builds](development-build.md) for
their supported source objects, assembly fallbacks, and verification contracts.
Arbitrary mutable-state relocation and a mod loader remain out of scope.

## Progress reports

```sh
python tools/progress.py
ninja report
```

See [Reading progress](progress.md) for source coverage, comparison reports,
categories, and CI artifacts.

## Project structure

```text
config/versions.json        serial, SHA-1 and gp of each version
config/<v>/SLUS_*.yaml      splat config: every .text unit and its .rodata/.lit4/.sdata
config/<v>/symbol_addrs.txt names and addresses (curated on top, generated below)
config/<v>/name_sources.txt provenance of every curated name (evidence / inferred)
config/<v>/cflags.txt       per-file compiler options, with evidence
src/<v>/<dir>/<unit>.c      C units; INCLUDE_ASM marks functions not decompiled yet
src/<v>/scripts/            exact, editable source for decompiled game scripts
src/<v>/data/field/         exact, editable source for field resources and interaction tables
include/                    common.h, include_asm.h, fpu.h, macro.inc
docs/CONTRIBUTING.md        how to decompile, verify, name and share a function
docs/compiler-decision-atlas.md route mismatches to compiler evidence and stop rules
docs/idioms.md              source shapes confirmed against retail codegen
docs/inf.md                 field interaction layout and editable source format
docs/field-environment.md   field NPC palette and sky-light source formats
docs/tu-names.md            where unit names come from (Nocturne __FILE__ strings)
tools/                      build, checking, splitting and analysis tools
asm/ assets/ build/ orig/   generated or extracted locally (git-ignored)
```

`.text` is split into C units named after the original source files where the
evidence exists (`kernel/dds3KernelCore`, `effect/effPCPMisc`,
`sdf/sdfModel`, ...). DDS has no `__FILE__` strings, but a December 2002
debug build of *Nocturne*, which shares the engine, does. Units without
proven names are `game/code_<vram>`, split at proven file boundaries.

See the [toolchain notes](toolchain.md) for compiler and assembler evidence,
and the [documentation index](README.md) for the full reference list.
