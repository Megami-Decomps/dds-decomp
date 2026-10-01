#include "common.h"

extern u64 func_00329930(void);

typedef struct SdfAllocation {
    u8 pad00[8];
    u32 address;
    u8 pad0C[2];
    union {
        s16 referenceCount;
        u16 unsignedReferenceCount;
    };
} SdfAllocation;

typedef struct SdfListNode {
    struct SdfListNode *previous;
    struct SdfListNode *next;
} SdfListNode;

extern u8 D_00439128;

void sdfPendingQueuePush(void *arg0, s32 arg1);

INCLUDE_ASM(const s32, "game/code_00329600", func_00329600);

void sdfSkipNextListNode(SdfListNode *node) {
    SdfListNode *next = node->next->next;
    next->previous = node;
    node->next = next;
}

INCLUDE_ASM(const s32, "game/code_00329600", func_003297C8);

void func_00329868(void) {
    u64 resource;

    resource = func_00329930();
    func_003297C8(resource);
}

void sdfReleaseMemorySlot(s32 *slot) {
    s32 resource;

    resource = *slot;
    if (resource != 0) {
        *slot = 0;
        func_003297C8(resource);
        return;
    }
}

void sdfQueueNonzeroResourceId(s32 arg0) {
    s32 id = arg0;

    if (id != 0) {
        sdfPendingQueuePush(&D_00439128, id);
    }
}

u32 sdfResourceRetainAddress(SdfAllocation *allocation) {
    allocation->referenceCount = allocation->referenceCount + 1;
    return allocation->address;
}

void sdfDecrementAllocationReferenceCount(SdfAllocation *allocation) {
    u16 value = allocation->unsignedReferenceCount;
    if (value != 0) {
        allocation->unsignedReferenceCount = value - 1;
    }
}

INCLUDE_ASM(const s32, "game/code_00329600", func_00329930);

INCLUDE_ASM(const s32, "game/code_00329600", func_00329A00);

INCLUDE_ASM(const s32, "game/code_00329600", func_00329AA8);

extern s32 func_003292A8(s32 size);
extern s32 func_0034DE68(void *, s32, s32, void *, s32, void *, s32, s32, s32);
extern u8 D_0045F120[];

s32 func_00329B18(char *name, s32 dataSize, void *data, s32 *outSize) {
    u8 buffer[0x50];
    s32 nameLength = strlen(name);
    s32 total = nameLength + dataSize + 0xC;
    u32 *block = (u32 *)sdfResourceRetainAddress((SdfAllocation *)func_003292A8(total));
    u32 *reply;
    s32 result;

    block[0] = nameLength;
    block[1] = dataSize;
    memcpy(block + 2, name, nameLength + 1);
    if (dataSize != 0) {
        memcpy((u8 *)block + nameLength + 9, data, dataSize);
    }
    reply = (u32 *)(((u32)buffer + 0x3F) & ~0x3F);
    result = func_0034DE68(D_0045F120, 1, 0, block, total, reply, 8, 0, 0);
    if (result >= 0) {
        if (outSize != NULL) {
            *outSize = reply[1];
        }
        result = reply[0];
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_00329600", func_00329C20);

INCLUDE_ASM(const s32, "game/code_00329600", func_00329CE0);
