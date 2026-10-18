# Boss geometry: complete publication and input-acquisition stages

`effBossUpdateGeometry` emits 2,544 bytes with the native `0x130` frame.
The DDS1 owner gate is **71 match, 0 differ** on the pinned EE GCC 2.96.

## Source and investigation credit

This completes PiM's native reconstruction, building on the KiB, Opal and
Brass investigation lineage and the publicly preserved cursor/header/length
continuation. The starting complete source is the
[paired 87-word checkpoint](https://github.com/Megami-Decomps/dds-collab/tree/c88f2d887b96ce45850d6ea4948c15178017d2b5/drafts/boss-geometry-cursor-continuation-20261018).
Q1, Q2 and PurBoss's later reports supplied useful SDK-carrier and residual
scheduling observations. Their private four-word body was not recovered or
used as source. The public 87-word source was reproduced against current
headers before the following bounded continuation.

## The four source properties

1. The existing `VU0_SCALE_VF` takes the real binary32 scale bits through a
   compiler-owned GPR. Stock `20.greg` records a `RELOAD_FOR_INPUT` for the
   scale, emitting the native MFC1/QMTC2/VMULX sequence. This changes later
   reload carriers while leaving the global work/color allocation order
   unchanged: 87 differing words become 79.
2. Each flip arm completes all its live color publications, including the
   fresh slot-2 read copied to slot 4 after the last multiply helper call.
   Stock `19.lreg` retains these complete paths through allocation: the
   color cursor changes from 44 references / 235 live instructions to
   50 / 236, while work changes from 76 / 478 to 76 / 481. `20.greg` now
   visits color before work, and late cross-jumping shares the equivalent
   tail. Runtime calls and memory operations are unchanged. This produces
   six remaining schedule differences without padding either lifetime.
3. The real unsigned-byte `cellCountFromFrame` input is acquired with the
   other cell-phase inputs before the positive-count guard. Native reads
   it in that ordinary branch's always-executed delay slot. The unchanged
   selection test consumes the snapshot on the positive path. This fixes
   the cell-gate and clamp schedules, leaving two words.
4. A read-only `trailWidth` pointer acquires the actual width-vector address
   before initializing the positive history loop's sample counter. The
   existing VU load consumes that pointer. This expresses the native
   preheader producer order rather than leaving the address implicit until
   loop-invariant motion; the last two words match.

These are real values and complete execution phases, not evidence of unique
historical C syntax. No compiler flags, helper templates or shared types change.

## Preserved contracts and verification

The primary `groupCount` remains `u32`. Its real producer returns a value in
`0..0x03ffffff`, allowing the updater's signed local captures exactly.
Lifecycle loops use unsigned comparisons, and changing the primary field
would also change an allocation product's signed-overflow contract.

The inherited four input-address-only VU stores remain limited to the pinned
compiler and their audited uses: three complete history append vectors and
one complete 16-byte axis passed to the immediate axis-angle rotation provider.
They do not provide a general memory barrier. No scalar readback, buffer
extent or callback boundary is changed by this continuation.

The final DDS1 ordinary and stock-probe objects/assembly agree after only
scratch-path normalization. An actual-native-base GNU link resolves all 26
target relocations exactly, with all 71 function placements and all 6,364
emitted text bytes exact. The four owned literal bytes are exact. The same
four final zero text bytes are omitted by the pristine production owner;
the normal final link supplies its existing alignment padding. No gameplay
or new full-retail-build claim is made by these owner-level receipts.

The separately qualified DDS2 port uses its actual node-pool and reference-vector
symbols. Its owner also gates **71 match, 0 differ**, with all 636 target words,
26 target relocation words, 71 placements, 6,364 emitted text bytes and four
literal bytes exact at native addresses. Ordinary/stock parity and the same
pristine-production four-byte zero tail were checked independently for DDS2.
