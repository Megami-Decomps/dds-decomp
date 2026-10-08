#include "common.h"
#include "sdf_resource.h"
#include "sdf.h"
#include "dat_state.h"
#include "mnu_profile_progress.h"
#include "mnu_sprite_resource.h"
#include "mnu_scene_list.h"
#include "sdf_grid.h"
#include "mnu_scene_work.h"
#include "mnu_mantra_grid.h"

#define MNU_MANTRA_GRID_ROW_COUNT 0x11
#define MNU_MANTRA_GRID_COLUMN_COUNT 15
#define MNU_MANTRA_PROFILE_MATCH_FLAG 1
#define MNU_MANTRA_PROFILE_CAP_FLAG 2
#define MNU_MANTRA_ENTRY_STATE_PAIR_MASK 0xC
#define MNU_MANTRA_SELECTED_DRAW_CODE 0x34
#define MNU_MANTRA_CAP_DRAW_CODE 0x32
#define MNU_MANTRA_STATE_DRAW_CODE 0x33
#define MNU_MANTRA_CAP_OVERLAY_CODE 0x2F
#define MNU_MANTRA_SELECTED_OVERLAY_CODE 0x30
#define MNU_MANTRA_COST_ICON_X_OFFSET 0x19
#define MNU_MANTRA_COST_ICON_Y_OFFSET 0x5C

extern void *sdfAllocSizeClassBlock(s32);

void effDestroyResourceSlotSet(u32);

typedef struct MovieCueNode MovieCueNode;


extern f32 effMiscRandUnitFloat(s32);
extern void mnuReleaseOptionalDrawAllocation(void *, void *);




void mnuDestroyMantraDrawPool(MnuSpriteResourceGroup *resources) {
    s32 i;
    for (i = 0; i < 10; i++) {
        if (resources->tasks[i] != 0) {
            sdfDestroyTaskWork(resources->tasks[i]);
        }
    }
    sdfReleaseResourceAllocation(resources->allocation);
}

extern void func_002BF4E0(s32, s32, s32, u32, s32, s32, s32, s32);
extern s32 D_0036C698[];

void mnuDrawIconAlpha(s32 x, s32 y, s32 z, s32 alpha, s32 param) {
    func_002BF4E0(x << 4, y << 3, z, (u32)((f32)(alpha << 8) * 0.0078125f), 0, D_0036C698[0], 0x25, param);
}

struct MovieCueNode {
    s32 x;
    s32 y;
    s32 framesLeft;       /* 0x08 */
    s32 duration;         /* 0x0C */
    u8 pad10;
    s8 cueIndex;          /* 0x11 */
    u8 enabled;           /* 0x12 */
};

extern void func_0025BA20(MnuSpriteResourceGroup *, SdfList *, void *, s8);

SdfList *mnuTickResourceGroup(MnuSpriteResourceGroup *owner, SdfList *group) {
    SdfListNode *list = group->head;
    MovieCueNode *node;

    if (list == NULL) {
        sdfDestroyTaskWork(group);
        return 0;
    }
    do {
        node = list->value;
        node->framesLeft = node->framesLeft - 1;
        /* Fire an enabled cue five frames before its node expires. */
        if (node->framesLeft == node->duration - 5 && node->enabled != 0) {
            func_0025BA20(owner, group, node, node->cueIndex);
        }
        if (node->framesLeft == 0) {
            list = sdfListRemoveNode(group, list);
        } else {
            list = list->next;
        }
    } while (list != NULL);
    return group;
}
void func_0025BDD0(MnuSpriteResourceGroup *resources) {
    SdfList **slot;
    s32 i;
    SdfList *group;

    if (resources->spawnCountdown == 0) {
        if (resources->activeCount < 10) {
            group = sdfCreateTaskHeader(0);
            group->onRemove = mnuReleaseOptionalDrawAllocation;
            func_0025BA20(resources, group, NULL, 0);
            for (i = 0; i < 10; i++) {
                if (resources->tasks[i] == 0) {
                    resources->tasks[i] = group;
                    resources->activeCount++;
                    break;
                }
            }
        }
        resources->spawnCountdown = (s32)(effMiscRandUnitFloat(0) * 20.0f + 1.0f);
    } else {
        resources->spawnCountdown--;
    }
    slot = resources->tasks;
    for (i = 0; i < 10; i++, slot++) {
        group = *slot;
        if (group != NULL) {
            *slot = mnuTickResourceGroup(resources, group);
            if (*slot == NULL) {
                resources->activeCount--;
            }
        }
    }
}


extern f32 sdfSinPoly(f32);

/* Draw every live cue in the ten task slots using its remaining-life sine fade. */
void func_0025BF18(s32 x, s32 y, s32 depth, s32 alpha,
                   MnuSpriteResourceGroup *resources, s32 drawArg) {
    SdfList **slot = resources->tasks;
    s32 i;

    for (i = 9; i >= 0; i--, slot++) {
        SdfList *group = *slot;

        if (group != NULL) {
            SdfListNode *link = group->head;

            if (link != NULL) {
                do {
                    MovieCueNode *cue = link->value;
                    f32 fade = sdfSinPoly(((f32)cue->framesLeft /
                                           (f32)cue->duration) * 3.14159265f);

                    mnuDrawIconAlpha(x + cue->x, y + cue->y, depth,
                                     (s32)((f32)alpha * fade), drawArg);
                    link = link->next;
                } while (link != NULL);
            }
        }
    }
}

typedef struct {
    u8 pad00[0x6C];
    u32 resourceHandle; /* 0x6C */
} MenuResourceWork;

u32 mnuRequestEffectResource(u32 ctx, u32 config) {
    MenuResourceWork *work = (MenuResourceWork *)sdfAllocSizeClassBlock(0x70);
    memset(work, 0, 0x70);
    effRequestResourceByMode(ctx, config, 0, (u32)&work->resourceHandle);
    return (u32)work;
}

u8 mnuHasEffectResourceHandle(MenuResourceWork *work) {
    return work->resourceHandle != 0;
}

void mnuReleaseEffectResource(MenuResourceWork *work) {
    effDestroyResourceSlotSet(work->resourceHandle);
    sdfReleaseChipBlock(work);
}
INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025C0D8);


typedef s16 MnuSpritePlacement[4];

/* Packed 12-byte scene row used by the profile-menu placement table. */
typedef s16 MnuSceneSpriteEntry[6];

enum {
    MNU_SCENE_SPRITE_INDEX = 1,
    MNU_SCENE_SPRITE_X_OFFSET,
    MNU_SCENE_SPRITE_Y_OFFSET,
};

extern MnuSpritePlacement D_0036C268[];
extern MnuSceneSpriteEntry D_0036BE38[];
extern s32 D_0036C6CC[];

void func_0025C1C8(s32 x, s32 y, s32 z, s32 alpha, s32 placementIndex,
                  s32 flags, s32 context) {
    func_002BF4E0((x + D_0036C268[placementIndex][2]) << 4,
                  (y + D_0036C268[placementIndex][3]) << 3,
                  z,
                  (u32)((f32)(alpha << 8) * 0.0078125f),
                  flags,
                  D_0036C698[D_0036C268[placementIndex][0]],
                  D_0036C268[placementIndex][1],
                  context);
}

void func_0025C278(s32 x, s32 y, s32 z, s32 alpha, s32 entryIndex,
                   s32 placementIndex, s32 flags, s32 context) {
    func_002BF4E0((x + D_0036BE38[entryIndex][MNU_SCENE_SPRITE_X_OFFSET] +
                   D_0036C268[placementIndex][2]) << 4,
                  (y + D_0036BE38[entryIndex][MNU_SCENE_SPRITE_Y_OFFSET] +
                   D_0036C268[placementIndex][3]) << 3,
                  z,
                  (u32)((f32)(alpha << 8) * 0.0078125f),
                  flags,
                  D_0036C698[D_0036C268[placementIndex][0]],
                  D_0036C268[placementIndex][1],
                  context);
}

void func_0025C350(s32 x, s32 y, s32 z, s32 alpha, s32 entryIndex,
                   s32 flags, s32 context) {
    if (D_0036BE38[entryIndex][MNU_SCENE_SPRITE_INDEX] != 0) {
        func_002BF4E0((x + D_0036BE38[entryIndex][MNU_SCENE_SPRITE_X_OFFSET] + 23) << 4,
                      (y + D_0036BE38[entryIndex][MNU_SCENE_SPRITE_Y_OFFSET] + 15) << 3,
                      z,
                      (u32)((f32)(alpha << 8) * 0.0078125f),
                      flags,
                      D_0036C6CC[0],
                      D_0036BE38[entryIndex][MNU_SCENE_SPRITE_INDEX],
                      context);
    }
}

void func_0025C418(s32 x, s32 y, s32 z, s32 alpha, s32 entryIndex,
                   s32 flags, s32 context) {
    func_002BF4E0((x + D_0036BE38[entryIndex][MNU_SCENE_SPRITE_X_OFFSET]) << 4,
                  (y + D_0036BE38[entryIndex][MNU_SCENE_SPRITE_Y_OFFSET]) << 3,
                  z,
                  (u32)((f32)(alpha << 8) * 0.0078125f),
                  flags,
                  D_0036C6CC[0],
                  D_0036BE38[entryIndex][MNU_SCENE_SPRITE_INDEX],
                  context);
}

void func_0025C4C8(s32 x, s32 y, s32 z, s32 alpha, s32 entryIndex,
                   s32 spriteIndex, s32 flags, s32 context) {
    func_002BF4E0((x + D_0036BE38[entryIndex][MNU_SCENE_SPRITE_X_OFFSET]) << 4,
                  (y + D_0036BE38[entryIndex][MNU_SCENE_SPRITE_Y_OFFSET]) << 3,
                  z,
                  (u32)((f32)(alpha << 8) * 0.0078125f),
                  flags,
                  D_0036C698[1],
                  spriteIndex,
                  context);
}

void mnuAdvanceLoopingFrame(s32 *frame) {
    s32 oldFrame;

    oldFrame = *frame;
    *frame = oldFrame + 1;
    if (0x3c < oldFrame + 1) {
        *frame = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025C588);

extern u32 mnuGetMantraDisplayFlags(MnuMantraGridEntry *, MnuProfileProgress *);

void mnuAdvanceGridSlotAnimation(MnuProfileProgress *animationContext, SdfGrid *unusedGrid, SdfGridCell *slot) {
    MnuMantraGridEntry *entry = (MnuMantraGridEntry *)(u32)slot->value;
    s32 value = entry->frame + 1;

    entry->frame = value;
    if ((f32)value > 60.0f) {
        entry->frame = 0;
    }
    mnuGetMantraDisplayFlags(entry, animationContext);
}

/* Advance only occupied animation slots across the fixed mantra grid. */
void mnuAdvanceActiveGridSlotAnimations(MnuProfileProgress *animationContext, MenuSceneWork *gridOwner) {
    SdfGrid *grid = gridOwner->gridHandle;
    s32 row;
    s32 column;

    for (row = 0; row < MNU_MANTRA_GRID_ROW_COUNT; row++) {
        SdfGridCell *slot = grid->cells + row * grid->width;
        for (column = 0; column < MNU_MANTRA_GRID_COLUMN_COUNT; column++) {
            if (slot[column].value != 0) {
                mnuAdvanceGridSlotAnimation(animationContext, grid, &slot[column]);
            }
        }
    }
}

extern void func_0025C278(s32, s32, s32, s32, s32, s32, s32, s32);

/* Choose draw codes from profile-match, cap and entry-state flags; cap overlay is half-strength. */
void func_0025C8D0(s32 x, s32 y, s32 depth, s32 amount, MnuProfileProgress *profileAddress, MnuMantraGridEntry *entry, s32 drawArg) {
    u32 flags = mnuGetMantraDisplayFlags(entry, profileAddress);

    if (flags & MNU_MANTRA_PROFILE_MATCH_FLAG) {
        func_0025C278(x, y, depth, amount, entry->sceneId, MNU_MANTRA_SELECTED_DRAW_CODE, 0, drawArg);
    } else if (flags & MNU_MANTRA_PROFILE_CAP_FLAG) {
        func_0025C278(x, y, depth, amount, entry->sceneId, MNU_MANTRA_CAP_DRAW_CODE, 0, drawArg);
    } else if (flags & MNU_MANTRA_ENTRY_STATE_PAIR_MASK) {
        func_0025C278(x, y, depth, amount, entry->sceneId, MNU_MANTRA_STATE_DRAW_CODE, 0, drawArg);
    }
    if (flags & MNU_MANTRA_PROFILE_CAP_FLAG) {
        func_0025C278(x, y, depth, (s32)((f32)amount * 0.5f),
                      entry->sceneId, MNU_MANTRA_CAP_OVERLAY_CODE, 0, drawArg);
    }
    if (flags & MNU_MANTRA_PROFILE_MATCH_FLAG) {
        func_0025C278(x, y, depth, amount, entry->sceneId, MNU_MANTRA_SELECTED_OVERLAY_CODE, 0, drawArg);
    }
}

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025CA50);

typedef struct MnuProfileIdList {
    s8 ids[22];
} MnuProfileIdList;

extern const MnuProfileIdList D_003AFA00;
extern u32 prfGetCapValue(u16);
extern u32 ptyGetProfileRecordValue(struct DatPartyRecord *, u16);
extern void func_0025C1C8(s32, s32, s32, s32, s32, s32, s32);

/* Draw cap markers, with a second marker for the first four profiles. */
void mnuDrawCappedProfileMarkers(s32 x, s32 y, s32 depth, s32 alpha,
                   MnuProfileProgress *owner, s32 context) {
    MnuProfileIdList profiles = D_003AFA00;
    s32 i;
    u32 cap;

    for (i = 0; i < 22; i++) {
        cap = prfGetCapValue(profiles.ids[i]);
        if (cap == ptyGetProfileRecordValue(owner->partyRecord, profiles.ids[i])) {
            func_0025C1C8(x, y, depth, alpha, i + 0x10, 0, context);
            if (i < 4) {
                func_0025C1C8(x, y, depth, alpha, i + 0x2B, 0x20, context);
            }
        }
    }
}

extern void func_0025CA50(s32, s32, s32, s32, s32,
                          MnuProfileProgress *, MnuMantraGridEntry *, s32);
extern void func_0025C588(s32, s32, s32, s32, s32, s32);

/* Draw the grid background and profile markers, then animation and status layers in separate passes. */
void func_0025D100(s32 x, s32 y, s32 z, s32 alpha,
                   MnuProfileProgress *profileOwner, MenuSceneWork *gridOwner,
                   s32 context) {
    SdfGrid *grid = gridOwner->gridHandle;
    SdfGridCell *cell;
    MnuMantraGridEntry *entry;
    s32 row;
    s32 column;

    func_0025C1C8(x, y, z, alpha, 0xF, 0x20, context);
    mnuDrawCappedProfileMarkers(x, y, z,
                                (s32)((f32)alpha * 0.5f),
                                profileOwner, context);
    for (row = 0; row < MNU_MANTRA_GRID_ROW_COUNT; row++) {
        cell = grid->cells + row * grid->width;
        for (column = 0; column < MNU_MANTRA_GRID_COLUMN_COUNT; column++, cell++) {
            entry = (MnuMantraGridEntry *)(u32)cell->value;
            if (entry != NULL) {
                func_0025CA50(x, y, z,
                              (s32)((f32)alpha * 0.5f),
                              (s32)gridOwner, profileOwner, entry, context);
            }
        }
    }
    for (row = 0; row < MNU_MANTRA_GRID_ROW_COUNT; row++) {
        cell = grid->cells + row * grid->width;
        for (column = 0; column < MNU_MANTRA_GRID_COLUMN_COUNT; column++, cell++) {
            entry = (MnuMantraGridEntry *)(u32)cell->value;
            if (entry != NULL) {
                func_0025C8D0(x, y, z, alpha,
                              profileOwner, entry, context);
            }
        }
    }
    func_0025C588(x, y, z, alpha, (s32)gridOwner, context);
}

extern void mnuAdvanceActiveGridSlotAnimations(MnuProfileProgress *, MenuSceneWork *);

/* Update the owner's grid slots and its independent looping-frame counter. */
void mnuAdvanceDisplayGridAndLoopingFrame(MenuSceneWork *gridOwner, MnuProfileProgress *animationContext) {
    mnuAdvanceActiveGridSlotAnimations(animationContext, gridOwner);
    mnuAdvanceLoopingFrame(&gridOwner->gridFrame);
}

typedef struct MnuRequirementIds {
    u16 ids[4];
} MnuRequirementIds;

typedef struct MnuPanelRegion {
    u16 x;
    u16 y;
} MnuPanelRegion;

typedef struct MnuPanelRegions {
    MnuPanelRegion rows[4];
} MnuPanelRegions;

extern const MnuRequirementIds D_003BC4D8[];
extern const MnuPanelRegions D_003AFA18;
extern u32 mnuGetSelectedNodeValue(void);
extern s32 prfReqCheckWithFallback(void *, u16);
extern s32 mdlFlagTest(s32);
extern void sdfSubmitGsTestOneRegisterPacket();
extern void sdfSubmitGsAlphaOneRegisterPacket(u32, u32);
extern void uiDrawUniformColorRect(u32, u32, u32, u32, u32, u32, u32);
extern void uiDrawActiveSurfaceRegion(s32);
extern void sdfDispatchSurfaceWithPreparedTexturePacket(s32);

/* Build the mantra selection surface, including requirement-dependent panels,
 * then draw the grid and restore the surface state for the caller. */
void func_0025D2F8(s32 x, s32 y, s32 unusedDepth, s32 alpha,
                   MenuSceneWork *gridOwner, s32 context) {
    MnuProfileProgress *profileOwner = (MnuProfileProgress *)mnuGetSelectedNodeValue();

    mnuAdvanceDisplayGridAndLoopingFrame(gridOwner, profileOwner);
    sdfSubmitGsTestOneRegisterPacket(0x30000, context);
    uiDrawUniformColorRect(0, 0, -1, 0x2000, 0xE00, 0, context);
    sdfSubmitGsTestOneRegisterPacket(0x3000DL, context);
    uiDrawActiveSurfaceRegion(context);
    sdfSubmitGsTestOneRegisterPacket(0x30000, context);

    {
        MnuRequirementIds requirementIds = D_003BC4D8[0];
        MnuPanelRegions panelRegions = D_003AFA18;
        u16 *requirement;
        MnuPanelRegion *region;
        s32 remaining;

        if (prfReqCheckWithFallback((void *)profileOwner->partyRecord,
                                    requirementIds.ids[0]) != 0) {
            uiDrawUniformColorRect(0x1980, 0, 0, 0x680, 0x280, 0x80,
                                   context);
            uiDrawUniformColorRect(0x1BF0, panelRegions.rows[0].y << 3, 0,
                                   0x410, 0x4B0, 0x80, context);
        }

        region = &panelRegions.rows[1];
        requirement = &requirementIds.ids[1];
        for (remaining = 2; remaining >= 0;
             remaining--, requirement++, region++) {
            if (prfReqCheckWithFallback((void *)profileOwner->partyRecord,
                                        *requirement) != 0) {
                uiDrawUniformColorRect(region->x << 4, region->y << 3, 0,
                                       0x200, 0x100, 0x80, context);
            }
        }

        if (prfReqCheckWithFallback((void *)profileOwner->partyRecord, 0x4E) != 0 ||
            mdlFlagTest(0x908) != 0) {
            uiDrawUniformColorRect(0x1C90, 0x640, 0, 0x200, 0xA0, 0x80,
                                   context);
        }
        if (prfReqCheckWithFallback((void *)profileOwner->partyRecord, 0x4F) != 0) {
            uiDrawUniformColorRect(0x1C90, 0x6E0, 0, 0x200, 0xA0, 0x80,
                                   context);
        }
    }

    {
        u32 displayFlags = (u8)gridOwner->boundsFlags;

        if (displayFlags & 1) {
            uiDrawUniformColorRect(0, 0x280, 0, 0x1BF0, 0xB80, 0x80,
                                   context);
        } else {
            uiDrawUniformColorRect(0, 0x280, 0, 0x1980, 0xB80, 0x80,
                                   context);
        }
    }
    uiDrawUniformColorRect(0x1980, 0xC10, 0, 0x680, 0x1F0, 0x80,
                           context);
    sdfDispatchSurfaceWithPreparedTexturePacket(context);
    sdfSubmitGsAlphaOneRegisterPacket(0x44, context);
    sdfSubmitGsTestOneRegisterPacket(0x50000, context);
    func_0025D100(x, y, 1, alpha, profileOwner, gridOwner, context);
    sdfSubmitGsAlphaOneRegisterPacket(0x44, context);
    sdfSubmitGsTestOneRegisterPacket(0x30000, context);
    uiDrawUniformColorRect(0, 0, 0, 0x2000, 0xE00, 0, context);
    sdfSubmitGsTestOneRegisterPacket(0x5100DL, context);
}

extern f32 func_002C84F0(f32 *);
extern s32 D_003BC4E0;

/* Ease the live cursor toward the selected scene row and track whether it moved. */
void func_0025D628(MenuSceneWork *work) {
    MnuMantraGridEntry *selectedEntry;
    s32 entryIndex;
    s32 targetX;
    s32 targetY;
    f32 direction[4];
    s32 moving;
    s32 current;
    s32 difference;
    s32 next;

    moving = 0;
    selectedEntry = (MnuMantraGridEntry *)(u32)work->gridHandle->cursor->value;
    entryIndex = selectedEntry->sceneId;
    targetX = D_0036BE38[entryIndex][MNU_SCENE_SPRITE_X_OFFSET];
    targetY = D_0036BE38[entryIndex][MNU_SCENE_SPRITE_Y_OFFSET];
    memset(direction, 0, sizeof(direction));
    direction[0] = targetX - work->entryPosition.x;
    direction[1] = targetY - work->entryPosition.y;
    func_002C84F0(direction);

    current = work->entryPosition.x;
    difference = targetX - current;
    if (difference != 0) {
        moving = 1;
        next = (s32)((f32)current + direction[0] * 15.0f);
        work->entryPosition.x = next;
        if (difference * (targetX - (s16)next) < 0) {
            work->entryPosition.x = targetX;
        }
    }

    current = work->entryPosition.y;
    difference = targetY - current;
    if (difference != 0) {
        moving = 1;
        next = (s32)((f32)current + direction[1] * 15.0f);
        work->entryPosition.y = next;
        if (difference * (targetY - (s16)next) < 0) {
            work->entryPosition.y = targetY;
        }
    }

    if (moving != 0) {
        D_003BC4E0++;
    } else {
        D_003BC4E0 = 0;
    }
    work->cursorMoving = moving;
}

typedef struct Bytes7 {
    s8 b[7];
} Bytes7;

extern Bytes7 D_003BC4E8[];
extern void func_0024E260(s32, s32, s32, s32, s32, s32);

/* Select the cost marker from the record's icon index and draw at the fixed badge offset. */
void mnuDrawMantraCostIcon(s32 x, s32 y, s32 depth, s32 recordAddress, s32 amount, s32 drawArg) {
    Bytes7 table = D_003BC4E8[0];

    func_0024E260(x - MNU_MANTRA_COST_ICON_X_OFFSET, y + MNU_MANTRA_COST_ICON_Y_OFFSET, depth, amount, table.b[*(u16 *)(recordAddress + 4)], drawArg);
}

/* Advance and draw one transient cost/list notification; report expiry at frame 11. */
s32 func_0025D7F8(MnuSceneListNode *node, s32 index, s32 drawArg) {
    s32 frame = node->frame;
    s32 opening = frame < 7;
    s32 context = drawArg;
    f32 progress;

    if (opening != 0) {
        progress = (f32)frame / 6.0f;
        if (progress > 1.0f) {
            progress = 1.0f;
        }
        mnuDrawMantraCostIcon(0, 0, 0, node->payload.timedDraw.recordAddress,
                              (s32)(progress * 128.0f), context);
    }

    if (node->next == NULL) {
        if (opening == 0) {
            mnuDrawMantraCostIcon(0, 0, 0, node->payload.timedDraw.recordAddress, 0x80,
                                  context);
        }
    }

    if (opening != 0) {
        progress = (f32)frame / 6.0f;
        if (node->payload.timedDraw.kind == 1) {
            func_0024E260((s32)(progress * -16.0f + -27.0f), 52, 1,
                          (s32)(progress * 96.0f + 32.0f), 0x18, context);
        } else if (node->payload.timedDraw.kind == 2) {
            func_0024E260((s32)(progress * 16.0f + -27.0f), 52, 1,
                          (s32)(progress * 96.0f + 32.0f), 0x19, context);
        }
    }

    if (frame >= 4) {
        if (frame < 9) {
            progress = (f32)(frame - 3) / 6.0f;
            progress = sdfSinPoly(progress * 3.14159265f);
            if (node->payload.timedDraw.kind == 1) {
                func_0024E260(-27, 52, 1, (s32)(progress * 128.0f),
                              0x16, context);
            } else if (node->payload.timedDraw.kind == 2) {
                func_0024E260(-27, 52, 1, (s32)(progress * 128.0f),
                              0x17, context);
            }
        }
    }

    node->frame++;
    if (frame < 11) {
        return 0;
    }
    return 1;
}

MnuSceneListNode *mnuAllocateMenuListNode(void) {
    MnuSceneListNode *node = sdfAllocSizeClassBlock(0x14);

    memset(node, 0, 0x14);
    return node;
}

MnuSceneListNode *mnuAppendNodeToDisplayList(MnuSceneListHead *head) {
    MnuSceneListNode *node = head->first;
    if (node == NULL) {
        node = mnuAllocateMenuListNode();
        head->first = node;
    } else {
        while (node->next != NULL) {
            node = node->next;
        }
        node->next = mnuAllocateMenuListNode();
        node = node->next;
    }
    return node;
}

MnuSceneListNode *mnuFreeMenuListNodeAndGetNext(MnuSceneListNode *node) {
    MnuSceneListNode *next;

    next = node->next;
    sdfReleaseChipBlock(node);
    return next;
}

void mnuReleaseListNodes(MnuSceneListHead *head) {
    MnuSceneListNode *node = head->first;

    while (node != NULL) {
        node = mnuFreeMenuListNodeAndGetNext(node);
    }
}

extern f32 sdfSinPoly(f32);

s32 mnuDrawMantraSineFade(s32 frame, s32 amount, s32 drawArg) {
    f32 wave = (f32)frame / 60.0f;
    s32 shown;
    s32 halfShown;

    wave = sdfSinPoly(wave * 3.14159265f);
    shown = (f32)amount * (wave * 0.5f + 0.5f);
    func_0024E260(-27, 52, 0, shown, 0x16, drawArg);
    func_0024E260(-27, 52, 0, shown, 0x17, drawArg);
    halfShown = (f32)amount * 0.5f;
    func_0024E260(-27, 52, 0, halfShown, 0x18, drawArg);
    func_0024E260(-27, 52, 0, halfShown, 0x19, drawArg);
    if (frame < 60) {
        return 0;
    }
    return 1;
}

extern s32 func_0025D7F8(MnuSceneListNode *, s32, s32);

s32 mnuAdvanceDisplayList(MnuSceneListHead *head, s32 amount, s32 drawContext) {
    MnuSceneListNode *node = head->first;
    s32 index = 0;

    if (mnuDrawMantraSineFade(head->frameCount, amount, drawContext) != 0) {
        head->frameCount = 0;
    } else {
        head->frameCount = head->frameCount + 1;
    }
    if (node == NULL) {
        return 1;
    }
    do {
        s32 hit = func_0025D7F8(node, index, drawContext);

        index++;
        if (hit != 0) {
            node = mnuFreeMenuListNodeAndGetNext(node);
            head->first = node;
        } else {
            node = node->next;
        }
    } while (node != NULL);
    return 0;
}

extern void mnuDrawMantraCostIcon(s32, s32, s32, s32, s32, s32);

/* Draw the selected record's cost marker once the animated list reports completion. */
void mnuDrawMantraCostAfterListAdvance(s32 recordAddress, MnuSceneListHead *head,
                                       s32 amount, s32 drawContext) {
    if (mnuAdvanceDisplayList(head, amount, drawContext) != 0) {
        mnuDrawMantraCostIcon(0, 0, 0, recordAddress, amount, drawContext);
    }
}

extern u16 D_0036C6D0[];
extern s32 mdlFlagTest(s32);

void mnuCollectFlagArray(u8 *flags) {
    u32 i;

    for (i = 0; i < 0x29; i++, flags++) {
        if (mdlFlagTest(D_0036C6D0[i]) != 0) {
            *flags = 1;
        }
    }
}

extern u16 D_0036C6D0[];
extern void mdlFlagSet(s32);

void mnuApplyFlagArray(u8 *flags) {
    u32 i;

    for (i = 0; i < 0x29; i++) {
        if (*flags++ != 0) {
            mdlFlagSet(D_0036C6D0[i]);
        }
    }
}

INCLUDE_RODATA(const s32, "game/code_0025BC38", D_003AFA00);

INCLUDE_RODATA(const s32, "game/code_0025BC38", D_003AFA18);

INCLUDE_SDATA(const s32, "game/code_0025BC38", D_003BC4D0);

INCLUDE_SDATA(const s32, "game/code_0025BC38", D_003BC4D8);

INCLUDE_SDATA(const s32, "game/code_0025BC38", D_003BC4E0);

INCLUDE_SDATA(const s32, "game/code_0025BC38", D_003BC4E8);
