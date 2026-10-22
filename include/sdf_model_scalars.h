#ifndef SDF_MODEL_SCALARS_H
#define SDF_MODEL_SCALARS_H

#include "common.h"

struct sdfModelRootNode_t;

void sdfModelSetFaceDivParm(struct sdfModelRootNode_t *model, f32 first, f32 second);
void sdfModelResetFaceDivParm(struct sdfModelRootNode_t *model);
f32 sdfModelGetFaceDivSoften(struct sdfModelRootNode_t *model);
f32 sdfModelGetFaceDivBias(struct sdfModelRootNode_t *model);

#endif /* SDF_MODEL_SCALARS_H */
