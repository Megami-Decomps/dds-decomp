#include "sdk_ps2.h"
#include "common.h"
#include "sdf_handler_node.h"
#include "eeregs.h"
#include "sdf_timer.h"
#include "sdf_texture_queue.h"
#include "sdf_image_upload.h"
#include "sdf_thread.h"
#include "ee_mmi.h"

#include "sdf_resource.h"
#include "sdf_chip.h"

#include "sdf.h"
#include "sdf_semaphore_api.h"
#include "sdf_texture_primary_buffer_api.h"
#include "sdf_texture_submit.h"
#include "sdf_texture_lists.h"
#include "sdf_texture_registry.h"
#include "sdf_pending.h"
#include "sdf_texture_release.h"
#include "sdf_image_packets.h"
#include "sdf_tex_copy_image_data_api.h"
#include "sdf_vu_transform_api.h"
#include "sdf_tex_packet.h"

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





s32 sdfGsMemAbsorbFreePredecessor(sdfGsMemSlice_t *block);
extern u8 sdfGrCaptureBuff;

extern u8 *sdfGrCaptureReq;

extern void (*sdfGrCaptureTarget)(u8 *);


extern SdfPendingRequest gsMemDelayedResrcCB;



extern sdfGsMemSlice_t *gsMemHandleLinkTail;

extern sdfGsMemSlice_t *gsMemHandleLinkHead;

extern void (*D_004389F4)(s32 size, s32 allocationMode);



extern sdfGsMemSlice_t *sdfGsMemAllocFromHead(s32 size, s32 allocationMode);

extern sdfGsMemSlice_t *sdfGsMemAllocFromTail(s32 size, s32 allocationMode);

extern SdfTextureQueue sdfImageSetCb;
extern s8 sdfDispFrameId[2];

extern volatile s8 sdfGrDrawBusy;

extern vu8 sdfFrameId;



void sdfGrReqCapture(u8 *destination, void (*onComplete)(u8 *)) {
    sdfGrCaptureReq = destination;
    sdfGrCaptureTarget = onComplete;
    sdfGrCaptureBuff = 1;
}

extern sceGsStoreImage sdfGsCaptureStoreImage;
extern sdfGrFrameSet_t D_0040B290;
extern volatile u8 sdfDispThreadFlag;


/* Interleave rows from both color buffers into the queued destination. */
void sdfGrCapture(void) {
    s32 width;
    s32 height;
    s32 format;
    s32 pixelBytes;
    s32 rowBytes;
    s32 stride;
    sdfMemHandle_t *allocation;
    s32 rows;
    u8 *pixels;
    u8 *source;
    u8 *destination;

    if (sdfGrCaptureBuff) {
        width = D_0040B290.width;
        height = D_0040B290.height;
        format = D_0040B290.bufferFormat;
        pixelBytes = 4;
        if (format == SDF_PSMCT16) {
            pixelBytes = 2;
        }
        rowBytes = pixelBytes * width;
        stride = rowBytes * 2;
        sdfDispThreadFlag = 5;
        allocation = sdfMemAllocDirect(rowBytes * height);
        pixels = (u8 *)sdfMemLock(allocation);
        sceGsSetDefStoreImage(
            (sceGsStoreImage *)(((u32)&sdfGsCaptureStoreImage & 0x0FFFFFFF) | 0x20000000),
            D_0040B290.buffers[1]->word >> 6,
            width / 64, format, 0, 0, width, height);
        sceGsExecStoreImage(&sdfGsCaptureStoreImage, pixels);
        sceGsSyncPath(0, 0);
        destination = sdfGrCaptureReq;
        source = pixels;
        rows = height;
        do {
            memcpy(destination, source, rowBytes);
            destination += stride;
            source += rowBytes;
        } while (--rows != 0);
        sceGsSetDefStoreImage(
            (sceGsStoreImage *)(((u32)&sdfGsCaptureStoreImage & 0x0FFFFFFF) | 0x20000000),
            D_0040B290.buffers[0]->word >> 6,
            width / 64, format, 0, 0, width, height);
        sceGsExecStoreImage(&sdfGsCaptureStoreImage, pixels);
        sceGsSyncPath(0, 0);
        destination = sdfGrCaptureReq + rowBytes;
        source = pixels;
        rows = height;
        do {
            memcpy(destination, source, rowBytes);
            destination += stride;
            source += rowBytes;
        } while (--rows != 0);
        sdfMemFreeDirect(allocation);
        sdfGrCaptureBuff = 0;
        if (sdfGrCaptureTarget) {
            sdfGrCaptureTarget(sdfGrCaptureReq);
        }
    }
}

/* Switch both references off the finished double-buffer slot before
 * publishing the slot currently in use. */
void sdfDeliverDrawStart(s32 oldBuffer, s32 nextBuffer) {
    if (sdfDispFrameId[0] == oldBuffer) {
        sdfDispFrameId[0] = oldBuffer ^ 1;
    }
    if (sdfDispFrameId[1] == oldBuffer) {
        sdfDispFrameId[1] = oldBuffer ^ 1;
    }
    sdfGrDrawBusy = nextBuffer;
}

/* Update one slot or both slots, then clear the busy-buffer status. */
void sdfDeliverDrawEnd(s32 updateSingleSlot, s32 bufferIndex, s32 slotIndex) {
    if (updateSingleSlot == 0) {
        sdfDispFrameId[0] = bufferIndex;
        sdfDispFrameId[1] = bufferIndex;
    } else {
        sdfDispFrameId[slotIndex] = bufferIndex;
    }
    sdfGrDrawBusy = SDF_NO_BUSY_BUFFER;
}

extern s8 reqMakeFrameBuff;
extern s8 reqGsReset;
extern volatile s32 dds3GsSema;
extern volatile u8 D_00438A1D;
extern volatile u8 sdfDrawFrameFlag;
extern u16 D_00438A1E;
extern SdfHandlerNode *sdfAddHandler(SdfHandlerKind, s32, SdfHandlerCallback, s32, s32);
extern void sdfGrMakeFrameSet(sdfGrFrameSet_t *);
extern void sdfGrGsReset(void);
extern void sdfGrDrawFrame(void);
extern s32 sdfDrawThIntHandler(s32);

void sdfDrawThread(void) {
    u32 startTicks;

    sdfDispThreadFlag = 0;
    sdfAddHandler(SDF_HANDLER_KIND_DMA, 1, sdfDrawThIntHandler, -1, 0);
    EnableDmac(1);
    for (;;) {
        CancelWakeupThread(GetThreadId());
        sdfDispThreadFlag = 1;
        SleepThread();
        startTicks = sdfGetRunTimeCountBase();
        WaitSema(dds3GsSema);
        if (reqMakeFrameBuff != 0) {
            reqMakeFrameBuff = 0;
            sdfGrMakeFrameSet(&D_0040B290);
        }
        if (reqGsReset != 0) {
            reqGsReset = 0;
            sdfGrGsReset();
        }
        D_00438A1D = 1;
        sdfGrDrawFrame();
        D_00438A1D = 0;
        if (sdfDrawFrameFlag != 0) {
            /* Preserve the controller enable state while stopping VIF1. */
            u32 enabledChannels = HW_READ(D_ENABLER);
            HW_WRITE(D_ENABLEW, 0x10000);
            HW_WRITE(D1_CHCR, 0);
            HW_WRITE(D_ENABLEW, enabledChannels);
            sceGsResetPath();
            HW_WRITE(GS_BUSDIR, 0);
            sdfDrawFrameFlag = 1;
        } else {
            sdfGrCapture();
        }
        SignalSema(dds3GsSema);
        D_00438A1E = sdfGetRunTimeCount(startTicks);
    }
}

extern u8 sdfFrameEvenOdd;
extern s32 sdfDrawThreadId;
extern s32 dds3GsThreadId;
extern volatile u8 sdfReqDrawDelayCount;
extern void sdfWaitVblank(s32);
extern void sdfDevSignalPendingSemaphore(void);
extern void sdfGrSetDisp(s32);

/* GS CSR bit 13 supplies the double-buffer field selector. */
#define SDF_GS_CSR_FIELD_SHIFT 13

/* Service device requests, select the GS field's buffer, and age packet slots.
 * Capture state 1 wakes its worker; state 2 precedes the semaphore wait. */
void sdfServiceGraphicsBuffers(void) {
    s8 bufferIndex;
    s32 invertedField;

    for (;;) {
        sdfWaitVblank(0);
        if ((sdfFrameEvenOdd & 1) == 0) {
            sdfDevSignalPendingSemaphore();
        }
        invertedField = (~HW_READ(GS_CSR) >> SDF_GS_CSR_FIELD_SHIFT) & 1;
        sdfFrameEvenOdd = invertedField ^ 1;
        if (sdfDispThreadFlag == 1) {
            sdfDispThreadFlag = 2;
            WakeupThread(sdfDrawThreadId);
            WaitSema(dds3GsThreadId);
        }
        bufferIndex = sdfDispFrameId[sdfFrameEvenOdd];
        if (bufferIndex >= 0) {
            sdfGrSetDisp(bufferIndex);
        }
        if (sdfReqDrawDelayCount != 0) {
            sdfReqDrawDelayCount--;
        }
    }
}

extern u8 sdfGrScreenMode;
extern u8 D_004389D9;
extern SdfThreadNode sdfGraphicsBufferThread;
extern u8 sdfGraphicsTransferThreadStack[0x8000];
extern u8 sdfGraphicsBufferThreadStack[0x1000];
extern void sdfGrMakePacketBuff(s32);
extern void sdfGrInitDraw(void);
extern void sdfTexInit(void);

extern s32 CreateThreadSimple(void (*entryAddress)(), void *workspace, s32 stackBytes, s32 priority);

/* Reset GS/buffer state and start graphics workers; packetBufferBytes sizes each packet-buffer half. */
void sdfGrInit(s32 packetBufferBytes) {
    s32 thread;
    sdfGrFrameSet_t *graph = &D_0040B290;

    sceGsResetPath();
    sdfGrScreenMode = 1;
    sdfGrSetFrameSetMode(graph, 1, 1);
    sdfGrMakeFrameSet(graph);
    D_004389D9 = 0;
    sdfDispFrameId[0] = sdfDispFrameId[1] = -1;
    sdfGrDrawBusy = -1;
    reqGsReset = 1;
    sdfGrMakePacketBuff(packetBufferBytes);
    sdfGrInitDraw();
    sdfTexInit();
    dds3GsSema = sdfCreateSema(1, 0x20, 0);
    dds3GsThreadId = sdfCreateSema(0, 0x20, 0);
    thread = CreateThreadSimple(sdfDrawThread, (s32)sdfGraphicsTransferThreadStack, 0x8000, 0x44);
    sdfDrawThreadId = thread;
    _StartThread(thread, 0);
    sdfStartThreadAndWait(&sdfGraphicsBufferThread, sdfServiceGraphicsBuffers, sdfGraphicsBufferThreadStack,
                          0x1000, 0x40, 0);
}

extern void sdfGrSetupPacketBuff(s32 bufferIndex);

/* Wait until the other buffer is no longer busy before selecting it. */
void sdfWaitAndSelectBuffer(void) {
    s8 nextBuffer = sdfFrameId ^ 1;

    /* Do not select a buffer while its index is the busy-buffer status. */
    while (sdfGrDrawBusy == nextBuffer) {
    }
    sdfFrameId = nextBuffer;
    sdfGrSetupPacketBuff((s8)sdfFrameId);
    sdfVuClearTransformCache();
}

/* Storage depth for supported formats: listed 24-bit/high-plane modes use 32 bits; unlisted is zero. */
s32 sdfGsMemGetBitCount(u32 format) {
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
s32 sdfGetTexelBitCount(u32 format) {
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

s32 sdfGsMemCheckHandle(sdfTexHandle_t *target) {
    sdfGsMemSlice_t *node = gsMemHandleLinkHead;

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


sdfGsMemSlice_t *sdfGsMemAllocFromHead(s32 size, s32 allocationMode) {
    sdfGsMemSlice_t *block;
    sdfGsMemSlice_t *allocated;
    s32 interruptsEnabled;

    interruptsEnabled = DIntr();
    for (block = gsMemHandleLinkHead;; block = block->prev) {
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
        allocated = sdfChipAllocDirect(sizeof(*allocated));
        allocated->next = block->next;
        if (allocated->next == NULL) {
            gsMemHandleLinkHead = allocated;
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

sdfGsMemSlice_t *sdfGsMemAllocFromTail(s32 size, s32 allocationMode) {
    sdfGsMemSlice_t *block;
    sdfGsMemSlice_t *allocated;
    s32 interruptsEnabled;

    interruptsEnabled = DIntr();
    for (block = gsMemHandleLinkTail;; block = block->next) {
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
        allocated = sdfChipAllocDirect(sizeof(*allocated));
        allocated->prev = block->prev;
        if (allocated->prev == NULL) {
            gsMemHandleLinkTail = allocated;
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
sdfGsMemSlice_t *sdfGsMemAllocRect(s32 width, s32 height, s32 format, s32 allocationMode, s32 useFirstAllocator) {
    s32 storageBitsPerPixel = sdfGsMemGetBitCount(format);
    s32 sizeWords = (width * height * storageBitsPerPixel) >> 5;
    sdfGsMemSlice_t *textureBlock;

    if (useFirstAllocator != 0) {
        textureBlock = sdfGsMemAllocFromHead(sizeWords, allocationMode);
    } else {
        textureBlock = sdfGsMemAllocFromTail(sizeWords, allocationMode);
    }
    textureBlock->width = width;
    textureBlock->height = height;
    textureBlock->format = format;
    return textureBlock;
}

/* Absorb the previous free range into block; return one when its record is released. */
s32 sdfGsMemAbsorbFreePredecessor(sdfGsMemSlice_t *block) {
    sdfGsMemSlice_t *previousBlock = block->prev;

    if (previousBlock != NULL) {
        if (previousBlock->allocationMode == 0) {
            sdfGsMemSlice_t *beforePreviousBlock = previousBlock->prev;

            block->size = block->size + previousBlock->size;
            block->prev = beforePreviousBlock;
            if (beforePreviousBlock != NULL) {
                previousBlock->prev->next = block;
            } else {
                gsMemHandleLinkTail = block;
            }
            sdfChipFreeDirect(previousBlock);
            return 1;
        }
    }
    return 0;
}

/* Walk both list endpoints without returning them or changing either list head. */
void sdfGsMemCheckLinkConsistence(void) {
    sdfGsMemSlice_t *node;

    node = gsMemHandleLinkHead;
    while (node->prev != NULL) {
        node = node->prev;
    }
    node = gsMemHandleLinkTail;
    while (node->next != NULL) {
        node = node->next;
    }
}



/* Mark a range free and coalesce its neighbors while interrupts are masked. */
void sdfGsMemFree(sdfGsMemSlice_t *textureBlock) {
    s32 restoreInterrupts;
    sdfGsMemSlice_t *nextBlock;

    if (textureBlock == NULL) {
        return;
    }
    restoreInterrupts = DIntr();
    textureBlock->allocationMode = 0;
    sdfGsMemAbsorbFreePredecessor(textureBlock);
    nextBlock = textureBlock->next;
    if (nextBlock != NULL && nextBlock->allocationMode == 0) {
        sdfGsMemAbsorbFreePredecessor(nextBlock);
    }
    if (restoreInterrupts != 0) {
        EIntr();
    }
}

void sdfGsMemFreeDelayed(sdfGsMemSlice_t *texture) {
    sdfDelayedResrcEntry(&gsMemDelayedResrcCB, texture);
}

void sdfGsMemInit(void) {
    sdfGsMemSlice_t *head;

    head = sdfChipAllocDirect(0x1C);
    head->size = 0x100000;
    head->next = NULL;
    head->prev = NULL;
    head->allocationMode = 0;
    head->word = 0;
    gsMemHandleLinkHead = head;
    gsMemHandleLinkTail = head;
    sdfDelayedResrcInitCB(&gsMemDelayedResrcCB, sdfGsMemFree);
}

/* Align the storage rows, pack 16-bit formats two pixels per word, then allocate whole GS pages. */
sdfGsMemSlice_t *sdfGsAllocFrameBuff(s32 width, s32 height, s32 format) {
    s32 rowWidth = (width + 0x3F) & -0x40;
    s32 rowCount = (height + 0x1F) & -0x20;
    sdfGsMemSlice_t *textureBlock;

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
    textureBlock = sdfGsMemAllocFromHead((rowWidth * rowCount + 0x7FF) & -0x800, 1);
    textureBlock->width = width;
    textureBlock->height = height;
    textureBlock->format = format;
    return textureBlock;
}

sdfGsMemSlice_t *sdfGsMemGetLinkHead(void) {
    return gsMemHandleLinkHead;
}

sdfGsMemSlice_t *sdfGsMemGetLinkTail(void) {
    return gsMemHandleLinkTail;
}

/* Transfer size in 16-byte quadwords; low bits of a partial quadword are discarded. */
s32 sdfImageDataSize(u32 format, s32 width, s32 height) {
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



void sdfImageSetReq(SdfImageUploadRequest *request) {
    SdfTextureQueue *queue = &sdfImageSetCb;
    SdfTextureReleaseHead *releaseEntry;
    SdfTextureDmaTail *oldTail;
    SdfImageTransferPacket *setup;
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

    qwordCount = sdfImageDataSize(format, width, height);
    allocationBlocks = (qwordCount + 0x7FEF) / 0x7FF0;
    allocation = sdfChipAllocDirect(allocationBlocks * 0x30 + 0x80);
    releaseEntry = (SdfTextureReleaseHead *)allocation;
    releaseEntry->next = NULL;
    releaseEntry->chipMemory = request->pixels;
    releaseEntry->allocation = request->allocation;
    releaseEntry->releaseMode = request->allocationMode;

    setup = (SdfImageTransferPacket *)((u8 *)allocation + sizeof(SdfTextureReleaseHead));
    packet = (u64 *)((u8 *)allocation + 0x70);
    formatAndDestination = ((u64)format << 56) |
                          ((u64)bufferWidthUnits << 48) |
                          ((u64)(request->destination >> 6) << 32);
    coordinates = ((u64)request->y << 48) | ((u64)request->x << 32);
    dimensions = ((u64)height << 32) | width;

    setup->header.dmaTag = 0x0000000010000005ULL;
    setup->header.vifCommands = 0x5000000500000000ULL;
    setup->header.gifTag = 0x1000000000000004ULL;
    setup->header.gifRegisters = SDF_GIF_REGISTER_AD;
    setup->transfer.bitbltbuf.value = formatAndDestination;
    setup->transfer.bitbltbuf.registerId = SDF_GS_BITBLTBUF;
    setup->transfer.trxpos.value = coordinates;
    setup->transfer.trxpos.registerId = SDF_GS_TRXPOS;
    setup->transfer.trxreg.value = dimensions;
    setup->transfer.trxreg.registerId = SDF_GS_TRXREG;
    setup->transfer.trxdir.value = 0;
    setup->transfer.trxdir.registerId = SDF_GS_TRXDIR;

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

    setup = (SdfImageTransferPacket *)((u32)setup & 0x0FFFFFFF);
    oldTail = queue->packetTail;
    if (oldTail != NULL) {
        oldTail->vifCodes = 0;
        oldTail->tag = ((u64)(u32)setup << 32) | 0x20000000ULL;
    } else {
        queue->dmaPacketHeadAddress = (u32)setup;
    }
    queue->packetTail = (SdfTextureDmaTail *)packet;
    SignalSema(queue->semaphoreId);
}


void sdfImageSetChainTag(SdfDmaAddress packetAddress, SdfTextureDmaTail *packet) {
    SdfTextureQueue *obj = &sdfImageSetCb;
    SdfTextureDmaTail *last;

    WaitSema(obj->semaphoreId);
    last = obj->packetTail;
    if (last != NULL) {
        last->vifCodes = 0;
        last->tag = ((u64)(packetAddress & 0x0FFFFFFF) << 32) | 0x20000000;
    } else {
        obj->dmaPacketHeadAddress = packetAddress;
    }
    obj->packetTail = packet;
    SignalSema(obj->semaphoreId);
}

void sdfImageSetChainReleaseMem(void *address) {
    SdfTextureQueue *obj = &sdfImageSetCb;
    SdfTextureReleaseHead *entry;

    if (address != NULL) {
        entry = sdfAllocAndClearQuadwords(sizeof(SdfTexReleaseEntry));
        if (sdfChipIsInRange((s32)address) != 0) {
            entry->chipMemory = address;
            entry->releaseMode = SDF_TEX_RELEASE_CHIP_ADDRESS;
        } else {
            entry->releaseMode = SDF_TEX_RELEASE_GENERAL_ALLOCATION;
            entry->allocation = sdfMemGetHandleByPtr(address);
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

void sdfImageSetClearCb(SdfTextureQueue *semaphore) {
    semaphore->releaseHead = NULL;
    semaphore->releaseTail = NULL;
    semaphore->dmaPacketHeadAddress = 0;
    semaphore->packetTail = NULL;
}
/* DMA-completion callback for the image-upload waiter; the channel argument is unused. Returns -1. */
s32 sdfImageSetDmacHandler(s32 channel, s32 threadId) {
    iWakeupThread(threadId);
    EE_ENABLE_INTERRUPTS_SYNC();
    return -1;
}

extern s32 sdfImageSetDmacHandler(s32 channel, s32 threadId);
extern s32 AddDmacHandler2(s32 channel, s32 handler, s32 arg, s32 threadId);
extern u8 D_0040B2B0[];

/* Detach one queued batch, wait for its DMA, then release its owned source and packet allocations. */
void sdfImageSetTrans(void) {
    SdfTextureQueue *work = &sdfImageSetCb;
    SdfTextureReleaseHead *entry;
    SdfTextureDmaTail *packet;
    SdfDmaAddress dmaPacketAddress;

    WaitSema(work->semaphoreId);
    entry = work->releaseHead;
    dmaPacketAddress = work->dmaPacketHeadAddress;
    packet = work->packetTail;
    sdfImageSetClearCb(work);
    SignalSema(work->semaphoreId);

    if (dmaPacketAddress != 0) {
        s32 threadId = GetThreadId();
        s32 handlerId = AddDmacHandler2(1, (s32)sdfImageSetDmacHandler, 0, threadId);
        u64 tag = ((u64)((u32)D_0040B2B0 & 0x0FFFFFFF) << 32) | 0x20000000;
        u32 *channel;

        packet->vifCodes = 0;
        packet->tag = tag;
        channel = (u32 *)sceDmaGetChan(1);
        HW_WRITE(channel, HW_READ(channel) | 0x40);
        FlushCache(0);
        sceDmaSend((void *)channel, (void *)dmaPacketAddress);
        SleepThread();
        RemoveDmacHandler(1, handlerId);
    }

    while (entry != NULL) {
        SdfTextureReleaseHead *next = entry->next;

        switch (entry->releaseMode) {
        case SDF_TEX_RELEASE_GENERAL_ALLOCATION:
            sdfMemFreeDirect(entry->allocation);
            break;
        case SDF_TEX_RELEASE_CHIP_ADDRESS:
            sdfChipFreeDirect(entry->chipMemory);
            break;
        }
        sdfChipFreeDirect(entry);
        entry = next;
    }
}

void sdfImageSetInit(void) {
    SdfTextureQueue *obj;

    obj = &sdfImageSetCb;
    obj->semaphoreId = sdfCreateSema(1, 0x7F, 0);
    sdfImageSetClearCb(obj);
}

sdfTexPacket_t *sdfTexGetPacket(const sdfTexHandle_t *texture) {
    return texture->primaryBuffer;
}

/* Size in bytes of a packed primary texture buffer: only the low 15 bits
 * contribute to its 16-byte block count. */
s32 sdfTexGetPacketLength(const sdfTexHandle_t *texture) {
    const sdfTexPacket_t *buffer = texture->primaryBuffer;

    if (buffer == NULL) {
        return 0;
    }
    return ((buffer->gifTagWord & SDF_GIF_LOOP_COUNT_MASK) + 1) << SDF_QWORD_BYTE_SHIFT;
}

extern void sdfTexMakeGIFPacketCtx2(sdfTexHandle_t *texture);

sdfTexPacket_t *sdfTexGetPacketCtx2(sdfTexHandle_t *texture) {
    sdfTexPacket_t *buffer;

    buffer = texture->secondaryBuffer;
    if (buffer == NULL) {
        sdfTexMakeGIFPacketCtx2(texture);
        buffer = texture->secondaryBuffer;
    }
    return buffer;
}

/* Mirror the primary-buffer size calculation for the secondary buffer. */
s32 sdfTexGetPacketCtx2Length(const sdfTexHandle_t *texture) {
    const sdfTexPacket_t *buffer = texture->secondaryBuffer;

    if (buffer == NULL) {
        return 0;
    }
    return ((buffer->gifTagWord & SDF_GIF_LOOP_COUNT_MASK) + 1) << SDF_QWORD_BYTE_SHIFT;
}

u8 sdfTexGetCLUTCount(sdfTexHandle_t *texture) {
    return texture->paletteCount;
}

u32 sdfTexGetCLUTAddr(sdfTexHandle_t *texture) {
    u32 word;

    word = 0;
    if (texture->secondaryResource != NULL) {
        word = texture->secondaryResource->word;
    }
    return word;
}

u32 sdfTexGetPixelAddr(sdfTexHandle_t *texture) {
    return texture->primaryResource->word;
}

/* Color-format pixel depth; every unlisted format falls through to four bits. */
s32 sdfTexGetPixelBitCount(u32 format) {
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
s32 sdfTexGetTexelBitCount(s32 format) {
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

u64 sdfTexGetTex0Reg(sdfTexHandle_t *texture) {
    return texture->primaryBuffer->textureState;
}

u64 sdfTexGetTex1Reg(sdfTexHandle_t *texture) {
    return texture->primaryBuffer->samplingState;
}

u64 sdfTexGetClampReg(sdfTexHandle_t *texture) {
    return texture->primaryBuffer->clampState;
}

/* Clear TEX1 MMAG/MMIN and OR in the supplied filters; input values are not masked. */
void sdfTexSetFilter(sdfTexHandle_t *texture, s32 magFilter, s32 minFilter) {
    sdfTexPacket_t *buffer;

    buffer = texture->primaryBuffer;
    buffer->samplingState = (buffer->samplingState & ~SDF_TEX_FILTER_MASK) | (magFilter << SDF_MAG_FILTER_SHIFT) | (minFilter << SDF_MIN_FILTER_SHIFT);
}

/* Create the secondary packet if absent, then update TEX1 filters without masking input values. */
void sdfTexSetFilterCtx2(sdfTexHandle_t *texture, s32 magFilter, s32 minFilter) {
    sdfTexPacket_t *buffer = texture->secondaryBuffer;

    if (buffer == NULL) {
        sdfTexMakeGIFPacketCtx2(texture);
        buffer = texture->secondaryBuffer;
    }
    buffer->samplingState = (minFilter << SDF_MIN_FILTER_SHIFT) | ((magFilter << SDF_MAG_FILTER_SHIFT) | (buffer->samplingState & ~SDF_TEX_FILTER_MASK));
}

extern void sdfTexRebuildPacket(sdfTexHandle_t *texture);

void sdfTexSetWrapMode(sdfTexHandle_t *texture, SdfTextureClampMode mode) {
    texture->clampMode = mode;
    sdfTexRebuildPacket(texture);
}

/* Borrow pixels or copy them to the selected heap; return the source cursor after the transfer bytes. */
u8 *sdfTexTransData(u32 destination, s32 width, s32 height, u32 format, u8 *pixels, s32 borrowPixels) {
    SdfImageUploadRequest request;
    s32 imageBytes = sdfImageDataSize(format, width, height) * 16;

    if (borrowPixels == 0) {
        if (imageBytes > SDF_UPLOAD_CHIP_MAX_BYTES) {
            request.allocation = sdfMemAllocDirect(imageBytes);
            request.pixels = (void *)sdfMemLock(request.allocation);
            request.allocationMode = SDF_UPLOAD_GENERAL_HEAP;
        } else {
            request.pixels = sdfChipAllocDirect(imageBytes);
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
    sdfImageSetReq(&request);
    return pixels + imageBytes;
}

/* Upload the CLUT as 16x16 for 8-bit indexed formats, otherwise 8x2; forward pixel ownership. */
u8 *sdfTexTransCLUT(sdfTexHandle_t *texture, u32 destination, u8 *pixels, s32 borrowPixels) {
    s32 paletteWidth;
    s32 paletteHeight;

    if (texture->pixelFormat == SDF_PSMT8 || texture->pixelFormat == SDF_PSMT8H) {
        paletteWidth = 0x10;
        paletteHeight = 0x10;
    } else {
        paletteWidth = 8;
        paletteHeight = 2;
    }
    return sdfTexTransData(destination, paletteWidth, paletteHeight, texture->clutFormat, pixels, borrowPixels);
}

void sdfTexClutToGs(sdfTexHandle_t *tex) {
    if (tex->secondaryResource != NULL) {
        sdfTexTransCLUT(tex, sdfTexGetCLUTAddr(tex), tex->paletteData, 0);
    }
}

void sdfTexLink(sdfTexHandle_t *texture) {
    texture->next = NULL;
    if (sdfTexHandleLink != NULL) {
        texture->prev = sdfTexHandleLink;
        sdfTexHandleLink->next = texture;
    } else {
        texture->prev = NULL;
    }
    sdfTexHandleLink = texture;
}


sdfTexHandle_t *sdfTexCreateResourceWithReference(s32 x, s32 y, s32 pixelFormat, s32 maxMipLevel, sdfGsMemSlice_t *primary, s32 paletteFormat, s32 paletteCount, sdfGsMemSlice_t *secondary) {
    sdfTexHandle_t *tex = sdfAllocAndClearQuadwords(0x40);
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
    sdfTexLink(tex);
    tex->unk38 = 0x80808080;
    return tex;
}

extern sdfGsMemSlice_t *sdfTexAllocClut(s32, s32, s32);
sdfTexHandle_t *sdfTexClone(sdfTexHandle_t *source) {
    sdfTexHandle_t *texture;
    SdfTexRef *reference;
    sdfTexHandle_t *original;

    texture = sdfChipAllocDirect(sizeof(*texture));
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
    sdfTexLink(texture);
    if (texture->secondaryResource != NULL) {
        texture->secondaryResource = sdfTexAllocClut(
            texture->pixelFormat, texture->clutFormat, texture->paletteCount);
        sdfTexAllocClutBuff(texture);
        sdfTexCopyClutData(texture, original->paletteData);
        texture->unk38 = 0x80808080;
    }
    sdfTexMakeGIFPacket(texture);
    sdfTexClutToGs(texture);
    return texture;
}

/* Allocate a texture head for a 0x20-byte (kind 0) or 0x10-byte (kind 2/10) unit; types 19/27 use unit*8 bytes and 0x20 rows, 20/36/44 use 0x40 bytes and 8 rows. */
sdfGsMemSlice_t *sdfTexAllocClut(s32 type, s32 kind, s32 unused) {
    sdfGsMemSlice_t *head = NULL;
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
    head = sdfGsMemAllocFromTail(size, SDF_TEX_RESOURCE_CLUT);
    head->width = 8;
    head->height = height;
    head->format = kind;
    return head;
}

INCLUDE_SDATA(const s32, "game/code_00329F60", sdfGrCaptureBuff);

INCLUDE_SDATA(const s32, "game/code_00329F60", sdfDispThreadFlag);

INCLUDE_SDATA(const s32, "game/code_00329F60", sdfGrCaptureReq);

INCLUDE_SDATA(const s32, "game/code_00329F60", sdfGrCaptureTarget);

INCLUDE_SDATA(const s32, "game/code_00329F60", dds3GsThreadId);

INCLUDE_SDATA(const s32, "game/code_00329F60", sdfDispFrameId);

INCLUDE_SDATA(const s32, "game/code_00329F60", sdfGrDrawBusy);

INCLUDE_SDATA(const s32, "game/code_00329F60", D_004389F4);

INCLUDE_SDATA(const s32, "game/code_00329F60", sdfTexHandleLink);
