#ifndef SDF_RESOURCE_H
#define SDF_RESOURCE_H

#include "common.h"

/* General-heap descriptors are distinct from the data addresses they own. */
struct SdfMemBlock;

/* Six signed words written by sdfGetGeneralHeapStats. */
typedef struct SdfGeneralHeapStats {
    s32 totalBytes;
    s32 freeBytes;
    s32 largestFreeBytes;
    s32 smallestFreeBytes;
    s32 blockCount;
    s32 freeBlockCount;
} SdfGeneralHeapStats;

/* Five totals followed by the seven native chip size-class counters. */
typedef struct SdfChipHeapStats {
    u32 totalBytes;
    u32 freeBytes;
    u32 blockCount;
    u32 emptyBlockCount;
    u32 partialBlockCount;
    u32 usedCells[7];
} SdfChipHeapStats;

typedef char SdfGeneralHeapStats_size_must_be_0x18[
    (sizeof(SdfGeneralHeapStats) == 0x18) ? 1 : -1];
typedef char SdfChipHeapStats_size_must_be_0x30[
    (sizeof(SdfChipHeapStats) == 0x30) ? 1 : -1];

struct SdfMemBlock *sdfAllocGeneralBlock(s32 requestedBytes);
struct SdfMemBlock *sdfAllocGeneralBlockHigh(s32 requestedBytes);
struct SdfMemBlock *sdfTryAllocGeneralBlock(s32 requestedBytes);
struct SdfMemBlock *sdfFindGeneralBlockByAddress(void *address);
void sdfGetGeneralHeapStats(SdfGeneralHeapStats *stats);
void sdfGetChipHeapStats(SdfChipHeapStats *stats);

u32 sdfMemoryGetBlockAddress(struct SdfMemBlock *block);
s32 sdfMemoryGetBlockSize(struct SdfMemBlock *block);

u32 sdfResourceRetainAddress(struct SdfMemBlock *allocation);
void sdfDecrementAllocationReferenceCount(struct SdfMemBlock *allocation);
void sdfReleaseResourceAllocation(struct SdfMemBlock *allocation);
void sdfReleaseMemorySlot(s32 *slot);

/* The optional outputs contain a data address and file size, not a descriptor.
 * A null outAddress releases the allocation before returning its descriptor. */
struct SdfMemBlock *sdfDevReadResourceWithExtraSpace(
    const char *name, u32 *outAddress, u32 *outSize, s32 extraBytes);
struct SdfMemBlock *sdfReadNamedResource(
    const char *name, u32 *outAddress, u32 *outSize);

/* Queue the two heap owners separately: the general heap stores descriptors,
 * while the chip heap stores the address of the cell to release. */
void sdfQueueGeneralAllocationRelease(struct SdfMemBlock *allocation);
void sdfQueuePendingChipRelease(void *memory);

#endif /* SDF_RESOURCE_H */
