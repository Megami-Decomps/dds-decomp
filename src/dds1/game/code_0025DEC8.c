#include "common.h"

extern u32 D_003BAA9C;

extern u64 func_00197C40(u64, u64, u64, u16, u32, u64);

void func_0025DEC8(s32 arg0, u64 arg1, u64 arg2, u64 arg3,
                                    u64 arg4) {
    s32 temp_v0;
    u64 temp_v1;

    temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0x70) + 0x14);
    if (*(s32 *)(temp_v0 + 0x20) != 0) {
        temp_v1 = func_00197C40(0x970, 0xb58, 1, *(u16 *)(*(s32 *)(temp_v0 + 0x1c) + 100), D_003BAA9C,
                                                    arg2);
        func_001954C8(temp_v1, arg3);
        func_001958A0(temp_v1, 1, arg4);
        func_00194920(temp_v1);
        return;
    }
}


