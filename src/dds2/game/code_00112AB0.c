#include "common.h"
#include "eff_object.h"



extern s32 sdfAllocSizeClassBlock(u32);

typedef struct WorldInnerState {
    u8 pad00[0x80];
    ObjBase *unk80;
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
ObjBase *dds3GetObjectOwnedHandle(WorldInnerOwner *object) {
    ObjBase *handle;

    handle = 0;
    switch (object->kind - 4) {
    case 0: handle = (ObjBase *)dds3GetCameraHandle(object); break;
    case 1: handle = func_00113230((NodeA *)object); break;
    case 2: handle = (ObjBase *)effObjGetDataHandle(object); break;
    case 3: handle = (ObjBase *)effObjGetObjectHandle(object); break;
    case 4: handle = (ObjBase *)dds3GetResourceOwnerHandle(object); break;
    case 5: handle = (ObjBase *)func_00116800(object); break;
    }
    return handle;
}


void dds3SetOwnedWorldInnerValue(WorldInnerOwner *object, u32 value) {
    ObjBase *base;

    base = dds3GetObjectOwnedHandle(object);
    base->unk44 = value;
}

u32 dds3CreateWorldInnerState(WorldInnerOwner *object) {
    WorldInnerState *inner;
    ObjBase *objectBase;

    effObjInnerCreate();
    inner = (WorldInnerState *)sdfAllocSizeClassBlock(0x90);
    object->inner = inner;
    objectBase = dds3CreateSlotResourceState(object);
    inner->unk80 = objectBase;
    dds3SetObjectFlags(object, 0x62);
    inner->state88 = 0;
    return 1;
}
