#include "common.h"
#include "mnu.h"


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

extern s32 datGameState;

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

typedef struct MenuTerminalWork {
    s32 allocation;          /* 0x00 */
    s32 groupResource;       /* 0x04 */
    u8 pad08[0x54];
    s32 messageResources[2]; /* 0x5C: second handle opens the message window */
    s32 batch;               /* 0x64 */
    u8 pad68[8];
    s32 listResource;        /* 0x70 */
    MenuProgressOwner *list; /* 0x74 */
    MenuProgressOwner *owner;/* 0x78 */
    s32 mode;                /* 0x7C */
    s32 initState;           /* 0x80 */
    u8 pad84[0x1C];
    s32 effect[7];           /* 0xA0: effect batches; [4] and [5] are the pair selected via cursor */
    s32 cursor[2];           /* 0xBC: current and previous node, -1 until selected */
    u8 padC4[0x14];
    s32 selectedSlot;        /* 0xD8 */
    s32 reduced;             /* 0xDC */
    u8 padE0[4];
    s32 unkE4;               /* 0xE4 */
    u8 padE8[0x78];
    u32 resourceHandle;      /* 0x160: music bank handle */
} MenuTerminalWork; /* 0x164 allocation (mnuTerminalCreateScene) */

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

extern char mnuNumberSpriteFormat[];

extern void func_001958A0(s32, s32, s32);

extern void frFontQueueGlyphInSelectedSlot(s32);

typedef struct EffectPair {
    s32 a;
    s32 b;
} EffectPair;

extern EffectPair D_003BC400[];
extern void itfDrawGridWithResolvedSlot(s32, s32, s32, s32, s32, s32, s32);
extern void func_0024BF48(s32, s32);

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

extern s32 func_00197760(s32, s32, s32, s32, s32, s32);
extern u8 D_00347C68[];
extern u8 D_003482A8[];

/* gridX/gridY are the glyph helper's grid cell coordinates (see
   sdfCounterDrawGlyphAtGridCell); depth is its z argument. */
void mnuQueueFontGlyphFromAtlasSlot(s32 gridX, s32 gridY, s32 depth, s32 value, s8 slot, s8 alternate) {
    u8 *entry;
    s32 handle;

    if (alternate == 0) {
        entry = D_00347C68 + slot * 32;
    } else {
        entry = D_003482A8 + slot * 32;
    }
    handle = func_00197760(gridX, gridY, depth, value, (s32)entry, 0);
    func_001958A0(handle, 1, 0x53);
    frFontQueueGlyphInSelectedSlot(handle);
}

typedef struct BoxRecord {
    u16 status; /* 0x00: bit 0 set when the record is in use */
    u8 pad02[4];
    u16 y0; /* 0x06 */
    u16 y1; /* 0x08 */
    u16 x0; /* 0x0A */
    u16 x1; /* 0x0C */
    u16 flags; /* 0x0E */
} BoxRecord;

s32 mnuTerminalScoreBox(BoxRecord *box) {
    f32 w = box->x1 - box->x0;
    f32 h = box->y1 - box->y0;
    s32 bonus = 0;

    if (box->flags & 0x400) {
        bonus = 100;
    }
    if (box->flags & 0x100) {
        bonus += 50;
    }
    if (box->flags & 0x80) {
        bonus += 100;
    }
    if (box->flags & 0x40) {
        bonus += 100;
    }
    if (box->flags & 0x10) {
        bonus += 100;
    }
    return (s32)h + (s32)(w * (w / 200.0f + 3.0f)) + bonus;
}

/* Mark entries whose required amount exceeds the current profile amount. */
void mnuRefreshThresholdNodeFlags(MenuProgressOwner *owner) {
    MenuProgressNode *node = owner->firstProgressNode;
    if (node != 0) {
        s32 base = datGameState;
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

    func_003014F0(text, mnuNumberSpriteFormat, number);
    sprite = func_001978E8(x, y, layer, uiBlendColors(color, color & ~0xFF, fade), (s32)text, 0);
    func_001958A0(sprite, 1, priority);
    frFontQueueGlyphInSelectedSlot(sprite);
}

INCLUDE_RODATA(const s32, "game/code_00248580", D_003AF5A8);

INCLUDE_ASM(const s32, "game/code_00248580", func_00248810);





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

/* Release both texture sets and the backing allocation for the panel pair. */
void mnuReleaseDualPercentPanel(s32 panel) {
    if (panel != 0) {
        mnuReleaseSpriteTextures();
        mnuReleaseSpriteTextures((s32)panel + 0x54);
        sdfReleaseChipBlock(panel);
        return;
    }
}

/* Rebuild each node's panel from its corresponding party entry. */
void mnuUpdateGroupResources(u8 *scene) {
    MenuProgressNode *node = *(MenuProgressNode **)(*(u8 **)(scene + 0x74) + 0x10);

    while (node != NULL) {
        node->panel = mnuCreateDualPercentPanel(datGameState + node->itemIndex * 420 + 0xA60, (s32)scene);
        node = node->next;
    }
}

void mnuDestroyThresholdNodePanels(s32 owner) {
    s32 entry;

    for (entry = (s32)((MenuTerminalWork *)owner)->list->firstProgressNode; entry != 0; entry = (s32)((MenuProgressNode *)entry)->next) {
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

extern s32 mnuTerminalScoreBox(BoxRecord *box);

extern s32 func_00248810(s32);

void mnuBuildTerminalNodeList(MenuTerminalWork *host) {
    MenuProgressList *list;
    s32 i;

    list = (MenuProgressList *)mnuCreateListState(0, 5, 0x24);
    list->callback = (s32)host;
    *(s32 *)&host->list = (s32)list;
    list->updateCallback = (s32)func_00248810;
    list->visible = 0;
    for (i = 0; i < 5; i++) {
        s32 box = datGameState + i * 0x1A4 + 0xA60;

        if ((u16)(*(u16 *)box & 1)) {
            s32 score = mnuTerminalScoreBox(box);

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

void mnuReleaseProgressWorkList(MenuTerminalWork *work) {
    mnuDestroyListState((u32)work->list);
}

void mnuReleaseSelectedProgressPanel(MenuTerminalWork *work) {
    mnuReleaseDualPercentPanel(work->list->selectedNode->panel);
    func_0027B888((u32)work->list);
}

INCLUDE_ASM(const s32, "game/code_00248580", func_00248E68);

typedef struct MenuSlotKind {
    s16 kind;
    s16 unk2;
} MenuSlotKind;

extern MenuSlotKind D_0032EF18[];

/* Same slot kind, or both kinds in the 30/31 pair. */
s32 mnuSlotKindMatchesGroupOrSpecial(s32 index, s32 value) {
    s16 current = D_0032EF18[index].kind;

    if (value == current) {
        return 1;
    }
    if (value == 30 || value == 31) {
        if (current == 30) {
            return 1;
        }
        if (current == 31) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00248580", func_00249058);

u8 func_00249198(void) {
    s64 flagSet;

    flagSet = mdlFlagTest(0x902);
    return flagSet == 0;
}


INCLUDE_ASM(const s32, "game/code_00248580", func_002491B8);





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


void mnuHighlightProgressNodeFromOwnerSelection(s32 object) {
    s32 state = ((MenuTerminalWork *)object)->mode;
    if (state < 2) {
        if (state < 0) {
            return;
        }
        if (((MenuTerminalWork *)object)->owner->selectionState == 0) {
            s32 selected = mnuWalkNodeList(2 - func_00249198(),
                                              ((MenuTerminalWork *)object)->listResource);
            ((MenuProgressNode *)selected)->flags |= 1;
        }
    }
}


void mnuHighlightProgressNodeByMode(s32 object) {
    s32 state = ((MenuTerminalWork *)object)->mode;
    s32 selectedIndex;
    if (state != 0) {
        if (state != 2) {
            return;
        }
        selectedIndex = 0;
    } else {
        selectedIndex = 3 - func_00249198();
    }
    if (((MenuTerminalWork *)object)->list->selectionState == 0) {
        s32 node = mnuWalkNodeList(selectedIndex, ((MenuTerminalWork *)object)->listResource);
        ((MenuProgressNode *)node)->flags |= 1;
    }
}

extern void mnuResolveStaffImageHandles(u8 *);

extern s32 mnuTerminalMenuTemplate[];

void mnuTerminalBuildMenus(MenuTerminalWork *host) {
    s32 table[15];
    s32 row;
    s32 count;
    s32 excluded = -1;

    memcpy(table, mnuTerminalMenuTemplate, 0x3C);


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
    mnuDestroyListState((u32)((MenuTerminalWork *)work)->owner);
}

extern void kwlnFadeOutStart(s32, s32, s32, s32);

extern void evtCreateEventScriptProcess(s32);

extern void evtClearActiveFlag(s32);

extern void evtSetBoundedDisplayValue(s32, s32);

void mnuFadeOrPlayCloseSfx(s32 skip, u8 *work) {
    if (skip == 0) {
        s32 mode = ((MenuTerminalWork *)work)->mode;

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
    u8 *owner = (u8 *)((MenuTerminalWork *)work)->owner;
    ((MenuTerminalWork *)work)->mode = 0;
    ((MenuTerminalWork *)work)->initState = *(s32 *)(*(u8 **)(owner + 0x1C) + 0x60);
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
    ((MenuTerminalWork *)work)->groupResource = mnuAllocateValueRecord(1);
    mnuInitPartyPanelSlots((s32)(work + 0x84));
    mnuAppendCampSpriteRequests(((MenuTerminalWork *)work)->groupResource, (s32)(work + 8));
    ((MenuTerminalWork *)work)->initState = 1;
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
    s32 state = ((MenuTerminalWork *)work)->initState;
    s32 *group;

    if (state == 0) {
        return 1;
    }
    if (state == 2) {
        return 0;
    }
    group = (s32 *)(work + 8);
    if (mnuStaffSlotsAllFilled(((MenuTerminalWork *)work)->groupResource, group) == 0) {
        return 1;
    }
    mnuReleaseStaffMenuResources(group);
    mnuInitializeStaffPageWindows((s32)(work + 0x190), group, 0, (s32)(work + 0x84));
    ((MenuTerminalWork *)work)->initState = 2;
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00248580", func_00249850);

void mnuReleaseMenuVisualWorkResources(MenuVisualWork *work) {
    mnuClearEntries((s32)work + 400);
    mnuReleasePartyIconBundles((s32)work + 400);
    mnuDestroyPanelGroup(work->panelGroup);
    func_00283820(work->displayResource);
    func_00285160(work->effectResource);
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

void mnuTerminalCreateEffects(MenuTerminalWork *state) {
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
    s32 *batch = ((MenuTerminalWork *)object)->effect;
    u32 i;

    for (i = 0; i < 7; i++) {
        effDestroyPackedBatch(batch[i]);
    }
}

extern s32 fldGetCurrentBgmHandle(void);
extern void sndEnsureMidiBankResident(u32);

/* Pick the scene's music bank (default bank when the mode is zero) and make it resident. */
void mnuSelectTerminalResourceBank(MenuTerminalWork *work) {
    if (work->mode == 0) {
        work->resourceHandle = 0x20001;
    } else {
        work->resourceHandle = fldGetCurrentBgmHandle();
    }
    sndEnsureMidiBankResident(work->resourceHandle & 0xFFFF0000);
}

extern void mnuClearPanelTransitionState(void *);

extern void mnuLoadResourceHandles(s32);

extern void mnuTerminalBuildMenus(MenuTerminalWork *host);

extern void evtLoadResourcePair(const char *, void *);

extern void evtCreateMessageWindowIfMissing(s32);

INCLUDE_RODATA(const s32, "game/code_00248580", mnuTerminalMenuTemplate);

INCLUDE_RODATA(const s32, "game/code_00248580", D_003AF620);

u8 *mnuTerminalCreateScene(reduced, slot)
    s32 reduced;
    s32 slot;
{
    s32 handle;
    u8 *obj;
    u32 i;

    handle = func_002D03F8(0x164);
    obj = (u8 *)sdfResourceRetainAddress(handle);
    memset(obj, 0, 0x164);
    ((MenuTerminalWork *)obj)->allocation = handle;
    mnuClearPanelTransitionState(obj + 8);
    mnuLoadResourceHandles(obj);
    mnuTerminalCreateEffects((MenuTerminalWork *)obj);
    ((MenuTerminalWork *)obj)->mode = reduced;
    ((MenuTerminalWork *)obj)->reduced = reduced;
    ((MenuTerminalWork *)obj)->initState = slot;
    ((MenuTerminalWork *)obj)->selectedSlot = slot;
    mnuTerminalBuildMenus((MenuTerminalWork *)obj);
    evtLoadResourcePair("/facility/msg/terminal/mes_data.bmd", ((MenuTerminalWork *)obj)->messageResources);
    evtCreateMessageWindowIfMissing(((MenuTerminalWork *)obj)->messageResources[1]);
    for (i = 0; i < 2; i++) {
        ((MenuTerminalWork *)obj)->cursor[i] = -1;
    }
    mnuSelectTerminalResourceBank((MenuTerminalWork *)obj);
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
extern s8 mnuTerminalTaskState;

/* Tear down the terminal menu task: release its resources and effect batches, then hand the saved mode/slot to the field scene. */
void mnuReleaseTerminalWorkAndResumeField(s32 arg) {
    MenuTerminalWork *work = (MenuTerminalWork *)kwlnTaskGetUserValue();

    if (work != NULL) {
        mnuReleaseWorkResources((u8 *)work);
        mnuReleaseResourceHandles((u32 *)work);
        mnuDestroyAllMenuSlotEffectBatches((s32)work);
        mnuDrainPanelTransitions((u8 *)work + 8, arg);
        dspCloseChannel();
        evtReleaseResourcePairHandle((u32 *)work->messageResources);
        func_002D0918(work->allocation);
        mnuTerminalTaskState = 2;
    }
    if (mnuCheckResourceTask() != 0) {
        mnuStopResourceTask();
    }
    func_00126038(work->mode, work->initState);
    fldProcessDeferredSceneCommand();
}


extern s32 kwlnFadeIsActive(void);


extern s64 evtGetMessageWindowControlState(void);

extern s64 func_00285670(s32, s32 *, u64, u64);


extern void kwlnTaskDestroyWithHierarchyByName(const char *, s32);

extern const char D_003AF658[];

extern const char D_003AF668[];

extern const char D_003AF678[];

extern s32 D_003BC3E4;

extern void effConfigureWithDefaultSetting(s32, s32, s32, s32, s32, s32);

extern s32 kwlnTaskCreate(const char *, s32, s32, s32, void (*)(s32), void (*)(s32), void *);
extern s64 mnuPreparePopupAndDispatchSelection(s32);
extern s64 func_0024A138(s32);
extern s64 func_0024A170(s32);
extern void mnuReleaseTerminalWorkAndResumeField(s32);

s32 mnuTerminalCreateTasks(void) {
    s32 result;
    void *work = mnuTerminalCreateScene();

    D_003BC3E4 = kwlnTaskCreate(D_003AF658, 0x404, 1, 1, mnuPreparePopupAndDispatchSelection, 0, work);
    kwlnTaskCreate(D_003AF668, 0x2B14, 1, 1, func_0024A138, 0, work);
    result = kwlnTaskCreate(D_003AF678, 0x5210, 1, 1, func_0024A170, mnuReleaseTerminalWorkAndResumeField, work);
    mnuTerminalTaskState = 1;
    return result;
}

void fldStopSceneTasks(void) {
    kwlnTaskDestroyWithHierarchyByName(D_003AF658, 0);
    kwlnTaskDestroyWithHierarchyByName(D_003AF668, 0);
    kwlnTaskDestroyWithHierarchyByName(D_003AF678, 0);
    D_003BC3E4 = 0;
}

s32 fldPollSceneState(void) {
    s32 state = mnuTerminalTaskState;
    if (state == 1) {
        return 1;
    }
    if (state < 2) {
        return 0;
    }
    if (state == 2) {
        mnuTerminalTaskState = 0;
    }
    return 0;
}

extern char D_0036ADF4[];

extern void mnuSetPopupEntry(s32 *, char *);

s64 mnuPreparePopupAndDispatchSelection(s32 value) {
    s32 context = kwlnTaskGetUserValue();
    s32 *state = (s32 *)(context + 0x54);

    mnuSetPopupEntry(state, D_0036ADF4);
    return menuRunPanel(context, 0, value);
}

s64 func_0024A138(s32 value) {
    s32 context = kwlnTaskGetUserValue();

    return menuRunPanel(context, 1, value);
}

s64 func_0024A170(s32 value) {
    s32 context = kwlnTaskGetUserValue();

    return menuRunPanel(context, 2, value);
}

s32 evtIsFadeCompleteAndMessageWindowIdle(void) {
    s32 fadeActive = kwlnFadeIsActive();

    if (fadeActive != 0) {
        return 0;
    }
    return evtGetMessageWindowControlState() == 0;
}

INCLUDE_ASM(const s32, "game/code_00248580", func_0024A1D8);

typedef struct {
    u8 pad00[6];
    u16 previousA; /* 0x06 */
    u16 currentA;  /* 0x08 */
    u16 previousB; /* 0x0A */
    u16 currentB;  /* 0x0C */
    u16 flags;     /* 0x0E */
} SceneOptionRecord;

void fldSaveSceneOptionsAndClearFlags(SceneOptionRecord *option) {
    u16 flags = option->flags;
    u16 currentA = option->currentA;
    u16 currentB = option->currentB;
    u16 retainedFlags = flags & 0xfa2f;

    option->previousA = currentA;
    option->previousB = currentB;
    option->flags = retainedFlags;
}

INCLUDE_ASM(const s32, "game/code_00248580", func_0024A2D8);

INCLUDE_ASM(const s32, "game/code_00248580", func_0024A340);

/* View of the frame countdown; the preceding scene state is not known here. */
typedef struct {
    u8 pad00[0x9C];
    s32 remainingFrames; /* 0x9C */
} SceneTimerView;

s32 fldClassifyRemainingFrames(SceneTimerView *timer) {
    s32 frames = timer->remainingFrames;
    if (frames == 0) {
        return 0;
    }
    return frames >= 60 ? 2 : 1;
}

void mnuTerminalConfigureEffects(u32 mode, MenuTerminalWork *state) {
    s32 *slot = &state->cursor[0];

    if (*slot < 0) {
        return;
    }
    switch (mode) {
    case 1:
        effConfigureWithDefaultSetting(state->batch, *slot, state->effect[4], 0, 5, 2);
        break;
    case 2:
        effConfigureWithDefaultSetting(state->batch, *slot, state->effect[5], 0, 0, 2);
        break;
    case 3:
        effConfigureWithDefaultSetting(state->batch, *slot, state->effect[4], 0, 0, 2);
        if (slot[1] >= 0) {
            effConfigureWithDefaultSetting(state->batch, slot[1], state->effect[5], 0, 0, 2);
        }
        break;
    }
}

INCLUDE_RODATA(const s32, "game/code_00248580", D_003AF658);

INCLUDE_RODATA(const s32, "game/code_00248580", D_003AF668);

INCLUDE_RODATA(const s32, "game/code_00248580", D_003AF678);

INCLUDE_RODATA(const s32, "game/code_00248580", D_003AF688);

void mnuSelectTerminalCursorSlot(u32 mode, s32 index, MenuTerminalWork *state) {
    s32 table[4] = {3, 1, 2, 0x2D};

    if (state->reduced == 1) {
        if (index == state->reduced) {
            index = 3;
        }
    }
    if (index >= 0) {
        state->cursor[1] = state->cursor[0];
        state->cursor[0] = table[index];
    } else if (index == -2) {
        state->cursor[1] = -1;
    }
    mnuTerminalConfigureEffects(mode, state);
}

void mnuDrawTerminalSelectedSlots(s32 context) {
    MenuTerminalWork *state = (MenuTerminalWork *)context;
    EffectPair position = D_003BC400[0];
    s32 *slot;
    u32 i;

    for (i = 0, slot = state->cursor; i < 2; i++, slot++) {
        if (*slot >= 0) {
            itfDrawGridWithResolvedSlot(position.a, position.b, 0, 0x81,
                                        state->batch, *slot, 0x53);
        }
    }
    if (state->mode == 2) {
        func_0024BF48(0, context);
    }
}

typedef struct {
    u8 pad00[0x14];
    u8 unk14;
    u8 pad15[0x8B];
} SceneFrameRecord;

typedef struct {
    u8 pad00[0x18];
    SceneFrameRecord *records;
} SceneFrameTable;

typedef struct {
    u8 pad00[0x64];
    SceneFrameTable *frameTable; /* 0x64 */
    u8 pad68[0x74];
    s32 mode; /* 0xDC */
} SceneFrameOwner;

extern s32 fldGetModeFrameRecordIndex(SceneFrameOwner *);

/* Scene modes 1 and 2 select different entries from the same frame table. */
s32 fldGetModeFrameRecordIndex(SceneFrameOwner *scene) {
    switch (scene->mode) {
    case 1:
        return 0x32;
    case 2:
        return 0x36;
    default:
        return 0;
    }
}

u8 func_0024A6E8(SceneFrameOwner *scene) {
    s32 index;

    index = fldGetModeFrameRecordIndex(scene);
    return scene->frameTable->records[index].unk14;
}


/* Transition host: two optional callbacks at +0xC4 and the flag at +0xCC that
   picks which value they are called with. */
typedef struct TransitionHost {
    u8 pad00[0xC4];
    void (*callbacks[2])(s32, struct TransitionHost *); /* 0xC4 */
    u32 forceCallbackIndexOne; /* 0xCC */
} TransitionHost;

typedef struct MenuFadeHost {
    u8 pad00[0x7C];
    s32 reduced;      /* 0x7C */
    u8 pad80[0xE0];
    s32 fadeColor;    /* 0x160 */
} MenuFadeHost;

extern void sndStartTrackExtended(s32);

extern void func_002E9708(void);

extern void func_002E96D8(s32);

extern void func_002E9730(void);


INCLUDE_ASM(const s32, "game/code_00248580", func_0024A728);

INCLUDE_ASM(const s32, "game/code_00248580", func_0024A930);

INCLUDE_ASM(const s32, "game/code_00248580", func_0024AB28);

INCLUDE_ASM(const s32, "game/code_00248580", func_0024AB70);


INCLUDE_ASM(const s32, "game/code_00248580", func_0024ACD8);

INCLUDE_ASM(const s32, "game/code_00248580", func_0024AE18);

INCLUDE_ASM(const s32, "game/code_00248580", func_0024AF58);

typedef struct GridPanelHost {
    u8 pad00[0x64];
    s32 grid;           /* 0x64 */
    u8 pad68[0x38];
    s32 settings[1];    /* 0xA0 */
} GridPanelHost;


extern void itfSetGridEntryQuantizedAndRefresh(s32, s32, s32, s32, s32, s32);

/* Reset grid entry 0x1A, then configure it from the panel's setting slot chosen by `kind`. */
void mnuApplyGridPanelHostSetting(u32 kind, GridPanelHost *host) {
    s32 flags = 0;
    s32 value = 0;
    s32 slot = 0;

    switch (kind) {
    case 2:
        flags = 2;
        value = 4;
        slot = 3;
        break;
    case 3:
        flags = 2;
        value = 7;
        slot = 2;
        break;
    case 4:
        value = 4;
        slot = 3;
        break;
    }
    itfSetGridEntryQuantizedAndRefresh(host->grid, 0x1A, 0, 0, 0, 0);
    effConfigureWithDefaultSetting(host->grid, 0x1A, host->settings[slot], 0, value, flags);
}

INCLUDE_ASM(const s32, "game/code_00248580", func_0024B168);

typedef struct {
    u8 pad00[0xC4];
    u32 callback;         /* 0xC4 */
    u32 previousCallback; /* 0xC8 */
} SceneTransition;

void evtRememberDispatchCallback(u32 callback, SceneTransition *transition) {
    u32 previous;

    previous = transition->callback;
    transition->callback = callback;
    transition->previousCallback = previous;
}

/* Dispatch registered transition callbacks, optionally forcing index one. */
void mnuDispatchTransitionHostCallbacks(TransitionHost *host) {
    u32 i;

    for (i = 0; i < 2; i++) {
        if (host->callbacks[i] != NULL) {
            host->callbacks[i](host->forceCallbackIndexOne ? 1 : (s32)i, host);
        }
    }
}

void mnuApplyFadeTrackMode(s32 mode, MenuFadeHost *host) {
    if (mode == 0) {
        if (host->reduced == 0) {
            sndStartTrackExtended(host->fadeColor);
        } else {
            func_002E9708();
        }
    } else if (host->reduced == 0) {
        func_002E96D8(host->fadeColor);
    } else {
        func_002E9730();
    }
}




extern void evtFinishMessageWindowAndNotify(void);

extern void func_0024A2D8(s32);

extern void func_0024DD78(void);




extern void func_0024A340(s32, s32);


extern void func_0024A930(s32);

extern void mnuDrawTerminalSelectedSlots(s32);

/* Offsets shared by the event-B menu/dispatch handlers in this unit. */
typedef struct EvtBContext {
    u8 pad00[0x54];
    s32 dispatchState; /* 0x54 */
    u32 dispatchTable; /* 0x58 */
    u8 pad5C[0x14];
    u32 visualList; /* 0x70 */
    s32 thresholdList; /* 0x74 */
    s32 selectionList; /* 0x78 */
    s32 state7C;       /* 0x7C: nonzero also re-requests the effect resource */
    u8  pad80[0x8];
    s32 panelMode; /* 0x88 */
    u8 pad8C[0x40]; /* 0x98: reset flag meaning still unclear */
    s32 exitPending; /* 0xCC */
    s32 transitionPending; /* 0xD0 */
    s32 transitionStage; /* 0xD4 */
    s32 menuActive;     /* 0xD8: cleared when the menu command chain ends */
    s32 selectionStep;  /* 0xDC: nonzero once the selection chain is running */
    u8 padE0[0x78];
    s32 effectHandle;   /* 0x158: effect resource handle */
    s32 dispatchMode; /* 0x15C */
    u32 resourceHandle; /* 0x160 */
} EvtBContext;

typedef struct EvtBSelectionNode {
    s32 kind;       /* 0x00 */
    u8 pad04[0x44];
    u32 flags;      /* 0x48 */
    u8 pad4C[0x14];
    s32 entryIndex; /* 0x60 */
} EvtBSelectionNode;

typedef struct EvtBSelectionList {
    u8 pad00[0x1C];
    EvtBSelectionNode *selected; /* 0x1C */
    s32 mode; /* 0x20 */
    u8 pad24[0x18];
    s32 scale; /* 0x3C: fade scale, 0x100 when fully shown */
} EvtBSelectionList;

INCLUDE_ASM(const s32, "game/code_00248580", func_0024B3A8);

u32 func_0024B470(void) {
    return 1;
}

extern u32 mnuMapPadMaskToFlags(s32 mask);
extern s32 func_0024A1D8(s32 action, s32 context);
extern void kwlnFadeInStart(s8, s8, s8, s32);
extern void mnuSetPopupEntryFlagged(s32 *state, void *entry);
extern void mnuClearListFlagsOneAndTwo(u32 list);
extern void mnuRetreatListCursorDefault(u32 list);
extern void mnuAdvanceListCursorDefault(u32 list);
extern void mnuPlayInputSound(s32 mode, u32 buttons, u32 list);
extern s32 D_0036AC80[];
extern u8 D_0036ACF8[];
extern u8 D_0036AD30[];
extern u8 D_0036AD68[];
extern u8 D_0036ADA0[];

/* Event-B panel input: confirm opens the popup for the selected entry's action, cancel opens the back popup, left/right step the list. */
s64 evtBHandleSelectionPanelInput(u64 input) {
    EvtBContext *context = (EvtBContext *)kwlnTaskGetUserValue();
    u32 buttons = mnuMapPadMaskToFlags(0x33);
    s32 *state = &context->dispatchState;
    s32 kind = ((EvtBSelectionList *)context->visualList)->selected->kind;
    s32 frames;
    s32 action;
    EvtBSelectionNode *node;
    s64 result;

    result = func_00285670((s32)context + 8, state, 0, input);
    if (result != 0) {
        return result;
    }
    frames = fldClassifyRemainingFrames((SceneTimerView *)context);
    if (frames != 2) {
        return 0;
    }
    if (((EvtBSelectionList *)context->visualList)->scale < 0x100) {
        return 0;
    }
    if (*state == 0) {
        if (buttons & 1) {
            node = ((EvtBSelectionList *)context->visualList)->selected;
            action = D_0036AC80[func_00249198() * 5 + context->state7C * 10 + kind];
            if (!(node->flags & 1) || action == 3 || action == frames) {
                if (func_0024A1D8(action, (s32)context) == 0) {
                    switch (action) {
                    case 2:
                        if (((EvtBSelectionList *)context->selectionList)->mode == 1) {
                            mnuSetPopupEntryFlagged(state, D_0036ADA0);
                        } else {
                            mnuSetPopupEntryFlagged(state, D_0036AD30);
                        }
                        break;
                    case 1:
                        kwlnFadeInStart(0, 0, 0, 0xF);
                    default:
                        mnuSetPopupEntryFlagged(state, D_0036ACF8 + action * 28);
                        break;
                    }
                }
            } else {
                buttons = 0x8000;
            }
        }
        if (buttons & 2) {
            mnuSetPopupEntryFlagged(state, D_0036AD68);
        }
        if (!(buttons & 0x300000)) {
            mnuClearListFlagsOneAndTwo(context->visualList);
        }
        if (buttons & 0x10) {
            mnuRetreatListCursorDefault(context->visualList);
        }
        if (buttons & 0x20) {
            mnuAdvanceListCursorDefault(context->visualList);
        }
        mnuPlayInputSound(0, buttons, context->visualList);
    }
    return 0;
}


s64 evtDispatchSelectionAfterFieldFrameGate(u64 request) {
    s32 state = kwlnTaskGetUserValue();

    func_0024A2D8(state);
    func_0024A340(0, state);
    if (fldClassifyRemainingFrames(state) != 2) {
        return 0;
    }
    mnuDispatchTransitionHostCallbacks(state);
    func_0024A930(state);
    mnuDrawTerminalSelectedSlots(state);
    return menuRunPanel(state, 1, request);
}

s64 evtBSetupDispatchSync(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    func_0024DD78();
    return menuRunPanel(context, 2, request);
}

s32 evtBClearAndReset(void) {
    s32 context = kwlnTaskGetUserValue();

    evtRememberDispatchCallback(0, context);
    ((EvtBSelectionList *)((EvtBContext *)context)->visualList)->scale = 0;
    mnuSetWorldObjectAndMenuEnabled(0);
    evtFinishMessageWindowAndNotify();
    return 1;
}

extern void func_0024AB70(s32, s32);
extern void func_0024ACD8(void);

u32 evtBeginSelectionExitFade(void) {
    s32 context = kwlnTaskGetUserValue();

    func_0024AB70(1, context);
    evtRememberDispatchCallback((s32)func_0024ACD8, context);
    mnuSelectTerminalCursorSlot(0, -2, context);
    mnuSetWorldObjectAndMenuEnabled(1);
    mnuReleaseVisualResources(context);
    kwlnFadeOutStart(0, 0, 0, 15);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00248580", func_0024B868);

s64 evtBDispatchStart(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    return menuRunPanel(context, 1, request);
}

s64 evtBSetupDispatchSyncB(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    func_0024DD78();
    return menuRunPanel(context, 2, request);
}

extern void mnuSelectFirstListNode(s32);
extern void func_0024A728(s32, s32);
extern void func_0024AF58(void);
extern void func_0024AE18(s32, s32);


u32 evtInitializeSelectionListWhenReady(void) {
    s32 context = kwlnTaskGetUserValue();

    if (((EvtBContext *)context)->transitionPending == 0) {
        mnuSelectFirstListNode(((EvtBContext *)context)->selectionList);
        func_0024A728(3, context);
        evtRememberDispatchCallback((s32)func_0024AF58, context);
        func_0024AE18(3, context);
        func_0024AB70(4, context);
        mnuSelectTerminalCursorSlot(3, 1, context);
    }
    ((EvtBContext *)context)->transitionPending = 0;
    evtClearActiveFlag(0);
    evtSetBoundedDisplayValue(0, 3);
    return 1;
}


u32 evtFinishPendingSelectionTransition(void) {
    s32 context = kwlnTaskGetUserValue();

    if (((EvtBContext *)context)->transitionPending != 0) {
        func_0024A728(3, context);
        evtRememberDispatchCallback((s32)func_0024ACD8, context);
        func_0024AE18(4, context);
        func_0024AB70(3, context);
        mnuSelectTerminalCursorSlot(3, 0, context);
        evtFinishMessageWindowAndNotify();
    }
    ((EvtBContext *)context)->transitionPending = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00248580", func_0024BB00);

s64 func_0024BC18(s32 item) {
    s32 state = kwlnTaskGetUserValue();
    func_0024A2D8(state);
    func_0024A340(0, state);
    mnuDispatchTransitionHostCallbacks(state);
    func_0024A930(state);
    mnuDrawTerminalSelectedSlots(state);
    return menuRunPanel(state, 1, item);
}

s64 evtBSetupDispatchSyncC(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    func_0024DD78();
    return menuRunPanel(context, 2, request);
}

extern void func_0024B168(void);

u32 evtEnterThresholdSelectionList(void) {
    s32 context = kwlnTaskGetUserValue();

    mnuRefreshThresholdNodeFlags(((EvtBContext *)context)->thresholdList);
    mnuSelectFirstListNode(((EvtBContext *)context)->thresholdList);
    mnuSelectTerminalCursorSlot(3, 2, context);
    mnuApplyGridPanelHostSetting(3, context);
    func_0024AB70(4, context);
    evtRememberDispatchCallback((s32)func_0024B168, context);
    return 1;
}

extern void mnuHighlightProgressNodeByMode(s32);

u32 evtBEnterStateA(void) {
    s32 context = kwlnTaskGetUserValue();

    mnuSelectTerminalCursorSlot(3, 0, context);
    mnuApplyGridPanelHostSetting(4, context);
    func_0024AB70(3, context);
    evtRememberDispatchCallback((s32)func_0024ACD8, context);
    mnuHighlightProgressNodeByMode(context);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00248580", func_0024BDB8);

INCLUDE_ASM(const s32, "game/code_00248580", func_0024BF48);

extern void func_0024BF48(s32, s32);

s64 mnuInitializeSelectionDispatchWhenModeUnset(s32 item) {
    s32 state = kwlnTaskGetUserValue();
    func_0024A2D8(state);
    func_0024A340(0, state);
    mnuDispatchTransitionHostCallbacks(state);
    func_0024A930(state);
    mnuDrawTerminalSelectedSlots(state);
    if (*(s32 *)(state + 0x7C) == 0) {
        func_0024BF48(1, state);
    }
    return menuRunPanel(state, 1, item);
}

s64 evtBSetupDispatchSyncD(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    func_0024DD78();
    return menuRunPanel(context, 2, request);
}

u32 evtSelectFinalVisualNode(void) {
    s32 context;

    context = kwlnTaskGetUserValue();
    mnuSelectLastListNode(((EvtBContext *)context)->visualList);
    return 1;
}

/* Terminal panel poll: once the message window is idle and the field frames are drained, open the follow-up popup. */
extern u8 D_0036AE2C[];
s64 evtOpenTerminalFollowupPopupWhenIdle(s32 request) {
    s32 state = kwlnTaskGetUserValue();
    s32 *panel = (s32 *)(state + 0x54);
    s64 result = func_00285670(state + 8, panel, 0, request);
    if (result != 0) {
        return result;
    }
    if (*panel == 0) {
        if (evtGetMessageWindowControlState() == 0) {
            if (fldClassifyRemainingFrames(state) == 0) {
                mnuSetPopupEntry((s32)panel, (s32)D_0036AE2C);
            }
        }
    }
    return 0;
}



INCLUDE_ASM(const s32, "game/code_00248580", func_0024C1B8);

s64 evtBSetupDispatchSyncE(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    func_0024DD78();
    return menuRunPanel(context, 2, request);
}


u32 evtBReleaseImagesAndQueueMenuTransition(void) {
    s32 context = kwlnTaskGetUserValue();

    mnuReleaseStaffImageHandles(context + 0xE0);
    func_0024A728(2, context);
    mnuSelectTerminalCursorSlot(2, -1, context);
    func_0024AB70(2, context);
    evtRememberDispatchCallback(0, context);
    ((EvtBContext *)context)->exitPending = 1;
    *(s32 *)(context + 0x98) = 0;
    evtFinishMessageWindowAndNotify();
    dspCloseChannel();
    func_002E96D8(((EvtBContext *)context)->resourceHandle);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00248580", mnuOpenTerminalSelectionMessageWindow);

INCLUDE_ASM(const s32, "game/code_00248580", func_0024C3F8);

/* Per-frame panel update: once the field frames are drained, pick the transition state from the selection chain, then run the panel. */
s64 evtBPollSelectionChainPanel(s32 item) {
    s32 state = kwlnTaskGetUserValue();

    func_0024A2D8(state);
    if (fldClassifyRemainingFrames((SceneTimerView *)state) != 0) {
        if (((EvtBContext *)state)->selectionStep == 0) {
            func_0024A340(1, state);
        } else if (func_0024A6E8((SceneFrameOwner *)state) == 0) {
            func_0024A340(1, state);
        } else {
            func_0024A340(0, state);
        }
        mnuDispatchTransitionHostCallbacks((TransitionHost *)state);
        func_0024A930(state);
        mnuDrawTerminalSelectedSlots(state);
    }
    return menuRunPanel(state, 1, item);
}

s64 evtBDispatchSync(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    return menuRunPanel(context, 2, request);
}

typedef struct MenuEntry32 {
    u8 data[32];
} MenuEntry32;

extern void func_0024DD90(s32, void *);
extern void dspSetActive(s32);
extern void dspStartEntry(s32);
extern void evtSetMessageWindowOptionWhenOpen(s32);
extern void evtCaptureMessageWindowSoundMode(s32);

u32 evtPrepareSelectedMenuEntry(void) {
    s32 state = kwlnTaskGetUserValue();
    s32 owner = ((EvtBContext *)state)->selectionList;
    s32 *selectionIndex = &((EvtBSelectionList *)owner)->selected->entryIndex;

    if (((EvtBSelectionList *)owner)->mode == 1) {
        mnuSelectFirstListNode(owner);
    }
    func_0024DD90(0, D_00347C68 + *selectionIndex * 32);
    dspSetActive(1);
    dspStartEntry(0);
    evtSetMessageWindowOptionWhenOpen(1);
    evtCaptureMessageWindowSoundMode(6);
    return 1;
}

u32 func_0024C6F8(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00248580", func_0024C700);

s64 func_0024C7E8(s32 item) {
    s32 state = kwlnTaskGetUserValue();
    func_0024A2D8(state);
    func_0024A340(0, state);
    mnuDispatchTransitionHostCallbacks(state);
    func_0024A930(state);
    mnuDrawTerminalSelectedSlots(state);
    return menuRunPanel(state, 1, item);
}

s64 evtBSetupDispatchSyncF(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    func_0024DD78();
    return menuRunPanel(context, 2, request);
}


u32 evtBCheckPanelMode(void) {
    s32 context = kwlnTaskGetUserValue();

    dspSetActive(1);
    switch (((EvtBContext *)context)->panelMode) {
    case 1:
        dspStartEntry(1);
        break;
    case 2:
        dspStartEntry(2);
        break;
    }
    return 1;
}

s64 evtBContinueDispatchOrRestoreTable(u64 input) {
    s32 context;
    s64 dispatchResult;
    s32 *dispatchState;

    context = kwlnTaskGetUserValue();
    dispatchState = &((EvtBContext *)context)->dispatchState;
    dispatchResult = func_00285670(context + 8, dispatchState, 0, input);
    if (dispatchResult == 0) {
        if ((*dispatchState == 0) && (dispatchResult = evtGetMessageWindowControlState(), dispatchResult == 0)) {
            mnuSetPopupEntry(dispatchState, ((EvtBContext *)context)->dispatchTable);
        }
        dispatchResult = 0;
    }
    return dispatchResult;
}

s64 mnuPrepareDispatchStateAndBindHandler(s32 item) {
    s32 state = kwlnTaskGetUserValue();
    func_0024A2D8(state);
    func_0024A340(0, state);
    mnuDispatchTransitionHostCallbacks(state);
    func_0024A930(state);
    mnuDrawTerminalSelectedSlots(state);
    return menuRunPanel(state, 1, item);
}

s64 evtBSetupDispatchSyncG(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    func_0024DD78();
    return menuRunPanel(context, 2, request);
}


u32 evtExitSelectionMenuAndSendSoundCommand(void) {
    s32 context = kwlnTaskGetUserValue();

    func_0024A728(2, context);
    if (((EvtBContext *)context)->transitionStage >= 2) {
        mnuSelectTerminalCursorSlot(2, -1, context);
        func_0024AE18(2, context);
    } else {
        mnuSelectTerminalCursorSlot(2, -1, context);
        func_0024AB70(2, context);
    }
    evtRememberDispatchCallback(0, context);
    ((EvtBContext *)context)->exitPending = 1;
    func_002E96D8(((EvtBContext *)context)->resourceHandle);
    evtClearActiveFlag(0);
    return 1;
}


u32 evtBRebuildTerminalMenuAndResetDispatch(void) {
    s32 context = kwlnTaskGetUserValue();

    mnuReleaseWorkResources(context);
    mnuTerminalBuildMenus(context);
    func_0024A728(1, context);
    mnuSelectTerminalCursorSlot(1, 0, context);
    func_0024AB70(1, context);
    evtRememberDispatchCallback((s32)func_0024ACD8, context);
    ((EvtBContext *)context)->dispatchMode = 0;
    ((EvtBContext *)context)->exitPending = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00248580", func_0024CB80);


s64 evtBDispatchSyncD2(s32 item) {
    s32 state = kwlnTaskGetUserValue();

    func_0024A2D8(state);
    if (func_0024A6E8(state) == 0) {
        func_0024A340(1, state);
    } else {
        func_0024A340(0, state);
    }
    mnuDispatchTransitionHostCallbacks(state);
    if (((EvtBContext *)state)->dispatchMode != 3) {
        func_0024A930(state);
    }
    mnuDrawTerminalSelectedSlots(state);
    return menuRunPanel(state, 1, item);
}

s64 evtBDispatchSyncB(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    return menuRunPanel(context, 2, request);
}

extern s32 mnuRequestEffectResource(char *, char *);
extern char D_003AF590[];
extern char D_003AF620[];

u32 evtBEndDispatchAndReloadEffectResource(void) {
    EvtBContext *context = (EvtBContext *)kwlnTaskGetUserValue();

    mnuFadeOrPlayCloseSfx(0, (s32)context);
    evtClearActiveFlag(0);
    evtSetBoundedDisplayValue(0, 0);
    if (context->state7C != 0) {
        context->effectHandle = mnuRequestEffectResource(D_003AF590, D_003AF620);
    }
    return 1;
}

u32 func_0024CE20(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00248580", func_0024CE28);

s64 evtBLateDispatchStart(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    func_0024A2D8(context);
    return menuRunPanel(context, 1, request);
}

s64 evtBDispatchSyncC(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    return menuRunPanel(context, 2, request);
}

INCLUDE_RODATA(const s32, "game/code_00248580", D_003AF6B0);

INCLUDE_RODATA(const s32, "game/code_00248580", D_003AF6E0);

INCLUDE_RODATA(const s32, "game/code_00248580", D_003AF6F0);

INCLUDE_RODATA(const s32, "game/code_00248580", D_003AF700);

INCLUDE_RODATA(const s32, "game/code_00248580", D_003AF710);

INCLUDE_SDATA(const s32, "game/code_00248580", mnuTerminalTaskState);

INCLUDE_SDATA(const s32, "game/code_00248580", D_003BC3E1);

INCLUDE_SDATA(const s32, "game/code_00248580", D_003BC3E4);

INCLUDE_SDATA(const s32, "game/code_00248580", mnuNumberSpriteFormat);

INCLUDE_SDATA(const s32, "game/code_00248580", D_003BC3F0);

INCLUDE_SDATA(const s32, "game/code_00248580", D_003BC3F8);

INCLUDE_SDATA(const s32, "game/code_00248580", D_003BC400);

