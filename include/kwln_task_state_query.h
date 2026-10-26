#ifndef KWLN_TASK_STATE_QUERY_H
#define KWLN_TASK_STATE_QUERY_H

#include "kwln_task_state.h"

struct dds3ProcessID_t;

/* Return the scheduler state, mapping unregistered or invalid states to DETACHED. */
KwlnTaskState dds3GetProcessStat(struct dds3ProcessID_t *task);

#endif /* KWLN_TASK_STATE_QUERY_H */
