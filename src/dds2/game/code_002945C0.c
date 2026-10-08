#include "common.h"
#include "mnu_list.h"
#include "eff.h"
#include "itf.h"


extern s32 D_00435E70;
extern FrFontGlyph *itfDrawBankTextWithLayoutFlags(s32, s32, s32, u16, FrFontTextBank *, s32);
extern s32 func_0019D550(FrFontGlyph *, s8, u32);


void itfEmitSelectedGlyph(MenuTerminalContext *context, s32 unused, s32 layoutFlags,
                          u32 color, u32 renderFlags) {
    struct MenuList *list;
    CampWindowParams *data;
    FrFontGlyph *glyph;
    s32 y;
    u32 code;

    list = context->window->list;
    if (list->count != 0) {
        data = &list->cursor->camp;
        code = data->id;
        if (data->mode == 1 || data->mode == 3) {
            y = 0xBD0;
        } else {
            y = 0xB08;
        }
        glyph = itfDrawBankTextWithLayoutFlags(0x600, y, 1, code, (FrFontTextBank *)D_00435E70, layoutFlags);
        frFontSetChildColors(glyph, color);
        func_0019D550(glyph, 1, renderFlags);
        frFontQueueGlyphInSelectedSlot(glyph);
        return;
    }
}

extern void func_00306CD0(s32, s32, s32, s32, s32, void *, s32, s32);


void func_00294680(MenuTerminalContext *w, s32 a1, s32 a2) {
    BdWork *work;

    if (w->type != 2) {
        return;
    }
    func_00306CD0(0xE30, 0x610, 0, a1, 0, w->effectSlots[2], 1, a2);
    work = w->effectSlots[3]->workEntries;
    work[1].geometry.angleDegrees = 90.0f;
    func_00306CD0(0x9F0, 0x610, 0, a1, 2, w->effectSlots[3], 1, a2);
}
