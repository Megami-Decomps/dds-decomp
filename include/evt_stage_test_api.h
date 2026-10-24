#ifndef EVT_STAGE_TEST_API_H
#define EVT_STAGE_TEST_API_H

#include "common.h"

s32 evtStageTestCountFlags(s32 columnKind);
s32 evtStageTestHasPendingMotion(void);

typedef enum EvtStageMotionKind {
    EVT_STAGE_MOTION_PRIMARY_COLUMN = 0,
    EVT_STAGE_MOTION_SECONDARY_ALLOW_FALLBACK = 1,
    EVT_STAGE_MOTION_SECONDARY_SUPPRESS_FALLBACK = 2
} EvtStageMotionKind;

void evtStageTestQueueMotion(EvtStageMotionKind motionKind, u32 columnIndex);

#endif
