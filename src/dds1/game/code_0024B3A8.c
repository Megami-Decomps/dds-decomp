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

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024B6C0);

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

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024B7E8);

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

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024B9D8);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024BA78);

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

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024BCD0);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024BD48);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024BDB8);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024BF48);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024C028);

void evtBSetupDispatchSyncD(s32 request) {
    s32 context = func_00101A70();

    func_0024DD78();
    func_00285670(context + 8, context + 0x54, 2, request);
}

u32 func_0024C0F8(void) {
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

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024C2E0);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024C368);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024C3F8);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024C578);

void evtBDispatchSync(s32 request) {
    s32 context = func_00101A70();

    func_00285670(context + 8, context + 0x54, 2, request);
}

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024C670);

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

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024C8A0);

s64 func_0024C918(u64 input) {
    s32 context;
    s64 result;
    s32 *dispatchState;

    context = func_00101A70();
    dispatchState = (s32 *)(context + 0x54);
    result = func_00285670(context + 8, dispatchState, 0, input);
    if (result == 0) {
        if ((*dispatchState == 0) && (result = func_0024DC08(), result == 0)) {
            func_002858E8(dispatchState, *(u32 *)(context + 0x58));
        }
        result = 0;
    }
    return result;
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

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024CA58);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024CB00);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024CB80);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024CCD8);

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
