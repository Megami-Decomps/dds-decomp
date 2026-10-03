#include "common.h"

#include "sdf.h"

/* GS pixel-storage modes, using the same private names as sdfTex.c. */
enum {
    SDF_PSMCT32 = 0x0, SDF_PSMCT24 = 0x1, SDF_PSMCT16 = 0x2,
    SDF_PSMCT16S = 0xA, SDF_PSMT8 = 0x13, SDF_PSMT4 = 0x14,
    SDF_PSMT8H = 0x1B, SDF_PSMT4HL = 0x24, SDF_PSMT4HH = 0x2C,
    SDF_PSMZ32 = 0x30, SDF_PSMZ24 = 0x31, SDF_PSMZ16 = 0x32,
    SDF_PSMZ16S = 0x3A
};

#define SDF_NO_BUSY_BUFFER -1
#define SDF_GIF_LOOP_COUNT_MASK 0x7FFF
#define SDF_QWORD_BYTE_SHIFT 4
#define SDF_TEX_FILTER_MASK 0x1E0
#define SDF_MAG_FILTER_SHIFT 5
#define SDF_MIN_FILTER_SHIFT 6
#define SDF_UPLOAD_CHIP_MAX_BYTES 0x400

enum {
    SDF_UPLOAD_BORROWED = 0,
    SDF_UPLOAD_GENERAL_HEAP = 1,
    SDF_UPLOAD_CHIP_HEAP = 2
};

typedef struct SdfImageUploadRequest {
    void *pixels;
    s32 allocation;
    u8 allocationMode;
    u8 format;
    u16 bufferWidth;
    u32 destination;
    u16 x;
    u16 y;
    u16 width;
    u16 height;
} SdfImageUploadRequest;

extern s32 sdfAllocGeneralBlock(s32);
extern void *sdfResourceRetainAddress(s32);
extern void func_0032AC30(SdfImageUploadRequest *);


typedef struct SdfTexPacketTail {
    u64 tag;      /* 0x00 */
    u64 next;     /* 0x08 */
} SdfTexPacketTail;

typedef struct SdfTexHead {
    struct SdfTexHead *next; /* 0x00 */
    struct SdfTexHead *prev; /* 0x04 */
    s32 allocationMode;     /* 0x08: zero marks a free range */
    u32 address;            /* 0x0C: VRAM offset in 32-bit words */
    s32 size;               /* 0x10: range length in 32-bit words */
    s16 width;              /* 0x14 */
    s16 height;             /* 0x16 */
    s32 format;             /* 0x18 */
} SdfTexHead;

extern void sdfReleaseChipBlock();
s32 sdfCoalesceUnusedTextureBlocks(SdfTexHead *block);

typedef struct SdfTexReleaseEntry {
    struct SdfTexReleaseEntry *next; /* 0x00 */
    s32 address;                     /* 0x04 */
    s32 handle;                      /* 0x08 */
    u8 mode;                         /* 0x0C: 1 = handle, 2 = chip memory address */
    u8 pad0D[0x93];
} SdfTexReleaseEntry; /* 0xA0 */

extern s32 sdfChipIsInRange();
extern s32 sdfFindGeneralBlockByAddress();


extern u8 D_004389E0;

extern u32 D_004389E4;

extern u32 D_004389E8;

extern SdfTex *sdfResourceListHead;

extern u8 sdfTextureUpdateQueue;

void sdfPendingQueuePush(void *request, s32 value);


extern SdfTexHead *sdfTextureBlockListHead;

extern SdfTexHead *sdfTextureListHead;

void *sdfAllocSizeClassBlock(s32 size);
extern void (*D_004389F4)(s32 size, s32 allocationMode);
extern s32 func_0036DE70(void);

s32 sdfUpdateTextureHeadsWithInterruptsMasked(SdfTexHead *block);

void sdfInitializeSynchronizedRequest(void *request, void (*onComplete)(void *));

extern SdfTexHead *func_0032A688(s32 size, s32 allocationMode);

extern SdfTexHead *func_0032A7A8(s32 size, s32 allocationMode);

extern SdfSemaObj sdfTextureQueueWork;

s32 sdfCreateSemaphore(s32 arg0, s32 arg1, s32 arg2);

extern s8 sdfBufferSlotIndices[2];

extern volatile s8 sdfBusyBufferIndex;

extern vu8 sdfCurrentBufferIndex;

extern void sdfVuClearTransformCache(void);

extern u8 *sdfTexSubmitImageCopy();


void sdfRequestDeferredGsImageCapture(u32 destination, u32 onComplete) {
    D_004389E4 = destination;
    D_004389E8 = onComplete;
    D_004389E0 = 1;
}

INCLUDE_ASM(const s32, "game/code_00329F60", func_00329F78);

/* Switch both references off the finished double-buffer slot before
 * publishing the slot currently in use. */
void sdfSwapBufferSlots(s32 oldBuffer, s32 nextBuffer) {
    if (sdfBufferSlotIndices[0] == oldBuffer) {
        sdfBufferSlotIndices[0] = oldBuffer ^ 1;
    }
    if (sdfBufferSlotIndices[1] == oldBuffer) {
        sdfBufferSlotIndices[1] = oldBuffer ^ 1;
    }
    sdfBusyBufferIndex = nextBuffer;
}

/* Update one slot or both slots, then clear the busy-buffer status. */
void sdfSetBufferSlot(s32 updateSingleSlot, s32 bufferIndex, s32 slotIndex) {
    if (updateSingleSlot == 0) {
        sdfBufferSlotIndices[0] = bufferIndex;
        sdfBufferSlotIndices[1] = bufferIndex;
    } else {
        sdfBufferSlotIndices[slotIndex] = bufferIndex;
    }
    sdfBusyBufferIndex = SDF_NO_BUSY_BUFFER;
}

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032A230);

extern u8 D_004389D1;
extern volatile u8 D_004389E1;
extern s32 D_00439138;
extern s32 D_004389EC;
extern volatile u8 sdfPacketSlotIndex;
extern void sdfSleepThreadCount(s32);
extern void sdfDevSignalPendingSemaphore(void);
extern s32 WakeupThread(s32);
extern s32 WaitSema(s32);
extern void sdfGraphSelectDisplayBuffer(s32);

void func_0032A378(void) {
    s8 bufferIndex;
    s32 idleFlag;

    for (;;) {
        sdfSleepThreadCount(0);
        if ((D_004389D1 & 1) == 0) {
            sdfDevSignalPendingSemaphore();
        }
        idleFlag = (~*(volatile u64 *)0x12001000 >> 13) & 1;
        D_004389D1 = idleFlag ^ 1;
        if (D_004389E1 == 1) {
            D_004389E1 = 2;
            WakeupThread(D_00439138);
            WaitSema(D_004389EC);
        }
        bufferIndex = sdfBufferSlotIndices[D_004389D1];
        if (bufferIndex >= 0) {
            sdfGraphSelectDisplayBuffer(bufferIndex);
        }
        if (sdfPacketSlotIndex != 0) {
            sdfPacketSlotIndex += 0xFF;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032A440);

/* Wait until the other buffer is no longer busy before selecting it. */
void sdfWaitAndSelectBuffer(void) {
    s8 nextBuffer = sdfCurrentBufferIndex ^ 1;

    /* Do not select a buffer while its index is the busy-buffer status. */
    while (sdfBusyBufferIndex == nextBuffer) {
    }
    sdfCurrentBufferIndex = nextBuffer;
    sdfSelectDoubleBuffer((s8)sdfCurrentBufferIndex);
    sdfVuClearTransformCache();
}

/* Storage depth for supported formats: listed 24-bit/high-plane modes use 32 bits; unlisted is zero. */
s32 sdfFormatBitsPerPixelA(u32 format) {
    switch (format) {
    case SDF_PSMCT32:
    case SDF_PSMCT24:
    case SDF_PSMT8H:
    case SDF_PSMT4HL:
    case SDF_PSMT4HH:
    case SDF_PSMZ32:
    case SDF_PSMZ24:
        return 0x20;
    case SDF_PSMCT16:
    case SDF_PSMCT16S:
    case SDF_PSMZ16:
    case SDF_PSMZ16S:
        return 0x10;
    case SDF_PSMT8:
        return 8;
    case SDF_PSMT4:
        return 4;
    default:
        return 0;
    }
}

/* Transfer pixel depth, including indexed high-plane and depth formats; unlisted values return zero. */
s32 sdfFormatBitsPerPixelB(u32 format) {
    switch (format) {
    case SDF_PSMCT32:
    case SDF_PSMZ32:
        return 0x20;
    case SDF_PSMCT24:
    case SDF_PSMZ24:
        return 0x18;
    case SDF_PSMCT16:
    case SDF_PSMCT16S:
    case SDF_PSMZ16:
    case SDF_PSMZ16S:
        return 0x10;
    case SDF_PSMT8:
    case SDF_PSMT8H:
        return 8;
    case SDF_PSMT4:
    case SDF_PSMT4HL:
    case SDF_PSMT4HH:
        return 4;
    default:
        return 0;
    }
}

s32 sdfTexListContains(SdfTex *target) {
    SdfTexHead *node = sdfTextureListHead;

    if (node == NULL) {
        return 0;
    }
    do {
        if (node == (void *)target) {
            return 1;
        }
        node = node->prev;
    } while (node != NULL);
    return 0;
}

SdfTexHead *func_0032A688(s32 size, s32 allocationMode) {
    SdfTexHead *block;
    SdfTexHead *allocated;
    s32 interruptsEnabled;

    interruptsEnabled = func_0036DE70();
    for (block = sdfTextureListHead;; block = block->prev) {
        if (block == NULL) {
            if (interruptsEnabled != 0) {
                EIntr();
            }
            if (D_004389F4 != NULL) {
                D_004389F4(size, allocationMode);
            }
        }
        if (block->allocationMode != 0 || block->size < size) {
            continue;
        }
        if (block->size == size) {
            block->allocationMode = allocationMode;
            if (interruptsEnabled != 0) {
                EIntr();
            }
            return block;
        }
        allocated = sdfAllocSizeClassBlock(sizeof(*allocated));
        allocated->next = block->next;
        if (allocated->next == NULL) {
            sdfTextureListHead = allocated;
        } else {
            block->next->prev = allocated;
        }
        allocated->allocationMode = allocationMode;
        block->next = allocated;
        allocated->address = block->address;
        allocated->size = size;
        allocated->prev = block;
        block->address += size;
        block->size -= size;
        if (interruptsEnabled != 0) {
            EIntr();
        }
        return allocated;
    }
}

SdfTexHead *func_0032A7A8(s32 size, s32 allocationMode) {
    SdfTexHead *block;
    SdfTexHead *allocated;
    s32 interruptsEnabled;

    interruptsEnabled = func_0036DE70();
    for (block = sdfTextureBlockListHead;; block = block->next) {
        if (block == NULL) {
            if (interruptsEnabled != 0) {
                EIntr();
            }
            if (D_004389F4 != NULL) {
                D_004389F4(size, allocationMode);
            }
        }
        if (block->allocationMode != 0 || block->size < size) {
            continue;
        }
        if (block->size == size) {
            block->allocationMode = allocationMode;
            if (interruptsEnabled != 0) {
                EIntr();
            }
            return block;
        }
        allocated = sdfAllocSizeClassBlock(sizeof(*allocated));
        allocated->prev = block->prev;
        if (allocated->prev == NULL) {
            sdfTextureBlockListHead = allocated;
        } else {
            block->prev->next = allocated;
        }
        allocated->allocationMode = allocationMode;
        block->prev = allocated;
        allocated->address = block->address + (block->size - size);
        allocated->size = size;
        allocated->next = block;
        block->size -= size;
        if (interruptsEnabled != 0) {
            EIntr();
        }
        return allocated;
    }
}

/* Convert storage bits to 32-bit VRAM words before selecting an allocation direction. */
SdfTexHead *sdfTexAllocateHeadForDimensions(s32 width, s32 height, s32 format, s32 allocationMode, s32 useFirstAllocator) {
    s32 storageBitsPerPixel = sdfFormatBitsPerPixelA(format);
    s32 sizeWords = (width * height * storageBitsPerPixel) >> 5;
    SdfTexHead *textureBlock;

    if (useFirstAllocator != 0) {
        textureBlock = func_0032A688(sizeWords, allocationMode);
    } else {
        textureBlock = func_0032A7A8(sizeWords, allocationMode);
    }
    textureBlock->width = width;
    textureBlock->height = height;
    textureBlock->format = format;
    return textureBlock;
}

/* Merge the previous free range into block; return one only when a record was recycled. */
s32 sdfCoalesceUnusedTextureBlocks(SdfTexHead *block) {
    SdfTexHead *previousBlock = block->prev;

    if (previousBlock != NULL) {
        if (previousBlock->allocationMode == 0) {
            SdfTexHead *beforePreviousBlock = previousBlock->prev;

            block->size = block->size + previousBlock->size;
            block->prev = beforePreviousBlock;
            if (beforePreviousBlock != NULL) {
                previousBlock->prev->next = block;
            } else {
                sdfTextureBlockListHead = block;
            }
            sdfReleaseChipBlock(previousBlock, block, previousBlock);
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032A9D8);



/* Mark a range free and coalesce its neighbors while interrupts are masked. */
s32 sdfUpdateTextureHeadsWithInterruptsMasked(SdfTexHead *textureBlock) {
    s32 restoreInterrupts;
    SdfTexHead *nextBlock;

    if (textureBlock != NULL) {
        restoreInterrupts = func_0036DE70();
        textureBlock->allocationMode = 0;
        sdfCoalesceUnusedTextureBlocks(textureBlock);
        nextBlock = textureBlock->next;
        if (nextBlock != NULL && nextBlock->allocationMode == 0) {
            sdfCoalesceUnusedTextureBlocks(nextBlock);
        }
        if (restoreInterrupts != 0) {
            EIntr();
        }
    }
}

void sdfTexQueuePendingWork(s32 value) {
    sdfPendingQueuePush(&sdfTextureUpdateQueue, value);
}

void sdfTexInitializeLists(void) {
    SdfTexHead *head;

    head = sdfAllocSizeClassBlock(0x1C);
    head->size = 0x100000;
    head->next = NULL;
    head->prev = NULL;
    head->allocationMode = 0;
    head->address = 0;
    sdfTextureListHead = head;
    sdfTextureBlockListHead = head;
    sdfInitializeSynchronizedRequest(&sdfTextureUpdateQueue, sdfUpdateTextureHeadsWithInterruptsMasked);
}

/* Align the storage rows, pack 16-bit formats two pixels per word, then allocate whole GS pages. */
SdfTexHead *sdfAllocImageBuffer(s32 width, s32 height, s32 format) {
    s32 rowWidth = (width + 0x3F) & -0x40;
    s32 rowCount = (height + 0x1F) & -0x20;
    SdfTexHead *textureBlock;

    switch (format) {
    case SDF_PSMCT16:
    case SDF_PSMCT16S:
    case SDF_PSMZ16:
    case SDF_PSMZ16S:
        rowWidth >>= 1;
        break;
    case SDF_PSMCT32:
    case SDF_PSMCT24:
    case SDF_PSMZ32:
    case SDF_PSMZ24:
        break;
    }
    textureBlock = func_0032A688((rowWidth * rowCount + 0x7FF) & -0x800, 1);
    textureBlock->width = width;
    textureBlock->height = height;
    textureBlock->format = format;
    return textureBlock;
}

u32 sdfGetTextureListHead(void) {
    return sdfTextureListHead;
}

u32 sdfGetTextureBlockListHead(void) {
    return sdfTextureBlockListHead;
}

/* Transfer size in 16-byte quadwords; low bits of a partial quadword are discarded. */
s32 sdfFormatImageSize(u32 format, s32 width, s32 height) {
    s32 transferBitsPerPixel;

    switch (format) {
    case SDF_PSMCT24:
    case SDF_PSMZ24:
        transferBitsPerPixel = 0x18;
        break;
    case SDF_PSMCT16:
    case SDF_PSMCT16S:
    case SDF_PSMZ16:
    case SDF_PSMZ16S:
        transferBitsPerPixel = 0x10;
        break;
    case SDF_PSMT8:
    case SDF_PSMT8H:
        transferBitsPerPixel = 8;
        break;
    case SDF_PSMT4:
    case SDF_PSMT4HL:
    case SDF_PSMT4HH:
        transferBitsPerPixel = 4;
        break;
    default:
        transferBitsPerPixel = 0x20;
        break;
    }
    return (transferBitsPerPixel * width * height) >> 7;
}

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032AC30);

void sdfTexEnqueuePacketWithSemaphore(s32 address, void *packet) {
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
            entry->handle = sdfFindGeneralBlockByAddress(address);
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
INCLUDE_ASM(const s32, "game/code_00329F60", func_0032AFF0);

extern s32 GetThreadId(void);
extern s32 func_0032AFF0(s32 threadId);
extern s32 func_003659C0(s32 channel, s32 (*handler)(s32), s32 arg, s32 threadId);
extern void *sceDmaGetChan(s32 channel);
extern void FlushCache(s32 mode);
extern void sceDmaSend(void *channel, void *packet);
extern void SleepThread(void);
extern s32 RemoveDmacHandler(s32 channel, s32 handlerId);
extern u8 D_0040B2B0[];

void func_0032B018(void) {
    SdfSemaObj *work = &sdfTextureQueueWork;
    SdfTexReleaseEntry *entry;
    SdfTexPacketTail *packet;
    void *dmaPacket;

    WaitSema(work->semaphoreId);
    entry = (SdfTexReleaseEntry *)work->unk4;
    dmaPacket = work->unkC;
    packet = (SdfTexPacketTail *)work->packetTail;
    sdfResetSemaphoreState(work);
    SignalSema(work->semaphoreId);

    if (dmaPacket != NULL) {
        s32 threadId = GetThreadId();
        s32 handlerId = func_003659C0(1, func_0032AFF0, 0, threadId);
        u64 tag = ((u64)((u32)D_0040B2B0 & 0x0FFFFFFF) << 32) | 0x20000000;
        u32 *channel;

        packet->next = 0;
        packet->tag = tag;
        channel = (u32 *)sceDmaGetChan(1);
        channel[0] |= 0x40;
        FlushCache(0);
        sceDmaSend((void *)channel, dmaPacket);
        SleepThread();
        RemoveDmacHandler(1, handlerId);
    }

    while (entry != NULL) {
        SdfTexReleaseEntry *next = entry->next;

        switch (entry->mode) {
        case 1:
            sdfReleaseResourceAllocation(entry->handle);
            break;
        case 2:
            sdfReleaseChipBlock((void *)entry->address);
            break;
        }
        sdfReleaseChipBlock(entry);
        entry = next;
    }
}

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
s32 sdfTexGetPrimaryBufferSize(SdfTex *texture) {
    SdfTexBuf *buffer = texture->primaryBuffer;

    if (buffer == NULL) {
        return 0;
    }
    return ((buffer->gifTagWord & SDF_GIF_LOOP_COUNT_MASK) + 1) << SDF_QWORD_BYTE_SHIFT;
}

s32 sdfTexGetOrInitializeSecondaryBuffer(SdfTex *texture) {
    SdfTexBuf *buffer;

    buffer = texture->secondaryBuffer;
    if (buffer == NULL) {
        sdfTexCreateSecondPacket();
        buffer = texture->secondaryBuffer;
    }
    return (s32)buffer;
}

/* Mirror the primary-buffer size calculation for the secondary buffer. */
s32 sdfTexGetSecondaryBufferSize(SdfTex *texture) {
    SdfTexBuf *buffer = texture->secondaryBuffer;

    if (buffer == NULL) {
        return 0;
    }
    return ((buffer->gifTagWord & SDF_GIF_LOOP_COUNT_MASK) + 1) << SDF_QWORD_BYTE_SHIFT;
}

u8 func_0032B240(SdfTex *texture) {
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

/* Color-format pixel depth; every unlisted format falls through to four bits. */
s32 sdfFormatBitsPerPixelC(u32 format) {
    switch (format) {
    case SDF_PSMCT32:
        return 0x20;
    case SDF_PSMCT24:
        return 0x18;
    case SDF_PSMCT16:
    case SDF_PSMCT16S:
        return 0x10;
    case SDF_PSMT8:
    case SDF_PSMT8H:
        return 8;
    default:
        return 4;
    }
}

/* Storage-depth subset for indexed and 16-bit color formats; all other values return 32. */
s32 sdfTexGetStorageBitsPerPixel(s32 format) {
    s32 storageBitsPerPixel;

    switch (format) {
    case SDF_PSMT8:
        storageBitsPerPixel = 8;
        break;
    case SDF_PSMT4:
        storageBitsPerPixel = 4;
        break;
    case SDF_PSMCT16:
    case SDF_PSMCT16S:
        storageBitsPerPixel = 16;
        break;
    default:
        storageBitsPerPixel = 32;
        break;
    }
    return storageBitsPerPixel;
}

u64 sdfTexGetPrimaryTextureState(SdfTex *texture) {
    return texture->primaryBuffer->textureState;
}

u64 sdfTexGetPrimarySamplingState(SdfTex *texture) {
    return texture->primaryBuffer->samplingState;
}

u64 sdfTexGetPrimaryClampState(SdfTex *texture) {
    return texture->primaryBuffer->clampState;
}

/* Clear TEX1 MMAG/MMIN and OR in the supplied filters; input values are not masked. */
void sdfTexSetPrimaryBufferModeBits(SdfTex *texture, s32 magFilter, s32 minFilter) {
    SdfTexBuf *buffer;

    buffer = texture->primaryBuffer;
    buffer->samplingState = (buffer->samplingState & ~SDF_TEX_FILTER_MASK) | (magFilter << SDF_MAG_FILTER_SHIFT) | (minFilter << SDF_MIN_FILTER_SHIFT);
}

/* Create the secondary packet if absent, then update TEX1 filters without masking input values. */
void sdfTexSetSecondaryPacketBits(SdfTex *texture, s32 magFilter, s32 minFilter) {
    SdfTexBuf *buffer = texture->secondaryBuffer;

    if (buffer == NULL) {
        sdfTexCreateSecondPacket();
        buffer = texture->secondaryBuffer;
    }
    buffer->samplingState = (minFilter << SDF_MIN_FILTER_SHIFT) | ((magFilter << SDF_MAG_FILTER_SHIFT) | (buffer->samplingState & ~SDF_TEX_FILTER_MASK));
}

void sdfTexSetClampMode(SdfTex *texture, u8 value) {
    texture->clampMode = value;
    sdfTexRefreshResourcePackets();
}

/* Borrow pixels or copy them to the selected heap; return the source cursor after the transfer bytes. */
u8 *sdfTexSubmitImageCopy(u32 destination, s32 width, s32 height, u32 format, u8 *pixels, s32 borrowPixels) {
    SdfImageUploadRequest request;
    s32 imageBytes = sdfFormatImageSize(format, width, height) * 16;

    if (borrowPixels == 0) {
        if (imageBytes > SDF_UPLOAD_CHIP_MAX_BYTES) {
            request.allocation = sdfAllocGeneralBlock(imageBytes);
            request.pixels = sdfResourceRetainAddress(request.allocation);
            request.allocationMode = SDF_UPLOAD_GENERAL_HEAP;
        } else {
            request.pixels = sdfAllocSizeClassBlock(imageBytes);
            request.allocationMode = SDF_UPLOAD_CHIP_HEAP;
        }
        memcpy(request.pixels, pixels, imageBytes);
    } else {
        request.pixels = pixels;
        request.allocationMode = SDF_UPLOAD_BORROWED;
    }
    request.format = format;
    request.destination = destination;
    request.bufferWidth = width;
    request.width = width;
    request.height = height;
    request.x = 0;
    request.y = 0;
    func_0032AC30(&request);
    return pixels + imageBytes;
}

/* Upload the CLUT as 16x16 for 8-bit indexed formats, otherwise 8x2; forward pixel ownership. */
u8 *sdfTexSubmitPixelsForFormat(SdfTex *texture, s32 destination, u8 *pixels, s32 borrowPixels) {
    s32 paletteWidth;
    s32 paletteHeight;

    if (texture->pixelFormat == SDF_PSMT8 || texture->pixelFormat == SDF_PSMT8H) {
        paletteWidth = 0x10;
        paletteHeight = 0x10;
    } else {
        paletteWidth = 8;
        paletteHeight = 2;
    }
    return sdfTexSubmitImageCopy(destination, paletteWidth, paletteHeight, texture->clutFormat, pixels, borrowPixels);
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

extern void *sdfAllocAndClearQuadwords(s32 size);
extern void func_0032B908(SdfTex *texture);
extern void sdfTexCopyImageData(SdfTex *texture, void *source);
extern void sdfTexCreateFirstPacket(SdfTex *texture);

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

extern SdfTexHead *func_0032B800(s32, s32, s32);
extern void func_0032B908(SdfTex *);
extern void sdfTexCopyImageData(SdfTex *, void *);
extern void sdfTexCreateFirstPacket(SdfTex *);
SdfTex *func_0032B6B0(SdfTex *source) {
    SdfTex *texture;
    SdfTexRef *reference;
    SdfTex *original;

    texture = sdfAllocSizeClassBlock(sizeof(*texture));
    *texture = *source;
    reference = sdfAllocAndClearQuadwords(sizeof(*reference));
    original = reference->unk0;
    reference->refCount = 1;
    texture->reference = reference;
    if (original == NULL) {
        original = source;
        reference->unk0 = original;
    }
    texture->primaryBuffer = NULL;
    texture->secondaryBuffer = NULL;
    texture->data = NULL;
    texture->auxiliaryAllocation = NULL;
    sdfTexListInsert(texture);
    if (texture->secondaryResource != NULL) {
        texture->secondaryResource = (SdfTexResource *)func_0032B800(
            texture->pixelFormat, texture->clutFormat, texture->unk18);
        func_0032B908(texture);
        sdfTexCopyImageData(texture, original->data);
        texture->unk38 = 0x80808080;
    }
    sdfTexCreateFirstPacket(texture);
    sdfTexUploadSecondaryResource(texture);
    return texture;
}

/* Allocate a texture head for a 0x20-byte (kind 0) or 0x10-byte (kind 2/10) unit; types 19/27 use unit*8 bytes and 0x20 rows, 20/36/44 use 0x40 bytes and 8 rows. */
SdfTexHead *func_0032B800(s32 type, s32 kind, s32 unused) {
    SdfTexHead *head = NULL;
    s32 unit;
    s32 size;
    s16 height;

    switch (kind) {
    case 0:
        unit = 0x20;
        break;
    case 2:
    case 10:
        unit = 0x10;
        break;
    default:
        return NULL;
    }
    switch (type) {
    case 19:
    case 27:
        size = unit * 8;
        height = 0x20;
        break;
    case 20:
    case 36:
    case 44:
        size = 0x40;
        height = 8;
        break;
    default:
        return head;
    }
    head = func_0032A7A8(size, 3);
    head->width = 8;
    head->height = height;
    head->format = kind;
    return head;
}

INCLUDE_SDATA(const s32, "game/code_00329F60", D_004389E0);

INCLUDE_SDATA(const s32, "game/code_00329F60", D_004389E1);

INCLUDE_SDATA(const s32, "game/code_00329F60", D_004389E4);

INCLUDE_SDATA(const s32, "game/code_00329F60", D_004389E8);

INCLUDE_SDATA(const s32, "game/code_00329F60", D_004389EC);

INCLUDE_SDATA(const s32, "game/code_00329F60", sdfBufferSlotIndices);

INCLUDE_SDATA(const s32, "game/code_00329F60", sdfBusyBufferIndex);

INCLUDE_SDATA(const s32, "game/code_00329F60", D_004389F4);

INCLUDE_SDATA(const s32, "game/code_00329F60", sdfResourceListHead);

