# Matching the paired event-unit transition managers

The DDS1 `func_00220910` and DDS2 `func_0023B480` reconstruct the complete
3,968-byte event-unit update in each game: model RGB/alpha tracks, target
validation and distance falloff, quaternion direction interpolation, both
colour tracks, and the pair of auxiliary model values. The pair contributes
7,936 bytes of matching C.

The important differences came from real source boundaries: which state a
stage produces, where a value is consumed, and which control-flow paths define
a default. The final source uses ordinary C around the shared VU/MMI primitives.

## Storage and the unpack read contract

`EvtUnit` keeps its physical offsets and complete allocation extent: 0x170
bytes in DDS1 and 0x1D0 bytes in DDS2, with four-byte alignment. Union views
expose the decoded colour words, direction vector and unsigned frame counters.
The extra DDS2 slot storage moves the trailing counters by 0x60; it does not
change the common colour/vector prefix.

Battle units hold this same complete `EvtUnit` through `BtlUnit.ext` (+0x320
in DDS1, +0x340 in DDS2), not a separate battle extension prefix. Its +0x8C
`owner` is the SDK's 0x38-byte `MdlCtx`: model data is `owner->inner`
(`SdfModel`, +0x18), and the active motion is `owner->first` (`Motion`, +0x1C).
Motion sampling uses `currentFrame`, `frameStep`, `frameCount` and `state`;
lighting belongs to `SdfModel.lighting`. Shared SDK structs replace the old
owner/data prefix overlays without changing either event-unit allocation.

The SDK motion sampler returns `void`, and the random modulo helper returns
`u32`. The DDS2 random-frame wrapper and the paired eligibility getters remain
ASM after the canonical-owner cutover: honest candidates are parked under
`build/parked`, with the exact remaining differences recorded in `qp_todo`.
The getters differ only in a native byte load versus the real owner's word
flag load; no byte alias or false return width is used to force a match.

## Battle scheduler ownership

The scheduler allocation has a 0x70-byte `BtlRuntimeTask` header, separate
from the queued actor's 0x170-byte `BtlTask`. Its two 0x10-byte
`BtlTaskCondition` subobjects start at +0x00 and +0x10. The evaluator receives
the appropriate subobject directly; it does not treat the end condition as
another complete task. The condition kind selects a signed count, a native
64-bit task handle or owner ID, or a 16-bit task kind.

Arguments begin immediately after the scheduler header when the requested
payload size is positive. The argument getter returns the opaque allocation
address, not a sound-specific packet view. Startup and finish slots use C89
unprototyped callbacks because the task kinds have different packet types,
including callbacks that need no arguments; no function-pointer casts are
needed. Poll callbacks return a 32-bit completion status.

The DDS2 camera constructors must retain their native task IDs 0x2B and
0x2C (DDS1 uses 0x28 and 0x29). Recovering these missing stores restores the
retail code without scheduling or register-allocation tricks. Their optional
actor input is a queued `BtlTask`, whose +0x18 unit supplies the +0x108 owner
ID. Scheduler registration and optional prerequisites retain real 64-bit
generation handles; they are not pointer-valued booleans to narrow.


## Packed-colour conversion read contract

The additive `EE_MMI_RGBA_UNPACK_READONLY` has the same instruction template
and fixed scratch register as `EE_MMI_RGBA_UNPACK`. Its only memory operation
reads one packed word. Declaring that word as an `m(*(const u32 *)src)` input
models the actual dependency without invalidating unrelated unit fields as
though the operation wrote arbitrary memory.

The manager's twelve unpack inputs are initialized element-zero words of
local four-word conversion buffers. Each pack writes its result buffer and
the following consumer reads that result. The unused lanes are never read.
The new primitive requires a live, normally aligned u32 object, a stable
side-effect-free source expression, and a binary32 scale or explicit scale
bits. It preserves the existing implicit VU-register convention. The old
broad-clobber macro remains available for its existing consumers.

This precise read contract matters for ordinary C optimization. The colour
countdown can remain available across conversions until its genuine wrapping
decrement, and the compiler can retain unrelated flags while preserving the
stores that feed each unpack.

## Separate target distances

Current-target and previous-target distances are different values. Reusing one
C variable for both computations combined their allocation statistics: eight
references over a live length of 24 gave priority 10000. That exceeded each
near-radius load's four references over length 10, priority 8000, and selected
the wrong floating-point homes.

Giving the two actual distances separate lifetimes produced four references
over length 12, priority 6666, for each. The radius then takes f1 and distance
takes f3, as in retail. No declaration-order search or register constraint is
needed.

## Keep the two default paths

An inactive target transition and an active transition with zero duration both
use a full blend weight, but they are separate control-flow cases:

```c
if (unit->flags & 0x180000) {
    if (unit->transitionFrameCount) {
        transition = (f32)(u16)unit->transitionElapsed /
                     unit->transitionFrameCount;
    } else {
        transition = 1.0f;
    }
    /* Apply fade-out direction and advance completion state. */
} else {
    transition = 1.0f;
}
```

Preinitializing `transition = 1.0f` before the outer condition obscured this
boundary. GCSE removed the redundant default arm and its jump; the second CSE
pass could then replace a later duration zero extension with the earlier
32-bit value. The result lost two retail masks.

With the defaults kept on their real paths, the live ratio/default join starts
a fresh CSE region. The raw 16-bit duration survives through the join and is
extended again for the later guard. Combine and reload consequently reproduce
the retail unsigned load, both masks and the intervening branch. No artificial
use of duration, extra arithmetic or compiler barrier is required.

## Refresh the direction cache after the transition writes

The target-transition stage updates stored flags. The direction stage should
take its snapshot after those updates:

```c
if (unit->flags & 0x180000) {
    /* Update elapsed state and clear completed transition bits. */
} else {
    transition = 1.0f;
}
flags = unit->flags;
```

Caching `flags` before the transition and updating it only on completion kept
that C variable live alongside a separate PRE temporary for the stored flags.
The two roles received the opposite registers from retail.

The post-transition refresh makes the final read part of the same stored-state
expression as the completion writes. PRE carries that expression through both
clears. Its merged updated value receives register 5; the original transition
snapshot receives register 7. The direction cache is no longer live inside the
producer stage and can share register 5 at the merged refresh.

## A delay slot does not prove source ownership

The final two instruction swaps were in the previous-target colour branches.
The previous falloff weight belongs inside the branch that uses it:

```c
if (previous) {
    factor = previousWeight;
    /* Convert and blend the previous target's colour. */
}
```

The generated floating-point move still appears in the conditional branch's
delay slot. Writing that assignment before the `if`, simply because the move
executes unconditionally in the final assembly, gave the wrong scheduling
input.

With the assignment inside the successor, sched2 issues the scale constant and
weight move at cycle zero, then the colour store and scratch-address calculation
at cycle one. Delay-slot scheduling subsequently donates the weight move to the
branch. With the assignment outside, the address calculation could issue beside
the scale constant at cycle zero, ahead of the store. The branch-local source
therefore recovers the exact retail order without inventing a store-to-address
dependency.

This mechanism repeats for both colour channels and transfers unchanged to
DDS2.

## Verification

Both complete canonical units report `35 match, 0 differ`:

```sh
python tools/check_unit.py src/dds1/event/evtUnitManager.c
python tools/check_unit.py src/dds2/event/evtUnitManager.c
ninja -j4 dds1 dds2 dds1-dev dds2-dev
```

Both rebuilt retail executables retain their expected SHA-1 values:

- DDS1: `6d18898e2724bf1d145392766e8ba1e678487419`
- DDS2: `9be91ee1b4a535a4cb6ec89237b5a6ba41be2add`

Both development ELF targets also pass, and a repeat build is a no-op. The
canonical checker and retail hashes are the match evidence; instruction
sequence alignment was used only to diagnose intermediate candidates.
