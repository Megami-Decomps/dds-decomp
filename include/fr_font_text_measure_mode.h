#ifndef FR_FONT_TEXT_MEASURE_MODE_H
#define FR_FONT_TEXT_MEASURE_MODE_H

#include "common.h"

/* The mode bit that halves ASCII advances for non-default font slots. */
typedef u8 FrFontTextMeasureMode;
enum {
    FR_FONT_TEXT_MEASURE_HALF_ASCII_WIDTH = 0x02
};

s32 frFontMeasureTextAdvance(const u8 *text, u8 fontIndex, FrFontTextMeasureMode mode);

#endif /* FR_FONT_TEXT_MEASURE_MODE_H */
