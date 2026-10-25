#ifndef FILE_REQUEST_CLEANUP_API_H
#define FILE_REQUEST_CLEANUP_API_H

#include "file_request_api.h"

/* Freeing a completed request invalidates its wrapper; pending work is retained. */
typedef s32 FileRequestCleanupResult;
enum {
    FILE_REQUEST_CLEANUP_FREED = 0,
    FILE_REQUEST_CLEANUP_PENDING = 1,
};

FileRequestCleanupResult fileRequestFreeIfComplete(struct FileRequest *request);

#endif /* FILE_REQUEST_CLEANUP_API_H */
