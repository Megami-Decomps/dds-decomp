#include "common.h"
#include "eff_curve.h"
#include "file.h"
#include "pcp_vu0.h"
struct EffectSlotSet;
extern void func_00306CD0(s32, s32, s32, u32, u32, struct EffectSlotSet *, s32, s32);

enum { FILE_CONFIG_SET, FILE_CONFIG_FRAME, FILE_CONFIG_X, FILE_CONFIG_Y };
extern s16 D_003E9028[27][4];

extern s32 D_00437D4C;
extern s32 func_002C9BD0(void);
extern void evtSubmitDefaultDepthGradientRect(s32, s32, s32, s32, s32, s32, s32, s32);

/* Compact metadata copied from the beginning of each save blob. */
typedef struct FileRecordHeader {
    char signature[3];
    s8 version;
    s8 mapGroup;
    s8 mapIndex;
    u8 pad06[2];
    s32 playTicks;
    s16 status;
    s16 newCycle;
    s8 party[8];
    s8 levels[8];
    u32 money;
    u32 header24;
    u32 header28;
    u32 header2C;
} FileRecordHeader;

typedef struct FileScrollArrowState {
    s32 angle;
    s32 upAlpha;
    s32 unk08;
    s32 downAlpha;
} FileScrollArrowState;

extern FileScrollArrowState D_003E7FF8;
extern f32 D_00437D24;
extern u8 D_003E8658[];
extern u8 D_003A41A8[][32];
extern u8 D_003A47E8[][32];
extern char D_00437D98[];
extern char D_00437DA0[];
extern char D_00437DA8[];
extern u32 func_002CF978(u32, s32, s8);
extern void fileDrawSlotIcon(s32, s32, s32, s32);
extern void fileCursorStepUp(void);
extern void fileFadeStepDown(void);
extern void fileLoadCtxSlideUpdate(void);

extern u8 D_003E8B98[];
extern u8 D_003E8BB0[];
extern u8 D_003E8BD0[];
typedef struct EffectSurfaceNode {
    u32 capacity;
    u32 color;
    f32 scale;
    u32 kind;
    u8 pad_10[0x1C];
    u32 index;
    u32 pad_30;
    void *resource;
    void **jobs;
    u32 jobHandle;
    void **queues;
    u32 queueHandle;
    void *referenceHolder;
    u32 active;
    u16 unk50;
} EffectSurfaceNode;

extern u32 effRetainResource(u32);
extern void billSetBillboardMode(u32, s16);
extern u32 billCreateIndexed(u32, u32);
extern void billMarkKindOneFlag(u32);
extern u32 billCloneObjectRetainingSharedData(u32);
extern u32 effReferenceObjectRetain(u32);
extern u32 fileSaveReadBuffer;
extern u32 fileAllocateGridRecordSlots(u16, u32, void *);
typedef struct MdlFlagPair {
    s32 flag;
    s32 unk4;
} MdlFlagPair;
extern MdlFlagPair D_003E8CE8[];
extern MdlFlagPair D_003E8D10[];
extern s32 mdlFlagTest(s32);
extern void mdlFlagSet(s32);
extern void mdlFlagClear(s32);
extern s32 mnuAdvanceTitleStateUnderSemaphore(void);
extern s32 mnuDestroyListState();
extern s32 effDestroyResourceSlotSet();
#include "kwln.h"
#include "fpu.h"
struct MenuListNode;
extern struct MenuListNode *mnuAdvanceListCursorDefault(u32 list);
extern struct MenuListNode *mnuRetreatListCursorDefault(u32 list);
extern void mnuClearListFlagsOneAndTwo(u32 *flags);
extern void kwlnFadeInStart(s8, s8, s8, s32);

extern void *fileDuplicateJob(void *);

extern s32 sdfAllocGeneralBlock();

extern s32 sdfResourceRetainAddress();

typedef struct DevState DevState;
extern DevState *sdfDevCreateCommandState(s32);

extern u32 sdfDevQueueControlAndWait(DevState *);
extern void sdfDevQueueReadAndWait(DevState *, void *, s32);
extern void sdfDevWaitThenReleaseCommandState(DevState *);


extern s32 fileLoadSelectionWork;

extern u32 fileWaitTicksRemaining;

extern u32 D_00437CEC;

extern s32 fileSlotSelectionPollCount;

extern u32 D_00437CFC;

extern u32 D_00437D08;

extern u32 D_00437D14;

extern u32 D_00437D18;

extern u32 D_00437D1C;

extern u32 D_00437D20;

extern u32 D_00437D40;

extern u32 D_00437D44;

extern u32 D_00437D3C;

extern u32 func_0019F5E8(s32, s32, s32, u32, char *, s32);
extern u32 itfCreateConvertedTextGlyph(s32, s32, s32, u32, const u8 *, s32);
extern u32 D_00439004;
extern u32 D_00439008;

extern u32 D_0043900C;

extern u32 func_0019CE78(u32, u32, u32, u32, u32);

extern s32 D_00437CD8;

extern s32 D_00437CE4;
extern s32 D_003E7FA8[];
extern s32 D_00437D74;
extern s32 D_00437D6C;
extern s32 D_00437D68;
extern s32 D_00437D64;
extern s32 D_00437D60;
extern s32 D_00437D5C;
extern u8 (*D_00437D54)[32];
extern u8 (*D_00437D58)[32];
extern s32 sdfTexReleaseReferenceViaHandler(s32);
extern s32 dds3GetWorldObject(void);
extern void dds3SetWorldObjectDataValue(s32, s32);
extern void fileWaitReady(u32);
extern void sdfReleaseMemorySlot(void *);
extern void sdfFreeMemoryFromEitherHeap(void *);

extern s32 D_00437D38;

extern s32 D_00437D48;

extern u32 fileMemoryCardRequestContext;

extern u32 D_00437CF4;

extern u32 D_00437CF8;

extern u32 fileSelectionPending;

extern u32 D_00437D04;

extern s32 datGameState;

extern u32 fileSavedSlotFlags;

extern s32 fileConfigTaskWork;

extern u32 effModelUpdateControlFlags;

extern u32 func_002DDF48(u32);

extern s8 fileMenuTaskAlive;
extern s32 mcdOriginalTitleFileMode;
extern s32 D_00437D88;
extern s32 func_0035C860(char *dst, const char *fmt, ...);

extern s32 fileSlotSelectPoll(void);

extern void fileReqBegin(s32 context);

extern void *fileBeginSlotMetadataRefresh(void);

extern void *fileStartMemoryCardDetection(void);

extern void *mcResetSlotMetadata(void);

extern void *filePollSlotDetectionAndBranch(void);

extern s32 fileIsCardSpaceAboveMinimum(void);

extern s32 fileReqGetSize(s32 context);

extern u32 D_00439020;

extern void mcdFinishFileDetection(void);

extern void *fileBeginDetectionRequest(u32 callback);

extern void *filePollSlotRequestAndResumeFlow(void);

extern s32 fileSlotScanIndex;

extern void mcdFormatSaveSlotName(void *buffer, s32 slot);

extern void mcChangeCurrentDirectory(u32 context, void *buffer);

extern void *fileScanSlotIconSysBegin(void);

extern void *fileScanSlotIconSysAltBegin(void);

extern void *filePrepareMainBlobWrite(void);

extern void fileOnAllWritten(void);

extern void func_001004A0(void);
extern s8 D_0037F510[];
extern void sndSetSequenceVolumePan(s32 sequence, s32 volume, s32 pan);
extern void func_00342580(s32 command);
extern void *fileBeginWait(s32 result);

extern void func_002C92D0(u32 context);

extern void *mcHandleDetectionResult(void);

extern char D_0042B720[];

extern KwlnTask *kwlnTaskGetTaskByName(const char *name);

/* Loader context at D_0037D4A0. */
typedef struct LoadCtx374A0 {
    u8 unk0[4]; /* 0x00 */
    s32 unk4;   /* 0x04 */
    s32 inputRepeatTimer; /* 0x08 */
    s32 unkC;   /* 0x0C */
    s8 unk10;   /* 0x10 */
    u8 pad11[3]; /* 0x11 */
    s32 unk14;  /* 0x14 */
    s32 unk18;  /* 0x18 */
    s32 unk1C;  /* 0x1C */
} LoadCtx374A0;

extern LoadCtx374A0 fileLoadMenuState;

/* Far scalar: incomplete array forces non-small-data addressing. */
extern s32 D_003E8008[];

extern u32 D_003E9150[];

/* Callback table at D_0037E14C (0x28 bytes per entry). */
typedef struct Cb3714C {
    void (*cb)(void *arg); /* 0x00 */
    u8 pad4[8];            /* 0x04 */
    void (*cbC)(void *, void *); /* 0x0C */
    u8 pad10[0x18];        /* 0x10 */
} Cb3714C;

extern Cb3714C D_003E916C[];

typedef struct FileTypeCallbacks {
    void *(*create)(void *);
    void (*unk4)(void *);
    void (*destroy)(void *);
    void *(*createChild)(void *, u16);
    u8 unk10[0x18];
} FileTypeCallbacks;

extern FileTypeCallbacks fileJobTypeOperations[];

extern void sdfVuMatrixToQuaternion(f32 matrix[4][4]);

/* Effect parameter-set dispatch tables. Every effect kind owns one 0x28-byte
 * entry per table; the handler lives at +0x0. Slots are declared as separate
 * arrays (D_00353710/14/18/1C/20/24/28/2C/30/34 and D_00353880/84/88/90/94/
 * 98/9C/A0/A4). The family2 create table (D_00353880) additionally carries a
 * fallback selector at +0x0C: nonzero calls the entry handler directly,
 * zero falls back through D_003536A0.
 */
typedef struct EffDispatchEntry {
    void *(*func)(void *); /* 0x00 handler, may be NULL */
    u8 pad04[0x08];        /* 0x04 */
    u32 unk0C;             /* 0x0C fallback selector (create table only) */
    u8 pad10[0x18];        /* 0x10 */
} EffDispatchEntry; /* 0x28 */

/* 8-byte parameter work (family1): kind id plus one data pointer. */
typedef struct EffParamWork {
    u16 id;       /* 0x00 effect kind */
    u8 pad02[2];  /* 0x02 */
    void *data;   /* 0x04 parameter block */
} EffParamWork; /* 0x08 */

typedef struct EffDispatchExtra {
    void (*func)(void *, void *); /* 0x00 handler, may be NULL */
    u8 pad04[0x24];
} EffDispatchExtra;

typedef struct EffDispatchScale {
    void (*func)(void *, f32); /* 0x00 handler, may be NULL */
    u8 pad04[0x24];
} EffDispatchScale;
typedef struct EffDispatchColor {
    void (*func)(void *, u32); /* 0x00 handler, may be NULL */
    u8 pad04[0x24];
} EffDispatchColor;

extern EffDispatchEntry D_003E917C[];

extern EffDispatchExtra D_003E9180[];

extern EffDispatchExtra D_003E9184[];

extern EffDispatchScale D_003E9188[];

extern EffDispatchColor D_003E918C[];

extern s8 fileConfigTaskState;

extern void mcdFinishFileDetectionWithAudio(void);

extern void func_002CAED0(void);

extern s32 D_00437D50;

extern s8 D_004580C3[];

extern void *(*fileMenuStateHandler)(s32);

extern void evtSetDrawSurfaceIndex(u32);

extern void evtSubmitPrimaryAlphaBlendMode(s32);

extern void evtSubmitPrimaryGsTest(s32, s32, s32, s32, s32, s32, s32, s32);

extern s32 D_00437D70;

extern void func_00108EC0(s32, s32, s32, s32, s32, s32, s32, s32, u32, u32, u32, u32, s32);

extern void fileCursorPulseUpdate(void);

extern void func_002CFF38(s32, s32, s32);

extern s32 D_00437D78;

extern void *fileReadSlotPreviewBegin(void);
extern s32 mcPollCommandStatusWithResult(s32 *);
extern void mcReadOpenFile(s32, u32, s32);
extern void *mcHandleSetupResult(void);
extern s32 fileSaveFileDescriptor;
extern u32 fileSaveReadBufferResource;
extern void *fileReadSlotPreviewWait(void);

extern void *func_002CBA90(void);

extern void fileReqSetSelectedSlot(u32 ctx, s32 slot);

extern void mcOpenFilePath(u32, const char *, s32);

extern void *fileBeginSlotOpen(void);

extern u32 fileReqGetSlotFlags(s32 context, s32 slot);

extern void *fileSlotSelectPollClear(void);

extern void *fileRestartSlotSelection(void);

extern void *fileSlotStatusPoll(void);

extern void *fileResetSelection(void);

extern u32 D_00458080[];

extern void *fileWaitContinuation;

extern void *fileUpdateWait(void);

extern void fileReqSetSlotFlags(s32 context, s32 slot, s32 flags);

extern void *mcHandleSlotWriteResult(void);

extern s32 mcPollNonnegativeResult(void *result);

extern s32 fileBeginSlotPromptFive(void);

extern void *fileScanSlotStatesAdvance(void);

extern void *mcHandleDirectoryWriteResult(void);

extern void *mcPrepareDirectory(void);

extern void fileReqMarkSlotMetadataDirty(s32);

extern void *mcHandleSearchResult(void);

extern void *fileScanSlotStates(void);

extern void *fileLoadMainBlobBegin(void);

extern s32 fileIsRequestReadyInCurrentMode(u32, void *);

extern u32 fileGetResourceHandle(u32);

extern u32 fileGetLoadedDataAddress(u32);

extern u32 fileGetResourceSize(u32);

extern void filePollEntryCleanup(u32);

extern s32 fileDrawMenuFrame(s32);

extern u32 D_00439034;

extern u32 D_00439038;

extern char fileConfigInputTaskName[];

extern char fileConfigLoadTaskName[]; /* "config_draw" */

extern char fileConfigOwnerTaskName[]; /* "config_update" */

extern void kwlnTaskDestroyWithHierarchyByName(const char *name, s32 hierarchy);

typedef struct LoadMirror {
    u32 current;
    u32 previous;
} LoadMirror;

extern LoadMirror fileSlotFlagMirror;

extern s32 fileLoadStateChanged(void);

extern void fileCacheSlotFlagsFromState(void);

extern void *sdfAllocSizeClassBlock(s32 size);


extern void fileLoadObjectSetResource(EffectSurfaceNode *node, u32 entryId, void *resource);
extern void fileLoadObjectOpenDevice(EffectSurfaceNode *node, u32 resourceId);
extern void fileLoadObjectOpenAndStartDevice(EffectSurfaceNode *node, u32 resourceId);
extern void fileLoadObjectOpenNamedDevice(EffectSurfaceNode *node, u32 resourceId);
extern void fileReplaceEffectSurfaceJobs(EffectSurfaceNode *node, FileJob *job);
extern void fileReplaceEffectSurfaceQueues(EffectSurfaceNode *node, FileJob *job);
extern void fileReplaceReferenceHolder(EffectSurfaceNode *node, u32 resource);

extern FileJob *fileCreateJob(u16 type);

extern void fileJobFreePrimaryBuffer(FileJob *job);
extern void fileJobFreeSecondaryBuffer(FileJob *job);

/* Only fields needed by the save copy are exposed; the remaining state is opaque. */
typedef struct FileSaveState {
    FileRecordHeader header; /* 0x00 */
    u8 pad30[0xA24];
    u32 slotFlags;      /* 0xA54 */
    u8 padA58[0x1DBF8];
    u32 savedMoney;     /* 0x1E650 */
} FileSaveState;

typedef struct FileQueue {
    f32 offset[4];
    f32 axis[4];
    u8 unk20[0x20];
    f32 position[4];   /* 0x40 */
    f32 quat[4];       /* 0x50 */
    f32 scale;         /* 0x60 */
    u32 color;         /* 0x64: modulation colour */
    u32 transformWord; /* 0x68 */
    u8 pad6C[8];
    f32 transformValue; /* 0x74 */
    u8 pad78[8];
    s32 count;
    u32 unk84;
    FileJob *last;   /* 0x88: append end */
    FileJob *first;  /* 0x8C: traversal start */
} FileQueue;

extern void fileQueueAppend(FileQueue *queue, FileJob *job);

extern void fileQueueInitTransform(void *queue);

extern FileJob *fileJobCreate(void);

typedef struct ScaleEntry {
    f32 unk0;
    f32 value;
} ScaleEntry;

typedef struct ScaleSet {
    u8 pad0[0x64];
    f32 unk64;
    f32 unk68;
    u8 pad6C[4];
    ScaleEntry entries[3];
    u8 pad88[0x40];
    f32 unkC8;
    f32 unkCC;
    f32 unkD0;
    f32 unkD4;
    f32 unkD8;
    f32 unkDC;
    f32 unkE0;
    f32 unkE4;
    u8 padE8[8];
    f32 unkF0;
} ScaleSet;

typedef struct ScaleOwner {
    u8 pad0[0x20];
    ScaleSet *dst;
    ScaleSet *src;
} ScaleOwner;

extern void *fileBeginSlotResetPrompt(void);

extern void *fileBeginSlotReset(void);

extern s32 D_00437D30;

extern s32 fileBeginPromptDialog(void *start, void *finish, s32 mode);

extern void *fileBeginDirectoryScan(void);

extern char D_0042B698[];

extern void fileAbortSlotScanOnInput(void);

extern s32 mcPollSyncResult(void);

extern void mcReadDirectoryEntries(u32 context, const char *path, void *buffer, s32 mode);

extern char D_0042B6B8[];

extern u8 D_00458040[];

extern void fileReqClearSlotFlags(s32, s32);

extern u8 fileReqIsSlotMetadataDirty(s32 context);

extern void *mcClearSlotMetadata(void);

extern void *mcChooseLoadPath(void);

extern s32 D_00437D2C;

extern void *fileBuildMainBlobAfterDelete(void);

extern void *func_002CC210();

extern void mcDeleteFilePath(void);

extern s32 mnuSelectFileBranch(void);

extern u32 D_00437D0C;

extern u8 sdfViewMatrix[];

extern u8 sdfProjectionMatrix[];

extern u8 D_0037F660[];


extern void sdfPostmultiplyVuMatrixFromMemory(void *);

extern void fileQueueSetPosition(FileQueue *queue, void *vec);
extern void fileQueueSetRotation(FileQueue *queue, void *rot);

extern void fileQueueSetScale(FileQueue *queue, f32 scale);

extern void func_002D49B8(FileQueue *queue, u32 color);

extern void fileJobCopyHeader(FileJob *dst, FileJob *src);
extern FileJob *fileQueueFindById(FileQueue *queue, u32 id);
extern FileJob *fileQueueGetAt(FileQueue *queue, s32 index);
extern s32 fileFindQueuedJobIndex(FileQueue *queue, FileJob *target);

extern s32 mcPollWithExtendedErrors(void);

extern s32 mcPollNormalizedCommandStatus(void);

extern void sdfReleaseChipBlock();

extern f32 fileSaveHighlightPhase;

extern f32 sdfSinPoly(f32);

extern s32 itfMesGetGlobalWindowValue(void);

extern u8 sdfPfsDebugMode;

extern char D_00437E10[];

extern char D_00437E18[];

extern char D_00437E20[];

extern char *sdfDevGetPathBuffer(void);

extern s32 func_00369B70(const char *path, s32 flags, ...);

extern void func_002D3B48(s32 fd, s32 arg1);

extern void func_00369DF8(s32 fd);

extern void func_0036BCD0(const char *path, s32 arg1);

void fileDestroyMenuTask(void) {
    if (D_00437CD8 != 0) {
        kwlnTaskDestroyWithHierarchy(D_00437CD8, 1);
        D_00437CD8 = 0;
        fileMenuTaskAlive = 0;
    }
}

s8 fileMenuTaskIsAlive(void) {
    return fileMenuTaskAlive;
}

void mnuFormatSaveSlotHeaderName(void) {
}

void mcdFormatSaveSlotName(void *buffer, s32 slot) {
    s32 titleId = 0x52a0;
    if (mcdOriginalTitleFileMode != 0) {
        titleId = 0x51ee;
    }
    D_00437D88 = titleId;
    func_0035C860(buffer, "BASLUS-%05d-new-%d", titleId, slot);
}

void fileReqGetSlotCode(void) {
    s32 slot = fileReqGetSelectedSlot(fileMemoryCardRequestContext);
    D_00437D50 = D_004580C3[slot * 0x30];
}

/* Size of the persistent main save block copied during a reload. */
#define FILE_MAIN_BLOB_SIZE 0x1E840

u32 fileMainBlobSize(void) {
    return FILE_MAIN_BLOB_SIZE;
}

void fileReloadSaveBuffer(void) {
    s32 saved = *(s32 *)(datGameState + 0x30);
    s32 size = FILE_MAIN_BLOB_SIZE;
    memcpy((void *)datGameState, (void *)fileSaveReadBuffer, size);
    *(s32 *)(datGameState + 0x30) = saved;
}

u8 fileIsLoadedAndConditionTrue(s32 condition) {
    return condition != 0 && D_00437CE4 == 1;
}

u8 fileIsLoadedWithActiveFlow(s32 condition) {
    return condition != 0 && D_00437CE4 == 1;
}

void func_002C9818(s32 x, s32 y, u32 colors, const u8 *text) {
    u32 handle = itfCreateConvertedTextGlyph(x << 4, y << 3, 0, colors, text, 0);
    D_00439004 = handle;
    frFontDrawGlyphWithSharedFlags(handle, 1);
    frFontQueueGlyphInSelectedSlot(D_00439004);
}

void mcdCreateConfiguredDrawHandle(s32 x, s32 y, u32 colors, const u8 *text) {
    u32 handle = itfCreateConvertedTextGlyph(x << 4, y << 3, 0, colors, text, 0);
    D_00439008 = handle;
    frFontSetChainFlag(handle, 3);
    frFontDrawGlyphWithSharedFlags(D_00439008, 1);
    frFontQueueGlyphInSelectedSlot(D_00439008);
}

void mcdCreateFontDrawHandle(s32 x, s32 y, u32 colors, u32 glyphSource) {
    frFontAddSharedGlyphFlags(1);
    D_0043900C = func_0019CE78(glyphSource, 0, 0, 0, 0);
    frFontClearFlagBits(1);
    frFontSetFlagAndMeasureGlyphs(D_0043900C, 1);
    frFontSetContextPair(D_0043900C, x << 4, y << 3);
    frFontSetChildColors(D_0043900C, colors);
    func_0019D550(D_0043900C, 0, 0x56);
    frFontQueueGlyphInSelectedSlot(D_0043900C);
    frFontSetSharedRenderFlags(0x54);
}

void fileDrawMenuImageAtPoint(s32 x, s32 y, u32 colors, char *text) {
    u32 handle;

    handle = func_0019F5E8(x << 4, y << 3, 0, colors, text, 0);
    frFontDrawGlyphWithSharedFlags(handle, 1);
    frFontQueueGlyphInSelectedSlot(handle);
}

/* Animate the save-window highlight's alpha with a sinusoidal phase. */
void fileDrawPulsingSaveHighlight(void) {
    s32 angle;
    f32 wave;

    evtSetDrawSurfaceIndex(0x56);
    evtSubmitPrimaryAlphaBlendMode(0);
    fileSaveHighlightPhase = fileSaveHighlightPhase + 0.39999998f;
    sdfSinPoly(fileSaveHighlightPhase);
    angle = ((s32)D_003E8008[2] + 8) % 360;
    D_003E8008[2] = angle;
    wave = sdfSinPoly((f32)((angle + 0x5A) % 360) / 180.0f * 3.1415899f);
    D_003E8008[3] = (s32)((wave + 1.0f) * 0.5f * 191.0f + 64.0f);
    func_00108EC0(0x1BE, 0x12C, 0x13, 0x1F, 1, 1, 0x13, 0x1F, (D_003E8008[3] << 24) | 0xAEC014,
                  (D_003E8008[3] << 24) | 0xAEC014, (D_003E8008[3] << 24) | 0xAEC014,
                  (D_003E8008[3] << 24) | 0xAEC014, itfMesGetGlobalWindowValue());
}

void fileDrawSaveWindow(void) {
    evtSetDrawSurfaceIndex(0x56);
    evtSubmitPrimaryAlphaBlendMode(0);
    func_00108EC0(0x112, 0x113, 0x98, 0x34, 0x14A, 0x1C5, 0x98, 0x34, 0x80808080, 0x80808080, 0x80808080, 0x80808080, D_00437D78);
    func_00108EC0(0x56, 0x113, 0xBC, 0x34, 0x14A, 0x1C5, 1, 0x34, 0x80808080, 0x80808080, 0x80808080, 0x80808080, D_00437D78);
    fileCursorPulseUpdate();
    func_002CFF38(0x17E, 0x118, 0x56);
    D_00437D44++;
}

s32 fileIsLoadStepComplete(void) {
    if (D_00437D48 < 7) {
        return 0;
    }
    return 1;
}

/* Draw the expanding prompt panel and report when its input delay has elapsed. */
s32 func_002C9BD0(void) {
    s32 y;
    s32 height;
    s32 value;

    evtSetDrawSurfaceIndex(0x56);
    evtSubmitPrimaryAlphaBlendMode(0);
    evtSubmitPrimaryGsTest(1, 0, 128, 3, 0, 0, 1, 1);
    if (D_00437D48 < 32) {
        D_00437D48++;
        D_00437D4C++;
    }
    if (D_00437D48 < 4) {
        height = D_00437D48 * 72;
        y = 224 - D_00437D48 * 36;
    } else {
        y = 80;
        height = 288;
    }
    if (D_00437D48 < 4) {
        value = D_00437D48 * 70 / 4;
    } else {
        value = 70;
    }
    evtSubmitDefaultDepthGradientRect(0, y, 512, height,
                 (((value << 7) / 100) << 24) | 0xA1000,
                 (((value << 7) / 100) << 24) | 0xA1000,
                 (((value << 7) / 100) << 24) | 0xA1000,
                 (((value << 7) / 100) << 24) | 0xA1000);
    evtSetDrawSurfaceIndex(0x53);
    if (D_00437D48 < 7) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002C96D0", func_002C9CF8);

void fileSetMenuFlowState(u32 value) {
    s32 previous;

    previous = D_00437D38;
    D_00437D38 = value;
    if (previous == 0) {
        D_00437D48 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_002C96D0", func_002CA1F0);

void fileClearAllSlotFlags(void) {
    fileReqMarkSlotMetadataDirty(fileMemoryCardRequestContext);
    fileReqClearSlotFlags(fileMemoryCardRequestContext, 0);
    fileReqClearSlotFlags(fileMemoryCardRequestContext, 1);
    fileReqClearSlotFlags(fileMemoryCardRequestContext, 2);
    fileReqClearSlotFlags(fileMemoryCardRequestContext, 3);
    fileReqClearSlotFlags(fileMemoryCardRequestContext, 4);
    fileReqClearSlotFlags(fileMemoryCardRequestContext, 5);
    fileReqClearSlotFlags(fileMemoryCardRequestContext, 6);
    fileReqClearSlotFlags(fileMemoryCardRequestContext, 7);
    fileReqClearSlotFlags(fileMemoryCardRequestContext, 8);
    fileReqClearSlotFlags(fileMemoryCardRequestContext, 9);
}

void *fileBeginSlotOpen(void) {
    char path[0x50];
    u32 ctx;
    s32 slot;
    s32 len;

    fileReqSetSelectedSlot(fileMemoryCardRequestContext, fileSlotScanIndex);
    ctx = fileMemoryCardRequestContext;
    slot = fileReqGetSelectedSlot(ctx);
    if ((fileReqGetSlotFlags(ctx, slot) & 1) == 0) {
        return func_002CBA90();
    }
    path[0] = 0x2F;
    mcdFormatSaveSlotName(&path[1], slot);
    len = strlen(&path[1]);
    path[len + 1] = 0x2F;
    memcpy(&path[len + 2], &path[1], len);
    path[len * 2 + 2] = 0;
    mcOpenFilePath(ctx, path, 1);
    return fileReadSlotPreviewBegin;
}

void *fileReadSlotPreviewBegin(void) {
    s32 status = mcPollCommandStatusWithResult(&fileSaveFileDescriptor);

    if (status == 0) {
        return NULL;
    }
    if (status == 1) {
        fileSaveReadBufferResource = sdfAllocGeneralBlock(0x30);
        fileSaveReadBuffer = sdfResourceRetainAddress(fileSaveReadBufferResource);
        mcReadOpenFile(fileSaveFileDescriptor, fileSaveReadBuffer, 0x30);
        return fileReadSlotPreviewWait;
    }
    return fileBeginSlotMetadataRefresh();
}

extern s32 mcPollCompletionStatus(void);
extern void mcCloseOpenFile(u32);
extern void *fileStoreSlotHeader(void);

void *fileReadSlotPreviewWait(void) {
    s32 r = mcPollCompletionStatus();

    if (r == 0) {
        return 0;
    }
    if (r == 1) {
        mcCloseOpenFile(fileSaveFileDescriptor);
        return (void *)fileStoreSlotHeader;
    }
    sdfReleaseResourceAllocation(fileSaveReadBufferResource);
    return fileBeginSlotMetadataRefresh();
}

extern s32 mcPollZeroCommandResult(void);
extern FileRecordHeader D_004580C0[];

void *fileStoreSlotHeader(void) {
    s32 status = mcPollZeroCommandResult();

    if (status == 0) {
        return NULL;
    }
    if (status == 1) {
        memcpy(&D_004580C0[fileSlotScanIndex], (void *)fileSaveReadBuffer, 0x30);
        sdfReleaseResourceAllocation(fileSaveReadBufferResource);
        return func_002CBA90();
    }
    sdfReleaseResourceAllocation(fileSaveReadBufferResource);
    return fileBeginSlotMetadataRefresh();
}

void *fileBeginWait(s32 result) {
    kwlnFadeInStart(0, 0, 0, 15);
    fileWaitContinuation = (void *)result;
    fileWaitTicksRemaining = 0x14;
    return fileUpdateWait;
}

void *fileBeginSlotReset(void) {
    D_00437D30 = 0;
    fileSetMenuFlowState(0);
    D_00437D3C = 0;
    D_00437D1C = 4;
    D_00437CF8 = 0;
    return (void *)fileBeginPromptDialog(&fileResetSelection, &fileBeginSlotResetPrompt, 0);
}

void *fileBeginDirectoryScan(void) {
    fileSetMenuFlowState(0);
    D_00437D3C = 0;
    D_00437D1C = 9;
    return (void *)fileBeginPromptDialog(&mcPrepareDirectory, &fileScanSlotStates, 1);
}

extern void *func_002CACF0(void);

void *fileBeginSlotResetPrompt(void) {
    fileSetMenuFlowState(0);
    D_00437D3C = 0;
    D_00437D1C = 8;
    D_00437CF8 = 0;
    return (void *)fileBeginPromptDialog(&func_002CACF0, &fileBeginSlotReset, 1);
}

void *fileReturnToSlotSelection(void) {
    fileSetMenuFlowState(0);
    D_00437CF8 = 0;
    return fileSlotSelectPoll;
}

void *fileRestartSlotSelection(void) {
    fileReqBegin(0);
    fileSlotSelectionPollCount = 30;
    D_00437CEC = 1;
    fileSetMenuFlowState(1);
    D_00437CF8 = 1;
    return fileSlotSelectPollClear;
}

s32 fileBeginSlotPromptFive(void) {
    D_00437CF4 = 1;
    fileSetMenuFlowState(0);
    D_00437D1C = 5;
    return fileBeginPromptDialog(&func_002CACF0, &fileRestartSlotSelection, 1);
}

extern void *func_002CACF0(void);

s32 fileBeginSlotPromptSix(void) {
    D_00437CF4 = 1;
    fileSetMenuFlowState(0);
    D_00437D1C = 6;
    return fileBeginPromptDialog(&func_002CACF0, &fileRestartSlotSelection, 1);
}

void *fileBeginSlotMetadataRefresh(void) {
    fileClearAllSlotFlags();
    fileSetMenuFlowState(0);
    D_00437D3C = 1;
    D_00437D40 = 0;
    fileReqBegin(fileMemoryCardRequestContext);
    return filePollSlotDetectionAndBranch;
}

void *fileResetSelection(void) {
    fileClearAllSlotFlags();
    D_00458080[0] = 0;
    D_00458080[1] = 0;
    D_00458080[2] = 0;
    D_00458080[3] = 0;
    D_00458080[4] = 0;
    D_00458080[5] = 0;
    D_00458080[6] = 0;
    D_00458080[7] = 0;
    D_00458080[8] = 0;
    D_00458080[9] = 0;
    fileReqBegin(0);
    fileSlotSelectionPollCount = 15;
    D_00437CEC = 1;
    fileSetMenuFlowState(1);
    D_00437CF8 = 1;
    return fileSlotStatusPoll;
}
extern void dds3AdminSubmitModeRequest(s32, const s32 *, s32, s32);
extern void *fileMenuWorkStart(void);


u32 fileAbortSlotFlow(void) {
    fileSetMenuFlowState(0);
    D_00437CF8 = 0;
    D_00437CF4 = 0;
    return 0xffffffff;
}

void *mcdEnterDefaultFileFlow(void) {
    s32 mode = 0;
    if (mcdOriginalTitleFileMode != 0) {
        return fileMenuWorkStart;
    }
    dds3AdminSubmitModeRequest(2, &mode, 4, 0);
    return 0;
}

void *func_002CACF0(void) {
    return mcdEnterDefaultFileFlow();
}

void *mcdEnterSelectedFileFlow(void) {
    s32 mode = 2;
    fileSelectionPending = 1;
    if (mcdOriginalTitleFileMode != 0) {
        return fileMenuWorkStart;
    }
    dds3AdminSubmitModeRequest(2, &mode, 4, 0);
    return 0;
}

void fileRestartSlotScan(void) {
    fileClearAllSlotFlags();
    fileSetMenuFlowState(1);
    D_00437D3C = 0;
    D_00437CF8 = 0;
    fileResetSelection();
}

extern u32 D_00458080[];
extern void *func_002CCAD0(void);

void *fileScanSlotStates(void) {
    s32 i;

    D_00437D14 = 0;
    D_00437CF8 = 1;
    for (i = 0; i < 10; i++) {
        u32 buttons = fileReqGetSlotFlags(fileMemoryCardRequestContext, i);

        D_00458080[i] = 0;
        if (buttons & 1) {
            if (buttons & 2) {
                if (buttons & 8) {
                    D_00458080[i] = 1;
                }
            }
        } else if (buttons & 2) {
            if (!(buttons & 8)) {
                D_00458080[i] = 2;
            }
        }
    }
    fileReqBegin(fileMemoryCardRequestContext);
    return func_002CCAD0;
}

void *fileResolveAbortSlotFlow(void) {
    if (D_00437D30 == 1 && kwlnTaskGetTaskByName(D_0042B698) == NULL) {
        return mcdEnterSelectedFileFlow();
    }
    fileSetMenuFlowState(0);
    return fileBeginWait(&fileAbortSlotFlow);
}

extern u32 D_00439030;
extern u32 fileSaveIconRequest;
extern char D_0042B6A8[];

void fileLoadIconFileAndResetSelection(void) {
    D_00439030 = 0;
    fileSaveIconRequest = fileQueueDefaultCallbackRequest(D_0042B6A8);
    fileResetSelection();
}

void func_002CAED0(void) {
    fileResetSelection();
}

void fileLoadIconFileAndBeginSlotReset(void) {
    D_00439030 = 0;
    fileSaveIconRequest = fileQueueDefaultCallbackRequest(D_0042B6A8);
    fileBeginSlotReset();
}

void fileResetSlotPollState(void) {
    fileReqBegin(0);
    D_00437CEC = 1;
    fileSlotSelectionPollCount = 0;
    fileSetMenuFlowState(0);
    D_00437CF8 = 0;
    fileReturnToSlotSelection();
}

void *fileSlotStatusPoll(void) {
    s32 status;

    if (fileReqPoll() == 0) {
        return NULL;
    }
    if (fileSlotSelectionPollCount > 0) {
        fileSlotSelectionPollCount--;
        fileReqBegin(fileMemoryCardRequestContext);
        return NULL;
    }
    status = fileReqGetStatus(fileMemoryCardRequestContext);
    switch (status) {
    case 0:
        return fileBeginSlotMetadataRefresh();
    case 2:
        if (D_00437D30 == 0) {
            fileClearAllSlotFlags();
            fileSetMenuFlowState(0);
            D_00437D3C = 0;
            D_00437D1C = 2;
            return (void *)fileBeginPromptDialog(fileStartMemoryCardDetection, mnuSelectFileBranch, 1);
        }
        fileSetMenuFlowState(0);
        D_00437D3C = 7;
        D_00437D40 = 0;
        fileReqBegin(fileMemoryCardRequestContext);
        return filePollSlotDetectionAndBranch;
    case 3:
        fileClearAllSlotFlags();
        fileSetMenuFlowState(0);
        D_00437D3C = 2;
        D_00437D40 = 0;
        fileReqBegin(fileMemoryCardRequestContext);
        return filePollSlotDetectionAndBranch;
    default:
        return NULL;
    case 1:
        return mcResetSlotMetadata();
    }
}

s32 fileCountSelectableFiles(void) {
    s32 count = 0;
    s32 index;
    for (index = 0; index < 10; index++) {
        u32 flags = fileReqGetSlotFlags(fileMemoryCardRequestContext, index);
        if (flags & 1) {
            if (flags & 2) {
                if (flags & 8) {
                    count++;
                }
            }
        }
    }
    return count;
}

s32 fileIsCardSpaceAboveMinimum(void) {
    return fileReqGetSize(fileMemoryCardRequestContext) > 0x2A7FF;
}

extern s32 fileBeginSlotPromptFive(void);
extern void *func_002CACF0(void);
extern s32 fileIsCardSpaceAboveMinimum(void);
extern void *fileRestartSlotSelection(void);

s32 fileSlotSelectPoll(void) {
    s32 result = fileReqPoll();

    if (result == 0) {
        return result;
    }
    if ((s32)fileSlotSelectionPollCount > 0) {
        fileSlotSelectionPollCount--;
        fileReqBegin(fileMemoryCardRequestContext);
        return 0;
    }
    switch (fileReqGetStatus(fileMemoryCardRequestContext)) {
    case 0:
    case 3:
        return fileBeginSlotPromptFive();
    case 2:
        return (s32)func_002CACF0();
    case 1:
        if (fileIsCardSpaceAboveMinimum() != 0) {
            return (s32)func_002CACF0();
        }
        return (s32)fileRestartSlotSelection();
    default:
        return 0;
    }
}

extern s32 fileBeginSlotPromptFive(void);
extern void *func_002CACF0(void);
extern s32 fileIsCardSpaceAboveMinimum(void);

void *fileSlotSelectPollClear(void) {
    if (fileReqPoll() == 0) {
        return NULL;
    }
    if ((s32)fileSlotSelectionPollCount > 0) {
        fileSlotSelectionPollCount--;
        fileReqBegin(fileMemoryCardRequestContext);
        return NULL;
    }
    switch (fileReqGetStatus(fileMemoryCardRequestContext)) {
    case 0:
    case 3:
        return (void *)fileBeginSlotPromptFive();
    case 2:
        return func_002CACF0();
    case 1:
        if (fileIsCardSpaceAboveMinimum() != 0) {
            return func_002CACF0();
        }
        return mcClearSlotMetadata();
    default:
        return NULL;
    }
}

void *filePollSlotDetectionAndBranch(void) {
    s32 reqStatus;

    D_00437CF8 = 0;
    if (D_00437D40 == 0) {
        if (fileIsLoadedAndConditionTrue(D_0037F510[0x21] < 0) ||
            fileIsLoadedAndConditionTrue(D_0037F510[0x23] < 0)) {
            sndSetSequenceVolumePan(0xA, 0x7F, 0x3F);
            D_00437D40 = 1;
        }
    }
    if (fileReqPoll() != 0) {
        if (D_00437D40 == 1) {
            D_00437D3C = 0;
            D_00437CF8 = 0;
            if (D_00437D0C != 0) {
                return fileBeginSlotResetPrompt();
            }
            if (D_00437D30 == 0) {
                return fileBeginWait(&fileAbortSlotFlow);
            }
            return mcdEnterSelectedFileFlow();
        }
        reqStatus = fileReqGetStatus(fileMemoryCardRequestContext);
        switch (reqStatus) {
        case 1:
            if (D_00437D3C == 1) {
                fileClearAllSlotFlags();
                fileSetMenuFlowState(1);
                D_00437D3C = 0;
                D_00437CF8 = 0;
                return fileResetSelection();
            }
            fileReqBegin(fileMemoryCardRequestContext);
            break;
        case 0:
            fileClearAllSlotFlags();
            if (D_00437D3C != 1) {
                D_00437D3C = 1;
                D_00437D40 = 0;
            }
            fileReqBegin(fileMemoryCardRequestContext);
            break;
        case 2:
            if (D_00437D30 == 0) {
                fileClearAllSlotFlags();
                D_00437D3C = 0;
                D_00437D1C = 2;
                return (void *)fileBeginPromptDialog(&fileStartMemoryCardDetection, &mnuSelectFileBranch, 1);
            }
            if (D_00437D3C != 7) {
                fileClearAllSlotFlags();
                D_00437D3C = 7;
                D_00437D40 = 0;
            }
            fileReqBegin(fileMemoryCardRequestContext);
            break;
        case 3:
            if (D_00437D3C != 2) {
                fileClearAllSlotFlags();
                D_00437D3C = 2;
                D_00437D40 = 0;
            }
            fileReqBegin(fileMemoryCardRequestContext);
            break;
        }
    }
    return NULL;
}

void *filePollSlotScanOrReset(void) {
    if (D_00437D40 == 0) {
        if (fileIsLoadedAndConditionTrue(D_0037F510[0x21] < 0) ||
            fileIsLoadedAndConditionTrue(D_0037F510[0x23] < 0)) {
            sndSetSequenceVolumePan(8, 0x7F, 0x3F);
            D_00437D40 = 1;
        }
    }
    if (fileReqPoll() != 0) {
        if (D_00437D40 == 1) {
            D_00437D3C = 0;
            return fileScanSlotStates();
        }
        if (fileReqGetStatus(fileMemoryCardRequestContext) != 1) {
            D_00437D3C = 0;
            D_00437CF8 = 0;
            return fileResetSelection();
        }
        fileReqBegin(fileMemoryCardRequestContext);
    }
    return NULL;
}

void fileAbortSlotScanOnInput(void) {
    if (fileIsLoadedAndConditionTrue(D_0037F510[0x21] < 0) ||
        fileIsLoadedAndConditionTrue(D_0037F510[0x23] < 0)) {
        sndSetSequenceVolumePan(8, 0x7F, 0x3F);
        D_00437D3C = 0;
        D_00437CF8 = 0;
        fileResetSelection();
    }
}

void *fileUpdateWait(void) {
    s32 remaining = fileWaitTicksRemaining - 1;
    fileWaitTicksRemaining = remaining;
    if (remaining <= 0) {
        if (D_00439020 == -1) {
            return (void *)-1;
        }
        return ((void *(*)(void))fileWaitContinuation)();
    }
    return NULL;
}
void *fileSetMenuCallbackAndClearResult(u32 menuCallback) {
    D_00439020 = menuCallback;
    D_00437D40 = 0;
    return mcdFinishFileDetection;
}

void mcdFinishFileDetection(void) {
    if (fileIsLoadedAndConditionTrue((u32)D_0037F510[0x21] >> 31) ||
        fileIsLoadedAndConditionTrue((u32)D_0037F510[0x23] >> 31)) {
        fileSetMenuFlowState(0);
        D_00437D3C = 0;
        D_00437CF8 = 0;
        if (mcdOriginalTitleFileMode == 0) {
            func_00342580(0x310000);
        }
        if (D_00439020 == (u32)-1) {
            fileBeginWait(-1);
            return;
        }
        ((void (*)(void))D_00439020)();
    }
}

void *fileBeginDetectionRequest(u32 callback) {
    D_00439020 = callback;
    D_00437D40 = 0;
    fileReqBegin(fileMemoryCardRequestContext);
    return filePollSlotRequestAndResumeFlow;
}

void *filePollSlotRequestAndResumeFlow(void) {
    s32 reqStatus;

    if (D_00437D40 == 0) {
        if (fileIsLoadedAndConditionTrue(D_0037F510[0x21] < 0) ||
            fileIsLoadedAndConditionTrue(D_0037F510[0x23] < 0)) {
            sndSetSequenceVolumePan(8, 0x7F, 0x3F);
            D_00437D40 = 1;
        }
    }
    if (fileReqPoll() != 0) {
        if (D_00437D40 == 1) {
            fileSetMenuFlowState(0);
            D_00437D3C = 0;
            D_00437CF8 = 0;
            if (D_00439020 == -1) {
                return fileBeginWait(-1);
            }
            return ((void *(*)(void))D_00439020)();
        }
        reqStatus = fileReqGetStatus(fileMemoryCardRequestContext);
        switch (reqStatus) {
        case 1:
            if (D_00437D3C == 1) {
                fileClearAllSlotFlags();
                fileSetMenuFlowState(1);
                D_00437D3C = 0;
                D_00437CF8 = 0;
                return fileResetSelection();
            }
            fileReqBegin(fileMemoryCardRequestContext);
            break;
        case 0:
            return fileBeginSlotMetadataRefresh();
        case 2:
            if (D_00437D30 == 0) {
                fileClearAllSlotFlags();
                fileSetMenuFlowState(0);
                D_00437D3C = 0;
                D_00437D1C = 2;
                return (void *)fileBeginPromptDialog(&fileStartMemoryCardDetection, &mnuSelectFileBranch, 1);
            }
            if (D_00437D3C != 7) {
                fileClearAllSlotFlags();
                fileSetMenuFlowState(0);
                D_00437D3C = 7;
                D_00437D40 = 0;
            }
            fileReqBegin(fileMemoryCardRequestContext);
            break;
        case 3:
            if (D_00437D3C != 2) {
                fileClearAllSlotFlags();
                fileSetMenuFlowState(0);
                D_00437D3C = 2;
                D_00437D40 = 0;
            }
            fileReqBegin(fileMemoryCardRequestContext);
            break;
        }
    }
    return NULL;
}

void *fileBeginReadSlotIcon(void) {
    u8 buf[0x50];

    buf[0] = 0x2F;
    mcdFormatSaveSlotName(&buf[1], fileSlotScanIndex);
    mcChangeCurrentDirectory(fileMemoryCardRequestContext, buf);
    return fileScanSlotIconSysBegin;
}

void *fileScanSlotIconSysBegin(void) {
    s32 t = mcPollSyncResult();

    if (t == 0) {
        return NULL;
    }
    if (t == 1) {
        fileReqSetSlotFlags(fileMemoryCardRequestContext, fileSlotScanIndex, 2);
        mcReadDirectoryEntries(fileMemoryCardRequestContext, D_0042B6B8, D_00458040, 1);
        return mcHandleSlotWriteResult;
    }
    if (t == -1) {
        return fileBeginSlotMetadataRefresh();
    }
    return fileBeginSlotOpen();
}

void *mcHandleSlotWriteResult(void) {
    s32 value;
    s32 status = mcPollNonnegativeResult(&value);
    if (status == 0) {
        return NULL;
    }
    if (status == 1) {
        if (value == 1) {
            fileReqSetSlotFlags(fileMemoryCardRequestContext, fileSlotScanIndex, 9);
        }
        return fileBeginSlotOpen();
    }
    if (status == -1) {
        return fileBeginSlotMetadataRefresh();
    }
    return fileBeginSlotOpen();
}

INCLUDE_ASM(const s32, "game/code_002C96D0", func_002CBA90);

void *mcResetSlotMetadata(void) {
    s32 index;
    if (fileReqIsSlotMetadataDirty(fileMemoryCardRequestContext) != 0) {
        fileSetMenuFlowState(1);
        fileSlotScanIndex = 0;
        index = 0;
        do {
            D_00458080[index] = 0;
            fileReqClearSlotFlags(fileMemoryCardRequestContext, index);
            index++;
        } while (index < 10);
        return fileBeginReadSlotIcon();
    } else {
        D_00437CEC = 0;
        fileSetMenuFlowState(0);
        return fileScanSlotStates();
    }
}

void *fileBeginSaveSlotIconScan(void) {
    u8 buf[0x50];

    buf[0] = 0x2F;
    mcdFormatSaveSlotName(&buf[1], fileSlotScanIndex);
    mcChangeCurrentDirectory(fileMemoryCardRequestContext, buf);
    return fileScanSlotIconSysAltBegin;
}

void *fileScanSlotIconSysAltBegin(void) {
    s32 t = mcPollSyncResult();

    if (t == 0) {
        return NULL;
    }
    if (t == 1) {
        fileReqSetSlotFlags(fileMemoryCardRequestContext, fileSlotScanIndex, 2);
        mcReadDirectoryEntries(fileMemoryCardRequestContext, D_0042B6B8, D_00458040, 1);
        return mcHandleDirectoryWriteResult;
    }
    if (t == -1) {
        fileSetMenuFlowState(0);
        D_00437CF8 = 0;
        D_00437D3C = 0;
        return (void *)fileBeginSlotPromptFive();
    }
    return fileScanSlotStatesAdvance();
}

void *mcHandleDirectoryWriteResult(void) {
    s32 value;
    s32 status = mcPollNonnegativeResult(&value);
    if (status == 0) {
        return NULL;
    }
    if (status == 1) {
        if (value == 1) {
            fileReqSetSlotFlags(fileMemoryCardRequestContext, fileSlotScanIndex, 9);
        }
        return fileScanSlotStatesAdvance();
    }
    if (status == -1) {
        fileSetMenuFlowState(0);
        D_00437CF8 = 0;
        D_00437D3C = 0;
        return (void *)fileBeginSlotPromptFive();
    }
    return fileScanSlotStatesAdvance();
}

void *fileScanSlotStatesAdvance(void) {
    s32 slot = fileSlotScanIndex;
    s32 buttons = fileReqGetSlotFlags(fileMemoryCardRequestContext, slot);

    slot++;
    if (buttons & 1) {
        if (buttons & 2) {
            if (buttons & 8) {
                fileSetMenuFlowState(0);
                D_00437CF8 = 0;
                D_00437D3C = 0;
                return (void *)func_002CACF0();
            }
        }
    }
    fileSlotScanIndex = slot;
    if (slot == 10) {
        fileSetMenuFlowState(0);
        D_00437CF8 = 0;
        D_00437D3C = 0;
        return (void *)fileBeginSlotPromptSix();
    }
    return fileBeginSaveSlotIconScan();
}

void *mcClearSlotMetadata(void) {
    s32 index = 0;
    u32 *saved;
    fileSetMenuFlowState(1);
    D_00437CF8 = 0;
    fileSlotScanIndex = 0;
    fileReqMarkSlotMetadataDirty(fileMemoryCardRequestContext);
    saved = D_00458080;
    do {
        *saved++ = 0;
        fileReqClearSlotFlags(fileMemoryCardRequestContext, index);
        index++;
    } while (index < 10);
    return fileBeginSaveSlotIconScan();
}

void *mcPrepareDirectory(void) {
    u8 name[0x50];
    u32 entry = fileMemoryCardRequestContext;
    s32 slot = fileReqGetSelectedSlot(entry);
    fileReqGetSlotFlags(entry, slot);
    fileSetMenuFlowState(2);
    fileReqMarkSlotMetadataDirty(entry);
    if (fileReqGetSlotFlags(entry, slot) & 2) {
        return fileCreateMainBegin();
    }
    name[0] = '/';
    mcdFormatSaveSlotName(name + 1, slot);
    mcMakeDirectory(entry, name);
    return mcHandleSearchResult;
}

void *fileCreateMainBegin(void) {
    u8 buf[0x50];
    u32 entry = fileMemoryCardRequestContext;
    s32 v = fileReqGetSelectedSlot(entry);

    buf[0] = 0x2F;
    mcdFormatSaveSlotName(&buf[1], v);
    mcChangeCurrentDirectory(entry, buf);
    return filePrepareMainBlobWrite;
}

void *mcHandleSearchResult(void) {
    s32 status = mcPollWithExtendedErrors();
    if (status == 0) {
        return NULL;
    }
    if (status == 1) {
        fileReqSetSlotFlags(fileMemoryCardRequestContext, D_00437D2C, 2);
        return fileCreateMainBegin();
    }
    fileSetMenuFlowState(0);
    D_00437D3C = 4;
    return fileAbortSlotScanOnInput;
}

void *filePrepareMainBlobWrite(void) {
    s32 t = mcPollSyncResult();

    if (t == 0) {
        return NULL;
    }
    if (t == 1) {
        return mcChooseLoadPath();
    }
    if (t == -1) {
        fileSetMenuFlowState(0);
        D_00437D3C = 4;
        return fileAbortSlotScanOnInput;
    }
    return NULL;
}

void *fileBuildMainBlobAfterDelete(void) {
    s32 t = mcPollNormalizedCommandStatus();

    if (t == 0) {
        return NULL;
    }
    if (t == 1) {
        return func_002CC210();
    }
    if (t == -1) {
        fileSetMenuFlowState(0);
        D_00437D3C = 4;
        return fileAbortSlotScanOnInput;
    }
    return NULL;
}

void fileOnAllWritten(void) {
    fileReqSetSlotFlags(fileMemoryCardRequestContext, D_00437D2C, 1);
    fileReqSetSlotFlags(fileMemoryCardRequestContext, D_00437D2C, 8);
    fileSetMenuFlowState(4);
    fileBeginDetectionRequest((u32)fileScanSlotStates);
}

void *fileWriteIconSysComplete(void) {
    fileDestroyMenuTask();
    return fileOnAllWritten;
}

extern u8 D_003E7BE0[];
extern u32 D_00437D90;
extern u32 D_00437D94;

s32 fileBeginLabeledSlotWrite(void) {
    s32 v = D_00437D2C + 1;
    s32 q = v / 10;
    s32 r = v % 10;

    D_003E7BE0[0xF8] = -0x7E;
    D_003E7BE0[0xFA] = -0x7E;
    D_003E7BE0[0xF9] = q + 0x4F;
    D_003E7BE0[0xFB] = r + 0x4F;
    return fileBeginRequest(D_0042B6B8, &D_00437D90, &D_00437D94, (void *)fileWriteIconSysComplete, 0);
}

extern s32 fileBeginRequest(char *, void *, void *, void *, void *);
extern s32 fileBeginLabeledSlotWrite(void);

INCLUDE_RODATA(const s32, "game/code_002C96D0", D_0042B698);

INCLUDE_RODATA(const s32, "game/code_002C96D0", D_0042B6A8);

INCLUDE_RODATA(const s32, "game/code_002C96D0", D_0042B6B8);

s32 fileRequestBaseIcon(void) {
    return fileBeginRequest("base.ico", &D_00439034, &D_00439038, fileBeginLabeledSlotWrite, &fileSaveIconRequest);
}

INCLUDE_ASM(const s32, "game/code_002C96D0", func_002CC210);

void *mcChooseLoadPath(void) {
    u32 entry = fileMemoryCardRequestContext;
    s32 slot = fileReqGetSelectedSlot(entry);
    u32 flags = fileReqGetSlotFlags(entry, slot);
    if (!(flags & 8)) {
        return func_002CC210(entry, D_0042B6B8);
    }
    mcDeleteFilePath();
    return fileBuildMainBlobAfterDelete;
}

extern u32 *D_00439048;
extern u32 *D_0043904C;
extern u32 D_00439050;
extern u32 D_00439054;
extern s32 fileWriteWaitOpen(void);

s32 fileBeginRequest(char *name, void *a1, void *a2, void *a3, void *a4) {
    D_00439048 = (u32 *)a1;
    D_0043904C = (u32 *)a2;
    D_00439050 = (u32)a3;
    D_00439054 = (u32)a4;
    mcOpenFilePath(fileMemoryCardRequestContext, name, 0x203);
    return (s32)fileWriteWaitOpen;
}

extern void mcBeginWrite(s32 request, u32 first, u32 second);
extern void *mcHandleLoadResult(void);
extern void *fileFinishRequest(void);

s32 fileWriteWaitOpen(void) {
    s32 status = mcPollCommandStatusWithResult(&fileSaveFileDescriptor);

    if (status == 0) {
        return 0;
    }
    if (status == 1) {
        if (D_00439054 != 0 && *(u32 *)D_00439054 != 0) {
            return (s32)fileFinishRequest;
        }
        mcBeginWrite(fileSaveFileDescriptor, *D_00439048, *D_0043904C);
        return (s32)mcHandleLoadResult;
    }
    if (status == -1) {
        fileSetMenuFlowState(0);
        D_00437D3C = 4;
        return (s32)fileAbortSlotScanOnInput;
    }
    return 0;
}


extern s32 mcPollWriteCompletion(void);
extern void *mcDispatchReadCallback(void);

void *mcHandleLoadResult(void) {
    s32 status = mcPollWriteCompletion();

    if (status == 0) {
        return 0;
    }
    if (status == 1) {
        mcCloseOpenFile(fileSaveFileDescriptor);
        return (void *)mcDispatchReadCallback;
    }
    if (status == -1) {
        fileSetMenuFlowState(0);
        D_00437D3C = 4;
        return (void *)fileAbortSlotScanOnInput;
    }
    return 0;
}

extern u32 D_00439050;
void *mcDispatchReadCallback(void) {
    s32 status = mcPollZeroCommandResult();

    if (status == 0) {
        return 0;
    }
    if (status == 1) {
        return ((void *(*)(void))D_00439050)();
    }
    if (status == -1) {
        fileSetMenuFlowState(0);
        D_00437D3C = 4;
        return (void *)fileAbortSlotScanOnInput;
    }
    return 0;
}

void *fileFinishRequest(void) {
    if (fileSaveIconRequest != 0) {
        return NULL;
    }
    mcBeginWrite(fileSaveFileDescriptor, *D_00439048, *D_0043904C);
    return mcHandleLoadResult;
}

void *mcHandleDetectionResult(void) {
    s32 status = mcPollStrictSuccess();

    if (status == 0) {
        return 0;
    }
    func_00100498();
    if (status < 0) {
        fileSetMenuFlowState(0);
        D_00437D3C = 6;
        return (void *)fileAbortSlotScanOnInput;
    }
    fileSetMenuFlowState(6);
    return (void *)fileBeginDetectionRequest((u32)fileRestartSlotScan);
}

void *fileStartMemoryCardDetection(void) {
    fileSetMenuFlowState(5);
    func_001004A0();
    func_002C92D0(fileMemoryCardRequestContext);
    return mcHandleDetectionResult;
}

s32 mnuSelectFileBranch(void) {
    if (D_00437D0C != 0) {
        D_00437D1C = 7;
        return fileBeginPromptDialog(fileBeginSlotResetPrompt, fileResetSelection, 1);
    }
    D_00437D1C = 7;
    return fileBeginPromptDialog(fileAbortSlotFlow, fileResetSelection, 1);
}

void *fileBeginSlotCreate(void) {
    char path[0x50];
    u32 ctx = fileMemoryCardRequestContext;
    s32 slot = fileReqGetSelectedSlot(ctx);
    u32 attr = fileReqGetSlotFlags(ctx, slot);
    u32 len;

    fileSetMenuFlowState(3);
    if ((attr & 1) == 0) {
        return fileScanSlotStates();
    }
    fileReqGetSlotCode();
    path[0] = '/';
    mcdFormatSaveSlotName(&path[1], slot);
    len = strlen(&path[1]);
    path[len + 1] = '/';
    memcpy(&path[len + 2], &path[1], len);
    path[len * 2 + 2] = 0;
    mcOpenFilePath(ctx, path, 1);
    return fileLoadMainBlobBegin;
}

void *fileLoadMainBlobBegin(void) {
    s32 status = mcPollCommandStatusWithResult(&fileSaveFileDescriptor);
    u32 size;

    if (status == 0) {
        return NULL;
    }
    size = fileMainBlobSize();
    fileSaveReadBufferResource = sdfAllocGeneralBlock(size);
    fileSaveReadBuffer = sdfResourceRetainAddress(fileSaveReadBufferResource);
    if (status == 1) {
        mcReadOpenFile(fileSaveFileDescriptor, fileSaveReadBuffer, size);
        return mcHandleSetupResult;
    }
    fileSetMenuFlowState(0);
    D_00437D3C = 3;
    return fileAbortSlotScanOnInput;
}

extern void *mcdHandleSaveSetupDone(void);
void *mcHandleSetupResult(void) {
    s32 status = mcPollCompletionStatus();

    if (status == 0) {
        return 0;
    }
    if (status == 1) {
        mcCloseOpenFile(fileSaveFileDescriptor);
        return (void *)mcdHandleSaveSetupDone;
    }
    sdfReleaseResourceAllocation(fileSaveReadBufferResource);
    fileSetMenuFlowState(0);
    D_00437D3C = 3;
    return (void *)fileAbortSlotScanOnInput;
}

extern s8 D_00437CD5;

void *mcdHandleSaveSetupDone(void) {
    s32 status = mcPollZeroCommandResult();

    if (status == 0) {
        return NULL;
    }
    if (status == 1) {
        fileReloadSaveBuffer();
        sdfReleaseResourceAllocation(fileSaveReadBufferResource);
        D_00437CD5 = 1;
        fileDestroyMenuTask();
        if (fileLoadStateChanged() == 0) {
            fileCacheSlotFlagsFromState();
        } else {
            fileRestoreSlotFlagsToState();
        }
        fileSetMenuFlowState(13);
        return fileSetMenuCallbackAndClearResult(-1);
    }
    sdfReleaseResourceAllocation(fileSaveReadBufferResource);
    fileSetMenuFlowState(0);
    D_00437D3C = 3;
    return (void *)fileAbortSlotScanOnInput;
}



extern s32 mcdContinueLoadSelection();

void *mcdAdvanceToLoadSelection(void) {
    fileSetMenuFlowState(13);
    return fileSetMenuCallbackAndClearResult((u32)mcdContinueLoadSelection);
}

extern s32 D_00437D00;
extern s32 D_00437D28;
extern void fileLoadSetMode(s8);
extern void fileSetMenuValueAndInitializeFlags(u32);
extern void fileResetLoadContextSlide(void);
extern void fileCopyRecordHeader(FileRecordHeader *, const FileRecordHeader *);

void *func_002CCAD0(void) {
    s32 oldSelection = D_00437D2C;
    s32 offset;
    s32 selection;
    s32 action;
    u32 flags;
    u32 slotState;

    D_00437D4C = 0;
    if (D_00437D14 == 0) {
        offset = oldSelection - D_00437D28;
        fileLoadMenuState.inputRepeatTimer++;
        fileLoadMenuState.inputRepeatTimer =
            fileLoadMenuState.inputRepeatTimer <= 0 ? 0 :
            fileLoadMenuState.inputRepeatTimer > 3 ? 3 : fileLoadMenuState.inputRepeatTimer;
        if (fileIsLoadedWithActiveFlow(((u8)D_0037F510[0x26] >> 1) & 1) != 0 &&
            fileLoadMenuState.inputRepeatTimer >= 3) {
            selection = D_00437D28;
            fileLoadMenuState.inputRepeatTimer = 0;
            if (selection > 0) {
                if (offset == 2) {
                    offset = 1;
                } else if (offset == 1) {
                    D_00437D28 = selection - 1;
                    fileLoadSetMode(2);
                    fileSetMenuValueAndInitializeFlags(0x80);
                }
            } else {
                offset -= offset > 0;
            }
        }
        if (fileIsLoadedWithActiveFlow(((u8)D_0037F510[0x27] >> 1) & 1) != 0 &&
            fileLoadMenuState.inputRepeatTimer >= 3) {
            selection = D_00437D28;
            fileLoadMenuState.inputRepeatTimer = 0;
            if (selection < 7) {
                if (offset == 0) {
                    offset = 1;
                } else if (offset == 1) {
                    D_00437D28 = selection + 1;
                    fileLoadSetMode(1);
                    fileSetMenuValueAndInitializeFlags(0x80);
                }
            } else {
                offset += offset < 2;
            }
        }
        D_00437D2C = D_00437D28 + offset;
        if (oldSelection != D_00437D2C) {
            sndSetSequenceVolumePan(0, 0x7F, 0x3F);
        }
        if (D_00437D2C < oldSelection) {
            D_00437D00 = -8;
        } else if (oldSelection < D_00437D2C) {
            D_00437D00 = 8;
        }
        if (fileIsLoadedAndConditionTrue(D_0037F510[0x21] < 0) != 0) {
            D_00437D14 = 1;
        }
        if (fileIsLoadedAndConditionTrue(D_0037F510[0x23] < 0) != 0) {
            D_00437D14 = 2;
        }
    }
    if (fileReqPoll() != 0) {
        flags = fileReqGetStatus(fileMemoryCardRequestContext);
        action = D_00437D14;
        if (flags == 0) {
            return fileBeginSlotMetadataRefresh();
        }
        if (action == 0) {
            fileReqBegin(fileMemoryCardRequestContext);
            return NULL;
        }
        if (action == 1) {
            fileSetMenuFlowState(0);
            if (D_00437D30 == 0) {
                fileReqSetSelectedSlot(fileMemoryCardRequestContext, D_00437D2C);
                flags = fileReqGetSlotFlags(fileMemoryCardRequestContext, D_00437D2C);
                if ((flags & 0xB) == 0xB) {
                    sndSetSequenceVolumePan(8, 0x7F, 0x3F);
                    if (D_00437D0C != 0) {
                        D_00437D08 = action;
                        D_00437D1C = action;
                        return (void *)fileBeginPromptDialog(fileBeginDirectoryScan, fileScanSlotStates, 1);
                    }
                    D_00437D08 = action;
                    D_00437D1C = action;
                    return (void *)fileBeginPromptDialog(mcPrepareDirectory, fileScanSlotStates, 1);
                }
                slotState = flags & 0xA;
                if (slotState == 2) {
                    sndSetSequenceVolumePan(8, 0x7F, 0x3F);
                    D_00437D08 = slotState;
                    return mcPrepareDirectory();
                }
                sndSetSequenceVolumePan(8, 0x7F, 0x3F);
                D_00437D08 = 0;
                if (fileIsCardSpaceAboveMinimum() != 0) {
                    return mcPrepareDirectory();
                }
                fileSetMenuFlowState(0);
                D_00437D3C = 5;
                D_00437D40 = 0;
                fileReqBegin(fileMemoryCardRequestContext);
                return filePollSlotScanOrReset;
            }
            fileReqSetSelectedSlot(fileMemoryCardRequestContext, D_00437D2C);
            flags = fileReqGetSlotFlags(fileMemoryCardRequestContext, D_00437D2C);
            if ((flags & 1) != 0) {
                sndSetSequenceVolumePan(8, 0x7F, 0x3F);
                if (mcdOriginalTitleFileMode != 0 &&
                    D_004580C0[D_00437D2C].status == 0) {
                    fileSetMenuFlowState(0);
                    D_00437D3C = 8;
                    D_00437D40 = 0;
                    fileReqBegin(fileMemoryCardRequestContext);
                    return filePollSlotScanOrReset;
                }
                D_00437D1C = 3;
                if (mcdOriginalTitleFileMode == 0) {
                    return (void *)fileBeginPromptDialog(fileBeginSlotCreate, fileScanSlotStates, 1);
                }
                fileCopyRecordHeader((FileRecordHeader *)fileLoadSelectionWork,
                    (const FileRecordHeader *)(&D_004580C0[D_00437D2C]));
                return (void *)fileBeginPromptDialog(mcdAdvanceToLoadSelection, fileScanSlotStates, 1);
            }
            sndSetSequenceVolumePan(0xA, 0x7F, 0x3F);
            return fileScanSlotStates();
        }
        if (action == 2) {
            sndSetSequenceVolumePan(0xA, 0x7F, 0x3F);
            fileResetLoadContextSlide();
            if (D_00437D0C != 0) {
                return fileBeginSlotResetPrompt();
            }
            return fileResolveAbortSlotFlow();
        }
    }
    return NULL;
}

s32 func_002CD028(s32 work) {
    char text[64];
    s32 row;
    s32 rowBase;
    s32 rowCount;
    s32 slotBase;
    s32 slot;
    s32 y;
    u32 titleColor;
    u32 detailColor;
    u32 alpha;
    u32 baseColor;
    u32 partyColor;
    s32 trail;
    s32 trailAlpha;
    s32 partyIndex;
    s32 portrait;
    s32 ticks;
    s32 hours;
    s32 minutes;
    s32 seconds;
    FileRecordHeader *preview;
    f32 targetY;
    f32 delta;
    f32 wave;

    if (D_00437CEC == 1) {
        return 0;
    }
    if (D_00437D00 > 0) {
        D_00437D00--;
    }
    if (D_00437D00 < 0) {
        D_00437D00++;
    }
    if (D_00437D30 == 0) {
        func_00108EC0(0x56, 0x33, 0x137, 0x17, 5, 0x1C4, 0x137, 0x17, 0x80808080, 0x80808080, 0x80808080, 0x80808080, D_00437D74);
    } else {
        func_00108EC0(0x56, 0x33, 0x137, 0x17, 5, 0x1DD, 0x137, 0x17, 0x80808080, 0x80808080, 0x80808080, 0x80808080, D_00437D74);
    }
    evtSetDrawSurfaceIndex(0x55);
    evtSubmitPrimaryGsTest(1, 0, 0x80, 2, 0, 0, 1, 1);
    evtSubmitPrimaryAlphaBlendMode(0);
    evtSubmitDefaultDepthGradientRect(0, 0, 0x200, 0x1C0, 0, 0, 0, 0);
    evtSubmitPrimaryGsTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    evtSetDrawSurfaceIndex(0x53);
    evtSubmitPrimaryAlphaBlendMode(0);
    evtSubmitPrimaryGsTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    /* Retail computes the clamped slide delta but snaps to the target. */
    targetY = D_00437D28 * -110 + 104;
    delta = targetY - D_00437D24;
    delta /= 3.0f;
    if (delta < 0.0f && delta > -16.0f) {
        delta = -16.0f;
    }
    if (delta > 0.0f && delta < 16.0f) {
        delta = 16.0f;
    }
    D_00437D24 = targetY;
    fileCursorStepUp();
    fileFadeStepDown();
    if (D_00437D38 == 0) {
        fileCursorPulseUpdate();
        func_002CFF38(0x25, 0x1F, 0x53);
    }
    fileLoadCtxSlideUpdate();
    if (fileLoadMenuState.unk10 == 0) {
        rowCount = 3;
        rowBase = D_00437D28;
        slotBase = D_00437D28;
    } else if (fileLoadMenuState.unk10 == 1) {
        rowCount = 4;
        rowBase = D_00437D28;
        slotBase = D_00437D28 - 1;
        if (slotBase < 0) {
            slotBase = 0;
        }
    } else {
        rowCount = 4;
        rowBase = D_00437D28 - 1;
        slotBase = D_00437D28;
    }
    for (row = 0; row < rowCount; row++) {
        slot = slotBase + row;
        y = (s32)D_00437D24 + (rowBase + row) * 110 + fileLoadMenuState.unk14;
        if (D_00437D2C == slot) {
            titleColor = 0xA09DC380;
            detailColor = 0xA09DC380;
        } else {
            titleColor = 0x605D8340;
            detailColor = 0x605D8340;
        }
        if (mcdOriginalTitleFileMode != 0) {
            if (D_004580C0[slot].status == 0) {
                titleColor = 0x605D8340;
                detailColor = 0x605D8340;
            } else {
                titleColor = 0xA09DC380;
                detailColor = 0xA09DC380;
            }
        }
        if (fileLoadMenuState.unk10 != 0) {
            if (row == 0) {
                alpha = fileLoadMenuState.unk18;
                titleColor = func_002CF978(titleColor, alpha, 0);
                detailColor = func_002CF978(detailColor, fileLoadMenuState.unk18, 0);
            } else if (row == 3) {
                alpha = fileLoadMenuState.unk1C;
                titleColor = func_002CF978(titleColor, alpha, 0);
                detailColor = func_002CF978(detailColor, fileLoadMenuState.unk1C, 0);
            } else {
                alpha = fileLoadMenuState.unkC;
                titleColor = func_002CF978(titleColor, alpha, 0);
                detailColor = func_002CF978(detailColor, fileLoadMenuState.unkC, 0);
            }
        } else {
            alpha = fileLoadMenuState.unkC;
            titleColor = func_002CF978(titleColor, alpha, 0);
            detailColor = func_002CF978(detailColor, fileLoadMenuState.unkC, 0);
        }
        baseColor = (alpha << 24) | 0x808080;
        func_00108EC0(0x1F, y, 0x1BB, 0x68, 3, 3, 0x1BB, 0x68, baseColor, baseColor, baseColor, baseColor, D_00437D5C);
        if (D_00458080[slot] == 0) {
            func_00108EC0(0x22, y + 0x27, 0x78, 0x40, 1, 0, 0x78, 0x40, baseColor, baseColor, baseColor, baseColor, D_00437D64);
        } else if (D_00458080[slot] == 2) {
            func_00108EC0(0x22, y + 0x27, 0x78, 0x40, 1, 0, 0x78, 0x40, baseColor, baseColor, baseColor, baseColor, D_00437D6C);
        } else if (D_004580C0[slot].status > 0) {
            func_00108EC0(0x22, y + 0x27, 0x78, 0x40, 1, 0, 0x78, 0x40, baseColor, baseColor, baseColor, baseColor, D_00437D68);
        } else if (mcdOriginalTitleFileMode != 0 && D_004580C0[slot].status == 0) {
            mcdCreateConfiguredDrawHandle(0x40, y + 0x20, detailColor, D_003E8BD0);
        }
        if (slot != 10) {
            fileDrawSlotIcon(slot + 1, 0x5A, y + 4, alpha);
            if (D_00437D2C == slot) {
                trailAlpha = 16;
                if (D_00437D00 > 0) {
                    for (trail = 0; trail < D_00437D00; trail++, trailAlpha -= 2) {
                        func_00108EC0(0x1F, y - trail * 4, 0x1BB, trail * 4 + 16, 3, 3, 0x1BB, trail * 4 + 16, (trailAlpha << 24) | 0x808080, (trailAlpha << 24) | 0x808080, (trailAlpha << 24) | 0x808080, (trailAlpha << 24) | 0x808080, D_00437D60);
                    }
                } else if (D_00437D00 < 0) {
                    trailAlpha = 32;
                    for (trail = 0; trail < -D_00437D00; trail++, trailAlpha -= 4) {
                        func_00108EC0(0x1F, y + 0x58, 0x1BB, trail * 4 + 16, 3, 0x5B - trail * 4, 0x1BB, trail * 4 + 16, (trailAlpha << 24) | 0x808080, (trailAlpha << 24) | 0x808080, (trailAlpha << 24) | 0x808080, (trailAlpha << 24) | 0x808080, D_00437D60);
                    }
                }
            }
            if (D_00458080[slot] != 1) {
                if (D_00437D2C == slot) {
                    u32 highlightColor = ((D_003E8008[0] + 0x80) << 24) | 0x808080;
                    func_00108EC0(0x1F, y, 0x1BB, 0x68, 3, 3, 0x1BB, 0x68, highlightColor, highlightColor, highlightColor, highlightColor, D_00437D60);
                }
            } else {
                u32 bannerColor;

                if (mcdOriginalTitleFileMode != 0) {
                    if (D_004580C0[slot].newCycle != 0) {
                        mcdCreateConfiguredDrawHandle(0x40, y + 0x20, titleColor, D_003E8BB0);
                    } else if (D_004580C0[slot].status != 0) {
                        mcdCreateConfiguredDrawHandle(0x40, y + 0x20, titleColor, D_003E8B98);
                    }
                } else if (D_004580C0[slot].newCycle != 0) {
                    mcdCreateConfiguredDrawHandle(0x40, y + 0x20, detailColor, D_003E8658);
                }
                bannerColor = (alpha << 24) | 0x808080;
                func_00108EC0(0x88, y + 4, 0x137, 0x1F, 4, 0x1C5, 0x137, 0x1F, bannerColor, bannerColor, bannerColor, bannerColor, D_00437D78);
                preview = &D_004580C0[slot];
                if (preview->newCycle == 0) {
                    if (mcdOriginalTitleFileMode == 0) {
                        if (preview->mapGroup == 0) {
                            mcdCreateConfiguredDrawHandle(0xF0, y, detailColor, D_003A41A8[preview->mapIndex]);
                        } else {
                            mcdCreateConfiguredDrawHandle(0xF0, y, detailColor, D_003A47E8[preview->mapIndex]);
                        }
                    } else {
                        if (preview->mapGroup == 0) {
                            mcdCreateConfiguredDrawHandle(0xF0, y, detailColor, D_00437D54[preview->mapIndex]);
                        } else {
                            mcdCreateConfiguredDrawHandle(0xF0, y, detailColor, D_00437D58[preview->mapIndex]);
                        }
                    }
                }
                ticks = D_004580C0[slot].playTicks;
                if (ticks > 0x066FEBF8) {
                    ticks = 0x066FEBF8;
                }
                hours = ticks / 108000;
                minutes = (ticks % 108000) / 1800;
                seconds = (ticks % 1800) / 30;
                func_0035C860(text, D_00437D98, hours);
                fileDrawMenuImageAtPoint(0x187, y + 0x15, titleColor, text);
                func_0035C860(text, D_00437DA0, minutes);
                fileDrawMenuImageAtPoint(0x1A7, y + 0x15, titleColor, text);
                func_0035C860(text, D_00437DA0, seconds);
                fileDrawMenuImageAtPoint(0x1BD, y + 0x15, titleColor, text);
                if (D_00437D2C == slot) {
                    u32 highlightColor = ((D_003E8008[0] + 0x80) << 24) | 0x808080;
                    func_00108EC0(0x1F, y, 0x1BB, 0x68, 3, 3, 0x1BB, 0x68, highlightColor, highlightColor, highlightColor, highlightColor, D_00437D60);
                }
                if (D_004580C0[slot].newCycle == 0) {
                    if (mcdOriginalTitleFileMode != 0) {
                        if (D_004580C0[slot].status != 0) {
                            partyColor = (alpha << 24) | 0x808080;
                        } else {
                            partyColor = (((alpha >> 1) + (alpha >> 2)) << 24) | 0x282828;
                        }
                    } else {
                        if (D_00437D2C == slot) {
                            partyColor = (alpha << 24) | 0x808080;
                        } else {
                            partyColor = (((alpha >> 1) + (alpha >> 2)) << 24) | 0x282828;
                        }
                    }
                    for (partyIndex = 0; partyIndex < 5; partyIndex++) {
                        if (D_004580C0[slot].party[partyIndex] != 0) {
                            portrait = 0;
                            switch (D_004580C0[slot].party[partyIndex]) {
                            case 1: portrait = 0; break;
                            case 3: portrait = 1; break;
                            case 4: portrait = 2; break;
                            case 5: portrait = 3; break;
                            case 6: portrait = 4; break;
                            case 7: portrait = 5; break;
                            case 2: portrait = 6; break;
                            case 8: portrait = 7; break;
                            }
                            func_00108EC0(partyIndex * 60 + 0x80, y + 0x23, 0x32, 0x3E, 1, 1, 0x32, 0x3E, partyColor, partyColor, partyColor, partyColor, D_003E7FA8[portrait]);
                            func_0035C860(text, D_00437DA8, D_004580C0[slot].levels[partyIndex]);
                            fileDrawMenuImageAtPoint(partyIndex * 60 + 0x98, y + 0x51, titleColor, text);
                        }
                    }
                }
                if (D_004580C0[slot].header2C & 0x80000000) {
                    u32 badgeColor = (alpha << 24) | 0x808080;
                    func_00108EC0(0x1B, y + 0x4C, 0x57, 0x23, 2, 0x5D, 0x57, 0x23, badgeColor, badgeColor, badgeColor, badgeColor, D_00437D70);
                }
            }
        }
    }
    evtSubmitPrimaryAlphaBlendMode(1);
    D_003E7FF8.angle = (D_003E7FF8.angle + 10) % 360;
    if (D_00437D28 > 0) {
        u32 arrowColor;

        func_00108EC0(0xF8, 0x50, 0xF, 0x22, 0x35, 2, 0xF, 0x22, 0x80808080, 0x80808080, 0x80808080, 0x80808080, D_00437D70);
        wave = sdfSinPoly((f32)((D_003E7FF8.angle + 90) % 360) / 180.0f * 3.1415899f);
        D_003E7FF8.upAlpha = (s32)((wave + 1.0f) * 0.5f * 176.0f + 16.0f);
        arrowColor = (D_003E7FF8.upAlpha << 24) | 0x808080;
        func_00108EC0(0xF8, 0x50, 0xF, 0x22, 0x35, 2, 0xF, 0x22, arrowColor, arrowColor, arrowColor, arrowColor, D_00437D70);
    }
    if (D_00437D28 < 7) {
        u32 arrowColor;

        func_00108EC0(0xF8, 0x18B, 0xF, 0x22, 0x47, 2, 0xF, 0x22, 0x80808080, 0x80808080, 0x80808080, 0x80808080, D_00437D70);
        wave = sdfSinPoly((f32)((D_003E7FF8.angle + 90) % 360) / 180.0f * 3.1415899f);
        D_003E7FF8.downAlpha = (s32)((wave + 1.0f) * 0.5f * 176.0f + 16.0f);
        arrowColor = (D_003E7FF8.downAlpha << 24) | 0x808080;
        func_00108EC0(0xF8, 0x18B, 0xF, 0x22, 0x47, 2, 0xF, 0x22, arrowColor, arrowColor, arrowColor, arrowColor, D_00437D70);
    }
    evtSubmitPrimaryAlphaBlendMode(0);
    evtSubmitPrimaryGsTest(1, 5, 0x80, 3, 0, 0, 1, 1);
    evtSetDrawSurfaceIndex(0x53);
    evtSubmitPrimaryAlphaBlendMode(0);
    evtSubmitPrimaryGsTest(1, 5, 0x80, 3, 0, 0, 1, 2);
    return 0;
}


void *fileRunMenuState(s32 arg) {
    void *next;
    u32 job;
    void *(*cur)(s32);
    D_00437CFC++;
    fileDrawMenuFrame(arg);
    next = fileMenuStateHandler(arg);
    if (next == (void *)-1) {
        return next;
    }
    job = fileSaveIconRequest;
    cur = fileMenuStateHandler;
    if (next != NULL) {
        cur = next;
    }
    fileMenuStateHandler = cur;
    if (job != 0 && fileIsRequestReadyInCurrentMode(job, next) != 0) {
        fileSaveIconRequest = 0;
        D_00439030 = fileGetResourceHandle(job);
        D_00439034 = fileGetLoadedDataAddress(job);
        D_00439038 = fileGetResourceSize(job);
        filePollEntryCleanup(job);
    }
    return NULL;
}


s32 fileDrawMenuFrame(s32 task) {
    s32 fade;
    s32 alpha;

    evtSetDrawSurfaceIndex(0x52);
    evtSubmitPrimaryAlphaBlendMode(0);
    evtSubmitPrimaryGsTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    if (D_00437CF4 != 0) {
        func_00108EC0(0, 0, 0x200, 0x1C0, 0, 0, 0x200, 0x1C0, 0x80808080, 0x80808080, 0x80808080, 0x80808080,
                      D_00437D74);
        func_00108EC0(0, 0, 0x200, 0x1C0, 0, 0, 0x200, 0x1C0, 0x80808080, 0x80808080, 0x80808080, 0x80808080,
                      D_00437D78);
    }
    if (D_00437CF8 != 0) {
        func_002CD028(task);
    }
    fade = D_00437D4C;
    if (fade > 0) {
        if (fade < 4) {
            alpha = fade * 20;
        } else {
            alpha = 0x50;
        }
        func_00108EC0(0, 0, 0x200, 0x1C0, 0, 0, 0x200, 0x1C0, (alpha * 0x80 / 100 << 24) | 0x808080,
                      (alpha * 0x80 / 100 << 24) | 0x808080, (alpha * 0x80 / 100 << 24) | 0x808080,
                      (alpha * 0x80 / 100 << 24) | 0x808080, D_00437D74);
    }
    func_002CA1F0();
    func_002CEEC0();
    func_002C9CF8();
    evtSubmitPrimaryAlphaBlendMode(0);
    evtSubmitPrimaryGsTest(1, 5, 0x80, 3, 0, 0, 1, 2);
    return 0;
}

void fileResetMenuFlowState(void) {
    fileSetMenuFlowState(0);
    D_00437D04 = 0;
    fileWaitTicksRemaining = 0;
    D_00437D18 = 0xffffffff;
    fileSelectionPending = 0;
    D_00437D3C = 0;
    D_00437D40 = 0;
    D_00437CF8 = 0;
    D_00437D08 = 0;
    fileSlotSelectionPollCount = 0;
    D_00437CFC = 0;
    D_00437CEC = 0;
    D_00437D14 = 0;
    D_00437D1C = 0;
    D_00437D20 = 0;
    D_00437D44 = 0;
    D_00437D48 = 0;
    fileClearAllSlotFlags();
}

INCLUDE_ASM(const s32, "game/code_002C96D0", func_002CE208);

void func_002CE738(void) {
    fileReleaseMenuResources();
}

void func_002CE750(void) {
}

void fileReleaseMenuResources(void) {
    s32 *slot;
    s32 i;
    s32 world;
    s32 handle;

    if (D_00437CE4 != 0) {
        D_00437CE4 = 0;
        slot = D_003E7FA8;
        i = 7;
        do {
            handle = *slot;
            i--;
            if (handle != 0) {
                sdfTexReleaseReferenceViaHandler(handle);
                *slot = 0;
            }
            slot++;
        } while (i >= 0);
        if (D_00437D78 != 0) {
            sdfTexReleaseReferenceViaHandler(D_00437D78);
            D_00437D78 = 0;
        }
        if (D_00437D74 != 0) {
            sdfTexReleaseReferenceViaHandler(D_00437D74);
            D_00437D74 = 0;
        }
        if (D_00437D70 != 0) {
            sdfTexReleaseReferenceViaHandler(D_00437D70);
            D_00437D70 = 0;
        }
        if (D_00437D6C != 0) {
            sdfTexReleaseReferenceViaHandler(D_00437D6C);
            D_00437D6C = 0;
        }
        if (D_00437D68 != 0) {
            sdfTexReleaseReferenceViaHandler(D_00437D68);
            D_00437D68 = 0;
        }
        if (D_00437D64 != 0) {
            sdfTexReleaseReferenceViaHandler(D_00437D64);
            D_00437D64 = 0;
        }
        if (D_00437D60 != 0) {
            sdfTexReleaseReferenceViaHandler(D_00437D60);
            D_00437D60 = 0;
        }
        if (D_00437D5C != 0) {
            sdfTexReleaseReferenceViaHandler(D_00437D5C);
            D_00437D5C = 0;
        }
        if (D_00437D54 != 0) {
            sdfFreeMemoryFromEitherHeap(D_00437D54);
            D_00437D54 = 0;
        }
        if (D_00437D58 != 0) {
            sdfFreeMemoryFromEitherHeap(D_00437D58);
            D_00437D58 = 0;
        }
        world = dds3GetWorldObject();
        if (world != 0) {
            dds3SetWorldObjectDataValue(world, 1);
        }
        if (fileSaveIconRequest != 0) {
            fileWaitReady(fileSaveIconRequest);
            D_00439030 = fileGetResourceHandle(fileSaveIconRequest);
            filePollEntryCleanup(fileSaveIconRequest);
            fileSaveIconRequest = 0;
        }
        sdfReleaseMemorySlot(&D_00439030);
        kwlnTaskDestroyWithHierarchyByName(D_0042B720, 1);
        func_00100498();
    }
}

u32 func_002CE920(void) {
    return 1;
}

s32 fileMenuTaskExists(void) {
    return kwlnTaskGetTaskByName(D_0042B720) != NULL;
}

u32 fileGetSelectionPendingFlag(void) {
    return fileSelectionPending;
}

INCLUDE_ASM(const s32, "game/code_002C96D0", func_002CE958);

extern u32 D_00439024;
extern u32 D_00439028;
extern void *func_002CE958(void);

s32 fileBeginPromptDialog(void *start, void *finish, s32 mode) {
    D_00439024 = (u32)start;
    D_00439028 = (u32)finish;
    D_00437D18 = (u32)mode;
    D_00437D20 = 0;
    if (D_00437D1C != 4 && D_00437D1C != 8) {
        fileReqBegin(fileMemoryCardRequestContext);
    }
    return (s32)func_002CE958;
}

INCLUDE_ASM(const s32, "game/code_002C96D0", func_002CEEC0);

u32 fileGetLoadSelectionState(void) {
    return D_00437D04;
}

typedef struct FilePreviewWork {
    u8 pad0[0x110F0];
    s16 previewX;
    s16 previewY;
} FilePreviewWork;

void fileSetPreviewLocation(s16 x, s16 y) {
    FilePreviewWork *work = (FilePreviewWork *)datGameState;

    work->previewX = x;
    work->previewY = y;
}

void fileSetMenuValueAndInitializeFlags(u32 value) {
    D_003E8008[0] = value;
    fileLoadMenuState.unkC = 0x80;
    fileLoadMenuState.unk4 = 0;
}

INCLUDE_ASM(const s32, "game/code_002C96D0", func_002CF978);

void fileCursorStepUp(void) {
    fileLoadMenuState.unk4 += 1;
    fileLoadMenuState.unk4 = fileLoadMenuState.unk4 <= 0 ? 0 : fileLoadMenuState.unk4 > 12 ? 12 : fileLoadMenuState.unk4;
    if (fileLoadMenuState.unk4 >= 7) {
        fileLoadMenuState.unkC += 0x30;
        fileLoadMenuState.unkC = fileLoadMenuState.unkC <= 0 ? 0 : fileLoadMenuState.unkC > 0x80 ? 0x80 : fileLoadMenuState.unkC;
    }
}

void fileFadeStepDown(void) {
    D_003E8008[0] -= 0x10;
    D_003E8008[0] = D_003E8008[0] <= 0 ? 0 : D_003E8008[0] > 0x80 ? 0x80 : D_003E8008[0];
}

INCLUDE_RODATA(const s32, "game/code_002C96D0", D_0042B720);

INCLUDE_RODATA(const s32, "game/code_002C96D0", jtbl_0042B730);

INCLUDE_RODATA(const s32, "game/code_002C96D0", D_0042B770);

void fileDrawSlotIcon(s32 index, s32 x, s32 y, s32 alpha) {
    s32 uv[21][2] = {
        {2, 2},   {2, 2},   {2, 20},  {2, 38},  {2, 56},  {2, 74},  {26, 2},
        {26, 20}, {26, 38}, {26, 56}, {26, 74}, {26, 74}, {26, 74}, {26, 74},
        {26, 74}, {26, 74}, {26, 74}, {26, 74}, {26, 74}, {26, 74}, {26, 74},
    };

    evtSetDrawSurfaceIndex(0x53);
    evtSubmitPrimaryAlphaBlendMode(0);
    evtSubmitPrimaryGsTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    func_00108EC0(x, y, 0x16, 0x10, uv[index][0], uv[index][1], 0x16, 0x10, (alpha << 24) | 0x808080,
                  (alpha << 24) | 0x808080, (alpha << 24) | 0x808080, (alpha << 24) | 0x808080, D_00437D70);
}

void fileCursorOffsetLookup(s32 dir, s32 step, s32 *outX, s32 *outY) {
    s32 offsets[3][5][2] = {
        {{0, 0}, {2, 1}, {4, 2}, {6, 4}, {8, 6}},
        {{0, 0}, {-2, 1}, {-4, 2}, {-6, 4}, {-8, 6}},
        {{0, 0}, {0, -2}, {0, -4}, {0, -6}, {0, -8}},
    };

    *outX = offsets[dir][step][0];
    *outY = offsets[dir][step][1];
}

/* Init record at fileCursorPulseState. */
typedef struct Init374E0 {
    s8 mode[3];      /* 0x00 */
    u8 pad3;         /* 0x03 */
    s32 pos[3][2];   /* 0x04 */
    s32 counter[3];  /* 0x1C */
    u8 pad28[0xC];   /* 0x28 */
    s32 alpha[3];    /* 0x34 */
} Init374E0;

extern Init374E0 fileCursorPulseState;
extern s32 D_003E7FE4[];

void fileInitCursorPulse(void) {
    fileCursorPulseState.mode[1] = 0;
    D_003E7FE4[0] = 0x80;
    fileCursorPulseState.mode[0] = 1;
    fileCursorPulseState.mode[2] = 2;
    fileCursorPulseState.alpha[0] = 0x80;
    fileCursorPulseState.alpha[1] = 0x80;
    fileCursorPulseState.alpha[2] = 0x80;
}

void fileCursorPulseUpdate(void) {
    s32 i;

    for (i = 0; i < 3; i++) {
        s32 mode = fileCursorPulseState.mode[i];

        if (mode < 3) {
            if (mode >= 0) {
                switch (mode) {
                case 0:
                    fileCursorPulseState.alpha[i] -= 12;
                    fileCursorPulseState.alpha[i] = fileCursorPulseState.alpha[i] <= 0x40 ? 0x40 : fileCursorPulseState.alpha[i] > 0x80 ? 0x80 : fileCursorPulseState.alpha[i];
                    break;
                case 1:
                    fileCursorPulseState.alpha[i] -= 28;
                    fileCursorPulseState.alpha[i] = fileCursorPulseState.alpha[i] <= 0 ? 0 : fileCursorPulseState.alpha[i] > 0x80 ? 0x80 : fileCursorPulseState.alpha[i];
                    break;
                }
                fileCursorPulseState.counter[i] += 1;
                fileCursorPulseState.counter[i] = fileCursorPulseState.counter[i] <= 0 ? 0 : fileCursorPulseState.counter[i] > 6 ? 6 : fileCursorPulseState.counter[i];
                if (fileCursorPulseState.counter[i] >= 6) {
                    fileCursorPulseState.mode[i]++;
                    fileCursorPulseState.alpha[i] = 0x80;
                    if (fileCursorPulseState.mode[i] >= 3) {
                        fileCursorPulseState.mode[i] = 0;
                    }
                    fileCursorPulseState.counter[i] = 0;
                }
                fileCursorOffsetLookup(i, 0, &fileCursorPulseState.pos[i][0], &fileCursorPulseState.pos[i][1]);
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_002C96D0", func_002CFF38);

void fileLoadSetMode(s8 mode) {
    fileLoadMenuState.unk14 = 0;
    fileLoadMenuState.unk10 = mode;
    if (mode == 1) {
        fileLoadMenuState.unk1C = 0;
        fileLoadMenuState.unk18 = 0x74;
    } else {
        fileLoadMenuState.unk18 = 0;
        fileLoadMenuState.unk1C = 0x74;
    }
}

void fileResetLoadContextSlide(void) {
    fileLoadMenuState.unk14 = 0;
    fileLoadMenuState.unk10 = 0;
    fileLoadMenuState.unk4 = 0;
}

void fileLoadCtxSlideUpdate(void) {
    switch (fileLoadMenuState.unk10) {
    case 1:
        fileLoadMenuState.unk14 -= 0x14;
        fileLoadMenuState.unk14 = fileLoadMenuState.unk14 <= -0x6E ? -0x6E : fileLoadMenuState.unk14 > 0 ? 0 : fileLoadMenuState.unk14;
        if (fileLoadMenuState.unk14 <= -0x6E) {
            fileLoadMenuState.unk14 = 0;
            fileLoadMenuState.unk10 = 0;
        }
        fileLoadMenuState.unk18 -= 0x20;
        fileLoadMenuState.unk18 = fileLoadMenuState.unk18 <= 0 ? 0 : fileLoadMenuState.unk18 > 0x80 ? 0x80 : fileLoadMenuState.unk18;
        fileLoadMenuState.unk1C += 0x10;
        fileLoadMenuState.unk1C = fileLoadMenuState.unk1C <= 0 ? 0 : fileLoadMenuState.unk1C > 0x80 ? 0x80 : fileLoadMenuState.unk1C;
        break;
    case 2:
        fileLoadMenuState.unk14 += 0x14;
        fileLoadMenuState.unk14 = fileLoadMenuState.unk14 <= 0 ? 0 : fileLoadMenuState.unk14 > 0x6E ? 0x6E : fileLoadMenuState.unk14;
        if (fileLoadMenuState.unk14 >= 0x6E) {
            fileLoadMenuState.unk14 = 0;
            fileLoadMenuState.unk10 = 0;
        }
        fileLoadMenuState.unk18 += 0x10;
        fileLoadMenuState.unk18 = fileLoadMenuState.unk18 <= 0 ? 0 : fileLoadMenuState.unk18 > 0x80 ? 0x80 : fileLoadMenuState.unk18;
        fileLoadMenuState.unk1C -= 0x20;
        fileLoadMenuState.unk1C = fileLoadMenuState.unk1C <= 0 ? 0 : fileLoadMenuState.unk1C > 0x80 ? 0x80 : fileLoadMenuState.unk1C;
        break;
    }
}

void mcdFinishFileDetectionWithAudio(void) {
    if (fileIsLoadedAndConditionTrue((u32)D_0037F510[0x21] >> 31) ||
        fileIsLoadedAndConditionTrue((u32)D_0037F510[0x23] >> 31)) {
        fileSetMenuFlowState(0);
        sndSetSequenceVolumePan(8, 0x7f, 0x3f);
        ((void (*)(void))D_00439020)();
    }
}

void *fileCreateDetectionAudioCallback(u32 callback) {
    D_00439020 = callback;
    D_00437D40 = 0;
    return mcdFinishFileDetectionWithAudio;
}

extern u32 D_0043902C;

void *fileFourWayDialogPoll(void) {
    if (fileIsLoadStepComplete() == 0) {
        return 0;
    }
    if (D_0037F510[0x27] < 0) {
        if (D_00437D18 != 1) {
            sndSetSequenceVolumePan(0, 0x7f, 0x3f);
        }
        D_00437D18 = 1;
    } else if (D_0037F510[0x26] < 0) {
        if (D_00437D18 != 0) {
            sndSetSequenceVolumePan(0, 0x7f, 0x3f);
        }
        D_00437D18 = 0;
    } else if (D_0037F510[0x21] < 0) {
        sndSetSequenceVolumePan(8, 0x7f, 0x3f);
        if (D_00437D18 == 0) {
            return ((void *(*)(void))D_00439024)();
        }
        return ((void *(*)(void))D_00439028)();
    } else if (D_0037F510[0x23] < 0) {
        if (D_0043902C != 0) {
            sndSetSequenceVolumePan(0xA, 0x7f, 0x3f);
            return ((void *(*)(void))D_0043902C)();
        }
    }
    return 0;
}

extern u32 D_00439024;
extern u32 D_00439028;
extern u32 D_0043902C;
extern void *fileFourWayDialogPoll(void);

void *fileBeginFourWayDialog(u32 ready, u32 completed, u32 cancelled, u32 state) {
    D_00439024 = ready;
    D_00439028 = completed;
    D_0043902C = cancelled;
    D_00437D18 = state;
    D_00437D20 = 0;
    return fileFourWayDialogPoll;
}

u32 func_002D0490(void) {
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_002C96D0", D_0042B8F8);

INCLUDE_ASM(const s32, "game/code_002C96D0", func_002D0498);

extern s32 func_002D0498();

void *fileBeginFadeAndConfirmSound(void) {
    kwlnFadeInStart(0, 0, 0, 8);
    func_00342580(0x310000);
    return func_002D0498;
}

typedef struct MenuWork {
    FileRecordHeader header;
    u8 unk30;
    u8 unk31;
    u16 unk32;
    u16 startBranchFlag; /* 0x34: selects one of two initial menu flows */
    u16 unk36;
    u16 unk38;
    u16 unk3A;
    u32 unk3C;
} MenuWork;

void func_002D06B0(void) {
    ((MenuWork *)fileLoadSelectionWork)->unk31 = 0;
    fileBeginFadeAndConfirmSound();
}

void func_002D06D0(void) {
    ((MenuWork *)fileLoadSelectionWork)->unk31 = 1;
    fileBeginFadeAndConfirmSound();
}

void *fileBeginLoadBranchDialog(void) {
    fileSetMenuFlowState(0);
    D_00437D1C = 12;
    return fileBeginFourWayDialog((u32)func_002D06B0, (u32)func_002D06D0, 0, 0);
}

void *fileStartLoadDetectionAfterBranchDialog(void) {
    D_00437D18 = -1;
    D_00437D1C = 0;
    fileSetMenuFlowState(0x18);
    ((MenuWork *)fileLoadSelectionWork)->unk30 = 1;
    return fileCreateDetectionAudioCallback((u32)fileBeginLoadBranchDialog);
}

void *fileRestartSelectionFlow(void) {
    fileResetMenuFlowState();
    return func_002CAED0;
}

void fileBeginLoadConfirmationDialog(void) {
    fileSetMenuFlowState(0);
    D_00437D1C = 11;
    fileBeginFourWayDialog((u32)fileStartLoadDetectionAfterBranchDialog, (u32)fileRestartSelectionFlow, (u32)fileRestartSelectionFlow, 0);
}

extern void *fileNextMenuFlowState(void);

void *fileWaitForLoadStepBeforeDetection(void) {
    if (fileIsLoadStepComplete() != 0) {
        return fileCreateDetectionAudioCallback((u32)fileNextMenuFlowState);
    }
    return 0;
}

typedef struct FileFlowEntry {
    u16 state;
    u16 mask;
} FileFlowEntry;

extern FileFlowEntry D_003E7FC8[];

void *fileNextMenuFlowState(void) {
    while (((MenuWork *)fileLoadSelectionWork)->unk36 < 4) {
        MenuWork *work = (MenuWork *)fileLoadSelectionWork;
        u32 index = work->unk36;

        work->unk36 = index + 1;
        if (D_003E7FC8[index].mask & work->unk32) {
            fileSetMenuFlowState(D_003E7FC8[index].state);
            return fileWaitForLoadStepBeforeDetection;
        }
    }
    return fileBeginLoadConfirmationDialog;
}

extern u32 kwlnDrawControlFlags;

s32 fileResetPendingRequest(void) {
    s32 mode;
    if (kwlnDrawControlFlags & 2) {
        return 0;
    }
    D_00437D18 = -1;
    mode = 3;
    D_00437D1C = 0;
    dds3AdminSubmitModeRequest(2, &mode, 4, 0);
    return 0;
}

void *fileFadeBeforeResettingRequest(void) {
    kwlnFadeInStart(0, 0, 0, 8);
    return fileResetPendingRequest;
}

void fileBeginLoadOrAbortDialog(void) {
    fileSetMenuFlowState(0);
    D_00437D1C = 10;
    ((MenuWork *)fileLoadSelectionWork)->unk30 = 0;
    fileBeginFourWayDialog((u32)fileRestartSelectionFlow, (u32)fileBeginFadeAndConfirmSound, (u32)fileFadeBeforeResettingRequest, 0);
}

s32 mcdContinueLoadSelection(void) {
    u32 state = fileGetLoadSelectionState();
    u32 block;
    s32 next = (s32)fileMenuWorkStart;
    if (state != 0) {
        if (state == 2) {
            block = fileLoadSelectionWork;
            ((MenuWork *)block)->unk36 = 0;
            fileApplyMenuFlagsToModel(block);
            sndSetSequenceVolumePan(8, 0x7f, 0x3f);
            next = (s32)fileNextMenuFlowState;
        } else {
            next = 0;
        }
    }
    return next;
}

extern s32 kwlnFadeIsActive(void);
extern void kwlnFadeStartIn(s32);
extern void fileBeginLoadOrAbortDialog(void);

void *fileMenuWorkStart(void) {
    D_00437D2C = 0;
    D_00437D28 = 0;
    D_00437CF8 = 0;
    if (kwlnFadeIsActive()) {
        kwlnFadeStartIn(0x10);
    }
    if (((MenuWork *)fileLoadSelectionWork)->startBranchFlag == 0) {
        return fileBeginLoadOrAbortDialog;
    }
    ((MenuWork *)fileLoadSelectionWork)->unk30 = 1;
    return fileBeginLoadBranchDialog();
}


void fileMenuWorkCreate(u32 startBranchFlag) {
    u32 buffer = sdfAllocGeneralBlock(0x40);
    MenuWork *work;

    fileLoadSelectionWork = sdfResourceRetainAddress(buffer);
    memset((void *)fileLoadSelectionWork, 0, 0x40);
    work = (MenuWork *)fileLoadSelectionWork;
    work->unk30 = 0;
    work->unk3C = buffer;
    work->startBranchFlag = startBranchFlag;
    work->unk38 = 0;
    ((MenuWork *)fileLoadSelectionWork)->unk31 = 0;
}

void fileReleaseMenuFlowResource(void) {
    if (fileLoadSelectionWork != 0) {
        sdfQueueNonzeroResourceId(((MenuWork *)fileLoadSelectionWork)->unk3C);
        fileLoadSelectionWork = 0;
    }
}

extern char D_0042B920[];

void func_002D0AB8(void) {
    func_0035B6E0(D_0042B920);
}

extern char D_0042B938[];

void fileSaveAndDisplayCurrentMoney(void) {
    FileSaveState *state = (FileSaveState *)datGameState;
    u32 money = state->header.money;
    state->savedMoney = money;
    func_0035B6E0(D_0042B938, money);
}

INCLUDE_RODATA(const s32, "game/code_002C96D0", D_0042B920);

INCLUDE_RODATA(const s32, "game/code_002C96D0", D_0042B938);

INCLUDE_ASM(const s32, "game/code_002C96D0", func_002D0B08);

void fileCopySaveHeaderNumbers(FileRecordHeader *source) {
    s32 state;

    state = datGameState;
    ((FileSaveState *)datGameState)->header.money = source->money;
    ((FileSaveState *)state)->header.header24 = source->header24;
    ((FileSaveState *)state)->header.header28 = source->header28;
    ((FileSaveState *)state)->header.header2C = source->header2C;
}

void fileCopyRecordHeader(FileRecordHeader *destination, const FileRecordHeader *source) {
    *destination = *source;
}

void fileApplyMenuFlagsToModel(MenuWork *work) {
    u32 i;
    s32 *bits = (s32 *)work + 1;

    for (i = 0; i < 0x4F; i++) {
        if ((bits[8 + (i >> 5)] >> (i & 0x1F)) & 1) {
            mdlFlagSet(0xB40 + i);
        } else {
            mdlFlagClear(0xB40 + i);
        }
    }
    for (i = 0; i < 5; i++) {
        if (mdlFlagTest(D_003E8CE8[i].flag)) {
            work->unk32 |= 2;
            break;
        }
    }
    for (i = 0; i < 0x18; i++) {
        if (mdlFlagTest(D_003E8D10[i].flag)) {
            work->unk32 |= 4;
            break;
        }
    }
    work->unk32 |= 9;
}

s32 fileLoadStateChanged(void) {
    return fileSlotFlagMirror.current != fileSlotFlagMirror.previous;
}

void fileCacheSlotFlagsFromState(void) {
    fileSlotFlagMirror.current = fileSlotFlagMirror.previous =
        ((FileSaveState *)datGameState)->slotFlags;
}

void fileRestoreSlotFlagsToState(void) {
    ((FileSaveState *)datGameState)->slotFlags = fileSavedSlotFlags;
}

extern void kwlnPadStartMotor(s32, u8, s32);

s32 fileToggleSlotFlagsBit(u32 kind, s32 *flags) {
    switch (kind) {
    case 0:
        *flags ^= 2;
        if (fileTestSlotFlagsBit(kind, flags) != 0) {
            kwlnPadStartMotor(0, 1, 0xF);
            kwlnPadStartMotor(1, 0x80, 0xF);
        } else {
            kwlnPadStartMotor(0, 0, 0xF);
            kwlnPadStartMotor(1, 0, 0xF);
        }
        return 1;
    case 1:
        *flags ^= 4;
        return 1;
    case 2:
        *flags ^= 8;
        return 1;
    case 3:
        *flags ^= 0x10;
        return 1;
    default:
        return 0;
    }
}

INCLUDE_RODATA(const s32, "game/code_002C96D0", D_0042B970);

INCLUDE_RODATA(const s32, "game/code_002C96D0", D_0042B980);

INCLUDE_RODATA(const s32, "game/code_002C96D0", D_0042B998);

INCLUDE_RODATA(const s32, "game/code_002C96D0", D_0042B9B0);

INCLUDE_RODATA(const s32, "game/code_002C96D0", D_0042B9D0);

INCLUDE_RODATA(const s32, "game/code_002C96D0", D_0042B9F0);

INCLUDE_RODATA(const s32, "game/code_002C96D0", D_0042BA10);

INCLUDE_RODATA(const s32, "game/code_002C96D0", D_0042BA30);

INCLUDE_RODATA(const s32, "game/code_002C96D0", D_0042BA50);

INCLUDE_RODATA(const s32, "game/code_002C96D0", D_0042BA70);

INCLUDE_RODATA(const s32, "game/code_002C96D0", D_0042BA90);

INCLUDE_RODATA(const s32, "game/code_002C96D0", D_0042BAB0);

s32 fileTestSlotFlagsBit(kind, flags)
    s32 kind;
    s32 *flags;
{
    switch (kind) {
    case 0:
        return ((*flags >> 1) ^ 1) & 1;
    case 1:
        return *flags & 4;
    case 2:
        return *flags & 8;
    case 3:
        return ((*flags >> 4) ^ 1) & 1;
    case 6:
        return 1;
    default:
        return -1;
    }
}

void fileTestSavedSlotFlags(u32 kind) {
    fileTestSlotFlagsBit(kind, datGameState + 0xa54);
}

INCLUDE_ASM(const s32, "game/code_002C96D0", func_002D1058);

typedef struct FileConfigCountdown {
    s32 ticks;
} FileConfigCountdown;

typedef struct FileConfigListNode {
    s32 index;
    u8 pad04[0x54];
    struct FileConfigListNode *next; /* 0x58 */
    u8 pad5C[0x14];
    FileConfigCountdown *resource;   /* 0x70: separately allocated countdown */
} FileConfigListNode;

typedef struct FileConfigList {
    u8 pad00[0x10];
    FileConfigListNode *head; /* 0x10 */
    u8 pad14[8];
    FileConfigListNode *cursor; /* 0x1C */
    s32 count;                /* 0x20 */
} FileConfigList;

/* Save/config task context: DDS2 places the status four bytes later. */
typedef struct FileConfigTask {
    void *memory;   /* 0x00 */
    u32 state;      /* 0x04 */
    s32 ticks;
    u32 frame;      /* 0x0C: FileConfigList, passed to menu window drawing */
    u32 slots[5];   /* 0x10 */
    u8 pad24[4];
    s32 result;     /* 0x28: negative when the queued load failed */
    s16 previousIndex;
    s16 transitionTicks;
    f32 choiceFade;
    f32 labelFade;
    u32 pending;    /* 0x38: zero when no load can start */
} FileConfigTask;



void fileConfigTaskDestroy(void) {
    s32 request = 1;
    FileConfigListNode *node;
    s32 i;

    if (fileConfigTaskWork != 0) {
        fileSavedSlotFlags = ((FileSaveState *)datGameState)->slotFlags;
        if (*(u32 *)(fileConfigTaskWork + 4) == 1) {
            dds3AdminSubmitModeRequest(2, &request, 4, 0);
            mnuAdvanceTitleStateUnderSemaphore();
        }
        node = ((FileConfigList *)((FileConfigTask *)fileConfigTaskWork)->frame)->head;
        for (i = 0; i < ((FileConfigList *)((FileConfigTask *)fileConfigTaskWork)->frame)->count; i++) {
            sdfReleaseChipBlock(node->resource);
            node = node->next;
        }
        mnuDestroyListState(((FileConfigTask *)fileConfigTaskWork)->frame);
        ((FileConfigTask *)fileConfigTaskWork)->frame = 0;
        for (i = 0; i < 5; i++) {
            if (((FileConfigTask *)fileConfigTaskWork)->slots[i] != 0) {
                effDestroyResourceSlotSet(((FileConfigTask *)fileConfigTaskWork)->slots[i]);
                ((FileConfigTask *)fileConfigTaskWork)->slots[i] = 0;
            }
        }
        sdfReleaseResourceAllocation(((FileConfigTask *)fileConfigTaskWork)->memory);
        fileConfigTaskWork = 0;
        fileConfigTaskState = 0;
    }
}

extern s32 func_002D1058(s32 mode);

extern s32 fileStartQueuedLoad(void);
extern void fileConfigTaskDestroy(void);
extern s32 func_002D1450(void);
extern u32 fileGetConfigTaskFailure(void);

extern s8 fileConfigTaskState;

void mnuCreateConfigTasks(s32 mode) {
    if (fileConfigTaskWork == 0) {
        fileConfigTaskWork = func_002D1058(mode);
        kwlnTaskCreate(fileConfigInputTaskName, 0x3F2, 1, 1, func_002D1450, NULL, (void *)fileConfigTaskWork);
        kwlnTaskCreate(fileConfigLoadTaskName, 0x2B07, 1, 1, fileStartQueuedLoad, NULL, (void *)fileConfigTaskWork);
        kwlnTaskCreate(fileConfigOwnerTaskName, 0x520B, 1, 1, fileGetConfigTaskFailure, fileConfigTaskDestroy, (void *)fileConfigTaskWork);
        fileConfigTaskState = 1;
    }
}

void mnuConfigTasksDestroy(void) {
    kwlnTaskDestroyWithHierarchyByName(fileConfigInputTaskName, 1);
    kwlnTaskDestroyWithHierarchyByName(fileConfigLoadTaskName, 1);
    kwlnTaskDestroyWithHierarchyByName(fileConfigOwnerTaskName, 1);
}

s32 fileConsumeConfigTaskReady(void) {
    s32 state = fileConfigTaskState;
    if (state == 1) {
        return 1;
    }
    if (state < 2) {
        return 0;
    }
    if (state == 2) {
        fileConfigTaskState = 0;
    }
    return 0;
}

u32 fileGetConfigTaskSlot(s32 slot) {
    if (slot < 4) {
        return ((FileConfigTask *)fileConfigTaskWork)->slots[slot];
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_002C96D0", fileConfigLoadTaskName);

INCLUDE_RODATA(const s32, "game/code_002C96D0", fileConfigOwnerTaskName);

s32 func_002D1450(void) {
    s32 oldIndex;
    s32 index;
    s32 i;
    s32 ticks;
    f32 fade;
    FileConfigListNode *cursor;
    FileConfigTask *loadingWork;

    if (((FileConfigTask *)fileConfigTaskWork)->transitionTicks != 0) {
        ((FileConfigTask *)fileConfigTaskWork)->transitionTicks--;
    }
    switch (((FileConfigTask *)fileConfigTaskWork)->result) {
    case 0:
        if (((FileConfigTask *)fileConfigTaskWork)->pending == 0) {
            ((FileConfigTask *)fileConfigTaskWork)->pending = 1;
            for (i = 0; i < 4; i++) {
                loadingWork = (FileConfigTask *)fileConfigTaskWork;
                if (loadingWork->slots[i] == 0) {
                    loadingWork->pending = 0;
                    return 0;
                }
            }
        }
        ((FileConfigTask *)fileConfigTaskWork)->result = 1;
        /* fallthrough */
    case 1:
        ticks = ((FileConfigTask *)fileConfigTaskWork)->ticks;
        ((FileConfigTask *)fileConfigTaskWork)->ticks = ticks + 1;
        fade = (f32)ticks / 15.0f;
        ((FileConfigTask *)fileConfigTaskWork)->choiceFade = ((FileConfigTask *)fileConfigTaskWork)->labelFade = fade;
        if (((FileConfigTask *)fileConfigTaskWork)->ticks >= 16) {
            ((FileConfigTask *)fileConfigTaskWork)->result = 2;
            ((FileConfigTask *)fileConfigTaskWork)->ticks = 0;
            ((FileConfigTask *)fileConfigTaskWork)->choiceFade = ((FileConfigTask *)fileConfigTaskWork)->labelFade = 1.0f;
        }
        break;
    case 2:
        ((FileConfigTask *)fileConfigTaskWork)->ticks++;
        if (((FileConfigTask *)fileConfigTaskWork)->ticks >= 121) {
            ((FileConfigTask *)fileConfigTaskWork)->ticks = 0;
        }
        break;
    case 3:
        if (((FileConfigTask *)fileConfigTaskWork)->state == 1) {
            kwlnFadeInStart(0, 0, 0, 15);
        }
        ((FileConfigTask *)fileConfigTaskWork)->ticks = 0;
        ((FileConfigTask *)fileConfigTaskWork)->result = 4;
        /* fallthrough */
    case 4:
        if (((FileConfigTask *)fileConfigTaskWork)->ticks >= 6) {
            ((FileConfigTask *)fileConfigTaskWork)->labelFade = 0.0f;
        } else {
            ((FileConfigTask *)fileConfigTaskWork)->labelFade = 1.0f - (f32)((FileConfigTask *)fileConfigTaskWork)->ticks / 5.0f;
        }
        ticks = ((FileConfigTask *)fileConfigTaskWork)->ticks;
        ((FileConfigTask *)fileConfigTaskWork)->ticks = ticks + 1;
        ((FileConfigTask *)fileConfigTaskWork)->choiceFade = 1.0f - (f32)ticks / 15.0f;
        if (((FileConfigTask *)fileConfigTaskWork)->ticks >= 16) {
            ((FileConfigTask *)fileConfigTaskWork)->result = 5;
            ((FileConfigTask *)fileConfigTaskWork)->ticks = 0;
            ((FileConfigTask *)fileConfigTaskWork)->labelFade = 0.0f;
            ((FileConfigTask *)fileConfigTaskWork)->choiceFade = 0.0f;
        }
        return 0;
    case 5:
        ((FileConfigTask *)fileConfigTaskWork)->result |= 0x80000000;
        return -1;
    }

    oldIndex = ((FileConfigList *)((FileConfigTask *)fileConfigTaskWork)->frame)->cursor->index;
    if ((u8)D_0037F510[0x26] & 2) {
        if (mnuRetreatListCursorDefault(((FileConfigTask *)fileConfigTaskWork)->frame) != NULL) {
            sndSetSequenceVolumePan(0, 0x7F, 0x3F);
            ((FileConfigTask *)fileConfigTaskWork)->transitionTicks = 8;
            ((FileConfigTask *)fileConfigTaskWork)->previousIndex = oldIndex;
        }
    }
    if ((u8)D_0037F510[0x27] & 2) {
        if (mnuAdvanceListCursorDefault(((FileConfigTask *)fileConfigTaskWork)->frame) != NULL) {
            sndSetSequenceVolumePan(0, 0x7F, 0x3F);
            ((FileConfigTask *)fileConfigTaskWork)->previousIndex = oldIndex;
            ((FileConfigTask *)fileConfigTaskWork)->transitionTicks = 8;
        }
    }
    index = ((FileConfigList *)((FileConfigTask *)fileConfigTaskWork)->frame)->cursor->index;
    if (index < 4) {
        if (D_0037F510[0x24] < 0 && fileTestSlotFlagsBit(index, (s32 *)(datGameState + 0xA54)) == 0) {
            fileToggleSlotFlagsBit(((FileConfigList *)((FileConfigTask *)fileConfigTaskWork)->frame)->cursor->index, (s32 *)(datGameState + 0xA54));
            sndSetSequenceVolumePan(8, 0x7F, 0x3F);
            cursor = ((FileConfigList *)((FileConfigTask *)fileConfigTaskWork)->frame)->cursor;
            cursor->resource->ticks = 8;
        }
        if (D_0037F510[0x25] < 0) {
            if (fileTestSlotFlagsBit(((FileConfigList *)((FileConfigTask *)fileConfigTaskWork)->frame)->cursor->index, (s32 *)(datGameState + 0xA54)) != 0) {
                fileToggleSlotFlagsBit(((FileConfigList *)((FileConfigTask *)fileConfigTaskWork)->frame)->cursor->index, (s32 *)(datGameState + 0xA54));
                sndSetSequenceVolumePan(8, 0x7F, 0x3F);
                cursor = ((FileConfigList *)((FileConfigTask *)fileConfigTaskWork)->frame)->cursor;
                cursor->resource->ticks = 8;
            }
        }
    }
    if (D_0037F510[0x26] == 0 && D_0037F510[0x27] == 0) {
        mnuClearListFlagsOneAndTwo((u32 *)((FileConfigTask *)fileConfigTaskWork)->frame);
    }
    if (D_0037F510[0x21] < 0) {
        if (((FileConfigList *)((FileConfigTask *)fileConfigTaskWork)->frame)->cursor->index == 4) {
            ((FileConfigTask *)fileConfigTaskWork)->result = 3;
            sndSetSequenceVolumePan(8, 0x7F, 0x3F);
        }
    } else if (D_0037F510[0x23] < 0) {
        ((FileConfigTask *)fileConfigTaskWork)->result = 3;
        fileRestoreSlotFlagsToState();
        sndSetSequenceVolumePan(10, 0x7F, 0x3F);
    }
    return 0;
}

extern void func_002D27A0(void *);

s32 fileStartQueuedLoad(void) {
    if (((FileConfigTask *)fileConfigTaskWork)->pending == 0) {
        return 0;
    }
    if (((FileConfigTask *)fileConfigTaskWork)->result < 0) {
        return -1;
    }
    func_002D27A0((void *)fileConfigTaskWork);
    mnuCallInitWide(0x400, 0x400, 0, ((FileConfigTask *)fileConfigTaskWork)->frame, 0x53);
    return 0;
}

u32 fileGetConfigTaskFailure(void) {
    u32 result;

    result = 0xffffffff;
    if ((((FileConfigTask *)fileConfigTaskWork)->result & 0x80000000) == 0) {
        result = 0;
    }
    return result;
}

INCLUDE_SDATA(const s32, "game/code_002C96D0", fileMemoryCardRequestContext);

INCLUDE_SDATA(const s32, "game/code_002C96D0", fileMenuTaskAlive);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437CD5);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437CD8);

INCLUDE_SDATA(const s32, "game/code_002C96D0", fileSaveIconRequest);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437CE4);

INCLUDE_SDATA(const s32, "game/code_002C96D0", fileWaitTicksRemaining);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437CEC);

INCLUDE_SDATA(const s32, "game/code_002C96D0", fileSlotSelectionPollCount);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437CF4);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437CF8);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437CFC);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D00);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D04);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D08);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D0C);

INCLUDE_SDATA(const s32, "game/code_002C96D0", fileSlotScanIndex);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D14);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D18);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D1C);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D20);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D24);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D28);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D2C);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D30);

INCLUDE_SDATA(const s32, "game/code_002C96D0", fileSelectionPending);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D38);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D3C);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D40);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D44);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D48);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D4C);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D50);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D54);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D58);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D5C);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D60);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D64);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D68);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D6C);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D70);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D74);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D78);

INCLUDE_SDATA(const s32, "game/code_002C96D0", mcdOriginalTitleFileMode);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D80);

INCLUDE_SDATA(const s32, "game/code_002C96D0", fileLoadSelectionWork);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D88);

INCLUDE_SDATA(const s32, "game/code_002C96D0", fileSaveHighlightPhase);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D90);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D94);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437D98);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437DA0);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437DA8);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437DB0);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437DB8);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437DC0);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437DD0);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437DD8);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437DE0);

INCLUDE_SDATA(const s32, "game/code_002C96D0", fileConfigTaskState);

INCLUDE_SDATA(const s32, "game/code_002C96D0", fileSlotFlagMirror);

INCLUDE_SDATA(const s32, "game/code_002C96D0", fileSavedSlotFlags);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437DF0);

INCLUDE_SDATA(const s32, "game/code_002C96D0", fileConfigInputTaskName);

void func_002D1930(s32 x, s32 y, s32 depth, FileConfigList *list,
                   FileConfigListNode *node, s32 drawArg) {
    s8 labelFrames[5] = {0, 1, 2, 3, 4};
    s32 index = node->index;
    FileConfigCountdown *countdown;
    s32 entryIndex;
    f32 pulse;
    f32 choiceFade = 1.0f;
    f32 labelFade;

    pulse = 0.0f;
    if (((FileConfigTask *)fileConfigTaskWork)->result != 1) {
        pulse = (f32)(((FileConfigTask *)fileConfigTaskWork)->ticks % 120) / 60.0f;
        if (pulse > choiceFade) {
            pulse = 2.0f - pulse;
        }
    }
    countdown = node->resource;
    choiceFade = ((FileConfigTask *)fileConfigTaskWork)->choiceFade;
    labelFade = ((FileConfigTask *)fileConfigTaskWork)->labelFade;
    if (countdown->ticks != 0) {
        --countdown->ticks;
    }
    if (list->cursor->index == node->index) {
        func_00306CD0(0x80, (index * 35 + 99) << 3, 0,
                     (u32)((pulse * 204.79998779296875f + 102.399993896484375f) * labelFade),
                     0, (struct EffectSlotSet *)((FileConfigTask *)fileConfigTaskWork)->slots[1], 18, 0x53);
        func_00306CD0(0x1830, (index * 35 + 99) << 3, 0,
                     (u32)((pulse * 204.79998779296875f + 102.399993896484375f) * labelFade),
                     0, (struct EffectSlotSet *)((FileConfigTask *)fileConfigTaskWork)->slots[1], 19, 0x53);
        func_00306CD0(0x8B0, 0x8C0, 0, (u32)(labelFade * 256.0f),
                     0, (struct EffectSlotSet *)((FileConfigTask *)fileConfigTaskWork)->slots[3], labelFrames[index], 0x53);
        entryIndex = index * 2;
        func_00306CD0(D_003E9028[entryIndex][FILE_CONFIG_X] << 4, (D_003E9028[entryIndex][FILE_CONFIG_Y] - 30) << 3, 0,
                     (u32)(choiceFade * 256.0f), 0,
                     (struct EffectSlotSet *)((FileConfigTask *)fileConfigTaskWork)->slots[D_003E9028[entryIndex][FILE_CONFIG_SET]],
                     D_003E9028[entryIndex][FILE_CONFIG_FRAME], 0x53);
        if (((FileConfigTask *)fileConfigTaskWork)->transitionTicks != 0) {
            ++entryIndex;
            func_00306CD0(D_003E9028[entryIndex][FILE_CONFIG_X] << 4, (D_003E9028[entryIndex][FILE_CONFIG_Y] - 30) << 3, 0,
                         (u32)((f32)((FileConfigTask *)fileConfigTaskWork)->transitionTicks * 0.125f * 256.0f), 0,
                         (struct EffectSlotSet *)((FileConfigTask *)fileConfigTaskWork)->slots[D_003E9028[entryIndex][FILE_CONFIG_SET]],
                         D_003E9028[entryIndex][FILE_CONFIG_FRAME], 0x53);
        }
    } else {
        entryIndex = index + 18;
        func_00306CD0(D_003E9028[entryIndex][FILE_CONFIG_X] << 4, (D_003E9028[entryIndex][FILE_CONFIG_Y] - 30) << 3, 0,
                     (u32)(labelFade * 256.0f), 0,
                     (struct EffectSlotSet *)((FileConfigTask *)fileConfigTaskWork)->slots[D_003E9028[entryIndex][FILE_CONFIG_SET]],
                     D_003E9028[entryIndex][FILE_CONFIG_FRAME], 0x53);
        entryIndex = index * 2;
        if (((FileConfigTask *)fileConfigTaskWork)->previousIndex == node->index) {
            func_00306CD0(D_003E9028[entryIndex][FILE_CONFIG_X] << 4, (D_003E9028[entryIndex][FILE_CONFIG_Y] - 30) << 3, 0,
                         (u32)((f32)((FileConfigTask *)fileConfigTaskWork)->transitionTicks * 0.125f * 128.0f), 0,
                         (struct EffectSlotSet *)((FileConfigTask *)fileConfigTaskWork)->slots[D_003E9028[entryIndex][FILE_CONFIG_SET]],
                         D_003E9028[entryIndex][FILE_CONFIG_FRAME], 0x53);
        }
    }
    if (index < 3) {
        if (fileTestSlotFlagsBit(index, datGameState + 0xA54) != 0) {
            func_00306CD0(D_003E9028[10][FILE_CONFIG_X] << 4, (D_003E9028[10][FILE_CONFIG_Y] + index * 35 - 30) << 3, 0,
                         (u32)(choiceFade * 256.0f), 0,
                         (struct EffectSlotSet *)((FileConfigTask *)fileConfigTaskWork)->slots[D_003E9028[10][FILE_CONFIG_SET]],
                         D_003E9028[10][FILE_CONFIG_FRAME], 0x53);
            if (countdown->ticks != 0) {
                func_00306CD0(D_003E9028[11][FILE_CONFIG_X] << 4, (D_003E9028[11][FILE_CONFIG_Y] + index * 35 - 30) << 3, 0,
                             (u32)((f32)countdown->ticks * 0.125f * 256.0f), 0,
                             (struct EffectSlotSet *)((FileConfigTask *)fileConfigTaskWork)->slots[D_003E9028[11][FILE_CONFIG_SET]],
                             D_003E9028[11][FILE_CONFIG_FRAME], 0x53);
            }
            func_00306CD0(D_003E9028[25][FILE_CONFIG_X] << 4, (D_003E9028[25][FILE_CONFIG_Y] + index * 35 - 30) << 3, 0,
                         (u32)(labelFade * 256.0f), 0,
                         (struct EffectSlotSet *)((FileConfigTask *)fileConfigTaskWork)->slots[D_003E9028[25][FILE_CONFIG_SET]],
                         D_003E9028[25][FILE_CONFIG_FRAME], 0x53);
            if (countdown->ticks != 0) {
                func_00306CD0(D_003E9028[14][FILE_CONFIG_X] << 4, (D_003E9028[14][FILE_CONFIG_Y] + index * 35 - 30) << 3, 0,
                             (u32)((f32)countdown->ticks * 0.125f * 128.0f), 0,
                             (struct EffectSlotSet *)((FileConfigTask *)fileConfigTaskWork)->slots[D_003E9028[14][FILE_CONFIG_SET]],
                             D_003E9028[14][FILE_CONFIG_FRAME], 0x53);
            }
        } else {
            func_00306CD0(D_003E9028[23][FILE_CONFIG_X] << 4, (D_003E9028[23][FILE_CONFIG_Y] + index * 35 - 30) << 3, 0,
                         (u32)(labelFade * 256.0f), 0,
                         (struct EffectSlotSet *)((FileConfigTask *)fileConfigTaskWork)->slots[D_003E9028[23][FILE_CONFIG_SET]],
                         D_003E9028[23][FILE_CONFIG_FRAME], 0x53);
            if (countdown->ticks != 0) {
                func_00306CD0(D_003E9028[10][FILE_CONFIG_X] << 4, (D_003E9028[10][FILE_CONFIG_Y] + index * 35 - 30) << 3, 0,
                             (u32)((f32)countdown->ticks * 0.125f * 128.0f), 0,
                             (struct EffectSlotSet *)((FileConfigTask *)fileConfigTaskWork)->slots[D_003E9028[10][FILE_CONFIG_SET]],
                             D_003E9028[10][FILE_CONFIG_FRAME], 0x53);
            }
            func_00306CD0(D_003E9028[14][FILE_CONFIG_X] << 4, (D_003E9028[14][FILE_CONFIG_Y] + index * 35 - 30) << 3, 0,
                         (u32)(choiceFade * 256.0f), 0,
                         (struct EffectSlotSet *)((FileConfigTask *)fileConfigTaskWork)->slots[D_003E9028[14][FILE_CONFIG_SET]],
                         D_003E9028[14][FILE_CONFIG_FRAME], 0x53);
            if (countdown->ticks != 0) {
                func_00306CD0(D_003E9028[15][FILE_CONFIG_X] << 4, (D_003E9028[15][FILE_CONFIG_Y] + index * 35 - 30) << 3, 0,
                             (u32)((f32)countdown->ticks * 0.125f * 256.0f), 0,
                             (struct EffectSlotSet *)((FileConfigTask *)fileConfigTaskWork)->slots[D_003E9028[15][FILE_CONFIG_SET]],
                             D_003E9028[15][FILE_CONFIG_FRAME], 0x53);
            }
        }
    } else if (index == 3) {
        if (fileTestSlotFlagsBit(3, datGameState + 0xA54) != 0) {
            func_00306CD0(D_003E9028[12][FILE_CONFIG_X] << 4, (D_003E9028[12][FILE_CONFIG_Y] - 30) << 3, 0,
                         (u32)(choiceFade * 256.0f), 0,
                         (struct EffectSlotSet *)((FileConfigTask *)fileConfigTaskWork)->slots[D_003E9028[12][FILE_CONFIG_SET]],
                         D_003E9028[12][FILE_CONFIG_FRAME], 0x53);
            if (countdown->ticks != 0) {
                func_00306CD0(D_003E9028[13][FILE_CONFIG_X] << 4, (D_003E9028[13][FILE_CONFIG_Y] - 30) << 3, 0,
                             (u32)((f32)countdown->ticks * 0.125f * 256.0f), 0,
                             (struct EffectSlotSet *)((FileConfigTask *)fileConfigTaskWork)->slots[D_003E9028[13][FILE_CONFIG_SET]],
                             D_003E9028[13][FILE_CONFIG_FRAME], 0x53);
            }
            func_00306CD0(D_003E9028[26][FILE_CONFIG_X] << 4, (D_003E9028[26][FILE_CONFIG_Y] - 30) << 3, 0,
                         (u32)(labelFade * 256.0f), 0,
                         (struct EffectSlotSet *)((FileConfigTask *)fileConfigTaskWork)->slots[D_003E9028[26][FILE_CONFIG_SET]],
                         D_003E9028[26][FILE_CONFIG_FRAME], 0x53);
            if (countdown->ticks != 0) {
                func_00306CD0(D_003E9028[16][FILE_CONFIG_X] << 4, (D_003E9028[16][FILE_CONFIG_Y] - 30) << 3, 0,
                             (u32)((f32)countdown->ticks * 0.125f * 128.0f), 0,
                             (struct EffectSlotSet *)((FileConfigTask *)fileConfigTaskWork)->slots[D_003E9028[16][FILE_CONFIG_SET]],
                             D_003E9028[16][FILE_CONFIG_FRAME], 0x53);
            }
        } else {
            func_00306CD0(D_003E9028[24][FILE_CONFIG_X] << 4, (D_003E9028[24][FILE_CONFIG_Y] - 30) << 3, 0,
                         (u32)(labelFade * 256.0f), 0,
                         (struct EffectSlotSet *)((FileConfigTask *)fileConfigTaskWork)->slots[D_003E9028[24][FILE_CONFIG_SET]],
                         D_003E9028[24][FILE_CONFIG_FRAME], 0x53);
            if (countdown->ticks != 0) {
                func_00306CD0(D_003E9028[12][FILE_CONFIG_X] << 4, (D_003E9028[12][FILE_CONFIG_Y] - 30) << 3, 0,
                             (u32)((f32)countdown->ticks * 0.125f * 128.0f), 0,
                             (struct EffectSlotSet *)((FileConfigTask *)fileConfigTaskWork)->slots[D_003E9028[12][FILE_CONFIG_SET]],
                             D_003E9028[12][FILE_CONFIG_FRAME], 0x53);
            }
            func_00306CD0(D_003E9028[16][FILE_CONFIG_X] << 4, (D_003E9028[16][FILE_CONFIG_Y] - 30) << 3, 0,
                         (u32)(choiceFade * 256.0f), 0,
                         (struct EffectSlotSet *)((FileConfigTask *)fileConfigTaskWork)->slots[D_003E9028[16][FILE_CONFIG_SET]],
                         D_003E9028[16][FILE_CONFIG_FRAME], 0x53);
            if (countdown->ticks != 0) {
                func_00306CD0(D_003E9028[17][FILE_CONFIG_X] << 4, (D_003E9028[17][FILE_CONFIG_Y] - 30) << 3, 0,
                             (u32)((f32)countdown->ticks * 0.125f * 256.0f), 0,
                             (struct EffectSlotSet *)((FileConfigTask *)fileConfigTaskWork)->slots[D_003E9028[17][FILE_CONFIG_SET]],
                             D_003E9028[17][FILE_CONFIG_FRAME], 0x53);
            }
        }
    }
}


void func_002D27A0(void *arg) {
    s16 sprites[5][4] = {
        {0, 2, 99, -57},
        {0, 3, -31, 365},
        {0, 5, 0, 120},
        {0, 4, 0, 120},
        {0, 6, -20, -20}
    };
    f32 labelFade = ((FileConfigTask *)fileConfigTaskWork)->labelFade;
    f32 choiceFade = ((FileConfigTask *)fileConfigTaskWork)->choiceFade;
    s32 index;

    index = 0;
    func_00306CD0(sprites[index][FILE_CONFIG_X] * 16, (s32)((u32)(sprites[index][FILE_CONFIG_Y] - 30) << 3), 0,
                 (u32)(labelFade * 256.0f), 0,
                 (struct EffectSlotSet *)((FileConfigTask *)fileConfigTaskWork)->slots[sprites[index][FILE_CONFIG_SET]],
                 sprites[index][FILE_CONFIG_FRAME], 0x53);
    index++;
    func_00306CD0(sprites[index][FILE_CONFIG_X] * 16, (s32)((u32)(sprites[index][FILE_CONFIG_Y] - 30) << 3), 0,
                 (u32)(labelFade * 256.0f), 0,
                 (struct EffectSlotSet *)((FileConfigTask *)fileConfigTaskWork)->slots[sprites[index][FILE_CONFIG_SET]],
                 sprites[index][FILE_CONFIG_FRAME], 0x53);
    if (((FileConfigTask *)fileConfigTaskWork)->state == 1) {
        func_00306CD0(0, 0, 0, 256, 0,
                     (struct EffectSlotSet *)((FileConfigTask *)fileConfigTaskWork)->slots[4], 0, 0x53);
    }
    index++;
    func_00306CD0(sprites[index][FILE_CONFIG_X] * 16, (s32)((u32)(sprites[index][FILE_CONFIG_Y] - 30) << 3), 0,
                 (u32)(labelFade * 256.0f), 0,
                 (struct EffectSlotSet *)((FileConfigTask *)fileConfigTaskWork)->slots[sprites[index][FILE_CONFIG_SET]],
                 sprites[index][FILE_CONFIG_FRAME], 0x53);
    index++;
    func_00306CD0(sprites[index][FILE_CONFIG_X] * 16, (s32)((u32)(sprites[index][FILE_CONFIG_Y] - 30) << 3), 0,
                 (u32)(choiceFade * 256.0f), 0,
                 (struct EffectSlotSet *)((FileConfigTask *)fileConfigTaskWork)->slots[sprites[index][FILE_CONFIG_SET]],
                 sprites[index][FILE_CONFIG_FRAME], 0x53);
    index++;
    func_00306CD0(sprites[index][FILE_CONFIG_X] * 16, (s32)((u32)(sprites[index][FILE_CONFIG_Y] - 30) << 3), 0,
                 (u32)(labelFade * 256.0f), 0,
                 (struct EffectSlotSet *)((FileConfigTask *)fileConfigTaskWork)->slots[sprites[index][FILE_CONFIG_SET]],
                 sprites[index][FILE_CONFIG_FRAME], 0x53);
    for (index = 0; index < 5; index++) {
        func_00306CD0(0, (131 + index * 35 - 30) * 8, 0, (u32)(labelFade * 256.0f), 0,
                     (struct EffectSlotSet *)((FileConfigTask *)fileConfigTaskWork)->slots[2], 9, 0x53);
        func_00306CD0(461 * 16, (131 + index * 35 - 30) * 8, 0, (u32)(labelFade * 256.0f), 0,
                     (struct EffectSlotSet *)((FileConfigTask *)fileConfigTaskWork)->slots[2], 10, 0x53);
    }
    func_00306CD0(73 * 16, (326 - 30) * 8, 0, (u32)(labelFade * 256.0f), 0,
                 (struct EffectSlotSet *)((FileConfigTask *)fileConfigTaskWork)->slots[2], 11, 0x53);
}


void effLoadCommonTexturesAndResetRenderFlags(void) {
    effLoadFlashTextures();
    effLoadWindTexture();
    effLoadScalyTexture();
    fileResetRenderFlags();
}

void fileSetRenderFlag(u32 mask) {
    effModelUpdateControlFlags = effModelUpdateControlFlags | mask;
}

u32 fileClearRenderFlag(u32 mask) {
    u32 flags = effModelUpdateControlFlags & ~mask;

    effModelUpdateControlFlags = flags;
    return flags;
}

void fileResetRenderFlags(void) {
    effModelUpdateControlFlags = 0;
}

u32 func_002D2CB0(s32 index) {
    return D_003E9150[index];
}

void mnuProjectViewPoint(void) {
    u8 *matrix;
    VU0_LOAD_MATRIX(sdfViewMatrix);
    matrix = sdfProjectionMatrix;
    sdfPostmultiplyVuMatrixFromMemory(matrix);
    VU0_TRANSFORM_POINT(vf10, vf10);
    VU0_PERSPECTIVE_DIVIDE_VF10();
    matrix += 0x40;
    VU0_LOAD_VF(vf11, matrix);
    VU0_MUL(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, D_0037F660);
    VU0_ADD(vf10, vf10, vf11);
}

extern u8 sdfViewTargetVector[];
extern void sdfInvertRigidVuTransform(void);

/* vu0 routine: tests whether vf10 projects within the camera's forward cone. */
s32 func_002D2D48(void) {
    f32 distance, facing;
    f32 *projection;

    VU0_MOVE_VF(vf12, vf10);
    VU0_LOAD_VF(vf11, sdfViewTargetVector);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LENGTH_VF10(distance);
    if (!(distance > 16.0))
        return 0;

    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_MATRIX(sdfViewMatrix);
    VU0_MOVE_MATRIX_TO_B();
    sdfInvertRigidVuTransform();
    VU0_MOVE_VF(vf10, vf30);
    VU0_NORMALIZE_VF10();
    VU0_DOT_XYZ(facing, vf10, vf11);
    if (facing <= 0.25f)
        return 0;

    VU0_MOVE_VF(vf10, vf12);
    VU0_MOVE_VF(vf28, vf24);
    VU0_MOVE_VF(vf29, vf25);
    VU0_MOVE_VF(vf30, vf26);
    VU0_MOVE_VF(vf31, vf27);
    projection = (f32 *)sdfProjectionMatrix;
    sdfPostmultiplyVuMatrixFromMemory(projection);
    VU0_TRANSFORM_POINT(vf10, vf10);
    VU0_PERSPECTIVE_DIVIDE_VF10();
    projection += 16;
    VU0_LOAD_VF(vf11, projection);
    VU0_MUL(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, D_0037F660);
    VU0_ADD(vf10, vf10, vf11);
    return 1;
}

extern u8 sdfViewUpVector[];

/* vu0 routine: distance between the projected view point and a second point offset perpendicular to
   the camera axis by rate; 0 when either projection (func_002D2D48) fails. Point comes in vf10. */
f32 mnuMeasureProjectedPerpendicularDistance(f32 rate) {
    f32 scale[4];
    f32 second[4];
    f32 first[4];
    f32 origin[4];
    f32 dx;
    f32 dy;

    VU0_STORE_VF(vf10, origin);
    if (func_002D2D48() == 0) {
        return 0.0f;
    }
    VU0_STORE_VF(vf10, first);
    scale[0] = scale[1] = scale[2] = rate;
    VU0_LOAD_VF(vf10, origin);
    VU0_MOVE_VF(vf12, vf10);
    VU0_LOAD_VF(vf11, sdfViewTargetVector);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, sdfViewUpVector);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_LOAD_VF(vf11, scale);
    VU0_MUL(vf10, vf10, vf11);
    VU0_MOVE_VF(vf11, vf12);
    VU0_ADD(vf10, vf10, vf11);
    if (func_002D2D48() == 0) {
        return 0.0f;
    }
    VU0_STORE_VF(vf10, second);
    dx = second[0] - first[0];
    dy = second[1] - first[1];
    VU0_LOAD_VF(vf10, first);
    return fsqrtf(dx * dx + dy * dy);
}


/* vu0 routine: point at t between p[1] and p[2] of a Catmull-Rom (Hermite, 0.5 tangents) spline, left in vf10 */
void vuCatmullRomPoint(f32 (*p)[4], f32 t)
{
    f32 tan[2][4];
    f32 half[4];
    f32 h[4][4];
    f32 t2 = t * t;
    f32 t3 = t2 * t;

    VEC3_SPLAT(half, 0.5f);
    VU0_LOAD_VF(vf10, p[1]);
    VU0_LOAD_VF(vf11, p[0]);
    VU0_SUB(vf10, vf10, vf11);
    VU0_MOVE_VF(vf12, vf10);
    VU0_LOAD_VF(vf10, p[2]);
    VU0_LOAD_VF(vf11, p[1]);
    VU0_SUB(vf10, vf10, vf11);
    VU0_ADD(vf10, vf10, vf12);
    VU0_LOAD_VF(vf11, half);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, tan[0]);
    VU0_LOAD_VF(vf10, p[2]);
    VU0_LOAD_VF(vf11, p[1]);
    VU0_SUB(vf10, vf10, vf11);
    VU0_MOVE_VF(vf12, vf10);
    VU0_LOAD_VF(vf10, p[3]);
    VU0_LOAD_VF(vf11, p[2]);
    VU0_SUB(vf10, vf10, vf11);
    VU0_ADD(vf10, vf10, vf12);
    VU0_LOAD_VF(vf11, half);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, tan[1]);
    VEC3_SPLAT(h[0], 2.0f * t3 - 3.0f * t2 + 1.0f);
    VEC3_SPLAT(h[1], t3 - 2.0f * t2 + t);
    VEC3_SPLAT(h[2], t3 - t2);
    VEC3_SPLAT(h[3], -2.0f * t3 + 3.0f * t2);
    VU0_LOAD_VF(vf10, h[0]);
    VU0_LOAD_VF(vf11, p[1]);
    VU0_MUL(vf10, vf10, vf11);
    VU0_MOVE_VF(vf12, vf10);
    VU0_LOAD_VF(vf10, h[1]);
    VU0_LOAD_VF(vf11, tan[0]);
    VU0_MUL(vf10, vf10, vf11);
    VU0_ADD(vf12, vf12, vf10);
    VU0_LOAD_VF(vf10, h[2]);
    VU0_LOAD_VF(vf11, tan[1]);
    VU0_MUL(vf10, vf10, vf11);
    VU0_ADD(vf12, vf12, vf10);
    VU0_LOAD_VF(vf10, h[3]);
    VU0_LOAD_VF(vf11, p[2]);
    VU0_MUL(vf10, vf10, vf11);
    VU0_ADD(vf10, vf10, vf12);
}

extern u8 sdfViewTargetVector[];
extern u8 sdfViewEyeVector[];
extern u8 sdfViewUpVector[];
extern f32 func_003532B8(f32);
extern f32 func_003532E8(f32, f32);
extern void func_00340DC8(f32, f32, f32);
extern void effMiscQuatMultiplyVU(void);
extern void effMiscQuaternionToMatrixVU(void);

/* vu0 routine: camera basis rows vf28-vf31 from eye sdfViewTargetVector, target sdfViewEyeVector and up sdfViewUpVector */
void vuBuildLookAtBasis(void)
{
    VU0_LOAD_VF(vf10, sdfViewTargetVector);
    VU0_MOVE_VF(vf31, vf10);
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, sdfViewEyeVector);
    VU0_SUB(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf30, vf10);
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, sdfViewUpVector);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf28, vf10);
    VU0_MOVE_VF(vf11, vf10);
    VU0_MOVE_VF(vf10, vf30);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_MOVE_VF(vf29, vf10);
}

/* The quaternion result is returned in vf10 for the VU transform routines. */
void sdfVuMatrixToQuaternion(f32 matrix[4][4]) {
    f32 quaternion[4];
    f32 trace = matrix[0][0] + matrix[1][1] + matrix[2][2] + matrix[3][3];
    f32 scale;
    s32 i;
    s32 j;
    s32 k;

    if (trace >= 1.0f) {
        scale = 2.0f * fsqrtf(trace);
        quaternion[3] = -scale * 0.25f;
        quaternion[0] = (matrix[1][2] - matrix[2][1]) / scale;
        quaternion[1] = (matrix[2][0] - matrix[0][2]) / scale;
        quaternion[2] = (matrix[0][1] - matrix[1][0]) / scale;
    } else {
        i = matrix[0][0] > matrix[1][1] ? 0 : 1;
        if (matrix[i][i] < matrix[2][2]) {
            i = 2;
        }
        j = (i + 1) % 3;
        k = (j + 1) % 3;
        trace = matrix[i][i] - matrix[j][j] - matrix[k][k] + 1.0f;
        if (trace != 0.0f) {
            scale = 2.0f * fsqrtf(trace);
            quaternion[i] = scale * 0.25f;
            quaternion[j] = (matrix[j][i] + matrix[i][j]) / scale;
            quaternion[k] = (matrix[k][i] + matrix[i][k]) / scale;
            quaternion[3] = -(matrix[j][k] - matrix[k][j]) / scale;
        } else {
            quaternion[i] = 1.0f;
            quaternion[j] = 0.0f;
            quaternion[k] = 0.0f;
            quaternion[3] = 0.0f;
        }
    }
    VU0_LOAD_VF(vf10, quaternion);
}

FileJob *fileCreateJob(u16 type) {
    u16 kind = type;
    FileJob *job = sdfAllocSizeClassBlock(0x2C);
    memset(job, 0, 0x2C);
    job->unk0 = 200;
    job->type = kind;
    return job;
}

void *fileResolvePrimaryBuffer(FileJob *job) {
    if (job->slots[0].allocation != NULL) {
        return (void *)job->slots[0].offset;
    }
    if (job->slots[0].offset != 0) {
        return (u8 *)job + job->slots[0].offset;
    }
    return NULL;
}

void *fileResolveSecondaryBuffer(FileJob *job) {
    if (job->slots[1].allocation != NULL) {
        return (void *)job->slots[1].offset;
    }
    if (job->slots[1].offset != 0) {
        return (u8 *)job + job->slots[1].offset;
    }
    return NULL;
}

FileJob *fileJobCreateFromJob(FileJob *request) {
    FileJob *job = fileCreateJob(request->type);
    job->option = request->option;
    job->slots[0].selector = request->slots[0].selector;
    job->data = fileJobTypeOperations[job->type].create(request);
    return job;
}

void fileJobDestroy(FileJob *job) {
    void *data = job->data;
    if (data != NULL) {
        fileJobTypeOperations[job->type].destroy(data);
    }
    fileJobFreePrimaryBuffer(job);
    fileJobFreeSecondaryBuffer(job);
    sdfReleaseChipBlock(job);
}

void fileJobFreePrimaryBuffer(FileJob *job) {
    void *buffer = job->slots[0].allocation;
    if (buffer != NULL) {
        sdfReleaseResourceAllocation(buffer);
        job->slots[0].offset = 0;
        job->slots[0].size = 0;
        job->slots[0].allocation = NULL;
    }
}

void fileJobFreeSecondaryBuffer(FileJob *job) {
    void *buffer = job->slots[1].allocation;
    if (buffer != NULL) {
        sdfReleaseResourceAllocation(buffer);
        job->slots[1].offset = 0;
        job->slots[1].size = 0;
        job->slots[1].allocation = NULL;
    }
}

FileJob *fileJobCreateChild(FileJob *request) {
    FileJob *job = fileCreateJob(request->type);
    FileTypeCallbacks *cb = &fileJobTypeOperations[job->type];

    job->option = request->option;
    job->slots[0].selector = request->slots[0].selector;
    job->data = cb->createChild(request->data, job->type);
    return job;
}

void fileJobNotifyPair(FileJob *left, FileJob *right) {
    void (*cb)(void *, void *) = D_003E916C[right->type].cbC;

    if (cb != NULL) {
        cb(left->data, right->data);
    }
}

void fileJobNotifyComplete(void *work) {
    u16 id = ((FileJob *)work)->type;

    if (D_003E917C[id].func != NULL) {
        D_003E917C[id].func(((FileJob *)work)->data);
    }
}

void fileJobInvokeTypeCallback(FileJob *work) {
    u16 idx = work->type;
    void *data = work->data;

    D_003E916C[idx].cb(data);
}

void fileJobInvokePositionCallback(void *work, void *extra) {
    u16 id = ((FileJob *)work)->type;

    if (D_003E9180[id].func != NULL) {
        D_003E9180[id].func(((FileJob *)work)->data, extra);
    }
}

void fileJobInvokeRotationCallback(void *work, void *extra) {
    u16 id = ((FileJob *)work)->type;

    if (D_003E9184[id].func != NULL) {
        D_003E9184[id].func(((FileJob *)work)->data, extra);
    }
}

void fileJobInvokeScaleCallback(void *work, f32 scale) {
    u16 id = ((FileJob *)work)->type;

    if (D_003E9188[id].func != NULL) {
        D_003E9188[id].func(((FileJob *)work)->data, scale);
    }
}

void fileDispatchJobTypeCallback(void *work, u32 color) {
    u16 id = ((FileJob *)work)->type;

    if (D_003E918C[id].func != NULL) {
        D_003E918C[id].func(((FileJob *)work)->data, color);
    }
}

void fileJobSetPrimaryData(job, src, size, option)
    FileJob *job;
    void *src;
    s32 size;
    u16 option;
{
    fileJobFreePrimaryBuffer(job);
    if (src != NULL && size > 0) {
        job->slots[0].allocation = (void *)sdfAllocGeneralBlock(size);
        job->slots[0].offset = sdfResourceRetainAddress(job->slots[0].allocation);
        job->slots[0].size = size;
        job->option = option;
        memcpy((void *)job->slots[0].offset, src, size);
    }
}

void fileJobCopyCommandIntoPrimaryData(FileJob *job, s32 commandId, u16 option) {
    DevState *command;
    s32 size;
    s32 handle;
    s32 address;

    command = sdfDevCreateCommandState(commandId);
    if (command != 0) {
        size = sdfDevQueueControlAndWait(command);
        handle = sdfAllocGeneralBlock(size);
        address = sdfResourceRetainAddress(handle);
        sdfDevQueueReadAndWait(command, (void *)address, size);
        sdfDevWaitThenReleaseCommandState(command);
        fileJobSetPrimaryData(job, (void *)address, size, option);
        sdfReleaseResourceAllocation(handle);
        return;
    }
}

void fileJobSetSecondaryData(job, src, size, selector)
    FileJob *job;
    void *src;
    s32 size;
    u16 selector;
{
    fileJobFreeSecondaryBuffer(job);
    if (src != NULL && size > 0) {
        job->slots[1].allocation = (void *)sdfAllocGeneralBlock(size);
        job->slots[1].offset = sdfResourceRetainAddress(job->slots[1].allocation);
        job->slots[1].size = size;
        job->slots[0].selector = selector;
        memcpy((void *)job->slots[1].offset, src, size);
    }
}

void fileJobCopyCommandIntoSecondaryData(FileJob *job, s32 commandId, u16 selector) {
    DevState *command;
    s32 size;
    s32 handle;
    s32 address;

    command = sdfDevCreateCommandState(commandId);
    if (command != 0) {
        size = sdfDevQueueControlAndWait(command);
        handle = sdfAllocGeneralBlock(size);
        address = sdfResourceRetainAddress(handle);
        sdfDevQueueReadAndWait(command, (void *)address, size);
        sdfDevWaitThenReleaseCommandState(command);
        fileJobSetSecondaryData(job, (void *)address, size, selector);
        sdfReleaseResourceAllocation(handle);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_002C96D0", func_002D3B48);

void fileWriteToPfs(s32 data, s32 slotIndex) {
    char path[0xD0];
    s32 fd;

    if (sdfPfsDebugMode != 0) {
        func_0035C860(path, D_00437E10, slotIndex);
        fd = func_00369B70(path, 0x602, 0x1B6);
    } else {
        func_0035C860(path, D_00437E18, sdfDevGetPathBuffer(), slotIndex);
        fd = func_00369B70(path, 0x602);
    }
    func_002D3B48(fd, data);
    func_00369DF8(fd);
    func_0036BCD0(D_00437E20, 0);
}

void *fileDuplicateJob(void *source) {
    FileJob *request = source;
    FileJob *job = fileCreateJob(request->type);

    if (request->slots[0].size != 0) {
        fileJobSetPrimaryData(job, fileResolvePrimaryBuffer(request), request->slots[0].size, request->option);
    }
    if (request->slots[1].size != 0) {
        fileJobSetSecondaryData(job, fileResolveSecondaryBuffer(request), request->slots[1].size, request->slots[0].selector);
    }
    return job;
}

/* No return on the path where no command state exists: retail hands back
 * whatever v0 held. */
void *fileJobCreateFromCommandState(entry)
    s32 entry;
{
    DevState *command;
    s32 size;
    s32 handle;
    s32 address;
    void *job;

    command = sdfDevCreateCommandState(entry);
    if (command != 0) {
        size = sdfDevQueueControlAndWait(command);
        handle = sdfAllocGeneralBlock(size);
        address = sdfResourceRetainAddress(handle);
        sdfDevQueueReadAndWait(command, (void *)address, size);
        sdfDevWaitThenReleaseCommandState(command);
        job = fileDuplicateJob((void *)address);
        sdfReleaseResourceAllocation(handle);
        return job;
    }
}

u32 fileJobSerializedSize(FileJob *job) {
    u32 size = 0x2C;

    if (fileResolvePrimaryBuffer(job) != NULL) {
        size = job->slots[0].size + 0x2C;
    }
    if (fileResolveSecondaryBuffer(job) != NULL) {
        u32 aligned = size >> 4;
        if ((size & 0xF) != 0) {
            aligned = (aligned + 1) << 4;
        } else {
            aligned = aligned << 4;
        }
        size = aligned + job->slots[1].size;
    }
    return size;
}

/* Initialize the shared transform prefix, leaving queue/job links untouched. */
void fileQueueInitTransform(void *queue)
{
    FileQueue *view = queue;

    memset(view, 0, 0x80);
    VU0_STORE_VF_UNCLOBBERED(vf0, view);
    VU0_STORE_VF_UNCLOBBERED(vf0, view->axis);
    VU0_STORE_VF_UNCLOBBERED(vf0, view->position);
    view->position[1] = -5.0f;
    VU0_STORE_VF_UNCLOBBERED(vf0, view->quat);
    view->scale = 1.0f;
    view->transformValue = 1.0f;
    view->color = 0x80808080;
    view->transformWord = 0x80;
}

void fileJobResetAndInitTransform(FileJob *job) {
    memset(job, 0, 0x90);
    job->scaleFlags = 1;
    job->unk88[0] = 8;
    job->unk88[1] = 0;
    job->unk88[2] = 0;
    fileQueueInitTransform(job);
}

void fileQueueAppend(FileQueue *queue, FileJob *job) {
    job->next = NULL;
    if (queue->last != NULL) {
        queue->last->next = job;
        job->prev = queue->last;
    } else {
        queue->first = job;
        job->prev = NULL;
    }
    queue->last = job;
    queue->count++;
}

void fileQueueInsertAfter(FileQueue *queue, FileJob *after, FileJob *job) {
    if (after->next != NULL) {
        after->next->prev = job;
        job->next = after->next;
    } else {
        job->next = NULL;
        queue->last = job;
    }
    after->next = job;
    job->prev = after;
    queue->count++;
}

void fileQueueRemove(FileQueue *queue, FileJob *job) {
    if (job->prev != NULL) {
        job->prev->next = job->next;
    } else {
        queue->first = job->next;
    }
    if (job->next != NULL) {
        job->next->prev = job->prev;
    } else {
        queue->last = job->prev;
    }
    queue->count--;
}

FileQueue *fileQueueCreate(void) {
    FileQueue *queue = sdfAllocSizeClassBlock(0x90);
    memset(queue, 0, 0x90);
    queue->count = 0;
    queue->unk84 = 0;
    fileQueueInitTransform(queue);
    return queue;
}

FileJob *fileJobCreate(void) {
    FileJob *job = sdfAllocSizeClassBlock(0xC0);
    memset(job, 0, 0xC0);
    fileJobResetAndInitTransform(job);
    return job;
}

void fileDestroyJob(FileJob *job) {
    sdfReleaseChipBlock();
}

FileQueue *fileCloneQueueEntries(FileQueue *source) {
    FileQueue *queue = fileQueueCreate();
    FileJob *entry;
    FileJob *job;
    s128 vec;

    PCP_COPY_VECTOR(queue->offset, source->offset);
    PCP_COPY_VECTOR(queue->axis, source->offset);
    queue->transformValue = source->transformValue;
    queue->transformWord = source->transformWord;
    if (source->first != NULL) {
        for (entry = source->first; entry != NULL; entry = entry->next) {
            job = fileJobCreate();

            if ((entry->flags & 1) == 0) {
                job->id = (u32)fileJobCreateFromJob((FileJob *)entry->id);
            } else {
                FileJob *parent = fileQueueFindById(source, entry->id);
                s32 index = fileFindQueuedJobIndex(source, parent);

                parent = fileQueueGetAt(queue, index);
                job->id = (u32)fileJobCreateChild((FileJob *)parent->id);
            }
            fileJobCopyHeader(job, entry);
            strcpy(job->name, entry->name);
            fileQueueAppend(queue, job);
        }
    } else {
        FileJob *records;

        entry = (FileJob *)((u8 *)source + (u32)source->last);
        records = entry;
        if (source->count > 0) {
            s32 count = source->count;

            do {
                job = fileJobCreate();

                if ((entry->flags & 1) == 0) {
                    FileJob *request = (FileJob *)((u8 *)source + entry->id);

                    if (entry->flags & 2) {
                        FileJob *secondary = (FileJob *)((u8 *)source + records[entry->sector].id);

                        request->slots[1].size = secondary->slots[1].size;
                        request->slots[1].offset = (u32)fileResolveSecondaryBuffer(secondary) - (u32)request;
                        job->id = (u32)fileJobCreateFromJob(request);
                        request->slots[1].offset = request->slots[1].size = 0;
                    } else {
                        job->id = (u32)fileJobCreateFromJob(request);
                    }
                } else {
                    FileJob *parent = fileQueueGetAt(queue, entry->id);

                    job->id = (u32)fileJobCreateChild((FileJob *)parent->id);
                }
                fileJobCopyHeader(job, entry);
                strcpy(job->name, entry->name);
                fileQueueAppend(queue, job);
                entry++;
            } while (--count != 0);
        }
    }
    VU0_STORE_VF(vf0, &vec);
    fileQueueSetPosition(queue, &vec);
    fileQueueSetRotation(queue, &vec);
    fileQueueSetScale(queue, 1.0f);
    func_002D49B8(queue, 0x80808080);
    return queue;
}

void func_002D4380(u32 unused, u32 job) {
    fileCloneQueueEntries((FileQueue *)job);
}

extern void camFollowOffsetVec();
extern void camAimRotation();

/* Per-frame update: refreshes the queue rotation when an aim flag (0x60) is set, then repositions and re-notifies every job whose start frame (job+0x80) has been reached; the frame counter only advances when effModelUpdateControlFlags bit 1 is clear. */
void fileQueueUpdate(FileQueue *queue)
{
    f32 pos[4];
    f32 aimQuat[4];
    f32 savedQuat[4];
    FileJob *job;
    s32 limit;
    f32 total;

    if (queue->transformWord & 0x60) {
        PCP_COPY_VECTOR(savedQuat, queue->quat);
        camAimRotation(queue, aimQuat);
        fileQueueSetRotation(queue, aimQuat);
        PCP_COPY_VECTOR(queue->quat, savedQuat);
    }
    total = queue->scale * queue->transformValue;
    limit = queue->unk84;
    for (job = queue->first; job != NULL; job = job->next) {
        if (limit < job->unk80) {
            continue;
        }
        if (job->scaleFlags & 2) {
            continue;
        }
        if (job->xformFlags & 0x18) {
            camFollowOffsetVec(job, pos);
            VU0_LOAD_VF(vf10, queue->quat);
            effMiscQuaternionToMatrixVU();
            VU0_LOAD_VF(vf10, queue->position);
            VU0_LOAD_VF(vf11, queue->offset);
            VU0_ADD(vf10, vf10, vf11);
            if (job->xformFlags & 4) {
                VU0_SCALAR_OP(-5.0f, "vaddx.y vf10, vf0, vf2x");
            }
            VU0_LOAD_VF(vf11, pos);
            if (job->xformFlags & 0x80) {
                VU0_SCALE_VF(vf11, total);
            }
            VU0_ADD(vf10, vf10, vf11);
            VU0_STORE_VF_UNCLOBBERED(vf10, pos);
            fileJobInvokePositionCallback((FileJob *)job->id, pos);
        }
        if (job->xformFlags & 0x60) {
            camAimRotation(job, aimQuat);
            fileJobInvokeRotationCallback((FileJob *)job->id, aimQuat);
        }
        fileJobInvokeTypeCallback((FileJob *)job->id);
    }
    if ((effModelUpdateControlFlags & 2) == 0) {
        queue->unk84++;
    }
}

/* A queued job's +0x90 word is a child pointer when flag 1 is clear; it is
 * an ID for other job kinds. Destroy children before their parent jobs. */
void fileQueueDestroy(FileQueue *queue) {
    FileJob *job = queue->first;
    while (job != NULL) {
        FileJob *next = job->next;
        if ((job->flags & 1) == 0) {
            fileJobDestroy((FileJob *)job->id);
        }
        fileDestroyJob(job);
        job = next;
    }
    sdfReleaseChipBlock(queue);
}

FileQueue *fileQueueClone(FileQueue *source) {
    FileQueue *queue = fileQueueCreate();
    FileJob *src;
    s128 vec;

    PCP_COPY_VECTOR(queue, source);
    PCP_COPY_VECTOR(queue->axis, source->axis);
    queue->transformValue = source->transformValue;
    queue->transformWord = source->transformWord;
    for (src = source->first; src != NULL; src = src->next) {
        FileJob *job = fileJobCreate();
        job->id = (u32)fileJobCreateChild((FileJob *)src->id);
        fileJobCopyHeader(job, src);
        fileQueueAppend(queue, job);
    }
    VU0_STORE_VF(vf0, &vec);
    fileQueueSetPosition(queue, &vec);
    fileQueueSetRotation(queue, &vec);
    fileQueueSetScale(queue, 1.0f);
    func_002D49B8(queue, 0x80808080);
    return queue;
}

void fileQueueNotifyAllJobsComplete(u8 *owner) {
    u8 *job = (u8 *)((FileQueue *)owner)->first;
    while (job != 0) {
        fileJobNotifyComplete(((FileJob *)job)->id);
        job = (u8 *)((FileJob *)job)->next;
    }
    ((FileQueue *)owner)->unk84 = 0;
}

void fileQueueSetPosition(FileQueue *queue, void *vec)
{
    f32 rot[16];
    f32 base[4];
    f32 pos[4];
    FileJob *job;
    f32 scale;

    VU0_LOAD_VF(vf10, vec);
    VU0_STORE_VF(vf10, queue->position);
    VU0_LOAD_VF(vf11, queue->offset);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, base);
    VU0_LOAD_VF(vf10, queue->quat);
    effMiscQuaternionToMatrixVU();
    VU0_STORE_MATRIX(rot);
    scale = queue->scale * queue->transformValue;
    for (job = queue->first; job != NULL; job = job->next) {
        VU0_LOAD_VF(vf10, base);
        if (job->xformFlags & 4) {
            VU0_SCALAR_OP(-5.0f, "vaddx.y vf10, vf0, vf2x");
        }
        VU0_LOAD_VF(vf11, job->offset);
        if (job->xformFlags & 0x80) {
            VU0_SCALE_VF(vf11, scale);
        }
        VU0_LOAD_MATRIX(rot);
        VU0_ROTATE_VEC(vf11, vf11);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF_UNCLOBBERED(vf10, pos);
        fileJobInvokePositionCallback((FileJob *)job->id, pos);
    }
}

void fileQueueSetRotation(FileQueue *queue, void *rot)
{
    f32 quat[4];
    f32 pos[4];
    FileJob *job;

    VU0_LOAD_VF(vf10, rot);
    VU0_STORE_VF(vf10, queue->quat);
    VU0_STORE_VF(vf10, quat);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, queue->axis);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_STORE_VF(vf10, queue->offset);
    for (job = queue->first; job != NULL; job = job->next) {
        VU0_LOAD_VF(vf10, job->quat);
        VU0_LOAD_VF(vf11, quat);
        effMiscQuatMultiplyVU();
        VU0_STORE_VF_UNCLOBBERED(vf10, pos);
        fileJobInvokeRotationCallback((FileJob *)job->id, pos);
    }
    fileQueueSetPosition(queue, queue->position);
}

void fileQueueSetScale(FileQueue *queue, f32 scale)
{
    s128 pos;
    FileJob *job;
    f32 total;
    f32 jobScale;

    queue->scale = scale;
    total = scale * queue->transformValue;
    for (job = queue->first; job != NULL; job = job->next) {
        jobScale = job->scale;
        if (job->scaleFlags & 1) {
            jobScale = jobScale * total;
        }
        fileJobInvokeScaleCallback((FileJob *)job->id, jobScale);
        if (job->xformFlags & 0x80) {
            VU0_LOAD_VF(vf10, queue->position);
            VU0_LOAD_VF(vf11, queue->offset);
            VU0_ADD(vf10, vf10, vf11);
            if (job->xformFlags & 4) {
                VU0_SCALAR_OP(-5.0f, "vaddx.y vf10, vf0, vf2x");
            }
            VU0_LOAD_VF(vf11, job->offset);
            VU0_SCALAR_OP(total, "vmulx.xyzw vf11, vf11, vf2x");
            VU0_ADD(vf10, vf10, vf11);
            VU0_STORE_VF_UNCLOBBERED(vf10, &pos);
            fileJobInvokePositionCallback((FileJob *)job->id, &pos);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_002C96D0", func_002D49B8);

void fileReadVector40(void *work, void *dst) {
    PCP_COPY_VECTOR(dst, ((FileQueue *)work)->position);
}

void fileReadStoredQuaternion(void *work, void *dst) {
    PCP_COPY_VECTOR(dst, ((FileQueue *)work)->quat);
}

f32 fileGetQueueScale(FileQueue *queue) {
    return queue->scale;
}

u32 fileGetQueueColor(FileQueue *queue) {
    return queue->color;
}

void fileQueueCopyRotationFromSource(void *dst, void *src) {
    s128 vec;
    sdfVuMatrixToQuaternion(src);
    VU0_STORE_VF(vf10, &vec);
    fileQueueSetRotation(dst, &vec);
}

extern void fileQueueAppend(FileQueue *queue, FileJob *job);

FileJob *fileAppendJob(FileQueue *queue, u32 id) {
    FileJob *job = fileJobCreate();

    job->id = id;
    fileQueueAppend(queue, job);
    return job;
}

FileJob *fileDuplicateAndAppendJob(FileQueue *queue, void *source) {
    void *job = fileDuplicateJob(source);
    return fileAppendJob(queue, (u32)job);
}

extern u8 D_00437E28[];

FileJob *fileAppendJobFromEntry(FileQueue *queue, void *entry) {
    func_0035B6E0(D_00437E28);
    return fileAppendJob(queue, (u32)fileJobCreateFromCommandState(entry));
}

FileJob *fileJobDuplicateAfter(FileQueue *queue, FileJob *src) {
    FileJob *job = fileJobCreate();

    memcpy(job, src, 0xC0);
    fileJobResetAndInitTransform(job);
    job->flags |= 1;
    job->id = src->id;
    fileQueueInsertAfter(queue, src, job);
    return job;
}

extern FileJob *fileQueueFindBySector(FileQueue *queue, u32 sector);
extern FileJob *fileQueueFindFlaggedById(FileQueue *queue, u32 id);
extern void fileQueueLinkJobToSectorLeader(FileQueue *queue, FileJob *job, FileJob *ref);

/* Makes the first job chained to owner's sector the leader and rechains the rest to it. */
static inline void fileQueueRechainSectorFollowers(FileQueue *queue, FileJob *owner, FileJob *leader) {
    FileJob *next;

    leader->sector = 0;
    leader->flags &= ~2;
    fileQueueLinkJobToSectorLeader(queue, leader, leader);
    next = fileQueueFindBySector(queue, owner->id);
    while (next != NULL) {
        fileQueueLinkJobToSectorLeader(queue, next, leader);
        next = fileQueueFindBySector(queue, owner->id);
    }
}

void fileQueueLinkJobToSectorLeader(FileQueue *queue, FileJob *job, FileJob *ref) {
    FileJob *node;
    FileJob *found;
    FileJob *source;
    u32 refIndex;
    u32 flags;

    if (!(job->flags & 1)) {
        for (node = queue->first; node != NULL; node = node->next) {
            if ((node->flags & 1) && node->id == job->id) {
                fileQueueLinkJobToSectorLeader(queue, node, ref);
            }
        }
    }
    if (job == ref) {
        return;
    }
    flags = job->flags;
    if (!(flags & 1)) {
        fileJobFreeSecondaryBuffer((FileJob *)job->id);
        source = (FileJob *)ref->id;
        fileJobSetSecondaryData((FileJob *)job->id, (void *)source->slots[1].offset, source->slots[1].size,
                                source->slots[0].selector);
        flags = job->flags;
    }
    job->flags = flags | 2;
    job->sector = ref->id;
    refIndex = fileFindQueuedJobIndex(queue, ref);
    if (fileFindQueuedJobIndex(queue, job) < refIndex) {
        found = fileQueueFindBySector(queue, ref->id);
        if (found != NULL) {
            fileQueueRechainSectorFollowers(queue, ref, found);
            fileQueueLinkJobToSectorLeader(queue, ref, found);
        }
    }
}

void fileQueueDetachSectorFollower(FileQueue *queue, FileJob *job) {
    u32 flags = job->flags;
    FileJob *first;

    job->flags = flags & ~2;
    if (!(flags & 1)) {
        first = fileQueueFindBySector(queue, job->id);
        if (first != NULL) {
            fileQueueRechainSectorFollowers(queue, job, first);
        }
    }
    job->sector = 0;
}

void fileQueueRemoveAndDestroyJob(FileQueue *queue, FileJob *job) {
    FileJob *first;

    fileQueueRemove(queue, job);
    if (!(job->flags & 1)) {
        first = fileQueueFindFlaggedById(queue, job->id);
        while (first != NULL) {
            fileQueueRemoveAndDestroyJob(queue, first);
            first = fileQueueFindFlaggedById(queue, job->id);
        }
        if (!(job->flags & 2)) {
            first = fileQueueFindBySector(queue, job->id);
            if (first != NULL) {
                fileQueueRechainSectorFollowers(queue, job, first);
            }
        }
        if (!(job->flags & 1)) {
            fileJobDestroy((FileJob *)job->id);
        }
    }
    fileDestroyJob(job);
}

void fileJobCopyHeader(FileJob *dst, FileJob *src) {
    memcpy(dst, src, 0x90);
}

INCLUDE_ASM(const s32, "game/code_002C96D0", func_002D50D8);

INCLUDE_ASM(const s32, "game/code_002C96D0", func_002D55B0);

INCLUDE_ASM(const s32, "game/code_002C96D0", func_002D5AA8);

FileJob *fileQueueFindById(FileQueue *queue, u32 id) {
    FileJob *job = queue->first;
    while (job != NULL) {
        if (job->id == id) {
            return job;
        }
        job = job->next;
    }
    return NULL;
}

FileJob *fileQueueFindFlaggedById(FileQueue *queue, u32 id) {
    FileJob *job = queue->first;
    while (job != NULL) {
        if ((job->flags & 1) != 0 && job->id == id) {
            return job;
        }
        job = job->next;
    }
    return NULL;
}

FileJob *fileQueueFindBySector(FileQueue *queue, u32 sector) {
    FileJob *job = queue->first;
    while (job != NULL) {
        if ((job->flags & 3) == 2 && job->sector == sector) {
            return job;
        }
        job = job->next;
    }
    return NULL;
}

FileJob *fileQueueGetAt(FileQueue *queue, s32 index) {
    FileJob *job = queue->first;
    while (job != NULL) {
        if (index-- == 0) {
            return job;
        }
        job = job->next;
    }
    return NULL;
}

s32 fileFindQueuedJobIndex(FileQueue *queue, FileJob *target) {
    FileJob *job = queue->first;
    s32 index = 0;
    while (job != NULL) {
        if (job == target) {
            return index;
        }
        job = job->next;
        index++;
    }
    return 0;
}

s32 fileQueueCountLinkedJobs(FileQueue *queue) {
    FileJob *job;
    s32 count;

    count = 0;
    for (job = queue->first; job != 0; job = job->next) {
        count = count + 1;
    }
    return count;
}


typedef struct CamFollow {
    u8 pad00[0x40];
    f32 pos[4];
    u8 pad50[0x18];
    u32 flags;
} CamFollow;

/* vu0 routine: dst = |pos| * unit(sdfViewTargetVector - sdfViewEyeVector); flag 0x10 flattens y, then puts pos.y back */
void camFollowOffsetVec(CamFollow *obj, void *dst)
{
    f32 len[4];
    f32 length;
    u32 flags = obj->flags;

    if ((flags & 0x18) == 0) {
        PCP_COPY_VECTOR(dst, obj->pos);
        return;
    }
    VU0_LOAD_VF(vf10, obj->pos);
    if (obj->flags & 0x10) {
        VU0_SCALAR_OP(0.0f, "vaddx.y vf10, vf0, vf2x");
    }
    VU0_LENGTH_VF10(length);
    VEC3_SPLAT(len, length);
    VU0_LOAD_VF(vf10, sdfViewTargetVector);
    VU0_LOAD_VF(vf11, sdfViewEyeVector);
    VU0_SUB(vf10, vf10, vf11);
    if (obj->flags & 0x10) {
        VU0_SCALAR_OP(0.0f, "vaddx.y vf10, vf0, vf2x");
    }
    VU0_NORMALIZE_VF10();
    VU0_LOAD_VF(vf11, len);
    VU0_MUL(vf10, vf10, vf11);
    if (obj->flags & 0x10) {
        VU0_SCALAR_OP(obj->pos[1], "vaddx.y vf10, vf0, vf2x");
    }
    VU0_STORE_VF(vf10, dst);
}

typedef struct CamAim {
    u8 pad00[0x50];
    f32 quat[4];
    u8 pad60[8];
    u32 flags;
} CamAim;

/* dst = rotation facing unit(sdfViewTargetVector - sdfViewEyeVector) (pitch and yaw); flag 0x40 composes it onto obj->quat, no flag 0x60 copies obj->quat */
void camAimRotation(CamAim *obj, void *dst)
{
    f32 v[4];
    f32 pitch;
    u32 flags = obj->flags;

    if ((flags & 0x60) == 0) {
        PCP_COPY_VECTOR(dst, obj->quat);
        return;
    }
    VU0_LOAD_VF(vf10, sdfViewTargetVector);
    VU0_LOAD_VF(vf11, sdfViewEyeVector);
    VU0_SUB(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, v);
    pitch = 0.0f;
    if ((flags & 0x40) == 0) {
        pitch = -func_003532B8(-v[1]);
    }
    func_00340DC8(pitch, func_003532E8(v[0], v[2]), 0.0f);
    if (obj->flags & 0x40) {
        VU0_MOVE_VF(vf11, vf10);
        VU0_LOAD_VF(vf10, obj->quat);
        effMiscQuatMultiplyVU();
    }
    VU0_STORE_VF(vf10, dst);
}

extern void *sdfAllocAndClearQuadwords(s32);

typedef struct FileSlot {
    f32 pos[4];       /* 0x00 */
    s32 state;        /* 0x10: frame, -1 available, -2 disabled */
    u32 color;        /* 0x14 */
    f32 scale;        /* 0x18 */
    f32 angle;        /* 0x1C */
} FileSlot;      /* 0x20, ordinary 4-byte field alignment */

typedef struct FileSlotTable {
    u16 type;
    u8 pad02[2];
    u32 instances;         /* 0x04: target's primary-slot count */
    u32 count;             /* 0x08: primary + trailing group cells */
    u32 flags;             /* 0x0C */
    u32 references;        /* 0x10: acquisition/frame counter */
    f32 spawnRemainder;    /* 0x14 */
    FileSlot *slots; /* 0x18 */
    u8 *unk1C;             /* 0x1C: per-type instance work */
    u8 *data0;             /* 0x20 */
    u8 *data1;             /* 0x24 */
    u32 handle;            /* 0x28 */
} FileSlotTable;

/* The record's signed +0x54 mode is consumed by every billboard opener. */
typedef struct FileBillboardRecord {
    u8 pad0[0x54];
    s16 mode; /* 0x54 */
} FileBillboardRecord;

EffectSurfaceNode *fileCreateSurfaceLoaderState(s32 capacity) {
    EffectSurfaceNode *rec = (EffectSurfaceNode *)sdfAllocAndClearQuadwords(sizeof(EffectSurfaceNode));
    u32 color = 0x80808080;

    rec->capacity = capacity;
    rec->unk50 = 1;
    rec->color = color;
    rec->scale = 1.0f;
    rec->resource = NULL;
    rec->jobs = NULL;
    rec->jobHandle = 0;
    rec->queues = NULL;
    rec->queueHandle = 0;
    rec->active = 0;
    return rec;
}

typedef struct FileGridHeader {
    u8 pad00[0x20];
    s32 rows;   /* 0x20 */
    s32 cols;   /* 0x24 */
    u8 pad28[0x90];
    s32 altCols; /* 0xB8 */
} FileGridHeader;

EffectSurfaceNode *fileCreateGridLoaderRecord(FileGridHeader *hdr) {
    u32 rows = hdr->rows;
    u32 count = (rows != 0 ? rows : hdr->cols) * (rows != 0 ? hdr->cols : hdr->altCols);

    return fileCreateSurfaceLoaderState(count <= 0x12C ? count : 0x12C);
}



EffectSurfaceNode *fileCreateEffectSurfaceFromJob(FileJob *job) {
    void *primary = fileResolvePrimaryBuffer(job);
    EffectSurfaceNode *node = fileCreateGridLoaderRecord(primary);
    void *secondary;

    fileLoadObjectSetResource(node, job->option, primary);
    secondary = fileResolveSecondaryBuffer(job);
    if (secondary != NULL) {
        switch (job->slots[0].selector) {
        case 1:
            fileLoadObjectOpenDevice(node, (u32)secondary);
            break;
        case 2:
            fileLoadObjectOpenAndStartDevice(node, (u32)secondary);
            break;
        case 4:
            fileLoadObjectOpenNamedDevice(node, *(u32 *)secondary);
            break;
        case 5:
            fileReplaceEffectSurfaceJobs(node, secondary);
            break;
        case 6:
            fileReplaceEffectSurfaceQueues(node, secondary);
            break;
        case 7:
            fileReplaceReferenceHolder(node, (u32)secondary);
            break;
        }
        node->kind = job->slots[0].selector;
    }
    return node;
}

void fileDestroyEffectSurfaceAndChildren(EffectSurfaceNode *node) {
    u32 count;
    u32 i;

    if (node->resource != NULL) {
        billDispatchByKind(node->resource);
    }
    if (node->jobHandle != 0) {
        count = ((FileSlotTable *)node->active)->count;
        for (i = 0; i < count; i++) {
            fileJobDestroy(node->jobs[i]);
        }
        sdfReleaseResourceAllocation(node->jobHandle);
    }
    if (node->queueHandle != 0) {
        count = ((FileSlotTable *)node->active)->count;
        for (i = 0; i < count; i++) {
            fileQueueDestroy(node->queues[i]);
        }
        sdfReleaseResourceAllocation(node->queueHandle);
    }
    if (node->referenceHolder != NULL) {
        effReleaseReferenceHolder((s32)node->referenceHolder);
    }
    if (node->active != 0) {
        fileReleaseGridRecordHandle((FileSlotTable *)node->active);
    }
    sdfReleaseChipBlock(node);
}

EffectSurfaceNode *fileLoadObjectCreateChild(EffectSurfaceNode *owner) {
    FileGridHeader *source = (FileGridHeader *)((FileSlotTable *)owner->active)->data1;
    EffectSurfaceNode *result = fileCreateGridLoaderRecord(source);

    fileLoadObjectSetResource(result, ((FileSlotTable *)owner->active)->type, source);
    fileCloneEffectSurfaceResources(result, owner);
    return result;
}

void fileCloneEffectSurfaceResources(EffectSurfaceNode *dst, EffectSurfaceNode *src) {
    u32 count;
    s32 size;
    u32 i;

    switch (src->kind) {
    case 1:
    case 2:
    case 4:
        if (dst->resource != NULL) {
            billDispatchByKind(dst->resource);
        }
        dst->resource = (void *)billCloneObjectRetainingSharedData((u32)src->resource);
        billMarkKindOneFlag((u32)dst->resource);
        if (dst->active != 0) {
            billSetBillboardMode((u32)dst->resource, ((FileBillboardRecord *)((FileSlotTable *)dst->active)->data0)->mode);
        }
        break;
    case 5:
        count = ((FileSlotTable *)src->active)->count;
        if (count == 0) {
            return;
        }
        if (dst->jobHandle != 0) {
            for (i = 0; i < count; i++) {
                fileJobDestroy(dst->jobs[i]);
            }
            sdfReleaseResourceAllocation(dst->jobHandle);
            dst->jobs = 0;
            dst->jobHandle = 0;
        }
        size = count * 4;
        if (size == 0) {
            return;
        }
        dst->jobHandle = sdfAllocGeneralBlock(size);
        dst->jobs = sdfResourceRetainAddress(dst->jobHandle);
        for (i = 0; i < count; i++) {
            dst->jobs[i] = fileJobCreateChild(src->jobs[0]);
        }
        break;
    case 6:
        count = ((FileSlotTable *)src->active)->count;
        if (count == 0) {
            return;
        }
        if (dst->queueHandle != 0) {
            for (i = 0; i < count; i++) {
                fileQueueDestroy(dst->queues[i]);
            }
            sdfReleaseResourceAllocation(dst->queueHandle);
            dst->queues = 0;
            dst->queueHandle = 0;
        }
        size = count * 4;
        if (size == 0) {
            return;
        }
        dst->queueHandle = sdfAllocGeneralBlock(size);
        dst->queues = sdfResourceRetainAddress(dst->queueHandle);
        for (i = 0; i < count; i++) {
            dst->queues[i] = fileQueueClone(src->queues[0]);
        }
        break;
    case 7:
        if (dst->referenceHolder != NULL) {
            effReleaseReferenceHolder((s32)dst->referenceHolder);
        }
        dst->referenceHolder = (void *)effReferenceObjectRetain((u32)src->referenceHolder);
        break;
    }
    dst->kind = src->kind;
}

void fileLoadObjectSetResource(EffectSurfaceNode *node, u32 entryId, void *resource) {
    if (node->active != 0) {
        fileReleaseGridRecordHandle((FileSlotTable *)node->active);
    }
    node->active = fileAllocateGridRecordSlots((u16)entryId, node->capacity, resource);
}

void fileLoadObjectOpenNamedDevice(EffectSurfaceNode *node, u32 resourceId) {
    u32 resource = node->resource;
    if (resource != 0) {
        billDispatchByKind((void *)resource);
    }
    resource = effRetainResource(resourceId);
    node->resource = (void *)resource;
    if (node->active != 0) {
        billSetBillboardMode(resource, ((FileBillboardRecord *)((FileSlotTable *)node->active)->data0)->mode);
    }
}

void fileLoadObjectOpenDevice(EffectSurfaceNode *node, u32 resourceId) {
    u32 resource = node->resource;
    if (resource != 0) {
        billDispatchByKind((void *)resource);
    }
    resource = billCreateIndexed(0, resourceId);
    node->resource = (void *)resource;
    if (node->active != 0) {
        billSetBillboardMode(resource, ((FileBillboardRecord *)((FileSlotTable *)node->active)->data0)->mode);
    }
}

void fileLoadObjectOpenAndStartDevice(EffectSurfaceNode *node, u32 resourceId) {
    u32 resource = node->resource;
    if (resource != 0) {
        billDispatchByKind((void *)resource);
    }
    resource = billCreateIndexed(1, resourceId);
    node->resource = (void *)resource;
    billMarkKindOneFlag(resource);
    if (node->active != 0) {
        billSetBillboardMode(node->resource, ((FileBillboardRecord *)((FileSlotTable *)node->active)->data0)->mode);
    }
}

void fileReplaceEffectSurfaceJobs(EffectSurfaceNode *node, FileJob *job) {
    u32 count = ((FileSlotTable *)node->active)->count;
    u32 i;
    s32 size;

    if (node->jobHandle != 0) {
        for (i = 0; i < count; i++) {
            fileJobDestroy(node->jobs[i]);
        }
        sdfReleaseResourceAllocation(node->jobHandle);
        node->jobs = 0;
        node->jobHandle = 0;
    }
    size = count * 4;
    if (size != 0) {
        node->jobHandle = sdfAllocGeneralBlock(size);
        node->jobs = sdfResourceRetainAddress(node->jobHandle);
        node->jobs[0] = fileJobCreateFromJob(job);
        for (i = 1; i < count; i++) {
            node->jobs[i] = fileJobCreateChild(node->jobs[0]);
        }
    }
}

void fileReplaceEffectSurfaceQueues(EffectSurfaceNode *node, FileJob *job) {
    u32 count = ((FileSlotTable *)node->active)->count;
    u32 i;
    s32 size;

    if (node->queueHandle != 0) {
        for (i = 0; i < count; i++) {
            fileQueueDestroy(node->queues[i]);
        }
        sdfReleaseResourceAllocation(node->queueHandle);
        node->queues = 0;
        node->queueHandle = 0;
    }
    size = count * 4;
    if (size != 0) {
        node->queueHandle = sdfAllocGeneralBlock(size);
        node->queues = sdfResourceRetainAddress(node->queueHandle);
        node->queues[0] = fileCloneQueueEntries((FileQueue *)job);
        for (i = 1; i < count; i++) {
            node->queues[i] = fileQueueClone(node->queues[0]);
        }
    }
}

void fileReplaceReferenceHolder(EffectSurfaceNode *obj, u32 resource) {
    u32 holder;

    if (obj->referenceHolder != NULL) {
        effReleaseReferenceHolder((s32)obj->referenceHolder);
    }
    holder = func_002DDF48(resource);
    obj->referenceHolder = (void *)holder;
}

void fileClearLoadObjectReferences(EffectSurfaceNode *obj) {
    if (obj->active != 0) {
        fileClearRecordReferences((FileSlotTable *)obj->active);
        return;
    }
}

/* The vector callers leave their second argument inherited through the
   unprototyped declarations and the K&R definitions below. */
extern void fileAcquireRecord();
extern void mnuRecordSetVector();
extern void fileSetRecordSecondVector();

/* Original-style implicit int; callers ignore its result. */
fileAcquireLoadObjectRecord(EffectSurfaceNode *obj) {
    if ((effModelUpdateControlFlags & 2) == 0 && obj->active != 0) {
        fileAcquireRecord((FileSlotTable *)obj->active);
    }
}

INCLUDE_ASM(const s32, "game/code_002C96D0", func_002D69B8);

void func_002D7398(u32 objectAddr) {
    fileAcquireLoadObjectRecord((EffectSurfaceNode *)objectAddr);
    func_002D69B8(objectAddr);
}

void fileSendLoadObjectRecordVector(EffectSurfaceNode *obj) {
    mnuRecordSetVector(obj->active);
}

void fileCopyLoadObjectRecordVector(EffectSurfaceNode *obj) {
    fileSetRecordSecondVector(obj->active);
}

void fileSetRecordWordFour(s32 record, u32 value) {
    *(u32 *)(record + 4) = value;
}

void fileSetLoadObjectScale(EffectSurfaceNode *obj, f32 scale) {
    obj->scale = scale;
    dds3DispatchIndexedCallback((FileSlotTable *)obj->active);
}


typedef struct FileRecordType {
    void (*acquire)(void *);
    u32 unk4;
    s32 slotBytes;
    s32 dataBytes;
} FileRecordType;

typedef struct FileGridDimensions {
    u8 pad0[0xC0];
    s32 columns;
    s32 rows;
} FileGridDimensions;

extern FileRecordType D_003E95C0[];

void fileResetSlotStates(FileSlotTable *table) {
    u32 count;
    FileSlot *slot;
    u32 index;

    count = table->count;
    index = 0;
    slot = table->slots;
    if (count != 0) {
        do {
            index = index + 1;
            slot->state = 0xffffffff;
            slot++;
        } while (index < count);
    }
}

INCLUDE_ASM(const s32, "game/code_002C96D0", func_002D7458);

f32 func_002D7770(EffScalarCurve *curve, s32 frame, s32 length) {
    f32 from, to, factor, duration;
    s32 firstFrame, secondFrame;
    if (length == 0) {
        return curve->initialValue;
    }
    duration = length;
    switch (curve->mode) {
    case 0:
        factor = (f32)frame / duration;
        from = curve->initialValue;
        to = curve->finalValue;
        break;
    case 1:
        firstFrame = (s32)(curve->firstFraction * duration);
        if (frame < firstFrame) {
            factor = (f32)frame / firstFrame;
            from = curve->initialValue;
            to = curve->firstValue;
        } else {
            f32 span = length - firstFrame;
            factor = (f32)(frame - firstFrame) / span;
            from = curve->firstValue;
            to = curve->finalValue;
        }
        break;
    case 2:
        firstFrame = (s32)(curve->firstFraction * duration);
        if (frame < firstFrame) {
            factor = (f32)frame / firstFrame;
            from = curve->initialValue;
            to = curve->firstValue;
        } else {
            secondFrame = (s32)(curve->secondFraction * duration);
            if (frame < secondFrame) {
                f32 span = secondFrame - firstFrame;
                factor = (f32)(frame - firstFrame) / span;
                from = curve->firstValue;
                to = curve->secondValue;
            } else {
                f32 span = length - secondFrame;
                factor = (f32)(frame - secondFrame) / span;
                from = curve->secondValue;
                to = curve->finalValue;
            }
        }
        break;
    default:
        from = curve->initialValue;
        to = curve->finalValue;
        factor = 0.0f;
        break;
    }
    return from + (to - from) * factor;
}


/* Output of fileSampleKeyTracks: a view-space position, the sampled frame, colour, scale and heading. */
typedef FileSlot FileKeyOut;

/* One emitter track uses +0x0C as its random multiplier; the curve sampler
 * treats the same word as reserved. */
typedef union FileKeyScalarTrack {
    EffScalarTrack track;
    struct {
        u8 mode;
        u8 reserved01[3];
        f32 initialValue;
        f32 finalValue;
        f32 randomness;
        u8 headingMode;
        u8 reserved11[3];
        f32 firstValue;
        f32 firstFraction;
        f32 secondValue;
        f32 secondFraction;
        u8 reserved24[8];
    } emitter;
} FileKeyScalarTrack;

/* Keyframe tracks of a view block (scale, heading and colour curves). */
typedef struct FileKeyBlock {
    f32 pos[4];             /* 0x00 */
    f32 orientation[4];    /* 0x10: emitter quaternion */
    s32 emissionDuration;   /* 0x20 */
    u32 spawnRate;          /* 0x24 */
    f32 spawnVariance;      /* 0x28 */
    u8 unk2C[0x24];         /* 0x2C: color track */
    u8 unk50[0x10];         /* 0x50: color data */
    FileKeyScalarTrack scale;   /* 0x60 */
    FileKeyScalarTrack heading; /* 0x8C */
    s32 length;            /* 0xB8 */
    u8 padBC;              /* 0xBC: allocator's relative-position flag */
    u8 prewarm;            /* 0xBD */
    u8 padBE[0x0A];         /* includes grid columns/rows at C0/C4 */
    union {                /* 0xC8: record-type-specific emitter parameters */
        struct {
            f32 radius;
            f32 radiusRandomness;
            f32 speed;
            f32 speedRandomness;
            f32 acceleration;
            f32 gravity;
        } radial;
        struct {
            f32 radius;
            f32 radiusRandomness;
            f32 spread;
            f32 spreadRandomness;
            f32 speed;
            f32 speedRandomness;
            f32 acceleration;
            f32 gravity;
        } directed;
        struct {
            f32 initialRadius;
            f32 axialSpeed;
            f32 axialSpeedRandomness;
            f32 axialDeceleration;
            f32 initialAmplitude;
            f32 initialAmplitudeRandomness;
            f32 finalAmplitude;
            f32 finalAmplitudeRandomness;
            f32 phaseStep;
            f32 phaseStepRandomness;
        } wave;
    } emitter;
} FileKeyBlock;             /* record-type-dependent parameter extent */

extern s32 func_002D7458(void *, void *, s32, s32);
extern f32 func_002D7770(EffScalarCurve *, s32, s32);

/* vu0 routine: samples the colour, scale and heading tracks at frame; in mode 2 the heading is the screen-space direction from out->pos to target (0 when they coincide) */
void fileSampleKeyTracks(FileKeyOut *out, FileKeyBlock *block, s32 frame, f32 *target)
{
    f32 delta[4];

    out->color = func_002D7458(block->unk2C, block->unk50, frame, block->length);
    out->scale = func_002D7770(&block->scale.track.curve, frame, block->length);
    if (block->heading.track.curve.headingMode != 2) {
        out->angle = func_002D7770(&block->heading.track.curve, frame, block->length);
        return;
    }
    VU0_MOVE_VF(vf20, vf28);
    VU0_MOVE_VF(vf21, vf29);
    VU0_MOVE_VF(vf22, vf30);
    VU0_MOVE_VF(vf23, vf31);
    VU0_LOAD_MATRIX(sdfViewMatrix);
    sdfPostmultiplyVuMatrixFromMemory(sdfProjectionMatrix);
    VU0_LOAD_VF(vf10, out->pos);
    VU0_TRANSFORM_POINT(vf10, vf10);
    VU0_PERSPECTIVE_DIVIDE_VF10();
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, target);
    VU0_TRANSFORM_POINT(vf10, vf10);
    VU0_PERSPECTIVE_DIVIDE_VF10();
    VU0_SUB(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, delta);
    if (delta[0] != 0.0f || delta[1] != 0.0f) {
        out->angle = func_003532E8(delta[1], delta[0]);
    } else {
        out->angle = 0.0f;
    }
    VU0_MOVE_VF(vf28, vf20);
    VU0_MOVE_VF(vf29, vf21);
    VU0_MOVE_VF(vf30, vf22);
    VU0_MOVE_VF(vf31, vf23);
}

void fileInvalidateSlotGroup(FileSlotTable *table, u32 slotAddr) {
    FileGridDimensions *grid = (FileGridDimensions *)table->data0;
    s32 rows = grid->rows;
    s32 columns = grid->columns;
    s32 n = columns * rows;
    u32 index;
    FileSlot *slot;
    s32 i;

    if (n != 0) {
        index = (slotAddr - (u32)table->slots) >> 5;
        slot = table->slots + (table->instances + index * n);
        for (i = 0; i < n; i++) {
            slot->state = 0xFFFFFFFF;
            slot++;
        }
    }
}

void fileCopyAndInvalidateSlotGroup(FileSlotTable *table, FileSlot *source) {
    FileGridDimensions *grid = (FileGridDimensions *)table->data0;
    s32 rows = grid->rows;
    s32 columns = grid->columns;
    s32 n = columns * rows;
    u32 index;
    FileSlot *slot;

    if (n != 0) {
        index = ((u32)source - (u32)table->slots) >> 5;
        slot = table->slots + (table->instances + index * n);
        *slot = *source;
        slot->state = 0xFFFFFFFF;
    }
}

INCLUDE_ASM(const s32, "game/code_002C96D0", func_002D7B58);

struct EffRandState;
extern struct EffRandState effSharedRandomState;
extern u32 effMiscRand(struct EffRandState *state);
extern f32 effMiscRandUnitFloat(void *state);
extern f32 fabsf(f32);
extern void func_002D7B58(FileSlotTable *, FileSlot *);

typedef struct FileWaveMotion {
    f32 axis[4];             /* 0x00: xyz only */
    f32 swayDirection[4];    /* 0x10: xyz only */
    f32 phase;
    f32 previousSine;
    f32 amplitude;
    f32 amplitudeStep;
    f32 phaseStep;
    f32 speed;
    f32 scaleMultiplier;
    f32 angle;
    f32 angleMultiplier;
} FileWaveMotion;            /* 0x44, ordinary word alignment */

extern f32 D_003E95B0[4]; /* (0, -1, 0, 0), existing shared axis */

/* vu0 routine: advances axial motion with a quaternion-oriented sine sway. */
void func_002D81B0(FileSlotTable *record) {
    f32 direction[4];
    f32 radiusVector[4];
    f32 previousPosition[4];
    FileKeyBlock *keys;
    u32 i;
    u32 count;
    u32 flags;
    FileSlot *slot;
    FileWaveMotion *motion;
    s32 duration;
    s32 prewarm;
    s32 mode;
    f32 speedDecrease;
    s32 toSpawn;
    s32 prewarmLength;
    s32 frame;
    f32 distance;
    f32 sine;
    f32 sway;
    s32 length;

    keys = (FileKeyBlock *)record->data0;
    count = record->instances;
    length = keys->length;
    flags = record->flags;
    slot = record->slots;
    motion = (FileWaveMotion *)record->unk1C;
    if (length != 0) {
        duration = keys->emissionDuration;
        prewarmLength = keys->length;
        mode = keys->heading.track.curve.headingMode;
        speedDecrease = keys->emitter.wave.axialDeceleration;
        radiusVector[3] = 0.0f;
        VU0_LOAD_VF(vf10, keys->orientation);
        effMiscQuaternionToMatrixVU();
        if (duration != 0 && (s32)record->references >= duration) {
            toSpawn = 0;
            prewarm = 0;
        } else {
            if (record->references == 0 && keys->prewarm != 0) {
                prewarm = 1;
                if (!(keys->spawnVariance > 0.0f)) {
                    toSpawn = record->instances;
                } else {
                    toSpawn = (s32)((f32)record->instances *
                        (effMiscRandUnitFloat(&effSharedRandomState) * (1.0999999046325684f - keys->spawnVariance)));
                }
            } else {
                prewarm = 0;
                if (keys->spawnVariance > 0.0f) {
                    record->spawnRemainder += (f32)keys->spawnRate *
                        (effMiscRandUnitFloat(&effSharedRandomState) * (1.0999999046325684f - keys->spawnVariance));
                } else {
                    record->spawnRemainder += (f32)keys->spawnRate;
                }
                toSpawn = (s32)fabsf(record->spawnRemainder);
                record->spawnRemainder -= (f32)toSpawn;
            }
        }
        i = 0;
        if (count != 0) {
            do {
                if (slot->state >= length) {
                    slot->state = duration != 0 ? -2 : -1;
                    fileInvalidateSlotGroup(record, (u32)slot);
                }
                frame = slot->state;
                if (frame != -2) {
                    if (frame == -1) {
                        if (toSpawn != 0) {
                            s32 localSpace = flags & 1;
                            f32 initialAmplitude;
                            f32 finalAmplitude;
                            f32 radius;

                            if (localSpace != 0) {
                                motion->axis[0] = 0.0f;
                                motion->axis[1] = -1.0f;
                                motion->axis[2] = 0.0f;
                            } else {
                                VU0_LOAD_VF(vf10, D_003E95B0);
                                VU0_ROTATE_VEC(vf10, vf10);
                                VU0_STORE_VF(vf10, direction);
                                motion->axis[0] = direction[0];
                                motion->axis[1] = direction[1];
                                motion->axis[2] = direction[2];
                            }
                            direction[0] = (effMiscRandUnitFloat(&effSharedRandomState) - 0.5f) * 2.0f;
                            direction[1] = 0.0f;
                            direction[2] = (effMiscRandUnitFloat(&effSharedRandomState) - 0.5f) * 2.0f;
                            direction[3] = 0.0f;
                            VU0_LOAD_VF(vf10, direction);
                            VU0_NORMALIZE_VF10();
                            if (localSpace == 0) {
                                VU0_ROTATE_VEC(vf10, vf10);
                            }
                            VU0_STORE_VF(vf10, direction);
                            motion->swayDirection[0] = direction[0];
                            motion->swayDirection[1] = direction[1];
                            motion->swayDirection[2] = direction[2];
                            motion->speed = keys->emitter.wave.axialSpeed *
                                (effMiscRandUnitFloat(&effSharedRandomState) * keys->emitter.wave.axialSpeedRandomness +
                                 (1.0f - keys->emitter.wave.axialSpeedRandomness));
                            initialAmplitude = keys->emitter.wave.initialAmplitude *
                                (effMiscRandUnitFloat(&effSharedRandomState) * keys->emitter.wave.initialAmplitudeRandomness +
                                 (1.0f - keys->emitter.wave.initialAmplitudeRandomness));
                            finalAmplitude = keys->emitter.wave.finalAmplitude *
                                (effMiscRandUnitFloat(&effSharedRandomState) * keys->emitter.wave.finalAmplitudeRandomness +
                                 (1.0f - keys->emitter.wave.finalAmplitudeRandomness));
                            motion->amplitude = initialAmplitude;
                            motion->amplitudeStep = (finalAmplitude - initialAmplitude) / (f32)length;
                            motion->phaseStep = keys->emitter.wave.phaseStep *
                                (effMiscRandUnitFloat(&effSharedRandomState) * keys->emitter.wave.phaseStepRandomness +
                                 (1.0f - keys->emitter.wave.phaseStepRandomness));
                            motion->phase = effMiscRandUnitFloat(&effSharedRandomState) * 6.2831850051879883f;
                            motion->previousSine = sdfSinPoly(motion->phase);
                            radius = keys->emitter.wave.initialRadius *
                                ((effMiscRandUnitFloat(&effSharedRandomState) - 0.5f) * 2.0f);
                            VEC3_SPLAT(radiusVector, radius);
                            if (localSpace != 0) {
                                VU0_LOAD_VF(vf10, direction);
                                VU0_LOAD_VF(vf11, radiusVector);
                                VU0_MUL(vf10, vf10, vf11);
                            } else {
                                VU0_LOAD_VF(vf10, direction);
                                VU0_LOAD_VF(vf11, radiusVector);
                                VU0_MUL(vf10, vf10, vf11);
                                VU0_LOAD_VF(vf11, keys->pos);
                                VU0_ADD(vf10, vf10, vf11);
                            }
                            VU0_STORE_VF(vf10, slot->pos);
                            motion->scaleMultiplier = effMiscRandUnitFloat(&effSharedRandomState) * keys->scale.emitter.randomness +
                                (1.0f - keys->scale.emitter.randomness);
                            if (mode != 2) {
                                motion->angleMultiplier = effMiscRandUnitFloat(&effSharedRandomState) * keys->heading.emitter.randomness +
                                    (1.0f - keys->heading.emitter.randomness);
                                if (mode == 1) {
                                    motion->angle = effMiscRandUnitFloat(&effSharedRandomState) * 6.2831850051879883f;
                                    if (effMiscRand(&effSharedRandomState) & 1) {
                                        motion->angleMultiplier = -motion->angleMultiplier;
                                    }
                                } else {
                                    motion->angle = 0.0f;
                                }
                            } else {
                                motion->angleMultiplier = 1.0f;
                                motion->angle = 0.0f;
                            }
                            slot->state = 0;
                            PCP_COPY_VECTOR(previousPosition, slot->pos);
                            if (prewarm != 0) {
                                f32 age = (f32)(effMiscRand(&effSharedRandomState) % prewarmLength);
                                distance = motion->speed * age;
                                distance -= speedDecrease * age * age * 0.5f;
                                slot->pos[0] += motion->axis[0] * distance;
                                slot->pos[1] += motion->axis[1] * distance;
                                slot->pos[2] += motion->axis[2] * distance;
                                motion->phase += motion->phaseStep * age;
                                motion->amplitude += motion->amplitudeStep * age;
                                sine = sdfSinPoly(motion->phase);
                                sway = motion->amplitude * sine;
                                slot->state = (s32)age;
                                slot->pos[0] += motion->swayDirection[0] * sway;
                                slot->pos[1] += motion->swayDirection[1] * sway;
                                slot->pos[2] += motion->swayDirection[2] * sway;
                                motion->previousSine = sine;
                                motion->phase += motion->phaseStep;
                                motion->amplitude += motion->amplitudeStep;
                            }
                            fileSampleKeyTracks(slot, keys, slot->state, previousPosition);
                            slot->scale *= motion->scaleMultiplier;
                            slot->angle *= motion->angleMultiplier;
                            slot->angle += motion->angle;
                            if (prewarm != 0) {
                                fileCopyAndInvalidateSlotGroup(record, slot);
                                slot->state++;
                            }
                            toSpawn--;
                        }
                    } else {
                        PCP_COPY_VECTOR(previousPosition, slot->pos);
                        sine = sdfSinPoly(motion->phase);
                        sway = motion->amplitude * (sine - motion->previousSine);
                        slot->pos[0] += motion->swayDirection[0] * sway;
                        slot->pos[1] += motion->swayDirection[1] * sway;
                        slot->pos[2] += motion->swayDirection[2] * sway;
                        distance = motion->speed - speedDecrease * (f32)frame;
                        slot->pos[0] += motion->axis[0] * distance;
                        slot->pos[1] += motion->axis[1] * distance;
                        slot->pos[2] += motion->axis[2] * distance;
                        motion->previousSine = sine;
                        motion->phase += motion->phaseStep;
                        motion->amplitude += motion->amplitudeStep;
                        fileSampleKeyTracks(slot, keys, frame, previousPosition);
                        slot->scale *= motion->scaleMultiplier;
                        slot->angle *= motion->angleMultiplier;
                        slot->angle += motion->angle;
                        func_002D7B58(record, slot);
                        slot->state = frame + 1;
                    }
                }
                motion++;
                slot++;
                i++;
            } while (i < count);
        }
    }
}


void effScaleParameterSet(ScaleOwner *owner, f32 scale) {
    ScaleSet *src = owner->src;
    ScaleSet *dst = owner->dst;
    u32 i;

    dst->unk64 = src->unk64 * scale;
    dst->unk68 = src->unk68 * scale;
    for (i = 0; i < 3; i++) {
        dst->entries[i].value = src->entries[i].value * scale;
    }
    dst->unkC8 = src->unkC8 * scale;
    dst->unkCC = src->unkCC * scale;
    dst->unkD4 = src->unkD4 * scale;
    dst->unkD8 = src->unkD8 * scale;
    dst->unkE0 = src->unkE0 * scale;
}

typedef struct FileSlotMotion {
    f32 direction[4];     /* 0x00; this callback initializes xyz only */
    f32 speed;            /* 0x10 */
    f32 scaleMultiplier;  /* 0x14 */
    f32 angle;            /* 0x18 */
    f32 angleMultiplier;  /* 0x1C */
} FileSlotMotion;

/* vu0 routine: emits and advances randomized radial slots, then samples their key tracks. */
void func_002D8AD0(FileSlotTable *record) {
    f32 delta[4];
    f32 radiusVector[4];
    f32 previousPosition[4];
    FileKeyBlock *keys;
    u32 i;
    u32 count;
    u32 flags;
    FileSlot *slot;
    FileSlotMotion *motion;
    s32 duration;
    s32 prewarm;
    s32 mode;
    f32 gravity;
    f32 acceleration;
    s32 toSpawn;
    s32 prewarmLength;
    s32 frame;
    f32 distance;
    s32 length;

    keys = (FileKeyBlock *)record->data0;
    count = record->instances;
    length = keys->length;
    flags = record->flags;
    slot = record->slots;
    motion = (FileSlotMotion *)record->unk1C;

    if (length != 0) {
        duration = keys->emissionDuration;
        prewarmLength = keys->length;
        mode = keys->heading.track.curve.headingMode;
        gravity = keys->emitter.radial.gravity;
        acceleration = keys->emitter.radial.acceleration;
        delta[3] = 0.0f;
        if (duration != 0 && (s32)record->references >= duration) {
            toSpawn = 0;
            prewarm = 0;
        } else {
            if (record->references == 0 && keys->prewarm != 0) {
                prewarm = 1;
                toSpawn = count;
                if (keys->spawnVariance > 0.0f) {
                    toSpawn = (s32)((f32)record->instances *
                        (effMiscRandUnitFloat(&effSharedRandomState) * (1.0999999046325684f - keys->spawnVariance)));
                }
            } else {
                prewarm = 0;
                if (keys->spawnVariance > 0.0f) {
                    record->spawnRemainder += (f32)keys->spawnRate *
                        (effMiscRandUnitFloat(&effSharedRandomState) * (1.0999999046325684f - keys->spawnVariance));
                } else {
                    record->spawnRemainder += (f32)keys->spawnRate;
                }
                toSpawn = (s32)fabsf(record->spawnRemainder);
                record->spawnRemainder -= (f32)toSpawn;
            }
        }
        i = 0;
        if (count != 0) {
            do {
                if (slot->state >= length) {
                    slot->state = duration != 0 ? -2 : -1;
                    fileInvalidateSlotGroup(record, (u32)slot);
                }
                frame = slot->state;
                if (frame != -2) {
                    if (frame == -1) {
                        if (toSpawn != 0) {
                            delta[0] = (effMiscRandUnitFloat(&effSharedRandomState) - 0.5f) * 2.0f;
                            delta[1] = (effMiscRandUnitFloat(&effSharedRandomState) - 0.5f) * 2.0f;
                            delta[2] = (effMiscRandUnitFloat(&effSharedRandomState) - 0.5f) * 2.0f;
                            VU0_LOAD_VF(vf10, delta);
                            VU0_NORMALIZE_VF10();
                            VU0_STORE_VF(vf10, delta);
                            motion->direction[0] = delta[0];
                            motion->direction[1] = delta[1];
                            motion->direction[2] = delta[2];
                            motion->speed = fabsf(keys->emitter.radial.speed *
                                (effMiscRandUnitFloat(&effSharedRandomState) * keys->emitter.radial.speedRandomness + (1.0f - keys->emitter.radial.speedRandomness)));
                            {
                                f32 radius = keys->emitter.radial.radius *
                                    (effMiscRandUnitFloat(&effSharedRandomState) * keys->emitter.radial.radiusRandomness + (1.0f - keys->emitter.radial.radiusRandomness));
                                VEC3_SPLAT(radiusVector, radius);
                            }
                            /* Retail initializes only XYZ of this radius vector; W is left untouched. */
                            VU0_LOAD_VF(vf10, delta);
                            if (keys->emitter.radial.speed < 0.0f) {
                                VU0_NEGATE_XYZ(vf10);
                            }
                            if (flags & 1) {
                                VU0_LOAD_VF(vf11, radiusVector);
                                VU0_MUL(vf10, vf10, vf11);
                            } else {
                                VU0_LOAD_VF(vf11, radiusVector);
                                VU0_MUL(vf10, vf10, vf11);
                                VU0_LOAD_VF(vf11, keys->pos);
                                VU0_ADD(vf10, vf10, vf11);
                            }
                            VU0_STORE_VF(vf10, slot->pos);
                            motion->scaleMultiplier = effMiscRandUnitFloat(&effSharedRandomState) * keys->scale.emitter.randomness +
                                (1.0f - keys->scale.emitter.randomness);
                            if (mode != 2) {
                                motion->angleMultiplier = effMiscRandUnitFloat(&effSharedRandomState) * keys->heading.emitter.randomness +
                                    (1.0f - keys->heading.emitter.randomness);
                                if (mode == 1) {
                                    motion->angle = effMiscRandUnitFloat(&effSharedRandomState) * 6.2831850051879883f;
                                    if (effMiscRand(&effSharedRandomState) & 1) {
                                        motion->angleMultiplier = -motion->angleMultiplier;
                                    }
                                } else {
                                    motion->angle = 0.0f;
                                }
                            } else {
                                motion->angleMultiplier = 1.0f;
                                motion->angle = 0.0f;
                            }
                            slot->state = 0;
                            PCP_COPY_VECTOR(previousPosition, slot->pos);
                            if (prewarm != 0) {
                                f32 age = (f32)(effMiscRand(&effSharedRandomState) % prewarmLength);
                                VU0_LOAD_VF(vf11, slot->pos);
                                distance = motion->speed * age;
                                distance += acceleration * age * age * 0.5f;
                                if (distance < 0.0f) {
                                    distance = 0.0f;
                                }
                                delta[0] = distance * motion->direction[0];
                                delta[1] = distance * motion->direction[1];
                                delta[2] = distance * motion->direction[2];
                                delta[1] += gravity * age * age * 0.5f;
                                VU0_LOAD_VF(vf10, delta);
                                VU0_ADD(vf10, vf10, vf11);
                                VU0_STORE_VF(vf10, slot->pos);
                                slot->state = (s32)age;
                            }
                            fileSampleKeyTracks(slot, keys, slot->state, previousPosition);
                            slot->scale *= motion->scaleMultiplier;
                            slot->angle *= motion->angleMultiplier;
                            slot->angle += motion->angle;
                            if (prewarm != 0) {
                                fileCopyAndInvalidateSlotGroup(record, slot);
                                slot->state++;
                            }
                            toSpawn--;
                        }
                    } else {
                        f32 age;
                        PCP_COPY_VECTOR(previousPosition, slot->pos);
                        VU0_LOAD_VF(vf11, slot->pos);
                        age = (f32)frame;
                        distance = motion->speed + acceleration * age;
                        if (distance < 0.0f) {
                            distance = 0.0f;
                        }
                        delta[0] = distance * motion->direction[0];
                        delta[1] = distance * motion->direction[1];
                        delta[2] = distance * motion->direction[2];
                        delta[1] += gravity * age;
                        VU0_LOAD_VF(vf10, delta);
                        VU0_ADD(vf10, vf10, vf11);
                        VU0_STORE_VF(vf10, slot->pos);
                        fileSampleKeyTracks(slot, keys, frame, previousPosition);
                        slot->scale *= motion->scaleMultiplier;
                        slot->angle *= motion->angleMultiplier;
                        slot->angle += motion->angle;
                        func_002D7B58(record, slot);
                        slot->state = frame + 1;
                    }
                }
                motion++;
                slot++;
                i++;
            } while (i < count);
        }
    }
}

void effLoadObjScaleParamsA(ScaleOwner *owner, f32 scale) {
    ScaleSet *src = owner->src;
    ScaleSet *dst = owner->dst;
    u32 i;

    dst->unk64 = src->unk64 * scale;
    dst->unk68 = src->unk68 * scale;
    for (i = 0; i < 3; i++) {
        dst->entries[i].value = src->entries[i].value * scale;
    }
    dst->unkC8 = src->unkC8 * scale;
    dst->unkD0 = src->unkD0 * scale;
    dst->unkD8 = src->unkD8 * scale;
    dst->unkDC = src->unkDC * scale;
}

INCLUDE_ASM(const s32, "game/code_002C96D0", func_002D9228);

void effLoadObjScaleParamsC(ScaleOwner *owner, f32 scale) {
    ScaleSet *src = owner->src;
    ScaleSet *dst = owner->dst;
    u32 i;

    dst->unk64 = src->unk64 * scale;
    dst->unk68 = src->unk68 * scale;
    for (i = 0; i < 3; i++) {
        dst->entries[i].value = src->entries[i].value * scale;
    }
    dst->unkC8 = src->unkC8 * scale;
    dst->unkCC = src->unkCC * scale;
    dst->unkD4 = src->unkD4 * scale;
    dst->unkE4 = src->unkE4 * scale;
    dst->unkF0 = src->unkF0 * scale;
}

INCLUDE_ASM(const s32, "game/code_002C96D0", func_002D9AF8);

void effScaleOwnerParametersFromSource(ScaleOwner *owner, f32 scale) {
    ScaleSet *dst = owner->dst;
    ScaleSet *src = owner->src;
    u32 i;

    dst->unk64 = src->unk64 * scale;
    dst->unk68 = src->unk68 * scale;
    for (i = 0; i < 3; i++) {
        dst->entries[i].value = src->entries[i].value * scale;
    }
    dst->unkC8 = src->unkC8 * scale;
    dst->unkD0 = src->unkD0 * scale;
    dst->unkE4 = src->unkE4 * scale;
}

/* vu0 routine: advances directed slots through the emitter quaternion frame. */
void func_002DA358(FileSlotTable *record) {
    f32 delta[4];
    f32 radiusVector[4];
    f32 previousPosition[4];
    FileKeyBlock *keys;
    u32 i;
    u32 count;
    u32 flags;
    FileSlot *slot;
    FileSlotMotion *motion;
    s32 duration;
    s32 prewarm;
    s32 mode;
    f32 gravity;
    f32 acceleration;
    s32 toSpawn;
    s32 prewarmLength;
    s32 frame;
    f32 distance;
    s32 length;

    keys = (FileKeyBlock *)record->data0;
    count = record->instances;
    length = keys->length;
    flags = record->flags;
    slot = record->slots;
    motion = (FileSlotMotion *)record->unk1C;

    if (length != 0) {
        duration = keys->emissionDuration;
        prewarmLength = keys->length;
        mode = keys->heading.track.curve.headingMode;
        gravity = keys->emitter.directed.gravity;
        acceleration = keys->emitter.directed.acceleration;
        delta[3] = 0.0f;
        VU0_LOAD_VF(vf10, keys->orientation);
        effMiscQuaternionToMatrixVU();
        if (duration != 0 && (s32)record->references >= duration) {
            toSpawn = 0;
            prewarm = 0;
        } else {
            if (record->references == 0 && keys->prewarm != 0) {
                prewarm = 1;
                if (!(keys->spawnVariance > 0.0f)) {
                    toSpawn = record->instances;
                } else {
                    toSpawn = (s32)((f32)record->instances *
                        (effMiscRandUnitFloat(&effSharedRandomState) * (1.0999999046325684f - keys->spawnVariance)));
                }
            } else {
                prewarm = 0;
                if (keys->spawnVariance > 0.0f) {
                    record->spawnRemainder += (f32)keys->spawnRate *
                        (effMiscRandUnitFloat(&effSharedRandomState) * (1.0999999046325684f - keys->spawnVariance));
                } else {
                    record->spawnRemainder += (f32)keys->spawnRate;
                }
                toSpawn = (s32)fabsf(record->spawnRemainder);
                record->spawnRemainder -= (f32)toSpawn;
            }
        }
        i = 0;
        if (count != 0) {
            do {
                if (slot->state >= length) {
                    slot->state = duration != 0 ? -2 : -1;
                    fileInvalidateSlotGroup(record, (u32)slot);
                }
                frame = slot->state;
                if (frame != -2) {
                    if (frame == -1) {
                        if (toSpawn != 0) {
                            f32 spread;
                            s32 localSpace;

                            spread = keys->emitter.directed.spread *
                                (effMiscRandUnitFloat(&effSharedRandomState) * keys->emitter.directed.spreadRandomness +
                                 (1.0f - keys->emitter.directed.spreadRandomness));
                            delta[0] = (effMiscRandUnitFloat(&effSharedRandomState) - 0.5f) * 2.0f * spread;
                            delta[1] = -(1.0f - spread);
                            delta[2] = (effMiscRandUnitFloat(&effSharedRandomState) - 0.5f) * 2.0f * spread;
                            VU0_LOAD_VF(vf10, delta);
                            VU0_NORMALIZE_VF10();
                            localSpace = flags & 1;
                            if (localSpace == 0) {
                                VU0_ROTATE_VEC(vf10, vf10);
                            }
                            VU0_STORE_VF(vf10, delta);
                            motion->direction[0] = delta[0];
                            motion->direction[1] = delta[1];
                            motion->direction[2] = delta[2];
                            motion->speed = fabsf(keys->emitter.directed.speed *
                                (effMiscRandUnitFloat(&effSharedRandomState) * keys->emitter.directed.speedRandomness + (1.0f - keys->emitter.directed.speedRandomness)));
                            {
                                f32 radius = keys->emitter.directed.radius *
                                    (effMiscRandUnitFloat(&effSharedRandomState) * keys->emitter.directed.radiusRandomness + (1.0f - keys->emitter.directed.radiusRandomness));
                                VEC3_SPLAT(radiusVector, radius);
                            }
                            /* Retail initializes only XYZ of this radius vector; W is left untouched. */
                            VU0_LOAD_VF(vf10, delta);
                            if (keys->emitter.directed.speed < 0.0f) {
                                VU0_NEGATE_XYZ(vf10);
                            }
                            if (localSpace != 0) {
                                VU0_LOAD_VF(vf11, radiusVector);
                                VU0_MUL(vf10, vf10, vf11);
                            } else {
                                VU0_LOAD_VF(vf11, radiusVector);
                                VU0_MUL(vf10, vf10, vf11);
                                VU0_LOAD_VF(vf11, keys->pos);
                                VU0_ADD(vf10, vf10, vf11);
                            }
                            VU0_STORE_VF(vf10, slot->pos);
                            motion->scaleMultiplier = effMiscRandUnitFloat(&effSharedRandomState) * keys->scale.emitter.randomness +
                                (1.0f - keys->scale.emitter.randomness);
                            if (mode != 2) {
                                motion->angleMultiplier = effMiscRandUnitFloat(&effSharedRandomState) * keys->heading.emitter.randomness +
                                    (1.0f - keys->heading.emitter.randomness);
                                if (mode == 1) {
                                    motion->angle = effMiscRandUnitFloat(&effSharedRandomState) * 6.2831850051879883f;
                                    if (effMiscRand(&effSharedRandomState) & 1) {
                                        motion->angleMultiplier = -motion->angleMultiplier;
                                    }
                                } else {
                                    motion->angle = 0.0f;
                                }
                            } else {
                                motion->angleMultiplier = 1.0f;
                                motion->angle = 0.0f;
                            }
                            slot->state = 0;
                            PCP_COPY_VECTOR(previousPosition, slot->pos);
                            if (prewarm != 0) {
                                f32 age = (f32)(effMiscRand(&effSharedRandomState) % prewarmLength);
                                VU0_LOAD_VF(vf11, slot->pos);
                                distance = motion->speed * age;
                                distance += acceleration * age * age * 0.5f;
                                if (distance < 0.0f) {
                                    distance = 0.0f;
                                }
                                delta[0] = distance * motion->direction[0];
                                delta[1] = distance * motion->direction[1];
                                delta[2] = distance * motion->direction[2];
                                delta[1] += gravity * age * age * 0.5f;
                                VU0_LOAD_VF(vf10, delta);
                                VU0_ADD(vf10, vf10, vf11);
                                VU0_STORE_VF(vf10, slot->pos);
                                slot->state = (s32)age;
                            }
                            fileSampleKeyTracks(slot, keys, slot->state, previousPosition);
                            slot->scale *= motion->scaleMultiplier;
                            slot->angle *= motion->angleMultiplier;
                            slot->angle += motion->angle;
                            if (prewarm != 0) {
                                fileCopyAndInvalidateSlotGroup(record, slot);
                                slot->state++;
                            }
                            toSpawn--;
                        }
                    } else {
                        f32 age;
                        PCP_COPY_VECTOR(previousPosition, slot->pos);
                        VU0_LOAD_VF(vf11, slot->pos);
                        age = (f32)frame;
                        distance = motion->speed + acceleration * age;
                        if (distance < 0.0f) {
                            distance = 0.0f;
                        }
                        delta[0] = distance * motion->direction[0];
                        delta[1] = distance * motion->direction[1];
                        delta[2] = distance * motion->direction[2];
                        delta[1] += gravity * age;
                        VU0_LOAD_VF(vf10, delta);
                        VU0_ADD(vf10, vf10, vf11);
                        VU0_STORE_VF(vf10, slot->pos);
                        fileSampleKeyTracks(slot, keys, frame, previousPosition);
                        slot->scale *= motion->scaleMultiplier;
                        slot->angle *= motion->angleMultiplier;
                        slot->angle += motion->angle;
                        func_002D7B58(record, slot);
                        slot->state = frame + 1;
                    }
                }
                motion++;
                slot++;
                i++;
            } while (i < count);
        }
    }
}

void fileScaleEffectSurfaceParameterFields(ScaleOwner *owner, f32 factor) {
    f32 *source = (f32 *)owner->src;
    f32 *destination = (f32 *)owner->dst;
    u8 *sourceEntries = (u8 *)source + 4;
    u8 *destinationEntries = (u8 *)destination + 4;
    u32 index = 0;
    u32 offset = 0x70;

    destination[0x64 / 4] = source[0x64 / 4] * factor;
    destination[0x68 / 4] = source[0x68 / 4] * factor;
    do {
        *(f32 *)(destinationEntries + offset) = *(f32 *)(sourceEntries + offset) * factor;
        index++;
        offset += 8;
    } while (index < 3);
    destination[0xC8 / 4] = source[0xC8 / 4] * factor;
    destination[0xD8 / 4] = source[0xD8 / 4] * factor;
    destination[0xE0 / 4] = source[0xE0 / 4] * factor;
    destination[0xE4 / 4] = source[0xE4 / 4] * factor;
}

INCLUDE_ASM(const s32, "game/code_002C96D0", func_002DAAE0);

void sdfScaleResourceFloatParameterGroups(ScaleOwner *owner, f32 factor) {
    f32 *source = (f32 *)owner->src;
    f32 *destination = (f32 *)owner->dst;
    u8 *sourceEntries = (u8 *)source + 4;
    u8 *destinationEntries = (u8 *)destination + 4;
    u32 index = 0;
    u32 offset = 0x70;

    destination[0x64 / 4] = source[0x64 / 4] * factor;
    destination[0x68 / 4] = source[0x68 / 4] * factor;
    do {
        *(f32 *)(destinationEntries + offset) = *(f32 *)(sourceEntries + offset) * factor;
        index++;
        offset += 8;
    } while (index < 3);
    destination[0xC8 / 4] = source[0xC8 / 4] * factor;
    destination[0xD8 / 4] = source[0xD8 / 4] * factor;
    destination[0xE0 / 4] = source[0xE0 / 4] * factor;
    destination[0xE4 / 4] = source[0xE4 / 4] * factor;
}

void func_002DB288(FileSlotTable *record, const f32 *value) {
    if (record->type == 7) {
        f32 *sprite = (f32 *)record->data0;
        sprite[0xFC / 4] = value[0];
        sprite[0x100 / 4] = value[1];
        sprite[0x104 / 4] = value[2];
    }
}

void func_002DB2C0(FileSlotTable *record, const f32 *value) {
    if (record->type == 7) {
        f32 *sprite = (f32 *)record->data0;
        sprite[0x108 / 4] = value[0];
        sprite[0x10C / 4] = value[1];
        sprite[0x110 / 4] = value[2];
    }
}

/* cubic Bezier point at t through four control points; result is left in vf10 */
void vu0CubicBezierPoint(f32 t, f32 *p0, f32 *p1, f32 *p2, f32 *p3) {
    f32 weight[4];
    f32 point[4];
    f32 t2 = t * t;
    f32 t3 = t2 * t;
    f32 u = 1.0f - t;
    f32 u2 = u * u;

    weight[0] = u2 * u;
    weight[1] = t * u2 * 3.0f;
    weight[2] = t2 * u * 3.0f;
    weight[3] = t3;
    point[0] = p0[0] * weight[0] + p1[0] * weight[1] + p2[0] * weight[2] + p3[0] * weight[3];
    point[1] = p0[1] * weight[0] + p1[1] * weight[1] + p2[1] * weight[2] + p3[1] * weight[3];
    point[2] = p0[2] * weight[0] + p1[2] * weight[1] + p2[2] * weight[2] + p3[2] * weight[3];
    VU0_LOAD_VF(vf10, point);
}

INCLUDE_ASM(const s32, "game/code_002C96D0", func_002DB3E0);

void effScaleParameterSetBase(ScaleOwner *owner, f32 scale) {
    ScaleSet *src = owner->src;
    ScaleSet *dst = owner->dst;
    u32 i;

    dst->unk64 = src->unk64 * scale;
    dst->unk68 = src->unk68 * scale;
    for (i = 0; i < 3; i++) {
        dst->entries[i].value = src->entries[i].value * scale;
    }
}

u32 fileAllocateGridRecordSlots(u16 type, u32 count, void *data) {
    FileGridDimensions *src = data;
    u32 slotCount = count * src->columns * src->rows + count;
    u32 slotBytes = slotCount << 5;
    s32 dataBytes = D_003E95C0[type].dataBytes;
    u32 headerSize = 0x30;
    u32 size;
    u32 handle;
    FileSlotTable *rec;
    u8 *body;
    u8 *vec;

    size = slotBytes + headerSize;
    size += D_003E95C0[type].slotBytes * count;
    size += dataBytes * 2;
    handle = sdfAllocGeneralBlock(size);
    rec = sdfResourceRetainAddress(handle);
    body = (u8 *)rec + headerSize;
    rec->type = type;
    rec->slots = (FileSlot *)body;
    body += slotBytes;
    rec->data0 = body;
    body += dataBytes;
    rec->data1 = body;
    body += dataBytes;
    rec->unk1C = body;
    rec->instances = count;
    rec->count = slotCount;
    rec->handle = handle;
    rec->flags = 0;
    rec->references = 0;
    rec->spawnRemainder = 0.0f;
    memcpy(rec->data0, src, dataBytes);
    memcpy(rec->data1, src, dataBytes);
    vec = rec->data0;
    VU0_STORE_VF(vf0, vec);
    VU0_STORE_VF(vf0, vec + 0x10);
    if (vec[0xBC] != 0) {
        rec->flags |= 1;
    }
    return (u32)rec;
}

void fileReleaseGridRecordHandle(FileSlotTable *record) {
    sdfReleaseResourceAllocation(record->handle);
}

void fileClearRecordReferences(FileSlotTable *record) {
    record->references = 0;
}

void fileAcquireRecord(record)
FileSlotTable *record;
{
    if (record->references == 0) {
        fileResetSlotStates(record);
    }
    D_003E95C0[record->type].acquire(record);
    record->references++;
}

void fileReadVectorPtr20(u8 *obj, void *dst) {
    PCP_COPY_VECTOR(dst, *(u8 **)(obj + 0x20));
}

void mnuRecordSetVector(obj, src)
u8 *obj;
void *src;
{
    PCP_COPY_VECTOR(*(u8 **)(obj + 0x20), src);
}

void fileReadRecordSecondVector(u8 *obj, void *dst) {
    PCP_COPY_VECTOR(dst, *(u8 **)(obj + 0x20) + 0x10);
}

void fileSetRecordSecondVector(obj, src)
u8 *obj;
void *src;
{
    PCP_COPY_VECTOR(*(u8 **)(obj + 0x20) + 0x10, src);
}

INCLUDE_SDATA(const s32, "game/code_002C96D0", effModelUpdateControlFlags);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437E10);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437E18);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437E20);

INCLUDE_SDATA(const s32, "game/code_002C96D0", D_00437E28);
