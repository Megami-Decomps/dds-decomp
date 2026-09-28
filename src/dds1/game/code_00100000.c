#include "common.h"

extern u8 D_003BA709;

extern u32 D_003BA70C;
extern u32 D_003BA710;
extern u32 D_003BA714;

extern u32 D_003BD680;

extern u32 D_003BA700;

extern u8 D_003BA708;

INCLUDE_ASM(const s32, "game/code_00100000", func_00100000);

INCLUDE_ASM(const s32, "game/code_00100000", _start);

INCLUDE_ASM(const s32, "game/code_00100000", func_001001C8);

INCLUDE_ASM(const s32, "game/code_00100000", func_001001D0);

INCLUDE_ASM(const s32, "game/code_00100000", func_001001D8);

void func_00100500(void) {
    D_003BA708 = 1;
}

u32 func_00100510(void) {
    return D_003BA700;
}

u32 func_00100518(void) {
    return D_003BD680;
}

void func_00100520(void) {
    D_003BA710 = 0;
    D_003BA70C = 1;
    D_003BA714 = 0;
}

void func_00100538(void) {
    D_003BA70C = 0;
    D_003BA710 = 0;
    D_003BA714 = 0;
}

void func_00100548(u32 arg0) {
    D_003BA710 = arg0;
    D_003BA70C = 1;
    D_003BA714 = 0;
}

INCLUDE_ASM(const s32, "game/code_00100000", func_00100560);

INCLUDE_ASM(const s32, "game/code_00100000", func_00100588);

void func_001005A0(void) {
}

void func_001005A8(void) {
}

void func_001005B0(void) {
    D_003BA709 = 0;
}

void func_001005B8(void) {
    D_003BA709 = 1;
}

INCLUDE_ASM(const s32, "game/code_00100000", func_001005C8);

INCLUDE_ASM(const s32, "game/code_00100000", func_001006E0);

INCLUDE_ASM(const s32, "game/code_00100000", func_00100858);
