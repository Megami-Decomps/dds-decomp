#ifndef BRS_TASK_LIFECYCLE_STATE_H
#define BRS_TASK_LIFECYCLE_STATE_H

#include "common.h"

/* The secondary staff-panel task is active until its destroy callback marks
 * it complete; completion is consumed by returning the state to idle. */
typedef s8 BrsTaskLifecycleState;

enum BrsTaskLifecycleStateValue {
    BRS_TASK_STATE_IDLE = 0,
    BRS_TASK_STATE_ACTIVE = 1,
    BRS_TASK_STATE_COMPLETE = 2
};

#endif /* BRS_TASK_LIFECYCLE_STATE_H */
