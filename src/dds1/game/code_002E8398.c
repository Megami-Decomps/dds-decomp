#include "sdk_ps2.h"
#include "snd_ring.h"
#include "common.h"
#include "eff_random.h"
#include "eff_random_mod.h"
#include "clock_time.h"
#include "sdf_cd_clock.h"
#include "sdf_kernel_thread_param.h"

extern s8 sdfSndBurstReqMode;
extern u16 D_00398910[];

extern EffRandState D_00398938;

void sdfRandomize(EffRandState *state, u32 seed);
s32 sdfGetCdClock(ClockTime *now);


/* Uniform float in [0, 1): 24 random bits scaled by 2^-24. */
f32 dds3GetRandFloatA(EffRandState *state) {
    u32 value = dds3GetRandA(state) & 0xFFFFFF;

    return (f32)value * 5.9604644775390625e-8f;
}

u32 dds3GetRandIntA(EffRandState *state, u32 modulus) {
    return dds3GetRandA(state) % modulus;
}

/* Seed the generator state (the shared effect state when `state` is NULL); each word is the previous one rotated right 8 bits and xored with a constant. */
void sdfRandomize(EffRandState *state, u32 seed) {
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
void sdfRandomizeByTime(EffRandState *state) {
    ClockTime now;
    s32 secondsSinceEpoch;

    sdfGetCdClock(&now);
    secondsSinceEpoch = now.year * 0x16E + now.day + D_00398910[now.month + 7] + 0xFFF4FF83;
    secondsSinceEpoch = secondsSinceEpoch * 0x18 + now.hour;
    secondsSinceEpoch = secondsSinceEpoch * 0x3C + now.min;
    sdfRandomize(state, secondsSinceEpoch * 0x3C + now.sec);
}

extern s32 sdfBcdToDecimal(s32);
extern u8 D_003BD480;
ClockTime D_003BD488 = {0};

/* Copy cached time to now; return 0 for a valid refresh, -1 for failure, or 1 for reuse.
 * The native 20-tick refresh gate and month-as-year conversion are retained. */
s32 sdfGetCdClock(ClockTime *now) {
    SdfCdClock clock;
    s32 result = 1;
    s32 year;

    if (D_003BD480 == 0) {
        sceCdReadClock(&clock);
        result = -1;
        if ((clock.status & 0x103) == 0) {
            D_003BD488.sec = sdfBcdToDecimal(clock.second);
            D_003BD488.min = sdfBcdToDecimal(clock.minute);
            D_003BD488.hour = sdfBcdToDecimal(clock.hour);
            D_003BD488.day = sdfBcdToDecimal(clock.day);
            D_003BD488.month = sdfBcdToDecimal(clock.month);
            /* Retail reuses the month byte as the two-digit year input. */
            year = sdfBcdToDecimal(clock.month);
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

extern SndRingPacket sndCommandRing[32];
/* Shared with the producer and its worker-thread wakeups. */
extern vu16 sdfSndCmdIndex;
extern vu16 sdfSndCmdTailIndex;

/* Attempt one queued SIF packet: 0 means sent, 1 means empty or rejected. */
s32 sndTryDispatchQueuedCommand(void) {
    s32 readIndex = (s16)sdfSndCmdIndex;
    SndRingPacket *packet;

    if (readIndex == (s16)sdfSndCmdTailIndex) {
        return 1;
    }
    packet = &sndCommandRing[readIndex];
    if (sceSifSendCmd(0, packet, (packet->command >> 24) & 0xf0, 0, 0, 0) == 0) {
        return 1;
    }
    readIndex = (readIndex + 1) & 31;
    sdfSndCmdIndex = readIndex;
    return 0;
}



void sdfSndCmdIFThread(void) {
    for (;;) {
        do {
            SleepThread();
        } while (sdfSndBurstReqMode != 0);
        while (sndTryDispatchQueuedCommand() == 0) {
        }
    }
}

extern SdfKernelThreadParam sndCommandThreadParameters;
extern u8 sndCommandThreadStack[];
extern u8 sndRpcServerThreadStack[];
extern void *_gp;
extern s32 sdfSndCmdBuf;
extern s32 D_003BD4A4;
extern s32 CreateThreadSimple(void (*entryAddress)(), void *workspace, s32 stackBytes, s32 priority);
extern void SifRpcServerThreadProc(void);

s32 sndStartIopCommandAndRpcWorkers(void) {
    s32 threadId;

    while (sceSifGetSreg(31) == 0) {
    }

    sndCommandThreadParameters.entry = sdfSndCmdIFThread;
    sndCommandThreadParameters.stack = sndCommandThreadStack;
    sndCommandThreadParameters.stackSize = 0x1000;
    sndCommandThreadParameters.priority = 0x42;
    sndCommandThreadParameters.gp = &_gp;
    sdfSndCmdIndex = 0;
    sdfSndCmdTailIndex = 0;
    threadId = CreateThread(&sndCommandThreadParameters);
    sdfSndCmdBuf = threadId;
    _StartThread(threadId, 0);
    threadId = CreateThreadSimple(SifRpcServerThreadProc, sndRpcServerThreadStack, 0x1000, 0x43);
    D_003BD4A4 = threadId;
    return _StartThread(threadId, 0);
}

