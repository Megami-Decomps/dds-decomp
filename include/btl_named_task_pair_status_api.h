#ifndef BTL_NAMED_TASK_PAIR_STATUS_API_H
#define BTL_NAMED_TASK_PAIR_STATUS_API_H

#include "common.h"

typedef s32 BtlNamedTaskPairStatus;
/* Zero stays unnamed: it represents both blocked work and raw state zero. */
enum {
    BTL_NAMED_TASK_PAIR_STATUS_UNAVAILABLE = -128,
    BTL_NAMED_TASK_PAIR_STATUS_CLOSE_IN_PROGRESS = 1,
    BTL_NAMED_TASK_PAIR_STATUS_CLOSE_COMPLETE = 2,
    BTL_NAMED_TASK_PAIR_STATUS_OPEN_IN_PROGRESS = 3,
};

BtlNamedTaskPairStatus btlGetNamedTaskPairStatusOrUnavailable(void);

#endif /* BTL_NAMED_TASK_PAIR_STATUS_API_H */
