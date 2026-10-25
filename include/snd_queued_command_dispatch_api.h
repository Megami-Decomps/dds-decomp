#ifndef DDS_SND_QUEUED_COMMAND_DISPATCH_API_H
#define DDS_SND_QUEUED_COMMAND_DISPATCH_API_H

#include "common.h"

typedef s32 SndQueuedCommandDispatchResult;
enum {
    SND_QUEUED_COMMAND_DISPATCH_RESULT_SENT = 0,
    SND_QUEUED_COMMAND_DISPATCH_RESULT_NOT_SENT = 1
};

/* Not sent means the queue was empty or SIF declined the packet. */
SndQueuedCommandDispatchResult sndTryDispatchQueuedCommand(void);

#endif /* DDS_SND_QUEUED_COMMAND_DISPATCH_API_H */
