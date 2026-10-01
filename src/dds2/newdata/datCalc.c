#include "common.h"

/* Two clamped cursor coordinates, each followed by its maximum. */
typedef struct DatCalcCursor {
    u8 unk0[6];
    u16 x;
    u16 xMax;
    u16 y;
    u16 yMax;
} DatCalcCursor;

extern s32 D_00435DD0;

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

INCLUDE_ASM(const s32, "newdata/datCalc", func_001197C0);

INCLUDE_ASM(const s32, "newdata/datCalc", func_001198C0);

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

INCLUDE_ASM(const s32, "newdata/datCalc", func_00119A78);

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

extern DatPartyMember *D_00435DEC;

/* Nonzero if `skill` is in the unit's skill list (party members use the party table). */
s32 func_00119AF8(DatSkillOwner *unit, s32 skill) {
    s32 i;

    if (!(unit->flags & 0x20)) {
        for (i = 0; i < 0x18; i++) {
            if (unit->skills[i] == skill) {
                return 1;
            }
        }
    } else {
        for (i = 0; i < 8; i++) {
            if (D_00435DEC[unit->partyIndex].skills[i] == skill) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "newdata/datCalc", func_00119BA0);

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
    s32 value = ((DatGameCounters *)D_00435DD0)->currency + delta;
    if (value < 0) {
        value = 0;
    }
    if (value > 0x98967F) {
        value = 0x98967F;
    }
    ((DatGameCounters *)D_00435DD0)->currency = value;
    return value;
}

s32 datHasEnoughCurrency(s32 value) {
    if (*(s32 *)(D_00435DD0 + 0x3C) < value) {
        return 0;
    }
    return 1;
}

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435DD8);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435DDC);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435DE0);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435DE4);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435DE8);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435DEC);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435DF0);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435DF4);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435DF8);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435DFC);

INCLUDE_SDATA(const s32, "newdata/datCalc", D_00435E00);

