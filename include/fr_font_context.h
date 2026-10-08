#ifndef FR_FONT_CONTEXT_H
#define FR_FONT_CONTEXT_H

#include "common.h"

struct FrFontGlyph;

void frFontSetContextEncodedByte(struct FrFontGlyph *glyph, s32 inputValue);
void frFontEnableContextMode(struct FrFontGlyph *glyph);
void frFontSetFlagAndMeasureGlyphs(struct FrFontGlyph *glyph, s32 requestedFlag);
void frFontSetContextPair(struct FrFontGlyph *glyph, u32 first, u32 second);
void frFontStoreShiftedContextValue(struct FrFontGlyph *glyph, u32 unshiftedValue);

#endif
