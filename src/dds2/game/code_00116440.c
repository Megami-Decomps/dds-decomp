#include "common.h"

typedef struct WorldResourceOwner {
    u8 pad00[0x18];
    u32 *resource;
} WorldResourceOwner;

INCLUDE_ASM(const s32, "game/code_00116440", func_00116440);

u32 func_001164B8(WorldResourceOwner *object) {
    return *object->resource;
}

INCLUDE_ASM(const s32, "game/code_00116440", func_001164C8);
