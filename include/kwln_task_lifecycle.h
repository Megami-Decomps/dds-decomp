#ifndef KWLN_TASK_LIFECYCLE_H
#define KWLN_TASK_LIFECYCLE_H

#include "kwln.h"

/* Whether the scheduler accepted a destruction request; this does not mean
 * that the task has completed destruction. */
typedef s32 KwlnTaskKillRequestResult;

#define KWLN_TASK_KILL_REQUEST_REJECTED 0
#define KWLN_TASK_KILL_REQUEST_ACCEPTED 1

/* Reserved invalid task-handle value; lifecycle requests reject it. */
#define KWLN_TASK_HANDLE_INVALID ((dds3ProcessID_t *)-1)

KwlnTaskKillRequestResult dds3KillProcess(dds3ProcessID_t *task, s32 delayTicks);
KwlnTaskKillRequestResult dds3KillProcessName(const char *name, s32 delayTicks);

#endif /* KWLN_TASK_LIFECYCLE_H */
