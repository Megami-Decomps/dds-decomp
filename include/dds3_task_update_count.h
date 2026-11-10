#ifndef DDS3_TASK_UPDATE_COUNT_H
#define DDS3_TASK_UPDATE_COUNT_H

#include "kwln.h"

/* Number of non-skipped scheduler update passes recorded for a task. */
typedef u32 dds3TaskUpdateCount_t;

/* The API returns this count; it resets on creation/activation and increments
 * after callback handling, except when an active callback finishes immediately. */
dds3TaskUpdateCount_t dds3GetProcessRunTime(const dds3ProcessID_t *task);

#endif /* DDS3_TASK_UPDATE_COUNT_H */
