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

## Randomized multi-band cells

`func_00166C18` (DDS1) and `func_0016E870` (DDS2) each match 1,108 bytes.
The kind-2 constructor allocates 6(2h-1)+6 = 12h quadwords per cell. Each
iteration emits two six-vector rows ordered from positive outer width through
the inner pair to negative outer width. The vertices-6/vertices-1 join choices
remain within the last completed row.

The local 0x48-byte CellParams owner now exposes its complete origin quadword,
travel distance scale at 0x14, and width increments at 0x30/0x38/0x40. Direction comes
from the cell's existing independent random XYZ components and is not
normalized. The per-step distance sample is evaluated once before copying it
to XYZ. The randomized turn uses the existing view axis and 70-degree range.

The initial camera-cross-direction normalization and side store are retained
as real VU operations, even though this variant does not subsequently read
that scratch vector. No artificial read is added. Setup and length/h remain
before the zero-count guard, as in retail. The constructor requires h >= 1 and nonzero timing ranges; it does not clamp
those inputs. Length is a scale, not guaranteed total travel, because both the
unnormalized direction magnitude and randomized steps affect distance.

Both kind-2 render dispatchers pass history to the XYZ-only packet builder.
Scratch values written by VU are read back by actual VU loads, without cached
C scalar substitutions. The inherited legacy SDK-store restriction continues
to apply only to these reviewed bodies and their observed readbacks; changed
loops or branches require another audit.

## Vector-cell strip generation

`func_00163780` (DDS1) and `func_0016B3D8` (DDS2) each match 884 bytes.
The separate effCreateThunderCellSystemWork constructor supplies kind-4
history with 5(p+1) quadwords. The generator computes row count from that
system capacity before publishing the active cell count, then binds the
indexed source cell. That source boundary avoids an alias-induced reload.
Each iteration writes one complete five-vector row.

The existing local 0x4C-byte VectorParams layout exposes the origin quadword
and widths at 0x3C/0x44. A single random multiplier scales both widths. Each
row uses three sampled 75-degree Euler angles to perturb its source axis,
then a fourth sample perturbs the cell's negative rotation angle by 70 percent.
The center is computed from the previous placement before the next rotation.

The original axis explicitly has W=0. Placement and output scratch W lanes
retain the original unspecified state; only XYZ contributes to rotation,
normalization and the kind-4 packet consumer. The existing SDK-store limitation
still applies. In particular, the actual next-iteration scalar loads of the
VU-written placement vector and the axis-angle callee's reads are part of this
pair's focused readback audit.

Geometry alone fills its capacity for every constructor u16 perCell value.
The existing symmetric-color initializer has a stricter convention: it uses
half=(rowCount>>1), divides by half before its cell-count guard, and writes
paired rows. Thus p=0 divides by zero during construction, and an even p>0
leaves the last color row untouched. Fully initialized constructor colors
require odd p>=1 unless another producer supplies that row. No new guard or
color-fill change is included here.

## Indexed two-vector strips

`func_001642B0` (DDS1) and `func_0016BF08` (DDS2) each match 748 bytes.
The separate local effThunderWorkCreate uses kind 0 with groupDivisor=0,
allocating 2(p+1) quadwords. The signed capacity >> 1 gives the row count;
each iteration writes its positive and negative width endpoints. It reuses
the recovered VectorParams layout without another field or primitive change.

This variant jitters width by 30 percent, uses half-turn * 0.5 for its Euler
range, and perturbs the source rotation scale by 80 percent. Each row samples
the raw angle RNG value before reading the live source rotation scale. Keeping
that producer boundary avoids holding an earlier scale across the call. The
half-turn * 0.5 association also preserves the compiler's authored float bit;
90 * (half-turn / 180) rounds one bit differently.

The actual axis-vector callee read and loop-carried placement scalar loads
are audited again for these bodies. Scratch W does not enter XYZ geometry,
and the kind-0 renderer uses the same XYZ-only packing convention. The legacy
SDK-store contract is still limited to these checked pinned-compiler paths.

Its kind-0 color dispatcher has a center-row branch and covers both even and
odd p; the preceding kind-4 odd-p restriction does not apply. At p=0 it still
computes a floating reciprocal with a zero denominator before filling the sole
center row. The renderer emits no geometry with fewer than four vertices.
These native setup and edge-case behaviors remain unchanged.

## Spark motion, fades and restart

`func_00165110` (DDS1) and `func_0016CD68` (DDS2) each match 772 bytes.
The update retains frame-entry timing and damping values, and caches each
spark's entry age for its state decisions. The increment path still reloads
the live age after callbacks. Age zero initializes motion and resets the
one-cell system without submitting geometry that frame. Active ages include
the duration endpoint; fade-in takes precedence when the fade intervals overlap.
The unsigned alpha conversion and restart remainder retain their native forms.

The scratch position is an XYZ point; the origin is a complete aligned
quadword. Only endpoint XYZ is rewritten, preserving both parameter W lanes.
The saved render-cell pointer receives the color after geometry, while system
submission reloads the spark's current system handle.

The final five-word scheduling difference came from ordinary endpoint-group
lifetimes. Building lowered XYZ before upper XYZ moves the pre-scheduling
final X/Z uses to
the upper group. GCC's pre-allocation register-pressure ranking prefers those
stores because of their REG_DEAD notes; the resulting machine order is upper
XYZ, lower Y, then lower X/Z. This is a source construction order with identical
observable endpoint values, not a new memory barrier or required publication
ordering. Both verbose scheduler traces and before/after death notes were
checked; no individual-store permutation search or artificial operation was
used. The comparator and weight calculation are documented in upstream
[haifa-sched.c](https://github.com/gcc-mirror/gcc/blob/2f93c5c3551b6b3c11a774ae6d42220eff1f4502/gcc/haifa-sched.c#L3995)
and its [register-weight calculation](https://github.com/gcc-mirror/gcc/blob/2f93c5c3551b6b3c11a774ae6d42220eff1f4502/gcc/haifa-sched.c#L4595).
Exact bytes support this source form without uniquely proving its historical
spelling. Existing timing and geometry preconditions remain unchanged.

## Verification

Canonical whole-unit checks report 71 matches and zero differences for each
version, including the two 5,988-byte bodies and their switch tables. Existing
functions in both units remain exact. The two affected miscellaneous-effect
units each retain 358 matches and zero differences. Both retail SHA-1 checks
and both dev-ELF build gates pass, followed by a no-work repeat build.

No retail binaries, disassembly or compiler dumps belong in this change.
