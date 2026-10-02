#include "common.h"
#include "sdf.h"

typedef struct SdfTexHead {
    SdfTex *next; /* 0x0: SdfTex-compatible linked-list prefix */
    SdfTex *prev; /* 0x4 */
    void *unk8; /* 0x8 */
    void *unkC; /* 0xC */
    s32 unk10; /* 0x10 */
    s16 width; /* 0x14 */
    s16 height; /* 0x16 */
    s32 format; /* 0x18 */
} SdfTexHead;

typedef struct SdfTexPacketTail {
    u64 tag;      /* 0x00 */
    u64 next;     /* 0x08 */
} SdfTexPacketTail;

typedef struct SdfTexReleaseEntry {
    struct SdfTexReleaseEntry *next; /* 0x00 */
    s32 address;                     /* 0x04 */
    s32 handle;                      /* 0x08 */
    u8 mode;                         /* 0x0C: 1 = handle, 2 = chip memory address */
    u8 pad0D[0x93];
} SdfTexReleaseEntry; /* 0xA0 */

extern SdfTexHead *sdfTextureBlockListHead;
extern SdfTexHead *sdfTextureListHead;
extern s8 D_003BD300[2];

/* Busy-buffer index is published by the slot setters and polled below;
 * volatile prevents the wait loop from reusing an earlier read. */
extern volatile s8 sdfBusyBufferIndex;
extern SdfTex *sdfResourceListHead;
extern u8 sdfTextureUpdateQueue;
extern SdfSemaObj sdfTextureQueueWork;
extern u8 D_003BD2F0;
extern u32 D_003BD2F4;
extern u32 D_003BD2F8;

void *func_002CFEB8(s32 size);
s32 sdfCreateSemaphore(s32 arg0, s32 arg1, s32 arg2);
struct SdfTexHead *func_002D17D8(s32 size, s32 arg1);
void sdfUpdateTextureHeadsWithInterruptsMasked(void *block);
void sdfTexCreateSecondPacket(void);
void func_002D2FB0(void);
void sdfPendingQueuePush(void *arg0, s32 arg1);
void sdfInitializeSynchronizedRequest(void *arg0, void (*arg1)(void *));
void *sdfAllocAndClearQuadwords(s32 size);

s32 func_002D0A80(s32 address);

s32 sdfChipIsInRange(s32 address);

void sdfRequestDeferredGsImageCapture(u32 destination, u32 onComplete) {
    D_003BD2F4 = destination;
    D_003BD2F8 = onComplete;
    D_003BD2F0 = 1;
}

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D10C8);

/* Switch both references off the finished double-buffer slot before
 * publishing the slot currently in use. */
void sdfSwapBufferSlots(s32 oldBuffer, s32 nextBuffer) {
    if (D_003BD300[0] == oldBuffer) {
        D_003BD300[0] = oldBuffer ^ 1;
    }
    if (D_003BD300[1] == oldBuffer) {
        D_003BD300[1] = oldBuffer ^ 1;
    }
    sdfBusyBufferIndex = nextBuffer;
}

void sdfSetBufferSlot(s32 singleBuffer, s32 value, s32 index) {
    if (singleBuffer == 0) {
        D_003BD300[0] = value;
        D_003BD300[1] = value;
    } else {
        D_003BD300[index] = value;
    }
    sdfBusyBufferIndex = -1;
}

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1380);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D14C8);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1590);

extern vu8 sdfCurrentBufferIndex;
extern void sdfVuClearTransformCache(void);

/* Wait until the other buffer is no longer busy before selecting it. */
void sdfWaitAndSelectBuffer(void) {
    s8 buffer = sdfCurrentBufferIndex ^ 1;

    /* Do not select a buffer while its index is the busy-buffer status. */
    while (sdfBusyBufferIndex == buffer) {
    }
    sdfCurrentBufferIndex = buffer;
    sdfSelectDoubleBuffer((s8)sdfCurrentBufferIndex);
    sdfVuClearTransformCache();
}

s32 sdfFormatBitsPerPixelA(u32 format) {
    switch (format) {
    case 0x0:
    case 0x1:
    case 0x1B:
    case 0x24:
    case 0x2C:
    case 0x30:
    case 0x31:
        return 0x20;
    case 0x2:
    case 0xA:
    case 0x32:
    case 0x3A:
        return 0x10;
    case 0x13:
        return 8;
    case 0x14:
        return 4;
    default:
        return 0;
    }
}

s32 sdfFormatBitsPerPixelB(u32 format) {
    switch (format) {
    case 0x0:
    case 0x30:
        return 0x20;
    case 0x1:
    case 0x31:
        return 0x18;
    case 0x2:
    case 0xA:
    case 0x32:
    case 0x3A:
        return 0x10;
    case 0x13:
    case 0x1B:
        return 8;
    case 0x14:
    case 0x24:
    case 0x2C:
        return 4;
    default:
        return 0;
    }
}

s32 sdfTexListContains(SdfTex *target) {
    SdfTex *node = (SdfTex *)sdfTextureListHead;

    if (node == NULL) {
        return 0;
    }
    do {
        if (node == target) {
            return 1;
        }
        node = node->prev;
    } while (node != NULL);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D17D8);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D18F8);

extern s32 func_00312C08(void);
extern void EIntr(void);
extern struct SdfTexHead *func_002D18F8(s32 size, s32 arg1);

SdfTexHead *sdfTexAllocateHeadForDimensions(s32 width, s32 height, s32 format, s32 allocationMode, s32 useFirstAllocator) {
    s32 bits = sdfFormatBitsPerPixelA(format);
    s32 size = (width * height * bits) >> 5;
    SdfTexHead *node;

    if (useFirstAllocator != 0) {
        node = func_002D17D8(size, allocationMode);
    } else {
        node = func_002D18F8(size, allocationMode);
    }
    node->width = width;
    node->height = height;
    node->format = format;
    return node;
}

s32 sdfCoalesceUnusedTextureBlocks(SdfTexHead *node) {
    SdfTexHead *next = node->prev;

    if (next == NULL || next->unk8 != NULL) {
        return 0;
    }
    node->unk10 += next->unk10;
    if ((node->prev = next->prev) != NULL) {
        next->prev->next = node;
    } else {
        sdfTextureBlockListHead = node;
    }
    sdfReleaseChipBlock((SdfTex *)next);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1B28);

void sdfUpdateTextureHeadsWithInterruptsMasked(void *block) {
    SdfTexHead *node = (SdfTexHead *)block;
    s32 state;

    if (node == NULL) {
        return;
    }
    state = func_00312C08();
    node->unk8 = NULL;
    sdfCoalesceUnusedTextureBlocks(node);
    if (node->next != NULL) {
        if (((SdfTexHead *)node->next)->unk8 == NULL) {
            sdfCoalesceUnusedTextureBlocks((SdfTexHead *)node->next);
        }
    }
    if (state != 0) {
        EIntr();
    }
}

void sdfTexQueuePendingWork(s32 value) {
    sdfPendingQueuePush(&sdfTextureUpdateQueue, value);
}

void sdfTexInitializeLists(void) {
    SdfTexHead *head;

    head = func_002CFEB8(0x1C);
    head->unk10 = 0x100000;
    head->next = NULL;
    head->prev = NULL;
    head->unk8 = NULL;
    head->unkC = NULL;
    sdfTextureListHead = head;
    sdfTextureBlockListHead = head;
    sdfInitializeSynchronizedRequest(&sdfTextureUpdateQueue, sdfUpdateTextureHeadsWithInterruptsMasked);
}

SdfTexHead *sdfAllocImageBuffer(s32 width, s32 height, s32 format) {
    s32 alignedWidth = (width + 0x3F) & -0x40;
    s32 alignedHeight = (height + 0x1F) & -0x20;
    SdfTexHead *node;

    switch (format) {
    case 0x2:
    case 0xA:
    case 0x32:
    case 0x3A:
        alignedWidth >>= 1;
        break;
    case 0x0:
    case 0x1:
    case 0x30:
    case 0x31:
        break;
    }
    node = func_002D17D8((alignedWidth * alignedHeight + 0x7FF) & -0x800, 1);
    node->width = width;
    node->height = height;
    node->format = format;
    return node;
}

SdfTexHead *sdfGetTextureListHead(void) {
    return sdfTextureListHead;
}

SdfTexHead *sdfGetTextureBlockListHead(void) {
    return sdfTextureBlockListHead;
}

s32 sdfFormatImageSize(u32 format, s32 width, s32 height) {
    s32 bits;

    switch (format) {
    case 0x1:
    case 0x31:
        bits = 0x18;
        break;
    case 0x2:
    case 0xA:
    case 0x32:
    case 0x3A:
        bits = 0x10;
        break;
    case 0x13:
    case 0x1B:
        bits = 8;
        break;
    case 0x14:
    case 0x24:
    case 0x2C:
        bits = 4;
        break;
    default:
        bits = 0x20;
        break;
    }
    return (bits * width * height) >> 7;
}

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D1D80);

void sdfTexEnqueuePacketWithSemaphore(s32 address, s32 packet) {
    SdfSemaObj *obj = &sdfTextureQueueWork;
    SdfTexPacketTail *last;

    WaitSema(obj->semaphoreId);
    last = (SdfTexPacketTail *)obj->packetTail;
    if (last != NULL) {
        last->next = 0;
        last->tag = ((u64)(address & 0x0FFFFFFF) << 32) | 0x20000000;
    } else {
        obj->unkC = (void *)address;
    }
    obj->packetTail = (s32)packet;
    SignalSema(obj->semaphoreId);
}

void sdfTexQueueResourceRelease(s32 address) {
    SdfSemaObj *obj = &sdfTextureQueueWork;
    SdfTexReleaseEntry *entry;

    if (address != 0) {
        entry = sdfAllocAndClearQuadwords(0xA0);
        if (sdfChipIsInRange(address) != 0) {
            entry->address = address;
            entry->mode = 2;
        } else {
            entry->mode = 1;
            entry->handle = func_002D0A80(address);
        }
        WaitSema(obj->semaphoreId);
        if (obj->releaseTail != NULL) {
            ((SdfTexReleaseEntry *)obj->releaseTail)->next = entry;
        } else {
            obj->unk4 = entry;
        }
        obj->releaseTail = entry;
        SignalSema(obj->semaphoreId);
    }
}

void sdfResetSemaphoreState(SdfSemaObj *semaphore) {
    semaphore->unk4 = NULL;
    semaphore->releaseTail = NULL;
    semaphore->unkC = NULL;
    semaphore->packetTail = 0;
}

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D2140);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D2168);

void sdfTexInitializeSemaphore(void) {
    SdfSemaObj *obj;

    obj = &sdfTextureQueueWork;
    obj->semaphoreId = sdfCreateSemaphore(1, 0x7F, 0);
    sdfResetSemaphoreState(obj);
}

u32 sdfTexGetPrimaryBuffer(SdfTex *texture) {
    return (u32)texture->primaryBuffer;
}

/* Size in bytes of a packed primary texture buffer: only the low 15 bits
 * contribute to its 16-byte block count. */
s32 sdfTexGetPrimaryBufferSize(SdfTex *tex) {
    SdfTexBuf *buf = tex->primaryBuffer;

    if (buf == NULL) {
        return 0;
    }
    return ((buf->gifTagWord & 0x7FFF) + 1) << 4;
}

s32 sdfTexGetOrInitializeSecondaryBuffer(SdfTex *texture) {
    SdfTexBuf *buf;

    buf = texture->secondaryBuffer;
    if (buf == NULL) {
        sdfTexCreateSecondPacket();
        buf = texture->secondaryBuffer;
    }
    return (s32)buf;
}

/* Mirror the primary-buffer size calculation for the secondary buffer. */
s32 sdfTexGetSecondaryBufferSize(SdfTex *tex) {
    SdfTexBuf *buf = tex->secondaryBuffer;

    if (buf == NULL) {
        return 0;
    }
    return ((buf->gifTagWord & 0x7FFF) + 1) << 4;
}

u8 func_002D2390(SdfTex *texture) {
    return texture->unk18;
}

u32 sdfTexGetSecondaryResourceWord(SdfTex *texture) {
    u32 word;

    word = 0;
    if (texture->secondaryResource != NULL) {
        word = texture->secondaryResource->word;
    }
    return word;
}

u32 sdfTexGetPrimaryResourceWord(SdfTex *texture) {
    return texture->primaryResource->word;
}

s32 sdfFormatBitsPerPixelC(u32 format) {
    switch (format) {
    case 0:
        return 0x20;
    case 1:
        return 0x18;
    case 2:
    case 10:
        return 0x10;
    case 19:
    case 27:
        return 8;
    default:
        return 4;
    }
}

s32 func_002D2410(s32 format) {
    s32 size;

    switch (format) {
    case 19:
        size = 8;
        break;
    case 20:
        size = 4;
        break;
    case 2:
    case 10:
        size = 16;
        break;
    default:
        size = 32;
        break;
    }
    return size;
}

u64 func_002D2468(SdfTex *texture) {
    return texture->primaryBuffer->textureState;
}

u64 func_002D2478(SdfTex *texture) {
    return texture->primaryBuffer->samplingState;
}

u64 func_002D2488(SdfTex *texture) {
    return texture->primaryBuffer->clampState;
}

void sdfTexSetPrimaryBufferModeBits(SdfTex *texture, s32 magFilter, s32 minFilter) {
    SdfTexBuf *buf;

    buf = texture->primaryBuffer;
    buf->samplingState = (buf->samplingState & ~0x1E0) | (magFilter << 5) | (minFilter << 6);
}

void sdfTexSetSecondaryPacketBits(SdfTex *tex, s32 magFilter, s32 minFilter) {
    SdfTexBuf *buf = tex->secondaryBuffer;

    if (buf == NULL) {
        sdfTexCreateSecondPacket();
        buf = tex->secondaryBuffer;
    }
    buf->samplingState = (buf->samplingState & ~0x1E0) | (magFilter << 5) | (minFilter << 6);
}

void func_002D2530(SdfTex *texture, u8 value) {
    texture->clampMode = value;
    func_002D2FB0();
}

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D2548);

extern void func_002D2548();

void sdfTexSubmitPixelsForFormat(SdfTex *texture, s32 resourceWord, u8 *pixels, s32 mode) {
    s32 width;
    s32 height;

    if (texture->pixelFormat == 0x13 || texture->pixelFormat == 0x1B) {
        width = 0x10;
        height = 0x10;
    } else {
        width = 8;
        height = 2;
    }
    func_002D2548(resourceWord, width, height, texture->clutFormat, pixels, mode);
}

void sdfTexUploadSecondaryResource(SdfTex *tex) {
    if (tex->secondaryResource != NULL) {
        sdfTexSubmitPixelsForFormat(tex, sdfTexGetSecondaryResourceWord(tex), tex->data, 0);
    }
}

void sdfTexListInsert(SdfTex *texture) {
    texture->next = NULL;
    if (sdfResourceListHead != NULL) {
        texture->prev = sdfResourceListHead;
        sdfResourceListHead->next = texture;
    } else {
        texture->prev = NULL;
    }
    sdfResourceListHead = texture;
}

SdfTex *sdfTexCreateResourceWithReference(s32 x, s32 y, s32 pixelFormat, s32 maxMipLevel, s32 primary, s32 paletteFormat, s32 arg6, s32 secondary) {
    SdfTex *tex = sdfAllocAndClearQuadwords(0x40);
    SdfTexRef *ref = sdfAllocAndClearQuadwords(8);

    ref->refCount = 1;
    tex->unk18 = arg6;
    tex->clutFormat = paletteFormat;
    tex->width = x;
    tex->height = y;
    tex->pixelFormat = pixelFormat;
    tex->maxMipLevel = maxMipLevel;
    tex->secondaryResource = (SdfTexResource *)secondary;
    tex->primaryResource = (SdfTexResource *)primary;
    tex->reference = ref;
    sdfTexListInsert(tex);
    tex->unk38 = 0x80808080;
    return tex;
}

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D2800);

INCLUDE_ASM(const s32, "game/code_002D10B0", func_002D2950);

INCLUDE_SDATA(const s32, "game/code_002D10B0", D_003BD2F0);

INCLUDE_SDATA(const s32, "game/code_002D10B0", D_003BD2F1);

INCLUDE_SDATA(const s32, "game/code_002D10B0", D_003BD2F4);

INCLUDE_SDATA(const s32, "game/code_002D10B0", D_003BD2F8);

INCLUDE_SDATA(const s32, "game/code_002D10B0", D_003BD2FC);

INCLUDE_SDATA(const s32, "game/code_002D10B0", D_003BD300);

INCLUDE_SDATA(const s32, "game/code_002D10B0", sdfBusyBufferIndex);

INCLUDE_SDATA(const s32, "game/code_002D10B0", D_003BD304);

INCLUDE_SDATA(const s32, "game/code_002D10B0", sdfResourceListHead);

