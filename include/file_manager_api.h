#ifndef FILE_MANAGER_API_H
#define FILE_MANAGER_API_H

#include "common.h"

/* Advance queued file work, dispatch completed callbacks, and report remaining work. */
s32 fileManUpdate(void);

/* Start or resume the file job at the head of the transfer queue. */
void fileStartOrResumeQueuedJob(void);

#endif /* FILE_MANAGER_API_H */
