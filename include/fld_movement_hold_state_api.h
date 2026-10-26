#ifndef FLD_MOVEMENT_HOLD_STATE_API_H
#define FLD_MOVEMENT_HOLD_STATE_API_H

#include "common.h"

typedef u8 FldMovementHoldState;
enum {
    FLD_MOVEMENT_HOLD_CLEAR = 0,
    FLD_MOVEMENT_HOLD_ACTIVE = 1
};

FldMovementHoldState fldGetMovementHoldState(void);

#endif /* FLD_MOVEMENT_HOLD_STATE_API_H */
