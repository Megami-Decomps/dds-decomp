#include "common.h"
#include "pcp_vu0.h"
#include "kwln.h"



extern void func_00104068(s32, u8, s32);
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
extern void func_00110860();
extern void fileWaitReady();
extern void sdfReleaseMemorySlot();
extern u8 D_00324690[];
extern u8 D_00324680[];
extern u8 D_003246A0[];
extern f32 func_002FA1C0(f32);
extern f32 func_002FA1F0(f32, f32);
extern void func_002E7F20(f32, f32, f32);
extern void effMiscQuatMultiplyVU(void);







extern void *func_0028D978(void);
extern s32 mnuSelectFileBranch(void);






extern u8 D_003DC800[];

extern void evtSubmitGsRegister47(s32, s32, s32, s32, s32, s32, s32, s32);
extern s32 D_003BC880;

extern void *fileReadSlotPreviewBegin(void);
extern void *fileAdvanceSlotScan(void);
extern void fileReqSetSelectedSlot(u32 ctx, s32 arg);

extern void *(*D_003BD8FC)(s32);
extern s32 D_003BC814;
extern s32 func_00288BA8(u32, void *);
extern u32 fileGetResourceHandle(u32);
extern u32 func_00288B90(u32);
extern u32 fileGetResourceSize(u32);
extern void func_002887A0(u32);
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

extern s32 func_002D03F8();

extern s32 sdfResourceRetainAddress();

extern s64 sdfDevCreateCommandState(u64);

extern u64 func_002E5C88(s64);

extern u32 D_003BC8F8;

extern s32 D_003BD938;

extern u32 D_003BC888;

extern void func_003014F0(void *dst, const char *fmt, ...);

extern u32 D_003BD924;

extern u32 D_003BD8EC;

extern u32 func_00197760(s32, s32, s32, u32, u32, s32);

extern void kwlnFadeInStart(s32, s32, s32, s32);

extern void *D_003BD900;

extern s32 D_003BC800;

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

extern void func_00289F80(u32, const char *, s32);

extern void func_00289F10(void);

extern void *fileWriteWaitOpen(void);

extern void *mcHandleLoadResult(void);

extern void *func_0028C8F8(void);

extern void *fileBuildMainBlobAfterDelete(void);

extern s32 D_003BD91C;

extern u32 D_003BD920;

extern s32 func_0028A088(void);

extern void func_0028A008(s32);

extern void *fileStoreSlotHeader(void);

extern s32 func_00289E38(void);

extern void *fileCreateMainBegin(void);

extern s32 D_003BAA00;

extern void *fileBeginSlotResetPrompt(void);

extern void *mcPrepareDirectory(void);

extern void *fileSlotSelectPollClear(void);

extern void *fileBeginDirectoryScan(void);

typedef struct LoadMirror {
    u32 current;
    u32 previous;
} LoadMirror;

extern LoadMirror D_003BC8D8;

extern u32 D_003BC8DC;

extern char D_003BC8E8[];

extern char D_003B29D8[]; /* "config_draw" */

extern char D_003B29E8[]; /* "config_update" */

extern void kwlnTaskDestroyWithHierarchyByName(const char *name, s32 hierarchy);

extern void func_003003F0(void *arg);

extern char D_003BC900[];

extern char D_003BC908[];

extern char D_003BC910[];

extern char D_003BC918[];

extern char D_003BC920[];

extern u32 D_003BC81C;

extern u32 D_003BC84C;

extern u32 D_003BC854;

extern u32 D_003BC80C;

extern u32 D_003BC810;

extern u32 D_003BC7E8;

extern s32 D_003BC850;

extern s32 D_003BC860;

extern u32 D_003BC858;

extern s32 D_003BC864;

extern s8 D_003DC803[];

extern u32 D_003BD904;

extern u32 D_003BD910;

extern u32 D_003BC7F8;

extern u64 func_001978E8(s32, s32, u64, u64, u64, u64);

extern u32 D_003BD8F0;

extern u32 func_001951C8(u32, u32, u32, u32, u32);

extern s32 D_003BC7FC;

extern s8 D_003BC7EC;

extern s32 D_003BC7F0;

/* Loader context at D_0037D4A0. */
typedef struct LoadCtx374A0 {
    u8 unk0[4]; /* 0x00 */
    s32 unk4;   /* 0x04 */
    u8 unk8[4]; /* 0x08 */
    s32 unkC;   /* 0x0C */
    s8 unk10;   /* 0x10 */
    u8 pad11[3]; /* 0x11 */
    s32 unk14;  /* 0x14 */
    s32 unk18;  /* 0x18 */
    s32 unk1C;  /* 0x1C */
} LoadCtx374A0;

extern LoadCtx374A0 D_0037D4A0;

/* Far scalar: incomplete array forces non-small-data addressing. */
extern s32 D_0037D4D0[];

extern u32 D_0037E130[];

extern char D_003B2668[];

extern char D_003B26C8[];


extern KwlnTask *kwlnTaskGetTaskByName(const char *name);

extern u32 func_00288B48(const char *path);

extern void dds3DispatchIndexedCallback(void *callback);

extern void func_002966D8(s32 object);

extern void *fileResetSelection(void);
extern void sndSetSequenceVolumePan(s32, s32, s32);

extern void *fileBeginSlotReset(void);

extern s32 fileSlotSelectPoll(void);

extern void fileOnAllWritten(void);

extern void func_0028BE78(void);

extern s32 D_003BC808;

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

extern void func_0028BF38(void);

extern s32 fileReqGetSize(s32 request);

extern u32 D_003BC804;

extern void func_001005B8(void);

extern void func_00289D50(u32 handle);

extern void *mcHandleDetectionResult(void);

extern void func_00293EA0(void *queue);

extern void func_00293158(void *src);

extern void func_00294798(void *dst, void *src);

extern void *func_002CFEB8(s32 size);

extern void *func_002CFF68(s32 size);

extern void func_002CFF98();

extern void fileClearRecordReferences(FileRecordSlots *record);

extern void mnuRecordSetVector(void *record, const u128 *vector);

extern void func_0029A7F8(void *record, const u128 *vector);

extern void func_001028E8(s32 kind, void *data, s32 size, s32 flags);

extern s32 D_003BC848;

extern s32 D_003BC828;

extern char D_003B2658[];

extern void *fileBeginWait(void *callback);

extern void mcFormatSaveFilename(void *dst, s32 number);

extern void func_00289DA8(u32 request, void *data);

extern void *fileScanSlotIconSysBegin(void);

extern s32 mcPollSyncResult(void);

extern void func_00289E80(u32 request, const char *path, void *data, s32 option);

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

extern void func_0028BAC0(void);
extern void *fileLoadMainBlobBegin(void);
extern void *mcHandleSetupResult(void);

extern void *filePrepareMainBlobWrite(void);

extern void *mcChooseLoadPath(void);

extern void fileAbortSlotScanOnInput(void);

extern void *mcDispatchReadCallback(void);

extern void *mcHandleSearchResult(void);

extern s32 func_00289F30(void);

extern void *fileBuildMainBlobAndWrite();

extern u8 fileReqIsSlotMetadataDirty(s32 request);

extern u32 D_003DC7C0[];

extern void billDispatchByKind(void *handle);

extern void *effRetainResource(void *name);

extern void *billCreateIndexed(s32 mode, void *name);

extern void func_001523B0(void *handle);

extern void billSetBillboardMode(void *handle, s16 index);

extern void *func_0029A5E0(u16 type, u32 count, void *src);

extern void *func_00151E60(void *handle);

extern void *func_0029C408(void *holder);

/* Init record at D_0037D4E0. */
typedef struct Init374E0 {
    s8 mode[3];      /* 0x00 */
    u8 pad3;         /* 0x03 */
    s32 pos[3][2];   /* 0x04 */
    s32 counter[3];  /* 0x1C */
    u8 pad28[0xC];   /* 0x28 */
    s32 alpha[3];    /* 0x34 */
} Init374E0;

extern Init374E0 D_0037D4E0;

extern u32 D_0037D4AC[];

/* Callback table at D_0037E14C (0x28 bytes per entry). */
typedef struct Cb3714C {
    void (*cb)(void *arg);    /* 0x00 */
    u8 pad4[8];               /* 0x04 */
    void (*cbC)(void *, void *); /* 0x0C */
    void (*cb10)(void *arg);  /* 0x10 */
    void (*cb14)(void *arg);  /* 0x14 */
    void (*cb18)(void *arg, void *extra);  /* 0x18 */
    void (*cb1C)(void *arg);  /* 0x1C */
    void (*cb20)(void *arg);  /* 0x20 */
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

extern FileTypeCallbacks D_0037E148[];

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
    void *recordWork;     /* 0x44: created by func_0029A5E0 */
    s16 unk48;          /* 0x48 */
    u16 unk4A;
} LoadObj;

extern LoadObj *func_00295F58(void *source);

extern void *mcdHandleSaveSetupDone(void);

extern s32 fileWaitCommandDone(void);

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
    u8 unk30[0x60];
    u32 id;
    u32 sector;
    u32 flags;
    u8 unk9C[0x10];
    struct FileJob *next;
    struct FileJob *prev;
} FileJob;

extern FileJob *fileCreateJob(u16 type);

extern void fileJobFreePrimaryBuffer(FileJob *job);

extern void fileJobFreeSecondaryBuffer(FileJob *job);

extern FileJob *fileJobCreate(void);

extern void func_0029A730(s32 record);

typedef struct FileQueue {
    u8 unk0[0x80];
    s32 count;
    u32 unk84;
    FileJob *head;
    FileJob *tail;
} FileQueue;

extern void fileQueueAppend(FileQueue *queue, FileJob *job);

extern s8 D_003BC8D5;

void fileWriteBegin(s32 request, u32 first, u32 second) {
    func_002F6670();
}

extern s32 func_002F6858(s32, s32 *, s32 *);

/* Poll the memory-card write: busy is 0, success 1, and the card's
 * -4 status is translated to the menu's -2 error. */
s32 fileWriteWait(void) {
    s32 cmdId;
    s32 status;
    s32 result = func_002F6858(1, &cmdId, &status);

    if (result == 1) {
        if (status >= 0) {
            return result;
        }
        if (status == -4) {
            return -2;
        }
        return -1;
    }
    return 0;
}

void fileDestroyMenuTask(void) {
    if (D_003BC7F0 != 0) {
        kwlnTaskDestroyWithHierarchy(D_003BC7F0, 1);
        D_003BC7F0 = 0;
        D_003BC7EC = 0;
    }
}

s8 fileMenuTaskIsAlive(void) {
    return D_003BC7EC;
}

void func_0028A188(void) {
}

void mcFormatSaveFilename(void *dst, s32 number) {
    func_003014F0(dst, "BASLUS-%05d-new-%d", D_003BC888, number);
}

void fileReqGetSlotCode(void) {
    s32 slot = fileReqGetSelectedSlot(D_003BC7E8);
    D_003BC864 = D_003DC803[slot * 0x30];
}

/* Size of the persistent main save block copied during a reload. */
#define FILE_MAIN_BLOB_SIZE 0x33600

u32 fileMainBlobSize(void) {
    return FILE_MAIN_BLOB_SIZE;
}

void fileReloadSaveBuffer(void) {
    s32 saved = *(s32 *)(D_003BAA00 + 0x30);
    s32 size = FILE_MAIN_BLOB_SIZE;
    memcpy((void *)D_003BAA00, (void *)D_003BD924, size);
    *(s32 *)(D_003BAA00 + 0x30) = saved;
}

u8 fileIsLoadedAndConditionTrue(s32 loaded) {
    return loaded != 0 && D_003BC7FC == 1;
}

u8 func_0028A268(s32 loaded) {
    return loaded != 0 && D_003BC7FC == 1;
}

void func_0028A288(s32 x, s32 y, u32 first, u32 second) {
    D_003BD8EC = func_00197760(x << 4, y << 3, 0, first, second, 0);
    func_00195880(D_003BD8EC, 1);
    func_00194920(D_003BD8EC);
}

void mcdCreateFontDrawHandle(s32 x, s32 y, u32 color, u32 font) {
    func_00195520(1);
    D_003BD8F0 = func_001951C8(font, 0, 0, 0, 0);
    func_00195530(1);
    frFontSetFlagAndMeasureGlyphs(D_003BD8F0, 1);
    func_00195450(D_003BD8F0, x << 4, y << 3);
    frFontSetChildColors(D_003BD8F0, color);
    func_001958A0(D_003BD8F0, 0, 0x56);
    func_00194920(D_003BD8F0);
    func_00195548(0x54);
}

void fileDrawMenuImageAtPoint(s32 x, s32 y, u64 first, u64 second) {
    u64 imageHandle;

    imageHandle = func_001978E8(x << 4, y << 3, 0, first, second, 0);
    func_00195880(imageHandle, 1);
    func_00194920(imageHandle);
}

extern f32 D_003BC88C;
extern f32 sdfSinPoly(f32);
extern s32 itfMesGetGlobalWindowValue(void);

/* Animate the save-window highlight's alpha with a sinusoidal phase. */
void fileDrawPulsingSaveHighlight(void) {
    s32 angle;
    f32 wave;

    evtSetDrawSurfaceIndex(0x56);
    func_00108CB8(0);
    D_003BC88C = D_003BC88C + 0.39999998f;
    sdfSinPoly(D_003BC88C);
    angle = ((s32)D_0037D4D0[2] + 8) % 360;
    D_0037D4D0[2] = angle;
    wave = sdfSinPoly((f32)((angle + 0x5A) % 360) / 180.0f * 3.1415899f);
    D_0037D4D0[3] = (s32)((wave + 1.0f) * 0.5f * 191.0f + 64.0f);
    func_00108FA0(0x1BE, 0x12C, 0x13, 0x1F, 1, 1, 0x13, 0x1F, (D_0037D4D0[3] << 24) | 0xAEC014,
                  (D_0037D4D0[3] << 24) | 0xAEC014, (D_0037D4D0[3] << 24) | 0xAEC014,
                  (D_0037D4D0[3] << 24) | 0xAEC014, itfMesGetGlobalWindowValue());
}

extern void evtSetDrawSurfaceIndex(s32);
extern void func_00108CB8(s32);
extern void func_00108FA0(s32, s32, s32, s32, s32, s32, s32, s32, u32, u32, u32, u32, s32);
extern void fileCursorPulseUpdate(void);
extern void func_00290A88(s32, s32, s32);
extern s32 D_003BC85C;
extern s32 D_003BC884;

void fileDrawSaveWindow(void) {
    evtSetDrawSurfaceIndex(0x56);
    func_00108CB8(0);
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

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028A5E8);

INCLUDE_ASM(const s32, "game/code_0028A0E0", fileShowStatusDialog);

void fileSetMenuFlowState(u32 state) {
    s32 previous;

    previous = D_003BC850;
    D_003BC850 = state;
    if (previous == 0) {
        D_003BC860 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028AB48);

void fileClearAllSlotFlags(void) {
    fileReqMarkSlotMetadataDirty(D_003BC7E8);
    fileReqClearSlotFlags(D_003BC7E8, 0);
    fileReqClearSlotFlags(D_003BC7E8, 1);
    fileReqClearSlotFlags(D_003BC7E8, 2);
    fileReqClearSlotFlags(D_003BC7E8, 3);
    fileReqClearSlotFlags(D_003BC7E8, 4);
    fileReqClearSlotFlags(D_003BC7E8, 5);
    fileReqClearSlotFlags(D_003BC7E8, 6);
    fileReqClearSlotFlags(D_003BC7E8, 7);
    fileReqClearSlotFlags(D_003BC7E8, 8);
    fileReqClearSlotFlags(D_003BC7E8, 9);
}

void *fileBeginSlotOpen(void) {
    char path[0x50];
    u32 ctx;
    s32 slot;
    s32 len;

    fileReqSetSelectedSlot(D_003BC7E8, D_003BC828);
    ctx = D_003BC7E8;
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
    func_00289F80(ctx, path, 1);
    return fileReadSlotPreviewBegin;
}

extern s32 func_00289FA8(s32 *);
extern void func_0028A070(s32, u32, s32);
extern void *fileReadSlotPreviewWait(void);
void *fileReadSlotPreviewBegin(void) {
    s32 status = func_00289FA8(&D_003BD91C);

    if (status == 0) {
        return NULL;
    }
    if (status == 1) {
        D_003BD920 = func_002D03F8(0x30);
        D_003BD924 = sdfResourceRetainAddress(D_003BD920);
        func_0028A070(D_003BD91C, D_003BD924, 0x30);
        return fileReadSlotPreviewWait;
    }
    return fileBeginSlotMetadataRefresh();
}

void *fileReadSlotPreviewWait(void) {
    s32 status = func_0028A088();
    if (status == 0) {
        return NULL;
    }
    if (status == 1) {
        func_0028A008(D_003BD91C);
        return fileStoreSlotHeader;
    }
    func_002D0918(D_003BD920);
    return fileBeginSlotMetadataRefresh();
}

void *fileStoreSlotHeader(void) {
    s32 status = fileWaitCommandDone();

    if (status == 0) {
        return NULL;
    }
    if (status == 1) {
        memcpy(D_003DC800 + D_003BC828 * 0x30, (void *)D_003BD924, 0x30);
        func_002D0918(D_003BD920);
        return fileAdvanceSlotScan();
    }
    func_002D0918(D_003BD920);
    return fileBeginSlotMetadataRefresh();
}

void *fileBeginWait(void *callback) {
    kwlnFadeInStart(0, 0, 0, 15);
    D_003BD900 = callback;
    D_003BC800 = 20;
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
    D_003BC808 = 30;
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
    fileReqBegin(D_003BC7E8);
    return func_0028BAC0;
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
    D_003BC808 = 15;
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
    func_001028E8(2, mode, 4, 0);
    return 0;
}

s32 func_0028B508(void) {
    return mcdEnterDefaultFileFlow();
}

s32 func_0028B520(void) {
    u32 v = 2;

    D_003BC84C = 1;
    func_001028E8(2, &v, 4, 0);
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
        u32 buttons = fileReqGetSlotFlags(D_003BC7E8, i);

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
    fileReqBegin(D_003BC7E8);
    return fileSlotBrowserUpdate;
}

void func_0028B658(void) {
    if (D_003BC848 == 1 && kwlnTaskGetTaskByName(D_003B2658) == NULL) {
        func_0028B520();
    } else {
        fileSetMenuFlowState(0);
        fileBeginWait(&fileAbortSlotFlow);
    }
}

void func_0028B6A8(void) {
    D_003BD910 = 0;
    D_003BC7F8 = func_00288B48(D_003B2668);
    fileResetSelection();
}

void func_0028B6D0(void) {
    fileResetSelection();
}

void func_0028B6E8(void) {
    D_003BD910 = 0;
    D_003BC7F8 = func_00288B48(D_003B2668);
    fileBeginSlotReset();
}

void fileResetSlotPollState(void) {
    fileReqBegin(0);
    D_003BC804 = 1;
    D_003BC808 = 0;
    fileSetMenuFlowState(0);
    D_003BC810 = 0;
    fileReturnToSlotSelection();
}

void *fileSlotStatusPoll(void) {
    s32 status;

    if (fileReqPoll() == 0) {
        return NULL;
    }
    if (D_003BC808 > 0) {
        D_003BC808--;
        fileReqBegin(D_003BC7E8);
        return NULL;
    }
    status = fileReqGetStatus(D_003BC7E8);
    switch (status) {
    case 0:
        return fileBeginSlotMetadataRefresh();
    case 2:
        if (D_003BC848 == 0) {
            fileClearAllSlotFlags();
            fileSetMenuFlowState(0);
            D_003BC854 = 0;
            D_003BC834 = 2;
            return (void *)fileBeginPromptDialog(func_0028D978, mnuSelectFileBranch, 1);
        }
        fileSetMenuFlowState(0);
        D_003BC854 = 7;
        D_003BC858 = 0;
        fileReqBegin(D_003BC7E8);
        return func_0028BAC0;
    case 3:
        fileClearAllSlotFlags();
        fileSetMenuFlowState(0);
        D_003BC854 = 2;
        D_003BC858 = 0;
        fileReqBegin(D_003BC7E8);
        return func_0028BAC0;
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
        u32 flags = fileReqGetSlotFlags(D_003BC7E8, index);
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
    return fileReqGetSize(D_003BC7E8) > 0x4C3FF;
}

s32 fileSlotSelectPoll(void) {
    s32 result = fileReqPoll();
    if (result == 0) {
        return result;
    }
    if (D_003BC808 > 0) {
        D_003BC808--;
        fileReqBegin(D_003BC7E8);
        return 0;
    }
    switch (fileReqGetStatus(D_003BC7E8)) {
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
    if (D_003BC808 > 0) {
        D_003BC808--;
        fileReqBegin(D_003BC7E8);
        return NULL;
    }
    switch (fileReqGetStatus(D_003BC7E8)) {
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

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028BAC0);

extern s8 D_00324510[];

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
        if (fileReqGetStatus(D_003BC7E8) != 1) {
            D_003BC854 = 0;
            D_003BC810 = 0;
            return fileResetSelection();
        }
        fileReqBegin(D_003BC7E8);
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
    s32 remaining = D_003BC800 - 1;
    D_003BC800 = remaining;
    if (remaining <= 0) {
        if (D_003BD904 == -1) {
            return (void *)-1;
        }
        return ((void *(*)(void))D_003BD900)();
    }
    return NULL;
}

void *func_0028BE60(u32 callback) {
    D_003BD904 = callback;
    D_003BC858 = 0;
    return func_0028BE78;
}

void func_0028BE78(void) {
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
    fileReqBegin(D_003BC7E8);
    return func_0028BF38;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028BF38);

void *fileBeginReadSlotIcon(void) {
    u8 buf[0x50];

    buf[0] = 0x2F;
    mcFormatSaveFilename(&buf[1], D_003BC828);
    func_00289DA8(D_003BC7E8, buf);
    return fileScanSlotIconSysBegin;
}

void *fileScanSlotIconSysBegin(void) {
    s32 t = mcPollSyncResult();

    if (t == 0) {
        return NULL;
    }
    if (t == 1) {
        fileReqSetSlotFlags(D_003BC7E8, D_003BC828, 2);
        func_00289E80(D_003BC7E8, D_003B2678, D_003DC780, 1);
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
            fileReqSetSlotFlags(D_003BC7E8, D_003BC828, 9);
        }
        return fileBeginSlotOpen();
    }
    if (status == -1) {
        return fileBeginSlotMetadataRefresh();
    }
    return fileBeginSlotOpen();
}

void *fileAdvanceSlotScan(void) {
    D_003BC828++;
    if (D_003BC828 == 10) {
        fileReqClearSlotMetadataDirty(D_003BC7E8);
        D_003BC828 = 0;
        D_003BC804 = 0;
        if (fileCountSelectableFiles() == 0) {
            if (D_003BC848 != 0) {
                D_003BC810 = 0;
                fileSetMenuFlowState(0);
                D_003BC854 = 7;
                D_003BC858 = 0;
                fileReqBegin(D_003BC7E8);
                return func_0028BAC0;
            }
            if (fileIsCardSpaceAboveMinimum() == 0) {
                fileSetMenuFlowState(0);
                D_003BC810 = 0;
                D_003BC854 = 5;
                D_003BC858 = 0;
                fileReqBegin(D_003BC7E8);
                return func_0028BAC0;
            }
        }
        fileSetMenuFlowState(0);
        return fileScanSlotStates();
    }
    return fileBeginReadSlotIcon();
}

void *mcResetSlotMetadata(void) {
    s32 index;
    if (fileReqIsSlotMetadataDirty(D_003BC7E8) != 0) {
        fileSetMenuFlowState(1);
        D_003BC828 = 0;
        index = 0;
        do {
            D_003DC7C0[index] = 0;
            fileReqClearSlotFlags(D_003BC7E8, index);
            index++;
        } while (index < 10);
        return fileBeginReadSlotIcon();
    } else {
        D_003BC804 = 0;
        fileSetMenuFlowState(0);
        return fileScanSlotStates();
    }
}

void *func_0028C3F0(void) {
    u8 buf[0x50];

    buf[0] = 0x2F;
    mcFormatSaveFilename(&buf[1], D_003BC828);
    func_00289DA8(D_003BC7E8, buf);
    return fileScanSlotIconSysAltBegin;
}

void *fileScanSlotIconSysAltBegin(void) {
    s32 t = mcPollSyncResult();

    if (t == 0) {
        return NULL;
    }
    if (t == 1) {
        fileReqSetSlotFlags(D_003BC7E8, D_003BC828, 2);
        func_00289E80(D_003BC7E8, D_003B2678, D_003DC780, 1);
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
            fileReqSetSlotFlags(D_003BC7E8, D_003BC828, 9);
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
    s32 slot = D_003BC828;
    s32 buttons = fileReqGetSlotFlags(D_003BC7E8, slot);

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
    D_003BC828 = slot;
    if (slot == 10) {
        fileSetMenuFlowState(0);
        D_003BC810 = 0;
        D_003BC854 = 0;
        return (void *)fileBeginSlotPromptSix();
    }
    return func_0028C3F0();
}

void *mcClearSlotMetadata(void) {
    s32 index = 0;
    u32 *saved;
    fileSetMenuFlowState(1);
    D_003BC810 = 0;
    D_003BC828 = 0;
    fileReqMarkSlotMetadataDirty(D_003BC7E8);
    saved = D_003DC7C0;
    do {
        *saved++ = 0;
        fileReqClearSlotFlags(D_003BC7E8, index);
        index++;
    } while (index < 10);
    return func_0028C3F0();
}

void *mcPrepareDirectory(void) {
    u8 name[0x50];
    u32 entry = D_003BC7E8;
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
    u32 entry = D_003BC7E8;
    s32 v = fileReqGetSelectedSlot(entry);

    buf[0] = 0x2F;
    mcFormatSaveFilename(&buf[1], v);
    func_00289DA8(entry, buf);
    return filePrepareMainBlobWrite;
}

void *mcHandleSearchResult(void) {
    s32 status = func_00289E38();
    if (status == 0) {
        return NULL;
    }
    if (status == 1) {
        fileReqSetSlotFlags(D_003BC7E8, D_003BC844, 2);
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
    s32 t = func_00289F30();

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
    fileReqSetSlotFlags(D_003BC7E8, D_003BC844, 1);
    fileReqSetSlotFlags(D_003BC7E8, D_003BC844, 8);
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

void *func_0028C8F8(void) {
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
                          func_0028C8F8, &D_003BC7F8);
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", fileBuildMainBlobAndWrite);

void *mcChooseLoadPath(void) {
    u32 entry = D_003BC7E8;
    s32 slot = fileReqGetSelectedSlot(entry);
    u32 flags = fileReqGetSlotFlags(entry, slot);
    if (!(flags & 8)) {
        return fileBuildMainBlobAndWrite(entry, D_003B2678);
    }
    func_00289F10();
    return fileBuildMainBlobAfterDelete;
}

void *fileBeginRequest(const char *name, u32 *first, u32 *second, void *callback, u32 *status) {
    D_003BD928 = first;
    D_003BD92C = second;
    D_003BD930 = callback;
    D_003BD934 = status;
    func_00289F80(D_003BC7E8, name, 0x203);
    return fileWriteWaitOpen;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", fileWriteWaitOpen);

void *mcHandleLoadResult(void) {
    s32 status = fileWriteWait();
    if (status == 0) {
        return NULL;
    }
    if (status == 1) {
        func_0028A008(D_003BD91C);
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
    s32 status = fileWaitCommandDone();
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
    if (D_003BC7F8 != 0) {
        return NULL;
    }
    fileWriteBegin(D_003BD91C, *D_003BD928, *D_003BD92C);
    return mcHandleLoadResult;
}

void *mcHandleDetectionResult(void) {
    s32 status = func_00289D68();
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

void *func_0028D978(void) {
    fileSetMenuFlowState(5);
    func_001005B8();
    func_00289D50(D_003BC7E8);
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
    u32 ctx = D_003BC7E8;
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
    func_00289F80(ctx, path, 1);
    return fileLoadMainBlobBegin;
}

void *fileLoadMainBlobBegin(void) {
    s32 status = func_00289FA8(&D_003BD91C);
    u32 size;

    if (status == 0) {
        return NULL;
    }
    size = fileMainBlobSize();
    D_003BD920 = func_002D03F8(size);
    D_003BD924 = sdfResourceRetainAddress(D_003BD920);
    if (status == 1) {
        func_0028A070(D_003BD91C, D_003BD924, size);
        return mcHandleSetupResult;
    }
    fileSetMenuFlowState(0);
    D_003BC854 = 3;
    return fileAbortSlotScanOnInput;
}

void *mcHandleSetupResult(void) {
    s32 status = func_0028A088();
    if (status == 0) {
        return NULL;
    }
    if (status == 1) {
        func_0028A008(D_003BD91C);
        return mcdHandleSaveSetupDone;
    }
    func_002D0918(D_003BD920);
    fileSetMenuFlowState(0);
    D_003BC854 = 3;
    return fileAbortSlotScanOnInput;
}

extern s8 D_003BC7ED;
extern s32 fileLoadStateChanged(void);
extern void fileCacheSlotFlagsFromState(void);
extern void fileRestoreSlotFlagsToState(void);

void *mcdHandleSaveSetupDone(void) {
    s32 status = fileWaitCommandDone();
    if (status == 0) {
        return NULL;
    }
    if (status == 1) {
        fileReloadSaveBuffer();
        func_002D0918(D_003BD920);
        D_003BC7ED = 1;
        fileDestroyMenuTask();
        if (fileLoadStateChanged() == 0) {
            fileCacheSlotFlagsFromState();
        } else {
            fileRestoreSlotFlagsToState();
        }
        fileSetMenuFlowState(13);
        return func_0028BE60(-1);
    }
    func_002D0918(D_003BD920);
    fileSetMenuFlowState(0);
    D_003BC854 = 3;
    return fileAbortSlotScanOnInput;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", fileSlotBrowserUpdate);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B2658);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B2668);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B2678);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B2688);

INCLUDE_ASM(const s32, "game/code_0028A0E0", fileDrawSlotListAndPreview);

void *fileRunMenuState(s32 arg) {
    void *next;
    u32 job;
    void *(*cur)(s32);
    D_003BC814++;
    fileDrawMenuFrame(arg);
    next = D_003BD8FC(arg);
    if (next == (void *)-1) {
        return next;
    }
    job = D_003BC7F8;
    cur = D_003BD8FC;
    if (next != NULL) {
        cur = next;
    }
    D_003BD8FC = cur;
    if (job != 0 && func_00288BA8(job, next) != 0) {
        D_003BC7F8 = 0;
        D_003BD910 = fileGetResourceHandle(job);
        D_003BD914 = func_00288B90(job);
        D_003BD918 = fileGetResourceSize(job);
        func_002887A0(job);
    }
    return NULL;
}

s32 fileDrawMenuFrame(s32 work) {
    evtSetDrawSurfaceIndex(0x52);
    func_00108CB8(0);
    evtSubmitGsRegister47(1, 0, 0x80, 3, 0, 0, 1, 1);
    if (D_003BC80C != 0) {
        func_00108FA0(0, 0, 0x200, 0x1C0, 0, 0, 0x200, 0x1C0, 0x80808080, 0x80808080, 0x80808080, 0x80808080,
                      D_003BC880);
        func_00108FA0(0, 0, 0x200, 0x1C0, 0, 0, 0x200, 0x1C0, 0x80808080, 0x80808080, 0x80808080, 0x80808080,
                      D_003BC884);
    }
    if (D_003BC810 != 0) {
        fileDrawSlotListAndPreview(work);
    }
    func_0028AB48();
    fileShowPromptDialog();
    fileShowStatusDialog();
    func_00108CB8(0);
    evtSubmitGsRegister47(1, 5, 0x80, 3, 0, 0, 1, 2);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", fileEnterMcPackScene);

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
            func_00110860(world, 1);
        }
        if (D_003BC7F8 != 0) {
            fileWaitReady(D_003BC7F8);
            D_003BD910 = fileGetResourceHandle(D_003BC7F8);
            func_002887A0(D_003BC7F8);
            D_003BC7F8 = 0;
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
    return D_003BC84C;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028F630);

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
        fileReqBegin(D_003BC7E8);
    }
    return (s32)func_0028F630;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", fileShowPromptDialog);

u32 func_00290478(void) {
    return D_003BC81C;
}

typedef struct FilePreviewWork {
    u8 pad0[0x15990];
    s16 previewX;
    s16 previewY;
} FilePreviewWork;

void fileSetPreviewLocation(s16 x, s16 y) {
    FilePreviewWork *work = (FilePreviewWork *)D_003BAA00;

    work->previewX = x;
    work->previewY = y;
}

void func_002904A8(u32 value) {
    D_0037D4D0[0] = value;
    D_0037D4A0.unkC = 0x80;
    D_0037D4A0.unk4 = 0;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002904C8);

void fileCursorStepUp(void) {
    D_0037D4A0.unk4++;
    D_0037D4A0.unk4 = D_0037D4A0.unk4 <= 0 ? 0 : D_0037D4A0.unk4 > 12 ? 12 : D_0037D4A0.unk4;
    if (D_0037D4A0.unk4 >= 7) {
        D_0037D4A0.unkC += 0x30;
        D_0037D4A0.unkC = D_0037D4A0.unkC <= 0 ? 0 : D_0037D4A0.unkC > 0x80 ? 0x80 : D_0037D4A0.unkC;
    }
}

void fileFadeStepDown(void) {
    D_0037D4D0[0] -= 0x10;
    D_0037D4D0[0] = D_0037D4D0[0] <= 0 ? 0 : D_0037D4D0[0] > 0x80 ? 0x80 : D_0037D4D0[0];
}

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B26C8);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", jtbl_003B26E0);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B2718);

void fileDrawSlotIcon(s32 index, s32 x, s32 y, s32 alpha) {
    s32 uv[21][2] = {
        {2, 2},   {2, 2},   {2, 20},  {2, 38},  {2, 56},  {2, 74},  {26, 2},
        {26, 20}, {26, 38}, {26, 56}, {26, 74}, {26, 74}, {26, 74}, {26, 74},
        {26, 74}, {26, 74}, {26, 74}, {26, 74}, {26, 74}, {26, 74}, {26, 74},
    };

    evtSetDrawSurfaceIndex(0x53);
    func_00108CB8(0);
    evtSubmitGsRegister47(1, 0, 0x80, 3, 0, 0, 1, 1);
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
    D_0037D4E0.mode[1] = 0;
    D_0037D4AC[0] = 0;
    D_0037D4E0.mode[0] = 1;
    D_0037D4E0.mode[2] = 2;
    D_0037D4E0.alpha[0] = 0x80;
    D_0037D4E0.alpha[1] = 0x80;
    D_0037D4E0.alpha[2] = 0x80;
}

void fileCursorPulseUpdate(void) {
    s32 i;

    for (i = 0; i < 3; i++) {
        s32 mode = D_0037D4E0.mode[i];

        if (mode < 3) {
            if (mode >= 0) {
                switch (mode) {
                case 0:
                    D_0037D4E0.alpha[i] -= 12;
                    D_0037D4E0.alpha[i] = D_0037D4E0.alpha[i] <= 0x40 ? 0x40 : D_0037D4E0.alpha[i] > 0x80 ? 0x80 : D_0037D4E0.alpha[i];
                    break;
                case 1:
                    D_0037D4E0.alpha[i] -= 28;
                    D_0037D4E0.alpha[i] = D_0037D4E0.alpha[i] <= 0 ? 0 : D_0037D4E0.alpha[i] > 0x80 ? 0x80 : D_0037D4E0.alpha[i];
                    break;
                }
                D_0037D4E0.counter[i] += 1;
                D_0037D4E0.counter[i] = D_0037D4E0.counter[i] <= 0 ? 0 : D_0037D4E0.counter[i] > 6 ? 6 : D_0037D4E0.counter[i];
                if (D_0037D4E0.counter[i] >= 6) {
                    D_0037D4E0.mode[i]++;
                    D_0037D4E0.alpha[i] = 0x80;
                    if (D_0037D4E0.mode[i] >= 3) {
                        D_0037D4E0.mode[i] = 0;
                    }
                    D_0037D4E0.counter[i] = 0;
                }
                fileCursorOffsetLookup(i, 0, &D_0037D4E0.pos[i][0], &D_0037D4E0.pos[i][1]);
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00290A88);

void fileLoadSetMode(s8 mode) {
    D_0037D4A0.unk14 = 0;
    D_0037D4A0.unk10 = mode;
    if (mode == 1) {
        D_0037D4A0.unk1C = 0;
        D_0037D4A0.unk18 = 0x74;
    } else {
        D_0037D4A0.unk18 = 0;
        D_0037D4A0.unk1C = 0x74;
    }
}

void fileResetLoadContextSlide(void) {
    D_0037D4A0.unk14 = 0;
    D_0037D4A0.unk10 = 0;
    D_0037D4A0.unk4 = 0;
}

void fileLoadCtxSlideUpdate(void) {
    switch (D_0037D4A0.unk10) {
    case 1:
        D_0037D4A0.unk14 -= 0x14;
        D_0037D4A0.unk14 = D_0037D4A0.unk14 <= -0x6E ? -0x6E : D_0037D4A0.unk14 > 0 ? 0 : D_0037D4A0.unk14;
        if (D_0037D4A0.unk14 <= -0x6E) {
            D_0037D4A0.unk14 = 0;
            D_0037D4A0.unk10 = 0;
        }
        D_0037D4A0.unk18 -= 0x20;
        D_0037D4A0.unk18 = D_0037D4A0.unk18 <= 0 ? 0 : D_0037D4A0.unk18 > 0x80 ? 0x80 : D_0037D4A0.unk18;
        D_0037D4A0.unk1C += 0x10;
        D_0037D4A0.unk1C = D_0037D4A0.unk1C <= 0 ? 0 : D_0037D4A0.unk1C > 0x80 ? 0x80 : D_0037D4A0.unk1C;
        break;
    case 2:
        D_0037D4A0.unk14 += 0x14;
        D_0037D4A0.unk14 = D_0037D4A0.unk14 <= 0 ? 0 : D_0037D4A0.unk14 > 0x6E ? 0x6E : D_0037D4A0.unk14;
        if (D_0037D4A0.unk14 >= 0x6E) {
            D_0037D4A0.unk14 = 0;
            D_0037D4A0.unk10 = 0;
        }
        D_0037D4A0.unk18 += 0x10;
        D_0037D4A0.unk18 = D_0037D4A0.unk18 <= 0 ? 0 : D_0037D4A0.unk18 > 0x80 ? 0x80 : D_0037D4A0.unk18;
        D_0037D4A0.unk1C -= 0x20;
        D_0037D4A0.unk1C = D_0037D4A0.unk1C <= 0 ? 0 : D_0037D4A0.unk1C > 0x80 ? 0x80 : D_0037D4A0.unk1C;
        break;
    }
}

s32 fileLoadStateChanged(void) {
    return D_003BC8D8.current != D_003BC8D8.previous;
}

void fileCacheSlotFlagsFromState(void) {
    D_003BC8D8.current = D_003BC8D8.previous =
        *(u32 *)(D_003BAA00 + 0xA54);
}

void fileRestoreSlotFlagsToState(void) {
    *(u32 *)(D_003BAA00 + 0xa54) = D_003BC8DC;
}

s32 fileToggleSlotFlagsBit(u32 kind, s32 *flags) {
    switch (kind) {
    case 0:
        *flags ^= 2;
        if (fileTestSlotFlagsBit(kind, flags) != 0) {
            func_00104068(0, 1, 0xF);
            func_00104068(1, 0x80, 0xF);
        } else {
            func_00104068(0, 0, 0xF);
            func_00104068(1, 0, 0xF);
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

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B28A0);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B28C0);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B28D0);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B28E8);

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
    fileTestSlotFlagsBit(kind, D_003BAA00 + 0xa54);
}

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B2920);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B2940);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B2960);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B2980);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B29A0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00290FE0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002911B8);

extern s32 func_00290FE0();
extern s32 func_00291418(void);
extern s32 func_002911B8(void);
extern s32 fileStartQueuedLoad(void);
extern u32 fileGetConfigTaskFailure(void);
extern void *kwlnTaskCreate(const char *name, s32 id, s32 optionA, s32 optionB, void *update, void *destroy, s32 data);

void mnuCreateConfigTasks(void) {
    if (D_003BD938 == 0) {
        D_003BD938 = func_00290FE0();
        kwlnTaskCreate(D_003BC8E8, 0x3F2, 1, 1, func_00291418, NULL, D_003BD938);
        kwlnTaskCreate(D_003B29D8, 0x2B07, 1, 1, fileStartQueuedLoad, NULL, D_003BD938);
        kwlnTaskCreate(D_003B29E8, 0x520B, 1, 1, fileGetConfigTaskFailure, func_002911B8, D_003BD938);
        D_003BC8D5 = 1;
    }
}

void mnuConfigTasksDestroy(void) {
    kwlnTaskDestroyWithHierarchyByName(D_003BC8E8, 1);
    kwlnTaskDestroyWithHierarchyByName(D_003B29D8, 1);
    kwlnTaskDestroyWithHierarchyByName(D_003B29E8, 1);
}

s32 fileConsumeConfigTaskReady(void) {
    s32 state = D_003BC8D5;
    if (state == 1) {
        return 1;
    }
    if (state < 2) {
        return 0;
    }
    if (state == 2) {
        D_003BC8D5 = 0;
    }
    return 0;
}

/* Save/config task context: its four resource slots start at +0x10. */
typedef struct FileConfigTask {
    u8 pad0[0xC];
    u32 frame;      /* 0x0C: passed to menu window drawing */
    u32 slots[4];   /* 0x10 */
    u8 pad20[4];
    s32 result;     /* 0x24: negative when the queued load failed */
    u8 pad28[0xC];
    u32 pending;    /* 0x34: zero when no load can start */
} FileConfigTask;

u32 fileGetConfigTaskSlot(s32 slot) {
    if (slot < 4) {
        return ((FileConfigTask *)D_003BD938)->slots[slot];
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B29D8);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B29E8);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00291418);

s32 fileStartQueuedLoad(void) {
    if (((FileConfigTask *)D_003BD938)->pending == 0) {
        return 0;
    }
    if (((FileConfigTask *)D_003BD938)->result < 0) {
        return -1;
    }
    func_00292720((void *)D_003BD938);
    mnuCallInitWide(0x400, 0x400, 0, ((FileConfigTask *)D_003BD938)->frame, 0x53);
    return 0;
}

u32 fileGetConfigTaskFailure(void) {
    u32 result;

    result = 0xffffffff;
    if ((((FileConfigTask *)D_003BD938)->result & 0x80000000) == 0) {
        result = 0;
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002918F8);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00292720);

void fileManagerResetSubsystems(void) {
    func_003003F0(D_003BC900);
    effLoadFlashTextures();
    func_003003F0(D_003BC908);
    effLoadWindTexture();
    func_003003F0(D_003BC910);
    effLoadScalyTexture();
    func_003003F0(D_003BC918);
    func_00292C40();
    func_003003F0(D_003BC920);
}

void fileSetRenderFlag(u32 bits) {
    D_003BC8F8 |= bits;
}

void fileClearRenderFlag(u32 bits) {
    D_003BC8F8 &= ~bits;
}

void func_00292C40(void) {
    D_003BC8F8 = 0;
}

u32 func_00292C48(s32 index) {
    return D_0037E130[index];
}

extern u8 D_003296F0[];
extern u8 D_00324610[];
extern u8 D_00324660[];
extern void func_002DDD60(void *);

void mnuProjectViewPoint(void) {
    u8 *matrix;
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf28, 0(%0)\n"
        "lqc2 vf29, 0x10(%0)\n"
        "lqc2 vf30, 0x20(%0)\n"
        "lqc2 vf31, 0x30(%0)\n"
        ".set reorder"
        : : "r"(D_003296F0) : "memory");
    matrix = D_00324610;
    func_002DDD60(matrix);
    __asm__ volatile (
        ".set noreorder\n"
        "vmulax.xyzw ACC, vf28, vf10x\n"
        "vmadday.xyzw ACC, vf29, vf10y\n"
        "vmaddaz.xyzw ACC, vf30, vf10z\n"
        "vmaddw.xyzw vf10, vf31, vf0w\n"
        "vdiv Q, vf0w, vf10w\n"
        "vmove.w vf10, vf0\n"
        "vwaitq\n"
        "vmulq.xyzw vf10, vf10, Q\n"
        ".set reorder"
        : : : "memory");
    matrix += 0x40;
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf11, 0(%0)\n"
        "vmul.xyzw vf10, vf10, vf11\n"
        ".set reorder"
        : : "r"(matrix) : "memory");
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf11, 0(%0)\n"
        "vadd.xyzw vf10, vf10, vf11\n"
        ".set reorder"
        : : "r"(D_00324660) : "memory");
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00292CE0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00292E50);

#define VEC3_SPLAT(v, x) ((v)[0] = (x), (v)[1] = (x), (v)[2] = (x))

/* vu0 routine: point at t between p[1] and p[2] of a Catmull-Rom (Hermite, 0.5 tangents) spline, left in vf10 */
void func_00292F48(f32 (*p)[4], f32 t)
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
#undef VEC3_SPLAT

/* vu0 routine: camera basis rows vf28-vf31 from eye D_00324690, target D_00324680 and up D_003246A0 */
void func_002930C0(void)
{
    VU0_LOAD_VF(vf10, D_00324690);
    VU0_MOVE_VF(vf31, vf10);
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, D_00324680);
    VU0_SUB(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf30, vf10);
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, D_003246A0);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf28, vf10);
    VU0_MOVE_VF(vf11, vf10);
    VU0_MOVE_VF(vf10, vf30);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_MOVE_VF(vf29, vf10);
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00293158);

FileJob *fileCreateJob(u16 type) {
    u16 kind = type;
    FileJob *job = func_002CFEB8(0x2C);
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
    job->data = D_0037E148[job->type].create(request);
    return job;
}

void fileJobDestroy(FileJob *job) {
    void *data = job->data;
    if (data != NULL) {
        u16 index = job->type;
        D_0037E148[index].destroy(data);
    }
    fileJobFreePrimaryBuffer(job);
    fileJobFreeSecondaryBuffer(job);
    func_002CFF98(job);
}

void fileJobFreePrimaryBuffer(FileJob *job) {
    void *buffer = job->slots[0].allocation;
    if (buffer != NULL) {
        func_002D0918(buffer);
        job->slots[0].offset = 0;
        job->slots[0].size = 0;
        job->slots[0].allocation = NULL;
    }
}

void fileJobFreeSecondaryBuffer(FileJob *job) {
    void *buffer = job->slots[1].allocation;
    if (buffer != NULL) {
        func_002D0918(buffer);
        job->slots[1].offset = 0;
        job->slots[1].size = 0;
        job->slots[1].allocation = NULL;
    }
}

FileJob *fileJobCreateChild(FileJob *request) {
    FileJob *job = fileCreateJob(request->type);
    job->option = request->option;
    job->slots[0].selector = request->slots[0].selector;
    job->data = D_0037E148[job->type].createChild(request->data, job->type);
    return job;
}

void fileJobNotifyPair(FileJob *left, FileJob *right) {
    void (*cb)(void *, void *) = D_0037E14C[right->type].cbC;
    if (cb != NULL) {
        cb(left->data, right->data);
    }
}

void fileJobNotifyComplete(FileJob *job) {
    u16 idx = ((FileJob *)job)->type;
    void (*cb)(void *) = D_0037E14C[idx].cb10;
    if (cb != NULL) {
        cb(((FileJob *)job)->data);
    }
}

void func_002936A8(FileJob *job) {
    u16 idx = ((FileJob *)job)->type;
    void *data = ((FileJob *)job)->data;

    D_0037E14C[idx].cb(data);
}

void func_002936E0(FileJob *job) {
    u16 idx = ((FileJob *)job)->type;
    void (*cb)(void *) = D_0037E14C[idx].cb14;
    if (cb != NULL) {
        cb(((FileJob *)job)->data);
    }
}

void func_00293720(FileJob *job, void *extra) {
    u16 idx = ((FileJob *)job)->type;
    void (*cb)(void *, void *) = D_0037E14C[idx].cb18;
    if (cb != NULL) {
        cb(((FileJob *)job)->data, extra);
    }
}

void func_00293760(FileJob *job) {
    u16 idx = ((FileJob *)job)->type;
    void (*cb)(void *) = D_0037E14C[idx].cb1C;
    if (cb != NULL) {
        cb(((FileJob *)job)->data);
    }
}

void func_002937A0(FileJob *job) {
    u16 idx = ((FileJob *)job)->type;
    void (*cb)(void *) = D_0037E14C[idx].cb20;
    if (cb != NULL) {
        cb(((FileJob *)job)->data);
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
        job->slots[0].allocation = (void *)func_002D03F8(size);
        job->slots[0].offset = sdfResourceRetainAddress(job->slots[0].allocation);
        job->slots[0].size = size;
        job->option = option;
        memcpy((void *)job->slots[0].offset, src, size);
    }
}

void func_00293880(u64 job, u64 state, u16 option) {
    s64 command;
    u64 size;
    u64 handle;
    u64 address;

    command = sdfDevCreateCommandState(state);
    if (command != 0) {
        size = func_002E5C88(command);
        handle = func_002D03F8(size);
        address = sdfResourceRetainAddress(handle);
        func_002E5C68(command, address, size);
        func_002E5C38(command);
        fileJobSetPrimaryData(job, address, size, option);
        func_002D0918(handle);
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
        job->slots[1].allocation = (void *)func_002D03F8(size);
        job->slots[1].offset = sdfResourceRetainAddress(job->slots[1].allocation);
        job->slots[1].size = size;
        job->slots[0].selector = selector;
        memcpy((void *)job->slots[1].offset, src, size);
    }
}

void func_00293A00(u64 job, u64 state, u16 selector) {
    s64 command;
    u64 size;
    u64 handle;
    u64 address;

    command = sdfDevCreateCommandState(state);
    if (command != 0) {
        size = func_002E5C88(command);
        handle = func_002D03F8(size);
        address = sdfResourceRetainAddress(handle);
        func_002E5C68(command, address, size);
        func_002E5C38(command);
        fileJobSetSecondaryData(job, address, size, selector);
        func_002D0918(handle);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00293AE0);

extern u8 D_003BD476;
extern char D_003BC928[];
extern char D_003BC930[];
extern char D_003BC938[];
extern char *func_002E5D70(void);
extern s32 func_0030E8F0(const char *path, s32 flags, ...);
extern void func_00293AE0(s32 fd, FileJob *job);
extern void func_0030EB78(s32 fd);
extern void func_00310A68(const char *path, s32 mode);

void fileWriteToPfs(FileJob *job, s32 slot) {
    char path[0xD0];
    s32 fd;

    if (D_003BD476 != 0) {
        func_003014F0(path, D_003BC928, slot);
        fd = func_0030E8F0(path, 0x602, 0x1B6);
    } else {
        func_003014F0(path, D_003BC930, func_002E5D70(), slot);
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
    u64 entry;
{
    s64 command;
    u64 size;
    s32 handle;
    s32 address;
    void *job;

    command = sdfDevCreateCommandState(entry);
    if (command != 0) {
        size = func_002E5C88(command);
        handle = func_002D03F8(size);
        address = sdfResourceRetainAddress(handle);
        func_002E5C68(command, address, size);
        func_002E5C38(command);
        job = fileDuplicateJob((void *)address);
        func_002D0918(handle);
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

/* View block initialised by func_00293EA0: (0,0,0,1) vectors, grey colour. */
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

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00293EA0);

void func_00293F18(void *queue) {
    memset(queue, 0, 0x90);
    *(u32 *)((u8 *)queue + 0x84) = 1;
    *(u8 *)((u8 *)queue + 0x88) = 8;
    *(u8 *)((u8 *)queue + 0x89) = 0;
    *(u8 *)((u8 *)queue + 0x8A) = 0;
    func_00293EA0(queue);
}

void fileQueueAppend(FileQueue *queue, FileJob *job) {
    job->next = NULL;
    if (queue->head != NULL) {
        queue->head->next = job;
        job->prev = queue->head;
    } else {
        queue->tail = job;
        job->prev = NULL;
    }
    queue->head = job;
    queue->count++;
}

void fileQueueInsertAfter(FileQueue *queue, FileJob *after, FileJob *job) {
    if (after->next != NULL) {
        after->next->prev = job;
        job->next = after->next;
    } else {
        job->next = NULL;
        queue->head = job;
    }
    after->next = job;
    job->prev = after;
    queue->count++;
}

void fileQueueRemove(FileQueue *queue, FileJob *job) {
    if (job->prev != NULL) {
        job->prev->next = job->next;
    } else {
        queue->tail = job->next;
    }
    if (job->next != NULL) {
        job->next->prev = job->prev;
    } else {
        queue->head = job->prev;
    }
    queue->count--;
}

FileQueue *fileQueueCreate(void) {
    FileQueue *queue = func_002CFEB8(0x90);
    memset(queue, 0, 0x90);
    queue->count = 0;
    queue->unk84 = 0;
    func_00293EA0(queue);
    return queue;
}

FileJob *fileJobCreate(void) {
    FileJob *job = func_002CFEB8(0xC0);
    memset(job, 0, 0xC0);
    func_00293F18(job);
    return job;
}

void func_002940B8(FileJob *job) {
    func_002CFF98(job);
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002940D0);

void func_00294318(u32 unused, u32 handle) {
    func_002940D0(handle);
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00294330);

void func_002944D8(FileQueue *queue) {
    FileJob *job = queue->tail;
    while (job != NULL) {
        FileJob *next = job->next;
        if ((job->flags & 1) == 0) {
            fileJobDestroy(*(FileJob **)((u8 *)job + 0x90));
        }
        func_002940B8(job);
        job = next;
    }
    func_002CFF98(queue);
}

extern void func_00294670(FileQueue *queue, void *vec);
extern void func_00294850(FileQueue *queue, f32 scale);
extern void func_00294938(FileQueue *queue, u32 color);
extern void fileJobCopyHeader(FileJob *dst, FileJob *src);

FileQueue *fileQueueClone(FileQueue *source) {
    FileQueue *queue = fileQueueCreate();
    FileJob *src;
    s128 vec;

    PCP_COPY_VECTOR(queue, source);
    PCP_COPY_VECTOR((u8 *)queue + 0x10, (u8 *)source + 0x10);
    *(f32 *)((u8 *)queue + 0x74) = *(f32 *)((u8 *)source + 0x74);
    *(u32 *)((u8 *)queue + 0x68) = *(u32 *)((u8 *)source + 0x68);
    for (src = source->tail; src != NULL; src = src->next) {
        FileJob *job = fileJobCreate();
        job->id = (u32)fileJobCreateChild((FileJob *)src->id);
        fileJobCopyHeader(job, src);
        fileQueueAppend(queue, job);
    }
    __asm__ volatile (
        ".set noreorder\n"
        "sqc2 vf0, 0(%0)\n"
        ".set reorder"
        : : "r"(&vec) : "memory");
    func_00294670(queue, &vec);
    func_00294798(queue, &vec);
    func_00294850(queue, 1.0f);
    func_00294938(queue, 0x80808080);
    return queue;
}

void func_00294630(FileQueue *queue) {
    FileJob *job;

    for (job = queue->tail; job != NULL; job = job->next) {
        fileJobNotifyComplete((void *)job->id);
    }
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00294670);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00294798);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00294850);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00294938);

void func_00294A18(void *dst, void *src) {
    s128 vec;
    func_00293158(src);
    __asm__ volatile (
        ".set noreorder\n"
        "sqc2 vf10, 0(%0)\n"
        ".set reorder"
        : : "r"(&vec) : "memory");
    func_00294798(dst, &vec);
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
    func_00293F18(job);
    job->flags |= 1;
    job->id = src->id;
    fileQueueInsertAfter(queue, src, job);
    return job;
}

extern FileJob *fileQueueFindBySector(FileQueue *queue, u32 sector);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00294C30);

extern void func_00294C30(FileQueue *queue, FileJob *job, FileJob *first);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00294DA0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00294E50);

void fileJobCopyHeader(FileJob *dst, FileJob *src) {
    memcpy(dst, src, 0x90);
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00295018);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002954F0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002959E8);

FileJob *fileQueueFindById(FileQueue *queue, u32 id) {
    FileJob *job = queue->tail;
    while (job != NULL) {
        if (job->id == id) {
            return job;
        }
        job = job->next;
    }
    return NULL;
}

FileJob *fileQueueFindFlaggedById(FileQueue *queue, u32 id) {
    FileJob *job = queue->tail;
    while (job != NULL) {
        if ((job->flags & 1) != 0 && job->id == id) {
            return job;
        }
        job = job->next;
    }
    return NULL;
}

FileJob *fileQueueFindBySector(FileQueue *queue, u32 sector) {
    FileJob *job = queue->tail;
    while (job != NULL) {
        if ((job->flags & 3) == 2 && job->sector == sector) {
            return job;
        }
        job = job->next;
    }
    return NULL;
}

FileJob *fileQueueGetAt(FileQueue *queue, s32 index) {
    FileJob *job = queue->tail;
    while (job != NULL) {
        if (index-- == 0) {
            return job;
        }
        job = job->next;
    }
    return NULL;
}

s32 fileFindQueuedJobIndex(FileQueue *queue, FileJob *target) {
    FileJob *job = queue->tail;
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
    for (job = queue->tail; job != NULL; job = job->next) {
        count++;
    }
    return count;
}

#define VEC3_SPLAT(v, x) ((v)[0] = (x), (v)[1] = (x), (v)[2] = (x))

typedef struct CamFollow {
    u8 pad00[0x40];
    f32 pos[4];
    u8 pad50[0x18];
    u32 flags;
} CamFollow;

/* vu0 routine: dst = |pos| * unit(D_00324690 - D_00324680); flag 0x10 flattens y, then puts pos.y back */
void func_00295D08(CamFollow *obj, void *dst)
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
    VU0_LOAD_VF(vf10, D_00324690);
    VU0_LOAD_VF(vf11, D_00324680);
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
#undef VEC3_SPLAT

typedef struct CamAim {
    u8 pad00[0x50];
    f32 quat[4];
    u8 pad60[8];
    u32 flags;
} CamAim;

/* dst = rotation facing unit(D_00324690 - D_00324680) (pitch and yaw); flag 0x40 composes it onto obj->quat, no flag 0x60 copies obj->quat */
void func_00295E00(CamAim *obj, void *dst)
{
    f32 v[4];
    f32 pitch;
    u32 flags = obj->flags;

    if ((flags & 0x60) == 0) {
        PCP_COPY_VECTOR(dst, obj->quat);
        return;
    }
    VU0_LOAD_VF(vf10, D_00324690);
    VU0_LOAD_VF(vf11, D_00324680);
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
    LoadObj *obj = func_002CFF68(0x4C);
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

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00295F58);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B2A18);

LoadObj *effLoadObjectCreateFromJob(FileJob *job) {
    void *primary = fileResolvePrimaryBuffer(job);
    LoadObj *obj = func_00295F58(primary);
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
        func_002D0918(obj->unk3C);
    }
    if (obj->referenceHolder != NULL) {
        effReleaseReferenceHolder((s32)obj->referenceHolder);
    }
    if (obj->recordWork != NULL) {
        func_0029A730((s32)obj->recordWork);
    }
    func_002CFF98(obj);
}

LoadObj *fileLoadObjectCreateChild(LoadObj *owner) {
    LoadObj *source = *(LoadObj **)((u8 *)owner->recordWork + 0x24);
    LoadObj *result = func_00295F58(source);
    fileLoadObjectSetResource(result, *(u16 *)owner->recordWork, source);
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
        dst->deviceHandle = func_00151E60(src->deviceHandle);
        func_001523B0(dst->deviceHandle);
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
            func_002D0918(dst->unk3C);
            dst->unk38 = 0;
            dst->unk3C = 0;
        }
        size = count * 4;
        if (size == 0) {
            return;
        }
        dst->unk3C = func_002D03F8(size);
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
        dst->referenceHolder = func_0029C408(src->referenceHolder);
        break;
    }
    dst->selector = src->selector;
}

void fileLoadObjectSetResource(LoadObj *obj, u32 type, void *data) {
    if (obj->recordWork != NULL) {
        func_0029A730((s32)obj->recordWork);
    }
    obj->recordWork = func_0029A5E0(type, (u32)obj->owner, data);
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
    func_001523B0(obj->deviceHandle);
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
        func_002D0918(obj->unk3C);
        obj->unk38 = 0;
        obj->unk3C = 0;
    }
    size = count * 4;
    if (size != 0) {
        obj->unk3C = func_002D03F8(size);
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

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002966D8);

void func_00296E98(s32 object) {
    fileAcquireLoadObjectRecord(object);
    func_002966D8(object);
}

void fileSendLoadObjectRecordVector(LoadObj *obj, u128 *vector) {
    mnuRecordSetVector(obj->recordWork, vector);
}

void fileCopyLoadObjectRecordVector(LoadObj *obj, u128 *vector) {
    func_0029A7F8(obj->recordWork, vector);
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

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00296F58);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00297270);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002973E8);

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
void func_00297558(FileSlotGroup *group, u8 *slot) {
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
void func_002975C8(FileSlotGroup *group, u8 *slot) {
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

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00297658);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00297CB0);

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

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002985D0);

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

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00298D28);

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

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002995F8);

void func_00299DD8(ScaleOwner *owner, f32 scale) {
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

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00299E58);

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

void *func_0029A5E0(u16 type, u32 count, void *data) {
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
    handle = func_002D03F8(size);
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

void func_0029A730(s32 record) {
    func_002D0918(((FileRecordSlots *)record)->handle);
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

void func_0029A7E0(void *record, u128 *out) {
    PCP_COPY_VECTOR(out, *(u128 **)((u8 *)record + 0x20) + 1);
}

void func_0029A7F8(void *record, const u128 *value) {
    PCP_COPY_VECTOR(*(u128 **)((u8 *)record + 0x20) + 1, value);
}

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC7E8);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC7EC);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC7ED);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC7F0);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC7F8);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC7FC);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC800);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC804);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC808);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC80C);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC810);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC814);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC818);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC81C);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC820);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC824);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC828);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC82C);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC830);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC834);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC838);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC83C);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC840);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC844);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC848);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC84C);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC850);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC854);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC858);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC85C);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC860);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC864);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC868);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC86C);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC870);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC874);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC878);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC87C);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC880);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC884);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC888);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC88C);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC890);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC894);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC898);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC8A0);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC8A8);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC8B0);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC8B8);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC8C0);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC8D0);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC8D5);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC8D8);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC8DC);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC8E0);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC8E8);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC8F0);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC8F8);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC900);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC908);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC910);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC918);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC920);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC928);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC930);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC938);

INCLUDE_SDATA(const s32, "game/code_0028A0E0", D_003BC940);

