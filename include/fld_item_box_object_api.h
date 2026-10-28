#ifndef FLD_ITEM_BOX_OBJECT_API_H
#define FLD_ITEM_BOX_OBJECT_API_H

#include "common.h"

/* Positive values select a map-slot auxiliary flag that hides the collision
 * record when set; zero and negative values bypass that gate. */
typedef s32 FldMapSlotAuxiliaryFlagSelector;

void fldAddItemBoxObject(f32 x, f32 y, f32 z, f32 extent, f32 height,
                         FldMapSlotAuxiliaryFlagSelector auxiliaryFlagSelector);

#endif
