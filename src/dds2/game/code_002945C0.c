#include "common.h"
#include "mnu_list.h"
#include "eff.h"

extern u32 D_00435E70;
extern u64 itfDrawBankTextWithLayoutFlags(u64, u64, u64, u16, u32, u64);


void itfEmitSelectedGlyph(MenuTerminalContext *context, u64 unused, u64 parentGlyph,
                  u64 color, u64 glyphAttribute) {
    struct MenuList *list;
    CampWindowParams *data;
    u64 glyph;
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
        glyph = itfDrawBankTextWithLayoutFlags(0x600, y, 1, code, D_00435E70, parentGlyph);
        frFontSetChildColors(glyph, color);
        func_0019D550(glyph, 1, glyphAttribute);
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
    work[1].angleDegrees = 90.0f;
    func_00306CD0(0x9F0, 0x610, 0, a1, 2, w->effectSlots[3], 1, a2);
}
