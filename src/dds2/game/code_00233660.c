#include "common.h"

#include "pcp_vu0.h"
#include "ee_mmi.h"

extern void *sdfCreateFormattedSifCommand(s32 source, s32 end, s32 argument, s32 index, const char *format, ...);

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

#define MDL_VIEWER_RESOURCE_SLOTS 12
#define MDL_VIEWER_TABLE_SLOT 5
#define MDL_VIEWER_LABEL_COUNT 3
#define MDL_VIEWER_HEADER_BYTES 8
#define MDL_VIEWER_NAME_TABLE_BYTES 0xC
#define MDL_RECORD_LIST_HEADER_BYTES 8
#define MDL_RECORD_END_KIND 0xFFFF
#define MDL_MARK_COLOR_COUNT 4
#define MDL_COLOR_CHANNEL_COUNT 4
#define MDL_COLOR_BYTE_MASK 0xFF
#define MDL_COLOR_BYTE_BITS 8
#define MDL_MARK_LAST_FIELD 0x16
#define MDL_MARK_COLOR_FIRST_FIELD 7
#define MDL_PAD_REPEAT_FLAG 2
#define MDL_PART_SLOT_BYTES 0x10
#define MDL_PART_VALUE_SIZE_THRESHOLD 0x11
#define MDL_RESOURCE_BILLBOARD 0
#define MDL_RESOURCE_EFFECT 1
#define MDL_RESOURCE_TRACK_POLY 2
#define MDL_RESOURCE_OBJECT 3
#define MDL_MAP_POSITION_TAG 0x534F504D
#define MDL_MAP_POSITION_DATA_OFFSET 0x10
#define MDL_MAP_POSITION_RECORD_BYTES 0x40

typedef struct MdlResource MdlResource;

typedef struct MdlSifCommand {
    s32 source;
    s32 end;
    s32 argument;
    u32 command;
} MdlSifCommand;

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
    s8 unk0F;
    s8 yawStepMode; /* 0x10: one yaw step per left/right button press */
    u8 pad11[3];
    s16 unk14;
    s16 resourceCount;
    s16 resourceGroup; /* 0x18: group passed to the resource loader */
    s16 resourceId;    /* 0x1A: ID passed to the resource loader */
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
    s16 editorMode; /* 0x40: 0 selects records, 1 edits a mark record */
    s16 unk42;
    u8 pad44[2];
    s16 markFieldCursor; /* 0x46: selected row in the mark parameter editor */
    s16 unk48;
    s16 unk4A;
    s16 unk4C;
    s16 unk4E;
    s16 unk50;
    s16 unk52;
    f32 viewerScale; /* 0x54: viewer zoom, in hundredths */
    u8 pad58[0x38];
    MdlResource *resources[MDL_VIEWER_RESOURCE_SLOTS]; /* 0x90 */
    s32 packetList; /* 0xC0: drawing packet destination */
} MdlViewState;

extern MdlViewState mdlViewerState;

static inline s8 mdlGetViewerDisplayMode(MdlViewState *state) {
    return state->unk0F;
}

extern char D_00421208[]; /* "MODEL : %d-%03d " */

extern char D_00421220[]; /* "MODEL : %d-%03x " */

extern char D_00421238[]; /* "%02d/%02d" */

extern char D_00421248[]; /* "%02x/%02x" */

extern char D_00437068[]; /* "" */

extern void sdfPktInit(void *, s32, s32, s32, s32);

extern s32 sdfFormatSifPacket();

extern s32 mdlGetNodeRefHalf(MdlResource *, s32);

extern s32 mdlGetNodeField2C(MdlResource *, s32);

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


extern void *memset(void *dst, s32 value, u32 size);

extern s32 dds3AdvanceWorldCounter();

extern s32 dds3SpawnCameraSlotObj5(s32 world, f32 *pos, f32 *rot);

extern void dds3SetObjectFlags(s32 obj, u32 flags);

extern void effObjSetInnerFloat(s32 obj, f32 value);

extern void func_00112058(s32 obj, s32 a, s32 b);

extern MdlResource *dds3GetObjectBaseResourceHandle(s32 obj);

extern void func_001129C8(s32 obj, s32 a);

extern s32 dds3GetWorldSecondaryObject();

extern s8 D_00453560[];

void func_00236568(void);

void mdlDrawViewerSelectionLabel(void);

extern s32 kwlnTaskGetTaskByName(void *name);

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

void fldDrawPackedRgbEditor(s32, s32, s32, s32, s32, s32, s32);

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

/* Native 0x40-byte package request; the package helpers fill handle at +0x30. */
typedef struct MdlPackageRequest {
    u8 pad00[0x30];
    s32 handle;
    u8 pad34[0xC];
} MdlPackageRequest;

/* Load a viewer package; flag 2 enables the extra request-preparation step. */
void mdlLoadViewerPackage(s32 first, s32 second, s32 flags, s32 requestFirst, s32 requestSecond) {
    MdlPackageRequest request;

    sdfPacInitializeDispatchPacket(&request, 0);
    if (flags & 2) {
        func_00346AD8(&request);
    }
    func_00346CF0(&request, requestFirst, requestSecond);
    func_00233280(request.handle, first, second, flags);
    func_00346AF8(&request);
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

/* Release all three handles in each viewer-table entry, then clear its tables and backing allocations. */
void mdlReleaseViewerSlotResources(void) {
    MdlSlotEntry *slotEntry;
    s32 entryCount;
    s32 entryIndex;

    entryCount = D_003C6588[MDL_VIEWER_TABLE_SLOT].count;
    if (entryCount > 0) {
        slotEntry = D_003C6588[MDL_VIEWER_TABLE_SLOT].entries;
        entryIndex = 0;
        do {
            entryIndex++;
            sdfReleaseChipBlock(slotEntry->secondHandle);
            sdfReleaseChipBlock(slotEntry->firstHandle);
            sdfReleaseChipBlock(slotEntry->thirdHandle);
            slotEntry++;
        } while (entryIndex < entryCount);
    }
    D_003C86B0[MDL_VIEWER_TABLE_SLOT].entries = NULL;
    D_003C86B0[MDL_VIEWER_TABLE_SLOT].count = 0;
    D_003C6588[MDL_VIEWER_TABLE_SLOT].entries = NULL;
    D_003C6588[MDL_VIEWER_TABLE_SLOT].count = 0;
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
extern void *sdfAllocSizeClassBlock(s32 size);
extern u32 sdfResourceRetainAddress(s32 handle);
extern u32 strlen(const char *);
extern char *strcpy(char *, const char *);
extern MdlViewerHeader *D_00438F98;
extern char **D_00438F9C;

/* Copy the three source strings into the viewer table, preserving the first/second handle ordering. */
void mdlInitializeViewerResourceTable(void) {
    s32 sourceIndex;
    char *sourceText;
    char *copiedText;

    mdlReleaseViewerSlotResources();
    D_00436FAC = sdfAllocGeneralBlock(MDL_VIEWER_HEADER_BYTES);
    D_00438F98 = (MdlViewerHeader *)sdfResourceRetainAddress(D_00436FAC);
    D_00438F98->kind = 5;
    D_00438F98->unk02 = 0;
    D_00438F98->unk04 = 0x1000;
    D_00438F98->unk06 = 0x3E8;
    D_00436FB0 = sdfAllocGeneralBlock(MDL_VIEWER_NAME_TABLE_BYTES);
    D_00438F9C = (char **)sdfResourceRetainAddress(D_00436FB0);
    for (sourceIndex = 0; sourceIndex != MDL_VIEWER_LABEL_COUNT; sourceIndex++) {
        switch (sourceIndex) {
        case 0:
            sourceText = (char *)D_003C6588[0].entries->secondHandle;
            break;
        case 1:
            sourceText = (char *)D_003C6588[0].entries->firstHandle;
            break;
        default:
            sourceText = (char *)D_003C6588[0].entries->thirdHandle;
            break;
        }
        if (sourceText != NULL) {
            copiedText = sdfAllocSizeClassBlock(strlen(sourceText) + 1);
            strcpy(copiedText, sourceText);
            switch (sourceIndex) {
            case 0:
                D_00438F9C[1] = copiedText;
                break;
            case 1:
                D_00438F9C[0] = copiedText;
                break;
            case 2:
                D_00438F9C[2] = copiedText;
                break;
            }
        }
    }
    D_003C86B0[MDL_VIEWER_TABLE_SLOT].entries = (MdlSlotEntry *)D_00438F98;
    D_003C86B0[MDL_VIEWER_TABLE_SLOT].count = 1;
    D_003C6588[MDL_VIEWER_TABLE_SLOT].entries = (MdlSlotEntry *)D_00438F9C;
    D_003C6588[MDL_VIEWER_TABLE_SLOT].count = 1;
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

/* Relative-linked record prefix with kind-dependent packed payload words.
 * Kinds 1/2 use the count/part-index halfwords at 0x0C/0x0E; kind 3 reads
 * the whole parameter word. Kind 4 reads both selector halfwords at 0x08/0x0A. */
typedef struct MdlRecord {
    s32 kind;       /* 0x00: 0xFFFF terminates the record chain */
    s32 nextOffset; /* 0x04: relative byte offset to next record */
    union {
        u32 word;               /* 0x08 */
        struct {
            u16 selectorA;
            u16 selectorB;
        } stream;
    } payload;
    union {
        u32 word;               /* 0x0C */
        struct {
            u16 count;
            u16 partIndex;
        } part;
    } parameter;
    u16 unk10;       /* 0x10 */
} MdlRecord;

typedef struct MdlViewerSlots MdlViewerSlots;

/* The viewer resource holds a pointer to the table container at +0x0C. */
typedef struct MdlViewerData {
    u8 pad00[0xA4];
    MdlRecord *records;         /* 0xA4: relative-linked model records */
    MdlViewerSlots *slotTable; /* 0xA8: indexable object slots */
} MdlViewerData;

/* Native loaded resource: viewer tables, linked parts, chunk data and motion. */
struct MdlResource {
    u8 pad00[0x0C];
    MdlViewerData *data; /* 0x0C */
    u8 pad10[2];
    s16 unk12;
    struct MdlResourceItem *first; /* 0x14: owned resource-item list */
    u8 *chunk;              /* 0x18: SDK chunk data, including the node-count prefix */
    MdlMotionState *motion; /* 0x1C: time also gates deferred object initialization */
};

struct MdlViewerSlots {
    u8 pad00[4];
    s16 count; /* 0x04 */
    u8 pad06[6];
    s32 entryBase; /* 0x0C: base of 0x10-byte slot entries */
};

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
    u32 kind;     /* 0x00: billboard, effect, or object */
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

/* Object attachment payload, embedded at resource item +8. */
typedef struct MdlObjectAttachment {
    MdlResource *owner;
    s32 objectAddress; /* Retain the native address-word type. */
    s32 data;
    s32 minimumTime; /* Owner's motion must reach this threshold. */
    u8 attributes[8]; /* Passed to stream initialization. */
} MdlObjectAttachment;

/* Native 0x20-byte list item. type selects a part handle or a deferred object
 * attachment; these are alternative payloads, not separate allocations. */
typedef struct MdlResourceItem {
    struct MdlResourceItem *next; /* 0x00 */
    u16 type; /* 0x04: MDL_RESOURCE_* */
    s16 subtype; /* 0x06 */
    union {
        struct {
            s32 handle; /* 0x08: billboard, effect, or tracked-poly handle */
            MdlPartEntry *unk0C; /* Retained slot; only written here. */
            void *unk10; /* Retained chunk record; only written here. */
            f32 unk14; /* Optional record value; only written here. */
            u8 pad18[8];
        } part;
        MdlObjectAttachment object;
    } payload;
} MdlResourceItem;


/* Read the model record's payload word without advancing its relative link. */
u32 mdlGetViewerRecordPayloadWord(MdlRecord *record) {
    return record->payload.word;
}

/* The part-list count is the low half of the packed parameter word. */
u16 mdlGetViewerRecordListCount(MdlRecord *record) {
    return record->parameter.part.count;
}

/* Follow relative links in the resource's record table to find an ID. */
MdlRecord *mdlFindViewerRecord(MdlResource *resource, s32 recordId) {
    MdlRecord *recordTable = resource->data->records;
    MdlRecord *recordCursor;
    s32 remainingRecords;

    if (recordTable == NULL) {
        return NULL;
    }
    recordCursor = recordTable;
    remainingRecords = mdlGetViewerRecordListCount(recordCursor);
    while (1) {
        remainingRecords--;
        recordCursor = (MdlRecord *)((u8 *)recordCursor + recordCursor->nextOffset);
        if (remainingRecords == -1) {
            return NULL;
        }
        if (recordCursor->kind == recordId) {
            return recordCursor;
        }
    }
}

/* The first eight bytes belong to the enclosing list, not its first record. */
MdlRecord *mdlGetFirstRecord(s32 listAddress) {
    MdlRecord *firstRecord;

    firstRecord = (MdlRecord *)(listAddress + MDL_RECORD_LIST_HEADER_BYTES);
    if (firstRecord->kind == MDL_RECORD_END_KIND) {
        firstRecord = NULL;
    }
    return firstRecord;
}

/* A terminal kind returns NULL; the link is a byte offset, not a pointer. */
MdlRecord *mdlGetNextRecord(MdlRecord *currentRecord) {
    MdlRecord *nextRecord;

    nextRecord = (MdlRecord *)((s32)currentRecord + currentRecord->nextOffset);
    if (nextRecord->kind == MDL_RECORD_END_KIND) {
        nextRecord = NULL;
    }
    return nextRecord;
}

/* Count relative-offset records until the 0xffff sentinel. */
s32 mdlCountRecords(s32 listAddress) {
    s32 recordCount;
    MdlRecord *recordCursor;

    if (listAddress == 0) {
        return 0;
    }
    recordCursor = mdlGetFirstRecord(listAddress);
    recordCount = 0;
    while (recordCursor != NULL) {
        recordCount++;
        recordCursor = mdlGetNextRecord(recordCursor);
    }
    return recordCount;
}

s32 mdlRecordMatchesId(MdlRecord *record, s32 wantedId) {
    return record->kind == wantedId;
}

u16 func_00233F58(MdlRecord *record) {
    return record->parameter.part.partIndex;
}

u16 func_00233F60(MdlRecord *record) {
    return record->unk10;
}

extern char D_00420FF0[], D_00421000[], D_00421010[], D_00421020[], D_00421030[], D_00421040[];
extern char D_00436FC0[], D_00436FC8[];

/* Append the kind-dependent record summary at the requested text position and style. */
INCLUDE_RODATA(const s32, "game/code_00233660", D_00420FF0);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421000);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421010);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421020);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421030);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421040);

void func_00233F68(s32 packetList, s32 x, s32 y, s32 depth, s32 textStyle, MdlRecord *record) {
    if (record->kind >= 1 && record->kind <= 5) {
        switch (record->kind) {
        case 1:
        case 2: {
            char *label = record->kind == 1 ? D_00436FC0 : D_00436FC8;

            if (record->parameter.part.count == 1) {
                sdfAppendPacket(packetList, sdfCreateFormattedSifCommand(x, y, depth, textStyle,
                                                                    D_00420FF0, label, record->payload.word,
                                                                    record->parameter.part.partIndex));
                return;
            }
            sdfAppendPacket(packetList, sdfCreateFormattedSifCommand(x, y, depth, textStyle,
                                                                D_00421000, label, record->payload.word,
                                                                record->payload.word + record->parameter.part.count - 1,
                                                                record->parameter.part.partIndex));
            return;
        }
        case 3:
            sdfAppendPacket(packetList, sdfCreateFormattedSifCommand(x, y, depth, textStyle,
                                                                D_00421010, record->payload.word,
                                                                record->parameter.word));
            return;
        case 4:
            sdfAppendPacket(packetList, sdfCreateFormattedSifCommand(x, y, depth, textStyle,
                                                                D_00421020, record->payload.stream.selectorA,
                                                                record->payload.stream.selectorB));
            return;
        case 5:
            sdfAppendPacket(packetList, sdfCreateFormattedSifCommand(x, y, depth, textStyle,
                                                                D_00421030, record->payload.word));
            return;
        }
    } else {
        sdfAppendPacket(packetList, sdfCreateFormattedSifCommand(x, y, depth, textStyle,
                                                            D_00421040, record->kind));
    }
}

/* Draw the scalar marker fields, then four colors with four byte channels each; negative selection hides the cursor. */
void mdlDrawMarkParamsPanel(s32 packetList, s32 x, s32 y, s32 depth, EffMarkParams *params, s32 selectedField) {
    s32 colorY = y + 0x300;
    s32 labelX = x + 0xC0;
    u32 packedColor;
    s32 colorIndex;
    s32 channelIndex;

    sdfAppendPacket(packetList, sdfCreateFormattedSifCommand(labelX, y, depth, 0, "MARK0:%d", params->mark0));
    sdfAppendPacket(packetList, sdfCreateFormattedSifCommand(labelX, y + 0x60, depth, 0, "MARK1:%d", params->mark1));
    sdfAppendPacket(packetList, sdfCreateFormattedSifCommand(labelX, y + 0xC0, depth, 0, "START:%d", params->start));
    sdfAppendPacket(packetList, sdfCreateFormattedSifCommand(labelX, y + 0x120, depth, 0, "END  :%d", params->end));
    sdfAppendPacket(packetList, sdfCreateFormattedSifCommand(labelX, y + 0x180, depth, 0, "ITRVL:%d", params->interval));
    sdfAppendPacket(packetList, sdfCreateFormattedSifCommand(labelX, y + 0x1E0, depth, 0, "FACE :%d", params->face));
    sdfAppendPacket(packetList, sdfCreateFormattedSifCommand(labelX, y + 0x240, depth, 0, "BLEND:%d", params->blend));
    for (colorIndex = 0; colorIndex != MDL_MARK_COLOR_COUNT; colorIndex++) {
        packedColor = params->colors[colorIndex];
        sdfAppendPacket(packetList, func_0011F250(x + 0x480, colorY, depth, 0x300, 0x180, (packedColor & 0xFFFFFF) | 0x80000000, 0x60404040));
        for (channelIndex = 0; channelIndex != MDL_COLOR_CHANNEL_COUNT; channelIndex++) {
            sdfAppendPacket(packetList, sdfCreateFormattedSifCommand(labelX, colorY, depth, D_003C8760[channelIndex], D_003C8768[channelIndex]));
            sdfAppendPacket(packetList, sdfCreateFormattedSifCommand(x + 0x240, colorY, depth, 0, D_00436FF0, packedColor & MDL_COLOR_BYTE_MASK));
            packedColor >>= MDL_COLOR_BYTE_BITS;
            colorY += 0x60;
        }
        colorY += 0x60;
    }
    if (selectedField >= 0) {
        sdfAppendPacket(packetList, sdfCreateFormattedSifCommand(x, y + D_003C8730[selectedField], depth, 0, D_00436FF8));
    }
}

/* Edit the marker parameters with the pad: rows 0-6 are the scalar fields, 7-22 the color bytes. */
void mdlEditMarkParametersWithPad(EffMarkParams *params, s16 *fieldCursor) {
    s32 selectedField = *fieldCursor;
    u8 *colorChannel;

    if (D_0037F510[0x27] < 0) {
        selectedField = selectedField != MDL_MARK_LAST_FIELD ? selectedField + 1 : 0;
    } else if (((u8)D_0037F510[0x27] & MDL_PAD_REPEAT_FLAG) != 0 && selectedField < MDL_MARK_LAST_FIELD) {
        selectedField++;
    } else if (D_0037F510[0x26] < 0) {
        selectedField = selectedField != 0 ? selectedField - 1 : MDL_MARK_LAST_FIELD;
    } else if (((u8)D_0037F510[0x26] & MDL_PAD_REPEAT_FLAG) != 0 && selectedField > 0) {
        selectedField--;
    } else {
        switch (selectedField) {
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
            colorChannel = (u8 *)params->colors + selectedField - MDL_MARK_COLOR_FIRST_FIELD;
            if (D_0037F510[0x25] < 0) {
                *colorChannel += 1;
            } else if (((u8)D_0037F510[0x25] & MDL_PAD_REPEAT_FLAG) != 0 && *colorChannel < MDL_COLOR_BYTE_MASK) {
                *colorChannel += 1;
            } else if (D_0037F510[0x24] < 0) {
                *colorChannel -= 1;
            } else if (((u8)D_0037F510[0x24] & MDL_PAD_REPEAT_FLAG) != 0 && *colorChannel != 0) {
                *colorChannel -= 1;
            }
            break;
        }
    }
    *fieldCursor = selectedField;
}

/* Append a newly created billboard to the next part-list slot. */
void mdlAddBillboardPart(MdlPartList *partList, s32 descriptorIndex) {
    MdlPartEntry *partEntry = &partList->entries[partList->count];

    partEntry->state = 0;
    partEntry->kind = MDL_PART_BILLBOARD;
    partEntry->object = billCreateIndexed(1, descriptorIndex);
    partList->count += 1;
}

/* Append a newly created effect to the next part-list slot. */
void mdlAddEffectPart(MdlPartList *partList, s32 descriptorIndex) {
    MdlPartEntry *partEntry = &partList->entries[partList->count];

    partEntry->kind = MDL_PART_EFFECT;
    partEntry->state = 0;
    partEntry->object = effCreateNodeFromDescriptor(descriptorIndex);
    partList->count += 1;
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

typedef struct DevRequest DevRequest;
extern DevRequest *sdfDevCreateBufferedRequest(s32, s32, s32);

MdlPartList *mdlCreateBufferedPartRequest(u32 request) {
    return (MdlPartList *)sdfDevCreateBufferedRequest(request, 0x10, 4);
}

/* Destroy each part according to its stored kind, then release the owning request. */
void mdlDestroyPartList(MdlPartList *partList) {
    s32 partIndex;

    if (partList != NULL) {
        for (partIndex = 0; partIndex < partList->count; partIndex++) {
            MdlPartEntry *partEntry = &partList->entries[partIndex];

            switch (partEntry->kind) {
            case MDL_PART_BILLBOARD:
                billDispatchByKind(partEntry->object);
                break;
            case MDL_PART_EFFECT:
                effDestroyNode(partEntry->object);
                break;
            case MDL_PART_OBJECT:
                mdlObjDestroy((MdlObj *)partEntry->object);
                break;
            }
        }
        sdfDestroyDevRequest(partList);
    }
}

/* Prepend a cleared, typed resource item to the owner's list. */
MdlResourceItem *mdlInsertResourceItem(MdlResource *owner, s32 type, s32 subtype) {
    MdlResourceItem *item = sdfAllocAndClearQuadwords(sizeof(MdlResourceItem));
    MdlResourceItem *previousHead = owner->first;
    item->type = type;
    item->next = previousHead;
    item->subtype = subtype;
    owner->first = item;
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

/* Resolve a native fixed-size slot when its table exists and index is below the upper bound; no lower-bound check. */
s32 mdlFindViewerPartSlot(MdlResource *resource, s32 slotIndex) {
    MdlViewerSlots *slotTable;

    slotTable = resource->data->slotTable;
    if (slotTable == NULL) {
        return 0;
    }
    if (slotIndex >= slotTable->count) {
        return 0;
    }
    return slotTable->entryBase + slotIndex * MDL_PART_SLOT_BYTES;
}

typedef struct MdlPartRec {
    u8 pad00[4];
    u32 size;      /* 0x04 */
    s32 firstId;   /* 0x08 */
    u16 count;     /* 0x0C */
    u16 partIndex; /* 0x0E */
    f32 value;     /* 0x10 */
} MdlPartRec;


extern void *sdfChunkFindRecordById(void *chunk, s32 id);

/* Bind each consecutive record ID to a newly created part when the chunk contains it. */
s32 mdlBindViewerPartRecords(MdlResource *owner, MdlPartRec *partRecord, s32 subtype, s32 type, s32 (*createPart)(MdlPartEntry *)) {
    MdlPartEntry *partSlot = (MdlPartEntry *)mdlFindViewerPartSlot(owner, partRecord->partIndex);

    if (partSlot != NULL) {
        void *chunk = owner->chunk;
        s32 recordId = partRecord->firstId;
        s32 remainingRecords = partRecord->count;
        f32 optionalValue = 0.0f;

        /* Retain the native 17-byte size threshold for the optional value. */
        if (partRecord->size >= MDL_PART_VALUE_SIZE_THRESHOLD) {
            optionalValue = partRecord->value;
        }
        do {
            void *chunkRecord = sdfChunkFindRecordById(chunk, recordId++);

            if (chunkRecord != NULL) {
                MdlResourceItem *resourceItem = mdlInsertResourceItem(owner, type, subtype);

                resourceItem->payload.part.handle = createPart(partSlot);
                resourceItem->payload.part.unk0C = partSlot;
                resourceItem->payload.part.unk10 = chunkRecord;
                resourceItem->payload.part.unk14 = optionalValue;
            }
        } while (--remainingRecords != 0);
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
    MdlResource *owner; /* 0x00 */
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

/* Convert the effect record to native creation parameters and retain the resulting handle in a list item. */
s32 mdlCreateViewerEffectPart(MdlResource *owner, MdlEffectRec *effectRecord, s32 subtype) {
    MdlResourceItem *resourceItem;
    MdlEffectParams effectParams;

    effectParams.owner = owner;
    effectParams.effectId = effectRecord->effectId;
    effectParams.param0C = effectRecord->param0C;
    effectParams.scaleX = effectRecord->scaleX;
    effectParams.scaleY = effectRecord->scaleY;
    effectParams.value14 = effectRecord->value14;
    effectParams.value16 = effectRecord->value16;
    effectParams.mode = 3;
    effectParams.value17 = effectRecord->value17;
    effectParams.value18 = effectRecord->value18;
    effectParams.value1C = effectRecord->value1C;
    effectParams.value20 = effectRecord->value20;
    effectParams.value24 = effectRecord->value24;
    resourceItem = mdlInsertResourceItem(owner, MDL_RESOURCE_TRACK_POLY, subtype);
    return resourceItem->payload.part.handle = effTrackPolyCreateWork(&effectParams);
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


typedef struct MdlEntryRec {
    u8 pad00[8];
    s32 dataId; /* 0x08 */
    s32 minimumTime; /* 0x0C: deferred initialization threshold */
    u8 flagB;   /* 0x10 */
    u8 flagA;   /* 0x11 */
    u16 index;  /* 0x12 */
} MdlEntryRec;

typedef struct MdlSlotRec {
    u8 pad00[8];
    MdlObj *obj; /* 0x08 */
} MdlSlotRec;

extern void *sdfFindResourceById(s32 id);

/* Claim an unused object only after its data resource resolves, retaining record flags and deferred-init parameters. */
s32 mdlClaimViewerObjectPart(MdlResource *owner, MdlEntryRec *entryRecord, s32 subtype) {
    MdlSlotRec *partSlot = (MdlSlotRec *)mdlFindViewerPartSlot(owner, entryRecord->index);

    if (partSlot != 0) {
        MdlObj *object = partSlot->obj;
        if (object->inUse == 0) {
            s32 resourceData = (s32)sdfFindResourceById(entryRecord->dataId);
            if (resourceData != 0) {
                MdlResourceItem *resourceItem;
                u8 *attributes;
                object->inUse = 1;
                resourceItem = mdlInsertResourceItem(owner, MDL_RESOURCE_OBJECT, subtype);
                /* Required to match: store object/data before binding the owner. */
                resourceItem->payload.object.objectAddress = (s32)object;
                attributes = resourceItem->payload.object.attributes;
                resourceItem->payload.object.data = resourceData;
                resourceItem->payload.object.owner = owner;
                resourceItem->payload.object.minimumTime = entryRecord->minimumTime;
                attributes[1] = 1;
                attributes[2] = entryRecord->flagA;
                attributes[3] = entryRecord->flagB;
            }
        }
    }
}

/* Initialize once the owner's motion reaches the stored threshold; retain native float-to-signed-to-unsigned conversion. */
void mdlCondInitEntry(s32 itemAddress) {
    s32 objectAddress = ((MdlResourceItem *)itemAddress)->payload.object.objectAddress;
    if (((MdlObj *)objectAddress)->initialized == 0) {
        s32 minimumTime = ((MdlResourceItem *)itemAddress)->payload.object.minimumTime;
        f32 motionTime = ((MdlResourceItem *)itemAddress)->payload.object.owner->motion->time;
        if ((u32)(s32)motionTime < (u32)minimumTime) {
            return;
        }
        mdlObjInit((MdlObj *)objectAddress, ((MdlResourceItem *)itemAddress)->payload.object.data, (s32)((MdlResourceItem *)itemAddress)->payload.object.attributes);
    }
}

extern s32 mdlBindViewerPartRecords(MdlResource *object, MdlPartRec *record, s32 option, s32 type, s32 (*advance)(MdlPartEntry *));

extern s32 mdlClaimViewerObjectPart(MdlResource *object, MdlEntryRec *record, s32 option);

/* Dispatch the five record kinds, retaining the game's native return behavior. */
s32 mdlDispatchResourceEntry(s32 owner, MdlRecord *record, s32 subtype) {
    switch (record->kind) {
    case 1:
        return mdlBindViewerPartRecords(owner, (MdlPartRec *)record, subtype, MDL_RESOURCE_BILLBOARD, mdlAdvanceBillboardPart);
    case 2:
        return mdlBindViewerPartRecords(owner, (MdlPartRec *)record, subtype, MDL_RESOURCE_EFFECT, mdlAdvanceEffectPart);
    case 3:
        return mdlCreateViewerEffectPart(owner, (MdlEffectRec *)record, subtype);
    case 4:
        return mdlLoadViewerStreamRecord(owner, (s32)record);
    case 5:
        mdlClaimViewerObjectPart(owner, (MdlEntryRec *)record, subtype);
        break;
    }
}

/* Find the requested record list and apply each relative-linked entry with the supplied subtype. */
void mdlApplyResourceEntries(s32 resourceAddress, s32 recordId, s32 subtype) {
    MdlRecord *recordList = mdlFindViewerRecord((MdlResource *)resourceAddress, recordId);
    if (recordList != NULL) {
        MdlRecord *recordCursor = mdlGetFirstRecord((s32)recordList);
        while (recordCursor != NULL) {
            mdlDispatchResourceEntry(resourceAddress, recordCursor, subtype);
            recordCursor = mdlGetNextRecord(recordCursor);
        }
    }
}

/* Release billboard, effect, or tracked-poly handles when applicable, then always free the list item. */
void mdlDestroyResourceItem(MdlResourceItem *item) {
    switch (item->type) {
    case MDL_RESOURCE_BILLBOARD:
        billDispatchByKind(item->payload.part.handle);
        break;
    case MDL_RESOURCE_EFFECT:
        effDestroyNode(item->payload.part.handle);
        break;
    case MDL_RESOURCE_TRACK_POLY:
        effTrackPolyRelease(item->payload.part.handle);
        break;
    }
    sdfReleaseChipBlock((void *)item);
}

/* Unlink matching subtypes without losing the incoming link when consecutive items are removed. */
void mdlRemoveResourceSubtype(MdlResource *owner, s32 subtype) {
    MdlResourceItem **itemLink = &owner->first;
    MdlResourceItem *item = *itemLink;
    while (item != 0) {
        if (item->subtype == subtype) {
            MdlResourceItem *nextItem = item->next;
            mdlDestroyResourceItem(item);
            *itemLink = nextItem;
            item = nextItem;
        } else {
            itemLink = &item->next;
            item = item->next;
        }
    }
}

/* vu0 routine: positionOut = p + normalize(p - sdfViewTargetVector) * scale, p = transformed node position. */
void mdlResolveAnchorPosition(void *chunk, MdlAnchorRec *anchorRecord, f32 *positionOut) {
    MdlNodeInfo *nodeInfo = anchorRecord->info;
    u8 *drawNode = sdfModelFindDrawNode(chunk, nodeInfo->id);
    f32 scale = anchorRecord->scale;

    VU0_LOAD_MATRIX(drawNode + 0xC0);
    VU0_LOAD_VF(vf10, nodeInfo->pos);
    VU0_TRANSFORM_POINT(vf10, vf10);
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf12, sdfViewTargetVector);
    VU0_SUB(vf10, vf10, vf12);
    VU0_NORMALIZE_VF10();
    VU0_SET_VF2X(scale);
    VU0_MUL_VF2X(vf10, vf10);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF_UNCLOBBERED(vf10, positionOut);
}

/* Update anchored billboard/effect positions, tracked polygons, or deferred object initialization by type. */
void mdlDispatchViewerAnchorRecord(MdlResource *owner, MdlAnchorRec *anchorRecord) {
    void *chunk = owner->chunk;
    f32 position[4];
    s32 resourceHandle;

    switch (anchorRecord->type) {
    case 0:
        mdlResolveAnchorPosition(chunk, anchorRecord, position);
        resourceHandle = anchorRecord->handle;
        effCopyVector(resourceHandle, position);
        billInvokeCallback(resourceHandle);
        break;
    case 1:
        mdlResolveAnchorPosition(chunk, anchorRecord, position);
        resourceHandle = anchorRecord->handle;
        effCopyVectorToNodeInstance(resourceHandle, position);
        effUpdateNode(resourceHandle);
        break;
    case 2:
        effTrackPolyUpdate(anchorRecord->handle);
        break;
    case 3:
        mdlCondInitEntry((s32)anchorRecord);
        break;
    }
}

/* Set a billboard/effect frame; other resource-item kinds have no frame dispatch. */
void mdlSetResourceFrame(s32 unused, MdlResourceItem *item, s32 frame) {
    switch (item->type) {
    case MDL_RESOURCE_BILLBOARD:
        billSetChildParameter(item->payload.part.handle, frame);
        return;
    case MDL_RESOURCE_EFFECT:
        effSetNodeParameterValue(item->payload.part.handle, frame);
        break;
    }
}

/* Apply amount to billboard/effect scale; other resource-item kinds are ignored. */
void mdlSetResourceAmount(s32 unused, MdlResourceItem *item, float amount) {
    switch (item->type) {
    case MDL_RESOURCE_BILLBOARD:
        billSetChildScaleComponents(item->payload.part.handle, amount, amount);
        return;
    case MDL_RESOURCE_EFFECT:
        effApplyNodeScale(item->payload.part.handle, amount);
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

/* Return 1 after a cursor update: fresh presses wrap, while repeated presses stop at the ends. */
s32 mdlUpdateViewerCursor(s16 *cursor, s32 entryCount) {
    s32 lastIndex = entryCount - 1;
    s32 selectedIndex = *cursor;
    s32 changed = 0;
    if (sdfPadButtonStates[5] < 0) {
        if (selectedIndex < lastIndex) {
            selectedIndex += 1;
            changed = 1;
        } else {
            selectedIndex = 0;
            changed = 1;
        }
    } else if (((u8)sdfPadButtonStates[5] & MDL_PAD_REPEAT_FLAG) != 0) {
        if (selectedIndex < lastIndex) {
            selectedIndex += 1;
            changed = 1;
        }
    } else if (sdfPadButtonStates[4] < 0) {
        if (selectedIndex > 0) {
            selectedIndex -= 1;
            changed = 1;
        } else {
            selectedIndex = lastIndex;
            changed = 1;
        }
    } else if ((((u8)sdfPadButtonStates[4] & MDL_PAD_REPEAT_FLAG) != 0) && (selectedIndex > 0)) {
        selectedIndex -= 1;
        changed = 1;
    }
    if (changed != 0) {
        *cursor = selectedIndex;
        mdlViewerState.unk09 = 0;
        return 1;
    }
    return 0;
}

/* Apply single-entry or optional page steps using the native wrap/clamp and input-precedence rules. */
s32 mdlUpdateViewerCursorWithPageStep(s16 *cursor, s32 entryCount, s32 pageStep) {
    s32 lastIndex = entryCount - 1;
    s32 selectedIndex = *cursor;
    s32 changed = 0;
    if (sdfPadButtonStates[7] < 0) {
        if (selectedIndex < lastIndex) {
            selectedIndex += 1;
        } else {
            selectedIndex = 0;
        }
        changed = 1;
    } else if (((u8)sdfPadButtonStates[7] & MDL_PAD_REPEAT_FLAG) != 0) {
        if (selectedIndex < lastIndex) {
            selectedIndex += 1;
            changed = 1;
        }
    } else if (sdfPadButtonStates[6] < 0) {
        if (selectedIndex > 0) {
            selectedIndex -= 1;
        } else {
            selectedIndex = lastIndex;
        }
        changed = 1;
    } else if (((u8)sdfPadButtonStates[6] & MDL_PAD_REPEAT_FLAG) != 0) {
        if (selectedIndex > 0) {
            selectedIndex -= 1;
            changed = 1;
        }
    } else if (pageStep != 0) {
        if (sdfPadButtonStates[11] < 0) {
            if (selectedIndex < lastIndex) {
                selectedIndex += pageStep;
                if (selectedIndex > lastIndex) {
                    selectedIndex = lastIndex;
                }
            } else {
                selectedIndex = 0;
            }
            changed = 1;
        } else if (((u8)sdfPadButtonStates[11] & MDL_PAD_REPEAT_FLAG) != 0) {
            if (selectedIndex < lastIndex) {
                selectedIndex += pageStep;
                if (selectedIndex > lastIndex) {
                    selectedIndex = lastIndex;
                }
                changed = 1;
            }
        } else if (sdfPadButtonStates[10] < 0) {
            if (selectedIndex > 0) {
                selectedIndex -= pageStep;
                if (selectedIndex < 0) {
                    selectedIndex = 0;
                }
            } else {
                selectedIndex = lastIndex;
            }
            changed = 1;
        } else if ((((u8)sdfPadButtonStates[10] & MDL_PAD_REPEAT_FLAG) != 0) && (selectedIndex > 0)) {
            selectedIndex -= pageStep;
            if (selectedIndex < 0) {
                selectedIndex = 0;
            }
            changed = 1;
        }
    }
    if (changed != 0) {
        *cursor = selectedIndex;
        mdlViewerState.unk09 = 0;
        return 1;
    }
    return 0;
}

extern MdlResource *func_00232198(s16 a, s16 b);

extern void mdlAddEntryFlagged(MdlResource *loaded, s32 a, s32 b);

void mdlLoadViewerResourceAndResetCursors(void) {
    MdlResource *loaded;

    loaded = func_00232198(mdlViewerState.resourceGroup, mdlViewerState.resourceId);
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

/* Rotate the viewer resource list in place without moving its allocation. */
void mdlRotateViewResourcesRight(void) {
    s32 resourceIndex = mdlViewerState.resourceCount - 1;
    MdlResource *lastResource = mdlViewerState.resources[resourceIndex];

    if (resourceIndex > 0) {
        do {
            mdlViewerState.resources[resourceIndex] = mdlViewerState.resources[resourceIndex - 1];
            resourceIndex -= 1;
        } while (resourceIndex > 0);
    }
    mdlViewerState.resources[0] = lastResource;
}

/* Move the first resource to the end of the active list without changing its allocation. */
void mdlRotateViewList(void) {
    s32 resourceIndex;
    s32 resourceCount = mdlViewerState.resourceCount;
    MdlResource *firstResource = mdlViewerState.resources[0];

    for (resourceIndex = 0; resourceIndex < resourceCount - 1; resourceIndex++) {
        mdlViewerState.resources[resourceIndex] = mdlViewerState.resources[resourceIndex + 1];
    }
    mdlViewerState.resources[resourceIndex] = firstResource;
}

INCLUDE_ASM(const s32, "game/code_00233660", func_00235568);


INCLUDE_ASM(const s32, "game/code_00233660", func_00235628);

void func_00235728(void) {
    MdlSifCommand packet;
    const char *format;
    s32 displayMode;
    s32 nodeCount;
    s32 formatted;

    mdlAppendViewerRectToDrawList(0x7E10, 0x8608, 0xFF0080, 0xEA0, 0x90, 0);
    sdfPktInit(&packet, 0x7E40, 0x8620, 0xFF0080, 0);

    if ((displayMode = mdlGetViewerDisplayMode(&mdlViewerState)) == 0) {
        format = D_00421208;
    } else {
        format = D_00421220;
    }
    sdfAppendPacket(mdlViewerState.packetList,
                    sdfFormatSifPacket(&packet, format, mdlViewerState.resourceGroup, mdlViewerState.resourceId));

    nodeCount = mdlGetNodeRefHalf(mdlViewerState.resources[0], 0);
    if (nodeCount == 0) {
        formatted = sdfFormatSifPacket(&packet, D_00437068);
    } else {
        if (mdlViewerState.unk0F == 0) {
            format = D_00421238;
        } else {
            format = D_00421248;
        }
        formatted = sdfFormatSifPacket(&packet, format,
                                       mdlGetNodeField2C(mdlViewerState.resources[0], 0), nodeCount - 1);
    }
    sdfAppendPacket(mdlViewerState.packetList, formatted);
}

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

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421208);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421220);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421238);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421248);

INCLUDE_ASM(const s32, "game/code_00233660", func_00235970);

INCLUDE_ASM(const s32, "game/code_00233660", func_00235C20);

u32 mdlRunViewerAssetSelectionTask(void) {
    func_00235970();
    func_00235C20();
    return 0;
}

extern void effMiscAxisAngleToQuaternionVf11(f32 angle);
extern void effMiscQuatMultiplyVU(void);
extern f32 sdfViewMatrix[];
extern void effMiscQuaternionToMatrixVU(void);

/* vu0 routine: move the viewer camera or orbit it about the look-at point. */
void func_00236080(void) {
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
        VU0_LOAD_VF(vf10, &D_00453620);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, &D_00453620);
        VU0_LOAD_VF(vf10, &D_00453630);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, &D_00453630);
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
            VU0_LOAD_VF(vf10, &D_00453630);
            VU0_LOAD_VF(vf11, &D_00453620);
            VU0_SUB(vf10, vf10, vf11);
            VU0_TRANSFORM_POINT(vf10, vf10);
            VU0_ADD(vf10, vf10, vf11);
            VU0_STORE_VF(vf10, &D_00453630);
            VU0_LOAD_VF(vf10, &D_00453640);
            VU0_TRANSFORM_POINT(vf10, vf10);
            VU0_STORE_VF(vf10, &D_00453640);
        }
    }
}

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

extern void mdlLoadPrimaryVectorVU(MdlResource *context);
extern void mdlStorePrimaryVectorVU(MdlResource *context);
extern void mdlLoadSecondaryVectorVU(MdlResource *context);
extern void mdlUpdateContextRotationBasisFromQuaternion(MdlResource *context);
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
        mdlStorePrimaryVectorVU(mdlViewerState.resources[0]);
        mdlUpdateContextRotationBasisFromQuaternion(mdlViewerState.resources[0]);
        return;
    }
    if (mdlViewerState.yawStepMode != 0) {
        mdlLoadSecondaryVectorVU(mdlViewerState.resources[0]);
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
        mdlUpdateContextRotationBasisFromQuaternion(mdlViewerState.resources[0]);
    } else if (mdlViewerState.labelIndexB == 0) {
        speed = 1.25f;
        if (sdfPadButtonStates[1] == 0) {
            speed = 20.0f;
            if (sdfPadButtonStates[3] == 0) {
                speed = 5.0f;
            }
        }
        mdlLoadPrimaryVectorVU(mdlViewerState.resources[0]);
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
        mdlStorePrimaryVectorVU(mdlViewerState.resources[0]);
    } else {
        speed = 0.5f * 3.14159265f / 180.0f;
        if (sdfPadButtonStates[1] == 0) {
            speed = 8.0f * 3.14159265f / 180.0f;
            if (sdfPadButtonStates[3] == 0) {
                speed = 2.0f * 3.14159265f / 180.0f;
            }
        }
        mdlLoadSecondaryVectorVU(mdlViewerState.resources[0]);
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
        mdlUpdateContextRotationBasisFromQuaternion(mdlViewerState.resources[0]);
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

/* Change the viewer scale in hundredths, with larger steps at larger values. */
void mdlAdjustViewerScale(void) {
    s32 scaleHundredths = (s32)(mdlViewerState.viewerScale * 100.0f + 0.5f);

    /* Keep the distinct up/down thresholds and the native float expression order. */
    if (D_0037F510[0x27] & MDL_PAD_REPEAT_FLAG) {
        if (scaleHundredths < 0x32) {
            scaleHundredths += 1;
        } else if (scaleHundredths < 0x1F4) {
            scaleHundredths += 10;
        } else {
            scaleHundredths += 100;
            if (scaleHundredths >= 0x7D1) {
                scaleHundredths = 0x7D0;
            }
        }
        mdlViewerState.viewerScale = scaleHundredths * 0.01f;
    } else if (D_0037F510[0x26] & MDL_PAD_REPEAT_FLAG) {
        if (scaleHundredths < 0x33) {
            scaleHundredths -= 1;
            if (scaleHundredths < 5) {
                scaleHundredths = 5;
            }
        } else if (scaleHundredths < 0x1F5) {
            scaleHundredths -= 10;
        } else {
            scaleHundredths -= 100;
        }
        mdlViewerState.viewerScale = scaleHundredths * 0.01f;
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
    s32 packetList;
    MdlResource *resource;
    MdlMotionState *motion;
    s32 playheadX;
    s32 textStyle;

    mdlAppendViewerRectToDrawList(0x81D0, 0x7948, 0xFF007F, 0xD20, 0xF0, 0);
    packetList = mdlViewerState.packetList;
    sdfAppendFillRectanglePacket(packetList, 0x80303030, 0, 0x8200, 0x7990, 0x8EC0, 0x7990, 0xFF0080, 0);
    sdfAppendFillRectanglePacket(packetList, 0x80303030, 0, 0x8200, 0x7960, 0x8200, 0x79C0, 0xFF0080, 0);
    sdfAppendFillRectanglePacket(packetList, 0x80303030, 0, 0x8EC0, 0x7960, 0x8EC0, 0x79C0, 0xFF0080, 0);
    resource = mdlViewerState.resources[0];
    motion = resource->motion;
    if (motion != NULL) {
        playheadX = (s32)(motion->time * 3264.0f / motion->length);
        if (playheadX < 0) {
            playheadX = 0;
        }
        playheadX += 0x8200;
        sdfAppendFillRectanglePacket(packetList, 0x800000E0, 0, playheadX, 0x7960, playheadX, 0x79C0, 0xFF0090, 0);
        sdfAppendPacket(mdlViewerState.packetList, sdfCreateFormattedSifCommand(0x8200, 0x79C0, 0xFF0080, 0, "[%5.1f/%-3d]", motion->time, motion->length));
    } else {
        sdfAppendPacket(mdlViewerState.packetList, sdfCreateFormattedSifCommand(0x8200, 0x79C0, 0xFF0080, 0, "[---.-/---]"));
    }
    textStyle = mdlViewerState.unitStepMode != 0 ? 2 : 0;
    sdfAppendPacket(mdlViewerState.packetList, sdfCreateFormattedSifCommand(0x8B00, 0x79C0, 0xFF0080, textStyle, D_004370C0, mdlViewerState.viewerScale));
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

extern u16 mdlGetContextResourceGroup(MdlResource *resource);
extern u16 mdlGetContextResourceId(MdlResource *resource);
extern void mdlDestroyContext(MdlResource *resource);
extern void sdfMotionInitializeAtZeroTime(void *, s32, s32);

void mdlApplyViewerResourceMenuAction(void) {
    s32 i;
    MdlResource *resource;
    s32 entryId;
    MdlMotionState *motion;

    if (mdlUpdateViewerCursorWithPageStep(&mdlViewerState.scrollPage, 5, 1) != 0) {
        return;
    }
    if (sdfPadButtonStates[1] >= 0) {
        return;
    }
    switch (mdlViewerState.scrollPage) {
    case 0:
        if (mdlViewerState.resourceCount != MDL_VIEWER_RESOURCE_SLOTS) {
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
            resource = mdlViewerState.resources[i];
            motion = resource->motion;
            if (motion != NULL) {
                entryId = resource->unk12;
                if (motion->reverse == 0) {
                    sdfMotionInitializeAtZeroTime(motion, entryId, 0);
                } else {
                    sdfMotionInitializeAtZeroTime(motion, entryId, 1);
                }
            }
        }
        break;
    }
    mdlViewerState.unk1C = mdlViewerState.resourceGroup = mdlGetContextResourceGroup(mdlViewerState.resources[0]);
    mdlViewerState.unk1E = mdlViewerState.resourceId = mdlGetContextResourceId(mdlViewerState.resources[0]);
    i = mdlViewerState.resources[0]->unk12;
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

    node = ((MdlLoadedInfo *)mdlViewerState.resources[0]->chunk)->first;
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

/* Sum list -1 and active-entry counts, retaining both independent lookups. */
s32 mdlCountActiveRecords(void) {
    MdlResource *resource = mdlViewerState.resources[0];
    s32 firstListCount = mdlCountRecords((s32)mdlFindViewerRecord(resource, -1));
    s32 activeListCount = mdlCountRecords((s32)mdlFindViewerRecord(resource, mdlViewerState.activeEntryId));

    return firstListCount + activeListCount;
}

INCLUDE_ASM(const s32, "game/code_00233660", func_00237A70);

extern MdlRecord *func_00237A70(void);

void mdlUpdateViewerMarkEditorInput(void) {
    MdlViewState *state = &mdlViewerState;
    s32 count;
    MdlRecord *record;

    switch (state->editorMode) {
    case 0:
        count = mdlCountActiveRecords();
        if (D_0037F510[0x27] < 0) {
            state->unk42++;
            if (state->unk42 >= count) {
                state->unk42 = 0;
            }
        } else if (((u8)D_0037F510[0x27] & MDL_PAD_REPEAT_FLAG) != 0) {
            if (state->unk42 < count - 1) {
                state->unk42++;
            }
        } else if (D_0037F510[0x26] < 0) {
            if (state->unk42 == 0) {
                state->unk42 = count - 1;
            } else {
                state->unk42--;
            }
        } else if (((u8)D_0037F510[0x26] & MDL_PAD_REPEAT_FLAG) != 0) {
            if (state->unk42 > 0) {
                state->unk42--;
            }
        } else if (D_0037F510[0x21] < 0) {
            record = func_00237A70();
            if (record != NULL && mdlRecordMatchesId(record, 3)) {
                state->editorMode = 1;
            }
        }
        break;
    case 1:
        record = func_00237A70();
        if (D_0037F510[0x23] >= 0 && record != NULL && mdlRecordMatchesId(record, 3)) {
            if (D_0037F510[0x21] < 0) {
                mdlAddPlainViewerEntryForSelectedNode();
            } else {
                mdlEditMarkParametersWithPad((EffMarkParams *)record, &state->markFieldCursor);
            }
        } else {
            state->editorMode = 0;
        }
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_00233660", func_00237D08);

u32 mdlRunViewerEffectEditorTask(void) {
    mdlUpdateViewerMarkEditorInput();
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

/* Submit the intermediate packet with an identity basis in phases 2 and 3. */
void mdlSubmitViewerIntermediateDrawPacket(void) {
    s32 packetList;

    if (mdlViewerState.taskPhase < 4) {
        if (mdlViewerState.taskPhase >= 2) {
            packetList = sdfCreateResetPacketList();
            VU0_SET_UNIT_MATRIX(vf28, vf29, vf30, vf31);
            sdfAppendPacket(packetList, func_00348188(D_003C8930, D_003C8990, 6, 0x80));
            D_00380048.submit(&D_00380048, packetList);
        }
    }
}

extern s32 D_004370F8;

extern u8 D_003C89B0[];

extern u8 D_003C89F0[];

extern s32 func_00343188(s32, s32);

/* Submit the selected resource in phase 3; keep the native bitwise input/once-flag test. */
void mdlSubmitViewerResourceDrawPacket(void) {
    s32 packetList;
    s32 drawPacket;
    MdlResource *resource;

    if (mdlViewerState.taskPhase == 3) {
        packetList = sdfCreateResetPacketList();
        resource = mdlViewerState.resources[0];
        VU0_LOAD_MATRIX(resource->chunk + 0x20);
        drawPacket = func_00348188(D_003C89B0, D_003C89F0, 4, 0x80);
        if ((sdfPadButtonStates[13] < 0) & (D_004370F8 == 0)) {
            D_004370F8 = 1;
            func_00343188(drawPacket, 0x100);
        }
        sdfAppendPacket(packetList, drawPacket);
        D_00380048.submit(&D_00380048, packetList);
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

/* Walk native 64-byte map-position records and submit their visualization in one packet list. */
void mdlDrawMapPositionRecords(MdlResource *resource) {
    s32 recordCount = sdfCountMapPositionRecords((s32)resource->chunk);

    if (recordCount > 0) {
        s32 recordIndex = 0;
        s32 packetList = sdfCreateResetPacketList();
        s32 recordCursor = sdfChunkFindByTag((s32)resource->chunk, MDL_MAP_POSITION_TAG) + MDL_MAP_POSITION_DATA_OFFSET;

        do {
            s32 currentRecord = recordCursor;

            recordIndex++;
            recordCursor += MDL_MAP_POSITION_RECORD_BYTES;
            sdfSetLookAtBasisFromRecord((s32)resource->chunk, currentRecord);
            sdfAppendPacket(packetList, func_00348188(D_003C8A00, D_003C8A60, 6, 0x80));
        } while (recordIndex != recordCount);
        D_00380048.submit(&D_00380048, packetList);
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
extern s32 fileQueueDefaultCallbackRequest(char *path);
extern void fileWaitReady(s32 file);
extern s32 fileGetResourceHandle(s32 file);
extern char *fileGetLoadedDataAddress(s32 file);
extern s32 fileGetResourceSize(s32 file);
extern void filePollEntryCleanup(s32 file);
extern s32 func_0035C8F8();
extern s32 memcmp(const void *, const void *, u32);

/* Read the viewer's config text file: bg-color=, eye-position=, target-position=, fovy=, fog= lines. */
void mdlLoadViewerPresentationConfig(void) {
    s32 fileRequest;
    s32 resourceHandle;
    char *fileData;
    s32 fileSize;
    s32 lineOffset;
    s32 nextLineOffset;
    char *lineStart;
    u32 color;
    f32 x;
    f32 y;
    f32 z;
    f32 verticalFov;
    s32 fogNear;
    f32 fogValue;
    s32 fogFar;
    f32 fogFarB;

    if (sdfPathExists(D_003C88A8) == 0) {
        return;
    }
    fileRequest = fileQueueDefaultCallbackRequest(D_003C88A8);
    fileWaitReady(fileRequest);
    resourceHandle = fileGetResourceHandle(fileRequest);
    fileData = fileGetLoadedDataAddress(fileRequest);
    fileSize = fileGetResourceSize(fileRequest);
    filePollEntryCleanup(fileRequest);
    lineOffset = 0;
    while (lineOffset < fileSize) {
        nextLineOffset = lineOffset;
        while (nextLineOffset < fileSize) {
            if (fileData[nextLineOffset++] == '\n') {
                break;
            }
        }
        lineStart = fileData + lineOffset;
        if (memcmp(lineStart, "bg-color=", 9) == 0) {
            if (func_0035C8F8(lineStart + 9, D_00437100, &color) == 1) {
                D_00435CBC = color;
            }
        } else if (memcmp(lineStart, "eye-position=", 13) == 0) {
            if (func_0035C8F8(lineStart + 13, "%f,%f,%f", &x, &y, &z) == 3) {
                D_003C87C0.x = x;
                D_003C87C0.y = y;
                D_003C87C0.z = z;
            }
        } else if (memcmp(lineStart, "target-position=", 16) == 0) {
            if (func_0035C8F8(lineStart + 16, "%f,%f,%f", &x, &y, &z) == 3) {
                D_003C87D0.x = x;
                D_003C87D0.y = y;
                D_003C87D0.z = z;
            }
        } else if (memcmp(lineStart, D_00437108, 5) == 0) {
            if (func_0035C8F8(lineStart + 5, D_00437110, &verticalFov) == 1) {
                sdfSceneProjectionParameters[3] = verticalFov;
            }
        } else if (memcmp(lineStart, D_00437118, 4) == 0) {
            if (func_0035C8F8(lineStart + 4, "%d,%f,%d,%f,%x", &fogNear, &fogValue, &fogFar, &fogFarB, &color) == 5) {
                kwlnDrawVector.near = fogNear;
                kwlnDrawVector.value = fogValue;
                kwlnDrawVector.farA = fogFar;
                kwlnDrawVector.farB = fogFarB;
                kwlnDrawVector.color = color;
            }
        }
        lineOffset = nextLineOffset;
    }
    sdfReleaseResourceAllocation(resourceHandle);
}

INCLUDE_ASM(const s32, "game/code_00233660", func_00238BD8);

INCLUDE_ASM(const s32, "game/code_00233660", func_00238D38);

/* Clear each occupied resource slot before destroying its context; retain the native scan form. */
void mdlFreeViewResources(void) {
    MdlViewState *viewerState = &mdlViewerState;
    MdlResource **resourceSlot = viewerState->resources;
    s32 resourceIndex;

    for (resourceIndex = 0; resourceIndex != MDL_VIEWER_RESOURCE_SLOTS; resourceIndex++) {
        MdlResource *resource = *resourceSlot;

        if (resource != 0) {
            *resourceSlot = 0;
            mdlDestroyContext(resource);
        }
        resourceSlot++;
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
        if (kwlnTaskGetTaskByName((void *)def->name) == 0) {
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

    fldDrawPackedRgbEditor(mdlViewerControlState.unk08, first, second,
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
    if (dds3GetObjectBaseResourceHandle(obj)->motion != NULL) {
        mdlAddEntryFlagged(dds3GetObjectBaseResourceHandle(obj), 0, 0);
    }
    func_001129C8(obj, 0);
    return world;
}

extern s32 dds3FindWorldObjectNodeByKey(s32 world, s32 a, s32 b);

extern void dds3SetSlotByKind(s32 obj, s32 slot);

extern void dds3RegisterObjectInHandlerIndex(s32 obj);

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
    mdlAddEntryFlagged(dds3GetObjectBaseResourceHandle(obj), 0, 0);
    dds3SetSlotByKind(obj, dds3FindWorldObjectNodeByKey(dds3GetWorldSecondaryObject(), 0x10000, 2));
    dds3RegisterObjectInHandlerIndex(obj);
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

