#ifndef FR_FONT_CONTEXT_H
#define FR_FONT_CONTEXT_H

#include "common.h"

struct _FRQ;

void frSetDrawSpeed(struct _FRQ *glyph, s32 inputValue);
void frFontEnableContextMode(struct _FRQ *glyph);
void frSetFontSpace(struct _FRQ *glyph, s32 spacing);
void frSetDrawPosition(struct _FRQ *glyph, s32 x, s32 y);
void frSetPriority(struct _FRQ *glyph, u32 unshiftedValue);

#endif
