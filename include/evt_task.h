#ifndef EVT_TASK_H
#define EVT_TASK_H

#include "kwln.h"

/* Event and camp task identities are scheduler task pointers. */
KwlnTask *evtFindTaskById(u32 taskId);
KwlnTask *evtCreateMotionSeTask(s32 modelKey, s32 eventTaskId, s32 resourceId);
KwlnTask *mnuCampCreateTask(s32 taskId);
void mnuCampDestroyTaskById(s32 taskId);

#endif /* EVT_TASK_H */
