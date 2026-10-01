#include "common.h"

extern u32 func_001119D0(u32);

extern u32 func_00328D68(u32);

/* The resource pointer is stored at +0x18 in both games. */
typedef struct WorldResourceOwner {
    u8 pad00[0x18];
    u32 *resource;
} WorldResourceOwner;

/* The record behind the owner's +0x18 pointer: a flag word, then the value
   pair at the end of the layout that a reset clears. */
typedef struct WorldResource {
    u8 pad00[4];
    u32 flags;
    u8 pad08[0x18];
    u32 field_0x20;
    u16 field_0x24;
    u16 field_0x26;
} WorldResource;

void func_00115F40(WorldResourceOwner *owner) {
    WorldResource *resource = (WorldResource *)owner->resource;

    resource->field_0x24 = 0;
    resource->field_0x20 = 0;
    resource->field_0x26 = 0;
    resource->flags &= ~4;
    resource->flags &= ~8;
}

INCLUDE_ASM(const s32, "game/code_00115F40", func_00115F70);

INCLUDE_ASM(const s32, "game/code_00115F40", func_00116078);

/* Attach the owner's newly allocated resource slot and store its handle. */
u32 dds3InitializeResourceOwner(WorldResourceOwner *object) {
    u32 *resource;
    u32 handle;

    effObjInnerCreate();
    resource = (u32 *)func_00328D68(0x10);
    object->resource = resource;
    handle = func_001119D0((u32)object);
    *resource = handle;
    return 1;
}
