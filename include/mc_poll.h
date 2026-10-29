#ifndef MC_POLL_H
#define MC_POLL_H

#include "common.h"

/* A helper-level result for one memory-card poll attempt. PENDING means the
 * helper has no terminal result yet; SDK poll values 0 and -1 both map here. */
typedef enum McPollResult {
    MC_POLL_PENDING = 0,
    MC_POLL_SUCCESS = 1,
    MC_POLL_ERROR = -1,
    MC_POLL_SPECIAL_ERROR = -2
} McPollResult;

/* sceMcOpen's nonnegative completion result is used as a file descriptor. */
typedef s32 McFileDescriptor;

/* On a completed sceMcGetDir request, sceMcSync writes the actual entry count
 * to its command-result output; sceMcSync's return value is the poll state. */
typedef s32 McDirectoryEntryCount;

McPollResult sdfMcPollFormat(void);
McPollResult sdfMcPollChdir(void);
McPollResult sdfMcPollMkdir(void);
McPollResult sdfMcPollGetDir(McDirectoryEntryCount *entryCountOut);
McPollResult sdfMcPollDelete(void);
McPollResult sdfMcPollOpen(McFileDescriptor *fileDescriptorOut);
McPollResult sdfMcPollClose(void);
McPollResult sdfMcPollRead(void);
McPollResult sdfMcPollWrite(void);

#endif
