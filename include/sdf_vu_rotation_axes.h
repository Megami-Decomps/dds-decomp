#ifndef SDF_VU_ROTATION_AXES_H
#define SDF_VU_ROTATION_AXES_H

#include "common.h"

/* Load the X-axis rotation matrix into the primary VU0 matrix registers. */
void sdfVuLoadRotationMatrixX(f32 angle);

/* Load the secondary Y-axis rotation matrix into the primary VU0 matrix registers. */
void sdfVuLoadSecondaryRotationMatrixY(f32 angle);

#endif /* SDF_VU_ROTATION_AXES_H */
