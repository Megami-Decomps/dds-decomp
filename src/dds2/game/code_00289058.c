#include "mnu_mantra.h"
#include "prf_requirement.h"
#include "common.h"
#include "mnu_list.h"
#include "dat_state.h"

extern s32 func_0028A018(DatPartyRecord *);
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
extern void mdlFlagSet(u16);

typedef struct MantraPanelPool MantraPanelPool;
extern void mnuQueueNextUnitPanelSelection(u32);
extern void mnuAdvanceMantraUnitPanelListState(u32);
extern u32 mnuQueueUnitPanelSelection(u32, s8);
extern void mnuTransitionActivePanelAnimations(MantraPanelPool *, s32);
extern void mnuSpawnMantraShortLoopIconAtPosition(u32, u32, u32);
extern void mnuSpawnMantraIconAtPosition(u32, u32, u32);
extern void mnuSetMantraFadeState(s32, u16, u16);
extern void func_0028F8A8(u8 *);
extern void evtPrintDeveloperConsoleMessage(const char *, ...);

/* Work block at object+0x240; the node IDs fill the eight slots before
   the selected index and count. */

u32 mnuGetSelectedNodeValue(MnuStatusResource *object) {
    return object->list->cursor->unk70;
}

s32 mnuGetNodeValueByIndex(MnuStatusResource *object, s32 index) {
    struct MenuListNode *node = object->list->first;
    s32 current = 0;
    while (node != 0) {
        if (current == index) {
            return node->unk70;
        }
        node = node->next;
        current++;
    }
    return 0;
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
static inline MantraNodePos *mnuFindNodePosition(u32 value) {
    s16 id = scrGetSelectedScriptEntryId((DatPartyRecord *)value);
    return (MantraNodePos *)mnuGetMantraNodePositionRecord(id);
}

/* Both icon variants use the same node-to-screen coordinate conversion. */
static inline void mnuRefreshNodeTransitionIcons(MnuStatusResource *object, MantraMenuWork *work, u32 value) {
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
    u32 value;

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
    u32 value;

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
void func_00289710(MnuStatusResource *object) {
    MantraMenuWork *work = &object->menu;
    MantraNodePos *position;
    MantraNodePos *initialPosition;
    MantraNodePos *neighbor;
    MantraNodePos **neighbors;
    u16 *flags;
    u32 selectedValue;
    u32 selectedIndex;
    s32 i;

    extern void mnuSpawnMantraVariantIconAtPosition(u32, u32, u32);
    extern void mnuSpawnMantraShortLoopVariantIconAtPosition(u32, u32, u32);

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
        s16 id = scrGetSelectedScriptEntryId((DatPartyRecord *)selectedValue);

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
    u32 selectedValue;
    u32 selectedIndex;
    s32 i;

    extern void mnuSpawnMantraVariantIconAtPosition(u32, u32, u32);
    extern void mnuSpawnMantraShortLoopVariantIconAtPosition(u32, u32, u32);

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
        s16 id = scrGetSelectedScriptEntryId((DatPartyRecord *)selectedValue);

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

INCLUDE_ASM(const s32, "game/code_00289058", func_00289BA0);

extern void func_0028E858(s32 object);

/* Opens the mantra menu: collects the list's node ids and starts the AT3 load. */
extern void mnuOpenMantraSelectionAndLoadTitleStream(MnuStatusResource *);

INCLUDE_ASM(const s32, "game/code_00289058", mnuOpenMantraSelectionAndLoadTitleStream);

extern void mnuDestroyMantraDrawPool(u32 address);
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

s32 func_0028A018(DatPartyRecord *unit) {
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
        if (func_0028A018((DatPartyRecord *)node->unk70) != 0) {
            return index;
        }
        node = node->next;
        index++;
    }
    return -1;
}

INCLUDE_ASM(const s32, "game/code_00289058", func_0028A1D0);

INCLUDE_ASM(const s32, "game/code_00289058", func_0028B1B0);


extern s8 D_0037F510[64];
struct MenuPanelObject;
extern s32 mnuNavigateMantraSelector(struct MenuPanelObject *, s8);
extern u16 mnuGetSelectedPanelValue(struct MenuPanelObject *);
extern void sndSetSequenceVolumePan(s32, s32, s32);

s32 func_0028B318(MnuStatusResource *object) {
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
        if (mnuNavigateMantraSelector((struct MenuPanelObject *)object,
                                      (s8)work->navigationMask) != 0) {
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
            switch (mnuGetSelectedPanelValue((struct MenuPanelObject *)object) & 0xF) {
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

INCLUDE_ASM(const s32, "game/code_00289058", func_0028BB80);

INCLUDE_ASM(const s32, "game/code_00289058", func_0028C8F8);

INCLUDE_ASM(const s32, "game/code_00289058", func_0028CBF8);


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

INCLUDE_ASM(const s32, "game/code_00289058", func_0028CD50);

extern s32 mnuGetActiveMantraModelFlagState(void);


INCLUDE_ASM(const s32, "game/code_00289058", func_0028D070);

INCLUDE_ASM(const s32, "game/code_00289058", func_0028D2F8);

INCLUDE_ASM(const s32, "game/code_00289058", func_0028D7C8);

/* Retail sign-extends mode and selection to halfwords at this call (+0x38/+0x4C sll/sra pairs). */
extern s32 func_0028D7C8(s32, u16, s16, s16);

/* Pick the neighbouring mantra node to move to. The edge mask from func_0028D7C8 is filtered against the
 * neighbours whose model flag state is compatible with the mode (mode 3 also accepts empty slots); edges 0 and 5
 * use paired masks, the side edges test the two adjacent neighbours. */
MantraNodePos *func_0028DC08(s32 object, u16 nodeId, s32 mode, s32 selection) {
    MantraNodePos *record;
    MantraNodePos *neighbor;
    MantraNodePos **neighbors;
    s32 edges;
    s32 compatible;
    s32 i;

    record = (MantraNodePos *)mnuGetMantraNodePositionRecord(nodeId);
    edges = func_0028D7C8(object, nodeId, mode, selection);
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
    selected = func_0028D7C8(object, index, modelFlagState, 0);
    record = (MantraNodePos *)mnuGetMantraNodePositionRecord(index);
    neighbors = record->neighbors;
    nextNode = nodes;
    for (; i < 6; i++, neighbors++) {
        if ((selected >> i) & 1) {
            *nextNode++ = func_0028DC08(object, (*neighbors)->id, modelFlagState, 0);
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
                    *cursor = func_0028DC08(object, (*cursor)->id, modelFlagState, rank);
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
                        if (func_00315C68(1, 2, unit, (u16)record->id, 0)) {
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

typedef struct MantraPanelAnimation MantraPanelAnimation;
extern MantraPanelAnimation *mnuSpawnPanelSlotA(MantraPanelPool *, s32, s8, s16, s16, u32);
extern void mnuOffsetPanelAndSetVisualParams(MantraPanelAnimation *, s32, s32, u32, u32, u32, u8, u8);

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

