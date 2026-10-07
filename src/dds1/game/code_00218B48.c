#include "kwln.h"
#include "common.h"
#include "sdf.h"
#include "mdl.h"

#include "pcp_vu0.h"
#include "ee_mmi.h"
#include "dat_state.h"
#include "sdf_draw.h"
#include "eff.h"
#include "sdf_sif_command.h"

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


/* Viewer-wide state for the model viewer task (DDS1 game/code_00218B48 and
 * DDS2 game/code_00233660 share this layout field for field). Fields that are
 * only written by a defaults initialiser and never read in either game are
 * left as unkNN on purpose: there is no read to earn a role from. */
typedef struct MdlViewState {
    s32 unk00;
    s32 viewerTask;
    s8 unk08;
    s8 unk09;
    s8 taskPhase; /* 0x0A: one-based index into D_00367A40 */
    s8 unk0B;
    s8 unitStepMode;  /* 0x0C: toggled by the step button (D_003D24510[0x23]) */
    s8 unitStepSign;  /* 0x0D: +1/-1, derived from the input keys at 0x24/0x25 */
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
    s16 activeEntryId;    /* 0x22: assigned from selectedEntryId when adding */
    s16 selectedEntryId;  /* 0x24: key passed to the mdlAddEntry* family;
                           signed in DDS1, unsigned in DDS2. */
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
    s16 editorMode; /* 0x40: 0 selects records, 1 edits a mark record */
    s16 unk42;
    s16 unk44;
    s16 markFieldCursor; /* 0x46: selected row in the mark parameter editor */
    s16 unk48;
    s16 unk4A;
    s16 unk4C;
    s16 unk4E;
    s16 unk50;
    s16 unk52;
    f32 viewerScale;      /* 0x54: viewer zoom, in hundredths */
    u8 pad58[0x38];
    MdlCtx *resources[MDL_VIEWER_RESOURCE_SLOTS]; /* 0x90 */
    s32 packetList; /* 0xC0: drawing packet destination */
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

static inline s8 mdlGetViewerDisplayMode(MdlViewState *state) {
    return state->unk0F;
}

extern MdlCtrlState mdlViewerControlState;


extern s32 D_003D7B10[];

extern s8 D_003D7A60[];

typedef struct MdlViewerTaskDef {
    const char *name;
    TaskUpdate update;
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

extern char D_003ABC98[]; /* "MODEL : %d-%03d " */

extern char D_003ABCB0[]; /* "MODEL : %d-%03x " */

extern char D_003ABCC8[]; /* "%02d/%02d" */

extern char D_003ABCD8[]; /* "%02x/%02x" */

extern char D_003BBC28[]; /* "" */

extern s32 mdlViewer(KwlnTask *);

extern s32 mdlViewerEnd(KwlnTask *);

s32 func_0011D3E8(s32, s32, s32, s32, s32, s32, s32);

void fldDrawPackedRgbEditor(void *, s32, s32, s32, u32, s32);

void effApplyNodeScale(s32, float);

s32 effCreateNodeFromDescriptor(s32);

void billSetChildScaleComponents(s32, float, float);

s32 billCreateIndexed(s32, s32);

s32 mdlBuildViewerRectanglePacket(s32, s32, s32, s32, s32);

s32 mdlUpdateViewerCursor(s16 *, s32);

extern void sdfPktInit(SifCommand *, s32, s32, s32, s32);

extern void *sdfFormatSifPacket(void *, const char *, ...);


extern s32 mdlGetNodeField2C(MdlCtx *, s32);

void mdlUpdateViewerSelectedModelFromPad(void);

void mdlDrawViewerSelectionLabel(void);

void sdfAppendPacket(SdfListHead *, u32);


void sdfStreamCreateWithParams(s32, s32, s32, s32, s32);

extern s32 kwlnTaskGetTaskByName(void *name);

extern void kwlnTaskDestroyWithHierarchy(s32 task, s32 flag);

extern KwlnTask *kwlnTaskCreate(const char *, u32, s32, s32, TaskUpdate, TaskDestroy, u32);

void dds3AdminSetControlFlag(void);

void func_00101A80(s32, s32);

void mdlCleanupViewerTasksAndResources(void);

void sdfPacInitializeDispatchPacket(void *buffer, s32);

void func_002EDC30(void *buffer);

s32 sdfPacFeedInput(void *buffer, void *input, s32 available);

void func_00218768(s32, s32, s32, s32);

void func_002EDC50(void *buffer);


s32 mdlCountRecords(s32);

extern u32 D_003BA8EC;

extern void kwlnDebugGraphSetEnabled(s8 mode);

extern s32 fldStepColorChannelByPad(u32 *color, s32 channel, s8 *pad);

extern void fldAdjustIntegerUsingMainPad(void *ptr, s32 type, s64 min, s64 max, s64 step, s64 bigStep);

extern s8 D_00324510[];

extern void fldStepIntByPad(void *ptr, s32 type, s64 min, s64 max, s64 small, s64 big, s8 *pad);

extern void sdfReleaseChipBlock();

/* Three independently allocated resources; their individual roles are unresolved. */
typedef struct MdlSlotEntry {
    s32 firstHandle;
    s32 secondHandle;
    s32 thirdHandle;
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
        func_002EDC30(&request);
    }
    sdfPacFeedInput(&request, (void *)requestFirst, requestSecond);
    func_00218768(request.handle, first, second, flags);
    func_002EDC50(&request);
}

void func_00218BE8(s32 resource) {
    sdfReleaseChipBlock((void *)resource);
}

/* Release all three handles in each viewer-table entry, then clear its tables and backing allocations. */
void mdlReleaseViewerSlotResources(void) {
    MdlSlotEntry *slotEntry;
    s32 entryCount;
    s32 entryIndex;

    entryCount = D_00365858[MDL_VIEWER_TABLE_SLOT].count;
    if (entryCount > 0) {
        slotEntry = D_00365858[MDL_VIEWER_TABLE_SLOT].entries;
        entryIndex = 0;
        do {
            entryIndex++;
            sdfReleaseChipBlock(slotEntry->secondHandle);
            sdfReleaseChipBlock(slotEntry->firstHandle);
            sdfReleaseChipBlock(slotEntry->thirdHandle);
            slotEntry++;
        } while (entryIndex < entryCount);
    }
    D_00367900[MDL_VIEWER_TABLE_SLOT].entries = NULL;
    D_00367900[MDL_VIEWER_TABLE_SLOT].count = 0;
    D_00365858[MDL_VIEWER_TABLE_SLOT].entries = NULL;
    D_00365858[MDL_VIEWER_TABLE_SLOT].count = 0;
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

/* Copy the three source strings into the viewer table, preserving the first/second handle ordering. */
void mdlInitializeViewerResourceTable(void) {
    s32 sourceIndex;
    char *sourceText;
    char *copiedText;

    mdlReleaseViewerSlotResources();
    D_003BBB6C = sdfAllocGeneralBlock(MDL_VIEWER_HEADER_BYTES);
    D_003BD880 = (MdlViewerHeader *)sdfResourceRetainAddress(D_003BBB6C);
    D_003BD880->kind = 5;
    D_003BD880->unk02 = 0;
    D_003BD880->unk04 = 0x1000;
    D_003BD880->unk06 = 0x3E8;
    D_003BBB70 = sdfAllocGeneralBlock(MDL_VIEWER_NAME_TABLE_BYTES);
    D_003BD884 = (char **)sdfResourceRetainAddress(D_003BBB70);
    for (sourceIndex = 0; sourceIndex != MDL_VIEWER_LABEL_COUNT; sourceIndex++) {
        switch (sourceIndex) {
        case 0:
            sourceText = (char *)D_00365858[0].entries->secondHandle;
            break;
        case 1:
            sourceText = (char *)D_00365858[0].entries->firstHandle;
            break;
        default:
            sourceText = (char *)D_00365858[0].entries->thirdHandle;
            break;
        }
        if (sourceText != NULL) {
            copiedText = sdfAllocSizeClassBlock(strlen(sourceText) + 1);
            strcpy(copiedText, sourceText);
            switch (sourceIndex) {
            case 0:
                D_003BD884[1] = copiedText;
                break;
            case 1:
                D_003BD884[0] = copiedText;
                break;
            case 2:
                D_003BD884[2] = copiedText;
                break;
            }
        }
    }
    D_00367900[MDL_VIEWER_TABLE_SLOT].entries = (MdlSlotEntry *)D_003BD880;
    D_00367900[MDL_VIEWER_TABLE_SLOT].count = 1;
    D_00365858[MDL_VIEWER_TABLE_SLOT].entries = (MdlSlotEntry *)D_003BD884;
    D_00365858[MDL_VIEWER_TABLE_SLOT].count = 1;
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_00218E20);

/* Relative-linked record prefix with kind-dependent packed payload words.
 * Kinds 1/2 use the count/part-index halfwords at 0x0C/0x0E; kind 3 reads
 * the whole parameter word. Kind 4 reads both selector halfwords at 0x08/0x0A. */
typedef struct MdlRecord {
    s32 kind;        /* 0x00: 0xFFFF terminates record traversal */
    s32 nextOffset;  /* 0x04: relative byte offset to next record */
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





/* Read the model record's payload word without advancing its relative link. */
u32 mdlGetViewerRecordPayloadWord(MdlRecord *record) {
    return record->payload.word;
}

/* The part-list count is the low half of the packed parameter word. */
u16 mdlGetViewerRecordListCount(MdlRecord *record) {
    return record->parameter.part.count;
}

/* Follow the relative links in a resource's record table to find an ID. */
MdlRecord *mdlFindViewerRecord(MdlCtx *resource, s32 recordId) {
    MdlRecord *recordTable = resource->sub->partInfo;
    MdlRecord *recordCursor;
    s32 remainingRecords;

    if (recordTable == NULL) {
        return NULL;
    }
    recordCursor = recordTable;
    remainingRecords = mdlGetViewerRecordListCount(recordCursor);
    do {
        remainingRecords--;
        recordCursor = (MdlRecord *)((s32)recordCursor + recordCursor->nextOffset);
        if (remainingRecords == -1) {
            return NULL;
        }
    } while (recordCursor->kind != recordId);
    return recordCursor;
}

/* The first eight bytes belong to the enclosing list, not its first record. */
MdlRecord *mdlGetFirstRecord(s32 listAddress) {
    MdlRecord *firstRecord = (MdlRecord *)(listAddress + MDL_RECORD_LIST_HEADER_BYTES);

    if (firstRecord->kind == MDL_RECORD_END_KIND) {
        firstRecord = NULL;
    }
    return firstRecord;
}

/* A terminal kind returns NULL; the link is a byte offset, not a pointer. */
MdlRecord *mdlGetNextRecord(MdlRecord *currentRecord) {
    MdlRecord *nextRecord = (MdlRecord *)((s32)currentRecord + currentRecord->nextOffset);

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

u16 func_002193E8(MdlRecord *record) {
    return record->parameter.part.partIndex;
}

u16 func_002193F0(MdlRecord *record) {
    return record->unk10;
}

extern void *sdfCreateFormattedSifCommand(s32 source, s32 end, s32 argument, s32 index, const char *format, ...);
extern char D_003ABA80[], D_003ABA90[], D_003ABAA0[], D_003ABAB0[], D_003ABAC0[], D_003ABAD0[];
extern char D_003BBB80[], D_003BBB88[];

/* Append the kind-dependent record summary at the requested text position and style. */
INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABA80);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABA90);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABAA0);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABAB0);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABAC0);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABAD0);

void func_002193F8(s32 packetList, s32 x, s32 y, s32 depth, s32 textStyle, MdlRecord *record) {
    if (record->kind >= 1 && record->kind <= 5) {
        switch (record->kind) {
        case 1:
        case 2: {
            char *label = record->kind == 1 ? D_003BBB80 : D_003BBB88;

            if (record->parameter.part.count == 1) {
                sdfAppendPacket((SdfListHead *)(packetList), (u32)(sdfCreateFormattedSifCommand(x, y, depth, textStyle,
                                                                    D_003ABA80, label, record->payload.word,
                                                                    record->parameter.part.partIndex)));
                return;
            }
            sdfAppendPacket((SdfListHead *)(packetList), (u32)(sdfCreateFormattedSifCommand(x, y, depth, textStyle,
                                                                D_003ABA90, label, record->payload.word,
                                                                record->payload.word + record->parameter.part.count - 1,
                                                                record->parameter.part.partIndex)));
            return;
        }
        case 3:
            sdfAppendPacket((SdfListHead *)(packetList), (u32)(sdfCreateFormattedSifCommand(x, y, depth, textStyle,
                                                                D_003ABAA0, record->payload.word,
                                                                record->parameter.word)));
            return;
        case 4:
            sdfAppendPacket((SdfListHead *)(packetList), (u32)(sdfCreateFormattedSifCommand(x, y, depth, textStyle,
                                                                D_003ABAB0, record->payload.stream.selectorA,
                                                                record->payload.stream.selectorB)));
            return;
        case 5:
            sdfAppendPacket((SdfListHead *)(packetList), (u32)(sdfCreateFormattedSifCommand(x, y, depth, textStyle,
                                                                D_003ABAC0, record->payload.word)));
            return;
        }
    } else {
        sdfAppendPacket((SdfListHead *)(packetList), (u32)(sdfCreateFormattedSifCommand(x, y, depth, textStyle,
                                                            D_003ABAD0, record->kind)));
    }
}

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

/* Draw the scalar marker fields, then four colors with four byte channels each; negative selection hides the cursor. */
void mdlDrawMarkParamsPanel(s32 packetList, s32 x, s32 y, s32 depth, EffMarkParams *params, s32 selectedField) {
    s32 colorY = y + 0x300;
    s32 labelX = x + 0xC0;
    u32 packedColor;
    s32 colorIndex;
    s32 channelIndex;

    sdfAppendPacket((SdfListHead *)(packetList), (u32)(sdfCreateFormattedSifCommand(labelX, y, depth, 0, "MARK0:%d", params->mark0)));
    sdfAppendPacket((SdfListHead *)(packetList), (u32)(sdfCreateFormattedSifCommand(labelX, y + 0x60, depth, 0, "MARK1:%d", params->mark1)));
    sdfAppendPacket((SdfListHead *)(packetList), (u32)(sdfCreateFormattedSifCommand(labelX, y + 0xC0, depth, 0, "START:%d", params->start)));
    sdfAppendPacket((SdfListHead *)(packetList), (u32)(sdfCreateFormattedSifCommand(labelX, y + 0x120, depth, 0, "END  :%d", params->end)));
    sdfAppendPacket((SdfListHead *)(packetList), (u32)(sdfCreateFormattedSifCommand(labelX, y + 0x180, depth, 0, "ITRVL:%d", params->interval)));
    sdfAppendPacket((SdfListHead *)(packetList), (u32)(sdfCreateFormattedSifCommand(labelX, y + 0x1E0, depth, 0, "FACE :%d", params->face)));
    sdfAppendPacket((SdfListHead *)(packetList), (u32)(sdfCreateFormattedSifCommand(labelX, y + 0x240, depth, 0, "BLEND:%d", params->blend)));
    for (colorIndex = 0; colorIndex != MDL_MARK_COLOR_COUNT; colorIndex++) {
        packedColor = params->colors[colorIndex];
        sdfAppendPacket((SdfListHead *)(packetList), (u32)(func_0011D3E8(x + 0x480, colorY, depth, 0x300, 0x180, (packedColor & 0xFFFFFF) | 0x80000000, 0x60404040)));
        for (channelIndex = 0; channelIndex != MDL_COLOR_CHANNEL_COUNT; channelIndex++) {
            sdfAppendPacket((SdfListHead *)(packetList), (u32)(sdfCreateFormattedSifCommand(labelX, colorY, depth, D_003679B0[channelIndex], D_003679B8[channelIndex])));
            sdfAppendPacket((SdfListHead *)(packetList), (u32)(sdfCreateFormattedSifCommand(x + 0x240, colorY, depth, 0, D_003BBBB0, packedColor & MDL_COLOR_BYTE_MASK)));
            packedColor >>= MDL_COLOR_BYTE_BITS;
            colorY += 0x60;
        }
        colorY += 0x60;
    }
    if (selectedField >= 0) {
        sdfAppendPacket((SdfListHead *)(packetList), (u32)(sdfCreateFormattedSifCommand(x, y + D_00367980[selectedField], depth, 0, D_003BBBB8)));
    }
}

/* Edit the marker parameters with the pad: rows 0-6 are the scalar fields, 7-22 the color bytes. */
void mdlEditMarkParametersWithPad(EffMarkParams *params, s16 *fieldCursor) {
    s32 selectedField = *fieldCursor;
    u8 *colorChannel;

    if (D_00324510[0x27] < 0) {
        selectedField = selectedField != MDL_MARK_LAST_FIELD ? selectedField + 1 : 0;
    } else if (((u8)D_00324510[0x27] & MDL_PAD_REPEAT_FLAG) != 0 && selectedField < MDL_MARK_LAST_FIELD) {
        selectedField++;
    } else if (D_00324510[0x26] < 0) {
        selectedField = selectedField != 0 ? selectedField - 1 : MDL_MARK_LAST_FIELD;
    } else if (((u8)D_00324510[0x26] & MDL_PAD_REPEAT_FLAG) != 0 && selectedField > 0) {
        selectedField--;
    } else {
        switch (selectedField) {
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
            colorChannel = (u8 *)params->colors + selectedField - MDL_MARK_COLOR_FIRST_FIELD;
            if (D_00324510[0x25] < 0) {
                *colorChannel += 1;
            } else if (((u8)D_00324510[0x25] & MDL_PAD_REPEAT_FLAG) != 0 && *colorChannel < MDL_COLOR_BYTE_MASK) {
                *colorChannel += 1;
            } else if (D_00324510[0x24] < 0) {
                *colorChannel -= 1;
            } else if (((u8)D_00324510[0x24] & MDL_PAD_REPEAT_FLAG) != 0 && *colorChannel != 0) {
                *colorChannel -= 1;
            }
            break;
        }
    }
    *fieldCursor = selectedField;
}

typedef struct MdlPartEntry {
    u32 kind;     /* 0x00: billboard, effect, or object */
    s32 state;    /* 0x04 */
    s32 object;   /* 0x08 */
    u8 pad0C[4];
} MdlPartEntry;


#define MDL_PART_BILLBOARD 0

#define MDL_PART_EFFECT 1

#define MDL_PART_OBJECT 3

/* Append a newly created billboard to the next part-list slot. */
void mdlAddBillboardPart(DevRequest *partList, s32 descriptorIndex) {
    MdlPartEntry *partEntry = &((MdlPartEntry *)partList->buffer)[partList->usedCount];

    partEntry->state = 0;
    partEntry->kind = MDL_PART_BILLBOARD;
    partEntry->object = billCreateIndexed(1, descriptorIndex);
    partList->usedCount += 1;
}

/* Append a newly created effect to the next part-list slot. */
void mdlAddEffectPart(DevRequest *partList, s32 descriptorIndex) {
    MdlPartEntry *partEntry = &((MdlPartEntry *)partList->buffer)[partList->usedCount];

    partEntry->kind = MDL_PART_EFFECT;
    partEntry->state = 0;
    partEntry->object = effCreateNodeFromDescriptor(descriptorIndex);
    partList->usedCount += 1;
}

extern void *sdfAllocAndClearQuadwords(s32 size);

typedef struct MdlHandlerNode {
    s32 a;      /* 0x00 */
    void *b;    /* 0x04 */
    u8 pad08[8];
    s32 c;      /* 0x10 */
    u8 pad14[0x98];
} MdlHandlerNode;

void mdlAppendObjectPart(DevRequest *list, s32 a, void *b, s32 c) {
    MdlHandlerNode *node = sdfAllocAndClearQuadwords(0xAC);
    MdlPartEntry *entry = &((MdlPartEntry *)list->buffer)[list->usedCount];

    node->c = c;
    node->a = a;
    node->b = b;
    entry->kind = MDL_PART_OBJECT;
    entry->state = 0;
    entry->object = (s32)node;
    list->usedCount += 1;
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

extern void sdfDestroyDevRequest(DevRequest *request);

DevRequest *mdlCreateBufferedPartRequest(u32 request) {
    return sdfDevCreateBufferedRequest(request, 0x10, 4);
}

/* Destroy each part according to its stored kind, then release the owning request. */
void mdlDestroyPartList(DevRequest *partList) {
    s32 partIndex;

    if (partList != NULL) {
        for (partIndex = 0; partIndex < partList->usedCount; partIndex++) {
            MdlPartEntry *partEntry = &((MdlPartEntry *)partList->buffer)[partIndex];

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
MdlResourceItem *mdlInsertResourceItem(MdlCtx *owner, s32 type, s32 subtype) {
    MdlResourceItem *item = sdfAllocAndClearQuadwords(sizeof(MdlResourceItem));
    MdlResourceItem *previousHead = owner->resourceItems;
    item->type = type;
    item->next = previousHead;
    item->subtype = subtype;
    owner->resourceItems = item;
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
s32 mdlFindViewerPartSlot(MdlCtx *resource, s32 slotIndex) {
    DevRequest *slotTable;

    slotTable = resource->sub->partList;
    if (slotTable == NULL) {
        return 0;
    }
    if (slotIndex >= slotTable->usedCount) {
        return 0;
    }
    return (s32)slotTable->buffer + slotIndex * MDL_PART_SLOT_BYTES;
}

typedef struct MdlPartRec {
    u8 pad00[4];
    u32 size;       /* 0x04 */
    s32 firstId;    /* 0x08 */
    u16 count;      /* 0x0C */
    u16 partIndex;  /* 0x0E */
    f32 value;      /* 0x10 */
} MdlPartRec;


extern void *sdfChunkFindRecordById(void *chunk, s32 id);

/* Bind each consecutive record ID to a newly created part when the chunk contains it. */
void mdlBindViewerPartRecords(MdlCtx *owner, MdlPartRec *partRecord, s32 subtype, s32 type, s32 (*createPart)(MdlPartEntry *)) {
    MdlPartEntry *partSlot = (MdlPartEntry *)mdlFindViewerPartSlot(owner, partRecord->partIndex);

    if (partSlot != NULL) {
        void *chunk = owner->inner;
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
                resourceItem->payload.part.slot = partSlot;
                resourceItem->payload.part.record = chunkRecord;
                resourceItem->payload.part.value = optionalValue;
            }
        } while (--remainingRecords != 0);
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


/* Convert the effect record to native track parameters and retain its work pointer. */
void mdlCreateViewerEffectPart(MdlCtx *owner, MdlEffectRec *effectRecord, s32 subtype) {
    EffTrackPolyParams effectParams;
    MdlResourceItem *resourceItem;

    effectParams.model = owner;
    effectParams.idA = effectRecord->effectId;
    effectParams.idB = effectRecord->param0C;
    effectParams.unk0C = effectRecord->scaleX;
    effectParams.unk10 = effectRecord->scaleY;
    effectParams.sampleInterval = effectRecord->value14;
    effectParams.historyLength = effectRecord->value16;
    effectParams.unk1C = 3;
    effectParams.kind = effectRecord->value17;
    effectParams.gradientColors[0] = effectRecord->value18;
    effectParams.gradientColors[1] = effectRecord->value1C;
    effectParams.gradientColors[2] = effectRecord->value20;
    effectParams.gradientColors[3] = effectRecord->value24;
    resourceItem = mdlInsertResourceItem(owner, MDL_RESOURCE_TRACK_POLY, subtype);
    resourceItem->payload.part.track = effTrackPolyCreateWork(&effectParams);
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


typedef struct MdlEntryRec {
    u8 pad00[8];
    s32 dataId;   /* 0x08 */
    s32 minimumTime; /* 0x0C: deferred initialization threshold */
    u8 flagB;     /* 0x10 */
    u8 flagA;     /* 0x11 */
    u16 index;    /* 0x12 */
} MdlEntryRec;

typedef struct MdlSlotRec {
    u8 pad00[8];
    MdlObj *obj; /* 0x08 */
} MdlSlotRec;

extern void *sdfFindResourceById(s32 id);

/* Claim an unused object only after its data resource resolves, retaining record flags and deferred-init parameters. */
void mdlClaimViewerObjectPart(MdlCtx *owner, MdlEntryRec *entryRecord, s32 subtype) {
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
                /* Fill the object and resource data before attaching the owner. */
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
        f32 motionTime = ((MdlResourceItem *)itemAddress)->payload.object.owner->first->currentFrame;
        if ((u32)(s32)motionTime < (u32)minimumTime) {
            return;
        }
        mdlObjInit(objectAddress, ((MdlResourceItem *)itemAddress)->payload.object.data, (s32)((MdlResourceItem *)itemAddress)->payload.object.attributes);
    }
}

/* Dispatch the five record kinds, retaining the game's native return behavior. */
void mdlDispatchResourceEntry(MdlCtx *owner, MdlRecord *record, s32 subtype) {
    switch (record->kind) {
    case 1:
        mdlBindViewerPartRecords(owner, (MdlPartRec *)record, subtype, MDL_RESOURCE_BILLBOARD, mdlAdvanceBillboardPart);
        return;
    case 2:
        mdlBindViewerPartRecords(owner, (MdlPartRec *)record, subtype, MDL_RESOURCE_EFFECT, mdlAdvanceEffectPart);
        return;
    case 3:
        mdlCreateViewerEffectPart(owner, (MdlEffectRec *)record, subtype);
        return;
    case 4:
        mdlLoadViewerStreamRecord((u32)owner, (s32)record);
        return;
    case 5:
        mdlClaimViewerObjectPart(owner, (MdlEntryRec *)record, subtype);
        break;
    }
}

/* Find the requested record list and apply each relative-linked entry with the supplied subtype. */
void mdlApplyResourceEntries(s32 resourceAddress, s32 recordId, s32 subtype) {
    MdlRecord *recordList = mdlFindViewerRecord((MdlCtx *)resourceAddress, recordId);
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
        effTrackPolyRelease(item->payload.part.track);
        break;
    }
    sdfReleaseChipBlock((void *)item);
}

/* Unlink matching subtypes without losing the incoming link when consecutive items are removed. */
void mdlRemoveResourceSubtype(MdlCtx *owner, s32 subtype) {
    MdlResourceItem **itemLink = &owner->resourceItems;
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

typedef struct MdlNodeInfo {
    s32 id;       /* 0x00 */
    u8 pad04[0xC];
    f32 pos[4];   /* 0x10 */
} MdlNodeInfo;


extern u8 sdfViewTargetVector[];

extern u8 *sdfModelFindDrawNode(void *chunk, s32 id);

/* vu0 routine: positionOut = p + normalize(p - sdfViewTargetVector) * scale, p = transformed node position. */
void mdlResolveAnchorPosition(void *chunk, MdlResourceItem *anchorRecord, f32 *positionOut) {
    MdlNodeInfo *nodeInfo = anchorRecord->payload.part.record;
    u8 *drawNode = sdfModelFindDrawNode(chunk, nodeInfo->id);
    f32 scale = anchorRecord->payload.part.value;

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

extern void effCopyVector(s32 handle, f32 *src);

extern void billInvokeCallback(s32 handle);


extern void effUpdateNode(s32 handle);


/* Update anchored billboard/effect positions, tracked polygons, or deferred object initialization by type. */
void mdlDispatchViewerAnchorRecord(MdlCtx *owner, MdlResourceItem *anchorRecord) {
    void *chunk = owner->inner;
    f32 position[4];
    s32 resourceHandle;

    switch (anchorRecord->type) {
    case 0:
        mdlResolveAnchorPosition(chunk, anchorRecord, position);
        resourceHandle = anchorRecord->payload.part.handle;
        effCopyVector(resourceHandle, position);
        billInvokeCallback(resourceHandle);
        break;
    case 1:
        mdlResolveAnchorPosition(chunk, anchorRecord, position);
        resourceHandle = anchorRecord->payload.part.handle;
        effCopyVectorToNodeInstance((struct EffNode *)resourceHandle, position);
        effUpdateNode(resourceHandle);
        break;
    case 2:
        effTrackPolyUpdate(anchorRecord->payload.part.track);
        break;
    case 3:
        mdlCondInitEntry((s32)anchorRecord);
        break;
    }
}

/* Set a billboard/effect frame; other resource-item kinds have no frame dispatch. */
void mdlSetResourceFrame(MdlCtx *owner, MdlResourceItem *item, s32 frame) {
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
void mdlSetResourceAmount(MdlCtx *owner, MdlResourceItem *item, float amount) {
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
    return func_0011D3E8(x, y, depth, width, height, 0x30000000, 0x60404040);
}

void mdlAppendViewerRectToDrawList(s32 x, s32 y, s32 depth, s32 width, s32 height, s32 unused) {
    s32 packet;

    packet = D_003D7B10[0];
    sdfAppendPacket((SdfListHead *)(packet), (u32)(mdlBuildViewerRectanglePacket(x, y, depth, width, height)));
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


extern MdlCtx *func_00217680(s16, s16);

extern void mdlAddEntryFlagged(void *, s32, s32);

void mdlLoadViewerResourceAndResetCursors(void) {
    MdlCtx *resource = func_00217680(mdlViewerState.resourceGroup, mdlViewerState.resourceId);

    mdlViewerState.resources[0] = resource;
    mdlViewerState.activeEntryId = 0;
    mdlViewerState.selectedEntryId = 0;
    mdlViewerState.selectedNodeId = 0;
    mdlViewerState.unk28 = 0;
    mdlViewerState.unk2A = 0;
    mdlViewerState.unk2C = 0;
    if (resource->first != NULL) {
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
    s32 resourceIndex = mdlViewerState.resourceCount - 1;
    MdlCtx *lastResource = mdlViewerState.resources[resourceIndex];

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
    MdlCtx *firstResource = mdlViewerState.resources[0];

    for (resourceIndex = 0; resourceIndex < resourceCount - 1; resourceIndex++) {
        mdlViewerState.resources[resourceIndex] = mdlViewerState.resources[resourceIndex + 1];
    }
    mdlViewerState.resources[resourceIndex] = firstResource;
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021A9F8);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021AAB8);

void mdlDrawViewerModelAndMotionSummary(void) {
    SifCommand packet;
    const char *format;
    s32 displayMode;
    s32 nodeCount;
    void *formatted;

    mdlAppendViewerRectToDrawList(0x7E10, 0x8608, 0xFF0080, 0xEA0, 0x90, 0);
    sdfPktInit(&packet, 0x7E40, 0x8620, 0xFF0080, 0);

    if ((displayMode = mdlGetViewerDisplayMode(&mdlViewerState)) == 0) {
        format = D_003ABC98;
    } else {
        format = D_003ABCB0;
    }
    sdfAppendPacket((SdfListHead *)(mdlViewerState.packetList), (u32)(sdfFormatSifPacket(&packet, format, mdlViewerState.resourceGroup, mdlViewerState.resourceId)));

    nodeCount = mdlGetNodeRefHalf(mdlViewerState.resources[0], 0);
    if (nodeCount == 0) {
        formatted = sdfFormatSifPacket(&packet, D_003BBC28);
    } else {
        if (mdlViewerState.unk0F == 0) {
            format = D_003ABCC8;
        } else {
            format = D_003ABCD8;
        }
        formatted = sdfFormatSifPacket(&packet, format,
                                       mdlGetNodeField2C(mdlViewerState.resources[0], 0), nodeCount - 1);
    }
    sdfAppendPacket((SdfListHead *)(mdlViewerState.packetList), (u32)(formatted));
}

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

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABC98);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABCB0);

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
void mdlUpdateViewerCameraFromPad(void) {
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

extern const char *D_00367B10[];



void mdlDrawViewerIndexedLabelOverlay(void) {
    mdlAppendViewerRectToDrawList(0x8A10, 0x7948, 0xFF007F, 0x4E0, 0x90, 0);
    sdfAppendPacket((SdfListHead *)(mdlViewerState.packetList), (u32)(sdfCreateFormattedSifCommand(0x8A40, 0x7960, 0xFF0080, 0, D_00367B10[mdlViewerState.labelIndexA])));
}

u32 mdlRunViewerIndexedLabelTask(void) {
    mdlUpdateViewerCameraFromPad();
    mdlDrawViewerIndexedLabelOverlay();
    return 0;
}

extern void mdlLoadPrimaryVectorVU(MdlCtx *context);
extern void mdlStorePrimaryVectorVU(MdlCtx *context);
extern void mdlLoadSecondaryVectorVU(MdlCtx *context);
extern void mdlUpdateContextRotationBasisFromQuaternion(MdlCtx *context);
extern f32 D_00398380[4];
extern f32 D_00398390[4];
extern f32 D_003983A0[4];

/* vu0 routine: edit the viewer transform with translation or quaternion steps. */
void mdlUpdateViewerSelectedModelFromPad(void) {
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
        mdlUpdateContextRotationBasisFromQuaternion(mdlViewerState.resources[0]);
    }
}

extern const char *D_00367B18[];

void mdlDrawViewerSelectionLabel(void) {
    mdlAppendViewerRectToDrawList(0x8A10, 0x7948, 0xFF007F, 0x4E0, 0x90, 0);
    sdfAppendPacket((SdfListHead *)(mdlViewerState.packetList), (u32)(sdfCreateFormattedSifCommand(0x8A40, 0x7960, 0xFF0080, 0, D_00367B18[mdlViewerState.labelIndexB])));
}

s32 mdlRunViewerSelectionLabelTask(void) {
    mdlUpdateViewerSelectedModelFromPad();
    if (D_003D7A60[0] == 0) {
        mdlDrawViewerSelectionLabel();
    }
    return 0;
}

/* Change the viewer scale in hundredths, with larger steps at larger values. */
void mdlAdjustViewerScale(void) {
    s32 scaleHundredths = (s32)(mdlViewerState.viewerScale * 100.0f + 0.5f);

    /* Keep the distinct up/down thresholds and the native float expression order. */
    if (D_00324510[0x27] & MDL_PAD_REPEAT_FLAG) {
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
    } else if (D_00324510[0x26] & MDL_PAD_REPEAT_FLAG) {
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
    s32 packetList;
    MdlCtx *resource;
    Motion *motion;
    s32 playheadX;
    s32 textStyle;

    mdlAppendViewerRectToDrawList(0x81D0, 0x7948, 0xFF007F, 0xD20, 0xF0, 0);
    packetList = mdlViewerState.packetList;
    sdfAppendFillRectanglePacket(packetList, 0x80303030, 0, 0x8200, 0x7990, 0x8EC0, 0x7990, 0xFF0080, 0);
    sdfAppendFillRectanglePacket(packetList, 0x80303030, 0, 0x8200, 0x7960, 0x8200, 0x79C0, 0xFF0080, 0);
    sdfAppendFillRectanglePacket(packetList, 0x80303030, 0, 0x8EC0, 0x7960, 0x8EC0, 0x79C0, 0xFF0080, 0);
    resource = mdlViewerState.resources[0];
    motion = resource->first;
    if (motion != NULL) {
        playheadX = (s32)(motion->currentFrame * 3264.0f / motion->frameCount);
        if (playheadX < 0) {
            playheadX = 0;
        }
        playheadX += 0x8200;
        sdfAppendFillRectanglePacket(packetList, 0x800000E0, 0, playheadX, 0x7960, playheadX, 0x79C0, 0xFF0090, 0);
        sdfAppendPacket((SdfListHead *)(mdlViewerState.packetList), (u32)(sdfCreateFormattedSifCommand(0x8200, 0x79C0, 0xFF0080, 0, "[%5.1f/%-3d]", motion->currentFrame, motion->frameCount)));
    } else {
        sdfAppendPacket((SdfListHead *)(mdlViewerState.packetList), (u32)(sdfCreateFormattedSifCommand(0x8200, 0x79C0, 0xFF0080, 0, "[---.-/---]")));
    }
    textStyle = mdlViewerState.unitStepMode != 0 ? 2 : 0;
    sdfAppendPacket((SdfListHead *)(mdlViewerState.packetList), (u32)(sdfCreateFormattedSifCommand(0x8B00, 0x79C0, 0xFF0080, textStyle, D_003BBC80, mdlViewerState.viewerScale)));
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

extern u16 mdlGetContextResourceGroup(MdlCtx *resource);
extern u16 mdlGetContextResourceId(MdlCtx *resource);
extern void mdlDestroyContext(MdlCtx *resource);
extern void sdfMotionInitializeAtZeroTime(void *, s32, s32);

void mdlApplyViewerResourceMenuAction(void) {
    s32 i;
    MdlCtx *resource;
    s32 entryId;
    Motion *motion;

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
            motion = resource->first;
            if (motion != NULL) {
                entryId = resource->current.h.arg;
                if (motion->loopEnabled == 0) {
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
    i = mdlViewerState.resources[0]->current.h.arg;
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
    u8 pad06[6];
    SdfTex **textures;
} MdlCountNode;



void mdlHandleViewerNodeCursorInput(void) {
    MdlCountNode *firstNode = mdlViewerState.resources[0]->inner->assetData;

    if (firstNode != 0) {
        s16 nodeCount = firstNode->count;

        if (nodeCount > 0) {
            mdlUpdateViewerCursor(&mdlViewerState.nodeCursor, nodeCount);
        }
    }
}

extern char D_003ABDA8[];

extern char D_003BBC88[];

extern void sdfConsCreateDrawPacket(s32, SdfTex *, s32);

extern void sdfAppendTexturedLinePacket(s32, u32, s32, s32, s32, s32, s32,
                                        s32, s32, s32, s32, s32, s32);

void mdlDrawViewerTexturePreview(void) {
    s32 index = 0;
    s32 count = 0;
    s32 width, height;
    MdlCountNode *node;
    s32 packetList;
    s32 displayWidth, displayHeight;
    s32 selectedNumber = 0;

    mdlAppendViewerRectToDrawList(0x7150, 0x7A08, 0xFF007F, 0x1060, 0x8F0, 0);
    node = mdlViewerState.resources[0]->inner->assetData;
    if (node != NULL) {
        count = node->count;
        index = mdlViewerState.nodeCursor;
        selectedNumber = index + (count > 0);
    }
    packetList = mdlViewerState.packetList;
    sdfAppendPacket((SdfListHead *)(packetList), (u32)(sdfCreateFormattedSifCommand(0x7180, 0x7A20,
        0xFF0080, 0, D_003ABDA8, selectedNumber, count)));
    if (count > 0) {
        width = node->textures[index]->width;
        height = node->textures[index]->height;
        sdfAppendPacket((SdfListHead *)(packetList), (u32)(sdfCreateFormattedSifCommand(0x7780, 0x7A20,
            0xFF0080, 0, D_003BBC88, width, height)));
        sdfConsCreateDrawPacket(packetList, node->textures[index], 0);
        displayWidth = width << 4;
        displayHeight = height << 3;
        if (width < height) {
            if (height > 256) {
                displayWidth = ((width << 8) / height) << 4;
                displayHeight = 0x800;
            }
        } else {
            if (width > 256) {
                displayWidth = 0x1000;
                displayHeight = ((height << 8) / width) << 3;
            }
        }
        sdfAppendTexturedLinePacket(packetList, 0x80808080, 0, 0x7180, 0x7AE0,
            0, 0, displayWidth + 0x7180, displayHeight + 0x7AE0,
            width << 4, height << 4, 0xFF0080, 0);
    }
}

u32 mdlUpdateViewerNodeCursorTask(void) {
    mdlHandleViewerNodeCursorInput();
    mdlDrawViewerTexturePreview();
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABD58);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABD68);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABD78);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABD88);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABD98);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABDA8);

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

extern const s32 D_00367B38[];
extern const char *D_00367B60[];
extern const char *D_00367B70[];
extern char D_003BBC90[];
extern char D_003BBC98[];
extern char D_003BBCA0[];
extern char D_003BBCA8[];

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABE18);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABE30);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABE48);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABE60);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABE70);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABE80);

void func_0021CB80(void) {
    /* The native prologue fills and then indexes these three status bytes
     * at sp+0..2; this is a real temporary stack table, not a source view. */
    s8 enabled[3];
    s32 packetList;
    s32 i;
    s32 y;
    s32 style;
    s16 value;

    mdlAppendViewerRectToDrawList(0x7150, 0x7A08, 0xFF007F, 0x1320, 0x5D0, 0);
    enabled[0] = mdlViewerState.unk0E;
    packetList = mdlViewerState.packetList;
    enabled[1] = mdlIsDebugTimeGraph();
    enabled[2] = mdlViewerState.unk0F;
    for (i = 0, y = 0x7A80; i < 3; i++, y += 0x60) {
        sdfAppendPacket((SdfListHead *)(packetList), (u32)(sdfCreateFormattedSifCommand(
            0x7300, y, 0xFF0080, 0, D_00367B60[i],
            enabled[i] == 0 ? D_003BBC90 : D_003BBC98)));
    }
    if (mdlViewerState.unk09 < 20) {
        sdfAppendPacket((SdfListHead *)(packetList), (u32)(sdfCreateFormattedSifCommand(
            0x7240, D_00367B38[mdlViewerState.unk3C], 0xFF0080, 0, D_003BBCA0)));
    }
    fldDrawPackedRgbEditor((void *)packetList, 0x7300, D_00367B38[3],
                           mdlViewerState.unk3E == 1 ? mdlViewerState.unk3C - 3 : -1,
                           D_003BA8EC, 0);
    sdfAppendPacket((SdfListHead *)(packetList), (u32)(sdfCreateFormattedSifCommand(
        0x7B40, 0x7C60, 0xFF0080, 0, "BACK COLOR")));
    for (i = 0; i < 3; i++) {
        /* The native channel loop clears the selected channel value before the switch. */
        value = 0;
        switch (i) {
        case 0: value = mdlViewerState.unk4A; break;
        case 1: value = mdlViewerState.unk4C; break;
        case 2: value = mdlViewerState.unk4E; break;
        }
        sdfAppendPacket((SdfListHead *)(packetList), (u32)(sdfCreateFormattedSifCommand(
            0x7300, 0x7D80 + i * 0x60, 0xFF0080, 0, D_00367B70[i], value)));
        style = i == mdlViewerState.unk3C - 6 && i == mdlViewerState.unk3E - 2 ? 6 : 0;
        sdfAppendPacket((SdfListHead *)(packetList), (u32)(sdfCreateFormattedSifCommand(
            0x7F00, 0x7D80 + i * 0x60, 0xFF0080, style, D_003BBCA8, value)));
    }
    sdfAppendPacket((SdfListHead *)(packetList), (u32)(sdfCreateFormattedSifCommand(
        0x7300, 0x7F00, 0xFF0080, 0, "SAVE CONFIG")));
}

u32 mdlRunViewerSettingsTask(void) {
    mdlUpdateViewerSettingsInput();
    func_0021CB80();
    return 0;
}

/* Sum list -1 and active-entry counts, retaining both independent lookups. */
s32 mdlCountActiveRecords(void) {
    MdlCtx *resource = mdlViewerState.resources[0];
    s32 firstListCount = mdlCountRecords((s32)mdlFindViewerRecord(resource, -1));
    s32 activeListCount = mdlCountRecords((s32)mdlFindViewerRecord(resource, mdlViewerState.activeEntryId));

    return firstListCount + activeListCount;
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021CF00);

extern MdlRecord *func_0021CF00(void);

void mdlUpdateViewerMarkEditorInput(void) {
    MdlViewState *state = &mdlViewerState;
    s32 count;
    MdlRecord *record;

    switch (state->editorMode) {
    case 0:
        count = mdlCountActiveRecords();
        if (D_00324510[0x27] < 0) {
            state->unk42++;
            if (state->unk42 >= count) {
                state->unk42 = 0;
            }
        } else if (((u8)D_00324510[0x27] & MDL_PAD_REPEAT_FLAG) != 0) {
            if (state->unk42 < count - 1) {
                state->unk42++;
            }
        } else if (D_00324510[0x26] < 0) {
            if (state->unk42 == 0) {
                state->unk42 = count - 1;
            } else {
                state->unk42--;
            }
        } else if (((u8)D_00324510[0x26] & MDL_PAD_REPEAT_FLAG) != 0) {
            if (state->unk42 > 0) {
                state->unk42--;
            }
        } else if (D_00324510[0x21] < 0) {
            record = func_0021CF00();
            if (record != NULL && mdlRecordMatchesId(record, 3)) {
                state->editorMode = 1;
            }
        }
        break;
    case 1:
        record = func_0021CF00();
        if (D_00324510[0x23] >= 0 && record != NULL && mdlRecordMatchesId(record, 3)) {
            if (D_00324510[0x21] < 0) {
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

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021D198);

u32 mdlRunViewerEffectEditorTask(void) {
    mdlUpdateViewerMarkEditorInput();
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

extern f32 D_00367B80[][4];

extern u32 D_00367BE0[];

extern s32 sdfCreateResetPacketList(void);

extern void *func_002EF2E0(const f32 (*)[4], const u32 *, s32, u32);

/* Submit the intermediate packet with an identity basis in phases 2 and 3. */
void mdlSubmitViewerIntermediateDrawPacket(void) {
    s32 packetList;

    if (mdlViewerState.taskPhase < 4) {
        if (mdlViewerState.taskPhase >= 2) {
            packetList = sdfCreateResetPacketList();
            VU0_SET_UNIT_MATRIX(vf28, vf29, vf30, vf31);
            sdfAppendPacket((SdfListHead *)(packetList), (u32)((s32)func_002EF2E0(D_00367B80, D_00367BE0, 6, 0x80)));
            D_00325048.submit(&D_00325048, packetList);
        }
    }
}

extern f32 D_00367C00[][4];

extern u32 D_00367C40[];

extern s32 D_003BBCB8;

extern void func_002EA2E0(s32 packet, s32 arg);

/* Submit the selected resource in phase 3; keep the native bitwise input/once-flag test. */
void mdlSubmitViewerResourceDrawPacket(void) {
    s32 packetList;
    s32 drawPacket;
    MdlCtx *resource;

    if (mdlViewerState.taskPhase == 3) {
        packetList = sdfCreateResetPacketList();
        resource = mdlViewerState.resources[0];
    VU0_LOAD_MATRIX(resource->inner->matrix);
        drawPacket = (s32)func_002EF2E0(D_00367C00, D_00367C40, 4, 0x80);
        if ((sdfPadButtonStates[13] < 0) & (D_003BBCB8 == 0)) {
            D_003BBCB8 = 1;
            func_002EA2E0(drawPacket, 0x100);
        }
        sdfAppendPacket((SdfListHead *)(packetList), (u32)(drawPacket));
        D_00325048.submit(&D_00325048, packetList);
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
    mdlViewerState.viewerTask = (s32)kwlnTaskCreate(D_00367A40[mdlViewerState.taskPhase - 1].name, 0x2B00, 1, 0, D_00367A40[mdlViewerState.taskPhase - 1].update, 0, D_00367A40[mdlViewerState.taskPhase - 1].data);
    func_00101A80(mdlViewerState.unk00, mdlViewerState.viewerTask);
}

INCLUDE_ASM(const s32, "game/code_00218B48", mdlViewer);

extern f32 D_00367C50[][4];

extern u32 D_00367CB0[];

extern s32 sdfCountMapPositionRecords(void *chunk);

extern u8 *sdfChunkFindByTag(void *chunk, s32 tag);

extern void sdfSetLookAtBasisFromRecord(void *chunk, u8 *record);

/* Walk native 64-byte map-position records and submit their visualization in one packet list. */
void mdlDrawMapPositionRecords(MdlCtx *resource) {
    s32 recordCount = sdfCountMapPositionRecords(resource->inner);
    s32 recordIndex;
    s32 packetList;
    u8 *recordCursor;

    if (recordCount > 0) {
        packetList = sdfCreateResetPacketList();
        recordCursor = sdfChunkFindByTag(resource->inner, MDL_MAP_POSITION_TAG) + MDL_MAP_POSITION_DATA_OFFSET;
        for (recordIndex = 0; recordIndex != recordCount; recordIndex++) {
            sdfSetLookAtBasisFromRecord(resource->inner, recordCursor);
            recordCursor += MDL_MAP_POSITION_RECORD_BYTES;
            sdfAppendPacket((SdfListHead *)(packetList), (u32)((s32)func_002EF2E0(D_00367C50, D_00367CB0, 6, 0x80)));
        }
        D_00325048.submit(&D_00325048, packetList);
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

    if (sdfPathExists(D_00367AF8) == 0) {
        return;
    }
    fileRequest = fileQueueDefaultCallbackRequest(D_00367AF8);
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
            if (func_00301588(lineStart + 9, D_003BBCC0, &color) == 1) {
                D_003BA8EC = color;
            }
        } else if (memcmp(lineStart, "eye-position=", 13) == 0) {
            if (func_00301588(lineStart + 13, "%f,%f,%f", &x, &y, &z) == 3) {
                D_00367A10.x = x;
                D_00367A10.y = y;
                D_00367A10.z = z;
            }
        } else if (memcmp(lineStart, "target-position=", 16) == 0) {
            if (func_00301588(lineStart + 16, "%f,%f,%f", &x, &y, &z) == 3) {
                D_00367A20.x = x;
                D_00367A20.y = y;
                D_00367A20.z = z;
            }
        } else if (memcmp(lineStart, D_003BBCC8, 5) == 0) {
            if (func_00301588(lineStart + 5, D_003BBCD0, &verticalFov) == 1) {
                sdfSceneProjectionParameters.fov = verticalFov;
            }
        } else if (memcmp(lineStart, D_003BBCD8, 4) == 0) {
            if (func_00301588(lineStart + 4, "%d,%f,%d,%f,%x", &fogNear, &fogValue, &fogFar, &fogFarB, &color) == 5) {
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

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021E068);

extern void func_00218E20(void);
extern s32 func_00103218(const char *);
extern MdlEyeVec D_00367CD0;
extern MdlEyeVec D_003246C0;

void func_0021E1C8(void) {
    MdlSystemTask *task;
    s32 i;

    mdlLoadViewerPresentationConfig();
    mdlViewerState.viewerScale = 1.0f;
    mdlViewerState.unk0F = mdlViewerState.unk0B = mdlViewerState.taskPhase = 1;
    mdlViewerState.unk4E = mdlViewerState.unk4C = mdlViewerState.unk4A = mdlViewerState.unk1C = mdlViewerState.resourceGroup = 2;
    mdlViewerState.resourceId = 0;
    mdlViewerState.unk1E = 0;
    mdlViewerState.unk08 = 0;
    mdlViewerState.unk09 = 0;
    mdlViewerState.unk20 = 0;
    mdlViewerState.entryHeight = 0;
    mdlViewerState.entryWidth = 0;
    mdlViewerState.labelIndexB = 0;
    mdlViewerState.scrollPage = 0;
    mdlViewerState.unitStepMode = 0;
    mdlViewerState.unk0E = 0;
    mdlViewerState.unk48 = 0;
    mdlResetViewerBasisVectors();
    sdfSceneProjectionParameters.fov = 0.4363323f;
    PCP_COPY_VECTOR(&D_003246C0, &D_00367CD0);
    for (i = 0; i != 12; i++) {
        mdlViewerState.resources[i] = NULL;
    }
    mdlLoadViewerResourceAndResetCursors();
    mdlViewerState.resourceCount = 1;
    func_00218E20();
    for (i = 0, task = D_00367AE0; i != 3; i++, task++) {
        func_00103218(task->name);
    }
    mdlViewerState.unk00 = (s32)kwlnTaskCreate(D_003ABF78, 0x2AFF, 1, 0, mdlViewer, NULL, 0);
    kwlnTaskCreate(D_003ABF88, 0x2B01, 1, 0, mdlViewerEnd, NULL, 0);
    mdlViewerState.viewerTask = 0;
    mdlRestartViewerPhaseTask();
}


/* Clear each occupied resource slot before destroying its context; retain the native scan form. */
void mdlFreeViewResources(void) {
    s32 resourceIndex;
    for (resourceIndex = 0; resourceIndex != MDL_VIEWER_RESOURCE_SLOTS; resourceIndex++) {
        MdlCtx *resource = mdlViewerState.resources[resourceIndex];
        if (resource != 0) {
            mdlViewerState.resources[resourceIndex] = 0;
            mdlDestroyContext(resource);
        }
    }
}

extern s32 kwlnTaskDestroyWithHierarchyByName(const char *name, s32 flag);

extern void func_00103498(const char *name, s32, s32, s32);

void mdlCleanupViewerTasksAndResources(void) {
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

    fldDrawPackedRgbEditor((void *)(mdlViewerControlState.unk08), first, second,
                  (mdlViewerControlState.unk04 == 0) ? -1 : variant, packedColor | 0x80000000, 1);
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


void mdlFlagClearAll(void) {
    s32 i = 0x7f;
    u32 *word = datGameState->modelFlags.words;

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
    datGameState->modelFlags.words[adjustedFlag >> 5] |= 1 << flag;
}

void mdlFlagClear(s32 flag) {
    s32 adjustedFlag = (flag < 0) ? flag + 0x1f : flag;
    datGameState->modelFlags.words[adjustedFlag >> 5] &= ~(1 << flag);
}

s32 mdlFlagTest(s32 flag) {
    s32 adjustedFlag = (flag < 0) ? flag + 0x1f : flag;
    return (((s32)datGameState->modelFlags.words[adjustedFlag >> 5] >> flag) & 1);
}

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABF78);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABF88);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABF98);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABFA8);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021F630);

extern u32 dds3AdvanceWorldCounter(void);

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

extern s32 *dds3GetObjectBaseResourceHandle(s32 object);

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
    if (dds3GetObjectBaseResourceHandle(object)[7] != 0) {
        mdlAddEntryFlagged(dds3GetObjectBaseResourceHandle(object), 0, 0);
    }
    func_001127A0(object, 0);
    return counter;
}

extern void *dds3FindWorldObjectNodeByKey(s32 world, s32 id, s32 kind);

extern void dds3SetSlotByKind(s32 object, s32 slot);

extern void dds3RegisterObjectInHandlerIndex(s32 object);

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
    mdlAddEntryFlagged(dds3GetObjectBaseResourceHandle(object), 0, 0);
    dds3SetSlotByKind(object, (s32)dds3FindWorldObjectNodeByKey(dds3GetWorldSecondaryObject(), 0x10000, 2));
    dds3RegisterObjectInHandlerIndex(object);
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

