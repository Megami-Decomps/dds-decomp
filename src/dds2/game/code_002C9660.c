#include "common.h"
#include "pcp_vu0.h"
typedef struct EffectSurfaceNode {
    u32 percent;
    u32 color;
    f32 opacity;
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
    u16 count;
} EffectSurfaceNode;

extern u32 effRetainResource(u32);
extern void billSetBillboardMode(u32, s16);
extern u32 billCreateIndexed(u32, u32);
extern void func_00159FA0(u32);
extern u32 func_00159A50(u32);
extern u32 func_002DE120(u32);
extern u32 D_00439044;
extern u32 func_002DBED8(u16, u32, void *);
#include "kwln.h"

extern void *fileDuplicateJob(void *);

extern s32 func_003292A8();

extern s32 sdfResourceRetainAddress();

extern s64 sdfDevCreateCommandState(u64);

extern u64 func_0033EB30(s64);

extern s32 D_00437D84;

extern u32 D_00437CE8;

extern u32 D_00437CEC;

extern s32 D_00437CF0;

extern u32 D_00437CFC;

extern u32 D_00437D08;

extern u32 D_00437D14;

extern u32 D_00437D18;

extern u32 D_00437D1C;

extern u32 D_00437D20;

extern u32 D_00437D40;

extern u32 D_00437D44;

extern u32 D_00437D3C;

extern u64 func_0019F5E8(s32, s32, u64, u64, u64, u64);
extern u32 func_0019F460(s32, s32, u64, u64, u64, u64);
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
extern s32 D_00437D54;
extern s32 D_00437D58;
extern s32 sdfTexReleaseReferenceViaHandler(s32);
extern s32 dds3GetWorldObject(void);
extern void func_00110A88(s32, s32);
extern void fileWaitReady(u32);
extern void sdfReleaseMemorySlot(void *);
extern void sdfFreeMemoryFromEitherHeap(s32);

extern s32 D_00437D38;

extern s32 D_00437D48;

extern u32 D_00437CD0;

extern u32 D_00437CF4;

extern u32 D_00437CF8;

extern u32 D_00437D34;

extern u32 D_00437D04;

extern s32 D_00435DD0;

extern u32 D_00437DEC;

extern s32 D_00439058;

extern u32 D_00437E08;

extern u32 func_002DDF48(u32);

extern s8 D_00437CD4;
extern s32 D_00437D7C;
extern s32 D_00437D88;
extern void func_0035C860(void *dst, const char *fmt, ...);

extern s32 fileSlotSelectPoll(void);

extern void fileReqBegin(s32 arg0);

extern void *fileBeginSlotMetadataRefresh(void);

extern void *func_002CC760(void);

extern void *mcResetSlotMetadata(void);

extern void func_002CB2C0(void);

extern s32 fileIsCardSpaceAboveMinimum(void);

extern s32 fileReqGetSize(s32 arg0);

extern u32 D_00439020;

extern void mcdFinishFileDetection(void);

extern void *fileBeginDetectionRequest(u32 arg0);

extern void func_002CB740(void);

extern s32 D_00437D10;

extern void mcdFormatSaveSlotName(void *arg0, s32 arg1);

extern void func_002C9328(u32 arg0, void *arg1);

extern void *fileScanSlotIconSysBegin(void);

extern void *fileScanSlotIconSysAltBegin(void);

extern void *filePrepareMainBlobWrite(void);

extern void fileOnAllWritten(void);

extern void func_001004A0(void);
extern s8 D_0037F510[];
extern void sndSetSequenceVolumePan(s32 sequence, s32 volume, s32 pan);
extern void func_00342580(s32 command);
extern void *fileBeginWait(s32 result);

extern void func_002C92D0(u32 arg0);

extern void *mcHandleDetectionResult(void);

extern char D_0042B720[];

extern KwlnTask *func_00101740(const char *name);

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

extern LoadCtx374A0 D_003E7FD8;

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

extern void func_002D31C0(void *src);

extern void func_002D4818(void *dst, void *src);

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

extern EffDispatchEntry D_003E917C[];

extern EffDispatchEntry D_003E9180[];

extern EffDispatchEntry D_003E9184[];

extern EffDispatchEntry D_003E9188[];

extern EffDispatchEntry D_003E918C[];

extern s8 D_00437DE5;

extern void mcdFinishFileDetectionWithAudio(void);

extern void func_002CAED0(void);

extern s32 D_00437D50;

extern s8 D_004580C3[];

extern void *(*D_00439018)(s32);

extern void evtSetDrawSurfaceIndex(s32);

extern void func_00108BD8(s32);

extern void evtSubmitGsRegister47(s32, s32, s32, s32, s32, s32, s32, s32);

extern s32 D_00437D70;

extern void func_00108EC0(s32, s32, s32, s32, s32, s32, s32, s32, u32, u32, u32, u32, s32);

extern void fileCursorPulseUpdate(void);

extern void func_002CFF38(s32, s32, s32);

extern s32 D_00437D78;

extern void *fileReadSlotPreviewBegin(void);
extern s32 func_002C9528(s32 *);
extern void func_002C95F0(s32, u32, s32);
extern void *mcHandleSetupResult(void);
extern s32 D_0043903C;
extern u32 D_00439040;
extern void *fileReadSlotPreviewWait(void);

extern void *func_002CBA90(void);

extern void fileReqSetSelectedSlot(u32 ctx, s32 arg);

extern void func_002C9500(u32, const char *, s32);

extern void *fileBeginSlotOpen(void);

extern u32 fileReqGetSlotFlags(s32 arg0, s32 arg1);

extern void *fileSlotSelectPollClear(void);

extern void *fileRestartSlotSelection(void);

extern void *fileSlotStatusPoll(void);

extern void *fileResetSelection(void);

extern u32 D_00458080[];

extern void *D_0043901C;

extern void *fileUpdateWait(void);

extern void fileReqSetSlotFlags(s32 arg0, s32 arg1, s32 arg2);

extern void *mcHandleSlotWriteResult(void);

extern s32 mcPollNonnegativeResult(void *arg0);

extern s32 fileBeginSlotPromptFive(void);

extern void *fileScanSlotStatesAdvance(void);

extern void *mcHandleDirectoryWriteResult(void);

extern void *mcPrepareDirectory(void);

extern void fileReqMarkSlotMetadataDirty(s32);

extern void *mcHandleSearchResult(void);

extern void *fileScanSlotStates(void);

extern void *fileLoadMainBlobBegin(void);

extern s32 func_002C8128(u32, void *);

extern u32 fileGetResourceHandle(u32);

extern u32 func_002C8110(u32);

extern u32 fileGetResourceSize(u32);

extern void func_002C7D00(u32);

extern s32 fileDrawMenuFrame(s32);

extern u32 D_00439034;

extern u32 D_00439038;

extern char D_00437DF8[];

extern char D_0042BAF0[]; /* "config_draw" */

extern char D_0042BB00[]; /* "config_update" */

extern void kwlnTaskDestroyWithHierarchyByName(const char *name, s32 hierarchy);

typedef struct LoadMirror {
    u32 current;
    u32 previous;
} LoadMirror;

extern LoadMirror D_00437DE8;

extern s32 fileLoadStateChanged(void);

extern void fileCacheSlotFlagsFromState(void);

extern void *func_00328D68(s32 size);

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

typedef struct FileTypeCallbacks {
    void *(*create)(void *);
    void (*unk4)(void *);
    void (*destroy)(void *);
    void *(*createChild)(void *, u16);
    u8 unk10[0x18];
} FileTypeCallbacks;

extern FileTypeCallbacks D_003E9168[];

/* Only fields needed by the save copy are exposed; the remaining state is opaque. */
typedef struct FileSaveState {
    u8 pad00[0x20];
    u32 money;          /* 0x20 */
    u32 header24;       /* 0x24 */
    u32 header28;       /* 0x28 */
    u32 header2C;       /* 0x2C */
    u8 pad30[0xA24];
    u32 slotFlags;      /* 0xA54 */
    u8 padA58[0x1DBF8];
    u32 savedMoney;     /* 0x1E650 */
} FileSaveState;

typedef struct FileQueue {
    u8 unk0[0x68];
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

extern void func_002D3F08(void *arg0);

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

extern void func_002C9400(u32 arg0, const char *arg1, void *arg2, s32 arg3);

extern char D_0042B6B8[];

extern u8 D_00458040[];

extern void fileReqClearSlotFlags(s32, s32);

extern u8 fileReqIsSlotMetadataDirty(s32 arg0);

extern void *mcClearSlotMetadata(void);

extern void *mcChooseLoadPath(void);

extern s32 D_00437D2C;

extern void *fileBuildMainBlobAfterDelete(void);

extern void *func_002CC210();

extern void func_002C9490(void);

extern s32 mnuSelectFileBranch(void);

extern u32 D_00437D0C;

extern u8 D_003846F0[];

extern u8 D_0037F610[];

extern u8 D_0037F660[];

extern void func_00336C10(void *);

extern void func_002D46F0(FileQueue *queue, void *vec);

extern void func_002D48D0(FileQueue *queue, f32 scale);

extern void func_002D49B8(FileQueue *queue, u32 color);

extern void fileJobCopyHeader(FileJob *dst, FileJob *src);

extern s32 func_002C93B8(void);

extern s32 func_002C94B0(void);

extern void func_00328E48();

extern f32 D_00437D8C;

extern f32 sdfSinPoly(f32);

extern s32 itfMesGetGlobalWindowValue(void);

extern u8 D_00438B66;

extern char D_00437E10[];

extern char D_00437E18[];

extern char D_00437E20[];

extern char *func_0033EC18(void);

extern s32 func_00369B70(const char *path, s32 flags, ...);

extern void func_002D3B48(s32 fd, s32 arg1);

extern void func_00369DF8(s32 fd);

extern void func_0036BCD0(const char *path, s32 arg1);

void fileWriteBegin(s32 request, u32 first, u32 second) {
    func_0034F490();
}

extern s32 func_0034F680(s32, s32 *, s32 *);

/* Poll the memory-card write: busy is 0, success 1, and the card's
 * -4 status is translated to the menu's -2 error. */
s32 fileWriteWait(void) {
    u32 cmdId;
    s32 status;
    s32 result = func_0034F680(1, &cmdId, &status);

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
    if (D_00437CD8 != 0) {
        kwlnTaskDestroyWithHierarchy(D_00437CD8, 1);
        D_00437CD8 = 0;
        D_00437CD4 = 0;
    }
}

s8 fileMenuTaskIsAlive(void) {
    return D_00437CD4;
}

void func_002C9708(void) {
}

void mcdFormatSaveSlotName(void *buffer, s32 slot) {
    s32 titleId = 0x52a0;
    if (D_00437D7C != 0) {
        titleId = 0x51ee;
    }
    D_00437D88 = titleId;
    func_0035C860(buffer, "BASLUS-%05d-new-%d", titleId, slot);
}

void fileReqGetSlotCode(void) {
    s32 slot = fileReqGetSelectedSlot(D_00437CD0);
    D_00437D50 = D_004580C3[slot * 0x30];
}

/* Size of the persistent main save block copied during a reload. */
#define FILE_MAIN_BLOB_SIZE 0x1E840

u32 fileMainBlobSize(void) {
    return FILE_MAIN_BLOB_SIZE;
}

void fileReloadSaveBuffer(void) {
    s32 saved = *(s32 *)(D_00435DD0 + 0x30);
    s32 size = FILE_MAIN_BLOB_SIZE;
    memcpy((void *)D_00435DD0, (void *)D_00439044, size);
    *(s32 *)(D_00435DD0 + 0x30) = saved;
}

u8 fileIsLoadedAndConditionTrue(s32 arg0) {
    return arg0 != 0 && D_00437CE4 == 1;
}

u8 func_002C97F8(s32 arg0) {
    return arg0 != 0 && D_00437CE4 == 1;
}

void func_002C9818(s32 x, s32 y, u64 width, u64 height) {
    u32 handle = func_0019F460(x << 4, y << 3, 0, width, height, 0);
    D_00439004 = handle;
    func_0019D530(handle, 1);
    func_0019C5B0(D_00439004);
}

void mcdCreateConfiguredDrawHandle(s32 x, s32 y, u64 width, u64 height) {
    u32 handle = func_0019F460(x << 4, y << 3, 0, width, height, 0);
    D_00439008 = handle;
    frFontSetChainFlag(handle, 3);
    func_0019D530(D_00439008, 1);
    func_0019C5B0(D_00439008);
}

void mcdCreateFontDrawHandle(s32 arg0, s32 arg1, u32 arg2, u32 arg3) {
    func_0019D1D0(1);
    D_0043900C = func_0019CE78(arg3, 0, 0, 0, 0);
    func_0019D1E0(1);
    frFontSetFlagAndMeasureGlyphs(D_0043900C, 1);
    func_0019D100(D_0043900C, arg0 << 4, arg1 << 3);
    frFontSetChildColors(D_0043900C, arg2);
    func_0019D550(D_0043900C, 0, 0x56);
    func_0019C5B0(D_0043900C);
    func_0019D1F8(0x54);
}

void fileDrawMenuImageAtPoint(s32 x, s32 y, u64 width, u64 height) {
    u64 handle;

    handle = func_0019F5E8(x << 4, y << 3, 0, width, height, 0);
    func_0019D530(handle, 1);
    func_0019C5B0(handle);
}

/* Animate the save-window highlight's alpha with a sinusoidal phase. */
void fileDrawPulsingSaveHighlight(void) {
    s32 angle;
    f32 wave;

    evtSetDrawSurfaceIndex(0x56);
    func_00108BD8(0);
    D_00437D8C = D_00437D8C + 0.39999998f;
    sdfSinPoly(D_00437D8C);
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
    func_00108BD8(0);
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

INCLUDE_ASM(const s32, "game/code_002C9660", func_002C9BD0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002C9CF8);

void fileSetMenuFlowState(u32 value) {
    s32 previous;

    previous = D_00437D38;
    D_00437D38 = value;
    if (previous == 0) {
        D_00437D48 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CA1F0);

void fileClearAllSlotFlags(void) {
    fileReqMarkSlotMetadataDirty(D_00437CD0);
    fileReqClearSlotFlags(D_00437CD0, 0);
    fileReqClearSlotFlags(D_00437CD0, 1);
    fileReqClearSlotFlags(D_00437CD0, 2);
    fileReqClearSlotFlags(D_00437CD0, 3);
    fileReqClearSlotFlags(D_00437CD0, 4);
    fileReqClearSlotFlags(D_00437CD0, 5);
    fileReqClearSlotFlags(D_00437CD0, 6);
    fileReqClearSlotFlags(D_00437CD0, 7);
    fileReqClearSlotFlags(D_00437CD0, 8);
    fileReqClearSlotFlags(D_00437CD0, 9);
}

void *fileBeginSlotOpen(void) {
    char path[0x50];
    u32 ctx;
    s32 slot;
    s32 len;

    fileReqSetSelectedSlot(D_00437CD0, D_00437D10);
    ctx = D_00437CD0;
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
    func_002C9500(ctx, path, 1);
    return fileReadSlotPreviewBegin;
}

void *fileReadSlotPreviewBegin(void) {
    s32 status = func_002C9528(&D_0043903C);

    if (status == 0) {
        return NULL;
    }
    if (status == 1) {
        D_00439040 = func_003292A8(0x30);
        D_00439044 = sdfResourceRetainAddress(D_00439040);
        func_002C95F0(D_0043903C, D_00439044, 0x30);
        return fileReadSlotPreviewWait;
    }
    return fileBeginSlotMetadataRefresh();
}

extern s32 func_002C9608(void);
extern void func_002C9588(u32);
extern void *fileStoreSlotHeader(void);

void *fileReadSlotPreviewWait(void) {
    s32 r = func_002C9608();

    if (r == 0) {
        return 0;
    }
    if (r == 1) {
        func_002C9588(D_0043903C);
        return (void *)fileStoreSlotHeader;
    }
    func_003297C8(D_00439040);
    return fileBeginSlotMetadataRefresh();
}

extern s32 fileWaitCommandDone(void);
extern u8 D_004580C0[];

void *fileStoreSlotHeader(void) {
    s32 status = fileWaitCommandDone();

    if (status == 0) {
        return NULL;
    }
    if (status == 1) {
        memcpy(D_004580C0 + D_00437D10 * 0x30, (void *)D_00439044, 0x30);
        func_003297C8(D_00439040);
        return func_002CBA90();
    }
    func_003297C8(D_00439040);
    return fileBeginSlotMetadataRefresh();
}

void *fileBeginWait(s32 result) {
    kwlnFadeInStart(0, 0, 0, 15);
    D_0043901C = (void *)result;
    D_00437CE8 = 0x14;
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
    D_00437CF0 = 30;
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
    fileReqBegin(D_00437CD0);
    return func_002CB2C0;
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
    D_00437CF0 = 15;
    D_00437CEC = 1;
    fileSetMenuFlowState(1);
    D_00437CF8 = 1;
    return fileSlotStatusPoll;
}
extern void func_001027D8(s32, const s32 *, s32, s32);
extern void *fileMenuWorkStart(void);


u32 fileAbortSlotFlow(void) {
    fileSetMenuFlowState(0);
    D_00437CF8 = 0;
    D_00437CF4 = 0;
    return 0xffffffff;
}

void *mcdEnterDefaultFileFlow(void) {
    s32 mode = 0;
    if (D_00437D7C != 0) {
        return fileMenuWorkStart;
    }
    func_001027D8(2, &mode, 4, 0);
    return 0;
}

void *func_002CACF0(void) {
    return mcdEnterDefaultFileFlow();
}

void *mcdEnterSelectedFileFlow(void) {
    s32 mode = 2;
    D_00437D34 = 1;
    if (D_00437D7C != 0) {
        return fileMenuWorkStart;
    }
    func_001027D8(2, &mode, 4, 0);
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
extern void func_002CCAD0(void);

void *fileScanSlotStates(void) {
    s32 i;

    D_00437D14 = 0;
    D_00437CF8 = 1;
    for (i = 0; i < 10; i++) {
        u32 buttons = fileReqGetSlotFlags(D_00437CD0, i);

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
    fileReqBegin(D_00437CD0);
    return func_002CCAD0;
}

void func_002CAE58(void) {
    if (D_00437D30 == 1 && func_00101740(D_0042B698) == NULL) {
        mcdEnterSelectedFileFlow();
    } else {
        fileSetMenuFlowState(0);
        fileBeginWait(&fileAbortSlotFlow);
    }
}

extern u32 D_00439030;
extern u32 D_00437CE0;
extern char D_0042B6A8[];

void func_002CAEA8(void) {
    D_00439030 = 0;
    D_00437CE0 = func_002C80C8(D_0042B6A8);
    fileResetSelection();
}

void func_002CAED0(void) {
    fileResetSelection();
}

void func_002CAEE8(void) {
    D_00439030 = 0;
    D_00437CE0 = func_002C80C8(D_0042B6A8);
    fileBeginSlotReset();
}

void fileResetSlotPollState(void) {
    fileReqBegin(0);
    D_00437CEC = 1;
    D_00437CF0 = 0;
    fileSetMenuFlowState(0);
    D_00437CF8 = 0;
    fileReturnToSlotSelection();
}

void *fileSlotStatusPoll(void) {
    s32 status;

    if (fileReqPoll() == 0) {
        return NULL;
    }
    if (D_00437CF0 > 0) {
        D_00437CF0--;
        fileReqBegin(D_00437CD0);
        return NULL;
    }
    status = fileReqGetStatus(D_00437CD0);
    switch (status) {
    case 0:
        return fileBeginSlotMetadataRefresh();
    case 2:
        if (D_00437D30 == 0) {
            fileClearAllSlotFlags();
            fileSetMenuFlowState(0);
            D_00437D3C = 0;
            D_00437D1C = 2;
            return (void *)fileBeginPromptDialog(func_002CC760, mnuSelectFileBranch, 1);
        }
        fileSetMenuFlowState(0);
        D_00437D3C = 7;
        D_00437D40 = 0;
        fileReqBegin(D_00437CD0);
        return func_002CB2C0;
    case 3:
        fileClearAllSlotFlags();
        fileSetMenuFlowState(0);
        D_00437D3C = 2;
        D_00437D40 = 0;
        fileReqBegin(D_00437CD0);
        return func_002CB2C0;
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
        u32 flags = fileReqGetSlotFlags(D_00437CD0, index);
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
    return fileReqGetSize(D_00437CD0) > 0x2A7FF;
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
    if ((s32)D_00437CF0 > 0) {
        D_00437CF0--;
        fileReqBegin(D_00437CD0);
        return 0;
    }
    switch (fileReqGetStatus(D_00437CD0)) {
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
    if ((s32)D_00437CF0 > 0) {
        D_00437CF0--;
        fileReqBegin(D_00437CD0);
        return NULL;
    }
    switch (fileReqGetStatus(D_00437CD0)) {
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

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CB2C0);

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
        if (fileReqGetStatus(D_00437CD0) != 1) {
            D_00437D3C = 0;
            D_00437CF8 = 0;
            return fileResetSelection();
        }
        fileReqBegin(D_00437CD0);
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
    s32 remaining = D_00437CE8 - 1;
    D_00437CE8 = remaining;
    if (remaining <= 0) {
        if (D_00439020 == -1) {
            return (void *)-1;
        }
        return ((void *(*)(void))D_0043901C)();
    }
    return NULL;
}

void *func_002CB660(u32 arg0) {
    D_00439020 = arg0;
    D_00437D40 = 0;
    return mcdFinishFileDetection;
}

void mcdFinishFileDetection(void) {
    if (fileIsLoadedAndConditionTrue((u32)D_0037F510[0x21] >> 31) ||
        fileIsLoadedAndConditionTrue((u32)D_0037F510[0x23] >> 31)) {
        fileSetMenuFlowState(0);
        D_00437D3C = 0;
        D_00437CF8 = 0;
        if (D_00437D7C == 0) {
            func_00342580(0x310000);
        }
        if (D_00439020 == (u32)-1) {
            fileBeginWait(-1);
            return;
        }
        ((void (*)(void))D_00439020)();
    }
}

void *fileBeginDetectionRequest(u32 arg0) {
    D_00439020 = arg0;
    D_00437D40 = 0;
    fileReqBegin(D_00437CD0);
    return func_002CB740;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CB740);

void *fileBeginReadSlotIcon(void) {
    u8 buf[0x50];

    buf[0] = 0x2F;
    mcdFormatSaveSlotName(&buf[1], D_00437D10);
    func_002C9328(D_00437CD0, buf);
    return fileScanSlotIconSysBegin;
}

void *fileScanSlotIconSysBegin(void) {
    s32 t = mcPollSyncResult();

    if (t == 0) {
        return NULL;
    }
    if (t == 1) {
        fileReqSetSlotFlags(D_00437CD0, D_00437D10, 2);
        func_002C9400(D_00437CD0, D_0042B6B8, D_00458040, 1);
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
            fileReqSetSlotFlags(D_00437CD0, D_00437D10, 9);
        }
        return fileBeginSlotOpen();
    }
    if (status == -1) {
        return fileBeginSlotMetadataRefresh();
    }
    return fileBeginSlotOpen();
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CBA90);

void *mcResetSlotMetadata(void) {
    s32 index;
    if (fileReqIsSlotMetadataDirty(D_00437CD0) != 0) {
        fileSetMenuFlowState(1);
        D_00437D10 = 0;
        index = 0;
        do {
            D_00458080[index] = 0;
            fileReqClearSlotFlags(D_00437CD0, index);
            index++;
        } while (index < 10);
        return fileBeginReadSlotIcon();
    } else {
        D_00437CEC = 0;
        fileSetMenuFlowState(0);
        return fileScanSlotStates();
    }
}

void *func_002CBC60(void) {
    u8 buf[0x50];

    buf[0] = 0x2F;
    mcdFormatSaveSlotName(&buf[1], D_00437D10);
    func_002C9328(D_00437CD0, buf);
    return fileScanSlotIconSysAltBegin;
}

void *fileScanSlotIconSysAltBegin(void) {
    s32 t = mcPollSyncResult();

    if (t == 0) {
        return NULL;
    }
    if (t == 1) {
        fileReqSetSlotFlags(D_00437CD0, D_00437D10, 2);
        func_002C9400(D_00437CD0, D_0042B6B8, D_00458040, 1);
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
            fileReqSetSlotFlags(D_00437CD0, D_00437D10, 9);
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
    s32 slot = D_00437D10;
    s32 buttons = fileReqGetSlotFlags(D_00437CD0, slot);

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
    D_00437D10 = slot;
    if (slot == 10) {
        fileSetMenuFlowState(0);
        D_00437CF8 = 0;
        D_00437D3C = 0;
        return (void *)fileBeginSlotPromptSix();
    }
    return func_002CBC60();
}

void *mcClearSlotMetadata(void) {
    s32 index = 0;
    u32 *saved;
    fileSetMenuFlowState(1);
    D_00437CF8 = 0;
    D_00437D10 = 0;
    fileReqMarkSlotMetadataDirty(D_00437CD0);
    saved = D_00458080;
    do {
        *saved++ = 0;
        fileReqClearSlotFlags(D_00437CD0, index);
        index++;
    } while (index < 10);
    return func_002CBC60();
}

void *mcPrepareDirectory(void) {
    u8 name[0x50];
    u32 entry = D_00437CD0;
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
    u32 entry = D_00437CD0;
    s32 v = fileReqGetSelectedSlot(entry);

    buf[0] = 0x2F;
    mcdFormatSaveSlotName(&buf[1], v);
    func_002C9328(entry, buf);
    return filePrepareMainBlobWrite;
}

void *mcHandleSearchResult(void) {
    s32 status = func_002C93B8();
    if (status == 0) {
        return NULL;
    }
    if (status == 1) {
        fileReqSetSlotFlags(D_00437CD0, D_00437D2C, 2);
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
    s32 t = func_002C94B0();

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
    fileReqSetSlotFlags(D_00437CD0, D_00437D2C, 1);
    fileReqSetSlotFlags(D_00437CD0, D_00437D2C, 8);
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

s32 func_002CC168(void) {
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
extern s32 func_002CC168(void);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B698);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B6A8);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B6B8);

s32 fileRequestBaseIcon(void) {
    return fileBeginRequest("base.ico", &D_00439034, &D_00439038, func_002CC168, &D_00437CE0);
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CC210);

void *mcChooseLoadPath(void) {
    u32 entry = D_00437CD0;
    s32 slot = fileReqGetSelectedSlot(entry);
    u32 flags = fileReqGetSlotFlags(entry, slot);
    if (!(flags & 8)) {
        return func_002CC210(entry, D_0042B6B8);
    }
    func_002C9490();
    return fileBuildMainBlobAfterDelete;
}

extern u32 D_00439048;
extern u32 D_0043904C;
extern u32 D_00439050;
extern u32 D_00439054;
extern s32 fileWriteWaitOpen(void);

s32 fileBeginRequest(char *name, void *a1, void *a2, void *a3, void *a4) {
    D_00439048 = (u32)a1;
    D_0043904C = (u32)a2;
    D_00439050 = (u32)a3;
    D_00439054 = (u32)a4;
    func_002C9500(D_00437CD0, name, 0x203);
    return (s32)fileWriteWaitOpen;
}

INCLUDE_ASM(const s32, "game/code_002C9660", fileWriteWaitOpen);

extern s32 fileWriteWait(void);
extern void *mcDispatchReadCallback(void);

void *mcHandleLoadResult(void) {
    s32 status = fileWriteWait();

    if (status == 0) {
        return 0;
    }
    if (status == 1) {
        func_002C9588(D_0043903C);
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
    s32 status = fileWaitCommandDone();

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
    if (D_00437CE0 != 0) {
        return NULL;
    }
    fileWriteBegin(D_0043903C, *(u32 *)D_00439048, *(u32 *)D_0043904C);
    return mcHandleLoadResult;
}

void *mcHandleDetectionResult(void) {
    s32 status = func_002C92E8();

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

void *func_002CC760(void) {
    fileSetMenuFlowState(5);
    func_001004A0();
    func_002C92D0(D_00437CD0);
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
    u32 ctx = D_00437CD0;
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
    func_002C9500(ctx, path, 1);
    return fileLoadMainBlobBegin;
}

void *fileLoadMainBlobBegin(void) {
    s32 status = func_002C9528(&D_0043903C);
    u32 size;

    if (status == 0) {
        return NULL;
    }
    size = fileMainBlobSize();
    D_00439040 = func_003292A8(size);
    D_00439044 = sdfResourceRetainAddress(D_00439040);
    if (status == 1) {
        func_002C95F0(D_0043903C, D_00439044, size);
        return mcHandleSetupResult;
    }
    fileSetMenuFlowState(0);
    D_00437D3C = 3;
    return fileAbortSlotScanOnInput;
}

extern void *mcdHandleSaveSetupDone(void);
void *mcHandleSetupResult(void) {
    s32 status = func_002C9608();

    if (status == 0) {
        return 0;
    }
    if (status == 1) {
        func_002C9588(D_0043903C);
        return (void *)mcdHandleSaveSetupDone;
    }
    func_003297C8(D_00439040);
    fileSetMenuFlowState(0);
    D_00437D3C = 3;
    return (void *)fileAbortSlotScanOnInput;
}

extern s8 D_00437CD5;

void *mcdHandleSaveSetupDone(void) {
    s32 status = fileWaitCommandDone();

    if (status == 0) {
        return NULL;
    }
    if (status == 1) {
        fileReloadSaveBuffer();
        func_003297C8(D_00439040);
        D_00437CD5 = 1;
        fileDestroyMenuTask();
        if (fileLoadStateChanged() == 0) {
            fileCacheSlotFlagsFromState();
        } else {
            fileRestoreSlotFlagsToState();
        }
        fileSetMenuFlowState(13);
        return func_002CB660(-1);
    }
    func_003297C8(D_00439040);
    fileSetMenuFlowState(0);
    D_00437D3C = 3;
    return (void *)fileAbortSlotScanOnInput;
}

extern s32 mcdContinueLoadSelection();

void *func_002CCAA8(void) {
    fileSetMenuFlowState(13);
    return func_002CB660((u32)mcdContinueLoadSelection);
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CCAD0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CD028);

void *fileRunMenuState(s32 arg) {
    void *next;
    u32 job;
    void *(*cur)(s32);
    D_00437CFC++;
    fileDrawMenuFrame(arg);
    next = D_00439018(arg);
    if (next == (void *)-1) {
        return next;
    }
    job = D_00437CE0;
    cur = D_00439018;
    if (next != NULL) {
        cur = next;
    }
    D_00439018 = cur;
    if (job != 0 && func_002C8128(job, next) != 0) {
        D_00437CE0 = 0;
        D_00439030 = fileGetResourceHandle(job);
        D_00439034 = func_002C8110(job);
        D_00439038 = fileGetResourceSize(job);
        func_002C7D00(job);
    }
    return NULL;
}

extern s32 D_00437D4C;

s32 fileDrawMenuFrame(s32 arg0) {
    s32 fade;
    s32 alpha;

    evtSetDrawSurfaceIndex(0x52);
    func_00108BD8(0);
    evtSubmitGsRegister47(1, 0, 0x80, 3, 0, 0, 1, 1);
    if (D_00437CF4 != 0) {
        func_00108EC0(0, 0, 0x200, 0x1C0, 0, 0, 0x200, 0x1C0, 0x80808080, 0x80808080, 0x80808080, 0x80808080,
                      D_00437D74);
        func_00108EC0(0, 0, 0x200, 0x1C0, 0, 0, 0x200, 0x1C0, 0x80808080, 0x80808080, 0x80808080, 0x80808080,
                      D_00437D78);
    }
    if (D_00437CF8 != 0) {
        func_002CD028(arg0);
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
    func_00108BD8(0);
    evtSubmitGsRegister47(1, 5, 0x80, 3, 0, 0, 1, 2);
    return 0;
}

void fileResetMenuFlowState(void) {
    fileSetMenuFlowState(0);
    D_00437D04 = 0;
    D_00437CE8 = 0;
    D_00437D18 = 0xffffffff;
    D_00437D34 = 0;
    D_00437D3C = 0;
    D_00437D40 = 0;
    D_00437CF8 = 0;
    D_00437D08 = 0;
    D_00437CF0 = 0;
    D_00437CFC = 0;
    D_00437CEC = 0;
    D_00437D14 = 0;
    D_00437D1C = 0;
    D_00437D20 = 0;
    D_00437D44 = 0;
    D_00437D48 = 0;
    fileClearAllSlotFlags();
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CE208);

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
            func_00110A88(world, 1);
        }
        if (D_00437CE0 != 0) {
            fileWaitReady(D_00437CE0);
            D_00439030 = fileGetResourceHandle(D_00437CE0);
            func_002C7D00(D_00437CE0);
            D_00437CE0 = 0;
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
    return func_00101740(D_0042B720) != NULL;
}

u32 fileGetSelectionPendingFlag(void) {
    return D_00437D34;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CE958);

extern u32 D_00439024;
extern u32 D_00439028;
extern void *func_002CE958(void);

s32 fileBeginPromptDialog(void *start, void *finish, s32 mode) {
    D_00439024 = (u32)start;
    D_00439028 = (u32)finish;
    D_00437D18 = (u32)mode;
    D_00437D20 = 0;
    if (D_00437D1C != 4 && D_00437D1C != 8) {
        fileReqBegin(D_00437CD0);
    }
    return (s32)func_002CE958;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CEEC0);

u32 func_002CF928(void) {
    return D_00437D04;
}

typedef struct FilePreviewWork {
    u8 pad0[0x110F0];
    s16 previewX;
    s16 previewY;
} FilePreviewWork;

void fileSetPreviewLocation(s16 x, s16 y) {
    FilePreviewWork *work = (FilePreviewWork *)D_00435DD0;

    work->previewX = x;
    work->previewY = y;
}

void func_002CF958(u32 arg0) {
    D_003E8008[0] = arg0;
    D_003E7FD8.unkC = 0x80;
    D_003E7FD8.unk4 = 0;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CF978);

void fileCursorStepUp(void) {
    D_003E7FD8.unk4 += 1;
    D_003E7FD8.unk4 = D_003E7FD8.unk4 <= 0 ? 0 : D_003E7FD8.unk4 > 12 ? 12 : D_003E7FD8.unk4;
    if (D_003E7FD8.unk4 >= 7) {
        D_003E7FD8.unkC += 0x30;
        D_003E7FD8.unkC = D_003E7FD8.unkC <= 0 ? 0 : D_003E7FD8.unkC > 0x80 ? 0x80 : D_003E7FD8.unkC;
    }
}

void fileFadeStepDown(void) {
    D_003E8008[0] -= 0x10;
    D_003E8008[0] = D_003E8008[0] <= 0 ? 0 : D_003E8008[0] > 0x80 ? 0x80 : D_003E8008[0];
}

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B720);

INCLUDE_RODATA(const s32, "game/code_002C9660", jtbl_0042B730);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B770);

void fileDrawSlotIcon(s32 index, s32 x, s32 y, s32 alpha) {
    s32 uv[21][2] = {
        {2, 2},   {2, 2},   {2, 20},  {2, 38},  {2, 56},  {2, 74},  {26, 2},
        {26, 20}, {26, 38}, {26, 56}, {26, 74}, {26, 74}, {26, 74}, {26, 74},
        {26, 74}, {26, 74}, {26, 74}, {26, 74}, {26, 74}, {26, 74}, {26, 74},
    };

    evtSetDrawSurfaceIndex(0x53);
    func_00108BD8(0);
    evtSubmitGsRegister47(1, 0, 0x80, 3, 0, 0, 1, 1);
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

/* Init record at D_003E8018. */
typedef struct Init374E0 {
    s8 mode[3];      /* 0x00 */
    u8 pad3;         /* 0x03 */
    s32 pos[3][2];   /* 0x04 */
    s32 counter[3];  /* 0x1C */
    u8 pad28[0xC];   /* 0x28 */
    s32 alpha[3];    /* 0x34 */
} Init374E0;

extern Init374E0 D_003E8018;
extern s32 D_003E7FE4[];

void fileInitCursorPulse(void) {
    D_003E8018.mode[1] = 0;
    D_003E7FE4[0] = 0x80;
    D_003E8018.mode[0] = 1;
    D_003E8018.mode[2] = 2;
    D_003E8018.alpha[0] = 0x80;
    D_003E8018.alpha[1] = 0x80;
    D_003E8018.alpha[2] = 0x80;
}

void fileCursorPulseUpdate(void) {
    s32 i;

    for (i = 0; i < 3; i++) {
        s32 mode = D_003E8018.mode[i];

        if (mode < 3) {
            if (mode >= 0) {
                switch (mode) {
                case 0:
                    D_003E8018.alpha[i] -= 12;
                    D_003E8018.alpha[i] = D_003E8018.alpha[i] <= 0x40 ? 0x40 : D_003E8018.alpha[i] > 0x80 ? 0x80 : D_003E8018.alpha[i];
                    break;
                case 1:
                    D_003E8018.alpha[i] -= 28;
                    D_003E8018.alpha[i] = D_003E8018.alpha[i] <= 0 ? 0 : D_003E8018.alpha[i] > 0x80 ? 0x80 : D_003E8018.alpha[i];
                    break;
                }
                D_003E8018.counter[i] += 1;
                D_003E8018.counter[i] = D_003E8018.counter[i] <= 0 ? 0 : D_003E8018.counter[i] > 6 ? 6 : D_003E8018.counter[i];
                if (D_003E8018.counter[i] >= 6) {
                    D_003E8018.mode[i]++;
                    D_003E8018.alpha[i] = 0x80;
                    if (D_003E8018.mode[i] >= 3) {
                        D_003E8018.mode[i] = 0;
                    }
                    D_003E8018.counter[i] = 0;
                }
                fileCursorOffsetLookup(i, 0, &D_003E8018.pos[i][0], &D_003E8018.pos[i][1]);
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CFF38);

void fileLoadSetMode(s8 mode) {
    /* Raw stores on purpose: writing these through D_003E7FD8's fields tips
       fileAcquireRecord's CSE in the full unit (check_unit reports CONTEXT),
       so the pointer form is what the build needs here. */
    u8 *p = (u8 *)&D_003E7FD8;

    *(u32 *)(p + 0x14) = 0;
    *(u8 *)(p + 0x10) = mode;
    if (mode == 1) {
        *(u32 *)(p + 0x1C) = 0;
        *(u32 *)(p + 0x18) = 0x74;
    } else {
        *(u32 *)(p + 0x18) = 0;
        *(u32 *)(p + 0x1C) = 0x74;
    }
}

void fileResetLoadContextSlide(void) {
    D_003E7FD8.unk14 = 0;
    D_003E7FD8.unk10 = 0;
    D_003E7FD8.unk4 = 0;
}

void fileLoadCtxSlideUpdate(void) {
    switch (D_003E7FD8.unk10) {
    case 1:
        D_003E7FD8.unk14 -= 0x14;
        D_003E7FD8.unk14 = D_003E7FD8.unk14 <= -0x6E ? -0x6E : D_003E7FD8.unk14 > 0 ? 0 : D_003E7FD8.unk14;
        if (D_003E7FD8.unk14 <= -0x6E) {
            D_003E7FD8.unk14 = 0;
            D_003E7FD8.unk10 = 0;
        }
        D_003E7FD8.unk18 -= 0x20;
        D_003E7FD8.unk18 = D_003E7FD8.unk18 <= 0 ? 0 : D_003E7FD8.unk18 > 0x80 ? 0x80 : D_003E7FD8.unk18;
        D_003E7FD8.unk1C += 0x10;
        D_003E7FD8.unk1C = D_003E7FD8.unk1C <= 0 ? 0 : D_003E7FD8.unk1C > 0x80 ? 0x80 : D_003E7FD8.unk1C;
        break;
    case 2:
        D_003E7FD8.unk14 += 0x14;
        D_003E7FD8.unk14 = D_003E7FD8.unk14 <= 0 ? 0 : D_003E7FD8.unk14 > 0x6E ? 0x6E : D_003E7FD8.unk14;
        if (D_003E7FD8.unk14 >= 0x6E) {
            D_003E7FD8.unk14 = 0;
            D_003E7FD8.unk10 = 0;
        }
        D_003E7FD8.unk18 += 0x10;
        D_003E7FD8.unk18 = D_003E7FD8.unk18 <= 0 ? 0 : D_003E7FD8.unk18 > 0x80 ? 0x80 : D_003E7FD8.unk18;
        D_003E7FD8.unk1C -= 0x20;
        D_003E7FD8.unk1C = D_003E7FD8.unk1C <= 0 ? 0 : D_003E7FD8.unk1C > 0x80 ? 0x80 : D_003E7FD8.unk1C;
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

void *fileCreateDetectionAudioCallback(u32 arg0) {
    D_00439020 = arg0;
    D_00437D40 = 0;
    return mcdFinishFileDetectionWithAudio;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0358);

extern u32 D_00439024;
extern u32 D_00439028;
extern u32 D_0043902C;
extern s32 func_002D0358();

void *fileBeginFourWayDialog(u32 ready, u32 completed, u32 cancelled, u32 state) {
    D_00439024 = ready;
    D_00439028 = completed;
    D_0043902C = cancelled;
    D_00437D18 = state;
    D_00437D20 = 0;
    return func_002D0358;
}

u32 func_002D0490(void) {
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B8F8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0498);

extern s32 func_002D0498();

void *fileBeginFadeAndConfirmSound(void) {
    kwlnFadeInStart(0, 0, 0, 8);
    func_00342580(0x310000);
    return func_002D0498;
}

typedef struct MenuWork {
    u8 pad0[0x30];
    u8 unk30;
    u8 unk31;
    u16 unk32;
    u16 unk34;
    u16 unk36;
    u16 unk38;
    u16 unk3A;
    u32 unk3C;
} MenuWork;

void func_002D06B0(void) {
    ((MenuWork *)D_00437D84)->unk31 = 0;
    fileBeginFadeAndConfirmSound();
}

void func_002D06D0(void) {
    ((MenuWork *)D_00437D84)->unk31 = 1;
    fileBeginFadeAndConfirmSound();
}

void *func_002D06F0(void) {
    fileSetMenuFlowState(0);
    D_00437D1C = 12;
    return fileBeginFourWayDialog((u32)func_002D06B0, (u32)func_002D06D0, 0, 0);
}

void *func_002D0730(void) {
    D_00437D18 = -1;
    D_00437D1C = 0;
    fileSetMenuFlowState(0x18);
    ((MenuWork *)D_00437D84)->unk30 = 1;
    return fileCreateDetectionAudioCallback((u32)func_002D06F0);
}

void *func_002D0770(void) {
    fileResetMenuFlowState();
    return func_002CAED0;
}

void func_002D0798(void) {
    fileSetMenuFlowState(0);
    D_00437D1C = 11;
    fileBeginFourWayDialog((u32)func_002D0730, (u32)func_002D0770, (u32)func_002D0770, 0);
}

extern s32 func_002D0810();

void *func_002D07D8(void) {
    if (fileIsLoadStepComplete() != 0) {
        return fileCreateDetectionAudioCallback((u32)func_002D0810);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0810);

extern u32 D_00435CD4;

s32 fileResetPendingRequest(void) {
    s32 mode;
    if (D_00435CD4 & 2) {
        return 0;
    }
    D_00437D18 = -1;
    mode = 3;
    D_00437D1C = 0;
    func_001027D8(2, &mode, 4, 0);
    return 0;
}

void *func_002D08F0(void) {
    kwlnFadeInStart(0, 0, 0, 8);
    return fileResetPendingRequest;
}

void func_002D0920(void) {
    fileSetMenuFlowState(0);
    D_00437D1C = 10;
    ((MenuWork *)D_00437D84)->unk30 = 0;
    fileBeginFourWayDialog((u32)func_002D0770, (u32)fileBeginFadeAndConfirmSound, (u32)func_002D08F0, 0);
}

s32 mcdContinueLoadSelection(void) {
    u32 state = func_002CF928();
    u32 block;
    s32 next = (s32)fileMenuWorkStart;
    if (state != 0) {
        if (state == 2) {
            block = D_00437D84;
            ((MenuWork *)block)->unk36 = 0;
            func_002D0D90(block);
            sndSetSequenceVolumePan(8, 0x7f, 0x3f);
            next = (s32)func_002D0810;
        } else {
            next = 0;
        }
    }
    return next;
}

extern s32 kwlnFadeIsActive(void);
extern void kwlnFadeStartIn(s32);
extern void func_002D0920(void);
extern s32 D_00437D28;

void *fileMenuWorkStart(void) {
    D_00437D2C = 0;
    D_00437D28 = 0;
    D_00437CF8 = 0;
    if (kwlnFadeIsActive()) {
        kwlnFadeStartIn(0x10);
    }
    if (((MenuWork *)D_00437D84)->unk34 == 0) {
        return func_002D0920;
    }
    ((MenuWork *)D_00437D84)->unk30 = 1;
    return func_002D06F0();
}


void fileMenuWorkCreate(u32 arg0) {
    u32 buffer = func_003292A8(0x40);
    MenuWork *work;

    D_00437D84 = sdfResourceRetainAddress(buffer);
    memset((void *)D_00437D84, 0, 0x40);
    work = (MenuWork *)D_00437D84;
    work->unk30 = 0;
    work->unk3C = buffer;
    work->unk34 = arg0;
    work->unk38 = 0;
    ((MenuWork *)D_00437D84)->unk31 = 0;
}

void func_002D0A90(void) {
    if (D_00437D84 != 0) {
        func_003298C0(((MenuWork *)D_00437D84)->unk3C);
        D_00437D84 = 0;
    }
}

extern char D_0042B920[];

void func_002D0AB8(void) {
    func_0035B6E0(D_0042B920);
}

extern char D_0042B938[];

void fileSaveAndDisplayCurrentMoney(void) {
    FileSaveState *state = (FileSaveState *)D_00435DD0;
    u32 money = state->money;
    state->savedMoney = money;
    func_0035B6E0(D_0042B938, money);
}

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B920);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B938);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0B08);

void fileCopySaveHeaderNumbers(s32 arg0) {
    s32 state;

    state = D_00435DD0;
    ((FileSaveState *)D_00435DD0)->money = ((FileSaveState *)arg0)->money;
    ((FileSaveState *)state)->header24 = ((FileSaveState *)arg0)->header24;
    ((FileSaveState *)state)->header28 = ((FileSaveState *)arg0)->header28;
    ((FileSaveState *)state)->header2C = ((FileSaveState *)arg0)->header2C;
}

typedef struct {
    u8 bytes[0x30];
} __attribute__((packed)) FileRecordHeader;

void fileCopyRecordHeader(FileRecordHeader *destination, const FileRecordHeader *source) {
    *destination = *source;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0D90);

s32 fileLoadStateChanged(void) {
    return D_00437DE8.current != D_00437DE8.previous;
}

void fileCacheSlotFlagsFromState(void) {
    D_00437DE8.current = D_00437DE8.previous =
        ((FileSaveState *)D_00435DD0)->slotFlags;
}

void fileRestoreSlotFlagsToState(void) {
    ((FileSaveState *)D_00435DD0)->slotFlags = D_00437DEC;
}

INCLUDE_ASM(const s32, "game/code_002C9660", fileToggleSlotFlagsBit);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B970);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B980);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B998);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B9B0);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B9D0);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B9F0);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042BA10);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042BA30);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042BA50);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042BA70);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042BA90);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042BAB0);

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

void fileTestSavedSlotFlags(u32 arg0) {
    fileTestSlotFlagsBit(arg0, D_00435DD0 + 0xa54);
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D1058);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D11F8);

extern s32 func_002D1058(void);
extern s32 fileStartQueuedLoad(void);
extern void func_002D11F8(void);
extern void func_002D1450(void);
extern u32 fileGetConfigTaskFailure(void);

extern s8 D_00437DE5;

void mnuCreateConfigTasks(void) {
    if (D_00439058 == 0) {
        D_00439058 = func_002D1058();
        kwlnTaskCreate(D_00437DF8, 0x3F2, 1, 1, func_002D1450, NULL, (void *)D_00439058);
        kwlnTaskCreate(D_0042BAF0, 0x2B07, 1, 1, fileStartQueuedLoad, NULL, (void *)D_00439058);
        kwlnTaskCreate(D_0042BB00, 0x520B, 1, 1, fileGetConfigTaskFailure, func_002D11F8, (void *)D_00439058);
        D_00437DE5 = 1;
    }
}

void mnuConfigTasksDestroy(void) {
    kwlnTaskDestroyWithHierarchyByName(D_00437DF8, 1);
    kwlnTaskDestroyWithHierarchyByName(D_0042BAF0, 1);
    kwlnTaskDestroyWithHierarchyByName(D_0042BB00, 1);
}

s32 fileConsumeConfigTaskReady(void) {
    s32 state = D_00437DE5;
    if (state == 1) {
        return 1;
    }
    if (state < 2) {
        return 0;
    }
    if (state == 2) {
        D_00437DE5 = 0;
    }
    return 0;
}

/* Save/config task context: DDS2 places the status four bytes later. */
typedef struct FileConfigTask {
    u8 pad0[0xC];
    u32 frame;      /* 0x0C: passed to menu window drawing */
    u32 slots[4];   /* 0x10 */
    u8 pad20[8];
    s32 result;     /* 0x28: negative when the queued load failed */
    u8 pad2C[0xC];
    u32 pending;    /* 0x38: zero when no load can start */
} FileConfigTask;

u32 fileGetConfigTaskSlot(s32 arg0) {
    if (arg0 < 4) {
        return ((FileConfigTask *)D_00439058)->slots[arg0];
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042BAF0);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042BB00);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D1450);

extern void func_002D27A0(void *);

s32 fileStartQueuedLoad(void) {
    if (((FileConfigTask *)D_00439058)->pending == 0) {
        return 0;
    }
    if (((FileConfigTask *)D_00439058)->result < 0) {
        return -1;
    }
    func_002D27A0((void *)D_00439058);
    mnuCallInitWide(0x400, 0x400, 0, ((FileConfigTask *)D_00439058)->frame, 0x53);
    return 0;
}

u32 fileGetConfigTaskFailure(void) {
    u32 result;

    result = 0xffffffff;
    if ((((FileConfigTask *)D_00439058)->result & 0x80000000) == 0) {
        result = 0;
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D1930);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D27A0);

void func_002D2C50(void) {
    effLoadFlashTextures();
    effLoadWindTexture();
    effLoadScalyTexture();
    func_002D2CA8();
}

void fileSetRenderFlag(u32 arg0) {
    D_00437E08 = D_00437E08 | arg0;
}

void fileClearRenderFlag(u32 arg0) {
    D_00437E08 = D_00437E08 & ~arg0;
}

void func_002D2CA8(void) {
    D_00437E08 = 0;
}

u32 func_002D2CB0(s32 arg0) {
    return D_003E9150[arg0];
}

void mnuProjectViewPoint(void) {
    u8 *matrix;
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf28, 0(%0)\n"
        "lqc2 vf29, 0x10(%0)\n"
        "lqc2 vf30, 0x20(%0)\n"
        "lqc2 vf31, 0x30(%0)\n"
        ".set reorder"
        : : "r"(D_003846F0) : "memory");
    matrix = D_0037F610;
    func_00336C10(matrix);
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
        : : "r"(D_0037F660) : "memory");
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D2D48);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D2EB8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D2FB0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D3128);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D31C0);

FileJob *fileCreateJob(u16 type) {
    u16 kind = type;
    FileJob *job = func_00328D68(0x2C);
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
    job->data = D_003E9168[job->type].create(request);
    return job;
}

void fileJobDestroy(FileJob *job) {
    void *data = job->data;
    if (data != NULL) {
        D_003E9168[job->type].destroy(data);
    }
    fileJobFreePrimaryBuffer(job);
    fileJobFreeSecondaryBuffer(job);
    func_00328E48(job);
}

void fileJobFreePrimaryBuffer(FileJob *job) {
    void *buffer = job->slots[0].allocation;
    if (buffer != NULL) {
        func_003297C8(buffer);
        job->slots[0].offset = 0;
        job->slots[0].size = 0;
        job->slots[0].allocation = NULL;
    }
}

void fileJobFreeSecondaryBuffer(FileJob *job) {
    void *buffer = job->slots[1].allocation;
    if (buffer != NULL) {
        func_003297C8(buffer);
        job->slots[1].offset = 0;
        job->slots[1].size = 0;
        job->slots[1].allocation = NULL;
    }
}

FileJob *fileJobCreateChild(FileJob *request) {
    FileJob *job = fileCreateJob(request->type);
    FileTypeCallbacks *cb = &D_003E9168[job->type];

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

void func_002D3710(void *arg0) {
    u16 idx = ((FileJob *)arg0)->type;
    void *data = ((FileJob *)arg0)->data;

    D_003E916C[idx].cb(data);
}

void func_002D3748(void *work) {
    u16 id = ((FileJob *)work)->type;

    if (D_003E9180[id].func != NULL) {
        D_003E9180[id].func(((FileJob *)work)->data);
    }
}

void func_002D3788(void *work) {
    u16 id = ((FileJob *)work)->type;

    if (D_003E9184[id].func != NULL) {
        D_003E9184[id].func(((FileJob *)work)->data);
    }
}

void func_002D37C8(void *work) {
    u16 id = ((FileJob *)work)->type;

    if (D_003E9188[id].func != NULL) {
        D_003E9188[id].func(((FileJob *)work)->data);
    }
}

void func_002D3808(void *work) {
    u16 id = ((FileJob *)work)->type;

    if (D_003E918C[id].func != NULL) {
        D_003E918C[id].func(((FileJob *)work)->data);
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
        job->slots[0].allocation = (void *)func_003292A8(size);
        job->slots[0].offset = sdfResourceRetainAddress(job->slots[0].allocation);
        job->slots[0].size = size;
        job->option = option;
        memcpy((void *)job->slots[0].offset, src, size);
    }
}

void func_002D38E8(u64 arg0, u64 arg1, u16 arg2) {
    s64 command;
    u64 size;
    u64 handle;
    u64 address;

    command = sdfDevCreateCommandState(arg1);
    if (command != 0) {
        size = func_0033EB30(command);
        handle = func_003292A8(size);
        address = sdfResourceRetainAddress(handle);
        func_0033EB10(command, address, size);
        func_0033EAE0(command);
        fileJobSetPrimaryData(arg0, address, size, arg2);
        func_003297C8(handle);
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
        job->slots[1].allocation = (void *)func_003292A8(size);
        job->slots[1].offset = sdfResourceRetainAddress(job->slots[1].allocation);
        job->slots[1].size = size;
        job->slots[0].selector = selector;
        memcpy((void *)job->slots[1].offset, src, size);
    }
}

void func_002D3A68(u64 arg0, u64 arg1, u16 arg2) {
    s64 command;
    u64 size;
    u64 handle;
    u64 address;

    command = sdfDevCreateCommandState(arg1);
    if (command != 0) {
        size = func_0033EB30(command);
        handle = func_003292A8(size);
        address = sdfResourceRetainAddress(handle);
        func_0033EB10(command, address, size);
        func_0033EAE0(command);
        fileJobSetSecondaryData(arg0, address, size, arg2);
        func_003297C8(handle);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D3B48);

void fileWriteToPfs(s32 arg0, s32 arg1) {
    char path[0xD0];
    s32 fd;

    if (D_00438B66 != 0) {
        func_0035C860(path, D_00437E10, arg1);
        fd = func_00369B70(path, 0x602, 0x1B6);
    } else {
        func_0035C860(path, D_00437E18, func_0033EC18(), arg1);
        fd = func_00369B70(path, 0x602);
    }
    func_002D3B48(fd, arg0);
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
    u64 entry;
{
    s64 command;
    u64 size;
    s32 handle;
    s32 address;
    void *job;

    command = sdfDevCreateCommandState(entry);
    if (command != 0) {
        size = func_0033EB30(command);
        handle = func_003292A8(size);
        address = sdfResourceRetainAddress(handle);
        func_0033EB10(command, address, size);
        func_0033EAE0(command);
        job = fileDuplicateJob((void *)address);
        func_003297C8(handle);
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

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D3F08);

void func_002D3F80(u32 arg0) {
    s32 queue;

    memset(arg0, 0, 0x90);
    queue = (s32)arg0;
    *(u32 *)(queue + 0x84) = 1;
    *(u8 *)(queue + 0x88) = 8;
    *(u8 *)(queue + 0x89) = 0;
    *(u8 *)(queue + 0x8a) = 0;
    func_002D3F08(arg0);
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
    FileQueue *queue = func_00328D68(0x90);
    memset(queue, 0, 0x90);
    queue->count = 0;
    queue->unk84 = 0;
    func_002D3F08(queue);
    return queue;
}

FileJob *fileJobCreate(void) {
    FileJob *job = func_00328D68(0xC0);
    memset(job, 0, 0xC0);
    func_002D3F80(job);
    return job;
}

void func_002D4120(FileJob *job) {
    func_00328E48();
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D4138);

void func_002D4380(u32 arg0, u32 arg1) {
    func_002D4138(arg1);
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D4398);

/* A queued job's +0x90 word is a child pointer when flag 1 is clear; it is
 * an ID for other job kinds. Destroy children before their parent jobs. */
void fileQueueDestroy(FileQueue *queue) {
    FileJob *job = queue->first;
    while (job != NULL) {
        FileJob *next = job->next;
        if ((job->flags & 1) == 0) {
            fileJobDestroy((FileJob *)job->id);
        }
        func_002D4120(job);
        job = next;
    }
    func_00328E48(queue);
}

FileQueue *fileQueueClone(FileQueue *source) {
    FileQueue *queue = fileQueueCreate();
    FileJob *src;
    s128 vec;

    PCP_COPY_VECTOR(queue, source);
    PCP_COPY_VECTOR((u8 *)queue + 0x10, (u8 *)source + 0x10);
    queue->transformValue = source->transformValue;
    queue->transformWord = source->transformWord;
    for (src = source->first; src != NULL; src = src->next) {
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
    func_002D46F0(queue, &vec);
    func_002D4818(queue, &vec);
    func_002D48D0(queue, 1.0f);
    func_002D49B8(queue, 0x80808080);
    return queue;
}

void func_002D46A0(u8 *owner) {
    u8 *job = (u8 *)((FileQueue *)owner)->first;
    while (job != 0) {
        fileJobNotifyComplete(((FileJob *)job)->id);
        job = (u8 *)((FileJob *)job)->next;
    }
    ((FileQueue *)owner)->unk84 = 0;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D46F0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D4818);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D48D0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D49B8);

void fileReadVector40(void *work, void *dst) {
    PCP_COPY_VECTOR(dst, (u8 *)work + 0x40);
}

void func_002D4AB0(void *work, void *dst) {
    PCP_COPY_VECTOR(dst, (u8 *)work + 0x50);
}

f32 func_002D4AC8(s32 object) {
    return *(f32 *)(object + 0x60);
}

u32 func_002D4AD0(s32 arg0) {
    return *(u32 *)(arg0 + 100);
}

void func_002D4AD8(void *dst, void *src) {
    s128 vec;
    func_002D31C0(src);
    __asm__ volatile (
        ".set noreorder\n"
        "sqc2 vf10, 0(%0)\n"
        ".set reorder"
        : : "r"(&vec) : "memory");
    func_002D4818(dst, &vec);
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
    func_002D3F80(job);
    job->flags |= 1;
    job->id = src->id;
    fileQueueInsertAfter(queue, src, job);
    return job;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D4CF0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D4E60);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D4F10);

void fileJobCopyHeader(FileJob *dst, FileJob *src) {
    memcpy(dst, src, 0x90);
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D50D8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D55B0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D5AA8);

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

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D5DC8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D5EC0);

extern void *func_00328E18(s32);

typedef struct FileSlot {
    u8 pad0[0x10];
    u32 state;
    u8 pad14[0xC];
} FileSlot;

typedef struct FileSlotTable {
    u16 type;
    u8 pad02[2];
    u32 instances;
    u32 count;
    u32 flags;
    u32 references;
    u32 unk14;
    FileSlot *slots;
    u8 *unk1C;
    u8 *data0;
    u8 *data1;
    u32 handle;
} FileSlotTable;

/* The record's signed +0x54 mode is consumed by every billboard opener. */
typedef struct FileBillboardRecord {
    u8 pad0[0x54];
    s16 mode; /* 0x54 */
} FileBillboardRecord;

/* 0x54-byte loader record: same head as LoadObj (owner/colour/scale) but with a
   u16 tail at 0x50, so it is a separate type rather than DDS2's 0x50 LoadObj. */
typedef struct LoaderRecord {
    s32 owner;   /* 0x00 */
    u32 color;   /* 0x04 */
    f32 scale;   /* 0x08 */
    u8 pad0C[0x28];
    s32 unk34;   /* 0x34 */
    s32 unk38;   /* 0x38 */
    s32 unk3C;   /* 0x3C */
    s32 unk40;   /* 0x40 */
    s32 unk44;   /* 0x44 */
    u8 pad48[4];
    s32 unk4C;   /* 0x4C */
    u16 unk50;   /* 0x50 */
    u8 pad52[2];
} LoaderRecord;

void *func_002D5FB8(s32 owner) {
    LoaderRecord *rec = (LoaderRecord *)func_00328E18(0x54);
    u32 color = 0x80808080;

    rec->owner = owner;
    rec->unk50 = 1;
    rec->color = color;
    rec->scale = 1.0f;
    rec->unk34 = 0;
    rec->unk38 = 0;
    rec->unk3C = 0;
    rec->unk40 = 0;
    rec->unk44 = 0;
    rec->unk4C = 0;
    return rec;
}

/* DDS2 loader work has a longer prefix than the DDS1 LoadObj. */
typedef struct LoadObj {
    u8 pad00[8];
    f32 scale;             /* 0x08 */
    u8 pad0C[0x3C];
    void *referenceHolder; /* 0x48 */
    void *recordWork;      /* 0x4C */
} LoadObj;

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D6020);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042BB28);

EffectSurfaceNode *func_002D6058(FileJob *job) {
    void *primary = fileResolvePrimaryBuffer(job);
    EffectSurfaceNode *node = (EffectSurfaceNode *)func_002D6020(primary);
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
            fileReplaceReferenceHolder((LoadObj *)node, (u32)secondary);
            break;
        }
        node->kind = job->slots[0].selector;
    }
    return node;
}

void func_002D6160(EffectSurfaceNode *node) {
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
        func_003297C8(node->jobHandle);
    }
    if (node->queueHandle != 0) {
        count = ((FileSlotTable *)node->active)->count;
        for (i = 0; i < count; i++) {
            fileQueueDestroy(node->queues[i]);
        }
        func_003297C8(node->queueHandle);
    }
    if (node->referenceHolder != NULL) {
        effReleaseReferenceHolder((s32)node->referenceHolder);
    }
    if (node->active != 0) {
        func_002DC028(node->active);
    }
    func_00328E48(node);
}

LoadObj *fileLoadObjectCreateChild(LoadObj *owner) {
    LoadObj *source = *(LoadObj **)((u8 *)owner->recordWork + 0x24);
    LoadObj *result = func_002D6020(source);

    fileLoadObjectSetResource(result, *(u16 *)owner->recordWork, source);
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
        dst->resource = (void *)func_00159A50((u32)src->resource);
        func_00159FA0((u32)dst->resource);
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
            func_003297C8(dst->jobHandle);
            dst->jobs = 0;
            dst->jobHandle = 0;
        }
        size = count * 4;
        if (size == 0) {
            return;
        }
        dst->jobHandle = func_003292A8(size);
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
            func_003297C8(dst->queueHandle);
            dst->queues = 0;
            dst->queueHandle = 0;
        }
        size = count * 4;
        if (size == 0) {
            return;
        }
        dst->queueHandle = func_003292A8(size);
        dst->queues = sdfResourceRetainAddress(dst->queueHandle);
        for (i = 0; i < count; i++) {
            dst->queues[i] = fileQueueClone(src->queues[0]);
        }
        break;
    case 7:
        if (dst->referenceHolder != NULL) {
            effReleaseReferenceHolder((s32)dst->referenceHolder);
        }
        dst->referenceHolder = (void *)func_002DE120((u32)src->referenceHolder);
        break;
    }
    dst->kind = src->kind;
}

void fileLoadObjectSetResource(EffectSurfaceNode *node, u32 entryId, void *resource) {
    if (node->active != 0) {
        func_002DC028(node->active);
    }
    node->active = func_002DBED8((u16)entryId, node->percent, resource);
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
    func_00159FA0(resource);
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
        func_003297C8(node->jobHandle);
        node->jobs = 0;
        node->jobHandle = 0;
    }
    size = count * 4;
    if (size != 0) {
        node->jobHandle = func_003292A8(size);
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
        func_003297C8(node->queueHandle);
        node->queues = 0;
        node->queueHandle = 0;
    }
    size = count * 4;
    if (size != 0) {
        node->queueHandle = func_003292A8(size);
        node->queues = sdfResourceRetainAddress(node->queueHandle);
        node->queues[0] = (void *)func_002D4138(job);
        for (i = 1; i < count; i++) {
            node->queues[i] = fileQueueClone(node->queues[0]);
        }
    }
}

void fileReplaceReferenceHolder(LoadObj *obj, u32 resource) {
    u32 holder;

    if (obj->referenceHolder != NULL) {
        effReleaseReferenceHolder((s32)obj->referenceHolder);
    }
    holder = func_002DDF48(resource);
    obj->referenceHolder = (void *)holder;
}

void fileClearLoadObjectReferences(LoadObj *obj) {
    if (obj->recordWork != NULL) {
        fileClearRecordReferences((s32)obj->recordWork);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D6980);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D69B8);

void func_002D7398(u32 arg0) {
    func_002D6980();
    func_002D69B8(arg0);
}

void fileSendLoadObjectRecordVector(LoadObj *obj) {
    mnuRecordSetVector((u32)obj->recordWork);
}

void fileCopyLoadObjectRecordVector(LoadObj *obj) {
    func_002DC0F0((u32)obj->recordWork);
}

void fileSetRecordWordFour(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 4) = arg1;
}

void fileSetLoadObjectScale(LoadObj *arg0, f32 arg1) {
    arg0->scale = arg1;
    dds3DispatchIndexedCallback(arg0->recordWork);
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

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D7458);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D7770);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D78E8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D7A58);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D7AC8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D7B58);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D81B0);

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

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D8AD0);

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

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D9228);

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

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D9AF8);

void func_002DA2D8(ScaleOwner *owner, f32 scale) {
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

INCLUDE_ASM(const s32, "game/code_002C9660", func_002DA358);

void func_002DAA58(u8 *object, f32 factor) {
    f32 *source = *(f32 **)(object + 0x24);
    f32 *destination = *(f32 **)(object + 0x20);
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

INCLUDE_ASM(const s32, "game/code_002C9660", func_002DAAE0);

void func_002DB200(u8 *object, f32 factor) {
    f32 *source = *(f32 **)(object + 0x24);
    f32 *destination = *(f32 **)(object + 0x20);
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

void func_002DB288(u8 *node, const f32 *value) {
    if (*(u16 *)node == 7) {
        f32 *sprite = *(f32 **)(node + 0x20);
        sprite[0xFC / 4] = value[0];
        sprite[0x100 / 4] = value[1];
        sprite[0x104 / 4] = value[2];
    }
}

void func_002DB2C0(u8 *node, const f32 *value) {
    if (*(u16 *)node == 7) {
        f32 *sprite = *(f32 **)(node + 0x20);
        sprite[0x108 / 4] = value[0];
        sprite[0x10C / 4] = value[1];
        sprite[0x110 / 4] = value[2];
    }
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002DB2F8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002DB3E0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002DBE78);

u32 func_002DBED8(u16 type, u32 count, void *data) {
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
    handle = func_003292A8(size);
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
    rec->unk14 = 0;
    memcpy(rec->data0, src, dataBytes);
    memcpy(rec->data1, src, dataBytes);
    vec = rec->data0;
    __asm__ volatile(".set noreorder\n\tsqc2 $vf0, 0(%0)\n\t.set reorder" : : "r"(vec) : "memory");
    __asm__ volatile(".set noreorder\n\tsqc2 $vf0, 0(%0)\n\t.set reorder" : : "r"(vec + 0x10) : "memory");
    if (vec[0xBC] != 0) {
        rec->flags |= 1;
    }
    return (u32)rec;
}

void func_002DC028(s32 arg0) {
    func_003297C8(((FileSlotTable *)arg0)->handle);
}

void fileClearRecordReferences(s32 arg0) {
    ((FileSlotTable *)arg0)->references = 0;
}

void fileAcquireRecord(FileSlotTable *record) {
    if (record->references == 0) {
        fileResetSlotStates(record);
    }
    D_003E95C0[record->type].acquire(record);
    record->references++;
}

void fileReadVectorPtr20(u8 *obj, void *dst) {
    PCP_COPY_VECTOR(dst, *(u8 **)(obj + 0x20));
}

void mnuRecordSetVector(u8 *obj, void *src) {
    PCP_COPY_VECTOR(*(u8 **)(obj + 0x20), src);
}

void func_002DC0D8(u8 *obj, void *dst) {
    PCP_COPY_VECTOR(dst, *(u8 **)(obj + 0x20) + 0x10);
}

void func_002DC0F0(u8 *obj, void *src) {
    PCP_COPY_VECTOR(*(u8 **)(obj + 0x20) + 0x10, src);
}

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437CD0);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437CD4);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437CD5);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437CD8);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437CE0);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437CE4);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437CE8);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437CEC);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437CF0);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437CF4);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437CF8);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437CFC);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D00);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D04);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D08);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D0C);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D10);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D14);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D18);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D1C);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D20);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D24);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D28);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D2C);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D30);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D34);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D38);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D3C);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D40);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D44);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D48);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D4C);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D50);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D54);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D58);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D5C);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D60);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D64);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D68);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D6C);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D70);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D74);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D78);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D7C);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D80);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D84);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D88);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D8C);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D90);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D94);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437D98);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437DA0);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437DA8);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437DB0);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437DB8);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437DC0);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437DD0);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437DD8);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437DE0);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437DE5);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437DE8);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437DEC);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437DF0);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437DF8);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437E00);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437E08);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437E10);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437E18);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437E20);

INCLUDE_SDATA(const s32, "game/code_002C9660", D_00437E28);

