#ifndef EVT_MOTION_SE_H
#define EVT_MOTION_SE_H

#include "common.h"

/* Runtime user value for the event's motion-sound task. */
typedef struct EvtMotionSeTaskParams {
    s32 modelKey;
    s32 eventTaskId;
    s32 resourceId;
} EvtMotionSeTaskParams;

typedef char EvtMotionSeTaskParamsSizeCheck[sizeof(EvtMotionSeTaskParams) == 0xC ? 1 : -1];

#endif /* EVT_MOTION_SE_H */
