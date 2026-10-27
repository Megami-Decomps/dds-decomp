#ifndef FR_FONT_MEASURE_H
#define FR_FONT_MEASURE_H

#include "common.h"

struct _FRQ;

/* Glyph measurement reads borrowed chains and never changes their nodes. */
s32 frGetStrWidth(const struct _FRQ *parentGlyph);
s32 frGetStrWidth2(const struct _FRQ *glyphChain);
s32 frFontMeasureLineWidth(s32 line, const struct _FRQ *glyph);
s32 frFontMeasureGlyphStripWidth(s32 line, const struct _FRQ *glyph);

#endif /* FR_FONT_MEASURE_H */
