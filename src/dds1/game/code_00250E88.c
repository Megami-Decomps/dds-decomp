#include "common.h"

#define MNU_SCENE_WORK_SIZE 0x5B0

extern void func_002512F0(s32, s32);

extern void mnuReleaseListNodes(s32);

extern void mnuInitializeMantraSelectionGrid(s32);

extern void mnuCopySceneCoordinates(s32);

extern void sdfDestroyGridWork(s32);

extern void mnuReleaseDisplayListNodes(s32);

extern void sdfReleaseResourceAllocation(s32);

extern void mnuResetWorkFloats(void);

extern s32 func_002CB3B8(u32, u32);

extern u32 mnuSceneResourceContext;

typedef struct {
    u32 unk0;
    u16 entryX;
    u16 entryY;
    u32 unk8;
} SceneEntry;

extern SceneEntry D_0036BE38[];

/* Fields initialized and released around the scene's 0x5B0-byte work block. */
typedef struct MenuSceneWork {
    s32 allocationHandle; /* 0x000 */
    u8 pad004[0x480];
    s32 gridHandle;       /* 0x484 */
    u32 gridRefreshControl[2]; /* 0x488 */
    u8 pad490[0xB0];
    s32 coordinateA;      /* 0x540 */
    s32 coordinateB;      /* 0x544 */
} MenuSceneWork;

typedef struct MenuSceneMetadata {
    u8 pad00[0x0C];
    s32 messageWindowResource;
    u8 pad10[8];
    s32 state;
    u8 pad1C[4];
    s32 messageShadeFrames;
    s32 attachedEffect;
    u32 attachedEffectControl;
    u16 entryId;
    u8 pad02E[0x20E];
    s32 displayedCurrency;
    u8 pad240[4];
    u16 pendingProfileId;
    s8 stageFinished;
    s8 stageStarted;
} MenuSceneMetadata;

typedef struct ScrVmOperand ScrVmOperand;

typedef struct MnuProfileProgress {
    ScrVmOperand *operand; /* 0x00 */
    s32 profileId;         /* 0x04 */
    u32 value;             /* 0x08 */
    u32 cap;               /* 0x0C */
} MnuProfileProgress;

typedef struct DatGameCounters {
    u8 pad00[0x3C];
    s32 currency;
} DatGameCounters;

typedef struct MenuGridCell {
    u32 index;
    s32 value;
} MenuGridCell;

typedef struct MenuGridCoordinate {
    s16 x;
    s16 y;
    u8 pad04[8];
} MenuGridCoordinate;

typedef struct MenuGrid {
    u8 pad00[8];
    MenuGridCell *cursor;
    u8 pad0C[0x24];
    MenuGridCoordinate *entries;
} MenuGrid;

typedef struct MenuSceneEntry {
    u8 pad00[0x0C];
    u16 sceneId;
    u8 pad0E[6];
    s32 state;
    u8 pad18[0x3C];
    s8 profileFlag;
} MenuSceneEntry;

extern void sdfReleaseChipBlock(void *);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00250E88);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00250F60);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00251260);

INCLUDE_ASM(const s32, "game/code_00250E88", func_002512F0);

void mnuFreeTaskData(s32 unused, void *data) {
    if (data != NULL) {
        sdfReleaseChipBlock(data);
    }
}

INCLUDE_ASM(const s32, "game/code_00250E88", func_002515F0);

INCLUDE_ASM(const s32, "game/code_00250E88", func_002517C0);

extern s32 sdfAllocGeneralBlock(s32);
extern s32 sdfMemoryGetBlockAddress(s32);
extern void *memset(void *, s32, u32);
extern u8 *datGameState;

/* Allocate and clear scene work before registering its grid and coordinates. */
s32 mnuCreateSceneWork(void) {
    s32 handle = sdfAllocGeneralBlock(MNU_SCENE_WORK_SIZE);
    u8 *work = (u8 *)sdfMemoryGetBlockAddress(handle);

    memset(work, 0, MNU_SCENE_WORK_SIZE);
    ((MenuSceneWork *)work)->allocationHandle = handle;
    mnuInitializeMantraSelectionGrid((s32)work);
    ((MenuSceneWork *)work)->coordinateA = 0;
    ((MenuSceneWork *)work)->coordinateB = 0;
    ((MenuSceneMetadata *)func_002CB3B8(mnuSceneResourceContext, -1))->displayedCurrency = ((DatGameCounters *)datGameState)->currency;
    mnuCopySceneCoordinates((s32)work);
    return (s32)work;
}

void mnuReleaseSceneContext(s32 unused, s32 context) {
    func_002CB3B8(mnuSceneResourceContext, -1);
    sdfDestroyGridWork(((MenuSceneWork *)context)->gridHandle);
    mnuReleaseDisplayListNodes(context + 0x584);
    sdfReleaseResourceAllocation(((MenuSceneWork *)context)->allocationHandle);
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

INCLUDE_ASM(const s32, "game/code_00250E88", func_00253018);

INCLUDE_ASM(const s32, "game/code_00250E88", func_002530D8);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00253208);

void mnuReinitializeSceneGrid(s32 context) {
    sdfDestroyGridWork(((MenuSceneWork *)context)->gridHandle);
    mnuInitializeMantraSelectionGrid(context);
    mnuCopySceneCoordinates(context);
}

extern void func_002CBB48(MenuGrid *grid);
extern void func_00253208(s32 context, s32 sceneId, s32 *x, s32 *y);
extern void *sdfGridSelectFilledCell(MenuGrid *grid, s32 x, s32 y);
extern void func_002CC0D0(MenuGrid *grid);

void func_00253558(s32 context) {
    s32 grid = ((MenuSceneWork *)context)->gridHandle;
    MenuGridCell *cursor = ((MenuGrid *)grid)->cursor;
    s32 selected = cursor->value;
    u16 entryId = *(u16 *)(selected + 0xC);
    MenuGridCoordinate *entries = ((MenuGrid *)grid)->entries;
    s16 x = entries[entryId].x;
    s16 y = entries[entryId].y;
    s32 scene;
    s32 field;

    func_002CBB48((MenuGrid *)grid);
    scene = func_002CB3B8(mnuSceneResourceContext, 0);
    field = *(s32 *)(*(s32 *)(scene + 0xC) + 0x1C);
    func_00253208(context, *(s32 *)(field + 0x70), NULL, NULL);
    if (sdfGridSelectFilledCell(
            (MenuGrid *)((MenuSceneWork *)context)->gridHandle, x, y) == NULL) {
        func_002CC0D0((MenuGrid *)((MenuSceneWork *)context)->gridHandle);
    }
}

s32 fldGetSceneMetadataNode(void) {
    s32 context = func_002CB3B8(mnuSceneResourceContext, 1);

    if (context == 0) {
        return 0;
    }
    return *(s32 *)(*(s32 *)(*(s32 *)(context + 0x484) + 8) + 4);
}

extern s32 dspCloseChannel(void);
extern s32 evtCreateMessageWindowIfMissing(s32);
extern u32 mnuGetSelectedNodeValue(void);
extern void func_00249850(s32, void *);
extern void *memcpy(void *, const void *, u32);

typedef struct SceneEntryNode {
    u8 pad00[0x0C];
    u16 entryIndex;
} SceneEntryNode;

s32 func_00253640(void) {
    SceneEntryNode *node = (SceneEntryNode *)fldGetSceneMetadataNode();
    MenuSceneMetadata *scene = (MenuSceneMetadata *)func_002CB3B8(mnuSceneResourceContext, -1);

    scene->state = 0;
    scene->pendingProfileId = node->entryIndex;
    dspCloseChannel();
    evtCreateMessageWindowIfMissing(scene->messageWindowResource);
    memcpy((u32 *)((u8 *)scene + 0x28), (u32 *)*(u32 *)mnuGetSelectedNodeValue(), 0x1A4);
    ((u8 *)scene)[0x7D] = (u8)scene->pendingProfileId;
    func_00249850((s32)((u8 *)scene + 0x28), (void *)scene->attachedEffect);
    scene->stageFinished = 0;
    scene->stageStarted = 0;
    return 0;
}

extern s8 scrSelectOperandIndex(ScrVmOperand *, s32);
extern s8 scrGetSelectedOperandIndex(ScrVmOperand *);
extern u32 ptyGetProfileRecordValue(void *, u16);
extern u32 prfGetCapValue(u16);
extern void func_00258AF0(u32 *, u32);
extern void evtFinishMessageWindowAndNotify(void);
extern void mnuReleaseMenuVisualWorkResources(s32);

void func_00253778(void) {
    MenuSceneMetadata *scene = (MenuSceneMetadata *)func_002CB3B8(mnuSceneResourceContext, -1);

    scene->messageShadeFrames = 0;
    if (scene->pendingProfileId != 0) {
        MnuProfileProgress *selection = (MnuProfileProgress *)mnuGetSelectedNodeValue();
        MenuSceneWork *work;

        scrSelectOperandIndex(selection->operand, scene->pendingProfileId);
        selection->profileId = scrGetSelectedOperandIndex(selection->operand);
        selection->value = ptyGetProfileRecordValue(selection->operand, (u16)selection->profileId);
        selection->cap = prfGetCapValue((u16)selection->profileId);

        work = (MenuSceneWork *)func_002CB3B8(mnuSceneResourceContext, 1);
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
extern s8 evtGetCapturedMessageWindowSoundMode(void);
extern void sdfSetTaskItemMode(void *, s32, u32);

s32 func_00253830(void) {
    MenuSceneEntry *entry = (MenuSceneEntry *)fldGetSceneMetadataNode();
    MnuProfileProgress *selection = (MnuProfileProgress *)mnuGetSelectedNodeValue();
    MenuSceneMetadata *scene = (MenuSceneMetadata *)func_002CB3B8(mnuSceneResourceContext, -1);

    switch (scene->state) {
    case 0:
        if (D_00324510[0x21] < 0) {
            if (selection->profileId == scene->pendingProfileId) {
                if (prfGetCapValue(entry->sceneId) ==
                    ptyGetProfileRecordValue(selection->operand, entry->sceneId)) {
                    scene->state = 8;
                } else {
                    scene->state = 5;
                }
                scene->pendingProfileId = 0;
            } else if (prfGetCapValue(entry->sceneId) ==
                       ptyGetProfileRecordValue(selection->operand, entry->sceneId)) {
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
            if (evtGetCapturedMessageWindowSoundMode() != 0) {
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
        ((DatGameCounters *)datGameState)->currency -= mnuGetMantraSourceValue(entry->sceneId);
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
            sdfSetTaskItemMode((void *)mnuSceneResourceContext, 1, 1);
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
extern void func_0025B0F0(s32, s32, s32, s32, MenuSceneMetadata *, s32);
extern s32 func_00249998(void *, s32, s32);
extern void evtStageTestSelectEntryWithoutInitialValue(u16, s32);
extern void effUpdateAttached(s32, s32, s32, s32, s32);
extern s32 evtStageTestUpdate(void *);
extern s64 evtGetMessageWindowControlState(void);
extern u8 D_00325818[];
extern void uiDrawGradientColorRect(s32, s32, s32, s32, s32, u32 *, s32);

s32 func_00253AD0(void) {
    MenuSceneMetadata *scene = (MenuSceneMetadata *)func_002CB3B8(mnuSceneResourceContext, -1);
    u32 colors[4];

    mnuGetSelectedNodeValue();
    fldGetSceneMetadataNode();
    func_0024DD78();
    func_00255E08(scene, 0x80, 0x4A);
    func_0025B0F0(0, -27, 0, 0x80, scene, 0x53);
    if (func_00249998(&scene->attachedEffectControl, scene->attachedEffect, 0x53) != 0) {
        if (scene->stageStarted == 0) {
            evtStageTestSelectEntryWithoutInitialValue(scene->entryId, 0);
        }
        scene->stageStarted = 1;
    }
    effUpdateAttached(0x1200, 0x730, 1, scene->attachedEffect, 0x53);
    if (evtStageTestUpdate(D_00325818) >= 2) {
        scene->stageFinished = 1;
    }
    if (evtGetMessageWindowControlState() != 0) {
        if (scene->messageShadeFrames < 10) {
            scene->messageShadeFrames++;
        }
    } else if (scene->messageShadeFrames > 0) {
        scene->messageShadeFrames--;
    }
    if (scene->messageShadeFrames != 0) {
        memset(colors, 0, sizeof(colors));
        colors[3] = colors[2] = (u32)((f32)scene->messageShadeFrames / 10.0f * 112.0f);
        uiDrawGradientColorRect(0, 0x700, 2, 0x2000, 0x700, colors, 0x53);
    }
    return 0;
}

typedef struct SceneMetadataNode {
    u8 pad00[0xC];
    u16 entryIndex; /* 0x0C */
} SceneMetadataNode;

typedef struct {
    u8 pad00[4];
    SceneMetadataNode *node; /* 0x04 */
} SceneMetadataSlot;

typedef struct {
    u8 pad00[8];
    SceneMetadataSlot *slot; /* 0x08 */
} SceneMetadataGrid;

typedef struct {
    u8 pad00[0x484];
    SceneMetadataGrid *grid; /* 0x484 */
    u8 pad488[0x114];
    u16 entryX; /* 0x59C */
    u16 entryY; /* 0x59E */
} SceneMetadataContext;

/* Copy the active scene entry coordinates selected by the grid metadata. */
void fldUpdateSceneEntryMetadata(s32 context) {
    SceneMetadataNode *node = ((SceneMetadataContext *)context)->grid->slot->node;
    u16 index = node->entryIndex;
    ((SceneMetadataContext *)context)->entryX = D_0036BE38[index].entryX;
    index = node->entryIndex;
    ((SceneMetadataContext *)context)->entryY = D_0036BE38[index].entryY;
}

s32 fldResetSceneState(void) {
    fldUpdateSceneEntryMetadata(func_002CB3B8(mnuSceneResourceContext, 1));
    return 0;
}

void mnuCopySceneCoordinatesAndReleaseNodeList(void) {
    s32 context = func_002CB3B8(mnuSceneResourceContext, 1);
    func_002512F0(context, 1);
    mnuCopySceneCoordinates(context);
    mnuReleaseListNodes(context + 0x590);
}

INCLUDE_SDATA(const s32, "game/code_00250E88", D_003BC420);

INCLUDE_SDATA(const s32, "game/code_00250E88", D_003BC424);

INCLUDE_SDATA(const s32, "game/code_00250E88", D_003BC428);

INCLUDE_SDATA(const s32, "game/code_00250E88", D_003BC430);

INCLUDE_SDATA(const s32, "game/code_00250E88", D_003BC438);
