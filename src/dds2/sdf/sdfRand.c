#include "sdk_ps2.h"
#include "snd_queued_command_dispatch_api.h"
#include "snd_ring.h"
#include "common.h"
#include "eff_random.h"
#include "eff_random_mod.h"
#include "clock_time.h"
#include "sdf_cd_clock.h"
#include "sdf_kernel_thread_param.h"

extern u16 D_0040BAC0[];

extern EffRandState D_0040BAE8;

void sdfRandomize(EffRandState *state, u32 seed);

s32 sdfGetCdClock(ClockTime *now);

/* Seed the generator state (the shared effect state when `state` is NULL); each word is the previous one rotated right 8 bits and xored with a constant. */
void sdfRandomize(EffRandState *state, u32 seed) {
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
void sdfRandomizeByTime(EffRandState *state) {
    ClockTime now;
    s32 t;

    sdfGetCdClock(&now);
    t = now.year * 0x16E + now.day + D_0040BAC0[now.month + 7] + 0xFFF4FF83;
    t = t * 0x18 + now.hour;
    t = t * 0x3C + now.min;
    sdfRandomize(state, t * 0x3C + now.sec);
}

extern s32 sdfBcdToDecimal(s32);

extern u8 D_00438B70;

ClockTime D_00438B78 = {0};

/* Copy cached time to now; return 0 for a valid refresh, -1 for failure, or 1 for reuse.
 * The native 20-tick refresh gate and month-as-year conversion are retained. */
s32 sdfGetCdClock(ClockTime *now) {
    SdfCdClock clock;
    s32 result = 1;
    s32 year;

    if (D_00438B70 == 0) {
        sceCdReadClock(&clock);
        result = -1;
        if ((clock.status & 0x103) == 0) {
            D_00438B78.sec = sdfBcdToDecimal(clock.second);
            D_00438B78.min = sdfBcdToDecimal(clock.minute);
            D_00438B78.hour = sdfBcdToDecimal(clock.hour);
            D_00438B78.day = sdfBcdToDecimal(clock.day);
            D_00438B78.month = sdfBcdToDecimal(clock.month);
            /* Retail reuses the month byte as the two-digit year input. */
            year = sdfBcdToDecimal(clock.month);
            result = 0;
            D_00438B78.year = year + (year < 70 ? 2000 : 1900);
        }
        D_00438B70 = 20;
    }
    *now = D_00438B78;
    return result;
}
