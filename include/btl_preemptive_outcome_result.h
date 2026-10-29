#ifndef BTL_PREEMPTIVE_OUTCOME_RESULT_H
#define BTL_PREEMPTIVE_OUTCOME_RESULT_H

#include "common.h"

/* The provider returns one of the two selected outcomes. */
typedef s32 BtlPreemptiveOutcomeResult;
enum {
    BTL_PREEMPTIVE_OUTCOME_UNSELECTED = 0,
    BTL_PREEMPTIVE_OUTCOME_PREEMPTIVE = 1,
    BTL_PREEMPTIVE_OUTCOME_NOT_PREEMPTIVE = 2,
};

BtlPreemptiveOutcomeResult btlSelectPreemptiveOutcome(void);

#endif /* BTL_PREEMPTIVE_OUTCOME_RESULT_H */
