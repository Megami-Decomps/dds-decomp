#ifndef DDS3_TASK_RUNNER_H
#define DDS3_TASK_RUNNER_H

#include "common.h"
#include "kwln.h"

typedef enum dds3TaskCursorAction_t {
    /* The callback finished an active task; EndProcess may unlink and free it. */
    DDS3_TASK_CURSOR_ACTION_RELOAD = 0,
    /* RunProcess did not perform the finish transition. */
    DDS3_TASK_CURSOR_ACTION_ADVANCE = 1
} dds3TaskCursorAction_t;

/* Sentinel callback results interpreted by RunProcess. */
#define DDS3_TASK_UPDATE_RETAIN_CALLBACK 0
#define DDS3_TASK_UPDATE_FINISH_CALLBACK (-1)

/*
 * Run one task update. A FINISH_CALLBACK result from an active task routes it
 * through EndProcess and returns RELOAD because that transition may unlink and
 * immediately free the task; the queue runner must not read its links. Other
 * paths return ADVANCE.
 */
dds3TaskCursorAction_t RunProcess(dds3ProcessID_t *task);

#endif /* DDS3_TASK_RUNNER_H */
