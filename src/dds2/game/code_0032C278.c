#include "common.h"
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

extern s32 D_004389FC;

extern s32 sdfCreateSemaphore(u32, u32, u32);

extern u64 sdfGraphHasPendingWork(void);

extern s64 func_0036DE70(void);

extern s32 D_00438A04;

extern u32 D_00438A08[2];

extern s32 D_00438A10;

extern s32 D_00438A14;

extern u32 sdfCreateReferenceDmaNode(u32);

extern u32 func_00328D68(u32);

extern SdfResEntry *D_0040B298[];

extern volatile s8 D_00438A23;

extern s32 D_00438A28;

extern u32 D_0040B308[];

extern SdfResource *D_004389F8;

void sdfPrependPacketList(SdfListHead *list, SdfListHead *item);

void func_0032D218();

void func_0032DC80();

extern void *D_00439160;

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

extern void func_00328E48(void *allocation);

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

extern void sdfBuildQuadPacket102(s32, s32, s32, s32, s32, s32, s32, s32);

extern void sdfBuildPacket116(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

extern void sdfBuildPacket104x4(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

extern void sdfBuildPacket10C(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032C278);

/* Walk the resource chain until the requested numeric ID is found. */
SdfResource *sdfFindResourceById(s32 id) {
    SdfResource *resource = D_004389F8;

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

void func_0032C448(void) {
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
    if (D_004389FC < 0) {
        D_004389FC = sdfCreateSemaphore(1, 0x7f, 0);
    }
    request->value = value;
    request->state = 0;
}

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032CAE0);

typedef struct SdfLink {
    struct SdfLink *next;
    struct SdfLink *peer;
} SdfLink;

/* Detach the pending chain and clear each node's peer back-reference. */
SdfLink *sdfDetachPendingList(void) {
    SdfLink *head = D_00439160;
    SdfLink *link;

    D_00439160 = NULL;
    link = head;
    if (head != NULL) {
        do {
            link->peer->peer = NULL;
            link = link->next;
        } while (link != NULL);
    }
    return head;
}

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032CBF0);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032CCC0);

/* Test all three pending-work sources: flag, chain, and two slots. */
u64 sdfGraphHasPendingWork(void) {
    u32 i;
    s32 *entry;
    if (D_00439164 != 0) {
        return 1;
    }
    if (D_00439160 != NULL) {
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
u64 sdfGraphHasPendingWorkInterruptSafe(void) {
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
    D_00438A04 = size;
    D_00438A00 = func_003292A8(size * 2);
    memory = sdfResourceRetainAddress(D_00438A00);
    D_00438A08[0] = memory;
    D_00438A08[1] = memory + size;
}

/* Select one half and expose its bounds to the packet allocator. */
void sdfSelectDoubleBuffer(s32 index) {
    D_00438A10 = D_00438A08[index];
    D_00438A14 = D_00438A08[index] + D_00438A04;
}

s32 sdfGetBufferRemaining(void) {
    return D_00438A14 - D_00438A10;
}

/* Reserve packet space at a 16-byte boundary, returning the old cursor. */
s32 sdfAllocPacketAligned(s32 size) {
    s32 address;

    address = D_00438A10;
    D_00438A10 = D_00438A10 + ((size + 0xfU) & 0xfffffff0);
    return address;
}

s32 sdfGetPacketCursor(void) {
    return D_00438A10;
}

void sdfSetPacketCursorAligned(s32 cursor) {
    D_00438A10 = (cursor + 0xF) & ~0xF;
}

/* Clear all links and metadata before building a new packet list. */
void sdfResetPacketList(SdfListHead *list) {
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

void func_0032CF98(SdfListHead *list, u32 packet) {
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

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032D2A8);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032D340);

void func_0032D3F0(SdfListHead *arg0) {
    arg0->unk0 = 0;
    arg0->first = 0;
    arg0->last = 0;
    arg0->unkC = 0;
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

void func_0032D428(SdfListHead *arg0) {
    arg0->unk0 = 0;
    arg0->first = 0;
}

void func_0032D438(s32 *arg0, s32 arg1) {
    if (arg0[1] == 0) {
        *arg0 = arg1;
    }
    else {
        **(u32 **)(arg0[1] + 8) = *(u32 *)(arg1 + 8);
    }
    arg0[1] = arg1;
}

void func_0032D460(SdfPacket *arg0, s32 arg1) {
    s64 temp;

    temp = arg1 + 1;
    arg0->unk8 = (((temp | 0x50000000) << 32) | 0x10000000);
    arg0->unk10 = (arg1 | (((s64)0x10000000 << 32) | 0x8000));
    arg0->unk0 = temp;
    arg0->unk18 = 0xE;
}

void func_0032D4A0(u64 *packet, u32 address, s32 count) {
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

void func_0032D5E0(u64 *packet, s32 x, s32 y) {
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

void func_0032D6B0(SdfPacket *packet, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g) {
    func_0032D460(packet, 5);
    func_0032D528(packet + 1, a, b, c, d, e, f, 0, g);
}

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032D758);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032D898);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032DA50);

void sdfAppendLinkedPacketPayload(SdfListHead *list, SdfListHead *other, u32 *node) {
    sdfAppendLinkedPacketNode(other, node);
    sdfAppendPacket(list, (s32)node + 0x10);
}

void func_0032DB30(s32 arg0, u32 arg1, s32 arg2) {
    if (arg2 == 0) {
        func_0032D4A0(arg1, arg0 + 0x180, 1);
        return;
    }
    func_0032D4A0(arg1, arg0 + 400, 1);
}

void func_0032DB78(s32 arg0, u32 arg1, s32 arg2) {
    if (arg2 == 0) {
        func_0032D4A0(arg1, arg0 + 0x70, 1);
        return;
    }
    func_0032D4A0(arg1, arg0 + 0xb0, 1);
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
    func_0032D460(packet->packets, 2);
    packet->mode = mode;
    packet->source = source;
    packet->data = data;
    packet->region = region;
    packet->prepare = func_0032DC80;
}

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032DD98);

void func_0032DE98(s32 arg0) {
    D_00438A23 = (arg0 < 0) ? 0 : arg0;
}

void func_0032DEB0(void) {
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
    sdfResetPacketList((SdfListHead *)list);
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
void func_0032E468(SdfPacket *arg0) {
    arg0->unk0 = 0x717FB;
    arg0->unk8 = 0x47;
    arg0->unk10 = 0x44;
    arg0->unk18 = 0x42;
}

void func_0032E490(SdfPacket *arg0) {
    arg0->unk0 = 0x717FB;
    arg0->unk8 = 0x48;
    arg0->unk10 = 0x44;
    arg0->unk18 = 0x43;
}

void func_0032E4B8(SdfPacket *arg0) {
    func_0032E468(arg0 + 1);
    arg0->unk0 = 3;
    arg0->unk8 = (((u64)0x50000003 << 16 | 0x1000) << 16);
    arg0->unk10 = (((u64)0x10000000 << 32) | 0x8002);
    arg0->unk18 = 0xE;
}

void func_0032E518(SdfPacket *arg0) {
    func_0032E490(arg0 + 1);
    arg0->unk0 = 3;
    arg0->unk8 = (((u64)0x50000003 << 16 | 0x1000) << 16);
    arg0->unk10 = (((u64)0x10000000 << 32) | 0x8002);
    arg0->unk18 = 0xE;
}

void func_0032E578(SdfPacket *arg0) {
    arg0->unk0 = 0x717FB;
    arg0->unk8 = 0x47;
    arg0->unk10 = 0x44;
    arg0->unk18 = 0x42;
}

void func_0032E5A0(SdfPacket *arg0) {
    arg0->unk0 = 0x717FB;
    arg0->unk8 = 0x48;
    arg0->unk10 = 0x44;
    arg0->unk18 = 0x43;
}

void func_0032E5C8(SdfPacket *arg0) {
    func_0032E578(arg0 + 1);
    arg0->unk0 = 3;
    arg0->unk8 = (((u64)0x50000003 << 16 | 0x1000) << 16);
    arg0->unk10 = (((u64)0x10000000 << 32) | 0x8002);
    arg0->unk18 = 0xE;
}

void func_0032E628(SdfPacket *arg0) {
    func_0032E5A0(arg0 + 1);
    arg0->unk0 = 3;
    arg0->unk8 = (((u64)0x50000003 << 16 | 0x1000) << 16);
    arg0->unk10 = (((u64)0x10000000 << 32) | 0x8002);
    arg0->unk18 = 0xE;
}

void func_0032E688(SdfPacket *arg0) {
    arg0->unk0 = 0x71801;
    arg0->unk8 = 0x47;
    arg0->unk10 = 0x48;
    arg0->unk18 = 0x42;
}

void func_0032E6B0(SdfPacket *arg0) {
    arg0->unk0 = 0x71801;
    arg0->unk8 = 0x48;
    arg0->unk10 = 0x48;
    arg0->unk18 = 0x43;
}

void func_0032E6D8(SdfPacket *arg0) {
    func_0032E688(arg0 + 1);
    arg0->unk0 = 3;
    arg0->unk8 = (((u64)0x50000003 << 16 | 0x1000) << 16);
    arg0->unk10 = (((u64)0x10000000 << 32) | 0x8002);
    arg0->unk18 = 0xE;
}

void func_0032E738(SdfPacket *arg0) {
    func_0032E6B0(arg0 + 1);
    arg0->unk0 = 3;
    arg0->unk8 = (((u64)0x50000003 << 16 | 0x1000) << 16);
    arg0->unk10 = (((u64)0x10000000 << 32) | 0x8002);
    arg0->unk18 = 0xE;
}

void func_0032E798(SdfPacket *arg0) {
    arg0->unk0 = 0x71801;
    arg0->unk8 = 0x47;
    arg0->unk10 = 0x42;
    arg0->unk18 = 0x42;
}

void func_0032E7C0(SdfPacket *arg0) {
    arg0->unk0 = 0x71801;
    arg0->unk8 = 0x48;
    arg0->unk10 = 0x42;
    arg0->unk18 = 0x43;
}

void func_0032E7E8(SdfPacket *arg0) {
    func_0032E798(arg0 + 1);
    arg0->unk0 = 3;
    arg0->unk8 = (((u64)0x50000003 << 16 | 0x1000) << 16);
    arg0->unk10 = (((u64)0x10000000 << 32) | 0x8002);
    arg0->unk18 = 0xE;
}

void func_0032E848(SdfPacket *arg0) {
    func_0032E7C0(arg0 + 1);
    arg0->unk0 = 3;
    arg0->unk8 = (((u64)0x50000003 << 16 | 0x1000) << 16);
    arg0->unk10 = (((u64)0x10000000 << 32) | 0x8002);
    arg0->unk18 = 0xE;
}

void func_0032E8A8(u64 *arg0) {
    *arg0 = 0;
    arg0[1] = 0x3f;
}

void func_0032E8B8(SdfPacket *arg0) {
    func_0032E8A8((u64 *)(arg0 + 1));
    arg0->unk0 = 2;
    arg0->unk8 = (((u64)0x50000002 << 16 | 0x1000) << 16);
    arg0->unk10 = (((u64)0x10000000 << 32) | 0x8001);
    arg0->unk18 = 0xE;
}

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032E918);

void func_0032E9C8(SdfPacket *arg0, u32 arg1, s32 arg2, s64 arg3, s64 arg4, s64 arg5, u32 arg6, s32 arg7, s32 arg_sp0, s32 arg_sp8, s32 arg_sp10, s32 arg_sp18, s32 arg_sp20, s32 arg_sp28) {
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
    func_0032E9C8((SdfPacket *)buffer, arg1, arg2, arg3, arg4, arg5, arg6, arg7,
                   arg_sp0, arg_sp8, arg_sp10, arg_sp18, arg_sp20, arg_sp28);
    sdfAppendPacket(list, buffer);
}

void func_0032EB40(SdfBigPacket *arg0, s32 arg1) {
    arg0->unk30 = (arg0->unk30 & ~0x3FFF) | (u64)(u32)(D_0040B298[arg1 ^ arg0->unk08]->unk0C >> 6);
}

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032EB80);

void sdfBuildPacket116(s32 address, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j, s32 k) {
    u64 *packet = (u64 *)address;
    u64 hi = (u64)k << 32;

    packet[0] = 0x6400000000008001ULL;
    packet[1] = 0x535310;
    packet[2] = (u32)(b | 0x116);
    packet[3] = (u32)a | ((u64)0xFE00 << 46);
    packet[4] = (e & 0xFFFF) | (f << 16);
    packet[5] = (u32)((c & 0xFFFF) | (d << 16)) | hi;
    packet[6] = (i & 0xFFFF) | (j << 16);
    packet[7] = (u32)((g & 0xFFFF) | (h << 16)) | hi;
}

void func_0032ED60(s32 list, s32 source, s32 a, s32 b, s32 c, s32 d, s32 e,
                   s32 f, s32 g, s32 h, s32 i, s32 j, s32 (*alloc)(s32)) {
    SdfPacket *packet;
    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    packet = (SdfPacket *)alloc(0x50);
    packet->unk0 = 0x20000004;
    packet->unk8 = (((u64)0x50000004 << 16) | 0x1000) << 16;
    sdfBuildPacket116((SdfPacket *)&packet->unk10, source, a, b, c, d, e, f, g, h, i, j);
    sdfAppendPacket(list, (s32)packet);
}

void sdfBuildTriPacket104(s32 address, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i) {
    u64 *packet = (u64 *)address;
    u64 hi = (u64)i << 32;

    packet[0] = 0x6400000000008001ULL;
    packet[1] = 0xF55510;
    packet[2] = (u32)(b | 0x104);
    packet[3] = (u32)a | ((u64)0xFE00 << 46);
    packet[4] = (u32)((c & 0xFFFF) | (d << 16)) | hi;
    packet[5] = (u32)((e & 0xFFFF) | (f << 16)) | hi;
    packet[6] = (u32)((g & 0xFFFF) | (h << 16)) | hi;
}

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032EF30);

void sdfBuildPacket104x4(s32 address, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j, s32 k) {
    u64 *packet = (u64 *)address;
    u64 hi = (u64)k << 32;

    packet[0] = 0x6400000000008001ULL;
    packet[1] = 0x555510;
    packet[2] = (u32)(b | 0x104);
    packet[3] = (u32)a | ((u64)0xFE00 << 46);
    packet[4] = (u32)((c & 0xFFFF) | (d << 16)) | hi;
    packet[5] = (u32)((e & 0xFFFF) | (f << 16)) | hi;
    packet[6] = (u32)((g & 0xFFFF) | (h << 16)) | hi;
    packet[7] = (u32)((i & 0xFFFF) | (j << 16)) | hi;
}

void sdfBuildPacketE(s32 list, s32 source, s32 a, s32 b, s32 c, s32 d, s32 e,
                   s32 f, s32 g, s32 h, s32 i, s32 j, s32 (*alloc)(s32)) {
    SdfPacket *packet;
    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    packet = (SdfPacket *)alloc(0x50);
    packet->unk0 = 0x20000004;
    packet->unk8 = (((u64)0x50000004 << 16) | 0x1000) << 16;
    sdfBuildPacket104x4((SdfPacket *)&packet->unk10, source, a, b, c, d, e, f, g, h, i, j);
    sdfAppendPacket(list, (s32)packet);
}

void sdfBuildPacket10C(s32 address, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j, s32 k) {
    u64 *packet = (u64 *)address;
    u64 hi = (u64)k << 32;

    packet[0] = 0x8400000000008001ULL;
    packet[1] = 0xFFFFFFFFF5151510ULL;
    packet[2] = (u32)(a | 0x10C);
    packet[3] = (u32)d | ((u64)0xFE00 << 46);
    packet[4] = (u32)((b & 0xFFFF) | (c << 16)) | hi;
    packet[5] = (u32)g | ((u64)0xFE00 << 46);
    packet[6] = (u32)((e & 0xFFFF) | (f << 16)) | hi;
    packet[7] = (u32)j | ((u64)0xFE00 << 46);
    packet[8] = (u32)((h & 0xFFFF) | (i << 16)) | hi;
}

void sdfBuildPacketF(s32 list, s32 source, s32 a, s32 b, s32 c, s32 d, s32 e,
                   s32 f, s32 g, s32 h, s32 i, s32 j, s32 (*alloc)(s32)) {
    SdfPacket *packet;
    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    packet = (SdfPacket *)alloc(0x60);
    packet->unk0 = 0x20000005;
    packet->unk8 = (((u64)0x50000005 << 16) | 0x1000) << 16;
    sdfBuildPacket10C((SdfPacket *)&packet->unk10, source, a, b, c, d, e, f, g, h, i, j);
    sdfAppendPacket(list, (s32)packet);
}

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032F428);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032F540);

void sdfBuildPacket114(s32 address, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j, s32 k, s32 l, s32 m, s32 n, s32 o) {
    u64 *packet = (u64 *)address;
    u64 hi = (u64)o << 32;

    packet[0] = 0x8400000000008001ULL;
    packet[1] = 0x53535310;
    packet[2] = (u32)(b | 0x114);
    packet[3] = (u32)a | ((u64)0xFE00 << 46);
    packet[4] = (e & 0xFFFF) | (f << 16);
    packet[5] = (u32)((c & 0xFFFF) | (d << 16)) | hi;
    packet[6] = (i & 0xFFFF) | (j << 16);
    packet[7] = (u32)((g & 0xFFFF) | (h << 16)) | hi;
    packet[8] = (m & 0xFFFF) | (n << 16);
    packet[9] = (u32)((k & 0xFFFF) | (l << 16)) | hi;
}

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032F788);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032F8F0);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032FA48);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032FBF0);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032FD30);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032FEB8);

INCLUDE_ASM(const s32, "game/code_0032C278", func_00330068);

void sdfBuildFillPacket106(s32 address, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g) {
    u64 *packet = (u64 *)address;
    u64 hi = (u64)g << 32;

    packet[0] = 0x4400000000008001ULL;
    packet[1] = 0x5510;
    packet[2] = (u32)(b | 0x106);
    packet[3] = (u32)a | ((u64)0xFE00 << 46);
    packet[4] = (u32)((c & 0xFFFF) | (d << 16)) | hi;
    packet[5] = (u32)((e & 0xFFFF) | (f << 16)) | hi;
}

void sdfCreatePacketA(SdfListHead *list, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 (*alloc)(s32)) {
    s32 buffer;

    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    buffer = alloc(0x40);
    *(u64 *)buffer = 0x20000003ULL;
    *(u64 *)(buffer + 8) = 0x5000000310000000ULL;
    sdfBuildFillPacket106(buffer + 0x10, a, b, c, d, e, f, g);
    sdfAppendPacket(list, buffer);
}

void sdfBuildFillPacket101(s32 address, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g) {
    u64 *packet = (u64 *)address;
    u64 hi = (u64)g << 32;

    packet[0] = 0x4400000000008001ULL;
    packet[1] = 0x5510;
    packet[2] = (u32)(b | 0x101);
    packet[3] = (u32)a | ((u64)0xFE00 << 46);
    packet[4] = (u32)((c & 0xFFFF) | (d << 16)) | hi;
    packet[5] = (u32)((e & 0xFFFF) | (f << 16)) | hi;
}

void func_00330430(SdfListHead *list, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 (*alloc)(s32)) {
    s32 buffer;

    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    buffer = alloc(0x40);
    *(u64 *)buffer = 0x20000003ULL;
    *(u64 *)(buffer + 8) = 0x5000000310000000ULL;
    sdfBuildFillPacket101(buffer + 0x10, a, b, c, d, e, f, g);
    sdfAppendPacket(list, buffer);
}

void sdfBuildQuadPacket102(s32 address, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g) {
    u64 *packet = (u64 *)address;
    u64 hi = (u64)g << 32;

    packet[0] = 0x8400000000008001ULL;
    packet[1] = 0xFFFFFFFFF5555510ULL;
    packet[2] = (u32)(b | 0x102);
    packet[3] = (u32)a | ((u64)0xFE00 << 46);
    packet[4] = (u32)((c & 0xFFFF) | (d << 16)) | hi;
    packet[5] = (u32)((e & 0xFFFF) | (d << 16)) | hi;
    packet[6] = (u32)((e & 0xFFFF) | (f << 16)) | hi;
    packet[7] = (u32)((c & 0xFFFF) | (f << 16)) | hi;
    packet[8] = (u32)((c & 0xFFFF) | (d << 16)) | hi;
}

void func_003305D0(SdfListHead *list, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 (*alloc)(s32)) {
    s32 buffer;

    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    buffer = alloc(0x60);
    *(u64 *)buffer = 0x20000005ULL;
    *(u64 *)(buffer + 8) = 0x5000000510000000ULL;
    sdfBuildQuadPacket102(buffer + 0x10, a, b, c, d, e, f, g);
    sdfAppendPacket(list, buffer);
}

extern SdfSynchronizedRequest D_00439168;
extern void sdfDestroyObjectList();

void func_003306C0(void) {
    sdfInitializeSynchronizedRequest(&D_00439168, (u32)sdfDestroyObjectList);
}

INCLUDE_ASM(const s32, "game/code_0032C278", func_003306E0);

void sdfEnsureFreeRootWorkspace(SdfFreeRoot *root) {
    u32 temp_v0;

    if (root->workspace == NULL) {
        temp_v0 = func_00328D68(0x100);
        root->workspace = (void *)temp_v0;
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
                func_00328E48(node);
            }
            node = next;
        }
        i++;
        lists++;
    } while (i != end);
}


void sdfReleaseFreeRoot(SdfFreeRoot *root) {
    sdfFreeNodeLists(root);
    func_00328E48(root->workspace);
    root->workspace = NULL;
    func_00328E48(root);
}

INCLUDE_ASM(const s32, "game/code_0032C278", func_00330870);

void sdfDestroyObjectList(SdfObjectList **owner) {
    s32 i;
    for (i = 0; i < (*owner)->count; i++) {
        sdfReleaseFreeRoot((*owner)->elements[i]);
    }
    sdfDestroyDevRequest(*owner);
    func_00328E48(owner);
}

extern void sdfReleaseQueuedResource(s32, s32);
extern void func_00328E48();
extern void func_0032CAE0(void *, void *);

typedef struct SdfDevSlot {
    u32 request;
    u8 pad04[8];
    s32 resource;
    void *device;
} SdfDevSlot;

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
        func_0032CAE0(&D_00439168, slot);
    } else {
        sdfDestroyDevRequest((void *)slot->request);
        func_00328E48(slot);
    }
}

void func_00330A00(u32 *arg0) {
    func_003405D8(*arg0);
}

INCLUDE_ASM(const s32, "game/code_0032C278", func_00330A18);

void func_00330A88(s32 *arg0, u32 arg1, u32 arg2) {
    s16 temp_v0;
    s32 temp_v1;
    s32 temp_v2;
    s32 temp_v3;

    temp_v2 = *arg0;
    temp_v0 = *(s16 *)(temp_v2 + 4);
    temp_v3 = temp_v0 + 1;
    if ((s64)*(s16 *)(temp_v2 + 6) < (s64)temp_v3) {
        func_00340558(temp_v2);
        temp_v2 = *arg0;
    }
    temp_v1 = *(s32 *)(temp_v2 + 0xc);
    *(s32 **)((s32)arg2 + 0x10) = arg0;
    *(s16 *)(temp_v2 + 4) = (s16)temp_v3;
    *(s32 *)(temp_v0 * 4 + temp_v1) = (s32)arg2;
    sdfLinkRouteNode(arg2, arg1);
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

INCLUDE_SDATA(const s32, "game/code_0032C278", D_004389FC);

INCLUDE_SDATA(const s32, "game/code_0032C278", D_00438A00);

INCLUDE_SDATA(const s32, "game/code_0032C278", D_00438A04);

INCLUDE_SDATA(const s32, "game/code_0032C278", D_00438A08);

INCLUDE_SDATA(const s32, "game/code_0032C278", D_00438A10);

INCLUDE_SDATA(const s32, "game/code_0032C278", D_00438A14);

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

