#ifndef SCR_COMMAND_TIMER_API_H
#define SCR_COMMAND_TIMER_API_H

#include "common.h"
#include "scr_command_yield_count.h"

/* During a command callback, return the number of earlier trace passes that
 * yielded while it was pending. The first invocation sees zero; each yielded
 * pass adds one, and a continuing or finishing handler resets the count. This
 * is callback wait state, not wall-clock time, and is valid only in the callback. */
ScrCommandYieldCount scrGetCommandTimer(void);

#endif /* SCR_COMMAND_TIMER_API_H */
