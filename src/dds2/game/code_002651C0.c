#include "mnu.h"
#include "dat_state.h"
#include "mnu_list.h"

extern s32 mdlFlagTest(u32);

extern u32 kwlnTaskGetUserValue();

extern void func_0026C900(void);


extern void func_00297970(s32);

extern s32 evtGetMessageWindowControlState(void);


extern char D_003CE63C[];
extern char (*D_00435E5C)[25];
extern char D_00437850[];
extern s32 func_00265038();
extern s32 evtGetCapturedWindowPanelValue();
extern void mnuShopReleaseWindowSprites(s32, MenuTerminalContext *);
extern void func_00260020();
extern s32 mnuCampHasEligibleOwnedItems();
extern void mnuAdvanceListCursorDefault();
extern void mnuShopLoadMessageResource(MenuTerminalContext *);
extern s32 mnuFirstPresentMainCharacterIndex();
extern void evtCreateEventScriptProcess();
extern void kwlnFadeOutStart();
extern void evtClearActiveFlag();
extern void evtSetBoundedDisplayValue();
extern u32 D_003CE460[];
extern char D_00437840[];
extern void mdlFlagSet();
extern void mdlFlagClear();
extern void func_0026C7F8();
extern s32 mnuCampResolveProgressTierValue();
extern void dspSetActive();
extern void evtCopyEntryStringToActiveWindow(s32, s32);
extern s32 dspStartEntry(s32);
extern void datAddCurrencyClamped();
extern void ptyAdjustItemQuantity();
extern s32 func_0035C860(char *, const char *, ...);
extern s32 evtIsLastSlot(s32);
extern void evtSetMessageWindowOptionWhenOpen(s32);
extern s32 evtStoreValueAndCaptureWindowPanelValue(s32);
struct KwlnTask;


s32 evtMenuPopulateSelectedSlotLabels(struct KwlnTask *task) {
    MenuTerminalContext *context = (MenuTerminalContext *)kwlnTaskGetUserValue(task);
    s32 slotIndex = func_00265038();
    s32 i;
    MnuProgressReward *reward;
    char text[0x40];

    if (context->unkCD != 0) {
        if (slotIndex >= 0) {
            i = 0;
            reward = D_003CE1A8[slotIndex].rewards;
            do {
                s32 value = reward->value;
                if ((reward++)->kind == 0) {
                    evtCopyEntryStringToActiveWindow(i, (s32)D_00435E5C[value]);
                } else {
                    func_0035C860(text, D_00437850, value);
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
    MenuTerminalContext *context = (MenuTerminalContext *)kwlnTaskGetUserValue();
    s32 *window = &context->popupState;
    s32 state = func_002C4038(&context->transitionWork, window, 0, (void *)callback);
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
    func_0025FD78((MenuTerminalContext *)context);
    func_00297970(context);
    return evtMenuSetHandler((void *)context, 1, (void *)callback);
}

s32 evtFinishPopupAfterMenuConfiguration(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_0026C7F8(1, 0);
    return evtMenuSetHandler((void *)context, 2, (void *)callback);
}

/* Hand out the captured slot's reward: an item (named in the window) or a currency amount. */
s32 func_00265408(void) {
    char text[0x40];
    MenuTerminalContext *context = (MenuTerminalContext *)kwlnTaskGetUserValue();
    s32 slotIndex = func_00265038();
    s32 choice;
    s32 rewardValue;

    context->rewardGranted = 0;
    if (context->unkCD != 0 && slotIndex >= 0) {
        choice = evtGetCapturedWindowPanelValue();
        rewardValue = D_003CE1A8[slotIndex].rewards[choice].value;
        if (D_003CE1A8[slotIndex].rewards[choice].kind == 0) {
            evtCopyEntryStringToActiveWindow(0, (s32)D_00435E5C[rewardValue]);
            ptyAdjustItemQuantity(rewardValue, 1);
        } else {
            func_0035C860(text, D_00437850, rewardValue);
            evtCopyEntryStringToActiveWindow(0, (s32)text);
            datAddCurrencyClamped(rewardValue);
        }
        context->rewardGranted = 1;
        dspStartEntry(0x28);
    }
    return 1;
}

/* Walk the list until its selected id is found, then persist the slot choice. */
s32 evtMenuPersistSelectedSlot(void) {
    MenuTerminalContext *context = (MenuTerminalContext *)kwlnTaskGetUserValue();
    s32 selectedId = context->ownedWindows[0]->list->cursor->camp.value;
    struct MenuListNode *node;
    MenuTerminalWindowState *record;
    s32 slot;
    mnuShopReleaseWindowSprites(1, context);
    func_00260020(context);
    for (node = context->ownedWindows[0]->list->first;
         node != 0 && node->camp.value != selectedId; node = node->next) {
        mnuAdvanceListCursorDefault(context->ownedWindows[0]->list);
    }
    record = context->ownedWindows[0]->list->context;
    slot = mnuCampHasEligibleOwnedItems(context);
    record->selectedSlot = slot;
    context->selectedSlot = slot;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002651C0", func_002655C0);

s32 func_002657F8(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_0025FD78((MenuTerminalContext *)context);
    func_00297970(context);
    return evtMenuSetHandler((void *)context, 1, (void *)callback);
}

s32 func_00265850(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    func_0026C900();
    return evtMenuSetHandler((void *)context, 2, (void *)callback);
}

/* Fade out according to the event mode, with a separate flag-dependent case 2. */
s32 evtStartFadeByState(void) {
    MenuTerminalContext *context = (MenuTerminalContext *)kwlnTaskGetUserValue();
    mnuShopLoadMessageResource(context);
    switch (context->type) {
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

u32 evtMenuSetProgressFlag(MenuTerminalContext *context) {
    u32 changed;
    s64 flagSet;

    if (((context->type == 2) && (flagSet = mdlFlagTest(4), flagSet != 0)) &&
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
    ptyAdjustItemQuantity(0x81, -datGameState->inventory.counts[0x81]);
    mdlFlagClear(0xa01);
}

/* One-shot menu flag: set the object's flag the first time it is not yet set, returning 1 only then. */
s32 func_00265A60(MenuTerminalContext *object) {
    u32 flag = D_003CE460[object->type];
    if (flag == 0) {
        return 0;
    }
    if (mdlFlagTest(flag) == 0) {
        mdlFlagSet(flag);
        return 1;
    }
    return 0;
}
