#include "common.h"

extern u32 effMiscRand(void *state);

extern s8 D_004391E0;

/* Uniform float in [0, 1): 24 random bits scaled by 2^-24. */
f32 func_00341240(void *state) {
    u32 value = effMiscRand(state) & 0xFFFFFF;

    return (f32)value * 5.9604644775390625e-8f;
}

u32 effMiscRandMod(void *state, u32 modulus) {
    return effMiscRand(state) % modulus;
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

