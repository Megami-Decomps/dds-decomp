#include "common.h"

/* File request entry: D_003DC698 table, 0x64 bytes per entry. */
typedef struct FileReqEntry {
    u32 unk0;      /* 0x00 */
    u32 unk4;      /* 0x04 */
    u32 sizeKiB;   /* 0x08: converted to bytes by fileReqGetSize */
    u32 unkC;      /* 0x0C */
    u8 unk10;      /* 0x10 */
    u8 unk11;      /* 0x11 */
    u8 unk12;      /* 0x12 */
    s8 unk13;      /* 0x13 */
    u32 unk14[20]; /* 0x14 */
} FileReqEntry;

extern FileReqEntry D_003DC698[];
/* Flag words of the entry table: entry arg0 occupies 0x19 words. */
extern u32 D_003DC6AC[];
extern s32 D_003BD8E8;

/* Work area behind the fileMan task (D_003DC658, 0x40 bytes). */
typedef struct FileManWork {
    s32 sema;   /* 0x00 */
    u8 unk4;    /* 0x04 */
    u8 unk5;    /* 0x05 */
    u8 unk6;    /* 0x06 */
    u8 unk7;    /* 0x07 */
    void *unk8; /* 0x08 */
    u32 unkC;   /* 0x0C */
    void *unk10; /* 0x10 */
    void *unk14; /* 0x14 */
    u32 unk18;  /* 0x18 */
    s32 unk1C;  /* 0x1C */
    u8 unk20[0x20]; /* 0x20 */
} FileManWork;

/* Async job handled by func_00288E70 and friends. */
typedef struct FileJob {
    u8 unk0;      /* 0x00 */
    u8 state;     /* 0x01 */
    u8 unk2[2];   /* 0x02 */
    u32 unk4;     /* 0x04 */
    u8 unk8[4];   /* 0x08 */
    u32 unkC;     /* 0x0C */
    s32 transferBytes; /* 0x10: capped at 0x8000 for each device operation */
    u8 unk14[0x14]; /* 0x14 */
    u32 unk28;    /* 0x28 */
} FileJob;

/* Completion node drained by fileManDispatchDone. */
typedef struct FileCbNode {
    u8 unk0[0x18];                   /* 0x00 */
    void (*cb)(void *node, u32 arg); /* 0x18 */
    u32 arg;                         /* 0x1C */
    u8 unk20[0xC];                   /* 0x20 */
    struct FileCbNode *next;         /* 0x2C */
} FileCbNode;

extern FileManWork D_003DC658;
extern char D_003BC7E0[];
extern s32 D_003BC7D8;
extern s32 (*D_003BD4A8)(void);

void WaitSema(s32 sema);
void SignalSema(s32 sema);
void *memset(void *dst, s32 val, u32 len);
s32 sdfCreateSemaphore(s32 arg0, s32 arg1, s32 arg2);
s32 func_002D03F8(s32 arg0);
s32 sdfResourceRetainAddress(s32 arg0);
s32 kwlnTaskCreate(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
s32 func_002F6990(s32 arg0, s32 arg1, void *arg2, void *arg3, void *arg4);
s32 func_002F6858(s32 arg0, void *arg1, s32 *arg2);
void func_002F6E90(u32 arg0, s32 arg1);
void func_002E6D48(u32 arg0, u32 arg1, u32 arg2);
void func_002E6DA8(u32 arg0, u32 arg1, u32 arg2);
s32 fileMan(void);
s32 fileManUpdate(void);
void fileReqInit(s32 arg0);

INCLUDE_ASM(const s32, "game/code_00288E70", func_00288E70);

void func_00289030(FileJob *job) {
    WaitSema(D_003DC658.sema);
    if (job->state != 3) {
        SignalSema(D_003DC658.sema);
        return;
    }
    job->state = 4;
    SignalSema(D_003DC658.sema);
    func_002E6D48(job->unkC, job->unk28, job->transferBytes <= 0x8000 ? job->transferBytes : 0x8000);
}

INCLUDE_ASM(const s32, "game/code_00288E70", func_002890B8);

void func_002892F8(FileJob *job) {
    WaitSema(D_003DC658.sema);
    if (job->state != 3) {
        SignalSema(D_003DC658.sema);
        return;
    }
    job->state = 4;
    SignalSema(D_003DC658.sema);
    func_002E6DA8(job->unkC, job->unk28, job->transferBytes <= 0x8000 ? job->transferBytes : 0x8000);
}

INCLUDE_ASM(const s32, "game/code_00288E70", func_00289380);

INCLUDE_ASM(const s32, "game/code_00288E70", func_00289540);

INCLUDE_ASM(const s32, "game/code_00288E70", fileManDispatchDone);

INCLUDE_ASM(const s32, "game/code_00288E70", fileManUpdate);

s32 fileMan(void) {
    fileManUpdate();
    return 0;
}

void fileManInit(void) {
    memset(&D_003DC658, 0, 0x40);
    D_003DC658.unk7 = 4;
    D_003DC658.sema = sdfCreateSemaphore(1, 0x7F, 0);
    D_003DC658.unk1C = sdfResourceRetainAddress(func_002D03F8(0x40000));
    kwlnTaskCreate((s32)&D_003BC7E0, 0x384, 1, 0, (s32)&fileMan, 0, 0);
    D_003BD4A8 = fileManUpdate;
}

INCLUDE_ASM(const s32, "game/code_00288E70", fileReqInit);

void fileReqBegin(s32 request) {
    D_003BD8E8 = request;
    fileReqInit(request);
    D_003DC698[request].unk10 = 0;
}

INCLUDE_ASM(const s32, "game/code_00288E70", fileReqPoll);

u8 func_00289B98(s32 arg0) {
    return D_003DC698[arg0].unk11;
}

s32 fileReqGetSize(s32 arg0) {
    return D_003DC698[arg0].sizeKiB << 10;
}

u8 func_00289BE8(s32 arg0) {
    return D_003DC698[arg0].unk12;
}

void func_00289C10(s32 arg0) {
    D_003DC698[arg0].unk12 = 0;
}

void func_00289C38(s32 arg0) {
    D_003DC698[arg0].unk12 = 1;
}

void func_00289C68(s32 request, s32 flagIndex) {
    flagIndex += request * 0x19;
    D_003DC6AC[flagIndex] = 0;
}

void func_00289C98(s32 request, s32 flagIndex, s32 mask) {
    flagIndex += request * 0x19;
    D_003DC6AC[flagIndex] |= mask;
}

u32 func_00289CD0(s32 request, s32 flagIndex) {
    flagIndex += request * 0x19;
    return D_003DC6AC[flagIndex];
}

s8 func_00289D00(s32 request) {
    return D_003DC698[request].unk13;
}

void func_00289D28(s32 request, s8 value) {
    D_003DC698[request].unk13 = value;
}

void func_00289D50(u32 arg0) {
    func_002F6E90(arg0, 0);
}

INCLUDE_SDATA(const s32, "game/code_00288E70", D_003BC7D8);

INCLUDE_SDATA(const s32, "game/code_00288E70", D_003BC7E0);

