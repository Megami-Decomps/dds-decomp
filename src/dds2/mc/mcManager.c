#include "common.h"

extern s32 func_0034F680(s32, s32 *, s32 *);

s32 func_002C92E8(void) {
    u32 cmdId;
    s32 status;
    s32 result = func_0034F680(1, &cmdId, &status);

    if (result != 1) {
        return 0;
    }
    if (status == 0) {
        return result;
    }
    return -1;
}

void func_002C9328(u32 arg0, u32 arg1) {
    func_0034FB90(arg0, 0, arg1, 0);
}

/* Async SDK polling: 0 pending, 1 success, -2 for SDK result -4, -1 otherwise. */
s32 mcPollSyncResult(void) {
    s32 command;
    s32 result;

    if (func_0034F680(1, &command, &result) == 1) {
        if (result == 0) {
            return 1;
        }
        return result == -4 ? -2 : -1;
    }
    return 0;
}

void mcMakeDirectory(u32 arg0, u32 arg1) {
    sceMcMkdir(arg0, 0, arg1);
}

s32 func_002C93B8(void) {
    u32 cmdId;
    s32 status;
    s32 result = func_0034F680(1, &cmdId, &status);

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

void func_002C9400(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    func_0034F9B0(arg0, 0, arg1, 0, arg3, arg2);
}

s32 func_002C9430(s32 *out) {
    u32 cmdId;
    s32 status;
    s32 result = func_0034F680(1, &cmdId, &status);

    if (result == 1) {
        if (status >= 0) {
            *out = status;
            return result;
        }
        if (status == -4) {
            return -2;
        }
        return -1;
    }
    return 0;
}

void func_002C9490(u32 arg0, u32 arg1) {
    func_0034FDA8(arg0, 0, arg1);
}

s32 func_002C94B0(void) {
    s32 command;
    s32 result;

    if (func_0034F680(1, &command, &result) == 1) {
        if (result == 0) {
            return 1;
        }
        return result == -4 ? -2 : -1;
    }
    return 0;
}

void func_002C9500(u32 arg0, u32 arg1, u32 arg2) {
    func_0034EFE0(arg0, 0, arg1, arg2);
}

s32 func_002C9528(s32 *out) {
    u32 cmdId;
    s32 status;
    s32 result = func_0034F680(1, &cmdId, &status);

    if (result == 1) {
        if (status >= 0) {
            *out = status;
            return result;
        }
        if (status == -4) {
            return -2;
        }
        return -1;
    }
    return 0;
}

void func_002C9588(void) {
    func_0034F150();
}

s32 func_002C95A0(void) {
    s32 command;
    s32 result;

    if (func_0034F680(1, &command, &result) == 1) {
        if (result == 0) {
            return 1;
        }
        return result == -4 ? -2 : -1;
    }
    return 0;
}

void func_002C95F0(void) {
    func_0034F370();
}

s32 func_002C9608(void) {
    u32 cmdId;
    s32 status;
    s32 result = func_0034F680(1, &cmdId, &status);

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
