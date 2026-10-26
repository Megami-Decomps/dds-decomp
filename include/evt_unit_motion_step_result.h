#ifndef EVT_UNIT_MOTION_STEP_RESULT_H
#define EVT_UNIT_MOTION_STEP_RESULT_H

#include "common.h"

/* Whether an event-unit update applied a movement step this frame. */
typedef s32 EvtUnitMotionStepResult;
enum {
    EVT_UNIT_MOTION_STEP_NOT_APPLIED = 0,
    EVT_UNIT_MOTION_STEP_APPLIED = 1
};

#endif /* EVT_UNIT_MOTION_STEP_RESULT_H */
