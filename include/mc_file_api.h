#ifndef MC_FILE_API_H
#define MC_FILE_API_H

#include "common.h"

/* SDK libmc operations on an open file descriptor. */
s32 sceMcClose(s32 fileDescriptor);
s32 sceMcRead(s32 fileDescriptor, void *buffer, s32 size);
s32 sceMcWrite(s32 fileDescriptor, const void *data, s32 size);

#endif
