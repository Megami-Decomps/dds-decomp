#include "mnu.h"


extern void func_00266C08();
extern void kwlnFadeOutStart(s32, s32, s32, s32);
extern s32 func_0026BC80();
extern void evtCreateEventScriptProcess(s32);
extern void evtClearActiveFlag(s32);
extern void evtSetBoundedDisplayValue(s32, s32);
extern s8 D_00437859;

typedef struct MenuSlotState {
    u8 pad00[0x64];
    s32 batch;     /* 0x64 */
    u8 pad68[0x1C];
    s32 reduced;    /* 0x84 */
    s32 slot;       /* 0x88 */
    u8 pad8C[0x1C];
    s32 effect[7]; /* 0xA8 */
    s32 cur;       /* 0xC4 */
    s32 prev;      /* 0xC8 */
    u8 padCC[0x14];
    s32 slotCopy;   /* 0xE0 */
    s32 mode;       /* 0xE4 */
    u8 padE8[0x6C];
    s32 fadeColor;  /* 0x154 */
} MenuSlotState;

extern void evtLoadResourcePair(const char *, u8 *);
extern void evtCreateMessageWindowIfMissing(s32);
extern void mnuSnapshotCampTextureHandles(u8 *);
extern void func_002673B8();
extern void mnuClearPanelTransitionState(u8 *);
extern void func_002C1B58(u8 *, s32);

extern s32 func_0035C860(char *, const char *, ...);
extern u32 uiBlendColors(u32, u32, s32);
extern s32 func_0019F5E8(s32, s32, s32, s32, s32, s32);
extern char mnuNumberSpriteFormat[];

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

extern s32 mdlFlagTest(u32);

extern s64 evtGetMessageWindowControlState(void);

extern s32 fldGetModeFrameRecordIndex(s32);

extern s32 kwlnFadeIsActive(void);

extern s32 func_002B86E8(u32);

extern void func_002686F0(s32);

extern s8 D_00437858;

extern s32 kwlnTaskGetUserValue();

extern s32 mnuDrawAndAdvanceProfilePanel(s32, s32, s32, s32);

extern void func_002C1B70(s32, s32);

extern void func_002C1B68(s32, s32);

extern s32 movAreTitleEffectsReady(s32, s32);

extern void mnuReleaseStaffMenuResources(s32);

extern s32 datGameState;

extern s32 func_003292A8(s32);

extern s32 sdfResourceRetainAddress(s32);

extern s32 mnuAllocateValueRecord(s32);

extern void mnuInitPartyPanelSlots(s32);

extern void mnuAppendCampSpriteRequests(s32, s32);

extern s32 mnuCreateProfilePanel(void);

extern void mnuSetGroupProperties(s32, s32, s32, s32, s32);

extern void mnuDrawListPanels(s32, s32, s32, s32, s32, s32);

extern void func_002C16F0(s32, s32, s32, s32, s32, s32, s32);

extern void mnuTerminalSetTrack(s8, s8);

extern s32 mnuWalkNodeList(s32, s32);

extern void kwlnTaskDestroyWithHierarchyByName(const char *, s32);

extern const char D_00424F00[];

extern const char D_00424F10[];

extern const char D_00424F20[];

extern s32 D_0043785C;

extern s32 mnuCreateListState();

extern s32 mnuListAppendNode(s32, s32);

extern u8 D_00437870[];

extern s32 func_00328D68(s32);

extern s32 mnuPercentOrHundred(u16, u16);

extern void mnuDrawPanelSequenceByRow(s32, s32, s32, s32, s32, s32);

extern s32 effDestroyPackedBatch(s32);
extern void func_002C3390(s32);

typedef struct MenuResourceGroup {
    u8 pad0[0x64];
    u32 primary;
    u32 secondary;
    u32 tertiary;
    u32 quaternary;
    u8 pad74[0x380];
    s32 reducedMode;
} MenuResourceGroup;

typedef struct MenuProgressNode {
    u8 pad00[0x48];
    u32 flags;
    u8 pad4C[0xC];
    struct MenuProgressNode *next;
    u8 pad5C[4];
    s32 entryId;
    u32 requiredAmount;
    u8 pad68[8];
    s32 childPanel;
} MenuProgressNode;

typedef struct MenuProgressList {
    u8 pad00[0x10];
    MenuProgressNode *head;
    u8 pad14[8];
    MenuProgressNode *selected;
    s32 busy;
    u8 pad24[8];
    s32 updateCallback;
    s32 callback;
    u8 pad34[8];
    s32 visible;
} MenuProgressList;

typedef struct MenuTitleResource {
    u8 pad00[6];
    u16 firstA;
    u16 firstB;
    u16 secondA;
    u16 secondB;
} MenuTitleResource;

typedef struct MenuProgressHost {
    s32 heapHandle;
    s32 titleEffectHandle;
    s32 resourceHandle;       /* 0x08: menu effect group's resource */
    u8 pad0C[0x60];
    s32 loadState;
    u8 pad70[8];
    s32 menuList;
    MenuProgressList *progressList;
    MenuProgressList *secondaryList;
    s32 state;
    s32 selectedSlot;
    u8 pad8C[0x60];
    s32 panelStyle;
    u8 padF0[0x8C];
    s32 drawFlags;            /* 0x17C: forwarded to menu draw helper */
    u8 pad180[0xA6A4];
    s32 effectResource;       /* 0xA824 */
    s32 currentEffect;        /* 0xA828 */
} MenuProgressHost;

extern void mnuSetPopupEntry(s32 *, void *);

extern u8 D_003CE944[];

extern void mnuDestroyListState(u32);

extern void mnuReleaseCampTextureHandlesAndClearOutput(u8 *);

extern s64 func_002C4038(s32, s32 *, u64, u64);

/* Work record whose packed effect batch is held at +0x3C. */
typedef struct MenuBatchContext {
    u8 pad00[0x3C];
    u32 batch;
} MenuBatchContext;

void func_002665B0(MenuBatchContext *context) {
    effDestroyPackedBatch(context->batch);
}

u8 func_002665C8() {
    s64 unlocked;

    unlocked = mdlFlagTest(0x31);
    return unlocked != 0;
}

INCLUDE_ASM(const s32, "game/code_002665B0", func_002665E8);

INCLUDE_ASM(const s32, "game/code_002665B0", func_00266808);

void mnuReleaseResourceGroup(s32 address) {
    MenuResourceGroup *group = (MenuResourceGroup *)address;
    if (group->reducedMode == 0) {
        effResolveAndReleaseResource(group->primary);
        effResolveAndReleaseResource(group->secondary);
        effResolveAndReleaseResource(group->tertiary);
        effResolveAndReleaseResource(group->quaternary);
        return;
    }
    effResolveAndReleaseResource(group->primary);
    effResolveAndReleaseResource(group->secondary);
}

void mnuReleaseMenuResourceGroup(s32 address, u32 value) {
    MenuResourceGroup *group = (MenuResourceGroup *)address;
    if (group->reducedMode == 0) {
        effReleaseSlotTextureReferencesAndResetWork(group->primary);
        effReleaseSlotTextureReferencesAndResetWork(group->secondary, value);
        effReleaseSlotTextureReferencesAndResetWork(group->tertiary, value);
        effReleaseSlotTextureReferencesAndResetWork(group->quaternary, value);
        return;
    }
    effReleaseSlotTextureReferencesAndResetWork(group->primary);
    effReleaseSlotTextureReferencesAndResetWork(group->secondary, value);
}

void mnuReleaseResourceGroupTextureHandles(u32 address) {
    mnuReleaseMenuResourceGroup(address, 0);
}

extern s32 func_0019F460(s32, s32, s32, s32, s32, s32);
extern void func_0019D550(s32, s32, s32);
extern void frFontQueueGlyphInSelectedSlot(s32);
extern u8 D_003A41A8[];
extern u8 D_003A47E8[];

/* gridX/gridY are the glyph helper's grid cell coordinates (see
   sdfCounterDrawGlyphAtGridCell); depth is its z argument. */
void mnuQueueFontGlyphFromSelectedAtlasSlot(s32 gridX, s32 gridY, s32 depth, s32 value, s8 slot, s8 alternate) {
    u8 *entry;
    s32 handle;

    if (alternate == 0) {
        entry = D_003A41A8 + slot * 32;
    } else {
        entry = D_003A47E8 + slot * 32;
    }
    handle = func_0019F460(gridX - 0x120, gridY, depth, value, (s32)entry, 0);
    func_0019D550(handle, 1, 0x52);
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
    return (s32)(h * 1.8f) + (s32)(w * (w / 40.0f + 5.0f)) + bonus;
}

/* Mark entries whose required amount exceeds the current profile amount. */
void mnuRefreshThresholdNodeFlags(MenuProgressList *list) {
    MenuProgressNode *node = list->head;
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

void mnuCreateNumberSprite(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, u32 color, s32 priority) {
    char buf[16];
    s32 handle;

    func_0035C860(buf, mnuNumberSpriteFormat, a4);
    handle = func_0019F5E8(a0, a1, a2, uiBlendColors(color, color & ~0xFF, a3), (s32)buf, 0);
    func_0019D550(handle, 1, priority);
    frFontQueueGlyphInSelectedSlot(handle);
}

INCLUDE_RODATA(const s32, "game/code_002665B0", D_00424E60);

INCLUDE_ASM(const s32, "game/code_002665B0", func_00266C08);

s32 mnuCreateDualPercentPanel(MenuTitleResource *resource, MenuProgressHost *host) {
    s32 panel = func_00328D68(0xa0);
    mnuDrawPanelSequenceByRow(panel, 0, 0, 0x1e,
        mnuPercentOrHundred(resource->firstA, resource->firstB),
        host->panelStyle);
    mnuDrawPanelSequenceByRow(panel + 0x50, 1, 0, 0x1e,
        mnuPercentOrHundred(resource->secondA, resource->secondB),
        host->panelStyle);
    return panel;
}

/* Release both texture sets and the backing allocation for the panel pair. */
void mnuReleaseDualPercentPanel(s32 panel) {
    if (panel != 0) {
        mnuReleaseSpriteTextures();
        mnuReleaseSpriteTextures(panel + 0x50);
        sdfReleaseChipBlock(panel);
        return;
    }
}

void mnuCreateThresholdNodePanels(MenuProgressHost *host) {
    MenuProgressNode *node = host->progressList->head;
    while (node != 0) {
        s32 id = node->entryId;
        node->childPanel =
            mnuCreateDualPercentPanel((MenuTitleResource *)(datGameState + id * 0x1c4 + 0xa60), host);
        node = node->next;
    }
}

void mnuDestroyThresholdNodePanels(MenuProgressHost *host) {
    MenuProgressNode *node;

    for (node = host->progressList->head; node != 0; node = node->next) {
        mnuReleaseDualPercentPanel(node->childPanel);
    }
}

typedef struct ThresholdEntry {
    s32 entryId;
    s32 requiredAmount;
} ThresholdEntry;

void mnuBuildTerminalNodeList(MenuProgressHost *host) {
    MenuProgressList *list;
    s32 i;

    list = (MenuProgressList *)mnuCreateListState(0, 5, 0x24);
    list->callback = (s32)host;
    *(s32 *)&host->progressList = (s32)list;
    list->updateCallback = (s32)func_00266C08;
    list->visible = 0;
    for (i = 0; i < 5; i++) {
        BoxRecord *box = (BoxRecord *)(datGameState + i * 0x1C4 + 0xA60);

        if ((u16)(box->status & 1)) {
            s32 score = mnuTerminalScoreBox(box);

            if (score != 0) {
                MenuProgressNode *node = (MenuProgressNode *)mnuListAppendNode((s32)host->progressList, (s32)D_00437870);
                ThresholdEntry *entry = (ThresholdEntry *)&node->entryId;

                node->childPanel = 0;
                entry->requiredAmount = score;
                entry->entryId = i;
            }
        }
    }
    mnuRefreshThresholdNodeFlags(host->progressList);
}

void mnuReleaseProgressWorkList(MenuProgressHost *host) {
    mnuDestroyListState((s32)host->progressList);
}

void mnuReleaseSelectedProgressPanel(MenuProgressHost *host) {
    mnuReleaseDualPercentPanel(host->progressList->selected->childPanel);
    func_002B86E8((u32)host->progressList);
}

INCLUDE_ASM(const s32, "game/code_002665B0", func_00267238);

typedef struct MenuSlotKind {
    s16 kind;
    s16 unk2;
} MenuSlotKind;

extern MenuSlotKind D_0038A3B8[];

/* Same slot kind, or both kinds in the 0xF/0x1D/0x1E group. */
s32 mnuSlotKindMatchesGroupOrSpecial(s32 index, s32 kind) {
    s16 current = D_0038A3B8[index].kind;

    if (kind == current) {
        return 1;
    }
    if (kind == 0xF || kind == 0x1D || kind == 0x1E) {
        if (current == 0xF) {
            return 1;
        }
        if (current == 0x1D) {
            return 1;
        }
        if (current == 0x1E) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002665B0", func_002673B8);

u32 func_002674F8(void) {
    return 0;
}

void mnuThresholdNodeDrawCallback(s32 x, s32 y, s32 unused, MenuProgressList *list, MenuProgressNode *node, s32 priority) {
    s32 width = list->visible;
    MenuSlotState *host = (MenuSlotState *)list->callback;
    s32 index = node->entryId;
    s32 isCurrent = node == list->selected;

    if (node->flags & 1) {
        width /= 2;
    }
    if (isCurrent) {
        func_00306CD0(x, y, 0, width, 0, host->batch, 0x16, priority);
        index += 1;
    }
    func_00306CD0(x + 0x50, y - 0x10, 0, width, 0, host->batch, index, priority);
}

/* Omit the selected entry when building the progress list. */
s32 mnuBuildThresholdNodeList(s32 *items, s32 count, s32 excluded, s32 callback) {
    MenuProgressList *list = (MenuProgressList *)mnuCreateListState(0, count, 0x16, callback);
    s32 i;
    list->callback = callback;
    list->updateCallback = (s32)mnuThresholdNodeDrawCallback;
    list->visible = 0;
    for (i = 0; i < count; i++) {
        if (i != excluded) {
            MenuProgressNode *node = (MenuProgressNode *)mnuListAppendNode((s32)list, (s32)D_00437870);
            node->entryId = items[i];
        }
    }
    return (s32)list;
}

void mnuHighlightProgressNodeFromOwnerSelection(MenuProgressHost *host) {
    s32 state = host->state;
    if (state < 2) {
        if (state < 0) {
            return;
        }
        if (host->secondaryList->busy == 0) {
            MenuProgressNode *selected = (MenuProgressNode *)mnuWalkNodeList(2 - func_002674F8(),
                                              host->menuList);
            selected->flags |= 1;
        }
    }
}

void mnuHighlightProgressNodeByMode(MenuProgressHost *host) {
    s32 state = host->state;
    s32 selectedIndex;
    if (state != 0) {
        if (state != 2) {
            return;
        }
        selectedIndex = 0;
    } else {
        selectedIndex = 3 - func_002674F8();
    }
    if (host->progressList->busy == 0) {
        MenuProgressNode *node = (MenuProgressNode *)mnuWalkNodeList(selectedIndex, host->menuList);
        node->flags |= 1;
    }
}

void mnuTerminalBuildMenus(MenuProgressHost *host) {
    s32 table[3][5] = {
        {0xB, 0xD, 0xF, 0x11, 0x13},
        {0xB, 0xD, 0x2E, 0x30, 0},
        {0x11, 0x30, 0, 0, 0},
    };
    s32 row;
    s32 count;
    s32 excluded = -1;

    switch (host->state) {
    case 0:
        row = 0;
        count = 5;
        if (func_002674F8() != 0) {
            excluded = 1;
        }
        break;
    case 1:
        row = 1;
        count = 4;
        if (func_002674F8() != 0) {
            excluded = 1;
        }
        break;
    default:
        row = 2;
        count = 2;
        break;
    }
    host->menuList = mnuBuildThresholdNodeList(table[row], count, excluded, (s32)host);
    mnuBuildTerminalNodeList(host);
    mnuSnapshotCampTextureHandles((u8 *)host + 0xE8);
    mnuCreateThresholdNodePanels(host);
    func_002673B8(host);
    mnuHighlightProgressNodeFromOwnerSelection(host);
    mnuHighlightProgressNodeByMode(host);
}

void mnuReleaseWorkResources(u8 *work) {
    MenuProgressHost *host = (MenuProgressHost *)work;
    u32 i;

    for (i = 0; i < 1; i++) {
        mnuDestroyListState(((s32 *)&host->menuList)[i]);
    }
    mnuDestroyThresholdNodePanels((s32)work);
    mnuReleaseCampTextureHandlesAndClearOutput(work + 0xE8);
    mnuReleaseProgressWorkList((s32)work);
    mnuDestroyListState((s32)host->secondaryList);
}

void mnuTerminalFadeOrClose(s32 flag, s32 scene) {
    if (flag == 0) {
        s32 state = ((MenuProgressHost *)scene)->state;

        if (state < 3) {
            if (state > 0) {
                kwlnFadeOutStart(0, 0, 0, 0xF);
            } else if (mdlFlagTest(0x429) != 0 || func_0026BC80() != 0 || func_002665C8(scene) != 0) {
                kwlnFadeOutStart(0, 0, 0, 0xF);
            } else {
                evtCreateEventScriptProcess(0x322);
            }
        } else if (mdlFlagTest(0x429) != 0 || func_0026BC80() != 0 || func_002665C8(scene) != 0) {
            kwlnFadeOutStart(0, 0, 0, 0xF);
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
    MenuProgressHost *host = (MenuProgressHost *)work;
    u8 *owner = (u8 *)host->secondaryList;
    host->state = 0;
    host->selectedSlot = *(s32 *)(*(u8 **)(owner + 0x1C) + 0x60);
}

s32 mnuCreateProgressHost(void) {
    s32 heap = func_003292A8(0xa82c);
    MenuProgressHost *host = (MenuProgressHost *)sdfResourceRetainAddress(heap);
    memset((void *)host, 0, 0xa82c);
    host->heapHandle = heap;
    host->titleEffectHandle = mnuAllocateValueRecord(1);
    mnuInitPartyPanelSlots((s32)host + 0x70);
    mnuAppendCampSpriteRequests(host->titleEffectHandle, (s32)host + 8);
    host->loadState = 1;
    return (s32)host;
}

void mnuReleaseStaffAndTitleVisualResources(u32 *hostWords) {
    mnuReleaseStaffMenuTextureHandles(hostWords + 2);
    mnuReleaseTitleEffectSprites(hostWords + 2);
    func_00303D58(hostWords[1]);
    func_003297C8(*hostWords);
}

s32 mnuPollTitleEffectsReady(MenuProgressHost *host) {
    s32 state = host->loadState;
    if (state == 0) {
        return 1;
    }
    if (state == 2) {
        return 0;
    }
    if (movAreTitleEffectsReady(host->titleEffectHandle, (s32)host + 8) == 0) {
        return 1;
    }
    mnuReleaseStaffMenuResources((s32)host + 8);
    host->loadState = 2;
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002665B0", func_00267B40);

INCLUDE_ASM(const s32, "game/code_002665B0", func_00267C48);

void mnuEnsureProfilePanelEffect(s32 unused, MenuProgressHost *host) {
    if (host->currentEffect == 0) {
        s32 effect = mnuCreateProfilePanel();
        host->currentEffect = effect;
        mnuSetGroupProperties(effect, host->resourceHandle, *(s32 *)((u8 *)host + 0x14), 1, 2);
    }
}

void mnuCloseCurrentProfilePanel(MenuProgressHost *host) {
    func_002C3390(host->currentEffect);
    host->currentEffect = 0;
}

s32 func_00267DE0(s32 x, s32 y, s32 mode, MenuProgressHost *host) {
    return mnuDrawAndAdvanceProfilePanel(x, y, mode, host->currentEffect);
}

s32 mnuDrawLoadedProgressPanels(s32 resource, MenuProgressHost *host, s32 mode) {
    if (host->loadState != 2) {
        return 0;
    }
    host->drawFlags |= 0x280;
    mnuDrawListPanels(0, 0, 0, *(u8 *)(resource + 0x55), (s32)&host->drawFlags, mode);
    func_002C16F0(0, 0, 0, resource, *(u8 *)(resource + 0x55),
                   host->effectResource, mode);
    return 1;
}

typedef struct MenuFadeWork {
    u8 pad00[0x84];
    s32 reduced;      /* 0x84 */
    u8 pad88[0xCC];
    s32 fadeColor;    /* 0x154 */
} MenuFadeWork;

extern void sndStartTrackExtended(s32);
extern void func_00342580(s32);

void mnuTerminalSetTrack(s8 mode, s8 enable) {
    s32 address = kwlnTaskGetUserValue(D_0043785C);
    MenuFadeWork *work = (MenuFadeWork *)address;

    if (mode == 1) {
        if (enable == 1) {
            /* Both arms are identical in retail; kept as written. */
            if (work->reduced == 0) {
                sndStartTrackExtended(work->fadeColor);
            } else {
                sndStartTrackExtended(work->fadeColor);
            }
        }
        D_00437859 = 1;
        mnuReleaseResourceGroup(address);
    } else {
        if (enable == 1) {
            func_00342580(work->fadeColor);
        }
        D_00437859 = 0;
        mnuReleaseMenuResourceGroup(address, 1);
    }
}

void func_00267F68(s8 index) {
    mnuTerminalSetTrack(index, 1);
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
    s32 *batch = (s32 *)(object + 0xA8);
    u32 i;

    for (i = 0; i < 7; i++) {
        effDestroyPackedBatch(batch[i]);
    }
}

extern s32 fldGetCurrentBgmHandle(void);
extern void sndEnsureMidiBankResident(u32);

/* Pick the scene's music bank (default bank when the scene is not reduced) and make it resident. */
void mnuSelectTerminalResourceBank(MenuSlotState *host) {
    if (host->reduced == 0) {
        host->fadeColor = 0x20000;
    } else {
        host->fadeColor = fldGetCurrentBgmHandle();
    }
    sndEnsureMidiBankResident(host->fadeColor & 0xFFFF0000);
}

extern void func_003425B0(void);
extern void func_003425D8(void);

void mnuApplyFadeTrackMode(s32 mode, MenuSlotState *host) {
    if (mode == 0) {
        if (host->reduced == 0) {
            sndStartTrackExtended(host->fadeColor);
        } else {
            func_003425B0();
        }
    } else if (host->reduced == 0) {
        func_00342580(host->fadeColor);
    } else {
        func_003425D8();
    }
}

u8 *mnuTerminalCreateScene(s32 reduced, s32 slot) {
    s32 handle;
    u8 *obj;
    u32 i;

    handle = func_003292A8(0x3F8);
    obj = (u8 *)sdfResourceRetainAddress(handle);
    memset(obj, 0, 0x3F8);
    *(s32 *)obj = handle;
    mnuClearPanelTransitionState(obj + 8);
    mnuTerminalCreateEffects((MenuSlotState *)obj);
    ((MenuSlotState *)obj)->reduced = reduced;
    ((MenuSlotState *)obj)->mode = reduced;
    ((MenuSlotState *)obj)->slot = slot;
    ((MenuSlotState *)obj)->slotCopy = slot;
    mnuTerminalBuildMenus((MenuProgressHost *)obj);
    evtLoadResourcePair("/facility/msg/terminal/mes_data.bmd", obj + 0x5C);
    evtCreateMessageWindowIfMissing(*(s32 *)(obj + 0x60));
    for (i = 0; i < 2; i++) {
        (&((MenuSlotState *)obj)->cur)[i] = -1;
    }
    *(s32 *)(obj + 0x150) = 0xF;
    mnuSelectTerminalResourceBank((MenuSlotState *)obj);
    mnuApplyFadeTrackMode(0, (MenuSlotState *)obj);
    func_002C1B58(obj + 0x3E8, 0x60);
    return obj;
}

extern void func_00266808(u32 *work);
extern void mnuDrainPanelTransitions(u8 *state, s32 arg);
extern void dspCloseChannel(void);
extern void evtReleaseResourcePairHandle(u32 *record);
extern void func_003297C8(s32 handle);
extern s32 mnuCheckResourceTask(void);
extern void mnuStopResourceTask(void);
extern void func_001285E8(s32 a, s32 b);
extern void fldProcessDeferredSceneCommand(void);

/* Tear down the terminal menu task: release its resources and effect batches, then hand the saved mode/slot to the field scene. */
void mnuReleaseTerminalWorkAndResumeField(s32 arg) {
    MenuProgressHost *work = (MenuProgressHost *)kwlnTaskGetUserValue();

    if (work != NULL) {
        mnuReleaseWorkResources((u8 *)work);
        func_00266808((u32 *)work);
        mnuDestroyAllMenuSlotEffectBatches((s32)work);
        mnuDrainPanelTransitions((u8 *)work + 8, arg);
        dspCloseChannel();
        evtReleaseResourcePairHandle((u32 *)((u8 *)work + 0x5C));
        func_003297C8(work->heapHandle);
        D_00437858 = 2;
    }
    if (mnuCheckResourceTask() != 0) {
        mnuStopResourceTask();
    }
    func_001285E8(work->state, work->selectedSlot);
    fldProcessDeferredSceneCommand();
}

s32 mnuUpdateTerminalMessageWindowIndicator(void) {
    s32 context = kwlnTaskGetUserValue() + 0x3e8;
    func_002C1B70(context, 0x53);
    if (evtGetMessageWindowControlState() != 0) {
        func_002C1B68(context, 1);
    } else {
        func_002C1B68(context, 0);
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_002665B0", D_00424F00);

INCLUDE_RODATA(const s32, "game/code_002665B0", D_00424F10);

INCLUDE_RODATA(const s32, "game/code_002665B0", D_00424F20);

INCLUDE_ASM(const s32, "game/code_002665B0", func_00268380);


void fldStopSceneTasks(void) {
    kwlnTaskDestroyWithHierarchyByName(D_00424F00, 0);
    kwlnTaskDestroyWithHierarchyByName(D_00424F10, 0);
    kwlnTaskDestroyWithHierarchyByName(D_00424F20, 0);
    D_0043785C = 0;
}

s32 fldPollSceneState(void) {
    s32 state = D_00437858;
    if (state == 1) {
        return 1;
    }
    if (state < 2) {
        return 0;
    }
    if (state == 2) {
        D_00437858 = 0;
    }
    return 0;
}

s64 func_002684F0(s32 callback) {
    s32 context = kwlnTaskGetUserValue();

    mnuSetPopupEntry((s32 *)(context + 0x54), D_003CE944);
    return menuSetHandler(context, 0, callback);
}

s64 func_00268550(s32 callback) {
    return menuSetHandler(kwlnTaskGetUserValue(), 1, callback);
}

s64 func_00268588(s32 callback) {
    return menuSetHandler(kwlnTaskGetUserValue(), 2, callback);
}

s32 evtIsFadeCompleteAndMessageWindowIdle(void) {
    s32 fadeActive = kwlnFadeIsActive();

    if (fadeActive != 0) {
        return 0;
    }
    return evtGetMessageWindowControlState() == 0;
}

INCLUDE_ASM(const s32, "game/code_002665B0", func_002685F0);

typedef struct {
    u8 pad00[6];
    u16 previousA; /* 0x06 */
    u16 currentA; /* 0x08 */
    u16 previousB; /* 0x0A */
    u16 currentB; /* 0x0C */
    u16 flags; /* 0x0E */
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

INCLUDE_ASM(const s32, "game/code_002665B0", func_002686F0);

INCLUDE_ASM(const s32, "game/code_002665B0", func_00268838);

/* The sequel stores this countdown eight bytes later than DDS1. */
typedef struct {
    u8 pad00[0xA4];
    s32 remainingFrames; /* 0xA4 */
} SceneTimerView;

s32 fldClassifyRemainingFrames(SceneTimerView *timer) {
    s32 frames = timer->remainingFrames;
    if (frames == 0) {
        return 0;
    }
    return frames >= 60 ? 2 : 1;
}

void mnuTerminalConfigureEffects(u32 mode, MenuSlotState *state) {
    s32 *slot = &state->cur;

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

void mnuTerminalSelectSlot(s32 ctx, s32 index, MenuSlotState *state) {
    s32 table[5] = {3, 1, 2, 0x2D, 0x52};

    if (state->mode == 1 && index == state->mode) {
        index = 3;
    }
    if (index >= 0) {
        state->prev = state->cur;
        state->cur = table[index];
    } else if (index == -2) {
        state->prev = -1;
    }
    mnuTerminalConfigureEffects(ctx, state);
}

INCLUDE_ASM(const s32, "game/code_002665B0", func_00268B48);

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
    SceneFrameTable *frameTable;
    u8 pad68[0x7C];
    s32 mode; /* 0xE4 */
} SceneFrameOwner;

/* Scene modes 1 and 2 select different entries from the same frame table. */
s32 fldGetModeFrameRecordIndex(s32 object) {
    switch (((SceneFrameOwner *)object)->mode) {
    case 1:
        return 0x32;
    case 2:
        return 0x36;
    default:
        return 0;
    }
}

u8 func_00268C08(SceneFrameOwner *scene) {
    s32 index;

    index = fldGetModeFrameRecordIndex((s32)scene);
    return scene->frameTable->records[index].unk14;
}

typedef struct MenuEffHost {
    u8 pad00[0x64];
    s32 batch;        /* 0x64 */
    u8 pad68[0x40];
    s32 effectA;      /* 0xA8 */
    u8 padAC[8];
    s32 effectB;      /* 0xB4 */
} MenuEffHost;

extern void effConfigureWithDefaultSetting(s32, s32, s32, s32, s32, s32);

void mnuConfigureSelectedSceneModeEffect(s32 mode, MenuEffHost *host) {
    switch (mode) {
    case 1:
        effConfigureWithDefaultSetting(host->batch, 6, host->effectA, 0, 0, 2);
        return;
    case 2:
        effConfigureWithDefaultSetting(host->batch, 6, host->effectB, 0, 0, 2);
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_002665B0", func_00268CC0);

INCLUDE_ASM(const s32, "game/code_002665B0", func_00268EC8);

INCLUDE_ASM(const s32, "game/code_002665B0", func_002690A8);

INCLUDE_ASM(const s32, "game/code_002665B0", func_00269230);

INCLUDE_RODATA(const s32, "game/code_002665B0", D_00424F58);

INCLUDE_RODATA(const s32, "game/code_002665B0", D_00424F88);

s32 func_00269418(s32 object) {
    switch (*(s32 *)(object + 0x20)) {
    case 2: return 0x3a;
    case 3: return 0x3b;
    case 4: return 0x3c;
    case 5: return 0x3d;
    case 6: return 0x3e;
    default: return 0x3f;
    }
}

INCLUDE_ASM(const s32, "game/code_002665B0", func_00269478);

INCLUDE_ASM(const s32, "game/code_002665B0", func_00269638);

typedef struct GridPanelHost {
    u8 pad00[0x64];
    s32 grid;           /* 0x64 */
    u8 pad68[0x40];
    s32 settings[1];    /* 0xA8 */
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


/* Transition host: two optional callbacks at +0xCC and the flag at +0xD4 that
   picks which value they are called with. */
typedef struct TransitionHost {
    u8 pad00[0xCC];
    void (*callbacks[2])(s32, struct TransitionHost *); /* 0xCC */
    u32 forceCallbackIndexOne; /* 0xD4 */
} TransitionHost;



extern void func_0026C900(void);


typedef struct EventMenuSelection {
    s32 kind;       /* 0x00 */
    u8 pad04[0x44];
    u32 flags;      /* 0x48 */
    u8 pad4C[0x14];
    s32 entryIndex; /* 0x60 */
} EventMenuSelection;

typedef struct EventMenuOwner {
    u8 pad00[0x1C];
    EventMenuSelection *selection; /* 0x1C */
    s32 state; /* 0x20 */
    u8 pad24[0x18];
    s32 scale; /* 0x3C: fade scale, 0x100 when fully shown */
} EventMenuOwner;

typedef struct EventDispatchState {
    u8 pad00[8];
    u8 dispatchWork[0x4C]; /* 0x08 */
    s32 dispatchStatus; /* 0x54 */
    u32 dispatchValue; /* 0x58 */
    u8 pad5C[0x1C];
    EventMenuOwner *visualState; /* 0x78 */
    EventMenuOwner *thresholdOwner; /* 0x7C */
    EventMenuOwner *menuOwner; /* 0x80 */
    s32 menuMode; /* 0x84 */
    u8 pad88[8];
    s32 displayMode; /* 0x90 */
    u8 pad94[0x0C];
    s32 fadeStarted; /* 0xA0 */
    u8 padA4[0x28];
    u32 callback; /* 0xCC */
    u32 previousCallback; /* 0xD0 */
    s32 exitState; /* 0xD4 */
    s32 menuActive;      /* 0xD8: cleared when the menu command chain ends */
    s32 selectionStep;  /* 0xDC: compared against 2 by evtExitSelectionMenuAndSendSoundCommand */
    u8 padE0[4];
    s32 savedMenuMode;   /* 0xE4: nonzero re-requests the bank resource on entry */
    u8 padE8[0x64];
    s32 stage;           /* 0x14C */
    u8 pad150[4];
    u32 menuResource;    /* 0x154: released by func_00342580 */
} EventDispatchState;

extern void func_00268CC0(s32, s32);






extern void func_0026A728(s32, s32);

extern void mnuSelectFirstListNode(s32);


typedef struct MenuEntry32 {
    u8 data[32];
} MenuEntry32;



extern void func_0026C918(s32, void *);

extern void dspSetActive(s32);

extern void dspStartEntry(s32);

extern void evtSetMessageWindowOptionWhenOpen(s32);

extern void evtCaptureMessageWindowSoundMode(s32);




extern void func_00269638(void);

extern void func_00269478(s32, s32);





extern void func_00269978(void);





extern u8 D_003CE97C[];

extern void mnuSetPopupEntry();

INCLUDE_ASM(const s32, "game/code_002665B0", func_00269978);

void evtRememberDispatchCallback(u32 callback, s32 address) {
    EventDispatchState *state = (EventDispatchState *)address;
    u32 previous;

    previous = state->callback;
    state->callback = callback;
    state->previousCallback = previous;
}

/* Dispatch registered transition callbacks, optionally forcing index one. */
void mnuDispatchTransitionHostCallbacks(s32 state) {
    TransitionHost *host = (TransitionHost *)state;
    u32 i;

    for (i = 0; i < 2; i++) {
        if (host->callbacks[i] != NULL) {
            host->callbacks[i](host->forceCallbackIndexOne ? 1 : (s32)i, host);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_002665B0", func_00269B80);

u32 func_00269C48(void) {
    return 1;
}

extern u32 mnuMapPadMaskToFlags(s32 mask);
extern s32 func_002685F0(s32 action, s32 context);
extern void kwlnFadeInStart(s8, s8, s8, s32);
extern void mnuSetPopupEntryFlagged(s32 *state, char *entry);
extern void mnuClearListFlagsOneAndTwo(u32 list);
extern void mnuRetreatListCursorDefault(u32 list);
extern void mnuAdvanceListCursorDefault(u32 list);
extern void mnuPlayInputSound(s32 mode, u32 buttons, u32 list);
extern s32 D_003CE7D0[];
extern char D_003CE848[];
extern char D_003CE880[];
extern char D_003CE8B8[];
extern char D_003CE8F0[];

/* Event panel input: confirm opens the popup for the selected entry's action, cancel opens the back popup, left/right step the list. */
s64 evtBHandleSelectionPanelInput(u64 input) {
    EventDispatchState *context = (EventDispatchState *)kwlnTaskGetUserValue();
    u32 buttons = mnuMapPadMaskToFlags(0x33);
    s32 *state = &context->dispatchStatus;
    s32 kind = context->visualState->selection->kind;
    s32 frames;
    s32 action;
    EventMenuSelection *node;
    s64 result;

    result = func_002C4038((s32)context + 8, state, 0, input);
    if (result != 0) {
        return result;
    }
    frames = fldClassifyRemainingFrames((SceneTimerView *)context);
    if (frames != 2) {
        return 0;
    }
    if (context->visualState->scale < 0x100) {
        return 0;
    }
    if (*state == 0) {
        if (buttons & 1) {
            node = context->visualState->selection;
            action = D_003CE7D0[func_002674F8() * 5 + context->menuMode * 10 + kind];
            if (!(node->flags & 1) || action == 3 || action == frames) {
                if (func_002685F0(action, (s32)context) == 0) {
                    switch (action) {
                    case 2:
                        if (context->menuOwner->state == 1) {
                            mnuSetPopupEntryFlagged(state, D_003CE8F0);
                        } else {
                            mnuSetPopupEntryFlagged(state, D_003CE880);
                        }
                        break;
                    case 1:
                        kwlnFadeInStart(0, 0, 0, 0xF);
                    default:
                        mnuSetPopupEntryFlagged(state, D_003CE848 + action * 28);
                        break;
                    }
                }
            } else {
                buttons = 0x8000;
            }
        }
        if (buttons & 2) {
            mnuSetPopupEntryFlagged(state, D_003CE8B8);
        }
        if (!(buttons & 0x300000)) {
            mnuClearListFlagsOneAndTwo((u32)context->visualState);
        }
        if (buttons & 0x10) {
            mnuRetreatListCursorDefault((u32)context->visualState);
        }
        if (buttons & 0x20) {
            mnuAdvanceListCursorDefault((u32)context->visualState);
        }
        mnuPlayInputSound(0, buttons, (u32)context->visualState);
    }
    return 0;
}

s64 evtDispatchSelectionAfterFieldFrameGate(s32 request) {
    s32 state = kwlnTaskGetUserValue();
    EventDispatchState *dispatchState = (EventDispatchState *)state;

    func_002686F0(state);
    func_00268838(0, state);
    if (fldClassifyRemainingFrames(state) != 2) {
        return 0;
    }
    mnuDispatchTransitionHostCallbacks(state);
    func_00268EC8(state);
    func_00268B48(state);
    return menuSetHandler((s32)dispatchState, 1, request);
}

s64 evtBSetupDispatchSync(s32 request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();

    func_0026C900();
    return menuSetHandler((s32)state, 2, request);
}


extern void evtFinishMessageWindowAndNotify(void);

s32 evtClearDispatchVisualFlag(void) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();
    evtRememberDispatchCallback(0, (s32)state);
    state->visualState->scale = 0;
    mnuTerminalSetTrack(0, 0);
    evtFinishMessageWindowAndNotify();
    return 1;
}

extern void func_002690A8(s32, s32);

extern void func_00269230(void);


extern void mnuReleaseResourceGroup(s32);

s32 evtBeginSelectionExitFade(void) {
    s32 state = kwlnTaskGetUserValue();
    func_002690A8(1, state);
    evtRememberDispatchCallback((u32)func_00269230, state);
    mnuTerminalSelectSlot(0, -2, state);
    mnuTerminalSetTrack(1, 0);
    mnuReleaseResourceGroup(state);
    kwlnFadeOutStart(0, 0, 0, 15);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002665B0", func_0026A048);

s64 evtBDispatchStart(s32 request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();

    return menuSetHandler((s32)state, 1, request);
}

s64 evtBSetupDispatchSyncB(s32 request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();

    func_0026C900();
    return menuSetHandler((s32)state, 2, request);
}

u32 evtInitializeSelectionListWhenReady(void) {
    EventDispatchState *context = (EventDispatchState *)kwlnTaskGetUserValue();

    if (context->menuActive == 0) {
        mnuSelectFirstListNode((s32)context->menuOwner);
        func_00268CC0(3, (s32)context);
        evtRememberDispatchCallback((s32)func_00269638, (s32)context);
        func_00269478(3, (s32)context);
        func_002690A8(4, (s32)context);
        mnuTerminalSelectSlot(3, 1, (s32)context);
    }
    context->menuActive = 0;
    evtClearActiveFlag(0);
    evtSetBoundedDisplayValue(0, 3);
    return 1;
}

u32 evtFinishPendingSelectionTransition(void) {
    EventDispatchState *context = (EventDispatchState *)kwlnTaskGetUserValue();

    if (context->menuActive != 0) {
        func_00268CC0(3, (s32)context);
        evtRememberDispatchCallback((s32)func_00269230, (s32)context);
        func_00269478(4, (s32)context);
        func_002690A8(3, (s32)context);
        mnuTerminalSelectSlot(3, 0, (s32)context);
        evtFinishMessageWindowAndNotify();
    }
    context->menuActive = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002665B0", func_0026A2E0);

s64 func_0026A3F8(s32 request) {
    s32 state = kwlnTaskGetUserValue();
    EventDispatchState *dispatchState = (EventDispatchState *)state;
    func_002686F0(state);
    func_00268838(0, state);
    mnuDispatchTransitionHostCallbacks(state);
    func_00268EC8(state);
    func_00268B48(state);
    return menuSetHandler((s32)dispatchState, 1, request);
}

s64 evtBSetupDispatchSyncC(s32 request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();

    func_0026C900();
    return menuSetHandler((s32)state, 2, request);
}

u32 evtEnterThresholdSelectionList(void) {
    EventDispatchState *context = (EventDispatchState *)kwlnTaskGetUserValue();

    mnuRefreshThresholdNodeFlags((s32)context->thresholdOwner);
    mnuSelectFirstListNode((s32)context->thresholdOwner);
    mnuTerminalSelectSlot(3, 2, (s32)context);
    mnuApplyGridPanelHostSetting(3, (s32)context);
    func_002690A8(4, (s32)context);
    evtRememberDispatchCallback((s32)func_00269978, (s32)context);
    return 1;
}

u32 evtBEnterStateA(void) {
    s32 context = kwlnTaskGetUserValue();

    mnuTerminalSelectSlot(3, 0, context);
    mnuApplyGridPanelHostSetting(4, context);
    func_002690A8(3, context);
    evtRememberDispatchCallback((s32)func_00269230, context);
    mnuHighlightProgressNodeByMode(context);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002665B0", func_0026A598);

INCLUDE_ASM(const s32, "game/code_002665B0", func_0026A728);

s64 mnuInitializeSelectionDispatchWhenModeUnset(s32 request) {
    s32 state = kwlnTaskGetUserValue();
    EventDispatchState *dispatchState = (EventDispatchState *)state;
    func_002686F0(state);
    func_00268838(0, state);
    mnuDispatchTransitionHostCallbacks(state);
    func_00268EC8(state);
    func_00268B48(state);
    if (dispatchState->menuMode == 0) {
        func_0026A728(1, state);
    }
    return menuSetHandler((s32)dispatchState, 1, request);
}

s64 evtBSetupDispatchSyncD(s32 request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();

    func_0026C900();
    return menuSetHandler((s32)state, 2, request);
}

u32 evtSelectFinalVisualNode(void) {
    EventDispatchState *state;

    state = (EventDispatchState *)kwlnTaskGetUserValue();
    mnuSelectLastListNode((u32)state->visualState);
    return 1;
}

s64 func_0026A900(s32 request) {
    s32 state = kwlnTaskGetUserValue();
    s32 *panel = (s32 *)(state + 0x54);
    s64 result = func_002C4038(state + 8, panel, 0, request);
    if (result != 0) {
        return result;
    }
    if (*panel == 0) {
        if (evtGetMessageWindowControlState() == 0) {
            if (fldClassifyRemainingFrames(state) == 0) {
                mnuSetPopupEntry(panel, D_003CE97C);
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002665B0", func_0026A998);

s64 evtBSetupDispatchSyncE(s32 request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();

    func_0026C900();
    return menuSetHandler((s32)state, 2, request);
}




s32 evtBReleaseImagesAndQueueMenuTransition(void) {
    s32 state = kwlnTaskGetUserValue();
    mnuReleaseCampTextureHandlesAndClearOutput(state + 0xE8);
    mnuConfigureSelectedSceneModeEffect(2, state);
    mnuTerminalSelectSlot(3, 4, state);
    func_002690A8(2, state);
    evtRememberDispatchCallback(0, state);
    *(u32 *)(state + 0xA0) = 0;
    evtFinishMessageWindowAndNotify();
    dspCloseChannel();
    return 1;
}



s32 mnuOpenTerminalSelectionMessageWindow(void) {
    s32 state = kwlnTaskGetUserValue();
    mnuSnapshotCampTextureHandles(state + 0xE8);
    mnuConfigureSelectedSceneModeEffect(1, state);
    mnuTerminalSelectSlot(3, 0, state);
    func_002690A8(1, state);
    evtRememberDispatchCallback((u32)func_00269230, state);
    evtCreateMessageWindowIfMissing(*(s32 *)(state + 0x60));
    return 1;
}


extern void mnuCreateResourceTask(void);


extern s32 sdfCheckPendingWorkWithInterrupts(void);

extern void mnuSetPopupEntryFlagged(s32 *, char *);

extern char D_003CE848[];


/* Dispatch completion waits for the fade and pending resource/graph work;
 * keep the request outstanding until that barrier has drained. */
s64 evtPollDispatchAfterFade(u64 request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();
    s32 *dispatch = &state->dispatchStatus;
    s64 result = func_002C4038((s32)state->dispatchWork, dispatch, 0, request);
    if (result == 0) {
        if (kwlnFadeIsActive() == 0) {
            if (state->fadeStarted == 0) {
                state->fadeStarted = 1;
                mnuCreateResourceTask();
            }
            if (*dispatch == 0 && state->fadeStarted == 1 &&
                mnuCheckResourceTask() == 0) {
                if (sdfCheckPendingWorkWithInterrupts() != 0) return 0;
                mnuSelectTerminalResourceBank((MenuSlotState *)state);
                state->fadeStarted = 0;
                mnuSetPopupEntryFlagged(dispatch, D_003CE848);
            }
        }
        result = 0;
    }
    return result;
}

extern void func_00268838(s32, s32);

extern void mnuDispatchTransitionHostCallbacks(s32);

extern void func_00268EC8(s32);

extern void func_00268B48(s32);

s64 func_0026AC90(s32 request) {
    s32 state = kwlnTaskGetUserValue();
    EventDispatchState *dispatchState = (EventDispatchState *)state;
    func_002686F0(state);
    func_00268838(0, state);
    mnuDispatchTransitionHostCallbacks(state);
    func_00268EC8(state);
    func_00268B48(state);
    return menuSetHandler((s32)dispatchState, 1, request);
}

s64 evtBDispatchSync(s32 request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();

    return menuSetHandler((s32)state, 2, request);
}

u32 evtPrepareSelectedMenuEntry(void) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();
    EventMenuOwner *owner = state->menuOwner;
    s32 *slot = &owner->selection->entryIndex;

    if (owner->state == 1) {
        mnuSelectFirstListNode((s32)owner);
    }
    func_0026C918(0, D_003A41A8 + *slot * 32);
    dspSetActive(1);
    dspStartEntry(0);
    evtSetMessageWindowOptionWhenOpen(1);
    evtCaptureMessageWindowSoundMode(6);
    return 1;
}

u32 func_0026ADC0(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002665B0", func_0026ADC8);

s64 func_0026AEB0(s32 request) {
    s32 state = kwlnTaskGetUserValue();
    EventDispatchState *dispatchState = (EventDispatchState *)state;
    func_002686F0(state);
    func_00268838(0, state);
    mnuDispatchTransitionHostCallbacks(state);
    func_00268EC8(state);
    func_00268B48(state);
    return menuSetHandler((s32)dispatchState, 1, request);
}

s64 evtBSetupDispatchSyncF(s32 request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();

    func_0026C900();
    return menuSetHandler((s32)state, 2, request);
}

u32 evtBCheckPanelMode(void) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();

    dspSetActive(1);
    switch (state->displayMode) {
    case 1:
        dspStartEntry(1);
        break;
    case 2:
        dspStartEntry(2);
        break;
    }
    return 1;
}

s64 evtBContinueDispatchOrRestoreTable(u64 request) {
    EventDispatchState *state;
    s64 result;
    s32 *dispatch;

    state = (EventDispatchState *)kwlnTaskGetUserValue();
    dispatch = &state->dispatchStatus;
    result = func_002C4038((s32)state->dispatchWork, dispatch, 0, request);
    if (result == 0) {
        if ((*dispatch == 0) && (result = evtGetMessageWindowControlState(), result == 0)) {
            mnuSetPopupEntry(dispatch, state->dispatchValue);
        }
        result = 0;
    }
    return result;
}

s64 mnuPrepareDispatchStateAndBindHandler(s32 request) {
    s32 state = kwlnTaskGetUserValue();
    EventDispatchState *dispatchState = (EventDispatchState *)state;
    func_002686F0(state);
    func_00268838(0, state);
    mnuDispatchTransitionHostCallbacks(state);
    func_00268EC8(state);
    func_00268B48(state);
    return menuSetHandler((s32)dispatchState, 1, request);
}

s64 evtBSetupDispatchSyncG(s32 request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();

    func_0026C900();
    return menuSetHandler((s32)state, 2, request);
}

u32 evtExitSelectionMenuAndSendSoundCommand(void) {
    EventDispatchState *context = (EventDispatchState *)kwlnTaskGetUserValue();

    func_00268CC0(2, (s32)context);
    if (context->selectionStep >= 2) {
        mnuTerminalSelectSlot(2, -1, (s32)context);
        func_00269478(2, (s32)context);
    } else {
        mnuTerminalSelectSlot(2, -1, (s32)context);
        func_002690A8(2, (s32)context);
    }
    evtRememberDispatchCallback(0, (s32)context);
    context->exitState = 1;
    func_00342580(context->menuResource);
    evtClearActiveFlag(0);
    return 1;
}


s32 evtBRebuildTerminalMenuAndResetDispatch(void) {
    s32 state = kwlnTaskGetUserValue();
    mnuReleaseWorkResources(state);
    mnuTerminalBuildMenus(state);
    func_00268CC0(1, state);
    mnuTerminalSelectSlot(1, 0, state);
    func_002690A8(1, state);
    evtRememberDispatchCallback((u32)func_00269230, state);
    ((EventDispatchState *)state)->stage = 1;
    ((EventDispatchState *)state)->exitState = 0;
    kwlnFadeOutStart(0, 0, 0, 15);
    return 1;
}

/* Selection chain startup: fade in once the field frames are drained, then open the popup when the fade finishes. */
s64 evtBStartSelectionChainAfterFade(u64 request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();
    s32 *dispatch = &state->dispatchStatus;
    s64 result = func_002C4038((s32)state + 8, dispatch, 0, request);

    if (result != 0) {
        return result;
    }
    if (*dispatch == 0) {
        if (fldClassifyRemainingFrames((SceneTimerView *)state) != 0) {
            return 0;
        }
        switch (state->stage) {
        case 1:
            kwlnFadeInStart(0, 0, 0, 15);
            if (state->savedMenuMode != 0) {
                mnuSelectTerminalResourceBank((MenuSlotState *)state);
            }
            state->stage = 2;
            break;
        case 2:
            if (kwlnFadeIsActive() != 0) {
                return 0;
            }
            mnuSetPopupEntryFlagged(dispatch, D_003CE848);
            break;
        default:
            return 0;
        }
    }
    return 0;
}

s64 evtBDispatchSyncD2(s32 request) {
    s32 state = kwlnTaskGetUserValue();
    EventDispatchState *dispatchState = (EventDispatchState *)state;

    func_002686F0(state);
    if (func_00268C08(state) == 0) {
        func_00268838(1, state);
    } else {
        func_00268838(0, state);
    }
    mnuDispatchTransitionHostCallbacks(state);
    if (dispatchState->stage != 3) {
        func_00268EC8(state);
    }
    func_00268B48(state);
    return menuSetHandler((s32)dispatchState, 1, request);
}

s64 evtBDispatchSyncB(s32 request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();

    return menuSetHandler((s32)state, 2, request);
}

extern void mnuTerminalFadeOrClose(s32, s32);



extern void mnuStartMantraSpriteLoad(void);

s32 evtBCloseTerminalAndLoadMantraSprites(void) {
    s32 state = kwlnTaskGetUserValue();
    s32 mode;
    mnuTerminalFadeOrClose(0, state);
    evtClearActiveFlag(0);
    evtSetBoundedDisplayValue(0, 0);
    mode = ((EventDispatchState *)state)->menuMode;
    if (mode < 2) {
        if (mode >= 0) {
            mnuStartMantraSpriteLoad();
        }
    }
    return 1;
}

u32 func_0026B4A0(void) {
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_002665B0", D_00424FC8);

INCLUDE_RODATA(const s32, "game/code_002665B0", D_00424FF8);

INCLUDE_ASM(const s32, "game/code_002665B0", func_0026B4A8);

s64 evtBLateDispatchStart(s32 request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();

    func_002686F0((s32)state);
    return menuSetHandler((s32)state, 1, request);
}

s64 evtBDispatchSyncC(s32 request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();

    return menuSetHandler((s32)state, 2, request);
}
INCLUDE_SDATA(const s32, "game/code_002665B0", D_00437858);

INCLUDE_SDATA(const s32, "game/code_002665B0", D_00437859);

INCLUDE_SDATA(const s32, "game/code_002665B0", D_0043785C);

INCLUDE_SDATA(const s32, "game/code_002665B0", mnuNumberSpriteFormat);

INCLUDE_SDATA(const s32, "game/code_002665B0", D_00437868);

INCLUDE_SDATA(const s32, "game/code_002665B0", D_00437870);

INCLUDE_SDATA(const s32, "game/code_002665B0", D_00437878);

