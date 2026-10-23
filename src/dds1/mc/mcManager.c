#include "sdk_ps2.h"
#include "common.h"
#include "mc_poll.h"
#include "mc_path_api.h"

#define MC_CARD_SLOT 0
#define MC_SYNC_POLL_MODE 1
#define MC_SYNC_COMPLETE 1
#define MC_SDK_COMMAND_SUCCESS 0
#define MC_SDK_SPECIAL_ERROR (-4)
#define MC_SDK_EXTENDED_ERROR_LIMIT (-10)

/* The SDK's queued requests on an open file: its descriptor, and for a
 * transfer the EE buffer and its size. */
s32 sceMcClose(s32 fileDescriptor);
s32 sceMcRead(s32 fileDescriptor, void *buffer, s32 size);
s32 sceMcWrite(s32 fileDescriptor, const void *data, s32 size);


/* Return 1 only for completed status zero, -1 for other completed statuses.
 * Every SDK poll return other than 1 maps to 0, including SDK-level failures. */
McPollResult sdfMcFuncFormat1(void) {
    u32 commandId;
    s32 commandResult;
    s32 pollResult = sceMcSync(MC_SYNC_POLL_MODE, &commandId, &commandResult);

    if (pollResult != MC_SYNC_COMPLETE) {
        return MC_POLL_PENDING;
    }
    if (commandResult == MC_SDK_COMMAND_SUCCESS) {
        return pollResult;
    }
    return MC_POLL_ERROR;
}


/* Start a directory change on slot zero using the supplied path. */
void sdfMcFuncChdir(u32 port, const char *path) {
    sceMcChdir(port, MC_CARD_SLOT, path, 0);
}


/* Accept only completed status zero; translate -4 to -2 and other statuses
 * to -1. Any SDK poll return other than 1 maps to 0; this does not wait. */
McPollResult sdfMcFuncChdir1(void) {
    s32 commandId;
    s32 commandResult;

    if (sceMcSync(MC_SYNC_POLL_MODE, &commandId, &commandResult) == MC_SYNC_COMPLETE) {
        if (commandResult == MC_SDK_COMMAND_SUCCESS) {
            return MC_POLL_SUCCESS;
        }
        return commandResult == MC_SDK_SPECIAL_ERROR ? MC_POLL_SPECIAL_ERROR : MC_POLL_ERROR;
    }
    return MC_POLL_PENDING;
}


/* Start directory creation on slot zero using the supplied path. */
void sdfMcFuncMkdir(u32 port, const char *path) {
    sceMcMkdir(port, MC_CARD_SLOT, path);
}

/* Accept only completed status zero. Completed statuses below -10 map to
 * -2, all other nonzero statuses to -1; non-complete SDK polls map to 0. */
McPollResult sdfMcFuncMkdir1(void) {
    u32 commandId;
    s32 commandResult;
    s32 pollResult = sceMcSync(MC_SYNC_POLL_MODE, &commandId, &commandResult);

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
void sdfMcFuncGetDir(u32 port, const char *path, sceMcTblGetDir *directoryEntries, s32 entryLimit) {
    sceMcGetDir(port, MC_CARD_SLOT, path, 0, entryLimit, directoryEntries);
}

/* Accept any completed nonnegative status and write it to resultOut, which
 * must be valid on success and stays untouched otherwise. Translate -4 to
 * -2, other negatives to -1; non-complete SDK polls map to 0. */
McPollResult sdfMcFuncGetDir1(s32 *commandResultOut) {
    u32 commandId;
    s32 commandResult;
    s32 pollResult = sceMcSync(MC_SYNC_POLL_MODE, &commandId, &commandResult);

    if (pollResult == MC_SYNC_COMPLETE) {
        if (commandResult >= MC_SDK_COMMAND_SUCCESS) {
            *commandResultOut = commandResult;
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
void sdfMcFuncDelete(u32 port, const char *path) {
    sceMcDelete(port, MC_CARD_SLOT, path);
}

/* Same zero-only completion policy as sdfMcFuncChdir1; no blocking loop. */
McPollResult sdfMcFuncDelete1(void) {
    s32 commandId;
    s32 commandResult;

    if (sceMcSync(MC_SYNC_POLL_MODE, &commandId, &commandResult) == MC_SYNC_COMPLETE) {
        if (commandResult == MC_SDK_COMMAND_SUCCESS) {
            return MC_POLL_SUCCESS;
        }
        return commandResult == MC_SDK_SPECIAL_ERROR ? MC_POLL_SPECIAL_ERROR : MC_POLL_ERROR;
    }
    return MC_POLL_PENDING;
}


/* Start opening a path on slot zero. The last operand is numeric open flags,
 * not a data-buffer address (callers use 1 and 0x203). */
void sdfMcFuncOpen(u32 port, const char *path, s32 openFlags) {
    sceMcOpen(port, MC_CARD_SLOT, path, openFlags);
}

/* Accept any completed nonnegative status, writing resultOut only then.
 * Preserve the -4/-2 translation and all other poll/result distinctions. */
McPollResult sdfMcFuncOpen1(s32 *commandResultOut) {
    u32 commandId;
    s32 commandResult;
    s32 pollResult = sceMcSync(MC_SYNC_POLL_MODE, &commandId, &commandResult);

    if (pollResult == MC_SYNC_COMPLETE) {
        if (commandResult >= MC_SDK_COMMAND_SUCCESS) {
            *commandResultOut = commandResult;
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
void sdfMcFuncClose(s32 fileDescriptor) {
    sceMcClose(fileDescriptor);
}

/* Despite its name, poll once rather than waiting; success requires status
 * zero, -4 maps to -2 and other completed statuses to -1. */
McPollResult sdfMcFuncClose1(void) {
    s32 commandId;
    s32 commandResult;

    if (sceMcSync(MC_SYNC_POLL_MODE, &commandId, &commandResult) == MC_SYNC_COMPLETE) {
        if (commandResult == MC_SDK_COMMAND_SUCCESS) {
            return MC_POLL_SUCCESS;
        }
        return commandResult == MC_SDK_SPECIAL_ERROR ? MC_POLL_SPECIAL_ERROR : MC_POLL_ERROR;
    }
    return MC_POLL_PENDING;
}

/* Queue the SDK's read of an open file into an EE buffer; do not poll it. */
void sdfMcFuncRead(s32 fileDescriptor, void *buffer, s32 size) {
    sceMcRead(fileDescriptor, buffer, size);
}

/* Any completed nonnegative status succeeds, but the SDK status is not
 * returned to a caller output. Non-complete SDK polls map to 0. */
McPollResult sdfMcFuncRead1(void) {
    u32 commandId;
    s32 commandResult;
    s32 pollResult = sceMcSync(MC_SYNC_POLL_MODE, &commandId, &commandResult);

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
void sdfMcFuncWrite(s32 fileDescriptor, const void *data, u32 dataSize) {
    sceMcWrite(fileDescriptor, data, (s32)dataSize);
}

/* Poll once: any completed nonnegative write status returns 1; -4 becomes
 * -2 and other negatives become -1. Non-complete SDK polls map to 0. */
McPollResult sdfMcFuncWrite1(void) {
    s32 commandId;
    s32 commandResult;
    s32 pollResult = sceMcSync(MC_SYNC_POLL_MODE, &commandId, &commandResult);

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
