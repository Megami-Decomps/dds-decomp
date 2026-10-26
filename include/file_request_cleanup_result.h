#ifndef FILE_REQUEST_CLEANUP_RESULT_H
#define FILE_REQUEST_CLEANUP_RESULT_H

#include "common.h"

/* Cleanup either invalidates a completed wrapper or retains pending work. */
typedef s32 FileRequestCleanupResult;
enum {
    FILE_REQUEST_CLEANUP_FREED = 0,
    FILE_REQUEST_CLEANUP_PENDING = 1,
};

#endif /* FILE_REQUEST_CLEANUP_RESULT_H */
