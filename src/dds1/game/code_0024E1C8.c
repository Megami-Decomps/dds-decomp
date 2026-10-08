#include "common.h"
#include "eff.h"
#include "mnu_list.h"
#include "mnu_profile_progress.h"
#include "prf_requirement.h"

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

extern s32 func_002CB3B8(u32, u32);

extern u8 *datGameState;
extern s8 scrGetSelectedOperandIndex(struct DatPartyRecord *);
extern u32 ptyGetProfileRecordValue(struct DatPartyRecord *, u16);
extern u32 prfGetCapValue(u16);


extern u8 mnuResourceTaskName[];
extern char D_003AF758[];
extern char D_003AF780[];

extern u32 mnuSceneResourceContext;
extern s32 D_0036C698[];
extern u8 D_0036C648[];

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

extern MnuSpritePlacement D_0036B510[];
extern MnuVariantSpritePlacement D_0036B7F0[];
extern u16 D_0036BC68[][4];
extern s32 D_0036C6AC[];
extern void func_002BF4E0(s32, s32, s32, u32, s32, s32, s32, s32);
extern void mnuDestroyMantraDrawPool(void *);
extern void mnuReleaseStaffMenuContextAndResources(MenuProgressHost *);
extern s32 dspCloseChannel(void);
extern void sdfQueueNonzeroResourceId(s32);
extern void sdfReleaseResourceAllocation(s32);
extern s32 sdfAllocGeneralBlock(s32);
extern void *sdfMemoryGetBlockAddress(s32);
extern struct SdfMemBlock *sdfReadNamedResource(const char *name, u32 *outAddress, u32 *outSize);
extern void mnuMarkTitleStreamResetPending(void);
extern void mnuResetTitleStreamLocked(void);
extern void func_0026A5F0(s32);
extern void mnuTitleStreamUpdateAndLogBgm(void);
extern void *mnuCreateSpriteResource(s32, u8, u8);
extern MenuProgressHost *mnuCreateWorkBlock(void);

typedef struct MnuResourceTaskWork {
    s32 allocation;
    u8 pad04[4];
    s32 messageResource1;
    u32 messageResourceInfo1;
    s32 messageResource2;
    u32 messageResourceInfo2;
    u8 pad18[0xC];
    MenuProgressHost *staffMenuContext;
    u8 pad28[0x210];
    void *drawPool;
    u8 pad23C[0xC];
} MnuResourceTaskWork;

void func_0024E1C8(s32 x, s32 y, s32 z, s32 alpha, s32 sprite, s32 placementIndex,
                  s32 flags, s32 context) {
    func_002BF4E0((x + D_0036B510[placementIndex][MNU_SPRITE_X_OFFSET]) << 4,
                         (y + D_0036B510[placementIndex][MNU_SPRITE_Y_OFFSET]) << 3,
                         z,
                         (u32)((f32)(alpha << 8) * 0.0078125f),
                         flags,
                         sprite,
                         D_0036B510[placementIndex][MNU_SPRITE_INDEX],
                         context);
}

void func_0024E260(s32 x, s32 y, s32 z, s32 alpha, s32 placementIndex, s32 context) {
    func_002BF4E0((x + D_0036B510[placementIndex][MNU_SPRITE_X_OFFSET]) << 4,
                         (y + D_0036B510[placementIndex][MNU_SPRITE_Y_OFFSET]) << 3,
                         z,
                         (u32)((f32)(alpha << 8) * 0.0078125f),
                         0,
                         D_0036C698[D_0036B510[placementIndex][MNU_SPRITE_RESOURCE_INDEX]],
                         D_0036B510[placementIndex][MNU_SPRITE_INDEX],
                         context);
}

void func_0024E310(s32 x, s32 y, s32 z, s32 alpha, s32 placementIndex, s32 mode,
                  s32 context) {
    func_002BF4E0((x + D_0036B510[placementIndex][MNU_SPRITE_X_OFFSET]) << 4,
                         (y + D_0036B510[placementIndex][MNU_SPRITE_Y_OFFSET]) << 3,
                         z,
                         (u32)((f32)(alpha << 8) * 0.0078125f),
                         0,
                         D_0036C698[D_0036B510[placementIndex][MNU_SPRITE_RESOURCE_INDEX]],
                         mode,
                         context);
}

void func_0024E3C0(s32 x, s32 y, s32 z, s32 alpha, s32 flags, s32 placementIndex,
                  s32 context) {
    func_002BF4E0((x + D_0036B510[placementIndex][MNU_SPRITE_X_OFFSET]) << 4,
                         (y + D_0036B510[placementIndex][MNU_SPRITE_Y_OFFSET]) << 3,
                         z,
                         (u32)((f32)(alpha << 8) * 0.0078125f),
                         flags,
                         D_0036C698[D_0036B510[placementIndex][MNU_SPRITE_RESOURCE_INDEX]],
                         D_0036B510[placementIndex][MNU_SPRITE_INDEX],
                         context);
}

void func_0024E470(s32 x, s32 y, s32 z, s32 alpha, s32 flags, s32 placementIndex,
                   s32 context, f32 rotation) {
    ((EffectSlotSet *)D_0036C698[
        D_0036B510[placementIndex][MNU_SPRITE_RESOURCE_INDEX]])
        ->workEntries[D_0036B510[placementIndex][MNU_SPRITE_INDEX]].geometry.angleDegrees = rotation;
    func_002BF4E0((x + D_0036B510[placementIndex][MNU_SPRITE_X_OFFSET]) << 4,
                  (y + D_0036B510[placementIndex][MNU_SPRITE_Y_OFFSET]) << 3,
                  z,
                  (u32)((f32)(alpha << 8) * 0.0078125f),
                  flags,
                  D_0036C698[D_0036B510[placementIndex][MNU_SPRITE_RESOURCE_INDEX]],
                  D_0036B510[placementIndex][MNU_SPRITE_INDEX],
                  context);
    ((EffectSlotSet *)D_0036C698[
        D_0036B510[placementIndex][MNU_SPRITE_RESOURCE_INDEX]])
        ->workEntries[D_0036B510[placementIndex][MNU_SPRITE_INDEX]].geometry.angleDegrees = 0.0f;
}

void func_0024E5A0(s32 x, s32 y, s32 z, s32 alpha, s32 placementIndex,
                   s32 context, f32 scaleX, f32 scaleY) {
    {
        EffectSlotSet *resource =
            (EffectSlotSet *)D_0036C698[
                D_0036B510[placementIndex][MNU_SPRITE_RESOURCE_INDEX]];

        resource->workEntries[D_0036B510[placementIndex][MNU_SPRITE_INDEX]].geometry.bounds[2] =
            (s32)(scaleX *
                  (f32)resource
                      ->workEntries[D_0036B510[placementIndex][MNU_SPRITE_INDEX]]
                      .sourceWidth)
            << 4;
        resource->workEntries[D_0036B510[placementIndex][MNU_SPRITE_INDEX]].geometry.bounds[3] =
            (s32)(scaleY *
                  (f32)resource
                      ->workEntries[D_0036B510[placementIndex][MNU_SPRITE_INDEX]]
                      .sourceHeight)
            << 3;
    }
    func_002BF4E0((x + D_0036B510[placementIndex][MNU_SPRITE_X_OFFSET]) << 4,
                  (y + D_0036B510[placementIndex][MNU_SPRITE_Y_OFFSET]) << 3,
                  z,
                  (u32)((f32)(alpha << 8) * 0.0078125f),
                  0,
                  D_0036C698[D_0036B510[placementIndex]
                                         [MNU_SPRITE_RESOURCE_INDEX]],
                  D_0036B510[placementIndex][MNU_SPRITE_INDEX],
                  context);

    {
        /* Reload the resource after drawing before restoring native size. */
        EffectSlotSet *resource =
            (EffectSlotSet *)D_0036C698[
                D_0036B510[placementIndex][MNU_SPRITE_RESOURCE_INDEX]];

        resource->workEntries[D_0036B510[placementIndex][MNU_SPRITE_INDEX]].geometry.bounds[2] =
            resource->workEntries[D_0036B510[placementIndex][MNU_SPRITE_INDEX]]
                .sourceWidth
            << 4;
        resource->workEntries[D_0036B510[placementIndex][MNU_SPRITE_INDEX]].geometry.bounds[3] =
            resource->workEntries[D_0036B510[placementIndex][MNU_SPRITE_INDEX]]
                .sourceHeight
            << 3;
    }
}

void func_0024E728(s32 x, s32 y, s32 z, s32 alpha, s32 flags,
                   s32 placementIndex, s32 context, f32 scaleX, f32 scaleY) {
    {
        EffectSlotSet *resource =
            (EffectSlotSet *)D_0036C698[
                D_0036B510[placementIndex][MNU_SPRITE_RESOURCE_INDEX]];

        resource->workEntries[D_0036B510[placementIndex][MNU_SPRITE_INDEX]].geometry.bounds[2] =
            (s32)(scaleX *
                  (f32)resource->workEntries[D_0036B510[placementIndex]
                                            [MNU_SPRITE_INDEX]]
                      .sourceWidth)
            << 4;
        resource->workEntries[D_0036B510[placementIndex][MNU_SPRITE_INDEX]].geometry.bounds[3] =
            (s32)(scaleY *
                  (f32)resource->workEntries[D_0036B510[placementIndex]
                                            [MNU_SPRITE_INDEX]]
                      .sourceHeight)
            << 3;
    }
    func_002BF4E0((s32)((f32)(x + D_0036B510[placementIndex]
                                           [MNU_SPRITE_X_OFFSET]) *
                        scaleX)
                      << 4,
                  (s32)((f32)(y + D_0036B510[placementIndex]
                                           [MNU_SPRITE_Y_OFFSET]) *
                        scaleY)
                      << 3,
                  z,
                  (u32)((f32)(alpha << 8) * 0.0078125f),
                  flags,
                  D_0036C698[D_0036B510[placementIndex]
                                         [MNU_SPRITE_RESOURCE_INDEX]],
                  D_0036B510[placementIndex][MNU_SPRITE_INDEX],
                  context);

    {
        EffectSlotSet *resource =
            (EffectSlotSet *)D_0036C698[
                D_0036B510[placementIndex][MNU_SPRITE_RESOURCE_INDEX]];

        resource->workEntries[D_0036B510[placementIndex][MNU_SPRITE_INDEX]].geometry.bounds[2] =
            resource->workEntries[D_0036B510[placementIndex][MNU_SPRITE_INDEX]]
                .sourceWidth
            << 4;
        resource->workEntries[D_0036B510[placementIndex][MNU_SPRITE_INDEX]].geometry.bounds[3] =
            resource->workEntries[D_0036B510[placementIndex][MNU_SPRITE_INDEX]]
                .sourceHeight
            << 3;
    }
}

void func_0024E8D0(s32 x, s32 y, s32 z, s32 alpha, s32 flags,
                   s32 placementIndex, s32 context, f32 scaleX, f32 scaleY) {
    {
        EffectSlotSet *resource =
            (EffectSlotSet *)D_0036C698[
                D_0036B510[placementIndex][MNU_SPRITE_RESOURCE_INDEX]];

        resource->workEntries[D_0036B510[placementIndex][MNU_SPRITE_INDEX]]
            .geometry.bounds[2] = (s32)(scaleX *
                           (f32)resource->workEntries[D_0036B510[placementIndex]
                                                      [MNU_SPRITE_INDEX]]
                               .sourceWidth)
                    << 4;
        resource->workEntries[D_0036B510[placementIndex][MNU_SPRITE_INDEX]]
            .geometry.bounds[3] = (s32)(scaleY *
                            (f32)resource->workEntries[D_0036B510[placementIndex]
                                                       [MNU_SPRITE_INDEX]]
                                .sourceHeight)
                     << 3;
    }
    func_002BF4E0((x + D_0036B510[placementIndex][MNU_SPRITE_X_OFFSET]) << 4,
                  (y + D_0036B510[placementIndex][MNU_SPRITE_Y_OFFSET]) << 3,
                  z,
                  (u32)((f32)(alpha << 8) * 0.0078125f),
                  flags,
                  D_0036C698[D_0036B510[placementIndex]
                                         [MNU_SPRITE_RESOURCE_INDEX]],
                  D_0036B510[placementIndex][MNU_SPRITE_INDEX],
                  context);

    {
        EffectSlotSet *resource =
            (EffectSlotSet *)D_0036C698[
                D_0036B510[placementIndex][MNU_SPRITE_RESOURCE_INDEX]];

        resource->workEntries[D_0036B510[placementIndex][MNU_SPRITE_INDEX]]
            .geometry.bounds[2] =
            resource->workEntries[D_0036B510[placementIndex][MNU_SPRITE_INDEX]]
                .sourceWidth
            << 4;
        resource->workEntries[D_0036B510[placementIndex][MNU_SPRITE_INDEX]]
            .geometry.bounds[3] =
            resource->workEntries[D_0036B510[placementIndex][MNU_SPRITE_INDEX]]
                .sourceHeight
            << 3;
    }
}

void mnuDrawScaledVariantSprite(s32 x, s32 y, s32 z, s32 alpha, s32 placementIndex,
                   s32 flags, f32 scaleX, f32 scaleY, s32 context) {
    {
        EffectSlotSet *resource = (EffectSlotSet *)D_0036C698[
            D_0036B7F0[placementIndex][MNU_SPRITE_RESOURCE_INDEX]];

        resource->workEntries[D_0036B7F0[placementIndex][MNU_SPRITE_INDEX]].geometry.bounds[2] =
            (s32)(scaleX * (f32)resource->workEntries[D_0036B7F0[placementIndex][MNU_SPRITE_INDEX]].sourceWidth) << 4;
        resource->workEntries[D_0036B7F0[placementIndex][MNU_SPRITE_INDEX]].geometry.bounds[3] =
            (s32)(scaleY * (f32)resource->workEntries[D_0036B7F0[placementIndex][MNU_SPRITE_INDEX]].sourceHeight) << 3;
    }
    func_002BF4E0((s32)((f32)(x + D_0036B7F0[placementIndex][MNU_SPRITE_X_OFFSET]) * scaleX) << 4,
                  (s32)((f32)(y + D_0036B7F0[placementIndex][MNU_SPRITE_Y_OFFSET]) * scaleY) << 3,
                  z,
                  (u32)((f32)(alpha << 8) * 0.0078125f),
                  flags,
                  D_0036C698[D_0036B7F0[placementIndex][MNU_SPRITE_RESOURCE_INDEX]],
                  D_0036B7F0[placementIndex][MNU_SPRITE_INDEX],
                  context);
    {
        EffectSlotSet *resource = (EffectSlotSet *)D_0036C698[
            D_0036B7F0[placementIndex][MNU_SPRITE_RESOURCE_INDEX]];

        resource->workEntries[D_0036B7F0[placementIndex][MNU_SPRITE_INDEX]].geometry.bounds[2] =
            resource->workEntries[D_0036B7F0[placementIndex][MNU_SPRITE_INDEX]].sourceWidth << 4;
        resource->workEntries[D_0036B7F0[placementIndex][MNU_SPRITE_INDEX]].geometry.bounds[3] =
            resource->workEntries[D_0036B7F0[placementIndex][MNU_SPRITE_INDEX]].sourceHeight << 3;
    }
}

void func_0024EC08(s32 x, s32 y, s32 z, s32 alpha, s32 placementIndex,
                   s32 flags, f32 scaleX, f32 scaleY, s32 context) {
    {
        EffectSlotSet *resource = (EffectSlotSet *)D_0036C698[
            D_0036B7F0[placementIndex][MNU_SPRITE_RESOURCE_INDEX]];

        resource->workEntries[D_0036B7F0[placementIndex][MNU_SPRITE_INDEX]].geometry.bounds[2] =
            (s32)(scaleX * (f32)resource->workEntries[D_0036B7F0[placementIndex][MNU_SPRITE_INDEX]].sourceWidth) << 4;
        resource->workEntries[D_0036B7F0[placementIndex][MNU_SPRITE_INDEX]].geometry.bounds[3] =
            (s32)(scaleY * (f32)resource->workEntries[D_0036B7F0[placementIndex][MNU_SPRITE_INDEX]].sourceHeight) << 3;
    }
    func_002BF4E0((s32)((f32)(x + D_0036B7F0[placementIndex][MNU_SPRITE_X_OFFSET]) * scaleX) << 4,
                  (s32)((f32)(y + D_0036B7F0[placementIndex][MNU_SPRITE_Y_OFFSET]) * scaleY) << 3,
                  z,
                  (u32)((f32)(alpha << 8) * 0.0078125f),
                  flags,
                  D_0036C698[D_0036B7F0[placementIndex][MNU_SPRITE_RESOURCE_INDEX]],
                  D_0036B7F0[placementIndex][MNU_SPRITE_INDEX],
                  context);
    {
        EffectSlotSet *resource = (EffectSlotSet *)D_0036C698[
            D_0036B7F0[placementIndex][MNU_SPRITE_RESOURCE_INDEX]];

        resource->workEntries[D_0036B7F0[placementIndex][MNU_SPRITE_INDEX]].geometry.bounds[2] =
            resource->workEntries[D_0036B7F0[placementIndex][MNU_SPRITE_INDEX]].sourceWidth << 4;
        resource->workEntries[D_0036B7F0[placementIndex][MNU_SPRITE_INDEX]].geometry.bounds[3] =
            resource->workEntries[D_0036B7F0[placementIndex][MNU_SPRITE_INDEX]].sourceHeight << 3;
    }
}

void func_0024EDC0(s32 x, s32 y, s32 z, s32 alpha, s32 placementIndex,
                   s32 flags, s32 context, f32 scaleX, f32 scaleY) {
    {
        EffectSlotSet *resource =
            (EffectSlotSet *)D_0036C698[
                D_0036BC68[placementIndex][MNU_SPRITE_RESOURCE_INDEX]];

        resource->workEntries[D_0036BC68[placementIndex][MNU_SPRITE_INDEX]].geometry.bounds[2] =
            (s32)(scaleX *
                  (f32)resource->workEntries[D_0036BC68[placementIndex]
                                            [MNU_SPRITE_INDEX]]
                      .sourceWidth)
            << 4;
        resource->workEntries[D_0036BC68[placementIndex][MNU_SPRITE_INDEX]].geometry.bounds[3] =
            (s32)(scaleY *
                  (f32)resource->workEntries[D_0036BC68[placementIndex]
                                            [MNU_SPRITE_INDEX]]
                      .sourceHeight)
            << 3;
    }
    func_002BF4E0((s32)((f32)(x + D_0036BC68[placementIndex]
                                           [MNU_SPRITE_X_OFFSET]) *
                        scaleX)
                      << 4,
                  (s32)((f32)(y + D_0036BC68[placementIndex]
                                           [MNU_SPRITE_Y_OFFSET]) *
                        scaleY)
                      << 3,
                  z,
                  (u32)((f32)(alpha << 8) * 0.0078125f),
                  flags,
                  D_0036C698[D_0036BC68[placementIndex]
                                         [MNU_SPRITE_RESOURCE_INDEX]],
                  D_0036BC68[placementIndex][MNU_SPRITE_INDEX],
                  context);

    {
        /* Reload the resource after drawing before restoring native size. */
        EffectSlotSet *resource =
            (EffectSlotSet *)D_0036C698[
                D_0036BC68[placementIndex][MNU_SPRITE_RESOURCE_INDEX]];

        resource->workEntries[D_0036BC68[placementIndex][MNU_SPRITE_INDEX]].geometry.bounds[2] =
            resource->workEntries[D_0036BC68[placementIndex][MNU_SPRITE_INDEX]]
                .sourceWidth
            << 4;
        resource->workEntries[D_0036BC68[placementIndex][MNU_SPRITE_INDEX]].geometry.bounds[3] =
            resource->workEntries[D_0036BC68[placementIndex][MNU_SPRITE_INDEX]]
                .sourceHeight
            << 3;
    }
}

void func_0024EF68(s32 x, s32 y, s32 z, s32 alpha, s32 placementIndex,
                   s32 flags, s32 context, f32 scaleX, f32 scaleY) {
    {
        EffectSlotSet *resource = (EffectSlotSet *)D_0036C698[
            D_0036BC68[placementIndex][MNU_SPRITE_RESOURCE_INDEX]];

        resource->workEntries[6].geometry.bounds[2] =
            (s32)(scaleX * (f32)resource->workEntries[6].sourceWidth) << 4;
        resource->workEntries[6].geometry.bounds[3] =
            (s32)(scaleY * (f32)resource->workEntries[6].sourceHeight) << 3;
    }
    func_002BF4E0(
        (s32)((f32)(x + D_0036BC68[placementIndex][MNU_SPRITE_X_OFFSET]) *
              scaleX) << 4,
        (s32)((f32)(y + D_0036BC68[placementIndex][MNU_SPRITE_Y_OFFSET]) *
              scaleY) << 3,
        z, (u32)((f32)(alpha << 8) * 0.0078125f), flags,
        D_0036C698[D_0036BC68[placementIndex][MNU_SPRITE_RESOURCE_INDEX]],
        6, context);
    {
        /* Reload the resource after drawing before restoring native size. */
        EffectSlotSet *resource = (EffectSlotSet *)D_0036C698[
            D_0036BC68[placementIndex][MNU_SPRITE_RESOURCE_INDEX]];

        resource->workEntries[6].geometry.bounds[2] =
            resource->workEntries[6].sourceWidth << 4;
        resource->workEntries[6].geometry.bounds[3] =
            resource->workEntries[6].sourceHeight << 3;
    }
}

void func_0024F0D0(s32 x, s32 y, s32 z, s32 alpha, s32 placementIndex,
                  s32 selector, s32 flags, s32 context,
                  f32 scaleX, f32 scaleY) {
    s8 spriteMap[15] = { -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 17, 18 };
    s32 sprite;

    sprite = spriteMap[(D_0036B7F0[placementIndex][MNU_VARIANT_SPRITE_GROUPS] >>
                        (selector * 4)) & 0xF];
    if (sprite == -1) {
        return;
    }
    func_002BF4E0((s32)((f32)(x + D_0036B7F0[placementIndex]
                                           [MNU_VARIANT_X_OFFSET]) * scaleX) << 4,
                  (s32)((f32)(y + D_0036B7F0[placementIndex]
                                           [MNU_VARIANT_Y_OFFSET]) * scaleY) << 3,
                  z,
                  (u32)((f32)(alpha << 8) * 0.0078125f),
                  flags, D_0036C6AC[0], sprite, context);
}


void func_0024F210(s32 x, s32 y, s32 z, s32 alpha, s32 groupPlacementIndex,
                   s32 coordinatePlacementIndex, s32 selector, s32 flags,
                   f32 scaleX, f32 scaleY, s32 context) {
    s8 spriteMap[12] = { -1, 12, 26, 27, 13, 14, 15, 16, 20, 21, 23, 24 };
    s32 sprite;

    selector += 2;
    sprite = spriteMap[(D_0036B7F0[groupPlacementIndex][MNU_VARIANT_SPRITE_GROUPS] >>
                        (selector * 4)) & 0xF];
    if (sprite == -1) {
        return;
    }
    func_002BF4E0((s32)((f32)(x + D_0036B7F0[coordinatePlacementIndex]
                                              [MNU_VARIANT_X_OFFSET]) * scaleX) << 4,
                  (s32)((f32)(y + D_0036B7F0[coordinatePlacementIndex]
                                              [MNU_VARIANT_Y_OFFSET]) * scaleY) << 3,
                  z,
                  (u32)((f32)(alpha << 8) * 0.0078125f),
                  flags,
                  D_0036C6AC[0],
                  sprite,
                  context);
}

void func_0024F338(s32 x, s32 y, s32 z, s32 alpha, s32 placementIndex,
                   s32 spriteIndex, s32 flags, s32 context, f32 scaleX,
                   f32 scaleY) {
    {
        EffectSlotSet *resource =
            (EffectSlotSet *)D_0036C698[D_0036B7F0[placementIndex][0]];

        resource->workEntries[spriteIndex].geometry.bounds[2] =
            (s32)(scaleX * (f32)resource->workEntries[spriteIndex].sourceWidth) << 4;
        resource->workEntries[spriteIndex].geometry.bounds[3] =
            (s32)(scaleY * (f32)resource->workEntries[spriteIndex].sourceHeight) << 3;
    }
    func_002BF4E0(
        (s32)((f32)(x + D_0036B7F0[placementIndex][MNU_VARIANT_X_OFFSET]) * scaleX) << 4,
        (s32)((f32)(y + D_0036B7F0[placementIndex][MNU_VARIANT_Y_OFFSET]) * scaleY) << 3,
        z, (u32)((f32)(alpha << 8) * 0.0078125f), flags,
        D_0036C698[5], spriteIndex, context);
    {
        EffectSlotSet *resource =
            (EffectSlotSet *)D_0036C698[D_0036B7F0[placementIndex][0]];

        resource->workEntries[spriteIndex].geometry.bounds[2] =
            resource->workEntries[spriteIndex].sourceWidth << 4;
        resource->workEntries[spriteIndex].geometry.bounds[3] =
            resource->workEntries[spriteIndex].sourceHeight << 3;
    }
}

extern u8 D_0036C568[];
extern void effRequestResourceByMode(char *, void *, s32, void *);

void mnuRequestMantraResources(MnuResourceTaskWork *work) {
    s32 i;

    for (i = 0; i < 14; i++) {
        if (D_0036C698[i] == 0) {
            effRequestResourceByMode("/facility/spr/mantra/", &D_0036C568[i * 0x10], 0, &D_0036C698[i]);
        }
    }
}

/* Return one only when every mantra sprite resource slot is occupied. */
s32 mnuAreResourceSlotsOccupied(void) {
    s32 slotIndex;
    for (slotIndex = 0; slotIndex < MNU_MANTRA_RESOURCE_SLOT_COUNT; ++slotIndex) {
        if (D_0036C698[slotIndex] == 0) {
            return 0;
        }
    }
    return 1;
}

/* Release nonzero mantra sprite slots and clear their stored handles; the work argument is unused. */
void mnuReleaseResourceSlots(MnuResourceTaskWork *unusedWork) {
    s32 slotIndex;
    for (slotIndex = 0; slotIndex < MNU_MANTRA_RESOURCE_SLOT_COUNT; ++slotIndex) {
        if (D_0036C698[slotIndex] != 0) {
            effDestroyResourceSlotSet(D_0036C698[slotIndex]);
            D_0036C698[slotIndex] = 0;
        }
    }
}

/* Allocate and clear resource-task work, load both message resources and prepare the mantra visuals. */
MnuResourceTaskWork *func_0024F608(void) {
    s32 allocationHandle = sdfAllocGeneralBlock(sizeof(MnuResourceTaskWork));
    MnuResourceTaskWork *resourceWork = sdfMemoryGetBlockAddress(allocationHandle);

    memset(resourceWork, 0, sizeof(MnuResourceTaskWork));
    resourceWork->allocation = allocationHandle;
    resourceWork->messageResource1 = (s32)(u32)sdfReadNamedResource(D_003AF758,
                                                  &resourceWork->messageResourceInfo1, 0);
    resourceWork->messageResource2 = (s32)(u32)sdfReadNamedResource(D_003AF780,
                                                  &resourceWork->messageResourceInfo2, 0);
    mnuMarkTitleStreamResetPending();
    mnuResetTitleStreamLocked();
    func_0026A5F0(0x10);
    mnuTitleStreamUpdateAndLogBgm();
    memset(D_0036C698, 0, sizeof(s32) * MNU_MANTRA_RESOURCE_SLOT_COUNT);
    mnuRequestMantraResources(resourceWork);
    resourceWork->drawPool = mnuCreateSpriteResource(0x3C, 8, 0);
    if (resourceWork->staffMenuContext == NULL) {
        resourceWork->staffMenuContext = mnuCreateWorkBlock();
    }
    return resourceWork;
}

/* Release task-owned slots, visuals and message resources before freeing the backing allocation. */
void func_0024F6F0(s32 unused, MnuResourceTaskWork *resourceWork) {
    if (resourceWork == NULL) {
        return;
    }
    mnuReleaseResourceSlots(resourceWork);
    mnuDestroyMantraDrawPool(resourceWork->drawPool);
    if (resourceWork->staffMenuContext != NULL) {
        mnuReleaseStaffMenuContextAndResources(resourceWork->staffMenuContext);
    }
    dspCloseChannel();
    sdfQueueNonzeroResourceId(resourceWork->messageResource1);
    sdfQueueNonzeroResourceId(resourceWork->messageResource2);
    sdfReleaseResourceAllocation(resourceWork->allocation);
}

/* The task handle is shared by the existence probe and explicit stop;
 * both clear it when the resource group is no longer active. */
void mnuCreateResourceTask(void) {
    MnuResourceTaskWork *resourceWork = func_0024F608();
    mnuSceneResourceContext = sdfCreateTaskWorker(mnuResourceTaskName, 0x402, 0x2B12, D_0036C648, func_0024F6F0, resourceWork);
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

    if (sourceEntry->slots[0].status & MNU_MANTRA_SOURCE_ACTIVE_BIT) {
        slotIndex = 0;
    } else if (sourceEntry->slots[1].status & MNU_MANTRA_SOURCE_ACTIVE_BIT) {
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
typedef struct MnuResourceSelectionNode {
    u8 pad00[0x70];
    u32 selectionAddress;
} MnuResourceSelectionNode;

typedef struct MnuResourceList {
    u8 pad00[0x1C];
    MnuResourceSelectionNode *selectionNode;
    u8 pad20[0xC];
    void (*drawCallback)();
    s32 *drawValues;
} MnuResourceList;

typedef struct MnuResourceTask {
    u8 pad00[0xC];
    MnuResourceList *menuList;
} MnuResourceTask;

typedef struct MnuPartyRecord {
    u16 flags;
    u8 pad02[2];
    u16 unitId;
    u8 pad06[0x19E];
} MnuPartyRecord;

extern MnuResourceList *mnuCreateListState(s32, s32, s32);
extern void *sdfAllocSizeClassBlock(s32);
extern void func_00254C68();
extern void *memset(void *, s32, u32);

/* Build party selections in unit-ID order from the five present rows.
 * i honestly serves first as a row index, then as the remaining order-table counter. */
void mnuBuildMantraPartyList(MnuResourceTask *task) {
    u16 partyOrder[MNU_PARTY_ORDER_COUNT];
    s32 i = 0;
    u16 *orderCursor;
    MnuResourceList *list = mnuCreateListState(0, 6, 0x1A);

    list->drawCallback = func_00254C68;
    list->drawValues = sdfAllocSizeClassBlock(MNU_LIST_DRAW_VALUE_BYTES);
    memset(list->drawValues, 0, MNU_LIST_DRAW_VALUE_BYTES);
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
            MnuResourceSelectionNode *selectionNode =
                (MnuResourceSelectionNode *)mnuListAppendNode((struct MenuList *)list, NULL);
            MnuProfileProgress *progress = sdfAllocSizeClassBlock(sizeof(MnuProfileProgress));
            selectionNode->selectionAddress = (u32)progress;
            mnuInitializeProfileProgress(*orderCursor - 1, progress);
        }
        orderCursor++;
    } while (--i >= 0);
    task->menuList = list;
}


/* Return the selection record address used by labels and transition IDs. */
u32 mnuGetSelectedNodeValue(void) {
    MnuResourceTask *task;

    task = (MnuResourceTask *)func_002CB3B8(mnuSceneResourceContext, 0);
    return task->menuList->selectionNode->selectionAddress;
}

/* Clear the list's two animation flags and request its default retreat. */
void mnuStopResourceAnimation(void) {
    s32 taskAddress = func_002CB3B8(mnuSceneResourceContext, 0);
    mnuClearListFlagsOneAndTwo(((MnuResourceTask *)taskAddress)->menuList);
    mnuRetreatListCursorDefault((struct MenuList *)((MnuResourceTask *)taskAddress)->menuList);
}

/* Clear the list's two animation flags and request its default advance. */
void mnuResetResourceAnimation(void) {
    s32 taskAddress = func_002CB3B8(mnuSceneResourceContext, 0);
    mnuClearListFlagsOneAndTwo(((MnuResourceTask *)taskAddress)->menuList);
    mnuAdvanceListCursorDefault((struct MenuList *)((MnuResourceTask *)taskAddress)->menuList);
}

/* Allocate the four-word list task work, construct its party list and clear both remaining words. */
u32 *mnuAllocateEmptyResourceListState(void) {
    s32 allocationHandle = sdfAllocGeneralBlock(MNU_RESOURCE_LIST_WORK_BYTES);
    u32 *taskWords = sdfMemoryGetBlockAddress(allocationHandle);

    memset(taskWords, 0, MNU_RESOURCE_LIST_WORK_BYTES);
    taskWords[0] = allocationHandle;
    mnuBuildMantraPartyList((MnuResourceTask *)taskWords);
    taskWords[1] = 0;
    taskWords[2] = 0;
    return taskWords;
}

extern void sdfReleaseChipBlock(void *);
extern void mnuDestroyListState(void *);
extern void mnuReleaseMenuVisualWorkResources(s32);

typedef struct MenuCleanupNode {
    u8 pad00[0x58];
    struct MenuCleanupNode *next;
    u8 pad5C[0x14];
    void *resource;
} MenuCleanupNode;

typedef struct MenuCleanupOwner {
    u8 pad00[0x1C];
    MenuCleanupNode *first;
    u8 pad20[0x10];
    void *resource;
} MenuCleanupOwner;

/* Tear down the linked resource nodes and release the task's allocation. */
void mnuReleaseResourceTaskData(s32 unused, s32 *taskData) {
    MenuCleanupOwner *listOwner = (MenuCleanupOwner *)taskData[3];
    MenuCleanupNode *nodeCursor = listOwner->first;
    u8 *sceneMetadata = (u8 *)func_002CB3B8(mnuSceneResourceContext, -1);

    while (nodeCursor != NULL) {
        sdfReleaseChipBlock(nodeCursor->resource);
        nodeCursor = nodeCursor->next;
    }
    sdfReleaseChipBlock(listOwner->resource);
    mnuDestroyListState(listOwner);
    mnuReleaseMenuVisualWorkResources(*(s32 *)(sceneMetadata + 0x24));
    sdfReleaseResourceAllocation(taskData[0]);
}

INCLUDE_RODATA(const s32, "game/code_0024E1C8", D_003AF758);

INCLUDE_RODATA(const s32, "game/code_0024E1C8", D_003AF780);

INCLUDE_RODATA(const s32, "game/code_0024E1C8", mnuResourceTaskName);

INCLUDE_ASM(const s32, "game/code_0024E1C8", func_0024FBB8);

extern void mnuDrawDisplaySpriteAndPanelMarks(s32, s32, s32);
extern void func_002546D8(s32, s32);
extern void func_00254758(s32, s32, s32, s32, s32);
extern void func_00254778(s32, s32, s32, s32, s32);
extern void mnuDrawDisplayModeSprites(s32, s32, s32, s32, s32, s32);
extern void itfDspDrawSelectionStrip(s32, s32, s32, s32, s32, s32);
extern void itfDspInitSelectedWindow(s32, s32, s32, s32, s32, s32);
extern void func_00254810(s32, s32, s32, s32, s32, s32);

/* Scene draw state: fade phase 1..5 and its frame counter. */
typedef struct MenuFadeWork {
    u8 pad00[4];
    s32 phase;      /* 0x04 */
    s32 timer;      /* 0x08 */
} MenuFadeWork;

/* Draw phase-specific sprite/window alpha and vertical offsets; every path returns zero.
 * Phases 1/5 use the timer ratio, 2/3 its inverse, and 4 full alpha. */
s32 func_002501E0(s32 unused, MenuFadeWork *work) {
    f32 shadeFactor;
    f32 elapsedFrames;
    f32 fadeRatio;
    s32 sceneMetadata = func_002CB3B8(mnuSceneResourceContext, -1);
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
        func_002546D8(alpha, 0x52);
        func_00254758(0, 0, 1, alpha, 0x53);
        func_00254778(0, 0, 1, alpha, 0x53);
        mnuDrawDisplayModeSprites(0, 0, 1, alpha, (s32)work, 0x53);
        itfDspDrawSelectionStrip(0, (s32)((1.0f - shadeFactor) * -24.0f), 1, alpha, (s32)work, 0x53);
        if (fadeRatio < 0.5f) {
            shadeFactor = 0.0f;
        } else {
            shadeFactor = (fadeRatio - 0.5f) * 2.0f;
        }
        itfDspInitSelectedWindow(0, 0, 1, (s32)(shadeFactor * MNU_SCENE_ALPHA_SCALE), (s32)work, 0x53);
        func_00254810(0, 0, 1, (s32)(fadeRatio * MNU_SCENE_ALPHA_SCALE), (s32)work, 0x53);
        return 0;
    case 5:
        fadeRatio = (f32)work->timer / MNU_SCENE_FADE_FRAMES;
        shadeFactor = fadeRatio + fadeRatio;
        if (shadeFactor > 1.0f) {
            shadeFactor = 1.0f;
        }
        mnuDrawDisplaySpriteAndPanelMarks(sceneMetadata, MNU_SCENE_FULL_ALPHA, 0x52);
        alpha = (s32)(fadeRatio * MNU_SCENE_ALPHA_SCALE);
        func_002546D8(alpha, 0x52);
        func_00254758(0, 0, 1, MNU_SCENE_FULL_ALPHA, 0x53);
        func_00254778(0, 0, 1, alpha, 0x53);
        mnuDrawDisplayModeSprites(0, 0, 1, alpha, (s32)work, 0x53);
        itfDspDrawSelectionStrip(0, (s32)((1.0f - shadeFactor) * -24.0f), 1, alpha, (s32)work, 0x53);
        if (fadeRatio < 0.5f) {
            shadeFactor = 0.0f;
        } else {
            shadeFactor = (fadeRatio - 0.5f) * 2.0f;
        }
        itfDspInitSelectedWindow(0, 0, 1, (s32)(shadeFactor * MNU_SCENE_ALPHA_SCALE), (s32)work, 0x53);
        func_00254810(0, 0, 1, (s32)(fadeRatio * MNU_SCENE_ALPHA_SCALE), (s32)work, 0x53);
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
        func_002546D8(alpha, 0x52);
        func_00254758(0, 0, 1, MNU_SCENE_FULL_ALPHA, 0x53);
        func_00254778(0, 0, 1, alpha, 0x53);
        mnuDrawDisplayModeSprites(0, 0, 1, alpha, (s32)work, 0x53);
        itfDspDrawSelectionStrip(0, (s32)(fadeRatio * 36.0f), 1, alpha, (s32)work, 0x53);
        itfDspInitSelectedWindow(0, 0, 1, (s32)((1.0f - shadeFactor) * MNU_SCENE_ALPHA_SCALE), (s32)work, 0x53);
        func_00254810(0, 0, 1, alpha, (s32)work, 0x53);
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
        func_002546D8(alpha, 0x52);
        func_00254758(0, 0, 1, alpha, 0x53);
        func_00254778(0, 0, 1, alpha, 0x53);
        mnuDrawDisplayModeSprites(0, 0, 1, alpha, (s32)work, 0x53);
        itfDspDrawSelectionStrip(0, (s32)((1.0f - fadeRatio) * 36.0f), 1, alpha, (s32)work, 0x53);
        itfDspInitSelectedWindow(0, 0, 1, (s32)(shadeFactor * MNU_SCENE_ALPHA_SCALE), (s32)work, 0x53);
        func_00254810(0, 0, 1, alpha, (s32)work, 0x53);
        return 0;
    case 4:
        mnuDrawDisplaySpriteAndPanelMarks(sceneMetadata, MNU_SCENE_FULL_ALPHA, 0x52);
        func_002546D8(MNU_SCENE_FULL_ALPHA, 0x52);
        func_00254758(0, 0, 1, MNU_SCENE_FULL_ALPHA, 0x53);
        func_00254778(0, 0, 1, MNU_SCENE_FULL_ALPHA, 0x53);
        mnuDrawDisplayModeSprites(0, 0, 1, MNU_SCENE_FULL_ALPHA, (s32)work, 0x53);
        itfDspDrawSelectionStrip(0, 0, 1, MNU_SCENE_FULL_ALPHA, (s32)work, 0x53);
        itfDspInitSelectedWindow(0, 0, 1, MNU_SCENE_FULL_ALPHA, (s32)work, 0x53);
        func_00254810(0, 0, 1, MNU_SCENE_FULL_ALPHA, (s32)work, 0x53);
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
s32 func_00250758(u16 profileId) {
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
u8 *func_00250820(u16 profileId) {
    s32 slotIndex = 0;
    PrfDds1RequirementRecord *profileRecord;

    if ((func_00250758(profileId) & 1) != 0) {
        profileRecord = prfReqGetEntryRecord(profileId);
        while (profileRecord->slots[slotIndex].status != 1) {
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
s32 func_002508D8(u16 profileId) {
    MnuResourceTask *task = (MnuResourceTask *)func_002CB3B8(mnuSceneResourceContext, 0);
    MnuProfileProgress *progress =
        (MnuProfileProgress *)task->menuList->selectionNode->selectionAddress;
    u8 *requiredProfiles = func_00250820(profileId);

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

typedef struct MnuSceneGridWork {
    u8 pad00[0x18];
    void (*callback)(void);
    void (*freeTaskData)(s32, void *);
} MnuSceneGridWork;

typedef struct MnuSceneContext {
    u8 pad00[0x484];
    MnuSceneGridWork *grid;
    u8 pad488[0x11C];
    u16 cursorX;
    u16 cursorY;
} MnuSceneContext;

extern MnuSceneGridWork *func_002CB9C0(s32, s32, s32, s32, s32, s32, void *, s32);
extern void sdfSetShortPairValues(MnuSceneGridWork *, s32, s32);
extern void mnuFreeTaskData(s32, void *);
extern void mnuDrawMantraEntryStatus(void);
extern void func_002CC0D0(MnuSceneGridWork *);
extern void func_00253208(s32, s32, s32 *, s32 *);
extern void *sdfGridSelectFilledCell(MnuSceneGridWork *, s32, s32);
extern void func_002512F0(s32, s32);

/* Construct the selection grid and callbacks, select its initial coordinates, then reset cached cursor coordinates. */
void mnuInitializeMantraSelectionGrid(s32 sceneAddress) {
    MnuSceneContext *sceneWork = (MnuSceneContext *)sceneAddress;
    s32 selectedCoordinates[2];
    s32 resourceTaskAddress;
    s32 fieldAddress;

    sceneWork->grid = func_002CB9C0(0xF, 0x11, 0x40, 0x43, 4, 4,
                                (u8 *)sceneWork + 4, 0);
    sdfSetShortPairValues(sceneWork->grid, 1, 1);
    sceneWork->grid->freeTaskData = mnuFreeTaskData;
    sceneWork->grid->callback = mnuDrawMantraEntryStatus;
    resourceTaskAddress = func_002CB3B8(mnuSceneResourceContext, 0);
    fieldAddress = *(s32 *)(*(s32 *)(resourceTaskAddress + 0xC) + 0x1C);
    func_00253208(sceneAddress, *(s32 *)(fieldAddress + 0x70),
                  &selectedCoordinates[0], &selectedCoordinates[1]);
    if (sdfGridSelectFilledCell(sceneWork->grid, selectedCoordinates[0], selectedCoordinates[1]) == NULL) {
        func_002CC0D0(sceneWork->grid);
    }
    sceneWork->cursorX = 0;
    sceneWork->cursorY = 0;
    func_002512F0(sceneAddress, 1);
}
