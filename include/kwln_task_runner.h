#ifndef KWLN_TASK_RUNNER_H
#define KWLN_TASK_RUNNER_H

#include "common.h"
#include "kwln.h"

typedef s32 KwlnTaskCursorAction;

enum {
    KWLN_TASK_CURSOR_ACTION_RELOAD = 0,
    KWLN_TASK_CURSOR_ACTION_ADVANCE = 1
};

/* Run one task update and tell the queue runner how to continue its cursor. */
KwlnTaskCursorAction RunProcess(dds3ProcessID_t *task);

#endif /* KWLN_TASK_RUNNER_H */
