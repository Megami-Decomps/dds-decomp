#include "fpu.h"
#include "common.h"
#include "eeregs.h"
#include "libsdf.h"
#include "ps2asm.h"

enum {
    SDF_STREAM_READ_QUERY = 0,
    SDF_STREAM_READ_COPY = 1,
    SDF_STREAM_READ_RESUME = 2
};

#include "sdf_movie_types.h"

void sdfIpuStrmDecodeInit(sdfIpuDecodeCb_t *aNode, struct sdfIpuInitParm_t *aFormat, SdfStreamRead aRead, struct sdfMovieCb_t *aContext);

void sdfIpuSetDecodeParams(sdfIpuDecodeCb_t *cb, struct sdfIpuInitParm_t *parm, SdfStreamRead hdr, struct sdfMovieCb_t *leng);

void sdfIpuAttachTask(sdfIpuDecodeCb_t *cb);

s32 SleepThread(void);

extern s32 sdfCdTrayMode;

extern s32 sdfFileSemaId;

extern u8 sdfFileDefaultSrcType;

extern u8 sdfFileCmdDefaultDevice;

extern u8 sdfFileCmdThreadPrioCheckCounter;

extern u32 p2sdPcmCallback;

extern u32 p2sdPcmState;

extern u32 p2sdPcmBusy;

extern sdfIpuDecodeCb_t *sdfIpuTaskLink;

typedef struct sdfHostPrefix_t {
    s8 b[7];
} sdfHostPrefix_t;

typedef u8 sdfDevOperationCode_t;

typedef s8 sdfDevStateCode_t;

/* Complete 0x40-byte request owner. sdfFileCmd_t describes only its opaque
 * 0x30-byte prefix for consumers outside the device queue implementation. */
typedef struct sdfFileCmdHandle_t {
    struct sdfFileCmdHandle_t *next;
    struct sdfFileCmdHandle_t *prev;
    struct sdfFileCmdHandle_t *deviceNext;
    struct sdfFileCmdHandle_t *devicePrev;
    char *filename;
    u8 device;
    sdfDevOperationCode_t cmd;
    sdfDevStateCode_t stat;
    u8 phase;
    s32 offset;
    s32 length;
    void *buffer;
    s32 cmdParm;
    s32 fd;
    s32 errorCode;
    s32 currentOffset;
    u8 remainder[4];
    void (*callback)(struct sdfFileCmdHandle_t *, s32, saddr_t, s32, s32);
    s32 userData;
} sdfFileCmdHandle_t;

enum SdfFileIOCmdStat {
    SDF_FILE_CMD_STAT_INIT = 0,
    SDF_FILE_CMD_STAT_DORMANT = 1,
    SDF_FILE_CMD_STAT_OPEN = 2,
    SDF_FILE_CMD_STAT_SEEK = 3,
    SDF_FILE_CMD_STAT_READ = 4,
    SDF_FILE_CMD_STAT_WRITE = 5,
    SDF_FILE_CMD_STAT_CLOSE = 6,
    SDF_FILE_CMD_STAT_SUSPEND = 7,
    SDF_FILE_CMD_STAT_DONE = 8,
    SDF_FILE_CMD_STAT_ERROR = 9,
    SDF_FILE_CMD_STAT_CANCEL = 10
};

enum SdfFileIOCmd {
    SDF_FILE_CMD_NONE = 0,
    SDF_FILE_CMD_READOPEN = 1,
    SDF_FILE_CMD_WRITEOPEN = 2,
    SDF_FILE_CMD_SEEK = 3,
    SDF_FILE_CMD_FILESIZE = 4,
    SDF_FILE_CMD_READ = 5,
    SDF_FILE_CMD_WRITE = 6,
    SDF_FILE_CMD_CLOSE = 7,
    SDF_FILE_CMD_READFILE = 8,
    SDF_FILE_CMD_WRITEFILE = 9
};

typedef struct sdfClock_t {
    u8 pad00;
    u8 sec;
    u8 min;
    u8 hour;
    u8 day;
    u8 month;
    u16 year;
} sdfClock_t;

enum sdf_tex_resource {
    SDF_TEX_RESOURCE_FREE = 0,
    SDF_TEX_RESOURCE_FRAME = 1,
    SDF_TEX_RESOURCE_TEXTURE = 2,
    SDF_TEX_RESOURCE_CLUT = 3,
    SDF_TEX_RESOURCE_KNOWN_MODE_COUNT = 4
};

/* One tag in the to-IPU chain. Written as two quadword halves rather than by
 * field, which is what the single 64-bit store in sdfIpuSetStreamChunk needs. */
struct sdfIpuDmaTag_t {
    u64 control;
    u64 reserved;
};

enum sdf_stream_output {
    SDF_STREAM_OUTPUT_RGBA32 = 0,
    SDF_STREAM_OUTPUT_RGBA16 = 1,
    SDF_STREAM_OUTPUT_RGBA16_DITHERED = 2
};

typedef struct sdfStreamFrameHeader_t {
    u8 reserved[8];
    u16 width;
    u16 height;
    u32 cycleLength;
} sdfStreamFrameHeader_t;

enum sdf_stream_playback {
    SDF_STREAM_PLAYBACK_INITIAL = 0,
    SDF_STREAM_PLAYBACK_FIRST_COMPLETION = 1,
    SDF_STREAM_PLAYBACK_CADENCED = 2
};

extern s32 DIntr();

#define SDK_SKIP_EIntr
#define SDK_SKIP_EnableDmac
#include "sdk_libkernl.h"

extern void *memcpy(void *, const void *, size_t);

extern s8 sdfPrintIopFlag;

extern char sdfPrintStrFmt[];

extern char sdfFileCdromPrefix[];

extern sdfHostPrefix_t sdfFileHostPrefix[];

extern u8 sdfFileHddPrefix[];

extern char sdfFileCdromName[];

extern void EIntr(void);

extern sdfFileCmdHandle_t *sdfFileCmdActiveHead;

extern sdfFileCmdHandle_t *sdfFileCmdFreeList;

extern s16 sdfFileCmdFreeCount;

extern s16 sdfFileCmdCount;

extern sdfFileCmdHandle_t *sdfFileCmdCacheHead;

extern sdfFileCmdHandle_t *sdfFileCmdCacheTail;

extern char sdfFileCmdCdromPrefix[];

extern char sdfFileCmdHostPrefix[];

extern char sdfFileCmdPfsPrefix[];

extern u16 sdfFileCmdDefaultCreateMode;

extern s32 sdfFileCmdThreadPrio;

extern u8 sdfDev9Ready;

extern u8 sdfCdClock;

extern sdfClock_t sdfCdClockCache;

extern u32 sdfSndSendCmd_cmd_id;

extern void FlushCache(s32);

extern s32 p2sdSmgHandle;

extern char sdfDumpAddrFmt[];

extern char sdfDumpPadStr[];

extern char sdfDumpMidGapStr[];

extern char sdfDumpByteSepStr[];

extern char sdfDumpByteFmt[];

extern char sdfDumpLineBuf[];

extern char sdfDumpEndStr[];

extern char sdfVifMaskedName[];

extern char sdfVifXferOkName[];

extern char sdfDumpIndentStr[];

extern char sdfDumpWordFmt[];


extern char sdfGsMemTypeFmt[];

extern sdfIpuDecodeCb_t *sdfIpuQueueHead;

extern sdfIpuDecodeCb_t *sdfIpuQueueTail;

void *sdfAllocAuto(s32 size);

extern void * memset(void *, s32, size_t);

extern sdfGsMemSlice_t *sdfGsMemAllocRect(s32, s32, s32, s32, s32);

void sdfFreeAuto(void *data);

extern s32 sceIpuSync(s32, s32);

extern sdfIpuDecodeCb_t *sdfCurrIpuDecodeCb;

extern void sdfImageSetChainReleaseMem(saddr_t);

extern void sdfGsMemFreeDelayed(sdfGsMemSlice_t *aHandle);

/* DDS keeps its own name: the shorter one shifts DDS1 code_001DEFE0. */
extern sdfIpuDecodeCb_t *sdfIpuPendingCb;

extern s32 sdfIpuDecode(sdfIpuDecodeCb_t *);

extern void sceIpuStopDMA(sdfIpuDmaRegs_t *);
extern void sceIpuRestartDMA(sdfIpuDmaRegs_t *);

extern s32 sdfIpuDecodeThId;

extern s32 iWakeupThread(s32 threadId);

extern s32 sdfIpuDoneFrame;

u32 sdfTexGetPixelAddr(sdfTexHandle_t *aHandle);

extern s32 sdfFromIpuDmacHandlerHandle;

extern s32 sdfToIpuDmacHandlerHandle;

extern void sceIpuInit(void);

extern s32 DmacAddHandler(s32, s32, void *, s32, s32);

extern void EnableDmac(s32);

extern s32 CreateThreadSimple(void *entryAddress, void *workspace, s32 stackBytes, s32 priority);

extern u8 sdfIpuDecodeThStack[];

extern void sdfIpuRemoveHandler(void *);

extern s32 DisableDmac(s32);

extern s32 (*p2sdSmgBuf)(void);

void sdfIpuExecQueue(s32 critical);

void sdfIpuMakeBltPacket(sdfIpuDecodeCb_t *cb);

/* Remove a queued node without clearing its old links; the caller may own the interrupt guard. */
void sdfIpuDetachQueue(sdfIpuDecodeCb_t *cb, s32 critical) {
    s32 interruptsEnabled = 0;
    sdfIpuDecodeCb_t *previousNode;
    sdfIpuDecodeCb_t *nextNode;
    if (critical == 0) {
        interruptsEnabled = DIntr();
    }
    if (cb->queueLinked != 0) {
        previousNode = cb->prev;
        nextNode = cb->streamNext;
        if (previousNode == 0) {
            sdfIpuQueueHead = nextNode;
        } else {
            previousNode->streamNext = nextNode;
        }
        if (nextNode == 0) {
            sdfIpuQueueTail = previousNode;
        } else {
            nextNode->prev = previousNode;
        }
        cb->queueLinked = 0;
    }
    if (critical == 0 && interruptsEnabled != 0) {
        EIntr();
    }
}

/* Move a node to the queue tail, using one interrupt guard for unlink plus append. */
void sdfIpuAttachQueue(sdfIpuDecodeCb_t *cb, s32 critical) {
    s32 interruptsEnabled = 0;
    if (critical == 0) {
        interruptsEnabled = DIntr();
    }
    if (cb->queueLinked != 0) {
        sdfIpuDetachQueue(cb, 1);
    }
    cb->queueLinked = 1;
    if (sdfIpuQueueTail == 0) {
        sdfIpuQueueHead = cb;
    } else {
        sdfIpuQueueTail->streamNext = cb;
    }
    cb->prev = sdfIpuQueueTail;
    cb->streamNext = 0;
    sdfIpuQueueTail = cb;
    if (critical == 0 && interruptsEnabled != 0) {
        EIntr();
    }
}

void sdfIpuAttachTask(sdfIpuDecodeCb_t *cb)
{
    sdfIpuDecodeCb_t **link = &sdfIpuTaskLink;
    sdfIpuDecodeCb_t *cur;

    for (cur = *link; cur != NULL; cur = *link) {
        link = &cur->next;
    }
    *link = cb;
    cb->next = NULL;
}

void sdfIpuDetachTask(sdfIpuDecodeCb_t *cb)
{
    sdfIpuDecodeCb_t **link = &sdfIpuTaskLink;
    sdfIpuDecodeCb_t *cur;

    for (cur = *link; cur != NULL; cur = *link) {
        if (cur == cb) {
            *link = cur->next;
            return;
        }
        link = &cur->next;
    }
}

/* Allocate two width-by-height frames, using four bytes per pixel for mode zero, two otherwise. */
void sdfIpuAllocBuff(sdfIpuDecodeCb_t *cb) {
    s32 bytesPerPixel = 4;
    s32 size;
    if (cb->rgbMode != SDF_STREAM_OUTPUT_RGBA32) {
        bytesPerPixel = 2;
    }
    size = cb->width * cb->height;
    size *= bytesPerPixel;
    cb->buffSize = size;
    cb->frameBuffers[0] = sdfAllocAuto(size);
    cb->frameBuffers[1] = sdfAllocAuto(size);
}

/* Each 16-byte DMA tag holds control/address bits and a zero reserved half. */
void sdfIpuMakeToIpuDmaTag(sdfIpuDecodeCb_t *cb, u8 *data, s32 leng) {
    s32 remaining = (leng + 15) & ~15;
    struct sdfIpuDmaTag_t *tag;
    addr_t address;
    tag = sdfAllocAuto(((remaining + 0xFFEFF) / 0xFFF00) * 16);
    cb->inputDmaChain = tag;
    address = EE_PHYSICAL(EE_ADDR(data));
    do {
        s32 chunk = remaining > 0xFFF00 ? 0xFFF00 : remaining;
        u32 id;
        remaining -= chunk;
        id = remaining != 0 ? 3 : 0;
        tag->control = ((u16)(chunk >> 4) | (id << 28)) | ((u64)address << 32);
        tag->reserved = 0;
        address += chunk;
        tag++;
    } while (remaining > 0);
    FlushCache(0);
}

/* Build both frame buffers' GIF upload packets: a BITBLTBUF header followed
 * by one transfer per 16x16 tile. */
void sdfIpuMakeBltPacket(sdfIpuDecodeCb_t *cb)
{
    s32 pixelMode;
    u32 tileQwords;
    s32 width;
    s32 height;
    s32 bufferWidth;
    s32 remainingTiles;
    s32 packetBytes;
    s32 sourceStride;
    s32 bufferIndex;
    s32 x;
    s32 y;
    s32 endOfPacket;
    addr_t sourceAddress;
    u64 bitblt;
    u32 sourceTag;
    u64 imageTag;
    u64 *packet;

    if (cb->rgbMode == 0) {
        pixelMode = 0;
        tileQwords = 64;
    } else {
        pixelMode = 2;
        tileQwords = 32;
    }
    width = cb->width;
    height = cb->height;
    /* Retail shares this count across both packet buffers. */
    remainingTiles = (cb->width >> 4) * (cb->height >> 4);
    bufferWidth = width >> 6;
    if (bufferWidth == 0) {
        bufferWidth = 1;
    }
    bitblt = ((u64)pixelMode << 56) | ((u64)bufferWidth << 48);
    sourceTag = tileQwords | 0x30000000;
    imageTag = (u64)(tileQwords | 0x50000000) << 32;
    sourceStride = tileQwords << 4;
    packetBytes = remainingTiles * 0x60 + 0x50;
    cb->transferPacketBytes = packetBytes;

    bufferIndex = 0;
    do {
        packet = sdfAllocAuto(packetBytes);
        cb->transferPacketBuffers[bufferIndex] = packet;
        sourceAddress = EE_PHYSICAL(EE_ADDR(cb->frameBuffers[bufferIndex]));

        packet[0] = 0x10000003;
        packet[1] = (u64)0x50000003 << 32;
        packet[4] = bitblt | ((u64)((u32)cb->resourceWord >> 6) << 32);
        packet[5] = 0x50;
        packet[6] = ((u64)16 << 32) | 16;
        packet[7] = 0x52;
        packet[2] = ((u64)0x10000000 << 32) | 2;
        packet[3] = 0xE;
        packet += 8;

        endOfPacket = 0;
        y = 0;
        do {
            x = 0;
            do {
                remainingTiles--;
                if (remainingTiles == 0) {
                    endOfPacket = 1;
                }
                packet[0] = 0x10000004;
                packet[1] = (u64)0x50000004 << 32;
                packet[5] = 0x51;
                packet[4] = ((u64)y << 48) | ((u64)x << 32);
                packet[8] = (u64)(endOfPacket << 15) | (u64)tileQwords | ((u64)2 << 58);
                packet[10] = sourceTag | ((u64)sourceAddress << 32);
                sourceAddress += sourceStride;
                packet[2] = ((u64)0x10000000 << 32) | 2;
                packet[3] = 0xE;
                packet[6] = 0;
                packet[7] = 0x53;
                packet[9] = 0;
                packet[11] = imageTag;
                x += 16;
                packet += 12;
            } while (x < width);
            y += 16;
        } while (y < height);
        bufferIndex++;
    } while (bufferIndex != 2);
}

/* Clear the decoder, choose 32-bit/16-bit/dithered output and copy playback options. */
void sdfIpuInitCb(sdfIpuDecodeCb_t *cb, struct sdfIpuInitParm_t *parm) {
    memset(cb, 0, 0x8C);
    if (parm->rgbMode == 0) {
        cb->rgbMode = SDF_STREAM_OUTPUT_RGBA32;
    } else if (parm->ditherMode == 0) {
        cb->rgbMode = SDF_STREAM_OUTPUT_RGBA16;
    } else {
        cb->rgbMode = SDF_STREAM_OUTPUT_RGBA16_DITHERED;
    }
    cb->loopMode = parm->loopMode;
    cb->fps = parm->fps;
}

/* Decode the frame header, allocate its buffers, then queue the bytes after that header. */
void sdfIpuSetDecodeParams(sdfIpuDecodeCb_t *cb, struct sdfIpuInitParm_t *parm, SdfStreamRead hdr, struct sdfMovieCb_t *leng) {
    saddr_t sourceSize = EE_ADDR_S(leng);
    s32 interruptsEnabled;
    u8 *frameBytes = (u8 *)hdr;
    sdfIpuInitCb(cb, parm);
    cb->width = ((sdfStreamFrameHeader_t *)frameBytes)->width;
    cb->frameCount = ((sdfStreamFrameHeader_t *)frameBytes)->cycleLength;
    cb->height = ((sdfStreamFrameHeader_t *)frameBytes)->height;
    sdfIpuAllocBuff(cb);
    sdfIpuMakeToIpuDmaTag(cb, frameBytes + 0x10, sourceSize - 0x10);
    interruptsEnabled = DIntr();
    sdfIpuAttachQueue(cb, 0);
    if (interruptsEnabled != 0) {
        EIntr();
    }
    sdfIpuExecQueue(0);
}

/* Store the callback and movie context, then allocate the feed ring and leading mirror bytes. */
void sdfIpuStrmDecodeInit(sdfIpuDecodeCb_t *node, struct sdfIpuInitParm_t *format, SdfStreamRead readSource, struct sdfMovieCb_t *readContext) {
    sdfIpuInitCb(node, format);
    node->read = readSource;
    node->strmCallbackParm = readContext;
    node->active = 1;
    node->scratchBuffer = (u8 *)sdfAllocAuto(0x10100) + 0x100;
}

/* Query for a complete header before reading it. Texture dimensions are rounded
 * only when larger than one GS page; smaller dimensions remain unchanged.
 */
void sdfIpuAllocGsRect(sdfIpuDecodeCb_t *node) {
    sdfStreamFrameHeader_t header;
    s32 readStatus;
    s32 interruptsEnabled;
    if (node->headerReady != 0) {
        return;
    }
    if (node->read(node, node->strmCallbackParm, SDF_STREAM_READ_QUERY, &readStatus, 0) < sizeof(header)) {
        return;
    }
    node->headerReady = 1;
    node->read(node, node->strmCallbackParm, SDF_STREAM_READ_COPY, &header, sizeof(header));
    node->width = header.width;
    node->height = header.height;
    node->frameCount = header.cycleLength;
    if (node->resourceWord == 0) {
        s32 textureWidth = node->width;
        s32 textureHeight = node->height;
        s32 pixelFormat;
        sdfGsMemSlice_t *texture;
        if (textureWidth > 64) {
            textureWidth = (textureWidth + 63) & ~63;
        }
        if (node->rgbMode == SDF_STREAM_OUTPUT_RGBA32) {
            pixelFormat = 0;
            if (textureHeight > 32) {
                textureHeight = (textureHeight + 31) & ~31;
            }
        } else {
            pixelFormat = 2;
            if (textureHeight > 64) {
                textureHeight = (textureHeight + 63) & ~63;
            }
        }
        texture = sdfGsMemAllocRect(textureWidth, textureHeight, pixelFormat, SDF_TEX_RESOURCE_TEXTURE, 0);
        node->textureHead = texture;
        node->resourceWord = texture->addr;
    }
    sdfIpuAllocBuff(node);
    interruptsEnabled = DIntr();
    sdfIpuAttachQueue(node, 0);
    if (interruptsEnabled != 0) {
        EIntr();
    }
}

void sdfIpuFree(sdfIpuDecodeCb_t *cb) {
    s32 interruptsEnabled;
    s32 wasActive;
    u32 dmaEnable;
    interruptsEnabled = DIntr();
    wasActive = 0;
    cb->drained = 1;
    sdfIpuDetachTask(cb);
    sdfIpuDetachQueue(cb, 1);
    if (sdfCurrIpuDecodeCb == cb) {
        HW_WRITE(IPU_CTRL, 0x40000000);
        sceIpuSync(0, 0);
        dmaEnable = HW_READ(D_ENABLER);
        HW_WRITE(D_ENABLER, 0x10000);
        HW_WRITE(D4_CHCR, 1);
        HW_WRITE(D3_CHCR, 0);
        HW_WRITE(D_ENABLER, dmaEnable);
        sdfCurrIpuDecodeCb = NULL;
        wasActive = 1;
    }
    if (interruptsEnabled != 0) {
        EIntr();
    }
    sdfFreeAuto(cb->inputDmaChain);
    sdfFreeAuto(cb->frameBuffers[0]);
    sdfFreeAuto(cb->frameBuffers[1]);
    sdfImageSetChainReleaseMem(EE_ADDR_S(cb->transferPacketBuffers[0]));
    sdfImageSetChainReleaseMem(EE_ADDR_S(cb->transferPacketBuffers[1]));
    if (cb->scratchBuffer != 0) {
        sdfFreeAuto(cb->scratchBuffer - 0x100);
    }
    if (cb->textureHead != 0) {
        sdfGsMemFreeDelayed(cb->textureHead);
    }
    if (wasActive != 0) {
        sdfIpuExecQueue(0);
    }
}

/* Fill at most seven of eight 8-KiB slots. A short non-EOF query waits for more
 * data; EOF permits a partial final read. Copy the final slot's trailing bytes
 * into the prefix mirror, retaining the original copy even for a partial read.
 */
void sdfIpuHandleDmaEnd(sdfIpuDecodeCb_t *feed) {
    s32 writeSlot;
    s32 filledSlots;
    s32 availableBytes;
    addr_t destinationAddress;
    s32 endOfStream;
    filledSlots = feed->filledSlots;
    if (feed->done == 0 && filledSlots < 7) {
        do {
            writeSlot = feed->firstSlot;
            endOfStream = 0;
            destinationAddress = EE_ADDR(feed->scratchBuffer);
            writeSlot += filledSlots;
            if (writeSlot >= 8) {
                writeSlot -= 8;
            }
            destinationAddress += writeSlot << 13;
            destinationAddress = EE_UNCACHED(EE_PHYSICAL(destinationAddress));
            availableBytes = feed->read(feed, feed->strmCallbackParm, SDF_STREAM_READ_QUERY, &endOfStream, 0);
            if (availableBytes < 0x2000) {
                if (endOfStream == 0) {
                    return;
                }
                if (availableBytes > 0) {
                    feed->read(feed, feed->strmCallbackParm, SDF_STREAM_READ_COPY, EE_ADDR_AS(void *, destinationAddress), availableBytes);
                    filledSlots++;
                }
                feed->done = 1;
            } else {
                if (availableBytes == 0x2000) {
                    if (endOfStream != 0) {
                        feed->done = 1;
                    }
                }
                feed->read(feed, feed->strmCallbackParm, SDF_STREAM_READ_COPY, EE_ADDR_AS(void *, destinationAddress), 0x2000);
                filledSlots++;
            }
            if (writeSlot == 7) {
                memcpy((void *)EE_UNCACHED(EE_PHYSICAL(EE_ADDR(feed->scratchBuffer - 0x100))), EE_ADDR_AS(void *, (destinationAddress + 0x1F00)), 0x100);
            }
            feed->filledSlots = filledSlots;
            if (feed->done != 0) {
                break;
            }
        } while (filledSlots < 7);
    }
}

/* Start IPU input from the initial chain or a feed slot; defer an empty ring. */
void sdfIpuSetToIpuDma(sdfIpuDecodeCb_t *cb) {
    if (cb->active == 0) {
        sdfIpuPendingCb = cb;
        HW_WRITE(D4_TADR, EE_PHYSICAL(EE_ADDR(cb->inputDmaChain)));
        HW_WRITE(D4_QWC, 0);
        HW_WRITE(D4_CHCR, 0x105);
    } else {
        if (cb->filledSlots == 0) {
            cb->inputDmaStartPending = 1;
            return;
        }
        sdfIpuPendingCb = cb;
        cb->inputFeedDmaInFlight = 1;
        HW_WRITE(D4_MADR, EE_PHYSICAL(EE_ADDR(cb->scratchBuffer) + (cb->firstSlot << 13)));
        HW_WRITE(D4_QWC, 0x2000 / 16);
        HW_WRITE(D4_CHCR, 0x101);
    }
    cb->inputDmaStartPending = 0;
}

/* Start DMA from the IPU into the selected buffer, then toggle the byte index. */
void sdfIpuSetFromIpuDma(sdfIpuDecodeCb_t *cb) {
    u8 bufferIndex = cb->bufferIndex;
    HW_WRITE(D3_MADR, EE_PHYSICAL(EE_ADDR(cb->frameBuffers[bufferIndex])));
    HW_WRITE(D3_QWC, cb->buffSize / 16);
    HW_WRITE(D3_CHCR, 0x100);
    cb->bufferIndex = bufferIndex ^ 1;
}

/* Read the FDEC result, then issue FDEC with an eight-bit advance; return the earlier result. */
s32 IpuWriteCommand(void) {
    s32 decodedBits;
    HW_WRITE(IPU_CMD, 0x40000000);
    sceIpuSync(0, 0);
    decodedBits = HW_READ(IPU_CMD);
    sceIpuSync(0, 0);
    HW_WRITE(IPU_CMD, 0x40000008);
    sceIpuSync(0, 0);
    return decodedBits;
}

extern u8 sdfIpuRgbModeBits[][2];

/* Residual: 18 of 114 words, first differing at +0x10. ee-gcc schedules
 * &cb->dma into the prologue exactly as retail does, then the delay-slot pass
 * moves it into the bnez slot; retail's slot holds a `$4 = 0` that stands
 * between that address and the test, which no argument of the calls inside the
 * if-body reaches, and whose live range is what pushes retail's constants off
 * $4. */
#ifdef NON_MATCHING
s32 sdfIpuDecode(sdfIpuDecodeCb_t *cb)
{
    sdfIpuDmaRegs_t *dma = &cb->dma;
    s32 decodedBits;
    s32 mode;

    if (cb->ipuCycleInitialized == 0) {
        u32 dmaEnable;

        cb->ipuCycleInitialized = 1;
        dmaEnable = HW_READ(D_ENABLER);
        HW_WRITE(D_ENABLER, 0x10000);
        HW_WRITE(D4_CHCR, 1);
        HW_WRITE(D_ENABLER, dmaEnable);
        HW_WRITE(IPU_CTRL, 0x40000000);
        sceIpuSync(0, 0);
        HW_WRITE(IPU_CMD, 0);
        sceIpuSync(0, 0);
        sdfIpuSetToIpuDma(cb);
        HW_WRITE(IPU_CTRL, 0);
        sceIpuStopDMA(dma);
    }

    sdfCurrIpuDecodeCb = cb;
    sdfIpuPendingCb = cb;
    cb->dma.outputAddress = EE_PHYSICAL(EE_ADDR(cb->frameBuffers[cb->bufferIndex]));
    cb->dma.outputQwords = cb->buffSize / 16;
    cb->dma.outputControl = 0x100;
    cb->bufferIndex ^= 1;
    cb->inputFeedEnabled = 1;
    sceIpuRestartDMA(dma);
    if (cb->strmToIpuDmaStep == 0) {
        cb->strmToIpuDmaStep = 1;
    } else {
        HW_WRITE(IPU_CMD, 0x40000020);
        sceIpuSync(0, 0);
    }
    decodedBits = IpuWriteCommand();
    HW_WRITE(IPU_CTRL, (decodedBits >> 8) & 0x00FB0000);
    mode = (decodedBits & 0x04000000) ? 0x01010000 : 0x00010000;
    HW_WRITE(IPU_CMD, 0x10000000 | ((u32)sdfIpuRgbModeBits[cb->rgbMode][0] << 27) |
                  ((u32)sdfIpuRgbModeBits[cb->rgbMode][1] << 26) | mode);
    return mode;
}
#else
INCLUDE_ASM(const s32, "sdf/sdfIpu", sdfIpuDecode);
#endif

/* Remove and advance the first list item matching the IPU cleanup test. */
void sdfIpuExecQueue(s32 critical) {
    sdfIpuDecodeCb_t *node;
    s32 interruptsEnabled = 0;
    if (sdfCurrIpuDecodeCb != NULL) {
        return;
    }
    if (critical == 0) {
        interruptsEnabled = DIntr();
    }
    node = sdfIpuQueueHead;
    while (node != 0) {
        if (node->activeBuffCount + node->imageTrans < 2 &&
            (node->active != 1 || node->filledSlots != 0)) {
            sdfIpuDetachQueue(node, 1);
            sdfIpuDecode(node);
            break;
        }
        node = node->streamNext;
    }
    if (critical == 0 && interruptsEnabled != 0) {
        EIntr();
    }
}

/* Sleep for IPU completion, advance cycle state and requeue non-drained nodes;
 * the DMA worker runs indefinitely and retains the native completion-byte gates. */
void sdfIpuDecodeThread(void) {
    sdfIpuDecodeCb_t *work;
    s32 interruptsEnabled;
    for (;;) {
        SleepThread();
        work = sdfCurrIpuDecodeCb;
        if (work == NULL) {
            continue;
        }
        sceIpuStopDMA(&work->dma);
        if (work->dma.inputQwords == 0) {
            work->inputFeedEnabled = 0;
        }
        if (work->playbackPhase == SDF_STREAM_PLAYBACK_INITIAL) {
            work->playbackPhase = SDF_STREAM_PLAYBACK_FIRST_COMPLETION;
        }
        work->activeBuffCount++;
        sdfCurrIpuDecodeCb = NULL;
        work->tickCount++;
        if (work->tickCount == work->frameCount) {
            if (work->loopMode != 0) {
                work->ipuCycleInitialized = 0;
                work->tickCount = 0;
                work->strmToIpuDmaStep = 0;
            } else {
                work->drained = 1;
            }
        }
        if (work->drained == 0) {
            interruptsEnabled = DIntr();
            sdfIpuAttachQueue(work, 0);
            if (interruptsEnabled != 0) {
                EIntr();
            }
        }
        sdfIpuExecQueue(0);
    }
}

s32 sdfIpuFromDmacHandler(void) {
    iWakeupThread(sdfIpuDecodeThId);
    EE_SYNC_EI();
    return 0;
}

s32 sdfIpuToDmacHandler(void) {
    sdfIpuDecodeCb_t *stream = sdfIpuPendingCb;
    if (stream != NULL) {
        sdfIpuDoneFrame = stream->frame;
        sdfIpuPendingCb = NULL;
        if (stream->inputFeedEnabled != 0 && stream->active == 1) {
            if (stream->inputFeedDmaInFlight != 0) {
                stream->inputFeedDmaInFlight = 0;
                stream->firstSlot++;
                if (stream->firstSlot == 8) {
                    stream->firstSlot = 0;
                }
                stream->filledSlots--;
            }
            sdfIpuHandleDmaEnd(stream);
            sdfIpuSetToIpuDma(stream);
        }
    }
    EE_SYNC_EI();
    return 0;
}

void *sdfIpuGetActivePacket(sdfIpuDecodeCb_t *cb)
{
    void *packet = NULL;

    if (cb->activeBuffCount != 0) {
        packet = cb->frameBuffers[cb->getBuffId];
    }
    return packet;
}

void *sdfIpuGetActiveFrame(sdfIpuDecodeCb_t *cb)
{
    void *frame = NULL;

    if (cb->activeBuffCount != 0) {
        frame = cb->transferPacketBuffers[cb->getBuffId];
    }
    return frame;
}

/* Consume a pending buffer when present, but always advance processed; loop only on exact equality. */
void sdfIpuFinishActiveFrame(sdfIpuDecodeCb_t *cb) {
    s32 interruptsEnabled = DIntr();
    s32 pendingBuffers = cb->activeBuffCount;
    s32 remainingBuffers = pendingBuffers - 1;
    if (pendingBuffers > 0) {
        cb->activeBuffCount = remainingBuffers;
        cb->getBuffId ^= 1;
        cb->imageTrans++;
    }
    cb->frame++;
    if (cb->frame == cb->frameCount && cb->loopMode != 0) {
        cb->frame = 0;
    }
    if (interruptsEnabled != 0) {
        EIntr();
    }
    sdfIpuExecQueue(0);
}

extern void sdfImageSetChainTag(u32 aHead, void *aTail);

/* Hand the finished buffer's packet to the image set, building it on first use. */
s32 sdfIpuSetChainTag(sdfIpuDecodeCb_t *cb)
{
    u32 *packet;

    if (cb->activeBuffCount == 0) {
        return 0;
    }
    packet = EE_ADDR_AS(u32 *, (cb->getBuffId * 4 + EE_ADDR_S(cb) + 0x28));
    if (*packet == 0) {
        sdfIpuMakeBltPacket(cb);
    }
    sdfImageSetChainTag(*packet, EE_ADDR_AS(void *, (*packet + cb->transferPacketBytes - 0x10)));
    sdfIpuFinishActiveFrame(cb);
    return 1;
}

/* DDS skips the cadence and the refill while the movie is paused. */
extern u32 sdfIpuPaused;

void sdfIpuRunDecode(s32 frames)
{
    sdfIpuDecodeCb_t *node = sdfCurrIpuDecodeCb;
    s32 cadence;
    s32 interruptsEnabled;

    if (node != NULL && node->inputDmaStartPending != 0) {
        sdfIpuSetToIpuDma(node);
    }
    node = sdfIpuTaskLink;
    while (node != NULL) {
        if (node->imageTrans != 0) {
            node->imageTrans--;
        }
        cadence = node->playbackCadenceRemainder;
        switch (node->playbackPhase) {
        case SDF_STREAM_PLAYBACK_FIRST_COMPLETION:
            sdfIpuSetChainTag(node);
            node->playbackPhase = SDF_STREAM_PLAYBACK_CADENCED;
            node->playbackCadenceRemainder = node->fps;
            break;
        case SDF_STREAM_PLAYBACK_CADENCED:
            if (sdfIpuPaused != 1) {
                cadence += node->fps;
                if (cadence >= frames) {
                    if (sdfIpuSetChainTag(node) != 0) {
                        cadence -= frames;
                    }
                }
                node->playbackCadenceRemainder = cadence;
            }
            break;
        }
        if (node->active == 1) {
            sdfIpuAllocGsRect(node);
            if (sdfIpuPaused != 1) {
                if (node->filledSlots < 8) {
                    interruptsEnabled = DIntr();
                    sdfIpuHandleDmaEnd(node);
                    if (interruptsEnabled != 0) {
                        EIntr();
                    }
                }
                node->read(node, node->strmCallbackParm, SDF_STREAM_READ_RESUME, NULL, 0);
            }
        }
        node = node->next;
    }
    sdfIpuExecQueue(0);
}

void sdfIpuStartDecodeTask(sdfIpuDecodeCb_t *aNode, struct sdfIpuInitParm_t *aFormat, SdfStreamRead aRead, struct sdfMovieCb_t *aContext, s32 aResourceWord)
{
    sdfIpuSetDecodeParams(aNode, aFormat, aRead, aContext);
    aNode->resourceWord = aResourceWord;
    sdfIpuAttachTask(aNode);
}

void sdfIpuSetOutputTex(sdfIpuDecodeCb_t *node, struct sdfIpuInitParm_t *params, u8 *sourceData, s32 sourceSize, sdfTexHandle_t *source) {
    struct sdfIpuInitParm_t streamParams = *params;
    switch (source->pixelFormat) {
    case 0:
        streamParams.rgbMode = 0;
        break;
    case 2:
        streamParams.rgbMode = 1;
        break;
    }
    sdfIpuStartDecodeTask(node, &streamParams, (SdfStreamRead)sourceData, EE_ADDR_AS(struct sdfMovieCb_t *, sourceSize), sdfTexGetPixelAddr(source));
}

void movieAttachDecodeTask(sdfIpuDecodeCb_t *aNode, struct sdfIpuInitParm_t *aFormat, SdfStreamRead aRead, struct sdfMovieCb_t *aContext, s32 aResourceWord)
{
    sdfIpuStrmDecodeInit(aNode, aFormat, aRead, aContext);
    aNode->resourceWord = aResourceWord;
    sdfIpuAttachTask(aNode);
}

void sdfIpuInit(void) {
    s32 thread;
    sceIpuInit();
    HW_WRITE(IPU_CMD, 0x90000000);
    sdfIpuTaskLink = 0;
    sdfCurrIpuDecodeCb = NULL;
    sdfFromIpuDmacHandlerHandle = DmacAddHandler(1, 3, sdfIpuFromDmacHandler, -1, 0);
    EnableDmac(3);
    sdfToIpuDmacHandlerHandle = DmacAddHandler(1, 4, sdfIpuToDmacHandler, -1, 0);
    EnableDmac(4);
    thread = CreateThreadSimple((void *)sdfIpuDecodeThread, sdfIpuDecodeThStack, 0x800, 0x46);
    sdfIpuDecodeThId = thread;
    _StartThread(thread, 0);
}

s32 sdfIpuClose(void) {
    if (sdfFromIpuDmacHandlerHandle != 0) {
        sdfIpuRemoveHandler(EE_ADDR_AS(void *, sdfFromIpuDmacHandlerHandle));
        sdfFromIpuDmacHandlerHandle = 0;
    }
    DisableDmac(3);
    if (sdfToIpuDmacHandlerHandle != 0) {
        sdfIpuRemoveHandler(EE_ADDR_AS(void *, sdfToIpuDmacHandlerHandle));
        sdfToIpuDmacHandlerHandle = 0;
    }
    return DisableDmac(4);
}

INCLUDE_SDATA(const s32, "sdf/sdfIpu", sdfIpuPaused);

