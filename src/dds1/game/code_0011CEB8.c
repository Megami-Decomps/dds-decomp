#include "common.h"

extern u64 func_0010D428(u64);
extern u64 func_0011B140(u64, u64);

extern u32 D_003BD7A8;

INCLUDE_ASM(const s32, "game/code_0011CEB8", func_0011CEB8);

u32 func_0011CEF0(void) {
    u64 value;
    u64 otherValue;

    value = func_0010D428(0);
    otherValue = func_0010D428(1);
    value = func_0011B140(value, otherValue);
    func_0010D5F0(value);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0011CEB8", func_0011CF38);

INCLUDE_ASM(const s32, "game/code_0011CEB8", func_0011CF88);

void dds3AppendLinkedNode(s32 *list, s32 node, s32 linkOffset) {
    s32 previous;

    previous = list[1];
    if (previous == 0) {
        *list = node;
    }
    else {
        *(s32 *)(previous + linkOffset + 4) = node;
    }
    *(s32 *)(node + linkOffset) = previous;
    ((s32 *)(node + linkOffset))[1] = 0;
    list[1] = node;
}

INCLUDE_ASM(const s32, "game/code_0011CEB8", func_0011CFF0);

INCLUDE_ASM(const s32, "game/code_0011CEB8", func_0011D030);

INCLUDE_ASM(const s32, "game/code_0011CEB8", func_0011D070);

void func_0011D0B0(void) {
    u32 current;
    while ((current = D_003BD7A8) != 0) {
        func_0011D070(current);
    }
}

void func_0011D0E0(s32 node, u32 value) {
    *(u32 *)(node + 8) = value;
}

INCLUDE_ASM(const s32, "game/code_0011CEB8", func_0011D0E8);

INCLUDE_ASM(const s32, "game/code_0011CEB8", func_0011D138);

void func_0011D178(u32 owner) {
    func_00194920(*(u32 *)((s32)owner + 0x10));
    func_002CFF98(owner);
}

void func_0011D1A8(s32 owner) {
    func_00195868(*(u32 *)(owner + 0x10));
}

INCLUDE_ASM(const s32, "game/code_0011CEB8", func_0011D1C0);

void func_0011D258(s32 owner, u8 value) {
    func_00195470(*(u32 *)(owner + 0x10), value);
}

void func_0011D278(void) {
    func_002CFF98();
}

INCLUDE_SDATA(const s32, "game/code_0011CEB8", D_003BAAD0);

