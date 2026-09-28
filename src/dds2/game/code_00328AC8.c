#include "common.h"

extern u64 func_00328A60(u64);

void func_00328AC8(void) {
    u64 temp_v0;

    temp_v0 = func_00328A60(0xffffffffffffffff);
    func_003289C8(temp_v0);
}

INCLUDE_ASM(const s32, "game/code_00328AC8", func_00328AE8);

INCLUDE_ASM(const s32, "game/code_00328AC8", func_00328B20);

INCLUDE_ASM(const s32, "game/code_00328AC8", func_00328BA0);

INCLUDE_ASM(const s32, "game/code_00328AC8", func_00328C00);

void func_00328C30(s32 *arg0) {
    s32 *piVar1;

    piVar1 = (s32 *)arg0[1];
    if (piVar1 != (s32 *)0x0) {
        arg0[1] = *piVar1;
    }
    *arg0 = (s32)piVar1;
}

INCLUDE_ASM(const s32, "game/code_00328AC8", func_00328C50);

INCLUDE_ASM(const s32, "game/code_00328AC8", func_00328CA0);

INCLUDE_ASM(const s32, "game/code_00328AC8", func_00328D20);

INCLUDE_ASM(const s32, "game/code_00328AC8", func_00328D68);

INCLUDE_SDATA(const s32, "game/code_00328AC8", D_004389C8);

