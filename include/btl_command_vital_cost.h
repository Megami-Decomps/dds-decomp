#ifndef BTL_COMMAND_VITAL_COST_H
#define BTL_COMMAND_VITAL_COST_H

#include "common.h"
#include "btl_command_id.h"

struct BtlUnit;

typedef s32 BtlCommandVitalCostResult;

/* 0 means affordable; 1 and 2 identify the failed HP and MP checks. */
#define BTL_COMMAND_VITAL_COST_OK 0
#define BTL_COMMAND_VITAL_COST_INSUFFICIENT_HP 1
#define BTL_COMMAND_VITAL_COST_INSUFFICIENT_MP 2

BtlCommandVitalCostResult btlCheckCommandVitalCost(
    struct BtlUnit *unit, BtlCommandId commandId);

#endif /* BTL_COMMAND_VITAL_COST_H */
