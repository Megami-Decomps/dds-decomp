#include "sdk_ps2.h"
#include "common.h"
#include "sdf_semaphore_api.h"
#include "kwln_task_create.h"
#include "sdf_resource.h"
#include "file.h"
#include "file_slot_flags.h"
#include "file_request_entry.h"
#include "file_manager_api.h"
#include "file_transfer_job.h"
#include "file_pac.h"
#include "sdf_dev_event.h"
#include "sdf_dev_state.h"

extern s32 D_00437CC0;



extern char fileManagerTaskName[];

extern s32 (*p2sdSmgBuf)(void);
extern s32 sdfFileCmdFileSize(sdfFileCmdHandle_t *);

extern void pacRead(FileTransferJob *);

extern void fileLinkCloseWait(FileManWork *work, FileRequest *job);

extern void fileUnlinkCloseWait(FileManWork *work, FileRequest *job);

extern sdfFileCmdHandle_t *sdfFileCmdReadOpen(const char *path,
                        s32 (*callback)(void *, s32, s32, s32, FileTransferJob *), FileTransferJob *job);

extern sdfFileCmdHandle_t *sdfFileCmdWriteOpen(const char *path,
                        s32 (*callback)(void *, s32, s32, s32, FileTransferJob *), FileTransferJob *job,
                        s32 options);

extern s32 sdfPrintf(const char *fmt, ...);

s32 pacLoadCallback(void *deviceRequest, s32 event, s32 unused, s32 byteCount, FileTransferJob *job) {
    FileManWork *work = &fileQueueHead;
    s32 saved;

    switch (event) {
    case SDF_DEV_EVENT_OPENED:
        job->deviceRequest = deviceRequest;
        job->state = 2;
        sdfFileCmdFileSize(deviceRequest);
        break;
    case SDF_DEV_EVENT_SIZE_REPLY:
        job->transferBytes = byteCount;
        job->state = FILE_JOB_READY;
        job->totalBytes = byteCount;
        pacRead(job);
        break;
    case SDF_DEV_EVENT_READ_REPLY:
        WaitSema(work->sema);
        if (job->readinessEnabled != 0) {
            job->state = 5;
        } else {
            work->slots[job->slot].value = byteCount;
            job->transferBytes -= byteCount;
            if (job->transferBytes == 0) {
                job->state = 5;
                work->head = (struct FileRequest *)job->next;
                if (job->next == NULL) {
                    work->tail = NULL;
                }
            } else {
                job->state = FILE_JOB_READY;
            }
        }
        work->activeSlots++;
        if (job->state == 5) {
            fileLinkCloseWait(work, (FileRequest *)job);
            SignalSema(work->sema);
            sdfFileCmdClose(deviceRequest);
        } else {
            SignalSema(work->sema);
        }
        saved = D_00437CC0;
        D_00437CC0 = 1;
        fileExecQueue();
        D_00437CC0 = saved;
        break;
    case SDF_DEV_EVENT_CLOSED:
        WaitSema(work->sema);
        fileUnlinkCloseWait(work, (FileRequest *)job);
        SignalSema(work->sema);
        sdfFileCmdFree(deviceRequest);
        job->deviceRequest = NULL;
        job->state = 6;
        break;
    }
    return 0;
}

void fileRead(FileTransferJob *job) {
    WaitSema(fileQueueHead.sema);
    if (job->state != FILE_JOB_READY) {
        SignalSema(fileQueueHead.sema);
        return;
    }
    job->state = FILE_JOB_TRANSFERRING;
    SignalSema(fileQueueHead.sema);
    sdfFileCmdRead(job->deviceRequest, job->transferAddress,
                    job->transferBytes <= FILE_IO_MAX_CHUNK_BYTES ? job->transferBytes : FILE_IO_MAX_CHUNK_BYTES);
}

/* Drive allocation, chunked reads, close, and completion-list handoff. */
s32 fileLoadCallback(void *deviceRequest, s32 event, s32 unused, s32 byteCount, FileTransferJob *job) {
    FileManWork *work = &fileQueueHead;
    s32 saved;

    switch (event) {
    case SDF_DEV_EVENT_OPENED:
        job->deviceRequest = deviceRequest;
        job->state = event;
        sdfFileCmdFileSize(deviceRequest);
        break;
    case SDF_DEV_EVENT_SIZE_REPLY: {
        struct sdfMemHandle_t *allocationHandle;

        job->transferBytes = byteCount;
        job->totalBytes = byteCount;
        if (job->allocationMode == 0) {
            allocationHandle = sdfMemAlloc2Direct(byteCount);
            job->allocationHandle = allocationHandle;
            if (allocationHandle == NULL) {
                job->retryCount = 10;
                job->state = 7;
                break;
            }
        } else {
            allocationHandle = sdfMemAllocFromTailDirect(byteCount);
            job->allocationHandle = allocationHandle;
        }
        {
            u8 *address = (u8 *)sdfMemLock(allocationHandle);

            job->transferAddress = address;
            job->retainedAddress = address;
            job->state = FILE_JOB_READY;
        }
        fileRead(job);
        break;
    }
    case SDF_DEV_EVENT_READ_REPLY:
        WaitSema(work->sema);
        job->transferBytes -= byteCount;
        if (job->transferBytes == 0) {
            job->state = 5;
            if (work->head == (struct FileRequest *)job) {
                work->head = (struct FileRequest *)job->next;
                if (job->next == NULL) {
                    work->tail = NULL;
                }
            }
        } else {
            job->state = FILE_JOB_READY;
            job->transferAddress += byteCount;
        }
        if (job->state == 5) {
            fileLinkCloseWait(work, (FileRequest *)job);
            SignalSema(work->sema);
            sdfFileCmdClose(deviceRequest);
            saved = D_00437CC0;
            D_00437CC0 = 2;
            fileExecQueue();
            D_00437CC0 = saved;
        } else {
            SignalSema(work->sema);
            fileRead(job);
        }
        break;
    case SDF_DEV_EVENT_CLOSED:
        sdfFileCmdFree(deviceRequest);
        job->deviceRequest = NULL;
        job->state = 6;
        WaitSema(work->sema);
        fileUnlinkCloseWait(work, (FileRequest *)job);
        if (job->completionCallback != NULL) {
            FileTransferJob **link = &work->done;
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

void fileWrite(FileTransferJob *job) {
    WaitSema(fileQueueHead.sema);
    if (job->state != FILE_JOB_READY) {
        SignalSema(fileQueueHead.sema);
        return;
    }
    job->state = FILE_JOB_TRANSFERRING;
    SignalSema(fileQueueHead.sema);
    sdfFileCmdWrite(job->deviceRequest, job->transferAddress,
                     job->transferBytes <= FILE_IO_MAX_CHUNK_BYTES ? job->transferBytes : FILE_IO_MAX_CHUNK_BYTES);
}

/* Advance a chunked write, then close and queue its completion callback. */
s32 fileSaveCallback(void *deviceRequest, s32 event, s32 unused, s32 byteCount, FileTransferJob *job) {
    FileManWork *work = &fileQueueHead;
    s32 saved;

    switch (event) {
    case SDF_DEV_EVENT_OPENED:
        job->state = FILE_JOB_READY;
        fileWrite(job);
        break;
    case SDF_DEV_EVENT_WRITE_REPLY:
        WaitSema(work->sema);
        job->transferAddress += byteCount;
        job->transferBytes -= byteCount;
        if (job->transferBytes == 0) {
            job->state = 5;
            if (work->head == (struct FileRequest *)job) {
                work->head = (struct FileRequest *)job->next;
                if (job->next == NULL) {
                    work->tail = NULL;
                }
            }
        } else {
            job->state = FILE_JOB_READY;
        }
        if (job->state == 5) {
            fileLinkCloseWait(work, (FileRequest *)job);
            SignalSema(work->sema);
            sdfFileCmdClose(deviceRequest);
            saved = D_00437CC0;
            D_00437CC0 = 3;
            fileExecQueue();
            D_00437CC0 = saved;
        } else {
            SignalSema(work->sema);
            fileWrite(job);
        }
        break;
    case SDF_DEV_EVENT_CLOSED:
        sdfFileCmdFree(deviceRequest);
        job->deviceRequest = NULL;
        job->state = 6;
        WaitSema(work->sema);
        fileUnlinkCloseWait(work, (FileRequest *)job);
        if (job->completionCallback != NULL) {
            FileTransferJob **link = &work->done;
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
void fileExecQueue(void) {
    FileManWork *work = &fileQueueHead;
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
            job->deviceRequest = sdfFileCmdReadOpen(job->name, fileLoadCallback, job);
            break;
        case 7:
            job->allocationHandle = sdfMemAlloc2Direct(job->totalBytes);
            if (job->allocationHandle == NULL) {
                sdfPrintf("alloc retry for %s\n",
                                            job->deviceRequest->resolvedPath);
                job->retryCount--;
            } else {
                u8 *address = (u8 *)sdfMemLock(job->allocationHandle);

                job->transferAddress = address;
                job->retainedAddress = address;
                job->state = FILE_JOB_READY;
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
            SignalSema(work->sema);
            job->deviceRequest = sdfFileCmdReadOpen(job->name, pacLoadCallback, job);
            break;
        case FILE_JOB_READY:
            locked = 0;
            SignalSema(work->sema);
            pacRead(job);
            break;
        }
        break;
    case 2:
        if (job->state == 0) {
            job->state = 1;
            locked = 0;
            SignalSema(work->sema);
            job->deviceRequest = sdfFileCmdWriteOpen(job->name, fileSaveCallback, job, 0x180);
        }
        break;
    }
    if (locked != 0) {
        SignalSema(work->sema);
    }
}

void fileExecCallback(void) {
    FileManWork *work = &fileQueueHead;
    FileTransferJob *node;

    WaitSema(work->sema);
    while ((node = work->done) != NULL) {
        work->done = node->completionNext;
        SignalSema(work->sema);
        node->completionCallback(node, (u32)node->completionArg);
        WaitSema(work->sema);
    }
    SignalSema(work->sema);
}

extern void fileUnlinkQueue(FileRequest *request);

/* Deliver callbacks queued after PAC input completes. */
s32 fileManager(void) {
    FileManWork *work = &fileQueueHead;
    u32 activeSlots;
    FilePacRequest **link;
    FilePacRequest *request;

    fileExecCallback();
    activeSlots = work->activeSlots;
    while (activeSlots != 0) {
        u32 slot = work->currentSlot;
        const u8 *input = work->buffer + (slot << 16);
        FileTransferJob *job = (FileTransferJob *)work->slots[slot].request;
        u8 kind = job->kind;

        if (kind == 1) {
            FilePacRequest *request = (FilePacRequest *)job;
            if (request != NULL &&
                sdfPacDecode(&request->packet, input, work->slots[slot].value) == -1) {
                fileUnlinkQueue((FileRequest *)request);
                request->readinessEnabled = kind;
                if (request->completionCallback != NULL) {
                    FilePacRequest **append = &work->pacCallbackHead;
                    FilePacRequest *next;

                    while ((next = *append) != NULL) {
                        append = &next->callbackNext;
                    }
                    *append = request;
                }
            }
            WaitSema(work->sema);
            slot++;
            if (slot == 4) {
                slot = 0;
            }
            work->currentSlot = slot;
            work->freeSlots++;
            work->activeSlots--;
            SignalSema(work->sema);
            activeSlots = work->activeSlots;
        }
    }
    fileExecQueue();
    link = &work->pacCallbackHead;
    while ((request = *link) != NULL) {
        if (request->state == 6) {
            *link = request->callbackNext;
            ((void (*)(FilePacRequest *, void *))request->completionCallback)(request, request->completionArg);
        } else {
            link = &request->callbackNext;
        }
    }
    return work->head != NULL;
}

/* Task callback driving asynchronous file work. */
s32 fileManagerProc(dds3ProcessID_t *task) {
    fileManager();
    return 0;
}

void fileSysInit(void) {
    memset(&fileQueueHead, 0, 0x40);
    fileQueueHead.freeSlots = 4;
    fileQueueHead.sema = sdfCreateSema(1, 0x7F, 0);
    fileQueueHead.buffer = (u8 *)sdfMemLock(sdfMemAllocDirect(0x40000));
    dds3StartProcess(fileManagerTaskName, 0x384, 1, 0, fileManagerProc, 0, 0);
    p2sdSmgBuf = fileManager;
}

s32 D_00437CC0 __attribute__((section(".sdata"))) = 0;

char fileManagerTaskName[8] __attribute__((section(".sdata"))) = "fileMan";
