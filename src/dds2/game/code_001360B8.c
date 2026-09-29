#include "common.h"

extern u32 D_0038BBD8[];

extern u32 D_0038BB50[];

extern u32 D_0038BB60[];

extern void *memset(void *s, s32 c, u32 n);

extern s32 D_00436170;

INCLUDE_ASM(const s32, "game/code_001360B8", func_001360B8);

INCLUDE_ASM(const s32, "game/code_001360B8", fldSetDisplayState);

void fldInitializeDisplayPointerTable(void) {
    u32 *displayTable = D_0038BBD8;

    memset(displayTable, 0, 0x14);
    displayTable[0] = (u32)D_0038BB50;
    displayTable[1] = (u32)D_0038BB60;
}

INCLUDE_ASM(const s32, "game/code_001360B8", func_001363D8);

INCLUDE_ASM(const s32, "game/code_001360B8", func_00136718);

INCLUDE_ASM(const s32, "game/code_001360B8", fldApplyLightSetCurrent);

INCLUDE_ASM(const s32, "game/code_001360B8", fldApplyLightSetIndex);

INCLUDE_ASM(const s32, "game/code_001360B8", func_00136C90);

s32 fldComposeFadeColor(s32 fade, s32 color, s32 alpha) {
    s32 scaled;

    if (fade < 0) {
        fade = 0;
    }
    scaled = alpha * D_00436170 / 100;
    if (fade < 0xE0) {
        return color | (scaled << 24);
    }
    scaled = (1.0f - (f32)(fade - 0xE0) * 0.00390625f) * scaled;
    if (scaled < 0) {
        scaled = 0;
    }
    if (scaled > 0x80) {
        scaled = 0x80;
    }
    return color | (scaled << 24);
}

INCLUDE_SDATA(const s32, "game/code_001360B8", D_00436168);

INCLUDE_SDATA(const s32, "game/code_001360B8", D_0043616C);

INCLUDE_SDATA(const s32, "game/code_001360B8", D_00436170);

