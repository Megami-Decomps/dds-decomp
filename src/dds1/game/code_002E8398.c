#include "snd_ring.h"
#include "common.h"

extern s8 D_003BDA80;
extern u16 D_00398910[];
extern void *sceSifAllocIopHeap(s32 size);

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

extern EffRandState D_00398938;

void effMiscSeedRandom(EffRandState *state, u32 seed);
s32 func_002E8548(ClockTime *now);

extern u32 effMiscRand(void *state);

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
        state = &D_00398938;
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

    func_002E8548(&now);
    t = now.year * 0x16E + now.day + D_00398910[now.month + 7] + 0xFFF4FF83;
    t = t * 0x18 + now.hour;
    t = t * 0x3C + now.min;
    effMiscSeedRandom(state, t * 0x3C + now.sec);
}

INCLUDE_ASM(const s32, "game/code_002E8398", func_002E8548);

u32 sndReserveIopWorkMemory(s32 size) {
    return (u32)sceSifAllocIopHeap(size);
}

extern SndRingPacket D_003FB080[32];
/* Shared with the producer and its worker-thread wakeups. */
extern vu16 D_003BDA82;
extern vu16 D_003BDA84;
extern s32 sceSifSendCmd(s32 command, void *packet, s32 size, void *source, void *destination, s32 extraSize);

s32 func_002E8640(void) {
    s32 readIndex = (s16)D_003BDA82;
    SndRingPacket *packet;

    if (readIndex == (s16)D_003BDA84) {
        return 1;
    }
    packet = &D_003FB080[readIndex];
    if (sceSifSendCmd(0, packet, (packet->command >> 24) & 0xf0, 0, 0, 0) == 0) {
        return 1;
    }
    readIndex = (readIndex + 1) & 31;
    D_003BDA82 = readIndex;
    return 0;
}


void sdfServiceUnblockedWorkerThread(void) {
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

