#ifndef BTL_TASK_POLL_RESULT_H
#define BTL_TASK_POLL_RESULT_H

#include "common.h"

/* btlRunTask continues at zero and enters end-delay for any nonzero result. */
typedef s32 BtlTaskPollResult;
enum {
    BTL_TASK_POLL_CONTINUE = 0,
};

#endif /* BTL_TASK_POLL_RESULT_H */
