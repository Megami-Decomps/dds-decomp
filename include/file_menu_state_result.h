#ifndef FILE_MENU_STATE_RESULT_H
#define FILE_MENU_STATE_RESULT_H

#include "common.h"

/* The dispatcher passes its task pointer to this unprototyped callback slot. */
typedef s32 (*FileMenuStateHandler)();

/* The returned word is 0 (retain), -1 (exit), or the next handler address. */
typedef union FileMenuStateResult {
    s32 encodedValue;
    FileMenuStateHandler nextHandler;
} FileMenuStateResult;

enum {
    FILE_MENU_STATE_RETAIN = 0,
    FILE_MENU_STATE_EXIT = -1,
};

typedef char FileMenuStateResult_size_must_be_4[
    (sizeof(FileMenuStateResult) == 4) ? 1 : -1];

#endif /* FILE_MENU_STATE_RESULT_H */
