# DDS2 party profile panel contents

`mnuDrawPartyProfilePanelContents` at `0x002C16F0`, in
`src/dds2/game/code_002BE628.c`, draws the profile frame and four skill rows.
All 1,124 native bytes match with ordinary C. There is no new inline assembly,
register constraint, helper, qualifier or compiler option. Exactness establishes
a reproducible source contract, not the original source spelling.

## Native data and callback boundaries

The function copies the existing 80-byte `MenuPanelSprite` table at `D_0042B220`
to SP+0x60 and the 32-byte `MenuPoint` table at `D_0042B270` to SP+0xB0.
Both remain under their existing rodata owners. The canonical typed records
retain their set, slot and XY fields; no flat alias or alternative layout is used.
The fade is captured before the table copies and skill-list provider.

The selected pieces are 4, then 0 and 1, then 2, then 3. Piece 4 is repeated
four times and piece 2 three times, with Y advancing by 0xB0. The two repeated
phases capture the resource pointer and slot before their callbacks, as native
`0x002C1860..0x002C188C` and `0x002C1940..0x002C1970` require. The two headings
and final piece acquire their current resource after the preceding calls.
All selected pieces and their set indices are in bounds.

The skill-list provider fills the canonical `PrfSkillListBuffer` prefix. The first four skill IDs in that output drive the range sprite, glyph, cost/icon and blank-cell
branches. Resource reads in these later branches retain their native callback
boundaries. The provider/draw ABIs and the caller-owned 0x40-byte list storage
are unchanged.

## One real selected-piece index closes the heading residual

The same `spriteIndex` selects each fixed piece and each heading. Every
assignment is initialized and consumed by the existing resource, slot and
offset accesses. Its address does not escape, and no value is carried beyond
its actual consumers. The accepted skill-list-panel sibling corroborates
this use of a common selected-piece index.

The isolated direct-fixed control keeps the selector's declaration, scope and
heading assignment unchanged, but replaces only selections 4, 2 and 3 with
direct constant subscripts. It restores exactly 19 different instruction words,
all in the heading region, at the same 1,124-byte extent. Thus the effect is not
merely moving a declaration out of the loop.

The qualified stock compiler trace establishes the causal chain:

- Pass 00 defines selector pseudo94 from piece 4 at UID93 and from the heading
  counter pseudo93 at UID221. UID222 advances the counter, and UID227 scales
  the selected piece by 16. Later definitions select pieces 2 and 3.
- At `03.cse`, the final source's UID222 becomes `counter = selector + 1`.
  The control's corresponding UID179 remains `counter = counter + 1`.
  Both started with the counter expression in pass 00. The shared selector's
  real definitions change the CSE choice; the exact internal preference
  predicate is not separately instrumented.
- In `17.sched`, the final copy UID221 issues at clock 0. The scale UID227
  and successor UID222 are now both true dependents of that copy and issue
  at clock 1, with scale first (priorities 10 and 3). In the control, the
  successor issues alongside the copy at clock 0, before the scale.
- `19.lreg` gives final selector pseudo94 six references over four instructions
  and assigns v0; scaled offset pseudo147 receives a3. The direct-fixed control
  gives selector pseudo94 four references over three instructions and assigns
  v1, which it also reuses for scaled offset pseudo127. The final native
  sequence retains the old index in v0, scales into a3, then derives its
  successor from v0.

These actual initialized values and consumers account for the native schedule
and allocation. No artificial dependency, register request or extra operation
is needed.

## Disjoint vertical phases

`vertical` first holds the frame's Y coordinates and, after their last draw,
becomes the skill-row countdown. These disjoint phases have real consumers;
each reset supplies the next value before use, the address never escapes,
and there is no added read, arithmetic, callback or alias. DDS1's matched
profile-contents function corroborates the coordinate-to-counter workspace.

An isolated separate-row control keeps the exact selector and creates a distinct
skill-row countdown. It emits 1,124 bytes with 31 different words. The final
shared vertical pseudo96 has 21 references, live length 99 and global priority
8,484, receiving s2. The separate coordinate has 14 references/live length 28,
priority 15,000, receiving s0; the separate countdown has 7/142, priority 985,
receiving s4. The changed allocation order also moves the real frame row and
X-coordinate homes. This is a measured source-ownership dependency, not an
inference from a shared hard register alone.

## Qualification and integration boundary

The frozen complete owner passes **193 match, 0 differ**, including owned data
and the previously matched skill-list panel. Only this target's guard/fallback
is removed; its table imports and all other functions are retained.

The final source and the two isolated controls each have independent ordinary
wrapper and stock `-da -fsched-verbose=5` captures. Normalized complete assembly,
six allocated sections' bytes and metadata, 964 complete symbols, 926 resolved
relocation contracts, two auxiliary nondebug sections and the ELF ABI agree.
All compiler and assembler exits succeeded. The pinned cc1 SHA-256 is
`d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`.
No debugger or modified compiler was used.

The frozen source SHA-256 is
`28cc39445c9df5866b14b092a93d0cfdc2f02c41b7af2528662253ff939ff465`.
These focused checks do not replace independent semantic review, both-game
full retail/development builds or repository-wide tests before publication.

## Integrated result

Independent semantic review approved source SHA-256
`28cc39445c9df5866b14b092a93d0cfdc2f02c41b7af2528662253ff939ff465`.
The integrated owner passes 193/0, both full retail executable SHA-1 checks
pass, all 442 tooling tests in 117 batches pass, and all 80 retained-C units
compile. The target contributes 1,124 exact bytes, with no new constraints or
assembly. Private binaries, compiler captures and controls are not part of the
source commit.
