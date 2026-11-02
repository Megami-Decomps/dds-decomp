#include "sdk_ps2.h"
#include "common.h"
#include "sdf_handler_node.h"
#include "eeregs.h"
#include "sdf_timer.h"
#include "sdf_texture_queue.h"
#include "sdf_image_upload.h"
#include "sdf_thread.h"
#include "ee_mmi.h"
#include "sdf_heap_api.h"
#include "sdf_chip.h"
#include "sdf_chip_range_api.h"
#include "sdf.h"
#include "sdf_gs_mem_delayed_free_api.h"
#include "sdf_graph_buffer_state.h"
#include "sdf_display_thread_state.h"
#include "sdf_draw_frame_execution_state.h"
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

extern void (*sdfGrCaptureTarget)(u8 *);

extern void (*D_004389F4)(s32 size, s32 allocationMode);

extern SdfTextureQueue sdfImageSetCb;

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

void sdfImageSetReq(const SdfImageUploadRequest *request) {
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
        entry = sdfChipAllocZeroDirect(sizeof(SdfTexReleaseEntry));
        if (sdfChipIsInRange(address) != SDF_CHIP_ADDRESS_OUTSIDE_RANGE) {
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
