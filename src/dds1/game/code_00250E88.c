#include "dsp_name.h"
#include "sdf_resource.h"
#include "mnu.h"
#include "dat_state.h"
#include "mnu_profile_progress.h"
#include "mnu_mantra_grid.h"
#include "mnu_scene_work.h"
#include "sdf_task_work.h"

#define MNU_SCENE_WORK_SIZE 0x5B0
#define MNU_SCENE_SHADE_FRAME_LIMIT 10
#define MNU_SCENE_SHADE_FRAME_SCALE 10.0f
#define MNU_SCENE_SHADE_ALPHA_SCALE 112.0f
#define MNU_SCENE_DRAW_CONTEXT 0x53

extern void func_002512F0(s32, s32);

extern void mnuInitializeMantraSelectionGrid(MenuSceneWork *);

extern void mnuCopySceneCoordinates(MenuSceneWork *);

extern void sdfDestroyGridWork(SdfGrid *);

extern void mnuResetWorkFloats(void);


extern u32 mnuSceneResourceContext;

typedef struct {
    u32 unk0;
    u16 entryX;
    u16 entryY;
    u32 unk8;
} SceneEntry;

extern SceneEntry D_0036BE38[];

typedef s16 MnuVariantSpritePlacement[6];
extern MnuVariantSpritePlacement D_0036B7F0[];

typedef struct ScrVmOperand ScrVmOperand;

typedef struct MenuGridCoordinate {
    s16 x;
    s16 y;
    u8 pad04[8];
} MenuGridCoordinate;

extern void sdfReleaseChipBlock(void *);

void func_00250E88(s32 *xCoordinate, s32 *yCoordinate, u16 index,
                   s32 maximumX, s32 maximumY) {
    s32 x = *xCoordinate;
    s32 deltaX = D_0036B7F0[index][2] - x;
    s32 deltaY = D_0036B7F0[index][3] - *yCoordinate;

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

INCLUDE_ASM(const s32, "game/code_00250E88", func_00250F60);

void func_00251260(MenuSceneWork *work) {
    MnuMantraGridEntry *entry =
        (MnuMantraGridEntry *)(u32)work->gridHandle->cursor->value;
    s32 maximumX = 0x307;
    u8 flags = work->boundsFlags;
    s32 position[2];

    if ((flags & 4) == 0) {
        maximumX = 0x2C8;
        if ((flags & 2) == 0) {
            maximumX = (flags & 1) != 0 ? 0x24C : 0x1BE;
        }
    }
    position[0] = work->scrollX;
    position[1] = work->scrollY;
    func_00250E88(&position[0], &position[1], entry->sceneId, maximumX, 0x38E);
    work->scrollX = position[0];
    work->scrollY = position[1];
}

INCLUDE_ASM(const s32, "game/code_00250E88", func_002512F0);

/* Release a nonnull grid-cell payload; the cell index is unused. */
void mnuFreeTaskData(u32 unused, u32 taskData) {
    if (taskData != 0) {
        sdfReleaseChipBlock((void *)taskData);
    }
}

INCLUDE_ASM(const s32, "game/code_00250E88", func_002515F0);

typedef struct DspUnitName {
    u8 encodedText[17];
} DspUnitName;


extern DspUnitName *D_003BAA70;
extern DspMantraName *D_003BAA78;
extern const s32 D_003AF840[];
extern const s32 D_003AF850[];
extern void *memcpy(void *, const void *, u32);
extern u32 mnuGetSelectedNodeValue(void);
extern MnuMantraGridEntry *fldGetSceneMetadataNode(void);
extern void evtCopyEntryStringToActiveWindow(s32, s32);
extern s32 dspStartEntry(s32);
extern SdfGridCell *sdfGridSelectFilledCell(SdfGrid *, s32, s32);

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
s32 mnuCreateSceneWork(void) {
    struct SdfMemBlock *allocation = sdfAllocGeneralBlock(MNU_SCENE_WORK_SIZE);
    u8 *sceneWork = (u8 *)sdfMemoryGetBlockAddress(allocation);

    memset(sceneWork, 0, MNU_SCENE_WORK_SIZE);
    ((MenuSceneWork *)sceneWork)->allocation = allocation;
    mnuInitializeMantraSelectionGrid((MenuSceneWork *)sceneWork);
    ((MenuSceneWork *)sceneWork)->scenePhase = 0;
    ((MenuSceneWork *)sceneWork)->phaseFrame = 0;
    ((MenuSceneMetadata *)sdfGetTaskValueByKey((TaskWork *)mnuSceneResourceContext, SDF_TASK_VALUE_USER_DATA_KEY))->displayedCurrency = datGameState->header.currency;
    mnuCopySceneCoordinates((MenuSceneWork *)sceneWork);
    return (s32)sceneWork;
}

/* Retain the native metadata lookup, then release grid/list/allocation resources and reset projection state. */
void mnuReleaseSceneContext(s32 unused, s32 sceneAddress) {
    sdfGetTaskValueByKey((TaskWork *)mnuSceneResourceContext, SDF_TASK_VALUE_USER_DATA_KEY);
    sdfDestroyGridWork(((MenuSceneWork *)sceneAddress)->gridHandle);
    mnuReleaseDisplayListNodes(
        &((MenuSceneWork *)(u32)sceneAddress)->sceneTransitionList);
    sdfReleaseResourceAllocation(((MenuSceneWork *)sceneAddress)->allocation);
    mnuResetWorkFloats();
}

INCLUDE_RODATA(const s32, "game/code_00250E88", D_003AF810);

INCLUDE_RODATA(const s32, "game/code_00250E88", D_003AF830);

INCLUDE_RODATA(const s32, "game/code_00250E88", D_003AF840);

INCLUDE_RODATA(const s32, "game/code_00250E88", D_003AF850);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00251A38);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00251E38);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00252CE8);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00252E38);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00252F88);

extern u8 D_0036B410[17][15];
extern s32 func_00252E38(SdfGrid *, s32, s32, u32);
extern SdfGridCell *func_002CC238(SdfGrid *);

s32 func_00253018(SdfGrid *grid) {
    MenuSceneWork *scene = (MenuSceneWork *)sdfGetTaskValueByKey(
        (TaskWork *)mnuSceneResourceContext, 1);
    MnuMantraGridEntry *entry = (MnuMantraGridEntry *)grid->cursor->value;
    MenuGridCoordinate *coordinates = (MenuGridCoordinate *)grid->userData;
    s32 x = coordinates[entry->sceneId].x;
    s32 y = coordinates[entry->sceneId].y;

    if ((x != 10 || scene->boundsFlags == 0) &&
        (D_0036B410[y][x] & 0x3C) != 0) {
        if (func_00252E38(grid, x, y, 0) != 0) {
            return 1;
        }
    } else {
        if (func_002CC238(grid) != NULL) {
            return 1;
        }
    }
    return 0;
}

extern s32 prfReqCheckWithFallback(ScrVmOperand *, u16);
extern void *sdfAllocSizeClassBlock(s32);
extern u16 prfGetRequiredProfileLevel(u16);
extern u8 prfGetParamWord7b5(u16);
extern s32 prfBuildRawSkillList(u16, PrfSkillList *);
extern u32 prfGetCapValue(u16);
extern s32 ptyTestProfileFlag1(DatPartyRecord *, u16);
extern s32 prfReq54Evaluate(s32, ScrVmOperand *, u16);
extern u32 ptyGetProfileRecordValue(DatPartyRecord *, u16);
extern s32 mdlFlagTest(s32);

/* Build the 0x58-byte list entry for one profile, or NULL when it is not available (profile 0x4E is still
 * listed once flag 0x908 is set). The entry's state is 1 when requirement 1 passes or the profile flag is set,
 * 2 when only requirement 0 passes, and 3 otherwise. The scheduler argument is unused. */
MnuMantraGridEntry *func_002530D8(s32 unused, u16 profileId, MnuProfileProgress *selection) {
    MnuMantraGridEntry *entry;

    if (prfReqCheckWithFallback((ScrVmOperand *)selection->partyRecord, profileId) == 0) {
        if (profileId != 0x4E || mdlFlagTest(0x908) == 0) {
            return NULL;
        }
    }

    entry = sdfAllocSizeClassBlock(sizeof(MnuMantraGridEntry));
    memset(entry, 0, sizeof(MnuMantraGridEntry));
    entry->sceneId = profileId;
    entry->requiredLevel = prfGetRequiredProfileLevel(entry->sceneId);
    entry->value05 = prfGetParamWord7b5(entry->sceneId);
    prfBuildRawSkillList(entry->sceneId, &entry->skills);
    entry->cap = prfGetCapValue(entry->sceneId);
    entry->frame = 0x3C;
    entry->profileFlag = ptyTestProfileFlag1(selection->partyRecord, entry->sceneId);

    if (prfReq54Evaluate(0, (ScrVmOperand *)selection->partyRecord, entry->sceneId) != 0 ||
        entry->sceneId == 0x4E) {
        if (prfReq54Evaluate(1, (ScrVmOperand *)selection->partyRecord, entry->sceneId) != 0) {
            entry->state = 1;
        } else if (entry->profileFlag != 0) {
            entry->state = 1;
        } else {
            entry->state = 2;
        }
    } else {
        entry->state = 3;
    }
    entry->value = ptyGetProfileRecordValue(selection->partyRecord, entry->sceneId);
    return entry;
}

INCLUDE_ASM(const s32, "game/code_00250E88", func_00253208);

/* Recreate the scene's selection grid and copy its resulting coordinates. */
void mnuReinitializeSceneGrid(s32 sceneAddress) {
    sdfDestroyGridWork(((MenuSceneWork *)sceneAddress)->gridHandle);
    mnuInitializeMantraSelectionGrid((MenuSceneWork *)(u32)sceneAddress);
    mnuCopySceneCoordinates((MenuSceneWork *)(u32)sceneAddress);
}

extern void func_002CBB48(SdfGrid *grid);
extern void func_00253208(s32 context, s32 sceneId, s32 *x, s32 *y);
extern SdfGridCell *sdfGridSelectFilledCell(SdfGrid *grid, s32 x, s32 y);
extern void func_002CC0D0(SdfGrid *grid);


void func_00253558(s32 context) {
    SdfGrid *grid = ((MenuSceneWork *)context)->gridHandle;
    SdfGridCell *cursor = grid->cursor;
    MnuMantraGridEntry *selected = (MnuMantraGridEntry *)(u32)cursor->value;
    u16 entryId = selected->sceneId;
    MenuGridCoordinate *entries = (MenuGridCoordinate *)grid->userData;
    s16 x = entries[entryId].x;
    s16 y = entries[entryId].y;
    s32 scene;
    s32 field;

    func_002CBB48(grid);
    scene = sdfGetTaskValueByKey((TaskWork *)mnuSceneResourceContext, 0);
    field = *(s32 *)(*(s32 *)(scene + 0xC) + 0x1C);
    func_00253208(context, *(s32 *)(field + 0x70), NULL, NULL);
    if (sdfGridSelectFilledCell(
            ((MenuSceneWork *)context)->gridHandle, x, y) == NULL) {
        func_002CC0D0(((MenuSceneWork *)context)->gridHandle);
    }
}

/* Return the selected entry address through the scene-work/grid/slot chain, or zero when scene work is absent. */
MnuMantraGridEntry *fldGetSceneMetadataNode(void) {
    MenuSceneWork *scene = (MenuSceneWork *)(u32)sdfGetTaskValueByKey((TaskWork *)mnuSceneResourceContext, 1);

    if (scene == NULL) {
        return 0;
    }
    return (MnuMantraGridEntry *)(u32)scene->gridHandle->cursor->value;
}

extern s32 dspCloseChannel(void);
extern s32 evtCreateMessageWindowIfMissing(s32);
extern void mnuSetupStaffMenuProfilePage(DatPartyRecord *, void *);
extern u32 mnuGetSelectedNodeValue(void);
extern void *memcpy(void *, const void *, u32);

s32 func_00253640(void) {
    MnuMantraGridEntry *entry = fldGetSceneMetadataNode();
    MenuSceneMetadata *scene = (MenuSceneMetadata *)sdfGetTaskValueByKey((TaskWork *)mnuSceneResourceContext, SDF_TASK_VALUE_USER_DATA_KEY);

    scene->state = 0;
    scene->pendingProfileId = entry->sceneId;
    dspCloseChannel();
    evtCreateMessageWindowIfMissing(scene->messageWindowResource);
    memcpy((u32 *)((u8 *)scene + 0x28), (u32 *)*(u32 *)mnuGetSelectedNodeValue(), 0x1A4);
    ((u8 *)scene)[0x7D] = (u8)scene->pendingProfileId;
    mnuSetupStaffMenuProfilePage((DatPartyRecord *)((u8 *)scene + 0x28), (void *)scene->attachedEffect);
    scene->stageFinished = 0;
    scene->stageStarted = 0;
    return 0;
}

extern s8 scrSelectOperandIndex(DatPartyRecord *, s32);
extern s8 scrGetSelectedOperandIndex(DatPartyRecord *);
extern void func_00258AF0(u32 *, u32);
extern void evtFinishMessageWindowAndNotify(void);
extern void mnuReleaseMenuVisualWorkResources(s32);

void func_00253778(void) {
    MenuSceneMetadata *scene = (MenuSceneMetadata *)sdfGetTaskValueByKey((TaskWork *)mnuSceneResourceContext, SDF_TASK_VALUE_USER_DATA_KEY);

    scene->messageShadeFrames = 0;
    if (scene->pendingProfileId != 0) {
        MnuProfileProgress *selection = (MnuProfileProgress *)mnuGetSelectedNodeValue();
        MenuSceneWork *work;

        scrSelectOperandIndex(selection->partyRecord, scene->pendingProfileId);
        selection->profileId = scrGetSelectedOperandIndex(selection->partyRecord);
        selection->value = ptyGetProfileRecordValue(selection->partyRecord, (u16)selection->profileId);
        selection->cap = prfGetCapValue((u16)selection->profileId);

        work = (MenuSceneWork *)sdfGetTaskValueByKey((TaskWork *)mnuSceneResourceContext, 1);
        mnuReinitializeSceneGrid((s32)work);
        func_00258AF0(work->gridRefreshControl, 1);
    }

    evtFinishMessageWindowAndNotify();
    mnuReleaseMenuVisualWorkResources(scene->attachedEffect);
}

extern s8 D_00324510[];
extern s32 func_002508D8(u16);
extern s32 mnuGetMantraSourceValue(u16);
extern void itfDspPopulatePrimaryLabels(void);
extern void itfDspPopulateAlternateLabels(void);
extern void itfDspPopulateThirdLabels(void);
extern void itfDspSignalA(void);
extern void itfDspSignalB(void);
extern void itfDspSignalC(void);
extern void itfDspSignalD(void);
extern void itfDspSignalE(void);
extern s64 evtGetMessageWindowControlState(void);
extern s8 evtGetCapturedWindowPanelValue(void);
s32 func_00253830(void) {
    MnuMantraGridEntry *entry = fldGetSceneMetadataNode();
    MnuProfileProgress *selection = (MnuProfileProgress *)mnuGetSelectedNodeValue();
    MenuSceneMetadata *scene = (MenuSceneMetadata *)sdfGetTaskValueByKey((TaskWork *)mnuSceneResourceContext, SDF_TASK_VALUE_USER_DATA_KEY);

    switch (scene->state) {
    case 0:
        if (D_00324510[0x21] < 0) {
            if (selection->profileId == scene->pendingProfileId) {
                if (prfGetCapValue(entry->sceneId) ==
                    ptyGetProfileRecordValue(selection->partyRecord, entry->sceneId)) {
                    scene->state = 8;
                } else {
                    scene->state = 5;
                }
                scene->pendingProfileId = 0;
            } else if (prfGetCapValue(entry->sceneId) ==
                       ptyGetProfileRecordValue(selection->partyRecord, entry->sceneId)) {
                scene->state = 7;
                scene->pendingProfileId = 0;
            } else if (entry->state != 1) {
                if (func_002508D8(entry->sceneId) == 0) {
                    scene->state = 6;
                } else {
                    scene->state = 4;
                }
                scene->pendingProfileId = 0;
            } else if (entry->profileFlag != 0) {
                scene->state = 2;
            } else {
                scene->state = entry->state;
            }
        } else if (D_00324510[0x23] < 0 && scene->stageFinished != 0) {
            scene->state = 10;
            scene->pendingProfileId = 0;
        }
        break;
    case 1:
        itfDspPopulatePrimaryLabels();
        scene->state = 9;
        break;
    case 2:
        itfDspPopulateAlternateLabels();
        scene->state = 9;
        break;
    case 9:
        if (evtGetMessageWindowControlState() == 0) {
            if (evtGetCapturedWindowPanelValue() != 0) {
                scene->state = 10;
                scene->pendingProfileId = 0;
            } else {
                scene->state = 3;
            }
        }
        break;
    case 3:
        itfDspPopulateThirdLabels();
        scene->state = 10;
        if (entry->profileFlag != 0) {
            return 0;
        }
        datGameState->header.currency -= mnuGetMantraSourceValue(entry->sceneId);
        break;
    case 4:
        itfDspSignalA();
        scene->state = 10;
        break;
    case 5:
        itfDspSignalB();
        scene->state = 10;
        break;
    case 6:
        itfDspSignalC();
        scene->state = 10;
        break;
    case 7:
        itfDspSignalD();
        scene->state = 10;
        break;
    case 8:
        itfDspSignalE();
        scene->state = 10;
        break;
    case 10:
        if (scene->stageFinished != 0 && evtGetMessageWindowControlState() == 0) {
            evtFinishMessageWindowAndNotify();
            sdfSetTaskItemMode((TaskWork *)mnuSceneResourceContext, 1,
                               SDF_TASK_ITEM_MODE_PENDING_ACTIVATION);
            return -1;
        }
        break;
    default:
        break;
    }
    return 0;
}

extern u32 mnuGetSelectedNodeValue(void);
extern void func_0024DD78(void);
extern void func_00255E08(MenuSceneMetadata *, s32, s32);
extern void mnuDrawAnimatedCurrencyCounter(s32, s32, s32, s32, MenuSceneMetadata *, s32);
extern s32 func_00249998(void *, s32, s32);
extern void evtStageTestSelectEntryWithoutInitialValue(u16, s32);
extern void effUpdateAttached(s32, s32, s32, s32, s32);
extern s32 evtStageTestUpdate(void *);
extern s64 evtGetMessageWindowControlState(void);
extern u8 D_00325818[];
extern void uiDrawGradientColorRect(u32, u32, u32, u32, u32, const u32 *, u32);

/* Update attached visuals and stage completion, then ramp the message-window shade over ten frames.
 * Only the final two rectangle colors receive alpha; preserve both native selection lookups and the zero return. */
s32 mnuUpdateMantraSceneDisplay(void) {
    MenuSceneMetadata *sceneMetadata = (MenuSceneMetadata *)sdfGetTaskValueByKey((TaskWork *)mnuSceneResourceContext, SDF_TASK_VALUE_USER_DATA_KEY);
    u32 shadeColors[4];

    mnuGetSelectedNodeValue();
    fldGetSceneMetadataNode();
    func_0024DD78();
    func_00255E08(sceneMetadata, 0x80, 0x4A);
    mnuDrawAnimatedCurrencyCounter(0, -27, 0, 0x80, sceneMetadata, MNU_SCENE_DRAW_CONTEXT);
    if (func_00249998(&sceneMetadata->attachedEffectControl, sceneMetadata->attachedEffect, MNU_SCENE_DRAW_CONTEXT) != 0) {
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

    scene->entryPosition.x = D_0036BE38[entryIndex].entryX;
    entryIndex = selectedEntry->sceneId;
    scene->entryPosition.y = D_0036BE38[entryIndex].entryY;
}

/* Refresh the active scene-work entry coordinates and return zero. */
s32 fldResetSceneState(void) {
    fldUpdateSceneEntryMetadata((MenuSceneWork *)(u32)sdfGetTaskValueByKey((TaskWork *)mnuSceneResourceContext, 1));
    return 0;
}

/* Update and copy scene coordinates before releasing its node list; retain the native call order. */
void mnuCopySceneCoordinatesAndReleaseNodeList(void) {
    s32 sceneAddress = sdfGetTaskValueByKey((TaskWork *)mnuSceneResourceContext, 1);
    func_002512F0(sceneAddress, 1);
    mnuCopySceneCoordinates((MenuSceneWork *)(u32)sceneAddress);
    mnuReleaseListNodes(
        &((MenuSceneWork *)(u32)sceneAddress)->costTransitionList);
}

INCLUDE_SDATA(const s32, "game/code_00250E88", D_003BC420);

INCLUDE_SDATA(const s32, "game/code_00250E88", D_003BC424);

INCLUDE_SDATA(const s32, "game/code_00250E88", D_003BC428);

INCLUDE_SDATA(const s32, "game/code_00250E88", D_003BC430);

INCLUDE_SDATA(const s32, "game/code_00250E88", D_003BC438);
