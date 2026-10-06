#include "common.h"
#include "mnu_list.h"
#include "dat_state.h"

extern s32 func_0028A018(s32);
extern s32 scrGetEntryRequirementFlags(u16);
extern s32 func_00314990(DatPartyRecord *, u16);
extern u32 scrGetSelectedScriptEntryId(DatPartyRecord *);
extern s32 mnuGetMantraNodePositionRecord(s16);
extern void func_0028D070(s32, s32, s32);
extern void mnuStoreMantraPanelFlagsToScript(void);
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
typedef struct MantraMenuSrc {
    union {
        u16 bits;
        struct {
            u16 unitIndex : 8;
            u16 kind : 4;
            u16 flag12 : 1;
            u16 queued : 1;
            u16 unk14 : 2;
        } fields;
    };
    u16 unk02;
    u32 unk04;
} MantraMenuSrc;

typedef struct MantraMenuWork {
    s16 frame;
    s8 waitFrames;
    u8 nextMode;
    u8 pad004[0x550];
    union {
        u32 flags;
        struct {
            u32 iconFade : 1;
            u32 hasSource : 1;
            u32 mode : 8;
            u32 unk10 : 4;
            u32 drawEnabled : 1;
            u32 unk15 : 1;
            u32 hasQueuedMastery : 1;
            u32 showOverlay : 1;
            u32 sourceKind : 8;
            u32 unk26 : 6;
        } bits;
    };
    u16 scrollX;
    u16 scrollY;
    u8 pad55C[4];
    s32 resourceId;
    s32 alternateResourceId;
    u8 pad568[4];
    u32 spriteHandles[18];
    u32 nodeIds[8];
    s32 selectedIndex;
    s32 nodeCount;
    s32 previousResourceId;
    s8 previousSelection;
    s8 panelTransitionMode;
    u16 panelTransitionTimer;
    u8 drawData[0x380];
    MantraMenuSrc *src;
    u16 sourceMode;
    u16 sourceFlag;
    u8 pad96C[4];
    MantraMenuSrc slots[5];
    MantraMenuSrc *currentSlot;
    s32 masteryFrames;
    s32 delayFrames;
    s32 navigationState;
    u8 pad9A8[4];
    u32 displaySprite;
    u8 pad9B0[0x10];
    u32 drawPool;
} MantraMenuWork;

typedef struct EvtMantraNodePositionRecord {
    u32 kind : 4;
    s32 modelFlagState : 4;
    u32 reserved : 8;
    s16 id;
    s16 firstKey;
    s16 secondKey;
    struct EvtMantraNodePositionRecord *neighbors[6]; /* 0x08: ring of six adjacent nodes, NULL when absent */
} EvtMantraNodePositionRecord;

typedef struct MenuContainer {
    u8 pad0[4];
    struct MenuList *list;
    u8 pad08[0x3C];
    s32 windowContext;
    u8 pad48[0x1D4];
    u8 streamWork[0x24];
    MantraMenuWork work;
} MenuContainer;

u32 mnuGetSelectedNodeValue(MenuContainer *object) {
    return object->list->cursor->unk70;
}

s32 mnuGetNodeValueByIndex(MenuContainer *object, s32 index) {
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

u32 func_002890A8(MenuContainer *object) {
    return object->list->cursor->index;
}

u32 mnuRetreatNodeCursorAndClearListFlags(MenuContainer *object) {
    mnuRetreatListCursorDefault(object->list);
    mnuClearListFlagsOneAndTwo(object->list);
    return 1;
}

u32 mnuAdvanceNodeCursorAndClearListFlags(MenuContainer *object) {
    mnuAdvanceListCursorDefault(object->list);
    mnuClearListFlagsOneAndTwo(object->list);
    return 1;
}


/* Steps the list cursor to `target` one entry at a time. */
s32 mnuMoveNodeCursorToTargetIndex(MenuContainer *object, s8 target) {
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
static inline EvtMantraNodePositionRecord *mnuFindNodePosition(u32 value) {
    s16 id = scrGetSelectedScriptEntryId((DatPartyRecord *)value);
    return (EvtMantraNodePositionRecord *)mnuGetMantraNodePositionRecord(id);
}

/* Both icon variants use the same node-to-screen coordinate conversion. */
static inline void mnuRefreshNodeTransitionIcons(MenuContainer *object, MantraMenuWork *work, u32 value) {
    EvtMantraNodePositionRecord *position = mnuFindNodePosition(value);

    mnuSpawnMantraShortLoopIconAtPosition((s32)((f32)position->firstKey / 10.0f * 40.0f),
                                        (s32)((f32)position->secondKey / 10.0f * 39.0f),
                                        object->work.drawPool);
    position = (EvtMantraNodePositionRecord *)work->resourceId;
    mnuSpawnMantraIconAtPosition((s32)((f32)position->firstKey / 10.0f * 40.0f),
                                (s32)((f32)position->secondKey / 10.0f * 39.0f),
                                object->work.drawPool);
    if ((work->flags >> 17) & 1) {
        mnuSetMantraFadeState(object->work.drawPool, 5, 0);
        mnuSetMantraFadeState(object->work.drawPool, 6, 5);
    }
    func_0028F8A8((u8 *)object);
}

void func_002891C0(MenuContainer *object) {
    MantraMenuWork *work = &object->work;
    u32 value;

    evtPrintDeveloperConsoleMessage("UnitIndex:%d\n", object->list->cursor->index);
    mnuAdvanceMantraUnitPanelListState(object->work.drawPool);
    mnuRetreatNodeCursorAndClearListFlags(object);
    value = mnuGetSelectedNodeValue(object);
    work->resourceId = (s32)mnuFindNodePosition(object->list->cursor->unk70);
    mnuTransitionActivePanelAnimations((MantraPanelPool *)work->displaySprite, 1);
    func_0028D070((s32)object, 2, 0);
    mnuRefreshNodeTransitionIcons(object, work, value);
    evtPrintDeveloperConsoleMessage("Next UnitIndex:%d\n", object->list->cursor->index);
}

void func_002893A0(MenuContainer *object) {
    MantraMenuWork *work = &object->work;
    u32 value;

    mnuQueueNextUnitPanelSelection(object->work.drawPool);
    mnuAdvanceNodeCursorAndClearListFlags(object);
    value = mnuGetSelectedNodeValue(object);
    work->resourceId = (s32)mnuFindNodePosition(object->list->cursor->unk70);
    mnuTransitionActivePanelAnimations((MantraPanelPool *)work->displaySprite, 1);
    func_0028D070((s32)object, 2, 0);
    mnuRefreshNodeTransitionIcons(object, work, value);
}

void func_00289550(MenuContainer *object, s8 target) {
    MantraMenuWork *work = &object->work;

    if (mnuQueueUnitPanelSelection(object->work.drawPool, target) != 0) {
        mnuMoveNodeCursorToTargetIndex(object, target);
        work->resourceId = (s32)mnuFindNodePosition(object->list->cursor->unk70);
        mnuTransitionActivePanelAnimations((MantraPanelPool *)work->displaySprite, 0);
        func_0028D070((s32)object, 5, 1);
        mnuRefreshNodeTransitionIcons(object, work, mnuGetSelectedNodeValue(object));
    }
}

INCLUDE_ASM(const s32, "game/code_00289058", func_00289710);

INCLUDE_ASM(const s32, "game/code_00289058", func_00289928);


void mnuUpdateSelectedMantraResourceId(s32 object) {
    MantraMenuWork *state = &((MenuContainer *)object)->work;
    s16 id = scrGetSelectedScriptEntryId((DatPartyRecord *)((MenuContainer *)object)->list->cursor->unk70);
    state->resourceId = mnuGetMantraNodePositionRecord(id);
    func_0028D070(object, 5, 0);
}

INCLUDE_ASM(const s32, "game/code_00289058", func_00289BA0);



extern void func_0028E858(s32 object);

/* Opens the mantra menu: collects the list's node ids and starts the AT3 load. */
extern void mnuOpenMantraSelectionAndLoadTitleStream(MenuContainer *);

INCLUDE_ASM(const s32, "game/code_00289058", mnuOpenMantraSelectionAndLoadTitleStream);

extern void mnuDestroyMantraDrawPool(u32 address);
extern void evtReleaseMantraSelectionWork(u32 *p);
extern void mnuReleaseMantraIconSpriteHandle(u32 *sprite);

void mnuReleaseMantraMenuDrawResources(s32 object) {
    s32 state;
    u32 *handle;
    s32 i;

    if (((MantraMenuWork *)(object + 0x240))->drawPool != 0) {
        mnuDestroyMantraDrawPool(((MantraMenuWork *)(object + 0x240))->drawPool);
    }
    state = object + 0x240;
    handle = ((MantraMenuWork *)state)->spriteHandles;
    for (i = 5; i >= 0; i--) {
        if (*handle != 0) {
            evtReleaseMantraSelectionWork((u32 *)*handle);
        }
        *handle = 0;
        handle++;
    }
    if (((MantraMenuWork *)state)->displaySprite != 0) {
        mnuReleaseMantraIconSpriteHandle((u32 *)((MantraMenuWork *)state)->displaySprite);
    }
}

void mnuCleanupMantraVisualsAndResetTitleStream(void) {
    mnuStoreMantraPanelFlagsToScript();
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

INCLUDE_RODATA(const s32, "game/code_00289058", D_00426370);

INCLUDE_RODATA(const s32, "game/code_00289058", D_00426380);

INCLUDE_ASM(const s32, "game/code_00289058", func_0028A018);

s32 mnuSetSelectedMantraOptionFlag(DatPartyRecord *party) {
    u16 flagIds[6] = {0x9a0, 0x9a1, 0x9a2, 0x9a3, 0x9a4, 0x9a5};
    u8 flagIndices[9] = {0, 0, 1, 2, 3, 4, 5, 2, 1};
    mdlFlagSet(flagIds[flagIndices[party->unitId]]);
    return 1;
}

s32 mnuFindFirstMatchingListItemIndex(MenuContainer *object) {
    struct MenuListNode *node = object->list->first;
    s32 index = 0;
    while (node != 0) {
        if (func_0028A018(node->unk70) != 0) {
            return index;
        }
        node = node->next;
        index++;
    }
    return -1;
}

INCLUDE_ASM(const s32, "game/code_00289058", func_0028A1D0);

INCLUDE_ASM(const s32, "game/code_00289058", func_0028B1B0);


INCLUDE_ASM(const s32, "game/code_00289058", func_0028B318);

INCLUDE_ASM(const s32, "game/code_00289058", func_0028B738);



extern MantraMenuSrc *func_0028FD10(void);

/* Binds the source record (or the default one) and unpacks its two bit fields. */
s32 mnuBindMantraMenuSourceRecord(s32 object, MantraMenuSrc *src) {
    MantraMenuWork *state = (MantraMenuWork *)(object + 0x240);

    if (src != 0) {
        state->src = src;
    } else {
        state->src = func_0028FD10();
    }
    if (state->src != 0) {
        state->bits.sourceKind = 4;
        state->sourceMode = (state->src->bits >> 8) & 0xF;
        state->sourceFlag = (state->src->bits >> 12) & 1;
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00289058", func_0028BB80);

INCLUDE_ASM(const s32, "game/code_00289058", func_0028C8F8);

INCLUDE_ASM(const s32, "game/code_00289058", func_0028CBF8);


void mnuStartMantraPanelEntryTransition(s32 object) {
    ((MantraMenuWork *)(object + 0x240))->panelTransitionTimer = 0;
    ((MantraMenuWork *)(object + 0x240))->panelTransitionMode = 1;
}

void mnuStartMantraPanelExitTransition(s32 object) {
    ((MantraMenuWork *)(object + 0x240))->panelTransitionTimer = 0;
    ((MantraMenuWork *)(object + 0x240))->panelTransitionMode = 2;
}


/* Advances the 6-frame countdown of mode 1 (-> 3) or mode 2 (-> 0). */
void mnuAdvanceMantraPanelTransitionTimer(s32 object) {
    MantraMenuWork *state = (MantraMenuWork *)(object + 0x240);

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

typedef struct MantraFlagResource {
    u8 pad00[8];
    u16 *flags;
} MantraFlagResource;

extern s32 mnuGetActiveMantraModelFlagState(void);
extern s32 func_00315C68(s32, s32, DatPartyRecord *, u16, s32);

INCLUDE_ASM(const s32, "game/code_00289058", func_0028D070);

INCLUDE_ASM(const s32, "game/code_00289058", func_0028D2F8);

INCLUDE_ASM(const s32, "game/code_00289058", func_0028D7C8);

/* Retail sign-extends mode and selection to halfwords at this call (+0x38/+0x4C sll/sra pairs). */
extern s32 func_0028D7C8(s32, u16, s16, s16);

/* Pick the neighbouring mantra node to move to. The edge mask from func_0028D7C8 is filtered against the
 * neighbours whose model flag state is compatible with the mode (mode 3 also accepts empty slots); edges 0 and 5
 * use paired masks, the side edges test the two adjacent neighbours. */
EvtMantraNodePositionRecord *func_0028DC08(s32 object, u16 nodeId, s32 mode, s32 selection) {
    EvtMantraNodePositionRecord *record;
    EvtMantraNodePositionRecord *neighbor;
    EvtMantraNodePositionRecord **neighbors;
    s32 edges;
    s32 compatible;
    s32 i;

    record = (EvtMantraNodePositionRecord *)mnuGetMantraNodePositionRecord(nodeId);
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

INCLUDE_ASM(const s32, "game/code_00289058", func_0028DE10);



void func_0028DFA0(s32 object) {
    s32 modelFlagState = mnuGetActiveMantraModelFlagState();
    struct MenuListNode *node = ((MenuContainer *)object)->list->first;
    u32 *resourceSlot;

    if (node != 0) {
        resourceSlot = ((MenuContainer *)object)->work.spriteHandles;
        do {
            DatPartyRecord *unit = (DatPartyRecord *)node->unk70;
            EvtMantraNodePositionRecord *record =
                (EvtMantraNodePositionRecord *)mnuGetMantraNodePositionRecord(0);
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
                        if (func_00315C68(1, 2, unit, record->id, 0)) {
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

INCLUDE_ASM(const s32, "game/code_00289058", func_0028E0E8);

INCLUDE_SDATA(const s32, "game/code_00289058", D_00437930);

INCLUDE_SDATA(const s32, "game/code_00289058", D_00437938);

