#include "common.h"

extern void func_0024DD78(void);

extern s64 func_00285670(s32, s32 *, u64, u64);

extern s32 func_00101A70();

INCLUDE_ASM(const s32, "game/code_00273AB0", func_00273AB0);

INCLUDE_ASM(const s32, "game/code_00273AB0", func_00273B98);

u32 func_00273C40(void) {
    return 1;
}

u32 func_00273C48(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00273AB0", func_00273C50);

INCLUDE_ASM(const s32, "game/code_00273AB0", func_00273D40);

s64 func_00273DE8(s32 callback) {
    s32 context = func_00101A70();
    return func_00285670(context + 8, (s32 *)(context + 0x54), 2, callback);
}

INCLUDE_ASM(const s32, "game/code_00273AB0", func_00273E20);

INCLUDE_ASM(const s32, "game/code_00273AB0", func_00273F20);

s64 func_00274008(s32 callback) {
    s32 context = func_00101A70();
    return func_00285670(context + 8, (s32 *)(context + 0x54), 2, callback);
}

u32 func_00274040(void) {
    return 1;
}

u32 func_00274048(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00273AB0", func_00274050);

INCLUDE_ASM(const s32, "game/code_00273AB0", func_00274228);

s64 func_00274310(s32 callback) {
    s32 context = func_00101A70();
    return func_00285670(context + 8, (s32 *)(context + 0x54), 2, callback);
}

INCLUDE_ASM(const s32, "game/code_00273AB0", func_00274348);

INCLUDE_ASM(const s32, "game/code_00273AB0", func_00274430);

extern u16 func_00286AD0(s32);
extern void func_0024DD90(s32, void *);
extern void func_0024DA58(s32);
extern void func_00283BF0(s32, s32);
extern void func_00119900(s32, s32);
extern u8 *D_003BAA70;
extern u8 *D_003BAA84;

/* Swap the equipped bullet item: update the actor/old/new message tokens and
 * queue the inventory delta, then latch the old/new ids in the context. */
void func_002744E0(s32 arg0, u8 *arg1, s32 arg2) {
    u8 *ctx = *(u8 **)(arg0 + 0x90C);
    s32 equipped = func_00286AD0((s32)arg1);

    func_00283BF0(arg0 + 0x914, 1);
    if (equipped != arg2) {
        func_0024DD90(0, D_003BAA70 + *(u16 *)(arg1 + 4) * 17);
        func_0024DD90(1, D_003BAA84 + equipped * 25);
        func_0024DD90(2, D_003BAA84 + arg2 * 25);
        func_0024DA58(0);
        if (equipped != 0) {
            func_00119900(equipped, 1);
        }
        func_00119900(arg2, -1);
        *(s32 *)(ctx + 0x1C) = equipped;
        *(s32 *)(ctx + 0x20) = arg2;
    } else {
        func_0024DD90(0, D_003BAA84 + equipped * 25);
        func_0024DA58(1);
        *(s32 *)(ctx + 0x1C) = 0;
        *(s32 *)(ctx + 0x20) = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00273AB0", func_00274610);

INCLUDE_ASM(const s32, "game/code_00273AB0", func_00274768);

INCLUDE_ASM(const s32, "game/code_00273AB0", func_00274978);

void func_00274B30(s32 selection) {
    s32 context = func_00101A70();

    func_0024DD78();
    func_00285670(context + 8, context + 0x54, 2, selection);
}

u32 func_00274B78(void) {
    return 1;
}
