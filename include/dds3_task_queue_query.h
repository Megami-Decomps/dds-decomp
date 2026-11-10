#ifndef DDS3_TASK_QUEUE_QUERY_H
#define DDS3_TASK_QUEUE_QUERY_H

#include "common.h"
#include "kwln.h"
#include "dds3_task_state.h"

/* Zero-based position in the current linked order of one scheduler queue. */
typedef s32 dds3TaskQueueIndex_t;

/* Nonnegative queue size; unknown or detached states have no entries. */
typedef s32 dds3TaskQueueCount_t;

dds3TaskQueueCount_t dds3GetProcessCount(dds3TaskState_t state);

/* For nonnegative queueIndex, returns that borrowed queue node or NULL for an
 * unsupported queue or an index beyond its end. Membership changes shift later
 * indexes. */
dds3ProcessID_t *dds3SearchProcessNumber(dds3TaskState_t taskState, dds3TaskQueueIndex_t queueIndex);

#endif /* DDS3_TASK_QUEUE_QUERY_H */
