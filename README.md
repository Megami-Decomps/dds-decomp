# Shin Megami Tensei: Digital Devil Saga 1 & 2

[![Build Status]][actions]
[![dds1-code]][dds1-progress] [![dds1-functions]][dds1-progress]
[![dds2-code]][dds2-progress] [![dds2-functions]][dds2-progress]

[Build Status]: https://github.com/Megami-Decomps/dds-decomp/actions/workflows/build.yml/badge.svg
[actions]: https://github.com/Megami-Decomps/dds-decomp/actions/workflows/build.yml
[dds1-code]: https://decomp.dev/Megami-Decomps/dds-decomp/dds1.svg?mode=shield&category=game&measure=matched_code_percent&label=dds1%20code%20bytes
[dds1-functions]: https://decomp.dev/Megami-Decomps/dds-decomp/dds1.svg?mode=shield&category=game&measure=matched_functions_percent&label=dds1%20functions
[dds2-code]: https://decomp.dev/Megami-Decomps/dds-decomp/dds2.svg?mode=shield&category=game&measure=matched_code_percent&label=dds2%20code%20bytes
[dds2-functions]: https://decomp.dev/Megami-Decomps/dds-decomp/dds2.svg?mode=shield&category=game&measure=matched_functions_percent&label=dds2%20functions
[dds1-progress]: https://decomp.dev/Megami-Decomps/dds-decomp/dds1?category=game
[dds2-progress]: https://decomp.dev/Megami-Decomps/dds-decomp/dds2?category=game
[progress]: https://decomp.dev/Megami-Decomps/dds-decomp

A work-in-progress **matching decompilation** of *Shin Megami Tensei: Digital
Devil Saga* and *Digital Devil Saga 2* for the PlayStation 2. The goal is C
source that compiles to byte-identical copies of the retail executables and
reads like the source the developers wrote.

> [!IMPORTANT]
> This repository contains reconstructed source, including source-form game
> scripts and data. It does **not** contain disc images, retail executables or
> archives, extracted game files, SDK binaries, or generated build outputs.
> You need your own copy of the games to build the retail executables.
>
> This is not a PC port. It rebuilds the original PS2 executables.

> [!WARNING]
> Work in progress. Unfinished functions are still assembly, and function
> names, types and file layout change often. Most names were chosen by us (see
> [Names](#names)).

## Versions

| Version | Game | Serial | ELF SHA-1 |
|---|---|---|---|
| `dds1` | Digital Devil Saga (USA) | `SLUS_209.74` | `6d18898e2724bf1d145392766e8ba1e678487419` |
| `dds2` | Digital Devil Saga 2 (USA) | `SLUS_211.52` | `9be91ee1b4a535a4cb6ec89237b5a6ba41be2add` |

`ninja` verifies each rebuilt executable against its retail SHA-1. The build
includes assembly fallbacks for unfinished functions, so a byte-identical
executable does not mean all its code has been decompiled.

### Reading progress

- `python tools/progress.py` reports current **source coverage** after splitting:
  game functions and their retail code bytes that no longer use `INCLUDE_ASM`.
  This inventory does not compile or independently verify the functions.
- [decomp.dev][progress] uses the generated **objdiff comparison reports**.
  Exact matching credits only functions with a 100% comparison result; fuzzy
  matching also gives partial credit for similar instructions. These are
  different measures from source coverage.
- The primary reports cover **Atlus game/engine EE code**, the C reconstruction
  target. Their overall totals and **Atlus game/engine** category contain the
  same units, including unfinished game functions. The headline and code-byte
  badges measure matching bytes; the separate function badges measure matching
  function counts. A short function and a large function contribute equally
  only to the latter.
- **Sony SDK / C runtime** code was linked from prebuilt libraries and remains
  assembly. **VU1 microcode (binary)** is the binary `.vutext` program, which
  splat exposes as a text unit rather than individual EE functions. These are
  outside the primary game-code denominator and `tools/progress.py`, but stay
  in the full-binary audit reports and local objdiff configurations.
- The reports do not currently set objdiff's **complete/linked** metadata.
  A zero there is not a measurement of source coverage or build success.

There is a known comparison-context limitation: objdiff bases are compiled
separately with `-DSKIP_ASM`. ee-gcc 2.96 can select different instructions
when the surrounding source or compiler pathnames change, so an accepted C
function can score below 100% in that comparison. `tools/check_unit.py` also
checks the full build context and recognizes these as-built matches. See
[the matching workflow](docs/CONTRIBUTING.md#3-verify) and
[compiler context](docs/idioms.md#code-that-changes-with-unrelated-text-context).
The reporting-context discrepancy remains open; use the unit checks and
retail checksum build when validating a match.

`ninja report` generates both views: `build/<v>/report.json` is the primary
game-code report, and `build/<v>/report.all.json` retains game, SDK/runtime,
and VU1 units with their separate categories. The root `report.json` combines
both games' full-binary reports. CI publishes the primary files as
`dds1_report` and `dds2_report` for decomp.dev and the full-binary files in the
separate **build-audit** artifact on [the build run][actions]. objdiff computes
each report's totals from its included units; no match results are rewritten.

## Quickstart

Requirements:
- Linux x86-64 (or WSL)
- Python 3.10+, `ninja`, `cpp`, `git`
- A kernel that runs 32-bit i386 programs (any normal x86-64 Linux or WSL2).
  The 2000-era compiler binaries are i386 ELF. `tools/download_tools.py`
  fetches the exact 32-bit glibc they are run under (Fedora `glibc-2.43-8`
  i686), because ee-gcc 2.96's output can depend on the C library's heap
  layout: a different libc can compile some functions differently.

```sh
git clone https://github.com/Megami-Decomps/dds-decomp.git && cd dds-decomp
python -m pip install -r requirements.txt
python tools/download_tools.py   # ee-gcc 2.96 + ee-as, decompals binutils, objdiff-cli
# copy your disc image(s) into the repo root or orig/, then:
python tools/extract.py          # -> SHA-1-checked executables and authored archive inputs under orig/
python configure.py              # split with splat, write build.ninja and objdiff.json
ninja                            # build and verify every extracted version (or: ninja dds1)
ninja dds1-dev dds2-dev          # build the relocatable development ELFs
ninja dds1-scripts dds2-scripts  # assemble and verify the tracked script corpora
ninja dds1-field-data dds2-field-data  # assemble and verify field tables, models, automaps, palettes, and lighting
ninja dds1-field-archives dds2-field-archives  # rebuild field resources inside exact LB archives
ninja dds1-battle-data dds2-battle-data  # assemble and verify battle tables
python3 tools/flw0.py view src/dds1/scripts/event/e670.bfasm  # readable script view
```

See [`docs/flw0.md`](docs/flw0.md) for script source,
[`docs/inf.md`](docs/inf.md) for interaction tables, and
[`docs/wap.md`](docs/wap.md) for actor, elevator, door, and transition tables.
See [`docs/fld.md`](docs/fld.md) for relocatable FLD1/FLD2 field resources,
[`docs/amb.md`](docs/amb.md) for standalone automap resources,
[`docs/field-environment.md`](docs/field-environment.md) for NPL palettes and
SKY light sets,
[`docs/lb.md`](docs/lb.md) for their compressed field archives, and
[`docs/tmx.md`](docs/tmx.md) for field texture bundles and PNG decoding. See
[`docs/battle-tables.md`](docs/battle-tables.md) for encounter and battle
content tables. See [`docs/development-build.md`](docs/development-build.md)
for the experimental relocatable development ELFs.

`ninja`'s last step runs `sha1sum --quiet -c` on each built ELF
(`build/<v>/SLUS_*`). It is silent when the ELF matches. A mismatch prints

```
build/dds1/SLUS_209.74: FAILED
```

and the build fails.

## How it's built

The toolchain was identified from the binaries, not guessed:

| Code | Toolchain |
|---|---|
| Atlus game and engine code | ee-gcc 2.96 (`2.96-ee-001003-1`) at `-O2`, assembled by Sony's ee-as with `-G8` |
| Sony SDK 2.5.x libraries, newlib, libgcc | prebuilt archives, identified by signature matching |
| `.vutext` | VU1 microcode, kept as binary |

Evidence:
- Callee-saved registers are stored with `sd`, never `sq` (MWCC uses `sq`).
- `move` is encoded as `daddu`.
- `.mdebug.eabi64` is present.
- Of 231 random functions compiled straight from m2c output, 94 match under
  ee-gcc 2.96, against 0 to 26 for the other candidate compilers.

The original assembler matters too: modern GNU as encodes `move` differently
and inserts FPU hazard `nop`s, so C is assembled with ee-as.
`docs/p4-transfer.md` has a second check against the Persona 4 decomp.

A handful of files were built without sibling-call optimisation. They are
recorded with their evidence in `config/<v>/cflags.txt`.

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

## FAQ

**What is a matching decompilation?**
Hand-written C that, compiled with the original compiler and flags,
reproduces the original machine code byte for byte. It isn't the original
source, but it behaves identically, and its structure and names are meant to
be what the developers could plausibly have written.

**Are there shortcuts in the matched code?**
No. `tools/check_unit.py` rejects the common fakematch techniques: register
pinning, computed-goto label tables standing in for switches, and functions
that only match when compiled outside their unit. Inline assembly is limited
to VU0 instructions that C cannot express. The rules are in
[docs/CONTRIBUTING.md](docs/CONTRIBUTING.md#matching-rules).

**Why two games in one repository?**
DDS2 reuses most of DDS1's engine: about 8,600 functions are byte-identical
once relocations are masked. Each one decompiled in either game is ported to
the other automatically (`tools/shared_funcs.py`).

**Can I mod the game with this?**
Not comfortably yet. The experimental `dds1-dev` and `dds2-dev` targets can
each recompile and relocate three code units' selected code, read-only data,
and initialized small data into an appended loadable segment without changing
the exact retail targets. Replacement code may change size, one current unit
may retain explicitly verified assembly fallbacks, and the targets also link a
development-only C entry hook and state block. General mutable-state relocation
and a mod loader are still out of scope.

## Names

Neither game ships symbols, so nearly every function and variable name here
was chosen by contributors. Names follow Atlus's convention: a lowercase
module prefix plus CamelCase, e.g. `sdfAddHandler` or `btlResetRuntime`.
`config/<v>/name_sources.txt` marks each name as `evidence` (the binary names
the function in its own debug text) or `inferred` (our choice). Treat
`inferred` names as descriptions, not original symbols.

## Contributing

Contributions are welcome, including small ones. Start with
[docs/CONTRIBUTING.md](docs/CONTRIBUTING.md) and
[docs/idioms.md](docs/idioms.md), pick an `INCLUDE_ASM` function, and send a
pull request once `ninja` stays green and `check_unit.py` is clean for the
units you touched.

## Acknowledgements

- [splat](https://github.com/ethteck/splat), [spimdisasm](https://github.com/Decompollaborate/spimdisasm),
  [m2c](https://github.com/matt-kempster/m2c) and [objdiff](https://github.com/encounter/objdiff)
- [decomp.me](https://decomp.me) for the ee-gcc 2.96 toolchain packaging
- the Persona 3/4 decompilation projects, for SDK layout and naming conventions
- romwright, for analysis and cross-game pairing
