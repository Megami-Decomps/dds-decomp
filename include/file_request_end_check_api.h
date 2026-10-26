#ifndef FILE_REQUEST_END_CHECK_API_H
#define FILE_REQUEST_END_CHECK_API_H

#include "file_request_api.h"

/* Distinguish a pending end condition from one already reached; this reports
 * request lifecycle completion, not whether loading succeeded. */
typedef s32 FileRequestEndCheckResult;
enum {
    FILE_REQUEST_END_CHECK_PENDING = 0,
    FILE_REQUEST_END_CHECK_REACHED = 1,
};

FileRequestEndCheckResult pacFileCheckEnd(const struct FileRequest *request);

#endif /* FILE_REQUEST_END_CHECK_API_H */
