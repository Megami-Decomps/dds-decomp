#include "common.h"

extern u32 effMiscRand(void *state);

extern s8 D_004391E0;

INCLUDE_ASM(const s32, "game/code_00341240", func_00341240);

u32 func_003412A0(void *arg0, u32 arg1) {
    return effMiscRand(arg0) % arg1;
}

INCLUDE_ASM(const s32, "game/code_00341240", func_003412D8);

INCLUDE_ASM(const s32, "game/code_00341240", func_00341348);

INCLUDE_ASM(const s32, "game/code_00341240", func_003413F0);

void func_003414D0(void) {
    sceSifAllocIopHeap();
}

INCLUDE_ASM(const s32, "game/code_00341240", func_003414E8);

void func_00341570(void) {
    for (;;) {
        do {
            SleepThread();
        } while (D_004391E0 != 0);
        while (func_003414E8() == 0) {
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00341240", func_003415A8);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438B78);

INCLUDE_SDATA(const s32, "game/code_00341240", D_00438B79);

