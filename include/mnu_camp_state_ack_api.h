#ifndef MNU_CAMP_STATE_ACK_API_H
#define MNU_CAMP_STATE_ACK_API_H

#include "common.h"

typedef s32 MnuCampStateAcknowledgementResult;
enum {
    MNU_CAMP_STATE_ACK_NOT_ACTIVE = 0,
    MNU_CAMP_STATE_ACK_ACTIVE = 1
};

/* Consume CLEANED_UP back to IDLE, then report whether camp remains active. */
MnuCampStateAcknowledgementResult mnuAcknowledgeCampState(void);

#endif /* MNU_CAMP_STATE_ACK_API_H */
