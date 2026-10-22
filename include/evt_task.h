#ifndef EVT_TASK_H
#define EVT_TASK_H

#include "kwln.h"

/* Event and camp task identities are scheduler task pointers. */
KwlnTask *evtSearchEventPack(u32 taskId);
void evtMakeEventPackName(s32 taskId, char *buffer);
KwlnTask *evtLoadModelSe(s32 modelKey, s32 eventTaskId, s32 resourceId);
KwlnTask *evtLoadEventPack(s32 taskId);
void evtKillEventPack(s32 taskId);

#endif /* EVT_TASK_H */
