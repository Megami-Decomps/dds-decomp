#include "common.h"

extern u32 func_00265E68(u32, s32);

extern s32 mdlFlagTest(u32);

extern s32 func_00101A70();

extern void func_0024DDC0(s32);

extern void func_0024DA58(s32);

extern s32 func_002CD788(void *);

extern s32 func_002CD2A8(u16);

extern s32 func_002CD548(void *, u16);

extern void func_002CD428(void *, u16);

extern void mdlFlagSet(s32);

extern void func_00265478(s32 arg0);

extern s64 func_00285670(s32, s32 *, u64, u64);

extern void func_0024DD78(void);

void kwlnItemApplySelection(u32 address) {
    u8 *scene = (u8 *)address;
    u8 *item = **(u8 ***)(scene + 0x98);
    s32 *data = (s32 *)func_002CD788(item);
    s8 selection = item[0x55];
    if (selection != 0 && func_002CD2A8((u16)selection) == *data &&
        func_002CD548(item, (u16)(s8)item[0x55]) == 0) {
        func_002CD428(item, (u16)(s8)item[0x55]);
        *(s32 *)(scene + 0x24C) = 1;
        if (mdlFlagTest(0x910) == 0) {
            *(s32 *)(scene + 4) |= 1;
            mdlFlagSet(0x910);
        }
    } else {
        *(s32 *)(scene + 0x24C) = 0;
    }
}

u32 func_00263220(u32 arg0) {
    u32 temp_v0;
    s32 temp_v1;

    temp_v1 = (s32)arg0;
    temp_v0 = func_00265E68(**(u32 **)(temp_v1 + 0x98), temp_v1 + 0x4c4);
    *(u32 *)(temp_v1 + 0x244) = temp_v0;
    kwlnItemApplySelection(arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00263148", func_00263260);

s32 kwlnItemDismissOverlay(u32 *state) {
    if (state[1] & 1) {
        func_0024DDC0(1);
        func_0024DA58(1);
        state[1] &= ~1;
        return 1;
    }
    return 0;
}

u32 func_002633D8(void) {
    s64 temp_v0;

    temp_v0 = mdlFlagTest(0x911);
    if (temp_v0 == 0) {
        mdlFlagSet(0x911);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00263148", func_00263408);

static inline s64 menuRunPanel(s32 context, u64 mode, u64 arg) {
    return func_00285670(context + 8, (s32 *)(context + 0x54), mode, arg);
}

s64 func_00263570(s32 request) {
    s32 context = func_00101A70();
    func_00265478(context);
    return menuRunPanel(context, 1, request);
}

s64 func_002635C0(s32 request) {
    s32 context = func_00101A70();
    func_0024DD78();
    return menuRunPanel(context, 2, request);
}

s32 func_00263608(void) {
    s32 *temp_v0 = (s32 *)func_00101A70();
    temp_v0[144] = 0;
    temp_v0[241] = 0;
    return 1;
}

u32 func_00263638(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00263148", func_00263640);

INCLUDE_ASM(const s32, "game/code_00263148", func_00263728);

INCLUDE_ASM(const s32, "game/code_00263148", func_00263838);

void func_002639E0(s32 arg0) {
    mnuDrawBackdrop(arg0 + 0xd1c, 0x20);
}

INCLUDE_ASM(const s32, "game/code_00263148", func_00263A00);

INCLUDE_ASM(const s32, "game/code_00263148", func_00263B78);

INCLUDE_ASM(const s32, "game/code_00263148", func_00263C98);

INCLUDE_ASM(const s32, "game/code_00263148", func_00263D10);

u32 func_00263D70(void) {
    s8 component;
    s32 scene;
    u32 *slot;
    s8 *byteCursor;
    s32 remaining;
    s32 extent;
    s32 sum;

    scene = func_00101A70();
    sum = 0;
    remaining = 4;
    extent = (*(s32 **)(scene + 0x98))[1] * 3;
    byteCursor = (s8 *)(**(s32 **)(scene + 0x98) + 0x16);
    do {
        component = *byteCursor;
        byteCursor = byteCursor + 1;
        remaining = remaining - 1;
        sum = sum + component;
    } while (-1 < remaining);
    *(u32 *)(scene + 0x3cc) = 0;
    remaining = 4;
    slot = (u32 *)(scene + 0x3e0);
    if (0x1ef - sum < extent) {
        extent = 0x1ef - sum;
    }
    *(s32 *)(scene + 0x3c8) = extent;
    do {
        remaining = remaining - 1;
        *slot = 0;
        slot = slot + -1;
    } while (-1 < remaining);
    if (*(s32 *)(scene + 0x1578) != 0) {
        func_002830F0(*(u32 *)(scene + 0xd10), 0);
    }
    return 1;
}

u32 func_00263E30(void) {
    return 1;
}

void func_00263E38(s32 scene) {
    s32 remaining;
    u32 *slot;

    *(u32 *)(scene + 0x3cc) = 0;
    slot = (u32 *)(scene + 0x3e0);
    remaining = 4;
    do {
        remaining = remaining - 1;
        *slot = 0;
        slot = slot + -1;
    } while (-1 < remaining);
}

void func_00263E70(u32 arg0, u32 arg1) {
    func_002CCE60(arg0, (s32)arg1 + 0x3d0);
    func_00262AC0(arg0, arg1);
}

INCLUDE_SDATA(const s32, "game/code_00263148", D_003BC550);

