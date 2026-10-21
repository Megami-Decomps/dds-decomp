#include "sdk_ps2.h"
#include "common.h"
#include "kwln_task_create.h"
#include "file_request.h"
#include "sdf_resource.h"
#include "file.h"
#include "file_slot_flags.h"
#include "file_request_entry.h"
#include "file_manager_api.h"
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
s32 fileReqPoll(void) {
    FileReqEntry *entry = &fileRequestEntries[fileActiveCardDetectionRequestIndex];
    s32 info[2];
    s32 result = sceMcSync(1, info, &info[1]);
    s32 retries;

    if (result == 1) {
        entry->unk0 = info[1];
        if (info[1] == -1) {
            entry->slotMetadataDirty = result;
            entry->selectedSlot = 0;
        }
        switch (entry->cardType) {
        case 0:
            entry->slotMetadataDirty = result;
            entry->status = 0;
            entry->selectedSlot = 0;
            break;
        case 2:
            if (entry->formatted == 0) {
                retries = entry->requestRetryCount;
                if (retries < 5) {
                    entry->requestRetryCount = retries + 1;
                    fileReqInit(fileActiveCardDetectionRequestIndex);
                    return 0;
                }
                entry->status = 2;
                entry->slotMetadataDirty = result;
                entry->selectedSlot = 0;
            } else {
                entry->status = result;
            }
            break;
        default:
            entry->status = 3;
            entry->slotMetadataDirty = 1;
            entry->selectedSlot = 0;
            break;
        }
        return 1;
    }
    return 0;
}

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

void fileReqSetSelectedSlot(s32 request, s32 selectedSlot) {
    fileRequestEntries[request].selectedSlot = selectedSlot;
}
