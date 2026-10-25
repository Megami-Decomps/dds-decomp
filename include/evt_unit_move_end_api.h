#ifndef EVT_UNIT_MOVE_END_API_H
#define EVT_UNIT_MOVE_END_API_H

#include "common.h"
#include "evt_unit.h"

/* Whether the unit reached its movement target or selected path endpoint. */
typedef s32 EvtUnitMoveEndResult;
enum {
    EVT_UNIT_MOVE_END_NOT_REACHED = 0,
    EVT_UNIT_MOVE_END_REACHED = 1
};

EvtUnitMoveEndResult evtCheckMoveEnd(evtUnitHandle_t *unit);

#endif /* EVT_UNIT_MOVE_END_API_H */
