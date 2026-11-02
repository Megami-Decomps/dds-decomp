#include "sdk_ps2.h"
#include "dsp_entry_api.h"
#include "evt_stage_test_api.h"
#include "evt_stage_test_resource_work_result.h"
#include "mnu_input.h"
#include "sdf_packet_list.h"
#include "eff_resource_slots.h"
#include "eff_resource_records.h"
#include "fpu.h"
#include "common.h"
#include "dat_assigned_item_id.h"
#include "pty_record_slot_value_api.h"
#include "sdf_vu0.h"
#include "pty_bullet_item_count_api.h"
#include "sdf_vu_matrix.h"
#include "kwln_draw_buffer.h"
#include "eff_random.h"
#include "snd_sequence_volume_pan.h"
#include "evt_world_object_api.h"
#include "evt_stage_model_scale.h"
#include "ui_color.h"
#include "btl_stage_task_cleanup.h"
#include "file_pac.h"
#include "file_pac_cleanup_api.h"
#include "sdf_motion.h"
#include "sdf_chip.h"
#include "fr_font.h"
#include "sdf_dev_state.h"
#include "dat_command.h"
#include "dds3obj.h"
#include "evt_world.h"
#include "sdf.h"
#include "kwln.h"
#include "kwln_task_create.h"
#include "sdf_pac_state.h"
#include "sdf_projection.h"
#include "sdf_sif_command.h"
#include "pcp_vu0.h"
#include "mnu.h"
#include "mnu_party_row_draw_state.h"
#include "mnu_ability_target_category.h"
#include "mnu_entry_cost_api.h"
#include "mnu_entry_use_status_api.h"
#include "mnu_popup_eligibility.h"
#include "mnu_party_entry_lookup.h"
#include "mnu_panel_item.h"
#include "mnu_shop.h"
#include "mnu_list.h"
#include "mnu_panel_state.h"
#include "mnu_panel_state_prefix.h"
#include "mnu_staff.h"
#include "mdl.h"
#include "mdl_asset_request.h"
#include "eff.h"
#include "dat_state.h"
#include "prf_current_record_view.h"
#include "pty_skill_nibble_state.h"
#include "pty_roster_api.h"
#include "pty_party_unit_presence_api.h"
#include "evt_stage_test.h"
#include "kwln_task_lifecycle.h"
#include "btl.h"
#include "btl_party_command_use_result.h"
#include "dds3_world_primary.h"
#include "fld_scene_command_state_api.h"
#include "itf_draw_grid.h"
#include "prf_profile_cap.h"
#include "sdf_packet_alloc.h"
#include "fr_font_draw_glyph.h"
#include "file.h"
#include "file_pac_load_api.h"
#include "file_request_cleanup_api.h"
#include "file_pac_completion_api.h"
#include "file_request_end_check_api.h"
#include "file_request_allocation_mode.h"
#include "file_manager_api.h"
#include "file_request.h"
#include "file_request_lifecycle.h"
#include "file_request_kind.h"
#include "file_transfer_job.h"
#include "sdf_debug_api.h"
#include "sdf_semaphore_api.h"
#include "kwln_task_runner.h"
#include "sdf_heap_api.h"
#include "file_slot_flags.h"
#include "file_request_entry.h"
#include "sdf_pac_decode.h"
#include "sdf_dev_event.h"

extern s32 D_00437C9C;

extern char D_00437CB0[];

extern s32 D_00437CB8;

extern s32 D_00437CBC;

extern const s32 D_00437C60[];

extern char D_00437C68[];

extern const s32 D_00437C70[];

extern char D_00437C68[]; /* "%2d", shared with mnuDrawPartyMemberCommandPanel. */

extern char D_00437C88[];

extern s32 D_00437CA4;

extern const char *D_00437CA0;

extern char D_00437CA8[];

/* Release the device state, packet nodes, copied name, and wrapper only after
 * the device lifecycle closes; otherwise retain the pending request. */
FileRequestCleanupResult pacFileHandleFree(request)
    FilePacRequest *request;
{
    if (request->state == FILE_REQUEST_STATE_COMPLETE) {
        sdfFileCmdHandle_t *deviceRequest = request->deviceRequest;
        if (deviceRequest != NULL) {
            sdfFileCmdFree(deviceRequest);
        }
        sdfPacFree(&request->packet);
        sdfChipFreeDirect(request->name);
        sdfChipFreeDirect((void *)request);
        return FILE_REQUEST_CLEANUP_FREED;
    }
    return FILE_REQUEST_CLEANUP_PENDING;
}

s32 pacFileHandleFree_(void *request) {
    return pacFileHandleFree(request);
}

FileManWork fileQueueHead __attribute__((section(".bss")));

#define FILE_READ_SLOT_COUNT 4

#define FILE_LAST_READ_SLOT 3

#define FILE_READ_SLOT_BYTES 0x10000

#define FILE_READ_SLOT_SHIFT 16

/* PAC requests delegate cleanup. Other kinds remain pending until the device
 * request closes, then release its state, duplicated name and entry. */
FileRequestCleanupResult fileRequestFreeIfComplete(struct FileRequest *request) {
    if (request->kind == FILE_REQUEST_KIND_PAC_LOAD) {
        return pacFileHandleFree((struct FilePacRequest *)request);
    }
    if (request->state == FILE_REQUEST_LIFECYCLE_CLOSED) {
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

/* Clear a PAC request, select its custom or standard packet callback, and
 * choose the PAC payload allocator. Queue its copied name and completion
 * context as kind one, returning the allocated work. No failure guard. */
struct FilePacRequest *fileCreatePacLoadWork(
    const char *requestName, FilePacResourceAllocationMode allocationMode,
    void *packetCallbackAddress, s32 onComplete, s32 userData) {
    FilePacRequest *request;

    printf("pac load %s\n", requestName);
    request = sdfChipAllocZeroDirect(sizeof(FilePacRequest));
    sdfPacInit(&request->packet, packetCallbackAddress);
    if (allocationMode != FILE_PAC_RESOURCE_ALLOCATION_DEFAULT) {
        sdfPacSetFromTail(&request->packet);
    }
    fileLinkQueue((FileRequest *)request, FILE_REQUEST_KIND_PAC_LOAD, requestName,
                             (void *)onComplete, (void *)userData);
    return request;
}

/* Queue PAC work with the default payload allocator and no completion context. */
struct FileRequest *pacLoad(const char *requestName) {
    return (struct FileRequest *)fileCreatePacLoadWork(
        requestName, FILE_PAC_RESOURCE_ALLOCATION_DEFAULT, 0, 0, 0);
}

/* Queue PAC work with the high-address payload allocator and no completion context. */
void pacLoadUseTail(const char *requestName) {
    fileCreatePacLoadWork(requestName, FILE_PAC_RESOURCE_ALLOCATION_FROM_TAIL,
                          0, 0, 0);
}

/* Queue an ordinary file load and narrow allocationMode into byte three.
 * callbackAddress is the queue callback; userData is its context, not another
 * callback. Keep the raw byte store and existing provider parameter types. */
struct FileRequest *fileLoadEx(const char *requestName, s32 allocationMode, s32 callbackAddress, s32 userData) {
    FileRequest *request;

    printf("file load %s\n", requestName);
    request = sdfChipAllocZeroDirect(FILE_REQUEST_HEADER_BYTES);
    request->allocationMode = allocationMode;
    fileLinkQueue(request, FILE_REQUEST_KIND_FILE_LOAD, requestName,
                             (void *)callbackAddress, (void *)userData);
    return request;
}

/* Queue an ordinary file load with retryable direct allocation and no callback/context. */
struct FileRequest *fileLoad(const char *requestName) {
    return fileLoadEx(requestName, FILE_REQUEST_ALLOCATION_MODE_DIRECT, 0, 0);
}

/* Queue an ordinary file load with tail allocation and no callback/context. */
struct FileRequest *fileLoadUseTail(const char *requestName) {
    return fileLoadEx(requestName, FILE_REQUEST_ALLOCATION_MODE_FROM_TAIL, 0, 0);
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
 * require a closed device request. */
FileRequestEndCheckResult pacFileCheckEnd(const FileRequest *request) {
    FileRequestEndCheckResult result;

    if (request->kind == FILE_REQUEST_KIND_PAC_LOAD) {
        result = FILE_REQUEST_END_CHECK_PENDING;
        if (request->pacDecodeComplete != 0) {
            result = request->state == FILE_REQUEST_LIFECYCLE_CLOSED
                ? FILE_REQUEST_END_CHECK_REACHED : FILE_REQUEST_END_CHECK_PENDING;
        }
        return result;
    }
    return request->state == FILE_REQUEST_LIFECYCLE_CLOSED
        ? FILE_REQUEST_END_CHECK_REACHED : FILE_REQUEST_END_CHECK_PENDING;
}

/* Require PAC decode completion and a closed device request. */
FilePacRequestCompletionResult pacCheckEnd(const FileRequest *request) {
    FilePacRequestCompletionResult result = FILE_PAC_REQUEST_COMPLETION_INCOMPLETE;
    if (request->pacDecodeComplete != 0) {
        result = request->state == FILE_REQUEST_LIFECYCLE_CLOSED;
    }
    return result;
}

extern void sdfFileCmdThreadSetHiPrio(void);

/* Pump the device scheduler and file manager while the request-specific end
 * condition is pending. No timeout or NULL-request guard is introduced. */
void fileSync(FileRequest *request) {
    s32 readyResult;

    while (readyResult = pacFileCheckEnd(request),
           readyResult == FILE_REQUEST_END_CHECK_PENDING) {
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

/* Create a file-save request with the caller's source buffer and byte count.
 * Its address is stored in the existing value word at +0x24. */
FileRequest *fileSaveEx(const char *requestName, const void *sourceData, s32 sourceByteCount, s32 callbackAddress, s32 userDataAddress) {
    FileRequest *request = sdfChipAllocZeroDirect(FILE_REQUEST_HEADER_BYTES);

    request->retainedAddress = (u8 *)sourceData;
    request->transferAddress = (u8 *)sourceData;
    request->totalBytes = sourceByteCount;
    request->transferBytes = sourceByteCount;
    fileLinkQueue(request, FILE_REQUEST_KIND_FILE_SAVE, requestName, (void *)callbackAddress,
                             (void *)userDataAddress);
    return request;
}

/* Queue the file-save request and expose its opaque request handle. */
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
    if (job->state != FILE_REQUEST_LIFECYCLE_READY) {
        SignalSema(work->sema);
        return;
    }
    job->state = FILE_REQUEST_LIFECYCLE_TRANSFERRING;
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

extern s32 D_00437CC0;

extern char fileManagerTaskName[];

extern FileManagerQueuedRequestResult (*p2sdSmgBuf)(void);

extern SdfFileCommandQueueResult sdfFileCmdFileSize(sdfFileCmdHandle_t *);

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
        sdfFileCmdFileSize(deviceRequest);
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
        saved = D_00437CC0;
        D_00437CC0 = 1;
        fileExecQueue();
        D_00437CC0 = saved;
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
            job->allocationHandle = sdfMemAlloc2Direct(job->totalBytes);
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
FileManagerQueuedRequestResult fileManager(void) {
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
    return work->head != NULL ? FILE_MANAGER_QUEUED_REQUESTS_PENDING
                              : FILE_MANAGER_QUEUED_REQUESTS_EMPTY;
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

s32 D_00437CC0 __attribute__((section(".sdata"))) = 0;

char fileManagerTaskName[8] __attribute__((section(".sdata"))) = "fileMan";


