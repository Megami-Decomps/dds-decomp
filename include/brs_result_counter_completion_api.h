#ifndef BRS_RESULT_COUNTER_COMPLETION_API_H
#define BRS_RESULT_COUNTER_COMPLETION_API_H

#include "common.h"

/* Completion follows six successful settled polls; unsettled polls do not
 * discard accumulated progress. */
typedef s32 BrsResultCounterCompletionResult;
enum {
    BRS_RESULT_COUNTER_COMPLETION_PENDING = 0,
    BRS_RESULT_COUNTER_COMPLETION_COMPLETE = 1
};

BrsResultCounterCompletionResult brsPollResultCounterCompletion(void);

#endif /* BRS_RESULT_COUNTER_COMPLETION_API_H */
