#include "sdk_ps2.h"
#include "common.h"
#include "pty_experience_threshold_type.h"
#include "scr_integer_parameter_api.h"
#include "bf_flw0_header.h"
#include "scr_procedure_start_api.h"
#include "evt_script_context.h"
#include "evt_solar.h"
#include "evt_item_value_threshold_api.h"
#include "evt_random_status_clear_api.h"
#include "pty_roster_value_record.h"
#include "pty_adjust_item_quantity_api.h"
#include "dat_state.h"
#include "dat_battle_adjustment_condition.h"
#include "dat_currency_requirement_api.h"
#include "dat_calculated_stat_value_api.h"
#include "dat_calculate_command_base_value_api.h"
#include "dat_unit_dying_api.h"
#include "btl_scene_party_level_threshold_api.h"
#include "btl_actor_attribute_api.h"
#include "dat_affinity_selector.h"
#include "dat_affinity_value_api.h"
#include "dat_effective_affinity_api.h"
#include "btl_conditional_encounter_roll_api.h"
#include "dat_world_field_flags.h"
#include "btl_affinity_row.h"
#include "btl_resolve_unit_value_api.h"
#include "evt_randomize_entry_api.h"
#include "mdl_flag_api.h"
#include "pty_active_roster_index_api.h"
#include "pty_roster_api.h"
#include "pty_party_unit_presence_api.h"
#include "pty_frontline_promotion_api.h"
#include "pty_status_query_api.h"
#include "pty_event_penalty_api.h"
#include "pty_field_hp_attrition_api.h"
#include "pty_read_roster_stat_api.h"
#include "dat_unit_has_skill_api.h"
#include "pty_skill_recovery_api.h"
#include "pty_item_quantity_update_view.h"
#include "pty_accumulate_stat_gains_api.h"
#include "eff_random_mod.h"

#define DAT_EXTERNAL_SKILL_TABLE 0x20
#define DAT_INLINE_SKILL_COUNT 0x18
#define DAT_TABLE_SKILL_COUNT 8
#define DAT_SKILL_HP_BONUS_SMALL 0x200
#define DAT_SKILL_HP_BONUS_MEDIUM 0x201
#define DAT_SKILL_HP_BONUS_LARGE 0x202
#define DAT_SKILL_MP_BONUS_SMALL 0x203
#define DAT_SKILL_MP_BONUS_MEDIUM 0x204
#define DAT_SKILL_MP_BONUS_LARGE 0x205
#define DAT_PERCENT_SCALE 100
#define DAT_BONUS_SMALL_PERCENT 10
#define DAT_BONUS_MEDIUM_PERCENT 20
#define DAT_BONUS_LARGE_PERCENT 30
#define DAT_BOOSTED_RESOURCE_LIMIT 1000
#define DAT_BOOSTED_RESOURCE_MAX 999
#define DAT_STATUS_VALUE_MASK 0x7FFF
#define DAT_STATUS_STAT_OVERRIDE 0x1000
#define DAT_PROFILE_STAT_LIMIT 100
#define DAT_PROFILE_STAT_MAX 99
#define DAT_CALC_HIGH_MASK 0xFFFF0000
#define DAT_CALC_SKIP_STATUS_MASK 0x70000
#define DAT_QUARTER_PERCENT 25
#define DAT_CURRENCY_MAX 0x98967F


extern s32 ptyComputeMaxHp(datUnitWork_t *unit);
extern s32 ptyComputeMaxMp(datUnitWork_t *unit);


typedef struct UiObject {
    u8 unk_00[0x110];
    u32 flags;
    u8 unk_114[0x10];
    u16 index;
    u16 currentValue;
    u16 maximumValue;
    u8 unk_12A[4];
    u16 statusFlags;
} UiObject;

/* Clear only the requested bits of the unit's halfword status. */
void datResetBadStat(datUnitWork_t *unit, s32 mask) {
    unit->status &= ~mask;
}

/* Add each owned HP bonus separately against the original maximum, truncating
 * each unsigned percentage independently. Only inline-skill units cap at 999. */
u32 datComputeSkillBoostedMaxHp(datUnitWork_t *unit) {
    u32 bonusHp = 0;
    u32 maxHp = ptyComputeMaxHp(unit);

    if (datUnitHasSkill(unit, DAT_SKILL_HP_BONUS_SMALL)) {
        bonusHp = maxHp * DAT_BONUS_SMALL_PERCENT / DAT_PERCENT_SCALE;
    }
    if (datUnitHasSkill(unit, DAT_SKILL_HP_BONUS_MEDIUM)) {
        bonusHp += maxHp * DAT_BONUS_MEDIUM_PERCENT / DAT_PERCENT_SCALE;
    }
    if (datUnitHasSkill(unit, DAT_SKILL_HP_BONUS_LARGE)) {
        bonusHp += maxHp * DAT_BONUS_LARGE_PERCENT / DAT_PERCENT_SCALE;
    }
    maxHp += bonusHp;
    if (!(unit->flags & DAT_EXTERNAL_SKILL_TABLE) && maxHp >= DAT_BOOSTED_RESOURCE_LIMIT) {
        maxHp = DAT_BOOSTED_RESOURCE_MAX;
    }
    return maxHp;
}

/* MP bonuses have the same independent truncation and conditional cap as HP. */
u32 datComputeSkillBoostedMaxMp(datUnitWork_t *unit) {
    u32 bonusMp = 0;
    u32 maxMp = ptyComputeMaxMp(unit);

    if (datUnitHasSkill(unit, DAT_SKILL_MP_BONUS_SMALL)) {
        bonusMp = maxMp * DAT_BONUS_SMALL_PERCENT / DAT_PERCENT_SCALE;
    }
    if (datUnitHasSkill(unit, DAT_SKILL_MP_BONUS_MEDIUM)) {
        bonusMp += maxMp * DAT_BONUS_MEDIUM_PERCENT / DAT_PERCENT_SCALE;
    }
    if (datUnitHasSkill(unit, DAT_SKILL_MP_BONUS_LARGE)) {
        bonusMp += maxMp * DAT_BONUS_LARGE_PERCENT / DAT_PERCENT_SCALE;
    }
    maxMp += bonusMp;
    if (!(unit->flags & DAT_EXTERNAL_SKILL_TABLE) && maxMp >= DAT_BOOSTED_RESOURCE_LIMIT) {
        maxMp = DAT_BOOSTED_RESOURCE_MAX;
    }
    return maxMp;
}

/* Add delta to current HP, then clamp to 0..maxHp. Preserve unsigned
 * addition, signed comparisons and the final s16 cast. */
s32 datAddHp(datUnitWork_t *unit, s32 delta) {
    u32 currentHp;

    currentHp = (u32)unit->hp + delta;
    if ((s32)currentHp < 0) {
        currentHp = 0;
    }
    if ((s32)unit->maxHp < (s32)currentHp) {
        currentHp = unit->maxHp;
    }
    unit->hp = (s16)currentHp;
    return currentHp;
}

/* Add delta to current MP and clamp with the same integer/cast rules as HP. */
s32 datAddMp(datUnitWork_t *unit, s32 delta) {
    u32 currentMp;

    currentMp = (u32)unit->mp + delta;
    if ((s32)currentMp < 0) {
        currentMp = 0;
    }
    if ((s32)unit->maxMp < (s32)currentMp) {
        currentMp = unit->maxMp;
    }
    unit->mp = (s16)currentMp;
    return currentMp;
}

extern s32 ptyGetCurrentProfileId(const datUnitWork_t *unit);
extern u32 prfGetIndexedProfileByte(u16 profileId, s32 statIndex);


/* Add the signed per-unit stat byte to its profile adjustment and clamp
 * the existing signed result to 1..99. Stat index is not checked here. */
DatCalculatedStatValue datGetClampedProfileAdjustedStat(
    const datUnitWork_t *unit, DatBaseStatIndex statIndex) {
    s32 adjustedStat = unit->baseStats[statIndex] +
                prfGetIndexedProfileByte(ptyGetCurrentProfileId(unit), statIndex);

    if (adjustedStat <= 0) {
        adjustedStat = 1;
    }
    if (adjustedStat >= DAT_PROFILE_STAT_LIMIT) adjustedStat = DAT_PROFILE_STAT_MAX;
    return adjustedStat;
}

/* Only an exact masked status of 0x1000 forces one; combinations of low
 * status bits do not take this path. Otherwise use the profile-adjusted stat. */
DatCalculatedStatValue datGetStatWithStatusOverride(
    const datUnitWork_t *unit, DatBaseStatIndex statIndex) {
    if ((unit->status & DAT_STATUS_VALUE_MASK) == DAT_STATUS_STAT_OVERRIDE) {
        return 1;
    }
    return datGetClampedProfileAdjustedStat(unit, statIndex);
}


extern DatEnemyRecord *datEnemyRecords;

/* Search 24 inline skill IDs or eight IDs in the selected external record.
 * Return on the first match; no record-index or pointer validation here. */
DatUnitHasSkillResult datUnitHasSkill(
    const datUnitWork_t *unit, DatSkillId skillId) {
    s32 skillIndex;

    if (!(unit->flags & DAT_EXTERNAL_SKILL_TABLE)) {
        for (skillIndex = 0; skillIndex < DAT_INLINE_SKILL_COUNT; skillIndex++) {
            if (unit->effectData[skillIndex] == skillId) {
                return DAT_UNIT_SKILL_RESULT_PRESENT;
            }
        }
    } else {
        for (skillIndex = 0; skillIndex < DAT_TABLE_SKILL_COUNT; skillIndex++) {
            if (datEnemyRecords[unit->unitId].skills[skillIndex] == skillId) {
                return DAT_UNIT_SKILL_RESULT_PRESENT;
            }
        }
    }
    return DAT_UNIT_SKILL_RESULT_ABSENT;
}

/* Unless a skip-status bit is set, adjust only the low 16 bits by exact masked
 * status: 8/0x100 floor at 200, 2 at 150, 1 at 300, and 4 forces one.
 * High bits are retained; combined or unlisted statuses leave the value alone. */
s32 datRaiseCalculatedValueFloor(const datUnitWork_t *unit, s32 packedValue) {
    if ((packedValue & DAT_CALC_SKIP_STATUS_MASK) == 0) {
        switch (unit->status & DAT_STATUS_VALUE_MASK) {
        case 8:
        case 0x100:
            if ((u16)packedValue < 0xC8) {
                packedValue = (packedValue & DAT_CALC_HIGH_MASK) | 0xC8;
            }
            break;
        case 4:
            packedValue = (packedValue & DAT_CALC_HIGH_MASK) | 1;
            break;
        case 2:
            if ((u16)packedValue < 0x96) {
                packedValue = (packedValue & DAT_CALC_HIGH_MASK) | 0x96;
            }
            break;
        case 1:
            if ((u16)packedValue < 0x12C) {
                packedValue = (packedValue & DAT_CALC_HIGH_MASK) | 0x12C;
            }
            break;
        }
    }
    return packedValue;
}

extern const BtlAffinityRow *datAffinityTablePrimary;
extern const BtlAffinityRow *datAffinityTableAlternate;
extern const BtlAffinityRow *datExternalAffinityTable;

DatEffectiveAffinityResult datGetEffectiveAffinity(
    const datUnitWork_t *unit, DatAffinitySelector selector) {
    DatEffectiveAffinityResult calculatedValue;
    u16 unitId;

    switch (selector) {
    case DAT_AFFINITY_SELECTOR_UNMAPPED:
        return 0;
    case 16:
    case 17:
    case 18:
        return 100;
    }

    unitId = unit->unitId;
    if (!(unit->flags & DAT_EXTERNAL_SKILL_TABLE)) {
        if (unit->flags & 0x1000) {
            calculatedValue = ((const s32 *)datAffinityTablePrimary)[unitId * 19 + selector];
        } else {
            calculatedValue = ((const s32 *)datAffinityTableAlternate)[unitId * 19 + selector];
        }
    } else {
        if (unit->affinityTableIndex != 0) {
            calculatedValue = ((const s32 *)datExternalAffinityTable)[unit->affinityTableIndex * 19 + selector];
        } else {
            calculatedValue = ((const s32 *)datExternalAffinityTable)[datEnemyRecords[unitId].affinityTableIndex * 19 + selector];
        }
    }

    if (selector == 15) {
        calculatedValue = datRaiseCalculatedValueFloor(unit, calculatedValue);
    }
    if (unit->flags & 0x2000) {
        calculatedValue = ((const s32 *)datExternalAffinityTable)[383 * 19 + selector];
    }
    if ((unit->status & DAT_STATUS_VALUE_MASK) == 0x800) {
        calculatedValue = ((const s32 *)datExternalAffinityTable)[382 * 19 + selector];
    }
    if ((unit->status & DAT_STATUS_VALUE_MASK) == 0x1000) {
        calculatedValue = ((const s32 *)datExternalAffinityTable)[381 * 19 + selector];
    }
    if ((unit->status & DAT_STATUS_VALUE_MASK) == 4) {
        if (selector < 2) {
            if (selector >= 0) {
                if ((u16)calculatedValue < 100) {
                    calculatedValue = (calculatedValue & DAT_CALC_HIGH_MASK) | 100;
                }
            }
        }
    }
    return calculatedValue;
}

DatAffinityValue datGetAisyoValue(const datUnitWork_t *unit, DatAffinitySelector selector) {
    return (u16)datGetEffectiveAffinity(unit, selector);
}

u32 datGetAisyoFlag(const datUnitWork_t *unit, DatAffinitySelector selector) {
    return datGetEffectiveAffinity(unit, selector) & DAT_CALC_HIGH_MASK;
}

/* Map one exact channel flag to an affinity-table column. Flag one and
 * unknown/combined flags have no column; several flags share a destination. */
DatAffinitySelector datFlagToElementIndex(s32 channelFlag) {
    DatAffinitySelector selector = DAT_AFFINITY_SELECTOR_UNMAPPED;

    switch (channelFlag) {
    case 1:
        selector = DAT_AFFINITY_SELECTOR_UNMAPPED;
        break;
    case 2:
        selector = 4;
        break;
    case 4:
        selector = 3;
        break;
    case 8:
        selector = 14;
        break;
    case 0x10:
        selector = 12;
        break;
    case 0x20:
        selector = 13;
        break;
    case 0x40:
        selector = 7;
        break;
    case 0x80:
        selector = 11;
        break;
    case 0x100:
        selector = 14;
        break;
    case 0x200:
        selector = 10;
        break;
    case 0x400:
        selector = 9;
        break;
    case 0x800:
        selector = 9;
        break;
    case 0x1000:
        selector = 7;
        break;
    case 0x2000:
        selector = 7;
        break;
    case 0x4000:
        selector = 9;
        break;
    }
    return selector;
}

DatUnitDyingResult datCheckDying(const datUnitWork_t *unit) {
    return unit->hp * DAT_PERCENT_SCALE / unit->maxHp < DAT_QUARTER_PERCENT;
}



/* Add delta using the existing s32 addition, then clamp the currency balance
 * to 0..9,999,999. No overflow validation is performed before the clamp. */
void datAddCurrencyClamped(s32 delta) {
    s32 balance = DDS3_GBWK->header.currency + delta;
    if (balance < 0) {
        balance = 0;
    }
    if (balance > DAT_CURRENCY_MAX) {
        balance = DAT_CURRENCY_MAX;
    }
    DDS3_GBWK->header.currency = balance;
}

/* Compare the signed balance against the requested amount; no debit or
 * validation is performed, so negative requests keep their original semantics. */
DatCurrencyRequirementResult datHasEnoughCurrency(s32 requiredAmount) {
    if (DDS3_GBWK->header.currency < requiredAmount) {
        return DAT_CURRENCY_REQUIREMENT_UNMET;
    }
    return DAT_CURRENCY_REQUIREMENT_MET;
}

#include "common.h"
#include "scr.h"
#include "evt_run_context_api.h"
#include "scr_work_task_api.h"
#include "dat_state.h"
#include "sdf.h"
#include "btl_action.h"
#include "dat_command.h"
#include "dat_command_record_index.h"

enum {
    PTY_ACTIVE_ROSTER_COUNT = 5,
    PTY_RECOVERY_DELTA = 9999,
    PTY_MAX_LEVEL = 99,
    PTY_LEVEL_LIMIT = 100
};

enum {
    EVT_DEFAULT_STAT_VALUE = 100,
    EVT_ROSTER_DETAIL_KIND = 5,
    EVT_DEFAULT_PARTY_RECORD_FLAGS_MASK = 4,
    EVT_ALTERNATE_PARTY_RECORD_FLAGS_MASK = 0x20,
    EVT_CONTEXT_BYTES = 0x18
};

enum {
    PTY_ITEM_MAX_QUANTITY = 99,
    PTY_ITEM_QUANTITY_LIMIT = 100,
    PTY_ITEM_SINGLE_MAX = 1,
    PTY_ITEM_SINGLE_LIMIT = 2
};

enum {
    BTL_SCENE_RECORD_BYTES = 0x28,
    BTL_SCENE_ENEMY_SLOT_COUNT = 11,
    BTL_ENEMY_RECORD_BYTES = 0x4C,
    BTL_ENEMY_LEVEL_OFFSET = 5
};

extern u8 ptyAffinityPermutationOrders[];
extern PtyAffinityMatchResult
ptyUnitMatchesAffinityRequirement(
    const datUnitWork_t *unit, BtlCommandId commandId, u8 requirementIndex);


extern EventRosterStat *datRosterDetails;
extern DatEnemyRecord *datEnemyRecords;
extern s32 btlConditionalEncounterCachedInterval;
extern datUnitWork_t *D_003BAA04;

extern PtyRosterValueRecord ptyRosterMenuValues[];
extern PtyItemQuantityUpdateView ptyItemQuantityUpdates[];

extern scrProcessWork_s *evtWorkScriptTask;

extern EvtScriptContext evtWorkScriptContext;

extern struct scrProcessWork_s *scrSetIntReturnValue(s32 retVal);
extern struct scrProcessWork_s *scrSetFloatReturnValue(f32 value);
extern datUnitWork_t *dds3FindEntry(DatUnitId unitId);
extern void ptyMergeStockSkills(datUnitWork_t *unit);
extern u8 btlIsRuntimeAllocated(void);
extern f32 btlGetEncounterAdjustmentScale(void);
extern u32 btlGetEscapeAttemptCount(void);
extern void evtBitOn(s32 flagIndex);

/* Flag-range items set their model flag regardless of quantityDelta.
 * Other items add the delta to their byte quantity and clamp it. */
void ptyAdjustItemQuantity(PtyItemId itemId, PtyItemQuantityDelta quantityDelta) {
    s32 quantity;

    if ((u32)(itemId - PTY_ITEM_FLAG_FIRST) < PTY_ITEM_FLAG_COUNT) {
        evtBitOn(itemId + PTY_ITEM_MODEL_FLAG_BASE);
        return;
    }
    quantity = DDS3_GBWK->inventory.counts[itemId];
    quantity += quantityDelta;
    if (quantity < 0) {
        quantity = 0;
    }

    if (itemId >= 0xA0) {
        if (quantity >= PTY_ITEM_QUANTITY_LIMIT) {
            quantity = PTY_ITEM_MAX_QUANTITY;
        }
    } else if (itemId >= PTY_ITEM_FLAG_FIRST) {
        if (quantity >= PTY_ITEM_SINGLE_LIMIT) {
            quantity = PTY_ITEM_SINGLE_MAX;
        }
    } else if (itemId >= 0x60) {
        if (quantity >= PTY_ITEM_QUANTITY_LIMIT) {
            quantity = PTY_ITEM_MAX_QUANTITY;
        }
    } else {
        quantity = quantity < PTY_ITEM_QUANTITY_LIMIT ? quantity : PTY_ITEM_MAX_QUANTITY;
    }
    DDS3_GBWK->inventory.counts[itemId] = quantity;
}

/* Flag-range items test their model flag and ignore minimumQuantity.
 * Other items require at least the requested byte quantity. */
EvtItemValueThresholdResult evtCheckValueThreshold(PtyItemId itemId, s32 minimumQuantity) {
    if ((u32)(itemId - PTY_ITEM_FLAG_FIRST) < PTY_ITEM_FLAG_COUNT) {
        return evtBitCheck(itemId + PTY_ITEM_MODEL_FLAG_BASE) != 0 ?
                EVT_ITEM_VALUE_THRESHOLD_MET : EVT_ITEM_VALUE_THRESHOLD_NOT_MET;
    }
    if (DDS3_GBWK->inventory.counts[itemId] < minimumQuantity) {
        return EVT_ITEM_VALUE_THRESHOLD_NOT_MET;
    }
    return EVT_ITEM_VALUE_THRESHOLD_MET;
}

/* Flag-range items have one slot; every quantity range caps at 99. */
u8 ptyIsInventoryItemAtCapacity(PtyItemId itemId) {
    s32 quantity = DDS3_GBWK->inventory.counts[itemId];

    if (itemId >= 0xA0) {
        if (quantity >= PTY_ITEM_MAX_QUANTITY) {
            return 1;
        }
    } else if (itemId >= PTY_ITEM_FLAG_FIRST) {
        if (quantity > 0) {
            return 1;
        }
    } else if (itemId >= 0x60) {
        if (quantity >= PTY_ITEM_MAX_QUANTITY) {
            return 1;
        }
    } else if (quantity >= PTY_ITEM_MAX_QUANTITY) {
        return 1;
    }
    return 0;
}

u8 evtGetFlaggedRosterValue(datUnitWork_t *entry) {
    if ((entry->flags & 0x20) == 0) {
        return 0;
    }
    return datEnemyRecords[entry->unitId].pad04;
}

PtyActiveRosterIndex dds3FindEntryIndex(DatUnitId unitId) {
    datUnitWork_t *entry = DDS3_GBWK->party;
    PtyActiveRosterIndex slotIndex = 0;

    do {
        if (entry->flags & DAT_PARTY_FLAG_OCCUPIED) {
            if (entry->unitId == unitId) {
                return slotIndex;
            }
        }
        slotIndex++;
        entry++;
    } while (slotIndex < PTY_ACTIVE_ROSTER_COUNT);
    return PTY_ACTIVE_ROSTER_INDEX_NOT_FOUND;
}

/* Read one indexed signed base-stat value from the first active roster record. */
s8 ptyReadSignedRosterStatByte(DatBaseStatIndex statIndex) {
    return DDS3_GBWK->party[0].baseStats[statIndex];
}

extern void sdfRaisePackedChannelValue(datUnitWork_t *, u32 minimumValue);

/* Event penalties affect living roster slots, then optionally raise a status channel. */
void ptyApplyEventPenalty(PtyEventPenaltyMode penaltyMode) {
    s32 nextHp, loss, slotIndex;
    if (penaltyMode == PTY_EVENT_PENALTY_HP_LOSS_10_PERCENT ||
        penaltyMode == PTY_EVENT_PENALTY_HP_LOSS_10_PERCENT_CHANNEL_MIN_0X80 ||
        penaltyMode == PTY_EVENT_PENALTY_HP_LOSS_10_PERCENT_CHANNEL_MIN_0X40 ||
        penaltyMode == PTY_EVENT_PENALTY_HP_LOSS_10_PERCENT_CHANNEL_MIN_0X10) {
        datUnitWork_t *entry = DDS3_GBWK->party;
        slotIndex = 0;
        do {
            if (entry->hp != 0) {
                nextHp = entry->hp;
                loss = nextHp / 10;
                if (loss == 0) loss = 1;
                nextHp -= loss;
                if (nextHp <= 0) nextHp = 1;
                entry->hp = nextHp;
            }
            slotIndex++;
            entry++;
        } while (slotIndex < PTY_ACTIVE_ROSTER_COUNT);
    }
    if (penaltyMode == PTY_EVENT_PENALTY_HP_LOSS_50_PERCENT) {
        datUnitWork_t *entry = DDS3_GBWK->party;
        slotIndex = 0;
        do {
            if (entry->hp != 0) {
                nextHp = entry->hp;
                loss = (u32)nextHp / 2;
                if (loss == 0) loss = 1;
                nextHp -= loss;
                if (nextHp <= 0) nextHp = 1;
                entry->hp = nextHp;
            }
            slotIndex++;
            entry++;
        } while (slotIndex < PTY_ACTIVE_ROSTER_COUNT);
    }
    if (penaltyMode == PTY_EVENT_PENALTY_SET_LIVING_HP_TO_ONE) {
        datUnitWork_t *entry = DDS3_GBWK->party;
        slotIndex = 0;
        do {
            if (entry->hp != 0) entry->hp = 1;
            slotIndex++;
            entry++;
        } while (slotIndex < PTY_ACTIVE_ROSTER_COUNT);
    }
    if (penaltyMode >= PTY_EVENT_PENALTY_HP_LOSS_10_PERCENT_CHANNEL_MIN_0X80 &&
        penaltyMode <= PTY_EVENT_PENALTY_HP_LOSS_10_PERCENT_CHANNEL_MIN_0X10) {
        slotIndex = 0;
        do {
            if (DDS3_GBWK->party[slotIndex].hp != 0) {
                if (penaltyMode == PTY_EVENT_PENALTY_HP_LOSS_10_PERCENT_CHANNEL_MIN_0X80) sdfRaisePackedChannelValue(&DDS3_GBWK->party[slotIndex], 0x80);
                if (penaltyMode == PTY_EVENT_PENALTY_HP_LOSS_10_PERCENT_CHANNEL_MIN_0X40) sdfRaisePackedChannelValue(&DDS3_GBWK->party[slotIndex], 0x40);
                if (penaltyMode == PTY_EVENT_PENALTY_HP_LOSS_10_PERCENT_CHANNEL_MIN_0X10) sdfRaisePackedChannelValue(&DDS3_GBWK->party[slotIndex], 0x10);
            }
            slotIndex++;
        } while (slotIndex < PTY_ACTIVE_ROSTER_COUNT);
    }
}


/* Apply field HP attrition without allowing a living roster entry to reach zero. */
void ptyApplyFieldHpAttrition(PtyFieldHpAttritionMode mode) {
    s32 nextHp, loss, slotIndex;
    if (mode == PTY_FIELD_HP_ATTRITION_FRONTLINE_STATUS_0X80_3_PERCENT) {
        datUnitWork_t *entry = DDS3_GBWK->party;
        slotIndex = 0;
        do {
            if ((entry->flags & DAT_PARTY_FLAG_FRONTLINE) && (entry->status & 0x80)) {
                if (entry->hp != 0) {
                    nextHp = entry->hp;
                    loss = nextHp * 3 / 100;
                    if (loss == 0) loss = 1;
                    nextHp -= loss;
                    if (nextHp <= 0) nextHp = 1;
                    entry->hp = nextHp;
                }
            }
            slotIndex++;
            entry++;
        } while (slotIndex < PTY_ACTIVE_ROSTER_COUNT);
    }
    if (mode == PTY_FIELD_HP_ATTRITION_FRONTLINE_3_PERCENT) {
        datUnitWork_t *entry = DDS3_GBWK->party;
        slotIndex = 0;
        do {
            if (entry->flags & DAT_PARTY_FLAG_FRONTLINE) {
                if (entry->hp != 0) {
                    nextHp = entry->hp;
                    loss = nextHp * 3 / 100;
                    if (loss == 0) loss = 1;
                    nextHp -= loss;
                    if (nextHp <= 0) nextHp = 1;
                    entry->hp = nextHp;
                }
            }
            slotIndex++;
            entry++;
        } while (slotIndex < PTY_ACTIVE_ROSTER_COUNT);
    }
}


/* Recover HP/MP in each occupied roster slot and preserve only status bit 15. */
void ptyRecoverAllUnits(void) {
    s32 slotIndex = 0;

    do {
        datUnitWork_t *entry = &DDS3_GBWK->party[slotIndex];
        if (entry->flags & DAT_PARTY_FLAG_OCCUPIED) {
            datAddHp(entry, PTY_RECOVERY_DELTA);
            datAddMp(entry, PTY_RECOVERY_DELTA);
            entry->status &= 0x8000;
        }
        slotIndex++;
    } while (slotIndex < PTY_ACTIVE_ROSTER_COUNT);
}

/* Check occupied, nonzero-HP roster members for a status bit. */
PtyPartyStatusMatchResult ptyAnyLivingUnitHasStatus(
    u32 statusMask, PtyPartyStatusScope scope) {
    datUnitWork_t *entry = DDS3_GBWK->party;
    s32 slotIndex = 0;

    do {
        if (entry->flags & DAT_PARTY_FLAG_OCCUPIED) {
            if (entry->hp != 0) {
                if (scope != PTY_PARTY_STATUS_SCOPE_FRONTLINE_ONLY ||
                    (entry->flags & DAT_PARTY_FLAG_FRONTLINE)) {
                    if (entry->status & statusMask) {
                        return PTY_PARTY_STATUS_MATCH_FOUND;
                    }
                }
            }
        }
        slotIndex++;
        entry++;
    } while (slotIndex < PTY_ACTIVE_ROSTER_COUNT);
    return PTY_PARTY_STATUS_MATCH_NOT_FOUND;
}

void datAddExp(datUnitWork_t *unit, s32 increment) {
    unit->totalExp += increment;
}

/* Apply the owned skill's positive HP/MP recovery rate; unsupported skills do nothing. */
void ptyApplySkillRecovery(datUnitWork_t *unit, DatSkillId skillId) {
    f32 recoveryRate;
    s32 hpRecovery;
    s32 mpRecovery;

    if (datUnitHasSkill(unit, skillId) == DAT_UNIT_SKILL_RESULT_ABSENT) {
        return;
    }
    hpRecovery = 0;
    mpRecovery = 0;
    recoveryRate = datAbilityParameters[skillId - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
    switch (skillId) {
    case 0x22B:
        if (recoveryRate > 0.0f) {
            hpRecovery = (s32)((f32)unit->maxHp * recoveryRate);
            mpRecovery = (s32)((f32)unit->maxMp * recoveryRate);
        }
        break;
    case 0x22A:
    case 0x22C:
    case 0x250:
        if (recoveryRate > 0.0f) {
            mpRecovery = (s32)((f32)unit->maxMp * recoveryRate);
        }
        break;
    }
    if (hpRecovery > 0) {
        datAddHp(unit, hpRecovery);
    }
    if (mpRecovery > 0) {
        datAddMp(unit, mpRecovery);
    }
}

/* Apply the two selected recovery skills to occupied frontline entries. */
void evtUpdateFlaggedStats(void) {
    s32 slotIndex = 0;

    do {
        datUnitWork_t *entry = &DDS3_GBWK->party[slotIndex];
        if (entry->flags & DAT_PARTY_FLAG_OCCUPIED) {
            if (entry->flags & DAT_PARTY_FLAG_FRONTLINE) {
                ptyApplySkillRecovery(entry, 0x22C);
                ptyApplySkillRecovery(entry, 0x250);
            }
        }
        slotIndex++;
    } while (slotIndex < PTY_ACTIVE_ROSTER_COUNT);
}

/* Return whether an occupied frontline entry owns the requested skill. */
s32 evtHasMatchingFlaggedEntry(DatSkillId skillId) {
    s32 slotIndex = 0;
    do {
        datUnitWork_t *entry = &DDS3_GBWK->party[slotIndex];
        if ((entry->flags & DAT_PARTY_FLAG_OCCUPIED) && (entry->flags & DAT_PARTY_FLAG_FRONTLINE)) {
            if (datUnitHasSkill(entry, skillId)) {
                return 1;
            }
        }
        slotIndex++;
    } while (slotIndex < PTY_ACTIVE_ROSTER_COUNT);
    return 0;
}

/* Resolve the command's unmodified integer value for this entry.
 * Mode one scales max HP and floors the result at one unless the entry is
 * enemy-flagged; mode two returns a fixed value subject to the enemy flag gate.
 * commandId selects the record; cost holds the resolved result. */
DatCommandBaseCost datCalculateCommandBaseValue(
    datUnitWork_t *battler, BtlCommandId commandId) {
    s32 cost = 0;
    DatCommandRecord *commands = datCommandRecords;

    switch (commands[commandId].costMode) {
    case DAT_COMMAND_COST_MODE_HP:
        if (battler->flags & 0x20) {
            return 0;
        }
        cost = battler->maxHp * commands[commandId].costPercentage / 100 +
                commands[commandId].costBase;
        if (cost <= 0) {
            cost = 1;
        }
        break;
    case DAT_COMMAND_COST_MODE_MP:
        if ((battler->flags & 0x20) &&
            (datEnemyRecords[battler->unitId].flags & 0x10)) {
            return 0;
        }
        cost = commands[commandId].costPercentage;
        break;
    }
    return cost;
}

void ptyInitRuntime(void) {
    s32 i;

    for (i = 0; i < 5; i++) {
        memset(&DDS3_GBWK->party[i], 0, sizeof(datUnitWork_t));
        DDS3_GBWK->partyOrder[i] = i;
    }
    DDS3_GBWK->partyCount = 0;
    DDS3_GBWK->party[0] = D_003BAA04[1];
    DDS3_GBWK->party[1] = D_003BAA04[3];
    DDS3_GBWK->party[2] = D_003BAA04[4];
    DDS3_GBWK->partyCount = 3;
    for (i = 0; i < 5; i++) {
        DDS3_GBWK->party[i].menuValue = 0;
    }
    DDS3_GBWK->header.currency = 0x7D0;
    for (i = 0; i < 192; i++) {
        DDS3_GBWK->inventory.counts[i] = 0;
    }
    for (i = 0; i < 16; i++) {
        memset(&DDS3_GBWK->templates[i], 0, sizeof(datUnitWork_t));
    }
}

/* Return the record's command index, zero-extended: its callers read a word. */
DatCommandRecordIndex evtGetIndexedEventRecordId(s32 tableIndex) {
    return datItemSkillRecords[tableIndex].commandIndex;
}

/* Read the entry's level, capped at the script-visible maximum. */
u16 datGetLevelForDraw(const datUnitWork_t *entry) {
    s32 level = entry->level;

    return level < PTY_LEVEL_LIMIT ? level : PTY_MAX_LEVEL;
}

/* Find the first occupied party record for this unit ID, or return NULL. */
datUnitWork_t *dds3FindEntry(DatUnitId unitId) {
    datUnitWork_t *entry = DDS3_GBWK->party;
    s32 slotIndex = 0;

    do {
        if (entry->unitId != unitId) {
            slotIndex++;
        } else {
            if (entry->flags & DAT_PARTY_FLAG_OCCUPIED) {
                return entry;
            }
            slotIndex++;
        }
        entry++;
    } while (slotIndex < PTY_ACTIVE_ROSTER_COUNT);
    return NULL;
}


/* Promote the selected roster entry into the current frontline boundary. */
PtyFrontlinePromotionResult ptyRebalanceFrontline(DatUnitId unitId) {
    DatGameState *scanState;
    datUnitWork_t savedEntry;
    const PtyActiveRosterIndex selectedIndex = dds3FindEntryIndex(unitId);
    s32 frontlineCount;
    s32 scanIndex;

    if (PTY_ACTIVE_ROSTER_INDEX_IS_NOT_FOUND(selectedIndex)) {
        return PTY_FRONTLINE_PROMOTION_NOT_PERFORMED;
    }

    {
        datUnitWork_t *const selectedEntry = &DDS3_GBWK->party[selectedIndex];
        if ((selectedEntry->flags & DAT_PARTY_FLAG_FRONTLINE) != 0) {
            return PTY_FRONTLINE_PROMOTION_NOT_PERFORMED;
        }
    }

    scanState = DDS3_GBWK;
    scanIndex = 0;
    frontlineCount = 0;
    for (; scanIndex < PTY_ACTIVE_ROSTER_COUNT; scanIndex++) {
        datUnitWork_t *const currentEntry = &scanState->party[scanIndex];
        u16 flags = currentEntry->flags;

        if ((u16)(flags & DAT_PARTY_FLAG_OCCUPIED) != 0) {
            if ((flags & DAT_PARTY_FLAG_FRONTLINE) == 0) {
                break;
            }
            frontlineCount++;
        }
    }

    if (frontlineCount >= 3) {
        frontlineCount--;
        scanState->party[frontlineCount].flags &= (u16)~DAT_PARTY_FLAG_FRONTLINE;
    }

    DDS3_GBWK->party[selectedIndex].flags |= DAT_PARTY_FLAG_FRONTLINE;
    memcpy(&savedEntry, &DDS3_GBWK->party[selectedIndex], sizeof(savedEntry));
    memcpy(&DDS3_GBWK->party[selectedIndex],
           &DDS3_GBWK->party[frontlineCount], sizeof(savedEntry));
    memcpy(&DDS3_GBWK->party[frontlineCount], &savedEntry,
           sizeof(savedEntry));
    return PTY_FRONTLINE_PROMOTION_PERFORMED;
}

PtyPartyUnitPresenceResult ptyIsPartyUnitPresent(DatUnitId unitId) {
    return dds3FindEntry(unitId) != 0;
}

/* Return the highest occupied-slot level, or zero when no slot is occupied. */
s32 dds3EntryMax(void) {
    datUnitWork_t *entry = DDS3_GBWK->party;
    s32 maximumLevel = 0;
    s32 remaining = 4;

    do {
        if (entry->flags & DAT_PARTY_FLAG_OCCUPIED) {
            if (maximumLevel < entry->level) {
                maximumLevel = entry->level;
            }
        }
        entry++;
        remaining--;
    } while (remaining >= 0);
    return maximumLevel;
}

/* Ceiling average of occupied-slot levels; native code assumes a nonempty roster. */
s32 ptyGetAverageLevel(void) {
    datUnitWork_t *entry = DDS3_GBWK->party;
    s32 levelSum = 0;
    s32 activeCount = 0;
    s32 remaining = 4;

    do {
        if (entry->flags & DAT_PARTY_FLAG_OCCUPIED) {
            activeCount++;
            levelSum += entry->level;
        }
        entry++;
        remaining--;
    } while (remaining >= 0);
    return (levelSum + activeCount - 1) / activeCount;
}

extern PtyExperienceThreshold ptyComputeTotalExp(const datUnitWork_t *, s32);
extern void ptyRecomputeMaxHpMp(datUnitWork_t *);
extern void ptyRebuildProfileSkills(s32, datUnitWork_t *);
extern void evtCopyRosterTableValue(datUnitWork_t *);
extern s32 D_0032ACA8[128];

/* Restore or initialise the first free roster slot and publish its model flags. */
PtyAddUnitResult ptyAddUnit(DatUnitId unitId) {
    s32 gains[DAT_BASE_STAT_COUNT];
    datUnitWork_t *entry;
    s32 slotIndex;
    s32 remaining;
    s8 *stat;
    s32 *gain;
    s32 *modelFlags;

    if (PTY_ACTIVE_ROSTER_INDEX_IS_FOUND(dds3FindEntryIndex(unitId))) {
        return PTY_ADD_UNIT_RESULT_ALREADY_PRESENT;
    }
    for (slotIndex = 0; slotIndex < 5; slotIndex++) {
        entry = &DDS3_GBWK->party[slotIndex];
        if ((entry->flags & DAT_PARTY_FLAG_OCCUPIED) == 0) {
            break;
        }
    }
    if (slotIndex == 5) {
        return PTY_ADD_UNIT_RESULT_PARTY_FULL;
    }

    memset(entry, 0, sizeof(*entry));
    if ((DDS3_GBWK->templates[unitId].flags & DAT_PARTY_FLAG_OCCUPIED) != 0 &&
        DDS3_GBWK->templates[unitId].level != 0) {
        s32 averageLevel = ptyGetAverageLevel();
        memcpy(entry, &DDS3_GBWK->templates[unitId], sizeof(*entry));
        memset(&DDS3_GBWK->templates[unitId], 0, sizeof(*entry));
        if (entry->level < averageLevel) {
            ptyAccumulateStatGains(gains, averageLevel - entry->level, entry);
            remaining = DAT_BASE_STAT_COUNT - 1;
            stat = entry->baseStats;
            gain = gains;
            do {
                *stat = (u8)*stat + (u8)*gain;
                stat++;
                gain++;
                remaining--;
            } while (remaining >= 0);
            entry->level = averageLevel;
            entry->totalExp = ptyComputeTotalExp(entry, 0);
            ptyRecomputeMaxHpMp(entry);
        }
        ptyRebuildProfileSkills(1, entry);
        entry->hp = entry->maxHp;
        entry->mp = entry->maxMp;
        entry->status = 0;
    } else {
        s32 maximumLevel = dds3EntryMax();
        memcpy(entry, &D_003BAA04[unitId], sizeof(*entry));
        if (entry->level < maximumLevel) {
            ptyAccumulateStatGains(gains, maximumLevel - entry->level, entry);
            remaining = DAT_BASE_STAT_COUNT - 1;
            stat = entry->baseStats;
            gain = gains;
            do {
                *stat = (u8)*stat + (u8)*gain;
                stat++;
                gain++;
                remaining--;
            } while (remaining >= 0);
            entry->level = maximumLevel;
        }
        entry->totalExp = ptyComputeTotalExp(entry, 0);
        ptyRebuildProfileSkills(0, entry);
        evtCopyRosterTableValue(entry);
    }

    entry->flags &= ~DAT_PARTY_FLAG_FRONTLINE;
    modelFlags = &D_0032ACA8[unitId * 4];
    for (remaining = 0; remaining < 4U; remaining++) {
        s32 modelFlag = *modelFlags++;
        if (modelFlag != 0) {
            evtBitOn(modelFlag);
        }
    }
    return slotIndex;
}


extern s32 D_0032ACA8[128];
extern void evtBitOff(s32);
PtyRemoveUnitResult ptyRemoveUnit(DatUnitId unitId) {
    PtyActiveRosterIndex partyIndex = dds3FindEntryIndex(unitId);
    s32 i;
    s32 completeSlotCount;
    s32 *modelFlag;
    datUnitWork_t *entry;

    if (PTY_ACTIVE_ROSTER_INDEX_IS_NOT_FOUND(partyIndex)) {
        return PTY_REMOVE_UNIT_RESULT_NOT_FOUND;
    }

    memcpy(&DDS3_GBWK->templates[unitId], &DDS3_GBWK->party[partyIndex], sizeof(datUnitWork_t));
    for (i = partyIndex; i < 4; i++) {
        memcpy(&DDS3_GBWK->party[i], &DDS3_GBWK->party[i + 1], sizeof(datUnitWork_t));
    }
    memset(&DDS3_GBWK->party[i], 0, sizeof(datUnitWork_t));

    modelFlag = &D_0032ACA8[unitId * 4];
    for (i = 0; i < 4U; i++) {
        if (*modelFlag != 0) {
            evtBitOff(*modelFlag);
        }
        modelFlag++;
    }

    completeSlotCount = 0;
    entry = DDS3_GBWK->party;
    for (i = 0; i < PTY_ACTIVE_ROSTER_COUNT; i++) {
        if ((entry->flags & 3) == 3) {
            completeSlotCount++;
        }
        entry++;
    }
    if (completeSlotCount != 0) {
        return PTY_REMOVE_UNIT_RESULT_REMOVED;
    }
    ptyRebalanceFrontline(1);
    return PTY_REMOVE_UNIT_RESULT_REMOVED;
}

u32 ptyReturnZero(void) {
    return 0;
}

u32 ptyReturnOneForCommand193(void) {
    return 1;
}

u32 ptyReturnOne(void) {
    return 1;
}

extern DatSkillList *D_003BAA58;

/* Test whether a party record satisfies one participant requirement of an affinity. */
PtyAffinityMatchResult ptyUnitMatchesAffinityRequirement(
    const datUnitWork_t *unit, BtlCommandId commandId, u8 requirementIndex) {
    const DatAffinityRecord *record;
    s32 requirement;
    s32 requirementValue;
    u32 i;

    if (unit == NULL) {
        return PTY_AFFINITY_MATCH_NOT_FOUND;
    }
    record = &datAffinityRecords[commandId - DAT_AFFINITY_FIRST_COMMAND];
    if (record->flags & 2) {
        return PTY_AFFINITY_MATCH_NOT_FOUND;
    }
    requirement = record->requirements[requirementIndex];
    if (requirement == -1) {
        return PTY_AFFINITY_MATCH_NOT_FOUND;
    }
    switch (requirement & 0xF0000000) {
    case 0:
        if (requirement == 0 ||
            datUnitHasSkill(unit, (DatSkillId)requirement) != DAT_UNIT_SKILL_RESULT_ABSENT) {
            return PTY_AFFINITY_MATCH_FOUND;
        }
        break;
    case 0x10000000:
        requirementValue = requirement & 0x0FFFFFFF;
        for (i = 0; i < 24; i++) {
            if (unit->effectData[i] != 0 && (1 << datCommandSelectors[unit->effectData[i]].stat) == requirementValue) {
                return PTY_AFFINITY_MATCH_FOUND;
            }
        }
        break;
    case 0x20000000:
        requirementValue = requirement & 0x0FFFFFFF;
        if (requirementValue == 0 || requirementValue == (1 << unit->unitId)) {
            return PTY_AFFINITY_MATCH_FOUND;
        }
        break;
    case 0x40000000:
        for (i = 0; i < 16; i++) {
            s16 skill = D_003BAA58[requirement].skills[i];

            if (skill >= 0 && datUnitHasSkill(unit, (DatSkillId)skill)) {
                return PTY_AFFINITY_MATCH_FOUND;
            }
        }
        break;
    }
    return PTY_AFFINITY_MATCH_NOT_FOUND;
}

/* Try the six stored orders; success requires exactly the non-sentinel requirement count. */
PtyAffinityMatchResult ptyMatchAffinityPermutation(
    const datUnitWork_t *const *actors, BtlCommandId commandId) {
    const s32 *requirementCursor =
        datAffinityRecords[commandId - DAT_AFFINITY_FIRST_COMMAND].requirements;
    u32 i;
    s32 requiredCount = 0;
    u32 orderIndex;
    s32 orderOffset;
    s32 matchedCount;
    u8 *orderCursor;
    const datUnitWork_t *const *actorCursor;
    const datUnitWork_t *actor;

    for (i = 0; i < 3; i++) {
        if (*requirementCursor++ != -1) {
            requiredCount++;
        }
    }
    for (orderIndex = 0, orderOffset = 0; orderIndex < 6; orderIndex++, orderOffset += 3) {
        matchedCount = 0;
        for (i = 0, orderCursor = ptyAffinityPermutationOrders + orderOffset, actorCursor = actors; i < 3; i++, orderCursor++) {
            actor = *actorCursor++;
            if (actor != 0 &&
                ptyUnitMatchesAffinityRequirement(actor, commandId, *orderCursor) != PTY_AFFINITY_MATCH_NOT_FOUND) {
                matchedCount++;
            }
        }
        if (requiredCount == matchedCount) {
            return PTY_AFFINITY_MATCH_FOUND;
        }
    }
    return PTY_AFFINITY_MATCH_NOT_FOUND;
}

void evtCopyRosterTableValue(datUnitWork_t *entry) {
    entry->menuValue = ptyRosterMenuValues[entry->unitId].value;
}

/* Copy table values only for occupied slots whose roster identifier is below 16. */
void evtUpdateFlaggedEntries(void) {
    s32 slotIndex = 0;

    do {
        datUnitWork_t *entry = &DDS3_GBWK->party[slotIndex];
        if (entry->flags & DAT_PARTY_FLAG_OCCUPIED) {
            s32 rosterIndex = 0;
            do {
                if (entry->unitId == rosterIndex) {
                    evtCopyRosterTableValue(entry);
                }
                rosterIndex++;
            } while (rosterIndex < 16);
        }
        slotIndex++;
    } while (slotIndex < PTY_ACTIVE_ROSTER_COUNT);
}

/* Apply the two configured count/flag updates; a zero index disables its record. */
void dds3ForEachEntry(void) {
    PtyItemQuantityUpdateView *updates = ptyItemQuantityUpdates;
    u32 updateIndex = 0;

    do {
        u16 itemId = updates->itemId;
        u8 quantityDelta = updates->quantityDelta;

        updates++;
        if (itemId != 0) {
            ptyAdjustItemQuantity(itemId, quantityDelta);
        }
        updateIndex++;
    } while (updateIndex < 2);
}

extern u16 ptyRosterDefaultSkillIds[16][4];
extern void scrClearAllSecondaryScriptFlags(datUnitWork_t *unit);
extern s32 scrSetFlag(datUnitWork_t *unit, u16 skill);
extern void scrSetSecondaryScriptFlag(datUnitWork_t *unit, u16 skill);

void ptyMergeStockSkills(datUnitWork_t *unit) {
    u16 unitId = unit->unitId;
    datUnitWork_t *stock = &DDS3_GBWK->templates[unitId];
    u16 occupied = stock->flags & DAT_PARTY_FLAG_OCCUPIED;
    u32 i;
    u16 skill;

    if (occupied != 0) {
        for (i = 0; i < 77; i++) {
            unit->skillSnapshotWords[i] |= stock->skillSnapshotWords[i];
        }
        unit->profileId = stock->profileId;
    }
    scrClearAllSecondaryScriptFlags(unit);
    for (i = 0; i < 4; i++) {
        u32 value = ptyRosterDefaultSkillIds[unitId][i];
        skill = value;
        if (value != 0) {
            scrSetFlag(unit, skill);
            scrSetSecondaryScriptFlag(unit, skill);
        }
    }
}

/* Visit occupied roster slots for native per-entry processing. */
void dds3ForEachFlagged(void) {
    s32 slotIndex = 0;

    do {
        datUnitWork_t *entry = &DDS3_GBWK->party[slotIndex];
        if (entry->flags & DAT_PARTY_FLAG_OCCUPIED) {
            ptyMergeStockSkills(entry);
        }
        slotIndex++;
    } while (slotIndex < PTY_ACTIVE_ROSTER_COUNT);
}

/* Copy every occupied active-party record to its unit template slot. */
void ptySaveActiveUnitsToStock(void) {
    s32 slotIndex = 0;

    do {
        datUnitWork_t *entry = &DDS3_GBWK->party[slotIndex];
        u16 occupied = entry->flags & DAT_PARTY_FLAG_OCCUPIED;

        if (occupied != 0) {
            memcpy(&DDS3_GBWK->templates[entry->unitId], entry, sizeof(datUnitWork_t));
        }
        slotIndex++;
    } while (slotIndex < PTY_ACTIVE_ROSTER_COUNT);
}

void evtRandomizeEntryValue(datUnitWork_t *entry) {
    s32 randomOffset;

    randomOffset = dds3GetRandIntA(0, 4);
    entry->randomizedValue = 0x12 - randomOffset;
}



/* Clear a subset of per-unit status flags on occupied qualifying entries,
 * gated by the event RNG; report whether any flags were cleared. */
EvtRandomStatusClearResult evtClearRandomStatusFlags(void) {
    EvtRandomStatusClearResult clearedAny = EVT_RANDOM_STATUS_CLEAR_NO_FLAGS;
    s32 remaining;
    datUnitWork_t *entry;
    s32 roll = (s32)dds3GetRandIntA(0, 100);

    if (roll >= 51) {
        return EVT_RANDOM_STATUS_CLEAR_NO_FLAGS;
    }
    remaining = 4;
    entry = DDS3_GBWK->party;
    do {
        if ((entry->flags & DAT_PARTY_FLAG_OCCUPIED) != 0 && entry->hp != 0) {
            u16 statusFlags = entry->status;
            if ((statusFlags & 0x5D0) != 0) {
                entry->status = statusFlags & ~0x5D0;
                clearedAny = EVT_RANDOM_STATUS_CLEAR_FLAGS_CLEARED;
            }
        }
        remaining--;
        entry++;
    } while (remaining >= 0);
    return clearedAny;
}

/* Step the script with these context values; clear the written flag, not the stored result. */
ScrVmInteger evtRunContext(ScrProcedureIndex procedureIndex, s32 firstArgument, s32 secondArgument, s32 commandIndex, u16 options) {
    scrStartProcessProcedure(evtWorkScriptTask, procedureIndex);
    evtWorkScriptContext.commandIndex = commandIndex;
    evtWorkScriptContext.firstArgument.bits = firstArgument;
    evtWorkScriptContext.secondArgument.bits = secondArgument;
    evtWorkScriptContext.options = options;
    evtWorkScriptContext.stateFlags &= EVT_SCRIPT_CONTEXT_RESULT_RESET_MASK;
    scrRunScript(evtWorkScriptTask);
    return evtWorkScriptContext.returnValue;
}

/* Create the script task and clear its separate 24-byte context. */
void dds3WorkInit(const BfFlw0Header *header) {
    evtWorkScriptTask = scrCreateTaskWithDefaultOption(header);
    memset(&evtWorkScriptContext, 0, EVT_CONTEXT_BYTES);
}

scrProcessWork_s *scrGetWorkTaskHandle(void) {
    return evtWorkScriptTask;
}

void scrDestroyWorkTask(void) {
    scrProcDestroyTask(evtWorkScriptTask);
    evtWorkScriptTask = 0;
}

s32 evtPushFirstRosterLevel(void) {
    scrSetIntReturnValue(((datUnitWork_t *)evtWorkScriptContext.firstArgument.pointer)->level);
    return 1;
}

s32 evtPushSecondRosterLevel(void) {
    scrSetIntReturnValue(((datUnitWork_t *)evtWorkScriptContext.secondArgument.pointer)->level);
    return 1;
}

s32 evtPushFirstRosterCurrentHp(void) {
    scrSetIntReturnValue(((datUnitWork_t *)evtWorkScriptContext.firstArgument.pointer)->hp);
    return 1;
}

s32 evtPushSecondRosterCurrentHp(void) {
    scrSetIntReturnValue(((datUnitWork_t *)evtWorkScriptContext.secondArgument.pointer)->hp);
    return 1;
}

s32 evtPushFirstRosterMaximumHp(void) {
    scrSetIntReturnValue(((datUnitWork_t *)evtWorkScriptContext.firstArgument.pointer)->maxHp);
    return 1;
}

s32 evtPushSecondRosterMaximumHp(void) {
    scrSetIntReturnValue(((datUnitWork_t *)evtWorkScriptContext.secondArgument.pointer)->maxHp);
    return 1;
}

/* Push the first entry's selected stat; selectors -1, 16 and 17 use the default. */
s32 evtPushFirstRosterSelectedStat(void) {
    DatAffinitySelector selector = datCommandSelectors[evtWorkScriptContext.commandIndex].stat;
    s32 statValue;

    switch (selector) {
    case DAT_AFFINITY_SELECTOR_UNMAPPED:
    case 16:
    case 17:
        statValue = EVT_DEFAULT_STAT_VALUE;
        break;
    default:
        if (btlIsRuntimeAllocated()) {
            statValue = (u16)btlResolveUnitValueWithOverride(
                (datUnitWork_t *)evtWorkScriptContext.firstArgument.pointer, selector);
        } else {
            statValue = datGetAisyoValue(
                (datUnitWork_t *)evtWorkScriptContext.firstArgument.pointer, selector);
        }
        break;
    }
    scrSetIntReturnValue(statValue);
    return 1;
}

/* Push the second entry's selected stat; selectors -1, 16 and 17 use the default. */
s32 evtPushSecondRosterSelectedStat(void) {
    DatAffinitySelector selector = datCommandSelectors[evtWorkScriptContext.commandIndex].stat;
    s32 statValue;

    switch (selector) {
    case DAT_AFFINITY_SELECTOR_UNMAPPED:
    case 16:
    case 17:
        statValue = EVT_DEFAULT_STAT_VALUE;
        break;
    default:
        if (btlIsRuntimeAllocated()) {
            statValue = (u16)btlResolveUnitValueWithOverride(
                (datUnitWork_t *)evtWorkScriptContext.secondArgument.pointer, selector);
        } else {
            statValue = datGetAisyoValue(
                (datUnitWork_t *)evtWorkScriptContext.secondArgument.pointer, selector);
        }
        break;
    }
    scrSetIntReturnValue(statValue);
    return 1;
}

/* Push the first entry's option-selected stat, retaining the same default selectors. */
s32 evtPushFirstRosterOptionStat(void) {
    s8 statIndex = datFlagToElementIndex(evtWorkScriptContext.options);
    s32 statValue = EVT_DEFAULT_STAT_VALUE;

    switch (statIndex) {
    case DAT_AFFINITY_SELECTOR_UNMAPPED:
    case 16:
    case 17:
        break;
    default:
        if (btlIsRuntimeAllocated()) {
            statValue = (u16)btlResolveUnitValueWithOverride(
                (datUnitWork_t *)evtWorkScriptContext.firstArgument.pointer, statIndex);
        } else {
            statValue = datGetAisyoValue(
                (datUnitWork_t *)evtWorkScriptContext.firstArgument.pointer, statIndex);
        }
        break;
    }
    scrSetIntReturnValue(statValue);
    return 1;
}

/* Push the second entry's option-selected stat, retaining the same default selectors. */
s32 evtPushSecondRosterOptionStat(void) {
    s8 statIndex = datFlagToElementIndex(evtWorkScriptContext.options);
    s32 statValue = EVT_DEFAULT_STAT_VALUE;

    switch (statIndex) {
    case DAT_AFFINITY_SELECTOR_UNMAPPED:
    case 16:
    case 17:
        break;
    default:
        if (btlIsRuntimeAllocated()) {
            statValue = (u16)btlResolveUnitValueWithOverride(
                (datUnitWork_t *)evtWorkScriptContext.secondArgument.pointer, statIndex);
        } else {
            statValue = datGetAisyoValue(
                (datUnitWork_t *)evtWorkScriptContext.secondArgument.pointer, statIndex);
        }
        break;
    }
    scrSetIntReturnValue(statValue);
    return 1;
}

/* Push the first entry's profile-adjusted stat, including its native status override. */
s32 evtPushFirstRosterStatEligibility(void) {
    s32 statIndex = scrGetIntPara(0);

    scrSetIntReturnValue(datGetStatWithStatusOverride((datUnitWork_t *)evtWorkScriptContext.firstArgument.pointer, statIndex));
    return 1;
}

/* Push the second entry's profile-adjusted stat, including its native status override. */
s32 evtPushSecondRosterStatEligibility(void) {
    s32 statIndex = scrGetIntPara(0);

    scrSetIntReturnValue(datGetStatWithStatusOverride((datUnitWork_t *)evtWorkScriptContext.secondArgument.pointer, statIndex));
    return 1;
}

/* Kind 5 selects the first entry's roster detail instead of the command-table stat. */
s32 evtPushSelectedStatOrRosterLowValue(void) {
    EvtScriptContext *scriptContext = &evtWorkScriptContext;
    s32 commandIndex = scriptContext->commandIndex;
    s32 statValue;
    if (datCommandSelectors[commandIndex].kind == EVT_ROSTER_DETAIL_KIND) {
        statValue = datRosterDetails[((datUnitWork_t *)scriptContext->firstArgument.pointer)->unitId].alternateA;
    } else {
        statValue = datCommandRecords[commandIndex].stat11;
    }
    scrSetIntReturnValue(statValue);
    return 1;
}

/* Kind 5 scales the command stat by the first entry's roster multiplier, then truncates. */
s32 evtPushSelectedScaledStat(void) {
    EvtScriptContext *scriptContext = &evtWorkScriptContext;
    s32 commandIndex = scriptContext->commandIndex;
    s32 statValue = datCommandRecords[commandIndex].attribute.parts.hitChance;
    if (datCommandSelectors[commandIndex].kind == EVT_ROSTER_DETAIL_KIND) {
        f32 rosterScale = datRosterDetails[((datUnitWork_t *)scriptContext->firstArgument.pointer)->unitId].multiplier;
        statValue = (s32)((f32)statValue * rosterScale);
    }
    scrSetIntReturnValue(statValue);
    return 1;
}

s32 evtPushSelectedStatGrade(void) {
    scrSetIntReturnValue(datCommandRecords[evtWorkScriptContext.commandIndex].stat2D);
    return 1;
}

/* Options 1 and 2 select alternate command halfwords; other options push zero. */
s32 evtSelectScriptStatValue(void) {
    s32 statValue;

    switch (evtWorkScriptContext.options) {
    case 1:
        statValue = datCommandRecords[evtWorkScriptContext.commandIndex].hpPower;
        break;
    case 2:
        statValue = datCommandRecords[evtWorkScriptContext.commandIndex].mpPower;
        break;
    default:
        statValue = 0;
        break;
    }
    scrSetIntReturnValue(statValue);
    return 1;
}

/* Select the alternate stat through the first entry's table value, not the context selector. */
s32 evtPushEntryIndexedStatOption(void) {
    EvtScriptContext *scriptContext = &evtWorkScriptContext;
    s32 statValue;
    u16 statOption = scriptContext->options;
    u16 commandIndex = datItemSkillRecords[((datUnitWork_t *)scriptContext->firstArgument.pointer)->menuValue].commandIndex;

    switch (statOption) {
    case 1:
        statValue = datCommandRecords[commandIndex].hpPower;
        break;
    case 2:
        statValue = datCommandRecords[commandIndex].mpPower;
        break;
    default:
        statValue = 0;
        break;
    }
    scrSetIntReturnValue(statValue);
    return 1;
}

/* Kind 5 selects the first entry's high roster detail instead of the command total. */
s32 evtPushSelectedTotalOrRosterHighValue(void) {
    EvtScriptContext *scriptContext = &evtWorkScriptContext;
    s32 commandIndex = scriptContext->commandIndex;
    s32 statValue;
    if (datCommandSelectors[commandIndex].kind == EVT_ROSTER_DETAIL_KIND) {
        statValue = datRosterDetails[((datUnitWork_t *)scriptContext->firstArgument.pointer)->unitId].alternateB;
    } else {
        statValue = datCommandRecords[commandIndex].criticalPercent;
    }
    scrSetIntReturnValue(statValue);
    return 1;
}

s32 evtPushSelectedStatMaximum(void) {
    scrSetIntReturnValue(datCommandRecords[evtWorkScriptContext.commandIndex].stat36);
    return 1;
}

/* Store the script argument as the context result and mark it written. */
s32 evtStoreScriptParameterResult(void) {
    EvtScriptContextFlags stateFlags = evtWorkScriptContext.stateFlags |
        EVT_SCRIPT_CONTEXT_RESULT_WRITTEN_FLAG;

    evtWorkScriptContext.stateFlags = stateFlags;
    evtWorkScriptContext.returnValue = scrGetIntPara(0);
    return 1;
}

s32 evtPushScriptContextResult(void) {
    scrSetIntReturnValue(evtWorkScriptContext.returnValue);
    return 1;
}

s32 evtPushEntryFlagBitInverted(void) {
    scrSetIntReturnValue((((((datUnitWork_t *)evtWorkScriptContext.firstArgument.pointer)->flags) >> 5) ^ 1) & 1);
    return 1;
}

/* For positive ranges, return 1.0 plus a percentage jitter; the upper endpoint is excluded. */
s32 evtRollRandomScale(void) {
    s32 percentRange = scrGetIntPara(0);
    s32 roll = dds3GetRandIntA(0, percentRange * 2);

    scrSetFloatReturnValue((f32)(roll - percentRange + 100) / 100.0f);
    return 1;
}

/* Without a battle runtime, push zero without reading the group-choice argument. */
s32 scrGetBattleAverageCurrentValueForGroup(void) {
    s32 battleAvailable = btlIsRuntimeAllocated();
    s32 averageValue;
    if (battleAvailable) {
        s32 groupChoice = scrGetIntPara(0);
        BtlPartyRecordFlagsMask partyRecordFlagsMask =
            groupChoice ? EVT_ALTERNATE_PARTY_RECORD_FLAGS_MASK : EVT_DEFAULT_PARTY_RECORD_FLAGS_MASK;
        averageValue = btlAverageAllCurrentForMask(partyRecordFlagsMask);
    }
    else {
        averageValue = 0;
    }
    scrSetIntReturnValue(averageValue);
    return 1;
}

/* Without a battle runtime, push zero without reading the group-choice argument. */
s32 scrGetBattleAverageMaximumValueForGroup(void) {
    s32 battleAvailable = btlIsRuntimeAllocated();
    s32 averageValue;
    if (battleAvailable) {
        s32 groupChoice = scrGetIntPara(0);
        BtlPartyRecordFlagsMask partyRecordFlagsMask =
            groupChoice ? EVT_ALTERNATE_PARTY_RECORD_FLAGS_MASK : EVT_DEFAULT_PARTY_RECORD_FLAGS_MASK;
        averageValue = btlAverageAllMaximumForMask(partyRecordFlagsMask);
    }
    else {
        averageValue = 0;
    }
    scrSetIntReturnValue(averageValue);
    return 1;
}

/* Without a battle runtime, push zero without reading the group-choice argument. */
s32 scrGetBattleAverageActorStatForGroup(void) {
    s32 battleAvailable = btlIsRuntimeAllocated();
    s32 averageValue;
    if (battleAvailable) {
        s32 groupChoice = scrGetIntPara(0);
        BtlPartyRecordFlagsMask partyRecordFlagsMask =
            groupChoice ? EVT_ALTERNATE_PARTY_RECORD_FLAGS_MASK : EVT_DEFAULT_PARTY_RECORD_FLAGS_MASK;
        averageValue = btlAverageAllLevelsForMask(partyRecordFlagsMask);
    }
    else {
        averageValue = 0;
    }
    scrSetIntReturnValue(averageValue);
    return 1;
}

/* Push the selected actor-group average for the requested attribute. */
s32 evtPushAvailableChoiceRatio(void) {
    s32 battleAvailable = btlIsRuntimeAllocated();
    s32 averageValue;
    if (battleAvailable) {
        s32 groupChoice = scrGetIntPara(0);
        DatBaseStatIndex baseStatIndex = scrGetIntPara(1);
        BtlPartyRecordFlagsMask partyRecordFlagsMask =
            groupChoice ? EVT_ALTERNATE_PARTY_RECORD_FLAGS_MASK : EVT_DEFAULT_PARTY_RECORD_FLAGS_MASK;
        averageValue = btlAverageActorAttribute(partyRecordFlagsMask, baseStatIndex);
    }
    else {
        averageValue = 0;
    }
    scrSetIntReturnValue(averageValue);
    return 1;
}

/* Push the native battle float when a runtime exists, otherwise zero. */
s32 evtPushAvailableFloatValue(void) {
    s32 battleAvailable = btlIsRuntimeAllocated();
    f32 value = 0.0f;

    if (battleAvailable != 0) {
        value = btlGetEncounterAdjustmentScale();
    }
    scrSetFloatReturnValue(value);
    return 1;
}

/* Push the native battle integer when a runtime exists, otherwise zero. */
s32 evtPushAvailableIntegerValue(void) {
    s32 battleAvailable = btlIsRuntimeAllocated();
    s32 value = 0;

    if (battleAvailable != 0) {
        value = btlGetEscapeAttemptCount();
    }
    scrSetIntReturnValue(value);
    return 1;
}

s32 evtPushLevelMaxHpGrowth(void) {
    return (s32)scrSetFloatReturnValue(datBattleParameters->maxHpGrowth[((datUnitWork_t *)evtWorkScriptContext.firstArgument.pointer)->level - 1]);
}

s32 evtPushLevelMaxMpGrowth(void) {
    return (s32)scrSetFloatReturnValue(datBattleParameters->maxMpGrowth[((datUnitWork_t *)evtWorkScriptContext.firstArgument.pointer)->level - 1]);
}

extern u32 datComputeSkillBoostedMaxHp(datUnitWork_t *);

/* Push the coarse HP-percentage table value; only exactly 100 percent uses index zero. */
s32 evtSelectStatGrade(void) {
    s32 maximumHp = datComputeSkillBoostedMaxHp((datUnitWork_t *)evtWorkScriptContext.secondArgument.pointer);
    s32 currentHp = ((datUnitWork_t *)evtWorkScriptContext.secondArgument.pointer)->hp;
    s32 hpPercent = (s32)((f32)currentHp / (f32)maximumHp * 100.0f);
    s32 gradeIndex = 0;

    if (hpPercent != 100) {
        gradeIndex = 1;
        if (hpPercent < 80) {
            gradeIndex = 2;
            if (hpPercent < 60) {
                gradeIndex = 3;
                if (hpPercent < 40) {
                    gradeIndex = 4;
                    if (hpPercent < 30) {
                        gradeIndex = 5;
                        if (hpPercent < 20) {
                            gradeIndex = hpPercent >= 10 ? 6 : 7;
                        }
                    }
                }
            }
        }
    }
    return (s32)scrSetFloatReturnValue(datBattleParameters->hpGradeValues[gradeIndex]);
}

s32 evtPushLevelValueA(void) {
    return (s32)scrSetFloatReturnValue(datBattleParameters->levelValuesA[((datUnitWork_t *)evtWorkScriptContext.firstArgument.pointer)->level - 1]);
}

s32 evtPushLevelValueB(void) {
    return (s32)scrSetFloatReturnValue(datBattleParameters->levelValuesB[((datUnitWork_t *)evtWorkScriptContext.firstArgument.pointer)->level - 1]);
}

s32 evtPushLevelValueCCommand18D(void) {
    return (s32)scrSetFloatReturnValue(datBattleParameters->levelValuesC[((datUnitWork_t *)evtWorkScriptContext.firstArgument.pointer)->level - 1]);
}

s32 evtPushLevelValueCCommand18E(void) {
    return (s32)scrSetFloatReturnValue(datBattleParameters->levelValuesC[((datUnitWork_t *)evtWorkScriptContext.firstArgument.pointer)->level - 1]);
}

s32 evtScriptSelectRandomValue(void) {
    s32 resultCode;
    s32 roll;

    if ((((datUnitWork_t *)evtWorkScriptContext.secondArgument.pointer)->flags & 0x20) == 0) {
        roll = dds3GetRandIntA(0, 0x20);
        resultCode = roll != 0 ? 10 : 0x80;
    } else {
        roll = dds3GetRandIntA(0, 0x30);
        resultCode = roll != 0 ? 0 : 0x80;
    }
    return (s32)scrSetIntReturnValue(resultCode);
}

s32 evtPushRosterBaseValue(void) {
    return (s32)scrSetIntReturnValue(datRosterDetails[((datUnitWork_t *)evtWorkScriptContext.firstArgument.pointer)->unitId].base);
}

/* Push the finer HP-percentage table value; only exactly 100 percent uses index zero. */
s32 evtSelectFineStatGrade(void) {
    s32 maximumHp = datComputeSkillBoostedMaxHp((datUnitWork_t *)evtWorkScriptContext.secondArgument.pointer);
    s32 currentHp = ((datUnitWork_t *)evtWorkScriptContext.secondArgument.pointer)->hp;
    s32 hpPercent = (s32)((f32)currentHp / (f32)maximumHp * 100.0f);
    s32 gradeIndex = 0;

    if (hpPercent != 100) {
        gradeIndex = 1;
        if (hpPercent < 90) {
            gradeIndex = 2;
            if (hpPercent < 80) {
                gradeIndex = 3;
                if (hpPercent < 70) {
                    gradeIndex = 4;
                    if (hpPercent < 60) {
                        gradeIndex = 5;
                        if (hpPercent < 50) {
                            gradeIndex = 6;
                            if (hpPercent < 40) {
                                gradeIndex = 7;
                                if (hpPercent < 30) {
                                    gradeIndex = 8;
                                    if (hpPercent < 20) {
                                        gradeIndex = hpPercent >= 10 ? 9 : 10;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return (s32)scrSetFloatReturnValue(datBattleParameters->hpFineGradeValues[gradeIndex]);
}


u8 evtTestSolarPhaseOrModelFlag(DatBattleAdjustmentCondition condition) {
    u32 conditionType = condition >> 16;
    switch (conditionType) {
    case DAT_BATTLE_ADJUSTMENT_CONDITION_NONE:
        break;
    case DAT_BATTLE_ADJUSTMENT_CONDITION_SOLAR_PHASE_MASK:
        if (condition & (1 << evtGetMirroredSolarPhase()) & 0xFFFF) {
            return 1;
        }
        break;
    case DAT_BATTLE_ADJUSTMENT_CONDITION_MODEL_FLAG:
        if (evtBitCheck(condition & 0xFFFF)) {
            return 1;
        }
        break;
    }
    return 0;
}


u8 btlSelectConditionalEncounterGroup(s32 encounterIndex) {
    s8 enabled[3];
    s32 conditionIndex;
    u8 code;

    for (conditionIndex = 0; conditionIndex < 3; conditionIndex++) {
        enabled[conditionIndex] = evtTestSolarPhaseOrModelFlag(datBattleAdjustmentRecords[encounterIndex].conditions[conditionIndex]);
    }
    code = datBattleAdjustmentRecords[encounterIndex].variantCodes[0];
    if (enabled[0] && enabled[1] && enabled[2]) {
        if (code != 8) {
            return code;
        }
    }
    code = datBattleAdjustmentRecords[encounterIndex].variantCodes[1];
    if (enabled[0] && enabled[1]) {
        if (code != 8) {
            return code;
        }
    }
    code = datBattleAdjustmentRecords[encounterIndex].variantCodes[2];
    if (enabled[0] && enabled[2]) {
        if (code != 8) {
            return code;
        }
    }
    code = datBattleAdjustmentRecords[encounterIndex].variantCodes[3];
    if (enabled[1] && enabled[2]) {
        if (code != 8) {
            return code;
        }
    }
    code = datBattleAdjustmentRecords[encounterIndex].variantCodes[4];
    if (enabled[0]) {
        if (code != 8) {
            return code;
        }
    }
    code = datBattleAdjustmentRecords[encounterIndex].variantCodes[5];
    if (enabled[1]) {
        if (code != 8) {
            return code;
        }
    }
    code = datBattleAdjustmentRecords[encounterIndex].variantCodes[6];
    if (enabled[2]) {
        if (code != 8) {
            return code;
        }
    }
    code = datBattleAdjustmentRecords[encounterIndex].variantCodes[7];
    return code == 8 ? 0 : code;
}

enum {
    BTL_LEVEL_GATE_PARTY_FLAG_ELIGIBLE = 4
};

/* Compare truncated averages: enemy average + party average / 4 >= party average.
 * Requires a nonzero scene, a nonempty enemy list, and party entries carrying
 * occupied, level-gate-eligible and frontline flags. No eligible entries returns zero.
 * count and remaining serve both loops; word holds enemy IDs, then entry flags.
 * partyLevel is accumulated first and divided in place before comparison. */
BtlScenePartyLevelThresholdResult btlCheckScenePartyLevelThreshold(u32 sceneIndex) {
    s32 result = 0;
    s32 enemyTotal;
    u16 *levelCursor;
    s32 count;
    s32 remaining;
    u16 *cursor;
    u32 sceneOffset;
    DatEnemyRecord *enemyRecordsBase;
    datUnitWork_t *entry;
    s32 partyTotal;
    s32 average;
    u16 value;

    if (sceneIndex != 0) {
        enemyTotal = 0;
        sceneOffset = sceneIndex * sizeof(DatBattleSceneRecord);
        cursor = ((DatBattleSceneRecord *)(sceneOffset + (u32)datBattleSceneRecords))->unitModes;
        enemyRecordsBase = datEnemyRecords;
        count = 0;
        remaining = 10;
        do {
            value = *cursor;
            cursor++;
            if (value != 0) {
                count++;
                enemyTotal += enemyRecordsBase[value].level;
            }
            remaining--;
        } while (remaining >= 0);
        result = 0;
        if (count != 0) {
            average = enemyTotal / count;
            partyTotal = 0;
            count = 0;
            remaining = 4;
            levelCursor = &DDS3_GBWK->party[0].level;
            entry = DDS3_GBWK->party;
            do {
                value = entry->flags;
                entry++;
                if (value & DAT_PARTY_FLAG_OCCUPIED) {
                    if (value & BTL_LEVEL_GATE_PARTY_FLAG_ELIGIBLE) {
                        if (value & DAT_PARTY_FLAG_FRONTLINE) {
                            count++;
                            partyTotal += *levelCursor;
                        }
                    }
                }
                remaining--;
                levelCursor = (u16 *)((u8 *)levelCursor + sizeof(datUnitWork_t));
            } while (remaining >= 0);
            result = 0;
            if (count != 0) {
                partyTotal = partyTotal / count;
                result = average + partyTotal / 4 < partyTotal;
                result = result == 0;
            }
        }
    }
    return result;
}

/* Reuse an active interval, or seed one for a nonempty encounter group. */
BtlConditionalEncounterGaugeThreshold btlGetConditionalEncounterInterval(s32 encounterIndex) {
    s32 variant;
    s32 total;
    s32 i;
    s32 interval;
    if (encounterIndex == 0) {
        return BTL_CONDITIONAL_ENCOUNTER_GAUGE_THRESHOLD_UNAVAILABLE;
    }
    variant = btlSelectConditionalEncounterGroup(encounterIndex);
    total = 0;
    for (i = 0; i < 20; i++) {
        total += datBattleAdjustmentRecords[encounterIndex].groups[variant].entries[i].weight;
    }
    if (total == 0) {
        return BTL_CONDITIONAL_ENCOUNTER_GAUGE_THRESHOLD_UNAVAILABLE;
    }
    if (btlConditionalEncounterCachedInterval > 0) {
        return btlConditionalEncounterCachedInterval;
    }
    interval = datBattleAdjustmentRecords[encounterIndex].groups[variant].interval;
    if (interval <= 0) {
        return BTL_CONDITIONAL_ENCOUNTER_GAUGE_THRESHOLD_UNAVAILABLE;
    }
    btlConditionalEncounterCachedInterval = dds3GetRandIntA(0, interval * 2 - 32) + 16;
    return btlConditionalEncounterCachedInterval;
}

extern s32 btlSelectedConditionalEncounterIndex;
extern s32 btlSelectedConditionalEncounterResult;
extern void dds3WorkClear(void);

BtlConditionalEncounterRollResult btlAdvanceConditionalEncounterRoll(f32 step, s32 encounterIndex) {
    s32 group;
    BtlConditionalEncounterGaugeThreshold threshold;
    s32 count;
    s32 i;
    s32 total;
    s32 roll;
    u32 scene;
    s32 packed;
    f32 counter;

    if (encounterIndex == 0 || step == 0.0f) {
        return BTL_CONDITIONAL_ENCOUNTER_ROLL_NO_SELECTION;
    }
    group = btlSelectConditionalEncounterGroup(encounterIndex);
    threshold = btlGetConditionalEncounterInterval(encounterIndex);
    if (threshold == BTL_CONDITIONAL_ENCOUNTER_GAUGE_THRESHOLD_UNAVAILABLE) {
        return BTL_CONDITIONAL_ENCOUNTER_ROLL_NO_SELECTION;
    }
    counter = DDS3_GBWK->conditionalEncounterStepRemainder + step;
    DDS3_GBWK->conditionalEncounterStepRemainder = counter;
    if (counter < 100.0f) {
        return BTL_CONDITIONAL_ENCOUNTER_ROLL_NO_SELECTION;
    }
    count = (s32)(counter / 100.0f);
    DDS3_GBWK->conditionalEncounterStepRemainder = counter - (f32)count * 100.0f;
    for (i = 0; i < count; i++) {
        if (DDS3_GBWK->world.fieldFlags & DAT_WORLD_FIELD_ENCOUNTER_RATE_BOOST) {
            DDS3_GBWK->conditionalEncounterGauge = DDS3_GBWK->conditionalEncounterGauge + dds3GetRandIntA(0, 7) + 6;
        } else {
            DDS3_GBWK->conditionalEncounterGauge = DDS3_GBWK->conditionalEncounterGauge + dds3GetRandIntA(0, 3) + 2;
        }
    }
    if (DDS3_GBWK->conditionalEncounterGauge > 0xFDE8) {
        DDS3_GBWK->conditionalEncounterGauge = -0x218;
    }
    if (DDS3_GBWK->conditionalEncounterGauge < threshold) {
        return BTL_CONDITIONAL_ENCOUNTER_ROLL_NO_SELECTION;
    }
    total = 0;
    for (i = 0; i < 20; i++) {
        if (datBattleAdjustmentRecords[encounterIndex].groups[group].entries[i].battleMode != 0) {
            total += datBattleAdjustmentRecords[encounterIndex].groups[group].entries[i].weight;
        }
    }
    count = 0;
    roll = dds3GetRandIntA(0, total);
    for (i = 0; i < 20; i++) {
        if (datBattleAdjustmentRecords[encounterIndex].groups[group].entries[i].battleMode != 0) {
            if (roll < count + datBattleAdjustmentRecords[encounterIndex].groups[group].entries[i].weight) {
                scene = datBattleAdjustmentRecords[encounterIndex].groups[group].entries[i].battleMode;
                if ((DDS3_GBWK->world.fieldFlags & DAT_WORLD_FIELD_ENCOUNTER_LEVEL_GATE) == 0 || btlCheckScenePartyLevelThreshold(scene) != BTL_SCENE_PARTY_LEVEL_THRESHOLD_REJECTED) {
                    packed = (group << 24) | (i << 16) | scene;
                    btlSelectedConditionalEncounterIndex = encounterIndex;
                    btlConditionalEncounterCachedInterval = 0;
                    btlSelectedConditionalEncounterResult = packed;
                    return packed;
                }
                dds3WorkClear();
                return BTL_CONDITIONAL_ENCOUNTER_ROLL_NO_SELECTION;
            }
            count += datBattleAdjustmentRecords[encounterIndex].groups[group].entries[i].weight;
        }
    }
    return BTL_CONDITIONAL_ENCOUNTER_ROLL_NO_SELECTION;
}

void dds3WorkClear(void) {
    DDS3_GBWK->conditionalEncounterStepRemainder = 0;
    DDS3_GBWK->conditionalEncounterGauge = 0;
    btlConditionalEncounterCachedInterval = 0;
}

void func_0011CE30(void) {
}

void btlPreemptiveChanceHook(void) {
}

/* The serial-battle caller supplies its clamped chance; retail leaves this hook empty. */
void btlSerialBattleChanceHook(s32 chance) {
}

/* The surprise-probability diagnostic hook receives the final chance. */
void btlSurpriseChanceHook(s32 chance) {
}

s32 evtPushRequestedRosterPresence(void) {
    s32 unitId = scrGetIntPara(0);

    scrSetIntReturnValue(
        ptyIsPartyUnitPresent(unitId) == PTY_PARTY_UNIT_PRESENCE_FOUND);
    return 1;
}

INCLUDE_SDATA(const s32, "newdata/datCalc", datAffinityTablePrimary);

INCLUDE_SDATA(const s32, "newdata/datCalc", datAffinityTableAlternate);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_003BAA10);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_003BAA14);

INCLUDE_SDATA(const s32, "newdata/datCalc", datRosterDetails);

INCLUDE_SDATA(const s32, "newdata/datCalc", datEnemyRecords);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_003BAA20);

INCLUDE_SDATA(const s32, "newdata/datCalc", datEnemyAiRecords);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_003BAA28);

INCLUDE_SDATA(const s32, "newdata/datCalc", datExternalAffinityTable);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_003BAA30);

INCLUDE_SDATA(const s32, "newdata/datCalc", datBattleSceneRecords);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_003BAA38);

INCLUDE_SDATA(const s32, "newdata/datCalc", datBattleAdjustmentRecords);

INCLUDE_SDATA(const s32, "newdata/datCalc", fldEncounterRollTable);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_003BAA44);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_003BAA48);

INCLUDE_SDATA(const s32, "newdata/datCalc", datCommandSelectors);

INCLUDE_SDATA(const s32, "newdata/datCalc", datCommandRecords);

INCLUDE_SDATA(const s32, "newdata/datCalc", datAffinityRecords);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_003BAA58);

INCLUDE_SDATA(const s32, "newdata/datCalc", datAbilityParameters);

INCLUDE_SDATA(const s32, "newdata/datCalc", datActionAnimationRecords);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_003BAA64);

INCLUDE_SDATA(const s32, "newdata/datCalc", datItemSkillRecords);

INCLUDE_SDATA(const s32, "newdata/datCalc", datBattleParameters);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_003BAA70);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_003BAA74);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_003BAA78);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_003BAA7C);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_003BAA80);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_003BAA84);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_003BAA88);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_003BAA8C);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_003BAA90);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_003BAA94);

INCLUDE_SDATA(const s32, "newdata/datCalc", datSkillMessages);

INCLUDE_SDATA(const s32, "newdata/datCalc", datItemMessages);

INCLUDE_SDATA(const s32, "newdata/datCalc", datCommandHelpMessages);

INCLUDE_SDATA(const s32, "newdata/datCalc", datStatusHelpMessages);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_003BAAA8);

INCLUDE_SDATA(BfFlw0Header *, "newdata/datCalc", D_003BAAAC);

INCLUDE_SDATA(const s32, "newdata/datCalc", evtWorkScriptTask);

INCLUDE_SDATA(const s32, "newdata/datCalc", btlConditionalEncounterCachedInterval);

INCLUDE_SDATA(const s32, "newdata/datCalc", btlSelectedConditionalEncounterIndex);

INCLUDE_SDATA(const s32, "newdata/datCalc", btlSelectedConditionalEncounterResult);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_003BAAC4);
