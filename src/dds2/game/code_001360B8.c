#include "common.h"

extern u32 D_0038BBD8[];

extern u32 D_0038BB50[];

extern u32 D_0038BB60[];

extern void *memset(void *s, s32 c, u32 n);

INCLUDE_ASM(const s32, "game/code_001360B8", func_001360B8);

INCLUDE_ASM(const s32, "game/code_001360B8", func_00136368);

void func_00136388(void) {
    u32 *temp_v0 = D_0038BBD8;

    memset(temp_v0, 0, 0x14);
    temp_v0[0] = (u32)D_0038BB50;
    temp_v0[1] = (u32)D_0038BB60;
}

INCLUDE_ASM(const s32, "game/code_001360B8", func_001363D8);

INCLUDE_ASM(const s32, "game/code_001360B8", func_00136718);

INCLUDE_ASM(const s32, "game/code_001360B8", func_00136850);

INCLUDE_ASM(const s32, "game/code_001360B8", func_00136A70);

INCLUDE_ASM(const s32, "game/code_001360B8", func_00136C90);

INCLUDE_ASM(const s32, "game/code_001360B8", func_00136E60);
INCLUDE_SDATA(const s32, "game/code_001360B8", D_00436168);

INCLUDE_SDATA(const s32, "game/code_001360B8", D_0043616C);


INCLUDE_SDATA(const s32, "game/code_001360B8", D_00436170);

