#ifndef MC_WRITE_API_H
#define MC_WRITE_API_H

#include "mc_poll.h"

/* Queue a memory-card write of a signed byte count, matching sceMcWrite. */
void sdfMcFuncWrite(McFileDescriptor fileDescriptor, const void *data, s32 byteCount);

#endif
