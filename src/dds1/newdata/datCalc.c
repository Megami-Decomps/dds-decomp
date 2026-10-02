#include "common.h"

typedef struct DatSkillOwner {
    u16 flags;        /* 0x00: 0x20 = skills live in the party table */
    u16 unk2;
    u16 partyIndex;   /* 0x04 */
    u8 unk6[0x1C];
    u16 skills[0x18]; /* 0x22 */
} DatSkillOwner;

extern s32 ptyComputeMaxHp(s32 unit);
extern s32 ptyComputeMaxMp(s32 unit);
extern s32 datUnitHasSkill(struct DatSkillOwner *unit, s32 skill);

/* 2D clamped position (e.g. a cursor): each axis keeps a value and its max. */
typedef struct DatCalcCursor {
    u8 unk0[6]; /* 0x0 */
    u16 x;      /* 0x6 */
    u16 xMax;   /* 0x8 */
    u16 y;      /* 0xA */
    u16 yMax;   /* 0xC */
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

void datClearUnitStatusBits(u8 *work, s32 mask) {
    *(u16 *)(work + 0xE) &= ~mask;
}

u32 datComputeSkillBoostedMaxHp(DatSkillOwner *unit) {
    u32 bonus = 0;
    u32 value = ptyComputeMaxHp((s32)unit);

    if (datUnitHasSkill(unit, 0x200)) {
        bonus = value * 10 / 100;
    }
    if (datUnitHasSkill(unit, 0x201)) {
        bonus += value * 20 / 100;
    }
    if (datUnitHasSkill(unit, 0x202)) {
        bonus += value * 30 / 100;
    }
    value += bonus;
    if (!(unit->flags & 0x20) && value >= 1000) {
        value = 999;
    }
    return value;
}

u32 datComputeSkillBoostedMaxMp(DatSkillOwner *unit) {
    u32 bonus = 0;
    u32 value = ptyComputeMaxMp((s32)unit);

    if (datUnitHasSkill(unit, 0x203)) {
        bonus = value * 10 / 100;
    }
    if (datUnitHasSkill(unit, 0x204)) {
        bonus += value * 20 / 100;
    }
    if (datUnitHasSkill(unit, 0x205)) {
        bonus += value * 30 / 100;
    }
    value += bonus;
    if (!(unit->flags & 0x20) && value >= 1000) {
        value = 999;
    }
    return value;
}

void datMoveCursorX(DatCalcCursor *cursor, s32 delta) {
    u32 value;

    value = (u32)cursor->x + delta;
    if ((s32)value < 0) {
        value = 0;
    }
    if ((s32)cursor->xMax < (s32)value) {
        value = cursor->xMax;
    }
    cursor->x = (s16)value;
}

void datMoveCursorY(DatCalcCursor *cursor, s32 delta) {
    u32 value;

    value = (u32)cursor->y + delta;
    if ((s32)value < 0) {
        value = 0;
    }
    if ((s32)cursor->yMax < (s32)value) {
        value = cursor->yMax;
    }
    cursor->y = (s16)value;
}

extern s8 ptyGetCurrentProfileId(u8 *unit);
extern s32 func_002CDDB0(u16 profileId, s32 statIndex);

typedef struct DatUnitStatus {
    u8 pad00[0xE];
    u16 status;          /* 0x0E */
    u8 pad10[6];
    s8 statValues[0x100];
} DatUnitStatus;

s32 func_00119300(DatUnitStatus *unit, s32 statIndex) {
    s32 value = unit->statValues[statIndex] +
                func_002CDDB0((u16)ptyGetCurrentProfileId((u8 *)unit), statIndex);

    if (value <= 0) {
        value = 1;
    }
    if (value >= 100) value = 99;
    return value;
}

/* Units with the 0x1000 status bypass the normal stat eligibility test. */
s32 func_00119368(DatUnitStatus *unit, s32 statIndex) {
    if ((unit->status & 0x7FFF) == 0x1000) {
        return 1;
    }
    return func_00119300(unit, statIndex);
}


typedef struct DatPartyMember {
    u8 unk0[0x18];
    u16 skills[8]; /* 0x18 */
    u8 unk28[0x24];
} DatPartyMember; /* 0x4C */

extern DatPartyMember *datEnemyRecords;

/* Nonzero if `skill` is in the unit's skill list (party members use the party table). */
s32 datUnitHasSkill(DatSkillOwner *unit, s32 skill) {
    s32 i;

    if (!(unit->flags & 0x20)) {
        for (i = 0; i < 0x18; i++) {
            if (unit->skills[i] == skill) {
                return 1;
            }
        }
    } else {
        for (i = 0; i < 8; i++) {
            if (datEnemyRecords[unit->partyIndex].skills[i] == skill) {
                return 1;
            }
        }
    }
    return 0;
}

/* Raise the low half of `value` to a per-status minimum (0x12C, 0x96, 0xC8; status 4 forces 1) unless a bit in 0x70000 is set. */
s32 datAdjustCalculatedValueForStatus(DatUnitStatus *unit, s32 value) {
    if ((value & 0x70000) == 0) {
        switch (unit->status & 0x7FFF) {
        case 8:
        case 0x100:
            if ((u16)value < 0xC8) {
                value = (value & 0xFFFF0000) | 0xC8;
            }
            break;
        case 4:
            value = (value & 0xFFFF0000) | 1;
            break;
        case 2:
            if ((u16)value < 0x96) {
                value = (value & 0xFFFF0000) | 0x96;
            }
            break;
        case 1:
            if ((u16)value < 0x12C) {
                value = (value & 0xFFFF0000) | 0x12C;
            }
            break;
        }
    }
    return value;
}

INCLUDE_ASM(const s32, "newdata/datCalc", func_00119520);

u32 datReadLowHalfOfCalculatedValue(void) {
    return (u16)func_00119520();
}

u32 datReadHighHalfOfCalculatedValue(void) {
    return func_00119520() & 0xFFFF0000;
}

s32 datMapFlagToStatIndex(s32 flag) {
    s32 result = -1;

    switch (flag) {
    case 1:
        result = -1;
        break;
    case 2:
        result = 4;
        break;
    case 4:
        result = 3;
        break;
    case 8:
        result = 14;
        break;
    case 0x10:
        result = 12;
        break;
    case 0x20:
        result = 13;
        break;
    case 0x40:
        result = 7;
        break;
    case 0x80:
        result = 11;
        break;
    case 0x100:
        result = 14;
        break;
    case 0x200:
        result = 10;
        break;
    case 0x400:
        result = 9;
        break;
    case 0x800:
        result = 9;
        break;
    case 0x1000:
        result = 7;
        break;
    case 0x2000:
        result = 7;
        break;
    case 0x4000:
        result = 9;
        break;
    }
    return result;
}

s32 datIsValueBelowQuarterMax(UiObject *object) {
    return *(u16 *)((u8 *)object + 6) * 100 / *(u16 *)((u8 *)object + 8) < 25;
}

extern s32 datGameState;

typedef struct DatGameCounters {
    u8 pad00[0x3C];
    s32 currency;        /* 0x3C: clamped to 0..9,999,999 */
} DatGameCounters;

/* Add to the party's currency counter, saturating at either bound. */
s32 datAddCurrencyClamped(s32 delta) {
    s32 value = ((DatGameCounters *)datGameState)->currency + delta;
    if (value < 0) {
        value = 0;
    }
    if (value > 0x98967F) {
        value = 0x98967F;
    }
    ((DatGameCounters *)datGameState)->currency = value;
    return value;
}

s32 datHasEnoughCurrency(s32 value) {
    if (*(s32 *)(datGameState + 0x3C) < value) {
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

