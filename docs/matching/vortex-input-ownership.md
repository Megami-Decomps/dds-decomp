# Vortex emitter input ownership

DDS1 `func_002ABDE0` and DDS2 `func_002EF1C0` each contain 1,788 bytes. They
advance the same vortex emitter and rebuild its live particles' ribbon rows.
Their shared source preserves the real randomized endpoints, state updates,
geometry and callback read boundaries.

## Ordinary source recovery

The spawn phase reuses the actual RNG result for randomized endpoint products,
then computes radius and climb interpolation steps. A negative climb-origin
snapshot preserves the native definition frontier. Initial burst fast-forward
updates climb, then radius, directly through the particle fields. The live
phase already uses those same two axes. One ordinary floating-age workspace
serves the disjoint burst and live branches, with a fresh definition before
every use.

The live angle calculation stages the integrated angular displacement before
adding its phase. After color-track sampling, it reads the fresh stored height
before publishing the returned color, as retail does. The center and radial
vectors initialize XYZ only, preserving the native partial-vector contract.

This ordinary source leaves seven instruction words different, all exchanging
climb's f1 and width's f3 against retail's climb f3 and width f1.

## Two localized initialized inputs

The matching compiler reconstruction constrains two short-lived inputs:

```c
register f32 previousClimb __asm__("$f3") = entry->vertical.vortex.climb;
f32 climb = previousClimb + entry->vertical.vortex.climbStep;
/* Other real geometry snapshots. */
register f32 spinIncrement __asm__("$f2");

/* Existing publication and segment computation. */
spinIncrement = spinAccel * ageAsFloat;
angle = (spin + spinIncrement * 0.5f) * ageAsFloat;
angle += phase;
```

The first bound value dies at the climb addition. The second dies at its
half-step multiplication. Both are initialized in the live branch, have one
real consumer, and die before any call. The resulting climb and angle are
ordinary locals. No source-visible read, write, arithmetic operation, callback,
clobber or artificial use is added. In particular, naming the first product
preserves the original left-associated floating-point operations.

These are compiler allocation constraints, not recovered original spelling or
hardware-register requirements. The existing VU helpers are unchanged.

## Stock compiler evidence

The pinned cc1 SHA-256 is
`d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`.
Each final source and each binding-removal control has a canonical whole-unit
ordinary compilation and a separate stock RTL capture. Qualification compares
complete normalized assembly, all allocated section bytes and metadata,
complete symbol and resolved relocation contracts, auxiliary nondebug sections
and ELF ABI. Compiler and assembler success are checked separately.

In DDS2's final capture:

- Pass 00 UID557 initializes f3 from the actual climb field. UID561 computes
  ordinary climb pseudo235 from that value and the real increment.
- Pass 00 UID590 computes acceleration times age and UID592 assigns it to f2.
  Pass 14 combines those into UID592's direct product, leaving no extra native
  copy. UID597 is the existing multiplication by 0.5f.
- Pass 17 records f3 dead at UID561 and f2 dead at UID597, before any call.
- Pass 19 allocates climb235 to f3, width242 to f1 and the half-step result253
  to f2. Pass 20 retains those homes.

Removing only the climb binding gives climb234/235 f1 and width242 f3, restoring
exactly seven differing words. Removing only the spin binding preserves climb
f3 but gives spin-product246 and half-step252 f1, displacing width242 to f2 and
leaving six words different. Removing both while keeping the meaningful named
inputs reproduces the ordinary seven-word residual. Both games exhibit the
same two removal outcomes.

The two final complete owners remain exact: DDS1 has 566 matches and DDS2 has
715. The usual both-game full retail builds and repository tests are required
in addition to these source and owner checks before publication.

## Integrated validation

Independent semantic review approved the frozen source hashes
`83e58fc3ab7cc0dd94df65fb9a9a1eb78b35b5dbb531b9f4de2a45fc4c56daf1`
(DDS1) and
`63b05462c72aba39333fe58a47142fbe9fb976cb714932bdd20576eae1dd5215`
(DDS2). Both complete owners pass with zero differences. Both full retail
executable SHA-1 checks pass, all 442 tooling tests in 117 batches pass, and
all 80 retained-C units compile. The pair contributes 3,576 exact bytes.
Private binaries, compiler captures and removal controls are not part of the
source commit.
