#include "common.h"
#include "mnu_list.h"
#include "eff.h"

struct FrFontGlyph;
struct TextStyleNode;

extern s32 D_00435E70;
extern s32 itfDrawBankTextWithLayoutFlags(s32, s32, s32, u16, s32, s32);
extern void frFontSetChildColors(struct TextStyleNode *, u32);
extern s32 func_0019D550(struct FrFontGlyph *, s8, u32);
extern s32 frFontQueueGlyphInSelectedSlot(struct FrFontGlyph *);


void itfEmitSelectedGlyph(MenuTerminalContext *context, s32 unused, s32 layoutFlags,
                          u32 color, u32 renderFlags) {
    struct MenuList *list;
    CampWindowParams *data;
    s32 glyphAddress;
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
        glyphAddress = itfDrawBankTextWithLayoutFlags(0x600, y, 1, code, D_00435E70, layoutFlags);
        frFontSetChildColors((struct TextStyleNode *)glyphAddress, color);
        func_0019D550((struct FrFontGlyph *)glyphAddress, 1, renderFlags);
        frFontQueueGlyphInSelectedSlot((struct FrFontGlyph *)glyphAddress);
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
