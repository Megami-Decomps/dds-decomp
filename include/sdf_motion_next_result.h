#ifndef SDF_MOTION_NEXT_RESULT_H
#define SDF_MOTION_NEXT_RESULT_H

#include "common.h"

typedef s32 SdfMotionNextResult;

/* Boundary events reported after advancing one frame of a motion. */
enum SdfMotionNextResultValue {
    SDF_MOTION_NEXT_NO_EVENT = 0,
    SDF_MOTION_NEXT_TERMINAL = 1,
    SDF_MOTION_NEXT_LOOPED = 2
};

struct sdfMotionCtrlr_t;
SdfMotionNextResult sdfMotionNext(struct sdfMotionCtrlr_t *motion);

#endif
