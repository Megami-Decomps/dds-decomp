#include "common.h"
#include "file.h"

/* File request entry: fileRequestEntries table, 0x64 bytes per entry. */
typedef struct FileReqEntry {
    u32 unk0;      /* 0x00 */
    u32 unk4;      /* 0x04 */
    u32 sizeKiB;   /* 0x08: converted to bytes by fileReqGetSize */
    u32 unkC;      /* 0x0C */
    u8 unk10;      /* 0x10 */
    u8 status;     /* 0x11: inspected by memory-card file request polling */
    u8 slotMetadataDirty; /* 0x12: checked before rebuilding slot metadata */
    s8 selectedSlot; /* 0x13: used to select a memory-card save directory */
    u32 slotFlags[20]; /* 0x14: save-slot flag words, aliased by fileRequestSlotFlags */
} FileReqEntry;

extern FileReqEntry fileRequestEntries[];
/* Flag words of the entry table: entry arg0 occupies 0x19 words. */
extern u32 fileRequestSlotFlags[];
extern s32 D_003BD8E8;

/* Work area behind the fileMan task (fileManagerWork, 0x40 bytes). */
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
    void *deviceRequest; /* 0x0C: transfer backend dereferences mode at +0x16 */
    s32 transferBytes; /* 0x10: capped at 0x8000 for each device operation */
    u8 unk14[0x14]; /* 0x14 */
    u32 transferAddress; /* 0x28: forwarded to backend request at +0x20 */
} FileJob;

/* Completion node drained by fileManDispatchDone. */
typedef struct FileCbNode {
    u8 unk0[0x18];                   /* 0x00 */
    void (*cb)(void *node, u32 arg); /* 0x18 */
    u32 arg;                         /* 0x1C */
    u8 unk20[0xC];                   /* 0x20 */
    struct FileCbNode *next;         /* 0x2C */
} FileCbNode;

extern FileManWork fileManagerWork;
extern char D_003BC7E0[];
extern s32 D_003BC7D8;
extern s32 (*fileIdleUpdateCallback)(void);

void WaitSema(s32 sema);
s32 SignalSema(s32 sema);
void *memset(void *dst, s32 val, u32 len);
s32 sdfCreateSemaphore(s32 arg0, s32 arg1, s32 arg2);
s32 sdfAllocGeneralBlock(s32 arg0);
s32 sdfResourceRetainAddress(s32 arg0);
s32 kwlnTaskCreate(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
s32 func_002F6990(s32 arg0, s32 arg1, void *arg2, void *arg3, void *arg4);
s32 func_002F6858(s32 arg0, void *arg1, s32 *arg2);
void func_002F6E90(u32 arg0, s32 arg1);
void sdfDevQueueRead(void *deviceRequest, u32 transferAddress, u32 byteCount);
void sdfDevQueueWrite(void *deviceRequest, u32 transferAddress, u32 byteCount);
s32 fileMan(void);
s32 fileManUpdate(void);
void fileReqInit(s32 arg0);

INCLUDE_ASM(const s32, "game/code_00288E70", func_00288E70);

void fileStartChunkedReadWhenReady(FileJob *job) {
    WaitSema(fileManagerWork.sema);
    if (job->state != FILE_JOB_READY) {
        SignalSema(fileManagerWork.sema);
        return;
    }
    job->state = FILE_JOB_TRANSFERRING;
    SignalSema(fileManagerWork.sema);
    sdfDevQueueRead(job->deviceRequest, job->transferAddress, job->transferBytes <= FILE_IO_MAX_CHUNK_BYTES ? job->transferBytes : FILE_IO_MAX_CHUNK_BYTES);
}

INCLUDE_ASM(const s32, "game/code_00288E70", func_002890B8);

void fileStartChunkedWriteWhenReady(FileJob *job) {
    WaitSema(fileManagerWork.sema);
    if (job->state != FILE_JOB_READY) {
        SignalSema(fileManagerWork.sema);
        return;
    }
    job->state = FILE_JOB_TRANSFERRING;
    SignalSema(fileManagerWork.sema);
    sdfDevQueueWrite(job->deviceRequest, job->transferAddress, job->transferBytes <= FILE_IO_MAX_CHUNK_BYTES ? job->transferBytes : FILE_IO_MAX_CHUNK_BYTES);
}

INCLUDE_ASM(const s32, "game/code_00288E70", func_00289380);

INCLUDE_ASM(const s32, "game/code_00288E70", func_00289540);

void fileManDispatchDone(void) {
    FileManWork *work = &fileManagerWork;
    FileCbNode *node;

    WaitSema(work->sema);
    while ((node = work->unk10) != NULL) {
        work->unk10 = node->next;
        SignalSema(work->sema);
        node->cb(node, node->arg);
        WaitSema(work->sema);
    }
    SignalSema(work->sema);
}

INCLUDE_ASM(const s32, "game/code_00288E70", fileManUpdate);

/* Task callback driving asynchronous file work. */
s32 fileMan(void) {
    fileManUpdate();
    return 0;
}

void fileManInit(void) {
    memset(&fileManagerWork, 0, 0x40);
    fileManagerWork.unk7 = 4;
    fileManagerWork.sema = sdfCreateSemaphore(1, 0x7F, 0);
    fileManagerWork.unk1C = sdfResourceRetainAddress(sdfAllocGeneralBlock(0x40000));
    kwlnTaskCreate((s32)&D_003BC7E0, 0x384, 1, 0, (s32)&fileMan, 0, 0);
    fileIdleUpdateCallback = fileManUpdate;
}

INCLUDE_ASM(const s32, "game/code_00288E70", fileReqInit);

/* Select and reinitialize one file request, then clear its +0x10 byte. */
void fileReqBegin(s32 request) {
    D_003BD8E8 = request;
    fileReqInit(request);
    fileRequestEntries[request].unk10 = 0;
}

INCLUDE_ASM(const s32, "game/code_00288E70", fileReqPoll);

u8 fileReqGetStatus(s32 request) {
    return fileRequestEntries[request].status;
}

s32 fileReqGetSize(s32 request) {
    return fileRequestEntries[request].sizeKiB << 10;
}

u8 fileReqIsSlotMetadataDirty(s32 request) {
    return fileRequestEntries[request].slotMetadataDirty;
}

void fileReqClearSlotMetadataDirty(s32 request) {
    fileRequestEntries[request].slotMetadataDirty = 0;
}

void fileReqMarkSlotMetadataDirty(s32 request) {
    fileRequestEntries[request].slotMetadataDirty = 1;
}

void fileReqClearSlotFlags(s32 request, s32 slot) {
    slot += request * FILE_REQ_WORDS_PER_ENTRY;
    fileRequestSlotFlags[slot] = 0;
}

void fileReqSetSlotFlags(s32 request, s32 slot, s32 mask) {
    slot += request * FILE_REQ_WORDS_PER_ENTRY;
    fileRequestSlotFlags[slot] |= mask;
}

u32 fileReqGetSlotFlags(s32 request, s32 slot) {
    slot += request * FILE_REQ_WORDS_PER_ENTRY;
    return fileRequestSlotFlags[slot];
}

s8 fileReqGetSelectedSlot(s32 request) {
    return fileRequestEntries[request].selectedSlot;
}

void fileReqSetSelectedSlot(s32 request, s8 selectedSlot) {
    fileRequestEntries[request].selectedSlot = selectedSlot;
}

void func_00289D50(u32 arg0) {
    func_002F6E90(arg0, 0);
}

INCLUDE_SDATA(const s32, "game/code_00288E70", D_003BC7D8);

INCLUDE_SDATA(const s32, "game/code_00288E70", D_003BC7E0);

