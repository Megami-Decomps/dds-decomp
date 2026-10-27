#ifndef MDL_RESOURCE_ENTRIES_H
#define MDL_RESOURCE_ENTRIES_H

#include "mdl.h"

/* Key for a part-info record list; -1 selects the context-wide list. */
typedef s32 MdlPartInfoListKey;
enum {
    MDL_PART_INFO_LIST_CONTEXT_WIDE = -1
};

/* Effect-item owner slot; -1 tags context-wide items outside per-slot replacement. */
typedef s32 MdlMotionSlotIndex;
enum {
    MDL_MOTION_SLOT_CONTEXT_WIDE = -1
};

/* Create items from a part-info list and scope motion-specific items to their owner slot. */
void mdlGenEffect(dds3ModelHandle_t *owner, MdlPartInfoListKey partInfoListKey,
                  MdlMotionSlotIndex motionSlotIndex);
/* Release all items tagged with the given slot. */
void mdlStopEffect(dds3ModelHandle_t *owner, MdlMotionSlotIndex motionSlotIndex);

#endif /* MDL_RESOURCE_ENTRIES_H */
