#include "common.h"

extern void func_0025DF68(s32, s32);

extern void func_00261760(s32);
extern s32 D_003BAA00;

extern void func_00260530(s32, u32);
extern void func_0025ECD0();
extern u8 D_0036AB64[];

extern void func_0025E108(s32, s32);

extern void mnuSetCommandPhase(s32, u32);
extern void func_00260570(s32, u32);
extern void func_00260AB0(s32);
extern void func_0025F138();
extern s32 func_00245A40(s32);
extern void evtClearActiveFlag(s32);
extern s32 func_0024DEF8(s32, s32);
extern void func_002E96D8(s32);

extern void func_00260670(s32 context);

extern void func_0024DD78(void);
extern void func_002858E8(s32, s32);
extern u8 D_0036AA68[];
extern u8 D_0036AA84[];
extern u8 D_0036AAA0[];
extern u8 D_0036AABC[];

extern s32 kwlnFadeIsActive(void);

extern s64 func_0024DC08(void);

extern s64 func_00285670(s32, s32 *, u64, u64);

extern s32 func_00101A70();

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
    return func_0024DC08() == 0;
}

void evtInstallStateTable(EvtDispatchState *state) {
    if (state->mode == 2) {
        state->stateTable = (s32)D_0036AA68;
        func_002858E8((s32)&state->dispatchState, (s32)D_0036AA68 + 0xC4);
    }
}

extern s32 func_00244658();
extern void func_002444D0();
extern s16 mnuShopHasPendingFlag();

/* Prepare the active menu state and copy the selection into its window. */
s32 evtInitializeActiveMenuState(void) {
    s32 state = func_00101A70();
    s32 window;
    s16 pending;

    evtClearActiveFlag(0);
    func_0024DEF8(0, 2);
    if (((EvtDispatchState *)state)->menuLink == 0) {
        ((EvtDispatchState *)state)->initialSelection = func_00244658(state);
        func_002444D0(state);
    }
    window = ((EvtDispatchTask *)((EvtDispatchLink *)((EvtDispatchState *)state)->menuLink)->target)->window;
    pending = mnuShopHasPendingFlag(state);
    ((EvtDispatchState *)state)->savedValue = *(s32 *)(D_003BAA00 + 0x3C);
    *(s16 *)(window + 0xE) = pending;
    ((EvtDispatchState *)state)->pendingSelection = pending;
    return 1;
}

/* Advance command phase when the current menu state is phase one. */
s32 evtAdvancePhaseOne(void) {
    EvtDispatchState *state = (EvtDispatchState *)func_00101A70();

    if (state->action == 1) {
        mnuSetCommandPhase((s32)state, 4);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_00245DE0);

void evtPrimeDispatchStart(s32 callback) {
    s32 context = func_00101A70();

    func_00260670(context);
    func_00285670(context + 8, context + 0x54, 1, callback);
}

void evtSetupDispatchSync(s32 callback) {
    s32 context = func_00101A70();

    func_0024DD78();
    func_00285670(context + 8, context + 0x54, 2, callback);
}

void evtInstallStateTableB(EvtDispatchState *state) {
    if (state->mode == 1) {
        state->stateTable = (s32)D_0036AA84;
        func_002858E8((s32)&state->dispatchState, (s32)D_0036AA84 + 0xA8);
    }
}

s32 func_00246160(void) {
    EvtDispatchState *context = (EvtDispatchState *)func_00101A70();

    if (context->action == 1) {
        func_002453C8(context);
    }
    return 1;
}

/* Map actions five and seven to their menu phases, then reset substate. */
s32 evtSelectStateAction(void) {
    s32 state = func_00101A70();
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
    s32 context = func_00101A70();

    func_00260AB0(context);
    func_00285670(context + 8, context + 0x54, 1, callback);
}

void evtSetupDispatchSyncB(s32 callback) {
    s32 context = func_00101A70();

    func_0024DD78();
    func_00285670(context + 8, context + 0x54, 2, callback);
}

void evtInstallStateTableC(EvtDispatchState *state) {
    if (state->mode == 1) {
        state->stateTable = (s32)D_0036AAA0;
        func_002858E8((s32)&state->dispatchState, (s32)D_0036AAA0 + 0x8C);
    }
}

s32 func_00246538(void) {
    s32 *state = (s32 *)func_00101A70();

    if (state[43] == 1) {
        func_002457E8(state);
    }
    return 1;
}

s32 evtSelectStateActionB(void) {
    s32 state = func_00101A70();
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
    s32 context = func_00101A70();

    func_00260AB0(context);
    func_00285670(context + 8, context + 0x54, 1, callback);
}

void evtSetupDispatchSyncC(s32 callback) {
    s32 context = func_00101A70();

    func_0024DD78();
    func_00285670(context + 8, context + 0x54, 2, callback);
}

void evtInstallStateTableD(EvtDispatchState *state) {
    if (state->mode == 2) {
        state->stateTable = (s32)D_0036AABC;
        func_002858E8((s32)&state->dispatchState, (s32)D_0036AABC + 0x70);
    }
}

s32 evtEnableStateFlag(void) {
    s32 state = func_00101A70();

    if ((((EvtDispatchState *)state)->action == 1) && (func_00245A40(state) == 0)) {
        ((EvtDispatchState *)state)->mode = 2;
    }
    return 1;
}

s32 evtSelectStateActionC(void) {
    s32 state = func_00101A70();
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

INCLUDE_ASM(const s32, "game/code_00245C98", func_002469F0);

void evtStageDispatchStartC(s32 callback) {
    s32 context = func_00101A70();

    func_00260AB0(context);
    func_00285670(context + 8, context + 0x54, 1, callback);
}

void evtSetupDispatchSyncD(s32 callback) {
    s32 context = func_00101A70();

    func_0024DD78();
    func_00285670(context + 8, context + 0x54, 2, callback);
}

u32 evtResetStateProgressTimer(void) {
    s32 state;

    state = func_00101A70();
    ((EvtDispatchState *)state)->progressTicks = 0;
    mnuSelectLastListNode(((EvtDispatchLink *)((EvtDispatchState *)state)->menuLink)->target);
    return 1;
}

/* Keep the dispatch query alive for 20 idle ticks before restarting its table. */
s64 evtQueryStateProgress(u64 argument) {
    s32 state = func_00101A70();
    s64 result = func_00285670(state + 8, state + 0x54, 0, argument);

    if (result == 0) {
        if ((((EvtDispatchState *)state)->dispatchState == 0) && (func_0024DC08() == 0)) {
            s32 ticks = ((EvtDispatchState *)state)->progressTicks;

            if ((f32)ticks < 20.0f) {
                ((EvtDispatchState *)state)->progressTicks = ticks + 1;
            } else {
                func_002858E8((s32)&((EvtDispatchState *)state)->dispatchState, (s32)D_0036AB64);
            }
        }
        result = 0;
    }
    return result;
}

void evtFetchDispatchStart(s32 callback) {
    s32 context = func_00101A70();

    func_0025E108(context, ((EvtDispatchState *)context)->progressTicks);
    func_00285670(context + 8, context + 0x54, 1, callback);
}

void evtSetupDispatchSyncE(s32 callback) {
    s32 context = func_00101A70();

    func_0024DD78();
    func_00285670(context + 8, context + 0x54, 2, callback);
}

s32 func_00246E00(void) {
    s32 *state = (s32 *)func_00101A70();

    state[32] = 1;
    mnuCampClampSceneCounter(-1, state);
    return 1;
}

s32 evtAdvanceStateStage(void) {
    s32 state = func_00101A70();

    if (((EvtDispatchState *)state)->action == 0xA) {
        s32 task;

        ((EvtDispatchState *)state)->substate = 0xA;
        mnuSetCommandPhase(state, 6);
        task = ((EvtDispatchLink *)((EvtDispatchState *)state)->taskLink)->target;
        ((EvtDispatchTask *)task)->callback = (s32)func_0025ECD0;
        func_00260530(task, 0);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_00246EA0);

void evtAlignDispatchStart(s32 callback) {
    s32 context = func_00101A70();

    func_00261760(context);
    func_00285670(context + 8, context + 0x54, 1, callback);
}

void evtSetupDispatchSyncF(s32 callback) {
    s32 context = func_00101A70();

    func_0024DD78();
    func_00285670(context + 8, context + 0x54, 2, callback);
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_00247190);

INCLUDE_ASM(const s32, "game/code_00245C98", func_002472D8);

void evtAccumulateStateScore(s32 state) {
    s32 score = *(s32 *)(((EvtDispatchLink *)((EvtDispatchState *)state)->taskLink)->target + 0x1C) + 0x60;

    if ((u32)(*(s32 *)(score + 4) - 0x60) < 0x20) {
        *(s32 *)(D_003BAA00 + 0xA50) += *(s32 *)score * ((EvtDispatchState *)state)->scoreFactor;
    }
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_00247420);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00247588);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00247728);

void evtSetupDispatchSyncG(s32 callback) {
    s32 context = func_00101A70();

    func_0024DD78();
    func_00285670(context + 8, context + 0x54, 2, callback);
}

extern void func_0024DA58();

s32 evtPlayDispatchModeCue(void) {
    EvtDispatchState *state = (EvtDispatchState *)func_00101A70();
    func_0024DDC0(1);
    switch (state->mode) {
    case 1:
        func_0024DA58(5);
        break;
    case 2:
        func_0024DA58(6);
        break;
    }
    return 1;
}

extern s32 func_00101A70();

extern u8 D_0036AA68[];

extern void mnuSetCommandPhase(s32, u32);

s32 evtApplyDispatchModeState(void) {
    EvtDispatchState *state = (EvtDispatchState *)func_00101A70();
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
s64 func_002479F0(u64 argument) {
    s32 state;
    s64 result;
    s32 *dispatchState;

    state = func_00101A70();
    dispatchState = (s32 *)(state + 0x54);
    result = func_00285670(state + 8, dispatchState, 0, argument);
    if (result == 0) {
        if ((*dispatchState == 0) && (result = func_0024DC08(), result == 0)) {
            func_002858F8(dispatchState, ((EvtDispatchState *)state)->stateTable);
        }
        result = 0;
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_00245C98", func_00247A78);

void evtSetupDispatchSyncH(s32 callback) {
    s32 context = func_00101A70();

    func_0024DD78();
    func_00285670(context + 8, context + 0x54, 2, callback);
}

u32 func_00247CF8(void) {
    func_00220110(0x323);
    kwlnFadeOutStart(0, 0, 0, 0xf);
    evtClearActiveFlag(0);
    func_0024DEF8(0, 0);
    func_0024DEF8(1, 1);
    return 1;
}

u32 func_00247D50(void) {
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_00245C98", D_003AF528);

INCLUDE_ASM(const s32, "game/code_00245C98", func_00247D58);

void evtRefreshDispatchStart(s32 callback) {
    s32 state = func_00101A70();
    s32 ticks = ((EvtDispatchState *)state)->progressTicks;

    if (ticks != 0) {
        func_0025DF68(state, ticks);
    }
    func_00285670(state + 8, state + 0x54, 1, callback);
}

void evtSetupDispatchSyncI(s32 callback) {
    s32 context = func_00101A70();

    func_0024DD78();
    func_00285670(context + 8, context + 0x54, 2, callback);
}

u32 evtResetStateFlags(void) {
    evtClearActiveFlag(0);
    func_0024DEF8(0, 1);
    func_0024DEF8(1, 0);
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
    s32 eventContext = func_00101A70();

    func_00285670(eventContext + 8, eventContext + 0x54, 1, callback);
}

/* Register a callback for synchronous dispatch (mode 2). */
void evtDispatchSync(s32 callback) {
    s32 eventContext = func_00101A70();

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
extern s32 func_002D03F8(s32);

typedef struct FlagPair {
    s32 first;
    s32 second;
} FlagPair;

typedef struct FlagSource {
    s32 first;
    s32 second;
    s32 pad[2];
} FlagSource;

extern FlagSource D_0036ABB8[];
extern FlagPair D_0036ABF8[];

/* Snapshot four primary flag pairs and sixteen extra pairs for restoration. */
s32 mnuCreateFlagEntries(void) {
    s32 handle = func_002D03F8(0x140);
    FlagEntry *entries = (FlagEntry *)sdfResourceRetainAddress(handle);
    u32 i;

    for (i = 0; i < 4; i++) {
        entries[i].firstFlag = D_0036ABB8[i].first;
        entries[i].firstOn = mdlFlagTest(entries[i].firstFlag);
        entries[i].secondFlag = D_0036ABB8[i].second;
        entries[i].secondOn = mdlFlagTest(entries[i].secondFlag);
    }
    for (i = 0; i < 16; i++) {
        entries[4 + i].firstFlag = D_0036ABF8[i].first;
        entries[4 + i].firstOn = mdlFlagTest(entries[4 + i].firstFlag);
        entries[4 + i].secondFlag = D_0036ABF8[i].second;
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

extern void func_002BDD60(u32);
extern void mnuReleaseEffectResource(u32);

void mnuReleaseResourceHandles(u32 *work) {
    s32 i;

    for (i = 0; i < 2; i++) {
        func_002BDD60(work[0x19 + i]);
    }
    func_002BDD60(work[0x1B]);
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

