#ifndef SDF_RESOURCE_H
#define SDF_RESOURCE_H

/* General-heap descriptors are distinct from the data addresses they own. */
struct SdfMemBlock;

void sdfReleaseResourceAllocation(struct SdfMemBlock *allocation);

#endif /* SDF_RESOURCE_H */
