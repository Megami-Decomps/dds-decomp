#include "common.h"

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
typedef struct MenuProgressHost MenuProgressHost;
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
    u32 unk02 : 30;
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
extern void evtPrintDeveloperConsoleMessage(const char *);
extern void sdfReleaseResourceAllocation(u32);
extern void mnuReleasePanelEntryPool(void);
INCLUDE_ASM(const s32, "game/code_00286BA8", func_00286BA8);

void func_00286E20(void) {
    s32 i;

    for (i = 0; i < 32; i++) {
        if (datGameState->records[i].flags & 1) {
            func_00286A58(&datGameState->records[i]);
        }
    }
    evtPrintDeveloperConsoleMessage("*****************[mtrMantraSetBitAll()]*****************\n");
}

extern u32 sdfAllocGeneralBlock(s32 size);
extern u32 sdfMemoryGetBlockAddress(void *block);
extern s32 mnuCreateProgressHost(void);
extern void mnuInitPanelSoundEntries(void);

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
    u8 pad40[8];
    MenuProgressHost *progressHost; /* 0x48 */
    u8 pad4C[0x1CC];
    MtrSelectionFlags flags;
    u8 pad21C[0x10];
    MtrSelectionState selection; /* 0x22C */
    u8 pad234[0x560];
    MtrPlayerFlags playerFlags; /* 0x794 */
    u8 pad798[0x40C];
    u32 unkBA4;
    u8 padBA8[8];
    MtrUnitMenuEntry unitEntries[5]; /* 0xBB0 */
    u8 padBD8[0x30];
} MnuStatusResource; /* 0xC08 */

extern void func_002885E8(MnuStatusResource *);

/* Allocate and zero the 0xC08 status resource, then wire up its host pointer,
 * console banner and panel sound entries. */
void *func_00286E98(void) {
    u32 handle = sdfAllocGeneralBlock(0xC08);
    MnuStatusResource *resource = (MnuStatusResource *)sdfMemoryGetBlockAddress(handle);

    memset(resource, 0, 0xC08);
    resource->allocationHandle = handle;
    resource->progressHost = (MenuProgressHost *)mnuCreateProgressHost();
    evtPrintDeveloperConsoleMessage("trmLoadStartStatusResource()!!!! \n");
    evtPrintDeveloperConsoleMessage("mtrInit\n");
    mnuInitPanelSoundEntries();
    return resource;
}

void func_00286F18(s32 arg0, s32 work) {
    if (work != 0) {
        MnuStatusResource *resource = (MnuStatusResource *)work;

        dspCloseChannel();
        sdfQueueNonzeroResourceId(resource->resourceIdA);
        sdfQueueNonzeroResourceId(resource->resourceIdB);
        mnuReleaseFirstMantraSpriteSlots();
        mnuReleaseStaffAndTitleVisualResources((u32 *)resource->progressHost);
        evtPrintDeveloperConsoleMessage("trmDestroyStatusResource()!!!! \n");
        sdfReleaseResourceAllocation(resource->allocationHandle);
        mnuReleasePanelEntryPool();
    }
    evtPrintDeveloperConsoleMessage("mtrRelease\n");
}

/* Store the task handle so the existence probe and explicit stop can
 * invalidate or destroy the same resource group. */
void mnuCreateResourceTask(void) {
    MnuStatusResource *resource = (MnuStatusResource *)func_00286E98();
    mnuMantraSelectionResource = sdfCreateTaskWorker(mnuResourceTaskName, 0x402, 0x2B12, D_003CFCC0, func_00286F18, resource);
}

s32 mnuCheckResourceTask(void) {
    if (kwlnTaskExists(mnuResourceTaskName) != 0) {
        return 1;
    }
    mnuMantraSelectionResource = 0;
    return 0;
}

void mnuStopResourceTask(void) {
    sdfDestroyTaskWorkerTasks(mnuMantraSelectionResource);
    mnuMantraSelectionResource = 0;
}

s32 func_00287030(void) {
    u32 selected = func_00312810(mnuMantraSelectionResource, -1);
    s32 result = func_00287078((u8 *)selected + 0x21C, 0);

    if (result != 0) {
        sdfAttachTaskItem((struct TaskWork *)mnuMantraSelectionResource, &D_003CFCD4);
        return -1;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_00286BA8", mnuResourceTaskName);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00287078);

s32 mtrUnitSelectInit(void) {
    MnuStatusResource *selected = (MnuStatusResource *)func_00312810(mnuMantraSelectionResource, -1);

    func_002885E8(selected);
    evtPrintDeveloperConsoleMessage("mtrUnitSelectInit\n");
    return 0;
}

void mtrUnitSelectRelease(void) {
    MnuStatusResource *selected = (MnuStatusResource *)func_00312810(mnuMantraSelectionResource, -1);

    mnuReleaseSelectionWorkResources(selected);
    evtPrintDeveloperConsoleMessage("mtrUnitSelectRelease\n");
}

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00287670);

/* Both handlers consume the current resource-task selection, but report
 * completion independently of the selected value. */
u64 func_00287768(void) {
    MnuStatusResource *selected;

    selected = (MnuStatusResource *)func_00312810(mnuMantraSelectionResource, -1);
    func_00288920(selected);
    return 0;
}

s32 mtrMantraSelectInit(void) {
    u64 selected = func_00312810(mnuMantraSelectionResource, -1);

    mnuEnableTerminalTrackMode(0);
    mnuOpenMantraSelectionAndLoadTitleStream(selected);
    evtPrintDeveloperConsoleMessage("mtrMantraSelectInit\n");
    return 0;
}

void mtrMantraSelectRelease(void) {
    MnuStatusResource *selected =
        (MnuStatusResource *)func_00312810(mnuMantraSelectionResource, -1);

    mnuReleaseMantraPanelPositionTable();
    mnuCleanupMantraVisualsAndResetTitleStream(selected);
    selected->flags.visible = 0;
    mnuEnableTerminalTrackMode(1);
    evtPrintDeveloperConsoleMessage("mtrMantraSelectRelease\n");
}

extern struct SdfTaskItemDesc D_003CFCFC;
extern void mnuTickPanelSoundEntries(void);
extern s32 func_0028A1D0(MnuStatusResource *);
extern void sdfSetTaskItemMode(void *, s32, u32);

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

u64 func_00287900(void) {
    u64 selected;

    selected = func_00312810(mnuMantraSelectionResource, -1);
    func_0028B1B0(selected);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00287930);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00287AF8);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00287C20);

extern void func_0026C900(void);
extern void mnuUpdateMantraDrawPool(u32 pool);
extern void func_0026E788(s32, s32, s32, s32, s32, s32, s32);
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
    mnuUpdateMantraDrawPool(*(u32 *)((u8 *)work + 0xC00));
    ratio = 0.0f;
    if (((*(u32 *)((u8 *)work + 0x218) >> 2) & 1) != 0) {
        if (*(s32 *)((u8 *)work + 0xBFC) < 30) {
            (*(s32 *)((u8 *)work + 0xBFC))++;
        }
        ratio = (f32)*(s32 *)((u8 *)work + 0xBFC) / 30.0f;
    }
    value = (s32)(ratio * 128.0f);
    func_0026E788(0, 0, 0, value, 0x68, 0, 0x4A);
    func_0026E788(0, 0, 0, value, 0x69, 0, 0x4A);
    if (mnuDrawLoadedProgressPanels((s32)((u8 *)work + 0x54), work->progressHost, 0x53) != 0) {
        if (((*(u32 *)((u8 *)work + 0x218) >> 3) & 1) == 0) {
            evtStageTestSelectEntryWithoutInitialValue(*(u16 *)((u8 *)work + 0x58), 0);
        }
        *(u32 *)((u8 *)work + 0x218) |= 8;
    }
    mnuDrawCurrentProfilePanel(0xE80, 0x5B8, 1, work->progressHost, 0x53);
    if (evtStageTestUpdate((s32)D_00380818) >= 2) {
        *(u32 *)((u8 *)work + 0x218) |= 4;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00286BA8", func_002882B8);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_002884C0);

void func_002885E8(MnuStatusResource *work) {
    MtrSelectionState *selection = &work->selection;
    MenuList *list;
    s32 i;

    selection->state = 1;
    selection->timer = 0;
    list = func_002884C0();
    list->userData = (u32)selection;
    work->list = list;
    work->flags.visible = 0;

    if (mdlFlagTest(0x1B1) != 0) {
        if (mdlFlagTest(0x995) == 0) {
            selection->state = 3;
            kwlnFadeInStart(0, 0, 0, 10);
        }
        func_00289BA0(work);
        if (work->unkBA4 != 0 || work->playerFlags.hasMarkedUnit) {
            if (work->playerFlags.hasMarkedUnit) {
                for (i = 0; i < 5; i++) {
                    if (work->unitEntries[i].marked) {
                        mnuMoveNodeCursorToTargetIndex((MenuContainer *)work, i);
                        break;
                    }
                }
            }
            selection->state = 3;
            kwlnFadeInStart(0, 0, 0, 10);
        }
    }
}

void mnuReleaseSelectionWorkResources(MnuStatusResource *work) {
    mnuDestroyListState(work->list);
    mnuCloseCurrentProfilePanel(work->progressHost);
    mnuReleaseMantraMenuDrawResources(work);
}

INCLUDE_RODATA(const s32, "game/code_00286BA8", D_00426280);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00288748);

extern void func_0026C900(void);
extern s32 func_00288BD8(s32, s32, s32, s32, MnuStatusResource *, s32, f32);
extern s32 func_00288DD0(s32, s32, s32, s32, MnuStatusResource *, s32);

s32 func_00288920(MnuStatusResource *work) {
    f32 phase = 1.0f;
    s32 alpha;
    MtrSelectionState *selection;

    if (!work->flags.visible) {
        return 0;
    }
    func_0026C900();
    selection = &work->selection;
    switch (selection->state) {
        case 1:
            phase = selection->timer * 0.125f;
            break;
        case 2:
            phase = selection->timer * 0.125f;
            break;
        case 3:
        case 4:
            phase = 0.0f;
            break;
    }
    if (phase > 1.0f) {
        alpha = (2.0f - phase) * 128.0f;
    } else {
        alpha = phase * 128.0f;
    }
    func_00288BD8(0, 0, 0, alpha, work, 0x53, phase);
    if (work->flags.visible) {
        func_00288DD0(0, 0, 1, alpha, work, 0x53);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00288A70);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00288BD8);

INCLUDE_ASM(const s32, "game/code_00286BA8", func_00288DD0);

INCLUDE_RODATA(const s32, "game/code_00286BA8", D_004262B0);

INCLUDE_SDATA(const s32, "game/code_00286BA8", D_00437918);

INCLUDE_SDATA(const s32, "game/code_00286BA8", D_00437920);

INCLUDE_SDATA(const s32, "game/code_00286BA8", mnuMantraSelectionResource);

INCLUDE_SDATA(const s32, "game/code_00286BA8", D_00437928);

INCLUDE_SDATA(const s32, "game/code_00286BA8", D_0043792C);

