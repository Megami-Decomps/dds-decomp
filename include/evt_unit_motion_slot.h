#ifndef EVT_UNIT_MOTION_SLOT_H
#define EVT_UNIT_MOTION_SLOT_H

#include "evt_unit.h"

typedef enum EvtUnitMotionSlotMode {
    /* Preserve the signed script-selector domain; unrecognized values add no mode flag. */
    EVT_UNIT_MOTION_SLOT_MODE_INVALID = -1,
    EVT_UNIT_MOTION_SLOT_MODE_LOOP_ON_MOTION_END = 0,
    EVT_UNIT_MOTION_SLOT_MODE_LOOP_EXTERNAL_MOTION = 1,
    EVT_UNIT_MOTION_SLOT_MODE_STOP_ON_BATTLE_ACTION = 2
} EvtUnitMotionSlotMode;

/* Store a motion slot's three parameters and apply its mode flags. */
void evtConfigureUnitMotionSlot(evtUnitHandle_t *unit, s32 slot, s32 parameterA,
                                s32 parameterB, s32 parameterC,
                                EvtUnitMotionSlotMode mode);

#endif
