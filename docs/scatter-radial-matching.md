# Scatter effect matching

`func_00170F28` (DDS1) and `func_00178B80` (DDS2) each match 1,512 bytes.
The paired routines advance radial particles, build a six-vertex strip for
each active particle, update optional duplicate work, and submit the pool.

## State and bounds

The existing 0x74-byte radial parameter layout now exposes the signed trailing
fade duration at 0x38, float height and angle damping at 0x40/0x48, and the
pulse flag/angle at 0x60/0x64. No shared owner or layout changes.

Particle age, active duration and end-age tests are signed. The fade-in and
fade-out windows are unsigned, including their float conversions. Age zero
reseeds the particle; negative ages clear its six colors. Active motion updates
radius, angle and height and damps the two steps. The pulse changes geometry at
the first two trailing frames without committing those temporary offsets to
particle state. Respawn and duplicate-work gates use the entry age; the final
increment reloads the particle's live age after callbacks.

The pool provides 0x60 bytes of positions and 0x18 bytes of colors per particle.
Three iterations write vertices j and j+3 and colors j and j+3, covering exactly
six entries. The constructor allocates duplicate handles by ceiling division;
normal construction clamps a zero particles-per-group factor when creating
children. The existing shared-duplicate path has its own native assumptions.
The updater adds no new bounds, zero-division or allocation-failure guards.
As in retail, age zero can reach the duplicate callback before the middle-point
scratch is produced when duplicateStartAge is nonpositive; no initialization
is invented for this inherited path.

## VU pipeline and source model

Eight complete 16-byte scratch vectors preserve the native aligned loads and
stores. The builder follows the radial curve, normalizes its tangent, offsets
the strip center, and forms the transverse direction with a cross product.
The middle iteration supplies the duplicate-work position. The real vector
copy primitive carries its existing memory effects. All other primitives are
existing SDK-style VU operations; no new assembly or artificial dependency was
added. The input-address-only stores remain a legacy pinned-compiler contract,
not a general-purpose memory barrier. Outputs have no scalar C readbacks.
Normalization and cross products use XYZ, and the draw packet packs only XYZ.
Duplicate callbacks can propagate all four lanes through the existing kind-6
extended dispatch and vector setters; their W behavior is left native. The
two-argument callback declarations retain the established native register ABI
and existing consumer practice; no provider interface is changed here.

The first draft already had the exact code length, frame and overall control
flow. Remaining differences identified the scalar local groups, sequential
color clearing, and the order of the pulse/damping snapshots and radial state
producers. Grouping each configuration value with its associated flag and
updating radius before angle/height gives the native schedule. No redundant
reads, guards, volatile fields, register constraints or compiler flags are used.

## Spin variant

`func_00171B28` (DDS1) and `func_00179780` (DDS2) each match 1,524 bytes.
Both first reconstructed drafts match using the radial variant's reviewed
fade, pool and duplicate-work model. Existing 0x64-byte spin parameters and
0x20-byte particles require no field or layout changes.

The spin variant copies the particle's seeded XYZ axis and builds the native
axis-angle matrix for each angular sample. It applies the complete four-lane
matrix operation to a radius vector, then uses the same tangent/transverse
strip construction. Radius and angle advance only during active motion;
angle-step damping and the two-frame trailing angle adjustment remain native.
A ninth full scratch vector holds the axis, covering the provider's quadword
load. Its formulas consume XYZ and explicitly form the basis W lanes and
translation row; the caller retains the native four-component operation. The same six-vertex/color bounds and
inherited duplicate-state assumptions apply. No helper, primitive, header,
compiler flag or artificial producer is added to make either body match.

## Radius-damped ring allocation

`effScatterCreateDampedRing` (DDS1 0x00173B48 / DDS2 0x0017B7A0)
matches 560 bytes per title. The existing 0x194-byte instance owns a contiguous
array of 0x28-byte particles. Its 0x13C-byte serialized parameter copy starts at
0x40 and ends exactly at the particles pointer at 0x17C. The allocation size
uses these existing owner types, and allocationHandle stays SdfMemBlock*.
The retain API returns the payload address; the destructor releases the draw
resource and then the allocation descriptor.

The prior struct-assignment body, with the current pointer owner, differed in
three metadata stores. Its typed block copy did not constrain the later
pointer store, while color and scale had anti-dependencies on the copy tail.
Standard memcpy of the complete serialized parameter block preserves every
byte, including padding, and records the actual generic byte-copy contract.
The descriptor-pointer store then has the same tail-copy anti-dependency and
retains native color/scale/allocation order. No field is weakened to an integer,
and no artificial memory clobber or extra runtime operation is introduced.

The constructor copies the native matrix, creates the draw object, optionally
attaches its resource, and initializes particle ages from the RNG. It clamps
the copied delay limit using the native signed test, then uses an unsigned
modulus. Allocation failure, oversized counts and source matrix alignment
retain their existing native preconditions.

## Basic ring allocation

`effPcpScatterCreateParticleInstance` (DDS1 0x001730D0 / DDS2 0x0017AD28)
matches 536 bytes per title. The 0x138-byte parameter block occupies
[0x40, 0x178) inside the existing 0x18C-byte owner, followed by the runtime
fields and its contiguous 0x28-byte particles. The constructor uses the same
complete serialized-parameter memcpy and current SdfMemBlock pointer owner.
The existing memcpy declaration now precedes this earlier consumer.

The constructor interprets the delay modulus with a signed nonpositive test,
clamps the local value to one, and uses unsigned RNG remainder for initial
negative ages. It leaves the copied randomDelayRange unchanged. Matrix copy,
draw creation, optional resource attachment, ownership and native unchecked
count/alignment assumptions retain the established contracts.

## Two-color ring allocation

`effScatterCreateTwoColorRing` (DDS1 0x00174680 / DDS2 0x0017C2D8)
matches 512 bytes per title. The existing 0x19C-byte owner contains its
0x144-byte parameter block at [0x40, 0x184), then runtime fields, followed
by 0x28-byte particles. The complete byte copy and pointer owner retain the
same reviewed contracts. The shorter code reflects its four-byte copy tail;
the damped variant has a 28-byte tail.

This variant initializes the instance age to zero and normalizes a signed
nonpositive copied delay range to one, preserving the writeback and unsigned
RNG remainder. All five initial metadata stores, matrix copy, drawable setup,
optional resource attachment and particle-age initialization remain native.
No shared header, owner layout or primitive changes are required.

## Plain ring allocation

`effPcpScatterCreatePlainInstance` (DDS1 0x00175230 / DDS2 0x0017CE88)
matches 496 bytes per title. The existing 0x13C-byte owner contains its
0xE8-byte parameter block at [0x40, 0x128), then runtime fields, followed
by 0x28-byte plain particles. Their age is at offset 0x0C; the other ring
constructors use the separate particle owner whose age is at offset 0x08.
This restores the prior C body with the canonical SdfMemBlock pointer owner
and the same complete serialized byte-copy contract as the other restored
constructors.

The instance matrix starts as identity. Drawable creation uses the source
count and word at 0x20, and copies the source word at 0x10 into the drawable.
The signed nonpositive delay test normalizes only the local modulus; the
copied field remains unchanged. This owner has no shared instance-age field.
Optional resource attachment and negative RNG-remainder particle ages retain
the native behavior and existing allocation preconditions.

## Validation

The radial bodies compare exactly against all 378 retail instruction words
per title; the spin bodies compare exactly against all 381 words per title.
The damped-ring constructors compare exactly against all 140 words per title;
the basic ring constructors compare exactly against all 134 words per title,
the two-color constructors against all 128 words per title, and the plain
constructors against all 124 words per title. Both installed canonical whole-unit checks report 75 matches
and zero differences. Both retail SHA-1 checks and both dev-ELF gates pass,
followed by a no-work repeat. Validation uses the pinned current source and
a byte-identical private copy of its canonical header in the established
b0183e08-based checkout; publication retains the ordinary eff.h include.
No retail data, disassembly, objects or compiler dumps belong in this change.
