#ifndef SDF_VU_TRANSFORM_API_H
#define SDF_VU_TRANSFORM_API_H

/* Rigid-invert the VU matrix in vf28-vf31 in place; translation becomes
 * -(R^T * t). The rotation input is assumed to contain no scale. */
void sdfInvertRigidVuTransform(void);

#endif /* SDF_VU_TRANSFORM_API_H */
