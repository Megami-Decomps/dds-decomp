#include "common.h"

extern u64 func_00328D68(void);

extern s32 D_00439110;

extern s32 D_00439114;

void func_00328E18(s32 arg0) {
    u64 temp_v0;

    temp_v0 = func_00328D68();
    func_00340328(temp_v0, (arg0 + 0xf) >> 4);
}

INCLUDE_ASM(const s32, "sdf/sdfChip", func_00328E48);

INCLUDE_ASM(const s32, "sdf/sdfChip", func_00328F68);

s32 func_00328F88(s32 arg0) {
    s32 ret;

    ret = 0;
    if (arg0 >= D_00439110) {
        ret = arg0 < D_00439114;
    }
    return ret;
}
