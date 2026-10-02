#include "common.h"

#define SDF_HEAP_BLOCK_FREE 0
#define SDF_HEAP_BLOCK_USED 1
#define SDF_HEAP_BLOCK_END 2
#define SDF_HEAP_ALIGNMENT_MASK 0x7F
#define SDF_HEAP_BLOCK_DESCRIPTOR_BYTES 0x10

/* Allocation handle: its represented byte span ends at the next descriptor's address. */
typedef struct MemBlock {
    /* 0x0 */ struct MemBlock *prev;
    /* 0x4 */ struct MemBlock *next;
    /* 0x8 */ u32 address;
    /* 0xC */ u16 state; /* 0 free, 1 used, 2 end marker */
    /* 0xE */ u16 unkE;
} MemBlock;

typedef struct MemHeap {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ MemBlock *head;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ MemBlock *tail;
} MemHeap;

extern MemHeap D_003E2748;
extern void (*D_003BD2DC)(s32);

s32 func_00312C08(void);
s32 EIntr(void);
MemBlock *sdfAllocSizeClassBlock(s32 size);

/* Return the next descriptor unless it is the end marker; used blocks are not filtered. */
MemBlock *sdfMemoryNextBlock(MemBlock *block) {
    MemBlock *nextBlock = block->next;
    if (nextBlock->state == SDF_HEAP_BLOCK_END) {
        return NULL;
    }
    return nextBlock;
}

/* Derive the byte count from adjacent addresses while preserving the interrupt state. */
s32 sdfMemoryGetBlockSize(MemBlock *block) {
    s32 blockBytes;
    s32 restoreInterrupts;

    restoreInterrupts = func_00312C08();
    blockBytes = block->next->address - block->address;
    if (restoreInterrupts) {
        EIntr();
    }
    return blockBytes;
}

/* Return the represented address, not the allocation descriptor's address. */
u32 sdfMemoryGetBlockAddress(MemBlock *block) {
    return block->address;
}

/* Low-end first-fit with 128-byte rounding; returns an allocation handle.
 * At the end marker, call the optional out-of-memory hook and continue searching. */
MemBlock *sdfAllocGeneralBlock(s32 requestedBytes) {
    MemHeap *heap = &D_003E2748;
    s32 alignedBytes = (requestedBytes + SDF_HEAP_ALIGNMENT_MASK) & ~SDF_HEAP_ALIGNMENT_MASK;
    s32 restoreInterrupts;
    MemBlock *block;

    restoreInterrupts = func_00312C08();
    for (block = heap->head;; block = block->next) {
        if (block->state != SDF_HEAP_BLOCK_FREE) {
            if (block->state == SDF_HEAP_BLOCK_END) {
                if (restoreInterrupts != 0) {
                    EIntr();
                }
                if (D_003BD2DC != NULL) {
                    D_003BD2DC(alignedBytes);
                }
            }
        } else {
            s32 availableBytes = block->next->address - block->address;

            if (availableBytes >= alignedBytes) {
                if (alignedBytes < availableBytes) {
                    MemBlock *splitBlock = sdfAllocSizeClassBlock(SDF_HEAP_BLOCK_DESCRIPTOR_BYTES);

                    splitBlock->prev = block;
                    splitBlock->address = block->address + alignedBytes;
                    splitBlock->state = SDF_HEAP_BLOCK_FREE;
                    splitBlock->next = block->next;
                    splitBlock->unkE = 0;
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
MemBlock *sdfAllocGeneralBlockHigh(s32 requestedBytes) {
    MemHeap *heap = &D_003E2748;
    s32 alignedBytes = (requestedBytes + SDF_HEAP_ALIGNMENT_MASK) & ~SDF_HEAP_ALIGNMENT_MASK;
    s32 restoreInterrupts;
    MemBlock *block;

    restoreInterrupts = func_00312C08();
    for (block = heap->tail;; block = block->prev) {
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
                    MemBlock *splitBlock = sdfAllocSizeClassBlock(SDF_HEAP_BLOCK_DESCRIPTOR_BYTES);

                    splitBlock->prev = block;
                    splitBlock->address = block->address + (availableBytes - alignedBytes);
                    splitBlock->state = SDF_HEAP_BLOCK_FREE;
                    splitBlock->next = block->next;
                    splitBlock->unkE = 0;
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
MemBlock *sdfTryAllocGeneralBlock(s32 requestedBytes) {
    MemHeap *heap = &D_003E2748;
    s32 alignedBytes = (requestedBytes + SDF_HEAP_ALIGNMENT_MASK) & ~SDF_HEAP_ALIGNMENT_MASK;
    s32 restoreInterrupts;
    MemBlock *block;

    restoreInterrupts = func_00312C08();
    for (block = heap->head;; block = block->next) {
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
                    MemBlock *splitBlock = sdfAllocSizeClassBlock(SDF_HEAP_BLOCK_DESCRIPTOR_BYTES);

                    splitBlock->prev = block;
                    splitBlock->address = block->address + alignedBytes;
                    splitBlock->state = SDF_HEAP_BLOCK_FREE;
                    splitBlock->next = block->next;
                    splitBlock->unkE = 0;
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
