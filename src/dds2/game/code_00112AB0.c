#include "common.h"
#include "eff_object.h"



extern void *sdfAllocSizeClassBlock(s32);

typedef struct WorldInnerState {
    u8 pad00[0x80];
    ObjBase *unk80;
    u8 pad84[4];
    u32 state88;
    u8 pad8C[4]; /* Complete native 0x90-byte allocation at 00112B80. */
} WorldInnerState;


/* Each object kind keeps its handle in a different structure. */
ObjBase *dds3GetObjectOwnedHandle(EffWorldNode *object) {
    ObjBase *handle;

    handle = 0;
    switch ((object->kindTag >> 24) - 4) {
    case 0: handle = dds3GetCameraHandle(object); break;
    case 1: handle = func_00113230(object); break;
    case 2: handle = (ObjBase *)effObjGetDataHandle(object); break;
    case 3: handle = (ObjBase *)effObjGetObjectHandle(object); break;
    case 4: handle = (ObjBase *)dds3GetResourceOwnerHandle(object); break;
    case 5: handle = (ObjBase *)func_00116800(object); break;
    }
    return handle;
}


void dds3SetOwnedWorldInnerValue(EffWorldNode *object, u32 value) {
    ObjBase *base;

    base = dds3GetObjectOwnedHandle(object);
    base->unk44 = value;
}

u32 dds3CreateWorldInnerState(EffWorldNode *object) {
    WorldInnerState *inner;
    ObjBase *objectBase;

    effObjInnerCreate();
    inner = sdfAllocSizeClassBlock(0x90);
    object->data = inner;
    objectBase = dds3CreateSlotResourceState(object);
    inner->unk80 = objectBase;
    dds3SetObjectFlags(object, 0x62);
    inner->state88 = 0;
    return 1;
}
