#include "common.h"

INCLUDE_ASM(const s32, "game/code_00111160", func_00111160);

INCLUDE_ASM(const s32, "game/code_00111160", func_00111188);

INCLUDE_ASM(const s32, "game/code_00111160", func_001111C8);

INCLUDE_ASM(const s32, "game/code_00111160", func_00111258);

s32 func_001112A8(u8 *obj, s32 index) {
    return *(s32 *)(*(u8 **)(obj + 0x18) + (index << 2));
}

INCLUDE_ASM(const s32, "game/code_00111160", func_001112C0);

INCLUDE_ASM(const s32, "game/code_00111160", func_00111388);

INCLUDE_ASM(const s32, "game/code_00111160", func_00111400);

void func_00111440(s32 object) {
    u32 *slot;

    slot = *(u32 **)(object + 0x18);
    dds3ReleaseSlotPath();
    dds3ExchangeSlot(*slot, 0, 1);
    func_002CFF98(slot);
}
