#ifndef FLD_ACTOR_MOTION_STEP_RESULT_API_H
#define FLD_ACTOR_MOTION_STEP_RESULT_API_H

#include "common.h"

/* APPLIED writes all three coordinates for a kind-2 row; NOT_APPLIED leaves them unchanged. */
typedef s32 FldActorMotionStepResult;
enum {
    FLD_ACTOR_MOTION_STEP_NOT_APPLIED = 0,
    FLD_ACTOR_MOTION_STEP_APPLIED = 1
};

FldActorMotionStepResult fldStepActorMotionByKey(u32 key, f32 *xOut, f32 *yOut, f32 *zOut);

#endif /* FLD_ACTOR_MOTION_STEP_RESULT_API_H */
