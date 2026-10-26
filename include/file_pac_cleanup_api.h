#ifndef FILE_PAC_CLEANUP_API_H
#define FILE_PAC_CLEANUP_API_H

#include "file_request_cleanup_result.h"

struct FilePacRequest;

/* Release the closed PAC wrapper and queue nodes; retain it while device work
 * is pending. */
FileRequestCleanupResult pacFileHandleFree(struct FilePacRequest *request);

#endif /* FILE_PAC_CLEANUP_API_H */
