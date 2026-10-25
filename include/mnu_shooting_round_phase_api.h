#ifndef MNU_SHOOTING_ROUND_PHASE_API_H
#define MNU_SHOOTING_ROUND_PHASE_API_H

#include "common.h"

typedef s32 MnuShootingRoundPhaseResult;
enum {
    MNU_SHOOTING_ROUND_RESULT_NO_ACTION = 0,
    MNU_SHOOTING_ROUND_RESULT_ROUND_COMPLETE = 1,
    MNU_SHOOTING_ROUND_RESULT_SESSION_STOP = 2,
    MNU_SHOOTING_ROUND_RESULT_SESSION_RESET = 3,
};

struct MnuShootingWork;

MnuShootingRoundPhaseResult mnuAdvanceShootingRoundPhase(
    struct MnuShootingWork *work);

#endif /* MNU_SHOOTING_ROUND_PHASE_API_H */
