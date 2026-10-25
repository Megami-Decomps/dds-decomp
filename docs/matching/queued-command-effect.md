# DDS2 queued command effect

`mnuInitializeQueuedCommandEffect` at `0x002BE730` is 1,504 bytes. It samples
20 or 14 candidate particles from ten or seven anchor pairs, with random
jitter and scale variation during the second pass. This builds on the Son3,
SdfModelVis, BigB, N2 and N1 investigations; KiB reconstructed the canonical
owner rather than importing the earlier homogeneous-tuple scratch layout.

The existing `MenuQueuedCommand` owns 128 position and motion records.
Field-derived byte offsets follow its matched random-line and drawing
siblings. Each accepted particle borrows the row base locally. The two-pass
bound is `anchorCount * 2`; signed coordinate displacements are computed
after each random variation before updating the actual coordinate. These
ordinary source boundaries preserve the native base allocation and address
association without flat word-array aliases or extra memory operations.
The three scale bytes are an ordinary initializer. The two external anchor
templates remain with their existing preceding owner.

## Caller and the narrow tail-call exception

Making the initializer visible adds `REG_EH_REGION 0` to its call in
`mnuQueueListEntry`. Strict probes show this is the sole normalized
`28.mach` difference before reorg. The old `s32` caller had no return value;
its exit `$2` use prevents donation of the constant 2 into the dispatch
branch once reorg scans through the known nonthrowing call. Its four source
callers discard the result, and the native body constructs no result.
The corrected `void` contract restores that donation.

A void caller alone makes the final command-initializer call a sibling jump,
unlike the native `jal` at `0x002BEF78` followed by its epilogue. This uses the
explicit post-call local-state-reset exception in `docs/CONTRIBUTING.md`:
one `i = 0` resets the existing bank-scan counter after dispatch. No new
counter, volatile access, wrapper, artificial return value or hidden callee
is introduced. In the strict probe, reset instruction 255 remains through
`01.sibling` and `11.cfg`, then disappears at `13.life`. Without it, the
sibling pass selects a sibling call; with it, the ordinary call survives.
This documents an allowed trace-backed source form, not a claim that the
original source contained that reset.
