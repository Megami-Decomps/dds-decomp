#include "common.h"
#include "sdf.h"
#include "dat_state.h"

#define DAT_EXTERNAL_SKILL_TABLE 0x20
#define DAT_INLINE_SKILL_COUNT 0x18
#define DAT_TABLE_SKILL_COUNT 8
#define DAT_SKILL_HP_BONUS_SMALL 0x220
#define DAT_SKILL_HP_BONUS_MEDIUM 0x221
#define DAT_SKILL_HP_BONUS_LARGE 0x222
#define DAT_SKILL_MP_BONUS_SMALL 0x223
#define DAT_SKILL_MP_BONUS_MEDIUM 0x224
#define DAT_SKILL_MP_BONUS_LARGE 0x225
#define DAT_PERCENT_SCALE 100
#define DAT_BONUS_SMALL_PERCENT 10
#define DAT_BONUS_MEDIUM_PERCENT 20
#define DAT_BONUS_LARGE_PERCENT 30
#define DAT_BOOSTED_RESOURCE_LIMIT 1000
#define DAT_BOOSTED_RESOURCE_MAX 999
#define DAT_STATUS_VALUE_MASK 0x7FFF
#define DAT_STATUS_STAT_OVERRIDE 0x1000
#define DAT_PROFILE_STAT_LIMIT 128
#define DAT_PROFILE_STAT_MAX 127
#define DAT_FINAL_STAT_LIMIT 100
#define DAT_FINAL_STAT_MAX 99
#define DAT_CALC_HIGH_MASK 0xFFFF0000
#define DAT_CALC_SKIP_STATUS_MASK 0x70000
#define DAT_QUARTER_PERCENT 25
#define DAT_CURRENCY_MAX 0x98967F


extern s32 ptyComputeMaxHp(DatPartyRecord *unit);
extern s32 ptyComputeMaxMp(DatPartyRecord *unit);
extern s32 datUnitHasSkill(DatPartyRecord *unit, s32 skillId);



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
void datClearUnitStatusBits(DatPartyRecord *work, s32 mask) {
    work->status &= ~mask;
}

/* Add each owned HP bonus separately against the original maximum, truncating
 * each unsigned percentage independently. Only inline-skill units cap at 999. */
u32 datComputeSkillBoostedMaxHp(DatPartyRecord *unit) {
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
u32 datComputeSkillBoostedMaxMp(DatPartyRecord *unit) {
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
void datAdjustCurrentHp(DatPartyRecord *unit, s32 delta) {
    u32 currentHp;

    currentHp = (u32)unit->hp + delta;
    if ((s32)currentHp < 0) {
        currentHp = 0;
    }
    if ((s32)unit->maxHp < (s32)currentHp) {
        currentHp = unit->maxHp;
    }
    unit->hp = (s16)currentHp;
}

/* Add delta to current MP and clamp with the same integer/cast rules as HP. */
void datAdjustCurrentMp(DatPartyRecord *unit, s32 delta) {
    u32 currentMp;

    currentMp = (u32)unit->mp + delta;
    if ((s32)currentMp < 0) {
        currentMp = 0;
    }
    if ((s32)unit->maxMp < (s32)currentMp) {
        currentMp = unit->maxMp;
    }
    unit->mp = (s16)currentMp;
}

extern s32 func_00314C10(DatPartyRecord *unit);
extern u32 prfGetIndexedProfileByte(u16 id, s32 sub);


/* Add the signed per-unit stat byte to its profile adjustment and clamp
 * the existing result to 1..127. Keep the profile callee's unsigned return type. */
s32 datGetClampedProfileAdjustedStat(DatPartyRecord *unit, s32 statIndex) {
    s32 adjustedStat = unit->baseStats[statIndex] +
                prfGetIndexedProfileByte(func_00314C10(unit), statIndex);

    if (adjustedStat <= 0) {
        adjustedStat = 1;
    }
    if (adjustedStat >= DAT_PROFILE_STAT_LIMIT) adjustedStat = DAT_PROFILE_STAT_MAX;
    return adjustedStat;
}
extern s32 ptyGetCombinedRecordAndSlotValue(s32 id, s32 slot);

/* Only an exact masked status of 0x1000 forces one. Otherwise add the
 * record/slot adjustment to the profile stat, then clamp to 0..99 (DDS2 only). */
s32 datGetStatWithStatusOverride(DatPartyRecord *unit, s32 statIndex) {
    s32 adjustedStat;

    if ((unit->status & DAT_STATUS_VALUE_MASK) == DAT_STATUS_STAT_OVERRIDE) {
        return 1;
    }
    adjustedStat = datGetClampedProfileAdjustedStat(unit, statIndex);
    adjustedStat += ptyGetCombinedRecordAndSlotValue(
        unit->itemId, statIndex);
    if (adjustedStat < 0) {
        adjustedStat = 0;
    }
    if (adjustedStat >= DAT_FINAL_STAT_LIMIT) {
        adjustedStat = DAT_FINAL_STAT_MAX;
    }
    return adjustedStat;
}


extern DatEnemyRecord *datEnemyRecords;

/* Search 24 inline skill IDs or eight IDs in the selected external record.
 * Return on the first match; no record-index or pointer validation here. */
s32 datUnitHasSkill(DatPartyRecord *unit, s32 skillId) {
    s32 skillIndex;

    if (!(unit->flags & DAT_EXTERNAL_SKILL_TABLE)) {
        for (skillIndex = 0; skillIndex < DAT_INLINE_SKILL_COUNT; skillIndex++) {
            if (unit->effectData[skillIndex] == skillId) {
                return 1;
            }
        }
    } else {
        for (skillIndex = 0; skillIndex < DAT_TABLE_SKILL_COUNT; skillIndex++) {
            if (datEnemyRecords[unit->unitId].skills[skillIndex] == skillId) {
                return 1;
            }
        }
    }
    return 0;
}

/* Unless a skip-status bit is set, adjust only the low 16 bits by exact masked
 * status: 8/0x100 floor at 200, 2 at 150, 1 at 300, and 4 forces one.
 * High bits are retained; combined or unlisted statuses leave the value alone. */
s32 datRaiseCalculatedValueFloor(DatPartyRecord *unit, s32 packedValue) {
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

extern s32 *D_00435DD8;
extern s32 *D_00435DDC;
extern s32 *D_00435DFC;
extern s32 mdlFlagTest(s32 flag);

s32 datGetEffectiveAffinity(DatPartyRecord *unit, s32 element) {
    s32 value;
    u16 unitId;
    u32 low;

    switch (element) {
    case -1:
        return 0;
    case 16:
    case 17:
    case 18:
        return 100;
    }

    unitId = unit->unitId;
    if (!(unit->flags & DAT_EXTERNAL_SKILL_TABLE)) {
        if (unit->flags & 0x1000) {
            value = D_00435DD8[unitId * 19 + element];
        } else {
            value = D_00435DDC[unitId * 19 + element];
        }
        if (mdlFlagTest(0x80E) && value < 0) {
            value = (value & DAT_CALC_HIGH_MASK) |
                    ((u32)(datBattleParameters->specialAffinityScale * 100.0f) & 0xFFFF);
        }
    } else {
        if (unit->affinityTableIndex != 0) {
            value = D_00435DFC[unit->affinityTableIndex * 19 + element];
        } else {
            value = D_00435DFC[unitId * 19 + element];
        }
    }

    if (element == 15) {
        value = datRaiseCalculatedValueFloor(unit, value);
    }
    if (unit->flags & 0x2000) {
        value = D_00435DFC[383 * 19 + element];
    }
    if ((unit->status & DAT_STATUS_VALUE_MASK) == 0x800) {
        value = D_00435DFC[382 * 19 + element];
    }
    if ((unit->status & DAT_STATUS_VALUE_MASK) == 0x1000) {
        value = D_00435DFC[381 * 19 + element];
    }

    low = value & 0xFFFF;
    if ((unit->status & DAT_STATUS_VALUE_MASK) == 4) {
        if (element < 2) {
            if (element >= 0) {
                if (low < 100) {
                    value = (value & DAT_CALC_HIGH_MASK) | 100;
                    low = value & 0xFFFF;
                }
            }
        }
    }
    if ((unit->status & DAT_STATUS_VALUE_MASK) == 0x400) {
        if (element == 9 && low < 300) {
            value = (value & DAT_CALC_HIGH_MASK) | 300;
            low = value & 0xFFFF;
        }
    }
    if ((unit->status & DAT_STATUS_VALUE_MASK) == 0x10 && element == 12 && low < 200) {
        value = (value & DAT_CALC_HIGH_MASK) | 200;
        low = value & 0xFFFF;
    }

    if (low == 0) {
        if (value < 0) {
            value |= 150;
        } else if (value & 0x170000) {
            value |= 100;
        }
    }
    return value;
}

u32 datReadLowHalfOfCalculatedValue(DatPartyRecord *unit, s32 element) {
    return (u16)datGetEffectiveAffinity(unit, element);
}

u32 datReadHighHalfOfCalculatedValue(DatPartyRecord *unit, s32 element) {
    return datGetEffectiveAffinity(unit, element) & DAT_CALC_HIGH_MASK;
}

/* Map one exact flag to a stat index. Flag one and unknown/combined flags
 * return -1; shared destination indices are intentional. */
s32 datFlagToElementIndex(s32 flag) {
    s32 statIndex = -1;

    switch (flag) {
    case 1:
        statIndex = -1;
        break;
    case 2:
        statIndex = 4;
        break;
    case 4:
        statIndex = 3;
        break;
    case 8:
        statIndex = 14;
        break;
    case 0x10:
        statIndex = 12;
        break;
    case 0x20:
        statIndex = 13;
        break;
    case 0x40:
        statIndex = 7;
        break;
    case 0x80:
        statIndex = 11;
        break;
    case 0x100:
        statIndex = 14;
        break;
    case 0x200:
        statIndex = 10;
        break;
    case 0x400:
        statIndex = 9;
        break;
    case 0x800:
        statIndex = 9;
        break;
    case 0x1000:
        statIndex = 7;
        break;
    case 0x2000:
        statIndex = 7;
        break;
    case 0x4000:
        statIndex = 9;
        break;
    }
    return statIndex;
}

/* Test the truncated current-HP percentage; maximum HP must be nonzero. */
s32 datIsValueBelowQuarterMax(DatPartyRecord *object) {
    return object->hp * DAT_PERCENT_SCALE / object->maxHp < DAT_QUARTER_PERCENT;
}

/* Add delta using the existing s32 addition, then clamp the currency balance
 * to 0..9,999,999. No overflow validation is performed before the clamp. */
s32 datAddCurrencyClamped(s32 delta) {
    s32 balance = datGameState->header.currency + delta;
    if (balance < 0) {
        balance = 0;
    }
    if (balance > DAT_CURRENCY_MAX) {
        balance = DAT_CURRENCY_MAX;
    }
    datGameState->header.currency = balance;
    return balance;
}

/* Compare the signed balance against the requested amount; no debit or
 * validation is performed, so negative requests keep their original semantics. */
s32 datHasEnoughCurrency(s32 requiredAmount) {
    if (datGameState->header.currency < requiredAmount) {
        return 0;
    }
    return 1;
}

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435DD8);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435DDC);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435DE0);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435DE4);

INCLUDE_SDATA(const s32, "newdata/datCalc", datRosterDetails);

INCLUDE_SDATA(const s32, "newdata/datCalc", datEnemyRecords);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435DF0);

INCLUDE_SDATA(const s32, "newdata/datCalc", datEnemyAiRecords);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435DF8);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435DFC);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435E00);

