#include "common.h"
#include "fpu.h"
#include "pcp_vu0.h"

extern s32 frFontMeasureTextAdvance(void *, s32, s32);

s32 sdfCounterMeasureLabelWidth(u32 arg0) {
    return frFontMeasureTextAdvance((void *)arg0, 1, 3);
}
