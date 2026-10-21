#ifndef FILE_TRANSFER_JOB_H
#define FILE_TRANSFER_JOB_H

#include "file_request.h"

/* A device-transfer request as the manager core sees it. */
typedef struct FileTransferJob {
    FILE_REQUEST_FIELDS(struct FileTransferJob, FileRequestCallback)
} FileTransferJob;

#endif /* FILE_TRANSFER_JOB_H */
