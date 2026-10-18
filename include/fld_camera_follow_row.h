#ifndef FLD_CAMERA_FOLLOW_ROW_H
#define FLD_CAMERA_FOLLOW_ROW_H

#include "common.h"

/* One 0x18-byte FLDALL.TBL camera row: distance, eye height, target height
 * and field of view, then how the camera follows. Both title loaders read a
 * 0xC00-byte table span into this array. */
typedef struct FldCamRow {
    f32 dist;          /* 0x00 */
    f32 y;             /* 0x04 */
    f32 targetY;       /* 0x08 */
    f32 fov;           /* 0x0C */
    s32 flags;         /* 0x10: FLD_CAM_ROW_* */
    s32 namedCamera;   /* 0x14: FldNamedCamera row for FLD_CAM_ROW_NAMED */
} FldCamRow;

enum {
    FLD_CAM_ROW_LIMITED = 1, /* the follow distance stays within dist and targetY */
    FLD_CAM_ROW_NAMED = 2,   /* the camera slides between a named node pair */
};

typedef char FldCamRowSizeCheck[(sizeof(FldCamRow) == 0x18) ? 1 : -1];
typedef char FldCamRowDistanceOffsetCheck[
    ((u32)&((FldCamRow *)0)->dist == 0x00) ? 1 : -1];
typedef char FldCamRowEyeHeightOffsetCheck[
    ((u32)&((FldCamRow *)0)->y == 0x04) ? 1 : -1];
typedef char FldCamRowTargetHeightOffsetCheck[
    ((u32)&((FldCamRow *)0)->targetY == 0x08) ? 1 : -1];
typedef char FldCamRowFovOffsetCheck[
    ((u32)&((FldCamRow *)0)->fov == 0x0C) ? 1 : -1];
typedef char FldCamRowFlagsOffsetCheck[
    ((u32)&((FldCamRow *)0)->flags == 0x10) ? 1 : -1];
typedef char FldCamRowNamedCameraOffsetCheck[
    ((u32)&((FldCamRow *)0)->namedCamera == 0x14) ? 1 : -1];

/* A FLD named camera (0x28): the camera sits at the first node when the
 * player's coordinate on axis (0 x, 1 y, 2 z) is at minimum, at the second
 * at maximum, and in proportion between. */
typedef struct FldNamedCamera {
    char firstNode[16];  /* 0x00 */
    char secondNode[12]; /* 0x10 */
    s32 axis;            /* 0x1C */
    f32 minimum;         /* 0x20 */
    f32 maximum;         /* 0x24 */
} FldNamedCamera;

typedef char FldNamedCameraSizeCheck[(sizeof(FldNamedCamera) == 0x28) ? 1 : -1];

#endif /* FLD_CAMERA_FOLLOW_ROW_H */
