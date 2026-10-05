#include "common.h"
#include "pcp_vu0.h"
#include "kwln.h"
#include "fpu.h"



extern void kwlnPadStartMotor(s32, u8, s32);
extern s32 D_003BC87C;


extern s32 D_0037D488[];
extern s32 D_003BC868;
extern s32 D_003BC86C;
extern s32 D_003BC870;
extern s32 D_003BC874;
extern s32 D_003BC878;
extern s32 D_003BC87C;
extern s32 sdfTexReleaseReferenceViaHandler();
extern s32 dds3GetWorldObject();
extern void dds3SetWorldObjectDataValue();
extern void fileWaitReady();
extern void sdfReleaseMemorySlot();
extern u8 sdfViewTargetVector[];
extern u8 sdfViewEyeVector[];
extern u8 sdfViewUpVector[];
extern f32 func_002FA1C0(f32);
extern f32 func_002FA1F0(f32, f32);
extern void func_002E7F20(f32, f32, f32);
extern void effMiscQuatMultiplyVU(void);







extern void *fileStartMemoryCardDetection(void);
extern s32 mnuSelectFileBranch(void);






extern u8 D_003DC800[];

extern void evtSubmitPrimaryGsTest(s32, s32, s32, s32, s32, s32, s32, s32);
extern s32 D_003BC880;

extern void *fileReadSlotPreviewBegin(void);
extern void *fileAdvanceSlotScan(void);
extern void fileReqSetSelectedSlot(u32 ctx, s32 arg);

extern void *(*fileMenuStateHandler)(s32);
extern s32 D_003BC814;
extern s32 fileIsRequestReadyInCurrentMode(u32, void *);
extern u32 fileGetResourceHandle(u32);
extern u32 fileGetLoadedDataAddress(u32);
extern u32 fileGetResourceSize(u32);
extern void filePollEntryCleanup(u32);
extern s32 fileDrawMenuFrame(s32);

typedef struct FileRecordSlot {
    u8 pad0[0x10];
    u32 state;
    u8 pad14[0xC];
} FileRecordSlot;

typedef struct FileRecordSlots {
    u16 type;
    u8 pad2[2];
    u32 instances;
    u32 count;
    u32 flags;
    u32 references;
    u32 unk14;
    FileRecordSlot *slots;
    u8 *unk1C;
    u8 *data0;
    u8 *data1;
    u32 handle;
} FileRecordSlots;

/* The record's signed +0x54 mode is consumed by every billboard opener. */
typedef struct FileBillboardRecord {
    u8 pad0[0x54];
    s16 mode; /* 0x54 */
} FileBillboardRecord;

typedef struct FileRecordType {
    void (*acquire)(void *);
    u32 unk4;
    s32 slotBytes;
    s32 dataBytes;
} FileRecordType;

extern u32 func_0029C230(u32);

extern void *fileDuplicateJob(void *);

extern s32 sdfAllocGeneralBlock();

extern s32 sdfResourceRetainAddress();

typedef struct DevState DevState;
extern DevState *sdfDevCreateCommandState(s32);

extern u32 sdfDevQueueControlAndWait(DevState *);
extern void sdfDevQueueReadAndWait(DevState *, void *, s32);
extern void sdfDevWaitThenReleaseCommandState(DevState *);

extern u32 effModelUpdateControlFlags;

extern s32 fileConfigTaskWork;

extern u32 D_003BC888;

extern void func_003014F0(void *dst, const char *fmt, ...);

extern u32 fileSaveReadBuffer;

extern u32 D_003BD8EC;

extern u32 itfCreateConvertedTextGlyph(s32, s32, s32, u32, const u8 *, s32);

extern void kwlnFadeInStart(s32, s32, s32, s32);

extern void *fileWaitContinuation;

extern s32 fileWaitTicksRemaining;

extern void *fileUpdateWait(void);

extern void *fileSlotStatusPoll(void);

extern FileRecordType D_0037E550[];

extern void fileResetSlotStates(FileRecordSlots *record);

extern void *fileJobCreateFromCommandState();

extern char D_003BC940[];

extern char D_003B2688[]; /* "base.ico"; retail record includes padding */

extern u32 D_003BD914;

extern u32 D_003BD918;

extern u32 *D_003BD928;

extern u32 *D_003BD92C;

extern void *D_003BD930;

extern u32 *D_003BD934;

extern u32 D_003BC824;

extern void *fileBeginRequest(const char *, u32 *, u32 *, void *, u32 *);

extern void mcOpenFilePath(u32, const char *, s32);

extern void mcDeleteFilePath(void);

extern void *fileWriteWaitOpen(void);

extern void *mcHandleLoadResult(void);

extern void *fileBeginLabeledSlotWrite(void);

extern void *fileBuildMainBlobAfterDelete(void);

extern s32 fileSaveFileDescriptor;

extern u32 fileSaveReadBufferResource;

extern s32 mcPollCompletionStatus(void);

extern void mcCloseOpenFile(s32);

extern void *fileStoreSlotHeader(void);

extern s32 mcPollWithExtendedErrors(void);

extern void *fileCreateMainBegin(void);

extern s32 datGameState;

extern void *fileBeginSlotResetPrompt(void);

extern void *mcPrepareDirectory(void);

extern void *fileSlotSelectPollClear(void);

extern void *fileBeginDirectoryScan(void);

typedef struct LoadMirror {
    u32 current;
    u32 previous;
} LoadMirror;

/* Shared save-header flag word used when leaving and re-entering file flow. */
typedef struct FileSaveState {
    u8 pad00[0xA54];
    u32 slotFlags;
} FileSaveState;

extern LoadMirror fileSlotFlagMirror;

extern u32 fileSavedSlotFlags;

extern char fileConfigInputTaskName[];

extern char fileConfigLoadTaskName[]; /* "config_draw" */

extern char fileConfigOwnerTaskName[]; /* "config_update" */

extern void kwlnTaskDestroyWithHierarchyByName(const char *name, s32 hierarchy);

extern void func_003003F0(void *arg);

extern char D_003BC900[];

extern char D_003BC908[];

extern char D_003BC910[];

extern char D_003BC918[];

extern char D_003BC920[];

extern u32 D_003BC81C;

extern u32 fileSelectionPending;

extern u32 D_003BC854;

extern u32 D_003BC80C;

extern u32 D_003BC810;

extern u32 fileMemoryCardRequestContext;

extern s32 D_003BC850;

extern s32 D_003BC860;

extern u32 D_003BC858;

extern s32 D_003BC864;

extern s8 D_003DC803[];

extern u32 D_003BD904;

extern u32 D_003BD910;

extern u32 fileSaveIconRequest;

extern u64 func_001978E8(s32, s32, u64, u64, u64, u64);

extern u32 D_003BD8F0;

extern u32 func_001951C8(u32, u32, u32, u32, u32);

extern s32 D_003BC7FC;

extern s8 fileMenuTaskAlive;

extern s32 D_003BC7F0;

/* Loader context at fileLoadMenuState. */
typedef struct LoadCtx374A0 {
    u8 unk0[4]; /* 0x00 */
    s32 unk4;   /* 0x04 */
    s32 unk8;   /* 0x08 */
    s32 unkC;   /* 0x0C */
    s8 unk10;   /* 0x10 */
    u8 pad11[3]; /* 0x11 */
    s32 unk14;  /* 0x14 */
    s32 unk18;  /* 0x18 */
    s32 unk1C;  /* 0x1C */
} LoadCtx374A0;

extern LoadCtx374A0 fileLoadMenuState;

/* Far scalar: incomplete array forces non-small-data addressing. */
extern s32 D_0037D4D0[];

extern u32 D_0037E130[];

extern char D_003B2668[];

extern char D_003B26C8[];


extern KwlnTask *kwlnTaskGetTaskByName(const char *name);

extern u32 fileQueueDefaultCallbackRequest(const char *path);

extern void dds3DispatchIndexedCallback(void *callback);

extern void func_002966D8(s32 object);

extern void *fileResetSelection(void);
extern void sndSetSequenceVolumePan(s32, s32, s32);

extern s32 D_003BC818;
extern s32 D_003BC820;
extern void fileSetMenuValueAndInitializeFlags(u32 value);
extern void fileLoadSetMode(s8 mode);

extern void *fileBeginSlotReset(void);

extern s32 fileSlotSelectPoll(void);

extern void fileOnAllWritten(void);

extern void mcdFinishFileDetection(void);

extern s32 fileSlotSelectionPollCount;

extern s32 fileReqPoll(void);

extern u8 fileReqGetStatus(s32 request);

extern void fileReqBegin(s32 request);

extern s32 func_0028B508(void);

extern void *fileRestartSlotSelection(void);

extern s32 fileBeginSlotPromptFive(void);

extern s32 fileIsCardSpaceAboveMinimum(void);

extern s32 D_003BC844;

extern void fileReqSetSlotFlags(s32 request, s32 slot, s32 flags);

extern void fileReqClearSlotFlags(s32, s32);

extern void fileReqMarkSlotMetadataDirty(s32);

extern void *fileBeginDetectionRequest(u32 callback);

extern void *fileScanSlotStates(void);

extern void *filePollSlotRequestAndResumeFlow(void);

extern s32 fileReqGetSize(s32 request);

extern u32 D_003BC804;

extern void func_001005B8(void);

extern void func_00289D50(u32 handle);

extern void *mcHandleDetectionResult(void);

extern void fileQueueInitTransform(void *queue);

extern void func_00293158(f32 matrix[4][4]);


extern void *sdfAllocSizeClassBlock(s32 size);

extern void *sdfAllocAndClearQuadwords(s32 size);

extern void sdfReleaseChipBlock();

extern void fileClearRecordReferences(FileRecordSlots *record);

extern void mnuRecordSetVector(void *record, const u128 *vector);

extern void fileSetRecordSecondVector(void *record, const u128 *vector);

extern void dds3AdminSubmitModeRequest(s32 kind, void *data, s32 size, s32 flags);

extern s32 D_003BC848;

extern s32 fileSlotScanIndex;

extern char D_003B2658[];

extern void *fileBeginWait(void *callback);

extern void mcFormatSaveFilename(void *dst, s32 number);

extern void mcChangeCurrentDirectory(u32 request, void *data);

extern void *fileScanSlotIconSysBegin(void);

extern s32 mcPollSyncResult(void);

extern void mcReadDirectoryEntries(u32 request, const char *path, void *data, s32 option);

extern char D_003B2678[];

extern u8 D_003DC780[];

extern void *mcHandleSlotWriteResult(void);

extern void *fileBeginSlotMetadataRefresh(void);

extern void *fileBeginSlotOpen(void);

extern s32 mcPollNonnegativeResult(void *request);

extern void *fileScanSlotStatesAdvance(void);

extern void *mcHandleDirectoryWriteResult(void);

extern void *fileScanSlotIconSysAltBegin(void);

extern u32 fileReqGetSlotFlags(s32 request, s32 slot);

extern s32 fileBeginSlotPromptSix(void);

extern u32 D_003BC834;

extern s32 fileBeginPromptDialog(void *start, void *finish, s32 mode);

extern void *filePollSlotDetectionAndBranch(void);
extern void *fileLoadMainBlobBegin(void);
extern void *mcHandleSetupResult(void);

extern void *filePrepareMainBlobWrite(void);

extern void *mcChooseLoadPath(void);

extern void fileAbortSlotScanOnInput(void);

extern void *mcDispatchReadCallback(void);

extern void *mcHandleSearchResult(void);

extern s32 mcPollNormalizedCommandStatus(void);

extern void *fileBuildMainBlobAndWrite();

extern u8 fileReqIsSlotMetadataDirty(s32 request);

extern u32 D_003DC7C0[];

extern void billDispatchByKind(void *handle);

extern void *effRetainResource(void *name);

extern void *billCreateIndexed(s32 mode, void *name);

extern void billMarkKindOneFlag(void *handle);

extern void billSetBillboardMode(void *handle, s16 index);

extern void *fileAllocateGridRecordSlots(u16 type, u32 count, void *src);

extern void *billCloneObjectRetainingSharedData(void *handle);

extern void *effReferenceObjectRetain(void *holder);

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

extern u32 D_0037D4AC[];

/* Callback table at D_0037E14C (0x28 bytes per entry). */
typedef struct Cb3714C {
    void (*cb)(void *arg);    /* 0x00 */
    u8 pad4[8];               /* 0x04 */
    void (*cbC)(void *, void *); /* 0x0C */
    void (*cb10)(void *arg);  /* 0x10 */
    void (*cb14)(void *arg, void *extra);  /* 0x14 */
    void (*cb18)(void *arg, void *extra);  /* 0x18 */
    void (*cb1C)(void *arg, f32 scale);  /* 0x1C */
    void (*cb20)(void *arg, u32 color);  /* 0x20 */
    u32 unk24;                /* 0x24 */
} Cb3714C;

extern Cb3714C D_0037E14C[];

typedef struct FileTypeCallbacks {
    void *(*create)(void *);
    void (*unk4)(void *);
    void (*destroy)(void *);
    void *(*createChild)(void *, u16);
    u8 unk10[0x18];
} FileTypeCallbacks;

extern FileTypeCallbacks fileJobTypeOperations[];

/* Object with loader sub-objects (+0x40...). */
typedef struct LoadObj {
    void *owner;        /* 0x00 */
    u32 color;          /* 0x04 */
    f32 scale;          /* 0x08 */
    u32 selector;       /* 0x0C: secondary job buffer operation */
    u8 pad10[0x24];
    void *deviceHandle; /* 0x34 */
    u32 unk38;          /* 0x38 */
    u32 unk3C;          /* 0x3C */
    void *referenceHolder; /* 0x40: released by effReleaseReferenceHolder */
    void *recordWork;     /* 0x44: created by fileAllocateGridRecordSlots */
    s16 unk48;          /* 0x48 */
    u16 unk4A;
} LoadObj;

extern void *mcdHandleSaveSetupDone(void);

extern s32 mcPollZeroCommandResult(void);

extern void func_00292720(void *);

extern void mnuCallInitWide(s32, s32, s32, u32, s32);

extern LoadObj *fileLoadObjectCreate(void *owner);

extern void fileCloneEffectSurfaceResources(LoadObj *result, LoadObj *owner);

typedef struct FileJobBufferSlot {
    u32 offset;
    u32 size;
    void *allocation;
    u16 selector;
    u16 unkE;
} FileJobBufferSlot;

typedef struct FileJob {
    u32 unk0;
    u16 type;
    u16 unk6;
    void *data;
    u16 option;
    u16 unkE;
    FileJobBufferSlot slots[2];
    u8 unk30[0x10];
    f32 offset[4];    /* 0x40 */
    f32 quat[4];
    f32 scale;        /* 0x60 */
    u32 color;              /* 0x64 */
    u32 xformFlags;   /* 0x68 */
    u8 unk6C[0x14];
    s32 unk80;
    u32 scaleFlags;   /* 0x84 */
    u8 unk88[8];
    u32 id;
    u32 sector;
    u32 flags;
    u8 unk9C[0x10];
    struct FileJob *next;
    struct FileJob *prev;
    u8 padB4[0xC];
} FileJob;

extern FileJob *fileCreateJob(u16 type);

extern void fileJobFreePrimaryBuffer(FileJob *job);

extern void fileJobFreeSecondaryBuffer(FileJob *job);

extern FileJob *fileJobCreate(void);

extern void fileReleaseGridRecordHandle(s32 record);

typedef struct FileQueue {
    f32 offset[4];
    f32 axis[4];
    u8 unk20[0x20];
    f32 position[4];
    f32 quat[4];
    f32 scale;        /* 0x60 */
    u32 color;         /* 0x64: modulation colour */
    /* 0x68: bitmask; bits 0x60 select the rotation branch. Left as unk68 because
     * renaming it alone changes codegen in fileJobNotifyPair. The DDS2 twin
     * calls this transformWord. */
    u32 unk68;
    u8 pad6C[8];
    f32 transformValue; /* 0x74 */
    u8 pad78[8];
    s32 count;
    u32 unk84;
    FileJob *last;   /* 0x88: append end */
    FileJob *first;  /* 0x8C: traversal start */
} FileQueue;
extern void fileQueueSetPosition(FileQueue *queue, void *vec);
extern void fileQueueSetRotation(FileQueue *queue, void *rot);
extern void effMiscQuaternionToMatrixVU(void);
extern void fileQueueSetScale(FileQueue *queue, f32 scale);
extern void func_00294938(FileQueue *queue, u32 color);
extern void fileJobCopyHeader(FileJob *dst, FileJob *src);

extern FileJob *fileQueueFindById(FileQueue *, u32);
extern FileJob *fileQueueGetAt(FileQueue *, s32);
extern s32 fileFindQueuedJobIndex(FileQueue *, FileJob *);


extern void fileQueueAppend(FileQueue *queue, FileJob *job);

extern s8 fileConfigTaskState;

void fileDestroyMenuTask(void) {
    if (D_003BC7F0 != 0) {
        kwlnTaskDestroyWithHierarchy(D_003BC7F0, 1);
        D_003BC7F0 = 0;
        fileMenuTaskAlive = 0;
    }
}

s8 fileMenuTaskIsAlive(void) {
    return fileMenuTaskAlive;
}

void mnuFormatSaveSlotHeaderName(void) {
}

void mcFormatSaveFilename(void *dst, s32 number) {
    func_003014F0(dst, "BASLUS-%05d-new-%d", D_003BC888, number);
}

void fileReqGetSlotCode(void) {
    s32 slot = fileReqGetSelectedSlot(fileMemoryCardRequestContext);
    D_003BC864 = D_003DC803[slot * 0x30];
}

/* Size of the persistent main save block copied during a reload. */
#define FILE_MAIN_BLOB_SIZE 0x33600

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
    return condition != 0 && D_003BC7FC == 1;
}

u8 fileIsLoadedWithActiveFlow(s32 loaded) {
    return loaded != 0 && D_003BC7FC == 1;
}

void mnuDrawAndStoreTextGlyphHandle(s32 x, s32 y, u32 colors, const u8 *text) {
    D_003BD8EC = itfCreateConvertedTextGlyph(x << 4, y << 3, 0, colors, text, 0);
    frFontDrawGlyphWithSharedFlags(D_003BD8EC, 1);
    frFontQueueGlyphInSelectedSlot(D_003BD8EC);
}

void mcdCreateFontDrawHandle(s32 x, s32 y, u32 color, u32 font) {
    frFontAddSharedGlyphFlags(1);
    D_003BD8F0 = func_001951C8(font, 0, 0, 0, 0);
    frFontClearFlagBits(1);
    frFontSetFlagAndMeasureGlyphs(D_003BD8F0, 1);
    frFontSetContextPair(D_003BD8F0, x << 4, y << 3);
    frFontSetChildColors(D_003BD8F0, color);
    func_001958A0(D_003BD8F0, 0, 0x56);
    frFontQueueGlyphInSelectedSlot(D_003BD8F0);
    frFontSetSharedRenderFlags(0x54);
}

void fileDrawMenuImageAtPoint(s32 x, s32 y, u64 first, u64 second) {
    u64 imageHandle;

    imageHandle = func_001978E8(x << 4, y << 3, 0, first, second, 0);
    frFontDrawGlyphWithSharedFlags(imageHandle, 1);
    frFontQueueGlyphInSelectedSlot(imageHandle);
}

extern f32 fileSaveHighlightPhase;
extern f32 sdfSinPoly(f32);
extern s32 itfMesGetGlobalWindowValue(void);

/* Animate the save-window highlight's alpha with a sinusoidal phase. */
void fileDrawPulsingSaveHighlight(void) {
    s32 angle;
    f32 wave;

    evtSetDrawSurfaceIndex(0x56);
    evtSubmitPrimaryAlphaBlendMode(0);
    fileSaveHighlightPhase = fileSaveHighlightPhase + 0.39999998f;
    sdfSinPoly(fileSaveHighlightPhase);
    angle = ((s32)D_0037D4D0[2] + 8) % 360;
    D_0037D4D0[2] = angle;
    wave = sdfSinPoly((f32)((angle + 0x5A) % 360) / 180.0f * 3.1415899f);
    D_0037D4D0[3] = (s32)((wave + 1.0f) * 0.5f * 191.0f + 64.0f);
    func_00108FA0(0x1BE, 0x12C, 0x13, 0x1F, 1, 1, 0x13, 0x1F, (D_0037D4D0[3] << 24) | 0xAEC014,
                  (D_0037D4D0[3] << 24) | 0xAEC014, (D_0037D4D0[3] << 24) | 0xAEC014,
                  (D_0037D4D0[3] << 24) | 0xAEC014, itfMesGetGlobalWindowValue());
}

extern void evtSetDrawSurfaceIndex(s32);
extern void evtSubmitPrimaryAlphaBlendMode(s32);
extern void func_00108FA0(s32, s32, s32, s32, s32, s32, s32, s32, u32, u32, u32, u32, s32);
extern void evtSubmitDefaultDepthGradientRect(s32, s32, s32, s32, s32, s32, s32, s32);
extern void fileCursorPulseUpdate(void);
extern void func_00290A88(s32, s32, s32);
extern s32 D_003BC85C;
extern s32 D_003BC884;

void fileDrawSaveWindow(void) {
    evtSetDrawSurfaceIndex(0x56);
    evtSubmitPrimaryAlphaBlendMode(0);
    func_00108FA0(0x112, 0x113, 0x98, 0x34, 0x14A, 0x1C5, 0x98, 0x34, 0x80808080, 0x80808080, 0x80808080, 0x80808080, D_003BC884);
    func_00108FA0(0x56, 0x113, 0xBC, 0x34, 0x14A, 0x1C5, 1, 0x34, 0x80808080, 0x80808080, 0x80808080, 0x80808080, D_003BC884);
    fileCursorPulseUpdate();
    func_00290A88(0x17E, 0x118, 0x56);
    D_003BC85C++;
}

s32 fileIsLoadStepComplete(void) {
    if (D_003BC860 < 7) {
        return 0;
    }
    return 1;
}

s32 fileDrawStatusDialogFrame(void) {
    s32 frame;
    s32 y;
    s32 height;
    s32 value;

    evtSetDrawSurfaceIndex(0x56);
    evtSubmitPrimaryAlphaBlendMode(0);
    evtSubmitPrimaryGsTest(1, 0, 128, 3, 0, 0, 1, 1);
    frame = ++D_003BC860;
    if (frame < 4) {
        height = frame * 72;
        y = 224 - frame * 36;
    } else {
        y = 80;
        height = 288;
    }
    if (frame < 4) {
        value = frame * 20;
    } else {
        value = 80;
    }
    func_00108FA0(0, 0, 512, 448, 0, 0, 512, 448,
                 (((value << 7) / 100) << 24) | 0x808080,
                 (((value << 7) / 100) << 24) | 0x808080,
                 (((value << 7) / 100) << 24) | 0x808080,
                 (((value << 7) / 100) << 24) | 0x808080, D_003BC880);
    if (D_003BC860 < 4) {
        value = D_003BC860 * 70 / 4;
    } else {
        value = 70;
    }
    evtSubmitDefaultDepthGradientRect(0, y, 512, height,
                 (((value << 7) / 100) << 24) | 0xA1000,
                 (((value << 7) / 100) << 24) | 0xA1000,
                 (((value << 7) / 100) << 24) | 0xA1000,
                 (((value << 7) / 100) << 24) | 0xA1000);
    evtSetDrawSurfaceIndex(0x53);
    if (D_003BC860 < 7) {
        return 0;
    }
    return 1;
}

extern char D_0037D6C8[];
extern char D_0037D6E8[];
extern char D_0037D700[];
extern char D_0037D768[];
extern char D_0037D7B8[];
extern char D_0037D7D8[];
extern char D_0037D800[];
extern char D_0037D758[];
extern char D_0037D948[];
extern char D_0037D778[];
extern char D_0037D960[];
extern char D_0037D930[];
extern s32 D_003BC840;
extern void fileResetLoadContextSlide(void);
extern char D_0037D520[];
extern char D_0037D550[];
extern char D_0037D570[];
extern char D_0037D580[];
extern char D_0037D5A8[];
extern char D_0037D620[];
extern char D_0037D650[];
extern char D_0037D668[];
extern char D_0037D690[];
extern char D_0037D6A8[];
extern char D_0037D788[];
extern char D_0037D798[];
extern char D_0037D7A8[];
extern char D_0037D820[];
extern char D_0037D848[];
extern char D_0037D868[];
extern char D_0037D880[];
extern char D_0037D8A0[];
extern char D_0037DB30[];
extern char D_0037DB58[];
extern char D_0037DB88[];

static inline void fileDrawDialogLine(s32 line, u32 glyphSource) {
    mcdCreateFontDrawHandle(86, 114 + line * 24, 0x89FEFF80U, glyphSource);
}

void fileShowStatusDialog(void) {
    if (D_003BC854 != 0) {
        if (D_003BC854 == 1) {
            D_003BC844 = 0;
            D_003BC840 = 0;
            fileResetLoadContextSlide();
            if (fileDrawStatusDialogFrame() != 0) {
                fileDrawDialogLine(0, (u32)D_0037D520);
                fileDrawDialogLine(1, (u32)D_0037D550);
                fileDrawDialogLine(2, (u32)D_0037D570);
                fileDrawDialogLine(3, (u32)D_0037D580);
                fileDrawDialogLine(4, (u32)D_0037D5A8);
                fileDrawPulsingSaveHighlight();
                D_003BC810 = 0;
            }
        }
        if (D_003BC854 == 2) {
            if (fileDrawStatusDialogFrame() != 0) {
                fileDrawDialogLine(0, (u32)D_0037D620);
                fileDrawDialogLine(1, (u32)D_0037D650);
                fileDrawDialogLine(2, (u32)D_0037D668);
                fileDrawDialogLine(3, (u32)D_0037D690);
                fileDrawDialogLine(4, (u32)D_0037D6A8);
                fileDrawPulsingSaveHighlight();
                D_003BC810 = 0;
            }
        }
        if (D_003BC854 == 3) {
            if (fileDrawStatusDialogFrame() != 0) {
                fileDrawDialogLine(0, (u32)D_0037D788);
                fileDrawPulsingSaveHighlight();
                D_003BC810 = 0;
            }
        }
        if (D_003BC854 == 4) {
            if (fileDrawStatusDialogFrame() != 0) {
                fileDrawDialogLine(0, (u32)D_0037D798);
                fileDrawPulsingSaveHighlight();
                D_003BC810 = 0;
            }
        }
        if (D_003BC854 == 5) {
            if (fileDrawStatusDialogFrame() != 0) {
                fileDrawDialogLine(0, (u32)D_0037D820);
                fileDrawDialogLine(1, (u32)D_0037D848);
                fileDrawDialogLine(2, (u32)D_0037D868);
                fileDrawDialogLine(3, (u32)D_0037D880);
                fileDrawDialogLine(4, (u32)D_0037D8A0);
                fileDrawPulsingSaveHighlight();
                D_003BC810 = 0;
            }
        }
        if (D_003BC854 == 6) {
            if (fileDrawStatusDialogFrame() != 0) {
                fileDrawDialogLine(0, (u32)D_0037D7A8);
                fileDrawPulsingSaveHighlight();
                D_003BC810 = 0;
            }
        }
        if (D_003BC854 == 7) {
            if (fileDrawStatusDialogFrame() != 0) {
                fileDrawDialogLine(0, (u32)D_0037DB30);
                fileDrawDialogLine(1, (u32)D_0037DB58);
                fileDrawDialogLine(2, (u32)D_0037DB88);
                fileDrawPulsingSaveHighlight();
                D_003BC810 = 0;
            }
        }
    }
}

void fileSetMenuFlowState(u32 state) {
    s32 previous;

    previous = D_003BC850;
    D_003BC850 = state;
    if (previous == 0) {
        D_003BC860 = 0;
    }
}

void fileShowFlowDialog(void) {
    if (D_003BC850 != 0) {
        if (D_003BC850 == 1) {
            if (fileDrawStatusDialogFrame() != 0) {
                fileDrawDialogLine(0, (u32)D_0037D6C8);
                fileDrawDialogLine(1, (u32)D_0037D6E8);
                fileDrawDialogLine(2, (u32)D_0037D700);
                fileDrawSaveWindow();
            }
        }
        if (D_003BC850 == 2) {
            if (fileDrawStatusDialogFrame() != 0) {
                fileDrawDialogLine(0, (u32)D_0037D768);
                fileDrawDialogLine(1, (u32)D_0037D7B8);
                fileDrawDialogLine(2, (u32)D_0037D7D8);
                fileDrawDialogLine(3, (u32)D_0037D800);
                fileDrawSaveWindow();
            }
        }
        if (D_003BC850 == 3) {
            if (fileDrawStatusDialogFrame() != 0) {
                fileDrawDialogLine(0, (u32)D_0037D758);
                fileDrawDialogLine(1, (u32)D_0037D7B8);
                fileDrawDialogLine(2, (u32)D_0037D7D8);
                fileDrawDialogLine(3, (u32)D_0037D800);
                fileDrawSaveWindow();
            }
        }
        if (D_003BC850 == 4) {
            if (fileDrawStatusDialogFrame() != 0) {
                fileDrawDialogLine(0, (u32)D_0037D948);
                fileDrawPulsingSaveHighlight();
            }
            D_003BC81C = 1;
        }
        if (D_003BC850 == 5) {
            if (fileDrawStatusDialogFrame() != 0) {
                fileDrawDialogLine(0, (u32)D_0037D778);
                fileDrawDialogLine(1, (u32)D_0037D7B8);
                fileDrawDialogLine(2, (u32)D_0037D7D8);
                fileDrawDialogLine(3, (u32)D_0037D800);
                fileDrawSaveWindow();
            }
        }
        if (D_003BC850 == 6) {
            if (fileDrawStatusDialogFrame() != 0) {
                fileDrawDialogLine(0, (u32)D_0037D960);
                fileDrawPulsingSaveHighlight();
            }
        }
        if (D_003BC850 == 13) {
            if (fileDrawStatusDialogFrame() != 0) {
                fileDrawDialogLine(0, (u32)D_0037D930);
                fileDrawPulsingSaveHighlight();
            }
            D_003BC81C = 2;
        }
    }
}

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
        return fileAdvanceSlotScan();
    }
    path[0] = 0x2F;
    mcFormatSaveFilename(&path[1], slot);
    len = strlen(&path[1]);
    path[len + 1] = 0x2F;
    memcpy(&path[len + 2], &path[1], len);
    path[len * 2 + 2] = 0;
    mcOpenFilePath(ctx, path, 1);
    return fileReadSlotPreviewBegin;
}

extern s32 mcPollCommandStatusWithResult(s32 *);
extern void mcReadOpenFile(s32, u32, s32);
extern void *fileReadSlotPreviewWait(void);
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

void *fileReadSlotPreviewWait(void) {
    s32 status = mcPollCompletionStatus();
    if (status == 0) {
        return NULL;
    }
    if (status == 1) {
        mcCloseOpenFile(fileSaveFileDescriptor);
        return fileStoreSlotHeader;
    }
    sdfReleaseResourceAllocation(fileSaveReadBufferResource);
    return fileBeginSlotMetadataRefresh();
}

void *fileStoreSlotHeader(void) {
    s32 status = mcPollZeroCommandResult();

    if (status == 0) {
        return NULL;
    }
    if (status == 1) {
        memcpy(D_003DC800 + fileSlotScanIndex * 0x30, (void *)fileSaveReadBuffer, 0x30);
        sdfReleaseResourceAllocation(fileSaveReadBufferResource);
        return fileAdvanceSlotScan();
    }
    sdfReleaseResourceAllocation(fileSaveReadBufferResource);
    return fileBeginSlotMetadataRefresh();
}

void *fileBeginWait(void *callback) {
    kwlnFadeInStart(0, 0, 0, 15);
    fileWaitContinuation = callback;
    fileWaitTicksRemaining = 20;
    return fileUpdateWait;
}

void *fileBeginSlotReset(void) {
    D_003BC848 = 0;
    fileSetMenuFlowState(0);
    D_003BC854 = 0;
    D_003BC834 = 4;
    D_003BC810 = 0;
    return (void *)fileBeginPromptDialog(&fileResetSelection, &fileBeginSlotResetPrompt, 0);
}

void *fileBeginDirectoryScan(void) {
    fileSetMenuFlowState(0);
    D_003BC854 = 0;
    D_003BC834 = 9;
    return (void *)fileBeginPromptDialog(&mcPrepareDirectory, &fileScanSlotStates, 1);
}

void *fileBeginSlotResetPrompt(void) {
    fileSetMenuFlowState(0);
    D_003BC854 = 0;
    D_003BC834 = 8;
    D_003BC810 = 0;
    return (void *)fileBeginPromptDialog(&func_0028B508, &fileBeginSlotReset, 1);
}

void *fileReturnToSlotSelection(void) {
    fileSetMenuFlowState(0);
    D_003BC810 = 0;
    return fileSlotSelectPoll;
}

void *fileRestartSlotSelection(void) {
    fileReqBegin(0);
    fileSlotSelectionPollCount = 30;
    D_003BC804 = 1;
    fileSetMenuFlowState(1);
    D_003BC810 = 1;
    return fileSlotSelectPollClear;
}

s32 fileBeginSlotPromptFive(void) {
    D_003BC80C = 1;
    fileSetMenuFlowState(0);
    D_003BC834 = 5;
    return fileBeginPromptDialog(&func_0028B508, &fileRestartSlotSelection, 1);
}

s32 fileBeginSlotPromptSix(void) {
    D_003BC80C = 1;
    fileSetMenuFlowState(0);
    D_003BC834 = 6;
    return fileBeginPromptDialog(&func_0028B508, &fileRestartSlotSelection, 1);
}

void *fileBeginSlotMetadataRefresh(void) {
    fileClearAllSlotFlags();
    fileSetMenuFlowState(0);
    D_003BC854 = 1;
    D_003BC858 = 0;
    fileReqBegin(fileMemoryCardRequestContext);
    return filePollSlotDetectionAndBranch;
}

void *fileResetSelection(void) {
    fileClearAllSlotFlags();
    D_003DC7C0[0] = 0;
    D_003DC7C0[1] = 0;
    D_003DC7C0[2] = 0;
    D_003DC7C0[3] = 0;
    D_003DC7C0[4] = 0;
    D_003DC7C0[5] = 0;
    D_003DC7C0[6] = 0;
    D_003DC7C0[7] = 0;
    D_003DC7C0[8] = 0;
    D_003DC7C0[9] = 0;
    fileReqBegin(0);
    fileSlotSelectionPollCount = 15;
    D_003BC804 = 1;
    fileSetMenuFlowState(1);
    D_003BC810 = 1;
    return fileSlotStatusPoll;
}

u32 fileAbortSlotFlow(void) {
    fileSetMenuFlowState(0);
    D_003BC810 = 0;
    D_003BC80C = 0;
    return 0xffffffff;
}

u32 mcdEnterDefaultFileFlow(void) {
    u32 mode[4];

    mode[0] = 0;
    dds3AdminSubmitModeRequest(2, mode, 4, 0);
    return 0;
}

s32 func_0028B508(void) {
    return mcdEnterDefaultFileFlow();
}

s32 mcdEnterSelectedFileFlow(void) {
    u32 v = 2;

    fileSelectionPending = 1;
    dds3AdminSubmitModeRequest(2, &v, 4, 0);
    return 0;
}

void fileRestartSlotScan(void) {
    fileClearAllSlotFlags();
    fileSetMenuFlowState(1);
    D_003BC854 = 0;
    D_003BC810 = 0;
    fileResetSelection();
}

extern s32 D_003BC82C;
extern void *fileSlotBrowserUpdate(void);

void *fileScanSlotStates(void) {
    s32 i;

    D_003BC82C = 0;
    D_003BC810 = 1;
    for (i = 0; i < 10; i++) {
        u32 buttons = fileReqGetSlotFlags(fileMemoryCardRequestContext, i);

        D_003DC7C0[i] = 0;
        if (buttons & 1) {
            if (buttons & 2) {
                if (buttons & 8) {
                    D_003DC7C0[i] = 1;
                }
            }
        } else if (buttons & 2) {
            if (!(buttons & 8)) {
                D_003DC7C0[i] = 2;
            }
        }
    }
    fileReqBegin(fileMemoryCardRequestContext);
    return fileSlotBrowserUpdate;
}

void *fileResolveAbortSlotFlow(void) {
    if (D_003BC848 == 1 && kwlnTaskGetTaskByName(D_003B2658) == NULL) {
        return (void *)mcdEnterSelectedFileFlow();
    }
    fileSetMenuFlowState(0);
    return fileBeginWait(&fileAbortSlotFlow);
}

void fileLoadIconFileAndResetSelection(void) {
    D_003BD910 = 0;
    fileSaveIconRequest = fileQueueDefaultCallbackRequest(D_003B2668);
    fileResetSelection();
}

void fileResetSlotSelection(void) {
    fileResetSelection();
}

void fileLoadIconFileAndBeginSlotReset(void) {
    D_003BD910 = 0;
    fileSaveIconRequest = fileQueueDefaultCallbackRequest(D_003B2668);
    fileBeginSlotReset();
}

void fileResetSlotPollState(void) {
    fileReqBegin(0);
    D_003BC804 = 1;
    fileSlotSelectionPollCount = 0;
    fileSetMenuFlowState(0);
    D_003BC810 = 0;
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
        if (D_003BC848 == 0) {
            fileClearAllSlotFlags();
            fileSetMenuFlowState(0);
            D_003BC854 = 0;
            D_003BC834 = 2;
            return (void *)fileBeginPromptDialog(fileStartMemoryCardDetection, mnuSelectFileBranch, 1);
        }
        fileSetMenuFlowState(0);
        D_003BC854 = 7;
        D_003BC858 = 0;
        fileReqBegin(fileMemoryCardRequestContext);
        return filePollSlotDetectionAndBranch;
    case 3:
        fileClearAllSlotFlags();
        fileSetMenuFlowState(0);
        D_003BC854 = 2;
        D_003BC858 = 0;
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
    return fileReqGetSize(fileMemoryCardRequestContext) > 0x4C3FF;
}

s32 fileSlotSelectPoll(void) {
    s32 result = fileReqPoll();
    if (result == 0) {
        return result;
    }
    if (fileSlotSelectionPollCount > 0) {
        fileSlotSelectionPollCount--;
        fileReqBegin(fileMemoryCardRequestContext);
        return 0;
    }
    switch (fileReqGetStatus(fileMemoryCardRequestContext)) {
    case 0:
    case 3:
        return fileBeginSlotPromptFive();
    case 2:
        return func_0028B508();
    case 1:
        if (fileIsCardSpaceAboveMinimum() != 0) {
            return func_0028B508();
        }
        return (s32)fileRestartSlotSelection();
    default:
        return 0;
    }
}

extern void *mcClearSlotMetadata(void);

void *fileSlotSelectPollClear(void) {
    if (fileReqPoll() == 0) {
        return NULL;
    }
    if (fileSlotSelectionPollCount > 0) {
        fileSlotSelectionPollCount--;
        fileReqBegin(fileMemoryCardRequestContext);
        return NULL;
    }
    switch (fileReqGetStatus(fileMemoryCardRequestContext)) {
    case 0:
    case 3:
        return (void *)fileBeginSlotPromptFive();
    case 2:
        return (void *)func_0028B508();
    case 1:
        if (fileIsCardSpaceAboveMinimum() != 0) {
            return (void *)func_0028B508();
        }
        return mcClearSlotMetadata();
    default:
        return NULL;
    }
}

extern s8 D_00324510[];

void *filePollSlotDetectionAndBranch(void) {
    s32 reqStatus;

    D_003BC810 = 0;
    if (D_003BC858 == 0) {
        if (fileIsLoadedAndConditionTrue(D_00324510[0x21] < 0) ||
            fileIsLoadedAndConditionTrue(D_00324510[0x23] < 0)) {
            sndSetSequenceVolumePan(0xA, 0x7F, 0x3F);
            D_003BC858 = 1;
        }
    }
    if (fileReqPoll() != 0) {
        if (D_003BC858 == 1) {
            D_003BC854 = 0;
            D_003BC810 = 0;
            if (D_003BC824 != 0) {
                return fileBeginSlotResetPrompt();
            }
            if (D_003BC848 == 0) {
                return fileBeginWait(&fileAbortSlotFlow);
            }
            return mcdEnterSelectedFileFlow();
        }
        reqStatus = fileReqGetStatus(fileMemoryCardRequestContext);
        switch (reqStatus) {
        case 1:
            if (D_003BC854 == 1) {
                fileClearAllSlotFlags();
                fileSetMenuFlowState(1);
                D_003BC854 = 0;
                D_003BC810 = 0;
                return fileResetSelection();
            }
            fileReqBegin(fileMemoryCardRequestContext);
            break;
        case 0:
            fileClearAllSlotFlags();
            if (D_003BC854 != 1) {
                D_003BC854 = 1;
                D_003BC858 = 0;
            }
            fileReqBegin(fileMemoryCardRequestContext);
            break;
        case 2:
            if (D_003BC848 == 0) {
                fileClearAllSlotFlags();
                D_003BC854 = 0;
                D_003BC834 = 2;
                return (void *)fileBeginPromptDialog(&fileStartMemoryCardDetection, &mnuSelectFileBranch, 1);
            }
            if (D_003BC854 != 7) {
                fileClearAllSlotFlags();
                D_003BC854 = 7;
                D_003BC858 = 0;
            }
            fileReqBegin(fileMemoryCardRequestContext);
            break;
        case 3:
            if (D_003BC854 != 2) {
                fileClearAllSlotFlags();
                D_003BC854 = 2;
                D_003BC858 = 0;
            }
            fileReqBegin(fileMemoryCardRequestContext);
            break;
        }
    }
    return NULL;
}

void *filePollSlotScanOrReset(void) {
    if (D_003BC858 == 0) {
        if (fileIsLoadedAndConditionTrue(D_00324510[0x21] < 0) ||
            fileIsLoadedAndConditionTrue(D_00324510[0x23] < 0)) {
            sndSetSequenceVolumePan(8, 0x7F, 0x3F);
            D_003BC858 = 1;
        }
    }
    if (fileReqPoll() != 0) {
        if (D_003BC858 == 1) {
            D_003BC854 = 0;
            return fileScanSlotStates();
        }
        if (fileReqGetStatus(fileMemoryCardRequestContext) != 1) {
            D_003BC854 = 0;
            D_003BC810 = 0;
            return fileResetSelection();
        }
        fileReqBegin(fileMemoryCardRequestContext);
    }
    return NULL;
}

void fileAbortSlotScanOnInput(void) {
    if (fileIsLoadedAndConditionTrue(D_00324510[0x21] < 0) ||
        fileIsLoadedAndConditionTrue(D_00324510[0x23] < 0)) {
        sndSetSequenceVolumePan(8, 0x7F, 0x3F);
        D_003BC854 = 0;
        D_003BC810 = 0;
        fileResetSelection();
    }
}

void *fileUpdateWait(void) {
    s32 remaining = fileWaitTicksRemaining - 1;
    fileWaitTicksRemaining = remaining;
    if (remaining <= 0) {
        if (D_003BD904 == -1) {
            return (void *)-1;
        }
        return ((void *(*)(void))fileWaitContinuation)();
    }
    return NULL;
}

void *fileSetMenuCallbackAndClearResult(u32 callback) {
    D_003BD904 = callback;
    D_003BC858 = 0;
    return mcdFinishFileDetection;
}

void mcdFinishFileDetection(void) {
    if (fileIsLoadedAndConditionTrue(D_00324510[0x21] < 0) ||
        fileIsLoadedAndConditionTrue(D_00324510[0x23] < 0)) {
        fileSetMenuFlowState(0);
        D_003BC854 = 0;
        D_003BC810 = 0;
        func_002E96D8(0x310000);
        if (D_003BD904 == -1) {
            fileBeginWait((void *)-1);
            return;
        }
        ((void (*)(void))D_003BD904)();
    }
}

void *fileBeginDetectionRequest(u32 callback) {
    D_003BD904 = callback;
    D_003BC858 = 0;
    fileReqBegin(fileMemoryCardRequestContext);
    return filePollSlotRequestAndResumeFlow;
}

void *filePollSlotRequestAndResumeFlow(void) {
    s32 fileStatus;

    if (D_003BC858 == 0) {
        if (fileIsLoadedAndConditionTrue(D_00324510[0x21] < 0) ||
            fileIsLoadedAndConditionTrue(D_00324510[0x23] < 0)) {
            sndSetSequenceVolumePan(8, 0x7F, 0x3F);
            D_003BC858 = 1;
        }
    }
    if (fileReqPoll() != 0) {
        if (D_003BC858 == 1) {
            fileSetMenuFlowState(0);
            D_003BC854 = 0;
            D_003BC810 = 0;
            if (D_003BD904 == -1) {
                return fileBeginWait((void *)-1);
            }
            return ((void *(*)(void))D_003BD904)();
        }
        fileStatus = fileReqGetStatus(fileMemoryCardRequestContext);
        switch (fileStatus) {
        case 1:
            if (D_003BC854 == 1) {
                fileClearAllSlotFlags();
                fileSetMenuFlowState(1);
                D_003BC854 = 0;
                D_003BC810 = 0;
                return fileResetSelection();
            }
            fileReqBegin(fileMemoryCardRequestContext);
            break;
        case 0:
            return fileBeginSlotMetadataRefresh();
        case 2:
            if (D_003BC848 == 0) {
                fileClearAllSlotFlags();
                fileSetMenuFlowState(0);
                D_003BC854 = 0;
                D_003BC834 = 2;
                return (void *)fileBeginPromptDialog(&fileStartMemoryCardDetection, &mnuSelectFileBranch, 1);
            }
            if (D_003BC854 != 7) {
                fileClearAllSlotFlags();
                fileSetMenuFlowState(0);
                D_003BC854 = 7;
                D_003BC858 = 0;
            }
            fileReqBegin(fileMemoryCardRequestContext);
            break;
        case 3:
            if (D_003BC854 != 2) {
                fileClearAllSlotFlags();
                fileSetMenuFlowState(0);
                D_003BC854 = 2;
                D_003BC858 = 0;
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
    mcFormatSaveFilename(&buf[1], fileSlotScanIndex);
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
        mcReadDirectoryEntries(fileMemoryCardRequestContext, D_003B2678, D_003DC780, 1);
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

void *fileAdvanceSlotScan(void) {
    fileSlotScanIndex++;
    if (fileSlotScanIndex == 10) {
        fileReqClearSlotMetadataDirty(fileMemoryCardRequestContext);
        fileSlotScanIndex = 0;
        D_003BC804 = 0;
        if (fileCountSelectableFiles() == 0) {
            if (D_003BC848 != 0) {
                D_003BC810 = 0;
                fileSetMenuFlowState(0);
                D_003BC854 = 7;
                D_003BC858 = 0;
                fileReqBegin(fileMemoryCardRequestContext);
                return filePollSlotDetectionAndBranch;
            }
            if (fileIsCardSpaceAboveMinimum() == 0) {
                fileSetMenuFlowState(0);
                D_003BC810 = 0;
                D_003BC854 = 5;
                D_003BC858 = 0;
                fileReqBegin(fileMemoryCardRequestContext);
                return filePollSlotDetectionAndBranch;
            }
        }
        fileSetMenuFlowState(0);
        return fileScanSlotStates();
    }
    return fileBeginReadSlotIcon();
}

void *mcResetSlotMetadata(void) {
    s32 index;
    if (fileReqIsSlotMetadataDirty(fileMemoryCardRequestContext) != 0) {
        fileSetMenuFlowState(1);
        fileSlotScanIndex = 0;
        index = 0;
        do {
            D_003DC7C0[index] = 0;
            fileReqClearSlotFlags(fileMemoryCardRequestContext, index);
            index++;
        } while (index < 10);
        return fileBeginReadSlotIcon();
    } else {
        D_003BC804 = 0;
        fileSetMenuFlowState(0);
        return fileScanSlotStates();
    }
}

void *fileBeginSaveSlotIconScan(void) {
    u8 buf[0x50];

    buf[0] = 0x2F;
    mcFormatSaveFilename(&buf[1], fileSlotScanIndex);
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
        mcReadDirectoryEntries(fileMemoryCardRequestContext, D_003B2678, D_003DC780, 1);
        return mcHandleDirectoryWriteResult;
    }
    if (t == -1) {
        fileSetMenuFlowState(0);
        D_003BC810 = 0;
        D_003BC854 = 0;
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
        D_003BC810 = 0;
        D_003BC854 = 0;
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
                D_003BC810 = 0;
                D_003BC854 = 0;
                return (void *)func_0028B508();
            }
        }
    }
    fileSlotScanIndex = slot;
    if (slot == 10) {
        fileSetMenuFlowState(0);
        D_003BC810 = 0;
        D_003BC854 = 0;
        return (void *)fileBeginSlotPromptSix();
    }
    return fileBeginSaveSlotIconScan();
}

void *mcClearSlotMetadata(void) {
    s32 index = 0;
    u32 *saved;
    fileSetMenuFlowState(1);
    D_003BC810 = 0;
    fileSlotScanIndex = 0;
    fileReqMarkSlotMetadataDirty(fileMemoryCardRequestContext);
    saved = D_003DC7C0;
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
    mcFormatSaveFilename(name + 1, slot);
    mcMakeDirectory(entry, name);
    return mcHandleSearchResult;
}

void *fileCreateMainBegin(void) {
    u8 buf[0x50];
    u32 entry = fileMemoryCardRequestContext;
    s32 v = fileReqGetSelectedSlot(entry);

    buf[0] = 0x2F;
    mcFormatSaveFilename(&buf[1], v);
    mcChangeCurrentDirectory(entry, buf);
    return filePrepareMainBlobWrite;
}

void *mcHandleSearchResult(void) {
    s32 status = mcPollWithExtendedErrors();
    if (status == 0) {
        return NULL;
    }
    if (status == 1) {
        fileReqSetSlotFlags(fileMemoryCardRequestContext, D_003BC844, 2);
        return fileCreateMainBegin();
    }
    fileSetMenuFlowState(0);
    D_003BC854 = 4;
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
        D_003BC854 = 4;
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
        return fileBuildMainBlobAndWrite();
    }
    if (t == -1) {
        fileSetMenuFlowState(0);
        D_003BC854 = 4;
        return fileAbortSlotScanOnInput;
    }
    return NULL;
}

void fileOnAllWritten(void) {
    fileReqSetSlotFlags(fileMemoryCardRequestContext, D_003BC844, 1);
    fileReqSetSlotFlags(fileMemoryCardRequestContext, D_003BC844, 8);
    fileSetMenuFlowState(4);
    fileBeginDetectionRequest((u32)fileScanSlotStates);
}

void *fileWriteIconSysComplete(void) {
    fileDestroyMenuTask();
    return fileOnAllWritten;
}

extern u8 D_0037D0C0[];
extern u32 D_003BC890;
extern u32 D_003BC894;

void *fileBeginLabeledSlotWrite(void) {
    s32 slot = D_003BC844 + 1;
    u8 tens = 0x4F + slot / 10;
    u8 ones = 0x4F + slot % 10;

    D_0037D0C0[0xD0] = 0x82;
    D_0037D0C0[0xD1] = tens;
    D_0037D0C0[0xD2] = 0x82;
    D_0037D0C0[0xD3] = ones;
    return fileBeginRequest(D_003B2678, &D_003BC890, &D_003BC894, fileWriteIconSysComplete, 0);
}

void *fileRequestBaseIcon(void) {
    return fileBeginRequest(D_003B2688, &D_003BD914, &D_003BD918,
                          fileBeginLabeledSlotWrite, &fileSaveIconRequest);
}

INCLUDE_ASM(const s32, "game/code_0028A150", fileBuildMainBlobAndWrite);

void *mcChooseLoadPath(void) {
    u32 entry = fileMemoryCardRequestContext;
    s32 slot = fileReqGetSelectedSlot(entry);
    u32 flags = fileReqGetSlotFlags(entry, slot);
    if (!(flags & 8)) {
        return fileBuildMainBlobAndWrite(entry, D_003B2678);
    }
    mcDeleteFilePath();
    return fileBuildMainBlobAfterDelete;
}

void *fileBeginRequest(const char *name, u32 *first, u32 *second, void *callback, u32 *status) {
    D_003BD928 = first;
    D_003BD92C = second;
    D_003BD930 = callback;
    D_003BD934 = status;
    mcOpenFilePath(fileMemoryCardRequestContext, name, 0x203);
    return fileWriteWaitOpen;
}

extern void mcBeginWrite(s32 request, u32 first, u32 second);
extern void *fileFinishRequest(void);

void *fileWriteWaitOpen(void) {
    s32 status = mcPollCommandStatusWithResult(&fileSaveFileDescriptor);

    if (status == 0) {
        return NULL;
    }
    if (status == 1) {
        if (D_003BD934 != NULL && *D_003BD934 != 0) {
            return fileFinishRequest;
        }
        mcBeginWrite(fileSaveFileDescriptor, *D_003BD928, *D_003BD92C);
        return mcHandleLoadResult;
    }
    if (status == -1) {
        fileSetMenuFlowState(0);
        D_003BC854 = 4;
        return fileAbortSlotScanOnInput;
    }
    return NULL;
}


void *mcHandleLoadResult(void) {
    s32 status = mcPollWriteCompletion();
    if (status == 0) {
        return NULL;
    }
    if (status == 1) {
        mcCloseOpenFile(fileSaveFileDescriptor);
        return mcDispatchReadCallback;
    }
    if (status == -1) {
        fileSetMenuFlowState(0);
        D_003BC854 = 4;
        return fileAbortSlotScanOnInput;
    }
    return NULL;
}

void *mcDispatchReadCallback(void) {
    s32 status = mcPollZeroCommandResult();
    if (status == 0) {
        return NULL;
    }
    if (status == 1) {
        return ((void *(*)(void))D_003BD930)();
    }
    if (status == -1) {
        fileSetMenuFlowState(0);
        D_003BC854 = 4;
        return fileAbortSlotScanOnInput;
    }
    return NULL;
}

void *fileFinishRequest(void) {
    if (fileSaveIconRequest != 0) {
        return NULL;
    }
    mcBeginWrite(fileSaveFileDescriptor, *D_003BD928, *D_003BD92C);
    return mcHandleLoadResult;
}

void *mcHandleDetectionResult(void) {
    s32 status = mcPollStrictSuccess();
    if (status == 0) {
        return NULL;
    }
    func_001005B0();
    if (status < 0) {
        fileSetMenuFlowState(0);
        D_003BC854 = 6;
        return fileAbortSlotScanOnInput;
    }
    fileSetMenuFlowState(6);
    return fileBeginDetectionRequest((u32)fileRestartSlotScan);
}

void *fileStartMemoryCardDetection(void) {
    fileSetMenuFlowState(5);
    func_001005B8();
    func_00289D50(fileMemoryCardRequestContext);
    return mcHandleDetectionResult;
}

s32 mnuSelectFileBranch(void) {
    if (D_003BC824 != 0) {
        D_003BC834 = 7;
        return fileBeginPromptDialog(fileBeginSlotResetPrompt, fileResetSelection, 1);
    }
    D_003BC834 = 7;
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
    mcFormatSaveFilename(&path[1], slot);
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
    D_003BC854 = 3;
    return fileAbortSlotScanOnInput;
}

void *mcHandleSetupResult(void) {
    s32 status = mcPollCompletionStatus();
    if (status == 0) {
        return NULL;
    }
    if (status == 1) {
        mcCloseOpenFile(fileSaveFileDescriptor);
        return mcdHandleSaveSetupDone;
    }
    sdfReleaseResourceAllocation(fileSaveReadBufferResource);
    fileSetMenuFlowState(0);
    D_003BC854 = 3;
    return fileAbortSlotScanOnInput;
}

extern s8 D_003BC7ED;
extern s32 fileLoadStateChanged(void);
extern void fileCacheSlotFlagsFromState(void);
extern void fileRestoreSlotFlagsToState(void);

void *mcdHandleSaveSetupDone(void) {
    s32 status = mcPollZeroCommandResult();
    if (status == 0) {
        return NULL;
    }
    if (status == 1) {
        fileReloadSaveBuffer();
        sdfReleaseResourceAllocation(fileSaveReadBufferResource);
        D_003BC7ED = 1;
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
    D_003BC854 = 3;
    return fileAbortSlotScanOnInput;
}

void *fileSlotBrowserUpdate(void) {
    s32 oldSelection = D_003BC844;
    s32 offset;
    s32 selection;
    s32 action;
    u32 flags;

    if (D_003BC82C == 0) {
        offset = oldSelection - D_003BC840;
        fileLoadMenuState.unk8++;
        fileLoadMenuState.unk8 = fileLoadMenuState.unk8 <= 0 ? 0 : fileLoadMenuState.unk8 > 3 ? 3 : fileLoadMenuState.unk8;

        if (fileIsLoadedWithActiveFlow(((u8)D_00324510[0x26] >> 1) & 1) != 0 && fileLoadMenuState.unk8 >= 3) {
            selection = D_003BC840;
            fileLoadMenuState.unk8 = 0;
            if (selection > 0) {
                if (offset == 2) {
                    offset = 1;
                } else if (offset == 1) {
                    D_003BC840 = selection - 1;
                    fileLoadSetMode(2);
                    fileSetMenuValueAndInitializeFlags(0x80);
                }
            } else {
                offset -= offset > 0;
            }
        }

        if (fileIsLoadedWithActiveFlow(((u8)D_00324510[0x27] >> 1) & 1) != 0 && fileLoadMenuState.unk8 >= 3) {
            selection = D_003BC840;
            fileLoadMenuState.unk8 = 0;
            if (selection < 7) {
                if (offset == 0) {
                    offset = 1;
                } else if (offset == 1) {
                    D_003BC840 = selection + 1;
                    fileLoadSetMode(1);
                    fileSetMenuValueAndInitializeFlags(0x80);
                }
            } else {
                offset += offset < 2;
            }
        }

        D_003BC844 = D_003BC840 + offset;
        if (oldSelection != D_003BC844) {
            sndSetSequenceVolumePan(0, 0x7F, 0x3F);
        }
        if (D_003BC844 < oldSelection) {
            D_003BC818 = -8;
        } else if (oldSelection < D_003BC844) {
            D_003BC818 = 8;
        }
        if (fileIsLoadedAndConditionTrue(D_00324510[0x21] < 0) != 0) {
            D_003BC82C = 1;
        }
        if (fileIsLoadedAndConditionTrue(D_00324510[0x23] < 0) != 0) {
            D_003BC82C = 2;
        }
    }

    if (fileReqPoll() != 0) {
        flags = fileReqGetStatus(fileMemoryCardRequestContext);
        action = D_003BC82C;
        if (flags == 0) {
            return fileBeginSlotMetadataRefresh();
        }

        if (action == 0) {
            fileReqBegin(fileMemoryCardRequestContext);
            return NULL;
        }
        if (action == 1) {

            fileSetMenuFlowState(0);
            if (D_003BC848 == 0) {
        fileReqSetSelectedSlot(fileMemoryCardRequestContext, D_003BC844);
        flags = fileReqGetSlotFlags(fileMemoryCardRequestContext, D_003BC844);
        if (0xB == (flags & 0xB)) {
            sndSetSequenceVolumePan(8, 0x7F, 0x3F);
            if (D_003BC824 != 0) {
                D_003BC820 = action;
                D_003BC834 = action;
                return (void *)fileBeginPromptDialog(fileBeginDirectoryScan, fileScanSlotStates, 1);
            }
            D_003BC820 = action;
            D_003BC834 = action;
            return (void *)fileBeginPromptDialog(mcPrepareDirectory, fileScanSlotStates, 1);
        }

        action = flags & 0xA;
        if (action == 2) {
            sndSetSequenceVolumePan(8, 0x7F, 0x3F);
            D_003BC820 = action;
            return mcPrepareDirectory();
        }

        sndSetSequenceVolumePan(8, 0x7F, 0x3F);
        D_003BC820 = 0;
        if (fileIsCardSpaceAboveMinimum() != 0) {
            return mcPrepareDirectory();
        }
        fileSetMenuFlowState(0);
        D_003BC854 = 5;
        D_003BC858 = 0;
        fileReqBegin(fileMemoryCardRequestContext);
        return filePollSlotScanOrReset;
    }

            fileReqSetSelectedSlot(fileMemoryCardRequestContext, D_003BC844);
            flags = fileReqGetSlotFlags(fileMemoryCardRequestContext, D_003BC844);
            if ((flags & 1) != 0) {
                sndSetSequenceVolumePan(8, 0x7F, 0x3F);
                D_003BC834 = 3;
                return (void *)fileBeginPromptDialog(fileBeginSlotCreate, fileScanSlotStates, 1);
            }
            sndSetSequenceVolumePan(0xA, 0x7F, 0x3F);
            return fileScanSlotStates();
        }

        if (action == 2) {
            sndSetSequenceVolumePan(0xA, 0x7F, 0x3F);
            fileResetLoadContextSlide();
            if (D_003BC824 != 0) {
                return fileBeginSlotResetPrompt();
            }
            return fileResolveAbortSlotFlow();
        }
    }

    return NULL;
}

INCLUDE_RODATA(const s32, "game/code_0028A150", D_003B2658);

INCLUDE_RODATA(const s32, "game/code_0028A150", D_003B2668);

INCLUDE_RODATA(const s32, "game/code_0028A150", D_003B2678);

INCLUDE_RODATA(const s32, "game/code_0028A150", D_003B2688);

INCLUDE_ASM(const s32, "game/code_0028A150", fileDrawSlotListAndPreview);

void *fileRunMenuState(s32 arg) {
    void *next;
    u32 job;
    void *(*cur)(s32);
    D_003BC814++;
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
        D_003BD910 = fileGetResourceHandle(job);
        D_003BD914 = fileGetLoadedDataAddress(job);
        D_003BD918 = fileGetResourceSize(job);
        filePollEntryCleanup(job);
    }
    return NULL;
}

s32 fileDrawMenuFrame(s32 work) {
    evtSetDrawSurfaceIndex(0x52);
    evtSubmitPrimaryAlphaBlendMode(0);
    evtSubmitPrimaryGsTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    if (D_003BC80C != 0) {
        func_00108FA0(0, 0, 0x200, 0x1C0, 0, 0, 0x200, 0x1C0, 0x80808080, 0x80808080, 0x80808080, 0x80808080,
                      D_003BC880);
        func_00108FA0(0, 0, 0x200, 0x1C0, 0, 0, 0x200, 0x1C0, 0x80808080, 0x80808080, 0x80808080, 0x80808080,
                      D_003BC884);
    }
    if (D_003BC810 != 0) {
        fileDrawSlotListAndPreview(work);
    }
    fileShowFlowDialog();
    fileShowPromptDialog();
    fileShowStatusDialog();
    evtSubmitPrimaryAlphaBlendMode(0);
    evtSubmitPrimaryGsTest(1, 5, 0x80, 3, 0, 0, 1, 2);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0028A150", fileEnterMcPackScene);

void func_0028F440(void) {
    fileReleaseMenuResources();
}

void func_0028F458(void) {
}

void fileReleaseMenuResources(void) {
    s32 *slot;
    s32 i;
    s32 world;
    s32 handle;

    if (D_003BC7FC != 0) {
        D_003BC7FC = 0;
        slot = D_0037D488;
        i = 4;
        do {
            handle = *slot;
            i--;
            if (handle != 0) {
                sdfTexReleaseReferenceViaHandler(handle);
                *slot = 0;
            }
            slot++;
        } while (i >= 0);
        if (D_003BC884 != 0) {
            sdfTexReleaseReferenceViaHandler(D_003BC884);
            D_003BC884 = 0;
        }
        if (D_003BC880 != 0) {
            sdfTexReleaseReferenceViaHandler(D_003BC880);
            D_003BC880 = 0;
        }
        if (D_003BC87C != 0) {
            sdfTexReleaseReferenceViaHandler(D_003BC87C);
            D_003BC87C = 0;
        }
        if (D_003BC878 != 0) {
            sdfTexReleaseReferenceViaHandler(D_003BC878);
            D_003BC878 = 0;
        }
        if (D_003BC874 != 0) {
            sdfTexReleaseReferenceViaHandler(D_003BC874);
            D_003BC874 = 0;
        }
        if (D_003BC870 != 0) {
            sdfTexReleaseReferenceViaHandler(D_003BC870);
            D_003BC870 = 0;
        }
        if (D_003BC86C != 0) {
            sdfTexReleaseReferenceViaHandler(D_003BC86C);
            D_003BC86C = 0;
        }
        if (D_003BC868 != 0) {
            sdfTexReleaseReferenceViaHandler(D_003BC868);
            D_003BC868 = 0;
        }
        world = dds3GetWorldObject();
        if (world != 0) {
            dds3SetWorldObjectDataValue(world, 1);
        }
        if (fileSaveIconRequest != 0) {
            fileWaitReady(fileSaveIconRequest);
            D_003BD910 = fileGetResourceHandle(fileSaveIconRequest);
            filePollEntryCleanup(fileSaveIconRequest);
            fileSaveIconRequest = 0;
        }
        sdfReleaseMemorySlot(&D_003BD910);
        kwlnTaskDestroyWithHierarchyByName(D_003B26C8, 1);
        func_001005B0();
    }
}

u32 func_0028F5F8(void) {
    return 1;
}

s32 fileMenuTaskExists(void) {
    return kwlnTaskGetTaskByName(D_003B26C8) != NULL;
}

u32 fileGetSelectionPendingFlag(void) {
    return fileSelectionPending;
}

INCLUDE_ASM(const s32, "game/code_0028A150", func_0028F630);

extern void *D_003BD908;
extern void *D_003BD90C;
extern s32 D_003BC830;
extern s32 D_003BC838;
extern void func_0028F630(void);
s32 fileBeginPromptDialog(void *start, void *finish, s32 mode) {
    D_003BD908 = start;
    D_003BD90C = finish;
    D_003BC830 = mode;
    D_003BC838 = 0;
    if (D_003BC834 != 4 && D_003BC834 != 8) {
        fileReqBegin(fileMemoryCardRequestContext);
    }
    return (s32)func_0028F630;
}

INCLUDE_ASM(const s32, "game/code_0028A150", fileShowPromptDialog);

u32 fileGetLoadSelectionState(void) {
    return D_003BC81C;
}

typedef struct FilePreviewWork {
    u8 pad0[0x15990];
    s16 previewX;
    s16 previewY;
} FilePreviewWork;

void fileSetPreviewLocation(s16 x, s16 y) {
    FilePreviewWork *work = (FilePreviewWork *)datGameState;

    work->previewX = x;
    work->previewY = y;
}

void fileSetMenuValueAndInitializeFlags(u32 value) {
    D_0037D4D0[0] = value;
    fileLoadMenuState.unkC = 0x80;
    fileLoadMenuState.unk4 = 0;
}

INCLUDE_ASM(const s32, "game/code_0028A150", func_002904C8);

void fileCursorStepUp(void) {
    fileLoadMenuState.unk4++;
    fileLoadMenuState.unk4 = fileLoadMenuState.unk4 <= 0 ? 0 : fileLoadMenuState.unk4 > 12 ? 12 : fileLoadMenuState.unk4;
    if (fileLoadMenuState.unk4 >= 7) {
        fileLoadMenuState.unkC += 0x30;
        fileLoadMenuState.unkC = fileLoadMenuState.unkC <= 0 ? 0 : fileLoadMenuState.unkC > 0x80 ? 0x80 : fileLoadMenuState.unkC;
    }
}

void fileFadeStepDown(void) {
    D_0037D4D0[0] -= 0x10;
    D_0037D4D0[0] = D_0037D4D0[0] <= 0 ? 0 : D_0037D4D0[0] > 0x80 ? 0x80 : D_0037D4D0[0];
}

INCLUDE_RODATA(const s32, "game/code_0028A150", D_003B26C8);

INCLUDE_RODATA(const s32, "game/code_0028A150", jtbl_003B26E0);

INCLUDE_RODATA(const s32, "game/code_0028A150", D_003B2718);

void fileDrawSlotIcon(s32 index, s32 x, s32 y, s32 alpha) {
    s32 uv[21][2] = {
        {2, 2},   {2, 2},   {2, 20},  {2, 38},  {2, 56},  {2, 74},  {26, 2},
        {26, 20}, {26, 38}, {26, 56}, {26, 74}, {26, 74}, {26, 74}, {26, 74},
        {26, 74}, {26, 74}, {26, 74}, {26, 74}, {26, 74}, {26, 74}, {26, 74},
    };

    evtSetDrawSurfaceIndex(0x53);
    evtSubmitPrimaryAlphaBlendMode(0);
    evtSubmitPrimaryGsTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    func_00108FA0(x, y, 0x16, 0x10, uv[index][0], uv[index][1], 0x16, 0x10, (alpha << 24) | 0x808080,
                  (alpha << 24) | 0x808080, (alpha << 24) | 0x808080, (alpha << 24) | 0x808080, D_003BC87C);
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

void fileInitCursorPulse(void) {
    fileCursorPulseState.mode[1] = 0;
    D_0037D4AC[0] = 0;
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

INCLUDE_ASM(const s32, "game/code_0028A150", func_00290A88);

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

INCLUDE_RODATA(const s32, "game/code_0028A150", D_003B28A0);

INCLUDE_RODATA(const s32, "game/code_0028A150", D_003B28C0);

INCLUDE_RODATA(const s32, "game/code_0028A150", D_003B28D0);

INCLUDE_RODATA(const s32, "game/code_0028A150", D_003B28E8);

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

s32 fileTestSavedSlotFlags(u32 kind) {
    return fileTestSlotFlagsBit(kind, datGameState + 0xa54);
}

INCLUDE_RODATA(const s32, "game/code_0028A150", D_003B2920);

INCLUDE_RODATA(const s32, "game/code_0028A150", D_003B2940);

INCLUDE_RODATA(const s32, "game/code_0028A150", D_003B2960);

INCLUDE_RODATA(const s32, "game/code_0028A150", D_003B2980);

INCLUDE_RODATA(const s32, "game/code_0028A150", D_003B29A0);

INCLUDE_ASM(const s32, "game/code_0028A150", func_00290FE0);

typedef struct FileConfigListNode {
    u8 pad00[0x58];
    struct FileConfigListNode *next; /* 0x58 */
    u8 pad5C[0x14];
    void *resource;                  /* 0x70 */
} FileConfigListNode;

typedef struct FileConfigList {
    u8 pad00[0x10];
    FileConfigListNode *head; /* 0x10 */
    u8 pad14[0xC];
    s32 count;                /* 0x20 */
} FileConfigList;

/* Save/config task context: its four resource slots start at +0x10. */
typedef struct FileConfigTask {
    void *memory;   /* 0x00 */
    s32 state;      /* 0x04 */
    u8 pad08[4];
    u32 frame;      /* 0x0C: FileConfigList, passed to menu window drawing */
    u32 slots[4];   /* 0x10 */
    u8 pad20[4];
    s32 result;     /* 0x24: negative when the queued load failed */
    u8 pad28[0xC];
    u32 pending;    /* 0x34: zero when no load can start */
    u32 effect;     /* 0x38: effect resource requested for the save scene */
} FileConfigTask;

extern void mnuReleaseEffectResource(u32);
extern s32 mnuAdvanceTitleStateUnderSemaphore(void);

void fileConfigTaskDestroy(void) {
    s32 request = 1;
    FileConfigListNode *node;
    s32 i;

    if (fileConfigTaskWork != 0) {
        fileSavedSlotFlags = ((FileSaveState *)datGameState)->slotFlags;
        if (*(u32 *)(fileConfigTaskWork + 4) == 1) {
            dds3AdminSubmitModeRequest(2, &request, 4, 0);
            mnuReleaseEffectResource(((FileConfigTask *)fileConfigTaskWork)->effect);
            mnuAdvanceTitleStateUnderSemaphore();
        }
        node = ((FileConfigList *)((FileConfigTask *)fileConfigTaskWork)->frame)->head;
        for (i = 0; i < ((FileConfigList *)((FileConfigTask *)fileConfigTaskWork)->frame)->count; i++) {
            sdfReleaseChipBlock(node->resource);
            node = node->next;
        }
        mnuDestroyListState(((FileConfigTask *)fileConfigTaskWork)->frame);
        ((FileConfigTask *)fileConfigTaskWork)->frame = 0;
        for (i = 0; i < 4; i++) {
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

extern s32 func_00290FE0();
extern s32 func_00291418(void);

extern s32 fileStartQueuedLoad(void);
extern u32 fileGetConfigTaskFailure(void);
extern void *kwlnTaskCreate(const char *name, s32 id, s32 optionA, s32 optionB, void *update, void *destroy, s32 data);

void mnuCreateConfigTasks(void) {
    if (fileConfigTaskWork == 0) {
        fileConfigTaskWork = func_00290FE0();
        kwlnTaskCreate(fileConfigInputTaskName, 0x3F2, 1, 1, func_00291418, NULL, fileConfigTaskWork);
        kwlnTaskCreate(fileConfigLoadTaskName, 0x2B07, 1, 1, fileStartQueuedLoad, NULL, fileConfigTaskWork);
        kwlnTaskCreate(fileConfigOwnerTaskName, 0x520B, 1, 1, fileGetConfigTaskFailure, fileConfigTaskDestroy, fileConfigTaskWork);
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

INCLUDE_RODATA(const s32, "game/code_0028A150", fileConfigLoadTaskName);

INCLUDE_RODATA(const s32, "game/code_0028A150", fileConfigOwnerTaskName);

INCLUDE_ASM(const s32, "game/code_0028A150", func_00291418);

s32 fileStartQueuedLoad(void) {
    if (((FileConfigTask *)fileConfigTaskWork)->pending == 0) {
        return 0;
    }
    if (((FileConfigTask *)fileConfigTaskWork)->result < 0) {
        return -1;
    }
    func_00292720((void *)fileConfigTaskWork);
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

INCLUDE_ASM(const s32, "game/code_0028A150", func_002918F8);

INCLUDE_ASM(const s32, "game/code_0028A150", func_00292720);

void fileManagerResetSubsystems(void) {
    func_003003F0(D_003BC900);
    effLoadFlashTextures();
    func_003003F0(D_003BC908);
    effLoadWindTexture();
    func_003003F0(D_003BC910);
    effLoadScalyTexture();
    func_003003F0(D_003BC918);
    fileResetRenderFlags();
    func_003003F0(D_003BC920);
}

void fileSetRenderFlag(u32 bits) {
    effModelUpdateControlFlags |= bits;
}

void fileClearRenderFlag(u32 bits) {
    effModelUpdateControlFlags &= ~bits;
}

void fileResetRenderFlags(void) {
    effModelUpdateControlFlags = 0;
}

u32 func_00292C48(s32 index) {
    return D_0037E130[index];
}

extern u8 sdfViewMatrix[];
extern u8 sdfProjectionMatrix[];
extern u8 D_00324660[];
extern void sdfPostmultiplyVuMatrixFromMemory(void *);

/* vu0 routine: project the point in vf10 through view/projection and screen scale/bias. */
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
    VU0_LOAD_VF(vf11, D_00324660);
    VU0_ADD(vf10, vf10, vf11);
}

/* vu0 routine: tests whether vf10 projects within the camera's forward cone. */
s32 func_00292CE0(void) {
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
    projection = sdfProjectionMatrix;
    sdfPostmultiplyVuMatrixFromMemory(projection);
    VU0_TRANSFORM_POINT(vf10, vf10);
    VU0_PERSPECTIVE_DIVIDE_VF10();
    projection += 16;
    VU0_LOAD_VF(vf11, projection);
    VU0_MUL(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, D_00324660);
    VU0_ADD(vf10, vf10, vf11);
    return 1;
}

extern s32 func_00292CE0();

/* vu0 routine: distance between the projected view point and a second point offset perpendicular to
   the camera axis by rate; 0 when either projection (func_00292CE0) fails. Point comes in vf10. */
f32 mnuMeasureProjectedPerpendicularDistance(f32 rate) {
    f32 scale[4];
    f32 second[4];
    f32 first[4];
    f32 origin[4];
    f32 dx;
    f32 dy;

    VU0_STORE_VF(vf10, origin);
    if (func_00292CE0() == 0) {
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
    if (func_00292CE0() == 0) {
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
void func_00293158(f32 matrix[4][4]) {
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
        u16 index = job->type;
        fileJobTypeOperations[index].destroy(data);
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
    job->option = request->option;
    job->slots[0].selector = request->slots[0].selector;
    job->data = fileJobTypeOperations[job->type].createChild(request->data, job->type);
    return job;
}

void fileJobNotifyPair(FileJob *left, FileJob *right) {
    Cb3714C *entry = &D_0037E14C[right->type];
    if (entry->cbC != NULL) {
        entry->cbC(left->data, right->data);
    }
}

void fileJobNotifyComplete(FileJob *job) {
    u16 idx = ((FileJob *)job)->type;
    void (*cb)(void *) = D_0037E14C[idx].cb10;
    if (cb != NULL) {
        cb(((FileJob *)job)->data);
    }
}

void fileJobInvokeTypeCallback(FileJob *job) {
    u16 idx = ((FileJob *)job)->type;
    void *data = ((FileJob *)job)->data;

    D_0037E14C[idx].cb(data);
}

void fileJobInvokePositionCallback(FileJob *job, void *extra) {
    u16 idx = ((FileJob *)job)->type;
    void (*cb)(void *, void *) = D_0037E14C[idx].cb14;
    if (cb != NULL) {
        cb(((FileJob *)job)->data, extra);
    }
}

void fileJobInvokeRotationCallback(FileJob *job, void *extra) {
    u16 idx = ((FileJob *)job)->type;
    void (*cb)(void *, void *) = D_0037E14C[idx].cb18;
    if (cb != NULL) {
        cb(((FileJob *)job)->data, extra);
    }
}

void fileJobInvokeScaleCallback(FileJob *job, f32 scale) {
    u16 idx = ((FileJob *)job)->type;
    void (*cb)(void *, f32) = D_0037E14C[idx].cb1C;
    if (cb != NULL) {
        cb(((FileJob *)job)->data, scale);
    }
}

void fileDispatchJobTypeCallback(FileJob *job, u32 color) {
    void (*cb)(void *, u32) = D_0037E14C[job->type].cb20;
    if (cb != NULL) {
        cb(job->data, color);
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

void fileJobCopyCommandIntoPrimaryData(FileJob *job, s32 state, u16 option) {
    DevState *command;
    s32 size;
    s32 handle;
    s32 address;

    command = sdfDevCreateCommandState(state);
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

void fileJobCopyCommandIntoSecondaryData(FileJob *job, s32 state, u16 selector) {
    DevState *command;
    s32 size;
    s32 handle;
    s32 address;

    command = sdfDevCreateCommandState(state);
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

INCLUDE_ASM(const s32, "game/code_0028A150", func_00293AE0);

extern u8 sdfPfsDebugMode;
extern char D_003BC928[];
extern char D_003BC930[];
extern char D_003BC938[];
extern char *sdfDevGetPathBuffer(void);
extern s32 func_0030E8F0(const char *path, s32 flags, ...);
extern void func_00293AE0(s32 fd, FileJob *job);
extern void func_0030EB78(s32 fd);
extern void func_00310A68(const char *path, s32 mode);

void fileWriteToPfs(FileJob *job, s32 slot) {
    char path[0xD0];
    s32 fd;

    if (sdfPfsDebugMode != 0) {
        func_003014F0(path, D_003BC928, slot);
        fd = func_0030E8F0(path, 0x602, 0x1B6);
    } else {
        func_003014F0(path, D_003BC930, sdfDevGetPathBuffer(), slot);
        fd = func_0030E8F0(path, 0x602);
    }
    func_00293AE0(fd, job);
    func_0030EB78(fd);
    func_00310A68(D_003BC938, 0);
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

/* View block initialised by fileQueueInitTransform: (0,0,0,1) vectors, grey colour. */
typedef struct FileViewBlock {
    u8 pad00[0x44];
    f32 unk44;
    u8 pad48[0x18];
    f32 unk60;
    u32 color;
    u32 unk68;
    u8 pad6C[8];
    f32 unk74;
} FileViewBlock;

void fileQueueInitTransform(void *queue)
{
    FileViewBlock *view = queue;

    memset(view, 0, 0x80);
    VU0_STORE_VF_UNCLOBBERED(vf0, view);
    VU0_STORE_VF_UNCLOBBERED(vf0, (u8 *)view + 0x10);
    VU0_STORE_VF_UNCLOBBERED(vf0, (u8 *)view + 0x40);
    view->unk44 = -5.0f;
    VU0_STORE_VF_UNCLOBBERED(vf0, (u8 *)view + 0x50);
    view->unk60 = 1.0f;
    view->unk74 = 1.0f;
    view->color = 0x80808080;
    view->unk68 = 0x80;
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
    sdfReleaseChipBlock(job);
}

FileQueue *func_002940D0(FileQueue *source) {
    FileQueue *queue = fileQueueCreate();
    FileJob *entry;
    FileJob *job;
    s128 vec;

    PCP_COPY_VECTOR(queue->offset, source->offset);
    PCP_COPY_VECTOR(queue->axis, source->offset);
    queue->transformValue = source->transformValue;
    queue->unk68 = source->unk68;
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
            strcpy((char *)job->unk9C, (char *)entry->unk9C);
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
                strcpy((char *)job->unk9C, (char *)entry->unk9C);
                fileQueueAppend(queue, job);
                entry++;
            } while (--count != 0);
        }
    }
    VU0_STORE_VF(vf0, &vec);
    fileQueueSetPosition(queue, &vec);
    fileQueueSetRotation(queue, &vec);
    fileQueueSetScale(queue, 1.0f);
    func_00294938(queue, 0x80808080);
    return queue;
}

void func_00294318(u32 unused, u32 handle) {
    func_002940D0((FileQueue *)handle);
}

/* Per-frame update: refreshes the queue rotation when an aim flag (0x60) is set, then repositions and re-notifies every job whose start time (job+0x80) has been reached. */
void fileQueueUpdate(FileQueue *queue)
{
    f32 pos[4];
    f32 aimQuat[4];
    f32 savedQuat[4];
    FileJob *job;
    s32 limit;
    f32 total;

    if (queue->unk68 & 0x60) {
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
    queue->unk84++;
}

void fileQueueDestroy(FileQueue *queue) {
    FileJob *job = queue->first;
    while (job != NULL) {
        FileJob *next = job->next;
        if ((job->flags & 1) == 0) {
            fileJobDestroy(*(FileJob **)((u8 *)job + 0x90));
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
    PCP_COPY_VECTOR((u8 *)queue + 0x10, (u8 *)source + 0x10);
    queue->transformValue = source->transformValue;
    queue->unk68 = source->unk68;
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
    func_00294938(queue, 0x80808080);
    return queue;
}

void fileQueueNotifyAllJobsComplete(FileQueue *queue) {
    FileJob *job;

    for (job = queue->first; job != NULL; job = job->next) {
        fileJobNotifyComplete((void *)job->id);
    }
}

/* Moves the queue to *vec and repositions every job: position = *vec + offset, plus each job's offset rotated by the queue quaternion (scaled when flag 0x80, y lowered by 5 for flag 0x04). */
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

/* Sets the queue rotation to *rot: stores it as queue->quat, rotates queue->axis into queue->offset, notifies each job with its quat multiplied by *rot, then repositions the jobs at queue->position. */
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

INCLUDE_ASM(const s32, "game/code_0028A150", func_00294938);

void fileQueueCopyRotationFromSource(void *dst, void *src) {
    s128 vec;
    func_00293158(src);
    VU0_STORE_VF(vf10, &vec);
    fileQueueSetRotation(dst, &vec);
}

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

FileJob *fileAppendJobFromEntry(FileQueue *queue, void *entry) {
    func_003003F0(D_003BC940);
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

INCLUDE_ASM(const s32, "game/code_0028A150", func_00295018);

INCLUDE_ASM(const s32, "game/code_0028A150", func_002954F0);

INCLUDE_ASM(const s32, "game/code_0028A150", func_002959E8);

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
    s32 count = 0;
    for (job = queue->first; job != NULL; job = job->next) {
        count++;
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
        pitch = -func_002FA1C0(-v[1]);
    }
    func_002E7F20(pitch, func_002FA1F0(v[0], v[2]), 0.0f);
    if (obj->flags & 0x40) {
        VU0_MOVE_VF(vf11, vf10);
        VU0_LOAD_VF(vf10, obj->quat);
        effMiscQuatMultiplyVU();
    }
    VU0_STORE_VF(vf10, dst);
}

LoadObj *fileLoadObjectCreate(void *owner) {
    LoadObj *obj = sdfAllocAndClearQuadwords(0x4C);
    obj->owner = owner;
    obj->color = 0x80808080;
    obj->scale = 1.0f;
    obj->recordWork = NULL;
    obj->deviceHandle = NULL;
    obj->unk38 = 0;
    obj->unk3C = 0;
    obj->unk48 = 1;
    return obj;
}

/* Creates a load object sized by the record's cell count, capped at 0x12C. */
typedef struct FileCellGrid {
    u8 pad00[0x20];
    u32 cols;
    u32 rows;
    u8 pad28[0x90];
    u32 layers;
} FileCellGrid;

LoadObj *fileCreateGridLoaderRecord(FileCellGrid *grid) {
    u32 cols = grid->cols;
    u32 count = (cols != 0 ? cols : grid->rows) * (cols != 0 ? grid->rows : grid->layers);

    return fileLoadObjectCreate((void *)(count <= 0x12C ? count : 0x12C));
}

INCLUDE_RODATA(const s32, "game/code_0028A150", D_003B2A18);

LoadObj *effLoadObjectCreateFromJob(FileJob *job) {
    void *primary = fileResolvePrimaryBuffer(job);
    LoadObj *obj = fileCreateGridLoaderRecord(primary);
    void *secondary;

    fileLoadObjectSetResource(obj, job->option, primary);
    secondary = fileResolveSecondaryBuffer(job);
    if (secondary != NULL) {
        switch (job->slots[0].selector) {
        case 1:
            fileLoadObjectOpenDevice(obj, secondary);
            break;
        case 2:
            fileLoadObjectOpenAndStartDevice(obj, secondary);
            break;
        case 4:
            fileLoadObjectOpenNamedDevice(obj, *(void **)secondary);
            break;
        case 5:
            fileReplaceEffectSurfaceJobs(obj, secondary);
            break;
        case 7:
            fileReplaceReferenceHolder((s32)obj, (u32)secondary);
            break;
        }
        obj->selector = job->slots[0].selector;
    }
    return obj;
}

void effLoadObjectDestroy(LoadObj *obj) {
    if (obj->deviceHandle != NULL) {
        billDispatchByKind(obj->deviceHandle);
    }
    if (obj->unk3C != 0) {
        u32 count = ((FileRecordSlots *)obj->recordWork)->count;
        u32 i;
        for (i = 0; i < count; i++) {
            fileJobDestroy(((FileJob **)obj->unk38)[i]);
        }
        sdfReleaseResourceAllocation(obj->unk3C);
    }
    if (obj->referenceHolder != NULL) {
        effReleaseReferenceHolder((s32)obj->referenceHolder);
    }
    if (obj->recordWork != NULL) {
        fileReleaseGridRecordHandle((s32)obj->recordWork);
    }
    sdfReleaseChipBlock(obj);
}

LoadObj *fileLoadObjectCreateChild(LoadObj *owner) {
    LoadObj *source = (LoadObj *)((FileRecordSlots *)owner->recordWork)->data1;
    LoadObj *result = fileCreateGridLoaderRecord(source);
    fileLoadObjectSetResource(result, ((FileRecordSlots *)owner->recordWork)->type, source);
    fileCloneEffectSurfaceResources(result, owner);
    return result;
}

void fileCloneEffectSurfaceResources(LoadObj *dst, LoadObj *src) {
    u32 selector = src->selector;

    switch (selector) {
    case 1:
    case 2:
    case 4:
        if (dst->deviceHandle != NULL) {
            billDispatchByKind(dst->deviceHandle);
        }
        dst->deviceHandle = billCloneObjectRetainingSharedData(src->deviceHandle);
        billMarkKindOneFlag(dst->deviceHandle);
        if (dst->recordWork != NULL) {
            FileBillboardRecord *record = (FileBillboardRecord *)((FileRecordSlots *)dst->recordWork)->data0;
            billSetBillboardMode(dst->deviceHandle, record->mode);
        }
        break;
    case 5: {
        u32 count = ((FileRecordSlots *)src->recordWork)->count;
        s32 size;
        u32 i;

        if (count == 0) {
            return;
        }
        if (dst->unk3C != 0) {
            for (i = 0; i < count; i++) {
                fileJobDestroy(((FileJob **)dst->unk38)[i]);
            }
            sdfReleaseResourceAllocation(dst->unk3C);
            dst->unk38 = 0;
            dst->unk3C = 0;
        }
        size = count * 4;
        if (size == 0) {
            return;
        }
        dst->unk3C = sdfAllocGeneralBlock(size);
        dst->unk38 = sdfResourceRetainAddress(dst->unk3C);
        for (i = 0; i < count; i++) {
            ((FileJob **)dst->unk38)[i] = fileJobCreateChild(*(FileJob **)src->unk38);
        }
        break;
    }
    case 7:
        if (dst->referenceHolder != NULL) {
            effReleaseReferenceHolder((s32)dst->referenceHolder);
        }
        dst->referenceHolder = effReferenceObjectRetain(src->referenceHolder);
        break;
    }
    dst->selector = src->selector;
}

void fileLoadObjectSetResource(LoadObj *obj, u32 type, void *data) {
    if (obj->recordWork != NULL) {
        fileReleaseGridRecordHandle((s32)obj->recordWork);
    }
    obj->recordWork = fileAllocateGridRecordSlots(type, (u32)obj->owner, data);
}

void fileLoadObjectOpenNamedDevice(LoadObj *obj, void *name) {
    void *handle;
    if (obj->deviceHandle != NULL) {
        billDispatchByKind(obj->deviceHandle);
    }
    handle = effRetainResource(name);
    obj->deviceHandle = handle;
    if (obj->recordWork != NULL) {
        FileBillboardRecord *record = (FileBillboardRecord *)((FileRecordSlots *)obj->recordWork)->data0;
        billSetBillboardMode(handle, record->mode);
    }
}

void fileLoadObjectOpenDevice(LoadObj *obj, void *name) {
    void *handle;
    if (obj->deviceHandle != NULL) {
        billDispatchByKind(obj->deviceHandle);
    }
    handle = billCreateIndexed(0, name);
    obj->deviceHandle = handle;
    if (obj->recordWork != NULL) {
        FileBillboardRecord *record = (FileBillboardRecord *)((FileRecordSlots *)obj->recordWork)->data0;
        billSetBillboardMode(handle, record->mode);
    }
}

void fileLoadObjectOpenAndStartDevice(LoadObj *obj, void *name) {
    if (obj->deviceHandle != NULL) {
        billDispatchByKind(obj->deviceHandle);
    }
    obj->deviceHandle = billCreateIndexed(1, name);
    billMarkKindOneFlag(obj->deviceHandle);
    if (obj->recordWork != NULL) {
        FileBillboardRecord *record = (FileBillboardRecord *)((FileRecordSlots *)obj->recordWork)->data0;
        billSetBillboardMode(obj->deviceHandle, record->mode);
    }
}

void fileReplaceEffectSurfaceJobs(LoadObj *obj, FileJob *job) {
    u32 count = ((FileRecordSlots *)obj->recordWork)->count;
    u32 i;
    s32 size;

    if (obj->unk3C != 0) {
        for (i = 0; i < count; i++) {
            fileJobDestroy(((FileJob **)obj->unk38)[i]);
        }
        sdfReleaseResourceAllocation(obj->unk3C);
        obj->unk38 = 0;
        obj->unk3C = 0;
    }
    size = count * 4;
    if (size != 0) {
        obj->unk3C = sdfAllocGeneralBlock(size);
        obj->unk38 = sdfResourceRetainAddress(obj->unk3C);
        *(FileJob **)obj->unk38 = fileJobCreateFromJob(job);
        for (i = 1; i < count; i++) {
            ((FileJob **)obj->unk38)[i] = fileJobCreateChild(*(FileJob **)obj->unk38);
        }
    }
}

void fileReplaceReferenceHolder(LoadObj *obj, u32 resource) {
    u32 holder;

    if (obj->referenceHolder != NULL) {
        effReleaseReferenceHolder((s32)obj->referenceHolder);
    }
    holder = func_0029C230(resource);
    obj->referenceHolder = (void *)holder;
}

void fileClearLoadObjectReferences(LoadObj *obj) {
    if (obj->recordWork != NULL) {
        fileClearRecordReferences((FileRecordSlots *)obj->recordWork);
        return;
    }
}

void fileAcquireLoadObjectRecord(LoadObj *obj) {
    if (obj->recordWork != NULL) {
        fileAcquireRecord((s32)obj->recordWork);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_0028A150", func_002966D8);

void func_00296E98(s32 object) {
    fileAcquireLoadObjectRecord(object);
    func_002966D8(object);
}

void fileSendLoadObjectRecordVector(LoadObj *obj, u128 *vector) {
    mnuRecordSetVector(obj->recordWork, vector);
}

void fileCopyLoadObjectRecordVector(LoadObj *obj, u128 *vector) {
    fileSetRecordSecondVector(obj->recordWork, vector);
}

void fileSetRecordWordFour(s32 record, u32 value) {
    *(u32 *)(record + 4) = value;
}

void fileSetLoadObjectScale(LoadObj *obj, f32 scale) {
    obj->scale = scale;
    dds3DispatchIndexedCallback(obj->recordWork);
}

void fileResetSlotStates(FileRecordSlots *record) {
    u32 count = record->count;
    u32 index = 0;
    FileRecordSlot *slot = record->slots;
    if (count != 0) {
        do {
            index++;
            slot->state = 0xffffffff;
            slot++;
        } while (index < count);
    }
}

INCLUDE_ASM(const s32, "game/code_0028A150", func_00296F58);

INCLUDE_ASM(const s32, "game/code_0028A150", func_00297270);

/* Output of fileSampleKeyTracks: a view-space position, the sampled frame, colour, scale and heading. */
typedef struct FileKeyOut {
    f32 pos[4];
    s32 frame;
    s32 color;
    f32 scale;
    f32 angle;
} FileKeyOut;

/* Keyframe tracks of a view block (scale, heading and colour curves). */
typedef struct FileKeyBlock {
    u8 pad00[0x2C];
    u8 unk2C[0x24];
    u8 unk50[0x10];
    u8 unk60[0x2C];
    u8 unk8C[0x10];
    u8 mode;
    u8 pad9D[0x1B];
    s32 length;
} FileKeyBlock;

extern s32 func_00296F58(void *, void *, s32, s32);
extern f32 func_00297270(void *, s32, s32);

/* vu0 routine: samples the colour, scale and heading tracks at frame; in mode 2 the heading is the screen-space direction from out->pos to target */
void fileSampleKeyTracks(FileKeyOut *out, FileKeyBlock *block, s32 frame, f32 *target)
{
    f32 delta[4];

    out->color = func_00296F58(block->unk2C, block->unk50, frame, block->length);
    out->scale = func_00297270(block->unk60, frame, block->length);
    if (block->mode != 2) {
        out->angle = func_00297270(block->unk8C, frame, block->length);
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
        out->angle = func_002FA1F0(delta[1], delta[0]);
    }
    VU0_MOVE_VF(vf28, vf20);
    VU0_MOVE_VF(vf29, vf21);
    VU0_MOVE_VF(vf30, vf22);
    VU0_MOVE_VF(vf31, vf23);
}

typedef struct FileGridDimensions {
    u8 pad0[0xC0];
    s32 columns;
    s32 rows;
} FileGridDimensions;

typedef struct FileSlotGroup {
    u8 pad0[4];
    u32 first;
    u8 pad8[0x10];
    u8 *slots;
    u8 pad1C[4];
    FileGridDimensions *dims;
} FileSlotGroup;

/* Invalidates the cells x rows slots of the group that starts at `slot`. */
void fileInvalidateSlotGroup(FileSlotGroup *group, u8 *slot) {
    FileGridDimensions *dims = group->dims;
    s32 rows = dims->rows;
    s32 columns = dims->columns;
    s32 count = columns * rows;
    FileRecordSlot *p;
    s32 i;

    if (count != 0) {
        u8 *base = group->slots;
        p = (FileRecordSlot *)(base + ((group->first + ((u32)(slot - base) >> 5) * count) << 5));
        for (i = 0; i < count; i++) {
            p->state = -1;
            p++;
        }
    }
}

/* Same, but first copies the slot at `slot` over the group's first slot. */
void fileCopyAndInvalidateSlotGroup(FileSlotGroup *group, u8 *slot) {
    FileGridDimensions *dims = group->dims;
    s32 rows = dims->rows;
    s32 columns = dims->columns;
    s32 count = columns * rows;
    FileRecordSlot *p;

    if (count != 0) {
        u8 *base = group->slots;
        p = (FileRecordSlot *)(base + ((group->first + ((u32)(slot - base) >> 5) * count) << 5));
        *p = *(FileRecordSlot *)slot;
        p->state = -1;
    }
}

INCLUDE_ASM(const s32, "game/code_0028A150", func_00297658);

INCLUDE_ASM(const s32, "game/code_0028A150", func_00297CB0);

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

INCLUDE_ASM(const s32, "game/code_0028A150", func_002985D0);

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

INCLUDE_ASM(const s32, "game/code_0028A150", func_00298D28);

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

INCLUDE_ASM(const s32, "game/code_0028A150", func_002995F8);

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

INCLUDE_ASM(const s32, "game/code_0028A150", func_00299E58);

void effLoadObjScaleParamsB(ScaleOwner *owner, f32 scale) {
    ScaleSet *src = owner->src;
    ScaleSet *dst = owner->dst;
    u32 i;

    dst->unk64 = src->unk64 * scale;
    dst->unk68 = src->unk68 * scale;
    for (i = 0; i < 3; i++) {
        dst->entries[i].value = src->entries[i].value * scale;
    }
    dst->unkC8 = src->unkC8 * scale;
    dst->unkD8 = src->unkD8 * scale;
    dst->unkE0 = src->unkE0 * scale;
    dst->unkE4 = src->unkE4 * scale;
}

void *fileAllocateGridRecordSlots(u16 type, u32 count, void *data) {
    FileGridDimensions *src = data;
    u32 slotCount = count * src->columns * src->rows + count;
    u32 slotBytes = slotCount << 5;
    s32 dataBytes = D_0037E550[type].dataBytes;
    u32 headerSize = 0x30;
    u32 size;
    u32 handle;
    FileRecordSlots *rec;
    u8 *body;
    u8 *vec;

    size = slotBytes + headerSize;
    size += D_0037E550[type].slotBytes * count;
    size += dataBytes * 2;
    handle = sdfAllocGeneralBlock(size);
    rec = sdfResourceRetainAddress(handle);
    body = (u8 *)rec + headerSize;
    rec->type = type;
    rec->slots = (FileRecordSlot *)body;
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
    rec->unk14 = 0;
    memcpy(rec->data0, src, dataBytes);
    memcpy(rec->data1, src, dataBytes);
    vec = rec->data0;
    VU0_STORE_VF($vf0, vec);
    VU0_STORE_VF($vf0, vec + 0x10);
    if (vec[0xBC] != 0) {
        rec->flags |= 1;
    }
    return rec;
}

void fileReleaseGridRecordHandle(s32 record) {
    sdfReleaseResourceAllocation(((FileRecordSlots *)record)->handle);
}

void fileClearRecordReferences(FileRecordSlots *record) {
    record->references = 0;
}

void fileAcquireRecord(FileRecordSlots *record) {
    if (record->references == 0) {
        fileResetSlotStates(record);
    }
    D_0037E550[record->type].acquire(record);
    record->references++;
}

void fileReadVectorPtr20(void *record, u128 *out) {
    PCP_COPY_VECTOR(out, *(u128 **)((u8 *)record + 0x20));
}

void mnuRecordSetVector(void *record, const u128 *value) {
    PCP_COPY_VECTOR(*(u128 **)((u8 *)record + 0x20), value);
}

void fileReadRecordSecondVector(void *record, u128 *out) {
    PCP_COPY_VECTOR(out, *(u128 **)((u8 *)record + 0x20) + 1);
}

void fileSetRecordSecondVector(void *record, const u128 *value) {
    PCP_COPY_VECTOR(*(u128 **)((u8 *)record + 0x20) + 1, value);
}

INCLUDE_SDATA(const s32, "game/code_0028A150", fileMemoryCardRequestContext);

INCLUDE_SDATA(const s32, "game/code_0028A150", fileMenuTaskAlive);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC7ED);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC7F0);

INCLUDE_SDATA(const s32, "game/code_0028A150", fileSaveIconRequest);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC7FC);

INCLUDE_SDATA(const s32, "game/code_0028A150", fileWaitTicksRemaining);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC804);

INCLUDE_SDATA(const s32, "game/code_0028A150", fileSlotSelectionPollCount);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC80C);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC810);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC814);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC818);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC81C);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC820);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC824);

INCLUDE_SDATA(const s32, "game/code_0028A150", fileSlotScanIndex);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC82C);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC830);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC834);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC838);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC83C);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC840);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC844);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC848);

INCLUDE_SDATA(const s32, "game/code_0028A150", fileSelectionPending);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC850);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC854);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC858);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC85C);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC860);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC864);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC868);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC86C);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC870);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC874);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC878);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC87C);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC880);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC884);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC888);

INCLUDE_SDATA(const s32, "game/code_0028A150", fileSaveHighlightPhase);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC890);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC894);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC898);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC8A0);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC8A8);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC8B0);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC8B8);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC8C0);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC8D0);

INCLUDE_SDATA(const s32, "game/code_0028A150", fileConfigTaskState);

INCLUDE_SDATA(const s32, "game/code_0028A150", fileSlotFlagMirror);

INCLUDE_SDATA(const s32, "game/code_0028A150", fileSavedSlotFlags);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC8E0);

INCLUDE_SDATA(const s32, "game/code_0028A150", fileConfigInputTaskName);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC8F0);

INCLUDE_SDATA(const s32, "game/code_0028A150", effModelUpdateControlFlags);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC900);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC908);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC910);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC918);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC920);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC928);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC930);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC938);

INCLUDE_SDATA(const s32, "game/code_0028A150", D_003BC940);

