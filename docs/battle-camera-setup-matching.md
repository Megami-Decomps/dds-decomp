# DDS1 battle camera setup

`func_001E6BB0` constructs the selected battle-camera policy's focus, eye and
optional endpoint. Its complete **12,848-byte** body matches retail. The ordinary
owning-unit check against the verified current input closure reports
**589 match, 0 differ**, with all 588 previously
reconstructed functions, data, five switch tables and production-context checks
preserved.

PiM reconstructed this source from the current native body. Credit also belongs
to Obsidian Heron's earlier complete camera work and
[release](https://github.com/Megami-Decomps/dds-collab/issues/12#issuecomment-6063864223).
That source was unavailable, so this result is neither a historical replay nor
a numerical comparison with the missing draft.

## Source roles

The seven policy banks use complete 0x60-byte records and their actual array
extents. Existing command, camera, task, unit and cursor owners remain in use.
The reconstruction preserves all 141 calls, callback-fresh policy and actor
loads, native defaults, and complete vector lanes.

Several values needed distinct source owners: selectors captured before a
callback versus selectors reloaded afterward; separate query masks; selected
basis state committed in its comparison arms; deferred signed-mode selectors
paired with their framing half-angle; and fresh post-transform `adjustedFocusY`
versus the separately captured and clamped original Y.

The final six instruction differences came from original Y using F2 instead of
native F1. A qualified observation of an earlier frozen source showed that F1
had no hard conflict, but the allocator's `someone_prefers` reservation excluded
it. Eye policy 0x2D had shared a scalar with unrelated group-framing spans; its
lateral-offset product introduced the unwanted preference. Giving the selected
actor's extent its actual case-local `actorExtent` owner resolved it. This value
has one provider definition, seven reads and no use after its destination-X
commit. Arithmetic, constants, read order, callbacks and branches are unchanged.
Both the extent and generic span still physically use F21; the carried Y now
uses F1. No register pinning, dummy use, volatile steering, source-order search
or compiler-flag change was needed.

## Finite SDK-store qualification

The target uniformly uses the existing `VU0_STORE_VF_UNCLOBBERED` form documented
in [Thunder's matching notes](thunder-bezier-matching.md). This is a qualification
of this compiler, source and emitted paths, **not a generally sound memory
barrier**. The documented stale-read and loop-hoisting counterexamples still
apply to other uses and future edits.

Independent audits cover all 230 vector stores, 232 loads, 296 floating memory
operations, scalar readbacks and conditional stores, 141 call boundaries,
169 branch signatures, complete provider vector arguments and XYZW/XYZ lane
effects. All 56 scales use the existing `VU0_SCALE_VF` with actual f32 inputs:
55 bit-preserving MFC1 definitions feed 56 QMTC2 uses. The sole shared definition
is the genuine unchanged framing distance used twice in eye policy 0x2E.
The complete emitted target, including all these consumers, is byte-identical.

## Reproduce the check

Use the repository's normal setup with legally supplied DDS1 retail input and
the original ee-gcc 2.96 toolchain, then run from the repository root:

```sh
python tools/check_unit.py src/dds1/game/code_001C8890.c
```

This checks the installed source at its canonical unit path. The qualified
compiler SHA-256 is
`d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`.
The resolved native and compiled target SHA-256 is
`c72fa6c9bbab4ed737463f0c5f1f100fd2a39a4f8cc91968a21c22ef82ccfaa5`.
Ordinary/probe target instructions and all 294 normalized target relocations
also agree.

The original exact source was qualified on the 50-file target input closure at
`b79ce97966e9f2cf6d0566cf77dcdf4116d8932e`. The landing preserves incoming owner,
header and configuration changes at
`5fc180413dce72bbc19a91be0d21f1d2aa00f92e`; its full owner SHA-256 is
`859cc4aa2bbf13bb9a4b28850770ba4e258f2b2bbb481d1ecc679d00f4c211b3`.
The current-context whole-unit check again passed **589 match, 0 differ**.
Actual EE preprocessing uses 41 headers, all in the verified 50-file closure.
The subsequent `1f7ae1c76ad1207b76aa3a8137cc90f81bd6134f` parent changes
only an unrelated shop unit; this complete input closure is unchanged.
These are unit and finite target qualifications; no full current-image or
hosted-CI result is claimed here.
