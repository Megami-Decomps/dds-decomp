#include "common.h"

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

INCLUDE_ASM(const s32, "newdata/datCalc", func_001190B0);

INCLUDE_ASM(const s32, "newdata/datCalc", func_001191B0);

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

INCLUDE_ASM(const s32, "newdata/datCalc", func_00119300);

extern s32 func_00119300(u8 *, s32);

typedef struct DatUnitStatus {
    u8 pad00[0xE];
    u16 status;          /* 0x0E */
} DatUnitStatus;

/* Units with the 0x1000 status bypass the normal stat eligibility test. */
s32 func_00119368(u8 *unit, s32 statIndex) {
    if ((((DatUnitStatus *)unit)->status & 0x7FFF) == 0x1000) {
        return 1;
    }
    return func_00119300(unit, statIndex);
}

typedef struct DatSkillOwner {
    u16 flags;        /* 0x00: 0x20 = skills live in the party table */
    u16 unk2;
    u16 partyIndex;   /* 0x04 */
    u8 unk6[0x1C];
    u16 skills[0x18]; /* 0x22 */
} DatSkillOwner;

typedef struct DatPartyMember {
    u8 unk0[0x18];
    u16 skills[8]; /* 0x18 */
    u8 unk28[0x24];
} DatPartyMember; /* 0x4C */

extern DatPartyMember *D_003BAA1C;

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
            if (D_003BAA1C[unit->partyIndex].skills[i] == skill) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "newdata/datCalc", func_00119448);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00119520);

u32 datReadLowHalfOfCalculatedValue(void) {
    return (u16)func_00119520();
}

u32 datReadHighHalfOfCalculatedValue(void) {
    return func_00119520() & 0xFFFF0000;
}

INCLUDE_ASM(const s32, "newdata/datCalc", func_00119750);

s32 datIsValueBelowQuarterMax(UiObject *object) {
    return *(u16 *)((u8 *)object + 6) * 100 / *(u16 *)((u8 *)object + 8) < 25;
}

extern s32 D_003BAA00;

typedef struct DatGameCounters {
    u8 pad00[0x3C];
    s32 currency;        /* 0x3C: clamped to 0..9,999,999 */
} DatGameCounters;

/* Add to the party's currency counter, saturating at either bound. */
s32 datAddCurrencyClamped(s32 delta) {
    s32 value = ((DatGameCounters *)D_003BAA00)->currency + delta;
    if (value < 0) {
        value = 0;
    }
    if (value > 0x98967F) {
        value = 0x98967F;
    }
    ((DatGameCounters *)D_003BAA00)->currency = value;
    return value;
}

s32 datHasEnoughCurrency(s32 value) {
    if (*(s32 *)(D_003BAA00 + 0x3C) < value) {
        return 0;
    }
    return 1;
}

INCLUDE_SDATA(const s32, "newdata/datCalc", D_003BAA08);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_003BAA0C);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_003BAA10);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_003BAA14);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_003BAA18);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_003BAA1C);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_003BAA20);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_003BAA24);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_003BAA28);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_003BAA2C);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_003BAA30);

