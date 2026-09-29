#include "common.h"

extern s64 func_0026C768(void);

extern s64 func_002C4038(s32, s32 *, u64, u64);

extern void func_002C42B0(s32, s32);

extern s32 func_00101958();

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
    u8 pad7C[4];
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
    u8 padD8[0x74];
    s32 stage; /* 0x14C */
} EventDispatchState;

extern s32 fldClassifyRemainingFrames(s32);

extern void func_0026A728(s32, s32);

extern void func_002B8968(s32);

extern void func_002B8968(s32);

typedef struct MenuEntry32 {
    u8 data[32];
} MenuEntry32;

extern MenuEntry32 D_003A41A8[];

extern void func_002B8968(s32);

extern void func_0026C918(s32, void *);

extern void func_0026C948(s32);

extern void func_0026C5B8(s32);

extern void func_0026C648(s32);

extern void func_0026C618(s32);

extern void func_0026C948(s32);

extern void func_0026C5B8(s32);

extern s32 func_00268C08(s32);

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

s64 func_00269E98(u64 item) {
    s32 state = func_00101958();
    EventDispatchState *dispatchState = (EventDispatchState *)state;

    func_002686F0(state);
    func_00268838(0, state);
    if (fldClassifyRemainingFrames(state) != 2) {
        return 0;
    }
    func_00269B08(state);
    func_00268EC8(state);
    func_00268B48(state);
    return func_002C4038((s32)dispatchState->dispatchWork,
                         &dispatchState->dispatchStatus, 1, item);
}

void evtBSetupDispatchSync(s32 request) {
    EventDispatchState *state = (EventDispatchState *)func_00101958();

    func_0026C900();
    func_002C4038((s32)state->dispatchWork, &state->dispatchStatus, 2, request);
}

extern void func_00267EA0(s32, s32);

extern void func_0026C710(void);

s32 func_00269F70(void) {
    EventDispatchState *state = (EventDispatchState *)func_00101958();
    evtRememberDispatchCallback(0, (s32)state);
    state->visualState->statusFlag = 0;
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
    EventDispatchState *state = (EventDispatchState *)func_00101958();

    func_002C4038((s32)state->dispatchWork, &state->dispatchStatus, 1, request);
}

void evtBSetupDispatchSyncB(s32 request) {
    EventDispatchState *state = (EventDispatchState *)func_00101958();

    func_0026C900();
    func_002C4038((s32)state->dispatchWork, &state->dispatchStatus, 2, request);
}

INCLUDE_ASM(const s32, "game/code_00269978", func_0026A1B8);

INCLUDE_ASM(const s32, "game/code_00269978", func_0026A258);

INCLUDE_ASM(const s32, "game/code_00269978", func_0026A2E0);

void func_0026A3F8(s32 item) {
    s32 state = func_00101958();
    EventDispatchState *dispatchState = (EventDispatchState *)state;
    func_002686F0(state);
    func_00268838(0, state);
    func_00269B08(state);
    func_00268EC8(state);
    func_00268B48(state);
    func_002C4038((s32)dispatchState->dispatchWork,
                  &dispatchState->dispatchStatus, 1, item);
}

void evtBSetupDispatchSyncC(s32 request) {
    EventDispatchState *state = (EventDispatchState *)func_00101958();

    func_0026C900();
    func_002C4038((s32)state->dispatchWork, &state->dispatchStatus, 2, request);
}

INCLUDE_ASM(const s32, "game/code_00269978", func_0026A4B0);

INCLUDE_ASM(const s32, "game/code_00269978", func_0026A528);

INCLUDE_ASM(const s32, "game/code_00269978", func_0026A598);

INCLUDE_ASM(const s32, "game/code_00269978", func_0026A728);

void func_0026A808(s32 item) {
    s32 state = func_00101958();
    EventDispatchState *dispatchState = (EventDispatchState *)state;
    func_002686F0(state);
    func_00268838(0, state);
    func_00269B08(state);
    func_00268EC8(state);
    func_00268B48(state);
    if (dispatchState->menuMode == 0) {
        func_0026A728(1, state);
    }
    func_002C4038((s32)dispatchState->dispatchWork,
                  &dispatchState->dispatchStatus, 1, item);
}

void evtBSetupDispatchSyncD(s32 request) {
    EventDispatchState *state = (EventDispatchState *)func_00101958();

    func_0026C900();
    func_002C4038((s32)state->dispatchWork, &state->dispatchStatus, 2, request);
}

u32 func_0026A8D8(void) {
    EventDispatchState *state;

    state = (EventDispatchState *)func_00101958();
    func_002B8988((u32)state->visualState);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00269978", func_0026A900);

INCLUDE_ASM(const s32, "game/code_00269978", func_0026A998);

void evtBSetupDispatchSyncE(s32 request) {
    EventDispatchState *state = (EventDispatchState *)func_00101958();

    func_0026C900();
    func_002C4038((s32)state->dispatchWork, &state->dispatchStatus, 2, request);
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
    EventDispatchState *state = (EventDispatchState *)func_00101958();
    s32 *dispatch = &state->dispatchStatus;
    s64 result = func_002C4038((s32)state->dispatchWork, dispatch, 0, request);
    if (result == 0) {
        if (kwlnFadeIsActive() == 0) {
            if (state->fadeStarted == 0) {
                state->fadeStarted = 1;
                func_00286F90();
            }
            if (*dispatch == 0 && state->fadeStarted == 1 &&
                func_00286FD8() == 0) {
                if (func_0032CD98() != 0) return 0;
                func_002680E0((s32)state);
                state->fadeStarted = 0;
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
    EventDispatchState *dispatchState = (EventDispatchState *)state;
    func_002686F0(state);
    func_00268838(0, state);
    func_00269B08(state);
    func_00268EC8(state);
    func_00268B48(state);
    func_002C4038((s32)dispatchState->dispatchWork,
                  &dispatchState->dispatchStatus, 1, item);
}

void evtBDispatchSync(s32 request) {
    EventDispatchState *state = (EventDispatchState *)func_00101958();

    func_002C4038((s32)state->dispatchWork, &state->dispatchStatus, 2, request);
}

u32 func_0026AD38(void) {
    EventDispatchState *state = (EventDispatchState *)func_00101958();
    EventMenuOwner *owner = state->menuOwner;
    s32 *slot = &owner->selection->entryIndex;

    if (owner->state == 1) {
        func_002B8968((s32)owner);
    }
    func_0026C918(0, &D_003A41A8[*slot]);
    func_0026C948(1);
    func_0026C5B8(0);
    func_0026C648(1);
    func_0026C618(6);
    return 1;
}

u32 func_0026ADC0(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00269978", func_0026ADC8);

void func_0026AEB0(s32 item) {
    s32 state = func_00101958();
    EventDispatchState *dispatchState = (EventDispatchState *)state;
    func_002686F0(state);
    func_00268838(0, state);
    func_00269B08(state);
    func_00268EC8(state);
    func_00268B48(state);
    func_002C4038((s32)dispatchState->dispatchWork,
                  &dispatchState->dispatchStatus, 1, item);
}

void evtBSetupDispatchSyncF(s32 request) {
    EventDispatchState *state = (EventDispatchState *)func_00101958();

    func_0026C900();
    func_002C4038((s32)state->dispatchWork, &state->dispatchStatus, 2, request);
}

u32 func_0026AF68(void) {
    EventDispatchState *state = (EventDispatchState *)func_00101958();

    func_0026C948(1);
    switch (state->displayMode) {
    case 1:
        func_0026C5B8(1);
        break;
    case 2:
        func_0026C5B8(2);
        break;
    }
    return 1;
}

s64 func_0026AFE0(u64 request) {
    EventDispatchState *state;
    s64 result;
    s32 *dispatch;

    state = (EventDispatchState *)func_00101958();
    dispatch = &state->dispatchStatus;
    result = func_002C4038((s32)state->dispatchWork, dispatch, 0, request);
    if (result == 0) {
        if ((*dispatch == 0) && (result = func_0026C768(), result == 0)) {
            func_002C42B0(dispatch, state->dispatchValue);
        }
        result = 0;
    }
    return result;
}

void func_0026B068(s32 item) {
    s32 state = func_00101958();
    EventDispatchState *dispatchState = (EventDispatchState *)state;
    func_002686F0(state);
    func_00268838(0, state);
    func_00269B08(state);
    func_00268EC8(state);
    func_00268B48(state);
    func_002C4038((s32)dispatchState->dispatchWork,
                  &dispatchState->dispatchStatus, 1, item);
}

void evtBSetupDispatchSyncG(s32 request) {
    EventDispatchState *state = (EventDispatchState *)func_00101958();

    func_0026C900();
    func_002C4038((s32)state->dispatchWork, &state->dispatchStatus, 2, request);
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
    ((EventDispatchState *)state)->stage = 1;
    ((EventDispatchState *)state)->exitState = 0;
    kwlnFadeOutStart(0, 0, 0, 15);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00269978", func_0026B260);

void func_0026B358(s32 item) {
    s32 state = func_00101958();
    EventDispatchState *dispatchState = (EventDispatchState *)state;

    func_002686F0(state);
    if (func_00268C08(state) == 0) {
        func_00268838(1, state);
    } else {
        func_00268838(0, state);
    }
    func_00269B08(state);
    if (dispatchState->stage != 3) {
        func_00268EC8(state);
    }
    func_00268B48(state);
    func_002C4038((s32)dispatchState->dispatchWork,
                  &dispatchState->dispatchStatus, 1, item);
}

void evtBDispatchSyncB(s32 request) {
    EventDispatchState *state = (EventDispatchState *)func_00101958();

    func_002C4038((s32)state->dispatchWork, &state->dispatchStatus, 2, request);
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
    mode = ((EventDispatchState *)state)->menuMode;
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

void evtBLateDispatchStart(s32 request) {
    EventDispatchState *state = (EventDispatchState *)func_00101958();

    func_002686F0((s32)state);
    func_002C4038((s32)state->dispatchWork, &state->dispatchStatus, 1, request);
}

void evtBDispatchSyncC(s32 request) {
    EventDispatchState *state = (EventDispatchState *)func_00101958();

    func_002C4038((s32)state->dispatchWork, &state->dispatchStatus, 2, request);
}
