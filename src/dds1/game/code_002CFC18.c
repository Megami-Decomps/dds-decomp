#include "common.h"

extern u64 func_002CFBB0(u64);

void func_002CFC18(void) {
    u64 temp_v0;

    temp_v0 = func_002CFBB0(0xffffffffffffffff);
    func_002CFB18(temp_v0);
}

INCLUDE_ASM(const s32, "game/code_002CFC18", func_002CFC38);

INCLUDE_ASM(const s32, "game/code_002CFC18", func_002CFC70);

INCLUDE_ASM(const s32, "game/code_002CFC18", func_002CFCF0);

INCLUDE_ASM(const s32, "game/code_002CFC18", func_002CFD50);

void func_002CFD80(s32 *arg0) {
    s32 *piVar1;

    piVar1 = (s32 *)arg0[1];
    if (piVar1 != (s32 *)0x0) {
        arg0[1] = *piVar1;
    }
    *arg0 = (s32)piVar1;
}

INCLUDE_ASM(const s32, "game/code_002CFC18", func_002CFDA0);

INCLUDE_ASM(const s32, "game/code_002CFC18", func_002CFDF0);

INCLUDE_ASM(const s32, "game/code_002CFC18", func_002CFE70);

INCLUDE_ASM(const s32, "game/code_002CFC18", func_002CFEB8);




INCLUDE_SDATA(const s32, "game/code_002CFC18", D_003BD2D8);

