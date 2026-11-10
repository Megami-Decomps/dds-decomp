# Resource-overwrite prompt: live coordinate allocation

`btlPollResourceNameOverwrite` occupies 440 native bytes in each game:

- DDS1: `src/dds1/game/code_001FC7D8.c`, address `0x001FC7D8`.
- DDS2: `src/dds2/game/code_0020E850.c`, address `0x0020E858`.

The two reconstructions preserve the path formatting, file-existence check,
prompt state changes, pad-button decisions, and all three text calls. The
five owned string literals retain their native storage contracts. Any negative
open result grants permission, preserving the native behavior for errors other
than a missing file as well. The shared
`OK` string remains an external symbol belonging to the resource-name editor.

## Ordinary source and qualified compiler cause

Ordinary source produces the complete 440-byte extent and the same operations
and preallocation instruction order. Different physical register homes and
consequent prologue save order leave 33 of 110 words differing. Naming the selected-text style constant does not resolve this.
Plain `register` hints on the real coordinate locals also leave the same
residual. Binding only X leaves 29 differing words.

The real compiler's `19.lreg` and `20.greg` dumps identify the cause. In the
DDS2 plain-local control, the selected-text style constant 4 is pseudo 154.
It is local to the drawing block, crosses two calls, and receives register 16
before global allocation starts. The record pointer, X coordinate, and Y
coordinate are global pseudos 84, 98, and 99. They then receive registers
17, 18, and 19 respectively. This is not a different drawing algorithm or
an instruction-scheduling residual.

The final source binds the actual X and Y snapshots to the native callee-saved
registers, inside a block beginning after the path-formatting calls:

```c
register s32 x __asm__("$16") = record->x;
register s32 y __asm__("$17") = record->y;
```

With those real values live in registers 16 and 17, local allocation puts the
style constant in register 18. Global allocation puts the record in register
19. The earlier directory-path value may still use register 16 before the
coordinate lifetime begins. These are localized compiler allocation
constraints, not recovered original spelling or hardware requirements.

## Semantics and ABI

Both coordinate values are initialized from their actual signed fields before
any use. They have the same definitions, arithmetic consumers, and memory-read
phase as ordinary source. They do not escape and add no loads, stores, dummy
consumers, false qualifiers, or extended lifetimes. The scoped bindings emit
no instruction templates and preserve the native stack frame and epilogue.

The coordinates remain live across the first two text calls, and are consumed
by the third. Registers 16 and 17 are callee-saved. The actual native
`btlDebugDrawText` in both games is additionally an empty return stub, so its
bodies directly confirm that these registers are not clobbered. The other
calls either precede the definitions or occur on paths that return without
using the coordinates.

Independent semantic review approves these specific initialized bindings, their
real consumers, disjoint lifetimes, unchanged memory effects, and ABI.

## Qualified output and native checks

The compiler SHA-256 is
`d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`.
For each game's final candidate and corresponding ordinary-local removal
control, ordinary compilation and diagnostic `-da` compilation agree across
normalized whole-owner assembly, every allocated section's contents and
metadata, complete symbol/relocation contracts, non-debug auxiliary sections,
and ELF ABI metadata.

Each owner has five allocated sections. DDS1 has 361 symbols and
301 relocation contracts; DDS2 has 383 symbols and 310 relocation
contracts. Section file positions and string-table name offsets are excluded
from semantic header comparison; the actual section names are compared.

Canonical complete-owner checks pass with 60 matches and zero differences in
DDS1, and 66 matches and zero differences in DDS2, including owned data.
Replacing the bound coordinate locals with ordinary locals leaves one
nonmatching function in each owner: 59/1 and 65/1 respectively. These removal
controls are distinct from instrumentation neutrality and final link proof.

Promotion also removes the two old `INCLUDE_SDATA` entries for `pfs:` and
`CANCEL` in each game. The C literals now own those two eight-byte entries.
Keeping the old includes would duplicate 16 bytes in the production object,
although the `SKIP_ASM` owner check suppresses those includes. The initial full
link caught that duplication; the final four qualification captures include
its cleanup. The external shared `OK` string is unchanged.

Private native files and compiler dumps are not part of this source change.

## Full validation

Both final complete retail builds pass their exact SHA-1 gates:

- DDS1: `6d18898e2724bf1d145392766e8ba1e678487419`.
- DDS2: `9be91ee1b4a535a4cb6ec89237b5a6ba41be2add`.

The repository suite passes all 442 tests in 117 batches.

All 81 remaining retained-C units compile, and `git diff --check` passes.
