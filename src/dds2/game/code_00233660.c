#include "common.h"

#include "pcp_vu0.h"

extern s32 func_0033D810();

extern void func_00328E48();

extern s32 D_003C88C0[];

extern s32 D_003C88C8[];

extern s32 kwlnTaskDestroyWithHierarchyByName(char *, s32);

extern void func_00103388(s32, s32, s32, s32);

extern s32 mdlHasNode(s32, s16);

extern void mdlAddEntryFlaggedEx(s32, s16, s16, f32, f32);

extern void mdlAddEntryPlainEx(s32, s16, s16, f32, f32);

extern s32 D_00435DD0;

s32 billCreateIndexed(s32 arg0, s32 arg1);

s32 func_001578C0(s32 arg0);

typedef struct MdlViewState {
    s32 unk00;
    s32 viewerTask;
    s8 unk08;
    s8 unk09;
    s8 unk0A;
    s8 unk0B;
    u8 unk0C;
    u8 pad0D[2];
    u8 unk0F;
    u8 unk10;
    u8 pad11[3];
    s16 unk14;
    s16 resourceCount;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 unk22;
    u16 unk24;
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
    u8 pad3C[6];
    s16 unk42;
    u8 pad44[4];
    s16 unk48;
    s16 unk4A;
    s16 unk4C;
    s16 unk4E;
    s16 unk50;
    s16 unk52;
    u8 pad54[0x38];
    s32 slotBeforeResources[1];
    void *resources[1];
    u8 pad94[0x2C];
    s32 unkC0;
} MdlViewState;

extern MdlViewState D_00453550;

typedef struct MdlCountNode {
    u8 pad00[4];
    s16 count; /* 0x04 */
} MdlCountNode;

typedef struct MdlLoadedInfo {
    u8 pad00[8];
    MdlCountNode *first; /* 0x08 */
} MdlLoadedInfo;

typedef struct MdlLoaded {
    u8 pad00[0x18];
    MdlLoadedInfo *info; /* 0x18 */
    s32 flags1C;         /* 0x1C */
} MdlLoaded;

extern void *memset(void *dst, s32 value, u32 size);

extern s32 dds3AdvanceWorldCounter();

extern s32 dds3SpawnCameraSlotObj5(s32 world, f32 *pos, f32 *rot);

extern void dds3SetObjectFlags(s32 obj, u32 flags);

extern void effObjSetInnerFloat(s32 obj, f32 value);

extern void func_00112058(s32 obj, s32 a, s32 b);

extern MdlLoaded *dds3GetUnk0C(s32 obj);

extern void func_001129C8(s32 obj, s32 a);

extern s32 dds3GetWorldSecondaryObject();

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

s32 *mdlFindViewerRecord(s32 arg0, s32 arg1);

void sdfStreamCreateWithParams(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

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

extern char *D_003C8768[];

extern u8 D_003C8760[];

extern u16 D_003C8730[];

extern char D_00436FF0[];

extern char D_00436FF8[];

/* Assemble the resource request in a temporary buffer before loading it. */

#define MDL_PART_OBJECT 3

typedef struct MdlHandlerNode {
    s32 a;      /* 0x00 */
    void *b;    /* 0x04 */
    u8 pad08[8];
    s32 c;      /* 0x10 */
    u8 pad14[0x98];
} MdlHandlerNode;

typedef struct MdlNodeInfo {
    s32 id;       /* 0x00 */
    u8 pad04[0xC];
    f32 pos[4];   /* 0x10 */
} MdlNodeInfo;

typedef struct MdlAnchorRec {
    u8 pad00[4];
    u16 type;          /* 0x04 */
    u8 pad06[2];
    s32 handle;        /* 0x08 */
    u8 pad0C[4];
    MdlNodeInfo *info; /* 0x10 */
    f32 scale;         /* 0x14 */
} MdlAnchorRec;

extern u8 D_0037F690[];

extern u8 *func_00330C18(void *chunk, s32 id);

extern void effCopyVector(s32 handle, f32 *src);

extern void billInvokeCallback(s32 handle);

extern void func_00157790(s32 handle, f32 *pos);

extern void effUpdateNode(s32 handle);

extern void func_0018FF50(s32 handle);

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

typedef struct MdlSlotEntry {
    s32 a;
    s32 b;
    s32 c;
} MdlSlotEntry;

typedef struct MdlSlotTable {
    MdlSlotEntry *entries;
    s32 count;
} MdlSlotTable;

extern MdlSlotTable D_003C6588[];

extern MdlSlotTable D_003C86B0[];

extern s32 D_00436FAC;

extern s32 D_00436FB0;

extern void sdfReleaseMemorySlot(s32 *slot);

void func_00233718(void) {
    MdlSlotEntry *entry;
    s32 count;
    s32 i;

    count = D_003C6588[5].count;
    if (count > 0) {
        entry = D_003C6588[5].entries;
        i = 0;
        do {
            i++;
            func_00328E48(entry->b);
            func_00328E48(entry->a);
            func_00328E48(entry->c);
            entry++;
        } while (i < count);
    }
    D_003C86B0[5].entries = NULL;
    D_003C86B0[5].count = 0;
    D_003C6588[5].entries = NULL;
    D_003C6588[5].count = 0;
    sdfReleaseMemorySlot(&D_00436FAC);
    sdfReleaseMemorySlot(&D_00436FB0);
}

INCLUDE_ASM(const s32, "game/code_00233660", func_002337C0);

INCLUDE_ASM(const s32, "game/code_00233660", func_00233938);

s32 func_00233DD8(s32 table, s32 slot, s32 a, s32 b, s32 c) {
    MdlSlotEntry *entries = D_003C6588[table].entries;

    if (entries == NULL) {
        return 0;
    }
    if (slot >= D_003C6588[table].count) {
        return 0;
    }
    entries[slot].a = a;
    entries[slot].b = b;
    entries[slot].c = c;
    return 1;
}

typedef struct MdlRecord {
    s32 kind;       /* 0x00: 0xFFFF terminates the record chain */
    s32 nextOffset; /* 0x04: relative byte offset to next record */
    u32 value08;    /* 0x08 */
    u16 value0C;    /* 0x0C */
    u16 field0E;    /* 0x0E */
    u16 field10;    /* 0x10 */
} MdlRecord;

typedef struct MdlObj {
    s32 unk0;             /* 0x00 */
    s32 handle;           /* 0x04 */
    u8 inUse;             /* 0x08: set when an item claims the object */
    u8 initialized;       /* 0x09 */
    u8 pad0A[6];
    s32 unk10;            /* 0x10 */
    u8 pad14[0xC];
    u8 data[1];           /* 0x20 */
} MdlObj;

typedef struct MdlPartEntry {
    u32 kind;     /* 0x00: billboard or effect */
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

#define MDL_PART_BILLBOARD 0

#define MDL_PART_EFFECT 1

typedef struct MdlItemCfg {
    u8 pad0;
    u8 enabled; /* 0x01 */
    u8 unk2;
    u8 unk3;
} MdlItemCfg;

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
    void *chunk;            /* 0x18 */
} MdlResourceOwner;

/* Read the model record's payload word without advancing its relative link. */
u32 func_00233E30(MdlRecord *record) {
    return record->value08;
}

u16 func_00233E38(MdlRecord *record) {
    return record->value0C;
}

/* Follow relative links in the resource's record table to find an ID. */
s32 *mdlFindViewerRecord(s32 arg0, s32 key) {
    s32 *list = *(s32 **)(*(s32 *)(arg0 + 0xC) + 0xA4);
    s32 *entry;
    s32 remaining;

    if (list == NULL) {
        return NULL;
    }
    entry = list;
    remaining = func_00233E38((MdlRecord *)entry);
    while (1) {
        remaining--;
        entry = (s32 *)((u8 *)entry + entry[1]);
        if (remaining == -1) {
            return NULL;
        }
        if (*entry == key) {
            return entry;
        }
    }
}

s32 * mdlGetFirstRecord(s32 table) {
    MdlRecord *first;

    first = (MdlRecord *)(table + 8);
    if (first->kind == 0xffff) {
        first = NULL;
    }
    return (s32 *)first;
}

s32 * mdlGetNextRecord(s32 record) {
    MdlRecord *next;

    next = (MdlRecord *)(record + ((MdlRecord *)record)->nextOffset);
    if (next->kind == 0xffff) {
        next = NULL;
    }
    return (s32 *)next;
}

/* Count relative-offset records until the 0xffff sentinel. */
s32 mdlCountRecords(s32 address) {
    s32 count;
    s32 *node;

    if (address == 0) {
        return 0;
    }
    node = mdlGetFirstRecord(address);
    count = 0;
    while (node != NULL) {
        count++;
        node = mdlGetNextRecord((s32)node);
    }
    return count;
}

u8 mdlRecordMatchesId(s32 *recordId, s32 wantedId) {
    return *recordId == wantedId;
}

u16 func_00233F58(MdlRecord *record) {
    return record->field0E;
}

u16 func_00233F60(MdlRecord *record) {
    return record->field10;
}

INCLUDE_ASM(const s32, "game/code_00233660", func_00233F68);

/* Draw labeled marker fields and each of four packed color channels. */
void mdlDrawMarkParamsPanel(s32 list, s32 x, s32 y, s32 z, EffMarkParams *params, s32 selected) {
    s32 boxY = y + 0x300;
    s32 labelX = x + 0xC0;
    u32 color;
    s32 row;
    s32 col;

    sdfAppendPacket(list, func_0033D810(labelX, y, z, 0, "MARK0:%d", params->mark0));
    sdfAppendPacket(list, func_0033D810(labelX, y + 0x60, z, 0, "MARK1:%d", params->mark1));
    sdfAppendPacket(list, func_0033D810(labelX, y + 0xC0, z, 0, "START:%d", params->start));
    sdfAppendPacket(list, func_0033D810(labelX, y + 0x120, z, 0, "END  :%d", params->end));
    sdfAppendPacket(list, func_0033D810(labelX, y + 0x180, z, 0, "ITRVL:%d", params->interval));
    sdfAppendPacket(list, func_0033D810(labelX, y + 0x1E0, z, 0, "FACE :%d", params->face));
    sdfAppendPacket(list, func_0033D810(labelX, y + 0x240, z, 0, "BLEND:%d", params->blend));
    for (row = 0; row != 4; row++) {
        color = params->colors[row];
        sdfAppendPacket(list, func_0011F250(x + 0x480, boxY, z, 0x300, 0x180, (color & 0xFFFFFF) | 0x80000000, 0x60404040));
        for (col = 0; col != 4; col++) {
            sdfAppendPacket(list, func_0033D810(labelX, boxY, z, D_003C8760[col], D_003C8768[col]));
            sdfAppendPacket(list, func_0033D810(x + 0x240, boxY, z, 0, D_00436FF0, color & 0xFF));
            color >>= 8;
            boxY += 0x60;
        }
        boxY += 0x60;
    }
    if (selected >= 0) {
        sdfAppendPacket(list, func_0033D810(x, y + D_003C8730[selected], z, 0, D_00436FF8));
    }
}

INCLUDE_ASM(const s32, "game/code_00233660", func_00234448);

void mdlAddBillboardPart(MdlPartList *list, s32 index) {
    MdlPartEntry *entry = &list->entries[list->count];

    entry->state = 0;
    entry->kind = MDL_PART_BILLBOARD;
    entry->object = billCreateIndexed(1, index);
    list->count += 1;
}

void mdlAddEffectPart(MdlPartList *list, s32 index) {
    MdlPartEntry *entry = &list->entries[list->count];

    entry->kind = MDL_PART_EFFECT;
    entry->state = 0;
    entry->object = func_001578C0(index);
    list->count += 1;
}

void func_00234720(MdlPartList *list, s32 a, void *b, s32 c) {
    MdlHandlerNode *node = func_00328E18(0xAC);
    MdlPartEntry *entry = &list->entries[list->count];

    node->c = c;
    node->a = a;
    node->b = b;
    entry->kind = MDL_PART_OBJECT;
    entry->state = 0;
    entry->object = (s32)node;
    list->count += 1;
}

void mdlObjDestroy(MdlObj *obj) {
    if (obj->initialized != 0) {
        func_00344A08(obj->data);
    }
    func_003297C8(obj->handle);
    func_00328E48(obj);
}

void mdlObjInit(MdlObj *obj, s32 arg1, s32 arg2) {
    if (obj->initialized == 0) {
        obj->initialized = 1;
        sdfStreamCreateWithParams((s32)obj->data, arg2, obj->unk0, obj->unk10, arg1);
    }
}

void func_00234838(u32 arg0) {
    sdfDevCreateBufferedRequest(arg0, 0x10, 4);
}

void func_00234858(MdlPartList *list) {
    s32 i;

    if (list != NULL) {
        for (i = 0; i < list->count; i++) {
            MdlPartEntry *entry = &list->entries[i];

            switch (entry->kind) {
            case MDL_PART_BILLBOARD:
                billDispatchByKind(entry->object);
                break;
            case MDL_PART_EFFECT:
                effDestroyNode(entry->object);
                break;
            case MDL_PART_OBJECT:
                mdlObjDestroy((MdlObj *)entry->object);
                break;
            }
        }
        sdfDestroyDevRequest(list);
    }
}

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

void mdlAdvanceBillboardPart(MdlPartEntry *entry) {
    func_00159A50((u32)entry->object);
    entry->state = entry->state + 1;
}

void mdlAdvanceEffectPart(MdlPartEntry *entry) {
    func_00157A50((u32)entry->object);
    entry->state = entry->state + 1;
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

s32 func_00234BF8(u32 arg0, s32 arg1) {
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
        mdlObjInit((MdlObj *)v, *(s32 *)(arg0 + 0x10), arg0 + 0x18);
    }
}

extern s32 func_00234A48(s32 object, s32 *record, s32 option, s32 type, void (*advance)());

extern s32 func_00234C20(s32 object, s32 *record, s32 option);

s32 mdlDispatchResourceEntry(s32 object, s32 *record, s32 option) {
    switch (*record) {
    case 1:
        return func_00234A48(object, record, option, 0, mdlAdvanceBillboardPart);
    case 2:
        return func_00234A48(object, record, option, 1, mdlAdvanceEffectPart);
    case 3:
        return func_00234B48(object, record, option);
    case 4:
        return func_00234BF8(object, record);
    case 5:
        func_00234C20(object, record, option);
        break;
    }
}

void mdlApplyResourceEntries(s32 object, s32 id, s32 option) {
    s32 *block = mdlFindViewerRecord(object, id);
    if (block != NULL) {
        s32 *entry = mdlGetFirstRecord((s32)block);
        while (entry != NULL) {
            mdlDispatchResourceEntry(object, entry, option);
            entry = mdlGetNextRecord((s32)entry);
        }
    }
}

void mdlDestroyResourceItem(MdlResourceItem *item) {
    switch (item->type) {
    case 0:
        billDispatchByKind(item->resource);
        break;
    case 1:
        effDestroyNode(item->resource);
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

/* vu0 routine: out = p + normalize(p - D_00324690) * scale, p = node position transformed by the node matrix */
void func_00234F48(void *chunk, MdlAnchorRec *rec, f32 *out) {
    MdlNodeInfo *info = rec->info;
    u8 *matrix = func_00330C18(chunk, info->id);
    f32 scale = rec->scale;

    __asm__ volatile(".set noreorder\n\tlqc2 vf28, 0(%0)\n\tlqc2 vf29, 0x10(%0)\n\tlqc2 vf30, 0x20(%0)\n\tlqc2 vf31, 0x30(%0)\n\t.set reorder" : : "r"(matrix + 0xC0));
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(info->pos));
    __asm__ volatile(".set noreorder\n\tvmulax.xyzw ACC, vf28, vf10x\n\tvmadday.xyzw ACC, vf29, vf10y\n\tvmaddaz.xyzw ACC, vf30, vf10z\n\tvmaddw.xyzw vf10, vf31, vf0w\n\tvmove.xyzw vf11, vf10\n\t.set reorder");
    __asm__ volatile(".set noreorder\n\tlqc2 vf12, 0(%0)\n\t.set reorder" : : "r"(D_0037F690));
    __asm__ volatile(".set noreorder\n\tvsub.xyzw vf10, vf10, vf12\n\tvmul.xyz vf2, vf10, vf10\n\tvmulax.w ACC, vf0, vf2x\n\tvmadday.w ACC, vf0, vf2y\n\tvmaddz.w vf2, vf0, vf2z\n\tvrsqrt Q, vf0w, vf2w\n\tvwaitq\n\tvmulq.xyz vf10, vf10, Q\n\t.set reorder");
    __asm__ volatile(".set noreorder\n\tqmtc2.ni %0, vf2\n\t.set reorder" : : "r"(scale));
    __asm__ volatile(".set noreorder\n\tvmulx.xyzw vf10, vf10, vf2x\n\tvadd.xyzw vf10, vf10, vf11\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(out));
}

void func_00235000(MdlResourceOwner *owner, MdlAnchorRec *rec) {
    void *chunk = owner->chunk;
    f32 position[4];
    s32 handle;

    switch (rec->type) {
    case 0:
        func_00234F48(chunk, rec, position);
        handle = rec->handle;
        effCopyVector(handle, position);
        billInvokeCallback(handle);
        break;
    case 1:
        func_00234F48(chunk, rec, position);
        handle = rec->handle;
        func_00157790(handle, position);
        effUpdateNode(handle);
        break;
    case 2:
        func_0018FF50(rec->handle);
        break;
    case 3:
        mdlCondInitEntry((s32)rec);
        break;
    }
}

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

void func_00235198(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s32 tmp;

    tmp = D_00453610[0];
    sdfAppendPacket(tmp, func_00235178(arg0, arg1, arg2, arg3, arg4));
}

INCLUDE_ASM(const s32, "game/code_00233660", func_002351D0);

INCLUDE_ASM(const s32, "game/code_00233660", func_00235288);

extern MdlLoaded *func_00232198(s16 a, s16 b);

extern void mdlAddEntryFlagged(MdlLoaded *loaded, s32 a, s32 b);

void func_002353F0(void) {
    MdlLoaded *loaded;

    loaded = func_00232198(D_00453550.unk18, D_00453550.unk1A);
    D_00453550.resources[0] = loaded;
    D_00453550.unk22 = 0;
    D_00453550.unk24 = 0;
    D_00453550.unk26 = 0;
    D_00453550.unk28 = 0;
    D_00453550.unk2A = 0;
    D_00453550.unk2C = 0;
    if (loaded->flags1C != 0) {
        mdlAddEntryFlagged(loaded, 0, 0);
    }
    D_00453550.unk3A = 0;
}

void func_00235460(void) {
    PCP_COPY_VECTOR(&D_00453620, &D_003C87C0);
    PCP_COPY_VECTOR(&D_00453630, &D_003C87D0);
    PCP_COPY_VECTOR(&D_00453640, &D_003C87E0);
    D_00453584[0] = 0;
}

void mdlRotateViewResourcesRight(void) {
    s32 i = D_00453550.resourceCount - 1;
    s32 saved = D_00453550.resources[i];

    if (i > 0) {
        do {
            D_00453550.resources[i] = D_00453550.slotBeforeResources[i];
            i -= 1;
        } while (i > 0);
    }
    D_00453550.resources[0] = saved;
}

void mdlRotateViewList(void) {
    s32 i;
    s32 count = D_00453550.resourceCount;
    s32 first = D_00453550.resources[0];

    for (i = 0; i < count - 1; i++) {
        D_00453550.resources[i] = D_00453550.resources[i + 1];
    }
    D_00453550.resources[i] = first;
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

void mdlAddViewEntryFlagged(void) {
    MdlViewState *state = &D_00453550;
    f32 width;
    f32 height;

    if (mdlHasNode(state->resources[0], state->unk26) == 0) {
        return;
    }
    height = (f32)state->unk2E;
    width = (f32)state->unk30;
    if (height < width) {
        width = height;
    }
    state->unk22 = state->unk24;
    mdlAddEntryFlaggedEx(state->resources[0], state->unk26, state->unk24, width, height);
}

void func_002358E8(void) {
    MdlViewState *state = &D_00453550;
    f32 width;
    f32 height;

    if (mdlHasNode(state->resources[0], state->unk26) == 0) {
        return;
    }
    height = (f32)state->unk2E;
    width = (f32)state->unk30;
    if (height < width) {
        width = height;
    }
    state->unk22 = state->unk24;
    mdlAddEntryPlainEx(state->resources[0], state->unk26, state->unk24, width, height);
}

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

void func_002364C0(void) {
    s32 packets;

    func_00235198(0x8A10L, 0x7948, 0xFF007F, 0x4E0, 0x90, 0);
    packets = D_00453550.unkC0;
    sdfAppendPacket(packets, func_0033D810(0x8A40L, 0x7960, 0xFF0080, 0, D_003C88C0[D_00453550.unk34]));
}

u32 func_00236540(void) {
    func_00236080();
    func_002364C0();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00233660", func_00236568);

void func_00236940(void) {
    s32 packets;

    func_00235198(0x8A10L, 0x7948, 0xFF007F, 0x4E0, 0x90, 0);
    packets = D_00453550.unkC0;
    sdfAppendPacket(packets, func_0033D810(0x8A40L, 0x7960, 0xFF0080, 0, D_003C88C8[D_00453550.unk36]));
}

s32 func_002369C0(void) {
    func_00236568();
    if (D_00453560[0] == 0) {
        func_00236940();
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00233660", mdlAdjustViewerScale);

INCLUDE_ASM(const s32, "game/code_00233660", func_00236B20);

u32 func_00236D50(void) {
    mdlAdjustViewerScale();
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

void func_00237210(void) {
    MdlCountNode *node;

    node = ((MdlLoaded *)D_00453550.resources[0])->info->first;
    if (node != NULL) {
        if (node->count > 0) {
            func_002351D0(&D_00453550.unk3A, node->count);
        }
    }
}

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
    s32 resource = D_00453550.resources[0];
    s32 first = mdlCountRecords((s32)mdlFindViewerRecord(resource, -1));
    s32 second = mdlCountRecords((s32)mdlFindViewerRecord(resource, D_00453550.unk22));

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

extern void func_00102908();

s32 func_00238100(void) {
    if (D_00453550.unk08 == 0) {
        D_00453550.unk08 = 1;
        func_00238F30();
        func_00102908();
    }
    return 0;
}

typedef struct MdlDrawSurface {
    u8 pad00[0x10];
    void (*submit)(struct MdlDrawSurface *, s32);
} MdlDrawSurface;

extern MdlDrawSurface D_00380048;

extern u8 D_003C8A00[];

extern u8 D_003C8A60[];

extern s32 sdfCreateResetPacketList(void);

extern s32 func_00348188(void *, void *, s32, s32);

extern s32 sdfCountMapPositionRecords(s32);

extern s32 sdfChunkFindByTag(s32, s32);

extern void sdfSetLookAtBasisFromRecord(s32, s32);

extern u8 D_003C8930[];

extern u8 D_003C8990[];

void func_00238140(void) {
    s32 list;

    if (D_00453550.unk0A < 4) {
        if (D_00453550.unk0A >= 2) {
            list = sdfCreateResetPacketList();
            __asm__ volatile(
                ".set noreorder\n\t"
                "vsub.xyzw $vf28, $vf0, $vf0\n\t"
                "vmr32.xyzw $vf30, $vf0\n\t"
                "vmove.xyzw $vf31, $vf0\n\t"
                "vaddw.x $vf28, $vf28, $vf0w\n\t"
                "vmr32.xyzw $vf29, $vf30\n\t"
                ".set reorder" ::: "memory");
            sdfAppendPacket(list, func_00348188(D_003C8930, D_003C8990, 6, 0x80));
            D_00380048.submit(&D_00380048, list);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00233660", func_002381D8);

void mdlViewerTaskDestroy(void) {
    if (D_00453550.viewerTask != 0) {
        kwlnTaskDestroyWithHierarchy(D_00453550.viewerTask, 0);
        D_00453550.viewerTask = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00233660", func_002382F0);

INCLUDE_ASM(const s32, "game/code_00233660", mdlViewer);

void func_00238650(u8 *obj) {
    s32 count = sdfCountMapPositionRecords(*(s32 *)(obj + 0x18));

    if (count > 0) {
        s32 i = 0;
        s32 list = sdfCreateResetPacketList();
        s32 record = sdfChunkFindByTag(*(s32 *)(obj + 0x18), 0x534F504D) + 0x10;

        do {
            s32 current = record;

            i++;
            record += 0x40;
            sdfSetLookAtBasisFromRecord(*(s32 *)(obj + 0x18), current);
            sdfAppendPacket(list, func_00348188(D_003C8A00, D_003C8A60, 6, 0x80));
        } while (i != count);
        D_00380048.submit(&D_00380048, list);
    }
}

INCLUDE_ASM(const s32, "game/code_00233660", mdlViewerEnd);

INCLUDE_ASM(const s32, "game/code_00233660", func_002388F8);

INCLUDE_ASM(const s32, "game/code_00233660", func_00238BD8);

INCLUDE_ASM(const s32, "game/code_00233660", func_00238D38);

void mdlFreeViewResources(void) {
    MdlViewState *state = &D_00453550;
    s32 *slot = state->resources;
    s32 i;

    for (i = 0; i != 12; i++) {
        s32 handle = *slot;

        if (handle != 0) {
            *slot = 0;
            mdlDestroyContext(handle);
        }
        slot++;
    }
}

typedef struct MdlTaskDef {
    s32 name;
    s32 arg;
} MdlTaskDef;

extern MdlTaskDef D_003C8890[];

extern char D_004214E8[];

extern char D_004214F8[];

void func_00238F30(void) {
    MdlTaskDef *def;
    s32 i = 0;

    mdlFreeViewResources();
    kwlnTaskDestroyWithHierarchyByName(D_004214E8, 0);
    kwlnTaskDestroyWithHierarchyByName(D_004214F8, 0);
    def = D_003C8890;
    do {
        if (func_00101740((void *)def->name) == 0) {
            func_00103388(def->name, def->arg, 0, 0);
        }
        i++;
        def++;
    } while (i != 3);
}

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
    evtSetSolarOverlayFullyTransparent();
    evtDisableSolarPhaseAdvance();
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

extern s32 func_00112E30(s32 world, f32 *pos, f32 *rot);

extern void dds3EnsureSlotData(s32 obj);

extern void func_00110BE0(s32 world, s32 obj);

s32 func_0023A5E8(void) {
    f32 pos[4] = {0.0f, -100.0f, -600.0f, 0.0f};
    f32 rot[4];
    s32 obj;

    memset(rot, 0, 0x10);
    rot[3] = 1.0f;
    obj = func_00112E30(dds3AdvanceWorldCounter(), pos, rot);
    effObjSetInnerFloat(obj, 10.0f);
    dds3EnsureSlotData(obj);
    func_00110BE0(dds3GetWorldSecondaryObject(), obj);
    func_001129C8(obj, 0);
}

s32 func_0023A6A0(s32 arg0, s32 arg1) {
    f32 pos[4];
    f32 rot[4];
    s32 world;
    s32 obj;

    memset(pos, 0, 0x10);
    pos[3] = 1.0f;
    memset(rot, 0, 0x10);
    rot[3] = 1.0f;
    world = dds3AdvanceWorldCounter();
    obj = dds3SpawnCameraSlotObj5(world, pos, rot);
    dds3SetObjectFlags(obj, 0x20);
    effObjSetInnerFloat(obj, 10.0f);
    func_00112058(obj, arg0, arg1);
    if (dds3GetUnk0C(obj)->flags1C != 0) {
        mdlAddEntryFlagged(dds3GetUnk0C(obj), 0, 0);
    }
    func_001129C8(obj, 0);
    return world;
}

extern s32 func_00110C70(s32 world, s32 a, s32 b);

extern void dds3SetSlotByKind(s32 obj, s32 slot);

extern void dds3InvokeSlot5Handler(s32 obj);

s32 func_0023A7A0(s32 arg0, s32 arg1) {
    f32 pos[4];
    f32 rot[4];
    s32 world;
    s32 obj;

    memset(pos, 0, 0x10);
    pos[3] = 1.0f;
    memset(rot, 0, 0x10);
    rot[3] = 1.0f;
    world = dds3AdvanceWorldCounter();
    obj = dds3SpawnCameraSlotObj5(world, pos, rot);
    dds3SetObjectFlags(obj, 0x20);
    effObjSetInnerFloat(obj, 10.0f);
    func_00112058(obj, arg0, arg1);
    mdlAddEntryFlagged(dds3GetUnk0C(obj), 0, 0);
    dds3SetSlotByKind(obj, func_00110C70(dds3GetWorldSecondaryObject(), 0x10000, 2));
    dds3InvokeSlot5Handler(obj);
    func_001129C8(obj, 0);
    dds3SetObjectFlags(obj, 0x400);
    return world;
}

typedef struct MdlAimSrc {
    u8 pad00[0x18];
    u8 *vecs;  /* 0x18 */
} MdlAimSrc;

typedef struct MdlAimObj {
    u8 pad00[0x1C];
    u8 *inner; /* 0x1C */
} MdlAimObj;

extern s32 func_00110C70(s32 world, s32 id, s32 kind);

extern void effObjSetInnerFirstVec(MdlAimObj *obj, u8 *vec);

extern void effObjSetInnerSecondVec(MdlAimObj *obj, f32 *vec);

extern void effObjFetchInnerFirstVec(MdlAimObj *obj);

extern void effMiscAxisAngleToQuaternionVU(f32 angle);

extern void effMiscQuatMultiplyVU();

void func_0023A8C0(s32 firstId, s32 secondId) {
    f32 axis[4] = {0.0f, 1.0f, 0.0f, 1.0f};
    MdlAimObj *obj;
    MdlAimSrc *src;
    u8 *vec;

    obj = (MdlAimObj *)func_00110C70(dds3GetWorldSecondaryObject(), firstId, 5);
    if (obj != NULL) {
        src = (MdlAimSrc *)func_00110C70(dds3GetWorldSecondaryObject(), secondId, 0x11);
        if (src != NULL) {
            vec = src->vecs;
            effObjSetInnerFirstVec(obj, vec);
            __asm__ volatile(".set noreorder\n\tlqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(axis));
            effMiscAxisAngleToQuaternionVU(3.14159265f);
            vec += 0x10;
            __asm__ volatile(".set noreorder\n\tlqc2 $vf11, 0(%0)\n\t.set reorder" : : "r"(vec));
            effMiscQuatMultiplyVU();
            __asm__ volatile(".set noreorder\n\tsqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(axis) : "memory");
            effObjSetInnerSecondVec(obj, axis);
            effObjFetchInnerFirstVec(obj);
            __asm__ volatile(".set noreorder\n\tsqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(obj->inner + 0x70) : "memory");
        }
    }
}

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

