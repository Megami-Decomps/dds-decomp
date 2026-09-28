#include "common.h"

extern u32 func_00116FA0(u32);

INCLUDE_ASM(const s32, "game/code_00111838", func_00111838);

void func_001118C0(s32 arg0, u32 arg1) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 8) = arg1;
}

void func_001118D0(s32 arg0, u32 arg1) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 0xc) = arg1;
}

void func_001118E0(s32 arg0) {
    s32 temp_v0;
    u32 temp_v1;

    temp_v0 = *(s32 *)(arg0 + 0x18);
    if (*(s32 *)(temp_v0 + 4) != 0) {
        func_00117170(*(s32 *)(temp_v0 + 4));
    }
    temp_v1 = func_00116FA0(*(u32 *)(temp_v0 + 0xc));
    *(u32 *)(temp_v0 + 4) = temp_v1;
}

void func_00111920(s32 arg0) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v0 = *(s32 *)(arg0 + 0x18);
    temp_v1 = *(s32 *)(temp_v0 + 4);
    if (temp_v1 != 0) {
        func_00117170(temp_v1);
        *(u32 *)(temp_v0 + 4) = 0;
    }
}

u32 func_00111958(s32 arg0) {
    return *(u32 *)(*(s32 *)(arg0 + 0x18) + 4);
}

void func_00111968(void) {
    func_001116A0();
}

INCLUDE_ASM(const s32, "game/code_00111838", func_00111980);

INCLUDE_ASM(const s32, "game/code_00111838", func_001119D0);

INCLUDE_SDATA(const s32, "game/code_00111838", D_00435D90);

