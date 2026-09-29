#include "common.h"
#include "sdf.h"

extern void *D_003BDA00;
extern s8 D_003BDA04;
extern s32 D_003BD9F8[2];

extern SdfResource *D_003BD308;

extern u32 func_002CFEB8(u32);

extern u32 func_002D42B8(u32);

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
void func_002D3BE0(void *arg0, void (*arg1)(void));
void sdfPrependPacketList(SdfListHead *list, SdfListHead *item);
void func_002D5A68(SdfPacket *arg0, u32 arg1, s32 arg2, s64 arg3, s64 arg4, s64 arg5, u32 arg6, s32 arg7, s32 arg_sp0, s32 arg_sp8, s32 arg_sp10, s32 arg_sp18, s32 arg_sp20, s32 arg_sp28);
void func_002D7A50();
void func_002D4368();
void func_002D35B8();
void func_002D4DD0();
s32 sdfAllocPacketAligned(s32 size);
void sdfAppendPacketRange(SdfListHead *list, u32 packet, u32 end);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D33C8);

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
    func_002D3BE0(&D_003BD9F0, sdfTexRelease);
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

void func_002D3880(SdfBigPacket *arg0, s32 arg1) {
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

void func_002D3BE0(void *arg0, void (*arg1)(void)) {
    void **head = arg0;

    if (D_003BD30C < 0) {
        D_003BD30C = sdfCreateSemaphore(1, 0x7f, 0);
    }
    head[0] = (void *)arg1;
    head[1] = NULL;
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D3C30);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D3D00);

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

u64 func_002D3EE8(void) {
    s64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_00312C08();
    temp_v1 = sdfGraphHasPendingWork();
    if (temp_v0 != 0) {
        EIntr();
    }
    return temp_v1;
}

extern s32 D_003BD310;
extern void func_002D0918(s32);
extern s32 func_002D03F8(s32);
extern s32 sdfResourceRetainAddress(s32);

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

void sdfSelectDoubleBuffer(s32 arg0) {
    D_003BD320 = D_003BD318[arg0];
    D_003BD324 = D_003BD318[arg0] + D_003BD314;
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

s32 func_002D3FF0(void) {
    return D_003BD320;
}

void func_002D3FF8(s32 arg0) {
    D_003BD320 = (arg0 + 0xF) & ~0xF;
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

void sdfAppendPacket(SdfListHead *list, u32 packet) {
    s32 last;

    last = list->last;
    if (last == 0) {
        list->first = packet;
    }
    else {
        *(u8 *)(last + 3) = 0x20;
        *(u32 *)(last + 4) = packet & 0xfffffff;
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
        *(u8 *)(last + 3) = 0x20;
        *(u32 *)(last + 4) = packet & 0xfffffff;
    }
    list->last = end;
}

void func_002D40A8(SdfListHead *list, u32 packet) {
    s32 last;

    *(u8 *)(packet + 3) = 0x30;
    last = list->last;
    if (last == 0) {
        list->first = packet;
    }
    else {
        *(u8 *)(last + 3) = 0x20;
        *(u32 *)(last + 4) = packet & 0xfffffff;
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
        *(u8 *)(last + 3) = 0x20;
        *(u32 *)(last + 4) = packet & 0xfffffff;
    }
    list->last = packet + 0x30;
}

void func_002D4120(SdfListHead *list, u32 packet) {
    s32 last;

    *(u8 *)(packet + 3) = 0x50;
    last = list->last;
    if (last == 0) {
        list->first = packet;
    }
    else {
        *(u8 *)(last + 3) = 0x20;
        *(u32 *)(last + 4) = packet & 0xfffffff;
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

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D4218);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D4240);

u32 func_002D42B8(u32 arg0) {
    SdfDmaNode *temp = (SdfDmaNode *)sdfAllocPacketAligned(0x20);
    SdfDmaSrc *src = (SdfDmaSrc *)arg0;
    u64 id = src->unk0;
    u32 addr = ((u32)src + 0x10) & 0x0FFFFFFF;
    s64 shifted = (s64)addr << 32;

    id |= 0x30000000;
    id |= shifted;
    temp->unk0 = id;
    temp->unk10 = 0;
    temp->unk8 = src->unk8;
    return (u32)temp;
}

s32 func_002D4320(s32 arg0, u32 arg1) {
    u32 temp_v0;

    temp_v0 = func_002D42B8(arg1);
    *(u8 *)(arg0 + 3) = 0x20;
    *(u32 *)(arg0 + 4) = temp_v0 & 0xfffffff;
    return temp_v0 + 0x10;
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

void func_002D4558(s32 arg0, u32 *arg1) {
    if (*(u32 **)(arg0 + 8) == (u32 *)0x0) {
        *(u32 **)(arg0 + 4) = arg1;
    }
    else {
        **(u32 **)(arg0 + 8) = arg1;
    }
    *(u32 **)(arg0 + 8) = arg1;
    *arg1 = 0;
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

void func_002D47B8(u64 *packet) {
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

void func_002D4C40(u32 arg0, u32 arg1, u32 arg2) {
    func_002D4558(arg1, arg2);
    sdfAppendPacket(arg0, (s32)arg2 + 0x10);
}

void func_002D4C80(s32 arg0, u32 arg1, s32 arg2) {
    if (arg2 == 0) {
        func_002D45F0(arg1, arg0 + 0x180, 1);
        return;
    }
    func_002D45F0(arg1, arg0 + 400, 1);
}

void func_002D4CC8(s32 arg0, u32 arg1, s32 arg2) {
    if (arg2 == 0) {
        func_002D45F0(arg1, arg0 + 0x70, 1);
        return;
    }
    func_002D45F0(arg1, arg0 + 0xb0, 1);
}

void sdfAppendDmaPrimary(s32 list, u32 source, SdfDmaNode *node) {
    node->unk8 = (((u64)0x50000004 << 16) | 0x1000) << 16;
    node->unk0 = ((u64)((source + 0x1a0) & 0xfffffff) << 32) | 0x30000004;
    node->unk10 = 0;
    func_002D40A8(list, (u32)node);
}

void sdfAppendDmaSecondary(s32 list, u32 source, SdfDmaNode *node) {
    node->unk8 = (((u64)0x50000004 << 16) | 0x1000) << 16;
    node->unk0 = ((u64)((source + 0x1e0) & 0xfffffff) << 32) | 0x30000004;
    node->unk10 = 0;
    func_002D40A8(list, (u32)node);
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

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D5498);

s32 func_002D5510(s32 (*arg0)(s32)) {
    s32 (*alloc)(s32) = arg0;
    s32 mem;

    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    mem = alloc(0x20);
    sdfInitPacketList((SdfListHead *)mem);
    return mem;
}

void func_002D5558(s32 arg0, void (*arg1)(s32), s32 arg2, s32 (*arg3)(s32)) {
    s32 (*alloc)(s32) = arg3;
    s32 mem;

    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    mem = alloc(arg2);
    arg1(mem);
    sdfAppendPacket(arg0, mem);
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

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D5DF8);

void func_002D5EB0(s32 list, s64 source, s32 a, s32 b, s32 c, s32 d, s32 e,
                   s32 f, s32 g, s32 h, s32 i, s32 j, s32 (*alloc)(s32)) {
    SdfPacket *packet;
    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    packet = (SdfPacket *)alloc(0x50);
    packet->unk0 = 0x20000004;
    packet->unk8 = (((u64)0x50000004 << 16) | 0x1000) << 16;
    func_002D5DF8((SdfPacket *)&packet->unk10, source, a, b, c, d, e, f, g, h, i, j);
    sdfAppendPacket(list, (s32)packet);
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D5FD8);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D6080);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D6188);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D6258);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D6380);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D6450);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D6578);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D6690);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D67E8);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D68D8);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D6A40);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D6B98);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D6D40);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D6E80);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D7008);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D71B8);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D7390);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D7410);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D7500);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D7580);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D7670);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D7720);

void func_002D7810(void) {
    func_002D3BE0(&D_003BDA08, func_002D7A50);
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D7830);

void func_002D78B8(s32 arg0) {
    u32 temp_v0;

    if (*(s32 *)(arg0 + 0x30) == 0) {
        temp_v0 = func_002CFEB8(0x100);
        *(u32 *)(arg0 + 0x30) = temp_v0;
    }
}

typedef struct SdfFreeNode {
    struct SdfFreeNode *next;
    u8 pad04[8];
    s32 allocation;
} SdfFreeNode;

extern void func_002D0918(s32 allocation);
extern void func_002CFF98(void *allocation);

void sdfFreeNodeLists(s32 root) {
    SdfFreeNode **lists = (SdfFreeNode **)(root + 0x28);
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

void func_002D7988(u32 arg0) {
    sdfFreeNodeLists(arg0);
    func_002CFF98(*(u32 *)((s32)arg0 + 0x30));
    *(u32 *)((s32)arg0 + 0x30) = 0;
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D79C0);

typedef struct SdfObjectList {
    u8 pad00[4];
    s16 count;
    u8 pad06[6];
    s32 *elements;
} SdfObjectList;
extern void sdfDestroyDevRequest(void *);

void func_002D7A50(SdfObjectList **owner) {
    s32 i;
    for (i = 0; i < (*owner)->count; i++) {
        func_002D7988((*owner)->elements[i]);
    }
    sdfDestroyDevRequest(*owner);
    func_002CFF98(owner);
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D7AC8);

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
    u8 pad00[0xC];
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
        if (*(SdfRouteNode **)((u8 *)root + 4) == node) {
            *(SdfRouteNode **)((u8 *)root + 4) = NULL;
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
        SdfRouteNode *first = *(SdfRouteNode **)((u8 *)root + 4);
        if (first != node) {
            *(SdfRouteNode **)((u8 *)root + 4) = node;
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

