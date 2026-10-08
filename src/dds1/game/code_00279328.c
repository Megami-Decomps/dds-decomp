#include "mnu.h"

extern s32 kwlnTaskGetUserValue();

extern void mnuCampMenuHandleInput(s32);
extern void ptySkillMenuHandleSelection(s32);
extern void ptySkillMenuHandleSlotReorder(s32);
extern void func_00272778(s32);
extern void mnuCreateStaffImageSprite(s32);
extern void func_002723B0(s32, s32);
extern void ptySkillMenuCopyPageState(s32);
extern void mnuDrawStaffCampScreen(s32, s32);
extern void func_00272518(s32, s32, s32, s32, s32, s32, s32);
extern void func_00272668(s32, s32, s32, s32, s32, s32);
extern void mnuDrawWindowContainer(s32, s32, s32, s32, s32);
extern u32 mnuHasSelectedListNodeId(s32);
extern s32 D_003BAA98;
extern s32 datGameState;
extern s32 mnuIsEntryCostUnaffordable(u16, s32);
extern s32 ptySkillApplyFieldUseEffect(s32, s32, s32, s32);
extern void mnuConsumeEntryCost(s32, s32);
extern void mnuInitPartyPanelSlots(s32);
extern void mnuUpdateHandleStates(s32);
extern void func_00280048(s32);
extern u32 mnuMapPadMaskToFlags(u32);
extern s32 mnuGetAbilityTargetCategory(u16);
extern void mnuStepPartyPanelListFromInput();
extern void mnuSetPopupEntry(s32 *, char *);
extern char D_0037CC58[];
extern void mnuClearListFlags();
extern void mnuPlayInputSound(s32, s32, u32 *);

typedef struct SkillListNode {
    s32 index;                   /* 0x00 */
    u8 pad04[0x44];
    u32 flags;                   /* 0x48 */
    u8 pad4C[0xC];
    struct SkillListNode *next;  /* 0x58 */
    u8 pad5C[4];
    u32 sortKey;                 /* 0x60 */
} SkillListNode;

typedef struct SkillList {
    u32 flags;                   /* 0x00 */
    u8 pad04[0xC];
    SkillListNode *first;        /* 0x10 */
    u8 pad14[8];
    SkillListNode *cursor;       /* 0x1C */
} SkillList;

typedef struct SkillListWindow {
    u8 pad00[0x14];
    SkillList *list;             /* 0x14 */
} SkillListWindow;

/* Skill menu work: the party list, one window per page and the active one. */
typedef struct SkillMenuState {
    u8 pad00[0xC];
    SkillList *partyList;        /* 0x0C */
    SkillListWindow *window[3];  /* 0x10 */
    u8 pad1C[4];
    s32 panelState;              /* 0x20 */
    SkillListWindow *selected;   /* 0x24 */
    u8 pad28[8];
    u32 selectionFlags;          /* 0x30 */
} SkillMenuState;

typedef struct SkillMenuContext {
    u8 pad00[0x78];
    s32 actor;                   /* 0x78 */
    u8 pad7C[0x64];
    s32 pageGrid;                /* 0xE0 */
    u8 padE4[4];
    s32 pageLabels;              /* 0xE8 */
    u8 padEC[0x38];
    SkillListWindow *panel;      /* 0x124 */
    u8 pad128[0x34];
    u32 actionFlags;             /* 0x15C */
    u8 pad160[0x678];
    SkillList *selection;        /* 0x7D8 */
    SkillList *target;           /* 0x7DC */
    u8 pad7E0[0x12C];
    SkillMenuState *menu;        /* 0x90C */
} SkillMenuContext;

typedef struct SkillPageBlock {
    u32 word[14];
} SkillPageBlock;

typedef struct SkillPageWindow {
    u8 pad00[4];
    s32 field04;
    u8 pad08[0xC];
    SkillList *list;             /* 0x14 */
    u8 pad18[0x34];
    SkillPageBlock block;        /* 0x4C */
    u8 pad84[4];
    s32 field88;
} SkillPageWindow;

extern void itfDrawGridWithResolvedSlot(s32, s32, s32, s32, s32, s32, s32);
extern void mnuSetPanelState(s32, s32);
extern void func_00282DA0(s32, s32, s32, s32, s32);

s32 ptySkillMenuUpdate(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    SkillMenuState *menu = ((SkillMenuContext *)context)->menu;
    s32 state = menuRunPanel((void *)context, 0, (void *)callback);
    if (state != 0) {
        return state;
    }
    if (((SkillMenuContext *)context)->panel->list->cursor->index == 0) {
        mnuCampMenuHandleInput(callback);
    } else if (menu->selectionFlags == 0) {
        ptySkillMenuHandleSelection(callback);
    } else {
        ptySkillMenuHandleSlotReorder(callback);
    }
    return 0;
}

static inline SkillPageWindow **getSkillPageSlot(SkillPageWindow **windows,
                                                  s32 index) {
    return windows + index;
}

void ptySkillMenuCopyPageState(s32 context) {
    SkillMenuContext *work = (SkillMenuContext *)context;
    SkillMenuState *menu = work->menu;
    SkillPageWindow **windows = (SkillPageWindow **)menu->window;
    s32 index = menu->partyList->cursor->index;
    s32 pageGrid;
    u32 i;

    itfDrawGridWithResolvedSlot(0x10E0, 0x598, 0, 1,
                                work->pageLabels,
                                9, 0x53);
    itfDrawGridWithResolvedSlot(0x10E0, 0xC88, 0, 1,
                                work->pageLabels,
                                0xA, 0x53);
    pageGrid = work->pageGrid;
    itfDrawGridWithResolvedSlot(0x10E0, 0xA18, 0, 1, pageGrid,
                                0x1D, 0x53);
    mnuSetPanelState(menu->panelState, index);
    func_00282DA0(0xDE0, 0x350, 0, menu->panelState, 0x53);
    mnuDrawWindowContainer(0x1220, 0x678, 0,
                           (s32)*getSkillPageSlot(windows, index), 0x53);
    for (i = 0; i < 4; i++) {
        if (i != index) {
            memcpy(&windows[i]->block,
                   &(*getSkillPageSlot(windows, index))->block,
                   sizeof(SkillPageBlock));
            windows[i]->field88 =
                (*getSkillPageSlot(windows, index))->field88;
            windows[i]->field04 =
                (*getSkillPageSlot(windows, index))->field04;
        }
    }
}

s32 ptySkillMenuEnterPage(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    SkillMenuContext *work = (SkillMenuContext *)context;
    SkillMenuState *menu = work->menu;
    s32 label;
    if (work->panel->list->cursor->index == 0) {
        mnuDrawWindowContainer(0x1C0, 0x3D0, 0, (s32)menu->selected, 0x53);
    } else {
        mnuDrawWindowContainer(0x1C0, 0x3D0, 0, (s32)menu->selected, 0x53);
        ptySkillMenuCopyPageState(context);
    }
    mnuDrawStaffCampScreen(1, callback);
    if (work->panel->list->cursor->index == 0) {
        mnuCreateStaffImageSprite(2);
    } else if (menu->selectionFlags != 0) {
        if (mnuHasSelectedListNodeId(callback) == 0) {
            mnuCreateStaffImageSprite(0xE);
        } else {
            mnuCreateStaffImageSprite(0xF);
        }
    } else if (((SkillListWindow *)*(s32 *)((s32)menu + 0x10 + (menu->partyList->cursor->index << 2)))->list->cursor->index == 0) {
        mnuCreateStaffImageSprite(0xD);
    } else {
        mnuCreateStaffImageSprite(0xC);
    }
    label = menu->selected->list->cursor->sortKey;
    if (label != 0xFFFF && label != 0) {
        func_00272518(1, label, D_003BAA98, context, 1, 1, 0x53);
    } else {
        func_00272668(1, 0, 0, context, 1, 0x53);
    }
    func_002723B0(0, work->actor);
    return menuRunPanel((void *)context, 1, (void *)callback);
}

s32 ptySkillMenuDispatchPageRequest(s32 selection) {
    s32 context = kwlnTaskGetUserValue();
    return menuRunPanel((void *)context, 2, (void *)selection);
}

s32 ptySkillMenuUseSelectedInField(id, context)
    u16 id;
    s32 context;
{
    s32 window = context + 0x15C;
    s32 slotA = datGameState + ((SkillMenuContext *)context)->selection->cursor->index * 0x1A4 + 0xA60;
    s32 slotB = datGameState + ((SkillMenuContext *)context)->target->cursor->index * 0x1A4 + 0xA60;
    if (mnuIsEntryCostUnaffordable(id, slotA) != 0) {
        return 0;
    }
    if (ptySkillApplyFieldUseEffect(window, id, slotA, slotB) != 0) {
        mnuConsumeEntryCost(id, slotA);
        mnuInitPartyPanelSlots(context + 0x7EC);
        mnuUpdateHandleStates(window);
        func_00280048(window);
        return 1;
    }
    return 0;
}

/* Same node as SkillListNode, read through its low halfword id. */
typedef struct SkillLink {
    u8 unk0[0x48];
    u32 flags;
    u8 unk4C[0xC];
    struct SkillLink *next;
    u8 unk5C[4];
    u16 id;
} SkillLink;

void mnuFlagMatchingEntries(s32 context) {
    s32 slot = datGameState + ((SkillMenuContext *)context)->selection->cursor->index * 0x1A4 + 0xA60;
    SkillLink *link = (SkillLink *)((SkillMenuContext *)context)->menu->selected->list->first;
    if (link != NULL) {
        do {
            if (mnuIsEntryCostUnaffordable(link->id, slot)) {
                link->flags |= 1;
            }
            link = link->next;
        } while (link != NULL);
    }
}

s32 ptySkillMenuHandleFieldUse(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    SkillMenuState *menu = ((SkillMenuContext *)context)->menu;
    s32 *popup = (s32 *)(context + 0x54);
    u32 buttons = mnuMapPadMaskToFlags(3);
    s32 state;
    s32 label;
    u16 code;
    s32 window;
    state = menuRunPanel((void *)context, 0, (void *)callback);
    if (state != 0) {
        return state;
    }
    label = menu->selected->list->cursor->sortKey;
    code = label;
    if (mnuGetAbilityTargetCategory(code) == 2) {
        ((SkillMenuContext *)context)->actionFlags |= 0x10;
    }
    if (mnuGetAbilityTargetCategory(code) == 3) {
        ((SkillMenuContext *)context)->actionFlags |= 0x20;
    }
    window = context + 0x15C;
    mnuStepPartyPanelListFromInput(8, window);
    if (buttons & 1) {
        buttons = ptySkillMenuUseSelectedInField(label, context) == 0 ? 0x8000 : 0;
        mnuFlagMatchingEntries(context);
    }
    if (buttons & 2) {
        mnuSetPopupEntry(popup, D_0037CC58);
        mnuClearListFlags(1, window);
    }
    mnuPlayInputSound(0, buttons, 0);
    return 0;
}

s32 ptySkillMenuEnterConfirm(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    SkillMenuState *menu = ((SkillMenuContext *)context)->menu;
    func_00272778(callback);
    mnuCreateStaffImageSprite(3);
    func_00272518(1, menu->selected->list->cursor->sortKey, D_003BAA98, context, 1, 1, 0x53);
    menu->selected->list->flags &= ~8;
    mnuDrawWindowContainer(0x1C0, 0x3D0, 0, (s32)menu->selected, 0x53);
    func_002723B0(0, ((SkillMenuContext *)context)->actor);
    return menuRunPanel((void *)context, 1, (void *)callback);
}

s32 ptySkillMenuDispatchConfirmRequest(s32 selection) {
    s32 context = kwlnTaskGetUserValue();
    return menuRunPanel((void *)context, 2, (void *)selection);
}

extern void mnuSelectPage(void *, u32);
extern void ptySkillMenuBuildEquippedSlots(s32, s32);
extern void ptySkillMenuInitPages(void *);

s32 ptySkillMenuOpenPartyPage(s32 menu) {
    u8 *ctx = (u8 *)kwlnTaskGetUserValue();
    u32 *panel = (u32 *)(ctx + 0x15C);

    mnuSelectPage(panel, ((SkillMenuContext *)ctx)->selection->cursor->index);
    *panel |= 0x400;
    ptySkillMenuBuildEquippedSlots(0, menu);
    ptySkillMenuInitPages(ctx);
    return 1;
}

s32 mnuCloseSelectionAndReleasePartyPanel(s32 selection) {
    s32 context = kwlnTaskGetUserValue();
    mnuDestroySelectedPartyWindow(selection);
    mnuDestroySkillMenuWindows(context);
    mnuReleasePageHandlesAndClearSelection(context + 0x15c);
    return 1;
}

extern void mnuClearListFlagsOneAndTwo();
extern void sndSetSequenceVolumePan(s32, s32, s32);

s32 ptySkillMenuHandlePageSwitch(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    s32 changed = 0;
    u32 buttons = mnuMapPadMaskToFlags(0x300);

    if (buttons & 0x100) {
        mnuCloseSelectionAndReleasePartyPanel(callback);
        mnuRetreatListCursorDefault(((SkillMenuContext *)context)->selection);
        changed = 1;
    }
    if ((buttons & 0x200) && changed == 0) {
        mnuCloseSelectionAndReleasePartyPanel(callback);
        mnuAdvanceListCursorDefault(((SkillMenuContext *)context)->selection);
        changed = 1;
    }
    mnuClearListFlagsOneAndTwo(((SkillMenuContext *)context)->selection);
    if (changed != 0) {
        ptySkillMenuOpenPartyPage(callback);
        sndSetSequenceVolumePan(4, 0x7F, 0x3F);
        return 1;
    }
    return 0;
}
