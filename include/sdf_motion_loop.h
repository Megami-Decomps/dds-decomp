#ifndef SDF_MOTION_LOOP_H
#define SDF_MOTION_LOOP_H

#include "sdf_motion.h"

typedef enum SdfMotionLoopMode {
    SDF_MOTION_LOOP_DISABLED = 0,
    SDF_MOTION_LOOP_ENABLED = 1
} SdfMotionLoopMode;

void sdfMotionChange(struct sdfMotionCtrlr_t *motion, s32 motionIndex, SdfMotionLoopMode loopMode);
void sdfMotionChangeBlend(struct sdfMotionCtrlr_t *motion, s32 motionIndex, SdfMotionLoopMode loopMode,
                         f32 blendLeadFrames, f32 blendDurationFrames);

#endif /* SDF_MOTION_LOOP_H */
