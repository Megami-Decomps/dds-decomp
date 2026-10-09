#ifndef FR_FONT_CONTEXT_H
#define FR_FONT_CONTEXT_H

#include "common.h"

struct FrFontGlyph;

void frFontSetContextEncodedByte(struct FrFontGlyph *glyph, s32 inputValue);
void frFontEnableContextMode(struct FrFontGlyph *glyph);
void frFontSetSpacingAndMeasureGlyphs(struct FrFontGlyph *glyph, s32 spacing);
void frFontSetGlyphPosition(struct FrFontGlyph *glyph, u32 x, u32 y);
void frFontStoreShiftedRenderValue(struct FrFontGlyph *glyph, u32 unshiftedValue);

#endif
