#include "common.h"

extern u64 func_00328D68(void);

extern s32 D_00439110;

extern s32 D_00439114;

void func_00328E18(s32 size) {
    u64 allocation;

    allocation = func_00328D68();
    func_00340328(allocation, (size + 0xf) >> 4);
}

INCLUDE_ASM(const s32, "sdf/sdfChip", func_00328E48);

INCLUDE_ASM(const s32, "sdf/sdfChip", func_00328F68);

s32 sdfChipIsInRange(s32 address) {
    s32 withinRange;

    withinRange = 0;
    if (address >= D_00439110) {
        withinRange = address < D_00439114;
    }
    return withinRange;
}
