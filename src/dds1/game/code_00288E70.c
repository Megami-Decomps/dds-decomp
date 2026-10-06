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

/* A 0x6C-byte async device transfer, distinct from an effect/file-queue FileJob. */
typedef struct FileTransferJob {
    u8 kind;      /* 0x00 */
    u8 state;     /* 0x01 */
    u8 retryCount; /* 0x02 */
    u8 allocationMode; /* 0x03 */
    struct FileTransferJob *next; /* 0x04 */
    char *name;   /* 0x08 */
    void *deviceRequest; /* 0x0C: transfer backend dereferences mode at +0x16 */
    s32 transferBytes; /* 0x10: capped at 0x8000 for each device operation */
    s32 totalBytes; /* 0x14 */
    void (*completionCallback)(void *job, u32 arg); /* 0x18 */
    u32 completionArg; /* 0x1C */
    s32 allocationHandle; /* 0x20 */
    u32 retainedAddress; /* 0x24 */
    u32 transferAddress; /* 0x28: forwarded to backend request at +0x20 */
    struct FileTransferJob *completionNext; /* 0x2C */
    u8 pad30[0x38];
    u16 stateRequired; /* 0x68 */
    u16 slot; /* 0x6A */
} FileTransferJob;

/* The open device state retains the resolved path at +0x10. */
typedef struct DevStatePathView {
    u8 pad00[0x10];
    char *path;
} DevStatePathView;

/* Completion node drained by fileManDispatchDone. */
typedef struct FileCbNode {
    u8 unk0[0x18];                   /* 0x00 */
    void (*cb)(void *node, u32 arg); /* 0x18 */
    u32 arg;                         /* 0x1C */
    u8 unk20[0xC];                   /* 0x20 */
    struct FileCbNode *next;         /* 0x2C */
} FileCbNode;

extern char D_003BC7E0[];
extern s32 D_003BC7D8;
extern s32 (*fileIdleUpdateCallback)(void);

s32 WaitSema(s32 sema);
s32 SignalSema(s32 sema);
void *memset(void *dst, s32 val, u32 len);
s32 sdfCreateSemaphore(s32 arg0, s32 arg1, s32 arg2);
s32 sdfAllocGeneralBlock(s32 arg0);
s32 sdfAllocGeneralBlockHigh(s32 size);
s32 sdfTryAllocGeneralBlock(s32 size);
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
extern void fileQueuePendingRequestInFreeSlot(FileTransferJob *);
extern void filePrependNode(void *, void *);
extern void fileUnlinkNode(void *, void *);
extern void func_00289540(void);
extern void *sdfDevCreateCallbackState(s32 path,
                        s32 (*callback)(void *, s32, s32, s32, FileTransferJob *), FileTransferJob *job);
extern void *sdfDevCreateModeState(s32 path,
                        s32 (*callback)(void *, s32, s32, s32, FileTransferJob *), FileTransferJob *job,
                        s32 options);
extern s32 sdfPrintFormattedDevMessage(const char *fmt, ...);

s32 func_00288E70(void *deviceRequest, s32 event, s32 unused, s32 byteCount, FileTransferJob *job) {
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
                work->head = (struct FileNode *)job->next;
                if (job->next == NULL) {
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
        saved = D_003BC7D8;
        D_003BC7D8 = 1;
        func_00289540();
        D_003BC7D8 = saved;
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

void fileStartChunkedReadWhenReady(FileTransferJob *job) {
    WaitSema(fileManagerWork.sema);
    if (job->state != FILE_JOB_READY) {
        SignalSema(fileManagerWork.sema);
        return;
    }
    job->state = FILE_JOB_TRANSFERRING;
    SignalSema(fileManagerWork.sema);
    sdfDevQueueRead(job->deviceRequest, job->transferAddress, job->transferBytes <= FILE_IO_MAX_CHUNK_BYTES ? job->transferBytes : FILE_IO_MAX_CHUNK_BYTES);
}

/* Drive allocation, chunked reads, close, and completion-list handoff. */
s32 func_002890B8(void *deviceRequest, s32 event, s32 unused, s32 byteCount, FileTransferJob *job) {
    FileManWork *work = &fileManagerWork;
    s32 saved;

    switch (event) {
    case 2:
        job->deviceRequest = deviceRequest;
        job->state = event;
        sdfDevQueueControlRequest(deviceRequest);
        break;
    case 4: {
        s32 allocationHandle;

        job->transferBytes = byteCount;
        job->totalBytes = byteCount;
        if (job->allocationMode == 0) {
            allocationHandle = sdfTryAllocGeneralBlock(byteCount);
            job->allocationHandle = allocationHandle;
            if (allocationHandle == 0) {
                job->retryCount = 10;
                job->state = 7;
                break;
            }
        } else {
            allocationHandle = sdfAllocGeneralBlockHigh(byteCount);
            job->allocationHandle = allocationHandle;
        }
        {
            u32 address = sdfResourceRetainAddress(allocationHandle);

            job->transferAddress = address;
            job->retainedAddress = address;
            job->state = FILE_JOB_READY;
        }
        fileStartChunkedReadWhenReady(job);
        break;
    }
    case 5:
        WaitSema(work->sema);
        job->transferBytes -= byteCount;
        if (job->transferBytes == 0) {
            job->state = 5;
            if (work->head == (struct FileNode *)job) {
                work->head = (struct FileNode *)job->next;
                if (job->next == NULL) {
                    work->tail = NULL;
                }
            }
        } else {
            job->state = FILE_JOB_READY;
            job->transferAddress += byteCount;
        }
        if (job->state == 5) {
            filePrependNode(work, job);
            SignalSema(work->sema);
            sdfDevQueueActiveOperation(deviceRequest);
            saved = D_003BC7D8;
            D_003BC7D8 = 2;
            func_00289540();
            D_003BC7D8 = saved;
        } else {
            SignalSema(work->sema);
            fileStartChunkedReadWhenReady(job);
        }
        break;
    case 7:
        sdfDevQueueReleaseState(deviceRequest);
        job->deviceRequest = NULL;
        job->state = 6;
        WaitSema(work->sema);
        fileUnlinkNode(work, job);
        if (job->completionCallback != NULL) {
            FileTransferJob **link = (FileTransferJob **)&work->done;
            FileTransferJob *next;

            while ((next = *link) != NULL) {
                link = &next->completionNext;
            }
            *link = job;
        }
        SignalSema(work->sema);
        break;
    }
    return 0;
}

void fileStartChunkedWriteWhenReady(FileTransferJob *job) {
    WaitSema(fileManagerWork.sema);
    if (job->state != FILE_JOB_READY) {
        SignalSema(fileManagerWork.sema);
        return;
    }
    job->state = FILE_JOB_TRANSFERRING;
    SignalSema(fileManagerWork.sema);
    sdfDevQueueWrite(job->deviceRequest, job->transferAddress, job->transferBytes <= FILE_IO_MAX_CHUNK_BYTES ? job->transferBytes : FILE_IO_MAX_CHUNK_BYTES);
}

/* Advance a chunked write, then close and queue its completion callback. */
s32 func_00289380(void *deviceRequest, s32 event, s32 unused, s32 byteCount, FileTransferJob *job) {
    FileManWork *work = &fileManagerWork;
    s32 saved;

    switch (event) {
    case 2:
        job->state = FILE_JOB_READY;
        fileStartChunkedWriteWhenReady(job);
        break;
    case 6:
        WaitSema(work->sema);
        job->transferAddress += byteCount;
        job->transferBytes -= byteCount;
        if (job->transferBytes == 0) {
            job->state = 5;
            if (work->head == (struct FileNode *)job) {
                work->head = (struct FileNode *)job->next;
                if (job->next == NULL) {
                    work->tail = NULL;
                }
            }
        } else {
            job->state = FILE_JOB_READY;
        }
        if (job->state == 5) {
            filePrependNode(work, job);
            SignalSema(work->sema);
            sdfDevQueueActiveOperation(deviceRequest);
            saved = D_003BC7D8;
            D_003BC7D8 = 3;
            func_00289540();
            D_003BC7D8 = saved;
        } else {
            SignalSema(work->sema);
            fileStartChunkedWriteWhenReady(job);
        }
        break;
    case 7:
        sdfDevQueueReleaseState(deviceRequest);
        job->deviceRequest = NULL;
        job->state = 6;
        WaitSema(work->sema);
        fileUnlinkNode(work, job);
        if (job->completionCallback != NULL) {
            FileTransferJob **link = (FileTransferJob **)&work->done;
            FileTransferJob *next;

            while ((next = *link) != NULL) {
                link = &next->completionNext;
            }
            *link = job;
        }
        SignalSema(work->sema);
        break;
    }
    return 0;
}

/* Start or resume the read, PAC, or write job at the head of the queue. */
void func_00289540(void) {
    FileManWork *work = &fileManagerWork;
    FileTransferJob *job;
    s32 locked;

    WaitSema(work->sema);
    job = (FileTransferJob *)work->head;
    if (job == NULL) {
        SignalSema(work->sema);
        return;
    }
    locked = 1;
    switch (job->kind) {
    case 0:
        switch (job->state) {
        case 0:
            job->state = 1;
            locked = 0;
            SignalSema(work->sema);
            job->deviceRequest = sdfDevCreateCallbackState((s32)job->name, func_002890B8, job);
            break;
        case 7:
            job->allocationHandle = sdfTryAllocGeneralBlock(job->totalBytes);
            if (job->allocationHandle == 0) {
                sdfPrintFormattedDevMessage("alloc retry for %s\n",
                                            ((DevStatePathView *)job->deviceRequest)->path);
                job->retryCount--;
            } else {
                u32 address = sdfResourceRetainAddress(job->allocationHandle);

                job->transferAddress = address;
                job->retainedAddress = address;
                job->state = FILE_JOB_READY;
                fileStartChunkedReadWhenReady(job);
            }
            break;
        }
        break;
    case 1:
        switch (job->state) {
        case 0:
            job->state = 1;
            locked = 0;
            SignalSema(work->sema);
            job->deviceRequest = sdfDevCreateCallbackState((s32)job->name, func_00288E70, job);
            break;
        case FILE_JOB_READY:
            locked = 0;
            SignalSema(work->sema);
            fileQueuePendingRequestInFreeSlot(job);
            break;
        }
        break;
    case 2:
        if (job->state == 0) {
            job->state = 1;
            locked = 0;
            SignalSema(work->sema);
            job->deviceRequest = sdfDevCreateModeState((s32)job->name, func_00289380, job, 0x180);
        }
        break;
    }
    if (locked != 0) {
        SignalSema(work->sema);
    }
}

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

s32 D_003BC7D8 __attribute__((section(".sdata"))) = 0;

char D_003BC7E0[8] __attribute__((section(".sdata"))) = "fileMan";
