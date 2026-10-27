#ifndef DDS_SCR_COMMAND_YIELD_COUNT_H
#define DDS_SCR_COMMAND_YIELD_COUNT_H

#include "common.h"

/* Prior interpreter trace passes that yielded while a script command remained
 * pending; callbacks observe the count before the current pass is recorded. */
typedef u32 ScrCommandYieldCount;

#endif /* DDS_SCR_COMMAND_YIELD_COUNT_H */
