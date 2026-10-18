#ifndef EVT_BLEND_VEC_COLOR_H
#define EVT_BLEND_VEC_COLOR_H

#include "common.h"

typedef struct EvtBlendVecColor {
    f32 vec[4];
    u32 color;
} EvtBlendVecColor;

typedef char EvtBlendVecColorSizeCheck[(sizeof(EvtBlendVecColor) == 0x14) ? 1 : -1];
typedef char EvtBlendVecColorVecOffsetCheck[((u32)&((EvtBlendVecColor *)0)->vec == 0x00) ? 1 : -1];
typedef char EvtBlendVecColorColorOffsetCheck[((u32)&((EvtBlendVecColor *)0)->color == 0x10) ? 1 : -1];

void evtBlendParamsC(s32 enable, f32 t, EvtBlendVecColor *a, EvtBlendVecColor *b, EvtBlendVecColor *out);

#endif
