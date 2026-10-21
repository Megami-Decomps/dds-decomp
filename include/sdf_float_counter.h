#ifndef SDF_FLOAT_COUNTER_H
#define SDF_FLOAT_COUNTER_H

#include "common.h"

/* The path and movie counters share this complete four-word state. */
typedef struct SdfFloatCounter {
    s32 direction; /* Zero counts up, one counts down. */
    u32 flags;
    f32 duration;
    f32 time;
} SdfFloatCounter;

typedef char SdfFloatCounter_size_must_be_0x10[
    sizeof(SdfFloatCounter) == 0x10 ? 1 : -1];
typedef char SdfFloatCounter_duration_at_0x08[
    ((u32)&((SdfFloatCounter *)0)->duration == 0x08) ? 1 : -1];
typedef char SdfFloatCounter_time_at_0x0C[
    ((u32)&((SdfFloatCounter *)0)->time == 0x0C) ? 1 : -1];

s32 sdfStepWrappingFloatCounter(SdfFloatCounter *counter);
void sdfSetFloatCounterDirection(SdfFloatCounter *counter, u32 direction);
u32 sdfGetFloatCounterDirection(const SdfFloatCounter *counter);
f32 evtGetValueScaleFactor(const SdfFloatCounter *counter);
void evtScaleValueByMultiplier(SdfFloatCounter *counter, f32 multiplier);
void sdfFreezeFloatCounter(SdfFloatCounter *counter);
void sdfUnfreezeFloatCounter(SdfFloatCounter *counter);
void sdfEnableFloatCounterWrap(SdfFloatCounter *counter);
void sdfDisableFloatCounterWrap(SdfFloatCounter *counter);

#endif /* SDF_FLOAT_COUNTER_H */
