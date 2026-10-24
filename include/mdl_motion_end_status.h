#ifndef MDL_MOTION_END_STATUS_H
#define MDL_MOTION_END_STATUS_H

#include "mdl_motion_api.h"

typedef s32 MdlMotionEndStatus;
enum {
    MDL_MOTION_END_NOT_TERMINAL = 0,
    MDL_MOTION_END_TERMINAL = 1,
    MDL_MOTION_END_NODE_MISSING = 2
};

/* Return the terminal state, or the distinct missing-node status. */
MdlMotionEndStatus mdlChkMotionEndEx(const dds3ModelHandle_t *ctx, s32 searchId);

#endif /* MDL_MOTION_END_STATUS_H */
