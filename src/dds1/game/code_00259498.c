#include "common.h"

extern void func_002CFF98(void *);
/* Retail retains a jal and epilogue; default TU -O2 changes the shape. */

extern s32 func_002D03F8(s32);

extern void *func_002D03F0(s32);

extern f32 func_002E8398(s32);

extern void *memset(void *, s32, u32);

INCLUDE_ASM(const s32, "game/code_00259498", func_00259498);

INCLUDE_ASM(const s32, "game/code_00259498", func_00259890);

INCLUDE_ASM(const s32, "game/code_00259498", func_00259B40);

INCLUDE_ASM(const s32, "game/code_00259498", func_0025A680);

INCLUDE_ASM(const s32, "game/code_00259498", func_0025AA20);

INCLUDE_ASM(const s32, "game/code_00259498", func_0025AB38);

INCLUDE_ASM(const s32, "game/code_00259498", func_0025AC50);

INCLUDE_ASM(const s32, "game/code_00259498", func_0025AD68);

INCLUDE_ASM(const s32, "game/code_00259498", func_0025AE80);

INCLUDE_ASM(const s32, "game/code_00259498", func_0025B0F0);

INCLUDE_ASM(const s32, "game/code_00259498", func_0025B350);

void func_0025B7B0(void *unused, void *allocation) {
    if (allocation != 0) {
        func_002CFF98(allocation);
    }
}


void *func_0025B7D8(s32 owner, u8 sprite, u8 variant) {
    s32 allocation = func_002D03F8(0x48);
    u8 *resource = func_002D03F0(allocation);
    memset(resource, 0, 0x48);
    *(s32 *)resource = allocation;
    *(s32 *)(resource + 0x34) = owner;
    resource[0x45] = sprite;
    resource[0x44] = variant;
    *(s32 *)(resource + 0x30) = (s32)(func_002E8398(0) * 30.0f + 10.0f);
    return resource;
}

INCLUDE_ASM(const s32, "game/code_00259498", func_0025B888);

INCLUDE_RODATA(const s32, "game/code_00259498", D_003AF9F0);

INCLUDE_SDATA(const s32, "game/code_00259498", D_003BC4A0);

INCLUDE_SDATA(const s32, "game/code_00259498", D_003BC4A8);

INCLUDE_SDATA(const s32, "game/code_00259498", D_003BC4B0);

INCLUDE_SDATA(const s32, "game/code_00259498", D_003BC4B8);

INCLUDE_SDATA(const s32, "game/code_00259498", D_003BC4C0);

INCLUDE_SDATA(const s32, "game/code_00259498", D_003BC4C8);

INCLUDE_SDATA(const s32, "game/code_00259498", D_003BC4CC);

