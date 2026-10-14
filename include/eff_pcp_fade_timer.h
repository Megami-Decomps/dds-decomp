#ifndef EFF_PCP_FADE_TIMER_H
#define EFF_PCP_FADE_TIMER_H

#include "common.h"

/* Parameter block copied by the fade-timer constructor; padding and the two
 * opaque words are retained because the update mutates this copy. */
typedef struct EffPCPFadeTimerParams {
    s32 duration;
    s32 fadeIn;
    s32 fadeOut;
    u32 color;
    u8 pad10[4];
    u32 unk14;
    u32 unk18;
    u32 width;
    u32 height;
} EffPCPFadeTimerParams;

/* Runtime owner adds blend endpoints and the current frame to the input. */
typedef struct EffPCPFadeTimer {
    EffPCPFadeTimerParams params;
    u32 colorFrom;
    u32 colorTo;
    s32 frame;
} EffPCPFadeTimer;

typedef char EffPCPFadeTimerParams_size_must_be_0x24[
    sizeof(EffPCPFadeTimerParams) == 0x24 ? 1 : -1];
typedef char EffPCPFadeTimer_size_must_be_0x30[
    sizeof(EffPCPFadeTimer) == 0x30 ? 1 : -1];

#endif
