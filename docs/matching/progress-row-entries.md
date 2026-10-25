# DDS1 progress-row entries

`brsDrawProgressRowEntries` (`0x00266668`, 672 bytes) uses the existing
`BrsSkillPackageWork`, `BrsProgressRow`, and unit contracts. Two placement rows
are selected by the captured unit's current frontline flag, then five common
rows are drawn. Unlike DDS2's matched sibling, DDS1 has no extra active-list
argument, overlay callback, or texture draw.

The four automatic initializers own the native palette and placement templates:
16, 24, 24, and 60 bytes at `0x003AFBB0` through `0x003AFC2B`, followed by four
alignment zeros. Coordinate/slot triplets are real local arrays, not flat aliases
of a typed external object. Their inferred outer bounds give the compiler
separate aggregate alias sets, restoring native independent copy scheduling
without reordering application work. See the inferred-array-bounds idiom.

Each selected row supplies ordinary `px`, `py`, and `slot` values before drawing.
The shared cursor has type `s32 (*)[3]` and advances complete rows: it does not
walk beyond a scalar pointer's first inner array. Explicitly acquiring these
three used values retains native cursor advancement instead of the compiler's
next-row preload/branch-likely tail. No dummy value, special qualifier, wrapper,
or dead-code exception is used.

Opacity is captured once; the mutable four-color array is passed to every draw.
The unit pointer is captured once, but its flag is re-read for each conditional
row. Resource availability is checked initially and the resource pointer is
reloaded after callbacks; subsequent draws are not newly suppressed if it changes.

## Provenance

This body was reconstructed from native instructions using the matched DDS2
sibling's real scalar placement/call contracts, not recovered historical draft
bytes. Earlier Raikaru/Lite2-2 (board comment 6242090018), SibRewrite
(6245139457), and N3 (6279617184) established earlier fragments, the aggregate
copy frontier, and the unsuccessful copy/order permutations. This work retains
that credit. The bounded final changes are inferred literal array extents,
real coordinate/slot extraction, and a correctly bounded row cursor—not a
repeat of the historical declaration-order search.
