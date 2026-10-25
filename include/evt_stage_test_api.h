#ifndef EVT_STAGE_TEST_API_H
#define EVT_STAGE_TEST_API_H

#include "common.h"

s32 evtStageTestCountFlags(s32 columnKind);

typedef s32 EvtStageTestSelectionResult;

#define EVT_STAGE_TEST_SELECTION_UNCHANGED 0
#define EVT_STAGE_TEST_SELECTION_SCHEDULED 1

EvtStageTestSelectionResult evtStageTestSelectEntry(
    s32 encodedIndex, s32 initialMotionIndex, s32 assetOption);

typedef s32 EvtStageMotionQueueStatus;
enum {
    EVT_STAGE_MOTION_QUEUE_SETTLED = 0,
    EVT_STAGE_MOTION_QUEUE_PENDING = 1
};

EvtStageMotionQueueStatus evtStageTestHasPendingMotion(void);

typedef enum EvtStageMotionKind {
    EVT_STAGE_MOTION_PRIMARY_COLUMN = 0,
    EVT_STAGE_MOTION_SECONDARY_ALLOW_FALLBACK = 1,
    EVT_STAGE_MOTION_SECONDARY_SUPPRESS_FALLBACK = 2
} EvtStageMotionKind;

void evtStageTestQueueMotion(EvtStageMotionKind motionKind, u32 columnIndex);

#endif
