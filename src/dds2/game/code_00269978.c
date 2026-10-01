#include "mnu.h"

/* Transition host: two optional callbacks at +0xCC and the flag at +0xD4 that
   picks which value they are called with. */
typedef struct TransitionHost {
    u8 pad00[0xCC];
    void (*callbacks[2])(s32, struct TransitionHost *); /* 0xCC */
    u32 unkD4; /* 0xD4 */
} TransitionHost;

extern s64 evtGetMessageWindowControlState(void);

extern s32 kwlnTaskGetUserValue();

extern void func_0026C900(void);

extern void func_002686F0(s32);

typedef struct EventMenuSelection {
    u8 pad00[0x60];
    s32 entryIndex; /* 0x60 */
} EventMenuSelection;

typedef struct EventMenuOwner {
    u8 pad00[0x1C];
    EventMenuSelection *selection; /* 0x1C */
    s32 state; /* 0x20 */
} EventMenuOwner;

typedef struct EventVisualState {
    u8 pad00[0x3C];
    u32 statusFlag; /* 0x3C */
} EventVisualState;

typedef struct EventDispatchState {
    u8 pad00[8];
    u8 dispatchWork[0x4C]; /* 0x08 */
    s32 dispatchStatus; /* 0x54 */
    u32 dispatchValue; /* 0x58 */
    u8 pad5C[0x1C];
    EventVisualState *visualState; /* 0x78 */
    EventMenuOwner *thresholdOwner; /* 0x7C */
    EventMenuOwner *menuOwner; /* 0x80 */
    s32 menuMode; /* 0x84 */
    u8 pad88[8];
    s32 displayMode; /* 0x90 */
    u8 pad94[0x0C];
    s32 fadeStarted; /* 0xA0 */
    u8 padA4[0x28];
    u32 callback; /* 0xCC */
    u32 previousCallback; /* 0xD0 */
    s32 exitState; /* 0xD4 */
    s32 menuActive;      /* 0xD8: cleared when the menu command chain ends */
    s32 selectionStep;  /* 0xDC: compared against 2 by evtExitSelectionMenuAndSendSoundCommand */
    u8 padE0[0x6C];
    s32 stage;           /* 0x14C */
    u8 pad150[4];
    u32 menuResource;    /* 0x154: released by func_00342580 */
} EventDispatchState;

extern void func_00268CC0(s32, s32);

extern void evtSetBoundedDisplayValue(s32, s32);

extern void evtClearActiveFlag(s32);

extern void func_002698A0(s32, s32);

extern void func_00342580(u32);

extern s32 fldClassifyRemainingFrames(s32);

extern void func_0026A728(s32, s32);

extern void mnuSelectFirstListNode(s32);

extern void mnuSelectFirstListNode(s32);

typedef struct MenuEntry32 {
    u8 data[32];
} MenuEntry32;

extern MenuEntry32 D_003A41A8[];

extern void mnuSelectFirstListNode(s32);

extern void func_0026C918(s32, void *);

extern void dspSetActive(s32);

extern void dspStartEntry(s32);

extern void evtSetMessageWindowOptionWhenOpen(s32);

extern void evtCaptureMessageWindowSoundMode(s32);

extern void dspSetActive(s32);

extern void dspStartEntry(s32);

extern s32 func_00268C08(s32);

extern void func_00269638(void);

extern void func_00269478(s32, s32);

extern void func_00269478(s32, s32);

extern void func_00269478(s32, s32);

extern void mnuRefreshThresholdNodeFlags(s32);

extern void func_002698A0(s32, s32);

extern void func_00269978(void);

extern void func_002698A0(s32, s32);

extern void mnuHighlightProgressNodeByMode(s32);

extern void func_00342580(u32);

extern void func_00342580(u32);

extern u8 D_003CE97C[];

extern void mnuSetPopupEntry();

INCLUDE_ASM(const s32, "game/code_00269978", func_00269978);

void evtRememberDispatchCallback(u32 callback, s32 address) {
    EventDispatchState *state = (EventDispatchState *)address;
    u32 previous;

    previous = state->callback;
    state->callback = callback;
    state->previousCallback = previous;
}

/* Call each registered transition callback once; the flag at +0xD4 picks the
   index they receive. */
void mnuDispatchTransitionHostCallbacks(s32 state) {
    TransitionHost *host = (TransitionHost *)state;
    u32 i;

    for (i = 0; i < 2; i++) {
        if (host->callbacks[i] != NULL) {
            host->callbacks[i](host->unkD4 ? 1 : (s32)i, host);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00269978", func_00269B80);

u32 func_00269C48(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00269978", func_00269C50);

s64 evtDispatchSelectionAfterFieldFrameGate(s32 request) {
    s32 state = kwlnTaskGetUserValue();
    EventDispatchState *dispatchState = (EventDispatchState *)state;

    func_002686F0(state);
    func_00268838(0, state);
    if (fldClassifyRemainingFrames(state) != 2) {
        return 0;
    }
    mnuDispatchTransitionHostCallbacks(state);
    func_00268EC8(state);
    func_00268B48(state);
    return menuSetHandler((s32)dispatchState, 1, request);
}

s64 evtBSetupDispatchSync(s32 request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();

    func_0026C900();
    return menuSetHandler((s32)state, 2, request);
}

extern void mnuTerminalSetTrack(s32, s32);

extern void evtFinishMessageWindowAndNotify(void);

s32 evtClearDispatchVisualFlag(void) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();
    evtRememberDispatchCallback(0, (s32)state);
    state->visualState->statusFlag = 0;
    mnuTerminalSetTrack(0, 0);
    evtFinishMessageWindowAndNotify();
    return 1;
}

extern void func_002690A8(s32, s32);

extern void func_00269230(void);

extern void mnuTerminalSelectSlot(s32, s32, s32);

extern void mnuReleaseResourceGroup(s32);

s32 evtBeginSelectionExitFade(void) {
    s32 state = kwlnTaskGetUserValue();
    func_002690A8(1, state);
    evtRememberDispatchCallback((u32)func_00269230, state);
    mnuTerminalSelectSlot(0, -2, state);
    mnuTerminalSetTrack(1, 0);
    mnuReleaseResourceGroup(state);
    kwlnFadeOutStart(0, 0, 0, 15);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00269978", func_0026A048);

s64 evtBDispatchStart(s32 request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();

    return menuSetHandler((s32)state, 1, request);
}

s64 evtBSetupDispatchSyncB(s32 request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();

    func_0026C900();
    return menuSetHandler((s32)state, 2, request);
}

u32 evtInitializeSelectionListWhenReady(void) {
    EventDispatchState *context = (EventDispatchState *)kwlnTaskGetUserValue();

    if (context->menuActive == 0) {
        mnuSelectFirstListNode((s32)context->menuOwner);
        func_00268CC0(3, (s32)context);
        evtRememberDispatchCallback((s32)func_00269638, (s32)context);
        func_00269478(3, (s32)context);
        func_002690A8(4, (s32)context);
        mnuTerminalSelectSlot(3, 1, (s32)context);
    }
    context->menuActive = 0;
    evtClearActiveFlag(0);
    evtSetBoundedDisplayValue(0, 3);
    return 1;
}

u32 evtFinishPendingSelectionTransition(void) {
    EventDispatchState *context = (EventDispatchState *)kwlnTaskGetUserValue();

    if (context->menuActive != 0) {
        func_00268CC0(3, (s32)context);
        evtRememberDispatchCallback((s32)func_00269230, (s32)context);
        func_00269478(4, (s32)context);
        func_002690A8(3, (s32)context);
        mnuTerminalSelectSlot(3, 0, (s32)context);
        evtFinishMessageWindowAndNotify();
    }
    context->menuActive = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00269978", func_0026A2E0);

s64 func_0026A3F8(s32 request) {
    s32 state = kwlnTaskGetUserValue();
    EventDispatchState *dispatchState = (EventDispatchState *)state;
    func_002686F0(state);
    func_00268838(0, state);
    mnuDispatchTransitionHostCallbacks(state);
    func_00268EC8(state);
    func_00268B48(state);
    return menuSetHandler((s32)dispatchState, 1, request);
}

s64 evtBSetupDispatchSyncC(s32 request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();

    func_0026C900();
    return menuSetHandler((s32)state, 2, request);
}

u32 evtEnterThresholdSelectionList(void) {
    EventDispatchState *context = (EventDispatchState *)kwlnTaskGetUserValue();

    mnuRefreshThresholdNodeFlags((s32)context->thresholdOwner);
    mnuSelectFirstListNode((s32)context->thresholdOwner);
    mnuTerminalSelectSlot(3, 2, (s32)context);
    func_002698A0(3, (s32)context);
    func_002690A8(4, (s32)context);
    evtRememberDispatchCallback((s32)func_00269978, (s32)context);
    return 1;
}

u32 evtBEnterStateA(void) {
    s32 context = kwlnTaskGetUserValue();

    mnuTerminalSelectSlot(3, 0, context);
    func_002698A0(4, context);
    func_002690A8(3, context);
    evtRememberDispatchCallback((s32)func_00269230, context);
    mnuHighlightProgressNodeByMode(context);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00269978", func_0026A598);

INCLUDE_ASM(const s32, "game/code_00269978", func_0026A728);

s64 mnuInitializeSelectionDispatchWhenModeUnset(s32 request) {
    s32 state = kwlnTaskGetUserValue();
    EventDispatchState *dispatchState = (EventDispatchState *)state;
    func_002686F0(state);
    func_00268838(0, state);
    mnuDispatchTransitionHostCallbacks(state);
    func_00268EC8(state);
    func_00268B48(state);
    if (dispatchState->menuMode == 0) {
        func_0026A728(1, state);
    }
    return menuSetHandler((s32)dispatchState, 1, request);
}

s64 evtBSetupDispatchSyncD(s32 request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();

    func_0026C900();
    return menuSetHandler((s32)state, 2, request);
}

u32 evtSelectFinalVisualNode(void) {
    EventDispatchState *state;

    state = (EventDispatchState *)kwlnTaskGetUserValue();
    mnuSelectLastListNode((u32)state->visualState);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00269978", func_0026A900);

INCLUDE_ASM(const s32, "game/code_00269978", func_0026A998);

s64 evtBSetupDispatchSyncE(s32 request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();

    func_0026C900();
    return menuSetHandler((s32)state, 2, request);
}

extern void mnuReleaseCampTextureHandlesAndClearOutput(s32);

extern void mnuConfigureSelectedSceneModeEffect(s32, s32);

extern void dspCloseChannel(void);

s32 evtBReleaseImagesAndQueueMenuTransition(void) {
    s32 state = kwlnTaskGetUserValue();
    mnuReleaseCampTextureHandlesAndClearOutput(state + 0xE8);
    mnuConfigureSelectedSceneModeEffect(2, state);
    mnuTerminalSelectSlot(3, 4, state);
    func_002690A8(2, state);
    evtRememberDispatchCallback(0, state);
    *(u32 *)(state + 0xA0) = 0;
    evtFinishMessageWindowAndNotify();
    dspCloseChannel();
    return 1;
}

extern void mnuSnapshotCampTextureHandles(s32);

extern void evtCreateMessageWindowIfMissing(s32);

s32 mnuOpenTerminalSelectionMessageWindow(void) {
    s32 state = kwlnTaskGetUserValue();
    mnuSnapshotCampTextureHandles(state + 0xE8);
    mnuConfigureSelectedSceneModeEffect(1, state);
    mnuTerminalSelectSlot(3, 0, state);
    func_002690A8(1, state);
    evtRememberDispatchCallback((u32)func_00269230, state);
    evtCreateMessageWindowIfMissing(*(s32 *)(state + 0x60));
    return 1;
}

extern s32 kwlnFadeIsActive(void);

extern void mnuCreateResourceTask(void);

extern s32 mnuCheckResourceTask(void);

extern s32 sdfCheckPendingWorkWithInterrupts(void);

extern void mnuSetPopupEntryFlagged(s32 *, char *);

extern char D_003CE848[];

extern void func_002680E0(s32);

/* Dispatch completion waits for the fade and pending resource/graph work;
 * keep the request outstanding until that barrier has drained. */
s64 evtPollDispatchAfterFade(u64 request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();
    s32 *dispatch = &state->dispatchStatus;
    s64 result = func_002C4038((s32)state->dispatchWork, dispatch, 0, request);
    if (result == 0) {
        if (kwlnFadeIsActive() == 0) {
            if (state->fadeStarted == 0) {
                state->fadeStarted = 1;
                mnuCreateResourceTask();
            }
            if (*dispatch == 0 && state->fadeStarted == 1 &&
                mnuCheckResourceTask() == 0) {
                if (sdfCheckPendingWorkWithInterrupts() != 0) return 0;
                func_002680E0((s32)state);
                state->fadeStarted = 0;
                mnuSetPopupEntryFlagged(dispatch, D_003CE848);
            }
        }
        result = 0;
    }
    return result;
}

extern void func_00268838(s32, s32);

extern void mnuDispatchTransitionHostCallbacks(s32);

extern void func_00268EC8(s32);

extern void func_00268B48(s32);

s64 func_0026AC90(s32 request) {
    s32 state = kwlnTaskGetUserValue();
    EventDispatchState *dispatchState = (EventDispatchState *)state;
    func_002686F0(state);
    func_00268838(0, state);
    mnuDispatchTransitionHostCallbacks(state);
    func_00268EC8(state);
    func_00268B48(state);
    return menuSetHandler((s32)dispatchState, 1, request);
}

s64 evtBDispatchSync(s32 request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();

    return menuSetHandler((s32)state, 2, request);
}

u32 evtPrepareSelectedMenuEntry(void) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();
    EventMenuOwner *owner = state->menuOwner;
    s32 *slot = &owner->selection->entryIndex;

    if (owner->state == 1) {
        mnuSelectFirstListNode((s32)owner);
    }
    func_0026C918(0, &D_003A41A8[*slot]);
    dspSetActive(1);
    dspStartEntry(0);
    evtSetMessageWindowOptionWhenOpen(1);
    evtCaptureMessageWindowSoundMode(6);
    return 1;
}

u32 func_0026ADC0(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00269978", func_0026ADC8);

s64 func_0026AEB0(s32 request) {
    s32 state = kwlnTaskGetUserValue();
    EventDispatchState *dispatchState = (EventDispatchState *)state;
    func_002686F0(state);
    func_00268838(0, state);
    mnuDispatchTransitionHostCallbacks(state);
    func_00268EC8(state);
    func_00268B48(state);
    return menuSetHandler((s32)dispatchState, 1, request);
}

s64 evtBSetupDispatchSyncF(s32 request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();

    func_0026C900();
    return menuSetHandler((s32)state, 2, request);
}

u32 evtBCheckPanelMode(void) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();

    dspSetActive(1);
    switch (state->displayMode) {
    case 1:
        dspStartEntry(1);
        break;
    case 2:
        dspStartEntry(2);
        break;
    }
    return 1;
}

s64 evtBContinueDispatchOrRestoreTable(u64 request) {
    EventDispatchState *state;
    s64 result;
    s32 *dispatch;

    state = (EventDispatchState *)kwlnTaskGetUserValue();
    dispatch = &state->dispatchStatus;
    result = func_002C4038((s32)state->dispatchWork, dispatch, 0, request);
    if (result == 0) {
        if ((*dispatch == 0) && (result = evtGetMessageWindowControlState(), result == 0)) {
            mnuSetPopupEntry(dispatch, state->dispatchValue);
        }
        result = 0;
    }
    return result;
}

s64 func_0026B068(s32 request) {
    s32 state = kwlnTaskGetUserValue();
    EventDispatchState *dispatchState = (EventDispatchState *)state;
    func_002686F0(state);
    func_00268838(0, state);
    mnuDispatchTransitionHostCallbacks(state);
    func_00268EC8(state);
    func_00268B48(state);
    return menuSetHandler((s32)dispatchState, 1, request);
}

s64 evtBSetupDispatchSyncG(s32 request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();

    func_0026C900();
    return menuSetHandler((s32)state, 2, request);
}

u32 evtExitSelectionMenuAndSendSoundCommand(void) {
    EventDispatchState *context = (EventDispatchState *)kwlnTaskGetUserValue();

    func_00268CC0(2, (s32)context);
    if (context->selectionStep >= 2) {
        mnuTerminalSelectSlot(2, -1, (s32)context);
        func_00269478(2, (s32)context);
    } else {
        mnuTerminalSelectSlot(2, -1, (s32)context);
        func_002690A8(2, (s32)context);
    }
    evtRememberDispatchCallback(0, (s32)context);
    context->exitState = 1;
    func_00342580(context->menuResource);
    evtClearActiveFlag(0);
    return 1;
}


s32 evtBRebuildTerminalMenuAndResetDispatch(void) {
    s32 state = kwlnTaskGetUserValue();
    mnuReleaseWorkResources(state);
    mnuTerminalBuildMenus(state);
    func_00268CC0(1, state);
    mnuTerminalSelectSlot(1, 0, state);
    func_002690A8(1, state);
    evtRememberDispatchCallback((u32)func_00269230, state);
    ((EventDispatchState *)state)->stage = 1;
    ((EventDispatchState *)state)->exitState = 0;
    kwlnFadeOutStart(0, 0, 0, 15);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00269978", func_0026B260);

s64 evtBDispatchSyncD2(s32 request) {
    s32 state = kwlnTaskGetUserValue();
    EventDispatchState *dispatchState = (EventDispatchState *)state;

    func_002686F0(state);
    if (func_00268C08(state) == 0) {
        func_00268838(1, state);
    } else {
        func_00268838(0, state);
    }
    mnuDispatchTransitionHostCallbacks(state);
    if (dispatchState->stage != 3) {
        func_00268EC8(state);
    }
    func_00268B48(state);
    return menuSetHandler((s32)dispatchState, 1, request);
}

s64 evtBDispatchSyncB(s32 request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();

    return menuSetHandler((s32)state, 2, request);
}

extern void mnuTerminalFadeOrClose(s32, s32);

extern void evtClearActiveFlag(s32);

extern void evtSetBoundedDisplayValue(s32, s32);

extern void mnuStartMantraSpriteLoad(void);

s32 evtBCloseTerminalAndLoadMantraSprites(void) {
    s32 state = kwlnTaskGetUserValue();
    s32 mode;
    mnuTerminalFadeOrClose(0, state);
    evtClearActiveFlag(0);
    evtSetBoundedDisplayValue(0, 0);
    mode = ((EventDispatchState *)state)->menuMode;
    if (mode < 2) {
        if (mode >= 0) {
            mnuStartMantraSpriteLoad();
        }
    }
    return 1;
}

u32 func_0026B4A0(void) {
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_00269978", D_00424FF8);

INCLUDE_ASM(const s32, "game/code_00269978", func_0026B4A8);

s64 evtBLateDispatchStart(s32 request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();

    func_002686F0((s32)state);
    return menuSetHandler((s32)state, 1, request);
}

s64 evtBDispatchSyncC(s32 request) {
    EventDispatchState *state = (EventDispatchState *)kwlnTaskGetUserValue();

    return menuSetHandler((s32)state, 2, request);
}
