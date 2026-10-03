#include "mnu.h"

extern s32 mdlFlagTest(u32);

extern u32 kwlnTaskGetUserValue();

extern void func_0026C900(void);

extern void func_0025FD78(s32);

extern void func_00297970(s32);

extern s32 evtGetMessageWindowControlState(void);

extern void mnuSetPopupEntryFlagged(s32 *, char *);

extern char D_003CE63C[];
typedef struct EvtSlotReward {
    u8 kind;
    s32 id;
} EvtSlotReward;

typedef struct EvtSlot {
    u32 threshold;
    s32 flag;
    EvtSlotReward sub[8];
} EvtSlot;

extern EvtSlot D_003CE1A8[];
extern char (*D_00435E5C)[25];
extern char D_00437850[];
extern s32 func_00265038();
extern s32 evtGetCapturedWindowPanelValue();
extern void mnuShopReleaseWindowSprites();
extern void func_00260020();
extern s32 mnuCampHasEligibleOwnedItems();
extern void mnuAdvanceListCursorDefault();
extern void mnuShopLoadMessageResource();
extern s32 mnuFirstPresentMainCharacterIndex();
extern void evtCreateEventScriptProcess();
extern void kwlnFadeOutStart();
extern void evtClearActiveFlag();
extern void evtSetBoundedDisplayValue();
extern u32 D_003CE460[];
extern s32 datGameState;
extern char D_00437840[];
extern void mdlFlagSet();
extern void mdlFlagClear();
extern void func_0026C7F8();
extern s32 mnuCampResolveProgressTierValue();
extern void dspSetActive();
extern void evtCopyEntryStringToActiveWindow(s32, s32);
extern s32 dspStartEntry(s32);
extern void datAddCurrencyClamped();
extern void func_0011A118();
extern s32 func_0035C860(char *, const char *, ...);
extern s32 evtIsLastSlot(s32);
extern void evtSetMessageWindowOptionWhenOpen(s32);
extern s32 evtStoreValueAndCaptureWindowPanelValue(s32);
struct KwlnTask;

/* Selection chain used by the event UI. Only accessed offsets are described. */
typedef struct EvtSelectionNode {
    u8 pad00[0x58];
    struct EvtSelectionNode *next; /* 0x58 */
    u8 pad5C[4];
    s32 id;                       /* 0x60 */
} EvtSelectionNode;

typedef struct EvtSelectionList {
    u8 pad00[0x10];
    EvtSelectionNode *first;      /* 0x10 */
    u8 pad14[8];
    EvtSelectionNode *selected;   /* 0x1C */
    u8 pad20[0x10];
    u8 *record;                   /* 0x30 */
} EvtSelectionList;

typedef struct EvtSelectionOwner {
    u8 pad00[0x18];
    EvtSelectionList *list;       /* 0x18 */
} EvtSelectionOwner;

typedef struct EvtMenuContext {
    u8 pad00[8];
    s32 mode;                     /* 0x08 */
    u8 pad0C[0x4C];
    s32 window;                   /* 0x58 */
    u8 pad5C[0x20];
    EvtSelectionOwner *selection; /* 0x7C */
    u8 pad80[0x26];
    u16 selectedSlot;             /* 0xA6 */
    u8 padA8[0x25];
    s8 unkCD;
} EvtMenuContext;

s32 evtMenuPopulateSelectedSlotLabels(struct KwlnTask *task) {
    EvtMenuContext *context = (EvtMenuContext *)kwlnTaskGetUserValue(task);
    s32 slotIndex = func_00265038();
    s32 i;
    EvtSlotReward *reward;
    char text[0x40];

    if (context->unkCD != 0) {
        if (slotIndex >= 0) {
            i = 0;
            reward = D_003CE1A8[slotIndex].sub;
            do {
                s32 id = reward->id;
                if ((reward++)->kind == 0) {
                    evtCopyEntryStringToActiveWindow(i, (s32)D_00435E5C[id]);
                } else {
                    func_0035C860(text, D_00437850, id);
                    evtCopyEntryStringToActiveWindow(i, (s32)text);
                }
                i++;
            } while (i < 3);
            if (evtIsLastSlot(slotIndex) == 0) {
                dspStartEntry(0x26);
            } else {
                dspStartEntry(0x2A);
            }
            evtSetMessageWindowOptionWhenOpen(0);
            evtStoreValueAndCaptureWindowPanelValue(0x27);
        }
    }
    return 1;
}

u32 func_002652D8(void) {
    return 1;
}

/* Poll the event window; when it closes, install the default window if needed. */
s32 evtMenuPollWindow(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    s32 *window = &((EvtMenuContext *)context)->window;
    s32 state = func_002C4038(context + 0xc, window, 0, callback);
    if (state == 0) {
        if (*window == 0) {
            if (evtGetMessageWindowControlState() == 0) {
                mnuSetPopupEntryFlagged(window, D_003CE63C);
            }
        }
        return 0;
    }
    return state;
}

s32 func_00265360(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_0025FD78(context);
    func_00297970(context);
    return evtMenuSetHandler(context, 1, callback);
}

s32 evtFinishPopupAfterMenuConfiguration(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_0026C7F8(1, 0);
    return evtMenuSetHandler(context, 2, callback);
}

INCLUDE_ASM(const s32, "game/code_002651C0", func_00265408);

/* Walk the list until its selected id is found, then persist the slot choice. */
s32 evtMenuPersistSelectedSlot(void) {
    s32 context = kwlnTaskGetUserValue();
    s32 selectedId = ((EvtMenuContext *)context)->selection->list->selected->id;
    EvtSelectionNode *node;
    u8 *record;
    s32 slot;
    mnuShopReleaseWindowSprites(1, context);
    func_00260020(context);
    for (node = ((EvtMenuContext *)context)->selection->list->first;
         node != 0 && node->id != selectedId; node = node->next) {
        mnuAdvanceListCursorDefault((s32)((EvtMenuContext *)context)->selection->list);
    }
    record = ((EvtMenuContext *)context)->selection->list->record;
    slot = mnuCampHasEligibleOwnedItems(context);
    *(u16 *)(record + 0x12) = slot;
    ((EvtMenuContext *)context)->selectedSlot = slot;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002651C0", func_002655C0);

s32 func_002657F8(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_0025FD78(context);
    func_00297970(context);
    return evtMenuSetHandler(context, 1, callback);
}

s32 func_00265850(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_0026C900();
    return evtMenuSetHandler(context, 2, callback);
}

/* Fade out according to the event mode, with a separate flag-dependent case 2. */
s32 evtStartFadeByState(void) {
    s32 context = kwlnTaskGetUserValue();
    mnuShopLoadMessageResource(context);
    switch (((EvtMenuContext *)context)->mode) {
    case 2:
        if (mdlFlagTest(0x42a) == 0 && mnuFirstPresentMainCharacterIndex() == 0) {
            evtCreateEventScriptProcess(0x323);
        } else {
            kwlnFadeOutStart(0, 0, 0, 0xf);
        }
        break;
    case 0:
    case 1:
    case 3:
        kwlnFadeOutStart(0, 0, 0, 0xf);
        break;
    }
    evtClearActiveFlag(0);
    evtSetBoundedDisplayValue(0, 0);
    evtSetBoundedDisplayValue(1, 1);
    return 1;
}

u32 func_00265980(void) {
    return 1;
}

u32 evtMenuSetProgressFlag(s32 context) {
    u32 changed;
    s64 flagSet;

    if (((((EvtMenuContext *)context)->mode == 2) && (flagSet = mdlFlagTest(4), flagSet != 0)) &&
          (flagSet = mdlFlagTest(0x290), flagSet == 0)) {
        mdlFlagSet(0x290);
        changed = 1;
    }
    else {
        changed = 0;
    }
    return changed;
}

void mnuAwardCampProgressCurrency(void) {
    char text[0x40];
    s32 index = mnuCampResolveProgressTierValue();
    dspSetActive(1);
    func_0035C860(text, D_00437840, index);
    evtCopyEntryStringToActiveWindow(0, (s32)text);
    dspStartEntry(0x19);
    datAddCurrencyClamped(index);
    func_0011A118(0x81, -*(u8 *)(datGameState + 0x13c1));
    mdlFlagClear(0xa01);
}

/* One-shot menu flag: set the object's flag the first time it is not yet set, returning 1 only then. */
s32 func_00265A60(s32 object) {
    u32 flag = D_003CE460[*(s32 *)(object + 8)];
    if (flag == 0) {
        return 0;
    }
    if (mdlFlagTest(flag) == 0) {
        mdlFlagSet(flag);
        return 1;
    }
    return 0;
}
