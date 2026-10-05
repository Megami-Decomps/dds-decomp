#include "mnu.h"

#define MTR_RECORD_COUNT 32
#define MTR_STATUS_RESOURCE_BYTES 0xC08
#define MTR_UNIT_ENTRY_COUNT 5
#define MTR_SELECTION_PHASE_PER_TICK 0.125f
#define MTR_SELECTION_ALPHA_SCALE 128.0f
#define MTR_UNIT_FADE_FRAMES 10

typedef struct {
    u16 flags;
    u16 pad2;
    u16 unk4;
    u8 pad6[0x1BE];
} MtrRecord;

typedef struct MtrGameState {
    u8 pad00[0xA60];
    MtrRecord records[32];
} MtrGameState;

extern MtrGameState *datGameState;
extern void func_00286A58(MtrRecord *);

typedef struct MenuListNode MenuListNode;
typedef struct MenuContainer MenuContainer;
struct MnuStatusResource;

typedef struct MenuList {
    u32 stateFlags;
    u32 flags;
    u32 id;
    s32 visibleCount;
    MenuListNode *first;
    MenuListNode *last;
    MenuListNode *head;
    MenuListNode *cursor;
    s32 count;
    s32 windowOffset;
    s32 rowHeight;
    u8 pad2C[4];
    u32 userData;
    u8 pad34[8];
    s32 scale;
} MenuList;

typedef struct MtrSelectionState {
    s16 state;
    u16 unk02;
    s32 timer;
} MtrSelectionState;

typedef struct MtrSelectionFlags {
    u32 unk00 : 1;
    u32 visible : 1;
    u32 fadeProgress : 1;
    u32 profileReady : 1;
    u32 unk04 : 1;
    u32 unk05 : 27;
} MtrSelectionFlags;

typedef struct MtrPlayerFlags {
    u32 unk00 : 16;
    u32 hasMarkedUnit : 1;
    u32 unk17 : 15;
} MtrPlayerFlags;

typedef struct MtrUnitMenuEntry {
    u16 unk00 : 13;
    u16 marked : 1;
    u16 unk14 : 2;
    u8 pad02[6];
} MtrUnitMenuEntry;

typedef struct MnuPartySnapshot {
    u16 flags;
    u16 unk02;
    u16 rosterIndex;
    u16 unk06;
    u16 hp;
    u16 unk0A;
    u16 mp;
    u16 unk0E;
    u32 totalExp;
    u16 level;
    u8 stats[5];
    u8 pad1B[0x3A];
    u8 unk55;
    u8 pad56[0x16E];
} MnuPartySnapshot;

struct MenuListNode {
    u8 pad00[0x70];
    u8 *items;
};

typedef struct MtrEquipState {
    s32 state;
    s32 timer;
} MtrEquipState;

extern MenuList *func_002884C0(void);
extern s32 mdlFlagTest(s32);
extern void kwlnFadeInStart(s8, s8, s8, s32);
extern void func_00289BA0(struct MnuStatusResource *);
extern s32 func_00288920(struct MnuStatusResource *);
extern s32 mnuMoveNodeCursorToTargetIndex(MenuContainer *, s8);

extern s32 func_00312810(u32, s32);

extern u32 mnuMantraSelectionResource;
extern void mnuReleaseMantraPanelPositionTable(void);
extern void mnuCleanupMantraVisualsAndResetTitleStream(struct MnuStatusResource *);
extern void mnuEnableTerminalTrackMode(s8);

extern u8 mnuResourceTaskName[];

extern void func_00286F18(s32, s32);

extern u8 D_003CFCC0[];

extern void mnuReleaseSelectionWorkResources(struct MnuStatusResource *);

extern u32 mnuDestroyListState(MenuList *);

extern void mnuCloseCurrentProfilePanel(MenuProgressHost *);

extern void mnuReleaseMantraMenuDrawResources(void *);

extern void dspCloseChannel(void);
extern void sdfQueueNonzeroResourceId(u32);
struct TaskWork;
struct SdfTaskItemDesc;
extern struct SdfTaskItemDesc D_003CFCD4;
extern void sdfAttachTaskItem(struct TaskWork *, struct SdfTaskItemDesc *);
extern void mnuReleaseFirstMantraSpriteSlots(void);
extern void mnuReleaseStaffAndTitleVisualResources(u32 *);
extern void evtPrintDeveloperConsoleMessage(const char *, ...);
extern void sdfReleaseResourceAllocation(u32);
extern void mnuReleasePanelEntryPool(void);
INCLUDE_ASM(const s32, "game/code_00286BA8", func_00286BA8);

/* Process each of the 32 game-state records whose flags bit 0 is set, then print the native mantra banner. */
void func_00286E20(void) {
    s32 recordIndex;

    for (recordIndex = 0; recordIndex < MTR_RECORD_COUNT; recordIndex++) {
        if (datGameState->records[recordIndex].flags & 1) {
            func_00286A58(&datGameState->records[recordIndex]);
        }
    }
    evtPrintDeveloperConsoleMessage("*****************[mtrMantraSetBitAll()]*****************\n");
}

extern u32 sdfAllocGeneralBlock(s32 size);
extern u32 sdfMemoryGetBlockAddress(void *block);
extern s32 mnuCreateProgressHost(void);
extern void mnuInitPanelSoundEntries(void);

/* File-load state machine: seven table entries and the active roster bitmask. */
typedef struct MtrResourceLoadState {
    u32 fileEntry;
    u32 unk04;
    s16 state;
    u16 entryIndex;
    u32 partyMask;
} MtrResourceLoadState;

/* One allocated mantra/status work area. The create side stores its own
 * allocation handle and the progress host; the destroy side releases both
 * resource ids and the host. Only the fields either side touches are named;
 * the rest of the block is passed on to the menu task untouched. */
typedef struct MnuStatusResource {
    u32 allocationHandle; /* 0x00: the block's own handle, freed on destroy */
    MenuList *list;       /* 0x04 */
    u8 pad08[0x30];
    u32 resourceIdA;      /* 0x38 */
    u32 resourceIdB;      /* 0x3C */
    s32 messageWindow;
    u8 pad44[4];
    MenuProgressHost *progressHost; /* 0x48 */
    u8 pad4C[8];
    MnuPartySnapshot snapshot;
    MtrSelectionFlags flags;
    MtrResourceLoadState resourceLoad; /* 0x21C */
    MtrSelectionState selection; /* 0x22C */
    u8 pad234[0x560];
    MtrPlayerFlags playerFlags; /* 0x794 */
    u8 pad798[0x40C];
    u32 unkBA4;
    u8 padBA8[8];
    MtrUnitMenuEntry unitEntries[5]; /* 0xBB0 */
    u8 padBD8[0x20];
    MtrEquipState equip;
    u32 drawPool;
    u8 padC04[4];
} MnuStatusResource; /* 0xC08 */

extern s32 func_00287078(MtrResourceLoadState *, u16);

extern void mtrInitUnitSelectionWork(MnuStatusResource *);

/* Allocate and clear status work, retain its allocation handle and create the progress host.
 * Print the native load banner and initialize panel sound entries before returning the work pointer. */
void *func_00286E98(void) {
    u32 allocationHandle = sdfAllocGeneralBlock(MTR_STATUS_RESOURCE_BYTES);
    MnuStatusResource *resourceWork = (MnuStatusResource *)sdfMemoryGetBlockAddress(allocationHandle);

    memset(resourceWork, 0, MTR_STATUS_RESOURCE_BYTES);
    resourceWork->allocationHandle = allocationHandle;
    resourceWork->progressHost = (MenuProgressHost *)mnuCreateProgressHost();
    evtPrintDeveloperConsoleMessage("trmLoadStartStatusResource()!!!! \n");
    evtPrintDeveloperConsoleMessage("mtrInit\n");
    mnuInitPanelSoundEntries();
    return resourceWork;
}

/* Release status-owned resources only for a nonzero work address; print the release banner even when it is zero. */
void func_00286F18(s32 unused, s32 resourceAddress) {
    if (resourceAddress != 0) {
        MnuStatusResource *resourceWork = (MnuStatusResource *)resourceAddress;

        dspCloseChannel();
        sdfQueueNonzeroResourceId(resourceWork->resourceIdA);
        sdfQueueNonzeroResourceId(resourceWork->resourceIdB);
        mnuReleaseFirstMantraSpriteSlots();
        mnuReleaseStaffAndTitleVisualResources((u32 *)resourceWork->progressHost);
        evtPrintDeveloperConsoleMessage("trmDestroyStatusResource()!!!! \n");
        sdfReleaseResourceAllocation(resourceWork->allocationHandle);
        mnuReleasePanelEntryPool();
    }
    evtPrintDeveloperConsoleMessage("mtrRelease\n");
}

/* Store the task handle so the existence probe and explicit stop can
 * invalidate or destroy the same resource group. */
void mnuCreateResourceTask(void) {
    MnuStatusResource *resourceWork = (MnuStatusResource *)func_00286E98();
    mnuMantraSelectionResource = sdfCreateTaskWorker(mnuResourceTaskName, 0x402, 0x2B12, D_003CFCC0, func_00286F18, resourceWork);
}

/* Return whether the named resource task exists; invalidate the cached handle when it does not. */
s32 mnuCheckResourceTask(void) {
    if (kwlnTaskExists(mnuResourceTaskName) != 0) {
        return 1;
    }
    mnuMantraSelectionResource = 0;
    return 0;
}

/* Destroy the cached resource-task group and clear its handle. */
void mnuStopResourceTask(void) {
    sdfDestroyTaskWorkerTasks(mnuMantraSelectionResource);
    mnuMantraSelectionResource = 0;
}

s32 func_00287030(void) {
    MnuStatusResource *selected = (MnuStatusResource *)func_00312810(mnuMantraSelectionResource, -1);
    s32 result = func_00287078(&selected->resourceLoad, 0);

    if (result != 0) {
        sdfAttachTaskItem((struct TaskWork *)mnuMantraSelectionResource, &D_003CFCD4);
        return -1;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_00286BA8", mnuResourceTaskName);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00287078);

/* Initialize the unit-selection state of the current resource-task work; return zero. */
s32 mtrUnitSelectInit(void) {
    MnuStatusResource *resourceWork = (MnuStatusResource *)func_00312810(mnuMantraSelectionResource, -1);

    mtrInitUnitSelectionWork(resourceWork);
    evtPrintDeveloperConsoleMessage("mtrUnitSelectInit\n");
    return 0;
}

/* Release the current work's unit-selection list, profile panel and drawing resources. */
void mtrUnitSelectRelease(void) {
    MnuStatusResource *resourceWork = (MnuStatusResource *)func_00312810(mnuMantraSelectionResource, -1);

    mnuReleaseSelectionWorkResources(resourceWork);
    evtPrintDeveloperConsoleMessage("mtrUnitSelectRelease\n");
}

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00287670);

/* Draw the current unit-selection resource; the handler's native u64 return is always zero. */
u64 func_00287768(void) {
    MnuStatusResource *resourceWork;

    resourceWork = (MnuStatusResource *)func_00312810(mnuMantraSelectionResource, -1);
    func_00288920(resourceWork);
    return 0;
}

/* Enter mantra selection on the current work address and disable terminal-track mode; return zero. */
s32 mtrMantraSelectInit(void) {
    u64 resourceAddress = func_00312810(mnuMantraSelectionResource, -1);

    mnuEnableTerminalTrackMode(0);
    mnuOpenMantraSelectionAndLoadTitleStream(resourceAddress);
    evtPrintDeveloperConsoleMessage("mtrMantraSelectInit\n");
    return 0;
}

/* Release mantra visuals, clear the work's visible bit and restore terminal-track mode. */
void mtrMantraSelectRelease(void) {
    MnuStatusResource *resourceWork =
        (MnuStatusResource *)func_00312810(mnuMantraSelectionResource, -1);

    mnuReleaseMantraPanelPositionTable();
    mnuCleanupMantraVisualsAndResetTitleStream(resourceWork);
    resourceWork->flags.visible = 0;
    mnuEnableTerminalTrackMode(1);
    evtPrintDeveloperConsoleMessage("mtrMantraSelectRelease\n");
}

extern struct SdfTaskItemDesc D_003CFCFC;
extern void mnuTickPanelSoundEntries(void);
extern s32 func_0028A1D0(MnuStatusResource *);
extern void sdfSetTaskItemMode(void *, s32, u32);

/* Tick panel sounds and process the selection result. Case 2 intentionally falls through to case 3;
 * case 4 requests task-item mode (1,1) and returns -1, while other results return zero. */
s32 func_00287848(s32 key) {
    mnuTickPanelSoundEntries();
    switch (func_0028A1D0((MnuStatusResource *)func_00312810(mnuMantraSelectionResource, -1))) {
    case 1:
        break;
    case 2:
        sdfAttachTaskItem((struct TaskWork *)mnuMantraSelectionResource, &D_003CFCFC);
        sdfSetTaskItemMode((void *)mnuMantraSelectionResource, key, 2);
    case 3:
        break;
    case 4:
        sdfSetTaskItemMode((void *)mnuMantraSelectionResource, 1, 1);
        return -1;
    }
    return 0;
}

/* Run the current mantra resource's drawing path; retain the native u64 work address and zero return. */
u64 func_00287900(void) {
    u64 resourceAddress;

    resourceAddress = func_00312810(mnuMantraSelectionResource, -1);
    func_0028B1B0(resourceAddress);
    return 0;
}

extern u32 mnuGetDefaultPanelSelector(MnuStatusResource *);
extern void evtCreateMessageWindowIfMissing(s32);
extern void func_00267B40(s32, MenuProgressHost *);
extern void mnuEnsureProfilePanelEffect(s32, MenuProgressHost *);
extern void mnuInitPartyPanelSlots(PartyPanel *);
extern void func_002BCAB0(MenuPageWindow *);
extern char D_00426208[];
extern char D_00426218[];

/* Initialize the equip panel from the selected party entry and selector. */
s32 mtrMantraEquipInit(void) {
    MnuStatusResource *work = (MnuStatusResource *)func_00312810(mnuMantraSelectionResource, -1);
    MnuPartySnapshot *snapshot = &work->snapshot;
    MtrEquipState *equip = &work->equip;
    u8 *selector;

    work->flags.unk04 = 0;
    selector = (u8 *)mnuGetDefaultPanelSelector(work);
    dspCloseChannel();
    evtCreateMessageWindowIfMissing(work->messageWindow);
    mnuCloseCurrentProfilePanel(work->progressHost);
    memcpy(snapshot, work->list->cursor->items, sizeof(*snapshot));
    work->snapshot.unk55 = selector[2];
    evtPrintDeveloperConsoleMessage(D_00426208, work->snapshot.hp, work->snapshot.mp);
    func_00267B40((s32)snapshot, work->progressHost);
    mnuEnsureProfilePanelEffect((s32)snapshot, work->progressHost);
    mnuInitPartyPanelSlots(&work->progressHost->partyPanel);
    func_002BCAB0(&work->progressHost->partyWindow);
    work->flags.fadeProgress = 0;
    work->flags.profileReady = 0;
    equip->state = 1;
    equip->timer = 0;
    evtPrintDeveloperConsoleMessage(D_00426218);
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_00286BA8", D_00426208);

INCLUDE_RODATA(const s32, "game/code_00286BA8", D_00426218);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00287AF8);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00287C20);

extern void func_0026C900(void);
extern void mnuUpdateMantraDrawPool(u32 pool);
extern void mnuDrawMantraSprite(s32, s32, s32, s32, s32, s32, s32);
extern s32 mnuDrawLoadedProgressPanels(s32, MenuProgressHost *, s32);
extern void evtStageTestSelectEntryWithoutInitialValue(u16, u32);
extern void mnuDrawCurrentProfilePanel(s32, s32, s32, MenuProgressHost *, s32);
extern s8 evtStageTestUpdate(s32);
extern u8 D_00380818[];

s32 func_00288158(void) {
    MnuStatusResource *work = (MnuStatusResource *)func_00312810(mnuMantraSelectionResource, -1);
    f32 ratio;
    s32 value;

    func_0026C900();
    mnuUpdateMantraDrawPool(work->drawPool);
    ratio = 0.0f;
    if (work->flags.fadeProgress) {
        if (work->equip.timer < 30) {
            work->equip.timer++;
        }
        ratio = (f32)work->equip.timer / 30.0f;
    }
    value = (s32)(ratio * 128.0f);
    mnuDrawMantraSprite(0, 0, 0, value, 0x68, 0, 0x4A);
    mnuDrawMantraSprite(0, 0, 0, value, 0x69, 0, 0x4A);
    if (mnuDrawLoadedProgressPanels((s32)&work->snapshot, work->progressHost, 0x53) != 0) {
        if (!work->flags.profileReady) {
            evtStageTestSelectEntryWithoutInitialValue(work->snapshot.rosterIndex, 0);
        }
        work->flags.profileReady = 1;
    }
    mnuDrawCurrentProfilePanel(0xE80, 0x5B8, 1, work->progressHost, 0x53);
    if (evtStageTestUpdate((s32)D_00380818) >= 2) {
        work->flags.fadeProgress = 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00286BA8", func_002882B8);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_002884C0);

/* Create the unit list and selection state; when a marked unit exists, move to the first marked entry.
 * Preserve the two independent fade-in requests and the post-callback work-field reads. */
void mtrInitUnitSelectionWork(MnuStatusResource *resourceWork) {
    MtrSelectionState *selectionState = &resourceWork->selection;
    MenuList *unitList;
    s32 unitIndex;

    selectionState->state = 1;
    selectionState->timer = 0;
    unitList = func_002884C0();
    unitList->userData = (u32)selectionState;
    resourceWork->list = unitList;
    resourceWork->flags.visible = 0;

    if (mdlFlagTest(0x1B1) != 0) {
        if (mdlFlagTest(0x995) == 0) {
            selectionState->state = 3;
            kwlnFadeInStart(0, 0, 0, MTR_UNIT_FADE_FRAMES);
        }
        func_00289BA0(resourceWork);
        if (resourceWork->unkBA4 != 0 || resourceWork->playerFlags.hasMarkedUnit) {
            if (resourceWork->playerFlags.hasMarkedUnit) {
                for (unitIndex = 0; unitIndex < MTR_UNIT_ENTRY_COUNT; unitIndex++) {
                    if (resourceWork->unitEntries[unitIndex].marked) {
                        mnuMoveNodeCursorToTargetIndex((MenuContainer *)resourceWork, unitIndex);
                        break;
                    }
                }
            }
            selectionState->state = 3;
            kwlnFadeInStart(0, 0, 0, MTR_UNIT_FADE_FRAMES);
        }
    }
}

/* Release list state, the current profile panel and mantra drawing resources in native order. */
void mnuReleaseSelectionWorkResources(MnuStatusResource *resourceWork) {
    mnuDestroyListState(resourceWork->list);
    mnuCloseCurrentProfilePanel(resourceWork->progressHost);
    mnuReleaseMantraMenuDrawResources(resourceWork);
}

INCLUDE_RODATA(const s32, "game/code_00286BA8", D_00426280);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00288748);

extern void func_0026C900(void);
extern s32 func_00288BD8(s32, s32, s32, s32, MnuStatusResource *, s32, f32);
extern s32 func_00288DD0(s32, s32, s32, s32, MnuStatusResource *, s32);

/* Draw visible unit-selection work using a timer-based triangular alpha.
 * Re-read visibility after the first drawing call; do not clamp the native phase/alpha range. */
s32 func_00288920(MnuStatusResource *resourceWork) {
    f32 fadePhase = 1.0f;
    s32 alpha;
    MtrSelectionState *selectionState;

    if (!resourceWork->flags.visible) {
        return 0;
    }
    func_0026C900();
    selectionState = &resourceWork->selection;
    switch (selectionState->state) {
        case 1:
            fadePhase = selectionState->timer * MTR_SELECTION_PHASE_PER_TICK;
            break;
        case 2:
            fadePhase = selectionState->timer * MTR_SELECTION_PHASE_PER_TICK;
            break;
        case 3:
        case 4:
            fadePhase = 0.0f;
            break;
    }
    if (fadePhase > 1.0f) {
        alpha = (2.0f - fadePhase) * MTR_SELECTION_ALPHA_SCALE;
    } else {
        alpha = fadePhase * MTR_SELECTION_ALPHA_SCALE;
    }
    func_00288BD8(0, 0, 0, alpha, resourceWork, 0x53, fadePhase);
    if (resourceWork->flags.visible) {
        func_00288DD0(0, 0, 1, alpha, resourceWork, 0x53);
    }
    return 0;
}

extern s8 D_0037F510[];
extern MenuListNode *mnuRetreatListCursorDefault(MenuList *);
extern MenuListNode *mnuAdvanceListCursorDefault(MenuList *);
extern void mnuRebuildProfilePanelFromRenderSnapshot(MnuStatusResource *);
extern void mnuClearListFlagsOneAndTwo(MenuList *);
extern void sndSetSequenceVolumePan(s32, s32, s32);

/* Move the mantra selection, rebuild its panel, or report confirm/cancel. */
s32 mnuHandleMantraSelectionInput(MnuStatusResource *resourceWork) {
    s32 sound = 0;
    s32 action = 0;
    MenuList *list = resourceWork->list;

    if (D_0037F510[0x26] & 2) {
        if (mnuRetreatListCursorDefault(list) != NULL) {
            sound = 1;
            mnuRebuildProfilePanelFromRenderSnapshot(resourceWork);
        }
    } else if (D_0037F510[0x27] & 2) {
        if (mnuAdvanceListCursorDefault(list) != NULL) {
            sound = 1;
            mnuRebuildProfilePanelFromRenderSnapshot(resourceWork);
        }
    } else if (D_0037F510[0x21] < 0) {
        action = 2;
        sound = 2;
    } else if (D_0037F510[0x23] < 0) {
        action = 3;
        sound = 3;
    }
    if (D_0037F510[0x21] < 0 ||
        (D_0037F510[0x26] == 0 && D_0037F510[0x27] == 0)) {
        mnuClearListFlagsOneAndTwo(list);
    }
    switch (sound) {
    case 1:
        sndSetSequenceVolumePan(0, 0x7F, 0x3F);
        break;
    case 2:
        sndSetSequenceVolumePan(8, 0x7F, 0x3F);
        break;
    case 3:
        sndSetSequenceVolumePan(10, 0x7F, 0x3F);
        break;
    }
    return action;
}

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00288BD8);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00288DD0);

INCLUDE_RODATA(const s32, "game/code_00286BA8", D_004262B0);

INCLUDE_SDATA(const s32, "game/code_00286BA8", D_00437918);

INCLUDE_SDATA(const s32, "game/code_00286BA8", D_00437920);

INCLUDE_SDATA(const s32, "game/code_00286BA8", mnuMantraSelectionResource);

INCLUDE_SDATA(const s32, "game/code_00286BA8", D_00437928);

INCLUDE_SDATA(const s32, "game/code_00286BA8", D_0043792C);

