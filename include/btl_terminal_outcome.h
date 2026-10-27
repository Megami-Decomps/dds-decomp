#ifndef BTL_TERMINAL_OUTCOME_H
#define BTL_TERMINAL_OUTCOME_H

#include "common.h"

/* A callback supplies a terminal outcome; NO_OVERRIDE leaves the unit scan in control. */
typedef s32 BtlTerminalOutcomeOverrideResult;
typedef BtlTerminalOutcomeOverrideResult (*BtlTerminalOutcomeOverrideCallback)(void);
enum {
    BTL_TERMINAL_OUTCOME_OVERRIDE_NO_OVERRIDE = -1,
};

typedef s32 BtlTerminalOutcomeReadiness;
enum {
    BTL_TERMINAL_OUTCOME_NOT_READY = 0,
    BTL_TERMINAL_OUTCOME_READY = 1,
};

BtlTerminalOutcomeReadiness btlResolveBattleTerminalOutcome(void);

/* Known outcomes published by btlResolveBattleTerminalOutcome. Other
 * override and scene-specific codes may also be stored in terminalOutcome. */
enum BtlTerminalOutcome {
    BTL_TERMINAL_OUTCOME_NONE = 0,
    BTL_TERMINAL_OUTCOME_WIN = 1,
    BTL_TERMINAL_OUTCOME_LOSS = 2,
    BTL_TERMINAL_OUTCOME_ALL_STONE = 3,
    BTL_TERMINAL_OUTCOME_ESCAPE = 4,
    BTL_TERMINAL_OUTCOME_SERIAL = 5,
};

#endif
