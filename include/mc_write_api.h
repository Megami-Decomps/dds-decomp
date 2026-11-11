#ifndef MC_WRITE_API_H
#define MC_WRITE_API_H

#include "mc_poll.h"

/* Queue a memory-card write of a signed byte count, matching sceMcWrite.
 * The SDK reads this caller-owned buffer asynchronously; keep it allocated
 * and unchanged until sdfMcFuncWrite1 returns a result other than
 * MC_POLL_PENDING. */
void sdfMcFuncWrite(McFileDescriptor fileDescriptor, const void *data, s32 byteCount);

#endif
