#ifndef SDF_MODEL_RELEASE_API_H
#define SDF_MODEL_RELEASE_API_H

#include "sdf_dev_slot_api.h"

/* Nonzero root cleanup queues sdfModelRootFreeResrc; nonzero material release
 * queues full release of the model's material-resource list. */
#define SDF_MODEL_ROOT_FREE_INLINE 0
#define SDF_MODEL_ROOT_FREE_DEFERRED 1
#define SDF_MODEL_MATERIAL_RESOURCES_RETAINED 0
#define SDF_MODEL_MATERIAL_RESOURCES_RELEASE_DEFERRED 1

void sdfModelRootFree(struct sdfModelRootNode_t *slot, s32 deferRootCleanup,
                      s32 deferMaterialResourceRelease);

#endif /* SDF_MODEL_RELEASE_API_H */
