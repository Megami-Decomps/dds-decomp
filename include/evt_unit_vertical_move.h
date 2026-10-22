#ifndef EVT_UNIT_VERTICAL_MOVE_H
#define EVT_UNIT_VERTICAL_MOVE_H

#include "evt_unit.h"

/* Simulate the motion and return the step count stored in unit->stepCount. */
s32 evtUnitPrepareVerticalMoveSteps(evtUnitHandle_t *unit);

#endif /* EVT_UNIT_VERTICAL_MOVE_H */
