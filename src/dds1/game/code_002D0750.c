#include "common.h"

typedef struct SdfResource {
    struct SdfResource *peer;
    struct SdfResource *block;
    s32 address;
    u16 busy;
    s16 referenceCount;
} SdfResource;

extern SdfResource *sdfFindGeneralBlockByAddress(void *address);
extern void sdfReleaseChipBlock(void *block);
extern s32 func_00312C08(void);
extern void EIntr(void);

extern u8 D_003BD9C8;

void sdfPendingQueuePush(void *arg0, s32 arg1);

INCLUDE_ASM(const s32, "game/code_002D0750", func_002D0750);

void sdfSkipNextListNode(u8 *node) {
    u8 *next = *(u8 **)(*(u8 **)(node + 4) + 4);
    *(u8 **)next = node;
    *(u8 **)(node + 4) = next;
}

void sdfReleaseResourceAllocation(SdfResource *node) {
    SdfResource *block;
    s32 lock;

    if (node == NULL) {
        return;
    }
    lock = func_00312C08();
    block = node->block;
    if (block->busy == 0) {
        sdfSkipNextListNode((u8 *)node);
        sdfReleaseChipBlock(block);
    }
    if (node->peer->busy == 0) {
        sdfSkipNextListNode((u8 *)node->peer);
        sdfReleaseChipBlock(node);
    } else {
        node->busy = 0;
        node->referenceCount = 0;
    }
    if (lock != 0) {
        EIntr();
    }
}

void sdfReleaseCurrentResourceHandle(void *address) {
    SdfResource *handle;

    handle = sdfFindGeneralBlockByAddress(address);
    sdfReleaseResourceAllocation(handle);
}

void sdfReleaseMemorySlot(s32 *slot) {
    s32 handle;

    handle = *slot;
    if (handle != 0) {
        *slot = 0;
        sdfReleaseResourceAllocation((SdfResource *)handle);
        return;
    }
}

void sdfQueueNonzeroResourceId(s32 arg0) {
    s32 id = arg0;

    if (id != 0) {
        sdfPendingQueuePush(&D_003BD9C8, id);
    }
}


u32 sdfResourceRetainAddress(SdfResource *resource) {
    resource->referenceCount = resource->referenceCount + 1;
    return resource->address;
}

void sdfDecrementAllocationReferenceCount(u8 *work) {
    u16 value = *(u16 *)(work + 0xE);
    if (value != 0) {
        *(u16 *)(work + 0xE) = value - 1;
    }
}

typedef struct SdfHeapRoot {
    s32 unk0; /* 0x0 */
    SdfResource *first; /* 0x4: first block record */
    s32 unk8; /* 0x8 */
    s32 unkC; /* 0xC */
    SdfResource *last; /* 0x10: end marker */
} SdfHeapRoot;

extern SdfHeapRoot D_003E2748;

/* Find the used heap block whose data address is `address`. */
SdfResource *sdfFindGeneralBlockByAddress(void *address) {
    SdfHeapRoot *heap = &D_003E2748;
    SdfResource *block;
    s32 interruptsDisabled;

    interruptsDisabled = func_00312C08();
    for (block = heap->first;; block = block->block) {
        if (block->busy != 1) {
            if (block->busy == 2) {
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

extern SdfResource *D_003E274C[];

/* Walk the general heap's block list and write its statistics: total bytes, free bytes, largest and smallest free block, block count and free block count. */
void sdfGetGeneralHeapStats(s32 *out) {
    SdfResource *block = D_003E274C[0];
    s32 total = 0;
    s32 freeBytes = 0;
    s32 largest = 0;
    s32 smallest = 0;
    s32 blocks = 0;
    s32 freeBlocks = 0;
    s32 size;
    u16 busy;

    for (; block->busy != 2; block = block->block) {
        size = block->block->address - block->address;
        busy = block->busy;
        blocks++;
        total += size;
        if (busy == 0) {
            if (smallest == 0 || size < smallest) {
                smallest = size;
            }
            if (largest < size) {
                largest = size;
            }
            freeBytes += size;
            freeBlocks++;
        }
    }
    out[0] = total;
    out[1] = freeBytes;
    out[3] = smallest;
    out[2] = largest;
    out[4] = blocks;
    out[5] = freeBlocks;
}

/* Find the used heap block that contains `address`; NULL when the end marker is reached. */
SdfResource *sdfFindGeneralBlockContaining(s32 address) {
    SdfResource *block = D_003E274C[0];
    SdfResource *next;

    for (;; block = next) {
        next = block->block;
        if (block->busy != 1) {
            if (block->busy == 2) {
                return NULL;
            }
        } else if (address >= block->address && address < next->address) {
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

s32 sdfSendNamedResourceRequest(char *name, s32 dataSize, void *data, s32 *outSize) {
    u8 buffer[0x50];
    s32 nameLength = strlen(name);
    s32 total = nameLength + dataSize + 0xC;
    u32 *block = (u32 *)sdfResourceRetainAddress((SdfResource *)sdfAllocGeneralBlock(total));
    u32 *reply;
    s32 result;

    block[0] = nameLength;
    block[1] = dataSize;
    memcpy(block + 2, name, nameLength + 1);
    if (dataSize != 0) {
        memcpy((u8 *)block + nameLength + 9, data, dataSize);
    }
    reply = (u32 *)(((u32)buffer + 0x3F) & ~0x3F);
    result = func_002F4FD8(&D_003E2770, 1, 0, block, total, reply, 8, 0, 0);
    if (result >= 0) {
        if (outSize != NULL) {
            *outSize = reply[1];
        }
        result = reply[0];
    }
    return result;
}

extern s32 sceSifLoadModule(const char *path, s32 argLen, const char *args);
extern s32 sceSifMBindRpc(SifRpcClientData *client, s32 id, s32 mode);
extern void func_002F4A38(s32 arg);
extern s32 func_002CF930(void);
extern s32 sdfGetElapsedTimerTicks(s32 start);

/* Load the two IOP modules (the first optional), then bind the RPC client to its server, retrying until it answers. */
void func_002D0D70(const char *module, const char *firstModule) {
    s32 start;

    if (firstModule != NULL) {
        while (sceSifLoadModule(firstModule, 0, NULL) < 0) {
        }
    }
    while (sceSifLoadModule(module, 0, NULL) < 0) {
    }
    if (firstModule != NULL) {
        func_002F4A38(0);
    }
    while (1) {
        sceSifMBindRpc(&D_003E2770, 0x6F496453, 0);
        if (D_003E2770.server != NULL) {
            break;
        }
        start = func_002CF930();
        while (sdfGetElapsedTimerTicks(start) < 0x1ED2) {
        }
    }
}

INCLUDE_ASM(const s32, "game/code_002D0750", func_002D0E30);
