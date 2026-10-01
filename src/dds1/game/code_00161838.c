#include "common.h"

extern u32 D_003BB024;

extern s32 effBTLFieldColorGetBaseColor(s32, s16, s16, s32);

extern u32 D_003BB028;

extern u32 D_003BB02C;

extern u32 D_003BB030;

extern u32 D_003BB034;

/* Keep the original selector and optional overrides separately for field-color lookups. */
void effBTLFieldColorSetSelectors(s32 baseId, u32 variant, s32 overrideId, s32 finalId) {
    D_003BB030 = (u32)baseId;
    D_003BB028 = D_003BB030;
    if (overrideId != 0) {
        D_003BB030 = (u32)overrideId;
    }
    if (finalId != 0) {
        baseId = finalId;
    }
    D_003BB02C = variant;
    D_003BB034 = (s32)baseId;
}

u32 func_00161858(void) {
    return D_003BB028;
}

u32 func_00161860(void) {
    return D_003BB02C;
}

u32 func_00161868(void) {
    return D_003BB030;
}

u32 func_00161870(void) {
    return D_003BB034;
}

/* Default RGB adjustment used when no field-specific color is supplied. */
void effBTLFieldColorGetFixedVector(u32 unused, f32 *color) {
    color[0] = -0.73f;
    color[1] = 1.55f;
    color[2] = 0.24f;
}

INCLUDE_ASM(const s32, "game/code_00161838", effBTLFieldColorGetBaseColor);

/* Variant and kind are narrowed to their stored widths before the lookup. */
s32 func_001619A0(s32 colorId, s16 variant, s16 kind, s32 arg3) {
    return effBTLFieldColorGetBaseColor(colorId, variant, kind, arg3);
}

typedef struct Entry20B {
    u32 value;
    u8 pad_0x04[0x10];
} Entry20B;

extern Entry20B D_0034E740[];

u32 effBTLFieldColorGetEntryWord(s32 index) {
    return D_0034E740[index].value;
}

u32 func_001619E8(void) {
    return 1;
}

void effBTLFieldColorSetFlags(u32 flags) {
    D_003BB024 = D_003BB024 | flags;
}

void effBTLFieldColorClearFlags(u32 flags) {
    D_003BB024 = D_003BB024 & ~flags;
}

void effBTLFieldColorResetFlags(void) {
    D_003BB024 = 0;
}

INCLUDE_ASM(const s32, "game/code_00161838", func_00161A20);

void func_00161A88(void) {
    func_00161A20();
}

INCLUDE_SDATA(const s32, "game/code_00161838", D_003BB028);

INCLUDE_SDATA(const s32, "game/code_00161838", D_003BB02C);

INCLUDE_SDATA(const s32, "game/code_00161838", D_003BB030);

INCLUDE_SDATA(const s32, "game/code_00161838", D_003BB034);

INCLUDE_SDATA(const s32, "game/code_00161838", D_003BB040);

