#include "common.h"

extern void evtRememberDispatchCallback(s32, s32);

extern void func_00249C08(s32);

extern void func_0024DBB0(void);

extern void func_0024A2D8(s32);

extern void func_0024DD78(void);

extern void func_002858E8(s32, s32);

extern s64 func_0024DC08(void);

extern s64 func_00285670(s32, s32 *, u64, u64);

extern s32 func_00101A70();

extern void func_0024A340(s32, s32);

extern void func_0024B2E0(s32);

extern void func_0024A930(s32);

extern void func_0024A610(s32);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024B3A8);

u32 func_0024B470(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024B478);

extern s32 fldClassifyRemainingFrames(s32);

s64 func_0024B6C0(u64 request) {
    s32 state = func_00101A70();

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
    s32 context = func_00101A70();

    func_0024DD78();
    func_00285670(context + 8, context + 0x54, 2, request);
}

s32 evtBClearAndReset(void) {
    s32 context = func_00101A70();

    evtRememberDispatchCallback(0, context);
    *(s32 *)(*(s32 *)(context + 0x70) + 0x3C) = 0;
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
    s32 context = func_00101A70();

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
    s32 context = func_00101A70();

    func_00285670(context + 8, context + 0x54, 1, request);
}

void evtBSetupDispatchSyncB(s32 request) {
    s32 context = func_00101A70();

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

extern void func_0024DEF8(s32, s32);

u32 func_0024B9D8(void) {
    s32 context = func_00101A70();

    if (*(s32 *)(context + 0xD0) == 0) {
        mnuSelectFirstListNode(*(s32 *)(context + 0x78));
        func_0024A728(3, context);
        evtRememberDispatchCallback((s32)func_0024AF58, context);
        func_0024AE18(3, context);
        func_0024AB70(4, context);
        func_0024A570(3, 1, context);
    }
    *(s32 *)(context + 0xD0) = 0;
    evtClearActiveFlag(0);
    func_0024DEF8(0, 3);
    return 1;
}

extern void func_0024A728(s32, s32);
extern void func_0024ACD8(void);
extern void func_0024AE18(s32, s32);
extern void func_0024AB70(s32, s32);
extern void func_0024A570(s32, s32, s32);

u32 func_0024BA78(void) {
    s32 context = func_00101A70();

    if (*(s32 *)(context + 0xD0) != 0) {
        func_0024A728(3, context);
        evtRememberDispatchCallback((s32)func_0024ACD8, context);
        func_0024AE18(4, context);
        func_0024AB70(3, context);
        func_0024A570(3, 0, context);
        func_0024DBB0();
    }
    *(s32 *)(context + 0xD0) = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024BB00);

void func_0024BC18(s32 item) {
    s32 state = func_00101A70();
    func_0024A2D8(state);
    func_0024A340(0, state);
    func_0024B2E0(state);
    func_0024A930(state);
    func_0024A610(state);
    func_00285670(state + 8, (s32 *)(state + 0x54), 1, item);
}

void evtBSetupDispatchSyncC(s32 request) {
    s32 context = func_00101A70();

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
    s32 context = func_00101A70();

    mnuRefreshThresholdNodeFlags(*(s32 *)(context + 0x74));
    mnuSelectFirstListNode(*(s32 *)(context + 0x74));
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
    s32 context = func_00101A70();

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
    s32 state = func_00101A70();
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
    s32 context = func_00101A70();

    func_0024DD78();
    func_00285670(context + 8, context + 0x54, 2, request);
}

u32 evtSelectFinalVisualNode(void) {
    s32 context;

    context = func_00101A70();
    func_0027BB28(*(u32 *)(context + 0x70));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024C120);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024C1B8);

void evtBSetupDispatchSyncE(s32 request) {
    s32 context = func_00101A70();

    func_0024DD78();
    func_00285670(context + 8, context + 0x54, 2, request);
}

extern void mnuReleaseStaffImageHandles(s32);
extern void func_0024A728(s32, s32);
extern void func_0024A570(s32, s32, s32);
extern void func_0024AB70(s32, s32);
extern void func_0024DBB0(void);
extern void func_0024DBC8(void);
extern void func_002E96D8(u32);

u32 func_0024C2E0(void) {
    s32 context = func_00101A70();

    mnuReleaseStaffImageHandles(context + 0xE0);
    func_0024A728(2, context);
    func_0024A570(2, -1, context);
    func_0024AB70(2, context);
    evtRememberDispatchCallback(0, context);
    *(s32 *)(context + 0xCC) = 1;
    *(s32 *)(context + 0x98) = 0;
    func_0024DBB0();
    func_0024DBC8();
    func_002E96D8(*(u32 *)(context + 0x160));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024C368);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024C3F8);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024C578);

void evtBDispatchSync(s32 request) {
    s32 context = func_00101A70();

    func_00285670(context + 8, context + 0x54, 2, request);
}

typedef struct MenuEntry32 {
    u8 data[32];
} MenuEntry32;

extern MenuEntry32 D_00347C68[];
extern void mnuSelectFirstListNode(s32);
extern void func_0024DD90(s32, void *);
extern void func_0024DDC0(s32);
extern void func_0024DA58(s32);
extern void func_0024DAE8(s32);
extern void func_0024DAB8(s32);

u32 evtPrepareSelectedMenuEntry(void) {
    s32 state = func_00101A70();
    s32 owner = *(s32 *)(state + 0x78);
    s32 *selectionIndex = (s32 *)(*(s32 *)(owner + 0x1C) + 0x60);

    if (*(s32 *)(owner + 0x20) == 1) {
        mnuSelectFirstListNode(owner);
    }
    func_0024DD90(0, &D_00347C68[*selectionIndex]);
    func_0024DDC0(1);
    func_0024DA58(0);
    func_0024DAE8(1);
    func_0024DAB8(6);
    return 1;
}

u32 func_0024C6F8(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024C700);

void func_0024C7E8(s32 item) {
    s32 state = func_00101A70();
    func_0024A2D8(state);
    func_0024A340(0, state);
    func_0024B2E0(state);
    func_0024A930(state);
    func_0024A610(state);
    func_00285670(state + 8, (s32 *)(state + 0x54), 1, item);
}

void evtBSetupDispatchSyncF(s32 request) {
    s32 context = func_00101A70();

    func_0024DD78();
    func_00285670(context + 8, context + 0x54, 2, request);
}

extern void func_0024DDC0(s32);
extern void func_0024DA58(s32);

u32 evtBCheckPanelMode(void) {
    s32 context = func_00101A70();

    func_0024DDC0(1);
    switch (*(s32 *)(context + 0x88)) {
    case 1:
        func_0024DA58(1);
        break;
    case 2:
        func_0024DA58(2);
        break;
    }
    return 1;
}

s64 evtBContinueDispatchOrRestoreTable(u64 input) {
    s32 context;
    s64 dispatchResult;
    s32 *dispatchState;

    context = func_00101A70();
    dispatchState = (s32 *)(context + 0x54);
    dispatchResult = func_00285670(context + 8, dispatchState, 0, input);
    if (dispatchResult == 0) {
        if ((*dispatchState == 0) && (dispatchResult = func_0024DC08(), dispatchResult == 0)) {
            func_002858E8(dispatchState, *(u32 *)(context + 0x58));
        }
        dispatchResult = 0;
    }
    return dispatchResult;
}

void func_0024C9A0(s32 item) {
    s32 state = func_00101A70();
    func_0024A2D8(state);
    func_0024A340(0, state);
    func_0024B2E0(state);
    func_0024A930(state);
    func_0024A610(state);
    func_00285670(state + 8, (s32 *)(state + 0x54), 1, item);
}

void evtBSetupDispatchSyncG(s32 request) {
    s32 context = func_00101A70();

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
    s32 context = func_00101A70();

    func_0024A728(2, context);
    if (*(s32 *)(context + 0xD4) >= 2) {
        func_0024A570(2, -1, context);
        func_0024AE18(2, context);
    } else {
        func_0024A570(2, -1, context);
        func_0024AB70(2, context);
    }
    evtRememberDispatchCallback(0, context);
    *(s32 *)(context + 0xCC) = 1;
    func_002E96D8(*(u32 *)(context + 0x160));
    evtClearActiveFlag(0);
    return 1;
}

extern void mnuReleaseWorkResources(s32);
extern void func_00249498(s32);
extern void func_0024A728(s32, s32);
extern void func_0024A570(s32, s32, s32);
extern void func_0024AB70(s32, s32);
extern void func_0024ACD8(void);

u32 func_0024CB00(void) {
    s32 context = func_00101A70();

    mnuReleaseWorkResources(context);
    func_00249498(context);
    func_0024A728(1, context);
    func_0024A570(1, 0, context);
    func_0024AB70(1, context);
    evtRememberDispatchCallback((s32)func_0024ACD8, context);
    *(s32 *)(context + 0x15C) = 0;
    *(s32 *)(context + 0xCC) = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024CB80);

extern s32 func_0024A6E8(s32);

void evtBDispatchSyncD2(s32 item) {
    s32 state = func_00101A70();

    func_0024A2D8(state);
    if (func_0024A6E8(state) == 0) {
        func_0024A340(1, state);
    } else {
        func_0024A340(0, state);
    }
    func_0024B2E0(state);
    if (*(s32 *)(state + 0x15C) != 3) {
        func_0024A930(state);
    }
    func_0024A610(state);
    func_00285670(state + 8, (s32 *)(state + 0x54), 1, item);
}

void evtBDispatchSyncB(s32 request) {
    s32 context = func_00101A70();

    func_00285670(context + 8, context + 0x54, 2, request);
}

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024CDB0);

u32 func_0024CE20(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024CE28);

void evtBLateDispatchStart(s32 request) {
    s32 context = func_00101A70();

    func_0024A2D8(context);
    func_00285670(context + 8, context + 0x54, 1, request);
}

void evtBDispatchSyncC(s32 request) {
    s32 context = func_00101A70();

    func_00285670(context + 8, context + 0x54, 2, request);
}

INCLUDE_RODATA(const s32, "game/code_0024B3A8", D_003AF710);

