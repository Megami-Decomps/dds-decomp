#include "common.h"

extern s32 func_00101958();

extern s64 func_002C4038(s32, s32 *, u64, u64);

extern s64 fileConsumeConfigTaskReady(void);

extern u8 D_003E7034[];

extern void func_002C42C0();

extern u8 D_003E7200[];

extern u8 D_003E73F8[];

extern s32 mnuUseFieldSkillOnParty(s32, s32, s32);

/* Sub-object reached through the context's +0x104 chain. */
typedef struct {
    u8 pad0[0x48]; /* 0x0 */
    u32 flags;     /* 0x48 */
} MtrSub;

typedef struct {
    u8 pad0[0x14]; /* 0x0 */
    MtrSub *sub;   /* 0x14 */
} MtrMid;

typedef struct {
    u8 pad0[0x18]; /* 0x0 */
    MtrMid *mid;   /* 0x18 */
} MtrRoot;

/* Mantra context fields reset when the menu task is (re)created. */
typedef struct {
    u8 pad0[0xB1D0]; /* 0x0 */
    u32 unkB1D0;     /* 0xB1D0 */
    u32 unkB1D4;     /* 0xB1D4 */
    u32 unkB1D8;     /* 0xB1D8 */
} MtrCtx;

u32 func_002AAEA0(void) {
    s32 context;

    context = func_00101958();
    if (mnuUseFieldSkillOnParty(0xA928 + context, context + 0x284, 0) == 0) {
        ((MtrRoot *)*(u32 *)(context + 0x104))->mid->sub->flags |= 1;
    } else {
        ((MtrRoot *)*(u32 *)(context + 0x104))->mid->sub->flags &= ~1;
    }
    ((MtrCtx *)context)->unkB1D4 = 0;
    ((MtrCtx *)context)->unkB1D0 = 0;
    ((MtrCtx *)context)->unkB1D8 = -0x32;
    return 1;
}

u32 func_002AAF40(void) {
    s32 context;

    context = func_00101958();
    *(u32 *)(context + 0xb1d0) = 1;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002AAEA0", func_002AAF70);

INCLUDE_ASM(const s32, "game/code_002AAEA0", func_002AB0E0);

s64 func_002AB1B0(s32 arg0) {
    s32 context;

    context = func_00101958();
    return func_002C4038(context + 8, (s32 *)(context + 0x54), 2, arg0);
}

u32 func_002AB1E8(void) {
    s32 context;

    context = func_00101958();
    func_002A9460(5, context);
    func_002BB498(*(u32 *)(context + 0x118), *(u32 *)(context + 0xF0), 0, 0);
    mnuCreateConfigTasks(0);
    return 1;
}

/* Switch the staff display to the alternate resource at context + 0x60. */
u32 func_002AB240(void) {
    s32 context;

    context = func_00101958();
    func_002BB498(*(u32 *)(context + 0x118), *(u32 *)(context + 0x60), 0, 1);
    return 1;
}

/* Dispatch a callback; on idle, install the default entry unless busy. */
s64 func_002AB278(s32 callback) {
    s32 context = func_00101958();
    s32 *dispatchEntry = (s32 *)(context + 0x54);
    s64 state = func_002C4038(context + 8, dispatchEntry, 0, callback);
    if (state == 0) {
        if (fileConsumeConfigTaskReady() == 0) {
            func_002C42C0(dispatchEntry, D_003E7034);
        }
        return 0;
    }
    return state;
}

s64 func_002AB2E8(s32 arg0) {
    s32 context;

    context = func_00101958();
    func_002B7F80(context + 0x11C, 0x20);
    func_002BB510(-0x10, -8, 0, *(u32 *)(context + 0x118), 0x54);
    mnuCreateStaffImageSprite(0x18);
    func_002AA7A0(2, *(u32 *)(context + 0x60));
    return func_002C4038(context + 8, (s32 *)(context + 0x54), 1, arg0);
}

s64 func_002AB368(s32 arg0) {
    s32 context;

    context = func_00101958();
    return func_002C4038(context + 8, (s32 *)(context + 0x54), 2, arg0);
}

u32 func_002AB3A0(void) {
    return 1;
}

u32 func_002AB3A8(void) {
    func_002AAEA0();
    return 1;
}

s64 func_002AB3C8(s32 arg0) {
    s32 context;
    s64 state;

    context = func_00101958();
    state = func_002C4038(context + 8, (s32 *)(context + 0x54), 0, arg0);
    if (state != 0) {
        return state;
    }
    mnuUseFieldSkillOnParty(0xA928 + context, context + 0x284, 1);
    func_002C42B0(context + 0x54, D_003E7034);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002AAEA0", func_002AB448);

s64 func_002AB518(s32 arg0) {
    s32 context;

    context = func_00101958();
    return func_002C4038(context + 8, (s32 *)(context + 0x54), 2, arg0);
}

u32 func_002AB550(void) {
    return 1;
}

s32 mtrMantraIdIsValid(s32 arg0) {
    u32 i;

    for (i = 0; i < 5; i++) {
        if (arg0 == D_003E73F8[i]) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002AAEA0", func_002AB598);

s32 mtrMantraFindIndex(s32 arg0) {
    u32 i;

    for (i = 0; i < 0x12; i++) {
        if (arg0 == *(u16 *)(D_003E7200 + i * 0x1C)) {
            return i + 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002AAEA0", func_002AB690);

INCLUDE_SDATA(const s32, "game/code_002AAEA0", D_00437B88);

INCLUDE_SDATA(const s32, "game/code_002AAEA0", D_00437B90);

INCLUDE_SDATA(const s32, "game/code_002AAEA0", D_00437B98);

INCLUDE_SDATA(const s32, "game/code_002AAEA0", D_00437BA0);

INCLUDE_SDATA(const s32, "game/code_002AAEA0", D_00437BA8);

INCLUDE_SDATA(const s32, "game/code_002AAEA0", D_00437BB0);

INCLUDE_SDATA(const s32, "game/code_002AAEA0", D_00437BB8);

INCLUDE_SDATA(const s32, "game/code_002AAEA0", D_00437BC0);

INCLUDE_SDATA(const s32, "game/code_002AAEA0", D_00437BC8);

