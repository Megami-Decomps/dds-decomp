#include "common.h"


extern u32 dds3CreateSlotResourceState(u32);

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
    u8 pad00[0xF];
    u8 kind; /* 0x0F selects which handle the object owns */
    u8 pad10[0x8];
    WorldInnerState *inner;
} WorldInnerOwner;

/* Each object kind keeps its handle in a different structure. */
s32 dds3GetObjectOwnedHandle(object)
    WorldInnerOwner *object;
{
    s32 handle;

    handle = 0;
    switch (object->kind - 4) {
    case 0: handle = dds3GetCameraHandle(object); break;
    case 1: handle = func_00113230(object); break;
    case 2: handle = func_00113F00(object); break;
    case 3: handle = effObjGetObjectHandle(object); break;
    case 4: handle = dds3GetResourceOwnerHandle(object); break;
    case 5: handle = func_00116800(object); break;
    }
    return handle;
}


void dds3SetOwnedWorldInnerValue(u32 unused, u32 value) {
    WorldInnerState *inner;

    inner = (WorldInnerState *)dds3GetObjectOwnedHandle();
    inner->value44 = value;
}

u32 dds3CreateWorldInnerState(WorldInnerOwner *object) {
    WorldInnerState *inner;
    u32 handle;

    effObjInnerCreate();
    inner = (WorldInnerState *)func_00328D68(0x90);
    object->inner = inner;
    handle = dds3CreateSlotResourceState((u32)object);
    inner->handle80 = handle;
    dds3SetObjectFlags(object, 0x62);
    inner->state88 = 0;
    return 1;
}
