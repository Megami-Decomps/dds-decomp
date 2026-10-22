#ifndef MDL_MOTION_API_H
#define MDL_MOTION_API_H

#include "common.h"

typedef struct MdlCtx MdlCtx;

/* Context-level motion APIs; per-Motion lifecycle methods are in sdf_motion.h. */
s32 mdlGetMotionNum(const MdlCtx *ctx, s32 searchId);
u16 mdlGetMotionLengthEx(const MdlCtx *ctx, s32 searchId);
s32 mdlGetMotionFrameEx(const MdlCtx *ctx, s32 searchId);

/* Returns 0 for nonterminal, 1 for terminal, and 2 when the node is absent. */
s32 mdlChkMotionEndEx(const MdlCtx *ctx, s32 searchId);

void mdlSetMotionSpeedEx(MdlCtx *ctx, s32 searchId, f32 frameStep);
void mdlMotionPause(MdlCtx *ctx);
void mdlMotionPauseOff(MdlCtx *ctx);

#endif /* MDL_MOTION_API_H */
