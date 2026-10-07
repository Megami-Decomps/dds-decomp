#include "common.h"
#include "mnu.h"
#include "dat_state.h"
#include "mnu_list.h"
#include "kwln.h"

/* The dispatcher passes its last argument to entry callbacks as opaque data,
 * not as a function address. Modes select polling, primary and secondary actions. */
#define EVT_DISPATCH_OPERATION_POLL 0
#define EVT_DISPATCH_OPERATION_PRIMARY 1
#define EVT_DISPATCH_OPERATION_SECONDARY 2

#define EVT_PROGRESS_SLOT_COUNT 8
#define EVT_PROGRESS_RECORD_WORDS 3
#define EVT_ALLOWED_ITEM_COUNT 3
#define EVT_PROGRESS_UNSIGNED_LIMIT 999999U
#define EVT_PROGRESS_FLAG_GATE_COUNT 1

extern s32 evtAdvanceSlotFlags(void);

extern s32 evtGetMessageWindowControlState(void);


extern s32 kwlnFadeIsActive(void);

extern u32 kwlnTaskGetUserValue();
extern s32 datAddCurrencyClamped(s32);
extern s32 mnuCampFindListedItemIndex(s32);
extern void mdlFlagClear(s32);
extern void func_002619A8(s32, s32);
extern void func_0025FD78(s32);
extern void func_00297320(s32);
extern void func_00297970(s32);


extern s32 D_003CE148[];
extern u8 D_003CE4EC[];
extern void func_00297220(struct MenuList *, u32);
extern s32 mnuCampAdvanceCounter(s32, MenuTerminalContext *);


extern u8 D_003CE498[];
extern s32 D_00435E48;
extern char D_00437840[];
extern s32 D_003C9A20[];
extern void evtCopyEntryStringToActiveWindow(s32, s32);
extern s32 func_0035C860(char *, const char *, ...);
extern void evtClearActiveFlag();
extern void evtSetBoundedDisplayValue();
extern void func_00260020();
extern s32 mnuCampHasEligibleOwnedItems();
extern struct MenuListNode *func_002B86E8(struct MenuList *);
extern s32 D_003CE14C[];
extern u8 D_003CE620[];
extern u8 D_003CE400[];
extern s32 mnuTickExtendedCommandPhase(MenuTerminalContext *);
extern s32 mdlFlagTest();
extern void mdlFlagSet();
extern s32 dspStartEntry(s32);


typedef struct EvtFlagGate {
    s8 threshold;
    u8 pad1;
    u16 cue;
    u32 flag;
} EvtFlagGate;


extern s32 func_002C5498();
extern u8 D_003CE604[];
extern u16 D_003CE3F8[];
extern void func_00261670();
extern void func_00294930();
extern s32 func_00298648(MenuTerminalContext *);

extern void mnuSetCommandPhase(MenuTerminalContext *, u32);

extern void func_0026C900(void);

extern u8 D_003CE4B4[];

extern void func_00297200(struct MenuList *, u32);

extern void func_00295D38();

extern u8 D_003CE4D0[];

extern u8 D_003CE508[];

extern s32 func_00261B98(s32);

extern u8 D_003CE690[];

extern void mnuStorePendingMenuCommandValue(struct MenuList *, u32);

extern void func_002958B0();
extern s32 mnuMapPadMaskToFlags(s32);
extern void func_002971E0(struct MenuList *, u32);
extern void func_002B9808(MenuWindowContainer *);
extern void mnuRetreatWindowListSelection(MenuWindowContainer *);
extern void mnuAdvanceWindowListSelection(MenuWindowContainer *);
extern void mnuHandleListPageJumpInput(s32, u8 *, u32 *);
extern void mnuPlayInputSound(s32, s32, u32 *);
extern u8 D_003CE578[];


s32 evtIsFadeDispatchIdle(void) {
    s32 fadeActive = kwlnFadeIsActive();

    if (fadeActive != 0) {
        return 0;
    }
    return evtGetMessageWindowControlState() == 0;
}

void evtInstallStateTable(s32 stateAddress) {
    if (((MenuTerminalContext *)stateAddress)->dispatchMode == 2) {
        ((MenuTerminalContext *)stateAddress)->stateTable = (s32)D_003CE498;
        mnuSetPopupEntry(&((MenuTerminalContext *)stateAddress)->popupState, (D_003CE498 + 0x118));
    }
}

/* Mirror the chosen slot into both the active scene record and dispatch state. */
s32 evtInitializeSelectedSlot(void) {
    s32 stateAddress = kwlnTaskGetUserValue();
    MenuTerminalWindowState *windowState;
    s32 selectedSlot;
    evtClearActiveFlag(0);
    evtSetBoundedDisplayValue(0, 2);
    if (((MenuTerminalContext *)stateAddress)->ownedWindows[0] == 0) {
        func_00260020(stateAddress);
    }
    windowState = ((MenuTerminalContext *)stateAddress)->ownedWindows[0]->list->context;
    selectedSlot = mnuCampHasEligibleOwnedItems(stateAddress);
    ((MenuTerminalContext *)stateAddress)->previousValue = datGameState->header.currency;
    windowState->selectedSlot = selectedSlot;
    ((MenuTerminalContext *)stateAddress)->selectedSlot = selectedSlot;
    return 1;
}

s32 evtAdvancePhaseOne(void) {
    MenuTerminalContext *state = (MenuTerminalContext *)kwlnTaskGetUserValue();
    if (state->phase == 1) {
        mnuSetCommandPhase(state, 4);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00261F48);

s32 func_00262190(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0025FD78(stateAddress);
    func_00297320(stateAddress);
    return func_002C4038(&((MenuTerminalContext *)stateAddress)->transitionWork, &((MenuTerminalContext *)stateAddress)->popupState, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

s32 evtSetupDispatchSync(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0026C900();
    return func_002C4038(&((MenuTerminalContext *)stateAddress)->transitionWork, &((MenuTerminalContext *)stateAddress)->popupState, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

void evtInstallStateTableB(s32 stateAddress) {
    if (((MenuTerminalContext *)stateAddress)->dispatchMode == 1) {
        ((MenuTerminalContext *)stateAddress)->stateTable = (s32)D_003CE4B4;
        mnuSetPopupEntry(&((MenuTerminalContext *)stateAddress)->popupState, (D_003CE4B4 + 0xfc));
    }
}

s32 func_00262270(void) {
    s32 stateAddress = kwlnTaskGetUserValue();
    if (((MenuTerminalContext *)stateAddress)->phase == 1) {
        func_00261670(stateAddress);
    }
    return 1;
}

s32 evtSelectStateAction(void) {
    s32 stateAddress = kwlnTaskGetUserValue();
    struct MenuList *linkedList;
    if (((MenuTerminalContext *)stateAddress)->phase == 5) {
        mnuSetCommandPhase((MenuTerminalContext *)stateAddress, 3);
    } else if (((MenuTerminalContext *)stateAddress)->phase == 7) {
        mnuSetCommandPhase((MenuTerminalContext *)stateAddress, 9);
        linkedList = ((MenuTerminalContext *)stateAddress)->window->list;
        linkedList->drawCallback = func_00295D38;
        func_00297200(linkedList, 0xa);
    }
    ((MenuTerminalContext *)stateAddress)->stateStep = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00262330);

s32 func_00262598(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0025FD78(stateAddress);
    func_00297970(stateAddress);
    return func_002C4038(&((MenuTerminalContext *)stateAddress)->transitionWork, &((MenuTerminalContext *)stateAddress)->popupState, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

s32 evtSetupDispatchSyncB(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0026C900();
    return func_002C4038(&((MenuTerminalContext *)stateAddress)->transitionWork, &((MenuTerminalContext *)stateAddress)->popupState, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

void evtInstallStateTableC(s32 stateAddress) {
    if (((MenuTerminalContext *)stateAddress)->dispatchMode == 1) {
        ((MenuTerminalContext *)stateAddress)->stateTable = (s32)D_003CE4D0;
        mnuSetPopupEntry(&((MenuTerminalContext *)stateAddress)->popupState, (D_003CE4D0 + 0xe0));
    }
}

s32 func_00262678(void) {
    s32 stateAddress = kwlnTaskGetUserValue();
    if (((MenuTerminalContext *)stateAddress)->phase == 1) {
        func_002619A8(stateAddress, 1);
    }
    return 1;
}

s32 evtSelectStateActionB(void) {
    s32 context = kwlnTaskGetUserValue();
    struct MenuList *linkedList;
    if (((MenuTerminalContext *)context)->phase == 5) {
        mnuSetCommandPhase((MenuTerminalContext *)context, 3);
    } else if (((MenuTerminalContext *)context)->phase == 7) {
        mnuSetCommandPhase((MenuTerminalContext *)context, 9);
        linkedList = ((MenuTerminalContext *)context)->window->list;
        linkedList->drawCallback = func_00295D38;
        func_00297200(linkedList, 0xa);
    }
    ((MenuTerminalContext *)context)->stateStep = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00262740);

s32 func_002629A8(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0025FD78(stateAddress);
    func_00297970(stateAddress);
    return func_002C4038(&((MenuTerminalContext *)stateAddress)->transitionWork, &((MenuTerminalContext *)stateAddress)->popupState, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

s32 func_00262A00(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0026C900();
    return func_002C4038(&((MenuTerminalContext *)stateAddress)->transitionWork, &((MenuTerminalContext *)stateAddress)->popupState, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

void func_00262A48(s32 stateAddress) {
    if (((MenuTerminalContext *)stateAddress)->dispatchMode == 1) {
        ((MenuTerminalContext *)stateAddress)->stateTable = (s32)D_003CE4EC;
        mnuSetPopupEntry(&((MenuTerminalContext *)stateAddress)->popupState, (D_003CE4EC + 0xc4));
    }
}

s32 func_00262A88(void) {
    s32 stateAddress = kwlnTaskGetUserValue();
    if (((MenuTerminalContext *)stateAddress)->phase == 1) {
        func_002619A8(stateAddress, 3);
    }
    return 1;
}

s32 mnuResetCommandStepAndSelectPhase(void) {
    s32 stateAddress = kwlnTaskGetUserValue();
    struct MenuList *linkedList;
    if (((MenuTerminalContext *)stateAddress)->phase == 5) {
        mnuSetCommandPhase((MenuTerminalContext *)stateAddress, 3);
    } else if (((MenuTerminalContext *)stateAddress)->phase == 7) {
        mnuSetCommandPhase((MenuTerminalContext *)stateAddress, 9);
        linkedList = ((MenuTerminalContext *)stateAddress)->window->list;
        linkedList->drawCallback = func_00295D38;
        func_00297200(linkedList, 0xa);
    }
    ((MenuTerminalContext *)stateAddress)->stateStep = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00262B50);

s32 func_00262DB8(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0025FD78(stateAddress);
    func_00297970(stateAddress);
    return func_002C4038(&((MenuTerminalContext *)stateAddress)->transitionWork, &((MenuTerminalContext *)stateAddress)->popupState, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

s32 func_00262E10(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0026C900();
    return func_002C4038(&((MenuTerminalContext *)stateAddress)->transitionWork, &((MenuTerminalContext *)stateAddress)->popupState, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

void evtInstallStateTableD(MenuTerminalContext *state) {
    if (state->dispatchMode == 2) {
        state->stateTable = (s32)D_003CE508;
        mnuSetPopupEntry(&state->popupState, D_003CE508 + 0xa8);
    }
}

s32 evtEnableStateFlag(void) {
    s32 stateAddress = kwlnTaskGetUserValue();
    if (((MenuTerminalContext *)stateAddress)->phase == 1 && !func_00261B98(stateAddress)) {
        ((MenuTerminalContext *)stateAddress)->dispatchMode = 2;
    }
    return 1;
}

s32 evtEnterProgressCommandPhase(void) {
    s32 stateAddress = kwlnTaskGetUserValue();
    struct MenuList *linkedList;
    if (((MenuTerminalContext *)stateAddress)->phase == 5) {
        mnuSetCommandPhase((MenuTerminalContext *)stateAddress, 3);
    } else if (((MenuTerminalContext *)stateAddress)->phase == 7) {
        mnuSetCommandPhase((MenuTerminalContext *)stateAddress, 9);
        linkedList = ((MenuTerminalContext *)stateAddress)->window->list;
        linkedList->drawCallback = func_00295D38;
        func_00297200(linkedList, 0xa);
    }
    ((MenuTerminalContext *)stateAddress)->stateStep = 0;
    return 1;
}

s32 func_00262F78(KwlnTask *task) {
    s32 dispatchResult;
    u32 inputFlags;
    MenuTerminalContext *state;
    s32 *dispatchSlot;
    struct MenuList *linkedList;
    struct MenuList *callbackList;

    state = (MenuTerminalContext *)kwlnTaskGetUserValue(task);
    inputFlags = mnuMapPadMaskToFlags(0xC33);
    dispatchSlot = &state->popupState;
    linkedList = state->window->list;
    dispatchResult = func_002C4038(&state->transitionWork, dispatchSlot, 0, task);
    if (dispatchResult == 0) {
        switch (mnuTickExtendedCommandPhase(state)) {
        case -1:
            break;
        case 4:
            mnuSetCommandPhase(state, 6);
            mnuStorePendingMenuCommandValue(linkedList, 10);
            break;
        case 5:
            mnuSetPopupEntryFlagged(dispatchSlot, D_003CE498);
            mnuStorePendingMenuCommandValue(state->ownedWindows[0]->list, 10);
            break;
        case 7:
            mnuSetPopupEntryFlagged(dispatchSlot, D_003CE578);
            break;
        case 8:
            mnuSetCommandPhase(state, 6);
            callbackList = state->window->list;
            callbackList->drawCallback = func_002958B0;
            mnuStorePendingMenuCommandValue(callbackList, 0);
            state->stateStep = 10;
            break;
        case 6:
            evtInstallStateTableD(state);
        default:
            if (state->popupState == 0) {
                if (inputFlags & 1) {
                    mnuSetCommandPhase(state, 7);
                } else if (inputFlags & 2) {
                    mnuSetCommandPhase(state, 5);
                    func_002971E0(linkedList, 4);
                } else if ((inputFlags & 0x300000) == 0) {
                    func_002B9808(state->window);
                } else if (inputFlags & 0x10) {
                    mnuRetreatWindowListSelection(state->window);
                } else if (inputFlags & 0x20) {
                    mnuAdvanceWindowListSelection(state->window);
                }
                mnuHandleListPageJumpInput(state->windowResource,
                                          (u8 *)state->window, &inputFlags);
            }
            mnuPlayInputSound(0, inputFlags, &state->window->list->stateFlags);
            break;
        }
        return 0;
    }
    return dispatchResult;
}

s32 func_00263180(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0025FD78(stateAddress);
    func_00297970(stateAddress);
    return func_002C4038(&((MenuTerminalContext *)stateAddress)->transitionWork, &((MenuTerminalContext *)stateAddress)->popupState, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

s32 func_002631D8(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0026C900();
    return func_002C4038(&((MenuTerminalContext *)stateAddress)->transitionWork, &((MenuTerminalContext *)stateAddress)->popupState, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

/* Return a slot's remaining threshold, or -1 for an unavailable slot. */
s32 evtGetRemainingSlotThreshold(u32 slotIndex) {
    s32 threshold;
    s32 remaining;
    if (slotIndex >= EVT_PROGRESS_SLOT_COUNT) {
        return -1;
    }
    threshold = D_003CE148[slotIndex * EVT_PROGRESS_RECORD_WORDS];
    if (threshold == 0) {
        return -1;
    }
    remaining = threshold - datGameState->progressTotal;
    if (remaining > 0) {
        return remaining;
    }
    return 0;
}

s32 evtShowResultText(void) {
    char formattedText[0x40];
    evtCopyEntryStringToActiveWindow(0, D_00435E48 + 0x11);
    func_0035C860(formattedText, D_00437840, datGameState->progressTotal);
    evtCopyEntryStringToActiveWindow(1, (s32)formattedText);
    evtCopyEntryStringToActiveWindow(2, D_003C9A20[datGameState->progressSlot]);
    func_0035C860(formattedText, D_00437840, evtGetRemainingSlotThreshold(datGameState->progressSlot + 1));
    evtCopyEntryStringToActiveWindow(3, (s32)formattedText);
    if (evtGetRemainingSlotThreshold(datGameState->progressSlot + 1) >= 0) {
        dspStartEntry(datGameState->progressSlot + 0x1a);
    } else {
        dspStartEntry(0x21);
    }
    return 1;
}

u32 func_00263378(void) {
    return 1;
}

s32 evtOpenProgressResultPopupWhenIdle(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    s32 *dispatchSlot = &((MenuTerminalContext *)stateAddress)->popupState;
    s32 dispatchResult = func_002C4038(&((MenuTerminalContext *)stateAddress)->transitionWork, dispatchSlot, EVT_DISPATCH_OPERATION_POLL, (void *)callbackContext);
    if (dispatchResult == 0) {
        if (evtGetMessageWindowControlState() == 0) {
            mnuSetPopupEntryFlagged(dispatchSlot, D_003CE498);
        }
        return 0;
    }
    return dispatchResult;
}

s32 func_002633F0(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0025FD78(stateAddress);
    func_00297320(stateAddress);
    return func_002C4038(&((MenuTerminalContext *)stateAddress)->transitionWork, &((MenuTerminalContext *)stateAddress)->popupState, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

s32 func_00263448(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0026C900();
    return func_002C4038(&((MenuTerminalContext *)stateAddress)->transitionWork, &((MenuTerminalContext *)stateAddress)->popupState, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00263490);

u32 func_002635E0(void) {
    return 1;
}

s32 func_002635E8(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    s32 *dispatchSlot = &((MenuTerminalContext *)stateAddress)->popupState;
    s32 dispatchResult = func_002C4038(&((MenuTerminalContext *)stateAddress)->transitionWork, dispatchSlot, EVT_DISPATCH_OPERATION_POLL, (void *)callbackContext);
    if (dispatchResult == 0) {
        if (evtGetMessageWindowControlState() == 0) {
            mnuSetPopupEntryFlagged(dispatchSlot, D_003CE498);
        }
        return 0;
    }
    return dispatchResult;
}

s32 func_00263658(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0025FD78(stateAddress);
    func_00297320(stateAddress);
    return func_002C4038(&((MenuTerminalContext *)stateAddress)->transitionWork, &((MenuTerminalContext *)stateAddress)->popupState, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

s32 func_002636B0(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0026C900();
    return func_002C4038(&((MenuTerminalContext *)stateAddress)->transitionWork, &((MenuTerminalContext *)stateAddress)->popupState, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

u32 evtResetStateProgressTimer(void) {
    MenuTerminalContext *state;

    state = (MenuTerminalContext *)kwlnTaskGetUserValue();
    state->retryFrames = 0;
    mnuSelectLastListNode(state->ownedWindows[0]->list);
    return 1;
}

/* Delay the next state table until dispatch is idle and 20 progress ticks elapse. */
s32 evtQueryStateProgress(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    s32 *dispatchSlot = &((MenuTerminalContext *)stateAddress)->popupState;
    s32 dispatchResult = func_002C4038(&((MenuTerminalContext *)stateAddress)->transitionWork, dispatchSlot, EVT_DISPATCH_OPERATION_POLL, (void *)callbackContext);
    if (dispatchResult == 0) {
        if (*dispatchSlot == 0) {
            if (evtGetMessageWindowControlState() == 0) {
                s32 progressTicks = ((MenuTerminalContext *)stateAddress)->retryFrames;
                if ((f32)progressTicks < 20.0f) {
                    ((MenuTerminalContext *)stateAddress)->retryFrames = progressTicks + 1;
                } else {
                    mnuSetPopupEntry(dispatchSlot, D_003CE690);
                }
            }
        }
        return 0;
    }
    return dispatchResult;
}

s32 evtDispatchProgressCallback(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0025FD78(stateAddress);
    func_00294930(stateAddress, ((MenuTerminalContext *)stateAddress)->retryFrames);
    return func_002C4038(&((MenuTerminalContext *)stateAddress)->transitionWork, &((MenuTerminalContext *)stateAddress)->popupState, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

s32 evtSetupDispatchSyncE(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0026C900();
    return func_002C4038(&((MenuTerminalContext *)stateAddress)->transitionWork, &((MenuTerminalContext *)stateAddress)->popupState, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

s32 evtApplyBaseRateProgressStep(void) {
    MenuTerminalContext *state = (MenuTerminalContext *)kwlnTaskGetUserValue();
    state->multiplier = 1;
    mnuCampAdvanceCounter(-1, state);
    return 1;
}

s32 evtAdvanceStateStage(void) {
    s32 stateAddress = kwlnTaskGetUserValue();
    struct MenuList *linkedList;
    if (((MenuTerminalContext *)stateAddress)->phase == 0xa) {
        ((MenuTerminalContext *)stateAddress)->stateStep = 0xa;
        mnuSetCommandPhase((MenuTerminalContext *)stateAddress, 6);
        linkedList = ((MenuTerminalContext *)stateAddress)->window->list;
        linkedList->drawCallback = func_002958B0;
        mnuStorePendingMenuCommandValue(linkedList, 0);
    }
    return 1;
}

extern s32 mnuMapPadMaskToFlags(s32);
extern s32 mnuTickCommandWaitPhase(MenuTerminalContext *);
extern void sndSetSequenceVolumePan(s32, s32, s32);
extern u8 D_003CE594[];

/* Poll command phases, adjust the idle quantity selection, and play UI sounds. */
s32 evtPollQuantitySelection(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    s32 *dispatchSlot = &((MenuTerminalContext *)stateAddress)->popupState;
    s32 previousQuantity = ((MenuTerminalContext *)stateAddress)->multiplier;
    CampWindowParams *values =
        &((MenuTerminalContext *)stateAddress)->window->list->cursor->camp;
    s32 input = mnuMapPadMaskToFlags(0xF000F3);
    s32 result = func_002C4038(&((MenuTerminalContext *)stateAddress)->transitionWork, dispatchSlot,
                             EVT_DISPATCH_OPERATION_POLL, (void *)callbackContext);

    if (result != 0) {
        return result;
    }
    switch (mnuTickCommandWaitPhase((MenuTerminalContext *)stateAddress)) {
    case -1:
        break;
    case 9:
        mnuSetCommandPhase((MenuTerminalContext *)stateAddress, 11);
        break;
    case 10: {
        struct MenuList *primaryList =
            ((MenuTerminalContext *)stateAddress)->ownedWindows[0]->list;
        values->value = values->price;
        mnuSetPopupEntryFlagged(dispatchSlot, (D_003CE4B4 +
            primaryList->cursor->camp.value * 0x1C));
        break;
    }
    case 12:
        mnuSetPopupEntryFlagged(dispatchSlot, D_003CE594);
        break;
    case 11:
    default:
        if (((MenuTerminalContext *)stateAddress)->popupState == 0) {
            if (input & 1) {
                mnuSetCommandPhase((MenuTerminalContext *)stateAddress, 12);
            } else if (input & 2) {
                mnuSetCommandPhase((MenuTerminalContext *)stateAddress, 10);
                func_00297220(
                    ((MenuTerminalContext *)stateAddress)->window->list, 10);
            }
            if (input & 0x300030) {
                if (input & 0x10) {
                    mnuCampAdvanceCounter(1, (MenuTerminalContext *)stateAddress);
                } else if (input & 0x20) {
                    mnuCampAdvanceCounter(-1, (MenuTerminalContext *)stateAddress);
                }
                if (previousQuantity == ((MenuTerminalContext *)stateAddress)->multiplier) {
                    input &= 3;
                }
            } else if (input & 0xC0) {
                if (input & 0x80) {
                    mnuCampAdvanceCounter(10, (MenuTerminalContext *)stateAddress);
                } else if (input & 0x40) {
                    mnuCampAdvanceCounter(-10, (MenuTerminalContext *)stateAddress);
                }
                if (previousQuantity == ((MenuTerminalContext *)stateAddress)->multiplier) {
                    input &= 3;
                }
            }
        }
        if (input & 0xF0) {
            sndSetSequenceVolumePan(1, 0x7F, 0x3F);
        }
        if (input & 1) {
            sndSetSequenceVolumePan(8, 0x7F, 0x3F);
        }
        if (input & 2) {
            sndSetSequenceVolumePan(10, 0x7F, 0x3F);
        }
        break;
    }
    return 0;
}

s32 func_00263B98(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0025FD78(stateAddress);
    func_00298648((MenuTerminalContext *)stateAddress);
    return func_002C4038(&((MenuTerminalContext *)stateAddress)->transitionWork, &((MenuTerminalContext *)stateAddress)->popupState, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

s32 func_00263BF0(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0026C900();
    return func_002C4038(&((MenuTerminalContext *)stateAddress)->transitionWork, &((MenuTerminalContext *)stateAddress)->popupState, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

extern void dspSetActive(s32);
extern void evtSetMessageWindowOptionWhenOpen(s32);
extern s32 evtStoreValueAndCaptureWindowPanelValue(s32);
s32 func_00263C38(void) {
    MenuTerminalContext *state = (MenuTerminalContext *)kwlnTaskGetUserValue();
    s32 entry = 7;
    CampWindowParams *item = &state->window->list->cursor->camp;
    s32 mode = state->ownedWindows[0]->list->cursor->camp.value + 1;
    char text[16];

    state->previousValue = datGameState->header.currency;
    state->elapsedFrames = 0;
    if (mode == 4) {
        if ((u32)(item->value * state->multiplier + datGameState->header.currency) > 9999999U) {
            entry = 12;
        }
    }
    func_0035C860(text, D_00437840, item->value * state->multiplier);
    evtCopyEntryStringToActiveWindow(3, (s32)text);
    dspSetActive(1);
    dspStartEntry(entry);
    evtSetMessageWindowOptionWhenOpen(0);
    evtStoreValueAndCaptureWindowPanelValue(11);
    state->window->list->drawCallback = func_00295D38;
    state->sceneReady = 0;
    return 1;
}


s32 evtUpdateSlotItemCompletionState(void) {
    s32 stateAddress = kwlnTaskGetUserValue();
    s32 nextStage = ((MenuTerminalContext *)stateAddress)->ownedWindows[0]->list->cursor->camp.value + 1;
    s32 itemId = ((MenuTerminalContext *)stateAddress)->window->list->cursor->camp.id;
    if (nextStage == 4) {
        if (datGameState->inventory.counts[itemId] == 0) {
            func_002B86E8(((MenuTerminalContext *)stateAddress)->window->list);
        }
        if (((MenuTerminalContext *)stateAddress)->window->list->count == 0) {
            ((MenuTerminalContext *)stateAddress)->dispatchMode = 2;
        }
    }
    return 1;
}

s32 evtPollStageSelectionAndAdvance(void) {
    s32 stateAddress = kwlnTaskGetUserValue();
    struct MenuList *linkedList;
    switch (mnuTickExtendedCommandPhase((MenuTerminalContext *)stateAddress)) {
    case 6:
        return 1;
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        return 0;
    case 8:
        mnuSetCommandPhase((MenuTerminalContext *)stateAddress, 6);
        linkedList = ((MenuTerminalContext *)stateAddress)->window->list;
        linkedList->drawCallback = func_002958B0;
        mnuStorePendingMenuCommandValue(linkedList, 0);
        ((MenuTerminalContext *)stateAddress)->stateStep = 0xa;
        return 0;
    default:
        return 0;
    }
}

/* The stage-selection caller forwards its task to the user-value lookup. */
void evtMarkSceneFollowupReadyAndQueueAction(s32 taskAddress) {
    s32 stateAddress = kwlnTaskGetUserValue(taskAddress);
    mnuSetCommandPhase((MenuTerminalContext *)stateAddress, 8);
    func_00297220(((MenuTerminalContext *)stateAddress)->window->list, 10);
    ((MenuTerminalContext *)stateAddress)->sceneReady = 1;
}

void evtAccumulateEligibleStageMultiplierValue(MenuTerminalContext *state) {
    CampWindowParams *entryValues = &state->window->list->cursor->camp;
    if (func_002C5498(entryValues->id)) {
        datGameState->world.score += entryValues->value * state->multiplier;
    }
}

s32 evtIsAllowedId(s32 itemId) {
    u32 allowedItemIndex;
    for (allowedItemIndex = 0; allowedItemIndex < EVT_ALLOWED_ITEM_COUNT; allowedItemIndex++) {
        if (D_003CE3F8[allowedItemIndex] == itemId) {
            return 1;
        }
    }
    return 0;
}

/* Add progress, treating zero as one. The unsigned check also clamps negative totals. */
s32 func_00263F50(s32 progressDelta) {
    if (progressDelta == 0) {
        progressDelta = 1;
    }
    datGameState->progressTotal += progressDelta;
    if ((u32)datGameState->progressTotal > EVT_PROGRESS_UNSIGNED_LIMIT) {
        datGameState->progressTotal = 999999;
    }
    return datGameState->progressTotal;
}

/* Apply the selected shop transaction and preserve its progress baseline. */
s32 func_00263FB0(KwlnTask *task) {
    MenuTerminalContext *state;
    CampWindowParams *values;
    s32 itemId;
    s32 operation;
    s32 total;

    state = (MenuTerminalContext *)kwlnTaskGetUserValue(task);
    values = &state->window->list->cursor->camp;
    itemId = values->id;
    operation = state->ownedWindows[0]->list->cursor->camp.value + 1;
    total = values->value * state->multiplier;
    state->unkC8 = datGameState->progressTotal;
    switch (operation) {
    case 1:
    case 2:
    case 3:
        datAddCurrencyClamped(-total);
        if (mnuCampFindListedItemIndex(itemId) < 0) {
            datGameState->inventory.counts[itemId] += state->multiplier;
        }
        if (!evtIsAllowedId(itemId)) {
            func_00263F50(total / 100);
        }
        break;
    case 4:
        datAddCurrencyClamped(total);
        datGameState->inventory.counts[itemId] -= state->multiplier;
        if (itemId == 0x54) {
            mdlFlagClear(0xA20);
        }
        func_00263F50(total / 100);
        break;
    }
    values->value = values->price;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00264120);

s32 evtDispatchSceneReadyFollowup(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0025FD78(stateAddress);
    if (((MenuTerminalContext *)stateAddress)->sceneReady == 1) {
        func_00297970(stateAddress);
    } else {
        func_00298648((MenuTerminalContext *)stateAddress);
    }
    return func_002C4038(&((MenuTerminalContext *)stateAddress)->transitionWork, &((MenuTerminalContext *)stateAddress)->popupState, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

s32 func_002642B8(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0026C900();
    return func_002C4038(&((MenuTerminalContext *)stateAddress)->transitionWork, &((MenuTerminalContext *)stateAddress)->popupState, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

s32 evtPlayDispatchModeCue(void) {
    s32 stateAddress = kwlnTaskGetUserValue();
    dspSetActive(1);
    switch (((MenuTerminalContext *)stateAddress)->dispatchMode) {
    case 1:
        dspStartEntry(5);
        break;
    case 2:
        dspStartEntry(6);
        break;
    }
    return 1;
}

s32 evtApplyDispatchModeState(void) {
    s32 stateAddress = kwlnTaskGetUserValue();
    switch (((MenuTerminalContext *)stateAddress)->dispatchMode) {
    case 1:
        mnuSetCommandPhase((MenuTerminalContext *)stateAddress, 6);
        break;
    case 2:
        if (((MenuTerminalContext *)stateAddress)->stateTable != (s32)D_003CE498) {
            mnuSetCommandPhase((MenuTerminalContext *)stateAddress, 5);
        }
        break;
    }
    ((MenuTerminalContext *)stateAddress)->dispatchMode = 0;
    return 1;
}

s32 evtSetPopupEntryWhenMessageWindowIdle(void *callbackContext) {
    s32 stateAddress;
    s32 result;
    s32 *dispatchSlot;

    stateAddress = kwlnTaskGetUserValue();
    dispatchSlot = &((MenuTerminalContext *)stateAddress)->popupState;
    result = func_002C4038(&((MenuTerminalContext *)stateAddress)->transitionWork, dispatchSlot, EVT_DISPATCH_OPERATION_POLL, callbackContext);
    if (result == 0) {
        if ((*dispatchSlot == 0) && (result = evtGetMessageWindowControlState(), result == 0)) {
            mnuSetPopupEntryFlagged(dispatchSlot, (void *)((MenuTerminalContext *)stateAddress)->stateTable);
        }
        result = 0;
    }
    return result;
}

extern void func_00294B40(s32, s32, s32, void *, s32, s32);
extern void func_00294D50(s32, s32, s32, void *, s32, s32);
extern void func_00294EB8(s32, s32, s32, void *, s32, s32);
extern void func_00295030(s32, s32, s32, void *, s32, s32, s32);
extern void mnuDrawIfActive(s32, s32, s32, void *, s32);
extern void func_002969D8(s32, s32, s32, void *, s32);
extern void mnuDrawListChildrenWithCountdown(s32, s32, s32, u8 *, s32);
extern void mnuClearWindowPanelTransitionFlag(void *);
extern void func_00296B48(s32, s32, s32, s32, s32, s32);
extern void func_00296D90(void *, s32);
extern void func_00296E98(s32, u32, s32, s32);

/* Draw the active dispatch mode before advancing its callback state. */
s32 func_00264480(s32 callbackContext) {
    MenuTerminalContext *state = (MenuTerminalContext *)kwlnTaskGetUserValue();

    func_0025FD78((s32)state);
    switch (state->dispatchMode) {
    case 1:
        func_00294B40(0, 0, 0, state, 0x100, 0x53);
        func_00294EB8(0, 0, 0, state, 0x100, 0x53);
        func_00295030(0, 0, 0, state, 0x100, 0, 0x53);
        mnuDrawIfActive(0, 0, 0, state->window, 0x53);
        func_002969D8(0, 0, 0, state, 0x53);
        func_00296D90(state, 0xA09DC380);
        func_00296E98((s32)state, 0x100, 3, 0x53);
        break;
    case 2:
        if (state->stateTable == (s32)D_003CE498) {
            func_00294B40(0, 0, 0, state, 0x100, 0x53);
            func_00294D50(0, 0, 0, state, 0x100, 0x53);
            mnuDrawListChildrenWithCountdown(
                0, 0, 0, (u8 *)state->ownedWindows[0]->list, 0x53);
            func_00296D90(state, 0xA09DC380);
            func_00296E98((s32)state, 0x100, 4, 0x53);
        } else {
            func_00294B40(0, 0, 0, state, 0x100, 0x53);
            func_00294EB8(0, 0, 0, state, 0x100, 0x53);
            func_00295030(0, 0, 0, state, 0x100, 0, 0x53);
            mnuClearWindowPanelTransitionFlag(state->ownedWindows[0]);
            func_00296B48(0, 0, 0, (s32)state, 0x100, 0x53);
            func_00296D90(state, 0xA09DC380);
            func_00296E98((s32)state, 0x100, 2, 0x53);
        }
        break;
    }
    return func_002C4038(&state->transitionWork, &state->popupState, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

s32 func_002646C8(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0026C900();
    return func_002C4038(&((MenuTerminalContext *)stateAddress)->transitionWork, &((MenuTerminalContext *)stateAddress)->popupState, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00264710);

u32 func_00264848(void) {
    return 1;
}

extern void ptyAdjustItemQuantity(s32, s32);
extern s32 func_00260DF0(s32);
extern u8 func_00260FE8(s32, s32);
extern u8 func_00261018(s32, s32);
extern s32 mnuCampResolveOwnedItemVariant(s32, s32);
extern s32 mnuCampGetCompactEntryId(s32, s32);
extern s32 func_002C54B0(s32);
extern char (*D_00435E5C)[25];
extern u8 D_003CE5E8[];
extern char D_00437848[];
extern char D_00437850[];

s32 evtAdvancePendingRewards(s32 callbackContext) {
    char text[64];
    MenuTerminalContext *state;
    s32 *dispatchSlot;
    s32 result;
    s32 column;
    s32 sequence;

    state = (MenuTerminalContext *)kwlnTaskGetUserValue();
    dispatchSlot = &state->popupState;
    result = func_002C4038(&state->transitionWork, dispatchSlot,
                         EVT_DISPATCH_OPERATION_POLL, (void *)callbackContext);
    if (result == 0) {
        if (*dispatchSlot == 0 &&
            (result = evtGetMessageWindowControlState()) == 0) {
            if (state->grantPendingReward != 0) {
                if (state->rewardKind == 0) {
                    ptyAdjustItemQuantity(state->rewardValue, 1);
                } else {
                    datAddCurrencyClamped(state->rewardValue);
                }
                state->grantPendingReward = 0;
            }
            if (state->announceNextReward != 0) {
                state->announceNextReward = 0;
                dspStartEntry(0x2F);
            } else if (state->remainingRewards == 0) {
                mnuSetPopupEntryFlagged(dispatchSlot, D_003CE5E8);
            } else if (state->rewardDelay <= 0) {
                if (state->rewardMode < 0) {
                    column = func_00260DF0(state->rewardRow);
                    state->rewardKind = func_00260FE8(state->rewardRow, column);
                    state->rewardValue = mnuCampResolveOwnedItemVariant(state->rewardRow, column);
                } else {
                    state->rewardKind = func_00261018(state->rewardRow, state->rewardIndex);
                    state->rewardValue = mnuCampGetCompactEntryId(state->rewardRow, state->rewardIndex);
                }
                if (state->rewardKind == 0 && func_002C54B0(state->rewardValue) != 0) {
                    state->rewardDelay = 60;
                    sequence = 0x300003;
                } else {
                    state->rewardDelay = 30;
                    sequence = 0x300002;
                }
                sndSetSequenceVolumePan(sequence, 0x7F, 0x3F);
            } else {
                state->rewardDelay--;
                if (state->rewardDelay > 0) {
                    return 0;
                }
                if (state->rewardKind == 0) {
                    evtCopyEntryStringToActiveWindow(0, (s32)D_00435E5C[state->rewardValue]);
                    evtCopyEntryStringToActiveWindow(1, (s32)D_00437848);
                    dspStartEntry(0x2D);
                } else {
                    func_0035C860(text, D_00437850, state->rewardValue);
                    evtCopyEntryStringToActiveWindow(0, (s32)text);
                    dspStartEntry(0x2E);
                }
                state->remainingRewards--;
                state->rewardIndex++;
                if (state->remainingRewards != 0) {
                    state->announceNextReward = 1;
                }
                state->grantPendingReward = 1;
            }
        }
        result = 0;
    }
    return result;
}

s32 func_00264AB8(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0025FD78(stateAddress);
    func_00297970(stateAddress);
    return func_002C4038(&((MenuTerminalContext *)stateAddress)->transitionWork, &((MenuTerminalContext *)stateAddress)->popupState, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

s32 func_00264B10(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0026C900();
    return func_002C4038(&((MenuTerminalContext *)stateAddress)->transitionWork, &((MenuTerminalContext *)stateAddress)->popupState, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

/* Mark each newly completed slot and advance the persistent slot index. */
s32 evtAdvanceSlotFlags(void) {
    s32 advancedSlots = 0;
    while (evtGetRemainingSlotThreshold(datGameState->progressSlot + advancedSlots + 1) == 0) {
        mdlFlagSet(D_003CE14C[(datGameState->progressSlot + advancedSlots) * EVT_PROGRESS_RECORD_WORDS + EVT_PROGRESS_RECORD_WORDS]);
        advancedSlots++;
    }
    datGameState->progressSlot += advancedSlots;
    return advancedSlots;
}

u32 evtUpdateSlotAdvanceCount(void) {
    u8 advancedSlots;
    s32 stateAddress;

    stateAddress = kwlnTaskGetUserValue();
    advancedSlots = evtAdvanceSlotFlags();
    ((MenuTerminalContext *)stateAddress)->advancedSlots = advancedSlots;
    if ((((MenuTerminalContext *)stateAddress)->unkC8 == 0) && (((MenuTerminalContext *)stateAddress)->unkCD == '\x01')) {
        dspStartEntry(0x22);
    }
    return 1;
}

u32 func_00264C58(void) {
    return 1;
}

s32 evtOpenSlotAdvancePopupWhenIdle(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    s32 *dispatchSlot = &((MenuTerminalContext *)stateAddress)->popupState;
    s32 dispatchResult = func_002C4038(&((MenuTerminalContext *)stateAddress)->transitionWork, dispatchSlot, EVT_DISPATCH_OPERATION_POLL, (void *)callbackContext);
    if (dispatchResult == 0) {
        if (*dispatchSlot == 0) {
            if (evtGetMessageWindowControlState() == 0) {
                mnuSetPopupEntryFlagged(dispatchSlot, D_003CE604);
            }
        }
        return 0;
    }
    return dispatchResult;
}

s32 func_00264CE0(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0025FD78(stateAddress);
    func_00297970(stateAddress);
    return func_002C4038(&((MenuTerminalContext *)stateAddress)->transitionWork, &((MenuTerminalContext *)stateAddress)->popupState, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

s32 func_00264D38(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0026C900();
    return func_002C4038(&((MenuTerminalContext *)stateAddress)->transitionWork, &((MenuTerminalContext *)stateAddress)->popupState, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

/* Raise a gate's flag and play its cue only on the first threshold crossing. */
s32 evtTriggerProgressFlagGate(s32 unusedContext) {
    EvtFlagGate *gateEntry = (EvtFlagGate *)D_003CE400;
    u32 gateIndex;
    for (gateIndex = 0; gateIndex < EVT_PROGRESS_FLAG_GATE_COUNT; gateIndex++, gateEntry++) {
        if ((u32)(datGameState->progressSlot + 1) >= (u32)gateEntry->threshold) {
            u32 progressFlag = gateEntry->flag;
            if (mdlFlagTest(progressFlag) == 0) {
                mdlFlagSet(progressFlag);
                dspStartEntry(gateEntry->cue);
                return 1;
            }
        }
    }
    return 0;
}

s32 evtShowSlotText(void) {
    char formattedText[0x40];
    if (((MenuTerminalContext *)kwlnTaskGetUserValue())->advancedSlots > 0) {
        evtCopyEntryStringToActiveWindow(0, D_00435E48 + 0x11);
        func_0035C860(formattedText, D_00437840, D_003CE148[datGameState->progressSlot * EVT_PROGRESS_RECORD_WORDS]);
        evtCopyEntryStringToActiveWindow(1, (s32)formattedText);
        evtCopyEntryStringToActiveWindow(2, D_003C9A20[datGameState->progressSlot]);
        if (evtGetRemainingSlotThreshold(datGameState->progressSlot + 1) >= 0) {
            dspStartEntry(0x24);
        } else {
            dspStartEntry(0x25);
        }
    }
    return 1;
}

u32 func_00264EF8(void) {
    return 1;
}

s32 evtTriggerProgressGateThenOpenPopup(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    s32 *dispatchSlot = &((MenuTerminalContext *)stateAddress)->popupState;
    s32 dispatchResult = func_002C4038(&((MenuTerminalContext *)stateAddress)->transitionWork, dispatchSlot, EVT_DISPATCH_OPERATION_POLL, (void *)callbackContext);
    if (dispatchResult == 0) {
        if (*dispatchSlot == 0) {
            if (evtGetMessageWindowControlState() == 0) {
                if (evtTriggerProgressFlagGate(stateAddress) == 0) {
                    mnuSetPopupEntryFlagged(dispatchSlot, D_003CE620);
                }
            }
        }
        return 0;
    }
    return dispatchResult;
}

s32 func_00264F98(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0025FD78(stateAddress);
    func_00297970(stateAddress);
    return func_002C4038(&((MenuTerminalContext *)stateAddress)->transitionWork, &((MenuTerminalContext *)stateAddress)->popupState, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

s32 func_00264FF0(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    func_0026C900();
    return func_002C4038(&((MenuTerminalContext *)stateAddress)->transitionWork, &((MenuTerminalContext *)stateAddress)->popupState, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00265038);

s32 evtIsLastSlot(s32 slotIndex) {
    s32 activeSlots = 0;
    u32 entryIndex;
    for (entryIndex = 0; entryIndex < EVT_PROGRESS_SLOT_COUNT; entryIndex++) {
        if (D_003CE1A8[entryIndex].flag != 0) {
            activeSlots++;
        }
    }
    return slotIndex + 1 == activeSlots;
}

INCLUDE_RODATA(const s32, "game/code_00261E10", D_00424D08);

INCLUDE_SDATA(const s32, "game/code_00261E10", D_00437840);

INCLUDE_SDATA(const s32, "game/code_00261E10", D_00437848);

INCLUDE_SDATA(const s32, "game/code_00261E10", D_00437850);
