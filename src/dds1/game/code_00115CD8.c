#include "common.h"

extern u32 func_001117A8(u32);
extern u32 func_002CFEB8(u32);

INCLUDE_ASM(const s32, "game/code_00115CD8", func_00115CD8);

INCLUDE_ASM(const s32, "game/code_00115CD8", func_00115D08);

INCLUDE_ASM(const s32, "game/code_00115CD8", func_00115E10);

u32 dds3InitializeResourceOwner(u32 object) {
    u32 *valueSlot;
    u32 value;

    effObjInnerCreate();
    valueSlot = (u32 *)func_002CFEB8(0x10);
    *(u32 **)((s32)object + 0x18) = valueSlot;
    value = func_001117A8(object);
    *valueSlot = value;
    return 1;
}
