#ifndef EVT_UNIT_MOTION_SLOT_H
#define EVT_UNIT_MOTION_SLOT_H

#include "evt_unit.h"

/* Store a motion slot's three parameters and apply its mode flags. */
void evtConfigureUnitMotionSlot(EvtUnit *unit, s32 slot, s32 parameterA, s32 parameterB, s32 parameterC, s32 mode);

#endif
