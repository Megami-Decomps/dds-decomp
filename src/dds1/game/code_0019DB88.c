#include "common.h"
#include "itf_debug_panel_work.h"
#include "itf.h"
#include "itf_message_manager_task.h"
#include "itf_option_bit_index_api.h"
#include "itf_mes_selection_commit_result.h"
#include "snd_sequence_volume_pan.h"
#include "snd_pad.h"
#include "itf_mes_window.h"
#include "itf_mes_panel_mode.h"
#include "sdf_packet_list.h"
#include "kwln.h"
#include "kwln_task_create.h"
#include "sdf_resource.h"
#include "fr_font_temporary_entry_api.h"
#include "fr_font_measure.h"
#include "itf_mes_state_api.h"
#include "pcp_vu0.h"
#include "scr.h"
#include "bf_flw0_header.h"
#include "sdf_sif_command.h"
#include "sdf_projection.h"
#include "itf_panel_draw.h"
#include "itf_panel_api.h"

#include "sdf.h"
/* Declared before first use (were implicit int). */
extern void itfMesBuildEntryGlyph(instanceMes_tag *mes);
extern void CommitVarTable(u8 **table);
#include "sdf_debug_api.h"
extern ItfMesGlobals itfMesMngManager;



extern void itfMesShiftPanelVertically(instanceMes_tag *panel, s32 dy);
extern void ChangeFontColorAll(_FRQ *node, u8 flagValue);
extern ItfMesOptionBitIndex itfMesFindOptionBitIndex(ItfMesVisibleOptionOrdinal optionOrdinal, u32 optionMask);
extern ItfMesSelectionCommitResult SelectItem(instanceMes_tag *);
extern void itfUpdateSoundSelectionTransition(instanceMes_tag *);


extern SndPad dds3PadButtonState;

extern sdfGrOt_t sdfGrOtTable[];

extern ItfMesDebugState D_00357D80;


extern void itfPnlDraw(itfpanel_t *sprite, sdfGrOt_t *surface);

extern void ChangeFontPosition(_FRQ *node, s32 dx, s32 dy);

extern void DebugDrawWindowLine(itfpanel_t *sprite);

extern s32 SelectItemDefKey(ItfMesBlk40 *seq);

extern void ChangeFontColor(_FRQ *glyphChain, s32 rowIndex, s32 rowCount, s32 requestedFlagValue);
extern s8 D_00324530[];


extern const char *D_00358308[];

extern u32 sndTestMessageResourceIndex;

extern sdfTexHandle_t *sndTestMessageTexture;



void frFontEnableNodeContextModes(_FRQ *node) {
    for (; node != NULL; node = node->previous) {
        frFontEnableContextMode(node);
    }
}

/* Initialize panel placement and retain its screen-space Y coordinate. */
void itfMesInitializePanelPlacementSprite(instanceMes_tag *panel) {
    ItfMesEntryBlock *pos = &panel->entryBlock;
    ItfMesBlkA4 *place = &panel->blkA4;
    s32 spriteTop;
    if (place->sprite == 0) {
        place->sprite = itfCreateUiSpriteByKind(6, itfMesMngManager.windowTexture);
        if (place->frame != 0) {
            place->sprite->textRightX = panel->blk14.glyphChain->x + frFontMeasureGlyphStripWidth(0, panel->blk14.glyphChain);
        }
    }
    spriteTop = pos->y + place->bounds[1];
    itfPnlSetPosition(place->sprite, pos->x + place->bounds[0], spriteTop, pos->x + place->bounds[2], pos->y + place->bounds[3], panel->renderValue);
    place->sprite->screenY = spriteTop;
    itfPnlSetColor(place->sprite, place->red, place->green, place->blue, 0);
    panel->flags = (panel->flags & ~0x300) | 0x100;
}

void OpenCheckSpeakerWindow(instanceMes_tag *panel) {
    ItfMesBlk14 *origin = &panel->blk14;
    ItfMesBlkA4 *place = &panel->blkA4;
    if (origin->glyphChain != NULL && !(panel->flags & 0x10000)) {
        if (place->frame == NULL) {
            s32 width = origin->glyphChain->advance * 16;
            place->frame = itfCreateUiSpriteByKind(7, itfMesMngManager.windowTexture);
            itfPnlSetPosition(place->frame, origin->x - 0x2D0, origin->y - 0x68, origin->x + width + 0x2D0, origin->y + 0xF0, panel->renderValue);
            itfPnlSetColor(place->frame, 0x7F, 0x7F, 0x7F, 0);
        }
        panel->flags = (panel->flags & ~0x3000) | 0x1000;
    } else if (place->frame != 0) {
        panel->flags |= 0x3000;
    }
}

void InitializeSpeaker(ItfMesBlk14 *cursor, s32 resetPosition) {
    if (resetPosition != 0) {
        cursor->x = 0x280;
        cursor->y = 0xa10;
    }
    cursor->glyphChain = 0;
    cursor->selectedIndex = 0xffff;
}

void InitializeMessage(ItfMesEntryBlock *cur, s32 resetPos) {
    if (resetPos != 0) {
        cur->x = 0x4B0;
        cur->y = 0xAF8;
    }
    cur->glyphChain = NULL;
    cur->textState = 0;
    cur->textInterfaceMask = 0;
    cur->lineCount = 0;
    cur->itemIndex = 0;
    cur->tableCount = 0;
    cur->color[0] = 0;
    cur->color[1] = 0;
    cur->color[2] = 0;
    cur->color[3] = 0x80;
    cur->table = NULL;
}

void itfInitializeCursorResetState(ItfMesBlk40 *cursor) {
    cursor->x = 0x560;
    cursor->y = 0xC48;
    cursor->glyphChain = NULL;
    cursor->panelValue = 0;
    cursor->selectionState = ITF_SOUND_SELECTION_STATE_INACTIVE;
    cursor->selectedIndex = -1;
    cursor->savedIndex = -1;
    cursor->rowCount = 0;
    cursor->widestRowX = 0;
    cursor->widestRowWidth = 0;
    cursor->glyphColorIntensity = 0;
    cursor->optionCount = 0;
}

void itfMesConfigurePanelBoundsAndFadeLimit(ItfMesBlkA4 *block, ItfMesPanelMode panelMode,
    ItfMesPanelFadeLimit fadeLimit);


void itfResetWindowResourceBlock(ItfMesBlkA4 *block) {
    block->frame = NULL;
    block->sprite = NULL;
    block->overlay = NULL;
    itfMesConfigurePanelBoundsAndFadeLimit(block, ITF_MES_PANEL_MODE_STANDARD, 0);
}

void InitializeVariable(ItfMesTextSlots *slots) {
    s32 remaining;
    u8 **cursor;

    cursor = &slots->addresses[31];
    remaining = 0x1f;
    do {
        remaining = remaining - 1;
        *cursor = 0;
        cursor = cursor + -1;
    } while (-1 < remaining);
}

void InitializeCurCtr(BtlFade *fade, s32 preserveKind) {
    if (preserveKind == 0) {
        fade->kind = 0;
    }
    fade->phase = BTL_FADE_PHASE_ALPHA_RISING;
    fade->timer = 0;
    fade->alpha = 0x40;
    fade->textAdvanceFrameCount = 0;
}

void ForceSetCurCtr(BtlFade *fade, BtlFadePhase phase, s16 alpha, s16 timer) {
    fade->phase = phase;
    fade->alpha = alpha;
    fade->timer = timer;
}

void btlReleaseEffectResourceHandles(instanceMes_tag *effect) {
    ItfMesBlkA4 *place = &effect->blkA4;
    if (place->frame != NULL) {
        itfPnlRelease(place->frame);
        place->frame = NULL;
    }
    if (place->sprite != NULL) {
        itfPnlRelease(place->sprite);
        place->sprite = NULL;
    }
    if (place->overlay != NULL) {
        itfPnlRelease(place->overlay);
        place->overlay = NULL;
    }
    effect->flags &= ~0xF00;
}

void ReleaseAllVariable(ItfMesTextSlots *slots) {
    s32 remaining;
    s32 index;

    remaining = 0x1f;
    index = 0;
    do {
        if (slots->addresses[index] != 0) {
            sdfMemFreeDirect(slots->handles[index]);
            slots->addresses[index] = 0;
        }
        remaining = remaining - 1;
        index++;
    } while (-1 < remaining);
}

u16 *txtFormatNumberU16(s32 number, u16 *out) {
    s32 digits[10];
    s32 count = 0;
    s32 i;
    do {
        digits[count] = number % 10;
        number = number / 10;
        count++;
    } while (number > 0 && count < 10);
    for (i = count - 1; i >= 0; i--) {
        *out++ = (digits[i] << 8) - 0x6F80;
    }
    *out = 0;
    return out;
}

/* Select the message panel's bounds by layout mode (2 is the wide panel) and
 * reset its fade channels; nonpositive limits default to 0x80. */
void itfMesConfigurePanelBoundsAndFadeLimit(ItfMesBlkA4 *block, ItfMesPanelMode panelMode,
    ItfMesPanelFadeLimit fadeLimit) {
    switch (panelMode) {
    default:
        block->bounds[0] = -0x390;
        block->bounds[1] = -0x80;
        block->bounds[2] = 0x1A30;
        block->bounds[3] = 0x288;
        block->red = 0x80;
        block->green = 0x80;
        block->blue = 0x80;
        block->fadeLimit = fadeLimit > 0 ? fadeLimit : 0x80;
        break;
    case ITF_MES_PANEL_MODE_STANDARD_VARIANT:
        block->bounds[0] = -0x390;
        block->bounds[1] = -0x80;
        block->bounds[2] = 0x1A30;
        block->bounds[3] = 0x288;
        block->red = 0x80;
        block->green = 0x80;
        block->blue = 0x80;
        block->fadeLimit = fadeLimit > 0 ? fadeLimit : 0x80;
        break;
    case ITF_MES_PANEL_MODE_WIDE:
        block->bounds[0] = -0x610;
        block->bounds[1] = -0x80;
        block->bounds[2] = 0x1CB0;
        block->bounds[3] = 0x288;
        block->red = 0x80;
        block->green = 0x80;
        block->blue = 0x80;
        block->fadeLimit = fadeLimit > 0 ? fadeLimit : 0x80;
        break;
    }
}

void itfMesUpdatePanelFades(instanceMes_tag *panel);
void CalcCurCtr(instanceMes_tag *panel);

void itfMesAdvanceEntryText(instanceMes_tag *mes);

void itfUpdateBattleDisplayAndFadeIndicator(instanceMes_tag *object) {
    itfMesUpdatePanelFades(object);
    itfMesAdvanceEntryText(object);
    itfUpdateSoundSelectionTransition(object);
    CalcCurCtr(object);
}

void itfMesUpdatePanelFades(instanceMes_tag *panel) {
    ItfMesBlkA4 *place = &panel->blkA4;
    itfpanel_t *sprite;
    s32 transition;

    sprite = place->sprite;
    transition = panel->flags & 0x300;
    switch (transition) {
    case 0x100:
        sprite->alpha += 24;
        if (sprite->alpha >= place->fadeLimit ||
            panel->panelMode == ITF_MES_PANEL_MODE_NO_SPRITE_TRANSITION) {
            sprite->alpha = place->fadeLimit;
            panel->flags = (panel->flags & ~0x307) | 0x203;
        }
        break;
    case 0x300:
        sprite->alpha -= 8;
        if (sprite->alpha <= 0 || panel->panelMode == ITF_MES_PANEL_MODE_NO_SPRITE_TRANSITION) {
            sprite->alpha = 0;
            panel->flags &= ~0x300;
        }
        break;
    }

    sprite = place->frame;
    transition = panel->flags & 0x3000;
    switch (transition) {
    case 0x1000:
        sprite->alpha += 32;
        if (sprite->alpha >= 200) {
            sprite->alpha = 200;
            panel->flags = (panel->flags & ~0x3000) | 0x2000;
        }
        break;
    case 0x3000:
        sprite->alpha -= 32;
        if (sprite->alpha <= 0) {
            sprite->alpha = 0;
            panel->flags &= ~0x3000;
            itfPnlRelease(sprite);
            place->frame = NULL;
        }
        break;
    }

    sprite = place->overlay;
    transition = panel->flags & 0xC00;
    switch (transition) {
    case 0x400:
        sprite->alpha += 24;
        if (sprite->alpha >= place->fadeLimit) {
            sprite->alpha = place->fadeLimit;
            panel->flags = (panel->flags & ~0xC07) | 0x803;
        }
        break;
    case 0xC00:
        sprite->alpha -= 8;
        if (sprite->alpha <= 0) {
            sprite->alpha = 0;
            panel->flags &= ~0xC00;
            itfPnlRelease(sprite);
            place->overlay = NULL;
        }
        break;
    }
}

extern u32 itfMessageFlags;
void itfMesAdvanceEntryText(instanceMes_tag *mes) {
    ItfMesEntryBlock *entryBlock = &mes->entryBlock;
    u32 flags;

    if (entryBlock->textState != 1) {
        return;
    }
    flags = mes->flags;
    if ((flags & 7) == 4) {
        mes->fade.textAdvanceFrameCount++;
        if (mes->flags & 0x800000) {
            entryBlock->textState = -1;
            return;
        }
        if (!(mes->flags & 0x100000)) {
            mes->fade.kind = 1;
        }
        if (!(dds3PadButtonState.confirm < 0 || dds3PadButtonState.primaryCancel < 0 ||
              (dds3PadButtonState.primaryTriangle != 0 && mes->fade.textAdvanceFrameCount >= 5 && !(itfMessageFlags & 1))) &&
              entryBlock->textInterfaceMask != 0) {
            return;
        }
        if (entryBlock->textInterfaceMask != 0) {
            p2sdPlaySE(9, 0x7F, 0x3F);
        }
        InitializeCurCtr(&mes->fade, 0);
        entryBlock->itemIndex++;
        if (entryBlock->itemIndex >= entryBlock->tableCount) {
            entryBlock->textState = -1;
            return;
        }
        CommitVarTable(mes->textSlots.addresses);
        if (mes->temporaryFontEntry != 0) {
            frSetExtDic(mes->temporaryFontEntry);
        }
        itfMesBuildEntryGlyph(mes);
        OpenCheckSpeakerWindow(mes);
        mes->flags = (mes->flags & ~7) | 3;
        return;
    }
    if ((dds3PadButtonState.confirm < 0 || dds3PadButtonState.primaryCancel < 0 || (dds3PadButtonState.primaryTriangle != 0 && !(itfMessageFlags & 1))) &&
        !(mes->flags & 0x1000000) && !(mes->flags & 0x400000)) {
        frFontEnableNodeContextModes(entryBlock->glyphChain);
        if (mes->blk14.glyphChain != NULL) {
            frFontEnableContextMode(mes->blk14.glyphChain);
        }
        flags = mes->flags;
    }
    mes->flags = flags & ~0x1000000;
}

void itfUpdateSoundSelectionTransition(instanceMes_tag *panel) {
    ItfMesBlk40 *selection = &panel->blk40;
    ItfMesEntryBlock *entry = &panel->entryBlock;
    u32 flags = panel->flags;
    s32 top;
    s32 bottom;
    s32 entryY;

    switch (selection->selectionState) {
    case ITF_SOUND_SELECTION_STATE_OPENING:
        if (panel->panelMode == ITF_MES_PANEL_MODE_NO_SPRITE_TRANSITION) {
            selection->selectionState = ITF_SOUND_SELECTION_STATE_INTERACTIVE;
            panel->flags = (flags & ~0x38) | 0x18;
            break;
        }
        entryY = entry->y;
        bottom = entryY + entry->lineCount * (21 << 3);
        top = selection->y - ((selection->rowCount * 21 - 21) << 3);
        if (entryY == 0xAF8 && panel->blkA4.sprite != NULL) {
            panel->blkA4.sprite->scrollSpan = ((bottom - top) / 64) * 64 + 64;
        }
        if (entry->glyphChain != NULL && top < bottom) {
            itfMesShiftPanelVertically(panel, -64);
            return;
        }
        if ((flags & 0xC00) != 0x400) {
            if (entry->glyphChain != NULL) {
                ChangeFontColorAll(entry->glyphChain, 3);
            }
            selection->selectionState = ITF_SOUND_SELECTION_STATE_INTERACTIVE;
            panel->flags = (panel->flags & ~0x38) | 0x18;
        }
        break;
    case ITF_SOUND_SELECTION_STATE_INTERACTIVE:
        if ((flags & 0x38) == 0x20 && SelectItem(panel) == ITF_MES_SELECTION_COMMITTED) {
            selection->glyphChain = UnchainFontHandleUnit(selection->glyphChain,
                selection->selectedIndex, selection->rowCount);
            if ((flags & 0xC00) == 0x800) {
                panel->flags |= 0xC00;
            }
            selection->selectedIndex = itfMesFindOptionBitIndex(selection->selectedIndex, selection->panelValue);
            selection->optionCount = 0;
            ForceSetCurCtr(&panel->fade, BTL_FADE_PHASE_ALPHA_FALLING, 0x7F, 0);
            panel->flags = (panel->flags & ~0x38) | 0x28;
            selection->glyphColorIntensity = 0x80;
            selection->selectionState = ITF_SOUND_SELECTION_STATE_SELECTION_FADE;
        }
        break;
    case ITF_SOUND_SELECTION_STATE_SELECTION_FADE:
        selection->glyphColorIntensity -= 16;
        if (selection->glyphColorIntensity <= 0) {
            selection->glyphColorIntensity = 0;
            selection->selectionState = ITF_SOUND_SELECTION_STATE_RETURNING;
            InitializeCurCtr(&panel->fade, 0);
        }
        ChangeFontAlpha(selection->glyphChain, selection->glyphColorIntensity);
        return;
    case ITF_SOUND_SELECTION_STATE_RETURNING:
        if (entry->glyphChain != NULL && entry->y < 0xAF8) {
            itfMesShiftPanelVertically(panel, 64);
            return;
        }
        selection->selectionState = ITF_SOUND_SELECTION_STATE_COMPLETE;
        break;
    }
}

void itfMesShiftPanelVertically(instanceMes_tag *panel, s32 dy) {
    ItfMesBlk14 *origin = &panel->blk14;
    ItfMesEntryBlock *pos = &panel->entryBlock;
    ItfMesBlkA4 *place = &panel->blkA4;
    pos->y += dy;
    ChangeFontPosition(pos->glyphChain, 0, dy);
    if (place->sprite != 0) {
        itfPnlSetPositionOffset(place->sprite, 0, dy, 0, 0, 0);
    }
    if (place->frame != 0) {
        itfPnlSetPositionOffset(place->frame, 0, dy, 0, dy, 0);
    }
    if (origin->glyphChain != NULL) {
        origin->y += dy;
        ChangeFontPosition(origin->glyphChain, 0, dy);
    }
}

void SelectItemMove(ItfMesBlk40 *obj, s32 dir);

ItfMesSelectionCommitResult SelectItem(instanceMes_tag *panel) {
    ItfMesBlk40 *sel = &panel->blk40;
    s32 dir = 0;
    s32 index;
    if (dds3PadButtonState.prev & 2) {
        if (sel->selectedIndex != 0) {
            dir = -1;
        } else if (dds3PadButtonState.prev < 0) {
            dir = -1;
        }
    } else if (dds3PadButtonState.next & 2) {
        if (sel->selectedIndex != sel->rowCount - 1 || dds3PadButtonState.next < 0) {
            dir = 1;
        }
    }
    if (dir != 0) {
        SelectItemMove(sel, dir);
        InitializeCurCtr(&panel->fade, 1);
    }
    if (dds3PadButtonState.confirm < 0) {
        p2sdPlaySE(8, 0x7F, 0x3F);
        return ITF_MES_SELECTION_COMMITTED;
    }
    if (sel->optionCount > 0 && (index = SelectItemDefKey(sel)) >= 0) {
        p2sdPlaySE(8, 0x7F, 0x3F);
        if (index != sel->selectedIndex) {
            ChangeFontColor(sel->glyphChain, sel->selectedIndex, sel->rowCount, 0);
            ChangeFontColor(sel->glyphChain, index, sel->rowCount, 1);
            sel->selectedIndex = index;
            sel->savedIndex = index;
        }
        return ITF_MES_SELECTION_COMMITTED;
    }
    return ITF_MES_SELECTION_NOT_COMMITTED;
}

void SelectItemMove(ItfMesBlk40 *obj, s32 dir) {
    s32 cur = obj->selectedIndex;
    u16 maxv;
    ChangeFontColor(obj->glyphChain, cur, obj->rowCount, 0);
    if (dir < 0) {
        cur = cur - 1;
        if (cur < 0) {
            cur = obj->rowCount - 1;
        }
        maxv = obj->rowCount;
    } else {
        cur = cur + 1;
        if (cur >= obj->rowCount) {
            cur = 0;
        }
        maxv = obj->rowCount;
    }
    ChangeFontColor(obj->glyphChain, cur, (s16)maxv, 1);
    obj->selectedIndex = cur;
    obj->savedIndex = cur;
    p2sdPlaySE(1, 0x7F, 0x3F);
}

s32 SelectItemDefKey(ItfMesBlk40 *selection) {
    s32 i;

    for (i = 0; i < selection->optionCount; i++) {
        ItfMesOption *option = &selection->options[i];

        if (dds3PadButtonState.buttons[option->id] < 0) {
            s32 prefixLength = option->value;
            s32 rank = 0;
            u32 mask = selection->panelValue;

            if (prefixLength > 0) {
                s32 remaining = prefixLength;
                do {
                    if ((mask & 1) == 0) {
                        rank++;
                    }
                    mask >>= 1;
                } while (--remaining != 0);
            }
            if ((mask & 1) == 0) {
                return rank;
            }
        }
    }
    return -1;
}


void CalcCurCtr(instanceMes_tag *panel) {
    BtlFade *fade = &panel->fade;
    s32 minimumAlpha;
    if (fade->kind != 0) {
        if (fade->timer > 0) {
            fade->timer -= 8;
        }
        switch (fade->phase) {
        case BTL_FADE_PHASE_ALPHA_RISING:
            fade->alpha += 8;
            if (fade->alpha >= 0xFF) {
                fade->phase = BTL_FADE_PHASE_ALPHA_FALLING;
                fade->alpha = 0xFF;
                fade->timer = 0x80;
            }
            break;
        case BTL_FADE_PHASE_ALPHA_FALLING:
            fade->alpha -= 8;
            minimumAlpha = (fade->kind & 1) ? 0x20 : 0x40;
            if (fade->alpha <= minimumAlpha) {
                fade->alpha = minimumAlpha;
                fade->phase = BTL_FADE_PHASE_ALPHA_RISING;
            }
            break;
        }
    }
}

extern void itfMesRenderActivePanelSprites(instanceMes_tag *);
extern void itfDrawSoundSelectorFadeLayers(instanceMes_tag *);
extern void itfDrawSelectedSoundRow(instanceMes_tag *);
extern void itfDrawSoundSelectorOptionPanel(instanceMes_tag *);

/* Draw the panel's glyph layers, advance its selection state and fade. */
void itfUpdateSoundSelectorPanel(instanceMes_tag *panel) {
    u32 flags = panel->flags;
    ItfMesBlk40 *selection;
    _FRQ *glyph;

    itfMesMngManager.flags &= ~2;
    itfMesRenderActivePanelSprites(panel);
    glyph = panel->blk14.glyphChain;
    if (!(flags & 0x10000) && glyph != 0) {
        frFontDrawGlyphInDefaultMode(glyph);
    }
    glyph = panel->entryBlock.glyphChain;
    if (!(flags & 0x20000) && (flags & 7) >= 3) {
        if (frFontDrawGlyphInDefaultMode(glyph) > 0) {
            if ((panel->flags & 7) != 4) {
                panel->fade.textAdvanceFrameCount = 0;
                panel->flags = (panel->flags & ~7) | 4;
            }
        }
    }
    glyph = panel->blk40.glyphChain;
    if (!(flags & 0x40000)) {
        flags &= 0x38;
        if (flags >= 0x18 && frFontDrawGlyphWithSharedFlags(glyph, 1) > 0) {
            if (flags == 0x18) {
                selection = &panel->blk40;
                if (selection->selectedIndex == -1) {
                    selection->selectedIndex = 0;
                    selection->savedIndex = 0;
                }
                ChangeFontColor(selection->glyphChain, selection->selectedIndex, selection->rowCount, 1);
                panel->flags = (panel->flags & ~0x38) | 0x20;
                panel->fade.kind = 2;
            }
        }
    }
    if (panel->fade.kind & 1) {
        itfDrawSoundSelectorFadeLayers(panel);
    }
    if (panel->fade.kind & 2) {
        if (panel->panelMode == ITF_MES_PANEL_MODE_NO_SPRITE_TRANSITION) {
            itfDrawSoundSelectorOptionPanel(panel);
        } else {
            itfDrawSelectedSoundRow(panel);
        }
    }
}

void itfMesRenderActivePanelSprites(instanceMes_tag *panel) {
    ItfMesBlkA4 *place;
    if (panel->flags & 0x80000) {
        return;
    }
    place = &panel->blkA4;
    if ((panel->flags & 0x300) >= 0x100) {
        if (panel->panelMode != ITF_MES_PANEL_MODE_NO_SPRITE_TRANSITION) {
            itfPnlDraw(place->sprite, &sdfGrOtTable[panel->drawSurfaceIndex]);
        }
        itfMesMngManager.flags |= 2;
        if (place->overlay != 0) {
            itfPnlDraw(place->overlay, &sdfGrOtTable[panel->drawSurfaceIndex]);
        }
    }
    if (D_00357D80.window != NULL && D_00357D80.window->state == panel && place != 0) {
        DebugDrawWindowLine(place->sprite);
    }
}

extern DrawColorRec D_00357FC8[];
#include "sdf_packet_alloc.h"
extern void itfDrawThreePanelBorderBands(DrawVertex *bounds, DrawColorRec *color, u32 tail, s32 borderWidth, sdfDrawTag_t *command);
extern void itfMiscDrawSpritePrim2(DrawVertex *, DrawColorRec *, DrawColorRec *, u32, s32, sdfDrawTag_t *);
extern void itfMiscDrawAlphaPrim(sdfDrawTag_t *, s32, s32);

void itfDrawSelectedSoundRow(instanceMes_tag *panel) {
    DrawColorRec uv;
    DrawColorRec color;
    DrawVertex bounds[2];
    ItfMesBlk40 *selection = &panel->blk40;
    BtlFade *fade = &panel->fade;
    s32 selected = selection->savedIndex;
    sdfDrawTag_t *list;
    s32 x;
    s32 y;
    s32 expansion;
    s32 rightExpansion;
    s32 verticalExpansion;
    sdfGrOt_t *surface;

    if (selected == -1) {
        return;
    }
    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    x = selection->x;
    y = (s32)selection->y - ((selection->rowCount * 21 - 21) << 3) + selected * 0xA0;
    bounds[0].x = x - 0x1D0;
    bounds[0].y = y + 0x10;
    bounds[1].x = x + frFontMeasureGlyphStripWidth(selected, selection->glyphChain) + 0x1D0;
    bounds[1].y = bounds[0].y + 0x80;
    itfDrawThreePanelBorderBands(bounds, D_00357FC8, panel->renderValue, 0x1D0, list);

    bounds[0].x = x;
    bounds[0].y = y + 8;
    bounds[1].x = x + 0x60;
    bounds[1].y = y + 0x88;
    uv.components[0] = 0x150;
    uv.components[1] = 0x2F0;
    uv.components[2] = 0x1B0;
    uv.components[3] = 0x3F0;
    color.components[0] = 0x80;
    color.components[1] = 0x80;
    color.components[2] = 0x80;
    color.components[3] = 0x26;
    itfMiscDrawSpritePrim((DrawVertex *)bounds, (DrawColorRec *)&uv, (DrawColorRec *)&color, panel->renderValue,
                                  itfMesMngManager.windowTexture, 0, list);

    bounds[0].x = x - 0xF0;
    bounds[0].y = y + 0x28;
    bounds[1].x = x - 0x30;
    bounds[1].y = y + 0x78;
    uv.components[0] = 0x290;
    uv.components[1] = 0x10;
    uv.components[2] = 0x350;
    uv.components[3] = 0xC0;
    color.components[3] = fade->alpha;
    itfMiscDrawSpritePrim2(bounds, &uv, &color, panel->renderValue, 0, list);
    if (fade->timer > 0) {
        expansion = 0x80 - fade->timer;
        rightExpansion = expansion << 1;
        verticalExpansion = expansion >> 1;
        bounds[0].x -= expansion;
        bounds[0].y -= verticalExpansion;
        bounds[1].x += rightExpansion;
        bounds[1].y += verticalExpansion;
        color.components[3] = fade->timer;
        itfMiscDrawAlphaPrim(list, 1, 0);
        itfMiscDrawSpritePrim2(bounds, &uv, &color, panel->renderValue, 0, list);
        itfMiscDrawAlphaPrim(list, 0, 0);
    }
    surface = &sdfGrOtTable[panel->drawSurfaceIndex];
    surface->append(surface, list);
}

#ifdef NON_MATCHING
#include "common.h"
#include "itf_debug_panel_work.h"
#include "snd_sequence_volume_pan.h"
#include "snd_pad.h"
#include "itf_mes_window.h"
#include "sdf_packet_list.h"
#include "kwln.h"
#include "kwln_task_create.h"
#include "sdf_resource.h"
#include "itf.h"
#include "fr_font_temporary_entry_api.h"
#include "itf_mes_state_api.h"
#include "pcp_vu0.h"
#include "scr.h"
#include "sdf_sif_command.h"
#include "sdf_projection.h"
#include "itf_panel_draw.h"
#include "itf_panel_api.h"
#include "sdf.h"
#include "sdf_debug_api.h"
#include "sdf_packet_alloc.h"
#include "fr_font_position.h"
#include "fr_font_glyph_chain_count.h"
#include "fr_font_measure.h"

extern ItfMesGlobals itfMesMngManager;
extern sdfGrOt_t sdfGrOtTable[];
extern void itfDrawSoundSelectorOptionPanel(instanceMes_tag *);
extern void itfDrawThreePanelBorderBands(DrawVertex *bounds, DrawColorRec *color, u32 tail, s32 borderWidth, sdfDrawTag_t *command);

extern DrawColorRec itfSoundSelectorBorderColor;
/* Draws the sound selector's option panel border and row highlights behind the
 * selected row: it widens the stored selection extent to the widest glyph
 * strip, draws the three-band border and the two end caps tinted by the panel
 * fade, then one pair of side bars for every row, and queues the packets on the
 * panel's draw surface. Upstream: INCLUDE_ASM in game/code_0019DB88; written
 * from the retail routine, the widest row and each row's top and bottom read
 * once, as retail does. It differs from retail only in register allocation
 * and scheduling (retail spills the fade pointer and several row coordinates
 * to the frame). */
void itfDrawSoundSelectorOptionPanel(instanceMes_tag *panel) {
    DrawColorRec uv;
    DrawColorRec color;
    DrawVertex bounds[2];
    FrFontPosition position;
    ItfMesBlk40 *selection = &panel->blk40;
    BtlFade *fade = &panel->fade;
    s32 selected = selection->savedIndex;
    sdfDrawTag_t *list;
    sdfGrOt_t *surface;
    s32 i;
    s32 width;
    s32 left;
    s32 right;
    s32 top;
    s32 bottom;
    s32 repeatLeft;
    s32 repeatRight;

    if (selected == -1) {
        return;
    }
    list = (sdfDrawTag_t *)(u32)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    for (i = 0; i < frFontCountGlyphChainNodes(selection->glyphChain); i++) {
        width = frFontMeasureGlyphStripWidth(i, selection->glyphChain);
        if (selection->widestRowWidth < width) {
            frFontGetGlyphPositionAtIndex(&position, i, selection->glyphChain);
            selection->widestRowWidth = width;
            selection->widestRowX = position.x;
        }
    }
    frFontGetGlyphPositionAtIndex(&position, selected, selection->glyphChain);
    left = selection->widestRowX;
    right = left + selection->widestRowWidth;
    repeatLeft = left - 0x250;
    repeatRight = right + 0x250;
    top = position.y + 0x28;
    bottom = position.y + 0xA0;
    bounds[0].x = left - 0x370;
    bounds[0].y = position.y + 0x10;
    bounds[1].x = right + 0x370;
    bounds[1].y = position.y + 0xB8;
    itfDrawThreePanelBorderBands(bounds, &itfSoundSelectorBorderColor, panel->renderValue, 0x370, list);

    bounds[0].x = left - 0x140;
    bounds[0].y = top;
    bounds[1].x = left - 0x60;
    bounds[1].y = bottom;
    uv.components[0] = 0x410;
    uv.components[1] = 0xF0;
    uv.components[2] = 0x4F0;
    uv.components[3] = 0x1E0;
    color.components[0] = 0x80;
    color.components[1] = 0x80;
    color.components[2] = 0x80;
    color.components[3] = fade->alpha;
    itfMiscDrawSpritePrim(bounds, &uv, &color, panel->renderValue, itfMesMngManager.windowTexture, 0, list);

    bounds[0].x = right + 0x60;
    bounds[0].y = top;
    bounds[1].x = right + 0x140;
    bounds[1].y = bottom;
    uv.components[0] = 0x4F0;
    uv.components[1] = 0xF0;
    uv.components[2] = 0x410;
    uv.components[3] = 0x1E0;
    color.components[0] = 0x80;
    color.components[1] = 0x80;
    color.components[2] = 0x80;
    color.components[3] = fade->alpha;
    itfMiscDrawSpritePrim(bounds, &uv, &color, panel->renderValue, itfMesMngManager.windowTexture, 0, list);

    for (i = 0; i < frFontCountGlyphChainNodes(selection->glyphChain); i++) {
        frFontGetGlyphPositionAtIndex(&position, i, selection->glyphChain);
        top = position.y + 0x28;
        bottom = position.y + 0xA0;
        bounds[0].x = repeatLeft;
        bounds[0].y = top;
        bounds[1].x = left - 0x120;
        bounds[1].y = bottom;
        uv.components[0] = 0x2C0;
        uv.components[1] = 0xF0;
        uv.components[2] = 0x3F0;
        uv.components[3] = 0x1E0;
        color.components[0] = 0x80;
        color.components[1] = 0x80;
        color.components[2] = 0x80;
        color.components[3] = 0x80;
        itfMiscDrawSpritePrim(bounds, &uv, &color, panel->renderValue, itfMesMngManager.windowTexture, 0, list);

        bounds[0].x = right + 0x120;
        bounds[0].y = top;
        bounds[1].x = repeatRight;
        bounds[1].y = bottom;
        uv.components[0] = 0x3F0;
        uv.components[1] = 0xF0;
        uv.components[2] = 0x2C0;
        uv.components[3] = 0x1E0;
        color.components[0] = 0x80;
        color.components[1] = 0x80;
        color.components[2] = 0x80;
        color.components[3] = 0x80;
        itfMiscDrawSpritePrim(bounds, &uv, &color, panel->renderValue, itfMesMngManager.windowTexture, 0, list);
    }
    surface = &sdfGrOtTable[panel->drawSurfaceIndex];
    surface->append(surface, list);
}
#else
INCLUDE_ASM(const s32, "game/code_0019DB88", itfDrawSoundSelectorOptionPanel);
#endif

extern u8 D_00358008[];
extern u8 D_00357FF8[];
extern s32 D_00358018[];
extern void itfMiscDrawAlphaPrim(sdfDrawTag_t *, s32, s32);

/* Draw the sound selector frame, its fade layer and the expanding timer outline. */
void itfDrawSoundSelectorFadeLayers(instanceMes_tag *panel) {
    s32 bounds[4];
    BtlFade *fade = &panel->fade;
    sdfDrawTag_t *packet;
    s32 expansion;
    sdfGrOt_t *surface;

    bounds[0] = 0x1AA0;
    bounds[1] = 0xC60;
    bounds[2] = 0x1BD0;
    bounds[3] = 0xD58;
    packet = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(packet);
    D_00358018[3] = 0xFF;
    itfMiscDrawSpritePrim((DrawVertex *)bounds, (DrawColorRec *)D_00358008, (DrawColorRec *)D_00358018, panel->renderValue,
                                  itfMesMngManager.windowTexture, 0, packet);
    D_00358018[3] = fade->alpha;
    itfMiscDrawSpritePrim((DrawVertex *)bounds, (DrawColorRec *)D_00357FF8, (DrawColorRec *)D_00358018, panel->renderValue,
                                  itfMesMngManager.windowTexture, 0, packet);
    if (fade->timer > 0) {
        expansion = 0x80 - fade->timer;
        bounds[0] -= expansion * 2;
        bounds[1] -= expansion;
        bounds[2] += expansion * 2;
        bounds[3] += expansion;
        D_00358018[3] = fade->timer;
        itfMiscDrawAlphaPrim(packet, 1, 0);
        itfMiscDrawSpritePrim((DrawVertex *)bounds, (DrawColorRec *)D_00357FF8, (DrawColorRec *)D_00358018, panel->renderValue,
                                      itfMesMngManager.windowTexture, 0, packet);
        itfMiscDrawAlphaPrim(packet, 0, 0);
    }
    surface = &sdfGrOtTable[panel->drawSurfaceIndex];
    surface->append(surface, packet);
}


KwlnTaskUpdateResult CalcMsgMng_Process(dds3ProcessID_t *task) {
    mesflow_tag *node = itfMesMngManager.pool.activeHead;
    while (node != 0) {
        itfUpdateBattleDisplayAndFadeIndicator(node->state);
        node = node->next;
    }
    return KWLN_TASK_UPDATE_RETAIN_CALLBACK;
}

extern void DestructFlow(s32 window);
extern u32 dds3NullDrawProc(void);

KwlnTaskUpdateResult DrawMsgMng_Process(dds3ProcessID_t *task) {
    mesflow_tag *node;

    if (dds3NullDrawProc() != 0) {
        return KWLN_TASK_UPDATE_RETAIN_CALLBACK;
    }
    for (node = itfMesMngManager.pool.activeHead; node != NULL; node = node->next) {
        itfUpdateSoundSelectorPanel(node->state);
    }
    itfMesMngManager.drawTickCount++;
    return KWLN_TASK_UPDATE_RETAIN_CALLBACK;
}

void DrawMsgMng_Shutdown(dds3ProcessID_t *task) {
    mesflow_tag *node = itfMesMngManager.pool.activeHead;
    s32 window;
    while (node != 0) {
        window = node->index;
        node = node->next;
        DestructFlow(window);
    }
    sdfTexReleaseReferenceViaHandler(itfMesMngManager.windowTexture);
    itfMesMngManager.windowTexture = NULL;
}


extern sdfGrOt_t D_00325708;
extern s32 D_00358028[];
extern u8 D_003BB230[5];
extern u8 D_003BB238[5];
extern void itfMiscDrawLineStripPrim(DrawVertex *vertices, DrawColorRec *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, sdfDrawTag_t *command);

void DebugDrawWindowLine(itfpanel_t *sprite) {
    s32 vertices[4][2] = {
        {sprite->left, sprite->top},
        {sprite->right, sprite->top},
        {sprite->right, sprite->bottom},
        {sprite->left, sprite->bottom}
    };
    sdfDrawTag_t *list;

    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(list);
    itfMiscDrawLineStripPrim((DrawVertex *)vertices, (DrawColorRec *)D_00358028, D_003BB230, D_003BB238, 5, 0xFFFFFF, list);
    D_00325708.append(&D_00325708, list);
}

void DebugMoveWinVert(ItfMesBlkA4 *panel, s32 boundsMode) {
    itfpanel_t *sprite = panel->sprite;
    s32 *bounds;
    s32 dx;
    s32 dy;

    if (sprite != NULL) {
        bounds = panel->bounds;
        if (dds3PadButtonState.coarseDown & 2) {
            dx = -16;
        } else {
            dx = ((u8)dds3PadButtonState.coarseUp << 3) & 0x10;
        }
        if (dds3PadButtonState.secondaryUp & 2) {
            dy = -8;
        } else {
            dy = ((u8)dds3PadButtonState.secondaryDown << 2) & 8;
        }
        if (dds3PadButtonState.secondaryCross != 0) {
            dx *= 8;
            dy *= 8;
        }
        if (dx != 0 || dy != 0) {
            switch (boundsMode) {
            case 0:
                itfPnlSetPositionOffset(sprite, dx, dy, 0, 0, 0);
                bounds[0] += dx;
                bounds[1] += dy;
                break;
            case 1:
                itfPnlSetPositionOffset(sprite, 0, 0, dx, dy, 0);
                bounds[2] += dx;
                bounds[3] += dy;
                break;
            case 2:
                itfPnlSetPositionOffset(sprite, dx, dy, dx, dy, 0);
                bounds[0] += dx;
                bounds[1] += dy;
                bounds[2] += dx;
                bounds[3] += dy;
                break;
            }
        }
    }
}

/* Step the panel fade limit by pad input: one per press, ten with the fast modifier held. */
void DebugMoveWinAlpha(ItfMesBlkA4 *panel) {
    s32 step;

    if (dds3PadButtonState.coarseDown & 2) {
        step = -1;
    } else {
        step = (dds3PadButtonState.coarseUp & 2) > 0;
    }
    if (dds3PadButtonState.secondaryUp & 2) {
        step = -1;
    } else if (dds3PadButtonState.secondaryDown & 2) {
        step = 1;
    }
    if (dds3PadButtonState.secondaryCross != 0) {
        step *= 10;
    }
    if (step != 0) {
        s32 alpha = (panel->fadeLimit + step) & 0xFF;

        panel->fadeLimit = alpha;
        if (panel->sprite != NULL) {
            panel->sprite->alpha = alpha;
        }
    }
}

mesflow_tag *DebugChangeInstance(mesflow_tag *node) {
    s32 index = 0;
    s32 count = itfMesMngManager.activeWindowCount;

    if (count <= 0) {
        return NULL;
    }
    for (;;) {
        if (node != NULL) {
            node = node->previous;
        }
        if (node == NULL) {
            node = itfMesMngManager.pool.activeTail;
        }
        if ((node->state)->blkA4.sprite != NULL) {
            break;
        }
        index++;
        if (count < index) {
            node = NULL;
            break;
        }
    }
    return node;
}


extern u8 D_00358038[];

extern char D_003A14F0[]; /* "TestMsgMngC" */

extern char D_003A1500[]; /* "TestMsgMngD" */

extern s32 D_003BA8EC;

extern dds3ProcessID_t *scrStartScript2(u32 priority, const BfFlw0Header *script, ScrProcedureIndex procedureIndex);


extern void TestMsgReloadTexture(void);

extern s32 TestMsgMngC_Process(dds3ProcessID_t *task);

extern void itfDebugPrintMark(void);

extern s32 TestMsgMngD_Process(dds3ProcessID_t *task);


void itfDebugMesMngInitialize(void) {
    D_003BA8EC = 0x80FFFFFF;
    TestMsgReloadTexture();
    itfMesMngSetKutiCallback(((scrProcessWork_s *)dds3GetProcessData(scrStartScript2(0x3E8, (const BfFlw0Header *)D_00358038, 0)))->messageWindowIndex, itfDebugPrintMark);
    dds3StartProcess(D_003A14F0, 0x3EF, 0, 0, TestMsgMngC_Process, 0, 0);
    dds3StartProcess(D_003A1500, 0x2AFE, 0, 0, TestMsgMngD_Process, 0, 0);
}

void TestMsgReloadTexture(void) {
    if (sndTestMessageTexture != 0) {
        sdfTexReleaseReferenceViaHandler(sndTestMessageTexture);
        sndTestMessageTexture = 0;
    }
    sndTestMessageResourceIndex = (sndTestMessageResourceIndex + 1) & 3;
    if (sndTestMessageResourceIndex != 3) {
        sndTestMessageTexture = itfMiscLoadTexture(D_00358308[sndTestMessageResourceIndex]);
    }
}

s32 TestMsgMngC_Process(dds3ProcessID_t *task) {
    if (D_00324530[0] < 0) {
        TestMsgReloadTexture();
    }
    return 0;
}


extern sdfGrOt_t D_003255A8;

extern u8 D_00358318[];

extern u8 D_00358328[];

extern u8 D_00358338[];


s32 TestMsgMngD_Process(dds3ProcessID_t *task) {
    sdfDrawTag_t *mem;
    if ((sndTestMessageTexture != 0) && (sndTestMessageResourceIndex != 3)) {
        mem = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
        sdfDrawTagInit(mem);
        itfMiscDrawAlphaPrim(mem, 0, 0);
        itfMiscDrawSpritePrim((DrawVertex *)D_00358318, (DrawColorRec *)D_00358328, (DrawColorRec *)D_00358338, 0xFFF, sndTestMessageTexture, 0, mem);
        D_003255A8.append(&D_003255A8, mem);
        return 0;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A14F0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A1500);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A1510);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A1528);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A1540);

void itfDebugPrintMark(void) {
    printf("********* AAAA ********\n");
}

typedef struct ItfFovPanelWork {
    u8 pad00[8];
    f32 degrees;
    u8 pad0C[4];
} ItfFovPanelWork;

extern ItfFovPanelWork D_003D73C0;
extern u64 *dds3PrimFrameRect(s32, s32, s32, s32, s32, u32, u32);
extern s32 pbCalcFovDegrees(f32 *, f32, f32, f32, f32);

s32 pbDebugPrintPanel(void) {
    SifCommand packet;
    sdfDrawTag_t *list;
    f32 radiansToDegrees = 57.2957795f;

    list = dds3FontReqAllocDrawTag();
    sdfDrawTagLink(list,
                    (u32)dds3PrimFrameRect(0x8500, 0x79C0, 0xFEFFFF,
                                  0xA80, 0x120, 0x60000000, 0x40806020));
    sdfPktInit(&packet, 0x85C0, 0x7A20, 0xFF0000, 0);
    sdfDrawTagLink(list,
                    (u32)sdfDevPrintfEx(&packet, "FOVY: %6.2f",
                        dds3ScreenParam.camera.fov * radiansToDegrees));
    D_00325708.append(&D_00325708, list);
    if (dds3PadButtonState.secondaryCancel < 0) {
        return -1;
    }
    D_003D73C0.degrees = dds3ScreenParam.camera.fov * radiansToDegrees;
    if (pbCalcFovDegrees(&D_003D73C0.degrees, 1.0f, 89.0f, 0.1f, 1.0f) != 0) {
        dds3ScreenParam.camera.fov = D_003D73C0.degrees * 0.0174532925f;
    }
    return 0;
}

s32 pbCalcFovDegrees(f32 *angleDegrees, f32 minimum, f32 maximum, f32 coarseStep, f32 fineStep) {
    f32 current = *angleDegrees;
    s32 changed = 0;

    if (dds3PadButtonState.coarseUp < 0) {
        if (current < maximum) {
            current += coarseStep;
        } else {
            current = minimum;
        }
        changed = 1;
    } else if (dds3PadButtonState.coarseUp & 2) {
        if (current < maximum) {
            current += coarseStep;
            changed = 1;
        }
    } else if (dds3PadButtonState.coarseDown < 0) {
        if (minimum < current) {
            current -= coarseStep;
        } else {
            current = maximum;
        }
        changed = 1;
    } else if (dds3PadButtonState.coarseDown & 2) {
        if (minimum < current) {
            current -= coarseStep;
            changed = 1;
        }
    } else if (fineStep != 0.0f) {
        if (dds3PadButtonState.fineUp < 0) {
            if (current < maximum) {
                current += fineStep;
                if (maximum < current) {
                    current = maximum;
                }
            } else {
                current = minimum;
            }
            changed = 1;
        } else if (dds3PadButtonState.fineUp & 2) {
            if (current < maximum) {
                current += fineStep;
                if (maximum < current) {
                    current = maximum;
                }
                changed = 1;
            }
        } else if (dds3PadButtonState.fineDown < 0) {
            if (minimum < current) {
                current -= fineStep;
                if (current < minimum) {
                    current = minimum;
                }
            } else {
                current = maximum;
            }
            changed = 1;
        } else if (dds3PadButtonState.fineDown & 2) {
            if (minimum < current) {
                current -= fineStep;
                if (current < minimum) {
                    current = minimum;
                }
                changed = 1;
            }
        }
    }
    if (changed != 0) {
        *angleDegrees = current;
        return 1;
    }
    return 0;
}

extern ItfDebugPanelWork D_003D73D0;
extern const char *D_00358348[6];
extern s32 D_003BA910[2];
extern u8 dds3ScreenBlurFlag;
extern s16 dds3ScreenBlurAlpha;
extern s16 dds3ScreenBlurScale;
extern sdfGrOt_t kwlnPositionedTextSurface;
extern char D_003BB260[];
extern char D_003BB268[];
extern char D_003BB270[];
extern char D_003BB278[];
extern char D_003BB280[];
extern char D_003BB288[];

/* Draw the blur settings and handle selection, editing and cancellation. */
s32 pbDebugPrintPanel2(void) {
    SifCommand packet;
    sdfDrawTag_t *list;
    s32 row;
    s32 y;
    s16 previous;
    s32 highlight;

    list = dds3FontReqAllocDrawTag();
    sdfDrawTagLink(list,
        (u32)dds3PrimFrameRect(0x8290, 0x79A8, 0xFEFFFF, 0xC60, 0x1B0, 0x60000000, 0x40806020));
    sdfPktInit(&packet, 0x82C0, 0x79C0, 0xFF0000, 0);
    sdfDrawTagLink(list, (u32)sdfDevPrintfEx(&packet,
        "BLUR:        %s", dds3ScreenBlurFlag ? D_003BB260 : D_003BB268));

    y = 0x7A20;
    if (D_003D73D0.editing == 0) {
        sdfPktInit(&packet, 0x82C0, D_003D73D0.selection * 0x60 + 0x7A20, 0xFF0000, 0);
        D_003D73D0.blink++;
        if (D_003D73D0.blink < 32 || (D_003D73D0.blink & 31) < 18) {
            sdfDrawTagLink(list, (u32)sdfDevPrintfEx(&packet, D_003BB270));
        }
    }

    for (row = 0; row != 3; row++) {
        sdfPktInit(&packet, 0x82C0, y, 0xFF0000, 0);
        sdfDrawTagLink(list,
            (u32)sdfDevPrintfEx(&packet, D_003BB278, D_00358348[row]));
        highlight = 0;
        if (D_003D73D0.editing != 0 && row == D_003D73D0.selection) {
            highlight = 6;
        }
        sdfPktInit(&packet, 0x8800, y, 0xFF0000, highlight);
        switch (row) {
        case 0:
            sdfDrawTagLink(list,
                (u32)sdfDevPrintfEx(&packet, D_003BB280, dds3ScreenBlurAlpha));
            break;
        case 1:
            sdfDrawTagLink(list,
                (u32)sdfDevPrintfEx(&packet, D_003BB280, dds3ScreenBlurScale));
            break;
        case 2:
            sdfDrawTagLink(list,
                (u32)sdfDevPrintfEx(&packet, D_003BB288, D_003BA910[0], D_003BA910[1]));
            break;
        }
        y += 0x60;
    }
    sdfDrawTagLink(list,
        (u32)dds3PrimFrameRect((D_003BA910[0] * 16) + 0x7FC0, (D_003BA910[1] * 8) + 0x7FE0,
            0xFF0000, 0x80, 0x40, 0x80008080, 0x80000000));
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, list);

    if ((s8)dds3PadButtonState.secondaryTriangle < 0) {
        dds3ScreenBlurFlag ^= 1;
    }
    if (D_003D73D0.editing == 0) {
        previous = D_003D73D0.selection;
        if ((u8)dds3PadButtonState.secondaryUp & 2) {
            D_003D73D0.selection--;
            if (D_003D73D0.selection < 0) {
                D_003D73D0.selection = 2;
            }
        }
        if ((u8)dds3PadButtonState.secondaryDown & 2) {
            D_003D73D0.selection++;
            if (D_003D73D0.selection >= 3) {
                D_003D73D0.selection = 0;
            }
        }
        if (previous != D_003D73D0.selection) {
            D_003D73D0.blink = 0;
        }
        if (dds3PadButtonState.secondaryCross < 0) {
            D_003D73D0.editing ^= 1;
            D_003D73D0.blink = 0;
        }
        if ((u8)dds3PadButtonState.secondaryCancel & 2) {
            return -1;
        }
    } else {
        if (D_003D73D0.selection == 2) {
            if (dds3PadButtonState.secondaryUp != 0) {
                D_003BA910[1] -= 16;
                if (D_003BA910[1] < -224) {
                    D_003BA910[1] = -224;
                }
            }
            if (dds3PadButtonState.secondaryDown != 0) {
                D_003BA910[1] += 16;
                if (D_003BA910[1] > 224) {
                    D_003BA910[1] = 224;
                }
            }
            if (dds3PadButtonState.coarseDown != 0) {
                D_003BA910[0] -= 16;
                if (D_003BA910[0] < -256) {
                    D_003BA910[0] = -256;
                }
            }
            if (dds3PadButtonState.coarseUp != 0) {
                D_003BA910[0] += 16;
                if (D_003BA910[0] > 256) {
                    D_003BA910[0] = 256;
                }
            }
        } else if (D_003D73D0.selection == 0) {
            if ((u8)dds3PadButtonState.coarseDown & 2) {
                dds3ScreenBlurAlpha -= 10;
                if (dds3ScreenBlurAlpha < 0) {
                    dds3ScreenBlurAlpha = 0;
                }
            }
            if ((u8)dds3PadButtonState.coarseUp & 2) {
                dds3ScreenBlurAlpha += 10;
                if (dds3ScreenBlurAlpha > 255) {
                    dds3ScreenBlurAlpha = 255;
                }
            }
        } else {
            if ((u8)dds3PadButtonState.coarseDown & 2) {
                dds3ScreenBlurScale -= 10;
                if (dds3ScreenBlurScale < -255) {
                    dds3ScreenBlurScale = -255;
                }
            }
            if ((u8)dds3PadButtonState.coarseUp & 2) {
                dds3ScreenBlurScale += 10;
                if (dds3ScreenBlurScale > 255) {
                    dds3ScreenBlurScale = 255;
                }
            }
        }
        if (dds3PadButtonState.secondaryCancel < 0) {
            D_003D73D0.editing ^= 1;
            D_003D73D0.blink = 0;
        }
    }
    return 0;
}

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB230);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB238);

INCLUDE_SDATA(const s32, "game/code_0019DB88", sndTestMessageResourceIndex);

INCLUDE_SDATA(const s32, "game/code_0019DB88", sndTestMessageTexture);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB248);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB250);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB258);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB260);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB268);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB270);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB278);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB280);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB288);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB290);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB298);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB2A0);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB2A8);

INCLUDE_SDATA(const s32, "game/code_0019DB88", D_003BB2B0);
