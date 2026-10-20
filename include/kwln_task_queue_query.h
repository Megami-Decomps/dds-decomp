#ifndef KWLN_TASK_QUEUE_QUERY_H
#define KWLN_TASK_QUEUE_QUERY_H

#include "common.h"
#include "kwln.h"

/* Queue size is a scalar count; unknown or detached states have no entries. */
s32 dds3GetProcessCount(u32 state);

/* Returns a borrowed queue node, or NULL for detached/unsupported states or exhaustion. */
KwlnTask *dds3SearchProcessNumber(u32 taskState, s32 taskIndex);

#endif /* KWLN_TASK_QUEUE_QUERY_H */
