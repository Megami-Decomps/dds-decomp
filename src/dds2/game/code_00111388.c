#include "common.h"

INCLUDE_ASM(const s32, "game/code_00111388", func_00111388);

INCLUDE_ASM(const s32, "game/code_00111388", func_001113B0);

INCLUDE_ASM(const s32, "game/code_00111388", func_001113F0);

INCLUDE_ASM(const s32, "game/code_00111388", func_00111480);

INCLUDE_ASM(const s32, "game/code_00111388", func_001114D0);

INCLUDE_ASM(const s32, "game/code_00111388", func_001114E8);

INCLUDE_ASM(const s32, "game/code_00111388", func_001115B0);

INCLUDE_ASM(const s32, "game/code_00111388", func_00111628);

void func_00111668(s32 arg0) {
    u32 *puVar1;

    puVar1 = *(u32 **)(arg0 + 0x18);
    dds3ReleaseObjectResource();
    dds3ExchangeSlot(*puVar1, 0, 1);
    func_00328E48(puVar1);
}
