#include "common.h"
#include "mnu_list.h"
#include "mnu_shop.h"
#include "itf.h"

extern s32 D_003BAA9C;

extern FrFontGlyph *itfDrawBankTextWithLayoutFlags(s32, s32, s32, u16, FrFontTextBank *, s32);
extern s32 frFontDrawGlyphChain(FrFontGlyph *, s8, u32);

void itfEmitSelectedGlyph(ShopScene *context, s32 unused, s32 layoutFlags,
                   u32 color, u32 glyphAttribute) {
    struct MenuList *list;
    FrFontGlyph *glyph;

    list = context->window->list;
    if (list->count != 0) {
        glyph = itfDrawBankTextWithLayoutFlags(0x970, 0xb58, 1, list->cursor->camp.id, (FrFontTextBank *)D_003BAA9C,
                              layoutFlags);
        frFontSetChildColors(glyph, color);
        frFontDrawGlyphChain(glyph, 1, glyphAttribute);
        frFontQueueGlyphForCurrentDrawBuffer(glyph);
        return;
    }
}


