#ifndef SDF_MOTION_H
#define SDF_MOTION_H

#include "common.h"

struct Motion;

typedef enum {
    SDF_MOTION_STATE_UNINITIALIZED = 0,
    SDF_MOTION_STATE_INITIALIZED = 1,
    SDF_MOTION_STATE_SAMPLING = 4,
    SDF_MOTION_STATE_TERMINAL = 5,
    SDF_MOTION_STATE_SUSPENDED = 6,
} SdfMotionState;

void sdfMotionInitializeAtZeroTime(struct Motion *motion, s32 motionIndex, s32 loopEnabled);
s32 sdfMotionUpdate(struct Motion *motion);

#endif /* SDF_MOTION_H */
