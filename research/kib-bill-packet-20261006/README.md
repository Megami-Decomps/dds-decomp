# Billboard texture packet renderers: preserved nonmatch

KiB, 2026-10-06. These files are source handoff candidates outside the production build. They are not matches and carry no matching credit.

Targets: DDS1 game/code_00151F58.c::func_001528C0 and DDS2 game/code_00159B48.c::func_0015A4B0, each 1,404 bytes / 351 words. Both remain INCLUDE_ASM.

## Reproduce

Use the source/header context of 7c091b095334ec939c6b8c792c9b5e8ef7e4dead, insert the corresponding candidate at its ASM line, and correct the existing local sdfInitPacketList declaration to void(SdfListHead *), casting its two existing packetAddress callers at that address boundary. Keep the existing EffUnitObject (DDS1) or EffInstance (DDS2) owner and normal shared headers. The included EffPacketParams declaration is the complete existing 0x2C owner from effPCPScatter, not a newly invented prefix. A production reconstruction should coherently reuse that primary owner.

Run tools/check_unit.py with --source and the canonical source unit. Local validation used checkout 5a65092384eeedbdd6b01117f22ede86739e2f70 with the published current billboard/header and caller composition, normal canonical flags, the pinned ee-gcc2.96 compiler (cc1 SHA256 d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1), and the verified retail executables. No compiler or production flag changes were made.

Both bodies emit 1,384 bytes / 346 instructions. Each has 300 differing positional words among those 346 compared words, plus five missing native instructions. This is a broad allocation/scheduling frontier, not a five-word near-match. Full units retain all 111 existing DDS1 and 110 existing DDS2 C matches, with only the added candidate differing. No full retail success is claimed for these candidates.

## Established semantics

- Compose the instance's two matrices, then replace XYZ translation from its billboard
- Build one quad with a primary texture and, for modes 2/3, replace the geometry and add the secondary UV stream/texture
- Authored UV corners map to strip destinations 0,1,3,2
- Primary UV normalization is reciprocal/multiply; secondary normalization performs eight direct divisions. This title-independent asymmetry is native
- Both passes use the same packed color, exact 0x2C draw descriptor, and real SdfTex / SdfPoolNode provider owners
- Independent native/source review found the geometry and UV mapping coherent

## Remaining causal questions

Native uses five saved FP values for x/y, scaled half-extents, and cosine; the half-extent registers later hold the corresponding texture normalization values. Reusing real width/height scratch values across these two phases recovers the native 0x150 frame. Separate normalization locals consume extra saved FP registers.

Native also retains copies of the left-edge trigonometric products before combining corners. The current readable formula coalesces them, producing four fewer moves across the two passes and different FP scheduling/register homes. Explicit raw-corner rotation, shared projected-edge values, genuine per-pass scopes, and canonical vector/flat-matrix spelling did not recover the native graph. Source-loop rotation retains a loop absent in native. These observations do not prove an unavailable or impossible source solution.

The matrix store/readback reuses one materialized stack address in the candidate; native rematerializes it after the translation loads. The existing memory-clobber load form and an SDK address-word cast did not change that. No invented barrier, dummy lifetime, register binding, or unsupported inline helper was adopted.

No retail binaries, generated assembly, or private input links are included.
