#ifndef DAT_NEW_CYCLE_STATE_H
#define DAT_NEW_CYCLE_STATE_H

#include "common.h"

/* The save header records whether this run was started as a New Cycle. */
typedef s16 DatNewCycleState;

enum DatNewCycleStates {
    DAT_NEW_CYCLE_STATE_INACTIVE = 0,
    DAT_NEW_CYCLE_STATE_ACTIVE = 1
};

#endif
