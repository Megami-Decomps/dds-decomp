#ifndef FR_FONT_MEASURE_H
#define FR_FONT_MEASURE_H

#include "common.h"

struct FrFontGlyph;

u32 frGetStrWidth(struct FrFontGlyph *parentGlyph);
u32 frGetStrWidth2(struct FrFontGlyph *glyphChain);
u32 frFontMeasureLineWidth(s32 line, struct FrFontGlyph *glyph);

#endif /* FR_FONT_MEASURE_H */
