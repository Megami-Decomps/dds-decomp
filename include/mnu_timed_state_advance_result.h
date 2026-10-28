#ifndef MNU_TIMED_STATE_ADVANCE_RESULT_H
#define MNU_TIMED_STATE_ADVANCE_RESULT_H

#include "common.h"

struct MenuStateRecord;

/* Completion stays latched until the caller clears the completed bit. */
typedef s32 MnuTimedStateAdvanceResult;
enum MnuTimedStateAdvanceResultValue {
    MNU_TIMED_STATE_ADVANCE_PENDING = 0,
    MNU_TIMED_STATE_ADVANCE_COMPLETE = 1,
};

MnuTimedStateAdvanceResult mnuAdvanceTimedStateRecord(struct MenuStateRecord *record);

#endif /* MNU_TIMED_STATE_ADVANCE_RESULT_H */
