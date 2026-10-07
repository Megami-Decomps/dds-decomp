#include "common.h"
#include "mnu_result.h"

extern u32 kwlnTaskGetUserValue();


extern s32 brsAdvanceSkillPackagePanel(BrsSkillPackageWork *);

extern void func_0029AC20(BrsSkillPackageWork *, s32);

extern void func_0026C900(void);

extern void mnuDrawCampIconBackdrop(MenuCampEffect *, s32);

void func_0029AA48(BrsSkillPackageWork *context) {
    mnuDrawCampIconBackdrop(&context->campEffect, 0x20);
}

struct FrFontGlyph;

extern void func_00306CD0(s32, s32, s32, u32, s32, s32, s32, s32);
extern u32 uiBlendColors(u32, u32, s32);
extern s32 func_0035C860(char *, const char *, ...);
extern u32 func_0019F6C8(s32, s32, s32, u32, char *, s32);
extern void frFontSetChainFlag(struct FrFontGlyph *, u8);
extern s32 func_0019D550(struct FrFontGlyph *, s8, u32);
extern s32 frFontQueueGlyphInSelectedSlot(struct FrFontGlyph *);
extern u32 mnuKindIsSelectable(u32);
extern char D_004379B0[];

void mnuDrawRemainingSelectionExtent(BrsSkillPackageWork *context) {
    char text[16];
    s32 iconFade = context->iconFade;
    DatPartyRecord *item = context->selectedRewardRow->unit;
    s32 delta;
    s32 x;
    struct FrFontGlyph *glyph;
    u32 color;

    func_00306CD0(0x1710, 0x8C0, 0, iconFade, 1,
                  context->unitHandle, 8, 0x53);
    delta = context->commitComplete != 0 ? 0 : context->availableStatPoints - context->assignedStatPoints;
    x = 0x19C0;
    if (delta / 100 <= 0) {
        if (delta / 10 > 0) {
            x = 0x1A50;
        } else {
            x = 0x1AA0;
        }
    }
    color = uiBlendColors(0xA09DC380, 0xA09DC300, iconFade);
    func_0035C860(text, D_004379B0, delta);
    glyph = (struct FrFontGlyph *)func_0019F6C8(x, 0x8E8, 0, color, text, 0);
    frFontSetChainFlag(glyph, 3);
    func_0019D550(glyph, 1, 0x53);
    frFontQueueGlyphInSelectedSlot(glyph);
    if (mnuKindIsSelectable(item->unitId) != 0) {
        if (context->iconFade < 0x100) {
            context->iconFade += 0x20;
        }
        if (context->iconFade > 0x100) {
            context->iconFade = 0x100;
        }
    } else {
        context->iconFade = 0;
    }
}

extern void uiDrawUniformColorRect(u32, u32, u32, u32, u32, u32, u32);
extern void itfDrawGridWithResolvedSlot(u32, u32, u32, u32, u32, u32, u32);
extern void mnuDrawPanelListDefault();
extern void mnuDrawAndAdvancePanelGroup(s32, s32, s32, DatPartyRecord *, MenuPanelGroup *, s32, s32);
extern void func_002C10F0(s32, s32, s32, DatPartyRecord *, MenuSpriteState *, s32);
extern s8 evtStageTestUpdate(s32);
extern char D_00380788[];

void func_0029AC20(BrsSkillPackageWork *context, s32 copyOptions) {
    DatPartyRecord *unit = context->selectedRewardRow->unit;
    s32 i;

    uiDrawUniformColorRect(0, 0, 0, 0x2000, 0xE00, 0x19, 0x53);
    itfDrawGridWithResolvedSlot(0x1740, 0x510, 0, 1, context->unitHandle, 0xA, 0x53);
    context->partyWindow.flags |= 0x200;
    mnuDrawPanelListDefault(0, 0, 0, &context->partyWindow, 0x53);

    for (i = 0; i < 5; i++) {
        if (copyOptions == 0) {
            mnuSetGroupSelection(context->panelHandle, i, context->statGains[i], 0);
        } else {
            mnuSetGroupSelection(context->panelHandle, i, context->statGains[i],
                                 context->statGains[i]);
        }
    }
    mnuApplyPackedGroupValues(context->panelHandle, unit->itemId);
    mnuDrawAndAdvancePanelGroup(0xEB0, 0x518, 0, unit, context->panelHandle, 0, 0x53);
    func_002C10F0(0, 0, 0, unit, context->spriteHandle, 0x53);
    mnuDrawRemainingSelectionExtent(context);
    evtStageTestUpdate((s32)D_00380788);
}

s32 mnuAdvanceSkillPackageToItemPanel(s32 request) {
    BrsSkillPackageWork *context = (BrsSkillPackageWork *)kwlnTaskGetUserValue();

    if (brsAdvanceSkillPackagePanel(context) != 0) {
        return 0;
    }
    func_0029AA48(context);
    func_0029AC20(context, 0);
    return menuSetHandler(context, 1, (void *)request);
}

s32 mnuAdvanceSkillPanelToNextMenu(s32 request) {
    BrsSkillPackageWork *context = (BrsSkillPackageWork *)kwlnTaskGetUserValue();

    if (brsAdvanceSkillPackagePanel(context) != 0) {
        return 0;
    }
    func_0026C900();
    return menuSetHandler(context, 2, (void *)request);
}

/* The five signed config bytes reserve space before the selected entry width. */
u32 mnuResetSelectionWidthsFromConfig(void) {
    s8 widthByte;
    BrsSkillPackageWork *context;
    s32 *widthSlot;
    s8 *configWidths;
    s32 remaining;
    s32 selectionWidth;
    s32 reservedWidth;

    context = (BrsSkillPackageWork *)kwlnTaskGetUserValue();
    reservedWidth = 0;
    remaining = 4;
    selectionWidth = context->selectedRewardRow->values.amount * 3;
    configWidths = context->selectedRewardRow->unit->baseStats;
    do {
        widthByte = *configWidths;
        configWidths = configWidths + 1;
        remaining = remaining - 1;
        reservedWidth = reservedWidth + widthByte;
    } while (-1 < remaining);
    context->assignedStatPoints = 0;
    remaining = 4;
    widthSlot = &context->statGains[4];
    if (0x1ef - reservedWidth < selectionWidth) {
        selectionWidth = 0x1ef - reservedWidth;
    }
    context->availableStatPoints = selectionWidth;
    do {
        remaining = remaining - 1;
        *widthSlot = 0;
        widthSlot = widthSlot + -1;
    } while (-1 < remaining);
    if (context->selectionInitialized != 0) {
        mnuSetPanelGroupSelection(context->panelHandle, 0);
    }
    return 1;
}

u32 func_0029AF40(void) {
    return 1;
}

void mnuClearItemSelectionSlots(BrsSkillPackageWork *context) {
    s32 remaining;
    s32 *destination;

    context->assignedStatPoints = 0;
    destination = &context->statGains[4];
    remaining = 4;
    do {
        remaining = remaining - 1;
        *destination = 0;
        destination = destination + -1;
    } while (-1 < remaining);
}

extern void func_00314298(DatPartyRecord *, const s32 *);
extern void mnuRefreshSelectedUnitPanels(DatPartyRecord *, BrsSkillPackageWork *);

void mnuRefreshPartyUnitVitalsPanels(DatPartyRecord *unit, BrsSkillPackageWork *menu) {
    func_00314298(unit, menu->statGains);
    mnuRefreshSelectedUnitPanels(unit, menu);
}
