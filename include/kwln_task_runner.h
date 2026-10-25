#ifndef KWLN_TASK_RUNNER_H
#define KWLN_TASK_RUNNER_H

#include "common.h"
#include "kwln.h"

typedef s32 KwlnTaskCursorAction;

enum {
    KWLN_TASK_CURSOR_ACTION_RELOAD = 0,
    KWLN_TASK_CURSOR_ACTION_ADVANCE = 1
};

/* 0 retains the current callback, -1 finishes it, and any other value is a
 * replacement callback address consumed by the scheduler. */
typedef s32 KwlnTaskUpdateResult;
#define KWLN_TASK_UPDATE_RETAIN_CALLBACK 0
#define KWLN_TASK_UPDATE_FINISH_CALLBACK (-1)

/* Run one task update and tell the queue runner how to continue its cursor. */
KwlnTaskCursorAction RunProcess(dds3ProcessID_t *task);

#endif /* KWLN_TASK_RUNNER_H */
