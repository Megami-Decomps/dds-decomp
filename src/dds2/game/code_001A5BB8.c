#include "fld_area_work.h"
#include "sdf_packet_list.h"
#include "common.h"
#include "itf_mes_window.h"
#include "fr_font_measure.h"
#include "eff_resource_slots.h"
#include "sdf_chip.h"
#include "kwln.h"
#include "sdf_resource.h"
#include "pcp_vu0.h"
#include "btl_scene_fade.h"
#include "btl_resource.h"
#include "eff.h"
#include "itf.h"
#include "itf_panel_draw.h"
#include "itf_panel_api.h"
#include "btl_state.h"
#include "btl_ui.h"
#include "sdf.h"
#include "sdf_projection.h"
#include "btl_action.h"
#include "scr.h"
#include "dat_state.h"
#include "mnu_result.h"
#include "dat_command.h"
#include "sdf_sif_command.h"
#include "mdl_resource_table.h"
#include "kwln_task_lifecycle.h"

extern s32 func_001ABB10(BtlUnit *, s32);

/* Named and indexed views of the same four signed 32-bit color channels. */
typedef union UiQuadColor {
    struct {
        s32 red;
        s32 green;
        s32 blue;
        s32 alpha;
    };
    s32 channels[4];
} UiQuadColor;
typedef char UiQuadColorSizeCheck[sizeof(UiQuadColor) == 0x10 ? 1 : -1];

extern const UiQuadColor D_00414D50;

extern s32 dds3FindEntryIndex();

extern DatPartyRecord *btlGetIndexedPartyEntryRecord(s32);

extern KwlnTask *func_00101820(u32 priority);

extern const char *D_003B4CF8[];

extern u32 sndTestMessageResourceIndex;

extern SdfTex *sndTestMessageTexture;

extern SdfTex *itfLoadTextureFromAsset(const char *path);

extern u64 D_004366E8;

extern s32 btlRuntime;

extern s32 D_00435DE0;

extern s32 D_00435DF0;

extern s32 D_00435DE4;

extern s32 D_00435DF8;

extern s32 btlGetRuntime(void);

extern void func_00101968(KwlnTask *parent, KwlnTask *child);

extern s8 D_0037F530[];

extern u8 D_003B4DC0[];

extern s32 D_00435E00;

extern void *D_003B4E88[];

typedef struct EncBgEntry {
    s32 unk00;
    s16 unk04;
    s16 unk06;
    s16 unk08;
    s16 unk0A;
} EncBgEntry;

typedef struct EncBgRow {
    s32 unk00;
    EncBgEntry entries[64];
} EncBgRow;

extern EncBgRow *D_00435E14;

extern s32 fldConsumeNextSceneRequest(s32 *, s32 *);

typedef struct BattleItemDrop {
    u16 id;
    u8 count;
    u8 pad03;
} BattleItemDrop;

typedef struct BattleController {
    u8 pad_000[0x214];
    s32 frame;
    u32 flags;
    u32 flags21C;
    u8 pad_220[0x28];
    ActionStateLink *taskHead; /* 0x248 */
    BtlUnit *actors;
    u8 pad_250[0x1E];
    u8 mode; /* 0x26E: mode 3 scales defeat experience in DDS2. */
    u8 pad26F;
    u16 variant; /* 0x270 */
    u8 pad_272[0x22];
    s32 adjustmentRecordIndex; /* 0x294 */
    s32 adjustmentGroupIndex;  /* 0x298 */
    s32 adjustmentEntryIndex;  /* 0x29C */
    u8 pad_2A0[0x24];
    KwlnTask *drawTask;
    u8 pad2C8[0x14];
    BattleItemDrop itemDrops[3];
    s32 moneyEarned; /* 0x2E8 */
    u32 rewardMacca; /* 0x2EC */
    s32 experienceEarned; /* 0x2F0 */
    s32 epEarned; /* 0x2F4 */
    s32 rewardAp; /* 0x2F8 */
    u16 specialEnemyDefeats; /* 0x2FC: defeated enemy kinds 100 through 103. */
    u8 pad2FE[0x3DE];
    s32 (*commandRangeOverride)(BtlUnit *, s32);
} BattleController;

extern s32 D_003B4F70[];

extern s32 datCalculateCommandBaseValue(DatPartyRecord *, s32);

extern DatEnemyRecord *datEnemyRecords;

extern char D_00415158[];

extern char D_004150B0[]; /* "btl:hunt ep=%d[id=%X]\n" */

extern void btlBossDebugPrintf(const char *, ...);

extern s8 effSharedRandomState[];

extern void *D_003B4E40[];

extern s32 D_003B4F78[];

extern s32 D_003B4F74[];

extern f32 D_003B4E28[];

extern s32 btlCheckSpecialAbility(DatPartyRecord *, s32);

extern s32 mdlFlagTest(s32);

extern u16 mnuGetPartyEntryCurrentId(DatPartyRecord *);

extern s32 btlDoesEnabledStatusMatchCurrentId(DatPartyRecord *, u32);

extern s32 evtCheckValueThreshold(s32, s32);

extern void mdlFlagSet(s32);

extern void mdlFlagClear(s32);

extern char D_00415638[]; /* "btl:hunt mp rec[%d]\n" */

extern ItfMesGlobals itfMesWork;

extern void itfMesDestroyWindow(s32 arg0);

extern void sdfTexReleaseReferenceViaHandler(SdfTex *texture);

extern s32 sndUpdateTestMsgTask(KwlnTask *task);

extern SdfPoolNode D_003805A8;

extern u8 D_003B4D08[];

extern u8 D_003B4D18[];

extern u8 D_003B4D28[];

extern s32 sdfAllocPacketAligned(s32 size);

extern void itfSendTablePacket(SdfListHead *list, s32 context, s32 mode);

extern void itfQueueTextureBoundQuadPacket(void *, void *, void *, s32, SdfTex *, s32, SdfListHead *);

extern s32 func_0019DBA8(s32 row, FrFontGlyph *glyph);

extern UiSprite *func_001A1858(s32, u32);


extern void itfMesOffsetNodeChain(FrFontGlyph *node, s32 dx, s32 dy);

extern void itfMesSetRowItemFlag();

extern void sndSetSequenceVolumePan();

extern void sndStepSequenceIndex(ItfMesBlk40 *sel, s32 dir);

/* Option IDs index the same signed-byte bank used by the named controls. */
typedef struct SndPad {
    u8 pad00[0x20];
    union {
        s8 buttons[0x20];
        struct {
            u8 pad20;
            s8 confirm;
            u8 pad22[4];
            s8 prev;
            s8 next;
            u8 pad28[9];
            s8 unk31;
            s8 unk32;
            s8 cancel;
            s8 coarseDown;
            s8 coarseUp;
            s8 unk36;
            s8 unk37;
            s8 fineDown;
            u8 pad39;
            s8 fineUp;
            u8 pad3B[5];
        };
    };
} SndPad;

extern SndPad D_0037F510;

extern s32 func_001A6AB8(ItfMesBlk40 *);

extern void itfResetBattleFadeState(BtlFade *, s32);

typedef struct UiOwnerRef { u8 pad0[0xC]; ItfMesState *owner; } UiOwnerRef;

extern SdfPoolNode kwlnDrawSurfaces[];

extern UiOwnerRef *D_003B4778[];

extern void itfBuildAndSubmitPanelPacket(UiSprite *sprite, SdfPoolNode *surface);

extern void func_001A7798(UiSprite *sprite);

#define BTL_ENTRY_STATUS_MASK 0x7FFF

extern u32 datComputeSkillBoostedMaxHp(DatPartyRecord *);

extern u32 datComputeSkillBoostedMaxMp(DatPartyRecord *);

/* Enable context rendering for every font object in the linked chain. */
void frFontEnableNodeContextModes(FrFontGlyph *fontObject) {
    for (; fontObject != NULL; fontObject = fontObject->previous) {
        frFontEnableContextMode(fontObject);
    }
}

void itfMesInitializePanelPlacementSprite(ItfMesState *panel) {
    ItfMesEntryBlock *pos = &panel->entryBlock;
    ItfMesBlkA4 *place = &panel->blkA4;
    s32 top;
    if (place->sprite == 0) {
        place->sprite = func_001A1858(6, (u32)itfMesWork.windowTexture);
        if (place->frame != 0) {
            place->sprite->unk20 = panel->blk14.glyphChain->x + func_0019DBA8(0, panel->blk14.glyphChain);
        }
    }
    top = pos->y + place->bounds[1];
    itfSetPanelLayoutAndNotify(place->sprite, pos->x + place->bounds[0], top, pos->x + place->bounds[2], pos->y + place->bounds[3], panel->renderValue);
    place->sprite->screenY = top;
    itfPanelUpdateValuesAndNotify(place->sprite, place->unk1C, place->unk20, place->unk24, 0);
    panel->flags = (panel->flags & ~0x300) | 0x100;
}

void itfMesCreatePanelOriginFrameWhenVisible(ItfMesState *panel) {
    ItfMesBlk14 *origin = &panel->blk14;
    ItfMesBlkA4 *place = &panel->blkA4;
    if (origin->glyphChain != NULL && !(panel->flags & 0x10000)) {
        if (place->frame == NULL) {
            s32 width = origin->glyphChain->advance * 16;
            place->frame = func_001A1858(7, (u32)itfMesWork.windowTexture);
            itfSetPanelLayoutAndNotify(place->frame, origin->x - 0x2D0, origin->y - 0x68, origin->x + width + 0x2D0, origin->y + 0x110, panel->renderValue);
            itfPanelUpdateValuesAndNotify(place->frame, 0x7F, 0x7F, 0x7F, 0);
        }
        panel->flags = (panel->flags & ~0x3000) | 0x1000;
    } else if (place->frame != 0) {
        panel->flags |= 0x3000;
    }
}

void itfResetCursorPositionAndState(ItfMesBlk14 *cursor, s32 resetPosition) {
    if (resetPosition != 0) {
        cursor->x = 0x280;
        cursor->y = 0xa10;
    }
    cursor->glyphChain = NULL;
    cursor->selectedIndex = 0xffff;
}

void itfMesResetCursorState(ItfMesEntryBlock *cur, s32 resetPos) {
    if (resetPos != 0) {
        cur->x = 0x4B0;
        cur->y = 0xAF8;
    }
    cur->glyphChain = NULL;
    cur->textState = 0;
    cur->unk11 = 0;
    cur->unk16 = 0;
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
    cursor->y = 0xC88;
    cursor->glyphChain = NULL;
    cursor->panelValue = 0;
    cursor->unk10 = 0;
    cursor->selectedIndex = -1;
    cursor->savedIndex = -1;
    cursor->rowCount = 0;
    cursor->unk18 = 0;
    cursor->unk1C = 0;
    cursor->unk20 = 0;
    cursor->optionCount = 0;
}

extern void func_001A6078(ItfMesBlkA4 *, s32, s32);

void itfResetWindowResourceBlock(ItfMesBlkA4 *block) {
    block->frame = NULL;
    block->sprite = NULL;
    block->overlay = NULL;
    func_001A6078(block, 0, 0);
}

/* Clear 32 words, from the end back toward the beginning of the buffer. */
void itfClearDrawStateWords(ItfMesTextSlots *slots) {
    s32 remaining;
    u32 *word;

    word = &slots->addresses[31];
    remaining = 0x1f;
    do {
        remaining = remaining - 1;
        *word = 0;
        word = word + -1;
    } while (-1 < remaining);
}

void itfResetBattleFadeState(BtlFade *fade, s32 preserveKind) {
    if (preserveKind == 0) {
        fade->kind = 0;
    }
    fade->phase = 0;
    fade->timer = 0;
    fade->alpha = 0x40;
    fade->unk08 = 0;
}

void btlSetFadePhaseAlphaTimer(BtlFade *fade, s16 phase, s16 alpha, s16 timer) {
    fade->phase = phase;
    fade->alpha = alpha;
    fade->timer = timer;
}

void btlReleaseEffectResourceHandles(ItfMesState *effect) {
    ItfMesBlkA4 *place = &effect->blkA4;
    if (place->frame != NULL) {
        itfPanelReleasePrimitiveResources(place->frame);
        place->frame = NULL;
    }
    if (place->sprite != NULL) {
        itfPanelReleasePrimitiveResources(place->sprite);
        place->sprite = NULL;
    }
    if (place->overlay != NULL) {
        itfPanelReleasePrimitiveResources(place->overlay);
        place->overlay = NULL;
    }
    effect->flags &= ~0xF00;
}

/* Release the handles in the second half for occupied entries in the first. */
void itfReleaseUiResourceSlotHandles(ItfMesTextSlots *slots) {
    s32 remaining;
    u32 *entries = slots->addresses;

    remaining = 0x1f;
    do {
        if (*entries != 0) {
            sdfReleaseResourceAllocation(slots->handles[entries - slots->addresses]);
            *entries = 0;
        }
        remaining = remaining - 1;
        entries = entries + 1;
    } while (-1 < remaining);
}

u16 *txtFormatNumberU16(s32 value, u16 *out) {
    s32 digits[10];
    s32 count = 0;
    s32 i;
    do {
        digits[count] = value % 10;
        value = value / 10;
        count++;
    } while (value > 0 && count < 10);
    for (i = count - 1; i >= 0; i--) {
        *out++ = (digits[i] << 8) - 0x6F80;
    }
    *out = 0;
    return out;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A6078);

void itfMesUpdatePanelFades(ItfMesState *panel);

void btlUpdateFadeIndicator(ItfMesState *panel);

void func_001A6528(ItfMesState *panel);

void itfUpdateBattleDisplayAndFadeIndicator(ItfMesState *panel) {
    itfMesUpdatePanelFades(panel);
    func_001A6350(panel);
    func_001A6528(panel);
    btlUpdateFadeIndicator(panel);
}

void itfMesUpdatePanelFades(ItfMesState *panel) {
    ItfMesBlkA4 *place = &panel->blkA4;
    UiSprite *sprite;
    s32 transition;

    sprite = place->sprite;
    transition = panel->flags & 0x300;
    switch (transition) {
    case 0x100:
        sprite->unk38 += 24;
        if (sprite->unk38 >= place->fadeLimit || panel->unk12 == 3) {
            sprite->unk38 = place->fadeLimit;
            panel->flags = (panel->flags & ~0x307) | 0x203;
        }
        break;
    case 0x300:
        sprite->unk38 -= 8;
        if (sprite->unk38 <= 0 || panel->unk12 == 3) {
            sprite->unk38 = 0;
            panel->flags &= ~0x300;
        }
        break;
    }

    sprite = place->frame;
    transition = panel->flags & 0x3000;
    switch (transition) {
    case 0x1000:
        sprite->unk38 += 32;
        if (sprite->unk38 >= 200) {
            sprite->unk38 = 200;
            panel->flags = (panel->flags & ~0x3000) | 0x2000;
        }
        break;
    case 0x3000:
        sprite->unk38 -= 32;
        if (sprite->unk38 <= 0) {
            sprite->unk38 = 0;
            panel->flags &= ~0x3000;
            itfPanelReleasePrimitiveResources(sprite);
            place->frame = NULL;
        }
        break;
    }

    sprite = place->overlay;
    transition = panel->flags & 0xC00;
    switch (transition) {
    case 0x400:
        sprite->unk38 += 24;
        if (sprite->unk38 >= place->fadeLimit) {
            sprite->unk38 = place->fadeLimit;
            panel->flags = (panel->flags & ~0xC07) | 0x803;
        }
        break;
    case 0xC00:
        sprite->unk38 -= 8;
        if (sprite->unk38 <= 0) {
            sprite->unk38 = 0;
            panel->flags &= ~0xC00;
            itfPanelReleasePrimitiveResources(sprite);
            place->overlay = NULL;
        }
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A6350);

extern void itfMesSetChildChainFlags(FrFontGlyph *glyph, u8 flagValue);

extern s32 itfMesNthClearBit(s32 clearBitsToSkip, u32 mask);

extern s32 sndSeqSelectPoll(ItfMesState *panel);

extern void itfMesShiftPanelVertically(ItfMesState *panel, s32 dy);

void func_001A6528(ItfMesState *panel) {
    ItfMesBlk40 *selection = &panel->blk40;
    ItfMesEntryBlock *entry = &panel->entryBlock;
    u32 flags = panel->flags;
    s32 top;
    s32 bottom;
    s32 entryY;

    switch (selection->unk10) {
    case 1:
        if (panel->unk12 == 3) {
            selection->unk10 = 2;
            panel->flags = (flags & ~0x38) | 0x18;
            break;
        }
        entryY = entry->y;
        bottom = entryY + entry->unk16 * (25 << 3);
        top = selection->y - ((selection->rowCount * 25 - 25) << 3);
        if (entryY == 0xAF8 && panel->blkA4.sprite != NULL) {
            panel->blkA4.sprite->scrollSpan = ((bottom - top) / 64) * 64 + 64;
        }
        if (entry->glyphChain != NULL && top < bottom) {
            itfMesShiftPanelVertically(panel, -64);
            return;
        }
        if ((flags & 0xC00) != 0x400) {
            if (entry->glyphChain != NULL) {
                itfMesSetChildChainFlags(entry->glyphChain, 3);
            }
            selection->unk10 = 2;
            panel->flags = (panel->flags & ~0x38) | 0x18;
        }
        break;
    case 2:
        if ((flags & 0x38) == 0x20 && sndSeqSelectPoll(panel) == 1) {
            selection->glyphChain = itfMesTrimGlyphChainToRow(selection->glyphChain,
                selection->selectedIndex, selection->rowCount);
            if ((flags & 0xC00) == 0x800) {
                panel->flags |= 0xC00;
            }
            selection->selectedIndex = itfMesNthClearBit(selection->selectedIndex, selection->panelValue);
            selection->optionCount = 0;
            btlSetFadePhaseAlphaTimer(&panel->fade, 1, 0x7F, 0);
            panel->flags = (panel->flags & ~0x38) | 0x28;
            selection->unk20 = 0x80;
            selection->unk10 = 3;
        }
        break;
    case 3:
        selection->unk20 -= 16;
        if (selection->unk20 <= 0) {
            selection->unk20 = 0;
            selection->unk10 = 4;
            itfResetBattleFadeState(&panel->fade, 0);
        }
        itfMesRecolorNodeChildren(selection->glyphChain, selection->unk20);
        return;
    case 4:
        if (entry->glyphChain != NULL && entry->y < 0xAF8) {
            itfMesShiftPanelVertically(panel, 64);
            return;
        }
        selection->unk10 = -1;
        break;
    }
}

void itfMesShiftPanelVertically(ItfMesState *panel, s32 dy) {
    ItfMesBlk14 *origin = &panel->blk14;
    ItfMesEntryBlock *pos = &panel->entryBlock;
    ItfMesBlkA4 *place = &panel->blkA4;
    pos->y += dy;
    itfMesOffsetNodeChain(pos->glyphChain, 0, dy);
    if (place->sprite != 0) {
        itfAdvancePanelLayoutAndNotify(place->sprite, 0, dy, 0, 0, 0);
    }
    if (place->frame != 0) {
        itfAdvancePanelLayoutAndNotify(place->frame, 0, dy, 0, dy, 0);
    }
    if (origin->glyphChain != NULL) {
        origin->y += dy;
        itfMesOffsetNodeChain(origin->glyphChain, 0, dy);
    }
}

s32 sndSeqSelectPoll(ItfMesState *panel) {
    ItfMesBlk40 *sel = &panel->blk40;
    s32 dir = 0;
    s32 index;
    if (D_0037F510.prev & 2) {
        if (sel->selectedIndex != 0) {
            dir = -1;
        } else if (D_0037F510.prev < 0) {
            dir = -1;
        }
    } else if (D_0037F510.next & 2) {
        if (sel->selectedIndex != sel->rowCount - 1 || D_0037F510.next < 0) {
            dir = 1;
        }
    }
    if (dir != 0) {
        sndStepSequenceIndex(sel, dir);
        itfResetBattleFadeState(&panel->fade, 1);
    }
    if (D_0037F510.confirm < 0) {
        sndSetSequenceVolumePan(8, 0x7F, 0x3F);
        return 1;
    }
    if (sel->optionCount > 0 && (index = func_001A6AB8(sel)) >= 0) {
        sndSetSequenceVolumePan(8, 0x7F, 0x3F);
        if (index != sel->selectedIndex) {
            itfMesSetRowItemFlag(sel->glyphChain, sel->selectedIndex, sel->rowCount, 0);
            itfMesSetRowItemFlag(sel->glyphChain, index, sel->rowCount, 1);
            sel->selectedIndex = index;
            sel->savedIndex = index;
        }
        return 1;
    }
    return 0;
}

void sndStepSequenceIndex(ItfMesBlk40 *sel, s32 dir) {
    s32 index = sel->selectedIndex;
    itfMesSetRowItemFlag(sel->glyphChain, index, sel->rowCount, 0);
    if (dir < 0) {
        index--;
        if (index < 0) {
            index = sel->rowCount - 1;
        }
    } else {
        index++;
        if (index >= sel->rowCount) {
            index = 0;
        }
    }
    itfMesSetRowItemFlag(sel->glyphChain, index, sel->rowCount, 1);
    sel->selectedIndex = index;
    sel->savedIndex = index;
    sndSetSequenceVolumePan(1, 0x7F, 0x3F);
}

s32 func_001A6AB8(ItfMesBlk40 *selection) {
    s32 i;

    for (i = 0; i < selection->optionCount; i++) {
        ItfMesOption *option = &selection->options[i];

        if (D_0037F510.buttons[option->id] < 0) {
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

void btlUpdateFadeIndicator(ItfMesState *panel) {
    BtlFade *fade = &panel->fade;
    s32 minimumAlpha;
    if (fade->kind != 0) {
        if (fade->timer > 0) {
            fade->timer -= 8;
        }
        switch (fade->phase) {
        case 0:
            fade->alpha += 8;
            if (fade->alpha >= 0xFF) {
                fade->phase = 1;
                fade->alpha = 0xFF;
                fade->timer = 0x80;
            }
            break;
        case 1:
            fade->alpha -= 8;
            minimumAlpha = (fade->kind & 1) ? 0x20 : 0x40;
            if (fade->alpha <= minimumAlpha) {
                fade->alpha = minimumAlpha;
                fade->phase = 0;
            }
            break;
        }
    }
}

extern void itfMesRenderActivePanelSprites(ItfMesState *);

extern void itfDrawSoundSelectorFadeLayers(ItfMesState *);

extern void func_001A7120(ItfMesState *);

extern void func_001A6E88(ItfMesState *);

void itfUpdateSoundSelectorPanel(ItfMesState *panel) {
    u32 flags = panel->flags;
    ItfMesBlk40 *selection;
    FrFontGlyph *glyph;

    itfMesWork.flags &= ~2;
    itfMesRenderActivePanelSprites(panel);
    glyph = panel->blk14.glyphChain;
    if (!(flags & 0x10000) && glyph != 0) {
        frFontDrawGlyphInDefaultMode(glyph);
    }
    glyph = panel->entryBlock.glyphChain;
    if (!(flags & 0x20000) && (flags & 7) >= 3) {
        if (frFontDrawGlyphInDefaultMode(glyph) > 0) {
            if ((panel->flags & 7) != 4) {
                panel->fade.unk08 = 0;
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
                itfMesSetRowItemFlag(selection->glyphChain, selection->selectedIndex, selection->rowCount, 1);
                panel->flags = (panel->flags & ~0x38) | 0x20;
                panel->fade.kind = 2;
            }
        }
    }
    if (panel->fade.kind & 1) {
        itfDrawSoundSelectorFadeLayers(panel);
    }
    if (panel->fade.kind & 2) {
        if (panel->unk12 == 3) {
            func_001A7120(panel);
        } else {
            func_001A6E88(panel);
        }
    }
}

void itfMesRenderActivePanelSprites(ItfMesState *panel) {
    ItfMesBlkA4 *place;
    if (panel->flags & 0x80000) {
        return;
    }
    place = &panel->blkA4;
    if ((panel->flags & 0x300) >= 0x100) {
        if (panel->unk12 != 3) {
            itfBuildAndSubmitPanelPacket(place->sprite, &kwlnDrawSurfaces[panel->unk10]);
        }
        itfMesWork.flags |= 2;
        if (place->overlay != 0) {
            itfBuildAndSubmitPanelPacket(place->overlay, &kwlnDrawSurfaces[panel->unk10]);
        }
    }
    if (D_003B4778[0] != 0 && D_003B4778[0]->owner == panel && place != 0) {
        func_001A7798(place->sprite);
    }
}

extern DrawColorRec D_003B49B8[];

extern void func_001A09C0(DrawVertex *, DrawColorRec *, u32, s32, SdfListHead *);

extern void itfQueueColoredTexturedQuadPacket(DrawVertex *, DrawColorRec *, DrawColorRec *, u32, s32, SdfListHead *);

/* Draw the selected sound row and its expanding fade outline. */
void func_001A6E88(ItfMesState *panel) {
    DrawColorRec uv;
    DrawColorRec color;
    DrawVertex bounds[2];
    ItfMesBlk40 *selection = &panel->blk40;
    BtlFade *fade = &panel->fade;
    s32 selected = selection->savedIndex;
    SdfListHead *list;
    s32 x;
    s32 y;
    s32 bottom;
    s32 expansion;
    s32 rightExpansion;
    s32 verticalExpansion;
    SdfPoolNode *surface;

    if (selected == -1) {
        return;
    }
    list = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(list);
    x = selection->x;
    y = (s32)selection->y - ((selection->rowCount * 25 - 23) << 3) + selected * 0xA0;
    bottom = y + 0x88;
    bounds[0].x = x - 0x1D0;
    bounds[0].y = y + 0x20;
    bounds[1].x = x + func_0019DBA8(selected, selection->glyphChain) + 0x1D0;
    bounds[1].y = bounds[0].y + 0x90;
    func_001A09C0(bounds, D_003B49B8, panel->renderValue, 0x1D0, list);

    bounds[0].x = x;
    bounds[0].y = y + 8;
    bounds[1].x = x + 0x60;
    bounds[1].y = bottom;
    uv.components[0] = 0x150;
    uv.components[1] = 0x2F0;
    uv.components[2] = 0x1B0;
    uv.components[3] = 0x3F0;
    color.components[0] = 0x80;
    color.components[1] = 0x80;
    color.components[2] = 0x80;
    color.components[3] = 0x26;
    itfQueueTextureBoundQuadPacket(bounds, &uv, &color, panel->renderValue,
                                  itfMesWork.windowTexture, 0, list);

    bounds[0].x = x - 0xF0;
    bounds[0].y = y + 0x30;
    bounds[1].x = x - 0x30;
    bounds[1].y = bottom;
    uv.components[0] = 0x290;
    uv.components[1] = 0x10;
    uv.components[2] = 0x350;
    uv.components[3] = 0xC0;
    color.components[3] = fade->alpha;
    itfQueueColoredTexturedQuadPacket(bounds, &uv, &color, panel->renderValue, 0, list);
    if (fade->timer > 0) {
        expansion = 0x80 - fade->timer;
        rightExpansion = expansion << 1;
        verticalExpansion = expansion >> 1;
        bounds[0].x -= expansion;
        bounds[0].y -= verticalExpansion;
        bounds[1].x += rightExpansion;
        bounds[1].y += verticalExpansion;
        color.components[3] = fade->timer;
        itfSendTablePacket(list, 1, 0);
        itfQueueColoredTexturedQuadPacket(bounds, &uv, &color, panel->renderValue, 0, list);
        itfSendTablePacket(list, 0, 0);
    }
    surface = &kwlnDrawSurfaces[panel->unk10];
    surface->append(surface, list);
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A7120);

extern u8 D_003B49F8[];

extern u8 D_003B49E8[];

extern s32 D_003B4A08[];

/* Draw the sound selector frame, its fade layer and the expanding timer outline. */
void itfDrawSoundSelectorFadeLayers(ItfMesState *object) {
    s32 bounds[4];
    BtlFade *fade = &object->fade;
    s32 packet;
    s32 expansion;
    SdfPoolNode *surface;

    bounds[0] = 0x1AA0;
    bounds[1] = 0xC60;
    bounds[2] = 0x1BD0;
    bounds[3] = 0xD58;
    packet = sdfAllocPacketAligned(0x20);
    sdfInitPacketList((SdfListHead *)packet);
    D_003B4A08[3] = 0xFF;
    itfQueueTextureBoundQuadPacket(bounds, D_003B49F8, D_003B4A08, object->renderValue,
                                  itfMesWork.windowTexture, 0, (SdfListHead *)packet);
    D_003B4A08[3] = fade->alpha;
    itfQueueTextureBoundQuadPacket(bounds, D_003B49E8, D_003B4A08, object->renderValue,
                                  itfMesWork.windowTexture, 0, (SdfListHead *)packet);
    if (fade->timer > 0) {
        expansion = 0x80 - fade->timer;
        bounds[0] -= expansion * 2;
        bounds[1] -= expansion;
        bounds[2] += expansion * 2;
        bounds[3] += expansion;
        D_003B4A08[3] = fade->timer;
        itfSendTablePacket((SdfListHead *)packet, 1, 0);
        itfQueueTextureBoundQuadPacket(bounds, D_003B49E8, D_003B4A08, object->renderValue,
                                      itfMesWork.windowTexture, 0, (SdfListHead *)packet);
        itfSendTablePacket((SdfListHead *)packet, 0, 0);
    }
    surface = &kwlnDrawSurfaces[object->unk10];
    surface->append(surface, (SdfListHead *)packet);
}

s32 sndVisitQueuedResources(void) {
    ItfMesPoolNode *node;
    for (node = itfMesWork.pool.activeHead; node != 0; node = node->next) {
        itfUpdateBattleDisplayAndFadeIndicator((ItfMesState *)node->stateAddress);
    }
    return 0;
}

extern s32 func_001200E0(void);

s32 func_001A76C8(void) {
    ItfMesPoolNode *node;

    if (func_001200E0() != 0) {
        return 0;
    }
    for (node = itfMesWork.pool.activeHead; node != NULL; node = node->next) {
        itfUpdateSoundSelectorPanel((ItfMesState *)node->stateAddress);
    }
    itfMesWork.unk8++;
    return 0;
}

void sndFlushMessageQueue(void) {
    ItfMesPoolNode *node = itfMesWork.pool.activeHead;
    s32 message;
    while (node != 0) {
        message = node->index;
        node = node->next;
        itfMesDestroyWindow(message);
    }
    sdfTexReleaseReferenceViaHandler(itfMesWork.windowTexture);
    itfMesWork.windowTexture = NULL;
}

extern SdfPoolNode D_00380708;

extern s32 D_003B4A18[];

extern u8 D_00436630[5];

extern u8 D_00436638[5];

extern void itfEmitQuadListA(void *, void *, u8 *, u8 *, s32, u32, SdfListHead *);

void func_001A7798(UiSprite *sprite) {
    s32 vertices[4][2] = {
        {sprite->left, sprite->top},
        {sprite->right, sprite->top},
        {sprite->right, sprite->bottom},
        {sprite->left, sprite->bottom}
    };
    s32 packet;

    packet = sdfAllocPacketAligned(0x20);
    sdfInitPacketList((SdfListHead *)packet);
    itfEmitQuadListA(vertices, D_003B4A18, D_00436630, D_00436638, 5, 0xFFFFFF, (SdfListHead *)packet);
    D_00380708.append(&D_00380708, (SdfListHead *)packet);
}

void itfAdjustPanelBoundsWithPad(ItfMesBlkA4 *object, s32 mode) {
    UiSprite *sprite = object->sprite;
    s32 *bounds;
    s32 dx;
    s32 dy;

    if (sprite != NULL) {
        bounds = object->bounds;
        if (D_0037F510.coarseDown & 2) {
            dx = -16;
        } else {
            dx = ((u8)D_0037F510.coarseUp << 3) & 0x10;
        }
        if (D_0037F510.unk36 & 2) {
            dy = -8;
        } else {
            dy = ((u8)D_0037F510.unk37 << 2) & 8;
        }
        if (D_0037F510.unk31 != 0) {
            dx *= 8;
            dy *= 8;
        }
        if (dx != 0 || dy != 0) {
            switch (mode) {
            case 0:
                itfAdvancePanelLayoutAndNotify(sprite, dx, dy, 0, 0, 0);
                bounds[0] += dx;
                bounds[1] += dy;
                break;
            case 1:
                itfAdvancePanelLayoutAndNotify(sprite, 0, 0, dx, dy, 0);
                bounds[2] += dx;
                bounds[3] += dy;
                break;
            case 2:
                itfAdvancePanelLayoutAndNotify(sprite, dx, dy, dx, dy, 0);
                bounds[0] += dx;
                bounds[1] += dy;
                bounds[2] += dx;
                bounds[3] += dy;
                break;
            }
        }
    }
}

typedef struct SndPadStepTarget {
    u8 pad00[0x38];
    s32 value; /* 0x38 */
} SndPadStepTarget;

typedef struct SndPadStepper {
    u8 pad00[4];
    SndPadStepTarget *target; /* 0x04 */
    u8 pad08[0x20];
    s32 index; /* 0x28 */
} SndPadStepper;

/* Step the stepper's index by pad input: one per press, ten with the fast modifier held; mirror it into the target. */
void sndStepIndexByPad(SndPadStepper *stepper) {
    s32 step;

    if (D_0037F510.coarseDown & 2) {
        step = -1;
    } else {
        step = (D_0037F510.coarseUp & 2) > 0;
    }
    if (D_0037F510.unk36 & 2) {
        step = -1;
    } else if (D_0037F510.unk37 & 2) {
        step = 1;
    }
    if (D_0037F510.unk31 != 0) {
        step *= 10;
    }
    if (step != 0) {
        s32 index = (stepper->index + step) & 0xFF;

        stepper->index = index;
        if (stepper->target != NULL) {
            stepper->target->value = index;
        }
    }
}

ItfMesPoolNode *func_001A7A98(ItfMesPoolNode *node) {
    s32 index = 0;
    s32 count = itfMesWork.activeWindowCount;

    if (count <= 0) {
        return NULL;
    }
    for (;;) {
        if (node != NULL) {
            node = node->previous;
        }
        if (node == NULL) {
            node = itfMesWork.pool.activeTail;
        }
        if (((ItfMesState *)node->stateAddress)->blkA4.sprite != NULL) {
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

void itfQueueOffsetTexturedRect(s32 *bounds, s32 *region, s32 x, s32 y,
                   s32 alpha, SdfTex *texture, SdfListHead *command) {
    s32 positions[4];
    s32 uv[4];
    UiQuadColor color = D_00414D50;

    positions[0] = (bounds[0] + x) << 4;
    positions[1] = (bounds[1] + y) << 3;
    positions[2] = (bounds[2] + x) << 4;
    positions[3] = (bounds[3] + y) << 3;
    uv[0] = region[0] << 4;
    uv[1] = region[1] << 4;
    uv[2] = (region[0] + region[2]) << 4;
    uv[3] = (region[1] + region[3]) << 4;
    color.alpha = alpha;
    itfQueueTextureBoundQuadPacket(positions, uv, &color, 0, texture, 0, command);
}

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00414D50);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00414D60);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A7C08);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A81F0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A85E0);

extern s32 D_00435CBC;

extern KwlnTask *kwlnTaskCreate(const char *, u32, s32, s32, TaskUpdate, TaskDestroy, u32);

extern void itfPrintTestMessageCallback();

extern s32 itfUpdateTestMessageResourceInput(KwlnTask *task);

extern u8 D_003B4A28[];

extern s32 scrCreateTaskForProcessId();


void sndCreateTestMsgTasks(void) {
    D_00435CBC = 0x80FFFFFF;
    sndCycleTestMessageResource();
    itfMesSetWindowCallbackAddress(((ScrData *)kwlnTaskGetUserValue((KwlnTask *)scrCreateTaskForProcessId(0x3E8, D_003B4A28, 0)))->resourceIndex, itfPrintTestMessageCallback);
    kwlnTaskCreate("TestMsgMngC", 0x3EF, 0, 0, itfUpdateTestMessageResourceInput, 0, 0);
    kwlnTaskCreate("TestMsgMngD", 0x2AFE, 0, 0, sndUpdateTestMsgTask, 0, 0);
}

void sndCycleTestMessageResource(void) {
    if (sndTestMessageTexture != 0) {
        sdfTexReleaseReferenceViaHandler(sndTestMessageTexture);
        sndTestMessageTexture = 0;
    }
    sndTestMessageResourceIndex = (sndTestMessageResourceIndex + 1) & 3;
    if (sndTestMessageResourceIndex != 3) {
        sndTestMessageTexture = itfLoadTextureFromAsset(D_003B4CF8[sndTestMessageResourceIndex]);
    }
}

s32 itfUpdateTestMessageResourceInput(KwlnTask *task) {
    if (D_0037F530[0] < 0) {
        sndCycleTestMessageResource();
    }
    return 0;
}

s32 sndUpdateTestMsgTask(KwlnTask *task) {
    s32 mem;
    if ((sndTestMessageTexture != 0) && (sndTestMessageResourceIndex != 3)) {
        mem = sdfAllocPacketAligned(0x20);
        sdfInitPacketList((SdfListHead *)mem);
        itfSendTablePacket((SdfListHead *)mem, 0, 0);
        itfQueueTextureBoundQuadPacket(D_003B4D08, D_003B4D18, D_003B4D28, 0xFFF, sndTestMessageTexture, 0, (SdfListHead *)mem);
        D_003805A8.append(&D_003805A8, (SdfListHead *)mem);
        return 0;
    }
    return 0;
}

extern s32 func_0035B6E0(const char *, ...);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00414EB0);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00414EC8);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00414EE0);

void itfPrintTestMessageCallback(void) {
    func_0035B6E0("********* AAAA ********\n");
}

typedef struct ItfFovPanelWork {
    u8 pad00[8];
    f32 degrees;
    u8 pad0C[4];
} ItfFovPanelWork;

typedef char ItfFovPanelWorkSizeCheck[sizeof(ItfFovPanelWork) == 0x10 ? 1 : -1];

extern ItfFovPanelWork D_00452E70;

extern s8 D_0037F543[];

extern void *func_0011F250(s32, s32, s32, s32, s32, u32, u32);

extern s32 itfStepFloatWithPad(f32 *, f32, f32, f32, f32);

s32 btlEditCameraFieldOfView(void) {
    SifCommand packet;
    s32 list;
    f32 radiansToDegrees = 57.2957795f;

    list = (s32)sdfCreateResetPacketList();
    sdfAppendPacket((SdfListHead *)list,
                    (u32)func_0011F250(0x8500, 0x79C0, 0xFEFFFF,
                                       0xA80, 0x120, 0x60000000, 0x40806020));
    sdfPktInit(&packet, 0x85C0, 0x7A20, 0xFF0000, 0);
    sdfAppendPacket((SdfListHead *)list,
                    (u32)sdfFormatSifPacket(&packet, "FOVY: %6.2f",
                        sdfSceneProjectionParameters.camera.fov * radiansToDegrees));
    D_00380708.append(&D_00380708, (SdfListHead *)list);
    if (D_0037F543[0] < 0) {
        return -1;
    }
    D_00452E70.degrees = sdfSceneProjectionParameters.camera.fov * radiansToDegrees;
    if (itfStepFloatWithPad(&D_00452E70.degrees, 1.0f, 89.0f, 0.1f, 1.0f) != 0) {
        sdfSceneProjectionParameters.camera.fov = D_00452E70.degrees * 0.0174532925f;
    }
    return 0;
}

s32 itfStepFloatWithPad(f32 *value, f32 minimum, f32 maximum, f32 coarseStep, f32 fineStep) {
    f32 current = *value;
    s32 changed = 0;

    if (D_0037F510.coarseUp < 0) {
        if (current < maximum) {
            current += coarseStep;
        } else {
            current = minimum;
        }
        changed = 1;
    } else if (D_0037F510.coarseUp & 2) {
        if (current < maximum) {
            current += coarseStep;
            changed = 1;
        }
    } else if (D_0037F510.coarseDown < 0) {
        if (minimum < current) {
            current -= coarseStep;
        } else {
            current = maximum;
        }
        changed = 1;
    } else if (D_0037F510.coarseDown & 2) {
        if (minimum < current) {
            current -= coarseStep;
            changed = 1;
        }
    } else if (fineStep != 0.0f) {
        if (D_0037F510.fineUp < 0) {
            if (current < maximum) {
                current += fineStep;
                if (maximum < current) {
                    current = maximum;
                }
            } else {
                current = minimum;
            }
            changed = 1;
        } else if (D_0037F510.fineUp & 2) {
            if (current < maximum) {
                current += fineStep;
                if (maximum < current) {
                    current = maximum;
                }
                changed = 1;
            }
        } else if (D_0037F510.fineDown < 0) {
            if (minimum < current) {
                current -= fineStep;
                if (current < minimum) {
                    current = minimum;
                }
            } else {
                current = maximum;
            }
            changed = 1;
        } else if (D_0037F510.fineDown & 2) {
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
        *value = current;
        return 1;
    }
    return 0;
}

typedef struct ItfBlurPanelWork {
    s16 editing;
    s16 selection;
    u32 blink;
    u8 pad08[8];
} ItfBlurPanelWork;

extern ItfBlurPanelWork D_00452E80;

extern const char *D_003B4D38[6];

extern u8 kwlnDrawOverlayEnabled;

extern s16 kwlnDrawOverlayAlpha;

extern s16 kwlnDrawOverlayScale;

extern s32 D_00435CE0[2];

extern char D_00436660[];

extern char D_00436668[];

extern char D_00436670[];

extern char D_00436678[];

extern char D_00436680[];

extern char D_00436688[];

extern SdfPoolNode kwlnPositionedTextSurface;

/* Draw the blur settings and handle selection, editing and cancellation. */
s32 func_001A8BD0(void) {
    SifCommand packet;
    SdfListHead *list;
    s32 row;
    s32 y;
    s16 previous;
    s32 highlight;

    list = sdfCreateResetPacketList();
    sdfAppendPacket(list,
        (u32)func_0011F250(0x8290, 0x79A8, 0xFEFFFF, 0xC60, 0x1B0, 0x60000000, 0x40806020));
    sdfPktInit(&packet, 0x82C0, 0x79C0, 0xFF0000, 0);
    sdfAppendPacket(list, (u32)sdfFormatSifPacket(&packet,
        "BLUR:        %s", kwlnDrawOverlayEnabled ? D_00436660 : D_00436668));

    y = 0x7A20;
    if (D_00452E80.editing == 0) {
        sdfPktInit(&packet, 0x82C0, D_00452E80.selection * 0x60 + 0x7A20, 0xFF0000, 0);
        D_00452E80.blink++;
        if (D_00452E80.blink < 32 || (D_00452E80.blink & 31) < 18) {
            sdfAppendPacket(list, (u32)sdfFormatSifPacket(&packet, D_00436670));
        }
    }

    for (row = 0; row != 3; row++) {
        sdfPktInit(&packet, 0x82C0, y, 0xFF0000, 0);
        sdfAppendPacket(list,
            (u32)sdfFormatSifPacket(&packet, D_00436678, D_003B4D38[row]));
        highlight = 0;
        if (D_00452E80.editing != 0 && row == D_00452E80.selection) {
            highlight = 6;
        }
        sdfPktInit(&packet, 0x8800, y, 0xFF0000, highlight);
        switch (row) {
        case 0:
            sdfAppendPacket(list,
                (u32)sdfFormatSifPacket(&packet, D_00436680, kwlnDrawOverlayAlpha));
            break;
        case 1:
            sdfAppendPacket(list,
                (u32)sdfFormatSifPacket(&packet, D_00436680, kwlnDrawOverlayScale));
            break;
        case 2:
            sdfAppendPacket(list,
                (u32)sdfFormatSifPacket(&packet, D_00436688, D_00435CE0[0], D_00435CE0[1]));
            break;
        }
        y += 0x60;
    }
    sdfAppendPacket(list,
        (u32)func_0011F250((D_00435CE0[0] * 16) + 0x7FC0, (D_00435CE0[1] * 8) + 0x7FE0,
            0xFF0000, 0x80, 0x40, 0x80008080, 0x80000000));
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, list);

    if ((s8)D_0037F510.unk32 < 0) {
        kwlnDrawOverlayEnabled ^= 1;
    }
    if (D_00452E80.editing == 0) {
        previous = D_00452E80.selection;
        if ((u8)D_0037F510.unk36 & 2) {
            D_00452E80.selection--;
            if (D_00452E80.selection < 0) {
                D_00452E80.selection = 2;
            }
        }
        if ((u8)D_0037F510.unk37 & 2) {
            D_00452E80.selection++;
            if (D_00452E80.selection >= 3) {
                D_00452E80.selection = 0;
            }
        }
        if (previous != D_00452E80.selection) {
            D_00452E80.blink = 0;
        }
        if (D_0037F510.unk31 < 0) {
            D_00452E80.editing ^= 1;
            D_00452E80.blink = 0;
        }
        if ((u8)D_0037F510.cancel & 2) {
            return -1;
        }
    } else {
        if (D_00452E80.selection == 2) {
            if (D_0037F510.unk36 != 0) {
                D_00435CE0[1] -= 16;
                if (D_00435CE0[1] < -224) {
                    D_00435CE0[1] = -224;
                }
            }
            if (D_0037F510.unk37 != 0) {
                D_00435CE0[1] += 16;
                if (D_00435CE0[1] > 224) {
                    D_00435CE0[1] = 224;
                }
            }
            if (D_0037F510.coarseDown != 0) {
                D_00435CE0[0] -= 16;
                if (D_00435CE0[0] < -256) {
                    D_00435CE0[0] = -256;
                }
            }
            if (D_0037F510.coarseUp != 0) {
                D_00435CE0[0] += 16;
                if (D_00435CE0[0] > 256) {
                    D_00435CE0[0] = 256;
                }
            }
        } else if (D_00452E80.selection == 0) {
            if ((u8)D_0037F510.coarseDown & 2) {
                kwlnDrawOverlayAlpha -= 10;
                if (kwlnDrawOverlayAlpha < 0) {
                    kwlnDrawOverlayAlpha = 0;
                }
            }
            if ((u8)D_0037F510.coarseUp & 2) {
                kwlnDrawOverlayAlpha += 10;
                if (kwlnDrawOverlayAlpha > 255) {
                    kwlnDrawOverlayAlpha = 255;
                }
            }
        } else {
            if ((u8)D_0037F510.coarseDown & 2) {
                kwlnDrawOverlayScale -= 10;
                if (kwlnDrawOverlayScale < -255) {
                    kwlnDrawOverlayScale = -255;
                }
            }
            if ((u8)D_0037F510.coarseUp & 2) {
                kwlnDrawOverlayScale += 10;
                if (kwlnDrawOverlayScale > 255) {
                    kwlnDrawOverlayScale = 255;
                }
            }
        }
        if (D_0037F510.cancel < 0) {
            D_00452E80.editing ^= 1;
            D_00452E80.blink = 0;
        }
    }
    return 0;
}


typedef struct ItfColorPanelWork {
    s16 editing;
    s16 selection;
    u32 blink;
    u8 pad08[8];
} ItfColorPanelWork;

typedef struct ItfColorPanelRow {
    const char *title;
    s32 highlight;
} ItfColorPanelRow;
typedef char ItfColorPanelWorkSizeCheck[sizeof(ItfColorPanelWork) == 0x10 ? 1 : -1];
typedef char ItfColorPanelRowSizeCheck[sizeof(ItfColorPanelRow) == 8 ? 1 : -1];

extern ItfColorPanelWork D_00452E90;
extern ItfColorPanelRow D_003B4D50[6];
extern s32 D_00438F3C;
extern UiQuadColor D_003B4D80;
extern s32 D_00438F40;
extern u8 D_004366B4;
extern char D_004366B8[]; /* "FILTER:" */
extern char D_004366C0[]; /* ">" */
extern char D_004366C8[]; /* " %s" */
extern char D_004366D0[]; /* "%3d" */
extern char D_004366D8[]; /* "%4d" */
void itfDrawPulsingTestOverlay(s32 surfaceIndex);

/* Draw the four color channels and handle selection and value editing. */
/* Retail 0x001A92F0 keeps the row < 4 select inside the four-row loop. */
s32 func_001A9130(void) {
    SifCommand packet;
    SdfListHead *list;
    s32 row;
    s32 y = 0x7A20;
    s16 previousSelection;
    s32 previousRate;
    s32 highlight;

    list = sdfCreateResetPacketList();
    sdfAppendPacket(list,
        (u32)func_0011F250(0x8950, 0x79A8, 0xFEFFFF, 0x5A0, 0x210, 0x60000000, 0x40806020));
    sdfPktInit(&packet, 0x8980, 0x79C0, 0xFF0000, 0);
    sdfAppendPacket(list, (u32)sdfFormatSifPacket(&packet, D_004366B8));
    if (D_00452E90.editing == 0) {
        sdfPktInit(&packet, 0x8980, D_00452E90.selection * 0x60 + 0x7A20, 0xFF0000, 0);
        D_00452E90.blink++;
        if (D_00452E90.blink < 32 || (D_00452E90.blink & 31) < 18) {
            sdfAppendPacket(list, (u32)sdfFormatSifPacket(&packet, D_004366C0));
        }
    }
    for (row = 0; row != 4; row++) {
        sdfPktInit(&packet, 0x8980, y, 0xFF0000, D_003B4D50[row].highlight);
        sdfAppendPacket(list,
            (u32)sdfFormatSifPacket(&packet, D_004366C8, D_003B4D50[row].title));
        highlight = 0;
        if (D_00452E90.editing != 0 && row == D_00452E90.selection) {
            highlight = 6;
        }
        sdfPktInit(&packet, 0x8C80, y, 0xFF0000, highlight);
        if (row < 4) {
            sdfAppendPacket(list,
                (u32)sdfFormatSifPacket(&packet, D_004366D0, D_003B4D80.channels[row]));
        } else {
            sdfAppendPacket(list,
                (u32)sdfFormatSifPacket(&packet, D_004366D8, D_00438F40));
        }
        y += 0x60;
    }
    kwlnPositionedTextSurface.append((SdfListHead *)&kwlnPositionedTextSurface, list);
    D_00438F3C += D_00438F40;
    itfDrawPulsingTestOverlay(0x52);
    if ((s8)D_0037F510.unk32 < 0) {
        D_004366B4 ^= 1;
    }
    if (D_00452E90.editing == 0) {
        previousSelection = D_00452E90.selection;
        if ((u8)D_0037F510.unk36 & 2) {
            D_00452E90.selection--;
            if (D_00452E90.selection < 0) {
                D_00452E90.selection = 3;
            }
        }
        if ((u8)D_0037F510.unk37 & 2) {
            D_00452E90.selection++;
            if (D_00452E90.selection >= 4) {
                D_00452E90.selection = 0;
            }
        }
        if (previousSelection != D_00452E90.selection) {
            D_00452E90.blink = 0;
        }
        if (D_0037F510.unk31 < 0) {
            D_00452E90.editing ^= 1;
            D_00452E90.blink = 0;
        }
        if ((u8)D_0037F510.cancel & 2) {
            return -1;
        }
    } else {
        if (D_00452E90.selection < 4) {
            if ((u8)D_0037F510.coarseDown & 2) {
                D_003B4D80.channels[D_00452E90.selection] -= 10;
                if (D_003B4D80.channels[D_00452E90.selection] < 0) {
                    D_003B4D80.channels[D_00452E90.selection] = 0;
                }
            }
            if ((u8)D_0037F510.coarseUp & 2) {
                D_003B4D80.channels[D_00452E90.selection] += 10;
                if (D_003B4D80.channels[D_00452E90.selection] > 255) {
                    D_003B4D80.channels[D_00452E90.selection] = 255;
                }
            }
        } else {
            previousRate = D_00438F40;
            if ((u8)D_0037F510.coarseDown & 2) {
                D_00438F40 -= 0x100;
                if (D_00438F40 < 0) {
                    D_00438F40 = 0;
                }
            }
            if ((u8)D_0037F510.coarseUp & 2) {
                D_00438F40 += 0x100;
                if (D_00438F40 > 0x800) {
                    D_00438F40 = 0x800;
                }
            }
            if (previousRate != D_00438F40) {
                D_00438F3C = 0;
            }
        }
        if (D_0037F510.cancel < 0) {
            D_00452E90.editing ^= 1;
            D_00452E90.blink = 0;
        }
    }
    return 0;
}



u64 *func_001A9580(u32 x, u32 y, u32 depth, u32 width, u32 height, u32 color0, u32 color1) {
    u64 *packet = (u64 *)sdfAllocPacketAligned(0x80);

    packet[0] = 7;
    packet[1] = 0x5000000700000000ULL;
    packet[2] = 0xB400000000008001ULL;
    packet[3] = 0xFF515151510ULL;
    packet[4] = 0x14D;
    packet[5] = color0 | ((u64)0xFE00 << 46);
    packet[7] = color0 | ((u64)0xFE00 << 46);
    packet[9] = color1 | ((u64)0xFE00 << 46);
    packet[11] = color1 | ((u64)0xFE00 << 46);
    packet[6] = (u64)((x & 0xFFFF) | (y << 16)) | ((u64)depth << 32);
    packet[8] = (u64)(((x + width) & 0xFFFF) | (y << 16)) | ((u64)depth << 32);
    packet[10] = (u64)(((x + width) & 0xFFFF) | ((y + height) << 16)) | ((u64)depth << 32);
    packet[12] = (u64)((x & 0xFFFF) | ((y + height) << 16)) | ((u64)depth << 32);
    packet[13] = packet[14] = packet[15] = 0;
    return packet;
}

u64 *btlCreateGsTestRegisterPacket(u64 owner, s32 alternative) {
    u64 *entry = (u64 *)sdfConsFinalizePacketHeader(sdfAllocPacketAligned(0x30), 0x30);
    entry[4] = owner;
    entry[5] = alternative ? 0x48 : 0x47;
    return entry;
}

u64 *btlCreateGsAlphaRegisterPacket(u64 owner, s32 alternative) {
    u64 *entry = (u64 *)sdfConsFinalizePacketHeader(sdfAllocPacketAligned(0x30), 0x30);
    entry[4] = owner;
    entry[5] = alternative ? 0x43 : 0x42;
    return entry;
}

extern f32 sdfSinPoly(f32);

extern u64 *func_001A9580(u32, u32, u32, u32, u32, u32, u32);

void itfDrawPulsingTestOverlay(s32 surfaceIndex) {
    u32 color = 0;
    s32 alpha;
    s32 i;
    u8 *component;
    s32 list;
    SdfPoolNode *surface;
    f32 phase;

    phase = (f32)(D_00438F3C % 4096) * (1.0f / 4096.0f);
    phase = phase * 6.2831852f + 1.5707963f + 0.78539815f;
    alpha = (s32)((sdfSinPoly(phase) + 1.0f) * 0.5f * D_003B4D80.alpha);
    component = (u8 *)&D_003B4D80;
    for (i = 0; i != 3; i++, component += 4) {
        color |= *component << (i * 8);
    }
    color |= alpha << 24;
    list = (s32)sdfCreateResetPacketList();
    sdfAppendPacket((SdfListHead *)list, (u32)btlCreateGsTestRegisterPacket(0x33001, 0));
    sdfAppendPacket((SdfListHead *)list, (u32)btlCreateGsAlphaRegisterPacket(6, 0));
    sdfAppendPacket((SdfListHead *)list, (u32)func_001A9580(0x7000, 0x7900, 0xFEFFFF, 0x2000, 0xE00, color, color));
    surface = &kwlnDrawSurfaces[surfaceIndex];
    surface->append(surface, (SdfListHead *)list);
}

void btlResetRuntimeSequenceCounter(void) {
    D_004366E8 = 1;
}

u64 btlAdvanceRuntimeSequenceCounter(void) {
    s64 value = D_004366E8 + 1;

    if (value < 0) {
        value = 1;
        D_004366E8 = value;
    } else {
        D_004366E8 = value;
    }
    return value;
}

/* Clear the battle model flags; the exclusive upper limit differs by title. */
void btlClearModelFlagRange(void) {
    s32 flagIndex;

    for (flagIndex = 0xBE0; flagIndex < 0xC00; flagIndex++) {
        mdlFlagClear(flagIndex);
    }
}

extern s32 btlUpdateFadeColor(void);

extern s32 btlUpdateAutoMusic(void);

extern s32 btlUpdateTintAndWorldLight(void);

extern void func_00205160(void);

extern s32 btlUpdateScene(void);

extern s32 func_001D3ED8(void);

extern s32 btlUpdateActionSeqs(void);

extern s32 func_001E7648(void);

extern s32 func_0020D108(void);

extern s32 btlSweepFinishedTasks(void);

extern s32 func_001E9130(void);

extern s32 btlExitWhenAudioAndTasksIdle(void);

s32 btlUpdateActiveBattleFrame(KwlnTask *task) {
    if (btlRuntime == 0) {
        return 0;
    }
    if (((BattleController *)btlRuntime)->flags & 1) {
        btlUpdateFadeColor();
        btlUpdateAutoMusic();
        btlUpdateTintAndWorldLight();
        func_00205160();
        btlUpdateScene();
        func_001D3ED8();
        btlUpdateActionSeqs();
        func_001E7648();
        func_0020D108();
        btlSweepFinishedTasks();
        func_001E9130();
        ((BattleController *)btlRuntime)->frame = ((BattleController *)btlRuntime)->frame + 1;
    } else {
        btlExitWhenAudioAndTasksIdle();
    }
    return 0;
}

extern s32 btlTickFieldSwayAndTint(void);

extern s32 btlReleaseRainSoundTransition(void);

extern s32 btlSweepFloorModelLists(void);

extern s32 btlUpdateActorModelColorAndLinks(void);

extern void func_0022AC10(void);

extern s32 func_0020D110(void);

extern s32 fldInitializeBattleSceneFlow(void);

extern s32 btlClearDeferredTasks(void);

s32 btlUpdateBattleFieldPresentation(KwlnTask *task) {
    BattleController *battle = (BattleController *)btlRuntime;
    if (battle == 0) {
        return 0;
    }
    if (battle->flags & 1) {
        btlTickFieldSwayAndTint();
        btlReleaseRainSoundTransition();
        btlSweepFloorModelLists();
        btlUpdateActorModelColorAndLinks();
        func_0022AC10();
        func_0020D110();
        fldInitializeBattleSceneFlow();
        btlClearDeferredTasks();
    }
    return 0;
}

void func_001A9AA8(KwlnTask *task) {
}

extern char D_004366F0[];

extern void func_001A9AA8(KwlnTask *task);

extern s32 btlUpdateBattleFieldPresentation(KwlnTask *task);

extern s32 btlUpdateActiveBattleFrame(KwlnTask *task);

void btlCreateDrawTasks(void) {
    BattleController *battle = (BattleController *)btlRuntime;
    KwlnTask *draw;
    draw = kwlnTaskCreate(D_004366F0, 0x3F9, 0, 0, btlUpdateActiveBattleFrame, func_001A9AA8, 0);
    battle->drawTask = draw;
    func_00101968(draw, kwlnTaskCreate("battle_draw", 0x2B0E, 0, 0, btlUpdateBattleFieldPresentation, 0, 0));
}

void btlDestroyDrawTaskAtPriorityWhenPresent(void) {
    KwlnTask *task;

    task = func_00101820(0x3f9);
    if (task != 0) {
        BattleController *battle = (BattleController *)btlRuntime;
        kwlnTaskDestroyWithHierarchy(battle->drawTask, 1);
        return;
    }
}

typedef struct BattleInitState {
    u8 pad000[0x214];
    u32 tick;
    u8 pad218[4];
    u32 commandRestrictFlags;
    u8 pad220[4];
    u32 unk224;
    u8 pad228[0x20];
    u32 listHeads[6];
    u8 pad260[0x1C];
    u8 endCode;
    u8 pad27D[0x2F];
    u16 backgroundA;
    u16 backgroundB;
    u32 unk2B0;
    u8 unk2B4;
    u8 pad2B5[0x20F];
    u8 unk4C4;
    u8 pad4C5[3];
    f32 unk4C8;
    s32 messageWindows[3];
    u8 pad4D8[0xE0];
    u8 unk5B8;
    u8 pad5B9[3];
    u32 unk5BC;
} BattleInitState;

extern SdfMemBlock *D_004366E0;

extern ItfMesSub *D_00435E78;

extern ItfMesSub D_003858D8;

extern ItfMesSub D_00385228;

extern u16 mnuMovieTaskState;

extern u32 func_001003F8(void);

extern u32 func_001B5600(void);

extern void effMiscSeedRandom(void *, u32);

extern s32 itfMesCreateWindow(ItfMesSub *);

extern void btlResetActorEntryState(void);


extern char D_004366F8[];

extern char D_003B4D90[];

extern char D_003B4DA8[];

void func_001A9B80(void) {
    SdfMemBlock *allocation;
    u32 seed;
    u8 *runtime;

    btlResetRuntimeSequenceCounter();
    allocation = sdfAllocGeneralBlock(0xFD4);
    D_004366E0 = allocation;
    runtime = (u8 *)sdfResourceRetainAddress(allocation);
    btlRuntime = (s32)runtime;
    memset(runtime, 0, 0xFD4);

    ((BattleInitState *)btlRuntime)->tick = 0;
    ((BattleInitState *)btlRuntime)->unk224 = 0;
    ((BattleInitState *)btlRuntime)->listHeads[0] = 0;
    ((BattleInitState *)btlRuntime)->listHeads[1] = 0;
    ((BattleInitState *)btlRuntime)->listHeads[2] = 0;
    ((BattleInitState *)btlRuntime)->listHeads[3] = 0;
    ((BattleInitState *)btlRuntime)->listHeads[4] = 0;
    ((BattleInitState *)btlRuntime)->listHeads[5] = 0;
    mnuMovieTaskState = 2;
    ((BattleInitState *)btlRuntime)->unk4C4 = 0x1E;
    {
        BattleInitState *state = (BattleInitState *)btlRuntime;
        state->endCode = 0;
        state->unk4C8 = 1.0f;
    }
    ((BattleInitState *)btlRuntime)->unk5B8 = 0;
    ((BattleInitState *)btlRuntime)->unk5BC = 0x80808080;
    ((BattleInitState *)btlRuntime)->backgroundA = 0xC9;
    ((BattleInitState *)btlRuntime)->backgroundB = 1;
    ((BattleInitState *)btlRuntime)->unk2B0 = 0;
    ((BattleInitState *)btlRuntime)->unk2B4 = func_001B5600();
    seed = func_001003F8();
    effMiscSeedRandom(effSharedRandomState, seed);
    btlResetActorEntryState();

    ((BattleInitState *)btlRuntime)->messageWindows[0] =
        itfMesCreateWindow(D_00435E78);
    ((BattleInitState *)btlRuntime)->messageWindows[1] =
        itfMesCreateWindow(&D_003858D8);
    ((BattleInitState *)btlRuntime)->messageWindows[2] =
        itfMesCreateWindow(&D_00385228);
    sndResetTransition();
    func_002050D0();
    btlResetDeferredTaskQueue();
    btlResetToInitialScene();
    fldClearSceneSlotsAndGroups();
    btlResetFieldColorAndSweepFlags();
    btlRefreshSoundEntries();
    btlRetainButtonTexture();
    func_001CFC40();
    func_0020D118();
    btlClearModelFlagRange();
    mdlFlagClear(0x82B);

    if (mdlFlagTest(0x290) == 0) {
        mdlSetViewerSlotResourceHandles(0, 1, 0, D_003B4D90, 0);
        btlBossDebugPrintf(D_004366F8, D_003B4D90);
    } else {
        mdlSetViewerSlotResourceHandles(0, 1, 0, D_003B4DA8, 0);
        btlBossDebugPrintf(D_004366F8, D_003B4DA8);
    }
}

extern void itfMesDestroyWindowIfPresent(s32);

extern char D_00414F48[];

extern char D_00414F68[];

extern char D_00414F80[];

extern char D_00414F98[];

extern char D_00414FB0[];

extern s32 sndHasOccupiedNodeSlots(void);

extern s32 btlCountRegisteredTasks(void);

s32 btlExitWhenAudioAndTasksIdle(void) {
    BattleInitState *state;

    if (btlRuntime == 0) {
        btlBossDebugPrintf(D_00414F48);
        return 0;
    }
    if (sndHasOccupiedNodeSlots() != 0) {
        btlBossDebugPrintf(D_00414F68);
        return -1;
    }
    if (btlCountRegisteredTasks() != 0) {
        btlBossDebugPrintf(D_00414F80);
        btlFlagTasksForUpdate();
        return -1;
    }
    btlReleaseBossData();
    btlClearTaskLists();
    btlDestroyAllActionSeqs();
    btlDestroyAllUnits();
    btlClearPendingSoundList();
    btlReleaseEventAssets();
    btlGetCurrentSceneRecordValue();
    btlFreeFieldBlocks();
    btlStopRainSoundTransition();
    btlClearTintAndEnableCamera();
    fldDestroySceneTasksAndBuffers();
    btlReleaseButtonTexture();
    sndFreeBattleSoundEntries();
    sndClearList();
    brsTaskTryDestroy();
    btlClearSoundAndModelResources();
    btlDestroyDrawTaskAtPriorityWhenPresent();
    itfMesDestroyWindowIfPresent(((BattleInitState *)btlRuntime)->messageWindows[2]);
    itfMesDestroyWindowIfPresent(((BattleInitState *)btlRuntime)->messageWindows[1]);
    itfMesDestroyWindowIfPresent(((BattleInitState *)btlRuntime)->messageWindows[0]);
    btlAdvanceTitleStateWithAudioCleanup();
    kwlnCancelConfiguredFadeFrames();
    evtDestroySelectionState();
    effResetSlots();
    evtSetSolarOverlayFullyTransparent();
    itfMesClearFlags(1);

    state = (BattleInitState *)btlRuntime;
    if (state->commandRestrictFlags & 0x8000) {
        state->commandRestrictFlags &= ~0x8000;
        sdfSceneProjectionParameters.camera.farZ = 65536.0f;
        btlBossDebugPrintf(D_00414F98);
    }

    sdfReleaseResourceAllocation(D_004366E0);
    D_004366E0 = 0;
    btlRuntime = 0;
    btlBossDebugPrintf(D_00414FB0);
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00414F48);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00414F68);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00414F80);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00414F98);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00414FB0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001A9F30);

void btlLoadInputIconsAndSystemSounds(void) {
    btlOpenButtonIconResource();
    func_001CFB48();
    sndLoadSysEffLb();
}

u8 btlIsRuntimeAllocated(void) {
    return btlRuntime != 0;
}

s32 btlIsCurrentActorFullyMarked(void) {
    if (btlIsRuntimeAllocated() == 0) {
        return 0;
    }
    return (((BattleController *)btlRuntime)->flags & 0x06000000) == 0x06000000;
}

s32 btlHasPendingRuntimeActivity(void) {
    s32 state;
    if (btlIsRuntimeAllocated() == 0) {
        return 0;
    }
    state = btlRuntime;
    if (*(s32 *)(state + 0x248) != 0) {
        return 1;
    }
    if ((*(s32 *)(state + 0x1EC) & 2) != 0) {
        return 1;
    }
    return *(u32 *)(state + 0x718) != 0;
}

void btlResetActorEntryState(void) {
    s32 context = btlRuntime;
    DatPartyRecord *party = datGameState->party;
    u32 i;
    *(s32 *)(context + 0x2E8) = 0;
    *(s32 *)(context + 0x2EC) = 0;
    *(s32 *)(context + 0x2F0) = 0;
    *(s32 *)(context + 0x2F4) = 0;
    *(s32 *)(context + 0x2F8) = 0;
    *(u16 *)(context + 0x2FC) = 0;
    for (i = 0; i < 5; i++) {
        party[i].huntExp = 0;
    }
    memset((void *)(btlRuntime + 0x2DC), 0, 12);
}

s32 func_001AA400(BrsRewardSummary *rewards) {
    u32 i;

    if (btlIsRuntimeAllocated() == 0) {
        return 0;
    }
    rewards->macca = mdlFlagTest(0x290) ? ((BattleController *)btlRuntime)->rewardMacca : 0;
    rewards->mitama = ((BattleController *)btlRuntime)->specialEnemyDefeats;
    rewards->totalExp = ((BattleController *)btlRuntime)->experienceEarned;
    rewards->totalAp = ((BattleController *)btlRuntime)->rewardAp;
    btlBossDebugPrintf("btl:----------------------\n");
    btlBossDebugPrintf("btl:money =%d\n", rewards->macca);
    btlBossDebugPrintf("btl:exp   =%d\n", rewards->totalExp);
    btlBossDebugPrintf("btl:ep    =%d\n", rewards->totalAp);
    btlBossDebugPrintf("btl:mitama=%d\n", rewards->mitama);
    for (i = 0; i < 5; i++) {
        rewards->unitApBonus[i] = datGameState->party[i].huntExp;
        btlBossDebugPrintf(D_004150B0, rewards->unitApBonus[i], datGameState->party[i].unitId);
    }
    btlBossDebugPrintf("btl:----------------------\n");
    for (i = 0; i < 3; i++) {
        rewards->icons[i].id = ((BattleController *)btlRuntime)->itemDrops[i].id;
        rewards->icons[i].param = ((BattleController *)btlRuntime)->itemDrops[i].count;
    }
    return 1;
}

s32 btlResolveQueuedSceneRequestParameters(s32 *outCode, s32 *outParameter) {
    s32 buffer[2];
    s16 i;

    if (fldConsumeNextSceneRequest(&buffer[0], &buffer[1]) == 0) {
        return 0;
    }
    for (i = 0; i < 0x10; i++) {
        if (D_00435E14[i].unk00 + 0xC8 != buffer[0]) {
            continue;
        }
        *outCode = D_00435E14[i].unk00 + 0xC8;
        if (D_00435E14[i].entries[buffer[1]].unk08 != -1 &&
            mdlFlagTest(D_00435E14[i].entries[buffer[1]].unk08) != 0) {
            *outParameter = D_00435E14[i].entries[buffer[1]].unk0A;
            return 1;
        }
        if (D_00435E14[i].entries[buffer[1]].unk04 != -1 &&
            mdlFlagTest(D_00435E14[i].entries[buffer[1]].unk04) != 0) {
            *outParameter = D_00435E14[i].entries[buffer[1]].unk06;
            return 1;
        }
        *outParameter = D_00435E14[i].entries[buffer[1]].unk00;
        return 1;
    }
    return 0;
}

s32 btlGetRuntime(void) {
    return btlRuntime;
}

/* Read current HP from a unit-entry address. */
s32 btlReadCurrentUnitHp(DatPartyRecord *entry) {
    return entry->hp;
}

/* Read current MP from a unit-entry address. */
u16 btlReadCurrentUnitMp(DatPartyRecord *entry) {
    return entry->mp;
}

extern s32 ptyComputeMaxHp(DatPartyRecord *);

extern s32 ptyComputeMaxMp(DatPartyRecord *);

void btlComputeProfileMaxHp(DatPartyRecord *unit) {
    ptyComputeMaxHp(unit);
}

void btlComputeProfileMaxMp(DatPartyRecord *unit) {
    ptyComputeMaxMp(unit);
}

s32 btlComputeSkillAdjustedMaxHp(DatPartyRecord *stats) {
    return datComputeSkillBoostedMaxHp(stats);
}

s32 btlComputeSkillAdjustedMaxMp(DatPartyRecord *stats) {
    return datComputeSkillBoostedMaxMp(stats);
}

extern void datAdjustCurrentHp(DatPartyRecord *, s32);

extern void datAdjustCurrentMp(DatPartyRecord *, s32);

void btlAdjustUnitHp(DatPartyRecord *object, s32 value) {
    datAdjustCurrentHp(object, value);
}

void btlAdjustUnitMp(DatPartyRecord *object, s32 value) {
    datAdjustCurrentMp(object, value);
}

/* Cache the skill-adjusted maximum and return current HP clamped to it.
 * The comparison uses the full-width result, not the u16 cache. */
u16 btlRefreshUnitMaximumHpAndClampCurrentHp(DatPartyRecord *entryAddress) {
    DatPartyRecord *entry = entryAddress;
    u32 currentHp = btlReadCurrentUnitHp(entry);
    u32 maxHp = btlComputeSkillAdjustedMaxHp(entryAddress);
    entry->maxHp = maxHp;
    if (maxHp < currentHp) {
        entry->hp = maxHp;
    }
    return entry->hp;
}

/* Cache the skill-adjusted maximum and return current MP clamped to it.
 * The comparison uses the full-width result, not the u16 cache. */
u16 btlRefreshUnitMaximumMpAndClampCurrentMp(DatPartyRecord *entryAddress) {
    u16 currentMp = btlReadCurrentUnitMp(entryAddress);
    u32 maxMp = btlComputeSkillAdjustedMaxMp(entryAddress);
    entryAddress->maxMp = maxMp;
    if (maxMp < currentMp) {
        entryAddress->mp = maxMp;
    }
    return entryAddress->mp;
}

/* Return the low 15 status bits; do not expose the stored high bit. */
u16 btlReadUnitStatusMask(DatPartyRecord *entry) {
    return entry->status & BTL_ENTRY_STATUS_MASK;
}

void func_001AA850(void) {
    sdfRaisePackedChannelValue();
}

void func_001AA868(void) {
    datClearUnitStatusBits();
}

/* Set the actor's selected entry index. */
void btlSetActorSelectedEntryIndex(BtlUnit *actor, u32 index) {
    actor->selectedEntryIndex = index;
}

/* No selected entry is represented by -1. */
void btlClearActorSelectedEntryIndex(BtlUnit *actor) {
    actor->selectedEntryIndex = -1;
}

/* Initialize enemy vitals/stats and pack its nonzero skill IDs into the party record. */
void func_001AA898(DatPartyRecord *entry, s32 index) {
    u32 count;
    u32 i;

    memset(entry, 0, sizeof(*entry));
    entry->flags = 1;
    entry->affinityTableIndex = 0;
    entry->unitId = index;
    entry->level = datEnemyRecords[index].level;
    entry->totalExp = 0;
    for (i = 0; i < DAT_BASE_STAT_COUNT; i++) {
        entry->baseStats[i] = datEnemyRecords[index].baseStats[i];
    }
    count = 0;
    for (i = 0; i < 8; i++) {
        if (datEnemyRecords[index].skills[i] != 0) {
            entry->effectData[count++] = datEnemyRecords[index].skills[i];
        }
    }
    for (i = count; i < 24; i++) {
        entry->effectData[i] = 0;
    }
    entry->unk20 = count;
    entry->maxHp = datEnemyRecords[index].maxHp;
    entry->hp = datEnemyRecords[index].hp;
    entry->maxMp = datEnemyRecords[index].maxMp;
    entry->mp = datEnemyRecords[index].mp;
}

DatPartyRecord *btlGetActorEntryData(BtlUnit *actor) {
    DatPartyRecord *entry;

    if ((actor->status.flags & 0x400) == 0) {
        entry = btlGetIndexedPartyEntryRecord(actor->unk2E4);
        return entry;
    }
    return &actor->partyRecord;
}

s32 btlGetCurrentPartyEntryRecord(void) {
    s32 temp_v0;

    temp_v0 = dds3FindEntryIndex();
    return (s32)&datGameState->party[temp_v0];
}

DatPartyRecord *btlGetIndexedPartyEntryRecord(s32 index) {
    return &datGameState->party[index];
}

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004150B0);

void btlSyncPlayerWork(BtlUnit *actor) {
    DatPartyRecord *src = &actor->partyRecord;
    DatPartyRecord *dst = btlGetIndexedPartyEntryRecord(actor->unk2E4);
    s32 maxHp;
    s32 maxMp;
    if (src->flags & 0x1000) {
        dst->flags |= 0x1000;
    } else {
        dst->flags &= ~0x1000;
    }
    if (src->flags & 0x4000) {
        dst->flags |= 0x4000;
    } else {
        dst->flags &= ~0x4000;
    }
    dst->level = src->level;
    maxHp = datComputeSkillBoostedMaxHp(dst);
    maxMp = datComputeSkillBoostedMaxMp(dst);
    dst->hp = src->hp < maxHp ? src->hp : maxHp;
    dst->mp = src->mp < maxMp ? src->mp : maxMp;
    memcpy(dst->baseStats, src->baseStats, sizeof(dst->baseStats));
    dst->status = src->status & 0x7FFF;
    dst->unk1AC = src->unk1AC;
    dst->unk1AE = src->unk1AE;
    dst->actionSlot = (u16)src->actionSlot;
    btlBossDebugPrintf("btl:player work set[%p]\n", actor);
}

s32 btlFindPartyEntryIndexForActor(BtlUnit *object) {
    return dds3FindEntryIndex(object->partyRecord.unitId);
}

void func_001AABD8(void) {
}

BtlUnit *btlFindActiveActorByKind(s32 kind) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *unit;
    u32 flags;
    for (unit = battle->units; unit != 0; unit = unit->nextActor) {
        flags = unit->status.flags;
        if (flags & 1) {
            if (flags & 0x200) {
                if (kind == unit->unk2E4) {
                    return unit;
                }
            }
        }
    }
    return 0;
}

extern void btlCopyUnitStats(BtlUnit *, DatPartyRecord *);

void func_001AAC50(BtlUnit *unit, u8 sourceIndex, u8 priority) {
    DatPartyRecord saved;
    s32 insertion = 0;
    s32 i;

    if (datGameState->party[0].flags & 2) {
        do {
            if ((u16)(datGameState->party[insertion].flags & 1) == 0) {
                break;
            }
            if (priority < btlFindActiveActorByKind(insertion)->lookupId) {
                break;
            }
            insertion++;
            if (insertion >= 5) {
                break;
            }
        } while (datGameState->party[insertion].flags & 2);
    }

    memcpy(&saved, &datGameState->party[sourceIndex], sizeof(saved));
    for (i = sourceIndex; i < 4; i++) {
        memcpy(&datGameState->party[i], &datGameState->party[i + 1], sizeof(saved));
        if (datGameState->party[i + 1].flags & 2) {
            btlFindActiveActorByKind(i + 1)->unk2E4 = i;
        }
    }

    for (i = 4; i > insertion; i--) {
        memcpy(&datGameState->party[i], &datGameState->party[i - 1], sizeof(saved));
        if (datGameState->party[i - 1].flags & 2) {
            btlFindActiveActorByKind(i - 1)->unk2E4 = i;
        }
    }

    memcpy(&datGameState->party[i], &saved, sizeof(saved));
    btlCopyUnitStats(unit, &saved);
    unit->partyRecord.flags |= 2;
    datGameState->party[i].flags |= 2;
    unit->unk2E4 = i;
    func_001AABD8();
    btlBossDebugPrintf("btl:party in %d->%d[%d]\n", sourceIndex, i, saved.unitId);
}

void func_001AB160(BtlUnit *unit) {
    DatPartyRecord saved;
    DatGameState *scanState = datGameState;
    s32 originalIndex;
    s32 index;

    originalIndex = unit->unk2E4;
    memcpy(&saved, &datGameState->party[originalIndex], sizeof(saved));
    index = originalIndex;
    if (index < 4 && (u16)(scanState->party[index + 1].flags & 1)) {
        do {
            memcpy(&datGameState->party[index], &datGameState->party[index + 1], sizeof(saved));
            if (datGameState->party[index + 1].flags & 2) {
                btlFindActiveActorByKind(index + 1)->unk2E4 = index;
            }
            index++;
            if (index >= 4) {
                break;
            }
            scanState = datGameState;
        } while ((u16)(scanState->party[index + 1].flags & 1));
    }

    memcpy(&datGameState->party[index], &saved, sizeof(saved));
    unit->partyRecord.flags &= ~2;
    datGameState->party[index].flags &= ~2;
    unit->unk2E4 = 6;
    func_001AABD8();
    btlBossDebugPrintf("btl:party out %d->%d[%d]\n", originalIndex, index, saved.unitId);
}

extern const char D_00415130[];

/* Swap the actor's roster entry, refresh its stats, and mark the active entry. */
void func_001AB510(BtlUnit *actor, u8 targetIndex) {
    DatPartyRecord previous;

    if (actor->unk2E4 != targetIndex) {
        memcpy(&previous, &datGameState->party[actor->unk2E4], sizeof(previous));
        memcpy(&datGameState->party[actor->unk2E4], &datGameState->party[targetIndex], sizeof(previous));
        memcpy(&datGameState->party[targetIndex], &previous, sizeof(previous));

        btlCopyUnitStats(actor, &datGameState->party[actor->unk2E4]);
        actor->partyRecord.flags |= 2;
        datGameState->party[actor->unk2E4].flags |= 2;
        datGameState->party[targetIndex].flags &= ~2;
        func_001AABD8();
        btlBossDebugPrintf(D_00415130,
                           actor->unk2E4, targetIndex, previous.unitId,
                           actor->partyRecord.unitId);
    }
}

s32 btlGetEntryFlagsUnlessDisabled(DatPartyRecord *record) {
    if ((record->flags & 4) != 0) {
        return 0;
    }
    return datEnemyRecords[record->unitId].flags;
}

extern s32 datGetClampedProfileAdjustedStat(DatPartyRecord *, s32);

extern s32 datGetStatWithStatusOverride(DatPartyRecord *, s32);

void func_001AB8C0(DatPartyRecord *unit, s32 statIndex) {
    datGetClampedProfileAdjustedStat(unit, statIndex);
}

s32 func_001AB8D8(DatPartyRecord *unit, s32 statIndex) {
    return datGetStatWithStatusOverride(unit, statIndex);
}

s32 btlApplyCommandAbilityMultiplier(DatPartyRecord *battler, s32 command) {
    u32 value = datCalculateCommandBaseValue(battler, command);
    f32 scale;

    if (value == 0) {
        return 0;
    }
    scale = 1.0f;
    switch (datCommandRecords[command].costMode) {
    case DAT_COMMAND_COST_MODE_HP:
        if (btlCheckSpecialAbility(battler, 0x254)) {
            scale = datAbilityParameters[0x254 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        break;
    case DAT_COMMAND_COST_MODE_MP:
        if (btlCheckSpecialAbility(battler, 0x255)) {
            scale = datAbilityParameters[0x255 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        break;
    }
    value = (s32)((f32)value * scale);
    return value == 0 ? 1 : value;
}

s32 btlGetSlotValueAdjustedForSpecialAbility(BtlUnit *battler, s32 slot) {
    s32 value = datAffinityRecords[slot - DAT_AFFINITY_FIRST_COMMAND].slotCost;
    if (btlDoesEnabledStatusMatchCurrentId(&battler->partyRecord, 0xe4) && (u32)value >= 2) {
        value--;
    }
    return value;
}

s32 func_001ABA40(BtlUnit *unit, s32 command) {
    s32 result = 0;
    u32 cost = btlApplyCommandAbilityMultiplier(&unit->partyRecord, command);

    switch (datCommandRecords[command].costMode) {
    case DAT_COMMAND_COST_MODE_HP:
        if ((datCommandRecords[command].flags & 8) == 0) {
            if (unit->partyRecord.hp <= cost) {
                result = 1;
            }
        } else if (unit->partyRecord.hp < cost) {
            result = 1;
        }
        break;
    case DAT_COMMAND_COST_MODE_MP:
        if (unit->partyRecord.mp < cost) {
            result = 2;
        }
        break;
    }
    return result;
}

s32 func_001ABB10(BtlUnit *unit, s32 command) {
    u32 availableSlots;

    if (command == 0) {
        return 0;
    }
    if ((unit->partyRecord.flags & 0x10) && (unit->status.flags & 0x200)) {
        if ((datCommandRecords[command].effectType != 0 &&
             datCommandRecords[command].effectType != 2) ||
            datCommandRecords[command].costMode >= 2) {
            return 7;
        }
    }
    if ((datCommandRecords[command].effectType == 1 ||
         datCommandRecords[command].costMode == DAT_COMMAND_COST_MODE_MP) &&
        (unit->partyRecord.status & 0x7FFF) == 0x10) {
        return 3;
    }
    if (datCommandSelectors[command].kind == 2 &&
        (unit->partyRecord.status & 0x7FFF) == 0x40) {
        return 5;
    }
    if (datCommandSelectors[command].kind == 1) {
        if ((unit->partyRecord.status & 0x7FFF) == 0x1000) {
            return 4;
        }
        availableSlots = fldCountSceneSlots();
        if (availableSlots < btlGetSlotValueAdjustedForSpecialAbility(unit, command)) {
            return 6;
        }
    }
    if (datCommandRecords[command].flags & 4) {
        return 0;
    }
    return func_001ABA40(unit, command);
}

s32 btlGetCombinedPartyCommandPower(DatPartyRecord *base, BtlUnit *first, BtlUnit *second,
                  BtlUnit *third, s32 command) {
    DatPartyRecord snapshot = *base;
    s32 totalMaxHp = 0;
    s32 count = 0;
    s32 average;

    if (first != NULL) {
        totalMaxHp = first->partyRecord.maxHp;
        count = 1;
    }
    if (second != NULL) {
        totalMaxHp += second->partyRecord.maxHp;
        count++;
    }
    if (third != NULL) {
        totalMaxHp += third->partyRecord.maxHp;
        count++;
    }
    average = totalMaxHp / count;
    snapshot.maxHp = average;
    snapshot.hp = average;
    return btlApplyCommandAbilityMultiplier(&snapshot, command);
}

s32 func_001ABDE8(BtlUnit *base, BtlUnit *first, BtlUnit *second,
                  BtlUnit *third, s32 command) {
    BtlUnit snapshot = *base;
    s32 totalMaxHp = 0;
    s32 count = 0;

    if (first != NULL) {
        if ((first->status.flags & 0x100) == 0) {
            return 3;
        }
        if ((first->partyRecord.status & 0x2A0E) != 0) {
            return 3;
        }
        totalMaxHp = first->partyRecord.maxHp;
        count = 1;
    }
    if (second != NULL) {
        if ((second->status.flags & 0x100) == 0) {
            return 3;
        }
        if ((second->partyRecord.status & 0x2A0E) != 0) {
            return 3;
        }
        count++;
        totalMaxHp += second->partyRecord.maxHp;
    }
    if (third != NULL) {
        if ((third->status.flags & 0x100) == 0) {
            return 3;
        }
        if ((third->partyRecord.status & 0x2A0E) != 0) {
            return 3;
        }
        count++;
        totalMaxHp += third->partyRecord.maxHp;
    }
    snapshot.partyRecord.maxHp = totalMaxHp / count;
    return func_001ABB10(&snapshot, command);
}

s8 btlGetActorIndexedSignedValue(BtlUnit *object, s32 index) {
    if (index == 0 && (object->status.flags & 0x400) != 0) {
        return datEnemyRecords[object->partyRecord.unitId].unk46;
    }
    return datCommandSelectors[index].stat;
}

extern s32 btlResolveUnitValueWithOverride(s32, s32);

s32 func_001ABF50(BtlUnit *battler, s32 element) {
    return btlResolveUnitValueWithOverride((s32)&battler->partyRecord.flags, element);
}

extern s32 datGetEffectiveAffinity(DatPartyRecord *, s32);

s32 btlResolveUnitValueWithOverride(s32 statusAddress, s32 element) {
    s32 (*hook)(s32, s32) = *(s32 (**)(s32, s32))(btlGetRuntime() + 0x6B8);
    if (hook != 0) {
        s32 value = hook(statusAddress, element);
        if (value != -1) {
            return value;
        }
    }
    return datGetEffectiveAffinity((DatPartyRecord *)statusAddress, element);
}

s32 btlGetSideIndexedActorStatusTable(s32 arg0, s32 arg1) {
    if (arg0 == 0) {
        return D_00435DE0 + arg1 * 0x270;
    }
    return D_00435DF0 + arg1 * 0x270;
}

s32 btlSelectSharedOrIndexedTransformParameters(s32 arg0, s32 arg1) {
    if (arg0 == 0) {
        return (s32)D_003B4DC0;
    }
    return D_00435E00 + arg1 * 24;
}

s32 btlSelectSideIndexedActorParameterTable(s32 arg0, s32 arg1) {
    if (arg0 == 0) {
        return D_00435DE4 + arg1 * 0x74;
    }
    return D_00435DF8 + arg1 * 0x74;
}

s32 btlGetLoggedIndexedCommandItem(s32 index) {
    u16 item = datItemSkillRecords[index].commandIndex;
    btlBossDebugPrintf(D_00415158, index, item);
    return item;
}

u16 btlGetActorBedAssetIdFromIndex(s32 arg0) {
    return datItemSkillRecords[arg0].commandIndex;
}

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415130);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415158);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AC0F8);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AC360);

extern s32 btlFindEligibleTargetForMultiActorCommand(s32 arg0, BtlIndexList *arg1);

s32 btlFindEligibleTargetForMultiActorCommand(s32 arg0, BtlIndexList *targets) {
    ActionStateLink *action = (ActionStateLink *)arg0;
    u32 count;
    u32 i;
    s32 command;
    s32 index;

    if (action == NULL || targets == NULL) {
        return 0;
    }
    count = btlGetIndexListCount(targets);
    if (count < 2) {
        return 0;
    }
    command = action->indexWork.phase;
    switch (command) {
    case 2:
    case 3:
    case 4:
    case 7:
    case 8:
        if (command == 4) {
            index = btlGetLoggedIndexedCommandItem(action->indexWork.reference);
        } else {
            index = action->indexWork.skillId;
        }
        if (datCommandRecords[index].targetType == 0 &&
            datCommandRecords[index].attribute.parts.kind == DAT_COMMAND_ATTRIBUTE_KIND_FLAG_MASK &&
            datCommandRecords[index].attribute.parts.valueMask != 0) {
            for (i = 0; i < count; i++) {
                if ((datCommandRecords[index].attribute.parts.valueMask &
                     ((BtlUnit *)btlGetIndexListEntry(targets, i))->partyRecord.status) != 0) {
                    return i;
                }
            }
        }
        break;
    }
    return 0;
}

s32 btlMarkInactiveActorCandidates(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *eligible[16];
    BtlUnit *unit;
    BtlUnit *selected;
    BtlUnit **read;
    s32 count;
    s32 remaining;
    s32 lowestId;

    unit = battle->units;
    count = 0;
    if (unit != NULL) {
        do {
            s32 flags = unit->status.flags;
            if ((flags & 0x200) == 0) {
                goto next_actor;
            }
            if ((flags & 1) != 0) {
                goto next_actor;
            }
            eligible[count] = unit;
            unit->status.flags = flags & ~0x100;
            count++;
next_actor:
            unit = unit->nextActor;
        } while (unit != NULL);
    }
    if (count == 0) {
        return 0;
    }
    if (battle->unk268 == 3 && count == 2) {
        eligible[0]->status.flags |= 0x100;
        eligible[1]->status.flags |= 0x100;
    } else {
        lowestId = 4;
        unit = NULL;
        if (count > 0) {
            remaining = count;
            read = eligible;
            do {
                BtlUnit *candidate = *read++;
                s32 lookupId = candidate->lookupId;
                if (lookupId < lowestId) {
                    unit = candidate;
                    lowestId = lookupId;
                }
            } while (--remaining != 0);
        }
        unit->status.flags |= 0x100;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AC750);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001ACD10);

u32 btlEncodeActorIndexAsSelectionMask(u32 id) {
    u32 mask;
    switch (id) {
    case 0xFFFFFFFF: mask = 0x1; break;
    case 0: mask = 0x2; break;
    case 1: mask = 0x4; break;
    case 2: mask = 0x8; break;
    case 3: mask = 0x10; break;
    case 4: mask = 0x20; break;
    case 5: mask = 0x40; break;
    case 6: mask = 0x80; break;
    case 7: mask = 0x100; break;
    case 8: mask = 0x200; break;
    case 9: mask = 0x400; break;
    case 10: mask = 0x800; break;
    case 11: mask = 0x1000; break;
    case 12: mask = 0x2000; break;
    case 13: mask = 0x4000; break;
    case 14: mask = 0x8000; break;
    case 15: mask = 0x10000; break;
    case 16: mask = 0x20000; break;
    case 17: mask = 0x40000; break;
    case 18: mask = 0x80000; break;
    default: mask = 0; break;
    }
    return mask;
}

extern s32 datFlagToElementIndex(s32);

s32 func_001AD090(s32 flag) {
    return datFlagToElementIndex(flag);
}

extern s32 datUnitHasSkill(DatPartyRecord *, s32);

extern s32 evtGetMirroredSolarPhase(void);

s32 btlCheckSpecialAbility(DatPartyRecord *record, s32 ability) {
    if (datUnitHasSkill(record, ability) == 0) {
        return 0;
    }
    switch (ability) {
    case 0x227:
        return evtGetMirroredSolarPhase() == 8;
    case 0x228:
        return evtGetMirroredSolarPhase() == 0;
    default:
        return 1;
    }
}

typedef struct DatSkillOwner {
    u16 flags;
    u16 unk2;
    u16 partyIndex;
    u8 unk6[0x1C];
    u16 skills[0x18];
} DatSkillOwner;

s32 func_001AD118(DatSkillOwner *unit, s32 skill) {
    s32 i;

    if (!(unit->flags & 0x20)) {
        for (i = 0; i < 8; i++) {
            if (unit->skills[i] == skill) {
                return 1;
            }
        }
    } else {
        for (i = 0; i < 8; i++) {
            if (datEnemyRecords[unit->partyIndex].skills[i] == skill) {
                return 1;
            }
        }
    }
    return 0;
}

s32 btlDoesEnabledStatusMatchCurrentId(DatPartyRecord *status, u32 value) {
    if (status->flags & 0x20) {
        return 0;
    }
    return mnuGetPartyEntryCurrentId(status) == value;
}

s32 btlSelectActorAction(s32 state) {
    s32 selection = func_001AD310(state, 1);
    if (selection == 0) {
        selection = (effMiscRand((s32)effSharedRandomState) & 1) ? 2 : 9;
    }
    return selection;
}

s32 btlIsEventThresholdSatisfiedForEntry(s32 arg) {
    s32 id = arg & 0xFFFF;
    switch (id) {
    case 0x8A:
        if (evtCheckValueThreshold(0x8A, 1) != 0 || evtCheckValueThreshold(0x93, 1) != 0) {
            return 1;
        }
        break;
    case 0x8B:
        if (evtCheckValueThreshold(0x8B, 1) != 0 || evtCheckValueThreshold(0x94, 1) != 0) {
            return 1;
        }
        break;
    }
    if ((u32)((id + 0xFF80) & 0xFFFF) < 0x20 && evtCheckValueThreshold(id, 1) != 0) {
        return 1;
    }
    return 0;
}

extern s32 btlRollAiBucket(void);

extern u32 effMiscRandMod(void *state, u32 modulus);

u32 func_001AD310(BtlUnit *enemyUnit, s32 mode) {
    u16 count = 0;
    u16 bonus = 0;
    u32 selected = 0;
    BtlState *battle = (BtlState *)btlGetRuntime();
    DatEnemyRecord *enemy = &datEnemyRecords[enemyUnit->partyRecord.unitId];
    BtlUnit *unit;
    u16 total;
    u16 index;
    u16 roll;

    for (unit = battle->units; unit != NULL; unit = unit->nextActor) {
        u32 flags = unit->status.flags;
        if (flags & 1) {
            if (flags & 0x200) {
                if (!(flags & 0xE0)) {
                    if (btlCheckSpecialAbility(&unit->partyRecord, 0x27B)) {
                        count++;
                    }
                }
            }
        }
    }
    if (count != 0) {
        switch (count) {
        case 1:
            bonus = datAbilityParameters[0x27B - BTL_ABILITY_PARAMETER_FIRST_SKILL].parameterA;
            break;
        case 2:
            bonus = datAbilityParameters[0x27B - BTL_ABILITY_PARAMETER_FIRST_SKILL].parameterA;
            bonus += (u16)datAbilityParameters[0x27B - BTL_ABILITY_PARAMETER_FIRST_SKILL].parameterB;
            break;
        default:
            bonus = datAbilityParameters[0x27B - BTL_ABILITY_PARAMETER_FIRST_SKILL].parameterA;
            bonus += datAbilityParameters[0x27B - BTL_ABILITY_PARAMETER_FIRST_SKILL].parameterB * 2;
            break;
        }
    }
    if (mode != 1 && enemy->overrideAction != 0 && enemy->overrideFlag != 0 &&
        mdlFlagTest(enemy->overrideFlag)) {
        if ((u8)btlRollAiBucket() < enemy->overrideChance) {
            selected = enemy->overrideAction;
        }
    }
    if (selected == 0) {
        total = 0;
        for (index = 0; index < 2; index++) {
            if (enemy->unk3E[index] != 0) {
                if (btlIsEventThresholdSatisfiedForEntry(enemy->unk3E[index])) {
                    btlBossDebugPrintf("btl:event item check[%X]\n", enemy->unk3E[index]);
                } else {
                    total += enemy->actionChances[index];
                }
            }
        }
        if (total != 0 && btlRollAiBucket() < total + bonus) {
            roll = effMiscRandMod(0, total);
            total = 0;
            for (index = 0; index < 2; index++) {
                if (enemy->unk3E[index] != 0) {
                    if (btlIsEventThresholdSatisfiedForEntry(enemy->unk3E[index])) {
                        btlBossDebugPrintf("btl:event item check[%X]\n", enemy->unk3E[index]);
                    } else {
                        total += enemy->actionChances[index];
                        if (roll < total) {
                            selected = enemy->unk3E[index];
                            break;
                        }
                    }
                }
            }
        }
    }
    return selected;
}

void func_001AD5B0(u16 item) {
    BattleController *controller = (BattleController *)btlGetRuntime();

    if (item != 0) {
        s32 found = 0;
        u16 i;

        for (i = 0; i < 3; i++) {
            if (controller->itemDrops[i].id == item) {
                found = 1;
                controller->itemDrops[i].count++;
                break;
            }
        }
        if (!found) {
            for (i = 0; i < 3; i++) {
                if (controller->itemDrops[i].id == 0) {
                    controller->itemDrops[i].id = item;
                    controller->itemDrops[i].count = 1;
                    break;
                }
            }
        }
    }
}

extern s32 func_001B39E8(u32);

extern s32 btlCalculateEnemyExperienceReward(BtlUnit *, BtlUnit *);

extern s32 btlGetEnemyMoney(BtlUnit *, BtlUnit *);

extern char D_004151E8[];

extern char D_00415208[];

extern char D_00415220[];

extern char D_00415238[];

extern char D_00415250[];

void btlAccumulateEnemyDefeatRewards(BtlUnit *enemy) {
    BattleController *controller = (BattleController *)btlGetRuntime();
    DatEnemyRecord *record = &datEnemyRecords[enemy->partyRecord.unitId];
    s32 level = func_001B39E8(4);
    s32 enemyLevel = record->level;
    s32 allowance = datBattleParameters->rewardLevelAllowance;
    s32 reward;
    s32 amount;
    s32 item;
    s32 kind;

    if (level > 62) {
        level = 62;
    }
    if (level >= enemyLevel + allowance && datBattleParameters->rewardDivisor != 0.0f) {
        reward = (s32)(record->unk2C / datBattleParameters->rewardDivisor);
        btlBossDebugPrintf(D_004151E8, reward, level, enemyLevel, allowance,
                          datBattleParameters->rewardDivisor);
    } else {
        reward = record->unk2C;
    }
    if (record->flags & 0x2000) {
        reward *= 100;
    }
    if (controller->mode == 3) {
        reward = (s32)(reward * datBattleParameters->majinRewardScale);
        btlBossDebugPrintf(D_00415208, reward, datBattleParameters->majinRewardScale);
    }
    controller->experienceEarned += reward;
    if ((btlUnitStatusPair(enemy) & 0x200800000ULL) == 0) {
        amount = btlCalculateEnemyExperienceReward(NULL, enemy);
        controller->epEarned += amount;
        btlBossDebugPrintf(D_00415220, controller->epEarned, amount);
    }
    if ((enemy->status.stateFlags & 0x400) == 0) {
        amount = btlGetEnemyMoney(NULL, enemy);
        controller->moneyEarned += amount;
        btlBossDebugPrintf(D_00415238, controller->moneyEarned, amount);
    }
    item = func_001AD310((s32)enemy, 0);
    if (item != 0) {
        func_001AD5B0(item);
    }
    kind = enemy->partyRecord.unitId;
    if (kind < 104) {
        if (kind >= 100) {
            controller->specialEnemyDefeats++;
        }
    }
    enemy->status.stateFlags |= 1;
    btlBossDebugPrintf(D_00415250, enemy);
}

/* Readiness requires each active actor to have cleared transient action flags. */
s32 btlAllActiveUnitsReady(void) {
    BtlUnit *unit;
    u32 flags;
    for (unit = ((BattleController *)btlGetRuntime())->actors; unit != 0; unit = unit->nextActor) {
        flags = unit->status.flags;
        if (flags & 1) {
            if (flags & 0x400) {
                if (flags & 0xC0) {
                    return 0;
                }
                if (flags & 0x20) {
                    if (!(unit->status.stateFlags & 1)) {
                        return 0;
                    }
                }
            }
        }
    }
    return 1;
}

typedef struct BattleAdjustmentEntry {
    u16 sceneIndex;
    u16 weight;
    s8 value;
    u8 unk05;
} BattleAdjustmentEntry;

typedef struct BattleAdjustmentGroup {
    s32 interval;
    BattleAdjustmentEntry entries[20];
} BattleAdjustmentGroup;

typedef struct BattleAdjustmentRecord {
    u8 pad00[8];
    u32 conditions[3];
    u8 variantCodes[8];
    BattleAdjustmentGroup groups[3];
} BattleAdjustmentRecord;

extern BattleAdjustmentRecord *D_00435E0C;

f32 func_001AD978(void) {
    BattleController *runtime = (BattleController *)btlGetRuntime();
    s32 adjustment = D_00435E0C[runtime->adjustmentRecordIndex]
                         .groups[runtime->adjustmentGroupIndex]
                         .entries[runtime->adjustmentEntryIndex]
                         .value;

    if (adjustment < -3) {
        adjustment = -3;
    } else if (adjustment > 3) {
        adjustment = 3;
    }
    return datBattleParameters->adjustmentScale[adjustment + 3];
}

u32 func_001ADA10(void) {
    s32 controller;

    controller = btlGetRuntime();
    return *(u32 *)(controller + 0x278);
}

extern u32 effMiscRandMod(void *, u32);

/* Return a random eligible actor task, or 0 if no candidate is available. */
s32 btlChooseAvailableUnit(void) {
    s32 candidates[16];
    s32 count = 0;
    ActionStateLink *node;
    BtlUnit *unit;
    u32 flags;
    for (node = ((BattleController *)btlGetRuntime())->taskHead; node != 0; node = node->next) {
        if (!(node->pendingFlags & 8)) {
            continue;
        }
        unit = node->unit;
        flags = unit->status.flags;
        if (flags & 1) {
            if (flags & 0x200) {
                if (flags & 2) {
                    if (!(flags & 0xE0)) {
                        candidates[count] = (s32)node;
                        count++;
                    }
                }
            }
        }
    }
    if (count == 0) {
        return 0;
    }
    return candidates[effMiscRandMod(0, count)];
}

s32 btlCountAvailableUnits(void) {
    BtlUnit *unit;
    s32 count = 0;
    u32 flags;
    for (unit = ((BattleController *)btlGetRuntime())->actors; unit != 0; unit = unit->nextActor) {
        flags = unit->status.flags;
        if (flags & 1) {
            if (flags & 0x200) {
                if (!(flags & 0xE0)) {
                    count++;
                }
            }
        }
    }
    return count;
}

/* Count ready scene actors plus eligible party entries stored in script state. */
s32 btlCountAvailableParticipants(void) {
    BtlUnit *unit;
    DatPartyRecord *entry;
    s32 count = 0;
    s32 i;
    u32 flags;
    for (unit = ((BattleController *)btlGetRuntime())->actors; unit != 0; unit = unit->nextActor) {
        flags = unit->status.flags;
        if (flags & 1) {
            if (flags & 0x200) {
                if (!(flags & 0xE0)) {
                    count++;
                }
            }
        }
    }
    entry = datGameState->party;
    for (i = 4; i >= 0; i--) {
        if (entry->flags & 1) {
            if (!(entry->flags & 2)) {
                if (!(entry->status & 0x4000)) {
                    count++;
                }
            }
        }
        entry++;
    }
    return count;
}

f32 btlGetActorStateScale(ActionStateLink *task) {
    if (task == NULL) {
        return 1.0f;
    }
    if (((BtlState *)btlGetRuntime())->battleFlags & 0x8000) {
        if ((btlUnitStatusPair(task->unit) & 0x1200) != 0x200
            || (task->unit->partyRecord.flags & 0x10)) {
            return 3.0f;
        }
    }
    return 1.0f;
}

void btlClearActorEntrySlot(BtlUnit *unit, s32 index);

void btlClearAllActorEntrySlots(BtlUnit *unit) {
    u32 temp_v0;
    u32 temp_v1;

    temp_v1 = 0;
    do {
        temp_v0 = temp_v1 + 1;
        btlClearActorEntrySlot(unit, temp_v1);
        temp_v1 = temp_v0;
    } while (temp_v0 < 7);
}

s32 btlActorEntryIsExpired(BtlUnit *unit, s32 index) {
    if (unit->entrySlots[index].code == 0) {
        return 0;
    }
    return unit->entrySlots[index].countdown < 1;
}

typedef struct EntryPair {
    s16 first;
    s16 second;
    s16 initialValue;
    s16 countdown;
} EntryPair;

extern EntryPair D_003B4DF0[];

s32 btlMatchActorEntryCode(BtlUnit *unit, s32 index) {
    s16 value = unit->entrySlots[index].code;
    if (D_003B4DF0[index].first != 0) {
        if (D_003B4DF0[index].first == value) {
            return 1;
        }
    }
    if (D_003B4DF0[index].second != 0) {
        if (D_003B4DF0[index].second == value) {
            return 2;
        }
    }
    return 0;
}

void func_001ADD30(BtlUnit *unit, s32 index, s16 delta) {
    s16 code = unit->entrySlots[index].code;
    code += delta;

    if (code > D_003B4DF0[index].first) {
        code = D_003B4DF0[index].first;
    }
    if (code < D_003B4DF0[index].second) {
        code = D_003B4DF0[index].second;
    }
    if (code != 0) {
        unit->entrySlots[index].unk02 = D_003B4DF0[index].initialValue;
        unit->entrySlots[index].countdown = D_003B4DF0[index].countdown;
    }
    unit->entrySlots[index].code = code;
}

void btlSetActorEntryCode(BtlUnit *unit, s32 index, u16 code) {
    unit->entrySlots[index].code = code;
}

void btlClearActorEntrySlot(BtlUnit *unit, s32 index) {
    unit->entrySlots[index].code = 0;
    unit->entrySlots[index].unk02 = -1;
    unit->entrySlots[index].countdown = -1;
}

s16 btlGetActorEntryCode(BtlUnit *unit, s32 index) {
    return unit->entrySlots[index].code;
}

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004151E8);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415208);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415220);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415238);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415250);

f32 btlGetActorEntryMultiplier(BtlUnit *unit, u32 index, s8 includeCharge) {
    f32 factor;
    s32 stage;

    stage = btlGetActorEntryCode(unit, index);
    factor = 1.0f;
    switch (index) {
    case 3:
        if (unit->status.flags & 0x200) {
            factor = (datBattleParameters->partyEntryScaleA + 3)[-stage];
        } else {
            factor = (datBattleParameters->enemyEntryScaleA + 3)[-stage];
        }
        break;
    case 2:
        if (unit->status.flags & 0x200) {
            factor = (datBattleParameters->partyEntryScaleB + 3)[-stage];
        } else {
            factor = (datBattleParameters->enemyEntryScaleB + 3)[-stage];
        }
        break;
    case 0:
    case 1:
        if (unit->status.flags & 0x200) {
            factor = (datBattleParameters->partyEntryScaleA + 3)[stage];
        } else {
            factor = (datBattleParameters->enemyEntryScaleA + 3)[stage];
        }
        break;
    case 4:
        if (unit->status.flags & 0x200) {
            factor = (datBattleParameters->partyEntryScaleB + 3)[stage];
        } else {
            factor = (datBattleParameters->enemyEntryScaleB + 3)[stage];
        }
        break;
    }
    if (index == 0 && includeCharge != 0 &&
        btlGetActorEntryCode(unit, 5) > 0) {
        if (btlActorEntryIsExpired(unit, 5) != 0) {
            factor *= 2.25f;
            btlBossDebugPrintf("btl:BUTURIx2\n");
        }
    }
    if (index == 1 && includeCharge != 0 &&
        btlGetActorEntryCode(unit, 6) > 0) {
        if (btlActorEntryIsExpired(unit, 6) != 0) {
            factor *= 2.25f;
            btlBossDebugPrintf("btl:MAGICx2\n");
        }
    }
    return factor;
}

void func_001ADFE0(BtlUnit *unit, u32 flags, s16 delta) {
    if (flags == 0) {
        return;
    }
    if (flags & 1) {
        func_001ADD30(unit, 0, delta);
    }
    if (flags & 2) {
        func_001ADD30(unit, 0, -delta);
    }
    if (flags & 4) {
        func_001ADD30(unit, 1, delta);
    }
    if (flags & 8) {
        func_001ADD30(unit, 1, -delta);
    }
    if (flags & 0x10) {
        func_001ADD30(unit, 2, delta);
    }
    if (flags & 0x20) {
        func_001ADD30(unit, 2, -delta);
    }
    if (flags & 0x40) {
        func_001ADD30(unit, 3, delta);
    }
    if (flags & 0x80) {
        func_001ADD30(unit, 3, -delta);
    }
    if (flags & 0x100) {
        func_001ADD30(unit, 4, delta);
    }
    if (flags & 0x200) {
        func_001ADD30(unit, 4, -delta);
    }
    if (flags & 0x400) {
        func_001ADD30(unit, 5, delta);
    }
    if (flags & 0x2000) {
        func_001ADD30(unit, 6, delta);
    }
    if (flags & 0x800) {
        if (btlGetActorEntryCode(unit, 0) > 0) {
            btlSetActorEntryCode(unit, 0, 0);
        }
        if (btlGetActorEntryCode(unit, 1) > 0) {
            btlSetActorEntryCode(unit, 1, 0);
        }
        if (btlGetActorEntryCode(unit, 2) > 0) {
            btlSetActorEntryCode(unit, 2, 0);
        }
        if (btlGetActorEntryCode(unit, 3) > 0) {
            btlSetActorEntryCode(unit, 3, 0);
        }
        if (btlGetActorEntryCode(unit, 4) > 0) {
            btlSetActorEntryCode(unit, 4, 0);
        }
    }
    if (flags & 0x1000) {
        if (btlGetActorEntryCode(unit, 0) < 0) {
            btlSetActorEntryCode(unit, 0, 0);
        }
        if (btlGetActorEntryCode(unit, 1) < 0) {
            btlSetActorEntryCode(unit, 1, 0);
        }
        if (btlGetActorEntryCode(unit, 2) < 0) {
            btlSetActorEntryCode(unit, 2, 0);
        }
        if (btlGetActorEntryCode(unit, 3) < 0) {
            btlSetActorEntryCode(unit, 3, 0);
        }
        if (btlGetActorEntryCode(unit, 4) < 0) {
            btlSetActorEntryCode(unit, 4, 0);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", btlLowestSetPairIndex);

void btlTickActorEntryCountdowns(BtlUnit *unit) {
    s16 *entry = &unit->entrySlots[0].countdown;
    u32 i;
    for (i = 0; i < 7; i++) {
        if (*entry >= 0) {
            if (*entry == 0) {
                *entry = -1;
            }
            *entry = *entry - 1;
        }
        entry += 3;
    }
}

f32 func_001AE3A8(BtlUnit *unit, s32 attr) {
    f32 scale = 1.0f;

    switch (attr) {
    case 2:
        if (btlCheckSpecialAbility(&unit->partyRecord, 0x22D)) {
            scale *= datAbilityParameters[0x22D - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        if (btlCheckSpecialAbility(&unit->partyRecord, 0x232)) {
            scale *= datAbilityParameters[0x232 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        if (scale == 1.0f && (unit->status.flags & 0x200)) {
            if (unit->partyRecord.unitId == 3) {
                scale *= datBattleParameters->unkBFC;
            }
        }
        break;
    case 3:
        if (btlCheckSpecialAbility(&unit->partyRecord, 0x22E)) {
            scale *= datAbilityParameters[0x22E - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        if (btlCheckSpecialAbility(&unit->partyRecord, 0x233)) {
            scale *= datAbilityParameters[0x233 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        if (scale == 1.0f && (unit->status.flags & 0x200)) {
            if (unit->partyRecord.unitId == 1) {
                scale *= datBattleParameters->unkBFC;
            }
            if (unit->partyRecord.unitId == 2) {
                scale *= datBattleParameters->unkBFC;
            }
        }
        break;
    case 4:
        if (btlCheckSpecialAbility(&unit->partyRecord, 0x22F)) {
            scale *= datAbilityParameters[0x22F - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        if (btlCheckSpecialAbility(&unit->partyRecord, 0x234)) {
            scale *= datAbilityParameters[0x234 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        if (scale == 1.0f && (unit->status.flags & 0x200)) {
            if (unit->partyRecord.unitId == 6) {
                scale *= datBattleParameters->unkBFC;
            }
            if (unit->partyRecord.unitId == 7) {
                scale *= datBattleParameters->unkBFC;
            }
        }
        break;
    case 5:
        if (btlCheckSpecialAbility(&unit->partyRecord, 0x230)) {
            scale *= datAbilityParameters[0x230 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        if (btlCheckSpecialAbility(&unit->partyRecord, 0x235)) {
            scale *= datAbilityParameters[0x235 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        if (scale == 1.0f && (unit->status.flags & 0x200)) {
            if (unit->partyRecord.unitId == 5) {
                scale *= datBattleParameters->unkBFC;
            }
        }
        break;
    case 6:
        if (btlCheckSpecialAbility(&unit->partyRecord, 0x231)) {
            scale *= datAbilityParameters[0x231 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        if (btlCheckSpecialAbility(&unit->partyRecord, 0x236)) {
            scale *= datAbilityParameters[0x236 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
        }
        if (scale == 1.0f && (unit->status.flags & 0x200)) {
            if (unit->partyRecord.unitId == 4) {
                scale *= datBattleParameters->unkBFC;
            }
        }
        break;
    }
    return scale;
}

s32 func_001AE678(BtlUnit *actor, s32 attr) {
    u32 value = 100;

    if (((BattleController *)btlGetRuntime())->flags21C & 0x20000) {
        return value;
    }
    switch (attr) {
    case 0:
    case 1:
        if (btlCheckSpecialAbility(&actor->partyRecord, 0x25C)) {
            value = (u32)(datAbilityParameters[0x25C - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)value);
        }
        break;
    case 2:
        if (btlCheckSpecialAbility(&actor->partyRecord, 0x25D)) {
            value = (u32)(datAbilityParameters[0x25D - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)value);
        }
        break;
    case 3:
        if (btlCheckSpecialAbility(&actor->partyRecord, 0x25E)) {
            value = (u32)(datAbilityParameters[0x25E - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)value);
        }
        break;
    case 4:
        if (btlCheckSpecialAbility(&actor->partyRecord, 0x25F)) {
            value = (u32)(datAbilityParameters[0x25F - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)value);
        }
        break;
    case 5:
        if (btlCheckSpecialAbility(&actor->partyRecord, 0x260)) {
            value = (u32)(datAbilityParameters[0x260 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)value);
        }
        break;
    case 6:
        if (btlCheckSpecialAbility(&actor->partyRecord, 0x261)) {
            value = (u32)(datAbilityParameters[0x261 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)value);
        }
        break;
    case 8:
        if (btlCheckSpecialAbility(&actor->partyRecord, 0x262)) {
            value = (u32)(datAbilityParameters[0x262 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)value);
        }
        break;
    case 9:
        if (btlCheckSpecialAbility(&actor->partyRecord, 0x263)) {
            value = (u32)(datAbilityParameters[0x263 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * (f32)value);
        }
        break;
    }
    return value;
}

s32 btlHasMappedSpecialAbilityForSlot(BtlUnit *unit, u32 slot) {
    if (((BattleController *)btlGetRuntime())->flags21C & 0x20000) {
        return 0;
    }
    if (slot < 2 && btlCheckSpecialAbility(&unit->partyRecord, 0x26F)) {
        return 1;
    }
    if (slot == 8 && btlCheckSpecialAbility(&unit->partyRecord, 0x271)) {
        return 1;
    }
    if (slot == 9 && btlCheckSpecialAbility(&unit->partyRecord, 0x272)) {
        return 1;
    }
    if (slot == 10 && btlCheckSpecialAbility(&unit->partyRecord, 0x264)) {
        return 1;
    }
    if (slot == 11 && btlCheckSpecialAbility(&unit->partyRecord, 0x265)) {
        return 1;
    }
    if (slot == 12 && btlCheckSpecialAbility(&unit->partyRecord, 0x266)) {
        return 1;
    }
    if (slot == 13 && btlCheckSpecialAbility(&unit->partyRecord, 0x267)) {
        return 1;
    }
    if (slot == 14 && btlCheckSpecialAbility(&unit->partyRecord, 0x268)) {
        return 1;
    }
    if (slot < 15) {
        if (slot >= 10 && btlCheckSpecialAbility(&unit->partyRecord, 0x273)) {
            return 1;
        }
    }
    if (slot < 7) {
        if (slot >= 2 && btlCheckSpecialAbility(&unit->partyRecord, 0x277)) {
            return 1;
        }
    }
    if (slot != 7) {
        if (btlCheckSpecialAbility(&unit->partyRecord, 0x269)) {
            return 1;
        }
        if (btlDoesEnabledStatusMatchCurrentId(&unit->partyRecord, 0xF7)) {
            return 1;
        }
    }
    if (slot == 3 && btlDoesEnabledStatusMatchCurrentId(&unit->partyRecord, 0xF2)) {
        return 1;
    }
    return 0;
}

s32 btlHasSpecialAbility274(BtlUnit *unit, u32 slot) {
    if (((BattleController *)btlGetRuntime())->flags21C & 0x20000) {
        return 0;
    }
    if (slot < 2 && btlCheckSpecialAbility(&unit->partyRecord, 0x274)) {
        return 1;
    }
    return 0;
}

s32 btlHasEnabledSpecialAbilityForSlot(BtlUnit *unit, u32 slot) {
    if (((BattleController *)btlGetRuntime())->flags21C & 0x20000) {
        return 0;
    }
    if (slot == 8 && btlCheckSpecialAbility(&unit->partyRecord, 0x275)) {
        return 1;
    }
    if (slot == 9 && btlCheckSpecialAbility(&unit->partyRecord, 0x276)) {
        return 1;
    }
    if (slot < 7) {
        if (slot >= 2 && btlCheckSpecialAbility(&unit->partyRecord, 0x278)) {
            return 1;
        }
    }
    if (slot == 8 && btlDoesEnabledStatusMatchCurrentId(&unit->partyRecord, 0xF0)) {
        return 1;
    }
    if (slot == 9 && btlDoesEnabledStatusMatchCurrentId(&unit->partyRecord, 0xF1)) {
        return 1;
    }
    return 0;
}

f32 func_001AEC18(BtlUnit *unit) {
    DatPartyRecord *entry = &unit->partyRecord;
    s32 maximum;
    s32 percentage;

    if (btlCheckSpecialAbility(entry, 0x27A) == 0) {
        return 1.0f;
    }
    maximum = btlComputeSkillAdjustedMaxHp(entry);
    percentage = (s32)((f32)btlReadCurrentUnitHp(entry) / (f32)maximum * 100.0f);
    if (percentage < 6) {
        return 3.0f;
    }
    if (percentage < 11) {
        return 2.2f;
    }
    if (percentage < 16) {
        return 1.7f;
    }
    if (percentage < 21) {
        return 1.4f;
    }
    if (percentage < 26) {
        return 1.2f;
    }
    if (percentage < 31) {
        return 1.1f;
    }
    return 1.0f;
}

f32 btlGetClampedBattleTableValue(BtlUnit *source) {
    BtlState *state = (BtlState *)btlGetRuntime();
    u16 index = state->activeGroupCount;
    if (index > 4) {
        index = 4;
    }
    return D_003B4E28[index];
}

s32 sndGetResourceForIndex(s32 index) {
    s8 resource = datCommandSelectors[index].stat;
    if (resource < 0) {
        return 0;
    }
    return (s32)D_003B4E40[resource];
}

/* The label resource index is the copied party record's unit ID. */
void *btlGetIndexedUiResource(BtlUnit *unit) {
    return D_003B4E88[unit->partyRecord.unitId];
}

typedef struct BattleSavedActorState {
    BtlUnitEntrySlot entrySlots[7];
    u8 pad2A[2];
    s32 flags;
    u8 pad30[4];
    u16 status;
    u16 unitId;
} BattleSavedActorState;

typedef char BattleSavedActorStateSize[(sizeof(BattleSavedActorState) == 0x38) ? 1 : -1];

extern BattleSavedActorState D_00452EA0[3];

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_004152F8);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415308);

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415318);

void func_001AED98(void) {
    s32 count = 0;
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *unit;

    memset(D_00452EA0, 0, sizeof(D_00452EA0));
    for (unit = battle->units; unit != NULL; unit = unit->nextActor) {
        if (unit->status.flags & 1) {
            if (unit->status.flags & 0x200) {
                D_00452EA0[count].flags = unit->status.flags;
                D_00452EA0[count].unitId = unit->partyRecord.unitId;
                memcpy(D_00452EA0[count].entrySlots, unit->entrySlots,
                       sizeof(D_00452EA0[count].entrySlots));
                D_00452EA0[count].status = unit->partyRecord.status & 0xCFF9;
                count++;
                func_0035B6E0("btl:state push[%d:%X]\n", count,
                              unit->partyRecord.unitId);
            }
        }
    }
}

extern const char D_00415340[];

void func_001AEEA8(void) {
    u32 index;
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *unit;

    for (index = 0; index < 3; index++) {
        if (D_00452EA0[index].unitId == 0) {
            continue;
        }
        for (unit = battle->units; unit != NULL; unit = unit->nextActor) {
            if (unit->status.flags & 1) {
                if (unit->status.flags & 0x200) {
                    if (unit->partyRecord.unitId == D_00452EA0[index].unitId) {
                        if (D_00452EA0[index].flags & 0x1000) {
                            unit->status.flags |= 0x1000;
                            unit->partyRecord.flags |= 0x1000;
                        } else {
                            unit->status.flags &= ~0x1000;
                            unit->partyRecord.flags &= ~0x1000;
                        }
                        memcpy(unit->entrySlots, D_00452EA0[index].entrySlots,
                               sizeof(unit->entrySlots));
                        unit->partyRecord.status = D_00452EA0[index].status;
                        if ((unit->partyRecord.status & 0x7FFF) == 0x4000) {
                            unit->partyRecord.hp = 0;
                            unit->status.flags |= 0x20;
                        }
                        func_0035B6E0(D_00415340, index,
                                      unit->partyRecord.unitId);
                        break;
                    }
                }
            }
        }
    }
}

void btlSyncModelFlagFromEventThresholds(void) {
    if (evtCheckValueThreshold(0x53, 1) || evtCheckValueThreshold(0x54, 1)) {
        mdlFlagSet(0xa20);
    } else {
        mdlFlagClear(0xa20);
    }
}

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415340);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AF0B0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AF4A0);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001AFF38);

typedef struct BtlHitResult {
    s32 amount;
    u8 pad04[0x28];
} BtlHitResult;

typedef struct BtlTargetResult {
    u8 hitCount;
    u8 pad01[7];
    u32 kind;
    u8 pad0C[4];
    u8 skipped;
    u8 pad11[3];
    u8 blocked;
    u8 pad15[7];
    BtlHitResult hits[32];
} BtlTargetResult;

s32 btlSumOtherTargetHitAmounts(u8 *action) {
    BtlTargetResult *result = *(BtlTargetResult **)(action + 0x88);
    u32 count = btlGetIndexListCount(*(BtlIndexList **)(action + 0x60));
    u32 i;
    u32 j;
    s32 total = 0;

    for (i = 0; i < count; i++, result++) {
        if (result->skipped != 0 || result->blocked != 0) {
            continue;
        }
        switch (result->kind) {
        case 2:
        case 4:
        case 0x10000:
        case 0x20000:
        case 0x40000:
            break;
        default:
            if (*(BtlUnit **)(action + 0x18) !=
                btlGetIndexListEntry(*(BtlIndexList **)(action + 0x60), i)) {
                for (j = 0; j < result->hitCount; j++) {
                    total += result->hits[j].amount;
                }
            }
            break;
        }
    }
    return total;
}

s32 btlTestActorStatusPredicate(BtlUnit *unit) {
    s32 (*hook)(BtlUnit *) = *(s32 (**)(BtlUnit *))(btlGetRuntime() + 0x698);
    if (hook != 0) {
        if (hook(unit) != 0) {
            return 1;
        }
    }
    return (unit->partyRecord.status & 0x2806) != 0;
}

s32 btlComputeStatusPenaltyFifth(BtlUnit *unit) {
    u16 status;
    s32 amount;

    status = unit->partyRecord.status & 0x7fff;
    amount = 0;
    if ((status == 0x80) || (status == 0x400)) {
        amount = (s32)-(u32)unit->partyRecord.maxHp / 5;
    }
    return amount;
}

extern char D_00415440[]; /* "btl:fear ratio[%d]\n" */

extern s32 btlRollAiBucket();

s32 btlRollFearChance(s32 unused, BtlUnit *unit, s32 flagsA, s32 flagsB) {
    s32 threshold;
    if (((BtlState *)btlGetRuntime())->unk220 & 0x80) {
        return 0;
    }
    if (unit->status.stateFlags & 8) {
        return 0;
    }
    if (!(flagsA & 1)) {
        return 0;
    }
    if (unit->partyRecord.status & 1) {
        return 0;
    }
    threshold = 0x1E;
    if (!(flagsB & 2)) {
        threshold = !(flagsB & 4) ? 0 : 0x28;
    }
    btlBossDebugPrintf(D_00415440, threshold);
    return btlRollAiBucket() < threshold;
}

extern s32 btlGetActionRecordLookupValue(s32);

extern s32 fldCountSceneSlots(void);

/* Combine both contributions; groups of three or more suppress the 30% case. */
INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415440);

s32 btlRollAllFearChance(s32 unused, BtlUnit *unit, u32 flags,
                  s32 unusedFlags, u8 useSelectedAction) {
    s32 actionThreshold;
    s32 statusThreshold;
    s32 threshold;
    u32 category;

    if (((BtlState *)btlGetRuntime())->unk220 & 0x80) {
        return 0;
    }
    actionThreshold = 0;
    statusThreshold = 0;
    if (useSelectedAction != 0) {
        if (unit->selectedEntryIndex == -1) {
            return 0;
        }
        category = btlGetActionRecordLookupValue(unit->selectedEntryIndex);
        switch (category) {
            case 0x20000:
            case 0x40000:
                actionThreshold = 40;
                break;
            case 0x10000:
                actionThreshold = 30;
                if (fldCountSceneSlots() >= 3) {
                    actionThreshold = 0;
                }
                break;
        }
    }
    if (flags & 0x60000) {
        statusThreshold = 40;
    } else if (flags & 0x10000) {
        statusThreshold = 30;
        if (fldCountSceneSlots() >= 3) {
            statusThreshold = 0;
        }
    }
    threshold = statusThreshold < actionThreshold ? actionThreshold : statusThreshold;
    btlBossDebugPrintf("btl:fear all ratio[%d]\n", threshold);
    return btlRollAiBucket() < threshold;
}

f32 func_001B0B20(void) {
    return 1.5f;
}

typedef struct EventRosterStat {
    s16 base;
    u8 alternateA;
    u8 alternateB;
    f32 multiplier;
    u8 pad08[6];
    u8 rangeMin;
    u8 rangeMax;
    u8 pad10[4];
} EventRosterStat;

extern s32 datRosterDetails;

u8 func_001B0B30(BtlUnit *unit, s32 command) {
    BattleController *controller = (BattleController *)btlGetRuntime();
    s32 result;
    s32 minimum;
    s32 maximum;

    if (controller->commandRangeOverride != NULL) {
        result = controller->commandRangeOverride(unit, command);
        if (result > 0) {
            return result;
        }
    }
    if (command == 0) {
        return 1;
    }
    if (datCommandSelectors[command].kind == 5 && (unit->status.flags & 0x200)) {
        minimum = ((EventRosterStat *)datRosterDetails)[unit->partyRecord.unitId].rangeMin;
        maximum = ((EventRosterStat *)datRosterDetails)[unit->partyRecord.unitId].rangeMax;
    } else {
        minimum = datCommandRecords[command].rangeMin;
        maximum = datCommandRecords[command].rangeMax;
    }
    if (minimum < maximum) {
        result = minimum + effMiscRandMod(0, maximum - minimum + 1);
    } else {
        result = minimum;
    }
    return result;
}

u8 btlGetActorDisplayByteWithDefault(BtlUnit *object, s32 index) {
    if (index == 0) {
        if ((object->status.flags & 0x400) != 0) {
            return datEnemyRecords[object->partyRecord.unitId].unk48;
        }
        return 12;
    }
    return 12;
}

extern s32 datActionAnimationRecords;

extern s32 D_003B4EF0[];

/* Delay before an action: 9 for a 0x200-group unit in mode 6 with stat bit 0x10 clear on a
 * kind-5 or empty command, 1 for no command, else the animation's delay-table row. */
s32 func_001B0C68(BtlUnit *unit, s32 actionId) {
    s32 *delayTable;
    s32 *delay;

    if (actionId == 0 || datCommandSelectors[actionId].kind == 5) {
        if ((btlUnitStatusPair(unit) & 0x1200) == 0x200 &&
            (*(u64 *)&unit->partyRecord.flags & 0xFFFF00000010ULL) == 0x600000000ULL) {
            return 9;
        }
    }
    if (actionId == 0) {
        return 1;
    }
    delayTable = D_003B4EF0;
    delayTable++;
    delay = delayTable + ((BtlActionAnimationRecord *)datActionAnimationRecords)[actionId].delayIndex * 2;
    btlBossDebugPrintf("btl:delay=%d\n", *delay);
    return *delay;
}

s32 btlMapActionCode(s32 unused, u32 id) {
    switch (id) {
    case 0x31:
    case 0x32:
    case 0x33:
    case 0x185:
        return 0x35;
    case 0x186:
        return 0x1E;
    case 0x193:
        return 0x25;
    default:
        return datCommandSelectors[id].kind == 2 ? 0x2D : 0;
    }
}

extern void btlUnitGetMuzzlePosVU(BtlUnit *);

typedef struct BtlAnimationModeRow {
    u8 mode;
    u8 pad01[3];
    s32 delay;
} BtlAnimationModeRow;

/* Sort same-side targets for the selected animation, or perform thirteen swaps. */
void func_001B0DB0(BtlUnit *unit, BtlIndexList *targets, s32 actionId) {
    BtlUnit *pair[2];
    f32 muzzle[2][4];
    u32 randomIndices[2];
    u32 count;
    u32 mode;
    u32 i;
    u32 last;
    s32 sorted;

    count = btlGetIndexListCount(targets);
    if (count < 2) {
        return;
    }
    if ((unit->status.flags & 0x200) != 0 &&
        datCommandSelectors[actionId].kind == 5) {
        mode = unit->partyRecord.unitId == 6 ? 2 : 0;
    } else {
        u32 delayIndex = ((BtlActionAnimationRecord *)datActionAnimationRecords)[actionId].delayIndex;
        mode = ((BtlAnimationModeRow *)D_003B4EF0)[delayIndex].mode;
    }
    switch (mode) {
    case 0:
        btlBossDebugPrintf("btl:anim=non\n");
        return;
    case 1:
        btlBossDebugPrintf("btl:anim=left\n");
        break;
    case 2:
        btlBossDebugPrintf("btl:anim=right\n");
        break;
    case 3:
        btlBossDebugPrintf("btl:anim=rand\n");
        break;
    }
    if (mode != 3) {
        last = count - 1;
        do {
            sorted = 1;
            for (i = 0; i < last; i++) {
                pair[0] = btlGetIndexListEntry(targets, i);
                pair[1] = btlGetIndexListEntry(targets, i + 1);
                btlUnitGetMuzzlePosVU(pair[0]);
                VU0_STORE_VF(vf10, muzzle[0]);
                btlUnitGetMuzzlePosVU(pair[1]);
                VU0_STORE_VF(vf10, muzzle[1]);
                if ((pair[0]->status.flags & 0x400) && (pair[1]->status.flags & 0x400)) {
                    if (mode == 1) {
                        if (muzzle[0][0] > muzzle[1][0]) {
                            btlSwapIndexListEntries(targets, i, i + 1);
                            sorted = 0;
                        }
                    } else if (muzzle[0][0] < muzzle[1][0]) {
                        btlSwapIndexListEntries(targets, i, i + 1);
                        sorted = 0;
                    }
                } else if ((pair[0]->status.flags & 0x200) && (pair[1]->status.flags & 0x200)) {
                    if (mode == 1) {
                        if (muzzle[0][0] < muzzle[1][0]) {
                            btlSwapIndexListEntries(targets, i, i + 1);
                            sorted = 0;
                        }
                    } else if (muzzle[0][0] > muzzle[1][0]) {
                        btlSwapIndexListEntries(targets, i, i + 1);
                        sorted = 0;
                    }
                }
            }
        } while (sorted == 0);
    } else {
        for (i = 0; i < 13; i++) {
            randomIndices[0] = effMiscRandMod(0, count);
            randomIndices[1] = effMiscRandMod(0, count);
            if (randomIndices[0] != randomIndices[1]) {
                btlSwapIndexListEntries(targets, randomIndices[0], randomIndices[1]);
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B1090);

void btlDistributeRandomTargetHits(BtlUnit *unit, BtlIndexList *targets,
                   BtlTargetResult *results, s32 command) {
    u8 selected[13];
    BtlIndexList *copy;
    void *previous;
    void *entry;
    u32 targetCount;
    u32 maximumHits;
    u32 hitCount;
    u32 i;
    u32 index;
    u32 resultIndex;
    s32 allowConsecutiveHits;

    targetCount = btlGetIndexListCount(targets);
    allowConsecutiveHits = targetCount < 2;
    maximumHits = targetCount + effMiscRandMod(0, 2);
    if (allowConsecutiveHits) {
        results[0].hitCount = maximumHits;
        return;
    }

    previous = NULL;
    copy = btlAllocateIndexList(ARRAY_COUNT(selected));
    i = 0;
    btlCopyIndexList(copy, targets);
    btlClearIndexList(targets);
    hitCount = func_001B0B30(unit, command);
    memset(selected, 0, sizeof(selected));
    while (i < hitCount) {
        index = effMiscRandMod(0, targetCount);
        entry = btlGetIndexListEntry(copy, index);
        if (!allowConsecutiveHits && entry == previous) {
            index = (index + effMiscRandMod(0, targetCount - 1) + 1) % targetCount;
            entry = btlGetIndexListEntry(copy, index);
        }
        previous = entry;
        if (!selected[index]) {
            btlAppendIndexListEntry(targets, entry);
            resultIndex = btlFindListIndex(targets, entry);
            selected[index] = 1;
            results[resultIndex].hitCount = 1;
        } else {
            resultIndex = btlFindListIndex(targets, entry);
            if (results[resultIndex].hitCount < maximumHits) {
                results[resultIndex].hitCount++;
            }
        }
        i++;
    }
    btlFreeIndexList(copy);
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B1350);

s32 btlQueryUnitChannelFlags(BtlUnit *first, BtlUnit *second, s32 other, s32 variant, s32 mode) {
    s32 flags;
    if (mode != 1) {
        return 0;
    }
    flags = sdfQueryChannelBits(other, (s32)&first->partyRecord, (s32)&second->partyRecord);
    if ((second->partyRecord.status & 8) != 0 && variant == 2) {
        flags |= 8;
    }
    return flags;
}

u32 btlGetAdjustedSlotAffinity(BtlUnit *unit, s32 unused, s32 index) {
    s32 selection;
    u32 value;
    u32 ratio;

    if (index == -1) {
        return 0;
    }
    selection = btlEncodeActorIndexAsSelectionMask(index);
    if ((selection & 1) != 0) {
        return 0;
    }
    if ((selection & 0xE0000) != 0) {
        return 100;
    }
    value = func_001ABF50(unit, index);
    ratio = func_001AE678(unit, index);
    if (ratio != 100) {
        ratio = (u16)value * ratio / 100;
        value = (value & 0xFFFF0000) | ratio;
        value &= 0x7FFFFFFF;
    }
    if (btlHasEnabledSpecialAbilityForSlot(unit, index) != 0) {
        value |= 0x20000;
    }
    if (btlHasSpecialAbility274(unit, index) != 0) {
        value |= 0x40000;
    }
    if (btlHasMappedSpecialAbilityForSlot(unit, index) != 0) {
        value |= 0x10000;
    }
    btlBossDebugPrintf("btl:aisyo=%d%%[%X][ratio=%d]\n",
                      (u16)value, value & 0xFFFF0000, ratio);
    return value;
}

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B17E8);

void func_001B1F78(void) {
}

extern f32 func_001B20C8(BtlUnit *, BtlUnit *, s32);

/* Scale the enemy's normal EP reward; flag 0x2000 multiplies it by 100. */
s32 btlCalculateEnemyExperienceReward(BtlUnit *acquirer, BtlUnit *enemy) {
    s32 result = 0;
    DatEnemyRecord *entry;
    f32 ratio;
    u32 ep;

    if (!(enemy->status.flags & 0x400)) {
        return result;
    }
    if (acquirer != 0 && !(acquirer->status.flags & 0x200)) {
        return result;
    }
    entry = &datEnemyRecords[enemy->partyRecord.unitId];
    ratio = func_001B20C8(acquirer, enemy, 1);
    ep = (u32)((f32)entry->experience * ratio);
    if (entry->flags & 0x2000) {
        ep *= 100;
    }
    if (acquirer != 0) {
        btlBossDebugPrintf("btl:ep=%d[%d,%.3f]\n", ep, entry->experience, ratio);
    } else {
        btlBossDebugPrintf("btl:ep=%d[%d,%.3f](acquisition)\n", ep, entry->experience, ratio);
    }
    return ep;
}

f32 func_001B20C8(BtlUnit *acquirer, BtlUnit *enemy, s32 rewardKind) {
    s32 level;
    s32 difference;
    f32 factor;

    if (datBattleSceneRecords[((BtlState *)btlGetRuntime())->battleMode].flags & 0x400) {
        btlBossDebugPrintf("btl:ep hosei off\n");
        return 1.0f;
    }
    if (acquirer == NULL) {
        level = func_001B39E8(4);
    } else {
        level = acquirer->partyRecord.level;
    }
    if (level > 60) {
        level = 60;
    }
    difference = level - enemy->partyRecord.level;
    if (difference > 15) {
        difference = 15;
    } else if (difference < -15) {
        difference = -15;
    }
    factor = datBattleParameters->rewardLevelScale[(15 - difference) * 2 + rewardKind];
    btlBossDebugPrintf("btl:ep[lv=%.3f(%d,%d)]\n",
                       factor, difference, rewardKind);
    return factor;
}

/* Read the money reward with the same eligibility and 100-fold table flag. */
s32 btlGetEnemyMoney(BtlUnit *acquirer, BtlUnit *enemy) {
    s32 result = 0;
    s32 money;
    DatEnemyRecord *entry;
    if (!(enemy->status.flags & 0x400)) {
        return result;
    }
    if (acquirer != 0 && !(acquirer->status.flags & 0x200)) {
        return result;
    }
    entry = &datEnemyRecords[enemy->partyRecord.unitId];
    money = entry->money;
    if (entry->flags & 0x2000) {
        money *= 100;
    }
    if (acquirer != 0) {
        btlBossDebugPrintf("btl:money=%d\n", money, acquirer);
    } else {
        btlBossDebugPrintf("btl:money=%d(acquisition)\n", money);
    }
    return money;
}

/* Hunt EP uses its own table quantity and the ratio calculator's mode 0. */
s32 btlCalculateHuntEpReward(BtlUnit *acquirer, BtlUnit *enemy) {
    DatEnemyRecord *entry = &datEnemyRecords[enemy->partyRecord.unitId];
    f32 ratio = func_001B20C8(acquirer, enemy, 0);
    u32 ep = (u32)((f32)entry->huntExperience * ratio);
    if (entry->flags & 0x2000) {
        ep *= 100;
    }
    btlBossDebugPrintf("btl:ep=%d[%d,%.3f](hunt)\n", ep, entry->huntExperience, ratio);
    return ep;
}

u32 func_001B2380(void) {
    return 0;
}

s32 btlCalculateAbilityRecoveryAmount(BtlUnit *unit) {
    s32 recovery = 0;
    if (btlCheckSpecialAbility(&unit->partyRecord, 0x26E) != 0) {
        recovery = (s32)(unit->partyRecord.maxMp * datAbilityParameters[0x26E - BTL_ABILITY_PARAMETER_FIRST_SKILL].value);
    } else if (btlCheckSpecialAbility(&unit->partyRecord, 0x249) != 0) {
        recovery = (s32)(unit->partyRecord.maxMp * datAbilityParameters[0x249 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value);
    }
    btlBossDebugPrintf(D_00415638, recovery);
    return recovery;
}

extern s32 btlHasEnemyRecordDefeatExemptionFlag(BtlUnit *);

s32 btlIsUnitDefeatTriggeredByValueDelta(BtlUnit *unit, s32 delta) {
    if (btlGetEntryFlagsUnlessDisabled(&unit->partyRecord) & 4) {
        return 0;
    }
    if (btlHasEnemyRecordDefeatExemptionFlag(unit) != 0) {
        return 0;
    }
    if ((unit->partyRecord.status & 0x7FFF) == 0x4000) {
        return 1;
    }
    if (!(((BtlState *)btlGetRuntime())->battleFlags & 0x80)) {
        return 0;
    }
    return unit->partyRecord.hp + delta < 1;
}

s32 btlIsCurrentValueBelowQuarterThreshold(BtlUnit *object) {
    return object->partyRecord.hp * 100 / object->partyRecord.maxHp < 26;
}

s32 btlWouldUiValueFallBelowQuarter(BtlUnit *object, s32 delta) {
    s32 value = object->partyRecord.hp + delta;
    if (value <= 0) {
        return 1;
    }
    return value * 100 / object->partyRecord.maxHp < 26;
}

s32 btlBothSidesActive(BtlUnit *unit) {
    BtlUnit *actor;
    s32 a;
    s32 b;
    if (btlIsUnitDefeatTriggeredByValueDelta(unit, 0) != 0) {
        return 0;
    }
    if (unit->status.flags & 0x60) {
        return 0;
    }
    a = 0;
    b = 0;
    for (actor = ((BattleController *)btlGetRuntime())->actors; actor != 0; actor = actor->nextActor) {
        if (actor->status.flags & 1) {
            if (!(actor->status.flags & 0xE0)) {
                if (actor->status.flags & 0x200) {
                    a++;
                }
                if (actor->status.flags & 0x400) {
                    b++;
                }
            }
        }
    }
    if (a != 0 && b != 0) {
        return 1;
    }
    return 0;
}

s32 btlIsUiObjectIndexAllowed(BtlUnit *object) {
    if ((object->status.flags & 0x400) != 0) {
        if (object->partyRecord.unitId >= 0x100) {
            return 0;
        }
    }
    return 1;
}

s32 btlIsUnitStatusFlagClear(BtlUnit *object) {
    if ((object->partyRecord.status & 0x1000) != 0) {
        return 0;
    }
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_001A5BB8", D_00415638);

INCLUDE_ASM(const s32, "game/code_001A5BB8", func_001B2630);

s32 btlSelectedEntryHitsElement(BtlUnit *source, BtlUnit *unit, s32 selectorIndex) {
    u32 indexedSelectorValue;
    s32 selectionMask;
    u32 maskTableIndex;
    if (unit->selectedEntryIndex <= 0) {
        return 0;
    }
    btlGetRuntime();
    indexedSelectorValue = btlGetActorIndexedSignedValue(source, selectorIndex);
    selectionMask = btlEncodeActorIndexAsSelectionMask(indexedSelectorValue);
    maskTableIndex = datCommandRecords[unit->selectedEntryIndex].unk2E;
    if (maskTableIndex == 0) {
        return 0;
    }
    if (indexedSelectorValue >= 0x10 && (indexedSelectorValue < 0x12 || indexedSelectorValue == -1)) {
        return 0;
    }
    if (maskTableIndex >= 0x21) {
        return 0;
    }
    return (D_003B4F78[maskTableIndex * 3] & selectionMask) != 0;
}

s32 btlGetActionRecordLookupValue(s32 actionRecordIndex) {
    u16 lookupTableIndex;

    lookupTableIndex = datCommandRecords[actionRecordIndex].unk2E;
    return D_003B4F70[lookupTableIndex * 3];
}

s32 btlTestSelectedItemCategoryMask(BtlUnit *unit, s32 actorIndex) {
    s32 selectedEntryIndex = unit->selectedEntryIndex;
    u16 maskTableIndex;
    if (selectedEntryIndex == -1) {
        return 0;
    }
    maskTableIndex = datCommandRecords[selectedEntryIndex].unk2E;
    return (D_003B4F78[maskTableIndex * 3] & btlEncodeActorIndexAsSelectionMask(actorIndex)) != 0;
}

s32 fldGetSelectedUnitStat(BtlUnit *unit) {
    s32 index = unit->selectedEntryIndex;
    if (index == -1) {
        return 0;
    }
    return btlGetActionRecordLookupValue(index);
}

s32 btlGetSelectedUnitProperty(BtlUnit *unit) {
    s32 index = unit->selectedEntryIndex;
    u16 property;
    if (index == -1) {
        return 0;
    }
    property = datCommandRecords[index].unk2E;
    return D_003B4F74[property * 3];
}

s32 btlCompareSkippedAndActiveTargetCounts(BtlIndexList *targets, BtlTargetResult *results) {
    s32 i = 0;
    u32 sides = 0;
    BattleController *controller = (BattleController *)btlGetRuntime();
    u32 count = btlGetIndexListCount(targets);
    s32 skipped;
    BtlUnit *actor;

    for (; i < count; i++) {
        sides |= ((BtlUnit *)btlGetIndexListEntry(targets, i))->status.flags & 0x600;
    }
    skipped = 0;
    for (i = 0; i < count; i++, results++) {
        if (results->skipped != 0) {
            skipped++;
        }
    }
    count = 0;
    for (actor = controller->actors; actor != NULL; actor = actor->nextActor) {
        if (actor->status.flags & 1) {
            if (actor->status.flags & 0xE0) {
                continue;
            }
            if (actor->status.flags & sides) {
                count++;
            }
        }
    }
    return skipped == count;
}
