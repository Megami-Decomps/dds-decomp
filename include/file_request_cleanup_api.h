#ifndef FILE_REQUEST_CLEANUP_API_H
#define FILE_REQUEST_CLEANUP_API_H

#include "file_request_api.h"
#include "file_request_cleanup_result.h"

/* Freeing a completed request invalidates its wrapper; pending work is retained. */
FileRequestCleanupResult fileRequestFreeIfComplete(struct FileRequest *request);

#endif /* FILE_REQUEST_CLEANUP_API_H */
