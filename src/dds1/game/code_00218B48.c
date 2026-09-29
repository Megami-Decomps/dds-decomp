#include "common.h"
#include "pcp_vu0.h"

typedef struct MdlViewState {
    s32 unk00;
    s32 viewerTask;
    s8 unk08;
    u8 pad09;
    s8 unk0A;
    u8 pad0B[11];
    s16 resourceCount;
    s16 unk18;
    s16 unk1A;
    s32 unk1C;
    u8 pad20[2];
    s16 unk22;
    s16 unk24;
    s16 unk26;
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    s16 unk2E;
    s16 unk30;
    u8 pad32[2];
    s16 unk34;
    s16 unk36;
    u8 pad38[2];
    s16 unk3A;
    u8 pad3C[4];
    s16 unk40;
    s16 unk42;
    s16 unk44;
    s16 unk46;
    u8 pad48[0x44];
    s32 slotBeforeResources[1]; /* One element before resources[] for rightward rotation. */
    s32 resources[1];
} MdlViewState;

typedef struct MdlCtrlState {
    u8 pad00[4];
    u8 unk04;
    u8 pad05[3];
    s32 unk08;
} MdlCtrlState;

extern MdlViewState D_003D7A50;
extern MdlCtrlState D_003D7B50;
extern s32 D_003BAA00;
extern s32 D_003D7B10[];
extern s8 D_003D7A60[];
extern s8 D_00367A40[];

s32 func_0011D3E8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
void func_0011E080(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
void func_0014FB70(s32 arg0, float arg1);
s32 func_0014FD20(s32 arg0);
void func_00152000(s32 arg0, float arg1, float arg2);
s32 billCreateIndexed(s32 arg0, s32 arg1);
s32 func_0021A608(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
s32 func_0021A660(s16 *arg0, s32 arg1);
void func_0021B9F8(void);
void func_0021BDD0(void);
void sdfAppendPacket(s32 arg0, s32 arg1);
void func_002EC780(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern s32 kwlnTaskGetTaskByName(void *name);
extern void kwlnTaskDestroyWithHierarchy(s32 task, s32 flag);
extern s32 kwlnTaskCreate(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
void func_00102A18(void);
void func_00101A80(s32 arg0, s32 arg1);
void func_0021E3C0(MdlViewState *arg0);
void func_002EDBD8(void *buffer, s32 arg1);
void func_002EDC30(void *buffer);
void func_002EDE48(void *buffer, s32 arg1, s32 arg2);
void func_00218768(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_002EDC50(void *buffer);
s32 *func_002192D0(s32 arg0, s32 arg1);
s32 mdlCountRecords(s32 arg0);
void mdlLoadViewerPackage(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    u8 buffer[0x40];

    func_002EDBD8(buffer, 0);
    if (arg2 & 2) {
        func_002EDC30(buffer);
    }
    func_002EDE48(buffer, arg3, arg4);
    func_00218768(*(s32 *)(buffer + 0x30), arg0, arg1, arg2);
    func_002EDC50(buffer);
}

void func_00218BE8(s32 arg0) {
    func_002CFF98((void *)arg0);
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_00218C00);

INCLUDE_ASM(const s32, "game/code_00218B48", func_00218CA8);

INCLUDE_ASM(const s32, "game/code_00218B48", func_00218E20);

typedef struct {
    s32 kind;        /* 0x00: 0xFFFF terminates record traversal */
    s32 nextOffset;  /* 0x04: relative byte offset to next record */
    u32 value08;     /* 0x08 */
    u16 value0C;     /* 0x0C */
    u16 field0E;     /* 0x0E */
    u16 field10;     /* 0x10 */
} MdlRecord;

u32 func_002192C0(MdlRecord *record) {
    return record->value08;
}

u16 func_002192C8(MdlRecord *record) {
    return record->value0C;
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_002192D0);

s32 * func_00219350(s32 address) {
    s32 *record = (s32 *)(address + 8);

    if (*record == 0xffff) {
        record = NULL;
    }
    return record;
}

s32 * func_00219368(MdlRecord *current) {
    s32 *record = (s32 *)((s32)current + current->nextOffset);

    if (*record == 0xffff) {
        record = NULL;
    }
    return record;
}

s32 mdlCountRecords(s32 arg0) {
    s32 count;
    s32 *node;

    if (arg0 == 0) {
        return 0;
    }
    node = func_00219350(arg0);
    count = 0;
    while (node != NULL) {
        count++;
        node = func_00219368((s32)node);
    }
    return count;
}

u8 func_002193D8(s32 *arg0, s32 arg1) {
    return *arg0 == arg1;
}

u16 func_002193E8(MdlRecord *record) {
    return record->field0E;
}

u16 func_002193F0(MdlRecord *record) {
    return record->field10;
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_002193F8);

typedef struct EffMarkParams {
    u8 unk0[8];
    s32 mark0;       /* 0x08 */
    s32 mark1;       /* 0x0C */
    u16 start;       /* 0x10 */
    u16 end;         /* 0x12 */
    u16 interval;    /* 0x14 */
    u8 face;         /* 0x16 */
    u8 blend;        /* 0x17 */
    u32 colors[4];   /* 0x18 */
} EffMarkParams;

extern char *D_003679B8[];
extern u8 D_003679B0[];
extern u16 D_00367980[];
extern char D_003BBBB0[];
extern char D_003BBBB8[];

void func_00219590(s32 list, s32 x, s32 y, s32 z, EffMarkParams *params, s32 selected) {
    s32 boxY = y + 0x300;
    s32 labelX = x + 0xC0;
    u32 color;
    s32 row;
    s32 col;

    sdfAppendPacket(list, func_002E4960(labelX, y, z, 0, "MARK0:%d", params->mark0));
    sdfAppendPacket(list, func_002E4960(labelX, y + 0x60, z, 0, "MARK1:%d", params->mark1));
    sdfAppendPacket(list, func_002E4960(labelX, y + 0xC0, z, 0, "START:%d", params->start));
    sdfAppendPacket(list, func_002E4960(labelX, y + 0x120, z, 0, "END  :%d", params->end));
    sdfAppendPacket(list, func_002E4960(labelX, y + 0x180, z, 0, "ITRVL:%d", params->interval));
    sdfAppendPacket(list, func_002E4960(labelX, y + 0x1E0, z, 0, "FACE :%d", params->face));
    sdfAppendPacket(list, func_002E4960(labelX, y + 0x240, z, 0, "BLEND:%d", params->blend));
    for (row = 0; row != 4; row++) {
        color = params->colors[row];
        sdfAppendPacket(list, func_0011D3E8(x + 0x480, boxY, z, 0x300, 0x180, (color & 0xFFFFFF) | 0x80000000, 0x60404040));
        for (col = 0; col != 4; col++) {
            sdfAppendPacket(list, func_002E4960(labelX, boxY, z, D_003679B0[col], D_003679B8[col]));
            sdfAppendPacket(list, func_002E4960(x + 0x240, boxY, z, 0, D_003BBBB0, color & 0xFF));
            color >>= 8;
            boxY += 0x60;
        }
        boxY += 0x60;
    }
    if (selected >= 0) {
        sdfAppendPacket(list, func_002E4960(x, y + D_00367980[selected], z, 0, D_003BBBB8));
    }
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_002198D8);

typedef struct MdlPartEntry {
    s32 kind;     /* 0x00: billboard or effect */
    s32 state;    /* 0x04 */
    s32 object;   /* 0x08 */
    u8 pad0C[4];
} MdlPartEntry;

typedef struct MdlPartList {
    u8 pad00[4];
    s16 count;    /* 0x04 */
    u8 pad06[6];
    MdlPartEntry *entries; /* 0x0C */
} MdlPartList;

void func_00219AF8(MdlPartList *list, s32 index) {
    MdlPartEntry *entry = &list->entries[list->count];

    entry->state = 0;
    entry->kind = 0;
    entry->object = billCreateIndexed(1, index);
    list->count += 1;
}

void func_00219B50(MdlPartList *list, s32 index) {
    MdlPartEntry *entry = &list->entries[list->count];

    entry->kind = 1;
    entry->state = 0;
    entry->object = func_0014FD20(index);
    list->count += 1;
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_00219BB0);

typedef struct MdlObj {
    s32 unk0;             /* 0x00 */
    s32 handle;           /* 0x04 */
    u8 pad08;
    u8 initialized;       /* 0x09 */
    u8 pad0A[6];
    s32 unk10;            /* 0x10 */
    u8 pad14[0xC];
    u8 data[1];           /* 0x20 */
} MdlObj;

void mdlObjDestroy(MdlObj *obj) {
    if (obj->initialized != 0) {
        func_002EBB60(obj->data);
    }
    func_002D0918(obj->handle);
    func_002CFF98(obj);
}

void mdlObjInit(MdlObj *obj, s32 arg1, s32 arg2) {
    if (obj->initialized == 0) {
        obj->initialized = 1;
        func_002EC780((s32)obj->data, arg2, obj->unk0, obj->unk10, arg1);
    }
}

void func_00219CC8(u32 arg0) {
    sdfDevCreateBufferedRequest(arg0, 0x10, 4);
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_00219CE8);

typedef struct MdlResourceItem {
    struct MdlResourceItem *next; /* 0x00 */
    u16 type;                     /* 0x04: billboard / effect kind */
    s16 subtype;                  /* 0x06 */
    s32 resource;                 /* 0x08 */
    u8 pad0C[0x14];
} MdlResourceItem;

typedef struct {
    u8 pad00[0x14];
    MdlResourceItem *first; /* 0x14 */
} MdlResourceOwner;

MdlResourceItem *mdlInsertResourceItem(MdlResourceOwner *object, s32 type, s32 subtype) {
    extern void *func_002CFF68(s32 size);
    MdlResourceItem *item = func_002CFF68(0x20);
    MdlResourceItem *previous = object->first;
    item->type = type;
    item->next = previous;
    item->subtype = subtype;
    object->first = item;
    return item;
}

void func_00219E30(MdlPartEntry *entry) {
    func_00151E60((u32)entry->object);
    entry->state = entry->state + 1;
}

void func_00219E68(MdlPartEntry *entry) {
    func_0014FEB0((u32)entry->object);
    entry->state = entry->state + 1;
}

s32 func_00219EA0(s32 arg0, s32 arg1) {
    s32 tmp;

    tmp = *(s32 *)(*(s32 *)(arg0 + 0xc) + 0xa8);
    if (tmp == 0) {
        return 0;
    }
    if (arg1 >= *(s16 *)(tmp + 4)) {
        return 0;
    }
    return *(s32 *)(tmp + 0xc) + arg1 * 0x10;
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_00219ED8);

INCLUDE_ASM(const s32, "game/code_00218B48", func_00219FD8);

void func_0021A088(u32 arg0, s32 arg1) {
    func_00217310(arg0, *(u16 *)(arg1 + 8), *(u16 *)(arg1 + 10),
                                *(u32 *)(arg1 + 0xc), *(u32 *)(arg1 + 0x10));
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021A0B0);

void mdlCondInitEntry(s32 arg0) {
    s32 v = *(s32 *)(arg0 + 0xC);
    if (*(u8 *)(v + 9) == 0) {
        s32 count = *(s32 *)(arg0 + 0x14);
        f32 f = *(f32 *)(*(s32 *)(*(s32 *)(arg0 + 8) + 0x1C) + 0x1C);
        if ((u32)(s32)f < (u32)count) {
            return;
        }
        mdlObjInit(v, *(s32 *)(arg0 + 0x10), arg0 + 0x18);
    }
}

INCLUDE_ASM(const s32, "game/code_00218B48", mdlDispatchResourceEntry);

void mdlApplyResourceEntries(s32 object, s32 id, s32 option) {
    s32 *block = func_002192D0(object, id);
    if (block != NULL) {
        s32 *entry = func_00219350((s32)block);
        while (entry != NULL) {
            mdlDispatchResourceEntry(object, entry, option);
            entry = func_00219368((s32)entry);
        }
    }
}

void mdlDestroyResourceItem(MdlResourceItem *item) {
    switch (item->type) {
    case 0:
        billDispatchByKind(item->resource);
        break;
    case 1:
        func_0014FAB8(item->resource);
        break;
    case 2:
        effTrackPolyRelease(item->resource);
        break;
    }
    func_002CFF98((void *)item);
}

void mdlRemoveResourceSubtype(MdlResourceOwner *object, s32 subtype) {
    MdlResourceItem **link = &object->first;
    MdlResourceItem *item = *link;
    while (item != 0) {
        if (item->subtype == subtype) {
            MdlResourceItem *next = item->next;
            mdlDestroyResourceItem(item);
            *link = next;
            item = next;
        } else {
            link = &item->next;
            item = item->next;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021A3D8);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021A490);

void mdlSetResourceFrame(s32 unused, MdlResourceItem *item, s32 frame) {
    switch (item->type) {
    case 0:
        func_00152010(item->resource, frame);
        return;
    case 1:
        func_0014FC60(item->resource, frame);
        break;
    }
}

void mdlSetResourceAmount(s32 unused, MdlResourceItem *item, float amount) {
    switch (item->type) {
    case 0:
        func_00152000(item->resource, amount, amount);
        return;
    case 1:
        func_0014FB70(item->resource, amount);
        break;
    }
}

s32 func_0021A608(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    return func_0011D3E8(arg0, arg1, arg2, arg3, arg4, 0x30000000, 0x60404040);
}

void func_0021A628(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s32 tmp;

    tmp = D_003D7B10[0];
    sdfAppendPacket(tmp, func_0021A608(arg0, arg1, arg2, arg3, arg4));
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021A660);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021A718);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021A880);

extern u128 D_003D7B20;
extern u128 D_003D7B30;
extern u128 D_003D7B40;
extern u128 D_00367A10;
extern u128 D_00367A20;
extern u128 D_00367A30;
extern s16 D_003D7A84[];

void func_0021A8F0(void) {
    PCP_COPY_VECTOR(&D_003D7B20, &D_00367A10);
    PCP_COPY_VECTOR(&D_003D7B30, &D_00367A20);
    PCP_COPY_VECTOR(&D_003D7B40, &D_00367A30);
    D_003D7A84[0] = 0;
}

void func_0021A948(void) {
    s32 i = D_003D7A50.resourceCount - 1;
    s32 saved = D_003D7A50.resources[i];

    if (i > 0) {
        do {
            D_003D7A50.resources[i] = D_003D7A50.slotBeforeResources[i];
            i -= 1;
        } while (i > 0);
    }
    D_003D7A50.resources[0] = saved;
}

void mdlRotateViewList(void) {
    s32 i;
    s32 count = D_003D7A50.resourceCount;
    s32 first = D_003D7A50.resources[0];

    for (i = 0; i < count - 1; i++) {
        D_003D7A50.resources[i] = D_003D7A50.resources[i + 1];
    }
    D_003D7A50.resources[i] = first;
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021A9F8);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021AAB8);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABBB0);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABBC0);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABBD0);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABBE0);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABBF0);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABC00);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABC10);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABC20);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABC30);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABC40);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABC50);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABC60);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABC70);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABC80);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021ABB8);

extern s32 mdlHasNode(s32 resource, s32 id);
extern void mdlAddEntryFlaggedEx(s32 resource, s32 id, s32 arg2, f32 arg4, f32 arg5);
extern void mdlAddEntryPlainEx(s32 resource, s32 id, s32 arg2, f32 arg4, f32 arg5);

void func_0021ACF0(void) {
    f32 low;
    f32 high;
    f32 first;

    if (mdlHasNode(D_003D7A50.resources[0], D_003D7A50.unk26)) {
        low = D_003D7A50.unk2E;
        high = D_003D7A50.unk30;
        first = high;
        if (low < high) {
            first = low;
        }
        D_003D7A50.unk22 = D_003D7A50.unk24;
        mdlAddEntryFlaggedEx(D_003D7A50.resources[0], D_003D7A50.unk26, D_003D7A50.unk24, first, low);
    }
}

void func_0021AD78(void) {
    f32 low;
    f32 high;
    f32 first;

    if (mdlHasNode(D_003D7A50.resources[0], D_003D7A50.unk26)) {
        low = D_003D7A50.unk2E;
        high = D_003D7A50.unk30;
        first = high;
        if (low < high) {
            first = low;
        }
        D_003D7A50.unk22 = D_003D7A50.unk24;
        mdlAddEntryPlainEx(D_003D7A50.resources[0], D_003D7A50.unk26, D_003D7A50.unk24, first, low);
    }
}

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABCC8);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABCD8);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021AE00);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021B0B0);

u32 func_0021B4E8(void) {
    func_0021AE00();
    func_0021B0B0();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021B510);

extern s32 D_00367B10[];
extern s32 func_002E4960();

void func_0021B950(void) {
    func_0021A628(0x8A10, 0x7948, 0xFF007F, 0x4E0, 0x90, 0);
    sdfAppendPacket(D_003D7A50.resources[12], func_002E4960(0x8A40, 0x7960, 0xFF0080, 0, D_00367B10[D_003D7A50.unk34]));
}

u32 func_0021B9D0(void) {
    func_0021B510();
    func_0021B950();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021B9F8);

extern s32 D_00367B18[];

void func_0021BDD0(void) {
    func_0021A628(0x8A10, 0x7948, 0xFF007F, 0x4E0, 0x90, 0);
    sdfAppendPacket(D_003D7A50.resources[12], func_002E4960(0x8A40, 0x7960, 0xFF0080, 0, D_00367B18[D_003D7A50.unk36]));
}

s32 func_0021BE50(void) {
    func_0021B9F8();
    if (D_003D7A60[0] == 0) {
        func_0021BDD0();
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021BE88);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021BFB0);

u32 func_0021C1E0(void) {
    func_0021BE88();
    func_0021BFB0();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021C208);

void func_0021C2E0(void) {
}

u32 func_0021C2E8(void) {
    func_0021C208();
    func_0021C2E0();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021C310);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021C518);

u32 func_0021C678(void) {
    func_0021C310();
    func_0021C518();
    return 0;
}

void func_0021C6A0(void) {
    s32 p = *(s32 *)(*(s32 *)(D_003D7A50.resources[0] + 0x18) + 8);

    if (p != 0) {
        s16 v = *(s16 *)(p + 4);

        if (v > 0) {
            func_0021A660((void *)((s32)&D_003D7A50 + 0x3A), v);
        }
    }
}

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABD58);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABD68);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABD78);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABD88);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABD98);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021C6E8);

u32 func_0021C8D0(void) {
    func_0021C6A0();
    func_0021C6E8();
    return 0;
}

s32 mdlIsDebugTimeGraph(void) {
    return kwlnTaskGetTaskByName("DebugTimeGrph") != 0;
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021C920);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABE18);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABE30);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABE48);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABE60);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABE70);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABE80);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021CB80);

u32 func_0021CE70(void) {
    func_0021C920();
    func_0021CB80();
    return 0;
}

s32 mdlCountActiveRecords(void) {
    s32 resource = D_003D7A50.resources[0];
    s32 first = mdlCountRecords((s32)func_002192D0(resource, -1));
    s32 second = mdlCountRecords((s32)func_002192D0(resource, D_003D7A50.unk22));

    return first + second;
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021CF00);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021CFC0);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021D198);

u32 func_0021D568(void) {
    func_0021CFC0();
    func_0021D198();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021D590);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021D5D0);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021D668);

void mdlViewerTaskDestroy(void) {
    if (D_003D7A50.viewerTask != 0) {
        kwlnTaskDestroyWithHierarchy(D_003D7A50.viewerTask, 0);
        D_003D7A50.viewerTask = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021D780);

INCLUDE_ASM(const s32, "game/code_00218B48", mdlViewer);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021DAE0);

INCLUDE_ASM(const s32, "game/code_00218B48", mdlViewerEnd);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021DD88);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021E068);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021E1C8);

extern void func_002177D0(s32 resource);

void mdlFreeViewResources(void) {
    s32 i;
    for (i = 0; i != 12; i++) {
        s32 handle = D_003D7A50.resources[i];
        if (handle != 0) {
            D_003D7A50.resources[i] = 0;
            func_002177D0(handle);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021E3C0);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021E450);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021E618);

void func_0021EB10(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp = arg2 & 0xffffff;

    func_0011E080(D_003D7B50.unk08, arg0, arg1,
                  (D_003D7B50.unk04 == 0) ? -1 : arg3, temp | 0x80000000, 1, temp);
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021EB60);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021ECF0);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021F098);

void func_0021F4B8(void) {
    mdlFlagClearAll();
    evtSetSolarPhase(0);
    func_00228778();
    func_00228728();
}

void mdlFlagClearAll(void) {
    s32 i = 0x7f;
    u32 *p = (u32 *)(D_003BAA00 + 0x840);

    do {
        i -= 1;
        *p = 0;
        p += 1;
    } while (i >= 0);
}

void mdlFlagClear(s32 flag);

void mdlClearFlagRanges(void) {
    s32 i = 0;
    do {
        mdlFlagClear(i++);
    } while (i < 0xC00);
    i = 0xD00;
    do {
        mdlFlagClear(i++);
    } while (i < 0x1000);
}

/* Signed flag indices need a bias before arithmetic right shift divides by 32. */
void mdlFlagSet(s32 flag) {
    s32 adjustedFlag = (flag < 0) ? flag + 0x1f : flag;
    s32 byteOffset = (adjustedFlag >> 5) * 4 + 0x840;
    *(u32 *)(D_003BAA00 + byteOffset) |= 1 << flag;
}

void mdlFlagClear(s32 flag) {
    s32 adjustedFlag = (flag < 0) ? flag + 0x1f : flag;
    s32 byteOffset = (adjustedFlag >> 5) * 4 + 0x840;
    *(u32 *)(D_003BAA00 + byteOffset) &= ~(1 << flag);
}

s32 mdlFlagTest(s32 flag) {
    s32 adjustedFlag = (flag < 0) ? flag + 0x1f : flag;
    s32 byteOffset = (adjustedFlag >> 5) * 4 + 0x840;
    return (*(s32 *)(D_003BAA00 + byteOffset) >> flag) & 1;
}

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABF78);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABF88);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABF98);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABFA8);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021F630);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021FA78);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021FB30);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021FC30);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021FD50);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABFE8);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABFF8);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBB6C);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBB70);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBB78);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBB80);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBB88);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBB90);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBB98);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBBA0);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBBA8);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBBB0);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBBB8);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBBC0);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBBC8);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBBD0);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBBD8);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBBE0);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBBE8);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBBF0);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBBF8);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBC00);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBC08);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBC10);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBC18);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBC20);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBC28);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBC30);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBC38);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBC40);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBC48);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBC50);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBC58);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBC60);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBC68);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBC70);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBC78);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBC80);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBC88);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBC90);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBC98);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBCA0);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBCA8);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBCB0);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBCB8);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBCC0);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBCC8);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBCD0);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBCD8);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBCE0);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBCE8);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBCF0);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBCF8);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBD00);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBD08);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBD10);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBD18);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBD20);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBD28);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBD30);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBD38);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBD40);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBD48);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBD50);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBD58);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBD60);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBD68);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBD70);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBD78);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBD80);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBD84);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBD88);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBD90);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBD98);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBDA0);

INCLUDE_SDATA(const s32, "game/code_00218B48", D_003BBDA8);

