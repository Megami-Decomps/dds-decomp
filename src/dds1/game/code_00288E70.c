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

/* Async job handled by func_00288E70 and friends. */
typedef struct FileJob {
    u8 unk0;      /* 0x00 */
    u8 state;     /* 0x01 */
    u8 unk2[2];   /* 0x02 */
    struct FileJob *unk4; /* 0x04 */
    u8 unk8[4];   /* 0x08 */
    void *deviceRequest; /* 0x0C: transfer backend dereferences mode at +0x16 */
    s32 transferBytes; /* 0x10: capped at 0x8000 for each device operation */
    s32 totalBytes; /* 0x14 */
    u8 pad18[0x10];
    u32 transferAddress; /* 0x28: forwarded to backend request at +0x20 */
    u8 pad2C[0x3C];
    u16 stateRequired; /* 0x68 */
    u16 slot; /* 0x6A */
} FileJob;

/* Completion node drained by fileManDispatchDone. */
typedef struct FileCbNode {
    u8 unk0[0x18];                   /* 0x00 */
    void (*cb)(void *node, u32 arg); /* 0x18 */
    u32 arg;                         /* 0x1C */
    u8 unk20[0xC];                   /* 0x20 */
    struct FileCbNode *next;         /* 0x2C */
} FileCbNode;

extern char D_003BC7E0[];
extern FileManGuardState D_003BC7D8;
/* Scalar view keeps the original direct GP-relative access. */
extern s32 fileManGuardActive __asm__("D_003BC7D8");
extern s32 (*fileIdleUpdateCallback)(void);

s32 WaitSema(s32 sema);
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
extern s32 sdfDevQueueControlRequest(void *);
extern s32 sdfDevQueueActiveOperation(void *);
extern s32 sdfDevQueueReleaseState(void *);
extern void fileQueuePendingRequestInFreeSlot(FileJob *);
extern void filePrependNode(void *, void *);
extern void fileUnlinkNode(void *, void *);
extern s32 func_00289540(void);

s32 func_00288E70(void *deviceRequest, s32 event, s32 unused, s32 byteCount, FileJob *job) {
    FileManWork *work = &fileManagerWork;
    s32 saved;

    switch (event) {
    case 2:
        job->deviceRequest = deviceRequest;
        job->state = 2;
        sdfDevQueueControlRequest(deviceRequest);
        break;
    case 4:
        job->transferBytes = byteCount;
        job->state = FILE_JOB_READY;
        job->totalBytes = byteCount;
        fileQueuePendingRequestInFreeSlot(job);
        break;
    case 5:
        WaitSema(work->sema);
        if (job->stateRequired != 0) {
            job->state = 5;
        } else {
            work->slots[job->slot].value = byteCount;
            job->transferBytes -= byteCount;
            if (job->transferBytes == 0) {
                job->state = 5;
                work->head = (struct FileNode *)job->unk4;
                if (job->unk4 == NULL) {
                    work->tail = NULL;
                }
            } else {
                job->state = FILE_JOB_READY;
            }
        }
        work->activeSlots++;
        if (job->state == 5) {
            filePrependNode(work, job);
            SignalSema(work->sema);
            sdfDevQueueActiveOperation(deviceRequest);
        } else {
            SignalSema(work->sema);
        }
        saved = fileManGuardActive;
        fileManGuardActive = 1;
        func_00289540();
        fileManGuardActive = saved;
        break;
    case 7:
        WaitSema(work->sema);
        fileUnlinkNode(work, job);
        SignalSema(work->sema);
        sdfDevQueueReleaseState(deviceRequest);
        job->deviceRequest = NULL;
        job->state = 6;
        break;
    }
    return 0;
}

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
    while ((node = work->done) != NULL) {
        work->done = node->next;
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
    fileManagerWork.freeSlots = 4;
    fileManagerWork.sema = sdfCreateSemaphore(1, 0x7F, 0);
    fileManagerWork.buffer = sdfResourceRetainAddress(sdfAllocGeneralBlock(0x40000));
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

FileManGuardState D_003BC7D8 __attribute__((section(".sdata"))) = { 0 };

char D_003BC7E0[8] __attribute__((section(".sdata"))) = "fileMan";
