#ifndef BTL_VEC3_H
#define BTL_VEC3_H

#include "common.h"

typedef struct BtlVec3 {
    f32 x;
    f32 y;
    f32 z;
} BtlVec3;

typedef char BtlVec3_size[(sizeof(BtlVec3) == 0x0C) ? 1 : -1];
typedef char BtlVec3_x_offset[
    ((u32)&((BtlVec3 *)0)->x == 0x00) ? 1 : -1];
typedef char BtlVec3_y_offset[
    ((u32)&((BtlVec3 *)0)->y == 0x04) ? 1 : -1];
typedef char BtlVec3_z_offset[
    ((u32)&((BtlVec3 *)0)->z == 0x08) ? 1 : -1];

#endif
