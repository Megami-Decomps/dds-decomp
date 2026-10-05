#include "common.h"
#include "kwln.h"
#include "mnu.h"

/* The dispatcher passes its last argument to entry callbacks as opaque data,
 * not as a function address. Modes select polling, primary and secondary actions. */
#define EVT_DISPATCH_OPERATION_POLL 0
#define EVT_DISPATCH_OPERATION_PRIMARY 1
#define EVT_DISPATCH_OPERATION_SECONDARY 2

#define MNU_SCENE_FLAG_PAIR_COUNT 4
#define MNU_PARTY_FLAG_PAIR_COUNT 16
#define MNU_FLAG_SNAPSHOT_BYTES 0x140

extern void func_0025DF68(s32, s32);

extern void func_00261760(s32);
extern s32 datGameState;

extern void mnuStorePendingMenuCommandValue(s32, u32);
extern void func_0025ECD0();
extern u8 D_0036AB64[];

extern void func_0025E108(s32, s32);

extern void mnuSetCommandPhase(s32, u32);
extern void func_00260570(s32, u32);
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

extern s32 func_00285670(s32, s32 *, u64, u64);

extern u32 kwlnTaskGetUserValue();

extern s32 mnuMapPadMaskToFlags(s32);
extern s32 mnuTickExtendedCommandPhase(s32);
extern void mnuSetPopupEntryFlagged(s32, s32);
extern void func_00260550(s32, u32);
extern s32 mnuCampClampSceneCounter();
extern void func_0027C788(s32);
extern void mnuRetreatWindowListSelection(s32);
extern void mnuAdvanceWindowListSelection(s32);
extern void mnuPlayInputSound(s32, s32, s32 *);
extern u8 D_0036AAF4[];

typedef struct EvtDispatchLink {
    u8 pad00[0x14];
    s32 target; /* 0x14 */
} EvtDispatchLink;

typedef struct EvtEntryRecord {
    s32 index;
    u8 pad04[0x5C];
    CampWindowParams params;
} EvtEntryRecord;

typedef struct EvtDispatchTask {
    u8 pad00[0x1C];
    EvtEntryRecord *entryRecord;
    u8 pad20[0xC];
    s32 callback; /* 0x2C */
    s32 window;   /* 0x30 */
} EvtDispatchTask;

typedef struct EvtDispatchState {
    u8 pad00[0x54];
    s32 dispatchState; /* 0x54 */
    s32 stateTable;    /* 0x58 */
    u8 pad5C[0x10];
    s32 menuLink;      /* 0x6C */
    s32 taskLink;      /* 0x70 */
    u8 pad74[0xC];
    s32 scoreFactor;   /* 0x80 */
    s32 mode;          /* 0x84 */
    u8 pad88[8];
    s16 initialSelection; /* 0x90 */
    s16 pendingSelection; /* 0x92 */
    u8 pad94[8];
    s32 savedValue;       /* 0x9C */
    u8 padA0[4];
    s32 progressTicks; /* 0xA4 */
    u8 padA8[4];
    s32 action;
    s16 substate;
} EvtDispatchState;

/* Wait until both the keyword fade and the dispatch callback are idle. */
s32 evtIsFadeDispatchIdle(void) {
    s32 fading = kwlnFadeIsActive();

    if (fading != 0) {
        return 0;
    }
    return evtGetMessageWindowControlState() == 0;
}

void evtInstallStateTable(EvtDispatchState *state) {
    if (state->mode == 2) {
        state->stateTable = (s32)D_0036AA68;
        mnuSetPopupEntry((s32)&state->dispatchState, (s32)D_0036AA68 + 0xC4);
    }
}

extern s32 func_00244658();
extern void func_002444D0();
extern s16 mnuShopHasPendingFlag();

/* Prepare the active menu state and copy the selection into its window. */
s32 evtInitializeActiveMenuState(void) {
    s32 stateAddress = kwlnTaskGetUserValue();
    s32 windowAddress;
    s16 pendingSelection;

    evtClearActiveFlag(0);
    evtSetBoundedDisplayValue(0, 2);
    if (((EvtDispatchState *)stateAddress)->menuLink == 0) {
        ((EvtDispatchState *)stateAddress)->initialSelection = func_00244658(stateAddress);
        func_002444D0(stateAddress);
    }
    windowAddress = ((EvtDispatchTask *)((EvtDispatchLink *)((EvtDispatchState *)stateAddress)->menuLink)->target)->window;
    pendingSelection = mnuShopHasPendingFlag(stateAddress);
    ((EvtDispatchState *)stateAddress)->savedValue = *(s32 *)(datGameState + 0x3C);
    *(s16 *)(windowAddress + 0xE) = pendingSelection;
    ((EvtDispatchState *)stateAddress)->pendingSelection = pendingSelection;
    return 1;
}

/* Advance command phase when the current menu state is phase one. */
s32 evtAdvancePhaseOne(void) {
    EvtDispatchState *state = (EvtDispatchState *)kwlnTaskGetUserValue();

    if (state->action == 1) {
        mnuSetCommandPhase((s32)state, 4);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_00245DE0);

s32 evtPrimeDispatchStart(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();

    func_00260670(stateAddress);
    return func_00285670(stateAddress + 8, stateAddress + 0x54, EVT_DISPATCH_OPERATION_PRIMARY, callbackContext);
}

s32 evtSetupDispatchSync(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();

    func_0024DD78();
    return func_00285670(stateAddress + 8, stateAddress + 0x54, EVT_DISPATCH_OPERATION_SECONDARY, callbackContext);
}

void evtInstallStateTableB(EvtDispatchState *state) {
    if (state->mode == 1) {
        state->stateTable = (s32)D_0036AA84;
        mnuSetPopupEntry((s32)&state->dispatchState, (s32)D_0036AA84 + 0xA8);
    }
}

s32 func_00246160(void) {
    EvtDispatchState *state = (EvtDispatchState *)kwlnTaskGetUserValue();

    if (state->action == 1) {
        func_002453C8(state);
    }
    return 1;
}

/* Map actions five and seven to their menu phases, then reset substate. */
s32 evtSelectStateAction(void) {
    s32 stateAddress = kwlnTaskGetUserValue();
    s32 action = ((EvtDispatchState *)stateAddress)->action;

    if (action == 5) {
        mnuSetCommandPhase(stateAddress, 3);
    } else if (action == 7) {
        s32 linkedTaskAddress;

        mnuSetCommandPhase(stateAddress, 9);
        linkedTaskAddress = ((EvtDispatchLink *)((EvtDispatchState *)stateAddress)->taskLink)->target;
        ((EvtDispatchTask *)linkedTaskAddress)->callback = (s32)func_0025F138;
        func_00260570(linkedTaskAddress, 10);
    }
    ((EvtDispatchState *)stateAddress)->substate = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_00246220);

s32 evtStageDispatchStart(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();

    func_00260AB0(stateAddress);
    return func_00285670(stateAddress + 8, stateAddress + 0x54, EVT_DISPATCH_OPERATION_PRIMARY, callbackContext);
}

s32 evtSetupDispatchSyncB(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();

    func_0024DD78();
    return func_00285670(stateAddress + 8, stateAddress + 0x54, EVT_DISPATCH_OPERATION_SECONDARY, callbackContext);
}

void evtInstallStateTableC(EvtDispatchState *state) {
    if (state->mode == 1) {
        state->stateTable = (s32)D_0036AAA0;
        mnuSetPopupEntry((s32)&state->dispatchState, (s32)D_0036AAA0 + 0x8C);
    }
}

s32 func_00246538(void) {
    s32 *stateWords = (s32 *)kwlnTaskGetUserValue();

    if (stateWords[43] == 1) {
        func_002457E8(stateWords);
    }
    return 1;
}

s32 evtSelectStateActionB(void) {
    s32 stateAddress = kwlnTaskGetUserValue();
    s32 action = ((EvtDispatchState *)stateAddress)->action;

    if (action == 5) {
        mnuSetCommandPhase(stateAddress, 3);
    } else if (action == 7) {
        s32 linkedTaskAddress;

        mnuSetCommandPhase(stateAddress, 9);
        linkedTaskAddress = ((EvtDispatchLink *)((EvtDispatchState *)stateAddress)->taskLink)->target;
        ((EvtDispatchTask *)linkedTaskAddress)->callback = (s32)func_0025F138;
        func_00260570(linkedTaskAddress, 10);
    }
    ((EvtDispatchState *)stateAddress)->substate = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_002465F8);

s32 evtStageDispatchStartB(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();

    func_00260AB0(stateAddress);
    return func_00285670(stateAddress + 8, stateAddress + 0x54, EVT_DISPATCH_OPERATION_PRIMARY, callbackContext);
}

s32 evtSetupDispatchSyncC(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();

    func_0024DD78();
    return func_00285670(stateAddress + 8, stateAddress + 0x54, EVT_DISPATCH_OPERATION_SECONDARY, callbackContext);
}

void evtInstallStateTableD(EvtDispatchState *state) {
    if (state->mode == 2) {
        state->stateTable = (s32)D_0036AABC;
        mnuSetPopupEntry((s32)&state->dispatchState, (s32)D_0036AABC + 0x70);
    }
}

s32 evtEnableStateFlag(void) {
    s32 stateAddress = kwlnTaskGetUserValue();

    if ((((EvtDispatchState *)stateAddress)->action == 1) && (func_00245A40(stateAddress) == 0)) {
        ((EvtDispatchState *)stateAddress)->mode = 2;
    }
    return 1;
}

s32 evtSelectStateActionC(void) {
    s32 stateAddress = kwlnTaskGetUserValue();
    s32 action = ((EvtDispatchState *)stateAddress)->action;

    if (action == 5) {
        mnuSetCommandPhase(stateAddress, 3);
    } else if (action == 7) {
        s32 linkedTaskAddress;

        mnuSetCommandPhase(stateAddress, 9);
        linkedTaskAddress = ((EvtDispatchLink *)((EvtDispatchState *)stateAddress)->taskLink)->target;
        ((EvtDispatchTask *)linkedTaskAddress)->callback = (s32)func_0025F138;
        func_00260570(linkedTaskAddress, 10);
    }
    ((EvtDispatchState *)stateAddress)->substate = 0;
    return 1;
}

/* Poll the dispatch slot, then process command phases and mapped input flags. */
s32 func_002469F0(KwlnTask *task) {
    s32 dispatchResult;
    s32 inputFlags;
    s32 stateAddress;
    s32 *dispatchSlot;
    EvtDispatchTask *linkedTask;
    EvtDispatchTask *callbackTask;

    stateAddress = kwlnTaskGetUserValue(task);
    inputFlags = mnuMapPadMaskToFlags(0x33);
    dispatchSlot = &((EvtDispatchState *)stateAddress)->dispatchState;
    linkedTask = (EvtDispatchTask *)((EvtDispatchLink *)((EvtDispatchState *)stateAddress)->taskLink)->target;
    dispatchResult = func_00285670(stateAddress + 8, dispatchSlot, EVT_DISPATCH_OPERATION_POLL, (s32)task);
    if (dispatchResult == 0) {
        switch (mnuTickExtendedCommandPhase(stateAddress)) {
        case -1:
            break;
        case 4:
            mnuSetCommandPhase(stateAddress, 6);
            mnuStorePendingMenuCommandValue((s32)linkedTask, 10);
            break;
        case 5:
            mnuSetPopupEntryFlagged((s32)dispatchSlot, (s32)D_0036AA68);
            mnuStorePendingMenuCommandValue(((EvtDispatchLink *)((EvtDispatchState *)stateAddress)->menuLink)->target, 10);
            break;
        case 7:
            mnuSetPopupEntryFlagged((s32)dispatchSlot, (s32)D_0036AAF4);
            break;
        case 8:
            mnuSetCommandPhase(stateAddress, 6);
            callbackTask = (EvtDispatchTask *)((EvtDispatchLink *)((EvtDispatchState *)stateAddress)->taskLink)->target;
            callbackTask->callback = (s32)func_0025ECD0;
            mnuStorePendingMenuCommandValue((s32)callbackTask, 0);
            ((EvtDispatchState *)stateAddress)->substate = 10;
            break;
        case 6:
            evtInstallStateTableD((EvtDispatchState *)stateAddress);
            /* Continue into the common input handling after installing the table. */
        default:
            if (((EvtDispatchState *)stateAddress)->dispatchState == 0) {
                if (inputFlags & 1) {
                    mnuSetCommandPhase(stateAddress, 7);
                } else if (inputFlags & 2) {
                    mnuSetCommandPhase(stateAddress, 5);
                    func_00260550((s32)linkedTask, 4);
                } else if ((inputFlags & 0x300000) == 0) {
                    func_0027C788(((EvtDispatchState *)stateAddress)->taskLink);
                } else if (inputFlags & 0x10) {
                    mnuRetreatWindowListSelection(((EvtDispatchState *)stateAddress)->taskLink);
                } else if (inputFlags & 0x20) {
                    mnuAdvanceWindowListSelection(((EvtDispatchState *)stateAddress)->taskLink);
                }
            }
            mnuPlayInputSound(0, inputFlags, (s32 *)((EvtDispatchLink *)((EvtDispatchState *)stateAddress)->taskLink)->target);
            break;
        }
        return 0;
    }
    return dispatchResult;
}

s32 evtStageDispatchStartC(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();

    func_00260AB0(stateAddress);
    return func_00285670(stateAddress + 8, stateAddress + 0x54, EVT_DISPATCH_OPERATION_PRIMARY, callbackContext);
}

s32 evtSetupDispatchSyncD(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();

    func_0024DD78();
    return func_00285670(stateAddress + 8, stateAddress + 0x54, EVT_DISPATCH_OPERATION_SECONDARY, callbackContext);
}

u32 evtResetStateProgressTimer(void) {
    s32 stateAddress;

    stateAddress = kwlnTaskGetUserValue();
    ((EvtDispatchState *)stateAddress)->progressTicks = 0;
    mnuSelectLastListNode(((EvtDispatchLink *)((EvtDispatchState *)stateAddress)->menuLink)->target);
    return 1;
}

/* Keep the dispatch query alive for 20 idle ticks before restarting its table. */
s32 evtQueryStateProgress(u64 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    s32 dispatchResult = func_00285670(stateAddress + 8, stateAddress + 0x54, EVT_DISPATCH_OPERATION_POLL, callbackContext);

    if (dispatchResult == 0) {
        if ((((EvtDispatchState *)stateAddress)->dispatchState == 0) && (evtGetMessageWindowControlState() == 0)) {
            s32 progressTicks = ((EvtDispatchState *)stateAddress)->progressTicks;

            if ((f32)progressTicks < 20.0f) {
                ((EvtDispatchState *)stateAddress)->progressTicks = progressTicks + 1;
            } else {
                mnuSetPopupEntry((s32)&((EvtDispatchState *)stateAddress)->dispatchState, (s32)D_0036AB64);
            }
        }
        dispatchResult = 0;
    }
    return dispatchResult;
}

s32 evtFetchDispatchStart(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();

    func_0025E108(stateAddress, ((EvtDispatchState *)stateAddress)->progressTicks);
    return func_00285670(stateAddress + 8, stateAddress + 0x54, EVT_DISPATCH_OPERATION_PRIMARY, callbackContext);
}

s32 evtSetupDispatchSyncE(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();

    func_0024DD78();
    return func_00285670(stateAddress + 8, stateAddress + 0x54, EVT_DISPATCH_OPERATION_SECONDARY, callbackContext);
}

s32 evtApplyBaseRateProgressStep(void) {
    s32 *state = (s32 *)kwlnTaskGetUserValue();

    state[32] = 1;
    mnuCampClampSceneCounter(-1, state);
    return 1;
}

s32 evtAdvanceStateStage(void) {
    s32 stateAddress = kwlnTaskGetUserValue();

    if (((EvtDispatchState *)stateAddress)->action == 0xA) {
        s32 linkedTaskAddress;

        ((EvtDispatchState *)stateAddress)->substate = 0xA;
        mnuSetCommandPhase(stateAddress, 6);
        linkedTaskAddress = ((EvtDispatchLink *)((EvtDispatchState *)stateAddress)->taskLink)->target;
        ((EvtDispatchTask *)linkedTaskAddress)->callback = (s32)func_0025ECD0;
        mnuStorePendingMenuCommandValue(linkedTaskAddress, 0);
    }
    return 1;
}

extern s32 mnuTickCommandWaitPhase();
extern void func_00260590();
extern void sndSetSequenceVolumePan(s32, s32, s32);
extern u8 D_0036AB10[];

s32 evtPollQuantitySelection(u64 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();
    s32 *dispatchSlot = &((EvtDispatchState *)stateAddress)->dispatchState;
    s32 previousQuantity = ((EvtDispatchState *)stateAddress)->scoreFactor;
    s32 input = mnuMapPadMaskToFlags(0xF000F3);
    s32 result = func_00285670(stateAddress + 8, dispatchSlot,
                             EVT_DISPATCH_OPERATION_POLL, callbackContext);

    if (result != 0) {
        return result;
    }
    switch (mnuTickCommandWaitPhase((EvtDispatchState *)stateAddress)) {
    case -1:
        break;
    case 9:
        mnuSetCommandPhase(stateAddress, 11);
        break;
    case 10: {
        EvtDispatchTask *primaryTask = (EvtDispatchTask *)
            ((EvtDispatchLink *)((EvtDispatchState *)stateAddress)->menuLink)->target;
        mnuSetPopupEntryFlagged((s32)dispatchSlot,
            (s32)(D_0036AA84 + primaryTask->entryRecord->index * 0x1C));
        break;
    }
    case 12:
        mnuSetPopupEntryFlagged((s32)dispatchSlot, (s32)D_0036AB10);
        break;
    case 11:
    default:
        if (((EvtDispatchState *)stateAddress)->dispatchState == 0) {
            if (input & 1) {
                mnuSetCommandPhase(stateAddress, 12);
            } else if (input & 2) {
                mnuSetCommandPhase(stateAddress, 10);
                func_00260590((EvtDispatchTask *)
                    ((EvtDispatchLink *)((EvtDispatchState *)stateAddress)->taskLink)->target, 10);
            }
            if (input & 0x300030) {
                if (input & 0x10) {
                    mnuCampClampSceneCounter(1, (EvtDispatchState *)stateAddress);
                } else if (input & 0x20) {
                    mnuCampClampSceneCounter(-1, (EvtDispatchState *)stateAddress);
                }
                if (previousQuantity == ((EvtDispatchState *)stateAddress)->scoreFactor) {
                    input &= 3;
                }
            } else if (input & 0xC0) {
                if (input & 0x80) {
                    mnuCampClampSceneCounter(10, (EvtDispatchState *)stateAddress);
                } else if (input & 0x40) {
                    mnuCampClampSceneCounter(-10, (EvtDispatchState *)stateAddress);
                }
                if (previousQuantity == ((EvtDispatchState *)stateAddress)->scoreFactor) {
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

    func_00261760(stateAddress);
    return func_00285670(stateAddress + 8, stateAddress + 0x54, EVT_DISPATCH_OPERATION_PRIMARY, callbackContext);
}

s32 evtSetupDispatchSyncF(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();

    func_0024DD78();
    return func_00285670(stateAddress + 8, stateAddress + 0x54, EVT_DISPATCH_OPERATION_SECONDARY, callbackContext);
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_00247190);

INCLUDE_ASM(const s32, "game/code_00245C98", func_002472D8);

void evtAccumulateStateScore(s32 stateAddress) {
    CampWindowParams *values = &((EvtDispatchTask *)
        ((EvtDispatchLink *)((EvtDispatchState *)stateAddress)->taskLink)->target)->entryRecord->params;

    if ((u32)(values->id - 0x60) < 0x20) {
        *(s32 *)(datGameState + 0xA50) += values->value * ((EvtDispatchState *)stateAddress)->scoreFactor;
    }
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_00247420);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00247588);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00247728);

s32 evtSetupDispatchSyncG(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();

    func_0024DD78();
    return func_00285670(stateAddress + 8, stateAddress + 0x54, EVT_DISPATCH_OPERATION_SECONDARY, callbackContext);
}

extern void dspStartEntry();

s32 evtPlayDispatchModeCue(void) {
    EvtDispatchState *state = (EvtDispatchState *)kwlnTaskGetUserValue();
    dspSetActive(1);
    switch (state->mode) {
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

extern void mnuSetCommandPhase(s32, u32);

s32 evtApplyDispatchModeState(void) {
    EvtDispatchState *state = (EvtDispatchState *)kwlnTaskGetUserValue();
    switch (state->mode) {
    case 1:
        mnuSetCommandPhase((s32)state, 6);
        break;
    case 2:
        if (state->stateTable != (s32)D_0036AA68) {
            mnuSetCommandPhase((s32)state, 5);
        }
        break;
    }
    state->mode = 0;
    return 1;
}

/* After idle dispatch, resume the state table at its saved position. */
s32 evtSetPopupEntryWhenMessageWindowIdle(u64 callbackContext) {
    s32 stateAddress;
    s32 result;
    s32 *dispatchSlot;

    stateAddress = kwlnTaskGetUserValue();
    dispatchSlot = (s32 *)(stateAddress + 0x54);
    result = func_00285670(stateAddress + 8, dispatchSlot, EVT_DISPATCH_OPERATION_POLL, callbackContext);
    if (result == 0) {
        if ((*dispatchSlot == 0) && (result = evtGetMessageWindowControlState(), result == 0)) {
            mnuSetPopupEntryFlagged((s32)dispatchSlot, ((EvtDispatchState *)stateAddress)->stateTable);
        }
        result = 0;
    }
    return result;
}

extern void func_0025E308(s32, s32, s32, void *, s32, s32);
extern void mnuDrawStatusIconAndCompanion(s32, s32, s32, void *, s32, s32);
extern void func_0025E6B0(s32, s32, s32, void *, s32, s32);
extern void mnuDrawIconTriple(s32, s32, s32, s32, s32, s32);
extern void mnuDrawIfActive(s32, s32, s32, void *, s32);
extern void func_0025FD50(s32, s32, s32, void *, s32);
extern void mnuDrawListChildrenWithCountdown(s32, s32, s32, u8 *, s32);
extern void mnuDrawIconFixedEntryWithBadge(s32, s32, s32, s32, s32, s32);
extern void mnuClearWindowPanelTransitionFlag(void *);
extern void func_00260100(void *, s32);
extern void func_00260208(s32, u32, s32, s32);

s32 func_00247A78(s32 callbackContext) {
    EvtDispatchState *state = (EvtDispatchState *)kwlnTaskGetUserValue();

    switch (state->mode) {
    case 1:
        func_0025E308(0, 0, 0, state, 0x100, 0x53);
        mnuDrawIconTriple(0, 0, 0, 0, 0x100, 0x53);
        func_0025E6B0(0, 0, 0, state, 0x100, 0x53);
        mnuDrawIfActive(0, 0, 0, (void *)state->taskLink, 0x53);
        func_0025FD50(0, 0, 0, state, 0x53);
        func_00260100(state, 0xA09DC380);
        func_00260208((s32)state, 0x100, 3, 0x53);
        break;
    case 2:
        if (state->stateTable == (s32)D_0036AA68) {
            func_0025E308(0, 0, 0, state, 0x100, 0x53);
            mnuDrawStatusIconAndCompanion(0, 0, 0, state, 0x100, 0x53);
            mnuDrawListChildrenWithCountdown(
                0, 0, 0, (u8 *)((EvtDispatchLink *)state->menuLink)->target, 0x53);
            func_00260100(state, 0xA09DC380);
            func_00260208((s32)state, 0x100, 4, 0x53);
        } else {
            func_0025E308(0, 0, 0, state, 0x100, 0x53);
            mnuDrawIconTriple(0, 0, 0, 0, 0x100, 0x53);
            func_0025E6B0(0, 0, 0, state, 0x100, 0x53);
            mnuClearWindowPanelTransitionFlag((void *)state->menuLink);
            mnuDrawIconFixedEntryWithBadge(0, 0, 0, (s32)state, 0x100, 0x53);
            func_00260100(state, 0xA09DC380);
            func_00260208((s32)state, 0x100, 2, 0x53);
        }
        break;
    }
    return func_00285670((s32)state + 8, &state->dispatchState, EVT_DISPATCH_OPERATION_PRIMARY, callbackContext);
}

s32 evtSetupDispatchSyncH(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();

    func_0024DD78();
    return func_00285670(stateAddress + 8, stateAddress + 0x54, EVT_DISPATCH_OPERATION_SECONDARY, callbackContext);
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
    s32 progressTicks = ((EvtDispatchState *)stateAddress)->progressTicks;

    if (progressTicks != 0) {
        func_0025DF68(stateAddress, progressTicks);
    }
    return func_00285670(stateAddress + 8, stateAddress + 0x54, EVT_DISPATCH_OPERATION_PRIMARY, callbackContext);
}

s32 evtSetupDispatchSyncI(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();

    func_0024DD78();
    return func_00285670(stateAddress + 8, stateAddress + 0x54, EVT_DISPATCH_OPERATION_SECONDARY, callbackContext);
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

    return func_00285670(stateAddress + 8, stateAddress + 0x54, EVT_DISPATCH_OPERATION_PRIMARY, callbackContext);
}

/* Select the secondary entry action with the same opaque callback context. */
s32 evtDispatchSync(s32 callbackContext) {
    s32 stateAddress = kwlnTaskGetUserValue();

    return func_00285670(stateAddress + 8, stateAddress + 0x54, EVT_DISPATCH_OPERATION_SECONDARY, callbackContext);
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

