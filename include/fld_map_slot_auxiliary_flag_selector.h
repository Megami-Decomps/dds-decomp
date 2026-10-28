#ifndef FLD_MAP_SLOT_AUXILIARY_FLAG_SELECTOR_H
#define FLD_MAP_SLOT_AUXILIARY_FLAG_SELECTOR_H

#include "common.h"

/* Positive values select a map-slot auxiliary flag that hides the collision
 * record when set; zero and negative values bypass that gate. */
typedef s32 FldMapSlotAuxiliaryFlagSelector;

#endif
