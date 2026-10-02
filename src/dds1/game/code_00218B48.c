#include "common.h"

#include "pcp_vu0.h"
#include "ee_mmi.h"

/* Viewer-wide state for the model viewer task (DDS1 game/code_00218B48 and
 * DDS2 game/code_00233660 share this layout field for field). Fields that are
 * only written by a defaults initialiser and never read in either game are
 * left as unkNN on purpose: there is no read to earn a role from. */
typedef struct MdlViewState {
    s32 unk00;
    s32 viewerTask;
    s8 unk08;
    s8 unk09;
    s8 unk0A;
    s8 unk0B;
    s8 unitStepMode;  /* 0x0C: toggled by the step button (D_003D24510[0x23]) */
    s8 unitStepSign;  /* 0x0D: +1/-1, derived from the input keys at 0x24/0x25 */
    u8 unk0E;
    s8 unk0F;
    s8 yawStepMode; /* 0x10: one yaw step per left/right button press */
    u8 pad11[3];
    s16 unk14;
    s16 resourceCount;
    s16 unk18; /* 0x18: first s16 handed to func_00217680, and set from
                 mdlGetContextResourceGroup's return */
    s16 unk1A; /* 0x1A: second s16, and set from mdlGetContextResourceId */
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 activeEntryId;    /* 0x22: assigned from selectedEntryId when adding */
    s16 selectedEntryId;  /* 0x24: key passed to the mdlAddEntry* family. DDS2
                           declares this u16; kept s16 here to match retail's
                           sign-extension into mdlAddEntryFlaggedEx. */
    s16 selectedNodeId;   /* 0x26: checked by mdlHasNode before adding */
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    s16 entryHeight;      /* 0x2E: low bound offered to mdlAddEntry* */
    s16 entryWidth;       /* 0x30: high bound, limited to entryHeight when drawing */
    u8 pad32[2];
    s16 labelIndexA;      /* 0x34: indexes D_00367B10 */
    s16 labelIndexB;      /* 0x36: indexes D_00367B18 */
    s16 scrollPage;  /* 0x38: page stepped by mdlUpdateViewerCursorWithPageStep */
    s16 nodeCursor;  /* 0x3A: selection within the loaded node count */
    s16 unk3C;
    s16 unk3E;
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
    f32 viewerScale;      /* 0x54: viewer zoom, in hundredths */
    u8 pad58[0x34];
    s32 slotBeforeResources[1]; /* One element before resources[] for rightward rotation. */
    s32 resources[1];
} MdlViewState;

typedef struct MdlPadState {
    u8 pad00[4];
    s8 stepDownA;  /* 0x04 */
    s8 stepUpA;    /* 0x05 */
    s8 stepDownB;  /* 0x06 */
    s8 stepUpB;    /* 0x07 */
} MdlPadState;

typedef struct MdlCtrlState {
    MdlPadState *pad;
    u8 unk04;
    u8 pad05[3];
    s32 unk08;
} MdlCtrlState;

extern MdlViewState mdlViewerState;

extern MdlCtrlState mdlViewerControlState;

extern s32 datGameState;

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

extern char D_003BBC80[]; /* "%5.2f" */

extern void mdlViewer();

extern void mdlViewerEnd();

s32 func_0011D3E8(s32, s32, s32, s32, s32, s32, s32);

void func_0011E080(s32, s32, s32, s32, s32, s32, s32);

void effApplyNodeScale(s32, float);

s32 effCreateNodeFromDescriptor(s32);

void billSetChildScaleComponents(s32, float, float);

s32 billCreateIndexed(s32, s32);

s32 mdlBuildViewerRectanglePacket(s32, s32, s32, s32, s32);

s32 mdlUpdateViewerCursor(s16 *, s32);

void func_0021B9F8(void);

void mdlDrawViewerSelectionLabel(void);

void sdfAppendPacket(s32, s32);

void sdfStreamCreateWithParams(s32, s32, s32, s32, s32);

extern s32 kwlnTaskGetTaskByName(void *name);

extern void kwlnTaskDestroyWithHierarchy(s32 task, s32 flag);

extern s32 kwlnTaskCreate(const char *name, s32 id, s32, s32, void *update, void *destroy, s32 data);

void dds3AdminSetControlFlag(void);

void func_00101A80(s32, s32);

void mdlCleanupViewerTasksAndResources();

void sdfPacInitializeDispatchPacket(void *buffer, s32);

void func_002EDC30(void *buffer);

void func_002EDE48(void *buffer, s32, s32);

void func_00218768(s32, s32, s32, s32);

void func_002EDC50(void *buffer);


s32 mdlCountRecords(s32);

extern u32 D_003BA8EC;

extern void kwlnDebugGraphSetEnabled(s8 mode);

extern s32 fldStepColorChannelByPad(u32 *color, s32 channel, s8 *pad);

extern void fldAdjustIntegerUsingMainPad(void *ptr, s32 type, s64 min, s64 max, s64 step, s64 bigStep);

extern s8 D_00324510[];

extern void fldStepIntByPad(void *ptr, s32 type, s64 min, s64 max, s64 small, s64 big, s8 *pad);

/* Assemble the resource request in a temporary buffer before loading it. */
extern void sdfReleaseChipBlock();

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

    sdfPacInitializeDispatchPacket(buffer, 0);
    if (flags & 2) {
        func_002EDC30(buffer);
    }
    func_002EDE48(buffer, requestFirst, requestSecond);
    func_00218768(((MdlPackageRequest *)buffer)->handle, first, second, flags);
    func_002EDC50(buffer);
}

void func_00218BE8(s32 resource) {
    sdfReleaseChipBlock((void *)resource);
}

void mdlReleaseViewerSlotResources(void) {
    MdlSlotEntry *entry;
    s32 count;
    s32 i;

    count = D_00365858[5].count;
    if (count > 0) {
        entry = D_00365858[5].entries;
        i = 0;
        do {
            i++;
            sdfReleaseChipBlock(entry->b);
            sdfReleaseChipBlock(entry->a);
            sdfReleaseChipBlock(entry->c);
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

/* Three-entry name table plus a small header, built from the first slot-table entry. */
typedef struct MdlViewerHeader {
    s16 kind;
    s16 unk02;
    s16 unk04;
    s16 unk06;
} MdlViewerHeader;

extern s32 sdfAllocGeneralBlock(s32 size);
extern void *sdfAllocSizeClassBlock(s32 size);
extern u32 sdfResourceRetainAddress(s32 handle);
extern u32 strlen(const char *);
extern char *strcpy(char *, const char *);
extern MdlViewerHeader *D_003BD880;
extern char **D_003BD884;

void mdlInitializeViewerResourceTable(void) {
    s32 i;
    char *source;
    char *copy;

    mdlReleaseViewerSlotResources();
    D_003BBB6C = sdfAllocGeneralBlock(8);
    D_003BD880 = (MdlViewerHeader *)sdfResourceRetainAddress(D_003BBB6C);
    D_003BD880->kind = 5;
    D_003BD880->unk02 = 0;
    D_003BD880->unk04 = 0x1000;
    D_003BD880->unk06 = 0x3E8;
    D_003BBB70 = sdfAllocGeneralBlock(0xC);
    D_003BD884 = (char **)sdfResourceRetainAddress(D_003BBB70);
    for (i = 0; i != 3; i++) {
        switch (i) {
        case 0:
            source = (char *)D_00365858[0].entries->b;
            break;
        case 1:
            source = (char *)D_00365858[0].entries->a;
            break;
        default:
            source = (char *)D_00365858[0].entries->c;
            break;
        }
        if (source != NULL) {
            copy = sdfAllocSizeClassBlock(strlen(source) + 1);
            strcpy(copy, source);
            switch (i) {
            case 0:
                D_003BD884[1] = copy;
                break;
            case 1:
                D_003BD884[0] = copy;
                break;
            case 2:
                D_003BD884[2] = copy;
                break;
            }
        }
    }
    D_00367900[5].entries = (MdlSlotEntry *)D_003BD880;
    D_00367900[5].count = 1;
    D_00365858[5].entries = (MdlSlotEntry *)D_003BD884;
    D_00365858[5].count = 1;
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_00218E20);

/* Relative-linked record prefix; payload fields depend on the record kind. */
typedef struct MdlRecord {
    s32 kind;        /* 0x00: 0xFFFF terminates record traversal */
    s32 nextOffset;  /* 0x04: relative byte offset to next record */
    u32 payloadWord; /* 0x08: first payload word */
    u16 listCount;   /* 0x0C: record count for list headers */
    u16 unk0E;       /* 0x0E */
    u16 unk10;       /* 0x10 */
} MdlRecord;

typedef struct MdlViewerSlots MdlViewerSlots;

/* The viewer resource holds a pointer to the table container at +0x0C. */
typedef struct MdlViewerData {
    u8 pad00[0xA4];
    MdlRecord *records;         /* 0xA4: relative-linked model records */
    MdlViewerSlots *slotTable; /* 0xA8: indexable object slots */
} MdlViewerData;

typedef struct MdlViewerResource {
    u8 pad00[0x0C];
    MdlViewerData *data;
} MdlViewerResource;

struct MdlViewerSlots {
    u8 pad00[4];
    s16 count; /* 0x04 */
    u8 pad06[6];
    s32 entryBase; /* 0x0C: base of 0x10-byte slot entries */
};

/* Read the model record's payload word without advancing its relative link. */
u32 mdlGetViewerRecordPayloadWord(MdlRecord *record) {
    return record->payloadWord;
}

u16 mdlGetViewerRecordListCount(MdlRecord *record) {
    return record->listCount;
}

/* Follow the relative links in a resource's record table to find an ID. */
MdlRecord *mdlFindViewerRecord(MdlViewerResource *resource, s32 id) {
    MdlRecord *table = resource->data->records;
    MdlRecord *record;
    s32 remaining;

    if (table == NULL) {
        return NULL;
    }
    record = table;
    remaining = mdlGetViewerRecordListCount(record);
    do {
        remaining--;
        record = (MdlRecord *)((s32)record + record->nextOffset);
        if (remaining == -1) {
            return NULL;
        }
    } while (record->kind != id);
    return record;
}

/* The first eight bytes belong to the enclosing list, not its first record. */
MdlRecord *mdlGetFirstRecord(s32 address) {
    MdlRecord *record = (MdlRecord *)(address + 8);

    if (record->kind == 0xffff) {
        record = NULL;
    }
    return record;
}

/* A terminal kind returns NULL; the link is a byte offset, not a pointer. */
MdlRecord *mdlGetNextRecord(MdlRecord *current) {
    MdlRecord *record = (MdlRecord *)((s32)current + current->nextOffset);

    if (record->kind == 0xffff) {
        record = NULL;
    }
    return record;
}

/* Count relative-offset records until the 0xffff sentinel. */
s32 mdlCountRecords(s32 address) {
    s32 count;
    MdlRecord *node;

    if (address == 0) {
        return 0;
    }
    node = mdlGetFirstRecord(address);
    count = 0;
    while (node != NULL) {
        count++;
        node = mdlGetNextRecord(node);
    }
    return count;
}

u8 mdlRecordMatchesId(MdlRecord *record, s32 wantedId) {
    return record->kind == wantedId;
}

u16 func_002193E8(MdlRecord *record) {
    return record->unk0E;
}

u16 func_002193F0(MdlRecord *record) {
    return record->unk10;
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

    sdfAppendPacket(list, sdfCreateFormattedSifCommand(labelX, y, z, 0, "MARK0:%d", params->mark0));
    sdfAppendPacket(list, sdfCreateFormattedSifCommand(labelX, y + 0x60, z, 0, "MARK1:%d", params->mark1));
    sdfAppendPacket(list, sdfCreateFormattedSifCommand(labelX, y + 0xC0, z, 0, "START:%d", params->start));
    sdfAppendPacket(list, sdfCreateFormattedSifCommand(labelX, y + 0x120, z, 0, "END  :%d", params->end));
    sdfAppendPacket(list, sdfCreateFormattedSifCommand(labelX, y + 0x180, z, 0, "ITRVL:%d", params->interval));
    sdfAppendPacket(list, sdfCreateFormattedSifCommand(labelX, y + 0x1E0, z, 0, "FACE :%d", params->face));
    sdfAppendPacket(list, sdfCreateFormattedSifCommand(labelX, y + 0x240, z, 0, "BLEND:%d", params->blend));
    for (row = 0; row != 4; row++) {
        color = params->colors[row];
        sdfAppendPacket(list, func_0011D3E8(x + 0x480, boxY, z, 0x300, 0x180, (color & 0xFFFFFF) | 0x80000000, 0x60404040));
        for (col = 0; col != 4; col++) {
            sdfAppendPacket(list, sdfCreateFormattedSifCommand(labelX, boxY, z, D_003679B0[col], D_003679B8[col]));
            sdfAppendPacket(list, sdfCreateFormattedSifCommand(x + 0x240, boxY, z, 0, D_003BBBB0, color & 0xFF));
            color >>= 8;
            boxY += 0x60;
        }
        boxY += 0x60;
    }
    if (selected >= 0) {
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y + D_00367980[selected], z, 0, D_003BBBB8));
    }
}

/* Edit the marker parameters with the pad: rows 0-6 are the scalar fields, 7-22 the color bytes. */
void mdlEditMarkParametersWithPad(EffMarkParams *params, s16 *cursor) {
    s32 selected = *cursor;
    u8 *channel;

    if (D_00324510[0x27] < 0) {
        selected = selected != 0x16 ? selected + 1 : 0;
    } else if (((u8)D_00324510[0x27] & 2) != 0 && selected < 0x16) {
        selected++;
    } else if (D_00324510[0x26] < 0) {
        selected = selected != 0 ? selected - 1 : 0x16;
    } else if (((u8)D_00324510[0x26] & 2) != 0 && selected > 0) {
        selected--;
    } else {
        switch (selected) {
        case 0:
            fldStepIntByPad(&params->mark0, 4, 0, 99999, 0, 1, D_00324510);
            break;
        case 1:
            fldStepIntByPad(&params->mark1, 4, 0, 99999, 0, 1, D_00324510);
            break;
        case 2:
            fldStepIntByPad(&params->start, 2, 0, 9999, 0, 1, D_00324510);
            break;
        case 3:
            fldStepIntByPad(&params->end, 2, 0, 9999, 0, 1, D_00324510);
            break;
        case 4:
            fldStepIntByPad(&params->interval, 2, 1, 10, 0, 1, D_00324510);
            break;
        case 5:
            fldStepIntByPad(&params->face, 1, 1, 10, 0, 1, D_00324510);
            break;
        case 6:
            fldStepIntByPad(&params->blend, 1, 0, 4, 0, 1, D_00324510);
            break;
        default:
            channel = (u8 *)params->colors + selected - 7;
            if (D_00324510[0x25] < 0) {
                *channel += 1;
            } else if (((u8)D_00324510[0x25] & 2) != 0 && *channel < 0xFF) {
                *channel += 1;
            } else if (D_00324510[0x24] < 0) {
                *channel -= 1;
            } else if (((u8)D_00324510[0x24] & 2) != 0 && *channel != 0) {
                *channel -= 1;
            }
            break;
        }
    }
    *cursor = selected;
}

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
    entry->object = effCreateNodeFromDescriptor(index);
    list->count += 1;
}

extern void *sdfAllocAndClearQuadwords(s32 size);

typedef struct MdlHandlerNode {
    s32 a;      /* 0x00 */
    void *b;    /* 0x04 */
    u8 pad08[8];
    s32 c;      /* 0x10 */
    u8 pad14[0x98];
} MdlHandlerNode;

void mdlAppendObjectPart(MdlPartList *list, s32 a, void *b, s32 c) {
    MdlHandlerNode *node = sdfAllocAndClearQuadwords(0xAC);
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
    sdfReleaseResourceAllocation(obj->handle);
    sdfReleaseChipBlock(obj);
}

void mdlObjInit(MdlObj *obj, s32 data, s32 attributes) {
    if (obj->initialized == 0) {
        obj->initialized = 1;
        sdfStreamCreateWithParams((s32)obj->data, attributes, obj->unk0, obj->unk10, data);
    }
}

typedef struct DevRequest DevRequest;
extern DevRequest *sdfDevCreateBufferedRequest(s32, s32, s32);

MdlPartList *mdlCreateBufferedPartRequest(u32 request) {
    return (MdlPartList *)sdfDevCreateBufferedRequest(request, 0x10, 4);
}

void mdlDestroyPartList(MdlPartList *list) {
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
    MdlResourceItem *item = sdfAllocAndClearQuadwords(0x20);
    MdlResourceItem *previous = object->first;
    item->type = type;
    item->next = previous;
    item->subtype = subtype;
    object->first = item;
    return item;
}

void mdlAdvanceBillboardPart(MdlPartEntry *entry) {
    billCloneObjectRetainingSharedData((u32)entry->object);
    entry->state = entry->state + 1;
}

void mdlAdvanceEffectPart(MdlPartEntry *entry) {
    effCloneSourceWithTypeHandler((u32)entry->object);
    entry->state = entry->state + 1;
}

/* Resolve a fixed-size viewer slot after checking the slot table's bounds. */
s32 mdlFindViewerPartSlot(MdlViewerResource *resource, s32 index) {
    MdlViewerSlots *slots;

    slots = resource->data->slotTable;
    if (slots == NULL) {
        return 0;
    }
    if (index >= slots->count) {
        return 0;
    }
    return slots->entryBase + index * 0x10;
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

/* Bind each consecutive record ID to a newly created part when the chunk contains it. */
void mdlBindViewerPartRecords(MdlResourceOwner *owner, MdlPartRec *rec, s32 option, s32 type, s32 (*create)(MdlPartEntry *)) {
    MdlPartEntry *part = (MdlPartEntry *)mdlFindViewerPartSlot((MdlViewerResource *)owner, rec->partIndex);

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

extern s32 effTrackPolyCreateWork(MdlEffectParams *params);

void mdlCreateViewerEffectPart(MdlResourceOwner *owner, MdlEffectRec *rec, s32 option) {
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
    item->resource = effTrackPolyCreateWork(&params);
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

void mdlLoadViewerStreamRecord(u32 owner, s32 record) {
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

void mdlClaimViewerObjectPart(MdlResourceOwner *owner, MdlEntryRec *entry, s32 option) {
    MdlSlotRec *slot = (MdlSlotRec *)mdlFindViewerPartSlot((MdlViewerResource *)owner, entry->index);

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

void mdlDispatchResourceEntry(MdlResourceOwner *owner, MdlRecord *record, s32 option) {
    switch (record->kind) {
    case 1:
        mdlBindViewerPartRecords(owner, (MdlPartRec *)record, option, 0, mdlAdvanceBillboardPart);
        return;
    case 2:
        mdlBindViewerPartRecords(owner, (MdlPartRec *)record, option, 1, mdlAdvanceEffectPart);
        return;
    case 3:
        mdlCreateViewerEffectPart(owner, (MdlEffectRec *)record, option);
        return;
    case 4:
        mdlLoadViewerStreamRecord((u32)owner, (s32)record);
        return;
    case 5:
        mdlClaimViewerObjectPart(owner, (MdlEntryRec *)record, option);
        break;
    }
}

void mdlApplyResourceEntries(s32 object, s32 id, s32 option) {
    MdlRecord *block = mdlFindViewerRecord((MdlViewerResource *)object, id);
    if (block != NULL) {
        MdlRecord *entry = mdlGetFirstRecord((s32)block);
        while (entry != NULL) {
            mdlDispatchResourceEntry(object, entry, option);
            entry = mdlGetNextRecord(entry);
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
    sdfReleaseChipBlock((void *)item);
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

extern u8 sdfViewTargetVector[];

extern u8 *sdfModelFindDrawNode(void *chunk, s32 id);

/* vu0 routine: out = p + normalize(p - sdfViewTargetVector) * scale, p = node position transformed by the node matrix */
void mdlResolveAnchorPosition(void *chunk, MdlAnchorRec *rec, f32 *out) {
    MdlNodeInfo *info = rec->info;
    u8 *matrix = sdfModelFindDrawNode(chunk, info->id);
    f32 scale = rec->scale;

    VU0_LOAD_MATRIX(matrix + 0xC0);
        VU0_LOAD_VF(vf10, info->pos);
    VU0_TRANSFORM_POINT(vf10, vf10);
    VU0_MOVE_VF(vf11, vf10);
        VU0_LOAD_VF(vf12, sdfViewTargetVector);
    VU0_SUB(vf10, vf10, vf12);
    VU0_NORMALIZE_VF10();
    VU0_SET_VF2X(scale);
    VU0_MUL_VF2X(vf10, vf10);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF_UNCLOBBERED(vf10, out);
}

extern void effCopyVector(s32 handle, f32 *src);

extern void billInvokeCallback(s32 handle);

extern void effCopyVectorToNodeInstance(s32 handle, f32 *pos);

extern void effUpdateNode(s32 handle);

extern void effTrackPolyUpdate(s32 handle);

void mdlDispatchViewerAnchorRecord(MdlResourceOwner *owner, MdlAnchorRec *rec) {
    void *chunk = owner->chunk;
    f32 position[4];
    s32 handle;

    switch (rec->type) {
    case 0:
        mdlResolveAnchorPosition(chunk, rec, position);
        handle = rec->handle;
        effCopyVector(handle, position);
        billInvokeCallback(handle);
        break;
    case 1:
        mdlResolveAnchorPosition(chunk, rec, position);
        handle = rec->handle;
        effCopyVectorToNodeInstance(handle, position);
        effUpdateNode(handle);
        break;
    case 2:
        effTrackPolyUpdate(rec->handle);
        break;
    case 3:
        mdlCondInitEntry((s32)rec);
        break;
    }
}

void mdlSetResourceFrame(s32 unused, MdlResourceItem *item, s32 frame) {
    switch (item->type) {
    case 0:
        billSetChildParameter(item->resource, frame);
        return;
    case 1:
        effSetNodeParameterValue(item->resource, frame);
        break;
    }
}

void mdlSetResourceAmount(s32 unused, MdlResourceItem *item, float amount) {
    switch (item->type) {
    case 0:
        billSetChildScaleComponents(item->resource, amount, amount);
        return;
    case 1:
        effApplyNodeScale(item->resource, amount);
        break;
    }
}

s32 mdlBuildViewerRectanglePacket(s32 x, s32 y, s32 depth, s32 width, s32 height) {
    return func_0011D3E8(x, y, depth, width, height, 0x30000000, 0x60404040);
}

void mdlAppendViewerRectToDrawList(s32 x, s32 y, s32 depth, s32 width, s32 height, s32 unused) {
    s32 packet;

    packet = D_003D7B10[0];
    sdfAppendPacket(packet, mdlBuildViewerRectanglePacket(x, y, depth, width, height));
}

extern s8 sdfPadButtonStates[];

s32 mdlUpdateViewerCursor(s16 *cursor, s32 count) {
    s32 max = count - 1;
    s32 v = *cursor;
    s32 changed = 0;
    if (sdfPadButtonStates[5] < 0) {
        if (v < max) {
            v += 1;
            changed = 1;
        } else {
            v = 0;
            changed = 1;
        }
    } else if (((u8)sdfPadButtonStates[5] & 2) != 0) {
        if (v < max) {
            v += 1;
            changed = 1;
        }
    } else if (sdfPadButtonStates[4] < 0) {
        if (v > 0) {
            v -= 1;
            changed = 1;
        } else {
            v = max;
            changed = 1;
        }
    } else if ((((u8)sdfPadButtonStates[4] & 2) != 0) && (v > 0)) {
        v -= 1;
        changed = 1;
    }
    if (changed != 0) {
        *cursor = v;
        mdlViewerState.unk09 = 0;
        return 1;
    }
    return 0;
}

s32 mdlUpdateViewerCursorWithPageStep(s16 *cursor, s32 count, s32 step) {
    s32 max = count - 1;
    s32 v = *cursor;
    s32 changed = 0;
    if (sdfPadButtonStates[7] < 0) {
        if (v < max) {
            v += 1;
        } else {
            v = 0;
        }
        changed = 1;
    } else if (((u8)sdfPadButtonStates[7] & 2) != 0) {
        if (v < max) {
            v += 1;
            changed = 1;
        }
    } else if (sdfPadButtonStates[6] < 0) {
        if (v > 0) {
            v -= 1;
        } else {
            v = max;
        }
        changed = 1;
    } else if (((u8)sdfPadButtonStates[6] & 2) != 0) {
        if (v > 0) {
            v -= 1;
            changed = 1;
        }
    } else if (step != 0) {
        if (sdfPadButtonStates[11] < 0) {
            if (v < max) {
                v += step;
                if (v > max) {
                    v = max;
                }
            } else {
                v = 0;
            }
            changed = 1;
        } else if (((u8)sdfPadButtonStates[11] & 2) != 0) {
            if (v < max) {
                v += step;
                if (v > max) {
                    v = max;
                }
                changed = 1;
            }
        } else if (sdfPadButtonStates[10] < 0) {
            if (v > 0) {
                v -= step;
                if (v < 0) {
                    v = 0;
                }
            } else {
                v = max;
            }
            changed = 1;
        } else if ((((u8)sdfPadButtonStates[10] & 2) != 0) && (v > 0)) {
            v -= step;
            if (v < 0) {
                v = 0;
            }
            changed = 1;
        }
    }
    if (changed != 0) {
        *cursor = v;
        mdlViewerState.unk09 = 0;
        return 1;
    }
    return 0;
}

typedef struct MdlMotionState {
    u8 pad00[0x1C];
    f32 time;   /* 0x1C */
    u8 pad20[0xE];
    u16 length; /* 0x2E */
    u8 pad30[2];
    u8 reverse; /* 0x32 */
} MdlMotionState;

typedef struct MdlResource {
    u8 pad00[0x12];
    s16 unk12;
    u8 pad14[4];
    u8 *chunk;              /* 0x18 */
    MdlMotionState *motion; /* 0x1C */
} MdlResource;

extern MdlResource *func_00217680(s16, s16);

extern void mdlAddEntryFlagged(void *, s32, s32);

void mdlLoadViewerResourceAndResetCursors(void) {
    MdlResource *resource = func_00217680(mdlViewerState.unk18, mdlViewerState.unk1A);

    /* Required to match: storing through a typed pointer preserves the load/store order. */
    *(MdlResource **)&mdlViewerState.resources[0] = resource;
    mdlViewerState.activeEntryId = 0;
    mdlViewerState.selectedEntryId = 0;
    mdlViewerState.selectedNodeId = 0;
    mdlViewerState.unk28 = 0;
    mdlViewerState.unk2A = 0;
    mdlViewerState.unk2C = 0;
    if (resource->motion != NULL) {
        mdlAddEntryFlagged(resource, 0, 0);
    }
    mdlViewerState.nodeCursor = 0;
}

extern u128 D_003D7B20;

extern u128 D_003D7B30;

extern u128 D_003D7B40;

typedef struct {
    f32 x, y, z, w;
} __attribute__((aligned(16))) MdlEyeVec;

extern MdlEyeVec D_00367A10;

extern MdlEyeVec D_00367A20;

extern u128 D_00367A30;

extern s16 D_003D7A84[];

void mdlResetViewerBasisVectors(void) {
    PCP_COPY_VECTOR(&D_003D7B20, &D_00367A10);
    PCP_COPY_VECTOR(&D_003D7B30, &D_00367A20);
    PCP_COPY_VECTOR(&D_003D7B40, &D_00367A30);
    D_003D7A84[0] = 0;
}

/* Rotate the viewer resource list in place without moving its allocation. */
void mdlRotateViewResourcesRight(void) {
    s32 i = mdlViewerState.resourceCount - 1;
    s32 saved = mdlViewerState.resources[i];

    if (i > 0) {
        do {
            mdlViewerState.resources[i] = mdlViewerState.slotBeforeResources[i];
            i -= 1;
        } while (i > 0);
    }
    mdlViewerState.resources[0] = saved;
}

void mdlRotateViewList(void) {
    s32 i;
    s32 count = mdlViewerState.resourceCount;
    s32 first = mdlViewerState.resources[0];

    for (i = 0; i < count - 1; i++) {
        mdlViewerState.resources[i] = mdlViewerState.resources[i + 1];
    }
    mdlViewerState.resources[i] = first;
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

    if (mdlHasNode(mdlViewerState.resources[0], mdlViewerState.selectedNodeId)) {
        low = mdlViewerState.entryHeight;
        high = mdlViewerState.entryWidth;
        first = high;
        if (low < high) {
            first = low;
        }
        mdlViewerState.activeEntryId = mdlViewerState.selectedEntryId;
        mdlAddEntryFlaggedEx(mdlViewerState.resources[0], mdlViewerState.selectedNodeId, mdlViewerState.selectedEntryId, first, low);
    }
}

void mdlAddPlainViewerEntryForSelectedNode(void) {
    f32 low;
    f32 high;
    f32 first;

    if (mdlHasNode(mdlViewerState.resources[0], mdlViewerState.selectedNodeId)) {
        low = mdlViewerState.entryHeight;
        high = mdlViewerState.entryWidth;
        first = high;
        if (low < high) {
            first = low;
        }
        mdlViewerState.activeEntryId = mdlViewerState.selectedEntryId;
        mdlAddEntryPlainEx(mdlViewerState.resources[0], mdlViewerState.selectedNodeId, mdlViewerState.selectedEntryId, first, low);
    }
}

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABCC8);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABCD8);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021AE00);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021B0B0);

u32 mdlRunViewerAssetSelectionTask(void) {
    func_0021AE00();
    func_0021B0B0();
    return 0;
}

extern void effMiscAxisAngleToQuaternionVf11(f32 angle);
extern void effMiscQuatMultiplyVU(void);
extern f32 sdfViewMatrix[];
extern void effMiscQuaternionToMatrixVU(void);

/* vu0 routine: move the viewer camera or orbit it about the look-at point. */
void func_0021B510(void) {
    f32 speed;
    s32 rotated;

    if (sdfPadButtonStates[0xC] < 0) {
        mdlResetViewerBasisVectors();
        return;
    }
    if (sdfPadButtonStates[0] < 0) {
        if (++mdlViewerState.labelIndexA == 2) {
            mdlViewerState.labelIndexA = 0;
        }
    }
    if (mdlViewerState.labelIndexA == 0) {
        speed = 1.25f;
        if (sdfPadButtonStates[1] == 0) {
            speed = 20.0f;
            if (sdfPadButtonStates[3] == 0) {
                speed = 5.0f;
            }
        }
        VU0_MOVE_VF(vf10, vf0);
        EE_MMI_LOAD_MATRIX_COLUMN(vf11, sdfViewMatrix);
        VU0_SCALAR_OP(speed, "vmulx.xyzw vf11, vf11, vf2x");
        if (sdfPadButtonStates[5] != 0) {
            VU0_ADD(vf10, vf10, vf11);
        } else if (sdfPadButtonStates[4] != 0) {
            VU0_SUB(vf10, vf10, vf11);
        }
        EE_MMI_LOAD_MATRIX_COLUMN(vf11, sdfViewMatrix + 2);
        VU0_SCALAR_OP(speed, "vmulx.xyzw vf11, vf11, vf2x");
        if (sdfPadButtonStates[6] != 0) {
            VU0_ADD(vf10, vf10, vf11);
        } else if (sdfPadButtonStates[7] != 0) {
            VU0_SUB(vf10, vf10, vf11);
        }
        EE_MMI_LOAD_MATRIX_COLUMN(vf11, sdfViewMatrix + 1);
        VU0_SCALAR_OP(speed, "vmulx.xyzw vf11, vf11, vf2x");
        if (sdfPadButtonStates[9] != 0) {
            VU0_ADD(vf10, vf10, vf11);
        } else if (sdfPadButtonStates[8] != 0) {
            VU0_SUB(vf10, vf10, vf11);
        }
        VU0_MOVE_VF(vf11, vf10);
        VU0_LOAD_VF(vf10, &D_003D7B20);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, &D_003D7B20);
        VU0_LOAD_VF(vf10, &D_003D7B30);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, &D_003D7B30);
    } else {
        speed = 0.25f * 3.14159265f / 180.0f;
        if (sdfPadButtonStates[1] == 0) {
            speed = 4.0f * 3.14159265f / 180.0f;
            if (sdfPadButtonStates[3] == 0) {
                speed = 1.0f * 3.14159265f / 180.0f;
            }
        }
        rotated = 0;
        VU0_MOVE_VF(vf10, vf0);
        if (sdfPadButtonStates[5] != 0) {
            EE_MMI_LOAD_MATRIX_COLUMN(vf11, sdfViewMatrix + 1);
            effMiscAxisAngleToQuaternionVf11(speed);
            effMiscQuatMultiplyVU();
            rotated = 1;
        } else if (sdfPadButtonStates[4] != 0) {
            EE_MMI_LOAD_MATRIX_COLUMN(vf11, sdfViewMatrix + 1);
            effMiscAxisAngleToQuaternionVf11(-speed);
            effMiscQuatMultiplyVU();
            rotated = 1;
        }
        if (sdfPadButtonStates[6] != 0) {
            EE_MMI_LOAD_MATRIX_COLUMN(vf11, sdfViewMatrix);
            effMiscAxisAngleToQuaternionVf11(speed);
            effMiscQuatMultiplyVU();
            rotated = 1;
        } else if (sdfPadButtonStates[7] != 0) {
            EE_MMI_LOAD_MATRIX_COLUMN(vf11, sdfViewMatrix);
            effMiscAxisAngleToQuaternionVf11(-speed);
            effMiscQuatMultiplyVU();
            rotated = 1;
        }
        if (sdfPadButtonStates[8] != 0) {
            EE_MMI_LOAD_MATRIX_COLUMN(vf11, sdfViewMatrix + 2);
            effMiscAxisAngleToQuaternionVf11(speed);
            effMiscQuatMultiplyVU();
            rotated = 1;
        } else if (sdfPadButtonStates[9] != 0) {
            EE_MMI_LOAD_MATRIX_COLUMN(vf11, sdfViewMatrix + 2);
            effMiscAxisAngleToQuaternionVf11(-speed);
            effMiscQuatMultiplyVU();
            rotated = 1;
        }
        if (rotated) {
            effMiscQuaternionToMatrixVU();
            VU0_LOAD_VF(vf10, &D_003D7B30);
            VU0_LOAD_VF(vf11, &D_003D7B20);
            VU0_SUB(vf10, vf10, vf11);
            VU0_TRANSFORM_POINT(vf10, vf10);
            VU0_ADD(vf10, vf10, vf11);
            VU0_STORE_VF(vf10, &D_003D7B30);
            VU0_LOAD_VF(vf10, &D_003D7B40);
            VU0_TRANSFORM_POINT(vf10, vf10);
            VU0_STORE_VF(vf10, &D_003D7B40);
        }
    }
}

extern s32 D_00367B10[];

extern s32 sdfCreateFormattedSifCommand();

void mdlDrawViewerIndexedLabelOverlay(void) {
    mdlAppendViewerRectToDrawList(0x8A10, 0x7948, 0xFF007F, 0x4E0, 0x90, 0);
    sdfAppendPacket(mdlViewerState.resources[12], sdfCreateFormattedSifCommand(0x8A40, 0x7960, 0xFF0080, 0, D_00367B10[mdlViewerState.labelIndexA]));
}

u32 mdlRunViewerIndexedLabelTask(void) {
    func_0021B510();
    mdlDrawViewerIndexedLabelOverlay();
    return 0;
}

typedef struct MdlCtx MdlCtx;
extern void mdlLoadPrimaryVectorVU(MdlCtx *context);
extern void mdlStorePrimaryVectorVU(MdlCtx *context);
extern void mdlLoadSecondaryVectorVU(MdlCtx *context);
extern void mdlUpdateContextRotationBasisFromQuaternion(MdlCtx *context);
extern f32 D_00398380[4];
extern f32 D_00398390[4];
extern f32 D_003983A0[4];

/* vu0 routine: edit the viewer transform with translation or quaternion steps. */
void func_0021B9F8(void) {
    f32 speed;
    f32 angle;

    if (sdfPadButtonStates[0x11] < 0) {
        mdlViewerState.yawStepMode ^= 1;
    }
    if (sdfPadButtonStates[0] < 0) {
        mdlViewerState.labelIndexB ^= 1;
        return;
    }
    if (sdfPadButtonStates[0xC] < 0) {
        VU0_MOVE_VF(vf10, vf0);
        mdlStorePrimaryVectorVU((MdlCtx *)mdlViewerState.resources[0]);
        mdlUpdateContextRotationBasisFromQuaternion((MdlCtx *)mdlViewerState.resources[0]);
        return;
    }
    if (mdlViewerState.yawStepMode != 0) {
        mdlLoadSecondaryVectorVU((MdlCtx *)mdlViewerState.resources[0]);
        if (sdfPadButtonStates[5] < 0) {
            VU0_LOAD_VF(vf11, D_00398390);
            angle = 5.0f * 3.14159265f / 180.0f;
        } else if (sdfPadButtonStates[4] < 0) {
            VU0_LOAD_VF(vf11, D_00398390);
            angle = -5.0f * 3.14159265f / 180.0f;
        } else {
            return;
        }
        effMiscAxisAngleToQuaternionVf11(angle);
        effMiscQuatMultiplyVU();
        mdlUpdateContextRotationBasisFromQuaternion((MdlCtx *)mdlViewerState.resources[0]);
    } else if (mdlViewerState.labelIndexB == 0) {
        speed = 1.25f;
        if (sdfPadButtonStates[1] == 0) {
            speed = 20.0f;
            if (sdfPadButtonStates[3] == 0) {
                speed = 5.0f;
            }
        }
        mdlLoadPrimaryVectorVU((MdlCtx *)mdlViewerState.resources[0]);
        EE_MMI_LOAD_MATRIX_COLUMN(vf11, sdfViewMatrix);
        VU0_SCALAR_OP(speed, "vmulx.xyzw vf11, vf11, vf2x");
        if (sdfPadButtonStates[5] != 0) {
            VU0_ADD(vf10, vf10, vf11);
        } else if (sdfPadButtonStates[4] != 0) {
            VU0_SUB(vf10, vf10, vf11);
        }
        EE_MMI_LOAD_MATRIX_COLUMN(vf11, sdfViewMatrix + 2);
        VU0_SCALAR_OP(speed, "vmulx.xyzw vf11, vf11, vf2x");
        if (sdfPadButtonStates[6] != 0) {
            VU0_ADD(vf10, vf10, vf11);
        } else if (sdfPadButtonStates[7] != 0) {
            VU0_SUB(vf10, vf10, vf11);
        }
        EE_MMI_LOAD_MATRIX_COLUMN(vf11, sdfViewMatrix + 1);
        VU0_SCALAR_OP(speed, "vmulx.xyzw vf11, vf11, vf2x");
        if (sdfPadButtonStates[9] != 0) {
            VU0_ADD(vf10, vf10, vf11);
        } else if (sdfPadButtonStates[8] != 0) {
            VU0_SUB(vf10, vf10, vf11);
        }
        mdlStorePrimaryVectorVU((MdlCtx *)mdlViewerState.resources[0]);
    } else {
        speed = 0.5f * 3.14159265f / 180.0f;
        if (sdfPadButtonStates[1] == 0) {
            speed = 8.0f * 3.14159265f / 180.0f;
            if (sdfPadButtonStates[3] == 0) {
                speed = 2.0f * 3.14159265f / 180.0f;
            }
        }
        mdlLoadSecondaryVectorVU((MdlCtx *)mdlViewerState.resources[0]);
        if (sdfPadButtonStates[5] != 0) {
            VU0_LOAD_VF(vf11, D_00398390);
            effMiscAxisAngleToQuaternionVf11(speed);
            effMiscQuatMultiplyVU();
        } else if (sdfPadButtonStates[4] != 0) {
            VU0_LOAD_VF(vf11, D_00398390);
            effMiscAxisAngleToQuaternionVf11(-speed);
            effMiscQuatMultiplyVU();
        }
        if (sdfPadButtonStates[6] != 0) {
            VU0_LOAD_VF(vf11, D_00398380);
            effMiscAxisAngleToQuaternionVf11(speed);
            effMiscQuatMultiplyVU();
        } else if (sdfPadButtonStates[7] != 0) {
            VU0_LOAD_VF(vf11, D_00398380);
            effMiscAxisAngleToQuaternionVf11(-speed);
            effMiscQuatMultiplyVU();
        }
        if (sdfPadButtonStates[9] != 0) {
            VU0_LOAD_VF(vf11, D_003983A0);
            effMiscAxisAngleToQuaternionVf11(speed);
            effMiscQuatMultiplyVU();
        } else if (sdfPadButtonStates[8] != 0) {
            VU0_LOAD_VF(vf11, D_003983A0);
            effMiscAxisAngleToQuaternionVf11(-speed);
            effMiscQuatMultiplyVU();
        }
        mdlUpdateContextRotationBasisFromQuaternion((MdlCtx *)mdlViewerState.resources[0]);
    }
}

extern s32 D_00367B18[];

void mdlDrawViewerSelectionLabel(void) {
    mdlAppendViewerRectToDrawList(0x8A10, 0x7948, 0xFF007F, 0x4E0, 0x90, 0);
    sdfAppendPacket(mdlViewerState.resources[12], sdfCreateFormattedSifCommand(0x8A40, 0x7960, 0xFF0080, 0, D_00367B18[mdlViewerState.labelIndexB]));
}

s32 mdlRunViewerSelectionLabelTask(void) {
    func_0021B9F8();
    if (D_003D7A60[0] == 0) {
        mdlDrawViewerSelectionLabel();
    }
    return 0;
}

/* Change the viewer scale in hundredths, with larger steps at larger values. */
void mdlAdjustViewerScale(void) {
    s32 value = (s32)(mdlViewerState.viewerScale * 100.0f + 0.5f);

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
        mdlViewerState.viewerScale = value * 0.01f;
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
        mdlViewerState.viewerScale = value * 0.01f;
    }
    if (D_00324510[0x23] < 0) {
        mdlViewerState.unitStepMode ^= 1;
    }
    mdlViewerState.unitStepSign = 0;
    if (mdlViewerState.unitStepMode != 0) {
        if (D_00324510[0x25] != 0) {
            mdlViewerState.unitStepSign = 1;
        } else if (D_00324510[0x24] != 0) {
            mdlViewerState.unitStepSign = -1;
        }
    }
}

extern void sdfAppendFillRectanglePacket();

/* Draw the motion progress bar: timeline frame, playhead marker and "[time/length]" label, then the zoom value. */
void mdlDrawViewerMotionTimeline(void) {
    s32 list;
    MdlResource *resource;
    MdlMotionState *motion;
    s32 x;
    s32 step;

    mdlAppendViewerRectToDrawList(0x81D0, 0x7948, 0xFF007F, 0xD20, 0xF0, 0);
    list = mdlViewerState.resources[12];
    sdfAppendFillRectanglePacket(list, 0x80303030, 0, 0x8200, 0x7990, 0x8EC0, 0x7990, 0xFF0080, 0);
    sdfAppendFillRectanglePacket(list, 0x80303030, 0, 0x8200, 0x7960, 0x8200, 0x79C0, 0xFF0080, 0);
    sdfAppendFillRectanglePacket(list, 0x80303030, 0, 0x8EC0, 0x7960, 0x8EC0, 0x79C0, 0xFF0080, 0);
    resource = (MdlResource *)mdlViewerState.resources[0];
    motion = resource->motion;
    if (motion != NULL) {
        x = (s32)(motion->time * 3264.0f / motion->length);
        if (x < 0) {
            x = 0;
        }
        x += 0x8200;
        sdfAppendFillRectanglePacket(list, 0x800000E0, 0, x, 0x7960, x, 0x79C0, 0xFF0090, 0);
        sdfAppendPacket(mdlViewerState.resources[12], sdfCreateFormattedSifCommand(0x8200, 0x79C0, 0xFF0080, 0, "[%5.1f/%-3d]", motion->time, motion->length));
    } else {
        sdfAppendPacket(mdlViewerState.resources[12], sdfCreateFormattedSifCommand(0x8200, 0x79C0, 0xFF0080, 0, "[---.-/---]"));
    }
    step = mdlViewerState.unitStepMode != 0 ? 2 : 0;
    sdfAppendPacket(mdlViewerState.resources[12], sdfCreateFormattedSifCommand(0x8B00, 0x79C0, 0xFF0080, step, D_003BBC80, mdlViewerState.viewerScale));
}

u32 mdlUpdateViewerScaleTask(void) {
    mdlAdjustViewerScale();
    mdlDrawViewerMotionTimeline();
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

extern s32 mdlGetContextResourceGroup(s32);
extern s32 mdlGetContextResourceId(s32);
extern void mdlDestroyContext(s32 resource);
extern void sdfMotionInitializeAtZeroTime(void *, s32, s32);

void mdlApplyViewerResourceMenuAction(void) {
    s32 i;
    s32 item; /* resource handle, then its frame id */
    MdlMotionState *motion;

    if (mdlUpdateViewerCursorWithPageStep(&mdlViewerState.scrollPage, 5, 1) != 0) {
        return;
    }
    if (sdfPadButtonStates[1] >= 0) {
        return;
    }
    switch (mdlViewerState.scrollPage) {
    case 0:
        if (mdlViewerState.resourceCount != 12) {
            mdlViewerState.resources[mdlViewerState.resourceCount] = 0;
            mdlViewerState.resourceCount++;
            mdlRotateViewResourcesRight();
            mdlLoadViewerResourceAndResetCursors();
        }
        break;
    case 1:
        if (mdlViewerState.resourceCount >= 2) {
            mdlDestroyContext(mdlViewerState.resources[0]);
            mdlViewerState.resources[0] = 0;
            mdlRotateViewList();
            mdlViewerState.resourceCount--;
        }
        break;
    case 2:
        if (mdlViewerState.resourceCount >= 2) {
            mdlRotateViewResourcesRight();
        }
        break;
    case 3:
        if (mdlViewerState.resourceCount >= 2) {
            mdlRotateViewList();
        }
        break;
    case 4:
        for (i = 0; i < mdlViewerState.resourceCount; i++) {
            item = mdlViewerState.resources[i];
            motion = ((MdlResource *)item)->motion;
            if (motion != NULL) {
                item = ((MdlResource *)item)->unk12;
                if (motion->reverse == 0) {
                    sdfMotionInitializeAtZeroTime(motion, item, 0);
                } else {
                    sdfMotionInitializeAtZeroTime(motion, item, 1);
                }
            }
        }
        break;
    }
    mdlViewerState.unk1C = mdlViewerState.unk18 = mdlGetContextResourceGroup(mdlViewerState.resources[0]);
    mdlViewerState.unk1E = mdlViewerState.unk1A = mdlGetContextResourceId(mdlViewerState.resources[0]);
    i = ((MdlResource *)mdlViewerState.resources[0])->unk12;
    if (i < 0) {
        i = 0;
    }
    mdlViewerState.selectedEntryId = mdlViewerState.activeEntryId = i;
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021C518);

u32 mdlRunViewerResourceMenuTask(void) {
    mdlApplyViewerResourceMenuAction();
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

void mdlHandleViewerNodeCursorInput(void) {
    MdlCountNode *firstNode = ((MdlLoaded *)mdlViewerState.resources[0])->info->first;

    if (firstNode != 0) {
        s16 nodeCount = firstNode->count;

        if (nodeCount > 0) {
            mdlUpdateViewerCursor(&mdlViewerState.nodeCursor, nodeCount);
        }
    }
}

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABD58);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABD68);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABD78);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABD88);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABD98);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021C6E8);

u32 mdlUpdateViewerNodeCursorTask(void) {
    mdlHandleViewerNodeCursorInput();
    func_0021C6E8();
    return 0;
}

s32 mdlIsDebugTimeGraph(void) {
    return kwlnTaskGetTaskByName("DebugTimeGrph") != 0;
}

/* Debug menu: page 0 selects an action, pages 1-4 edit the color channels and value steps. */
void mdlUpdateViewerSettingsInput(void) {
    switch (mdlViewerState.unk3E) {
    case 0:
        if (mdlUpdateViewerCursorWithPageStep(&mdlViewerState.unk3C, 10, 1) != 0) {
            return;
        }
        if (sdfPadButtonStates[1] >= 0) {
            return;
        }
        switch (mdlViewerState.unk3C) {
        case 0:
            mdlViewerState.unk0E ^= 1;
            break;
        case 1:
            kwlnDebugGraphSetEnabled(mdlIsDebugTimeGraph() == 0);
            break;
        case 2:
            mdlViewerState.unk0F ^= 1;
            break;
        case 3:
        case 4:
        case 5:
            mdlViewerState.unk3E = 1;
            break;
        case 6:
        case 7:
        case 8:
            mdlViewerState.unk3E = mdlViewerState.unk3C - 4;
            break;
        case 9:
            PCP_COPY_VECTOR(&D_00367A10, &D_003D7B20);
            PCP_COPY_VECTOR(&D_00367A20, &D_003D7B30);
            func_0021E068();
            break;
        }
        break;
    case 1:
        fldStepColorChannelByPad(&D_003BA8EC, mdlViewerState.unk3C - 3, sdfPadButtonStates);
        if (sdfPadButtonStates[1] < 0 || sdfPadButtonStates[3] < 0) {
            mdlViewerState.unk3E = 0;
        }
        break;
    case 2:
        if (sdfPadButtonStates[1] < 0 || sdfPadButtonStates[3] < 0) {
            mdlViewerState.unk3E = 0;
        } else {
            fldAdjustIntegerUsingMainPad(&mdlViewerState.unk4A, 2, 1, 8, 1, 1);
        }
        break;
    case 3:
        if (sdfPadButtonStates[1] < 0 || sdfPadButtonStates[3] < 0) {
            mdlViewerState.unk3E = 0;
        } else {
            fldAdjustIntegerUsingMainPad(&mdlViewerState.unk4C, 2, 1, 8, 1, 1);
        }
        break;
    case 4:
        if (sdfPadButtonStates[1] < 0 || sdfPadButtonStates[3] < 0) {
            mdlViewerState.unk3E = 0;
        } else {
            fldAdjustIntegerUsingMainPad(&mdlViewerState.unk4E, 2, 1, 8, 1, 1);
        }
        break;
    }
}

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABE18);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABE30);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABE48);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABE60);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABE70);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABE80);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021CB80);

u32 mdlRunViewerSettingsTask(void) {
    mdlUpdateViewerSettingsInput();
    func_0021CB80();
    return 0;
}

s32 mdlCountActiveRecords(void) {
    MdlViewerResource *resource = (MdlViewerResource *)mdlViewerState.resources[0];
    s32 first = mdlCountRecords((s32)mdlFindViewerRecord(resource, -1));
    s32 second = mdlCountRecords((s32)mdlFindViewerRecord(resource, mdlViewerState.activeEntryId));

    return first + second;
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021CF00);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021CFC0);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021D198);

u32 mdlRunViewerEffectEditorTask(void) {
    func_0021CFC0();
    func_0021D198();
    return 0;
}

s32 mdlRequestViewerExitOnce(void) {
    if (mdlViewerState.unk08 == 0) {
        mdlViewerState.unk08 = 1;
        mdlCleanupViewerTasksAndResources();
        dds3AdminSetControlFlag();
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

void mdlSubmitViewerIntermediateDrawPacket(void) {
    s32 list;

    if (mdlViewerState.unk0A < 4) {
        if (mdlViewerState.unk0A >= 2) {
            list = sdfCreateResetPacketList();
            VU0_SET_UNIT_MATRIX(vf28, vf29, vf30, vf31);
            sdfAppendPacket(list, func_002EF2E0(D_00367B80, D_00367BE0, 6, 0x80));
            D_00325048.submit(&D_00325048, list);
        }
    }
}

extern u8 D_00367C00[];

extern u8 D_00367C40[];

extern s32 D_003BBCB8;

extern void func_002EA2E0(s32 packet, s32 arg);

void mdlSubmitViewerResourceDrawPacket(void) {
    s32 list;
    s32 packet;
    MdlResource *resource;

    if (mdlViewerState.unk0A == 3) {
        list = sdfCreateResetPacketList();
        resource = (MdlResource *)mdlViewerState.resources[0];
    VU0_LOAD_MATRIX(resource->chunk + 0x20);
        packet = func_002EF2E0(D_00367C00, D_00367C40, 4, 0x80);
        if ((sdfPadButtonStates[13] < 0) & (D_003BBCB8 == 0)) {
            D_003BBCB8 = 1;
            func_002EA2E0(packet, 0x100);
        }
        sdfAppendPacket(list, packet);
        D_00325048.submit(&D_00325048, list);
    }
}

void mdlViewerTaskDestroy(void) {
    if (mdlViewerState.viewerTask != 0) {
        kwlnTaskDestroyWithHierarchy(mdlViewerState.viewerTask, 0);
        mdlViewerState.viewerTask = 0;
    }
}

void mdlRestartViewerPhaseTask(void) {
    mdlViewerTaskDestroy();
    mdlViewerState.viewerTask = kwlnTaskCreate(D_00367A40[mdlViewerState.unk0A - 1].name, 0x2B00, 1, 0, D_00367A40[mdlViewerState.unk0A - 1].update, 0, D_00367A40[mdlViewerState.unk0A - 1].data);
    func_00101A80(mdlViewerState.unk00, mdlViewerState.viewerTask);
}

INCLUDE_ASM(const s32, "game/code_00218B48", mdlViewer);

extern u8 D_00367C50[];

extern u8 D_00367CB0[];

extern s32 sdfCountMapPositionRecords(void *chunk);

extern u8 *sdfChunkFindByTag(void *chunk, s32 tag);

extern void sdfSetLookAtBasisFromRecord(void *chunk, u8 *record);

void mdlDrawMapPositionRecords(MdlResource *resource) {
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

typedef struct MdlFogParams {
    f32 near;  /* 0x00 */
    f32 farA;  /* 0x04 */
    f32 value; /* 0x08 */
    f32 farB;  /* 0x0C */
    u32 color; /* 0x10 */
} MdlFogParams;

extern char D_00367AF8[];
extern char D_003BBCC0[]; /* "%x" */
extern char D_003BBCC8[]; /* "fovy=" */
extern char D_003BBCD0[]; /* "%f" */
extern char D_003BBCD8[]; /* "fog=" */
extern MdlFogParams kwlnDrawVector;
extern f32 sdfSceneProjectionParameters[];
extern s32 sdfPathExists(char *path);
extern s32 fileQueueDefaultCallbackRequest(char *path);
extern void fileWaitReady(s32 file);
extern s32 fileGetResourceHandle(s32 file);
extern char *fileGetLoadedDataAddress(s32 file);
extern s32 fileGetResourceSize(s32 file);
extern void filePollEntryCleanup(s32 file);
extern s32 func_00301588();
extern s32 memcmp(const void *, const void *, u32);

/* Read the viewer's config text file: bg-color=, eye-position=, target-position=, fovy=, fog= lines. */
void mdlLoadViewerPresentationConfig(void) {
    s32 file;
    s32 handle;
    char *data;
    s32 size;
    s32 pos;
    s32 i;
    char *line;
    u32 color;
    f32 x;
    f32 y;
    f32 z;
    f32 fovy;
    s32 fogNear;
    f32 fogValue;
    s32 fogFar;
    f32 fogFarB;

    if (sdfPathExists(D_00367AF8) == 0) {
        return;
    }
    file = fileQueueDefaultCallbackRequest(D_00367AF8);
    fileWaitReady(file);
    handle = fileGetResourceHandle(file);
    data = fileGetLoadedDataAddress(file);
    size = fileGetResourceSize(file);
    filePollEntryCleanup(file);
    pos = 0;
    while (pos < size) {
        i = pos;
        while (i < size) {
            if (data[i++] == '\n') {
                break;
            }
        }
        line = data + pos;
        if (memcmp(line, "bg-color=", 9) == 0) {
            if (func_00301588(line + 9, D_003BBCC0, &color) == 1) {
                D_003BA8EC = color;
            }
        } else if (memcmp(line, "eye-position=", 13) == 0) {
            if (func_00301588(line + 13, "%f,%f,%f", &x, &y, &z) == 3) {
                D_00367A10.x = x;
                D_00367A10.y = y;
                D_00367A10.z = z;
            }
        } else if (memcmp(line, "target-position=", 16) == 0) {
            if (func_00301588(line + 16, "%f,%f,%f", &x, &y, &z) == 3) {
                D_00367A20.x = x;
                D_00367A20.y = y;
                D_00367A20.z = z;
            }
        } else if (memcmp(line, D_003BBCC8, 5) == 0) {
            if (func_00301588(line + 5, D_003BBCD0, &fovy) == 1) {
                sdfSceneProjectionParameters[3] = fovy;
            }
        } else if (memcmp(line, D_003BBCD8, 4) == 0) {
            if (func_00301588(line + 4, "%d,%f,%d,%f,%x", &fogNear, &fogValue, &fogFar, &fogFarB, &color) == 5) {
                kwlnDrawVector.near = fogNear;
                kwlnDrawVector.value = fogValue;
                kwlnDrawVector.farA = fogFar;
                kwlnDrawVector.farB = fogFarB;
                kwlnDrawVector.color = color;
            }
        }
        pos = i;
    }
    sdfReleaseResourceAllocation(handle);
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021E068);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021E1C8);

void mdlFreeViewResources(void) {
    s32 i;
    for (i = 0; i != 12; i++) {
        s32 handle = mdlViewerState.resources[i];
        if (handle != 0) {
            mdlViewerState.resources[i] = 0;
            mdlDestroyContext(handle);
        }
    }
}

extern s32 kwlnTaskDestroyWithHierarchyByName(const char *name, s32 flag);

extern void func_00103498(const char *name, s32, s32, s32);

void mdlCleanupViewerTasksAndResources(MdlViewState *view) {
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

typedef struct MdlRotationContext {
    u8 pad00[8];
    MdlPadState *pad;
} MdlRotationContext;

extern void effMiscAxisAngleToQuaternionVU(f32 angle);
extern void effMiscQuaternionToMatrixVU(void);

/* vu0 routine: rotate a viewer position about the camera axes selected by the pad. */
void func_0021E450(MdlRotationContext *context, f32 *position)
{
    if (context->pad->stepUpA != 0) {
        EE_MMI_LOAD_MATRIX_COLUMN(vf10, sdfViewMatrix + 1);
        effMiscAxisAngleToQuaternionVU(2.0f * 3.14159265f / 180.0f);
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, position);
        VU0_TRANSFORM_POINT(vf10, vf10);
        VU0_SET_W_ONE(vf10);
        VU0_STORE_VF(vf10, position);
    } else if (context->pad->stepDownA != 0) {
        EE_MMI_LOAD_MATRIX_COLUMN(vf10, sdfViewMatrix + 1);
        effMiscAxisAngleToQuaternionVU(-2.0f * 3.14159265f / 180.0f);
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, position);
        VU0_TRANSFORM_POINT(vf10, vf10);
        VU0_SET_W_ONE(vf10);
        VU0_STORE_VF(vf10, position);
    }
    if (context->pad->stepUpB != 0) {
        EE_MMI_LOAD_MATRIX_COLUMN(vf10, sdfViewMatrix);
        effMiscAxisAngleToQuaternionVU(-2.0f * 3.14159265f / 180.0f);
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, position);
        VU0_TRANSFORM_POINT(vf10, vf10);
        VU0_SET_W_ONE(vf10);
        VU0_STORE_VF(vf10, position);
    } else if (context->pad->stepDownB != 0) {
        EE_MMI_LOAD_MATRIX_COLUMN(vf10, sdfViewMatrix);
        effMiscAxisAngleToQuaternionVU(2.0f * 3.14159265f / 180.0f);
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, position);
        VU0_TRANSFORM_POINT(vf10, vf10);
        VU0_SET_W_ONE(vf10);
        VU0_STORE_VF(vf10, position);
    }
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021E618);

void mdlDrawViewerLabelWithPackedColor(s32 first, s32 second, s32 color, s32 variant) {
    s32 packedColor = color & 0xffffff;

    func_0011E080(mdlViewerControlState.unk08, first, second,
                  (mdlViewerControlState.unk04 == 0) ? -1 : variant, packedColor | 0x80000000, 1, packedColor);
}

typedef struct MdlValueEdit {
    u8 pad00[4];
    f32 *target;   /* 0x04 */
    s32 min;       /* 0x08 */
    s32 max;       /* 0x0C */
    u8 pad10[8];
} MdlValueEdit;

extern MdlValueEdit D_00367D58[];

/* Step the number being edited in table slot `index`: coarse steps (A) and fine steps (B),
   held-button repeat, wrapping from one end of [min, max] to the other on a fresh press. */
void mdlViewerStepEditedNumericValue(s32 index) {
    MdlValueEdit *edit = &D_00367D58[index];
    f32 *target = edit->target;
    s32 value = (s32)(*target + 0.5f);
    s32 stepA;
    s32 stepB;
    MdlPadState *pad;
    s32 min;
    s32 max;

    if (value < 1000) {
        stepA = 10;
        stepB = 1;
    } else if (value < 10000) {
        stepA = 100;
        stepB = 10;
    } else {
        stepA = 1000;
        stepB = 100;
    }
    min = edit->min;
    pad = mdlViewerControlState.pad;
    max = edit->max;
    if (pad->stepUpA & 0x80) {
        if (value < max) {
            value += stepA;
            if (value > max) {
                value = max;
            }
        } else {
            value = min;
        }
    } else if (pad->stepUpA & 2) {
        value += stepA;
        if (value > max) {
            value = max;
        }
    } else if (pad->stepDownA & 0x80) {
        if (min < value) {
            value -= stepA;
            if (value < min) {
                value = min;
            }
        } else {
            value = max;
        }
    } else if (pad->stepDownA & 2) {
        value -= stepA;
        if (value < min) {
            value = min;
        }
    } else if (pad->stepUpB & 0x80) {
        if (value < max) {
            value += stepB;
            if (value > max) {
                value = max;
            }
        } else {
            value = min;
        }
    } else if (pad->stepUpB & 2) {
        value += stepB;
        if (value > max) {
            value = max;
        }
    } else if (pad->stepDownB & 0x80) {
        if (min < value) {
            value -= stepB;
            if (value < min) {
                value = min;
            }
        } else {
            value = max;
        }
    } else if (pad->stepDownB & 2) {
        value -= stepB;
        if (value < min) {
            value = min;
        }
    } else {
        return;
    }
    *target = value;
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021ECF0);
INCLUDE_ASM(const s32, "game/code_00218B48", func_0021F098);



void mdlResetViewerFlagsAndSolarOverlay(void) {
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
    u32 *word = ((MdlFlagBank *)datGameState)->words;

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
    ((MdlFlagBank *)datGameState)->words[adjustedFlag >> 5] |= 1 << flag;
}

void mdlFlagClear(s32 flag) {
    s32 adjustedFlag = (flag < 0) ? flag + 0x1f : flag;
    ((MdlFlagBank *)datGameState)->words[adjustedFlag >> 5] &= ~(1 << flag);
}

s32 mdlFlagTest(s32 flag) {
    s32 adjustedFlag = (flag < 0) ? flag + 0x1f : flag;
    return (((s32)((MdlFlagBank *)datGameState)->words[adjustedFlag >> 5] >> flag) & 1);
}

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABF78);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABF88);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABF98);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABFA8);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021F630);

extern s32 dds3AdvanceWorldCounter(void);

extern s32 dds3CreateCameraObject(s32 counter, f32 *position, f32 *rotation);

extern void effObjSetInnerFloat(s32 object, f32 value);

extern void dds3EnsureSlotData();

extern s32 dds3GetWorldSecondaryObject(void);

extern void dds3SetWorldCameraObject(s32 world, s32 object);

extern void func_001127A0(s32 object, s32 arg);

extern void *memset(void *dst, s32 value, u32 size);

void mdlSpawnViewerWorldObject(void) {
    f32 position[4] = { 0.0f, -100.0f, -600.0f, 0.0f };
    f32 rotation[4];
    s32 object;

    memset(rotation, 0, 0x10);
    rotation[3] = 1.0f;
    object = dds3CreateCameraObject(dds3AdvanceWorldCounter(), position, rotation);
    effObjSetInnerFloat(object, 10.0f);
    dds3EnsureSlotData(object);
    dds3SetWorldCameraObject(dds3GetWorldSecondaryObject(), object);
    func_001127A0(object, 0);
}

extern void func_00111E30(s32 object, s32, s32);

extern s32 dds3SpawnCameraSlotObj5(s32 counter, f32 *position, f32 *rotation);

extern void dds3SetObjectFlags(s32 object, s32 flags);

extern s32 *dds3GetUnk0C(s32 object);

s32 mdlSpawnCameraSlotViewerObject(s32 slotKind, s32 resource) {
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

extern void *dds3FindWorldObjectNodeByKey(s32 world, s32 id, s32 kind);

extern void dds3SetSlotByKind(s32 object, s32 slot);

extern void dds3InvokeSlot5Handler(s32 object);

s32 mdlSpawnLinkedCameraSlotViewerObject(s32 slotKind, s32 resource) {
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
    dds3SetSlotByKind(object, (s32)dds3FindWorldObjectNodeByKey(dds3GetWorldSecondaryObject(), 0x10000, 2));
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


extern void effMiscQuatMultiplyVU(void);

void mdlAttachWorldObjectToSourceVector(s32 targetId, s32 sourceId) {
    f32 quaternion[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
    MdlAttachObj *target;
    MdlAttachSlot *source;
    u8 *base;

    target = (MdlAttachObj *)dds3FindWorldObjectNodeByKey(dds3GetWorldSecondaryObject(), targetId, 5);
    if (target != NULL) {
        source = (MdlAttachSlot *)dds3FindWorldObjectNodeByKey(dds3GetWorldSecondaryObject(), sourceId, 0x11);
        if (source != NULL) {
            base = source->firstVec;
            effObjSetInnerFirstVec(target, base);
                        VU0_LOAD_VF(vf10, quaternion);
            effMiscAxisAngleToQuaternionVU(3.14159265f);
            base += 0x10;
                        VU0_LOAD_VF(vf11, base);
            effMiscQuatMultiplyVU();
                        VU0_STORE_VF(vf10, quaternion);
            effObjSetInnerSecondVec(target, quaternion);
            effObjFetchInnerFirstVec(target);
            VU0_STORE_VF(vf10, target->inner + 0x70);
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

INCLUDE_SDATA(const s32, "game/code_00218B48", evtPendingEventSelection);

