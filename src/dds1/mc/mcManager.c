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
#include "mc_poll.h"
#include "mc_path_api.h"
#include "mc_file_api.h"
#include "mc_read_api.h"
#include "mc_write_api.h"

#define MC_CARD_SLOT 0
#define MC_SYNC_POLL_MODE 1
#define MC_SYNC_COMPLETE 1
#define MC_SDK_COMMAND_SUCCESS 0
#define MC_SDK_SPECIAL_ERROR (-4)
#define MC_SDK_EXTENDED_ERROR_LIMIT (-10)

extern s32 sdfMcInfo;

s32 sceMcGetInfo(s32 requestIndex, s32 arg1, void *cardTypeOut, void *sizeKiBOut, void *formattedOut);


/* The dummy address keeps the card-info query a jal, not a sibling call. */
void sdfMcFuncGetInfo(s32 request) {
    FileReqEntry *entry = &sdfMcCb[request];
    s32 dummy;
    s32 *p = &dummy;

    sceMcGetInfo(request, 0, &entry->cardType, &entry->sizeKiB, &entry->formatted);
}

/* Select and reinitialize one file request, then clear its +0x10 byte. */
void sdfMcRegetInfo(s32 request) {
    sdfMcInfo = request;
    sdfMcFuncGetInfo(request);
    sdfMcCb[request].requestRetryCount = 0;
}

/* Keep the zero-status tail distinct and the ready path after retry work;
 * see docs/idioms.md, "Trace-justified switch tails: fileReqPoll". */
FileReqPollResult sdfMcFuncGetInfo1(void) {
    FileReqEntry *entry = &sdfMcCb[sdfMcInfo];
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
                    sdfMcFuncGetInfo(sdfMcInfo);
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

FileReqStatus sdfMcGetFileInfoReady(s32 request) {
    return sdfMcCb[request].status;
}

s32 sdfMcGetFreeBytes(s32 request) {
    return sdfMcCb[request].sizeKiB << 10;
}

u8 sdfMcGetStatus(s32 request) {
    return sdfMcCb[request].slotMetadataDirty;
}

void sdfMcClearCardStat(s32 request) {
    sdfMcCb[request].slotMetadataDirty = 0;
}

void sdfMcSetCardStat(s32 request) {
    sdfMcCb[request].slotMetadataDirty = 1;
}

void sdfMcClearSlotFlags(s32 request, s32 slot) {
    slot += request * FILE_REQ_WORDS_PER_ENTRY;
    fileRequestSlotFlags[slot] = 0;
}

void sdfMcSetSlotFlags(s32 request, s32 slot, s32 mask) {
    slot += request * FILE_REQ_WORDS_PER_ENTRY;
    fileRequestSlotFlags[slot] |= mask;
}

u32 sdfMcGetSlotFlags(s32 request, s32 slot) {
    slot += request * FILE_REQ_WORDS_PER_ENTRY;
    return fileRequestSlotFlags[slot];
}

s8 sdfMcGetSlotIndex(s32 request) {
    return sdfMcCb[request].selectedSlot;
}

void sdfMcSetSlotIndex(s32 request, s32 selectedSlot) {
    sdfMcCb[request].selectedSlot = selectedSlot;
}

void sceMcFormat(u32 arg0, s32 arg1);

void sdfMcFuncFormat(u32 port) {
    sceMcFormat(port, 0);
}

/* Return 1 only for completed status zero, -1 for other completed statuses.
 * Every SDK poll return other than 1 maps to 0, including SDK-level failures. */
McPollResult sdfMcFuncFormat1(void) {
    s32 commandId;
    McSyncCommandResult commandResult;
    McSyncPollStatus pollResult = sceMcSync(MC_SYNC_POLL_MODE, &commandId, &commandResult);

    if (pollResult != MC_SYNC_COMPLETE) {
        return MC_POLL_PENDING;
    }
    if (commandResult == MC_SDK_COMMAND_SUCCESS) {
        return pollResult;
    }
    return MC_POLL_ERROR;
}


/* Start a directory change on slot zero using the supplied path. */
void sdfMcFuncChdir(McPort port, const char *path) {
    sceMcChdir(port, MC_CARD_SLOT, path, 0);
}


/* Accept only completed status zero; translate -4 to -2 and other statuses
 * to -1. Any SDK poll return other than 1 maps to 0; this does not wait. */
McPollResult sdfMcFuncChdir1(void) {
    s32 commandId;
    McSyncCommandResult commandResult;

    if (sceMcSync(MC_SYNC_POLL_MODE, &commandId, &commandResult) == MC_SYNC_COMPLETE) {
        if (commandResult == MC_SDK_COMMAND_SUCCESS) {
            return MC_POLL_SUCCESS;
        }
        return commandResult == MC_SDK_SPECIAL_ERROR ? MC_POLL_SPECIAL_ERROR : MC_POLL_ERROR;
    }
    return MC_POLL_PENDING;
}


/* Start directory creation on slot zero using the supplied path. */
void sdfMcFuncMkdir(McPort port, const char *path) {
    sceMcMkdir(port, MC_CARD_SLOT, path);
}

/* Accept only completed status zero. Completed statuses below -10 map to
 * -2, all other nonzero statuses to -1; non-complete SDK polls map to 0. */
McPollResult sdfMcFuncMkdir1(void) {
    s32 commandId;
    McSyncCommandResult commandResult;
    McSyncPollStatus pollResult = sceMcSync(MC_SYNC_POLL_MODE, &commandId, &commandResult);

    if (pollResult == MC_SYNC_COMPLETE) {
        if (commandResult == MC_SDK_COMMAND_SUCCESS) {
            return pollResult;
        }
        if (commandResult < MC_SDK_EXTENDED_ERROR_LIMIT) {
            return MC_POLL_SPECIAL_ERROR;
        }
        return MC_POLL_ERROR;
    }
    return MC_POLL_PENDING;
}

/* Request directory entries on slot zero with SDK mode zero. The output table
 * is SDK argument six; its entry limit is argument five. */
void sdfMcFuncGetDir(McPort port, const char *path, sceMcTblGetDir *directoryEntries, s32 entryLimit) {
    sceMcGetDir(port, MC_CARD_SLOT, path, 0, entryLimit, directoryEntries);
}

/* Accept any completed nonnegative entry count and write it to entryCountOut,
 * which must be valid on success and stays untouched otherwise. Translate
 * -4 to -2, other negatives to -1; non-complete SDK polls map to 0. */
McPollResult sdfMcFuncGetDir1(McDirectoryEntryCount *entryCountOut) {
    s32 commandId;
    McSyncCommandResult commandResult;
    McSyncPollStatus pollResult = sceMcSync(MC_SYNC_POLL_MODE, &commandId, &commandResult);

    if (pollResult == MC_SYNC_COMPLETE) {
        if (commandResult >= MC_SDK_COMMAND_SUCCESS) {
            *entryCountOut = commandResult;
            return pollResult;
        }
        if (commandResult == MC_SDK_SPECIAL_ERROR) {
            return MC_POLL_SPECIAL_ERROR;
        }
        return MC_POLL_ERROR;
    }
    return MC_POLL_PENDING;
}


/* Start removal of the supplied path on slot zero; no completion wait here. */
void sdfMcFuncDelete(McPort port, const char *path) {
    sceMcDelete(port, MC_CARD_SLOT, path);
}

/* Same zero-only completion policy as sdfMcFuncChdir1; no blocking loop. */
McPollResult sdfMcFuncDelete1(void) {
    s32 commandId;
    McSyncCommandResult commandResult;

    if (sceMcSync(MC_SYNC_POLL_MODE, &commandId, &commandResult) == MC_SYNC_COMPLETE) {
        if (commandResult == MC_SDK_COMMAND_SUCCESS) {
            return MC_POLL_SUCCESS;
        }
        return commandResult == MC_SDK_SPECIAL_ERROR ? MC_POLL_SPECIAL_ERROR : MC_POLL_ERROR;
    }
    return MC_POLL_PENDING;
}


/* Start opening a path on slot zero with the requested read/save mode. */
void sdfMcFuncOpen(McPort port, const char *path, McOpenMode openMode) {
    sceMcOpen(port, MC_CARD_SLOT, path, openMode);
}

/* Accept any completed nonnegative open result as a file descriptor and write
 * it to fileDescriptorOut only then. Preserve all poll/error translations. */
McPollResult sdfMcFuncOpen1(McFileDescriptor *fileDescriptorOut) {
    s32 commandId;
    McSyncCommandResult commandResult;
    McSyncPollStatus pollResult = sceMcSync(MC_SYNC_POLL_MODE, &commandId, &commandResult);

    if (pollResult == MC_SYNC_COMPLETE) {
        if (commandResult >= MC_SDK_COMMAND_SUCCESS) {
            *fileDescriptorOut = commandResult;
            return pollResult;
        }
        if (commandResult == MC_SDK_SPECIAL_ERROR) {
            return MC_POLL_SPECIAL_ERROR;
        }
        return MC_POLL_ERROR;
    }
    return MC_POLL_PENDING;
}

/* Queue the SDK's close of an open file; do not poll it. */
void sdfMcFuncClose(McFileDescriptor fileDescriptor) {
    sceMcClose(fileDescriptor);
}

/* Despite its name, poll once rather than waiting; success requires status
 * zero, -4 maps to -2 and other completed statuses to -1. */
McPollResult sdfMcFuncClose1(void) {
    s32 commandId;
    McSyncCommandResult commandResult;

    if (sceMcSync(MC_SYNC_POLL_MODE, &commandId, &commandResult) == MC_SYNC_COMPLETE) {
        if (commandResult == MC_SDK_COMMAND_SUCCESS) {
            return MC_POLL_SUCCESS;
        }
        return commandResult == MC_SDK_SPECIAL_ERROR ? MC_POLL_SPECIAL_ERROR : MC_POLL_ERROR;
    }
    return MC_POLL_PENDING;
}

/* Queue the SDK's read of an open file into an EE buffer; do not poll it. */
void sdfMcFuncRead(McFileDescriptor fileDescriptor, void *buffer, s32 size) {
    sceMcRead(fileDescriptor, buffer, size);
}

/* Any completed nonnegative status succeeds, but the SDK status is not
 * returned to a caller output. Non-complete SDK polls map to 0. */
McPollResult sdfMcFuncRead1(void) {
    s32 commandId;
    McSyncCommandResult commandResult;
    McSyncPollStatus pollResult = sceMcSync(MC_SYNC_POLL_MODE, &commandId, &commandResult);

    if (pollResult == MC_SYNC_COMPLETE) {
        if (commandResult >= MC_SDK_COMMAND_SUCCESS) {
            return pollResult;
        }
        if (commandResult == MC_SDK_SPECIAL_ERROR) {
            return MC_POLL_SPECIAL_ERROR;
        }
        return MC_POLL_ERROR;
    }
    return MC_POLL_PENDING;
}

/* Queue the SDK's write of an EE buffer to an open file; this entry does
 * not report completion. */
void sdfMcFuncWrite(McFileDescriptor fileDescriptor, const void *data, s32 byteCount) {
    sceMcWrite(fileDescriptor, data, byteCount);
}

/* Poll once: any completed nonnegative write status returns 1; -4 becomes
 * -2 and other negatives become -1. Non-complete SDK polls map to 0. */
McPollResult sdfMcFuncWrite1(void) {
    s32 commandId;
    McSyncCommandResult commandResult;
    McSyncPollStatus pollResult = sceMcSync(MC_SYNC_POLL_MODE, &commandId, &commandResult);

    if (pollResult == MC_SYNC_COMPLETE) {
        if (commandResult >= MC_SDK_COMMAND_SUCCESS) {
            return pollResult;
        }
        if (commandResult == MC_SDK_SPECIAL_ERROR) {
            return MC_POLL_SPECIAL_ERROR;
        }
        return MC_POLL_ERROR;
    }
    return MC_POLL_PENDING;
}
