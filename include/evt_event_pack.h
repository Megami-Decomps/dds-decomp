#ifndef EVT_EVENT_PACK_H
#define EVT_EVENT_PACK_H

#include "evt_unit.h"

/* These numeric task IDs resolve to the camp-owned event-pack state. */
EvtPackLoadState *evtGetEventPackLoadState(u32 taskId);
s32 evtGetEventPackLoadedState(u32 taskId);

/* Entry payload layouts vary by resource kind and remain opaque here. */
void *evtFindTaskResourceEntryByKey(u32 taskId, s32 key);

#endif /* EVT_EVENT_PACK_H */
