#include "common.h"

#include "pcp_vu0.h"

typedef struct MdlViewState {
    s32 unk00;
    s32 viewerTask;
    s8 unk08;
    s8 unk09;
    s8 unk0A;
    s8 unk0B;
    s8 unk0C;
    s8 unk0D;
    s8 unk0E;
    s8 unk0F;
    s8 unk10;
    u8 pad11[3];
    s16 unk14;
    s16 resourceCount;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
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
    s16 unk38;
    s16 nodeCursor; /* 0x3A: selection within the loaded node count */
    u8 pad3C[4];
    s16 unk40;
    s16 unk42;
    s16 unk44;
    s16 unk46;
    s16 unk48;
    s16 unk4A;
    s16 unk4C;
    s16 unk4E;
    s16 unk50;
    s16 unk52;
    f32 unk54;
    u8 pad58[0x34];
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

typedef struct MdlViewerTaskDef {
    const char *name;
    void *update;
    s32 data;
} MdlViewerTaskDef;

extern MdlViewerTaskDef D_00367A40[];

typedef struct MdlSystemTask {
    const char *name;
    s32 arg;
} MdlSystemTask;

extern MdlSystemTask D_00367AE0[3];

extern char D_003ABF78[]; /* "modelViewer" */

extern char D_003ABF88[]; /* "modelViewerEnd" */

extern void mdlViewer();

extern void mdlViewerEnd();

s32 func_0011D3E8(s32, s32, s32, s32, s32, s32, s32);

void func_0011E080(s32, s32, s32, s32, s32, s32, s32);

void func_0014FB70(s32, float);

s32 func_0014FD20(s32);

void func_00152000(s32, float, float);

s32 billCreateIndexed(s32, s32);

s32 func_0021A608(s32, s32, s32, s32, s32);

s32 func_0021A660(s16 *, s32);

void func_0021B9F8(void);

void func_0021BDD0(void);

void sdfAppendPacket(s32, s32);

void sdfStreamCreateWithParams(s32, s32, s32, s32, s32);

extern s32 kwlnTaskGetTaskByName(void *name);

extern void kwlnTaskDestroyWithHierarchy(s32 task, s32 flag);

extern s32 kwlnTaskCreate(const char *name, s32 id, s32, s32, void *update, void *destroy, s32 data);

void func_00102A18(void);

void func_00101A80(s32, s32);

void func_0021E3C0();

void func_002EDBD8(void *buffer, s32);

void func_002EDC30(void *buffer);

void func_002EDE48(void *buffer, s32, s32);

void func_00218768(s32, s32, s32, s32);

void func_002EDC50(void *buffer);


s32 mdlCountRecords(s32);

/* Assemble the resource request in a temporary buffer before loading it. */
extern void func_002CFF98();

typedef struct MdlSlotEntry {
    s32 a;
    s32 b;
    s32 c;
} MdlSlotEntry;

typedef struct MdlSlotTable {
    MdlSlotEntry *entries;
    s32 count;
} MdlSlotTable;

extern MdlSlotTable D_00365858[];

extern MdlSlotTable D_00367900[];

extern s32 D_003BBB6C;

extern s32 D_003BBB70;

extern void sdfReleaseMemorySlot(s32 *slot);

/* The package request helpers fill a 0x40-byte buffer with a handle at +0x30. */
typedef struct MdlPackageRequest {
    u8 pad00[0x30];
    s32 handle;
    u8 pad34[0xC];
} MdlPackageRequest;

void mdlLoadViewerPackage(s32 first, s32 second, s32 flags, s32 requestFirst, s32 requestSecond) {
    u8 buffer[0x40];

    func_002EDBD8(buffer, 0);
    if (flags & 2) {
        func_002EDC30(buffer);
    }
    func_002EDE48(buffer, requestFirst, requestSecond);
    func_00218768(((MdlPackageRequest *)buffer)->handle, first, second, flags);
    func_002EDC50(buffer);
}

void func_00218BE8(s32 resource) {
    func_002CFF98((void *)resource);
}

void func_00218C00(void) {
    MdlSlotEntry *entry;
    s32 count;
    s32 i;

    count = D_00365858[5].count;
    if (count > 0) {
        entry = D_00365858[5].entries;
        i = 0;
        do {
            i++;
            func_002CFF98(entry->b);
            func_002CFF98(entry->a);
            func_002CFF98(entry->c);
            entry++;
        } while (i < count);
    }
    D_00367900[5].entries = NULL;
    D_00367900[5].count = 0;
    D_00365858[5].entries = NULL;
    D_00365858[5].count = 0;
    sdfReleaseMemorySlot(&D_003BBB6C);
    sdfReleaseMemorySlot(&D_003BBB70);
}

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

/* The viewer resource holds a pointer to the table container at +0x0C. */
typedef struct MdlViewerData {
    u8 pad00[0xA4];
    s32 *records;  /* 0xA4: relative-linked model records */
    s32 slotTable; /* 0xA8: indexable object slots */
} MdlViewerData;

typedef struct MdlViewerResource {
    u8 pad00[0x0C];
    MdlViewerData *data;
} MdlViewerResource;

typedef struct MdlViewerSlots {
    u8 pad00[4];
    s16 count; /* 0x04 */
    u8 pad06[6];
    s32 first; /* 0x0C: base of 0x10-byte slot entries */
} MdlViewerSlots;

/* Read the model record's payload word without advancing its relative link. */
u32 func_002192C0(MdlRecord *record) {
    return record->value08;
}

u16 func_002192C8(MdlRecord *record) {
    return record->value0C;
}

/* Follow the relative links in a resource's record table to find an ID. */
s32 *mdlFindViewerRecord(MdlViewerResource *resource, s32 id) {
    s32 *table = resource->data->records;
    s32 *record;
    s32 remaining;

    if (table == NULL) {
        return NULL;
    }
    record = table;
    remaining = func_002192C8((MdlRecord *)record);
    do {
        remaining--;
        record = (s32 *)((s32)record + ((MdlRecord *)record)->nextOffset);
        if (remaining == -1) {
            return NULL;
        }
    } while (*record != id);
    return record;
}

s32 * mdlGetFirstRecord(s32 address) {
    s32 *record = (s32 *)(address + 8);

    if (((MdlRecord *)record)->kind == 0xffff) {
        record = NULL;
    }
    return record;
}

s32 * mdlGetNextRecord(MdlRecord *current) {
    s32 *record = (s32 *)((s32)current + current->nextOffset);

    if (((MdlRecord *)record)->kind == 0xffff) {
        record = NULL;
    }
    return record;
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

/* Draw labeled marker fields and each of four packed color channels. */
void mdlDrawMarkParamsPanel(s32 list, s32 x, s32 y, s32 z, EffMarkParams *params, s32 selected) {
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

#define MDL_PART_OBJECT 3

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
    entry->object = func_0014FD20(index);
    list->count += 1;
}

extern void *func_002CFF68(s32 size);

typedef struct MdlHandlerNode {
    s32 a;      /* 0x00 */
    void *b;    /* 0x04 */
    u8 pad08[8];
    s32 c;      /* 0x10 */
    u8 pad14[0x98];
} MdlHandlerNode;

void func_00219BB0(MdlPartList *list, s32 a, void *b, s32 c) {
    MdlHandlerNode *node = func_002CFF68(0xAC);
    MdlPartEntry *entry = &list->entries[list->count];

    node->c = c;
    node->a = a;
    node->b = b;
    entry->kind = MDL_PART_OBJECT;
    entry->state = 0;
    entry->object = (s32)node;
    list->count += 1;
}

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

void mdlObjDestroy(MdlObj *obj) {
    if (obj->initialized != 0) {
        func_002EBB60(obj->data);
    }
    func_002D0918(obj->handle);
    func_002CFF98(obj);
}

void mdlObjInit(MdlObj *obj, s32 data, s32 attributes) {
    if (obj->initialized == 0) {
        obj->initialized = 1;
        sdfStreamCreateWithParams((s32)obj->data, attributes, obj->unk0, obj->unk10, data);
    }
}

void func_00219CC8(u32 request) {
    sdfDevCreateBufferedRequest(request, 0x10, 4);
}

void func_00219CE8(MdlPartList *list) {
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

typedef struct MdlResourceItem {
    struct MdlResourceItem *next; /* 0x00 */
    u16 type;                     /* 0x04: billboard / effect kind */
    s16 subtype;                  /* 0x06 */
    s32 resource;                 /* 0x08 */
    u8 pad0C[0x14];
} MdlResourceItem;

/* A resource owner's counter source exposes the current scalar at +0x1C. */
typedef struct MdlCounterSource {
    u8 pad00[0x1C];
    f32 value;
} MdlCounterSource;

typedef struct {
    u8 pad00[0x14];
    MdlResourceItem *first; /* 0x14 */
    void *chunk;            /* 0x18 */
    MdlCounterSource *counterSource; /* 0x1C */
} MdlResourceOwner;

MdlResourceItem *mdlInsertResourceItem(MdlResourceOwner *object, s32 type, s32 subtype) {
    MdlResourceItem *item = func_002CFF68(0x20);
    MdlResourceItem *previous = object->first;
    item->type = type;
    item->next = previous;
    item->subtype = subtype;
    object->first = item;
    return item;
}

void mdlAdvanceBillboardPart(MdlPartEntry *entry) {
    func_00151E60((u32)entry->object);
    entry->state = entry->state + 1;
}

void mdlAdvanceEffectPart(MdlPartEntry *entry) {
    func_0014FEB0((u32)entry->object);
    entry->state = entry->state + 1;
}

s32 func_00219EA0(MdlViewerResource *resource, s32 index) {
    s32 slotTable;

    slotTable = resource->data->slotTable;
    if (slotTable == 0) {
        return 0;
    }
    if (index >= ((MdlViewerSlots *)slotTable)->count) {
        return 0;
    }
    return ((MdlViewerSlots *)slotTable)->first + index * 0x10;
}

typedef struct MdlPartRec {
    u8 pad00[4];
    u32 size;       /* 0x04 */
    s32 firstId;    /* 0x08 */
    u16 count;      /* 0x0C */
    u16 partIndex;  /* 0x0E */
    f32 value;      /* 0x10 */
} MdlPartRec;

typedef struct MdlPartItem {
    u8 pad00[8];
    s32 handle;          /* 0x08 */
    MdlPartEntry *part;  /* 0x0C */
    void *record;        /* 0x10 */
    f32 value;           /* 0x14 */
} MdlPartItem;

extern void *sdfChunkFindRecordById(void *chunk, s32 id);

void func_00219ED8(MdlResourceOwner *owner, MdlPartRec *rec, s32 option, s32 type, s32 (*create)(MdlPartEntry *)) {
    MdlPartEntry *part = (MdlPartEntry *)func_00219EA0((MdlViewerResource *)owner, rec->partIndex);

    if (part != NULL) {
        void *chunk = owner->chunk;
        s32 id = rec->firstId;
        s32 count = rec->count;
        f32 value = 0.0f;

        if (rec->size >= 0x11) {
            value = rec->value;
        }
        do {
            void *record = sdfChunkFindRecordById(chunk, id++);

            if (record != NULL) {
                MdlPartItem *item = (MdlPartItem *)mdlInsertResourceItem(owner, type, option);

                item->handle = create(part);
                item->part = part;
                item->record = record;
                item->value = value;
            }
        } while (--count != 0);
    }
}

typedef struct MdlEffectRec {
    u8 pad00[8];
    s32 effectId;    /* 0x08 */
    s32 param0C;     /* 0x0C */
    u16 scaleX;      /* 0x10 */
    u16 scaleY;      /* 0x12 */
    u16 value14;     /* 0x14 */
    u8 value16;      /* 0x16 */
    u8 value17;      /* 0x17 */
    s32 value18;     /* 0x18 */
    s32 value1C;     /* 0x1C */
    s32 value20;     /* 0x20 */
    s32 value24;     /* 0x24 */
} MdlEffectRec;

typedef struct MdlEffectParams {
    MdlResourceOwner *owner; /* 0x00 */
    s32 effectId;            /* 0x04 */
    s32 param0C;             /* 0x08 */
    f32 scaleX;              /* 0x0C */
    f32 scaleY;              /* 0x10 */
    s32 value14;             /* 0x14 */
    s32 value16;             /* 0x18 */
    s32 mode;                /* 0x1C */
    s32 value17;             /* 0x20 */
    s32 value18;             /* 0x24 */
    s32 value1C;             /* 0x28 */
    s32 value20;             /* 0x2C */
    s32 value24;             /* 0x30 */
} MdlEffectParams;

extern s32 func_00188150(MdlEffectParams *params);

void func_00219FD8(MdlResourceOwner *owner, MdlEffectRec *rec, s32 option) {
    MdlEffectParams params;
    MdlResourceItem *item;

    params.owner = owner;
    params.effectId = rec->effectId;
    params.param0C = rec->param0C;
    params.scaleX = rec->scaleX;
    params.scaleY = rec->scaleY;
    params.value14 = rec->value14;
    params.value16 = rec->value16;
    params.mode = 3;
    params.value17 = rec->value17;
    params.value18 = rec->value18;
    params.value1C = rec->value1C;
    params.value20 = rec->value20;
    params.value24 = rec->value24;
    item = mdlInsertResourceItem(owner, 2, option);
    item->resource = func_00188150(&params);
}

/* Kind-four model record: two selectors and two 32-bit stream parameters. */
typedef struct MdlStreamRecord {
    s32 kind;
    u32 size;
    u16 selectorA; /* 0x08 */
    u16 selectorB; /* 0x0A */
    u32 value0C;
    u32 value10;
} MdlStreamRecord;

void func_0021A088(u32 owner, s32 record) {
    func_00217310(owner, ((MdlStreamRecord *)record)->selectorA,
                  ((MdlStreamRecord *)record)->selectorB,
                  ((MdlStreamRecord *)record)->value0C,
                  ((MdlStreamRecord *)record)->value10);
}

typedef struct MdlObjItem {
    u8 pad00[8];
    MdlResourceOwner *owner; /* 0x08 */
    s32 obj;                 /* 0x0C */
    s32 data;                /* 0x10 */
    s32 param;               /* 0x14 */
    u8 attr[8];              /* 0x18 */
} MdlObjItem;

typedef struct MdlEntryRec {
    u8 pad00[8];
    s32 dataId;   /* 0x08 */
    s32 param;    /* 0x0C */
    u8 flagB;     /* 0x10 */
    u8 flagA;     /* 0x11 */
    u16 index;    /* 0x12 */
} MdlEntryRec;

typedef struct MdlSlotRec {
    u8 pad00[8];
    MdlObj *obj; /* 0x08 */
} MdlSlotRec;

extern void *sdfFindResourceById(s32 id);

void func_0021A0B0(MdlResourceOwner *owner, MdlEntryRec *entry, s32 option) {
    MdlSlotRec *slot = (MdlSlotRec *)func_00219EA0((MdlViewerResource *)owner, entry->index);

    if (slot != 0) {
        MdlObj *obj = slot->obj;
        if (obj->inUse == 0) {
            s32 data = (s32)sdfFindResourceById(entry->dataId);
            if (data != 0) {
                MdlObjItem *item;
                u8 *attr;
                obj->inUse = 1;
                item = (MdlObjItem *)mdlInsertResourceItem(owner, 3, option);
                item->owner = owner;
                item->obj = (s32)obj;
                attr = item->attr;
                item->data = data;
                item->param = entry->param;
                attr[1] = 1;
                attr[2] = entry->flagA;
                attr[3] = entry->flagB;
            }
        }
    }
}

void mdlCondInitEntry(s32 entry) {
    s32 object = ((MdlObjItem *)entry)->obj;
    if (((MdlObj *)object)->initialized == 0) {
        s32 count = ((MdlObjItem *)entry)->param;
        f32 counterValue = ((MdlObjItem *)entry)->owner->counterSource->value;
        if ((u32)(s32)counterValue < (u32)count) {
            return;
        }
        mdlObjInit(object, ((MdlObjItem *)entry)->data, entry + 0x18);
    }
}

typedef struct MdlRecordHeader {
    s32 kind; /* 0x00 */
} MdlRecordHeader;

void mdlDispatchResourceEntry(MdlResourceOwner *owner, MdlRecordHeader *record, s32 option) {
    switch (record->kind) {
    case 1:
        func_00219ED8(owner, (MdlPartRec *)record, option, 0, mdlAdvanceBillboardPart);
        return;
    case 2:
        func_00219ED8(owner, (MdlPartRec *)record, option, 1, mdlAdvanceEffectPart);
        return;
    case 3:
        func_00219FD8(owner, (MdlEffectRec *)record, option);
        return;
    case 4:
        func_0021A088((u32)owner, (s32)record);
        return;
    case 5:
        func_0021A0B0(owner, (MdlEntryRec *)record, option);
        break;
    }
}

void mdlApplyResourceEntries(s32 object, s32 id, s32 option) {
    s32 *block = mdlFindViewerRecord((MdlViewerResource *)object, id);
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

extern u8 D_00324690[];

extern u8 *func_002D7D68(void *chunk, s32 id);

/* vu0 routine: out = p + normalize(p - D_00324690) * scale, p = node position transformed by the node matrix */
void func_0021A3D8(void *chunk, MdlAnchorRec *rec, f32 *out) {
    MdlNodeInfo *info = rec->info;
    u8 *matrix = func_002D7D68(chunk, info->id);
    f32 scale = rec->scale;

    __asm__ volatile(".set noreorder\n\tlqc2 vf28, 0(%0)\n\tlqc2 vf29, 0x10(%0)\n\tlqc2 vf30, 0x20(%0)\n\tlqc2 vf31, 0x30(%0)\n\t.set reorder" : : "r"(matrix + 0xC0));
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(info->pos));
    __asm__ volatile(".set noreorder\n\tvmulax.xyzw ACC, vf28, vf10x\n\tvmadday.xyzw ACC, vf29, vf10y\n\tvmaddaz.xyzw ACC, vf30, vf10z\n\tvmaddw.xyzw vf10, vf31, vf0w\n\tvmove.xyzw vf11, vf10\n\t.set reorder");
    __asm__ volatile(".set noreorder\n\tlqc2 vf12, 0(%0)\n\t.set reorder" : : "r"(D_00324690));
    __asm__ volatile(".set noreorder\n\tvsub.xyzw vf10, vf10, vf12\n\tvmul.xyz vf2, vf10, vf10\n\tvmulax.w ACC, vf0, vf2x\n\tvmadday.w ACC, vf0, vf2y\n\tvmaddz.w vf2, vf0, vf2z\n\tvrsqrt Q, vf0w, vf2w\n\tvwaitq\n\tvmulq.xyz vf10, vf10, Q\n\t.set reorder");
    __asm__ volatile(".set noreorder\n\tqmtc2.ni %0, vf2\n\t.set reorder" : : "r"(scale));
    __asm__ volatile(".set noreorder\n\tvmulx.xyzw vf10, vf10, vf2x\n\tvadd.xyzw vf10, vf10, vf11\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(out));
}

extern void effCopyVector(s32 handle, f32 *src);

extern void billInvokeCallback(s32 handle);

extern void func_0014FBF0(s32 handle, f32 *pos);

extern void effUpdateNode(s32 handle);

extern void func_00188318(s32 handle);

void func_0021A490(MdlResourceOwner *owner, MdlAnchorRec *rec) {
    void *chunk = owner->chunk;
    f32 position[4];
    s32 handle;

    switch (rec->type) {
    case 0:
        func_0021A3D8(chunk, rec, position);
        handle = rec->handle;
        effCopyVector(handle, position);
        billInvokeCallback(handle);
        break;
    case 1:
        func_0021A3D8(chunk, rec, position);
        handle = rec->handle;
        func_0014FBF0(handle, position);
        effUpdateNode(handle);
        break;
    case 2:
        func_00188318(rec->handle);
        break;
    case 3:
        mdlCondInitEntry((s32)rec);
        break;
    }
}

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

s32 func_0021A608(s32 x, s32 y, s32 depth, s32 width, s32 height) {
    return func_0011D3E8(x, y, depth, width, height, 0x30000000, 0x60404040);
}

void func_0021A628(s32 x, s32 y, s32 depth, s32 width, s32 height, s32 unused) {
    s32 packet;

    packet = D_003D7B10[0];
    sdfAppendPacket(packet, func_0021A608(x, y, depth, width, height));
}

extern s8 D_00398628[];

s32 func_0021A660(s16 *cursor, s32 count) {
    s32 max = count - 1;
    s32 v = *cursor;
    s32 changed = 0;
    if (D_00398628[5] < 0) {
        if (v < max) {
            v += 1;
            changed = 1;
        } else {
            v = 0;
            changed = 1;
        }
    } else if (((u8)D_00398628[5] & 2) != 0) {
        if (v < max) {
            v += 1;
            changed = 1;
        }
    } else if (D_00398628[4] < 0) {
        if (v > 0) {
            v -= 1;
            changed = 1;
        } else {
            v = max;
            changed = 1;
        }
    } else if ((((u8)D_00398628[4] & 2) != 0) && (v > 0)) {
        v -= 1;
        changed = 1;
    }
    if (changed != 0) {
        *cursor = v;
        D_003D7A50.unk09 = 0;
        return 1;
    }
    return 0;
}

s32 func_0021A718(s16 *cursor, s32 count, s32 step) {
    s32 max = count - 1;
    s32 v = *cursor;
    s32 changed = 0;
    if (D_00398628[7] < 0) {
        if (v < max) {
            v += 1;
        } else {
            v = 0;
        }
        changed = 1;
    } else if (((u8)D_00398628[7] & 2) != 0) {
        if (v < max) {
            v += 1;
            changed = 1;
        }
    } else if (D_00398628[6] < 0) {
        if (v > 0) {
            v -= 1;
        } else {
            v = max;
        }
        changed = 1;
    } else if (((u8)D_00398628[6] & 2) != 0) {
        if (v > 0) {
            v -= 1;
            changed = 1;
        }
    } else if (step != 0) {
        if (D_00398628[11] < 0) {
            if (v < max) {
                v += step;
                if (v > max) {
                    v = max;
                }
            } else {
                v = 0;
            }
            changed = 1;
        } else if (((u8)D_00398628[11] & 2) != 0) {
            if (v < max) {
                v += step;
                if (v > max) {
                    v = max;
                }
                changed = 1;
            }
        } else if (D_00398628[10] < 0) {
            if (v > 0) {
                v -= step;
                if (v < 0) {
                    v = 0;
                }
            } else {
                v = max;
            }
            changed = 1;
        } else if ((((u8)D_00398628[10] & 2) != 0) && (v > 0)) {
            v -= step;
            if (v < 0) {
                v = 0;
            }
            changed = 1;
        }
    }
    if (changed != 0) {
        *cursor = v;
        D_003D7A50.unk09 = 0;
        return 1;
    }
    return 0;
}

typedef struct MdlResource {
    u8 pad00[0x18];
    u8 *chunk;      /* 0x18 */
    s32 hasEntries; /* 0x1C */
} MdlResource;

extern MdlResource *func_00217680(s16, s16);

extern void mdlAddEntryFlagged(void *, s32, s32);

void func_0021A880(void) {
    MdlResource *resource = func_00217680(D_003D7A50.unk18, D_003D7A50.unk1A);

    /* Required to match: storing through a typed pointer preserves the load/store order. */
    *(MdlResource **)&D_003D7A50.resources[0] = resource;
    D_003D7A50.unk22 = 0;
    D_003D7A50.unk24 = 0;
    D_003D7A50.unk26 = 0;
    D_003D7A50.unk28 = 0;
    D_003D7A50.unk2A = 0;
    D_003D7A50.unk2C = 0;
    if (resource->hasEntries != 0) {
        mdlAddEntryFlagged(resource, 0, 0);
    }
    D_003D7A50.nodeCursor = 0;
}

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

/* Rotate the viewer resource list in place without moving its allocation. */
void mdlRotateViewResourcesRight(void) {
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

extern void mdlAddEntryFlaggedEx(s32 resource, s32 id, s32, f32, f32);

extern void mdlAddEntryPlainEx(s32 resource, s32 id, s32, f32, f32);

void mdlAddViewEntryFlagged(void) {
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

extern s8 D_00324510[];

/* Change the viewer scale in hundredths, with larger steps at larger values. */
void mdlAdjustViewerScale(void) {
    s32 value = (s32)(D_003D7A50.unk54 * 100.0f + 0.5f);

    if (D_00324510[0x27] & 2) {
        if (value < 0x32) {
            value += 1;
        } else if (value < 0x1F4) {
            value += 10;
        } else {
            value += 100;
            if (value >= 0x7D1) {
                value = 0x7D0;
            }
        }
        D_003D7A50.unk54 = value * 0.01f;
    } else if (D_00324510[0x26] & 2) {
        if (value < 0x33) {
            value -= 1;
            if (value < 5) {
                value = 5;
            }
        } else if (value < 0x1F5) {
            value -= 10;
        } else {
            value -= 100;
        }
        D_003D7A50.unk54 = value * 0.01f;
    }
    if (D_00324510[0x23] < 0) {
        D_003D7A50.unk0C ^= 1;
    }
    D_003D7A50.unk0D = 0;
    if (D_003D7A50.unk0C != 0) {
        if (D_00324510[0x25] != 0) {
            D_003D7A50.unk0D = 1;
        } else if (D_00324510[0x24] != 0) {
            D_003D7A50.unk0D = -1;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021BFB0);

u32 func_0021C1E0(void) {
    mdlAdjustViewerScale();
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

/* Loaded-resource count chain, identical to the DDS2 model viewer. */
typedef struct MdlCountNode {
    u8 pad00[4];
    s16 count;
} MdlCountNode;

typedef struct MdlLoadedInfo {
    u8 pad00[8];
    MdlCountNode *first;
} MdlLoadedInfo;

typedef struct MdlLoaded {
    u8 pad00[0x18];
    MdlLoadedInfo *info;
} MdlLoaded;

void func_0021C6A0(void) {
    MdlCountNode *firstNode = ((MdlLoaded *)D_003D7A50.resources[0])->info->first;

    if (firstNode != 0) {
        s16 nodeCount = firstNode->count;

        if (nodeCount > 0) {
            func_0021A660(&D_003D7A50.nodeCursor, nodeCount);
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
    MdlViewerResource *resource = (MdlViewerResource *)D_003D7A50.resources[0];
    s32 first = mdlCountRecords((s32)mdlFindViewerRecord(resource, -1));
    s32 second = mdlCountRecords((s32)mdlFindViewerRecord(resource, D_003D7A50.unk22));

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

s32 func_0021D590(void) {
    if (D_003D7A50.unk08 == 0) {
        D_003D7A50.unk08 = 1;
        func_0021E3C0();
        func_00102A18();
    }
    return 0;
}

typedef struct MdlDrawDevice {
    u8 pad00[0x10];
    void (*submit)(struct MdlDrawDevice *device, s32 list); /* 0x10 */
} MdlDrawDevice;

extern MdlDrawDevice D_00325048;

extern u8 D_00367B80[];

extern u8 D_00367BE0[];

extern s32 sdfCreateResetPacketList(void);

extern s32 func_002EF2E0(void *, void *, s32, s32);

void func_0021D5D0(void) {
    s32 list;

    if (D_003D7A50.unk0A < 4) {
        if (D_003D7A50.unk0A >= 2) {
            list = sdfCreateResetPacketList();
            __asm__ volatile(".set noreorder\n\tvsub.xyzw vf28, vf0, vf0\n\tvmr32.xyzw vf30, vf0\n\tvmove.xyzw vf31, vf0\n\tvaddw.x vf28, vf28, vf0w\n\tvmr32.xyzw vf29, vf30\n\t.set reorder");
            sdfAppendPacket(list, func_002EF2E0(D_00367B80, D_00367BE0, 6, 0x80));
            D_00325048.submit(&D_00325048, list);
        }
    }
}

extern u8 D_00367C00[];

extern u8 D_00367C40[];

extern s32 D_003BBCB8;

extern void func_002EA2E0(s32 packet, s32 arg);

void func_0021D668(void) {
    s32 list;
    s32 packet;
    MdlResource *resource;

    if (D_003D7A50.unk0A == 3) {
        list = sdfCreateResetPacketList();
        resource = (MdlResource *)D_003D7A50.resources[0];
        __asm__ volatile(".set noreorder\n\tlqc2 vf28, 0(%0)\n\tlqc2 vf29, 0x10(%0)\n\tlqc2 vf30, 0x20(%0)\n\tlqc2 vf31, 0x30(%0)\n\t.set reorder" : : "r"(resource->chunk + 0x20));
        packet = func_002EF2E0(D_00367C00, D_00367C40, 4, 0x80);
        if ((D_00398628[13] < 0) & (D_003BBCB8 == 0)) {
            D_003BBCB8 = 1;
            func_002EA2E0(packet, 0x100);
        }
        sdfAppendPacket(list, packet);
        D_00325048.submit(&D_00325048, list);
    }
}

void mdlViewerTaskDestroy(void) {
    if (D_003D7A50.viewerTask != 0) {
        kwlnTaskDestroyWithHierarchy(D_003D7A50.viewerTask, 0);
        D_003D7A50.viewerTask = 0;
    }
}

void func_0021D780(void) {
    mdlViewerTaskDestroy();
    D_003D7A50.viewerTask = kwlnTaskCreate(D_00367A40[D_003D7A50.unk0A - 1].name, 0x2B00, 1, 0, D_00367A40[D_003D7A50.unk0A - 1].update, 0, D_00367A40[D_003D7A50.unk0A - 1].data);
    func_00101A80(D_003D7A50.unk00, D_003D7A50.viewerTask);
}

INCLUDE_ASM(const s32, "game/code_00218B48", mdlViewer);

extern u8 D_00367C50[];

extern u8 D_00367CB0[];

extern s32 sdfCountMapPositionRecords(void *chunk);

extern u8 *sdfChunkFindByTag(void *chunk, s32 tag);

extern void sdfSetLookAtBasisFromRecord(void *chunk, u8 *record);

void func_0021DAE0(MdlResource *resource) {
    s32 count = sdfCountMapPositionRecords(resource->chunk);
    s32 i;
    s32 list;
    u8 *record;

    if (count > 0) {
        list = sdfCreateResetPacketList();
        record = sdfChunkFindByTag(resource->chunk, 0x534F504D) + 0x10;
        for (i = 0; i != count; i++) {
            sdfSetLookAtBasisFromRecord(resource->chunk, record);
            record += 0x40;
            sdfAppendPacket(list, func_002EF2E0(D_00367C50, D_00367CB0, 6, 0x80));
        }
        D_00325048.submit(&D_00325048, list);
    }
}

INCLUDE_ASM(const s32, "game/code_00218B48", mdlViewerEnd);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021DD88);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021E068);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021E1C8);

extern void mdlDestroyContext(s32 resource);

void mdlFreeViewResources(void) {
    s32 i;
    for (i = 0; i != 12; i++) {
        s32 handle = D_003D7A50.resources[i];
        if (handle != 0) {
            D_003D7A50.resources[i] = 0;
            mdlDestroyContext(handle);
        }
    }
}

extern s32 kwlnTaskDestroyWithHierarchyByName(const char *name, s32 flag);

extern void func_00103498(const char *name, s32, s32, s32);

void func_0021E3C0(MdlViewState *view) {
    s32 i;

    mdlFreeViewResources();
    kwlnTaskDestroyWithHierarchyByName(D_003ABF78, 0);
    kwlnTaskDestroyWithHierarchyByName(D_003ABF88, 0);
    for (i = 0; i != 3; i++) {
        if (kwlnTaskGetTaskByName((void *)D_00367AE0[i].name) == 0) {
            func_00103498(D_00367AE0[i].name, D_00367AE0[i].arg, 0, 0);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021E450);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021E618);

void func_0021EB10(s32 first, s32 second, s32 color, s32 variant) {
    s32 packedColor = color & 0xffffff;

    func_0011E080(D_003D7B50.unk08, first, second,
                  (D_003D7B50.unk04 == 0) ? -1 : variant, packedColor | 0x80000000, 1, packedColor);
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021EB60);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021ECF0);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021F098);

void func_0021F4B8(void) {
    mdlFlagClearAll();
    evtSetSolarPhase(0);
    evtSetSolarOverlayFullyTransparent();
    evtDisableSolarPhaseAdvance();
}

/* Model flag words are stored directly in the global work area at +0x840. */
typedef struct MdlFlagBank {
    u8 pad00[0x840];
    u32 words[0x80];
} MdlFlagBank;

void mdlFlagClearAll(void) {
    s32 i = 0x7f;
    u32 *word = ((MdlFlagBank *)D_003BAA00)->words;

    do {
        i -= 1;
        *word = 0;
        word += 1;
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
    ((MdlFlagBank *)D_003BAA00)->words[adjustedFlag >> 5] |= 1 << flag;
}

void mdlFlagClear(s32 flag) {
    s32 adjustedFlag = (flag < 0) ? flag + 0x1f : flag;
    ((MdlFlagBank *)D_003BAA00)->words[adjustedFlag >> 5] &= ~(1 << flag);
}

s32 mdlFlagTest(s32 flag) {
    s32 adjustedFlag = (flag < 0) ? flag + 0x1f : flag;
    return (((s32)((MdlFlagBank *)D_003BAA00)->words[adjustedFlag >> 5] >> flag) & 1);
}

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABF78);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABF88);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABF98);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABFA8);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021F630);

extern s32 dds3AdvanceWorldCounter(void);

extern s32 func_00112C08(s32 counter, f32 *position, f32 *rotation);

extern void effObjSetInnerFloat(s32 object, f32 value);

extern void dds3EnsureSlotData();

extern s32 dds3GetWorldSecondaryObject(void);

extern void func_001109B8(s32 world, s32 object);

extern void func_001127A0(s32 object, s32 arg);

extern void *memset(void *dst, s32 value, u32 size);

void func_0021FA78(void) {
    f32 position[4] = { 0.0f, -100.0f, -600.0f, 0.0f };
    f32 rotation[4];
    s32 object;

    memset(rotation, 0, 0x10);
    rotation[3] = 1.0f;
    object = func_00112C08(dds3AdvanceWorldCounter(), position, rotation);
    effObjSetInnerFloat(object, 10.0f);
    dds3EnsureSlotData(object);
    func_001109B8(dds3GetWorldSecondaryObject(), object);
    func_001127A0(object, 0);
}

extern void func_00111E30(s32 object, s32, s32);

extern s32 dds3SpawnCameraSlotObj5(s32 counter, f32 *position, f32 *rotation);

extern void dds3SetObjectFlags(s32 object, s32 flags);

extern s32 *dds3GetUnk0C(s32 object);

s32 func_0021FB30(s32 slotKind, s32 resource) {
    f32 position[4];
    f32 rotation[4];
    s32 counter;
    s32 object;

    memset(position, 0, 0x10);
    position[3] = 1.0f;
    memset(rotation, 0, 0x10);
    rotation[3] = 1.0f;
    counter = dds3AdvanceWorldCounter();
    object = dds3SpawnCameraSlotObj5(counter, position, rotation);
    dds3SetObjectFlags(object, 0x20);
    effObjSetInnerFloat(object, 10.0f);
    func_00111E30(object, slotKind, resource);
    if (dds3GetUnk0C(object)[7] != 0) {
        mdlAddEntryFlagged(dds3GetUnk0C(object), 0, 0);
    }
    func_001127A0(object, 0);
    return counter;
}

extern void *func_00110A48(s32 world, s32 id, s32 kind);

extern void dds3SetSlotByKind(s32 object, s32 slot);

extern void dds3InvokeSlot5Handler(s32 object);

s32 func_0021FC30(s32 slotKind, s32 resource) {
    f32 position[4];
    f32 rotation[4];
    s32 counter;
    s32 object;

    memset(position, 0, 0x10);
    position[3] = 1.0f;
    memset(rotation, 0, 0x10);
    rotation[3] = 1.0f;
    counter = dds3AdvanceWorldCounter();
    object = dds3SpawnCameraSlotObj5(counter, position, rotation);
    dds3SetObjectFlags(object, 0x20);
    effObjSetInnerFloat(object, 10.0f);
    func_00111E30(object, slotKind, resource);
    mdlAddEntryFlagged(dds3GetUnk0C(object), 0, 0);
    dds3SetSlotByKind(object, (s32)func_00110A48(dds3GetWorldSecondaryObject(), 0x10000, 2));
    dds3InvokeSlot5Handler(object);
    func_001127A0(object, 0);
    dds3SetObjectFlags(object, 0x400);
    return counter;
}

typedef struct MdlAttachSlot {
    u8 pad00[0x18];
    u8 *firstVec; /* 0x18 */
    u8 pad1C[4];
} MdlAttachSlot;

typedef struct MdlAttachObj {
    u8 pad00[0x1C];
    u8 *inner; /* 0x1C */
} MdlAttachObj;

extern void effObjSetInnerFirstVec();

extern void effObjSetInnerSecondVec();

extern void effObjFetchInnerFirstVec();

extern void effMiscAxisAngleToQuaternionVU(f32 angle);

extern void effMiscQuatMultiplyVU(void);

void func_0021FD50(s32 targetId, s32 sourceId) {
    f32 quaternion[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
    MdlAttachObj *target;
    MdlAttachSlot *source;
    u8 *base;

    target = (MdlAttachObj *)func_00110A48(dds3GetWorldSecondaryObject(), targetId, 5);
    if (target != NULL) {
        source = (MdlAttachSlot *)func_00110A48(dds3GetWorldSecondaryObject(), sourceId, 0x11);
        if (source != NULL) {
            base = source->firstVec;
            effObjSetInnerFirstVec(target, base);
            __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(quaternion));
            effMiscAxisAngleToQuaternionVU(3.14159265f);
            base += 0x10;
            __asm__ volatile(".set noreorder\n\tlqc2 vf11, 0(%0)\n\t.set reorder" : : "r"(base));
            effMiscQuatMultiplyVU();
            __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(quaternion));
            effObjSetInnerSecondVec(target, quaternion);
            effObjFetchInnerFirstVec(target);
            __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(target->inner + 0x70));
        }
    }
}

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

