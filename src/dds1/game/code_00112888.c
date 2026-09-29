#include "common.h"

typedef struct WorldInnerState {
    u8 pad00[0x44];
    u32 value44;
    u8 pad48[0x38];
    u32 handle80;
    u8 pad84[4];
    u32 state88;
} WorldInnerState;

typedef struct WorldInnerOwner {
    u8 pad00[0x18];
    WorldInnerState *inner;
} WorldInnerOwner;

extern u32 func_001117A8(u32);
extern s32 func_002CFEB8(u32);

extern s32 func_00112888(void);

INCLUDE_ASM(const s32, "game/code_00112888", func_00112888);

/* Store a value in the current world object's inner state. */
void func_00112930(u32 unused, u32 value) {
    WorldInnerState *inner;

    inner = (WorldInnerState *)func_00112888();
    inner->value44 = value;
}

/* Allocate the inner state and associate it with its world-object handle. */
u32 dds3CreateWorldInnerState(WorldInnerOwner *object) {
    WorldInnerState *inner;
    u32 handle;

    effObjInnerCreate();
    inner = (WorldInnerState *)func_002CFEB8(0x90);
    object->inner = inner;
    handle = func_001117A8((u32)object);
    inner->handle80 = handle;
    dds3SetObjectFlags(object, 0x62);
    inner->state88 = 0;
    return 1;
}
