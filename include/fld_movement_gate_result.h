#ifndef FLD_MOVEMENT_GATE_RESULT_H
#define FLD_MOVEMENT_GATE_RESULT_H

#include "common.h"

/* Transition and damage-reaction updates report whether player movement can
 * proceed this tick; FldMovementGateState stores the separate countdown. */
typedef s32 FldMovementGateResult;
enum {
    FLD_MOVEMENT_GATE_PROCEED = 0,
    FLD_MOVEMENT_GATE_HELD = -1
};

#endif /* FLD_MOVEMENT_GATE_RESULT_H */
