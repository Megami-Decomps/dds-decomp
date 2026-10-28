#include "mnu_mantra_sprite_draw.h"
#include "sdk_ps2.h"
#include "mnu_variant_sprite_placement.h"
#include "mnu.h"
#include "snd_sequence_volume_pan.h"
#include "sdf_trig.h"
#include "eff_random.h"
#include "sdf_task_work.h"
#include "sdf_resource.h"
#include "dat_state.h"
#include "mnu_scene.h"
#include "mnu_profile_progress.h"
#include "mnu_mantra_grid.h"
#include "mnu_sprite_resource.h"
#include "mnu_sprite_placement.h"
#include "sdf_grid.h"
#include "mnu_scene_work.h"
#include "sdf_chip.h"



extern void mnuDrawPlacedSpriteFromTable(s32 x, s32 y, s32 z, s32 alpha, s32 placementIndex, s32 context);


extern MnuVariantSpritePlacement mnuMantraVariantSpritePlacements[];
extern TaskWork *mnuSceneResourceContext;
extern u32 mnuGetMantraDisplayFlags(MnuMantraGridEntry *, MnuProfileProgress *);
extern void uiDrawUniformColorRect(u32, u32, u32, u32, u32, u32, u32);

extern void mnuDrawScaledVariantSprite(s32, s32, s32, s32, s32, s32, f32, f32, s32);

extern char D_003BC458[];
extern char D_003BC488[];
extern char D_003BC490[];
extern char D_003BC498[];
extern MnuProfileProgress *mnuGetSelectedNodeValue(void);
extern u32 mnuGetMantraPrerequisiteFlags(s32, MnuMantraGridEntry *, MnuMantraNodeState *, MnuProfileProgress *);
extern s32 frFontMeasureAndQueueGlyph(s32, s32, s32, u32, const u8 *, s32);

/* Retail clears four prerequisite flag words at +0x84, then stores results
 * at +0xE4 without reading them; preserve this original write-only work. */
void mnuDrawMantraEntryStatus(s32 x, s32 y, s32 depth, SdfGrid *grid,
                              SdfGridCell *entry, s32 context) {
    u32 prerequisiteFlags[4];
    MnuMantraGridEntry *scene;
    MnuProfileProgress *selection;
    MnuMantraNodeState *states;
    MantraPrerequisiteRecord *record;
    MenuSceneWork *display;
    s8 i;
    s32 alpha;
    u32 flags;
    u32 color;

    scene = (MnuMantraGridEntry *)(u32)entry->value;
    if (scene == NULL) {
        return;
    }
    selection = mnuGetSelectedNodeValue();
    states = grid->userData;
    display = (MenuSceneWork *)sdfGetTaskValueByKey(mnuSceneResourceContext, 1);
    record = &mnuMantraPrerequisiteRecords[scene->sceneId];
    alpha = display->displayAlpha;
    memset(prerequisiteFlags, 0, sizeof(prerequisiteFlags));
    if (record->kind == 0) {
        for (i = 0; i < 4 && record->ids[i] != 0; i++) {
            if (states[record->ids[i]].state != MNU_MANTRA_GRID_ENTRY_ABSENT) {
                prerequisiteFlags[i] = mnuGetMantraPrerequisiteFlags(i, scene, states, selection);
            }
        }
    }
    mnuDrawScaledVariantSprite(x, y, depth, alpha, scene->sceneId, 0x20,
                               1.0f, 1.0f, context);
    flags = mnuGetMantraDisplayFlags(scene, selection);
    if (flags & MNU_MANTRA_DISPLAY_FLAG_PROFILE_MATCH) {
        color = (s32)((f32)((alpha * 5) << 4) * 0.0078125f) | 0x60501000;
        uiDrawUniformColorRect((x - 4) << 4, (y - 4) << 3, 0, 0x1C0, 0xE0,
                               color, context);
    }
    if (flags & MNU_MANTRA_DISPLAY_FLAG_AT_CAP) {
        color = (s32)((f32)((alpha * 15) << 4) * 0.0078125f) | 0x80802000;
        frFontMeasureAndQueueGlyph(x + 4, y, depth, color,
                                   (const u8 *)D_003BC458, context);
        frFontMeasureAndQueueGlyph(x, y, depth, color,
                                   (const u8 *)D_003BC488, context);
    } else if (flags & MNU_MANTRA_DISPLAY_FLAG_ENTRY_STATE_1) {
        frFontMeasureAndQueueGlyph(x, y, depth,
            (s32)((f32)((alpha * 15) << 4) * 0.0078125f) | 0x10808000,
            (const u8 *)D_003BC488, context);
    } else if (flags & MNU_MANTRA_DISPLAY_FLAG_ENTRY_STATE_2) {
        frFontMeasureAndQueueGlyph(x + 4, y, depth,
            (s32)((f32)((alpha * 15) << 4) * 0.0078125f) | 0x40404000,
            (const u8 *)D_003BC488, context);
    } else if (flags & MNU_MANTRA_DISPLAY_FLAG_REQUIREMENT_PAIR_SET_FALLBACK) {
        frFontMeasureAndQueueGlyph(x, y, depth,
            (s32)((f32)(alpha << 7) * 0.0078125f) | 0x40404000,
            (const u8 *)D_003BC490, context);
    } else {
        frFontMeasureAndQueueGlyph(x, y, depth,
            (s32)((f32)(alpha << 7) * 0.0078125f) | 0x40404000,
            (const u8 *)D_003BC498, context);
    }
}

void mnuDrawMantraUpNavigationPulse(s32 frame, s32 size, s32 context) {
    f32 x = frame;
    f32 scale = size;
    f32 t;

    t = x > 5.0f ? (x - 5.0f) / 40.0f : 0.0f;
    t = sdfSin(t * 3.14159265f) * 0.2f + 0.1f;
    mnuDrawPlacedSpriteFromTable(0, -10, 0, (s32)(scale * t), 0, context);
    t = x < 35.0f ? x / 35.0f : 0.0f;
    t = sdfSin(t * 3.14159265f) * 0.5f + 0.5f;
    mnuDrawPlacedSpriteFromTable(0, 0, 0, (s32)(scale * t), 0, context);
}

void mnuDrawMantraRightNavigationPulse(s32 frame, s32 size, s32 context) {
    f32 x = frame;
    f32 scale = size;
    f32 t;

    t = x > 5.0f ? (x - 5.0f) / 40.0f : 0.0f;
    t = sdfSin(t * 3.14159265f) * 0.2f + 0.1f;
    mnuDrawPlacedSpriteFromTable(10, 0, 0, (s32)(scale * t), 1, context);
    t = x < 35.0f ? x / 35.0f : 0.0f;
    t = sdfSin(t * 3.14159265f) * 0.5f + 0.5f;
    mnuDrawPlacedSpriteFromTable(0, 0, 0, (s32)(scale * t), 1, context);
}

void mnuDrawMantraDownNavigationPulse(s32 frame, s32 size, s32 context) {
    f32 x = frame;
    f32 scale = size;
    f32 t;

    t = x > 5.0f ? (x - 5.0f) / 40.0f : 0.0f;
    t = sdfSin(t * 3.14159265f) * 0.2f + 0.1f;
    mnuDrawPlacedSpriteFromTable(0, 10, 0, (s32)(scale * t), 2, context);
    t = x < 35.0f ? x / 35.0f : 0.0f;
    t = sdfSin(t * 3.14159265f) * 0.5f + 0.5f;
    mnuDrawPlacedSpriteFromTable(0, 0, 0, (s32)(scale * t), 2, context);
}

void mnuDrawMantraLeftNavigationPulse(s32 frame, s32 size, s32 context) {
    f32 x = frame;
    f32 scale = size;
    f32 t;

    t = x > 5.0f ? (x - 5.0f) / 40.0f : 0.0f;
    t = sdfSin(t * 3.14159265f) * 0.2f + 0.1f;
    mnuDrawPlacedSpriteFromTable(-10, 0, 0, (s32)(scale * t), 3, context);
    t = x < 35.0f ? x / 35.0f : 0.0f;
    t = sdfSin(t * 3.14159265f) * 0.5f + 0.5f;
    mnuDrawPlacedSpriteFromTable(0, 0, 0, (s32)(scale * t), 3, context);
}

void mnuDrawMantraNavigationPulses(MenuSceneWork *display, s32 inputScale, s32 context) {
    s8 enabled[4] __attribute__((aligned(4)));
    const u32 clearWord = 0;
    MantraPulseAnimationWork *pulse = &display->pulse;
    s32 threshold;
    s32 i;

    memcpy(enabled, &clearWord, sizeof(enabled));
    if (display->cursorPosition.y != 0) {
        enabled[0] = 1;
    }
    if (display->cursorPosition.y < 0x38E) {
        enabled[1] = 1;
    }
    if (display->cursorPosition.x != 0) {
        enabled[2] = 1;
    }

    if (display->boundsFlags & MENU_SCENE_REQUIREMENT_GROUP_2_MET) {
        threshold = 0x307;
    } else if (display->boundsFlags & MENU_SCENE_REQUIREMENT_GROUP_1_MET) {
        threshold = 0x2C8;
    } else if (display->boundsFlags & MENU_SCENE_REQUIREMENT_GROUP_0_MET) {
        threshold = 0x24C;
    } else {
        threshold = 0x1BE;
    }
    if (display->cursorPosition.x < threshold) {
        enabled[3] = 1;
    }

    for (i = 0; i < 4; i++) {
        if (enabled[i] == 1) {
            pulse->alpha[i] += 0x20;
            if (pulse->alpha[i] > 0x80) {
                pulse->alpha[i] = 0x80;
            }
        } else {
            pulse->alpha[i] -= 0x20;
            if (pulse->alpha[i] < 0) {
                pulse->alpha[i] = 0;
            }
        }

        switch (i) {
        case 0:
            mnuDrawMantraUpNavigationPulse(pulse->frame,
                          (s32)((f32)(inputScale * pulse->alpha[i]) * 0.0078125f),
                          context);
            break;
        case 1:
            mnuDrawMantraDownNavigationPulse(pulse->frame,
                          (s32)((f32)(inputScale * pulse->alpha[i]) * 0.0078125f),
                          context);
            break;
        case 2:
            mnuDrawMantraLeftNavigationPulse(pulse->frame,
                          (s32)((f32)(inputScale * pulse->alpha[i]) * 0.0078125f),
                          context);
            break;
        case 3:
            mnuDrawMantraRightNavigationPulse(pulse->frame,
                          (s32)((f32)(inputScale * pulse->alpha[i]) * 0.0078125f),
                          context);
            break;
        }
    }

    pulse->frame++;
    if ((f32)pulse->frame > 45.0f) {
        pulse->frame = 0;
    }
}

extern MnuSpritePlacement mnuMantraCommonSpritePlacements[];
extern struct EffectSlotSet *mnuMantraSpriteSlots[];
extern char D_003BC4C8[];
extern void effDrawSpriteWithCornerFade(s32, s32, s32, u32, s32, s32, s32, s32);
extern s32 itfDrawGlyphChainWithWidthQuery(s32, s32, s32, u32, u8, char *, s32, u32);

/* Interpolate currency changes over twenty draws, with the native ticking sound. */
void mnuDrawAnimatedCurrencyCounter(s32 x, s32 y, s32 depth, s32 alpha,
                  MenuSceneMetadata *scene, s32 context) {
    char currencyText[16];
    u32 color = alpha | 0xA09DC300;
    struct EffectSlotSet *resource;

    resource = mnuMantraSpriteSlots[mnuMantraCommonSpritePlacements[19].resourceIndex];
    effDrawSpriteWithCornerFade((x + mnuMantraCommonSpritePlacements[19].x) << 4,
                  (y + mnuMantraCommonSpritePlacements[19].y) << 3, depth,
                  (u32)((f32)(alpha << 8) * 0.0078125f), 0,
                  (s32)resource,
                  mnuMantraCommonSpritePlacements[19].spriteIndex, context);
    resource = mnuMantraSpriteSlots[mnuMantraCommonSpritePlacements[34].resourceIndex];
    effDrawSpriteWithCornerFade((x + mnuMantraCommonSpritePlacements[34].x) << 4,
                  (y + mnuMantraCommonSpritePlacements[34].y) << 3, depth,
                  (u32)((f32)(alpha << 8) * 0.0078125f), 0,
                  (s32)resource,
                  mnuMantraCommonSpritePlacements[34].spriteIndex, context);
    if (DDS3_GBWK->header.currency != scene->displayedCurrency) {
        p2sdPlaySE(0x13, 0x7F, 0x3F);
        scene->currencyFrame++;
        sprintf(currencyText, D_003BC4C8,
                      scene->displayedCurrency +
                      (DDS3_GBWK->header.currency - scene->displayedCurrency) *
                          scene->currencyFrame / 20);
        if (scene->currencyFrame == 20) {
            scene->displayedCurrency = DDS3_GBWK->header.currency;
            scene->currencyFrame = 0;
        }
    } else {
        sprintf(currencyText, D_003BC4C8, DDS3_GBWK->header.currency);
    }
    itfDrawGlyphChainWithWidthQuery(x + 0x191, y + 0x39, depth, color,
                                    0, currencyText, 0, context);
}

void mnuDrawScaledMantraGraphCell(s32 x, s32 y, s32 z, SdfGrid *grid, SdfGridCell *cell, s32 surface) {
    u32 prerequisiteFlags[4];
    MnuMantraGridEntry *scene;
    MnuProfileProgress *selection;
    MnuMantraNodeState *states;
    MantraPrerequisiteRecord *record;
    s8 i;
    u32 flags;
    u32 color;

    scene = (MnuMantraGridEntry *)(u32)cell->value;
    if (scene == NULL) {
        return;
    }
    x = (s32)((f32)x + (f32)mnuMantraVariantSpritePlacements[scene->sceneId][2] / 10.0f);
    y = (s32)((f32)y + (f32)mnuMantraVariantSpritePlacements[scene->sceneId][3] / 10.0f);
    selection = mnuGetSelectedNodeValue();
    states = grid->userData;
    sdfGetTaskValueByKey(mnuSceneResourceContext, 1);
    record = &mnuMantraPrerequisiteRecords[scene->sceneId];
    memset(prerequisiteFlags, 0, sizeof(prerequisiteFlags));
    if (record->kind == 0) {
        for (i = 0; i < 4 && record->ids[i] != 0; i++) {
            if (states[record->ids[i]].state != MNU_MANTRA_GRID_ENTRY_ABSENT) {
                prerequisiteFlags[i] = mnuGetMantraPrerequisiteFlags(i, scene, states, selection);
            }
        }
        /* The prerequisite markers are no longer drawn; only their colours remain. */
        for (i = 0; i < 4; i++) {
            if (prerequisiteFlags[i] & 0x100) {
                color = 0x50505080;
                if (prerequisiteFlags[i] & 2) {
                    color = 0x80808080;
                }
            }
        }
    }
    switch (scene->backgroundColorIndex) {
    case 1:
        uiDrawUniformColorRect((x - 2) << 4, (y - 2) << 3, 0, 0x120, 0xA0, 0x10707020, surface);
        break;
    case 2:
        uiDrawUniformColorRect((x - 2) << 4, (y - 2) << 3, 0, 0x120, 0xA0, 0x60401020, surface);
        break;
    case 3:
        uiDrawUniformColorRect((x - 2) << 4, (y - 2) << 3, 0, 0x120, 0xA0, 0x10601020, surface);
        break;
    case 4:
        uiDrawUniformColorRect((x - 2) << 4, (y - 2) << 3, 0, 0x120, 0xA0, 0x70701020, surface);
        break;
    case 5:
        uiDrawUniformColorRect((x - 2) << 4, (y - 2) << 3, 0, 0x120, 0xA0, 0x70107020, surface);
        break;
    case 6:
        uiDrawUniformColorRect((x - 2) << 4, (y - 2) << 3, 0, 0x120, 0xA0, 0x10107020, surface);
        break;
    case 7:
        uiDrawUniformColorRect((x - 2) << 4, (y - 2) << 3, 0, 0x120, 0xA0, 0x70707020, surface);
        break;
    }
    flags = mnuGetMantraDisplayFlags(scene, selection);
    if (flags & 1) {
        uiDrawUniformColorRect((x - 4) << 4, (y - 4) << 3, 0, 0x1C0, 0xE0, 0x60501050, surface);
    }
    if (flags & 2) {
        frFontMeasureAndQueueGlyph(x, y, z, 0x808020F0, (const u8 *)D_003BC488, surface);
        frFontMeasureAndQueueGlyph(x + 4, y, z, 0x808020F0, (const u8 *)D_003BC458, surface);
    } else if (flags & 4) {
        frFontMeasureAndQueueGlyph(x, y, z, 0x108080F0, (const u8 *)D_003BC488, surface);
    } else if (flags & 8) {
        frFontMeasureAndQueueGlyph(x + 4, y, z, 0x108080F0, (const u8 *)D_003BC458, surface);
    } else if (flags & 0x20) {
        frFontMeasureAndQueueGlyph(x, y, z, 0x40404080, (const u8 *)D_003BC490, surface);
    } else {
        frFontMeasureAndQueueGlyph(x, y, z, 0x40404080, (const u8 *)D_003BC498, surface);
    }
}

void mnuReleaseOptionalDrawAllocation(void *unused, void *allocation) {
    if (allocation != 0) {
        sdfChipFreeDirect(allocation);
    }
}


MnuSpriteResourceGroup *mnuCreateSpriteResource(s32 cueDuration,
                                                u8 initialGenerations,
                                                u8 variant) {
    struct sdfMemHandle_t *allocation = sdfMemAllocDirect(0x48);
    MnuSpriteResourceGroup *resource = (MnuSpriteResourceGroup *)sdfMemGetPtr(allocation);
    memset(resource, 0, 0x48);
    resource->allocation = allocation;
    resource->cueDuration = cueDuration;
    resource->initialGenerations = initialGenerations;
    resource->variant = variant;
    resource->spawnCountdown = (s32)(dds3GetRandFloatA(0) * 30.0f + 10.0f);
    return resource;
}

extern char D_003AF9F0[];


/* Spawn a cue at a random cell, or one hex step from its parent in the turned direction. */
SpriteSpawnNode *mnuCreateSpriteSpawnNode(MnuSpriteResourceGroup *group, SpriteSpawnNode *parent, s8 turn) {
    HexStepTable steps = *(HexStepTable *)D_003AF9F0;
    SpriteSpawnNode *node = sdfChipAllocDirect(sizeof(SpriteSpawnNode));

    memset(node, 0, sizeof(SpriteSpawnNode));
    if (parent != NULL) {
        s8 direction = turn + parent->direction;

        if (direction < 0) {
            direction += 6;
        }
        if (direction >= 6) {
            direction -= 6;
        }
        node->x = parent->x + steps.offsets[direction * 2];
        node->y = parent->y + steps.offsets[direction * 2 + 1];
        node->generations = parent->generations - 1;
        node->direction = direction;
        node->framesLeft = node->duration = group->cueDuration - 5;
    } else {
        node->x = (s32)(dds3GetRandFloatA(0) * 30.0f) * 26 + 16;
        node->y = (s32)(dds3GetRandFloatA(0) * 20.0f) * 18 + 50;
        node->generations = group->initialGenerations;
        node->direction = 0;
        node->framesLeft = node->duration = group->cueDuration;
    }
    return node;
}

INCLUDE_RODATA(const s32, "game/code_0025A680", D_003AF9F0);

INCLUDE_SDATA(const s32, "game/code_0025A680", D_003BC4C8);

INCLUDE_SDATA(const s32, "game/code_0025A680", mnuSceneResourceContext);

