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

INCLUDE_ASM(const s32, "newdata/datCalc", func_00119098);

INCLUDE_ASM(const s32, "newdata/datCalc", func_001190B0);

INCLUDE_ASM(const s32, "newdata/datCalc", func_001191B0);

void func_001192B0(DatCalcCursor *cursor, s32 delta) {
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

void func_001192D8(DatCalcCursor *cursor, s32 delta) {
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

INCLUDE_ASM(const s32, "newdata/datCalc", func_00119368);

INCLUDE_ASM(const s32, "newdata/datCalc", func_001193A0);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00119448);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00119520);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00119708);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00119728);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00119750);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00119880);

INCLUDE_ASM(const s32, "newdata/datCalc", func_001198B8);

INCLUDE_ASM(const s32, "newdata/datCalc", func_001198E8);

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

