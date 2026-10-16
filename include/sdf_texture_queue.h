#ifndef SDF_TEXTURE_QUEUE_H
#define SDF_TEXTURE_QUEUE_H

#include "common.h"
#include "sdf_texture_release.h"

/* Packed terminal DMA tag: the next packet address is encoded in tag.
 * Linking clears the two VIF command words, rather than a CPU next pointer. */
typedef struct SdfTextureDmaTail {
    u64 tag;
    u64 vifCodes;
} SdfTextureDmaTail;

typedef u32 SdfDmaAddress;

/* The semaphore protects independent release-node and DMA-packet chains. */
typedef struct SdfTextureQueue {
    s32 semaphoreId;
    SdfTextureReleaseHead *releaseHead;
    SdfTextureReleaseHead *releaseTail;
    SdfDmaAddress dmaPacketHeadAddress; /* 0x0C: EE physical address of the queued DMA chain */
    SdfTextureDmaTail *packetTail;
} SdfTextureQueue;

typedef char SdfTextureDmaTail_layout[
    (sizeof(SdfTextureDmaTail) == 0x10 &&
     (u32)&((SdfTextureDmaTail *)0)->vifCodes == 0x08) ? 1 : -1];
typedef char SdfTextureQueue_layout[
    (sizeof(SdfTextureQueue) == 0x14 &&
     (u32)&((SdfTextureQueue *)0)->releaseHead == 0x04 &&
     (u32)&((SdfTextureQueue *)0)->releaseTail == 0x08 &&
     (u32)&((SdfTextureQueue *)0)->dmaPacketHeadAddress == 0x0C &&
     (u32)&((SdfTextureQueue *)0)->packetTail == 0x10) ? 1 : -1];

/* packetAddress is the EE DMA address; packetTail points to its terminal tag. */
void sdfTexEnqueuePacketWithSemaphore(SdfDmaAddress packetAddress, SdfTextureDmaTail *packetTail);

#endif /* SDF_TEXTURE_QUEUE_H */
