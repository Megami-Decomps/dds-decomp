#include "common.h"
#include "itf.h"
#include "pcp_vu0.h"
#include "scr.h"
#include "sdf_sif_command.h"
#include "sdf_projection.h"



extern ItfMesGlobals itfMesWork;



extern s32 func_00195ED8();

extern UiSprite *func_00199828(s32, u32);

extern void itfSetPanelLayoutAndNotify();

extern void itfPanelUpdateValuesAndNotify();

typedef struct SndPad {
    u8 pad00[0x21];
    s8 confirm;
    s8 edge22;
    s8 edge23;
    u8 pad24[2];
    s8 prev;
    s8 next;
    u8 pad28[9];
    s8 unk31;
    u8 unk32;
    s8 cancel;
    s8 coarseDown;
    s8 coarseUp;
    s8 unk36;
    s8 unk37;
    s8 fineDown;
    u8 pad39;
    s8 fineUp;
} SndPad;


typedef struct UiOwnerRef { u8 pad0[0xC]; ItfMesState *owner; } UiOwnerRef;

extern SndPad D_00324510;

extern SdfPoolNode kwlnDrawSurfaces[];

extern UiOwnerRef *D_00357D88[];

extern void itfAdvancePanelLayoutAndNotify(UiSprite *sprite, s32 a, s32 b, s32 c, s32 d, s32 e);

extern void itfBuildAndSubmitPanelPacket(UiSprite *sprite, SdfPoolNode *surface);

extern void itfMesOffsetNodeChain(FrFontGlyph *node, s32 dx, s32 dy);

extern void func_0019F770(UiSprite *sprite);

extern s32 func_0019EA88(ItfMesBlk40 *seq);

extern void itfMesSetRowItemFlag(FrFontGlyph *sequence, s32 index, s32 count, s32 selected);

extern void sndSetSequenceVolumePan(s32 sequence, s32 volume, s32 pan);

extern s8 D_00324530[];

extern u32 kwlnTaskGetUserValue(s64);

extern const char *D_00358308[];

extern u32 sndTestMessageResourceIndex;

extern SdfTex *sndTestMessageTexture;

extern SdfTex *itfLoadTextureFromAsset(const char *);


void frFontEnableNodeContextModes(FrFontGlyph *node) {
    for (; node != NULL; node = node->previous) {
        frFontEnableContextMode(node);
    }
}

/* Initialize panel placement and retain its screen-space Y coordinate. */
void itfMesInitializePanelPlacementSprite(ItfMesState *panel) {
    ItfMesEntryBlock *pos = &panel->entryBlock;
    ItfMesBlkA4 *place = &panel->blkA4;
    s32 spriteTop;
    if (place->sprite == 0) {
        place->sprite = func_00199828(6, (u32)itfMesWork.windowTexture);
        if (place->frame != 0) {
            place->sprite->unk20 = panel->blk14.glyphChain->x + func_00195ED8(0, panel->blk14.glyphChain);
        }
    }
    spriteTop = pos->y + place->bounds[1];
    itfSetPanelLayoutAndNotify(place->sprite, pos->x + place->bounds[0], spriteTop, pos->x + place->bounds[2], pos->y + place->bounds[3], panel->renderValue);
    place->sprite->screenY = spriteTop;
    itfPanelUpdateValuesAndNotify(place->sprite, place->unk1C, place->unk20, place->unk24, 0);
    panel->flags = (panel->flags & ~0x300) | 0x100;
}

void itfMesCreatePanelOriginFrameWhenVisible(ItfMesState *panel) {
    ItfMesBlk14 *origin = &panel->blk14;
    ItfMesBlkA4 *place = &panel->blkA4;
    if (origin->glyphChain != NULL && !(panel->flags & 0x10000)) {
        if (place->frame == NULL) {
            s32 width = origin->glyphChain->advance * 16;
            place->frame = func_00199828(7, (u32)itfMesWork.windowTexture);
            itfSetPanelLayoutAndNotify(place->frame, origin->x - 0x2D0, origin->y - 0x68, origin->x + width + 0x2D0, origin->y + 0xF0, panel->renderValue);
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
    cursor->glyphChain = 0;
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
    cursor->y = 0xC48;
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

void func_0019E048(ItfMesBlkA4 *block, s32 mode, s32 value);


void itfResetWindowResourceBlock(ItfMesBlkA4 *block) {
    block->frame = NULL;
    block->sprite = NULL;
    block->overlay = NULL;
    func_0019E048(block, 0, 0);
}

void itfClearDrawStateWords(ItfMesTextSlots *slots) {
    s32 remaining;
    u32 *cursor;

    cursor = &slots->addresses[31];
    remaining = 0x1f;
    do {
        remaining = remaining - 1;
        *cursor = 0;
        cursor = cursor + -1;
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

void itfReleaseUiResourceSlotHandles(ItfMesTextSlots *slots) {
    s32 remaining;
    s32 index;

    remaining = 0x1f;
    index = 0;
    do {
        if (slots->addresses[index] != 0) {
            sdfReleaseResourceAllocation(slots->handles[index]);
            slots->addresses[index] = 0;
        }
        remaining = remaining - 1;
        index++;
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

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019E048);

void itfMesUpdatePanelFades(ItfMesState *panel);
void btlUpdateFadeIndicator(ItfMesState *panel);

void itfUpdateBattleDisplayAndFadeIndicator(ItfMesState *object) {
    itfMesUpdatePanelFades(object);
    func_0019E320(object);
    func_0019E4F8(object);
    btlUpdateFadeIndicator(object);
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

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019E320);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019E4F8);

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
    if (D_00324510.prev & 2) {
        if (sel->selectedIndex != 0) {
            dir = -1;
        } else if (D_00324510.prev < 0) {
            dir = -1;
        }
    } else if (D_00324510.next & 2) {
        if (sel->selectedIndex != sel->rowCount - 1 || D_00324510.next < 0) {
            dir = 1;
        }
    }
    if (dir != 0) {
        sndStepSequenceIndex(sel, dir);
        itfResetBattleFadeState(&panel->fade, 1);
    }
    if (D_00324510.confirm < 0) {
        sndSetSequenceVolumePan(8, 0x7F, 0x3F);
        return 1;
    }
    if (sel->optionCount > 0 && (index = func_0019EA88(sel)) >= 0) {
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

void sndStepSequenceIndex(ItfMesBlk40 *obj, s32 dir) {
    s32 cur = obj->selectedIndex;
    u16 maxv;
    itfMesSetRowItemFlag(obj->glyphChain, cur, obj->rowCount, 0);
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
    itfMesSetRowItemFlag(obj->glyphChain, cur, (s16)maxv, 1);
    obj->selectedIndex = cur;
    obj->savedIndex = cur;
    sndSetSequenceVolumePan(1, 0x7F, 0x3F);
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019EA88);


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
extern void func_0019EE58(ItfMesState *);
extern void func_0019F0F8(ItfMesState *);

/* Draw the panel's glyph layers, advance its selection state and fade. */
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
            func_0019F0F8(panel);
        } else {
            func_0019EE58(panel);
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
    if (D_00357D88[0] != 0 && D_00357D88[0]->owner == panel && place != 0) {
        func_0019F770(place->sprite);
    }
}

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019EE58);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019F0F8);

extern u8 D_00358008[];
extern u8 D_00357FF8[];
extern s32 D_00358018[];
extern s32 sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(SdfListHead *);
extern void itfQueueTextureBoundQuadPacket(void *, void *, void *, s32, SdfTex *, s32, SdfListHead *);
extern void itfSendTablePacket(SdfListHead *, s32, s32);

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
    D_00358018[3] = 0xFF;
    itfQueueTextureBoundQuadPacket(bounds, D_00358008, D_00358018, object->renderValue,
                                  itfMesWork.windowTexture, 0, (SdfListHead *)packet);
    D_00358018[3] = fade->alpha;
    itfQueueTextureBoundQuadPacket(bounds, D_00357FF8, D_00358018, object->renderValue,
                                  itfMesWork.windowTexture, 0, (SdfListHead *)packet);
    if (fade->timer > 0) {
        expansion = 0x80 - fade->timer;
        bounds[0] -= expansion * 2;
        bounds[1] -= expansion;
        bounds[2] += expansion * 2;
        bounds[3] += expansion;
        D_00358018[3] = fade->timer;
        itfSendTablePacket((SdfListHead *)packet, 1, 0);
        itfQueueTextureBoundQuadPacket(bounds, D_00357FF8, D_00358018, object->renderValue,
                                      itfMesWork.windowTexture, 0, (SdfListHead *)packet);
        itfSendTablePacket((SdfListHead *)packet, 0, 0);
    }
    surface = &kwlnDrawSurfaces[object->unk10];
    surface->append((SdfListHead *)surface, (SdfListHead *)packet);
}


s32 sndVisitQueuedResources(void) {
    ItfMesPoolNode *node = itfMesWork.pool.activeHead;
    while (node != 0) {
        itfUpdateBattleDisplayAndFadeIndicator((ItfMesState *)node->stateAddress);
        node = node->next;
    }
    return 0;
}

extern void itfMesDestroyWindow(s32 window);

extern void sdfTexReleaseReferenceViaHandler(SdfTex *texture);

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019F6A0);

void sndFlushMessageQueue(void) {
    ItfMesPoolNode *node = itfMesWork.pool.activeHead;
    s32 window;
    while (node != 0) {
        window = node->index;
        node = node->next;
        itfMesDestroyWindow(window);
    }
    sdfTexReleaseReferenceViaHandler(itfMesWork.windowTexture);
    itfMesWork.windowTexture = NULL;
}


extern SdfPoolNode D_00325708;
extern s32 D_00358028[];
extern u8 D_003BB230[5];
extern u8 D_003BB238[5];
extern s32 sdfAllocPacketAligned(s32 size);
extern void sdfInitPacketList(SdfListHead *list);
extern void itfEmitQuadListA(void *, void *, u8 *, u8 *, s32, u32, SdfListHead *);

void func_0019F770(UiSprite *sprite) {
    s32 vertices[4][2] = {
        {sprite->left, sprite->top},
        {sprite->right, sprite->top},
        {sprite->right, sprite->bottom},
        {sprite->left, sprite->bottom}
    };
    s32 packet;

    packet = sdfAllocPacketAligned(0x20);
    sdfInitPacketList((SdfListHead *)packet);
    itfEmitQuadListA(vertices, D_00358028, D_003BB230, D_003BB238, 5, 0xFFFFFF, (SdfListHead *)packet);
    D_00325708.append((SdfListHead *)&D_00325708, (SdfListHead *)packet);
}

void itfAdjustPanelBoundsWithPad(ItfMesBlkA4 *object, s32 mode) {
    UiSprite *sprite = object->sprite;
    s32 *bounds;
    s32 dx;
    s32 dy;

    if (sprite != NULL) {
        bounds = object->bounds;
        if (D_00324510.coarseDown & 2) {
            dx = -16;
        } else {
            dx = ((u8)D_00324510.coarseUp << 3) & 0x10;
        }
        if (D_00324510.unk36 & 2) {
            dy = -8;
        } else {
            dy = ((u8)D_00324510.unk37 << 2) & 8;
        }
        if (D_00324510.unk31 != 0) {
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

    if (D_00324510.coarseDown & 2) {
        step = -1;
    } else {
        step = (D_00324510.coarseUp & 2) > 0;
    }
    if (D_00324510.unk36 & 2) {
        step = -1;
    } else if (D_00324510.unk37 & 2) {
        step = 1;
    }
    if (D_00324510.unk31 != 0) {
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

ItfMesPoolNode *func_0019FA70(ItfMesPoolNode *node) {
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

extern u32 kwlnTaskCreate(u32, u32, u32, u32, s32 (*)(s64), void (*)(s64), u32);

extern u8 D_00358038[];

extern char D_003A14F0[]; /* "TestMsgMngC" */

extern char D_003A1500[]; /* "TestMsgMngD" */

extern s32 D_003BA8EC;

extern s32 scrCreateTaskForProcessId(s32, u8 *, s32);

extern void itfMesSetWindowCallbackAddress(s32, void (*)(void));

extern void sndCycleTestMessageResource(void);

extern s32 itfUpdateTestMessageResourceInput(void);

extern void itfPrintTestMessageCallback(void);

extern s32 sndUpdateTestMsgTask(void);


void sndCreateTestMsgTasks(void) {
    D_003BA8EC = 0x80FFFFFF;
    sndCycleTestMessageResource();
    itfMesSetWindowCallbackAddress(((ScrData *)kwlnTaskGetUserValue(scrCreateTaskForProcessId(0x3E8, D_00358038, 0)))->resourceIndex, itfPrintTestMessageCallback);
    kwlnTaskCreate((u32)D_003A14F0, 0x3EF, 0, 0, (s32 (*)(s64))itfUpdateTestMessageResourceInput, 0, 0);
    kwlnTaskCreate((u32)D_003A1500, 0x2AFE, 0, 0, (s32 (*)(s64))sndUpdateTestMsgTask, 0, 0);
}

void sndCycleTestMessageResource(void) {
    if (sndTestMessageTexture != 0) {
        sdfTexReleaseReferenceViaHandler(sndTestMessageTexture);
        sndTestMessageTexture = 0;
    }
    sndTestMessageResourceIndex = (sndTestMessageResourceIndex + 1) & 3;
    if (sndTestMessageResourceIndex != 3) {
        sndTestMessageTexture = itfLoadTextureFromAsset(D_00358308[sndTestMessageResourceIndex]);
    }
}

s32 itfUpdateTestMessageResourceInput(void) {
    if (D_00324530[0] < 0) {
        sndCycleTestMessageResource();
    }
    return 0;
}


extern SdfPoolNode D_003255A8;

extern u8 D_00358318[];

extern u8 D_00358328[];

extern u8 D_00358338[];


s32 sndUpdateTestMsgTask(void) {
    s32 mem;
    if ((sndTestMessageTexture != 0) && (sndTestMessageResourceIndex != 3)) {
        mem = sdfAllocPacketAligned(0x20);
        sdfInitPacketList((SdfListHead *)mem);
        itfSendTablePacket((SdfListHead *)mem, 0, 0);
        itfQueueTextureBoundQuadPacket(D_00358318, D_00358328, D_00358338, 0xFFF, sndTestMessageTexture, 0, (SdfListHead *)mem);
        D_003255A8.append((SdfListHead *)&D_003255A8, (SdfListHead *)mem);
        return 0;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A14F0);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A1500);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A1510);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A1528);

INCLUDE_RODATA(const s32, "game/code_0019DB88", D_003A1540);

void itfPrintTestMessageCallback(void) {
    func_003003F0("********* AAAA ********\n");
}

typedef struct ItfFovPanelWork {
    u8 pad00[8];
    f32 degrees;
    u8 pad0C[4];
} ItfFovPanelWork;

extern ItfFovPanelWork D_003D73C0;
extern s32 sdfCreateResetPacketList(void);
extern s32 func_0011D3E8(s32, s32, s32, s32, s32, u32, u32);
extern void sdfAppendPacket(SdfListHead *, u32);
extern s32 itfStepFloatWithPad(f32 *, f32, f32, f32, f32);

s32 func_0019FCC8(void) {
    SifCommand packet;
    s32 list;
    f32 radiansToDegrees = 57.2957795f;

    list = sdfCreateResetPacketList();
    sdfAppendPacket((SdfListHead *)list,
                    func_0011D3E8(0x8500, 0x79C0, 0xFEFFFF,
                                  0xA80, 0x120, 0x60000000, 0x40806020));
    sdfPktInit(&packet, 0x85C0, 0x7A20, 0xFF0000, 0);
    sdfAppendPacket((SdfListHead *)list,
                    (u32)sdfFormatSifPacket(&packet, "FOVY: %6.2f",
                        sdfSceneProjectionParameters.camera.fov * radiansToDegrees));
    D_00325708.append((SdfListHead *)&D_00325708, (SdfListHead *)list);
    if (D_00324510.cancel < 0) {
        return -1;
    }
    D_003D73C0.degrees = sdfSceneProjectionParameters.camera.fov * radiansToDegrees;
    if (itfStepFloatWithPad(&D_003D73C0.degrees, 1.0f, 89.0f, 0.1f, 1.0f) != 0) {
        sdfSceneProjectionParameters.camera.fov = D_003D73C0.degrees * 0.0174532925f;
    }
    return 0;
}

s32 itfStepFloatWithPad(f32 *value, f32 minimum, f32 maximum, f32 coarseStep, f32 fineStep) {
    f32 current = *value;
    s32 changed = 0;

    if (D_00324510.coarseUp < 0) {
        if (current < maximum) {
            current += coarseStep;
        } else {
            current = minimum;
        }
        changed = 1;
    } else if (D_00324510.coarseUp & 2) {
        if (current < maximum) {
            current += coarseStep;
            changed = 1;
        }
    } else if (D_00324510.coarseDown < 0) {
        if (minimum < current) {
            current -= coarseStep;
        } else {
            current = maximum;
        }
        changed = 1;
    } else if (D_00324510.coarseDown & 2) {
        if (minimum < current) {
            current -= coarseStep;
            changed = 1;
        }
    } else if (fineStep != 0.0f) {
        if (D_00324510.fineUp < 0) {
            if (current < maximum) {
                current += fineStep;
                if (maximum < current) {
                    current = maximum;
                }
            } else {
                current = minimum;
            }
            changed = 1;
        } else if (D_00324510.fineUp & 2) {
            if (current < maximum) {
                current += fineStep;
                if (maximum < current) {
                    current = maximum;
                }
                changed = 1;
            }
        } else if (D_00324510.fineDown < 0) {
            if (minimum < current) {
                current -= fineStep;
                if (current < minimum) {
                    current = minimum;
                }
            } else {
                current = maximum;
            }
            changed = 1;
        } else if (D_00324510.fineDown & 2) {
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

INCLUDE_ASM(const s32, "game/code_0019DB88", func_0019FF60);

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

