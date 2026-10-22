#ifndef SDF_MOTION_H
#define SDF_MOTION_H

#include "common.h"

struct Motion;
struct MotionTable;
struct SdfModel;

typedef enum {
    SDF_MOTION_STATE_UNINITIALIZED = 0,
    SDF_MOTION_STATE_INITIALIZED = 1,
    SDF_MOTION_STATE_SAMPLING = 4,
    SDF_MOTION_STATE_TERMINAL = 5,
    SDF_MOTION_STATE_SUSPENDED = 6,
} SdfMotionState;

void sdfMotionChange(struct Motion *motion, s32 motionIndex, s32 loopEnabled);
void *sdfMotionNodeNew(struct Motion *motion, u32 commandWord, s32 targetIndex);
struct Motion *sdfMotionCtrlrNew(struct SdfModel *model, struct MotionTable *table);
void sdfMotionCtrlrFree(struct Motion *motion);
void sdfMotionChangeBlend(struct Motion *motion, s32 motionIndex, s32 loopEnabled,
                         f32 blendLeadFrames, f32 blendDurationFrames);
void sdfMotionSeek(struct Motion *motion, f32 frame);
s32 sdfMotionNext(struct Motion *motion);
void sdfMotionPause(struct Motion *motion);
void sdfMotionPauseOff(struct Motion *motion);

#endif /* SDF_MOTION_H */
