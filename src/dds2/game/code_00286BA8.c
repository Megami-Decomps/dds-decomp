#include "mnu_mantra.h"
#include "mnu.h"
#include "mnu_staff.h"
#include "dat_state.h"
#include "dsp_name.h"

#define MTR_RECORD_COUNT 32
#define MTR_STATUS_RESOURCE_BYTES 0xC08
#define MTR_UNIT_ENTRY_COUNT 5
#define MTR_SELECTION_PHASE_PER_TICK 0.125f
#define MTR_SELECTION_ALPHA_SCALE 128.0f
#define MTR_UNIT_FADE_FRAMES 10

extern void func_00286A58(DatPartyRecord *);

struct MnuStatusResource;

typedef struct MenuListNode MenuListNode;
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
    void (*drawCallback)();
    u32 userData;
    u8 pad34[8];
    s32 scale;
} MenuList;

struct MenuListNode {
    s32 index;
    u8 pad04[0x6C];
    u8 *items;
};

extern MenuList *func_002884C0(void);
extern s32 mdlFlagTest(s32);
extern void kwlnFadeInStart(s8, s8, s8, s32);
extern void func_00289BA0(struct MnuStatusResource *);
extern s32 func_00288920(struct MnuStatusResource *);
extern s32 mnuMoveNodeCursorToTargetIndex(MnuStatusResource *, s8);

struct TaskWork;
extern s32 func_00312810(struct TaskWork *, s32);
extern void sdfDestroyTaskWorkerTasks(struct TaskWork *);

extern u32 mnuMantraSelectionResource;
extern void mnuReleaseMantraPanelPositionTable(void);
extern void mnuCleanupMantraVisualsAndResetTitleStream(struct MnuStatusResource *);
extern void mnuEnableTerminalTrackMode(s8);

extern u8 mnuResourceTaskName[];

extern void func_00286F18(s32, struct MnuStatusResource *);

extern u8 D_003CFCC0[];

extern void mnuReleaseSelectionWorkResources(struct MnuStatusResource *);

extern u32 mnuDestroyListState(MenuList *);

extern void mnuCloseCurrentProfilePanel(MenuProgressHost *);

extern void mnuReleaseMantraMenuDrawResources(MnuStatusResource *);

extern s32 dspCloseChannel(void);
extern void sdfQueueNonzeroResourceId(u32);
struct SdfTaskItemDesc;
extern struct SdfTaskItemDesc D_003CFCD4;
extern void sdfAttachTaskItem(struct TaskWork *, struct SdfTaskItemDesc *);
extern void sdfSetTaskItemMode(void *, s32, u32);
extern void mnuReleaseFirstMantraSpriteSlots(void);
extern void mnuReleaseStaffAndTitleVisualResources(MenuProgressHost *);
extern void evtPrintDeveloperConsoleMessage(const char *, ...);
extern void sdfReleaseResourceAllocation(u32);
extern void mnuReleasePanelEntryPool(void);
INCLUDE_ASM(const s32, "game/code_00286BA8", func_00286BA8);

/* Process each of the 32 game-state records whose flags bit 0 is set, then print the native mantra banner. */
void func_00286E20(void) {
    s32 recordIndex;

    for (recordIndex = 0; recordIndex < MTR_RECORD_COUNT; recordIndex++) {
        if (datGameState->party[recordIndex].flags & 1) {
            func_00286A58(&datGameState->party[recordIndex]);
        }
    }
    evtPrintDeveloperConsoleMessage("*****************[mtrMantraSetBitAll()]*****************\n");
}

extern u32 sdfAllocGeneralBlock(s32 size);
extern u32 sdfMemoryGetBlockAddress(void *block);
extern MenuProgressHost *mnuCreateProgressHost(void);
extern void mnuInitPanelSoundEntries(void);

extern s32 func_00288748(MnuStatusResource *);
extern s32 mnuPollTitleEffectsReady(MenuProgressHost *);
extern void mnuRebuildProfilePanelFromRenderSnapshot(MnuStatusResource *);
extern struct SdfTaskItemDesc D_003CFCE8;

extern s32 func_00287078(MtrResourceLoadState *, u16);

extern void mtrInitUnitSelectionWork(MnuStatusResource *);

/* Allocate and clear status work, retain its allocation handle and create the progress host.
 * Print the native load banner and initialize panel sound entries before returning the work pointer. */
MnuStatusResource *func_00286E98(void) {
    u32 allocationHandle = sdfAllocGeneralBlock(MTR_STATUS_RESOURCE_BYTES);
    MnuStatusResource *resourceWork = (MnuStatusResource *)sdfMemoryGetBlockAddress(allocationHandle);

    memset(resourceWork, 0, MTR_STATUS_RESOURCE_BYTES);
    resourceWork->allocationHandle = allocationHandle;
    resourceWork->progressHost = mnuCreateProgressHost();
    evtPrintDeveloperConsoleMessage("trmLoadStartStatusResource()!!!! \n");
    evtPrintDeveloperConsoleMessage("mtrInit\n");
    mnuInitPanelSoundEntries();
    return resourceWork;
}

/* Release status-owned resources only for a nonzero work address; print the release banner even when it is zero. */
void func_00286F18(s32 unused, MnuStatusResource *resourceWork) {
    if (resourceWork != NULL) {

        dspCloseChannel();
        sdfQueueNonzeroResourceId(resourceWork->resourceIdA);
        sdfQueueNonzeroResourceId(resourceWork->resourceIdB);
        mnuReleaseFirstMantraSpriteSlots();
        mnuReleaseStaffAndTitleVisualResources(resourceWork->progressHost);
        evtPrintDeveloperConsoleMessage("trmDestroyStatusResource()!!!! \n");
        sdfReleaseResourceAllocation(resourceWork->allocationHandle);
        mnuReleasePanelEntryPool();
    }
    evtPrintDeveloperConsoleMessage("mtrRelease\n");
}

/* Store the task handle so the existence probe and explicit stop can
 * invalidate or destroy the same resource group. */
void mnuCreateResourceTask(void) {
    MnuStatusResource *resourceWork = func_00286E98();
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
    sdfDestroyTaskWorkerTasks((struct TaskWork *)mnuMantraSelectionResource);
    mnuMantraSelectionResource = 0;
}

s32 func_00287030(void) {
    MnuStatusResource *selected = (MnuStatusResource *)func_00312810((struct TaskWork *)mnuMantraSelectionResource, -1);
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
    MnuStatusResource *resourceWork = (MnuStatusResource *)func_00312810((struct TaskWork *)mnuMantraSelectionResource, -1);

    mtrInitUnitSelectionWork(resourceWork);
    evtPrintDeveloperConsoleMessage("mtrUnitSelectInit\n");
    return 0;
}

/* Release the current work's unit-selection list, profile panel and drawing resources. */
void mtrUnitSelectRelease(void) {
    MnuStatusResource *resourceWork = (MnuStatusResource *)func_00312810((struct TaskWork *)mnuMantraSelectionResource, -1);

    mnuReleaseSelectionWorkResources(resourceWork);
    evtPrintDeveloperConsoleMessage("mtrUnitSelectRelease\n");
}

/* Status 3 continues this selection task; only status 4 ends it. */
s32 func_00287670(s32 mode) {
    MnuStatusResource *resource = (MnuStatusResource *)func_00312810(
        (struct TaskWork *)mnuMantraSelectionResource, -1);

    switch (func_00288748(resource)) {
    case 1:
        if (resource->flags.unk00 == 0) {
            if (mnuPollTitleEffectsReady(resource->progressHost) == 0) {
                resource->flags.unk00 = 1;
            }
        } else {
            resource->flags.unk00 = 0;
            resource->flags.visible = 1;
            mnuRebuildProfilePanelFromRenderSnapshot(resource);
        }
        break;
    case 2:
        sdfAttachTaskItem((struct TaskWork *)mnuMantraSelectionResource,
                          &D_003CFCE8);
        sdfSetTaskItemMode((void *)mnuMantraSelectionResource, mode, 2);
        /* fall through */
    case 3:
        break;
    case 4:
        return -1;
    }
    return 0;
}

/* Draw the current unit-selection resource; the handler's native u64 return is always zero. */
u64 func_00287768(void) {
    MnuStatusResource *resourceWork;

    resourceWork = (MnuStatusResource *)func_00312810((struct TaskWork *)mnuMantraSelectionResource, -1);
    func_00288920(resourceWork);
    return 0;
}

/* Enter mantra selection on the current work address and disable terminal-track mode; return zero. */
s32 mtrMantraSelectInit(void) {
    u64 resourceAddress = func_00312810((struct TaskWork *)mnuMantraSelectionResource, -1);

    mnuEnableTerminalTrackMode(0);
    mnuOpenMantraSelectionAndLoadTitleStream(resourceAddress);
    evtPrintDeveloperConsoleMessage("mtrMantraSelectInit\n");
    return 0;
}

/* Release mantra visuals, clear the work's visible bit and restore terminal-track mode. */
void mtrMantraSelectRelease(void) {
    MnuStatusResource *resourceWork =
        (MnuStatusResource *)func_00312810((struct TaskWork *)mnuMantraSelectionResource, -1);

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
    switch (func_0028A1D0((MnuStatusResource *)func_00312810((struct TaskWork *)mnuMantraSelectionResource, -1))) {
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

    resourceAddress = func_00312810((struct TaskWork *)mnuMantraSelectionResource, -1);
    func_0028B1B0(resourceAddress);
    return 0;
}

extern u32 mnuGetDefaultPanelSelector(MnuStatusResource *);
extern s32 evtCreateMessageWindowIfMissing(struct ItfMesSub *);
extern void func_00267B40(s32, MenuProgressHost *);
extern void mnuEnsureProfilePanelEffect(DatPartyRecord *, MenuProgressHost *);
extern char D_00426208[];
extern char D_00426218[];

/* Initialize the equip panel from the selected party entry and selector. */
s32 mtrMantraEquipInit(void) {
    MnuStatusResource *work = (MnuStatusResource *)func_00312810((struct TaskWork *)mnuMantraSelectionResource, -1);
    DatPartyRecord *snapshot = &work->snapshot;
    MtrEquipState *equip = &work->menu.equip;
    u8 *selector;

    work->flags.unk04 = 0;
    selector = (u8 *)mnuGetDefaultPanelSelector(work);
    dspCloseChannel();
    evtCreateMessageWindowIfMissing(work->messageWindow);
    mnuCloseCurrentProfilePanel(work->progressHost);
    memcpy(snapshot, work->list->cursor->items, sizeof(*snapshot));
    work->snapshot.profileId = selector[2];
    evtPrintDeveloperConsoleMessage(D_00426208, work->snapshot.maxHp, work->snapshot.maxMp);
    func_00267B40((s32)snapshot, work->progressHost);
    mnuEnsureProfilePanelEffect(snapshot, work->progressHost);
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

typedef struct DspUnitName {
    u8 encodedText[17];
} DspUnitName;

extern DspUnitName *D_00435E48;
extern DspMantraName *D_00435E50;
extern s8 D_0037F510[];
extern u32 mnuGetSelectedNodeValue(MnuStatusResource *);
extern u16 mnuGetSelectedPanelValue(MnuStatusResource *);
extern u32 mnuGetDefaultPanelSelector(MnuStatusResource *);
extern u32 scrGetSelectedScriptEntryId(DatPartyRecord *);
extern s32 mnuGetMantraSourceValue(u16);
extern u8 scrSelectScriptEntryAndInitialize(DatPartyRecord *, u32);
extern void evtCopyEntryStringToActiveWindow(s32, s32);
extern s32 func_0035C860(char *, const char *, ...);
extern s32 dspStartEntry(s32);
extern s32 evtStoreValueAndCaptureWindowPanelValue(s32);
extern s32 evtGetMessageWindowControlState(void);
extern s8 evtGetCapturedWindowPanelValue(void);
extern void evtFinishMessageWindowAndNotify(void);
extern void evtSetMessageWindowOptionWhenOpen(s32);
extern void sndSetSequenceVolumePan(s32, s32, s32);
extern void mnuShowMantraInfo(u32);
extern void mnuShowMantraScrollCursor(u32);
extern void mnuShowMantraUnitPanel(u32);
extern void mnuToggleMantraTitleBlink(u32);
extern void mnuToggleMantraTypeOnePanelMode(u32);
extern void mnuSetMantraBackgroundVariant(u32, s8);
extern void mnuShowMantraLimitLine(u32);
extern void mnuKeepMantraBackgroundMaskVisible(u32);
extern void sdfSetTaskItemMode(void *, s32, u32);
/* The native call forwards only the work pointer; later callers pass all three provider inputs. */
extern void func_0028D070();

INCLUDE_SDATA(const s32, "game/code_00286BA8", D_00437918);

s32 func_00287C20(void) {
    MnuStatusResource *work;
    MtrEquipState *equip;
    DatPartyRecord *selectedRecord;
    MantraNodePos *defaultSelector;
    s32 selectedPanelFlags;
    s32 selectedHighFlags;
    u16 sourceEntryId;
    s32 selectedEntryId;
    s32 sourceAmount;
    s32 resultKind = 0;
    char text[16];

    mnuTickPanelSoundEntries();
    work = (MnuStatusResource *)func_00312810((struct TaskWork *)mnuMantraSelectionResource, -1);
    equip = &work->menu.equip;
    selectedRecord = (DatPartyRecord *)mnuGetSelectedNodeValue(work);
    defaultSelector = (MantraNodePos *)mnuGetDefaultPanelSelector(work);
    selectedEntryId = defaultSelector->selector.fields.index;
    selectedPanelFlags = mnuGetSelectedPanelValue(work);
    switch (equip->state) {
    case 1:
        if (D_0037F510[0x21] < 0) {
            resultKind = 1;
            if (scrGetSelectedScriptEntryId(selectedRecord) == (u32)selectedEntryId) {
                if (((u32)selectedPanelFlags >> 8) & 1) {
                    equip->state = 11;
                } else {
                    equip->state = 8;
                }
            } else {
                selectedHighFlags = (selectedPanelFlags & 0xFF00) >> 8;
                if (selectedHighFlags & 1) {
                    equip->state = 10;
                } else if (selectedHighFlags & 2) {
                    equip->state = 3;
                } else {
                    sourceAmount = mnuGetMantraSourceValue((u16)selectedEntryId);
                    if ((u32)datGameState->header.currency < (u32)sourceAmount) {
                        equip->state = 7;
                    } else {
                        equip->state = 2;
                    }
                }
            }
        } else if (D_0037F510[0x23] >= 0) {
            break;
        } else {
            resultKind = 2;
            if (work->flags.fadeProgress != 0) {
                equip->state = 12;
            }
        }
        break;

    case 2:
        evtCopyEntryStringToActiveWindow(0, (s32)D_00435E48[selectedRecord->unitId].encodedText);
        evtCopyEntryStringToActiveWindow(
            1, (s32)D_00435E50[scrGetSelectedScriptEntryId(selectedRecord)].encodedText);
        evtCopyEntryStringToActiveWindow(
            2, (s32)D_00435E50[selectedEntryId].encodedText);
        sourceAmount = mnuGetMantraSourceValue((u16)selectedEntryId);
        func_0035C860(text, "%d", sourceAmount);
        evtCopyEntryStringToActiveWindow(3, (s32)text);
        evtSetMessageWindowOptionWhenOpen(0);
        dspStartEntry(0);
        evtStoreValueAndCaptureWindowPanelValue(8);
        equip->state = 4;
        break;

    case 3:
        evtCopyEntryStringToActiveWindow(0, (s32)D_00435E48[selectedRecord->unitId].encodedText);
        evtCopyEntryStringToActiveWindow(
            1, (s32)D_00435E50[scrGetSelectedScriptEntryId(selectedRecord)].encodedText);
        evtCopyEntryStringToActiveWindow(
            2, (s32)D_00435E50[selectedEntryId].encodedText);
        sourceAmount = mnuGetMantraSourceValue((u16)selectedEntryId);
        func_0035C860(text, "%d", sourceAmount);
        evtCopyEntryStringToActiveWindow(3, (s32)text);
        evtSetMessageWindowOptionWhenOpen(0);
        dspStartEntry(1);
        evtStoreValueAndCaptureWindowPanelValue(8);
        equip->state = 4;
        break;

    case 4:
        if (evtGetMessageWindowControlState() != 0) {
            break;
        }
        if (evtGetCapturedWindowPanelValue() != 0) {
            equip->state = 12;
        } else {
            equip->state = 6;
        }
        break;

    case 5:
        break;

    case 6:
        sourceEntryId = (u16)selectedEntryId;
        evtCopyEntryStringToActiveWindow(0, (s32)D_00435E48[selectedRecord->unitId].encodedText);
        evtCopyEntryStringToActiveWindow(
            1, (s32)D_00435E50[scrGetSelectedScriptEntryId(selectedRecord)].encodedText);
        evtCopyEntryStringToActiveWindow(
            2, (s32)D_00435E50[selectedEntryId].encodedText);
        sourceAmount = mnuGetMantraSourceValue(sourceEntryId);
        func_0035C860(text, "%d", sourceAmount);
        evtCopyEntryStringToActiveWindow(3, (s32)text);
        dspStartEntry(2);
        equip->state = 12;
        if ((((u32)selectedPanelFlags >> 8) & 2) == 0) {
            sourceAmount = mnuGetMantraSourceValue(sourceEntryId);
            datGameState->header.currency -= sourceAmount;
        }
        work->flags.unk04 = 1;
        scrSelectScriptEntryAndInitialize(selectedRecord, selectedEntryId);
        break;

    case 7:
        dspStartEntry(3);
        equip->state = 12;
        break;

    case 8:
        dspStartEntry(4);
        equip->state = 12;
        break;

    case 9:
        dspStartEntry(5);
        equip->state = 12;
        break;

    case 10:
        dspStartEntry(6);
        equip->state = 12;
        break;

    case 11:
        dspStartEntry(7);
        equip->state = 12;
        break;

    case 12:
        if (work->flags.fadeProgress == 0 || evtGetMessageWindowControlState() != 0) {
            break;
        }
        evtFinishMessageWindowAndNotify();
        mnuShowMantraInfo(work->menu.selectionController);
        mnuShowMantraScrollCursor(work->menu.selectionController);
        mnuShowMantraUnitPanel(work->menu.selectionController);
        mnuToggleMantraTitleBlink(work->menu.selectionController);
        mnuToggleMantraTypeOnePanelMode(work->menu.selectionController);
        mnuSetMantraBackgroundVariant(work->menu.selectionController, 0);
        if (work->menu.drawBits.showOverlay) {
            mnuShowMantraLimitLine(work->menu.selectionController);
        }
        mnuKeepMantraBackgroundMaskVisible(work->menu.selectionController);
        func_0028D070(work);
        sdfSetTaskItemMode((struct TaskWork *)mnuMantraSelectionResource, 2, 1);
        return -1;
    default:
        break;
    }

    switch (resultKind) {
    case 1:
        sndSetSequenceVolumePan(8, 0x7F, 0x3F);
        break;
    case 2:
        sndSetSequenceVolumePan(10, 0x7F, 0x3F);
        break;
    default:
        break;
    }
    return 0;
}

extern void func_0026C900(void);
extern void mnuUpdateMantraDrawPool(u32 pool);
extern void mnuDrawMantraSprite(s32, s32, s32, s32, s32, s32, s32);
extern s32 mnuDrawLoadedProgressPanels(s32, MenuProgressHost *, s32);
extern void evtStageTestSelectEntryWithoutInitialValue(u16, u32);
extern void mnuDrawCurrentProfilePanel(s32, s32, s32, MenuProgressHost *, s32);
extern s8 evtStageTestUpdate(s32);
extern u8 D_00380818[];

s32 func_00288158(void) {
    MnuStatusResource *work = (MnuStatusResource *)func_00312810((struct TaskWork *)mnuMantraSelectionResource, -1);
    f32 ratio;
    s32 value;

    func_0026C900();
    mnuUpdateMantraDrawPool(work->menu.selectionController);
    ratio = 0.0f;
    if (work->flags.fadeProgress) {
        if (work->menu.equip.timer < 30) {
            work->menu.equip.timer++;
        }
        ratio = (f32)work->menu.equip.timer / 30.0f;
    }
    value = (s32)(ratio * 128.0f);
    mnuDrawMantraSprite(0, 0, 0, value, 0x68, 0, 0x4A);
    mnuDrawMantraSprite(0, 0, 0, value, 0x69, 0, 0x4A);
    if (mnuDrawLoadedProgressPanels((s32)&work->snapshot, work->progressHost, 0x53) != 0) {
        if (!work->flags.profileReady) {
            evtStageTestSelectEntryWithoutInitialValue(work->snapshot.unitId, 0);
        }
        work->flags.profileReady = 1;
    }
    mnuDrawCurrentProfilePanel(0xE80, 0x5B8, 1, work->progressHost, 0x53);
    if (evtStageTestUpdate((s32)D_00380818) >= 2) {
        work->flags.fadeProgress = 1;
    }
    return 0;
}

extern char D_00437928[];
extern s32 func_0035C860(char *, const char *, ...);
extern s32 frFontDrawTextVariantBAndMeasure(s32, s32, s32, u32, s32, char *, s32, s32);

void mtrDrawUnitSelectionRow(s32 unusedX, s32 unusedY, s32 drawPool, MenuList *list,
                  MenuListNode *node, s32 depth) {
    MtrSelectionState *selection = (MtrSelectionState *)list->userData;
    DatPartyRecord *record;
    s32 selected;
    s32 highlightAlpha;
    s8 icons[9] = {0, 6, 16, 8, 12, 10, 14, 18, 20};
    char text[16];

    selected = list->cursor->index == node->index;
    highlightAlpha = selection->highlightAlpha;
    record = (DatPartyRecord *)node->items;
    mnuDrawMantraSprite(0, node->index * 22 + (selection->scale - 1.0f) * 32.0f,
                        drawPool, selection->alpha, 0x22, 0, depth);
    mnuDrawMantraSprite(0, node->index * 22, drawPool, highlightAlpha,
                        icons[record->unitId] + selected, 0, depth);
    mnuDrawMantraSprite(0, node->index * 22, drawPool, highlightAlpha,
                        selected + 0x20, 0, depth);
    func_0035C860(text, D_00437928, record->level);
    frFontDrawTextVariantBAndMeasure(0xBB, node->index * 22 + 0x81, drawPool,
                                    (highlightAlpha & 0xFF) | 0xA09DC300,
                                    selected * 4, text, 0, depth);
    if (selected) {
        mnuDrawMantraSprite(0, node->index * 22, drawPool, highlightAlpha, 0x23, 0, depth);
    }
}

extern MenuList *mnuCreateListState(s32, s32, s32);
extern MenuListNode *mnuListAppendNode(MenuList *list, const void *value);

MenuList *func_002884C0(void) {
    MenuList *list;
    u16 slots[32];
    MenuListNode *node;
    u16 *p;
    u16 partyIndex;
    s32 i;

    list = mnuCreateListState(0, 5, 0x16);
    list->drawCallback = mtrDrawUnitSelectionRow;
    memset(slots, 0, sizeof(slots));
    for (i = 0; i < 5; i++) {
        if (datGameState->party[i].flags & 1) {
            if (datGameState->party[i].unitId == 8) {
                slots[0] = i + 1;
            } else {
                slots[datGameState->party[i].unitId] = i + 1;
            }
        }
    }
    p = slots;
    for (i = 31; i >= 0; i--) {
        if (*p != 0) {
            node = mnuListAppendNode(list, 0);
            partyIndex = *p - 1;
            node->items = (u8 *)&datGameState->party[partyIndex];
        }
        p++;
    }
    return list;
}

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
        if (resourceWork->menu.src != 0 || resourceWork->menu.drawBits.hasQueuedMastery) {
            if (resourceWork->menu.drawBits.hasQueuedMastery) {
                for (unitIndex = 0; unitIndex < MTR_UNIT_ENTRY_COUNT; unitIndex++) {
                    if (resourceWork->menu.unitEntries[unitIndex].marked) {
                        mnuMoveNodeCursorToTargetIndex((MnuStatusResource *)resourceWork, unitIndex);
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

extern s32 mnuHandleMantraSelectionInput(MnuStatusResource *);
extern s32 dspStartEntry(s32);
extern s32 evtGetMessageWindowControlState(void);

s32 func_00288748(MnuStatusResource *resourceWork) {
    MtrSelectionState *selection;
    s32 result = 0;

    if (resourceWork->flags.visible == 0) {
        return 1;
    }
    selection = &resourceWork->selection;
    switch (selection->state) {
    case 1:
        if (++selection->timer >= 8) {
            selection->state = 5;
        }
        break;
    case 2:
        if (++selection->timer >= 16) {
            selection->timer = 0;
            selection->state = 4;
        }
        break;
    case 3:
        if (++selection->timer >= 10) {
            selection->timer = 0;
            selection->state = 1;
            result = 2;
        }
        break;
    case 4:
        if (selection->outcome != 2) {
            selection->outcome = 0;
            selection->state = 1;
            result = 4;
        } else {
            selection->outcome = 2;
            selection->state = 3;
            kwlnFadeInStart(0, 0, 0, 10);
        }
        break;
    case 5:
        result = mnuHandleMantraSelectionInput(resourceWork);
        switch (result) {
        case 2:
            if (mdlFlagTest(0x1B1) == 0) {
                selection->state = 6;
                result = 0;
                break;
            }
            /* Confirmation and cancellation both enter the selection fade. */
        case 3:
            selection->outcome = result;
            selection->state = 2;
            result = 0;
            break;
        }
        break;
    case 6:
        dspCloseChannel();
        evtCreateMessageWindowIfMissing(resourceWork->messageDefinition);
        dspStartEntry(2);
        selection->state = 7;
        break;
    case 7:
        if (evtGetMessageWindowControlState() == 0) {
            selection->state = 8;
        }
        break;
    case 8:
        selection->state = 5;
        break;
    }
    return result;
}

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

INCLUDE_SDATA(const s32, "game/code_00286BA8", mnuMantraSelectionResource);

INCLUDE_SDATA(const s32, "game/code_00286BA8", D_00437928);

INCLUDE_SDATA(const s32, "game/code_00286BA8", D_0043792C);

