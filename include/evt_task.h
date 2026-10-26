#ifndef EVT_TASK_H
#define EVT_TASK_H

#include "kwln.h"
#include "evt_motion_se.h"

/* Event and camp task identities are scheduler task pointers. */
dds3ProcessID_t *evtSearchEventPack(s32 taskId);
void evtMakeEventPackName(s32 taskId, char *buffer);
EvtMotionSeModelKey evtInitMotionSeProcess(s32 eventId, s32 resourceId);
dds3ProcessID_t *evtLoadModelSe(EvtMotionSeModelKey modelKey, s32 eventTaskId, s32 resourceId);
dds3ProcessID_t *evtLoadEventPack(s32 taskId);
void evtKillEventPack(s32 taskId);

#endif /* EVT_TASK_H */
