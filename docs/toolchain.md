# Toolchain

A matching decompilation reconstructs readable C that reproduces the original
machine code byte for byte when compiled with the original compiler and flags.
The recovered source describes the implementation; it is not the original
source text.

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
[Persona 4 transfer](p4-transfer.md) has a second check against that decomp.

A handful of files were built without sibling-call optimisation. They are
recorded with their evidence in `config/<v>/cflags.txt`.

## Pinned compiler runtime

The 2000-era compiler binaries are i386 ELF. `tools/download_tools.py`
fetches the exact 32-bit glibc they are run under (Fedora `glibc-2.43-8`
i686), because ee-gcc 2.96's output can depend on the C library's heap
layout: a different libc can compile some functions differently.

See [Building](building.md) for setup and build targets, and the
[matching rules](CONTRIBUTING.md#matching-rules) for source requirements.
