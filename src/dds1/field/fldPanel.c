#include "common.h"

extern u32 D_003BAE74;
extern u64 func_00101A70(void);

void func_001410E8(void) {
    u64 temp_v0;

    temp_v0 = func_00101A70();
    func_002CFF98(temp_v0);
    D_003BAE74 = 0;
}

INCLUDE_ASM(const s32, "field/fldPanel", func_00141110);

INCLUDE_ASM(const s32, "field/fldPanel", func_00141158);

INCLUDE_ASM(const s32, "field/fldPanel", func_00141190);

INCLUDE_ASM(const s32, "field/fldPanel", func_001411C0);
