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
s32 datAdjustCurrentHp(DatPartyRecord *unit, s32 delta) {
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
s32 datAdjustCurrentMp(DatPartyRecord *unit, s32 delta) {
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

extern s32 ptyGetCurrentProfileId(DatPartyRecord *unit);
extern u32 prfGetIndexedProfileByte(u16 id, s32 sub);


/* Add the signed per-unit stat byte to its profile adjustment and clamp
 * the existing result to 1..127. Keep the profile callee's unsigned return type. */
s32 datGetClampedProfileAdjustedStat(DatPartyRecord *unit, s32 statIndex) {
    s32 adjustedStat = unit->baseStats[statIndex] +
                prfGetIndexedProfileByte(ptyGetCurrentProfileId(unit), statIndex);

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

#include "common.h"
#include "scr.h"
#include "sdf.h"
#include "btl_action.h"
#include "dat_state.h"
#include "dat_command.h"

enum {
    PTY_ACTIVE_ROSTER_COUNT = 5,
    PTY_RECOVERY_DELTA = 9999,
    PTY_MAX_LEVEL = 99,
    PTY_LEVEL_LIMIT = 100
};

enum {
    EVT_DEFAULT_STAT_VALUE = 100,
    EVT_ROSTER_DETAIL_KIND = 5,
    EVT_DEFAULT_GROUP_MASK = 4,
    EVT_ALTERNATE_GROUP_MASK = 0x20,
    EVT_RESULT_WRITTEN_FLAG = 1,
    EVT_RESULT_RESET_MASK = 0xFFFE
};

enum {
    PTY_ITEM_FLAG_FIRST = 0x80,
    PTY_ITEM_FLAG_COUNT = 0x20,
    PTY_ITEM_MODEL_FLAG_BASE = 0x980,
    PTY_ITEM_MAX_QUANTITY = 99,
    PTY_ITEM_QUANTITY_LIMIT = 100,
    PTY_ITEM_SINGLE_MAX = 1,
    PTY_ITEM_SINGLE_LIMIT = 2
};

enum {
    PTY_TEMPLATE_KEEP_BASE_LEVEL = 1,
    PTY_TEMPLATE_USE_PARTY_MAX_LEVEL = 4
};

extern u8 D_00386350[];
extern s32 func_0011C6A8(DatPartyRecord *, s32, u8);



extern DatPartyRecord *dds3FindEntry();

extern ScrData *evtWorkScriptTask;

extern DatEnemyRecord *datEnemyRecords;

typedef struct TableEntry32 {
    u16 value; /* 0x0: copied to active roster entry */
    u16 unk2; /* 0x2 */
} TableEntry32;

extern TableEntry32 D_00386248[];

typedef struct Entry4 {
    u16 unk0; /* 0x0 */
    u8 unk2; /* 0x2 */
    u8 pad3; /* 0x3 */
} Entry4;

extern Entry4 D_003862C8[];



/* This separate script context is cleared as one native 24-byte allocation. */
typedef struct EvtScriptContext {
    u16 stateFlags;
    u16 pad02;
    s32 third;
    s32 first;
    s32 second;
    s32 result;
    u16 options;
    u8 pad16[2];
} EvtScriptContext;
typedef char EvtScriptContextSizeCheck[sizeof(EvtScriptContext) == 0x18 ? 1 : -1];


extern void ptyAdjustItemQuantity(s32 itemId, s32 quantityDelta);

extern void ptyRefreshEntryFromSavedTemplate(DatPartyRecord *entry);


extern s32 scrSetIntegerReturnValue();


extern s32 scrReadIntParameter(s32 idx);

extern s32 datGetStatWithStatusOverride(DatPartyRecord *unit, s32 statIndex);


extern s32 datRosterDetails;

extern s32 evtGetMirroredSolarPhase(void);


extern EvtScriptContext D_0043E5C0;


extern void scrSetFloatReturnValue(f32 value);

extern u32 effMiscRandMod(void *stream, u32 modulus);



extern u8 btlIsRuntimeAllocated(void);

extern f32 func_001AD978(void);

extern u32 func_001ADA10(void);

extern s32 D_00435E8C;

extern s32 mdlFlagTest(s32 flagIndex);

extern s8 (*D_00435E3C)[6];

extern s32 D_00386288[];

extern u32 mnuSetPartyEntryCurrentId(DatPartyRecord *, u32);

extern s32 ptyRebalanceFrontline(s32);

extern void scrRemoveAvailableSkillFlagAndSlot(DatPartyRecord *, s32);


extern s32 datUnitHasSkill(DatPartyRecord *, s32);

extern s32 func_0010C058(ScrData *context, s32 procedureIndex);


extern s32 btlAverageAllCurrentForMask(u32 arg0);

extern s32 btlAverageAllMaximumForMask(u32 arg0);

extern s32 func_001B3A00(u32 arg0);


extern void *memset(void *dst, s32 c, u32 n);
extern DatPartyRecord *D_00435DD4;
extern void mdlFlagSet(s32 flagIndex);

/* Flag-range items set their model flag regardless of quantityDelta.
 * Other items add the delta to their byte quantity and clamp it; DDS2 also
 * caps IDs at or above 0xC0 to one. */
void ptyAdjustItemQuantity(s32 itemId, s32 quantityDelta) {
    s32 quantity;

    if ((u32)(itemId - PTY_ITEM_FLAG_FIRST) < PTY_ITEM_FLAG_COUNT) {
        mdlFlagSet(itemId + PTY_ITEM_MODEL_FLAG_BASE);
        return;
    }
    quantity = datGameState->inventory.counts[itemId];
    quantity += quantityDelta;
    if (quantity < 0) {
        quantity = 0;
    }
    if (itemId >= 0xC0) {
        if (quantity >= PTY_ITEM_SINGLE_LIMIT) {
            quantity = PTY_ITEM_SINGLE_MAX;
        }
    } else if (itemId >= 0xA0) {
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
    datGameState->inventory.counts[itemId] = quantity;
}

/* Flag-range items test their model flag and ignore minimumQuantity.
 * Other items require at least the requested byte quantity. */
s32 evtCheckValueThreshold(s32 itemId, s32 minimumQuantity) {
    if ((u32)(itemId - PTY_ITEM_FLAG_FIRST) < PTY_ITEM_FLAG_COUNT) {
        return mdlFlagTest(itemId + PTY_ITEM_MODEL_FLAG_BASE) != 0;
    }
    if (datGameState->inventory.counts[itemId] < minimumQuantity) {
        return 0;
    }
    return 1;
}

/* Flag-range items have one slot; every quantity range caps at 99. */
s32 func_0011A220(s32 itemId) {
    s32 quantity = datGameState->inventory.counts[itemId];

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

u8 evtGetFlaggedRosterValue(DatPartyRecord *entry) {
    if ((entry->flags & 0x20) == 0) {
        return 0;
    }
    return *((u8 *)&datEnemyRecords[entry->unitId] + 4);
}

s32 dds3FindEntryIndex(rosterIndex)
    s32 rosterIndex;
{
    s32 slotIndex = 0;
    DatPartyRecord *entry = datGameState->party;
    do {
        if (entry->flags & DAT_PARTY_FLAG_OCCUPIED) {
            if (entry->unitId == rosterIndex) {
                return slotIndex;
            }
        }
        slotIndex++;
        entry++;
    } while (slotIndex < PTY_ACTIVE_ROSTER_COUNT);
    return -1;
}

/* Read a signed stat byte in the first active roster record. */
s8 ptyReadSignedRosterStatByte(s32 statIndex) {
    return datGameState->party[0].baseStats[statIndex];
}

extern void sdfRaisePackedChannelValue(DatPartyRecord *, u32);

/* Event penalties affect living roster slots, then optionally raise a status channel. */
void func_0011A328(s32 mode) {
    s32 nextHp, loss, slotIndex;
    if (mode == 1 || mode == 4 || mode == 5 || mode == 6) {
        DatPartyRecord *entry = datGameState->party;
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
    if (mode == 2) {
        DatPartyRecord *entry = datGameState->party;
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
    if (mode == 3) {
        DatPartyRecord *entry = datGameState->party;
        slotIndex = 0;
        do {
            if (entry->hp != 0) entry->hp = 1;
            slotIndex++;
            entry++;
        } while (slotIndex < PTY_ACTIVE_ROSTER_COUNT);
    }
    if (mode >= 4 && mode <= 6) {
        slotIndex = 0;
        do {
            if (datGameState->party[slotIndex].hp != 0) {
                if (mode == 4) sdfRaisePackedChannelValue(&datGameState->party[slotIndex], 0x80);
                if (mode == 5) sdfRaisePackedChannelValue(&datGameState->party[slotIndex], 0x40);
                if (mode == 6) sdfRaisePackedChannelValue(&datGameState->party[slotIndex], 0x10);
            }
            slotIndex++;
        } while (slotIndex < PTY_ACTIVE_ROSTER_COUNT);
    }
}


/* Apply field HP attrition without allowing a living roster entry to reach zero. */
void func_0011A510(s32 mode) {
    s32 nextHp, loss, slotIndex;
    if (mode == 1) {
        DatPartyRecord *entry = datGameState->party;
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
    if (mode == 2) {
        DatPartyRecord *entry = datGameState->party;
        slotIndex = 0;
        do {
            if (entry->flags & DAT_PARTY_FLAG_FRONTLINE) {
                if (entry->hp != 0) {
                    nextHp = entry->hp;
                    loss = nextHp / 10;
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
    if (mode == 3) {
        DatPartyRecord *entry = datGameState->party;
        slotIndex = 0;
        do {
            if (entry->flags & DAT_PARTY_FLAG_FRONTLINE) {
                if (entry->hp != 0) {
                    nextHp = entry->hp;
                    loss = (u32)nextHp / 2;
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
    if (mode == 4) {
        DatPartyRecord *entry = datGameState->party;
        slotIndex = 0;
        do {
            if (entry->flags & DAT_PARTY_FLAG_FRONTLINE) {
                if (entry->hp != 0) {
                    nextHp = entry->hp;
                    loss = nextHp * 30 / 100;
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
    s32 index;
    for (index = 0; index < PTY_ACTIVE_ROSTER_COUNT; index++) {
        DatPartyRecord *entry = &datGameState->party[index];
        if (entry->flags & DAT_PARTY_FLAG_OCCUPIED) {
            datAdjustCurrentHp(entry, PTY_RECOVERY_DELTA);
            datAdjustCurrentMp(entry, PTY_RECOVERY_DELTA);
            entry->status &= 0x8000;
        }
    }
}

/* Test occupied entries with nonzero HP; mode 1 additionally requires a frontline member. */
s32 ptyAnyUnitFlagMatch(u32 statusMask, s32 flagMode) {
    s32 slotIndex = 0;
    DatPartyRecord *entry = datGameState->party;
    do {
        if (entry->flags & DAT_PARTY_FLAG_OCCUPIED) {
            if (entry->hp != 0) {
                if (flagMode != 1 || (entry->flags & DAT_PARTY_FLAG_FRONTLINE)) {
                    if (entry->status & statusMask) {
                        return 1;
                    }
                }
            }
        }
        slotIndex++;
        entry++;
    } while (slotIndex < PTY_ACTIVE_ROSTER_COUNT);
    return 0;
}

void evtAdvanceCounterValue(s32 counterAddress, s32 increment) {
    *(s32 *)(counterAddress + 0x10) = *(s32 *)(counterAddress + 0x10) + increment;
}

/* Apply the owned skill's positive HP/MP recovery rate; unsupported skills do nothing. */
void ptyApplySkillRecovery(DatPartyRecord *entry, u32 skillId) {
    f32 recoveryRate;
    s32 hpRecovery;
    s32 mpRecovery;

    if (datUnitHasSkill(entry, skillId) == 0) {
        return;
    }
    hpRecovery = 0;
    mpRecovery = 0;
    recoveryRate = datAbilityParameters[skillId - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
    switch (skillId) {
    case 0x24B:
        if (recoveryRate > 0.0f) {
            hpRecovery = (s32)((f32)entry->maxHp * recoveryRate);
            mpRecovery = (s32)((f32)entry->maxMp * recoveryRate);
        }
        break;
    case 0x24A:
    case 0x24C:
    case 0x270:
        if (recoveryRate > 0.0f) {
            mpRecovery = (s32)((f32)entry->maxMp * recoveryRate);
        }
        break;
    }
    if (hpRecovery > 0) {
        datAdjustCurrentHp(entry, hpRecovery);
    }
    if (mpRecovery > 0) {
        datAdjustCurrentMp(entry, mpRecovery);
    }
}

/* Apply the two selected recovery skills to occupied frontline entries. */
void evtUpdateFlaggedStats(void) {
    s32 index;
    for (index = 0; index < PTY_ACTIVE_ROSTER_COUNT; index++) {
        DatPartyRecord *entry = &datGameState->party[index];
        if (entry->flags & DAT_PARTY_FLAG_OCCUPIED) {
            if (entry->flags & DAT_PARTY_FLAG_FRONTLINE) {
                ptyApplySkillRecovery(entry, 0x24C);
                ptyApplySkillRecovery(entry, 0x270);
            }
        }
    }
}

/* Return whether an occupied frontline entry owns the requested skill. */
s32 evtHasMatchingFlaggedEntry(s32 skillId) {
    s32 slotIndex = 0;
    do {
        DatPartyRecord *entry = &datGameState->party[slotIndex];
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
 * value is intentionally reused: incoming command ID, then resolved result. */
s32 datCalculateCommandBaseValue(DatPartyRecord *entry, s32 value) {
    s32 commandId = value;
    DatCommandRecord *commands = datCommandRecords;

    value = 0;
    switch (commands[commandId].costMode) {
    case DAT_COMMAND_COST_MODE_HP:
        if (entry->flags & 0x20) {
            return 0;
        }
        value = entry->maxHp * commands[commandId].costPercentage / 100 + commands[commandId].costBase;
        if (value <= 0) {
            value = 1;
        }
        break;
    case DAT_COMMAND_COST_MODE_MP:
        if ((entry->flags & 0x20) &&
            (datEnemyRecords[entry->unitId].flags & 0x10)) {
            return 0;
        }
        value = commands[commandId].costPercentage;
        break;
    }
    return value;
}

void ptyInitRuntime(void) {
    s32 i;

    for (i = 0; i < 5; i++) {
        memset(&datGameState->party[i], 0, sizeof(DatPartyRecord));
        datGameState->partyOrder[i] = i;
    }
    datGameState->partyCount = 0;
    datGameState->party[0] = D_00435DD4[1];
    datGameState->party[1] = D_00435DD4[4];
    datGameState->party[2] = D_00435DD4[5];
    datGameState->partyCount = 3;
    for (i = 0; i < 5; i++) {
        datGameState->party[i].menuValue = 0;
        datGameState->party[i].itemId = 0;
    }
    datGameState->header.currency = 0;
    for (i = 0; i < 256; i++) {
        datGameState->inventory.counts[i] = 0;
    }
    for (i = 0; i < 16; i++) {
        memset(&datGameState->templates[i], 0, sizeof(DatPartyRecord));
    }
}

u16 evtGetIndexedEventRecordId(s32 tableIndex) {
    return datItemSkillRecords[tableIndex].commandIndex;
}

/* Read the entry's level, capped at the script-visible maximum. */
u16 dds3Clamp99(s32 entryAddress) {
    s32 level = ((DatPartyRecord *)entryAddress)->level;

    return level < PTY_LEVEL_LIMIT ? level : PTY_MAX_LEVEL;
}

/* Find the first occupied slot with this roster identifier, or return zero. */
DatPartyRecord *dds3FindEntry(rosterIndex)
    s32 rosterIndex;
{
    s32 slotIndex = 0;
    DatPartyRecord *entry = datGameState->party;
    do {
        if (entry->unitId == rosterIndex && (entry->flags & DAT_PARTY_FLAG_OCCUPIED)) {
            return entry;
        }
        slotIndex++;
        entry++;
    } while (slotIndex < PTY_ACTIVE_ROSTER_COUNT);
    return 0;
}

s32 ptyRebalanceFrontline(s32 rosterIndex) {
    DatGameState *scanState;
    DatPartyRecord savedEntry;
    s32 selectedIndex;
    s32 scanIndex;
    s32 frontlineCount;

    selectedIndex = dds3FindEntryIndex(rosterIndex);
    if (selectedIndex < 0) {
        return 0;
    }

    {
        DatPartyRecord *const selectedEntry = &datGameState->party[selectedIndex];
        if ((selectedEntry->flags & DAT_PARTY_FLAG_FRONTLINE) != 0) {
            return 0;
        }
    }

    scanState = datGameState;
    scanIndex = 0;
    frontlineCount = 0;
    for (; scanIndex < PTY_ACTIVE_ROSTER_COUNT; scanIndex++) {
        DatPartyRecord *const currentEntry = &scanState->party[scanIndex];
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

    datGameState->party[selectedIndex].flags |= DAT_PARTY_FLAG_FRONTLINE;
    memcpy(&savedEntry, &datGameState->party[selectedIndex], sizeof(savedEntry));
    memcpy(&datGameState->party[selectedIndex], &datGameState->party[frontlineCount], sizeof(savedEntry));
    memcpy(&datGameState->party[frontlineCount], &savedEntry, sizeof(savedEntry));
    return 1;
}

u8 ptyIsRosterEntryPresent(void) {
    DatPartyRecord *entry;

    entry = dds3FindEntry();
    return entry != 0;
}

/* Return the highest occupied-slot level, or zero when no slot is occupied. */
s32 dds3EntryMax(void) {
    s32 maximumLevel = 0;
    s32 remaining = 4;
    DatPartyRecord *entry = datGameState->party;
    do {
        remaining--;
        if (entry->flags & DAT_PARTY_FLAG_OCCUPIED) {
            s32 level = entry->level;
            if (maximumLevel < level) {
                maximumLevel = level;
            }
        }
        entry++;
    } while (remaining >= 0);
    return maximumLevel;
}

/* Ceiling average of occupied-slot levels, with zero for an empty roster. */
s32 ptyGetRoundedAveragePartyLevel(void) {
    s32 levelSum = 0;
    s32 activeCount = 0;
    s32 remaining = 4;
    DatPartyRecord *entry = datGameState->party;
    do {
        remaining--;
        if ((entry->flags & DAT_PARTY_FLAG_OCCUPIED) != 0) {
            activeCount++;
            levelSum += entry->level;
        }
        entry++;
    } while (remaining >= 0);
    if (activeCount == 0) {
        return 0;
    }
    return (levelSum + activeCount - 1) / activeCount;
}

extern void ptyAccumulateStatGains(s32 *, s32, DatPartyRecord *);
extern s32 ptyComputeTotalExp(DatPartyRecord *, s32);
extern void ptyRecomputeMaxHpMp(DatPartyRecord *);
extern void evtCopyRosterTableValue(DatPartyRecord *);
extern void func_00286618(void);
extern void func_002866C8(void);
extern void func_003140C8(s32, DatPartyRecord *);
void ptyAssignRosterItemAndMarkOwned(DatPartyRecord *entry);

/* Clone an entry template and raise it to the maximum occupied party level. */
void ptyCloneTemplateAtPartyMaxLevel(DatPartyRecord *entry, s32 templateIndex) {
    s32 targetLevel = dds3EntryMax();
    s32 statGains[DAT_BASE_STAT_COUNT];
    s8 *stat;
    s32 *gain;
    s32 remaining;

    *entry = D_00435DD4[templateIndex];
    if (entry->level < targetLevel) {
        ptyAccumulateStatGains(statGains, targetLevel - entry->level, entry);
        stat = entry->baseStats;
        gain = statGains;
        for (remaining = DAT_BASE_STAT_COUNT - 1; remaining >= 0; remaining--) {
            *stat++ += *gain++;
        }
        entry->level = targetLevel;
    }
    entry->totalExp = ptyComputeTotalExp(entry, 0);
    func_003140C8(0, entry);
    evtCopyRosterTableValue(entry);
    ptyAssignRosterItemAndMarkOwned(entry);
}

extern void func_00286BA8(void *record);

/* Consume a saved template, optionally raise its level, then mark its item owned. */
void func_0011B4B0(DatPartyRecord *entry, s32 templateIndex, s32 initFlags) {
    s32 targetLevel = 0;
    s32 maxPartyLevel = dds3EntryMax();
    s32 averagePartyLevel = ptyGetRoundedAveragePartyLevel();
    s32 statGains[DAT_BASE_STAT_COUNT];
    s8 *stat;
    s32 *gain;
    s32 remaining;

    memcpy(entry, &datGameState->templates[templateIndex], sizeof(*entry));
    memset(&datGameState->templates[templateIndex], 0, sizeof(*entry));
    if (!(initFlags & PTY_TEMPLATE_USE_PARTY_MAX_LEVEL)) {
        if (!(initFlags & PTY_TEMPLATE_KEEP_BASE_LEVEL)) {
            targetLevel = averagePartyLevel;
        }
    } else {
        targetLevel = maxPartyLevel;
    }
    if (entry->level < targetLevel) {
        ptyAccumulateStatGains(statGains, targetLevel - entry->level, entry);
        stat = entry->baseStats;
        gain = statGains;
        for (remaining = DAT_BASE_STAT_COUNT - 1; remaining >= 0; remaining--) {
            *stat++ += *gain++;
        }
        entry->level = targetLevel;
        entry->totalExp = ptyComputeTotalExp(entry, 0);
        ptyRecomputeMaxHpMp(entry);
    }
    if (!(initFlags & 2)) {
        func_003140C8(1, entry);
    }
    func_00286BA8(entry);
    if (entry->itemId != 0) {
        datGameState->inventory.counts[entry->itemId] = 1;
    }
}



/* Clone template 1 into roster 2 and inherit its mantra state, then clear the
 * assigned item. The maximum-party-level flag overrides keep-base-level;
 * otherwise use the rounded party average only when keep-base-level is clear. */
void ptyInitRosterAndClearItem(DatPartyRecord *entry, s32 initFlags) {
    s32 targetLevel = 0;
    s32 maxPartyLevel = dds3EntryMax();
    s32 averagePartyLevel = ptyGetRoundedAveragePartyLevel();
    s32 statGains[DAT_BASE_STAT_COUNT];
    s32 statIndex;
    DatGameState *gameState;

    memcpy(entry, &datGameState->templates[1], sizeof(*entry));
    entry->unitId = 2;
    if (!(initFlags & PTY_TEMPLATE_USE_PARTY_MAX_LEVEL)) {
        if (!(initFlags & PTY_TEMPLATE_KEEP_BASE_LEVEL)) {
            targetLevel = averagePartyLevel;
        }
    } else {
        targetLevel = maxPartyLevel;
    }
    if (entry->level < targetLevel) {
        ptyAccumulateStatGains(statGains, targetLevel - entry->level, entry);
        for (statIndex = 0; statIndex < DAT_BASE_STAT_COUNT; statIndex++) {
            entry->baseStats[statIndex] += statGains[statIndex];
        }
        entry->level = targetLevel;
        entry->totalExp = ptyComputeTotalExp(entry, 0);
        ptyRecomputeMaxHpMp(entry);
    }
    evtCopyRosterTableValue(entry);
    gameState = datGameState;
    memcpy(&gameState->mantraBits[2], &gameState->mantraBits[1], sizeof(gameState->mantraBits[2]));
    memcpy(gameState->profileBanks[2].records, gameState->profileBanks[1].records, sizeof(gameState->profileBanks[2].records));
    func_00286618();
    entry->itemId = 0;
}

/* Clone template 7 into roster 3 and inherit its mantra state. After optional
 * post-initialization, mark any assigned item owned. Level-flag precedence is
 * the same as the adjacent initializer; flag mask 2 skips the optional call. */
void func_0011B9A0(DatPartyRecord *entry, s32 initFlags) {
    s32 targetLevel = 0;
    s32 maxPartyLevel = dds3EntryMax();
    s32 averagePartyLevel = ptyGetRoundedAveragePartyLevel();
    s32 statGains[DAT_BASE_STAT_COUNT];
    s32 statIndex;
    DatGameState *gameState;

    memcpy(entry, &datGameState->templates[7], sizeof(*entry));
    entry->unitId = 3;
    if (!(initFlags & PTY_TEMPLATE_USE_PARTY_MAX_LEVEL)) {
        if (!(initFlags & PTY_TEMPLATE_KEEP_BASE_LEVEL)) {
            targetLevel = averagePartyLevel;
        }
    } else {
        targetLevel = maxPartyLevel;
    }
    if (entry->level < targetLevel) {
        ptyAccumulateStatGains(statGains, targetLevel - entry->level, entry);
        for (statIndex = 0; statIndex < DAT_BASE_STAT_COUNT; statIndex++) {
            entry->baseStats[statIndex] += statGains[statIndex];
        }
        entry->level = targetLevel;
        entry->totalExp = ptyComputeTotalExp(entry, 0);
        ptyRecomputeMaxHpMp(entry);
    }
    gameState = datGameState;
    memcpy(&gameState->mantraBits[3], &gameState->mantraBits[7], sizeof(gameState->mantraBits[3]));
    memcpy(gameState->profileBanks[3].records, gameState->profileBanks[7].records, sizeof(gameState->profileBanks[3].records));
    func_002866C8();
    if (!(initFlags & 2)) {
        func_003140C8(1, entry);
    }
    if (entry->itemId != 0) {
        datGameState->inventory.counts[entry->itemId] = 1;
    }
}

extern s32 func_00314B00(DatPartyRecord *, u16);
extern void func_00314A80(DatPartyRecord *, u16);
extern s32 func_00314990(DatPartyRecord *, u16);
extern void func_00314868(DatPartyRecord *, u16);
extern u32 ptyGetProfileRecordValue(DatPartyRecord *, u16);
extern void ptySetProfileRecordValue(DatPartyRecord *, u16, u32);
extern void func_00286738(void);
extern u32 ptyGetSkillNibbleState(DatPartyRecord *, u16);
extern s32 scrSetFlag(DatPartyRecord *, u16);

/* Merge saved templates 1 and 2 into roster 8, taking their greater level
 * and stats and combining both sets of profile and skill flags. */
void ptyMergeSavedUnitTemplates(DatPartyRecord *entry) {
    DatPartyRecord first;
    DatPartyRecord second;
    s32 index;
    s32 present;
    s32 firstValue;
    s32 secondValue;

    memcpy(entry, &datGameState->templates[1], sizeof(*entry));
    entry->unitId = 8;
    memcpy(&first, &datGameState->templates[1], sizeof(first));
    memcpy(&second, &datGameState->templates[2], sizeof(second));
    if (second.menuValue != 0) {
        ptyAdjustItemQuantity(second.menuValue, 1);
    }
    if (first.level > second.level) {
        entry->level = first.level;
    } else {
        entry->level = second.level;
    }
    for (index = 0; index < DAT_BASE_STAT_COUNT; index++) {
        if (first.baseStats[index] > second.baseStats[index]) {
            entry->baseStats[index] = first.baseStats[index];
        } else {
            entry->baseStats[index] = second.baseStats[index];
        }
    }
    ptyRecomputeMaxHpMp(entry);
    entry->totalExp = ptyComputeTotalExp(entry, 0);
    for (index = 1; index < 0xB0; index++) {
        present = func_00314B00(&first, index) != 0;
        if (func_00314B00(&second, index)) {
            present = 1;
        }
        if (present) {
            func_00314A80(entry, index);
        }
        present = func_00314990(&first, index) != 0;
        if (func_00314990(&second, index)) {
            present = 1;
        }
        if (present) {
            func_00314868(entry, index);
        }
        /* This merge compares the raw profile words as signed counters. */
        firstValue = ptyGetProfileRecordValue(&first, index);
        secondValue = ptyGetProfileRecordValue(&second, index);
        if (firstValue >= secondValue) {
            ptySetProfileRecordValue(entry, index, ptyGetProfileRecordValue(&first, index));
        } else {
            ptySetProfileRecordValue(entry, index, ptyGetProfileRecordValue(&second, index));
        }
    }
    func_00286738();
    for (index = 1; index < 0x2A0; index++) {
        present = ptyGetSkillNibbleState(&first, index) != 0;
        if (ptyGetSkillNibbleState(&second, index)) {
            present = 1;
        }
        if (present) {
            scrSetFlag(entry, index);
        }
    }
}

extern s32 func_002D0B08(s32 rosterIndex);
extern s32 D_00386048[][4];

/* Add a roster member to the first free party slot: initialise it from its saved template or by
 * roster kind, refill HP/MP, set its model flags and apply skill-boosted maxima. Returns the
 * slot, -1 if the member is already present or -2 if the party is full. */
s32 func_0011C0B0(s32 rosterIndex, s32 initFlags) {
    DatPartyRecord *entry;
    DatPartyRecord *party;
    s32 slotIndex;
    s32 occupiedCount;
    s32 modelFlagIndex;
    s32 *modelFlags;

    if (dds3FindEntryIndex(rosterIndex) >= 0) {
        return -1;
    }

    for (slotIndex = 0; slotIndex < PTY_ACTIVE_ROSTER_COUNT; slotIndex++) {
        entry = &datGameState->party[slotIndex];
        if ((entry->flags & 1) == 0) {
            break;
        }
    }
    if (slotIndex == PTY_ACTIVE_ROSTER_COUNT) {
        return -2;
    }

    memset(entry, 0, sizeof(*entry));
    if ((datGameState->templates[rosterIndex].flags & 1) != 0 &&
        datGameState->templates[rosterIndex].level != 0) {
        func_0011B4B0(entry, rosterIndex, initFlags);
    } else {
        switch (rosterIndex) {
        case 2:
            ptyInitRosterAndClearItem(entry, initFlags);
            break;
        case 3:
            func_0011B9A0(entry, initFlags);
            break;
        case 8:
            ptyMergeSavedUnitTemplates(entry);
            break;
        default:
            ptyCloneTemplateAtPartyMaxLevel(entry, rosterIndex);
            break;
        }
    }

    entry->flags &= ~2;
    entry->hp = entry->maxHp;
    entry->mp = entry->maxMp;
    entry->status = 0;

    occupiedCount = 0;
    party = datGameState->party;
    for (modelFlagIndex = 0; modelFlagIndex < PTY_ACTIVE_ROSTER_COUNT; modelFlagIndex++) {
        if ((party->flags & 3) == 3) {
            occupiedCount++;
        }
        party++;
    }
    if (occupiedCount == 0) {
        ptyRebalanceFrontline(rosterIndex);
    }

    modelFlags = D_00386048[rosterIndex];
    for (modelFlagIndex = 0; modelFlagIndex < 4U; modelFlagIndex++) {
        s32 modelFlag = *modelFlags++;
        if (modelFlag != 0) {
            mdlFlagSet(modelFlag);
        }
    }

    if (func_002D0B08(rosterIndex) != 0) {
        entry->maxHp = datComputeSkillBoostedMaxHp(entry);
        entry->maxMp = datComputeSkillBoostedMaxMp(entry);
        entry->hp = entry->maxHp;
        entry->mp = entry->maxMp;
    }
    return slotIndex;
}

void func_0011C328(u32 arg0) {
    func_0011C0B0(arg0, 0);
}

INCLUDE_ASM(const s32, "newdata/datCalc", func_0011C340);

void func_0011C680(u32 arg0) {
    func_0011C340(arg0, 0);
}

u32 func_0011C698(void) {
    return 0;
}

u32 func_0011C6A0(void) {
    return 1;
}

typedef struct DatSkillList {
    s16 skills[24];
} DatSkillList;

extern DatSkillList *D_00435E28;

/* Test whether a party record satisfies one participant requirement of an affinity. */
s32 func_0011C6A8(DatPartyRecord *unit, s32 affinity, u8 slot) {
    DatAffinityRecord *record;
    s32 requirement;
    s32 value;
    u32 i;

    if (unit == NULL) {
        return 0;
    }
    record = &datAffinityRecords[affinity - DAT_AFFINITY_FIRST_COMMAND];
    if (record->flags & 2) {
        return 0;
    }
    requirement = record->requirements[slot];
    if (requirement == -1) {
        return 0;
    }
    switch (requirement & 0xF0000000) {
    case 0:
        if (requirement == 0 || datUnitHasSkill(unit, requirement) != 0) {
            return 1;
        }
        break;
    case 0x10000000:
        value = requirement & 0x0FFFFFFF;
        for (i = 0; i < 24; i++) {
            if (unit->effectData[i] != 0 && (1 << datCommandSelectors[unit->effectData[i]].stat) == value) {
                return 1;
            }
        }
        break;
    case 0x20000000:
        value = requirement & 0x0FFFFFFF;
        if (value == 0 || value == (1 << unit->unitId)) {
            return 1;
        }
        break;
    case 0x40000000:
        value = requirement & 0x0FFFFFFF;
        for (i = 0; i < 24; i++) {
            s16 skill = D_00435E28[value].skills[i];

            if (skill >= 0 && datUnitHasSkill(unit, skill)) {
                return 1;
            }
        }
        break;
    }
    return 0;
}

/* Try the six stored orders; success requires exactly the non-sentinel requirement count. */
s32 ptyMatchAffinityPermutation(s32 *actors, s32 affinity) {
    s32 *requirementCursor = datAffinityRecords[affinity - DAT_AFFINITY_FIRST_COMMAND].requirements;
    u32 i;
    s32 requiredCount = 0;
    u32 orderIndex;
    s32 orderOffset;
    s32 matchedCount;
    u8 *orderCursor;
    s32 *actorCursor;
    s32 actorValue;

    for (i = 0; i < 3; i++) {
        if (*requirementCursor++ != -1) {
            requiredCount++;
        }
    }
    for (orderIndex = 0, orderOffset = 0; orderIndex < 6; orderIndex++, orderOffset += 3) {
        matchedCount = 0;
        for (i = 0, orderCursor = D_00386350 + orderOffset, actorCursor = actors; i < 3; i++, orderCursor++) {
            actorValue = *actorCursor++;
            if (actorValue != 0 && func_0011C6A8(actorValue, affinity, *orderCursor) != 0) {
                matchedCount++;
            }
        }
        if (requiredCount == matchedCount) {
            return 1;
        }
    }
    return 0;
}

void evtCopyRosterTableValue(DatPartyRecord *entry) {
    entry->menuValue = D_00386248[entry->unitId].value;
}

/* Copy table values only for occupied slots whose roster identifier is below 16. */
void evtUpdateFlaggedEntries(void) {
    s32 slotIndex;
    for (slotIndex = 0; slotIndex < PTY_ACTIVE_ROSTER_COUNT; slotIndex++) {
        DatPartyRecord *entry = &datGameState->party[slotIndex];
        if (entry->flags & DAT_PARTY_FLAG_OCCUPIED) {
            s32 rosterIndex = 0;
            do {
                if (entry->unitId == rosterIndex) {
                    evtCopyRosterTableValue(entry);
                }
                rosterIndex++;
            } while (rosterIndex < 16);
        }
    }
}

/* Apply the two configured count/flag updates; a zero index disables its record. */
void dds3ForEachEntry(void) {
    Entry4 *updates = D_003862C8;
    u32 updateIndex = 0;

    do {
        u16 valueIndex = updates->unk0;
        u8 delta = updates->unk2;

        updates++;
        if (valueIndex != 0) {
            ptyAdjustItemQuantity(valueIndex, delta);
        }
        updateIndex++;
    } while (updateIndex < 2);
}

extern void func_00286670(void);
extern void func_00286700(void);
extern void scrClearAllSecondaryScriptFlags(DatPartyRecord *);
extern void scrSetSecondaryScriptFlag(DatPartyRecord *, u16);
extern u16 D_003862D0[16][4];

/* Restore special-character banks, occupied stock skills and four presets. */
void ptyRefreshEntryFromSavedTemplate(DatPartyRecord *entry) {
    u16 unitId = entry->unitId;
    u32 stockId;
    u16 occupied;
    DatPartyRecord *stock;
    u32 i;
    u16 *preset;
    if (!mdlFlagTest(0x80E)) {
        switch (unitId) {
        case 1:
            stockId = 8;
            datGameState->mantraBits[1] = datGameState->mantraBits[8];
            datGameState->profileBanks[1] = datGameState->profileBanks[8];
            func_00286670();
            break;
        case 7:
            stockId = 7;
            if (datGameState->templates[3].totalExp != 0) {
                stockId = 3;
                datGameState->mantraBits[7] = datGameState->mantraBits[3];
                datGameState->profileBanks[7] = datGameState->profileBanks[3];
                func_00286700();
            }
            break;
        default:
            stockId = unitId;
            break;
        }
        stock = &datGameState->templates[stockId];
        occupied = stock->flags & DAT_PARTY_FLAG_OCCUPIED;
        if (occupied != 0) {
            for (i = 0; i < 85; i++)
                entry->skillFlags[i] |= stock->skillFlags[i];
            entry->profileId = stock->profileId;
        }
    }
    scrClearAllSecondaryScriptFlags(entry);
    preset = D_003862D0[unitId];
    i = 0;
    do {
        u32 value = *preset++;
        u16 skill = value;
        if (value != 0) {
            scrSetFlag(entry, skill);
            scrSetSecondaryScriptFlag(entry, skill);
        }
        i++;
    } while (i < 4);
}

/* Visit occupied roster slots for native per-entry processing. */
void dds3ForEachFlagged(void) {
    s32 slotIndex;
    for (slotIndex = 0; slotIndex < PTY_ACTIVE_ROSTER_COUNT; slotIndex++) {
        DatPartyRecord *entry = &datGameState->party[slotIndex];

        if (entry->flags & DAT_PARTY_FLAG_OCCUPIED) {
            ptyRefreshEntryFromSavedTemplate(entry);
        }
    }
}

void ptyClearSelectedSkillFlagsFromActiveEntries(void) {
    s32 slotIndex;
    for (slotIndex = 0; slotIndex < PTY_ACTIVE_ROSTER_COUNT; slotIndex++) {
        DatPartyRecord *entry = &datGameState->party[slotIndex];
        if ((entry->flags & DAT_PARTY_FLAG_OCCUPIED) != 0) {
            scrRemoveAvailableSkillFlagAndSlot(entry, 0x5B);
            scrRemoveAvailableSkillFlagAndSlot(entry, 0x5C);
            scrRemoveAvailableSkillFlagAndSlot(entry, 0x5D);
        }
    }
}

void func_0011D0D8(void) {
    s32 slotIndex;

    for (slotIndex = 0; slotIndex < PTY_ACTIVE_ROSTER_COUNT; slotIndex++) {
        DatPartyRecord *entry = &datGameState->party[slotIndex];

        if (entry->flags & DAT_PARTY_FLAG_OCCUPIED) {
            if (entry->itemId == 0xF8) {
                entry->itemId = 0;
            }
        }
    }
    datGameState->inventory.counts[0xF8] = 0;
}

/* Save occupied active entries to stock; clear the absent special-character slots. */
extern void func_0011D130(void);
void func_0011D130(void) {
    s32 slotIndex;

    for (slotIndex = 0; slotIndex < PTY_ACTIVE_ROSTER_COUNT; slotIndex++) {
        DatPartyRecord *entry = &datGameState->party[slotIndex];

        if (entry->flags & DAT_PARTY_FLAG_OCCUPIED) {
            memcpy(&datGameState->templates[entry->unitId], entry, sizeof(*entry));
        }
    }
    if (dds3FindEntryIndex(3) < 0) {
        memset(&datGameState->templates[3], 0, sizeof(DatPartyRecord));
    }
    if (dds3FindEntryIndex(7) < 0) {
        memset(&datGameState->templates[7], 0, sizeof(DatPartyRecord));
    }
}


void evtRandomizeEntryValue(DatPartyRecord *unit) {
    s32 randomOffset;

    randomOffset = effMiscRandMod(0, 4);
    unit->randomizedValue = 0x12 - randomOffset;
}

/* Clear a subset of per-unit status flags on occupied qualifying entries,
 * gated by the event RNG; report whether any flags were cleared. */
s32 evtClearRandomStatusFlags(void) {
    s32 clearedAny = 0;
    s32 remaining;
    DatPartyRecord *entry;
    s32 roll = (s32)effMiscRandMod(0, 100);
    if (roll >= 51) {
        return 0;
    }
    remaining = 4;
    entry = datGameState->party;
    do {
        if ((entry->flags & DAT_PARTY_FLAG_OCCUPIED) != 0 && entry->hp != 0) {
            u16 statusFlags = entry->status;
            if ((statusFlags & 0x5D0) != 0) {
                entry->status = statusFlags & ~0x5D0;
                clearedAny = 1;
            }
        }
        remaining--;
        entry++;
    } while (remaining >= 0);
    return clearedAny;
}

s32 ptyGetCombinedRecordAndSlotValue(s32 id, s32 slot) {
    s32 index = id - 0xC0;
    if (index <= 0) {
        return 0;
    }
    return D_00435E3C[index][slot] + datGameState->itemStatBonuses[index][slot];
}

void ptyAddClampedEntryValue(DatPartyRecord *entry, s32 statIndex, s32 amount) {
    s32 updated = entry->baseStats[statIndex] + amount;
    if (updated < 0) {
        updated = 0;
    }
    if (updated >= 100) {
        updated = 99;
    }
    entry->baseStats[statIndex] = updated;
}

void ptyAssignRosterItemAndMarkOwned(DatPartyRecord *entry) {
    s32 value = D_00386288[entry->unitId];
    mnuSetPartyEntryCurrentId(entry, value);
    if (value != 0) {
        datGameState->inventory.counts[value] = 1;
    }
}

void ptyAssignPartyRosterItemsAndMarkOwned(void) {
    s32 slotIndex;
    for (slotIndex = 0; slotIndex < PTY_ACTIVE_ROSTER_COUNT; slotIndex++) {
        DatPartyRecord *entry = &datGameState->party[slotIndex];
        if (entry->flags & DAT_PARTY_FLAG_OCCUPIED) {
            s32 index = 0;
            do {
                if (entry->unitId == index) {
                    ptyAssignRosterItemAndMarkOwned(entry);
                }
                index++;
            } while (index < 16);
        }
    }
}

/* Step the script with these context values; clear the written flag, not the stored result. */
s32 evtRunContext(s32 script, s32 first, s32 second, s32 third, u16 options) {
    func_0010C058(evtWorkScriptTask, script);
    D_0043E5C0.third = third;
    D_0043E5C0.first = first;
    D_0043E5C0.second = second;
    D_0043E5C0.options = options;
    D_0043E5C0.stateFlags &= EVT_RESULT_RESET_MASK;
    bfStepContext(evtWorkScriptTask);
    return D_0043E5C0.result;
}

/* Create the script task and clear its separate 24-byte context. */
void dds3WorkInit(void *header) {
    evtWorkScriptTask = scrCreateTaskWithDefaultOption(header);
    memset(&D_0043E5C0, 0, sizeof(D_0043E5C0));
}

u32 scrGetWorkTaskHandle(void) {
    return (u32)evtWorkScriptTask;
}

void scrDestroyWorkTask(void) {
    scrProcDestroyTask(evtWorkScriptTask);
    evtWorkScriptTask = 0;
}

s32 evtPushFirstRosterLevel(void) {
    scrSetIntegerReturnValue(((DatPartyRecord *)D_0043E5C0.first)->level);
    return 1;
}

s32 evtPushSecondRosterLevel(void) {
    scrSetIntegerReturnValue(((DatPartyRecord *)D_0043E5C0.second)->level);
    return 1;
}

s32 evtPushFirstRosterCurrentHp(void) {
    scrSetIntegerReturnValue(((DatPartyRecord *)D_0043E5C0.first)->hp);
    return 1;
}

s32 evtPushSecondRosterCurrentHp(void) {
    scrSetIntegerReturnValue(((DatPartyRecord *)D_0043E5C0.second)->hp);
    return 1;
}

s32 evtPushFirstRosterMaximumHp(void) {
    scrSetIntegerReturnValue(((DatPartyRecord *)D_0043E5C0.first)->maxHp);
    return 1;
}

s32 evtPushSecondRosterMaximumHp(void) {
    scrSetIntegerReturnValue(((DatPartyRecord *)D_0043E5C0.second)->maxHp);
    return 1;
}

extern s32 btlResolveUnitValueWithOverride(s32, s32);
extern u32 datReadLowHalfOfCalculatedValue(DatPartyRecord *, s32);
/* Push the first entry's selected stat; selectors -1, 16 and 17 use the default. */
s32 evtPushFirstRosterSelectedStat(void) {
    s8 statIndex = datCommandSelectors[D_0043E5C0.third].stat;
    s32 statValue;

    switch (statIndex) {
    case -1:
    case 16:
    case 17:
        statValue = EVT_DEFAULT_STAT_VALUE;
        break;
    default:
        if (btlIsRuntimeAllocated()) {
            statValue = (u16)btlResolveUnitValueWithOverride(D_0043E5C0.first, statIndex);
        } else {
            statValue = datReadLowHalfOfCalculatedValue(
                (DatPartyRecord *)D_0043E5C0.first, statIndex);
        }
        break;
    }
    scrSetIntegerReturnValue(statValue);
    return 1;
}

/* Push the second entry's selected stat; selectors -1, 16 and 17 use the default. */
s32 evtPushSecondRosterSelectedStat(void) {
    s8 statIndex = datCommandSelectors[D_0043E5C0.third].stat;
    s32 statValue;

    switch (statIndex) {
    case -1:
    case 16:
    case 17:
        statValue = EVT_DEFAULT_STAT_VALUE;
        break;
    default:
        if (btlIsRuntimeAllocated()) {
            statValue = (u16)btlResolveUnitValueWithOverride(D_0043E5C0.second, statIndex);
        } else {
            statValue = datReadLowHalfOfCalculatedValue(
                (DatPartyRecord *)D_0043E5C0.second, statIndex);
        }
        break;
    }
    scrSetIntegerReturnValue(statValue);
    return 1;
}

extern s32 datFlagToElementIndex(s32);

/* Push the first entry's option-selected stat, retaining the same default selectors. */
s32 evtPushFirstRosterOptionStat(void) {
    s8 statIndex = datFlagToElementIndex(D_0043E5C0.options);
    s32 statValue = EVT_DEFAULT_STAT_VALUE;

    switch (statIndex) {
    case -1:
    case 16:
    case 17:
        break;
    default:
        if (btlIsRuntimeAllocated()) {
            statValue = (u16)btlResolveUnitValueWithOverride(D_0043E5C0.first, statIndex);
        } else {
            statValue = datReadLowHalfOfCalculatedValue(
                (DatPartyRecord *)D_0043E5C0.first, statIndex);
        }
        break;
    }
    scrSetIntegerReturnValue(statValue);
    return 1;
}

/* Push the second entry's option-selected stat, retaining the same default selectors. */
s32 evtPushSecondRosterOptionStat(void) {
    s8 statIndex = datFlagToElementIndex(D_0043E5C0.options);
    s32 statValue = EVT_DEFAULT_STAT_VALUE;

    switch (statIndex) {
    case -1:
    case 16:
    case 17:
        break;
    default:
        if (btlIsRuntimeAllocated()) {
            statValue = (u16)btlResolveUnitValueWithOverride(D_0043E5C0.second, statIndex);
        } else {
            statValue = datReadLowHalfOfCalculatedValue(
                (DatPartyRecord *)D_0043E5C0.second, statIndex);
        }
        break;
    }
    scrSetIntegerReturnValue(statValue);
    return 1;
}

/* Push the first entry's profile-adjusted stat, including its native status override. */
s32 evtPushFirstRosterStatEligibility(void) {
    s32 statIndex = scrReadIntParameter(0);

    scrSetIntegerReturnValue(datGetStatWithStatusOverride((DatPartyRecord *)D_0043E5C0.first, statIndex));
    return 1;
}

/* Push the second entry's profile-adjusted stat, including its native status override. */
s32 evtPushSecondRosterStatEligibility(void) {
    s32 statIndex = scrReadIntParameter(0);

    scrSetIntegerReturnValue(datGetStatWithStatusOverride((DatPartyRecord *)D_0043E5C0.second, statIndex));
    return 1;
}

/* Kind 5 selects the first entry's roster detail instead of the command-table stat. */
s32 evtPushSelectedStatOrRosterLowValue(void) {
    s32 statValue;
    s32 commandIndex = D_0043E5C0.third;
    if (datCommandSelectors[commandIndex].kind == EVT_ROSTER_DETAIL_KIND) {
        u16 rosterIndex = ((DatPartyRecord *)D_0043E5C0.first)->unitId;
        statValue = ((EventRosterStat *)datRosterDetails)[rosterIndex].alternateA;
    } else {
        statValue = datCommandRecords[commandIndex].stat11;
    }
    scrSetIntegerReturnValue(statValue);
    return 1;
}

/* Kind 5 scales the command stat by the first entry's roster multiplier, then truncates. */
s32 evtPushSelectedScaledStat(void) {
    s32 statValue;
    s32 commandIndex = D_0043E5C0.third;
    statValue = datCommandRecords[commandIndex].attribute.parts.hitChance;
    if (datCommandSelectors[commandIndex].kind == EVT_ROSTER_DETAIL_KIND) {
        u16 rosterIndex = ((DatPartyRecord *)D_0043E5C0.first)->unitId;
        statValue = (s32)((f32)statValue * ((EventRosterStat *)datRosterDetails)[rosterIndex].multiplier);
    }
    scrSetIntegerReturnValue(statValue);
    return 1;
}

s32 evtPushSelectedStatGrade(void) {
    scrSetIntegerReturnValue(datCommandRecords[D_0043E5C0.third].stat2D);
    return 1;
}

/* Options 1 and 2 select alternate command halfwords; other options push zero. */
s32 evtSelectScriptStatValue(void) {
    EvtScriptContext *context = &D_0043E5C0;
    s32 statValue;
    u16 statOption = context->options;
    switch (statOption) {
    case 1:
        statValue = datCommandRecords[context->third].hpPower;
        break;
    case 2:
        statValue = datCommandRecords[context->third].mpPower;
        break;
    default:
        statValue = 0;
        break;
    }
    scrSetIntegerReturnValue(statValue);
    return 1;
}

/* Select the alternate stat through the first entry's table value, not the context selector. */
s32 evtPushEntryIndexedStatOption(void) {
    EvtScriptContext *context = &D_0043E5C0;
    s32 statValue;
    u16 statOption = context->options;
    u16 commandIndex = datItemSkillRecords[((DatPartyRecord *)context->first)->menuValue].commandIndex;
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
    scrSetIntegerReturnValue(statValue);
    return 1;
}

/* Kind 5 selects the first entry's high roster detail instead of the command total. */
s32 evtPushSelectedTotalOrRosterHighValue(void) {
    s32 statValue;
    s32 commandIndex = D_0043E5C0.third;
    if (datCommandSelectors[commandIndex].kind == EVT_ROSTER_DETAIL_KIND) {
        u16 rosterIndex = ((DatPartyRecord *)D_0043E5C0.first)->unitId;
        statValue = ((EventRosterStat *)datRosterDetails)[rosterIndex].alternateB;
    } else {
        statValue = datCommandRecords[commandIndex].stat34;
    }
    scrSetIntegerReturnValue(statValue);
    return 1;
}

s32 evtPushSelectedStatMaximum(void) {
    scrSetIntegerReturnValue(datCommandRecords[D_0043E5C0.third].stat36);
    return 1;
}

/* Store the script argument as the context result and mark it written. */
s32 evtStoreScriptParameterResult(void) {
    u16 stateFlags = D_0043E5C0.stateFlags | EVT_RESULT_WRITTEN_FLAG;

    D_0043E5C0.stateFlags = stateFlags;
    D_0043E5C0.result = scrReadIntParameter(0);
    return 1;
}

s32 evtPushScriptContextResult(void) {
    scrSetIntegerReturnValue(D_0043E5C0.result);
    return 1;
}

s32 evtPushEntryFlagBitInverted(void) {
    scrSetIntegerReturnValue(((((DatPartyRecord *)D_0043E5C0.first)->flags >> 5) ^ 1) & 1);
    return 1;
}

/* For positive ranges, return 1.0 plus a percentage jitter; the upper endpoint is excluded. */
s32 evtRollRandomScale(void) {
    s32 percentRange = scrReadIntParameter(0);
    s32 roll = effMiscRandMod(0, percentRange * 2);

    scrSetFloatReturnValue((f32)(roll - percentRange + 100) / 100.0f);
    return 1;
}

/* Without a battle runtime, push zero without reading the group-choice argument. */
s32 scrGetBattleAverageCurrentValueForGroup(void) {
    s32 battleAvailable = btlIsRuntimeAllocated();
    s32 averageValue;
    if (battleAvailable) {
        s32 groupChoice = scrReadIntParameter(0);
        averageValue = btlAverageAllCurrentForMask(groupChoice ? EVT_ALTERNATE_GROUP_MASK : EVT_DEFAULT_GROUP_MASK);
    }
    else {
        averageValue = 0;
    }
    scrSetIntegerReturnValue(averageValue);
    return 1;
}

/* Without a battle runtime, push zero without reading the group-choice argument. */
s32 scrGetBattleAverageMaximumValueForGroup(void) {
    s32 battleAvailable = btlIsRuntimeAllocated();
    s32 averageValue;
    if (battleAvailable) {
        s32 groupChoice = scrReadIntParameter(0);
        averageValue = btlAverageAllMaximumForMask(groupChoice ? EVT_ALTERNATE_GROUP_MASK : EVT_DEFAULT_GROUP_MASK);
    }
    else {
        averageValue = 0;
    }
    scrSetIntegerReturnValue(averageValue);
    return 1;
}

/* Without a battle runtime, push zero without reading the group-choice argument. */
s32 scrGetBattleAverageActorStatForGroup(void) {
    s32 battleAvailable = btlIsRuntimeAllocated();
    s32 averageValue;
    if (battleAvailable) {
        s32 groupChoice = scrReadIntParameter(0);
        averageValue = func_001B3A00(groupChoice ? EVT_ALTERNATE_GROUP_MASK : EVT_DEFAULT_GROUP_MASK);
    }
    else {
        averageValue = 0;
    }
    scrSetIntegerReturnValue(averageValue);
    return 1;
}

/* Divide the numeric group mask, not an actor stat; preserve native unsigned division. */
s32 evtPushAvailableChoiceRatio(void) {
    s32 battleAvailable = btlIsRuntimeAllocated();
    u64 quotient;
    if (battleAvailable) {
        s32 groupChoice = scrReadIntParameter(0);
        s32 divisor = scrReadIntParameter(1);
        s32 selectedMask = groupChoice ? EVT_ALTERNATE_GROUP_MASK : EVT_DEFAULT_GROUP_MASK;
        quotient = (u64)selectedMask / divisor;
    }
    else {
        quotient = 0;
    }
    scrSetIntegerReturnValue(quotient);
    return 1;
}

/* Push the native battle float when a runtime exists, otherwise zero. */
s32 evtPushAvailableFloatValue(void) {
    s32 battleAvailable = btlIsRuntimeAllocated();
    f32 value = 0.0f;

    if (battleAvailable != 0) {
        value = func_001AD978();
    }
    scrSetFloatReturnValue(value);
    return 1;
}

/* Push the native battle integer when a runtime exists, otherwise zero. */
s32 evtPushAvailableIntegerValue(void) {
    s32 battleAvailable = btlIsRuntimeAllocated();
    s32 value = 0;

    if (battleAvailable != 0) {
        value = func_001ADA10();
    }
    scrSetIntegerReturnValue(value);
    return 1;
}


/* The paired DDS1 readers identify the halfword index as the entry's level.
 * The shared coefficient arrays start at level one. */
s32 func_0011DFE0(void) {
    DatPartyRecord *entry = (DatPartyRecord *)D_0043E5C0.first;
    u16 levelIndex = entry->level;
    scrSetFloatReturnValue(datBattleParameters->maxHpGrowth[levelIndex - 1]);
    return 1;
}

s32 func_0011E018(void) {
    DatPartyRecord *entry = (DatPartyRecord *)D_0043E5C0.first;
    u16 levelIndex = entry->level;
    scrSetFloatReturnValue(datBattleParameters->maxMpGrowth[levelIndex - 1]);
    return 1;
}

extern u32 datComputeSkillBoostedMaxHp(DatPartyRecord *);

/* Push the coarse HP-percentage table value; only exactly 100 percent uses index zero. */
s32 evtSelectStatGrade(void) {
    s32 maximumHp = datComputeSkillBoostedMaxHp((DatPartyRecord *)D_0043E5C0.second);
    s32 currentHp = ((DatPartyRecord *)D_0043E5C0.second)->hp;
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
    scrSetFloatReturnValue(datBattleParameters->hpGradeValues[gradeIndex]);
    return 1;
}

s32 func_0011E128(void) {
    DatPartyRecord *entry = (DatPartyRecord *)D_0043E5C0.first;
    u16 levelIndex = entry->level;
    scrSetFloatReturnValue(datBattleParameters->levelValuesA[levelIndex - 1]);
    return 1;
}

s32 func_0011E160(void) {
    DatPartyRecord *entry = (DatPartyRecord *)D_0043E5C0.first;
    u16 levelIndex = entry->level;
    scrSetFloatReturnValue(datBattleParameters->levelValuesB[levelIndex - 1]);
    return 1;
}

s32 func_0011E198(void) {
    DatPartyRecord *entry = (DatPartyRecord *)D_0043E5C0.first;
    u16 levelIndex = entry->level;
    scrSetFloatReturnValue(datBattleParameters->levelValuesC[levelIndex - 1]);
    return 1;
}

s32 func_0011E1D0(void) {
    DatPartyRecord *entry = (DatPartyRecord *)D_0043E5C0.first;
    u16 levelIndex = entry->level;
    scrSetFloatReturnValue(datBattleParameters->levelValuesC[levelIndex - 1]);
    return 1;
}

s32 evtRollFlagDependentResultCode(void) {
    s32 value;
    if ((((DatPartyRecord *)D_0043E5C0.second)->flags & 0x20) == 0) {
        value = effMiscRandMod(0, 0x20) != 0 ? 0xA : 0x80;
    } else {
        value = effMiscRandMod(0, 0x30) != 0 ? 0 : 0x80;
    }
    scrSetIntegerReturnValue(value);
    return 1;
}

s32 evtPushRosterBaseValue(void) {
    u16 index = ((DatPartyRecord *)D_0043E5C0.first)->unitId;
    scrSetIntegerReturnValue(((EventRosterStat *)datRosterDetails)[index].base);
    return 1;
}

/* Push the finer HP-percentage table value; only exactly 100 percent uses index zero. */
s32 evtSelectFineStatGrade(void) {
    s32 maximumHp = datComputeSkillBoostedMaxHp((DatPartyRecord *)D_0043E5C0.second);
    s32 currentHp = ((DatPartyRecord *)D_0043E5C0.second)->hp;
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
    scrSetFloatReturnValue(datBattleParameters->hpFineGradeValues[gradeIndex]);
    return 1;
}

s32 func_0011E3A0(void) {
    DatPartyRecord *unit = (DatPartyRecord *)D_0043E5C0.first;
    u16 level = unit->level;
    s32 grade = datGetStatWithStatusOverride(unit, 4);

    grade -= (s32)datBattleParameters->levelValuesC[level - 1];
    grade /= 2;

    if (grade < -5) {
        grade = -5;
    } else if (grade >= 6) {
        grade = 5;
    }
    scrSetFloatReturnValue(datBattleParameters->gradeScale[grade + 5]);
    return 1;
}

s32 evtPushRosterOrGlobalCounterValue(void) {
    DatPartyRecord *entry = (DatPartyRecord *)D_0043E5C0.first;
    s32 result;
    if (!(entry->flags & 0x20)) {
        result = datGameState->header.currency;
    } else {
        s32 index = entry->unitId;
        result = *(s32 *)((u8 *)&datEnemyRecords[index] + 0x28);
    }
    scrSetIntegerReturnValue(result);
    return 1;
}

s32 evtTestSolarPhaseOrModelFlag(u32 flags) {
    u32 type = flags >> 16;
    switch (type) {
    case 0:
        break;
    case 1:
        if (flags & (1 << evtGetMirroredSolarPhase()) & 0xFFFF) {
            return 1;
        }
        break;
    case 2:
        if (mdlFlagTest(flags & 0xFFFF)) {
            return 1;
        }
        break;
    }
    return 0;
}

/* Three conditional encounter groups follow the record's condition header. */
typedef struct BattleAdjustmentEntry {
    u16 sceneIndex;
    u16 weight;
    s8 value;
    u8 unk05;
} BattleAdjustmentEntry;

typedef struct BattleAdjustmentGroup {
    s32 interval;
    BattleAdjustmentEntry entries[20];
} BattleAdjustmentGroup;

typedef struct BattleAdjustmentRecord {
    u8 pad00[8];
    u32 conditions[3];
    u8 variantCodes[8];
    BattleAdjustmentGroup groups[3];
} BattleAdjustmentRecord;

extern BattleAdjustmentRecord *D_00435E0C;

u8 btlSelectConditionalEncounterGroup(s32 index) {
    s8 enabled[3];
    s32 i;
    u8 code;

    for (i = 0; i < 3; i++) {
        enabled[i] = evtTestSolarPhaseOrModelFlag(D_00435E0C[index].conditions[i]);
    }
    code = D_00435E0C[index].variantCodes[0];
    if (enabled[0] && enabled[1] && enabled[2]) {
        if (code != 8) {
            return code;
        }
    }
    code = D_00435E0C[index].variantCodes[1];
    if (enabled[0] && enabled[1]) {
        if (code != 8) {
            return code;
        }
    }
    code = D_00435E0C[index].variantCodes[2];
    if (enabled[0] && enabled[2]) {
        if (code != 8) {
            return code;
        }
    }
    code = D_00435E0C[index].variantCodes[3];
    if (enabled[1] && enabled[2]) {
        if (code != 8) {
            return code;
        }
    }
    code = D_00435E0C[index].variantCodes[4];
    if (enabled[0]) {
        if (code != 8) {
            return code;
        }
    }
    code = D_00435E0C[index].variantCodes[5];
    if (enabled[1]) {
        if (code != 8) {
            return code;
        }
    }
    code = D_00435E0C[index].variantCodes[6];
    if (enabled[2]) {
        if (code != 8) {
            return code;
        }
    }
    code = D_00435E0C[index].variantCodes[7];
    return code == 8 ? 0 : code;
}

INCLUDE_ASM(const s32, "newdata/datCalc", btlCheckScenePartyLevelThreshold);

/* Reuse an active interval, or seed one for a nonempty encounter group. */
s32 func_0011E848(s32 index) {
    s32 variant;
    s32 total;
    s32 i;
    s32 interval;
    if (index == 0) {
        return 0;
    }
    variant = btlSelectConditionalEncounterGroup(index);
    total = 0;
    for (i = 0; i < 20; i++) {
        total += D_00435E0C[index].groups[variant].entries[i].weight;
    }
    if (total == 0) {
        return 0;
    }
    if (D_00435E8C > 0) {
        return D_00435E8C;
    }
    interval = D_00435E0C[index].groups[variant].interval;
    if (interval <= 0) {
        return 0;
    }
    D_00435E8C = effMiscRandMod(0, interval * 2 - 60) + 30;
    return D_00435E8C;
}

extern s32 D_00435E90;
extern s32 D_00435E94;
extern s32 btlCheckScenePartyLevelThreshold(u32 scene);
extern void dds3WorkClear(void);

s32 func_0011E930(s32 index, f32 step) {
    s32 group;
    s32 interval;
    s32 count;
    s32 i;
    s32 total;
    s32 roll;
    u32 scene;
    s32 packed;
    f32 counter;

    if (index == 0 || step == 0.0f) {
        return 0;
    }
    group = btlSelectConditionalEncounterGroup(index);
    interval = func_0011E848(index);
    if (interval == 0) {
        return interval;
    }
    counter = datGameState->unk1440 + step;
    datGameState->unk1440 = counter;
    if (counter < 100.0f) {
        return 0;
    }
    count = (s32)(counter / 100.0f);
    datGameState->unk1440 = counter - (f32)count * 100.0f;
    for (i = 0; i < count; i++) {
        if (datGameState->world.fieldFlags & 2) {
            datGameState->unk1444 = datGameState->unk1444 + effMiscRandMod(0, 7) + 6;
        } else {
            datGameState->unk1444 = datGameState->unk1444 + effMiscRandMod(0, 3) + 2;
        }
    }
    if (datGameState->unk1444 > 0xFDE8) {
        datGameState->unk1444 = -0x218;
    }
    if (datGameState->unk1444 < interval) {
        return 0;
    }
    total = 0;
    for (i = 0; i < 20; i++) {
        if (D_00435E0C[index].groups[group].entries[i].sceneIndex != 0) {
            total += D_00435E0C[index].groups[group].entries[i].weight;
        }
    }
    count = 0;
    roll = effMiscRandMod(0, total);
    for (i = 0; i < 20; i++) {
        if (D_00435E0C[index].groups[group].entries[i].sceneIndex != 0) {
            if (roll < count + D_00435E0C[index].groups[group].entries[i].weight) {
                scene = D_00435E0C[index].groups[group].entries[i].sceneIndex;
                if ((datGameState->world.fieldFlags & 1) == 0 || btlCheckScenePartyLevelThreshold(scene) != 0) {
                    packed = (group << 24) | (i << 16) | scene;
                    D_00435E90 = index;
                    D_00435E8C = 0;
                    D_00435E94 = packed;
                    return packed;
                }
                dds3WorkClear();
                return 0;
            }
            count += D_00435E0C[index].groups[group].entries[i].weight;
        }
    }
    return 0;
}

void dds3WorkClear(void) {
    DatGameState *state = datGameState;

    state->unk1440 = 0;
    state->unk1444 = 0;
    D_00435E8C = 0;
}

void func_0011EBE0(void) {
}

void func_0011EBE8(void) {
}

void func_0011EBF0(void) {
}

void func_0011EBF8(s32 chance) {
}

/* Mode zero queries presence; nonzero mode selects/moves the entry into the
 * frontline group. The latter is not a pure presence test: its retail
 * callee changes flag mask 2 and swaps roster records. Preserve the short-arity
 * presence call and push whether the operation returned exactly one. */
s32 func_0011EC00(void) {
    s32 rosterIndex = scrReadIntParameter(0);
    s32 result;
    if (scrReadIntParameter(1) == 0) {
        result = ptyIsRosterEntryPresent();
    } else {
        result = ptyRebalanceFrontline(rosterIndex);
    }
    scrSetIntegerReturnValue(result == 1);
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

INCLUDE_SDATA(const s32, "newdata/datCalc", datBattleSceneRecords);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435E08);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435E0C);

INCLUDE_SDATA(const s32, "newdata/datCalc", fldEncounterRollTable);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435E14);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435E18);

INCLUDE_SDATA(const s32, "newdata/datCalc", datCommandSelectors);

INCLUDE_SDATA(const s32, "newdata/datCalc", datCommandRecords);

INCLUDE_SDATA(const s32, "newdata/datCalc", datAffinityRecords);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435E28);

INCLUDE_SDATA(const s32, "newdata/datCalc", datAbilityParameters);

INCLUDE_SDATA(const s32, "newdata/datCalc", datActionAnimationRecords);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435E34);

INCLUDE_SDATA(const s32, "newdata/datCalc", datItemSkillRecords);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435E3C);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435E40);

INCLUDE_SDATA(const s32, "newdata/datCalc", datBattleParameters);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435E48);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435E4C);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435E50);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435E54);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435E58);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435E5C);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435E60);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435E64);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435E68);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435E6C);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435E70);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435E74);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435E78);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435E7C);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435E80);

INCLUDE_SDATA(const s32, "newdata/datCalc", evtWorkScriptTask);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435E8C);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435E90);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435E94);

