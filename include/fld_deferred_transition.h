#ifndef FLD_DEFERRED_TRANSITION_H
#define FLD_DEFERRED_TRANSITION_H

#include "common.h"

typedef s32 FldDeferredCommandDisposition;

enum {
    FLD_DEFERRED_COMMAND_CLEAR_PENDING = 0,
    FLD_DEFERRED_COMMAND_PRESERVE_PENDING = 1,
};

FldDeferredCommandDisposition fldStartDeferredActorTransition(s32 command, u32 parameter);

#endif /* FLD_DEFERRED_TRANSITION_H */
