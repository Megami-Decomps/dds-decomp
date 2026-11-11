# Particle instance advance in both games

`billAdvanceParticleInstances` is exact at DDS1 `0x0029FB48` and DDS2
`0x002E1BB0`, 1,816 native bytes each. This continues tge-was-taken's retained
reconstruction in `6087e4a3ba`. The changes are ordinary C around the existing
shared COP2/MMI helpers; no helper, type, flag, constraint or arithmetic changes.
Exact output does not establish the original source spelling.

## Configuration and state lifetimes

Resolve the resource, lifetime, entry array and asset before capturing the
initial item count, following the matched cell/emitter family in the same
owner. The native count load occupies the lifetime-guard delay slot at DDS1
`0x0029FBCC` / DDS2 `0x002E1C34`.

Capture acceleration, gravity and respawn before initializing the two private
radial vectors. DDS1 loads acceleration at `0x0029FCC0`, gravity at
`0x0029FCC8` and respawn at `0x0029FCCC`, before the identity stores at
`0x0029FCD0` / `0x0029FCD8`; the corresponding DDS2 addresses are
`0x002E1D28`, `0x002E1D30`, `0x002E1D34`, `0x002E1D38` and `0x002E1D40`.
These are genuine snapshots, retained through the particle loop. The moved
reads cross only initialization of distinct private stack vectors, no calls
or external writes. The guarded-out path now also reads gravity, as retail
does, while performing no particle work.

Use real waiting (`-1`) and inactive (`-2`) state locals for both retirement
selection and the age comparisons, as the matched cell/emitter routines do.
Initialize the separate running countdown after retirement selection on the
positive-count path. Every path reaching the loop has all three state values
and the countdown initialized. The initial count remains separate from the
mutable countdown. The existing frame snapshot is retained: a reduction
control proved removing it was unnecessary.

All motion formulas, floating-point associations, RNG calls and configuration
read boundaries during spawning remain unchanged. In particular, the shared
half-acceleration term affects both spin and rise, while gravity contributes
its separate `gravity * time * time * 0.5f` term. Skipped, spawned, expired and
live entries still advance one entry, thirteen positions and thirteen colors.

## Qualified stock compiler evidence

The final complete owners have ordinary/probed assembly equality after only
normalizing the scratch `.file` pathname, all six allocated sections agree,
and resolved allocated relocation contracts agree: 3,340 in DDS1 and 3,933
in DDS2. Stock RTL dumps were collected through the regular pinned compiler
wrapper. No debugger or patched compiler was used.

In final DDS1 `00.rtl`, insn 45 captures initial count pseudo 94 after the
resource/entry/asset loads. Instructions 160, 163 and 166 capture acceleration,
gravity and respawn before memory-clobbered radial stores 170 and 174. State
pseudos 99 and 100 are defined by insns 186 and 189, consumed by retirement
selection 193/198 and age tests 216/225. Insn 202 then initializes running
count pseudo 92. Thus the extra state names hold real values used on both
paths, rather than dummy constants or artificial register lifetime extensions.
The `20.greg` dispositions assign acceleration/gravity pseudos 107/108 to
hard FPRs 60/59 (`f28`/`f27`), and initial count 94 to GPR 6. The running
count is separately spilled at SP+0xA4 in the final code.

The DDS1 baseline has 22/454 differing words at native size. The matched
family state form with respawn still between identity stores leaves exactly
two differing words at +0x184/+0x188. Capturing respawn with the motion inputs
restores their native order. Moving countdown initialization back before
retirement selection produces 25/454 differences in the frame-direct control.
Both negative controls, the baseline and the motion/count control have the
same complete assembly/allocated-section/relocation parity qualification.

## Finite existing COP2 contract

The four legacy input-address-only stores write first color vector at SP+0,
second color vector at SP+0x10, and width twice at SP+0x30. The two color
stores dominate all later color-vector COP2 loads. Each live row's first width
store precedes its width load; the transformed-width store precedes both ring
loops' loads. No value from these objects is read as a cached C scalar, and
no address escapes to a callee. Every destination is sixteen-byte aligned.

Keep the ordinary memory-clobbered radial identity stores. They establish all
four lanes at SP+0x40/+0x50; each live row writes its radius to lane Y before
the actual COP2 reload. Direction at SP+0x20 has initial Y/W and per-row trig
X/Z writes before consumption. The generated VU pipeline retains the center
in vf12 while building both rings and keeps color scaling in vf11. Existing
hard-coded scalar scratch uses have no unrelated live value at those sites.
External position/expiry stores retain their broad memory effect.

This audit qualifies these exact emitted paths under the pinned compiler,
not a portable memory guarantee for the legacy macros. Compiler, source,
control-flow or macro changes require a fresh audit. No new NOMEM use or
weakened clobber was introduced.

## Release checks

- Complete DDS1 owner and owned data: **501 match, 0 differ**.
- Complete DDS2 owner and owned data: **588 match, 0 differ**.
- Both complete retail builds pass the configured SHA-1 checks.
- **445 tests in 118 batches: OK**; `git diff --check` is clean.
- Independent semantic review verifies snapshots, state uses, preserved motion
  and RNG behavior, twin equivalence and the finite COP2/scratch contract.

The compact receipt is
`docs/compiler-evidence/particle-instance-advance/qualification.json`.
Full source controls, stock dumps, plain objects and validation logs are
preserved in the local investigation evidence.
