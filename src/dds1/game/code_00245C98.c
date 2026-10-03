#include "common.h"
#include "kwln.h"

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

extern s64 evtGetMessageWindowControlState(void);

extern s64 func_00285670(s32, s32 *, u64, u64);

extern u32 kwlnTaskGetUserValue();

extern s32 mnuMapPadMaskToFlags(s32);
extern s32 mnuTickExtendedCommandPhase(s32);
extern void mnuSetPopupEntryFlagged(s32, s32);
extern void func_00260550(s32, u32);
extern void func_0027C788(s32);
extern void mnuRetreatWindowListSelection(s32);
extern void mnuAdvanceWindowListSelection(s32);
extern void mnuPlayInputSound(s32, s32, s32 *);
extern u8 D_0036AAF4[];

typedef struct EvtDispatchLink {
    u8 pad00[0x14];
    s32 target; /* 0x14 */
} EvtDispatchLink;

typedef struct EvtDispatchTask {
    u8 pad00[0x2C];
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
    s32 state = kwlnTaskGetUserValue();
    s32 window;
    s16 pending;

    evtClearActiveFlag(0);
    evtSetBoundedDisplayValue(0, 2);
    if (((EvtDispatchState *)state)->menuLink == 0) {
        ((EvtDispatchState *)state)->initialSelection = func_00244658(state);
        func_002444D0(state);
    }
    window = ((EvtDispatchTask *)((EvtDispatchLink *)((EvtDispatchState *)state)->menuLink)->target)->window;
    pending = mnuShopHasPendingFlag(state);
    ((EvtDispatchState *)state)->savedValue = *(s32 *)(datGameState + 0x3C);
    *(s16 *)(window + 0xE) = pending;
    ((EvtDispatchState *)state)->pendingSelection = pending;
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

void evtPrimeDispatchStart(s32 callback) {
    s32 context = kwlnTaskGetUserValue();

    func_00260670(context);
    func_00285670(context + 8, context + 0x54, 1, callback);
}

void evtSetupDispatchSync(s32 callback) {
    s32 context = kwlnTaskGetUserValue();

    func_0024DD78();
    func_00285670(context + 8, context + 0x54, 2, callback);
}

void evtInstallStateTableB(EvtDispatchState *state) {
    if (state->mode == 1) {
        state->stateTable = (s32)D_0036AA84;
        mnuSetPopupEntry((s32)&state->dispatchState, (s32)D_0036AA84 + 0xA8);
    }
}

s32 func_00246160(void) {
    EvtDispatchState *context = (EvtDispatchState *)kwlnTaskGetUserValue();

    if (context->action == 1) {
        func_002453C8(context);
    }
    return 1;
}

/* Map actions five and seven to their menu phases, then reset substate. */
s32 evtSelectStateAction(void) {
    s32 state = kwlnTaskGetUserValue();
    s32 action = ((EvtDispatchState *)state)->action;

    if (action == 5) {
        mnuSetCommandPhase(state, 3);
    } else if (action == 7) {
        s32 task;

        mnuSetCommandPhase(state, 9);
        task = ((EvtDispatchLink *)((EvtDispatchState *)state)->taskLink)->target;
        ((EvtDispatchTask *)task)->callback = (s32)func_0025F138;
        func_00260570(task, 10);
    }
    ((EvtDispatchState *)state)->substate = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_00246220);

void evtStageDispatchStart(s32 callback) {
    s32 context = kwlnTaskGetUserValue();

    func_00260AB0(context);
    func_00285670(context + 8, context + 0x54, 1, callback);
}

void evtSetupDispatchSyncB(s32 callback) {
    s32 context = kwlnTaskGetUserValue();

    func_0024DD78();
    func_00285670(context + 8, context + 0x54, 2, callback);
}

void evtInstallStateTableC(EvtDispatchState *state) {
    if (state->mode == 1) {
        state->stateTable = (s32)D_0036AAA0;
        mnuSetPopupEntry((s32)&state->dispatchState, (s32)D_0036AAA0 + 0x8C);
    }
}

s32 func_00246538(void) {
    s32 *state = (s32 *)kwlnTaskGetUserValue();

    if (state[43] == 1) {
        func_002457E8(state);
    }
    return 1;
}

s32 evtSelectStateActionB(void) {
    s32 state = kwlnTaskGetUserValue();
    s32 action = ((EvtDispatchState *)state)->action;

    if (action == 5) {
        mnuSetCommandPhase(state, 3);
    } else if (action == 7) {
        s32 task;

        mnuSetCommandPhase(state, 9);
        task = ((EvtDispatchLink *)((EvtDispatchState *)state)->taskLink)->target;
        ((EvtDispatchTask *)task)->callback = (s32)func_0025F138;
        func_00260570(task, 10);
    }
    ((EvtDispatchState *)state)->substate = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_002465F8);

void evtStageDispatchStartB(s32 callback) {
    s32 context = kwlnTaskGetUserValue();

    func_00260AB0(context);
    func_00285670(context + 8, context + 0x54, 1, callback);
}

void evtSetupDispatchSyncC(s32 callback) {
    s32 context = kwlnTaskGetUserValue();

    func_0024DD78();
    func_00285670(context + 8, context + 0x54, 2, callback);
}

void evtInstallStateTableD(EvtDispatchState *state) {
    if (state->mode == 2) {
        state->stateTable = (s32)D_0036AABC;
        mnuSetPopupEntry((s32)&state->dispatchState, (s32)D_0036AABC + 0x70);
    }
}

s32 evtEnableStateFlag(void) {
    s32 state = kwlnTaskGetUserValue();

    if ((((EvtDispatchState *)state)->action == 1) && (func_00245A40(state) == 0)) {
        ((EvtDispatchState *)state)->mode = 2;
    }
    return 1;
}

s32 evtSelectStateActionC(void) {
    s32 state = kwlnTaskGetUserValue();
    s32 action = ((EvtDispatchState *)state)->action;

    if (action == 5) {
        mnuSetCommandPhase(state, 3);
    } else if (action == 7) {
        s32 task;

        mnuSetCommandPhase(state, 9);
        task = ((EvtDispatchLink *)((EvtDispatchState *)state)->taskLink)->target;
        ((EvtDispatchTask *)task)->callback = (s32)func_0025F138;
        func_00260570(task, 10);
    }
    ((EvtDispatchState *)state)->substate = 0;
    return 1;
}

s64 func_002469F0(KwlnTask *task) {
    s64 result;
    s32 input;
    s32 context;
    s32 *dispatch;
    EvtDispatchTask *node;
    EvtDispatchTask *pending;

    context = kwlnTaskGetUserValue(task);
    input = mnuMapPadMaskToFlags(0x33);
    dispatch = &((EvtDispatchState *)context)->dispatchState;
    node = (EvtDispatchTask *)((EvtDispatchLink *)((EvtDispatchState *)context)->taskLink)->target;
    result = func_00285670(context + 8, dispatch, 0, (s32)task);
    if (result == 0) {
        switch (mnuTickExtendedCommandPhase(context)) {
        case -1:
            break;
        case 4:
            mnuSetCommandPhase(context, 6);
            mnuStorePendingMenuCommandValue((s32)node, 10);
            break;
        case 5:
            mnuSetPopupEntryFlagged((s32)dispatch, (s32)D_0036AA68);
            mnuStorePendingMenuCommandValue(((EvtDispatchLink *)((EvtDispatchState *)context)->menuLink)->target, 10);
            break;
        case 7:
            mnuSetPopupEntryFlagged((s32)dispatch, (s32)D_0036AAF4);
            break;
        case 8:
            mnuSetCommandPhase(context, 6);
            pending = (EvtDispatchTask *)((EvtDispatchLink *)((EvtDispatchState *)context)->taskLink)->target;
            pending->callback = (s32)func_0025ECD0;
            mnuStorePendingMenuCommandValue((s32)pending, 0);
            ((EvtDispatchState *)context)->substate = 10;
            break;
        case 6:
            evtInstallStateTableD((EvtDispatchState *)context);
        default:
            if (((EvtDispatchState *)context)->dispatchState == 0) {
                if (input & 1) {
                    mnuSetCommandPhase(context, 7);
                } else if (input & 2) {
                    mnuSetCommandPhase(context, 5);
                    func_00260550((s32)node, 4);
                } else if ((input & 0x300000) == 0) {
                    func_0027C788(((EvtDispatchState *)context)->taskLink);
                } else if (input & 0x10) {
                    mnuRetreatWindowListSelection(((EvtDispatchState *)context)->taskLink);
                } else if (input & 0x20) {
                    mnuAdvanceWindowListSelection(((EvtDispatchState *)context)->taskLink);
                }
            }
            mnuPlayInputSound(0, input, (s32 *)((EvtDispatchLink *)((EvtDispatchState *)context)->taskLink)->target);
            break;
        }
        return 0;
    }
    return result;
}

void evtStageDispatchStartC(s32 callback) {
    s32 context = kwlnTaskGetUserValue();

    func_00260AB0(context);
    func_00285670(context + 8, context + 0x54, 1, callback);
}

void evtSetupDispatchSyncD(s32 callback) {
    s32 context = kwlnTaskGetUserValue();

    func_0024DD78();
    func_00285670(context + 8, context + 0x54, 2, callback);
}

u32 evtResetStateProgressTimer(void) {
    s32 state;

    state = kwlnTaskGetUserValue();
    ((EvtDispatchState *)state)->progressTicks = 0;
    mnuSelectLastListNode(((EvtDispatchLink *)((EvtDispatchState *)state)->menuLink)->target);
    return 1;
}

/* Keep the dispatch query alive for 20 idle ticks before restarting its table. */
s64 evtQueryStateProgress(u64 argument) {
    s32 state = kwlnTaskGetUserValue();
    s64 result = func_00285670(state + 8, state + 0x54, 0, argument);

    if (result == 0) {
        if ((((EvtDispatchState *)state)->dispatchState == 0) && (evtGetMessageWindowControlState() == 0)) {
            s32 ticks = ((EvtDispatchState *)state)->progressTicks;

            if ((f32)ticks < 20.0f) {
                ((EvtDispatchState *)state)->progressTicks = ticks + 1;
            } else {
                mnuSetPopupEntry((s32)&((EvtDispatchState *)state)->dispatchState, (s32)D_0036AB64);
            }
        }
        result = 0;
    }
    return result;
}

void evtFetchDispatchStart(s32 callback) {
    s32 context = kwlnTaskGetUserValue();

    func_0025E108(context, ((EvtDispatchState *)context)->progressTicks);
    func_00285670(context + 8, context + 0x54, 1, callback);
}

void evtSetupDispatchSyncE(s32 callback) {
    s32 context = kwlnTaskGetUserValue();

    func_0024DD78();
    func_00285670(context + 8, context + 0x54, 2, callback);
}

s32 evtApplyBaseRateProgressStep(void) {
    s32 *state = (s32 *)kwlnTaskGetUserValue();

    state[32] = 1;
    mnuCampClampSceneCounter(-1, state);
    return 1;
}

s32 evtAdvanceStateStage(void) {
    s32 state = kwlnTaskGetUserValue();

    if (((EvtDispatchState *)state)->action == 0xA) {
        s32 task;

        ((EvtDispatchState *)state)->substate = 0xA;
        mnuSetCommandPhase(state, 6);
        task = ((EvtDispatchLink *)((EvtDispatchState *)state)->taskLink)->target;
        ((EvtDispatchTask *)task)->callback = (s32)func_0025ECD0;
        mnuStorePendingMenuCommandValue(task, 0);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_00246EA0);

void evtAlignDispatchStart(s32 callback) {
    s32 context = kwlnTaskGetUserValue();

    func_00261760(context);
    func_00285670(context + 8, context + 0x54, 1, callback);
}

void evtSetupDispatchSyncF(s32 callback) {
    s32 context = kwlnTaskGetUserValue();

    func_0024DD78();
    func_00285670(context + 8, context + 0x54, 2, callback);
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_00247190);

INCLUDE_ASM(const s32, "game/code_00245C98", func_002472D8);

void evtAccumulateStateScore(s32 state) {
    s32 score = *(s32 *)(((EvtDispatchLink *)((EvtDispatchState *)state)->taskLink)->target + 0x1C) + 0x60;

    if ((u32)(*(s32 *)(score + 4) - 0x60) < 0x20) {
        *(s32 *)(datGameState + 0xA50) += *(s32 *)score * ((EvtDispatchState *)state)->scoreFactor;
    }
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_00247420);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00247588);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00247728);

void evtSetupDispatchSyncG(s32 callback) {
    s32 context = kwlnTaskGetUserValue();

    func_0024DD78();
    func_00285670(context + 8, context + 0x54, 2, callback);
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
s64 evtSetPopupEntryWhenMessageWindowIdle(u64 argument) {
    s32 state;
    s64 result;
    s32 *dispatchState;

    state = kwlnTaskGetUserValue();
    dispatchState = (s32 *)(state + 0x54);
    result = func_00285670(state + 8, dispatchState, 0, argument);
    if (result == 0) {
        if ((*dispatchState == 0) && (result = evtGetMessageWindowControlState(), result == 0)) {
            mnuSetPopupEntryFlagged((s32)dispatchState, ((EvtDispatchState *)state)->stateTable);
        }
        result = 0;
    }
    return result;
}

extern void func_0025E308(s32, s32, s32, void *, s32, s32);
extern void func_0025E508(s32, s32, s32, void *, s32, s32);
extern void func_0025E6B0(s32, s32, s32, void *, s32, s32);
extern void mnuDrawIconTriple(s32, s32, s32, s32, s32, s32);
extern void mnuDrawIfActive(s32, s32, s32, void *, s32);
extern void func_0025FD50(s32, s32, s32, void *, s32);
extern void mnuDrawListChildrenWithCountdown(s32, s32, s32, u8 *, s32);
extern void mnuDrawIconFixedEntryWithBadge(s32, s32, s32, s32, s32, s32);
extern void mnuClearWindowPanelTransitionFlag(void *);
extern void func_00260100(void *, s32);
extern s32 func_00260208(s32, u32, s32, s32);

void func_00247A78(s32 callback) {
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
            func_0025E508(0, 0, 0, state, 0x100, 0x53);
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
    func_00285670((s32)state + 8, &state->dispatchState, 1, callback);
}

void evtSetupDispatchSyncH(s32 callback) {
    s32 context = kwlnTaskGetUserValue();

    func_0024DD78();
    func_00285670(context + 8, context + 0x54, 2, callback);
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

void evtRefreshDispatchStart(s32 callback) {
    s32 state = kwlnTaskGetUserValue();
    s32 ticks = ((EvtDispatchState *)state)->progressTicks;

    if (ticks != 0) {
        func_0025DF68(state, ticks);
    }
    func_00285670(state + 8, state + 0x54, 1, callback);
}

void evtSetupDispatchSyncI(s32 callback) {
    s32 context = kwlnTaskGetUserValue();

    func_0024DD78();
    func_00285670(context + 8, context + 0x54, 2, callback);
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

/* Register a callback for the next asynchronous dispatch. */
void evtDispatchStart(s32 callback) {
    s32 eventContext = kwlnTaskGetUserValue();

    func_00285670(eventContext + 8, eventContext + 0x54, 1, callback);
}

/* Register a callback for synchronous dispatch (mode 2). */
void evtDispatchSync(s32 callback) {
    s32 eventContext = kwlnTaskGetUserValue();

    func_00285670(eventContext + 8, eventContext + 0x54, 2, callback);
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
    s32 handle = sdfAllocGeneralBlock(0x140);
    FlagEntry *entries = (FlagEntry *)sdfResourceRetainAddress(handle);
    u32 i;

    for (i = 0; i < 4; i++) {
        entries[i].firstFlag = mnuSceneFlagEventEntries[i].first;
        entries[i].firstOn = mdlFlagTest(entries[i].firstFlag);
        entries[i].secondFlag = mnuSceneFlagEventEntries[i].second;
        entries[i].secondOn = mdlFlagTest(entries[i].secondFlag);
    }
    for (i = 0; i < 16; i++) {
        entries[4 + i].firstFlag = mnuPartyFlagEventEntries[i].first;
        entries[4 + i].firstOn = mdlFlagTest(entries[4 + i].firstFlag);
        entries[4 + i].secondFlag = mnuPartyFlagEventEntries[i].second;
        entries[4 + i].secondOn = mdlFlagTest(entries[4 + i].secondFlag);
    }
    return handle;
}

/* Restore only those flags that were enabled in the saved snapshot. */
void mnuApplyFlagEntries(s32 handle) {
    FlagEntry *entries = (FlagEntry *)sdfResourceRetainAddress(handle);
    u32 i;

    for (i = 0; i < 4; i++) {
        if (entries[i].secondOn != 0) {
            mdlFlagSet(entries[i].secondFlag);
        }
    }
    for (i = 0; i < 16; i++) {
        if (entries[4 + i].firstOn != 0) {
            mdlFlagSet(entries[4 + i].firstFlag);
        }
        if (entries[4 + i].secondOn != 0) {
            mdlFlagSet(entries[4 + i].secondFlag);
        }
    }
}

extern char D_003AF590[];
extern s32 D_0036AC78[];
extern u32 effLoadIndexedResource(char *, s32, s32);
extern void effResolveAndReleaseResource(u32);
extern u32 effCreateResourceSlotSet(u32, s32, s32);

void mnuLoadResourceHandles(u32 *work) {
    s32 i;

    for (i = 0; i < 2; i++) {
        u32 resource = effLoadIndexedResource(D_003AF590, D_0036AC78[i], 1);

        work[0x19 + i] = resource;
        effResolveAndReleaseResource(resource);
    }
    work[0x1B] = effCreateResourceSlotSet(work[0x19], 7, 1);
}

extern void effDestroyResourceSlotSet(u32);
extern void mnuReleaseEffectResource(u32);

void mnuReleaseResourceHandles(u32 *work) {
    s32 i;

    for (i = 0; i < 2; i++) {
        effDestroyResourceSlotSet(work[0x19 + i]);
    }
    effDestroyResourceSlotSet(work[0x1B]);
    if (work[0x56] != 0) {
        mnuReleaseEffectResource(work[0x56]);
        work[0x56] = 0;
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

