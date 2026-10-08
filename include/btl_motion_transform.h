#ifndef BTL_MOTION_TRANSFORM_H
#define BTL_MOTION_TRANSFORM_H

#include "btl_command.h"

/* Copies both vectors, distance and field of view. */
void btlCopyMotionTransform(BtlCamState *destination, BtlCamState *source);
/* Field of view is stored in radians. */
void btlSetMotionTransformFieldOfView(BtlCamState *object, f32 fovRadians);

/* Both vector inputs contain four floats; direction supplies a quaternion.
 * Initializes the camera pose and clears the runtime motion-update flag. */
void btlInitMotionTransformFromVectors(BtlCamState *object, f32 *origin,
                                       f32 *direction);
/* The final input is a field-of-view angle in degrees. */
void btlInitMotionTransformFromComponents(BtlCamState *object,
    f32 x, f32 y, f32 z, f32 vx, f32 vy, f32 vz, f32 vw, f32 fovDegrees);

#endif /* BTL_MOTION_TRANSFORM_H */
