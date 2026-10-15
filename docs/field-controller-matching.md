# Field controller: action joins, complete choices and final real uses

The field controller pair is DDS1 `func_0012FF48` (4,428 bytes) and DDS2
`func_00132540` (4,864 bytes). They share the input/collision/camera pipeline,
but DDS2 adds guide-position interpolation, a movement-sound latch, a fresh
quarter-speed heading query and secondary-object pose updates. They are not
identical twins. Both use the established `FldAreaWork` and `ObjectTransform`
owners, preserve the full position W lane, and publish only the established
32-byte prefix of each 48-byte exported transform buffer.

## Keep each action complete until the compiler merges it

Native discrete camera turns share a snap-and-commit tail. Writing one early
source join after two field stores loses the freshly computed scalar at first
CSE: the joined call reloads state. Giving that join a new float local creates
a separate value home and still differs. Each button arm instead performs its
complete action: add/subtract 88 degrees, publish the new camera target, call
cardinal snapping, request recentering and publish the opposite target heading.
First CSE can consume each arm's actual state update. Later cross-jumping
merges the identical completion code, reproducing the shared native call.

The held-button graph also matters. Test the first button, then its opposite:
when both are held, fall through to analog control. Test the second button in
the first-not-held arm. Rechecking the first button after those results are
known adds a real extra native read/branch. A shared update label represents
the actual completion phase. The near-camera/mode-specific highlight-clear
join remains separate from the far-distance clear call.

## A native constant before a call can come from a later complete choice

Movement speed is 32 when the field mode is enabled and 25 otherwise. A default
32 before the location callback plus a one-sided 25 override emitted eight
bytes less than DDS1 retail. Its first CSE eliminated the subsequent state
base low-sum and reused the preceding callback-argument base through the join.

The complete ordinary `if (mode != 0) speed = 32; else speed = 25;` preserves
the state address at the two-predecessor join. In the recorded DDS1 control,
first CSE keeps low-sum uid1437/base481; the old one-sided form had removed
uid1431 and reused base470. Pass 15 (`ce`) then moves real constant-32 uid1423
before the conditional, and pass 17 (`sched`) moves it before the location
callback. The final native constant placement and base rematerialization both
match. Source order must not be inferred directly from scheduled assembly.

## Reset lifetimes explain an apparently reversed store pair

The final two-word residual was the tilt reset: the source reset the float
angle and then the active flag, while native stores those values in the
opposite emitted order. A read-only observation of the pinned compiler's
DDS2 pre-reload scheduler captured the actual comparison: float-store uid400
and flag-store uid405 tie on priority 1, cost 2, tick 0 and zero dependencies.
The flag store has register weight -1 because it owns `REG_DEAD` for their
shared state base; the float store has weight 0. The comparison picks the
flag store first.

The ordinary reset lifecycle disables the active state, then resets the stored
tilt. Its float store becomes the final real use of the existing state base;
no artificial use, variable, qualifier or dependency is added. The scheduler
then emits native float-first, flag-second. This source matches both titles.
The observer's 17,818 events are qualified by byte equality of all 28 declared
compiler/assembler artifacts. This establishes a compiler-state requirement,
not a unique reconstruction of the original line order.

## Data and SDK qualification

The axis and complete 16-entry direction table are real local initializers.
Their compiler-owned native pools replace the corresponding two retained
`INCLUDE_RODATA` entries. The earlier mode-1 axis must precede those pools;
otherwise correct table bytes are duplicated and full-unit layout fails.

Four legacy input-address-only SDK VU stores are retained under a finite
pinned-emission audit. They do not declare output memory and are not a portable
C/asm contract. Actual initial position, conditional refreshed position,
quaternion consumers and loop/branch joins must be checked whenever source
changes. All three vector arrays are explicitly aligned; the axis type itself
has four-byte alignment, while its actual audited stack slot is 16-byte
aligned. Mutable collision XYZ is a separate three-float object. Position
fetch sets W=1, and no scalar update overwrites that lane before a full setter.

DDS1's truthful-output-only control still emits 4,428 bytes but differs in 788
instruction words. Its complete `.rodata` remains byte-identical; the checker's
unreferenced-data diagnostic is an attribution consequence of changed code,
not changed table values. Ordinary no-dump and RTL-probe objects reproduce all
allocated and relocation sections exactly. Final whole-owner checks, both
retail hashes and an updated emitted audit remain the acceptance gates after
integration or semantic renaming.

## Escaped guide-coordinate outputs

DDS2's guide query writes one float through each of its two output pointers.
Keep those real output objects as `f32 guideX[1]` and `guideZ[1]`, passed by
array decay and consumed through element zero. The pinned compiler leaves
uninitialized alias metadata on the address-taken scalar form: changing the
model scale's primary type exchanges zero and an invalid alias number on
the two guide loads and changes twenty words in the interpolation clusters.
The one-float buffers give all four output reads the normal float alias set
and preserve the native `sp+0x90` / `sp+0x94` layout and 4,864-byte body.
The callbacks, arithmetic and publication epochs are unchanged.
