#include "common.h"
#include "sdf.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"

/* DMAC tag IDs occupy the high nibble of the header byte at offset three. */
#define SDF_DMA_TAG_NEXT_BYTE 0x20
#define SDF_DMA_TAG_REF_BYTE 0x30
#define SDF_DMA_TAG_CALL_BYTE 0x50
#define SDF_DMA_QWC_MASK 0xFFFF
#define SDF_DMA_ADDRESS_MASK 0x0FFFFFFF

/* First tag word carries the DMA operation; second is its 28-bit address. */
typedef struct SdfDmaTagHeader {
    u8 pad00[3];
    u8 kind;
    u32 address;
} SdfDmaTagHeader;

typedef struct SdfPacketChain {
    SdfListHead *head;
    SdfListHead *tail;
} SdfPacketChain;

typedef struct SdfPacketSlot {
    SdfListHead *list;
    u32 unk4;
    u8 state;
    u8 unk9;
    u8 pad0A[6];
} SdfPacketSlot;

extern u32 func_002CF930(void);
extern u32 sdfGetElapsedTimerTicks(u32);
extern s32 EIntr(void);
extern u16 D_003BD334;
extern s32 D_003BD340;
extern s8 D_003BDA05;
extern u8 sdfCurrentBufferIndex;

extern void *sdfPendingQueueHead;
extern s8 sdfPendingQueueRotationActive;
extern s32 sdfPendingQueueSlots[2];

extern SdfResource *sdfResourceListHead;

extern u32 sdfAllocSizeClassBlock(u32);
extern void sdfReleaseChipBlock(void *allocation);

extern u32 sdfCreateReferenceDmaNode(u32);

extern s32 sdfPacketBufferSize;
extern s32 sdfPacketBuffers[2];
extern s32 sdfPacketCursor;
extern s32 sdfPacketBufferEnd;

extern u64 sdfGraphHasPendingWork(void);
extern s32 func_00312C08(void);

extern s32 sdfPendingQueueSemaphore;
extern s32 sdfCreateSemaphore(u32, u32, u32);
extern u8 sdfTextureReleaseQueue;
extern u8 sdfObjectListReleaseQueue;
extern volatile s8 sdfPacketSlotIndex;
extern s32 D_003BD338;
extern SdfPacketSlot D_00398158[];
extern SdfResEntry *sdfPacketResourceEntries[];

void sdfTexRelease(void);
void sdfInitializeSynchronizedRequest(void *arg0, void (*arg1)(void));
void sdfPrependPacketList(SdfListHead *list, SdfListHead *item);
void func_002D5A68(SdfPacket *packet, u32 destinationBufferAddress, s32 destinationBufferWidth,
                  s64 destinationFormat, s64 destinationX, s64 destinationY,
                  u32 sourceBufferAddress, s32 sourceBufferWidth, s32 sourceFormat,
                  s32 sourceX, s32 sourceY, s32 transferWidth, s32 transferHeight, s32 transferDirection);
void sdfDestroyObjectList();
void sdfConnectPacketLists(SdfListHead *previous, SdfListHead *item);
void func_002D35B8();
void func_002D4DD0();
s32 sdfAllocPacketAligned(s32 size);
void sdfAppendPacketRange(SdfListHead *list, u32 packet, u32 end);
void sdfAppendLinkedPacketNode(SdfListHead *list, u32 *node);

typedef struct SdfSynchronizedRequest {
    u32 value;
    u32 state;
} SdfSynchronizedRequest;

extern void sdfReleaseQueuedResource(s32, s32);


/* Dev slot: buffered request, unit matrix at +0x20, two vectors at +0x60/+0x70. */
typedef struct SdfDevSlot {
    u32 request;
    u8 pad04[8];
    s32 resource;
    void *device;
    u8 pad14[6];
    s16 unk1A;
    u32 unk1C;
    u128 unitMatrix[4]; /* 0x20: four quadword rows */
    u128 zeroVector;    /* 0x60 */
    u128 unitScale;     /* 0x70: ones in XYZ */
} SdfDevSlot;

extern void *sdfAllocAndClearQuadwords(s32);

extern void *sdfDevCreateBufferedRequest(s32, s32, s32);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D33C8);

/* Search the linked resource registry by its numeric resource identifier. */
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

void sdfRegisterTextureReleaseRequestHandler(void) {
    sdfInitializeSynchronizedRequest(&sdfTextureReleaseQueue, sdfTexRelease);
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D35B8);

void sdfCreateResourcePacket(SdfListHead *list, s32 arg1, s32 arg2, s32 arg3, s32 arg4,
                   s32 arg5, s32 arg6, s32 arg7, s32 arg_sp0, s32 (*alloc)(s32)) {
    s32 buffer;

    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    buffer = alloc(0xf0);
    func_002D35B8(buffer, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg_sp0);
    sdfAppendPacketRange(list, buffer, buffer + 0xc0);
}

void sdfPatchPacketResourceField(SdfBigPacket *packet, s32 entryIndex) {
    packet->unk80 = (packet->unk80 & ~0x3FFF) | (u64)(u32)(sdfPacketResourceEntries[entryIndex]->baseAddress >> 6);
}

void sdfCreatePatchableResourcePacket(SdfListHead *list, SdfListHead *linkedList, s32 arg2, s32 arg3,
                   s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8,
                   s32 (*alloc)(s32)) {
    s32 packet;

    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    packet = alloc(0x100);
    ((SdfNode *)packet)->unk4 = (u32)sdfPatchPacketResourceField;
    func_002D35B8(packet + 0x10, sdfPacketResourceEntries[0], arg2, arg3, arg4, arg5, arg6, arg7, arg8);
    sdfAppendLinkedPacketNode(linkedList, (u32 *)packet);
    sdfAppendPacketRange(list, packet + 0x10, packet + 0xD0);
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D39B0);

void sdfCreateDescriptorPacket(SdfListHead *list, s32 source, s32 a, s32 b, s32 c, s32 d, s32 e,
                   s32 (*alloc)(s32)) {
    s32 block;
    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    block = alloc(0xB0);
    func_002D39B0(block, source, a, b, c, d, e);
    sdfAppendPacketRange(list, block, block + 0x80);
}

/* Register a completion callback, creating the shared semaphore on first use. */
void sdfInitializeSynchronizedRequest(void *request, void (*callback)(void)) {
    void **head = request;

    if (sdfPendingQueueSemaphore < 0) {
        sdfPendingQueueSemaphore = sdfCreateSemaphore(1, 0x7f, 0);
    }
    head[0] = (void *)callback;
    head[1] = NULL;
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
    SdfPendingNode *node;
    SdfPendingBuffer *buffer;
    s32 remaining;

    if (entry != 0) {
        WaitSema(sdfPendingQueueSemaphore);
        node = owner->pending;
        if (node == NULL) {
            node = (SdfPendingNode *)sdfAllocSizeClassBlock(0x10);
            node->remaining = 0;
            node->next = sdfPendingQueueHead;
            node->buffer = NULL;
            node->owner = owner;
            owner->pending = node;
            sdfPendingQueueHead = node;
        }
        remaining = node->remaining;
        buffer = node->buffer;
        if (remaining == 0) {
            SdfPendingBuffer *fresh = (SdfPendingBuffer *)sdfAllocSizeClassBlock(0x100);
            fresh->next = buffer;
            node->buffer = fresh;
            buffer = fresh;
            remaining = 0x3F;
        }
        buffer->entry[0x3F - remaining] = entry;
        node->remaining = remaining - 1;
        SignalSema(sdfPendingQueueSemaphore);
    }
}

typedef struct SdfQueueNode {
    struct SdfQueueNode *next;
    struct SdfQueueNode *link;
} SdfQueueNode;

SdfQueueNode *sdfDetachQueue(void) {
    SdfQueueNode *head = sdfPendingQueueHead;
    SdfQueueNode *node;

    sdfPendingQueueHead = NULL;
    for (node = head; node != NULL; node = node->next) {
        node->link->link = NULL;
    }
    return head;
}

/* Run each node's handler over its queued entries, freeing chunks and nodes. */
void sdfPendingQueueFlush(SdfPendingNode *node) {
    SdfPendingNode *nextNode;
    SdfPendingBuffer *buffer;
    SdfPendingBuffer *nextBuffer;
    void (*handler)(u32);
    s32 count;
    s32 i;

    if (node != NULL) {
        do {
            count = 0x3F - node->remaining;
            handler = node->owner->handler;
            buffer = node->buffer;
            if (buffer != NULL) {
                do {
                    i = 0;
                    do {
                        handler(buffer->entry[i]);
                        i++;
                    } while (i < count);
                    nextBuffer = buffer->next;
                    sdfReleaseChipBlock(buffer);
                    buffer = nextBuffer;
                    count = 0x3F;
                } while (buffer != NULL);
            }
            nextNode = node->next;
            sdfReleaseChipBlock(node);
            node = nextNode;
        } while (node != NULL);
    }
}

/* Hand the oldest pending slot to the worker, shift the slot list down and
 * refill the last slot with the newly detached queue. */
void sdfRotatePendingSlots(void) {
    u32 i;

    sdfPendingQueueRotationActive = 1;
    WaitSema(sdfPendingQueueSemaphore);
    sdfPendingQueueFlush(sdfPendingQueueSlots[0]);
    for (i = 0; i < 1; i++) {
        sdfPendingQueueSlots[i] = sdfPendingQueueSlots[i + 1];
    }
    sdfPendingQueueSlots[1] = (s32)sdfDetachQueue();
    SignalSema(sdfPendingQueueSemaphore);
    sdfPendingQueueRotationActive = 0;
}

u64 sdfGraphHasPendingWork(void) {
    u32 i;
    s32 *entry;
    if (sdfPendingQueueRotationActive != 0) {
        return 1;
    }
    if (sdfPendingQueueHead != NULL) {
        return 1;
    }
    i = 0;
    entry = sdfPendingQueueSlots;
    do {
        if (*entry != 0) {
            return 1;
        }
        entry++;
        i++;
    } while (i < 2);
    return 0;
}

u64 sdfCheckPendingWorkWithInterrupts(void) {
    s64 interruptState;
    u64 pendingWork;

    interruptState = func_00312C08();
    pendingWork = sdfGraphHasPendingWork();
    if (interruptState != 0) {
        EIntr();
    }
    return pendingWork;
}

extern s32 sdfDoubleBufferAllocation;
extern void sdfReleaseResourceAllocation(s32);
extern s32 sdfAllocGeneralBlock(s32);
extern s32 sdfResourceRetainAddress(s32);

/* Reallocate two adjacent, 128-byte-aligned packet workspaces. */
void sdfResizeDoubleBuffer(s32 size) {
    s32 memory;
    if (sdfDoubleBufferAllocation != 0) {
        sdfReleaseResourceAllocation(sdfDoubleBufferAllocation);
        sdfDoubleBufferAllocation = 0;
    }
    size = (size + 0x7F) & ~0x7F;
    sdfPacketBufferSize = size;
    sdfDoubleBufferAllocation = sdfAllocGeneralBlock(size * 2);
    memory = sdfResourceRetainAddress(sdfDoubleBufferAllocation);
    sdfPacketBuffers[0] = memory;
    sdfPacketBuffers[1] = memory + size;
}

void sdfSelectDoubleBuffer(s32 index) {
    sdfPacketCursor = sdfPacketBuffers[index];
    sdfPacketBufferEnd = sdfPacketBuffers[index] + sdfPacketBufferSize;
}

s32 sdfGetBufferRemaining(void) {
    return sdfPacketBufferEnd - sdfPacketCursor;
}

s32 sdfAllocPacketAligned(s32 size) {
    s32 packet;

    packet = sdfPacketCursor;
    sdfPacketCursor = sdfPacketCursor + ((size + 0xfU) & 0xfffffff0);
    return packet;
}

s32 sdfGetPacketCursor(void) {
    return sdfPacketCursor;
}

void sdfSetPacketCursorAligned(s32 cursor) {
    sdfPacketCursor = (cursor + 0xF) & ~0xF;
}

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

/* Chain a DMA packet by rewriting the previous packet's tag to NEXT. */
void sdfAppendPacket(SdfListHead *list, u32 packet) {
    s32 last;

    last = list->last;
    if (last == 0) {
        list->first = packet;
    }
    else {
        ((SdfDmaTagHeader *)last)->kind = SDF_DMA_TAG_NEXT_BYTE;
        ((SdfDmaTagHeader *)last)->address = packet & 0xfffffff;
    }
    list->last = packet;
}

void sdfAppendPacketRange(SdfListHead *list, u32 packet, u32 end) {
    s32 last;

    last = list->last;
    if (last == 0) {
        list->first = packet;
    }
    else {
        ((SdfDmaTagHeader *)last)->kind = SDF_DMA_TAG_NEXT_BYTE;
        ((SdfDmaTagHeader *)last)->address = packet & 0xfffffff;
    }
    list->last = end;
}

/* REF packets carry an extra quadword after the DMA tag. */
void sdfAppendReferencePacket(SdfListHead *list, u32 packet) {
    s32 last;

    ((SdfDmaTagHeader *)packet)->kind = SDF_DMA_TAG_REF_BYTE;
    last = list->last;
    if (last == 0) {
        list->first = packet;
    }
    else {
        ((SdfDmaTagHeader *)last)->kind = SDF_DMA_TAG_NEXT_BYTE;
        ((SdfDmaTagHeader *)last)->address = packet & 0xfffffff;
    }
    list->last = packet + 0x10;
}

void sdfAppendDmaTagToList(SdfListHead *list, u32 packet) {
    s32 last;

    last = list->last;
    if (last == 0) {
        list->first = packet;
    }
    else {
        ((SdfDmaTagHeader *)last)->kind = SDF_DMA_TAG_NEXT_BYTE;
        ((SdfDmaTagHeader *)last)->address = packet & 0xfffffff;
    }
    list->last = packet + 0x30;
}

/* CALL packets likewise end one quadword after their tag. */
void sdfAppendCallPacket(SdfListHead *list, u32 packet) {
    s32 last;

    ((SdfDmaTagHeader *)packet)->kind = SDF_DMA_TAG_CALL_BYTE;
    last = list->last;
    if (last == 0) {
        list->first = packet;
    }
    else {
        ((SdfDmaTagHeader *)last)->kind = SDF_DMA_TAG_NEXT_BYTE;
        ((SdfDmaTagHeader *)last)->address = packet & 0xfffffff;
    }
    list->last = packet + 0x10;
}

void sdfPrependPacketList(SdfListHead *list, SdfListHead *item) {
    SdfListHead *head;

    if (item->last == 0) {
        return;
    }
    head = (SdfListHead *)list->first;
    if (head == NULL) {
        list->last = (u32)item;
    } else {
        sdfConnectPacketLists(item, head);
    }
    item->unk0 = (u32)head;
    list->first = (u32)item;
}

void sdfAppendPacketList(SdfListHead *list, SdfListHead *item) {
    s32 *last;

    if (item->first != 0) {
        last = (s32 *)list->last;
        if (last == NULL) {
            list->first = (u32)item;
        }
        else {
            *last = (s32)item;
            sdfConnectPacketLists(last, item);
        }
        list->last = (u32)item;
    }
}

s32 sdfPrependIfMode1(SdfListHead *list, s32 mode, SdfListHead *packet) {
    if (mode == 1) {
        sdfPrependPacketList(list, packet);
    }
}

/* Pool entry with a packet list and the handlers used to append/prepend lists. */
typedef struct SdfPoolNode {
    struct SdfPoolNode *next; /* 0x0 */
    u32 first;                /* 0x4 */
    u32 last;                 /* 0x8 */
    u32 unkC;
    void (*append)(SdfListHead *, SdfListHead *);      /* 0x10 */
    s32 (*prepend)(SdfListHead *, s32, SdfListHead *); /* 0x14 */
    u32 unk18;
    u32 unk1C;
} SdfPoolNode;

void func_002D4240(SdfPoolNode *node, s32 count) {
    node->append = sdfAppendPacketList;
    node->prepend = sdfPrependIfMode1;
    node->first = 0;
    node->last = 0;
    node->unkC = 0;
    node->unk18 = 0;
    count--;
    if (count == 0) {
        node->next = NULL;
        return;
    }
next_node:
    node->next = node + 1;
    node++;
    node->append = sdfAppendPacketList;
    node->prepend = sdfPrependIfMode1;
    node->first = 0;
    node->last = 0;
    node->unkC = 0;
    node->unk18 = 0;
    if (--count != 0) {
        goto next_node;
    }
    node->next = NULL;
}

/* Make a REF DMA node for the payload following the source tag. */
u32 sdfCreateReferenceDmaNode(u32 source) {
    SdfDmaNode *node = (SdfDmaNode *)sdfAllocPacketAligned(0x20);
    SdfDmaSrc *src = (SdfDmaSrc *)source;
    u64 tag = src->quadwordCount;
    u32 address = ((u32)src + 0x10) & 0x0FFFFFFF;
    s64 shifted = (s64)address << 32;

    tag |= 0x30000000;
    tag |= shifted;
    node->unk0 = tag;
    node->unk10 = 0;
    node->unk8 = src->vifCommands;
    return (u32)node;
}

/* Patch the prior NEXT tag to chain in a reference to source's payload. */
s32 sdfLinkReferenceDmaNode(s32 previous, u32 source) {
    u32 packet;

    packet = sdfCreateReferenceDmaNode(source);
    ((SdfDmaTagHeader *)previous)->kind = SDF_DMA_TAG_NEXT_BYTE;
    ((SdfDmaTagHeader *)previous)->address = packet & 0xfffffff;
    return packet + 0x10;
}

void sdfConnectPacketLists(SdfListHead *previous, SdfListHead *item) {
    u32 head = previous->last;
    u32 source;
    u32 pending;

    source = previous->firstReferenceSource;
    pending = item->firstReferenceSource;
    if (source != pending) {
        if (pending != 0) {
            head = sdfLinkReferenceDmaNode(head, pending);
        } else {
            item->firstReferenceSource = source;
        }
    }
    source = previous->secondReferenceSource;
    pending = item->secondReferenceSource;
    if (source != pending) {
        if (pending != 0) {
            head = sdfLinkReferenceDmaNode(head, pending);
        } else {
            item->secondReferenceSource = source;
        }
    }
    ((SdfDmaTagHeader *)head)->kind = SDF_DMA_TAG_NEXT_BYTE;
    ((SdfDmaTagHeader *)head)->address = item->first & 0x0FFFFFFF;
}

typedef struct SdfRefNode {
    u8 pad00[0x10];
    u64 chain; /* 0x10: NEXT tag chaining to the previous head */
} SdfRefNode;

/* Give each pending reference (+0x14, then +0x10) its own DMA node, chained in front of the list. */
void sdfChainReferenceNodes(SdfListHead *list) {
    SdfRefNode *node;
    u32 address;
    u32 source;

    source = list->secondReferenceSource;
    if (source != 0) {
        node = (SdfRefNode *)sdfCreateReferenceDmaNode(source);
        address = list->first & 0xFFFFFFF;
        list->first = (u32)node;
        node->chain = ((s64)address << 32) | 0x20000000;
    }
    source = list->firstReferenceSource;
    if (source != 0) {
        node = (SdfRefNode *)sdfCreateReferenceDmaNode(source);
        address = list->first & 0xFFFFFFF;
        list->first = (u32)node;
        node->chain = ((s64)address << 32) | 0x20000000;
    }
}
/* Flush every pool entry, chain the packet lists together and terminate the last. */
s32 sdfFlushPoolNodes(SdfPoolNode *node) {
    SdfPoolNode *tail = NULL;
    s32 head = 0;

    for (; node != NULL; node = node->next) {
        node->prepend((SdfListHead *)node, 0, NULL);
        if (node->first != 0) {
            if (head != 0) {
                sdfConnectPacketLists(tail, node->first);
            } else {
                head = node->first;
                sdfChainReferenceNodes((SdfListHead *)head);
            }
            tail = (SdfPoolNode *)node->last;
        }
    }
    if (tail != NULL) {
        ((SdfDmaTagHeader *)tail->last)->kind = 0x70;
        ((SdfDmaTagHeader *)tail->last)->address = 0;
    }
    return head;
}

void sdfClearLinkedPacketList(SdfListHead *list) {
    list->unk0 = 0;
    list->first = 0;
    list->last = 0;
    list->unkC = 0;
}

void sdfAppendLinkedPacketNode(SdfListHead *list, u32 *node) {
    if (list->last == 0) {
        list->first = (u32)node;
    }
    else {
        *(u32 *)list->last = (u32)node;
    }
    list->last = (u32)node;
    *node = 0;
}

void sdfClearPacketListHead(SdfListHead *list) {
    list->unk0 = 0;
    list->first = 0;
}

void sdfAppendPacketChainNode(SdfPacketChain *head, SdfListHead *node) {
    if (head->tail == NULL) {
        head->head = node;
    }
    else {
        *(u32 *)head->tail->last = node->last;
    }
    head->tail = node;
}

/* Encode the packed A+D payload count and its DMA/VIF transfer length. */
void sdfInitializeDmaReferenceTag(SdfPacket *packet, s32 payloadQwords) {
    s64 dmaQwords;

    dmaQwords = payloadQwords + 1;
    packet->unk8 = (((dmaQwords | 0x50000000) << 32) | 0x10000000);
    packet->unk10 = (payloadQwords | (((s64)0x10000000 << 32) | 0x8000));
    packet->unk0 = dmaQwords;
    packet->unk18 = 0xE;
}

/* Emit a GIF header, a REF tag with masked count/address, and the trailing NEXT tag. */
void sdfBuildDmaReferenceChain(u64 *packet, u32 sourceAddress, s32 qwordCount) {
    packet[0] = 0x10000001;
    packet[1] = 0x5000000110000000ULL;
    packet[2] = (u64)qwordCount | 0x1000000000008000ULL;
    packet[3] = 0xE;
    packet[4] = (u32)((qwordCount & SDF_DMA_QWC_MASK) | 0x30000000) | ((u64)(sourceAddress & SDF_DMA_ADDRESS_MASK) << 32);
    packet[5] = 0x5000000100000000ULL;
    packet[6] = 0x20000000;
    packet[7] = 0;
}

void sdfBuildFrameDepthScissorPacket(SdfPacket *packet, s32 frameAddress, s32 width, s32 height,
                  s32 frameFormat, s32 depthAddress, s32 depthFormat,
                  s32 fieldOffset, s32 context) {
    s64 frameRegister;
    s64 depthRegister;
    s64 offsetRegister;
    s64 scissorRegister;
    s32 x;
    s32 y;

    if (context == 0) {
        frameRegister = 0x4C;
        depthRegister = 0x4E;
        offsetRegister = 0x18;
        scissorRegister = 0x40;
    } else {
        frameRegister = 0x4D;
        depthRegister = 0x4F;
        offsetRegister = 0x19;
        scissorRegister = 0x41;
    }
    packet[1].unk18 = scissorRegister;
    y = (0x1000 - height) << 3;
    x = (0x1000 - width) << 3;
    packet[0].unk0 = (s64)(frameFormat << 24) | (s64)(((width + 63) >> 6) << 16) | (s64)(frameAddress >> 11);
    if (fieldOffset != 0) {
        y += 8;
    }
    packet[0].unk8 = frameRegister;
    packet[0].unk10 = (s64)(depthFormat << 24) | (s64)(depthAddress >> 11);
    packet[0].unk18 = depthRegister;
    packet[1].unk8 = offsetRegister;
    packet[1].unk10 = ((s64)(height - 1) << 48) | ((s64)(width - 1) << 16);
    packet[1].unk0 = x | ((s64)y << 32);
}

/* Encode centered viewport bounds in GS coordinate words; the last two arguments are unused. */
void sdfBuildCenteredViewBoundsPacket(u64 *packet, s32 width, s32 height, s32 unused0, s32 unused1) {
    u32 lowerBounds = ((0x1000 - height) << 19) | ((0x1000 - width) << 3);
    u32 upperBounds = ((height + 0x1000) << 19) | ((width + 0x1000) << 3);

    packet[3] = 0;
    packet[7] = 5;
    packet[0] = 0x30003;
    packet[1] = 0x47;
    packet[2] = 6;
    packet[4] = (u64)0xFE00 << 46;
    packet[5] = 1;
    packet[6] = lowerBounds;
    packet[8] = upperBounds;
    packet[9] = 5;
}

/* Seed PRMODECONT, COLCLAMP, DTHE and TEXA drawing registers. */
void sdfInitDrawPacket(u64 *packet) {
    packet[0] = 1;
    packet[1] = 0x1A;
    packet[2] = 1;
    packet[3] = 0x46;
    packet[4] = 0;
    packet[5] = 0x45;
    packet[6] = 0x4000000080ULL;
    packet[7] = 0x3B;
}

extern void sdfBuildFrameDepthScissorPacket(SdfPacket *, s32, s32, s32, s32, s32, s32, s32, s32);

/* Build the common header and FRAME/ZBUF/XYOFFSET/SCISSOR state for one GS context. */
void sdfBuildSceneDrawHeader(SdfPacket *packet, s32 frameAddress, s32 width, s32 height,
                           s32 frameFormat, s32 depthAddress, s32 depthFormat, s32 context) {
    sdfInitializeDmaReferenceTag(packet, 5);
    sdfBuildFrameDepthScissorPacket(packet + 1, frameAddress, width, height, frameFormat, depthAddress, depthFormat, 0, context);
}

typedef struct SdfBufferRef {
    u8 pad00[0xC];
    s32 bufferAddress; /* 0xC */
} SdfBufferRef;

typedef struct SdfRenderTargetView {
    s16 width;       /* 0x0 */
    u8 pad2[2];
    s16 height;      /* 0x4 */
    u8 frameFormat;  /* 0x6 */
    u8 depthFormat;  /* 0x7 */
    SdfBufferRef *buffers[3]; /* 0x8 */
} SdfRenderTargetView;

typedef struct SdfSceneDrawPacket {
    SdfPacket header;    /* 0x00 */
    u64 draw[8];         /* 0x20 */
    SdfPacket contextOne[2]; /* 0x60 */
    SdfPacket contextTwo[2]; /* 0xA0 */
    u64 limits[10];      /* 0xE0 */
    u64 regs[8];         /* 0x130 */
} SdfSceneDrawPacket;

extern u8 D_003BD332;

/* Build both GS drawing contexts from the selected frame buffer and shared depth buffer. */
void sdfBuildTextureScenePacket(SdfSceneDrawPacket *packet, SdfRenderTargetView *view, s32 bufferIndex) {
    s32 frameAddress;
    s32 depthAddress;
    s32 width;
    s32 height;
    s32 frameFormat;
    s32 depthFormat;

    sdfInitializeDmaReferenceTag(&packet->header, 0x15);
    frameAddress = view->buffers[bufferIndex]->bufferAddress;
    depthAddress = view->buffers[2]->bufferAddress;
    width = view->width;
    frameFormat = view->frameFormat;
    height = view->height;
    depthFormat = view->depthFormat;
    sdfBuildFrameDepthScissorPacket(packet->contextOne, frameAddress, width, height, frameFormat, depthAddress, depthFormat, D_003BD332, 0);
    sdfBuildFrameDepthScissorPacket(packet->contextTwo, frameAddress, width, height, frameFormat, depthAddress, depthFormat, D_003BD332, 1);
    sdfBuildCenteredViewBoundsPacket(packet->limits, view->width, view->height, view->frameFormat, view->depthFormat);
    packet->regs[0] = 0x517FB;
    packet->regs[1] = 0x47;
    packet->regs[2] = 0x44;
    packet->regs[3] = 0x42;
    packet->regs[4] = 0x517FB;
    packet->regs[5] = 0x48;
    packet->regs[6] = 0x44;
    packet->regs[7] = 0x43;
    sdfInitDrawPacket(packet->draw);
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D49E8);

typedef struct SdfRenderTargetInfo {
    s16 width;       /* 0x0 */
    u8 pad2[2];
    s16 height;      /* 0x4 */
    u8 frameFormat;  /* 0x6 */
    u8 depthFormat;  /* 0x7 */
} SdfRenderTargetInfo;

typedef struct SdfSceneNode {
    u8 pad00[4];
    void (*handler)(); /* 0x4 */
    SdfRenderTargetInfo *view; /* 0x8 */
    u8 padC[4];
    SdfPacket header;  /* 0x10 */
    u64 draw[24];      /* 0x30 */
    u64 limits[10];    /* 0xF0 */
    u64 regs[8];       /* 0x140 */
} SdfSceneNode;

extern void func_002D49E8();

/* Retain the render-target view and initialize the scene callback and fixed drawing state. */
void sdfInitSceneNode(SdfSceneNode *node, SdfRenderTargetInfo *view) {
    sdfInitializeDmaReferenceTag(&node->header, 0x15);
    node->view = view;
    node->handler = func_002D49E8;
    sdfBuildCenteredViewBoundsPacket(node->limits, view->width, view->height, view->frameFormat, view->depthFormat);
    node->regs[0] = 0x517FB;
    node->regs[1] = 0x47;
    node->regs[2] = 0x44;
    node->regs[3] = 0x42;
    node->regs[4] = 0x517FB;
    node->regs[5] = 0x48;
    node->regs[6] = 0x44;
    node->regs[7] = 0x43;
    sdfInitDrawPacket(node->draw);
}

void sdfAppendLinkedPacketPayload(SdfListHead *list, SdfListHead *other, u32 *node) {
    sdfAppendLinkedPacketNode(other, node);
    sdfAppendPacket(list, (s32)node + 0x10);
}

void func_002D4C80(s32 source, u32 packet, s32 variant) {
    if (variant == 0) {
        sdfBuildDmaReferenceChain(packet, source + 0x180, 1);
        return;
    }
    sdfBuildDmaReferenceChain(packet, source + 400, 1);
}

void func_002D4CC8(s32 source, u32 packet, s32 variant) {
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
    node->unk0 = ((u64)((source + 0x1e0) & 0xfffffff) << 32) | 0x30000004;
    node->unk10 = 0;
    sdfAppendReferencePacket(list, (u32)node);
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D4DD0);

void sdfInitPacketBuilder(SdfPacketBuilder *packet, s32 source, s32 data, s32 region, s32 mode) {
    sdfInitializeDmaReferenceTag(packet->packets, 2);
    packet->mode = mode;
    packet->source = source;
    packet->data = data;
    packet->region = region;
    packet->prepare = func_002D4DD0;
}

void func_002D4EE8(SdfListHead *list, SdfPacketChain *chain) {
    u32 start = func_002CF930();
    s32 interrupts;
    s32 index;
    SdfPacketSlot *slot;

    while (sdfPacketSlotIndex != 0) {
    }
    D_003BD334 = sdfGetElapsedTimerTicks(start);
    interrupts = func_00312C08();
    index = D_003BD338 + 1;
    if (index == 3) {
        index = 0;
    }
    slot = &D_00398158[index];
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
    D_003BD338 = index;
    D_003BDA05 = 1;
    D_003BD340++;
    if (interrupts != 0) {
        EIntr();
    }
}

void sdfSetNonnegativePacketIndex(s32 value) {
    sdfPacketSlotIndex = (value < 0) ? 0 : value;
}

void sdfResetPacketSlotState(void) {
    sdfPacketSlotIndex = 0;
    D_00398158[0].list = NULL;
    D_003BD338 = 0;
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D5018);

extern vu8 D_003BD32D;

/* Wait for the shared busy flag, then sleep until the active slot is empty or ready. */
void sdfWaitSlotReady(void) {
    SdfPacketSlot *table = D_00398158;

    for (;;) {
        while (D_003BD32D != 0) {
        }
        if (table[D_003BD338].list == NULL) {
            break;
        }
        if (table[D_003BD338].state >= 2) {
            break;
        }
        sdfSleepThreadCount(0);
    }
}

s32 sdfAllocatePacketList(s32 (*alloc)(s32)) {
    s32 list;

    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    list = alloc(0x20);
    sdfInitPacketList((SdfListHead *)list);
    return list;
}

void sdfAppendInitializedPacket(s32 list, void (*initialize)(s32), s32 size, s32 (*alloc)(s32)) {
    s32 packet;

    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    packet = alloc(size);
    initialize(packet);
    sdfAppendPacket(list, packet);
}

void sdfInitPrimaryAlphaBlendRegisters(SdfPacket *packet) {
    packet->unk0 = 0x717FB;
    packet->unk8 = 0x47;
    packet->unk10 = 0x44;
    packet->unk18 = 0x42;
}

void sdfInitSecondaryAlphaBlendRegisters(SdfPacket *packet) {
    packet->unk0 = 0x717FB;
    packet->unk8 = 0x48;
    packet->unk10 = 0x44;
    packet->unk18 = 0x43;
}

void sdfBuildPrimaryAlphaBlendDmaPacket(SdfPacket *packet) {
    sdfInitPrimaryAlphaBlendRegisters(packet + 1);
    packet->unk0 = 3;
    packet->unk8 = (((u64)0x50000003 << 16 | 0x1000) << 16);
    packet->unk10 = (((u64)0x10000000 << 32) | 0x8002);
    packet->unk18 = 0xE;
}

void sdfBuildSecondaryAlphaBlendDmaPacket(SdfPacket *packet) {
    sdfInitSecondaryAlphaBlendRegisters(packet + 1);
    packet->unk0 = 3;
    packet->unk8 = (((u64)0x50000003 << 16 | 0x1000) << 16);
    packet->unk10 = (((u64)0x10000000 << 32) | 0x8002);
    packet->unk18 = 0xE;
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

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D5A68);

/* Wrap BITBLTBUF, TRXPOS, TRXREG and TRXDIR image-transfer settings in a DMA/GIF header. */
void sdfInitializeExtendedDrawPacket(SdfPacket *packet, u32 destinationBufferAddress, s32 destinationBufferWidth,
                                    s64 destinationFormat, s64 destinationX, s64 destinationY,
                                    u32 sourceBufferAddress, s32 sourceBufferWidth, s32 sourceFormat,
                                    s32 sourceX, s32 sourceY, s32 transferWidth, s32 transferHeight, s32 transferDirection) {
    packet->unk0 = 5;
    packet->unk8 = (((u64)0x50000005 << 16 | 0x1000) << 16);
    packet->unk10 = (((u64)0x10000000 << 32) | 0x8004);
    packet->unk18 = 0xE;
    func_002D5A68(packet + 1, destinationBufferAddress, destinationBufferWidth,
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

typedef struct SdfExtendedPacketSource {
    u8 pad00[0xC];
    u32 resourceWord;
    u8 pad10[4];
    s16 formatSelector;
    u8 pad16[2];
    s32 pixelFormat;
} SdfExtendedPacketSource;

typedef struct SdfGraphPacketState {
    s16 width;
    s16 unk2;
    s16 height;
    u8 bufferMode;
    u8 auxiliaryMode;
    SdfTexResource *buffers[3];
} SdfGraphPacketState;

extern SdfGraphPacketState D_003980E0;

/* Wrap an extended texture draw packet with a patchable resource header. */
void func_002D5CD0(SdfListHead *drawList, SdfListHead *linkedList,
                   SdfExtendedPacketSource *source, s32 arg3, s32 arg4,
                   s32 arg5, s32 arg6, s32 arg7, s32 arg_sp0,
                   s32 arg_sp8, s32 (*allocPacket)(s32)) {
    SdfNode *packet;
    SdfPacket *drawPacket;

    if (allocPacket == NULL) {
        allocPacket = sdfAllocPacketAligned;
    }
    packet = (SdfNode *)allocPacket(0x70);
    packet->unk8 = arg_sp8;
    packet->unk4 = (u32)sdfPatchPacketResourceReference;
    drawPacket = (SdfPacket *)((u8 *)packet + 0x10);

    sdfInitializeExtendedDrawPacket(
        drawPacket, source->resourceWord, source->formatSelector,
        source->pixelFormat, arg3, arg4, D_003980E0.buffers[0]->word,
        D_003980E0.width, D_003980E0.bufferMode, arg5, arg6, arg7,
        arg_sp0, 2);
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
void sdfBuildTriPacket104(s32 dstAddr, s32 color, s32 primitive, s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, s32 depth) {
    u64 *dst = (u64 *)dstAddr;
    u64 depthHigh = (u64)depth << 32;
    u32 first = ((u32)x0 & 0xFFFF) | ((u32)y0 << 16);
    u32 second = ((u32)x1 & 0xFFFF) | ((u32)y1 << 16);
    u32 third = ((u32)x2 & 0xFFFF) | ((u32)y2 << 16);

    dst[0] = 0x6400000000008001ULL;
    dst[1] = 0xF55510;
    dst[2] = (u32)primitive | 0x104;
    dst[3] = (u32)color | 0x3F80000000000000ULL;
    dst[4] = first | depthHigh;
    dst[5] = second | depthHigh;
    dst[6] = third | depthHigh;
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
    sdfBuildTriPacket104((s32)&packet->unk10, color, primitive, x0, y0, x1, y1, x2, y2, depth);
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

extern void sdfBuildPacket104x4(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

void sdfBuildPacketE(SdfListHead *list, s32 color, s32 primitive, s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, s32 x3, s32 y3, s32 depth, s32 (*alloc)(s32)) {
    s32 buffer;

    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    buffer = alloc(0x50);
    *(u64 *)buffer = 0x20000004ULL;
    *(u64 *)(buffer + 8) = 0x5000000410000000ULL;
    sdfBuildPacket104x4(buffer + 0x10, color, primitive, x0, y0, x1, y1, x2, y2, x3, y3, depth);
    sdfAppendPacket(list, buffer);
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

extern void sdfBuildPacket10C(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

void sdfBuildPacketF(SdfListHead *list, s32 primitive, s32 x0, s32 y0, s32 color0, s32 x1, s32 y1, s32 color1, s32 x2, s32 y2, s32 color2, s32 depth, s32 (*alloc)(s32)) {
    s32 buffer;

    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    buffer = alloc(0x60);
    *(u64 *)buffer = 0x20000005ULL;
    *(u64 *)(buffer + 8) = 0x5000000510000000ULL;
    sdfBuildPacket10C(buffer + 0x10, primitive, x0, y0, color0, x1, y1, color1, x2, y2, color2, depth);
    sdfAppendPacket(list, buffer);
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
void sdfBuildFillPacket106(s32 dstAddr, s32 color, s32 primitive, s32 left, s32 top, s32 right, s32 bottom, s32 depth) {
    u64 *dst = (u64 *)dstAddr;
    u64 depthHigh = (u64)(u32)depth << 32;
    u32 first = ((u32)left & 0xFFFF) | ((u32)top << 16);
    u32 second = ((u32)right & 0xFFFF) | ((u32)bottom << 16);

    dst[0] = 0x4400000000008001ULL;
    dst[1] = 0x5510;
    dst[2] = (u32)primitive | 0x106;
    dst[3] = (u32)color | 0x3F80000000000000ULL;
    dst[4] = first | depthHigh;
    dst[5] = second | depthHigh;
}

extern void sdfBuildFillPacket106(s32, s32, s32, s32, s32, s32, s32, s32);

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

void sdfBuildFillPacket101(s32 dstAddr, s32 color, s32 primitive, s32 left, s32 top, s32 right, s32 bottom, s32 depth) {
    u64 *dst = (u64 *)dstAddr;
    u64 depthHigh = (u64)(u32)depth << 32;
    u32 first = ((u32)left & 0xFFFF) | ((u32)top << 16);
    u32 second = ((u32)right & 0xFFFF) | ((u32)bottom << 16);

    dst[0] = 0x4400000000008001ULL;
    dst[1] = 0x5510;
    dst[2] = (u32)primitive | 0x101;
    dst[3] = (u32)color | 0x3F80000000000000ULL;
    dst[4] = first | depthHigh;
    dst[5] = second | depthHigh;
}

extern void sdfBuildFillPacket101(s32, s32, s32, s32, s32, s32, s32, s32);

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
void sdfBuildQuadPacket(s32 dstAddr, s32 color, s32 primitive, s32 left, s32 top, s32 right, s32 bottom, s32 depth) {
    u64 *dst = (u64 *)dstAddr;
    u64 depthHigh = (u64)(u32)depth << 32;
    u32 x0 = (u32)left & 0xFFFF;
    u32 x1 = (u32)right & 0xFFFF;
    u32 y0 = (u32)top << 16;
    u32 y1 = (u32)bottom << 16;

    dst[0] = 0x8400000000008001ULL;
    dst[1] = (s32)0xF5555510;
    dst[2] = (u32)primitive | 0x102;
    dst[3] = (u32)color | 0x3F80000000000000ULL;
    dst[4] = (x0 | y0) | depthHigh;
    dst[5] = (x1 | y0) | depthHigh;
    dst[6] = (x1 | y1) | depthHigh;
    dst[7] = (x0 | y1) | depthHigh;
    dst[8] = (x0 | y0) | depthHigh;
}

extern void sdfBuildQuadPacket(s32, s32, s32, s32, s32, s32, s32, s32);

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

void sdfInitializeObjectListRequest(void) {
    sdfInitializeSynchronizedRequest(&sdfObjectListReleaseQueue, sdfDestroyObjectList);
}

typedef struct SdfModelNodeDefaults {
    void *next;
    void *previous;
    u8 pad08[0x0E];
    s16 index;
    u8 pad18[4];
    u32 color;
    u8 pad20[0x30];
    u128 zeroRotation;
    u128 zeroPosition;
    u128 unitScale;
    u128 unitMatrix[4];
} SdfModelNodeDefaults;

SdfModelNodeDefaults *func_002D7830(void) {
    SdfModelNodeDefaults *node = sdfAllocAndClearQuadwords(0x100);

    node->color = 0x80808080;
    node->index = -1;
    node->next = node;
    node->previous = node;
    VU0_STORE_VF($vf0, &node->zeroRotation);
    VU0_STORE_VF($vf0, &node->zeroPosition);
    VU0_SET_ONES_XYZ($vf10);
    VU0_STORE_VF($vf10, &node->unitScale);
    EE_MMI_UNIT_MATRIX(node->unitMatrix);
    return node;
}

typedef struct SdfFreeNode {
    struct SdfFreeNode *next;
    u8 pad04[8];
    s32 allocation;
} SdfFreeNode;

typedef struct SdfFreeRoot {
    u8 pad00[0x28];
    SdfFreeNode *lists[2]; /* 0x28 */
    void *workspace;       /* 0x30 */
} SdfFreeRoot;

void sdfEnsureFreeRootWorkspace(SdfFreeRoot *root) {
    u32 workspace;

    if (root->workspace == NULL) {
        workspace = sdfAllocSizeClassBlock(0x100);
        root->workspace = (void *)workspace;
    }
}

extern void sdfReleaseResourceAllocation(s32 allocation);
extern void sdfReleaseChipBlock(void *allocation);

/* Release allocations in both free-node lists; the stored head pointers are not cleared. */
void sdfFreeNodeLists(SdfFreeRoot *root) {
    SdfFreeNode **listCursor = root->lists;
    s32 listIndex = 0;
    s32 listCount = 2;
    do {
        SdfFreeNode *node = *listCursor;
        while (node != NULL) {
            SdfFreeNode *next = node->next;
            if (node->allocation != 0) {
                sdfReleaseResourceAllocation(node->allocation);
            } else {
                sdfReleaseChipBlock(node);
            }
            node = next;
        }
        listIndex++;
        listCursor++;
    } while (listIndex != listCount);
}

void sdfReleaseFreeRoot(SdfFreeRoot *root) {
    sdfFreeNodeLists(root);
    sdfReleaseChipBlock(root->workspace);
    root->workspace = NULL;
    sdfReleaseChipBlock(root);
}

/* Allocate a dev slot and seed its unit matrix, unit scale and colour. */
SdfDevSlot *sdfCreateBufferedTransformSlot(void) {
    SdfDevSlot *slot;

    slot = sdfAllocAndClearQuadwords(0x9C);
    slot->request = sdfDevCreateBufferedRequest(0, 4, 0x20);
    EE_MMI_UNIT_MATRIX((u8 *)&slot->unitMatrix[0]);
    VU0_STORE_VF(vf0, (u8 *)&slot->zeroVector);
    VU0_SET_ONES_XYZ(vf10);
    VU0_STORE_VF(vf10, (u8 *)&slot->unitScale);
    slot->unk1A = -1;
    slot->unk1C = 0x80808080;
    return slot;
}

typedef struct SdfObjectList {
    u8 pad00[4];
    s16 count;
    u8 pad06[6];
    SdfFreeRoot **elements;
} SdfObjectList;
extern void sdfDestroyDevRequest(void *);

/* Release each element root, then the buffered request and its owner allocation. */
void sdfDestroyObjectList(SdfObjectList **owner) {
    s32 elementIndex;
    for (elementIndex = 0; elementIndex < (*owner)->count; elementIndex++) {
        sdfReleaseFreeRoot((*owner)->elements[elementIndex]);
    }
    sdfDestroyDevRequest(*owner);
    sdfReleaseChipBlock(owner);
}

void sdfReleaseDevSlot(SdfDevSlot *slot, s32 recycle, s32 release) {
    if (slot == NULL) {
        return;
    }
    if (release != 0) {
        sdfReleaseQueuedResource(slot->resource, 1);
    }
    if (slot->device != NULL) {
        sdfDestroyDevRequest(slot->device);
    }
    if (recycle != 0) {
        sdfPendingQueuePush((SdfPendingOwner *)&sdfObjectListReleaseQueue, (u32)slot);
    } else {
        sdfDestroyDevRequest((void *)slot->request);
        sdfReleaseChipBlock(slot);
    }
}

void sdfResizeBufferedSlotRequest(u32 *handle) {
    sdfDevResizeBufferedRequest(*handle);
}

/* Node of the object tree: first child, else next sibling, else back up. */
typedef struct SdfHierarchyNode {
    u8 pad00[4];
    struct SdfHierarchyNode *nextSibling; /* 0x4 */
    struct SdfHierarchyNode *parent;      /* 0x8 */
    struct SdfHierarchyNode *firstChild;  /* 0xC */
} SdfHierarchyNode;

typedef struct SdfHierarchy {
    SdfObjectList *list; /* 0x0 */
    SdfHierarchyNode *root; /* 0x4 */
} SdfHierarchy;

/* Store the preorder hierarchy traversal in the element array; capacity is the caller's responsibility. */
void sdfCollectTreeNodes(SdfHierarchy *hierarchy) {
    SdfHierarchyNode **nodeArray = (SdfHierarchyNode **)hierarchy->list->elements;
    SdfHierarchyNode *currentNode = hierarchy->root;
    SdfHierarchyNode **output;

    if (currentNode != NULL) {
        output = nodeArray;
        do {
            *output++ = currentNode;
            if (currentNode->firstChild != NULL) {
                currentNode = currentNode->firstChild;
            } else {
                do {
                    SdfHierarchyNode *nextSibling = currentNode->nextSibling;
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

/* DevRequest +4 tracks occupied slots; +6 is capacity and +C is the element buffer. */
void sdfAppendBufferedRouteNode(s32 *list, u32 owner, u32 node) {
    s16 usedCount;
    s32 elements;
    s32 request;
    s32 newCount;

    request = *list;
    usedCount = *(s16 *)(request + 4);
    newCount = usedCount + 1;
    if ((s64)*(s16 *)(request + 6) < (s64)newCount) {
        sdfDevBufferedRequestGrow(request);
        request = *list;
    }
    elements = *(s32 *)(request + 0xc);
    *(s32 **)((s32)node + 0x10) = list;
    *(s16 *)(request + 4) = (s16)newCount;
    *(s32 *)(usedCount * 4 + elements) = (s32)node;
    sdfLinkRouteNode(node, owner);
}

typedef struct SdfRouteNode SdfRouteNode;
typedef struct SdfRouteOwner {
    u8 pad00[4];
    SdfRouteNode *first;
    u8 pad08[4];
    SdfRouteNode *last;
} SdfRouteOwner;

struct SdfRouteNode {
    SdfRouteNode *next;
    SdfRouteNode *previous;
    SdfRouteOwner *owner;
    SdfRouteNode *unkC;
    SdfRouteOwner *root;
};

void sdfUnlinkRouteNode(SdfRouteNode *node) {
    SdfRouteOwner *owner = node->owner;
    if (owner == NULL) {
        SdfRouteOwner *root = node->root;
        if (root->first == node) {
            root->first = NULL;
        }
        return;
    }
    {
        SdfRouteNode *previous = node->previous;
        SdfRouteNode *next = node->next;
        if (previous != node) {
            previous->next = next;
            next->previous = previous;
            if (owner->last == node) {
                owner->last = previous;
            }
            node->next = node;
            node->previous = node;
            return;
        }
        if (owner->last == node) {
            owner->last = NULL;
        }
    }
}

void sdfLinkRouteNode(SdfRouteNode *node, SdfRouteOwner *owner) {
    if (owner == NULL) {
        SdfRouteOwner *root = node->root;
        SdfRouteNode *first = root->first;
        if (first != node) {
            root->first = node;
            if (first != NULL) {
                first->owner = (SdfRouteOwner *)node;
                node->unkC = first;
            }
            node->owner = NULL;
        }
    } else if (node->owner != owner) {
        SdfRouteNode *last;
        sdfUnlinkRouteNode(node);
        last = owner->last;
        if (last == NULL) {
            owner->last = node;
        } else {
            SdfRouteNode *next = last->next;
            node->next = next;
            next->previous = node;
            last->next = node;
            node->previous = last;
        }
        node->owner = owner;
    }
}

INCLUDE_SDATA(const s32, "game/code_002D33C8", sdfPendingQueueSemaphore);

INCLUDE_SDATA(const s32, "game/code_002D33C8", sdfDoubleBufferAllocation);

INCLUDE_SDATA(const s32, "game/code_002D33C8", sdfPacketBufferSize);

INCLUDE_SDATA(const s32, "game/code_002D33C8", sdfPacketBuffers);

INCLUDE_SDATA(const s32, "game/code_002D33C8", sdfPacketCursor);

INCLUDE_SDATA(const s32, "game/code_002D33C8", sdfPacketBufferEnd);

INCLUDE_SDATA(const s32, "game/code_002D33C8", D_003BD32C);

INCLUDE_SDATA(const s32, "game/code_002D33C8", D_003BD32D);

INCLUDE_SDATA(const s32, "game/code_002D33C8", D_003BD32E);

INCLUDE_SDATA(const s32, "game/code_002D33C8", D_003BD330);

INCLUDE_SDATA(const s32, "game/code_002D33C8", D_003BD331);

INCLUDE_SDATA(const s32, "game/code_002D33C8", D_003BD332);

INCLUDE_SDATA(const s32, "game/code_002D33C8", sdfPacketSlotIndex);

INCLUDE_SDATA(const s32, "game/code_002D33C8", D_003BD334);

INCLUDE_SDATA(const s32, "game/code_002D33C8", D_003BD338);

INCLUDE_SDATA(const s32, "game/code_002D33C8", D_003BD33C);

INCLUDE_SDATA(const s32, "game/code_002D33C8", D_003BD340);

INCLUDE_SDATA(const s32, "game/code_002D33C8", D_003BD344);

