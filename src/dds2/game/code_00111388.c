#include "common.h"

typedef struct WorldSlotObject {
    u8 pad00[0x18];
    u32 *slots;
} WorldSlotObject;

INCLUDE_ASM(const s32, "game/code_00111388", func_00111388);

INCLUDE_ASM(const s32, "game/code_00111388", func_001113B0);

INCLUDE_ASM(const s32, "game/code_00111388", func_001113F0);

INCLUDE_ASM(const s32, "game/code_00111388", func_00111480);

s32 func_001114D0(u8 *obj, s32 index) {
    return *(s32 *)(*(u8 **)(obj + 0x18) + (index << 2));
}

INCLUDE_ASM(const s32, "game/code_00111388", func_001114E8);

INCLUDE_ASM(const s32, "game/code_00111388", func_001115B0);

INCLUDE_ASM(const s32, "game/code_00111388", func_00111628);

void func_00111668(WorldSlotObject *object) {
    u32 *resource;

    resource = object->slots;
    dds3ReleaseObjectResource();
    dds3ExchangeSlot(*resource, 0, 1);
    func_00328E48(resource);
}
