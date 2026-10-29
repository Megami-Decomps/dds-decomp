#include "sdk_ps2.h"
#include "common.h"
#include "sdf_semaphore_api.h"
#include "kwln_task_create.h"
#include "kwln_task_runner.h"
#include "file_pac.h"
#include "sdf_pac_decode.h"
#include "sdf_resource.h"
#include "file.h"
#include "file_request_lifecycle.h"
#include "file_slot_flags.h"
#include "file_request_entry.h"
#include "file_request_kind.h"
#include "file_manager_api.h"
#include "file_transfer_job.h"
#include "sdf_dev_event.h"
#include "sdf_dev_state.h"

extern char fileManagerTaskName[];

extern s32 D_003BC7D8;

extern s32 (*p2sdSmgBuf)(void);
KwlnTaskUpdateResult fileManagerProc(dds3ProcessID_t *task);

extern s32 sdfFileCmdRequestFileSize(sdfFileCmdHandle_t *);

extern void pacRead(FileTransferJob *);

extern void fileLinkCloseWait(FileManWork *work, FileTransferJob *job);

extern void fileUnlinkCloseWait(FileManWork *work, FileTransferJob *job);

extern sdfFileCmdHandle_t *sdfFileCmdReadOpen(const char *path,
                        s32 (*callback)(sdfFileCmdHandle_t *, SdfDevEvent, s32, s32, FileTransferJob *), void *callbackContext);

extern sdfFileCmdHandle_t *sdfFileCmdWriteOpen(const char *path,
                        s32 (*callback)(sdfFileCmdHandle_t *, SdfDevEvent, s32, s32, FileTransferJob *), void *callbackContext,
                        s32 options);

extern s32 sdfPrintf(const char *fmt, ...);

s32 pacLoadCallback(sdfFileCmdHandle_t *deviceRequest, SdfDevEvent event, s32 unused, s32 byteCount, FileTransferJob *job) {
    FileManWork *work = &fileQueueHead;
    s32 saved;

    switch (event) {
    case SDF_DEV_EVENT_OPENED:
        job->deviceRequest = deviceRequest;
        job->state = FILE_REQUEST_LIFECYCLE_OPENED;
        sdfFileCmdRequestFileSize(deviceRequest);
        break;
    case SDF_DEV_EVENT_SIZE_REPLY:
        job->transferBytes = byteCount;
        job->state = FILE_REQUEST_LIFECYCLE_READY;
        job->totalBytes = byteCount;
        pacRead(job);
        break;
    case SDF_DEV_EVENT_READ_REPLY:
        WaitSema(work->sema);
        if (job->pacDecodeComplete != 0) {
            job->state = FILE_REQUEST_LIFECYCLE_CLOSING;
        } else {
            work->slots[job->slot].value = byteCount;
            job->transferBytes -= byteCount;
            if (job->transferBytes == 0) {
                job->state = FILE_REQUEST_LIFECYCLE_CLOSING;
                work->head = (struct FileRequest *)job->next;
                if (job->next == NULL) {
                    work->tail = NULL;
                }
            } else {
                job->state = FILE_REQUEST_LIFECYCLE_READY;
            }
        }
        work->activeSlots++;
        if (job->state == FILE_REQUEST_LIFECYCLE_CLOSING) {
            fileLinkCloseWait(work, job);
            SignalSema(work->sema);
            sdfFileCmdClose(deviceRequest);
        } else {
            SignalSema(work->sema);
        }
        saved = D_003BC7D8;
        D_003BC7D8 = 1;
        fileExecQueue();
        D_003BC7D8 = saved;
        break;
    case SDF_DEV_EVENT_CLOSED:
        WaitSema(work->sema);
        fileUnlinkCloseWait(work, job);
        SignalSema(work->sema);
        sdfFileCmdFree(deviceRequest);
        job->deviceRequest = NULL;
        job->state = FILE_REQUEST_LIFECYCLE_CLOSED;
        break;
    }
    return 0;
}

void fileRead(FileTransferJob *job) {
    WaitSema(fileQueueHead.sema);
    if (job->state != FILE_REQUEST_LIFECYCLE_READY) {
        SignalSema(fileQueueHead.sema);
        return;
    }
    job->state = FILE_REQUEST_LIFECYCLE_TRANSFERRING;
    SignalSema(fileQueueHead.sema);
    sdfFileCmdRead(job->deviceRequest, job->transferAddress,
                    job->transferBytes <= FILE_IO_MAX_CHUNK_BYTES ? job->transferBytes : FILE_IO_MAX_CHUNK_BYTES);
}

/* Drive allocation, chunked reads, close, and completion-list handoff. */
s32 fileLoadCallback(sdfFileCmdHandle_t *deviceRequest, SdfDevEvent event, s32 unused, s32 byteCount, FileTransferJob *job) {
    FileManWork *work = &fileQueueHead;
    s32 saved;

    switch (event) {
    case SDF_DEV_EVENT_OPENED:
        job->deviceRequest = deviceRequest;
        job->state = FILE_REQUEST_LIFECYCLE_OPENED;
        sdfFileCmdRequestFileSize(deviceRequest);
        break;
    case SDF_DEV_EVENT_SIZE_REPLY: {
        struct sdfMemHandle_t *allocationHandle;

        job->transferBytes = byteCount;
        job->totalBytes = byteCount;
        if (job->allocationMode == 0) {
            allocationHandle = sdfMemTryAllocDirect(byteCount);
            job->allocationHandle = allocationHandle;
            if (allocationHandle == NULL) {
                job->retryCount = 10;
                job->state = FILE_REQUEST_LIFECYCLE_ALLOCATION_RETRY;
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
            job->state = FILE_REQUEST_LIFECYCLE_READY;
        }
        fileRead(job);
        break;
    }
    case SDF_DEV_EVENT_READ_REPLY:
        WaitSema(work->sema);
        job->transferBytes -= byteCount;
        if (job->transferBytes == 0) {
            job->state = FILE_REQUEST_LIFECYCLE_CLOSING;
            if (work->head == (struct FileRequest *)job) {
                work->head = (struct FileRequest *)job->next;
                if (job->next == NULL) {
                    work->tail = NULL;
                }
            }
        } else {
            job->state = FILE_REQUEST_LIFECYCLE_READY;
            job->transferAddress += byteCount;
        }
        if (job->state == FILE_REQUEST_LIFECYCLE_CLOSING) {
            fileLinkCloseWait(work, job);
            SignalSema(work->sema);
            sdfFileCmdClose(deviceRequest);
            saved = D_003BC7D8;
            D_003BC7D8 = 2;
            fileExecQueue();
            D_003BC7D8 = saved;
        } else {
            SignalSema(work->sema);
            fileRead(job);
        }
        break;
    case SDF_DEV_EVENT_CLOSED:
        sdfFileCmdFree(deviceRequest);
        job->deviceRequest = NULL;
        job->state = FILE_REQUEST_LIFECYCLE_CLOSED;
        WaitSema(work->sema);
        fileUnlinkCloseWait(work, job);
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
    if (job->state != FILE_REQUEST_LIFECYCLE_READY) {
        SignalSema(fileQueueHead.sema);
        return;
    }
    job->state = FILE_REQUEST_LIFECYCLE_TRANSFERRING;
    SignalSema(fileQueueHead.sema);
    sdfFileCmdWrite(job->deviceRequest, job->transferAddress,
                     job->transferBytes <= FILE_IO_MAX_CHUNK_BYTES ? job->transferBytes : FILE_IO_MAX_CHUNK_BYTES);
}

/* Advance a chunked write, then close and queue its completion callback. */
s32 fileSaveCallback(sdfFileCmdHandle_t *deviceRequest, SdfDevEvent event, s32 unused, s32 byteCount, FileTransferJob *job) {
    FileManWork *work = &fileQueueHead;
    s32 saved;

    switch (event) {
    case SDF_DEV_EVENT_OPENED:
        job->state = FILE_REQUEST_LIFECYCLE_READY;
        fileWrite(job);
        break;
    case SDF_DEV_EVENT_WRITE_REPLY:
        WaitSema(work->sema);
        job->transferAddress += byteCount;
        job->transferBytes -= byteCount;
        if (job->transferBytes == 0) {
            job->state = FILE_REQUEST_LIFECYCLE_CLOSING;
            if (work->head == (struct FileRequest *)job) {
                work->head = (struct FileRequest *)job->next;
                if (job->next == NULL) {
                    work->tail = NULL;
                }
            }
        } else {
            job->state = FILE_REQUEST_LIFECYCLE_READY;
        }
        if (job->state == FILE_REQUEST_LIFECYCLE_CLOSING) {
            fileLinkCloseWait(work, job);
            SignalSema(work->sema);
            sdfFileCmdClose(deviceRequest);
            saved = D_003BC7D8;
            D_003BC7D8 = 3;
            fileExecQueue();
            D_003BC7D8 = saved;
        } else {
            SignalSema(work->sema);
            fileWrite(job);
        }
        break;
    case SDF_DEV_EVENT_CLOSED:
        sdfFileCmdFree(deviceRequest);
        job->deviceRequest = NULL;
        job->state = FILE_REQUEST_LIFECYCLE_CLOSED;
        WaitSema(work->sema);
        fileUnlinkCloseWait(work, job);
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
    case FILE_REQUEST_KIND_FILE_LOAD:
        switch (job->state) {
        case FILE_REQUEST_LIFECYCLE_QUEUED:
            job->state = FILE_REQUEST_LIFECYCLE_OPENING;
            locked = 0;
            SignalSema(work->sema);
            job->deviceRequest = sdfFileCmdReadOpen(job->name, fileLoadCallback, job);
            break;
        case FILE_REQUEST_LIFECYCLE_ALLOCATION_RETRY:
            job->allocationHandle = sdfMemTryAllocDirect(job->totalBytes);
            if (job->allocationHandle == NULL) {
                sdfPrintf("alloc retry for %s\n",
                                            job->deviceRequest->resolvedPath);
                job->retryCount--;
            } else {
                u8 *address = (u8 *)sdfMemLock(job->allocationHandle);

                job->transferAddress = address;
                job->retainedAddress = address;
                job->state = FILE_REQUEST_LIFECYCLE_READY;
                fileRead(job);
            }
            break;
        }
        break;
    case FILE_REQUEST_KIND_PAC_LOAD:
        switch (job->state) {
        case FILE_REQUEST_LIFECYCLE_QUEUED:
            job->state = FILE_REQUEST_LIFECYCLE_OPENING;
            locked = 0;
            SignalSema(work->sema);
            job->deviceRequest = sdfFileCmdReadOpen(job->name, pacLoadCallback, job);
            break;
        case FILE_REQUEST_LIFECYCLE_READY:
            locked = 0;
            SignalSema(work->sema);
            pacRead(job);
            break;
        }
        break;
    case FILE_REQUEST_KIND_FILE_SAVE:
        if (job->state == FILE_REQUEST_LIFECYCLE_QUEUED) {
            job->state = FILE_REQUEST_LIFECYCLE_OPENING;
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
    u32 pendingReadReplies;
    FilePacRequest **link;
    FilePacRequest *request;

    fileExecCallback();
    pendingReadReplies = work->activeSlots;
    while (pendingReadReplies != 0) {
        u32 slot = work->currentSlot;
        const u8 *input = work->buffer + (slot << 16);
        FileTransferJob *job = (FileTransferJob *)work->slots[slot].request;
        u8 kind = job->kind;

        if (kind == FILE_REQUEST_KIND_PAC_LOAD) {
            FilePacRequest *request = (FilePacRequest *)job;
            if (request != NULL &&
                sdfPacDecode(&request->packet, input, work->slots[slot].value) == SDF_PAC_PHASE_COMPLETE) {
                fileUnlinkQueue((FileRequest *)request);
                request->pacDecodeComplete = kind;
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
            pendingReadReplies = work->activeSlots;
        }
    }
    fileExecQueue();
    link = &work->pacCallbackHead;
    while ((request = *link) != NULL) {
        if (((FileTransferJob *)request)->state == FILE_REQUEST_LIFECYCLE_CLOSED) {
            *link = request->callbackNext;
            ((void (*)(FilePacRequest *, void *))request->completionCallback)(request, request->completionArg);
        } else {
            link = &request->callbackNext;
        }
    }
    return work->head != NULL;
}

/* Task callback driving asynchronous file work. */
KwlnTaskUpdateResult fileManagerProc(dds3ProcessID_t *task) {
    fileManager();
    return KWLN_TASK_UPDATE_RETAIN_CALLBACK;
}

void fileSysInit(void) {
    memset(&fileQueueHead, 0, 0x40);
    fileQueueHead.freeSlots = 4;
    fileQueueHead.sema = sdfCreateSema(1, 0x7F, 0);
    fileQueueHead.buffer = (u8 *)sdfMemLock(sdfMemAllocDirect(0x40000));
    dds3StartProcess(fileManagerTaskName, 0x384, 1, 0, fileManagerProc, 0, 0);
    p2sdSmgBuf = fileManager;
}

s32 D_003BC7D8 __attribute__((section(".sdata"))) = 0;

char fileManagerTaskName[8] __attribute__((section(".sdata"))) = "fileMan";
