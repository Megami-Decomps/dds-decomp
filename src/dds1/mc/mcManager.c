#include "common.h"

#define MC_CARD_SLOT 0
#define MC_SYNC_POLL_MODE 1
#define MC_SYNC_COMPLETE 1
#define MC_SDK_COMMAND_SUCCESS 0
#define MC_SDK_SPECIAL_ERROR (-4)
#define MC_SDK_EXTENDED_ERROR_LIMIT (-10)
#define MC_POLL_PENDING 0
#define MC_POLL_SUCCESS 1
#define MC_POLL_ERROR (-1)
#define MC_POLL_SPECIAL_ERROR (-2)

/* Return 1 only for completed status zero, -1 for other completed statuses.
 * Every SDK poll return other than 1 maps to 0, including SDK-level failures. */
s32 mcPollStrictSuccess(void) {
    u32 commandId;
    s32 commandResult;
    s32 pollResult = func_002F6858(MC_SYNC_POLL_MODE, &commandId, &commandResult);

    if (pollResult != MC_SYNC_COMPLETE) {
        return MC_POLL_PENDING;
    }
    if (commandResult == MC_SDK_COMMAND_SUCCESS) {
        return pollResult;
    }
    return MC_POLL_ERROR;
}

/* Start a directory change on slot zero; the u32 argument holds the path address. */
void mcChangeCurrentDirectory(u32 port, u32 pathAddress) {
    func_002F6D50(port, MC_CARD_SLOT, pathAddress, 0);
}

extern s32 func_002F6858(s32, s32 *, s32 *);

/* Accept only completed status zero; translate -4 to -2 and other statuses
 * to -1. Any SDK poll return other than 1 maps to 0; this does not wait. */
s32 mcPollSyncResult(void) {
    s32 commandId;
    s32 commandResult;

    if (func_002F6858(MC_SYNC_POLL_MODE, &commandId, &commandResult) == MC_SYNC_COMPLETE) {
        if (commandResult == MC_SDK_COMMAND_SUCCESS) {
            return MC_POLL_SUCCESS;
        }
        return commandResult == MC_SDK_SPECIAL_ERROR ? MC_POLL_SPECIAL_ERROR : MC_POLL_ERROR;
    }
    return MC_POLL_PENDING;
}

/* Start directory creation on slot zero; preserve the integer path representation. */
void mcMakeDirectory(u32 port, u32 path) {
    sceMcMkdir(port, MC_CARD_SLOT, path);
}

/* Accept only completed status zero. Completed statuses below -10 map to
 * -2, all other nonzero statuses to -1; non-complete SDK polls map to 0. */
s32 mcPollWithExtendedErrors(void) {
    u32 commandId;
    s32 commandResult;
    s32 pollResult = func_002F6858(MC_SYNC_POLL_MODE, &commandId, &commandResult);

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

/* Request directory entries on slot zero with SDK mode zero. The caller's
 * output address is SDK argument six; its entry limit is argument five. */
void mcReadDirectoryEntries(u32 port, u32 path, u32 entriesAddress, u32 entryLimit) {
    func_002F6B80(port, MC_CARD_SLOT, path, 0, entryLimit, entriesAddress);
}

/* Accept any completed nonnegative status and write it to resultOut, which
 * must be valid on success and stays untouched otherwise. Translate -4 to
 * -2, other negatives to -1; non-complete SDK polls map to 0. */
s32 mcPollNonnegativeResult(s32 *resultOut) {
    u32 commandId;
    s32 commandResult;
    s32 pollResult = func_002F6858(MC_SYNC_POLL_MODE, &commandId, &commandResult);

    if (pollResult == MC_SYNC_COMPLETE) {
        if (commandResult >= MC_SDK_COMMAND_SUCCESS) {
            *resultOut = commandResult;
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
void mcDeleteFilePath(u32 port, u32 pathAddress) {
    func_002F6F60(port, MC_CARD_SLOT, pathAddress);
}

/* Same zero-only completion policy as mcPollSyncResult; no blocking loop. */
s32 mcPollNormalizedCommandStatus(void) {
    s32 commandId;
    s32 commandResult;

    if (func_002F6858(MC_SYNC_POLL_MODE, &commandId, &commandResult) == MC_SYNC_COMPLETE) {
        if (commandResult == MC_SDK_COMMAND_SUCCESS) {
            return MC_POLL_SUCCESS;
        }
        return commandResult == MC_SDK_SPECIAL_ERROR ? MC_POLL_SPECIAL_ERROR : MC_POLL_ERROR;
    }
    return MC_POLL_PENDING;
}

/* Start opening a path on slot zero. The last operand is numeric open flags,
 * not a data-buffer address (callers use 1 and 0x203). */
void mcOpenFilePath(u32 port, u32 pathAddress, u32 openFlags) {
    func_002F61D8(port, MC_CARD_SLOT, pathAddress, openFlags);
}

/* Accept any completed nonnegative status, writing resultOut only then.
 * Preserve the -4/-2 translation and all other poll/result distinctions. */
s32 mcPollCommandStatusWithResult(s32 *resultOut) {
    u32 commandId;
    s32 commandResult;
    s32 pollResult = func_002F6858(MC_SYNC_POLL_MODE, &commandId, &commandResult);

    if (pollResult == MC_SYNC_COMPLETE) {
        if (commandResult >= MC_SDK_COMMAND_SUCCESS) {
            *resultOut = commandResult;
            return pollResult;
        }
        if (commandResult == MC_SDK_SPECIAL_ERROR) {
            return MC_POLL_SPECIAL_ERROR;
        }
        return MC_POLL_ERROR;
    }
    return MC_POLL_PENDING;
}

/* Forward the existing no-explicit-argument SDK close call; do not poll it. */
void mcCloseOpenFile(void) {
    func_002F6338();
}

/* Despite its name, poll once rather than waiting; success requires status
 * zero, -4 maps to -2 and other completed statuses to -1. */
s32 mcPollZeroCommandResult(void) {
    s32 commandId;
    s32 commandResult;

    if (func_002F6858(MC_SYNC_POLL_MODE, &commandId, &commandResult) == MC_SYNC_COMPLETE) {
        if (commandResult == MC_SDK_COMMAND_SUCCESS) {
            return MC_POLL_SUCCESS;
        }
        return commandResult == MC_SDK_SPECIAL_ERROR ? MC_POLL_SPECIAL_ERROR : MC_POLL_ERROR;
    }
    return MC_POLL_PENDING;
}

/* Forward the existing no-explicit-argument SDK read call; do not poll it. */
void mcReadOpenFile(void) {
    func_002F6558();
}

/* Any completed nonnegative status succeeds, but the SDK status is not
 * returned to a caller output. Non-complete SDK polls map to 0. */
s32 mcPollCompletionStatus(void) {
    u32 commandId;
    s32 commandResult;
    s32 pollResult = func_002F6858(MC_SYNC_POLL_MODE, &commandId, &commandResult);

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

/* Begin the SDK write through the existing unprototyped call. Keep its
 * argument form unchanged; this entry does not report completion. */
void mcBeginWrite(s32 request, u32 first, u32 second) {
    func_002F6670();
}

/* Poll once: any completed nonnegative write status returns 1; -4 becomes
 * -2 and other negatives become -1. Non-complete SDK polls map to 0. */
s32 mcPollWriteCompletion(void) {
    s32 commandId;
    s32 commandResult;
    s32 pollResult = func_002F6858(MC_SYNC_POLL_MODE, &commandId, &commandResult);

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
