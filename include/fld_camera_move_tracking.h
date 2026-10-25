#ifndef FLD_CAMERA_MOVE_TRACKING_H
#define FLD_CAMERA_MOVE_TRACKING_H

#include "common.h"

typedef s32 FldCameraTrackingResult;

enum {
    FLD_CAMERA_TRACKING_NO_ACTION = 0,
    FLD_CAMERA_TRACKING_MOVE_STARTED = 1,
    FLD_CAMERA_TRACKING_MODE_PENDING = 2,
};

FldCameraTrackingResult fldEvaluateCameraMoveTracking(void);

#endif /* FLD_CAMERA_MOVE_TRACKING_H */
