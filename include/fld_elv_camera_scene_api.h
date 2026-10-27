#ifndef FLD_ELV_CAMERA_SCENE_API_H
#define FLD_ELV_CAMERA_SCENE_API_H

#include "common.h"

/* Signed selector: travel uses ±1, arrival uses ±2, and the sign follows the floor-index direction. */
typedef s32 FldElvCameraSceneMode;
enum FldElvCameraSceneModeValue {
    FLD_ELV_CAMERA_SCENE_MODE_IDLE = 0,
    FLD_ELV_CAMERA_SCENE_MODE_TRAVEL_INCREASING_FLOOR_INDEX = 1,
    FLD_ELV_CAMERA_SCENE_MODE_TRAVEL_DECREASING_FLOOR_INDEX = -1,
    FLD_ELV_CAMERA_SCENE_MODE_ARRIVAL_INCREASING_FLOOR_INDEX = 2,
    FLD_ELV_CAMERA_SCENE_MODE_ARRIVAL_DECREASING_FLOOR_INDEX = -2
};

void fldSetElvCameraScene(FldElvCameraSceneMode mode);

#endif /* FLD_ELV_CAMERA_SCENE_API_H */
