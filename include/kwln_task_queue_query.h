#ifndef KWLN_TASK_QUEUE_QUERY_H
#define KWLN_TASK_QUEUE_QUERY_H

#include "common.h"
#include "kwln.h"

/* Queue size is a scalar count; unknown or detached states have no entries. */
s32 kwlnTaskGetStateList(u32 state);

/* Returns a borrowed queue node, or NULL for detached/unsupported states or exhaustion. */
KwlnTask *kwlnTaskGetByStateAndIndex(u32 state, s32 index);

#endif /* KWLN_TASK_QUEUE_QUERY_H */
