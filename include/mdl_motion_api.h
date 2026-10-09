#ifndef MDL_MOTION_API_H
#define MDL_MOTION_API_H

#include "common.h"

typedef struct MdlCtx MdlCtx;

/* Context-level motion APIs; per-Motion lifecycle methods are in sdf_motion.h. */
s32 mdlGetNodeMotionIndex(MdlCtx *ctx, s32 searchId);
u16 mdlGetNodeFrameCount(MdlCtx *ctx, s32 searchId);
s32 mdlGetNodeFrameAsInt(MdlCtx *ctx, s32 searchId);

/* Returns 0 for nonterminal, 1 for terminal, and 2 when the node is absent. */
s32 mdlGetNodeMotionTerminalStatus(MdlCtx *ctx, s32 searchId);

void mdlSetNodeFrameStep(MdlCtx *ctx, s32 searchId, f32 value);
void mdlSuspendAllContextMotions(MdlCtx *ctx);
void mdlResumeAllContextMotions(MdlCtx *ctx);

#endif /* MDL_MOTION_API_H */
