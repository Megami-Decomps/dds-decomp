#ifndef SDF_RESOURCE_H
#define SDF_RESOURCE_H

#include "common.h"

/* General-heap descriptors are distinct from the data addresses they own. */
struct SdfMemBlock;

struct SdfMemBlock *sdfAllocGeneralBlock(s32 requestedBytes);
struct SdfMemBlock *sdfAllocGeneralBlockHigh(s32 requestedBytes);
struct SdfMemBlock *sdfTryAllocGeneralBlock(s32 requestedBytes);

u32 sdfMemoryGetBlockAddress(struct SdfMemBlock *block);
s32 sdfMemoryGetBlockSize(struct SdfMemBlock *block);

u32 sdfResourceRetainAddress(struct SdfMemBlock *allocation);
void sdfDecrementAllocationReferenceCount(struct SdfMemBlock *allocation);
void sdfReleaseResourceAllocation(struct SdfMemBlock *allocation);

#endif /* SDF_RESOURCE_H */
