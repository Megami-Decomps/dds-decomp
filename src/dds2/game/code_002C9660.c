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
    u8 pad_38[0x14];
    u32 active;
    u16 count;
} EffectSurfaceNode;

extern u32 effRetainResource(u32);
extern void func_00159C40(u32, s16);
extern u32 billCreateIndexed(u32, u32);
extern void func_00159FA0(u32);
extern u32 D_00439044;
extern u32 func_002DBED8(u16, u32, void *);
#include "kwln.h"

extern u64 fileDuplicateJob(u64);

extern u64 func_003292A8(u64);

extern u64 sdfResourceRetainAddress(u64);

extern s64 sdfDevCreateCommandState(u64);

extern u64 func_0033EB30(s64);

extern s32 D_00437D84;

extern u32 D_00437CE8;

extern u32 D_00437CEC;

extern u32 D_00437CF0;

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

extern s32 D_00437D38;

extern u32 D_00437D48;

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
extern void func_0035C860(void *buffer, const char *format, s32 titleId, s32 slot);

extern s32 fileSlotSelectPoll(void);

extern void fileReqBegin(s32 arg0);

extern void *func_002CABC0(void);

extern void func_002CB2C0(void);

extern s32 func_002CB100(void);

extern s32 fileReqGetSize(s32 arg0);

extern u32 D_00439020;

extern void mcdFinishFileDetection(void);

extern void *func_002CB710(u32 arg0);

extern void func_002CB740(void);

extern s32 D_00437D10;

extern void mcdFormatSaveSlotName(void *arg0, s32 arg1);

extern void func_002C9328(u32 arg0, void *arg1);

extern void *func_002CB988(void);

extern void *func_002CBCA0(void);

extern void *func_002CC038(void);

extern void func_002CC0F8(void);

extern void func_001004A0(void);
extern s8 D_0037F510[];
extern void sndSetSequenceVolumePan(s32 sequence, s32 volume, s32 pan);
extern void func_00342580(s32 command);
extern void fileBeginWait(s32 result);

extern void func_002C92D0(u32 arg0);

extern void mcHandleDetectionResult(void);

extern char D_0042B720[];

extern KwlnTask *func_00101740(const char *name);

/* Loader context at D_0037D4A0. */
typedef struct LoadCtx374A0 {
    u8 unk0[4]; /* 0x00 */
    u32 unk4;   /* 0x04 */
    u8 unk8[4]; /* 0x08 */
    u32 unkC;   /* 0x0C */
    u8 unk10;   /* 0x10 */
    u8 pad11[3]; /* 0x11 */
    u32 unk14;  /* 0x14 */
} LoadCtx374A0;

extern LoadCtx374A0 D_003E7FD8;

/* Far scalar: incomplete array forces non-small-data addressing. */
extern u32 D_003E8008[];

extern u32 D_003E9150[];

/* Callback table at D_0037E14C (0x28 bytes per entry). */
typedef struct Cb3714C {
    void (*cb)(void *arg); /* 0x00 */
    u8 pad4[0x24];         /* 0x04 */
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

extern void func_001089A0(s32);

extern void func_00108BD8(s32);

extern void func_00108EC0(s32, s32, s32, s32, s32, s32, s32, s32, u32, u32, u32, u32, s32);

extern void func_002CFD80(void);

extern void func_002CFF38(s32, s32, s32);

extern s32 D_00437D78;

extern void *func_002CA7B0(void);

extern void *func_002CBA90(void);

extern void func_002C92A8(u32 ctx, s32 arg);

extern void func_002C9500(u32, const char *, s32);

extern void *fileBeginSlotOpen(void);

extern u32 func_002C9250(s32 arg0, s32 arg1);

extern void *fileSlotSelectPollClear(void);

extern void *func_002CAAF8(void);

extern void *func_002CAF48(void);

extern void *fileResetSelection(void);

extern u32 D_00458080[];

extern void *D_0043901C;

extern void *fileUpdateWait(void);

extern void func_002C9218(s32 arg0, s32 arg1, s32 arg2);

extern void *mcHandleSlotWriteResult(void);

extern s32 func_002C9430(void *arg0);

extern s32 func_002CAB40(void);

extern void *func_002CBDD0(void);

extern void *mcHandleDirectoryWriteResult(void);

extern void *mcPrepareDirectory(void);

extern void func_002C91B8(s32);

extern void *mcHandleSearchResult(void);

extern void *fileScanSlotStates(void);

extern void func_002CC8E0(void);

extern s32 func_002C8128(u32, void *);

extern u32 func_002C8108(u32);

extern u32 func_002C8110(u32);

extern u32 func_002C8118(u32);

extern void func_002C7D00(u32);

extern s32 func_002CDFD8(s32);

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

extern void func_002D0EB0(void);

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

typedef struct FileTypeCallbacks {
    void *(*create)(void *, u16);
    void (*unk4)(void *);
    void (*destroy)(void *);
    void *(*createChild)(void *, u16);
    u8 unk10[0x18];
} FileTypeCallbacks;

extern FileTypeCallbacks D_003E9168[];

typedef struct FileQueue {
    u8 unk0[0x80];
    s32 count;
    u32 unk84;
    FileJob *head;
    FileJob *tail;
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

extern void *func_002CAA90(void);

extern void *func_002CAA08(void);

extern s32 D_00437D30;

extern s32 func_002CEE70(void *arg0, void *arg1, s32 arg2);

extern void *func_002CAA50(void);

extern char D_0042B698[];

extern void func_002CB5A0(void);

extern s32 func_002C9348(void);

extern void func_002C9400(u32 arg0, const char *arg1, void *arg2, s32 arg3);

extern char D_0042B6B8[];

extern u8 D_00458040[];

extern void func_002C91E8(s32, s32);

extern u8 func_002C9168(s32 arg0);

extern void *mcClearSlotMetadata(void);

extern void *mcChooseLoadPath(void);

extern s32 D_00437D2C;

extern void *func_002CC098(void);

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

void func_002C9660(void) {
    func_0034F490();
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002C9678);

void func_002C96D0(void) {
    if (D_00437CD8 != 0) {
        kwlnTaskDestroyWithHierarchy(D_00437CD8, 1);
        D_00437CD8 = 0;
        D_00437CD4 = 0;
    }
}

s8 func_002C9700(void) {
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
    s32 slot = func_002C9280(D_00437CD0);
    D_00437D50 = D_004580C3[slot * 0x30];
}

u32 func_002C9788(void) {
    return 0x1e840;
}

void fileReloadSaveBuffer(void) {
    s32 saved = *(s32 *)(D_00435DD0 + 0x30);
    s32 size = 0x1E840;
    memcpy((void *)D_00435DD0, (void *)D_00439044, size);
    *(s32 *)(D_00435DD0 + 0x30) = saved;
}

u8 func_002C97D8(s32 arg0) {
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
    func_0019D120(handle, 3);
    func_0019D530(D_00439008, 1);
    func_0019C5B0(D_00439008);
}

void func_002C98B8(s32 arg0, s32 arg1, u32 arg2, u32 arg3) {
    func_0019D1D0(1);
    D_0043900C = func_0019CE78(arg3, 0, 0, 0, 0);
    func_0019D1E0(1);
    func_0019D058(D_0043900C, 1);
    func_0019D100(D_0043900C, arg0 << 4, arg1 << 3);
    func_0019D178(D_0043900C, arg2);
    func_0019D550(D_0043900C, 0, 0x56);
    func_0019C5B0(D_0043900C);
    func_0019D1F8(0x54);
}

void func_002C9970(s32 x, s32 y, u64 width, u64 height) {
    u64 handle;

    handle = func_0019F5E8(x << 4, y << 3, 0, width, height, 0);
    func_0019D530(handle, 1);
    func_0019C5B0(handle);
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002C99C0);

void fileDrawSaveWindow(void) {
    func_001089A0(0x56);
    func_00108BD8(0);
    func_00108EC0(0x112, 0x113, 0x98, 0x34, 0x14A, 0x1C5, 0x98, 0x34, 0x80808080, 0x80808080, 0x80808080, 0x80808080, D_00437D78);
    func_00108EC0(0x56, 0x113, 0xBC, 0x34, 0x14A, 0x1C5, 1, 0x34, 0x80808080, 0x80808080, 0x80808080, 0x80808080, D_00437D78);
    func_002CFD80();
    func_002CFF38(0x17E, 0x118, 0x56);
    D_00437D44++;
}

INCLUDE_ASM(const s32, "game/code_002C9660", fileIsLoadStepComplete);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002C9BD0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002C9CF8);

void func_002CA1D8(u32 value) {
    s32 previous;

    previous = D_00437D38;
    D_00437D38 = value;
    if (previous == 0) {
        D_00437D48 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CA1F0);

void func_002CA638(void) {
    func_002C91B8(D_00437CD0);
    func_002C91E8(D_00437CD0, 0);
    func_002C91E8(D_00437CD0, 1);
    func_002C91E8(D_00437CD0, 2);
    func_002C91E8(D_00437CD0, 3);
    func_002C91E8(D_00437CD0, 4);
    func_002C91E8(D_00437CD0, 5);
    func_002C91E8(D_00437CD0, 6);
    func_002C91E8(D_00437CD0, 7);
    func_002C91E8(D_00437CD0, 8);
    func_002C91E8(D_00437CD0, 9);
}

void *fileBeginSlotOpen(void) {
    char path[0x50];
    u32 ctx;
    s32 slot;
    s32 len;

    func_002C92A8(D_00437CD0, D_00437D10);
    ctx = D_00437CD0;
    slot = func_002C9280(ctx);
    if ((func_002C9250(ctx, slot) & 1) == 0) {
        return func_002CBA90();
    }
    path[0] = 0x2F;
    mcdFormatSaveSlotName(&path[1], slot);
    len = strlen(&path[1]);
    path[len + 1] = 0x2F;
    memcpy(&path[len + 2], &path[1], len);
    path[len * 2 + 2] = 0;
    func_002C9500(ctx, path, 1);
    return func_002CA7B0;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CA7B0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CA828);

INCLUDE_ASM(const s32, "game/code_002C9660", fileStoreSlotHeader);

INCLUDE_ASM(const s32, "game/code_002C9660", fileBeginWait);

void *func_002CAA08(void) {
    D_00437D30 = 0;
    func_002CA1D8(0);
    D_00437D3C = 0;
    D_00437D1C = 4;
    D_00437CF8 = 0;
    return (void *)func_002CEE70(&fileResetSelection, &func_002CAA90, 0);
}

void *func_002CAA50(void) {
    func_002CA1D8(0);
    D_00437D3C = 0;
    D_00437D1C = 9;
    return (void *)func_002CEE70(&mcPrepareDirectory, &fileScanSlotStates, 1);
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CAA90);

void *func_002CAAD0(void) {
    func_002CA1D8(0);
    D_00437CF8 = 0;
    return fileSlotSelectPoll;
}

void *func_002CAAF8(void) {
    fileReqBegin(0);
    D_00437CF0 = 30;
    D_00437CEC = 1;
    func_002CA1D8(1);
    D_00437CF8 = 1;
    return fileSlotSelectPollClear;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CAB40);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CAB80);

void *func_002CABC0(void) {
    func_002CA638();
    func_002CA1D8(0);
    D_00437D3C = 1;
    D_00437D40 = 0;
    fileReqBegin(D_00437CD0);
    return func_002CB2C0;
}

void *fileResetSelection(void) {
    func_002CA638();
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
    func_002CA1D8(1);
    D_00437CF8 = 1;
    return func_002CAF48;
}
extern void func_001027D8(s32, const s32 *, s32, s32);
extern void func_002D09C8(void);


u32 func_002CAC80(void) {
    func_002CA1D8(0);
    D_00437CF8 = 0;
    D_00437CF4 = 0;
    return 0xffffffff;
}

void *mcdEnterDefaultFileFlow(void) {
    s32 mode = 0;
    if (D_00437D7C != 0) {
        return func_002D09C8;
    }
    func_001027D8(2, &mode, 4, 0);
    return 0;
}

void func_002CACF0(void) {
    mcdEnterDefaultFileFlow();
}

void *mcdEnterSelectedFileFlow(void) {
    s32 mode = 2;
    D_00437D34 = 1;
    if (D_00437D7C != 0) {
        return func_002D09C8;
    }
    func_001027D8(2, &mode, 4, 0);
    return 0;
}

void func_002CAD60(void) {
    func_002CA638();
    func_002CA1D8(1);
    D_00437D3C = 0;
    D_00437CF8 = 0;
    fileResetSelection();
}

INCLUDE_ASM(const s32, "game/code_002C9660", fileScanSlotStates);

void func_002CAE58(void) {
    if (D_00437D30 == 1 && func_00101740(D_0042B698) == NULL) {
        mcdEnterSelectedFileFlow();
    } else {
        func_002CA1D8(0);
        fileBeginWait(&func_002CAC80);
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
    func_002CAA08();
}

void func_002CAF10(void) {
    fileReqBegin(0);
    D_00437CEC = 1;
    D_00437CF0 = 0;
    func_002CA1D8(0);
    D_00437CF8 = 0;
    func_002CAAD0();
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CAF48);

s32 fileCountSelectableFiles(void) {
    s32 count = 0;
    s32 index;
    for (index = 0; index < 10; index++) {
        u32 flags = func_002C9250(D_00437CD0, index);
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

s32 func_002CB100(void) {
    return fileReqGetSize(D_00437CD0) > 0x2A7FF;
}

INCLUDE_ASM(const s32, "game/code_002C9660", fileSlotSelectPoll);

INCLUDE_ASM(const s32, "game/code_002C9660", fileSlotSelectPollClear);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CB2C0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CB4D0);

void func_002CB5A0(void) {
    if (func_002C97D8(D_0037F510[0x21] < 0) ||
        func_002C97D8(D_0037F510[0x23] < 0)) {
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
    if (func_002C97D8((u32)D_0037F510[0x21] >> 31) ||
        func_002C97D8((u32)D_0037F510[0x23] >> 31)) {
        func_002CA1D8(0);
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

void *func_002CB710(u32 arg0) {
    D_00439020 = arg0;
    D_00437D40 = 0;
    fileReqBegin(D_00437CD0);
    return func_002CB740;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CB740);

void *func_002CB948(void) {
    u8 buf[0x50];

    buf[0] = 0x2F;
    mcdFormatSaveSlotName(&buf[1], D_00437D10);
    func_002C9328(D_00437CD0, buf);
    return func_002CB988;
}

void *func_002CB988(void) {
    s32 t = func_002C9348();

    if (t == 0) {
        return NULL;
    }
    if (t == 1) {
        func_002C9218(D_00437CD0, D_00437D10, 2);
        func_002C9400(D_00437CD0, D_0042B6B8, D_00458040, 1);
        return mcHandleSlotWriteResult;
    }
    if (t == -1) {
        return func_002CABC0();
    }
    return fileBeginSlotOpen();
}

void *mcHandleSlotWriteResult(void) {
    s32 value;
    s32 status = func_002C9430(&value);
    if (status == 0) {
        return NULL;
    }
    if (status == 1) {
        if (value == 1) {
            func_002C9218(D_00437CD0, D_00437D10, 9);
        }
        return fileBeginSlotOpen();
    }
    if (status == -1) {
        return func_002CABC0();
    }
    return fileBeginSlotOpen();
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CBA90);

void *mcResetSlotMetadata(void) {
    s32 index;
    if (func_002C9168(D_00437CD0) != 0) {
        func_002CA1D8(1);
        D_00437D10 = 0;
        index = 0;
        do {
            D_00458080[index] = 0;
            func_002C91E8(D_00437CD0, index);
            index++;
        } while (index < 10);
        return func_002CB948();
    } else {
        D_00437CEC = 0;
        func_002CA1D8(0);
        return fileScanSlotStates();
    }
}

void *func_002CBC60(void) {
    u8 buf[0x50];

    buf[0] = 0x2F;
    mcdFormatSaveSlotName(&buf[1], D_00437D10);
    func_002C9328(D_00437CD0, buf);
    return func_002CBCA0;
}

void *func_002CBCA0(void) {
    s32 t = func_002C9348();

    if (t == 0) {
        return NULL;
    }
    if (t == 1) {
        func_002C9218(D_00437CD0, D_00437D10, 2);
        func_002C9400(D_00437CD0, D_0042B6B8, D_00458040, 1);
        return mcHandleDirectoryWriteResult;
    }
    if (t == -1) {
        func_002CA1D8(0);
        D_00437CF8 = 0;
        D_00437D3C = 0;
        return (void *)func_002CAB40();
    }
    return func_002CBDD0();
}

void *mcHandleDirectoryWriteResult(void) {
    s32 value;
    s32 status = func_002C9430(&value);
    if (status == 0) {
        return NULL;
    }
    if (status == 1) {
        if (value == 1) {
            func_002C9218(D_00437CD0, D_00437D10, 9);
        }
        return func_002CBDD0();
    }
    if (status == -1) {
        func_002CA1D8(0);
        D_00437CF8 = 0;
        D_00437D3C = 0;
        return (void *)func_002CAB40();
    }
    return func_002CBDD0();
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CBDD0);

void *mcClearSlotMetadata(void) {
    s32 index = 0;
    u32 *saved;
    func_002CA1D8(1);
    D_00437CF8 = 0;
    D_00437D10 = 0;
    func_002C91B8(D_00437CD0);
    saved = D_00458080;
    do {
        *saved++ = 0;
        func_002C91E8(D_00437CD0, index);
        index++;
    } while (index < 10);
    return func_002CBC60();
}

void *mcPrepareDirectory(void) {
    u8 name[0x50];
    u32 entry = D_00437CD0;
    s32 slot = func_002C9280(entry);
    func_002C9250(entry, slot);
    func_002CA1D8(2);
    func_002C91B8(entry);
    if (func_002C9250(entry, slot) & 2) {
        return func_002CBF80();
    }
    name[0] = '/';
    mcdFormatSaveSlotName(name + 1, slot);
    mcMakeDirectory(entry, name);
    return mcHandleSearchResult;
}

void *func_002CBF80(void) {
    u8 buf[0x50];
    u32 entry = D_00437CD0;
    s32 v = func_002C9280(entry);

    buf[0] = 0x2F;
    mcdFormatSaveSlotName(&buf[1], v);
    func_002C9328(entry, buf);
    return func_002CC038;
}

void *mcHandleSearchResult(void) {
    s32 status = func_002C93B8();
    if (status == 0) {
        return NULL;
    }
    if (status == 1) {
        func_002C9218(D_00437CD0, D_00437D2C, 2);
        return func_002CBF80();
    }
    func_002CA1D8(0);
    D_00437D3C = 4;
    return func_002CB5A0;
}

void *func_002CC038(void) {
    s32 t = func_002C9348();

    if (t == 0) {
        return NULL;
    }
    if (t == 1) {
        return mcChooseLoadPath();
    }
    if (t == -1) {
        func_002CA1D8(0);
        D_00437D3C = 4;
        return func_002CB5A0;
    }
    return NULL;
}

void *func_002CC098(void) {
    s32 t = func_002C94B0();

    if (t == 0) {
        return NULL;
    }
    if (t == 1) {
        return mcChooseLoadPath();
    }
    if (t == -1) {
        func_002CA1D8(0);
        D_00437D3C = 4;
        return func_002CB5A0;
    }
    return NULL;
}

void func_002CC0F8(void) {
    func_002C9218(D_00437CD0, D_00437D2C, 1);
    func_002C9218(D_00437CD0, D_00437D2C, 8);
    func_002CA1D8(4);
    func_002CB710((u32)fileScanSlotStates);
}

void *func_002CC140(void) {
    func_002C96D0();
    return func_002CC0F8;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CC168);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B6A8);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B6B8);

INCLUDE_ASM(const s32, "game/code_002C9660", fileRequestBaseIcon);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CC210);

void *mcChooseLoadPath(void) {
    u32 entry = D_00437CD0;
    s32 slot = func_002C9280(entry);
    u32 flags = func_002C9250(entry, slot);
    if (!(flags & 8)) {
        return func_002CC210(entry, D_0042B6B8);
    }
    func_002C9490();
    return func_002CC098;
}

INCLUDE_ASM(const s32, "game/code_002C9660", fileBeginRequest);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CC530);

INCLUDE_ASM(const s32, "game/code_002C9660", mcHandleLoadResult);

INCLUDE_ASM(const s32, "game/code_002C9660", mcDispatchReadCallback);

INCLUDE_ASM(const s32, "game/code_002C9660", fileFinishRequest);

INCLUDE_ASM(const s32, "game/code_002C9660", mcHandleDetectionResult);

void *func_002CC760(void) {
    func_002CA1D8(5);
    func_001004A0();
    func_002C92D0(D_00437CD0);
    return mcHandleDetectionResult;
}

s32 mnuSelectFileBranch(void) {
    if (D_00437D0C != 0) {
        D_00437D1C = 7;
        return func_002CEE70(func_002CAA90, fileResetSelection, 1);
    }
    D_00437D1C = 7;
    return func_002CEE70(func_002CAC80, fileResetSelection, 1);
}

void *fileBeginSlotCreate(void) {
    char path[0x50];
    u32 ctx = D_00437CD0;
    s32 slot = func_002C9280(ctx);
    u32 attr = func_002C9250(ctx, slot);
    u32 len;

    func_002CA1D8(3);
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
    return func_002CC8E0;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CC8E0);

INCLUDE_ASM(const s32, "game/code_002C9660", mcHandleSetupResult);

INCLUDE_ASM(const s32, "game/code_002C9660", handleSaveSetupDone);

extern s32 mcdContinueLoadSelection();

void *func_002CCAA8(void) {
    func_002CA1D8(13);
    return func_002CB660((u32)mcdContinueLoadSelection);
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CCAD0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CD028);

void *fileRunMenuState(s32 arg) {
    void *next;
    u32 job;
    void *(*cur)(s32);
    D_00437CFC++;
    func_002CDFD8(arg);
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
        D_00439030 = func_002C8108(job);
        D_00439034 = func_002C8110(job);
        D_00439038 = func_002C8118(job);
        func_002C7D00(job);
    }
    return NULL;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CDFD8);

void func_002CE1A8(void) {
    func_002CA1D8(0);
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
    func_002CA638();
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CE208);

void func_002CE738(void) {
    func_002CE758();
}

void func_002CE750(void) {
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CE758);

u32 func_002CE920(void) {
    return 1;
}

s32 func_002CE928(void) {
    return func_00101740(D_0042B720) != NULL;
}

u32 func_002CE950(void) {
    return D_00437D34;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CE958);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CEE70);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CEEC0);

u32 func_002CF928(void) {
    return D_00437D04;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CF930);

void func_002CF958(u32 arg0) {
    D_003E8008[0] = arg0;
    D_003E7FD8.unkC = 0x80;
    D_003E7FD8.unk4 = 0;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CF978);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CF9D0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CFA58);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CFA98);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CFC38);

extern u8 D_003E8018[];
extern s32 D_003E7FE4[];

void func_002CFD48(void) {
    s32 *state = (s32 *)D_003E8018;
    D_003E8018[1] = 0;
    D_003E7FE4[0] = 0x80;
    D_003E8018[0] = 1;
    D_003E8018[2] = 2;
    state[0x34 / 4] = 0x80;
    state[0x38 / 4] = 0x80;
    state[0x3C / 4] = 0x80;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CFD80);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B720);

INCLUDE_RODATA(const s32, "game/code_002C9660", jtbl_0042B730);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B770);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B7C0);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B868);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002CFF38);

INCLUDE_ASM(const s32, "game/code_002C9660", fileLoadSetMode);

void func_002D0148(void) {
    D_003E7FD8.unk14 = 0;
    D_003E7FD8.unk10 = 0;
    D_003E7FD8.unk4 = 0;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0160);

void mcdFinishFileDetectionWithAudio(void) {
    if (func_002C97D8((u32)D_0037F510[0x21] >> 31) ||
        func_002C97D8((u32)D_0037F510[0x23] >> 31)) {
        func_002CA1D8(0);
        sndSetSequenceVolumePan(8, 0x7f, 0x3f);
        ((void (*)(void))D_00439020)();
    }
}

void *func_002D0340(u32 arg0) {
    D_00439020 = arg0;
    D_00437D40 = 0;
    return mcdFinishFileDetectionWithAudio;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0358);

extern u32 D_00439024;
extern u32 D_00439028;
extern u32 D_0043902C;
extern s32 func_002D0358();

void *func_002D0470(u32 ready, u32 completed, u32 cancelled, u32 state) {
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

void *func_002D0678(void) {
    kwlnFadeInStart(0, 0, 0, 8);
    func_00342580(0x310000);
    return func_002D0498;
}

void func_002D06B0(void) {
    *(u8 *)(D_00437D84 + 0x31) = 0;
    func_002D0678();
}

void func_002D06D0(void) {
    *(u8 *)(D_00437D84 + 0x31) = 1;
    func_002D0678();
}

void func_002D06F0(void) {
    func_002CA1D8(0);
    D_00437D1C = 12;
    func_002D0470((u32)func_002D06B0, (u32)func_002D06D0, 0, 0);
}

void *func_002D0730(void) {
    D_00437D18 = -1;
    D_00437D1C = 0;
    func_002CA1D8(0x18);
    *(u8 *)(D_00437D84 + 0x30) = 1;
    return func_002D0340((u32)func_002D06F0);
}

void *func_002D0770(void) {
    func_002CE1A8();
    return func_002CAED0;
}

void func_002D0798(void) {
    func_002CA1D8(0);
    D_00437D1C = 11;
    func_002D0470((u32)func_002D0730, (u32)func_002D0770, (u32)func_002D0770, 0);
}

extern s32 func_002D0810();

void *func_002D07D8(void) {
    if (fileIsLoadStepComplete() != 0) {
        return func_002D0340((u32)func_002D0810);
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
    func_002CA1D8(0);
    D_00437D1C = 10;
    *(u8 *)(D_00437D84 + 0x30) = 0;
    func_002D0470((u32)func_002D0770, (u32)func_002D0678, (u32)func_002D08F0, 0);
}

s32 mcdContinueLoadSelection(void) {
    u32 state = func_002CF928();
    u32 block;
    s32 next = (s32)func_002D09C8;
    if (state != 0) {
        if (state == 2) {
            block = D_00437D84;
            *(u16 *)(block + 0x36) = 0;
            func_002D0D90(block);
            sndSetSequenceVolumePan(8, 0x7f, 0x3f);
            next = (s32)func_002D0810;
        } else {
            next = 0;
        }
    }
    return next;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D09C8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0A20);

void func_002D0A90(void) {
    if (D_00437D84 != 0) {
        func_003298C0(*(u32 *)(D_00437D84 + 0x3c));
        D_00437D84 = 0;
    }
}

extern char D_0042B920[];

void func_002D0AB8(void) {
    func_0035B6E0(D_0042B920);
}

extern char D_0042B938[];

void func_002D0AD8(void) {
    u8 *state = (u8 *)D_00435DD0;
    u32 money = *(u32 *)(state + 0x20);
    *(u32 *)(state + 0x1E650) = money;
    func_0035B6E0(D_0042B938, money);
}

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B920);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042B938);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0B08);

void func_002D0D00(s32 arg0) {
    s32 temp_v0;

    temp_v0 = D_00435DD0;
    *(u32 *)(D_00435DD0 + 0x20) = *(u32 *)(arg0 + 0x20);
    *(u32 *)(temp_v0 + 0x24) = *(u32 *)(arg0 + 0x24);
    *(u32 *)(temp_v0 + 0x28) = *(u32 *)(arg0 + 0x28);
    *(u32 *)(temp_v0 + 0x2c) = *(u32 *)(arg0 + 0x2c);
}

typedef struct {
    u8 bytes[0x30];
} __attribute__((packed)) FileRecordHeader;

void func_002D0D28(FileRecordHeader *destination, const FileRecordHeader *source) {
    *destination = *source;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0D90);

s32 fileLoadStateChanged(void) {
    return D_00437DE8.current != D_00437DE8.previous;
}

void func_002D0EB0(void) {
    D_00437DE8.current = D_00437DE8.previous =
        *(u32 *)(D_00435DD0 + 0xA54);
}

void func_002D0EC8(void) {
    *(u32 *)(D_00435DD0 + 0xa54) = D_00437DEC;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0ED8);

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

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D0FB8);

void func_002D1038(u32 arg0) {
    func_002D0FB8(arg0, D_00435DD0 + 0xa54);
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D1058);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D11F8);

INCLUDE_ASM(const s32, "game/code_002C9660", configTasksCreate);

void mnuConfigTasksDestroy(void) {
    kwlnTaskDestroyWithHierarchyByName(D_00437DF8, 1);
    kwlnTaskDestroyWithHierarchyByName(D_0042BAF0, 1);
    kwlnTaskDestroyWithHierarchyByName(D_0042BB00, 1);
}

s32 func_002D13F0(void) {
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

u32 func_002D1428(s32 arg0) {
    if (arg0 < 4) {
        return *(u32 *)((s32)arg0 * 4 + D_00439058 + 0x10);
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042BAF0);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042BB00);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D1450);

INCLUDE_ASM(const s32, "game/code_002C9660", fileStartQueuedLoad);

u32 func_002D1910(void) {
    u32 temp_v0;

    temp_v0 = 0xffffffff;
    if ((*(u32 *)(D_00439058 + 0x28) & 0x80000000) == 0) {
        temp_v0 = 0;
    }
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D1930);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D27A0);

void func_002D2C50(void) {
    func_002E6390();
    effLoadWindTexture();
    effLoadScalyTexture();
    func_002D2CA8();
}

void func_002D2C80(u32 arg0) {
    D_00437E08 = D_00437E08 | arg0;
}

void func_002D2C90(u32 arg0) {
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

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D34B8);

void fileJobDestroy(FileJob *job) {
    void *data = *(void **)((u8 *)job + 8);
    if (data != NULL) {
        u16 index = *(u16 *)((u8 *)job + 4);
        D_003E9168[index].destroy(data);
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
    job->option = request->option;
    job->slots[0].selector = request->slots[0].selector;
    job->data = D_003E9168[job->type].createChild(request->data, job->type);
    return job;
}

INCLUDE_ASM(const s32, "game/code_002C9660", fileJobNotifyPair);

void fileJobNotifyComplete(void *work) {
    u16 id = *(u16 *)((u8 *)work + 4);

    if (D_003E917C[id].func != NULL) {
        D_003E917C[id].func(*(void **)((u8 *)work + 8));
    }
}

void func_002D3710(void *arg0) {
    u16 idx = *(u16 *)((u8 *)arg0 + 4);
    void *data = *(void **)((u8 *)arg0 + 8);

    D_003E916C[idx].cb(data);
}

void func_002D3748(void *work) {
    u16 id = *(u16 *)((u8 *)work + 4);

    if (D_003E9180[id].func != NULL) {
        D_003E9180[id].func(*(void **)((u8 *)work + 8));
    }
}

void func_002D3788(void *work) {
    u16 id = *(u16 *)((u8 *)work + 4);

    if (D_003E9184[id].func != NULL) {
        D_003E9184[id].func(*(void **)((u8 *)work + 8));
    }
}

void func_002D37C8(void *work) {
    u16 id = *(u16 *)((u8 *)work + 4);

    if (D_003E9188[id].func != NULL) {
        D_003E9188[id].func(*(void **)((u8 *)work + 8));
    }
}

void func_002D3808(void *work) {
    u16 id = *(u16 *)((u8 *)work + 4);

    if (D_003E918C[id].func != NULL) {
        D_003E918C[id].func(*(void **)((u8 *)work + 8));
    }
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D3848);

void func_002D38E8(u64 arg0, u64 arg1, u16 arg2) {
    s64 temp_v0;
    u64 temp_v1;
    u64 temp_v2;
    u64 temp_v3;

    temp_v0 = sdfDevCreateCommandState(arg1);
    if (temp_v0 != 0) {
        temp_v1 = func_0033EB30(temp_v0);
        temp_v2 = func_003292A8(temp_v1);
        temp_v3 = sdfResourceRetainAddress(temp_v2);
        func_0033EB10(temp_v0, temp_v3, temp_v1);
        func_0033EAE0(temp_v0);
        func_002D3848(arg0, temp_v3, temp_v1, arg2);
        func_003297C8(temp_v2);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D39C8);

void func_002D3A68(u64 arg0, u64 arg1, u16 arg2) {
    s64 temp_v0;
    u64 temp_v1;
    u64 temp_v2;
    u64 temp_v3;

    temp_v0 = sdfDevCreateCommandState(arg1);
    if (temp_v0 != 0) {
        temp_v1 = func_0033EB30(temp_v0);
        temp_v2 = func_003292A8(temp_v1);
        temp_v3 = sdfResourceRetainAddress(temp_v2);
        func_0033EB10(temp_v0, temp_v3, temp_v1);
        func_0033EAE0(temp_v0);
        func_002D39C8(arg0, temp_v3, temp_v1, arg2);
        func_003297C8(temp_v2);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D3B48);

INCLUDE_ASM(const s32, "game/code_002C9660", fileWriteToPfs);

INCLUDE_ASM(const s32, "game/code_002C9660", fileDuplicateJob);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D3DF8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D3E98);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D3F08);

void func_002D3F80(u32 arg0) {
    s32 temp_v0;

    memset(arg0, 0, 0x90);
    temp_v0 = (s32)arg0;
    *(u32 *)(temp_v0 + 0x84) = 1;
    *(u8 *)(temp_v0 + 0x88) = 8;
    *(u8 *)(temp_v0 + 0x89) = 0;
    *(u8 *)(temp_v0 + 0x8a) = 0;
    func_002D3F08(arg0);
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

void func_002D4120(void) {
    func_00328E48();
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D4138);

void func_002D4380(u32 arg0, u32 arg1) {
    func_002D4138(arg1);
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D4398);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D4548);

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
    func_002D46F0(queue, &vec);
    func_002D4818(queue, &vec);
    func_002D48D0(queue, 1.0f);
    func_002D49B8(queue, 0x80808080);
    return queue;
}

void func_002D46A0(u8 *owner) {
    u8 *job = *(u8 **)(owner + 0x8C);
    while (job != 0) {
        fileJobNotifyComplete(*(u32 *)(job + 0x90));
        job = *(u8 **)(job + 0xAC);
    }
    *(u32 *)(owner + 0x84) = 0;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D46F0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D4818);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D48D0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D49B8);

void func_002D4A98(void *work, void *dst) {
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

INCLUDE_ASM(const s32, "game/code_002C9660", fileAppendJob);

void fileDuplicateAndAppendJob(u64 arg0, u64 arg1) {
    u64 temp_v0;

    temp_v0 = fileDuplicateJob(arg1);
    fileAppendJob(arg0, temp_v0);
}

INCLUDE_ASM(const s32, "game/code_002C9660", fileAppendJobFromEntry);

INCLUDE_ASM(const s32, "game/code_002C9660", fileJobDuplicateAfter);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D4CF0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D4E60);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D4F10);

INCLUDE_ASM(const s32, "game/code_002C9660", fileJobCopyHeader);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D50D8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D55B0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D5AA8);

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

s32 func_002D5D90(FileQueue *queue) {
    FileJob *job;
    s32 count;

    count = 0;
    for (job = queue->tail; job != 0; job = job->next) {
        count = count + 1;
    }
    return count;
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D5DC8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D5EC0);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D5FB8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D6020);

INCLUDE_RODATA(const s32, "game/code_002C9660", D_0042BB28);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D6058);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D6160);

INCLUDE_ASM(const s32, "game/code_002C9660", fileLoadObjectCreateChild);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D62D8);

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
        func_00159C40(resource, *(s16 *)(*(u32 *)(node->active + 0x20) + 0x54));
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
        func_00159C40(resource, *(s16 *)(*(u32 *)(node->active + 0x20) + 0x54));
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
        func_00159C40(node->resource, *(s16 *)(*(u32 *)(node->active + 0x20) + 0x54));
    }
}

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D6710);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D6808);

/* DDS2 loader work has a longer prefix than the DDS1 LoadObj. */
typedef struct LoadObj {
    u8 pad00[8];
    f32 scale;             /* 0x08 */
    u8 pad0C[0x3C];
    void *referenceHolder; /* 0x48 */
    void *recordWork;      /* 0x4C */
} LoadObj;

void func_002D6900(LoadObj *obj, u32 resource) {
    u32 holder;

    if (obj->referenceHolder != NULL) {
        effReleaseReferenceHolder((s32)obj->referenceHolder);
    }
    holder = func_002DDF48(resource);
    obj->referenceHolder = (void *)holder;
}

void func_002D6950(LoadObj *obj) {
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

void func_002D73C0(LoadObj *obj) {
    func_002DC0C0((u32)obj->recordWork);
}

void func_002D73D8(LoadObj *obj) {
    func_002DC0F0((u32)obj->recordWork);
}

void func_002D73F0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 4) = arg1;
}

void func_002D73F8(LoadObj *arg0, f32 arg1) {
    arg0->scale = arg1;
    func_002DC108(arg0->recordWork);
}

typedef struct FileSlot {
    u8 pad0[0x10];
    u32 state;
    u8 pad14[0xC];
} FileSlot;

typedef struct FileSlotTable {
    u8 pad00[8];
    u32 count;
    u8 pad0C[0xC];
    FileSlot *slots;
} FileSlotTable;

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

void func_002D8A38(ScaleOwner *owner, f32 scale) {
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

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D9228);

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

INCLUDE_ASM(const s32, "game/code_002C9660", func_002D9AF8);

INCLUDE_ASM(const s32, "game/code_002C9660", func_002DA2D8);

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

INCLUDE_ASM(const s32, "game/code_002C9660", func_002DBED8);

void func_002DC028(s32 arg0) {
    func_003297C8(*(u32 *)(arg0 + 0x28));
}

void fileClearRecordReferences(s32 arg0) {
    *(u32 *)(arg0 + 0x10) = 0;
}

INCLUDE_ASM(const s32, "game/code_002C9660", fileAcquireRecord);

void func_002DC0A8(u8 *obj, void *dst) {
    PCP_COPY_VECTOR(dst, *(u8 **)(obj + 0x20));
}

void func_002DC0C0(u8 *obj, void *src) {
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

