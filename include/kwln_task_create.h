#ifndef KWLN_TASK_CREATE_H
#define KWLN_TASK_CREATE_H

#include "kwln.h"

/*
 * Allocate a task and place it in scheduler queues. Returns NULL on allocation
 * failure. On success, the scheduler owns the task handle and frees it after
 * its destroy callback; callers may borrow the handle only until finalization.
 */
dds3ProcessID_t *dds3StartProcess(const char *name, u32 priority, s32 startDelay,
                         s32 destroyDelay, KwlnTaskUpdateCallback update,
                         TaskDestroy destroy, u32 userValue);

#endif /* KWLN_TASK_CREATE_H */
