#ifndef EVT_STAGE_MODEL_SCALE_H
#define EVT_STAGE_MODEL_SCALE_H

#include "common.h"

/* Confirmed prefix view of a selected stage-test model table entry. */
typedef struct EvtStageModelScalePrefix {
    u8 opaque00[0x10];
    f32 scaleFactor;
} EvtStageModelScalePrefix;

typedef char EvtStageModelScalePrefix_size[
    (sizeof(EvtStageModelScalePrefix) == 0x14 &&
     (u32)&((EvtStageModelScalePrefix *)0)->scaleFactor == 0x10)
        ? 1 : -1];

#endif
