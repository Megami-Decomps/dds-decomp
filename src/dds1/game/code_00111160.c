#include "common.h"

INCLUDE_ASM(const s32, "game/code_00111160", func_00111160);

INCLUDE_ASM(const s32, "game/code_00111160", func_00111188);

INCLUDE_ASM(const s32, "game/code_00111160", func_001111C8);

INCLUDE_ASM(const s32, "game/code_00111160", func_00111258);

INCLUDE_ASM(const s32, "game/code_00111160", func_001112A8);

INCLUDE_ASM(const s32, "game/code_00111160", func_001112C0);

INCLUDE_ASM(const s32, "game/code_00111160", func_00111388);

INCLUDE_ASM(const s32, "game/code_00111160", func_00111400);

void func_00111440(s32 arg0) {
    u32 *puVar1;

    puVar1 = *(u32 **)(arg0 + 0x18);
    func_001116F8();
    objExchangeSlot(*puVar1, 0, 1);
    func_002CFF98(puVar1);
}
