#include "common.h"
#include "fpu.h"
#include "pcp_vu0.h"

extern s32 frFontMeasureTextAdvance(const u8 *text, u8 fontIndex, u8 mode);

s32 sdfCounterMeasureLabelWidth(const u8 *labelText) {
    return frFontMeasureTextAdvance(labelText, 1, 3);
}
