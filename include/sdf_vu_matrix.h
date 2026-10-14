#ifndef SDF_VU_MATRIX_H
#define SDF_VU_MATRIX_H

#include "common.h"

/* Apply the primary VU0 matrix bank vf28-vf31 to basis vectors vf24-vf27,
 * then replace vf28-vf31 with the four transformed vectors. Provider bodies:
 * DDS1 game/code_002DD8B8.c and DDS2 game/code_00336768.c. */
void sdfComposeVuMatrixFromRegisters(void);

/* Load the Z-axis rotation matrix rows into the primary VU0 matrix registers. */
void sdfVuLoadRotationMatrixZ(f32 angle);

/* Convert three Euler angle components to the VU0 quaternion output.
 * Providers: DDS1 game/code_002E7C20.c and DDS2 game/code_00340AC8.c. */
void sdfConvertEulerAnglesToQuaternionVU(f32 x, f32 y, f32 z);

/* Load matrix into the VU0 scratch bank and postmultiply the primary bank. */
void sdfPostmultiplyVuMatrixFromMemory(void *matrix);

/* Convert the quaternion in vf10 into the primary matrix bank vf28-vf31.
 * Providers: DDS1 game/code_002E7C20.c and DDS2 game/code_00340AC8.c. */
void effMiscQuaternionToMatrixVU(void);

/* Build the camera basis from the global view target, eye, and up vectors.
 * The resulting basis rows replace VU0 registers vf28-vf31 in place. */
void vuBuildLookAtBasis(void);

#endif /* SDF_VU_MATRIX_H */
