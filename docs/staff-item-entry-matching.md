# Ordered staff item entry: shared vertical placement

DDS2 `mnuDrawOrderedStaffItemEntry` at `0x002AC050`, in
`src/dds2/game/code_002AB890.c`, matches all 948 native bytes (237 words).
The complete owner improves from 28 matching C functions to 29, with zero
remaining differences or checker advisories.

[Halcyon Drift's complete reconstruction](https://github.com/Megami-Decomps/dds-collab/commit/533ebcfe8a19e9bc8cfd9f587da8b31607d3f910)
provided the five-word baseline and the truthful `MenuStaffChoices::pulse`
field at `0x50`. PiM completes its real vertical-placement ownership; preserve
the donor's reconstruction and diagnostic lineage.

## Source property

The selected header uses `y - 8` for its two edge sprites and returns. The
mutually exclusive empty-slot path uses `y + 0x40` for its marker sprites.
One `placementY` local holds these genuinely consumed vertical coordinates.
All assignments remain at their original positions, with the same arithmetic,
callback order and fresh pointer reads. There is no artificial use, lifetime
extension, register constraint or compiler change.

Ordinary vendor RTL explains the five-word closure. The baseline edge value
(reg102) is block-local and local allocation chooses S0, despite its S3 copy
preference. Shared placement (reg101) has two real definitions/deaths in the
two arms, becomes globally allocated, and receives S3, also used by formal
`y` (reg85). All five remaining operand differences disappear. The trace
establishes a sufficient ordinary-C property, not the original variable name.

The signed 32-bit pulse field follows native signed accesses and arithmetic;
`MenuStaffChoices` stays `0x54` bytes with every field offset unchanged.
Independent source review checked the complete native algorithm, both arm
lifetimes, callback/read epochs and the header layout.

## Qualification

Frozen qualification pin: `26fbc3881efc7aedb11f8377d6d71080f1baed33`.
The exact source and header pass 29/29 functions, all 28 existing peer
functions and resolved relocations, and ordinary/`-da` assembly plus allocated
section/raw relocation parity. All 81 actual `mnu.h` consumers pass (38 DDS1,
43 DDS2; 4,187 matching C functions, zero failures).

Both retail images reproduce their expected SHA1:

- DDS1: `6d18898e2724bf1d145392766e8ba1e678487419`
- DDS2: `9be91ee1b4a535a4cb6ec89237b5a6ba41be2add`

Both development images have zero changed payload words. Immediate repeated
builds of all four targets are no-ops. Generated unrelated source placement
changes were restored, and DDS2 was relinked and rechecked before publication.

Current-context replay at `5cfe135964cefe5c1de16906f08961d1ec3f2962` also
passes 29/29, with every allocated section and raw relocation section
byte-identical to the frozen exact object. Later unrelated commits are
retained using a source/header and compile-input guard; this focused replay
is distinct from the frozen full-image qualification. No game or compiler
binaries are distributed with this source.
