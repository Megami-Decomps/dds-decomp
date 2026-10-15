#include "common.h"
#include "fpu.h"
#include "pcp_vu0.h"

extern s32 frFontMeasureTextAdvance(void *, s32, s32);

s32 sdfCounterMeasureLabelWidth(u32 labelText) {
    return frFontMeasureTextAdvance((void *)labelText, 1, 3);
}
