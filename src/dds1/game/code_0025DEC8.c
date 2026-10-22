#include "common.h"
#include "mnu_list.h"
#include "mnu_shop.h"
#include "itf.h"
#include "fr_font.h"
#include "fr_font_draw_glyph.h"

extern u8 *datItemMessages;

extern _FRQ *itfDrawBankTextWithLayoutFlags(s32, s32, s32, u16, FrFontTextBank *, s32);

void itfEmitSelectedGlyph(ShopScene *context, s32 unused, s32 layoutFlags,
                   u32 color, u32 glyphAttribute) {
    struct MenuList *list;
    _FRQ *glyph;

    list = context->window->list;
    if (list->count != 0) {
        glyph = itfDrawBankTextWithLayoutFlags(0x970, 0xb58, 1, list->cursor->camp.id, (FrFontTextBank *)datItemMessages,
                              layoutFlags);
        frSetFontColor(glyph, color);
        frFontDrawGlyphChain(glyph, 1, glyphAttribute);
        frDestroyFontRequestAfter(glyph);
        return;
    }
}


