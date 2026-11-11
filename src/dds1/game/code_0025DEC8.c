#include "common.h"
#include "mnu_list.h"
#include "mnu_shop.h"
#include "itf.h"
#include "fr_font.h"
#include "fr_font_draw_glyph.h"

extern u8 *datItemMessages;

extern frFontHandle_t *itfDrawBankTextWithLayoutFlags(s32, s32, s32, u16, frTextBank_t *, s32);

void itfEmitSelectedGlyph(ShopScene *context, s32 unused, s32 layoutFlags,
                   u32 color, u32 glyphAttribute) {
    struct MenuList *list;
    frFontHandle_t *glyph;

    list = context->window->list;
    if (list->count != 0) {
        glyph = itfDrawBankTextWithLayoutFlags(0x970, 0xb58, 1, list->cursor->camp.id, (frTextBank_t *)datItemMessages,
                              layoutFlags);
        frSetFontColor(glyph, color);
        frDrawFontOt(glyph, 1, glyphAttribute);
        frDestroyFontRequestAfter(glyph);
        return;
    }
}


