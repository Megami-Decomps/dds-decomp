#include "common.h"

extern u32 func_001986E0(u32);

extern u32 func_00101A70(void);

INCLUDE_ASM(const s32, "game/code_00234C18", func_00234C18);

INCLUDE_ASM(const s32, "game/code_00234C18", func_00234CA8);

INCLUDE_ASM(const s32, "game/code_00234C18", func_00234DA8);

INCLUDE_ASM(const s32, "game/code_00234C18", func_00234F18);

void func_00235088(void) {
    u32 *puVar1;

    puVar1 = (u32 *)func_00101A70();
    *puVar1 = *puVar1 | 1;
}

void func_002350B0(void) {
    u32 *puVar1;

    puVar1 = (u32 *)func_00101A70();
    *puVar1 = *puVar1 & 0xfffffffe;
}

void func_002350E0(u32 arg0) {
    kwlnTaskDestroyWithHierarchy(arg0, 1);
}

INCLUDE_ASM(const s32, "game/code_00234C18", func_002350F8);

void func_00235120(s32 arg0, u32 arg1) {
    u32 temp_v0;

    temp_v0 = func_001986E0(arg1);
    *(u32 *)(arg0 + 4) = temp_v0;
}

INCLUDE_SDATA(const s32, "game/code_00234C18", D_003BBF78);

