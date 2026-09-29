#include "common.h"

extern s32 func_00101958();

void func_0029AA48(s32 arg0) {
    func_002B7F80(arg0 + 0xad40, 0x20);
}

INCLUDE_ASM(const s32, "game/code_0029AA48", func_0029AA68);

INCLUDE_ASM(const s32, "game/code_0029AA48", func_0029AC20);

INCLUDE_ASM(const s32, "game/code_0029AA48", func_0029AD98);

INCLUDE_ASM(const s32, "game/code_0029AA48", func_0029AE10);

u32 func_0029AE70(void) {
    s8 current;
    s32 context;
    u32 *destination;
    s8 *source;
    s32 remaining;
    s32 selectionWidth;
    s32 totalWidth;

    context = func_00101958();
    totalWidth = 0;
    remaining = 4;
    selectionWidth = (*(s32 **)(context + 0x9c))[1] * 3;
    source = (s8 *)(**(s32 **)(context + 0x9c) + 0x16);
    do {
        current = *source;
        source = source + 1;
        remaining = remaining - 1;
        totalWidth = totalWidth + current;
    } while (-1 < remaining);
    *(u32 *)(context + 0x3f0) = 0;
    remaining = 4;
    destination = (u32 *)(context + 0x404);
    if (0x1ef - totalWidth < selectionWidth) {
        selectionWidth = 0x1ef - totalWidth;
    }
    *(s32 *)(context + 0x3ec) = selectionWidth;
    do {
        remaining = remaining - 1;
        *destination = 0;
        destination = destination + -1;
    } while (-1 < remaining);
    if (*(s32 *)(context + 0xb6e4) != 0) {
        func_002C0CF8(*(u32 *)(context + 0xad34), 0);
    }
    return 1;
}

u32 func_0029AF40(void) {
    return 1;
}

void func_0029AF48(s32 context) {
    s32 remaining;
    u32 *destination;

    *(u32 *)(context + 0x3f0) = 0;
    destination = (u32 *)(context + 0x404);
    remaining = 4;
    do {
        remaining = remaining - 1;
        *destination = 0;
        destination = destination + -1;
    } while (-1 < remaining);
}

void func_0029AF80(u32 arg0, u32 arg1) {
    func_00314298(arg0, (s32)arg1 + 0x3f4);
    func_00299A38(arg0, arg1);
}
