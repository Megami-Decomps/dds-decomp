#include "common.h"

extern u32 effMiscRand(void *state);

extern u16 D_0040BAC0[];

typedef struct ClockTime {
    u8 pad0;
    u8 sec;   /* 0x1 */
    u8 min;   /* 0x2 */
    u8 hour;  /* 0x3 */
    u8 day;   /* 0x4 */
    u8 month; /* 0x5 */
    u16 year; /* 0x6 */
} ClockTime;

typedef struct EffRandState {
    u32 x[4];
} EffRandState;

extern EffRandState D_0040BAE8;

void effMiscSeedRandom(EffRandState *state, u32 seed);
s32 func_003413F0(ClockTime *now);

extern s8 D_004391E0;
extern void *sceSifAllocIopHeap(s32);

/* Uniform float in [0, 1): 24 random bits scaled by 2^-24. */
f32 effMiscRandUnitFloat(void *state) {
    u32 value = effMiscRand(state) & 0xFFFFFF;

    return (f32)value * 5.9604644775390625e-8f;
}

u32 effMiscRandMod(void *state, u32 modulus) {
    return effMiscRand(state) % modulus;
}

/* Seed the generator state (the shared effect state when `state` is NULL); each word is the previous one rotated right 8 bits and xored with a constant. */
void effMiscSeedRandom(EffRandState *state, u32 seed) {
    if (state == NULL) {
        state = &D_0040BAE8;
    }
    seed ^= 0x0AED1A0C;
    state->x[0] = seed;
    seed = (seed << 24) | (seed >> 8);
    state->x[1] = seed;
    seed ^= 0xAA5A02FE;
    seed = (seed << 24) | (seed >> 8);
    state->x[2] = seed;
    seed ^= 0x11BE81C7;
    seed = (seed << 24) | (seed >> 8);
    state->x[3] = seed;
}

/* Seeds the generator at `state` from the current clock (time of day in seconds since an epoch). */
void effMiscSeedRandomFromClock(void *state) {
    ClockTime now;
    s32 t;

    func_003413F0(&now);
    t = now.year * 0x16E + now.day + D_0040BAC0[now.month + 7] + 0xFFF4FF83;
    t = t * 0x18 + now.hour;
    t = t * 0x3C + now.min;
    effMiscSeedRandom(state, t * 0x3C + now.sec);
}

INCLUDE_ASM(const s32, "game/code_00341240", func_003413F0);

u32 sndReserveIopWorkMemory(s32 size) {
    return (u32)sceSifAllocIopHeap(size);
}

INCLUDE_ASM(const s32, "game/code_00341240", func_003414E8);

void sdfServiceUnblockedWorkerThread(void) {
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

