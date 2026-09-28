# Shin Megami Tensei: Digital Devil Saga 1 & 2

[![Build Status]][actions] [![dds1]][progress] [![dds2]][progress]

[Build Status]: https://github.com/Raikaru/dds-decomp/actions/workflows/build.yml/badge.svg
[actions]: https://github.com/Raikaru/dds-decomp/actions/workflows/build.yml
[dds1]: https://decomp.dev/Raikaru/dds-decomp/dds1.svg?mode=shield&label=dds1
[dds2]: https://decomp.dev/Raikaru/dds-decomp/dds2.svg?mode=shield&label=dds2
[progress]: https://decomp.dev/Raikaru/dds-decomp

A work-in-progress **matching decompilation** of *Shin Megami Tensei: Digital
Devil Saga* and *Digital Devil Saga 2* for the PlayStation 2. The goal is C
source that compiles to byte-identical copies of the retail executables and
reads like the source the developers wrote.

> [!IMPORTANT]
> This repository does **not** contain game assets, executables or any other
> copyrighted material. You need your own copy of the games to build it.
>
> This is not a PC port. It rebuilds the original PS2 executables.

> [!WARNING]
> Work in progress. Unfinished functions are still assembly, and function
> names, types and file layout change often. Most names were chosen by us (see
> [Names](#names)).

## Versions

| Version | Game | Serial | ELF SHA-1 | Functions in C | Code bytes in C |
|---|---|---|---|---|---|
| `dds1` | Digital Devil Saga (USA) | `SLUS_209.74` | `6d18898e2724bf1d145392766e8ba1e678487419` | 4,924 / 9,595 (51.3%) | 14.1% |
| `dds2` | Digital Devil Saga 2 (USA) | `SLUS_211.52` | `9be91ee1b4a535a4cb6ec89237b5a6ba41be2add` | 4,360 / 10,877 (40.1%) | 9.0% |

Both builds are byte-identical at every commit: `ninja` fails when a SHA-1
doesn't match. Run `python tools/progress.py` for current numbers. The Sony
SDK libraries are prebuilt archives in the original game too; they stay
assembly and are not counted.

## Quickstart

Requirements:
- Linux x86-64 (or WSL)
- Python 3.10+, `ninja`, `cpp`, `git`
- 32-bit glibc, because the 2000-era compiler binaries are i386 ELF.
  Install `libc6:i386` (Debian/Ubuntu) or `glibc.i686` (Fedora). Without a
  system `/lib/ld-linux.so.2`, point `DDS_I386_LIBDIR` at a directory that
  contains `ld-linux.so.2` and `libc.so.6`.

```sh
git clone https://github.com/Raikaru/dds-decomp.git && cd dds-decomp
python -m pip install -r requirements.txt
python tools/download_tools.py   # ee-gcc 2.96 + ee-as, decompals binutils, objdiff-cli
# copy your disc image(s) into the repo root or orig/, then:
python tools/extract.py          # -> orig/dds1/SLUS_209.74, orig/dds2/SLUS_211.52 (SHA-1 checked)
python configure.py              # split with splat, write build.ninja and objdiff.json
ninja                            # build and verify every extracted version (or: ninja dds1)
```

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
include/                    common.h, include_asm.h, fpu.h, macro.inc
docs/CONTRIBUTING.md        how to decompile, verify, name and share a function
docs/idioms.md              source shapes confirmed against retail codegen
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
Not comfortably yet. Much of the code is still assembly at fixed addresses,
and data isn't shiftable.

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
