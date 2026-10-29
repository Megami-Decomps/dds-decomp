#ifndef MC_SYNC_API_H
#define MC_SYNC_API_H

#include "common.h"

/* sceMcSync's return reports polling progress, while its result output is
 * meaningful only after completion and carries the command's status, count,
 * or file descriptor. */
typedef s32 McSyncPollStatus;
typedef s32 McSyncCommandResult;

McSyncPollStatus sceMcSync(s32 mode, s32 *commandIdOut,
                           McSyncCommandResult *commandResultOut);

#endif /* MC_SYNC_API_H */
