#include "sdk_ps2.h"
#include "sdf_line_pair_packet_api.h"
#include "dds3_path.h"
#include "sdf_request.h"
#include "kwln.h"
#include "kwln_task_create.h"
#include "sdf_packet_list.h"
#include "sdf_packet_builders.h"
#include "sdf_bordered_rectangle_packet.h"
#include "common.h"
#include "dds3_object_flags.h"
#include "sdf_vu0.h"
#include "mdl_option_label.h"
#include "eff_inner_float_api.h"
#include "mdl_light_editor_state.h"
#include "dds3_admin_control_api.h"
#include "kwln_task_add_child.h"
#include "mdl_motion_api.h"
#include "sdf_motion_loop.h"
#include "sdf_texture_draw_packet.h"
#include "bill_object_api.h"
#include "sdf_chip.h"
#include "sdf_resource.h"
#include "sdf.h"
#include "sdf_texture_registry.h"
#include "sdf_projection.h"
#include "mdl.h"
#include "mdl_part_entry.h"
#include "mdl_resource_entries.h"
#include "mdl_track_poly_record.h"
#include "mdl_mark_params.h"
#include "mdl_resource_table.h"
#include "mdl_grouped_parts_api.h"
#include "file_request_api.h"
#include "mdl_flag_api.h"
#include "mdl_world_object_source_vector_api.h"
#include "dds3_world_counter.h"
#include "sdf_vu_matrix.h"

#include "pcp_vu0.h"
#include "ee_mmi.h"
#include "dat_state.h"
#include "sdf_draw.h"
#include "sdf_chunk.h"
#include "sdf_pac_state.h"
#include "eff.h"
#include "eff_node_descriptor.h"
#include "eff_node.h"
#include "sdf_sif_command.h"
#include "eff_transform.h"
#include "evt_world_source_transform.h"
#include "file.h"
#include "kwln_task_lifecycle.h"
#include "mdl_object_stream.h"
#include "dds3obj.h"



extern s32 D_003C88C0[];

extern s32 D_003C88C8[];


extern void fclEncycAllocEntry(s32, s32, s32, s32);




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
#define MDL_MAP_POSITION_TAG 0x534F504D


typedef enum MdlViewerRecordKind {
    MDL_VIEWER_RECORD_KIND_BILLBOARD_PART = 1,
    MDL_VIEWER_RECORD_KIND_EFFECT_PART = 2,
    MDL_VIEWER_RECORD_KIND_TRACK_POLY = 3,
    MDL_VIEWER_RECORD_KIND_STREAM = 4,
    MDL_VIEWER_RECORD_KIND_OBJECT_PART = 5
} MdlViewerRecordKind;

/* Viewer-wide state for the model viewer task (DDS2 game/code_00233660 and
 * DDS1 game/code_00218B48 share this layout field for field). Fields that are
 * only written by a defaults initialiser and never read in either game are
 * left as unkNN on purpose: there is no read to earn a role from. */
typedef struct MdlViewState {
    dds3ProcessID_t *unk00;
    dds3ProcessID_t *viewerTask;
    s8 exitCleanupStarted; /* 0x08: one-shot guard for viewer task/resource teardown. */
    s8 selectionHighlightPhase;
    s8 taskPhase; /* 0x0A: one-based index into D_003C87F0 */
    s8 unk0B;
    s8 unitStepMode;  /* 0x0C: toggled by the step button, read as a 0/2 step */
    s8 unitStepSign;  /* 0x0D: +1/-1, derived from the input keys */
    u8 unk0E;
    s8 assetDisplayMode; /* 0x0F: toggles asset labels and cursor paging. */
    s8 yawStepMode; /* 0x10: one yaw step per left/right button press */
    u8 pad11[3];
    s16 selectedTaskIndex;
    s16 resourceCount;
    s16 resourceGroup; /* 0x18: group passed to the resource loader */
    s16 resourceId;    /* 0x1A: ID passed to the resource loader */
    s16 selectedResourceGroup; /* 0x1C: resource group under the selection cursor. */
    s16 selectedResourceId; /* 0x1E: resource ID under the selection cursor. */
    s16 selectionCursor; /* 0x20: selects one of five controls in the asset panel. */
    s16 activeEntryId;   /* 0x22: record key most recently added to the viewer */
    u16 selectedEntryId; /* 0x24: key passed to mdlAddEntry* */
    s16 selectedNodeId;  /* 0x26: checked by mdlHasMotionEx */
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
    s16 settingsCursor; /* 0x3C: selected row among the ten viewer settings. */
    s16 settingsPage;
    s16 editorMode; /* 0x40: 0 selects records, 1 edits a mark record */
    s16 selectedRecordIndex; /* 0x42: index across the shared and active-entry record lists */
    s16 effectListScroll; /* 0x44: first record displayed by the effect list */
    s16 markFieldCursor; /* 0x46: selected row in the mark parameter editor */
    s16 workspaceSetupPhase; /* 0x48: capture workspace setup advances through phases 0, 1, 2. */
    s16 captureScale; /* 0x4A: model capture scale setting */
    s16 captureDivX;  /* 0x4C: model capture horizontal division setting */
    s16 captureDivY;  /* 0x4E: model capture vertical division setting */
    s16 unk50;
    s16 unk52;
    f32 viewerScale; /* 0x54: viewer zoom, in hundredths */
    u8 pad58[8];
    u128 unk60[3]; /* 0x60: quadword slots; their alignment makes the state quadword aligned */
    dds3ModelHandle_t *resources[MDL_VIEWER_RESOURCE_SLOTS]; /* 0x90 */
    sdfDrawTag_t *packetList; /* 0xC0: drawing packet destination */
} MdlViewState;
typedef char MdlViewState_selectedTaskIndex_offset_check[
    ((u32)&((MdlViewState *)0)->selectedTaskIndex == 0x14) ? 1 : -1];
typedef char MdlViewState_selectedResourceId_offset_check[
    ((u32)&((MdlViewState *)0)->selectedResourceId == 0x1E) ? 1 : -1];
typedef char MdlViewState_selectedResourceGroup_offset_check[
    ((u32)&((MdlViewState *)0)->selectedResourceGroup == 0x1C) ? 1 : -1];
typedef char MdlViewState_size_check[sizeof(MdlViewState) == 0xD0 ? 1 : -1];
typedef char MdlViewState_exitCleanupStarted_offset_check[
    ((u32)&((MdlViewState *)0)->exitCleanupStarted == 0x08) ? 1 : -1];
typedef char MdlViewState_workspaceSetupPhase_offset_check[
    ((u32)&((MdlViewState *)0)->workspaceSetupPhase == 0x48) ? 1 : -1];
typedef char MdlViewState_assetDisplayMode_offset_check[
    ((u32)&((MdlViewState *)0)->assetDisplayMode == 0x0F) ? 1 : -1];
typedef char MdlViewState_selectionCursor_offset_check[
    ((u32)&((MdlViewState *)0)->selectionCursor == 0x20) ? 1 : -1];
typedef char MdlViewState_settingsCursor_offset_check[
    ((u32)&((MdlViewState *)0)->settingsCursor == 0x3C) ? 1 : -1];

extern MdlViewState mdlViewerState;

static inline s8 mdlGetViewerDisplayMode(MdlViewState *state) {
    return state->assetDisplayMode;
}

extern char D_00421208[]; /* "MODEL : %d-%03d " */

extern char D_00421220[]; /* "MODEL : %d-%03x " */

extern char D_00421238[]; /* "%02d/%02d" */

extern char D_00421248[]; /* "%02x/%02x" */

extern char D_00437068[]; /* "" */
extern char D_00421278[];





typedef struct MdlCountNode {
    u8 pad00[4];
    s16 count; /* 0x04 */
    u8 pad06[6];
    sdfTexHandle_t **textures;
} MdlCountNode;






extern dds3Basic_t *dds3AddUnitObjectBasic(s32 world, u128 *pos, u128 *rot);



extern void dds3RebuildObjectModelContext(dds3Basic_t *obj, s32 a, s32 b);
#include "dds3_world_secondary.h"
extern s8 D_00453560[];

void mdlUpdateViewerSelectedModelFromPad(void);

void mdlCleanupViewerTasksAndResources(void);

void mdlDrawViewerSelectionLabel(void);




extern u32 D_00435CBC;

extern void kwlnDebugGraphSetEnabled(s8 mode);

extern s32 fldStepColorChannelByPad(u32 *color, s32 channel, s8 *pad);

extern void dds3DebugIntEdit(void *ptr, s32 type, s64 min, s64 max, s64 step, s64 bigStep);

extern s8 dds3PadButtonState[64];

extern void fldStepIntByPad(void *ptr, s32 type, s64 min, s64 max, s64 small, s64 big, s8 *pad);


void sdfIpuSetOutputTex(sdfIpuDecodeCb_t *, SdfStreamParams *, u8 *, s32, sdfTexHandle_t *);
void sdfIpuFree(sdfIpuDecodeCb_t *);

extern f32 D_00453620[4] __attribute__((aligned(16)));

extern f32 D_00453630[4] __attribute__((aligned(16)));

extern u128 D_00453640;

typedef struct {
    f32 x, y, z, w;
} __attribute__((aligned(16))) MdlEyeVec;

extern MdlEyeVec D_003C87C0;

extern MdlEyeVec D_003C87D0;

extern u128 D_003C87E0;

extern s16 D_00453584[];

typedef struct MdlPadState {
    u8 pad00;
    s8 confirm;
    u8 pad02;
    s8 cancel;
    s8 stepDownA;  /* 0x04 */
    s8 stepUpA;    /* 0x05 */
    s8 stepDownB;  /* 0x06 */
    s8 stepUpB;    /* 0x07 */
} MdlPadState;

typedef struct MdlCtrlState {
    MdlPadState *pad;
    u8 editing;
    u8 blinkTick;
    s16 selection;
    sdfDrawTag_t *packetList;
} MdlCtrlState;

extern MdlCtrlState mdlDebugScaleWork;

void dds3PrimDebugTextBoxRGB(sdfDrawTag_t *packetList, s32 x, s32 y, s32 selectedChannel,
                            u32 packedColor, s32 showNormalizedValues);


extern char *D_003C8768[];

extern u8 D_003C8760[];

extern u16 D_003C8730[];

extern char D_00436FF0[];

extern char D_00436FF8[];

extern char D_004370C0[]; /* "%5.2f" */

#define MDL_PART_OBJECT 3

extern u8 sdfViewTargetVector[];


extern void dds3BillboardDraw(billHandle_t *billboard);




/* Load a viewer package; flag 2 enables the extra request-preparation step. */
void mdlPackDecode(s32 first, s32 second, s32 flags, s32 requestFirst, s32 requestSecond) {
    PacState request;

    sdfPacInit(&request, 0);
    if (flags & 2) {
        sdfPacSetStay(&request);
    }
    sdfPacDecode(&request, (const u8 *)(u32)requestFirst, requestSecond);
    mdlPackSet(request.queueHead, first, second, flags);
    sdfPacFree(&request);
}

/* Release the chip-cell allocation itself, not a pointer-to-pointer wrapper. */
void mdlFreeFileItem(void *memory) {
    sdfChipFreeDirect(memory);
}


extern MdlResourceTable D_003C6588[];

extern MdlResourceTable D_003C86B0[];

extern struct sdfMemHandle_t *D_00436FAC;

extern struct sdfMemHandle_t *D_00436FB0;


/* Release the three copied filename paths, then their tables and backing allocations. */
void mdlReleaseHddFileList(void) {
    MdlResourcePath *slotEntry;
    s32 entryCount;
    s32 entryIndex;

    entryCount = D_003C6588[MDL_VIEWER_TABLE_SLOT].count;
    if (entryCount > 0) {
        slotEntry = D_003C6588[MDL_VIEWER_TABLE_SLOT].entries;
        entryIndex = 0;
        do {
            entryIndex++;
            sdfChipFreeDirect((void *)slotEntry->path);
            sdfChipFreeDirect((void *)slotEntry->resourceListPath);
            sdfChipFreeDirect((void *)slotEntry->motionPath);
            slotEntry++;
        } while (entryIndex < entryCount);
    }
    D_003C86B0[MDL_VIEWER_TABLE_SLOT].entries = NULL;
    D_003C86B0[MDL_VIEWER_TABLE_SLOT].count = 0;
    D_003C6588[MDL_VIEWER_TABLE_SLOT].entries = NULL;
    D_003C6588[MDL_VIEWER_TABLE_SLOT].count = 0;
    sdfMemFreeHandleDirect(&D_00436FAC);
    sdfMemFreeHandleDirect(&D_00436FB0);
}


extern MdlResourceSelection *D_00438F98;
extern MdlResourcePath *D_00438F9C;

/* Copy the model, resource-list and motion filenames into the viewer's path table. */
void mdlMakeHddFileListDummy(void) {
    s32 sourceIndex;
    const char *sourceText;
    char *copiedText;

    mdlReleaseHddFileList();
    D_00436FAC = sdfMemAllocDirect(MDL_VIEWER_HEADER_BYTES);
    D_00438F98 = (MdlResourceSelection *)sdfMemLock(D_00436FAC);
    D_00438F98->pathTable = 5;
    D_00438F98->pathIndex = 0;
    D_00438F98->unk4 = 0x1000;
    D_00438F98->unk6 = 0x3E8;
    D_00436FB0 = sdfMemAllocDirect(MDL_VIEWER_NAME_TABLE_BYTES);
    D_00438F9C = (MdlResourcePath *)sdfMemLock(D_00436FB0);
    for (sourceIndex = 0; sourceIndex != MDL_VIEWER_LABEL_COUNT; sourceIndex++) {
        switch (sourceIndex) {
        case 0:
            sourceText = ((MdlResourcePath *)D_003C6588[0].entries)->path;
            break;
        case 1:
            sourceText = ((MdlResourcePath *)D_003C6588[0].entries)->resourceListPath;
            break;
        default:
            sourceText = ((MdlResourcePath *)D_003C6588[0].entries)->motionPath;
            break;
        }
        if (sourceText != NULL) {
            copiedText = sdfChipAllocDirect(strlen(sourceText) + 1);
            strcpy(copiedText, sourceText);
            switch (sourceIndex) {
            case 0:
                D_00438F9C->path = copiedText;
                break;
            case 1:
                D_00438F9C->resourceListPath = copiedText;
                break;
            case 2:
                D_00438F9C->motionPath = copiedText;
                break;
            }
        }
    }
    D_003C86B0[MDL_VIEWER_TABLE_SLOT].entries = D_00438F98;
    D_003C86B0[MDL_VIEWER_TABLE_SLOT].count = 1;
    D_003C6588[MDL_VIEWER_TABLE_SLOT].entries = D_00438F9C;
    D_003C6588[MDL_VIEWER_TABLE_SLOT].count = 1;
}

/* File-I/O directory record: native stat prefix, filename and private words. */
typedef struct {
    u32 mode;
    u8 stat[0x3C];
    char name[0x100];
    u32 privateWords[2];
} DebugDirectoryEntry;

/* One debug model base name and the .?B variants found for it (MB/PB, TB, AB). */
typedef struct DebugModelFile {
    struct DebugModelFile *next;
    char name[47];
    u8 available[3];
    s8 extension[3][2];
} DebugModelFile;

/* Five-character device prefix copied in front of each work-file path. */
typedef struct {
    char text[5];
} DebugWorkPrefix;

/* newlib's character-class table: one EOF slot, then one entry per byte value. */
extern const char _ctype_[];
#define _L 02
#define islower(c) ((_ctype_ + 1)[(unsigned)(c)] & _L)

extern DebugModelFile *D_00438F94;
extern const char D_00420FD8[];
extern DebugWorkPrefix D_00436FB8[];
extern u8 sdfHddValid;

static inline s32 debugFileUpper(s32 c) {
    return islower(c) ? c - 'a' + 'A' : c;
}


void mdlMakeHddFileList(void) {
    DebugDirectoryEntry directory;
    s32 index;
    s32 count;
    s32 handle;
    s32 length;
    s32 end;
    DebugModelFile *next;
    DebugModelFile *file;
    DebugModelFile **link;

    if (sdfHddValid && (handle = sceDopen(D_00420FD8)) >= 0) {
        mdlReleaseHddFileList();
        while ((length = sceDread(handle, &directory)) > 0) {
            if ((directory.mode & 0xF000) != 0x1000 && length >= 4 && length < 49) {
                s32 kind = -1;

                if (directory.name[length - 3] == '.' &&
                    (s8)debugFileUpper(directory.name[length - 1]) == 'B') {
                    s32 ext = directory.name[length - 2];

                    if (islower(ext)) {
                        ext -= 'a' - 'A';
                    }
                    switch (ext) {
                    case 'A':
                        kind = 2;
                        break;
                    case 'T':
                        kind = 1;
                        break;
                    case 'M':
                    case 'P':
                        kind = 0;
                        break;
                    }
                    if (kind >= 0) {
                        file = D_00438F94;
                        while (file != NULL && memcmp(file->name, directory.name, length - 2) != 0) {
                            file = file->next;
                        }
                        if (file == NULL) {
                            file = sdfChipAllocDirect(sizeof(*file));
                            memset(file, 0, sizeof(*file));
                            memcpy(file->name, directory.name, length - 2);
                            file->name[length - 2] = 0;
                            file->next = D_00438F94;
                            D_00438F94 = file;
                        }
                        file->available[kind] = 1;
                        file->extension[kind][0] = directory.name[length - 2];
                        file->extension[kind][1] = directory.name[length - 1];
                    }
                }
            }
        }
        count = 0;
        sceDclose(handle);
        link = &D_00438F94;
        next = D_00438F94;
        while (next != NULL) {
            file = next;
            next = file->next;
            if (!file->available[0]) {
                *link = next;
                mdlFreeFileItem(file);
            } else {
                if (debugFileUpper(file->extension[0][0]) == 'P') {
                    file->available[1] = 0;
                    file->available[2] = 0;
                }
                link = &file->next;
                count++;
            }
        }
        if (count != 0) {
            index = 0;
            D_00436FAC = sdfMemAllocDirect(count * sizeof(MdlResourceSelection));
            D_00438F98 = sdfMemLock(D_00436FAC);
            D_00436FB0 = sdfMemAllocDirect(count * sizeof(MdlResourcePath));
            D_00438F9C = sdfMemLock(D_00436FB0);
            memset(D_00438F9C, 0, count * sizeof(MdlResourcePath));
            file = D_00438F94;
            while (file != NULL) {
                const char *name = file->name;
                s32 kind;

                D_00438F98[index].pathTable = 5;
                D_00438F98[index].pathIndex = index;
                D_00438F98[index].unk4 = 0x1000;
                D_00438F98[index].unk6 = 1000;
                length = strlen(name);
                end = length + 5;
                for (kind = 0; kind != 3; kind++) {
                    if (file->available[kind]) {
                        char *path = sdfChipAllocDirect(length + 8);

                        *(DebugWorkPrefix *)path = D_00436FB8[0];
                        memcpy(path + 5, name, length);
                        path[end] = file->extension[kind][0];
                        path[end + 1] = file->extension[kind][1];
                        path[end + 2] = 0;
                        switch (kind) {
                        case 0:
                            D_00438F9C[index].path = path;
                            break;
                        case 1:
                            D_00438F9C[index].resourceListPath = path;
                            break;
                        case 2:
                            D_00438F9C[index].motionPath = path;
                            break;
                        }
                    }
                }
                file = file->next;
                index++;
            }
            D_003C86B0[MDL_VIEWER_TABLE_SLOT].entries = D_00438F98;
            D_003C86B0[MDL_VIEWER_TABLE_SLOT].count = count;
            D_003C6588[MDL_VIEWER_TABLE_SLOT].entries = D_00438F9C;
            D_003C6588[MDL_VIEWER_TABLE_SLOT].count = count;
            return;
        }
    }
    mdlMakeHddFileListDummy();
}

s32 mdlSetViewerSlotResourceHandles(s32 table, s32 slot, const char *resourceListPath, const char *path, const char *motionPath) {
    MdlResourcePath *entries = D_003C6588[table].entries;

    if (entries == NULL) {
        return 0;
    }
    if (slot >= D_003C6588[table].count) {
        return 0;
    }
    entries[slot].resourceListPath = resourceListPath;
    entries[slot].path = path;
    entries[slot].motionPath = motionPath;
    return 1;
}


s32 mdlPibGetChunkItemCount(MdlRecord *listHeader);




#define MDL_PART_BILLBOARD 0

#define MDL_PART_EFFECT 1

typedef struct MdlItemCfg {
    u8 pad0;
    u8 enabled; /* 0x01 */
    u8 unk2;
    u8 unk3;
} MdlItemCfg;



/* Read the model record's payload word without advancing its relative link. */
u32 mdlPibGetVersion(MdlRecord *record) {
    return record->payload.word;
}

/* The part-list count is the low half of the packed parameter word. */
u16 mdlPibGetChunkCount(MdlRecord *record) {
    return record->parameter.part.count;
}

/* Follow relative links in the resource's record table to find an ID. */
MdlRecord *mdlPibGetChunk(dds3ModelHandle_t *resource, s32 recordId) {
    MdlRecord *recordTable = resource->sub->partInfo;
    MdlRecord *recordCursor;
    s32 remainingRecords;

    if (recordTable == NULL) {
        return NULL;
    }
    recordCursor = recordTable;
    remainingRecords = mdlPibGetChunkCount(recordCursor);
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
MdlRecord *mdlPibGetFirstChunkItem(MdlRecord *listHeader) {
    MdlRecord *firstRecord;

    firstRecord = (MdlRecord *)((u8 *)listHeader + MDL_RECORD_LIST_HEADER_BYTES);
    if (firstRecord->kind == MDL_RECORD_END_KIND) {
        firstRecord = NULL;
    }
    return firstRecord;
}

/* A terminal kind returns NULL; the link is a byte offset, not a pointer. */
MdlRecord *mdlPibGetNextChunkItem(MdlRecord *currentRecord) {
    MdlRecord *nextRecord;

    nextRecord = (MdlRecord *)((s32)currentRecord + currentRecord->nextOffset);
    if (nextRecord->kind == MDL_RECORD_END_KIND) {
        nextRecord = NULL;
    }
    return nextRecord;
}

/* Count relative-offset records until the 0xffff sentinel. */
s32 mdlPibGetChunkItemCount(MdlRecord *listHeader) {
    s32 recordCount;
    MdlRecord *recordCursor;

    if (listHeader == NULL) {
        return 0;
    }
    recordCursor = mdlPibGetFirstChunkItem(listHeader);
    recordCount = 0;
    while (recordCursor != NULL) {
        recordCount++;
        recordCursor = mdlPibGetNextChunkItem(recordCursor);
    }
    return recordCount;
}

s32 mdlPibCheckItemType(MdlRecord *record, s32 kind) {
    return record->kind == kind;
}

u16 mdlPibGetEffectCount(MdlRecord *record) {
    return record->parameter.part.partIndex;
}

/* Read the counted group record's number of following entity-slot packets. */
u16 mdlPibGetMotionCount(const MdlRecord *record) {
    return record->tail.half.slotCount;
}

extern char D_00420FF0[], D_00421000[], D_00421010[], D_00421020[], D_00421030[], D_00421040[];
extern char D_00436FC0[], D_00436FC8[];

/* Append the kind-dependent record summary at the requested text position and style. */
/* Append a kind-specific record description at the supplied packet coordinates/style. */
INCLUDE_RODATA(const s32, "game/code_00233660", D_00420FF0);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421000);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421010);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421020);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421030);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421040);

void mdlPibDrawEffectInfo(sdfDrawTag_t *packetList, s32 x, s32 y, s32 depth, s32 textStyle, MdlRecord *record) {
    if (record->kind >= MDL_VIEWER_RECORD_KIND_BILLBOARD_PART &&
        record->kind <= MDL_VIEWER_RECORD_KIND_OBJECT_PART) {
        switch (record->kind) {
        case MDL_VIEWER_RECORD_KIND_BILLBOARD_PART:
        case MDL_VIEWER_RECORD_KIND_EFFECT_PART: {
            char *label = record->kind == MDL_VIEWER_RECORD_KIND_BILLBOARD_PART ? D_00436FC0 : D_00436FC8;

            if (record->parameter.part.count == 1) {
                sdfDrawTagLink(packetList, (u32)sdfDevPrintf(x, y, depth, textStyle,
                                                                    D_00420FF0, label, record->payload.word,
                                                                    record->parameter.part.partIndex));
                return;
            }
            sdfDrawTagLink(packetList, (u32)sdfDevPrintf(x, y, depth, textStyle,
                                                                D_00421000, label, record->payload.word,
                                                                record->payload.word + record->parameter.part.count - 1,
                                                                record->parameter.part.partIndex));
            return;
        }
        case MDL_VIEWER_RECORD_KIND_TRACK_POLY:
            sdfDrawTagLink(packetList, (u32)sdfDevPrintf(x, y, depth, textStyle,
                                                                D_00421010, record->payload.word,
                                                                record->parameter.word));
            return;
        case MDL_VIEWER_RECORD_KIND_STREAM:
            sdfDrawTagLink(packetList, (u32)sdfDevPrintf(x, y, depth, textStyle,
                                                                D_00421020, record->payload.stream.selectorA,
                                                                record->payload.stream.selectorB));
            return;
        case MDL_VIEWER_RECORD_KIND_OBJECT_PART:
            sdfDrawTagLink(packetList, (u32)sdfDevPrintf(x, y, depth, textStyle,
                                                                D_00421030, record->payload.word));
            return;
        }
    } else {
        sdfDrawTagLink(packetList, (u32)sdfDevPrintf(x, y, depth, textStyle,
                                                            D_00421040, record->kind));
    }
}

/* Draw the scalar marker fields, then four colors with four byte channels each; negative selection hides the cursor. */
void mdlPibDrawTrackInfo(sdfDrawTag_t *packetList, s32 x, s32 y, s32 depth, EffMarkParams *params, s32 selectedField) {
    s32 colorY = y + 0x300;
    s32 labelX = x + 0xC0;
    u32 packedColor;
    s32 colorIndex;
    s32 channelIndex;

    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(labelX, y, depth, 0, "MARK0:%d", params->mark0));
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(labelX, y + 0x60, depth, 0, "MARK1:%d", params->mark1));
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(labelX, y + 0xC0, depth, 0, "START:%d", params->start));
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(labelX, y + 0x120, depth, 0, "END  :%d", params->end));
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(labelX, y + 0x180, depth, 0, "ITRVL:%d", params->interval));
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(labelX, y + 0x1E0, depth, 0, "FACE :%d", params->face));
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(labelX, y + 0x240, depth, 0, "BLEND:%d", params->blend));
    for (colorIndex = 0; colorIndex != MDL_MARK_COLOR_COUNT; colorIndex++) {
        packedColor = params->colors[colorIndex];
        sdfDrawTagLink(packetList, (u32)dds3PrimFrameRect(x + 0x480, colorY, depth, 0x300, 0x180, (packedColor & 0xFFFFFF) | 0x80000000, 0x60404040));
        for (channelIndex = 0; channelIndex != MDL_COLOR_CHANNEL_COUNT; channelIndex++) {
            sdfDrawTagLink(packetList, (u32)sdfDevPrintf(labelX, colorY, depth, D_003C8760[channelIndex], D_003C8768[channelIndex]));
            sdfDrawTagLink(packetList, (u32)sdfDevPrintf(x + 0x240, colorY, depth, 0, D_00436FF0, packedColor & MDL_COLOR_BYTE_MASK));
            packedColor >>= MDL_COLOR_BYTE_BITS;
            colorY += 0x60;
        }
        colorY += 0x60;
    }
    if (selectedField >= 0) {
        sdfDrawTagLink(packetList, (u32)sdfDevPrintf(x, y + D_003C8730[selectedField], depth, 0, D_00436FF8));
    }
}

/* Edit the marker parameters with the pad: rows 0-6 are the scalar fields, 7-22 the color bytes. */
void mdlPibEditTrackInfo(EffMarkParams *params, s16 *fieldCursor) {
    s32 selectedField = *fieldCursor;
    u8 *colorChannel;

    if (dds3PadButtonState[0x27] < 0) {
        selectedField = selectedField != MDL_MARK_LAST_FIELD ? selectedField + 1 : 0;
    } else if (((u8)dds3PadButtonState[0x27] & MDL_PAD_REPEAT_FLAG) != 0 && selectedField < MDL_MARK_LAST_FIELD) {
        selectedField++;
    } else if (dds3PadButtonState[0x26] < 0) {
        selectedField = selectedField != 0 ? selectedField - 1 : MDL_MARK_LAST_FIELD;
    } else if (((u8)dds3PadButtonState[0x26] & MDL_PAD_REPEAT_FLAG) != 0 && selectedField > 0) {
        selectedField--;
    } else {
        switch (selectedField) {
        case 0:
            fldStepIntByPad(&params->mark0, 4, 0, 99999, 0, 1, dds3PadButtonState);
            break;
        case 1:
            fldStepIntByPad(&params->mark1, 4, 0, 99999, 0, 1, dds3PadButtonState);
            break;
        case 2:
            fldStepIntByPad(&params->start, 2, 0, 9999, 0, 1, dds3PadButtonState);
            break;
        case 3:
            fldStepIntByPad(&params->end, 2, 0, 9999, 0, 1, dds3PadButtonState);
            break;
        case 4:
            fldStepIntByPad(&params->interval, 2, 1, 10, 0, 1, dds3PadButtonState);
            break;
        case 5:
            fldStepIntByPad(&params->face, 1, 1, 10, 0, 1, dds3PadButtonState);
            break;
        case 6:
            fldStepIntByPad(&params->blend, 1, 0, 4, 0, 1, dds3PadButtonState);
            break;
        default:
            colorChannel = (u8 *)params->colors + selectedField - MDL_MARK_COLOR_FIRST_FIELD;
            if (dds3PadButtonState[0x25] < 0) {
                *colorChannel += 1;
            } else if (((u8)dds3PadButtonState[0x25] & MDL_PAD_REPEAT_FLAG) != 0 && *colorChannel < MDL_COLOR_BYTE_MASK) {
                *colorChannel += 1;
            } else if (dds3PadButtonState[0x24] < 0) {
                *colorChannel -= 1;
            } else if (((u8)dds3PadButtonState[0x24] & MDL_PAD_REPEAT_FLAG) != 0 && *colorChannel != 0) {
                *colorChannel -= 1;
            }
            break;
        }
    }
    *fieldCursor = selectedField;
}

/* Append a newly created billboard to the next part-list slot. */
void mdlCreateEffectResrc_P2A(DevRequest *partList, void *descriptor) {
    MdlPartEntry *partEntry = &((MdlPartEntry *)partList->buffer)[partList->usedCount];

    partEntry->state = 0;
    partEntry->kind = MDL_PART_BILLBOARD;
    partEntry->object = dds3BillboardCreate(1, (u32)descriptor);
    partList->usedCount += 1;
}

/* Append a newly created effect to the next part-list slot. */
void mdlCreateEffectResrc_D3P(DevRequest *partList, EffNodeDescriptor *descriptor) {
    MdlPartEntry *partEntry = &((MdlPartEntry *)partList->buffer)[partList->usedCount];

    partEntry->kind = MDL_PART_EFFECT;
    partEntry->state = 0;
    partEntry->object = dds3EffectLoad(descriptor);
    partList->usedCount += 1;
}

void mdlCreateEffectResrc_IPU(DevRequest *list, u8 *sourceAddress, sdfMemHandle_t *backingAllocation, s32 sourceSize) {
    MdlObj *node = sdfAllocAndClearQuadwords(sizeof(*node));
    MdlPartEntry *entry = &((MdlPartEntry *)list->buffer)[list->usedCount];

    node->sourceSize = sourceSize;
    /* Store the recovered pointer through the legacy 32-bit address slot. */
    *(u32 *)&node->sourceAddress = (u32)sourceAddress;
    node->backingAllocation = backingAllocation;
    entry->kind = MDL_PART_OBJECT;
    entry->state = 0;
    entry->object = node;
    list->usedCount += 1;
}

void mdlDeleteEffectResrc_IPU(MdlObj *obj) {
    if (obj->initialized != 0) {
        sdfIpuFree(&obj->soundNode);
    }
    sdfMemFreeDirect(obj->backingAllocation);
    sdfChipFreeDirect(obj);
}

void mdlStartEffectResrc_IPU(MdlObj *obj, sdfTexHandle_t *resource, SdfStreamParams *params) {
    if (obj->initialized == 0) {
        obj->initialized = 1;
        sdfIpuSetOutputTex(&obj->soundNode, params, obj->sourceAddress, obj->sourceSize, resource);
    }
}


DevRequest *mdlEffectResrcListNew(u32 request) {
    return sdfDevCreateBufferedRequest(request, 0x10, 4);
}

/* Destroy each part according to its stored kind, then release the owning request. */
void mdlEffectResrcListFree(DevRequest *partList) {
    s32 partIndex;

    if (partList != NULL) {
        for (partIndex = 0; partIndex < partList->usedCount; partIndex++) {
            MdlPartEntry *partEntry = &((MdlPartEntry *)partList->buffer)[partIndex];

            switch (partEntry->kind) {
            case MDL_PART_BILLBOARD:
                dds3BillboardDelete(mdlPartObjectFromStorage(partEntry->object).billboard);
                break;
            case MDL_PART_EFFECT:
                dds3EffectDelete(mdlPartObjectFromStorage(partEntry->object).effect);
                break;
            case MDL_PART_OBJECT:
                mdlDeleteEffectResrc_IPU(mdlPartObjectFromStorage(partEntry->object).model);
                break;
            }
        }
        sdfItemArrayFree(partList);
    }
}

/* Prepend a cleared, typed resource item to the owner's list. */
dds3ModelEffectCtrlr_t *mdlCreateEffectCtrlr(dds3ModelHandle_t *owner, s32 type, s32 motionSlotIndex) {
    dds3ModelEffectCtrlr_t *item = sdfAllocAndClearQuadwords(sizeof(dds3ModelEffectCtrlr_t));
    dds3ModelEffectCtrlr_t *previousHead = owner->resourceItems;
    item->type = type;
    item->next = previousHead;
    item->motionSlotIndex = motionSlotIndex;
    owner->resourceItems = item;
    return item;
}

void *mdlEffectDup_P2A(MdlPartEntry *entry) {
    struct billHandle_t *clone = dds3BillboardCopy(mdlPartObjectFromStorage(entry->object).billboard);
    entry->state = entry->state + 1;
    return clone;
}

void *mdlEffectDup_D3P(MdlPartEntry *entry) {
    EffNode *clone = dds3EffectCopy(mdlPartObjectFromStorage(entry->object).effect);
    entry->state = entry->state + 1;
    return clone;
}

/* Resolve a native fixed-size slot when its table exists and index is below the upper bound; no lower-bound check. */
MdlPartEntry *mdlGetEffectResrc(dds3ModelHandle_t *resource, s32 slotIndex) {
    DevRequest *slotTable;

    slotTable = resource->sub->partList;
    if (slotTable == NULL) {
        return NULL;
    }
    if (slotIndex >= slotTable->usedCount) {
        return NULL;
    }
    return (MdlPartEntry *)((u8 *)slotTable->buffer + slotIndex * MDL_PART_SLOT_BYTES);
}

/* Bind each consecutive record ID to a newly created part when the chunk contains it. */
void mdlCreateEffect_P2A_D3P(dds3ModelHandle_t *owner, MdlRecord *partRecord, s32 motionSlotIndex, s32 resourceType, void *(*createPart)(MdlPartEntry *)) {
    MdlPartEntry *partSlot = mdlGetEffectResrc(owner, partRecord->parameter.part.partIndex);

    if (partSlot != NULL) {
        sdfModelRootNode_t *model = owner->inner;
        s32 recordId = partRecord->payload.id;
        s32 remainingRecords = partRecord->parameter.part.count;
        f32 optionalAnchorScale = 0.0f;

        /* Retain the native 17-byte size threshold for the optional anchor scale. */
        if ((u32)partRecord->nextOffset >= MDL_PART_VALUE_SIZE_THRESHOLD) {
            optionalAnchorScale = partRecord->tail.anchorScale;
        }
        do {
            SdfMapPositionRecord *chunkRecord = sdfModelSearchMarkPosData(model, recordId++);

            if (chunkRecord != NULL) {
                dds3ModelEffectCtrlr_t *resourceItem = mdlCreateEffectCtrlr(owner, resourceType, motionSlotIndex);

                resourceItem->payload.part.instance = createPart(partSlot);
                resourceItem->payload.part.slot = partSlot;
                resourceItem->payload.part.mapPositionRecord = chunkRecord;
                resourceItem->payload.part.anchorScale = optionalAnchorScale;
            }
        } while (--remainingRecords != 0);
    }
}

/* Convert the effect record to native track parameters and retain its work pointer. */
void mdlCreateEffect_TRACK(dds3ModelHandle_t *owner, MdlTrackPolyRecordView *record, s32 motionSlotIndex) {
    dds3ModelEffectCtrlr_t *resourceItem;
    EffTrackPolyParams effectParams;

    effectParams.model = owner;
    effectParams.idA = record->startPositionId;
    effectParams.idB = record->endPositionId;
    effectParams.startFrame = record->startFrame;
    effectParams.endFrame = record->endFrame;
    effectParams.sampleInterval = record->sampleInterval;
    effectParams.historyLength = record->historyLength;
    effectParams.unk1C = 3;
    effectParams.kind = record->drawKind;
    effectParams.gradientColors[0] = record->gradientColors[0];
    effectParams.gradientColors[1] = record->gradientColors[1];
    effectParams.gradientColors[2] = record->gradientColors[2];
    effectParams.gradientColors[3] = record->gradientColors[3];
    resourceItem = mdlCreateEffectCtrlr(owner, MDL_RESOURCE_TRACK_POLY, motionSlotIndex);
    resourceItem->payload.part.track = effTrackPolyCreateWork(&effectParams);
}

extern void mdlSetBlur(dds3ModelHandle_t *, s32, s32, u32, u32);

void mdlCreateEffect_BLUR(dds3ModelHandle_t *owner, MdlRecord *record) {
    mdlSetBlur(owner, record->payload.stream.selectorA,
                  record->payload.stream.selectorB,
                  record->parameter.word, record->tail.word10);
}


/* Claim an unused object only after its data resource resolves, retaining record flags and deferred-init parameters. */
void mdlCreateEffect_IPU(dds3ModelHandle_t *owner, MdlRecord *entryRecord, s32 motionSlotIndex) {
    MdlPartEntry *partSlot = mdlGetEffectResrc(owner, entryRecord->tail.object.partIndex);

    if (partSlot != 0) {
        MdlObj *object = mdlPartObjectFromStorage(partSlot->object).model;
        if (object->inUse == 0) {
            sdfTexHandle_t *texture = sdfTexSearch(entryRecord->payload.id);
            if (texture != 0) {
                dds3ModelEffectCtrlr_t *resourceItem;
                u8 *attributes;
                object->inUse = 1;
                resourceItem = mdlCreateEffectCtrlr(owner, MDL_RESOURCE_OBJECT, motionSlotIndex);
                /* Fill the object and resource data before attaching the owner. */
                resourceItem->payload.object.object = object;
                attributes = resourceItem->payload.object.attributes;
                resourceItem->payload.object.texture = texture;
                resourceItem->payload.object.owner = owner;
                resourceItem->payload.object.minimumTime = entryRecord->parameter.minimumTime;
                attributes[1] = 1;
                attributes[2] = entryRecord->tail.object.loopMode;
                attributes[3] = entryRecord->tail.object.playbackCadenceStep;
            }
        }
    }
}

/* Initialize once the owner's motion reaches the stored threshold; retain native float-to-signed-to-unsigned conversion. */
void mdlDrawEffect_IPU(dds3ModelEffectCtrlr_t *item) {
    MdlObj *object = item->payload.object.object;
    if (object->initialized == 0) {
        s32 minimumTime = item->payload.object.minimumTime;
        f32 motionTime = item->payload.object.owner->first->currentFrame;
        if ((u32)(s32)motionTime < (u32)minimumTime) {
            return;
        }
        mdlStartEffectResrc_IPU(object, item->payload.object.texture,
                   (SdfStreamParams *)item->payload.object.attributes);
    }
}


/* Dispatch the five record kinds; resource application ignores any callee return value. */
void mdlCreateEffect(dds3ModelHandle_t *owner, MdlRecord *record, s32 motionSlotIndex) {
    switch (record->kind) {
    case MDL_VIEWER_RECORD_KIND_BILLBOARD_PART:
        mdlCreateEffect_P2A_D3P(owner, record, motionSlotIndex, MDL_RESOURCE_BILLBOARD, mdlEffectDup_P2A);
        return;
    case MDL_VIEWER_RECORD_KIND_EFFECT_PART:
        mdlCreateEffect_P2A_D3P(owner, record, motionSlotIndex, MDL_RESOURCE_EFFECT, mdlEffectDup_D3P);
        return;
    case MDL_VIEWER_RECORD_KIND_TRACK_POLY:
        mdlCreateEffect_TRACK(owner, (MdlTrackPolyRecordView *)record, motionSlotIndex);
        return;
    case MDL_VIEWER_RECORD_KIND_STREAM:
        mdlCreateEffect_BLUR(owner, record);
        return;
    case MDL_VIEWER_RECORD_KIND_OBJECT_PART:
        mdlCreateEffect_IPU(owner, record, motionSlotIndex);
        break;
    }
}

/* Find the requested record list and associate each entry with this motion slot. */
void mdlGenEffect(dds3ModelHandle_t *owner, s32 recordId, s32 motionSlotIndex) {
    MdlRecord *recordList = mdlPibGetChunk(owner, recordId);
    if (recordList != NULL) {
        MdlRecord *recordCursor = mdlPibGetFirstChunkItem(recordList);
        while (recordCursor != NULL) {
            mdlCreateEffect(owner, recordCursor, motionSlotIndex);
            recordCursor = mdlPibGetNextChunkItem(recordCursor);
        }
    }
}

/* Release billboard, effect, or tracked-poly handles when applicable, then always free the list item. */
void mdlFreeEffect(dds3ModelEffectCtrlr_t *item) {
    switch (item->type) {
    case MDL_RESOURCE_BILLBOARD:
        dds3BillboardDelete((billHandle_t *)item->payload.part.instance);
        break;
    case MDL_RESOURCE_EFFECT:
        dds3EffectDelete((EffNode *)item->payload.part.instance);
        break;
    case MDL_RESOURCE_TRACK_POLY:
        effTrackPolyRelease(item->payload.part.track);
        break;
    }
    sdfChipFreeDirect((void *)item);
}

/* Release all items for this slot without losing the link across consecutive matches. */
void mdlStopEffect(dds3ModelHandle_t *owner, s32 motionSlotIndex) {
    dds3ModelEffectCtrlr_t **itemLink = &owner->resourceItems;
    dds3ModelEffectCtrlr_t *item = *itemLink;
    while (item != 0) {
        if (item->motionSlotIndex == motionSlotIndex) {
            dds3ModelEffectCtrlr_t *nextItem = item->next;
            mdlFreeEffect(item);
            *itemLink = nextItem;
            item = nextItem;
        } else {
            itemLink = &item->next;
            item = item->next;
        }
    }
}

/* vu0 routine: positionOut = p + normalize(p - sdfViewTargetVector) * scale, p = transformed node position. */
void mdlEffect_P2A_D3P_GetPos(sdfModelRootNode_t *model, dds3ModelEffectCtrlr_t *anchorRecord, f32 *positionOut) {
    SdfMapPositionRecord *mapPosition = anchorRecord->payload.part.mapPositionRecord;
    sdfModelNode_t *drawNode = sdfModelSearchNode(model, mapPosition->nodeId);
    f32 scale = anchorRecord->payload.part.anchorScale;

    VU0_LOAD_MATRIX(drawNode->worldMatrix);
    SDF_VU0_LOADVECTOR(&mapPosition->position);
    SDF_VU0_APPLYMATRIX_XYZ1();
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR_C(sdfViewTargetVector);
    SDF_VU0_SUBVECTOR_A_C();
    SDF_VU0_NORMALIZE();
    VU0_SET_VF2X(scale);
    VU0_MUL_VF2X(vf10, vf10);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_STOREVECTOR_NOMEM(positionOut);
}

/* Update anchored billboard/effect positions, tracked polygons, or deferred object initialization by type. */
void mdlLinkEffect(dds3ModelHandle_t *owner, dds3ModelEffectCtrlr_t *anchorRecord) {
    sdfModelRootNode_t *model = owner->inner;
    f32 position[4];
    void *resourceInstance;

    switch (anchorRecord->type) {
    case MDL_RESOURCE_BILLBOARD:
        mdlEffect_P2A_D3P_GetPos(model, anchorRecord, position);
        resourceInstance = anchorRecord->payload.part.instance;
        effCopyVector(((billHandle_t *)resourceInstance)->position, position);
        dds3BillboardDraw((billHandle_t *)resourceInstance);
        break;
    case MDL_RESOURCE_EFFECT:
        mdlEffect_P2A_D3P_GetPos(model, anchorRecord, position);
        resourceInstance = anchorRecord->payload.part.instance;
        effCopyVectorToNodeInstance((EffNode *)resourceInstance, position);
        dds3EffectDraw((EffNode *)resourceInstance);
        break;
    case MDL_RESOURCE_TRACK_POLY:
        effTrackPolyUpdate(anchorRecord->payload.part.track);
        break;
    case MDL_RESOURCE_OBJECT:
        mdlDrawEffect_IPU(anchorRecord);
        break;
    }
}

/* Set a billboard/effect frame; other resource-item kinds have no frame dispatch. */
void mdlSetEffectColorSub(dds3ModelHandle_t *owner, dds3ModelEffectCtrlr_t *item, s32 frame) {
    switch (item->type) {
    case MDL_RESOURCE_BILLBOARD:
        dds3BillboardSetRGBA((billHandle_t *)item->payload.part.instance, frame);
        return;
    case MDL_RESOURCE_EFFECT:
        effSetNodeParameterValue((EffNode *)item->payload.part.instance, frame);
        break;
    }
}

/* Apply amount to billboard/effect scale; other resource-item kinds are ignored. */
void mdlSetEffectScaleSub(dds3ModelHandle_t *owner, dds3ModelEffectCtrlr_t *item, float amount) {
    switch (item->type) {
    case MDL_RESOURCE_BILLBOARD:
        dds3BillboardSetScale((struct billHandle_t *)item->payload.part.instance, amount, amount);
        return;
    case MDL_RESOURCE_EFFECT:
        dds3EffectSetScale((EffNode *)item->payload.part.instance, amount);
        break;
    }
}

u64 *mdlBuildViewerRectanglePacket(s32 x, s32 y, s32 depth, s32 width, s32 height) {
    return dds3PrimFrameRect(x, y, depth, width, height, 0x30000000, 0x60404040);
}

void mdlAppendViewerRectToDrawList(s32 x, s32 y, s32 depth, s32 width, s32 height, s32 unused) {
    sdfDrawTag_t *packet;

    packet = mdlViewerState.packetList;
    sdfDrawTagLink(packet, (u32)mdlBuildViewerRectanglePacket(x, y, depth, width, height));
}

extern s8 dds3PadBitTable[];

/* Return 1 after a cursor update: fresh presses wrap, while repeated presses stop at the ends. */
s32 mdlUpdateViewerCursor(s16 *cursor, s32 entryCount) {
    s32 lastIndex = entryCount - 1;
    s32 selectedIndex = *cursor;
    s32 changed = 0;
    if (dds3PadBitTable[5] < 0) {
        if (selectedIndex < lastIndex) {
            selectedIndex += 1;
            changed = 1;
        } else {
            selectedIndex = 0;
            changed = 1;
        }
    } else if (((u8)dds3PadBitTable[5] & MDL_PAD_REPEAT_FLAG) != 0) {
        if (selectedIndex < lastIndex) {
            selectedIndex += 1;
            changed = 1;
        }
    } else if (dds3PadBitTable[4] < 0) {
        if (selectedIndex > 0) {
            selectedIndex -= 1;
            changed = 1;
        } else {
            selectedIndex = lastIndex;
            changed = 1;
        }
    } else if ((((u8)dds3PadBitTable[4] & MDL_PAD_REPEAT_FLAG) != 0) && (selectedIndex > 0)) {
        selectedIndex -= 1;
        changed = 1;
    }
    if (changed != 0) {
        *cursor = selectedIndex;
        mdlViewerState.selectionHighlightPhase = 0;
        return 1;
    }
    return 0;
}

/* Apply single-entry or optional page steps using the native wrap/clamp and input-precedence rules. */
s32 mdlUpdateViewerCursorWithPageStep(s16 *cursor, s32 entryCount, s32 pageStep) {
    s32 lastIndex = entryCount - 1;
    s32 selectedIndex = *cursor;
    s32 changed = 0;
    if (dds3PadBitTable[7] < 0) {
        if (selectedIndex < lastIndex) {
            selectedIndex += 1;
        } else {
            selectedIndex = 0;
        }
        changed = 1;
    } else if (((u8)dds3PadBitTable[7] & MDL_PAD_REPEAT_FLAG) != 0) {
        if (selectedIndex < lastIndex) {
            selectedIndex += 1;
            changed = 1;
        }
    } else if (dds3PadBitTable[6] < 0) {
        if (selectedIndex > 0) {
            selectedIndex -= 1;
        } else {
            selectedIndex = lastIndex;
        }
        changed = 1;
    } else if (((u8)dds3PadBitTable[6] & MDL_PAD_REPEAT_FLAG) != 0) {
        if (selectedIndex > 0) {
            selectedIndex -= 1;
            changed = 1;
        }
    } else if (pageStep != 0) {
        if (dds3PadBitTable[11] < 0) {
            if (selectedIndex < lastIndex) {
                selectedIndex += pageStep;
                if (selectedIndex > lastIndex) {
                    selectedIndex = lastIndex;
                }
            } else {
                selectedIndex = 0;
            }
            changed = 1;
        } else if (((u8)dds3PadBitTable[11] & MDL_PAD_REPEAT_FLAG) != 0) {
            if (selectedIndex < lastIndex) {
                selectedIndex += pageStep;
                if (selectedIndex > lastIndex) {
                    selectedIndex = lastIndex;
                }
                changed = 1;
            }
        } else if (dds3PadBitTable[10] < 0) {
            if (selectedIndex > 0) {
                selectedIndex -= pageStep;
                if (selectedIndex < 0) {
                    selectedIndex = 0;
                }
            } else {
                selectedIndex = lastIndex;
            }
            changed = 1;
        } else if ((((u8)dds3PadBitTable[10] & MDL_PAD_REPEAT_FLAG) != 0) && (selectedIndex > 0)) {
            selectedIndex -= pageStep;
            if (selectedIndex < 0) {
                selectedIndex = 0;
            }
            changed = 1;
        }
    }
    if (changed != 0) {
        *cursor = selectedIndex;
        mdlViewerState.selectionHighlightPhase = 0;
        return 1;
    }
    return 0;
}

void mdlLoadViewerResourceAndResetCursors(void) {
    dds3ModelHandle_t *loaded;

    loaded = mdlLoad(mdlViewerState.resourceGroup, mdlViewerState.resourceId);
    mdlViewerState.resources[0] = loaded;
    mdlViewerState.activeEntryId = 0;
    mdlViewerState.selectedEntryId = 0;
    mdlViewerState.selectedNodeId = 0;
    mdlViewerState.unk28 = 0;
    mdlViewerState.unk2A = 0;
    mdlViewerState.unk2C = 0;
    if (loaded->first != NULL) {
        mdlLoopMotion(loaded, 0, 0);
    }
    mdlViewerState.nodeCursor = 0;
}

void mdlResetViewerBasisVectors(void) {
    SDF_VU0_COPYVECTOR(&D_00453620, &D_003C87C0);
    SDF_VU0_COPYVECTOR(&D_00453630, &D_003C87D0);
    SDF_VU0_COPYVECTOR(&D_00453640, &D_003C87E0);
    D_00453584[0] = 0;
}

/* Rotate the viewer resource list in place without moving its allocation. */
void mdlRotateViewResourcesRight(void) {
    s32 resourceIndex = mdlViewerState.resourceCount - 1;
    dds3ModelHandle_t *lastResource = mdlViewerState.resources[resourceIndex];

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
    dds3ModelHandle_t *firstResource = mdlViewerState.resources[0];

    for (resourceIndex = 0; resourceIndex < resourceCount - 1; resourceIndex++) {
        mdlViewerState.resources[resourceIndex] = mdlViewerState.resources[resourceIndex + 1];
    }
    mdlViewerState.resources[resourceIndex] = firstResource;
}

extern const char *D_003C8780[];
extern char D_00437060[];

/* Draw the MODE header and current or saved viewer mode label. */
void mdlDrawViewerModeLabel(void) {
    SifCommand packet;
    s32 phase;

    mdlAppendViewerRectToDrawList(0x7150, 0x7948, 0xFF0080, 0xBA0, 0x90, 0);
    sdfPktInit(&packet, 0x7180, 0x7960, 0xFF0080, 0);
    sdfDrawTagLink(mdlViewerState.packetList, (u32)(sdfDevPrintfEx(&packet, D_00437060)));
    phase = mdlViewerState.taskPhase;
    if (phase == 0) {
        phase = mdlViewerState.unk0B - 1;
    } else {
        phase = phase - 1;
    }
    sdfDrawTagLink(mdlViewerState.packetList, (u32)(sdfDevPrintfEx(&packet, D_003C8780[phase])));
}


extern const char *D_003C8780[];

void mdlDrawViewerOptionLabels(void) {
    const char **label;
    u32 i;

    if (mdlViewerState.taskPhase != 0) {
        return;
    }

    mdlAppendViewerRectToDrawList(0x7690, 0x7A08, 0xFF0080, 0xC60, 0x2D0, 0);
    label = D_003C8780;
    i = 0;
    do {
        s32 style = (i == mdlViewerState.selectedTaskIndex && mdlViewerState.selectionHighlightPhase < 0x14) ? 4 : 0;
        s32 xOffset;
        u32 row;
        u32 packet;

        if (i >= 7) {
            xOffset = 0x600;
            row = i - 7;
        } else {
            xOffset = 0;
            row = i;
        }
        packet = (u32)sdfDevPrintf(0x76C0 + xOffset,
                                                   0x7A20 + row * 0x60,
                                                   0xFF0080, style, *label);
        i++;
        sdfDrawTagLink(mdlViewerState.packetList, packet);
        label++;
    } while (i != 13);
}

void mdlDrawViewerModelAndMotionSummary(void) {
    SifCommand packet;
    const char *format;
    s32 displayMode;
    s32 nodeCount;
    void *formatted;

    mdlAppendViewerRectToDrawList(0x7E10, 0x8608, 0xFF0080, 0xEA0, 0x90, 0);
    sdfPktInit(&packet, 0x7E40, 0x8620, 0xFF0080, 0);

    if ((displayMode = mdlGetViewerDisplayMode(&mdlViewerState)) == 0) {
        format = D_00421208;
    } else {
        format = D_00421220;
    }
    sdfDrawTagLink(mdlViewerState.packetList,
                    (u32)sdfDevPrintfEx(&packet, format, mdlViewerState.resourceGroup, mdlViewerState.resourceId));

    nodeCount = mdlGetNumOfCut(mdlViewerState.resources[0], 0);
    if (nodeCount == 0) {
        formatted = sdfDevPrintfEx(&packet, D_00437068);
    } else {
        if (mdlViewerState.assetDisplayMode == 0) {
            format = D_00421238;
        } else {
            format = D_00421248;
        }
        formatted = sdfDevPrintfEx(&packet, format,
                                       mdlGetMotionNum(mdlViewerState.resources[0], 0), nodeCount - 1);
    }
    sdfDrawTagLink(mdlViewerState.packetList, (u32)formatted);
}

void mdlAddViewEntryFlagged(void) {
    MdlViewState *state = &mdlViewerState;
    f32 width;
    f32 height;

    if (mdlHasMotionEx(state->resources[0], state->selectedNodeId) == 0) {
        return;
    }
    height = (f32)state->entryHeight;
    width = (f32)state->entryWidth;
    if (height < width) {
        width = height;
    }
    state->activeEntryId = state->selectedEntryId;
    mdlSmoothLoopMotion(state->resources[0], state->selectedNodeId, (s16)state->selectedEntryId, width, height);
}

void mdlAddPlainViewerEntryForSelectedNode(void) {
    MdlViewState *state = &mdlViewerState;
    f32 width;
    f32 height;

    if (mdlHasMotionEx(state->resources[0], state->selectedNodeId) == 0) {
        return;
    }
    height = (f32)state->entryHeight;
    width = (f32)state->entryWidth;
    if (height < width) {
        width = height;
    }
    state->activeEntryId = state->selectedEntryId;
    mdlSmoothSingleMotion(state->resources[0], state->selectedNodeId, (s16)state->selectedEntryId, width, height);
}

extern u32 mdlGetNumOfMajor(void);
extern u32 mdlGetNumOfMinor(s32 tableIndex);
extern void mdlMakeHddFileList(void);
extern void mdlFreeViewResources(void);
extern u8 sdfHddValid;

/* Step the viewer's selected row, then apply that row's pending resource or
 * entry selection when the confirm buttons are pressed. */
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

void mdlHandleViewerAssetSelection(void) {
    s32 pageStep;
    s32 motionIndex;
    s32 entryCount;
    s32 referenceCount;

    if (mdlUpdateViewerCursor(&mdlViewerState.selectionCursor, 5) != 0) {
        return;
    }
    pageStep = mdlViewerState.assetDisplayMode == 0 ? 10 : 16;
    if (dds3PadBitTable[0] < 0) {
        mdlViewerState.selectedNodeId++;
        if (mdlViewerState.selectedNodeId == 4) {
            mdlViewerState.selectedNodeId = 0;
        }
        motionIndex = mdlGetMotionNum(mdlViewerState.resources[0], mdlViewerState.selectedNodeId);
        if (motionIndex < 0) {
            motionIndex = 0;
        }
        mdlViewerState.selectedEntryId = motionIndex;
    }
    switch (mdlViewerState.selectionCursor) {
    case 0:
        if (mdlUpdateViewerCursorWithPageStep(&mdlViewerState.selectedResourceGroup, mdlGetNumOfMajor(), 1) != 0) {
            entryCount = mdlGetNumOfMinor(mdlViewerState.selectedResourceGroup);
            if (mdlViewerState.selectedResourceId < entryCount) {
                return;
            }
            mdlViewerState.selectedResourceId = entryCount - 1;
            return;
        }
        break;
    case 1:
        if (mdlUpdateViewerCursorWithPageStep(&mdlViewerState.selectedResourceId, mdlGetNumOfMinor(mdlViewerState.selectedResourceGroup), pageStep) != 0) {
            return;
        }
        break;
    case 2:
        referenceCount = mdlGetNumOfCut(mdlViewerState.resources[0], mdlViewerState.selectedNodeId);
        if (referenceCount == 0) {
            return;
        }
        if (mdlUpdateViewerCursorWithPageStep(&mdlViewerState.selectedEntryId, referenceCount, pageStep) != 0) {
            return;
        }
        break;
    case 3:
        if (mdlUpdateViewerCursorWithPageStep(&mdlViewerState.entryHeight, 100, 10) != 0) {
            return;
        }
        break;
    case 4:
        if (mdlUpdateViewerCursorWithPageStep(&mdlViewerState.entryWidth, 100, 10) != 0) {
            return;
        }
        break;
    }
    if (dds3PadBitTable[12] < 0) {
        if (sdfHddValid == 0) {
            return;
        }
        mdlFreeViewResources();
        mdlMakeHddFileList();
        if (mdlViewerState.resourceGroup == mdlGetNumOfMajor() - 1) {
            mdlViewerState.resourceId = 0;
            mdlViewerState.selectedResourceGroup = mdlViewerState.resourceGroup;
            mdlViewerState.selectedResourceId = 0;
        }
        mdlLoadViewerResourceAndResetCursors();
        return;
    }
    if (mdlViewerState.selectionCursor < 2) {
        if (dds3PadBitTable[1] >= 0) {
            return;
        }
        if (mdlViewerState.resourceGroup == mdlViewerState.selectedResourceGroup && mdlViewerState.resourceId == mdlViewerState.selectedResourceId) {
            return;
        }
        mdlFree(mdlViewerState.resources[0]);
        mdlViewerState.resources[0] = NULL;
        mdlViewerState.resourceGroup = mdlViewerState.selectedResourceGroup;
        mdlViewerState.resourceId = mdlViewerState.selectedResourceId;
        mdlLoadViewerResourceAndResetCursors();
        return;
    } else if (dds3PadBitTable[1] < 0) {
        mdlAddViewEntryFlagged();
    } else if (dds3PadBitTable[3] < 0) {
        mdlAddPlainViewerEntryForSelectedNode();
    }
}

extern char D_00437070[];
extern char D_00437078[];
extern char D_00437080[];
extern char D_00437088[];
extern char D_00437090[];
extern char D_00437098[];
extern char D_004370A0[];
extern char D_004370A8[];

extern void mdlGetFilebase(s32 selectionListIndex, s32 selectionIndex, char *destination, s32 capacity);

/* Draw the pending resource details and the current selection in each model node. */
void mdlDrawViewerAssetSelection(void) {
    SifCommand packet;
    char basename[32];
    s32 panelTop;
    s32 panelHeight;
    s32 basenameWidth;
    s32 rowY;
    s32 index;
    void *formatted;
    const char *format;

    if (mdlViewerState.selectedNodeId == 0) {
        panelTop = 0x8560;
        panelHeight = 0x90;
    } else {
        panelTop = 0x8440;
        panelHeight = 0x1B0;
    }
    mdlAppendViewerRectToDrawList(0x8410, panelTop - 0x18, 0xFF0080, 0xAE0,
                                 panelHeight, 0);

    mdlGetFilebase(mdlViewerState.selectedResourceGroup, mdlViewerState.selectedResourceId, basename, 0x20);
    basenameWidth = (s32)strlen(basename) * 3 << 6;
    mdlAppendViewerRectToDrawList(0x8E90 - basenameWidth,
                                 panelTop - 0xD8, 0xFF0080,
                                 basenameWidth + 0x60, 0x90, 0);
    formatted = sdfDevPrintf(0x8EC0 - basenameWidth,
                                             panelTop - 0xC0, 0xFF0080, 0,
                                             basename);
    sdfDrawTagLink(mdlViewerState.packetList, (u32)formatted);

    sdfPktInit(&packet, 0x8440, panelTop + mdlViewerState.selectedNodeId * 0x60,
               0xFF0080, mdlViewerState.selectionHighlightPhase < 0x14 && mdlViewerState.selectionCursor == 0 ? 4 : 0);
    formatted = sdfDevPrintfEx(&packet, D_00437070, mdlViewerState.selectedResourceGroup + 0x30);
    sdfDrawTagLink(mdlViewerState.packetList, (u32)formatted);

    sdfPktSetCmd(&packet, 0);
    formatted = sdfDevPrintfEx(&packet, D_00437078);
    sdfDrawTagLink(mdlViewerState.packetList, (u32)formatted);

    sdfPktSetCmd(&packet,
                 mdlViewerState.selectionHighlightPhase < 0x14 && mdlViewerState.selectionCursor == 1 ? 4 : 0);
    if (mdlViewerState.assetDisplayMode == 0) {
        format = D_00437080;
    } else {
        format = D_00437088;
    }
    formatted = sdfDevPrintfEx(&packet, format, mdlViewerState.selectedResourceId);
    sdfDrawTagLink(mdlViewerState.packetList, (u32)formatted);

    sdfPktSetCmd(&packet,
                 mdlViewerState.selectionHighlightPhase < 0x14 && mdlViewerState.selectionCursor == 3 ? 4 : 0);
    formatted = sdfDevPrintfEx(&packet, D_00421278, mdlViewerState.entryHeight);
    sdfDrawTagLink(mdlViewerState.packetList, (u32)formatted);

    sdfPktSetCmd(&packet,
                 mdlViewerState.selectionHighlightPhase < 0x14 && mdlViewerState.selectionCursor == 4 ? 4 : 0);
    formatted = sdfDevPrintfEx(&packet, D_00437090, mdlViewerState.entryWidth);
    sdfDrawTagLink(mdlViewerState.packetList, (u32)formatted);

    rowY = panelTop;
    index = 0;
    while (mdlViewerState.selectedNodeId == 0 ? index <= 0 : index < 4) {
        s32 referenceCount = mdlGetNumOfCut(mdlViewerState.resources[0], index);

        if (mdlViewerState.selectedNodeId == index) {
            s32 style = mdlViewerState.selectionHighlightPhase < 0x14 && mdlViewerState.selectionCursor == 2 ? 4 : 0;

            if (referenceCount == 0) {
                formatted = sdfDevPrintf(
                    0x88C0, rowY, 0xFF0080, style, D_00437098);
            } else {
                if (mdlViewerState.assetDisplayMode == 0) {
                    format = D_004370A0;
                } else {
                    format = D_004370A8;
                }
                formatted = sdfDevPrintf(
                    0x88C0, rowY, 0xFF0080, style, format,
                    (s16)mdlViewerState.selectedEntryId);
            }
        } else {
            referenceCount = mdlGetNumOfCut(mdlViewerState.resources[0], index);
            if (referenceCount == 0) {
                formatted = sdfDevPrintf(
                    0x88C0, rowY, 0xFF0080, 0, D_00437068);
            } else {
                s32 motionIndex;

                referenceCount--;
                motionIndex = mdlGetMotionNum(mdlViewerState.resources[0], index);
                if (mdlViewerState.assetDisplayMode == 0) {
                    format = D_00421238;
                } else {
                    format = D_00421248;
                }
                formatted = sdfDevPrintf(
                    0x88C0, rowY, 0xFF0080, 0, format,
                    motionIndex, referenceCount);
            }
        }
        rowY += 0x60;
        index++;
        sdfDrawTagLink(mdlViewerState.packetList, (u32)formatted);
    }
}

u32 mdlRunViewerAssetSelectionTask(void) {
    mdlHandleViewerAssetSelection();
    mdlDrawViewerAssetSelection();
    return 0;
}

extern void FromRotation_B(f32 angle);
extern f32 sdfViewMatrix[];

/* vu0 routine: move the viewer camera or orbit it about the look-at point. */
void mdlUpdateViewerCameraFromPad(void) {
    f32 speed;
    s32 rotated;

    if (dds3PadBitTable[0xC] < 0) {
        mdlResetViewerBasisVectors();
        return;
    }
    if (dds3PadBitTable[0] < 0) {
        if (++mdlViewerState.labelIndexA == 2) {
            mdlViewerState.labelIndexA = 0;
        }
    }
    if (mdlViewerState.labelIndexA == 0) {
        speed = 1.25f;
        if (dds3PadBitTable[1] == 0) {
            speed = 20.0f;
            if (dds3PadBitTable[3] == 0) {
                speed = 5.0f;
            }
        }
        SDF_VU0_LOADIDENTITYVECTOR();
        EE_MMI_LOAD_MATRIX_COLUMN(vf11, sdfViewMatrix);
        VU0_SCALAR_OP(speed, "vmulx.xyzw vf11, vf11, vf2x");
        if (dds3PadBitTable[5] != 0) {
            SDF_VU0_ADDVECTOR();
        } else if (dds3PadBitTable[4] != 0) {
            SDF_VU0_SUBVECTOR();
        }
        EE_MMI_LOAD_MATRIX_COLUMN(vf11, sdfViewMatrix + 2);
        VU0_SCALAR_OP(speed, "vmulx.xyzw vf11, vf11, vf2x");
        if (dds3PadBitTable[6] != 0) {
            SDF_VU0_ADDVECTOR();
        } else if (dds3PadBitTable[7] != 0) {
            SDF_VU0_SUBVECTOR();
        }
        EE_MMI_LOAD_MATRIX_COLUMN(vf11, sdfViewMatrix + 1);
        VU0_SCALAR_OP(speed, "vmulx.xyzw vf11, vf11, vf2x");
        if (dds3PadBitTable[9] != 0) {
            SDF_VU0_ADDVECTOR();
        } else if (dds3PadBitTable[8] != 0) {
            SDF_VU0_SUBVECTOR();
        }
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_LOADVECTOR(&D_00453620);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(&D_00453620);
        SDF_VU0_LOADVECTOR(&D_00453630);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(&D_00453630);
    } else {
        speed = 0.25f * 3.14159265f / 180.0f;
        if (dds3PadBitTable[1] == 0) {
            speed = 4.0f * 3.14159265f / 180.0f;
            if (dds3PadBitTable[3] == 0) {
                speed = 1.0f * 3.14159265f / 180.0f;
            }
        }
        rotated = 0;
        SDF_VU0_LOADIDENTITYVECTOR();
        if (dds3PadBitTable[5] != 0) {
            EE_MMI_LOAD_MATRIX_COLUMN(vf11, sdfViewMatrix + 1);
            FromRotation_B(speed);
            effMiscQuatMultiplyVU();
            rotated = 1;
        } else if (dds3PadBitTable[4] != 0) {
            EE_MMI_LOAD_MATRIX_COLUMN(vf11, sdfViewMatrix + 1);
            FromRotation_B(-speed);
            effMiscQuatMultiplyVU();
            rotated = 1;
        }
        if (dds3PadBitTable[6] != 0) {
            EE_MMI_LOAD_MATRIX_COLUMN(vf11, sdfViewMatrix);
            FromRotation_B(speed);
            effMiscQuatMultiplyVU();
            rotated = 1;
        } else if (dds3PadBitTable[7] != 0) {
            EE_MMI_LOAD_MATRIX_COLUMN(vf11, sdfViewMatrix);
            FromRotation_B(-speed);
            effMiscQuatMultiplyVU();
            rotated = 1;
        }
        if (dds3PadBitTable[8] != 0) {
            EE_MMI_LOAD_MATRIX_COLUMN(vf11, sdfViewMatrix + 2);
            FromRotation_B(speed);
            effMiscQuatMultiplyVU();
            rotated = 1;
        } else if (dds3PadBitTable[9] != 0) {
            EE_MMI_LOAD_MATRIX_COLUMN(vf11, sdfViewMatrix + 2);
            FromRotation_B(-speed);
            effMiscQuatMultiplyVU();
            rotated = 1;
        }
        if (rotated) {
            effMiscQuaternionToMatrixVU();
            SDF_VU0_LOADVECTOR(&D_00453630);
            SDF_VU0_LOADVECTOR_B(&D_00453620);
            SDF_VU0_SUBVECTOR();
            SDF_VU0_APPLYMATRIX_XYZ1();
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(&D_00453630);
            SDF_VU0_LOADVECTOR(&D_00453640);
            SDF_VU0_APPLYMATRIX_XYZ1();
            SDF_VU0_STOREVECTOR(&D_00453640);
        }
    }
}

void mdlDrawViewerIndexedLabelOverlay(void) {
    sdfDrawTag_t *packets;

    mdlAppendViewerRectToDrawList(0x8A10L, 0x7948, 0xFF007F, 0x4E0, 0x90, 0);
    packets = mdlViewerState.packetList;
    sdfDrawTagLink(packets, (u32)sdfDevPrintf(0x8A40L, 0x7960, 0xFF0080, 0, D_003C88C0[mdlViewerState.labelIndexA]));
}

u32 mdlRunViewerIndexedLabelTask(void) {
    mdlUpdateViewerCameraFromPad();
    mdlDrawViewerIndexedLabelOverlay();
    return 0;
}

extern f32 D_0040B530[4];
extern f32 D_0040B540[4];
extern f32 D_0040B550[4];

/* vu0 routine: edit the viewer transform with translation or quaternion steps. */
void mdlUpdateViewerSelectedModelFromPad(void) {
    f32 speed;
    f32 angle;

    if (dds3PadBitTable[0x11] < 0) {
        mdlViewerState.yawStepMode ^= 1;
    }
    if (dds3PadBitTable[0] < 0) {
        mdlViewerState.labelIndexB ^= 1;
        return;
    }
    if (dds3PadBitTable[0xC] < 0) {
        SDF_VU0_LOADIDENTITYVECTOR();
        mdlSetPosition(mdlViewerState.resources[0]);
        mdlSetRotate(mdlViewerState.resources[0]);
        return;
    }
    if (mdlViewerState.yawStepMode != 0) {
        mdlGetRotate(mdlViewerState.resources[0]);
        if (dds3PadBitTable[5] < 0) {
            SDF_VU0_LOADVECTOR_B(D_0040B540);
            angle = 5.0f * 3.14159265f / 180.0f;
        } else if (dds3PadBitTable[4] < 0) {
            SDF_VU0_LOADVECTOR_B(D_0040B540);
            angle = -5.0f * 3.14159265f / 180.0f;
        } else {
            return;
        }
        FromRotation_B(angle);
        effMiscQuatMultiplyVU();
        mdlSetRotate(mdlViewerState.resources[0]);
    } else if (mdlViewerState.labelIndexB == 0) {
        speed = 1.25f;
        if (dds3PadBitTable[1] == 0) {
            speed = 20.0f;
            if (dds3PadBitTable[3] == 0) {
                speed = 5.0f;
            }
        }
        mdlGetPosition(mdlViewerState.resources[0]);
        EE_MMI_LOAD_MATRIX_COLUMN(vf11, sdfViewMatrix);
        VU0_SCALAR_OP(speed, "vmulx.xyzw vf11, vf11, vf2x");
        if (dds3PadBitTable[5] != 0) {
            SDF_VU0_ADDVECTOR();
        } else if (dds3PadBitTable[4] != 0) {
            SDF_VU0_SUBVECTOR();
        }
        EE_MMI_LOAD_MATRIX_COLUMN(vf11, sdfViewMatrix + 2);
        VU0_SCALAR_OP(speed, "vmulx.xyzw vf11, vf11, vf2x");
        if (dds3PadBitTable[6] != 0) {
            SDF_VU0_ADDVECTOR();
        } else if (dds3PadBitTable[7] != 0) {
            SDF_VU0_SUBVECTOR();
        }
        EE_MMI_LOAD_MATRIX_COLUMN(vf11, sdfViewMatrix + 1);
        VU0_SCALAR_OP(speed, "vmulx.xyzw vf11, vf11, vf2x");
        if (dds3PadBitTable[9] != 0) {
            SDF_VU0_ADDVECTOR();
        } else if (dds3PadBitTable[8] != 0) {
            SDF_VU0_SUBVECTOR();
        }
        mdlSetPosition(mdlViewerState.resources[0]);
    } else {
        speed = 0.5f * 3.14159265f / 180.0f;
        if (dds3PadBitTable[1] == 0) {
            speed = 8.0f * 3.14159265f / 180.0f;
            if (dds3PadBitTable[3] == 0) {
                speed = 2.0f * 3.14159265f / 180.0f;
            }
        }
        mdlGetRotate(mdlViewerState.resources[0]);
        if (dds3PadBitTable[5] != 0) {
            SDF_VU0_LOADVECTOR_B(D_0040B540);
            FromRotation_B(speed);
            effMiscQuatMultiplyVU();
        } else if (dds3PadBitTable[4] != 0) {
            SDF_VU0_LOADVECTOR_B(D_0040B540);
            FromRotation_B(-speed);
            effMiscQuatMultiplyVU();
        }
        if (dds3PadBitTable[6] != 0) {
            SDF_VU0_LOADVECTOR_B(D_0040B530);
            FromRotation_B(speed);
            effMiscQuatMultiplyVU();
        } else if (dds3PadBitTable[7] != 0) {
            SDF_VU0_LOADVECTOR_B(D_0040B530);
            FromRotation_B(-speed);
            effMiscQuatMultiplyVU();
        }
        if (dds3PadBitTable[9] != 0) {
            SDF_VU0_LOADVECTOR_B(D_0040B550);
            FromRotation_B(speed);
            effMiscQuatMultiplyVU();
        } else if (dds3PadBitTable[8] != 0) {
            SDF_VU0_LOADVECTOR_B(D_0040B550);
            FromRotation_B(-speed);
            effMiscQuatMultiplyVU();
        }
        mdlSetRotate(mdlViewerState.resources[0]);
    }
}

void mdlDrawViewerSelectionLabel(void) {
    sdfDrawTag_t *packets;

    mdlAppendViewerRectToDrawList(0x8A10L, 0x7948, 0xFF007F, 0x4E0, 0x90, 0);
    packets = mdlViewerState.packetList;
    sdfDrawTagLink(packets, (u32)sdfDevPrintf(0x8A40L, 0x7960, 0xFF0080, 0, D_003C88C8[mdlViewerState.labelIndexB]));
}

s32 mdlRunViewerSelectionLabelTask(void) {
    mdlUpdateViewerSelectedModelFromPad();
    if (D_00453560[0] == 0) {
        mdlDrawViewerSelectionLabel();
    }
    return 0;
}

/* Change the viewer scale in hundredths, with larger steps at larger values. */
void mdlAdjustViewerScale(void) {
    s32 scaleHundredths = (s32)(mdlViewerState.viewerScale * 100.0f + 0.5f);

    /* Keep the distinct up/down thresholds and the native float expression order. */
    if (dds3PadButtonState[0x27] & MDL_PAD_REPEAT_FLAG) {
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
    } else if (dds3PadButtonState[0x26] & MDL_PAD_REPEAT_FLAG) {
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
    if (dds3PadButtonState[0x23] < 0) {
        mdlViewerState.unitStepMode ^= 1;
    }
    mdlViewerState.unitStepSign = 0;
    if (mdlViewerState.unitStepMode != 0) {
        if (dds3PadButtonState[0x25] != 0) {
            mdlViewerState.unitStepSign = 1;
        } else if (dds3PadButtonState[0x24] != 0) {
            mdlViewerState.unitStepSign = -1;
        }
    }
}

/* Draw the motion progress bar: timeline frame, playhead marker and "[time/length]" label, then the zoom value. */
INCLUDE_RODATA(const s32, "game/code_00233660", D_00421278);

void mdlDrawViewerMotionTimeline(void) {
    sdfDrawTag_t *packetList;
    dds3ModelHandle_t *resource;
    sdfMotionCtrlr_t *motion;
    s32 playheadX;
    s32 textStyle;

    mdlAppendViewerRectToDrawList(0x81D0, 0x7948, 0xFF007F, 0xD20, 0xF0, 0);
    packetList = mdlViewerState.packetList;
    sdfPrimLinkLine(packetList, 0x80303030, 0, 0x8200, 0x7990, 0x8EC0, 0x7990, 0xFF0080, 0);
    sdfPrimLinkLine(packetList, 0x80303030, 0, 0x8200, 0x7960, 0x8200, 0x79C0, 0xFF0080, 0);
    sdfPrimLinkLine(packetList, 0x80303030, 0, 0x8EC0, 0x7960, 0x8EC0, 0x79C0, 0xFF0080, 0);
    resource = mdlViewerState.resources[0];
    motion = resource->first;
    if (motion != NULL) {
        playheadX = (s32)(motion->currentFrame * 3264.0f / motion->frameCount);
        if (playheadX < 0) {
            playheadX = 0;
        }
        playheadX += 0x8200;
        sdfPrimLinkLine(packetList, 0x800000E0, 0, playheadX, 0x7960, playheadX, 0x79C0, 0xFF0090, 0);
        sdfDrawTagLink(mdlViewerState.packetList, (u32)sdfDevPrintf(0x8200, 0x79C0, 0xFF0080, 0, "[%5.1f/%-3d]", motion->currentFrame, motion->frameCount));
    } else {
        sdfDrawTagLink(mdlViewerState.packetList, (u32)sdfDevPrintf(0x8200, 0x79C0, 0xFF0080, 0, "[---.-/---]"));
    }
    textStyle = mdlViewerState.unitStepMode != 0 ? 2 : 0;
    sdfDrawTagLink(mdlViewerState.packetList, (u32)sdfDevPrintf(0x8B00, 0x79C0, 0xFF0080, textStyle, D_004370C0, mdlViewerState.viewerScale));
}

u32 mdlUpdateViewerScaleTask(void) {
    mdlAdjustViewerScale();
    mdlDrawViewerMotionTimeline();
    return 0;
}

extern s8 D_0040B7D9[];
extern u8 D_00438FA0;
extern s8 D_00438A50;
extern f32 D_00438A54;
extern f32 D_00438A58;
extern f32 D_00438A5C;
extern f32 D_00438A60;

/* Final workspace publication: see docs/idioms.md, "Publishing viewer workspace". */
void mdlUpdateViewerWorkspaceSetup(void) {
    s32 initialPhase = 1;

    switch (mdlViewerState.workspaceSetupPhase) {
    case 0:
        if (D_0040B7D9[0] < 0) {
            f32 scale = mdlViewerState.captureScale;

            mdlViewerState.yawStepMode = initialPhase;
            mdlViewerState.unk50 = 0;
            mdlViewerState.unk52 = 0;
            mdlViewerState.taskPhase = -1;
            D_00438FA0 = mdlViewerState.unitStepMode;
            mdlViewerState.unitStepMode = initialPhase;
            D_00438A50 = initialPhase;
            D_00438A58 = D_00438A54 = scale;
            mdlViewerState.workspaceSetupPhase = initialPhase;
        }
        break;
    case 1:
        D_00438A5C = mdlViewerState.captureDivX * 0x100 -
                   mdlViewerState.unk50 * 0x200 - 0x100;
        D_00438A60 = mdlViewerState.captureDivY * 0x70 -
                   mdlViewerState.unk52 * 0xE0 - 0x70;
        mdlViewerState.workspaceSetupPhase = 2;
        break;
    case 2:
        /* 0x00236D88..0x00236D98: completed phase retained in retail dispatch. */
        break;
    }
}

/* Retail no-op invoked after every workspace setup phase update. */
void mdlAfterViewerWorkspaceUpdate(void) {
}

/* Run workspace setup and its separate hook; keep the task result zero. */
u32 mdlUpdateViewerWorkspaceSetupTask(void) {
    mdlUpdateViewerWorkspaceSetup();
    mdlAfterViewerWorkspaceUpdate();
    return 0;
}

void mdlApplyViewerResourceMenuAction(void) {
    s32 i;
    dds3ModelHandle_t *resource;
    s32 entryId;
    sdfMotionCtrlr_t *motion;

    if (mdlUpdateViewerCursorWithPageStep(&mdlViewerState.scrollPage, 5, 1) != 0) {
        return;
    }
    if (dds3PadBitTable[1] >= 0) {
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
            mdlFree(mdlViewerState.resources[0]);
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
                entryId = resource->current.h.motionIndex;
                if (motion->loopEnabled == SDF_MOTION_LOOP_DISABLED) {
                    sdfMotionChange(motion, entryId, SDF_MOTION_LOOP_DISABLED);
                } else {
                    sdfMotionChange(motion, entryId, SDF_MOTION_LOOP_ENABLED);
                }
            }
        }
        break;
    }
    mdlViewerState.selectedResourceGroup = mdlViewerState.resourceGroup = mdlGetMajorNum(mdlViewerState.resources[0]);
    mdlViewerState.selectedResourceId = mdlViewerState.resourceId = mdlGetMinorNum(mdlViewerState.resources[0]);
    i = mdlViewerState.resources[0]->current.h.motionIndex;
    if (i < 0) {
        i = 0;
    }
    mdlViewerState.selectedEntryId = mdlViewerState.activeEntryId = i;
}

extern const char *D_003C88D0[];

/* Draw resource actions with independent eligibility and selection styles. */
void mdlDrawViewerResourceMenu(void) {
    SifCommand packet;
    s32 row;
    s32 enabled;
    s32 style;

    mdlAppendViewerRectToDrawList(0x8590, 0x7948, 0xFF007F, 0x960, 0x210, 0);
    for (row = 0; row != 5; row++) {
        enabled = 0;
        switch (row) {
        case 0:
            if (mdlViewerState.resourceCount < MDL_VIEWER_RESOURCE_SLOTS) {
                enabled = 1;
            }
            break;
        case 1:
        case 2:
        case 3:
        case 4:
            if (mdlViewerState.resourceCount >= 2) {
                enabled = 1;
            }
            break;
        }
        if (mdlViewerState.selectionHighlightPhase < 20 && mdlViewerState.scrollPage == row) {
            style = enabled ? 4 : 12;
        } else {
            style = enabled ? 0 : 8;
        }
        sdfPktInit(&packet, 0x85C0, 0x7960 + row * 0x60, 0xFF0080, style);
        sdfDrawTagLink(mdlViewerState.packetList,
            (u32)sdfDevPrintfEx(&packet, D_003C88D0[row]));
    }
}


u32 mdlRunViewerResourceMenuTask(void) {
    mdlApplyViewerResourceMenuAction();
    mdlDrawViewerResourceMenu();
    return 0;
}

void mdlHandleViewerNodeCursorInput(void) {
    MdlCountNode *node;

    node = mdlViewerState.resources[0]->inner->assetData;
    if (node != NULL) {
        if (node->count > 0) {
            mdlUpdateViewerCursor(&mdlViewerState.nodeCursor, node->count);
        }
    }
}

extern char D_00421318[];
extern char D_004370C8[];
void mdlDrawViewerTexturePreview(void) {
    s32 index = 0;
    s32 count = 0;
    s32 width, height;
    MdlCountNode *node;
    sdfDrawTag_t *packetList;
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
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x7180, 0x7A20,
        0xFF0080, 0, D_00421318, selectedNumber, count));
    if (count > 0) {
        width = node->textures[index]->width;
        height = node->textures[index]->height;
        sdfDrawTagLink(packetList, (u32)sdfDevPrintf(0x7780, 0x7A20,
            0xFF0080, 0, D_004370C8, width, height));
        sdf2DLinkTexPacket(packetList, node->textures[index], SDF_GS_CONTEXT_1);
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
        sdfPrimLinkSprite(packetList, 0x80808080, 0, 0x7180, 0x7AE0,
            0, 0, displayWidth + 0x7180, displayHeight + 0x7AE0,
            width << 4, height << 4, 0xFF0080, 0);
    }
}

u32 mdlUpdateViewerNodeCursorTask(void) {
    mdlHandleViewerNodeCursorInput();
    mdlDrawViewerTexturePreview();
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_00233660", D_004212C8);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004212D8);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004212E8);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004212F8);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421308);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421318);

s32 mdlIsDebugTimeGraph(void) {
    return dds3SearchProcessName("DebugTimeGrph") != 0;
}

void mdlSaveViewerPresentationConfig(void);

/* Debug menu: page 0 selects an action, pages 1-4 edit the color channels and value steps. */
void mdlUpdateViewerSettingsInput(void) {
    switch (mdlViewerState.settingsPage) {
    case 0:
        if (mdlUpdateViewerCursorWithPageStep(&mdlViewerState.settingsCursor, 10, 1) != 0) {
            return;
        }
        if (dds3PadBitTable[1] >= 0) {
            return;
        }
        switch (mdlViewerState.settingsCursor) {
        case 0:
            mdlViewerState.unk0E ^= 1;
            break;
        case 1:
            kwlnDebugGraphSetEnabled(mdlIsDebugTimeGraph() == 0);
            break;
        case 2:
            mdlViewerState.assetDisplayMode ^= 1;
            break;
        case 3:
        case 4:
        case 5:
            mdlViewerState.settingsPage = 1;
            break;
        case 6:
        case 7:
        case 8:
            mdlViewerState.settingsPage = mdlViewerState.settingsCursor - 4;
            break;
        case 9:
            SDF_VU0_COPYVECTOR(&D_003C87C0, &D_00453620);
            SDF_VU0_COPYVECTOR(&D_003C87D0, &D_00453630);
            mdlSaveViewerPresentationConfig();
            break;
        }
        break;
    case 1:
        fldStepColorChannelByPad(&D_00435CBC, mdlViewerState.settingsCursor - 3, dds3PadBitTable);
        if (dds3PadBitTable[1] < 0 || dds3PadBitTable[3] < 0) {
            mdlViewerState.settingsPage = 0;
        }
        break;
    case 2:
        if (dds3PadBitTable[1] < 0 || dds3PadBitTable[3] < 0) {
            mdlViewerState.settingsPage = 0;
        } else {
            dds3DebugIntEdit(&mdlViewerState.captureScale, 2, 1, 8, 1, 1);
        }
        break;
    case 3:
        if (dds3PadBitTable[1] < 0 || dds3PadBitTable[3] < 0) {
            mdlViewerState.settingsPage = 0;
        } else {
            dds3DebugIntEdit(&mdlViewerState.captureDivX, 2, 1, 8, 1, 1);
        }
        break;
    case 4:
        if (dds3PadBitTable[1] < 0 || dds3PadBitTable[3] < 0) {
            mdlViewerState.settingsPage = 0;
        } else {
            dds3DebugIntEdit(&mdlViewerState.captureDivY, 2, 1, 8, 1, 1);
        }
        break;
    }
}

extern const s32 D_003C88E8[];
extern const char *D_003C8910[];
extern const char *D_003C8920[];
extern char D_004370D0[];
extern char D_004370D8[];
extern char D_004370E0[];
extern char D_004370E8[];

/* Draw viewer toggles, time-graph status and the background-color/config controls. */
INCLUDE_RODATA(const s32, "game/code_00233660", D_00421388);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004213A0);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004213B8);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004213D0);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004213E0);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004213F0);

void mdlDrawViewerSettings(void) {
    /* Retail00237754..74 fills and then indexes these three status bytes
     * at sp+0..2; this is a real temporary stack table, not a source view. */
    s8 enabled[3];
    sdfDrawTag_t *packetList;
    s32 i;
    s32 y;
    s32 style;
    s16 value;

    mdlAppendViewerRectToDrawList(0x7150, 0x7A08, 0xFF007F, 0x1320, 0x5D0, 0);
    enabled[0] = mdlViewerState.unk0E;
    packetList = mdlViewerState.packetList;
    enabled[1] = mdlIsDebugTimeGraph();
    enabled[2] = mdlViewerState.assetDisplayMode;
    for (i = 0, y = 0x7A80; i < 3; i++, y += 0x60) {
        sdfDrawTagLink(packetList, (u32)sdfDevPrintf(
            0x7300, y, 0xFF0080, 0, D_003C8910[i],
            enabled[i] == 0 ? D_004370D0 : D_004370D8));
    }
    if (mdlViewerState.selectionHighlightPhase < 20) {
        sdfDrawTagLink(packetList, (u32)sdfDevPrintf(
            0x7240, D_003C88E8[mdlViewerState.settingsCursor], 0xFF0080, 0, D_004370E0));
    }
    dds3PrimDebugTextBoxRGB(packetList, 0x7300, D_003C88E8[3],
                           mdlViewerState.settingsPage == 1 ? mdlViewerState.settingsCursor - 3 : -1,
                           D_00435CBC, 0);
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(
        0x7B40, 0x7C60, 0xFF0080, 0, "BACK COLOR"));
    for (i = 0; i < 3; i++) {
        /* Retail002378B8 clears the selected channel value before the switch. */
        value = 0;
        switch (i) {
        case 0: value = mdlViewerState.captureScale; break;
        case 1: value = mdlViewerState.captureDivX; break;
        case 2: value = mdlViewerState.captureDivY; break;
        }
        sdfDrawTagLink(packetList, (u32)sdfDevPrintf(
            0x7300, 0x7D80 + i * 0x60, 0xFF0080, 0, D_003C8920[i], value));
        style = i == mdlViewerState.settingsCursor - 6 && i == mdlViewerState.settingsPage - 2 ? 6 : 0;
        sdfDrawTagLink(packetList, (u32)sdfDevPrintf(
            0x7F00, 0x7D80 + i * 0x60, 0xFF0080, style, D_004370E8, value));
    }
    sdfDrawTagLink(packetList, (u32)sdfDevPrintf(
        0x7300, 0x7F00, 0xFF0080, 0, "SAVE CONFIG"));
}

u32 mdlRunViewerSettingsTask(void) {
    mdlUpdateViewerSettingsInput();
    mdlDrawViewerSettings();
    return 0;
}

/* Sum list -1 and active-entry counts, retaining both independent lookups. */
s32 mdlCountActiveRecords(void) {
    dds3ModelHandle_t *resource = mdlViewerState.resources[0];
    s32 firstListCount = mdlPibGetChunkItemCount(mdlPibGetChunk(resource, -1));
    s32 activeListCount = mdlPibGetChunkItemCount(mdlPibGetChunk(resource, mdlViewerState.activeEntryId));

    return firstListCount + activeListCount;
}

/* Record at the cursor index, counting list -1 first and then the active entry's list. */
MdlRecord *mdlGetSelectedRecord(void) {
    dds3ModelHandle_t *resource = mdlViewerState.resources[0];
    MdlRecord *firstList = mdlPibGetChunk(resource, -1);
    MdlRecord *secondList = mdlPibGetChunk(resource, mdlViewerState.activeEntryId);
    s32 index = mdlViewerState.selectedRecordIndex;
    MdlRecord *record;

    if (firstList != NULL) {
        record = mdlPibGetFirstChunkItem(firstList);
        while (record != NULL) {
            if (index == 0) {
                return record;
            }
            index--;
            record = mdlPibGetNextChunkItem(record);
        }
    }
    if (secondList == NULL) {
        return NULL;
    }
    record = mdlPibGetFirstChunkItem(secondList);
    while (record != NULL) {
        if (index == 0) {
            return record;
        }
        index--;
        record = mdlPibGetNextChunkItem(record);
    }
    return NULL;
}

extern MdlRecord *mdlGetSelectedRecord(void);

void mdlUpdateViewerMarkEditorInput(void) {
    MdlViewState *state = &mdlViewerState;
    s32 count;
    MdlRecord *record;

    switch (state->editorMode) {
    case 0:
        count = mdlCountActiveRecords();
        if (dds3PadButtonState[0x27] < 0) {
            state->selectedRecordIndex++;
            if (state->selectedRecordIndex >= count) {
                state->selectedRecordIndex = 0;
            }
        } else if (((u8)dds3PadButtonState[0x27] & MDL_PAD_REPEAT_FLAG) != 0) {
            if (state->selectedRecordIndex < count - 1) {
                state->selectedRecordIndex++;
            }
        } else if (dds3PadButtonState[0x26] < 0) {
            if (state->selectedRecordIndex == 0) {
                state->selectedRecordIndex = count - 1;
            } else {
                state->selectedRecordIndex--;
            }
        } else if (((u8)dds3PadButtonState[0x26] & MDL_PAD_REPEAT_FLAG) != 0) {
            if (state->selectedRecordIndex > 0) {
                state->selectedRecordIndex--;
            }
        } else if (dds3PadButtonState[0x21] < 0) {
            record = mdlGetSelectedRecord();
            if (record != NULL && mdlPibCheckItemType(record, MDL_VIEWER_RECORD_KIND_TRACK_POLY)) {
                state->editorMode = 1;
            }
        }
        break;
    case 1:
        record = mdlGetSelectedRecord();
        if (dds3PadButtonState[0x23] >= 0 && record != NULL && mdlPibCheckItemType(record, MDL_VIEWER_RECORD_KIND_TRACK_POLY)) {
            if (dds3PadButtonState[0x21] < 0) {
                mdlAddPlainViewerEntryForSelectedNode();
            } else {
                mdlPibEditTrackInfo((EffMarkParams *)record, &state->markFieldCursor);
            }
        } else {
            state->editorMode = 0;
        }
        break;
    }
}

extern const char D_00421420[];
extern const char D_004370F0[];
void mdlDrawViewerEffectRecordList(void) {
    MdlViewState *state = &mdlViewerState;
    dds3ModelHandle_t *resource;
    MdlRecord *firstList;
    MdlRecord *secondList;
    MdlRecord *record;
    s32 firstCount;
    s32 secondCount;
    s32 totalCount;
    s32 firstRemaining;
    s32 selected;
    s32 scroll;
    s32 skipCount;
    s32 rowCount;
    s32 markerY;
    s32 descriptionY;
    sdfDrawTag_t *packetList;

    mdlAppendViewerRectToDrawList(0x7150, 0x7A08, 0xFF007F, 0xF60, 0x270, 0);
    resource = state->resources[0];
    packetList = state->packetList;
    firstList = mdlPibGetChunk(resource, -1);
    firstCount = mdlPibGetChunkItemCount(firstList);
    secondList = mdlPibGetChunk(resource, state->activeEntryId);
    secondCount = mdlPibGetChunkItemCount(secondList);
    totalCount = firstCount + secondCount;

    if (totalCount == 0) {
        sdfDrawTagLink(packetList,
                        (u32)sdfDevPrintf(0x7300, 0x7A20,
                                                         0xFF0080, 0,
                                                         D_00421420));
    } else {
        s32 maxScroll = totalCount - 6;

        if (maxScroll < 0) {
            maxScroll = 0;
        }
        scroll = state->effectListScroll;
        if (scroll > maxScroll) {
            scroll = maxScroll;
        }

        firstRemaining = firstCount;
        if (firstCount != 0) {
            record = mdlPibGetFirstChunkItem(firstList);
        } else {
            record = mdlPibGetFirstChunkItem(secondList);
        }

        selected = mdlViewerState.selectedRecordIndex;
        if (selected >= totalCount) {
            selected = totalCount - 1;
            mdlViewerState.selectedRecordIndex = selected;
        }
        if (selected < scroll) {
            scroll = selected;
        } else if (selected >= scroll + 6) {
            scroll = selected - 5;
        }
        mdlViewerState.effectListScroll = scroll;

        skipCount = scroll;
        rowCount = 0;
        if (record != NULL) {
            descriptionY = 0x7A20;
            do {
                s32 selectionMarker = ' ';

                if (skipCount > 0) {
                    skipCount--;
                } else {
                    s32 listMarker = ' ';
                    s32 selectedStyle = 0;

                    if (selected == 0) {
                        if (mdlViewerState.editorMode == 0) {
                            selectionMarker = '>';
                        } else {
                            selectedStyle = 4;
                        }
                    }

                    if (firstRemaining > 0) {
                        listMarker = '*';
                    }

                    markerY = 0x7A20 + rowCount * 0x60;
                    if (selectionMarker != ' ' || listMarker != ' ') {
                        sdfDrawTagLink(packetList,
                                        (u32)sdfDevPrintf(
                                            0x7180, markerY, 0xFF0080, selectedStyle,
                                            D_004370F0, selectionMarker, listMarker));
                    }

                    mdlPibDrawEffectInfo(packetList, 0x7300, descriptionY, 0xFF0080,
                                  selectedStyle, record);
                    descriptionY += 0x60;
                    rowCount++;
                }

                if (firstRemaining > 0 && --firstRemaining == 0) {
                    record = secondList != NULL ? mdlPibGetFirstChunkItem(secondList) : NULL;
                } else {
                    record = mdlPibGetNextChunkItem(record);
                }
                selected--;
                if (rowCount >= 6) {
                    break;
                }
            } while (record != NULL);
        }

        if (scroll != 0) {
            sdfPrimLinkF3(packetList, 0x8000A0C0, 0,
                                 0x8040, 0x7A30, 0x8000, 0x7A70,
                                 0x8080, 0x7A70, 0xFF0080, NULL);
        }
        if (record != NULL) {
            sdfPrimLinkF3(packetList, 0x8000A0C0, 0,
                                 0x8000, 0x7C10, 0x8080, 0x7C10,
                                 0x8040, 0x7C50, 0xFF0080, NULL);
        }
    }

    if (mdlViewerState.editorMode == 1) {
        MdlRecord *markRecord = mdlGetSelectedRecord();

        if (markRecord != NULL && mdlPibCheckItemType(markRecord, MDL_VIEWER_RECORD_KIND_TRACK_POLY)) {
            mdlAppendViewerRectToDrawList(0x8590, 0x7A08, 0xFF007F, 0x960, 0xA50, 0);
            mdlPibDrawTrackInfo(packetList, 0x85C0, 0x7A20, 0xFF0080,
                                   (EffMarkParams *)markRecord,
                                   mdlViewerState.markFieldCursor);
        }
    }
}

u32 mdlRunViewerEffectEditorTask(void) {
    mdlUpdateViewerMarkEditorInput();
    mdlDrawViewerEffectRecordList();
    return 0;
}

s32 mdlRequestViewerExitOnce(void) {
    if (mdlViewerState.exitCleanupStarted == 0) {
        mdlViewerState.exitCleanupStarted = 1;
        mdlCleanupViewerTasksAndResources();
        dds3AdminiExitSequence();
    }
    return 0;
}

extern SdfPoolNode D_00380048; /* sdfGrOtTable[40], a native interior symbol. */

extern f32 D_003C8A00[][4];

extern u32 D_003C8A60[];



extern f32 D_003C8930[][4];

extern u32 D_003C8990[];

/* Submit the intermediate packet with an identity basis in phases 2 and 3. */
void mdlSubmitViewerIntermediateDrawPacket(void) {
    sdfDrawTag_t *packetList;

    if (mdlViewerState.taskPhase < 4) {
        if (mdlViewerState.taskPhase >= 2) {
            packetList = dds3FontReqAllocDrawTag();
            SDF_VU0_LOADIDENTITY();
            sdfDrawTagLink(packetList, (u32)sdfDrawRotatedModel(D_003C8930, D_003C8990, 6, 0x80));
            D_00380048.append(&D_00380048, packetList);
        }
    }
}

extern s32 D_004370F8;

extern f32 D_003C89B0[][4];

extern u32 D_003C89F0[];

extern s32 sdfDumpHexLine(s32, s32);

/* Submit the selected resource in phase 3; keep the native bitwise input/once-flag test. */
void mdlSubmitViewerResourceDrawPacket(void) {
    sdfDrawTag_t *packetList;
    s32 drawPacket;
    dds3ModelHandle_t *resource;

    if (mdlViewerState.taskPhase == 3) {
        packetList = dds3FontReqAllocDrawTag();
        resource = mdlViewerState.resources[0];
        VU0_LOAD_MATRIX(resource->inner->matrix);
        drawPacket = (s32)sdfDrawRotatedModel(D_003C89B0, D_003C89F0, 4, 0x80);
        if ((dds3PadBitTable[13] < 0) & (D_004370F8 == 0)) {
            D_004370F8 = 1;
            sdfDumpHexLine(drawPacket, 0x100);
        }
        sdfDrawTagLink(packetList, (u32)drawPacket);
        D_00380048.append(&D_00380048, packetList);
    }
}

void mdlViewerTaskDestroy(void) {
    if (mdlViewerState.viewerTask != NULL) {
        dds3KillProcess(mdlViewerState.viewerTask, 0);
        mdlViewerState.viewerTask = NULL;
    }
}

typedef struct MdlViewerTaskDef {
    const char *name;
    TaskUpdate update;
    s32 data;
} MdlViewerTaskDef;

extern MdlViewerTaskDef D_003C87F0[];



void mdlRestartViewerPhaseTask(void) {
    mdlViewerTaskDestroy();
    mdlViewerState.viewerTask =
        dds3StartProcess(D_003C87F0[mdlViewerState.taskPhase - 1].name, 0x2B00, 1, 0,
                              D_003C87F0[mdlViewerState.taskPhase - 1].update, 0,
                              D_003C87F0[mdlViewerState.taskPhase - 1].data);
    dds3LinkProcess(mdlViewerState.unk00, mdlViewerState.viewerTask);
}

#include "sdf_gs_header.h"
extern s8 D_0040B7E6[];
extern s8 D_0037F532[];
extern u8 aViewTarget[];
extern u8 aViewDown[];
extern u32 sdfGetLodChunkValue(sdfModelRootNode_t *);
extern s32 sdfGrAllocPacketBuff(s32 size);

s32 mdlViewer(dds3ProcessID_t *task) {
    sdfModelRootNode_t *model;
    SdfGsPacketHeader *packet;
    s32 lodCount;
    s32 selection;

    mdlViewerState.selectionHighlightPhase += 2;
    if (mdlViewerState.selectionHighlightPhase == 30) mdlViewerState.selectionHighlightPhase = 0;
    if (D_0040B7E6[0] < 0) {
        model = mdlViewerState.resources[0]->inner;
        lodCount = sdfGetLodChunkValue(model);
        if (lodCount > 0) {
            if (model->lodIndex == lodCount) model->lodIndex = 0;
            else model->lodIndex++;
        }
    }
    if (mdlViewerState.taskPhase == 0) {
        selection = mdlViewerState.selectedTaskIndex;
        if ((dds3PadButtonState[0x27] & 2) && (u32)selection < 12) {
            mdlViewerState.selectedTaskIndex = selection + 1;
        } else if (dds3PadButtonState[0x27] < 0) {
            if (selection == 12) mdlViewerState.selectedTaskIndex = 0;
            else { mdlViewerState.selectedTaskIndex = selection + 1; }
        } else if ((dds3PadButtonState[0x26] & 2) && selection > 0) {
            mdlViewerState.selectedTaskIndex = selection - 1;
        } else if (dds3PadButtonState[0x26] < 0) {
            if (selection == 0) mdlViewerState.selectedTaskIndex = 12;
            else { mdlViewerState.selectedTaskIndex = selection - 1; }
        } else if (dds3PadButtonState[0x25] < 0 || dds3PadButtonState[0x24] < 0) {
            if ((u32)selection < 7) {
                selection += 7;
                if ((u32)selection < 13) mdlViewerState.selectedTaskIndex = selection;
            } else {
                u16 pageStep = -7;

                mdlViewerState.selectedTaskIndex = selection + pageStep;
            }
        } else if (dds3PadButtonState[0x21] < 0) {
            mdlViewerState.selectionHighlightPhase = 0;
            mdlViewerState.taskPhase = (u8)mdlViewerState.selectedTaskIndex + 1;
            mdlRestartViewerPhaseTask();
        } else if (dds3PadButtonState[0x22] < 0) {
            mdlViewerState.taskPhase = mdlViewerState.unk0B;
            mdlRestartViewerPhaseTask();
        }
    } else if (mdlViewerState.taskPhase >= 0 && D_0037F532[0] < 0) {
        mdlViewerTaskDestroy();
        mdlViewerState.unk0B = mdlViewerState.taskPhase;
        mdlViewerState.taskPhase = 0;
        mdlViewerState.selectedTaskIndex = mdlViewerState.unk0B - 1;
    }
    SDF_VU0_COPYVECTOR(sdfViewTargetVector, &D_00453620);
    SDF_VU0_COPYVECTOR(aViewTarget, &D_00453630);
    SDF_VU0_COPYVECTOR(aViewDown, &D_00453640);
    mdlViewerState.packetList = dds3FontReqAllocDrawTag();
    packet = sdfGrAllocPacketBuff(0x40);
    sdfGrSetRegsPacketHeader(packet, 2);
    ((u64*)packet)[4] = 0x44;
    ((u64*)packet)[5] = 0x42;
    ((u64*)packet)[6] = 0x3000D;
    ((u64*)packet)[7] = 0x47;
    sdfDrawTagLink(mdlViewerState.packetList, (u32)packet);
    if (mdlViewerState.yawStepMode == 0) {
        mdlDrawViewerModeLabel();
        mdlDrawViewerModelAndMotionSummary();
        mdlDrawViewerOptionLabels();
        mdlSubmitViewerIntermediateDrawPacket();
        mdlSubmitViewerResourceDrawPacket();
    }
    return 0;
}

/* Walk native 64-byte map-position records and submit their visualization in one packet list. */
void mdlDrawMapPositionRecords(dds3ModelHandle_t *resource) {
    s32 recordCount = sdfModelGetMarkPosCount(resource->inner);

    if (recordCount > 0) {
        s32 recordIndex = 0;
        sdfDrawTag_t *packetList = dds3FontReqAllocDrawTag();
        SdfMapPositionRecord *recordCursor = (SdfMapPositionRecord *)(
            (SdfMapPositionChunkPrefix *)sdfChunkFindByTag(resource->inner, MDL_MAP_POSITION_TAG) + 1);

        do {
            SdfMapPositionRecord *currentRecord = recordCursor;

            recordIndex++;
            recordCursor++;
            sdfModelMakeMarkPosMatrix(resource->inner, currentRecord);
            sdfDrawTagLink(packetList, (u32)sdfDrawRotatedModel(D_003C8A00, D_003C8A60, 6, 0x80));
        } while (recordIndex != recordCount);
        D_00380048.append(&D_00380048, packetList);
    }
}

extern SdfPoolNode D_003805A8;
extern SdfPoolNode *kwlnSceneSurfaceSets[13][4];
s32 mdlViewerEnd(dds3ProcessID_t *task)
{
  f32 step = mdlViewerState.viewerScale;
  s32 i;
  if (mdlViewerState.unitStepMode != 0)
  {
    i = 0;
    do
    {
      sdfMotionCtrlr_t *motion;
      if (mdlViewerState.resources[i] != 0)
      {
        motion = mdlViewerState.resources[i]->first;
        if (motion != 0)
        {
          memset(&motion->frameStep, 0, sizeof(motion->frameStep));
          switch (mdlViewerState.unitStepSign)
          {
            case 1:
              motion->frameStep = step;
              break;
            case -1:
            {
              f32 frame = motion->currentFrame;
              frame -= step;
              if (frame < 0.0f)
              {
                do
                {
                  frame += motion->frameCount;
                }
                while (frame < motion->frameStep);
              }
              sdfMotionSeek(motion, frame);
              break;
            }
          }
        }
        mdlLink(mdlViewerState.resources[i], kwlnSceneSurfaceSets[0]);
        if (((s8) mdlViewerState.unk0E) != 0)
        {
          mdlDrawMapPositionRecords(mdlViewerState.resources[i]);
        }
      }
      i++;
    }
    while (i != MDL_VIEWER_RESOURCE_SLOTS);
  }
  else
  {
    i = 0;
    do
    {
      if (mdlViewerState.resources[i] != 0)
      {
        if (mdlViewerState.resources[i]->first != 0)
        {
          mdlViewerState.resources[i]->first->frameStep = step;
        }
        mdlLink(mdlViewerState.resources[i], kwlnSceneSurfaceSets[0]);
        if (((s8) mdlViewerState.unk0E) != 0)
        {
          mdlDrawMapPositionRecords(mdlViewerState.resources[i]);
        }
      }
      i++;
    }
    while (i != MDL_VIEWER_RESOURCE_SLOTS);
  }
  D_003805A8.append(&D_003805A8, mdlViewerState.packetList);
  return 0;
}


extern char D_003C88A8[];
extern char D_00437100[]; /* "%x" */
extern char D_00437108[]; /* "fovy=" */
extern char D_00437110[]; /* "%f" */
extern char D_00437118[]; /* "fog=" */
extern KwlnDrawVectorParams kwlnDrawVector;
extern s32 sdfFileCheckExist(const char *path);


/* Read the viewer's config text file: bg-color=, eye-position=, target-position=, fovy=, fog= lines. */
INCLUDE_RODATA(const s32, "game/code_00233660", D_00421420);

void mdlLoadViewerPresentationConfig(void) {
    struct FileRequest *fileRequest;
    struct sdfMemHandle_t *resourceHandle;
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

    if (sdfFileCheckExist(D_003C88A8) == 0) {
        return;
    }
    fileRequest = fileLoad(D_003C88A8);
    fileSync(fileRequest);
    resourceHandle = fileGetMemHandle(fileRequest);
    fileData = (char *)fileGetLoadPtr(fileRequest);
    fileSize = (s32)fileGetLoadSize(fileRequest);
    fileHandleFree(fileRequest);
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
            if (sscanf(lineStart + 9, D_00437100, &color) == 1) {
                D_00435CBC = color;
            }
        } else if (memcmp(lineStart, "eye-position=", 13) == 0) {
            if (sscanf(lineStart + 13, "%f,%f,%f", &x, &y, &z) == 3) {
                D_003C87C0.x = x;
                D_003C87C0.y = y;
                D_003C87C0.z = z;
            }
        } else if (memcmp(lineStart, "target-position=", 16) == 0) {
            if (sscanf(lineStart + 16, "%f,%f,%f", &x, &y, &z) == 3) {
                D_003C87D0.x = x;
                D_003C87D0.y = y;
                D_003C87D0.z = z;
            }
        } else if (memcmp(lineStart, D_00437108, 5) == 0) {
            if (sscanf(lineStart + 5, D_00437110, &verticalFov) == 1) {
                dds3ScreenParam.camera.fov = verticalFov;
            }
        } else if (memcmp(lineStart, D_00437118, 4) == 0) {
            if (sscanf(lineStart + 4, "%d,%f,%d,%f,%x", &fogNear, &fogValue, &fogFar, &fogFarB, &color) == 5) {
                kwlnDrawVector.fogNear = fogNear;
                kwlnDrawVector.depthNear = fogValue;
                kwlnDrawVector.fogFar = fogFar;
                kwlnDrawVector.depthFar = fogFarB;
                kwlnDrawVector.fogColor = color;
            }
        }
        lineOffset = nextLineOffset;
    }
    sdfMemFreeDirect(resourceHandle);
}

const char D_00421488[] = "bg-color=%06x\neye-position=%f,%f,%f\ntarget-position=%f,%f,%f\nfovy=%f\nfog=%d,%f,%d,%f,%06x\n";

void mdlSaveViewerPresentationConfig(void) {
    char buffer[0x130];
    s32 size;
    struct FileRequest *request;

    size = sprintf(buffer, D_00421488, D_00435CBC,
                        D_00453620[0], D_00453620[1], D_00453620[2],
                        D_00453630[0], D_00453630[1], D_00453630[2],
                        dds3ScreenParam.camera.fov, (s32)kwlnDrawVector.fogNear,
                        kwlnDrawVector.depthNear, (s32)kwlnDrawVector.fogFar,
                        kwlnDrawVector.depthFar, kwlnDrawVector.fogColor);
    request = fileSave(D_003C88A8, buffer, size);
    fileSync(request);
    fileHandleFree((struct FileRequest *)request);
}

typedef struct MdlTaskDef {
    const char *name;
    s32 arg;
} MdlTaskDef;

extern MdlTaskDef D_003C8890[];

extern char D_004214E8[];

extern char D_004214F8[];

extern s32 mdlViewer(dds3ProcessID_t *);
extern s32 mdlViewerEnd(dds3ProcessID_t *);
extern void mdlMakeHddFileList(void);
extern s32 fclEncycFreeEntries(const char *);
extern MdlEyeVec D_003C8A80;
extern MdlEyeVec D_0037F6C0;

/* Load viewer config/resources, initialize view state and recreate the viewer tasks. */
void mdlInitializeViewerTasksAndResources(void) {
    MdlTaskDef *task;
    s32 i;

    mdlLoadViewerPresentationConfig();
    mdlViewerState.viewerScale = 1.0f;
    mdlViewerState.assetDisplayMode = mdlViewerState.unk0B = mdlViewerState.taskPhase = 1;
    mdlViewerState.captureDivY = mdlViewerState.captureDivX = mdlViewerState.captureScale = mdlViewerState.selectedResourceGroup = mdlViewerState.resourceGroup = 2;
    mdlViewerState.resourceId = 0;
    mdlViewerState.selectedResourceId = 0;
    mdlViewerState.exitCleanupStarted = 0;
    mdlViewerState.selectionHighlightPhase = 0;
    mdlViewerState.selectionCursor = 0;
    mdlViewerState.entryHeight = 0;
    mdlViewerState.entryWidth = 0;
    mdlViewerState.labelIndexB = 0;
    mdlViewerState.scrollPage = 0;
    mdlViewerState.unitStepMode = 0;
    mdlViewerState.unk0E = 0;
    mdlViewerState.workspaceSetupPhase = 0;
    mdlResetViewerBasisVectors();
    dds3ScreenParam.camera.fov = 0.4363323f;
    SDF_VU0_COPYVECTOR(&D_0037F6C0, &D_003C8A80);
    for (i = 0; i != 12; i++) {
        mdlViewerState.resources[i] = NULL;
    }
    mdlLoadViewerResourceAndResetCursors();
    mdlViewerState.resourceCount = 1;
    mdlMakeHddFileList();
    for (i = 0, task = D_003C8890; i != 3; i++, task++) {
        fclEncycFreeEntries(task->name);
    }
    mdlViewerState.unk00 = dds3StartProcess(D_004214E8, 0x2AFF, 1, 0, mdlViewer, NULL, 0);
    dds3StartProcess(D_004214F8, 0x2B01, 1, 0, mdlViewerEnd, NULL, 0);
    mdlViewerState.viewerTask = NULL;
    mdlRestartViewerPhaseTask();
}


/* Clear each occupied resource slot before destroying its context; retain the native scan form. */
void mdlFreeViewResources(void) {
    MdlViewState *viewerState = &mdlViewerState;
    dds3ModelHandle_t **resourceSlot = viewerState->resources;
    s32 resourceIndex;

    for (resourceIndex = 0; resourceIndex != MDL_VIEWER_RESOURCE_SLOTS; resourceIndex++) {
        dds3ModelHandle_t *resource = *resourceSlot;

        if (resource != 0) {
            *resourceSlot = 0;
            mdlFree(resource);
        }
        resourceSlot++;
    }
}

void mdlCleanupViewerTasksAndResources(void) {
    MdlTaskDef *def;
    s32 i = 0;

    mdlFreeViewResources();
    dds3KillProcessName(D_004214E8, 0);
    dds3KillProcessName(D_004214F8, 0);
    def = D_003C8890;
    do {
        if (dds3SearchProcessName(def->name) == NULL) {
            fclEncycAllocEntry(def->name, def->arg, 0, 0);
        }
        i++;
        def++;
    } while (i != 3);
}

extern MdlLightEditorState D_003C8AE0;
extern MdlOptionLabel D_003C8A90[4];
extern u16 D_003C8AC0[16];
extern s8 D_0037F520[16];
extern SdfLightSources D_0037F770;
extern f32 kwlnDefaultColorVector[4];
extern char D_00437138[]; /* ">" in .sdata */
extern char D_00437140[]; /* "%6.2f" in .sdata */
extern SdfPoolNode D_00380708;
extern void fldStepValueByPad(f32 *value, u8 *padState, f32 min, f32 max, f32 step, f32 bigStep);
extern void dds3PrimDebugTextBox(sdfDrawTag_t *packetList, s32 x, s32 y, s32 selected, f32 *values);
/* Direction-preview packet builder consumes the vector loaded in vf10. */
extern u64 *dds3PrimLine3D(s32, s32, s32, s32);

extern void FromRotation(f32 angle);

/* vu0 routine: rotate the light editor vector by two degrees about pad-selected camera axes. */
void mdlSetDebugMatrix(MdlLightEditorState *context, f32 *position)
{
    if (context->pad->stepUpA != 0) {
        SDF_VU0_LOADVECTOR_MTXCOL(sdfViewMatrix + 1);
        FromRotation(2.0f * 3.14159265f / 180.0f);
        effMiscQuaternionToMatrixVU();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_APPLYMATRIX_XYZ1();
        SDF_VU0_VECTOR_W_SET_ONE();
        SDF_VU0_STOREVECTOR(position);
    } else if (context->pad->stepDownA != 0) {
        SDF_VU0_LOADVECTOR_MTXCOL(sdfViewMatrix + 1);
        FromRotation(-2.0f * 3.14159265f / 180.0f);
        effMiscQuaternionToMatrixVU();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_APPLYMATRIX_XYZ1();
        SDF_VU0_VECTOR_W_SET_ONE();
        SDF_VU0_STOREVECTOR(position);
    }
    if (context->pad->stepUpB != 0) {
        SDF_VU0_LOADVECTOR_MTXCOL(sdfViewMatrix);
        FromRotation(-2.0f * 3.14159265f / 180.0f);
        effMiscQuaternionToMatrixVU();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_APPLYMATRIX_XYZ1();
        SDF_VU0_VECTOR_W_SET_ONE();
        SDF_VU0_STOREVECTOR(position);
    } else if (context->pad->stepDownB != 0) {
        SDF_VU0_LOADVECTOR_MTXCOL(sdfViewMatrix);
        FromRotation(2.0f * 3.14159265f / 180.0f);
        effMiscQuaternionToMatrixVU();
        SDF_VU0_LOADVECTOR(position);
        SDF_VU0_APPLYMATRIX_XYZ1();
        SDF_VU0_VECTOR_W_SET_ONE();
        SDF_VU0_STOREVECTOR(position);
    }
}

/* Update/draw the task-selected light and ambient editor; return zero. */
s32 mdlDebugPrintEffectScale(dds3ProcessID_t *task) {
    MdlLightEditorState *state = (MdlLightEditorState *)dds3GetProcessData(task);
    f32 (*light)[4];
    MdlOptionLabel *label;
    s32 changed;
    s32 labelIndex;
    s32 lightIndex;
    s32 j;
    s32 selected;
    s32 y;
    s32 directionStyle;
    s32 directionY;
    f32 *color;

    if ((u32)state == 0) {
        state = &D_003C8AE0;
        state->pad = (MdlPadState *)D_0037F520;
    } else if ((u32)state == 1) {
        state = &D_003C8AE0;
        state->pad = (MdlPadState *)dds3PadButtonState;
    }
    if (state->pad == NULL) {
        state->pad = (MdlPadState *)D_0037F520;
    }
    if (state->lights == NULL) {
        state->lights = D_0037F770;
    }
    if (state->ambient == NULL) {
        state->ambient = kwlnDefaultColorVector;
    }
    state->packetList = dds3FontReqAllocDrawTag();
    if (state->editing == 0) {
        changed = 1;
        if (state->pad->stepUpB & 0x80) {
            if (state->selection >= 14) {
                state->selection = 0;
            } else {
                state->selection++;
            }
        } else if (state->pad->stepUpB & 2) {
            if (state->selection < 14) {
                state->selection++;
            }
        } else if (state->pad->stepDownB & 0x80) {
            if (state->selection > 0) {
                state->selection--;
            } else {
                state->selection = 14;
            }
        } else if (state->pad->stepDownB & 2) {
            if (state->selection > 0) {
                state->selection--;
            }
        } else if (state->pad->confirm & 0x80) {
            state->editing = 1;
        } else {
            changed = 0;
        }
        if (changed != 0) {
            state->blinkTick = 0;
        }
    } else {
        if (state->pad->confirm & 0x80) {
            state->editing = 0;
        } else if (state->pad->cancel & 0x80) {
            state->editing = 0;
        } else if (state->selection < 12) {
            light = state->lights[state->selection >> 2];
            if (light != NULL) {
                if ((state->selection & 3) == 3) {
                    mdlSetDebugMatrix(state, light[1]);
                } else {
                    fldStepValueByPad(&light[0][state->selection & 3], (u8 *)state->pad,
                                      0.0f, 2.0f, 0.1f, 0.01f);
                }
            }
        } else if (state->selection < 15) {
            fldStepValueByPad(&state->ambient[state->selection - 12], (u8 *)state->pad,
                              0.0f, 2.0f, 0.1f, 0.01f);
        }
    }
    state->blinkTick++;
    if (state->blinkTick == 30) {
        state->blinkTick = 0;
    }
    sdfDrawTagLink(state->packetList,
                    (u32)dds3PrimFrameRect(0x8650, 0x79A8, 0xFF007F, 0x8A0, 0xAB0,
                                  0x60000000, 0x40806020));
    label = D_003C8A90;
    for (labelIndex = 0; labelIndex != 4; labelIndex++, label++) {
        sdfDrawTagLink(state->packetList,
                        (u32)sdfDevPrintf(label->x, label->y,
                                                         0xFF0080, label->style,
                                                         label->format));
    }
    if (state->editing == 0 && state->blinkTick < 20) {
        sdfDrawTagLink(state->packetList,
                        (u32)sdfDevPrintf(0x8680, D_003C8AC0[state->selection],
                                                         0xFF0080, 0, D_00437138));
    }
    selected = state->selection;
    y = 0x7A20;
    for (lightIndex = 0; lightIndex != 3; lightIndex++, selected -= 4, y += 0x300) {
        light = state->lights[lightIndex];
        if (light != NULL) {
            SDF_VU0_LOADVECTOR(light[1]);
            directionY = y + 0x1B0;
            directionStyle = 0;
            if (state->editing == 1) {
                directionStyle = selected == 3;
            }
            sdfDrawTagLink(state->packetList,
                            (u32)dds3PrimLine3D(0x88C0, directionY, 0xFF0080, directionStyle));
            for (j = 0; j != 3; j++) {
                sdfDrawTagLink(state->packetList,
                                (u32)sdfDevPrintf(0x8A40, y + 0x120 + j * 0x60, 0xFF0080,
                                                                 0, D_00437140, light[1][j]));
            }
            color = light[0];
        } else {
            color = NULL;
        }
        dds3PrimDebugTextBox(state->packetList, 0x8740, y,
                          state->editing == 1 ? selected : -1, color);
    }
    dds3PrimDebugTextBox(state->packetList, 0x8740, 0x8320,
                      state->editing == 1 ? state->selection - 12 : -1, state->ambient);
    D_00380708.append(&D_00380708, state->packetList);
    return 0;
}




void mdlDebugTextBox(s32 first, s32 second, s32 color, s32 variant) {
    s32 packedColor = color & 0xffffff;

    dds3PrimDebugTextBoxRGB(mdlDebugScaleWork.packetList, first, second,
                  (mdlDebugScaleWork.editing == 0) ? -1 : variant, packedColor | 0x80000000, 1);
}

typedef struct MdlValueEdit {
    s32 y;
    f32 *target;   /* 0x04 */
    s32 min;       /* 0x08 */
    s32 max;       /* 0x0C */
    const char *label;
    const char *format;
} MdlValueEdit;

extern MdlValueEdit D_003C8B08[];

/* Step the number being edited in table slot `index`: coarse steps (A) and fine steps (B),
   held-button repeat, wrapping from one end of [min, max] to the other on a fresh press. */
void mdlGetDebugEntry(s32 valueEditIndex) {
    MdlValueEdit *edit = &D_003C8B08[valueEditIndex];
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
    pad = mdlDebugScaleWork.pad;
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

struct dds3ProcessID_t;
extern s32 mdlDebugPrintEffectScale2(struct dds3ProcessID_t *);
extern MdlOptionLabel D_003C8AF8;
extern char D_00437178[];
extern s8 D_0040B7E8[16];

/* Edit the draw-vector target: four numeric rows and three packed-colour channels. */
s32 mdlDebugPrintEffectScale2(dds3ProcessID_t *task) {
    MdlCtrlState *state = &mdlDebugScaleWork;
    s8 *pad;
    s32 changed;
    s32 value;
    s32 i;

    if (dds3GetProcessData(task) == 0) {
        pad = D_0040B7E8;
    } else {
        pad = dds3PadButtonState;
    }
    state->pad = (MdlPadState *)pad;
    mdlDebugScaleWork.packetList = dds3FontReqAllocDrawTag();
    if (++mdlDebugScaleWork.blinkTick == 30) {
        mdlDebugScaleWork.blinkTick = 0;
    }
    if (mdlDebugScaleWork.editing == 0) {
        changed = 1;
        if (mdlDebugScaleWork.pad->stepUpB & 0x80) {
            if (mdlDebugScaleWork.selection >= 6) {
                mdlDebugScaleWork.selection = 0;
            } else {
                mdlDebugScaleWork.selection++;
            }
        } else if (mdlDebugScaleWork.pad->stepUpB & 2) {
            if (mdlDebugScaleWork.selection < 6) {
                mdlDebugScaleWork.selection++;
            }
        } else if (mdlDebugScaleWork.pad->stepDownB & 0x80) {
            if (mdlDebugScaleWork.selection > 0) {
                mdlDebugScaleWork.selection--;
            } else {
                mdlDebugScaleWork.selection = 6;
            }
        } else if (mdlDebugScaleWork.pad->stepDownB & 2) {
            if (mdlDebugScaleWork.selection > 0) {
                mdlDebugScaleWork.selection--;
            }
        } else if (mdlDebugScaleWork.pad->confirm & 0x80) {
            mdlDebugScaleWork.editing = 1;
        } else {
            changed = 0;
        }
        if (changed != 0) {
            mdlDebugScaleWork.blinkTick = 0;
        }
    } else {
        if (mdlDebugScaleWork.pad->confirm & 0x80) {
            mdlDebugScaleWork.editing = 0;
        } else if (mdlDebugScaleWork.pad->cancel & 0x80) {
            mdlDebugScaleWork.editing = 0;
        } else if (mdlDebugScaleWork.selection < 4) {
            mdlGetDebugEntry(mdlDebugScaleWork.selection);
        } else {
            fldStepColorChannelByPad(&kwlnDrawVector.fogColor, mdlDebugScaleWork.selection - 4, (s8 *)mdlDebugScaleWork.pad);
        }
    }
    sdfDrawTagLink(mdlDebugScaleWork.packetList,
                    (u32)dds3PrimFrameRect(0x8590, 0x79A8, 0xFF007F, 0x960, 0x570, 0x60000000, 0x40806020));
    sdfDrawTagLink(mdlDebugScaleWork.packetList,
                    (u32)sdfDevPrintf(D_003C8AF8.x, D_003C8AF8.y, 0xFF0080, D_003C8AF8.style, D_003C8AF8.format));
    if (mdlDebugScaleWork.editing == 0 && mdlDebugScaleWork.blinkTick < 0x14) {
        s32 y;

        if (mdlDebugScaleWork.selection < 4) {
            y = D_003C8B08[mdlDebugScaleWork.selection].y;
        } else {
            y = mdlDebugScaleWork.selection * 0x60 + 0x7AE0;
        }
        sdfDrawTagLink(mdlDebugScaleWork.packetList, (u32)sdfDevPrintf(0x85C0, y, 0xFF0080, 0, D_00437178));
    }
    for (i = 0; i != 4; i++) {
        s32 y = D_003C8B08[i].y;

        sdfDrawTagLink(mdlDebugScaleWork.packetList, (u32)sdfDevPrintf(0x8680, y, 0xFF0080, 0, D_003C8B08[i].label));
        value = *D_003C8B08[i].target + 0.5f;
        sdfDrawTagLink(mdlDebugScaleWork.packetList,
                        (u32)sdfDevPrintf(0x8A40, y, 0xFF0080,
                                                          (mdlDebugScaleWork.editing == 1 && mdlDebugScaleWork.selection == i) ? 6 : 0,
                                                          D_003C8B08[i].format, value));
    }
    mdlDebugTextBox(0x8680, 0x7C60, kwlnDrawVector.fogColor, mdlDebugScaleWork.selection - 4);
    D_00380708.append(&D_00380708, mdlDebugScaleWork.packetList);
    return 0;
}

#include "mdl_option_state.h"

extern MdlOptionState D_00453660;
extern s8 D_0040B7E8[16];
extern MdlOptionLabel D_003C8B68[4] __attribute__((section(".data")));
extern u16 D_003C8B98[4] __attribute__((section(".data")));
extern f32 sdfFaceDivSoften __attribute__((section(".sdata")));
extern f32 sdfFaceDivBias __attribute__((section(".sdata")));
/* The selector occupies byte zero of its aligned small-data allocation. */
extern u8 corePktSoftenBias __attribute__((section(".sdata")));
extern const char D_00437198[];
extern const char D_004371A0[];
extern const char D_004371A8[];
extern const char D_004371B0[];
extern const char D_004371B8[];

/* Update/draw the FACE-DIV soften, bias and divide controls; return zero. */
s32 soften_bias_Process(dds3ProcessID_t *task) {
    MdlOptionState *state = &D_00453660;
    MdlOptionLabel *label;
    s8 *pad;
    f32 value;
    f32 adjustment;
    s32 changed;
    s32 i;
    s32 valueStyle;
    s32 adjustmentStyle;
    const char *format;

    if (dds3GetProcessData(task) == 0) {
        pad = D_0040B7E8;
    } else {
        pad = dds3PadButtonState;
    }
    value = sdfFaceDivSoften;
    adjustment = sdfFaceDivBias;
    state->pad = pad;
    D_00453660.value = value;
    D_00453660.adjustment = adjustment;
    D_00453660.packetList = dds3FontReqAllocDrawTag();
    D_00453660.control.fields.blink++;
    if (D_00453660.control.fields.blink == 30) {
        D_00453660.control.fields.blink = 0;
    }

    if (D_00453660.control.fields.active == 0) {
        changed = 1;
        if (D_00453660.pad[7] & 0x80) {
            if (D_00453660.control.fields.cursor >= 2) {
                D_00453660.control.fields.cursor = 0;
            } else {
                D_00453660.control.fields.cursor++;
            }
        } else if (D_00453660.pad[7] & 2) {
            if (D_00453660.control.fields.cursor < 2) {
                D_00453660.control.fields.cursor++;
            }
        } else if (D_00453660.pad[6] & 0x80) {
            if (D_00453660.control.fields.cursor > 0) {
                D_00453660.control.fields.cursor--;
            } else {
                D_00453660.control.fields.cursor = 2;
            }
        } else if (D_00453660.pad[6] & 2) {
            if (D_00453660.control.fields.cursor > 0) {
                D_00453660.control.fields.cursor--;
            }
        } else if (D_00453660.pad[1] & 0x80) {
            if (D_00453660.control.fields.cursor == 2) {
                corePktSoftenBias ^= 1;
            } else {
                D_00453660.control.fields.active = 1;
            }
        } else {
            changed = 0;
        }
        if (changed != 0) {
            D_00453660.control.fields.blink = 0;
        }
    } else {
        if (D_00453660.pad[1] & 0x80) {
            D_00453660.control.fields.active = 0;
        } else if (D_00453660.pad[3] & 0x80) {
            D_00453660.control.fields.active = 0;
        } else {
            switch (D_00453660.control.fields.cursor) {
            case 0:
                fldStepValueByPad(&D_00453660.value, (u8 *)D_00453660.pad,
                                  1.0f, 50.0f, 1.0f, 0.1f);
                break;
            case 1:
                fldStepValueByPad(&D_00453660.adjustment, (u8 *)D_00453660.pad,
                                  -50.0f, 50.0f, 0.1f, 0.01f);
                break;
            }
        }
    }

    sdfDrawTagLink(D_00453660.packetList,
                    (u32)dds3PrimFrameRect(0x84D0, 0x79A8, 0xFF007F, 0xA20, 0x270,
                                  0x60000000, 0x40806020));
    label = D_003C8B68;
    for (i = 0; i != 4; i++, label++) {
        sdfDrawTagLink(D_00453660.packetList,
                        (u32)sdfDevPrintf(label->x, label->y,
                                                         0xFF0080, label->style,
                                                         label->format));
    }
    if (D_00453660.control.fields.active == 0 && D_00453660.control.fields.blink < 20) {
        sdfDrawTagLink(D_00453660.packetList,
                        (u32)sdfDevPrintf(0x8500,
                                                         D_003C8B98[D_00453660.control.fields.cursor],
                                                         0xFF0080, 0, D_00437198));
    }

    /* Ignore the blink byte when testing the active mode and selected row. */
    valueStyle = (D_00453660.control.packed & 0xFFFF00FF) == 1 ? 6 : 0;
    sdfDrawTagLink(D_00453660.packetList,
                    (u32)sdfDevPrintf(0x8B00, 0x7A80, 0xFF0080,
                                                     valueStyle, D_004371A0, D_00453660.value));
    adjustmentStyle = (D_00453660.control.packed & 0xFFFF00FF) == 0x10001 ? 6 : 0;
    sdfDrawTagLink(D_00453660.packetList,
                    (u32)sdfDevPrintf(0x8B00, 0x7AE0, 0xFF0080,
                                                     adjustmentStyle, D_004371A8, D_00453660.adjustment));
    format = corePktSoftenBias == 0 ? D_004371B0 : D_004371B8;
    sdfDrawTagLink(D_00453660.packetList,
                    (u32)sdfDevPrintf(0x8B00, 0x7BA0, 0xFF0080,
                                                     0, format));
    D_00380708.append(&D_00380708,
                      D_00453660.packetList);
    sdfFaceDivSoften = D_00453660.value;
    sdfFaceDivBias = D_00453660.adjustment;
    return 0;
}




extern void evtDisableMoonCalc(void);
extern void evtDisableMoonDraw(void);
extern void evtSetAgeOfMoon(s32 phase);

void evtInitGlobalWork(void) {
    evtBitAllOff();
    evtSetAgeOfMoon(0);
    evtDisableMoonDraw();
    evtDisableMoonCalc();
}


void evtBitAllOff(void) {
    u32 *word;
    s32 remaining;

    remaining = 0x7f;
    word = DDS3_GBWK->modelFlags.words;
    do {
        remaining = remaining - 1;
        *word = 0;
        word = word + 1;
    } while (-1 < remaining);
}

void evtBitAllOff_FieldDebug(void) {
    s32 i = 0;
    do {
        evtBitOff(i++);
    } while (i < 0xC00);
    i = 0xD00;
    do {
        evtBitOff(i++);
    } while (i < 0x1000);
}

/* Signed flag indices need a bias before arithmetic right shift divides by 32. */
void evtBitOn(s32 flag) {
    s32 adjustedFlag = (flag < 0) ? flag + 0x1f : flag;
    DDS3_GBWK->modelFlags.words[adjustedFlag >> 5] |= 1 << flag;
}

void evtBitOff(s32 flag) {
    s32 adjustedFlag = (flag < 0) ? flag + 0x1f : flag;
    DDS3_GBWK->modelFlags.words[adjustedFlag >> 5] &= ~(1 << flag);
}

s32 evtBitCheck(s32 flag) {
    s32 adjustedFlag = (flag < 0) ? flag + 0x1f : flag;
    return (((s32)DDS3_GBWK->modelFlags.words[adjustedFlag >> 5] >> flag) & 1);
}

extern s32 D_004371C0, D_004371C4, D_004371C8;
extern char D_004371D0[], D_004371D8[], D_004371E0[];
extern s32 dds3SelectMenu(s32, s32, s32, s32, s32,
    s32 *, s32 *, s32 *, s32 *);

/* Draw and toggle model-flag bits; return -1 on cursor exit, otherwise 0. */
INCLUDE_RODATA(const s32, "game/code_00233660", D_004214E8);

INCLUDE_RODATA(const s32, "game/code_00233660", D_004214F8);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421508);

INCLUDE_RODATA(const s32, "game/code_00233660", D_00421518);

s32 dds3BitEditorProcess(void) {
    char glyph[2];
    sdfDrawTag_t *packets;
    s32 column, row, style, result;
    s32 cellWidth = 0xC0, cellHeight = 0x60;

    /* Native 0023A1D4..E4 copies this space/NUL template as one block;
     * see docs/idioms.md, "Bit-editor glyph template block copy". */
    memcpy(glyph, D_004371D0, sizeof(glyph));
    packets = (sdfDrawTag_t *)sdfGrAllocPacketBuff(sizeof(*packets));
    sdfDrawTagInit(packets);
    dds3ConsPanelTagLink(packets, 50, 50, 23, 20);
    sdfDrawTagLink(packets, (u32)sdfDevPrintf(
        0x7320, 0x7A90, 0xFF0000, 0, "BitEditor"));
    if (evtBitCheck((D_004371C0 + D_004371C8) * 16 + D_004371C4)) {
        sdfDrawTagLink(packets, (u32)sdfDevPrintf(
            0x7AA0, 0x7A90, 0xFF0000, 7, "%04X = ON",
            (D_004371C0 + D_004371C8) * 16 + D_004371C4));
    } else {
        sdfDrawTagLink(packets, (u32)sdfDevPrintf(
            0x7AA0, 0x7A90, 0xFF0000, 7, "%04X = OFF",
            (D_004371C0 + D_004371C8) * 16 + D_004371C4));
    }
    for (column = 0; column < 16; column++) {
        style = D_004371C4 == column ? 14 : 7;
        sdfDrawTagLink(packets, (u32)sdfDevPrintf(
            0x7320 + (column + 6) * cellWidth, 0x7A90 + cellHeight * 2,
            0xFF0000, style, D_004371D8, column));
    }
    for (row = 0; row < 16; row++) {
        style = D_004371C8 == row ? 14 : 7;
        sdfDrawTagLink(packets, (u32)sdfDevPrintf(
            0x73E0, 0x7A90 + (row + 3) * cellHeight, 0xFF0000,
            style, D_004371E0, (D_004371C0 + row) * 16));
        for (column = 0; column < 16; column++) {
            if (evtBitCheck((D_004371C0 + row) * 16 + column)) {
                glyph[0] = 'O';
                style = D_004371C4 == column || D_004371C8 == row ? 6 : 0;
            } else {
                glyph[0] = '+';
                style = D_004371C4 == column || D_004371C8 == row ? 14 : 8;
            }
            if (D_004371C4 == column && D_004371C8 == row) {
                style = 4;
            }
            sdfDrawTagLink(packets, (u32)sdfDevPrintf(
                0x7320 + (column + 6) * cellWidth,
                0x7A90 + (row + 3) * cellHeight, 0xFF0000, style, glyph));
        }
    }
    D_00380708.append(&D_00380708, packets);
    if ((dds3PadBitTable[24] & 2) && D_004371C0 > 0) {
        D_004371C0 -= 16;
        if (D_004371C0 < 0) D_004371C0 = 0;
    }
    if (dds3PadBitTable[26] & 2) {
        if (D_004371C0 + 16 < 256) {
            if (D_004371C0 + 32 > 256) D_004371C0 = 240;
            else D_004371C0 += 16;
        }
    }
    result = dds3SelectMenu(1, 16, 256, 16, 16, NULL,
        &D_004371C0, &D_004371C4, &D_004371C8);
    if (result == -1) return -1;
    if (result == 1) {
        if (evtBitCheck((D_004371C0 + D_004371C8) * 16 + D_004371C4)) {
            evtBitOff((D_004371C0 + D_004371C8) * 16 + D_004371C4);
        } else {
            evtBitOn((D_004371C0 + D_004371C8) * 16 + D_004371C4);
        }
    }
    return 0;
}

extern dds3Basic_t *dds3AddCameraObjectBasicTypeFree(s32 world, u128 *pos, u128 *rot);

void evtSetTestCamera(void) {
    f32 pos[4] = {0.0f, -100.0f, -600.0f, 0.0f};
    f32 rot[4];
    dds3Basic_t *obj;

    memset(rot, 0, 0x10);
    rot[3] = 1.0f;
    obj = dds3AddCameraObjectBasicTypeFree(dds3AllocObjectId(), (u128 *)pos, (u128 *)rot);
    dds3GetRadiusPoseSet(obj, 10.0f);
    dds3ObjectBaseCreateMover(obj);
    dds3SetActiveCameraSceneBasic(dds3GetControlScene(), obj);
    dds3ObjectBaseSetPadControl(obj, 0);
}

s32 mdlSpawnCameraSlotViewerObject(s32 slotKind, s32 resource) {
    f32 pos[4];
    f32 rot[4];
    s32 world;
    dds3Basic_t *obj;

    memset(pos, 0, 0x10);
    pos[3] = 1.0f;
    memset(rot, 0, 0x10);
    rot[3] = 1.0f;
    world = dds3AllocObjectId();
    obj = dds3AddUnitObjectBasic(world, (u128 *)pos, (u128 *)rot);
    dds3ObjectBaseOnFlag(obj, 0x20);
    dds3GetRadiusPoseSet(obj, 10.0f);
    dds3RebuildObjectModelContext(obj, slotKind, resource);
    if (((dds3ModelHandle_t *)dds3ObjectBaseGetModelHandle(obj))->first != NULL) {
        mdlLoopMotion((dds3ModelHandle_t *)dds3ObjectBaseGetModelHandle(obj), 0, 0);
    }
    dds3ObjectBaseSetPadControl(obj, 0);
    return world;
}


extern s32 dds3ObjectBaseRegistArea(dds3Basic_t *obj);

s32 evtLoadUnit(s32 slotKind, s32 resource) {
    f32 pos[4];
    f32 rot[4];
    s32 world;
    dds3Basic_t *obj;

    memset(pos, 0, 0x10);
    pos[3] = 1.0f;
    memset(rot, 0, 0x10);
    rot[3] = 1.0f;
    world = dds3AllocObjectId();
    obj = dds3AddUnitObjectBasic(world, (u128 *)pos, (u128 *)rot);
    dds3ObjectBaseOnFlag(obj, 0x20);
    dds3GetRadiusPoseSet(obj, 10.0f);
    dds3RebuildObjectModelContext(obj, slotKind, resource);
    mdlLoopMotion((dds3ModelHandle_t *)dds3ObjectBaseGetModelHandle(obj), 0, 0);
    dds3ObjectBaseSetControlBasic(obj, dds3SearchSceneBasic(dds3GetControlScene(), 0x10000, 2));
    dds3ObjectBaseRegistArea(obj);
    dds3ObjectBaseSetPadControl(obj, 0);
    dds3ObjectBaseOnFlag(obj, 0x400);
    return world;
}

void evtSetUnitPos(u32 targetKey, u32 sourceKey) {
    f32 axis[4] = {0.0f, 1.0f, 0.0f, 1.0f};
    dds3Basic_t *obj;
    dds3Basic_t *src;
    EvtWorldSourceTransformPrefix *sourceTransform;

    obj = dds3SearchSceneBasic(dds3GetControlScene(), targetKey, 5);
    if (obj != NULL) {
        src = dds3SearchSceneBasic(dds3GetControlScene(), sourceKey, 0x11);
        if (src != NULL) {
            sourceTransform = src->data;
            dds3SetPosPoseSet(obj, (u128 *)&sourceTransform->position);
            SDF_VU0_LOADVECTOR(axis);
            FromRotation(3.14159265f);
            SDF_VU0_LOADVECTOR_B(sourceTransform->rotation);
            effMiscQuatMultiplyVU();
            SDF_VU0_STOREVECTOR(axis);
            dds3SetRotPoseSet(obj, (u128 *)axis);
            dds3GetPosPoseSet(obj);
            SDF_VU0_STOREVECTOR(obj->inner->smoothedPosition);
        }
    }
}

INCLUDE_SDATA(struct sdfMemHandle_t * const, "game/code_00233660", D_00436FAC);

INCLUDE_SDATA(struct sdfMemHandle_t * const, "game/code_00233660", D_00436FB0);

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
