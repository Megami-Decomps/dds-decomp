#include "common.h"

extern u64 func_0016AEB0(u64, u64);

INCLUDE_ASM(const s32, "game/code_0016EDD0", func_0016EDD0);

INCLUDE_ASM(const s32, "game/code_0016EDD0", func_0016EFA8);

u32 func_0016F018(u32 arg0) {
    return arg0;
}

void func_0016F020(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x120) = arg1;
}

INCLUDE_ASM(const s32, "game/code_0016EDD0", func_0016F028);

INCLUDE_ASM(const s32, "game/code_0016EDD0", func_0016F6D0);

INCLUDE_ASM(const s32, "game/code_0016EDD0", func_0016F850);

void func_0016FAD0(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0016AEB0(arg0, 0);
    temp_v1 = func_0016AEB0(arg0, 1);
    func_0016F850(temp_v0, temp_v1);
}

INCLUDE_ASM(const s32, "game/code_0016EDD0", func_0016FB18);

INCLUDE_ASM(const s32, "game/code_0016EDD0", func_0016FD90);

INCLUDE_ASM(const s32, "game/code_0016EDD0", func_0016FE18);

INCLUDE_ASM(const s32, "game/code_0016EDD0", func_00171580);

void func_00171590(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x54) = arg1;
}

INCLUDE_ASM(const s32, "game/code_0016EDD0", func_00171598);

void func_00171798(s32 arg0) {
    func_00333918(*(u32 *)(arg0 + 0x20));
    func_003297C8(*(u32 *)(arg0 + 0x24));
}

void func_001717C8(u32 *arg0) {
    arg0[4] = 3;
    *arg0 = 0x80808080;
    arg0[3] = 0;
}

INCLUDE_ASM(const s32, "game/code_0016EDD0", func_001717E8);

INCLUDE_ASM(const s32, "game/code_0016EDD0", func_001719D0);

INCLUDE_ASM(const s32, "game/code_0016EDD0", func_00171A68);

INCLUDE_ASM(const s32, "game/code_0016EDD0", func_00171CE0);
