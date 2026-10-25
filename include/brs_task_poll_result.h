#ifndef BRS_TASK_POLL_RESULT_H
#define BRS_TASK_POLL_RESULT_H

#include "common.h"

/* The poll result is separate from the 0/1/2 lifecycle: COMPLETE is consumed
 * as INACTIVE, and only ACTIVE reports that work is still running. */
typedef s32 BrsTaskPollResult;
enum {
    BRS_TASK_POLL_RESULT_INACTIVE = 0,
    BRS_TASK_POLL_RESULT_ACTIVE = 1
};

#endif /* BRS_TASK_POLL_RESULT_H */
