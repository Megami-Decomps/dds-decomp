#include "common.h"

extern u32 D_003BB024;

extern u32 D_003BB028;
extern u32 D_003BB02C;
extern u32 D_003BB030;
extern u32 D_003BB034;

void func_00161838(s32 arg0, u32 arg1, s32 arg2, s32 arg3) {
    D_003BB030 = (u32)arg0;
    D_003BB028 = D_003BB030;
    if (arg2 != 0) {
        D_003BB030 = (u32)arg2;
    }
    if (arg3 != 0) {
        arg0 = arg3;
    }
    D_003BB02C = arg1;
    D_003BB034 = (s32)arg0;
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

INCLUDE_ASM(const s32, "game/code_00161838", func_00161878);

INCLUDE_ASM(const s32, "game/code_00161838", effBTLFieldColorGetBaseColor);

INCLUDE_ASM(const s32, "game/code_00161838", func_001619A0);

INCLUDE_ASM(const s32, "game/code_00161838", func_001619C8);

u32 func_001619E8(void) {
    return 1;
}

void func_001619F0(u32 arg0) {
    D_003BB024 = D_003BB024 | arg0;
}

void func_00161A00(u32 arg0) {
    D_003BB024 = D_003BB024 & ~arg0;
}

void func_00161A18(void) {
    D_003BB024 = 0;
}

INCLUDE_ASM(const s32, "game/code_00161838", func_00161A20);

void func_00161A88(void) {
    func_00161A20();
}

