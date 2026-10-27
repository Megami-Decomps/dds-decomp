#ifndef SDF_MODEL_RELEASE_API_H
#define SDF_MODEL_RELEASE_API_H

#include "sdf_dev_slot_api.h"

/* Root cleanup and material-reference release are independent choices.
 * Deferred root cleanup queues sdfModelRootFreeResrc; deferred material release
 * queues full release of the model's material-resource list. */
#define SDF_MODEL_ROOT_FREE_INLINE 0
#define SDF_MODEL_ROOT_FREE_DEFERRED 1
#define SDF_MODEL_MATERIAL_RESOURCES_RETAINED 0
#define SDF_MODEL_MATERIAL_RESOURCES_RELEASE_DEFERRED 1

#endif /* SDF_MODEL_RELEASE_API_H */
