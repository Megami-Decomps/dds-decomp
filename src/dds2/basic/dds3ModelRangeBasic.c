#include "common.h"

extern u64 func_00329930(s32);

void func_00116CB8(s32 arg0) {
    s32 temp_v0;
    s32 temp_v1;
    u64 temp_v2;

    temp_v0 = *(s32 *)(arg0 + 0x18);
    temp_v1 = *(s32 *)(temp_v0 + 4);
    if (temp_v1 != 0) {
        temp_v2 = func_00329930(temp_v1);
        func_003298C0(temp_v2);
    }
    func_00328E48(temp_v0);
}
