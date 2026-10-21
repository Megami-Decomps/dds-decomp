#include "sdk_ps2.h"
#include "common.h"
#include "ui_color.h"
#include "mnu.h"

extern void mnuClearPanelWorkState(u32 panel);

void mnuClearPairedSpriteRecords(s32 scene, s32 groupIndex) {
    s32 remaining;
    s32 entry;

    remaining = 1;
    entry = groupIndex * 0x134 + scene + 0x16c;
    do {
        remaining = remaining - 1;
        mnuClearPanelWorkState(entry);
        entry = entry + 0x20;
    } while (-1 < remaining);
}

extern void uiDrawGradientColorRect(u32, u32, u32, u32, u32, const u32 *, u32);
extern void uiDrawUniformColorRect(u32, u32, u32, u32, u32, u32, u32);

void mnuDrawWidthScaledPanelGradient(s32 x, s32 y, s32 z, MenuPanelFade *work, s32 surface) {
    u32 colors[4];
    s32 width = 0x138;
    s32 color = uiBlendColors(0x14806E4D, 0x14806E00, work->blend);
    s32 height = 0x170;

    if (work->compact == 0) {
        x += 0x660;
        y += 0x110;
    } else {
        x += 0x1A0;
        y += 0x50;
        height = 0x100;
    }
    colors[3] = colors[1] = color & 0xFFFFFF00;
    colors[2] = colors[0] = color;
    uiDrawGradientColorRect(x, y, z, ((width * (0x200 - work->blend)) / 0x200 + 0x40) << 4,
                 height, colors, surface);
}

void mnuDrawHeightScaledPanelGradient(s32 x, s32 y, s32 z, MenuPanelFade *work, s32 surface) {
    u32 colors[4];
    s32 color = uiBlendColors(0x80501E80, 0x80501E00, work->blend);
    s32 height = 0x4A;
    s32 extent;

    if (work->compact == 0) {
        x += 0x660;
        y += 0x280;
    } else {
        x += 0x1A0;
        y += 0x150;
        height = 0x30;
    }
    colors[3] = color;
    colors[2] = color;
    colors[1] = colors[0] = color & 0xFFFFFF00;
    extent = (height * (0x200 - work->blend) / 0x200) << 3;
    uiDrawGradientColorRect(x, y - extent, z, 0x1000, extent, colors, surface);
}

/* Draw the expanding panel gradient and its dim full-size overlay. */
void mnuDrawPanelFadeSprite(s32 x, s32 y, s32 z, MenuPanelFade *work, s32 surface) {
    u32 colors[4];
    u32 color;
    u32 transparentColor;
    s32 height;

    color = uiBlendColors(0x5A3C8026, 0x5A3C8000, work->blend);
    transparentColor = color & 0xFFFFFF00;
    colors[0] = color;
    colors[1] = color;
    colors[2] = transparentColor;
    colors[3] = transparentColor;
    height = ((0x200 - work->blend) / 2 + 0x40) << 4;
    uiDrawGradientColorRect(x, y, z, 0x2000, height, colors, surface);
    uiDrawUniformColorRect(x, y, z, 0x2000, 0xE00,
                          transparentColor | ((color & 0xFF) >> 2), surface);
}

typedef enum MenuPanelFadeKind {
    MENU_PANEL_FADE_WIDTH,
    MENU_PANEL_FADE_HEIGHT,
    MENU_PANEL_FADE_SPRITE
} MenuPanelFadeKind;

extern void sdfSubmitGsAlphaOneRegisterPacket(s32 property, s32 object);

void mnuDrawAndAdvancePanelFades(s32 x, s32 y, s32 z, MenuPanelFade *work, s32 surface) {
    s32 i;

    sdfSubmitGsAlphaOneRegisterPacket(0x48, surface);
    for (i = 0; i < 2; i++) {
        if (work[i].blend != 0) {
            if (work[i].delay == 0) {
                switch ((MenuPanelFadeKind)work[i].kind) {
                case MENU_PANEL_FADE_WIDTH:
                    mnuDrawWidthScaledPanelGradient(x, y, z, &work[i], surface);
                    break;
                case MENU_PANEL_FADE_HEIGHT:
                    mnuDrawHeightScaledPanelGradient(x, y, z, &work[i], surface);
                    break;
                case MENU_PANEL_FADE_SPRITE:
                    mnuDrawPanelFadeSprite(0, 0, z, &work[i], 0x54);
                    break;
                }
                if (work[i].blend > 0x100) {
                    work[i].blend -= work[i].highBlendStep;
                } else {
                    work[i].blend -= work[i].lowBlendStep;
                }
                if (work[i].blend <= 0) {
                    mnuClearPanelWorkState(&work[i]);
                }
            } else {
                work[i].delay--;
            }
        }
    }
}

#include "fr_font_draw_glyph.h"
extern char D_003BC760[]; /* "%2d", shared with the status-panel renderer. */
extern void mnuDrawPartyPanelResourceIcons(s32, s32, s32, MenuPageSlot *, s32, s32);
extern void mnuDrawPartyPageEntry(s32, s32, s32, s32, MenuSprites *, s32);
extern void mnuDrawPartyProfileAndExpOverlay(s32, s32, s32, MenuPageSlot *, s32, u32, s32);
extern void effDrawSpriteWithCornerFade(s32, s32, s32, u32, s32, struct EffectSlotSet *, s32, s32);
extern void mnuDrawPartyPanelSlotWithBlend(s32, s32, s32, MenuPageWindow *, s32, struct EffectSlotSet **, s32, s32);
extern void mnuDrawPartyRowFrameVariant(s32, s32, s32, MenuPageWindow *, s32, s32, s32);
extern void uiDrawActiveSurfaceRegion(s32);
extern void sdfDispatchSurfaceWithPreparedTexturePacket(s32);
extern void uiDrawSurfaceAtNearDepth(u32);
extern struct FrFontGlyph *itfCreateFont2TextGlyph(s32, s32, s32, u32, char *, struct FrFontGlyph *);
extern s32 frDestroyFontRequestAfter(struct FrFontGlyph *);
/* Retail forwards the surface word; the selection helper uses five arguments. */
extern s32 mnuClearWindowPendingFlagAfterSelection();
extern void mnuDrawPartyPageContents(s32, s32, s32, s32, struct MenuPageResources *, s32);
extern u32 mnuGetPanelRatioColor(s32, s32, s32);
extern void mnuDrawAndAdvanceRatioPanel(s32, s32, s32, u32, s32, s32, MenuEffectPair *, u32);

/* Draw the party row, level label and its page or HP/MP bars.
 * Retains the current DDS2 sibling and Son3 reconstruction findings. */
void mnuDrawPartyMemberCommandPanel(s32 x, s32 y, s32 z, MenuPageWindow *menu,
                                    s32 panelIndex, s32 surface) {
    char text[16];
    MenuPageSlot *panel = &menu->slots[panelIndex];
    s32 labelX, labelY;
    s32 transition = menu->transitionValue;
    s32 iconPositions[4] = {32, 32, 512, 152};
    s32 framePositions[6] = {0x1C0, 0x108, 0x210, 0x238, 0x430, 0x10};
    /* Inferred bound keeps this literal's array alias set distinct from the
     * fixed two-axis bar origin. Stock cc1 RTL/sched1 preserves the native
     * independent 8-byte copies; spelling both [2] adds an output dependency. */
    s32 pagePosition[] = {0x6B0, 0xD8};
    s32 barPosition[2] = {0x850, 0x138};
    s32 alpha;
    u32 color;
    s32 selection;
    s32 level;
    s32 iconX, iconY;
    struct FrFontGlyph *glyph;

    if (panel->flags & 1) {
        return;
    }
    iconX = x + iconPositions[0];
    iconY = y + iconPositions[1];
    if (panel->leftHandle != NULL && (panel->flags & 0x80) != 0) {
        mnuDrawPartyPanelResourceIcons(iconX, iconY, z, panel, 0, surface);
        return;
    }
    if (menu->flags & 0x500) {
        if (panel->windowSprites != NULL) {
            mnuDrawPartyPageEntry(x, y, z, menu->records->slots[panelIndex].index,
                                 panel->windowSprites, surface);
        }
        mnuDrawPartyProfileAndExpOverlay(x, y, z, panel,
                                        menu->records->slots[panelIndex].index,
                                        menu->flags, surface);
    }
    effDrawSpriteWithCornerFade(iconX, iconY, z, transition, 1, panel->icon[0], 0, surface);
    uiDrawActiveSurfaceRegion(surface);
    mnuDrawPartyPanelSlotWithBlend(x + framePositions[4], y + framePositions[5], 0xFFFFFF,
                                  menu, panelIndex, panel->frame, 1, surface);
    sdfDispatchSurfaceWithPreparedTexturePacket(surface);
    mnuDrawPartyRowFrameVariant(x, y, z, menu, panelIndex, 0, surface);
    labelX = x + iconPositions[2];
    labelY = y + iconPositions[3];
    color = uiBlendColors(0xA09DC380, 0xA09DC300, transition);
    if (panel->iconBundle != NULL) {
        color = uiBlendColors(0xFFF06480, color, panel->iconBundle->fade);
        alpha = 0x100 - panel->iconBundle->fade;
    } else {
        alpha = transition;
    }
    effDrawSpriteWithCornerFade(labelX, labelY, z, alpha, 1, panel->icon[2], 0, surface);
    level = menu->records->slots[panelIndex].level;
    sprintf(text, D_003BC760, level);
    glyph = itfCreateFont2TextGlyph(labelX + 0x150, labelY, z, color, text, NULL);
    frFontDrawGlyphChain(glyph, 1, surface);
    frDestroyFontRequestAfter(glyph);
    uiDrawActiveSurfaceRegion(surface);
    mnuDrawPartyPanelSlotWithBlend(x + framePositions[4], y + framePositions[5], 0xFFFFFF,
                                  menu, panelIndex, panel->frame, 1, surface);
    sdfDispatchSurfaceWithPreparedTexturePacket(surface);
    mnuDrawAndAdvancePanelFades(x, y, z, panel->work, surface);
    uiDrawSurfaceAtNearDepth(surface);
    mnuDrawPartyPanelSlotWithBlend(x + framePositions[4], y + framePositions[5], z,
                                  menu, panelIndex, panel->frame, 0, surface);
    if (panel->frame[5] == NULL) {
        effDrawSpriteWithCornerFade(x + framePositions[0], y + framePositions[1], z,
                                  transition, 1, panel->frame[4], 0, surface);
    } else {
        effDrawSpriteWithCornerFade(x + framePositions[0] + 0x60, y + framePositions[1] + 0x30, z,
                                  transition, 1, panel->frame[5], 0, surface);
    }
    selection = mnuClearWindowPendingFlagAfterSelection(x + framePositions[2], y + framePositions[3], z,
                                                       menu, panelIndex, surface);
    effDrawSpriteWithCornerFade(x + framePositions[2], y + framePositions[3], z,
                              transition, 1, panel->frame[selection + 2], 0, surface);
    if (panel->resources != NULL && (menu->flags & 0x40)) {
        mnuDrawPartyPageContents(x + pagePosition[0], y + pagePosition[1], z,
                                panel->kind, panel->resources, surface);
        return;
    }
    if (panel->bars[0].resourceSets[0] != NULL) {
        s32 hp = menu->records->slots[panelIndex].hp;
        s32 maxHp = (menu->flags & 0x400) ? menu->records->slots[panelIndex].maxHp : -1;
        u32 hpColor = mnuGetPanelRatioColor(0, hp, menu->records->slots[panelIndex].maxHp);
        mnuDrawAndAdvanceRatioPanel(x + barPosition[0] + 0x40, y + barPosition[1] + 0x28, z,
                                    hpColor, hp, maxHp, &panel->bars[0], surface);
        hp = menu->records->slots[panelIndex].mp;
        maxHp = (menu->flags & 0x400) ? menu->records->slots[panelIndex].maxMp : -1;
        mnuDrawAndAdvanceRatioPanel(x + barPosition[0] + 0x40, y + barPosition[1] + 0xB8, z,
                                    0xA09DC380, hp, maxHp, &panel->bars[1], surface);
    }
}

/* Shared level-number format, followed by the next literal's alignment. */
char D_003BC760[] = "%2d";

/* Draw the status row, level label and its page or HP/MP bars.
 * Retains the released Son4/Son3 reconstruction findings. */
void mnuDrawPartyMemberStatusPanel(s32 x, s32 y, s32 z, MenuPageWindow *menu,
                                 s32 panelIndex, s32 surface) {
    char text[16];
    MenuPageSlot *panel = &menu->slots[panelIndex];
    s32 alpha = menu->transitionValue;
    u32 textColor = uiBlendColors(0x80808080, 0x80808000, alpha);
    /* As in the command panel, inferred bounds preserve distinct array alias
     * sets for independent literal copies in the stock compiler. */
    s32 positions[4] = {32, 16, 0x280, 0x60};
    s32 framePositions[] = {0x50, 0x38, 0x2C0, 0xC8};
    s32 pagePosition[] = {0x540, 0x28};
    s32 barPosition[2] = {0x6E0, 0x48};
    s32 labelX, labelY;
    s32 iconX, iconY;
    s32 selection;
    s32 level;
    struct FrFontGlyph *glyph;

    if (panel->flags & 1) {
        return;
    }
    iconX = x + positions[0];
    iconY = y + positions[1];
    if (panel->leftHandle != NULL && (panel->flags & 0x80) != 0) {
        mnuDrawPartyPanelResourceIcons(iconX, iconY, z, panel, 1, surface);
        return;
    }
    effDrawSpriteWithCornerFade(iconX, iconY, z, alpha, 1, panel->icon[1], 0, surface);
    uiDrawActiveSurfaceRegion(surface);
    effDrawSpriteWithCornerFade(x + framePositions[0], y + framePositions[1], 0xFFFFFF,
                               alpha, 1, panel->frame[4], 0, surface);
    sdfDispatchSurfaceWithPreparedTexturePacket(surface);
    mnuDrawPartyRowFrameVariant(x, y, z, menu, panelIndex, 0, surface);
    labelX = x + positions[2];
    labelY = y + positions[3];
    effDrawSpriteWithCornerFade(labelX, labelY, z, alpha, 1, panel->icon[2], 0, surface);
    level = menu->records->slots[panelIndex].level;
    sprintf(text, D_003BC760, level);
    glyph = itfCreateFont2TextGlyph(labelX + 0x170, labelY, z, textColor, text, NULL);
    frFontDrawGlyphChain(glyph, 1, surface);
    frDestroyFontRequestAfter(glyph);
    uiDrawActiveSurfaceRegion(surface);
    effDrawSpriteWithCornerFade(x + framePositions[0], y + framePositions[1], 0xFFFFFF,
                               alpha, 1, panel->frame[4], 0, surface);
    sdfDispatchSurfaceWithPreparedTexturePacket(surface);
    mnuDrawAndAdvancePanelFades(x, y, z, panel->work, surface);
    uiDrawSurfaceAtNearDepth(surface);
    if (panel->frame[5] == NULL) {
        effDrawSpriteWithCornerFade(x + framePositions[0], y + framePositions[1], z,
                                   alpha, 1, panel->frame[4], 0, surface);
    } else {
        effDrawSpriteWithCornerFade(x + framePositions[0] + 0x60, y + framePositions[1] + 0x30, z,
                                   alpha, 1, panel->frame[5], 0, surface);
    }
    selection = mnuClearWindowPendingFlagAfterSelection(x + framePositions[2], y + framePositions[3], z,
                                                       menu, panelIndex, surface);
    if (selection != 0) {
        mnuDrawPartyRowFrameVariant(x, y, z, menu, panelIndex, 1, surface);
    }
    effDrawSpriteWithCornerFade(x + framePositions[2], y + framePositions[3], z,
                               alpha, 1, panel->frame[selection + 2], 0, surface);
    if (panel->resources != NULL && (menu->flags & 0x40)) {
        mnuDrawPartyPageContents(x + pagePosition[0], y + pagePosition[1], z,
                                panel->kind, panel->resources, surface);
        return;
    }
    if (panel->bars[0].resourceSets[0] != NULL) {
        s32 hp = menu->records->slots[panelIndex].hp;
        u32 hpColor = mnuGetPanelRatioColor(0, hp, menu->records->slots[panelIndex].maxHp);
        mnuDrawAndAdvanceRatioPanel(x + barPosition[0] + 0x40, y + barPosition[1] + 0x28, z,
                                   hpColor, hp, -1, &panel->bars[0], surface);
        mnuDrawAndAdvanceRatioPanel(x + barPosition[0] + 0x40, y + barPosition[1] + 0xB8, z,
                                   0xA09DC380, menu->records->slots[panelIndex].mp,
                                   -1, &panel->bars[1], surface);
    }
}
