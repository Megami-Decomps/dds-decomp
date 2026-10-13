#ifndef SDF_VECTOR_TRANSFORM_H
#define SDF_VECTOR_TRANSFORM_H

#include "common.h"

/* Transform XYZ without translation and return the updated direction vector. */
f32 *sdfTransformDirectionByMatrix(f32 *direction, f32 *matrix);

#endif /* SDF_VECTOR_TRANSFORM_H */
