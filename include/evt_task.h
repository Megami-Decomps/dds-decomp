#ifndef EVT_TASK_H
#define EVT_TASK_H

#include "kwln.h"
#include "evt_event_pack.h"
#include "evt_motion_se.h"

/* Event-pack IDs identify camp tasks; lookup returns the scheduler handle. */
dds3ProcessID_t *evtSearchEventPack(EvtPackEventId eventId);
void evtMakeEventPackName(EvtPackEventId eventId, char *buffer);
EvtMotionSeModelKey evtInitMotionSeProcess(EvtPackEventId eventId, s32 resourceId);
/* The task reads borrowed event-pack state while updating. Link it under the
 * matching event-pack task so teardown cancels it before releasing that state. */
dds3ProcessID_t *evtLoadModelSe(EvtMotionSeModelKey modelKey, EvtPackEventId eventId, s32 resourceId);
dds3ProcessID_t *evtLoadEventPack(EvtPackEventId eventId);
void evtKillEventPack(EvtPackEventId eventId);

#endif /* EVT_TASK_H */
