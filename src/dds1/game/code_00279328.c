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
extern s32 D_003BAA00;
extern s32 mnuIsEntryCostUnaffordable(u16, s32);
extern s32 ptySkillApplyFieldUseEffect(s32, s32, s32, s32);
extern void mnuConsumeEntryCost(s32, s32);
extern void mnuInitPartyPanelSlots(s32);
extern void mnuUpdateHandleStates(s32);
extern void func_00280048(s32);
extern u32 func_00285B20(u32);
extern s32 mnuGetAbilityByteCategory(u16);
extern void func_00280978();
extern void mnuSetPopupEntry(s32 *, char *);
extern char D_0037CC58[];
extern void mnuClearListFlags();
extern void mnuPlayInputSound(s32, u32, s32);

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
    u8 pad1C[8];
    SkillListWindow *selected;   /* 0x24 */
    u8 pad28[8];
    u32 selectionFlags;          /* 0x30 */
} SkillMenuState;

typedef struct SkillMenuContext {
    u8 pad00[0x78];
    s32 actor;                   /* 0x78 */
    u8 pad7C[0xA8];
    SkillListWindow *panel;      /* 0x124 */
    u8 pad128[0x34];
    u32 actionFlags;             /* 0x15C */
    u8 pad160[0x678];
    SkillList *selection;        /* 0x7D8 */
    SkillList *target;           /* 0x7DC */
    u8 pad7E0[0x12C];
    SkillMenuState *menu;        /* 0x90C */
} SkillMenuContext;

s64 ptySkillMenuUpdate(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    SkillMenuState *menu = ((SkillMenuContext *)context)->menu;
    s64 state = menuRunPanel(context, 0, callback);
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

INCLUDE_ASM(const s32, "game/code_00279328", ptySkillMenuCopyPageState);

s64 ptySkillMenuEnterPage(s32 callback) {
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
    return menuRunPanel(context, 1, callback);
}

s64 func_00279728(s32 selection) {
    s32 context = kwlnTaskGetUserValue();
    return menuRunPanel(context, 2, selection);
}

s32 ptySkillMenuUseSelectedInField(id, context)
    u16 id;
    s32 context;
{
    s32 window = context + 0x15C;
    s32 slotA = D_003BAA00 + ((SkillMenuContext *)context)->selection->cursor->index * 0x1A4 + 0xA60;
    s32 slotB = D_003BAA00 + ((SkillMenuContext *)context)->target->cursor->index * 0x1A4 + 0xA60;
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
    s32 slot = D_003BAA00 + ((SkillMenuContext *)context)->selection->cursor->index * 0x1A4 + 0xA60;
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

s64 ptySkillMenuHandleFieldUse(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    SkillMenuState *menu = ((SkillMenuContext *)context)->menu;
    s32 *popup = (s32 *)(context + 0x54);
    u32 buttons = func_00285B20(3);
    s64 state;
    s32 label;
    u16 code;
    s32 window;
    state = func_00285670(context + 8, popup, 0, callback);
    if (state != 0) {
        return state;
    }
    label = menu->selected->list->cursor->sortKey;
    code = label;
    if (mnuGetAbilityByteCategory(code) == 2) {
        ((SkillMenuContext *)context)->actionFlags |= 0x10;
    }
    if (mnuGetAbilityByteCategory(code) == 3) {
        ((SkillMenuContext *)context)->actionFlags |= 0x20;
    }
    window = context + 0x15C;
    func_00280978(8, window);
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

s64 ptySkillMenuEnterConfirm(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    SkillMenuState *menu = ((SkillMenuContext *)context)->menu;
    func_00272778(callback);
    mnuCreateStaffImageSprite(3);
    func_00272518(1, menu->selected->list->cursor->sortKey, D_003BAA98, context, 1, 1, 0x53);
    menu->selected->list->flags &= ~8;
    mnuDrawWindowContainer(0x1C0, 0x3D0, 0, (s32)menu->selected, 0x53);
    func_002723B0(0, ((SkillMenuContext *)context)->actor);
    return menuRunPanel(context, 1, callback);
}

s64 func_00279AF8(s32 selection) {
    s32 context = kwlnTaskGetUserValue();
    return menuRunPanel(context, 2, selection);
}

extern void mnuSelectPage(void *, u32);
extern void ptySkillMenuBuildEquippedSlots(s32, s32);
extern void ptySkillMenuInitPages(void *);

s32 func_00279B30(s32 menu) {
    u8 *ctx = (u8 *)kwlnTaskGetUserValue();
    u32 *panel = (u32 *)(ctx + 0x15C);

    mnuSelectPage(panel, ((SkillMenuContext *)ctx)->selection->cursor->index);
    *panel |= 0x400;
    ptySkillMenuBuildEquippedSlots(0, menu);
    ptySkillMenuInitPages(ctx);
    return 1;
}

s32 func_00279BA8(s32 selection) {
    s32 context = kwlnTaskGetUserValue();
    func_00277C80(selection);
    func_002786E8(context);
    func_002807E8(context + 0x15c);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00279328", ptySkillMenuHandlePageSwitch);
