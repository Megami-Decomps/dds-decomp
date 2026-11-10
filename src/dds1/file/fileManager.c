#include "common.h"
#include "libsdf.h"
#include "kernel.h"
#include "kwln.h"

#define fileHandleFree fileRequestFreeIfComplete
#include "fileManager.h"

/* All three views share the 0x30-byte request header. Their differently
 * typed links distinguish transfer work from the queue and PAC callbacks. */
typedef void (*fileTransferCallback_t)(void *request, u32 arg);

#define FILE_REQUEST_FIELDS(Link, Callback) \
    u8 kind; \
    u8 state; \
    u8 retry; \
    u8 mode; \
    Link *next; \
    char *name; \
    void *dev; \
    s32 rest; \
    s32 size; \
    Callback callback; \
    void *arg; \
    sdfMemHandle_t *handle; \
    addr_t top; \
    addr_t ptr; \
    Link *donenext; \
    sdfPacDecodeCb_t pac; \
    u16 pacrest; \
    u16 slot; \
    Link *cbnext;


typedef struct fileTransferJob {
    FILE_REQUEST_FIELDS(struct fileTransferJob, fileTransferCallback_t)
} fileCb_t;

typedef struct pacFileRequest {
    FILE_REQUEST_FIELDS(struct pacFileRequest, void *)
} pacFileRequest_t;

#undef FILE_REQUEST_FIELDS

typedef struct fileSlot {
    /* 0x0 */ u32 size;
    /* 0x4 */ fileRequest_t *req;
} fileSlot_t;

typedef struct fileQueue {
    /* 0x00 */ s32 semaId;
    /* 0x04 */ u8 index;
    /* 0x05 */ u8 readIndex;
    /* 0x06 */ u8 activeCount;
    /* 0x07 */ u8 freeCount;
    /* 0x08 */ fileRequest_t *queue;
    /* 0x0C */ fileRequest_t *queueTail;
    /* 0x10 */ fileCb_t *endCallbackLink;
    /* 0x14 */ pacFileRequest_t *pacEndCallbackLink;
    /* 0x18 */ fileRequest_t *closeLink;
    /* 0x1C */ u8 *buffer;
    /* 0x20 */ fileSlot_t data[4];
} fileMngCb_tag;


extern fileMngCb_tag fileQueueHead;
extern u32 fileCallbackStatus;
extern s32 (*p2sdSmgBuf)(void);
extern char fileManagerProcName[];

void fileLinkQueue(fileRequest_t *req, s32 kind, const char *name, void *callback, void *arg);
void fileLinkCloseWait(fileMngCb_tag *cb, fileRequest_t *fileHandle);
void fileUnlinkCloseWait(fileMngCb_tag *cb, fileRequest_t *fileHandle);
fileRequest_t *pacLoadEx(const char *filename, u32 headOrTail, u32 callback, u32 pendCallback, u32 callbackParm);
s32 pacFileCheckEnd(const fileRequest_t *pacFileHandle);
void pacRead(fileRequest_t *pacHandle);
s32 pacLoadCallback(void *fileHandle, s32 cause, s32 buffer, s32 length, fileCb_t *data);
s32 fileLoadCallback(void *fileHandle, s32 cause, s32 buffer, s32 length, fileCb_t *data);
s32 fileSaveCallback(void *fileHandle, s32 cause, s32 buffer, s32 length, fileCb_t *data);
void fileRead(fileCb_t *fileHandle);
void fileExecQueue(void);
s32 fileManager(void);
s32 fileManagerProc(dds3ProcessID_t *procId);

dds3ProcessID_t *dds3StartProcess(char *v1, s32 dDS3_PRIORITY_DRAW_PB, s32 v2, s32 v3,
                                s32 (*drawExit)(dds3ProcessID_t *),
                                void (*deleteExit)(dds3ProcessID_t *), void *p);
s32 sdfCreateSema(s32 initCount, s32 maxCount, s32 option);
s32 WaitSema(s32 sema);
s32 SignalSema(s32 sema);
void *sdfChipAllocZeroDirect(u32 aSize);
char *sdfStrdupChip(const char *str);
void *sdfMemAllocDirect(u32 aSize);
void *sdfMemAlloc2Direct(u32 aSize);
u32 sdfMemLock(void *aHandle);
void sdfPacInit(void *cb, void *callback);
void sdfPacSetFromTail(void *cb);
void *sdfFileCmdReadOpen(const char *aFileName, void *aCallback, void *aUserData);
void *sdfFileCmdWriteOpen(const char *aFileName, void *aCallback, void *aUserData, u32 aMode);
s32 sdfFileCmdFileSize(void *aHandle);
s32 sdfFileCmdRead(void *aHandle, void *aBuffer, s32 aLength);
s32 sdfFileCmdWrite(void *aHandle, void *aBuffer, s32 aLength);
s32 sdfFileCmdClose(void *aHandle);
s32 sdfFileCmdFree(void *aHandle);
void sdfFileCmdThreadSetHiPrio(void);
#ifdef FIX_BUGS
s32 DIntr(void);
s32 EIntr(void);
#endif
s32 sdfPrintf(const char *str, ...);
void *memset(void *a, s32 b, size_t c);
void sdfChipFreeDirect(void *aPtr);
void sdfPacFree(sdfPacDecodeCb_t *cb);
s32 sdfPacDecode(sdfPacDecodeCb_t *cb, const u8 *data, s32 length);
void *sdfMemAllocFromTailDirect(u32 aSize);
void fileWrite(fileCb_t *fileHandle);
void fileExecCallback(void);

s32 pacFileHandleFree(fileRequest_t *pacHandle)
{
    if (pacHandle->state == 6) {
        void *deviceRequest = pacHandle->dev;
        if (deviceRequest != NULL) {
            sdfFileCmdFree(deviceRequest);
        }
        sdfPacFree(&pacHandle->pac);
        sdfChipFreeDirect(pacHandle->name);
        sdfChipFreeDirect(pacHandle);
        return 0;
    }
    return 1;
}

s32 pacFileHandleFree_(void *pacHandle) {
    return pacFileHandleFree(pacHandle);
}

s32 fileHandleFree(fileRequest_t *fileHandle) {
    if (fileHandle->kind == 1) {
        return pacFileHandleFree(fileHandle);
    }
    if (fileHandle->state == 6) {
        if (fileHandle->dev != NULL) {
            sdfFileCmdFree((sdfFileCmd_t *)fileHandle->dev);
        }
        sdfChipFreeDirect(fileHandle->name);
        sdfChipFreeDirect(fileHandle);
        return 0;
    }
    return 1;
}

void fileLinkQueue(fileRequest_t *req, s32 kind, const char *name, void *callback, void *arg) {
    fileMngCb_tag *q = &fileQueueHead;
    char *dup;
    s32 first;

    req->kind = kind;
    dup = sdfStrdupChip(name);
    req->callback = (void (*)(fileRequest_t *, u32))callback;
    req->name = dup;
    req->arg = arg;

    WaitSema(q->semaId);
    if (q->queueTail == NULL) {
        q->queue = req;
        first = 1;
    } else {
        q->queueTail->next = req;
        first = 0;
    }
    q->queueTail = req;
    SignalSema(q->semaId);

    if (first != 0) {
        fileExecQueue();
    }
}

void fileUnlinkQueue(fileRequest_t *fileHandle) {
    fileMngCb_tag *work = &fileQueueHead;
    fileRequest_t *previousNode;
    fileRequest_t *currentNode;
    s32 slotIndex;

    WaitSema(work->semaId);
    for (slotIndex = 0; slotIndex != 4; slotIndex++) {
        if (work->data[slotIndex].req == fileHandle) {
            work->data[slotIndex].req = NULL;
        }
    }
    previousNode = NULL;
    currentNode = work->queue;
    while (currentNode != NULL) {
        if (currentNode == fileHandle) {
            if (currentNode->next == NULL) {
                work->queueTail = previousNode;
            }
            if (previousNode == NULL) {
                work->queue = currentNode->next;
            } else {
                previousNode->next = currentNode->next;
            }
            break;
        }
        previousNode = currentNode;
        currentNode = currentNode->next;
    }
    SignalSema(work->semaId);
}

void fileLinkCloseWait(fileMngCb_tag *cb, fileRequest_t *fileHandle) {
    fileHandle->next = cb->closeLink;
    cb->closeLink = fileHandle;
}

void fileUnlinkCloseWait(fileMngCb_tag *cb, fileRequest_t *fileHandle) {
    fileRequest_t **link = &cb->closeLink;
    fileRequest_t *p;

    while ((p = *link) != fileHandle) {
        link = &p->next;
    }
    *link = fileHandle->next;
}

fileRequest_t *pacLoadEx(const char *filename, u32 headOrTail, u32 callback, u32 pendCallback, u32 callbackParm) {
    fileRequest_t *req = sdfChipAllocZeroDirect(sizeof(fileRequest_t));

    sdfPacInit(&req->pac, EE_ADDR_AS(void *, callback));
    if (headOrTail != 0) {
        sdfPacSetFromTail(&req->pac);
    }
    fileLinkQueue(req, 1, filename, EE_ADDR_AS(void *, pendCallback), EE_ADDR_AS(void *, callbackParm));
    return req;
}

fileRequest_t *pacLoad(const char *filename) {
    return pacLoadEx(filename, 0, 0, 0, 0);
}

fileRequest_t *pacLoadUseTail(const char *filename) {
    return pacLoadEx(filename, 1, 0, 0, 0);
}

fileRequest_t *fileLoadEx(const char *filename, u32 headOrTail, addr_t endCallback, addr_t callbackParm) {
    fileRequest_t *req = sdfChipAllocZeroDirect(0x30);

    req->mode = headOrTail;
    fileLinkQueue(req, 0, filename, EE_ADDR_AS(void *, endCallback), EE_ADDR_AS(void *, callbackParm));
    return req;
}

fileRequest_t *fileLoad(const char *filename) {
    return fileLoadEx(filename, 0, 0, 0);
}

fileRequest_t *fileLoadUseTail(const char *filename) {
    return fileLoadEx(filename, 1, 0, 0);
}

sdfMemHandle_t *fileGetMemHandle(const fileRequest_t *fileHandle) {
    return fileHandle->handle;
}

u8 *fileGetLoadPtr(const fileRequest_t *fileHandle) {
    return EE_ADDR_AS(u8 *, fileHandle->top);
}

u32 fileGetLoadSize(const fileRequest_t *fileHandle) {
    return fileHandle->size;
}

u32 fileGetLoadRemain(fileRequest_t *fileHandle) {
    return fileHandle->rest;
}

s32 pacFileCheckEnd(const fileRequest_t *pacFileHandle) {
    s32 end;

    if (pacFileHandle->kind == 1) {
        end = 0;
        if (pacFileHandle->pacrest != 0) {
            end = pacFileHandle->state == 6;
        }
        return end;
    }
    return pacFileHandle->state == 6;
}

s32 pacCheckEnd(const fileRequest_t *pacHandle) {
    s32 end = 0;

    if (pacHandle->pacrest != 0) {
        end = pacHandle->state == 6;
    }
    return end;
}

void fileSync(fileRequest_t *fileHandle) {
    while (pacFileCheckEnd(fileHandle) == 0) {
        sdfFileCmdThreadSetHiPrio();
        fileManager();
    }
}

void pacSync(fileRequest_t *pacFileHandle) {
    fileSync(pacFileHandle);
}

void fileSyncAll(void) {
    fileMngCb_tag *work = &fileQueueHead;
    while (work->queue != 0 || work->closeLink != 0) {
        fileManager();
    }
}

fileRequest_t *fileSaveEx(const char *filename, const void *buff, s32 size, s32 endCallback, s32 callbackParm) {
    fileRequest_t *req = sdfChipAllocZeroDirect(0x30);

    req->top = EE_ADDR(buff);
    req->ptr = EE_ADDR(buff);
    req->size = size;
    req->rest = size;
    fileLinkQueue(req, 2, filename, EE_ADDR_AS(void *, endCallback), EE_ADDR_AS(void *, callbackParm));
    return req;
}

fileRequest_t *fileSave(const char *filename, const void *buff, s32 size) {
    return fileSaveEx(filename, buff, size, 0, 0);
}

void pacRead(fileRequest_t *pacHandle) {
    fileMngCb_tag *work = &fileQueueHead;
    u8 slotIndex;
    s32 chunkBytes;

    WaitSema(work->semaId);
    if (work->freeCount == 0) {
        SignalSema(work->semaId);
        return;
    }
    if (pacHandle->state != 3) {
        SignalSema(work->semaId);
        return;
    }
    pacHandle->state = 4;
    slotIndex = work->readIndex;
    if (slotIndex == 3) {
        work->readIndex = 0;
    } else {
        work->readIndex = slotIndex + 1;
    }
    pacHandle->slot = slotIndex;
    work->freeCount--;
    work->data[slotIndex].req = pacHandle;
    chunkBytes = pacHandle->rest;
    if (chunkBytes > 0x10000) {
        chunkBytes = 0x10000;
    }
    SignalSema(work->semaId);
    sdfFileCmdRead((sdfFileCmd_t *)pacHandle->dev,
                    work->buffer + (slotIndex << 16), chunkBytes);
}

s32 pacLoadCallback(void *fileHandle, s32 cause, s32 buffer, s32 length, fileCb_t *data) {
    fileMngCb_tag *work = &fileQueueHead;
    s32 saved;

    switch (cause) {
    case 2:
        data->dev = fileHandle;
        data->state = 2;
        sdfFileCmdFileSize(fileHandle);
        break;
    case 4:
        data->rest = length;
        data->state = 3;
        data->size = length;
        pacRead((fileRequest_t *)data);
        break;
    case 5:
        WaitSema(work->semaId);
        if (data->pacrest != 0) {
            data->state = 5;
        } else {
            work->data[data->slot].size = length;
            data->rest -= length;
            if (data->rest == 0) {
                data->state = 5;
                work->queue = (fileRequest_t *)data->next;
                if (data->next == NULL) {
                    work->queueTail = NULL;
                }
            } else {
                data->state = 3;
            }
        }
        work->activeCount++;
        if (data->state == 5) {
            fileLinkCloseWait(work, (fileRequest_t *)data);
            SignalSema(work->semaId);
            sdfFileCmdClose(fileHandle);
        } else {
            SignalSema(work->semaId);
        }
        saved = fileCallbackStatus;
        fileCallbackStatus = 1;
        fileExecQueue();
        fileCallbackStatus = saved;
        break;
    case 7:
        WaitSema(work->semaId);
        fileUnlinkCloseWait(work, (fileRequest_t *)data);
        SignalSema(work->semaId);
        sdfFileCmdFree(fileHandle);
        data->dev = NULL;
        data->state = 6;
        break;
    }
    return 0;
}

void fileRead(fileCb_t *fileHandle) {
    fileMngCb_tag *q = &fileQueueHead;

    WaitSema(q->semaId);
    if (fileHandle->state != 3) {
        SignalSema(q->semaId);
        return;
    }
    fileHandle->state = 4;
    SignalSema(q->semaId);
    sdfFileCmdRead(fileHandle->dev, EE_ADDR_AS(void *, fileHandle->ptr), fileHandle->rest <= 0x8000 ? fileHandle->rest : 0x8000);
}

s32 fileLoadCallback(void *fileHandle, s32 cause, s32 buffer, s32 length, fileCb_t *data) {
    fileMngCb_tag *work = &fileQueueHead;
    s32 saved;

    switch (cause) {
    case 2:
        data->dev = fileHandle;
        data->state = cause;
        sdfFileCmdFileSize(fileHandle);
        break;
    case 4: {
        sdfMemHandle_t *handle;

        data->rest = length;
        data->size = length;
        if (data->mode == 0) {
            handle = sdfMemAlloc2Direct(length);
            data->handle = handle;
            if (handle == NULL) {
                data->retry = 10;
                data->state = 7;
                break;
            }
        } else {
            handle = sdfMemAllocFromTailDirect(length);
            data->handle = handle;
        }
        {
            u32 address = sdfMemLock(handle);

            data->ptr = address;
            data->top = address;
            data->state = 3;
        }
        fileRead(data);
        break;
    }
    case 5:
        WaitSema(work->semaId);
        data->rest -= length;
        if (data->rest == 0) {
            data->state = 5;
            if (work->queue == (fileRequest_t *)data) {
                work->queue = (fileRequest_t *)data->next;
                if (data->next == NULL) {
                    work->queueTail = NULL;
                }
            }
        } else {
            data->state = 3;
            data->ptr += length;
        }
        if (data->state == 5) {
            fileLinkCloseWait(work, (fileRequest_t *)data);
            SignalSema(work->semaId);
            sdfFileCmdClose(fileHandle);
            saved = fileCallbackStatus;
            fileCallbackStatus = 2;
            fileExecQueue();
            fileCallbackStatus = saved;
        } else {
            SignalSema(work->semaId);
            fileRead(data);
        }
        break;
    case 7:
        sdfFileCmdFree(fileHandle);
        data->dev = NULL;
        data->state = 6;
        WaitSema(work->semaId);
        fileUnlinkCloseWait(work, (fileRequest_t *)data);
        if (data->callback != NULL) {
            fileCb_t **link = &work->endCallbackLink;
            fileCb_t *next;

            while ((next = *link) != NULL) {
                link = &next->donenext;
            }
            *link = data;
        }
        SignalSema(work->semaId);
        break;
    }
    return 0;
}

void fileWrite(fileCb_t *fileHandle) {
    fileMngCb_tag *q = &fileQueueHead;

    WaitSema(q->semaId);
    if (fileHandle->state != 3) {
        SignalSema(q->semaId);
        return;
    }
    fileHandle->state = 4;
    SignalSema(q->semaId);
    sdfFileCmdWrite(fileHandle->dev, EE_ADDR_AS(void *, fileHandle->ptr), fileHandle->rest <= 0x8000 ? fileHandle->rest : 0x8000);
}

s32 fileSaveCallback(void *fileHandle, s32 cause, s32 buffer, s32 length, fileCb_t *data) {
    fileMngCb_tag *work = &fileQueueHead;
    s32 saved;

    switch (cause) {
    case 2:
        data->state = 3;
        fileWrite(data);
        break;
    case 6:
        WaitSema(work->semaId);
        data->ptr += length;
        data->rest -= length;
        if (data->rest == 0) {
            data->state = 5;
            if (work->queue == (fileRequest_t *)data) {
                work->queue = (fileRequest_t *)data->next;
                if (data->next == NULL) {
                    work->queueTail = NULL;
                }
            }
        } else {
            data->state = 3;
        }
        if (data->state == 5) {
            fileLinkCloseWait(work, (fileRequest_t *)data);
            SignalSema(work->semaId);
            sdfFileCmdClose(fileHandle);
            saved = fileCallbackStatus;
            fileCallbackStatus = 3;
            fileExecQueue();
            fileCallbackStatus = saved;
        } else {
            SignalSema(work->semaId);
            fileWrite(data);
        }
        break;
    case 7:
        sdfFileCmdFree(fileHandle);
        data->dev = NULL;
        data->state = 6;
        WaitSema(work->semaId);
        fileUnlinkCloseWait(work, (fileRequest_t *)data);
        if (data->callback != NULL) {
            fileCb_t **link = &work->endCallbackLink;
            fileCb_t *next;

            while ((next = *link) != NULL) {
                link = &next->donenext;
            }
            *link = data;
        }
        SignalSema(work->semaId);
        break;
    }
    return 0;
}

void fileExecQueue(void) {
    fileMngCb_tag *work = &fileQueueHead;
    fileCb_t *job;
    s32 locked;

    WaitSema(work->semaId);
    job = (fileCb_t *)work->queue;
    if (job == NULL) {
        SignalSema(work->semaId);
        return;
    }
    locked = 1;
    switch (job->kind) {
    case 0:
        switch (job->state) {
        case 0:
            job->state = 1;
            locked = 0;
            SignalSema(work->semaId);
            job->dev = sdfFileCmdReadOpen(job->name, fileLoadCallback, job);
            break;
        case 7:
            job->handle = sdfMemAlloc2Direct(job->size);
            if (job->handle == NULL) {
                sdfPrintf("alloc retry for %s\n",
                                            ((sdfFileCmd_t *)job->dev)->path);
                job->retry--;
            } else {
                u32 address = sdfMemLock(job->handle);

                job->ptr = address;
                job->top = address;
                job->state = 3;
                fileRead(job);
            }
            break;
        }
        break;
    case 1:
        switch (job->state) {
        case 0:
            job->state = 1;
            locked = 0;
            SignalSema(work->semaId);
#ifdef FIX_BUGS
            /* Retail's file thread runs at lower priority, so the callback that frees
             * the request cannot run before dev is stored. Hold it off here. */
            {
                s32 intrEnabled = DIntr();

                job->dev = sdfFileCmdReadOpen(job->name, pacLoadCallback, job);
                if (intrEnabled != 0) {
                    EIntr();
                }
            }
#else
            job->dev = sdfFileCmdReadOpen(job->name, pacLoadCallback, job);
#endif
            break;
        case 3:
            locked = 0;
            SignalSema(work->semaId);
            pacRead((fileRequest_t *)job);
            break;
        }
        break;
    case 2:
        if (job->state == 0) {
            job->state = 1;
            locked = 0;
            SignalSema(work->semaId);
#ifdef FIX_BUGS
            /* Retail's file thread runs at lower priority, so the callback that frees
             * the request cannot run before dev is stored. Hold it off here. */
            {
                s32 intrEnabled = DIntr();

                job->dev = sdfFileCmdWriteOpen(job->name, fileSaveCallback, job, 0x180);
                if (intrEnabled != 0) {
                    EIntr();
                }
            }
#else
            job->dev = sdfFileCmdWriteOpen(job->name, fileSaveCallback, job, 0x180);
#endif
        }
        break;
    }
    if (locked != 0) {
        SignalSema(work->semaId);
    }
}

void fileExecCallback(void) {
    fileMngCb_tag *work = &fileQueueHead;
    fileCb_t *node;

    WaitSema(work->semaId);
    while ((node = work->endCallbackLink) != NULL) {
        work->endCallbackLink = node->donenext;
        SignalSema(work->semaId);
        node->callback(node, EE_ADDR(node->arg));
        WaitSema(work->semaId);
    }
    SignalSema(work->semaId);
}

s32 fileManager(void) {
    fileMngCb_tag *work = &fileQueueHead;
    u32 fullcnt;
    pacFileRequest_t **link;
    pacFileRequest_t *req;

    fileExecCallback();
    fullcnt = work->activeCount;
    while (fullcnt != 0) {
        u32 slot = work->index;
        const u8 *input = work->buffer + (slot << 16);
        fileCb_t *job = (fileCb_t *)work->data[slot].req;
        u8 kind = job->kind;

        if (kind == 1) {
            pacFileRequest_t *req = (pacFileRequest_t *)job;
            if (req != NULL &&
                sdfPacDecode(&req->pac, input, work->data[slot].size) == -1) {
                fileUnlinkQueue((fileRequest_t *)req);
                req->pacrest = kind;
                if (req->callback != NULL) {
                    pacFileRequest_t **append = &work->pacEndCallbackLink;
                    pacFileRequest_t *next;

                    while ((next = *append) != NULL) {
                        append = &next->cbnext;
                    }
                    *append = req;
                }
            }
            WaitSema(work->semaId);
            slot++;
            if (slot == 4) {
                slot = 0;
            }
            work->index = slot;
            work->freeCount++;
            work->activeCount--;
            SignalSema(work->semaId);
            fullcnt = work->activeCount;
        }
    }
    fileExecQueue();
    link = &work->pacEndCallbackLink;
    while ((req = *link) != NULL) {
        if (req->state == 6) {
            *link = req->cbnext;
            ((void (*)(pacFileRequest_t *, void *))req->callback)(req, req->arg);
        } else {
            link = &req->cbnext;
        }
    }
    return work->queue != NULL;
}

s32 fileManagerProc(dds3ProcessID_t *procId) {
    fileManager();
    return 0;
}

void fileSysInit(void) {
    memset(&fileQueueHead, 0, 0x40);
    fileQueueHead.freeCount = 4;
    fileQueueHead.semaId = sdfCreateSema(1, 0x7F, 0);
    fileQueueHead.buffer = EE_ADDR_AS(u8 *, sdfMemLock(sdfMemAllocDirect(0x40000)));
    dds3StartProcess(fileManagerProcName, 0x384, 1, 0, fileManagerProc, 0, 0);
    p2sdSmgBuf = fileManager;
}

fileMngCb_tag fileQueueHead __attribute__((section(".bss")));

u32 fileCallbackStatus __attribute__((section(".sdata"))) = 0;

char fileManagerProcName[8] __attribute__((section(".sdata"))) = "fileMan";
