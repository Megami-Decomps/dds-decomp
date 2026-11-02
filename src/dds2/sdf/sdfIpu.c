#include "sdk_ps2.h"
#include "common.h"
#include "sdf_handler_node.h"
#include "eeregs.h"
#include "snd_command_packet.h"
#include "mnu_voice_resource_path_api.h"
#include "sdf_heap_api.h"
#include "sdf_packed_resource.h"
#include "sdf_pac_reloc.h"
#include "sdf.h"
#include "sdf_gs_mem_delayed_free_api.h"
#include "sdf_semaphore_api.h"
#include "sdf_movie_stream.h"
#include "sdf_packet_alloc.h"
#include "sdf_packet_list.h"
#include "sdf_gs_packet.h"
#include "sdf_texture_lists.h"
#include "sdf_draw.h"
#include "sdf_resource_list_item.h"
#include "ee_mmi.h"
#include "sdf_stream_read.h"
#include "sdf_stream_read_api.h"
#include "sdf_dev_event.h"
#include "sdf_dev_state.h"
#include "mdl_object_stream.h"
#include "sdf_texture_offset_list.h"
#include "sdf_texture_file.h"
#include "sdf_texture_queue.h"
#include "sdf_stream_input_dma.h"
#include "sdf_sound_rpc_records.h"
#include "snd_midi_state_views.h"
#include "mnu_title_sound.h"
#include "sdf_movie_draw_packet.h"

extern s32 sdfIpuDecodeThId;

#define SDF_STREAM_NODE_BYTES 0x8C

#define SDF_STREAM_SCRATCH_BYTES 0x10100

#define SDF_STREAM_PREFIX_BYTES 0x100

#define SDF_STREAM_SLOT_BYTES 0x2000

#define SDF_STREAM_SLOT_SHIFT 13

#define SDF_STREAM_RING_SLOTS 8

#define SDF_STREAM_MAX_FILLED 7

#define SDF_STREAM_TRAILER_START 0x1F00

#define SDF_STREAM_QWORD_BYTES 16

#define SDF_STREAM_PIXEL_BYTES_WIDE 4

#define SDF_STREAM_PIXEL_BYTES_NARROW 2

#define SDF_STREAM_PAGE_WIDTH 64

#define SDF_STREAM_PAGE_WIDTH_MASK 63

#define SDF_STREAM_WIDE_PAGE_HEIGHT 32

#define SDF_STREAM_WIDE_HEIGHT_MASK 31

#define SDF_STREAM_NARROW_PAGE_HEIGHT 64

#define SDF_STREAM_NARROW_HEIGHT_MASK 63

#define SDF_STREAM_PSMCT32 0

#define SDF_STREAM_PSMCT16 2

#define SDF_EE_PHYSICAL_MASK 0x0FFFFFFF

#define SDF_EE_UNCACHED_BASE 0x20000000

#define SDF_IPU_DMA_START 0x100

#define SDF_IPU_DMA_NORMAL_START 0x101

#define SDF_IPU_DMA_CHAIN_START 0x105

#define SDF_IPU_FDEC 0x40000000

#define SDF_IPU_FDEC_BYTE 0x40000008

extern sdfIpuDecodeCb_t *sdfCurrIpuDecodeCb;

extern sdfIpuDecodeCb_t *sdfIpuTaskLink;

extern sdfIpuDecodeCb_t *sdfIpuInputStream;

extern s32 D_004391F4;

extern sdfIpuDecodeCb_t *sdfIpuQueueHead;

extern sdfIpuDecodeCb_t *sdfIpuQueueTail;

extern void sdfIpuExecQueue(s32 skipInterruptGuard);

extern void sdfIpuDecode(sdfIpuDecodeCb_t *);

/* Remove a queued node without clearing its old links; the caller may own the interrupt guard. */
void sdfIpuDetachQueue(sdfIpuDecodeCb_t *node, s32 skipInterruptGuard) {
    s32 interruptsEnabled = 0;
    sdfIpuDecodeCb_t *previousNode;
    sdfIpuDecodeCb_t *nextNode;
    if (skipInterruptGuard == 0) {
        interruptsEnabled = DIntr();
    }
    if (node->queued != 0) {
        previousNode = node->streamPrev;
        nextNode = node->streamNext;
        if (previousNode == 0) {
            sdfIpuQueueHead = nextNode;
        } else {
            previousNode->streamNext = nextNode;
        }
        if (nextNode == 0) {
            sdfIpuQueueTail = previousNode;
        } else {
            nextNode->streamPrev = previousNode;
        }
        node->queued = 0;
    }
    if (skipInterruptGuard == 0 && interruptsEnabled != 0) {
        EIntr();
    }
}

/* Move a node to the queue tail, using one interrupt guard for unlink plus append. */
void sdfIpuAttachQueue(sdfIpuDecodeCb_t *node, s32 skipInterruptGuard) {
    s32 interruptsEnabled = 0;
    if (skipInterruptGuard == 0) {
        interruptsEnabled = DIntr();
    }
    if (node->queued != 0) {
        sdfIpuDetachQueue(node, 1);
    }
    node->queued = 1;
    if (sdfIpuQueueTail == 0) {
        sdfIpuQueueHead = node;
    } else {
        sdfIpuQueueTail->streamNext = node;
    }
    node->streamPrev = sdfIpuQueueTail;
    node->streamNext = 0;
    sdfIpuQueueTail = node;
    if (skipInterruptGuard == 0 && interruptsEnabled != 0) {
        EIntr();
    }
}

/* Walk link addresses to append to the independent singly-linked sound list. */
void sdfIpuAttachTask(sdfIpuDecodeCb_t *node) {
    sdfIpuDecodeCb_t **tailLink = &sdfIpuTaskLink;
    sdfIpuDecodeCb_t *currentNode = *tailLink;
    if (currentNode != NULL) {
        tailLink = &currentNode->next;
        while ((currentNode = *tailLink) != NULL) {
            tailLink = &currentNode->next;
        }
    }
    *tailLink = node;
    node->next = NULL;
}

/* Unlink the first matching sound node; absence leaves the list unchanged. */
void sdfIpuDetachTask(sdfIpuDecodeCb_t *node) {
    sdfIpuDecodeCb_t **nodeLink = &sdfIpuTaskLink;
    sdfIpuDecodeCb_t *currentNode = *nodeLink;
    while (currentNode != NULL) {
        if (currentNode == node) {
            *nodeLink = currentNode->next;
            return;
        }
        nodeLink = &currentNode->next;
        currentNode = *nodeLink;
    }
}

/* Allocate two width-by-height frames, using four bytes per pixel for mode zero, two otherwise. */
void sdfIpuAllocBuff(sdfIpuDecodeCb_t *node) {
    s32 bytesPerPixel = SDF_STREAM_PIXEL_BYTES_WIDE;
    s32 size;

    if (node->audioMode != SDF_STREAM_AUDIO_DISABLED) {
        bytesPerPixel = SDF_STREAM_PIXEL_BYTES_NARROW;
    }
    size = node->width * node->height;
    size *= bytesPerPixel;
    node->bufferSize = size;
    node->frameBuffers[0] = sdfAllocAuto(size);
    node->frameBuffers[1] = sdfAllocAuto(size);
}

/* Each 16-byte DMA tag holds control/address bits and a zero reserved half. */
void sdfIpuMakeToIpuDmaTag(sdfIpuDecodeCb_t *node, u8 *source, s32 bytes) {
    s32 remaining = (bytes + 15) & ~15;
    SdfStreamInputDmaTag *tag;
    u32 address;

    tag = sdfAllocAuto(((remaining + 0xFFEFF) / 0xFFF00) * 16);
    node->inputDmaChain = tag;
    address = (u32)source & SDF_EE_PHYSICAL_MASK;
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

/* Build the two GIF upload buffers for the node's tiled frame images. */
void sdfIpuMakeBltPacket(sdfIpuDecodeCb_t *node) {
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
    u32 sourceAddress;
    u64 bitblt;
    u32 sourceTag;
    u64 imageTag;
    u64 *packet;

    if (node->audioMode == SDF_STREAM_AUDIO_DISABLED) {
        pixelMode = SDF_STREAM_PSMCT32;
        tileQwords = 64;
    } else {
        pixelMode = SDF_STREAM_PSMCT16;
        tileQwords = 32;
    }
    width = node->width;
    height = node->height;
    /* Retail shares this count across both packet buffers. */
    remainingTiles = (node->width >> 4) * (node->height >> 4);
    bufferWidth = width >> 6;
    if (bufferWidth == 0) {
        bufferWidth = 1;
    }
    bitblt = ((u64)pixelMode << 56) | ((u64)bufferWidth << 48);
    sourceTag = tileQwords | 0x30000000;
    imageTag = (u64)(tileQwords | 0x50000000) << 32;
    sourceStride = tileQwords << 4;
    packetBytes = remainingTiles * 0x60 + 0x50;
    node->transferPacketBytes = packetBytes;

    bufferIndex = 0;
    do {
        packet = sdfAllocAuto(packetBytes);
        node->transferPacketBuffers[bufferIndex] = packet;
        sourceAddress = (u32)node->frameBuffers[bufferIndex] & SDF_EE_PHYSICAL_MASK;

        packet[0] = 0x10000003;
        packet[1] = (u64)0x50000003 << 32;
        packet[4] = bitblt | ((u64)((u32)node->resourceWord >> 6) << 32);
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

/* Clear the node, map the format's two selector bytes to mode 0/1/2, and copy loop/playback modes. */
void sdfIpuInitCb(sdfIpuDecodeCb_t *node, SoundFormat *format) {
    memset(node, 0, SDF_STREAM_NODE_BYTES);
    if (format->hasAudio == 0) {
        node->audioMode = SDF_STREAM_AUDIO_DISABLED;
    } else {
        if (format->stereo == 0) {
            node->audioMode = SDF_STREAM_AUDIO_MONO;
        } else {
            node->audioMode = SDF_STREAM_AUDIO_STEREO;
        }
    }
    node->loopMode = format->loopMode;
    node->playbackCadenceStep = format->playbackCadenceStep;
}

/* Decode the frame header, allocate its buffers, then queue the bytes after that header. */
void sdfIpuSetDecodeParams(sdfIpuDecodeCb_t *node, SoundFormat *format, u8 *sourceAddress, s32 sourceSize) {
    s32 interruptsEnabled;
    u8 *frameBytes = sourceAddress;
    sdfIpuInitCb(node, format);
    node->width = ((SdfStreamFrameHeader *)frameBytes)->width;
    node->cycleLength = ((SdfStreamFrameHeader *)frameBytes)->cycleLength;
    node->height = ((SdfStreamFrameHeader *)frameBytes)->height;
    sdfIpuAllocBuff(node);
    sdfIpuMakeToIpuDmaTag(node, frameBytes + SDF_STREAM_FRAME_HEADER_BYTES, sourceSize - SDF_STREAM_FRAME_HEADER_BYTES);
    interruptsEnabled = DIntr();
    sdfIpuAttachQueue(node, 0);
    if (interruptsEnabled != 0) {
        EIntr();
    }
    sdfIpuExecQueue(0);
}

/* Store the callback and movie context, then allocate the feed ring and leading mirror bytes. */
void sdfIpuStrmDecodeInit(sdfIpuDecodeCb_t *node, SoundFormat *format, SdfStreamReadCallback readSource, movieStream_t *readContext) {
    sdfIpuInitCb(node, format);
    node->read = readSource;
    node->readContext = readContext;
    node->active = 1;
    node->scratchBuffer = (u8 *)sdfAllocAuto(SDF_STREAM_SCRATCH_BYTES) + SDF_STREAM_PREFIX_BYTES;
}

extern sdfGsMemSlice_t *sdfGsMemAllocRect(s32, s32, s32, s32, s32);

/* Query for a complete header before reading it. Texture dimensions are rounded
 * only when larger than one GS page; smaller dimensions remain unchanged.
 */
void sdfIpuAllocGsRect(sdfIpuDecodeCb_t *node) {
    SdfStreamFrameHeader header;
    s32 readStatus;
    s32 interruptsEnabled;

    if (node->headerReady != 0) {
        return;
    }
    if (node->read(node, node->readContext, SDF_STREAM_READ_QUERY, &readStatus, 0) < sizeof(header)) {
        return;
    }
    node->headerReady = 1;
    node->read(node, node->readContext, SDF_STREAM_READ_COPY, &header, sizeof(header));
    node->width = header.width;
    node->height = header.height;
    node->cycleLength = header.cycleLength;
    if (node->resourceWord == 0) {
        s32 textureWidth = node->width;
        s32 textureHeight = node->height;
        s32 pixelFormat;
        sdfGsMemSlice_t *texture;

        if (textureWidth > SDF_STREAM_PAGE_WIDTH) {
            textureWidth = (textureWidth + SDF_STREAM_PAGE_WIDTH_MASK) & ~SDF_STREAM_PAGE_WIDTH_MASK;
        }
        if (node->audioMode == SDF_STREAM_AUDIO_DISABLED) {
            pixelFormat = SDF_STREAM_PSMCT32;
            if (textureHeight > SDF_STREAM_WIDE_PAGE_HEIGHT) {
                textureHeight = (textureHeight + SDF_STREAM_WIDE_HEIGHT_MASK) & ~SDF_STREAM_WIDE_HEIGHT_MASK;
            }
        } else {
            pixelFormat = SDF_STREAM_PSMCT16;
            if (textureHeight > SDF_STREAM_NARROW_PAGE_HEIGHT) {
                textureHeight = (textureHeight + SDF_STREAM_NARROW_HEIGHT_MASK) & ~SDF_STREAM_NARROW_HEIGHT_MASK;
            }
        }
        texture = sdfGsMemAllocRect(textureWidth, textureHeight, pixelFormat, SDF_TEX_RESOURCE_TEXTURE, 0);
        node->textureHead = texture;
        node->resourceWord = texture->word;
    }
    sdfIpuAllocBuff(node);
    interruptsEnabled = DIntr();
    sdfIpuAttachQueue(node, 0);
    if (interruptsEnabled != 0) {
        EIntr();
    }
}

extern void sdfImageSetChainReleaseMem(void *);

void sdfIpuFree(sdfIpuDecodeCb_t *node) {
    s32 interruptsEnabled;
    s32 wasActive;
    u32 dmaEnable;

    interruptsEnabled = DIntr();
    wasActive = 0;
    node->drained = 1;
    sdfIpuDetachTask(node);
    sdfIpuDetachQueue(node, 1);
    if (sdfCurrIpuDecodeCb == node) {
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
    sdfFreeAuto(node->inputDmaChain);
    sdfFreeAuto(node->frameBuffers[0]);
    sdfFreeAuto(node->frameBuffers[1]);
    sdfImageSetChainReleaseMem(node->transferPacketBuffers[0]);
    sdfImageSetChainReleaseMem(node->transferPacketBuffers[1]);
    if (node->scratchBuffer != 0) {
        sdfFreeAuto(node->scratchBuffer - 0x100);
    }
    if (node->textureHead != 0) {
        sdfGsMemFreeDelayed(node->textureHead);
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
    SdfStreamReadResult availableBytes;
    u32 destinationAddress;
    s32 endOfStream;

    filledSlots = feed->filledSlots;
    if (feed->done == 0 && filledSlots < SDF_STREAM_MAX_FILLED) {
        do {
            writeSlot = feed->firstSlot;
            endOfStream = 0;
            destinationAddress = (u32)feed->scratchBuffer;
            writeSlot += filledSlots;
            if (writeSlot >= SDF_STREAM_RING_SLOTS) {
                writeSlot -= SDF_STREAM_RING_SLOTS;
            }
            destinationAddress += writeSlot << SDF_STREAM_SLOT_SHIFT;
            destinationAddress = (destinationAddress & SDF_EE_PHYSICAL_MASK) | SDF_EE_UNCACHED_BASE;
            availableBytes = feed->read(feed, feed->readContext, SDF_STREAM_READ_QUERY, &endOfStream, 0);
            if (availableBytes < SDF_STREAM_SLOT_BYTES) {
                if (endOfStream == 0) {
                    return;
                }
                if (availableBytes > 0) {
                    feed->read(feed, feed->readContext, SDF_STREAM_READ_COPY, (void *)destinationAddress, availableBytes);
                    filledSlots++;
                }
                feed->done = 1;
            } else {
                if (availableBytes == SDF_STREAM_SLOT_BYTES) {
                    if (endOfStream != 0) {
                        feed->done = 1;
                    }
                }
                feed->read(feed, feed->readContext, SDF_STREAM_READ_COPY, (void *)destinationAddress, SDF_STREAM_SLOT_BYTES);
                filledSlots++;
            }
            if (writeSlot == SDF_STREAM_MAX_FILLED) {
                memcpy((void *)(((u32)(feed->scratchBuffer - SDF_STREAM_PREFIX_BYTES) & SDF_EE_PHYSICAL_MASK) | SDF_EE_UNCACHED_BASE), (void *)(destinationAddress + SDF_STREAM_TRAILER_START), SDF_STREAM_PREFIX_BYTES);
            }
            feed->filledSlots = filledSlots;
            if (feed->done != 0) {
                break;
            }
        } while (filledSlots < SDF_STREAM_MAX_FILLED);
    }
}

/* Start IPU input from the initial chain or a feed slot; defer an empty ring. */
void sdfIpuSetToIpuDma(sdfIpuDecodeCb_t *stream) {
    if (stream->active == 0) {
        sdfIpuInputStream = stream;
        HW_WRITE(D4_TADR, (u32)stream->inputDmaChain & SDF_EE_PHYSICAL_MASK);
        HW_WRITE(D4_QWC, 0);
        HW_WRITE(D4_CHCR, SDF_IPU_DMA_CHAIN_START);
    } else {
        if (stream->filledSlots == 0) {
            stream->inputDmaStartPending = 1;
            return;
        }
        sdfIpuInputStream = stream;
        stream->inputFeedDmaInFlight = 1;
        HW_WRITE(D4_MADR, ((u32)stream->scratchBuffer + (stream->firstSlot << SDF_STREAM_SLOT_SHIFT)) &
                              SDF_EE_PHYSICAL_MASK);
        HW_WRITE(D4_QWC, SDF_STREAM_SLOT_BYTES / SDF_STREAM_QWORD_BYTES);
        HW_WRITE(D4_CHCR, SDF_IPU_DMA_NORMAL_START);
    }
    stream->inputDmaStartPending = 0;
}

/* Start DMA from the IPU into the selected buffer, then toggle the byte index. */
void sdfIpuSetFromIpuDma(sdfIpuDecodeCb_t *stream) {
    u8 bufferIndex = stream->bufferIndex;
    HW_WRITE(D3_MADR, (u32)stream->frameBuffers[bufferIndex] & SDF_EE_PHYSICAL_MASK);
    HW_WRITE(D3_QWC, stream->bufferSize / SDF_STREAM_QWORD_BYTES);
    HW_WRITE(D3_CHCR, SDF_IPU_DMA_START);
    stream->bufferIndex = bufferIndex ^ 1;
}

/* Read the FDEC result, then issue FDEC with an eight-bit advance; return the earlier result. */
s32 IpuWriteCommand(void) {
    s32 decodedBits;
    HW_WRITE(IPU_CMD, SDF_IPU_FDEC);
    sceIpuSync(0, 0);
    decodedBits = HW_READ(IPU_CMD);
    sceIpuSync(0, 0);
    HW_WRITE(IPU_CMD, SDF_IPU_FDEC_BYTE);
    sceIpuSync(0, 0);
    return decodedBits;
}

#ifdef NON_MATCHING
#include "common.h"
#include "eeregs.h"
#include "sdf.h"
#include "sdk_libipu.h"

#define SDF_STREAM_QWORD_BYTES 16
#define SDF_EE_PHYSICAL_MASK 0x0FFFFFFF
#define SDF_IPU_DMA_START 0x100         /* CHCR STR */
#define IPU_CTRL_RST 0x40000000
#define IPU_CMD_BCLR 0x00000000         /* clear the input FIFO, bit pointer 0 */
#define IPU_CMD_FDEC_BIT32 0x40000020   /* skip the 32-bit start code */
#define IPU_CMD_IDEC 0x10000000
#define IPU_IDEC_QSC1 0x00010000
#define IPU_IDEC_DTD 0x01000000         /* the picture's DCT type is decoded */
#define SDF_STREAM_HEADER_DT 0x04000000 /* the frame header's DCT-type flag */

extern void sceIpuRestartDMA(void *);
extern void sdfIpuSetToIpuDma(sdfIpuDecodeCb_t *stream);
extern s32 IpuWriteCommand(void);
extern sdfIpuDecodeCb_t *sdfCurrIpuDecodeCb; /* the stream the IPU output DMA serves */
extern sdfIpuDecodeCb_t *sdfIpuInputStream; /* the stream the IPU input DMA serves */
extern const u8 sdfIpuAudioModeIdecBits[];          /* per audio mode: IDEC's OFM bit, then its DTE bit */

/* Decode the stream's next frame into its idle frame buffer. The first call
 * of a playback cycle resets the IPU and its input DMA; every call points the
 * output DMA at the buffer, skips the frame's start code (after the first
 * frame), and issues IDEC with the mode's output format and dithering.
 * Upstream: INCLUDE_ASM in both games; written from the retail routine.
 * The same register accesses, calls and stores in the same order. */
void sdfIpuDecode(sdfIpuDecodeCb_t *stream) {
    IpuDmaState *dma = &stream->dma;
    u8 bufferIndex;
    s32 header;
    u32 command;
    const u8 *modeBits;

    if (stream->ipuCycleInitialized == 0) {
        u32 dmaEnable;

        stream->ipuCycleInitialized = 1;
        /* Retail writes D_ENABLER's own address around the channel setup. */
        dmaEnable = HW_READ(D_ENABLER);
        HW_WRITE(D_ENABLER, 0x10000);
        HW_WRITE(D4_CHCR, 1);
        HW_WRITE(D_ENABLER, dmaEnable);
        HW_WRITE(IPU_CTRL, IPU_CTRL_RST);
        sceIpuSync(0, 0);
        HW_WRITE(IPU_CMD, IPU_CMD_BCLR);
        sceIpuSync(0, 0);
        sdfIpuSetToIpuDma(stream);
        HW_WRITE(IPU_CTRL, 0);
        sceIpuStopDMA(dma);
    }
    bufferIndex = stream->bufferIndex;
    sdfCurrIpuDecodeCb = stream;
    sdfIpuInputStream = stream;
    dma->outputAddress = (u32)stream->frameBuffers[bufferIndex] & SDF_EE_PHYSICAL_MASK;
    dma->outputQwords = stream->bufferSize / SDF_STREAM_QWORD_BYTES;
    dma->outputControl = SDF_IPU_DMA_START;
    stream->bufferIndex = bufferIndex ^ 1;
    stream->inputFeedEnabled = 1;
    sceIpuRestartDMA(dma);
    if (stream->unk65 == 0) {
        stream->unk65 = 1;
    } else {
        HW_WRITE(IPU_CMD, IPU_CMD_FDEC_BIT32);
        sceIpuSync(0, 0);
    }
    header = IpuWriteCommand();
    HW_WRITE(IPU_CTRL, (header >> 8) & 0xFB0000);
    modeBits = &sdfIpuAudioModeIdecBits[stream->audioMode * 2];
    command = ((u32)modeBits[0] << 27) | ((u32)modeBits[1] << 26) | IPU_CMD_IDEC;
    command |= (header & SDF_STREAM_HEADER_DT) ? (IPU_IDEC_DTD | IPU_IDEC_QSC1) : IPU_IDEC_QSC1;
    HW_WRITE(IPU_CMD, command);
}
#else
INCLUDE_ASM(void, "sdf/sdfIpu", sdfIpuDecode);
#endif

/* Remove and advance the first list item matching the IPU cleanup test. */
void sdfIpuExecQueue(s32 skipInterruptGuard) {
    sdfIpuDecodeCb_t *node;
    s32 interruptsEnabled = 0;

    if (sdfCurrIpuDecodeCb != NULL) {
        return;
    }
    if (skipInterruptGuard == 0) {
        interruptsEnabled = DIntr();
    }

    node = sdfIpuQueueHead;
    while (node != 0) {
        if (node->completedBufferCount + node->pendingPlaybackSubmissions < 2 &&
            (node->active != 1 || node->filledSlots != 0)) {
            sdfIpuDetachQueue(node, 1);
            sdfIpuDecode(node);
            break;
        }
        node = node->streamNext;
    }

    if (skipInterruptGuard == 0 && interruptsEnabled != 0) {
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
        work->completedBufferCount++;
        sdfCurrIpuDecodeCb = NULL;
        work->tickCount++;
        if (work->tickCount == work->cycleLength) {
            if (work->loopMode != 0) {
                work->ipuCycleInitialized = 0;
                work->tickCount = 0;
                work->unk65 = 0;
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
    EE_ENABLE_INTERRUPTS_SYNC();
    return 0;
}

s32 sdfIpuToDmacHandler(void) {
    sdfIpuDecodeCb_t *stream = sdfIpuInputStream;

    if (stream != NULL) {
        D_004391F4 = stream->playbackFrameIndex;
        sdfIpuInputStream = NULL;
        if (stream->inputFeedEnabled != 0 && stream->active == 1) {
            if (stream->inputFeedDmaInFlight != 0) {
                stream->inputFeedDmaInFlight = 0;
                stream->firstSlot++;
                if (stream->firstSlot == SDF_STREAM_RING_SLOTS) {
                    stream->firstSlot = 0;
                }
                stream->filledSlots--;
            }
            sdfIpuHandleDmaEnd(stream);
            sdfIpuSetToIpuDma(stream);
        }
    }
    EE_ENABLE_INTERRUPTS_SYNC();
    return 0;
}

/* Return the indexed earlier entry only when enabled; the byte index is unchecked. */
u32 sdfIpuGetActivePacket(SndMidiChannel *channel) {
    u32 selectedEntry = 0;
    if (channel->isEnabled != 0) {
        selectedEntry = channel->previousEntries[channel->entryIndex];
    }
    return selectedEntry;
}

/* Return the indexed current entry only when enabled; the byte index is unchecked. */
u32 sdfIpuGetActiveFrame(SndMidiChannel *channel) {
    u32 selectedEntry = 0;
    if (channel->isEnabled != 0) {
        selectedEntry = channel->currentEntries[channel->entryIndex];
    }
    return selectedEntry;
}

/* Consume a pending buffer when present, but always advance processed; loop only on exact equality. */
void sdfIpuFinishActiveFrame(sdfIpuDecodeCb_t *node) {
    s32 interruptsEnabled = DIntr();
    s32 pendingBuffers = node->completedBufferCount;
    s32 remainingBuffers = pendingBuffers - 1;
    if (pendingBuffers > 0) {
        node->completedBufferCount = remainingBuffers;
        node->transferPacketIndex ^= 1;
        node->pendingPlaybackSubmissions++;
    }
    node->playbackFrameIndex++;
    if (node->playbackFrameIndex == node->cycleLength && node->loopMode != 0) {
        node->playbackFrameIndex = 0;
    }
    if (interruptsEnabled != 0) {
        EIntr();
    }
    sdfIpuExecQueue(0);
}

extern void sdfIpuMakeBltPacket(sdfIpuDecodeCb_t *node);

/* Return zero only when nothing is pending; otherwise call the zero-buffer handler if needed, queue and advance. */
s32 sdfIpuSetChainTag(sdfIpuDecodeCb_t *node) {
    u32 *selectedBuffer;
    if (node->completedBufferCount == 0) {
        return 0;
    }
    selectedBuffer = (u32 *)(node->transferPacketIndex * 4 + (s32)node + 0x28);
    if (*selectedBuffer == 0) {
        sdfIpuMakeBltPacket(node);
    }
    sdfImageSetChainTag(*selectedBuffer, (SdfTextureDmaTail *)(*selectedBuffer + node->transferPacketBytes - SDF_STREAM_QWORD_BYTES));
    sdfIpuFinishActiveFrame(node);
    return 1;
}

extern u32 D_00438D04;

/* Advance playback cadence, refill active feeds and retry a deferred IPU input. */
void sdfAdvanceStreamPlayback(s32 cadence) {
    sdfIpuDecodeCb_t *node = sdfCurrIpuDecodeCb;
    s32 elapsed;
    s32 interruptsEnabled;

    if (node != NULL && node->inputDmaStartPending != 0) {
        sdfIpuSetToIpuDma(node);
    }
    node = sdfIpuTaskLink;
    while (node != NULL) {
        if (node->pendingPlaybackSubmissions != 0) {
            node->pendingPlaybackSubmissions--;
        }
        elapsed = node->playbackCadenceRemainder;
        /* The playback provider views the same 0x8C stream allocation. */
        switch (node->playbackPhase) {
        case SDF_STREAM_PLAYBACK_FIRST_COMPLETION:
            sdfIpuSetChainTag(node);
            node->playbackPhase = SDF_STREAM_PLAYBACK_CADENCED;
            node->playbackCadenceRemainder = node->playbackCadenceStep;
            break;
        case SDF_STREAM_PLAYBACK_CADENCED:
            if (D_00438D04 != 1) {
                elapsed += node->playbackCadenceStep;
                if (elapsed >= cadence) {
                    if (sdfIpuSetChainTag(node) != 0) {
                        elapsed -= cadence;
                    }
                }
                node->playbackCadenceRemainder = elapsed;
            }
            break;
        }
        if (node->active == 1) {
            sdfIpuAllocGsRect(node);
            if (D_00438D04 != 1) {
                if (node->filledSlots < SDF_STREAM_RING_SLOTS) {
                    interruptsEnabled = DIntr();
                    sdfIpuHandleDmaEnd(node);
                    if (interruptsEnabled != 0) {
                        EIntr();
                    }
                }
                node->read(node, node->readContext, SDF_STREAM_READ_RESUME, NULL, 0);
            }
        }
        node = node->next;
    }
    sdfIpuExecQueue(0);
}

void sdfIpuStartDecodeTask(sdfIpuDecodeCb_t *node, SoundFormat *format, u8 *source, s32 size, s32 resourceWord) {
    sdfIpuSetDecodeParams(node, format, source, size);
    node->resourceWord = resourceWord;
    sdfIpuAttachTask(node);
}

void sdfIpuSetOutputTex(sdfIpuDecodeCb_t *node, const SdfStreamParams *params, u8 *source, s32 size, sdfTexHandle_t *resource) {
    SdfStreamParams streamParams = *params;
    switch (resource->pixelFormat) {
    case 0:
        streamParams.hasAudio = 0;
        break;
    case 2:
        streamParams.hasAudio = 1;
        break;
    }
    sdfIpuStartDecodeTask(node, &streamParams, source, size, sdfTexGetPixelAddr(resource));
}

void movieAttachDecodeTask(sdfIpuDecodeCb_t *node, SoundFormat *format, SdfStreamReadCallback read, movieStream_t *readContext, s32 resourceWord) {
    sdfIpuStrmDecodeInit(node, format, read, readContext);
    node->resourceWord = resourceWord;
    sdfIpuAttachTask(node);
}

extern SdfHandlerNode *sdfFromIpuDmacHandlerHandle;

extern SdfHandlerNode *sdfToIpuDmacHandlerHandle;

extern u8 sdfIpuDecodeThStack[];

extern SdfHandlerNode *DmacAddHandler(SdfHandlerKind kind, s32 index, SdfHandlerCallback handler, s32 argument, s32 option);

extern s32 CreateThreadSimple(void (*entryAddress)(), void *workspace, s32 stackBytes, s32 priority);

extern void sdfIpuDecodeThread();

void sdfIpuInit(void) {
    sceIpuInit();
    HW_WRITE(IPU_CMD, 0x90000000);
    sdfIpuTaskLink = 0;
    sdfCurrIpuDecodeCb = NULL;
    sdfFromIpuDmacHandlerHandle = DmacAddHandler(SDF_HANDLER_KIND_DMA, 3, sdfIpuFromDmacHandler, -1, 0);
    EnableDmac(3);
    sdfToIpuDmacHandlerHandle = DmacAddHandler(SDF_HANDLER_KIND_DMA, 4, sdfIpuToDmacHandler, -1, 0);
    EnableDmac(4);
    sdfIpuDecodeThId = CreateThreadSimple(sdfIpuDecodeThread, sdfIpuDecodeThStack, 0x800, 0x46);
    _StartThread(sdfIpuDecodeThId, 0);
}

extern void sdfIpuRemoveHandler(SdfHandlerNode *target);

/* Remove both IPU handlers and forward the final completion-source disable result. */
s32 sdfIpuClose(void) {
    if (sdfFromIpuDmacHandlerHandle != 0) {
        sdfIpuRemoveHandler(sdfFromIpuDmacHandlerHandle);
        sdfFromIpuDmacHandlerHandle = 0;
    }
    DisableDmac(3);
    if (sdfToIpuDmacHandlerHandle != 0) {
        sdfIpuRemoveHandler(sdfToIpuDmacHandlerHandle);
        sdfToIpuDmacHandlerHandle = 0;
    }
    return DisableDmac(4);
}

INCLUDE_SDATA(const s32, "sdf/sdfIpu", D_00438D04);

