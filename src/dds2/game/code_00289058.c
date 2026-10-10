#include "mnu_mantra.h"
#include "prf_requirement.h"
#include "common.h"
#include "mnu_list.h"
#include "dat_state.h"

extern s32 mtrChkMantraCompleteMaster(DatPartyRecord *);
extern s32 mdlFlagTest(s32);
extern s32 prfAreAllRequiredProfileFlagsSet(DatPartyRecord *);
extern s32 scrGetEntryRequirementFlags(u16);
extern s32 func_00314990(DatPartyRecord *, u16);
extern u32 scrGetSelectedScriptEntryId(DatPartyRecord *);
extern MantraNodePos *mnuGetMantraNodePositionRecord(s16);
extern void func_0028D070(MnuStatusResource *, s32, s32);
extern void mnuStoreMantraPanelFlagsToScript(MnuStatusResource *);
extern void mnuReleaseMiddleMantraSpriteSlots(void);
extern void kwlnFadeOutStart(s32, s32, s32, s32);
extern void mnuMarkTitleStreamResetPending(void);
extern void mnuResetTitleStreamLocked(void);
extern void mdlFlagSet(s32);

typedef struct MantraPanelPool MantraPanelPool;
extern void mnuQueueNextUnitPanelSelection(struct MantraDrawPool *pool);
extern void mnuAdvanceMantraUnitPanelListState(struct MantraDrawPool *pool);
extern u32 mnuQueueUnitPanelSelection(struct MantraDrawPool *pool, s8);
extern void mnuTransitionActivePanelAnimations(MantraPanelPool *, s32);
extern void mnuSpawnMantraShortLoopIconAtPosition(u32, u32, struct MantraDrawPool *pool);
extern void mnuSpawnMantraIconAtPosition(u32, u32, struct MantraDrawPool *pool);
extern void mnuSetMantraFadeState(struct MantraDrawPool *pool, u16, u16);
extern void func_0028F8A8(u8 *);
extern void evtPrintDeveloperConsoleMessage(const char *, ...);

/* Work block at object+0x240; the node IDs fill the eight slots before
   the selected index and count. */

DatPartyRecord *mnuGetSelectedNodeValue(MnuStatusResource *object) {
    return object->list->cursor->partyRecord;
}

DatPartyRecord *mnuGetNodeValueByIndex(MnuStatusResource *object, s32 index) {
    struct MenuListNode *node = object->list->first;
    s32 current = 0;
    while (node != 0) {
        if (current == index) {
            return node->partyRecord;
        }
        node = node->next;
        current++;
    }
    return NULL;
}

u32 func_002890A8(MnuStatusResource *object) {
    return object->list->cursor->index;
}

u32 mnuRetreatNodeCursorAndClearListFlags(MnuStatusResource *object) {
    mnuRetreatListCursorDefault(object->list);
    mnuClearListFlagsOneAndTwo(object->list);
    return 1;
}

u32 mnuAdvanceNodeCursorAndClearListFlags(MnuStatusResource *object) {
    mnuAdvanceListCursorDefault(object->list);
    mnuClearListFlagsOneAndTwo(object->list);
    return 1;
}


/* Steps the list cursor to `target` one entry at a time. */
s32 mnuMoveNodeCursorToTargetIndex(MnuStatusResource *object, s8 target) {
    s32 diff;
    s32 current;

    current = object->list->cursor->index;
    evtPrintDeveloperConsoleMessage("Jump!! %d to %d\n", current, target);
    diff = current - target;
    while (diff != 0) {
        if (diff > 0) {
            mnuRetreatListCursorDefault(object->list);
            diff--;
            mnuClearListFlagsOneAndTwo(object->list);
        } else {
            mnuAdvanceListCursorDefault(object->list);
            diff++;
            mnuClearListFlagsOneAndTwo(object->list);
        }
    }
    return 1;
}

/* Script entry IDs are signed 16-bit indices into the position table. */
static inline MantraNodePos *mnuFindNodePosition(DatPartyRecord *value) {
    s16 id = scrGetSelectedScriptEntryId(value);
    return (MantraNodePos *)mnuGetMantraNodePositionRecord(id);
}

/* Both icon variants use the same node-to-screen coordinate conversion. */
static inline void mnuRefreshNodeTransitionIcons(MnuStatusResource *object, MantraMenuWork *work, DatPartyRecord *value) {
    MantraNodePos *position = mnuFindNodePosition(value);

    mnuSpawnMantraShortLoopIconAtPosition((s32)((f32)position->x / 10.0f * 40.0f),
                                        (s32)((f32)position->y / 10.0f * 39.0f),
                                        object->menu.selectionController);
    position = (MantraNodePos *)work->defaultSelector;
    mnuSpawnMantraIconAtPosition((s32)((f32)position->x / 10.0f * 40.0f),
                                (s32)((f32)position->y / 10.0f * 39.0f),
                                object->menu.selectionController);
    if ((work->drawFlags >> 17) & 1) {
        mnuSetMantraFadeState(object->menu.selectionController, 5, 0);
        mnuSetMantraFadeState(object->menu.selectionController, 6, 5);
    }
    func_0028F8A8((u8 *)object);
}

void func_002891C0(MnuStatusResource *object) {
    MantraMenuWork *work = &object->menu;
    DatPartyRecord *value;

    evtPrintDeveloperConsoleMessage("UnitIndex:%d\n", object->list->cursor->index);
    mnuAdvanceMantraUnitPanelListState(object->menu.selectionController);
    mnuRetreatNodeCursorAndClearListFlags(object);
    value = mnuGetSelectedNodeValue(object);
    work->defaultSelector = mnuFindNodePosition(object->list->cursor->unk70);
    mnuTransitionActivePanelAnimations((MantraPanelPool *)work->resource, 1);
    func_0028D070(object, 2, 0);
    mnuRefreshNodeTransitionIcons(object, work, value);
    evtPrintDeveloperConsoleMessage("Next UnitIndex:%d\n", object->list->cursor->index);
}

void func_002893A0(MnuStatusResource *object) {
    MantraMenuWork *work = &object->menu;
    DatPartyRecord *value;

    mnuQueueNextUnitPanelSelection(object->menu.selectionController);
    mnuAdvanceNodeCursorAndClearListFlags(object);
    value = mnuGetSelectedNodeValue(object);
    work->defaultSelector = mnuFindNodePosition(object->list->cursor->unk70);
    mnuTransitionActivePanelAnimations((MantraPanelPool *)work->resource, 1);
    func_0028D070(object, 2, 0);
    mnuRefreshNodeTransitionIcons(object, work, value);
}

void func_00289550(MnuStatusResource *object, s8 target) {
    MantraMenuWork *work = &object->menu;

    if (mnuQueueUnitPanelSelection(object->menu.selectionController, target) != 0) {
        mnuMoveNodeCursorToTargetIndex(object, target);
        work->defaultSelector = mnuFindNodePosition(object->list->cursor->unk70);
        mnuTransitionActivePanelAnimations((MantraPanelPool *)work->resource, 0);
        func_0028D070(object, 5, 1);
        mnuRefreshNodeTransitionIcons(object, work, mnuGetSelectedNodeValue(object));
    }
}

/* The selection allocator reserves 0x16C bytes for 176 inline flag entries. */

/* Skip unavailable neighbors after retreating the node selection. */
void mnuSelectAvailableMantraNeighbor(MnuStatusResource *object) {
    MantraMenuWork *work = &object->menu;
    MantraNodePos *position;
    MantraNodePos *initialPosition;
    MantraNodePos *neighbor;
    MantraNodePos **neighbors;
    u16 *flags;
    DatPartyRecord *selectedValue;
    u32 selectedIndex;
    s32 i;

    extern void mnuSpawnMantraVariantIconAtPosition(u32, u32, struct MantraDrawPool *pool);
    extern void mnuSpawnMantraShortLoopVariantIconAtPosition(u32, u32, struct MantraDrawPool *pool);

    mnuAdvanceMantraUnitPanelListState(object->menu.selectionController);
    mnuRetreatNodeCursorAndClearListFlags(object);
    selectedValue = mnuGetSelectedNodeValue(object);
    selectedIndex = func_002890A8(object);
    initialPosition = (MantraNodePos *)work->defaultSelector;
    flags = ((MantraFlagResource *)work->slots[selectedIndex])->flags;

    if ((flags[initialPosition->id] & 0xF) == 3) {
        position = initialPosition;
        neighbors = position->neighbors;
        for (i = 0; i < 6; i++) {
            if (neighbors[i] != NULL) {
                selectedIndex = func_002890A8(object);
                neighbor = neighbors[i];
                flags =
                    ((MantraFlagResource *)work->slots[selectedIndex])->flags;
                if ((flags[neighbor->id] & 0xF) != 3) {
                    position = neighbor;
                    break;
                }
            }
        }

        work->defaultSelector = position;
        mnuSpawnMantraVariantIconAtPosition(
            (s32)((f32)(position->x * 20) / 10.0f),
            (s32)((f32)(position->y * 20) / 10.0f),
            object->menu.selectionController);
    }

    {
        s16 id = scrGetSelectedScriptEntryId(selectedValue);

        position = (MantraNodePos *)mnuGetMantraNodePositionRecord(id);
        mnuSpawnMantraShortLoopVariantIconAtPosition(
            (s32)((f32)(position->x * 20) / 10.0f),
            (s32)((f32)(position->y * 20) / 10.0f),
            object->menu.selectionController);
        func_0028F8A8((u8 *)object);
    }
}

/* Skip unavailable neighbors before refreshing both transition icons. */
void func_00289928(MnuStatusResource *object) {
    MantraMenuWork *work = &object->menu;
    MantraNodePos *position;
    MantraNodePos *initialPosition;
    MantraNodePos *neighbor;
    MantraNodePos **neighbors;
    u16 *flags;
    DatPartyRecord *selectedValue;
    u32 selectedIndex;
    s32 i;

    extern void mnuSpawnMantraVariantIconAtPosition(u32, u32, struct MantraDrawPool *pool);
    extern void mnuSpawnMantraShortLoopVariantIconAtPosition(u32, u32, struct MantraDrawPool *pool);

    mnuQueueNextUnitPanelSelection(object->menu.selectionController);
    mnuAdvanceNodeCursorAndClearListFlags(object);
    selectedValue = mnuGetSelectedNodeValue(object);
    selectedIndex = func_002890A8(object);
    initialPosition = (MantraNodePos *)work->defaultSelector;
    flags = ((MantraFlagResource *)work->slots[selectedIndex])->flags;

    if ((flags[initialPosition->id] & 0xF) == 3) {
        position = initialPosition;
        neighbors = position->neighbors;
        for (i = 0; i < 6; i++) {
            if (neighbors[i] != NULL) {
                selectedIndex = func_002890A8(object);
                neighbor = neighbors[i];
                flags =
                    ((MantraFlagResource *)work->slots[selectedIndex])->flags;
                if ((flags[neighbor->id] & 0xF) != 3) {
                    position = neighbor;
                    break;
                }
            }
        }

        work->defaultSelector = position;
        mnuSpawnMantraVariantIconAtPosition(
            (s32)((f32)(position->x * 20) / 10.0f),
            (s32)((f32)(position->y * 20) / 10.0f),
            object->menu.selectionController);
    }

    {
        s16 id = scrGetSelectedScriptEntryId(selectedValue);

        position = (MantraNodePos *)mnuGetMantraNodePositionRecord(id);
        mnuSpawnMantraShortLoopVariantIconAtPosition(
            (s32)((f32)(position->x * 20) / 10.0f),
            (s32)((f32)(position->y * 20) / 10.0f),
            object->menu.selectionController);
        func_0028F8A8((u8 *)object);
    }
}

void mnuUpdateSelectedMantraResourceId(s32 object) {
    MantraMenuWork *state = &((MnuStatusResource *)object)->menu;
    s16 id = scrGetSelectedScriptEntryId((DatPartyRecord *)((MnuStatusResource *)object)->list->cursor->unk70);
    state->defaultSelector = mnuGetMantraNodePositionRecord(id);
    func_0028D070((MnuStatusResource *)object, 5, 0);
}

extern struct MantraDrawPool *mnuCreateMantraDrawPool(u32);
extern MantraFlagResource *evtAllocateMantraSelectionWork(DatPartyRecord *, s32);
extern s32 mnuValidateProfileEntry(MantraFlagResource *, DatPartyRecord *);
extern u16 scrGetEntryLowFlags(DatPartyRecord *, u16);
extern void mnuMergePartyMantraProfileCapStates(MantraFlagResource **);
extern void func_00315A50(void);
extern void func_0028EF50(MnuStatusResource *);
extern s32 func_0028F9A0(MnuStatusResource *);
extern s32 func_00290A78(MnuStatusResource *);
extern MantraPanelPool *func_002799D8(s32, MnuStatusResource *);
extern s32 mnuGetActiveMantraModelFlagState(void);

const char D_00426300[] = "******************** Debug Error!!!! ********************\n";
const char D_00426340[] = "mtrMantraPlayerDataCreate!!!! \n";

void func_00289BA0(MnuStatusResource *object) {
    MantraMenuWork *state = &object->menu;
    struct MenuListNode *node;
    DatPartyRecord *record;
    s32 i;

    object->menu.selectionController = mnuCreateMantraDrawPool(0xB);
    object->currency = datGameState->header.currency;
    state->drawFlags = 4;
    state->drawBits.unk10 = mnuGetActiveMantraModelFlagState();
    i = 0;
    for (node = object->list->first; node != NULL; node = node->next, i++) {
        record = node->partyRecord;
        if (scrGetEntryLowFlags(record, 1) == 0) {
            state->slots[i] = evtAllocateMantraSelectionWork(record, 0);
            evtPrintDeveloperConsoleMessage(D_00426300);
        } else {
            state->slots[i] = evtAllocateMantraSelectionWork(NULL, 0);
            evtPrintDeveloperConsoleMessage(D_00426340);
        }
        state->unitEntries[i].nodeId = mnuValidateProfileEntry(state->slots[i], record);
        if (state->unitEntries[i].nodeId != 0) {
            state->drawBits.hasQueuedMastery = 1;
            state->unitEntries[i].marked = 1;
            state->unitEntries[i].kind = 1;
        } else if (mtrChkMantraCompleteMaster(record) != 0) {
            state->drawBits.hasQueuedMastery = 1;
            state->unitEntries[i].nodeId = scrGetSelectedScriptEntryId(record);
            state->unitEntries[i].marked = 1;
            state->unitEntries[i].kind = 2;
        }
    }
    state->slots[5] = evtAllocateMantraSelectionWork(NULL, 0);
    mnuMergePartyMantraProfileCapStates(state->slots);
    func_00315A50();
    func_0028EF50(object);
    state->drawBits.hasSource = func_0028F9A0(object);
    if (state->src == NULL) {
        mnuBindMantraMenuSourceRecord(object, NULL);
    }
    if (!(state->drawFlags & 0x10002)) {
        func_00290A78(object);
    }
    state->resource = func_002799D8(0x3CE, object);
}

extern void func_0028E858(s32 object);

/* Opens mantra selection, saving party record addresses and starting the AT3 load. */
extern void mnuOpenMantraSelectionAndLoadTitleStream(MnuStatusResource *);

extern void evtStageTestInit(s32);
extern void func_002A2200(s32);
extern void mnuResetTitleStreamAfterFileIdle(void);
const char D_00426360[16] = "AT3 LOAD!!\n";

void mnuOpenMantraSelectionAndLoadTitleStream(MnuStatusResource *object) {
    MantraMenuWork *work = &object->menu;
    struct MenuListNode *node;
    DatPartyRecord *selectedValue = object->list->cursor->partyRecord;
    s32 count;

    work->drawBits.drawEnabled = 0;
    work->drawBits.mode = 1;
    work->defaultSelector = mnuGetMantraNodePositionRecord(
        (s16)scrGetSelectedScriptEntryId(selectedValue));
    func_0028E858((s32)object);

    node = object->list->first;
    count = 0;
    while (node != 0) {
        work->collectedValues[count++] = node->unk70;
        node = node->next;
    }
    work->collectedCount = count;
    work->savedSelection = object->list->cursor->index;
    func_0028D070(object, 5, 0);
    evtStageTestInit(0);
    kwlnFadeOutStart(0, 0, 0, 0);
    evtPrintDeveloperConsoleMessage(D_00426360);
    mnuMarkTitleStreamResetPending();
    mnuResetTitleStreamLocked();
    func_002A2200(16);
    mnuResetTitleStreamAfterFileIdle();
}


extern void mnuDestroyMantraDrawPool(struct MantraDrawPool *pool);
extern void evtReleaseMantraSelectionWork(u32 *p);
extern void mnuReleaseMantraIconSpriteHandle(u32 *sprite);

void mnuReleaseMantraMenuDrawResources(MnuStatusResource *object) {
    MantraMenuWork *state;
    MantraFlagResource **handle;
    s32 i;

    if (object->menu.selectionController != 0) {
        mnuDestroyMantraDrawPool(object->menu.selectionController);
    }
    state = &object->menu;
    handle = state->slots;
    for (i = 5; i >= 0; i--) {
        if (*handle != 0) {
            evtReleaseMantraSelectionWork((u32 *)*handle);
        }
        *handle = 0;
        handle++;
    }
    if (state->resource != 0) {
        mnuReleaseMantraIconSpriteHandle((u32 *)state->resource);
    }
}

void mnuCleanupMantraVisualsAndResetTitleStream(MnuStatusResource *object) {
    mnuStoreMantraPanelFlagsToScript(object);
    mnuReleaseMiddleMantraSpriteSlots();
    kwlnFadeOutStart(0, 0, 0, 0);
    mnuMarkTitleStreamResetPending();
    mnuResetTitleStreamLocked();
}

s32 mnuCheckRequiredMantraEntries(DatPartyRecord *unit) {
    s32 index;
    for (index = 1; index < 0xb0; index++) {
        u16 id = index;
        if ((scrGetEntryRequirementFlags(id) & 1) == 0 &&
            func_00314990(unit, id) == 0) {
            return 0;
        }
    }
    return 1;
}

s32 mtrChkMantraCompleteMaster(DatPartyRecord *unit) {
    u16 flagIds[6] = {0x9A0, 0x9A1, 0x9A2, 0x9A3, 0x9A4, 0x9A5};
    u8 flagIndices[9] = {0, 0, 1, 2, 3, 4, 5, 2, 1};

    evtPrintDeveloperConsoleMessage("mtrChkMantraCompleteMaster!!\n");
    if (mdlFlagTest(flagIds[flagIndices[unit->unitId]]) != 0) {
        return 0;
    }
    evtPrintDeveloperConsoleMessage("Bit Check None ....\n");
    if (prfAreAllRequiredProfileFlagsSet(unit) == 0) {
        return 0;
    }
    evtPrintDeveloperConsoleMessage("Mantra All Master!!\n");
    return 1;
}

s32 mnuSetSelectedMantraOptionFlag(DatPartyRecord *party) {
    u16 flagIds[6] = {0x9a0, 0x9a1, 0x9a2, 0x9a3, 0x9a4, 0x9a5};
    u8 flagIndices[9] = {0, 0, 1, 2, 3, 4, 5, 2, 1};
    mdlFlagSet(flagIds[flagIndices[party->unitId]]);
    return 1;
}

s32 mnuFindFirstMatchingListItemIndex(MnuStatusResource *object) {
    struct MenuListNode *node = object->list->first;
    s32 index = 0;
    while (node != 0) {
        if (mtrChkMantraCompleteMaster((DatPartyRecord *)node->unk70) != 0) {
            return index;
        }
        node = node->next;
        index++;
    }
    return -1;
}

INCLUDE_ASM(const s32, "game/code_00289058", func_0028A1D0);

extern void func_0026C900(MnuStatusResource *);
extern s32 func_0028B738(s32, s32, s32, s32, MnuStatusResource *, s32);
extern s32 func_00293DB0(MnuStatusResource *);
extern s32 func_0028CBF8(s32, s32, s32, s32, MnuStatusResource *, s32);

s32 mnuDrawMantraPhaseOpacity(MnuStatusResource *object) {
    MantraMenuWork *work;
    f32 ratio;

    func_0026C900(object);
    work = &object->menu;
    switch (work->drawBits.mode) {
    case 2:
        ratio = (f32)work->frame / 10.0f;
        func_0028B738(0, 0, 0, (s32)(ratio * 128.0f), object, 0x53);
        break;
    case 3:
        ratio = (f32)work->frame / 10.0f;
        func_0028B738(0, 0, 0, (s32)((1.0f - ratio) * 128.0f), object, 0x53);
        break;
    case 5:
    case 12:
    case 13:
    case 18:
        func_0028B738(0, 0, 0, 0x80, object, 0x53);
        break;
    case 25:
        func_00293DB0(object);
        break;
    case 19:
    case 20:
    case 21:
        func_0028CBF8(0, 0, 0, 0x80, object, 0x52);
        break;
    case 1:
        break;
    }
    return 0;
}


extern s8 D_0037F510[64];
extern s32 mnuNavigateMantraSelector(MnuStatusResource *, s8);
extern u16 mnuGetSelectedPanelValue(MnuStatusResource *);
extern void sndSetSequenceVolumePan(s32, s32, s32);

s32 mnuPollMantraNodeNavigation(MnuStatusResource *object) {
    MantraMenuWork *work = &object->menu;
    s32 sound = 0;
    s32 result = 0;
    s32 direction = 0;

    if (D_0037F510[0x26] != 0) {
        direction = 1;
    } else if (D_0037F510[0x27] != 0) {
        direction |= 4;
    }
    if (D_0037F510[0x24] != 0) {
        direction |= 8;
    } else if (D_0037F510[0x25] != 0) {
        direction |= 2;
    }
    if (D_0037F510[0x26] < 0 || D_0037F510[0x27] < 0 ||
        D_0037F510[0x24] < 0 || D_0037F510[0x25] < 0) {
        if (work->navigationState != 0) {
            work->navigationState = 9;
        }
    }
    if (work->navigationState > 0) {
        work->navigationState--;
        if (work->navigationState >= 8) {
            work->navigationMask |= direction;
        } else if (work->navigationState == 7) {
            work->navigationMask |= direction;
            work->drawFlags |= 0x04000000;
        } else {
            work->navigationMask = direction;
            work->drawFlags &= ~0x04000000;
        }
    } else if (direction != 0) {
        work->navigationMask = direction;
        work->navigationState = 9;
    } else {
        work->navigationMask = 0;
    }
    if (((work->drawFlags >> 26) & 1) != 0 || D_0037F510[0x26] != 0 ||
        D_0037F510[0x27] != 0 || D_0037F510[0x24] != 0 || D_0037F510[0x25] != 0) {
        if (mnuNavigateMantraSelector(object, (s8)work->navigationMask) != 0) {
            MantraNodePos *position = (MantraNodePos *)work->defaultSelector;
            sound = 1;
            work->navigationMask = 0;
            mnuSpawnMantraIconAtPosition(
                (s32)((f32)position->x / 10.0f * 40.0f),
                (s32)((f32)position->y / 10.0f * 39.0f),
                object->menu.selectionController);
        }
    }
    if (work->navigationState == 0) {
        if (D_0037F510[0x28] < 0) {
            sound = 4;
            func_002891C0(object);
            work->navigationState = 5;
        } else if (D_0037F510[0x2A] < 0) {
            sound = 4;
            func_002893A0(object);
            work->navigationState = 5;
        }
    }
    if (D_0037F510[0x21] < 0 && work->navigationState < 3) {
        work->navigationState = 0;
        if (work->drawBits.iconFade != 0) {
            sound = 3;
            work->drawBits.iconFade = 0;
        } else if ((((MantraNodePos *)object->menu.defaultSelector)->selector.packed & 0x100) == 0) {
            switch (mnuGetSelectedPanelValue(object) & 0xF) {
            case 1:
                result = 2;
                sound = 2;
                if (work->drawBits.showOverlay != 0) {
                    mnuSetMantraFadeState(object->menu.selectionController, 5, 0);
                }
                break;
            default:
                sound = 3;
                break;
            }
        }
    } else if (D_0037F510[0x23] < 0 && work->navigationState == 0) {
        sound = 3;
        if (work->drawBits.iconFade != 0) {
            work->drawBits.iconFade ^= 1;
        } else {
            result = 3;
        }
    } else if (D_0037F510[0x20] < 0 && work->navigationState == 0) {
        sound = 2;
        result = 5;
    } else if (D_0037F510[0x2D] < 0) {
        sound = 2;
        result = 6;
    }
    switch (sound) {
    case 1: sndSetSequenceVolumePan(2, 0x7F, 0x3F); break;
    case 2: sndSetSequenceVolumePan(8, 0x7F, 0x3F); break;
    case 3: sndSetSequenceVolumePan(0xA, 0x7F, 0x3F); break;
    case 4: sndSetSequenceVolumePan(4, 0x7F, 0x3F); break;
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_00289058", func_0028B738);

extern MantraMenuSrc *func_0028FD10(void);

/* Binds the source record (or the default one) and unpacks its two bit fields. */
s32 mnuBindMantraMenuSourceRecord(MnuStatusResource *object, MantraMenuSrc *src) {
    MantraMenuWork *state = &object->menu;

    if (src != 0) {
        state->src = src;
    } else {
        state->src = func_0028FD10();
    }
    if (state->src != 0) {
        state->drawBits.sourceKind = 4;
        state->sourceMode = (state->src->bits >> 8) & 0xF;
        state->sourceFlag = (state->src->bits >> 12) & 1;
        return 1;
    }
    return 0;
}

struct MenuSearchObject;
struct MenuSearchState;
typedef struct DspUnitName { u8 encodedText[17]; } DspUnitName;
extern DspUnitName *D_00435E48;
extern s32 evtCreateMessageWindowIfMissing(struct ItfMesSub *);
extern s32 dspStartEntry(s32);
extern void evtFinishMessageWindowAndNotify(void);
extern s32 dspCloseChannel(void);
extern s32 evtGetMessageWindowControlState(void);
extern void evtCopyEntryStringToActiveWindow(s32, const void *);
extern void mnuActivateMantraNode(struct MenuSearchObject *, u16);
extern s32 func_0028F380(struct MenuSearchObject *, struct MenuSearchState *);
extern s32 mnuSelectPreferredMantraNode(struct MenuSearchObject *, struct MenuSearchState *);
extern s32 mnuSelectMatchingNode(struct MenuSearchObject *, struct MenuSearchState *);
extern void mnuSelectMantraLimitLine(struct MenuSearchObject *, u16);
extern void mnuApplyMantraUnlockToPartyList(MnuStatusResource *, u16);
extern s32 func_00290A78(MnuStatusResource *);
extern s32 mnuGetActiveMantraModelFlagState(void);
extern void mnuSetMantraBackgroundVariant(struct MantraDrawPool *, s8);
extern void mnuEnableMantraBackground(struct MantraDrawPool *);
extern void mnuBeginMantraBackgroundMaskFadeIn(struct MantraDrawPool *);
extern void mnuBeginMantraBackgroundMaskFadeOut(struct MantraDrawPool *);
extern void mnuShowMantraLimitLine(struct MantraDrawPool *);
extern void mnuHideMantraInfo(struct MantraDrawPool *);
extern void mnuShowMantraInfo(struct MantraDrawPool *);
extern void mnuHideMantraScrollCursor(struct MantraDrawPool *);
extern void mnuShowMantraScrollCursor(struct MantraDrawPool *);
extern void mnuHideMantraUnitPanel(struct MantraDrawPool *);
extern void mnuShowMantraUnitPanel(struct MantraDrawPool *);
extern void func_00278F60(struct MantraDrawPool *);
extern void func_00278FA8(struct MantraDrawPool *);
extern void func_00278FF0(struct MantraDrawPool *);
extern void func_002790F0(u32, u32, struct MantraDrawPool *);
extern void func_00279148(struct MantraDrawPool *);
extern void mtrDrawRankPass(s32, u16);
extern void func_0028E0E8(MnuStatusResource *, u16, u16);
/* The following three declarations remain inferred from native callers. */
extern s32 func_0028EB38(MnuStatusResource *, s32);
extern void func_0028E638(MnuStatusResource *, u16);
extern s32 func_0028FEF0(MantraMenuSrc *, MnuStatusResource *);

const char D_00426B90[0x48] = "MTR_MSL_MASTER_COMPLETE_CHECK  ...->MTR_MSL_MASTER_COMPLETE_WAIT\n";
const char D_00426BD8[0x38] = "MTR_MSL_MASTER_COMPLETE_CHECK  ... ->CHANGE_OUT\n";
const char D_00426C10[0x48] = "MTR_MSL_MASTER_COMPLETE_WAIT  ... ->MTR_MSL_MASTER_COMPLETE_CHECK\n";
const char D_00426C58[0x18] = "MTR_MSL_EFFECT_INIT\n";
const char D_00426C70[0x20] = "DrawRankUp[%d] Type[%d]\n";
const char D_00426C90[0x10] = "NOT DATA!!!!\n";
const char D_00426CA0[0x28] = "MTR_MSL_EFFECT_INIT  ... ->CHANGE_IN\n";
const char D_00426CC8[0x38] = "MTR_MSL_EFFECT_INIT  ... ->MASTER_COMPLETE_CHECK\n";
const char D_00426D00[0x28] = "MTR_MSL_EFFECT_INIT  ... ->CHANGE_OUT\n";
const char D_00426D28[0x20] = "MTR_MSL_EFFECT_CHANGE_IN\n";
const char D_00426D48[0x30] = "MTR_MSL_EFFECT_CHANGE_IN  ... ->CHANGE_IN_WAIT\n";
const char D_00426D78[0x38] = "MTR_MSL_EFFECT_CHANGE_IN_WAIT  ... -> MTR_MSL_EVENT\n";
const char D_00426DB0[0x20] = "MTR_MSL_EFFECT_CHANGE_OUT\n";
const char D_00426DD0[0x30] = "mtrMantraPlayerRestor!!!!!!!!!!!!!!!!!!!!\n";
const char D_00426E00[0x30] = "MTR_MSL_EFFECT_CHANGE_OUT  ... ->EXIT(ROOT)\n";
const char D_00426E30[0x18] = "MTR_MSL_EFFECT_IN\n";
const char D_00426E48[0x38] = "MTR_MSL_EFFECT_IN  ... -> MTR_MSL_EFFECT_IN_WAIT\n";
const char D_00426E80[0x30] = "MTR_MSL_EFFECT_IN_WAIT  ... -> MTR_MSL_EFFECT\n";
const char D_00426EB0[0x18] = "MTR_MSL_EFFECT_OUT\n";
const char D_00426EC8[0x38] = "MTR_MSL_EFFECT_OUT  ... -> MTR_MSL_EFFECT_OUT_WAIT\n";
const char D_00426F00[0x38] = "MTR_MSL_EFFECT_OUT_WAIT  ... -> MTR_MSL_EFFECT_INIT\n";
const char D_00426F38[0x10] = "MTR_MSL_EFFECT\n";
const char D_00426F48[0x38] = "MTR_MSL_EFFECT         ... -> MTR_MSL_EFFECT_WAIT\n";
const char D_00426F80[0x30] = "MTR_MSL_EFFECT_WAIT     ... -> MTR_MSL_EVENT\n";
const char D_00426FB0[0x18] = "MTR_MSL_EVENT[%d]!!!\n";
const char D_00426FC8[0x30] = "MTR_MSL_EVENT       ... -> MTR_MSL_EVENT_WAIT\n";
const char D_00426FF8[0x30] = "MTR_MSL_EVENT_WAIT ... -> MTR_MSL_EVENT_EXIT\n";
const char D_00427028[0x18] = "MTR_MSL_EVENT_EXIT\n";
const char D_00427040[0x30] = "MTR_MSL_EVENT_EXIT ... -> MTR_MSL_EFFECT_IN\n";
const char D_00427070[0x30] = "MTR_MSL_EVENT_EXIT ... -> MTR_MSL_EFFECT_OUT\n";

/* Advance source selection, entry/exit effects and their message handoff.
 * Source search APIs use another complete view of this same menu allocation. */
s32 mnuAdvanceMantraSourceTransition(MnuStatusResource *object) {
    MantraMenuWork *work = &object->menu;
    s32 result = 0;
    MantraNodePos *position;
    DatPartyRecord *party;
    s32 index;
    s32 dialogResult;

    switch (work->drawBits.sourceKind) {
    case 1:
        index = work->sourcePartyIndex;
        if (index != -1) {
            party = mnuGetNodeValueByIndex(object, index);
            mnuSetSelectedMantraOptionFlag(party);
            dspCloseChannel();
            evtCreateMessageWindowIfMissing(object->messageDefinition);
            evtCopyEntryStringToActiveWindow(0, D_00435E48[party->unitId].encodedText);
            evtCopyEntryStringToActiveWindow(1, D_00435E48[party->unitId + 16].encodedText);
            dspStartEntry(3);
            func_00289550(object, (s8)index);
            work->sourceNextState = 3;
            work->sourceWaitFrames = 10;
            work->drawBits.sourceKind = 18;
            evtPrintDeveloperConsoleMessage(D_00426B90);
        } else {
            work->sourceNextState = 7;
            work->sourceWaitFrames = 20;
            work->drawBits.sourceKind = 18;
            evtPrintDeveloperConsoleMessage(D_00426BD8);
        }
        break;
    case 3:
        if (evtGetMessageWindowControlState() == 0) {
            work->sourceNextState = 1;
            work->drawBits.sourceKind = 18;
            work->sourceWaitFrames = 30;
            evtPrintDeveloperConsoleMessage(D_00426C10);
            evtFinishMessageWindowAndNotify();
            work->sourcePartyIndex = mnuFindFirstMatchingListItemIndex(object);
        }
        break;
    case 4:
        evtPrintDeveloperConsoleMessage(D_00426C58);
        if (!work->drawBits.hasQueuedMastery && work->src == NULL) {
            mnuBindMantraMenuSourceRecord(object, NULL);
        }
        if (work->src != NULL) {
            evtPrintDeveloperConsoleMessage(D_00426C70, work->src->bits >> 15,
                                           (work->src->bits >> 8) & 0xF);
            if (work->src->bits & 0x8000) {
                func_0028F380((struct MenuSearchObject *)object,
                             (struct MenuSearchState *)work->src);
            } else if ((work->src->bits & 0xF00) == 0x200) {
                mnuSelectPreferredMantraNode((struct MenuSearchObject *)object,
                                            (struct MenuSearchState *)work->src);
            } else if ((work->src->bits & 0xF00) == 0x300) {
                mnuSelectMatchingNode((struct MenuSearchObject *)object,
                                     (struct MenuSearchState *)work->src);
            } else if ((work->src->bits & 0xF00) == 0x400) {
                work->src->selectedIndex = func_002890A8(object);
            } else {
                evtPrintDeveloperConsoleMessage(D_00426C90);
            }
            work->drawBits.sourceKind = 5;
            evtPrintDeveloperConsoleMessage(D_00426CA0);
        } else {
            work->sourcePartyIndex = mnuFindFirstMatchingListItemIndex(object);
            if (work->sourcePartyIndex != -1) {
                work->sourceNextState = 1;
                work->sourceWaitFrames = 0;
                work->drawBits.sourceKind = 18;
                evtPrintDeveloperConsoleMessage(D_00426CC8);
            } else {
                work->sourceNextState = 7;
                work->sourceWaitFrames = 20;
                work->drawBits.sourceKind = 18;
                evtPrintDeveloperConsoleMessage(D_00426D00);
            }
        }
        break;
    case 5:
        evtPrintDeveloperConsoleMessage(D_00426D28);
        mnuHideMantraScrollCursor(object->menu.selectionController);
        func_00289550(object, work->src->selectedIndex);
        work->sourceNextState = 6;
        work->sourceWaitFrames = 0;
        work->drawBits.sourceKind = 18;
        evtPrintDeveloperConsoleMessage(D_00426D48);
        break;
    case 6:
        if (func_0028EB38(object, 1) == 0) {
            work->sourceNextState = 15;
            work->sourceWaitFrames = 0;
            work->drawBits.sourceKind = 18;
            evtPrintDeveloperConsoleMessage(D_00426D78);
        }
        break;
    case 7: {
        MantraNodePos *restoredPosition;
        evtPrintDeveloperConsoleMessage(D_00426DB0);
        if (!work->drawBits.hasQueuedMastery) {
            evtPrintDeveloperConsoleMessage(D_00426DD0);
            func_00290A78(object);
            if (mnuGetActiveMantraModelFlagState() >= 2 ||
                mdlFlagTest(0x977) || mdlFlagTest(0x978) ||
                mdlFlagTest(0x979) || mdlFlagTest(0x97A)) {
                mnuShowMantraScrollCursor(object->menu.selectionController);
            }
            if ((u16)(work->sourceMode - 3) >= 2 && work->sourceFlag != 0) {
                restoredPosition = mnuGetMantraNodePositionRecord(
                    (s16)scrGetSelectedScriptEntryId(object->list->cursor->partyRecord));
                work->defaultSelector = restoredPosition;
                mnuSpawnMantraIconAtPosition(
                    (s32)((f32)restoredPosition->x / 10.0f * 40.0f),
                    (s32)((f32)restoredPosition->y / 10.0f * 39.0f),
                    object->menu.selectionController);
            }
        }
        work->sourceFlag = work->sourceMode = 0;
        result = 1;
        evtPrintDeveloperConsoleMessage(D_00426E00);
        break;
    }
    case 8:
        evtPrintDeveloperConsoleMessage(D_00426E30);
        mnuHideMantraInfo(object->menu.selectionController);
        mnuHideMantraUnitPanel(object->menu.selectionController);
        mnuSetMantraBackgroundVariant(object->menu.selectionController, 2);
        mnuBeginMantraBackgroundMaskFadeOut(object->menu.selectionController);
        if (work->src->bits & 0x8000) {
            position = mnuGetMantraNodePositionRecord(work->src->fields.unitIndex);
            func_00278FF0(object->menu.selectionController);
            func_00279148(object->menu.selectionController);
            mdlFlagSet(work->src->unk02);
            work->alternateSelector = work->defaultSelector;
            work->defaultSelector = position;
        } else if ((work->src->bits & 0xF00) == 0x200) {
            position = mnuGetMantraNodePositionRecord(work->src->fields.unitIndex);
            mnuTransitionActivePanelAnimations(work->resource, 1);
            func_0028E0E8(object, work->src->fields.unitIndex, 2);
            mnuSpawnMantraIconAtPosition(
                (s32)((f32)position->x / 10.0f * 40.0f),
                (s32)((f32)position->y / 10.0f * 39.0f),
                object->menu.selectionController);
            func_00278FA8(object->menu.selectionController);
            func_00279148(object->menu.selectionController);
            work->alternateSelector = work->defaultSelector;
            work->defaultSelector = position;
        } else {
            position = mnuGetMantraNodePositionRecord(work->src->fields.unitIndex);
            mnuSpawnMantraIconAtPosition(
                (s32)((f32)position->x / 10.0f * 40.0f),
                (s32)((f32)position->y / 10.0f * 39.0f),
                object->menu.selectionController);
            func_00278FA8(object->menu.selectionController);
            func_00279148(object->menu.selectionController);
            work->alternateSelector = work->defaultSelector;
            work->defaultSelector = position;
        }
        work->drawBits.sourceKind = 9;
        evtPrintDeveloperConsoleMessage(D_00426E48);
        break;
    case 9:
        if (func_0028EB38(object, 1) == 0) {
            work->sourceNextState = 12;
            work->sourceWaitFrames = 0;
            work->drawBits.sourceKind = 18;
            evtPrintDeveloperConsoleMessage(D_00426E80);
        }
        break;
    case 10:
        evtPrintDeveloperConsoleMessage(D_00426EB0);
        work->alternateSelector = NULL;
        mnuShowMantraInfo(object->menu.selectionController);
        mnuShowMantraUnitPanel(object->menu.selectionController);
        mnuSetMantraBackgroundVariant(object->menu.selectionController, 0);
        mnuBeginMantraBackgroundMaskFadeIn(object->menu.selectionController);
        work->defaultSelector = mnuGetMantraNodePositionRecord(work->src->fields.unitIndex);
        if (work->src->bits & 0x8000) {
            if ((work->src->bits & 0xF00) == 0x300) {
                mnuShowMantraLimitLine(object->menu.selectionController);
                mnuEnableMantraBackground(object->menu.selectionController);
                object->menu.drawBits.showOverlay = 1;
            }
            work->defaultSelector = mnuGetMantraNodePositionRecord(
                (s16)scrGetSelectedScriptEntryId(mnuGetSelectedNodeValue(object)));
        } else if ((work->src->bits & 0xF00) == 0x200) {
            mnuTransitionActivePanelAnimations(work->resource, 1);
            func_0028D070(object, 10, 1);
            if (work->src->bits & 0x1000) {
                MantraNodePos *cursorPosition;
                cursorPosition = mnuGetMantraNodePositionRecord(
                    (s16)scrGetSelectedScriptEntryId(mnuGetSelectedNodeValue(object)));
                work->defaultSelector = cursorPosition;
                mnuSpawnMantraIconAtPosition(
                    (s32)((f32)cursorPosition->x / 10.0f * 40.0f),
                    (s32)((f32)cursorPosition->y / 10.0f * 39.0f),
                    object->menu.selectionController);
            }
        } else if ((work->src->bits & 0xF00) == 0x300 ||
                   (work->src->bits & 0xF00) == 0x400) {
            mnuSelectMantraLimitLine((struct MenuSearchObject *)object, work->src->fields.unitIndex);
        }
        func_00278F60(object->menu.selectionController);
        position = mnuGetMantraNodePositionRecord(
            (s16)scrGetSelectedScriptEntryId(mnuGetSelectedNodeValue(object)));
        func_002790F0((s32)((f32)position->x / 10.0f * 40.0f),
                      (s32)((f32)position->y / 10.0f * 39.0f),
                      object->menu.selectionController);
        work->drawBits.sourceKind = 11;
        evtPrintDeveloperConsoleMessage(D_00426EC8);
        break;
    case 11:
        work->frame++;
        if (func_0028EB38(object, 1) == 0) {
            mdlFlagSet(work->src->unk02);
            work->frame = 0;
            work->drawBits.hasSource = 0;
            work->src = NULL;
            work->sourceNextState = 4;
            work->sourceWaitFrames = 0;
            work->drawBits.sourceKind = 18;
            evtPrintDeveloperConsoleMessage(D_00426F00);
        }
        break;
    case 12:
        evtPrintDeveloperConsoleMessage(D_00426F38);
        if (work->src->bits & 0x8000) {
            mtrDrawRankPass((s32)object, work->src->fields.unitIndex);
            work->delayFrames = 60;
        } else if ((work->src->bits & 0xF00) == 0x200) {
            mnuApplyMantraUnlockToPartyList(object, work->src->fields.unitIndex);
            work->delayFrames = 120;
        } else if ((work->src->bits & 0xF00) == 0x300) {
            mnuActivateMantraNode((struct MenuSearchObject *)object, work->src->fields.unitIndex);
            work->delayFrames = 60;
        } else {
            func_0028E638(object, work->src->fields.unitIndex);
            index = func_002890A8(object);
            if ((object->menu.slots[index]->flags[work->src->fields.unitIndex] & 0xF) == 2) {
                work->delayFrames = 60;
            } else {
                work->delayFrames = 100;
            }
        }
        work->sourceNextState = 13;
        work->sourceWaitFrames = 20;
        work->drawBits.sourceKind = 18;
        evtPrintDeveloperConsoleMessage(D_00426F48);
        break;
    case 13:
        if (work->delayFrames == 0) {
            work->sourceNextState = 15;
            work->sourceWaitFrames = 0;
            work->drawBits.sourceKind = 18;
            evtPrintDeveloperConsoleMessage(D_00426F80);
        }
        break;
    case 14:
        work->drawBits.sourceKind = 15;
        break;
    case 15:
        dspCloseChannel();
        evtCreateMessageWindowIfMissing(object->messageDefinition);
        evtPrintDeveloperConsoleMessage(D_00426FB0, work->drawBits.unk15);
        dialogResult = func_0028FEF0(work->src, object);
        if (dialogResult == 0) {
            dspCloseChannel();
            work->drawBits.sourceKind = 17;
        } else {
            if (dialogResult > 0) {
                mnuGetMantraNodePositionRecord((s16)dialogResult);
            }
            work->drawBits.sourceKind = 16;
            evtPrintDeveloperConsoleMessage(D_00426FC8);
        }
        break;
    case 16:
        if (evtGetMessageWindowControlState() == 0) {
            work->drawBits.sourceKind = 17;
            evtFinishMessageWindowAndNotify();
            evtPrintDeveloperConsoleMessage(D_00426FF8);
        }
        break;
    case 17:
        evtPrintDeveloperConsoleMessage(D_00427028);
        if (work->drawBits.unk15 == 0) {
            work->sourceNextState = 8;
            work->sourceWaitFrames = 0;
            work->drawBits.sourceKind = 18;
            evtPrintDeveloperConsoleMessage(D_00427040);
        } else if (work->drawBits.unk15 == 1) {
            work->sourceNextState = 10;
            work->sourceWaitFrames = 0;
            work->drawBits.sourceKind = 18;
            evtPrintDeveloperConsoleMessage(D_00427070);
        }
        work->drawBits.unk15 ^= 1;
        work->frame = 0;
        break;
    case 18:
        if (work->sourceWaitFrames > 0) {
            work->sourceWaitFrames--;
        }
        if (work->sourceWaitFrames == 0) {
            work->drawBits.sourceKind = work->sourceNextState;
        }
        break;
    }
    return result;
}


extern void mnuSpawnMantraVariantIconAtPosition(u32, u32, struct MantraDrawPool *);

/* Mantra grid input, including directional repeat and page/confirm buttons. */
s32 mnuHandleMantraGridInput(MnuStatusResource *object) {
    MantraMenuWork *work = &object->menu;
    s32 sound = 0;
    s32 result = 0;
    s32 directions = 0;
    MantraNodePos *position;
    s32 x;
    s32 y;

    if (D_0037F510[0x26] != 0) {
        directions |= 1;
    } else if (D_0037F510[0x27] != 0) {
        directions |= 4;
    }
    if (D_0037F510[0x24] != 0) {
        directions |= 8;
    } else if (D_0037F510[0x25] != 0) {
        directions |= 2;
    }
    if (D_0037F510[0x26] < 0 || D_0037F510[0x27] < 0 ||
        D_0037F510[0x24] < 0 || D_0037F510[0x25] < 0) {
        if (work->navigationState != 0) {
            work->navigationState = 9;
        }
    }
    if (work->navigationState > 0) {
        work->navigationState--;
        if (work->navigationState >= 8) {
            work->navigationMask |= directions;
        } else if (work->navigationState == 7) {
            work->navigationMask |= directions;
            work->drawFlags |= 0x4000000;
        } else {
            work->navigationMask = directions;
            work->drawFlags &= ~0x4000000;
        }
    } else if (directions != 0) {
        work->navigationMask = directions;
        work->navigationState = 9;
    } else {
        work->navigationMask = 0;
    }
    if (((work->drawFlags >> 26) & 1) || D_0037F510[0x26] != 0 ||
        D_0037F510[0x27] != 0 || D_0037F510[0x24] != 0 ||
        D_0037F510[0x25] != 0) {
        if (mnuNavigateMantraSelector(object, directions)) {
            sound = 1;
            position = work->defaultSelector;
            x = position->x * 20 / 10.0f;
            y = position->y * 20 / 10.0f;
            mnuSpawnMantraVariantIconAtPosition(x, y, object->menu.selectionController);
        }
    }
    if (D_0037F510[0x28] < 0) {
        mnuSelectAvailableMantraNeighbor(object);
        sound = 4;
    } else if (D_0037F510[0x2A] < 0) {
        func_00289928(object);
        sound = 4;
    }
    if (D_0037F510[0x23] < 0 || D_0037F510[0x20] < 0 ||
        D_0037F510[0x21] < 0) {
        sound = 3;
        result = 1;
    }
    switch (sound) {
    case 1:
        sndSetSequenceVolumePan(0, 0x7F, 0x3F);
        break;
    case 3:
        sndSetSequenceVolumePan(10, 0x7F, 0x3F);
        break;
    case 4:
        sndSetSequenceVolumePan(4, 0x7F, 0x3F);
        break;
    }
    return result;
}

extern void mnuSetMantraUnitPanelValue(struct MantraDrawPool *pool, s16 value);

s32 func_0028CBF8(s32 unused0, s32 unused1, s32 unused2, s32 unused3, MnuStatusResource *object, s32 unused5) {
    MantraFlagResource *table = object->menu.slots[5];
    MantraMenuWork *work = &object->menu;
    MantraNodePos *position;
    u64 tableByte;
    u16 *selectedFlags;
    u16 *tableFlags;
    u16 high;
    s16 value;
    s16 id;

    selectedFlags = object->menu.slots[func_002890A8(object)]->flags;
    position = work->defaultSelector;
    id = position->id;
    tableFlags = table->flags + id;
    selectedFlags += id;
    mnuGetMantraNodePositionRecord(id);
    high = *selectedFlags >> 8;
    value = 4;
    if ((high & 4) == 0) {
        value = 3;
        if ((high & 1) == 0) {
            tableByte = *(u8 *)tableFlags;
            value = ((tableByte >> 4) == 1) ? 5 : 0;
        }
    }
    mnuSetMantraUnitPanelValue(object->menu.selectionController, value);
    return 0;
}


void mnuStartMantraPanelEntryTransition(MnuStatusResource *object) {
    object->menu.panelTransitionTimer = 0;
    object->menu.panelTransitionMode = 1;
}

void mnuStartMantraPanelExitTransition(MnuStatusResource *object) {
    object->menu.panelTransitionTimer = 0;
    object->menu.panelTransitionMode = 2;
}


/* Advances the 6-frame countdown of mode 1 (-> 3) or mode 2 (-> 0). */
void mnuAdvanceMantraPanelTransitionTimer(MnuStatusResource *object) {
    MantraMenuWork *state = &object->menu;

    switch (state->panelTransitionMode) {
    case 0:
        break;
    case 1:
        state->panelTransitionTimer++;
        if ((s16)state->panelTransitionTimer >= 6) {
            state->panelTransitionMode = 3;
            state->panelTransitionTimer = 0;
        }
        break;
    case 2:
        state->panelTransitionTimer++;
        if ((s16)state->panelTransitionTimer >= 6) {
            state->panelTransitionTimer = 0;
            state->panelTransitionMode = 0;
        }
        break;
    }
}

extern void mnuDrawMantraSprite(s32 x, s32 y, s32 depth, s32 fade, s32 sprite, s32 unused, s32 drawArg);

/* Level bits of one flag entry: high nibble of the low byte. */
typedef struct MantraFlagLevel {
    u32 state : 4;
    u32 level : 4;
} MantraFlagLevel;

/* Draws every visible node of the mantra map; the fade follows the panel transition. */
void func_0028CD50(s32 offsetX, s32 offsetY, MnuStatusResource *object, s32 drawArg) {
    MantraMenuWork *work = &object->menu;
    MantraFlagResource *selected;
    MantraFlagResource *profile;
    MantraNodePos *node;
    f32 ratio;
    s32 fade = 0;
    s32 count;
    s32 id;
    s32 x;
    s32 y;
    u16 high;
    MantraFlagLevel *profileEntry;
    u16 *selectedEntry;

    ratio = (f32)(s16)work->panelTransitionTimer / 5.0f;
    switch (work->panelTransitionMode) {
    case 0:
        return;
    case 1:
        fade = (s32)(ratio * 128.0f);
        break;
    case 2:
        fade = (s32)((1.0f - ratio) * 128.0f);
        break;
    case 3:
        fade = 0x80;
        break;
    }
    node = mnuGetMantraNodePositionRecord(0);
    selected = object->menu.slots[func_002890A8(object)];
    profile = object->menu.slots[5];
    count = 175;
    do {
        id = node->id;
        if (id != 0) {
            selectedEntry = selected->flags + id;
            profileEntry = (MantraFlagLevel *)(profile->flags + id);
            if ((*selectedEntry & 0xF) != 3) {
                x = (s32)((((((f32)node->x / 10.0f) * 20.0f) - 180.0f) + 241.0f) - 10.0f + (f32)offsetX);
                y = (s32)((((f32)node->y / 10.0f) * 20.0f) - 180.0f + 221.0f + (f32)offsetY);
                switch (node->kind) {
                case 1:
                case 3:
                case 4:
                    if ((*selectedEntry >> 8) & 1) {
                        mnuDrawMantraSprite(x, y, 0, fade, 0x105, 0, drawArg);
                    } else if (profileEntry->level == 1) {
                        mnuDrawMantraSprite(x, y, 0, fade, 0x106, 0, drawArg);
                    } else {
                        mnuDrawMantraSprite(x, y, 0, fade, 0x107, 0, drawArg);
                    }
                    break;
                case 2:
                    high = (*selectedEntry & 0xFF00) >> 8;
                    if (high & 8) {
                        if (node->selector.packed & 0x100) {
                            mnuDrawMantraSprite(x, y, 0, fade, 0x108, 0, drawArg);
                        } else {
                            mnuDrawMantraSprite(x, y, 0, fade, 0x10A, 0, drawArg);
                        }
                    } else if (high & 1) {
                        mnuDrawMantraSprite(x, y, 0, fade, 0x105, 0, drawArg);
                    } else {
                        mnuDrawMantraSprite(x, y, 0, fade, 0x109, 0, drawArg);
                    }
                    break;
                }
            }
        }
        node++;
    } while (--count >= 0);
}

extern s32 mnuGetActiveMantraModelFlagState(void);
typedef struct MantraPanelAnimation MantraPanelAnimation;
extern MantraPanelAnimation *mnuSpawnPanelSlotA(MantraPanelPool *, s32, s8, s16, s16, u32);
extern void mnuOffsetPanelAndSetVisualParams(MantraPanelAnimation *, s32, s32, u32, u32, u32, u8, u8);
extern u32 mnuQueuePanelAnimationTransition(MantraPanelAnimation *, u32, s16);


/* Spawn the available node panels from the currently selected unit's flags. */
void func_0028D070(MnuStatusResource *object, s32 startDelay, s32 transitionMode) {
    MantraPanelPool *pool = object->menu.resource;
    MantraNodePos *position;
    MantraPanelAnimation *panel;
    s32 recordIndex;

    mnuGetActiveMantraModelFlagState();
    position = mnuGetMantraNodePositionRecord(0);
    recordIndex = 175;
    do {
        if (position->id != 0) {
            u32 selectedIndex = func_002890A8(object);
            MantraFlagResource *resource = (MantraFlagResource *)object->menu.slots[selectedIndex];
            u16 flags = resource->flags[position->id];
            s32 panelKind = -1;

            if ((flags & 0xF) != 3) {
                if (position->kind == 2) {
                    if (position->selector.packed & 0x100) {
                        panelKind = ((flags >> 8) & 8) ? 7 : 6;
                    } else if ((flags >> 8) & 1) {
                        panelKind = 5;
                    } else if ((flags & 0xF) == 1) {
                        panelKind = 4;
                    } else if ((flags >> 8) & 8) {
                        panelKind = 3;
                    } else if ((flags & 0xF) == 2) {
                        panelKind = 6;
                    }
                } else if (position->kind == 3) {
                    if ((flags >> 8) & 1) {
                        panelKind = 9;
                    } else if ((flags & 0xF) == 1) {
                        panelKind = 8;
                    }
                } else if (position->kind == 4) {
                    if ((flags >> 8) & 1) {
                        panelKind = 12;
                    } else if ((flags & 0xF) == 1) {
                        panelKind = 11;
                    } else if ((flags & 0xF) == 2) {
                        panelKind = 10;
                    }
                } else {
                    if ((flags >> 8) & 1) {
                        panelKind = 2;
                    } else if ((flags & 0xF) == 1) {
                        panelKind = 1;
                    } else if ((flags & 0xF) == 2) {
                        panelKind = 0;
                    }
                }
                if (panelKind != -1) {
                    panel = mnuSpawnPanelSlotA(pool, position->id, (s8)panelKind, (s16)startDelay, 0, 0);
                    if (panel != NULL) {
                        mnuOffsetPanelAndSetVisualParams(panel, 0, 0, 0, 0x80, 0x53, 0, 0);
                        if (transitionMode == 1) {
                            mnuQueuePanelAnimationTransition(panel, 8, 0);
                        } else if (transitionMode == 2) {
                            mnuQueuePanelAnimationTransition(panel, 0, 0);
                        }
                    }
                }
            }
        }
        position++;
    } while (--recordIndex >= 0);
}

INCLUDE_ASM(const s32, "game/code_00289058", func_0028D2F8);

struct MantraPanelAnimation;
extern struct MantraPanelAnimation *mnuSpawnPanelSlotA(MantraPanelPool *, s32, s8, s16, s16, u32);
extern void mnuOffsetPanelAndSetVisualParams(struct MantraPanelAnimation *, s32, s32, u32, u32, u32, u8, u8);
extern u32 mnuQueuePanelAnimationTransition(struct MantraPanelAnimation *, u32, s16);

/* Activate eligible neighboring nodes and return their six-bit edge mask. */
s32 mnuActivateMantraNeighborEdges(MnuStatusResource *object, u16 nodeId, s16 modelFlagState, s16 panelX) {
    MantraNodePos *position;
    MantraNodePos **neighbors;
    MantraFlagResource *resource;
    struct MantraPanelAnimation *panel;
    u16 *flags;
    s32 i = 0;
    u8 edges = 0;
    s32 kind;
    s32 resultKind;

    position = mnuGetMantraNodePositionRecord((s16)nodeId);
    resource = object->menu.slots[func_002890A8(object)];
    neighbors = position->neighbors;
    for (; i < 6; i++) {
        if (neighbors[i] != NULL && neighbors[i]->modelFlagState == modelFlagState) {
            kind = neighbors[i]->selector.packed & 0xF;
            if (kind != 3) {
                flags = resource->flags + neighbors[i]->id;
                if ((*flags & 0xF) == 3) {
                    resultKind = 0;
                    edges |= 1 << i;
                    if (kind == 1) {
                        if (prfReqEvaluateRules(1, 2, mnuGetSelectedNodeValue(object), (u16)neighbors[i]->id, 0)) {
                            *flags = (*flags & 0xFFF0) | 1;
                            resultKind = 1;
                        } else {
                            *flags = (*flags & 0xFFF0) | 2;
                        }
                    } else if (kind == 2) {
                        *flags = (*flags & 0xFFF0) | 2;
                        resultKind = 2;
                    }
                    panel = mnuSpawnPanelSlotA(object->menu.resource, neighbors[i]->id, 13, panelX, 0, 0);
                    mnuOffsetPanelAndSetVisualParams(panel, 0, 0, 0, 0x80, 0x53, 0, 0);
                    mnuQueuePanelAnimationTransition(panel, 1, 0);
                    panel = mnuSpawnPanelSlotA(object->menu.resource, neighbors[i]->id, 13, (s16)(panelX * 5.0f / 3.0f + 20.0f), 0, 0);
                    mnuOffsetPanelAndSetVisualParams(panel, 0, 0, 0, 0x80, 0x53, 0, 0);
                    mnuQueuePanelAnimationTransition(panel, 3, 0);
                    if (resultKind == 0) {
                        panel = mnuSpawnPanelSlotA(object->menu.resource, neighbors[i]->id, 0, 80, 0, 0);
                        mnuOffsetPanelAndSetVisualParams(panel, 0, 0, 0, 0x80, 0x53, 0, 0);
                        mnuQueuePanelAnimationTransition(panel, 8, 0);
                    } else if (resultKind == 1) {
                        panel = mnuSpawnPanelSlotA(object->menu.resource, neighbors[i]->id, 1, 80, 0, 0);
                        mnuOffsetPanelAndSetVisualParams(panel, 0, 0, 0, 0x80, 0x53, 0, 0);
                        mnuQueuePanelAnimationTransition(panel, 8, 0);
                    } else if (resultKind == 2) {
                        panel = mnuSpawnPanelSlotA(object->menu.resource, neighbors[i]->id, 6, 80, 0, 0);
                        mnuOffsetPanelAndSetVisualParams(panel, 0, 0, 0, 0x80, 0x53, 0, 0);
                        mnuQueuePanelAnimationTransition(panel, 8, 0);
                    }
                }
            }
        }
    }
    if (modelFlagState == 3) {
        for (i = 0; i < 6; i++) {
            if ((edges >> i) & 1) {
                mnuActivateMantraNeighborEdges(object, neighbors[i]->id, modelFlagState, panelX + 2);
            }
        }
        flags = resource->flags + position->id;
        *flags |= 0x1000;
        edges = 0;
        for (i = 0; i < 6; i++) {
            if (neighbors[i] != NULL) {
                flags = resource->flags + neighbors[i]->id;
                if (neighbors[i]->modelFlagState == 2 && ((*flags >> 8) & 0x10) == 0) {
                    edges |= 1 << i;
                }
            }
        }
    }
    return edges;
}


/* Retail sign-extends mode and selection to halfwords at this call (+0x38/+0x4C sll/sra pairs). */
extern s32 mnuActivateMantraNeighborEdges(MnuStatusResource *, u16, s16, s16);

/* Pick the neighbouring mantra node to move to. The edge mask from mnuActivateMantraNeighborEdges is filtered against the
 * neighbours whose model flag state is compatible with the mode (mode 3 also accepts empty slots); edges 0 and 5
 * use paired masks, the side edges test the two adjacent neighbours. */
MantraNodePos *func_0028DC08(MnuStatusResource *object, u16 nodeId, s32 mode, s32 selection) {
    MantraNodePos *record;
    MantraNodePos *neighbor;
    MantraNodePos **neighbors;
    s32 edges;
    s32 compatible;
    s32 i;

    record = (MantraNodePos *)mnuGetMantraNodePositionRecord(nodeId);
    edges = mnuActivateMantraNeighborEdges(object, nodeId, mode, selection);
    compatible = 0;
    if (mode != 3) {
        neighbors = record->neighbors;
        for (i = 0; i < 6; i++) {
            neighbor = neighbors[i];
            if (neighbor != NULL && neighbor->modelFlagState == mode - 1) {
                compatible |= 1 << i;
            }
        }
        if ((edges & 1) && (compatible & 0x22)) {
            return record->neighbors[0];
        }
        for (i = 1; i < 5; i++) {
            if (((edges >> i) & 1) && (compatible & (5 << (i - 1)))) {
                return record->neighbors[i];
            }
        }
    } else {
        neighbors = record->neighbors;
        for (i = 0; i < 6; i++) {
            neighbor = neighbors[i];
            if (neighbor == NULL || neighbor->modelFlagState == 3) {
                compatible |= 1 << i;
            }
        }
        if ((edges & 1) && (compatible & 0x22)) {
            return record->neighbors[0];
        }
        for (i = 1; i < 5; i++) {
            if (((edges >> i) & 1) && (compatible & (5 << (i - 1)))) {
                return record->neighbors[i];
            }
        }
    }
    if ((edges & 0x20) && (compatible & 0x11)) {
        return record->neighbors[5];
    }
    return NULL;
}

extern void mnuStorePanelEntry(s32, s32);
extern void func_0028DFA0(s32);

/* Collect selected neighbours, then advance their three rank chains for at most 20 passes. */
void mtrDrawRankPass(s32 object, u16 index) {
    MantraNodePos *nodes[3];
    MantraNodePos **cursor;
    MantraNodePos **neighbors;
    MantraNodePos **nextNode;
    MantraNodePos *record;
    s32 i = 0;
    s32 hasNode = 0;
    s16 rank = 0;
    s16 remaining;
    s32 modelFlagState = mnuGetActiveMantraModelFlagState();
    s32 selected;

    evtPrintDeveloperConsoleMessage("DrawRank[%d]\n", modelFlagState);
    memset(nodes, 0, sizeof(nodes));
    selected = mnuActivateMantraNeighborEdges((MnuStatusResource *)object, index, modelFlagState, 0);
    record = (MantraNodePos *)mnuGetMantraNodePositionRecord(index);
    neighbors = record->neighbors;
    nextNode = nodes;
    for (; i < 6; i++, neighbors++) {
        if ((selected >> i) & 1) {
            *nextNode++ = func_0028DC08((MnuStatusResource *)object, (*neighbors)->id, modelFlagState, 0);
            hasNode = 1;
        }
    }
    remaining = 20;
    if (hasNode) {
        do {
            rank += 3;
            hasNode = 0;
            cursor = nodes;
            for (i = 2; i >= 0; i--, cursor++) {
                if (*cursor != NULL) {
                    *cursor = func_0028DC08((MnuStatusResource *)object, (*cursor)->id, modelFlagState, rank);
                    hasNode = 1;
                }
            }
            remaining--;
        } while (hasNode && remaining != 0);
    }
    mnuStorePanelEntry(0x20006, 0);
    func_0028DFA0(object);
}

void func_0028DFA0(s32 object) {
    s32 modelFlagState = mnuGetActiveMantraModelFlagState();
    struct MenuListNode *node = ((MnuStatusResource *)object)->list->first;
    MantraFlagResource **resourceSlot;

    if (node != 0) {
        resourceSlot = ((MnuStatusResource *)object)->menu.slots;
        do {
            DatPartyRecord *unit = (DatPartyRecord *)node->unk70;
            MantraNodePos *record =
                (MantraNodePos *)mnuGetMantraNodePositionRecord(0);
            u16 *flag = ((MantraFlagResource *)*resourceSlot)->flags;
            s32 count = 175;

            do {
                if (record->id != 0 && record->kind != 3 &&
                    record->kind != 4 &&
                    modelFlagState >= record->modelFlagState &&
                    (*flag & 0xF) == 3) {
                    if (record->kind == 2) {
                        *flag = (*flag & 0xFFF0) | 2;
                    } else {
                        if (prfReqEvaluateRules(1, 2, unit, (u16)record->id, 0)) {
                            *flag = (*flag & 0xFFF0) | 1;
                        } else {
                            *flag = (*flag & 0xFFF0) | 2;
                        }
                    }
                }
                record++;
                flag++;
            } while (--count >= 0);
            node = node->next;
            resourceSlot++;
        } while (node != 0);
    }
}


const char D_004271E0[] = "Mantra Hiding[%x]!!!\n";
const char D_004271F8[] = "Mantra !!!\n";
const char D_00427208[] = "Mantra Map!!!\n";

/* Show the selection panels, then eligible nodes outside that selection. */
void func_0028E0E8(MnuStatusResource *object, u16 nodeId, u16 panelX) {
    MantraNodePos *position;
    MantraPanelAnimation *panel;
    MantraFlagResource *resource;
    u16 visibleIds[7];
    u32 selectedIndex;
    u8 modelFlagState;
    s32 i;
    s32 recordIndex;

    position = (MantraNodePos *)mnuGetMantraNodePositionRecord((s16)nodeId);
    selectedIndex = func_002890A8(object);
    resource = (MantraFlagResource *)object->menu.slots[selectedIndex];
    modelFlagState = mnuGetActiveMantraModelFlagState();

    visibleIds[6] = nodeId;
    for (recordIndex = 0; recordIndex < 6; recordIndex++) {
        visibleIds[recordIndex] = position->neighbors[recordIndex]->id;
    }

    evtPrintDeveloperConsoleMessage(D_004271E0, nodeId);
    panel = mnuSpawnPanelSlotA((MantraPanelPool *)object->menu.resource,
                               nodeId, 6, (s16)panelX, 0, 0);
    mnuOffsetPanelAndSetVisualParams(panel, 0, 0, 0, 0x80, 0x53, 0, 0);
    evtPrintDeveloperConsoleMessage(D_004271F8);

    for (i = 0; i < 6; i++) {
        mnuGetMantraNodePositionRecord((s16)visibleIds[i]);
        panel = mnuSpawnPanelSlotA((MantraPanelPool *)object->menu.resource,
                                   visibleIds[i], 2, (s16)panelX, 0, 0);
        mnuOffsetPanelAndSetVisualParams(panel, 0, 0, 0, 0x80, 0x53, 0, 0);
    }

    evtPrintDeveloperConsoleMessage(D_00427208);
    position = (MantraNodePos *)mnuGetMantraNodePositionRecord(0);
    recordIndex = 175;
    do {
        if (position->id != 0 && position->modelFlagState <= modelFlagState) {
            s32 hidden = 1;
            for (i = 0; i < 7; i++) {
                if (position->id == visibleIds[i]) {
                    hidden = 0;
                    break;
                }
            }
            if (hidden) {
                u16 *nodeFlags = resource->flags + position->id;
                if ((*nodeFlags & 0xF) != 3) {
                    panel = mnuSpawnPanelSlotA((MantraPanelPool *)object->menu.resource,
                                               position->id, 13, (s16)panelX, 0, 0);
                    mnuOffsetPanelAndSetVisualParams(panel, 0, 0, 0, 0x80, 0x53, 0, 0);
                }
            }
        }
        position++;
    } while (--recordIndex >= 0);
}

INCLUDE_SDATA(const s32, "game/code_00289058", D_00437930);

INCLUDE_SDATA(const s32, "game/code_00289058", D_00437938);

