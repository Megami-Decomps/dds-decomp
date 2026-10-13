#ifndef BRS_TASK_H
#define BRS_TASK_H

#include "common.h"

/* Return 1 only while the task state is 1. State 2 is consumed and cleared;
 * other states return 0 without changing the state. */
s32 brsTaskConsumeDone(void);

#endif /* BRS_TASK_H */
