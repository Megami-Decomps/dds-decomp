#ifndef SDF_MODEL_SCALARS_H
#define SDF_MODEL_SCALARS_H

#include "common.h"

struct SdfModel;

void sdfModelSetFaceDivParm(struct SdfModel *model, f32 first, f32 second);
void sdfModelResetFaceDivParm(struct SdfModel *model);
f32 sdfModelGetFaceDivSoften(struct SdfModel *model);
f32 sdfModelGetFaceDivBias(struct SdfModel *model);

#endif /* SDF_MODEL_SCALARS_H */
