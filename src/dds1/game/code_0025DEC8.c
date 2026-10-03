#include "common.h"

extern u32 D_003BAA9C;

extern u64 itfDrawBankTextWithLayoutFlags(u64, u64, u64, u16, u32, u64);

typedef struct ItfGlyphData {
    u8 pad00[0x64];
    u16 code; /* 0x64 */
} ItfGlyphData;

typedef struct ItfGlyphEntry {
    u8 pad00[0x1C];
    ItfGlyphData *data; /* 0x1C */
    s32 active; /* 0x20 */
} ItfGlyphEntry;

typedef struct ItfGlyphList {
    u8 pad00[0x14];
    ItfGlyphEntry *selected; /* 0x14 */
} ItfGlyphList;

typedef struct ItfGlyphDisplayContext {
    u8 pad00[0x70];
    ItfGlyphList *glyphList; /* 0x70 */
} ItfGlyphDisplayContext;

void itfEmitSelectedGlyph(ItfGlyphDisplayContext *context, u64 unused, u64 parentGlyph,
                   u64 color, u64 glyphAttribute) {
    ItfGlyphEntry *entry;
    u64 glyph;

    entry = context->glyphList->selected;
    if (entry->active != 0) {
        glyph = itfDrawBankTextWithLayoutFlags(0x970, 0xb58, 1, entry->data->code, D_003BAA9C,
                              parentGlyph);
        frFontSetChildColors(glyph, color);
        func_001958A0(glyph, 1, glyphAttribute);
        frFontQueueGlyphInSelectedSlot(glyph);
        return;
    }
}


