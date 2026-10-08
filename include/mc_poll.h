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

McPollResult mcPollStrictSuccess(void);
McPollResult mcPollSyncResult(void);
McPollResult mcPollWithExtendedErrors(void);
McPollResult mcPollNonnegativeResult(s32 *resultOut);
McPollResult mcPollNormalizedCommandStatus(void);
McPollResult mcPollCommandStatusWithResult(s32 *resultOut);
McPollResult mcPollZeroCommandResult(void);
McPollResult mcPollCompletionStatus(void);
McPollResult mcPollWriteCompletion(void);

#endif
