# DDS2 party skill-list panel

`mnuDrawPartySkillListPanel` at `0x002C10F0`, in
`src/dds2/game/code_002BE628.c`, is 1,392 bytes. It draws a two-column skill grid
and its row-count-dependent frame. The reconstruction uses ordinary C, with no
new inline assembly, register binding, qualifier, helper, or compiler option.
Exact bytes establish a reproducible source contract, not the original names
or source spelling.

## Native storage and callback epochs

The panel captures its fade before `brsGetLevelStepForValue`, then copies the
32-byte coordinate table at `D_0042B160` to SP+0 and the 160-byte sprite table at
`D_0042B180` to SP+0x20. The tables keep their existing rodata owners. The
coordinate bank is `[names/blank cells][column][X/Y]`; the sprite records keep
the canonical `MenuPanelSprite` set, slot, and `MenuPoint` offset fields. No
existing typed object is read through a flat cursor or alternative alias.

The capacity switch maps 4 to two rows, 6 to three, and every other capacity to
four. Frame pieces are selected in order: repeated piece 0, pieces 1 and 2,
piece `rowCount + 1`, repeated piece 6, then piece `rowCount + 5`. Repeated
pieces advance Y by 0xB0 after each draw. Every selection is in the copied
ten-record table for every reaching row count.

DDS2 captures the repeated pieces' resource pointer and slot before their draw
loops. Native resource/slot captures are `0x002C1298`/`0x002C12A4` and
`0x002C13F4`/`0x002C1400`. These values must survive the callbacks unchanged;
reloading `panel->resourceSets` every iteration changes the native contract.
The other pieces acquire their resources after the preceding callbacks.
The later skill loop reads each current skill after the frame has been drawn,
including changes callbacks could have made to the record.

DDS1's matched `mnuDrawPartyInfoSprites` corroborates the coordinate banks and
disjoint coordinate/counter workspaces. It does **not** justify the DDS2
resource snapshots: its own repeated draw reloads the resource, unlike the
DDS2 native code above.

## A real selector preserves the field-address bases

One `spriteIndex` holds the actual selected piece for fixed and dynamic draws.
Every assignment has a real table consumer. The first definition, zero, feeds
the same indexed resource/slot/offset access shape as the subsequent selections.
In qualified pass 00, selector pseudo96 is defined at UID126 and consumed by
the first record-index expansion. Later CSE removes constant index arithmetic
while preserving the address-sharing structure needed by the variable-selected
footers.

The source therefore retains the native set/slot/Y bases at SP+0x20, SP+0x24,
and SP+0x2C. A remove-selector control substitutes direct subscripts only for
pieces 0, 1, 2 and 6, retaining the dynamic footer selections and all behavior.
It still emits 1,392 bytes but differs in 95 words. This control loses the
native retained-address pattern. The selector is not an added effect or a
request for a particular hard register.

## Disjoint coordinate and grid phases

`horizontal` and `vertical` first hold real frame coordinates. After their
last draw consumes those values, the grid resets them to column and row indices.
Their addresses do not escape. Every reaching use has an initialized value,
and sharing storage adds no arithmetic, memory operation, call, or artificial
use. The separate frame-row countdown remains `remainingRows`.

The qualified final pass 00 defines coordinate pseudos94/95 at UIDs177/192,
resets those same pseudos to zero at UIDs904/917, and advances the grid indices
at UIDs1114/1098. The remove-reuse control instead creates distinct counter
pseudos96/97 while leaving the coordinate definitions intact.

The shared horizontal pseudo has 13 references, live length 106 and allocation
priority 3,679; it receives s3. Shared vertical has 25 references, live length
90 and priority 11,111; it receives s1. With separate counters, the column has
7 references, live length 142 and priority 985 and spills; the row has
11 references, live length 112 and priority 2,946 and receives s4. Those changed
allocation inputs propagate to the parameter and address homes. The control
emits 1,408 bytes, 16 beyond native, with 252 of 348 compared words different.
This is a causal source-ownership control, not an inference from physical
register reuse alone.

## Acquisition order closes the final 18 words

Each repeated piece stages the existing values in record order: resource,
slot, X, then Y. The reads all precede the same draw, and the copied table
cannot be changed by an intervening call because there is none. Each staged
value has the existing draw consumer.

The coordinate-first control differs in 18 words at the same 1,392-byte extent.
It defines horizontal before resource in pass 00. Through sched1 this gives
horizontal pseudo94 13 references/live length 107/priority 3,644, while resource
pseudo99 has 6 references/live length 32/priority 3,750. Global allocation assigns
resource to s3 and horizontal to s4, opposite native.

Resource-first staging gives horizontal 13/106/3,679 and resource 6/35/3,428.
The actual global allocation order reverses: horizontal receives native s3,
resource native s4. The corresponding real load definitions and schedules also
recover the native resource-before-final-Y computation and second-piece X/Y
load ownership. All 348 instruction words then match. No added operand,
constraint or invented lifetime is needed.

## Qualification and integration boundary

The frozen whole-owner candidate checks **192 match, 0 differ**, including
owned data. The target's NON_MATCHING guard and assembly fallback are removed;
all other fallbacks and both table imports remain unchanged. No shared header,
external prototype or other-game source changes are required.

The final and remove-reuse/remove-selector controls have independently compiled
ordinary and stock `-da -fsched-verbose=5` captures using the canonical complete
owner. Their normalized whole assembly, six allocated sections' bytes and
metadata, 964 complete symbols, 925 resolved relocations, two auxiliary
nondebug sections and ELF ABI agree. The 18-word staging control and its exact
successor have the same complete qualification. Every compiler and assembler
exit succeeded. The pinned cc1 SHA-256 is
`d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`.
No debugger or modified compiler was used.

These focused receipts do not replace independent semantic review, both-game
full retail/development builds, or repository-wide tests before publication.

## Integrated result

Independent semantic review approved source SHA-256
`4e5e4686c092b9ad93c0f3836a8c131ec71555a8ab08378ae663edfcac4d678c`.
The integrated complete owner passes 192/0, both full retail executable SHA-1
checks pass, all 442 tooling tests in 117 batches pass, and all 80 retained-C
units compile. The target contributes 1,392 exact bytes. Private binaries,
compiler captures and controls are not part of the source commit.
