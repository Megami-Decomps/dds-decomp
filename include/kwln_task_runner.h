#ifndef KWLN_TASK_RUNNER_H
#define KWLN_TASK_RUNNER_H

#include "common.h"
#include "kwln.h"

typedef enum KwlnTaskCursorAction {
    /* The callback finished an active task; EndProcess may unlink and free it. */
    KWLN_TASK_CURSOR_ACTION_RELOAD = 0,
    /* RunProcess did not perform the finish transition. */
    KWLN_TASK_CURSOR_ACTION_ADVANCE = 1
} KwlnTaskCursorAction;

/* Sentinel callback results interpreted by RunProcess. */
#define KWLN_TASK_UPDATE_RETAIN_CALLBACK 0
#define KWLN_TASK_UPDATE_FINISH_CALLBACK (-1)

/*
 * Run one task update. A FINISH_CALLBACK result from an active task routes it
 * through EndProcess and returns RELOAD because that transition may unlink and
 * immediately free the task; the queue runner must not read its links. Other
 * paths return ADVANCE.
 */
KwlnTaskCursorAction RunProcess(dds3ProcessID_t *task);

#endif /* KWLN_TASK_RUNNER_H */
