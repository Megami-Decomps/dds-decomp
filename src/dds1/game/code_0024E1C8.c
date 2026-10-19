#include "mnu_variant_sprite_placement.h"
#include "common.h"
#include "mnu_mantra_sprite_draw.h"
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


extern EffectSlotSet *mnuMantraSpriteSlots[];

typedef s16 MnuSpritePlacement[4];

extern MnuSpritePlacement mnuMantraCommonSpritePlacements[];

extern MnuVariantSpritePlacement mnuMantraVariantSpritePlacements[];

extern u16 mnuMantraCoordinateSpritePlacements[][4];

extern s32 mnuMantraVariantSpriteSlotView[];

extern void effDrawSpriteWithCornerFade(s32, s32, s32, u32, s32, s32, s32, s32);


static inline void mnuDrawPlacementSprite(
        void (*draw)(s32, s32, s32, u32, s32, s32, s32, s32),
        s32 x, s32 y, s32 z, u32 alpha, s32 flags, s32 slot,
        s32 sprite, s32 context) {
    draw(x, y, z, alpha, flags, slot, sprite, context);
}

void mnuDrawSpriteAtPlacement(s32 x, s32 y, s32 z, s32 alpha, s32 sprite, s32 placementIndex,
                  s32 flags, s32 context) {
    mnuDrawPlacementSprite(effDrawSpriteWithCornerFade, (x + mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_X_OFFSET]) << 4,
                         (y + mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_Y_OFFSET]) << 3,
                         z,
                         (u32)((f32)(alpha << 8) * 0.0078125f),
                         flags,
                         sprite,
                         mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_INDEX],
                         context);
}

void mnuDrawPlacedSpriteFromTable(s32 x, s32 y, s32 z, s32 alpha, s32 placementIndex, s32 context) {
    mnuDrawPlacementSprite(effDrawSpriteWithCornerFade, (x + mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_X_OFFSET]) << 4,
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
    mnuDrawPlacementSprite(effDrawSpriteWithCornerFade, (x + mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_X_OFFSET]) << 4,
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
    mnuDrawPlacementSprite(effDrawSpriteWithCornerFade, (x + mnuMantraCommonSpritePlacements[placementIndex][MNU_SPRITE_X_OFFSET]) << 4,
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
                   s32 flags, f32 scaleX, f32 scaleY, s32 context) {
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
                   s32 flags, f32 scaleX, f32 scaleY, s32 context) {
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
                  s32 selector, s32 flags, f32 scaleX, f32 scaleY, s32 context) {
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
                   s32 spriteIndex, s32 flags, f32 scaleX, f32 scaleY, s32 context) {
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
