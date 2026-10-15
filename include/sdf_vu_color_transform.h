#ifndef SDF_VU_COLOR_TRANSFORM_H
#define SDF_VU_COLOR_TRANSFORM_H

#include "common.h"

/* VU0 input basis and emitted base/delta color vectors. */
typedef struct SdfVuColorTransform {
    u8 opaque00[4]; /* 0x00 */
    f32 scaledY; /* 0x04 */
    u8 opaque08[4]; /* 0x08 */
    f32 scale; /* 0x0C */
    f32 colorBase[4]; /* 0x10 */
    f32 colorDelta[4]; /* 0x20 */
} SdfVuColorTransform;

void sdfVuBuildPackedColorTransform(SdfVuColorTransform *result, u32 reference,
                                    u32 packedColor, u32 firstColor,
                                    u32 secondColor, f32 blend, f32 scale,
                                    f32 y);

typedef char SdfVuColorTransform_size_must_be_0x30[
    (sizeof(SdfVuColorTransform) == 0x30) ? 1 : -1];
typedef char SdfVuColorTransform_opaque00_offset_must_be_0[
    ((u32)&((SdfVuColorTransform *)0)->opaque00 == 0x00) ? 1 : -1];
typedef char SdfVuColorTransform_scaledY_offset_must_be_0x04[
    ((u32)&((SdfVuColorTransform *)0)->scaledY == 0x04) ? 1 : -1];
typedef char SdfVuColorTransform_opaque08_offset_must_be_0x08[
    ((u32)&((SdfVuColorTransform *)0)->opaque08 == 0x08) ? 1 : -1];
typedef char SdfVuColorTransform_scale_offset_must_be_0x0C[
    ((u32)&((SdfVuColorTransform *)0)->scale == 0x0C) ? 1 : -1];
typedef char SdfVuColorTransform_colorBase_offset_must_be_0x10[
    ((u32)&((SdfVuColorTransform *)0)->colorBase == 0x10) ? 1 : -1];
typedef char SdfVuColorTransform_colorDelta_offset_must_be_0x20[
    ((u32)&((SdfVuColorTransform *)0)->colorDelta == 0x20) ? 1 : -1];

#endif /* SDF_VU_COLOR_TRANSFORM_H */
