#include "common.h"
#include "fpu.h"
#include "pcp_vu0.h"
#include "fr_font_text_measure_mode.h"

s32 sdfCounterMeasureLabelWidth(const u8 *labelText) {
    return frFontMeasureTextAdvance(labelText, 1, 3);
}
