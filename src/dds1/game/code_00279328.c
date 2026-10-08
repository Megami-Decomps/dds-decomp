#include "kwln.h"
#include "mnu.h"
#include "mnu_list.h"
#include "mnu_shop.h"
#include "mnu_staff.h"
#include "dat_state.h"


extern void mnuCampMenuHandleInput(s32);
extern void ptySkillMenuHandleSelection(s32);
extern void ptySkillMenuHandleSlotReorder(s32);
extern void func_00272778(s32);
extern void mnuCreateStaffImageSprite(s32);
extern void mnuDrawStaffGridLabelsForKind(s32, s32);
extern void ptySkillMenuCopyPageState(s32);
extern void mnuDrawStaffCampScreen(s32, s32);
extern void func_00272518(s32, s32, s32, s32, s32, s32, s32);
extern void func_00272668(s32, s32, s32, s32, s32, s32);
extern void mnuDrawWindowContainer(s32, s32, s32, s32, s32);
extern u32 mnuHasSelectedListNodeId(s32);
extern s32 D_003BAA98;
extern void func_00280048(s32);
extern u32 mnuMapPadMaskToFlags(u32);
extern s32 mnuGetAbilityTargetCategory(u16);
extern void mnuStepPartyPanelListFromInput();
extern void mnuSetPopupEntry(s32 *, char *);
extern char D_0037CC58[];
extern void mnuClearListFlags();
extern void mnuPlayInputSound(s32, s32, u32 *);

/* These context pointers lack a proven allocator path; retain only their
 * observed cursor/list views until their owners are established. */
typedef struct SkillListView {
    u8 pad00[0x1C];
    struct MenuListNode *cursor;
} SkillListView;

typedef struct SkillListWindowView {
    u8 pad00[0x14];
    SkillListView *list;
} SkillListWindowView;

/* Native construction makes four standard windows for the skill pages. */
typedef struct SkillMenuState {
    u8 pad00[0xC];
    struct MenuList *partyList;             /* 0x0C */
    MenuWindowContainer *window[4];  /* 0x10 */
    s32 panelState;              /* 0x20 */
    MenuWindowContainer *selected;   /* 0x24 */
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
    SkillListWindowView *panel;  /* 0x124: producer not yet identified */
    u8 pad128[0x34];
    u32 actionFlags;             /* 0x15C */
    u8 pad160[0x678];
    SkillListView *selection;    /* 0x7D8 */
    SkillListView *target;       /* 0x7DC */
    u8 pad7E0[0xC];
    PartyPanel partyPanel;       /* 0x7EC: five party display rows */
    u8 pad8F8[0x14];
    SkillMenuState *menu;        /* 0x90C */
} SkillMenuContext;

extern void itfDrawGridWithResolvedSlot(s32, s32, s32, s32, s32, s32, s32);
extern void mnuSetPanelState(s32, s32);
extern void func_00282DA0(s32, s32, s32, s32, s32);

s32 ptySkillMenuUpdate(KwlnTask *callback) {
    s32 context = kwlnTaskGetUserValue(callback);
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

static inline MenuWindowContainer **getSkillPageSlot(MenuWindowContainer **windows,
                                                    s32 index) {
    return windows + index;
}

void ptySkillMenuCopyPageState(s32 context) {
    SkillMenuContext *work = (SkillMenuContext *)context;
    SkillMenuState *menu = work->menu;
    MenuWindowContainer **windows = menu->window;
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
            memcpy(&windows[i]->panel, &(*getSkillPageSlot(windows, index))->panel,
                   sizeof(MenuPanelHandles));
            windows[i]->fade = (*getSkillPageSlot(windows, index))->fade;
            windows[i]->flags = (*getSkillPageSlot(windows, index))->flags;
        }
    }
}

s32 ptySkillMenuEnterPage(KwlnTask *callback) {
    s32 context = kwlnTaskGetUserValue(callback);
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
    } else if (((MenuWindowContainer *)*(s32 *)((s32)menu + 0x10 + (menu->partyList->cursor->index << 2)))->list->cursor->index == 0) {
        mnuCreateStaffImageSprite(0xD);
    } else {
        mnuCreateStaffImageSprite(0xC);
    }
    label = menu->selected->list->cursor->sortKeyPrimary;
    if (label != 0xFFFF && label != 0) {
        func_00272518(1, label, D_003BAA98, context, 1, 1, 0x53);
    } else {
        func_00272668(1, 0, 0, context, 1, 0x53);
    }
    mnuDrawStaffGridLabelsForKind(0, work->actor);
    return menuRunPanel((void *)context, 1, (void *)callback);
}

s32 ptySkillMenuDispatchPageRequest(KwlnTask *selection) {
    s32 context = kwlnTaskGetUserValue(selection);
    return menuRunPanel((void *)context, 2, (void *)selection);
}

s32 ptySkillMenuUseSelectedInField(id, context)
    u16 id;
    s32 context;
{
    s32 window = context + 0x15C;
    DatPartyRecord *selectedEntry =
        &datGameState->party[((SkillMenuContext *)context)->selection->cursor->index];
    DatPartyRecord *targetEntry =
        &datGameState->party[((SkillMenuContext *)context)->target->cursor->index];
    if (mnuIsEntryCostUnaffordable(id, selectedEntry) != 0) {
        return 0;
    }
    if (ptySkillApplyFieldUseEffect((MenuPageWindow *)window, id,
                                    selectedEntry, targetEntry) != 0) {
        mnuConsumeEntryCost(id, selectedEntry);
        mnuInitPartyPanelSlots(&((SkillMenuContext *)context)->partyPanel);
        mnuUpdateHandleStates((MenuPageWindow *)window);
        func_00280048(window);
        return 1;
    }
    return 0;
}

void mnuFlagMatchingEntries(s32 context) {
    DatPartyRecord *selectedEntry =
        &datGameState->party[((SkillMenuContext *)context)->selection->cursor->index];
    struct MenuListNode *node = ((SkillMenuContext *)context)->menu->selected->list->first;
    if (node != NULL) {
        do {
            u16 skillId = (u16)node->sortKeyPrimary;
            if (mnuIsEntryCostUnaffordable(skillId, selectedEntry)) {
                node->flags48 |= 1;
            }
            node = node->next;
        } while (node != NULL);
    }
}

s32 ptySkillMenuHandleFieldUse(KwlnTask *callback) {
    s32 context = kwlnTaskGetUserValue(callback);
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
    label = menu->selected->list->cursor->sortKeyPrimary;
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

s32 ptySkillMenuEnterConfirm(KwlnTask *callback) {
    s32 context = kwlnTaskGetUserValue(callback);
    SkillMenuState *menu = ((SkillMenuContext *)context)->menu;
    func_00272778(callback);
    mnuCreateStaffImageSprite(3);
    func_00272518(1, menu->selected->list->cursor->sortKeyPrimary, D_003BAA98, context, 1, 1, 0x53);
    menu->selected->list->stateFlags &= ~8;
    mnuDrawWindowContainer(0x1C0, 0x3D0, 0, (s32)menu->selected, 0x53);
    mnuDrawStaffGridLabelsForKind(0, ((SkillMenuContext *)context)->actor);
    return menuRunPanel((void *)context, 1, (void *)callback);
}

s32 ptySkillMenuDispatchConfirmRequest(KwlnTask *selection) {
    s32 context = kwlnTaskGetUserValue(selection);
    return menuRunPanel((void *)context, 2, (void *)selection);
}

extern void mnuSelectPage(MenuPageWindow *, s32);
extern void ptySkillMenuBuildEquippedSlots(s32, s32);
extern void ptySkillMenuInitPages(void *);

s32 ptySkillMenuOpenPartyPage(KwlnTask *menu) {
    u8 *ctx = (u8 *)kwlnTaskGetUserValue(menu);
    MenuPageWindow *panel = (MenuPageWindow *)(ctx + 0x15C);

    mnuSelectPage(panel, ((SkillMenuContext *)ctx)->selection->cursor->index);
    panel->flags |= 0x400;
    ptySkillMenuBuildEquippedSlots(0, menu);
    ptySkillMenuInitPages(ctx);
    return 1;
}

s32 mnuCloseSelectionAndReleasePartyPanel(KwlnTask *selection) {
    s32 context = kwlnTaskGetUserValue(selection);
    mnuDestroySelectedPartyWindow(selection);
    mnuDestroySkillMenuWindows(context);
    mnuReleasePageHandlesAndClearSelection((MenuPageWindow *)(context + 0x15c));
    return 1;
}

extern void mnuClearListFlagsOneAndTwo();
extern void sndSetSequenceVolumePan(s32, s32, s32);

s32 ptySkillMenuHandlePageSwitch(KwlnTask *callback) {
    s32 context = kwlnTaskGetUserValue(callback);
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
