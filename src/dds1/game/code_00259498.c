#include "mnu.h"
#include "sdf_task_work.h"
#include "sdf_resource.h"
#include "dat_state.h"
#include "mnu_scene.h"
#include "mnu_profile_progress.h"
#include "mnu_mantra_grid.h"
#include "mnu_sprite_resource.h"
#include "sdf_grid.h"
#include "mnu_scene_work.h"
#include "sdf_chip.h"

extern void sdfReleaseChipBlock(void *);
/* Retail retains a jal and epilogue; default TU -O2 changes the shape. */



extern f32 effMiscRandUnitFloat(s32);

extern void *memset(void *, s32, u32);
extern void *memcpy(void *, const void *, u32);

extern f32 sdfSinPoly(f32);

extern void func_0024E260(s32, s32, s32, s32, s32, s32);

typedef s16 MnuVariantSpritePlacement[6];

extern MnuVariantSpritePlacement D_0036B7F0[];
extern TaskWork *mnuSceneResourceContext;
extern u32 mnuGetMantraDisplayFlags(MnuMantraGridEntry *, MnuProfileProgress *);
extern void uiDrawUniformColorRect(s32, s32, s32, s32, s32, s32, s32);
extern void func_0024EC08(s32, s32, s32, s32, s32, s32, f32, f32, s32);
extern void func_00258A70(s32, s32, s32, s32, u32, f32, f32, s32);

INCLUDE_ASM(const s32, "game/code_00259498", func_00259498);

INCLUDE_SDATA(const s32, "game/code_00259498", D_003BC4A0);

void func_00259890(s32 x, s32 y, s32 depth, s32 alpha,
                   MnuProfileProgress *profileOwner, SdfGrid *grid,
                   f32 scaleX, f32 scaleY, SdfGridCell *entry,
                   s32 context) {
    u8 mappedIds[6] = { 5, 15, 45, 52, 63, 68 };
    MnuMantraGridEntry *scene;
    s32 drawX;
    s32 drawY;
    s32 i;
    f32 pulse;

    {
        u32 flags;
        MenuSceneWork *display;

        display = (MenuSceneWork *)(u32)sdfGetTaskValueByKey(mnuSceneResourceContext, 1);
        scene = (MnuMantraGridEntry *)(u32)entry->value;
        pulse = 0.0f;
        scene->frame++;
        if ((f32)scene->frame > 60.0f) {
            scene->frame = 0;
        }
        pulse = sdfSinPoly(pulse);
        drawX = x + D_0036B7F0[scene->sceneId][2];
        drawY = y + D_0036B7F0[scene->sceneId][3];
        flags = mnuGetMantraDisplayFlags(scene, profileOwner);

        if (flags & MNU_MANTRA_DISPLAY_FLAG_PROFILE_MATCH) {
            uiDrawUniformColorRect(drawX << 4, drawY << 3, 0, 0x300, 0x180,
                                   (s32)((f32)((alpha * 5) << 4) * 0.0078125f) |
                                       0x60501000,
                                   context);
        }
        {
            s32 displayFlags = (u8)display->boundsFlags;

            if (displayFlags & MENU_SCENE_REQUIREMENT_GROUP_0_MET) {
                func_0024EC08(x, y, depth, alpha, scene->sceneId, 0x20,
                              scaleX, scaleY, context);
            } else {
                for (i = 0; i < 6; i++) {
                    if (scene->sceneId == mappedIds[i]) {
                        func_0024EC08(x, y, depth, alpha, i + 0x59, 0x20,
                                      scaleX, scaleY, context);
                        break;
                    }
                }
                if (i == 6) {
                    func_0024EC08(x, y, depth, alpha, scene->sceneId, 0x20,
                                  scaleX, scaleY, context);
                }
            }
        }
        func_00258A70(drawX, drawY, depth,
                      (s32)((f32)alpha * (pulse * 0.4f + 0.2f)),
                      flags, scaleX, scaleY, context);
    }
}

INCLUDE_ASM(const s32, "game/code_00259498", func_00259B40);


typedef struct MantraPrerequisiteRecord {
    u32 unk00;
    u32 unk04;
    s8 ids[4];
    u8 flags[4];
} MantraPrerequisiteRecord;

extern MantraPrerequisiteRecord D_0036AE80[];
extern char D_003BC458[];
extern char D_003BC488[];
extern char D_003BC490[];
extern char D_003BC498[];
extern u32 mnuGetSelectedNodeValue(void);
extern u32 func_00258508(s32, MnuMantraGridEntry *, MnuMantraNodeState *, MnuProfileProgress *);
extern void mnuDrawScaledVariantSprite(s32, s32, s32, s32, s32, s32, f32, f32, s32);
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
    selection = (MnuProfileProgress *)mnuGetSelectedNodeValue();
    states = (MnuMantraNodeState *)(u32)grid->userData;
    display = (MenuSceneWork *)sdfGetTaskValueByKey(mnuSceneResourceContext, 1);
    record = &D_0036AE80[scene->sceneId];
    alpha = display->displayAlpha;
    memset(prerequisiteFlags, 0, sizeof(prerequisiteFlags));
    if (record->unk00 == 0) {
        for (i = 0; i < 4 && record->ids[i] != 0; i++) {
            if (states[record->ids[i]].state != 0) {
                prerequisiteFlags[i] = func_00258508(i, scene, states, selection);
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

void func_0025AA20(s32 frame, s32 size, s32 param) {
    f32 x = frame;
    f32 scale = size;
    f32 t;

    t = x > 5.0f ? (x - 5.0f) / 40.0f : 0.0f;
    t = sdfSinPoly(t * 3.14159265f) * 0.2f + 0.1f;
    func_0024E260(0, -10, 0, (s32)(scale * t), 0, param);
    t = x < 35.0f ? x / 35.0f : 0.0f;
    t = sdfSinPoly(t * 3.14159265f) * 0.5f + 0.5f;
    func_0024E260(0, 0, 0, (s32)(scale * t), 0, param);
}

void func_0025AB38(s32 frame, s32 size, s32 param) {
    f32 x = frame;
    f32 scale = size;
    f32 t;

    t = x > 5.0f ? (x - 5.0f) / 40.0f : 0.0f;
    t = sdfSinPoly(t * 3.14159265f) * 0.2f + 0.1f;
    func_0024E260(10, 0, 0, (s32)(scale * t), 1, param);
    t = x < 35.0f ? x / 35.0f : 0.0f;
    t = sdfSinPoly(t * 3.14159265f) * 0.5f + 0.5f;
    func_0024E260(0, 0, 0, (s32)(scale * t), 1, param);
}

void func_0025AC50(s32 frame, s32 size, s32 param) {
    f32 x = frame;
    f32 scale = size;
    f32 t;

    t = x > 5.0f ? (x - 5.0f) / 40.0f : 0.0f;
    t = sdfSinPoly(t * 3.14159265f) * 0.2f + 0.1f;
    func_0024E260(0, 10, 0, (s32)(scale * t), 2, param);
    t = x < 35.0f ? x / 35.0f : 0.0f;
    t = sdfSinPoly(t * 3.14159265f) * 0.5f + 0.5f;
    func_0024E260(0, 0, 0, (s32)(scale * t), 2, param);
}

void func_0025AD68(s32 frame, s32 size, s32 param) {
    f32 x = frame;
    f32 scale = size;
    f32 t;

    t = x > 5.0f ? (x - 5.0f) / 40.0f : 0.0f;
    t = sdfSinPoly(t * 3.14159265f) * 0.2f + 0.1f;
    func_0024E260(-10, 0, 0, (s32)(scale * t), 3, param);
    t = x < 35.0f ? x / 35.0f : 0.0f;
    t = sdfSinPoly(t * 3.14159265f) * 0.5f + 0.5f;
    func_0024E260(0, 0, 0, (s32)(scale * t), 3, param);
}

void func_0025AE80(MenuSceneWork *display, s32 inputScale, s32 param) {
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
            func_0025AA20(pulse->frame,
                          (s32)((f32)(inputScale * pulse->alpha[i]) * 0.0078125f),
                          param);
            break;
        case 1:
            func_0025AC50(pulse->frame,
                          (s32)((f32)(inputScale * pulse->alpha[i]) * 0.0078125f),
                          param);
            break;
        case 2:
            func_0025AD68(pulse->frame,
                          (s32)((f32)(inputScale * pulse->alpha[i]) * 0.0078125f),
                          param);
            break;
        case 3:
            func_0025AB38(pulse->frame,
                          (s32)((f32)(inputScale * pulse->alpha[i]) * 0.0078125f),
                          param);
            break;
        }
    }

    pulse->frame++;
    if ((f32)pulse->frame > 45.0f) {
        pulse->frame = 0;
    }
}

typedef struct MnuSpritePlacement {
    s16 resourceIndex;
    s16 spriteIndex;
    s16 x;
    s16 y;
} MnuSpritePlacement;


extern MnuSpritePlacement D_0036B510[];
extern s32 D_0036C698[];
extern char D_003BC4C8[];
extern void func_002BF4E0(s32, s32, s32, u32, s32, s32, s32, s32);
extern void sndSetSequenceVolumePan(s32, s32, s32);
extern s32 func_003014F0(char *, const char *, ...);
extern s32 itfDrawGlyphChainWithWidthQuery(s32, s32, s32, u32, u8, char *, s32, u32);

/* Interpolate currency changes over twenty draws, with the native ticking sound. */
void mnuDrawAnimatedCurrencyCounter(s32 x, s32 y, s32 depth, s32 alpha,
                  MenuSceneMetadata *scene, s32 context) {
    char currencyText[16];
    u32 color = alpha | 0xA09DC300;
    s32 resource;

    resource = D_0036C698[D_0036B510[19].resourceIndex];
    func_002BF4E0((x + D_0036B510[19].x) << 4,
                  (y + D_0036B510[19].y) << 3, depth,
                  (u32)((f32)(alpha << 8) * 0.0078125f), 0,
                  resource,
                  D_0036B510[19].spriteIndex, context);
    resource = D_0036C698[D_0036B510[34].resourceIndex];
    func_002BF4E0((x + D_0036B510[34].x) << 4,
                  (y + D_0036B510[34].y) << 3, depth,
                  (u32)((f32)(alpha << 8) * 0.0078125f), 0,
                  resource,
                  D_0036B510[34].spriteIndex, context);
    if (datGameState->header.currency != scene->displayedCurrency) {
        sndSetSequenceVolumePan(0x13, 0x7F, 0x3F);
        scene->currencyFrame++;
        func_003014F0(currencyText, D_003BC4C8,
                      scene->displayedCurrency +
                      (datGameState->header.currency - scene->displayedCurrency) *
                          scene->currencyFrame / 20);
        if (scene->currencyFrame == 20) {
            scene->displayedCurrency = datGameState->header.currency;
            scene->currencyFrame = 0;
        }
    } else {
        func_003014F0(currencyText, D_003BC4C8, datGameState->header.currency);
    }
    itfDrawGlyphChainWithWidthQuery(x + 0x191, y + 0x39, depth, color,
                                    0, currencyText, 0, context);
}

INCLUDE_ASM(const s32, "game/code_00259498", func_0025B350);

void mnuReleaseOptionalDrawAllocation(void *unused, void *allocation) {
    if (allocation != 0) {
        sdfReleaseChipBlock(allocation);
    }
}


MnuSpriteResourceGroup *mnuCreateSpriteResource(s32 owner, u8 sprite, u8 variant) {
    struct SdfMemBlock *allocation = sdfAllocGeneralBlock(0x48);
    MnuSpriteResourceGroup *resource = (MnuSpriteResourceGroup *)sdfMemoryGetBlockAddress(allocation);
    memset(resource, 0, 0x48);
    resource->allocation = allocation;
    resource->owner = owner;
    resource->sprite = sprite;
    resource->variant = variant;
    resource->spawnCountdown = (s32)(effMiscRandUnitFloat(0) * 30.0f + 10.0f);
    return resource;
}

extern char D_003AF9F0[];


/* Spawn a cue at a random cell, or one hex step from its parent in the turned direction. */
SpriteSpawnNode *func_0025B888(MnuSpriteResourceGroup *group, SpriteSpawnNode *parent, s8 turn) {
    HexStepTable steps = *(HexStepTable *)D_003AF9F0;
    SpriteSpawnNode *node = sdfAllocSizeClassBlock(sizeof(SpriteSpawnNode));

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
        node->framesLeft = node->duration = group->owner - 5;
    } else {
        node->x = (s32)(effMiscRandUnitFloat(0) * 30.0f) * 26 + 16;
        node->y = (s32)(effMiscRandUnitFloat(0) * 20.0f) * 18 + 50;
        node->generations = group->sprite;
        node->direction = 0;
        node->framesLeft = node->duration = group->owner;
    }
    return node;
}

INCLUDE_RODATA(const s32, "game/code_00259498", D_003AF9F0);

INCLUDE_SDATA(const s32, "game/code_00259498", D_003BC4B0);

INCLUDE_SDATA(const s32, "game/code_00259498", D_003BC4B8);

INCLUDE_SDATA(const s32, "game/code_00259498", D_003BC4C0);

INCLUDE_SDATA(const s32, "game/code_00259498", D_003BC4C8);

INCLUDE_SDATA(const s32, "game/code_00259498", mnuSceneResourceContext);

