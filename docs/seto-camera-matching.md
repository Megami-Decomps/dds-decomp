# DDS2 Seto actor-category camera selection

`func_002247D0` in `src/dds2/game/code_00214948.c` matches the native
1,364-byte/341-word body with the native112-byte frame (0x70). The complete
owner has271 matching functions and zero differences, up from270/0 with this
body in assembly. All270 earlier C peers remain exact. This is one newly
matched function; the camera family is not claimed as a unit.

## Source and prior work

The native-led source preserves Sable's earlier Seto scope. No complete
historical Seto body was recovered in this continuation, so this is not a
replay or score comparison against that unavailable source. The typed sibling
basis is the exact Satan actor-category selector at
[ddab9e863979a9984e8452dc88cf3d51c7303833](https://github.com/Megami-Decomps/dds-decomp/commit/ddab9e863979a9984e8452dc88cf3d51c7303833):
Halcyon Drift reconstructed it, Harbor Maple closed and integrated it, and
Luna reviewed its native paths and provider contracts. PiM adapts the actual
Seto behavior and closes its distinct exit topology; the independent Seto
review covers native contracts and the final source delta.

Seto retains signed-eight-bit interpretations of the callback's s32 mode
formals, unsigned target count, the captured link and callback-fresh reads,
all27 action labels, separate keyed-camera calls, the unmatched random
choice's success return, and camera publication before front/back adjustment.
Its return structure is not copied wholesale from Satan.

## Causal source progression

- 001 reconstructed the complete behavior with the typed sibling basis:
  1,316 bytes,251/329 positional differences and48 uncovered bytes. Jump2
  merged the two keyed-camera calls, leaving40 physical call sites.
- 002 made the existing keyed case0/case1 success exits explicit. It restored
  all41 physical calls and exact target jump-table/data content, at1,356 bytes
  and184/339 positional differences. Eight bytes remained uncovered.
- 003 made the action-switch default success return explicit after unchanged
  camera publication. It reached the full1,364 bytes with all41 call PCs and
  target jump-table/data exact. Only +0xA4 and +0x4F0 BNEZ/BNEZL annul choices
  differed; both delay slots define return register V0 as zero.
- 004 changes only the multi-target cameraMode test from early failure followed
  by success into a guarded success body followed by failure. The predicate,
  callback sequence, arguments, read epochs and return values are unchanged.
  This natural shared-exit topology resolves both annul differences, including
  the earlier site whose source was not edited, and yields exact native bytes.

The saved stock dumps make that exit-sharing result concrete. In003 jump2,
the first branch has a separate zero/exit and the second has a preceding zero
before branching to the common epilogue. In004 jump2, a real late rejection
label owns zero UID1243 and an exit; both residual branches target it. In29.dbr,
both filled annulled sequences contain copies of that same zero definition.
The initial marked gate also uses the shared rejection. External fallthrough
calls retain their prior absence of REG_EH_REGION notes: provider visibility
was not the cause changed here. These are observed stock-pass input/output
facts, not an instrumented internal resource-query trace or a universal claim
about guard spellings.

No manual instruction ordering, register pinning, extra arithmetic consumer,
provider attribute, shared-header change or compiler-flag change is needed.

## Frozen qualification

The implementation pin is `877ffa3debe28d7ed45f63b0fd3505f4c4f99312`.
Frozen full-owner SHA-256:
`6038083f7e3229ec5afe9f029a54fcb94cb8aaeb848793a49e6ffbf14c280f64`.
Target-body SHA-256:
`99834f52b7dd44bbc3e51b75cff90e8d19da061a168588877944fe9b7271b8a0`.
Independent final semantic-review SHA-256:
`01d5558c1650f47e5843921af207c77bc37c9c21d70e88c5e7f80887ecbce7e3`.

Explicit-DDS2 ordinary and stock `-da` output agrees across every allocated
section, raw relocation section and function metadata record. The independent
source review accepts semantics separately from the implementing validator's
byte/build results. No compiler or game binaries accompany this source change.

## Current-context and linked acceptance

Current qualification pin: `bad6a472ddeaa98f9e12dcb0f7f8fec910be9be7`.
The source preserves its unrelated motion-frame and camera-provider renames;
only the target assembly placeholder is replaced. Published full-owner SHA-256:
`e12a185df829121cb54abf712f37847ec437c76373c938512e30f384ed3b97d9`.

Current-owner checking passes271/0. Every allocated and raw-relocation section
is byte/metadata identical to the frozen exact object. Function names outside
the target legitimately changed between pins, so cross-pin symbol-name identity
is not claimed.

`ninja dds2 dds2-dev` completed all5,644 configured steps. The retail executable
SHA-1 equals the original:
`9be91ee1b4a535a4cb6ec89237b5a6ba41be2add`.
The dev link reports zero changed payload words; the immediate repeat of the
same targets reports no work to do. The sole tracked implementation change is
the owner source, and `git diff --check` passes. No headers, configuration or
compiler flags changed.

The standard available environment reports26 unavailable retail base field
archives and excludes those archives from default targets. These results cover
the available retail executable and configured dev targets, not those missing
archives or gameplay. No hosted-CI result is needed or claimed for this direct
source publication.

Compact acceptance-receipt SHA-256:
`1b7c163c3073925c7bd471ebaad34b11559583e4eece89eb975935822d57b957`.
Current-context parity-receipt SHA-256:
`00642cd10b6f3ec32a239b3b068e0dfc76f14d7ba524957e42a3807fb113f9ca`.
