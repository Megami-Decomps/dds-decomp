#ifndef FR_FONT_MEASURE_H
#define FR_FONT_MEASURE_H

#include "common.h"

struct frFontHandle_t;

/* Glyph measurement reads borrowed chains and never changes their nodes. */
s32 frGetStrWidth(const struct frFontHandle_t *parentGlyph);
s32 frGetStrWidth2(const struct frFontHandle_t *glyphChain);
s32 frFontMeasureLineWidth(s32 line, const struct frFontHandle_t *glyph);
s32 frFontMeasureGlyphStripWidth(s32 line, const struct frFontHandle_t *glyph);

#endif /* FR_FONT_MEASURE_H */
