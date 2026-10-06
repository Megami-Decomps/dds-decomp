# Thunder Bezier group updates

The paired group updates are `func_001681C0` (DDS1) and `func_0016FE18`
(DDS2), each 5,988 bytes. They construct two joined cubic Bezier segments,
advance a three-vector ribbon history, build an eight-vector end cap, fade its
color, and update the attached endpoint event.

## Owned data and call boundaries

- `EffGroupParams` remains 0x50 bytes. The recovered scalar fields occupy the
  previously opaque parameter prefix; all existing constructor offsets remain.
- `EffSegmentedBezierSlot` is shared with `effEvent.c`: seven xyz control points, a first
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

## Chain-fragment continuation

`func_001673D0` (DDS1) and `func_0016F028` (DDS2) each contribute another
1,700 bytes. Their caller passes the preceding segment's last five-vector
row. The continuation copies that row, advances a randomized sinusoidal
path, builds strip rows and selects each join edge from the new direction's
dot product with the preceding side vector. The observed anchor is seed[3],
not the middle vertex.

The shared fragment parameter owner exposes three previously opaque floats:
wave amplitude at 0x2C, band width at 0x44 and added edge width at 0x4C.
Its size remains 0x54. The base width vector is filled from the owner; the
outer and negative-outer vectors are then derived from it. Keeping this
actual producer/consumer flow, rather than separate scalar caches, preserves
the native initial XYZ stores without a statement-order search.

For positive halfLife h, the constructor requests kind 4 with 2h-1 segments.
The real allocator expands that to 5(2h-1)+5 = 10h quadwords. Each of h
iterations writes two five-vector rows, filling exactly that allocation.
The seed belongs to the preceding separately allocated segment, which the
outer caller updates first. Both vertices-5 and vertices-1 select within
the last completed row. Valid constructed groups, h >= 1 and a valid fragment
index remain prerequisites.

This pair has its own scoped SDK-store/readback review. Scalar distance and
facing use XYZ reductions, normalization and rotation do not mix input W into
XYZ, and kind-4 drawing reaches the same XYZ-only packet builder. No new
fourth-lane initialization or general store-safety claim is warranted.

The Thunder declarations use the existing shared EffSegmentedBezierSlot
owner, replacing the superseded local curve types after that owner was
promoted on main. The old Bezier update bytes are unchanged.

## First-segment generation

The first-segment counterparts are `func_00165758` (DDS1) and `func_0016D3B0`
(DDS2), 1,572 bytes each. They use the work's own start vector and generate
every first row rather than copying a predecessor seed. The same allocator
and two-row loop still account for exactly 10h quadwords. The helper body
matches on its first reconstruction using the already recovered owners and
VU operations; no additional primitive or parameter fields are needed.

Removing the seed branch naturally lets the compiler use a countdown loop
and hoist its phase increment. The source remains an ordinary forward C for
loop. Its final join reads still stay within the last completed row, and the
XYZ/W-lane conclusions above remain valid.

At h=0 this helper performs no history writes or seed access, but setup,
including distance/h, still occurs before the zero-loop guard. That native
ordering is preserved; positive h remains necessary for meaningful geometry,
and continuation segments still require a nonempty predecessor row.

## Dual-strip generation

`func_001661D8` (DDS1) and `func_0016DE30` (DDS2) each match 1,592 bytes.
They partition each row into a kind-0 core strip with two vertices and a
kind-1 outer strip with three. The existing parameter owner exposes the real
float coreWidth at 0x3C without changing its 0x54-byte layout. The native
association (coreWidth + edgeWidth) + bandWidth is retained.

For h steps, kind 0 allocates 2(2h-1)+2 = 4h quads and kind 1 allocates 6h.
The loop fills exactly four core and six outer quads per iteration. Its
vertices-3/vertices-1 join choices are the endpoints of the last completed
outer row. Both renderer kinds reach the XYZ-only packer, so the earlier
unused-W and scoped SDK-store conclusions apply. Zero-count setup remains
unchanged. Both complete first drafts match without source variants.

## Per-spark generation

`func_00164BA8` (DDS1) and `func_0016C800` (DDS2) each match 1,384 bytes.
The indexed spark owns one kind-0 system with exactly one cell. Two two-vector
rows per iteration fill its 4h-quadword capacity, with vertices-2/vertices-1
joining within the last row. The same XYZ-only consumer and zero-count setup
restrictions apply.

The local SparkParams recovery gives the endpoint quadwords at 0x10/0x20
complete four-float objects, exposes wave amplitude at 0x3C and width at 0x4C,
and asserts the unchanged 0xA4 size. Destination selection directly reads the
indexed spark's system field after preparing the width vectors; it does not
keep an unnecessary alias for the full spark record. That natural entry
boundary reproduces the remaining address and scheduling details.

## Verification

Canonical whole-unit checks report 67 matches and zero differences for each
version, including the two 5,988-byte bodies and their switch tables. Existing
functions in both units remain exact. The two affected miscellaneous-effect
units each retain 358 matches and zero differences. Both retail SHA-1 checks
and both dev-ELF build gates pass, followed by a no-work repeat build.

No retail binaries, disassembly or compiler dumps belong in this change.
