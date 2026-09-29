#include "common.h"

typedef struct WorldSlotObject {
    u8 pad00[0x18];
    u32 *slots;
} WorldSlotObject;

INCLUDE_ASM(const s32, "game/code_00111388", func_00111388);

INCLUDE_ASM(const s32, "game/code_00111388", evtSpawnActionObj2);

INCLUDE_ASM(const s32, "game/code_00111388", func_001113F0);

INCLUDE_ASM(const s32, "game/code_00111388", func_00111480);

/* Read a 32-bit value from the object's array of world slots. */
s32 dds3GetWorldSlotValue(u8 *object, s32 index) {
    return *(s32 *)(*(u8 **)(object + 0x18) + (index << 2));
}

INCLUDE_ASM(const s32, "game/code_00111388", func_001114E8);

INCLUDE_ASM(const s32, "game/code_00111388", func_001115B0);

INCLUDE_ASM(const s32, "game/code_00111388", func_00111628);

void dds3ReleaseWorldSlotResource(WorldSlotObject *object) {
    u32 *resource;

    resource = object->slots;
    dds3ReleaseObjectResource();
    dds3ExchangeSlot(*resource, 0, 1);
    func_00328E48(resource);
}
