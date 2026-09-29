#include "common.h"

extern void func_0025FD78(s32);

extern s64 func_002C4038(s32, s32 *, u64, u64);

extern s32 func_00101958();

extern void func_0026C900(void);

typedef struct EventCallbackContext {
    u8 pad00[0x58];
    s32 dispatch; /* +0x58: state passed to the callback dispatcher */
} EventCallbackContext;

typedef struct FlagEntry {
    s32 firstFlag;
    s32 firstOn;
    s32 secondFlag;
    s32 secondOn;
} FlagEntry;

extern s32 sdfResourceRetainAddress(s32);

extern s32 mdlFlagTest(s32);

extern s32 func_003292A8(s32);

typedef struct FlagPair {
    s32 first;
    s32 second;
} FlagPair;

typedef struct FlagSource {
    s32 first;
    s32 second;
    s32 pad[2];
} FlagSource;

extern FlagSource D_003CE6E8[];

extern FlagPair D_003CE728[];

extern s64 kwlnFadeIsActive(void);

extern u8 D_003CE6AC[];

extern void func_002C42C0();

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

INCLUDE_ASM(const s32, "game/code_00265AD8", func_00265AD8);

INCLUDE_ASM(const s32, "game/code_00265AD8", func_00265E78);

/* Route the supplied callback through dispatch mode 2 after menu setup. */
s64 func_00265EE8(s32 callback) {
    s32 eventContext = func_00101958();
    func_0026C900();
    return func_002C4038(eventContext + 0xc, &((EventCallbackContext *)eventContext)->dispatch, 2, callback);
}

u32 func_00265F30(void) {
    func_0026C948(1);
    func_0026C5B8(0xd);
    return 1;
}

u32 func_00265F58(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00265AD8", func_00265F60);

INCLUDE_ASM(const s32, "game/code_00265AD8", func_00265FE8);

s64 func_00266038(s32 callback) {
    s32 eventContext = func_00101958();
    func_0026C900();
    return func_002C4038(eventContext + 0xc, &((EventCallbackContext *)eventContext)->dispatch, 2, callback);
}

INCLUDE_ASM(const s32, "game/code_00265AD8", func_00266080);

u32 evtStartFadeOut(void) {
    kwlnFadeOutStart(0, 0, 0, 0);
    return 1;
}

s64 func_00266108(s32 callback) {
    s32 context = func_00101958();
    s32 *window = (s32 *)(context + 0x58);
    s64 state = func_002C4038(context + 0xc, window, 0, callback);
    if (state == 0) {
        if (*window == 0) {
            if (kwlnFadeIsActive() == 0) {
                func_002C42C0(window, D_003CE6AC);
            }
        }
        return 0;
    }
    return state;
}

INCLUDE_ASM(const s32, "game/code_00265AD8", func_00266188);

INCLUDE_ASM(const s32, "game/code_00265AD8", evtDispatchSync);

u32 func_00266210(void) {
    return 1;
}

u32 func_00266218(void) {
    return 1;
}

u32 func_00266220(void) {
    return 0;
}

u32 func_00266228(void) {
    return 0;
}

u32 func_00266230(void) {
    return 0;
}

/* Snapshot four primary flag pairs and sixteen extra pairs for restoration. */
s32 mnuCreateFlagEntries(void) {
    s32 handle = func_003292A8(0x140);
    FlagEntry *entries = (FlagEntry *)sdfResourceRetainAddress(handle);
    u32 i;

    for (i = 0; i < 4; i++) {
        entries[i].firstFlag = D_003CE6E8[i].first;
        entries[i].firstOn = mdlFlagTest(entries[i].firstFlag);
        entries[i].secondFlag = D_003CE6E8[i].second;
        entries[i].secondOn = mdlFlagTest(entries[i].secondFlag);
    }
    for (i = 0; i < 16; i++) {
        entries[4 + i].firstFlag = D_003CE728[i].first;
        entries[4 + i].firstOn = mdlFlagTest(entries[4 + i].firstFlag);
        entries[4 + i].secondFlag = D_003CE728[i].second;
        entries[4 + i].secondOn = mdlFlagTest(entries[4 + i].secondFlag);
    }
    return handle;
}

INCLUDE_ASM(const s32, "game/code_00265AD8", func_00266320);

INCLUDE_ASM(const s32, "game/code_00265AD8", func_002663D8);

INCLUDE_RODATA(const s32, "game/code_00265AD8", D_00424D50);

INCLUDE_RODATA(const s32, "game/code_00265AD8", D_00424D60);

INCLUDE_RODATA(const s32, "game/code_00265AD8", D_00424D70);

INCLUDE_RODATA(const s32, "game/code_00265AD8", D_00424D80);

INCLUDE_RODATA(const s32, "game/code_00265AD8", D_00424D90);

INCLUDE_RODATA(const s32, "game/code_00265AD8", D_00424DA0);

INCLUDE_RODATA(const s32, "game/code_00265AD8", D_00424DB0);

INCLUDE_RODATA(const s32, "game/code_00265AD8", D_00424DC0);

INCLUDE_RODATA(const s32, "game/code_00265AD8", D_00424DD0);

INCLUDE_RODATA(const s32, "game/code_00265AD8", D_00424DE0);

INCLUDE_RODATA(const s32, "game/code_00265AD8", D_00424E10);

INCLUDE_ASM(const s32, "game/code_00265AD8", func_00266460);

INCLUDE_RODATA(const s32, "game/code_00265AD8", D_00424E48);

