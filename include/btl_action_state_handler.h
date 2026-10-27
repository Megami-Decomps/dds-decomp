#ifndef BTL_ACTION_STATE_HANDLER_H
#define BTL_ACTION_STATE_HANDLER_H

#include "btl_action_state_api.h"

typedef struct BtlActionStateHandler {
    void (*start)(BtlActionStateOwner *);
    void (*update)(BtlActionStateOwner *);
    const char *name; /* Debug label read by the task-list printer. */
} BtlActionStateHandler;

#endif
