#ifndef BTL_CAMERA_SNAPSHOTS_H
#define BTL_CAMERA_SNAPSHOTS_H

#include "btl_command.h"

typedef struct BtlCameraStoredPose {
    BtlCamState pose;
    u8 pad28[8];
} BtlCameraStoredPose;

typedef struct BtlCameraParameterSnapshot {
    f32 pathEnd[4];
    f32 pathStart[4];
    f32 targetFrom[4];
    f32 targetTo[4];
    f32 cameraFrom[4];
    f32 cameraTo[4];
    f32 direction[4];
    f32 distance;
    u8 pad74[0xC];
    f32 angleVector[4];
    f32 startFrame;
    u8 pad94[0xC];
    f32 duration;
    u8 padA4[0xC];
    u8 parameterD0;
    u8 padB1[0xF];
    f32 parameterE0;
    u8 padC4[0xC];
    f32 blendRate;
    u8 padD4[0xC];
    f32 rangeStart;
    u8 padE4[0xC];
    f32 rangeEnd;
    u8 padF4[0xC];
} BtlCameraParameterSnapshot;

typedef char BtlCameraParameterSnapshot_size_check[
    (sizeof(BtlCameraParameterSnapshot) == 0x100) ? 1 : -1];
typedef char BtlCameraStoredPose_size_check[
    (sizeof(BtlCameraStoredPose) == 0x30) ? 1 : -1];
typedef char BtlCameraStoredPose_pose_offset_check[
    ((u32)&((BtlCameraStoredPose *)0)->pose == 0) ? 1 : -1];
typedef char BtlCameraParameterSnapshot_distance_offset_check[
    ((u32)&((BtlCameraParameterSnapshot *)0)->distance == 0x70) ? 1 : -1];
typedef char BtlCameraParameterSnapshot_angleVector_offset_check[
    ((u32)&((BtlCameraParameterSnapshot *)0)->angleVector == 0x80) ? 1 : -1];
typedef char BtlCameraParameterSnapshot_parameterD0_offset_check[
    ((u32)&((BtlCameraParameterSnapshot *)0)->parameterD0 == 0xB0) ? 1 : -1];
typedef char BtlCameraParameterSnapshot_parameterE0_offset_check[
    ((u32)&((BtlCameraParameterSnapshot *)0)->parameterE0 == 0xC0) ? 1 : -1];
typedef char BtlCameraParameterSnapshot_blendRate_offset_check[
    ((u32)&((BtlCameraParameterSnapshot *)0)->blendRate == 0xD0) ? 1 : -1];
typedef char BtlCameraParameterSnapshot_rangeStart_offset_check[
    ((u32)&((BtlCameraParameterSnapshot *)0)->rangeStart == 0xE0) ? 1 : -1];
typedef char BtlCameraParameterSnapshot_rangeEnd_offset_check[
    ((u32)&((BtlCameraParameterSnapshot *)0)->rangeEnd == 0xF0) ? 1 : -1];

#endif /* BTL_CAMERA_SNAPSHOTS_H */
