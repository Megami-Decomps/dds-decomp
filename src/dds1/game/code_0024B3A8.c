#include "common.h"

extern void evtRememberDispatchCallback(s32, s32);

extern void func_00249C08(s32);

extern void func_0024DBB0(void);

extern void func_0024A2D8(s32);

extern void func_0024DD78(void);

extern void mnuSetPopupEntry(s32, s32);

extern s64 evtGetMessageWindowControlState(void);

extern s64 func_00285670(s32, s32 *, u64, u64);

extern s32 kwlnTaskGetUserValue();

extern void func_0024A340(s32, s32);

extern void func_0024B2E0(s32);

extern void func_0024A930(s32);

extern void func_0024A610(s32);

/* Offsets shared by the event-B menu/dispatch handlers in this unit. */
typedef struct EvtBContext {
    u8 pad00[0x54];
    s32 dispatchState; /* 0x54 */
    u32 dispatchTable; /* 0x58 */
    u8 pad5C[0x14];
    u32 visualList; /* 0x70 */
    s32 thresholdList; /* 0x74 */
    s32 selectionList; /* 0x78 */
    s32 state7C;       /* 0x7C: nonzero also re-requests the effect resource */
    u8  pad80[0x8];
    s32 panelMode; /* 0x88 */
    u8 pad8C[0x40]; /* 0x98: reset flag meaning still unclear */
    s32 exitPending; /* 0xCC */
    s32 transitionPending; /* 0xD0 */
    s32 transitionStage; /* 0xD4 */
    u8 padD8[0x80];
    s32 effectHandle;   /* 0x158: effect resource handle */
    s32 dispatchMode; /* 0x15C */
    u32 resourceHandle; /* 0x160 */
} EvtBContext;

typedef struct EvtBSelectionNode {
    u8 pad00[0x60];
    s32 entryIndex; /* 0x60 */
} EvtBSelectionNode;

typedef struct EvtBSelectionList {
    u8 pad00[0x1C];
    EvtBSelectionNode *selected; /* 0x1C */
    s32 mode; /* 0x20 */
} EvtBSelectionList;

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024B3A8);

u32 func_0024B470(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024B478);

extern s32 fldClassifyRemainingFrames(s32);

s64 func_0024B6C0(u64 request) {
    s32 state = kwlnTaskGetUserValue();

    func_0024A2D8(state);
    func_0024A340(0, state);
    if (fldClassifyRemainingFrames(state) != 2) {
        return 0;
    }
    func_0024B2E0(state);
    func_0024A930(state);
    func_0024A610(state);
    return func_00285670(state + 8, (s32 *)(state + 0x54), 1, request);
}

void evtBSetupDispatchSync(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    func_0024DD78();
    func_00285670(context + 8, context + 0x54, 2, request);
}

s32 evtBClearAndReset(void) {
    s32 context = kwlnTaskGetUserValue();

    evtRememberDispatchCallback(0, context);
    *(s32 *)(((EvtBContext *)context)->visualList + 0x3C) = 0;
    func_00249C08(0);
    func_0024DBB0();
    return 1;
}

extern void func_0024AB70(s32, s32);
extern void func_0024ACD8(void);
extern void func_0024A570(s32, s32, s32);
extern void mnuReleaseVisualResources(s32);
extern void kwlnFadeOutStart(s32, s32, s32, s32);

u32 func_0024B7E8(void) {
    s32 context = kwlnTaskGetUserValue();

    func_0024AB70(1, context);
    evtRememberDispatchCallback((s32)func_0024ACD8, context);
    func_0024A570(0, -2, context);
    func_00249C08(1);
    mnuReleaseVisualResources(context);
    kwlnFadeOutStart(0, 0, 0, 15);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024B868);

void evtBDispatchStart(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    func_00285670(context + 8, context + 0x54, 1, request);
}

void evtBSetupDispatchSyncB(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    func_0024DD78();
    func_00285670(context + 8, context + 0x54, 2, request);
}

extern void mnuSelectFirstListNode(s32);
extern void func_0024A728(s32, s32);
extern void func_0024ACD8(void);
extern void func_0024AF58(void);
extern void func_0024AE18(s32, s32);
extern void func_0024AB70(s32, s32);
extern void func_0024A570(s32, s32, s32);
extern void evtClearActiveFlag(s32);

extern void evtSetBoundedDisplayValue(s32, s32);

u32 func_0024B9D8(void) {
    s32 context = kwlnTaskGetUserValue();

    if (((EvtBContext *)context)->transitionPending == 0) {
        mnuSelectFirstListNode(((EvtBContext *)context)->selectionList);
        func_0024A728(3, context);
        evtRememberDispatchCallback((s32)func_0024AF58, context);
        func_0024AE18(3, context);
        func_0024AB70(4, context);
        func_0024A570(3, 1, context);
    }
    ((EvtBContext *)context)->transitionPending = 0;
    evtClearActiveFlag(0);
    evtSetBoundedDisplayValue(0, 3);
    return 1;
}

extern void func_0024A728(s32, s32);
extern void func_0024ACD8(void);
extern void func_0024AE18(s32, s32);
extern void func_0024AB70(s32, s32);
extern void func_0024A570(s32, s32, s32);

u32 func_0024BA78(void) {
    s32 context = kwlnTaskGetUserValue();

    if (((EvtBContext *)context)->transitionPending != 0) {
        func_0024A728(3, context);
        evtRememberDispatchCallback((s32)func_0024ACD8, context);
        func_0024AE18(4, context);
        func_0024AB70(3, context);
        func_0024A570(3, 0, context);
        func_0024DBB0();
    }
    ((EvtBContext *)context)->transitionPending = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024BB00);

void func_0024BC18(s32 item) {
    s32 state = kwlnTaskGetUserValue();
    func_0024A2D8(state);
    func_0024A340(0, state);
    func_0024B2E0(state);
    func_0024A930(state);
    func_0024A610(state);
    func_00285670(state + 8, (s32 *)(state + 0x54), 1, item);
}

void evtBSetupDispatchSyncC(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    func_0024DD78();
    func_00285670(context + 8, context + 0x54, 2, request);
}

extern void mnuRefreshThresholdNodeFlags(s32);
extern void mnuSelectFirstListNode(s32);
extern void func_0024A570(s32, s32, s32);
extern void func_0024B090(s32, s32);
extern void func_0024AB70(s32, s32);
extern void func_0024B168(void);

u32 func_0024BCD0(void) {
    s32 context = kwlnTaskGetUserValue();

    mnuRefreshThresholdNodeFlags(((EvtBContext *)context)->thresholdList);
    mnuSelectFirstListNode(((EvtBContext *)context)->thresholdList);
    func_0024A570(3, 2, context);
    func_0024B090(3, context);
    func_0024AB70(4, context);
    evtRememberDispatchCallback((s32)func_0024B168, context);
    return 1;
}

extern void func_0024A570(s32, s32, s32);
extern void func_0024B090(s32, s32);
extern void func_0024AB70(s32, s32);
extern void func_0024ACD8(void);
extern void func_00249420(s32);

u32 evtBEnterStateA(void) {
    s32 context = kwlnTaskGetUserValue();

    func_0024A570(3, 0, context);
    func_0024B090(4, context);
    func_0024AB70(3, context);
    evtRememberDispatchCallback((s32)func_0024ACD8, context);
    func_00249420(context);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024BDB8);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024BF48);

extern void func_0024BF48(s32, s32);

void func_0024C028(s32 item) {
    s32 state = kwlnTaskGetUserValue();
    func_0024A2D8(state);
    func_0024A340(0, state);
    func_0024B2E0(state);
    func_0024A930(state);
    func_0024A610(state);
    if (*(s32 *)(state + 0x7C) == 0) {
        func_0024BF48(1, state);
    }
    func_00285670(state + 8, (s32 *)(state + 0x54), 1, item);
}

void evtBSetupDispatchSyncD(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    func_0024DD78();
    func_00285670(context + 8, context + 0x54, 2, request);
}

u32 evtSelectFinalVisualNode(void) {
    s32 context;

    context = kwlnTaskGetUserValue();
    mnuSelectLastListNode(((EvtBContext *)context)->visualList);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024C120);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024C1B8);

void evtBSetupDispatchSyncE(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    func_0024DD78();
    func_00285670(context + 8, context + 0x54, 2, request);
}

extern void mnuReleaseStaffImageHandles(s32);
extern void func_0024A728(s32, s32);
extern void func_0024A570(s32, s32, s32);
extern void func_0024AB70(s32, s32);
extern void func_0024DBB0(void);
extern void dspCloseChannel(void);
extern void func_002E96D8(u32);

u32 func_0024C2E0(void) {
    s32 context = kwlnTaskGetUserValue();

    mnuReleaseStaffImageHandles(context + 0xE0);
    func_0024A728(2, context);
    func_0024A570(2, -1, context);
    func_0024AB70(2, context);
    evtRememberDispatchCallback(0, context);
    ((EvtBContext *)context)->exitPending = 1;
    *(s32 *)(context + 0x98) = 0;
    func_0024DBB0();
    dspCloseChannel();
    func_002E96D8(((EvtBContext *)context)->resourceHandle);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024C368);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024C3F8);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024C578);

void evtBDispatchSync(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    func_00285670(context + 8, context + 0x54, 2, request);
}

typedef struct MenuEntry32 {
    u8 data[32];
} MenuEntry32;

extern MenuEntry32 D_00347C68[];
extern void mnuSelectFirstListNode(s32);
extern void func_0024DD90(s32, void *);
extern void dspSetActive(s32);
extern void dspStartEntry(s32);
extern void func_0024DAE8(s32);
extern void evtCaptureMessageWindowSoundMode(s32);

u32 evtPrepareSelectedMenuEntry(void) {
    s32 state = kwlnTaskGetUserValue();
    s32 owner = ((EvtBContext *)state)->selectionList;
    s32 *selectionIndex = &((EvtBSelectionList *)owner)->selected->entryIndex;

    if (((EvtBSelectionList *)owner)->mode == 1) {
        mnuSelectFirstListNode(owner);
    }
    func_0024DD90(0, &D_00347C68[*selectionIndex]);
    dspSetActive(1);
    dspStartEntry(0);
    func_0024DAE8(1);
    evtCaptureMessageWindowSoundMode(6);
    return 1;
}

u32 func_0024C6F8(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024C700);

void func_0024C7E8(s32 item) {
    s32 state = kwlnTaskGetUserValue();
    func_0024A2D8(state);
    func_0024A340(0, state);
    func_0024B2E0(state);
    func_0024A930(state);
    func_0024A610(state);
    func_00285670(state + 8, (s32 *)(state + 0x54), 1, item);
}

void evtBSetupDispatchSyncF(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    func_0024DD78();
    func_00285670(context + 8, context + 0x54, 2, request);
}

extern void dspSetActive(s32);
extern void dspStartEntry(s32);

u32 evtBCheckPanelMode(void) {
    s32 context = kwlnTaskGetUserValue();

    dspSetActive(1);
    switch (((EvtBContext *)context)->panelMode) {
    case 1:
        dspStartEntry(1);
        break;
    case 2:
        dspStartEntry(2);
        break;
    }
    return 1;
}

s64 evtBContinueDispatchOrRestoreTable(u64 input) {
    s32 context;
    s64 dispatchResult;
    s32 *dispatchState;

    context = kwlnTaskGetUserValue();
    dispatchState = &((EvtBContext *)context)->dispatchState;
    dispatchResult = func_00285670(context + 8, dispatchState, 0, input);
    if (dispatchResult == 0) {
        if ((*dispatchState == 0) && (dispatchResult = evtGetMessageWindowControlState(), dispatchResult == 0)) {
            mnuSetPopupEntry(dispatchState, ((EvtBContext *)context)->dispatchTable);
        }
        dispatchResult = 0;
    }
    return dispatchResult;
}

void func_0024C9A0(s32 item) {
    s32 state = kwlnTaskGetUserValue();
    func_0024A2D8(state);
    func_0024A340(0, state);
    func_0024B2E0(state);
    func_0024A930(state);
    func_0024A610(state);
    func_00285670(state + 8, (s32 *)(state + 0x54), 1, item);
}

void evtBSetupDispatchSyncG(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    func_0024DD78();
    func_00285670(context + 8, context + 0x54, 2, request);
}

extern void func_0024A728(s32, s32);
extern void func_0024A570(s32, s32, s32);
extern void func_0024AE18(s32, s32);
extern void func_0024AB70(s32, s32);
extern void func_002E96D8(u32);
extern void evtClearActiveFlag(s32);

u32 func_0024CA58(void) {
    s32 context = kwlnTaskGetUserValue();

    func_0024A728(2, context);
    if (((EvtBContext *)context)->transitionStage >= 2) {
        func_0024A570(2, -1, context);
        func_0024AE18(2, context);
    } else {
        func_0024A570(2, -1, context);
        func_0024AB70(2, context);
    }
    evtRememberDispatchCallback(0, context);
    ((EvtBContext *)context)->exitPending = 1;
    func_002E96D8(((EvtBContext *)context)->resourceHandle);
    evtClearActiveFlag(0);
    return 1;
}

extern void mnuReleaseWorkResources(s32);
extern void mnuTerminalBuildMenus(s32);
extern void func_0024A728(s32, s32);
extern void func_0024A570(s32, s32, s32);
extern void func_0024AB70(s32, s32);
extern void func_0024ACD8(void);

u32 func_0024CB00(void) {
    s32 context = kwlnTaskGetUserValue();

    mnuReleaseWorkResources(context);
    mnuTerminalBuildMenus(context);
    func_0024A728(1, context);
    func_0024A570(1, 0, context);
    func_0024AB70(1, context);
    evtRememberDispatchCallback((s32)func_0024ACD8, context);
    ((EvtBContext *)context)->dispatchMode = 0;
    ((EvtBContext *)context)->exitPending = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024CB80);

extern s32 func_0024A6E8(s32);

void evtBDispatchSyncD2(s32 item) {
    s32 state = kwlnTaskGetUserValue();

    func_0024A2D8(state);
    if (func_0024A6E8(state) == 0) {
        func_0024A340(1, state);
    } else {
        func_0024A340(0, state);
    }
    func_0024B2E0(state);
    if (((EvtBContext *)state)->dispatchMode != 3) {
        func_0024A930(state);
    }
    func_0024A610(state);
    func_00285670(state + 8, (s32 *)(state + 0x54), 1, item);
}

void evtBDispatchSyncB(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    func_00285670(context + 8, context + 0x54, 2, request);
}

extern void mnuFadeOrPlayCloseSfx(s32, s32);
extern s32 mnuRequestEffectResource(char *, char *);
extern void evtSetBoundedDisplayValue(s32, s32);
extern char D_003AF590[];
extern char D_003AF620[];

u32 func_0024CDB0(void) {
    EvtBContext *context = (EvtBContext *)kwlnTaskGetUserValue();

    mnuFadeOrPlayCloseSfx(0, (s32)context);
    evtClearActiveFlag(0);
    evtSetBoundedDisplayValue(0, 0);
    if (context->state7C != 0) {
        context->effectHandle = mnuRequestEffectResource(D_003AF590, D_003AF620);
    }
    return 1;
}

u32 func_0024CE20(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024CE28);

void evtBLateDispatchStart(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    func_0024A2D8(context);
    func_00285670(context + 8, context + 0x54, 1, request);
}

void evtBDispatchSyncC(s32 request) {
    s32 context = kwlnTaskGetUserValue();

    func_00285670(context + 8, context + 0x54, 2, request);
}

INCLUDE_RODATA(const s32, "game/code_0024B3A8", D_003AF710);

