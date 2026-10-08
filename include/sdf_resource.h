#ifndef SDF_RESOURCE_H
#define SDF_RESOURCE_H

#include "common.h"

/* General-heap descriptors are distinct from the data addresses they own. */
struct SdfMemBlock;

u32 sdfResourceRetainAddress(struct SdfMemBlock *allocation);
void sdfReleaseResourceAllocation(struct SdfMemBlock *allocation);

#endif /* SDF_RESOURCE_H */
