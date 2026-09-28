#include "common.h"

extern s32 D_00438930;

extern u32 D_0043891C;

extern s32 func_00317FE0(u32);

extern u32 D_00438918;

/* 0xAARRGGBB color split into RGB and alpha fields. */
typedef struct RgbAlpha {
    u8 pad_0x00[0x18]; // 0x00
    u32 rgb18;         // 0x18
    u8 pad_0x1C[0x1C]; // 0x1C
    u32 alpha38;       // 0x38
    u32 x3C;           // 0x3C
    u8 pad_0x40[0x10]; // 0x40
    float f50;         // 0x50
} RgbAlpha; // 0x54

/* Copy source for func_002CF3F8 (layout inferred from field accesses). */
typedef struct CfSrc {
    u8 pad_0x00[0x04]; // 0x00
    u32 x04;           // 0x04
    u8 pad_0x08[0x1C]; // 0x08
    u32 x24;           // 0x24
    u32 x28;           // 0x28
    u8 pad_0x2C[0x10]; // 0x2C
    float f3C;         // 0x3C
} CfSrc; // 0x40

extern char D_0042D4D0[];

extern s32 func_00101740(const char *arg0);

void func_00316E08(RgbAlpha *p, u32 color) {
    p->rgb18 = color & 0xFFFFFF;
    p->alpha38 = color >> 24;
}

u32 func_00316E28(s32 arg0) {
    return *(u32 *)(arg0 + 0x3c);
}

void func_00316E30(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x3c) = arg1;
}

void func_00316E38(RgbAlpha *dst, CfSrc *src) {
    dst->rgb18 = src->x04;
    dst->f50 = src->f3C;
    dst->alpha38 = src->x24;
    dst->x3C = src->x28;
}

void func_00316E60(void) {
    D_00438918 = 1;
}

void func_00316E70(void) {
    D_00438918 = 0;
}

INCLUDE_ASM(const s32, "game/code_00316E08", func_00316E78);

u8 func_00316ED0(void) {
    return func_00101740(D_0042D4D0) != 0;
}

INCLUDE_ASM(const s32, "game/code_00316E08", func_00316EF8);

INCLUDE_ASM(const s32, "game/code_00316E08", func_00316F40);

INCLUDE_ASM(const s32, "game/code_00316E08", func_00316FA8);

u32 func_00316FC8(void) {
    u32 temp_v0;
    s64 temp_v1;

    temp_v1 = func_00317FE0(D_0043891C);
    if (temp_v1 == -1) {
        func_00128658();
        temp_v0 = 0xffffffff;
    }
    else {
        func_00318068(D_0043891C);
        temp_v0 = 0;
    }
    return temp_v0;
}

INCLUDE_RODATA(const s32, "game/code_00316E08", D_0042D4D0);

INCLUDE_ASM(const s32, "game/code_00316E08", func_00317010);

INCLUDE_ASM(const s32, "game/code_00316E08", func_00317058);

INCLUDE_ASM(const s32, "game/code_00316E08", func_00317958);

INCLUDE_ASM(const s32, "game/code_00316E08", func_00317988);

u32 func_00317AC8(void) {
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_00316E08", D_0042D7A0);

INCLUDE_ASM(const s32, "game/code_00316E08", func_00317AD0);

INCLUDE_ASM(const s32, "game/code_00316E08", func_00317E48);

INCLUDE_ASM(const s32, "game/code_00316E08", func_00317FE0);

INCLUDE_ASM(const s32, "game/code_00316E08", func_00318068);

INCLUDE_ASM(const s32, "game/code_00316E08", func_003180B8);

INCLUDE_ASM(const s32, "game/code_00316E08", func_00318570);

INCLUDE_ASM(const s32, "game/code_00316E08", func_00318660);

INCLUDE_ASM(const s32, "game/code_00316E08", func_00318C00);

INCLUDE_ASM(const s32, "game/code_00316E08", func_003191B0);

INCLUDE_ASM(const s32, "game/code_00316E08", func_00319388);

INCLUDE_ASM(const s32, "game/code_00316E08", func_00319A58);

INCLUDE_ASM(const s32, "game/code_00316E08", func_00319E48);

INCLUDE_ASM(const s32, "game/code_00316E08", func_00319F48);

INCLUDE_ASM(const s32, "game/code_00316E08", func_00319FF0);

void func_0031A090(void) {
    if ((D_00438930 != 0) && ((*(u32 *)(D_00438930 + 4) & 1) != 0)) {
        *(u32 *)(D_00438930 + 4) = *(u32 *)(D_00438930 + 4) | 0x800;
    }
}

INCLUDE_ASM(const s32, "game/code_00316E08", func_0031A0B8);

INCLUDE_SDATA(const s32, "game/code_00316E08", D_0043891C);

INCLUDE_SDATA(const s32, "game/code_00316E08", D_00438920);

INCLUDE_SDATA(const s32, "game/code_00316E08", D_00438928);

INCLUDE_SDATA(const s32, "game/code_00316E08", D_00438930);

INCLUDE_SDATA(const s32, "game/code_00316E08", D_00438934);

INCLUDE_SDATA(const s32, "game/code_00316E08", D_00438938);

INCLUDE_SDATA(const s32, "game/code_00316E08", D_00438940);

INCLUDE_SDATA(const s32, "game/code_00316E08", D_00438944);

INCLUDE_SDATA(const s32, "game/code_00316E08", D_00438948);

INCLUDE_SDATA(const s32, "game/code_00316E08", D_0043894C);

