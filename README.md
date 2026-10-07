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
Devil Saga* and *Digital Devil Saga 2* for PlayStation 2. The goal is to
understand and document how the games work by reconstructing readable source
code from their retail binaries. Matching builds verify the reconstruction
against the original executables.

Building requires your own lawfully obtained copy of the corresponding game.
Disc images and retail executables are not included; the required files are
extracted locally from your copy.

## Status and versions

Unfinished functions use assembly extracted from the original executable, so a
matching build does not mean the decompilation is complete. Names, types, and
source organization are still being recovered.

| Version | Game | Serial | ELF SHA-1 |
|---|---|---|---|
| `dds1` | Digital Devil Saga (USA) | `SLUS_209.74` | `6d18898e2724bf1d145392766e8ba1e678487419` |
| `dds2` | Digital Devil Saga 2 (USA) | `SLUS_211.52` | `9be91ee1b4a535a4cb6ec89237b5a6ba41be2add` |

The badges track matching game/engine code bytes and function counts for each
game. SDK/runtime code and VU1 microcode are outside those totals.
[Reading progress](docs/progress.md) explains the measurements and comparison
limits.

## Quickstart

Requirements:

- Linux x86-64 or WSL2, with support for running 32-bit i386 programs.
- Python 3.10+, `ninja`, `cpp`, and `git`.
- Your own disc image of either supported game.

```sh
git clone https://github.com/Megami-Decomps/dds-decomp.git
cd dds-decomp
python -m pip install -r requirements.txt
python tools/download_tools.py

# Copy your disc image(s) into the repository root or orig/, then:
python tools/extract.py
python configure.py
ninja
```

`ninja` builds every extracted version and checks each executable against its
retail SHA-1. Use `ninja dds1` or `ninja dds2` to build one game. A checksum
mismatch fails the build.

See [Building](docs/building.md) for optional script, data, and development
targets, generated files, and checksum troubleshooting. The
[toolchain notes](docs/toolchain.md) explain the original compiler, assembler,
and pinned 32-bit runtime.

## Documentation and contributing

Start with the [contribution guide](docs/CONTRIBUTING.md) and
[confirmed source idioms](docs/idioms.md) to reconstruct a function. Pick an
`INCLUDE_ASM` function, replace it with readable C, check the whole unit with
`tools/check_unit.py`, and verify that `ninja` still produces a matching
executable.

DDS2 reuses much of DDS1's engine. The contribution guide also covers sharing
reconstructed functions between games and recovering names and types.

The [documentation index](docs/README.md) groups the available references:

- **Code reconstruction:** matching workflow, compiler behavior, and unit names.
- **Scripts:** compiled VM scripts and editable source.
- **Field resources:** interaction and transition tables, field formats,
  automaps, lighting, archives, and textures.
- **Battle data:** encounter and battle content tables.
- **Development builds:** experimental relocatable executables.

## Acknowledgements

- [splat](https://github.com/ethteck/splat), [spimdisasm](https://github.com/Decompollaborate/spimdisasm),
  [m2c](https://github.com/matt-kempster/m2c) and [objdiff](https://github.com/encounter/objdiff)
- [decomp.me](https://decomp.me) for the ee-gcc 2.96 toolchain packaging
- the Persona 3/4 decompilation projects, for SDK layout and naming conventions
- romwright, for analysis and cross-game pairing
