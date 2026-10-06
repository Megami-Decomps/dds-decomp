# Scatter strip updater matching

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

## Validation

The radial bodies compare exactly against all 378 retail instruction words
per title; the spin bodies compare exactly against all 381 words per title. Both installed canonical whole-unit checks report 71 matches
and zero differences. Both retail SHA-1 checks and both dev-ELF gates pass,
followed by a no-work repeat. Validation uses the pinned current source and
a byte-identical private copy of its canonical header in the established
b0183e08-based checkout; publication retains the ordinary eff.h include.
No retail data, disassembly, objects or compiler dumps belong in this change.
