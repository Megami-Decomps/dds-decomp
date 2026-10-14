#ifndef EVT_UNIT_COLOR_TRANSITION_H
#define EVT_UNIT_COLOR_TRANSITION_H

#include "evt_unit.h"

/* Initialize an event unit's color transition for the requested duration. */
void evtInitializeUnitColorTransition(EvtUnit *unit, s32 duration, u32 firstColor, u32 secondColor);

#endif /* EVT_UNIT_COLOR_TRANSITION_H */
