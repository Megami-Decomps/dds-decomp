#include "file.h"

/* Intrusive list node threaded through +0x4. */
typedef struct FileNode {
    u32 unk0;              /* 0x0 */
    struct FileNode *next; /* 0x4 */
    void *unk8;            /* 0x08: duplicated string */
    u32 unk10;             /* 0x10 */
    u32 unk14;             /* 0x14 */
} FileNode;

/* Request fields initialized before a node joins FileManWork's queue. */
typedef struct FileQueueEntry {
    u8 kind;
    u8 pad01[3];
    FileNode *next;
    char *name;
    u8 pad0C[0xC];
    void *callback;
    void *userData;
} FileQueueEntry;

/* Work record behind the fileManager getters below. */
typedef struct FileWork {
    u8 unk0[0x10];   /* 0x0 */
    u32 unk10;       /* 0x10 */
    u32 size;        /* 0x14: loaded resource size */
    FileNode *head;  /* 0x18 */
    u8 unk1C[4];     /* 0x1C */
    u32 resourceHandle; /* 0x20: released with sdfReleaseResourceAllocation */
    u32 loadedDataAddress; /* 0x24: returned to packed-resource relocation callers */
} FileWork;

typedef struct FileRequest {
    u8 kind; /* 0x00: kind 1 uses the conditional readiness path */
    u8 state; /* 0x01: ready when 6 */
    u8 pad02[0xA];
    u32 handle; /* 0x0C */
    s32 size; /* 0x10 */
    u8 pad14[0x54];
    u16 readinessEnabled; /* 0x68: enables the gated state == 6 readiness check */
    u16 slot; /* 0x6A */
} FileRequest;

FileManWork fileManagerWork __attribute__((section(".bss")));

typedef struct FileCleanup {
    u8 kind;
    u8 state;
    u8 pad02[6];
    void *requestNameCopy; /* 0x08: name duplicated by fileManQueueNamedRequest */
    u32 handle;
} FileCleanup;

extern s32 btlDestroyStageTask(void *);
extern void sdfDevQueueReleaseState(u32);
extern void sdfReleaseChipBlock(void *);

extern s32 WaitSema(s32);
extern s32 SignalSema(s32);


extern s32 fileIsRequestReadyInCurrentMode(FileRequest *request);

#define FILE_REQUEST_KIND_CALLBACK 0
#define FILE_REQUEST_KIND_PAC 1
#define FILE_REQUEST_KIND_VALUE_PAIR 2
#define FILE_REQUEST_COMPLETE 6
#define FILE_READ_SLOT_COUNT 4
#define FILE_LAST_READ_SLOT 3
#define FILE_READ_SLOT_BYTES 0x10000
#define FILE_READ_SLOT_SHIFT 16
#define FILE_PAC_REQUEST_BYTES 0x70
#define FILE_PAC_PACKET_OFFSET 0x30
#define FILE_VALUE_PAIR_REQUEST_BYTES 0x30

/* PAC requests delegate cleanup. Other kinds remain pending until state six,
 * then release any device state, duplicated name and entry, returning zero.
 * A pending non-PAC entry returns one; entry is required. */
s32 filePollEntryCleanup(FileCleanup *entry) {
    if (entry->kind == FILE_REQUEST_KIND_PAC) {
        return btlDestroyStageTask(entry);
    }
    if (entry->state == FILE_REQUEST_COMPLETE) {
        if (entry->handle != 0) {
            sdfDevQueueReleaseState(entry->handle);
        }
        sdfReleaseChipBlock(entry->requestNameCopy);
        sdfReleaseChipBlock(entry);
        return 0;
    }
    return 1;
}

extern char *sdfStrDup(const char *text);
extern s32 func_00289540(void);

/* Duplicate the request name, narrow kind to a byte and append under the
 * semaphore. Start processing only after an empty-to-nonempty transition.
 * The caller supplies initialized next linkage; this function does not clear it. */
void fileManQueueNamedRequest(FileQueueEntry *request, s32 kind, const char *requestName,
                   void *callback, void *userData) {
    FileManWork *work;
    char *duplicatedName;
    s32 startsQueue;

    work = &fileManagerWork;
    request->kind = kind;
    duplicatedName = sdfStrDup(requestName);
    request->callback = callback;
    request->name = duplicatedName;
    request->userData = userData;

    WaitSema(work->sema);
    if (work->tail == NULL) {
        work->head = (FileNode *)request;
        startsQueue = 1;
    } else {
        work->tail->next = (FileNode *)request;
        startsQueue = 0;
    }
    work->tail = (FileNode *)request;
    SignalSema(work->sema);

    if (startsQueue != 0) {
        func_00289540();
    }
}

/* Clear every matching read slot and unlink the first queued occurrence under
 * the semaphore. No request/name memory is freed; absence from the queue is OK. */
void fileManCancelRequest(FileNode *node) {
    FileManWork *work = &fileManagerWork;
    FileNode *previousNode;
    FileNode *currentNode;
    s32 slotIndex;

    WaitSema(work->sema);
    for (slotIndex = 0; slotIndex != FILE_READ_SLOT_COUNT; slotIndex++) {
        if (work->slots[slotIndex].request == (FileRequest *)node) {
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

/* Prepend without locking or allocation. Both list and node are required. */
void filePrependNode(FileWork *list, FileNode *node) {
    node->next = list->head;
    list->head = node;
}

/* Unlink without locking/freeing. The node must occur in the list: unlike
 * fileManCancelRequest, this walk has no end-of-list guard. */
void fileUnlinkNode(FileWork *list, FileNode *node) {
    FileNode **incomingLink = &list->head;
    FileNode *currentNode;
    while ((currentNode = *incomingLink) != node) {
        incomingLink = &currentNode->next;
    }
    *incomingLink = node->next;
}

extern void sdfPacInitializeDispatchPacket(void *, u32);
extern void func_002EDC40(void *);
/* Clear a PAC request, initialize its embedded dispatch packet and optionally
 * apply extra packet setup for any nonzero flags. Queue its copied name and
 * completion context as kind one, returning the allocated work. No failure guard. */
void *fileAllocateDispatchRequest(u32 requestName, u32 flags, u32 dispatchValue, u32 onComplete, u32 userData) {
    void *requestWork = sdfAllocAndClearQuadwords(FILE_PAC_REQUEST_BYTES);
    void *dispatchPacket = (u8 *)requestWork + FILE_PAC_PACKET_OFFSET;

    sdfPacInitializeDispatchPacket(dispatchPacket, dispatchValue);
    if (flags != 0) {
        func_002EDC40(dispatchPacket);
    }
    fileManQueueNamedRequest(requestWork, FILE_REQUEST_KIND_PAC, requestName, onComplete, userData);
    return requestWork;
}

/* Queue PAC work without extra packet setup or completion context. */
void *fileQueuePlainDispatchRequest(u32 requestName) {
    return fileAllocateDispatchRequest(requestName, 0, 0, 0, 0);
}

/* Queue PAC work with extra packet setup enabled and no completion context. */
void fileQueueFlaggedDispatchRequest(u32 requestName) {
    fileAllocateDispatchRequest(requestName, 1, 0, 0, 0);
}

typedef struct FileRequestCallbackWork {
    u8 pad00[3];
    u8 unk03;
    u8 pad04[0x2C];
} FileRequestCallbackWork;

/* Clear kind-zero request work and narrow callbackMode into byte three.
 * callbackAddress is the queue callback; userData is its context, not another
 * callback. Preserve the existing integer-address parameter representations. */
void *fileCreateCallbackRequest(u32 requestName, u32 callbackMode, u32 callbackAddress, u32 userData) {
    FileRequestCallbackWork *requestWork = sdfAllocAndClearQuadwords(sizeof(FileRequestCallbackWork));

    requestWork->unk03 = callbackMode;
    fileManQueueNamedRequest(requestWork, FILE_REQUEST_KIND_CALLBACK, requestName, callbackAddress, userData);
    return requestWork;
}

/* Queue a callback-kind request with mode zero and no callback/context. */
void fileQueueDefaultCallbackRequest(u32 requestName) {
    fileCreateCallbackRequest(requestName, 0, 0, 0);
}

/* Queue a callback-kind request with mode one and no callback/context. */
void fileQueueAlternateCallbackRequest(u32 requestName) {
    fileCreateCallbackRequest(requestName, 1, 0, 0);
}

/* Return the stored resource handle without changing ownership. work is required. */
u32 fileGetResourceHandle(FileWork *work) {
    return work->resourceHandle;
}

/* Return the stored loaded-data address as its existing u32 representation. */
u32 fileGetLoadedDataAddress(FileWork *work) {
    return work->loadedDataAddress;
}

/* Return the recorded resource size; work is required. */
u32 fileGetResourceSize(FileWork *work) {
    return work->size;
}

/* Return the still-unidentified work word at +0x10; do not infer its role. */
u32 func_00288BA0(FileWork *work) {
    return work->unk10;
}

/* Kind one additionally requires readinessEnabled; other kinds only require
 * state six. The enable word is a gate, not the expected state value. */
s32 fileIsRequestReadyInCurrentMode(FileRequest *request) {
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
s32 fileRequestIsReady(FileRequest *request) {
    s32 result = 0;
    if (request->readinessEnabled != 0) {
        result = request->state == FILE_REQUEST_COMPLETE;
    }
    return result;
}

/* Pump the device scheduler and file manager while the kind-specific readiness
 * predicate is zero. No timeout or NULL-request guard is introduced. */
void fileWaitReady(u32 requestAddress) {
    while (fileIsRequestReadyInCurrentMode(requestAddress) == 0) {
        sdfRestoreDeviceThreadPriority();
        fileManUpdate();
    }
}

/* Forward the existing request address to the readiness wait, without cleanup. */
void func_00288C50(u32 requestAddress) {
    fileWaitReady(requestAddress);
}


/* Pump updates while the queued head or unknown manager word is nonzero.
 * This function does not directly test the callback list or individual slots. */
void fileWaitIdle(void) {
    FileManWork *work = &fileManagerWork;
    while (work->head != 0 || work->unk18 != 0) {
        fileManUpdate();
    }
}

typedef struct FileWindowSlot {
    u8 pad00[0x10];
    s32 secondValueCopy; /* 0x10 */
    s32 secondValue;     /* 0x14 */
    u8 pad18[0xC];
    s32 firstValue;      /* 0x24 */
    s32 firstValueCopy;  /* 0x28 */
    u8 pad2C[4];
} FileWindowSlot; /* 0x30 */

/* Create a kind-two request with two stored values and their copies.
 * requestNameAddress is passed to name duplication, not used as a numeric id;
 * callbackAddress/userDataAddress are completion fields, not window coordinates. */
FileWindowSlot *fileWindowSlotCreate(s32 requestNameAddress, s32 firstValue, s32 secondValue, s32 callbackAddress, s32 userDataAddress) {
    FileWindowSlot *requestSlot = sdfAllocAndClearQuadwords(FILE_VALUE_PAIR_REQUEST_BYTES);

    requestSlot->firstValue = firstValue;
    requestSlot->firstValueCopy = firstValue;
    requestSlot->secondValue = secondValue;
    requestSlot->secondValueCopy = secondValue;
    fileManQueueNamedRequest(requestSlot, FILE_REQUEST_KIND_VALUE_PAIR, requestNameAddress, callbackAddress, userDataAddress);
    return requestSlot;
}

/* Queue the value-pair request without completion context. Preserve the
 * existing K&R parameter declarations and their signed integer representations. */
void fileQueueWindowSlotRequest(requestNameAddress, firstValue, secondValue)
s32 requestNameAddress;
s32 firstValue;
s32 secondValue;
{
    fileWindowSlotCreate(requestNameAddress, firstValue, secondValue, 0, 0);
}

extern void sdfDevQueueRead(u32 handle, u32 buffer, u32 size);

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
    chunkBytes = request->size;
    if (chunkBytes > FILE_READ_SLOT_BYTES) {
        chunkBytes = FILE_READ_SLOT_BYTES;
    }
    SignalSema(work->sema);
    sdfDevQueueRead(request->handle, work->buffer + (slotIndex << FILE_READ_SLOT_SHIFT), chunkBytes);
}
