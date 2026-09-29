#include "common.h"

extern u32 func_0029D790(u32, s32);

extern void func_0026C900(void);

extern s32 func_00101958();

extern void func_002C44E8(s32);

extern void mnuTitleRenderFadeAndPanels(s32);

extern void func_002C4038(s32, s32, s32, s32);

extern s32 func_00298E80(s32);

extern void func_0029CDD8(s32);

typedef struct MenuItem {
    u8 pad00[0x55];
    s8 selection;
} MenuItem;

typedef struct MenuItemScene {
    u8 pad00[4];
    u32 overlayFlags;
    u8 pad08[0x90];
    MenuItem **items;
    u8 pad9C[0x1A4];
    u32 resetStateA;
    u32 selectedAction;
    u8 pad248[4];
    s32 selectionApplied;
    u8 pad250[0x174];
    u32 resetStateB;
    s32 selectedExtent;
    u32 activeSlot;
    u32 slots[5];
} MenuItemScene;

INCLUDE_ASM(const s32, "game/code_00299D58", func_00299D58);

void mnuStaffRunPanel1(s32 input) {
    s32 context = func_00101958();

    if (func_00298E80(context) != 0) {
        mnuTitleRenderFadeAndPanels(context);
        func_002C4038(context + 8, context + 0x54, 1, input);
    }
}

void mnuStaffRunPanel2(s32 input) {
    s32 context = func_00101958();

    if (func_00298E80(context) != 0) {
        func_0029CDD8(context);
        func_002C4038(context + 8, context + 0x54, 2, input);
    }
}

u32 func_00299EF0(void) {
    return 1;
}

u32 func_00299EF8(void) {
    return 1;
}

void func_00299F00(s32 input) {
    s32 context = func_00101958();
    func_002C44E8(0x33);
    func_002C4038(context + 8, context + 0x54, 0, input);
}

void func_00299F50(s32 input) {
    s32 context = func_00101958();
    mnuTitleRenderFadeAndPanels(context);
    func_002C4038(context + 8, context + 0x54, 1, input);
}

void func_00299FA0(s32 input) {
    s32 context = func_00101958();
    func_002C4038(context + 8, context + 0x54, 2, input);
}

u32 func_00299FD8(void) {
    return 1;
}

u32 func_00299FE0(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00299D58", func_00299FE8);

u32 mnuProcessItemSelection(u32 address) {
    u32 result;
    s32 context;

    context = (s32)address;
    result = func_0029D790(**(u32 **)(context + 0x9c), context + 0x4e8);
    *(u32 *)(context + 0x268) = result;
    func_00299FE8(address);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00299D58", func_0029A0C8);

u32 func_0029A1E0(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00299D58", func_0029A1E8);

INCLUDE_ASM(const s32, "game/code_00299D58", func_0029A270);

INCLUDE_ASM(const s32, "game/code_00299D58", func_0029A2F8);

INCLUDE_ASM(const s32, "game/code_00299D58", func_0029A400);

void func_0029A588(s32 request) {
    s32 context = func_00101958();

    mnuTitleRenderFadeAndPanels(context);
    func_002C4038(context + 8, context + 0x54, 1, request);
}

void func_0029A5D8(s32 request) {
    s32 context = func_00101958();

    func_0026C900();
    func_002C4038(context + 8, context + 0x54, 2, request);
}

INCLUDE_ASM(const s32, "game/code_00299D58", mnuResetItemSelectionMarkers);

u32 func_0029A650(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00299D58", func_0029A658);

INCLUDE_ASM(const s32, "game/code_00299D58", func_0029A748);

INCLUDE_ASM(const s32, "game/code_00299D58", func_0029A768);

INCLUDE_ASM(const s32, "game/code_00299D58", func_0029A898);

INCLUDE_SDATA(const s32, "game/code_00299D58", D_004379B0);

