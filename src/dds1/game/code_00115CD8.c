#include "common.h"

extern u32 func_001117A8(u32);
extern u32 func_002CFEB8(u32);

INCLUDE_ASM(const s32, "game/code_00115CD8", func_00115CD8);

INCLUDE_ASM(const s32, "game/code_00115CD8", func_00115D08);

INCLUDE_ASM(const s32, "game/code_00115CD8", func_00115E10);

u32 func_001160F8(u32 arg0) {
    u32 *puVar1;
    u32 temp_v0;

    effObjInnerCreate();
    puVar1 = (u32 *)func_002CFEB8(0x10);
    *(u32 **)((s32)arg0 + 0x18) = puVar1;
    temp_v0 = func_001117A8(arg0);
    *puVar1 = temp_v0;
    return 1;
}
