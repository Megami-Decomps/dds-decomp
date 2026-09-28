#include "common.h"

extern s32 func_00112AB0(void);

extern u32 func_001119D0(u32);

extern s32 func_00328D68(u32);

INCLUDE_ASM(const s32, "game/code_00112AB0", func_00112AB0);

void func_00112B58(u32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = func_00112AB0();
    *(u32 *)(temp_v0 + 0x44) = arg1;
}

u32 func_00112B80(u32 arg0) {
    s32 temp_v0;
    u32 temp_v1;

    effObjInnerCreate();
    temp_v0 = func_00328D68(0x90);
    *(s32 *)((s32)arg0 + 0x18) = temp_v0;
    temp_v1 = func_001119D0(arg0);
    *(u32 *)(temp_v0 + 0x80) = temp_v1;
    func_00111B30(arg0, 0x62);
    *(u32 *)(temp_v0 + 0x88) = 0;
    return 1;
}
