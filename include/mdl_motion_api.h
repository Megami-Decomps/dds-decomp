#ifndef MDL_MOTION_API_H
#define MDL_MOTION_API_H

#include "common.h"

typedef struct dds3ModelHandle_t dds3ModelHandle_t;

/* Context-level motion APIs; per-sdfMotionCtrlr_t lifecycle methods are in sdf_motion.h. */
s32 mdlGetMotionNum(const dds3ModelHandle_t *ctx, s32 searchId);
u16 mdlGetMotionLengthEx(const dds3ModelHandle_t *ctx, s32 searchId);
s32 mdlGetMotionFrameEx(const dds3ModelHandle_t *ctx, s32 searchId);

/* Returns 0 for nonterminal, 1 for terminal, and 2 when the node is absent. */
s32 mdlChkMotionEndEx(const dds3ModelHandle_t *ctx, s32 searchId);

void mdlSetMotionSpeedEx(dds3ModelHandle_t *ctx, s32 searchId, f32 frameStep);
void mdlMotionPause(dds3ModelHandle_t *ctx);
void mdlMotionPauseOff(dds3ModelHandle_t *ctx);

#endif /* MDL_MOTION_API_H */
