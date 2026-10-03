#include "common.h"
#include "sdf.h"

#define SDF_CHIP_BLOCK_SHIFT 12
#define SDF_CHIP_BLOCK_BYTES (1 << SDF_CHIP_BLOCK_SHIFT)
#define SDF_MEM_ALIGNMENT_MASK 0x7F
#define SDF_MEM_BLOCK_FREE 0
#define SDF_MEM_BLOCK_SENTINEL 2

extern u32 D_0045F0FC[];

extern void *func_0035A828(u32 size);
extern void sdfInitializeSynchronizedRequest(void *request, u32 callback);
extern void sdfReleaseChipBlock(void *memory);

void func_00328FA8(u32 heapSize) {
    u32 recordBytes;
    s32 alignment;
    s32 remaining;
    void *allocation;
    SdfChipPage *record;
    SdfChipPage *next;
    u8 *recordEnd;
    u8 *heapStart;
    u8 *page;
    s32 pageCount;
    SdfChipClass *sizeClass;
    s32 unitSize;
    s32 cellCount;

    allocation = func_0035A828(heapSize);
    sdfChipPages = allocation;
    recordBytes = (heapSize / 0x1018U) * sizeof(SdfChipPage);
    recordEnd = (u8 *)allocation + recordBytes;
    record = allocation;
    sdfFreeChipPages = record;
    alignment = -(s32)recordEnd & 0x3F;
    remaining = (heapSize - recordBytes) - alignment;
    heapStart = recordEnd + alignment;
    sdfChipHeapStart = heapStart;
    pageCount = remaining / SDF_CHIP_BLOCK_BYTES;
    sdfChipPageCount[0] = pageCount;
    sdfChipHeapEnd = heapStart + pageCount * SDF_CHIP_BLOCK_BYTES;
    page = heapStart;
    do {
        next = record + 1;
        pageCount--;
        record->base = page;
        page += SDF_CHIP_BLOCK_BYTES;
        record->sizeClass = NULL;
        record->next = next;
        record = next;
    } while (pageCount != 0);
    next[-1].next = NULL;

    sizeClass = sdfChipClassTable.classes;
    unitSize = 0x10;
    cellCount = 0x100;
    pageCount = SDF_CHIP_CLASS_COUNT;
    do {
        pageCount--;
        sizeClass->cellSize = unitSize;
        sizeClass->cellCount = cellCount;
        cellCount >>= 1;
        sizeClass->currentPage = NULL;
        unitSize <<= 1;
        sizeClass->availablePages = NULL;
        sizeClass++;
    } while (pageCount != 0);
    sdfInitializeSynchronizedRequest(&sdfChipReleaseRequest, (u32)sdfReleaseChipBlock);
}

typedef struct SdfChipStats {
    u32 totalBytes; /* 0x00 */
    u32 freeBytes; /* 0x04 */
    u32 blockCount; /* 0x08 */
    u32 emptyBlocks; /* 0x0C: blocks without a size class */
    u32 partialBlocks; /* 0x10: blocks with free cells */
    u32 usedCells[SDF_CHIP_CLASS_COUNT]; /* 0x14: used cells per size class */
} SdfChipStats;

/* Fill `stats` with the chip heap's block totals and per-size-class usage. */
void sdfGetChipHeapStats(SdfChipStats *stats) {
    SdfChipPage *block;
    s32 blocksRemaining;
    s32 classIndex;
    s32 partialBlocks;
    s32 emptyBlocks;
    s32 freeBytes;
    s32 freeCells;

    blocksRemaining = sdfChipPageCount[0];
    stats->blockCount = blocksRemaining;
    stats->totalBytes = blocksRemaining << SDF_CHIP_BLOCK_SHIFT;
    for (classIndex = 0; classIndex != SDF_CHIP_CLASS_COUNT; classIndex++) {
        stats->usedCells[classIndex] = 0;
    }
    block = sdfChipPages;
    emptyBlocks = 0;
    partialBlocks = 0;
    freeBytes = 0;
    /* This post-tested walk assumes a nonzero heap block count. */
    do {
        if (block->sizeClass == NULL) {
            emptyBlocks++;
            freeBytes += SDF_CHIP_BLOCK_BYTES;
        } else {
            freeCells = block->sizeClass->cellCount - block->usedCells;
            if (freeCells != 0) {
                partialBlocks++;
                freeBytes += freeCells * block->sizeClass->cellSize;
            }
            stats->usedCells[block->sizeClass - sdfChipClassTable.classes] += block->usedCells;
        }
        block++;
    } while (--blocksRemaining != 0);
    stats->freeBytes = freeBytes;
    stats->emptyBlocks = emptyBlocks;
    stats->partialBlocks = partialBlocks;
}

extern void *func_0035A828(u32 size);
extern void *sdfAllocSizeClassBlock(u32 size);
extern s32 D_004389CC;
extern u8 D_00439128[4];
extern void sdfReleaseResourceAllocation();
extern void sdfInitializeSynchronizedRequest();

/* Align the usable span to 128 bytes and link one free block between sentinels. */
void sdfInitGeneralHeap(u32 heapSize) {
    SdfMemHeap *heap = &sdfGeneralHeap;
    SdfMemBlock *freeBlock;
    u32 alignedStart;
    u32 alignedEnd;

    heap->base = (u32)func_0035A828(heapSize);
    heap->size = heapSize;
    freeBlock = sdfAllocSizeClassBlock(0x10);
    alignedStart = (heap->base + SDF_MEM_ALIGNMENT_MASK) & ~SDF_MEM_ALIGNMENT_MASK;
    alignedEnd = (heap->base + heapSize) & ~SDF_MEM_ALIGNMENT_MASK;
    heap->head.prev = NULL;
    heap->head.next = freeBlock;
    heap->head.state = SDF_MEM_BLOCK_SENTINEL;
    heap->head.referenceCount = -1;
    heap->tail.state = SDF_MEM_BLOCK_SENTINEL;
    heap->tail.referenceCount = -1;
    heap->tail.address = alignedEnd;
    heap->tail.prev = freeBlock;
    heap->tail.next = NULL;
    heap->head.address = alignedStart;
    freeBlock->prev = &heap->head;
    freeBlock->state = SDF_MEM_BLOCK_FREE;
    freeBlock->next = &heap->tail;
    freeBlock->address = alignedStart;
    freeBlock->referenceCount = 0;
    D_004389CC = 0;
    sdfInitializeSynchronizedRequest(D_00439128, (u32)sdfReleaseResourceAllocation);
}

u16 sdfGetMemoryBlockState(SdfMemBlock *block) {
    return block->state;
}

u32 func_00329230(void) {
    return (u32)sdfGeneralHeap.head.next;
}

INCLUDE_SDATA(const s32, "game/code_00328FA8", D_004389CC);

INCLUDE_SDATA(const s32, "game/code_00328FA8", D_004389D0);

INCLUDE_SDATA(const s32, "game/code_00328FA8", D_004389D1);
