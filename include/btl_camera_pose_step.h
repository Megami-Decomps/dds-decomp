#ifndef BTL_CAMERA_POSE_STEP_H
#define BTL_CAMERA_POSE_STEP_H

#include "btl_command.h"

/* The camera update keeps the pose mode active until a step reports complete. */
typedef s32 BtlCameraPoseStepResult;
enum {
    BTL_CAMERA_POSE_STEP_CONTINUE = 0,
    BTL_CAMERA_POSE_STEP_COMPLETE = 1,
};

BtlCameraPoseStepResult btlStepPoseBlendHalf(BtlLinkedCommand *command);
BtlCameraPoseStepResult btlStepPoseBlend(BtlLinkedCommand *command);
BtlCameraPoseStepResult btlStepPoseBlendFrame(BtlLinkedCommand *command);
BtlCameraPoseStepResult btlStepPoseBlendRatio(BtlLinkedCommand *command);

#endif /* BTL_CAMERA_POSE_STEP_H */
