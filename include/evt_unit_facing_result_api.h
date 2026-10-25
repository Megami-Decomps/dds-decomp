#ifndef EVT_UNIT_FACING_RESULT_API_H
#define EVT_UNIT_FACING_RESULT_API_H

#include "common.h"
#include "evt_unit.h"

/* Whether a facing update applied a rotation step or was already aligned. */
typedef s32 EvtUnitFacingTargetResult;
enum {
    EVT_UNIT_ROTATION_STEP_APPLIED = 0,
    EVT_UNIT_ALREADY_FACING_TARGET = 1
};

EvtUnitFacingTargetResult evtRotateUnitTowardTarget(evtUnitHandle_t *unit, f32 *direction, f32 scale);
EvtUnitFacingTargetResult evtUnitRotationQuat(evtUnitHandle_t *unit, f32 *quat, f32 angle);

#endif /* EVT_UNIT_FACING_RESULT_API_H */
