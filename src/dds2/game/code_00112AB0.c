#include "common.h"

extern s32 func_00112AB0(void);

extern u32 func_001119D0(u32);

extern s32 func_00328D68(u32);

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

INCLUDE_ASM(const s32, "game/code_00112AB0", func_00112AB0);

void func_00112B58(u32 unused, u32 value) {
    WorldInnerState *inner;

    inner = (WorldInnerState *)func_00112AB0();
    inner->value44 = value;
}

u32 dds3CreateWorldInnerState(WorldInnerOwner *object) {
    WorldInnerState *inner;
    u32 handle;

    effObjInnerCreate();
    inner = (WorldInnerState *)func_00328D68(0x90);
    object->inner = inner;
    handle = func_001119D0((u32)object);
    inner->handle80 = handle;
    dds3SetObjectFlags(object, 0x62);
    inner->state88 = 0;
    return 1;
}
