# Thunder Bezier group updates

The paired group updates are `func_001681C0` (DDS1) and `func_0016FE18`
(DDS2), each 5,988 bytes. They construct two joined cubic Bezier segments,
advance a three-vector ribbon history, build an eight-vector end cap, fade its
color, and update the attached endpoint event.

## Owned data and call boundaries

- `EffGroupParams` remains 0x50 bytes. The recovered scalar fields occupy the
  previously opaque parameter prefix; all existing constructor offsets remain.
- `EffBezierSlot` agrees with `effEvent.c`: seven xyz control points, a first
  control-point index at 0x54, parameter at 0x58 and increment at 0x5C.
- The curve begins at 0x1C in the 0x80-byte `EffGroupSlot`. Its stepping state
  therefore retains offsets 0x70/0x74/0x78, followed by the event node at 0x7C.
- Stack vectors explicitly require 16-byte alignment. The ribbon is one real
  48-byte object, consumed as exactly three quadwords by the history helper.
- The resource allocator supplies 0x80 bytes for the eight end-cap vectors.
  The final event placement remains a separate complete 0x30-byte record.
- The shared draw consumer packs only XYZ, discarding W. Normalization uses
  XYZ only, so the inherited unused fourth-lane values do not affect geometry.
- The axis-angle, actor-muzzle, Bezier and packed-record declarations agree
  with their existing providers. The color-state reset uses the existing
  resource prefix, not a new owner.

The constructor's delay/subdivision clamps, valid mode range and meaningful
frame parameters remain runtime preconditions. No new range guards, scratch
reads, padding operations or extra fourth-lane initializers were added.

## Why the VU source form matters

The new `*_EXTENDED` macros are additive forms of seven existing register-only
VU operations. They emit the same instructions, keep volatility, and use
empty extended-asm operand sections. There are no artificial instructions,
reads, outputs or clobbers. The SDF convention still owns the implicit VU
registers, ACC and Q; the C compiler does not allocate that register bank.
Existing users of the basic-asm forms are unchanged.

In this compiler, basic asm is `ASM_INPUT`; explicit extended asm is
`ASM_OPERANDS`. Visiting a top-level volatile `ASM_OPERANDS` flushes local
CSE's expression table. A `PARALLEL` containing an asm and a memory clobber
has a different path. This difference explains the reconstructed pointer
lifetimes, including the repeated rotation output address and the ribbon
argument rematerialized after the right-vector store. The original candidate
retained one extra pointer and needed a 512-byte frame; the audited source
uses the retail 496-byte frame. Changing register ordering was not a solution.

The source-level explanation is corroborated by probes of the actual pinned
EE compiler. Upstream source references:
[cse.c](https://github.com/gcc-mirror/gcc/blob/2f93c5c3551b6b3c11a774ae6d42220eff1f4502/gcc/cse.c#L5964-L5968),
[haifa-sched.c](https://github.com/gcc-mirror/gcc/blob/2f93c5c3551b6b3c11a774ae6d42220eff1f4502/gcc/haifa-sched.c#L3534-L3578),
[flow.c](https://github.com/gcc-mirror/gcc/blob/2f93c5c3551b6b3c11a774ae6d42220eff1f4502/gcc/flow.c#L5601-L5635).

## Legacy store restriction

These two audited bodies use the existing `VU0_STORE_VF_UNCLOBBERED` form.
Sony-attributed 2001 libVu0 has the same input-pointer-only convention for
real `sqc2` writes, including
[UnitMatrix](https://github.com/ps2dev/ps2sdk-ports/blob/d7b2d8a803294353a2444ec6b9817deb09f605a7/ode/ode/src/libVu0.c#L344-L357).
This is historical precedent, not proof of Atlus's exact original macro names.

**This form is not a generally sound memory barrier, even on EE GCC 2.96.**
Isolated controls demonstrate stale readback after conditional stores and
loads hoisted out of loops. Volatility and the local CSE flush do not provide
an all-pass memory contract. Do not replace the ordinary memory-clobbered
store globally or use this as a portable inline-asm abstraction.

For these specific bodies, review checks the actual post-store scalar
readbacks, real call boundaries and complete vector consumers. The repeated
rotation stores retain their scalar reloads; each ribbon is fully written
before the history call; cap readback follows the history call and actual
memory-copy operations. Changing those expressions, branches or loops
requires a fresh memory-access audit as well as an exact-match check.

## Adjacent vector data alignment

The switch table contributes 20 bytes of rodata. Its old ASM fragment also
carried trailing padding; removing it exposed the next unit's underaligned
quadword constant. `D_003A0EF0` / `D_00414610` are real (0, 1, 0, 0)
quaternions loaded with `lqc2` by the staggered-pulse updater. Their typed
`const f32[4]` owners explicitly require 16-byte alignment. This preserves
both the hardware load contract and the retail addresses of subsequent data;
it does not add a padding array or alter either consumer body. A plain array
still gets only eight-byte alignment from this compiler.

## Verification

Canonical whole-unit checks report 63 matches and zero differences for each
version, including the two 5,988-byte bodies and their switch tables. Existing
functions in both units remain exact. The two affected miscellaneous-effect
units each retain 358 matches and zero differences. Both retail SHA-1 checks
and both dev-ELF build gates pass, followed by a no-work repeat build.

No retail binaries, disassembly or compiler dumps belong in this change.
