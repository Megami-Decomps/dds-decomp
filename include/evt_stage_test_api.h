#ifndef EVT_STAGE_TEST_API_H
#define EVT_STAGE_TEST_API_H

#include "common.h"

s32 evtStageTestCountFlags(s32 columnKind);
s32 evtStageTestHasPendingMotion(void);
void evtStageTestQueueMotion(s32 motionKind, u32 columnIndex);

#endif
