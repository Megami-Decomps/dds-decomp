#ifndef FR_FONT_MEASURE_H
#define FR_FONT_MEASURE_H

#include "common.h"

struct FrFontGlyph;

u32 frFontMeasureGlyphChain(struct FrFontGlyph *parentGlyph);
u32 frFontMeasureLines(struct FrFontGlyph *glyphChain);

#endif /* FR_FONT_MEASURE_H */
