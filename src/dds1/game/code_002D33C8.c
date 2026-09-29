#include "common.h"
#include "sdf.h"

/* DMAC tag IDs occupy the high nibble of the header byte at offset three. */
#define SDF_DMA_TAG_NEXT_BYTE 0x20
#define SDF_DMA_TAG_REF_BYTE 0x30
#define SDF_DMA_TAG_CALL_BYTE 0x50

/* First tag word carries the DMA operation; second is its 28-bit address. */
typedef struct SdfDmaTagHeader {
    u8 pad00[3];
    u8 kind;
    u32 address;
} SdfDmaTagHeader;

extern void *D_003BDA00;
extern s8 D_003BDA04;
extern s32 D_003BD9F8[2];

extern SdfResource *D_003BD308;

extern u32 func_002CFEB8(u32);

extern u32 sdfCreateReferenceDmaNode(u32);

extern s32 D_003BD314;
extern s32 D_003BD318[2];
extern s32 D_003BD320;
extern s32 D_003BD324;

extern u64 sdfGraphHasPendingWork(void);
extern s64 func_00312C08(void);

extern s32 D_003BD30C;
extern s32 sdfCreateSemaphore(u32, u32, u32);
extern u8 D_003BD9F0;
extern u8 D_003BDA08;
extern volatile s8 D_003BD333;
extern s32 D_003BD338;
extern u32 D_00398158[];
extern SdfResEntry *D_003980E8[];

void sdfTexRelease(void);
void sdfInitializeSynchronizedRequest(void *arg0, void (*arg1)(void));
void sdfPrependPacketList(SdfListHead *list, SdfListHead *item);
void func_002D5A68(SdfPacket *arg0, u32 arg1, s32 arg2, s64 arg3, s64 arg4, s64 arg5, u32 arg6, s32 arg7, s32 arg_sp0, s32 arg_sp8, s32 arg_sp10, s32 arg_sp18, s32 arg_sp20, s32 arg_sp28);
void sdfDestroyObjectList();
void func_002D4368();
void func_002D35B8();
void func_002D4DD0();
s32 sdfAllocPacketAligned(s32 size);
void sdfAppendPacketRange(SdfListHead *list, u32 packet, u32 end);

typedef struct SdfSynchronizedRequest {
    u32 value;
    u32 state;
} SdfSynchronizedRequest;

extern void sdfReleaseQueuedResource(s32, s32);

extern void func_002D3C30(void *, void *);

typedef struct SdfDevSlot {
    u32 request;
    u8 pad04[8];
    s32 resource;
    void *device;
} SdfDevSlot;

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D33C8);

/* Search the linked resource registry by its numeric resource identifier. */
SdfResource *sdfFindResourceById(s32 id) {
    SdfResource *resource = D_003BD308;

    while (resource != NULL) {
        if (resource->id == id) {
            return resource;
        }
        resource = resource->next;
    }
    return NULL;
}

void func_002D3598(void) {
    sdfInitializeSynchronizedRequest(&D_003BD9F0, sdfTexRelease);
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

void sdfPatchPacketResourceField(SdfBigPacket *arg0, s32 arg1) {
    arg0->unk80 = (arg0->unk80 & ~0x3FFF) | (u64)(u32)(D_003980E8[arg1]->unk0C >> 6);
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D38B8);

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

    if (D_003BD30C < 0) {
        D_003BD30C = sdfCreateSemaphore(1, 0x7f, 0);
    }
    head[0] = (void *)callback;
    head[1] = NULL;
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D3C30);

typedef struct SdfQueueNode {
    struct SdfQueueNode *next;
    struct SdfQueueNode *link;
} SdfQueueNode;

SdfQueueNode *sdfDetachQueue(void) {
    SdfQueueNode *head = D_003BDA00;
    SdfQueueNode *node;

    D_003BDA00 = NULL;
    for (node = head; node != NULL; node = node->next) {
        node->link->link = NULL;
    }
    return head;
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D3D40);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D3E10);

u64 sdfGraphHasPendingWork(void) {
    u32 i;
    s32 *entry;
    if (D_003BDA04 != 0) {
        return 1;
    }
    if (D_003BDA00 != NULL) {
        return 1;
    }
    i = 0;
    entry = D_003BD9F8;
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

extern s32 D_003BD310;
extern void func_002D0918(s32);
extern s32 func_002D03F8(s32);
extern s32 sdfResourceRetainAddress(s32);

/* Reallocate two adjacent, 128-byte-aligned packet workspaces. */
void sdfResizeDoubleBuffer(s32 size) {
    s32 memory;
    if (D_003BD310 != 0) {
        func_002D0918(D_003BD310);
        D_003BD310 = 0;
    }
    size = (size + 0x7F) & ~0x7F;
    D_003BD314 = size;
    D_003BD310 = func_002D03F8(size * 2);
    memory = sdfResourceRetainAddress(D_003BD310);
    D_003BD318[0] = memory;
    D_003BD318[1] = memory + size;
}

void sdfSelectDoubleBuffer(s32 index) {
    D_003BD320 = D_003BD318[index];
    D_003BD324 = D_003BD318[index] + D_003BD314;
}

s32 sdfGetBufferRemaining(void) {
    return D_003BD324 - D_003BD320;
}

s32 sdfAllocPacketAligned(s32 size) {
    s32 packet;

    packet = D_003BD320;
    D_003BD320 = D_003BD320 + ((size + 0xfU) & 0xfffffff0);
    return packet;
}

s32 sdfGetPacketCursor(void) {
    return D_003BD320;
}

void sdfSetPacketCursorAligned(s32 cursor) {
    D_003BD320 = (cursor + 0xF) & ~0xF;
}

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

void func_002D40E8(SdfListHead *list, u32 packet) {
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
        func_002D4368(item, head);
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
            func_002D4368(last);
        }
        list->last = (u32)item;
    }
}

s32 sdfPrependIfMode1(SdfListHead *list, s32 mode, SdfListHead *packet) {
    if (mode == 1) {
        sdfPrependPacketList(list, packet);
    }
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D4240);

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

/* Patch the prior NEXT tag to chain in a reference to source's payload. */
s32 sdfLinkReferenceDmaNode(s32 previous, u32 source) {
    u32 packet;

    packet = sdfCreateReferenceDmaNode(source);
    ((SdfDmaTagHeader *)previous)->kind = SDF_DMA_TAG_NEXT_BYTE;
    ((SdfDmaTagHeader *)previous)->address = packet & 0xfffffff;
    return packet + 0x10;
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D4368);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D43F8);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D4490);

void func_002D4540(SdfListHead *list) {
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

void func_002D4578(SdfListHead *list) {
    list->unk0 = 0;
    list->first = 0;
}

void func_002D4588(s32 *arg0, s32 arg1) {
    if (arg0[1] == 0) {
        *arg0 = arg1;
    }
    else {
        **(u32 **)(arg0[1] + 8) = *(u32 *)(arg1 + 8);
    }
    arg0[1] = arg1;
}

void func_002D45B0(SdfPacket *arg0, s32 arg1) {
    s64 temp;

    temp = arg1 + 1;
    arg0->unk8 = (((temp | 0x50000000) << 32) | 0x10000000);
    arg0->unk10 = (arg1 | (((s64)0x10000000 << 32) | 0x8000));
    arg0->unk0 = temp;
    arg0->unk18 = 0xE;
}

void func_002D45F0(u64 *packet, u32 address, s32 count) {
    packet[0] = 0x10000001;
    packet[1] = 0x5000000110000000ULL;
    packet[2] = (u64)count | 0x1000000000008000ULL;
    packet[3] = 0xE;
    packet[4] = (u32)((count & 0xFFFF) | 0x30000000) | ((u64)(address & 0x0FFFFFFF) << 32);
    packet[5] = 0x5000000100000000ULL;
    packet[6] = 0x20000000;
    packet[7] = 0;
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D4678);

/* Encode symmetric positive/negative X and Y bounds into two packet words. */
void func_002D4730(u64 *packet, s32 x, s32 y) {
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

extern void func_002D4678(SdfPacket *, s32, s32, s32, s32, s32, s32, s32, s32);

void func_002D4800(SdfPacket *packet, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g) {
    func_002D45B0(packet, 5);
    func_002D4678(packet + 1, a, b, c, d, e, f, 0, g);
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D48A8);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D49E8);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D4BA0);

void sdfAppendLinkedPacketPayload(SdfListHead *list, SdfListHead *other, u32 *node) {
    sdfAppendLinkedPacketNode(other, node);
    sdfAppendPacket(list, (s32)node + 0x10);
}

void func_002D4C80(s32 source, u32 packet, s32 variant) {
    if (variant == 0) {
        func_002D45F0(packet, source + 0x180, 1);
        return;
    }
    func_002D45F0(packet, source + 400, 1);
}

void func_002D4CC8(s32 source, u32 packet, s32 variant) {
    if (variant == 0) {
        func_002D45F0(packet, source + 0x70, 1);
        return;
    }
    func_002D45F0(packet, source + 0xb0, 1);
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
    func_002D45B0(packet->packets, 2);
    packet->mode = mode;
    packet->source = source;
    packet->data = data;
    packet->region = region;
    packet->prepare = func_002D4DD0;
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D4EE8);

void func_002D4FE8(s32 arg0) {
    D_003BD333 = (arg0 < 0) ? 0 : arg0;
}

void func_002D5000(void) {
    D_003BD333 = 0;
    D_00398158[0] = 0;
    D_003BD338 = 0;
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D5018);

extern vu8 D_003BD32D;
typedef struct SdfSlotEntry {
    u32 handle;    /* 0x0 */
    u32 unk4;      /* 0x4 */
    u8 state;      /* 0x8 */
    u8 pad9[7];    /* 0x9 */
} SdfSlotEntry;

/* Wait for the shared busy flag, then sleep until the active slot is empty or ready. */
void sdfWaitSlotReady(void) {
    SdfSlotEntry *table = (SdfSlotEntry *)D_00398158;

    for (;;) {
        while (D_003BD32D != 0) {
        }
        if (table[D_003BD338].handle == 0) {
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

void func_002D55B8(SdfPacket *arg0) {
    arg0->unk0 = 0x717FB;
    arg0->unk8 = 0x47;
    arg0->unk10 = 0x44;
    arg0->unk18 = 0x42;
}

void func_002D55E0(SdfPacket *arg0) {
    arg0->unk0 = 0x717FB;
    arg0->unk8 = 0x48;
    arg0->unk10 = 0x44;
    arg0->unk18 = 0x43;
}

void func_002D5608(SdfPacket *arg0) {
    func_002D55B8(arg0 + 1);
    arg0->unk0 = 3;
    arg0->unk8 = (((u64)0x50000003 << 16 | 0x1000) << 16);
    arg0->unk10 = (((u64)0x10000000 << 32) | 0x8002);
    arg0->unk18 = 0xE;
}

void func_002D5668(SdfPacket *arg0) {
    func_002D55E0(arg0 + 1);
    arg0->unk0 = 3;
    arg0->unk8 = (((u64)0x50000003 << 16 | 0x1000) << 16);
    arg0->unk10 = (((u64)0x10000000 << 32) | 0x8002);
    arg0->unk18 = 0xE;
}

void func_002D56C8(SdfPacket *arg0) {
    arg0->unk0 = 0x717FB;
    arg0->unk8 = 0x47;
    arg0->unk10 = 0x44;
    arg0->unk18 = 0x42;
}

void func_002D56F0(SdfPacket *arg0) {
    arg0->unk0 = 0x717FB;
    arg0->unk8 = 0x48;
    arg0->unk10 = 0x44;
    arg0->unk18 = 0x43;
}

void func_002D5718(SdfPacket *arg0) {
    func_002D56C8(arg0 + 1);
    arg0->unk0 = 3;
    arg0->unk8 = (((u64)0x50000003 << 16 | 0x1000) << 16);
    arg0->unk10 = (((u64)0x10000000 << 32) | 0x8002);
    arg0->unk18 = 0xE;
}

void func_002D5778(SdfPacket *arg0) {
    func_002D56F0(arg0 + 1);
    arg0->unk0 = 3;
    arg0->unk8 = (((u64)0x50000003 << 16 | 0x1000) << 16);
    arg0->unk10 = (((u64)0x10000000 << 32) | 0x8002);
    arg0->unk18 = 0xE;
}

void func_002D57D8(SdfPacket *arg0) {
    arg0->unk0 = 0x71801;
    arg0->unk8 = 0x47;
    arg0->unk10 = 0x48;
    arg0->unk18 = 0x42;
}

void func_002D5800(SdfPacket *arg0) {
    arg0->unk0 = 0x71801;
    arg0->unk8 = 0x48;
    arg0->unk10 = 0x48;
    arg0->unk18 = 0x43;
}

void func_002D5828(SdfPacket *arg0) {
    func_002D57D8(arg0 + 1);
    arg0->unk0 = 3;
    arg0->unk8 = (((u64)0x50000003 << 16 | 0x1000) << 16);
    arg0->unk10 = (((u64)0x10000000 << 32) | 0x8002);
    arg0->unk18 = 0xE;
}

void func_002D5888(SdfPacket *arg0) {
    func_002D5800(arg0 + 1);
    arg0->unk0 = 3;
    arg0->unk8 = (((u64)0x50000003 << 16 | 0x1000) << 16);
    arg0->unk10 = (((u64)0x10000000 << 32) | 0x8002);
    arg0->unk18 = 0xE;
}

void func_002D58E8(SdfPacket *arg0) {
    arg0->unk0 = 0x71801;
    arg0->unk8 = 0x47;
    arg0->unk10 = 0x42;
    arg0->unk18 = 0x42;
}

void func_002D5910(SdfPacket *arg0) {
    arg0->unk0 = 0x71801;
    arg0->unk8 = 0x48;
    arg0->unk10 = 0x42;
    arg0->unk18 = 0x43;
}

void func_002D5938(SdfPacket *arg0) {
    func_002D58E8(arg0 + 1);
    arg0->unk0 = 3;
    arg0->unk8 = (((u64)0x50000003 << 16 | 0x1000) << 16);
    arg0->unk10 = (((u64)0x10000000 << 32) | 0x8002);
    arg0->unk18 = 0xE;
}

void func_002D5998(SdfPacket *arg0) {
    func_002D5910(arg0 + 1);
    arg0->unk0 = 3;
    arg0->unk8 = (((u64)0x50000003 << 16 | 0x1000) << 16);
    arg0->unk10 = (((u64)0x10000000 << 32) | 0x8002);
    arg0->unk18 = 0xE;
}

void func_002D59F8(u64 *arg0) {
    *arg0 = 0;
    arg0[1] = 0x3f;
}

void func_002D5A08(SdfPacket *arg0) {
    func_002D59F8((u64 *)(arg0 + 1));
    arg0->unk0 = 2;
    arg0->unk8 = (((u64)0x50000002 << 16 | 0x1000) << 16);
    arg0->unk10 = (((u64)0x10000000 << 32) | 0x8001);
    arg0->unk18 = 0xE;
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D5A68);

void func_002D5B18(SdfPacket *arg0, u32 arg1, s32 arg2, s64 arg3, s64 arg4, s64 arg5, u32 arg6, s32 arg7, s32 arg_sp0, s32 arg_sp8, s32 arg_sp10, s32 arg_sp18, s32 arg_sp20, s32 arg_sp28) {
    /* arg1 and below flow through untouched to func_002D5A68. */
    arg0->unk0 = 5;
    arg0->unk8 = (((u64)0x50000005 << 16 | 0x1000) << 16);
    arg0->unk10 = (((u64)0x10000000 << 32) | 0x8004);
    arg0->unk18 = 0xE;
    func_002D5A68(arg0 + 1, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg_sp0, arg_sp8, arg_sp10, arg_sp18, arg_sp20, arg_sp28);
}

void sdfCreateExtendedPacket(s32 list, u32 arg1, s32 arg2, s64 arg3, s64 arg4, s64 arg5,
                   u32 arg6, s32 arg7, s32 arg_sp0, s32 arg_sp8, s32 arg_sp10,
                   s32 arg_sp18, s32 arg_sp20, s32 arg_sp28, s32 (*alloc)(s32)) {
    s32 buffer;

    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    buffer = alloc(0x60);
    func_002D5B18((SdfPacket *)buffer, arg1, arg2, arg3, arg4, arg5, arg6, arg7,
                   arg_sp0, arg_sp8, arg_sp10, arg_sp18, arg_sp20, arg_sp28);
    sdfAppendPacket(list, buffer);
}

void func_002D5C90(SdfBigPacket *arg0, s32 arg1) {
    arg0->unk30 = (arg0->unk30 & ~0x3FFF) | (u64)(u32)(D_003980E8[arg1 ^ arg0->unk08]->unk0C >> 6);
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D5CD0);

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


void func_002D5EB0(s32 list, s32 source, s32 a, s32 b, s32 c, s32 d, s32 e,
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

void sdfBuildTrianglePacket(s32 dstAddr, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i) {
    u64 *dst = (u64 *)dstAddr;
    u64 iHigh = (u64)i << 32;
    u32 first = ((u32)c & 0xFFFF) | ((u32)d << 16);
    u32 second = ((u32)e & 0xFFFF) | ((u32)f << 16);
    u32 third = ((u32)g & 0xFFFF) | ((u32)h << 16);

    dst[0] = 0x6400000000008001ULL;
    dst[1] = 0xF55510;
    dst[2] = (u32)b | 0x104;
    dst[3] = (u32)a | 0x3F80000000000000ULL;
    dst[4] = first | iHigh;
    dst[5] = second | iHigh;
    dst[6] = third | iHigh;
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D6080);

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

extern void sdfBuildPacket104x4(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

void sdfBuildPacketE(SdfListHead *list, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j, s32 k, s32 (*alloc)(s32)) {
    s32 buffer;

    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    buffer = alloc(0x50);
    *(u64 *)buffer = 0x20000004ULL;
    *(u64 *)(buffer + 8) = 0x5000000410000000ULL;
    sdfBuildPacket104x4(buffer + 0x10, a, b, c, d, e, f, g, h, i, j, k);
    sdfAppendPacket(list, buffer);
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

extern void sdfBuildPacket10C(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);

void sdfBuildPacketF(SdfListHead *list, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i, s32 j, s32 k, s32 (*alloc)(s32)) {
    s32 buffer;

    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    buffer = alloc(0x60);
    *(u64 *)buffer = 0x20000005ULL;
    *(u64 *)(buffer + 8) = 0x5000000510000000ULL;
    sdfBuildPacket10C(buffer + 0x10, a, b, c, d, e, f, g, h, i, j, k);
    sdfAppendPacket(list, buffer);
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D6578);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D6690);

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

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D68D8);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D6A40);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D6B98);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D6D40);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D6E80);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D7008);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D71B8);

void sdfBuildLinePacket(s32 dstAddr, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g) {
    u64 *dst = (u64 *)dstAddr;
    u64 gHigh = (u64)(u32)g << 32;
    u32 first = ((u32)c & 0xFFFF) | ((u32)d << 16);
    u32 second = ((u32)e & 0xFFFF) | ((u32)f << 16);

    dst[0] = 0x4400000000008001ULL;
    dst[1] = 0x5510;
    dst[2] = (u32)b | 0x106;
    dst[3] = (u32)a | 0x3F80000000000000ULL;
    dst[4] = first | gHigh;
    dst[5] = second | gHigh;
}

extern void sdfBuildLinePacket(s32, s32, s32, s32, s32, s32, s32, s32);

void sdfCreatePacketA(SdfListHead *list, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 (*alloc)(s32)) {
    s32 buffer;

    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    buffer = alloc(0x40);
    *(u64 *)buffer = 0x20000003ULL;
    *(u64 *)(buffer + 8) = 0x5000000310000000ULL;
    sdfBuildLinePacket(buffer + 0x10, a, b, c, d, e, f, g);
    sdfAppendPacket(list, buffer);
}

void sdfBuildLinePacketB(s32 dstAddr, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g) {
    u64 *dst = (u64 *)dstAddr;
    u64 gHigh = (u64)(u32)g << 32;
    u32 first = ((u32)c & 0xFFFF) | ((u32)d << 16);
    u32 second = ((u32)e & 0xFFFF) | ((u32)f << 16);

    dst[0] = 0x4400000000008001ULL;
    dst[1] = 0x5510;
    dst[2] = (u32)b | 0x101;
    dst[3] = (u32)a | 0x3F80000000000000ULL;
    dst[4] = first | gHigh;
    dst[5] = second | gHigh;
}

extern void sdfBuildLinePacketB(s32, s32, s32, s32, s32, s32, s32, s32);

void func_002D7580(SdfListHead *list, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 (*alloc)(s32)) {
    s32 buffer;

    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    buffer = alloc(0x40);
    *(u64 *)buffer = 0x20000003ULL;
    *(u64 *)(buffer + 8) = 0x5000000310000000ULL;
    sdfBuildLinePacketB(buffer + 0x10, a, b, c, d, e, f, g);
    sdfAppendPacket(list, buffer);
}

void sdfBuildQuadPacket(s32 dstAddr, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g) {
    u64 *dst = (u64 *)dstAddr;
    u64 gHigh = (u64)(u32)g << 32;
    u32 x0 = (u32)c & 0xFFFF;
    u32 x1 = (u32)e & 0xFFFF;
    u32 y0 = (u32)d << 16;
    u32 y1 = (u32)f << 16;

    dst[0] = 0x8400000000008001ULL;
    dst[1] = (s32)0xF5555510;
    dst[2] = (u32)b | 0x102;
    dst[3] = (u32)a | 0x3F80000000000000ULL;
    dst[4] = (x0 | y0) | gHigh;
    dst[5] = (x1 | y0) | gHigh;
    dst[6] = (x1 | y1) | gHigh;
    dst[7] = (x0 | y1) | gHigh;
    dst[8] = (x0 | y0) | gHigh;
}

extern void sdfBuildQuadPacket(s32, s32, s32, s32, s32, s32, s32, s32);

void func_002D7720(SdfListHead *list, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 (*alloc)(s32)) {
    s32 buffer;

    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    buffer = alloc(0x60);
    *(u64 *)buffer = 0x20000005ULL;
    *(u64 *)(buffer + 8) = 0x5000000510000000ULL;
    sdfBuildQuadPacket(buffer + 0x10, a, b, c, d, e, f, g);
    sdfAppendPacket(list, buffer);
}

void func_002D7810(void) {
    sdfInitializeSynchronizedRequest(&D_003BDA08, sdfDestroyObjectList);
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D7830);

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
    u32 temp_v0;

    if (root->workspace == NULL) {
        temp_v0 = func_002CFEB8(0x100);
        root->workspace = (void *)temp_v0;
    }
}

extern void func_002D0918(s32 allocation);
extern void func_002CFF98(void *allocation);

void sdfFreeNodeLists(SdfFreeRoot *root) {
    SdfFreeNode **lists = root->lists;
    s32 i = 0;
    s32 end = 2;
    do {
        SdfFreeNode *node = *lists;
        while (node != NULL) {
            SdfFreeNode *next = node->next;
            if (node->allocation != 0) {
                func_002D0918(node->allocation);
            } else {
                func_002CFF98(node);
            }
            node = next;
        }
        i++;
        lists++;
    } while (i != end);
}

void sdfFreeNodeRoot(SdfFreeRoot *root) {
    sdfFreeNodeLists(root);
    func_002CFF98(root->workspace);
    root->workspace = NULL;
    func_002CFF98(root);
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D79C0);

typedef struct SdfObjectList {
    u8 pad00[4];
    s16 count;
    u8 pad06[6];
    SdfFreeRoot **elements;
} SdfObjectList;
extern void sdfDestroyDevRequest(void *);

void sdfDestroyObjectList(SdfObjectList **owner) {
    s32 i;
    for (i = 0; i < (*owner)->count; i++) {
        sdfFreeNodeRoot((*owner)->elements[i]);
    }
    sdfDestroyDevRequest(*owner);
    func_002CFF98(owner);
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
        func_002D3C30(&D_003BDA08, slot);
    } else {
        sdfDestroyDevRequest((void *)slot->request);
        func_002CFF98(slot);
    }
}

void func_002D7B50(u32 *arg0) {
    func_002E7730(*arg0);
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D7B68);

void func_002D7BD8(s32 *arg0, u32 arg1, u32 arg2) {
    s16 temp_v0;
    s32 temp_v1;
    s32 temp_v2;
    s32 temp_v3;

    temp_v2 = *arg0;
    temp_v0 = *(s16 *)(temp_v2 + 4);
    temp_v3 = temp_v0 + 1;
    if ((s64)*(s16 *)(temp_v2 + 6) < (s64)temp_v3) {
        func_002E76B0(temp_v2);
        temp_v2 = *arg0;
    }
    temp_v1 = *(s32 *)(temp_v2 + 0xc);
    *(s32 **)((s32)arg2 + 0x10) = arg0;
    *(s16 *)(temp_v2 + 4) = (s16)temp_v3;
    *(s32 *)(temp_v0 * 4 + temp_v1) = (s32)arg2;
    sdfLinkRouteNode(arg2, arg1);
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

INCLUDE_SDATA(const s32, "game/code_002D33C8", D_003BD30C);

INCLUDE_SDATA(const s32, "game/code_002D33C8", D_003BD310);

INCLUDE_SDATA(const s32, "game/code_002D33C8", D_003BD314);

INCLUDE_SDATA(const s32, "game/code_002D33C8", D_003BD318);

INCLUDE_SDATA(const s32, "game/code_002D33C8", D_003BD320);

INCLUDE_SDATA(const s32, "game/code_002D33C8", D_003BD324);

INCLUDE_SDATA(const s32, "game/code_002D33C8", D_003BD32C);

INCLUDE_SDATA(const s32, "game/code_002D33C8", D_003BD32D);

INCLUDE_SDATA(const s32, "game/code_002D33C8", D_003BD32E);

INCLUDE_SDATA(const s32, "game/code_002D33C8", D_003BD330);

INCLUDE_SDATA(const s32, "game/code_002D33C8", D_003BD331);

INCLUDE_SDATA(const s32, "game/code_002D33C8", D_003BD332);

INCLUDE_SDATA(const s32, "game/code_002D33C8", D_003BD333);

INCLUDE_SDATA(const s32, "game/code_002D33C8", D_003BD334);

INCLUDE_SDATA(const s32, "game/code_002D33C8", D_003BD338);

INCLUDE_SDATA(const s32, "game/code_002D33C8", D_003BD33C);

INCLUDE_SDATA(const s32, "game/code_002D33C8", D_003BD340);

INCLUDE_SDATA(const s32, "game/code_002D33C8", D_003BD344);

