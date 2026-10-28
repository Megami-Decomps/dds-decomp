#ifndef EVT_UNIT_PATH_MODE_H
#define EVT_UNIT_PATH_MODE_H

#include "common.h"

/* Preserve the signed script-selector width at the path-follow boundary. */
typedef s32 EvtUnitPathFollowMode;
enum {
    EVT_UNIT_PATH_FOLLOW_MODE_TARGET_XZ = 0,
    EVT_UNIT_PATH_FOLLOW_MODE_ENDPOINT = 1
};

/* These modes choose the path-facing flags applied to an event unit. */
typedef s32 EvtUnitPathSideMode;
enum {
    EVT_UNIT_PATH_SIDE_MODE_DEFAULT = 0,
    EVT_UNIT_PATH_SIDE_MODE_SIDE = 1,
    EVT_UNIT_PATH_SIDE_MODE_PATH_ROTATION = 2
};

/* Unrecognized values preserve the prior mode because both switches omit a default. */

#endif /* EVT_UNIT_PATH_MODE_H */
