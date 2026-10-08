#ifndef SDF_RESOURCE_H
#define SDF_RESOURCE_H

#include "common.h"

/* General-heap descriptors are distinct from the data addresses they own. */
struct SdfMemBlock;

enum {
    SDF_HEAP_STAT_TOTAL_BYTES = 0,
    SDF_HEAP_STAT_FREE_BYTES = 1,
    SDF_HEAP_STAT_LARGEST_FREE = 2,
    SDF_HEAP_STAT_SMALLEST_FREE = 3,
    SDF_HEAP_STAT_BLOCK_COUNT = 4,
    SDF_HEAP_STAT_FREE_BLOCK_COUNT = 5
};

struct SdfMemBlock *sdfAllocGeneralBlock(s32 requestedBytes);
struct SdfMemBlock *sdfAllocGeneralBlockHigh(s32 requestedBytes);
struct SdfMemBlock *sdfTryAllocGeneralBlock(s32 requestedBytes);
struct SdfMemBlock *sdfFindGeneralBlockByAddress(void *address);
void sdfGetGeneralHeapStats(s32 *stats);

u32 sdfMemoryGetBlockAddress(struct SdfMemBlock *block);
s32 sdfMemoryGetBlockSize(struct SdfMemBlock *block);

u32 sdfResourceRetainAddress(struct SdfMemBlock *allocation);
void sdfDecrementAllocationReferenceCount(struct SdfMemBlock *allocation);
void sdfReleaseResourceAllocation(struct SdfMemBlock *allocation);

/* Queue the two heap owners separately: the general heap stores descriptors,
 * while the chip heap stores the address of the cell to release. */
void sdfQueueGeneralAllocationRelease(struct SdfMemBlock *allocation);
void sdfQueuePendingChipRelease(void *memory);

#endif /* SDF_RESOURCE_H */
