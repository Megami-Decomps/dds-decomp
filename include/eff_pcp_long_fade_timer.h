#ifndef EFF_PCP_LONG_FADE_TIMER_H
#define EFF_PCP_LONG_FADE_TIMER_H

#include "eff_blur.h"

typedef struct EffPCPFadeTimerLongParams {
    s32 duration;
    s32 fadeIn;
    s32 fadeOut;
    EffBlurQuad source;
} EffPCPFadeTimerLongParams;

typedef struct EffPCPFadeTimerLong {
    EffPCPFadeTimerLongParams params;
    u32 colorFrom;
    u32 colorTo;
    s32 frame;
} EffPCPFadeTimerLong;

typedef char EffPCPFadeTimerLongParamsSizeCheck[
    (sizeof(EffPCPFadeTimerLongParams) == 0x34) ? 1 : -1];
typedef char EffPCPFadeTimerLongSourceOffsetCheck[
    ((u32)&((EffPCPFadeTimerLongParams *)0)->source == 0x0C) ? 1 : -1];
typedef char EffPCPFadeTimerLongColorFromOffsetCheck[
    ((u32)&((EffPCPFadeTimerLong *)0)->colorFrom == 0x34) ? 1 : -1];
typedef char EffPCPFadeTimerLongColorToOffsetCheck[
    ((u32)&((EffPCPFadeTimerLong *)0)->colorTo == 0x38) ? 1 : -1];
typedef char EffPCPFadeTimerLongFrameOffsetCheck[
    ((u32)&((EffPCPFadeTimerLong *)0)->frame == 0x3C) ? 1 : -1];
typedef char EffPCPFadeTimerLongSizeCheck[
    (sizeof(EffPCPFadeTimerLong) == 0x40) ? 1 : -1];

#endif
