#include "common.h"
#include "dat_state.h"

INCLUDE_ASM(const s32, "game/code_002BD5C8", func_002BD5C8);
INCLUDE_ASM(const s32, "game/code_002BD5C8", func_002BD710);

s32 mnuClearWindowPendingFlagAfterSelection(s32 arg0, s32 arg1, s32 arg2, u32 *window, s32 arg4) {
    s32 result = func_002BD5C8(window, arg4);

    switch (result) {
    case 1:
        *window &= ~2;
        return 1;
    case 2:
        *window &= ~2;
        return 1;
    default:
        return 0;
    }
}


u8 func_002BDA50(s32 index) {
    if (index == 0) {
        return 0;
    }
    return datGameState->itemRequirementCounts[index - 0xC0];
}

extern s32 D_00435E3C;

s8 func_002BDA78(s32 value) {
    s32 index = value - 0xC0;

    if (value == 0) {
        return 0;
    }
    return *(s8 *)(D_00435E3C + index * 6 + 5);
}

INCLUDE_ASM(const s32, "game/code_002BD5C8", func_002BDAA8);


INCLUDE_ASM(const s32, "game/code_002BD5C8", func_002BDC38);

extern void func_00314500(u32, s32, char *);
extern s32 func_0019CE78(s32 *, s32, s32, s32, s32);
extern void frFontSetChildColors(s32, u32);
extern void frFontSetContextPair(s32, s32, s32);

void func_002BE080(s32 x, s32 y, s32 unused, s32 color, s32 textId, s32 param) {
    char text[0x40];
    s32 item;

    color = (color & 0xFF) | 0xD7ABFA00;
    func_00314500(textId & 0xFFFF, 1, text);
    item = func_0019CE78((s32 *)text, 0, 0, 0, 0);
    frFontSetChildColors(item, color);
    frFontSetContextPair(item, x, y);
    func_0019D550(item, 1, param);
    frFontQueueGlyphInSelectedSlot(item);
}

extern s32 func_00314C10(DatPartyRecord *);
extern s32 uiBlendColors();
extern s32 scrGetIndexedRecordAddress(s32, s32 *);
extern u32 itfCreateConvertedTextGlyph(s32, s32, s32, u32, const u8 *, s32);
extern void func_0019D550(s32, s32, s32);
extern void frFontQueueGlyphInSelectedSlot(s32);

void mnuDrawSelectedPartyProfileLabel(s32 unusedX, s32 unusedY, s32 depth, s32 fade, s32 selectedCode, s32 unused,
                                      s32 partyIndex, s32 param) {
    s32 outValue;
    s32 cost = func_00314C10(&datGameState->party[partyIndex]);
    s32 code;
    s32 texture;
    s32 item;

    texture = uiBlendColors(0xA09DC380, 0xA09DC300, fade);
    code = selectedCode != 0 ? selectedCode : cost;
    if (code != 0) {
        if (scrGetIndexedRecordAddress(code & 0xFFFF, &outValue) != 0) {
            func_002BE080(0x16B0, 0x4B8, depth, texture, code, param);
            return;
        }
        item = itfCreateConvertedTextGlyph(0x16B0, 0x4B8, depth, texture, (const u8 *)outValue, 0);
        func_0019D550(item, 1, param);
        frFontQueueGlyphInSelectedSlot(item);
    }
}

INCLUDE_ASM(const s32, "game/code_002BD5C8", func_002BE240);

INCLUDE_ASM(const s32, "game/code_002BD5C8", func_002BE438);

typedef struct MenuBlendContext {
    u8 pad00[0x14];
    s32 result[4];
    u8 pad24[0x60];
    s32 source[4];
} MenuBlendContext;

typedef struct MenuBlendObject {
    u8 pad00[0x18];
    MenuBlendContext *context;
} MenuBlendObject;

void mnuBlendPanelSlots(MenuBlendObject *dst, MenuBlendObject *src, u32 amount) {
    s32 i;
    s32 ctx = (s32)dst->context;

    for (i = 0; i < 4; i++) {
        s32 result = uiBlendColors(((MenuBlendContext *)ctx)->source[i],
                                   src->context->source[i],
                                   (s32)amount / 2 + 0x80, ctx);
        s32 current = (s32)dst->context;
        ctx = current;
        ((MenuBlendContext *)current)->result[i] = result;
    }
}
INCLUDE_RODATA(const s32, "game/code_002BD5C8", D_0042B028);

INCLUDE_RODATA(const s32, "game/code_002BD5C8", D_0042B048);

INCLUDE_RODATA(const s32, "game/code_002BD5C8", D_0042B098);

INCLUDE_SDATA(const s32, "game/code_002BD5C8", D_00437C38);

INCLUDE_SDATA(const s32, "game/code_002BD5C8", D_00437C40);

INCLUDE_SDATA(const s32, "game/code_002BD5C8", D_00437C48);

INCLUDE_SDATA(const s32, "game/code_002BD5C8", D_00437C50);

