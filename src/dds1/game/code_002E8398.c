#include "snd_ring.h"
#include "common.h"
#include "eff_random.h"
#include "eff_random_mod.h"
#include "clock_time.h"
#include "sdf_cd_clock.h"

extern s8 D_003BDA80;
extern u16 D_00398910[];
extern void *sceSifAllocIopHeap(s32 size);

extern EffRandState D_00398938;

void effMiscSeedRandom(EffRandState *state, u32 seed);
s32 effMiscReadCachedClockTime(ClockTime *now);


/* Uniform float in [0, 1): 24 random bits scaled by 2^-24. */
f32 effMiscRandUnitFloat(void *state) {
    u32 value = effMiscRand(state) & 0xFFFFFF;

    return (f32)value * 5.9604644775390625e-8f;
}

u32 effMiscRandMod(EffRandState *state, u32 modulus) {
    return effMiscRand(state) % modulus;
}

/* Seed the generator state (the shared effect state when `state` is NULL); each word is the previous one rotated right 8 bits and xored with a constant. */
void effMiscSeedRandom(EffRandState *state, u32 seed) {
    if (state == NULL) {
        state = &D_00398938;
    }
    seed ^= 0x0AED1A0C;
    state->words[0] = seed;
    seed = (seed << 24) | (seed >> 8);
    state->words[1] = seed;
    seed ^= 0xAA5A02FE;
    seed = (seed << 24) | (seed >> 8);
    state->words[2] = seed;
    seed ^= 0x11BE81C7;
    seed = (seed << 24) | (seed >> 8);
    state->words[3] = seed;
}

/* Seeds the generator at `state` from the current clock (time of day in seconds since an epoch). */
void effMiscSeedRandomFromClock(EffRandState *state) {
    ClockTime now;
    s32 t;

    effMiscReadCachedClockTime(&now);
    t = now.year * 0x16E + now.day + D_00398910[now.month + 7] + 0xFFF4FF83;
    t = t * 0x18 + now.hour;
    t = t * 0x3C + now.min;
    effMiscSeedRandom(state, t * 0x3C + now.sec);
}

extern s32 func_002F4460(SdfCdClock *);
extern s32 sdfBcdStrToInt(s32);
extern u8 D_003BD480;
ClockTime D_003BD488 = {0};

/* Copy cached time to now; return 0 for a valid refresh, -1 for failure, or 1 for reuse.
 * The native 20-tick refresh gate and month-as-year conversion are retained. */
s32 effMiscReadCachedClockTime(ClockTime *now) {
    SdfCdClock clock;
    s32 result = 1;
    s32 year;

    if (D_003BD480 == 0) {
        func_002F4460(&clock);
        result = -1;
        if ((clock.status & 0x103) == 0) {
            D_003BD488.sec = sdfBcdStrToInt(clock.second);
            D_003BD488.min = sdfBcdStrToInt(clock.minute);
            D_003BD488.hour = sdfBcdStrToInt(clock.hour);
            D_003BD488.day = sdfBcdStrToInt(clock.day);
            D_003BD488.month = sdfBcdStrToInt(clock.month);
            /* Retail reuses the month byte as the two-digit year input. */
            year = sdfBcdStrToInt(clock.month);
            result = 0;
            D_003BD488.year = year + (year < 70 ? 2000 : 1900);
        }
        D_003BD480 = 20;
    }
    *now = D_003BD488;
    return result;
}

u32 sndReserveIopWorkMemory(s32 size) {
    return (u32)sceSifAllocIopHeap(size);
}

extern SndRingPacket D_003FB080[32];
/* Shared with the producer and its worker-thread wakeups. */
extern vu16 D_003BDA82;
extern vu16 D_003BDA84;
extern s32 sceSifSendCmd(s32 command, void *packet, s32 size, void *source, void *destination, s32 extraSize);

/* Attempt one queued SIF packet: 0 means sent, 1 means empty or rejected. */
s32 sndTryDispatchQueuedCommand(void) {
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
        while (sndTryDispatchQueuedCommand() == 0) {
        }
    }
}

/* Native CreateThread storage has six modeled fields and an untouched tail.
 * The static descriptor and sdfCreateThread's cleared workspace both span 0x30.
 * _gp is the linker's GP address, as in the EE kernel API. */
typedef struct {
    s32 status;
    void (*entry)(void);
    void *stack;
    s32 stackSize;
    void *gp;
    s32 priority;
    u8 opaque18[0x18];
} SdfKernelThreadParam; /* native descriptor storage: 0x30 bytes */

extern SdfKernelThreadParam D_003FB000;
extern u8 D_003FC080[];
extern u8 D_003FD0C0[];
extern void *_gp;
extern s32 D_003BDA88;
extern s32 D_003BD4A4;
extern s32 func_0030C970(s32);
extern s32 CreateThread(SdfKernelThreadParam *);
extern s32 _StartThread(s32, s32);
extern s32 sdfCreateThread(void *entryAddress, void *workspace, s32 stackBytes, s32 priority);
extern void sdfSoundStartRpcServer(void);

s32 func_002E8700(void) {
    s32 threadId;

    while (func_0030C970(31) == 0) {
    }

    D_003FB000.entry = sdfServiceUnblockedWorkerThread;
    D_003FB000.stack = D_003FC080;
    D_003FB000.stackSize = 0x1000;
    D_003FB000.priority = 0x42;
    D_003FB000.gp = &_gp;
    D_003BDA82 = 0;
    D_003BDA84 = 0;
    threadId = CreateThread(&D_003FB000);
    D_003BDA88 = threadId;
    _StartThread(threadId, 0);
    threadId = sdfCreateThread(sdfSoundStartRpcServer, D_003FD0C0, 0x1000, 0x43);
    D_003BD4A4 = threadId;
    return _StartThread(threadId, 0);
}

