#include "common.h"

extern u32 D_00436418;

extern u32 D_0043641C;

extern u32 D_00436420;

extern u32 D_00436424;

extern u32 D_00436414;

extern s32 effBTLFieldColorGetBaseColor(s32, s16, s16, s32);

/* Keep the original selector and optional overrides separately for field-color lookups. */
typedef struct Entry20B {
    u32 value;
    u8 pad_0x04[0x10];
} Entry20B;

extern Entry20B D_003AB070[];

void effBTLFieldColorSetSelectors(s32 baseId, u32 variant, s32 overrideId, s32 finalId) {
    D_00436420 = (u32)baseId;
    D_00436418 = D_00436420;
    if (overrideId != 0) {
        D_00436420 = (u32)overrideId;
    }
    if (finalId != 0) {
        baseId = finalId;
    }
    D_0043641C = variant;
    D_00436424 = (s32)baseId;
}

u32 func_00169438(void) {
    return D_00436418;
}

u32 func_00169440(void) {
    return D_0043641C;
}

u32 func_00169448(void) {
    return D_00436420;
}

u32 func_00169450(void) {
    return D_00436424;
}

/* Default RGB adjustment used when no field-specific color is supplied. */
void effBTLFieldColorGetFixedVector(u32 unused, f32 *color) {
    color[0] = -0.73f;
    color[1] = 1.55f;
    color[2] = 0.24f;
}

INCLUDE_ASM(const s32, "game/code_00169418", effBTLFieldColorGetBaseColor);

/* Variant and kind are narrowed to their stored widths before the lookup. */
s32 func_00169580(s32 colorId, s16 variant, s16 kind, s32 arg3) {
    return effBTLFieldColorGetBaseColor(colorId, variant, kind, arg3);
}

u32 effBTLFieldColorGetEntryWord(s32 index) {
    return D_003AB070[index].value;
}

u32 func_001695C8(void) {
    return 1;
}

void effBTLFieldColorSetFlags(u32 bits) {
    D_00436414 = D_00436414 | bits;
}

void effBTLFieldColorClearFlags(u32 bits) {
    D_00436414 = D_00436414 & ~bits;
}

u8 effBTLFieldColorTestFlags(u32 bits) {
    return (D_00436414 & bits) != 0;
}

void effBTLFieldColorResetFlags(void) {
    D_00436414 = 0;
}

INCLUDE_ASM(const s32, "game/code_00169418", func_00169610);

void func_001696B8(void) {
    func_00169610();
}

INCLUDE_SDATA(const s32, "game/code_00169418", D_00436414);

INCLUDE_SDATA(const s32, "game/code_00169418", D_00436418);

INCLUDE_SDATA(const s32, "game/code_00169418", D_0043641C);

INCLUDE_SDATA(const s32, "game/code_00169418", D_00436420);

INCLUDE_SDATA(const s32, "game/code_00169418", D_00436424);

INCLUDE_SDATA(const s32, "game/code_00169418", D_00436430);

