# Billboard packet renderer frontier

KiB, 2026-10-06. Research candidates only; production ASM remains authoritative.

DDS1 `code_00151F58.c::func_001528C0` and DDS2 `code_00159B48.c::func_0015A4B0` are each 1,404 bytes / 351 words. These updated candidates each have **20 differing words out of 351**, replacing the earlier 346-word reconstruction. They are not matches and carry no matching credit.

Insert the respective snippet at its INCLUDE_ASM in the canonical translation unit, preserving existing headers and neighboring C. The required EffPacketParams definition is the complete existing packet owner; promotion/reuse must be reconciled before production integration. Tested with the pinned compiler cc1 SHA256 d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1, normal per-unit flags, and current eff/sdf/pcp_vu0/ee_mmi headers captured at cd495a8d. Use tools/check_unit.py with --source and the canonical unit; filename identity matters.

## Resolved causes

- Separate scaleX/scaleY samples preserve the native field sampling and FP homes.
- Reusing cornerX/cornerY as the actual input pair for each corner reproduces the native FP products and copies. Primary and secondary UV arithmetic intentionally differs: reciprocal multiplication versus direct division.
- A qualified, assembly-identical RTL trace proves first CSE merged the matrix address, and combine then removed its second computation. The existing VU0_STORE_MATRIX_UNCLOBBERED form restores the native address graph through the historical top-level volatile-ASM CSE behavior. This is not a new barrier or a compiler flag.

At this specific legacy store site, VU writes all 64 matrix bytes, C overwrites only translation XYZ, and VU reloads all 64 bytes. There are no C matrix reads. The emitted sequence was reviewed; this is not a generally safe replacement for memory-clobbered stores.

## Remaining frontier

The 20 words are confined to texture-dimension GPR homes and first U/V component load/conversion ordering, starting at +0x190 and +0x378. Frame size, operation count, matrix/XYZ flow, FP geometry graph and all other words agree. No source impossibility is claimed.

Retail originals and generated assembly are deliberately absent.
