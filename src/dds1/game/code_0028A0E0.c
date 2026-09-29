#include "common.h"
#include "pcp_vu0.h"
#include "kwln.h"




extern s32 D_003BC87C;


extern s32 D_0037D488[];
extern s32 D_003BC868;
extern s32 D_003BC86C;
extern s32 D_003BC870;
extern s32 D_003BC874;
extern s32 D_003BC878;
extern s32 D_003BC87C;
extern s32 func_002D2D00();
extern s32 dds3GetWorldObject();
extern void func_00110860();
extern void fileWaitReady();
extern void sdfReleaseMemorySlot();







extern void *func_0028D978(void);
extern s32 mnuSelectFileBranch(void);






extern u8 D_003DC800[];

extern void func_00108A88(s32, s32, s32, s32, s32, s32, s32, s32);
extern s32 D_003BC880;

extern void *func_0028AFE0(void);
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
    u8 pad2[6];
    u32 count;
    u8 padC[4];
    u32 references;
    u8 pad14[4];
    FileRecordSlot *slots;
} FileRecordSlots;

extern u32 func_0029C230(u32);

extern void *fileDuplicateJob(void *);

extern u64 func_002D03F8(u64);

extern u64 sdfResourceRetainAddress(u64);

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

extern void *func_0028B748(void);

extern void (*D_0037E550[][4])(void *);

extern void fileResetSlotStates(FileRecordSlots *record);

extern void *func_00293D90(void *entry);

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

extern void *func_0028D748(void);

extern void *mcHandleLoadResult(void);

extern void *func_0028C8F8(void);

extern void *func_0028C828(void);

extern s32 D_003BD91C;

extern u32 D_003BD920;

extern s32 func_0028A088(void);

extern void func_0028A008(s32);

extern void *fileStoreSlotHeader(void);

extern s32 func_00289E38(void);

extern void *func_0028C710(void);

extern s32 D_003BAA00;

extern void *func_0028B2C0(void);

extern void *mcPrepareDirectory(void);

extern void *fileSlotSelectPollClear(void);

extern void *func_0028B280(void);

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
    u32 unk4;   /* 0x04 */
    u8 unk8[4]; /* 0x08 */
    u32 unkC;   /* 0x0C */
    u8 unk10;   /* 0x10 */
    u8 pad11[3]; /* 0x11 */
    u32 unk14;  /* 0x14 */
    u32 unk18;  /* 0x18 */
    u32 unk1C;  /* 0x1C */
} LoadCtx374A0;

extern LoadCtx374A0 D_0037D4A0;

/* Far scalar: incomplete array forces non-small-data addressing. */
extern u32 D_0037D4D0[];

extern u32 D_0037E130[];

extern char D_003B2668[];

extern char D_003B26C8[];


extern KwlnTask *kwlnTaskGetTaskByName(const char *name);

extern u32 func_00288B48(const char *arg0);

extern void dds3DispatchIndexedCallback(void *arg0);

extern void func_002966D8(s32 arg0);

extern void *fileResetSelection(void);

extern void *func_0028B238(void);

extern s32 fileSlotSelectPoll(void);

extern void func_0028C888(void);

extern void func_0028BE78(void);

extern s32 D_003BC808;

extern s32 fileReqPoll(void);

extern u8 fileReqGetStatus(s32 arg0);

extern void fileReqBegin(s32 arg0);

extern s32 func_0028B508(void);

extern void *func_0028B328(void);

extern s32 func_0028B370(void);

extern s32 func_0028B900(void);

extern s32 D_003BC844;

extern void fileReqSetSlotFlags(s32 arg0, s32 arg1, s32 arg2);

extern void fileReqClearSlotFlags(s32, s32);

extern void fileReqMarkSlotMetadataDirty(s32);

extern void *func_0028BF08(u32 arg0);

extern void *fileScanSlotStates(void);

extern void func_0028BF38(void);

extern s32 fileReqGetSize(s32 arg0);

extern u32 D_003BC804;

extern void func_001005B8(void);

extern void func_00289D50(u32 arg0);

extern void *mcHandleDetectionResult(void);

extern void func_00293EA0(void *arg0);

extern void func_00293158(void *src);

extern void func_00294798(void *dst, void *src);

extern void *func_002CFEB8(s32 size);

extern void *func_002CFF68(s32 size);

extern void func_002CFF98();

extern void fileClearRecordReferences(FileRecordSlots *record);

extern void menuRecordSetVector(void *arg0, const u128 *arg1);

extern void func_0029A7F8(void *arg0, const u128 *arg1);

extern void func_001028E8(s32 arg0, void *arg1, s32 arg2, s32 arg3);

extern s32 D_003BC848;

extern s32 D_003BC828;

extern char D_003B2658[];

extern void *fileBeginWait(void *arg0);

extern void mcFormatSaveFilename(void *arg0, s32 arg1);

extern void func_00289DA8(u32 arg0, void *arg1);

extern void *func_0028C180(void);

extern s32 mcPollSyncResult(void);

extern void func_00289E80(u32 arg0, const char *arg1, void *arg2, s32 arg3);

extern char D_003B2678[];

extern u8 D_003DC780[];

extern void *mcHandleSlotWriteResult(void);

extern void *func_0028B3F0(void);

extern void *fileBeginSlotOpen(void);

extern s32 func_00289EB0(void *arg0);

extern void *func_0028C560(void);

extern void *mcHandleDirectoryWriteResult(void);

extern void *func_0028C430(void);

extern u32 fileReqGetSlotFlags(s32 arg0, s32 arg1);

extern s32 func_0028B3B0(void);

extern u32 D_003BC834;

extern s32 func_0028FB48(void *arg0, void *arg1, s32 arg2);

extern void func_0028BAC0(void);
extern void func_0028DAF8(void);

extern void *func_0028C7C8(void);

extern void *mcChooseLoadPath(void);

extern void func_0028BDA0(void);

extern void *mcDispatchReadCallback(void);

extern void *mcHandleSearchResult(void);

extern s32 func_00289F30(void);

extern void *func_0028C9A0();

extern u8 fileReqIsSlotMetadataDirty(s32 arg0);

extern u32 D_003DC7C0[];

extern void billDispatchByKind(void *handle);

extern void *effRetainResource(void *name);

extern void *billCreateIndexed(s32 mode, void *name);

extern void func_001523B0(void *handle);

extern void effBillSetMode(void *handle, s16 index);

extern void *func_0029A5E0(u16 type, u32 owner, void *data);

/* Init record at D_0037D4E0. */
typedef struct Init374E0 {
    u8 unk0;        /* 0x00 */
    u8 unk1;        /* 0x01 */
    u8 unk2;        /* 0x02 */
    u8 pad3[0x31];  /* 0x03 */
    u32 unk34;      /* 0x34 */
    u32 unk38;      /* 0x38 */
    u32 unk3C;      /* 0x3C */
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
    void *(*create)(void *, u16);
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
    u8 unkC[0x28];     /* 0x0C */
    void *deviceHandle; /* 0x34 */
    u32 unk38;          /* 0x38 */
    u32 unk3C;          /* 0x3C */
    void *referenceHolder; /* 0x40: released by effReleaseReferenceHolder */
    void *recordWork;     /* 0x44: created by func_0029A5E0 */
    s16 unk48;          /* 0x48 */
    u16 unk4A;
} LoadObj;

extern LoadObj *func_00295F58(LoadObj *source);

extern void *handleSaveSetupDone(void);

extern s32 func_0028A020(void);

extern void func_00292720(void *);

extern void mnuCallInitWide(s32, s32, s32, u32, s32);

extern LoadObj *fileLoadObjectCreate(void *owner);

extern void func_002961B0(LoadObj *result, LoadObj *owner);

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

extern void func_0029A730(s32 arg0);

typedef struct FileQueue {
    u8 unk0[0x80];
    s32 count;
    u32 unk84;
    FileJob *head;
    FileJob *tail;
} FileQueue;

extern void fileQueueAppend(FileQueue *queue, FileJob *job);

extern s8 D_003BC8D5;

void func_0028A0E0(s32 request, u32 first, u32 second) {
    func_002F6670();
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028A0F8);

void func_0028A150(void) {
    if (D_003BC7F0 != 0) {
        kwlnTaskDestroyWithHierarchy(D_003BC7F0, 1);
        D_003BC7F0 = 0;
        D_003BC7EC = 0;
    }
}

s8 func_0028A180(void) {
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

u32 func_0028A1F8(void) {
    return 0x33600;
}

void fileReloadSaveBuffer(void) {
    s32 saved = *(s32 *)(D_003BAA00 + 0x30);
    s32 size = 0x33600;
    memcpy((void *)D_003BAA00, (void *)D_003BD924, size);
    *(s32 *)(D_003BAA00 + 0x30) = saved;
}

u8 func_0028A248(s32 arg0) {
    return arg0 != 0 && D_003BC7FC == 1;
}

u8 func_0028A268(s32 arg0) {
    return arg0 != 0 && D_003BC7FC == 1;
}

void func_0028A288(s32 x, s32 y, u32 first, u32 second) {
    D_003BD8EC = func_00197760(x << 4, y << 3, 0, first, second, 0);
    func_00195880(D_003BD8EC, 1);
    func_00194920(D_003BD8EC);
}

void func_0028A2D0(s32 arg0, s32 arg1, u32 arg2, u32 arg3) {
    func_00195520(1);
    D_003BD8F0 = func_001951C8(arg3, 0, 0, 0, 0);
    func_00195530(1);
    frFontSetFlagAndMeasureGlyphs(D_003BD8F0, 1);
    func_00195450(D_003BD8F0, arg0 << 4, arg1 << 3);
    frFontSetChildColors(D_003BD8F0, arg2);
    func_001958A0(D_003BD8F0, 0, 0x56);
    func_00194920(D_003BD8F0);
    func_00195548(0x54);
}

void func_0028A388(s32 arg0, s32 arg1, u64 arg2, u64 arg3) {
    u64 temp_v0;

    temp_v0 = func_001978E8(arg0 << 4, arg1 << 3, 0, arg2, arg3, 0);
    func_00195880(temp_v0, 1);
    func_00194920(temp_v0);
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028A3D8);

extern void func_00108A80(s32);
extern void func_00108CB8(s32);
extern void func_00108FA0(s32, s32, s32, s32, s32, s32, s32, s32, u32, u32, u32, u32, s32);
extern void func_002908D0(void);
extern void func_00290A88(s32, s32, s32);
extern s32 D_003BC85C;
extern s32 D_003BC884;

void fileDrawSaveWindow(void) {
    func_00108A80(0x56);
    func_00108CB8(0);
    func_00108FA0(0x112, 0x113, 0x98, 0x34, 0x14A, 0x1C5, 0x98, 0x34, 0x80808080, 0x80808080, 0x80808080, 0x80808080, D_003BC884);
    func_00108FA0(0x56, 0x113, 0xBC, 0x34, 0x14A, 0x1C5, 1, 0x34, 0x80808080, 0x80808080, 0x80808080, 0x80808080, D_003BC884);
    func_002908D0();
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

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028A7A8);

void func_0028AB30(u32 arg0) {
    s32 temp_v0;

    temp_v0 = D_003BC850;
    D_003BC850 = arg0;
    if (temp_v0 == 0) {
        D_003BC860 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028AB48);

void func_0028AE68(void) {
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
    return func_0028AFE0;
}

extern s32 func_00289FA8(s32 *);
extern void func_0028A070(s32, u32, s32);
extern void *func_0028B058(void);
INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028AFE0);

void *func_0028B058(void) {
    s32 status = func_0028A088();
    if (status == 0) {
        return NULL;
    }
    if (status == 1) {
        func_0028A008(D_003BD91C);
        return fileStoreSlotHeader;
    }
    func_002D0918(D_003BD920);
    return func_0028B3F0();
}

void *fileStoreSlotHeader(void) {
    s32 status = func_0028A020();

    if (status == 0) {
        return NULL;
    }
    if (status == 1) {
        memcpy(D_003DC800 + D_003BC828 * 0x30, (void *)D_003BD924, 0x30);
        func_002D0918(D_003BD920);
        return fileAdvanceSlotScan();
    }
    func_002D0918(D_003BD920);
    return func_0028B3F0();
}

void *fileBeginWait(void *callback) {
    kwlnFadeInStart(0, 0, 0, 15);
    D_003BD900 = callback;
    D_003BC800 = 20;
    return fileUpdateWait;
}

void *func_0028B238(void) {
    D_003BC848 = 0;
    func_0028AB30(0);
    D_003BC854 = 0;
    D_003BC834 = 4;
    D_003BC810 = 0;
    return (void *)func_0028FB48(&fileResetSelection, &func_0028B2C0, 0);
}

void *func_0028B280(void) {
    func_0028AB30(0);
    D_003BC854 = 0;
    D_003BC834 = 9;
    return (void *)func_0028FB48(&mcPrepareDirectory, &fileScanSlotStates, 1);
}

void *func_0028B2C0(void) {
    func_0028AB30(0);
    D_003BC854 = 0;
    D_003BC834 = 8;
    D_003BC810 = 0;
    return (void *)func_0028FB48(&func_0028B508, &func_0028B238, 1);
}

void *func_0028B300(void) {
    func_0028AB30(0);
    D_003BC810 = 0;
    return fileSlotSelectPoll;
}

void *func_0028B328(void) {
    fileReqBegin(0);
    D_003BC808 = 30;
    D_003BC804 = 1;
    func_0028AB30(1);
    D_003BC810 = 1;
    return fileSlotSelectPollClear;
}

s32 func_0028B370(void) {
    D_003BC80C = 1;
    func_0028AB30(0);
    D_003BC834 = 5;
    return func_0028FB48(&func_0028B508, &func_0028B328, 1);
}

s32 func_0028B3B0(void) {
    D_003BC80C = 1;
    func_0028AB30(0);
    D_003BC834 = 6;
    return func_0028FB48(&func_0028B508, &func_0028B328, 1);
}

void *func_0028B3F0(void) {
    func_0028AE68();
    func_0028AB30(0);
    D_003BC854 = 1;
    D_003BC858 = 0;
    fileReqBegin(D_003BC7E8);
    return func_0028BAC0;
}

void *fileResetSelection(void) {
    func_0028AE68();
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
    func_0028AB30(1);
    D_003BC810 = 1;
    return func_0028B748;
}

u32 func_0028B4B0(void) {
    func_0028AB30(0);
    D_003BC810 = 0;
    D_003BC80C = 0;
    return 0xffffffff;
}

u32 func_0028B4D8(void) {
    u32 temp_v0 [4];

    temp_v0[0] = 0;
    func_001028E8(2, temp_v0, 4, 0);
    return 0;
}

s32 func_0028B508(void) {
    return func_0028B4D8();
}

s32 func_0028B520(void) {
    u32 v = 2;

    D_003BC84C = 1;
    func_001028E8(2, &v, 4, 0);
    return 0;
}

void func_0028B560(void) {
    func_0028AE68();
    func_0028AB30(1);
    D_003BC854 = 0;
    D_003BC810 = 0;
    fileResetSelection();
}

extern s32 D_003BC82C;
extern void *func_0028DCC0(void);

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
    return func_0028DCC0;
}

void func_0028B658(void) {
    if (D_003BC848 == 1 && kwlnTaskGetTaskByName(D_003B2658) == NULL) {
        func_0028B520();
    } else {
        func_0028AB30(0);
        fileBeginWait(&func_0028B4B0);
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
    func_0028B238();
}

void func_0028B710(void) {
    fileReqBegin(0);
    D_003BC804 = 1;
    D_003BC808 = 0;
    func_0028AB30(0);
    D_003BC810 = 0;
    func_0028B300();
}

void *func_0028B748(void) {
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
        return func_0028B3F0();
    case 2:
        if (D_003BC848 == 0) {
            func_0028AE68();
            func_0028AB30(0);
            D_003BC854 = 0;
            D_003BC834 = 2;
            return (void *)func_0028FB48(func_0028D978, mnuSelectFileBranch, 1);
        }
        func_0028AB30(0);
        D_003BC854 = 7;
        D_003BC858 = 0;
        fileReqBegin(D_003BC7E8);
        return func_0028BAC0;
    case 3:
        func_0028AE68();
        func_0028AB30(0);
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

s32 func_0028B900(void) {
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
        return func_0028B370();
    case 2:
        return func_0028B508();
    case 1:
        if (func_0028B900() != 0) {
            return func_0028B508();
        }
        return (s32)func_0028B328();
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
        return (void *)func_0028B370();
    case 2:
        return (void *)func_0028B508();
    case 1:
        if (func_0028B900() != 0) {
            return (void *)func_0028B508();
        }
        return mcClearSlotMetadata();
    default:
        return NULL;
    }
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028BAC0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028BCD0);

extern s8 D_00324510[];

void func_0028BDA0(void) {
    if (func_0028A248(D_00324510[0x21] < 0) ||
        func_0028A248(D_00324510[0x23] < 0)) {
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

void *func_0028BE60(u32 arg0) {
    D_003BD904 = arg0;
    D_003BC858 = 0;
    return func_0028BE78;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028BE78);

void *func_0028BF08(u32 arg0) {
    D_003BD904 = arg0;
    D_003BC858 = 0;
    fileReqBegin(D_003BC7E8);
    return func_0028BF38;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028BF38);

void *func_0028C140(void) {
    u8 buf[0x50];

    buf[0] = 0x2F;
    mcFormatSaveFilename(&buf[1], D_003BC828);
    func_00289DA8(D_003BC7E8, buf);
    return func_0028C180;
}

void *func_0028C180(void) {
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
        return func_0028B3F0();
    }
    return fileBeginSlotOpen();
}

void *mcHandleSlotWriteResult(void) {
    s32 value;
    s32 status = func_00289EB0(&value);
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
        return func_0028B3F0();
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
                func_0028AB30(0);
                D_003BC854 = 7;
                D_003BC858 = 0;
                fileReqBegin(D_003BC7E8);
                return func_0028BAC0;
            }
            if (func_0028B900() == 0) {
                func_0028AB30(0);
                D_003BC810 = 0;
                D_003BC854 = 5;
                D_003BC858 = 0;
                fileReqBegin(D_003BC7E8);
                return func_0028BAC0;
            }
        }
        func_0028AB30(0);
        return fileScanSlotStates();
    }
    return func_0028C140();
}

void *mcResetSlotMetadata(void) {
    s32 index;
    if (fileReqIsSlotMetadataDirty(D_003BC7E8) != 0) {
        func_0028AB30(1);
        D_003BC828 = 0;
        index = 0;
        do {
            D_003DC7C0[index] = 0;
            fileReqClearSlotFlags(D_003BC7E8, index);
            index++;
        } while (index < 10);
        return func_0028C140();
    } else {
        D_003BC804 = 0;
        func_0028AB30(0);
        return fileScanSlotStates();
    }
}

void *func_0028C3F0(void) {
    u8 buf[0x50];

    buf[0] = 0x2F;
    mcFormatSaveFilename(&buf[1], D_003BC828);
    func_00289DA8(D_003BC7E8, buf);
    return func_0028C430;
}

void *func_0028C430(void) {
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
        func_0028AB30(0);
        D_003BC810 = 0;
        D_003BC854 = 0;
        return (void *)func_0028B370();
    }
    return func_0028C560();
}

void *mcHandleDirectoryWriteResult(void) {
    s32 value;
    s32 status = func_00289EB0(&value);
    if (status == 0) {
        return NULL;
    }
    if (status == 1) {
        if (value == 1) {
            fileReqSetSlotFlags(D_003BC7E8, D_003BC828, 9);
        }
        return func_0028C560();
    }
    if (status == -1) {
        func_0028AB30(0);
        D_003BC810 = 0;
        D_003BC854 = 0;
        return (void *)func_0028B370();
    }
    return func_0028C560();
}

void *func_0028C560(void) {
    s32 slot = D_003BC828;
    s32 buttons = fileReqGetSlotFlags(D_003BC7E8, slot);

    slot++;
    if (buttons & 1) {
        if (buttons & 2) {
            if (buttons & 8) {
                func_0028AB30(0);
                D_003BC810 = 0;
                D_003BC854 = 0;
                return (void *)func_0028B508();
            }
        }
    }
    D_003BC828 = slot;
    if (slot == 10) {
        func_0028AB30(0);
        D_003BC810 = 0;
        D_003BC854 = 0;
        return (void *)func_0028B3B0();
    }
    return func_0028C3F0();
}

void *mcClearSlotMetadata(void) {
    s32 index = 0;
    u32 *saved;
    func_0028AB30(1);
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
    func_0028AB30(2);
    fileReqMarkSlotMetadataDirty(entry);
    if (fileReqGetSlotFlags(entry, slot) & 2) {
        return func_0028C710();
    }
    name[0] = '/';
    mcFormatSaveFilename(name + 1, slot);
    mcMakeDirectory(entry, name);
    return mcHandleSearchResult;
}

void *func_0028C710(void) {
    u8 buf[0x50];
    u32 entry = D_003BC7E8;
    s32 v = fileReqGetSelectedSlot(entry);

    buf[0] = 0x2F;
    mcFormatSaveFilename(&buf[1], v);
    func_00289DA8(entry, buf);
    return func_0028C7C8;
}

void *mcHandleSearchResult(void) {
    s32 status = func_00289E38();
    if (status == 0) {
        return NULL;
    }
    if (status == 1) {
        fileReqSetSlotFlags(D_003BC7E8, D_003BC844, 2);
        return func_0028C710();
    }
    func_0028AB30(0);
    D_003BC854 = 4;
    return func_0028BDA0;
}

void *func_0028C7C8(void) {
    s32 t = mcPollSyncResult();

    if (t == 0) {
        return NULL;
    }
    if (t == 1) {
        return mcChooseLoadPath();
    }
    if (t == -1) {
        func_0028AB30(0);
        D_003BC854 = 4;
        return func_0028BDA0;
    }
    return NULL;
}

void *func_0028C828(void) {
    s32 t = func_00289F30();

    if (t == 0) {
        return NULL;
    }
    if (t == 1) {
        return func_0028C9A0();
    }
    if (t == -1) {
        func_0028AB30(0);
        D_003BC854 = 4;
        return func_0028BDA0;
    }
    return NULL;
}

void func_0028C888(void) {
    fileReqSetSlotFlags(D_003BC7E8, D_003BC844, 1);
    fileReqSetSlotFlags(D_003BC7E8, D_003BC844, 8);
    func_0028AB30(4);
    func_0028BF08((u32)fileScanSlotStates);
}

void *func_0028C8D0(void) {
    func_0028A150();
    return func_0028C888;
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
    return fileBeginRequest(D_003B2678, &D_003BC890, &D_003BC894, func_0028C8D0, 0);
}

void *fileRequestBaseIcon(void) {
    return fileBeginRequest(D_003B2688, &D_003BD914, &D_003BD918,
                          func_0028C8F8, &D_003BC7F8);
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028C9A0);

void *mcChooseLoadPath(void) {
    u32 entry = D_003BC7E8;
    s32 slot = fileReqGetSelectedSlot(entry);
    u32 flags = fileReqGetSlotFlags(entry, slot);
    if (!(flags & 8)) {
        return func_0028C9A0(entry, D_003B2678);
    }
    func_00289F10();
    return func_0028C828;
}

void *fileBeginRequest(const char *name, u32 *first, u32 *second, void *callback, u32 *status) {
    D_003BD928 = first;
    D_003BD92C = second;
    D_003BD930 = callback;
    D_003BD934 = status;
    func_00289F80(D_003BC7E8, name, 0x203);
    return func_0028D748;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028D748);

void *mcHandleLoadResult(void) {
    s32 status = func_0028A0F8();
    if (status == 0) {
        return NULL;
    }
    if (status == 1) {
        func_0028A008(D_003BD91C);
        return mcDispatchReadCallback;
    }
    if (status == -1) {
        func_0028AB30(0);
        D_003BC854 = 4;
        return func_0028BDA0;
    }
    return NULL;
}

void *mcDispatchReadCallback(void) {
    s32 status = func_0028A020();
    if (status == 0) {
        return NULL;
    }
    if (status == 1) {
        return ((void *(*)(void))D_003BD930)();
    }
    if (status == -1) {
        func_0028AB30(0);
        D_003BC854 = 4;
        return func_0028BDA0;
    }
    return NULL;
}

void *fileFinishRequest(void) {
    if (D_003BC7F8 != 0) {
        return NULL;
    }
    func_0028A0E0(D_003BD91C, *D_003BD928, *D_003BD92C);
    return mcHandleLoadResult;
}

void *mcHandleDetectionResult(void) {
    s32 status = func_00289D68();
    if (status == 0) {
        return NULL;
    }
    func_001005B0();
    if (status < 0) {
        func_0028AB30(0);
        D_003BC854 = 6;
        return func_0028BDA0;
    }
    func_0028AB30(6);
    return func_0028BF08((u32)func_0028B560);
}

void *func_0028D978(void) {
    func_0028AB30(5);
    func_001005B8();
    func_00289D50(D_003BC7E8);
    return mcHandleDetectionResult;
}

s32 mnuSelectFileBranch(void) {
    if (D_003BC824 != 0) {
        D_003BC834 = 7;
        return func_0028FB48(func_0028B2C0, fileResetSelection, 1);
    }
    D_003BC834 = 7;
    return func_0028FB48(func_0028B4B0, fileResetSelection, 1);
}

void *fileBeginSlotCreate(void) {
    char path[0x50];
    u32 ctx = D_003BC7E8;
    s32 slot = fileReqGetSelectedSlot(ctx);
    u32 attr = fileReqGetSlotFlags(ctx, slot);
    u32 len;

    func_0028AB30(3);
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
    return func_0028DAF8;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028DAF8);

void *mcHandleSetupResult(void) {
    s32 status = func_0028A088();
    if (status == 0) {
        return NULL;
    }
    if (status == 1) {
        func_0028A008(D_003BD91C);
        return handleSaveSetupDone;
    }
    func_002D0918(D_003BD920);
    func_0028AB30(0);
    D_003BC854 = 3;
    return func_0028BDA0;
}

extern s8 D_003BC7ED;
extern s32 fileLoadStateChanged(void);
extern void func_00290E38(void);
extern void func_00290E50(void);

void *handleSaveSetupDone(void) {
    s32 status = func_0028A020();
    if (status == 0) {
        return NULL;
    }
    if (status == 1) {
        fileReloadSaveBuffer();
        func_002D0918(D_003BD920);
        D_003BC7ED = 1;
        func_0028A150();
        if (fileLoadStateChanged() == 0) {
            func_00290E38();
        } else {
            func_00290E50();
        }
        func_0028AB30(13);
        return func_0028BE60(-1);
    }
    func_002D0918(D_003BD920);
    func_0028AB30(0);
    D_003BC854 = 3;
    return func_0028BDA0;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028DCC0);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B2658);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B2668);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B2678);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B2688);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028E180);

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

s32 fileDrawMenuFrame(s32 arg0) {
    func_00108A80(0x52);
    func_00108CB8(0);
    func_00108A88(1, 0, 0x80, 3, 0, 0, 1, 1);
    if (D_003BC80C != 0) {
        func_00108FA0(0, 0, 0x200, 0x1C0, 0, 0, 0x200, 0x1C0, 0x80808080, 0x80808080, 0x80808080, 0x80808080,
                      D_003BC880);
        func_00108FA0(0, 0, 0x200, 0x1C0, 0, 0, 0x200, 0x1C0, 0x80808080, 0x80808080, 0x80808080, 0x80808080,
                      D_003BC884);
    }
    if (D_003BC810 != 0) {
        func_0028E180(arg0);
    }
    func_0028AB48();
    func_0028FB98();
    func_0028A7A8();
    func_00108CB8(0);
    func_00108A88(1, 5, 0x80, 3, 0, 0, 1, 2);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028F0E0);

void func_0028F440(void) {
    func_0028F460();
}

void func_0028F458(void) {
}

void func_0028F460(void) {
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
                func_002D2D00(handle);
                *slot = 0;
            }
            slot++;
        } while (i >= 0);
        if (D_003BC884 != 0) {
            func_002D2D00(D_003BC884);
            D_003BC884 = 0;
        }
        if (D_003BC880 != 0) {
            func_002D2D00(D_003BC880);
            D_003BC880 = 0;
        }
        if (D_003BC87C != 0) {
            func_002D2D00(D_003BC87C);
            D_003BC87C = 0;
        }
        if (D_003BC878 != 0) {
            func_002D2D00(D_003BC878);
            D_003BC878 = 0;
        }
        if (D_003BC874 != 0) {
            func_002D2D00(D_003BC874);
            D_003BC874 = 0;
        }
        if (D_003BC870 != 0) {
            func_002D2D00(D_003BC870);
            D_003BC870 = 0;
        }
        if (D_003BC86C != 0) {
            func_002D2D00(D_003BC86C);
            D_003BC86C = 0;
        }
        if (D_003BC868 != 0) {
            func_002D2D00(D_003BC868);
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

s32 func_0028F600(void) {
    return kwlnTaskGetTaskByName(D_003B26C8) != NULL;
}

u32 func_0028F628(void) {
    return D_003BC84C;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028F630);

extern void *D_003BD908;
extern void *D_003BD90C;
extern s32 D_003BC830;
extern s32 D_003BC838;
extern void func_0028F630(void);
s32 func_0028FB48(void *start, void *finish, s32 mode) {
    D_003BD908 = start;
    D_003BD90C = finish;
    D_003BC830 = mode;
    D_003BC838 = 0;
    if (D_003BC834 != 4 && D_003BC834 != 8) {
        fileReqBegin(D_003BC7E8);
    }
    return (s32)func_0028F630;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0028FB98);

u32 func_00290478(void) {
    return D_003BC81C;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00290480);

void func_002904A8(u32 arg0) {
    D_0037D4D0[0] = arg0;
    D_0037D4A0.unkC = 0x80;
    D_0037D4A0.unk4 = 0;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002904C8);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00290520);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002905A8);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B26C8);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", jtbl_003B26E0);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B2718);

void func_002905E8(s32 index, s32 x, s32 y, s32 alpha) {
    s32 uv[21][2] = {
        {2, 2},   {2, 2},   {2, 20},  {2, 38},  {2, 56},  {2, 74},  {26, 2},
        {26, 20}, {26, 38}, {26, 56}, {26, 74}, {26, 74}, {26, 74}, {26, 74},
        {26, 74}, {26, 74}, {26, 74}, {26, 74}, {26, 74}, {26, 74}, {26, 74},
    };

    func_00108A80(0x53);
    func_00108CB8(0);
    func_00108A88(1, 0, 0x80, 3, 0, 0, 1, 1);
    func_00108FA0(x, y, 0x16, 0x10, uv[index][0], uv[index][1], 0x16, 0x10, (alpha << 24) | 0x808080,
                  (alpha << 24) | 0x808080, (alpha << 24) | 0x808080, (alpha << 24) | 0x808080, D_003BC87C);
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00290788);

void func_00290898(void) {
    D_0037D4E0.unk1 = 0;
    D_0037D4AC[0] = 0;
    D_0037D4E0.unk0 = 1;
    D_0037D4E0.unk2 = 2;
    D_0037D4E0.unk34 = 0x80;
    D_0037D4E0.unk38 = 0x80;
    D_0037D4E0.unk3C = 0x80;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002908D0);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B2810);

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

void func_00290C98(void) {
    D_0037D4A0.unk14 = 0;
    D_0037D4A0.unk10 = 0;
    D_0037D4A0.unk4 = 0;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00290CB0);

s32 fileLoadStateChanged(void) {
    return D_003BC8D8.current != D_003BC8D8.previous;
}

void func_00290E38(void) {
    D_003BC8D8.current = D_003BC8D8.previous =
        *(u32 *)(D_003BAA00 + 0xA54);
}

void func_00290E50(void) {
    *(u32 *)(D_003BAA00 + 0xa54) = D_003BC8DC;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00290E60);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B28A0);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B28C0);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B28D0);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B28E8);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00290F40);

void func_00290FC0(u32 arg0) {
    func_00290F40(arg0, D_003BAA00 + 0xa54);
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
extern u32 func_002918D8(void);
extern void *kwlnTaskCreate(const char *name, s32 id, s32 arg2, s32 arg3, void *update, void *destroy, s32 data);

void configTasksCreate(void) {
    if (D_003BD938 == 0) {
        D_003BD938 = func_00290FE0();
        kwlnTaskCreate(D_003BC8E8, 0x3F2, 1, 1, func_00291418, NULL, D_003BD938);
        kwlnTaskCreate(D_003B29D8, 0x2B07, 1, 1, fileStartQueuedLoad, NULL, D_003BD938);
        kwlnTaskCreate(D_003B29E8, 0x520B, 1, 1, func_002918D8, func_002911B8, D_003BD938);
        D_003BC8D5 = 1;
    }
}

void mnuConfigTasksDestroy(void) {
    kwlnTaskDestroyWithHierarchyByName(D_003BC8E8, 1);
    kwlnTaskDestroyWithHierarchyByName(D_003B29D8, 1);
    kwlnTaskDestroyWithHierarchyByName(D_003B29E8, 1);
}

s32 func_002913B8(void) {
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

u32 func_002913F0(s32 arg0) {
    if (arg0 < 4) {
        return *(u32 *)((s32)arg0 * 4 + D_003BD938 + 0x10);
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B29D8);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B29E8);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00291418);

s32 fileStartQueuedLoad(void) {
    if (*(u32 *)(D_003BD938 + 0x34) == 0) {
        return 0;
    }
    if (*(s32 *)(D_003BD938 + 0x24) < 0) {
        return -1;
    }
    func_00292720((void *)D_003BD938);
    mnuCallInitWide(0x400, 0x400, 0, *(u32 *)(D_003BD938 + 0xC), 0x53);
    return 0;
}

u32 func_002918D8(void) {
    u32 temp_v0;

    temp_v0 = 0xffffffff;
    if ((*(u32 *)(D_003BD938 + 0x24) & 0x80000000) == 0) {
        temp_v0 = 0;
    }
    return temp_v0;
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

void func_00292C18(u32 arg0) {
    D_003BC8F8 = D_003BC8F8 | arg0;
}

void func_00292C28(u32 arg0) {
    D_003BC8F8 = D_003BC8F8 & ~arg0;
}

void func_00292C40(void) {
    D_003BC8F8 = 0;
}

u32 func_00292C48(s32 arg0) {
    return D_0037E130[arg0];
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

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00292F48);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002930C0);

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

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00293450);

void fileJobDestroy(FileJob *job) {
    void *data = *(void **)((u8 *)job + 8);
    if (data != NULL) {
        u16 index = *(u16 *)((u8 *)job + 4);
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

void fileJobNotifyComplete(void *arg0) {
    u16 idx = *(u16 *)((u8 *)arg0 + 4);
    void (*cb)(void *) = D_0037E14C[idx].cb10;
    if (cb != NULL) {
        cb(*(void **)((u8 *)arg0 + 8));
    }
}

void func_002936A8(void *arg0) {
    u16 idx = *(u16 *)((u8 *)arg0 + 4);
    void *data = *(void **)((u8 *)arg0 + 8);

    D_0037E14C[idx].cb(data);
}

void func_002936E0(void *arg0) {
    u16 idx = *(u16 *)((u8 *)arg0 + 4);
    void (*cb)(void *) = D_0037E14C[idx].cb14;
    if (cb != NULL) {
        cb(*(void **)((u8 *)arg0 + 8));
    }
}

void func_00293720(void *arg0, void *extra) {
    u16 idx = *(u16 *)((u8 *)arg0 + 4);
    void (*cb)(void *, void *) = D_0037E14C[idx].cb18;
    if (cb != NULL) {
        cb(*(void **)((u8 *)arg0 + 8), extra);
    }
}

void func_00293760(void *arg0) {
    u16 idx = *(u16 *)((u8 *)arg0 + 4);
    void (*cb)(void *) = D_0037E14C[idx].cb1C;
    if (cb != NULL) {
        cb(*(void **)((u8 *)arg0 + 8));
    }
}

void func_002937A0(void *arg0) {
    u16 idx = *(u16 *)((u8 *)arg0 + 4);
    void (*cb)(void *) = D_0037E14C[idx].cb20;
    if (cb != NULL) {
        cb(*(void **)((u8 *)arg0 + 8));
    }
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002937E0);

void func_00293880(u64 arg0, u64 arg1, u16 arg2) {
    s64 temp_v0;
    u64 temp_v1;
    u64 temp_v2;
    u64 temp_v3;

    temp_v0 = sdfDevCreateCommandState(arg1);
    if (temp_v0 != 0) {
        temp_v1 = func_002E5C88(temp_v0);
        temp_v2 = func_002D03F8(temp_v1);
        temp_v3 = sdfResourceRetainAddress(temp_v2);
        func_002E5C68(temp_v0, temp_v3, temp_v1);
        func_002E5C38(temp_v0);
        func_002937E0(arg0, temp_v3, temp_v1, arg2);
        func_002D0918(temp_v2);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00293960);

void func_00293A00(u64 arg0, u64 arg1, u16 arg2) {
    s64 temp_v0;
    u64 temp_v1;
    u64 temp_v2;
    u64 temp_v3;

    temp_v0 = sdfDevCreateCommandState(arg1);
    if (temp_v0 != 0) {
        temp_v1 = func_002E5C88(temp_v0);
        temp_v2 = func_002D03F8(temp_v1);
        temp_v3 = sdfResourceRetainAddress(temp_v2);
        func_002E5C68(temp_v0, temp_v3, temp_v1);
        func_002E5C38(temp_v0);
        func_00293960(arg0, temp_v3, temp_v1, arg2);
        func_002D0918(temp_v2);
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
extern void func_00293AE0(s32 fd, s32 arg1);
extern void func_0030EB78(s32 fd);
extern void func_00310A68(const char *path, s32 arg1);

void fileWriteToPfs(s32 arg0, s32 arg1) {
    char path[0xD0];
    s32 fd;

    if (D_003BD476 != 0) {
        func_003014F0(path, D_003BC928, arg1);
        fd = func_0030E8F0(path, 0x602, 0x1B6);
    } else {
        func_003014F0(path, D_003BC930, func_002E5D70(), arg1);
        fd = func_0030E8F0(path, 0x602);
    }
    func_00293AE0(fd, arg0);
    func_0030EB78(fd);
    func_00310A68(D_003BC938, 0);
}

void *fileDuplicateJob(void *source) {
    FileJob *request = source;
    FileJob *job = fileCreateJob(request->type);
    if (request->slots[0].size != 0) {
        func_002937E0(job, fileResolvePrimaryBuffer(request), request->slots[0].size, request->option);
    }
    if (request->slots[1].size != 0) {
        func_00293960(job, fileResolveSecondaryBuffer(request), request->slots[1].size, request->slots[0].selector);
    }
    return job;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00293D90);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00293E30);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00293EA0);

void func_00293F18(void *arg0) {
    memset(arg0, 0, 0x90);
    *(u32 *)((u8 *)arg0 + 0x84) = 1;
    *(u8 *)((u8 *)arg0 + 0x88) = 8;
    *(u8 *)((u8 *)arg0 + 0x89) = 0;
    *(u8 *)((u8 *)arg0 + 0x8A) = 0;
    func_00293EA0(arg0);
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

void func_00294318(u32 arg0, u32 arg1) {
    func_002940D0(arg1);
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
    return fileAppendJob(queue, (u32)func_00293D90(entry));
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

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00294C30);

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

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00295D08);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00295E00);

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

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00295F58);

INCLUDE_RODATA(const s32, "game/code_0028A0E0", D_003B2A18);

LoadObj *loadObjectCreateFromJob(FileJob *job) {
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
            func_00296530(obj, secondary);
            break;
        case 7:
            fileReplaceReferenceHolder((s32)obj, (u32)secondary);
            break;
        }
        *(u32 *)((u8 *)obj + 0xC) = job->slots[0].selector;
    }
    return obj;
}

void loadObjectDestroy(LoadObj *obj) {
    if (obj->deviceHandle != NULL) {
        billDispatchByKind(obj->deviceHandle);
    }
    if (obj->unk3C != 0) {
        u32 count = *(u32 *)((u8 *)obj->recordWork + 8);
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
    func_002961B0(result, owner);
    return result;
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002961B0);

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
        void *record = *(void **)((u8 *)obj->recordWork + 0x20);
        effBillSetMode(handle, *(s16 *)((u8 *)record + 0x54));
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
        void *record = *(void **)((u8 *)obj->recordWork + 0x20);
        effBillSetMode(handle, *(s16 *)((u8 *)record + 0x54));
    }
}

void fileLoadObjectOpenAndStartDevice(LoadObj *obj, void *name) {
    if (obj->deviceHandle != NULL) {
        billDispatchByKind(obj->deviceHandle);
    }
    obj->deviceHandle = billCreateIndexed(1, name);
    func_001523B0(obj->deviceHandle);
    if (obj->recordWork != NULL) {
        void *record = *(void **)((u8 *)obj->recordWork + 0x20);
        effBillSetMode(obj->deviceHandle, *(s16 *)((u8 *)record + 0x54));
    }
}

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00296530);

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

void func_00296E98(s32 arg0) {
    fileAcquireLoadObjectRecord(arg0);
    func_002966D8(arg0);
}

void func_00296EC0(LoadObj *arg0, u128 *arg1) {
    menuRecordSetVector(arg0->recordWork, arg1);
}

void func_00296ED8(LoadObj *arg0, u128 *arg1) {
    func_0029A7F8(arg0->recordWork, arg1);
}

void func_00296EF0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 4) = arg1;
}

void fileSetLoadObjectScale(LoadObj *arg0, f32 arg1) {
    arg0->scale = arg1;
    dds3DispatchIndexedCallback(arg0->recordWork);
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

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00297558);
INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002975C8);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00297658);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00297CB0);

INCLUDE_ASM(const s32, "game/code_0028A0E0", effScaleParameterSet);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_002985D0);

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

void loadObjScaleParamsA(ScaleOwner *owner, f32 scale) {
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

void loadObjScaleParamsC(ScaleOwner *owner, f32 scale) {
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

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00299DD8);

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_00299E58);

void loadObjScaleParamsB(ScaleOwner *owner, f32 scale) {
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

INCLUDE_ASM(const s32, "game/code_0028A0E0", func_0029A5E0);

void func_0029A730(s32 arg0) {
    func_002D0918(*(u32 *)(arg0 + 0x28));
}

void fileClearRecordReferences(FileRecordSlots *record) {
    record->references = 0;
}

void fileAcquireRecord(FileRecordSlots *record) {
    if (record->references == 0) {
        fileResetSlotStates(record);
    }
    D_0037E550[record->type][0](record);
    record->references++;
}

void menuRecordGetVector(void *record, u128 *out) {
    PCP_COPY_VECTOR(out, *(u128 **)((u8 *)record + 0x20));
}

void menuRecordSetVector(void *record, const u128 *value) {
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

