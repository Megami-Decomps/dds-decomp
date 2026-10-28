#ifndef DAT_BATTLE_ADJUSTMENT_CONDITION_H
#define DAT_BATTLE_ADJUSTMENT_CONDITION_H

#include "common.h"

/* Packed encounter condition: the high half selects how to interpret the
 * low-half solar-phase mask or model-flag index. */
typedef u32 DatBattleAdjustmentCondition;

enum DatBattleAdjustmentConditionType {
    DAT_BATTLE_ADJUSTMENT_CONDITION_NONE = 0,
    DAT_BATTLE_ADJUSTMENT_CONDITION_SOLAR_PHASE_MASK = 1,
    DAT_BATTLE_ADJUSTMENT_CONDITION_MODEL_FLAG = 2
};

#endif /* DAT_BATTLE_ADJUSTMENT_CONDITION_H */
