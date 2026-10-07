#include "common.h"
#include "dat_state.h"
#include "dat_command.h"
#include "eff.h"
#include "mnu.h"

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

/* Text builders return integer glyph handles; frFont consumes glyph pointers. */
typedef struct FrFontGlyph FrFontGlyph;

extern const u8 (*D_00435E5C)[25];
extern char D_00437C38[];
extern char D_00437C40[];
extern u32 uiBlendColors(u32, u32, u32);
extern s32 mdlFlagTest(u32);
extern s32 mnuGetPartyEntryMenuValue(DatPartyRecord *);
extern u16 mnuGetPartyEntryCurrentId(DatPartyRecord *);
extern s32 evtGetIndexedEventRecordId(s32);
extern s32 func_0035C860(char *, const char *, ...);
extern u32 itfCreateConvertedTextGlyph(s32, s32, s32, u32, const u8 *, s32);
extern u32 func_0019F5E8(s32, s32, s32, u32, char *, s32);
extern void frFontSetChainFlag(FrFontGlyph *, u8);
extern s32 func_0019D550(FrFontGlyph *, s8, u32);
extern s32 frFontQueueGlyphInSelectedSlot(FrFontGlyph *);
extern void func_00306CD0(s32, s32, s32, s32, s32, s32, s32, s32);
extern char D_00437C38[];

void func_002BDAA8(s32 x, s32 y, s32 alpha, s32 entryId, s32 sprite, s32 spriteArg) {
    char text[0x10];
    s32 current;
    s32 required;
    u32 glyph;
    u32 color;
    s32 thresholdX = x + 0x130;

    if (entryId != 0) {
        current = func_002BDA50(entryId);
        required = func_002BDA78(entryId);
        func_0035C860(text, D_00437C38, required - current);
        color = uiBlendColors(0xA09DC380, 0xA09DC300, alpha);
        glyph = func_0019F5E8(x, y, 0, color, text, 0);
        frFontSetChainFlag((FrFontGlyph *)glyph, 4);
        func_0019D550((FrFontGlyph *)glyph, 1, 0x53);
        frFontQueueGlyphInSelectedSlot((FrFontGlyph *)glyph);
        func_00306CD0(x + 0x80, y, 0, alpha, 0, sprite, spriteArg, 0x53);
        func_0035C860(text, D_00437C38, required);
        color = uiBlendColors(0xA09DC340, 0xA09DC300, alpha);
        glyph = func_0019F5E8(thresholdX, y, 0, color, text, 0);
        func_0019D550((FrFontGlyph *)glyph, 1, 0x53);
        frFontQueueGlyphInSelectedSlot((FrFontGlyph *)glyph);
    }
}

void mnuDrawPartyCommandPage(s32 unusedX, s32 unusedY, s32 depth, s32 partyIndex, MenuSprites *page, s32 param) {
    char text[0x20];
    DatPartyRecord *party = &datGameState->party[partyIndex];
    void **sprite = page->item;
    u32 i = 0;
    s32 alpha = page->drawAlpha;
    u32 color = uiBlendColors(0xA09DC380, 0xA09DC300, alpha);
    s32 showCurrent = mdlFlagTest(0x901);
    s32 x = page->slideOffset * 16 + 0xC80;
    s32 value;
    s32 glyph;

    do {
        void *resource = *sprite++;

        if (resource != NULL) {
            if (i == 2) {
                func_00306CD0(x - 0x40, 0x10, depth, alpha, 0, page->item[2], 0, param);
            } else {
                func_00306CD0(x, 0x20, depth, alpha, 0, resource, 0, param);
            }
        }
        i++;
    } while (i < 11);

    value = mnuGetPartyEntryMenuValue(party);
    if (value != 0) {
        glyph = itfCreateConvertedTextGlyph(x + 0x630, 0x340, depth, color, D_00435E5C[value], 0);
        func_0035C860(text, D_00437C40, datCommandRecords[evtGetIndexedEventRecordId(value)].stat18);
        glyph = func_0019F5E8(x + 0x1050, 0x360, depth, color, text, glyph);
        if (page->unk74 != 0) {
            frFontSetChainFlag((FrFontGlyph *)glyph, 4);
        }
        func_0019D550((FrFontGlyph *)glyph, 1, param);
        frFontQueueGlyphInSelectedSlot((FrFontGlyph *)glyph);
    } else {
        func_00306CD0(x, 0x20, depth, alpha, 0, page->cursor[0], 0, param);
    }

    if (showCurrent != 0) {
        value = mnuGetPartyEntryCurrentId(party);
        if (value != 0) {
            s8 style;

            glyph = itfCreateConvertedTextGlyph(x + 0x630, 0x418, depth, color, D_00435E5C[value], 0);
            style = page->unk75;
            if (style == 1 || (style == 2 && func_002BDA50(value) == 0)) {
                frFontSetChainFlag((FrFontGlyph *)glyph, 4);
            } else if (style == 2) {
                frFontSetChainFlag((FrFontGlyph *)glyph, 3);
            }
            func_0019D550((FrFontGlyph *)glyph, 1, param);
            frFontQueueGlyphInSelectedSlot((FrFontGlyph *)glyph);
            if (page->flags & 1) {
                func_00306CD0(x + 0xE30, 0x408, depth, alpha, 0, page->cursor[3], 0, param);
                func_002BDAA8(x + 0x1020, 0x438, alpha, value, page->cursor[2], 0);
            }
        } else {
            func_00306CD0(x, 0x20, depth, alpha, 0, page->cursor[1], 0, param);
        }
    }

    if (page->fadeOut == 0) {
        if (page->drawAlpha < 256) {
            page->drawAlpha += 16;
        }
        if (page->drawAlpha > 256) {
            page->drawAlpha = 256;
        }
        page->slideOffset -= page->slideSpeed / 256;
        if (page->slideOffset < 0) {
            page->slideOffset = 0;
        }
        if (page->slideOffset != 0) {
            page->slideSpeed *= 1.5f;
        }
    } else {
        if (page->drawAlpha > 0) {
            page->drawAlpha -= 32;
        }
        if (page->drawAlpha < 0) {
            page->drawAlpha = 0;
        }
        page->slideOffset += page->slideSpeed / 256;
        if (page->slideOffset != 0) {
            page->slideSpeed /= 1.5f;
        }
    }
}

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
    func_0019D550((FrFontGlyph *)item, 1, param);
    frFontQueueGlyphInSelectedSlot((FrFontGlyph *)item);
}

extern s32 func_00314C10(DatPartyRecord *);
extern s32 scrGetIndexedRecordAddress(s32, s32 *);

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
        func_0019D550((FrFontGlyph *)item, 1, param);
        frFontQueueGlyphInSelectedSlot((FrFontGlyph *)item);
    }
}

extern u32 ptyComputeTotalExp(DatPartyRecord *, s32);
extern char D_00437C48[];
extern void mnuDrawFadeIcons(s32, s32, s32, s32, MenuIconBundle *, s32);
extern void mnuDrawIconRow(s32, s32, s32, s32, MenuSprites *, s32);

void func_002BE240(s32 x, s32 y, s32 depth, MenuPageSlot *slot,
                   s32 partyIndex, u32 flags, s32 surface) {
    char text[0x40];
    DatPartyRecord *unit = &datGameState->party[partyIndex];
    u32 color;
    s32 remainingExp;
    FrFontGlyph *glyph;

    if (slot->iconBundle != 0) {
        mnuDrawFadeIcons(x, y, depth, slot->kind,
            (MenuIconBundle *)slot->iconBundle, surface);
        color = uiBlendColors(0xA09DC380, 0xA09DC300,
            ((MenuIconBundle *)slot->iconBundle)->fade);
        remainingExp = ptyComputeTotalExp(unit, 1) - unit->totalExp;
        if (remainingExp != 0) {
            func_0035C860(text, D_00437C48, remainingExp);
            glyph = (FrFontGlyph *)func_0019F5E8(0x1A50,
                0xD8, depth, color, text, 0);
            frFontSetChainFlag(glyph, 3);
            func_0019D550(glyph, 1, surface);
            frFontQueueGlyphInSelectedSlot(glyph);
        }
    }
    if (slot->windowSprites != NULL) {
        if (flags & 0x200) {
            mnuDrawIconRow(x, y, depth, 0, slot->windowSprites, surface);
        }
        mnuDrawSelectedPartyProfileLabel(x, y, depth,
            slot->windowSprites->fade, slot->windowSprites->unkC, (s32)slot,
            partyIndex, surface);
        if (slot->windowSprites->fadeOut == 0) {
            if (slot->windowSprites->fade > 0) {
                slot->windowSprites->fade -= 0x20;
            }
            if (slot->windowSprites->fade < 0) {
                slot->windowSprites->fade = 0;
            }
        } else {
            if (slot->windowSprites->fade < 0x100) {
                slot->windowSprites->fade += 0x20;
            }
            if (slot->windowSprites->fade > 0x100) {
                slot->windowSprites->fade = 0x100;
            }
        }
    }
}

extern const s32 D_0042B028[8];
extern const s32 D_00437C50[];

/* Party-page slot, including the resource handles initialized by
 * mnuLoadPanelSectionResources and both queued commands. */
typedef struct MenuPageSpriteSlot {
    s32 kind;
    u32 flags;
    u8 pad08[8];
    struct EffectSlotSet *icon[3];
    MenuPageBar hp;
    MenuPageBar mp;
    struct EffectSlotSet *frame[8];
    struct MenuSprites *windowSprites;
    u32 iconBundle;
    u32 sectionResources[3]; /* 0xE4 */
    MenuQueuedCommand commands[2]; /* 0xF0 */
} MenuPageSpriteSlot;

typedef char MenuPageSpriteSlot_size_check[
    sizeof(MenuPageSpriteSlot) == sizeof(MenuPageSlot) ? 1 : -1];
typedef char MenuPageSpriteSlot_resources_check[
    (u32)&((MenuPageSpriteSlot *)0)->sectionResources == 0xE4 ? 1 : -1];

void func_002BE438(s32 x, s32 y, s32 z, MenuPageSpriteSlot *slot,
                   s32 coordinateSet, s32 skipSprites, s32 spriteArg) {
    s32 firstPosition[2];
    s32 alternatePositions[8];

    memcpy(firstPosition, D_00437C50, sizeof(firstPosition));
    memcpy(alternatePositions, D_0042B028, sizeof(alternatePositions));
    if (skipSprites == 0) {
        func_00306CD0(x + firstPosition[0], y + firstPosition[1], z,
                      0x100, 1, slot->sectionResources[0], 0, spriteArg);
        func_00306CD0(x + alternatePositions[0], y + alternatePositions[1], z,
                      0x100, 1, slot->sectionResources[1], 0, spriteArg);
        func_00306CD0(x + alternatePositions[coordinateSet * 2 + 2],
                      y + alternatePositions[coordinateSet * 2 + 3], z,
                      0x100, 1, slot->sectionResources[2], 0, spriteArg);
    }
}

void mnuBlendPanelSlots(EffectSlotSet *dst, EffectSlotSet *src, s32 amount) {
    s32 i;

    for (i = 0; i < 4; i++) {
        dst->workEntries->geometry.cornerColors[i] = uiBlendColors(
            dst->workEntries->savedColors[i],
            src->workEntries->savedColors[i],
            amount / 2 + 0x80);
    }
}
INCLUDE_RODATA(const s32, "game/code_002BD5C8", D_0042B028);

INCLUDE_RODATA(const s32, "game/code_002BD5C8", D_0042B048);

INCLUDE_RODATA(const s32, "game/code_002BD5C8", D_0042B098);

INCLUDE_SDATA(const s32, "game/code_002BD5C8", D_00437C38);

INCLUDE_SDATA(const s32, "game/code_002BD5C8", D_00437C40);

INCLUDE_SDATA(const s32, "game/code_002BD5C8", D_00437C48);

INCLUDE_SDATA(const s32, "game/code_002BD5C8", D_00437C50);

