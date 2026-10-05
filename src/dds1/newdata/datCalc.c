#include "common.h"

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

typedef struct DatSkillOwner {
    u16 flags;        /* 0x00: 0x20 selects the external skill table */
    u16 unk2;
    u16 skillTableIndex; /* 0x04: index used when the external-table flag is set */
    u8 unk6[0x1C];
    u16 skills[DAT_INLINE_SKILL_COUNT]; /* 0x22 */
} DatSkillOwner;

extern s32 ptyComputeMaxHp(s32 unit);
extern s32 ptyComputeMaxMp(s32 unit);
extern s32 datUnitHasSkill(struct DatSkillOwner *unit, s32 skillId);

/* Current/maximum HP and MP halfwords. Legacy type name retained here. */
typedef struct DatCalcCursor {
    u8 unk0[6]; /* 0x0 */
    u16 hp;      /* 0x6 */
    u16 maxHp;   /* 0x8 */
    u16 mp;      /* 0xA */
    u16 maxMp;   /* 0xC */
} DatCalcCursor;

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

/* Clear the requested bits of the halfword status without touching other bytes.
 * Keep the existing raw access and compound-assignment narrowing. */
void datClearUnitStatusBits(u8 *work, s32 mask) {
    *(u16 *)(work + 0xE) &= ~mask;
}

/* Add each owned HP bonus separately against the original maximum, truncating
 * each unsigned percentage independently. Only inline-skill units cap at 999. */
u32 datComputeSkillBoostedMaxHp(DatSkillOwner *unit) {
    u32 bonusHp = 0;
    u32 maxHp = ptyComputeMaxHp((s32)unit);

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
u32 datComputeSkillBoostedMaxMp(DatSkillOwner *unit) {
    u32 bonusMp = 0;
    u32 maxMp = ptyComputeMaxMp((s32)unit);

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
void datAdjustCurrentHp(DatCalcCursor *unit, s32 delta) {
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
void datAdjustCurrentMp(DatCalcCursor *unit, s32 delta) {
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

extern s8 ptyGetCurrentProfileId(u8 *unit);
extern s32 func_002CDDB0(u16 profileId, s32 statIndex);

typedef struct DatUnitStatus {
    u16 flags;
    u16 affinityTableIndex;
    u16 unitId;
    u16 hp;
    u16 maxHp;
    u16 mp;
    u16 maxMp;
    u16 status;          /* 0x0E */
    u8 pad10[6];
    s8 statValues[0x100];
} DatUnitStatus;

/* Add the signed per-unit stat byte to its profile adjustment and clamp
 * the existing signed result to 1..99. Stat index is not checked here. */
s32 datGetClampedProfileAdjustedStat(DatUnitStatus *unit, s32 statIndex) {
    s32 adjustedStat = unit->statValues[statIndex] +
                func_002CDDB0((u16)ptyGetCurrentProfileId((u8 *)unit), statIndex);

    if (adjustedStat <= 0) {
        adjustedStat = 1;
    }
    if (adjustedStat >= DAT_PROFILE_STAT_LIMIT) adjustedStat = DAT_PROFILE_STAT_MAX;
    return adjustedStat;
}

/* Only an exact masked status of 0x1000 forces one; combinations of low
 * status bits do not take this path. Otherwise use the profile-adjusted stat. */
s32 datGetStatWithStatusOverride(DatUnitStatus *unit, s32 statIndex) {
    if ((unit->status & DAT_STATUS_VALUE_MASK) == DAT_STATUS_STAT_OVERRIDE) {
        return 1;
    }
    return datGetClampedProfileAdjustedStat(unit, statIndex);
}


typedef struct DatPartyMember {
    u8 pad00[0xE];
    s16 affinityTableIndex; /* 0x0E: signed affinity-bank row */
    u8 pad10[8];
    u16 skills[DAT_TABLE_SKILL_COUNT]; /* 0x18 */
    u8 unk28[0x24];
} DatPartyMember; /* 0x4C */

extern DatPartyMember *datEnemyRecords;

/* Search 24 inline skill IDs or eight IDs in the selected external record.
 * Return on the first match; no record-index or pointer validation here. */
s32 datUnitHasSkill(DatSkillOwner *unit, s32 skillId) {
    s32 skillIndex;

    if (!(unit->flags & DAT_EXTERNAL_SKILL_TABLE)) {
        for (skillIndex = 0; skillIndex < DAT_INLINE_SKILL_COUNT; skillIndex++) {
            if (unit->skills[skillIndex] == skillId) {
                return 1;
            }
        }
    } else {
        for (skillIndex = 0; skillIndex < DAT_TABLE_SKILL_COUNT; skillIndex++) {
            if (datEnemyRecords[unit->skillTableIndex].skills[skillIndex] == skillId) {
                return 1;
            }
        }
    }
    return 0;
}

/* Unless a skip-status bit is set, adjust only the low 16 bits by exact masked
 * status: 8/0x100 floor at 200, 2 at 150, 1 at 300, and 4 forces one.
 * High bits are retained; combined or unlisted statuses leave the value alone. */
s32 datRaiseCalculatedValueFloor(DatUnitStatus *unit, s32 packedValue) {
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

extern s32 *D_003BAA08;
extern s32 *D_003BAA0C;
extern s32 *D_003BAA2C;

s32 func_00119520(DatUnitStatus *unit, s32 element) {
    s32 value;
    u16 unitId;

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
            value = D_003BAA08[unitId * 19 + element];
        } else {
            value = D_003BAA0C[unitId * 19 + element];
        }
    } else {
        if (unit->affinityTableIndex != 0) {
            value = D_003BAA2C[unit->affinityTableIndex * 19 + element];
        } else {
            value = D_003BAA2C[datEnemyRecords[unitId].affinityTableIndex * 19 + element];
        }
    }

    if (element == 15) {
        value = datRaiseCalculatedValueFloor(unit, value);
    }
    if (unit->flags & 0x2000) {
        value = D_003BAA2C[383 * 19 + element];
    }
    if ((unit->status & DAT_STATUS_VALUE_MASK) == 0x800) {
        value = D_003BAA2C[382 * 19 + element];
    }
    if ((unit->status & DAT_STATUS_VALUE_MASK) == 0x1000) {
        value = D_003BAA2C[381 * 19 + element];
    }
    if ((unit->status & DAT_STATUS_VALUE_MASK) == 4) {
        if (element < 2) {
            if (element >= 0) {
                if ((u16)value < 100) {
                    value = (value & DAT_CALC_HIGH_MASK) | 100;
                }
            }
        }
    }
    return value;
}

u32 datReadLowHalfOfCalculatedValue(DatUnitStatus *unit, s32 element) {
    return (u16)func_00119520(unit, element);
}

u32 datReadHighHalfOfCalculatedValue(DatUnitStatus *unit, s32 element) {
    return func_00119520(unit, element) & DAT_CALC_HIGH_MASK;
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
s32 datIsValueBelowQuarterMax(DatCalcCursor *object) {
    return object->hp * DAT_PERCENT_SCALE / object->maxHp < DAT_QUARTER_PERCENT;
}

extern s32 datGameState;

typedef struct DatGameCounters {
    u8 pad00[0x3C];
    s32 currency;        /* 0x3C: clamped to 0..9,999,999 */
} DatGameCounters;

/* Add delta using the existing s32 addition, then clamp the currency balance
 * to 0..9,999,999. No overflow validation is performed before the clamp. */
s32 datAddCurrencyClamped(s32 delta) {
    s32 balance = ((DatGameCounters *)datGameState)->currency + delta;
    if (balance < 0) {
        balance = 0;
    }
    if (balance > DAT_CURRENCY_MAX) {
        balance = DAT_CURRENCY_MAX;
    }
    ((DatGameCounters *)datGameState)->currency = balance;
    return balance;
}

/* Compare the signed balance against the requested amount; no debit or
 * validation is performed, so negative requests keep their original semantics. */
s32 datHasEnoughCurrency(s32 requiredAmount) {
    if (*(s32 *)(datGameState + 0x3C) < requiredAmount) {
        return 0;
    }
    return 1;
}

INCLUDE_SDATA(const s32, "newdata/datCalc", D_003BAA08);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_003BAA0C);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_003BAA10);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_003BAA14);

INCLUDE_SDATA(const s32, "newdata/datCalc", datRosterDetails);

INCLUDE_SDATA(const s32, "newdata/datCalc", datEnemyRecords);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_003BAA20);

INCLUDE_SDATA(const s32, "newdata/datCalc", datEnemyAiRecords);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_003BAA28);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_003BAA2C);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_003BAA30);

