#ifndef FR_FONT_POSITION_H
#define FR_FONT_POSITION_H

#include "common.h"

/* Output point written by glyph-chain position lookup. */
typedef struct FrFontPosition {
    s32 x;
    s32 y;
} FrFontPosition;

typedef char FrFontPosition_size_must_be_0x08[
    (sizeof(FrFontPosition) == 0x08) ? 1 : -1];
typedef char FrFontPosition_y_offset_must_be_0x04[
    ((u32)&((FrFontPosition *)0)->y == 0x04) ? 1 : -1];

#endif /* FR_FONT_POSITION_H */
