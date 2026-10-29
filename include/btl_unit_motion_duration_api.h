#ifndef BTL_UNIT_MOTION_DURATION_API_H
#define BTL_UNIT_MOTION_DURATION_API_H

#include "common.h"

struct BtlUnit;

typedef s32 BtlUnitMotionDurationFrames;

BtlUnitMotionDurationFrames btlGetUnitMotionDurationFrames(
    const struct BtlUnit *unit, s32 motionIndex);

#endif /* BTL_UNIT_MOTION_DURATION_API_H */
