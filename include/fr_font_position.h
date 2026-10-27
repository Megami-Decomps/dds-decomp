#ifndef FR_FONT_POSITION_H
#define FR_FONT_POSITION_H

#include "common.h"

/* Output point written by glyph-chain position lookup. */
typedef struct FrFontPosition {
    s32 x;
    s32 y;
} FrFontPosition;

struct _FRQ;

/* Write a position from a borrowed glyph chain without changing the chain. */
void frFontGetGlyphPositionAtIndex(FrFontPosition *position, s32 index,
                                   const struct _FRQ *glyph);

typedef char FrFontPosition_size_must_be_0x08[
    (sizeof(FrFontPosition) == 0x08) ? 1 : -1];
typedef char FrFontPosition_y_offset_must_be_0x04[
    ((u32)&((FrFontPosition *)0)->y == 0x04) ? 1 : -1];

#endif /* FR_FONT_POSITION_H */
