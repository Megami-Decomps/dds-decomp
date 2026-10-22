#ifndef EVT_TASK_H
#define EVT_TASK_H

#include "kwln.h"

/* Event and camp task identities are scheduler task pointers. */
dds3ProcessID_t *evtSearchEventPack(u32 taskId);
void evtMakeEventPackName(s32 taskId, char *buffer);
dds3ProcessID_t *evtLoadModelSe(s32 modelKey, s32 eventTaskId, s32 resourceId);
dds3ProcessID_t *evtLoadEventPack(s32 taskId);
void evtKillEventPack(s32 taskId);

#endif /* EVT_TASK_H */
