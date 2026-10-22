#ifndef SDF_RESOURCE_H
#define SDF_RESOURCE_H

#include "common.h"

/* General-heap descriptors are distinct from the data addresses they own. */
struct SdfMemBlock;

/* Six signed words written by sdfMemGetInfo. */
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

struct SdfMemBlock *sdfMemAllocDirect(s32 requestedBytes);
struct SdfMemBlock *sdfMemAllocFromTailDirect(s32 requestedBytes);
struct SdfMemBlock *sdfMemAlloc2Direct(s32 requestedBytes);
struct SdfMemBlock *sdfMemGetHandleByPtr(void *address);
void sdfMemGetInfo(SdfGeneralHeapStats *stats);
void sdfChipGetInfo(SdfChipHeapStats *stats);

/* Return the descriptor's payload base as a byte address, not the descriptor. */
u8 *sdfMemGetPtr(const struct SdfMemBlock *block);
s32 sdfMemGetSize(const struct SdfMemBlock *block);
/* Advance one descriptor; return NULL when its successor is the end marker. */
struct SdfMemBlock *sdfMemGetNext(struct SdfMemBlock *block);

/* Route payload addresses between the chip and general heaps. */
void *sdfAllocAuto(s32 size);
void sdfFreeAuto(void *data);
void sdfFreeAutoDelayed(void *data);

u32 sdfMemLock(struct SdfMemBlock *allocation);
void sdfMemUnlock(struct SdfMemBlock *allocation);
void sdfMemFreeDirect(struct SdfMemBlock *allocation);
void sdfMemFreeHandleDirect(s32 *slot);

/* The optional outputs contain a data address and file size, not a descriptor.
 * A null outAddress releases the allocation before returning its descriptor. */
struct SdfMemBlock *sdfDevLoadFile(
    const char *name, u32 *outAddress, u32 *outSize, s32 extraBytes);
struct SdfMemBlock *sdfDevLoadFileEx(
    const char *name, u32 *outAddress, u32 *outSize);

/* Queue the two heap owners separately: the general heap stores descriptors,
 * while the chip heap stores the address of the cell to release. */
void sdfMemFreeDelayedDirect(struct SdfMemBlock *allocation);
void sdfChipFreeDelayedDirect(void *memory);

#endif /* SDF_RESOURCE_H */
