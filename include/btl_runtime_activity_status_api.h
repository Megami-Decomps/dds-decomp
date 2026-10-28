#ifndef BTL_RUNTIME_ACTIVITY_STATUS_API_H
#define BTL_RUNTIME_ACTIVITY_STATUS_API_H

#include "common.h"

typedef s32 BtlRuntimeActivityStatus;
enum {
    BTL_RUNTIME_ACTIVITY_NONE = 0,
    BTL_RUNTIME_ACTIVITY_PENDING = 1,
};

/* Whether the result panel must wait for battle tasks, events, or effects. */
BtlRuntimeActivityStatus btlHasPendingRuntimeActivity(void);

#endif /* BTL_RUNTIME_ACTIVITY_STATUS_API_H */
