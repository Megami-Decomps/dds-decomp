#ifndef KWLN_TASK_QUEUE_QUERY_H
#define KWLN_TASK_QUEUE_QUERY_H

#include "common.h"
#include "kwln.h"
#include "kwln_task_state.h"

/* Queue size is a scalar count; unknown or detached states have no entries. */
s32 dds3GetProcessCount(KwlnTaskState state);

/* Returns a borrowed queue node, or NULL for detached/unsupported states or exhaustion. */
dds3ProcessID_t *dds3SearchProcessNumber(KwlnTaskState taskState, s32 taskIndex);

#endif /* KWLN_TASK_QUEUE_QUERY_H */
