#ifndef FILE_MANAGER_API_H
#define FILE_MANAGER_API_H

#include "common.h"

typedef s32 FileManagerQueuedRequestResult;
enum {
    FILE_MANAGER_QUEUED_REQUESTS_EMPTY = 0,
    FILE_MANAGER_QUEUED_REQUESTS_PENDING = 1
};

/* Pump queued work, then report only whether the request queue has a head.
 * Closing jobs and pending PAC callbacks are not included in this result. */
FileManagerQueuedRequestResult fileManager(void);

/* Start or resume the file job at the head of the transfer queue. */
void fileExecQueue(void);

#endif /* FILE_MANAGER_API_H */
