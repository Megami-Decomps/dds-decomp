#ifndef DDS3_VU_TRANSFORM_H
#define DDS3_VU_TRANSFORM_H

#include "common.h"

/* Build the VU transform from borrowed four-float scale, rotation and
 * translation vectors. The result stays in the VU output registers. */
void dds3BuildVuTransformFromComponents(const f32 scale[4],
                                       const f32 rotation[4],
                                       const f32 translation[4]);

#endif /* DDS3_VU_TRANSFORM_H */
