# Player-follow camera: one scratch scalar, two disjoint phases

The player-follow camera functions are `fldUpdateCameraFollowPlayer` in DDS1
(`0x0012D528`) and `func_0012FA58` in DDS2 (`0x0012FA58`), both in
`field/fldFileResolver`. Each native body is 1,608 bytes, or 402 instruction
words. The reconstruction builds on Opus5's earlier camera draft and analysis,
followed by KiB's semantic, vector-storage, allocation, and qualification work.

## The source property

`cameraScalar` holds two real values in separate phases:

1. Homogeneous W = 1 for the near/world-eye vector.
2. After that publication, the tilt-dependent distance divisor: 3 when tilt is
   active and 1 otherwise, consumed by the two far-position divisions.

The scalar's address never escapes. Its earlier value is no longer needed when
the later phase assigns it, and both branches define the divisor before either
division. The later far-vector W remains its own literal 1, rather than reading
the divisor. A neutral name and phase comments make the distinct roles explicit;
the source does not pretend that a homogeneous coordinate and a distance divisor
are the same semantic quantity.

This is ordinary scratch-storage reuse under the
[matching rules](../CONTRIBUTING.md#matching-rules). It adds no value, consumer,
operation, call, memory access, or artificial lifetime extension. It does not
use register pinning, a dummy read or write, a qualifier or type fiction, a
codegen-only wrapper, or a compiler-option change.

## The qualified allocator cause

With separate locals, the best reconstruction differed in only four register
operands. The early homogeneous value was locally allocated to F23. The later
divisor was a separate global pseudo: both F22 and F23 were available and
already used, neither had a relevant preference, and first-fit chose F22.
Retail uses F23 for the divisor.

A differential control using one scalar for both phases preserved the other
398 words and closed those four differences. Its read-only observation of the
actual EE GCC 2.96 recorded **14,537 events**, with **all 28 ordinary and observed
artifacts byte-identical**, including the complete object, assembly, RTL dumps,
and compiler/assembler logs. Inputs were verified before and after observation.
An independent plain-versus-`-da` comparison also agreed in complete normalized
assembly, all allocated sections, and all 2,634 allocated relocation signatures.
These counts describe that qualified control, not every later compilation.

In that control, the shared scalar is global pseudo 86: six references, three
definitions, two deaths, five crossed calls, and a live length of 170. It is
31st of 38 global allocands. Its early phase overlaps the real near-radius
constant 50 in F22. Sharing one pseudo therefore carries that F22 conflict into
the allocation decision for both phases, even though the later divisor does
not itself overlap the near-radius value.

The decisive `global_alloc` search has no normal, copy, full, or
`someone_prefers` preference. Its first tier finds no already-used register
that is available: F20 through F22 conflict, while F23 has not yet been used
because there is no separate local W allocation. The second tier drops the
already-used restriction, rejects F22, and chooses F23 as the first available
saved FPR.
This is the union of the two phases' interference constraints, not a special
cost preference for F23 or a later reload repair.

After accounting for the removed pseudo number and source-location metadata,
the control's instruction and dependency records from `13.life` through
`17.sched` agree with the separate-local version. No scheduler change is needed
to explain the result. The local-allocation pass changes two constant notes;
the shared pseudo and its interference explain the final register choice.

An immutable numerical-one local was also tested as an alternative. GCSE
propagated its copy into the no-tilt divisor branch to literal 1 before
allocation, so it supplied no surviving F23 preference. Extending that one to
the later far-vector W instead made it overlap the divisor and was not a fix.

## Semantic review boundaries

Independent review must cover more than the last four words. In particular:

- Retain the scene-object slot and reload the node at the native call phases.
- `dds3CameraObjectBasicSetWorldEye` consumes the near vector; it is not a
  getter. Read the tilt state again at the later divisor decision.
- Preserve separate sine/cosine calls, fresh row reads, and binary32
  division-before-multiplication order.
- Keep the aligned, float-only reference buffer distinct from near/far vectors
  passed through the quadword camera APIs. Keep all six geometry scratch slots
  and the full SDK vector copies; do not invent reference or collision W writes.
- Retain the base ray length across the collision call, the 45-unit fallback,
  support-height publication, and distance evolution even when collision is
  disabled.
- Preserve strict comparisons, equality and unordered no-store paths, and
  the native smoothing factors and publication order.

## Integration verification

The final named source passes the complete-owner gates: **193 matches and zero
differences in DDS1**, and **159 matches and zero differences in DDS2**, including
both 1,608-byte target bodies. Independent plain and strict-dump compilations
agree in full normalized assembly, all allocated sections, and all allocated
relocation signatures (2,634 in DDS1 and 2,277 in DDS2). Both strict probes
assemble successfully and retain all 22 ordinary RTL passes.

All four retail/development builds pass. Both retail image checksum checks pass,
both development images report zero changed payload words, and a repeated build
reports no work. The complete tooling suite passes all **415 tests in 111
batches**. These are verification results for the integrated source, separate
from the earlier allocator control.

## What this establishes—and what it does not

The former same-semantic-kind-only rule rejected this live scratch reuse even
after the compiler mechanism was qualified. The revised rule instead requires
disjoint real values, correct definitions and consumers, no escaping address or
behavioral coupling, truthful naming and types, qualified compiler evidence,
independent semantic review, and complete-owner exactness. The bans on fabricated
work and hard-register steering remain intact.

The original variable names and source spelling are unknown. Matching bytes
and a qualified trace demonstrate a reproducible, behavior-preserving source
form; they do not prove that the original authors wrote this form. Nor does
physical-register reuse alone justify sharing any other pair of values.
