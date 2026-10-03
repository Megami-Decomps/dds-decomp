#include "common.h"
#include "sdf.h"

#define SDF_HEAP_BLOCK_FREE 0
#define SDF_HEAP_BLOCK_USED 1
#define SDF_HEAP_BLOCK_END 2
#define SDF_HEAP_ALIGNMENT_MASK 0x7F
#define SDF_HEAP_BLOCK_DESCRIPTOR_BYTES 0x10

SdfMemHeap sdfGeneralHeap __attribute__((section(".bss"), aligned(8)));

/* Return the next descriptor unless it is the end marker; used blocks are not filtered. */
SdfMemBlock *sdfMemoryNextBlock(SdfMemBlock *block) {
    SdfMemBlock *nextBlock = block->next;
    if (nextBlock->state == SDF_HEAP_BLOCK_END) {
        return NULL;
    }
    return nextBlock;
}

extern s32 func_0036DE70(void);

extern void EIntr(void);

/* Derive the byte count from adjacent addresses while preserving the interrupt state. */
s32 sdfMemoryGetBlockSize(SdfMemBlock *block) {
    s32 blockBytes;
    s32 restoreInterrupts;

    restoreInterrupts = func_0036DE70();
    blockBytes = block->next->address - block->address;
    if (restoreInterrupts) {
        EIntr();
    }
    return blockBytes;
}

/* Return the represented address, not the allocation descriptor's address. */
u32 sdfMemoryGetBlockAddress(SdfMemBlock *block) {
    return block->address;
}

extern void (*D_004389CC)(s32);
extern SdfMemBlock *sdfAllocSizeClassBlock(s32 size);

/* Low-end first-fit with 128-byte rounding; returns an allocation handle.
 * At the end marker, call the optional out-of-memory hook and continue searching. */
SdfMemBlock *sdfAllocGeneralBlock(s32 requestedBytes) {
    SdfMemHeap *heap = &sdfGeneralHeap;
    s32 alignedBytes = (requestedBytes + SDF_HEAP_ALIGNMENT_MASK) & ~SDF_HEAP_ALIGNMENT_MASK;
    s32 restoreInterrupts;
    SdfMemBlock *block;

    restoreInterrupts = func_0036DE70();
    for (block = heap->head.next;; block = block->next) {
        if (block->state != SDF_HEAP_BLOCK_FREE) {
            if (block->state == SDF_HEAP_BLOCK_END) {
                if (restoreInterrupts != 0) {
                    EIntr();
                }
                if (D_004389CC != NULL) {
                    D_004389CC(alignedBytes);
                }
            }
        } else {
            s32 availableBytes = block->next->address - block->address;

            if (availableBytes >= alignedBytes) {
                if (alignedBytes < availableBytes) {
                    SdfMemBlock *splitBlock = sdfAllocSizeClassBlock(SDF_HEAP_BLOCK_DESCRIPTOR_BYTES);

                    splitBlock->prev = block;
                    splitBlock->address = block->address + alignedBytes;
                    splitBlock->state = SDF_HEAP_BLOCK_FREE;
                    splitBlock->next = block->next;
                    splitBlock->referenceCount = 0;
                    block->next->prev = splitBlock;
                    block->next = splitBlock;
                }
                block->state = SDF_HEAP_BLOCK_USED;
                if (restoreInterrupts != 0) {
                    EIntr();
                }
                return block;
            }
        }
    }
}

/* Search backward and carve the upper end of a free span with 128-byte rounding.
 * End markers restore interrupts but do not terminate the search or invoke the hook. */
SdfMemBlock *sdfAllocGeneralBlockHigh(s32 requestedBytes) {
    SdfMemHeap *heap = &sdfGeneralHeap;
    s32 alignedBytes = (requestedBytes + SDF_HEAP_ALIGNMENT_MASK) & ~SDF_HEAP_ALIGNMENT_MASK;
    s32 restoreInterrupts;
    SdfMemBlock *block;

    restoreInterrupts = func_0036DE70();
    for (block = heap->tail.prev;; block = block->prev) {
        if (block->state != SDF_HEAP_BLOCK_FREE) {
            if (block->state == SDF_HEAP_BLOCK_END) {
                if (restoreInterrupts != 0) {
                    EIntr();
                }
            }
        } else {
            s32 availableBytes = block->next->address - block->address;

            if (availableBytes >= alignedBytes) {
                if (alignedBytes < availableBytes) {
                    SdfMemBlock *splitBlock = sdfAllocSizeClassBlock(SDF_HEAP_BLOCK_DESCRIPTOR_BYTES);

                    splitBlock->prev = block;
                    splitBlock->address = block->address + (availableBytes - alignedBytes);
                    splitBlock->state = SDF_HEAP_BLOCK_FREE;
                    splitBlock->next = block->next;
                    splitBlock->referenceCount = 0;
                    block->next->prev = splitBlock;
                    block->next = splitBlock;
                    /* This new upper descriptor becomes the allocation, not the free remainder. */
                    block = splitBlock;
                }
                block->state = SDF_HEAP_BLOCK_USED;
                if (restoreInterrupts != 0) {
                    EIntr();
                }
                return block;
            }
        }
    }
}

/* Low-end first-fit without the out-of-memory hook; return NULL at the end marker.
 * Rounding and split layout are the same as the normal low-end allocator. */
SdfMemBlock *sdfTryAllocGeneralBlock(s32 requestedBytes) {
    SdfMemHeap *heap = &sdfGeneralHeap;
    s32 alignedBytes = (requestedBytes + SDF_HEAP_ALIGNMENT_MASK) & ~SDF_HEAP_ALIGNMENT_MASK;
    s32 restoreInterrupts;
    SdfMemBlock *block;

    restoreInterrupts = func_0036DE70();
    for (block = heap->head.next;; block = block->next) {
        if (block->state != SDF_HEAP_BLOCK_FREE) {
            if (block->state == SDF_HEAP_BLOCK_END) {
                if (restoreInterrupts != 0) {
                    EIntr();
                }
                return NULL;
            }
        } else {
            s32 availableBytes = block->next->address - block->address;

            if (availableBytes >= alignedBytes) {
                if (alignedBytes < availableBytes) {
                    SdfMemBlock *splitBlock = sdfAllocSizeClassBlock(SDF_HEAP_BLOCK_DESCRIPTOR_BYTES);

                    splitBlock->prev = block;
                    splitBlock->address = block->address + alignedBytes;
                    splitBlock->state = SDF_HEAP_BLOCK_FREE;
                    splitBlock->next = block->next;
                    splitBlock->referenceCount = 0;
                    block->next->prev = splitBlock;
                    block->next = splitBlock;
                }
                block->state = SDF_HEAP_BLOCK_USED;
                if (restoreInterrupts != 0) {
                    EIntr();
                }
                return block;
            }
        }
    }
}
