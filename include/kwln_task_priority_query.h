#ifndef KWLN_TASK_PRIORITY_QUERY_H
#define KWLN_TASK_PRIORITY_QUERY_H

#include "kwln.h"

/* Returns a borrowed task from the scheduler queues, or NULL when none has this priority. */
KwlnTask *dds3SearchProcessPriority(u32 priority);

#endif /* KWLN_TASK_PRIORITY_QUERY_H */
