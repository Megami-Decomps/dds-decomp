#include "common.h"
#include "dsp_channel.h"
#include "mnu_title_stream.h"
#include "sdf_chip.h"
#include "eff_resource_slots.h"
#include "kwln.h"
#include "sdf_resource.h"
#include "eff.h"
#include "mnu_list.h"
#include "mnu_profile_progress.h"
#include "mnu_sprite_resource.h"
#include "prf_requirement.h"
#include "sdf_grid.h"
#include "sdf_task_work.h"
#include "mnu_scene_work.h"

#define MNU_MANTRA_RESOURCE_SLOT_COUNT 14
#define MNU_MANTRA_SOURCE_ACTIVE_BIT 0x20
#define MNU_PARTY_RECORD_BYTES 0x1A4
#define MNU_GAME_PARTY_RECORD_OFFSET 0xA60
#define MNU_PARTY_ORDER_COUNT 32
#define MNU_VISIBLE_PARTY_COUNT 5
#define MNU_LIST_DRAW_VALUE_BYTES 8
#define MNU_RESOURCE_LIST_WORK_BYTES 0x10
#define MNU_SCENE_FADE_FRAMES 10.0f
#define MNU_SCENE_ALPHA_SCALE 128.0f
#define MNU_SCENE_FULL_ALPHA 0x80
#define MNU_REQUIREMENT_SLOT_COUNT 2
#define MNU_REQUIREMENT_MIN_COUNT 2


extern u8 *datGameState;
extern s8 scrGetSelectedOperandIndex(struct DatPartyRecord *);
extern u32 ptyGetProfileRecordValue(struct DatPartyRecord *, u16);
extern u32 prfGetCapValue(u16);


extern u8 mnuResourceTaskName[];
extern char D_003AF758[];
extern char D_003AF780[];

extern TaskWork *mnuSceneResourceContext;
extern EffectSlotSet *mnuMantraSpriteSlots[];
extern u8 mnuMantraResourceTaskDescriptor[];

typedef s16 MnuSpritePlacement[4];


typedef s16 MnuVariantSpritePlacement[6];

enum {
    MNU_VARIANT_X_OFFSET = 2,
    MNU_VARIANT_Y_OFFSET,
    MNU_VARIANT_SPRITE_GROUPS,
};

enum {
    MNU_SPRITE_RESOURCE_INDEX,
    MNU_SPRITE_INDEX,
    MNU_SPRITE_X_OFFSET,
    MNU_SPRITE_Y_OFFSET,
};

extern MnuSpritePlacement mnuMantraCommonSpritePlacements[];
extern MnuVariantSpritePlacement mnuMantraVariantSpritePlacements[];
extern u16 mnuMantraCoordinateSpritePlacements[][4];
extern s32 mnuMantraVariantSpriteSlotView[];
extern void effDrawSpriteWithCornerFade(s32, s32, s32, u32, s32, s32, s32, s32);
extern void mnuDestroyMantraDrawPool(MnuSpriteResourceGroup *);
extern void mnuReleaseStaffMenuContextAndResources(MenuProgressHost *);
extern void mnuTitleStreamUpdateAndLogBgm(void);
extern MnuSpriteResourceGroup *mnuCreateSpriteResource(s32, u8, u8);
extern MenuProgressHost *mnuCreateWorkBlock(void);

typedef struct MnuResourceTaskWork {
    struct SdfMemBlock *allocation;
    u8 pad04[4];
    struct SdfMemBlock *messageResource1;
    u32 messageResourceInfo1;
    struct SdfMemBlock *messageResource2;
    u32 messageResourceInfo2;
    u8 pad18[0xC];
    MenuProgressHost *staffMenuContext;
    u8 pad28[0x210];
    MnuSpriteResourceGroup *drawPool;
    u8 pad23C[0xC];
} MnuResourceTaskWork;

void mnuDrawSpriteAtPlacement(s32 x, s32 y, s32 z, s32 alpha, s32 sprite, s32 placementIndex,
                  s32 flags, s32 context) {
    effDrawSpriteWithCornerFade((x + mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_X_OFFSET]) << 4,
                         (y + mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_Y_OFFSET]) << 3,
                         z,
                         (u32)((f32)(alpha << 8) * 0.0078125f),
                         flags,
                         sprite,
                         mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_INDEX],
                         context);
}

void mnuDrawPlacedSpriteFromTable(s32 x, s32 y, s32 z, s32 alpha, s32 placementIndex, s32 context) {
    effDrawSpriteWithCornerFade((x + mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_X_OFFSET]) << 4,
                         (y + mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_Y_OFFSET]) << 3,
                         z,
                         (u32)((f32)(alpha << 8) * 0.0078125f),
                         0,
                         (s32)mnuMantraSpriteSlots[mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_RESOURCE_INDEX]],
                         mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_INDEX],
                         context);
}

void mnuDrawPlacedSpriteByIndex(s32 x, s32 y, s32 z, s32 alpha, s32 placementIndex, s32 mode,
                  s32 context) {
    effDrawSpriteWithCornerFade((x + mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_X_OFFSET]) << 4,
                         (y + mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_Y_OFFSET]) << 3,
                         z,
                         (u32)((f32)(alpha << 8) * 0.0078125f),
                         0,
                         (s32)mnuMantraSpriteSlots[mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_RESOURCE_INDEX]],
                         mode,
                         context);
}

void mnuDrawPlacedSpriteWithFlags(s32 x, s32 y, s32 z, s32 alpha, s32 flags, s32 placementIndex,
                  s32 context) {
    effDrawSpriteWithCornerFade((x + mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_X_OFFSET]) << 4,
                         (y + mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_Y_OFFSET]) << 3,
                         z,
                         (u32)((f32)(alpha << 8) * 0.0078125f),
                         flags,
                         (s32)mnuMantraSpriteSlots[mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_RESOURCE_INDEX]],
                         mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_INDEX],
                         context);
}

void mnuDrawRotatedPlacedSprite(s32 x, s32 y, s32 z, s32 alpha, s32 flags, s32 placementIndex,
                   s32 context, f32 rotation) {
    (mnuMantraSpriteSlots[
        mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_RESOURCE_INDEX]])
        ->workEntries[mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_INDEX]].geometry.angleDegrees = rotation;
    effDrawSpriteWithCornerFade((x + mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_X_OFFSET]) << 4,
                  (y + mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_Y_OFFSET]) << 3,
                  z,
                  (u32)((f32)(alpha << 8) * 0.0078125f),
                  flags,
                  (s32)mnuMantraSpriteSlots[mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_RESOURCE_INDEX]],
                  mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_INDEX],
                  context);
    (mnuMantraSpriteSlots[
        mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_RESOURCE_INDEX]])
        ->workEntries[mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_INDEX]].geometry.angleDegrees = 0.0f;
}

void mnuDrawScaledPlacedSprite(s32 x, s32 y, s32 z, s32 alpha, s32 placementIndex,
                   s32 context, f32 scaleX, f32 scaleY) {
    {
        EffectSlotSet *resource =
            mnuMantraSpriteSlots[
                mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_RESOURCE_INDEX]];

        resource->workEntries[mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_INDEX]].geometry.bounds[2] =
            (s32)(scaleX *
                  (f32)resource
                      ->workEntries[mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_INDEX]]
                      .sourceWidth)
            << 4;
        resource->workEntries[mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_INDEX]].geometry.bounds[3] =
            (s32)(scaleY *
                  (f32)resource
                      ->workEntries[mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_INDEX]]
                      .sourceHeight)
            << 3;
    }
    effDrawSpriteWithCornerFade((x + mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_X_OFFSET]) << 4,
                  (y + mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_Y_OFFSET]) << 3,
                  z,
                  (u32)((f32)(alpha << 8) * 0.0078125f),
                  0,
                  (s32)mnuMantraSpriteSlots[mnuMantraCommonSpritePlacements[placementIndex]
                                         [MNU_SPRITE_RESOURCE_INDEX]],
                  mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_INDEX],
                  context);

    {
        /* Reload the resource after drawing before restoring native size. */
        EffectSlotSet *resource =
            mnuMantraSpriteSlots[
                mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_RESOURCE_INDEX]];

        resource->workEntries[mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_INDEX]].geometry.bounds[2] =
            resource->workEntries[mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_INDEX]]
                .sourceWidth
            << 4;
        resource->workEntries[mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_INDEX]].geometry.bounds[3] =
            resource->workEntries[mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_INDEX]]
                .sourceHeight
            << 3;
    }
}

void mnuDrawSpriteAtScaledPlacement(s32 x, s32 y, s32 z, s32 alpha, s32 flags,
                   s32 placementIndex, s32 context, f32 scaleX, f32 scaleY) {
    {
        EffectSlotSet *resource =
            mnuMantraSpriteSlots[
                mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_RESOURCE_INDEX]];

        resource->workEntries[mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_INDEX]].geometry.bounds[2] =
            (s32)(scaleX *
                  (f32)resource->workEntries[mnuMantraCommonSpritePlacements[placementIndex]
                                            [MNU_SPRITE_INDEX]]
                      .sourceWidth)
            << 4;
        resource->workEntries[mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_INDEX]].geometry.bounds[3] =
            (s32)(scaleY *
                  (f32)resource->workEntries[mnuMantraCommonSpritePlacements[placementIndex]
                                            [MNU_SPRITE_INDEX]]
                      .sourceHeight)
            << 3;
    }
    effDrawSpriteWithCornerFade((s32)((f32)(x + mnuMantraCommonSpritePlacements[placementIndex]
                                           [MNU_SPRITE_X_OFFSET]) *
                        scaleX)
                      << 4,
                  (s32)((f32)(y + mnuMantraCommonSpritePlacements[placementIndex]
                                           [MNU_SPRITE_Y_OFFSET]) *
                        scaleY)
                      << 3,
                  z,
                  (u32)((f32)(alpha << 8) * 0.0078125f),
                  flags,
                  (s32)mnuMantraSpriteSlots[mnuMantraCommonSpritePlacements[placementIndex]
                                         [MNU_SPRITE_RESOURCE_INDEX]],
                  mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_INDEX],
                  context);

    {
        EffectSlotSet *resource =
            mnuMantraSpriteSlots[
                mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_RESOURCE_INDEX]];

        resource->workEntries[mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_INDEX]].geometry.bounds[2] =
            resource->workEntries[mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_INDEX]]
                .sourceWidth
            << 4;
        resource->workEntries[mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_INDEX]].geometry.bounds[3] =
            resource->workEntries[mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_INDEX]]
                .sourceHeight
            << 3;
    }
}

void mnuDrawScaledPlacedSpriteWithFlags(s32 x, s32 y, s32 z, s32 alpha, s32 flags,
                   s32 placementIndex, s32 context, f32 scaleX, f32 scaleY) {
    {
        EffectSlotSet *resource =
            mnuMantraSpriteSlots[
                mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_RESOURCE_INDEX]];

        resource->workEntries[mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_INDEX]]
            .geometry.bounds[2] = (s32)(scaleX *
                           (f32)resource->workEntries[mnuMantraCommonSpritePlacements[placementIndex]
                                                      [MNU_SPRITE_INDEX]]
                               .sourceWidth)
                    << 4;
        resource->workEntries[mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_INDEX]]
            .geometry.bounds[3] = (s32)(scaleY *
                            (f32)resource->workEntries[mnuMantraCommonSpritePlacements[placementIndex]
                                                       [MNU_SPRITE_INDEX]]
                                .sourceHeight)
                     << 3;
    }
    effDrawSpriteWithCornerFade((x + mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_X_OFFSET]) << 4,
                  (y + mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_Y_OFFSET]) << 3,
                  z,
                  (u32)((f32)(alpha << 8) * 0.0078125f),
                  flags,
                  (s32)mnuMantraSpriteSlots[mnuMantraCommonSpritePlacements[placementIndex]
                                         [MNU_SPRITE_RESOURCE_INDEX]],
                  mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_INDEX],
                  context);

    {
        EffectSlotSet *resource =
            mnuMantraSpriteSlots[
                mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_RESOURCE_INDEX]];

        resource->workEntries[mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_INDEX]]
            .geometry.bounds[2] =
            resource->workEntries[mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_INDEX]]
                .sourceWidth
            << 4;
        resource->workEntries[mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_INDEX]]
            .geometry.bounds[3] =
            resource->workEntries[mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_INDEX]]
                .sourceHeight
            << 3;
    }
}

void mnuDrawScaledVariantSprite(s32 x, s32 y, s32 z, s32 alpha, s32 placementIndex,
                   s32 flags, f32 scaleX, f32 scaleY, s32 context) {
    {
        EffectSlotSet *resource = mnuMantraSpriteSlots[
            mnuMantraVariantSpritePlacements[placementIndex][MNU_SPRITE_RESOURCE_INDEX]];

        resource->workEntries[mnuMantraVariantSpritePlacements[placementIndex][MNU_SPRITE_INDEX]].geometry.bounds[2] =
            (s32)(scaleX * (f32)resource->workEntries[mnuMantraVariantSpritePlacements[placementIndex][MNU_SPRITE_INDEX]].sourceWidth) << 4;
        resource->workEntries[mnuMantraVariantSpritePlacements[placementIndex][MNU_SPRITE_INDEX]].geometry.bounds[3] =
            (s32)(scaleY * (f32)resource->workEntries[mnuMantraVariantSpritePlacements[placementIndex][MNU_SPRITE_INDEX]].sourceHeight) << 3;
    }
    effDrawSpriteWithCornerFade((s32)((f32)(x + mnuMantraVariantSpritePlacements[placementIndex][MNU_SPRITE_X_OFFSET]) * scaleX) << 4,
                  (s32)((f32)(y + mnuMantraVariantSpritePlacements[placementIndex][MNU_SPRITE_Y_OFFSET]) * scaleY) << 3,
                  z,
                  (u32)((f32)(alpha << 8) * 0.0078125f),
                  flags,
                  (s32)mnuMantraSpriteSlots[mnuMantraVariantSpritePlacements[placementIndex][MNU_SPRITE_RESOURCE_INDEX]],
                  mnuMantraVariantSpritePlacements[placementIndex][MNU_SPRITE_INDEX],
                  context);
    {
        EffectSlotSet *resource = mnuMantraSpriteSlots[
            mnuMantraVariantSpritePlacements[placementIndex][MNU_SPRITE_RESOURCE_INDEX]];

        resource->workEntries[mnuMantraVariantSpritePlacements[placementIndex][MNU_SPRITE_INDEX]].geometry.bounds[2] =
            resource->workEntries[mnuMantraVariantSpritePlacements[placementIndex][MNU_SPRITE_INDEX]].sourceWidth << 4;
        resource->workEntries[mnuMantraVariantSpritePlacements[placementIndex][MNU_SPRITE_INDEX]].geometry.bounds[3] =
            resource->workEntries[mnuMantraVariantSpritePlacements[placementIndex][MNU_SPRITE_INDEX]].sourceHeight << 3;
    }
}

void mnuDrawScaledScenePlacementSprite(s32 x, s32 y, s32 z, s32 alpha, s32 placementIndex,
                   s32 flags, f32 scaleX, f32 scaleY, s32 context) {
    {
        EffectSlotSet *resource = mnuMantraSpriteSlots[
            mnuMantraVariantSpritePlacements[placementIndex][MNU_SPRITE_RESOURCE_INDEX]];

        resource->workEntries[mnuMantraVariantSpritePlacements[placementIndex][MNU_SPRITE_INDEX]].geometry.bounds[2] =
            (s32)(scaleX * (f32)resource->workEntries[mnuMantraVariantSpritePlacements[placementIndex][MNU_SPRITE_INDEX]].sourceWidth) << 4;
        resource->workEntries[mnuMantraVariantSpritePlacements[placementIndex][MNU_SPRITE_INDEX]].geometry.bounds[3] =
            (s32)(scaleY * (f32)resource->workEntries[mnuMantraVariantSpritePlacements[placementIndex][MNU_SPRITE_INDEX]].sourceHeight) << 3;
    }
    effDrawSpriteWithCornerFade((s32)((f32)(x + mnuMantraVariantSpritePlacements[placementIndex][MNU_SPRITE_X_OFFSET]) * scaleX) << 4,
                  (s32)((f32)(y + mnuMantraVariantSpritePlacements[placementIndex][MNU_SPRITE_Y_OFFSET]) * scaleY) << 3,
                  z,
                  (u32)((f32)(alpha << 8) * 0.0078125f),
                  flags,
                  (s32)mnuMantraSpriteSlots[mnuMantraVariantSpritePlacements[placementIndex][MNU_SPRITE_RESOURCE_INDEX]],
                  mnuMantraVariantSpritePlacements[placementIndex][MNU_SPRITE_INDEX],
                  context);
    {
        EffectSlotSet *resource = mnuMantraSpriteSlots[
            mnuMantraVariantSpritePlacements[placementIndex][MNU_SPRITE_RESOURCE_INDEX]];

        resource->workEntries[mnuMantraVariantSpritePlacements[placementIndex][MNU_SPRITE_INDEX]].geometry.bounds[2] =
            resource->workEntries[mnuMantraVariantSpritePlacements[placementIndex][MNU_SPRITE_INDEX]].sourceWidth << 4;
        resource->workEntries[mnuMantraVariantSpritePlacements[placementIndex][MNU_SPRITE_INDEX]].geometry.bounds[3] =
            resource->workEntries[mnuMantraVariantSpritePlacements[placementIndex][MNU_SPRITE_INDEX]].sourceHeight << 3;
    }
}

void mnuDrawScaledSpriteAtCoordinatePlacement(s32 x, s32 y, s32 z, s32 alpha, s32 placementIndex,
                   s32 flags, s32 context, f32 scaleX, f32 scaleY) {
    {
        EffectSlotSet *resource =
            mnuMantraSpriteSlots[
                mnuMantraCoordinateSpritePlacements[placementIndex][MNU_SPRITE_RESOURCE_INDEX]];

        resource->workEntries[mnuMantraCoordinateSpritePlacements[placementIndex][MNU_SPRITE_INDEX]].geometry.bounds[2] =
            (s32)(scaleX *
                  (f32)resource->workEntries[mnuMantraCoordinateSpritePlacements[placementIndex]
                                            [MNU_SPRITE_INDEX]]
                      .sourceWidth)
            << 4;
        resource->workEntries[mnuMantraCoordinateSpritePlacements[placementIndex][MNU_SPRITE_INDEX]].geometry.bounds[3] =
            (s32)(scaleY *
                  (f32)resource->workEntries[mnuMantraCoordinateSpritePlacements[placementIndex]
                                            [MNU_SPRITE_INDEX]]
                      .sourceHeight)
            << 3;
    }
    effDrawSpriteWithCornerFade((s32)((f32)(x + mnuMantraCoordinateSpritePlacements[placementIndex]
                                           [MNU_SPRITE_X_OFFSET]) *
                        scaleX)
                      << 4,
                  (s32)((f32)(y + mnuMantraCoordinateSpritePlacements[placementIndex]
                                           [MNU_SPRITE_Y_OFFSET]) *
                        scaleY)
                      << 3,
                  z,
                  (u32)((f32)(alpha << 8) * 0.0078125f),
                  flags,
                  (s32)mnuMantraSpriteSlots[mnuMantraCoordinateSpritePlacements[placementIndex]
                                         [MNU_SPRITE_RESOURCE_INDEX]],
                  mnuMantraCoordinateSpritePlacements[placementIndex][MNU_SPRITE_INDEX],
                  context);

    {
        /* Reload the resource after drawing before restoring native size. */
        EffectSlotSet *resource =
            mnuMantraSpriteSlots[
                mnuMantraCoordinateSpritePlacements[placementIndex][MNU_SPRITE_RESOURCE_INDEX]];

        resource->workEntries[mnuMantraCoordinateSpritePlacements[placementIndex][MNU_SPRITE_INDEX]].geometry.bounds[2] =
            resource->workEntries[mnuMantraCoordinateSpritePlacements[placementIndex][MNU_SPRITE_INDEX]]
                .sourceWidth
            << 4;
        resource->workEntries[mnuMantraCoordinateSpritePlacements[placementIndex][MNU_SPRITE_INDEX]].geometry.bounds[3] =
            resource->workEntries[mnuMantraCoordinateSpritePlacements[placementIndex][MNU_SPRITE_INDEX]]
                .sourceHeight
            << 3;
    }
}

void mnuDrawScaledFixedSpritePlacement(s32 x, s32 y, s32 z, s32 alpha, s32 placementIndex,
                   s32 flags, s32 context, f32 scaleX, f32 scaleY) {
    {
        EffectSlotSet *resource = mnuMantraSpriteSlots[
            mnuMantraCoordinateSpritePlacements[placementIndex][MNU_SPRITE_RESOURCE_INDEX]];

        resource->workEntries[6].geometry.bounds[2] =
            (s32)(scaleX * (f32)resource->workEntries[6].sourceWidth) << 4;
        resource->workEntries[6].geometry.bounds[3] =
            (s32)(scaleY * (f32)resource->workEntries[6].sourceHeight) << 3;
    }
    effDrawSpriteWithCornerFade(
        (s32)((f32)(x + mnuMantraCoordinateSpritePlacements[placementIndex][MNU_SPRITE_X_OFFSET]) *
              scaleX) << 4,
        (s32)((f32)(y + mnuMantraCoordinateSpritePlacements[placementIndex][MNU_SPRITE_Y_OFFSET]) *
              scaleY) << 3,
        z, (u32)((f32)(alpha << 8) * 0.0078125f), flags,
        (s32)mnuMantraSpriteSlots[mnuMantraCoordinateSpritePlacements[placementIndex][MNU_SPRITE_RESOURCE_INDEX]],
        6, context);
    {
        /* Reload the resource after drawing before restoring native size. */
        EffectSlotSet *resource = mnuMantraSpriteSlots[
            mnuMantraCoordinateSpritePlacements[placementIndex][MNU_SPRITE_RESOURCE_INDEX]];

        resource->workEntries[6].geometry.bounds[2] =
            resource->workEntries[6].sourceWidth << 4;
        resource->workEntries[6].geometry.bounds[3] =
            resource->workEntries[6].sourceHeight << 3;
    }
}

void mnuDrawVariantSpriteBySelector(s32 x, s32 y, s32 z, s32 alpha, s32 placementIndex,
                  s32 selector, s32 flags, s32 context,
                  f32 scaleX, f32 scaleY) {
    s8 spriteMap[15] = { -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 17, 18 };
    s32 sprite;

    sprite = spriteMap[(mnuMantraVariantSpritePlacements[placementIndex][MNU_VARIANT_SPRITE_GROUPS] >>
                        (selector * 4)) & 0xF];
    if (sprite == -1) {
        return;
    }
    effDrawSpriteWithCornerFade((s32)((f32)(x + mnuMantraVariantSpritePlacements[placementIndex]
                                           [MNU_VARIANT_X_OFFSET]) * scaleX) << 4,
                  (s32)((f32)(y + mnuMantraVariantSpritePlacements[placementIndex]
                                           [MNU_VARIANT_Y_OFFSET]) * scaleY) << 3,
                  z,
                  (u32)((f32)(alpha << 8) * 0.0078125f),
                  flags, mnuMantraVariantSpriteSlotView[0], sprite, context);
}


void mnuDrawVariantSpriteAtCoordinatePlacement(s32 x, s32 y, s32 z, s32 alpha, s32 groupPlacementIndex,
                   s32 coordinatePlacementIndex, s32 selector, s32 flags,
                   f32 scaleX, f32 scaleY, s32 context) {
    s8 spriteMap[12] = { -1, 12, 26, 27, 13, 14, 15, 16, 20, 21, 23, 24 };
    s32 sprite;

    selector += 2;
    sprite = spriteMap[(mnuMantraVariantSpritePlacements[groupPlacementIndex][MNU_VARIANT_SPRITE_GROUPS] >>
                        (selector * 4)) & 0xF];
    if (sprite == -1) {
        return;
    }
    effDrawSpriteWithCornerFade((s32)((f32)(x + mnuMantraVariantSpritePlacements[coordinatePlacementIndex]
                                              [MNU_VARIANT_X_OFFSET]) * scaleX) << 4,
                  (s32)((f32)(y + mnuMantraVariantSpritePlacements[coordinatePlacementIndex]
                                              [MNU_VARIANT_Y_OFFSET]) * scaleY) << 3,
                  z,
                  (u32)((f32)(alpha << 8) * 0.0078125f),
                  flags,
                  mnuMantraVariantSpriteSlotView[0],
                  sprite,
                  context);
}

void mnuDrawScaledVariantSpriteByIndex(s32 x, s32 y, s32 z, s32 alpha, s32 placementIndex,
                   s32 spriteIndex, s32 flags, s32 context, f32 scaleX,
                   f32 scaleY) {
    {
        EffectSlotSet *resource =
            mnuMantraSpriteSlots[mnuMantraVariantSpritePlacements[placementIndex][0]];

        resource->workEntries[spriteIndex].geometry.bounds[2] =
            (s32)(scaleX * (f32)resource->workEntries[spriteIndex].sourceWidth) << 4;
        resource->workEntries[spriteIndex].geometry.bounds[3] =
            (s32)(scaleY * (f32)resource->workEntries[spriteIndex].sourceHeight) << 3;
    }
    effDrawSpriteWithCornerFade(
        (s32)((f32)(x + mnuMantraVariantSpritePlacements[placementIndex][MNU_VARIANT_X_OFFSET]) * scaleX) << 4,
        (s32)((f32)(y + mnuMantraVariantSpritePlacements[placementIndex][MNU_VARIANT_Y_OFFSET]) * scaleY) << 3,
        z, (u32)((f32)(alpha << 8) * 0.0078125f), flags,
        (s32)mnuMantraSpriteSlots[5], spriteIndex, context);
    {
        EffectSlotSet *resource =
            mnuMantraSpriteSlots[mnuMantraVariantSpritePlacements[placementIndex][0]];

        resource->workEntries[spriteIndex].geometry.bounds[2] =
            resource->workEntries[spriteIndex].sourceWidth << 4;
        resource->workEntries[spriteIndex].geometry.bounds[3] =
            resource->workEntries[spriteIndex].sourceHeight << 3;
    }
}

extern u8 mnuMantraSpriteResourceNames[];
extern void effRequestResourceByMode(char *, void *, s32, void *);

void mnuRequestMantraResources(MnuResourceTaskWork *work) {
    s32 i;

    for (i = 0; i < 14; i++) {
        if (mnuMantraSpriteSlots[i] == 0) {
            effRequestResourceByMode("/facility/spr/mantra/", &mnuMantraSpriteResourceNames[i * 0x10], 0, (u32 *)&mnuMantraSpriteSlots[i]);
        }
    }
}

/* Return one only when every mantra sprite resource slot is occupied. */
s32 mnuAreResourceSlotsOccupied(void) {
    s32 slotIndex;
    for (slotIndex = 0; slotIndex < MNU_MANTRA_RESOURCE_SLOT_COUNT; ++slotIndex) {
        if (mnuMantraSpriteSlots[slotIndex] == 0) {
            return 0;
        }
    }
    return 1;
}

/* Release nonzero mantra sprite slots and clear their stored handles; the work argument is unused. */
void mnuReleaseResourceSlots(MnuResourceTaskWork *unusedWork) {
    s32 slotIndex;
    for (slotIndex = 0; slotIndex < MNU_MANTRA_RESOURCE_SLOT_COUNT; ++slotIndex) {
        if (mnuMantraSpriteSlots[slotIndex] != 0) {
            effDestroyResourceSlotSet(mnuMantraSpriteSlots[slotIndex]);
            mnuMantraSpriteSlots[slotIndex] = 0;
        }
    }
}

/* Allocate and clear resource-task work, load both message resources and prepare the mantra visuals. */
MnuResourceTaskWork *mnuAllocateMantraResourceWork(void) {
    struct SdfMemBlock *allocation = sdfAllocGeneralBlock(sizeof(MnuResourceTaskWork));
    MnuResourceTaskWork *resourceWork = (MnuResourceTaskWork *)sdfMemoryGetBlockAddress(allocation);

    memset(resourceWork, 0, sizeof(MnuResourceTaskWork));
    resourceWork->allocation = allocation;
    resourceWork->messageResource1 = sdfReadNamedResource(D_003AF758,
                                                  &resourceWork->messageResourceInfo1, 0);
    resourceWork->messageResource2 = sdfReadNamedResource(D_003AF780,
                                                  &resourceWork->messageResourceInfo2, 0);
    mnuMarkTitleStreamResetPending();
    mnuResetTitleStreamLocked();
    mnuQueueTitleSoundStreamEntry(0x10);
    mnuTitleStreamUpdateAndLogBgm();
    memset(mnuMantraSpriteSlots, 0, sizeof(mnuMantraSpriteSlots[0]) * MNU_MANTRA_RESOURCE_SLOT_COUNT);
    mnuRequestMantraResources(resourceWork);
    resourceWork->drawPool = mnuCreateSpriteResource(0x3C, 8, 0);
    if (resourceWork->staffMenuContext == NULL) {
        resourceWork->staffMenuContext = mnuCreateWorkBlock();
    }
    return resourceWork;
}

/* Release task-owned slots, visuals and message resources before freeing the backing allocation. */
void mnuReleaseMantraResourceWork(s32 unused, MnuResourceTaskWork *resourceWork) {
    if (resourceWork == NULL) {
        return;
    }
    mnuReleaseResourceSlots(resourceWork);
    mnuDestroyMantraDrawPool(resourceWork->drawPool);
    if (resourceWork->staffMenuContext != NULL) {
        mnuReleaseStaffMenuContextAndResources(resourceWork->staffMenuContext);
    }
    dspCloseChannel();
    sdfQueueGeneralAllocationRelease(resourceWork->messageResource1);
    sdfQueueGeneralAllocationRelease(resourceWork->messageResource2);
    sdfReleaseResourceAllocation(resourceWork->allocation);
}

/* The task handle is shared by the existence probe and explicit stop;
 * both clear it when the resource group is no longer active. */
void mnuCreateResourceTask(void) {
    MnuResourceTaskWork *resourceWork = mnuAllocateMantraResourceWork();
    mnuSceneResourceContext = sdfCreateTaskWorker(mnuResourceTaskName, 0x402, 0x2B12, (SdfTaskItemDesc *)mnuMantraResourceTaskDescriptor, mnuReleaseMantraResourceWork, resourceWork);
}

/* Return whether the named resource task exists; invalidate the cached handle when it does not. */
s32 mnuCheckResourceTask(void) {
    if (kwlnTaskExists(mnuResourceTaskName) != 0) {
        return 1;
    }
    mnuSceneResourceContext = 0;
    return 0;
}

/* Destroy the cached resource-task group and clear its handle. */
void mnuStopResourceTask(void) {
    sdfDestroyTaskWorkerTasks(mnuSceneResourceContext);
    mnuSceneResourceContext = 0;
}

/* Pick the value of the first active slot, preferring slot 0. */
s32 mnuGetMantraSourceValue(u16 profileId) {
    s32 defaultValue = 0;
    PrfDds1RequirementRecord *sourceEntry = prfReqGetEntryRecord(profileId);
    s32 slotIndex = 0;

    if (sourceEntry->slots[0].operation & MNU_MANTRA_SOURCE_ACTIVE_BIT) {
        slotIndex = 0;
    } else if (sourceEntry->slots[1].operation & MNU_MANTRA_SOURCE_ACTIVE_BIT) {
        slotIndex = 1;
    } else {
        return defaultValue;
    }
    return sourceEntry->slots[slotIndex].sourceValue;
}

/* Populate a progress record from a party row's selected profile and its current/cap values. */
void mnuInitializeProfileProgress(u16 partyIndex, MnuProfileProgress *progress) {
    s32 recordOffset = partyIndex * MNU_PARTY_RECORD_BYTES;
    struct DatPartyRecord *partyRecord =
        (struct DatPartyRecord *)(datGameState + recordOffset + MNU_GAME_PARTY_RECORD_OFFSET);

    progress->partyRecord = partyRecord;
    progress->profileId = scrGetSelectedOperandIndex(partyRecord);
    progress->value = ptyGetProfileRecordValue(
        (struct DatPartyRecord *)(datGameState + recordOffset + MNU_GAME_PARTY_RECORD_OFFSET),
        progress->profileId);
    progress->cap = prfGetCapValue(progress->profileId);
}

/* Resource-task -> list -> selection chain used by the mantra display. */
typedef struct MnuResourceTask {
    struct SdfMemBlock *allocation;
    u32 unk04[2];
    struct MenuList *menuList;
} MnuResourceTask;

typedef struct MnuPartyRecord {
    u16 flags;
    u8 pad02[2];
    u16 unitId;
    u8 pad06[0x19E];
} MnuPartyRecord;

extern void mnuDrawMantraPartyListEntry();
extern void *memset(void *, s32, u32);

typedef char MnuResourceTask_size_must_be_0x10[(sizeof(MnuResourceTask) == 0x10) ? 1 : -1];
typedef char MnuResourceTask_menuList_offset_must_be_0x0C[
    ((u32)&((MnuResourceTask *)0)->menuList == 0x0C) ? 1 : -1];

/* Build party selections in unit-ID order from the five present rows.
 * i honestly serves first as a row index, then as the remaining order-table counter. */
void mnuBuildMantraPartyList(MnuResourceTask *task) {
    u16 partyOrder[MNU_PARTY_ORDER_COUNT];
    s32 i = 0;
    u16 *orderCursor;
    struct MenuList *list = mnuCreateListState(0, 6, 0x1A);

    list->drawCallback = mnuDrawMantraPartyListEntry;
    list->context = sdfAllocSizeClassBlock(MNU_LIST_DRAW_VALUE_BYTES);
    memset(list->context, 0, MNU_LIST_DRAW_VALUE_BYTES);
    memset(partyOrder, 0, sizeof(partyOrder));
    do {
        s32 recordOffset = i * sizeof(MnuPartyRecord) + MNU_GAME_PARTY_RECORD_OFFSET;
        MnuPartyRecord *partyRecord = (MnuPartyRecord *)(datGameState + recordOffset);
        u16 active = partyRecord->flags & 1;
        if (active != 0) {
            partyOrder[partyRecord->unitId] = i + 1;
        }
        i++;
    } while (i < MNU_VISIBLE_PARTY_COUNT);
    orderCursor = partyOrder;
    i = MNU_PARTY_ORDER_COUNT - 1;
    do {
        if (*orderCursor != 0) {
            struct MenuListNode *selectionNode = mnuListAppendNode(list, NULL);
            MnuProfileProgress *progress = sdfAllocSizeClassBlock(sizeof(MnuProfileProgress));
            u16 partySlot = *orderCursor;

            selectionNode->userValue = (u32)progress;
            mnuInitializeProfileProgress(partySlot - 1, progress);
        }
        orderCursor++;
    } while (--i >= 0);
    task->menuList = list;
}


/* Return the selection record address used by labels and transition IDs. */
u32 mnuGetSelectedNodeValue(void) {
    MnuResourceTask *task;

    task = (MnuResourceTask *)sdfGetTaskValueByKey(mnuSceneResourceContext, 0);
    return task->menuList->cursor->userValue;
}

/* Clear the list's two animation flags and request its default retreat. */
void mnuStopResourceAnimation(void) {
    s32 taskAddress = sdfGetTaskValueByKey(mnuSceneResourceContext, 0);
    mnuClearListFlagsOneAndTwo(((MnuResourceTask *)taskAddress)->menuList);
    mnuRetreatListCursorDefault((struct MenuList *)((MnuResourceTask *)taskAddress)->menuList);
}

/* Clear the list's two animation flags and request its default advance. */
void mnuResetResourceAnimation(void) {
    s32 taskAddress = sdfGetTaskValueByKey(mnuSceneResourceContext, 0);
    mnuClearListFlagsOneAndTwo(((MnuResourceTask *)taskAddress)->menuList);
    mnuAdvanceListCursorDefault((struct MenuList *)((MnuResourceTask *)taskAddress)->menuList);
}

/* Allocate the four-word list task work, construct its party list and clear both remaining words. */
MnuResourceTask *mnuAllocateEmptyResourceListState(void) {
    struct SdfMemBlock *allocation = sdfAllocGeneralBlock(MNU_RESOURCE_LIST_WORK_BYTES);
    MnuResourceTask *task = (MnuResourceTask *)sdfMemoryGetBlockAddress(allocation);

    memset(task, 0, MNU_RESOURCE_LIST_WORK_BYTES);
    task->allocation = allocation;
    mnuBuildMantraPartyList(task);
    task->unk04[0] = 0;
    task->unk04[1] = 0;
    return task;
}

/* Tear down the linked resource nodes and release the task's allocation. */
void mnuReleaseResourceTaskData(s32 unused, MnuResourceTask *task) {
    struct MenuList *list = task->menuList;
    struct MenuListNode *nodeCursor = list->cursor;
    MenuSceneMetadata *sceneMetadata = (MenuSceneMetadata *)sdfGetTaskValueByKey(mnuSceneResourceContext, SDF_TASK_VALUE_USER_DATA_KEY);

    while (nodeCursor != NULL) {
        sdfReleaseChipBlock((void *)(u32)nodeCursor->userValue);
        nodeCursor = nodeCursor->next;
    }
    sdfReleaseChipBlock(list->context);
    mnuDestroyListState(list);
    mnuReleaseMenuVisualWorkResources(sceneMetadata->attachedEffect);
    sdfReleaseResourceAllocation(task->allocation);
}

INCLUDE_RODATA(const s32, "game/code_0024E1C8", D_003AF758);

INCLUDE_RODATA(const s32, "game/code_0024E1C8", D_003AF780);

INCLUDE_RODATA(const s32, "game/code_0024E1C8", mnuResourceTaskName);

/* Snapshot is one native DatPartyRecord: 0x1A4 bytes with word alignment. */
typedef struct ResourcePartySnapshot { u32 words[0x1A4/4]; } ResourcePartySnapshot;
extern s8 D_00324510[];
extern SdfTaskItemDesc D_0036C65C;
extern s32 mnuTickInitState(MenuProgressHost *);
extern void mnuCommitTitleStreamReadyState(void);
/* The native party snapshot occupies scene metadata +0x28..+0x1CB. */
static inline ResourcePartySnapshot *resourceTaskPartySnapshot(MenuSceneMetadata *metadata) {
    return (ResourcePartySnapshot *)((u8 *)metadata + 0x28);
}
s32 mnuUpdateResourceTaskState(s32 key, MnuResourceTask *task) {
    s32 action=0;
    struct MenuList *list=task->menuList;
    u32 state;
    MenuSceneMetadata *metadata=(MenuSceneMetadata *)sdfGetTaskValueByKey(mnuSceneResourceContext,-1);
    s32 frame;
    ResourcePartySnapshot **selectedParty;
    ResourcePartySnapshot *snapshot;
    state=task->unk04[0];
    switch (state) {
    case 0:
        if (mnuTickInitState(metadata->attachedEffect)==0 && ((s32 (*)())mnuAreResourceSlotsOccupied)(metadata)==1) task->unk04[0]=6;
        return 0;
    case 6:
        selectedParty=(ResourcePartySnapshot **)(u32)list->cursor->userValue;
        task->unk04[0]=1;
        snapshot=resourceTaskPartySnapshot(metadata);
        *snapshot=**selectedParty;
        mnuSetupStaffMenuProfilePage((struct DatPartyRecord *)snapshot,metadata->attachedEffect);
        return 0;
    case 7:
        selectedParty=(ResourcePartySnapshot **)(u32)list->cursor->userValue;
        task->unk04[0]=5;
        snapshot=resourceTaskPartySnapshot(metadata);
        *snapshot=**selectedParty;
        mnuSetupStaffMenuProfilePage((struct DatPartyRecord *)snapshot,metadata->attachedEffect);
        return 0;
    case 1:
    case 5:
        frame=(s32)task->unk04[1]+1;
        task->unk04[1]=frame;
        if ((f32)frame>=10.0f) {task->unk04[1]=0;task->unk04[0]=4;}
        return 0;
    case 2:
        frame=(s32)task->unk04[1]+1;
        task->unk04[1]=frame;
        if ((f32)frame>=10.0f) {
            sdfAttachTaskItem(mnuSceneResourceContext,&D_0036C65C);
            sdfSetTaskItemMode(mnuSceneResourceContext,key,2);
            task->unk04[0]=7;task->unk04[1]=0;
            mnuReleaseMenuVisualWorkResources(metadata->attachedEffect);
        }
        return 0;
    case 3: {
        s32 frames=(s32)task->unk04[1]+1;
        task->unk04[1]=frames;
        if(frames>=10) {task->unk04[0]=8;return 0;}
        break;
    }
    case 8:
        frame=(s32)task->unk04[1]+1;
        task->unk04[1]=frame;
        if (frame>=5) {mnuMarkTitleStreamResetPending();mnuResetTitleStreamLocked();return -1;}
        return 0;
    default:
        if (D_00324510[0x26]&2) {
            if (mnuRetreatListCursorDefault(list)!=NULL) {
                action=1;
                ((s32 (*)())mnuReleaseMenuVisualWorkResources)(metadata->attachedEffect);
                selectedParty=(ResourcePartySnapshot **)(u32)list->cursor->userValue;
                snapshot=resourceTaskPartySnapshot(metadata);
                *snapshot=**selectedParty;
                mnuSetupStaffMenuProfilePage((struct DatPartyRecord *)snapshot,metadata->attachedEffect);
            }
        } else if (D_00324510[0x27]&2) {
            if (mnuAdvanceListCursorDefault(list)!=NULL) {
                action=1;
                ((s32 (*)())mnuReleaseMenuVisualWorkResources)(metadata->attachedEffect);
                selectedParty=(ResourcePartySnapshot **)(u32)list->cursor->userValue;
                snapshot=resourceTaskPartySnapshot(metadata);
                *snapshot=**selectedParty;
                mnuSetupStaffMenuProfilePage((struct DatPartyRecord *)snapshot,metadata->attachedEffect);
            }
        } else if (D_00324510[0x21]<0) {
            action=2; task->unk04[1]=0; task->unk04[0]=2;
        } else if (D_00324510[0x23]<0) {
            mnuCommitTitleStreamReadyState();
            task->unk04[0]=3; task->unk04[1]=0;
            action=3;
        }
        if ((D_00324510[0x26]==0) & (D_00324510[0x27]==0)) mnuClearListFlagsOneAndTwo(list);
        if (action != 0) {
            switch (action) {
            case 1:
                sndSetSequenceVolumePan(0,0x7F,0x3F);
                break;
            case 2:
                sndSetSequenceVolumePan(8,0x7F,0x3F);
                break;
            case 3:
                sndSetSequenceVolumePan(10,0x7F,0x3F);
                break;
            }
        }
        return 0;
    }
    return 0;
}

extern void mnuDrawDisplaySpriteAndPanelMarks(s32, s32, s32);
extern void mnuDrawMantraSceneSpriteSequence(s32, s32);
extern void mnuDrawMantraScenePlacementSprite(s32, s32, s32, s32, s32);
extern void mnuDrawMantraSceneSpriteGroup(s32, s32, s32, s32, s32);
extern void mnuDrawDisplayModeSprites(s32, s32, s32, s32, s32, s32);
extern void itfDspDrawSelectionStrip(s32, s32, s32, s32, s32, s32);
extern void itfDspInitSelectedWindow(s32, s32, s32, s32, s32, s32);
extern void mnuDrawSelectedMantraProfile(s32, s32, s32, s32, s32, s32);

/* Scene draw state: fade phase 1..5 and its frame counter. */
typedef struct MenuFadeWork {
    u8 pad00[4];
    s32 phase;      /* 0x04 */
    s32 timer;      /* 0x08 */
} MenuFadeWork;

/* Draw phase-specific sprite/window alpha and vertical offsets; every path returns zero.
 * Phases 1/5 use the timer ratio, 2/3 its inverse, and 4 full alpha. */
s32 mnuDrawMenuFadePhase(s32 unused, MenuFadeWork *work) {
    f32 shadeFactor;
    f32 elapsedFrames;
    f32 fadeRatio;
    s32 sceneMetadata = sdfGetTaskValueByKey(mnuSceneResourceContext, SDF_TASK_VALUE_USER_DATA_KEY);
    s32 alpha;
    s32 elapsedTicks;

    switch (work->phase) {
    case 1:
        fadeRatio = (f32)work->timer / MNU_SCENE_FADE_FRAMES;
        shadeFactor = fadeRatio + fadeRatio;
        if (shadeFactor > 1.0f) {
            shadeFactor = 1.0f;
        }
        alpha = (s32)(fadeRatio * MNU_SCENE_ALPHA_SCALE);
        mnuDrawDisplaySpriteAndPanelMarks(sceneMetadata, alpha, 0x52);
        mnuDrawMantraSceneSpriteSequence(alpha, 0x52);
        mnuDrawMantraScenePlacementSprite(0, 0, 1, alpha, 0x53);
        mnuDrawMantraSceneSpriteGroup(0, 0, 1, alpha, 0x53);
        mnuDrawDisplayModeSprites(0, 0, 1, alpha, (s32)work, 0x53);
        itfDspDrawSelectionStrip(0, (s32)((1.0f - shadeFactor) * -24.0f), 1, alpha, (s32)work, 0x53);
        if (fadeRatio < 0.5f) {
            shadeFactor = 0.0f;
        } else {
            shadeFactor = (fadeRatio - 0.5f) * 2.0f;
        }
        itfDspInitSelectedWindow(0, 0, 1, (s32)(shadeFactor * MNU_SCENE_ALPHA_SCALE), (s32)work, 0x53);
        mnuDrawSelectedMantraProfile(0, 0, 1, (s32)(fadeRatio * MNU_SCENE_ALPHA_SCALE), (s32)work, 0x53);
        return 0;
    case 5:
        fadeRatio = (f32)work->timer / MNU_SCENE_FADE_FRAMES;
        shadeFactor = fadeRatio + fadeRatio;
        if (shadeFactor > 1.0f) {
            shadeFactor = 1.0f;
        }
        mnuDrawDisplaySpriteAndPanelMarks(sceneMetadata, MNU_SCENE_FULL_ALPHA, 0x52);
        alpha = (s32)(fadeRatio * MNU_SCENE_ALPHA_SCALE);
        mnuDrawMantraSceneSpriteSequence(alpha, 0x52);
        mnuDrawMantraScenePlacementSprite(0, 0, 1, MNU_SCENE_FULL_ALPHA, 0x53);
        mnuDrawMantraSceneSpriteGroup(0, 0, 1, alpha, 0x53);
        mnuDrawDisplayModeSprites(0, 0, 1, alpha, (s32)work, 0x53);
        itfDspDrawSelectionStrip(0, (s32)((1.0f - shadeFactor) * -24.0f), 1, alpha, (s32)work, 0x53);
        if (fadeRatio < 0.5f) {
            shadeFactor = 0.0f;
        } else {
            shadeFactor = (fadeRatio - 0.5f) * 2.0f;
        }
        itfDspInitSelectedWindow(0, 0, 1, (s32)(shadeFactor * MNU_SCENE_ALPHA_SCALE), (s32)work, 0x53);
        mnuDrawSelectedMantraProfile(0, 0, 1, (s32)(fadeRatio * MNU_SCENE_ALPHA_SCALE), (s32)work, 0x53);
        return 0;
    case 2:
        elapsedTicks = work->timer;
        fadeRatio = (f32)elapsedTicks / MNU_SCENE_FADE_FRAMES;
        if (elapsedTicks < 4) {
            shadeFactor = (f32)elapsedTicks * 0.25f;
        } else {
            shadeFactor = 1.0f;
        }
        mnuDrawDisplaySpriteAndPanelMarks(sceneMetadata, MNU_SCENE_FULL_ALPHA, 0x52);
        alpha = (s32)((1.0f - fadeRatio) * MNU_SCENE_ALPHA_SCALE);
        mnuDrawMantraSceneSpriteSequence(alpha, 0x52);
        mnuDrawMantraScenePlacementSprite(0, 0, 1, MNU_SCENE_FULL_ALPHA, 0x53);
        mnuDrawMantraSceneSpriteGroup(0, 0, 1, alpha, 0x53);
        mnuDrawDisplayModeSprites(0, 0, 1, alpha, (s32)work, 0x53);
        itfDspDrawSelectionStrip(0, (s32)(fadeRatio * 36.0f), 1, alpha, (s32)work, 0x53);
        itfDspInitSelectedWindow(0, 0, 1, (s32)((1.0f - shadeFactor) * MNU_SCENE_ALPHA_SCALE), (s32)work, 0x53);
        mnuDrawSelectedMantraProfile(0, 0, 1, alpha, (s32)work, 0x53);
        return 0;
    case 3:
        elapsedTicks = work->timer;
        elapsedFrames = elapsedTicks;
        fadeRatio = elapsedFrames / MNU_SCENE_FADE_FRAMES;
        if (elapsedTicks < 4) {
            shadeFactor = elapsedFrames * 0.25f;
            shadeFactor = 1.0f - shadeFactor;
        } else {
            shadeFactor = 0.0f;
        }
        fadeRatio = 1.0f - fadeRatio;
        alpha = (s32)(fadeRatio * MNU_SCENE_ALPHA_SCALE);
        mnuDrawDisplaySpriteAndPanelMarks(sceneMetadata, alpha, 0x52);
        mnuDrawMantraSceneSpriteSequence(alpha, 0x52);
        mnuDrawMantraScenePlacementSprite(0, 0, 1, alpha, 0x53);
        mnuDrawMantraSceneSpriteGroup(0, 0, 1, alpha, 0x53);
        mnuDrawDisplayModeSprites(0, 0, 1, alpha, (s32)work, 0x53);
        itfDspDrawSelectionStrip(0, (s32)((1.0f - fadeRatio) * 36.0f), 1, alpha, (s32)work, 0x53);
        itfDspInitSelectedWindow(0, 0, 1, (s32)(shadeFactor * MNU_SCENE_ALPHA_SCALE), (s32)work, 0x53);
        mnuDrawSelectedMantraProfile(0, 0, 1, alpha, (s32)work, 0x53);
        return 0;
    case 4:
        mnuDrawDisplaySpriteAndPanelMarks(sceneMetadata, MNU_SCENE_FULL_ALPHA, 0x52);
        mnuDrawMantraSceneSpriteSequence(MNU_SCENE_FULL_ALPHA, 0x52);
        mnuDrawMantraScenePlacementSprite(0, 0, 1, MNU_SCENE_FULL_ALPHA, 0x53);
        mnuDrawMantraSceneSpriteGroup(0, 0, 1, MNU_SCENE_FULL_ALPHA, 0x53);
        mnuDrawDisplayModeSprites(0, 0, 1, MNU_SCENE_FULL_ALPHA, (s32)work, 0x53);
        itfDspDrawSelectionStrip(0, 0, 1, MNU_SCENE_FULL_ALPHA, (s32)work, 0x53);
        itfDspInitSelectedWindow(0, 0, 1, MNU_SCENE_FULL_ALPHA, (s32)work, 0x53);
        mnuDrawSelectedMantraProfile(0, 0, 1, MNU_SCENE_FULL_ALPHA, (s32)work, 0x53);
        break;
    case 0:
    case 6:
    case 7:
        break;
    }
    return 0;
}

/* Summarize two requirement slots: bit 0 marks a qualifying list of at least three IDs;
 * bit 1 reflects the conjunction of the two native record flags. */
s32 mnuGetProfileRequirementFlags(u16 profileId) {
    s32 resultFlags = 0;
    u8 *profileRecord = (u8 *)prfReqGetEntryRecord(profileId);
    s32 requirementWordIndex = 0;
    u8 *slotStatusBase = profileRecord + 0xC;
    s32 slotOffset = 0x20;
    s32 remainingSlots = MNU_REQUIREMENT_SLOT_COUNT - 1;

    do {
        if (*(u32 *)(slotStatusBase + slotOffset) & 1) {
            u8 *requirementsBase = profileRecord + 0x18;
            s32 requirementCount = 0;
            if (requirementsBase[slotOffset] != 0) {
                s32 requirementByteOffset = requirementWordIndex * sizeof(s32);
                u8 *requirementCursor =
                    (u8 *)(requirementByteOffset + (s32)requirementsBase + 0x20);
                do {
                    requirementCursor++;
                    requirementCount++;
                } while (*requirementCursor != 0);
            }
            if (requirementCount >= MNU_REQUIREMENT_MIN_COUNT) {
                resultFlags |= 1;
            }
        }
        remainingSlots--;
        requirementWordIndex += 5;
        slotOffset += 0x14;
    } while (remainingSlots >= 0);
    if ((*(u32 *)(profileRecord + 4) & 4) != 0 && (*(u32 *)(profileRecord + 0x18) & 4) != 0) {
        resultFlags |= 2;
    }
    return resultFlags;
}

/* Return the first status-exactly-one requirement list when summary bit 0 is set, otherwise NULL. */
u8 *mnuGetProfileRequirementIds(u16 profileId) {
    s32 slotIndex = 0;
    PrfDds1RequirementRecord *profileRecord;

    if ((mnuGetProfileRequirementFlags(profileId) & 1) != 0) {
        profileRecord = prfReqGetEntryRecord(profileId);
        while (profileRecord->slots[slotIndex].operation != 1) {
            slotIndex++;
            if (slotIndex >= MNU_REQUIREMENT_SLOT_COUNT) {
                return NULL;
            }
        }
        return profileRecord->slots[slotIndex].profileIds;
    }
    return NULL;
}

/* Return whether the selected party row has capped both required profiles.
 * No qualifying requirement list is treated as satisfied; only its first two IDs are checked. */
s32 mnuAreRequiredProfilesCapped(u16 profileId) {
    MnuResourceTask *task = (MnuResourceTask *)sdfGetTaskValueByKey(mnuSceneResourceContext, 0);
    MnuProfileProgress *progress =
        (MnuProfileProgress *)task->menuList->cursor->userValue;
    u8 *requiredProfiles = mnuGetProfileRequirementIds(profileId);

    if (requiredProfiles == NULL) {
        return 1;
    }
    if (prfGetCapValue(requiredProfiles[0]) ==
        ptyGetProfileRecordValue(progress->partyRecord, requiredProfiles[0])) {
        if (prfGetCapValue(requiredProfiles[1]) ==
            ptyGetProfileRecordValue(progress->partyRecord, requiredProfiles[1])) {
            return 1;
        }
    }
    return 0;
}

extern SdfGrid *sdfGridCreate(s32, s32, s32, s32, s32, s32, void *, s32);
extern void sdfSetShortPairValues(SdfGrid *, s32, s32);
extern void mnuFreeTaskData(u32, u32);
extern void mnuDrawMantraEntryStatus(s32, s32, s32, SdfGrid *, SdfGridCell *, s32);
extern SdfGridCell *sdfGridCursorLeftToFilledCell(SdfGrid *);
extern void mnuBuildMantraGridAndGetProfileCoordinates(MenuSceneWork *, s32, s32 *, s32 *);
extern SdfGridCell *sdfGridSelectFilledCell(SdfGrid *, u32, u32);
extern void mnuUpdateMantraSelectionPositions(MenuSceneWork *, s32);

/* Construct the selection grid and callbacks, select its initial coordinates, then reset the cached scroll position. */
void mnuInitializeMantraSelectionGrid(MenuSceneWork *sceneWork) {
    s32 selectedCoordinates[2];
    s32 resourceTaskAddress;
    s32 fieldAddress;

    sceneWork->gridHandle = sdfGridCreate(0xF, 0x11, 0x40, 0x43, 4, 4,
                                          sceneWork->nodes, 0);
    sdfSetShortPairValues(sceneWork->gridHandle, 1, 1);
    sceneWork->gridHandle->releaseCell = mnuFreeTaskData;
    sceneWork->gridHandle->drawCell = mnuDrawMantraEntryStatus;
    resourceTaskAddress = sdfGetTaskValueByKey(mnuSceneResourceContext, 0);
    fieldAddress = *(s32 *)(*(s32 *)(resourceTaskAddress + 0xC) + 0x1C);
    mnuBuildMantraGridAndGetProfileCoordinates(sceneWork, *(s32 *)(fieldAddress + 0x70),
                  &selectedCoordinates[0], &selectedCoordinates[1]);
    if (sdfGridSelectFilledCell(sceneWork->gridHandle,
                                selectedCoordinates[0], selectedCoordinates[1]) == NULL) {
        sdfGridCursorLeftToFilledCell(sceneWork->gridHandle);
    }
    sceneWork->scrollX = 0;
    sceneWork->scrollY = 0;
    mnuUpdateMantraSelectionPositions(sceneWork, 1);
}
