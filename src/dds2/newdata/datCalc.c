#include "common.h"

typedef struct DatSkillOwner {
    u16 flags;        /* 0x00: 0x20 = skills live in the party table */
    u16 unk2;
    u16 partyIndex;   /* 0x04 */
    u8 unk6[0x1C];
    u16 skills[0x18]; /* 0x22 */
} DatSkillOwner;

extern s32 func_001188F0(s32 unit);
extern s32 func_001189D0(s32 unit);
extern s32 datUnitHasSkill(struct DatSkillOwner *unit, s32 skill);

/* Two clamped cursor coordinates, each followed by its maximum. */
typedef struct DatCalcCursor {
    u8 unk0[6];
    u16 x;
    u16 xMax;
    u16 y;
    u16 yMax;
} DatCalcCursor;

extern s32 datGameState;

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

typedef struct DatGameCounters {
    u8 pad00[0x3C];
    s32 currency;        /* 0x3C: clamped to 0..9,999,999 */
} DatGameCounters;

void datClearUnitStatusBits(u8 *work, s32 mask) {
    *(u16 *)(work + 0xE) &= ~mask;
}

u32 func_001197C0(DatSkillOwner *unit) {
    u32 bonus = 0;
    u32 value = func_001188F0((s32)unit);

    if (datUnitHasSkill(unit, 0x220)) {
        bonus = value * 10 / 100;
    }
    if (datUnitHasSkill(unit, 0x221)) {
        bonus += value * 20 / 100;
    }
    if (datUnitHasSkill(unit, 0x222)) {
        bonus += value * 30 / 100;
    }
    value += bonus;
    if (!(unit->flags & 0x20) && value >= 1000) {
        value = 999;
    }
    return value;
}

u32 func_001198C0(DatSkillOwner *unit) {
    u32 bonus = 0;
    u32 value = func_001189D0((s32)unit);

    if (datUnitHasSkill(unit, 0x223)) {
        bonus = value * 10 / 100;
    }
    if (datUnitHasSkill(unit, 0x224)) {
        bonus += value * 20 / 100;
    }
    if (datUnitHasSkill(unit, 0x225)) {
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

INCLUDE_ASM(const s32, "newdata/datCalc", func_00119A10);
extern s32 func_00119A10(u8 *unit);
extern s32 ptyGetCombinedRecordAndSlotValue(s32 id, s32 slot);

s32 func_00119A78(u8 *unit, s32 slot) {
    s32 value;

    if ((*(u16 *)(unit + 0xE) & 0x7FFF) == 0x1000) {
        return 1;
    }
    value = func_00119A10(unit);
    value += ptyGetCombinedRecordAndSlotValue(*(u16 *)(unit + 0x1B2), slot);
    if (value < 0) {
        value = 0;
    }
    if (value >= 100) {
        value = 99;
    }
    return value;
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

typedef struct DatUnitStatus {
    u8 pad00[0xE];
    u16 status; /* 0x0E */
} DatUnitStatus;

/* Raise the low half of `value` to a per-status minimum (0x12C, 0x96, 0xC8; status 4 forces 1) unless a bit in 0x70000 is set. */
s32 func_00119BA0(DatUnitStatus *unit, s32 value) {
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

INCLUDE_ASM(const s32, "newdata/datCalc", func_00119C78);

u32 datReadLowHalfOfCalculatedValue(void) {
    return (u16)func_00119C78();
}

u32 datReadHighHalfOfCalculatedValue(void) {
    return func_00119C78() & 0xFFFF0000;
}

INCLUDE_ASM(const s32, "newdata/datCalc", func_00119F68);

s32 datIsValueBelowQuarterMax(UiObject *object) {
    return *(u16 *)((u8 *)object + 6) * 100 / *(u16 *)((u8 *)object + 8) < 25;
}

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

