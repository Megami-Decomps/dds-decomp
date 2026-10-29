#define SDK_SKIP_sceMcSync
#include "sdk_ps2.h"
#undef SDK_SKIP_sceMcSync
#include "common.h"
#include "mc_sync_api.h"
#include "kwln_task_create.h"
#include "file_pac.h"
#include "sdf_resource.h"
#include "file.h"
#include "file_slot_flags.h"
#include "file_request_entry.h"
#include "file_manager_api.h"
#include "file_transfer_job.h"
#include "sdf_dev_event.h"
#include "sdf_dev_state.h"

extern s32 fileActiveCardDetectionRequestIndex;

s32 sceMcGetInfo(s32 requestIndex, s32 arg1, void *cardTypeOut, void *sizeKiBOut, void *formattedOut);


/* This unit builds with -fno-optimize-sibling-calls (config/dds1/cflags.txt), so
 * the card-info query stays jal + epilogue. */
void fileReqInit(s32 request) {
    FileReqEntry *entry = &fileRequestEntries[request];
    sceMcGetInfo(request, 0, &entry->cardType, &entry->sizeKiB, &entry->formatted);
}

/* Select and reinitialize one file request, then clear its +0x10 byte. */
void fileReqBegin(s32 request) {
    fileActiveCardDetectionRequestIndex = request;
    fileReqInit(request);
    fileRequestEntries[request].requestRetryCount = 0;
}

/* Keep the zero-status tail distinct and the ready path after retry work;
 * see docs/idioms.md, "Trace-justified switch tails: fileReqPoll". */
FileReqPollResult fileReqPoll(void) {
    FileReqEntry *entry = &fileRequestEntries[fileActiveCardDetectionRequestIndex];
    s32 commandId;
    McSyncCommandResult commandResult;
    McSyncPollStatus pollStatus = sceMcSync(1, &commandId, &commandResult);
    s32 retries;

    if (pollStatus == 1) {
        entry->unk0 = commandResult;
        if (commandResult == -1) {
            entry->slotMetadataDirty = pollStatus;
            entry->selectedSlot = 0;
        }
        switch (entry->cardType) {
        case 0:
            entry->slotMetadataDirty = pollStatus;
            entry->status = FILE_REQ_STATUS_NO_CARD;
            entry->selectedSlot = 0;
            break;
        case 2:
            if (entry->formatted == 0) {
                retries = entry->requestRetryCount;
                if (retries < 5) {
                    entry->requestRetryCount = retries + 1;
                    fileReqInit(fileActiveCardDetectionRequestIndex);
                    return FILE_REQ_POLL_PENDING;
                }
                entry->status = FILE_REQ_STATUS_UNFORMATTED;
                entry->slotMetadataDirty = pollStatus;
                entry->selectedSlot = 0;
            } else {
                entry->status = FILE_REQ_STATUS_READY;
            }
            break;
        default:
            entry->status = FILE_REQ_STATUS_UNSUPPORTED_CARD;
            entry->slotMetadataDirty = 1;
            entry->selectedSlot = 0;
            break;
        }
        return FILE_REQ_POLL_COMPLETE;
    }
    return FILE_REQ_POLL_PENDING;
}

FileReqStatus fileReqGetStatus(s32 request) {
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

void fileReqSetSelectedSlot(s32 request, s32 selectedSlot) {
    fileRequestEntries[request].selectedSlot = selectedSlot;
}
