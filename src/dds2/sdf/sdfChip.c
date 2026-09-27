#include "common.h"

extern s32 D_00439110;

extern s32 D_00439114;

INCLUDE_ASM(const s32, "sdf/sdfChip", func_00328E18);

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
