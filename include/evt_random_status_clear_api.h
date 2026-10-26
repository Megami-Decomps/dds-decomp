#ifndef EVT_RANDOM_STATUS_CLEAR_API_H
#define EVT_RANDOM_STATUS_CLEAR_API_H

#include "common.h"

/* Whether the random event cleared any status flags on living roster entries. */
typedef s32 EvtRandomStatusClearResult;
enum {
    EVT_RANDOM_STATUS_CLEAR_NO_FLAGS = 0,
    EVT_RANDOM_STATUS_CLEAR_FLAGS_CLEARED = 1
};

EvtRandomStatusClearResult evtClearRandomStatusFlags(void);

#endif /* EVT_RANDOM_STATUS_CLEAR_API_H */
