#ifndef BRS_RESULT_COUNTER_H
#define BRS_RESULT_COUNTER_H

#include "common.h"

/* Cross-title completion view; bytes without confirmed meaning stay opaque. */
typedef struct BrsResultCounterView {
    u8 pad00[0x14];
    s8 phase;
    u8 pad15[0x1F];
    s32 remaining;
    u8 pad38[0x14];
    s8 pad4C[1];
    u8 pad4D[0x13];
    s8 pad60[8];
} BrsResultCounterView;

typedef char BrsResultCounterView_size_must_be_0x68[(sizeof(BrsResultCounterView) == 0x68) ? 1 : -1];
typedef char BrsResultCounterView_phase_must_be_at_0x14[
    ((u32)&((BrsResultCounterView *)0)->phase == 0x14) ? 1 : -1];
typedef char BrsResultCounterView_remaining_must_be_at_0x34[
    ((u32)&((BrsResultCounterView *)0)->remaining == 0x34) ? 1 : -1];
typedef char BrsResultCounterView_pad4C_must_be_at_0x4C[
    ((u32)&((BrsResultCounterView *)0)->pad4C == 0x4C) ? 1 : -1];
typedef char BrsResultCounterView_pad60_must_be_at_0x60[
    ((u32)&((BrsResultCounterView *)0)->pad60 == 0x60) ? 1 : -1];

#endif
