#ifndef BTL_PAIR_CAMERA_PRESET_H
#define BTL_PAIR_CAMERA_PRESET_H

#include "common.h"

/* Shared action-camera preset row used by both title camera providers. */
typedef struct BattlePairCameraPreset {
    f32 fromQuaternion[4]; /* 0x00 */
    f32 toQuaternion[4];   /* 0x10 */
    f32 fromDistanceScale; /* 0x20 */
    f32 toDistanceScale;   /* 0x24 */
    f32 fromHeightScale;   /* 0x28 */
    f32 motionParameter;   /* 0x2C */
} BattlePairCameraPreset;

typedef char BattlePairCameraPresetSizeCheck[
    (sizeof(BattlePairCameraPreset) == 0x30) ? 1 : -1];
typedef char BattlePairCameraPresetFromQuaternionOffsetCheck[
    ((u32)&((BattlePairCameraPreset *)0)->fromQuaternion == 0x00) ? 1 : -1];
typedef char BattlePairCameraPresetToQuaternionOffsetCheck[
    ((u32)&((BattlePairCameraPreset *)0)->toQuaternion == 0x10) ? 1 : -1];
typedef char BattlePairCameraPresetFromDistanceOffsetCheck[
    ((u32)&((BattlePairCameraPreset *)0)->fromDistanceScale == 0x20) ? 1 : -1];
typedef char BattlePairCameraPresetToDistanceOffsetCheck[
    ((u32)&((BattlePairCameraPreset *)0)->toDistanceScale == 0x24) ? 1 : -1];
typedef char BattlePairCameraPresetFromHeightOffsetCheck[
    ((u32)&((BattlePairCameraPreset *)0)->fromHeightScale == 0x28) ? 1 : -1];
typedef char BattlePairCameraPresetMotionParameterOffsetCheck[
    ((u32)&((BattlePairCameraPreset *)0)->motionParameter == 0x2C) ? 1 : -1];

#endif /* BTL_PAIR_CAMERA_PRESET_H */
