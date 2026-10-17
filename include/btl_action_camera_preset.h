#ifndef BTL_ACTION_CAMERA_PRESET_H
#define BTL_ACTION_CAMERA_PRESET_H

#include "common.h"

/* Action-filtered camera-pair preset consumed by both title providers. */
typedef struct BtlActionCameraPreset {
    f32 frontQuaternion[4]; /* 0x00 */
    f32 backQuaternion[4]; /* 0x10 */
    f32 frontDistanceScale; /* 0x20 */
    f32 backDistanceScale; /* 0x24 */
    f32 frontCameraYScale; /* 0x28 */
    u32 actionMask; /* 0x2C */
} BtlActionCameraPreset;

typedef char BtlActionCameraPresetSizeCheck[
    (sizeof(BtlActionCameraPreset) == 0x30) ? 1 : -1];
typedef char BtlActionCameraPresetFrontQuaternionOffsetCheck[
    ((u32)&((BtlActionCameraPreset *)0)->frontQuaternion == 0x00) ? 1 : -1];
typedef char BtlActionCameraPresetBackQuaternionOffsetCheck[
    ((u32)&((BtlActionCameraPreset *)0)->backQuaternion == 0x10) ? 1 : -1];
typedef char BtlActionCameraPresetFrontDistanceScaleOffsetCheck[
    ((u32)&((BtlActionCameraPreset *)0)->frontDistanceScale == 0x20) ? 1 : -1];
typedef char BtlActionCameraPresetBackDistanceScaleOffsetCheck[
    ((u32)&((BtlActionCameraPreset *)0)->backDistanceScale == 0x24) ? 1 : -1];
typedef char BtlActionCameraPresetFrontCameraYScaleOffsetCheck[
    ((u32)&((BtlActionCameraPreset *)0)->frontCameraYScale == 0x28) ? 1 : -1];
typedef char BtlActionCameraPresetActionMaskOffsetCheck[
    ((u32)&((BtlActionCameraPreset *)0)->actionMask == 0x2C) ? 1 : -1];

#endif /* BTL_ACTION_CAMERA_PRESET_H */
