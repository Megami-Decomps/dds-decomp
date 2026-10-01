#include "common.h"

/* Poll an asynchronous memory-card request: only status zero succeeds. */
s32 mcPollStrictSuccess(void) {
    u32 cmdId;
    s32 status;
    s32 result = func_002F6858(1, &cmdId, &status);

    if (result != 1) {
        return 0;
    }
    if (status == 0) {
        return result;
    }
    return -1;
}

void func_00289DA8(u32 port, u32 request) {
    func_002F6D50(port, 0, request, 0);
}

extern s32 func_002F6858(s32, s32 *, s32 *);

/* Async SDK polling: 0 pending, 1 success, -2 for SDK result -4, -1 otherwise. */
s32 mcPollSyncResult(void) {
    s32 command;
    s32 result;

    if (func_002F6858(1, &command, &result) == 1) {
        if (result == 0) {
            return 1;
        }
        return result == -4 ? -2 : -1;
    }
    return 0;
}

void mcMakeDirectory(u32 port, u32 path) {
    sceMcMkdir(port, 0, path);
}

/* Treat SDK errors below -10 as a distinct memory-card failure. */
s32 mcPollWithExtendedErrors(void) {
    u32 cmdId;
    s32 status;
    s32 result = func_002F6858(1, &cmdId, &status);

    if (result == 1) {
        if (status == 0) {
            return result;
        }
        if (status < -10) {
            return -2;
        }
        return -1;
    }
    return 0;
}

void func_00289E80(u32 port, u32 path, u32 mode, u32 flags) {
    func_002F6B80(port, 0, path, 0, flags, mode);
}

/* Return a nonnegative SDK result through out; map -4 to -2. */
s32 mcPollNonnegativeResult(s32 *resultOut) {
    u32 cmdId;
    s32 status;
    s32 result = func_002F6858(1, &cmdId, &status);

    if (result == 1) {
        if (status >= 0) {
            *resultOut = status;
            return result;
        }
        if (status == -4) {
            return -2;
        }
        return -1;
    }
    return 0;
}

void func_00289F10(u32 port, u32 request) {
    func_002F6F60(port, 0, request);
}

s32 mcPollNormalizedCommandStatus(void) {
    s32 command;
    s32 result;

    if (func_002F6858(1, &command, &result) == 1) {
        if (result == 0) {
            return 1;
        }
        return result == -4 ? -2 : -1;
    }
    return 0;
}

void func_00289F80(u32 port, u32 request, u32 buffer) {
    func_002F61D8(port, 0, request, buffer);
}

s32 mcPollCommandStatusWithResult(s32 *resultOut) {
    u32 cmdId;
    s32 status;
    s32 result = func_002F6858(1, &cmdId, &status);

    if (result == 1) {
        if (status >= 0) {
            *resultOut = status;
            return result;
        }
        if (status == -4) {
            return -2;
        }
        return -1;
    }
    return 0;
}

void func_0028A008(void) {
    func_002F6338();
}

s32 fileWaitCommandDone(void) {
    s32 command;
    s32 result;

    if (func_002F6858(1, &command, &result) == 1) {
        if (result == 0) {
            return 1;
        }
        return result == -4 ? -2 : -1;
    }
    return 0;
}

void func_0028A070(void) {
    func_002F6558();
}

/* Like the result poll, but only report completion, not the SDK result. */
s32 mcPollCompletionStatus(void) {
    u32 cmdId;
    s32 status;
    s32 result = func_002F6858(1, &cmdId, &status);

    if (result == 1) {
        if (status >= 0) {
            return result;
        }
        if (status == -4) {
            return -2;
        }
        return -1;
    }
    return 0;
}

void fileWriteBegin(s32 request, u32 first, u32 second) {
    func_002F6670();
}

/* Poll the memory-card write: busy is 0, success 1, and the card's
 * -4 status is translated to the menu's -2 error. */
s32 fileWriteWait(void) {
    s32 cmdId;
    s32 status;
    s32 result = func_002F6858(1, &cmdId, &status);

    if (result == 1) {
        if (status >= 0) {
            return result;
        }
        if (status == -4) {
            return -2;
        }
        return -1;
    }
    return 0;
}
