# Effect quad drawing: snapshot, corner, and packet ownership

`effDrawQuadWork` matches in both games: DDS1 at `0x002A8020` (1,424
bytes) and DDS2 at `0x002EA5D8` (1,436 bytes). This is an ordinary C
reconstruction around the existing EE SDK operations, not a claim that the
original source spelling has been recovered. No new assembly, register
binding, volatile object, compiler barrier, or dummy consumer is introduced.

## Actual value owners

The two color unpack operations share their real conversion scale in a local
scope containing those operations. Giving that value function-wide scope
unnecessarily carries it across preceding calls. The draw request publishes
its vertex and UV pointers in field order, using the declared float UV type.

The fallback path constructs the actual `BillSnapshot`: its texture field owns
the sampled reference, and integer width and height first describe UV extents,
then the scaled geometry dimensions. The already matched
`effDrawAndAdvanceSharedParticleFrame` supplies the same snapshot-building
pattern. Conversion and floating-point operation order are preserved; no
algebraic reassociation is used.

The width and height working scalars are consumed as geometry half-dimensions
before becoming UV normalization reciprocals. Corner construction follows the
perimeter order 0, 1, 3, 2, updating actual corner X and Y values. This pattern
also appears in the shared-particle draw function and DDS2
`code_00159B48.c`. Each stored corner has the same XZ expression and the same
zero Y component as before. Only the calculation traversal changes; the
vertex array layout and winding submitted to the consumer do not change.

UV stores follow the corresponding record rows: destination pairs 1 and 0,
then 3 and 2. All eight source components retain their original destinations.
The optional setup and restore packets initialize their real DMA and VIF
fields in structure order. Calls, packet values, surface selection, and frame
advance policy are unchanged, including DDS2's pause flag.

## Synchronous borrowed-array contract

The retained nonmatching source cleared the shared request's vertex and UV
pointers after packet construction. Neither native routine contains those
stores. The builder `effPkt3DPrimMakePoly` allocates a separate packet, packs
XYZ positions, copies the UV stream, and returns the new packet. It does not
retain either borrowed array pointer. The subsequent draw-tag and surface
operations receive that packet/list, not the stack arrays.

Direct symbol references to `effQuadPacketParams` are confined to its
initializer and this draw routine. Every path reaching the builder replaces
both pointers before use; the expired-frame early return does not read them.
Consequently, the native stale pointer representations are not subsequently
evaluated before replacement. Removing the extra clears restores that native
contract; it does not introduce asynchronous borrowing. This remains the
existing shared, non-reentrant request design, not a new reentrancy guarantee.

The temporary positions initialize XYZ, not W. The existing SDK
`EE_MMI_STORE_VEC3_VALUE` operation takes a quadword carrier but emits only
its first three words. This partial-vector convention is already documented
in `docs/idioms.md`; no new W-lane read or initializer is added here. This is
an EE SDK/native contract, **not** a claim that every byte of the carrier is
initialized or that the macro is a portable ISO C vector implementation.

## Qualified corner-traversal control

A control changes only the final DDS2 corner walk to 0, 1, 2, 3, leaving
snapshot construction, UV stores, packet fields, and pointer handling intact.
It emits 1,444 bytes rather than 1,436. Its ordinary/dump owner sections and
relocations also agree, so the paired traces are qualified on both sides.

In `00.rtl`, the actual corner-X pseudo 97 has three definitions for the
perimeter walk (`x + width`, `x - width`, `x + width`), versus four for the
row-major control (an additional `x - width`). In `19.lreg`, that real
quantity has 9 uses across 36 instructions and three definitions in the
matched source, versus 12 uses across 39 instructions and four definitions
in the control. The width workspace also changes from 9 uses across 77
instructions to 10 across 83. These are actual corner consumers, not inserted
lifetime anchors. Final output keeps the native pair of right-edge product
copies; the control keeps four product copies and gains two instructions.
The evidence explains the source-use graph difference without asserting that
source traversal order can be uniquely recovered from final assembly.

## Qualification

The final ordinary and dump-enabled builds have identical bytes, sizes, and
types for every allocated owner section and identical normalized relocation
contracts, separately in each game. This qualifies the saved compiler traces
as observations of the ordinary build rather than merely a similar probe.
The full C-owner checks report 565 matching functions and zero differences
in DDS1, and 714 matching functions and zero differences in DDS2.

The owned data is independently compared to retail, resolving relocations:

| Owner | `.rodata` | `.lit4` | `.sdata` |
| --- | ---: | ---: | ---: |
| DDS1 | 4,496 bytes / 112 relocations | 240 bytes | 2,232 bytes / 1 relocation |
| DDS2 | 5,680 bytes / 142 relocations | 292 bytes | 2,592 bytes / 2 relocations |

Every listed section agrees byte-for-byte after relocation. Final linked
retail/development builds remain a separate integration gate; a local masked
function comparison is not used as a substitute for those checks.
