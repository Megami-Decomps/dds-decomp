#include "common.h"

extern s64 func_0026C768(void);

extern s64 func_002C4038(s32, s32 *, u64, u64);

extern void func_002C42B0(s32, s32);

extern s32 func_00101958();

extern void func_0026C900(void);

extern void func_002686F0(s32);

typedef struct EventDispatchState {
    u8 pad0[0xCC];
    u32 callback;
    u32 previousCallback;
} EventDispatchState;

INCLUDE_ASM(const s32, "game/code_00269978", func_00269978);

void evtRememberDispatchCallback(u32 callback, s32 address) {
    EventDispatchState *state = (EventDispatchState *)address;
    u32 previous;

    previous = state->callback;
    state->callback = callback;
    state->previousCallback = previous;
}

INCLUDE_ASM(const s32, "game/code_00269978", func_00269B08);

INCLUDE_ASM(const s32, "game/code_00269978", func_00269B80);

u32 func_00269C48(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00269978", func_00269C50);

INCLUDE_ASM(const s32, "game/code_00269978", func_00269E98);

void evtBSetupDispatchSync(s32 request) {
    s32 state = func_00101958();

    func_0026C900();
    func_002C4038(state + 8, state + 0x54, 2, request);
}

extern void func_00267EA0(s32, s32);
extern void func_0026C710(void);

s32 func_00269F70(void) {
    u8 *state = (u8 *)func_00101958();
    evtRememberDispatchCallback(0, (s32)state);
    *(u32 *)(*(s32 *)(state + 0x78) + 0x3C) = 0;
    func_00267EA0(0, 0);
    func_0026C710();
    return 1;
}

extern void func_002690A8(s32, s32);
extern void func_00269230(void);
extern void func_00268AA0(s32, s32, s32);
extern void func_002668C0(s32);

s32 func_00269FC8(void) {
    s32 state = func_00101958();
    func_002690A8(1, state);
    evtRememberDispatchCallback((u32)func_00269230, state);
    func_00268AA0(0, -2, state);
    func_00267EA0(1, 0);
    func_002668C0(state);
    kwlnFadeOutStart(0, 0, 0, 15);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00269978", func_0026A048);

void evtBDispatchStart(s32 request) {
    s32 state = func_00101958();

    func_002C4038(state + 8, state + 0x54, 1, request);
}

void evtBSetupDispatchSyncB(s32 request) {
    s32 state = func_00101958();

    func_0026C900();
    func_002C4038(state + 8, state + 0x54, 2, request);
}

INCLUDE_ASM(const s32, "game/code_00269978", func_0026A1B8);

INCLUDE_ASM(const s32, "game/code_00269978", func_0026A258);

INCLUDE_ASM(const s32, "game/code_00269978", func_0026A2E0);

void func_0026A3F8(s32 item) {
    s32 state = func_00101958();
    func_002686F0(state);
    func_00268838(0, state);
    func_00269B08(state);
    func_00268EC8(state);
    func_00268B48(state);
    func_002C4038(state + 8, (s32 *)(state + 0x54), 1, item);
}

void evtBSetupDispatchSyncC(s32 request) {
    s32 state = func_00101958();

    func_0026C900();
    func_002C4038(state + 8, state + 0x54, 2, request);
}

INCLUDE_ASM(const s32, "game/code_00269978", func_0026A4B0);

INCLUDE_ASM(const s32, "game/code_00269978", func_0026A528);

INCLUDE_ASM(const s32, "game/code_00269978", func_0026A598);

INCLUDE_ASM(const s32, "game/code_00269978", func_0026A728);

INCLUDE_ASM(const s32, "game/code_00269978", func_0026A808);

void evtBSetupDispatchSyncD(s32 request) {
    s32 state = func_00101958();

    func_0026C900();
    func_002C4038(state + 8, state + 0x54, 2, request);
}

u32 func_0026A8D8(void) {
    s32 temp_v0;

    temp_v0 = func_00101958();
    func_002B8988(*(u32 *)(temp_v0 + 0x78));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00269978", func_0026A900);

INCLUDE_ASM(const s32, "game/code_00269978", func_0026A998);

void evtBSetupDispatchSyncE(s32 request) {
    s32 state = func_00101958();

    func_0026C900();
    func_002C4038(state + 8, state + 0x54, 2, request);
}

extern void func_002A9200(s32);
extern void func_00268C48(s32, s32);
extern void func_0026C728(void);

s32 func_0026AAC0(void) {
    s32 state = func_00101958();
    func_002A9200(state + 0xE8);
    func_00268C48(2, state);
    func_00268AA0(3, 4, state);
    func_002690A8(2, state);
    evtRememberDispatchCallback(0, state);
    *(u32 *)(state + 0xA0) = 0;
    func_0026C710();
    func_0026C728();
    return 1;
}

extern void func_002A91A0(s32);
extern void func_0026C538(s32);

s32 func_0026AB38(void) {
    s32 state = func_00101958();
    func_002A91A0(state + 0xE8);
    func_00268C48(1, state);
    func_00268AA0(3, 0, state);
    func_002690A8(1, state);
    evtRememberDispatchCallback((u32)func_00269230, state);
    func_0026C538(*(s32 *)(state + 0x60));
    return 1;
}

extern s32 kwlnFadeIsActive(void);
extern void func_00286F90(void);
extern s32 func_00286FD8(void);
extern s32 func_0032CD98(void);
extern void func_002C42C0(s32 *, char *);
extern char D_003CE848[];

extern void func_002680E0(s32);

s64 func_0026ABB0(u64 request) {
    s32 state = func_00101958();
    s32 *dispatch = (s32 *)(state + 0x54);
    s64 result = func_002C4038(state + 8, dispatch, 0, request);
    if (result == 0) {
        if (kwlnFadeIsActive() == 0) {
            if (*(s32 *)(state + 0xA0) == 0) {
                *(s32 *)(state + 0xA0) = 1;
                func_00286F90();
            }
            if (*dispatch == 0 && *(s32 *)(state + 0xA0) == 1 &&
                func_00286FD8() == 0) {
                if (func_0032CD98() != 0) return 0;
                func_002680E0(state);
                *(s32 *)(state + 0xA0) = 0;
                func_002C42C0(dispatch, D_003CE848);
            }
        }
        result = 0;
    }
    return result;
}

extern void func_00268838(s32, s32);
extern void func_00269B08(s32);
extern void func_00268EC8(s32);
extern void func_00268B48(s32);

void func_0026AC90(s32 item) {
    s32 state = func_00101958();
    func_002686F0(state);
    func_00268838(0, state);
    func_00269B08(state);
    func_00268EC8(state);
    func_00268B48(state);
    func_002C4038(state + 8, (s32 *)(state + 0x54), 1, item);
}

void evtBDispatchSync(s32 arg0) {
    s32 temp_v0 = func_00101958();

    func_002C4038(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}

INCLUDE_ASM(const s32, "game/code_00269978", func_0026AD38);

u32 func_0026ADC0(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00269978", func_0026ADC8);

void func_0026AEB0(s32 item) {
    s32 state = func_00101958();
    func_002686F0(state);
    func_00268838(0, state);
    func_00269B08(state);
    func_00268EC8(state);
    func_00268B48(state);
    func_002C4038(state + 8, (s32 *)(state + 0x54), 1, item);
}

void evtBSetupDispatchSyncF(s32 arg0) {
    s32 temp_v0 = func_00101958();

    func_0026C900();
    func_002C4038(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}

INCLUDE_ASM(const s32, "game/code_00269978", func_0026AF68);

s64 func_0026AFE0(u64 arg0) {
    s32 temp_v0;
    s64 temp_v1;
    s32 *piVar3;

    temp_v0 = func_00101958();
    piVar3 = (s32 *)(temp_v0 + 0x54);
    temp_v1 = func_002C4038(temp_v0 + 8, piVar3, 0, arg0);
    if (temp_v1 == 0) {
        if ((*piVar3 == 0) && (temp_v1 = func_0026C768(), temp_v1 == 0)) {
            func_002C42B0(piVar3, *(u32 *)(temp_v0 + 0x58));
        }
        temp_v1 = 0;
    }
    return temp_v1;
}

void func_0026B068(s32 item) {
    s32 state = func_00101958();
    func_002686F0(state);
    func_00268838(0, state);
    func_00269B08(state);
    func_00268EC8(state);
    func_00268B48(state);
    func_002C4038(state + 8, (s32 *)(state + 0x54), 1, item);
}

void evtBSetupDispatchSyncG(s32 arg0) {
    s32 temp_v0 = func_00101958();

    func_0026C900();
    func_002C4038(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}

INCLUDE_ASM(const s32, "game/code_00269978", func_0026B120);

extern void func_00268CC0(s32, s32);

s32 func_0026B1C8(void) {
    s32 state = func_00101958();
    func_002678C8(state);
    func_00267768(state);
    func_00268CC0(1, state);
    func_00268AA0(1, 0, state);
    func_002690A8(1, state);
    evtRememberDispatchCallback((u32)func_00269230, state);
    *(s32 *)(state + 0x14C) = 1;
    *(s32 *)(state + 0xD4) = 0;
    kwlnFadeOutStart(0, 0, 0, 15);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00269978", func_0026B260);

INCLUDE_ASM(const s32, "game/code_00269978", func_0026B358);

void evtBDispatchSyncB(s32 arg0) {
    s32 temp_v0 = func_00101958();

    func_002C4038(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}

extern void func_00267938(s32, s32);
extern void func_0026CA60(s32);
extern void func_0026CA80(s32, s32);
extern void func_0026E5F8(void);

s32 func_0026B430(void) {
    s32 state = func_00101958();
    s32 mode;
    func_00267938(0, state);
    func_0026CA60(0);
    func_0026CA80(0, 0);
    mode = *(s32 *)(state + 0x84);
    if (mode < 2) {
        if (mode >= 0) {
            func_0026E5F8();
        }
    }
    return 1;
}

u32 func_0026B4A0(void) {
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_00269978", D_00424FF8);

INCLUDE_ASM(const s32, "game/code_00269978", func_0026B4A8);

void evtBLateDispatchStart(s32 arg0) {
    s32 temp_v0 = func_00101958();

    func_002686F0(temp_v0);
    func_002C4038(temp_v0 + 8, temp_v0 + 0x54, 1, arg0);
}

void evtBDispatchSyncC(s32 arg0) {
    s32 temp_v0 = func_00101958();

    func_002C4038(temp_v0 + 8, temp_v0 + 0x54, 2, arg0);
}
