#ifndef FR_FONT_CONTEXT_H
#define FR_FONT_CONTEXT_H

#include "common.h"

struct frFontHandle_t;

void frSetDrawSpeed(struct frFontHandle_t *glyph, s32 inputValue);
void frFontEnableContextMode(struct frFontHandle_t *glyph);
void frSetFontSpace(struct frFontHandle_t *glyph, s32 spacing);
void frSetDrawPosition(struct frFontHandle_t *glyph, s32 x, s32 y);
void frSetPriority(struct frFontHandle_t *glyph, u32 unshiftedValue);

#endif
