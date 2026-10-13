#include "snd_ring.h"
#include "common.h"
#include "eff_random.h"
#include "eff_random_mod.h"
#include "clock_time.h"
#include "sdf_cd_clock.h"


extern u16 D_0040BAC0[];

extern EffRandState D_0040BAE8;

void effMiscSeedRandom(EffRandState *state, u32 seed);
s32 effMiscReadCachedClockTime(ClockTime *now);

extern s8 D_004391E0;
extern void *sceSifAllocIopHeap(s32);

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
        state = &D_0040BAE8;
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
    t = now.year * 0x16E + now.day + D_0040BAC0[now.month + 7] + 0xFFF4FF83;
    t = t * 0x18 + now.hour;
    t = t * 0x3C + now.min;
    effMiscSeedRandom(state, t * 0x3C + now.sec);
}

extern s32 func_0034D308(SdfCdClock *);
extern s32 sdfBcdStrToInt(s32);
extern u8 D_00438B70;
extern ClockTime D_00438B78;

/* Copy cached time to now; return 0 for a valid refresh, -1 for failure, or 1 for reuse.
 * The native 20-tick refresh gate and month-as-year conversion are retained. */
INCLUDE_SDATA(const s32, "game/code_00341240", D_00438B78);

s32 effMiscReadCachedClockTime(ClockTime *now) {
    SdfCdClock clock;
    s32 result = 1;
    s32 year;

    if (D_00438B70 == 0) {
        func_0034D308(&clock);
        result = -1;
        if ((clock.status & 0x103) == 0) {
            D_00438B78.sec = sdfBcdStrToInt(clock.second);
            D_00438B78.min = sdfBcdStrToInt(clock.minute);
            D_00438B78.hour = sdfBcdStrToInt(clock.hour);
            D_00438B78.day = sdfBcdStrToInt(clock.day);
            D_00438B78.month = sdfBcdStrToInt(clock.month);
            /* Retail reuses the month byte as the two-digit year input. */
            year = sdfBcdStrToInt(clock.month);
            result = 0;
            D_00438B78.year = year + (year < 70 ? 2000 : 1900);
        }
        D_00438B70 = 20;
    }
    *now = D_00438B78;
    return result;
}

u32 sndReserveIopWorkMemory(s32 size) {
    return (u32)sceSifAllocIopHeap(size);
}

extern SndRingPacket D_00477A00[32];
/* Shared with the producer and its worker-thread wakeups. */
extern vu16 D_004391E2;
extern vu16 D_004391E4;
extern s32 sceSifSendCmd(s32 command, void *packet, s32 size, void *source, void *destination, s32 extraSize);

/* Attempt one queued SIF packet: 0 means sent, 1 means empty or rejected. */
s32 sndTryDispatchQueuedCommand(void) {
    s32 readIndex = (s16)D_004391E2;
    SndRingPacket *packet;

    if (readIndex == (s16)D_004391E4) {
        return 1;
    }
    packet = &D_00477A00[readIndex];
    if (sceSifSendCmd(0, packet, (packet->command >> 24) & 0xf0, 0, 0, 0) == 0) {
        return 1;
    }
    readIndex = (readIndex + 1) & 31;
    D_004391E2 = readIndex;
    return 0;
}

void sdfServiceUnblockedWorkerThread(void) {
    for (;;) {
        do {
            SleepThread();
        } while (D_004391E0 != 0);
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

extern SdfKernelThreadParam D_00477980;
extern u8 D_00478A00[];
extern u8 D_00479A40[];
extern void *_gp;
extern s32 D_004391E8;
extern s32 D_00438B94;
extern s32 func_00367BF0(s32);
extern s32 CreateThread(SdfKernelThreadParam *);
extern s32 _StartThread(s32, s32);
extern s32 sdfCreateThread(void *entryAddress, void *workspace, s32 stackBytes, s32 priority);
extern void sdfSoundStartRpcServer(void);

s32 func_003415A8(void) {
    s32 threadId;

    while (func_00367BF0(31) == 0) {
    }

    D_00477980.entry = sdfServiceUnblockedWorkerThread;
    D_00477980.stack = D_00478A00;
    D_00477980.stackSize = 0x1000;
    D_00477980.priority = 0x42;
    D_00477980.gp = &_gp;
    D_004391E2 = 0;
    D_004391E4 = 0;
    threadId = CreateThread(&D_00477980);
    D_004391E8 = threadId;
    _StartThread(threadId, 0);
    threadId = sdfCreateThread(sdfSoundStartRpcServer, D_00479A40, 0x1000, 0x43);
    D_00438B94 = threadId;
    return _StartThread(threadId, 0);
}

