#include "common.h"

extern u32 func_002AB598(void);

extern void func_0010D818(s32 value);

extern s32 func_0011C0B0(s32 param0, s32 param1);

/* Persona 4 func_002993c0 @ 002993C0 (src/Script/scrCommonCommand.c), recompiled unchanged */
extern s32 func_0010D650(s32);

INCLUDE_ASM(const s32, "game/code_0011EC90", func_0011EC90);

s32 func_0011ECC8(void) {
    s32 param0 = func_0010D650(0);
    s32 param1 = func_0010D650(1);

    func_0010D818(func_0011C0B0(param0, param1));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0011EC90", func_0011ED10);

s32 func_0011ED60(void) {
    func_0010D818(func_002AB598());
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0011EC90", func_0011ED88);

void dds3AppendIntrusiveNode(s32 *list, s32 node, s32 linkOffset) {
    s32 last;

    last = list[1];
    if (last == 0) {
        *list = node;
    }
    else {
        *(s32 *)(last + linkOffset + 4) = node;
    }
    *(s32 *)(node + linkOffset) = last;
    ((s32 *)(node + linkOffset))[1] = 0;
    list[1] = node;
}

INCLUDE_ASM(const s32, "game/code_0011EC90", func_0011EE58);

INCLUDE_ASM(const s32, "game/code_0011EC90", func_0011EE98);

INCLUDE_ASM(const s32, "game/code_0011EC90", func_0011EED8);

INCLUDE_ASM(const s32, "game/code_0011EC90", func_0011EF18);

void func_0011EF48(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 8) = arg1;
}

INCLUDE_ASM(const s32, "game/code_0011EC90", func_0011EF50);

INCLUDE_ASM(const s32, "game/code_0011EC90", func_0011EFA0);

void func_0011EFE0(u32 arg0) {
    func_0019C5B0(*(u32 *)((s32)arg0 + 0x10));
    func_00328E48(arg0);
}

void func_0011F010(s32 arg0) {
    func_0019D518(*(u32 *)(arg0 + 0x10));
}

INCLUDE_ASM(const s32, "game/code_0011EC90", func_0011F028);

void func_0011F0C0(s32 arg0, u8 arg1) {
    func_0019D120(*(u32 *)(arg0 + 0x10), arg1);
}

void func_0011F0E0(void) {
    func_00328E48();
}

INCLUDE_SDATA(const s32, "game/code_0011EC90", D_00435EA8);

