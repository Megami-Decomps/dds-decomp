#include "common.h"
#include "sdf_chip.h"
#include "sdf_heap_api.h"
#include "sdf_general_heap_api.h"
#include "sdf.h"
#include "sdf_chip_page_link_view.h"
#include "sdf_pending.h"
#include "sdk_ps2.h"
#include "sdf_timer.h"
#include "sdf_named_resource_request_api.h"
#include "sif_rpc_client_prefix.h"
#include "sdf_mem_block_state.h"

#define SDF_MEM_ALIGNMENT_MASK 0x7F

extern void *malloc(u32 size);

extern void (*D_003BD2DC)(s32);

extern SdfPendingRequest sdfMemDelayedResrcCB;

/* Align the usable span to 128 bytes and link one free block between sentinels. */
void sdfMemInit(u32 heapSize) {
    SdfMemHeap *heap = &sdfMemCB;
    sdfMemHandle_t *freeBlock;
    u32 alignedStart;
    u32 alignedEnd;

    heap->base = (u32)malloc(heapSize);
    heap->size = heapSize;
    freeBlock = sdfChipAllocDirect(sizeof(sdfMemHandle_t));
    alignedStart = (heap->base + SDF_MEM_ALIGNMENT_MASK) & ~SDF_MEM_ALIGNMENT_MASK;
    alignedEnd = (heap->base + heapSize) & ~SDF_MEM_ALIGNMENT_MASK;
    heap->head.prev = NULL;
    heap->head.next = freeBlock;
    heap->head.state = SDF_MEM_BLOCK_STATE_END_SENTINEL;
    heap->head.referenceCount = -1;
    heap->tail.state = SDF_MEM_BLOCK_STATE_END_SENTINEL;
    heap->tail.referenceCount = -1;
    heap->tail.address = alignedEnd;
    heap->tail.prev = freeBlock;
    heap->tail.next = NULL;
    heap->head.address = alignedStart;
    freeBlock->prev = &heap->head;
    freeBlock->state = SDF_MEM_BLOCK_STATE_FREE;
    freeBlock->next = &heap->tail;
    freeBlock->address = alignedStart;
    freeBlock->referenceCount = 0;
    D_003BD2DC = 0;
    sdfDelayedResrcInitCB(&sdfMemDelayedResrcCB, (SdfPendingEntryCallback)sdfMemFreeDirect);
}

SdfMemBlockState sdfMemGetType(const sdfMemHandle_t *block) {
    return block->state;
}

/* Return the first general-heap allocation descriptor. */
const sdfMemHandle_t *sdfMemGetTop(void) {
    return sdfMemCB.head.next;
}

#define SDF_HEAP_ALIGNMENT_MASK 0x7F

SdfMemHeap sdfMemCB __attribute__((section(".bss"), aligned(8)));

/* Return the next descriptor unless it is the end marker; used blocks are not filtered. */
const sdfMemHandle_t *sdfMemGetNext(const sdfMemHandle_t *block) {
    const sdfMemHandle_t *nextBlock = block->next;
    if (nextBlock->state == SDF_MEM_BLOCK_STATE_END_SENTINEL) {
        return NULL;
    }
    return nextBlock;
}

/* Derive the byte count from adjacent addresses while preserving the interrupt state. */
s32 sdfMemGetSize(const sdfMemHandle_t *block) {
    const sdfMemHandle_t *nextBlock;
    s32 blockBytes;
    s32 interruptsWereEnabled;

    interruptsWereEnabled = DIntr();
    nextBlock = block->next;
    blockBytes = nextBlock->address - block->address;
    if (interruptsWereEnabled) {
        EIntr();
    }
    return blockBytes;
}

/* Return the represented address, not the allocation descriptor's address. */
u8 *sdfMemGetPtr(const sdfMemHandle_t *block) {
    return (u8 *)(u32)block->address;
}

/* Low-end first-fit with 128-byte rounding; returns an allocation handle.
 * At the end marker, call the optional out-of-memory hook and continue searching. */
sdfMemHandle_t *sdfMemAllocDirect(s32 requestedBytes) {
    SdfMemHeap *heap = &sdfMemCB;
    s32 alignedBytes = (requestedBytes + SDF_HEAP_ALIGNMENT_MASK) & ~SDF_HEAP_ALIGNMENT_MASK;
    s32 interruptsWereEnabled;
    sdfMemHandle_t *block;

    interruptsWereEnabled = DIntr();
    for (block = heap->head.next;; block = block->next) {
        if (block->state != SDF_MEM_BLOCK_STATE_FREE) {
            if (block->state == SDF_MEM_BLOCK_STATE_END_SENTINEL) {
                if (interruptsWereEnabled != 0) {
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
                    sdfMemHandle_t *splitBlock = sdfChipAllocDirect(sizeof(sdfMemHandle_t));

                    splitBlock->prev = block;
                    splitBlock->address = block->address + alignedBytes;
                    splitBlock->state = SDF_MEM_BLOCK_STATE_FREE;
                    splitBlock->next = block->next;
                    splitBlock->referenceCount = 0;
                    block->next->prev = splitBlock;
                    block->next = splitBlock;
                }
                block->state = SDF_MEM_BLOCK_STATE_USED;
                if (interruptsWereEnabled != 0) {
                    EIntr();
                }
                return block;
            }
        }
    }
}

/* Search backward and carve the upper end of a free span with 128-byte rounding.
 * End markers restore interrupts but do not terminate the search or invoke the hook. */
sdfMemHandle_t *sdfMemAllocFromTailDirect(s32 requestedBytes) {
    SdfMemHeap *heap = &sdfMemCB;
    s32 alignedBytes = (requestedBytes + SDF_HEAP_ALIGNMENT_MASK) & ~SDF_HEAP_ALIGNMENT_MASK;
    s32 interruptsWereEnabled;
    sdfMemHandle_t *block;

    interruptsWereEnabled = DIntr();
    for (block = heap->tail.prev;; block = block->prev) {
        if (block->state != SDF_MEM_BLOCK_STATE_FREE) {
            if (block->state == SDF_MEM_BLOCK_STATE_END_SENTINEL) {
                if (interruptsWereEnabled != 0) {
                    EIntr();
                }
            }
        } else {
            s32 availableBytes = block->next->address - block->address;

            if (availableBytes >= alignedBytes) {
                if (alignedBytes < availableBytes) {
                    sdfMemHandle_t *splitBlock = sdfChipAllocDirect(sizeof(sdfMemHandle_t));

                    splitBlock->prev = block;
                    splitBlock->address = block->address + (availableBytes - alignedBytes);
                    splitBlock->state = SDF_MEM_BLOCK_STATE_FREE;
                    splitBlock->next = block->next;
                    splitBlock->referenceCount = 0;
                    block->next->prev = splitBlock;
                    block->next = splitBlock;
                    /* This new upper descriptor becomes the allocation, not the free remainder. */
                    block = splitBlock;
                }
                block->state = SDF_MEM_BLOCK_STATE_USED;
                if (interruptsWereEnabled != 0) {
                    EIntr();
                }
                return block;
            }
        }
    }
}

/* Low-end first-fit without the out-of-memory hook; return NULL at the end marker.
 * Rounding and split layout are the same as the normal low-end allocator. */
sdfMemHandle_t *sdfMemAlloc2Direct(s32 requestedBytes) {
    SdfMemHeap *heap = &sdfMemCB;
    s32 alignedBytes = (requestedBytes + SDF_HEAP_ALIGNMENT_MASK) & ~SDF_HEAP_ALIGNMENT_MASK;
    s32 interruptsWereEnabled;
    sdfMemHandle_t *block;

    interruptsWereEnabled = DIntr();
    for (block = heap->head.next;; block = block->next) {
        if (block->state != SDF_MEM_BLOCK_STATE_FREE) {
            if (block->state == SDF_MEM_BLOCK_STATE_END_SENTINEL) {
                if (interruptsWereEnabled != 0) {
                    EIntr();
                }
                return NULL;
            }
        } else {
            s32 availableBytes = block->next->address - block->address;

            if (availableBytes >= alignedBytes) {
                if (alignedBytes < availableBytes) {
                    sdfMemHandle_t *splitBlock = sdfChipAllocDirect(sizeof(sdfMemHandle_t));

                    splitBlock->prev = block;
                    splitBlock->address = block->address + alignedBytes;
                    splitBlock->state = SDF_MEM_BLOCK_STATE_FREE;
                    splitBlock->next = block->next;
                    splitBlock->referenceCount = 0;
                    block->next->prev = splitBlock;
                    block->next = splitBlock;
                }
                block->state = SDF_MEM_BLOCK_STATE_USED;
                if (interruptsWereEnabled != 0) {
                    EIntr();
                }
                return block;
            }
        }
    }
}

#define SDF_NAMED_REQUEST_OVERHEAD_BYTES 0xC

#define SDF_RPC_REPLY_ALIGNMENT_MASK 0x3F

/* Resize backing storage while preserving this resource's allocation descriptor. */
void sdfMemReallocRecord(sdfMemHandle_t *allocation, s32 size) {
    sdfMemHandle_t *nextBlock;
    sdfMemHandle_t *replacement;
    s32 temporaryAddress;
    s32 interruptsDisabled;
    s32 currentSize;
    s32 available;

    interruptsDisabled = DIntr();
    nextBlock = allocation->next;
    temporaryAddress = allocation->address;
    currentSize = nextBlock->address - temporaryAddress;
    size = (size + 0x7F) & -0x80;
    if (size == currentSize) {
        if (interruptsDisabled != 0) {
            EIntr();
        }
        return;
    }
    if (currentSize < size) {
        available = nextBlock->next->address - nextBlock->address + currentSize;
        if (nextBlock->state == SDF_MEM_BLOCK_STATE_FREE && available >= size) {
            if (available == size) {
                nextBlock->next->prev = allocation;
                allocation->next = nextBlock->next;
                sdfChipFreeDirect(nextBlock);
            } else {
                nextBlock->address = temporaryAddress + size;
            }
        } else {
            s32 oldAddress;

            replacement = sdfMemAllocDirect(size);
            memcpy((void *)replacement->address, (void *)allocation->address, currentSize);
            temporaryAddress = (s32)allocation->prev;
            oldAddress = allocation->address;
            allocation->prev = replacement->prev;
            allocation->address = replacement->address;
            allocation->next = replacement->next;
            replacement->prev = (sdfMemHandle_t *)temporaryAddress;
            replacement->next = nextBlock;
            replacement->address = oldAddress;
            allocation->prev->next = allocation;
            allocation->next->prev = allocation;
            replacement->prev->next = replacement;
            replacement->next->prev = replacement;
            sdfMemFreeDirect(replacement);
        }
    } else if (nextBlock->state == SDF_MEM_BLOCK_STATE_FREE) {
        nextBlock->address = temporaryAddress + size;
    } else {
        temporaryAddress = (s32)sdfChipAllocDirect(0x10);
        ((sdfMemHandle_t *)temporaryAddress)->prev = allocation;
        ((sdfMemHandle_t *)temporaryAddress)->next = nextBlock;
        nextBlock->prev = (sdfMemHandle_t *)temporaryAddress;
        ((sdfMemHandle_t *)temporaryAddress)->state = SDF_MEM_BLOCK_STATE_FREE;
        allocation->next = (sdfMemHandle_t *)temporaryAddress;
        ((sdfMemHandle_t *)temporaryAddress)->referenceCount = 0;
        ((sdfMemHandle_t *)temporaryAddress)->address = allocation->address + size;
    }
    if (interruptsDisabled != 0) {
        EIntr();
    }
}

/* Unlink the successor, not node itself, and reconnect both neighboring links. */
void sdfMemFreeMerge(sdfMemHandle_t *node) {
    sdfMemHandle_t *followingNode = node->next->next;
    followingNode->prev = node;
    node->next = followingNode;
}

/* Coalesce adjacent free records, then recycle this record or mark it free. */
void sdfMemFreeDirect(struct sdfMemHandle_t *allocation) {
    sdfMemHandle_t *nextBlock;
    s32 interruptsDisabled;

    if (allocation == NULL) {
        return;
    }
    interruptsDisabled = DIntr();
    nextBlock = allocation->next;
    if (nextBlock->state == SDF_MEM_BLOCK_STATE_FREE) {
        sdfMemFreeMerge(allocation);
        sdfChipFreeDirect(nextBlock);
    }
    if (allocation->prev->state == SDF_MEM_BLOCK_STATE_FREE) {
        sdfMemFreeMerge(allocation->prev);
        sdfChipFreeDirect(allocation);
    } else {
        allocation->state = SDF_MEM_BLOCK_STATE_FREE;
        allocation->referenceCount = 0;
    }
    if (interruptsDisabled != 0) {
        EIntr();
    }
}

/* Resolve a data address to its allocation record before releasing it. */
void sdfMemFreePtrDirect(const void *address) {
    sdfMemHandle_t *allocation;

    allocation = sdfMemGetHandleByPtr(address);
    sdfMemFreeDirect(allocation);
}

/* Clear the owner's handle before releasing the allocation it contained. */
void sdfMemFreeHandleDirect(struct sdfMemHandle_t **handleSlot) {
    struct sdfMemHandle_t *allocationHandle;

    allocationHandle = *handleSlot;
    if (allocationHandle != NULL) {
        *handleSlot = NULL;
        sdfMemFreeDirect(allocationHandle);
        return;
    }
}

/* Zero is a sentinel and is never submitted to the pending queue. */
void sdfMemFreeDelayedDirect(struct sdfMemHandle_t *allocation) {
    if (allocation != NULL) {
        sdfDelayedResrcEntry(&sdfMemDelayedResrcCB, allocation);
    }
}

/* Increment the signed reference count and return the stored data address. */
void *sdfMemLock(sdfMemHandle_t *allocation) {
    allocation->referenceCount = allocation->referenceCount + 1;
    return (void *)allocation->address;
}

/* This path reads the same reference-count storage unsigned and never decrements zero. */
void sdfMemUnlock(sdfMemHandle_t *allocation) {
    u16 referenceCount = (u16)allocation->referenceCount;
    if (referenceCount != 0) {
        allocation->referenceCount = (s16)(referenceCount - 1);
    }
}

/* The address must belong to an existing used block: the end-marker path does not end this scan. */
sdfMemHandle_t *sdfMemGetHandleByPtr(const void *address) {
    SdfMemHeap *heap = &sdfMemCB;
    sdfMemHandle_t *block;
    s32 interruptsDisabled;

    interruptsDisabled = DIntr();
    for (block = heap->head.next;; block = block->next) {
        if (block->state != SDF_MEM_BLOCK_STATE_USED) {
            if (block->state == SDF_MEM_BLOCK_STATE_END_SENTINEL) {
                if (interruptsDisabled != 0) {
                    EIntr();
                }
            }
        } else if (block->address == (u32)address) {
            if (interruptsDisabled != 0) {
                EIntr();
            }
            return block;
        }
    }
}

/* Walk the general heap's block list and write its statistics: total bytes, free bytes, largest and smallest free block, block count and free block count. */
void sdfMemGetInfo(SdfGeneralHeapStats *stats) {
    sdfMemHandle_t *block = sdfMemCB.head.next;
    s32 totalBytes = 0;
    s32 freeBytes = 0;
    s32 largestFreeBytes = 0;
    s32 smallestFreeBytes = 0;
    s32 blockCount = 0;
    s32 freeBlockCount = 0;
    s32 blockBytes;
    SdfMemBlockState state;

    for (; block->state != SDF_MEM_BLOCK_STATE_END_SENTINEL; block = block->next) {
        blockBytes = block->next->address - block->address;
        state = block->state;
        blockCount++;
        totalBytes += blockBytes;
        if (state == SDF_MEM_BLOCK_STATE_FREE) {
            if (smallestFreeBytes == 0 || blockBytes < smallestFreeBytes) {
                smallestFreeBytes = blockBytes;
            }
            if (largestFreeBytes < blockBytes) {
                largestFreeBytes = blockBytes;
            }
            freeBytes += blockBytes;
            freeBlockCount++;
        }
    }
    stats->totalBytes = totalBytes;
    stats->freeBytes = freeBytes;
    stats->smallestFreeBytes = smallestFreeBytes;
    stats->largestFreeBytes = largestFreeBytes;
    stats->blockCount = blockCount;
    stats->freeBlockCount = freeBlockCount;
}

/* Find the used heap block that contains `address`; NULL when the end marker is reached. */
sdfMemHandle_t *sdfMemSearchHandle(s32 address) {
    sdfMemHandle_t *block = sdfMemCB.head.next;
    sdfMemHandle_t *nextBlock;

    for (;; block = nextBlock) {
        nextBlock = block->next;
        if (block->state != SDF_MEM_BLOCK_STATE_USED) {
            if (block->state == SDF_MEM_BLOCK_STATE_END_SENTINEL) {
                return NULL;
            }
        } else if (address >= block->address && address < nextBlock->address) {
            return block;
        }
    }
}

extern SifRpcClientDataPrefix sdfIopRpcClient;

/* Pack name/data lengths, a terminated name and optional data; return transport error or server result. */
s32 SdfIopFileRpc(const char *name, s32 dataSize, const void *data, s32 *outSize) {
    u8 replyScratch[0x50];
    s32 nameLength = strlen(name);
    s32 requestBytes = nameLength + dataSize + SDF_NAMED_REQUEST_OVERHEAD_BYTES;
    u32 *requestWords = (u32 *)sdfMemLock((struct sdfMemHandle_t *)sdfMemAllocDirect(requestBytes));
    u32 *replyWords;
    s32 result;

    requestWords[0] = nameLength;
    requestWords[1] = dataSize;
    memcpy(requestWords + 2, name, nameLength + 1);
    if (dataSize != 0) {
        memcpy((u8 *)requestWords + nameLength + 9, data, dataSize);
    }
    /* Align the two-word reply within the stack scratch buffer to 64 bytes. */
    replyWords = (u32 *)(((u32)replyScratch + SDF_RPC_REPLY_ALIGNMENT_MASK) & ~SDF_RPC_REPLY_ALIGNMENT_MASK);
    result = sceSifMCallRpc(&sdfIopRpcClient, 1, 0, requestWords, requestBytes, replyWords, 8, NULL, NULL);
    if (result >= 0) {
        if (outSize != NULL) {
            *outSize = replyWords[1];
        }
        result = replyWords[0];
    }
    return result;
}
INCLUDE_SDATA(const s32, "sdf/sdfMemory", D_003BD2DC);

INCLUDE_SDATA(const s32, "sdf/sdfMemory", sdfGrDrawFirstFlag);

INCLUDE_SDATA(const s32, "sdf/sdfMemory", sdfFrameEvenOdd);

