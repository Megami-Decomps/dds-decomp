#include "common.h"
#include "kwln.h"
#include "mnu.h"
#include "mnu_list.h"
#include "mnu_shop.h"
#include "dat_state.h"

/* The dispatcher passes its last argument to entry callbacks as opaque data,
 * not as a function address. Modes select polling, primary and secondary actions. */
#define EVT_DISPATCH_OPERATION_POLL 0
#define EVT_DISPATCH_OPERATION_PRIMARY 1
#define EVT_DISPATCH_OPERATION_SECONDARY 2

#define MNU_SCENE_FLAG_PAIR_COUNT 4
#define MNU_PARTY_FLAG_PAIR_COUNT 16
#define MNU_FLAG_SNAPSHOT_BYTES 0x140

extern void func_0025DF68(s32, s32);

extern s32 func_00261760(ShopScene *);

extern void mnuStorePendingMenuCommandValue(struct MenuList *, u32);
extern void func_0025ECD0();
extern u8 D_0036AB64[];

extern void func_0025E108(s32, s32);

extern void mnuSetCommandPhase(ShopScene *, u32);
extern void func_00260570(struct MenuList *, u32);
extern void func_00260AB0(s32);
extern void func_0025F138();
extern s32 func_00245A40(s32);
extern void evtClearActiveFlag(s32);
extern s32 evtSetBoundedDisplayValue(s32, s32);
extern void func_002E96D8(s32);

extern void func_00260670(s32 context);

extern void func_0024DD78(void);
extern void mnuSetPopupEntry(s32, s32);
extern u8 D_0036AA68[];
extern u8 D_0036AA84[];
extern u8 D_0036AAA0[];
extern u8 D_0036AABC[];

extern s32 kwlnFadeIsActive(void);

extern s32 evtGetMessageWindowControlState(void);


extern u32 kwlnTaskGetUserValue();

extern s32 mnuMapPadMaskToFlags(s32);
extern s32 mnuTickExtendedCommandPhase(ShopScene *);
extern void mnuSetPopupEntryFlagged(s32, s32);
extern void func_00260550(struct MenuList *, u32);
extern s32 mnuCampClampSceneCounter(s32, ShopScene *);
extern void func_0027C788(MenuWindowContainer *);
extern void mnuRetreatWindowListSelection(MenuWindowContainer *);
extern void mnuAdvanceWindowListSelection(MenuWindowContainer *);
extern void mnuPlayInputSound(s32, s32, u32 *);
extern u8 D_0036AAF4[];
extern void mnuSelectLastListNode(struct MenuList *);


/* Wait until both the keyword fade and the dispatch callback are idle. */
s32 evtIsFadeDispatchIdle(void) {
    s32 fading = kwlnFadeIsActive();

    if (fading != 0) {
        return 0;
    }
    return evtGetMessageWindowControlState() == 0;
}

void evtInstallStateTable(ShopScene *state) {
    if (state->menuMode == 2) {
        state->stateTable = (s32)D_0036AA68;
        mnuSetPopupEntry((s32)&state->dispatchState, (s32)D_0036AA68 + 0xC4);
    }
}

extern s32 func_00244658();
extern void func_002444D0(ShopScene *);
extern s16 mnuShopHasPendingFlag(ShopScene *);

/* Prepare the active menu state and copy the selection into its window. */
s32 evtInitializeActiveMenuState(void) {
    s32 stateAddress = kwlnTaskGetUserValue();
    MnuShopListContext *windowData;
    s16 pendingSelection;

    evtClearActiveFlag(0);
    evtSetBoundedDisplayValue(0, 2);
    if (((ShopScene *)stateAddress)->sprite == 0) {
        ((ShopScene *)stateAddress)->extraOption = func_00244658(stateAddress);
        func_002444D0((ShopScene *)stateAddress);
    }
    windowData = ((ShopScene *)stateAddress)->sprite->list->context;
    pendingSelection = mnuShopHasPendingFlag((ShopScene *)stateAddress);
    ((ShopScene *)stateAddress)->previousValue = datGameState->header.currency;
    windowData->pendingSelection = pendingSelection;
    ((ShopScene *)stateAddress)->pendingSelection = pendingSelection;
    return 1;
}

/* Advance command phase when the current menu state is phase one. */
s32 evtAdvancePhaseOne(void) {
    ShopScene *state = (ShopScene *)kwlnTaskGetUserValue();

    if (state->action == 1) {
        mnuSetCommandPhase(state, 4);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_00245DE0);

s32 evtPrimeDispatchStart(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();

    func_00260670(stateAddress);
    return menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

s32 evtSetupDispatchSync(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();

    func_0024DD78();
    return menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

void evtInstallStateTableB(ShopScene *state) {
    if (state->menuMode == 1) {
        state->stateTable = (s32)D_0036AA84;
        mnuSetPopupEntry((s32)&state->dispatchState, (s32)D_0036AA84 + 0xA8);
    }
}

s32 func_00246160(void) {
    ShopScene *state = (ShopScene *)kwlnTaskGetUserValue();

    if (state->action == 1) {
        func_002453C8(state);
    }
    return 1;
}

/* Map actions five and seven to their menu phases, then reset substate. */
s32 evtSelectStateAction(void) {
    s32 stateAddress = kwlnTaskGetUserValue();
    s32 action = ((ShopScene *)stateAddress)->action;

    if (action == 5) {
        mnuSetCommandPhase((ShopScene *)stateAddress, 3);
    } else if (action == 7) {
        struct MenuList *linkedTask;

        mnuSetCommandPhase((ShopScene *)stateAddress, 9);
        linkedTask = ((ShopScene *)stateAddress)->window->list;
        linkedTask->drawCallback = func_0025F138;
        func_00260570(linkedTask, 10);
    }
    ((ShopScene *)stateAddress)->substate = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_00246220);

s32 evtStageDispatchStart(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();

    func_00260AB0(stateAddress);
    return menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

s32 evtSetupDispatchSyncB(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();

    func_0024DD78();
    return menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

void evtInstallStateTableC(ShopScene *state) {
    if (state->menuMode == 1) {
        state->stateTable = (s32)D_0036AAA0;
        mnuSetPopupEntry((s32)&state->dispatchState, (s32)D_0036AAA0 + 0x8C);
    }
}

s32 func_00246538(void) {
    ShopScene *state = (ShopScene *)kwlnTaskGetUserValue();

    if (state->action == 1) {
        func_002457E8(state);
    }
    return 1;
}

s32 evtSelectStateActionB(void) {
    s32 stateAddress = kwlnTaskGetUserValue();
    s32 action = ((ShopScene *)stateAddress)->action;

    if (action == 5) {
        mnuSetCommandPhase((ShopScene *)stateAddress, 3);
    } else if (action == 7) {
        struct MenuList *linkedTask;

        mnuSetCommandPhase((ShopScene *)stateAddress, 9);
        linkedTask = ((ShopScene *)stateAddress)->window->list;
        linkedTask->drawCallback = func_0025F138;
        func_00260570(linkedTask, 10);
    }
    ((ShopScene *)stateAddress)->substate = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_002465F8);

s32 evtStageDispatchStartB(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();

    func_00260AB0(stateAddress);
    return menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

s32 evtSetupDispatchSyncC(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();

    func_0024DD78();
    return menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

void evtInstallStateTableD(ShopScene *state) {
    if (state->menuMode == 2) {
        state->stateTable = (s32)D_0036AABC;
        mnuSetPopupEntry((s32)&state->dispatchState, (s32)D_0036AABC + 0x70);
    }
}

s32 evtEnableStateFlag(void) {
    s32 stateAddress = kwlnTaskGetUserValue();

    if ((((ShopScene *)stateAddress)->action == 1) && (func_00245A40(stateAddress) == 0)) {
        ((ShopScene *)stateAddress)->menuMode = 2;
    }
    return 1;
}

s32 evtSelectStateActionC(void) {
    s32 stateAddress = kwlnTaskGetUserValue();
    s32 action = ((ShopScene *)stateAddress)->action;

    if (action == 5) {
        mnuSetCommandPhase((ShopScene *)stateAddress, 3);
    } else if (action == 7) {
        struct MenuList *linkedTask;

        mnuSetCommandPhase((ShopScene *)stateAddress, 9);
        linkedTask = ((ShopScene *)stateAddress)->window->list;
        linkedTask->drawCallback = func_0025F138;
        func_00260570(linkedTask, 10);
    }
    ((ShopScene *)stateAddress)->substate = 0;
    return 1;
}

/* Poll the dispatch slot, then process command phases and mapped input flags. */
s32 func_002469F0(KwlnTask *task) {
    s32 dispatchResult;
    s32 inputFlags;
    s32 stateAddress;
    s32 *dispatchSlot;
    struct MenuList *linkedTask;
    struct MenuList *callbackTask;

    stateAddress = kwlnTaskGetUserValue(task);
    inputFlags = mnuMapPadMaskToFlags(0x33);
    dispatchSlot = &((ShopScene *)stateAddress)->dispatchState;
    linkedTask = ((ShopScene *)stateAddress)->window->list;
    dispatchResult = menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_POLL, task);
    if (dispatchResult == 0) {
        switch (mnuTickExtendedCommandPhase((ShopScene *)stateAddress)) {
        case -1:
            break;
        case 4:
            mnuSetCommandPhase((ShopScene *)stateAddress, 6);
            mnuStorePendingMenuCommandValue(linkedTask, 10);
            break;
        case 5:
            mnuSetPopupEntryFlagged((s32)dispatchSlot, (s32)D_0036AA68);
            mnuStorePendingMenuCommandValue(((ShopScene *)stateAddress)->sprite->list, 10);
            break;
        case 7:
            mnuSetPopupEntryFlagged((s32)dispatchSlot, (s32)D_0036AAF4);
            break;
        case 8:
            mnuSetCommandPhase((ShopScene *)stateAddress, 6);
            callbackTask = ((ShopScene *)stateAddress)->window->list;
            callbackTask->drawCallback = func_0025ECD0;
            mnuStorePendingMenuCommandValue(callbackTask, 0);
            ((ShopScene *)stateAddress)->substate = 10;
            break;
        case 6:
            evtInstallStateTableD((ShopScene *)stateAddress);
            /* Continue into the common input handling after installing the table. */
        default:
            if (((ShopScene *)stateAddress)->dispatchState == 0) {
                if (inputFlags & 1) {
                    mnuSetCommandPhase((ShopScene *)stateAddress, 7);
                } else if (inputFlags & 2) {
                    mnuSetCommandPhase((ShopScene *)stateAddress, 5);
                    func_00260550(linkedTask, 4);
                } else if ((inputFlags & 0x300000) == 0) {
                    func_0027C788(((ShopScene *)stateAddress)->window);
                } else if (inputFlags & 0x10) {
                    mnuRetreatWindowListSelection(((ShopScene *)stateAddress)->window);
                } else if (inputFlags & 0x20) {
                    mnuAdvanceWindowListSelection(((ShopScene *)stateAddress)->window);
                }
            }
            mnuPlayInputSound(0, inputFlags, &((ShopScene *)stateAddress)->window->list->stateFlags);
            break;
        }
        return 0;
    }
    return dispatchResult;
}

s32 evtStageDispatchStartC(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();

    func_00260AB0(stateAddress);
    return menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

s32 evtSetupDispatchSyncD(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();

    func_0024DD78();
    return menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

u32 evtResetStateProgressTimer(void) {
    s32 stateAddress;

    stateAddress = kwlnTaskGetUserValue();
    ((ShopScene *)stateAddress)->progressTicks = 0;
    mnuSelectLastListNode(((ShopScene *)stateAddress)->sprite->list);
    return 1;
}

/* Keep the dispatch query alive for 20 idle ticks before restarting its table. */
s32 evtQueryStateProgress(void *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    s32 dispatchResult = menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_POLL, callbackContext);

    if (dispatchResult == 0) {
        if ((((ShopScene *)stateAddress)->dispatchState == 0) && (evtGetMessageWindowControlState() == 0)) {
            s32 progressTicks = ((ShopScene *)stateAddress)->progressTicks;

            if ((f32)progressTicks < 20.0f) {
                ((ShopScene *)stateAddress)->progressTicks = progressTicks + 1;
            } else {
                mnuSetPopupEntry((s32)&((ShopScene *)stateAddress)->dispatchState, (s32)D_0036AB64);
            }
        }
        dispatchResult = 0;
    }
    return dispatchResult;
}

s32 evtFetchDispatchStart(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();

    func_0025E108(stateAddress, ((ShopScene *)stateAddress)->progressTicks);
    return menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

s32 evtSetupDispatchSyncE(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();

    func_0024DD78();
    return menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

s32 evtApplyBaseRateProgressStep(void) {
    ShopScene *state = (ShopScene *)kwlnTaskGetUserValue();

    state->counter = 1;
    mnuCampClampSceneCounter(-1, state);
    return 1;
}

s32 evtAdvanceStateStage(void) {
    s32 stateAddress = kwlnTaskGetUserValue();

    if (((ShopScene *)stateAddress)->action == 0xA) {
        struct MenuList *linkedTask;

        ((ShopScene *)stateAddress)->substate = 0xA;
        mnuSetCommandPhase((ShopScene *)stateAddress, 6);
        linkedTask = ((ShopScene *)stateAddress)->window->list;
        linkedTask->drawCallback = func_0025ECD0;
        mnuStorePendingMenuCommandValue(linkedTask, 0);
    }
    return 1;
}

extern s32 mnuTickCommandWaitPhase(ShopScene *);
extern void func_00260590(struct MenuList *, u32);
extern void sndSetSequenceVolumePan(s32, s32, s32);
extern u8 D_0036AB10[];

s32 evtPollQuantitySelection(void *callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    s32 *dispatchSlot = &((ShopScene *)stateAddress)->dispatchState;
    s32 previousQuantity = ((ShopScene *)stateAddress)->counter;
    s32 input = mnuMapPadMaskToFlags(0xF000F3);
    s32 result = menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_POLL, callbackContext);

    if (result != 0) {
        return result;
    }
    switch (mnuTickCommandWaitPhase((ShopScene *)stateAddress)) {
    case -1:
        break;
    case 9:
        mnuSetCommandPhase((ShopScene *)stateAddress, 11);
        break;
    case 10: {
        struct MenuList *primaryTask = ((ShopScene *)stateAddress)->sprite->list;
        mnuSetPopupEntryFlagged((s32)dispatchSlot,
            (s32)(D_0036AA84 + primaryTask->cursor->index * 0x1C));
        break;
    }
    case 12:
        mnuSetPopupEntryFlagged((s32)dispatchSlot, (s32)D_0036AB10);
        break;
    case 11:
    default:
        if (((ShopScene *)stateAddress)->dispatchState == 0) {
            if (input & 1) {
                mnuSetCommandPhase((ShopScene *)stateAddress, 12);
            } else if (input & 2) {
                mnuSetCommandPhase((ShopScene *)stateAddress, 10);
                func_00260590(((ShopScene *)stateAddress)->window->list, 10);
            }
            if (input & 0x300030) {
                if (input & 0x10) {
                    mnuCampClampSceneCounter(1, (ShopScene *)stateAddress);
                } else if (input & 0x20) {
                    mnuCampClampSceneCounter(-1, (ShopScene *)stateAddress);
                }
                if (previousQuantity == ((ShopScene *)stateAddress)->counter) {
                    input &= 3;
                }
            } else if (input & 0xC0) {
                if (input & 0x80) {
                    mnuCampClampSceneCounter(10, (ShopScene *)stateAddress);
                } else if (input & 0x40) {
                    mnuCampClampSceneCounter(-10, (ShopScene *)stateAddress);
                }
                if (previousQuantity == ((ShopScene *)stateAddress)->counter) {
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

s32 evtAlignDispatchStart(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();

    func_00261760((ShopScene *)stateAddress);
    return menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

s32 evtSetupDispatchSyncF(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();

    func_0024DD78();
    return menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_00247190);

INCLUDE_ASM(const s32, "game/code_00245C98", func_002472D8);

void evtAccumulateStateScore(s32 stateAddress) {
    CampWindowParams *values = &((ShopScene *)stateAddress)->window->list->cursor->camp;

    if ((u32)(values->id - 0x60) < 0x20) {
        datGameState->world.score += values->value * ((ShopScene *)stateAddress)->counter;
    }
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_00247420);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00247588);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00247728);

s32 evtSetupDispatchSyncG(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();

    func_0024DD78();
    return menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

extern void dspStartEntry();

s32 evtPlayDispatchModeCue(void) {
    ShopScene *state = (ShopScene *)kwlnTaskGetUserValue();
    dspSetActive(1);
    switch (state->menuMode) {
    case 1:
        dspStartEntry(5);
        break;
    case 2:
        dspStartEntry(6);
        break;
    }
    return 1;
}


extern u8 D_0036AA68[];

extern void mnuSetCommandPhase(ShopScene *, u32);

s32 evtApplyDispatchModeState(void) {
    ShopScene *state = (ShopScene *)kwlnTaskGetUserValue();
    switch (state->menuMode) {
    case 1:
        mnuSetCommandPhase(state, 6);
        break;
    case 2:
        if (state->stateTable != (s32)D_0036AA68) {
            mnuSetCommandPhase(state, 5);
        }
        break;
    }
    state->menuMode = 0;
    return 1;
}

/* After idle dispatch, resume the state table at its saved position. */
s32 evtSetPopupEntryWhenMessageWindowIdle(void *callbackContext) {
    s32 stateAddress;
    s32 result;
    s32 *dispatchSlot;

    stateAddress = kwlnTaskGetUserValue();
    dispatchSlot = &((ShopScene *)stateAddress)->dispatchState;
    result = menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_POLL, callbackContext);
    if (result == 0) {
        if ((*dispatchSlot == 0) && (result = evtGetMessageWindowControlState(), result == 0)) {
            mnuSetPopupEntryFlagged((s32)dispatchSlot, ((ShopScene *)stateAddress)->stateTable);
        }
        result = 0;
    }
    return result;
}

extern void func_0025E308(s32, s32, s32, ShopScene *, s32, s32);
extern void mnuDrawStatusIconAndCompanion(s32, s32, s32, ShopScene *, s32, s32);
extern void func_0025E6B0(s32, s32, s32, ShopScene *, s32, s32);
extern void mnuDrawIconTriple(s32, s32, s32, s32, s32, s32);
extern void mnuDrawIfActive(s32, s32, s32, MenuWindowContainer *, s32);
extern void func_0025FD50(s32, s32, s32, ShopScene *, s32);
extern void mnuDrawListChildrenWithCountdown(s32, s32, s32, struct MenuList *, s32);
extern void mnuDrawIconFixedEntryWithBadge(s32, s32, s32, s32, s32, s32);
extern void mnuClearWindowPanelTransitionFlag(MenuWindowContainer *);
extern void func_00260100(ShopScene *, s32);
extern void func_00260208(s32, u32, s32, s32);

s32 func_00247A78(s32 callbackContext) {
    ShopScene *state = (ShopScene *)kwlnTaskGetUserValue();

    switch (state->menuMode) {
    case 1:
        func_0025E308(0, 0, 0, state, 0x100, 0x53);
        mnuDrawIconTriple(0, 0, 0, 0, 0x100, 0x53);
        func_0025E6B0(0, 0, 0, state, 0x100, 0x53);
        mnuDrawIfActive(0, 0, 0, state->window, 0x53);
        func_0025FD50(0, 0, 0, state, 0x53);
        func_00260100(state, 0xA09DC380);
        func_00260208((s32)state, 0x100, 3, 0x53);
        break;
    case 2:
        if (state->stateTable == (s32)D_0036AA68) {
            func_0025E308(0, 0, 0, state, 0x100, 0x53);
            mnuDrawStatusIconAndCompanion(0, 0, 0, state, 0x100, 0x53);
            mnuDrawListChildrenWithCountdown(0, 0, 0, state->sprite->list, 0x53);
            func_00260100(state, 0xA09DC380);
            func_00260208((s32)state, 0x100, 4, 0x53);
        } else {
            func_0025E308(0, 0, 0, state, 0x100, 0x53);
            mnuDrawIconTriple(0, 0, 0, 0, 0x100, 0x53);
            func_0025E6B0(0, 0, 0, state, 0x100, 0x53);
            mnuClearWindowPanelTransitionFlag(state->sprite);
            mnuDrawIconFixedEntryWithBadge(0, 0, 0, (s32)state, 0x100, 0x53);
            func_00260100(state, 0xA09DC380);
            func_00260208((s32)state, 0x100, 2, 0x53);
        }
        break;
    }
    return func_00285670(state->transitionWork, &state->dispatchState, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

s32 evtSetupDispatchSyncH(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();

    func_0024DD78();
    return menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

u32 evtStartScriptFadeAndResetDisplayFlags(void) {
    evtCreateEventScriptProcess(0x323);
    kwlnFadeOutStart(0, 0, 0, 0xf);
    evtClearActiveFlag(0);
    evtSetBoundedDisplayValue(0, 0);
    evtSetBoundedDisplayValue(1, 1);
    return 1;
}

u32 func_00247D50(void) {
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_00245C98", D_003AF528);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00247D58);

s32 evtRefreshDispatchStart(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    s32 progressTicks = ((ShopScene *)stateAddress)->progressTicks;

    if (progressTicks != 0) {
        func_0025DF68(stateAddress, progressTicks);
    }
    return menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

s32 evtSetupDispatchSyncI(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();

    func_0024DD78();
    return menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

u32 evtResetStateFlags(void) {
    evtClearActiveFlag(0);
    evtSetBoundedDisplayValue(0, 1);
    evtSetBoundedDisplayValue(1, 0);
    func_002E96D8(0x300000);
    return 1;
}

u32 evtStartFadeOut(void) {
    kwlnFadeOutStart(0, 0, 0, 0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_002481A0);

/* Select the primary entry action with opaque callback data, not a callback address. */
s32 evtDispatchStart(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();

    return menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_PRIMARY, (void *)callbackContext);
}

/* Select the secondary entry action with the same opaque callback context. */
s32 evtDispatchSync(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();

    return menuRunPanel((void *)stateAddress, EVT_DISPATCH_OPERATION_SECONDARY, (void *)callbackContext);
}

u32 func_002482B0(void) {
    return 1;
}

u32 func_002482B8(void) {
    return 1;
}

u32 func_002482C0(void) {
    return 0;
}

u32 func_002482C8(void) {
    return 0;
}

u32 func_002482D0(void) {
    return 0;
}

typedef struct FlagEntry {
    s32 firstFlag;
    s32 firstOn;
    s32 secondFlag;
    s32 secondOn;
} FlagEntry;

extern s32 sdfResourceRetainAddress(s32);
extern void mdlFlagSet(s32);

extern s32 mdlFlagTest(s32);
extern s32 sdfAllocGeneralBlock(s32);

typedef struct FlagPair {
    s32 first;
    s32 second;
} FlagPair;

typedef struct FlagSource {
    s32 first;
    s32 second;
    s32 pad[2];
} FlagSource;

extern FlagSource mnuSceneFlagEventEntries[];
extern FlagPair mnuPartyFlagEventEntries[];

/* Snapshot four primary flag pairs and sixteen extra pairs for restoration. */
s32 mnuCreateFlagEntries(void) {
    s32 snapshotHandle = sdfAllocGeneralBlock(MNU_FLAG_SNAPSHOT_BYTES);
    FlagEntry *snapshot = (FlagEntry *)sdfResourceRetainAddress(snapshotHandle);
    u32 pairIndex;

    for (pairIndex = 0; pairIndex < MNU_SCENE_FLAG_PAIR_COUNT; pairIndex++) {
        snapshot[pairIndex].firstFlag = mnuSceneFlagEventEntries[pairIndex].first;
        snapshot[pairIndex].firstOn = mdlFlagTest(snapshot[pairIndex].firstFlag);
        snapshot[pairIndex].secondFlag = mnuSceneFlagEventEntries[pairIndex].second;
        snapshot[pairIndex].secondOn = mdlFlagTest(snapshot[pairIndex].secondFlag);
    }
    for (pairIndex = 0; pairIndex < MNU_PARTY_FLAG_PAIR_COUNT; pairIndex++) {
        snapshot[MNU_SCENE_FLAG_PAIR_COUNT + pairIndex].firstFlag = mnuPartyFlagEventEntries[pairIndex].first;
        snapshot[MNU_SCENE_FLAG_PAIR_COUNT + pairIndex].firstOn = mdlFlagTest(snapshot[MNU_SCENE_FLAG_PAIR_COUNT + pairIndex].firstFlag);
        snapshot[MNU_SCENE_FLAG_PAIR_COUNT + pairIndex].secondFlag = mnuPartyFlagEventEntries[pairIndex].second;
        snapshot[MNU_SCENE_FLAG_PAIR_COUNT + pairIndex].secondOn = mdlFlagTest(snapshot[MNU_SCENE_FLAG_PAIR_COUNT + pairIndex].secondFlag);
    }
    return snapshotHandle;
}

/* Restore only those flags that were enabled in the saved snapshot. */
void mnuApplyFlagEntries(s32 snapshotHandle) {
    FlagEntry *snapshot = (FlagEntry *)sdfResourceRetainAddress(snapshotHandle);
    u32 pairIndex;

    /* Scene pairs restore only their second flag; party pairs restore both. */
    for (pairIndex = 0; pairIndex < MNU_SCENE_FLAG_PAIR_COUNT; pairIndex++) {
        if (snapshot[pairIndex].secondOn != 0) {
            mdlFlagSet(snapshot[pairIndex].secondFlag);
        }
    }
    for (pairIndex = 0; pairIndex < MNU_PARTY_FLAG_PAIR_COUNT; pairIndex++) {
        if (snapshot[MNU_SCENE_FLAG_PAIR_COUNT + pairIndex].firstOn != 0) {
            mdlFlagSet(snapshot[MNU_SCENE_FLAG_PAIR_COUNT + pairIndex].firstFlag);
        }
        if (snapshot[MNU_SCENE_FLAG_PAIR_COUNT + pairIndex].secondOn != 0) {
            mdlFlagSet(snapshot[MNU_SCENE_FLAG_PAIR_COUNT + pairIndex].secondFlag);
        }
    }
}

extern char D_003AF590[];
extern s32 D_0036AC78[];
extern u32 effLoadIndexedResource(char *, s32, s32);
extern void effResolveAndReleaseResource(u32);
extern u32 effCreateResourceSlotSet(u32, s32, s32);

/* Keep the two loaded handles and their derived slot set in the menu work array. */
void mnuLoadResourceHandles(u32 *menuWork) {
    s32 resourceIndex;

    for (resourceIndex = 0; resourceIndex < 2; resourceIndex++) {
        u32 resourceHandle = effLoadIndexedResource(D_003AF590, D_0036AC78[resourceIndex], 1);

        menuWork[0x19 + resourceIndex] = resourceHandle;
        effResolveAndReleaseResource(resourceHandle);
    }
    menuWork[0x1B] = effCreateResourceSlotSet(menuWork[0x19], 7, 1);
}

extern void effDestroyResourceSlotSet(u32);
extern void mnuReleaseEffectResource(u32);

/* Destroy the stored slot sets, then release and clear any optional effect handle. */
void mnuReleaseResourceHandles(u32 *menuWork) {
    s32 resourceIndex;

    for (resourceIndex = 0; resourceIndex < 2; resourceIndex++) {
        effDestroyResourceSlotSet(menuWork[0x19 + resourceIndex]);
    }
    effDestroyResourceSlotSet(menuWork[0x1B]);
    if (menuWork[0x56] != 0) {
        mnuReleaseEffectResource(menuWork[0x56]);
        menuWork[0x56] = 0;
    }
}

INCLUDE_RODATA(const s32, "game/code_00245C98", D_003AF570);

INCLUDE_RODATA(const s32, "game/code_00245C98", D_003AF580);

INCLUDE_RODATA(const s32, "game/code_00245C98", D_003AF590);

INCLUDE_SDATA(const s32, "game/code_00245C98", D_003BC3B0);

INCLUDE_SDATA(const s32, "game/code_00245C98", D_003BC3B8);

INCLUDE_SDATA(const s32, "game/code_00245C98", D_003BC3C0);

INCLUDE_SDATA(const s32, "game/code_00245C98", D_003BC3C8);

INCLUDE_SDATA(const s32, "game/code_00245C98", D_003BC3D0);

INCLUDE_SDATA(const s32, "game/code_00245C98", D_003BC3D8);

