#include "common.h"

extern u32 D_00436204;

extern u64 func_00101958(void);

void func_001441C8(void) {
    u64 temp_v0;

    temp_v0 = func_00101958();
    func_00328E48(temp_v0);
    D_00436204 = 0;
}

INCLUDE_ASM(const s32, "field/fldPanel", func_001441F0);

INCLUDE_ASM(const s32, "field/fldPanel", func_00144238);

INCLUDE_ASM(const s32, "field/fldPanel", func_00144270);

INCLUDE_ASM(const s32, "field/fldPanel", func_001442A0);

INCLUDE_SDATA(const s32, "field/fldPanel", D_00436204);

