#ifndef EVT_UNIT_LIGHT_ANIMATION_STATUS_H
#define EVT_UNIT_LIGHT_ANIMATION_STATUS_H

#include "common.h"
#include "evt_unit.h"

typedef u8 EvtUnitLightAnimationStatus;
enum {
    EVT_UNIT_LIGHT_ANIMATION_INACTIVE = 0,
    EVT_UNIT_LIGHT_ANIMATION_ACTIVE = 1
};

EvtUnitLightAnimationStatus evtUnitLightAnimeStat(evtUnitHandle_t *unit);

#endif /* EVT_UNIT_LIGHT_ANIMATION_STATUS_H */
