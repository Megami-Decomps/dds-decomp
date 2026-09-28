#include "common.h"

extern u8 D_00435B88;

extern u32 D_00435B80;

extern u32 D_00438D80;

extern u32 D_00435B8C;

extern u32 D_00435B90;

extern u32 D_00435B94;

extern u8 D_00435B89;

INCLUDE_ASM(const s32, "game/code_00100000", func_00100000);

INCLUDE_ASM(const s32, "game/code_00100000", _start);

INCLUDE_ASM(const s32, "game/code_00100000", func_001001C8);

INCLUDE_ASM(const s32, "game/code_00100000", func_001001D0);

INCLUDE_ASM(const s32, "game/code_00100000", func_001001D8);

void func_001003E8(void) {
    D_00435B88 = 1;
}

u32 func_001003F8(void) {
    return D_00435B80;
}

u32 func_00100400(void) {
    return D_00438D80;
}

void func_00100408(void) {
    D_00435B90 = 0;
    D_00435B8C = 1;
    D_00435B94 = 0;
}

void func_00100420(void) {
    D_00435B8C = 0;
    D_00435B90 = 0;
    D_00435B94 = 0;
}

void func_00100430(u32 arg0) {
    D_00435B90 = arg0;
    D_00435B8C = 1;
    D_00435B94 = 0;
}

INCLUDE_ASM(const s32, "game/code_00100000", func_00100448);

INCLUDE_ASM(const s32, "game/code_00100000", func_00100470);

void func_00100488(void) {
}

void func_00100490(void) {
}

void func_00100498(void) {
    D_00435B89 = 0;
}

void func_001004A0(void) {
    D_00435B89 = 1;
}

INCLUDE_ASM(const s32, "game/code_00100000", func_001004B0);

INCLUDE_ASM(const s32, "game/code_00100000", func_001005C8);

INCLUDE_ASM(const s32, "game/code_00100000", func_00100740);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435B80);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435B84);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435B88);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435B89);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435B8C);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435B90);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435B94);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435B98);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435B9C);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435BA0);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435BA4);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435BA8);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435BAA);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435BAC);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435BB0);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435BB4);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435BB8);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435BC0);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435BC8);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435BD0);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435BD4);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435BD8);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435BDC);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435BE0);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435BE4);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435BE8);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435BEC);

INCLUDE_SDATA(const s32, "game/code_00100000", D_00435BF0);

