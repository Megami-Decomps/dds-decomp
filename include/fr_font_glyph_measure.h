#ifndef FR_FONT_GLYPH_MEASURE_H
#define FR_FONT_GLYPH_MEASURE_H

#include "common.h"

typedef struct FrFontGlyphMeasureWork {
    u16 code;           /* 0x00 */
    u8 opaque02[2];
    s32 x;              /* 0x04 */
    s32 y;              /* 0x08 */
    s32 advance;        /* 0x0C */
    u8 opaque10[5];
    u8 fontIndex;       /* 0x15 */
    u8 opaque16;
    u8 mode;            /* 0x17 */
    u8 cellWidth;       /* 0x18 */
    u8 cellHeight;      /* 0x19 */
    u8 opaque1A[0x16];
} FrFontGlyphMeasureWork;

typedef char FrFontGlyphMeasureWork_size_must_be_0x30[
    (sizeof(FrFontGlyphMeasureWork) == 0x30) ? 1 : -1];
typedef char FrFontGlyphMeasureWork_fontIndex_at_0x15[
    ((u32)&((FrFontGlyphMeasureWork *)0)->fontIndex == 0x15) ? 1 : -1];
typedef char FrFontGlyphMeasureWork_mode_at_0x17[
    ((u32)&((FrFontGlyphMeasureWork *)0)->mode == 0x17) ? 1 : -1];
typedef char FrFontGlyphMeasureWork_cellWidth_at_0x18[
    ((u32)&((FrFontGlyphMeasureWork *)0)->cellWidth == 0x18) ? 1 : -1];

#endif
