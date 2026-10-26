#ifndef EVT_UNIT_MOTION_CALLBACK_RESULT_H
#define EVT_UNIT_MOTION_CALLBACK_RESULT_H

#include "common.h"

/* Whether an event-unit motion callback should keep running or finish its motion. */
typedef s32 EvtUnitMotionCallbackResult;
enum {
    EVT_UNIT_MOTION_CALLBACK_IN_PROGRESS = 0,
    EVT_UNIT_MOTION_CALLBACK_COMPLETE = 1
};

struct evtUnitHandle_t;
typedef EvtUnitMotionCallbackResult (*EvtUnitMotionCallback)(struct evtUnitHandle_t *, s32);

#endif /* EVT_UNIT_MOTION_CALLBACK_RESULT_H */
