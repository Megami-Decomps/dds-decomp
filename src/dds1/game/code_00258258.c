#include "common.h"

extern void func_002CFF98(void *);

extern s32 func_002D03F8(s32);

extern void *func_002D03F0(s32);

extern f32 func_002E8398(s32);

extern void *memset(void *, s32, u32);

INCLUDE_ASM(const s32, "game/code_00258258", func_00258258);

INCLUDE_ASM(const s32, "game/code_00258258", func_00258508);

INCLUDE_ASM(const s32, "game/code_00258258", func_00258620);

INCLUDE_ASM(const s32, "game/code_00258258", func_00258A70);

void func_00258AF0(u32 *arg0, u32 arg1) {
    arg0[1] = arg1;
    *arg0 = 0;
}

INCLUDE_ASM(const s32, "game/code_00258258", func_00258B00);

INCLUDE_ASM(const s32, "game/code_00258258", func_00258B90);

INCLUDE_ASM(const s32, "game/code_00258258", func_00258EB8);

INCLUDE_ASM(const s32, "game/code_00258258", func_00258FD0);

INCLUDE_ASM(const s32, "game/code_00258258", func_002593E0);

INCLUDE_ASM(const s32, "game/code_00258258", func_00259498);

INCLUDE_ASM(const s32, "game/code_00258258", func_00259890);

INCLUDE_ASM(const s32, "game/code_00258258", func_00259B40);

INCLUDE_ASM(const s32, "game/code_00258258", func_0025A680);

INCLUDE_ASM(const s32, "game/code_00258258", func_0025AA20);

INCLUDE_ASM(const s32, "game/code_00258258", func_0025AB38);

INCLUDE_ASM(const s32, "game/code_00258258", func_0025AC50);

INCLUDE_ASM(const s32, "game/code_00258258", func_0025AD68);

INCLUDE_ASM(const s32, "game/code_00258258", func_0025AE80);

INCLUDE_ASM(const s32, "game/code_00258258", func_0025B0F0);

INCLUDE_RODATA(const s32, "game/code_00258258", D_003AF9B8);

INCLUDE_ASM(const s32, "game/code_00258258", func_0025B350);

INCLUDE_ASM(const s32, "game/code_00258258", func_0025B7B0);

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

INCLUDE_ASM(const s32, "game/code_00258258", func_0025B888);

INCLUDE_ASM(const s32, "game/code_00258258", func_0025BA20);

INCLUDE_RODATA(const s32, "game/code_00258258", D_003AF9F0);

INCLUDE_SDATA(const s32, "game/code_00258258", D_003BC488);

INCLUDE_SDATA(const s32, "game/code_00258258", D_003BC490);

INCLUDE_SDATA(const s32, "game/code_00258258", D_003BC498);

INCLUDE_SDATA(const s32, "game/code_00258258", D_003BC4A0);

INCLUDE_SDATA(const s32, "game/code_00258258", D_003BC4A8);

INCLUDE_SDATA(const s32, "game/code_00258258", D_003BC4B0);

INCLUDE_SDATA(const s32, "game/code_00258258", D_003BC4B8);

INCLUDE_SDATA(const s32, "game/code_00258258", D_003BC4C0);

INCLUDE_SDATA(const s32, "game/code_00258258", D_003BC4C8);

INCLUDE_SDATA(const s32, "game/code_00258258", D_003BC4CC);
