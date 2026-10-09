#include "common.h"
#include "sdf_texture_queue.h"
#include "sdf_image_upload.h"
#include "sdf_thread.h"
#include "ee_mmi.h"
extern s32 iWakeupThread(s32 threadId);
#include "sdf_chip.h"
#include "sdf_resource.h"
#include "sdf.h"
#include "sdf_pending.h"
#include "sdf_texture_release.h"

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





extern void func_002D1D80(SdfImageUploadRequest *);





extern SdfTexResource *sdfTextureBlockListHead;
extern SdfTexResource *sdfTextureListHead;
extern s8 sdfBufferSlotIndices[2];

/* Busy-buffer index is published by the slot setters and polled below;
 * volatile prevents the wait loop from reusing an earlier read. */
extern volatile s8 sdfBusyBufferIndex;
extern SdfTex *sdfResourceListHead;
extern SdfPendingRequest sdfTextureUpdateQueue;
extern SdfTextureQueue sdfTextureQueueWork;
extern u8 D_003BD2F0;
extern u8 *D_003BD2F4;
extern void (*D_003BD2F8)(void *);

extern s32 func_00312C08(void);
extern void EIntr(void);
extern void (*D_003BD304)(s32 size, s32 allocationMode);
s32 sdfCreateSemaphore(s32 arg0, s32 arg1, s32 arg2);
struct SdfTexResource *sdfTexAllocHeadLow(s32 size, s32 arg1);
void sdfUpdateTextureHeadsWithInterruptsMasked(SdfTexResource *textureBlock);
void sdfTexCreateSecondPacket(SdfTex *texture);
void sdfTexRefreshResourcePackets(SdfTex *texture);

void sdfRequestDeferredGsImageCapture(u8 *destination, void (*onComplete)(void *)) {
    D_003BD2F4 = destination;
    D_003BD2F8 = onComplete;
    D_003BD2F0 = 1;
}

typedef struct sceGsStoreImage sceGsStoreImage;
extern sceGsStoreImage D_003E27B0;
extern SdfGraphObj D_003980E0;
extern volatile u8 D_003BD2F1;
extern s32 sceGsSetDefStoreImage(sceGsStoreImage *, s16, s16, s16, s16, s16, s16, s16);
extern s32 sceGsExecStoreImage(sceGsStoreImage *, void *);
extern s32 sceGsSyncPath(s32, s32);

/* Interleave rows from both color buffers into the queued destination. */
void sdfCaptureDeferredGsImage(void) {
    s32 width;
    s32 height;
    s32 format;
    s32 pixelBytes;
    s32 rowBytes;
    s32 stride;
    SdfMemBlock *allocation;
    s32 rows;
    u8 *pixels;
    u8 *source;
    u8 *destination;

    if (D_003BD2F0) {
        width = D_003980E0.width;
        height = D_003980E0.height;
        format = D_003980E0.bufferFormat;
        pixelBytes = 4;
        if (format == SDF_PSMCT16) {
            pixelBytes = 2;
        }
        rowBytes = pixelBytes * width;
        stride = rowBytes * 2;
        D_003BD2F1 = 5;
        allocation = sdfAllocGeneralBlock(rowBytes * height);
        pixels = (u8 *)sdfResourceRetainAddress(allocation);
        sceGsSetDefStoreImage(
            (sceGsStoreImage *)(((u32)&D_003E27B0 & 0x0FFFFFFF) | 0x20000000),
            D_003980E0.buffers[1]->word >> 6,
            width / 64, format, 0, 0, width, height);
        sceGsExecStoreImage(&D_003E27B0, pixels);
        sceGsSyncPath(0, 0);
        destination = D_003BD2F4;
        source = pixels;
        rows = height;
        do {
            memcpy(destination, source, rowBytes);
            destination += stride;
            source += rowBytes;
        } while (--rows != 0);
        sceGsSetDefStoreImage(
            (sceGsStoreImage *)(((u32)&D_003E27B0 & 0x0FFFFFFF) | 0x20000000),
            D_003980E0.buffers[0]->word >> 6,
            width / 64, format, 0, 0, width, height);
        sceGsExecStoreImage(&D_003E27B0, pixels);
        sceGsSyncPath(0, 0);
        destination = D_003BD2F4 + rowBytes;
        source = pixels;
        rows = height;
        do {
            memcpy(destination, source, rowBytes);
            destination += stride;
            source += rowBytes;
        } while (--rows != 0);
        sdfReleaseResourceAllocation(allocation);
        D_003BD2F0 = 0;
        if (D_003BD2F8) {
            D_003BD2F8(D_003BD2F4);
        }
    }
}

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

extern s8 D_003BD9DC;
extern s8 D_003BD9DD;
extern volatile s32 sdfGsImageUploadSemaphore;
extern volatile u8 D_003BD32D;
extern volatile u8 D_003BD32C;
extern u16 D_003BD32E;
extern s32 sdfAddHandler(s32, s32, s32 (*)(s32), s32, s32);
extern s32 func_0030B638(s32);
extern s32 GetThreadId(void);
extern s32 CancelWakeupThread(s32);
extern s32 SleepThread(void);
extern s32 WaitSema(s32);
extern s32 SignalSema(s32);
extern u32 func_002CF930(void);
extern u32 sdfGetElapsedTimerTicks(u32);
extern void sdfGraphRecreateBuffers(SdfGraphObj *);
extern void sdfGraphResetDeviceForCurrentMode(void);
extern void func_002D5018(void);
extern s32 sdfGraphWakeDisplayThread(s32);
extern void sceGsResetPath(void);

void sdfRunGraphicsTransferWorker(void) {
    u32 startTicks;

    D_003BD2F1 = 0;
    sdfAddHandler(1, 1, sdfGraphWakeDisplayThread, -1, 0);
    func_0030B638(1);
    for (;;) {
        CancelWakeupThread(GetThreadId());
        D_003BD2F1 = 1;
        SleepThread();
        startTicks = func_002CF930();
        WaitSema(sdfGsImageUploadSemaphore);
        if (D_003BD9DC != 0) {
            D_003BD9DC = 0;
            sdfGraphRecreateBuffers(&D_003980E0);
        }
        if (D_003BD9DD != 0) {
            D_003BD9DD = 0;
            sdfGraphResetDeviceForCurrentMode();
        }
        D_003BD32D = 1;
        func_002D5018();
        D_003BD32D = 0;
        if (D_003BD32C != 0) {
            /* Preserve the controller enable state while stopping VIF1. */
            u32 enabledChannels = *(volatile u32 *)0x1000F520;
            *(volatile u32 *)0x1000F590 = 0x10000;
            *(volatile u32 *)0x10009000 = 0;
            *(volatile u32 *)0x1000F590 = enabledChannels;
            sceGsResetPath();
            *(volatile u64 *)0x12001040 = 0;
            D_003BD32C = 1;
        } else {
            sdfCaptureDeferredGsImage();
        }
        SignalSema(sdfGsImageUploadSemaphore);
        D_003BD32E = sdfGetElapsedTimerTicks(startTicks);
    }
}

extern u8 D_003BD2E1;
extern s32 D_003BD9D8;
extern s32 D_003BD2FC;
extern volatile u8 sdfPacketSlotIndex;
extern void sdfSleepThreadCount(s32);
extern void sdfDevSignalPendingSemaphore(void);
extern s32 WakeupThread(s32);
extern void sdfGraphSelectDisplayBuffer(s32);

/* GS CSR bit 13 supplies the double-buffer field selector. */
#define SDF_GS_CSR_FIELD_SHIFT 13

/* Service device requests, select the GS field's buffer, and age packet slots.
 * Capture state 1 wakes its worker; state 2 precedes the semaphore wait. */
void sdfServiceGraphicsBuffers(void) {
    s8 bufferIndex;
    s32 invertedField;

    for (;;) {
        sdfSleepThreadCount(0);
        if ((D_003BD2E1 & 1) == 0) {
            sdfDevSignalPendingSemaphore();
        }
        invertedField = (~*(volatile u64 *)0x12001000 >> SDF_GS_CSR_FIELD_SHIFT) & 1;
        D_003BD2E1 = invertedField ^ 1;
        if (D_003BD2F1 == 1) {
            D_003BD2F1 = 2;
            WakeupThread(D_003BD9D8);
            WaitSema(D_003BD2FC);
        }
        bufferIndex = sdfBufferSlotIndices[D_003BD2E1];
        if (bufferIndex >= 0) {
            sdfGraphSelectDisplayBuffer(bufferIndex);
        }
        if (sdfPacketSlotIndex != 0) {
            sdfPacketSlotIndex--;
        }
    }
}

extern s32 sdfCreateThread(void *, void *, s32, s32);
extern s32 _StartThread(s32, s32);

extern u8 D_003BD2E8;
extern u8 D_003BD2E9;
extern SdfThreadNode D_003BD9D0;
extern u8 D_003E3820[0x8000];
extern u8 D_003E2820[0x1000];
extern void sdfResizeDoubleBuffer(s32);
extern void sdfResetPacketSlotState(void);
extern void sdfRegisterTextureReleaseRequestHandler(void);

void func_002D1590(s32 size) {
    s32 thread;
    SdfGraphObj *graph = &D_003980E0;

    sceGsResetPath();
    D_003BD2E8 = 1;
    sdfGraphApplyModeDefaults(graph, 1, 1);
    sdfGraphRecreateBuffers(graph);
    D_003BD2E9 = 0;
    sdfBufferSlotIndices[0] = sdfBufferSlotIndices[1] = -1;
    sdfBusyBufferIndex = -1;
    D_003BD9DD = 1;
    sdfResizeDoubleBuffer(size);
    sdfResetPacketSlotState();
    sdfRegisterTextureReleaseRequestHandler();
    sdfGsImageUploadSemaphore = sdfCreateSemaphore(1, 0x20, 0);
    D_003BD2FC = sdfCreateSemaphore(0, 0x20, 0);
    thread = sdfCreateThread((void *)sdfRunGraphicsTransferWorker, D_003E3820, 0x8000, 0x44);
    D_003BD9D8 = thread;
    _StartThread(thread, 0);
    sdfStartTrackedThread(&D_003BD9D0, sdfServiceGraphicsBuffers, D_003E2820,
                          0x1000, 0x40, 0);
}

extern vu8 sdfCurrentBufferIndex;
extern void sdfVuClearTransformCache(void);

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
    SdfTexResource *node = sdfTextureListHead;

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

SdfTexResource *sdfTexAllocHeadLow(s32 size, s32 allocationMode) {
    SdfTexResource *block;
    SdfTexResource *allocated;
    s32 interruptsEnabled;

    interruptsEnabled = func_00312C08();
    for (block = sdfTextureListHead;; block = block->prev) {
        if (block == NULL) {
            if (interruptsEnabled != 0) {
                EIntr();
            }
            if (D_003BD304 != NULL) {
                D_003BD304(size, allocationMode);
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
        allocated->word = block->word;
        allocated->size = size;
        allocated->prev = block;
        block->word += size;
        block->size -= size;
        if (interruptsEnabled != 0) {
            EIntr();
        }
        return allocated;
    }
}

SdfTexResource *sdfTexAllocHeadHigh(s32 size, s32 allocationMode) {
    SdfTexResource *block;
    SdfTexResource *allocated;
    s32 interruptsEnabled;

    interruptsEnabled = func_00312C08();
    for (block = sdfTextureBlockListHead;; block = block->next) {
        if (block == NULL) {
            if (interruptsEnabled != 0) {
                EIntr();
            }
            if (D_003BD304 != NULL) {
                D_003BD304(size, allocationMode);
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
        allocated->word = block->word + (block->size - size);
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
SdfTexResource *sdfTexAllocateHeadForDimensions(s32 width, s32 height, s32 format, s32 allocationMode, s32 useFirstAllocator) {
    s32 storageBitsPerPixel = sdfFormatBitsPerPixelA(format);
    s32 sizeWords = (width * height * storageBitsPerPixel) >> 5;
    SdfTexResource *textureBlock;

    if (useFirstAllocator != 0) {
        textureBlock = sdfTexAllocHeadLow(sizeWords, allocationMode);
    } else {
        textureBlock = sdfTexAllocHeadHigh(sizeWords, allocationMode);
    }
    textureBlock->width = width;
    textureBlock->height = height;
    textureBlock->format = format;
    return textureBlock;
}

/* Merge the previous free range into block; return one only when a record was recycled. */
s32 sdfCoalesceUnusedTextureBlocks(SdfTexResource *block) {
    SdfTexResource *previousBlock = block->prev;

    if (previousBlock == NULL || previousBlock->allocationMode != 0) {
        return 0;
    }
    block->size += previousBlock->size;
    if ((block->prev = previousBlock->prev) != NULL) {
        previousBlock->prev->next = block;
    } else {
        sdfTextureBlockListHead = block;
    }
    sdfReleaseChipBlock((SdfTex *)previousBlock);
    return 1;
}

void func_002D1B28(void) {
    SdfTexResource *node;

    node = sdfTextureListHead;
    while (node->prev != NULL) {
        node = node->prev;
    }
    node = sdfTextureBlockListHead;
    while (node->next != NULL) {
        node = node->next;
    }
}

/* Mark a range free and coalesce its neighbors while interrupts are masked. */
void sdfUpdateTextureHeadsWithInterruptsMasked(SdfTexResource *textureBlock) {
    s32 restoreInterrupts;

    if (textureBlock == NULL) {
        return;
    }
    restoreInterrupts = func_00312C08();
    textureBlock->allocationMode = 0;
    sdfCoalesceUnusedTextureBlocks(textureBlock);
    if (textureBlock->next != NULL) {
        if (textureBlock->next->allocationMode == 0) {
            sdfCoalesceUnusedTextureBlocks(textureBlock->next);
        }
    }
    if (restoreInterrupts != 0) {
        EIntr();
    }
}

void sdfTexQueuePendingWork(SdfTexResource *texture) {
    sdfPendingQueuePush(&sdfTextureUpdateQueue, (u32)texture);
}

void sdfTexInitializeLists(void) {
    SdfTexResource *head;

    head = sdfAllocSizeClassBlock(0x1C);
    head->size = 0x100000;
    head->next = NULL;
    head->prev = NULL;
    head->allocationMode = 0;
    head->word = 0;
    sdfTextureListHead = head;
    sdfTextureBlockListHead = head;
    sdfInitializeSynchronizedRequest(&sdfTextureUpdateQueue, sdfUpdateTextureHeadsWithInterruptsMasked);
}

/* Align the storage rows, pack 16-bit formats two pixels per word, then allocate whole GS pages. */
SdfTexResource *sdfAllocImageBuffer(s32 width, s32 height, s32 format) {
    s32 rowWidth = (width + 0x3F) & -0x40;
    s32 rowCount = (height + 0x1F) & -0x20;
    SdfTexResource *textureBlock;

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
    textureBlock = sdfTexAllocHeadLow((rowWidth * rowCount + 0x7FF) & -0x800, 1);
    textureBlock->width = width;
    textureBlock->height = height;
    textureBlock->format = format;
    return textureBlock;
}

SdfTexResource *sdfGetTextureListHead(void) {
    return sdfTextureListHead;
}

SdfTexResource *sdfGetTextureBlockListHead(void) {
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



void func_002D1D80(SdfImageUploadRequest *request) {
    SdfTextureQueue *queue = &sdfTextureQueueWork;
    SdfTextureReleaseHead *releaseEntry;
    SdfTextureDmaTail *oldTail;
    u64 *setup;
    u64 *packet;
    void *allocation;
    u32 format;
    u16 width;
    u16 height;
    u16 bufferWidth;
    s32 qwordCount;
    s32 allocationBlocks;
    s32 remaining;
    s32 bufferWidthUnits;
    s32 isLast;
    u32 sourceAddress;
    u64 formatAndDestination;
    u64 coordinates;
    u64 dimensions;

    format = request->format;
    width = request->width;
    height = request->height;
    bufferWidth = request->bufferWidth;
    bufferWidthUnits = bufferWidth >> 6;
    if (bufferWidthUnits == 0) {
        bufferWidthUnits = 1;
    }

    qwordCount = sdfFormatImageSize(format, width, height);
    allocationBlocks = (qwordCount + 0x7FEF) / 0x7FF0;
    allocation = sdfAllocSizeClassBlock(allocationBlocks * 0x30 + 0x80);
    releaseEntry = (SdfTextureReleaseHead *)allocation;
    releaseEntry->next = NULL;
    releaseEntry->chipMemory = request->pixels;
    releaseEntry->allocation = request->allocation;
    releaseEntry->releaseMode = request->allocationMode;

    setup = (u64 *)((u8 *)allocation + 0x10);
    packet = (u64 *)((u8 *)allocation + 0x70);
    formatAndDestination = ((u64)format << 56) |
                          ((u64)bufferWidthUnits << 48) |
                          ((u64)(request->destination >> 6) << 32);
    coordinates = ((u64)request->y << 48) | ((u64)request->x << 32);
    dimensions = ((u64)height << 32) | width;

    setup[0] = 0x0000000010000005ULL;
    setup[1] = 0x5000000500000000ULL;
    setup[2] = 0x1000000000000004ULL;
    setup[3] = 0x000000000000000EULL;
    setup[4] = formatAndDestination;
    setup[5] = 0x50ULL;
    setup[6] = coordinates;
    setup[7] = 0x51ULL;
    setup[8] = dimensions;
    setup[9] = 0x52ULL;
    setup[10] = 0;
    setup[11] = 0x53ULL;

    sourceAddress = (u32)request->pixels & 0x0FFFFFFF;
    remaining = qwordCount;
    isLast = 0;
    do {
        s32 chunk = 0x7FF0;

        if (remaining < 0x7FF1) {
            chunk = remaining;
            isLast = 1;
        }

        packet[0] = 0x0000000010000001ULL;
        packet[1] = 0x5000000100000000ULL;
        packet[2] = ((u64)chunk | ((u64)isLast << 15)) | 0x0800000000000000ULL;
        packet[4] = ((u32)chunk & 0xFFFF) | 0x30000000ULL | ((u64)sourceAddress << 32);
        packet[5] = ((u64)((u32)chunk | 0x51000000)) << 32;

        sourceAddress += (u32)chunk << 4;
        remaining -= chunk;
        packet += 6;
    } while (isLast == 0);

    WaitSema(queue->semaphoreId);

    if (queue->releaseTail != NULL) {
        queue->releaseTail->next = releaseEntry;
    } else {
        queue->releaseHead = releaseEntry;
    }
    queue->releaseTail = releaseEntry;

    setup = (u64 *)((u32)setup & 0x0FFFFFFF);
    oldTail = queue->packetTail;
    if (oldTail != NULL) {
        oldTail->vifCodes = 0;
        oldTail->tag = ((u64)(u32)setup << 32) | 0x20000000ULL;
    } else {
        queue->dmaPacketHead = (void *)(u32)setup;
    }
    queue->packetTail = (SdfTextureDmaTail *)packet;
    SignalSema(queue->semaphoreId);
}


void sdfTexEnqueuePacketWithSemaphore(s32 address, SdfTextureDmaTail *packet) {
    SdfTextureQueue *obj = &sdfTextureQueueWork;
    SdfTextureDmaTail *last;

    WaitSema(obj->semaphoreId);
    last = obj->packetTail;
    if (last != NULL) {
        last->vifCodes = 0;
        last->tag = ((u64)(address & 0x0FFFFFFF) << 32) | 0x20000000;
    } else {
        obj->dmaPacketHead = (void *)address;
    }
    obj->packetTail = packet;
    SignalSema(obj->semaphoreId);
}

void sdfTexQueueResourceRelease(s32 address) {
    SdfTextureQueue *obj = &sdfTextureQueueWork;
    SdfTextureReleaseHead *entry;

    if (address != 0) {
        entry = sdfAllocAndClearQuadwords(sizeof(SdfTexReleaseEntry));
        if (sdfChipIsInRange(address) != 0) {
            entry->chipMemory = (void *)address;
            entry->releaseMode = SDF_TEX_RELEASE_CHIP_ADDRESS;
        } else {
            entry->releaseMode = SDF_TEX_RELEASE_GENERAL_ALLOCATION;
            entry->allocation = sdfFindGeneralBlockByAddress((void *)address);
        }
        WaitSema(obj->semaphoreId);
        if (obj->releaseTail != NULL) {
            obj->releaseTail->next = entry;
        } else {
            obj->releaseHead = entry;
        }
        obj->releaseTail = entry;
        SignalSema(obj->semaphoreId);
    }
}

void sdfResetSemaphoreState(SdfTextureQueue *semaphore) {
    semaphore->releaseHead = NULL;
    semaphore->releaseTail = NULL;
    semaphore->dmaPacketHead = NULL;
    semaphore->packetTail = NULL;
}

s32 func_002D2140(s32 channel, s32 threadId) {
    iWakeupThread(threadId);
    EE_ENABLE_INTERRUPTS_SYNC();
    return -1;
}

extern s32 func_0030A740(s32 channel, s32 (*handler)(s32, s32), s32 arg, s32 threadId);
extern void *sceDmaGetChan(s32 channel);
extern void FlushCache(s32 mode);
extern void sceDmaSend(void *channel, void *packet);
extern s32 RemoveDmacHandler(s32 channel, s32 handlerId);
extern u8 D_00398100[];

void func_002D2168(void) {
    SdfTextureQueue *work = &sdfTextureQueueWork;
    SdfTextureReleaseHead *entry;
    SdfTextureDmaTail *packet;
    void *dmaPacket;

    WaitSema(work->semaphoreId);
    entry = work->releaseHead;
    dmaPacket = work->dmaPacketHead;
    packet = work->packetTail;
    sdfResetSemaphoreState(work);
    SignalSema(work->semaphoreId);

    if (dmaPacket != NULL) {
        s32 threadId = GetThreadId();
        s32 handlerId = func_0030A740(1, func_002D2140, 0, threadId);
        u64 tag = ((u64)((u32)D_00398100 & 0x0FFFFFFF) << 32) | 0x20000000;
        u32 *channel;

        packet->vifCodes = 0;
        packet->tag = tag;
        channel = (u32 *)sceDmaGetChan(1);
        channel[0] |= 0x40;
        FlushCache(0);
        sceDmaSend((void *)channel, dmaPacket);
        SleepThread();
        RemoveDmacHandler(1, handlerId);
    }

    while (entry != NULL) {
        SdfTextureReleaseHead *next = entry->next;

        switch (entry->releaseMode) {
        case SDF_TEX_RELEASE_GENERAL_ALLOCATION:
            sdfReleaseResourceAllocation(entry->allocation);
            break;
        case SDF_TEX_RELEASE_CHIP_ADDRESS:
            sdfReleaseChipBlock(entry->chipMemory);
            break;
        }
        sdfReleaseChipBlock(entry);
        entry = next;
    }
}

void sdfTexInitializeSemaphore(void) {
    SdfTextureQueue *obj;

    obj = &sdfTextureQueueWork;
    obj->semaphoreId = sdfCreateSemaphore(1, 0x7F, 0);
    sdfResetSemaphoreState(obj);
}

SdfTexBuf *sdfTexGetPrimaryBuffer(SdfTex *texture) {
    return texture->primaryBuffer;
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

SdfTexBuf *sdfTexGetOrInitializeSecondaryBuffer(SdfTex *texture) {
    SdfTexBuf *buf;

    buf = texture->secondaryBuffer;
    if (buf == NULL) {
        sdfTexCreateSecondPacket(texture);
        buf = texture->secondaryBuffer;
    }
    return buf;
}

/* Mirror the primary-buffer size calculation for the secondary buffer. */
s32 sdfTexGetSecondaryBufferSize(SdfTex *texture) {
    SdfTexBuf *buffer = texture->secondaryBuffer;

    if (buffer == NULL) {
        return 0;
    }
    return ((buffer->gifTagWord & SDF_GIF_LOOP_COUNT_MASK) + 1) << SDF_QWORD_BYTE_SHIFT;
}

u8 sdfTexGetPaletteCount(SdfTex *texture) {
    return texture->paletteCount;
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
s32 sdfTexFormatSizeHint(s32 format) {
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
        sdfTexCreateSecondPacket(texture);
        buffer = texture->secondaryBuffer;
    }
    buffer->samplingState = (buffer->samplingState & ~SDF_TEX_FILTER_MASK) | (magFilter << SDF_MAG_FILTER_SHIFT) | (minFilter << SDF_MIN_FILTER_SHIFT);
}

void sdfTexSetClampMode(SdfTex *texture, u8 value) {
    texture->clampMode = value;
    sdfTexRefreshResourcePackets(texture);
}

/* Borrow pixels or copy them to the selected heap; return the source cursor after the transfer bytes. */
u8 *sdfTexSubmitImageCopy(u32 destination, s32 width, s32 height, u32 format, u8 *pixels, s32 borrowPixels) {
    SdfImageUploadRequest request;
    s32 imageBytes = sdfFormatImageSize(format, width, height) * 16;

    if (borrowPixels == 0) {
        if (imageBytes > SDF_UPLOAD_CHIP_MAX_BYTES) {
            request.allocation = sdfAllocGeneralBlock(imageBytes);
            request.pixels = (void *)sdfResourceRetainAddress(request.allocation);
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
    func_002D1D80(&request);
    return pixels + imageBytes;
}

extern u8 *sdfTexSubmitImageCopy(u32 destination, s32 width, s32 height,
    u32 format, u8 *pixels, s32 borrowPixels);

/* Upload the CLUT as 16x16 for 8-bit indexed formats, otherwise 8x2; forward pixel ownership. */
u8 *sdfTexSubmitPixelsForFormat(SdfTex *texture, u32 destination, u8 *pixels, s32 borrowPixels) {
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
        sdfTexSubmitPixelsForFormat(tex, sdfTexGetSecondaryResourceWord(tex), tex->paletteData, 0);
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

SdfTex *sdfTexCreateResourceWithReference(s32 x, s32 y, s32 pixelFormat, s32 maxMipLevel, SdfTexResource *primary, s32 paletteFormat, s32 paletteCount, SdfTexResource *secondary) {
    SdfTex *tex = sdfAllocAndClearQuadwords(0x40);
    SdfTexRef *ref = sdfAllocAndClearQuadwords(8);

    ref->refCount = 1;
    tex->paletteCount = paletteCount;
    tex->clutFormat = paletteFormat;
    tex->width = x;
    tex->height = y;
    tex->pixelFormat = pixelFormat;
    tex->maxMipLevel = maxMipLevel;
    tex->secondaryResource = secondary;
    tex->primaryResource = primary;
    tex->reference = ref;
    sdfTexListInsert(tex);
    tex->unk38 = 0x80808080;
    return tex;
}

extern SdfTexResource *sdfTexAllocHead(s32, s32, s32);
extern void sdfTexAllocatePaletteData(SdfTex *);
extern void sdfTexCopyImageData(SdfTex *, void *);
extern void sdfTexCreateFirstPacket(SdfTex *);

SdfTex *sdfTexClone(SdfTex *source) {
    SdfTex *texture;
    SdfTexRef *reference;
    SdfTex *original;

    texture = sdfAllocSizeClassBlock(sizeof(*texture));
    *texture = *source;
    reference = sdfAllocAndClearQuadwords(sizeof(*reference));
    original = reference->cloneSource;
    reference->refCount = 1;
    texture->reference = reference;
    if (original == NULL) {
        original = source;
        reference->cloneSource = original;
    }
    texture->primaryBuffer = NULL;
    texture->secondaryBuffer = NULL;
    texture->paletteData = NULL;
    texture->intensityMap = NULL;
    sdfTexListInsert(texture);
    if (texture->secondaryResource != NULL) {
        texture->secondaryResource = sdfTexAllocHead(
            texture->pixelFormat, texture->clutFormat, texture->paletteCount);
        sdfTexAllocatePaletteData(texture);
        sdfTexCopyImageData(texture, original->paletteData);
        texture->unk38 = 0x80808080;
    }
    sdfTexCreateFirstPacket(texture);
    sdfTexUploadSecondaryResource(texture);
    return texture;
}

/* Allocate a texture head for a 0x20-byte (kind 0) or 0x10-byte (kind 2/10) unit; types 19/27 use unit*8 bytes and 0x20 rows, 20/36/44 use 0x40 bytes and 8 rows. */
SdfTexResource *sdfTexAllocHead(s32 type, s32 kind, s32 unused) {
    SdfTexResource *head = NULL;
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
    head = sdfTexAllocHeadHigh(size, SDF_TEX_RESOURCE_CLUT);
    head->width = 8;
    head->height = height;
    head->format = kind;
    return head;
}

INCLUDE_SDATA(const s32, "game/code_002D10B0", D_003BD2F0);

INCLUDE_SDATA(const s32, "game/code_002D10B0", D_003BD2F1);

INCLUDE_SDATA(const s32, "game/code_002D10B0", D_003BD2F4);

INCLUDE_SDATA(const s32, "game/code_002D10B0", D_003BD2F8);

INCLUDE_SDATA(const s32, "game/code_002D10B0", D_003BD2FC);

INCLUDE_SDATA(const s32, "game/code_002D10B0", sdfBufferSlotIndices);

INCLUDE_SDATA(const s32, "game/code_002D10B0", sdfBusyBufferIndex);

INCLUDE_SDATA(const s32, "game/code_002D10B0", D_003BD304);

INCLUDE_SDATA(const s32, "game/code_002D10B0", sdfResourceListHead);

