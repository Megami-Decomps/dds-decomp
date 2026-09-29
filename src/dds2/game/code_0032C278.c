#include "common.h"
#include "sdf.h"

extern s32 D_004389FC;

extern s32 sdfCreateSemaphore(u32, u32, u32);

extern u64 sdfGraphHasPendingWork(void);

extern s64 func_0036DE70(void);

extern s32 D_00438A04;

extern u32 D_00438A08;

extern s32 D_00438A10;

extern s32 D_00438A14;

extern u32 func_0032D168(u32);

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

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032C278);

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

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032C448);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032C468);

INCLUDE_ASM(const s32, "game/code_0032C278", sdfCreateResourcePacket);

void func_0032C730(SdfBigPacket *packet, s32 entryIndex) {
    packet->unk80 = (packet->unk80 & ~0x3FFF) | (u64)(u32)(D_0040B298[entryIndex]->unk0C >> 6);
}

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032C768);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032C860);

INCLUDE_ASM(const s32, "game/code_0032C278", sdfCreateDescriptorPacket);

void func_0032CA90(SdfSynchronizedRequest *request, u32 value) {
    if (D_004389FC < 0) {
        D_004389FC = sdfCreateSemaphore(1, 0x7f, 0);
    }
    request->value = value;
    request->state = 0;
}

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032CAE0);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032CBB0);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032CBF0);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032CCC0);

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

u64 func_0032CD98(void) {
    s64 interruptState;
    u64 pendingWork;

    interruptState = func_0036DE70();
    pendingWork = sdfGraphHasPendingWork();
    if (interruptState != 0) {
        EIntr();
    }
    return pendingWork;
}

INCLUDE_ASM(const s32, "game/code_0032C278", sdfResizeDoubleBuffer);

void sdfSelectDoubleBuffer(s32 arg0) {
    D_00438A10 = (&D_00438A08)[arg0];
    D_00438A14 = (&D_00438A08)[arg0] + D_00438A04;
}

s32 sdfGetBufferRemaining(void) {
    return D_00438A14 - D_00438A10;
}

s32 sdfAllocPacketAligned(s32 size) {
    s32 address;

    address = D_00438A10;
    D_00438A10 = D_00438A10 + ((size + 0xfU) & 0xfffffff0);
    return address;
}

s32 func_0032CEA0(void) {
    return D_00438A10;
}

void func_0032CEA8(s32 arg0) {
    D_00438A10 = (arg0 + 0xF) & ~0xF;
}

void sdfResetPacketList(SdfListHead *arg0) {
    arg0->unkC = 0xFFFF;
    arg0->unk0 = 0;
    arg0->first = 0;
    arg0->last = 0;
    arg0->unk10 = 0;
    arg0->unk14 = 0;
    arg0->unk18 = 0;
    arg0->unk1C = 0;
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

void func_0032CF58(SdfListHead *list, u32 packet) {
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

void func_0032CF98(SdfListHead *list, u32 packet) {
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

void func_0032CFD0(SdfListHead *list, u32 packet) {
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

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032D0C8);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032D0F0);

u32 func_0032D168(u32 arg0) {
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

s32 func_0032D1D0(s32 arg0, u32 arg1) {
    u32 temp_v0;

    temp_v0 = func_0032D168(arg1);
    *(u8 *)(arg0 + 3) = 0x20;
    *(u32 *)(arg0 + 4) = temp_v0 & 0xfffffff;
    return temp_v0 + 0x10;
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

void func_0032D408(s32 arg0, u32 *arg1) {
    if (*(u32 **)(arg0 + 8) == (u32 *)0x0) {
        *(u32 **)(arg0 + 4) = arg1;
    }
    else {
        **(u32 **)(arg0 + 8) = arg1;
    }
    *(u32 **)(arg0 + 8) = arg1;
    *arg1 = 0;
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

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032D4A0);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032D528);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032D5E0);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032D668);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032D6B0);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032D758);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032D898);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032DA50);

void func_0032DAF0(u32 arg0, u32 arg1, u32 arg2) {
    func_0032D408(arg1, arg2);
    sdfAppendPacket(arg0, (s32)arg2 + 0x10);
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

INCLUDE_ASM(const s32, "game/code_0032C278", sdfAppendDmaPrimary);

INCLUDE_ASM(const s32, "game/code_0032C278", sdfAppendDmaSecondary);

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

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032E348);

s32 func_0032E3C0(s32 (*arg0)(s32)) {
    s32 (*alloc)(s32) = arg0;
    s32 mem;

    if (alloc == NULL) {
        alloc = sdfAllocPacketAligned;
    }
    mem = alloc(0x20);
    sdfResetPacketList((SdfListHead *)mem);
    return mem;
}

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032E408);

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

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032E9C8);

INCLUDE_ASM(const s32, "game/code_0032C278", sdfCreateExtendedPacket);

void func_0032EB40(SdfBigPacket *arg0, s32 arg1) {
    arg0->unk30 = (arg0->unk30 & ~0x3FFF) | (u64)(u32)(D_0040B298[arg1 ^ arg0->unk08]->unk0C >> 6);
}

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032EB80);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032ECA8);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032ED60);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032EE88);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032EF30);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032F038);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032F108);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032F230);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032F300);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032F428);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032F540);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032F698);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032F788);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032F8F0);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032FA48);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032FBF0);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032FD30);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032FEB8);

INCLUDE_ASM(const s32, "game/code_0032C278", func_00330068);

INCLUDE_ASM(const s32, "game/code_0032C278", func_00330240);

INCLUDE_ASM(const s32, "game/code_0032C278", func_003302C0);

INCLUDE_ASM(const s32, "game/code_0032C278", func_003303B0);

INCLUDE_ASM(const s32, "game/code_0032C278", func_00330430);

INCLUDE_ASM(const s32, "game/code_0032C278", func_00330520);

INCLUDE_ASM(const s32, "game/code_0032C278", func_003305D0);

INCLUDE_ASM(const s32, "game/code_0032C278", func_003306C0);

INCLUDE_ASM(const s32, "game/code_0032C278", func_003306E0);

void func_00330768(s32 arg0) {
    u32 temp_v0;

    if (*(s32 *)(arg0 + 0x30) == 0) {
        temp_v0 = func_00328D68(0x100);
        *(u32 *)(arg0 + 0x30) = temp_v0;
    }
}

INCLUDE_ASM(const s32, "game/code_0032C278", sdfFreeNodeLists);

void func_00330838(u32 arg0) {
    sdfFreeNodeLists();
    func_00328E48(*(u32 *)((s32)arg0 + 0x30));
    *(u32 *)((s32)arg0 + 0x30) = 0;
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_0032C278", func_00330870);

INCLUDE_ASM(const s32, "game/code_0032C278", func_00330900);

INCLUDE_ASM(const s32, "game/code_0032C278", func_00330978);

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

INCLUDE_ASM(const s32, "game/code_0032C278", sdfUnlinkRouteNode);

INCLUDE_ASM(const s32, "game/code_0032C278", sdfLinkRouteNode);

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

