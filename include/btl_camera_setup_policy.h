#ifndef BTL_CAMERA_SETUP_POLICY_H
#define BTL_CAMERA_SETUP_POLICY_H

#include "common.h"

/* Complete 0x60-byte camera-setup records. The seven bank boundaries and
 * their native 0x60 indexing establish the record and array extents. */
typedef struct BtlCameraSetupPointPolicy {
    u8 kind;
    u8 mode;
    u8 reserved02[2];
    f32 side;
    f32 distance;
    f32 height;
} BtlCameraSetupPointPolicy;

typedef struct BtlCameraSetupPolicy {
    f32 singleFov;
    f32 multipleFov;
    u8 reserved08[8];
    f32 boundsSeed[4];
    f32 boundsRadius;
    BtlCameraSetupPointPolicy focusPolicy;
    BtlCameraSetupPointPolicy eyePolicy;
    BtlCameraSetupPointPolicy terminalPolicy;
    u8 reserved54[12];
} BtlCameraSetupPolicy;

#endif /* BTL_CAMERA_SETUP_POLICY_H */
