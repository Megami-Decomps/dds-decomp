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

INCLUDE_ASM(const s32, "game/code_00273AB0", func_002744E0);

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
