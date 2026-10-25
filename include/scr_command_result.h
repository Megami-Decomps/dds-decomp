#ifndef SCR_COMMAND_RESULT_H
#define SCR_COMMAND_RESULT_H

#include "common.h"

/* A command remains at its current instruction while pending. Any nonzero
 * callback result completes it and lets the interpreter consume its operands. */
typedef u32 ScrCommandResult;
#define SCR_COMMAND_RESULT_PENDING 0
#define SCR_COMMAND_RESULT_COMPLETE 1

#endif /* SCR_COMMAND_RESULT_H */
