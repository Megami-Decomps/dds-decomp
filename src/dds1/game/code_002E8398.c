#include "common.h"

extern s8 D_003BDA80;

extern u32 effMiscRand(void *state);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002E8398);

u32 effMiscRandMod(void *state, u32 modulus) {
    return effMiscRand(state) % modulus;
}

INCLUDE_ASM(const s32, "game/code_002E8398", func_002E8430);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002E84A0);

INCLUDE_ASM(const s32, "game/code_002E8398", func_002E8548);

void func_002E8628(void) {
    sceSifAllocIopHeap();
}

INCLUDE_ASM(const s32, "game/code_002E8398", func_002E8640);

void func_002E86C8(void) {
    for (;;) {
        do {
            SleepThread();
        } while (D_003BDA80 != 0);
        while (func_002E8640() == 0) {
        }
    }
}

INCLUDE_ASM(const s32, "game/code_002E8398", func_002E8700);

INCLUDE_SDATA(const s32, "game/code_002E8398", D_003BD488);

INCLUDE_SDATA(const s32, "game/code_002E8398", D_003BD489);

