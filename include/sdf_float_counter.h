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

#define SDF_FLOAT_COUNTER_STEP_FINISHED 0
#define SDF_FLOAT_COUNTER_STEP_CONTINUES 1

/* Advance one counter step unless frozen. FINISHED means a non-looping
 * counter was already at its endpoint; a frozen counter still CONTINUES. */
s32 dds3PathBasicControlStep(SdfFloatCounter *counter);
void dds3PathBasicControlAlterMode(SdfFloatCounter *counter, u32 direction);
u32 dds3PathBasicControlGetMode(const SdfFloatCounter *counter);
f32 dds3PathBasicControlGetRatio(const SdfFloatCounter *counter);
void dds3PathBasicControlSetRatio(SdfFloatCounter *counter, f32 multiplier);
void dds3PathBasicPauseOn(SdfFloatCounter *counter);
void dds3PathBasicPauseOff(SdfFloatCounter *counter);
void dds3PathBasicLoopOn(SdfFloatCounter *counter);
void dds3PathBasicLoopOff(SdfFloatCounter *counter);

#endif /* SDF_FLOAT_COUNTER_H */
