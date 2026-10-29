#ifndef EVT_VIEWER_OBJECT_SLOT_API_H
#define EVT_VIEWER_OBJECT_SLOT_API_H

#include "evt_viewer.h"

/* Returns an EvtRuntime.objects index, or NOT_FOUND when all 127 slots are occupied. */
typedef s32 EvtViewerObjectSlotResult;
enum {
    EVT_VIEWER_OBJECT_SLOT_NOT_FOUND = -1
};

EvtViewerObjectSlotResult evtViewerCreateObjectInFreeSlot(
    s32 unused, EvtRuntimeGroup *track, EvtRuntimeChild *key, evtViewerInfo_t *viewer);

#endif /* EVT_VIEWER_OBJECT_SLOT_API_H */
