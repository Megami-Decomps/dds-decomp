#ifndef BTL_ACTION_STATE_HANDLER_H
#define BTL_ACTION_STATE_HANDLER_H

#include "common.h"

typedef struct BtlActionStateHandler {
    void (*start)(void *);
    void (*update)(void *);
    const char *name; /* Debug label read by the task-list printer. */
} BtlActionStateHandler;

#endif
