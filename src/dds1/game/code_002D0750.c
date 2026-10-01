#include "common.h"

extern u64 func_002D0A80(void);

extern u8 D_003BD9C8;

void sdfPendingQueuePush(void *arg0, s32 arg1);

INCLUDE_ASM(const s32, "game/code_002D0750", func_002D0750);

void sdfSkipNextListNode(u8 *node) {
    u8 *next = *(u8 **)(*(u8 **)(node + 4) + 4);
    *(u8 **)next = node;
    *(u8 **)(node + 4) = next;
}

INCLUDE_ASM(const s32, "game/code_002D0750", func_002D0918);

void func_002D09B8(void) {
    u64 handle;

    handle = func_002D0A80();
    func_002D0918(handle);
}

void sdfReleaseMemorySlot(s32 *slot) {
    s32 handle;

    handle = *slot;
    if (handle != 0) {
        *slot = 0;
        func_002D0918(handle);
        return;
    }
}

void func_002D0A10(s32 arg0) {
    s32 id = arg0;

    if (id != 0) {
        sdfPendingQueuePush(&D_003BD9C8, id);
    }
}

typedef struct {
    u8 pad00[8];
    u32 address;     /* 0x08 */
    u8 pad0C[2];
    s16 referenceCount; /* 0x0E */
} SdfResource;

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

INCLUDE_ASM(const s32, "game/code_002D0750", func_002D0A80);

INCLUDE_ASM(const s32, "game/code_002D0750", func_002D0B50);

INCLUDE_ASM(const s32, "game/code_002D0750", func_002D0BF8);

extern s32 func_002D03F8(s32 size);
extern s32 func_002F4FD8(void *, s32, s32, void *, s32, void *, s32, s32, s32);
extern u8 D_003E2770[];

s32 func_002D0C68(char *name, s32 dataSize, void *data, s32 *outSize) {
    u8 buffer[0x50];
    s32 nameLength = strlen(name);
    s32 total = nameLength + dataSize + 0xC;
    u32 *block = (u32 *)sdfResourceRetainAddress((SdfResource *)func_002D03F8(total));
    u32 *reply;
    s32 result;

    block[0] = nameLength;
    block[1] = dataSize;
    memcpy(block + 2, name, nameLength + 1);
    if (dataSize != 0) {
        memcpy((u8 *)block + nameLength + 9, data, dataSize);
    }
    reply = (u32 *)(((u32)buffer + 0x3F) & ~0x3F);
    result = func_002F4FD8(D_003E2770, 1, 0, block, total, reply, 8, 0, 0);
    if (result >= 0) {
        if (outSize != NULL) {
            *outSize = reply[1];
        }
        result = reply[0];
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_002D0750", func_002D0D70);

INCLUDE_ASM(const s32, "game/code_002D0750", func_002D0E30);
