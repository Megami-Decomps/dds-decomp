#include "common.h"

extern s32 func_0027B888(u32);

extern u8 D_003BC3E1;

extern s32 dds3GetWorldObject(void);

extern s32 mdlFlagTest(u32);

typedef struct {
    u8 pad00[0x64];
    u32 firstResource;   /* 0x64 */
    u32 secondResource;  /* 0x68 */
    u8 pad6C[0x7B4];
    u32 panelGroup;      /* 0x820 */
    u32 displayResource; /* 0x824 */
    u32 effectResource;  /* 0x828 */
} MenuVisualWork;

extern s32 D_003BAA00;

typedef struct MenuProgressNode {
    u8 pad00[0x48];
    u32 flags;
    u8 pad4C[0xC];
    struct MenuProgressNode *next;
    u8 pad5C[4];
    u32 itemIndex; /* 0x60: party entry index */
    u32 requiredAmount; /* 0x64 */
    u8 pad68[8];
    s32 panel; /* 0x70: allocated panel resource */
} MenuProgressNode;

typedef struct {
    u8 pad00[0x10];
    MenuProgressNode *firstProgressNode; /* 0x10 */
    u8 pad14[8];
    MenuProgressNode *selectedNode;      /* 0x1C */
    s32 selectionState;                   /* 0x20 */
} MenuProgressOwner;

typedef struct MenuProgressWork {
    s32 allocation;          /* 0x00 */
    s32 groupResource;       /* 0x04 */
    u8 pad08[0x54];
    s32 messageResources[2]; /* 0x5C: second handle opens the message window */
    u8 pad64[0xC];
    s32 listResource;        /* 0x70 */
    MenuProgressOwner *list; /* 0x74 */
    MenuProgressOwner *owner;/* 0x78 */
    s32 mode;                /* 0x7C */
    s32 initState;           /* 0x80 */
} MenuProgressWork;

extern s32 mnuCreateDualPercentPanel(s32, s32);

extern s32 func_002CFEB8(s32);

extern s32 mnuPercentOrHundred(u16, u16);

extern void mnuDrawPanelSequenceByRow(s32, s32, s32, s32, s32, s32);

extern s32 mnuCreateListState();

extern s32 mnuListAppendNode(s32, s32);

extern void func_002491B8(void);

extern u8 D_003BC3F8[];

extern s32 mnuWalkNodeList(s32, s32);

extern s32 func_003014F0(char *, const char *, ...);

extern u32 uiBlendColors(u32, u32, s32);

extern s32 func_001978E8(s32, s32, s32, s32, s32, s32);

extern char D_003BC3E8[];

extern void func_001958A0(s32, s32, s32);

extern void frFontQueueGlyphInSelectedSlot(s32);

typedef struct MenuSlotState {
    u8 pad00[0x64];
    s32 batch;     /* 0x64 */
    u8 pad68[0x38];
    s32 effect[7]; /* 0xA0 */
    s32 cursorPositions[2]; /* 0xBC: initialized to -1 until selected */
    u8 padC4[0x14];
    s32 selectedSlot; /* 0xD8 */
    s32 mode;         /* 0xDC */
} MenuSlotState;

typedef struct EffectPair {
    s32 a;
    s32 b;
} EffectPair;

typedef struct EffectInner {
    u8 pad00[0x20];
    EffectPair *pair; /* 0x20 */
} EffectInner;

typedef struct EffectObject {
    u8 pad00[8];
    EffectInner *inner; /* 0x08 */
} EffectObject;

extern EffectObject *effCreateStatusBatch(s32);

extern s32 effDestroyPackedBatch(s32);

void mnuReleaseVisualResources(MenuVisualWork *work) {
    effResolveAndReleaseResource(work->firstResource);
    effResolveAndReleaseResource(work->secondResource);
}

void mnuReleaseBothVisualResourceTextures(MenuVisualWork *work) {
    effReleaseTextureHandlesAndResetSlots(work->firstResource);
    effReleaseTextureHandlesAndResetSlots(work->secondResource);
}

INCLUDE_ASM(const s32, "game/code_00248580", func_002485E0);

INCLUDE_ASM(const s32, "game/code_00248580", func_00248658);

/* Mark entries whose required amount exceeds the current profile amount. */
void mnuRefreshThresholdNodeFlags(MenuProgressOwner *owner) {
    MenuProgressNode *node = owner->firstProgressNode;
    if (node != 0) {
        s32 base = D_003BAA00;
        do {
            u32 amount = *(u32 *)(base + 0x3c);
            if (amount < node->requiredAmount) {
                node->flags |= 1;
            } else {
                node->flags &= ~1u;
            }
            node = node->next;
        } while (node != 0);
    }
}

void mnuCreateNumberSprite(s32 x, s32 y, s32 layer, s32 fade, s32 number, u32 color, s32 priority) {
    char text[16];
    s32 sprite;

    func_003014F0(text, D_003BC3E8, number);
    sprite = func_001978E8(x, y, layer, uiBlendColors(color, color & ~0xFF, fade), (s32)text, 0);
    func_001958A0(sprite, 1, priority);
    frFontQueueGlyphInSelectedSlot(sprite);
}

INCLUDE_RODATA(const s32, "game/code_00248580", D_003AF5A8);

INCLUDE_ASM(const s32, "game/code_00248580", func_00248810);

extern s32 mnuCreateDualPercentPanel(s32, s32);

extern s32 func_002CFEB8(s32);

extern s32 mnuPercentOrHundred(u16, u16);

extern void mnuDrawPanelSequenceByRow(s32, s32, s32, s32, s32, s32);

s32 mnuCreateDualPercentPanel(s32 resource, s32 context) {
    s32 panel = func_002CFEB8(0xa8);
    mnuDrawPanelSequenceByRow(panel, 0, 0, 0x1e,
        mnuPercentOrHundred(*(u16 *)(resource + 6), *(u16 *)(resource + 8)),
        *(s32 *)(context + 0xe0));
    mnuDrawPanelSequenceByRow(panel + 0x54, 1, 0, 0x1e,
        mnuPercentOrHundred(*(u16 *)(resource + 0xa), *(u16 *)(resource + 0xc)),
        *(s32 *)(context + 0xe0));
    return panel;
}

void mnuReleaseDualPercentPanel(s32 arg0) {
    if (arg0 != 0) {
        mnuReleaseSpriteTextures();
        mnuReleaseSpriteTextures((s32)arg0 + 0x54);
        sdfReleaseChipBlock(arg0);
        return;
    }
}

/* Rebuild each node's panel from its corresponding party entry. */
void mnuUpdateGroupResources(u8 *scene) {
    MenuProgressNode *node = *(MenuProgressNode **)(*(u8 **)(scene + 0x74) + 0x10);

    while (node != NULL) {
        node->panel = mnuCreateDualPercentPanel(D_003BAA00 + node->itemIndex * 420 + 0xA60, (s32)scene);
        node = node->next;
    }
}

void mnuDestroyThresholdNodePanels(s32 owner) {
    s32 entry;

    for (entry = (s32)((MenuProgressWork *)owner)->list->firstProgressNode; entry != 0; entry = (s32)((MenuProgressNode *)entry)->next) {
        mnuReleaseDualPercentPanel(((MenuProgressNode *)entry)->panel);
    }
}

typedef struct MenuProgressList {
    u8 pad00[0x2C];
    s32 updateCallback; /* 0x2C */
    s32 callback;       /* 0x30 */
    u8 pad34[8];
    s32 visible;        /* 0x3C */
} MenuProgressList;

typedef struct MenuThresholdEntry {
    s32 entryId;        /* 0x00 */
    s32 requiredAmount; /* 0x04 */
} MenuThresholdEntry;

extern s32 func_00248658(s32);

extern s32 func_00248810(s32);

void mnuBuildTerminalNodeList(MenuProgressWork *host) {
    MenuProgressList *list;
    s32 i;

    list = (MenuProgressList *)mnuCreateListState(0, 5, 0x24);
    list->callback = (s32)host;
    *(s32 *)&host->list = (s32)list;
    list->updateCallback = (s32)func_00248810;
    list->visible = 0;
    for (i = 0; i < 5; i++) {
        s32 box = D_003BAA00 + i * 0x1A4 + 0xA60;

        if ((u16)(*(u16 *)box & 1)) {
            s32 score = func_00248658(box);

            if (score != 0) {
                MenuProgressNode *node =
                    (MenuProgressNode *)mnuListAppendNode(host->list, (s32)D_003BC3F8);
                MenuThresholdEntry *entry = (MenuThresholdEntry *)&node->itemIndex;

                node->panel = 0;
                entry->requiredAmount = score;
                entry->entryId = i;
            }
        }
    }
    mnuRefreshThresholdNodeFlags(host->list);
}

void mnuReleaseProgressWorkList(s32 arg0) {
    mnuDestroyListState((u32)((MenuProgressWork *)arg0)->list);
}

void mnuReleaseSelectedProgressPanel(s32 arg0) {
    mnuReleaseDualPercentPanel(((MenuProgressWork *)arg0)->list->selectedNode->panel);
    func_0027B888((u32)((MenuProgressWork *)arg0)->list);
}

INCLUDE_ASM(const s32, "game/code_00248580", func_00248E68);

INCLUDE_ASM(const s32, "game/code_00248580", func_00249010);

INCLUDE_ASM(const s32, "game/code_00248580", func_00249058);

u8 func_00249198(void) {
    s64 flagSet;

    flagSet = mdlFlagTest(0x902);
    return flagSet == 0;
}

INCLUDE_ASM(const s32, "game/code_00248580", func_002491B8);

extern s32 mnuCreateListState();

extern s32 mnuListAppendNode(s32, s32);

extern void func_002491B8(void);

extern u8 D_003BC3F8[];

/* Omit the selected entry when building the progress list. */
s32 mnuBuildThresholdNodeList(s32 *items, s32 count, s32 excluded, s32 callback) {
    s32 list = mnuCreateListState(0, count, 0x15, callback);
    s32 i;
    *(s32 *)(list + 0x30) = callback;
    *(s32 *)(list + 0x2c) = (s32)func_002491B8;
    *(s32 *)(list + 0x3c) = 0;
    for (i = 0; i < count; i++) {
        if (i != excluded) {
            s32 node = mnuListAppendNode(list, (s32)D_003BC3F8);
            *(s32 *)(node + 0x60) = items[i];
        }
    }
    return list;
}

extern s32 mnuWalkNodeList(s32, s32);

void mnuHighlightProgressNodeFromOwnerSelection(s32 object) {
    s32 state = ((MenuProgressWork *)object)->mode;
    if (state < 2) {
        if (state < 0) {
            return;
        }
        if (((MenuProgressWork *)object)->owner->selectionState == 0) {
            s32 selected = mnuWalkNodeList(2 - func_00249198(),
                                              ((MenuProgressWork *)object)->listResource);
            ((MenuProgressNode *)selected)->flags |= 1;
        }
    }
}

extern s32 mnuWalkNodeList(s32, s32);

void mnuHighlightProgressNodeByMode(s32 object) {
    s32 state = ((MenuProgressWork *)object)->mode;
    s32 selectedIndex;
    if (state != 0) {
        if (state != 2) {
            return;
        }
        selectedIndex = 0;
    } else {
        selectedIndex = 3 - func_00249198();
    }
    if (((MenuProgressWork *)object)->list->selectionState == 0) {
        s32 node = mnuWalkNodeList(selectedIndex, ((MenuProgressWork *)object)->listResource);
        ((MenuProgressNode *)node)->flags |= 1;
    }
}

extern void mnuResolveStaffImageHandles(u8 *);

extern s32 D_003AF5E0[];

void mnuTerminalBuildMenus(MenuProgressWork *host) {
    s32 table[15];
    s32 row;
    s32 count;
    s32 excluded = -1;

    memcpy(table, D_003AF5E0, 0x3C);


    switch (host->mode) {
    case 0:
        row = 0;
        count = 5;
        if (func_00249198() != 0) {
            excluded = 1;
        }
        break;
    case 1:
        row = 1;
        count = 4;
        if (func_00249198() != 0) {
            excluded = 1;
        }
        break;
    default:
        row = 2;
        count = 2;
        break;
    }
    host->listResource = mnuBuildThresholdNodeList(table + row * 5, count, excluded, (s32)host);
    mnuBuildTerminalNodeList(host);
    mnuResolveStaffImageHandles((u8 *)host + 0xE0);
    mnuUpdateGroupResources((u8 *)host);
    func_00249058(host);
    mnuHighlightProgressNodeFromOwnerSelection(host);
    mnuHighlightProgressNodeByMode(host);
}

extern void mnuDestroyListState(u32);

extern void mnuReleaseStaffImageHandles(u8 *);

void mnuReleaseWorkResources(u8 *work) {
    u32 i;

    for (i = 0; i < 1; i++) {
        mnuDestroyListState(*(u32 *)(work + 0x70 + i * 4));
    }
    mnuDestroyThresholdNodePanels((s32)work);
    mnuReleaseStaffImageHandles(work + 0xE0);
    mnuReleaseProgressWorkList((s32)work);
    mnuDestroyListState((u32)((MenuProgressWork *)work)->owner);
}

extern void kwlnFadeOutStart(s32, s32, s32, s32);

extern void evtCreateEventScriptProcess(s32);

extern void evtClearActiveFlag(s32);

extern void evtSetBoundedDisplayValue(s32, s32);

void mnuFadeOrPlayCloseSfx(s32 skip, u8 *work) {
    if (skip == 0) {
        s32 mode = ((MenuProgressWork *)work)->mode;

        if (mode < 3) {
            if (mode > 0) {
                kwlnFadeOutStart(0, 0, 0, 15);
            } else {
                evtCreateEventScriptProcess(0x322);
            }
        } else {
            evtCreateEventScriptProcess(0x322);
        }
    } else {
        evtCreateEventScriptProcess(0x322);
    }
    evtClearActiveFlag(0);
    evtSetBoundedDisplayValue(1, 1);
}

void mnuResetProgressModeFromOwner(u8 *work) {
    u8 *owner = (u8 *)((MenuProgressWork *)work)->owner;
    ((MenuProgressWork *)work)->mode = 0;
    ((MenuProgressWork *)work)->initState = *(s32 *)(*(u8 **)(owner + 0x1C) + 0x60);
}

extern s32 func_002D03F8(s32);

extern s32 sdfResourceRetainAddress(s32);

extern void *memset(void *, s32, u32);

extern s32 mnuAllocateValueRecord(s32);

extern void mnuInitPartyPanelSlots(s32);

extern void mnuAppendCampSpriteRequests(s32, s32);

u8 *mnuCreateWorkBlock(void) {
    s32 handle = func_002D03F8(0x82C);
    u8 *work = (u8 *)sdfResourceRetainAddress(handle);

    memset(work, 0, 0x82C);
    *(s32 *)work = handle;
    ((MenuProgressWork *)work)->groupResource = mnuAllocateValueRecord(1);
    mnuInitPartyPanelSlots((s32)(work + 0x84));
    mnuAppendCampSpriteRequests(((MenuProgressWork *)work)->groupResource, (s32)(work + 8));
    ((MenuProgressWork *)work)->initState = 1;
    return work;
}

void mnuReleaseStaffMenuContextAndResources(u32 *arg0) {
    mnuShutdownContext(arg0 + 100);
    mnuReleaseStaffMenuTextureHandles(arg0 + 2);
    mnuReleaseStaffResourceGroups(arg0 + 2);
    func_002BC618(arg0[1]);
    func_002D0918(*arg0);
}

extern s32 mnuStaffSlotsAllFilled(s32, s32 *);

extern void mnuReleaseStaffMenuResources(s32 *);

extern void mnuInitializeStaffPageWindows(s32, s32 *, s32, s32);

s32 mnuTickInitState(u8 *work) {
    s32 state = ((MenuProgressWork *)work)->initState;
    s32 *group;

    if (state == 0) {
        return 1;
    }
    if (state == 2) {
        return 0;
    }
    group = (s32 *)(work + 8);
    if (mnuStaffSlotsAllFilled(((MenuProgressWork *)work)->groupResource, group) == 0) {
        return 1;
    }
    mnuReleaseStaffMenuResources(group);
    mnuInitializeStaffPageWindows((s32)(work + 0x190), group, 0, (s32)(work + 0x84));
    ((MenuProgressWork *)work)->initState = 2;
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00248580", func_00249850);

void mnuReleaseMenuVisualWorkResources(s32 arg0) {
    mnuClearEntries(arg0 + 400);
    mnuReleasePartyIconBundles(arg0 + 400);
    mnuDestroyPanelGroup(((MenuVisualWork *)arg0)->panelGroup);
    func_00283820(((MenuVisualWork *)arg0)->displayResource);
    func_00285160(((MenuVisualWork *)arg0)->effectResource);
}

void effUpdateAttached(s32 arg0, s32 arg1, s32 arg2, MenuVisualWork *work) {
    mnuDrawAndAdvanceProfilePanel(arg0, arg1, arg2, work->effectResource);
}

INCLUDE_ASM(const s32, "game/code_00248580", func_00249998);

INCLUDE_ASM(const s32, "game/code_00248580", func_00249A60);

void mnuSetWorldObjectAndMenuEnabled(s8 enabled) {
    s64 worldObject;

    if (enabled == '\x01') {
        worldObject = dds3GetWorldObject();
        if (worldObject != 0) {
            dds3SetWorldObjectDataValue(worldObject, 1);
        }
        D_003BC3E1 = 1;
    }
    else {
        worldObject = dds3GetWorldObject();
        if (worldObject != 0) {
            dds3SetWorldObjectDataValue(worldObject, 0);
        }
        D_003BC3E1 = 0;
    }
}

void mnuTerminalCreateEffects(MenuSlotState *state) {
    EffectObject *obj;

    obj = effCreateStatusBatch(1);
    state->effect[0] = (s32)obj;
    obj->inner->pair->a = 0x14;
    obj->inner->pair->b = 1;
    obj = effCreateStatusBatch(1);
    state->effect[1] = (s32)obj;
    obj->inner->pair->a = 0xF;
    obj->inner->pair->b = 0;
    obj = effCreateStatusBatch(8);
    state->effect[2] = (s32)obj;
    obj->inner->pair->a = 6;
    obj->inner->pair->b = 1;
    obj = effCreateStatusBatch(8);
    state->effect[3] = (s32)obj;
    obj->inner->pair->a = 6;
    obj->inner->pair->b = 0;
    obj = effCreateStatusBatch(1);
    state->effect[4] = (s32)obj;
    obj->inner->pair->a = 6;
    obj->inner->pair->b = 1;
    obj = effCreateStatusBatch(1);
    state->effect[5] = (s32)obj;
    obj->inner->pair->a = 6;
    obj->inner->pair->b = 0;
    obj = effCreateStatusBatch(1);
    state->effect[6] = (s32)obj;
    obj->inner->pair->a = 0x78;
    obj->inner->pair->b = 0;
}

void mnuDestroyAllMenuSlotEffectBatches(s32 object) {
    s32 *batch = ((MenuSlotState *)object)->effect;
    u32 i;

    for (i = 0; i < 7; i++) {
        effDestroyPackedBatch(batch[i]);
    }
}

INCLUDE_ASM(const s32, "game/code_00248580", func_00249DD0);

extern void mnuClearPanelTransitionState(void *);

extern void mnuLoadResourceHandles(s32);

extern void mnuTerminalBuildMenus(MenuProgressWork *host);

extern void evtLoadResourcePair(const char *, void *);

extern void evtCreateMessageWindowIfMissing(s32);

extern void func_00249DD0(s32);

INCLUDE_RODATA(const s32, "game/code_00248580", D_003AF5E0);

INCLUDE_RODATA(const s32, "game/code_00248580", D_003AF620);

u8 *mnuTerminalCreateScene(s32 reduced, s32 slot) {
    s32 handle;
    u8 *obj;
    u32 i;

    handle = func_002D03F8(0x164);
    obj = (u8 *)sdfResourceRetainAddress(handle);
    memset(obj, 0, 0x164);
    ((MenuProgressWork *)obj)->allocation = handle;
    mnuClearPanelTransitionState(obj + 8);
    mnuLoadResourceHandles(obj);
    mnuTerminalCreateEffects((MenuSlotState *)obj);
    ((MenuProgressWork *)obj)->mode = reduced;
    ((MenuSlotState *)obj)->mode = reduced;
    ((MenuProgressWork *)obj)->initState = slot;
    ((MenuSlotState *)obj)->selectedSlot = slot;
    mnuTerminalBuildMenus((MenuProgressWork *)obj);
    evtLoadResourcePair("/facility/msg/terminal/mes_data.bmd", ((MenuProgressWork *)obj)->messageResources);
    evtCreateMessageWindowIfMissing(((MenuProgressWork *)obj)->messageResources[1]);
    for (i = 0; i < 2; i++) {
        ((MenuSlotState *)obj)->cursorPositions[i] = -1;
    }
    func_00249DD0(obj);
    return obj;
}

extern s32 kwlnTaskGetUserValue();
extern void mnuReleaseResourceHandles(u32 *work);
extern void mnuDrainPanelTransitions(u8 *state, s32 arg);
extern void dspCloseChannel(void);
extern void evtReleaseResourcePairHandle(u32 *record);
extern s32 mnuCheckResourceTask(void);
extern void mnuStopResourceTask(void);
extern void func_00126038(s32 a, s32 b);
extern void fldProcessDeferredSceneCommand(void);
extern void func_002D0918(s32 handle);
extern u8 D_003BC3E0;

/* Tear down the terminal menu task: release its resources and effect batches, then hand the saved mode/slot to the field scene. */
void mnuReleaseTerminalWorkAndResumeField(s32 arg) {
    MenuProgressWork *work = (MenuProgressWork *)kwlnTaskGetUserValue();

    if (work != NULL) {
        mnuReleaseWorkResources((u8 *)work);
        mnuReleaseResourceHandles((u32 *)work);
        mnuDestroyAllMenuSlotEffectBatches((s32)work);
        mnuDrainPanelTransitions((u8 *)work + 8, arg);
        dspCloseChannel();
        evtReleaseResourcePairHandle((u32 *)work->messageResources);
        func_002D0918(work->allocation);
        D_003BC3E0 = 2;
    }
    if (mnuCheckResourceTask() != 0) {
        mnuStopResourceTask();
    }
    func_00126038(work->mode, work->initState);
    fldProcessDeferredSceneCommand();
}

INCLUDE_SDATA(const s32, "game/code_00248580", D_003BC3E0);

INCLUDE_SDATA(const s32, "game/code_00248580", D_003BC3E1);

INCLUDE_SDATA(const s32, "game/code_00248580", D_003BC3E4);

INCLUDE_SDATA(const s32, "game/code_00248580", D_003BC3E8);

INCLUDE_SDATA(const s32, "game/code_00248580", D_003BC3F0);

INCLUDE_SDATA(const s32, "game/code_00248580", D_003BC3F8);

