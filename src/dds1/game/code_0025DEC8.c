#include "common.h"
#include "mnu_list.h"
#include "mnu_shop.h"

extern s32 D_003BAA9C;

struct FrFontGlyph;
struct TextStyleNode;

extern s32 itfDrawBankTextWithLayoutFlags(s32, s32, s32, u16, s32, s32);
extern void frFontSetChildColors(struct TextStyleNode *, u32);
extern s32 func_001958A0(struct FrFontGlyph *, s8, u32);
extern s32 frFontQueueGlyphInSelectedSlot(struct FrFontGlyph *);

void itfEmitSelectedGlyph(ShopScene *context, s32 unused, s32 layoutFlags,
                   u32 color, u32 glyphAttribute) {
    struct MenuList *list;
    struct FrFontGlyph *glyph;

    list = context->window->list;
    if (list->count != 0) {
        glyph = (struct FrFontGlyph *)itfDrawBankTextWithLayoutFlags(0x970, 0xb58, 1, list->cursor->camp.id, D_003BAA9C,
                              layoutFlags);
        frFontSetChildColors((struct TextStyleNode *)glyph, color);
        func_001958A0(glyph, 1, glyphAttribute);
        frFontQueueGlyphInSelectedSlot(glyph);
        return;
    }
}


