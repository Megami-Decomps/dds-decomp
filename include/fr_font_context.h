#ifndef FR_FONT_CONTEXT_H
#define FR_FONT_CONTEXT_H

#include "common.h"

struct FrFontGlyph;

void frSetDrawSpeed(struct FrFontGlyph *glyph, s32 inputValue);
void frFontEnableContextMode(struct FrFontGlyph *glyph);
void frSetFontSpace(struct FrFontGlyph *glyph, s32 spacing);
void frSetDrawPosition(struct FrFontGlyph *glyph, u32 x, u32 y);
void frSetPriority(struct FrFontGlyph *glyph, u32 unshiftedValue);

#endif
