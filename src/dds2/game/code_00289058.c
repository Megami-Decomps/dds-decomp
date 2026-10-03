#include "common.h"

extern s32 func_0028A018(s32);
extern s32 scrGetEntryRequirementFlags(u16);
extern s32 func_00314990(s32, u16);
extern s32 scrGetSelectedScriptEntryId(s32);
extern s32 mnuGetMantraNodePositionRecord(s32);
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

typedef struct MenuNode {
    s32 index;
    u8 pad04[0x54];
    struct MenuNode *next;
    u8 pad5C[0x14];
    u32 value;
} MenuNode;

typedef struct MenuNodeList {
    u8 pad0[0x10];
    MenuNode *head;
    u8 pad14[8];
    MenuNode *selected;
} MenuNodeList;

/* Work block at object+0x240; the node IDs fill the eight slots before
   the selected index and count. */
typedef struct MantraMenuWork {
    u8 pad000[0x554];
    u32 flags;
    u8 pad558[8];
    s32 resourceId; /* 0x560 */
    u8 pad564[8];
    u32 spriteHandles[6]; /* 0x56C */
    u8 pad584[0x30];
    u32 nodeIds[8]; /* 0x5B4 */
    s32 selectedIndex; /* 0x5D4 */
    s32 nodeCount; /* 0x5D8 */
    u8 pad5DC[0x3D0];
    u32 displaySprite; /* 0x9AC */
    u8 pad9B0[0x10];
    u32 drawPool; /* 0x9C0 */
} MantraMenuWork;

typedef struct EvtMantraNodePositionRecord {
    u32 kind : 4;
    s32 modelFlagState : 4;
    u32 reserved : 8;
    s16 id;
    s16 firstKey;
    s16 secondKey;
    u8 pad08[0x18];
} EvtMantraNodePositionRecord;

typedef struct MenuContainer {
    u8 pad0[4];
    MenuNodeList *list;
    u8 pad08[0x238];
    MantraMenuWork work;
} MenuContainer;

u32 mnuGetSelectedNodeValue(MenuContainer *object) {
    return object->list->selected->value;
}

s32 mnuGetNodeValueByIndex(MenuContainer *object, s32 index) {
    MenuNode *node = object->list->head;
    s32 current = 0;
    while (node != 0) {
        if (current == index) {
            return node->value;
        }
        node = node->next;
        current++;
    }
    return 0;
}

u32 func_002890A8(MenuContainer *object) {
    return object->list->selected->index;
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

    current = object->list->selected->index;
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
    s16 id = scrGetSelectedScriptEntryId(value);
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

    evtPrintDeveloperConsoleMessage("UnitIndex:%d\n", object->list->selected->index);
    mnuAdvanceMantraUnitPanelListState(object->work.drawPool);
    mnuRetreatNodeCursorAndClearListFlags(object);
    value = mnuGetSelectedNodeValue(object);
    work->resourceId = (s32)mnuFindNodePosition(object->list->selected->value);
    mnuTransitionActivePanelAnimations((MantraPanelPool *)work->displaySprite, 1);
    func_0028D070((s32)object, 2, 0);
    mnuRefreshNodeTransitionIcons(object, work, value);
    evtPrintDeveloperConsoleMessage("Next UnitIndex:%d\n", object->list->selected->index);
}

void func_002893A0(MenuContainer *object) {
    MantraMenuWork *work = &object->work;
    u32 value;

    mnuQueueNextUnitPanelSelection(object->work.drawPool);
    mnuAdvanceNodeCursorAndClearListFlags(object);
    value = mnuGetSelectedNodeValue(object);
    work->resourceId = (s32)mnuFindNodePosition(object->list->selected->value);
    mnuTransitionActivePanelAnimations((MantraPanelPool *)work->displaySprite, 1);
    func_0028D070((s32)object, 2, 0);
    mnuRefreshNodeTransitionIcons(object, work, value);
}

void func_00289550(MenuContainer *object, s8 target) {
    MantraMenuWork *work = &object->work;

    if (mnuQueueUnitPanelSelection(object->work.drawPool, target) != 0) {
        mnuMoveNodeCursorToTargetIndex(object, target);
        work->resourceId = (s32)mnuFindNodePosition(object->list->selected->value);
        mnuTransitionActivePanelAnimations((MantraPanelPool *)work->displaySprite, 0);
        func_0028D070((s32)object, 5, 1);
        mnuRefreshNodeTransitionIcons(object, work, mnuGetSelectedNodeValue(object));
    }
}

INCLUDE_ASM(const s32, "game/code_00289058", func_00289710);

INCLUDE_ASM(const s32, "game/code_00289058", func_00289928);


void mnuUpdateSelectedMantraResourceId(s32 object) {
    s32 state = object + 0x240;
    s16 id = scrGetSelectedScriptEntryId(((MenuContainer *)object)->list->selected->value);
    ((MantraMenuWork *)state)->resourceId = mnuGetMantraNodePositionRecord(id);
    func_0028D070(object, 5, 0);
}

INCLUDE_ASM(const s32, "game/code_00289058", func_00289BA0);

/* Bit layout of the flag word at state+0x554 (only the fields this file sets). */
typedef struct MantraMenuBits {
    u32 pad0 : 2;
    u32 mode : 8;
    u32 pad10 : 4;
    u32 flag14 : 1;
    u32 pad15 : 17;
} MantraMenuBits;


extern void func_0028E858(s32 object);
extern void evtStageTestInit(s32 a);
extern void func_002A2200(s32 a);
extern void mnuResetTitleStreamAfterFileIdle(void);

/* Opens the mantra menu: collects the list's node ids and starts the AT3 load. */
void mnuOpenMantraSelectionAndLoadTitleStream(s32 object) {
    s32 state = object + 0x240;
    MenuNode *node;
    s32 count;
    MenuNodeList *list;

    ((MantraMenuBits *)(state + 0x554))->flag14 = 0;
    ((MantraMenuBits *)(state + 0x554))->mode = 1;
    ((MantraMenuWork *)state)->resourceId = (s32)mnuFindNodePosition(((MenuContainer *)object)->list->selected->value);
    func_0028E858(object);
    list = ((MenuContainer *)object)->list;
    node = list->head;
    count = 0;
    for (; node != 0; node = node->next) {
        ((MantraMenuWork *)state)->nodeIds[count++] = node->value;
    }
    ((MantraMenuWork *)state)->nodeCount = count;
    ((MantraMenuWork *)state)->selectedIndex = list->selected->index;
    func_0028D070(object, 5, 0);
    evtStageTestInit(0);
    kwlnFadeOutStart(0, 0, 0, 0);
    evtPrintDeveloperConsoleMessage("AT3 LOAD!!\n");
    mnuMarkTitleStreamResetPending();
    mnuResetTitleStreamLocked();
    func_002A2200(0x10);
    mnuResetTitleStreamAfterFileIdle();
}

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

s32 mnuCheckRequiredMantraEntries(s32 object) {
    s32 index;
    for (index = 1; index < 0xb0; index++) {
        u16 id = index;
        if ((scrGetEntryRequirementFlags(id) & 1) == 0 &&
            func_00314990(object, id) == 0) {
            return 0;
        }
    }
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_00289058", D_00426370);

INCLUDE_RODATA(const s32, "game/code_00289058", D_00426380);

INCLUDE_ASM(const s32, "game/code_00289058", func_0028A018);

s32 mnuSetSelectedMantraOptionFlag(s32 object) {
    u16 flagIds[6] = {0x9a0, 0x9a1, 0x9a2, 0x9a3, 0x9a4, 0x9a5};
    u8 flagIndices[9] = {0, 0, 1, 2, 3, 4, 5, 2, 1};
    mdlFlagSet(flagIds[flagIndices[*(u16 *)(object + 4)]]);
    return 1;
}

s32 mnuFindFirstMatchingListItemIndex(MenuContainer *object) {
    MenuNode *node = object->list->head;
    s32 index = 0;
    while (node != 0) {
        if (func_0028A018(node->value) != 0) {
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

typedef struct MantraMenuSrc {
    u16 bits;
} MantraMenuSrc;

typedef struct MantraMenuSrcState {
    u8 pad000[0x554];
    u32 low : 18;
    u32 kind : 8;
    u32 high : 6;
    u8 pad558[0x40C];
    MantraMenuSrc *src;
    u16 field968;
    u16 flag96A;
} MantraMenuSrcState;

extern MantraMenuSrc *func_0028FD10(void);

/* Binds the source record (or the default one) and unpacks its two bit fields. */
s32 mnuBindMantraMenuSourceRecord(s32 object, MantraMenuSrc *src) {
    MantraMenuSrcState *state = (MantraMenuSrcState *)(object + 0x240);

    if (src != 0) {
        state->src = src;
    } else {
        state->src = func_0028FD10();
    }
    if (state->src != 0) {
        state->kind = 4;
        state->field968 = (state->src->bits >> 8) & 0xF;
        state->flag96A = (state->src->bits >> 12) & 1;
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00289058", func_0028BB80);

INCLUDE_ASM(const s32, "game/code_00289058", func_0028C8F8);

INCLUDE_ASM(const s32, "game/code_00289058", func_0028CBF8);

typedef struct MantraMenuTimer {
    u8 pad00[0x5E1];
    s8 mode;
    u16 timer;
} MantraMenuTimer;

void mnuStartMantraPanelEntryTransition(s32 object) {
    ((MantraMenuTimer *)(object + 0x240))->timer = 0;
    ((MantraMenuTimer *)(object + 0x240))->mode = 1;
}

void mnuStartMantraPanelExitTransition(s32 object) {
    ((MantraMenuTimer *)(object + 0x240))->timer = 0;
    ((MantraMenuTimer *)(object + 0x240))->mode = 2;
}


/* Advances the 6-frame countdown of mode 1 (-> 3) or mode 2 (-> 0). */
void mnuAdvanceMantraPanelTransitionTimer(s32 object) {
    MantraMenuTimer *state = (MantraMenuTimer *)(object + 0x240);

    switch (state->mode) {
    case 0:
        break;
    case 1:
        state->timer++;
        if ((s16)state->timer >= 6) {
            state->mode = 3;
            state->timer = 0;
        }
        break;
    case 2:
        state->timer++;
        if ((s16)state->timer >= 6) {
            state->timer = 0;
            state->mode = 0;
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
extern s32 func_00315C68(s32, s32, u32, u16, s32);

INCLUDE_ASM(const s32, "game/code_00289058", func_0028D070);

INCLUDE_ASM(const s32, "game/code_00289058", func_0028D2F8);

INCLUDE_ASM(const s32, "game/code_00289058", func_0028D7C8);

INCLUDE_ASM(const s32, "game/code_00289058", func_0028DC08);

INCLUDE_ASM(const s32, "game/code_00289058", func_0028DE10);



void func_0028DFA0(s32 object) {
    s32 modelFlagState = mnuGetActiveMantraModelFlagState();
    MenuNode *node = ((MenuContainer *)object)->list->head;
    u32 *resourceSlot;

    if (node != 0) {
        resourceSlot = ((MantraMenuWork *)(object + 0x240))->spriteHandles;
        do {
            u32 value = node->value;
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
                        if (func_00315C68(1, 2, value, record->id, 0)) {
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

