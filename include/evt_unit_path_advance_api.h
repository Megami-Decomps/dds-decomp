#ifndef EVT_UNIT_PATH_ADVANCE_API_H
#define EVT_UNIT_PATH_ADVANCE_API_H

#include "common.h"
#include "evt_unit.h"

/* Whether evtGetNextMovePos advanced a path-backed unit position. */
typedef s32 EvtUnitPathAdvanceResult;
enum {
    EVT_UNIT_PATH_NOT_ADVANCED = 0,
    EVT_UNIT_PATH_ADVANCED = 1
};

EvtUnitPathAdvanceResult evtGetNextMovePos(evtUnitHandle_t *unit);

#endif /* EVT_UNIT_PATH_ADVANCE_API_H */
