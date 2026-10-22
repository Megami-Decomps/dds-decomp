#ifndef FR_FONT_MEASURE_H
#define FR_FONT_MEASURE_H

#include "common.h"

struct _FRQ;

u32 frGetStrWidth(struct _FRQ *parentGlyph);
u32 frGetStrWidth2(struct _FRQ *glyphChain);
u32 frFontMeasureLineWidth(s32 line, struct _FRQ *glyph);

#endif /* FR_FONT_MEASURE_H */
