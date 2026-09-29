#include "common.h"

typedef struct WorldResourceOwner {
    u8 pad00[0x18];
    u32 *resource;
} WorldResourceOwner;

INCLUDE_ASM(const s32, "game/code_00116440", dds3SpawnInnerVecObj8);

u32 dds3GetResourceOwnerHandle(WorldResourceOwner *object) {
    return *object->resource;
}

INCLUDE_ASM(const s32, "game/code_00116440", func_001164C8);
