#ifndef BTL_CAMERA_PATH_PRESET_H
#define BTL_CAMERA_PATH_PRESET_H

#include "common.h"

/* Camera-path preset banks have a native 0x60-byte record stride. */
typedef struct BtlCameraPathPreset {
    u16 kind;              /* 0x00 */
    s8 operation;          /* 0x02: endpoint construction mode */
    u8 pad03;
    f32 startFrame;        /* 0x04 */
    f32 duration;          /* 0x08 */
    f32 angleScale;        /* 0x0C */
    f32 firstOffset[3];    /* 0x10 */
    u8 pad1C[4];
    f32 secondOffset[3];   /* 0x20 */
    u8 pad2C[4];
    u8 unk30;
    u8 pad31[0xF];
    f32 unk40;
    u8 pad44[0xC];
    f32 blendRate;         /* 0x50 */
    u8 pad54[0xC];
} BtlCameraPathPreset;

typedef char BtlCameraPathPresetSizeCheck[
    (sizeof(BtlCameraPathPreset) == 0x60) ? 1 : -1];
typedef char BtlCameraPathPresetOperationOffsetCheck[
    ((u32)&((BtlCameraPathPreset *)0)->operation == 0x02) ? 1 : -1];
typedef char BtlCameraPathPresetStartFrameOffsetCheck[
    ((u32)&((BtlCameraPathPreset *)0)->startFrame == 0x04) ? 1 : -1];
typedef char BtlCameraPathPresetDurationOffsetCheck[
    ((u32)&((BtlCameraPathPreset *)0)->duration == 0x08) ? 1 : -1];
typedef char BtlCameraPathPresetAngleScaleOffsetCheck[
    ((u32)&((BtlCameraPathPreset *)0)->angleScale == 0x0C) ? 1 : -1];
typedef char BtlCameraPathPresetFirstOffsetCheck[
    ((u32)&((BtlCameraPathPreset *)0)->firstOffset == 0x10) ? 1 : -1];
typedef char BtlCameraPathPresetSecondOffsetCheck[
    ((u32)&((BtlCameraPathPreset *)0)->secondOffset == 0x20) ? 1 : -1];
typedef char BtlCameraPathPresetUnknown30OffsetCheck[
    ((u32)&((BtlCameraPathPreset *)0)->unk30 == 0x30) ? 1 : -1];
typedef char BtlCameraPathPresetUnknown40OffsetCheck[
    ((u32)&((BtlCameraPathPreset *)0)->unk40 == 0x40) ? 1 : -1];
typedef char BtlCameraPathPresetBlendRateOffsetCheck[
    ((u32)&((BtlCameraPathPreset *)0)->blendRate == 0x50) ? 1 : -1];

#endif /* BTL_CAMERA_PATH_PRESET_H */
