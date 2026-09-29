#include "common.h"

/* Poll an asynchronous memory-card request: only status zero succeeds. */
s32 func_00289D68(void) {
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
s32 func_00289E38(void) {
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
s32 func_00289EB0(s32 *resultOut) {
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

s32 func_00289F30(void) {
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

s32 func_00289FA8(s32 *resultOut) {
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

s32 func_0028A020(void) {
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
s32 func_0028A088(void) {
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
