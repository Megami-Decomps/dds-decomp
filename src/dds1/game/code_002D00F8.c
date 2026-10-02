#include "common.h"

#define SDF_CHIP_CLASS_COUNT 7
#define SDF_CHIP_BLOCK_SHIFT 12
#define SDF_CHIP_BLOCK_BYTES (1 << SDF_CHIP_BLOCK_SHIFT)
#define SDF_MEM_ALIGNMENT_MASK 0x7F
#define SDF_MEM_BLOCK_FREE 0
#define SDF_MEM_BLOCK_SENTINEL 2

extern u32 D_003E274C[];

INCLUDE_ASM(const s32, "game/code_002D00F8", func_002D00F8);

/* Size class of the chip heap: cells per block and bytes per cell. */
typedef struct SdfChipClass {
    u32 unk0; /* 0x0 */
    u32 unk4; /* 0x4 */
    s16 unitSize; /* 0x8: bytes per cell */
    s16 cellCount; /* 0xA: cells per block */
} SdfChipClass; /* 0xC */

/* One 0x18-byte heap block record. */
typedef struct SdfChipBlockRecord {
    u8 pad00[0xC];
    SdfChipClass *sizeClass; /* 0xC: NULL when the block is unassigned */
    u8 pad10[4];
    s16 usedCells; /* 0x14 */
    u8 pad16[2];
} SdfChipBlockRecord; /* 0x18 */

typedef struct SdfChipStats {
    u32 totalBytes; /* 0x00 */
    u32 freeBytes; /* 0x04 */
    u32 blockCount; /* 0x08 */
    u32 emptyBlocks; /* 0x0C: blocks without a size class */
    u32 partialBlocks; /* 0x10: blocks with free cells */
    u32 usedCells[SDF_CHIP_CLASS_COUNT]; /* 0x14: used cells per size class */
} SdfChipStats;

extern SdfChipBlockRecord *D_003BD9A8;
extern s32 D_003BD9B8;
extern SdfChipClass D_003E26F0[];

/* Fill `stats` with the chip heap's block totals and per-size-class usage. */
void sdfGetChipHeapStats(SdfChipStats *stats) {
    SdfChipBlockRecord *block;
    s32 blocksRemaining;
    s32 classIndex;
    s32 partialBlocks;
    s32 emptyBlocks;
    s32 freeBytes;
    s32 freeCells;

    blocksRemaining = D_003BD9B8;
    stats->blockCount = blocksRemaining;
    stats->totalBytes = blocksRemaining << SDF_CHIP_BLOCK_SHIFT;
    for (classIndex = 0; classIndex != SDF_CHIP_CLASS_COUNT; classIndex++) {
        stats->usedCells[classIndex] = 0;
    }
    block = D_003BD9A8;
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
                freeBytes += freeCells * block->sizeClass->unitSize;
            }
            stats->usedCells[block->sizeClass - D_003E26F0] += block->usedCells;
        }
        block++;
    } while (--blocksRemaining != 0);
    stats->freeBytes = freeBytes;
    stats->emptyBlocks = emptyBlocks;
    stats->partialBlocks = partialBlocks;
}

/* Heap block header: linked list node with its address, state (0 free, 1 used, 2 end marker) and tag. */
typedef struct SdfMemBlock {
    struct SdfMemBlock *prev; /* 0x0 */
    struct SdfMemBlock *next; /* 0x4 */
    u32 address; /* 0x8 */
    u16 state; /* 0xC */
    s16 tag; /* 0xE */
} SdfMemBlock; /* 0x10 */

typedef struct SdfMemHeap {
    SdfMemBlock head; /* 0x00: start sentinel */
    SdfMemBlock tail; /* 0x10: end sentinel */
    u32 base; /* 0x20 */
    u32 size; /* 0x24 */
} SdfMemHeap;

extern SdfMemHeap D_003E2748;
extern void *func_002FF538(u32 size);
extern void *sdfAllocSizeClassBlock(u32 size);
extern s32 D_003BD2DC;
extern u8 D_003BD9C8[4];
extern void sdfReleaseResourceAllocation();
extern void sdfInitializeSynchronizedRequest();

/* Align the usable span to 128 bytes and link one free block between sentinels. */
void sdfInitGeneralHeap(u32 heapSize) {
    SdfMemHeap *heap = &D_003E2748;
    SdfMemBlock *freeBlock;
    u32 alignedStart;
    u32 alignedEnd;

    heap->base = (u32)func_002FF538(heapSize);
    heap->size = heapSize;
    freeBlock = sdfAllocSizeClassBlock(0x10);
    alignedStart = (heap->base + SDF_MEM_ALIGNMENT_MASK) & ~SDF_MEM_ALIGNMENT_MASK;
    alignedEnd = (heap->base + heapSize) & ~SDF_MEM_ALIGNMENT_MASK;
    heap->head.prev = NULL;
    heap->head.next = freeBlock;
    heap->head.state = SDF_MEM_BLOCK_SENTINEL;
    heap->head.tag = -1;
    heap->tail.state = SDF_MEM_BLOCK_SENTINEL;
    heap->tail.tag = -1;
    heap->tail.address = alignedEnd;
    heap->tail.prev = freeBlock;
    heap->tail.next = NULL;
    heap->head.address = alignedStart;
    freeBlock->prev = &heap->head;
    freeBlock->state = SDF_MEM_BLOCK_FREE;
    freeBlock->next = &heap->tail;
    freeBlock->address = alignedStart;
    freeBlock->tag = 0;
    D_003BD2DC = 0;
    sdfInitializeSynchronizedRequest(D_003BD9C8, sdfReleaseResourceAllocation);
}

u16 sdfGetMemoryBlockState(SdfMemBlock *block) {
    return block->state;
}

u32 func_002D0380(void) {
    return D_003E274C[0];
}

INCLUDE_SDATA(const s32, "game/code_002D00F8", D_003BD2DC);

INCLUDE_SDATA(const s32, "game/code_002D00F8", D_003BD2E0);

INCLUDE_SDATA(const s32, "game/code_002D00F8", D_003BD2E1);

