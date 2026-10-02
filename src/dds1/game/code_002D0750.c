#include "common.h"

#define SDF_HEAP_BLOCK_FREE 0
#define SDF_HEAP_BLOCK_USED 1
#define SDF_HEAP_BLOCK_END 2

#define SDF_HEAP_STAT_TOTAL_BYTES 0
#define SDF_HEAP_STAT_FREE_BYTES 1
#define SDF_HEAP_STAT_LARGEST_FREE 2
#define SDF_HEAP_STAT_SMALLEST_FREE 3
#define SDF_HEAP_STAT_BLOCK_COUNT 4
#define SDF_HEAP_STAT_FREE_BLOCK_COUNT 5

#define SDF_NAMED_REQUEST_OVERHEAD_BYTES 0xC
#define SDF_RPC_REPLY_ALIGNMENT_MASK 0x3F
#define SDF_NAMED_RESOURCE_RPC_ID 0x6F496453
#define SDF_RPC_BIND_RETRY_TICKS 0x1ED2

typedef struct SdfAllocation {
    struct SdfAllocation *previous;
    struct SdfAllocation *next;
    s32 address;
    u16 state;
    s16 referenceCount;
} SdfAllocation;

extern SdfAllocation *sdfFindGeneralBlockByAddress(void *address);
extern void sdfReleaseChipBlock(void *block);
extern s32 func_00312C08(void);
extern void EIntr(void);

extern u8 D_003BD9C8;

void sdfPendingQueuePush(void *arg0, s32 arg1);

INCLUDE_ASM(const s32, "game/code_002D0750", func_002D0750);

/* Unlink the successor, not node itself, and reconnect both neighboring links. */
void sdfSkipNextListNode(u8 *node) {
    u8 *followingNode = *(u8 **)(*(u8 **)(node + 4) + 4);
    *(u8 **)followingNode = node;
    *(u8 **)(node + 4) = followingNode;
}

/* Coalesce adjacent free records, then recycle this record or mark it free. */
void sdfReleaseResourceAllocation(SdfAllocation *allocation) {
    SdfAllocation *nextBlock;
    s32 interruptsDisabled;

    if (allocation == NULL) {
        return;
    }
    interruptsDisabled = func_00312C08();
    nextBlock = allocation->next;
    if (nextBlock->state == SDF_HEAP_BLOCK_FREE) {
        sdfSkipNextListNode((u8 *)allocation);
        sdfReleaseChipBlock(nextBlock);
    }
    if (allocation->previous->state == SDF_HEAP_BLOCK_FREE) {
        sdfSkipNextListNode((u8 *)allocation->previous);
        sdfReleaseChipBlock(allocation);
    } else {
        allocation->state = SDF_HEAP_BLOCK_FREE;
        allocation->referenceCount = 0;
    }
    if (interruptsDisabled != 0) {
        EIntr();
    }
}

/* Resolve a data address to its allocation record before releasing it. */
void sdfReleaseCurrentResourceHandle(void *address) {
    SdfAllocation *allocation;

    allocation = sdfFindGeneralBlockByAddress(address);
    sdfReleaseResourceAllocation(allocation);
}

/* Clear the owner's handle before releasing the allocation it contained. */
void sdfReleaseMemorySlot(s32 *handleSlot) {
    s32 allocationHandle;

    allocationHandle = *handleSlot;
    if (allocationHandle != 0) {
        *handleSlot = 0;
        sdfReleaseResourceAllocation((SdfAllocation *)allocationHandle);
        return;
    }
}

void sdfQueueNonzeroResourceId(s32 arg0) {
    s32 id = arg0;

    if (id != 0) {
        sdfPendingQueuePush(&D_003BD9C8, id);
    }
}


/* Increment the signed reference count and return the stored data address. */
u32 sdfResourceRetainAddress(SdfAllocation *allocation) {
    allocation->referenceCount = allocation->referenceCount + 1;
    return allocation->address;
}

/* This path reads the same reference-count storage unsigned and never decrements zero. */
void sdfDecrementAllocationReferenceCount(u8 *allocation) {
    u16 referenceCount = *(u16 *)(allocation + 0xE);
    if (referenceCount != 0) {
        *(u16 *)(allocation + 0xE) = referenceCount - 1;
    }
}

typedef struct SdfHeapRoot {
    s32 unk0; /* 0x0 */
    SdfAllocation *first; /* 0x4: first block record */
    s32 unk8; /* 0x8 */
    s32 unkC; /* 0xC */
    SdfAllocation *last; /* 0x10: end marker */
} SdfHeapRoot;

extern SdfHeapRoot D_003E2748;

/* The address must belong to an existing used block: the end-marker path does not end this scan. */
SdfAllocation *sdfFindGeneralBlockByAddress(void *address) {
    SdfHeapRoot *heap = &D_003E2748;
    SdfAllocation *block;
    s32 interruptsDisabled;

    interruptsDisabled = func_00312C08();
    for (block = heap->first;; block = block->next) {
        if (block->state != SDF_HEAP_BLOCK_USED) {
            if (block->state == SDF_HEAP_BLOCK_END) {
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

extern SdfAllocation *D_003E274C[];

/* Walk the general heap's block list and write its statistics: total bytes, free bytes, largest and smallest free block, block count and free block count. */
void sdfGetGeneralHeapStats(s32 *stats) {
    SdfAllocation *block = D_003E274C[0];
    s32 totalBytes = 0;
    s32 freeBytes = 0;
    s32 largestFreeBytes = 0;
    s32 smallestFreeBytes = 0;
    s32 blockCount = 0;
    s32 freeBlockCount = 0;
    s32 blockBytes;
    u16 state;

    for (; block->state != SDF_HEAP_BLOCK_END; block = block->next) {
        blockBytes = block->next->address - block->address;
        state = block->state;
        blockCount++;
        totalBytes += blockBytes;
        if (state == SDF_HEAP_BLOCK_FREE) {
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
    stats[SDF_HEAP_STAT_TOTAL_BYTES] = totalBytes;
    stats[SDF_HEAP_STAT_FREE_BYTES] = freeBytes;
    stats[SDF_HEAP_STAT_SMALLEST_FREE] = smallestFreeBytes;
    stats[SDF_HEAP_STAT_LARGEST_FREE] = largestFreeBytes;
    stats[SDF_HEAP_STAT_BLOCK_COUNT] = blockCount;
    stats[SDF_HEAP_STAT_FREE_BLOCK_COUNT] = freeBlockCount;
}

/* Find the used heap block that contains `address`; NULL when the end marker is reached. */
SdfAllocation *sdfFindGeneralBlockContaining(s32 address) {
    SdfAllocation *block = D_003E274C[0];
    SdfAllocation *nextBlock;

    for (;; block = nextBlock) {
        nextBlock = block->next;
        if (block->state != SDF_HEAP_BLOCK_USED) {
            if (block->state == SDF_HEAP_BLOCK_END) {
                return NULL;
            }
        } else if (address >= block->address && address < nextBlock->address) {
            return block;
        }
    }
}

extern s32 sdfAllocGeneralBlock(s32 size);
extern s32 func_002F4FD8(void *, s32, s32, void *, s32, void *, s32, s32, s32);
typedef struct SifRpcClientData {
    u8 pad00[0x24];
    void *server; /* 0x24: set once the bind succeeded */
} SifRpcClientData;

extern SifRpcClientData D_003E2770;

/* Pack name/data lengths, a terminated name and optional data; return transport error or server result. */
s32 sdfSendNamedResourceRequest(char *name, s32 dataSize, void *data, s32 *outSize) {
    u8 replyScratch[0x50];
    s32 nameLength = strlen(name);
    s32 requestBytes = nameLength + dataSize + SDF_NAMED_REQUEST_OVERHEAD_BYTES;
    u32 *requestWords = (u32 *)sdfResourceRetainAddress((SdfAllocation *)sdfAllocGeneralBlock(requestBytes));
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
    result = func_002F4FD8(&D_003E2770, 1, 0, requestWords, requestBytes, replyWords, 8, 0, 0);
    if (result >= 0) {
        if (outSize != NULL) {
            *outSize = replyWords[1];
        }
        result = replyWords[0];
    }
    return result;
}

extern s32 sceSifLoadModule(const char *path, s32 argLen, const char *args);
extern s32 sceSifMBindRpc(SifRpcClientData *client, s32 id, s32 mode);
extern void func_002F4A38(s32 arg);
extern s32 func_002CF930(void);
extern s32 sdfGetElapsedTimerTicks(s32 start);

/* Load the optional module first, then the required one; bind retries wait in timer ticks. */
void func_002D0D70(const char *modulePath, const char *optionalModulePath) {
    s32 retryStartTick;

    if (optionalModulePath != NULL) {
        while (sceSifLoadModule(optionalModulePath, 0, NULL) < 0) {
        }
    }
    while (sceSifLoadModule(modulePath, 0, NULL) < 0) {
    }
    if (optionalModulePath != NULL) {
        func_002F4A38(0);
    }
    while (1) {
        sceSifMBindRpc(&D_003E2770, SDF_NAMED_RESOURCE_RPC_ID, 0);
        if (D_003E2770.server != NULL) {
            break;
        }
        retryStartTick = func_002CF930();
        while (sdfGetElapsedTimerTicks(retryStartTick) < SDF_RPC_BIND_RETRY_TICKS) {
        }
    }
}

INCLUDE_ASM(const s32, "game/code_002D0750", func_002D0E30);
