#ifndef SDF_MATH_VECTOR4_H
#define SDF_MATH_VECTOR4_H

#include "common.h"

/* Four-float scratch/control-point vector used by the paired geometry helpers. */
typedef struct SdfMathVector4 {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} SdfMathVector4;

typedef char SdfMathVector4_size_must_be_0x10[
    (sizeof(SdfMathVector4) == 0x10) ? 1 : -1];
typedef char SdfMathVector4_y_offset_must_be_0x04[
    ((u32)&((SdfMathVector4 *)0)->y == 0x04) ? 1 : -1];
typedef char SdfMathVector4_z_offset_must_be_0x08[
    ((u32)&((SdfMathVector4 *)0)->z == 0x08) ? 1 : -1];
typedef char SdfMathVector4_w_offset_must_be_0x0C[
    ((u32)&((SdfMathVector4 *)0)->w == 0x0C) ? 1 : -1];

#endif /* SDF_MATH_VECTOR4_H */
