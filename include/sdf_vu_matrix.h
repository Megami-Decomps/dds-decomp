#ifndef SDF_VU_MATRIX_H
#define SDF_VU_MATRIX_H

#include "common.h"

/* Apply the primary VU0 matrix bank vf28-vf31 to basis vectors vf24-vf27,
 * then replace vf28-vf31 with the four transformed vectors. Provider bodies:
 * DDS1 game/code_002DD8B8.c and DDS2 game/code_00336768.c. */
void sdfComposeVuMatrixFromRegisters(void);

/* Build the camera basis from the global view target, eye, and up vectors.
 * The resulting basis rows replace VU0 registers vf28-vf31 in place. */
void vuBuildLookAtBasis(void);

#endif /* SDF_VU_MATRIX_H */
