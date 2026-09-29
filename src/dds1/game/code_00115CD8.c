#include "common.h"

extern u32 func_001117A8(u32);
extern u32 func_002CFEB8(u32);

/* The resource pointer is stored at +0x18 in both games. */
typedef struct WorldResourceOwner {
    u8 pad00[0x18];
    u32 *resource;
} WorldResourceOwner;

INCLUDE_ASM(const s32, "game/code_00115CD8", func_00115CD8);

INCLUDE_ASM(const s32, "game/code_00115CD8", func_00115D08);

INCLUDE_ASM(const s32, "game/code_00115CD8", func_00115E10);

/* Attach the owner's newly allocated resource slot and store its handle. */
u32 dds3InitializeResourceOwner(WorldResourceOwner *object) {
    u32 *resource;
    u32 handle;

    effObjInnerCreate();
    resource = (u32 *)func_002CFEB8(0x10);
    object->resource = resource;
    handle = func_001117A8((u32)object);
    *resource = handle;
    return 1;
}
