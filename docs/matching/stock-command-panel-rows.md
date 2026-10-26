# Stock-command panel rows (DDS2)

`btlDrawStockCommandPanelRows` at `0x001C98E8` is 764 bytes.
The implementation draws the three stock-command rows with title-specific
eligibility checks, the selected-row color, and the current signed row fade.

## Source and data ownership

The matched DDS1 `func_001BE590` is the structural donor:
[original landing by redthing1](https://github.com/Megami-Decomps/dds-decomp/commit/b4b19d1d9fd71a1c0171ae427ca0a7a00ad3204a).
Its nested enabled/disabled color expressions and count/label variable roles
are retained. DDS2 additionally checks the party flag, event bit, unit ID and
unit status before enabling the first row. Actual early switch exits reproduce
its independent rejection paths without an artificial shared dependency.
The existing shared glyph-submission helper remains the real call boundary.

Two inferred three-entry local pointer arrays own the native label templates
at `0x004169E0` and `0x004169F0`. Each contains 12 bytes of pointers followed by
four bytes of alignment, not a fourth live entry. Shared strings retain their
existing providers. The now-redundant `D_004169F0` assembly include is removed.

## The remaining scheduling difference

A raw-coordinate reconstruction differed in 13 instructions. A qualified live
trace of the pinned compiler showed pre-reload scheduling choosing the fade
index shift (priority 14) before the coordinate shift (priority 12). This
changed the last use of the selection pointer and its local coalescing; it was
not a final register-order tie.

The source captures **translated pixel coordinates** before setting the text
limit, then scales those actual pixel values for glyph creation. This gives
the pre-reload scheduler the real coordinate arithmetic dependency chain.
The qualified `17.sched` dump, block 22, shows coordinate-shift UID 454
issuing at clock 0 and fade-shift UID 490 at clock 1. Pixel additions occur before the limit call in
`sched1`; after register allocation, `sched2` sinks the same pure additions
past that call while preserving the native saved coordinate registers.
Specifically, translated-pixel ADDs 464/481 precede call 511 in `17.sched`;
in `25.sched2` that call issues at clock 7 and those ADDs at clock 9.
No dummy dependency, new wrapper, qualifier, flag change or register pinning
is involved.

The spill roles use the actual DDS1 donor order (total, shown, labels). A
qualified dump independently showed reload allocating their slots in pseudo
order; this one donor-backed choice preserves the native three stack homes.

## Validation and credit

The unchanged project-patched compiler is SHA-256
`d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`.
Ordinary and diagnostic (`-da`) complete-owner assembly agree. The target is
764/764 bytes with no differing instruction words; the configured owner has
281 exact functions. Both emitted pointer templates, all six resolved pointer
relocations and their padding independently agree with the retail image.

PiM completed the DDS2 reconstruction and compiler-guided closure. Credit also
belongs to the matched DDS1 donor, and to the prior KiB, SibRewrite, PurFAD and
H1-2 stock-renderer investigations and canonical selection-storage work.

Fresh four-target builds (`dds1`, `dds2`, `dds1-dev`, `dds2-dev`) pass. Both
retail SHA-1 checks agree with the originals, both development images report
`changed_payload_words: 0`, and a repeat build is a no-op. The linked DDS2
target and both label banks are identical in the original, retail build and
development image. All 415 tooling tests pass. The minimal canonical source
also passes all 281 owner functions, retaining unrelated typed data declarations.

The October 26 refresh was validated against upstream
`4333c69c40a7cf579b6f1eff073e7ef29d14f8ee`, preserving its selection-field
names `firstVisibleOption` and `selectedOption` and all concurrent owner changes.
