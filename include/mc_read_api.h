#ifndef MC_READ_API_H
#define MC_READ_API_H

#include "mc_poll.h"

/* Queue a memory-card read into a caller-owned buffer. The SDK keeps using
 * that buffer asynchronously, so keep it allocated and do not access it until
 * sdfMcPollRead returns a result other than MC_POLL_PENDING. */
void sdfMcFuncRead(McFileDescriptor fileDescriptor, void *buffer, s32 size);

#endif
