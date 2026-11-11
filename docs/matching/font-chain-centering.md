# Font-chain row centering

`frFontCenterGlyphChainAt` is now ordinary matching C in both font owners:
DDS1 `0x00196088` and DDS2 `0x0019DD48`, 296 native bytes each. The same
body uses the existing per-game band heights, `0x54` and `0x64`.

This continues redthing1's retained reconstructions (`359b34e938` in DDS1
and `d1a8b84bc5` in DDS2), with tge-was-taken's subsequent font type and
field names. The matching source spelling is not claimed to be original.

## Source and read epochs

A null input chain is a no-op. A non-null chain requires a non-null
`chainHead`: retail reads that origin's Y before checking it, as does the
matched `frFontMoveChainTo` family. The initial Y supplies the displacement
from the first row to the requested Y.

For each row, the current glyph's original Y is captured before the width
calls. That value supplies both the measurement and translation limits.
The measurement walk follows the guarded, null-tolerant band-scan shape
already present in `frFontMeasureGlyphStripWidth`/`frFontMeasureLineWidth`
and `frFontGetGlyphPositionAtIndex` in the same owners. Its checks and
bounds are real comparisons, not invented loop barriers.

After measuring, the first glyph's current X supplies the horizontal
translation. The translation walk reads each glyph's current Y again,
updates that field, then stores `oldX + deltaX - (width / 2) * 16`. Width
and division remain signed, including truncation toward zero. The next
outer iteration takes its own row snapshot rather than making the source
snapshot variable a cursor updated at every translation comparison.

Every local has real consumers. No bindings, empty constraints, inline
assembly, false types, dead values, wrappers, or compiler-flag changes
were added. A split pair of meaningful X updates also matched, but the
single add-then-subtract expression is the smaller retained form.

## Qualified stock compiler evidence

The complete owners were compiled normally and with `-da` through the
canonical `tools/cc.sh` wrapper and pinned ee-gcc 2.96 binary
`d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`.
Assembly agrees after normalizing only the scratch `.file` path. All five
allocated sections and all allocated-section relocation contracts agree:
317 relocations in DDS1 and 411 in DDS2. The same parity checks pass for
the DDS1 causal control below. No debugger or instrumentation was used.

In the accepted DDS1 whole-owner dumps:

- Expansion gives the outer-row Y snapshot a real load at instruction 44.
- `08.gcse` reports `PRE: redundant insn 44 (expression 1) in bb 2,
  reaching reg is 115`. Copies 220, 221 and 222 receive that expression
  from the initial Y load 23, the translation-entry Y load 203, and the
  next-glyph Y load 114. The width and translation bounds consume it.
- Allocation gives reaching pseudo115 `s2`, while the translation's
  current-value pseudo99 is in `v1`. Thus the field update can consume
  the transient coordinate while the next row's baseline remains available.
  The native entry copy and later loop copy emerge from these live reads.
- `09.loop` identifies the nine-instruction measurement loop and the
  48-instruction outer loop. It reports the translation loop's existing
  multiple entries. These are consequences of the real band walk and PRE,
  not an artificial one-pass block or an attempt to disable optimization.

The causal control removes the outer snapshot and instead assigns
`currentY` in the translation condition. Its source value remains live,
but it no longer gives GCSE the outer load to eliminate. That control has
35 differing covered words, emits 280 bytes, and leaves 16 native bytes
uncovered. The original retained DDS1 draft emits 284 bytes with 69 of 71
compared words differing. Neither control was promoted.

## Owned-data path repair

Both `symbol_aliases.json` files still assigned the seven interior labels
of `frFontWork` to the pre-split `game/code_00193C08`/`game/code_0019B840`
units. The existing font-system definition, compile-time field offsets,
and YAML BSS owner are already in `interface/frFont`. Correct only those
fourteen `unit` strings. Names, owner symbols, offsets, native addresses,
primary extent (`0x19C`), and ownership checking rules remain unchanged.
This removes the stale-path ownership errors without allocating aliases
or hiding any object boundary.

## Verification

- DDS1 and DDS2 complete owners: each **92 match, 0 differ**, including
  owned data.
- Both full retail executables pass their configured SHA-1 checks.
- **445 tests in 118 batches: OK**; `git diff --check` is clean.
- Independent semantic review accepts the read epochs, signed arithmetic,
  ordinary family loop shape, and non-null-origin requirement.

A compact receipt with source and evidence hashes is in
`docs/compiler-evidence/font-chain-centering/qualification.json`.
The full stock dumps and negative-control sources remain in the local
investigation evidence rather than the source repository.
