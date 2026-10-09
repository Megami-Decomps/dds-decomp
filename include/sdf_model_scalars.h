#ifndef SDF_MODEL_SCALARS_H
#define SDF_MODEL_SCALARS_H

#include "common.h"

struct SdfModel;

void sdfSetModelScalarOverrides(struct SdfModel *model, f32 first, f32 second);
void sdfClearModelScalarOverrides(struct SdfModel *model);
f32 sdfGetFirstTextOverrideOrDefault(struct SdfModel *model);
f32 sdfGetSecondTextOverrideOrDefault(struct SdfModel *model);

#endif /* SDF_MODEL_SCALARS_H */
