#include "common.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"
/* Polled in a spin-wait below; its writer is outside this C unit. */
extern volatile u8 D_00438A1D;
extern void sdfSleepThreadCount(s32);
extern s32 sdfDoubleBufferAllocation;
extern s32 sdfReleaseResourceAllocation(s32);
extern s32 sdfAllocGeneralBlock(s32);
extern s32 sdfResourceRetainAddress(s32);
#include "sdf.h"
#include "sdf_linked_packet.h"
#include "sdf_packet_builders.h"
#include "sdf_draw.h"

typedef struct SdfPacketSlot {
    SdfListHead *list;
    u32 unk4;
    u8 state;
    u8 unk9;
    u8 pad0A[6];
} SdfPacketSlot;

extern u32 func_003287E0(void);
extern u32 sdfGetElapsedTimerTicks(u32);
extern s32 EIntr(void);
extern u16 D_00438A24;
extern s32 D_00438A30;
extern s8 D_00439165;
extern u8 sdfCurrentBufferIndex;

#define SDF_DMA_TAG_NEXT_BYTE 0x20
#define SDF_DMA_TAG_REF_BYTE 0x30
#define SDF_DMA_TAG_CALL_BYTE 0x50
#define SDF_DMA_TAG_END_BYTE 0x70
#define SDF_DMA_QWC_MASK 0xFFFF
#define SDF_DMA_ADDRESS_MASK 0x0FFFFFFF
#define SDF_QWORD_BYTES 0x10
#define SDF_QWORD_ALIGNMENT_MASK 0xF
#define SDF_PACKET_BUFFER_ALIGNMENT_MASK 0x7F
#define SDF_PACKET_BUFFER_COUNT 2
#define SDF_PENDING_NODE_BYTES 0x10
#define SDF_PENDING_BUFFER_BYTES 0x100
#define SDF_PENDING_BUFFER_CAPACITY 0x3F
#define SDF_PENDING_SLOT_COUNT 2
#define SDF_PENDING_LAST_SLOT 1
#define SDF_PATCHABLE_PACKET_BYTES 0x100
#define SDF_PATCHABLE_PACKET_TAIL_OFFSET 0xD0
#define SDF_DESCRIPTOR_PACKET_BYTES 0xB0
#define SDF_DESCRIPTOR_PACKET_TAIL_OFFSET 0x80
#define SDF_DMA_EXTENDED_TAIL_OFFSET 0x30
#define SDF_DMA_REFERENCE_NODE_BYTES 0x20
#define SDF_DMA_TAG_REF_WORD 0x30000000
#define SDF_DMA_TAG_NEXT_WORD 0x20000000
#define SDF_PACKET_LIST_BYTES 0x20

#define SDF_VIF_DIRECT_WORD 0x50000000
#define SDF_VIF_FLUSHE_WORD 0x10000000
#define SDF_GIF_ONE_REGISTER_WORD 0x10000000
#define SDF_GIF_EOP_BIT 0x8000
#define SDF_GIF_REGISTER_AD 0xE
#define SDF_DMA_CNT_ONE_WORD 0x10000001
#define SDF_VIF_DIRECT_ONE_FLUSHE 0x5000000110000000ULL
#define SDF_GIF_PACKED_AD_BITS 0x1000000000008000ULL
#define SDF_VIF_DIRECT_ONE_NOP 0x5000000100000000ULL
#define SDF_GS_FRAME_PRIMARY 0x4C
#define SDF_GS_FRAME_SECONDARY 0x4D
#define SDF_GS_ZBUF_PRIMARY 0x4E
#define SDF_GS_ZBUF_SECONDARY 0x4F
#define SDF_GS_XYOFFSET_PRIMARY 0x18
#define SDF_GS_XYOFFSET_SECONDARY 0x19
#define SDF_GS_SCISSOR_PRIMARY 0x40
#define SDF_GS_SCISSOR_SECONDARY 0x41
#define SDF_GS_CENTER_BIAS 0x1000
#define SDF_GS_FIELD_OFFSET_STEP 8
#define SDF_GS_PRMODECONT 0x1A
#define SDF_GS_COLCLAMP 0x46
#define SDF_GS_DTHE 0x45
#define SDF_GS_TEXA 0x3B
#define SDF_GS_DEFAULT_TEXA 0x4000000080ULL
#define SDF_GS_TEST_PRIMARY 0x47
#define SDF_GS_TEST_SECONDARY 0x48
#define SDF_GS_ALPHA_PRIMARY 0x42
#define SDF_GS_ALPHA_SECONDARY 0x43
#define SDF_GS_DEFAULT_ALPHA 0x44
#define SDF_GS_SCENE_TEST 0x517FB
#define SDF_GS_ALPHA_TEST 0x717FB
#define SDF_SCENE_DRAW_PAYLOAD_QWORDS 5
#define SDF_TEXTURE_SCENE_PAYLOAD_QWORDS 0x15
#define SDF_GRAPH_DEPTH_BUFFER_INDEX 2
#define SDF_ALPHA_DMA_QWORDS 3
#define SDF_VIF_DIRECT_THREE_WORD 0x50000003
#define SDF_VIF_FLUSHE_HALFWORD 0x1000
#define SDF_GIF_TWO_AD_LOOPS_EOP 0x8002
#define SDF_GS_BOUNDS_TEST 0x30003
#define SDF_GS_PRIM_SPRITE 6
#define SDF_GS_PRIM 0
#define SDF_GS_RGBAQ 1
#define SDF_GS_XYZ2 5

/* Native DMAC tag: QWC/ID/ADDR followed by the two packed VIF command words. */
typedef struct SdfDmaTag {
    u16 quadwordCount;
    u8 pad02;
    u8 kind;
    u32 address;
    u64 vifCommands;
} SdfDmaTag;

extern s32 sdfPendingQueueSemaphore;

extern s32 sdfCreateSemaphore(u32, u32, u32);

extern u64 sdfGraphHasPendingWork(void);

extern s32 func_0036DE70(void);

extern s32 sdfPacketBufferSize;

extern u32 sdfPacketBuffers[2];

extern s32 sdfPacketCursor;

extern s32 sdfPacketBufferEnd;

extern u32 sdfCreateReferenceDmaNode(u32);

extern u32 sdfAllocSizeClassBlock(u32);

extern void *sdfAllocAndClearQuadwords(s32);

typedef struct SdfDescriptorSource {
    u8 pad00[0xC];
    u32 baseAddress;
    u8 pad10[4];
    s16 bufferWidth;
    u8 pad16[2];
    s32 pixelFormat;
} SdfDescriptorSource;

extern SdfDescriptorSource *sdfPacketResourceEntries[];

extern volatile s8 sdfPacketSlotIndex;

extern s32 D_00438A28;

extern SdfPacketSlot D_0040B308[];

extern SdfResource *sdfResourceListHead;

void sdfPrependPacketList(SdfListHead *list, SdfListHead *item);

void sdfConnectPacketLists();

void sdfPrepareFrameDepthPacket(SdfPacketBuilder *packet, s32 bufferIndex);

extern void *sdfPendingQueueHead;

extern s8 sdfPendingQueueRotationActive;

extern s32 sdfPendingQueueSlots[2];

typedef struct SdfSynchronizedRequest {
    u32 value;
    u32 state;
} SdfSynchronizedRequest;

s32 sdfAllocPacketAligned(s32 size);

void sdfAppendPacketRange(SdfListHead *list, u32 packet, u32 end);
void sdfAppendLinkedPacketNode(SdfLinkedPacketList *list, u32 *node);

extern void sdfBuildFrameDepthScissorPacket(SdfPacket *, s32, s32, s32, s32, s32, s32, s32, s32);

void sdfWriteImageTransferRegisters(SdfPacket *packet, u32 destinationBufferAddress, s32 destinationBufferWidth,
                  s64 destinationFormat, s64 destinationX, s64 destinationY,
                  u32 sourceBufferAddress, s32 sourceBufferWidth, s32 sourceFormat,
                  s32 sourceX, s32 sourceY, s32 transferWidth, s32 transferHeight, s32 transferDirection);

extern void sdfBuildFillPacket106(s32, s32, s32, s32, s32, s32, s32, s32);

extern void sdfBuildFillPacket101(s32, s32, s32, s32, s32, s32, s32, s32);

void sdfDestroyObjectList();

extern void sdfReleaseChipBlock(void *allocation);


extern void sdfDestroyDevRequest(DevRequest *request);
extern void sdfDevResizeBufferedRequest(DevRequest *request, s32 count);
extern void sdfDevBufferedRequestGrow(DevRequest *request);


extern void sdfBuildQuadPacket(s32, s32, s32, s32, s32, s32, s32, s32);

extern void sdfBuildPacket116(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

extern void sdfBuildPacket104x4(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

extern void sdfBuildPacket10C(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032C278);

/* Walk the resource chain until the requested numeric ID is found. */
SdfResource *sdfFindResourceById(s32 id) {
    SdfResource *resource = sdfResourceListHead;

    while (resource != NULL) {
        if (resource->id == id) {
            return resource;
        }
        resource = resource->next;
    }
    return NULL;
}

extern SdfSynchronizedRequest sdfTextureReleaseQueue;
extern void sdfTexRelease();

void sdfRegisterTextureReleaseRequestHandler(void) {
    sdfInitializeSynchronizedRequest(&sdfTextureReleaseQueue, (u32)sdfTexRelease);
}

/* Transfer setup, deferred image metadata, and its linked primitive-reset tail. */
typedef struct SdfResourcePacket {
    u64 header[14];
    SdfPacket transfer[2];
    s32 quadwordCount;
    u32 unkB4;
    u32 unkB8;
    u32 unkBC;
    u64 tail[6];
} SdfResourcePacket;

void sdfBuildResourceTransferPacket(SdfResourcePacket *packet, SdfDescriptorSource *source,
    s32 sourceX, s32 sourceY, s32 width, s32 height, u32 arg6, u32 arg7, u32 arg8) {
    s32 quadwordCount;

    packet->header[0] = 0x90000001ULL;
    packet->header[1] = 0x10000000ULL;
    packet->header[2] = 0;
    packet->header[3] = 0;
    packet->header[4] = 0x90000002ULL;
    packet->header[5] = 0x5000000206008000ULL;
    packet->header[6] = 0x1000000000000001ULL;
    packet->header[7] = 0xE;
    packet->header[8] = 0;
    packet->header[9] = 0x61;
    packet->header[10] = ((u64)((u32)packet->tail & SDF_DMA_ADDRESS_MASK) << 32) | 0xA0000005ULL;
    packet->header[11] = 0x5000000500000000ULL;
    packet->header[12] = 0x1000000000000004ULL;
    packet->header[13] = 0xE;
    sdfWriteImageTransferRegisters(packet->transfer, 0, 0, 0, 0, 0,
        source->baseAddress, source->bufferWidth, source->pixelFormat,
        sourceX, sourceY, width, height, 1);
    quadwordCount = (sdfFormatBitsPerPixelB(source->pixelFormat) * width * height) >> 7;
    packet->quadwordCount = quadwordCount;
    packet->unkB4 = arg6;
    packet->unkB8 = arg7;
    packet->unkBC = arg8;
    packet->tail[0] = 0x20000002ULL;
    packet->tail[1] = 0x5000000206000000ULL;
    packet->tail[2] = 0x1000000000008001ULL;
    packet->tail[3] = 0xE;
    packet->tail[4] = 0x46;
    packet->tail[5] = 0;
}

/* Allocate a resource packet; append only its initialized 0xC0-byte payload. */
void sdfCreateResourcePacket(SdfListHead *list, s32 arg1, s32 arg2, s32 arg3, s32 arg4,
                   s32 arg5, s32 arg6, s32 arg7, s32 arg_sp0, s32 (*allocate)(s32)) {
    s32 packet;

    if (allocate == NULL) {
        allocate = sdfAllocPacketAligned;
    }
    packet = allocate(0xf0);
    sdfBuildResourceTransferPacket((SdfResourcePacket *)packet, (SdfDescriptorSource *)arg1,
        arg2, arg3, arg4, arg5, arg6, arg7, arg_sp0);
    sdfAppendPacketRange(list, packet, packet + 0xc0);
}

void sdfPatchPacketResourceField(SdfBigPacket *packet, s32 entryIndex) {
    packet->unk80 = (packet->unk80 & ~0x3FFF) | (u64)(u32)(sdfPacketResourceEntries[entryIndex]->baseAddress >> 6);
}

/* Allocate metadata plus a patchable DMA payload, registering both list views.
 * The drawing arguments remain opaque and are forwarded to the native builder. */
void sdfCreatePatchableResourcePacket(SdfListHead *list, SdfLinkedPacketList *linkedList, s32 arg2, s32 arg3,
                   s32 arg4, s32 arg5, s32 resourceAddress, s32 arg7, s32 arg8,
                   s32 (*allocatePacket)(s32)) {
    s32 packetAddress;

    if (allocatePacket == NULL) {
        allocatePacket = sdfAllocPacketAligned;
    }
    packetAddress = allocatePacket(SDF_PATCHABLE_PACKET_BYTES);
    ((SdfNode *)packetAddress)->unk4 = (u32)sdfPatchPacketResourceField;
    sdfBuildResourceTransferPacket((SdfResourcePacket *)(packetAddress + SDF_QWORD_BYTES),
        sdfPacketResourceEntries[0], arg2, arg3, arg4, arg5, resourceAddress, arg7, arg8);
    sdfAppendLinkedPacketNode(linkedList, (u32 *)packetAddress);
    sdfAppendPacketRange(list, packetAddress + SDF_QWORD_BYTES, packetAddress + SDF_PATCHABLE_PACKET_TAIL_OFFSET);
}

typedef struct SdfDescriptorPacket {
    u64 header[4];
    u64 transfer[8];
    u64 imageReferenceTag;
    u64 imageReferencePad;
    u64 imageGifTag0;
    u64 imageGifTag1;
    u64 flushTag0;
    u64 flushTag1;
    u64 flushGifTag;
    u64 flushRegister;
    u64 zero;
    u64 finishRegister;
} SdfDescriptorPacket;

void sdfBuildHostToLocalImagePacket(SdfDescriptorPacket *packet, SdfDescriptorSource *source,
                   s64 destinationX, s64 destinationY, s32 transferWidth,
                   s32 transferHeight, u32 sourceAddress) {
    s32 qwc;

    packet->header[0] = 0x10000006;
    packet->header[1] = 0x5000000611000000;
    packet->header[2] = 0x1000000000000004;
    packet->header[3] = 0xE;
    sdfWriteImageTransferRegisters((SdfPacket *)packet->transfer, source->baseAddress, source->bufferWidth,
                  source->pixelFormat, destinationX, destinationY, 0, 0, 0, 0, 0,
                  transferWidth, transferHeight, 0);

    qwc = (sdfFormatBitsPerPixelB(source->pixelFormat) * transferWidth * transferHeight) >> 7;
    packet->imageReferenceTag = 0x0800000000000000 | qwc;
    packet->imageReferencePad = 0;
    packet->imageGifTag0 = (u32)((qwc & SDF_DMA_QWC_MASK) | 0x30000000) |
                           ((u64)(sourceAddress & SDF_DMA_ADDRESS_MASK) << 32);
    packet->imageGifTag1 = (u64)(0x51000000 | qwc) << 32;
    packet->flushTag0 = 0x20000002;
    packet->flushTag1 = 0x5000000200000000;
    packet->flushGifTag = 0x1000000000008001;
    packet->flushRegister = 0xE;
    packet->zero = 0;
    packet->finishRegister = 0x3F;
}

/* Allocate a descriptor packet, delegate its opaque options, and append its range. */
void sdfCreateDescriptorPacket(SdfListHead *list, s32 descriptorAddress, s32 a, s32 b, s32 c, s32 d, s32 e,
                   s32 (*allocatePacket)(s32)) {
    s32 packetAddress;
    if (allocatePacket == NULL) {
        allocatePacket = sdfAllocPacketAligned;
    }
    packetAddress = allocatePacket(SDF_DESCRIPTOR_PACKET_BYTES);
    sdfBuildHostToLocalImagePacket((SdfDescriptorPacket *)packetAddress, (SdfDescriptorSource *)descriptorAddress,
                  a, b, c, d, e);
    sdfAppendPacketRange(list, packetAddress, packetAddress + SDF_DESCRIPTOR_PACKET_TAIL_OFFSET);
}

/* Lazily create the shared semaphore and reset this request's state. */
void sdfInitializeSynchronizedRequest(SdfSynchronizedRequest *request, u32 value) {
    if (sdfPendingQueueSemaphore < 0) {
        sdfPendingQueueSemaphore = sdfCreateSemaphore(1, 0x7f, 0);
    }
    request->value = value;
    request->state = 0;
}

typedef struct SdfPendingOwner SdfPendingOwner;

/* One chunk of queued entries: link to the previous chunk, then 0x3F entries. */
typedef struct SdfPendingBuffer {
    struct SdfPendingBuffer *next;
    u32 entry[0x3F];
} SdfPendingBuffer;

typedef struct SdfPendingNode {
    struct SdfPendingNode *next;    /* 0x00 */
    SdfPendingOwner *owner;         /* 0x04 */
    SdfPendingBuffer *buffer;       /* 0x08 */
    s32 remaining;                  /* 0x0C */
} SdfPendingNode;

struct SdfPendingOwner {
    void (*handler)(u32);
    SdfPendingNode *pending;
};

/* Queue an entry on the owner's pending node, adding a node or buffer chunk as needed. */
void sdfPendingQueuePush(SdfPendingOwner *owner, u32 entry) {
    SdfPendingNode *pendingNode;
    SdfPendingBuffer *entryBuffer;
    s32 freeEntries;

    if (entry != 0) {
        WaitSema(sdfPendingQueueSemaphore);
        pendingNode = owner->pending;
        if (pendingNode == NULL) {
            pendingNode = (SdfPendingNode *)sdfAllocSizeClassBlock(SDF_PENDING_NODE_BYTES);
            pendingNode->remaining = 0;
            pendingNode->next = sdfPendingQueueHead;
            pendingNode->buffer = NULL;
            pendingNode->owner = owner;
            owner->pending = pendingNode;
            sdfPendingQueueHead = pendingNode;
        }
        freeEntries = pendingNode->remaining;
        entryBuffer = pendingNode->buffer;
        if (freeEntries == 0) {
            SdfPendingBuffer *newBuffer = (SdfPendingBuffer *)sdfAllocSizeClassBlock(SDF_PENDING_BUFFER_BYTES);
            newBuffer->next = entryBuffer;
            pendingNode->buffer = newBuffer;
            entryBuffer = newBuffer;
            freeEntries = SDF_PENDING_BUFFER_CAPACITY;
        }
        entryBuffer->entry[SDF_PENDING_BUFFER_CAPACITY - freeEntries] = entry;
        pendingNode->remaining = freeEntries - 1;
        SignalSema(sdfPendingQueueSemaphore);
    }
}

typedef struct SdfLink {
    struct SdfLink *next;
    struct SdfLink *peer;
} SdfLink;

/* Detach the pending chain and clear each node's peer back-reference. */
SdfLink *sdfDetachQueue(void) {
    SdfLink *head = sdfPendingQueueHead;
    SdfLink *link;

    sdfPendingQueueHead = NULL;
    link = head;
    if (head != NULL) {
        do {
            link->peer->peer = NULL;
            link = link->next;
        } while (link != NULL);
    }
    return head;
}

/* Flush the newest buffer's populated entries first, then older full buffers.
 * Invoke the owner's handler in entry order within each buffer before freeing it. */
void sdfPendingQueueFlush(SdfPendingNode *pendingNode) {
    SdfPendingNode *nextPendingNode;
    SdfPendingBuffer *entryBuffer;
    SdfPendingBuffer *nextEntryBuffer;
    void (*entryHandler)(u32);
    s32 entryCount;
    s32 entryIndex;

    if (pendingNode != NULL) {
        do {
            entryCount = SDF_PENDING_BUFFER_CAPACITY - pendingNode->remaining;
            entryHandler = pendingNode->owner->handler;
            entryBuffer = pendingNode->buffer;
            if (entryBuffer != NULL) {
                do {
                    entryIndex = 0;
                    do {
                        entryHandler(entryBuffer->entry[entryIndex]);
                        entryIndex++;
                    } while (entryIndex < entryCount);
                    nextEntryBuffer = entryBuffer->next;
                    sdfReleaseChipBlock(entryBuffer);
                    entryBuffer = nextEntryBuffer;
                    entryCount = SDF_PENDING_BUFFER_CAPACITY;
                } while (entryBuffer != NULL);
            }
            nextPendingNode = pendingNode->next;
            sdfReleaseChipBlock(pendingNode);
            pendingNode = nextPendingNode;
        } while (pendingNode != NULL);
    }
}

/* Hand the oldest pending slot to the worker, shift the slot list down and
 * refill the last slot with the newly detached list. */
void sdfRotatePendingSlots(void) {
    u32 slotIndex;

    sdfPendingQueueRotationActive = 1;
    WaitSema(sdfPendingQueueSemaphore);
    sdfPendingQueueFlush(sdfPendingQueueSlots[0]);
    for (slotIndex = 0; slotIndex < SDF_PENDING_LAST_SLOT; slotIndex++) {
        sdfPendingQueueSlots[slotIndex] = sdfPendingQueueSlots[slotIndex + 1];
    }
    sdfPendingQueueSlots[SDF_PENDING_LAST_SLOT] = (s32)sdfDetachQueue();
    SignalSema(sdfPendingQueueSemaphore);
    sdfPendingQueueRotationActive = 0;
}

/* Test all three pending-work sources: flag, chain, and two slots. */
u64 sdfGraphHasPendingWork(void) {
    u32 slotIndex;
    s32 *slotCursor;
    if (sdfPendingQueueRotationActive != 0) {
        return 1;
    }
    if (sdfPendingQueueHead != NULL) {
        return 1;
    }
    slotIndex = 0;
    slotCursor = sdfPendingQueueSlots;
    do {
        if (*slotCursor != 0) {
            return 1;
        }
        slotCursor++;
        slotIndex++;
    } while (slotIndex < SDF_PENDING_SLOT_COUNT);
    return 0;
}

/* Preserve interrupt state while querying pending work. */
u64 sdfCheckPendingWorkWithInterrupts(void) {
    s64 interruptState;
    u64 pendingWork;

    interruptState = func_0036DE70();
    pendingWork = sdfGraphHasPendingWork();
    if (interruptState != 0) {
        EIntr();
    }
    return pendingWork;
}

/* Replace both packet buffers with one allocation, rounding each half to 128 bytes. */
void sdfResizeDoubleBuffer(s32 bufferBytes) {
    s32 allocationAddress;

    if (sdfDoubleBufferAllocation != 0) {
        sdfReleaseResourceAllocation(sdfDoubleBufferAllocation);
        sdfDoubleBufferAllocation = 0;
    }
    bufferBytes = (bufferBytes + SDF_PACKET_BUFFER_ALIGNMENT_MASK) & ~SDF_PACKET_BUFFER_ALIGNMENT_MASK;
    sdfPacketBufferSize = bufferBytes;
    sdfDoubleBufferAllocation = sdfAllocGeneralBlock(bufferBytes * SDF_PACKET_BUFFER_COUNT);
    allocationAddress = sdfResourceRetainAddress(sdfDoubleBufferAllocation);
    sdfPacketBuffers[0] = allocationAddress;
    sdfPacketBuffers[1] = allocationAddress + bufferBytes;
}

/* Select one half and expose its bounds to the packet allocator. */
void sdfSelectDoubleBuffer(s32 bufferIndex) {
    sdfPacketCursor = sdfPacketBuffers[bufferIndex];
    sdfPacketBufferEnd = sdfPacketBuffers[bufferIndex] + sdfPacketBufferSize;
}

s32 sdfGetBufferRemaining(void) {
    return sdfPacketBufferEnd - sdfPacketCursor;
}

/* Reserve packet space at a 16-byte boundary, returning the old cursor. */
s32 sdfAllocPacketAligned(s32 size) {
    s32 address;

    address = sdfPacketCursor;
    sdfPacketCursor = sdfPacketCursor + ((size + 0xfU) & 0xfffffff0);
    return address;
}

s32 sdfGetPacketCursor(void) {
    return sdfPacketCursor;
}

/* Round a supplied byte address upward to the next quadword boundary. */
void sdfSetPacketCursorAligned(s32 cursorAddress) {
    sdfPacketCursor = (cursorAddress + SDF_QWORD_ALIGNMENT_MASK) & ~SDF_QWORD_ALIGNMENT_MASK;
}

/* Clear all links and metadata before building a new packet list. */
void sdfInitPacketList(SdfListHead *list) {
    list->unkC = 0xFFFF;
    list->unk0 = 0;
    list->first = 0;
    list->last = 0;
    list->firstReferenceSource = 0;
    list->secondReferenceSource = 0;
    list->unk18 = 0;
    list->unk1C = 0;
}

/* Link the previous DMA packet to this packet with a NEXT tag. */
void sdfAppendPacket(SdfListHead *list, u32 packet) {
    s32 last;

    last = list->last;
    if (last == 0) {
        list->first = packet;
    }
    else {
        ((SdfDmaTag *)last)->kind = SDF_DMA_TAG_NEXT_BYTE;
        ((SdfDmaTag *)last)->address = packet & SDF_DMA_ADDRESS_MASK;
    }
    list->last = packet;
}

/* Append a DMA range, keeping its start and trailing tag addresses distinct. */
void sdfAppendPacketRange(SdfListHead *list, u32 packetAddress, u32 rangeTailAddress) {
    s32 previousTailAddress;

    previousTailAddress = list->last;
    if (previousTailAddress == 0) {
        list->first = packetAddress;
    }
    else {
        ((SdfDmaTag *)previousTailAddress)->kind = SDF_DMA_TAG_NEXT_BYTE;
        ((SdfDmaTag *)previousTailAddress)->address = packetAddress & SDF_DMA_ADDRESS_MASK;
    }
    list->last = rangeTailAddress;
}

/* Tag the new packet as REF and chain it into the DMA list. */
void sdfAppendReferencePacket(SdfListHead *list, u32 packetAddress) {
    s32 previousTailAddress;

    ((SdfDmaTag *)packetAddress)->kind = SDF_DMA_TAG_REF_BYTE;
    previousTailAddress = list->last;
    if (previousTailAddress == 0) {
        list->first = packetAddress;
    }
    else {
        ((SdfDmaTag *)previousTailAddress)->kind = SDF_DMA_TAG_NEXT_BYTE;
        ((SdfDmaTag *)previousTailAddress)->address = packetAddress & SDF_DMA_ADDRESS_MASK;
    }
    list->last = packetAddress + SDF_QWORD_BYTES;
}

/* Append a DMA packet whose trailing tag is three quadwords after its start. */
void sdfAppendDmaTagToList(SdfListHead *list, u32 packetAddress) {
    s32 previousTailAddress;

    previousTailAddress = list->last;
    if (previousTailAddress == 0) {
        list->first = packetAddress;
    }
    else {
        ((SdfDmaTag *)previousTailAddress)->kind = SDF_DMA_TAG_NEXT_BYTE;
        ((SdfDmaTag *)previousTailAddress)->address = packetAddress & SDF_DMA_ADDRESS_MASK;
    }
    list->last = packetAddress + SDF_DMA_EXTENDED_TAIL_OFFSET;
}

/* Tag the new packet as CALL and chain it into the DMA list. */
void sdfAppendCallPacket(SdfListHead *list, u32 packetAddress) {
    s32 previousTailAddress;

    ((SdfDmaTag *)packetAddress)->kind = SDF_DMA_TAG_CALL_BYTE;
    previousTailAddress = list->last;
    if (previousTailAddress == 0) {
        list->first = packetAddress;
    }
    else {
        ((SdfDmaTag *)previousTailAddress)->kind = SDF_DMA_TAG_NEXT_BYTE;
        ((SdfDmaTag *)previousTailAddress)->address = packetAddress & SDF_DMA_ADDRESS_MASK;
    }
    list->last = packetAddress + SDF_QWORD_BYTES;
}

/* Prepend a nonempty packet list, connecting its DMA tail to the former first list. */
void sdfPrependPacketList(SdfListHead *destinationList, SdfListHead *incomingList) {
    SdfListHead *firstList;

    if (incomingList->last == 0) {
        return;
    }
    firstList = (SdfListHead *)destinationList->first;
    if (firstList == NULL) {
        destinationList->last = (u32)incomingList;
    } else {
        sdfConnectPacketLists(incomingList, firstList);
    }
    incomingList->unk0 = (u32)firstList;
    destinationList->first = (u32)incomingList;
}

void sdfAppendPacketList(SdfListHead *list, SdfListHead *item) {
    u32 *last;

    if (item->first != 0) {
        last = (u32 *)list->last;
        if (last == NULL) {
            list->first = (u32)item;
        }
        else {
            *last = (u32)item;
            sdfConnectPacketLists(last);
        }
        list->last = (u32)item;
    }
}

s32 sdfPrependIfMode1(SdfListHead *list, s32 mode, SdfListHead *packet) {
    if (mode == 1) {
        sdfPrependPacketList(list, packet);
    }
}


extern void func_0032D0F0(SdfPoolNode *, s32);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032D0F0);

/* Make a REF DMA node for the payload following the source tag. */
u32 sdfCreateReferenceDmaNode(u32 sourceTagAddress) {
    SdfDmaNode *referenceNode = (SdfDmaNode *)sdfAllocPacketAligned(SDF_DMA_REFERENCE_NODE_BYTES);
    SdfDmaTag *sourceTag = (SdfDmaTag *)sourceTagAddress;
    u64 dmaHeader = sourceTag->quadwordCount;
    u32 payloadAddress = ((u32)sourceTag + SDF_QWORD_BYTES) & SDF_DMA_ADDRESS_MASK;
    s64 packedAddress = (s64)payloadAddress << 32;

    dmaHeader |= SDF_DMA_TAG_REF_WORD;
    dmaHeader |= packedAddress;
    referenceNode->unk0 = dmaHeader;
    referenceNode->unk10 = 0;
    referenceNode->unk8 = sourceTag->vifCommands;
    return (u32)referenceNode;
}

/* Create a reference node and patch the preceding DMA NEXT tag to point at it. */
s32 sdfLinkReferenceDmaNode(s32 previousTagAddress, u32 sourceTagAddress) {
    u32 referenceAddress;

    referenceAddress = sdfCreateReferenceDmaNode(sourceTagAddress);
    ((SdfDmaTag *)previousTagAddress)->kind = SDF_DMA_TAG_NEXT_BYTE;
    ((SdfDmaTag *)previousTagAddress)->address = referenceAddress & SDF_DMA_ADDRESS_MASK;
    return referenceAddress + SDF_QWORD_BYTES;
}

extern s32 sdfLinkReferenceDmaNode(s32 previous, u32 source);

/* Insert differing nonzero reference sources before linking the incoming DMA chain.
 * Zero incoming sources inherit prior state; the locals cover both source slots. */
void sdfConnectPacketLists(previousList, incomingList)
    SdfListHead *previousList;
    SdfListHead *incomingList;
{
    u32 tailTagAddress = previousList->last;
    u32 previousSource;
    u32 incomingSource;

    previousSource = previousList->firstReferenceSource;
    incomingSource = incomingList->firstReferenceSource;
    if (previousSource != incomingSource) {
        if (incomingSource != 0) {
            tailTagAddress = sdfLinkReferenceDmaNode(tailTagAddress, incomingSource);
        } else {
            incomingList->firstReferenceSource = previousSource;
        }
    }
    previousSource = previousList->secondReferenceSource;
    incomingSource = incomingList->secondReferenceSource;
    if (previousSource != incomingSource) {
        if (incomingSource != 0) {
            tailTagAddress = sdfLinkReferenceDmaNode(tailTagAddress, incomingSource);
        } else {
            incomingList->secondReferenceSource = previousSource;
        }
    }
    ((SdfDmaTag *)tailTagAddress)->kind = SDF_DMA_TAG_NEXT_BYTE;
    ((SdfDmaTag *)tailTagAddress)->address = incomingList->first & SDF_DMA_ADDRESS_MASK;
}

typedef struct SdfRefNode {
    u8 pad00[0x10];
    u64 chain; /* 0x10: NEXT tag chaining to the previous head */
} SdfRefNode;

/* Prepend the second reference, then the first: the resulting order is first, second, payload. */
void sdfChainReferenceNodes(SdfListHead *list) {
    SdfRefNode *referenceNode;
    u32 priorHeadAddress;
    u32 sourceTagAddress;

    sourceTagAddress = list->secondReferenceSource;
    if (sourceTagAddress != 0) {
        referenceNode = (SdfRefNode *)sdfCreateReferenceDmaNode(sourceTagAddress);
        priorHeadAddress = list->first & SDF_DMA_ADDRESS_MASK;
        list->first = (u32)referenceNode;
        referenceNode->chain = ((s64)priorHeadAddress << 32) | SDF_DMA_TAG_NEXT_WORD;
    }
    sourceTagAddress = list->firstReferenceSource;
    if (sourceTagAddress != 0) {
        referenceNode = (SdfRefNode *)sdfCreateReferenceDmaNode(sourceTagAddress);
        priorHeadAddress = list->first & SDF_DMA_ADDRESS_MASK;
        list->first = (u32)referenceNode;
        referenceNode->chain = ((s64)priorHeadAddress << 32) | SDF_DMA_TAG_NEXT_WORD;
    }
}

/* Flush every pool entry, chain the packet lists together and terminate the last. */
s32 sdfFlushPoolNodes(SdfPoolNode *node) {
    SdfListHead *tail = NULL;
    s32 head = 0;

    for (; node != NULL; node = node->next) {
        node->prepend((SdfListHead *)node, 0, NULL);
        if (node->first != 0) {
            if (head != 0) {
                sdfConnectPacketLists(tail, (SdfListHead *)node->first);
            } else {
                head = node->first;
                sdfChainReferenceNodes((SdfListHead *)head);
            }
            tail = (SdfListHead *)node->last;
        }
    }
    if (tail != NULL) {
        ((SdfDmaTag *)tail->last)->kind = SDF_DMA_TAG_END_BYTE;
        ((SdfDmaTag *)tail->last)->address = 0;
    }
    return head;
}

void sdfClearLinkedPacketList(SdfLinkedPacketList *list) {
    list->unk0 = 0;
    list->first = 0;
    list->last = 0;
    list->unkC = 0;
}

void sdfAppendLinkedPacketNode(SdfLinkedPacketList *list, u32 *node) {
    if (list->last == 0) {
        list->first = (u32)node;
    }
    else {
        *(u32 *)list->last = (u32)node;
    }
    list->last = (u32)node;
    *node = 0;
}

void sdfClearPacketChain(SdfPacketChain *chain) {
    chain->head = NULL;
    chain->tail = NULL;
}

void sdfAppendPacketChainNode(SdfPacketChain *chain, SdfLinkedPacketList *node) {
    if (chain->tail == NULL) {
        chain->head = node;
    }
    else {
        *(u32 *)chain->tail->last = node->last;
    }
    chain->tail = node;
}

/* Encode the packed A+D payload count and its DMA/VIF transfer length. */
void sdfInitializeDmaReferenceTag(SdfPacket *packet, s32 payloadQwords) {
    s64 dmaQwords;

    dmaQwords = payloadQwords + 1;
    packet->unk8 = (((dmaQwords | SDF_VIF_DIRECT_WORD) << 32) | SDF_VIF_FLUSHE_WORD);
    packet->unk10 = (payloadQwords | (((s64)SDF_GIF_ONE_REGISTER_WORD << 32) | SDF_GIF_EOP_BIT));
    packet->unk0 = dmaQwords;
    packet->unk18 = SDF_GIF_REGISTER_AD;
}

/* Emit a GIF header, a REF tag with masked count/address, and the trailing NEXT tag. */
void sdfBuildDmaReferenceChain(u64 *packet, u32 sourceAddress, s32 qwordCount) {
    packet[0] = SDF_DMA_CNT_ONE_WORD;
    packet[1] = SDF_VIF_DIRECT_ONE_FLUSHE;
    packet[2] = (u64)qwordCount | SDF_GIF_PACKED_AD_BITS;
    packet[3] = SDF_GIF_REGISTER_AD;
    packet[4] = (u32)((qwordCount & SDF_DMA_QWC_MASK) | SDF_DMA_TAG_REF_WORD) | ((u64)(sourceAddress & SDF_DMA_ADDRESS_MASK) << 32);
    packet[5] = SDF_VIF_DIRECT_ONE_NOP;
    packet[6] = SDF_DMA_TAG_NEXT_WORD;
    packet[7] = 0;
}

/* Emit FRAME/ZBUF/XYOFFSET/SCISSOR A+D pairs for the selected GS context.
 * Register IDs are separate from their values; fieldOffset adds a half-pixel Y step. */
void sdfBuildFrameDepthScissorPacket(SdfPacket *packet, s32 frameAddress, s32 width, s32 height,
                  s32 frameFormat, s32 depthAddress, s32 depthFormat,
                  s32 fieldOffset, s32 gsContext) {
    s64 frameRegisterId;
    s64 depthRegisterId;
    s64 offsetRegisterId;
    s64 scissorRegisterId;
    s32 xOffset;
    s32 yOffset;

    if (gsContext == 0) {
        frameRegisterId = SDF_GS_FRAME_PRIMARY;
        depthRegisterId = SDF_GS_ZBUF_PRIMARY;
        offsetRegisterId = SDF_GS_XYOFFSET_PRIMARY;
        scissorRegisterId = SDF_GS_SCISSOR_PRIMARY;
    } else {
        frameRegisterId = SDF_GS_FRAME_SECONDARY;
        depthRegisterId = SDF_GS_ZBUF_SECONDARY;
        offsetRegisterId = SDF_GS_XYOFFSET_SECONDARY;
        scissorRegisterId = SDF_GS_SCISSOR_SECONDARY;
    }
    packet[1].unk18 = scissorRegisterId;
    yOffset = (SDF_GS_CENTER_BIAS - height) << 3;
    xOffset = (SDF_GS_CENTER_BIAS - width) << 3;
    packet[0].unk0 = (s64)(frameFormat << 24) | (s64)(((width + 63) >> 6) << 16) | (s64)(frameAddress >> 11);
    if (fieldOffset != 0) {
        yOffset += SDF_GS_FIELD_OFFSET_STEP;
    }
    packet[0].unk8 = frameRegisterId;
    packet[0].unk10 = (s64)(depthFormat << 24) | (s64)(depthAddress >> 11);
    packet[0].unk18 = depthRegisterId;
    packet[1].unk8 = offsetRegisterId;
    packet[1].unk10 = ((s64)(height - 1) << 48) | ((s64)(width - 1) << 16);
    packet[1].unk0 = xOffset | ((s64)yOffset << 32);
}

/* Encode centered viewport bounds in GS coordinate words; the last two arguments are unused. */
void sdfBuildCenteredViewBoundsPacket(u64 *packet, s32 width, s32 height, s32 unused0, s32 unused1) {
    u32 lowerBounds = ((SDF_GS_CENTER_BIAS - height) << 19) | ((SDF_GS_CENTER_BIAS - width) << 3);
    u32 upperBounds = ((height + SDF_GS_CENTER_BIAS) << 19) | ((width + SDF_GS_CENTER_BIAS) << 3);

    packet[3] = SDF_GS_PRIM;
    packet[7] = SDF_GS_XYZ2;
    packet[0] = SDF_GS_BOUNDS_TEST;
    packet[1] = SDF_GS_TEST_PRIMARY;
    packet[2] = SDF_GS_PRIM_SPRITE;
    packet[4] = (u64)0xFE00 << 46;
    packet[5] = SDF_GS_RGBAQ;
    packet[6] = lowerBounds;
    packet[8] = upperBounds;
    packet[9] = SDF_GS_XYZ2;
}

/* Seed PRMODECONT, COLCLAMP, DTHE and TEXA drawing registers. */
void sdfInitDrawPacket(u64 *packet) {
    packet[0] = 1;
    packet[1] = SDF_GS_PRMODECONT;
    packet[2] = 1;
    packet[3] = SDF_GS_COLCLAMP;
    packet[4] = 0;
    packet[5] = SDF_GS_DTHE;
    packet[6] = SDF_GS_DEFAULT_TEXA;
    packet[7] = SDF_GS_TEXA;
}

/* Build the common header and FRAME/ZBUF/XYOFFSET/SCISSOR state for one GS context. */
void sdfBuildSceneDrawHeader(SdfPacket *packet, s32 frameAddress, s32 width, s32 height,
                           s32 frameFormat, s32 depthAddress, s32 depthFormat, s32 gsContext) {
    sdfInitializeDmaReferenceTag(packet, SDF_SCENE_DRAW_PAYLOAD_QWORDS);
    sdfBuildFrameDepthScissorPacket(packet + 1, frameAddress, width, height, frameFormat, depthAddress, depthFormat, 0, gsContext);
}

/* Native VRAM range returned by sdfAllocImageBuffer; offsets match its owner unit. */
typedef struct SdfTexHead {
    struct SdfTexHead *next;
    struct SdfTexHead *prev;
    s32 allocationMode;
    u32 address; /* 0x0C: VRAM offset in 32-bit words */
    s32 size;
    s16 width;
    s16 height;
    s32 format;
} SdfTexHead;

typedef struct SdfSceneDrawPacket {
    SdfPacket header;    /* 0x00 */
    u64 draw[8];         /* 0x20 */
    SdfPacket contextOne[2]; /* 0x60 */
    SdfPacket contextTwo[2]; /* 0xA0 */
    u64 limits[10];      /* 0xE0 */
    u64 regs[8];         /* 0x130 */
} SdfSceneDrawPacket;

extern u8 D_00438A22;

/* Build both GS drawing contexts from the selected frame buffer and shared depth buffer. */
void sdfBuildTextureScenePacket(SdfSceneDrawPacket *packet, SdfGraphObj *view, s32 bufferIndex) {
    s32 frameAddress;
    s32 depthAddress;
    s32 width;
    s32 height;
    s32 frameFormat;
    s32 depthFormat;

    sdfInitializeDmaReferenceTag(&packet->header, SDF_TEXTURE_SCENE_PAYLOAD_QWORDS);
    frameAddress = view->buffers[bufferIndex]->word;
    depthAddress = view->buffers[SDF_GRAPH_DEPTH_BUFFER_INDEX]->word;
    width = view->width;
    frameFormat = view->bufferFormat;
    height = view->height;
    depthFormat = view->auxiliaryFormat;
    sdfBuildFrameDepthScissorPacket(packet->contextOne, frameAddress, width, height, frameFormat, depthAddress, depthFormat, D_00438A22, 0);
    sdfBuildFrameDepthScissorPacket(packet->contextTwo, frameAddress, width, height, frameFormat, depthAddress, depthFormat, D_00438A22, 1);
    sdfBuildCenteredViewBoundsPacket(packet->limits, view->width, view->height, view->bufferFormat, view->auxiliaryFormat);
    packet->regs[0] = SDF_GS_SCENE_TEST;
    packet->regs[1] = SDF_GS_TEST_PRIMARY;
    packet->regs[2] = SDF_GS_DEFAULT_ALPHA;
    packet->regs[3] = SDF_GS_ALPHA_PRIMARY;
    packet->regs[4] = SDF_GS_SCENE_TEST;
    packet->regs[5] = SDF_GS_TEST_SECONDARY;
    packet->regs[6] = SDF_GS_DEFAULT_ALPHA;
    packet->regs[7] = SDF_GS_ALPHA_SECONDARY;
    sdfInitDrawPacket(packet->draw);
}

INCLUDE_ASM(const s32, "game/code_0032C278", sdfRefreshSceneNodePackets);

typedef struct SdfSceneNode {
    u8 pad00[4];
    void (*handler)(); /* 0x4 */
    SdfGraphObj *view; /* 0x8 */
    u8 padC[4];
    SdfPacket header;  /* 0x10 */
    u64 draw[8];       /* 0x30 */
    SdfPacket contextOne[2]; /* 0x70 */
    SdfPacket contextTwo[2]; /* 0xB0 */
    u64 limits[10];    /* 0xF0 */
    u64 regs[8];       /* 0x140 */
    u64 framePacketWords[4]; /* 0x180 */
    SdfTexBuf texturePackets[2]; /* 0x1A0 */
} SdfSceneNode;

typedef char SdfSceneNode_size_must_be_0x220[(sizeof(SdfSceneNode) == 0x220) ? 1 : -1];

extern void sdfRefreshSceneNodePackets();

/* Retain the render-target view and initialize the scene callback and fixed drawing state. */
void sdfInitSceneNode(SdfSceneNode *node, SdfGraphObj *view) {
    sdfInitializeDmaReferenceTag(&node->header, SDF_TEXTURE_SCENE_PAYLOAD_QWORDS);
    node->view = view;
    node->handler = sdfRefreshSceneNodePackets;
    sdfBuildCenteredViewBoundsPacket(node->limits, view->width, view->height, view->bufferFormat, view->auxiliaryFormat);
    node->regs[0] = SDF_GS_SCENE_TEST;
    node->regs[1] = SDF_GS_TEST_PRIMARY;
    node->regs[2] = SDF_GS_DEFAULT_ALPHA;
    node->regs[3] = SDF_GS_ALPHA_PRIMARY;
    node->regs[4] = SDF_GS_SCENE_TEST;
    node->regs[5] = SDF_GS_TEST_SECONDARY;
    node->regs[6] = SDF_GS_DEFAULT_ALPHA;
    node->regs[7] = SDF_GS_ALPHA_SECONDARY;
    sdfInitDrawPacket(node->draw);
}

/* Link the metadata node separately from the DMA payload one quadword later. */
void sdfAppendLinkedPacketPayload(SdfListHead *dmaList, SdfLinkedPacketList *linkedList, u32 *linkedNode) {
    sdfAppendLinkedPacketNode(linkedList, linkedNode);
    sdfAppendPacket(dmaList, (s32)linkedNode + SDF_QWORD_BYTES);
}

void func_0032DB30(s32 source, u32 packet, s32 variant) {
    if (variant == 0) {
        sdfBuildDmaReferenceChain(packet, source + 0x180, 1);
        return;
    }
    sdfBuildDmaReferenceChain(packet, source + 400, 1);
}

void func_0032DB78(s32 source, u32 packet, s32 variant) {
    if (variant == 0) {
        sdfBuildDmaReferenceChain(packet, source + 0x70, 1);
        return;
    }
    sdfBuildDmaReferenceChain(packet, source + 0xb0, 1);
}

void sdfAppendDmaPrimary(s32 list, u32 source, SdfDmaNode *node) {
    node->unk8 = (((u64)0x50000004 << 16) | 0x1000) << 16;
    node->unk0 = ((u64)((source + 0x1a0) & 0xfffffff) << 32) | 0x30000004;
    node->unk10 = 0;
    sdfAppendReferencePacket(list, (u32)node);
}

void sdfAppendDmaSecondary(s32 list, u32 source, SdfDmaNode *node) {
    node->unk8 = (((u64)0x50000004 << 16) | 0x1000) << 16;
    node->unk0 = ((u64)((source + 0x1E0) & 0xfffffff) << 32) | 0x30000004;
    node->unk10 = 0;
    sdfAppendReferencePacket(list, (u32)node);
}

INCLUDE_ASM(const s32, "game/code_0032C278", sdfPrepareFrameDepthPacket);

void sdfInitPacketBuilder(SdfPacketBuilder *packet, SdfGraphObj *source, u32 frameMask, s32 region, s32 mode) {
    sdfInitializeDmaReferenceTag(packet->packets, 2);
    packet->mode = mode;
    packet->source = source;
    packet->frameMask = frameMask;
    packet->region = region;
    packet->prepare = sdfPrepareFrameDepthPacket;
}

void sdfQueueFramePackets(SdfListHead *list, SdfPacketChain *chain) {
    u32 start = func_003287E0();
    s32 interrupts;
    s32 index;
    SdfPacketSlot *slot;

    while (sdfPacketSlotIndex != 0) {
    }
    D_00438A24 = sdfGetElapsedTimerTicks(start);
    interrupts = func_0036DE70();
    index = D_00438A28 + 1;
    if (index == 3) {
        index = 0;
    }
    slot = &D_0040B308[index];
    slot->list = NULL;
    slot->unk4 = 0;
    slot->state = 0;
    if (list != NULL && list->last != 0) {
        slot->list = list;
        if (chain != NULL) {
            slot->unk4 = chain->head->first;
            if (slot->unk4 != 0) {
                *(u32 *)chain->tail->last = 0;
            }
        }
        slot->unk9 = sdfCurrentBufferIndex;
    }
    D_00438A28 = index;
    D_00439165 = 1;
    D_00438A30++;
    if (interrupts != 0) {
        EIntr();
    }
}

void sdfSetNonnegativePacketIndex(s32 value) {
    sdfPacketSlotIndex = (value < 0) ? 0 : value;
}

void sdfResetPacketSlotState(void) {
    sdfPacketSlotIndex = 0;
    D_0040B308[0].list = NULL;
    D_00438A28 = 0;
}

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032DEC8);

void sdfWaitSlotReady(void) {
    SdfPacketSlot *entry;

    while (1) {
        while (D_00438A1D != 0) {
        }
        entry = &D_0040B308[D_00438A28];
        if (entry->list == NULL) {
            return;
        }
        if (entry->state >= 2) {
            return;
        }
        sdfSleepThreadCount(0);
    }
}

/* Allocate and initialize an empty packet list using the selected allocator. */
s32 sdfAllocatePacketList(s32 (*allocatorArgument)(s32)) {
    s32 (*allocator)(s32) = allocatorArgument;
    s32 listAddress;

    if (allocator == NULL) {
        allocator = sdfAllocPacketAligned;
    }
    listAddress = allocator(SDF_PACKET_LIST_BYTES);
    sdfInitPacketList((SdfListHead *)listAddress);
    return listAddress;
}

/* Allocate the requested packet bytes, initialize through the callback, and append. */
void sdfAppendInitializedPacket(s32 listAddress, void (*initialize)(s32), s32 packetBytes, s32 (*allocatorArgument)(s32)) {
    s32 (*allocator)(s32) = allocatorArgument;
    s32 packetAddress;

    if (allocator == NULL) {
        allocator = sdfAllocPacketAligned;
    }
    packetAddress = allocator(packetBytes);
    initialize(packetAddress);
    sdfAppendPacket(listAddress, packetAddress);
}
/* Set primary-context TEST and ALPHA values; 0x44 is blend data, not a register ID. */
void sdfInitPrimaryAlphaBlendRegisters(SdfPacket *packet) {
    packet->unk0 = SDF_GS_ALPHA_TEST;
    packet->unk8 = SDF_GS_TEST_PRIMARY;
    packet->unk10 = SDF_GS_DEFAULT_ALPHA;
    packet->unk18 = SDF_GS_ALPHA_PRIMARY;
}

/* Set the same TEST/ALPHA values for the secondary GS context. */
void sdfInitSecondaryAlphaBlendRegisters(SdfPacket *packet) {
    packet->unk0 = SDF_GS_ALPHA_TEST;
    packet->unk8 = SDF_GS_TEST_SECONDARY;
    packet->unk10 = SDF_GS_DEFAULT_ALPHA;
    packet->unk18 = SDF_GS_ALPHA_SECONDARY;
}

/* Transfer a GIF tag and two primary-context A+D register writes with FLUSHE/DIRECT. */
void sdfBuildPrimaryAlphaBlendDmaPacket(SdfPacket *packet) {
    sdfInitPrimaryAlphaBlendRegisters(packet + 1);
    packet->unk0 = SDF_ALPHA_DMA_QWORDS;
    packet->unk8 = (((u64)SDF_VIF_DIRECT_THREE_WORD << 16 | SDF_VIF_FLUSHE_HALFWORD) << 16);
    packet->unk10 = (((u64)SDF_GIF_ONE_REGISTER_WORD << 32) | SDF_GIF_TWO_AD_LOOPS_EOP);
    packet->unk18 = SDF_GIF_REGISTER_AD;
}

/* Transfer the corresponding secondary-context A+D pair without changing tag encoding. */
void sdfBuildSecondaryAlphaBlendDmaPacket(SdfPacket *packet) {
    sdfInitSecondaryAlphaBlendRegisters(packet + 1);
    packet->unk0 = SDF_ALPHA_DMA_QWORDS;
    packet->unk8 = (((u64)SDF_VIF_DIRECT_THREE_WORD << 16 | SDF_VIF_FLUSHE_HALFWORD) << 16);
    packet->unk10 = (((u64)SDF_GIF_ONE_REGISTER_WORD << 32) | SDF_GIF_TWO_AD_LOOPS_EOP);
    packet->unk18 = SDF_GIF_REGISTER_AD;
}

void sdfSetPrimaryTestBlendRegisters(SdfPacket *packet) {
    packet->unk0 = 0x717FB;
    packet->unk8 = 0x47;
    packet->unk10 = 0x44;
    packet->unk18 = 0x42;
}
void sdfSetSecondaryTestBlendRegisters(SdfPacket *packet) {
    packet->unk0 = 0x717FB;
    packet->unk8 = 0x48;
    packet->unk10 = 0x44;
    packet->unk18 = 0x43;
}

void sdfBuildPrimaryTestBlendPacket(SdfPacket *packet) {
    sdfSetPrimaryTestBlendRegisters(packet + 1);
    packet->unk0 = 3;
    packet->unk8 = (((u64)0x50000003 << 16 | 0x1000) << 16);
    packet->unk10 = (((u64)0x10000000 << 32) | 0x8002);
    packet->unk18 = 0xE;
}

void sdfBuildSecondaryTestBlendPacket(SdfPacket *packet) {
    sdfSetSecondaryTestBlendRegisters(packet + 1);
    packet->unk0 = 3;
    packet->unk8 = (((u64)0x50000003 << 16 | 0x1000) << 16);
    packet->unk10 = (((u64)0x10000000 << 32) | 0x8002);
    packet->unk18 = 0xE;
}

void sdfInitPrimaryAlphaAdditiveRegisters(SdfPacket *packet) {
    packet->unk0 = 0x71801;
    packet->unk8 = 0x47;
    packet->unk10 = 0x48;
    packet->unk18 = 0x42;
}

void sdfInitSecondaryAlphaAdditiveRegisters(SdfPacket *packet) {
    packet->unk0 = 0x71801;
    packet->unk8 = 0x48;
    packet->unk10 = 0x48;
    packet->unk18 = 0x43;
}

void sdfBuildPrimaryAlphaAdditiveDmaPacket(SdfPacket *packet) {
    sdfInitPrimaryAlphaAdditiveRegisters(packet + 1);
    packet->unk0 = 3;
    packet->unk8 = (((u64)0x50000003 << 16 | 0x1000) << 16);
    packet->unk10 = (((u64)0x10000000 << 32) | 0x8002);
    packet->unk18 = 0xE;
}

void sdfBuildSecondaryAlphaAdditiveDmaPacket(SdfPacket *packet) {
    sdfInitSecondaryAlphaAdditiveRegisters(packet + 1);
    packet->unk0 = 3;
    packet->unk8 = (((u64)0x50000003 << 16 | 0x1000) << 16);
    packet->unk10 = (((u64)0x10000000 << 32) | 0x8002);
    packet->unk18 = 0xE;
}

void sdfInitPrimaryAlphaSubtractiveRegisters(SdfPacket *packet) {
    packet->unk0 = 0x71801;
    packet->unk8 = 0x47;
    packet->unk10 = 0x42;
    packet->unk18 = 0x42;
}

void sdfInitSecondaryAlphaSubtractiveRegisters(SdfPacket *packet) {
    packet->unk0 = 0x71801;
    packet->unk8 = 0x48;
    packet->unk10 = 0x42;
    packet->unk18 = 0x43;
}

void sdfBuildPrimaryAlphaSubtractiveDmaPacket(SdfPacket *packet) {
    sdfInitPrimaryAlphaSubtractiveRegisters(packet + 1);
    packet->unk0 = 3;
    packet->unk8 = (((u64)0x50000003 << 16 | 0x1000) << 16);
    packet->unk10 = (((u64)0x10000000 << 32) | 0x8002);
    packet->unk18 = 0xE;
}

void sdfBuildSecondaryAlphaSubtractiveDmaPacket(SdfPacket *packet) {
    sdfInitSecondaryAlphaSubtractiveRegisters(packet + 1);
    packet->unk0 = 3;
    packet->unk8 = (((u64)0x50000003 << 16 | 0x1000) << 16);
    packet->unk10 = (((u64)0x10000000 << 32) | 0x8002);
    packet->unk18 = 0xE;
}

void sdfInitializeTextureFlushRegister(u64 *packet) {
    *packet = 0;
    packet[1] = 0x3f;
}

void sdfInitializeTextureFlushPacket(SdfPacket *packet) {
    sdfInitializeTextureFlushRegister((u64 *)(packet + 1));
    packet->unk0 = 2;
    packet->unk8 = (((u64)0x50000002 << 16 | 0x1000) << 16);
    packet->unk10 = (((u64)0x10000000 << 32) | 0x8001);
    packet->unk18 = 0xE;
}

/* Emit the four A+D registers controlling an image transfer. */
void sdfWriteImageTransferRegisters(SdfPacket *packet, u32 destinationBufferAddress, s32 destinationBufferWidth,
                  s64 destinationFormat, s64 destinationX, s64 destinationY,
                  u32 sourceBufferAddress, s32 sourceBufferWidth, s32 sourceFormat,
                  s32 sourceX, s32 sourceY, s32 transferWidth, s32 transferHeight, s32 transferDirection) {
    packet[0].unk0 = (destinationFormat << 56) |
        ((u64)(destinationBufferWidth >> 6) << 48) |
        ((u64)(destinationBufferAddress >> 6) << 32) |
        (sourceFormat << 24) | ((sourceBufferWidth >> 6) << 16) |
        (sourceBufferAddress >> 6);
    packet[0].unk8 = 0x50;
    packet[0].unk10 = (destinationY << 48) | (destinationX << 32) |
        (sourceY << 16) | sourceX;
    packet[0].unk18 = 0x51;
    packet[1].unk0 = transferWidth | ((u64)transferHeight << 32);
    packet[1].unk8 = 0x52;
    packet[1].unk10 = transferDirection;
    packet[1].unk18 = 0x53;
}

/* Wrap BITBLTBUF, TRXPOS, TRXREG and TRXDIR image-transfer settings in a DMA/GIF header. */
void sdfInitializeExtendedDrawPacket(SdfPacket *packet, u32 destinationBufferAddress, s32 destinationBufferWidth,
                                    s64 destinationFormat, s64 destinationX, s64 destinationY,
                                    u32 sourceBufferAddress, s32 sourceBufferWidth, s32 sourceFormat,
                                    s32 sourceX, s32 sourceY, s32 transferWidth, s32 transferHeight, s32 transferDirection) {
    packet->unk0 = 5;
    packet->unk8 = (((u64)0x50000005 << 16 | 0x1000) << 16);
    packet->unk10 = (((u64)0x10000000 << 32) | 0x8004);
    packet->unk18 = 0xE;
    sdfWriteImageTransferRegisters(packet + 1, destinationBufferAddress, destinationBufferWidth,
                  destinationFormat, destinationX, destinationY, sourceBufferAddress, sourceBufferWidth,
                  sourceFormat, sourceX, sourceY, transferWidth, transferHeight, transferDirection);
}

/* Allocate and append the image-transfer packet; use the packet arena when allocator is null. */
void sdfCreateExtendedPacket(s32 packetList, u32 destinationBufferAddress, s32 destinationBufferWidth,
                             s64 destinationFormat, s64 destinationX, s64 destinationY,
                             u32 sourceBufferAddress, s32 sourceBufferWidth, s32 sourceFormat,
                             s32 sourceX, s32 sourceY, s32 transferWidth, s32 transferHeight,
                             s32 transferDirection, s32 (*allocator)(s32)) {
    s32 packetAddress;

    if (allocator == NULL) {
        allocator = sdfAllocPacketAligned;
    }
    packetAddress = allocator(0x60);
    sdfInitializeExtendedDrawPacket((SdfPacket *)packetAddress, destinationBufferAddress, destinationBufferWidth,
                                    destinationFormat, destinationX, destinationY, sourceBufferAddress, sourceBufferWidth,
                                    sourceFormat, sourceX, sourceY, transferWidth, transferHeight, transferDirection);
    sdfAppendPacket(packetList, packetAddress);
}

void sdfPatchPacketResourceReference(SdfBigPacket *packet, s32 entryIndex) {
    packet->unk30 = (packet->unk30 & ~0x3FFF) | (u64)(u32)(sdfPacketResourceEntries[entryIndex ^ packet->resourceIndexXor]->baseAddress >> 6);
}

extern SdfGraphObj D_0040B290;

/* Build a local-to-local GS copy from graph buffer zero into destination. */
void sdfCreateGraphBufferCopyPacket(SdfListHead *drawList, SdfLinkedPacketList *linkedList,
                   SdfTexResource *destination, s32 destinationX, s32 destinationY,
                   s32 sourceX, s32 sourceY, s32 transferWidth, s32 transferHeight,
                   s32 resourceIndexXor, s32 (*allocPacket)(s32)) {
    SdfNode *packet;
    SdfPacket *drawPacket;

    if (allocPacket == NULL) {
        allocPacket = sdfAllocPacketAligned;
    }
    packet = (SdfNode *)allocPacket(0x70);
    packet->unk8 = resourceIndexXor;
    packet->unk4 = (u32)sdfPatchPacketResourceReference;
    drawPacket = (SdfPacket *)(packet + 1);

    sdfInitializeExtendedDrawPacket(
        drawPacket, destination->word, destination->width,
        destination->format, destinationX, destinationY, D_0040B290.buffers[0]->word,
        D_0040B290.width, D_0040B290.bufferFormat, sourceX, sourceY, transferWidth,
        transferHeight, 2);
    sdfAppendLinkedPacketNode(linkedList, (u32 *)packet);
    sdfAppendPacket(drawList, (s32)drawPacket);
}

/* Pack two UV/XYZ vertex pairs after the common primitive and color. */
void sdfBuildPacket116(s32 address, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0, s32 x1, s32 y1, s32 u1, s32 v1, s32 depth) {
    u64 *packet = (u64 *)address;
    u64 depthHigh = (u64)depth << 32;

    packet[0] = 0x6400000000008001ULL;
    packet[1] = 0x535310;
    packet[2] = (u32)(primitive | 0x116);
    packet[3] = (u32)color | ((u64)0xFE00 << 46);
    packet[4] = (u0 & 0xFFFF) | (v0 << 16);
    packet[5] = (u32)((x0 & 0xFFFF) | (y0 << 16)) | depthHigh;
    packet[6] = (u1 & 0xFFFF) | (v1 << 16);
    packet[7] = (u32)((x1 & 0xFFFF) | (y1 << 16)) | depthHigh;
}

void sdfAppendTexturedLinePacket(s32 list, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0,
                   s32 v0, s32 x1, s32 y1, s32 u1, s32 v1, s32 depth, s32 (*alloc)(s32)) {
    SdfPacket *packet;
    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    packet = (SdfPacket *)alloc(0x50);
    packet->unk0 = 0x20000004;
    packet->unk8 = (((u64)0x50000004 << 16) | 0x1000) << 16;
    sdfBuildPacket116((SdfPacket *)&packet->unk10, color, primitive, x0, y0, u0, v0, x1, y1, u1, v1, depth);
    sdfAppendPacket(list, (s32)packet);
}

/* Pack three GS XYZ vertices with a common depth. */
void sdfBuildTriPacket104(s32 address, s32 color, s32 primitive, s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, s32 depth) {
    u64 *packet = (u64 *)address;
    u64 depthHigh = (u64)depth << 32;

    packet[0] = 0x6400000000008001ULL;
    packet[1] = 0xF55510;
    packet[2] = (u32)(primitive | 0x104);
    packet[3] = (u32)color | ((u64)0xFE00 << 46);
    packet[4] = (u32)((x0 & 0xFFFF) | (y0 << 16)) | depthHigh;
    packet[5] = (u32)((x1 & 0xFFFF) | (y1 << 16)) | depthHigh;
    packet[6] = (u32)((x2 & 0xFFFF) | (y2 << 16)) | depthHigh;
}

void sdfQueueFlatTriangle(s32 list, s32 color, s32 primitive, s32 x0, s32 y0, s32 x1,
                   s32 y1, s32 x2, s32 y2, s32 depth, s32 (*alloc)(s32)) {
    SdfPacket *packet;
    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    packet = (SdfPacket *)alloc(0x50);
    packet->unk0 = 0x20000004;
    packet->unk8 = (((u64)0x50000004 << 16) | 0x1000) << 16;
    sdfBuildTriPacket104((SdfPacket *)&packet->unk10, color, primitive, x0, y0, x1, y1, x2, y2, depth);
    sdfAppendPacket(list, (s32)packet);
}

/* Pack four GS XYZ vertices with a common depth. */
void sdfBuildPacket104x4(s32 address, s32 color, s32 primitive, s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, s32 x3, s32 y3, s32 depth) {
    u64 *packet = (u64 *)address;
    u64 depthHigh = (u64)depth << 32;

    packet[0] = 0x6400000000008001ULL;
    packet[1] = 0x555510;
    packet[2] = (u32)(primitive | 0x104);
    packet[3] = (u32)color | ((u64)0xFE00 << 46);
    packet[4] = (u32)((x0 & 0xFFFF) | (y0 << 16)) | depthHigh;
    packet[5] = (u32)((x1 & 0xFFFF) | (y1 << 16)) | depthHigh;
    packet[6] = (u32)((x2 & 0xFFFF) | (y2 << 16)) | depthHigh;
    packet[7] = (u32)((x3 & 0xFFFF) | (y3 << 16)) | depthHigh;
}

void sdfBuildPacketE(s32 list, s32 color, s32 primitive, s32 x0, s32 y0, s32 x1,
                   s32 y1, s32 x2, s32 y2, s32 x3, s32 y3, s32 depth, s32 (*alloc)(s32)) {
    SdfPacket *packet;
    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    packet = (SdfPacket *)alloc(0x50);
    packet->unk0 = 0x20000004;
    packet->unk8 = (((u64)0x50000004 << 16) | 0x1000) << 16;
    sdfBuildPacket104x4((SdfPacket *)&packet->unk10, color, primitive, x0, y0, x1, y1, x2, y2, x3, y3, depth);
    sdfAppendPacket(list, (s32)packet);
}

/* Emit three GS vertices, each with its own packed color and shared depth. */
void sdfBuildPacket10C(s32 address, s32 primitive, s32 x0, s32 y0, s32 color0, s32 x1, s32 y1, s32 color1, s32 x2, s32 y2, s32 color2, s32 depth) {
    u64 *packet = (u64 *)address;
    u64 depthHigh = (u64)depth << 32;

    packet[0] = 0x8400000000008001ULL;
    packet[1] = 0xFFFFFFFFF5151510ULL;
    packet[2] = (u32)(primitive | 0x10C);
    packet[3] = (u32)color0 | ((u64)0xFE00 << 46);
    packet[4] = (u32)((x0 & 0xFFFF) | (y0 << 16)) | depthHigh;
    packet[5] = (u32)color1 | ((u64)0xFE00 << 46);
    packet[6] = (u32)((x1 & 0xFFFF) | (y1 << 16)) | depthHigh;
    packet[7] = (u32)color2 | ((u64)0xFE00 << 46);
    packet[8] = (u32)((x2 & 0xFFFF) | (y2 << 16)) | depthHigh;
}

void sdfBuildPacketF(s32 list, s32 primitive, s32 x0, s32 y0, s32 color0, s32 x1, s32 y1,
                   s32 color1, s32 x2, s32 y2, s32 color2, s32 depth, s32 (*alloc)(s32)) {
    SdfPacket *packet;
    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    packet = (SdfPacket *)alloc(0x60);
    packet->unk0 = 0x20000005;
    packet->unk8 = (((u64)0x50000005 << 16) | 0x1000) << 16;
    sdfBuildPacket10C((SdfPacket *)&packet->unk10, primitive, x0, y0, color0, x1, y1, color1, x2, y2, color2, depth);
    sdfAppendPacket(list, (s32)packet);
}

/* Four vertices each carry their own color, with a common Z value. */
void sdfWriteGouraudQuadPacket(s32 address, s32 primitive, s32 x0, s32 y0, s32 color0, s32 x1, s32 y1, s32 color1, s32 x2, s32 y2, s32 color2, s32 x3, s32 y3, s32 color3, s32 depth) {
    u64 *packet = (u64 *)address;
    u64 depthHigh = (u64)depth << 32;

    packet[0] = 0xA400000000008001ULL;
    packet[1] = 0xF515151510ULL;
    packet[2] = (u32)(primitive | 0x10C);
    packet[3] = (u32)color0 | ((u64)0xFE00 << 46);
    packet[4] = (u32)((x0 & 0xFFFF) | (y0 << 16)) | depthHigh;
    packet[5] = (u32)color1 | ((u64)0xFE00 << 46);
    packet[6] = (u32)((x1 & 0xFFFF) | (y1 << 16)) | depthHigh;
    packet[7] = (u32)color2 | ((u64)0xFE00 << 46);
    packet[8] = (u32)((x2 & 0xFFFF) | (y2 << 16)) | depthHigh;
    packet[9] = (u32)color3 | ((u64)0xFE00 << 46);
    packet[10] = (u32)((x3 & 0xFFFF) | (y3 << 16)) | depthHigh;
}

void sdfQueueGouraudQuad(s32 list, s32 primitive, s32 x0, s32 y0, s32 color0, s32 x1,
                   s32 y1, s32 color1, s32 x2, s32 y2, s32 color2, s32 x3,
                   s32 y3, s32 color3, s32 depth, s32 (*alloc)(s32)) {
    SdfPacket *packet;
    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    packet = (SdfPacket *)alloc(0x70);
    packet->unk0 = 0x20000006;
    packet->unk8 = (((u64)0x50000006 << 16) | 0x1000) << 16;
    sdfWriteGouraudQuadPacket((s32)&packet->unk10, primitive, x0, y0, color0, x1, y1, color1, x2, y2, color2, x3, y3, color3, depth);
    sdfAppendPacket(list, (s32)packet);
}

/* Pack three UV/XYZ vertex pairs after the common primitive and color. */
void sdfBuildPacket114(s32 address, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0, s32 x1, s32 y1, s32 u1, s32 v1, s32 x2, s32 y2, s32 u2, s32 v2, s32 depth) {
    u64 *packet = (u64 *)address;
    u64 depthHigh = (u64)depth << 32;

    packet[0] = 0x8400000000008001ULL;
    packet[1] = 0x53535310;
    packet[2] = (u32)(primitive | 0x114);
    packet[3] = (u32)color | ((u64)0xFE00 << 46);
    packet[4] = (u0 & 0xFFFF) | (v0 << 16);
    packet[5] = (u32)((x0 & 0xFFFF) | (y0 << 16)) | depthHigh;
    packet[6] = (u1 & 0xFFFF) | (v1 << 16);
    packet[7] = (u32)((x1 & 0xFFFF) | (y1 << 16)) | depthHigh;
    packet[8] = (u2 & 0xFFFF) | (v2 << 16);
    packet[9] = (u32)((x2 & 0xFFFF) | (y2 << 16)) | depthHigh;
}

void sdfQueueTexturedTriangle(s32 list, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0,
                   s32 v0, s32 x1, s32 y1, s32 u1, s32 v1, s32 x2, s32 y2,
                   s32 u2, s32 v2, s32 depth, s32 (*alloc)(s32)) {
    SdfPacket *packet;
    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    packet = (SdfPacket *)alloc(0x60);
    packet->unk0 = 0x20000005;
    packet->unk8 = (((u64)0x50000005 << 16) | 0x1000) << 16;
    sdfBuildPacket114((s32)&packet->unk10, color, primitive, x0, y0, u0, v0, x1, y1, u1, v1, x2, y2, u2, v2, depth);
    sdfAppendPacket(list, (s32)packet);
}

/* Pack four textured vertices with a common color and depth. */
void sdfWriteTexturedQuadPacket(s32 address, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0,
                   s32 x1, s32 y1, s32 u1, s32 v1, s32 x2, s32 y2, s32 u2, s32 v2,
                   s32 x3, s32 y3, s32 u3, s32 v3, s32 depth) {
    u64 *packet = (u64 *)address;
    u64 depthHigh = (u64)depth << 32;

    packet[0] = 0xA400000000008001ULL;
    packet[1] = 0x5353535310ULL;
    packet[2] = (u32)(primitive | 0x114);
    packet[3] = (u32)color | ((u64)0xFE00 << 46);
    packet[4] = (u0 & 0xFFFF) | (v0 << 16);
    packet[5] = (u32)((x0 & 0xFFFF) | (y0 << 16)) | depthHigh;
    packet[6] = (u1 & 0xFFFF) | (v1 << 16);
    packet[7] = (u32)((x1 & 0xFFFF) | (y1 << 16)) | depthHigh;
    packet[8] = (u2 & 0xFFFF) | (v2 << 16);
    packet[9] = (u32)((x2 & 0xFFFF) | (y2 << 16)) | depthHigh;
    packet[10] = (u3 & 0xFFFF) | (v3 << 16);
    packet[11] = (u32)((x3 & 0xFFFF) | (y3 << 16)) | depthHigh;
}

void sdfQueueTexturedQuad(s32 list, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0,
                   s32 v0, s32 x1, s32 y1, s32 u1, s32 v1, s32 x2, s32 y2,
                   s32 u2, s32 v2, s32 x3, s32 y3, s32 u3, s32 v3, s32 depth,
                   s32 (*alloc)(s32)) {
    SdfPacket *packet;
    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    packet = (SdfPacket *)alloc(0x70);
    packet->unk0 = 0x20000006;
    packet->unk8 = (((u64)0x50000006 << 16) | 0x1000) << 16;
    sdfWriteTexturedQuadPacket((s32)&packet->unk10, color, primitive, x0, y0, u0, v0, x1, y1, u1, v1, x2, y2, u2, v2, x3, y3, u3, v3, depth);
    sdfAppendPacket(list, (s32)packet);
}

/* Three vertices each carry UV and color plus a shared Z value. */
void sdfWriteGouraudTexturedTrianglePacket(s32 address, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0, s32 color0,
                   s32 x1, s32 y1, s32 u1, s32 v1, s32 color1, s32 x2, s32 y2,
                   s32 u2, s32 v2, s32 color2, s32 depth) {
    u64 *packet = (u64 *)address;
    u64 depthHigh = (u64)depth << 32;

    packet[0] = 0xA400000000008001ULL;
    packet[1] = 0x5135135130ULL;
    packet[2] = (u32)(primitive | 0x11C);
    packet[3] = (u0 & 0xFFFF) | (v0 << 16);
    packet[4] = (u32)color0 | ((u64)0xFE00 << 46);
    packet[5] = (u32)((x0 & 0xFFFF) | (y0 << 16)) | depthHigh;
    packet[6] = (u1 & 0xFFFF) | (v1 << 16);
    packet[7] = (u32)color1 | ((u64)0xFE00 << 46);
    packet[8] = (u32)((x1 & 0xFFFF) | (y1 << 16)) | depthHigh;
    packet[9] = (u2 & 0xFFFF) | (v2 << 16);
    packet[10] = (u32)color2 | ((u64)0xFE00 << 46);
    packet[11] = (u32)((x2 & 0xFFFF) | (y2 << 16)) | depthHigh;
}

void sdfQueueGouraudTexturedTriangle(s32 list, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0,
                   s32 color0, s32 x1, s32 y1, s32 u1, s32 v1, s32 color1,
                   s32 x2, s32 y2, s32 u2, s32 v2, s32 color2, s32 depth,
                   s32 (*alloc)(s32)) {
    SdfPacket *packet;
    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    packet = (SdfPacket *)alloc(0x70);
    packet->unk0 = 0x20000006;
    packet->unk8 = (((u64)0x50000006 << 16) | 0x1000) << 16;
    sdfWriteGouraudTexturedTrianglePacket((s32)&packet->unk10, primitive, x0, y0, u0, v0, color0, x1, y1, u1, v1, color1, x2, y2, u2, v2, color2, depth);
    sdfAppendPacket(list, (s32)packet);
}

/* Pack four vertices with individual UV and color and a common depth. */
void sdfWriteGouraudTexturedQuadPacket(s32 address, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0, s32 color0,
                   s32 x1, s32 y1, s32 u1, s32 v1, s32 color1, s32 x2, s32 y2, s32 u2,
                   s32 v2, s32 color2, s32 x3, s32 y3, s32 u3, s32 v3, s32 color3, s32 depth) {
    u64 *packet = (u64 *)address;
    u64 depthHigh = (u64)depth << 32;

    packet[0] = 0xE400000000008001ULL;
    packet[1] = 0xF5135135135130ULL;
    packet[2] = (u32)(primitive | 0x11C);
    packet[3] = (u0 & 0xFFFF) | (v0 << 16);
    packet[4] = (u32)color0 | ((u64)0xFE00 << 46);
    packet[5] = (u32)((x0 & 0xFFFF) | (y0 << 16)) | depthHigh;
    packet[6] = (u1 & 0xFFFF) | (v1 << 16);
    packet[7] = (u32)color1 | ((u64)0xFE00 << 46);
    packet[8] = (u32)((x1 & 0xFFFF) | (y1 << 16)) | depthHigh;
    packet[9] = (u2 & 0xFFFF) | (v2 << 16);
    packet[10] = (u32)color2 | ((u64)0xFE00 << 46);
    packet[11] = (u32)((x2 & 0xFFFF) | (y2 << 16)) | depthHigh;
    packet[12] = (u3 & 0xFFFF) | (v3 << 16);
    packet[13] = (u32)color3 | ((u64)0xFE00 << 46);
    packet[14] = (u32)((x3 & 0xFFFF) | (y3 << 16)) | depthHigh;
}

void sdfQueueGouraudTexturedQuad(s32 list, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0,
                   s32 color0, s32 x1, s32 y1, s32 u1, s32 v1, s32 color1,
                   s32 x2, s32 y2, s32 u2, s32 v2, s32 color2, s32 x3, s32 y3,
                   s32 u3, s32 v3, s32 color3, s32 depth, s32 (*alloc)(s32)) {
    SdfPacket *packet;
    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    packet = (SdfPacket *)alloc(0x90);
    packet->unk0 = 0x20000008;
    packet->unk8 = (((u64)0x50000008 << 16) | 0x1000) << 16;
    sdfWriteGouraudTexturedQuadPacket((s32)&packet->unk10, primitive, x0, y0, u0, v0, color0, x1, y1, u1, v1, color1, x2, y2, u2, v2, color2, x3, y3, u3, v3, color3, depth);
    sdfAppendPacket(list, (s32)packet);
}

/* Emit two packed GS XYZ vertices; the high word supplies their shared depth. */
void sdfBuildFillPacket106(s32 address, s32 color, s32 primitive, s32 left, s32 top, s32 right, s32 bottom, s32 depth) {
    u64 *packet = (u64 *)address;
    u64 depthHigh = (u64)depth << 32;

    packet[0] = 0x4400000000008001ULL;
    packet[1] = 0x5510;
    packet[2] = (u32)(primitive | 0x106);
    packet[3] = (u32)color | ((u64)0xFE00 << 46);
    packet[4] = (u32)((left & 0xFFFF) | (top << 16)) | depthHigh;
    packet[5] = (u32)((right & 0xFFFF) | (bottom << 16)) | depthHigh;
}

void sdfCreatePacketA(SdfListHead *list, s32 color, s32 primitive, s32 left, s32 top, s32 right, s32 bottom, s32 depth, s32 (*alloc)(s32)) {
    s32 buffer;

    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    buffer = alloc(0x40);
    *(u64 *)buffer = 0x20000003ULL;
    *(u64 *)(buffer + 8) = 0x5000000310000000ULL;
    sdfBuildFillPacket106(buffer + 0x10, color, primitive, left, top, right, bottom, depth);
    sdfAppendPacket(list, buffer);
}

void sdfBuildFillPacket101(s32 address, s32 color, s32 primitive, s32 left, s32 top, s32 right, s32 bottom, s32 depth) {
    u64 *packet = (u64 *)address;
    u64 depthHigh = (u64)depth << 32;

    packet[0] = 0x4400000000008001ULL;
    packet[1] = 0x5510;
    packet[2] = (u32)(primitive | 0x101);
    packet[3] = (u32)color | ((u64)0xFE00 << 46);
    packet[4] = (u32)((left & 0xFFFF) | (top << 16)) | depthHigh;
    packet[5] = (u32)((right & 0xFFFF) | (bottom << 16)) | depthHigh;
}

void sdfAppendFillRectanglePacket(SdfListHead *list, s32 color, s32 primitive, s32 left, s32 top, s32 right, s32 bottom, s32 depth, s32 (*alloc)(s32)) {
    s32 buffer;

    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    buffer = alloc(0x40);
    *(u64 *)buffer = 0x20000003ULL;
    *(u64 *)(buffer + 8) = 0x5000000310000000ULL;
    sdfBuildFillPacket101(buffer + 0x10, color, primitive, left, top, right, bottom, depth);
    sdfAppendPacket(list, buffer);
}

/* Close the rectangle by repeating its first GS XYZ vertex. */
void sdfBuildQuadPacket(s32 address, s32 color, s32 primitive, s32 left, s32 top, s32 right, s32 bottom, s32 depth) {
    u64 *packet = (u64 *)address;
    u64 depthHigh = (u64)depth << 32;

    packet[0] = 0x8400000000008001ULL;
    packet[1] = 0xFFFFFFFFF5555510ULL;
    packet[2] = (u32)(primitive | 0x102);
    packet[3] = (u32)color | ((u64)0xFE00 << 46);
    packet[4] = (u32)((left & 0xFFFF) | (top << 16)) | depthHigh;
    packet[5] = (u32)((right & 0xFFFF) | (top << 16)) | depthHigh;
    packet[6] = (u32)((right & 0xFFFF) | (bottom << 16)) | depthHigh;
    packet[7] = (u32)((left & 0xFFFF) | (bottom << 16)) | depthHigh;
    packet[8] = (u32)((left & 0xFFFF) | (top << 16)) | depthHigh;
}

void sdfAppendClosedRectanglePacket(SdfListHead *list, s32 color, s32 primitive, s32 left, s32 top, s32 right, s32 bottom, s32 depth, s32 (*alloc)(s32)) {
    s32 buffer;

    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    buffer = alloc(0x60);
    *(u64 *)buffer = 0x20000005ULL;
    *(u64 *)(buffer + 8) = 0x5000000510000000ULL;
    sdfBuildQuadPacket(buffer + 0x10, color, primitive, left, top, right, bottom, depth);
    sdfAppendPacket(list, buffer);
}

extern SdfSynchronizedRequest sdfObjectListReleaseQueue;
extern void sdfDestroyObjectList();

void sdfInitializeObjectListRequest(void) {
    sdfInitializeSynchronizedRequest(&sdfObjectListReleaseQueue, (u32)sdfDestroyObjectList);
}


SdfDrawNode *sdfCreateDrawNode(void) {
    SdfDrawNode *node = sdfAllocAndClearQuadwords(0x100);

    node->color = 0x80808080;
    node->unk16 = -1;
    node->previous = node;
    node->next = node;
    VU0_STORE_VF($vf0, node->quaternion);
    VU0_STORE_VF($vf0, node->translation);
    VU0_SET_ONES_XYZ($vf10);
    VU0_STORE_VF($vf10, node->scale);
    EE_MMI_UNIT_MATRIX(node->localMatrix);
    return node;
}

void *sdfEnsureFreeRootWorkspace(SdfDrawNode *root) {
    void *workspace = (void *)root->address;

    if (workspace == NULL) {
        workspace = (void *)sdfAllocSizeClassBlock(0x100);
        root->address = (u32)workspace;
    }
    return workspace;
}

/* Release allocations in both free-node lists; the stored head pointers are not cleared. */
void sdfFreeNodeLists(SdfDrawNode *root) {
    SdfCommandNode **listCursor = root->lists;
    s32 listIndex = 0;
    s32 listCount = 2;
    do {
        SdfCommandNode *node = *listCursor;
        while (node != NULL) {
            SdfCommandNode *next = node->next;
            if (node->resourceHandle != 0) {
                sdfReleaseResourceAllocation(node->resourceHandle);
            } else {
                sdfReleaseChipBlock(node);
            }
            node = next;
        }
        listIndex++;
        listCursor++;
    } while (listIndex != listCount);
}


void sdfReleaseFreeRoot(SdfDrawNode *root) {
    sdfFreeNodeLists(root);
    sdfReleaseChipBlock((void *)root->address);
    root->address = 0;
    sdfReleaseChipBlock(root);
}


/* Allocate a model root and seed its unit matrix, identity rotation quaternion, unit scale and colour. */
SdfModel *sdfCreateBufferedTransformSlot(void) {
    SdfModel *slot;

    slot = sdfAllocAndClearQuadwords(sizeof(SdfModel));
    slot->list = sdfDevCreateBufferedRequest(0, 4, 0x20);
    EE_MMI_UNIT_MATRIX(slot->matrix);
    VU0_STORE_VF(vf0, slot->rotationQuaternion);
    VU0_SET_ONES_XYZ(vf10);
    VU0_STORE_VF(vf10, slot->scaleVector);
    slot->unk1A = -1;
    slot->color = 0x80808080;
    return slot;
}

/* Release each element root, then the buffered request and its owner allocation. */
void sdfDestroyObjectList(SdfModel *owner) {
    s32 elementIndex;
    for (elementIndex = 0; elementIndex < owner->list->usedCount; elementIndex++) {
        sdfReleaseFreeRoot(((SdfDrawNode **)owner->list->buffer)[elementIndex]);
    }
    sdfDestroyDevRequest(owner->list);
    sdfReleaseChipBlock(owner);
}

extern void sdfReleaseQueuedResource(void *resource, s32 retained);
extern void sdfReleaseChipBlock();

void sdfReleaseDevSlot(SdfModel *slot, s32 recycle, s32 release) {
    if (slot == NULL) {
        return;
    }
    if (release != 0) {
        sdfReleaseQueuedResource(slot->resources, 1);
    }
    if (slot->slotPairs != NULL) {
        sdfDestroyDevRequest(slot->slotPairs);
    }
    if (recycle != 0) {
        sdfPendingQueuePush((SdfPendingOwner *)&sdfObjectListReleaseQueue, (u32)slot);
    } else {
        sdfDestroyDevRequest(slot->list);
        sdfReleaseChipBlock(slot);
    }
}

void sdfResizeBufferedSlotRequest(SdfModel *model, s32 count) {
    sdfDevResizeBufferedRequest(model->list, count);
}


/* Store the preorder hierarchy traversal in the element array; capacity is the caller's responsibility. */
void sdfCollectTreeNodes(SdfModel *hierarchy) {
    SdfDrawNode **nodeArray = hierarchy->list->buffer;
    SdfDrawNode *currentNode = hierarchy->rootNode;
    SdfDrawNode **output;

    if (currentNode != NULL) {
        output = nodeArray;
        do {
            *output++ = currentNode;
            if (currentNode->children != NULL) {
                currentNode = currentNode->children;
            } else {
                do {
                    SdfDrawNode *nextSibling = currentNode->next;
                    if (nextSibling != NULL) {
                        currentNode = nextSibling;
                        break;
                    }
                    currentNode = currentNode->parent;
                } while (currentNode != NULL);
            }
        } while (currentNode != NULL);
    }
}

/* Grow the buffered root's node array if needed, then attach the new node.
 * Fetch the element buffer after growth because its allocation can move. */
void sdfAppendBufferedRouteNode(SdfModel *list, SdfDrawNode *owner, SdfDrawNode *node) {
    s16 usedCount;
    SdfDrawNode **elements;
    DevRequest *request;
    s32 newCount;

    request = list->list;
    usedCount = request->usedCount;
    newCount = usedCount + 1;
    if ((s16)request->capacity < newCount) {
        sdfDevBufferedRequestGrow(request);
        request = list->list;
    }
    elements = request->buffer;
    node->root = list;
    request->usedCount = newCount;
    elements[usedCount] = node;
    sdfLinkRouteNode(node, owner);
}

void sdfUnlinkRouteNode(SdfDrawNode *node) {
    SdfDrawNode *owner = node->parent;
    if (owner == NULL) {
        SdfModel *root = node->root;
        if (root->rootNode == node) {
            root->rootNode = NULL;
        }
        return;
    }
    {
        SdfDrawNode *next = node->next;
        SdfDrawNode *previous = node->previous;
        if (next != node) {
            next->previous = previous;
            previous->next = next;
            if (owner->children == node) {
                owner->children = next;
            }
            node->previous = node;
            node->next = node;
            return;
        }
        if (owner->children == node) {
            owner->children = NULL;
        }
    }
}

void sdfLinkRouteNode(SdfDrawNode *node, SdfDrawNode *owner) {
    if (owner == NULL) {
        SdfModel *root = node->root;
        SdfDrawNode *first = root->rootNode;
        if (first != node) {
            root->rootNode = node;
            if (first != NULL) {
                node->children = first;
                first->parent = node;
            }
            node->parent = NULL;
        }
    } else if (node->parent != owner) {
        SdfDrawNode *first;
        sdfUnlinkRouteNode(node);
        first = owner->children;
        if (first == NULL) {
            owner->children = node;
        } else {
            SdfDrawNode *previous = first->previous;
            node->previous = previous;
            previous->next = node;
            first->previous = node;
            node->next = first;
        }
        node->parent = owner;
    }
}

INCLUDE_SDATA(const s32, "game/code_0032C278", sdfPendingQueueSemaphore);

INCLUDE_SDATA(const s32, "game/code_0032C278", sdfDoubleBufferAllocation);

INCLUDE_SDATA(const s32, "game/code_0032C278", sdfPacketBufferSize);

INCLUDE_SDATA(const s32, "game/code_0032C278", sdfPacketBuffers);

INCLUDE_SDATA(const s32, "game/code_0032C278", sdfPacketCursor);

INCLUDE_SDATA(const s32, "game/code_0032C278", sdfPacketBufferEnd);

INCLUDE_SDATA(const s32, "game/code_0032C278", D_00438A1C);

INCLUDE_SDATA(const s32, "game/code_0032C278", D_00438A1D);

INCLUDE_SDATA(const s32, "game/code_0032C278", D_00438A1E);

INCLUDE_SDATA(const s32, "game/code_0032C278", D_00438A20);

INCLUDE_SDATA(const s32, "game/code_0032C278", D_00438A21);

INCLUDE_SDATA(const s32, "game/code_0032C278", D_00438A22);

INCLUDE_SDATA(const s32, "game/code_0032C278", sdfPacketSlotIndex);

INCLUDE_SDATA(const s32, "game/code_0032C278", D_00438A24);

INCLUDE_SDATA(const s32, "game/code_0032C278", D_00438A28);

INCLUDE_SDATA(const s32, "game/code_0032C278", D_00438A2C);

INCLUDE_SDATA(const s32, "game/code_0032C278", D_00438A30);

INCLUDE_SDATA(const s32, "game/code_0032C278", D_00438A34);

