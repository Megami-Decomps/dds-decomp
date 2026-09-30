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
    u8 pad00[0xF];
    u8 kind; /* 0x0F selects which handle the object owns */
    u8 pad10[0x8];
    WorldInnerState *inner;
} WorldInnerOwner;

extern u32 func_001117A8(u32);
extern s32 func_002CFEB8(u32);

/* Each object kind keeps its handle in a different structure. */
s32 func_00112888(object)
    WorldInnerOwner *object;
{
    s32 handle;

    handle = 0;
    switch (object->kind - 4) {
    case 0: handle = dds3GetCameraHandle(object); break;
    case 1: handle = func_00113008(object); break;
    case 2: handle = func_00113CD8(object); break;
    case 3: handle = effObjGetObjectHandle(object); break;
    case 4: handle = dds3GetResourceOwnerHandle(object); break;
    case 5: handle = func_00116598(object); break;
    }
    return handle;
}



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
