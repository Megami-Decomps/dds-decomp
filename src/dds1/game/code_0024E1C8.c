#include "common.h"

extern s32 func_002CB3B8(u32, u32);
extern void func_0024F6F0(s32, s32);


extern u8 D_003AF7A8[];

extern u32 D_003BC4CC;
extern s32 D_0036C698[];
extern u8 D_0036C648[];


INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024E1C8);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024E260);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024E310);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024E3C0);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024E470);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024E5A0);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024E728);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024E8D0);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024EA50);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024EC08);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024EDC0);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024EF68);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024F0D0);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024F210);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024F338);

INCLUDE_RODATA(const s32, "game/code_0024E1C8", D_003AF720);

INCLUDE_RODATA(const s32, "game/code_0024E1C8", D_003AF730);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024F4F0);

s32 func_0024F570(void) {
    s32 i;
    for (i = 0; i < 14; ++i) {
        if (D_0036C698[i] == 0) {
            return 0;
        }
    }
    return 1;
}

void func_0024F5B0(void) {
    s32 i;
    for (i = 0; i < 14; ++i) {
        if (D_0036C698[i] != 0) {
            func_002BDD60(D_0036C698[i]);
            D_0036C698[i] = 0;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024F608);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024F6F0);

void func_0024F760(void) {
    s32 data = func_0024F608();
    D_003BC4CC = func_002CB1C8(D_003AF7A8, 0x402, 0x2B12, D_0036C648, func_0024F6F0, data);
}

s32 func_0024F7A8(void) {
    if (func_002CB2E0(D_003AF7A8) != 0) {
        return 1;
    }
    D_003BC4CC = 0;
    return 0;
}

void func_0024F7D8(void) {
    func_002CB278(D_003BC4CC);
    D_003BC4CC = 0;
}

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024F800);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024F858);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024F8D8);

u32 func_0024FA18(void) {
    s32 temp_v0;

    temp_v0 = func_002CB3B8(D_003BC4CC, 0);
    return *(u32 *)(*(s32 *)(*(s32 *)(temp_v0 + 0xc) + 0x1c) + 0x70);
}

void func_0024FA48(void) {
    s32 object = func_002CB3B8(D_003BC4CC, 0);
    func_0027BED0(*(s32 *)(object + 0xC));
    func_0027BEB0(*(s32 *)(object + 0xC));
}

void func_0024FA88(void) {
    s32 object = func_002CB3B8(D_003BC4CC, 0);
    func_0027BED0(*(s32 *)(object + 0xC));
    func_0027BE90(*(s32 *)(object + 0xC));
}

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024FAC8);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024FB30);

INCLUDE_RODATA(const s32, "game/code_0024E1C8", D_003AF7A8);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024FBB8);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_002501E0);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_00250758);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_00250820);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_002508D8);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_00250978);

