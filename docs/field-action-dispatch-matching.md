# DDS2 field action dispatcher

`func_001411F8` in `game/code_0013B818` is 1,604 native bytes.
Reconstructed against `6858c80a777bfbbf85ce77b949003f9a9807b7b4`.
Production-shaped whole-owner check: **63 match, 0 differ**, including all
62 previously exact peers. Native byte SHA-256:
`61d2cda878c85d20a18536e0a4d32137255341c7ba1d3803e3a196c3b46daea2`.

The action dispatcher uses the first enabled matching actor. Its outer mode
chain is ordered 2, 0, 1, 3; the nested transition switch has physical blocks
0, 3, 4. Default-first area/room selection preserves the native branch shape.
The destination-room local is shared between ordinary room transitions and
header-selected transitions; both become sequence record `kind`, consumed as
scene `room`. The elevator selector is a distinct local. Floor flags before
callbacks and bitmap flags reloaded afterward are distinct values/epochs.

The final address expression uses the existing `FldWaypointHeaderValues.data`
view, `body.data[headerFloor + 6]`. It accesses the second six-halfword bank of
the real 12-halfword union, as already used by `fldGetRowValue` and
`fldFindTableEntry`. Valid elevator inputs remain header index 0..7 and floor
index 0..5. This does not extend the supported input domain or flatten records.

Stock cc1 `-da` explains the last word: the equivalent nested `elevator.blocks`
expression reached combine with offset plus (base + 20); combine folded 20 into
the load but left base plus offset, reversing the native `addu` operands.
With the canonical whole-body view, combine insn 1115 retains offset pseudo376
plus base pseudo360, while insn1131 loads the signed halfword at +20. The result
is byte-exact. No flags, aliases, type definitions or compiler changes were used.
Stock-dump and normal compilation have identical normalized assembly and all
substantive ELF sections; only scratch-path debug metadata differs.

Full-owner production data compared unchanged to the pristine owner: .rodata
780 bytes, .sdata112 bytes, .lit4 36 bytes, empty .data. Calls and global accesses
use existing providers and relocation-aware checking. Sequence initialization
remains partial, and reloads after callbacks remain intact.

This is a fresh reconstruction. A historical Son4 near-match was unavailable;
its prior eight-byte deficit was not diagnosed or reused. Local collaboration
supplied the canonical union-view hypothesis and independent provider review.
There is no claim of recovering the historical original source.

## Request storage and acquisition order

The area request is a real one-word payload, represented by `s32 area[1]`.
Mode 1 captures current area, reads the selected header's destination room,
and only then publishes the area payload, matching the native load/store order.
The temporary current-area value is consumed by that publication.

This form remains exact both before and after the independent canonical
`SdfColorTrack` key-record correction. The earlier address-taken scalar form
changed eight mode-1 scheduling words under that unrelated header change.
The explicit request storage and acquisition/publication phases remove that
observed context sensitivity. Both gates are 63/0, and both complete retail
executables retain their expected SHA-1 with the canonical key records applied.
