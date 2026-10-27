#ifndef SDF_DEV_SLOT_API_H
#define SDF_DEV_SLOT_API_H

#include "common.h"

struct sdfModelRootNode_t;

/* Independent root-storage and material-reference lifetime choices. */
typedef s32 SdfModelRootCleanupPolicy;
typedef s32 SdfModelMaterialResourceReleasePolicy;

void sdfModelRootFree(struct sdfModelRootNode_t *slot,
                      SdfModelRootCleanupPolicy rootCleanupPolicy,
                      SdfModelMaterialResourceReleasePolicy materialResourcePolicy);

#endif /* SDF_DEV_SLOT_API_H */
