#ifndef PTY_EVENT_PENALTY_API_H
#define PTY_EVENT_PENALTY_API_H

#include "common.h"

/* Script-selected party HP penalties; unrecognized values have no effect. */
typedef s32 PtyEventPenaltyMode;
enum {
    PTY_EVENT_PENALTY_HP_LOSS_10_PERCENT = 1,
    PTY_EVENT_PENALTY_HP_LOSS_50_PERCENT = 2,
    PTY_EVENT_PENALTY_SET_LIVING_HP_TO_ONE = 3,
    PTY_EVENT_PENALTY_HP_LOSS_10_PERCENT_CHANNEL_MIN_0X80 = 4,
    PTY_EVENT_PENALTY_HP_LOSS_10_PERCENT_CHANNEL_MIN_0X40 = 5,
    PTY_EVENT_PENALTY_HP_LOSS_10_PERCENT_CHANNEL_MIN_0X10 = 6,
};

void ptyApplyEventPenalty(PtyEventPenaltyMode mode);

#endif /* PTY_EVENT_PENALTY_API_H */
