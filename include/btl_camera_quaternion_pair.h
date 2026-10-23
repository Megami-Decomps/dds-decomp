#ifndef BTL_CAMERA_QUATERNION_PAIR_H
#define BTL_CAMERA_QUATERNION_PAIR_H

#include "common.h"

/* Front and back camera rotations stored together in a framing preset. */
typedef struct BtlCameraQuaternionPair {
    f32 front[4]; /* 0x00 */
    f32 back[4];  /* 0x10 */
} BtlCameraQuaternionPair;

typedef char BtlCameraQuaternionPairSizeCheck[
    sizeof(BtlCameraQuaternionPair) == 0x20 ? 1 : -1];
typedef char BtlCameraQuaternionPairBackOffsetCheck[
    (u32)&((BtlCameraQuaternionPair *)0)->back == 0x10 ? 1 : -1];

#endif
