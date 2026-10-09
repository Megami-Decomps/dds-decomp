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

void sdfMotionInitializeAtZeroTime(struct Motion *motion, s32 motionIndex, s32 loopEnabled);
struct Motion *sdfCreateMotion(struct SdfModel *model, struct MotionTable *table);
void sdfDestroyMotion(struct Motion *motion);
void sdfMotionInitialize(struct Motion *motion, s32 motionIndex, s32 loopEnabled,
                         f32 blendLeadFrames, f32 blendDurationFrames);
void sdfMotionSampleAtFrame(struct Motion *motion, f32 frame);
s32 sdfMotionUpdate(struct Motion *motion);
void sdfMotionSuspend(struct Motion *motion);
void sdfMotionResume(struct Motion *motion);

#endif /* SDF_MOTION_H */
