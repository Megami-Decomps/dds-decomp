#ifndef SDF_QUATERNION_H
#define SDF_QUATERNION_H

#include "common.h"

/* Normalize the supplied axis and write an XYZW quaternion. The native
 * conversion uses nonnegative square roots for both half-angle components. */
void sdfQuatFromAxisAngle(f32 *out, f32 x, f32 y, f32 z, f32 angle);

/* Write three axis components and twice acos(W), using the native small
 * denominator fallback when sqrt(1-W*W) is near zero. */
void sdfQuatToAxisAngle(f32 *axis, f32 *angle, const f32 *quaternion);

#endif /* SDF_QUATERNION_H */
