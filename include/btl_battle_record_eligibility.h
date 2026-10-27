#ifndef BTL_BATTLE_RECORD_ELIGIBILITY_H
#define BTL_BATTLE_RECORD_ELIGIBILITY_H

#include "common.h"

struct BtlUnit;

/* Final result after the mode-specific gate and ordinary record checks. */
typedef s32 BtlBattleRecordEligibilityResult;

#define BTL_BATTLE_RECORD_INELIGIBLE ((BtlBattleRecordEligibilityResult)0)
#define BTL_BATTLE_RECORD_ELIGIBLE ((BtlBattleRecordEligibilityResult)1)

/* The override rejects on zero; nonzero permits the ordinary checks to run. */
typedef s32 BtlBattleRecordEligibilityOverrideResult;

#define BTL_BATTLE_RECORD_OVERRIDE_REJECT \
    ((BtlBattleRecordEligibilityOverrideResult)0)
#define BTL_BATTLE_RECORD_OVERRIDE_CONTINUE \
    ((BtlBattleRecordEligibilityOverrideResult)1)

typedef BtlBattleRecordEligibilityOverrideResult (*BtlBattleRecordEligibilityOverrideCallback)(
    struct BtlUnit *actor, struct BtlUnit *target, s32 speciesIndex);

BtlBattleRecordEligibilityResult btlIsBattleRecordEligible(
    struct BtlUnit *actor, struct BtlUnit *target, s32 recordIndex, s32 speciesIndex);

#endif /* BTL_BATTLE_RECORD_ELIGIBILITY_H */
