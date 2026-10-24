#ifndef BTL_TERMINAL_OUTCOME_H
#define BTL_TERMINAL_OUTCOME_H

/* Known outcomes published by btlResolveBattleTerminalOutcome. Other
 * override and scene-specific codes may also be stored in eventReady. */
enum BtlTerminalOutcome {
    BTL_TERMINAL_OUTCOME_NONE = 0,
    BTL_TERMINAL_OUTCOME_WIN = 1,
    BTL_TERMINAL_OUTCOME_LOSS = 2,
    BTL_TERMINAL_OUTCOME_ALL_STONE = 3,
    BTL_TERMINAL_OUTCOME_ESCAPE = 4,
    BTL_TERMINAL_OUTCOME_SERIAL = 5,
};

#endif
