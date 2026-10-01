#include "common.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"
/* Polled in a spin-wait below; its writer is outside this C unit. */
extern volatile u8 D_00438A1D;
extern void sdfSleepThreadCount(s32);
extern s32 D_00438A00;
extern s32 func_003297C8(s32);
extern s32 func_003292A8(s32);
extern s32 sdfResourceRetainAddress(s32);
#include "sdf.h"

#define SDF_DMA_TAG_NEXT 0x20
#define SDF_DMA_TAG_REF 0x30
#define SDF_DMA_TAG_CALL 0x50

/* DMA tag first word: upper byte selects the tag kind; second word is ADDR. */
typedef struct SdfDmaTagHeader {
    u8 pad00[3];
    u8 kind;
    u32 address;
} SdfDmaTagHeader;

extern s32 sdfPendingQueueSemaphore;

extern s32 sdfCreateSemaphore(u32, u32, u32);

extern u64 sdfGraphHasPendingWork(void);

extern s64 func_0036DE70(void);

extern s32 sdfPacketBufferSize;

extern u32 sdfPacketBuffers[2];

extern s32 sdfPacketCursor;

extern s32 sdfPacketBufferEnd;

extern u32 sdfCreateReferenceDmaNode(u32);

extern u32 func_00328D68(u32);

extern SdfResEntry *D_0040B298[];

extern volatile s8 D_00438A23;

extern s32 D_00438A28;

extern u32 D_0040B308[];

extern SdfResource *sdfResourceListHead;

void sdfPrependPacketList(SdfListHead *list, SdfListHead *item);

void func_0032D218();

void func_0032DC80();

extern void *sdfPendingQueueHead;

extern s8 D_00439164;

extern s32 D_00439158[2];

typedef struct SdfSynchronizedRequest {
    u32 value;
    u32 state;
} SdfSynchronizedRequest;

void func_0032C468();

s32 sdfAllocPacketAligned(s32 size);

void sdfAppendPacketRange(SdfListHead *list, u32 packet, u32 end);

extern void func_0032D528(SdfPacket *, s32, s32, s32, s32, s32, s32, s32, s32);

void func_0032E918(SdfPacket *arg0, u32 arg1, s32 arg2, s64 arg3, s64 arg4, s64 arg5, u32 arg6, s32 arg7, s32 arg_sp0, s32 arg_sp8, s32 arg_sp10, s32 arg_sp18, s32 arg_sp20, s32 arg_sp28);

extern void sdfBuildFillPacket106(s32, s32, s32, s32, s32, s32, s32, s32);

extern void sdfBuildFillPacket101(s32, s32, s32, s32, s32, s32, s32, s32);

void sdfDestroyObjectList();

extern void sdfReleaseChipBlock(void *allocation);

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

typedef struct SdfObjectList {
    u8 pad00[4];
    s16 count;
    u8 pad06[6];
    SdfFreeRoot **elements;
} SdfObjectList;

extern void sdfDestroyDevRequest(void *);

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

extern SdfSynchronizedRequest D_00439150;
extern void sdfTexRelease();

void sdfRegisterTextureReleaseRequestHandler(void) {
    sdfInitializeSynchronizedRequest(&D_00439150, (u32)sdfTexRelease);
}

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032C468);

/* Allocate a resource packet; append only its initialized 0xC0-byte payload. */
void sdfCreateResourcePacket(SdfListHead *list, s32 arg1, s32 arg2, s32 arg3, s32 arg4,
                   s32 arg5, s32 arg6, s32 arg7, s32 arg_sp0, s32 (*allocate)(s32)) {
    s32 packet;

    if (allocate == NULL) {
        allocate = sdfAllocPacketAligned;
    }
    packet = allocate(0xf0);
    func_0032C468(packet, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg_sp0);
    sdfAppendPacketRange(list, packet, packet + 0xc0);
}

void sdfPatchPacketResourceField(SdfBigPacket *packet, s32 entryIndex) {
    packet->unk80 = (packet->unk80 & ~0x3FFF) | (u64)(u32)(D_0040B298[entryIndex]->unk0C >> 6);
}

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032C768);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032C860);

void sdfCreateDescriptorPacket(SdfListHead *list, s32 source, s32 a, s32 b, s32 c, s32 d, s32 e,
                   s32 (*alloc)(s32)) {
    s32 block;
    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    block = alloc(0xB0);
    func_0032C860(block, source, a, b, c, d, e);
    sdfAppendPacketRange(list, block, block + 0x80);
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
    SdfPendingNode *node;
    SdfPendingBuffer *buffer;
    s32 remaining;

    if (entry != 0) {
        WaitSema(sdfPendingQueueSemaphore);
        node = owner->pending;
        if (node == NULL) {
            node = (SdfPendingNode *)func_00328D68(0x10);
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
            SdfPendingBuffer *fresh = (SdfPendingBuffer *)func_00328D68(0x100);
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
 * refill the last slot with the newly detached list. */
void sdfRotatePendingSlots(void) {
    u32 i;

    D_00439164 = 1;
    WaitSema(sdfPendingQueueSemaphore);
    sdfPendingQueueFlush(D_00439158[0]);
    for (i = 0; i < 1; i++) {
        D_00439158[i] = D_00439158[i + 1];
    }
    D_00439158[1] = (s32)sdfDetachQueue();
    SignalSema(sdfPendingQueueSemaphore);
    D_00439164 = 0;
}

/* Test all three pending-work sources: flag, chain, and two slots. */
u64 sdfGraphHasPendingWork(void) {
    u32 i;
    s32 *entry;
    if (D_00439164 != 0) {
        return 1;
    }
    if (sdfPendingQueueHead != NULL) {
        return 1;
    }
    i = 0;
    entry = D_00439158;
    do {
        if (*entry != 0) {
            return 1;
        }
        entry++;
        i++;
    } while (i < 2);
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

/* Reallocate two 0x80-aligned halves from one contiguous allocation. */
void sdfResizeDoubleBuffer(s32 size) {
    s32 memory;

    if (D_00438A00 != 0) {
        func_003297C8(D_00438A00);
        D_00438A00 = 0;
    }
    size = (size + 0x7F) & ~0x7F;
    sdfPacketBufferSize = size;
    D_00438A00 = func_003292A8(size * 2);
    memory = sdfResourceRetainAddress(D_00438A00);
    sdfPacketBuffers[0] = memory;
    sdfPacketBuffers[1] = memory + size;
}

/* Select one half and expose its bounds to the packet allocator. */
void sdfSelectDoubleBuffer(s32 index) {
    sdfPacketCursor = sdfPacketBuffers[index];
    sdfPacketBufferEnd = sdfPacketBuffers[index] + sdfPacketBufferSize;
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

void sdfSetPacketCursorAligned(s32 cursor) {
    sdfPacketCursor = (cursor + 0xF) & ~0xF;
}

/* Clear all links and metadata before building a new packet list. */
void sdfInitPacketList(SdfListHead *list) {
    list->unkC = 0xFFFF;
    list->unk0 = 0;
    list->first = 0;
    list->last = 0;
    list->unk10 = 0;
    list->unk14 = 0;
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
        ((SdfDmaTagHeader *)last)->kind = SDF_DMA_TAG_NEXT;
        ((SdfDmaTagHeader *)last)->address = packet & 0xfffffff;
    }
    list->last = packet;
}

/* Like sdfAppendPacket, but the tail points to the end of a packet range. */
void sdfAppendPacketRange(SdfListHead *list, u32 packet, u32 end) {
    s32 last;

    last = list->last;
    if (last == 0) {
        list->first = packet;
    }
    else {
        ((SdfDmaTagHeader *)last)->kind = SDF_DMA_TAG_NEXT;
        ((SdfDmaTagHeader *)last)->address = packet & 0xfffffff;
    }
    list->last = end;
}

/* Tag the new packet as REF and chain it into the DMA list. */
void sdfAppendReferencePacket(SdfListHead *list, u32 packet) {
    s32 last;

    ((SdfDmaTagHeader *)packet)->kind = SDF_DMA_TAG_REF;
    last = list->last;
    if (last == 0) {
        list->first = packet;
    }
    else {
        ((SdfDmaTagHeader *)last)->kind = SDF_DMA_TAG_NEXT;
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
        ((SdfDmaTagHeader *)last)->kind = SDF_DMA_TAG_NEXT;
        ((SdfDmaTagHeader *)last)->address = packet & 0xfffffff;
    }
    list->last = packet + 0x30;
}

/* Tag the new packet as CALL and chain it into the DMA list. */
void sdfAppendCallPacket(SdfListHead *list, u32 packet) {
    s32 last;

    ((SdfDmaTagHeader *)packet)->kind = SDF_DMA_TAG_CALL;
    last = list->last;
    if (last == 0) {
        list->first = packet;
    }
    else {
        ((SdfDmaTagHeader *)last)->kind = SDF_DMA_TAG_NEXT;
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
        func_0032D218(item, head);
    }
    item->unk0 = (u32)head;
    list->first = (u32)item;
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
            func_0032D218(last);
        }
        list->last = (u32)item;
    }
}

s32 sdfPrependIfMode1(SdfListHead *list, s32 mode, SdfListHead *packet) {
    if (mode == 1) {
        sdfPrependPacketList(list, packet);
    }
}

typedef struct SdfPoolInit {
    struct SdfPoolInit *next;
    u32 first;
    u32 last;
    u32 unkC;
    void (*append)(SdfListHead *, SdfListHead *);
    s32 (*prepend)(SdfListHead *, s32, SdfListHead *);
    u32 unk18;
    u32 unk1C;
} SdfPoolInit;

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032D0F0);

/* Make a REF DMA node for the payload following the source tag. */
u32 sdfCreateReferenceDmaNode(u32 source) {
    SdfDmaNode *node = (SdfDmaNode *)sdfAllocPacketAligned(0x20);
    SdfDmaSrc *src = (SdfDmaSrc *)source;
    u64 tag = src->unk0;
    u32 address = ((u32)src + 0x10) & 0x0FFFFFFF;
    s64 shifted = (s64)address << 32;

    tag |= 0x30000000;
    tag |= shifted;
    node->unk0 = tag;
    node->unk10 = 0;
    node->unk8 = src->unk8;
    return (u32)node;
}

/* Create a reference node and patch the preceding DMA NEXT tag to point at it. */
s32 sdfLinkReferenceDmaNode(s32 previous, u32 source) {
    u32 node;

    node = sdfCreateReferenceDmaNode(source);
    ((SdfDmaTagHeader *)previous)->kind = SDF_DMA_TAG_NEXT;
    ((SdfDmaTagHeader *)previous)->address = node & 0xfffffff;
    return node + 0x10;
}

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032D218);

typedef struct SdfRefNode {
    u8 pad00[0x10];
    u64 chain; /* 0x10: NEXT tag chaining to the previous head */
} SdfRefNode;

/* Give each pending reference (+0x14, then +0x10) its own DMA node, chained in front of the list. */
void sdfChainReferenceNodes(SdfListHead *list) {
    SdfRefNode *node;
    u32 address;
    u32 source;

    source = list->unk14;
    if (source != 0) {
        node = (SdfRefNode *)sdfCreateReferenceDmaNode(source);
        address = list->first & 0xFFFFFFF;
        list->first = (u32)node;
        node->chain = ((s64)address << 32) | 0x20000000;
    }
    source = list->unk10;
    if (source != 0) {
        node = (SdfRefNode *)sdfCreateReferenceDmaNode(source);
        address = list->first & 0xFFFFFFF;
        list->first = (u32)node;
        node->chain = ((s64)address << 32) | 0x20000000;
    }
}

/* Pool entry made by func_0032D0F0: per-entry packet list with append/prepend handlers. */
typedef struct SdfPoolNode {
    struct SdfPoolNode *next; /* 0x0 */
    u32 first;                /* 0x4: first packet */
    u32 last;                 /* 0x8: last packet */
    u32 unkC;
    void (*append)(struct SdfPoolNode *, struct SdfPoolNode *);      /* 0x10 */
    s32 (*prepend)(struct SdfPoolNode *, s32, struct SdfPoolNode *); /* 0x14 */
    u32 unk18;
    u32 unk1C;
} SdfPoolNode;

/* Flush every pool entry, chain the packet lists together and terminate the last. */
s32 sdfFlushPoolNodes(SdfPoolNode *node) {
    SdfPoolNode *tail = NULL;
    s32 head = 0;

    for (; node != NULL; node = node->next) {
        node->prepend(node, 0, NULL);
        if (node->first != 0) {
            if (head != 0) {
                func_0032D218(tail, node->first);
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

void sdfAppendPacketChainNode(s32 *head, s32 node) {
    if (head[1] == 0) {
        *head = node;
    }
    else {
        **(u32 **)(head[1] + 8) = *(u32 *)(node + 8);
    }
    head[1] = node;
}

void sdfInitializeDmaReferenceTag(SdfPacket *packet, s32 address) {
    s64 temp;

    temp = address + 1;
    packet->unk8 = (((temp | 0x50000000) << 32) | 0x10000000);
    packet->unk10 = (address | (((s64)0x10000000 << 32) | 0x8000));
    packet->unk0 = temp;
    packet->unk18 = 0xE;
}

void sdfBuildDmaReferenceChain(u64 *packet, u32 address, s32 count) {
    packet[0] = 0x10000001;
    packet[1] = 0x5000000110000000ULL;
    packet[2] = (u64)count | 0x1000000000008000ULL;
    packet[3] = 0xE;
    packet[4] = (u32)((count & 0xFFFF) | 0x30000000) | ((u64)(address & 0x0FFFFFFF) << 32);
    packet[5] = 0x5000000100000000ULL;
    packet[6] = 0x20000000;
    packet[7] = 0;
}

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032D528);

void sdfBuildCenteredViewBoundsPacket(u64 *packet, s32 x, s32 y, s32 unused0, s32 unused1) {
    u32 low = ((0x1000 - y) << 19) | ((0x1000 - x) << 3);
    u32 high = ((y + 0x1000) << 19) | ((x + 0x1000) << 3);

    packet[3] = 0;
    packet[7] = 5;
    packet[0] = 0x30003;
    packet[1] = 0x47;
    packet[2] = 6;
    packet[4] = (u64)0xFE00 << 46;
    packet[5] = 1;
    packet[6] = low;
    packet[8] = high;
    packet[9] = 5;
}

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

void sdfBuildSceneDrawHeader(SdfPacket *packet, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g) {
    sdfInitializeDmaReferenceTag(packet, 5);
    func_0032D528(packet + 1, a, b, c, d, e, f, 0, g);
}

typedef struct SdfResRef {
    u8 pad00[0xC];
    s32 handle;     /* 0xC */
} SdfResRef;

typedef struct SdfTexView {
    s16 x;          /* 0x0 */
    u8 pad2[2];
    s16 y;          /* 0x4 */
    u8 width;        /* 0x6 */
    u8 height;       /* 0x7 */
    SdfResRef *res[3]; /* 0x8 */
} SdfTexView;

typedef struct SdfTexScenePacket {
    SdfPacket header;    /* 0x00 */
    u64 draw[8];         /* 0x20 */
    SdfPacket tex0[2];   /* 0x60 */
    SdfPacket tex1[2];   /* 0xA0 */
    u64 limits[10];      /* 0xE0 */
    u64 regs[8];         /* 0x130 */
} SdfTexScenePacket;

extern u8 D_00438A22;

/* Build a textured scene packet: two texture setups, view bounds, GS registers and draw tail. */
void func_0032D758(SdfTexScenePacket *packet, SdfTexView *view, s32 index) {
    s32 tex;
    s32 shade;
    s32 x;
    s32 y;
    s32 w;
    s32 h;

    sdfInitializeDmaReferenceTag(&packet->header, 0x15);
    tex = view->res[index]->handle;
    shade = view->res[2]->handle;
    x = view->x;
    w = view->width;
    y = view->y;
    h = view->height;
    func_0032D528(packet->tex0, tex, x, y, w, shade, h, D_00438A22, 0);
    func_0032D528(packet->tex1, tex, x, y, w, shade, h, D_00438A22, 1);
    sdfBuildCenteredViewBoundsPacket(packet->limits, view->x, view->y, view->width, view->height);
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

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032D898);

typedef struct SdfViewBox {
    s16 x;       /* 0x0 */
    u8 pad2[2];
    s16 y;       /* 0x4 */
    u8 width;     /* 0x6 */
    u8 height;    /* 0x7 */
} SdfViewBox;

typedef struct SdfSceneNode {
    u8 pad00[4];
    void (*handler)(); /* 0x4 */
    SdfViewBox *view;  /* 0x8 */
    u8 padC[4];
    SdfPacket header;  /* 0x10 */
    u64 draw[24];      /* 0x30 */
    u64 limits[10];    /* 0xF0 */
    u64 regs[8];       /* 0x140 */
} SdfSceneNode;

extern void func_0032D898();

void sdfInitSceneNode(SdfSceneNode *node, SdfViewBox *view) {
    sdfInitializeDmaReferenceTag(&node->header, 0x15);
    node->view = view;
    node->handler = func_0032D898;
    sdfBuildCenteredViewBoundsPacket(node->limits, view->x, view->y, view->width, view->height);
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

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032DC80);

void sdfInitPacketBuilder(SdfPacketBuilder *packet, s32 source, s32 data, s32 region, s32 mode) {
    sdfInitializeDmaReferenceTag(packet->packets, 2);
    packet->mode = mode;
    packet->source = source;
    packet->data = data;
    packet->region = region;
    packet->prepare = func_0032DC80;
}

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032DD98);

void sdfSetNonnegativePacketIndex(s32 value) {
    D_00438A23 = (value < 0) ? 0 : value;
}

void sdfResetPacketSlotState(void) {
    D_00438A23 = 0;
    D_0040B308[0] = 0;
    D_00438A28 = 0;
}

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032DEC8);

void sdfWaitSlotReady(void) {
    u8 *entry;

    while (1) {
        while (D_00438A1D != 0) {
        }
        entry = (u8 *)D_0040B308 + D_00438A28 * 16;
        if (*(u32 *)entry == 0) {
            return;
        }
        if (entry[8] >= 2) {
            return;
        }
        sdfSleepThreadCount(0);
    }
}

/* Allocate and initialize an empty packet list using the selected allocator. */
s32 sdfAllocatePacketList(s32 (*allocate)(s32)) {
    s32 (*allocator)(s32) = allocate;
    s32 list;

    if (allocator == NULL) {
        allocator = sdfAllocPacketAligned;
    }
    list = allocator(0x20);
    sdfInitPacketList((SdfListHead *)list);
    return list;
}

/* Allocate a packet, initialize it with a callback, then append it. */
void sdfAppendInitializedPacket(s32 list, void (*initialize)(s32), s32 size, s32 (*allocate)(s32)) {
    s32 (*allocator)(s32) = allocate;
    s32 packet;

    if (allocator == NULL) {
        allocator = sdfAllocPacketAligned;
    }
    packet = allocator(size);
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

void func_0032E578(SdfPacket *packet) {
    packet->unk0 = 0x717FB;
    packet->unk8 = 0x47;
    packet->unk10 = 0x44;
    packet->unk18 = 0x42;
}
void func_0032E5A0(SdfPacket *packet) {
    packet->unk0 = 0x717FB;
    packet->unk8 = 0x48;
    packet->unk10 = 0x44;
    packet->unk18 = 0x43;
}

void func_0032E5C8(SdfPacket *packet) {
    func_0032E578(packet + 1);
    packet->unk0 = 3;
    packet->unk8 = (((u64)0x50000003 << 16 | 0x1000) << 16);
    packet->unk10 = (((u64)0x10000000 << 32) | 0x8002);
    packet->unk18 = 0xE;
}

void func_0032E628(SdfPacket *packet) {
    func_0032E5A0(packet + 1);
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

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032E918);

void sdfInitializeExtendedDrawPacket(SdfPacket *arg0, u32 arg1, s32 arg2, s64 arg3, s64 arg4, s64 arg5, u32 arg6, s32 arg7, s32 arg_sp0, s32 arg_sp8, s32 arg_sp10, s32 arg_sp18, s32 arg_sp20, s32 arg_sp28) {
    /* arg1 and below flow through untouched to func_002D5A68. */
    arg0->unk0 = 5;
    arg0->unk8 = (((u64)0x50000005 << 16 | 0x1000) << 16);
    arg0->unk10 = (((u64)0x10000000 << 32) | 0x8004);
    arg0->unk18 = 0xE;
    func_0032E918(arg0 + 1, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg_sp0, arg_sp8, arg_sp10, arg_sp18, arg_sp20, arg_sp28);
}

void sdfCreateExtendedPacket(s32 list, u32 arg1, s32 arg2, s64 arg3, s64 arg4, s64 arg5,
                   u32 arg6, s32 arg7, s32 arg_sp0, s32 arg_sp8, s32 arg_sp10,
                   s32 arg_sp18, s32 arg_sp20, s32 arg_sp28, s32 (*alloc)(s32)) {
    s32 buffer;

    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    buffer = alloc(0x60);
    sdfInitializeExtendedDrawPacket((SdfPacket *)buffer, arg1, arg2, arg3, arg4, arg5, arg6, arg7,
                   arg_sp0, arg_sp8, arg_sp10, arg_sp18, arg_sp20, arg_sp28);
    sdfAppendPacket(list, buffer);
}

void sdfPatchPacketResourceReference(SdfBigPacket *packet, s32 entryIndex) {
    packet->unk30 = (packet->unk30 & ~0x3FFF) | (u64)(u32)(D_0040B298[entryIndex ^ packet->unk08]->unk0C >> 6);
}

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032EB80);

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

void func_0032EF30(s32 list, s32 color, s32 primitive, s32 x0, s32 y0, s32 x1,
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
void func_0032F428(s32 address, s32 primitive, s32 x0, s32 y0, s32 color0, s32 x1, s32 y1, s32 color1, s32 x2, s32 y2, s32 color2, s32 x3, s32 y3, s32 color3, s32 depth) {
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

void func_0032F540(s32 list, s32 primitive, s32 x0, s32 y0, s32 color0, s32 x1,
                   s32 y1, s32 color1, s32 x2, s32 y2, s32 color2, s32 x3,
                   s32 y3, s32 color3, s32 depth, s32 (*alloc)(s32)) {
    SdfPacket *packet;
    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    packet = (SdfPacket *)alloc(0x70);
    packet->unk0 = 0x20000006;
    packet->unk8 = (((u64)0x50000006 << 16) | 0x1000) << 16;
    func_0032F428((s32)&packet->unk10, primitive, x0, y0, color0, x1, y1, color1, x2, y2, color2, x3, y3, color3, depth);
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

void func_0032F788(s32 list, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0,
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
void func_0032F8F0(s32 address, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0,
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

void func_0032FA48(s32 list, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0,
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
    func_0032F8F0((s32)&packet->unk10, color, primitive, x0, y0, u0, v0, x1, y1, u1, v1, x2, y2, u2, v2, x3, y3, u3, v3, depth);
    sdfAppendPacket(list, (s32)packet);
}

/* Three vertices each carry UV and color plus a shared Z value. */
void func_0032FBF0(s32 address, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0, s32 color0,
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

void func_0032FD30(s32 list, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0,
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
    func_0032FBF0((s32)&packet->unk10, primitive, x0, y0, u0, v0, color0, x1, y1, u1, v1, color1, x2, y2, u2, v2, color2, depth);
    sdfAppendPacket(list, (s32)packet);
}

/* Pack four vertices with individual UV and color and a common depth. */
void func_0032FEB8(s32 address, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0, s32 color0,
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

void func_00330068(s32 list, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0,
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
    func_0032FEB8((s32)&packet->unk10, primitive, x0, y0, u0, v0, color0, x1, y1, u1, v1, color1, x2, y2, u2, v2, color2, x3, y3, u3, v3, color3, depth);
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

extern SdfSynchronizedRequest D_00439168;
extern void sdfDestroyObjectList();

void sdfInitializeObjectListRequest(void) {
    sdfInitializeSynchronizedRequest(&D_00439168, (u32)sdfDestroyObjectList);
}

INCLUDE_ASM(const s32, "game/code_0032C278", func_003306E0);

void sdfEnsureFreeRootWorkspace(SdfFreeRoot *root) {
    u32 workspace;

    if (root->workspace == NULL) {
        workspace = func_00328D68(0x100);
        root->workspace = (void *)workspace;
    }
}

void sdfFreeNodeLists(SdfFreeRoot *root) {
    SdfFreeNode **lists = root->lists;
    s32 i = 0;
    s32 end = 2;
    do {
        SdfFreeNode *node = *lists;
        while (node != NULL) {
            SdfFreeNode *next = node->next;
            if (node->allocation != 0) {
                func_003297C8(node->allocation);
            } else {
                sdfReleaseChipBlock(node);
            }
            node = next;
        }
        i++;
        lists++;
    } while (i != end);
}


void sdfReleaseFreeRoot(SdfFreeRoot *root) {
    sdfFreeNodeLists(root);
    sdfReleaseChipBlock(root->workspace);
    root->workspace = NULL;
    sdfReleaseChipBlock(root);
}

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

/* Allocate a dev slot and seed its unit matrix, unit scale and colour. */
SdfDevSlot *func_00330870(void) {
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

void sdfDestroyObjectList(SdfObjectList **owner) {
    s32 i;
    for (i = 0; i < (*owner)->count; i++) {
        sdfReleaseFreeRoot((*owner)->elements[i]);
    }
    sdfDestroyDevRequest(*owner);
    sdfReleaseChipBlock(owner);
}

extern void sdfReleaseQueuedResource(s32, s32);
extern void sdfReleaseChipBlock();

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
        sdfPendingQueuePush((SdfPendingOwner *)&D_00439168, (u32)slot);
    } else {
        sdfDestroyDevRequest((void *)slot->request);
        sdfReleaseChipBlock(slot);
    }
}

void func_00330A00(u32 *handle) {
    sdfDevResizeBufferedRequest(*handle);
}

/* Node of the object tree: first child, else next sibling, else back up. */
typedef struct SdfTreeNode {
    u8 pad00[4];
    struct SdfTreeNode *sibling; /* 0x4 */
    struct SdfTreeNode *parent;  /* 0x8 */
    struct SdfTreeNode *child;   /* 0xC */
} SdfTreeNode;

typedef struct SdfTree {
    SdfObjectList *list; /* 0x0 */
    SdfTreeNode *root;   /* 0x4 */
} SdfTree;

/* Store every node of the tree, in depth-first order, into the list's element array. */
void sdfCollectTreeNodes(SdfTree *tree) {
    SdfTreeNode **elements = (SdfTreeNode **)tree->list->elements;
    SdfTreeNode *node = tree->root;
    SdfTreeNode **out;

    if (node != NULL) {
        out = elements;
        do {
            *out++ = node;
            if (node->child != NULL) {
                node = node->child;
            } else {
                do {
                    SdfTreeNode *next = node->sibling;
                    if (next != NULL) {
                        node = next;
                        break;
                    }
                    node = node->parent;
                } while (node != NULL);
            }
        } while (node != NULL);
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

INCLUDE_SDATA(const s32, "game/code_0032C278", sdfPendingQueueSemaphore);

INCLUDE_SDATA(const s32, "game/code_0032C278", D_00438A00);

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

INCLUDE_SDATA(const s32, "game/code_0032C278", D_00438A23);

INCLUDE_SDATA(const s32, "game/code_0032C278", D_00438A24);

INCLUDE_SDATA(const s32, "game/code_0032C278", D_00438A28);

INCLUDE_SDATA(const s32, "game/code_0032C278", D_00438A2C);

INCLUDE_SDATA(const s32, "game/code_0032C278", D_00438A30);

INCLUDE_SDATA(const s32, "game/code_0032C278", D_00438A34);

