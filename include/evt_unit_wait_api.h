#ifndef EVT_UNIT_WAIT_API_H
#define EVT_UNIT_WAIT_API_H

#include "common.h"
#include "evt_unit.h"

/* Whether a unit's motion has satisfied an event wait. */
typedef s32 EvtUnitWaitResult;
enum {
    EVT_UNIT_WAIT_IN_PROGRESS = 0,
    EVT_UNIT_WAIT_COMPLETE = 1
};

EvtUnitWaitResult evtCheckUnitWait(evtUnitHandle_t *unit);

#endif /* EVT_UNIT_WAIT_API_H */
