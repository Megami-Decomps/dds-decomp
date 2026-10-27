#ifndef KWLN_TASK_UPDATE_COUNT_H
#define KWLN_TASK_UPDATE_COUNT_H

#include "kwln.h"

/* Number of non-skipped scheduler update passes recorded for a task. */
typedef u32 KwlnTaskUpdateCount;

/* The API returns this count; it resets on creation/activation and increments
 * after callback handling, except when an active callback finishes immediately. */
KwlnTaskUpdateCount dds3GetProcessRunTime(const dds3ProcessID_t *task);

#endif /* KWLN_TASK_UPDATE_COUNT_H */
