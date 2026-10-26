#ifndef EVT_EVENT_PACK_TASK_H
#define EVT_EVENT_PACK_TASK_H

#include "kwln_task_runner.h"

KwlnTaskUpdateResult evtCheckScriptDone(dds3ProcessID_t *task);
void evtDestroyEventPackTask(dds3ProcessID_t *task);

#endif /* EVT_EVENT_PACK_TASK_H */
