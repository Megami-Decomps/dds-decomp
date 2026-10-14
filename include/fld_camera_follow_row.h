#ifndef FLD_CAMERA_FOLLOW_ROW_H
#define FLD_CAMERA_FOLLOW_ROW_H

#include "common.h"

/* One 0x18-byte FLDALL.TBL camera row. The projection/follow clients consume
 * the distance, eye height, target height and field of view; the tail stays
 * opaque. Both title loaders read a 0xC00-byte table span into this array. */
typedef struct FldCamRow {
    f32 dist;          /* 0x00 */
    f32 y;             /* 0x04 */
    f32 targetY;       /* 0x08 */
    f32 fov;           /* 0x0C */
    u8 opaque10[8];    /* 0x10 */
} FldCamRow;

typedef char FldCamRowSizeCheck[(sizeof(FldCamRow) == 0x18) ? 1 : -1];
typedef char FldCamRowDistanceOffsetCheck[
    ((u32)&((FldCamRow *)0)->dist == 0x00) ? 1 : -1];
typedef char FldCamRowEyeHeightOffsetCheck[
    ((u32)&((FldCamRow *)0)->y == 0x04) ? 1 : -1];
typedef char FldCamRowTargetHeightOffsetCheck[
    ((u32)&((FldCamRow *)0)->targetY == 0x08) ? 1 : -1];
typedef char FldCamRowFovOffsetCheck[
    ((u32)&((FldCamRow *)0)->fov == 0x0C) ? 1 : -1];
typedef char FldCamRowOpaqueTailOffsetCheck[
    ((u32)&((FldCamRow *)0)->opaque10 == 0x10) ? 1 : -1];

#endif /* FLD_CAMERA_FOLLOW_ROW_H */
