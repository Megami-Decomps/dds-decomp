#ifndef KWLN_TASK_QUEUE_QUERY_H
#define KWLN_TASK_QUEUE_QUERY_H

#include "common.h"
#include "kwln.h"
#include "kwln_task_state.h"

/* Zero-based position in the current linked order of one scheduler queue. */
typedef s32 KwlnTaskQueueIndex;

/* Nonnegative queue size; unknown or detached states have no entries. */
typedef s32 KwlnTaskQueueCount;

KwlnTaskQueueCount dds3GetProcessCount(KwlnTaskState state);

/* For nonnegative queueIndex, returns that borrowed queue node or NULL for an
 * unsupported queue or an index beyond its end. Membership changes shift later
 * indexes. */
dds3ProcessID_t *dds3SearchProcessNumber(KwlnTaskState taskState, KwlnTaskQueueIndex queueIndex);

#endif /* KWLN_TASK_QUEUE_QUERY_H */
