#ifndef ITF_TEXT_VECTOR_H
#define ITF_TEXT_VECTOR_H

#include "common.h"

/* Four signed fixed-point components used by interface quad colors. */
typedef struct TextVector {
    s32 x;
    s32 y;
    s32 z;
    s32 w;
} TextVector;

typedef char TextVector_size_must_be_0x10[
    sizeof(TextVector) == 0x10 ? 1 : -1];
typedef char TextVector_x_offset_must_be_0x00[
    ((u32)&((TextVector *)0)->x) == 0x00 ? 1 : -1];
typedef char TextVector_y_offset_must_be_0x04[
    ((u32)&((TextVector *)0)->y) == 0x04 ? 1 : -1];
typedef char TextVector_z_offset_must_be_0x08[
    ((u32)&((TextVector *)0)->z) == 0x08 ? 1 : -1];
typedef char TextVector_w_offset_must_be_0x0C[
    ((u32)&((TextVector *)0)->w) == 0x0C ? 1 : -1];

#endif
