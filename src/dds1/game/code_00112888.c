#include "common.h"

extern u32 func_001117A8(u32);
extern s32 func_002CFEB8(u32);

extern s32 func_00112888(void);

INCLUDE_ASM(const s32, "game/code_00112888", func_00112888);

void func_00112930(u32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = func_00112888();
    *(u32 *)(temp_v0 + 0x44) = arg1;
}

u32 func_00112958(u32 arg0) {
    s32 temp_v0;
    u32 temp_v1;

    effObjInnerCreate();
    temp_v0 = func_002CFEB8(0x90);
    *(s32 *)((s32)arg0 + 0x18) = temp_v0;
    temp_v1 = func_001117A8(arg0);
    *(u32 *)(temp_v0 + 0x80) = temp_v1;
    func_00111908(arg0, 0x62);
    *(u32 *)(temp_v0 + 0x88) = 0;
    return 1;
}
