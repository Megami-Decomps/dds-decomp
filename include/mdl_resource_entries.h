#ifndef MDL_RESOURCE_ENTRIES_H
#define MDL_RESOURCE_ENTRIES_H

#include "mdl.h"

/* Initial context-wide resource records have no associated motion slot. */
#define MDL_CONTEXT_WIDE_MOTION_SLOT (-1)

/* Apply a viewer record list and associate created items with a motion slot. */
void mdlApplyResourceEntries(MdlCtx *owner, s32 recordId, s32 motionSlotIndex);
void mdlRemoveResourcesForMotionSlot(MdlCtx *owner, s32 motionSlotIndex);

#endif /* MDL_RESOURCE_ENTRIES_H */
