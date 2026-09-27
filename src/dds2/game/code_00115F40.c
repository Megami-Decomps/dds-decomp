#include "common.h"

extern u32 func_001119D0(u32);

extern u32 func_00328D68(u32);

INCLUDE_ASM(const s32, "game/code_00115F40", func_00115F40);

INCLUDE_ASM(const s32, "game/code_00115F40", func_00115F70);

INCLUDE_ASM(const s32, "game/code_00115F40", func_00116078);

u32 func_00116360(u32 arg0) {
    u32 *puVar1;
    u32 temp_v0;

    func_0010F798();
    puVar1 = (u32 *)func_00328D68(0x10);
    *(u32 **)((s32)arg0 + 0x18) = puVar1;
    temp_v0 = func_001119D0(arg0);
    *puVar1 = temp_v0;
    return 1;
}
