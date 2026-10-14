#ifndef SDF_QUATERNION_H
#define SDF_QUATERNION_H

#include "common.h"

/* RenderWare XYZ axis used by the native SDF matrix builders. */
typedef struct RwV3d
{
    f32 x;
    f32 y;
    f32 z;
} RwV3d;

typedef char RwV3dSizeAssert[(sizeof(RwV3d) == 0xC) ? 1 : -1];

/* Load axis-angle rotation into VU0 vf28-vf31. The angle-first interface
 * follows the native Flash caller at DDS2 00171FF8..00172000; EE's separate
 * f12/a0 argument registers alone do not distinguish the two source orders. */
void sdfBuildVuRotationFromAxisAngle(f32 angle, const RwV3d *axis);

/* Normalize the supplied axis and write an XYZW quaternion. The native
 * conversion uses nonnegative square roots for both half-angle components. */
void sdfQuatFromAxisAngle(f32 *out, f32 x, f32 y, f32 z, f32 angle);

/* Write three axis components and twice acos(W), using the native small
 * denominator fallback when sqrt(1-W*W) is near zero. */
void sdfQuatToAxisAngle(f32 *axis, f32 *angle, const f32 *quaternion);

void sdfQuatMultiply(f32 *output, f32 *left, f32 *right);

#endif /* SDF_QUATERNION_H */
