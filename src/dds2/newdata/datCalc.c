#include "common.h"

/* Two clamped cursor coordinates, each followed by its maximum. */
typedef struct DatCalcCursor {
    u8 unk0[6];
    u16 x;
    u16 xMax;
    u16 y;
    u16 yMax;
} DatCalcCursor;

INCLUDE_ASM(const s32, "newdata/datCalc", func_001197A8);

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

INCLUDE_ASM(const s32, "newdata/datCalc", func_00119AF8);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00119BA0);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00119C78);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00119F20);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00119F40);

INCLUDE_ASM(const s32, "newdata/datCalc", func_00119F68);

INCLUDE_ASM(const s32, "newdata/datCalc", func_0011A098);

INCLUDE_ASM(const s32, "newdata/datCalc", func_0011A0D0);

INCLUDE_ASM(const s32, "newdata/datCalc", func_0011A100);

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

