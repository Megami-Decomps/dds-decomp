#include "sdf_chip.h"
#include "file.h"
#include "file_pac.h"
#include "sdf_pac_state.h"
#include "file_request_api.h"
#include "sdf_dev_state.h"
#include "file_manager_api.h"
#include "file_request.h"

FileManWork fileManagerWork __attribute__((section(".bss")));

extern s32 btlDestroyStageTask(void *);
#include "sdf_debug_api.h"
#define FILE_REQUEST_KIND_CALLBACK 0
#define FILE_REQUEST_KIND_PAC 1
#define FILE_REQUEST_KIND_VALUE_PAIR 2
#define FILE_REQUEST_COMPLETE 6
#define FILE_READ_SLOT_COUNT 4
#define FILE_LAST_READ_SLOT 3
#define FILE_READ_SLOT_BYTES 0x10000
#define FILE_READ_SLOT_SHIFT 16

/* PAC requests delegate cleanup. Other kinds remain pending until state six,
 * then release any device state, duplicated name and entry, returning zero.
 * A pending non-PAC request returns one; request is required. */
s32 filePollEntryCleanup(struct FileRequest *request) {
    if (request->kind == FILE_REQUEST_KIND_PAC) {
        return btlDestroyStageTask(request);
    }
    if (request->state == FILE_REQUEST_COMPLETE) {
        if (request->deviceRequest != NULL) {
            sdfFileCmdFree((DevState *)request->deviceRequest);
        }
        sdfChipFreeDirect(request->name);
        sdfChipFreeDirect(request);
        return 0;
    }
    return 1;
}

extern char *sdfStrDup(const char *text);
extern s32 WaitSema(s32);
extern s32 SignalSema(s32);

/* Duplicate the request name, narrow kind to a byte and append under the
 * semaphore. Start processing only after an empty-to-nonempty transition.
 * The caller supplies initialized next linkage; this function does not clear it. */
void fileManQueueNamedRequest(FileRequest *request, s32 kind, const char *requestName,
                   void *callback, void *userData) {
    FileManWork *work;
    char *duplicatedName;
    s32 startsQueue;

    work = &fileManagerWork;
    request->kind = kind;
    duplicatedName = sdfStrDup(requestName);
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
        fileStartOrResumeQueuedJob();
    }
}

/* Clear every matching read slot and unlink the first queued occurrence under
 * the semaphore. No request/name memory is freed; absence from the queue is OK. */
void fileManCancelRequest(FileRequest *node) {
    FileManWork *work = &fileManagerWork;
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
void filePrependNode(FileManWork *work, FileRequest *job) {
    job->next = work->closingJobs;
    work->closingJobs = job;
}

/* Remove a closed job without locking or freeing. The job must be in the
 * list: unlike fileManCancelRequest, this walk has no end-of-list guard. */
void fileUnlinkNode(FileManWork *work, FileRequest *job) {
    FileRequest **incomingLink = &work->closingJobs;
    FileRequest *currentJob;

    while ((currentJob = *incomingLink) != job) {
        incomingLink = &currentJob->next;
    }
    *incomingLink = job->next;
}

/* Clear a PAC request, initialize its embedded dispatch packet and optionally
 * apply extra packet setup for any nonzero flags. Queue its copied name and
 * completion context as kind one, returning the allocated work. No failure guard. */
struct FilePacRequest *fileCreatePacLoadWork(const char *requestName, s32 flags, void *dispatchValue, s32 onComplete, s32 userData) {
    FilePacRequest *request;

    sdfDebugPrintf("pac load %s\n", requestName);
    request = sdfAllocAndClearQuadwords(sizeof(FilePacRequest));
    sdfPacInitializeDispatchPacket(&request->packet, dispatchValue);
    if (flags != 0) {
        sdfPacUseHighAddressAllocator(&request->packet);
    }
    fileManQueueNamedRequest((FileRequest *)request, FILE_REQUEST_KIND_PAC, requestName,
                             (void *)onComplete, (void *)userData);
    return request;
}

/* Queue PAC work without extra packet setup or completion context. */
struct FileRequest *fileQueuePlainDispatchRequest(const char *requestName) {
    return (struct FileRequest *)fileCreatePacLoadWork(requestName, 0, 0, 0, 0);
}

/* Queue PAC work with extra packet setup enabled and no completion context. */
void fileQueueFlaggedDispatchRequest(const char *requestName) {
    fileCreatePacLoadWork(requestName, 1, 0, 0, 0);
}

/* Clear kind-zero request work and narrow callbackMode into byte three.
 * callbackAddress is the queue callback; userData is its context, not another
 * callback. Keep the raw byte store and existing provider parameter types. */
struct FileRequest *fileCreateCallbackRequest(const char *requestName, s32 callbackMode, s32 callbackAddress, s32 userData) {
    FileRequest *request;

    sdfDebugPrintf("file load %s\n", requestName);
    request = sdfAllocAndClearQuadwords(FILE_REQUEST_HEADER_BYTES);
    request->allocationMode = callbackMode;
    fileManQueueNamedRequest(request, FILE_REQUEST_KIND_CALLBACK, requestName,
                             (void *)callbackAddress, (void *)userData);
    return request;
}

/* Queue a callback-kind request with mode zero and no callback/context. */
struct FileRequest *fileQueueDefaultCallbackRequest(const char *requestName) {
    return fileCreateCallbackRequest(requestName, 0, 0, 0);
}

/* Queue a callback-kind request with mode one and no callback/context. */
struct FileRequest *fileQueueAlternateCallbackRequest(const char *requestName) {
    return fileCreateCallbackRequest(requestName, 1, 0, 0);
}

/* Return the stored allocation descriptor without changing ownership; work is required. */
struct SdfMemBlock *fileGetResourceHandle(const struct FileRequest *request) {
    return request->allocationHandle;
}

/* Return the loaded payload address represented by the stored address word. */
u8 *fileGetLoadedDataAddress(const struct FileRequest *request) {
    return (u8 *)request->retainedAddress;
}

/* Return the recorded resource size; work is required. */
u32 fileGetResourceSize(const struct FileRequest *request) {
    return request->totalBytes;
}

/* Return the request's remaining transfer byte count. */
u32 func_002C8120(FileRequest *request) {
    return request->transferBytes;
}

/* Kind one additionally requires readinessEnabled; other kinds only require
 * state six. The enable word is a gate, not the expected state value. */
s32 fileIsRequestReadyInCurrentMode(const FileRequest *request) {
    s32 result;

    if (request->kind == FILE_REQUEST_KIND_PAC) {
        result = 0;
        if (request->readinessEnabled != 0) {
            result = request->state == FILE_REQUEST_COMPLETE;
        }
        return result;
    }
    return request->state == FILE_REQUEST_COMPLETE;
}

/* Always require the enable gate and state six, irrespective of request kind. */
s32 fileRequestIsReady(const FileRequest *request) {
    s32 result = 0;
    if (request->readinessEnabled != 0) {
        result = request->state == FILE_REQUEST_COMPLETE;
    }
    return result;
}

extern void sdfFileCmdThreadSetLoPrio(void);

/* Pump the device scheduler and file manager while the kind-specific readiness
 * predicate is zero. No timeout or NULL-request guard is introduced. */
void fileWaitReady(FileRequest *request) {
    s32 readyResult;

    while (readyResult = fileIsRequestReadyInCurrentMode(request), readyResult == 0) {
        sdfFileCmdThreadSetLoPrio();
        fileManUpdate();
    }
}

/* Forward the opaque request pointer to the readiness wait, without cleanup. */
void fileWaitForRequestReady(FileRequest *request) {
    fileWaitReady(request);
}

/* Pump updates while the queued head or unknown manager word is nonzero.
 * This function does not directly test the callback list or individual slots. */
void fileWaitIdle(void) {
    FileManWork *work = &fileManagerWork;

    while (work->head != 0 || work->closingJobs != 0) {
        fileManUpdate();
    }
}

/* Create a kind-two request with the caller's source buffer and byte count.
 * Its address is stored in the existing value word at +0x24. */
FileRequest *fileWindowSlotCreate(const char *requestName, const void *sourceData, s32 sourceByteCount, s32 callbackAddress, s32 userDataAddress) {
    FileRequest *request = sdfAllocAndClearQuadwords(FILE_REQUEST_HEADER_BYTES);

    request->retainedAddress = (u32)sourceData;
    request->transferAddress = (u32)sourceData;
    request->totalBytes = sourceByteCount;
    request->transferBytes = sourceByteCount;
    fileManQueueNamedRequest(request, FILE_REQUEST_KIND_VALUE_PAIR, requestName, (void *)callbackAddress,
                             (void *)userDataAddress);
    return request;
}

/* Queue the value-pair request and expose its opaque request handle. */
struct FileRequest *fileQueueWindowSlotRequest(const char *requestName, const void *sourceData, s32 sourceByteCount) {
    return fileWindowSlotCreate(requestName, sourceData, sourceByteCount, 0, 0);
}


/* If capacity exists and state is ready, claim nextSlot and advance its
 * four-slot cursor; this does not search for an empty slot. Mark transferring
 * under the semaphore, then start the read after unlocking. Cap at 64 KiB but
 * preserve the absence of a lower size bound and the original request size. */
void fileQueuePendingRequestInFreeSlot(FileRequest *request) {
    FileManWork *work = &fileManagerWork;
    u8 slotIndex;
    s32 chunkBytes;

    WaitSema(work->sema);
    if (work->freeSlots == 0) {
        SignalSema(work->sema);
        return;
    }
    if (request->state != FILE_JOB_READY) {
        SignalSema(work->sema);
        return;
    }
    request->state = FILE_JOB_TRANSFERRING;
    slotIndex = work->nextSlot;
    if (slotIndex == FILE_LAST_READ_SLOT) {
        work->nextSlot = 0;
    } else {
        work->nextSlot = slotIndex + 1;
    }
    request->slot = slotIndex;
    work->freeSlots--;
    work->slots[slotIndex].request = request;
    chunkBytes = request->transferBytes;
    if (chunkBytes > FILE_READ_SLOT_BYTES) {
        chunkBytes = FILE_READ_SLOT_BYTES;
    }
    SignalSema(work->sema);
    sdfFileCmdRead((DevState *)request->deviceRequest,
                    work->buffer + (slotIndex << FILE_READ_SLOT_SHIFT), chunkBytes);
}
