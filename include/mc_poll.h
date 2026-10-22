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

McPollResult sdfMcFuncFormat1(void);
McPollResult sdfMcFuncChdir1(void);
McPollResult sdfMcFuncMkdir1(void);
McPollResult sdfMcFuncGetDir1(s32 *resultOut);
McPollResult sdfMcFuncDelete1(void);
McPollResult sdfMcFuncOpen1(s32 *resultOut);
McPollResult sdfMcFuncClose1(void);
McPollResult sdfMcFuncRead1(void);
McPollResult sdfMcFuncWrite1(void);

#endif
