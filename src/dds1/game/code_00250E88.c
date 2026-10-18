#include "mnu_variant_sprite_placement.h"
#include "dsp_entry_api.h"
#include "dsp_channel.h"
#include "evt_message_window_api.h"
#include "dsp_name.h"
#include "sdf_resource.h"
#include "mnu.h"
#include "dat_state.h"
#include "mnu_profile_progress.h"
#include "mnu_mantra_grid.h"
#include "mnu_scene_work.h"
#include "sdf_task_work.h"
#include "sdf_chip.h"
#include "mdl_flag_api.h"
#include "prf_profile_cap.h"
#include "prf_requirement_fallback.h"
#include "common.h"
#include "sdf_trig.h"
#include "snd_sequence_volume_pan.h"
/* Declared before first use (were implicit int). */
extern void * memset(void *, s32, u32);

extern u32 *mnuGetSelectedNodeValue(void);
#define MNU_SCENE_WORK_SIZE 0x5B0
#define MNU_SCENE_SHADE_FRAME_LIMIT 10
#define MNU_SCENE_SHADE_FRAME_SCALE 10.0f
#define MNU_SCENE_SHADE_ALPHA_SCALE 112.0f
#define MNU_SCENE_DRAW_CONTEXT 0x53

extern void mnuUpdateMantraSelectionPositions(MenuSceneWork *, s32);

extern void mnuInitializeMantraSelectionGrid(MenuSceneWork *);

extern void mnuCopySceneCoordinates(MenuSceneWork *);

extern void sdfDestroyGridWork(SdfGrid *);

extern void mnuResetWorkFloats(void);


extern TaskWork *mnuSceneResourceContext;

typedef struct {
    u32 unk0;
    u16 entryX;
    u16 entryY;
    u32 unk8;
} SceneEntry;

extern SceneEntry mnuMantraEntrySpritePlacements[];

extern MnuVariantSpritePlacement mnuMantraVariantSpritePlacements[];

typedef struct ScrVmOperand ScrVmOperand;



void mnuScrollSceneViewportToEntry(s32 *xCoordinate, s32 *yCoordinate, u16 index,
                   s32 maximumX, s32 maximumY) {
    s32 x = *xCoordinate;
    s32 deltaX = mnuMantraVariantSpritePlacements[index][2] - x;
    s32 deltaY = mnuMantraVariantSpritePlacements[index][3] - *yCoordinate;

    if (deltaX >= 0x13D) {
        *xCoordinate += deltaX - 0x13C;
        x = *xCoordinate;
    }
    if (maximumX < x) {
        *xCoordinate = maximumX;
    }
    if (deltaY >= 0x9C) {
        *yCoordinate += deltaY - 0x9B;
    }
    if (maximumY < *yCoordinate) {
        *yCoordinate = maximumY;
    }
    if (deltaX < 0x82) {
        /* Reload X after updating Y; the caller's coordinate pointers may alias. */
        s32 adjustedX = *xCoordinate + deltaX - 0x82;
        *xCoordinate = adjustedX < 0 ? 0 : adjustedX;
    }
    if (deltaY < 0x19) {
        s32 adjustedY = *yCoordinate + deltaY - 0x19;
        *yCoordinate = adjustedY < 0 ? 0 : adjustedY;
    }
}

INCLUDE_ASM(const s32, "game/code_00250E88", mnuAdvanceMantraSceneScroll);

void mnuUpdateMantraSceneScroll(MenuSceneWork *work) {
    MnuMantraGridEntry *entry =
        (MnuMantraGridEntry *)(u32)work->gridHandle->cursor->value;
    s32 maximumX = 0x307;
    u8 flags = work->boundsFlags;
    s32 position[2];

    if ((flags & MENU_SCENE_REQUIREMENT_GROUP_2_MET) == 0) {
        maximumX = 0x2C8;
        if ((flags & MENU_SCENE_REQUIREMENT_GROUP_1_MET) == 0) {
            maximumX = (flags & MENU_SCENE_REQUIREMENT_GROUP_0_MET) != 0
                           ? 0x24C
                           : 0x1BE;
        }
    }
    position[0] = work->scrollX;
    position[1] = work->scrollY;
    mnuScrollSceneViewportToEntry(&position[0], &position[1], entry->sceneId, maximumX, 0x38E);
    work->scrollX = position[0];
    work->scrollY = position[1];
}

extern f32 sdfVec3NormalizeInPlaceAndGetLength(f32 *vector);
s32 D_003BC420 = 0;
s32 D_003BC424 = 0;

void mnuUpdateMantraSelectionPositions(MenuSceneWork *work, s32 mode) {
    SdfGrid *grid = work->gridHandle;
    MnuMantraGridEntry *entry;
    f32 vector[4];
    s32 targetX;
    s32 targetY;
    s32 deltaX;
    s32 deltaY;
    s32 candidate;
    s32 step;
    s32 moving = 0;

    mnuUpdateMantraSceneScroll(work);
    targetX = work->scrollX;
    targetY = work->scrollY;

    if (mode == 1) {
        work->cursorPosition.y = targetY;
        work->cursorPosition.x = targetX;
        entry = (MnuMantraGridEntry *)(u32)grid->cursor->value;
        targetX = mnuMantraVariantSpritePlacements[entry->sceneId][2];
        targetY = mnuMantraVariantSpritePlacements[entry->sceneId][3];
        work->entryPosition.x = targetX;
        work->entryPosition.y = targetY;
        return;
    }

    step = D_003BC424 * 10 + 4;
    if (step >= 0x2D) {
        step = 0x2C;
    }

    memset(vector, 0, sizeof(vector));
    vector[0] = (f32)(targetX - work->cursorPosition.x);
    vector[1] = (f32)(targetY - work->cursorPosition.y);
    sdfVec3NormalizeInPlaceAndGetLength(vector);

    deltaX = targetX - work->cursorPosition.x;
    if (deltaX != 0) {
        candidate = (s32)((f32)work->cursorPosition.x + vector[0] * (f32)step);
        work->cursorPosition.x = candidate;
        if (deltaX * (targetX - work->cursorPosition.x) < 0) {
            work->cursorPosition.x = targetX;
        }
    }
    deltaY = targetY - work->cursorPosition.y;
    if (deltaY != 0) {
        candidate = (s32)((f32)work->cursorPosition.y + vector[1] * (f32)step);
        work->cursorPosition.y = candidate;
        if (deltaY * (targetY - work->cursorPosition.y) < 0) {
            work->cursorPosition.y = targetY;
        }
    }

    entry = (MnuMantraGridEntry *)(u32)grid->cursor->value;
    targetX = mnuMantraVariantSpritePlacements[entry->sceneId][2];
    targetY = mnuMantraVariantSpritePlacements[entry->sceneId][3];
    memset(vector, 0, sizeof(vector));
    vector[0] = (f32)(targetX - work->entryPosition.x);
    vector[1] = (f32)(targetY - work->entryPosition.y);
    sdfVec3NormalizeInPlaceAndGetLength(vector);

    deltaX = targetX - work->entryPosition.x;
    if (deltaX != 0) {
        moving = 1;
        candidate = (s32)((f32)work->entryPosition.x + vector[0] * 40.0f);
        work->entryPosition.x = candidate;
        if (deltaX * (targetX - work->entryPosition.x) < 0) {
            work->entryPosition.x = targetX;
        }
    }
    deltaY = targetY - work->entryPosition.y;
    if (deltaY != 0) {
        moving = 1;
        candidate = (s32)((f32)work->entryPosition.y + vector[1] * 40.0f);
        work->entryPosition.y = candidate;
        if (deltaY * (targetY - work->entryPosition.y) < 0) {
            work->entryPosition.y = targetY;
        }
    }

    if (moving != 0) {
        D_003BC424++;
    } else {
        D_003BC424 = 0;
    }
    work->cursorMoving = moving;
}

/* Release a nonnull grid-cell payload; the cell index is unused. */
s32 mnuFreeTaskData(u32 unused, u32 taskData) {
    if (taskData != 0) {
        sdfReleaseChipBlock((void *)taskData);
    }
}

extern s32 prfCheckRequirementRules(s32, ScrVmOperand *, u16);
extern void sdfGridGetCursorCoordinates(SdfGrid *, u32 *, u32 *);
extern u32 ptyGetProfileRecordValue(DatPartyRecord *, u16);
extern void *memcpy(void *, const void *, u32);
extern const s32 D_003AF810[8];
extern const u16 D_003AF830[8];

s32 mnuScheduleUnlockedMantraMessages(MenuSceneWork *work) {
    s32 pendingFlagBases[7];
    u16 requirementIds[8];
    MnuProfileProgress *selection;
    s32 *flagBase;
    s32 i;
    u32 cap;
    u32 value;
    s32 pending = 0;

    memcpy(pendingFlagBases, D_003AF810, 0x1C);
    memcpy(requirementIds, D_003AF830, 0x10);
    /* The native clear includes 32 reserved bytes after the eight flags. */
    memset((u8 *)work + (u32)&((MenuSceneWork *)0)->pendingMantras, 0, 0x40);

    selection = (MnuProfileProgress *)(u32)mnuGetSelectedNodeValue();
    flagBase = &pendingFlagBases[selection->partyRecord->unitId];

    for (i = 0; i < 8; i++) {
        if (mdlFlagTest(*flagBase + i) == 0 &&
            (prfCheckRequirementRules(0, (ScrVmOperand *)selection->partyRecord,
                              requirementIds[i]) != 0 ||
             (i == 6 && mdlFlagTest(0x908) != 0))) {
            cap = prfGetCapValue(requirementIds[i]);
            value = ptyGetProfileRecordValue(selection->partyRecord, requirementIds[i]);
            if (cap == value) {
                mdlFlagSet(*flagBase + i);
            } else {
                work->pendingMantras[i] = 1;
                mdlFlagSet(*flagBase + i);
                if (pending == 0) {
                    sdfGridGetCursorCoordinates(work->gridHandle,
                                                &work->pendingGridColumn,
                                                &work->pendingGridRow);
                    work->pendingCursorCoordinates.x = work->cursorPosition.x;
                    work->pendingCursorCoordinates.y = work->cursorPosition.y;
                }
                pending = 1;
            }
        }
    }

    return pending;
}


extern DspUnitName *D_003BAA70;
extern DspMantraName *D_003BAA78;
extern const s32 D_003AF840[];
extern const s32 D_003AF850[];
extern void *memcpy(void *, const void *, u32);
extern MnuMantraGridEntry *fldGetSceneMetadataNode(void);
extern void evtCopyEntryStringToActiveWindow(s32, s32);
extern SdfGridCell *sdfGridSelectFilledCell(SdfGrid *, u32, u32);

/* Display the selected mantra and move the scene grid to its filled cell. */
s32 mnuDisplayNextPendingMantra(s32 context) {
    s16 mantraIds[8];
    s32 coordinates[8][2];
    MnuProfileProgress *selection;
    s32 *pendingFlags;
    s32 i;
    SdfGrid *grid;
    s32 x;
    s32 y;

    memcpy(mantraIds, D_003AF840, sizeof(mantraIds));
    memcpy(coordinates, D_003AF850, sizeof(coordinates));
    selection = (MnuProfileProgress *)mnuGetSelectedNodeValue();
    fldGetSceneMetadataNode();
    pendingFlags = ((MenuSceneWork *)context)->pendingMantras;
    for (i = 0; i < 8; i++) {
        if (pendingFlags[i] != 0) {
            evtCopyEntryStringToActiveWindow(
                0, (s32)D_003BAA70[selection->partyRecord->unitId].encodedText);
            evtCopyEntryStringToActiveWindow(
                1, (s32)D_003BAA78[mantraIds[i]].encodedText);
            dspStartEntry(1);
            pendingFlags[i] = 0;
            grid = ((MenuSceneWork *)context)->gridHandle;
            y = coordinates[i][1];
            x = coordinates[i][0];
            sdfGridSelectFilledCell(grid, x, y);
            return 0;
        }
    }
    return 1;
}


extern void *memset(void *, s32, u32);

/* Allocate and clear scene work before registering its grid and coordinates. */
MenuSceneWork *mnuCreateSceneWork(void) {
    struct SdfMemBlock *allocation = sdfAllocGeneralBlock(MNU_SCENE_WORK_SIZE);
    u8 *sceneWork = (u8 *)sdfMemoryGetBlockAddress(allocation);

    memset(sceneWork, 0, MNU_SCENE_WORK_SIZE);
    ((MenuSceneWork *)sceneWork)->allocation = allocation;
    mnuInitializeMantraSelectionGrid((MenuSceneWork *)sceneWork);
    ((MenuSceneWork *)sceneWork)->scenePhase = 0;
    ((MenuSceneWork *)sceneWork)->phaseFrame = 0;
    ((MenuSceneMetadata *)sdfGetTaskValueByKey(mnuSceneResourceContext, SDF_TASK_VALUE_USER_DATA_KEY))->displayedCurrency = datGameState->header.currency;
    mnuCopySceneCoordinates((MenuSceneWork *)sceneWork);
    return (MenuSceneWork *)sceneWork;
}

/* Retain the native metadata lookup, then release grid/list/allocation resources and reset projection state. */
void mnuReleaseSceneContext(s32 unused, MenuSceneWork *scene) {
    sdfGetTaskValueByKey(mnuSceneResourceContext, SDF_TASK_VALUE_USER_DATA_KEY);
    sdfDestroyGridWork(scene->gridHandle);
    mnuReleaseDisplayListNodes(&scene->sceneTransitionList);
    sdfReleaseResourceAllocation(scene->allocation);
    mnuResetWorkFloats();
    /* Post-final-call exception: retail keeps jal + epilogue; this clear survives 01.sibling..11.cfg and is deleted in 13.life (cc1probe trace). */
    scene = NULL; /* the scene work was released above */
}

INCLUDE_RODATA(const s32, "game/code_00250E88", D_003AF810);

INCLUDE_RODATA(const s32, "game/code_00250E88", D_003AF830);

INCLUDE_RODATA(const s32, "game/code_00250E88", D_003AF840);

INCLUDE_RODATA(const s32, "game/code_00250E88", D_003AF850);

INCLUDE_ASM(const s32, "game/code_00250E88", mnuUpdateMantraSceneTaskState);

INCLUDE_ASM(const s32, "game/code_00250E88", mnuDrawMantraSceneFrame);

extern u8 D_0036B410[17][15];
s32 mnuSelectMantraGridLeftCell(SdfGrid *grid, s32 x, s32 y, u32 from) {
    if (from != 0 && sdfGridSelectFilledCell(grid, x, y) != NULL) {
        return 1;
    }
    if (D_0036B410[y][x] & 0x20) {
        if (mnuSelectMantraGridLeftCell(grid, x - 1, y, 0x20) != 0) {
            return 1;
        }
    } else if ((D_0036B410[y][x] & 0x8) || (D_0036B410[y][x] & 0x4)) {
        if (mnuSelectMantraGridLeftCell(grid, x - 1, y, 0x20) != 0) {
            return 1;
        }
    }
    if ((D_0036B410[y][x] & 0x80) && (from & 0x40) == 0) {
        if (mnuSelectMantraGridLeftCell(grid, x, y - 1, 0x80) != 0) {
            return 1;
        }
    }
    if ((D_0036B410[y][x] & 0x40) && (from & 0x80) == 0) {
        if (mnuSelectMantraGridLeftCell(grid, x, y + 1, 0x40) != 0) {
            return 1;
        }
    }
    return 0;
}

extern u8 D_0036B410[17][15];
s32 mnuSelectMantraGridRightCell(SdfGrid *grid, s32 x, s32 y, u32 from) {
    if ((from & 0x10) && sdfGridSelectFilledCell(grid, x, y) != NULL) {
        return 1;
    }
    if (D_0036B410[y][x] & 0x10) {
        if (mnuSelectMantraGridRightCell(grid, x + 1, y, 0x10) != 0) {
            return 1;
        }
    } else if ((D_0036B410[y][x] & 0x8) || (D_0036B410[y][x] & 0x4)) {
        if (mnuSelectMantraGridRightCell(grid, x + 1, y, 0x10) != 0) {
            return 1;
        }
    }
    if ((D_0036B410[y][x] & 0x80) && !(from & 0x40)) {
        if (mnuSelectMantraGridRightCell(grid, x, y - 1, 0x80) != 0) {
            return 1;
        }
    }
    if ((D_0036B410[y][x] & 0x40) && !(from & 0x80)) {
        if (mnuSelectMantraGridRightCell(grid, x, y + 1, 0x40) != 0) {
            return 1;
        }
    }
    return 0;
}

extern u8 D_0036B410[17][15];

extern s32 mnuSelectMantraGridLeftCell(SdfGrid *, s32, s32, u32);
extern SdfGridCell *sdfGridCursorLeftToFilledCell(SdfGrid *);

/* Select the connected left neighbor, or fall back to the previous filled cell. */
s32 mnuSelectMantraGridLeftNeighbor(SdfGrid *grid) {
    MnuMantraGridEntry *entry = (MnuMantraGridEntry *)(u32)grid->cursor->value;
    MnuMantraNodeState *coordinates = (MnuMantraNodeState *)(u32)grid->userData;
    s32 x = coordinates[entry->sceneId].x;
    s32 y = coordinates[entry->sceneId].y;

    if (x == 0) {
        if (sdfGridCursorLeftToFilledCell(grid) != NULL) {
            return 1;
        }
    } else if ((D_0036B410[y][x - 1] & 0x1C) != 0) {
        if (mnuSelectMantraGridLeftCell(grid, x - 1, y, 1) != 0) {
            return 1;
        }
    } else {
        if (sdfGridCursorLeftToFilledCell(grid) != NULL) {
            return 1;
        }
    }
    return 0;
}

extern s32 mnuSelectMantraGridRightCell(SdfGrid *, s32, s32, u32);
extern SdfGridCell *sdfGridCursorRightToFilledCell(SdfGrid *);

s32 mnuSelectMantraGridRightNeighbor(SdfGrid *grid) {
    MenuSceneWork *scene = (MenuSceneWork *)sdfGetTaskValueByKey(
        mnuSceneResourceContext, 1);
    MnuMantraGridEntry *entry = (MnuMantraGridEntry *)grid->cursor->value;
    MnuMantraNodeState *coordinates = (MnuMantraNodeState *)grid->userData;
    s32 x = coordinates[entry->sceneId].x;
    s32 y = coordinates[entry->sceneId].y;

    if ((x != 10 || scene->boundsFlags == 0) &&
        (D_0036B410[y][x] & 0x3C) != 0) {
        if (mnuSelectMantraGridRightCell(grid, x, y, 0) != 0) {
            return 1;
        }
    } else {
        if (sdfGridCursorRightToFilledCell(grid) != NULL) {
            return 1;
        }
    }
    return 0;
}

extern u16 prfGetRequiredProfileLevel(u16);
extern u8 prfGetProfileParameterByte5(u16);
extern s32 prfBuildRawSkillList(u16, PrfSkillList *);
extern s32 ptyTestSecondaryProfileFlag(DatPartyRecord *, u16);
extern s32 prfCheckRequirementRules(s32, ScrVmOperand *, u16);
extern u32 ptyGetProfileRecordValue(DatPartyRecord *, u16);

/* Build the 0x58-byte list entry for one profile, or NULL when it is not available (profile 0x4E is still
 * listed once flag 0x908 is set). The entry's state is 1 when requirement 1 passes or the profile flag is set,
 * 2 when only requirement 0 passes, and 3 otherwise. The scheduler argument is unused. */
MnuMantraGridEntry *mnuCreateMantraGridEntry(s32 unused, u16 profileId, MnuProfileProgress *selection) {
    MnuMantraGridEntry *entry;

    if (prfReqCheckWithFallback(selection->partyRecord, profileId) == 0) {
        if (profileId != 0x4E || mdlFlagTest(0x908) == 0) {
            return NULL;
        }
    }

    entry = sdfAllocSizeClassBlock(sizeof(MnuMantraGridEntry));
    memset(entry, 0, sizeof(MnuMantraGridEntry));
    entry->sceneId = profileId;
    entry->requiredLevel = prfGetRequiredProfileLevel(entry->sceneId);
    entry->backgroundColorIndex = prfGetProfileParameterByte5(entry->sceneId);
    prfBuildRawSkillList(entry->sceneId, &entry->skills);
    entry->cap = prfGetCapValue(entry->sceneId);
    entry->frame = 0x3C;
    entry->profileFlag = ptyTestSecondaryProfileFlag(selection->partyRecord, entry->sceneId);

    if (prfCheckRequirementRules(0, (ScrVmOperand *)selection->partyRecord, entry->sceneId) != 0 ||
        entry->sceneId == 0x4E) {
        if (prfCheckRequirementRules(1, (ScrVmOperand *)selection->partyRecord, entry->sceneId) != 0) {
            entry->state = MNU_MANTRA_GRID_ENTRY_SECOND_CHECK_OR_PROFILE_FLAG;
        } else if (entry->profileFlag != 0) {
            entry->state = MNU_MANTRA_GRID_ENTRY_SECOND_CHECK_OR_PROFILE_FLAG;
        } else {
            entry->state = MNU_MANTRA_GRID_ENTRY_FIRST_CHECK_OR_SPECIAL_PROFILE;
        }
    } else {
        entry->state = MNU_MANTRA_GRID_ENTRY_FIRST_PATH_REJECTED;
    }
    entry->value = ptyGetProfileRecordValue(selection->partyRecord, entry->sceneId);
    return entry;
}

extern MantraPrerequisiteRecord mnuMantraPrerequisiteRecords[];
extern void sdfGridSetCellValue(SdfGrid *grid, s32 column, s32 row, u32 cellValue);

/* Rebuild the 88-entry mantra grid from the prerequisite coordinates (tenths, or required level / 5
 * when the record's x is -1), report the selected profile's cell, then set the three
 * requirement-group bits in boundsFlags. */
void mnuBuildMantraGridAndGetProfileCoordinates(MenuSceneWork *work, s32 selectionAddress, s32 *outX, s32 *outY) {
    MnuProfileProgress *selection = (MnuProfileProgress *)selectionAddress;
    MnuMantraNodeState *nodes = (MnuMantraNodeState *)work->gridHandle->userData;
    MnuMantraGridEntry *entry;
    SdfGrid *grid;
    u32 column;
    u32 row;
    s32 foundX = 0;
    s32 foundY = 0;
    u32 i;

    memset(nodes, 0, 0x480);
    for (i = 1; i < 0x59; i++) {
        entry = mnuCreateMantraGridEntry((s32)work, i, selection);
        if (entry == NULL) {
            continue;
        }
        if (mnuMantraPrerequisiteRecords[i].x == -1) {
            column = entry->requiredLevel / 5;
            nodes[i].xRemainder = 0;
        } else {
            column = mnuMantraPrerequisiteRecords[i].x / 10;
            nodes[i].xRemainder = mnuMantraPrerequisiteRecords[i].x % 10;
        }
        row = mnuMantraPrerequisiteRecords[i].y / 10;
        grid = work->gridHandle;
        if (column >= grid->width) {
            column = grid->width - 1;
        }
        if (row >= grid->cellCount / grid->width) {
            row = grid->cellCount / grid->width - 1;
        }
        nodes[i].yRemainder = mnuMantraPrerequisiteRecords[i].y % 10;
        nodes[i].state = entry->state;
        nodes[i].y = row;
        nodes[i].x = column;
        sdfGridSetCellValue(grid, column, row, (u32)entry);
        if (entry->sceneId == selection->profileId) {
            foundX = column;
            foundY = row;
        }
    }
    if (outX != NULL) {
        *outX = foundX;
    }
    if (outY != NULL) {
        *outY = foundY;
    }
    {
        u16 groupA[3] = {0x13, 0x2E, 0x48};
        s8 groupB[4] = {'P', 'Q', 'R', 'S'};
        s8 groupC[2] = {'N', 'O'};
        s32 j;

        work->boundsFlags = 0;
        for (j = 0; j < 3; j++) {
            if (prfReqCheckWithFallback(selection->partyRecord, groupA[j])) {
                work->boundsFlags |= 1;
            }
        }
        for (j = 0; j < 4; j++) {
            if (prfReqCheckWithFallback(selection->partyRecord, groupB[j])) {
                work->boundsFlags |= 2;
            }
        }
        for (j = 0; j < 2; j++) {
            if (prfReqCheckWithFallback(selection->partyRecord, groupC[j])) {
                work->boundsFlags |= 4;
                if (j == 0) {
                    mdlFlagSet(0x908);
                }
            }
        }
        if (mdlFlagTest(0x908)) {
            work->boundsFlags |= 4;
        }
    }
}

/* Recreate the scene's selection grid and copy its resulting coordinates. */
void mnuReinitializeSceneGrid(s32 sceneAddress) {
    SdfGrid *oldGrid = ((MenuSceneWork *)sceneAddress)->gridHandle;

    sdfDestroyGridWork(oldGrid);
    mnuInitializeMantraSelectionGrid((MenuSceneWork *)(u32)sceneAddress);
    mnuCopySceneCoordinates((MenuSceneWork *)(u32)sceneAddress);
    /* Post-final-call exception: retail keeps jal + epilogue; this clear survives 01.sibling..11.cfg and is deleted in 13.life (cc1probe trace). */
    oldGrid = NULL; /* handle released by the first call */
}

extern void sdfGridClear(SdfGrid *grid);
extern void mnuBuildMantraGridAndGetProfileCoordinates(MenuSceneWork *work, s32 sceneId, s32 *x, s32 *y);
extern SdfGridCell *sdfGridSelectFilledCell(SdfGrid *grid, u32 x, u32 y);
extern SdfGridCell *sdfGridCursorLeftToFilledCell(SdfGrid *grid);


s32 mnuRestoreMantraGridSelection(MenuSceneWork *work) {
    SdfGrid *grid = work->gridHandle;
    SdfGridCell *cursor = grid->cursor;
    MnuMantraGridEntry *selected = (MnuMantraGridEntry *)(u32)cursor->value;
    u16 entryId = selected->sceneId;
    MnuMantraNodeState *entries = (MnuMantraNodeState *)grid->userData;
    s16 x = entries[entryId].x;
    s16 y = entries[entryId].y;
    s32 scene;
    s32 field;

    sdfGridClear(grid);
    scene = sdfGetTaskValueByKey(mnuSceneResourceContext, 0);
    field = *(s32 *)(*(s32 *)(scene + 0xC) + 0x1C);
    mnuBuildMantraGridAndGetProfileCoordinates(work, *(s32 *)(field + 0x70), NULL, NULL);
    if (sdfGridSelectFilledCell(
            work->gridHandle, x, y) == NULL) {
        sdfGridCursorLeftToFilledCell(work->gridHandle);
    }
}

/* Return the selected entry address through the scene-work/grid/slot chain, or zero when scene work is absent. */
MnuMantraGridEntry *fldGetSceneMetadataNode(void) {
    MenuSceneWork *scene = (MenuSceneWork *)(u32)sdfGetTaskValueByKey(mnuSceneResourceContext, 1);

    if (scene == NULL) {
        return 0;
    }
    return (MnuMantraGridEntry *)(u32)scene->gridHandle->cursor->value;
}

extern s32 evtCreateMessageWindowIfMissing(s32);
extern void *memcpy(void *, const void *, u32);

s32 mnuBeginMantraProfileSelection(void) {
    MnuMantraGridEntry *entry = fldGetSceneMetadataNode();
    MenuSceneMetadata *scene = (MenuSceneMetadata *)sdfGetTaskValueByKey(mnuSceneResourceContext, SDF_TASK_VALUE_USER_DATA_KEY);

    scene->state = MENU_SCENE_STATE_SELECTING;
    scene->pendingProfileId = entry->sceneId;
    dspCloseChannel();
    evtCreateMessageWindowIfMissing(scene->messageWindowResource);
    memcpy((u32 *)((u8 *)scene + 0x28), (u32 *)*(u32 *)mnuGetSelectedNodeValue(), 0x1A4);
    ((u8 *)scene)[0x7D] = (u8)scene->pendingProfileId;
    mnuSetupStaffMenuProfilePage((DatPartyRecord *)((u8 *)scene + 0x28), scene->attachedEffect);
    scene->stageFinished = 0;
    scene->stageStarted = 0;
    return 0;
}

extern s8 scrSelectOperandIndex(DatPartyRecord *, s32);
extern s8 scrGetSelectedOperandIndex(DatPartyRecord *);
extern void mnuSetMantraGridFeedbackMode(MnuGridFeedbackState *, u32);
void mnuFinishMantraProfileSelection(void) {
    MenuSceneMetadata *scene = (MenuSceneMetadata *)sdfGetTaskValueByKey(mnuSceneResourceContext, SDF_TASK_VALUE_USER_DATA_KEY);
    struct MenuProgressHost *effect;

    scene->messageShadeFrames = 0;
    if (scene->pendingProfileId != 0) {
        MnuProfileProgress *selection = (MnuProfileProgress *)mnuGetSelectedNodeValue();
        MenuSceneWork *work;

        scrSelectOperandIndex(selection->partyRecord, scene->pendingProfileId);
        selection->profileId = scrGetSelectedOperandIndex(selection->partyRecord);
        selection->value = ptyGetProfileRecordValue(selection->partyRecord, (u16)selection->profileId);
        selection->cap = prfGetCapValue((u16)selection->profileId);

        work = (MenuSceneWork *)sdfGetTaskValueByKey(mnuSceneResourceContext, 1);
        mnuReinitializeSceneGrid((s32)work);
        mnuSetMantraGridFeedbackMode(&work->gridFeedback, 1);
    }

    evtFinishMessageWindowAndNotify();
    effect = scene->attachedEffect;
    mnuReleaseMenuVisualWorkResources(effect);
    /* Post-final-call exception: retail keeps jal + epilogue; this clear survives 01.sibling..11.cfg and is deleted in 13.life (cc1probe trace). */
    effect = NULL; /* handle released by the call above */
}

extern s8 D_00324510[];
extern s32 mnuAreRequiredProfilesCapped(u16);
extern s32 mnuGetMantraSourceValue(u16);
extern void itfDspPopulatePrimaryLabels(void);
extern void itfDspPopulateAlternateLabels(void);
extern void itfDspPopulateThirdLabels(void);
extern void itfDspSignalA(void);
extern void itfDspSignalB(void);
extern void itfDspSignalC(void);
extern void itfDspSignalD(void);
extern void itfDspSignalE(void);
extern s8 evtGetCapturedWindowPanelValue(void);
s32 mnuUpdateMantraSceneState(void) {
    MnuMantraGridEntry *entry = fldGetSceneMetadataNode();
    MnuProfileProgress *selection = (MnuProfileProgress *)mnuGetSelectedNodeValue();
    MenuSceneMetadata *scene = (MenuSceneMetadata *)sdfGetTaskValueByKey(mnuSceneResourceContext, SDF_TASK_VALUE_USER_DATA_KEY);

    switch (scene->state) {
    case MENU_SCENE_STATE_SELECTING:
        if (D_00324510[0x21] < 0) {
            if (selection->profileId == scene->pendingProfileId) {
                if (prfGetCapValue(entry->sceneId) ==
                    ptyGetProfileRecordValue(selection->partyRecord, entry->sceneId)) {
                    scene->state = MENU_SCENE_STATE_SIGNAL_E;
                } else {
                    scene->state = MENU_SCENE_STATE_SIGNAL_B;
                }
                scene->pendingProfileId = 0;
            } else if (prfGetCapValue(entry->sceneId) ==
                       ptyGetProfileRecordValue(selection->partyRecord, entry->sceneId)) {
                scene->state = MENU_SCENE_STATE_SIGNAL_D;
                scene->pendingProfileId = 0;
            } else if (entry->state != MNU_MANTRA_GRID_ENTRY_SECOND_CHECK_OR_PROFILE_FLAG) {
                if (mnuAreRequiredProfilesCapped(entry->sceneId) == 0) {
                    scene->state = MENU_SCENE_STATE_SIGNAL_C;
                } else {
                    scene->state = MENU_SCENE_STATE_SIGNAL_A;
                }
                scene->pendingProfileId = 0;
            } else if (entry->profileFlag != 0) {
                scene->state = MENU_SCENE_STATE_ALTERNATE_LABELS;
            } else {
                scene->state = entry->state;
            }
        } else if (D_00324510[0x23] < 0 && scene->stageFinished != 0) {
            scene->state = MENU_SCENE_STATE_WAITING_FOR_MESSAGE_COMPLETION;
            scene->pendingProfileId = 0;
        }
        break;
    case MENU_SCENE_STATE_PRIMARY_LABELS:
        itfDspPopulatePrimaryLabels();
        scene->state = MENU_SCENE_STATE_WAITING_FOR_MESSAGE_WINDOW;
        break;
    case MENU_SCENE_STATE_ALTERNATE_LABELS:
        itfDspPopulateAlternateLabels();
        scene->state = MENU_SCENE_STATE_WAITING_FOR_MESSAGE_WINDOW;
        break;
    case MENU_SCENE_STATE_WAITING_FOR_MESSAGE_WINDOW:
        if (evtGetMessageWindowControlState() == 0) {
            if (evtGetCapturedWindowPanelValue() != 0) {
                scene->state = MENU_SCENE_STATE_WAITING_FOR_MESSAGE_COMPLETION;
                scene->pendingProfileId = 0;
            } else {
                scene->state = MENU_SCENE_STATE_THIRD_LABELS;
            }
        }
        break;
    case MENU_SCENE_STATE_THIRD_LABELS:
        itfDspPopulateThirdLabels();
        scene->state = MENU_SCENE_STATE_WAITING_FOR_MESSAGE_COMPLETION;
        if (entry->profileFlag != 0) {
            return 0;
        }
        datGameState->header.currency -= mnuGetMantraSourceValue(entry->sceneId);
        break;
    case MENU_SCENE_STATE_SIGNAL_A:
        itfDspSignalA();
        scene->state = MENU_SCENE_STATE_WAITING_FOR_MESSAGE_COMPLETION;
        break;
    case MENU_SCENE_STATE_SIGNAL_B:
        itfDspSignalB();
        scene->state = MENU_SCENE_STATE_WAITING_FOR_MESSAGE_COMPLETION;
        break;
    case MENU_SCENE_STATE_SIGNAL_C:
        itfDspSignalC();
        scene->state = MENU_SCENE_STATE_WAITING_FOR_MESSAGE_COMPLETION;
        break;
    case MENU_SCENE_STATE_SIGNAL_D:
        itfDspSignalD();
        scene->state = MENU_SCENE_STATE_WAITING_FOR_MESSAGE_COMPLETION;
        break;
    case MENU_SCENE_STATE_SIGNAL_E:
        itfDspSignalE();
        scene->state = MENU_SCENE_STATE_WAITING_FOR_MESSAGE_COMPLETION;
        break;
    case MENU_SCENE_STATE_WAITING_FOR_MESSAGE_COMPLETION:
        if (scene->stageFinished != 0 && evtGetMessageWindowControlState() == 0) {
            evtFinishMessageWindowAndNotify();
            sdfSetTaskItemMode(mnuSceneResourceContext, 1,
                               SDF_TASK_ITEM_MODE_PENDING_ACTIVATION);
            return -1;
        }
        break;
    default:
        break;
    }
    return 0;
}

extern void dspRequestMessageWindowModeOne(void);
extern void mnuDrawMantraSparkScene(MenuSceneMetadata *, s32, s32);
extern void mnuDrawAnimatedCurrencyCounter(s32, s32, s32, s32, MenuSceneMetadata *, s32);
extern s32 mnuDrawLoadedProgressPanels(void *control, MenuProgressHost *work, s32 context);
extern void evtStageTestSelectEntryWithoutInitialValue(u16 entryIndex, u32 assetOption);
struct SdfPoolNode;
extern s8 evtStageTestUpdate(struct SdfPoolNode **);
extern struct SdfPoolNode *D_00325818[4];
extern void uiDrawGradientColorRect(u32, u32, u32, u32, u32, const u32 *, u32);

/* Update attached visuals and stage completion, then ramp the message-window shade over ten frames.
 * Only the final two rectangle colors receive alpha; preserve both native selection lookups and the zero return. */
s32 mnuUpdateMantraSceneDisplay(void) {
    MenuSceneMetadata *sceneMetadata = (MenuSceneMetadata *)sdfGetTaskValueByKey(mnuSceneResourceContext, SDF_TASK_VALUE_USER_DATA_KEY);
    u32 shadeColors[4];

    mnuGetSelectedNodeValue();
    fldGetSceneMetadataNode();
    dspRequestMessageWindowModeOne();
    mnuDrawMantraSparkScene(sceneMetadata, 0x80, 0x4A);
    mnuDrawAnimatedCurrencyCounter(0, -27, 0, 0x80, sceneMetadata, MNU_SCENE_DRAW_CONTEXT);
    if (mnuDrawLoadedProgressPanels(&sceneMetadata->attachedEffectControl, sceneMetadata->attachedEffect, MNU_SCENE_DRAW_CONTEXT) != 0) {
        if (sceneMetadata->stageStarted == 0) {
            evtStageTestSelectEntryWithoutInitialValue(sceneMetadata->entryId, 0);
        }
        sceneMetadata->stageStarted = 1;
    }
    effUpdateAttached(0x1200, 0x730, 1, sceneMetadata->attachedEffect, MNU_SCENE_DRAW_CONTEXT);
    if (evtStageTestUpdate(D_00325818) >= 2) {
        sceneMetadata->stageFinished = 1;
    }
    if (evtGetMessageWindowControlState() != 0) {
        if (sceneMetadata->messageShadeFrames < MNU_SCENE_SHADE_FRAME_LIMIT) {
            sceneMetadata->messageShadeFrames++;
        }
    } else if (sceneMetadata->messageShadeFrames > 0) {
        sceneMetadata->messageShadeFrames--;
    }
    if (sceneMetadata->messageShadeFrames != 0) {
        memset(shadeColors, 0, sizeof(shadeColors));
        shadeColors[3] = shadeColors[2] = (u32)((f32)sceneMetadata->messageShadeFrames / MNU_SCENE_SHADE_FRAME_SCALE * MNU_SCENE_SHADE_ALPHA_SCALE);
        uiDrawGradientColorRect(0, 0x700, 2, 0x2000, 0x700, shadeColors, MNU_SCENE_DRAW_CONTEXT);
    }
    return 0;
}

/* Copy the active scene entry coordinates selected by the grid metadata. */
void fldUpdateSceneEntryMetadata(MenuSceneWork *scene) {
    MnuMantraGridEntry *selectedEntry =
        (MnuMantraGridEntry *)(u32)scene->gridHandle->cursor->value;
    u16 entryIndex = selectedEntry->sceneId;

    scene->entryPosition.x = mnuMantraEntrySpritePlacements[entryIndex].entryX;
    entryIndex = selectedEntry->sceneId;
    scene->entryPosition.y = mnuMantraEntrySpritePlacements[entryIndex].entryY;
}

/* Refresh the active scene-work entry coordinates and return zero. */
s32 fldResetSceneState(void) {
    fldUpdateSceneEntryMetadata((MenuSceneWork *)(u32)sdfGetTaskValueByKey(mnuSceneResourceContext, 1));
    return 0;
}

/* Update and copy scene coordinates before releasing its node list; retain the native call order. */
void mnuCopySceneCoordinatesAndReleaseNodeList(void) {
    s32 sceneAddress = sdfGetTaskValueByKey(mnuSceneResourceContext, 1);
    MnuSceneListHead *nodes;
    mnuUpdateMantraSelectionPositions((MenuSceneWork *)(u32)sceneAddress, 1);
    mnuCopySceneCoordinates((MenuSceneWork *)(u32)sceneAddress);
    nodes = &((MenuSceneWork *)(u32)sceneAddress)->costTransitionList;
    mnuReleaseListNodes(nodes);
    /* Post-final-call exception: retail keeps jal + epilogue; this clear survives 01.sibling..11.cfg and is deleted in 13.life (cc1probe trace). */
    nodes = NULL; /* list released by the call above */
}


extern void mnuStopResourceAnimation(void);
extern void mnuResetResourceAnimation(void);
extern s32 mnuRestoreMantraGridSelection(MenuSceneWork *work);

void mnuBeginTransitionAlt(MenuSceneWork *work, s32 mode) {
    MnuSceneListNode *transition = mnuAppendNodeToDisplayList(&work->costTransitionList);

    if (transition != NULL) {
        if (work->transitionBlendCounter >= 6) {
            mnuStopResourceAnimation();
            mnuRestoreMantraGridSelection(work);
        } else if (work->transitionBlendCounter < -5) {
            mnuResetResourceAnimation();
            mnuRestoreMantraGridSelection(work);
        }
        transition->payload.transition.mode = mode;
        if (mode == 1) {
            work->transitionBlendCounter = 10;
            transition->payload.transition.fromValue = *mnuGetSelectedNodeValue();
            mnuStopResourceAnimation();
            transition->payload.transition.toValue = *mnuGetSelectedNodeValue();
            mnuResetResourceAnimation();
            return;
        }
        if (mode == 2) {
            work->transitionBlendCounter = -10;
            transition->payload.transition.fromValue = *mnuGetSelectedNodeValue();
            mnuResetResourceAnimation();
            transition->payload.transition.toValue = *mnuGetSelectedNodeValue();
            mnuStopResourceAnimation();
        }
    }
}


extern void mnuDrawPlacedSpriteFromTable(s32 x, s32 y, s32 z, s32 alpha, s32 placementIndex, s32 context);
/* Shared draw-callback helper: the retail draw wrappers reach the placed-sprite
 * provider through an operation callback and keep `jal` + epilogue tails. */
static inline void mnuDrawPlacedSpriteVia(void (*draw)(s32, s32, s32, s32, s32, s32), s32 x, s32 y, s32 z, s32 alpha, s32 placementIndex, s32 context) {
    draw(x, y, z, alpha, placementIndex, context);
}
extern TaskWork *mnuSceneResourceContext;
extern void mnuDrawScaledPlacedSprite(s32, s32, s32, s32, s32, s32, f32, f32);
extern void mnuDrawMantraProfileSelectionSurface(s32, s32, s32, s32, MenuSceneWork *, s32);
extern void mnuChooseDisplaySpriteKindFromEntryFlags(MenuSceneWork *, s32, s32);
extern void mnuDrawMantraScenePlacementSprite(s32, s32, s32, s32, s32);

typedef struct {
    u8 pad00[0x38];
    s32 shade; /* 0x38 */
} DspEffectShade;

typedef struct {
    u8 pad00[0x828];
    DspEffectShade *shade; /* 0x828 */
} DspEffectRoot;

typedef struct {
    u8 pad00[0x1C];
    s32 phase;
    s32 frame;
    DspEffectRoot *effectRoot; /* 0x24 */
} DspTableScene;

extern s8 D_00324510[64];
extern void mnuStopResourceAnimation(void);
extern void mnuResetResourceAnimation(void);
extern s32 mnuRestoreMantraGridSelection(MenuSceneWork *);
extern s32 mnuScheduleUnlockedMantraMessages(MenuSceneWork *);
extern void mnuUpdateMantraSelectionCursorPosition(MenuSceneWork *);
extern void mnuBeginTransitionAlt(MenuSceneWork *, s32);
extern SdfGridCell *sdfGridCursorUpToFilledCell(SdfGrid *);
extern SdfGridCell *sdfGridCursorDownToFilledCell(SdfGrid *);
extern s32 mnuSelectMantraGridLeftNeighbor(SdfGrid *);
extern s32 mnuSelectMantraGridRightNeighbor(SdfGrid *);
extern u8 D_0036B410[17][15];
extern s32 mnuSelectMantraGridLeftCell(SdfGrid *, s32, s32, u32);
extern SdfGridCell *sdfGridCursorLeftToFilledCell(SdfGrid *);
extern s32 mnuSelectMantraGridRightCell(SdfGrid *, s32, s32, u32);
extern SdfGridCell *sdfGridCursorRightToFilledCell(SdfGrid *);
extern SdfGridCell *sdfGridCursorLeftToFilledCell(SdfGrid *);
extern SdfGridCell *sdfGridCursorRightToFilledCell(SdfGrid *);

s32 mnuHandleMantraSceneInput(void) {
    DspTableScene *host;
    MenuSceneWork *work;
    s32 feedback = 0;
    u32 direction;

    host = (DspTableScene *)sdfGetTaskValueByKey(mnuSceneResourceContext,
        SDF_TASK_VALUE_USER_DATA_KEY);
    work = (MenuSceneWork *)sdfGetTaskValueByKey(mnuSceneResourceContext, 1);
    switch (host->phase) {
    case 0:
        if (host->frame < 10) {
            host->frame++;
        } else {
            host->phase = 1;
            host->frame = 0;
        }
        return 0;
    case 2:
        if (host->frame < 10) {
            host->frame++;
            return 0;
        }
        host->frame = 0;
        host->phase = 0;
        sdfSetTaskItemMode(mnuSceneResourceContext, 1, 1);
        return -1;
    case 1:
        if (work->unk52C != 0) {
            host->phase = 2;
            host->frame = 0;
            work->pendingPhase13 = 1;
            work->scenePhase = 10;
            return 0;
        }
        if (work->transitionBlendCounter > 0) {
            work->transitionBlendCounter--;
            if (work->transitionBlendCounter == 5) {
                mnuStopResourceAnimation();
                mnuRestoreMantraGridSelection(work);
                work->unk52C = mnuScheduleUnlockedMantraMessages(work);
                if (work->unk52C != 0) {
                    work->transitionBlendCounter = 0;
                }
            }
        } else if (work->transitionBlendCounter < 0) {
            work->transitionBlendCounter++;
            if (work->transitionBlendCounter == -5) {
                mnuResetResourceAnimation();
                mnuRestoreMantraGridSelection(work);
                work->unk52C = mnuScheduleUnlockedMantraMessages(work);
                if (work->unk52C != 0) {
                    work->transitionBlendCounter = 0;
                }
            }
        }
        break;
    }
    mnuUpdateMantraSelectionCursorPosition(work);
    if (D_00324510[0x26] != 0) {
        work->cursorInputMask = 0x80;
    } else if (D_00324510[0x27] != 0) {
        work->cursorInputMask = 0x40;
    } else if (D_00324510[0x24] != 0) {
        work->cursorInputMask = 0x20;
    } else if (D_00324510[0x25] != 0) {
        work->cursorInputMask = 0x10;
    } else {
        work->cursorInputMask = 0;
    }
    direction = work->cursorInputMask;
    if (work->cursorMoving == 0) {
        if ((D_00324510[0x26] & 2) && (direction & 0x80)) {
            if (sdfGridCursorUpToFilledCell(work->gridHandle) != NULL) {
                feedback = 1;
            }
        } else if ((D_00324510[0x27] & 2) && (direction & 0x40)) {
            if (sdfGridCursorDownToFilledCell(work->gridHandle) != NULL) {
                feedback = 1;
            }
        } else if ((D_00324510[0x24] & 2) && (direction & 0x20)) {
            if (mnuSelectMantraGridLeftNeighbor(work->gridHandle) != 0) {
                feedback = 1;
            }
        } else if ((D_00324510[0x25] & 2) && (direction & 0x10)) {
            if (mnuSelectMantraGridRightNeighbor(work->gridHandle) != 0) {
                feedback = 1;
            }
        } else if (D_00324510[0x28] < 0) {
            mnuBeginTransitionAlt(work, 1);
            feedback = 4;
        } else if (D_00324510[0x2A] < 0) {
            mnuBeginTransitionAlt(work, 2);
            feedback = 4;
        }
    }
    if ((D_00324510[0x23] < 0 || D_00324510[0x20] < 0 ||
        D_00324510[0x21] < 0) && work->transitionBlendCounter == 0) {
        host->frame = 0;
        host->phase = 2;
        feedback = 3;
    }
    if (feedback != 0) {
        switch (feedback) {
        case 1:
            sndSetSequenceVolumePan(2, 127, 63);
            break;
        case 2:
            sndSetSequenceVolumePan(8, 127, 63);
            break;
        case 3:
            sndSetSequenceVolumePan(10, 127, 63);
            break;
        case 4:
            sndSetSequenceVolumePan(4, 127, 63);
            break;
        }
    }
    return 0;
}

extern void uiDrawGradientColorRect(u32, u32, u32, u32, u32, const u32 *, u32);

void mnuDrawPanelWithPackedColorPattern(s32 x, s32 y, s32 width, s32 fade, s32 t0) {
    u32 arr[4];
    s32 v;

    v = (s32)(((f32)(fade << 4)) * 0.0078125f);
    v |= 0x0A050700;
    arr[0] = v;
    arr[1] = v;
    arr[2] = v;
    arr[3] = v;
    y += 0x32;
    y *= 8;
    uiDrawGradientColorRect(x * 16, y, width, 0x2000, 0xC70, arr, t0);
}

/* Fill-level dispatch: direct, inverted, or sin-pulsed, then the shared layers. */
s32 mnuDrawMantraCostTransition(void) {
    s32 nodePrev;
    s32 nodeNext;
    MenuSceneWork *scene;
    s32 level;
    s32 scaled;
    s32 raw;
    f32 frac;
    u32 *cost;

    frac = 0.0f;
    mnuGetSelectedNodeValue();
    nodePrev = sdfGetTaskValueByKey(mnuSceneResourceContext, SDF_TASK_VALUE_USER_DATA_KEY);
    nodeNext = sdfGetTaskValueByKey(mnuSceneResourceContext, 1);
    scene = (MenuSceneWork *)(u32)nodeNext;
    mnuDrawMantraSparkScene((MenuSceneMetadata *)nodePrev, 0x80, 0x52);
    level = *(s32 *)(nodePrev + 0x1C);
    switch (level) {
    case 0:
        frac = (f32)*(s32 *)(nodePrev + 0x20) / 10.0f;
        scaled = (s32)(frac * 128.0f);
        mnuDrawScaledPlacedSprite(0, 0, 0, scaled, 0x3A, 0x53, 1.5f, 1.5f);
        mnuDrawPanelWithPackedColorPattern(0, 0, 0, scaled, 0x53);
        mnuDrawMantraProfileSelectionSurface(0, 0, 1, scaled, scene, 0x53);
        mnuDrawMantraScenePlacementSprite(0, 0, 1, 0x80, 0x53);
        mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0, 0, 0, 0x80, 0xF, 0x53);
        mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0, 0, 0, scaled, 0x5B, 0x53);
        cost = mnuGetSelectedNodeValue();
        mnuDrawMantraCostAfterListAdvance(*cost, &scene->costTransitionList, scaled, 0x53);
        mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0x20, 0, 0, scaled, 0xB, 0x53);
        break;
    case 2:
        frac = (f32)*(s32 *)(nodePrev + 0x20) / 10.0f;
        frac = 1.0f - frac;
        scaled = (s32)(frac * 128.0f);
        mnuDrawScaledPlacedSprite(0, 0, 0, scaled, 0x3A, 0x53, 1.5f, 1.5f);
        mnuDrawPanelWithPackedColorPattern(0, 0, 0, scaled, 0x53);
        mnuDrawMantraProfileSelectionSurface(0, 0, 1, scaled, scene, 0x53);
        mnuDrawMantraScenePlacementSprite(0, 0, 0, 0x80, 0x53);
        mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0, 0, 0, 0x80, 0xF, 0x53);
        mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0, 0, 0, scaled, 0x5B, 0x53);
        cost = mnuGetSelectedNodeValue();
        mnuDrawMantraCostAfterListAdvance(*cost, &scene->costTransitionList, scaled, 0x53);
        mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0x20, 0, 0, scaled, 0xB, 0x53);
        break;
    case 1:
        /* frac keeps its 0.0f initializer when raw == 0. */
        raw = scene->transitionBlendCounter;
        if (raw > 0) {
            frac = (f32)raw / 10.0f;
        } else if (raw < 0) {
            frac = (f32)(-raw) / 10.0f;
        }
        frac = 1.0f - sdfSinPoly(frac * 3.14159265f);
        mnuDrawScaledPlacedSprite(0, 0, 0, 0x80, 0x3A, 0x53, 1.5f, 1.5f);
        mnuDrawPanelWithPackedColorPattern(0, 0, 0, 0x80, 0x53);
        mnuDrawMantraProfileSelectionSurface(0, 0, 1, (s32)(frac * 128.0f), scene, 0x53);
        mnuDrawMantraScenePlacementSprite(0, 0, 1, 0x80, 0x53);
        mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0, 0, 0, 0x80, 0xF, 0x53);
        mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0, 0, 0, 0x80, 0x5B, 0x53);
        cost = mnuGetSelectedNodeValue();
        mnuDrawMantraCostAfterListAdvance(*cost, &scene->costTransitionList, 0x80, 0x53);
        mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0x20, 0, 0, 0x80, 0xB, 0x53);
        break;
    default:
        break;
    }
    mnuChooseDisplaySpriteKindFromEntryFlags(scene, 0x80, 0x53);
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0, 0, 0, 0x80, 0xD, 0x53);
    return 0;
}

extern void itfDspDrawMarksA(s32, s32);
/* Shared draw-callback helper for the TU's eight itfDspDrawMarksA sites (retail keeps jal + epilogue tails, like mnuDrawPlacedSpriteVia). */
static inline void itfDrawMarksVia(void (*draw)(s32, s32), s32 scale, s32 context) {
    draw(scale, context);
}

void mnuDrawDisplaySpriteAndPanelMarks(MenuSceneMetadata *scene, s32 alpha, s32 context) {
    mnuDrawMantraSparkScene(scene, alpha, context);
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0, 0, 0, alpha, 0x5A, context);
    itfDrawMarksVia(itfDspDrawMarksA, alpha, context);
}

/* Three sprite layers drawn at the origin for one draw context. */
void mnuDrawMantraSceneSpriteSequence(s32 scale, s32 context) {
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0, 0, 0, scale, 0x4F, context);
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0, 0, 0, scale, 0xC, context);
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, 0, 0, 0, scale, 0xD, context);
}

void mnuDrawMantraScenePlacementSprite(s32 x, s32 y, s32 z, s32 drawContext, s32 drawArgument) {
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, x, y, z, drawContext, 14, drawArgument);
}

void mnuDrawMantraSceneSpriteGroup(s32 x, s32 y, s32 z, s32 alpha, s32 context) {
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, x, y, z, alpha, 0x47, context);
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, x, y, z, alpha, 0x48, context);
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, x, y, z, alpha, 0x49, context);
}

typedef struct {
    s8 spriteIndices[7];
} DspUnitSpriteTable;


typedef struct {
    u8 pad00[4];
    u16 unitId; /* 0x4 */
} DspTablePartyRecord;

typedef struct {
    DspTablePartyRecord *partyRecord; /* 0x0 */
    s32 profileId;                    /* 0x4: index into the 19-byte mantra-name rows */
} DspTableProgress;

typedef struct {
    u8 pad00[0x1C];
    struct { u8 pad00[0x70]; DspTableProgress *progress; } *selectedNode; /* 0x1C */
    u8 pad20[0x10];
    s32 *selectedValue; /* 0x30 */
} DspTableUnit;

typedef struct {
    u8 pad00[0xC];
    DspTableUnit *unit; /* 0xC */
} DspTableWindowContext;


extern DspUnitSpriteTable D_003BC440[];
extern DspMantraName *D_003BAA78;
extern void *memset(void *, s32, u32);
extern u32 strlen(const char *);
extern s32 frFontQueueTextAndOptionallyMeasure(s32 x, s32 y, u32 first, u32 second, s8 type, void *name, s32 flag, s32 option);

/* Draw the selected mantra's display: sprite, fade-driven effect and its name (shifted left for long names). */
void mnuDrawSelectedMantraProfile(s32 x, s32 y, s32 z, s32 alpha, DspTableWindowContext *windowContext, s32 drawContext) {
    DspUnitSpriteTable sprites = D_003BC440[0];
    DspTableUnit *unit = windowContext->unit;
    s32 *selectedValue = unit->selectedValue;
    DspTableProgress *progress = unit->selectedNode->progress;
    DspTableScene *scene = (DspTableScene *)sdfGetTaskValueByKey(mnuSceneResourceContext, SDF_TASK_VALUE_USER_DATA_KEY);
    u8 scratch[0x20];

    selectedValue[1] = alpha;
    mnuDrawPlacedSpriteVia(mnuDrawPlacedSpriteFromTable, x, y, z, alpha, sprites.spriteIndices[progress->partyRecord->unitId] + 0x4A, drawContext);
    scene->effectRoot->shade->shade = (s32)((f32)(alpha << 8) * 0.0078125f);
    effUpdateAttached(0xD80, 0xC30, 1, scene->effectRoot, 0x53);
    memset(scratch, 0, sizeof(scratch));
    if (strlen((const char *)(D_003BAA78 + progress->profileId)) >= 5) {
        frFontQueueTextAndOptionallyMeasure(x + 0xEF, y + 0x15E, z, alpha | 0xA09DC300, 0, D_003BAA78 + progress->profileId, 0x80000000, drawContext);
    } else {
        frFontQueueTextAndOptionallyMeasure(x + 0xF8, y + 0x15E, z, alpha | 0xA09DC300, 0, D_003BAA78 + progress->profileId, 0x80000000, drawContext);
    }
}

INCLUDE_SDATA(const s32, "game/code_00250E88", D_003BC440);

