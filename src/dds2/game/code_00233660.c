#include "common.h"

#include "pcp_vu0.h"
#include "ee_mmi.h"

extern s32 sdfCreateFormattedSifCommand();

extern void sdfReleaseChipBlock();
extern void *sdfAllocAndClearQuadwords(s32 size);

extern s32 D_003C88C0[];

extern s32 D_003C88C8[];

extern s32 kwlnTaskDestroyWithHierarchyByName(char *, s32);

extern void func_00103388(s32, s32, s32, s32);

extern s32 mdlHasNode(s32, s16);

extern void mdlAddEntryFlaggedEx(s32, s16, s16, f32, f32);

extern void mdlAddEntryPlainEx(s32, s16, s16, f32, f32);

extern s32 datGameState;

s32 billCreateIndexed(s32, s32);

s32 effCreateNodeFromDescriptor(s32);

/* Viewer-wide state for the model viewer task (DDS2 game/code_00233660 and
 * DDS1 game/code_00218B48 share this layout field for field). Fields that are
 * only written by a defaults initialiser and never read in either game are
 * left as unkNN on purpose: there is no read to earn a role from. */
typedef struct MdlViewState {
    s32 unk00;
    s32 viewerTask;
    s8 unk08;
    s8 unk09;
    s8 taskPhase; /* 0x0A: one-based index into D_003C87F0 */
    s8 unk0B;
    s8 unitStepMode;  /* 0x0C: toggled by the step button, read as a 0/2 step */
    s8 unitStepSign;  /* 0x0D: +1/-1, derived from the input keys */
    u8 unk0E;
    u8 unk0F;
    s8 yawStepMode; /* 0x10: one yaw step per left/right button press */
    u8 pad11[3];
    s16 unk14;
    s16 resourceCount;
    s16 unk18; /* 0x18: first s16 handed to func_00232198, and set from
                 func_00232EE8's return */
    s16 unk1A; /* 0x1A: second s16, and set from func_00232EF8 */
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 activeEntryId;   /* 0x22: record key most recently added to the viewer */
    u16 selectedEntryId; /* 0x24: key passed to mdlAddEntry* */
    s16 selectedNodeId;  /* 0x26: checked by mdlHasNode */
    s16 unk28;
    s16 unk2A;
    s16 unk2C;
    s16 entryHeight; /* 0x2E: low bound offered to mdlAddEntry* */
    s16 entryWidth;  /* 0x30: high bound, limited to entryHeight when drawing */
    u8 pad32[2];
    s16 labelIndexA; /* 0x34: indexes D_003C88C0 */
    s16 labelIndexB; /* 0x36: indexes D_003C88C8 */
    s16 scrollPage;  /* 0x38: page stepped by mdlUpdateViewerCursorWithPageStep */
    s16 nodeCursor; /* 0x3A: selection within the loaded node count */
    s16 unk3C;
    s16 unk3E;
    u8 pad40[2];
    s16 unk42;
    u8 pad44[4];
    s16 unk48;
    s16 unk4A;
    s16 unk4C;
    s16 unk4E;
    s16 unk50;
    s16 unk52;
    f32 viewerScale; /* 0x54: viewer zoom, in hundredths */
    u8 pad58[0x34];
    s32 slotBeforeResources[1];
    void *resources[1];
    u8 pad94[0x2C];
    s32 packetList; /* 0xC0: drawing packet destination */
} MdlViewState;

extern MdlViewState mdlViewerState;

typedef struct MdlCountNode {
    u8 pad00[4];
    s16 count; /* 0x04 */
} MdlCountNode;

typedef struct MdlLoadedInfo {
    u8 pad00[8];
    MdlCountNode *first; /* 0x08 */
} MdlLoadedInfo;

typedef struct MdlMotionState {
    u8 pad00[0x1C];
    f32 time;   /* 0x1C */
    u8 pad20[0xE];
    u16 length; /* 0x2E */
    u8 pad30[2];
    u8 reverse; /* 0x32 */
} MdlMotionState;

typedef struct MdlLoaded {
    u8 pad00[0x12];
    s16 unk12;
    u8 pad14[4];
    MdlLoadedInfo *info;    /* 0x18 */
    MdlMotionState *motion; /* 0x1C */
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

void mdlDrawViewerSelectionLabel(void);

extern s32 func_00101740(void *name);

extern void kwlnTaskDestroyWithHierarchy(s32 task, s32 flag);

void sdfPacInitializeDispatchPacket(void *buffer, s32);

void func_00346AD8(void *buffer);

void func_00346CF0(void *buffer, s32, s32);

void func_00233280(s32, s32, s32, s32);

void func_00346AF8(void *buffer);

s32 mdlCountRecords(s32);

extern u32 D_00435CBC;

extern void kwlnDebugGraphSetEnabled(s8 mode);

extern s32 fldStepColorChannelByPad(u32 *color, s32 channel, s8 *pad);

extern void fldAdjustIntegerUsingMainPad(void *ptr, s32 type, s64 min, s64 max, s64 step, s64 bigStep);

extern s8 D_0037F510[];

extern void fldStepIntByPad(void *ptr, s32 type, s64 min, s64 max, s64 small, s64 big, s8 *pad);


void sdfStreamCreateWithParams(s32, s32, s32, s32, s32);

void effApplyNodeScale(s32, float);

void billSetChildScaleComponents(s32, float, float);

s32 func_0011F250(s32, s32, s32, s32, s32, s32, s32);

s32 mdlBuildViewerRectanglePacket(s32, s32, s32, s32, s32);

extern s32 D_00453610[];

void sdfAppendPacket(s32, s32);

extern u128 D_00453620;

extern u128 D_00453630;

extern u128 D_00453640;

typedef struct {
    f32 x, y, z, w;
} __attribute__((aligned(16))) MdlEyeVec;

extern MdlEyeVec D_003C87C0;

extern MdlEyeVec D_003C87D0;

extern u128 D_003C87E0;

extern s16 D_00453584[];

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

extern MdlCtrlState mdlViewerControlState;

void func_0011FEE8(s32, s32, s32, s32, s32, s32, s32);

void mdlFlagClear(s32);

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

extern char D_004370C0[]; /* "%5.2f" */

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

extern u8 sdfViewTargetVector[];

extern u8 *sdfModelFindDrawNode(void *chunk, s32 id);

extern void effCopyVector(s32 handle, f32 *src);

extern void billInvokeCallback(s32 handle);

extern void effCopyVectorToNodeInstance(s32 handle, f32 *pos);

extern void effUpdateNode(s32 handle);

extern void effTrackPolyUpdate(s32 handle);

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
        func_00346AD8(buffer);
    }
    func_00346CF0(buffer, requestFirst, requestSecond);
    func_00233280(((MdlPackageRequest *)buffer)->handle, first, second, flags);
    func_00346AF8(buffer);
}

void func_00233700(void) {
    sdfReleaseChipBlock();
}

/* Three separately allocated resources per slot; their roles are not yet known. */
typedef struct MdlSlotEntry {
    s32 firstHandle;
    s32 secondHandle;
    s32 thirdHandle;
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

void mdlReleaseViewerSlotResources(void) {
    MdlSlotEntry *entry;
    s32 count;
    s32 i;

    count = D_003C6588[5].count;
    if (count > 0) {
        entry = D_003C6588[5].entries;
        i = 0;
        do {
            i++;
            sdfReleaseChipBlock(entry->secondHandle);
            sdfReleaseChipBlock(entry->firstHandle);
            sdfReleaseChipBlock(entry->thirdHandle);
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

/* Three-entry name table plus a small header, built from the first slot-table entry. */
typedef struct MdlViewerHeader {
    s16 kind;
    s16 unk02;
    s16 unk04;
    s16 unk06;
} MdlViewerHeader;

extern s32 sdfAllocGeneralBlock(s32 size);
extern void *func_00328D68(s32 size);
extern u32 sdfResourceRetainAddress(s32 handle);
extern u32 strlen(const char *);
extern char *strcpy(char *, const char *);
extern MdlViewerHeader *D_00438F98;
extern char **D_00438F9C;

void mdlInitializeViewerResourceTable(void) {
    s32 i;
    char *source;
    char *copy;

    mdlReleaseViewerSlotResources();
    D_00436FAC = sdfAllocGeneralBlock(8);
    D_00438F98 = (MdlViewerHeader *)sdfResourceRetainAddress(D_00436FAC);
    D_00438F98->kind = 5;
    D_00438F98->unk02 = 0;
    D_00438F98->unk04 = 0x1000;
    D_00438F98->unk06 = 0x3E8;
    D_00436FB0 = sdfAllocGeneralBlock(0xC);
    D_00438F9C = (char **)sdfResourceRetainAddress(D_00436FB0);
    for (i = 0; i != 3; i++) {
        switch (i) {
        case 0:
            source = (char *)D_003C6588[0].entries->secondHandle;
            break;
        case 1:
            source = (char *)D_003C6588[0].entries->firstHandle;
            break;
        default:
            source = (char *)D_003C6588[0].entries->thirdHandle;
            break;
        }
        if (source != NULL) {
            copy = func_00328D68(strlen(source) + 1);
            strcpy(copy, source);
            switch (i) {
            case 0:
                D_00438F9C[1] = copy;
                break;
            case 1:
                D_00438F9C[0] = copy;
                break;
            case 2:
                D_00438F9C[2] = copy;
                break;
            }
        }
    }
    D_003C86B0[5].entries = (MdlSlotEntry *)D_00438F98;
    D_003C86B0[5].count = 1;
    D_003C6588[5].entries = (MdlSlotEntry *)D_00438F9C;
    D_003C6588[5].count = 1;
}

INCLUDE_ASM(const s32, "game/code_00233660", func_00233938);

s32 mdlSetViewerSlotResourceHandles(s32 table, s32 slot, s32 firstHandle, s32 secondHandle, s32 thirdHandle) {
    MdlSlotEntry *entries = D_003C6588[table].entries;

    if (entries == NULL) {
        return 0;
    }
    if (slot >= D_003C6588[table].count) {
        return 0;
    }
    entries[slot].firstHandle = firstHandle;
    entries[slot].secondHandle = secondHandle;
    entries[slot].thirdHandle = thirdHandle;
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
    s32 entryBase; /* 0x0C: base of 0x10-byte slot entries */
} MdlViewerSlots;

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

/* Read the model record's payload word without advancing its relative link. */
u32 mdlGetViewerRecordPayloadWord(MdlRecord *record) {
    return record->value08;
}

u16 mdlGetViewerRecordListCount(MdlRecord *record) {
    return record->value0C;
}

/* Follow relative links in the resource's record table to find an ID. */
s32 *mdlFindViewerRecord(MdlViewerResource *resource, s32 key) {
    s32 *list = resource->data->records;
    s32 *entry;
    s32 remaining;

    if (list == NULL) {
        return NULL;
    }
    entry = list;
    remaining = mdlGetViewerRecordListCount((MdlRecord *)entry);
    while (1) {
        remaining--;
        entry = (s32 *)((u8 *)entry + ((MdlRecord *)entry)->nextOffset);
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

    sdfAppendPacket(list, sdfCreateFormattedSifCommand(labelX, y, z, 0, "MARK0:%d", params->mark0));
    sdfAppendPacket(list, sdfCreateFormattedSifCommand(labelX, y + 0x60, z, 0, "MARK1:%d", params->mark1));
    sdfAppendPacket(list, sdfCreateFormattedSifCommand(labelX, y + 0xC0, z, 0, "START:%d", params->start));
    sdfAppendPacket(list, sdfCreateFormattedSifCommand(labelX, y + 0x120, z, 0, "END  :%d", params->end));
    sdfAppendPacket(list, sdfCreateFormattedSifCommand(labelX, y + 0x180, z, 0, "ITRVL:%d", params->interval));
    sdfAppendPacket(list, sdfCreateFormattedSifCommand(labelX, y + 0x1E0, z, 0, "FACE :%d", params->face));
    sdfAppendPacket(list, sdfCreateFormattedSifCommand(labelX, y + 0x240, z, 0, "BLEND:%d", params->blend));
    for (row = 0; row != 4; row++) {
        color = params->colors[row];
        sdfAppendPacket(list, func_0011F250(x + 0x480, boxY, z, 0x300, 0x180, (color & 0xFFFFFF) | 0x80000000, 0x60404040));
        for (col = 0; col != 4; col++) {
            sdfAppendPacket(list, sdfCreateFormattedSifCommand(labelX, boxY, z, D_003C8760[col], D_003C8768[col]));
            sdfAppendPacket(list, sdfCreateFormattedSifCommand(x + 0x240, boxY, z, 0, D_00436FF0, color & 0xFF));
            color >>= 8;
            boxY += 0x60;
        }
        boxY += 0x60;
    }
    if (selected >= 0) {
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y + D_003C8730[selected], z, 0, D_00436FF8));
    }
}

/* Edit the marker parameters with the pad: rows 0-6 are the scalar fields, 7-22 the color bytes. */
void mdlEditMarkParametersWithPad(EffMarkParams *params, s16 *cursor) {
    s32 selected = *cursor;
    u8 *channel;

    if (D_0037F510[0x27] < 0) {
        selected = selected != 0x16 ? selected + 1 : 0;
    } else if (((u8)D_0037F510[0x27] & 2) != 0 && selected < 0x16) {
        selected++;
    } else if (D_0037F510[0x26] < 0) {
        selected = selected != 0 ? selected - 1 : 0x16;
    } else if (((u8)D_0037F510[0x26] & 2) != 0 && selected > 0) {
        selected--;
    } else {
        switch (selected) {
        case 0:
            fldStepIntByPad(&params->mark0, 4, 0, 99999, 0, 1, D_0037F510);
            break;
        case 1:
            fldStepIntByPad(&params->mark1, 4, 0, 99999, 0, 1, D_0037F510);
            break;
        case 2:
            fldStepIntByPad(&params->start, 2, 0, 9999, 0, 1, D_0037F510);
            break;
        case 3:
            fldStepIntByPad(&params->end, 2, 0, 9999, 0, 1, D_0037F510);
            break;
        case 4:
            fldStepIntByPad(&params->interval, 2, 1, 10, 0, 1, D_0037F510);
            break;
        case 5:
            fldStepIntByPad(&params->face, 1, 1, 10, 0, 1, D_0037F510);
            break;
        case 6:
            fldStepIntByPad(&params->blend, 1, 0, 4, 0, 1, D_0037F510);
            break;
        default:
            channel = (u8 *)params->colors + selected - 7;
            if (D_0037F510[0x25] < 0) {
                *channel += 1;
            } else if (((u8)D_0037F510[0x25] & 2) != 0 && *channel < 0xFF) {
                *channel += 1;
            } else if (D_0037F510[0x24] < 0) {
                *channel -= 1;
            } else if (((u8)D_0037F510[0x24] & 2) != 0 && *channel != 0) {
                *channel -= 1;
            }
            break;
        }
    }
    *cursor = selected;
}

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

void mdlObjDestroy(MdlObj *obj) {
    if (obj->initialized != 0) {
        func_00344A08(obj->data);
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

void func_00234838(u32 request) {
    sdfDevCreateBufferedRequest(request, 0x10, 4);
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

s32 mdlFindViewerPartSlot(MdlViewerResource *resource, s32 index) {
    s32 slotTable;

    slotTable = resource->data->slotTable;
    if (slotTable == 0) {
        return 0;
    }
    if (index >= ((MdlViewerSlots *)slotTable)->count) {
        return 0;
    }
    return ((MdlViewerSlots *)slotTable)->entryBase + index * 0x10;
}

typedef struct MdlPartRec {
    u8 pad00[4];
    u32 size;      /* 0x04 */
    s32 firstId;   /* 0x08 */
    u16 count;     /* 0x0C */
    u16 partIndex; /* 0x0E */
    f32 value;     /* 0x10 */
} MdlPartRec;

typedef struct MdlPartItem {
    u8 pad00[8];
    s32 handle;         /* 0x08 */
    MdlPartEntry *part; /* 0x0C */
    void *record;       /* 0x10 */
    f32 value;          /* 0x14 */
} MdlPartItem;

extern void *sdfChunkFindRecordById(void *chunk, s32 id);

s32 mdlBindViewerPartRecords(MdlResourceOwner *owner, MdlPartRec *rec, s32 option, s32 type, s32 (*create)(MdlPartEntry *)) {
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
    s32 effectId; /* 0x08 */
    s32 param0C;  /* 0x0C */
    u16 scaleX;   /* 0x10 */
    u16 scaleY;   /* 0x12 */
    u16 value14;  /* 0x14 */
    u8 value16;   /* 0x16 */
    u8 value17;   /* 0x17 */
    s32 value18;  /* 0x18 */
    s32 value1C;  /* 0x1C */
    s32 value20;  /* 0x20 */
    s32 value24;  /* 0x24 */
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

s32 mdlCreateViewerEffectPart(MdlResourceOwner *owner, MdlEffectRec *rec, s32 option) {
    MdlResourceItem *item;
    MdlEffectParams params;

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
    return item->resource = effTrackPolyCreateWork(&params);
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

s32 mdlLoadViewerStreamRecord(u32 owner, s32 record) {
    func_00231E28(owner, ((MdlStreamRecord *)record)->selectorA,
                  ((MdlStreamRecord *)record)->selectorB,
                  ((MdlStreamRecord *)record)->value0C,
                  ((MdlStreamRecord *)record)->value10);
}

/* Same conditional object-entry offsets as the DDS1 model viewer. */
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
    s32 dataId; /* 0x08 */
    s32 param;  /* 0x0C */
    u8 flagB;   /* 0x10 */
    u8 flagA;   /* 0x11 */
    u16 index;  /* 0x12 */
} MdlEntryRec;

typedef struct MdlSlotRec {
    u8 pad00[8];
    MdlObj *obj; /* 0x08 */
} MdlSlotRec;

extern void *sdfFindResourceById(s32 id);

s32 mdlClaimViewerObjectPart(MdlResourceOwner *owner, MdlEntryRec *entry, s32 option) {
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
        mdlObjInit((MdlObj *)object, ((MdlObjItem *)entry)->data, entry + 0x18);
    }
}

extern s32 mdlBindViewerPartRecords(MdlResourceOwner *object, MdlPartRec *record, s32 option, s32 type, s32 (*advance)(MdlPartEntry *));

extern s32 mdlClaimViewerObjectPart(MdlResourceOwner *object, MdlEntryRec *record, s32 option);

s32 mdlDispatchResourceEntry(s32 object, s32 *record, s32 option) {
    switch (*record) {
    case 1:
        return mdlBindViewerPartRecords(object, record, option, 0, mdlAdvanceBillboardPart);
    case 2:
        return mdlBindViewerPartRecords(object, record, option, 1, mdlAdvanceEffectPart);
    case 3:
        return mdlCreateViewerEffectPart(object, record, option);
    case 4:
        return mdlLoadViewerStreamRecord(object, record);
    case 5:
        mdlClaimViewerObjectPart(object, record, option);
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

/* vu0 routine: out = p + normalize(p - D_00324690) * scale, p = node position transformed by the node matrix */
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
    return func_0011F250(x, y, depth, width, height, 0x30000000, 0x60404040);
}

void mdlAppendViewerRectToDrawList(s32 x, s32 y, s32 depth, s32 width, s32 height, s32 unused) {
    s32 packet;

    packet = D_00453610[0];
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

extern MdlLoaded *func_00232198(s16 a, s16 b);

extern void mdlAddEntryFlagged(MdlLoaded *loaded, s32 a, s32 b);

void mdlLoadViewerResourceAndResetCursors(void) {
    MdlLoaded *loaded;

    loaded = func_00232198(mdlViewerState.unk18, mdlViewerState.unk1A);
    mdlViewerState.resources[0] = loaded;
    mdlViewerState.activeEntryId = 0;
    mdlViewerState.selectedEntryId = 0;
    mdlViewerState.selectedNodeId = 0;
    mdlViewerState.unk28 = 0;
    mdlViewerState.unk2A = 0;
    mdlViewerState.unk2C = 0;
    if (loaded->motion != NULL) {
        mdlAddEntryFlagged(loaded, 0, 0);
    }
    mdlViewerState.nodeCursor = 0;
}

void mdlResetViewerBasisVectors(void) {
    PCP_COPY_VECTOR(&D_00453620, &D_003C87C0);
    PCP_COPY_VECTOR(&D_00453630, &D_003C87D0);
    PCP_COPY_VECTOR(&D_00453640, &D_003C87E0);
    D_00453584[0] = 0;
}

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
    MdlViewState *state = &mdlViewerState;
    f32 width;
    f32 height;

    if (mdlHasNode(state->resources[0], state->selectedNodeId) == 0) {
        return;
    }
    height = (f32)state->entryHeight;
    width = (f32)state->entryWidth;
    if (height < width) {
        width = height;
    }
    state->activeEntryId = state->selectedEntryId;
    mdlAddEntryFlaggedEx(state->resources[0], state->selectedNodeId, state->selectedEntryId, width, height);
}

void mdlAddPlainViewerEntryForSelectedNode(void) {
    MdlViewState *state = &mdlViewerState;
    f32 width;
    f32 height;

    if (mdlHasNode(state->resources[0], state->selectedNodeId) == 0) {
        return;
    }
    height = (f32)state->entryHeight;
    width = (f32)state->entryWidth;
    if (height < width) {
        width = height;
    }
    state->activeEntryId = state->selectedEntryId;
    mdlAddEntryPlainEx(state->resources[0], state->selectedNodeId, state->selectedEntryId, width, height);
}

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421238);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421248);

INCLUDE_ASM(const s32, "game/code_00233660", func_00235970);

INCLUDE_ASM(const s32, "game/code_00233660", func_00235C20);

u32 mdlRunViewerAssetSelectionTask(void) {
    func_00235970();
    func_00235C20();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00233660", func_00236080);

void mdlDrawViewerIndexedLabelOverlay(void) {
    s32 packets;

    mdlAppendViewerRectToDrawList(0x8A10L, 0x7948, 0xFF007F, 0x4E0, 0x90, 0);
    packets = mdlViewerState.packetList;
    sdfAppendPacket(packets, sdfCreateFormattedSifCommand(0x8A40L, 0x7960, 0xFF0080, 0, D_003C88C0[mdlViewerState.labelIndexA]));
}

u32 mdlRunViewerIndexedLabelTask(void) {
    func_00236080();
    mdlDrawViewerIndexedLabelOverlay();
    return 0;
}

typedef struct MdlCtx MdlCtx;
extern void mdlLoadPrimaryVectorVU(MdlCtx *context);
extern void mdlStorePrimaryVectorVU(MdlCtx *context);
extern void mdlLoadSecondaryVectorVU(MdlCtx *context);
extern void mdlUpdateContextRotationBasisFromQuaternion(MdlCtx *context);
extern void effMiscAxisAngleToQuaternionVf11(f32 angle);
extern void effMiscQuatMultiplyVU(void);
extern f32 sdfViewMatrix[];
extern f32 D_0040B530[4];
extern f32 D_0040B540[4];
extern f32 D_0040B550[4];

/* vu0 routine: edit the viewer transform with translation or quaternion steps. */
void func_00236568(void) {
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
            VU0_LOAD_VF(vf11, D_0040B540);
            angle = 5.0f * 3.14159265f / 180.0f;
        } else if (sdfPadButtonStates[4] < 0) {
            VU0_LOAD_VF(vf11, D_0040B540);
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
            VU0_LOAD_VF(vf11, D_0040B540);
            effMiscAxisAngleToQuaternionVf11(speed);
            effMiscQuatMultiplyVU();
        } else if (sdfPadButtonStates[4] != 0) {
            VU0_LOAD_VF(vf11, D_0040B540);
            effMiscAxisAngleToQuaternionVf11(-speed);
            effMiscQuatMultiplyVU();
        }
        if (sdfPadButtonStates[6] != 0) {
            VU0_LOAD_VF(vf11, D_0040B530);
            effMiscAxisAngleToQuaternionVf11(speed);
            effMiscQuatMultiplyVU();
        } else if (sdfPadButtonStates[7] != 0) {
            VU0_LOAD_VF(vf11, D_0040B530);
            effMiscAxisAngleToQuaternionVf11(-speed);
            effMiscQuatMultiplyVU();
        }
        if (sdfPadButtonStates[9] != 0) {
            VU0_LOAD_VF(vf11, D_0040B550);
            effMiscAxisAngleToQuaternionVf11(speed);
            effMiscQuatMultiplyVU();
        } else if (sdfPadButtonStates[8] != 0) {
            VU0_LOAD_VF(vf11, D_0040B550);
            effMiscAxisAngleToQuaternionVf11(-speed);
            effMiscQuatMultiplyVU();
        }
        mdlUpdateContextRotationBasisFromQuaternion((MdlCtx *)mdlViewerState.resources[0]);
    }
}

void mdlDrawViewerSelectionLabel(void) {
    s32 packets;

    mdlAppendViewerRectToDrawList(0x8A10L, 0x7948, 0xFF007F, 0x4E0, 0x90, 0);
    packets = mdlViewerState.packetList;
    sdfAppendPacket(packets, sdfCreateFormattedSifCommand(0x8A40L, 0x7960, 0xFF0080, 0, D_003C88C8[mdlViewerState.labelIndexB]));
}

s32 mdlRunViewerSelectionLabelTask(void) {
    func_00236568();
    if (D_00453560[0] == 0) {
        mdlDrawViewerSelectionLabel();
    }
    return 0;
}

void mdlAdjustViewerScale(void) {
    s32 value = (s32)(mdlViewerState.viewerScale * 100.0f + 0.5f);

    if (D_0037F510[0x27] & 2) {
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
    } else if (D_0037F510[0x26] & 2) {
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
    if (D_0037F510[0x23] < 0) {
        mdlViewerState.unitStepMode ^= 1;
    }
    mdlViewerState.unitStepSign = 0;
    if (mdlViewerState.unitStepMode != 0) {
        if (D_0037F510[0x25] != 0) {
            mdlViewerState.unitStepSign = 1;
        } else if (D_0037F510[0x24] != 0) {
            mdlViewerState.unitStepSign = -1;
        }
    }
}

extern void sdfAppendFillRectanglePacket();

/* Draw the motion progress bar: timeline frame, playhead marker and "[time/length]" label, then the zoom value. */
void mdlDrawViewerMotionTimeline(void) {
    s32 list;
    MdlLoaded *resource;
    MdlMotionState *motion;
    s32 x;
    s32 step;

    mdlAppendViewerRectToDrawList(0x81D0, 0x7948, 0xFF007F, 0xD20, 0xF0, 0);
    list = mdlViewerState.packetList;
    sdfAppendFillRectanglePacket(list, 0x80303030, 0, 0x8200, 0x7990, 0x8EC0, 0x7990, 0xFF0080, 0);
    sdfAppendFillRectanglePacket(list, 0x80303030, 0, 0x8200, 0x7960, 0x8200, 0x79C0, 0xFF0080, 0);
    sdfAppendFillRectanglePacket(list, 0x80303030, 0, 0x8EC0, 0x7960, 0x8EC0, 0x79C0, 0xFF0080, 0);
    resource = (MdlLoaded *)mdlViewerState.resources[0];
    motion = resource->motion;
    if (motion != NULL) {
        x = (s32)(motion->time * 3264.0f / motion->length);
        if (x < 0) {
            x = 0;
        }
        x += 0x8200;
        sdfAppendFillRectanglePacket(list, 0x800000E0, 0, x, 0x7960, x, 0x79C0, 0xFF0090, 0);
        sdfAppendPacket(mdlViewerState.packetList, sdfCreateFormattedSifCommand(0x8200, 0x79C0, 0xFF0080, 0, "[%5.1f/%-3d]", motion->time, motion->length));
    } else {
        sdfAppendPacket(mdlViewerState.packetList, sdfCreateFormattedSifCommand(0x8200, 0x79C0, 0xFF0080, 0, "[---.-/---]"));
    }
    step = mdlViewerState.unitStepMode != 0 ? 2 : 0;
    sdfAppendPacket(mdlViewerState.packetList, sdfCreateFormattedSifCommand(0x8B00, 0x79C0, 0xFF0080, step, D_004370C0, mdlViewerState.viewerScale));
}

u32 mdlUpdateViewerScaleTask(void) {
    mdlAdjustViewerScale();
    mdlDrawViewerMotionTimeline();
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

extern s32 func_00232EE8(s32);
extern s32 func_00232EF8(s32);
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
            motion = ((MdlLoaded *)item)->motion;
            if (motion != NULL) {
                item = ((MdlLoaded *)item)->unk12;
                if (motion->reverse == 0) {
                    sdfMotionInitializeAtZeroTime(motion, item, 0);
                } else {
                    sdfMotionInitializeAtZeroTime(motion, item, 1);
                }
            }
        }
        break;
    }
    mdlViewerState.unk1C = mdlViewerState.unk18 = func_00232EE8(mdlViewerState.resources[0]);
    mdlViewerState.unk1E = mdlViewerState.unk1A = func_00232EF8(mdlViewerState.resources[0]);
    i = ((MdlLoaded *)mdlViewerState.resources[0])->unk12;
    if (i < 0) {
        i = 0;
    }
    mdlViewerState.selectedEntryId = mdlViewerState.activeEntryId = i;
}

INCLUDE_ASM(const s32, "game/code_00233660", func_00237088);

u32 mdlRunViewerResourceMenuTask(void) {
    mdlApplyViewerResourceMenuAction();
    func_00237088();
    return 0;
}

void mdlHandleViewerNodeCursorInput(void) {
    MdlCountNode *node;

    node = ((MdlLoaded *)mdlViewerState.resources[0])->info->first;
    if (node != NULL) {
        if (node->count > 0) {
            mdlUpdateViewerCursor(&mdlViewerState.nodeCursor, node->count);
        }
    }
}

INCLUDE_RODATA(const s32, "game/code_00233660", D_004212C8);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004212D8);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004212E8);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004212F8);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421308);

INCLUDE_ASM(const s32, "game/code_00233660", func_00237258);

u32 mdlUpdateViewerNodeCursorTask(void) {
    mdlHandleViewerNodeCursorInput();
    func_00237258();
    return 0;
}

s32 mdlIsDebugTimeGraph(void) {
    return func_00101740("DebugTimeGrph") != 0;
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
            PCP_COPY_VECTOR(&D_003C87C0, &D_00453620);
            PCP_COPY_VECTOR(&D_003C87D0, &D_00453630);
            func_00238BD8();
            break;
        }
        break;
    case 1:
        fldStepColorChannelByPad(&D_00435CBC, mdlViewerState.unk3C - 3, sdfPadButtonStates);
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

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421388);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004213A0);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004213B8);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004213D0);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004213E0);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004213F0);

INCLUDE_ASM(const s32, "game/code_00233660", func_002376F0);

u32 mdlRunViewerSettingsTask(void) {
    mdlUpdateViewerSettingsInput();
    func_002376F0();
    return 0;
}

s32 mdlCountActiveRecords(void) {
    MdlViewerResource *resource = mdlViewerState.resources[0];
    s32 first = mdlCountRecords((s32)mdlFindViewerRecord(resource, -1));
    s32 second = mdlCountRecords((s32)mdlFindViewerRecord(resource, mdlViewerState.activeEntryId));

    return first + second;
}

INCLUDE_ASM(const s32, "game/code_00233660", func_00237A70);

INCLUDE_ASM(const s32, "game/code_00233660", func_00237B30);

INCLUDE_ASM(const s32, "game/code_00233660", func_00237D08);

u32 mdlRunViewerEffectEditorTask(void) {
    func_00237B30();
    func_00237D08();
    return 0;
}

extern void dds3AdminSetControlFlag();

s32 mdlRequestViewerExitOnce(void) {
    if (mdlViewerState.unk08 == 0) {
        mdlViewerState.unk08 = 1;
        mdlCleanupViewerTasksAndResources();
        dds3AdminSetControlFlag();
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

void mdlSubmitViewerIntermediateDrawPacket(void) {
    s32 list;

    if (mdlViewerState.taskPhase < 4) {
        if (mdlViewerState.taskPhase >= 2) {
            list = sdfCreateResetPacketList();
            VU0_SET_UNIT_MATRIX(vf28, vf29, vf30, vf31);
            sdfAppendPacket(list, func_00348188(D_003C8930, D_003C8990, 6, 0x80));
            D_00380048.submit(&D_00380048, list);
        }
    }
}

extern s32 D_004370F8;

extern u8 D_003C89B0[];

extern u8 D_003C89F0[];

extern s32 func_00343188(s32, s32);

void mdlSubmitViewerResourceDrawPacket(void) {
    s32 list;
    s32 packet;
    MdlResourceOwner *resource;

    if (mdlViewerState.taskPhase == 3) {
        list = sdfCreateResetPacketList();
        resource = (MdlResourceOwner *)mdlViewerState.resources[0];
        VU0_LOAD_MATRIX(resource->chunk + 0x20);
        packet = func_00348188(D_003C89B0, D_003C89F0, 4, 0x80);
        if ((sdfPadButtonStates[13] < 0) & (D_004370F8 == 0)) {
            D_004370F8 = 1;
            func_00343188(packet, 0x100);
        }
        sdfAppendPacket(list, packet);
        D_00380048.submit(&D_00380048, list);
    }
}

void mdlViewerTaskDestroy(void) {
    if (mdlViewerState.viewerTask != 0) {
        kwlnTaskDestroyWithHierarchy(mdlViewerState.viewerTask, 0);
        mdlViewerState.viewerTask = 0;
    }
}

typedef struct MdlViewerTaskDef {
    const char *name;
    void *update;
    s32 data;
} MdlViewerTaskDef;

extern MdlViewerTaskDef D_003C87F0[];

extern s32 kwlnTaskCreate(const char *name, s32 id, s32, s32, void *update, void *destroy, s32 data);

extern void func_00101968(s32, s32);

void mdlRestartViewerPhaseTask(void) {
    mdlViewerTaskDestroy();
    mdlViewerState.viewerTask =
        kwlnTaskCreate(D_003C87F0[mdlViewerState.taskPhase - 1].name, 0x2B00, 1, 0,
                       D_003C87F0[mdlViewerState.taskPhase - 1].update, 0,
                       D_003C87F0[mdlViewerState.taskPhase - 1].data);
    func_00101968(mdlViewerState.unk00, mdlViewerState.viewerTask);
}

INCLUDE_ASM(const s32, "game/code_00233660", mdlViewer);

void mdlDrawMapPositionRecords(u8 *obj) {
    s32 count = sdfCountMapPositionRecords((s32)((MdlResourceOwner *)obj)->chunk);

    if (count > 0) {
        s32 i = 0;
        s32 list = sdfCreateResetPacketList();
        s32 record = sdfChunkFindByTag((s32)((MdlResourceOwner *)obj)->chunk, 0x534F504D) + 0x10;

        do {
            s32 current = record;

            i++;
            record += 0x40;
            sdfSetLookAtBasisFromRecord((s32)((MdlResourceOwner *)obj)->chunk, current);
            sdfAppendPacket(list, func_00348188(D_003C8A00, D_003C8A60, 6, 0x80));
        } while (i != count);
        D_00380048.submit(&D_00380048, list);
    }
}

INCLUDE_ASM(const s32, "game/code_00233660", mdlViewerEnd);

typedef struct MdlFogParams {
    f32 near;  /* 0x00 */
    f32 farA;  /* 0x04 */
    f32 value; /* 0x08 */
    f32 farB;  /* 0x0C */
    u32 color; /* 0x10 */
} MdlFogParams;

extern char D_003C88A8[];
extern char D_00437100[]; /* "%x" */
extern char D_00437108[]; /* "fovy=" */
extern char D_00437110[]; /* "%f" */
extern char D_00437118[]; /* "fog=" */
extern MdlFogParams kwlnDrawVector;
extern f32 sdfSceneProjectionParameters[];
extern s32 sdfPathExists(char *path);
extern s32 func_002C80C8(char *path);
extern void fileWaitReady(s32 file);
extern s32 fileGetResourceHandle(s32 file);
extern char *fileGetLoadedDataAddress(s32 file);
extern s32 fileGetResourceSize(s32 file);
extern void filePollEntryCleanup(s32 file);
extern s32 func_0035C8F8();
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

    if (sdfPathExists(D_003C88A8) == 0) {
        return;
    }
    file = func_002C80C8(D_003C88A8);
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
            if (func_0035C8F8(line + 9, D_00437100, &color) == 1) {
                D_00435CBC = color;
            }
        } else if (memcmp(line, "eye-position=", 13) == 0) {
            if (func_0035C8F8(line + 13, "%f,%f,%f", &x, &y, &z) == 3) {
                D_003C87C0.x = x;
                D_003C87C0.y = y;
                D_003C87C0.z = z;
            }
        } else if (memcmp(line, "target-position=", 16) == 0) {
            if (func_0035C8F8(line + 16, "%f,%f,%f", &x, &y, &z) == 3) {
                D_003C87D0.x = x;
                D_003C87D0.y = y;
                D_003C87D0.z = z;
            }
        } else if (memcmp(line, D_00437108, 5) == 0) {
            if (func_0035C8F8(line + 5, D_00437110, &fovy) == 1) {
                sdfSceneProjectionParameters[3] = fovy;
            }
        } else if (memcmp(line, D_00437118, 4) == 0) {
            if (func_0035C8F8(line + 4, "%d,%f,%d,%f,%x", &fogNear, &fogValue, &fogFar, &fogFarB, &color) == 5) {
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

INCLUDE_ASM(const s32, "game/code_00233660", func_00238BD8);

INCLUDE_ASM(const s32, "game/code_00233660", func_00238D38);

void mdlFreeViewResources(void) {
    MdlViewState *state = &mdlViewerState;
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

void mdlCleanupViewerTasksAndResources(void) {
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

typedef struct MdlRotationContext {
    u8 pad00[8];
    MdlPadState *pad;
} MdlRotationContext;

extern void effMiscAxisAngleToQuaternionVU(f32 angle);
extern void effMiscQuaternionToMatrixVU(void);

/* vu0 routine: rotate a viewer position about the camera axes selected by the pad. */
void func_00238FC0(MdlRotationContext *context, f32 *position)
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

INCLUDE_ASM(const s32, "game/code_00233660", func_00239188);

void mdlDrawViewerLabelWithPackedColor(s32 first, s32 second, s32 color, s32 variant) {
    s32 packedColor = color & 0xffffff;

    func_0011FEE8(mdlViewerControlState.unk08, first, second,
                  (mdlViewerControlState.unk04 == 0) ? -1 : variant, packedColor | 0x80000000, 1, packedColor);
}

typedef struct MdlValueEdit {
    u8 pad00[4];
    f32 *target;   /* 0x04 */
    s32 min;       /* 0x08 */
    s32 max;       /* 0x0C */
    u8 pad10[8];
} MdlValueEdit;

extern MdlValueEdit D_003C8B08[];

/* Step the number being edited in table slot `index`: coarse steps (A) and fine steps (B),
   held-button repeat, wrapping from one end of [min, max] to the other on a fresh press. */
void mdlViewerStepEditedNumericValue(s32 index) {
    MdlValueEdit *edit = &D_003C8B08[index];
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

INCLUDE_ASM(const s32, "game/code_00233660", func_00239860);
INCLUDE_ASM(const s32, "game/code_00233660", func_00239C08);



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
    u32 *word;
    s32 remaining;

    remaining = 0x7f;
    word = ((MdlFlagBank *)datGameState)->words;
    do {
        remaining = remaining - 1;
        *word = 0;
        word = word + 1;
    } while (-1 < remaining);
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

INCLUDE_RODATA(const s32, "game/code_00233660", D_004214E8);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004214F8);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421508);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421518);

INCLUDE_ASM(const s32, "game/code_00233660", func_0023A1A0);

extern s32 dds3CreateCameraObject(s32 world, f32 *pos, f32 *rot);

extern void dds3EnsureSlotData(s32 obj);

extern void dds3SetWorldCameraObject(s32 world, s32 obj);

s32 mdlSpawnViewerWorldObject(void) {
    f32 pos[4] = {0.0f, -100.0f, -600.0f, 0.0f};
    f32 rot[4];
    s32 obj;

    memset(rot, 0, 0x10);
    rot[3] = 1.0f;
    obj = dds3CreateCameraObject(dds3AdvanceWorldCounter(), pos, rot);
    effObjSetInnerFloat(obj, 10.0f);
    dds3EnsureSlotData(obj);
    dds3SetWorldCameraObject(dds3GetWorldSecondaryObject(), obj);
    func_001129C8(obj, 0);
}

s32 mdlSpawnCameraSlotViewerObject(s32 slotKind, s32 resource) {
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
    func_00112058(obj, slotKind, resource);
    if (dds3GetUnk0C(obj)->motion != NULL) {
        mdlAddEntryFlagged(dds3GetUnk0C(obj), 0, 0);
    }
    func_001129C8(obj, 0);
    return world;
}

extern s32 dds3FindWorldObjectNodeByKey(s32 world, s32 a, s32 b);

extern void dds3SetSlotByKind(s32 obj, s32 slot);

extern void dds3InvokeSlot5Handler(s32 obj);

s32 mdlSpawnLinkedCameraSlotViewerObject(s32 slotKind, s32 resource) {
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
    func_00112058(obj, slotKind, resource);
    mdlAddEntryFlagged(dds3GetUnk0C(obj), 0, 0);
    dds3SetSlotByKind(obj, dds3FindWorldObjectNodeByKey(dds3GetWorldSecondaryObject(), 0x10000, 2));
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

extern s32 dds3FindWorldObjectNodeByKey(s32 world, s32 id, s32 kind);

extern void effObjSetInnerFirstVec(MdlAimObj *obj, u8 *vec);

extern void effObjSetInnerSecondVec(MdlAimObj *obj, f32 *vec);

extern void effObjFetchInnerFirstVec(MdlAimObj *obj);


extern void effMiscQuatMultiplyVU();

void mdlAttachWorldObjectToSourceVector(s32 firstId, s32 secondId) {
    f32 axis[4] = {0.0f, 1.0f, 0.0f, 1.0f};
    MdlAimObj *obj;
    MdlAimSrc *src;
    u8 *vec;

    obj = (MdlAimObj *)dds3FindWorldObjectNodeByKey(dds3GetWorldSecondaryObject(), firstId, 5);
    if (obj != NULL) {
        src = (MdlAimSrc *)dds3FindWorldObjectNodeByKey(dds3GetWorldSecondaryObject(), secondId, 0x11);
        if (src != NULL) {
            vec = src->vecs;
            effObjSetInnerFirstVec(obj, vec);
            VU0_LOAD_VF(vf10, axis);
            effMiscAxisAngleToQuaternionVU(3.14159265f);
            vec += 0x10;
            VU0_LOAD_VF(vf11, vec);
            effMiscQuatMultiplyVU();
            VU0_STORE_VF(vf10, axis);
            effObjSetInnerSecondVec(obj, axis);
            effObjFetchInnerFirstVec(obj);
            VU0_STORE_VF(vf10, obj->inner + 0x70);
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

INCLUDE_SDATA(const s32, "game/code_00233660", evtPendingEventSelection);

