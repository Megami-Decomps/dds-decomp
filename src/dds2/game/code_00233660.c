#include "common.h"

extern s32 D_00435DD0;

s32 billCreateIndexed(s32 arg0, s32 arg1);

s32 func_001578C0(s32 arg0);

typedef struct MdlViewState {
    s32 unk00;
    s32 unk04;
    s8 unk08;
    u8 pad09;
    s8 unk0A;
    u8 pad0B[11];
    s16 unk16;
    u8 pad18[10];
    s16 unk22;
    u8 pad24[0x68];
    s32 unk8C[1];
    s32 unk90[1];
} MdlViewState;

extern MdlViewState D_00453550;

extern s8 D_00453560[];

void func_00236568(void);

void func_00236940(void);

extern s32 func_00101740(void *name);

extern void kwlnTaskDestroyWithHierarchy(s32 task, s32 flag);

void func_00346A80(void *buffer, s32 arg1);

void func_00346AD8(void *buffer);

void func_00346CF0(void *buffer, s32 arg1, s32 arg2);

void func_00233280(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void func_00346AF8(void *buffer);

s32 mdlCountRecords(s32 arg0);

s32 *func_00233E40(s32 arg0, s32 arg1);

void func_00345628(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

void func_00157710(s32 arg0, float arg1);

void func_00159BF0(s32 arg0, float arg1, float arg2);

s32 func_0011F250(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);

s32 func_00235178(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

extern s32 D_00453610[];

void sdfAppendPacket(s32 arg0, s32 arg1);

extern u128 D_00453620;

extern u128 D_00453630;

extern u128 D_00453640;

extern u128 D_003C87C0;

extern u128 D_003C87D0;

extern u128 D_003C87E0;

extern s16 D_00453584[];

typedef struct MdlCtrlState {
    u8 pad00[4];
    u8 unk04;
    u8 pad05[3];
    s32 unk08;
} MdlCtrlState;

extern MdlCtrlState D_00453650;

void func_0011FEE8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);

void mdlFlagClear(s32 arg0);

void mdlLoadViewerPackage(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    u8 buffer[0x40];

    func_00346A80(buffer, 0);
    if (arg2 & 2) {
        func_00346AD8(buffer);
    }
    func_00346CF0(buffer, arg3, arg4);
    func_00233280(*(s32 *)(buffer + 0x30), arg0, arg1, arg2);
    func_00346AF8(buffer);
}

void func_00233700(void) {
    func_00328E48();
}

INCLUDE_ASM(const s32, "game/code_00233660", func_00233718);

INCLUDE_ASM(const s32, "game/code_00233660", func_002337C0);

INCLUDE_ASM(const s32, "game/code_00233660", func_00233938);

INCLUDE_ASM(const s32, "game/code_00233660", func_00233DD8);

typedef struct MdlRecord {
    s32 kind;       /* 0x00: 0xFFFF terminates the record chain */
    s32 nextOffset; /* 0x04: relative byte offset to next record */
    u32 value08;    /* 0x08 */
    u16 value0C;    /* 0x0C */
    u16 field0E;    /* 0x0E */
    u16 field10;    /* 0x10 */
} MdlRecord;

typedef struct MdlPartEntry {
    s32 kind;
    s32 state;
    s32 object;
    u8 pad0C[4];
} MdlPartEntry;

typedef struct MdlPartList {
    u8 pad00[4];
    s16 count;
    u8 pad06[6];
    MdlPartEntry *entries; /* 0x0C */
} MdlPartList;

typedef struct MdlResourceItem {
    struct MdlResourceItem *next; /* 0x00 */
    u16 type;                     /* 0x04 */
    s16 subtype;                  /* 0x06 */
    s32 resource;                 /* 0x08 */
    u8 pad0C[0x14];
} MdlResourceItem;

typedef struct MdlResourceOwner {
    u8 pad00[0x14];
    MdlResourceItem *first; /* 0x14 */
} MdlResourceOwner;

u32 func_00233E30(MdlRecord *record) {
    return record->value08;
}

u16 func_00233E38(MdlRecord *record) {
    return record->value0C;
}

INCLUDE_ASM(const s32, "game/code_00233660", func_00233E40);

s32 * func_00233EC0(s32 table) {
    s32 *first;

    first = (s32 *)(table + 8);
    if (*first == 0xffff) {
        first = (s32 *)0x0;
    }
    return first;
}

s32 * func_00233ED8(s32 record) {
    s32 *next;

    next = (s32 *)(record + *(s32 *)(record + 4));
    if (*next == 0xffff) {
        next = (s32 *)0x0;
    }
    return next;
}

s32 mdlCountRecords(s32 arg0) {
    s32 count;
    s32 *node;

    if (arg0 == 0) {
        return 0;
    }
    node = func_00233EC0(arg0);
    count = 0;
    while (node != NULL) {
        count++;
        node = func_00233ED8((s32)node);
    }
    return count;
}

u8 func_00233F48(s32 *arg0, s32 arg1) {
    return *arg0 == arg1;
}

u16 func_00233F58(MdlRecord *record) {
    return record->field0E;
}

u16 func_00233F60(MdlRecord *record) {
    return record->field10;
}

INCLUDE_ASM(const s32, "game/code_00233660", func_00233F68);

INCLUDE_ASM(const s32, "game/code_00233660", func_00234100);

INCLUDE_ASM(const s32, "game/code_00233660", func_00234448);

void func_00234668(MdlPartList *list, s32 index) {
    MdlPartEntry *entry = &list->entries[list->count];

    entry->state = 0;
    entry->kind = 0;
    entry->object = billCreateIndexed(1, index);
    list->count += 1;
}

void func_002346C0(MdlPartList *list, s32 index) {
    MdlPartEntry *entry = &list->entries[list->count];

    entry->kind = 1;
    entry->state = 0;
    entry->object = func_001578C0(index);
    list->count += 1;
}

INCLUDE_ASM(const s32, "game/code_00233660", func_00234720);

void mdlObjDestroy(s32 arg0) {
    if (*(u8 *)(arg0 + 9) != 0) {
        func_00344A08((void *)(arg0 + 0x20));
    }
    func_003297C8(*(s32 *)(arg0 + 4));
    func_00328E48((void *)arg0);
}

void mdlObjInit(s32 arg0, s32 arg1, s32 arg2) {
    if (*(u8 *)(arg0 + 9) == 0) {
        *(u8 *)(arg0 + 9) = 1;
        func_00345628(arg0 + 0x20, arg2, *(s32 *)(arg0 + 0), *(s32 *)(arg0 + 0x10), arg1);
    }
}

void func_00234838(u32 arg0) {
    sdfDevCreateBufferedRequest(arg0, 0x10, 4);
}

INCLUDE_ASM(const s32, "game/code_00233660", func_00234858);

MdlResourceItem *mdlInsertResourceItem(MdlResourceOwner *object, s32 type, s32 subtype) {
    extern void *func_00328E18(s32 size);
    MdlResourceItem *item = func_00328E18(0x20);
    MdlResourceItem *previous = object->first;
    item->type = type;
    item->next = previous;
    item->subtype = subtype;
    object->first = item;
    return item;
}

void func_002349A0(s32 arg0) {
    func_00159A50(*(u32 *)(arg0 + 8));
    *(s32 *)(arg0 + 4) = *(s32 *)(arg0 + 4) + 1;
}

void func_002349D8(s32 arg0) {
    func_00157A50(*(u32 *)(arg0 + 8));
    *(s32 *)(arg0 + 4) = *(s32 *)(arg0 + 4) + 1;
}

s32 func_00234A10(s32 arg0, s32 arg1) {
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

INCLUDE_ASM(const s32, "game/code_00233660", func_00234A48);

INCLUDE_ASM(const s32, "game/code_00233660", func_00234B48);

void func_00234BF8(u32 arg0, s32 arg1) {
    func_00231E28(arg0, *(u16 *)(arg1 + 8), *(u16 *)(arg1 + 10),
                                *(u32 *)(arg1 + 0xc), *(u32 *)(arg1 + 0x10));
}

INCLUDE_ASM(const s32, "game/code_00233660", func_00234C20);

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

INCLUDE_ASM(const s32, "game/code_00233660", mdlDispatchResourceEntry);

void mdlApplyResourceEntries(s32 object, s32 id, s32 option) {
    s32 *block = func_00233E40(object, id);
    if (block != NULL) {
        s32 *entry = func_00233EC0((s32)block);
        while (entry != NULL) {
            mdlDispatchResourceEntry(object, entry, option);
            entry = func_00233ED8((s32)entry);
        }
    }
}

void mdlDestroyResourceItem(MdlResourceItem *item) {
    switch (item->type) {
    case 0:
        billDispatchByKind(item->resource);
        break;
    case 1:
        func_00157658(item->resource);
        break;
    case 2:
        effTrackPolyRelease(item->resource);
        break;
    }
    func_00328E48((void *)item);
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

INCLUDE_ASM(const s32, "game/code_00233660", func_00234F48);

INCLUDE_ASM(const s32, "game/code_00233660", func_00235000);

void mdlSetResourceFrame(s32 unused, MdlResourceItem *item, s32 frame) {
    switch (item->type) {
    case 0:
        func_00159C00(item->resource, frame);
        return;
    case 1:
        func_00157800(item->resource, frame);
        break;
    }
}

void mdlSetResourceAmount(s32 unused, MdlResourceItem *item, float amount) {
    switch (item->type) {
    case 0:
        func_00159BF0(item->resource, amount, amount);
        return;
    case 1:
        func_00157710(item->resource, amount);
        break;
    }
}

s32 func_00235178(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    return func_0011F250(arg0, arg1, arg2, arg3, arg4, 0x30000000, 0x60404040);
}

void func_00235198(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 tmp;

    tmp = D_00453610[0];
    sdfAppendPacket(tmp, func_00235178(arg0, arg1, arg2, arg3, arg4));
}

INCLUDE_ASM(const s32, "game/code_00233660", func_002351D0);

INCLUDE_ASM(const s32, "game/code_00233660", func_00235288);

INCLUDE_ASM(const s32, "game/code_00233660", func_002353F0);

INCLUDE_ASM(const s32, "game/code_00233660", func_00235460);

void func_002354B8(void) {
    s32 i = D_00453550.unk16 - 1;
    s32 saved = D_00453550.unk90[i];

    if (i > 0) {
        do {
            D_00453550.unk90[i] = D_00453550.unk8C[i];
            i -= 1;
        } while (i > 0);
    }
    D_00453550.unk90[0] = saved;
}

void mdlRotateViewList(void) {
    s32 i;
    s32 count = D_00453550.unk16;
    s32 first = D_00453550.unk90[0];

    for (i = 0; i < count - 1; i++) {
        D_00453550.unk90[i] = D_00453550.unk90[i + 1];
    }
    D_00453550.unk90[i] = first;
}

INCLUDE_ASM(const s32, "game/code_00233660", func_00235568);

INCLUDE_ASM(const s32, "game/code_00233660", func_00235628);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421120);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421130);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421140);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421150);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421160);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421170);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421180);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421190);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004211A0);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004211B0);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004211C0);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004211D0);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004211E0);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004211F0);

INCLUDE_ASM(const s32, "game/code_00233660", func_00235728);

INCLUDE_ASM(const s32, "game/code_00233660", func_00235860);

INCLUDE_ASM(const s32, "game/code_00233660", func_002358E8);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421238);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421248);

INCLUDE_ASM(const s32, "game/code_00233660", func_00235970);

INCLUDE_ASM(const s32, "game/code_00233660", func_00235C20);

u32 func_00236058(void) {
    func_00235970();
    func_00235C20();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00233660", func_00236080);

INCLUDE_ASM(const s32, "game/code_00233660", func_002364C0);

u32 func_00236540(void) {
    func_00236080();
    func_002364C0();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00233660", func_00236568);

INCLUDE_ASM(const s32, "game/code_00233660", func_00236940);

s32 func_002369C0(void) {
    func_00236568();
    if (D_00453560[0] == 0) {
        func_00236940();
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00233660", func_002369F8);

INCLUDE_ASM(const s32, "game/code_00233660", func_00236B20);

u32 func_00236D50(void) {
    func_002369F8();
    func_00236B20();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00233660", func_00236D78);

void func_00236E50(void) {
}

u32 func_00236E58(void) {
    func_00236D78();
    func_00236E50();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00233660", func_00236E80);

INCLUDE_ASM(const s32, "game/code_00233660", func_00237088);

u32 func_002371E8(void) {
    func_00236E80();
    func_00237088();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00233660", func_00237210);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004212C8);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004212D8);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004212E8);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004212F8);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421308);

INCLUDE_ASM(const s32, "game/code_00233660", func_00237258);

u32 func_00237440(void) {
    func_00237210();
    func_00237258();
    return 0;
}

s32 mdlIsDebugTimeGraph(void) {
    return func_00101740("DebugTimeGrph") != 0;
}

INCLUDE_ASM(const s32, "game/code_00233660", func_00237490);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421388);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004213A0);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004213B8);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004213D0);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004213E0);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004213F0);

INCLUDE_ASM(const s32, "game/code_00233660", func_002376F0);

u32 func_002379E0(void) {
    func_00237490();
    func_002376F0();
    return 0;
}

s32 mdlCountActiveRecords(void) {
    s32 resource = D_00453550.unk90[0];
    s32 first = mdlCountRecords((s32)func_00233E40(resource, -1));
    s32 second = mdlCountRecords((s32)func_00233E40(resource, D_00453550.unk22));

    return first + second;
}

INCLUDE_ASM(const s32, "game/code_00233660", func_00237A70);

INCLUDE_ASM(const s32, "game/code_00233660", func_00237B30);

INCLUDE_ASM(const s32, "game/code_00233660", func_00237D08);

u32 func_002380D8(void) {
    func_00237B30();
    func_00237D08();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00233660", func_00238100);

INCLUDE_ASM(const s32, "game/code_00233660", func_00238140);

INCLUDE_ASM(const s32, "game/code_00233660", func_002381D8);

void mdlViewerTaskDestroy(void) {
    if (D_00453550.unk04 != 0) {
        kwlnTaskDestroyWithHierarchy(D_00453550.unk04, 0);
        D_00453550.unk04 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00233660", func_002382F0);

INCLUDE_ASM(const s32, "game/code_00233660", mdlViewer);

INCLUDE_ASM(const s32, "game/code_00233660", func_00238650);

INCLUDE_ASM(const s32, "game/code_00233660", mdlViewerEnd);

INCLUDE_ASM(const s32, "game/code_00233660", func_002388F8);

INCLUDE_ASM(const s32, "game/code_00233660", func_00238BD8);

INCLUDE_ASM(const s32, "game/code_00233660", func_00238D38);

INCLUDE_ASM(const s32, "game/code_00233660", mdlFreeViewResources);

INCLUDE_ASM(const s32, "game/code_00233660", func_00238F30);

INCLUDE_ASM(const s32, "game/code_00233660", func_00238FC0);

INCLUDE_ASM(const s32, "game/code_00233660", func_00239188);

void func_00239680(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp = arg2 & 0xffffff;

    func_0011FEE8(D_00453650.unk08, arg0, arg1,
                  (D_00453650.unk04 == 0) ? -1 : arg3, temp | 0x80000000, 1, temp);
}

INCLUDE_ASM(const s32, "game/code_00233660", func_002396D0);

INCLUDE_ASM(const s32, "game/code_00233660", func_00239860);

INCLUDE_ASM(const s32, "game/code_00233660", func_00239C08);

void func_0023A028(void) {
    mdlFlagClearAll();
    evtSetSolarPhase(0);
    func_002433E8();
    func_00243398();
}

void mdlFlagClearAll(void) {
    u32 *puVar1;
    s32 temp_v0;

    temp_v0 = 0x7f;
    puVar1 = (u32 *)(D_00435DD0 + 0x840);
    do {
        temp_v0 = temp_v0 - 1;
        *puVar1 = 0;
        puVar1 = puVar1 + 1;
    } while (-1 < temp_v0);
}

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

void mdlFlagSet(s32 arg0) {
    s32 temp_v0 = (arg0 < 0) ? arg0 + 0x1f : arg0;
    s32 off = (temp_v0 >> 5) * 4 + 0x840;
    *(u32 *)(D_00435DD0 + off) |= 1 << arg0;
}

void mdlFlagClear(s32 arg0) {
    s32 temp_v0 = (arg0 < 0) ? arg0 + 0x1f : arg0;
    s32 off = (temp_v0 >> 5) * 4 + 0x840;
    *(u32 *)(D_00435DD0 + off) &= ~(1 << arg0);
}

s32 mdlFlagTest(s32 arg0) {
    s32 temp_v0 = (arg0 < 0) ? arg0 + 0x1f : arg0;
    s32 off = (temp_v0 >> 5) * 4 + 0x840;
    return (*(s32 *)(D_00435DD0 + off) >> arg0) & 1;
}

INCLUDE_RODATA(const s32, "game/code_00233660", D_004214E8);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004214F8);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421508);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421518);

INCLUDE_ASM(const s32, "game/code_00233660", func_0023A1A0);

INCLUDE_ASM(const s32, "game/code_00233660", func_0023A5E8);

INCLUDE_ASM(const s32, "game/code_00233660", func_0023A6A0);

INCLUDE_ASM(const s32, "game/code_00233660", func_0023A7A0);

INCLUDE_ASM(const s32, "game/code_00233660", func_0023A8C0);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421558);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421568);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00436FAC);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00436FB0);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00436FB8);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00436FC0);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00436FC8);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00436FD0);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00436FD8);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00436FE0);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00436FE8);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00436FF0);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00436FF8);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00437000);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00437008);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00437010);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00437018);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00437020);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00437028);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00437030);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00437038);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00437040);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00437048);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00437050);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00437058);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00437060);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00437068);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00437070);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00437078);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00437080);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00437088);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00437090);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00437098);

INCLUDE_SDATA(const s32, "game/code_00233660", D_004370A0);

INCLUDE_SDATA(const s32, "game/code_00233660", D_004370A8);

INCLUDE_SDATA(const s32, "game/code_00233660", D_004370B0);

INCLUDE_SDATA(const s32, "game/code_00233660", D_004370B8);

INCLUDE_SDATA(const s32, "game/code_00233660", D_004370C0);

INCLUDE_SDATA(const s32, "game/code_00233660", D_004370C8);

INCLUDE_SDATA(const s32, "game/code_00233660", D_004370D0);

INCLUDE_SDATA(const s32, "game/code_00233660", D_004370D8);

INCLUDE_SDATA(const s32, "game/code_00233660", D_004370E0);

INCLUDE_SDATA(const s32, "game/code_00233660", D_004370E8);

INCLUDE_SDATA(const s32, "game/code_00233660", D_004370F0);

INCLUDE_SDATA(const s32, "game/code_00233660", D_004370F8);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00437100);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00437108);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00437110);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00437118);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00437120);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00437128);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00437130);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00437138);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00437140);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00437148);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00437150);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00437158);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00437160);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00437168);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00437170);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00437178);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00437180);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00437188);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00437190);

INCLUDE_SDATA(const s32, "game/code_00233660", D_00437198);

INCLUDE_SDATA(const s32, "game/code_00233660", D_004371A0);

INCLUDE_SDATA(const s32, "game/code_00233660", D_004371A8);

INCLUDE_SDATA(const s32, "game/code_00233660", D_004371B0);

INCLUDE_SDATA(const s32, "game/code_00233660", D_004371B8);

INCLUDE_SDATA(const s32, "game/code_00233660", D_004371C0);

INCLUDE_SDATA(const s32, "game/code_00233660", D_004371C4);

INCLUDE_SDATA(const s32, "game/code_00233660", D_004371C8);

INCLUDE_SDATA(const s32, "game/code_00233660", D_004371D0);

INCLUDE_SDATA(const s32, "game/code_00233660", D_004371D8);

INCLUDE_SDATA(const s32, "game/code_00233660", D_004371E0);

INCLUDE_SDATA(const s32, "game/code_00233660", D_004371E8);
