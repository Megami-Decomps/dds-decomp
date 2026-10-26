#include "sdk_ps2.h"
#include "sdf_chip.h"
#include "file.h"
#include "sdf_pac_state.h"
#include "file_request_cleanup_api.h"
#include "file_pac_completion_api.h"
#include "sdf_dev_state.h"
#include "file_manager_api.h"
#include "file_request.h"
#include "file_request_kind.h"
#include "file_transfer_job.h"

FileManWork fileQueueHead __attribute__((section(".bss")));

extern s32 pacFileHandleFree(struct FilePacRequest *);



#define FILE_READ_SLOT_COUNT 4
#define FILE_LAST_READ_SLOT 3
#define FILE_READ_SLOT_BYTES 0x10000
#define FILE_READ_SLOT_SHIFT 16

/* PAC requests delegate cleanup. Other kinds remain pending until state six,
 * then release any device state, duplicated name and entry. The request is required. */
FileRequestCleanupResult fileRequestFreeIfComplete(struct FileRequest *request) {
    if (request->kind == FILE_REQUEST_KIND_PAC) {
        return pacFileHandleFree((struct FilePacRequest *)request);
    }
    if (request->state == FILE_REQUEST_STATE_COMPLETE) {
        if (request->deviceRequest != NULL) {
            sdfFileCmdFree(request->deviceRequest);
        }
        sdfChipFreeDirect(request->name);
        sdfChipFreeDirect(request);
        return FILE_REQUEST_CLEANUP_FREED;
    }
    return FILE_REQUEST_CLEANUP_PENDING;
}

extern char *sdfStrdupChip(const char *text);

/* Duplicate the request name, store its kind in the byte field, and append
 * under the semaphore. Start processing only after an empty-to-nonempty transition.
 * The caller supplies initialized next linkage; this function does not clear it. */
void fileLinkQueue(FileRequest *request, FileRequestKind kind, const char *requestName,
                   void *callback, void *userData) {
    FileManWork *work;
    char *duplicatedName;
    s32 startsQueue;

    work = &fileQueueHead;
    request->kind = kind;
    duplicatedName = sdfStrdupChip(requestName);
    request->completionCallback = callback;
    request->name = duplicatedName;
    request->completionArg = userData;

    WaitSema(work->sema);
    if (work->tail == NULL) {
        work->head = request;
        startsQueue = 1;
    } else {
        work->tail->next = request;
        startsQueue = 0;
    }
    work->tail = request;
    SignalSema(work->sema);

    if (startsQueue != 0) {
        fileExecQueue();
    }
}

/* Clear every matching read slot and unlink the first queued occurrence under
 * the semaphore. No request/name memory is freed; absence from the queue is OK. */
void fileUnlinkQueue(FileRequest *node) {
    FileManWork *work = &fileQueueHead;
    FileRequest *previousNode;
    FileRequest *currentNode;
    s32 slotIndex;

    WaitSema(work->sema);
    for (slotIndex = 0; slotIndex != FILE_READ_SLOT_COUNT; slotIndex++) {
        if (work->slots[slotIndex].request == node) {
            work->slots[slotIndex].request = NULL;
        }
    }
    previousNode = NULL;
    currentNode = work->head;
    while (currentNode != NULL) {
        if (currentNode == node) {
            if (currentNode->next == NULL) {
                work->tail = previousNode;
            }
            if (previousNode == NULL) {
                work->head = currentNode->next;
            } else {
                previousNode->next = currentNode->next;
            }
            break;
        }
        previousNode = currentNode;
        currentNode = currentNode->next;
    }
    SignalSema(work->sema);
}

/* Add a finished job to the jobs whose device state is still open, without
 * locking or allocation. */
void fileLinkCloseWait(FileManWork *work, FileTransferJob *job) {
    job->next = work->closingJobs;
    work->closingJobs = job;
}

/* Remove a closed job without locking or freeing. The job must be in the
 * list: unlike fileUnlinkQueue, this walk has no end-of-list guard. */
void fileUnlinkCloseWait(FileManWork *work, FileTransferJob *job) {
    FileTransferJob **incomingLink = &work->closingJobs;
    FileTransferJob *currentJob;
    while ((currentJob = *incomingLink) != job) {
        incomingLink = &currentJob->next;
    }
    *incomingLink = job->next;
}

/* Clear a PAC request, initialize its embedded dispatch packet and optionally
 * apply extra packet setup for any nonzero flags. Queue its copied name and
 * completion context as kind one, returning the allocated work. No failure guard. */
struct FileRequest *pacLoadEx(const char *requestName, u32 flags, u32 dispatchValue, u32 onComplete, u32 userData) {
    FileRequest *request = sdfAllocAndClearQuadwords(sizeof(FileRequest));

    sdfPacInit(&request->packet, (void *)dispatchValue);
    if (flags != 0) {
        sdfPacSetFromTail(&request->packet);
    }
    fileLinkQueue(request, FILE_REQUEST_KIND_PAC, requestName,
                             (void *)onComplete, (void *)userData);
    return request;
}

/* Queue PAC work without extra packet setup or completion context. */
struct FileRequest *pacLoad(const char *requestName) {
    return pacLoadEx(requestName, 0, 0, 0, 0);
}

/* Queue PAC work with extra packet setup enabled and no completion context. */
void pacLoadUseTail(const char *requestName) {
    pacLoadEx(requestName, 1, 0, 0, 0);
}

/* Clear kind-zero request work and narrow callbackMode into byte three.
 * callbackAddress is the queue callback; userData is its context, not another
 * callback. Preserve the existing integer-address parameter representations. */
struct FileRequest *fileLoadEx(const char *requestName, u32 callbackMode, u32 callbackAddress, u32 userData) {
    FileRequest *request = sdfAllocAndClearQuadwords(FILE_REQUEST_HEADER_BYTES);

    request->allocationMode = callbackMode;
    fileLinkQueue(request, FILE_REQUEST_KIND_CALLBACK, requestName,
                             (void *)callbackAddress, (void *)userData);
    return request;
}

/* Queue a callback-kind request with mode zero and no callback/context. */
struct FileRequest *fileLoad(const char *requestName) {
    return fileLoadEx(requestName, 0, 0, 0);
}

/* Queue a callback-kind request with mode one and no callback/context. */
struct FileRequest *fileLoadUseTail(const char *requestName) {
    return fileLoadEx(requestName, 1, 0, 0);
}

/* Return the stored allocation descriptor without changing ownership; work is required. */
struct sdfMemHandle_t *fileGetMemHandle(const struct FileRequest *request) {
    return request->allocationHandle;
}

/* Return the loaded payload's retained byte address. */
u8 *fileGetLoadPtr(const struct FileRequest *request) {
    return request->retainedAddress;
}

/* Return the signed byte count recorded by the file transfer request. */
s32 fileGetLoadSize(const struct FileRequest *request) {
    return request->totalBytes;
}

/* Return the request's remaining transfer byte count. */
u32 fileGetLoadRemain(FileRequest *request) {
    return request->transferBytes;
}

/* PAC requests additionally require a decoded end marker; other kinds only
 * require state six. */
s32 pacFileCheckEnd(const FileRequest *request) {
    s32 result;

    if (request->kind == FILE_REQUEST_KIND_PAC) {
        result = 0;
        if (request->pacDecodeComplete != 0) {
            result = request->state == FILE_REQUEST_STATE_COMPLETE;
        }
        return result;
    }
    return request->state == FILE_REQUEST_STATE_COMPLETE;
}

/* Require PAC decode completion and state six. */
FilePacRequestCompletionResult pacCheckEnd(const FileRequest *request) {
    FilePacRequestCompletionResult result = FILE_PAC_REQUEST_COMPLETION_INCOMPLETE;
    if (request->pacDecodeComplete != 0) {
        result = request->state == FILE_REQUEST_STATE_COMPLETE;
    }
    return result;
}

extern void sdfFileCmdThreadSetHiPrio(void);

/* Pump the device scheduler and file manager while the kind-specific readiness
 * predicate is zero. No timeout or NULL-request guard is introduced. */
void fileSync(FileRequest *request) {
    while (pacFileCheckEnd(request) == 0) {
        sdfFileCmdThreadSetHiPrio();
        fileManager();
    }
}

/* Forward the opaque request pointer to the readiness wait, without cleanup. */
void pacSync(FileRequest *request) {
    fileSync(request);
}


/* Pump updates while the queued head or unknown manager word is nonzero.
 * This function does not directly test the callback list or individual slots. */
void fileSyncAll(void) {
    FileManWork *work = &fileQueueHead;
    while (work->head != 0 || work->closingJobs != 0) {
        fileManager();
    }
}

/* Create a kind-two request with the caller's source buffer and byte count.
 * Its address is stored in the existing value word at +0x24. */
FileRequest *fileSaveEx(const char *requestName, const void *sourceData, s32 sourceByteCount, s32 callbackAddress, s32 userDataAddress) {
    FileRequest *request = sdfAllocAndClearQuadwords(FILE_REQUEST_HEADER_BYTES);

    request->retainedAddress = (u8 *)sourceData;
    request->transferAddress = (u8 *)sourceData;
    request->totalBytes = sourceByteCount;
    request->transferBytes = sourceByteCount;
    fileLinkQueue(request, FILE_REQUEST_KIND_VALUE_PAIR, requestName, (void *)callbackAddress,
                             (void *)userDataAddress);
    return request;
}

/* Queue the value-pair request and expose its opaque request handle. */
struct FileRequest *fileSave(const char *requestName, const void *sourceData, s32 sourceByteCount) {
    return fileSaveEx(requestName, sourceData, sourceByteCount, 0, 0);
}


/* If capacity exists and state is ready, claim nextSlot and advance its
 * four-slot cursor; this does not search for an empty slot. Mark transferring
 * under the semaphore, then start the read after unlocking. Cap at 64 KiB but
 * preserve the absence of a lower size bound and the original request size. */
void pacRead(FileTransferJob *job) {
    FileManWork *work = &fileQueueHead;
    u8 slotIndex;
    s32 chunkBytes;

    WaitSema(work->sema);
    if (work->freeSlots == 0) {
        SignalSema(work->sema);
        return;
    }
    if (job->state != FILE_JOB_READY) {
        SignalSema(work->sema);
        return;
    }
    job->state = FILE_JOB_TRANSFERRING;
    slotIndex = work->nextSlot;
    if (slotIndex == FILE_LAST_READ_SLOT) {
        work->nextSlot = 0;
    } else {
        work->nextSlot = slotIndex + 1;
    }
    job->slot = slotIndex;
    work->freeSlots--;
    work->slots[slotIndex].request = (FileRequest *)job;
    chunkBytes = job->transferBytes;
    if (chunkBytes > FILE_READ_SLOT_BYTES) {
        chunkBytes = FILE_READ_SLOT_BYTES;
    }
    SignalSema(work->sema);
    sdfFileCmdRead(job->deviceRequest,
                    work->buffer + (slotIndex << FILE_READ_SLOT_SHIFT), chunkBytes);
}
