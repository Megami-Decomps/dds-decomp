#include "common.h"

extern u32 D_00438FA8;

extern u32 D_00438FAC;

void func_00245508(s32 arg0);

extern s8 D_00438FA4;

INCLUDE_ASM(const s32, "game/code_00244F00", func_00244F00);

INCLUDE_ASM(const s32, "game/code_00244F00", func_00245508);

INCLUDE_ASM(const s32, "game/code_00244F00", func_00245590);

INCLUDE_ASM(const s32, "game/code_00244F00", func_00245618);

s32 func_002457A8(void) {
    return D_00438FA4 != 0;
}

void func_002457B8(s32 arg0) {
    if (arg0 == 0) {
        D_00438FA4 = 0;
        D_00438FA8 = 0;
        D_00438FAC = 0;
        return;
    }
    D_00438FAC = (s32)arg0;
    D_00438FA4 = 3;
    D_00438FA8 = 0;
}

void func_002457E0(s32 arg0) {
    if (arg0 == 0) {
        D_00438FA4 = 5;
        D_00438FAC = 1;
        D_00438FA8 = 0;
    } else {
        D_00438FA8 = arg0;
        D_00438FA4 = 5;
        D_00438FAC = arg0;
    }
}

void func_00245810(void) {
}

u32 func_00245818(void) {
    return 0;
}

void func_00245820(void) {
    func_0010BFE0();
}

INCLUDE_ASM(const s32, "game/code_00244F00", func_00245838);

INCLUDE_ASM(const s32, "game/code_00244F00", func_00245880);

INCLUDE_ASM(const s32, "game/code_00244F00", func_002458B8);

INCLUDE_ASM(const s32, "game/code_00244F00", func_00245F80);

INCLUDE_ASM(const s32, "game/code_00244F00", func_00245F88);

INCLUDE_ASM(const s32, "game/code_00244F00", func_00246028);

INCLUDE_ASM(const s32, "game/code_00244F00", func_00246078);

INCLUDE_ASM(const s32, "game/code_00244F00", func_00246108);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_0043722C);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437230);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437234);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437238);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437240);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437248);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437250);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437258);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437260);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437268);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437270);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437278);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437280);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437288);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437290);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_00437298);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_004372A0);

INCLUDE_SDATA(const s32, "game/code_00244F00", D_004372A8);

