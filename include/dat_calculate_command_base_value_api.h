#ifndef DAT_CALCULATE_COMMAND_BASE_VALUE_API_H
#define DAT_CALCULATE_COMMAND_BASE_VALUE_API_H

#include "btl_command_id.h"

struct datUnitWork_t;

/* commandId must select a command record; the provider indexes it unchecked. */
s32 datCalculateCommandBaseValue(
    struct datUnitWork_t *battler, BtlCommandId commandId);

#endif /* DAT_CALCULATE_COMMAND_BASE_VALUE_API_H */
